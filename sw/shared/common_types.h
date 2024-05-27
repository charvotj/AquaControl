// Need to be defined explicitely that way because of PIC compiler... 
// enum is then using this definition
#define DEVICE_TYPE_UNKNOWN              -1       
#define DEVICE_TYPE_MASTER                0
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
    NODE_TYPE_MASTER = DEVICE_TYPE_MASTER,
    NODE_TYPE_UNSUPPORTED = DEVICE_TYPE_UNSUPPORTED,
    NODE_TYPE_LED_BOARD = DEVICE_TYPE_LED_BOARD,
    NODE_TYPE_TEMP_SENSOR = DEVICE_TYPE_TEMP_SENSOR,
    NODE_TYPE_WATER_LEVEL_SENSOR = DEVICE_TYPE_WATER_LEVEL_SENSOR,
    NODE_TYPE_PH_SENSOR = DEVICE_TYPE_PH_SENSOR,
    NODE_TYPE_MAX
} node_type_t;

typedef uint32_t node_sn_t;
typedef uint64_t node_uptime_t; // [ms], only 7 bytes 
typedef uint8_t node_temp_t; //[˚C]

typedef enum {
    NODEST_NORMAL,
    NODEST_STARTUP,
    NODEST_ERROR_GENERIC,
    NODEST_UNDEFINED,
    NODEST_OFFLINE,
    NODEST_MAXSTATUS, // to check enum overflow
} node_status_t;

typedef enum {
    CANST_OK = 0,
    CANST_UNSUPPORTED_CMD,
    CANST_GENERAL_ERROR,
    CANST_LED_BOARD_SPECIFIC_ER = 10*DEVICE_TYPE_LED_BOARD,
    CANST_TEMP_SENSOR_SPECIFIC_ER = 10*DEVICE_TYPE_TEMP_SENSOR,
    CANST_WATER_LEVEL_SENSOR_SPECIFIC_ER = 10*DEVICE_TYPE_WATER_LEVEL_SENSOR,
    CANST_PH_SENSOR_SPECIFIC_ER = 10*DEVICE_TYPE_PH_SENSOR,
} can_cmd_status;

typedef struct
{
    uint8_t data_len;
    float* data_p;
} node_data_t;
#define DEFAULT_NODE_DATA (node_data_t){0,NULL}

typedef struct
{
    node_type_t node_type;
    uint8_t can_address;
    node_sn_t SN;
    node_status_t status;
    void* config;
    node_data_t data;
} can_node_t;

// #define DEFAULT_CAN_NODE ((can_node_t){NODE_TYPE_UNKNOWN,0,0,NODEST_UNDEFINED,NULL,DEFAULT_NODE_DATA})
#define INIT_CAN_NODE(node) \
    do { \
        (node).node_type = NODE_TYPE_UNKNOWN; \
        (node).can_address = 0; \
        (node).SN = 0; \
        (node).status = NODEST_UNDEFINED; \
        (node).config = NULL; \
        (node).data = DEFAULT_NODE_DATA; \
    } while(0)








#endif /* COMMON_TYPES_H*/

