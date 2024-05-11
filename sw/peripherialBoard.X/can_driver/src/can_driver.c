/*
 * File:   can_driver.c
 * Author: jakub
 *
 * Created on 8. kv?tna 2024, 15:27
 */

#include "../can_driver.h"

void CAN_SendDebugPrint(const char* txData)
{
    struct CAN_MSG_OBJ Transmission;  //create the CAN message object
    Transmission.field.brs=CAN_NON_BRS_MODE; // No bit rate switching
    Transmission.field.dlc=DLC_8; //8 data bytes
    Transmission.field.formatType=CAN_2_0_FORMAT; //CAN 2.0 frames 
    Transmission.field.frameType=CAN_FRAME_DATA; //Data frame
    Transmission.field.idType=CAN_FRAME_STD; //Standard ID
    Transmission.msgId=(CAN_DEBUG_PRINT | MY_CAN_ADDRESS);
    
    
    uint8_t Transmit_Data[8] = {0x00}; // data bytes
    
    uint8_t index = 0;
    while(*txData != '\0')
    {
        Transmit_Data[index++] = *txData;
        if(index == 8)
        {
            Transmission.data=Transmit_Data;
            // wait for FIFO
            while(CAN_TX_FIFO_AVAILABLE != CAN1_TransmitFIFOStatusGet(CAN1_TXQ));//ensure that the TXQ has space for a message
            CAN1_Transmit(CAN1_TXQ, &Transmission); //transmit frame
            index = 0;
        }
        txData++;
    }
    if(index > 0)
    {
        Transmission.data=Transmit_Data;
        Transmission.field.dlc=index;
        // wait for FIFO
        while(CAN_TX_FIFO_AVAILABLE != CAN1_TransmitFIFOStatusGet(CAN1_TXQ));//ensure that the TXQ has space for a message
        CAN1_Transmit(CAN1_TXQ, &Transmission); //transmit frame
        index = 0;
    }
}

void CAN_SendTMCmd(uint8_t cmd_num, uint8_t data_len, uint8_t data[7])
{
    struct CAN_MSG_OBJ Transmission;  //create the CAN message object
    Transmission.field.brs=CAN_NON_BRS_MODE; // No bit rate switching
    Transmission.field.formatType=CAN_2_0_FORMAT; //CAN 2.0 frames 
    Transmission.field.frameType=CAN_FRAME_DATA; //Data frame
    Transmission.field.idType=CAN_FRAME_STD; //Standard ID
    Transmission.msgId=(CAN_TO_MASTER | MY_CAN_ADDRESS);
    
    
    uint8_t Transmit_Data[8] = {0x00}; // data bytes
    Transmit_Data[0] = cmd_num;
    memcpy(&Transmit_Data[1], &data[0], data_len);
    Transmission.data=Transmit_Data;
    
    Transmission.field.dlc= (data_len+1);
    // wait for FIFO
    while(CAN_TX_FIFO_AVAILABLE != CAN1_TransmitFIFOStatusGet(CAN1_TXQ));//ensure that the TXQ has space for a message
    CAN1_Transmit(CAN1_TXQ, &Transmission); //transmit frame
}

// FIFO 1
void CAN1_RX_BR_FIFOHandler(void)
{
    struct CAN_MSG_OBJ rxMessage;  //create a message object for holding the data
    while(1)
    {
        if(CAN1_ReceivedMessageCountGet() > 0) //check for received message
        {
            if(CAN1_Receive(&rxMessage)) //receive the message
            {
                uint8_t cmd_number = CAN_SID_to_adress(rxMessage.msgId);
                
                // DEBUG
                char formattedData[20]; // Adjust the buffer size as needed
                sprintf(formattedData, "REC BR CMD %d", cmd_number);
                CAN_SendDebugPrint(formattedData);

                // proccess commands
//                switch(cmd_number)
//                {
//                    case
//                }
                break;
            }
        }
    }
}

// FIFO 2
void CAN1_RX_TS_FIFOHandler(void)
{
   struct CAN_MSG_OBJ rxMessage;  //create a message object for holding the data
    while(1)
    {
        if(CAN1_ReceivedMessageCountGet() > 0) //check for received message
        {
            if(CAN1_Receive(&rxMessage)) //receive the message
            {
                // check correct recipient                
                if(MY_CAN_ADDRESS != CAN_SID_to_adress(rxMessage.msgId))
                    return;
                
                // check DLC (minimum for TS command is 1)
                if(rxMessage.field.dlc < 1)
                    return;
                
                uint8_t cmd_number = rxMessage.data[0];
                               
                // DEBUG
                char formattedData[20]; // Adjust the buffer size as needed
                sprintf(formattedData, "REC TS CMD %d", cmd_number);
                CAN_SendDebugPrint(formattedData);

//                 proccess commands
                uint8_t data[7] = {0,0,0,0,0,0,0};
                switch(cmd_number)
                {
                    case CAN_TS_RST:
                        CAN_SendTMCmd(CAN_TM_RST_RES,0,data);
                        // wait for rewsponse to send
                        while(CAN_TX_FIFO_AVAILABLE != CAN1_TransmitFIFOStatusGet(CAN1_TXQ));//ensure that the TXQ has space for a message
                        __delay_ms(2000);
                        Reset();
                        break;
                        
                    case CAN_TS_TEMP_SENS_GET_TEMP:
                         if(ds18b20_read(&data[0]))
                            {
//                                sprintf(formattedData, "tmp 0x%04X ", raw_temp);
//                                CAN_SendDebugPrint(formattedData);
                                  CAN_SendTMCmd(CAN_TM_TEMP_SENS_GET_TEMP_RES,2,data);
                            }
                            else
                            {
                                CAN_SendDebugPrint("temp error...");
                            }
                }
                break;
            }
        }
    }
}