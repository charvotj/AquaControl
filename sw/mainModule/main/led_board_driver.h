#ifndef LED_BOARD_DRIVER_H
#define LED_BOARD_DRIVER_H


#include "driver/gpio.h"
#include "esp_log.h"

#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"
#include "esp_err.h"
#include "driver/twai.h"

#include "pins.h"
#include "status_control.h"
#include "CAN_driver.h"
#include"../../shared/common_types.h"
#include"../../shared/CAN_definitions.h"

esp_err_t led_board_can_ch0_set_brightness(can_node_t* node_handle, uint8_t brightness);
esp_err_t led_board_can_ch1_set_brightness(can_node_t* node_handle, uint8_t brightness);

esp_err_t led_board_process_config(can_node_t* node,  simple_time_t time, bool use_rtc);




#endif // LED_BOARD_DRIVER_H