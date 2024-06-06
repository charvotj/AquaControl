#include "config_manager.h"

static const char *TAG = "mainBoard - config_manager.c";
static uint64_t CONFIG_VERSION_from_nvm = 0u;
uint64_t CONFIG_VERSION_from_web = 0u;
SemaphoreHandle_t config_control_sem;

esp_err_t config_manager_init()
{
    config_control_sem = xSemaphoreCreateMutex();
    xSemaphoreGive(config_control_sem);
    return ESP_OK;
}

bool config_is_obsolete()
{
    printf("comparing: %llu from web, %llu from nvm \n",CONFIG_VERSION_from_web,CONFIG_VERSION_from_nvm);
    return (CONFIG_VERSION_from_web > CONFIG_VERSION_from_nvm);
}

static esp_err_t parse_time_hhmm(const char* time_str, uint8_t* hours, uint8_t* minutes) 
{
    int temp_hours, temp_minutes;
    // Docs: On success, the function returns the number of variables filled. In the case of an input failure before any data could be successfully read, EOF is returned.
    if(2 != sscanf(time_str, "%d:%d", &temp_hours, &temp_minutes))
        return ESP_FAIL;

    // Ensure values are within valid range for hours and minutes
    if (temp_hours >= 0 && temp_hours < 24 && temp_minutes >= 0 && temp_minutes < 60) {
        *hours = (uint8_t)temp_hours;
        *minutes = (uint8_t)temp_minutes;
        return ESP_OK;
    }
    return ESP_FAIL;
}

static esp_err_t parse_relays_cfg(cJSON* relaysConfig, config_relay_t* rel_cfg_arr)
{
    for (uint8_t i = 0; i < RELAYS_NUM; i++) 
    {
        cJSON* relay = cJSON_GetArrayItem(relaysConfig, i);
        if(NULL == relay) goto jsonError; // prevent memory leak
        cJSON* manual = cJSON_GetObjectItem(relay, "manual");
        cJSON* timerOn = cJSON_GetObjectItem(relay, "timerOn");
        cJSON* timerOff = cJSON_GetObjectItem(relay, "timerOff");
        cJSON* manualState = cJSON_GetObjectItem(relay, "manualState");
        if(NULL == manual || NULL == timerOn || NULL == timerOff || NULL == manualState) goto jsonError; // prevent memory leak
        rel_cfg_arr[i].manual               = (bool)manual->valueint;
        rel_cfg_arr[i].manual_state         = (bool)manualState->valueint;
        if(ESP_OK != parse_time_hhmm(timerOn->valuestring,
            &(rel_cfg_arr[i].timer_on_hours),
            &(rel_cfg_arr[i].timer_on_minutes))) goto jsonError;
        if(ESP_OK != parse_time_hhmm(timerOff->valuestring,
            &(rel_cfg_arr[i].timer_off_hours),
            &(rel_cfg_arr[i].timer_off_minutes))) goto jsonError;
        
        printf("Relay %d - Manual: %d, TimerOn: %s, TimerOff: %s, ManualState: %d\n", 
            i, manual->valueint, timerOn->valuestring, timerOff->valuestring, manualState->valueint);
    }
    return ESP_OK;
    // error labels:
jsonError:
    return ESP_FAIL;
}

static esp_err_t parse_alarm_cfg(cJSON* alarmConfig, config_alarm_t* alarm_cfg)
{
    cJSON* active   = cJSON_GetObjectItem(alarmConfig, "active");
    cJSON* maxValue = cJSON_GetObjectItem(alarmConfig, "maxValue");
    cJSON* minValue = cJSON_GetObjectItem(alarmConfig, "minValue");
    if(NULL == active || NULL == maxValue || NULL == minValue) goto jsonError; // prevent memory leak

    alarm_cfg->active = (bool)active->valueint;
    alarm_cfg->max_value = (float)maxValue->valuedouble;
    alarm_cfg->min_value = (float)minValue->valuedouble;

    //  printf("Alarm - Active: %d, MaxValue: %f, MinValue: %f\n", 
    //            active->valueint , maxValue->valuedouble, minValue->valuedouble);
    return ESP_OK;
    // error labels:
jsonError:
    return ESP_FAIL;
}

