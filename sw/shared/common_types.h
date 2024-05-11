// Need to be defined explicitely that way because of PIC compiler... 
// enum is then using this definition
#define DEVICE_TYPE_UNKNOWN               0       
#define DEVICE_TYPE_UNSUPPORTED           1
#define DEVICE_TYPE_LED_BOARD             2
#define DEVICE_TYPE_TEMP_SENSOR           3
#define DEVICE_TYPE_WATER_LEVEL_SENSOR    4
#define DEVICE_TYPE_PH_SENSOR             5

#ifndef COMMON_TYPES_H
#define COMMON_TYPES_H

#include <stdint.h>


typedef enum
{
    NODE_TYPE_UNKNOWN = DEVICE_TYPE_UNKNOWN,
    NODE_TYPE_UNSUPPORTED = DEVICE_TYPE_UNSUPPORTED,
    NODE_TYPE_LED_BOARD = DEVICE_TYPE_LED_BOARD,
    NODE_TYPE_TEMP_SENSOR = DEVICE_TYPE_TEMP_SENSOR,
    NODE_TYPE_WATER_LEVEL_SENSOR = DEVICE_TYPE_WATER_LEVEL_SENSOR,
    NODE_TYPE_PH_SENSOR = DEVICE_TYPE_PH_SENSOR
} node_type_t;

typedef uint32_t node_sn_t;
typedef uint64_t node_uptime_t; // [ms], only 7 bytes 
typedef uint8_t node_temp_t; //[˚C]

typedef enum {
    NODEST_NORMAL,
    NODEST_STARTUP,
    NODEST_ERROR_GENERIC,
    NODEST_UNDEFINED,
    NODEST_OFFLINE
} node_status_t;


typedef struct
{
    node_type_t node_type;
    uint8_t can_address;
    node_sn_t SN;
    node_status_t status;
} can_node_t;

#define DEFAULT_CAN_NODE {NODE_TYPE_UNKNOWN,0,0,NODEST_UNDEFINED}






#endif /* COMMON_TYPES_H*/

