/**
 * Generated Pins header File
 * 
 * @file pins.h
 * 
 * @defgroup  pinsdriver Pins Driver
 * 
 * @brief This is generated driver header for pins. 
 *        This header file provides APIs for all pins selected in the GUI.
 *
 * @version Driver Version  3.1.0
*/

/*
© [2024] Microchip Technology Inc. and its subsidiaries.

    Subject to your compliance with these terms, you may use Microchip 
    software and any derivatives exclusively with Microchip products. 
    You are responsible for complying with 3rd party license terms  
    applicable to your use of 3rd party software (including open source  
    software) that may accompany Microchip software. SOFTWARE IS ?AS IS.? 
    NO WARRANTIES, WHETHER EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS 
    SOFTWARE, INCLUDING ANY IMPLIED WARRANTIES OF NON-INFRINGEMENT,  
    MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE. IN NO EVENT 
    WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE, 
    INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY 
    KIND WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF 
    MICROCHIP HAS BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE 
    FORESEEABLE. TO THE FULLEST EXTENT ALLOWED BY LAW, MICROCHIP?S 
    TOTAL LIABILITY ON ALL CLAIMS RELATED TO THE SOFTWARE WILL NOT 
    EXCEED AMOUNT OF FEES, IF ANY, YOU PAID DIRECTLY TO MICROCHIP FOR 
    THIS SOFTWARE.
*/

#ifndef PINS_H
#define PINS_H

#include <xc.h>

#define INPUT   1
#define OUTPUT  0

#define HIGH    1
#define LOW     0

#define ANALOG      1
#define DIGITAL     0

#define PULL_UP_ENABLED      1
#define PULL_UP_DISABLED     0

// get/set RA0 aliases
#define LED1_EN_TRIS                 TRISAbits.TRISA0
#define LED1_EN_LAT                  LATAbits.LATA0
#define LED1_EN_PORT                 PORTAbits.RA0
#define LED1_EN_WPU                  WPUAbits.WPUA0
#define LED1_EN_OD                   ODCONAbits.ODCA0
#define LED1_EN_ANS                  ANSELAbits.ANSELA0
#define LED1_EN_SetHigh()            do { LATAbits.LATA0 = 1; } while(0)
#define LED1_EN_SetLow()             do { LATAbits.LATA0 = 0; } while(0)
#define LED1_EN_Toggle()             do { LATAbits.LATA0 = ~LATAbits.LATA0; } while(0)
#define LED1_EN_GetValue()           PORTAbits.RA0
#define LED1_EN_SetDigitalInput()    do { TRISAbits.TRISA0 = 1; } while(0)
#define LED1_EN_SetDigitalOutput()   do { TRISAbits.TRISA0 = 0; } while(0)
#define LED1_EN_SetPullup()          do { WPUAbits.WPUA0 = 1; } while(0)
#define LED1_EN_ResetPullup()        do { WPUAbits.WPUA0 = 0; } while(0)
#define LED1_EN_SetPushPull()        do { ODCONAbits.ODCA0 = 0; } while(0)
#define LED1_EN_SetOpenDrain()       do { ODCONAbits.ODCA0 = 1; } while(0)
#define LED1_EN_SetAnalogMode()      do { ANSELAbits.ANSELA0 = 1; } while(0)
#define LED1_EN_SetDigitalMode()     do { ANSELAbits.ANSELA0 = 0; } while(0)

// get/set RA1 aliases
#define LED1_FB_TRIS                 TRISAbits.TRISA1
#define LED1_FB_LAT                  LATAbits.LATA1
#define LED1_FB_PORT                 PORTAbits.RA1
#define LED1_FB_WPU                  WPUAbits.WPUA1
#define LED1_FB_OD                   ODCONAbits.ODCA1
#define LED1_FB_ANS                  ANSELAbits.ANSELA1
#define LED1_FB_SetHigh()            do { LATAbits.LATA1 = 1; } while(0)
#define LED1_FB_SetLow()             do { LATAbits.LATA1 = 0; } while(0)
#define LED1_FB_Toggle()             do { LATAbits.LATA1 = ~LATAbits.LATA1; } while(0)
#define LED1_FB_GetValue()           PORTAbits.RA1
#define LED1_FB_SetDigitalInput()    do { TRISAbits.TRISA1 = 1; } while(0)
#define LED1_FB_SetDigitalOutput()   do { TRISAbits.TRISA1 = 0; } while(0)
#define LED1_FB_SetPullup()          do { WPUAbits.WPUA1 = 1; } while(0)
#define LED1_FB_ResetPullup()        do { WPUAbits.WPUA1 = 0; } while(0)
#define LED1_FB_SetPushPull()        do { ODCONAbits.ODCA1 = 0; } while(0)
#define LED1_FB_SetOpenDrain()       do { ODCONAbits.ODCA1 = 1; } while(0)
#define LED1_FB_SetAnalogMode()      do { ANSELAbits.ANSELA1 = 1; } while(0)
#define LED1_FB_SetDigitalMode()     do { ANSELAbits.ANSELA1 = 0; } while(0)

