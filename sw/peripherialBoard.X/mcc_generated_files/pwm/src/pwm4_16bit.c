/**
  * PWM4_16BIT Generated Driver File.
  *
  * @file pwm4_16bit.c
  *
  * @ingroup pwm4_16bit
  *
  * @brief This file contains the API implementation for the PWM4_16BIT module.
  *
  * version PWM4_16BIT Driver Version 1.0.1
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

/**
 * @section: Included Files
 */

#include <xc.h>
#include "../pwm4_16bit.h"

//Pointers to PWM4_16BIT interrupt handlers
//User can use them in application code to initialize with custom ISRs
static void (*LED_PWM_Slice1Output1_InterruptHandler)(void);   //SaP1IF and hence PWMxIF is set
static void (*LED_PWM_Slice1Output2_InterruptHandler)(void);   //SaP2IF and hence PWMxIF is set
static void (*LED_PWM_Period_InterruptHandler)(void);          //PWMxPIF is set
static void LED_PWM_Slice1Output1_DefaultInterruptHandler(void);
static void LED_PWM_Slice1Output2_DefaultInterruptHandler(void);
static void LED_PWM_Period_DefaultInterruptHandler(void);

void LED_PWM_Initialize(void)
{
    //PWMERS External Reset Disabled; 
    PWM4ERS = 0x0;

    //PWMCLK FOSC; 
    PWM4CLK = 0x2;

    //PWMLDS Autoload disabled; 
    PWM4LDS = 0x0;

    //PWMPRL 31; 
    PWM4PRL = 0x63;

    //PWMPRH 0; 
    PWM4PRH = 0x01;

    //PWMCPRE No prescale; 
    PWM4CPRE = 0x0;

    //PWMPIPOS No postscale; 
    PWM4PIPOS = 0x0;

    //PWMS1P1IF PWM1 output match did not occur; PWMS1P2IF PWM2 output match did not occur; 
    PWM4GIR = 0x0;

    //PWMS1P1IE disabled; PWMS1P2IE disabled; 
    PWM4GIE = 0x0;

    //PWMPOL1 disabled; PWMPOL2 disabled; PWMPPEN disabled; PWMMODE Left aligned mode; 
    PWM4S1CFG = 0x0;

    //PWMS1P1L 6; 
    PWM4S1P1L = 0x0;

    //PWMS1P1H 0; 
    PWM4S1P1H = 0x0;

    //PWMS1P2L 25; 
    PWM4S1P2L = 0x0;

    //PWMS1P2H 0; 
    PWM4S1P2H = 0x0;
    
    //Clear PWM4_16BIT period interrupt flag
    PIR9bits.PWM4PIF = 0;
    
    //Clear PWM4_16BIT interrupt flag
    PIR9bits.PWM4IF = 0;
    
    //Clear PWM4_16BIT slice 1, output 1 interrupt flag
    PWM4GIRbits.S1P1IF = 0;
    
    //Clear PWM4_16BIT slice 1, output 2 interrupt flag
    PWM4GIRbits.S1P2IF = 0;
    
    //PWM4_16BIT interrupt enable bit
    PIE9bits.PWM4IE = 0;
    
    //PWM4_16BIT period interrupt enable bit
    PIE9bits.PWM4PIE = 0;
    
    //Set default interrupt handlers
    LED_PWM_Slice1Output1_SetInterruptHandler(LED_PWM_Slice1Output1_DefaultInterruptHandler);
    LED_PWM_Slice1Output2_SetInterruptHandler(LED_PWM_Slice1Output2_DefaultInterruptHandler);
    LED_PWM_Period_SetInterruptHandler(LED_PWM_Period_DefaultInterruptHandler);

    //PWMEN enabled; PWMLD disabled; PWMERSPOL disabled; PWMERSNOW disabled; 
    PWM4CON = 0x80;
}

void LED_PWM_Enable()
{
    PWM4CON |= _PWM4CON_EN_MASK;
}

void LED_PWM_Disable()
{
    PWM4CON &= (~_PWM4CON_EN_MASK);
}

void LED_PWM_WritePeriodRegister(uint16_t periodCount)
{
    PWM4PRL = (uint8_t)periodCount;
    PWM4PRH= (uint8_t)(periodCount >> 8);
}

void LED_PWM_SetSlice1Output1DutyCycleRegister(uint16_t registerValue)
{    
    PWM4S1P1L = (uint8_t)(registerValue);
    PWM4S1P1H = (uint8_t)(registerValue >> 8);
}

void LED_PWM_SetSlice1Output2DutyCycleRegister(uint16_t registerValue)
{        
    PWM4S1P2L = (uint8_t)(registerValue);
    PWM4S1P2H = (uint8_t)(registerValue >> 8);
}

void LED_PWM_LoadBufferRegisters(void)
{
    //Load the period and duty cycle registers on the next period event
    PWM4CONbits.LD = 1;
}

void LED_PWM_PWMI_ISR(void)
{
    PIR9bits.PWM4IF = 0;
    if((PWM4GIEbits.S1P1IE == 1) && (PWM4GIRbits.S1P1IF == 1))
    {
        PWM4GIRbits.S1P1IF = 0;
        if(LED_PWM_Slice1Output1_InterruptHandler != NULL)
            LED_PWM_Slice1Output1_InterruptHandler();
    }
    else if((PWM4GIEbits.S1P2IE == 1) && (PWM4GIRbits.S1P2IF == 1))
    {
        PWM4GIRbits.S1P2IF = 0;
        if(LED_PWM_Slice1Output2_InterruptHandler != NULL)
            LED_PWM_Slice1Output2_InterruptHandler();
    }
}

void LED_PWM_PWMPI_ISR(void)
{
    PIR9bits.PWM4PIF = 0;
    if(LED_PWM_Period_InterruptHandler != NULL)
        LED_PWM_Period_InterruptHandler();
}

void LED_PWM_Slice1Output1_SetInterruptHandler(void (* InterruptHandler)(void))
{
    LED_PWM_Slice1Output1_InterruptHandler = InterruptHandler;
}

void LED_PWM_Slice1Output2_SetInterruptHandler(void (* InterruptHandler)(void))
{
    LED_PWM_Slice1Output2_InterruptHandler = InterruptHandler;
}

void LED_PWM_Period_SetInterruptHandler(void (* InterruptHandler)(void))
{
    LED_PWM_Period_InterruptHandler = InterruptHandler;
}

static void LED_PWM_Slice1Output1_DefaultInterruptHandler(void)
{
    //Add your interrupt code here or
    //Use LED_PWM_Slice1Output1_SetInterruptHandler() function to use Custom ISR
}

static void LED_PWM_Slice1Output2_DefaultInterruptHandler(void)
{
    //Add your interrupt code here or
    //Use LED_PWM_Slice1Output2_SetInterruptHandler() function to use Custom ISR
}

static void LED_PWM_Period_DefaultInterruptHandler(void)
{
    //Add your interrupt code here or
    //Use LED_PWM_Period_SetInterruptHandler() function to use Custom ISR
}