static esp_err_t parse_module_cfg_led_board(cJSON* moduleConfig, config_module_led_board_t* module_cfg)
{
    cJSON* ledStrips = cJSON_GetObjectItem(moduleConfig, "ledStrips");
    if(NULL == ledStrips) goto jsonError; // prevent memory leak
    if(2 != cJSON_GetArraySize(ledStrips)) goto jsonError; // there should be two led strips
    for (uint8_t i = 0; i < 2; i++) {
        cJSON* ledStrip = cJSON_GetArrayItem(ledStrips, i);
        if(NULL == ledStrips) goto jsonError; // prevent memory leak
        cJSON* startTime = cJSON_GetObjectItem(ledStrip, "startTime");
        cJSON* endTime = cJSON_GetObjectItem(ledStrip, "endTime");
        cJSON* riseTime = cJSON_GetObjectItem(ledStrip, "riseTime");
        cJSON* fallTime = cJSON_GetObjectItem(ledStrip, "fallTime");
        cJSON* intensity = cJSON_GetObjectItem(ledStrip, "intensity");
        if(NULL == endTime || NULL == fallTime || NULL == riseTime || NULL == intensity || NULL == startTime) goto jsonError; // prevent memory leak
        config_ledstrip_t* curr_led_strip;
        if(0 == i)
            curr_led_strip = &(module_cfg->ledstrip_0);
        else
            curr_led_strip = &(module_cfg->ledstrip_1);
        
        if(ESP_OK != parse_time_hhmm(startTime->valuestring,
            &(curr_led_strip->start_time_hours),
            &(curr_led_strip->start_time_minutes))) goto jsonError;
        if(ESP_OK != parse_time_hhmm(endTime->valuestring,
            &(curr_led_strip->end_time_hours),
            &(curr_led_strip->end_time_minutes))) goto jsonError;
        curr_led_strip->rise_time = riseTime->valueint;
        curr_led_strip->fall_time = fallTime->valueint;
        curr_led_strip->intensity = intensity->valueint;
        // printf("LedStrip %d - EndTime: %s, FallTime: %d, RiseTime: %d, Intensity: %d, StartTime: %s\n", 
            // j, endTime->valuestring, fallTime->valueint, riseTime->valueint, intensity->valueint, startTime->valuestring);
    }
    return ESP_OK;
    // error labels:
jsonError:
    return ESP_FAIL;
}

static esp_err_t parse_module_cfg_temp_sens(cJSON* moduleConfig, config_module_temp_sens_t* module_cfg)
{
    cJSON* alarm = cJSON_GetObjectItem(moduleConfig, "alarm");
    if(NULL == alarm) goto jsonError; // prevent memory leak
    
    return parse_alarm_cfg(alarm,&(module_cfg->alarm_cfg));
    // error labels:
jsonError:
    return ESP_FAIL;
}

static esp_err_t parse_module_cfg_wl_sens(cJSON* moduleConfig, config_module_wl_sens_t* module_cfg)
{
    cJSON* alarm = cJSON_GetObjectItem(moduleConfig, "alarm");
    if(NULL == alarm) goto jsonError; // prevent memory leak
    
    return parse_alarm_cfg(alarm,&(module_cfg->alarm_cfg));
    // error labels:
jsonError:
    return ESP_FAIL;
}

static esp_err_t parse_module_cfg_ph_sens(cJSON* moduleConfig, config_module_ph_sens_t* module_cfg)
{
    cJSON* alarm = cJSON_GetObjectItem(moduleConfig, "alarm");
    if(NULL == alarm) goto jsonError; // prevent memory leak
    
    return parse_alarm_cfg(alarm,&(module_cfg->alarm_cfg));
    // error labels:
jsonError:
    return ESP_FAIL;
}