// get/set RA2 aliases
#define LED0_EN_TRIS                 TRISAbits.TRISA2
#define LED0_EN_LAT                  LATAbits.LATA2
#define LED0_EN_PORT                 PORTAbits.RA2
#define LED0_EN_WPU                  WPUAbits.WPUA2
#define LED0_EN_OD                   ODCONAbits.ODCA2
#define LED0_EN_ANS                  ANSELAbits.ANSELA2
#define LED0_EN_SetHigh()            do { LATAbits.LATA2 = 1; } while(0)
#define LED0_EN_SetLow()             do { LATAbits.LATA2 = 0; } while(0)
#define LED0_EN_Toggle()             do { LATAbits.LATA2 = ~LATAbits.LATA2; } while(0)
#define LED0_EN_GetValue()           PORTAbits.RA2
#define LED0_EN_SetDigitalInput()    do { TRISAbits.TRISA2 = 1; } while(0)
#define LED0_EN_SetDigitalOutput()   do { TRISAbits.TRISA2 = 0; } while(0)
#define LED0_EN_SetPullup()          do { WPUAbits.WPUA2 = 1; } while(0)
#define LED0_EN_ResetPullup()        do { WPUAbits.WPUA2 = 0; } while(0)
#define LED0_EN_SetPushPull()        do { ODCONAbits.ODCA2 = 0; } while(0)
#define LED0_EN_SetOpenDrain()       do { ODCONAbits.ODCA2 = 1; } while(0)
#define LED0_EN_SetAnalogMode()      do { ANSELAbits.ANSELA2 = 1; } while(0)
#define LED0_EN_SetDigitalMode()     do { ANSELAbits.ANSELA2 = 0; } while(0)

// get/set RA3 aliases
#define LED0_FB_TRIS                 TRISAbits.TRISA3
#define LED0_FB_LAT                  LATAbits.LATA3
#define LED0_FB_PORT                 PORTAbits.RA3
#define LED0_FB_WPU                  WPUAbits.WPUA3
#define LED0_FB_OD                   ODCONAbits.ODCA3
#define LED0_FB_ANS                  ANSELAbits.ANSELA3
#define LED0_FB_SetHigh()            do { LATAbits.LATA3 = 1; } while(0)
#define LED0_FB_SetLow()             do { LATAbits.LATA3 = 0; } while(0)
#define LED0_FB_Toggle()             do { LATAbits.LATA3 = ~LATAbits.LATA3; } while(0)
#define LED0_FB_GetValue()           PORTAbits.RA3
#define LED0_FB_SetDigitalInput()    do { TRISAbits.TRISA3 = 1; } while(0)
#define LED0_FB_SetDigitalOutput()   do { TRISAbits.TRISA3 = 0; } while(0)
#define LED0_FB_SetPullup()          do { WPUAbits.WPUA3 = 1; } while(0)
#define LED0_FB_ResetPullup()        do { WPUAbits.WPUA3 = 0; } while(0)
#define LED0_FB_SetPushPull()        do { ODCONAbits.ODCA3 = 0; } while(0)
#define LED0_FB_SetOpenDrain()       do { ODCONAbits.ODCA3 = 1; } while(0)
#define LED0_FB_SetAnalogMode()      do { ANSELAbits.ANSELA3 = 1; } while(0)
#define LED0_FB_SetDigitalMode()     do { ANSELAbits.ANSELA3 = 0; } while(0)

