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






#endif /* CAN_DEFINITIONS_H*/