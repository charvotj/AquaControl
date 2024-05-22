#include "water_level_sensor_driver.h"

static const char *TAG = "mainBoard - water_level_driver.c";


esp_err_t water_level_sensor_get_data(can_node_t* node_handle, uint8_t* percent_of_wl, bool* boye_state)
{
    // TODO: check CAN status and node status maybe
    // check node type
    if(NODE_TYPE_WATER_LEVEL_SENSOR != node_handle->node_type)
    {
        ESP_LOGE(TAG,"water_level_sensor_get_data call on the wrong sensor type");
        return ESP_FAIL;
    }

    uint8_t tx_data_len = 0u;
    uint8_t tx_data[TWAI_FRAME_MAX_DLC-1] = {0u};
    uint8_t rx_data_len = 0u;
    uint8_t rx_data[TWAI_FRAME_MAX_DLC-2] = {0u};
    can_cmd_status rx_status = CANST_GENERAL_ERROR;

    if(ESP_OK != can_tx_cmd_to_slave(node_handle->can_address, CAN_TS_WATER_LVL_SENS_GET_LVL,tx_data_len, tx_data, &rx_status, &rx_data_len, &rx_data[0]))
    {
        ESP_LOGE(TAG,"Failed to transmit or receive CAN_TS_WATER_LVL_SENS_GET_LVL");
        return ESP_FAIL;
    }

    // check status
    if(CANST_OK != rx_status)
    {
        ESP_LOGE(TAG,"CAN_TS_WATER_LVL_SENS_GET_LVL returned %u", rx_status);
        return ESP_FAIL;
    }


    // check payload length
    if(rx_data_len != 2)
    {
        ESP_LOGE(TAG,"Wrong payload of CAN_TS_WATER_LVL_SENS_GET_LVL");
        return ESP_FAIL;
    }
    // printf("0:%d, 1:%d \n",rx_data[0],rx_data[1]);
    *percent_of_wl = rx_data[0];
    *boye_state    = (0x01 & rx_data[1]);
    return ESP_OK;
}