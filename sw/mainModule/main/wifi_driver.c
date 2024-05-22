#include "wifi_driver.h"

static const char *TAG = "mainBoard - wifi_driver.c";

/* --------------------- Definitions and static variables ------------------ */
// Root CA certificate (replace with the actual certificate)
extern const uint8_t server_root_ca_pem_start[] asm("_binary_isrgrootx1_pem_start");
extern const uint8_t server_root_ca_pem_end[] asm("_binary_isrgrootx1_pem_end");

static int s_retry_num = 0;
static SemaphoreHandle_t http_req_mutex;

static char http_req_buffer[MAX_HTTP_REQ_BUFFER] = {'\0'};





// Event handler for Wi-Fi events
static void event_handler(void* arg, esp_event_base_t event_base, int32_t event_id, void* event_data) 
{
    if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_START) {
        esp_wifi_connect();
    } else if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_DISCONNECTED) {
        if (s_retry_num < WIFI_CONN_MAX_RETRY) {
            esp_wifi_connect();
            s_retry_num++;
            ESP_LOGI(TAG, "retry to connect to the AP");
            STATUS_wifi = WIFIST_STARTUP;
        } else {
            ESP_LOGE(TAG, "connect to the AP fails");
            STATUS_wifi = WIFIST_ERROR;
        }
        ESP_LOGE(TAG,"connect to the AP fail");
    } else if (event_base == IP_EVENT && event_id == IP_EVENT_STA_GOT_IP) {
        ip_event_got_ip_t* event = (ip_event_got_ip_t*) event_data;
        ESP_LOGI(TAG, "got ip:" IPSTR, IP2STR(&event->ip_info.ip));
        s_retry_num = 0;
        STATUS_wifi = WIFIST_CONNECTED;
    }
}

// HTTP event handler
esp_err_t _http_event_handler(esp_http_client_event_t *evt) 
{
    switch(evt->event_id) {
        case HTTP_EVENT_ERROR:
            ESP_LOGI(TAG, "HTTP_EVENT_ERROR");
            break;
        case HTTP_EVENT_ON_CONNECTED:
            ESP_LOGI(TAG, "HTTP_EVENT_ON_CONNECTED");
            break;
        case HTTP_EVENT_HEADER_SENT:
            ESP_LOGI(TAG, "HTTP_EVENT_HEADER_SENT");
            break;
        case HTTP_EVENT_ON_HEADER:
            ESP_LOGI(TAG, "HTTP_EVENT_ON_HEADER, key=%s, value=%s", evt->header_key, evt->header_value);
            break;
        case HTTP_EVENT_ON_DATA:
            if (!esp_http_client_is_chunked_response(evt->client)) {
                ESP_LOGI(TAG, "HTTP_EVENT_ON_DATA, len=%d", evt->data_len);
                ESP_LOGI(TAG, "Response: %.*s", evt->data_len, (char*)evt->data);
            }
            break;
        case HTTP_EVENT_ON_FINISH:
            ESP_LOGI(TAG, "HTTP_EVENT_ON_FINISH");
            break;
        case HTTP_EVENT_DISCONNECTED:
            ESP_LOGI(TAG, "HTTP_EVENT_DISCONNECTED");
            break;
        case HTTP_EVENT_REDIRECT:
            ESP_LOGI(TAG, "HTTP_EVENT_REDIRECT");
            break;
    }
    return ESP_OK;
}

// hodne temp funkce
esp_err_t wifi_driver_routine()
{
    // wait for mutex because of static payload string
    xSemaphoreTake(http_req_mutex, portMAX_DELAY); 
     esp_http_client_config_t config = {
            .url = "https://akvaphp.charvot.cz/api",
            .event_handler = _http_event_handler,
            .cert_pem = (char *)server_root_ca_pem_start,  // Adding the root CA certificate
        };
        esp_http_client_handle_t client = esp_http_client_init(&config);

        esp_err_t err = esp_http_client_perform(client);
        if (err == ESP_OK) {
            ESP_LOGI(TAG, "HTTPS Status = %d, content_length = %lld",
                    esp_http_client_get_status_code(client),
                    esp_http_client_get_content_length(client));
            STATUS_wifi = WIFIST_ONLINE;
        } else {
            ESP_LOGE(TAG, "HTTP GET request failed: %s", esp_err_to_name(err));
            STATUS_wifi = WIFIST_CONNECTED;
        }

        err = esp_http_client_cleanup(client);
        xSemaphoreGive(http_req_mutex);  
        return err;
}