static esp_err_t config_update_version_nvm(uint64_t config_version)
{
    printf("Updating cfg version in nvm to %llu \n", config_version);
    nvs_handle_t my_handle;
    esp_err_t ret;

    if(0 == config_version)
    {
        ESP_LOGE(TAG,"Config version 0 would not be saved to nvm");
        return ESP_FAIL;
    }

    ret = nvs_open(NVS_NAMESPACE_SYSTEM_CONFIG, NVS_READWRITE, &my_handle);
    if(ESP_OK != ret) return ret;

    // critical section
    xSemaphoreTake(config_control_sem, portMAX_DELAY);
    size_t nvs_size = sizeof(uint64_t);   
    char* nvs_key = "cfgVersion\0";
    printf("NVS set, key: %s\n", nvs_key);
    ret |= nvs_set_blob(my_handle,nvs_key,&config_version,nvs_size);

    if(ESP_OK == ret)
    {
        ret |= nvs_commit(my_handle);
        printf("NVS commit \n");
    }
    nvs_close(my_handle);
    xSemaphoreGive(config_control_sem); // release mutex
    return ret;
}

static esp_err_t config_get_version_from_nvm(uint64_t* config_version)
{
    nvs_handle_t my_handle;
    esp_err_t ret;

    if(NULL == config_version)
    {
        ESP_LOGE(TAG,"Config version cannot be loaded, null is provided \n");
        return ESP_FAIL;
    }

    ret = nvs_open(NVS_NAMESPACE_SYSTEM_CONFIG, NVS_READWRITE, &my_handle);
    if(ESP_OK != ret) return ret;

    // critical section
    xSemaphoreTake(config_control_sem, portMAX_DELAY);
    size_t nvs_size = sizeof(uint64_t);   
    char* nvs_key = "cfgVersion\0";
    printf("NVS get, key: %s\n", nvs_key);
    ret |= nvs_get_blob(my_handle,nvs_key,config_version,&nvs_size);

    nvs_close(my_handle);
    xSemaphoreGive(config_control_sem); // release mutex
    return ret;
}

static esp_err_t config_system_save_to_nvm(config_system_t* system_config)
{
    nvs_handle_t my_handle;
    esp_err_t ret;

    ret = nvs_open(NVS_NAMESPACE_SYSTEM_CONFIG, NVS_READWRITE, &my_handle);
    if(ESP_OK != ret) return ret;

    // critical section
    xSemaphoreTake(config_control_sem, portMAX_DELAY);
    // store relays config
    if(NULL != system_config && NULL != system_config->relays)
    {
        char nvs_key[15] = {0};
        size_t nvs_size = sizeof(config_relay_t);

        for(uint8_t i=0;i<RELAYS_NUM;i++)
        {
            sprintf(nvs_key,"relay%02u",i);
            printf("NVS set, key: %s\n", nvs_key);
                printf("rel adr: %lu, timerOn val: %u \n)",(uint32_t)&(system_config->relays[i]),system_config->relays[i].timer_on_hours);
            ret |= nvs_set_blob(my_handle,nvs_key,&(system_config->relays[i]),nvs_size);
        }
        if(ESP_OK == ret)
        {
            ret |= nvs_commit(my_handle);
            printf("NVS commit \n");
        }
    }
    else
    {
        ret = ESP_FAIL;
    }
    nvs_close(my_handle);
    xSemaphoreGive(config_control_sem); // release mutex

    return ret;
}

