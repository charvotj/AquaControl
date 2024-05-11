/*
 * File:   led_board_driver.c
 * Author: jakub
 *
 * Created on 6. kv?tna 2024, 15:27
 */

#include "../led_board_driver.h"
#if DEVICE_TYPE == DEVICE_TYPE_LED_BOARD

void led_enable_channel(led_channel_t channel)
{
    if(channel == LED_CH_0)
        LED0_EN_SetHigh();
    else if(channel == LED_CH_1)
        LED1_EN_SetHigh();
}

void led_disable_channel(led_channel_t channel)
{
    if(channel == LED_CH_0)
        LED0_EN_SetLow();
    else if(channel == LED_CH_1)
        LED1_EN_SetLow();
}

void led_set_brightness(led_channel_t channel, uint8_t brightness)
{
//    TODO: this is temp
    DAC1_SetOutput(brightness);
}

#endif
