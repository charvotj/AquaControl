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
#include "string.h"


#include "pins.h"
#include "../../shared/CAN_definitions.h"
#include "../../shared/common_types.h"
#include "status_control.h"
#include "globals.h"
#include "config_manager.h"


#define CAN_DEBUG_PRINTS                1
#define CAN_DEFAULT_NUM_OF_RETRANSMIT   5
#define CAN_DEFAULT_RX_TIMEOUT          pdMS_TO_TICKS(3000)


typedef enum
{
    CAN_MST_EMPTY,
    CAN_MST_TO_TRANSMIT,
    CAN_MST_TO_RECEIVE,
} can_message_state_t;

typedef struct
{
    twai_message_t* message;
    can_message_state_t message_state;
    uint8_t num_of_retransmit;
    TickType_t rx_timeout_time;
} can_message_action_t;


extern SemaphoreHandle_t can_rx_sem;


esp_err_t can_driver_init();


esp_err_t can_proccess_rx();
esp_err_t can_process_cmd_debug_print(twai_message_t *message);
esp_err_t can_process_cmd_broadcast(twai_message_t *message);

esp_err_t can_tx_cmd_broadcast(uint8_t cmd_number, uint8_t dlc, uint8_t data[TWAI_FRAME_MAX_DLC]);
esp_err_t can_tx_cmd_to_slave(uint8_t slave_address, uint8_t cmd_number, uint8_t tx_data_len, uint8_t* tx_data, can_cmd_status* rx_status, uint8_t* rx_data_len, uint8_t* rx_data);

esp_err_t can_slave_reset(uint8_t slave_address);
esp_err_t can_slave_restore_defaults(uint8_t slave_address);
esp_err_t can_slave_mute(uint8_t slave_address, u_int8_t time_s);
esp_err_t can_slave_ping(uint8_t slave_address);
esp_err_t can_slave_get_SN(uint8_t slave_address, node_sn_t* sn);
esp_err_t can_slave_get_node_type(uint8_t slave_address, node_type_t* node_type);
esp_err_t can_slave_get_node_status(uint8_t slave_address, node_status_t* status);
esp_err_t can_slave_get_temperature(uint8_t slave_address, node_temp_t* temp);
esp_err_t can_slave_get_uptime(uint8_t slave_address, node_uptime_t* uptime);

esp_err_t can_perform_addresing(can_node_t* can_connected_nodes, uint8_t* can_num_address_given);
esp_err_t can_poll_nodes_sn(can_node_t* can_connected_nodes, uint8_t can_num_address_given);
esp_err_t can_poll_nodes_status(can_node_t* can_connected_nodes, uint8_t can_num_address_given);
esp_err_t can_poll_nodes_type(can_node_t* can_connected_nodes, uint8_t can_num_address_given);

esp_err_t can_driver_deinit();


#endif /* CAN_DRIVER_H*/
