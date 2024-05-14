// This is a guard condition so that contents of this file are not included
// more than once.  
#ifndef DEVICE_TYPE_H
#define	DEVICE_TYPE_H

#include <xc.h> // include processor files - each processor file is guarded.  
#include "../shared/common_types.h"

// Choose only values defined in common_types.h
// ?????????????????????????????????????????
#define DEVICE_TYPE DEVICE_TYPE_WATER_LEVEL_SENSOR
// ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
#define FW_VERSION_MAJOR    0
#define FW_VERSION_MINOR    1

node_type_t GLOBAL_device_type = DEVICE_TYPE;
node_status_t GLOBAL_device_status = NODEST_UNDEFINED;



// SPECIFIC INCLUDES
#if DEVICE_TYPE == DEVICE_TYPE_LED_BOARD
    #include  "led_board_driver/led_board_driver.h"
#elif DEVICE_TYPE == DEVICE_TYPE_TEMP_SENSOR
    #include "temp_sensor_driver/temp_sensor_driver.h"
#elif DEVICE_TYPE == DEVICE_TYPE_WATER_LEVEL_SENSOR
    #include "water_level_driver/water_level_driver.h"
#else
    #error "Not supported DEVICE_TYPE"
#endif
#endif	/* DEVICE_TYPE_H */

