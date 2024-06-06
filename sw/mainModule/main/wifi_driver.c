#include "wifi_driver.h"

static const char *TAG = "mainBoard - wifi_driver.c";

/* --------------------- Definitions and static variables ------------------ */
// Root CA certificate (replace with the actual certificate)
extern const uint8_t server_root_ca_pem_start[] asm("_binary_isrgrootx1_pem_start");
extern const uint8_t server_root_ca_pem_end[] asm("_binary_isrgrootx1_pem_end");

static int s_retry_num = 0;
static SemaphoreHandle_t http_req_mutex;
static SemaphoreHandle_t http_res_saved_mutex;
SemaphoreHandle_t wifi_conn_check_sem;

static char http_req_buffer[MAX_HTTP_REQ_BUFFER] = {'\0'};
static char http_res_buffer[MAX_HTTP_RECV_BUFFER] = {0};
static uint16_t http_res_buffer_len = 0u;





// Event handler for Wi-Fi events
static void event_handler(void* arg, esp_event_base_t event_base, int32_t event_id, void* event_data) 
{
    if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_START) {
        esp_wifi_connect();
        STATUS_wifi = WIFIST_STARTUP;
    } else if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_DISCONNECTED) {
        ESP_LOGE(TAG,"connect to the AP fail");
        STATUS_wifi = WIFIST_ERROR;
        if (s_retry_num < WIFI_CONN_MAX_RETRY) {
            esp_wifi_connect();
            s_retry_num++;
            ESP_LOGI(TAG, "retry to connect to the AP");
            // STATUS_wifi = WIFIST_STARTUP;
        } else {
            ESP_LOGE(TAG, "WIFI error: Wait 30 s before next retries");
            vTaskDelay(30000/portTICK_PERIOD_MS);
            s_retry_num = 0; // reset retries
            esp_wifi_connect();
        }
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
    #ifdef WIFI_DEBUG_PRINT_HTTP
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
            // if (!esp_http_client_is_chunked_response(evt->client)) {
            //     ESP_LOGI(TAG, "HTTP_EVENT_ON_DATA, len=%d", evt->data_len);
            //     ESP_LOGI(TAG, "Response: %.*s", evt->data_len, (char*)evt->data);
            // }
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
    #endif // WIFI_DEBUG_PRINT_HTTP
    switch(evt->event_id) {
        case HTTP_EVENT_ON_DATA:
            if (!esp_http_client_is_chunked_response(evt->client)) {
                ESP_LOGI(TAG, "HTTP_EVENT_ON_DATA, len=%d", evt->data_len);
                ESP_LOGI(TAG, "Response: %.*s", evt->data_len, (char*)evt->data);
                // check if buffer has enough space
                if(http_res_buffer_len + evt->data_len < MAX_HTTP_RECV_BUFFER)
                {
                    memcpy(&http_res_buffer[http_res_buffer_len],(char*)evt->data,evt->data_len);
                    http_res_buffer_len += evt->data_len;
                }
            }
            break;
        case HTTP_EVENT_ON_FINISH:
            ESP_LOGI(TAG, "HTTP_EVENT_ON_FINISH");
            http_res_buffer[http_res_buffer_len] = '\0'; // terminate string if not terminated
            http_res_buffer_len = 0u;   // clear counter
            xSemaphoreGive(http_res_saved_mutex);
            break;
        default:
    }
    
    return ESP_OK;
}


static TickType_t wifi_conn_check_last = 0;
static TickType_t wifi_conn_check_period = CONFIG_DEFAULT_WIFI_CONN_CHECK_PER;

static TickType_t wifi_config_check_last = 0;
static TickType_t wifi_config_check_period = CONFIG_DEFAULT_WIFI_CONFIG_CHECK_PER;

static TickType_t wifi_ntp_check_last = 0;
static TickType_t wifi_ntp_check_period = CONFIG_DEFAULT_WIFI_NTP_CHECK_PER;

esp_err_t wifi_driver_routine()
{
    // 2000 ms is blocking delay when other tasks could do their stuff
    if(xSemaphoreTake(wifi_conn_check_sem, pdMS_TO_TICKS(2000)) || xTaskGetTickCount() > (wifi_conn_check_last + wifi_conn_check_period))
    {
        wifi_conn_check_last = xTaskGetTickCount();
        wifi_driver_check_connection();
    }

    if(WIFIST_ONLINE == STATUS_wifi && xTaskGetTickCount() > (wifi_config_check_last + wifi_config_check_period))
    {
        uint64_t temp_ver = 0;
        if(ESP_OK == wifi_driver_check_config_version(&temp_ver))
        {
            wifi_config_check_last = xTaskGetTickCount(); // clear counter only after success
            CONFIG_VERSION_from_web = temp_ver;
            ESP_LOGI(TAG,"Config version from web: %llu \n",CONFIG_VERSION_from_web);
        }
    }

    if(WIFIST_ONLINE == STATUS_wifi && ( ESP_OK != is_time_set() || xTaskGetTickCount() > (wifi_ntp_check_last + wifi_ntp_check_period)))
    {
        wifi_ntp_check_last = xTaskGetTickCount();
        sync_ntp_time();
        printf("Is time set? : %d \n",is_time_set());
        print_time();
    }

    // change periods
    if(WIFIST_ONLINE == STATUS_wifi)
        wifi_conn_check_period = CONFIG_MAX_WIFI_CONN_CHECK_PER;
    else
        wifi_conn_check_period = CONFIG_DEFAULT_WIFI_CONN_CHECK_PER;
    
    return ESP_OK;
}

