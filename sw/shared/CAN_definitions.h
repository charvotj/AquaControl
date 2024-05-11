#ifndef CAN_DEFINITIONS_H
#define CAN_DEFINITIONS_H

// Macros
#define CAN_SID_to_adress(sid)  (uint8_t)(sid & 0b00011111111)

// COMMAND TYPES
#define CAN_CMD_TYPE_MASK (0b111 << 8)
#define CAN_BROADCAST     (0b000 << 8)
#define CAN_TO_MASTER     (0b010 << 8)
#define CAN_TO_SLAVE      (0b100 << 8)
#define CAN_DEBUG_PRINT   (0b111 << 8)

/********************************************
 *  GENERAL COMMANDS
*********************************************/
// TO SLAVE COMMANDS
#define CAN_TS_RST            (uint8_t)0x00
#define CAN_TS_RES_DEF        (uint8_t)0x01
#define CAN_TS_MUTE           (uint8_t)0x02
#define CAN_TS_PING           (uint8_t)0x03
#define CAN_TS_GET_SN         (uint8_t)0x04
#define CAN_TS_GET_NODE_TYPE  (uint8_t)0x05
#define CAN_TS_GET_STATUS     (uint8_t)0x06
#define CAN_TS_GET_TEMP       (uint8_t)0x07
#define CAN_TS_GET_UPTIME     (uint8_t)0x08

// TO MASTER COMMANDS - cmd number for response is the same, only command type is changing
#define CAN_TM_RST_RES            CAN_TS_RST
#define CAN_TM_RES_DEF_RES        CAN_TS_RES_DEF
#define CAN_TM_MUTE_RES           CAN_TS_MUTE
#define CAN_TM_PONG               CAN_TS_PING
#define CAN_TM_GET_SN_RES         CAN_TS_GET_SN
#define CAN_TM_GET_NODE_TYPE_RES  CAN_TS_GET_NODE_TYPE
#define CAN_TM_GET_STATUS_RES     CAN_TS_GET_STATUS
#define CAN_TM_GET_TEMP_RES       CAN_TS_GET_TEMP
#define CAN_TM_GET_UPTIME_RES     CAN_TS_GET_UPTIME

/********************************************
 *  PERIPHERIAL SPECIFIC COMMANDS - temperature sensor
*********************************************/
// TO SLAVE COMMANDS
#define CAN_TS_TEMP_SENS_GET_TEMP            (uint8_t)0x80


// TO MASTER COMMANDS - cmd number for response is the same, only command type is changing
#define CAN_TM_TEMP_SENS_GET_TEMP_RES        CAN_TS_TEMP_SENS_GET_TEMP





#endif /* CAN_DEFINITIONS_H*/

