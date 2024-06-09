#include "control_routines.h"

static const char *TAG = "mainBoard - control_routines.c";

static config_system_t system_config = {.relays = NULL};

static uint8_t can_num_address_given = 0u;
static can_node_t can_connected_nodes[CONFIG_MAX_CONNECTED_NODES];

static TickType_t control_routine_last = 0;
static TickType_t control_routine_period = pdMS_TO_TICKS(2000);

static TickType_t web_send_last = 0;
static TickType_t web_send_period = CONFIG_DEFAULT_WEB_DATA_SEND_PERIOD;

static void waitForPeriodPrecise(TickType_t last_start, TickType_t period)
{
    // calculate delay
    TickType_t correct_next_start = last_start + period;
    TickType_t now = xTaskGetTickCount();
    TickType_t delay = last_start + period - xTaskGetTickCount();
    if(last_start > now)
    {
        ESP_LOGE(TAG, "last start not set correctly");
        delay = pdMS_TO_TICKS(100);
    }
    else if(correct_next_start < now) // already late !
    {
        ESP_LOGE(TAG, "routine waiting too long");
        delay = 1;
    }
    else
    {
        delay = last_start + period - xTaskGetTickCount();
    }
    vTaskDelay(delay);
}

esp_err_t control_prepare_can_modules(TickType_t timeout_for_can)
{
    // initialize array before doing anathing
    for(uint8_t index=0; index<CONFIG_MAX_CONNECTED_NODES; index++)
    {
        INIT_CAN_NODE(can_connected_nodes[index]);
    }

    TickType_t start_ticks = xTaskGetTickCount();
    ESP_LOGI(TAG,"Waiting for CAN bus init");
    while (MODST_READY != STATUS_module_can)
    {
        // check timeout
        if(xTaskGetTickCount() > (start_ticks + timeout_for_can)) 
        {
            ESP_LOGE(TAG,"Waiting for CAN bus init -- timeouted");
            return ESP_ERR_TIMEOUT;
        }
        // get time to other tasks meanwhile
        vTaskDelay(5 / portTICK_PERIOD_MS);
    };

    ESP_LOGI(TAG,"Performing CAN addressing");
    if(ESP_OK != can_perform_addresing(&can_connected_nodes[0],&can_num_address_given))
        ESP_LOGE(TAG,"addressing failed ...");
    ESP_LOGI(TAG,"Found %u devices on the bus.\n",can_num_address_given);

    if(ESP_OK != can_poll_nodes_sn(&can_connected_nodes[0],can_num_address_given))
        ESP_LOGE(TAG,"polling sn failed ...");

    if(ESP_OK != can_poll_nodes_status(&can_connected_nodes[0],can_num_address_given))
        ESP_LOGE(TAG,"polling status failed ...");

    if(ESP_OK != can_poll_nodes_type(&can_connected_nodes[0],can_num_address_given))
        ESP_LOGE(TAG,"polling node type failed ...");

    if(ESP_OK != config_load_nvm_all(&system_config, &can_connected_nodes[0],can_num_address_given))
        ESP_LOGE(TAG,"load all config failed ...");

    return ESP_OK;
}

static esp_err_t prepare_node_data(node_data_t* node_data, uint8_t expected_size)
{
    if(expected_size != node_data->data_len)
    {
        if(NULL != node_data->data_p)
            vPortFree(node_data->data_p);
        
        node_data->data_len = expected_size;
    }
    if(NULL == node_data->data_p)
    {
        node_data->data_p = (float*)pvPortMalloc(expected_size*sizeof(float));
        for(uint8_t i=0; i<expected_size;i++) {node_data->data_p[i] = 0.0f;}
    }

    if(NULL == node_data->data_p)
        return ESP_FAIL;
    
    return ESP_OK;
}

esp_err_t poll_nodes_data()
{
    if(can_num_address_given == CONFIG_MAX_CONNECTED_NODES)
        return ESP_FAIL;

    esp_err_t ret = ESP_OK;
    for(uint8_t index=0; index<can_num_address_given; index++)
    {
        // proccess only nodes with OK status 
        if(NODEST_NORMAL != can_connected_nodes[index].status)
            continue; // try another node

        uint8_t adr = can_connected_nodes[index].can_address;
        if(0 == adr)
        {
            ret |= ESP_FAIL;
            ESP_LOGE(TAG,"Node with index %u has no address \n",index);
            continue; // try another node
        }
        node_data_t* node_data_p = &(can_connected_nodes[index].data);
        switch (can_connected_nodes[index].node_type)
        {
            case NODE_TYPE_TEMP_SENSOR:
                if(ESP_OK == prepare_node_data(node_data_p, 1))
                    /*
                    [0] Temperature 
                    */
                    ret |= temp_sensor_get_temperature(&(can_connected_nodes[index]),&(node_data_p->data_p[0]));
                break;

            case NODE_TYPE_WATER_LEVEL_SENSOR:
                if(ESP_OK == prepare_node_data(node_data_p, 2))
                {
                    /*
                    [0] % of water 
                    [1] Boye state
                    */
                    ret |= water_level_sensor_get_data(&(can_connected_nodes[index]),&(node_data_p->data_p[0]),&(node_data_p->data_p[1]));
                }
                break;

            case NODE_TYPE_LED_BOARD:
            case NODE_TYPE_PH_SENSOR:
                break;
        
            default:
                ESP_LOGE(TAG,"Cannot poll data from module with type %d \n", can_connected_nodes[index].node_type);
                ret |= ESP_FAIL;
                break;
        }
    }
    return ret;
}

