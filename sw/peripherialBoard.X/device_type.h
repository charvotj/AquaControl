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
#ifndef DEVICE_TYPE_H
#define	DEVICE_TYPE_H

#include <xc.h> // include processor files - each processor file is guarded.  
#include "../shared/common_types.h"

// Choose only values defined in common_types.h
// ?????????????????????????????????????????
#define DEVICE_TYPE DEVICE_TYPE_TEMP_SENSOR
// ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

node_type_t GLOBAL_device_type = DEVICE_TYPE;



// SPECIFIC INCLUDES
#if DEVICE_TYPE == DEVICE_TYPE_LED_BOARD
    #include  "led_board_driver/led_board_driver.h"
#elif DEVICE_TYPE == DEVICE_TYPE_TEMP_SENSOR
    #include "temp_sensor_driver/temp_sensor_driver.h"

#else
    #error "Not supported DEVICE_TYPE"
#endif
#endif	/* DEVICE_TYPE_H */

