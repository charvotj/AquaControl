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

void can_cmd_send_sn(void)
{
    uint8_t data_len = 4u;
    uint8_t data[6] = {0u};
    can_cmd_status st = CANST_OK;
    
    uint32_t SN = SERIAL_NUMBER;
    
    data[0] = (uint8_t)(0xFF & SN);
    data[1] = (uint8_t)(0xFF &(SN >> 8));
    data[2] = (uint8_t)(0xFF &(SN >> 16));
    data[3] = (uint8_t)(0xFF &(SN >> 24));
        
    CAN_SendTMCmd(CAN_TM_GET_SN_RES,st,data_len,data);
}

void can_cmd_send_node_type(void)
{
    uint8_t data_len = 1u;
    uint8_t data[6] = {0u};
    can_cmd_status st = CANST_OK;
    
    uint32_t SN = SERIAL_NUMBER;
    
    data[0] = (uint8_t)(0xFF & GLOBAL_device_type);
    
        
    CAN_SendTMCmd(CAN_TM_GET_NODE_TYPE_RES,st,data_len,data);
}

void can_cmd_send_status(void)
{
    uint8_t data_len = 1u;
    uint8_t data[6] = {0u};
    can_cmd_status st = CANST_OK;
    
    uint32_t SN = SERIAL_NUMBER;
    
    data[0] = (uint8_t)(0xFF & GLOBAL_device_status);
    
        
    CAN_SendTMCmd(CAN_TM_GET_STATUS_RES,st,data_len,data);
}

void can_cmd_unsupported(uint8_t cmd_num)
{
    CAN_SendTMCmd(cmd_num,CANST_UNSUPPORTED_CMD,0,data);
}