esp_err_t print_nodes_data()
{
    esp_err_t ret = ESP_OK;
    for(uint8_t index=0; index<can_num_address_given; index++)
    {
        node_data_t* node_data_p = &(can_connected_nodes[index].data);
        switch (can_connected_nodes[index].node_type)
        {
            case NODE_TYPE_LED_BOARD:
                
                break;

            case NODE_TYPE_TEMP_SENSOR:
                if(1 == node_data_p->data_len)
                {
                    printf("Temperature: %f \n",node_data_p->data_p[0]);
                }

                break;

            case NODE_TYPE_WATER_LEVEL_SENSOR:
                if(2 == node_data_p->data_len)
                {
                    printf("WL: %f, Boye: %f \n",node_data_p->data_p[0],node_data_p->data_p[0]);
                }

            case NODE_TYPE_PH_SENSOR:
                break;
        
            default:
                ESP_LOGE(TAG,"Cannot print data from module with type %d \n", can_connected_nodes[index].node_type);
                ret |= ESP_FAIL;
                break;
        }
    }
    return ret;
}

static esp_err_t update_config()
{
    // if it is neccesarry, upadate config from web
    if(config_is_obsolete())
    {
        if(WIFIST_ONLINE == STATUS_wifi && ESP_OK == config_update_from_web())
        {
            ESP_LOGI(TAG,"Config updated form the web, reloading nvm... \n");
            // reload config
            if(ESP_OK != config_load_nvm_all(&system_config, &can_connected_nodes[0],can_num_address_given))
            {
                ESP_LOGE(TAG,"load config from nvm failed ...");
                return ESP_FAIL;
            }
        }
        else
        {
            ESP_LOGE(TAG,"config update from web failed \n");
            return ESP_FAIL;
        }
    }
    return ESP_OK;
}

esp_err_t process_config()
{
    // get RTC if possible
    simple_time_t time_now;
    bool use_rtc = false;
    if(ESP_OK == is_time_set() && ESP_OK == time_now_simple(&time_now))
    {
        // double check for sure...
        if(1900 + time_now.year > 2020)
        {
            use_rtc = true;
        }
    }

    esp_err_t ret = ESP_OK;
    alarm_status_t alarm1_flag = ALARMST_OK;
    alarm_status_t alarm2_flag = ALARMST_OK;
    // process config for aech connected node
    for(uint8_t i = 0; i < can_num_address_given;i++)
    {
        if(NULL != can_connected_nodes[i].config)
        {
            switch (can_connected_nodes[i].node_type)
            {
                case NODE_TYPE_LED_BOARD:
                    led_board_process_config(&(can_connected_nodes[i]),time_now, use_rtc);
                    break;
                case NODE_TYPE_PH_SENSOR:
                    /* code */
                    break;
                case NODE_TYPE_TEMP_SENSOR:
                    temp_sensor_process_config(&(can_connected_nodes[i]),&alarm1_flag,&alarm2_flag);
                    break;
                case NODE_TYPE_WATER_LEVEL_SENSOR:
                    /* code */
                    break;
                        
                default:
                    ret |=ESP_FAIL;
                    break;
            }
        }
    }
    // update alarms 
    STATUS_alarm1 = alarm1_flag;
    STATUS_alarm2 = alarm2_flag;

    //process relays 
    ret |= relays_process_config(system_config.relays,time_now, use_rtc);

    return ret;
}


esp_err_t control_routine()
{
    /*
    STAGES:
        - wait for period start
        - update entire system config (if neccesarry)
        - poll modules status 
        - poll modules data
        - send data to web 
        - process modules configs 
            - get RTC
            - set or unset alarms
        - process system config 
            - update relays state

    */
    
    // - wait for period start
    waitForPeriodPrecise(control_routine_last, control_routine_period);
    control_routine_last = xTaskGetTickCount();
    
    // - update entire system config (if neccesarry)
    if(ESP_OK != update_config())
        ESP_LOGE(TAG,"update config failed ...");
    // - poll modules status 
    if(ESP_OK != can_poll_nodes_status(&can_connected_nodes[0],can_num_address_given))
        ESP_LOGE(TAG,"polling status failed ...");
    
    // - poll modules data
    if(ESP_OK != poll_nodes_data())
        ESP_LOGE(TAG,"polling status failed ...");
    print_nodes_data();
    
    // - send data to web 
    if(WIFIST_ONLINE == STATUS_wifi)
    {
        if(xTaskGetTickCount() > (web_send_last + web_send_period))
        {
            web_send_last = xTaskGetTickCount();
            if(ESP_OK != wifi_driver_send_sensor_data(&can_connected_nodes[0],can_num_address_given))
                ESP_LOGE(TAG,"Sending data to web failed ...");
        }
    }
    // - process modules configs 
    //     - get RTC
    //     - set or unset alarms
    // - process system config 
    //     - update relays state
    if(ESP_OK != process_config())
        ESP_LOGE(TAG,"Processing config failed ...");

    return ESP_OK;
}