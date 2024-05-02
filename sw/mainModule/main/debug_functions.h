#ifndef DEBUG_FUNCTIONS_H
#define DEBUG_FUNCTIONS_H

#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"
#include "esp_err.h"

#define ARRAY_SIZE_OFFSET   7   //Increase this if print_real_time_stats returns ESP_ERR_INVALID_SIZE


esp_err_t print_real_time_stats(TickType_t xTicksToWait);

#endif // DEBUG_FUNCTIONS_H