esp_err_t wifi_driver_check_connection()
{
    // wait for mutex because of static payload string
    xSemaphoreTake(http_req_mutex, portMAX_DELAY); 
     esp_http_client_config_t config = {
            .url = "https://akvaphp.charvot.cz/api/ping",
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
            if(WIFIST_ONLINE == STATUS_wifi)
                STATUS_wifi = WIFIST_CONNECTED;
        }

        err = esp_http_client_cleanup(client);
        xSemaphoreGive(http_req_mutex);  
        return err;
}

static esp_err_t _prepare_sensor_data_payload(const char* buffer_p, uint16_t* payload_len, can_node_t* can_connected_nodes, uint8_t can_num_address_given)
{
    *payload_len = 0;
    int16_t n = 0;
    // snprintf return number of characters added (without null char at the end)
    n = snprintf(buffer_p, MAX_HTTP_REQ_BUFFER-*payload_len, "{\"mainUnitSN\":%lu,\"sensors\":[",SERIAL_NUMBER);
    if(n < 0 || n >= MAX_HTTP_REQ_BUFFER-*payload_len)
        return ESP_FAIL;
    *payload_len += n;
    
    // loop for each node data structure
    bool first_item = true;
    bool first_data = true;
    for(uint8_t i=0;i<can_num_address_given;i++)
    {
        // send only data from OK nodes 
        if(NODEST_NORMAL != can_connected_nodes[i].status)
            continue;

        if(first_item)
        {
            first_item = false;
        }
        else
        {
            // if not the first item, then begin with comma
            n = snprintf(buffer_p+*payload_len, MAX_HTTP_REQ_BUFFER-*payload_len, ",");
            if(n < 0 || n >= MAX_HTTP_REQ_BUFFER-*payload_len)
                return ESP_FAIL;
            *payload_len += n;
        }
        node_data_t* curr_node = &(can_connected_nodes[i].data);

        // loop for each data - index (here "j") is also dataType
        for(uint8_t j=0;j<curr_node->data_len;j++)
        {
            if(first_data)
            {
                first_data = false;
            }
            else
            {
                // if not the first data, then begin with comma
                n = snprintf(buffer_p+*payload_len, MAX_HTTP_REQ_BUFFER-*payload_len, ",");
                if(n < 0 || n >= MAX_HTTP_REQ_BUFFER-*payload_len)
                    return ESP_FAIL;
                *payload_len += n;
            }

            float* curr_data = (curr_node->data_p)+j;
            n = snprintf(buffer_p+*payload_len, MAX_HTTP_REQ_BUFFER-*payload_len, "{\"SN\":%lu,\"nodeType\":%u,\"dataType\":%u,\"value\":%f}",can_connected_nodes[i].SN,can_connected_nodes[i].node_type,j,*curr_data);
            if(n < 0 || n >= MAX_HTTP_REQ_BUFFER-*payload_len)
                return ESP_FAIL;
            *payload_len += n;
        }
        first_data = true; // reset boolean
    }
    n = snprintf(buffer_p+*payload_len, MAX_HTTP_REQ_BUFFER-*payload_len, "]}");
    if(n < 0 || n >= MAX_HTTP_REQ_BUFFER-*payload_len)
        return ESP_FAIL;
    *payload_len += n;

    
    return ESP_OK;
}


esp_err_t wifi_driver_send_sensor_data(can_node_t* can_connected_nodes, uint8_t can_num_address_given)
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
        _prepare_sensor_data_payload(&http_req_buffer,&req_len,can_connected_nodes,can_num_address_given);
        // printf("len %u: %s\n",req_len,http_req_buffer);
        esp_http_client_set_post_field(client, &http_req_buffer, req_len);

        esp_err_t err = esp_http_client_perform(client);
        if (err == ESP_OK) {
            ESP_LOGI(TAG, "HTTPS Status = %d, content_length = %lld",
                    esp_http_client_get_status_code(client),
                    esp_http_client_get_content_length(client));
        } else {
            ESP_LOGE(TAG, "HTTP GET request failed: %s", esp_err_to_name(err));
            xSemaphoreGive(wifi_conn_check_sem); // force ping request for connection check
        }

        err = esp_http_client_cleanup(client);
        xSemaphoreGive(http_req_mutex);  
        return err;
}

