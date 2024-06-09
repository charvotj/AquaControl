#ifndef CONTROL_ROUTINES_H
#define CONTROL_ROUTINES_H

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
#include "status_control.h"
#include "wifi_driver.h"
#include "relays_driver.h"
#include "config_manager.h"
#include "CAN_driver.h"
#include "temp_sensor_driver.h"
#include "water_level_sensor_driver.h"
#include "led_board_driver.h"


esp_err_t control_prepare_can_modules(TickType_t timeout_for_can);
esp_err_t control_routine();


#endif /* CONTROL_ROUTINES_H*/