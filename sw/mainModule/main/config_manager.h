#ifndef CONFIG_MANAGER_H
#define CONFIG_MANAGER_H

#include "driver/gpio.h"
#include "esp_log.h"

#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"
#include "esp_err.h"
#include "cJSON.h"
#include "nvs_flash.h"
#include "nvs.h"

#include "pins.h"
#include "status_control.h"
#include "wifi_driver.h"
#include "relays_driver.h"

#define NVS_NAMESPACE_MODULE_CONFIG "modulecfg"

extern SemaphoreHandle_t config_control_sem;


typedef struct 
{
    bool    manual;
    uint8_t timer_on_hours;
    uint8_t timer_on_minutes;
    uint8_t timer_off_hours;
    uint8_t timer_off_minutes;
    bool    manual_state;
} config_relay_t;

typedef struct
{
    bool active;
    float max_value;
    float min_value;
} config_alarm_t;

#define DEFAULT_CONFIG_ALARM (config_alarm_t){false,0.0,0.0}

// modules config
//--------------------
// NODE_TYPE_LED_BOARD
typedef struct
{
    uint8_t start_time_hours;
    uint8_t start_time_minutes;
    uint8_t end_time_hours;
    uint8_t end_time_minutes;
    uint8_t fall_time; // duration in minutes
    uint8_t rise_time; // duration in minutes
    uint8_t intensity;
} config_ledstrip_t;

#define DEFAULT_CONFIG_LEDSTRIP (config_ledstrip_t){0,0,0,0,0,0,0}

typedef struct
{
    config_ledstrip_t ledstrip_0;
    config_ledstrip_t ledstrip_1;
} config_module_led_board_t;

#define DEFAULT_CONFIG_MODULE_LED_BOARD (config_module_led_board_t){DEFAULT_CONFIG_LEDSTRIP,DEFAULT_CONFIG_LEDSTRIP}

// NODE_TYPE_TEMP_SENSOR
typedef struct
{
    config_alarm_t alarm_cfg;
} config_module_temp_sens_t;

#define DEFAULT_CONFIG_MODULE_TEMP_SENS (config_module_temp_sens_t){DEFAULT_CONFIG_ALARM}

// NODE_TYPE_WATER_LEVEL_SENSOR
typedef struct
{
    config_alarm_t alarm_cfg;
} config_module_wl_sens_t;

#define DEFAULT_CONFIG_MODULE_WL_SENS   (config_module_wl_sens_t){DEFAULT_CONFIG_ALARM}

// NODE_TYPE_PH_SENSOR
typedef struct
{
    config_alarm_t alarm_cfg;
} config_module_ph_sens_t;

#define DEFAULT_CONFIG_MODULE_PH_SENS   (config_module_ph_sens_t){DEFAULT_CONFIG_ALARM}

esp_err_t config_manager_init();

esp_err_t config_update_from_web();
esp_err_t config_module_load_from_nvm(node_sn_t node_sn, node_type_t node_type, void* module_cfg);
esp_err_t config_load_nvm_all(can_node_t* can_connected_nodes, uint8_t can_num_address_given);




#endif /* CONFIG_MANAGER_H*/