esp_err_t wifi_driver_get_system_config(cJSON** configJSON)
{
    // wait for mutex because of static payload string
    xSemaphoreTake(http_req_mutex, portMAX_DELAY);  
    char url_buffer[256];
    const char *base_url = "https://akvaphp.charvot.cz/api/configuration/get?mainUnitSN=";
    // const char *base_url = "https://akvaphp.charvot.cz/api/ping?mainUnitSN=";
    sprintf(url_buffer, "%s%" PRIu32, base_url, SERIAL_NUMBER);

    esp_http_client_config_t config = {
        .url = url_buffer,
        .event_handler = _http_event_handler,
        .cert_pem = (char *)server_root_ca_pem_start,  // Adding the root CA certificate
        .user_data = http_res_buffer
    };

    printf("%s \n",config.url);
    esp_http_client_handle_t client = esp_http_client_init(&config);

    // clear mutex before calling
    xSemaphoreTake(http_res_saved_mutex,0);
    esp_err_t err = esp_http_client_perform(client);
    if (err == ESP_OK) {
        int status_code = esp_http_client_get_status_code(client);
        int64_t resp_len = esp_http_client_get_content_length(client);  
        ESP_LOGI(TAG, "HTTPS Status = %d, content_length = %lld",status_code,resp_len);

        if(200 == status_code && resp_len > 0)
        {
            // wait for event handler to save the response or timeout
            if(xSemaphoreTake(http_res_saved_mutex,2000/portTICK_PERIOD_MS))
            {
                // printf("len %lld: %.*s\n",resp_len,(int)resp_len,&http_res_buffer[0]);
                *configJSON = cJSON_Parse(&http_res_buffer[0]);
            }
            else // timeout
            {
                err = ESP_FAIL;
            }
        }
        else{
            err = ESP_FAIL;
        }
    } else {
        ESP_LOGE(TAG, "HTTP GET request failed: %s", esp_err_to_name(err));
        xSemaphoreGive(wifi_conn_check_sem); // force ping request for connection check
    }

    err |= esp_http_client_cleanup(client);
    xSemaphoreGive(http_req_mutex);  
    return err;
}

esp_err_t wifi_driver_check_config_version(uint64_t* configVersion)
{
    // wait for mutex because of static payload string
    xSemaphoreTake(http_req_mutex, portMAX_DELAY);  
    char url_buffer[256];
    const char *base_url = "https://akvaphp.charvot.cz/api/ping?mainUnitSN=";
    sprintf(url_buffer, "%s%" PRIu32, base_url, SERIAL_NUMBER);

    esp_http_client_config_t config = {
        .url = url_buffer,
        .event_handler = _http_event_handler,
        .cert_pem = (char *)server_root_ca_pem_start,  // Adding the root CA certificate
        .user_data = http_res_buffer
    };

    printf("%s \n",config.url);
    esp_http_client_handle_t client = esp_http_client_init(&config);

    // clear mutex before calling
    xSemaphoreTake(http_res_saved_mutex,0);
    esp_err_t err = esp_http_client_perform(client);
    if (err == ESP_OK) {
        int status_code = esp_http_client_get_status_code(client);
        int64_t resp_len = esp_http_client_get_content_length(client);  
        ESP_LOGI(TAG, "HTTPS Status = %d, content_length = %lld",status_code,resp_len);

        if(200 == status_code && resp_len > 0)
        {
            // wait for event handler to save the response or timeout
            if(xSemaphoreTake(http_res_saved_mutex,2000/portTICK_PERIOD_MS))
            {
                // printf("len %lld: %.*s\n",resp_len,(int)resp_len,&http_res_buffer[0]);
                cJSON* response = cJSON_Parse(&http_res_buffer[0]);
                if(NULL != response)
                {
                    cJSON* lastConfigVersion = cJSON_GetObjectItem(response,"lastConfigVersion");
                    if(NULL != lastConfigVersion)
                        *configVersion = lastConfigVersion->valueint;
                    else 
                        err = ESP_FAIL;
                }
                else
                    err = ESP_FAIL;
            }
            else // timeout
            {
                err = ESP_FAIL;
            }
        }
        else{
            err = ESP_FAIL;
        }
    } else {
        ESP_LOGE(TAG, "HTTP GET request failed: %s", esp_err_to_name(err));
        xSemaphoreGive(wifi_conn_check_sem); // force ping request for connection check
    }

    err |= esp_http_client_cleanup(client);
    xSemaphoreGive(http_req_mutex);  
    return err;
}

// Initialize Wi-Fi
void wifi_init_sta(void) 
{
    wifi_conn_check_sem = xSemaphoreCreateBinary();
    http_req_mutex = xSemaphoreCreateMutex();
    http_res_saved_mutex = xSemaphoreCreateBinary();

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
    // enable for tx
    xSemaphoreGive(http_req_mutex);
}