static esp_err_t config_system_load_from_nvm(config_system_t* system_config)
{
    nvs_handle_t my_handle;
    esp_err_t ret;

    ret = nvs_open(NVS_NAMESPACE_SYSTEM_CONFIG, NVS_READWRITE, &my_handle);
    if(ESP_OK != ret) return ret;

    // critical section
    xSemaphoreTake(config_control_sem, portMAX_DELAY);
    // load relays config
    if(NULL != system_config)
    {
        char nvs_key[15] = {0};
        size_t nvs_size = sizeof(config_relay_t);
        config_relay_t* temp_relays = (config_relay_t*)pvPortMalloc(RELAYS_NUM * nvs_size);
        for(uint8_t i=0;i<RELAYS_NUM;i++)
        {
            sprintf(nvs_key,"relay%02u",i);
            size_t current_nvs_size = 0;
            // get only size
            printf("NVS get, key: %s \n",nvs_key);
            ret = nvs_get_blob(my_handle,nvs_key,NULL,&current_nvs_size);
            // check if the size matches
            if(current_nvs_size != nvs_size)
            {
                ESP_LOGE(TAG,"Size in NVS is different then expected.\n");
                ret |= ESP_FAIL;
                continue;
            }
            else // size is correct
            {
                ret |= nvs_get_blob(my_handle,nvs_key,&(temp_relays[i]),&current_nvs_size);
            }
        }
        if(ESP_OK == ret)
        {
            if(NULL != system_config->relays)
                vPortFree(system_config->relays);

            system_config->relays = temp_relays;
        }
    }
    else
    {
        ESP_LOGE(TAG,"Provided NULL system_config reference \n");
        ret = ESP_FAIL;
    }
    nvs_close(my_handle);
    xSemaphoreGive(config_control_sem); // release mutex
    return ret;
}

static esp_err_t config_module_save_to_nvm(node_sn_t node_sn, node_type_t node_type, void* module_cfg)
{
    nvs_handle_t my_handle;
    esp_err_t ret;

   // convert sn to nvs key
    char nvs_key[15] = {0};
    sprintf(nvs_key,"%08lX",node_sn);
    printf("NVS set, sn: %lu key: %s\n",node_sn, nvs_key);

    size_t nvs_size = 0;
    switch (node_type)
    {
        case NODE_TYPE_LED_BOARD:
            nvs_size = sizeof(config_module_led_board_t);
            break;

        case NODE_TYPE_TEMP_SENSOR:
            nvs_size = sizeof(config_module_temp_sens_t);
            break;

        case NODE_TYPE_WATER_LEVEL_SENSOR:
            nvs_size = sizeof(config_module_wl_sens_t);
            break;

        case NODE_TYPE_PH_SENSOR:
            nvs_size = sizeof(config_module_ph_sens_t);
            break;
    
        default:
            ESP_LOGE(TAG,"Cfg of module with type %d could not be saved \n", node_type);
            return ESP_FAIL;
            break;
    }

    ret = nvs_open(NVS_NAMESPACE_MODULE_CONFIG, NVS_READWRITE, &my_handle);
    if(ESP_OK != ret) return ret;

    // critical section
    xSemaphoreTake(config_control_sem, portMAX_DELAY);
    if(NULL != module_cfg)
    {
        ret = nvs_set_blob(my_handle,nvs_key,module_cfg,nvs_size);
        if(ESP_OK == ret)
        {
            ret = nvs_commit(my_handle);
            printf("NVS commit \n");
        }
    }
    else
    {
        ret = ESP_FAIL;
    }
    nvs_close(my_handle);
    xSemaphoreGive(config_control_sem); // release mutex

    return ret;
}

esp_err_t config_module_load_from_nvm(node_sn_t node_sn, node_type_t node_type, void* module_cfg)
{
    nvs_handle_t my_handle;
    esp_err_t ret;

    // convert sn to nvs key
    char nvs_key[15] = {0};
    sprintf(nvs_key,"%08lX",node_sn);
    printf("NVS get, sn: %lu key: %s\n",node_sn, nvs_key);
    
    // check params 
    if(NULL == module_cfg)
    {
        ESP_LOGE(TAG,"Null pointer provoded as module_cfg \n");
        return ESP_FAIL;
    }

    size_t nvs_size = 0;
    switch (node_type)
    {
        case NODE_TYPE_LED_BOARD:
            nvs_size = sizeof(config_module_led_board_t);
            break;

        case NODE_TYPE_TEMP_SENSOR:
            nvs_size = sizeof(config_module_temp_sens_t);
            break;

        case NODE_TYPE_WATER_LEVEL_SENSOR:
            nvs_size = sizeof(config_module_wl_sens_t);
            break;

        case NODE_TYPE_PH_SENSOR:
            nvs_size = sizeof(config_module_ph_sens_t);
            break;
    
        default:
            ESP_LOGE(TAG,"Cfg of module with type %d could not be loaded\n", node_type);
            return ESP_FAIL;
            break;
    }

    ret = nvs_open(NVS_NAMESPACE_MODULE_CONFIG, NVS_READWRITE, &my_handle);
    if(ESP_OK != ret) return ret;

    size_t current_nvs_size = 0;
    // get only size
    ret = nvs_get_blob(my_handle,nvs_key,NULL,&current_nvs_size);
    // check if the size matches
    if(current_nvs_size != nvs_size)
    {
        ESP_LOGE(TAG,"Size in NVS is different then expected.\n");
        ret |= ESP_FAIL;
    }
    
    if(ESP_OK == ret)
    {
        ret = nvs_get_blob(my_handle,nvs_key,module_cfg,&current_nvs_size);
    }
    
    nvs_close(my_handle);
    return ret;
}

