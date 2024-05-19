#ifndef RELAYS_DRIVER_H
#define RELAYS_DRIVER_H


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"
#include "esp_err.h"
#include "esp_log.h"

#include <pcf8574.h> // from external esp-idf-lib, inluded in CMakeLists.txt in project root folder 

#include "pins.h"
#include "status_control.h"
#include"../../shared/common_types.h"


#define RELAYS_NUM  4u   // number of relays available

// Relays are numbered "human readably" to be consistent with GUI, but internally are indexed from 0
typedef enum
{
    RELAY_NUM_1 = 0u,
    RELAY_NUM_2 = 1u,
    RELAY_NUM_3 = 2u,
    RELAY_NUM_4 = 3u,
} relay_num_t;

esp_err_t relays_init();

esp_err_t relays_get_all(uint8_t* relays_st);
/*
    relays_set_all(params):
        relay_st: state of all relays, default bahavior is masking unused bits according to relay_num_t definition
        force: masking is disabled, all values are directly passed to relays
*/
esp_err_t relays_set_all(uint8_t relays_st);
esp_err_t relays_set_all_force(uint8_t relays_st, bool force);

esp_err_t relays_get_state(relay_num_t relay_num, bool* relay_st);
esp_err_t relays_set_state(relay_num_t relay_num, bool relay_st);






#endif // RELAYS_DRIVER_H
