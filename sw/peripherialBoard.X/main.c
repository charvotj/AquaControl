/*
 * File:   main.c
 * Author: jakub
 *
 * Created on April 13, 2024, 10:21 AM
 */


// Do not forgot to specify device type in device_type.h !!

#include "mcc_generated_files/system/system.h"
#include "device_type.h"
#include "can_driver/can_driver.h"
#include "../shared/CAN_definitions.h"
#include "temp_sensor_driver/temp_sensor_driver.h"



uint8_t led_brightness = 0;





void timer0ISR(void)
{
    counter_second++;
    
    if(counter_second >= 2)
    {
//        LED_PIN_Toggle();
//        UART1_println("ping\0");
//        CAN_SendDebugPrint("debug\0");
    }
//    LED1_FB_Toggle();
//    LED0_FB_Toggle();
//    led_brightness += 20;
//    led_set_brightness(LED_CH_0,led_brightness);
//    
}






void main(void) 
{
    SYSTEM_Initialize();
    TMR0_OverflowCallbackRegister(timer0ISR);
    INTCON0bits.GIE = 1;
    CAN_SendDebugPrint("System started");
    
    while(counter_second < 3)
    {
        
    }

    
//    led_disable_channel(LED_CH_0);
//    led_enable_channel(LED_CH_1);
//    
    
    

    

    
    
            
    // If using interrupts in PIC18 High/Low Priority Mode you need to enable the Global High and Low Interrupts 
    // If using interrupts in PIC Mid-Range Compatibility Mode you need to enable the Global Interrupts 
    // Use the following macros to: 

    // Enable the Global Interrupts 
//    INTERRUPT_GalobalInterruptEnable(); 
//    LED_PIN_Toggle();
    
    uint16_t raw_temp = 0;
    char formattedData[20]; // Adjust the buffer size as needed
    
    
    while(1)
    {
       
        __delay_ms(1000);
    }    
    return;
}
