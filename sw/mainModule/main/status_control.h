#ifndef STATUS_CONTROL_H
#define STATUS_CONTROL_H


#include "driver/gpio.h"
#include "esp_log.h"

#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"
#include "esp_err.h"


#include "pins.h"

#define STATUS_DEVICE_STARTUP_TIMEOUT_MS 30000u
#define NUMBER_OF_STATUS_MODULES 5u


typedef enum {
    DEVST_NORMAL,
    DEVST_STARTUP,
    DEVST_ERROR_CAN_BUS,
    DEVST_ERROR_WIFI,
    DEVST_ERROR_GENERIC,
    DEVST_UNDEFINED
} device_status_t;

typedef enum {
    WIFIST_STARTUP,
    WIFIST_ERROR,
    WIFIST_CONNECTED,
    WIFIST_ONLINE,
    WIFIST_UNDEFINED
} wifi_status_t;

typedef enum {
    ALARMST_UNDEFINED = -1,
    ALARMST_OK = 0,
    ALARMST_WARNING = 1,
    ALARMST_ALARM = 3 // in bits, error status contains warning but not OK
} alarm_status_t;



typedef enum {
    MODST_STARTUP,
    MODST_READY,
    MODST_ERROR,
    MODST_UNDEFINED,
} module_status_t;



extern device_status_t STATUS_device;
extern wifi_status_t STATUS_wifi;
extern alarm_status_t STATUS_alarm1;
extern alarm_status_t STATUS_alarm2;

// Update NUMBER_OF_STATUS_MODULES if add new module
extern module_status_t STATUS_module_can;
extern module_status_t STATUS_module_display;
extern module_status_t STATUS_module_ota;
extern module_status_t STATUS_module_relays;
extern module_status_t STATUS_module_control;


esp_err_t status_control_init();
esp_err_t process_device_status();

bool _check_all_modules_status();

esp_err_t set_can_module_status(module_status_t status);
esp_err_t set_display_module_status(module_status_t status);
esp_err_t set_ota_module_status(module_status_t status);
esp_err_t set_relays_module_status(module_status_t status);
esp_err_t set_control_module_status(module_status_t status);





#endif // STATUS_CONTROL_H