// get/set RA4 aliases
#define LED0_PG_TRIS                 TRISAbits.TRISA4
#define LED0_PG_LAT                  LATAbits.LATA4
#define LED0_PG_PORT                 PORTAbits.RA4
#define LED0_PG_WPU                  WPUAbits.WPUA4
#define LED0_PG_OD                   ODCONAbits.ODCA4
#define LED0_PG_ANS                  ANSELAbits.ANSELA4
#define LED0_PG_SetHigh()            do { LATAbits.LATA4 = 1; } while(0)
#define LED0_PG_SetLow()             do { LATAbits.LATA4 = 0; } while(0)
#define LED0_PG_Toggle()             do { LATAbits.LATA4 = ~LATAbits.LATA4; } while(0)
#define LED0_PG_GetValue()           PORTAbits.RA4
#define LED0_PG_SetDigitalInput()    do { TRISAbits.TRISA4 = 1; } while(0)
#define LED0_PG_SetDigitalOutput()   do { TRISAbits.TRISA4 = 0; } while(0)
#define LED0_PG_SetPullup()          do { WPUAbits.WPUA4 = 1; } while(0)
#define LED0_PG_ResetPullup()        do { WPUAbits.WPUA4 = 0; } while(0)
#define LED0_PG_SetPushPull()        do { ODCONAbits.ODCA4 = 0; } while(0)
#define LED0_PG_SetOpenDrain()       do { ODCONAbits.ODCA4 = 1; } while(0)
#define LED0_PG_SetAnalogMode()      do { ANSELAbits.ANSELA4 = 1; } while(0)
#define LED0_PG_SetDigitalMode()     do { ANSELAbits.ANSELA4 = 0; } while(0)

// get/set RB0 aliases
#define LED1_CURR_TRIS                 TRISBbits.TRISB0
#define LED1_CURR_LAT                  LATBbits.LATB0
#define LED1_CURR_PORT                 PORTBbits.RB0
#define LED1_CURR_WPU                  WPUBbits.WPUB0
#define LED1_CURR_OD                   ODCONBbits.ODCB0
#define LED1_CURR_ANS                  ANSELBbits.ANSELB0
#define LED1_CURR_SetHigh()            do { LATBbits.LATB0 = 1; } while(0)
#define LED1_CURR_SetLow()             do { LATBbits.LATB0 = 0; } while(0)
#define LED1_CURR_Toggle()             do { LATBbits.LATB0 = ~LATBbits.LATB0; } while(0)
#define LED1_CURR_GetValue()           PORTBbits.RB0
#define LED1_CURR_SetDigitalInput()    do { TRISBbits.TRISB0 = 1; } while(0)
#define LED1_CURR_SetDigitalOutput()   do { TRISBbits.TRISB0 = 0; } while(0)
#define LED1_CURR_SetPullup()          do { WPUBbits.WPUB0 = 1; } while(0)
#define LED1_CURR_ResetPullup()        do { WPUBbits.WPUB0 = 0; } while(0)
#define LED1_CURR_SetPushPull()        do { ODCONBbits.ODCB0 = 0; } while(0)
#define LED1_CURR_SetOpenDrain()       do { ODCONBbits.ODCB0 = 1; } while(0)
#define LED1_CURR_SetAnalogMode()      do { ANSELBbits.ANSELB0 = 1; } while(0)
#define LED1_CURR_SetDigitalMode()     do { ANSELBbits.ANSELB0 = 0; } while(0)

// get/set RB1 aliases
#define LED0_CURR_TRIS                 TRISBbits.TRISB1
#define LED0_CURR_LAT                  LATBbits.LATB1
#define LED0_CURR_PORT                 PORTBbits.RB1
#define LED0_CURR_WPU                  WPUBbits.WPUB1
#define LED0_CURR_OD                   ODCONBbits.ODCB1
#define LED0_CURR_ANS                  ANSELBbits.ANSELB1
#define LED0_CURR_SetHigh()            do { LATBbits.LATB1 = 1; } while(0)
#define LED0_CURR_SetLow()             do { LATBbits.LATB1 = 0; } while(0)
#define LED0_CURR_Toggle()             do { LATBbits.LATB1 = ~LATBbits.LATB1; } while(0)
#define LED0_CURR_GetValue()           PORTBbits.RB1
#define LED0_CURR_SetDigitalInput()    do { TRISBbits.TRISB1 = 1; } while(0)
#define LED0_CURR_SetDigitalOutput()   do { TRISBbits.TRISB1 = 0; } while(0)
#define LED0_CURR_SetPullup()          do { WPUBbits.WPUB1 = 1; } while(0)
#define LED0_CURR_ResetPullup()        do { WPUBbits.WPUB1 = 0; } while(0)
#define LED0_CURR_SetPushPull()        do { ODCONBbits.ODCB1 = 0; } while(0)
#define LED0_CURR_SetOpenDrain()       do { ODCONBbits.ODCB1 = 1; } while(0)
#define LED0_CURR_SetAnalogMode()      do { ANSELBbits.ANSELB1 = 1; } while(0)
#define LED0_CURR_SetDigitalMode()     do { ANSELBbits.ANSELB1 = 0; } while(0)