/* Loads all module and system config from nvm or provide default if failed */
esp_err_t config_load_nvm_all(config_system_t* system_config, can_node_t* can_connected_nodes, uint8_t can_num_address_given)
{
    if(can_num_address_given == CONFIG_MAX_CONNECTED_NODES)
        return ESP_FAIL;

    esp_err_t ret = ESP_OK;
    // load modules
    for(uint8_t index=0; index<can_num_address_given; index++)
    {
        // check paramaters
        node_sn_t SN = can_connected_nodes[index].SN;
        if(SN <= 0) 
        {
            ESP_LOGE(TAG, "Failed to get config for module without SN \n");
            ret |= ESP_FAIL;
            break;
        }

        node_type_t node_type = can_connected_nodes[index].node_type;
        if(node_type >= NODE_TYPE_MAX || NODE_TYPE_MASTER == node_type || NODE_TYPE_UNSUPPORTED == node_type) 
        {
            ESP_LOGE(TAG, "Failed to get config for module with type %u \n",node_type);
            ret |= ESP_FAIL;
            break;
        }

        xSemaphoreTake(config_control_sem, portMAX_DELAY);
        // allocate new memory for config 
        if(NULL != can_connected_nodes[index].config)
            vPortFree(can_connected_nodes[index].config);

        switch (node_type)
        {
            case NODE_TYPE_LED_BOARD:
                can_connected_nodes[index].config = (config_module_led_board_t*)pvPortMalloc(sizeof(config_module_led_board_t));
                break;
            case NODE_TYPE_TEMP_SENSOR:
                can_connected_nodes[index].config = (config_module_temp_sens_t*)pvPortMalloc(sizeof(config_module_temp_sens_t));
                break;
            case NODE_TYPE_WATER_LEVEL_SENSOR:
                can_connected_nodes[index].config = (config_module_wl_sens_t*)pvPortMalloc(sizeof(config_module_wl_sens_t));
                break;
            case NODE_TYPE_PH_SENSOR:
                can_connected_nodes[index].config = (config_module_ph_sens_t*)pvPortMalloc(sizeof(config_module_ph_sens_t));
                break;
            default:
                ESP_LOGE(TAG,"Unknown config size for type %d \n", node_type);
                ret |= ESP_FAIL;
                break;
        }
        if(NULL == can_connected_nodes[index].config)
        {
            xSemaphoreGive(config_control_sem);
            continue; // take another node if there is a problem
        }

        if(ESP_OK == config_module_load_from_nvm(SN, node_type, can_connected_nodes[index].config))
        {
            printf("Module cfg for SN %04lX loaded from nvm\n",SN);
        }
        else 
        {
            ESP_LOGI(TAG, "Failed to get config for module with SN %04lX, providing default\n",SN);
            

            switch (node_type)
            {
                case NODE_TYPE_LED_BOARD:
                    *(config_module_led_board_t*)can_connected_nodes[index].config = DEFAULT_CONFIG_MODULE_LED_BOARD;
                    break;
                case NODE_TYPE_TEMP_SENSOR:
                    *(config_module_temp_sens_t*)can_connected_nodes[index].config = DEFAULT_CONFIG_MODULE_TEMP_SENS;
                    break;
                case NODE_TYPE_WATER_LEVEL_SENSOR:
                    *(config_module_wl_sens_t*)can_connected_nodes[index].config = DEFAULT_CONFIG_MODULE_WL_SENS;
                    break;
                case NODE_TYPE_PH_SENSOR:
                    *(config_module_ph_sens_t*)can_connected_nodes[index].config = DEFAULT_CONFIG_MODULE_PH_SENS;
                    break;
                default:
                    ESP_LOGE(TAG,"Default cfg of module with type %d could not be provided\n", node_type);
                    ret |= ESP_FAIL;
                    break;
            }
        }
        xSemaphoreGive(config_control_sem);
    }

    // load system cfg
    ret |= config_system_load_from_nvm(system_config);

    // if everything succeed, reload config version variable
    if(ESP_OK == ret)
    {
        uint64_t temp_version = 0;
        ret |= config_get_version_from_nvm(&temp_version);
        if(ESP_OK == ret)
        {
            CONFIG_VERSION_from_nvm = temp_version;
            printf("CONFIG_VERSION_from_nvm = %llu",CONFIG_VERSION_from_nvm);
        }
        else
            ESP_LOGE(TAG,"Failed to load cfg version from nvm.\n");
    }


    return ret;
}

