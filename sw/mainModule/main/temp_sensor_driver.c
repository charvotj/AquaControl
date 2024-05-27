#include "temp_sensor_driver.h"

static const char *TAG = "mainBoard - temp_sensor_driver.c";


esp_err_t temp_sensor_get_temperature(can_node_t* node_handle, float* temperature)
{
    // TODO: check CAN status and node status maybe
    // check node type
    if(NODE_TYPE_TEMP_SENSOR != node_handle->node_type)
    {
        ESP_LOGE(TAG,"get_temperature call on the wrong sensor type");
        return ESP_FAIL;
    }

    uint8_t tx_data_len = 0u;
    uint8_t tx_data[TWAI_FRAME_MAX_DLC-1] = {0u};
    uint8_t rx_data_len = 0u;
    uint8_t rx_data[TWAI_FRAME_MAX_DLC-2] = {0u};
    can_cmd_status rx_status = CANST_GENERAL_ERROR;

    if(ESP_OK != can_tx_cmd_to_slave(node_handle->can_address, CAN_TS_TEMP_SENS_GET_TEMP,tx_data_len, tx_data,&rx_status, &rx_data_len, &rx_data[0]))
    {
        ESP_LOGE(TAG,"Failed to transmit or receive CAN_TS_TEMP_SENS_GET_TEMP\n");
        return ESP_FAIL;
    }

    // check status
    if(CANST_OK != rx_status)
    {
        ESP_LOGE(TAG,"CAN_TM_TEMP_SENS_GET_TEMP_RES returned %u\n", rx_status);
        return ESP_FAIL;
    }

    // check payload length
    if(rx_data_len != 2)
    {
        ESP_LOGE(TAG,"Wrong payload of CAN_TM_TEMP_SENS_GET_TEMP_RES\n");
        return ESP_FAIL;
    }
    // printf("0:%d, 1:%d \n",rx_data[0],rx_data[1]);
    *temperature = ((int16_t)(rx_data[1] << 8) | (rx_data[0])) / 16.0;
    return ESP_OK;
}