// get/set RB2 aliases
#define CAN_STBY_PIN_TRIS                 TRISBbits.TRISB2
#define CAN_STBY_PIN_LAT                  LATBbits.LATB2
#define CAN_STBY_PIN_PORT                 PORTBbits.RB2
#define CAN_STBY_PIN_WPU                  WPUBbits.WPUB2
#define CAN_STBY_PIN_OD                   ODCONBbits.ODCB2
#define CAN_STBY_PIN_ANS                  ANSELBbits.ANSELB2
#define CAN_STBY_PIN_SetHigh()            do { LATBbits.LATB2 = 1; } while(0)
#define CAN_STBY_PIN_SetLow()             do { LATBbits.LATB2 = 0; } while(0)
#define CAN_STBY_PIN_Toggle()             do { LATBbits.LATB2 = ~LATBbits.LATB2; } while(0)
#define CAN_STBY_PIN_GetValue()           PORTBbits.RB2
#define CAN_STBY_PIN_SetDigitalInput()    do { TRISBbits.TRISB2 = 1; } while(0)
#define CAN_STBY_PIN_SetDigitalOutput()   do { TRISBbits.TRISB2 = 0; } while(0)
#define CAN_STBY_PIN_SetPullup()          do { WPUBbits.WPUB2 = 1; } while(0)
#define CAN_STBY_PIN_ResetPullup()        do { WPUBbits.WPUB2 = 0; } while(0)
#define CAN_STBY_PIN_SetPushPull()        do { ODCONBbits.ODCB2 = 0; } while(0)
#define CAN_STBY_PIN_SetOpenDrain()       do { ODCONBbits.ODCB2 = 1; } while(0)
#define CAN_STBY_PIN_SetAnalogMode()      do { ANSELBbits.ANSELB2 = 1; } while(0)
#define CAN_STBY_PIN_SetDigitalMode()     do { ANSELBbits.ANSELB2 = 0; } while(0)

// get/set RB3 aliases
#define CAN_RX_PIN_TRIS                 TRISBbits.TRISB3
#define CAN_RX_PIN_LAT                  LATBbits.LATB3
#define CAN_RX_PIN_PORT                 PORTBbits.RB3
#define CAN_RX_PIN_WPU                  WPUBbits.WPUB3
#define CAN_RX_PIN_OD                   ODCONBbits.ODCB3
#define CAN_RX_PIN_ANS                  ANSELBbits.ANSELB3
#define CAN_RX_PIN_SetHigh()            do { LATBbits.LATB3 = 1; } while(0)
#define CAN_RX_PIN_SetLow()             do { LATBbits.LATB3 = 0; } while(0)
#define CAN_RX_PIN_Toggle()             do { LATBbits.LATB3 = ~LATBbits.LATB3; } while(0)
#define CAN_RX_PIN_GetValue()           PORTBbits.RB3
#define CAN_RX_PIN_SetDigitalInput()    do { TRISBbits.TRISB3 = 1; } while(0)
#define CAN_RX_PIN_SetDigitalOutput()   do { TRISBbits.TRISB3 = 0; } while(0)
#define CAN_RX_PIN_SetPullup()          do { WPUBbits.WPUB3 = 1; } while(0)
#define CAN_RX_PIN_ResetPullup()        do { WPUBbits.WPUB3 = 0; } while(0)
#define CAN_RX_PIN_SetPushPull()        do { ODCONBbits.ODCB3 = 0; } while(0)
#define CAN_RX_PIN_SetOpenDrain()       do { ODCONBbits.ODCB3 = 1; } while(0)
#define CAN_RX_PIN_SetAnalogMode()      do { ANSELBbits.ANSELB3 = 1; } while(0)
#define CAN_RX_PIN_SetDigitalMode()     do { ANSELBbits.ANSELB3 = 0; } while(0)