esp_err_t config_update_from_web()
{
    char err_message[50] = {0};
    // get config JSON from web 
    cJSON* root = NULL;
    if(ESP_OK != wifi_driver_get_system_config(&root))
    {
        sprintf(err_message, "Error getting JSON config from web.\n");
        goto configError;
    }
    if(NULL == root) goto jsonError; // prevent memory leak

    // Extract mainUnitSN
    cJSON* mainUnitSN = cJSON_GetObjectItem(root, "mainUnitSN");
    if(NULL == mainUnitSN) goto jsonError; // prevent memory leak
    if(SERIAL_NUMBER != mainUnitSN->valueint)
    {
        sprintf(err_message, "Config has wrong mainUnitSN.\n");
        goto configError;
    }

    // Extract configVersion
    cJSON* configVersion = cJSON_GetObjectItem(root, "configVersion");
    if(NULL == configVersion) goto jsonError; // prevent memory leak
    if(configVersion->valueint < CONFIG_VERSION_from_nvm)
    {
        sprintf(err_message, "Config has lower ver. num. then current one.\n");
        goto configError;
    }

    // Extract relaysConfig array
    cJSON* relaysConfig = cJSON_GetObjectItem(root, "relaysConfig");
    if(NULL == relaysConfig) goto jsonError; // prevent memory leak
    if(RELAYS_NUM != cJSON_GetArraySize(relaysConfig))
    {
        sprintf(err_message, "Wrong num. of relays in config.\n");
        goto configError;
    }
    // Save values to custom struct
    config_relay_t relays_cfg[RELAYS_NUM];
    if(ESP_OK != parse_relays_cfg(relaysConfig, &relays_cfg[0]))
    {
        sprintf(err_message, "Failed to parse relays cfg.\n");
        goto configError;
    }

    printf("Relay timerOn hour: ");
    printf("%u \n",relays_cfg[0].timer_on_hours);

    // Store relays to nvm 
    config_system_t sys_cfg = {.relays = &relays_cfg[0]};

    printf("Relay timerOn hour: ");
    printf("%u \n",sys_cfg.relays[0].timer_on_hours);
    
    if(ESP_OK != config_system_save_to_nvm(&sys_cfg))
    {
        sprintf(err_message, "Failed to save sys cfg into nvm.\n");
        goto configError;
    }  

    // Extract modulesConfig array
    cJSON* modulesConfig = cJSON_GetObjectItem(root, "modulesConfig");
    if(NULL == modulesConfig) goto jsonError; // prevent memory leak
    uint8_t modules_count = cJSON_GetArraySize(modulesConfig);

    for (uint8_t i = 0; i < modules_count; i++) {
        cJSON* moduleConfig = cJSON_GetArrayItem(modulesConfig, i);
        if(NULL == moduleConfig) goto jsonError; // prevent memory leak
        // first get common parameters
        cJSON* SN = cJSON_GetObjectItem(moduleConfig, "SN");
        cJSON* nodeType = cJSON_GetObjectItem(moduleConfig, "nodeType");
        if(NULL == SN || NULL == nodeType) goto jsonError; // prevent memory leak
        node_sn_t module_sn = SN->valueint;
        node_type_t module_node_type = nodeType->valueint;

        // printf("Module %d - SN: %d, NodeType: %d\n", i, SN->valueint, nodeType->valueint);

        // get device specific config
        switch (module_node_type)
        {
            case NODE_TYPE_MASTER:
            case NODE_TYPE_UNKNOWN:
                ESP_LOGE(TAG,"Module in cfg from web has suspitious node type.");
                break;

            case NODE_TYPE_LED_BOARD:
                config_module_led_board_t module_led_cfg;
                if(ESP_OK != parse_module_cfg_led_board(moduleConfig, &module_led_cfg))
                {
                    sprintf(err_message, "Failed to parse module cfg, type %u.\n",module_node_type);
                    goto configError;
                }
                if(ESP_OK != config_module_save_to_nvm(module_sn,module_node_type,&module_led_cfg))
                {
                    sprintf(err_message, "Failed to save cfg into nvm, type %u.\n",module_node_type);
                    goto configError;
                }
                break;

            case NODE_TYPE_TEMP_SENSOR:
                config_module_temp_sens_t module_temp_cfg;
                if(ESP_OK != parse_module_cfg_temp_sens(moduleConfig, &module_temp_cfg))
                {
                    sprintf(err_message, "Failed to parse module cfg, type %u.\n",module_node_type);
                    goto configError;
                }
                if(ESP_OK != config_module_save_to_nvm(module_sn,module_node_type,&module_temp_cfg))
                {
                    sprintf(err_message, "Failed to save cfg into nvm, type %u.\n",module_node_type);
                    goto configError;
                }
                break;

            case NODE_TYPE_WATER_LEVEL_SENSOR:
                config_module_wl_sens_t module_wl_cfg;
                if(ESP_OK != parse_module_cfg_wl_sens(moduleConfig, &module_wl_cfg))
                {
                    sprintf(err_message, "Failed to parse module cfg, type %u.\n",module_node_type);
                    goto configError;
                }
                if(ESP_OK != config_module_save_to_nvm(module_sn,module_node_type,&module_wl_cfg))
                {
                    sprintf(err_message, "Failed to save cfg into nvm, type %u.\n",module_node_type);
                    goto configError;
                }
                break;

            case NODE_TYPE_PH_SENSOR:
                config_module_ph_sens_t module_ph_cfg;
                if(ESP_OK != parse_module_cfg_ph_sens(moduleConfig, &module_ph_cfg))
                {
                    sprintf(err_message, "Failed to parse module cfg, type %u.\n",module_node_type);
                    goto configError;
                }
                if(ESP_OK != config_module_save_to_nvm(module_sn,module_node_type,&module_ph_cfg))
                {
                    sprintf(err_message, "Failed to save cfg into nvm, type %u.\n",module_node_type);
                    goto configError;
                }
                break;

        
        default:
            break;
        }
    }
    if(ESP_OK != config_update_version_nvm((uint64_t)(configVersion->valueint)))
    {
        sprintf(err_message, "Failed to update cfg version (to %llu) in nvm.\n",(uint64_t)configVersion->valueint);
        goto configError;
    }

    cJSON_Delete(root);
    return ESP_OK;
    // error labels:
jsonError:
    sprintf(err_message, "Error parsing JSON config from web.\n");
configError:
    ESP_LOGE(TAG,"%s",err_message);
    cJSON_Delete(root);
    return ESP_FAIL;
}



