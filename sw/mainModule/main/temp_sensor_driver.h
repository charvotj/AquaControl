#ifndef TEMP_SENSOR_DRIVER_H
#define TEMP_SENSOR_DRIVER_H


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

esp_err_t temp_sensor_get_temperature(can_node_t* node_handle, float* temperature);





#endif // TEMP_SENSOR_DRIVER_H