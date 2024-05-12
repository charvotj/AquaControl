/*
 * File:   can_commands.c
 * Author: jakub
 *
 * Created on 12. kv?tna 2024, 8:42
 */


#include "../can_commands.h"

static uint8_t data[7] = {0,0,0,0,0,0,0};
void can_cmd_reset()
{
    CAN_SendTMCmd(CAN_TM_RST_RES,0,data);
    // wait for response to send
    while(CAN_TX_FIFO_AVAILABLE != CAN1_TransmitFIFOStatusGet(CAN1_TXQ));
    __delay_ms(2000);
    Reset();
}


