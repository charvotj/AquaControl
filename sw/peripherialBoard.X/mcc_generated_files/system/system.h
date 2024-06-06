/**
 * System Driver Header File
 * 
 * @file system.h
 * 
 * @defgroup systemdriver System Driver
 * 
 * @brief This is the generated header file for the System driver.
 *
 * @version Driver Version 1.0.2
 *
 * @version Package Version 1.0.3
*/

/*
� [2024] Microchip Technology Inc. and its subsidiaries.

    Subject to your compliance with these terms, you may use Microchip 
    software and any derivatives exclusively with Microchip products. 
    You are responsible for complying with 3rd party license terms  
    applicable to your use of 3rd party software (including open source  
    software) that may accompany Microchip software. SOFTWARE IS ?AS IS.? 
    NO WARRANTIES, WHETHER EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS 
    SOFTWARE, INCLUDING ANY IMPLIED WARRANTIES OF NON-INFRINGEMENT,  
    MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE. IN NO EVENT 
    WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE, 
    INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY 
    KIND WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF 
    MICROCHIP HAS BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE 
    FORESEEABLE. TO THE FULLEST EXTENT ALLOWED BY LAW, MICROCHIP?S 
    TOTAL LIABILITY ON ALL CLAIMS RELATED TO THE SOFTWARE WILL NOT 
    EXCEED AMOUNT OF FEES, IF ANY, YOU PAID DIRECTLY TO MICROCHIP FOR 
    THIS SOFTWARE.
*/

#ifndef SYSTEM_H
#define	SYSTEM_H

#include <xc.h>
#include <stdint.h>
#include <stdbool.h>

#include "../../device_type.h"
#include "../../../shared/common_types.h"
#include "../system/config_bits.h"
#include "../system/clock.h"
#include "../system/pins.h"
#include "../can/can1.h"
#include "../timer/tmr0.h"
#include "../uart/uart1.h"
#include "../system/interrupt.h"
#include "../../can_driver/can_driver.h"

#ifndef DEVICE_TYPE
    #error "Device type not defined in this context"
#endif
#if DEVICE_TYPE == DEVICE_TYPE_LED_BOARD
    #include "../pwm/pwm4_16bit.h"
#elif DEVICE_TYPE == DEVICE_TYPE_WATER_LEVEL_SENSOR
    #include "../adc/adc.h"
#endif



/**
 * @ingroup systemdriver
 * @brief Initializes the system module. This is called only once before calling other APIs.
 * @param None.
 * @return None.
*/

extern volatile uint64_t counter_second;
void SYSTEM_Initialize(void);

#endif	/* SYSTEM_H */
/**
 End of File
*/