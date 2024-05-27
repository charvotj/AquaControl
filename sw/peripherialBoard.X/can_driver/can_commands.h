/* 
 * File:   
 * Author: 
 * Comments:
 * Revision history: 
 */

// This is a guard condition so that contents of this file are not included
// more than once.  
#ifndef CAN_COMMANDS_H
#define	CAN_COMMANDS_H

#include "../mcc_generated_files/system/system.h"
#include "can_driver.h"
#include <stdint.h>
#include "string.h"

void can_cmd_reset(void);
void can_cmd_send_sn(void);
void can_cmd_send_node_type(void);
void can_cmd_send_status(void);

void can_cmd_unsupported(uint8_t cmd_num);





#ifdef	__cplusplus
extern "C" {
#endif /* __cplusplus */

    // TODO If C++ is being used, regular C code needs function names to have C 
    // linkage so the functions can be used by the c code. 

#ifdef	__cplusplus
}
#endif /* __cplusplus */

#endif	/* CAN_COMMANDS_H */

