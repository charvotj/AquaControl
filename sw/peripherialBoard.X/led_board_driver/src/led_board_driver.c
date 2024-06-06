/*
 * File:   led_board_driver.c
 * Author: jakub
 *
 * Created on 6. kv?tna 2024, 15:27
 */

#include "../led_board_driver.h"
#if DEVICE_TYPE == DEVICE_TYPE_LED_BOARD

static uint8_t ch0_curr_brightness = 0;
static uint8_t ch0_desired_brightness = 255;

static uint8_t ch1_curr_brightness = 0;
static uint8_t ch1_desired_brightness = 255;

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
    // not neccesary bacouse of uint8_t and max range is 255 also
//    uint16_t value_to_set = LED_PWM_MAX - brightness;
//    if(value_to_set < LED_PWM_MIN)
//        value_to_set = LED_PWM_MIN;
    
    
    uint16_t value_to_set = LED_PWM_MAX - brightness;
    
    if(LED_CH_0 == channel)
    {
        LED_PWM_SetSlice1Output1DutyCycleRegister(value_to_set);
        ch0_curr_brightness = brightness;
    }
    else if(LED_CH_1 == channel)
    {
        LED_PWM_SetSlice1Output2DutyCycleRegister(value_to_set);
        ch1_curr_brightness = brightness;
    }
    
    LED_PWM_LoadBufferRegisters();
}

void led_board_init(void)
{
    // prepare pwm to minimum
    led_set_brightness(LED_CH_0,ch0_curr_brightness);
    led_set_brightness(LED_CH_1,ch1_curr_brightness);
    
    // turn on leds on minimum intensity
    led_enable_channel(LED_CH_0);
    led_enable_channel(LED_CH_1);
}

node_status_t led_board_routine(void)
{
    // change led brightness slowly
    if(ch0_desired_brightness > ch0_curr_brightness)
        led_set_brightness(LED_CH_0, ch0_curr_brightness + 1);
    else if(ch0_desired_brightness < ch0_curr_brightness)
        led_set_brightness(LED_CH_0, ch0_curr_brightness - 1);
    
    if(ch1_desired_brightness > ch1_curr_brightness)
        led_set_brightness(LED_CH_1, ch1_curr_brightness + 1);
    else if(ch1_desired_brightness < ch1_curr_brightness)
        led_set_brightness(LED_CH_1, ch1_curr_brightness - 1);
    
    
    return NODEST_NORMAL;
}

void led_board_can_ch0_set_brightness(uint8_t brightness)
{
    ch0_desired_brightness = brightness;
    
    uint8_t data_len = 0u;
    uint8_t data[6] = {0u};
    can_cmd_status st = CANST_LED_BOARD_SPECIFIC_ER;
     
    if(NODEST_NORMAL == GLOBAL_device_status)
    {
        st = CANST_OK;
    }

    CAN_SendTMCmd(CAN_TS_LED_BOARD_CH1_SET_BRIGHTNESS,st,data_len,data);
}

void led_board_can_ch1_set_brightness(uint8_t brightness)
{
    ch1_desired_brightness = brightness;
    
    uint8_t data_len = 0u;
    uint8_t data[6] = {0u};
    can_cmd_status st = CANST_LED_BOARD_SPECIFIC_ER;
     
    if(NODEST_NORMAL == GLOBAL_device_status)
    {
        st = CANST_OK;
    }

    CAN_SendTMCmd(CAN_TM_LED_BOARD_CH1_SET_BRIGHTNESS_RES,st,data_len,data);
}


#endif
