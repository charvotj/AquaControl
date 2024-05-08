#ifndef CAN_DRIVER_H
#define CAN_DRIVER_H

#include <stdio.h>
#include <stdlib.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "freertos/semphr.h"
#include "esp_err.h"
#include "esp_log.h"
#include "driver/twai.h"


#include "pins.h"
#include "../../shared/CAN_definitions.h"


#define CAN_DEBUG_PRINTS    1



esp_err_t can_driver_init();
esp_err_t can_proccess_rx(TickType_t timeout);
esp_err_t can_process_cmd_debug_print(twai_message_t *message);
esp_err_t can_process_cmd_to_master(twai_message_t *message);

esp_err_t can_driver_deinit();


#endif /* CAN_DRIVER_H*/