// get/set RB4 aliases
#define CAN_TX_PIN_TRIS                 TRISBbits.TRISB4
#define CAN_TX_PIN_LAT                  LATBbits.LATB4
#define CAN_TX_PIN_PORT                 PORTBbits.RB4
#define CAN_TX_PIN_WPU                  WPUBbits.WPUB4
#define CAN_TX_PIN_OD                   ODCONBbits.ODCB4
#define CAN_TX_PIN_ANS                  ANSELBbits.ANSELB4
#define CAN_TX_PIN_SetHigh()            do { LATBbits.LATB4 = 1; } while(0)
#define CAN_TX_PIN_SetLow()             do { LATBbits.LATB4 = 0; } while(0)
#define CAN_TX_PIN_Toggle()             do { LATBbits.LATB4 = ~LATBbits.LATB4; } while(0)
#define CAN_TX_PIN_GetValue()           PORTBbits.RB4
#define CAN_TX_PIN_SetDigitalInput()    do { TRISBbits.TRISB4 = 1; } while(0)
#define CAN_TX_PIN_SetDigitalOutput()   do { TRISBbits.TRISB4 = 0; } while(0)
#define CAN_TX_PIN_SetPullup()          do { WPUBbits.WPUB4 = 1; } while(0)
#define CAN_TX_PIN_ResetPullup()        do { WPUBbits.WPUB4 = 0; } while(0)
#define CAN_TX_PIN_SetPushPull()        do { ODCONBbits.ODCB4 = 0; } while(0)
#define CAN_TX_PIN_SetOpenDrain()       do { ODCONBbits.ODCB4 = 1; } while(0)
#define CAN_TX_PIN_SetAnalogMode()      do { ANSELBbits.ANSELB4 = 1; } while(0)
#define CAN_TX_PIN_SetDigitalMode()     do { ANSELBbits.ANSELB4 = 0; } while(0)

// get/set RB5 aliases
#define LED1_PG_TRIS                 TRISBbits.TRISB5
#define LED1_PG_LAT                  LATBbits.LATB5
#define LED1_PG_PORT                 PORTBbits.RB5
#define LED1_PG_WPU                  WPUBbits.WPUB5
#define LED1_PG_OD                   ODCONBbits.ODCB5
#define LED1_PG_ANS                  ANSELBbits.ANSELB5
#define LED1_PG_SetHigh()            do { LATBbits.LATB5 = 1; } while(0)
#define LED1_PG_SetLow()             do { LATBbits.LATB5 = 0; } while(0)
#define LED1_PG_Toggle()             do { LATBbits.LATB5 = ~LATBbits.LATB5; } while(0)
#define LED1_PG_GetValue()           PORTBbits.RB5
#define LED1_PG_SetDigitalInput()    do { TRISBbits.TRISB5 = 1; } while(0)
#define LED1_PG_SetDigitalOutput()   do { TRISBbits.TRISB5 = 0; } while(0)
#define LED1_PG_SetPullup()          do { WPUBbits.WPUB5 = 1; } while(0)
#define LED1_PG_ResetPullup()        do { WPUBbits.WPUB5 = 0; } while(0)
#define LED1_PG_SetPushPull()        do { ODCONBbits.ODCB5 = 0; } while(0)
#define LED1_PG_SetOpenDrain()       do { ODCONBbits.ODCB5 = 1; } while(0)
#define LED1_PG_SetAnalogMode()      do { ANSELBbits.ANSELB5 = 1; } while(0)
#define LED1_PG_SetDigitalMode()     do { ANSELBbits.ANSELB5 = 0; } while(0)

// get/set RB7 aliases
#define DAC_OUT_TRIS                 TRISBbits.TRISB7
#define DAC_OUT_LAT                  LATBbits.LATB7
#define DAC_OUT_PORT                 PORTBbits.RB7
#define DAC_OUT_WPU                  WPUBbits.WPUB7
#define DAC_OUT_OD                   ODCONBbits.ODCB7
#define DAC_OUT_ANS                  ANSELBbits.ANSELB7
#define DAC_OUT_SetHigh()            do { LATBbits.LATB7 = 1; } while(0)
#define DAC_OUT_SetLow()             do { LATBbits.LATB7 = 0; } while(0)
#define DAC_OUT_Toggle()             do { LATBbits.LATB7 = ~LATBbits.LATB7; } while(0)
#define DAC_OUT_GetValue()           PORTBbits.RB7
#define DAC_OUT_SetDigitalInput()    do { TRISBbits.TRISB7 = 1; } while(0)
#define DAC_OUT_SetDigitalOutput()   do { TRISBbits.TRISB7 = 0; } while(0)
#define DAC_OUT_SetPullup()          do { WPUBbits.WPUB7 = 1; } while(0)
#define DAC_OUT_ResetPullup()        do { WPUBbits.WPUB7 = 0; } while(0)
#define DAC_OUT_SetPushPull()        do { ODCONBbits.ODCB7 = 0; } while(0)
#define DAC_OUT_SetOpenDrain()       do { ODCONBbits.ODCB7 = 1; } while(0)
#define DAC_OUT_SetAnalogMode()      do { ANSELBbits.ANSELB7 = 1; } while(0)
#define DAC_OUT_SetDigitalMode()     do { ANSELBbits.ANSELB7 = 0; } while(0)

