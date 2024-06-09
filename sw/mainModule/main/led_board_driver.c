#include "led_board_driver.h"

static const char *TAG = "mainBoard - led_board_driver.c";


esp_err_t led_board_can_ch0_set_brightness(can_node_t* node_handle, uint8_t brightness)
{
    // check node type
    if(NODE_TYPE_LED_BOARD != node_handle->node_type)
    {
        ESP_LOGE(TAG,"led_board_can_ch0_set_brightness call on the wrong sensor type");
        return ESP_FAIL;
    }

    printf("Sending cmd ...\n");

    uint8_t tx_data_len = 1u;
    uint8_t tx_data[TWAI_FRAME_MAX_DLC-1] = {0u};
    uint8_t rx_data_len = 0u;
    uint8_t rx_data[TWAI_FRAME_MAX_DLC-2] = {0u};
    can_cmd_status rx_status = CANST_GENERAL_ERROR;

    tx_data[0] = brightness;
    if(ESP_OK != can_tx_cmd_to_slave(node_handle->can_address, CAN_TS_LED_BOARD_CH0_SET_BRIGHTNESS,tx_data_len, tx_data,&rx_status, &rx_data_len, &rx_data[0]))
    {
        ESP_LOGE(TAG,"Failed to transmit or receive CAN_TS_LED_BOARD_CH0_SET_BRIGHTNESS\n");
        return ESP_FAIL;
    }

    // check status
    if(CANST_OK != rx_status)
    {
        ESP_LOGE(TAG,"CAN_TS_LED_BOARD_CH0_SET_BRIGHTNESS returned %u\n", rx_status);
        return ESP_FAIL;
    }

    // check payload length
    if(rx_data_len != 0)
    {
        ESP_LOGE(TAG,"Wrong payload of CAN_TS_LED_BOARD_CH0_SET_BRIGHTNESS\n");
        return ESP_FAIL;
    }
    return ESP_OK;
}

esp_err_t led_board_can_ch1_set_brightness(can_node_t* node_handle, uint8_t brightness)
{
    // check node type
    if(NODE_TYPE_LED_BOARD != node_handle->node_type)
    {
        ESP_LOGE(TAG,"led_board_can_ch0_set_brightness call on the wrong sensor type");
        return ESP_FAIL;
    }

    uint8_t tx_data_len = 1u;
    uint8_t tx_data[TWAI_FRAME_MAX_DLC-1] = {0u};
    uint8_t rx_data_len = 0u;
    uint8_t rx_data[TWAI_FRAME_MAX_DLC-2] = {0u};
    can_cmd_status rx_status = CANST_GENERAL_ERROR;

    tx_data[0] = brightness;
    if(ESP_OK != can_tx_cmd_to_slave(node_handle->can_address, CAN_TS_LED_BOARD_CH1_SET_BRIGHTNESS,tx_data_len, &tx_data[0],&rx_status, &rx_data_len, &rx_data[0]))
    {
        ESP_LOGE(TAG,"Failed to transmit or receive CAN_TS_LED_BOARD_CH0_SET_BRIGHTNESS\n");
        return ESP_FAIL;
    }

    // check status
    if(CANST_OK != rx_status)
    {
        ESP_LOGE(TAG,"CAN_TS_LED_BOARD_CH0_SET_BRIGHTNESS returned %u\n", rx_status);
        return ESP_FAIL;
    }

    // check payload length
    if(rx_data_len != 0)
    {
        ESP_LOGE(TAG,"Wrong payload of CAN_TS_LED_BOARD_CH0_SET_BRIGHTNESS\n");
        return ESP_FAIL;
    }
    return ESP_OK;
}


static uint8_t _get_strip_brightness(config_ledstrip_t* cfg, simple_time_t time)
{
    printf("Getting strip brightness... \n");
    // check turn off (before start, after end)
    if(cfg->start_time_hours > time.h || (cfg->start_time_hours == time.h && cfg->start_time_minutes >= time.m))
        return 0;

    if(cfg->end_time_hours < time.h || (cfg->end_time_hours == time.h && cfg->end_time_minutes <= time.m))
        return 0;

    // between start and end
    uint16_t min_from_start = 60 * (time.h - cfg->start_time_hours) + (time.m - cfg->start_time_minutes);
    uint16_t min_to_end = 60 * (cfg->end_time_hours - time.h) + (cfg->end_time_minutes - time.m);

    uint8_t br_from_start, br_from_end;

    if(min_from_start >= cfg->rise_time)
        br_from_start = cfg->intensity;
    else
        br_from_start = cfg->intensity * (1.0 * min_from_start / cfg->rise_time);

    if(min_to_end >= cfg->fall_time)
        br_from_end = cfg->intensity;
    else
        br_from_end = cfg->intensity * (1.0 * min_to_end / cfg->fall_time);

    printf("Hoho \n");
    // return lower brightness 
    if(br_from_start < br_from_end)
        return br_from_start;
    else 
        return br_from_end;
    
}

esp_err_t led_board_process_config(can_node_t* node,  simple_time_t time, bool use_rtc)
{
    if(NODE_TYPE_LED_BOARD != node->node_type)
        return ESP_FAIL;
    // if node isnt in normal state, skip config process 
    if(NODEST_NORMAL != node->status)
        return ESP_OK;
    
    if(NULL == node->config)
        return ESP_FAIL;

    esp_err_t ret = ESP_OK;
    // proccess config
    config_module_led_board_t* cfg = (config_module_led_board_t*)(node->config);

    // 1. clear both strips if rtc is not known 
    if(!use_rtc)
    {
        ret |= led_board_can_ch0_set_brightness(node,0u);
        ret |= led_board_can_ch1_set_brightness(node,0u);
        return ret;
    }

    // 2. set strip according to rtc
    uint8_t br = _get_strip_brightness(&(cfg->ledstrip_0),time);
    ret |= led_board_can_ch0_set_brightness(node,br);
printf("Setting LED strip 0 br. to %u\n",br);
    br = _get_strip_brightness(&(cfg->ledstrip_1),time);
    ret |= led_board_can_ch1_set_brightness(node,br);
printf("Setting LED strip 1 br. to %u\n",br);
    return ret;
}