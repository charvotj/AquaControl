/*
 * File:   main.c
 * Author: jakub
 *
 * Created on April 13, 2024, 10:21 AM
 */


//#include <xc.h>
#include "mcc_generated_files/system/system.h"
#include "device_type.h"
#include "../shared/CAN_definitions.h"

#define MY_CAN_ADDRESS 4

int counter_second = 0;
uint8_t led_brightness = 0;

void CAN_SendTestMessage(void)
{
    struct CAN_MSG_OBJ Transmission;  //create the CAN message object
    uint8_t Transmit_Data[8]={0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00}; // data bytes
    Transmission.field.brs=CAN_NON_BRS_MODE; // No bit rate switching
    Transmission.field.dlc=DLC_0; //8 data bytes
    Transmission.field.formatType=CAN_2_0_FORMAT; //CAN 2.0 frames 
    Transmission.field.frameType=CAN_FRAME_DATA; //Data frame
    Transmission.field.idType=CAN_FRAME_STD; //Standard ID
    Transmission.msgId=0x100; //ID of 0x100
    Transmission.data=Transmit_Data; //transmit the data from the data bytes
    if(CAN_TX_FIFO_AVAILABLE == (CAN1_TransmitFIFOStatusGet(CAN1_TXQ) & CAN_TX_FIFO_AVAILABLE))//ensure that the TXQ has space for a message
    {
        CAN1_Transmit(CAN1_TXQ, &Transmission); //transmit frame
    }  
}

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

void timer0ISR(void)
{
    counter_second++;
    
    if(counter_second >= 2)
    {
//        LED_PIN_Toggle();
        UART1_println("ping\0");
        CAN_SendDebugPrint("debug\0");
    }
    LED1_FB_Toggle();
    LED0_FB_Toggle();
//    led_brightness += 20;
//    led_set_brightness(LED_CH_0,led_brightness);
//    
}



void CAN1_FIFO1CustomHandler(void)
{

    struct CAN_MSG_OBJ EchoMessage;  //create a message object for holding the data
    while(1)
    {
        if(CAN1_ReceivedMessageCountGet() > 0) //check for received message
        {
//            LED_PIN_Toggle();
            if(CAN1_Receive(&EchoMessage)) //receive the message
            {
//                LED_PIN_Toggle();
                UART1_println("Message received\0");
                break;
            }
        }
    }
    EchoMessage.msgId=0x0b2; //Change the ID to 0x222 then echo the message back out
    if(CAN_TX_FIFO_AVAILABLE == (CAN1_TransmitFIFOStatusGet(CAN1_TXQ) & CAN_TX_FIFO_AVAILABLE))
    {
        CAN1_Transmit(CAN1_TXQ, &EchoMessage); //Send the message
    }
}

void CAN1_FIFO2CustomHandler(void)
{
    struct CAN_MSG_OBJ InternalMessage; //create a message object for holding data
    while(1)
    {
        if(CAN1_ReceivedMessageCountGet() > 0) //check for received message
        {
            if(CAN1_Receive(&InternalMessage)) //receive the message
            {
                break;
            }
        }
    }
    LATAbits.LATA2 = ~(InternalMessage.data[0] & 0b1); // set if first bit of first byte is 1 or 0
}

void main(void) 
{
    SYSTEM_Initialize();
    TMR0_OverflowCallbackRegister(timer0ISR);
    INTCON0bits.GIE = 1;
    
    while(counter_second < 3)
    {
        
    }
  
    CAN1_Initialize();
    UART1_println("CAN init\0");
    
    led_disable_channel(LED_CH_0);
    led_enable_channel(LED_CH_1);
    
    
    
    CAN1_FIFO1NotEmptyCallbackRegister(CAN1_FIFO1CustomHandler);
    CAN1_FIFO2NotEmptyCallbackRegister(CAN1_FIFO2CustomHandler);
    

    
    
            
    // If using interrupts in PIC18 High/Low Priority Mode you need to enable the Global High and Low Interrupts 
    // If using interrupts in PIC Mid-Range Compatibility Mode you need to enable the Global Interrupts 
    // Use the following macros to: 

    // Enable the Global Interrupts 
//    INTERRUPT_GalobalInterruptEnable(); 
//    LED_PIN_Toggle();
    while(1)
    {
    }    
    return;
}