// get/set RC2 aliases
#define UART0_RX_TRIS                 TRISCbits.TRISC2
#define UART0_RX_LAT                  LATCbits.LATC2
#define UART0_RX_PORT                 PORTCbits.RC2
#define UART0_RX_WPU                  WPUCbits.WPUC2
#define UART0_RX_OD                   ODCONCbits.ODCC2
#define UART0_RX_ANS                  ANSELCbits.ANSELC2
#define UART0_RX_SetHigh()            do { LATCbits.LATC2 = 1; } while(0)
#define UART0_RX_SetLow()             do { LATCbits.LATC2 = 0; } while(0)
#define UART0_RX_Toggle()             do { LATCbits.LATC2 = ~LATCbits.LATC2; } while(0)
#define UART0_RX_GetValue()           PORTCbits.RC2
#define UART0_RX_SetDigitalInput()    do { TRISCbits.TRISC2 = 1; } while(0)
#define UART0_RX_SetDigitalOutput()   do { TRISCbits.TRISC2 = 0; } while(0)
#define UART0_RX_SetPullup()          do { WPUCbits.WPUC2 = 1; } while(0)
#define UART0_RX_ResetPullup()        do { WPUCbits.WPUC2 = 0; } while(0)
#define UART0_RX_SetPushPull()        do { ODCONCbits.ODCC2 = 0; } while(0)
#define UART0_RX_SetOpenDrain()       do { ODCONCbits.ODCC2 = 1; } while(0)
#define UART0_RX_SetAnalogMode()      do { ANSELCbits.ANSELC2 = 1; } while(0)
#define UART0_RX_SetDigitalMode()     do { ANSELCbits.ANSELC2 = 0; } while(0)

// get/set RC3 aliases
#define UART0_TX_TRIS                 TRISCbits.TRISC3
#define UART0_TX_LAT                  LATCbits.LATC3
#define UART0_TX_PORT                 PORTCbits.RC3
#define UART0_TX_WPU                  WPUCbits.WPUC3
#define UART0_TX_OD                   ODCONCbits.ODCC3
#define UART0_TX_ANS                  ANSELCbits.ANSELC3
#define UART0_TX_SetHigh()            do { LATCbits.LATC3 = 1; } while(0)
#define UART0_TX_SetLow()             do { LATCbits.LATC3 = 0; } while(0)
#define UART0_TX_Toggle()             do { LATCbits.LATC3 = ~LATCbits.LATC3; } while(0)
#define UART0_TX_GetValue()           PORTCbits.RC3
#define UART0_TX_SetDigitalInput()    do { TRISCbits.TRISC3 = 1; } while(0)
#define UART0_TX_SetDigitalOutput()   do { TRISCbits.TRISC3 = 0; } while(0)
#define UART0_TX_SetPullup()          do { WPUCbits.WPUC3 = 1; } while(0)
#define UART0_TX_ResetPullup()        do { WPUCbits.WPUC3 = 0; } while(0)
#define UART0_TX_SetPushPull()        do { ODCONCbits.ODCC3 = 0; } while(0)
#define UART0_TX_SetOpenDrain()       do { ODCONCbits.ODCC3 = 1; } while(0)
#define UART0_TX_SetAnalogMode()      do { ANSELCbits.ANSELC3 = 1; } while(0)
#define UART0_TX_SetDigitalMode()     do { ANSELCbits.ANSELC3 = 0; } while(0)

/**
 * @ingroup  pinsdriver
 * @brief GPIO and peripheral I/O initialization
 * @param none
 * @return none
 */
void PIN_MANAGER_Initialize (void);

/**
 * @ingroup  pinsdriver
 * @brief Interrupt on Change Handling routine
 * @param none
 * @return none
 */
void PIN_MANAGER_IOC(void);


#endif // PINS_H
/**
 End of File
*/