static esp_err_t _prepare_sensor_data_payload(const char* buffer_p, uint16_t* payload_len, uint8_t num_of_nodes, node_data_t* data)
{
    *payload_len = 0;
    int16_t n = 0;
    // snprintf return number of characters added (without null char at the end)
    n = snprintf(buffer_p, MAX_HTTP_REQ_BUFFER-*payload_len, "{\"mainUnitSN\":%lu,\"sensors\":[",SERIAL_NUMBER);
    if(n < 0 || n >= MAX_HTTP_REQ_BUFFER-*payload_len)
        return ESP_FAIL;
    *payload_len += n;
    
    // loop for each node data structure
    for(uint8_t i=0;i<num_of_nodes;i++)
    {
        node_data_t* curr_node = data+i;
        // loop for each data - index (here "j") is also dataType
        for(uint8_t j=0;j<curr_node->data_len;j++)
        {
            float* curr_data = (curr_node->data_p)+j;
            n = snprintf(buffer_p+*payload_len, MAX_HTTP_REQ_BUFFER-*payload_len, "{\"SN\":%lu,\"nodeType\":%u,\"dataType\":%u,\"value\":%f}",curr_node->can_node_p->SN,curr_node->can_node_p->node_type,j,*curr_data);
            if(n < 0 || n >= MAX_HTTP_REQ_BUFFER-*payload_len)
                return ESP_FAIL;
            *payload_len += n;
            // if not the last item, then insert comma
            if(i != num_of_nodes-1 || j != curr_node->data_len-1)
            {
                n = snprintf(buffer_p+*payload_len, MAX_HTTP_REQ_BUFFER-*payload_len, ",");
                if(n < 0 || n >= MAX_HTTP_REQ_BUFFER-*payload_len)
                    return ESP_FAIL;
                *payload_len += n;
            }
        }
    }
    n = snprintf(buffer_p+*payload_len, MAX_HTTP_REQ_BUFFER-*payload_len, "]}");
    if(n < 0 || n >= MAX_HTTP_REQ_BUFFER-*payload_len)
        return ESP_FAIL;
    *payload_len += n;
    // (*payload_len)++; // null character at the end
    
    return ESP_OK;
}


esp_err_t wifi_driver_send_sensor_data(uint8_t num_of_nodes, node_data_t* data)
{
    // wait for mutex because of static payload string
    xSemaphoreTake(http_req_mutex, portMAX_DELAY);  
     esp_http_client_config_t config = {
            .url = "https://akvaphp.charvot.cz/api/data/add",
            .event_handler = _http_event_handler,
            .cert_pem = (char *)server_root_ca_pem_start,  // Adding the root CA certificate
        };

        
        esp_http_client_handle_t client = esp_http_client_init(&config);

        esp_http_client_set_method(client, HTTP_METHOD_POST);
        esp_http_client_set_header(client, "Content-Type", "application/json");
        uint16_t req_len = 0u;
        _prepare_sensor_data_payload(&http_req_buffer,&req_len,num_of_nodes,data);
        printf("len %u: %s\n",req_len,http_req_buffer);
        esp_http_client_set_post_field(client, &http_req_buffer, req_len);

        esp_err_t err = esp_http_client_perform(client);
        if (err == ESP_OK) {
            ESP_LOGI(TAG, "HTTPS Status = %d, content_length = %lld",
                    esp_http_client_get_status_code(client),
                    esp_http_client_get_content_length(client));
        } else {
            ESP_LOGE(TAG, "HTTP GET request failed: %s", esp_err_to_name(err));
        }

        err = esp_http_client_cleanup(client);
        xSemaphoreGive(http_req_mutex);  
        return err;
}



// Initialize Wi-Fi
void wifi_init_sta(void) 
{
    esp_netif_init();
    esp_event_loop_create_default();
    esp_netif_create_default_wifi_sta();
    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    esp_wifi_init(&cfg);

    esp_event_handler_instance_t instance_any_id;
    esp_event_handler_instance_t instance_got_ip;
    esp_event_handler_instance_register(WIFI_EVENT, ESP_EVENT_ANY_ID, &event_handler, NULL, &instance_any_id);
    esp_event_handler_instance_register(IP_EVENT, IP_EVENT_STA_GOT_IP, &event_handler, NULL, &instance_got_ip);

    wifi_config_t wifi_config = {
        .sta = {
            .ssid = WIFI_SSID,
            .password = WIFI_PASS,
        },
    };
    esp_wifi_set_mode(WIFI_MODE_STA);
    esp_wifi_set_config(ESP_IF_WIFI_STA, &wifi_config);
    esp_wifi_start();
    http_req_mutex = xSemaphoreCreateMutex();
    // enable for tx
    xSemaphoreGive(http_req_mutex);
}
