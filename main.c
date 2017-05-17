/* ========================================
 *
 * Copyright YOUR COMPANY, THE YEAR
 * All Rights Reserved
 * UNPUBLISHED, LICENSED SOFTWARE.
 *
 * CONFIDENTIAL AND PROPRIETARY INFORMATION
 * WHICH IS THE PROPERTY OF your company.
 *
 * ========================================
*/

#include "project.h"


/*----------Global Variables---------*/
int displayNumber;
int displayPosition = 0;    //The digit that the display starts from
int displayDigits = 4;      //The number of digits on the 7-segment display

/*----------Function Prototypes------*/
void Start();

/*----------Interrupt Prototypes-----*/
CY_ISR_PROTO(sleep_isrHandler);



int main(void)
{
    
    CyGlobalIntEnable; /* Enable global interrupts. */
    displayNumber = 1583; // Random test number
    /* Place your initialization/startup code here (e.g. MyInst_Start()) */
    
    Start();
    for(;;)
    {
        
        LED_Driver_Write7SegNumberDec(displayNumber, displayPosition, displayDigits, LED_Driver_RIGHT_ALIGN);
        /*Test*/
        CyDelay(100);
        displayNumber = displayNumber + 1;
        
    }
    
    
}

void Start() {//start up flashing sequence
    
        EEPROM_Enable();
        LED_Driver_Start();
        sleep_isr_StartEx(sleep_isrHandler);
        
        
        LED_Driver_ClearDisplayAll();
        LED_Driver_SetDisplayRAM((uint8) 0xFF, 0);      //Write all sections 'on' to digit 0. 
        CyDelay(1000);
        
        LED_Driver_ClearDisplayAll();                   //Clears the display to nothing.
        LED_Driver_SetDisplayRAM((uint8) 0xFF, 1);      //Write all sections 'on' to digit 1.
        CyDelay(1000);
        
        LED_Driver_ClearDisplayAll();
        LED_Driver_SetDisplayRAM((uint8) 0xFF, 2);   
        CyDelay(1000);
        
        LED_Driver_ClearDisplayAll();
        LED_Driver_SetDisplayRAM((uint8) 0xFF, 3);   
        CyDelay(1000);
        
}

/*-----------------Interrupts--------------*/
CY_ISR(sleep_isrHandler)
    //This interrupt makes the screen go blank except for the decimal point when the ultrasonic is not in use
    {
        /* Clear pending Interrupt */
        sleep_isr_ClearPending();
        sleep_timer_ReadStatusRegister();
        
        /* Code */
//        Timer_1_Sleep();
//        Timer_2_Sleep();
//        sleep_flash_timer_Wakeup(); //Wakes up the flashing function for sleep mode
//        segA_Write(1);
//        segB_Write(1);
//        segC_Write(1);
//        segD_Write(1);
//        segE_Write(1);
//        segF_Write(1);
//        segG_Write(1);
//        segDec_Write(0);
//        digit1_Write(1);
//        digit2_Write(0);
//        digit3_Write(1);
//        digit4_Write(1);
        
    }

/* [] END OF FILE */
