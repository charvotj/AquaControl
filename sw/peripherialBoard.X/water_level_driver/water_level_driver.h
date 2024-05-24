// This is a guard condition so that contents of this file are not included
// more than once.  
#ifndef WATER_LEVEL_DRIVER_H
#define	WATER_LEVEL_DRIVER_H

#include "../device_type.h"
#include "../mcc_generated_files/system/system.h"
#include "../mcc_generated_files/system/pins.h"
#include "../can_driver/can_driver.h"
#include <stdint.h>

#if DEVICE_TYPE == DEVICE_TYPE_WATER_LEVEL_SENSOR

#define WATER_LEVEL_SENSOR_LIMIT_LOW       (uint16_t)(20000)  // Bottom of sensor is under water
#define WATER_LEVEL_SENSOR_LIMIT_HIGH      (uint16_t)(34000)  // Whole sensor is under water

#define BOYE_STATE_DEBOUNCE_DELAY          (uint8_t)(10u)     // After 10 iterantion change is propagated

//function for interfacing sensor

// functions for whole modul abstraction level
uint16_t water_level_get_level(void);
node_status_t water_level_routine(void);
void water_level_can_send_data(void);


#ifdef	__cplusplus
extern "C" {
#endif /* __cplusplus */

    // TODO If C++ is being used, regular C code needs function names to have C 
    // linkage so the functions can be used by the c code. 

#ifdef	__cplusplus
}
#endif /* __cplusplus */

#endif /* DEVICE_TYPE == DEVICE_TYPE_WATER_LEVEL_SENSOR */

#endif	/* WATER_LEVEL_DRIVER_H */

