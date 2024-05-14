/*
 * File:   can_commands.c
 * Author: jakub
 *
 * Created on 12. kv?tna 2024, 8:42
 */


#include "../can_commands.h"

static uint8_t data[6] = {0u};
void can_cmd_reset(void)
{
    CAN_SendTMCmd(CAN_TM_RST_RES, CANST_OK, 0, data);
    // wait for response to send
    while(CAN_TX_FIFO_AVAILABLE != CAN1_TransmitFIFOStatusGet(CAN1_TXQ));
    __delay_ms(2000);
    Reset();
}

void can_cmd_unsupported(uint8_t cmd_num)
{
    CAN_SendTMCmd(cmd_num,CANST_UNSUPPORTED_CMD,0,data);
}


