#include "status_control.h"

static const char *TAG = "mainBoard - status_control.c";

typedef enum{
    CHECK_ANY,
    CHECK_ALL,
} check_type_t;

module_status_t* all_modules_status[NUMBER_OF_STATUS_MODULES];

device_status_t STATUS_device           = (device_status_t)DEVST_UNDEFINED;

// Update NUMBER_OF_STATUS_MODULES if add new module and add it with extern to the header
module_status_t STATUS_module_can       = (module_status_t)MODST_UNDEFINED;
module_status_t STATUS_module_display   = (module_status_t)MODST_UNDEFINED;
module_status_t STATUS_module_ota       = (module_status_t)MODST_UNDEFINED;

esp_err_t status_control_init()
{
    // Register modules
    uint8_t index = 0;
    all_modules_status[index++] = &STATUS_module_can;
    all_modules_status[index++] = &STATUS_module_display;
    all_modules_status[index++] = &STATUS_module_ota;

    if(NUMBER_OF_STATUS_MODULES != index)
    {
        ESP_LOGE(TAG,"Some of the modules was not registered here!");
        return ESP_FAIL;
    }

    // // Defined variables with default
    // STATUS_device = DEVST_UNDEFINED;
    // for(index=0;index<NUMBER_OF_STATUS_MODULES;index++)
    // {
    //     *all_modules_status[index] = MODST_UNDEFINED;
    // }
    return ESP_OK;
}

esp_err_t process_device_status()
{
    // Detect normal state
    if ( true == _check_all_modules_status(MODST_READY, CHECK_ALL))
    {
        STATUS_device = DEVST_NORMAL;
        return ESP_OK;
    }

    // Detect undefined
    if( true == _check_all_modules_status(MODST_UNDEFINED, CHECK_ANY))
    {
        STATUS_device = DEVST_UNDEFINED;
        return ESP_OK;
    }
    

    // First detect specific errors
    if( MODST_ERROR == STATUS_module_can )
    {
        STATUS_device = DEVST_ERROR_CAN_BUS;
        return ESP_OK;
    }
    // Then detect other errors
    if( true == _check_all_modules_status(MODST_ERROR, CHECK_ANY))
    {
        STATUS_device = DEVST_ERROR_GENERIC;
        return ESP_OK;
    }

    // Detect startup or error (timeout)
    // ms from program start
    uint32_t current_millis = xTaskGetTickCount() * portTICK_PERIOD_MS;
    if(current_millis < STATUS_DEVICE_STARTUP_TIMEOUT_MS)
    {
        STATUS_device = DEVST_STARTUP;
    }
    else
    {
        STATUS_device = DEVST_ERROR_GENERIC;
    }
    return ESP_OK;
    
    // Note: Be carefull when adding new condition, order really matters.
}

bool _check_all_modules_status(module_status_t expected_state, check_type_t check_type)
{
    switch (check_type)
    {
    case CHECK_ALL:
        for(uint8_t index=0;index<NUMBER_OF_STATUS_MODULES;index++)
        {
            if(*all_modules_status[index] != expected_state)
                return false;
        }
        return true;
    case CHECK_ANY:
        for(uint8_t index=0;index<NUMBER_OF_STATUS_MODULES;index++)
            {
                if(*all_modules_status[index] == expected_state)
                    return true;
            }
            return false;
    default:
        return false;
    }
}

esp_err_t set_can_module_status(module_status_t status)
{
    STATUS_module_can = status;
    return ESP_OK;
}

esp_err_t set_display_module_status(module_status_t status)
{
    STATUS_module_display = status;
    return ESP_OK;
}

esp_err_t set_ota_module_status(module_status_t status)
{
    STATUS_module_ota = status;
    return ESP_OK;
}




