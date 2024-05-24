/*
 * File:   led_board_driver.c
 * Author: jakub
 *
 * Created on 6. kv?tna 2024, 15:27
 */

#include "../water_level_driver.h"

#if DEVICE_TYPE == DEVICE_TYPE_WATER_LEVEL_SENSOR

// STATIC VARIABLES
static uint16_t water_level_raw = 0; //  last reading
static uint16_t water_level_avg = 0; // averaged reading
static volatile uint8_t water_level_percent = 0; // computed percent of the sensor

static bool boye_state_raw = 0;
static bool boye_state_final = 0;
static uint8_t boye_state_change_counter = 0;


uint16_t water_level_get_level(void)
{
    ADC_DischargeSampleCapacitor();
    __delay_ms(100);
    uint16_t reading = ADC_GetSingleConversion(WATER_LEVEL_ADC_PIN);
    
    return reading;
}



node_status_t water_level_routine(void)
{
    // check shield connection
    if(0 != SHIELD_ON_PIN_GetValue())
         return NODEST_ERROR_GENERIC;
    
        
    water_level_raw = water_level_get_level();
    
    water_level_avg = 0.95*water_level_avg + 0.05*water_level_raw;
    
    if(water_level_avg < WATER_LEVEL_SENSOR_LIMIT_LOW)
    {
//        sensor out of water
        water_level_percent = 0;
        
//        char formattedData[20];
//        sprintf(formattedData, "WL out of lim: %u", water_level_avg);
//        CAN_SendDebugPrint(formattedData);
//        return NODEST_ERROR_GENERIC;
    }
    else if (water_level_avg > WATER_LEVEL_SENSOR_LIMIT_HIGH)
    {
        water_level_percent = 100;
        
//        char formattedData[20];
//        sprintf(formattedData, "WL out of lim: %u", water_level_avg);
//        CAN_SendDebugPrint(formattedData);
//        return NODEST_ERROR_GENERIC;
    }
    else
    {
        water_level_percent = 100.0*(water_level_avg - WATER_LEVEL_SENSOR_LIMIT_LOW) / (WATER_LEVEL_SENSOR_LIMIT_HIGH - WATER_LEVEL_SENSOR_LIMIT_LOW);
    }
    
    
     boye_state_raw = WATER_LEVEL_BOYE_GetValue();
     if(boye_state_raw != boye_state_final)
     {
         boye_state_change_counter++;
     }
     else
     {
         boye_state_change_counter = 0; // clear counter because values are the same
     }
     if(boye_state_change_counter > BOYE_STATE_DEBOUNCE_DELAY)
     {
         // change is registered
         boye_state_final = boye_state_raw;
     }
    
    
//    char formattedData[20];
//    sprintf(formattedData, "WL: %u", percent);
//    CAN_SendDebugPrint(formattedData);
    
    return NODEST_NORMAL;
}


void water_level_can_send_data(void)
{
    uint8_t data_len = 0u;
    uint8_t data[6] = {0u};
    can_cmd_status st = CANST_WATER_LEVEL_SENSOR_SPECIFIC_ER;
     
    if(NODEST_NORMAL == GLOBAL_device_status)
    {
        data_len = 2u;
        data[0] = water_level_percent;
        data[1] = boye_state_final; 
        st = CANST_OK;
    }
    else
    {
        char formattedData[20];
        sprintf(formattedData, "%u", water_level_raw);
        CAN_SendDebugPrint(formattedData);
    }
    CAN_SendTMCmd(CAN_TM_WATER_LVL_SENS_GET_LVL_RES,st,data_len,data);
}


#endif /* DEVICE_TYPE == DEVICE_TYPE_WATER_LEVEL_SENSOR */