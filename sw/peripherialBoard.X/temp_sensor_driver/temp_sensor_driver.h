/* Microchip Technology Inc. and its subsidiaries.  You may use this software 
 * and any derivatives exclusively with Microchip products. 
 * 
 * THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS".  NO WARRANTIES, WHETHER 
 * EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY IMPLIED 
 * WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS FOR A 
 * PARTICULAR PURPOSE, OR ITS INTERACTION WITH MICROCHIP PRODUCTS, COMBINATION 
 * WITH ANY OTHER PRODUCTS, OR USE IN ANY APPLICATION. 
 *
 * IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE, 
 * INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND 
 * WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP HAS 
 * BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE.  TO THE 
 * FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS 
 * IN ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT OF FEES, IF 
 * ANY, THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS SOFTWARE.
 *
 * MICROCHIP PROVIDES THIS SOFTWARE CONDITIONALLY UPON YOUR ACCEPTANCE OF THESE 
 * TERMS. 
 */

/* 
 * File:   
 * Author: 
 * Comments:
 * Revision history: 
 */

// This is a guard condition so that contents of this file are not included
// more than once.  
#ifndef TEMP_SENSOR_DRIVER_H
#define	TEMP_SENSOR_DRIVER_H

#include "../device_type.h"
#include "../mcc_generated_files/system/system.h"
#include "../mcc_generated_files/system/pins.h"
#include "../can_driver/can_driver.h"
#include <stdint.h>

#if DEVICE_TYPE == DEVICE_TYPE_TEMP_SENSOR

#define TEMP_SENSOR_MAX_NUM_ERRORS  5
#define TEMP_SENSOR_LIMIT_LOW       (int16_t)(3<<4)  // 3 degC
#define TEMP_SENSOR_LIMIT_HIGH      (int16_t)(83<<4) // 83 degC, God bless your fish :) ...85degC is deafult value after sensor init

//function for interfacing sensor

__bit ds18b20_start();
 
void ds18b20_write_bit(uint8_t value);
 
void ds18b20_write_byte(uint8_t value);
 
__bit ds18b20_read_bit(void);
 
uint8_t ds18b20_read_byte(void);
 
__bit ds18b20_read(uint8_t *raw_temp_value);

// functions for whole modul abstraction level

node_status_t temp_sensor_routine();
void temp_sensor_can_send_temp();


#ifdef	__cplusplus
extern "C" {
#endif /* __cplusplus */

    // TODO If C++ is being used, regular C code needs function names to have C 
    // linkage so the functions can be used by the c code. 

#ifdef	__cplusplus
}
#endif /* __cplusplus */

#endif /* DEVICE_TYPE == DEVICE_TYPE_TEMP_SENSOR */

#endif	/* TEMP_SENSOR_DRIVER_H */

