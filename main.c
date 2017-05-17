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

/*----------Flags--------------------*/
int sleep_flag = 0;         // Used to tell functions whether the device is asleep
int sleep_flash = 0;        // Used to tell the decimal point to flash in sleep mode
int measure_flag = 0;       // Used to tell the device when to measure heart rate


/*----------Function Prototypes------*/
void Start();

/*----------Interrupt Prototypes-----*/
CY_ISR_PROTO(sleep_isrHandler);
CY_ISR_PROTO(button_isrHandler);
CY_ISR_PROTO(display_isrHandler);



int main(void)
{
    
    CyGlobalIntEnable; /* Enable global interrupts. */
    displayNumber = 1583; // Random test number
    /* Place your initialization/startup code here (e.g. MyInst_Start()) */
    
    Start();
    for(;;)
    {
        if (measure_flag) {
            LED_Driver_WriteString7Seg("calc",0);
            // TODO: Write function to calibrate LED brightness 
            CyDelay(2000);
            LED_Driver_WriteString7Seg("read",0);
            CyDelay(2000);
            // TODO: Write function to take heart rate measurement
            measure_flag = 0;
            sleep_isr_SetPending();                         //Puts the device to sleep
        }
        
    }
    
    
}

void Start() {//start up flashing sequence
    
        EEPROM_Enable();
        LED_Driver_Start();
        sleep_isr_StartEx(sleep_isrHandler);
        button_isr_StartEx(button_isrHandler);
        display_isr_StartEx(display_isrHandler);
        
        
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
        
        sleep_timer_Start();                            //Allows the startup sequence to run without being interrupted by the sleep isr
        Display_timer_Start();
        sleep_isr_SetPending();                         //Puts the device to sleep
}

/*-----------------Interrupts--------------*/
CY_ISR(sleep_isrHandler)
    //This interrupt makes the screen go blank except for the decimal point when the ultrasonic is not in use
    {
        /* Clear pending Interrupt */
        sleep_isr_ClearPending();
        sleep_timer_ReadStatusRegister();
        display_isr_ClearPending();     //Stops the display updating after this interrupt has run
        Display_timer_ReadStatusRegister();
        
        /* Code */
        LED_Driver_ClearDisplayAll(); //Sets all display segments blank
        LED_Driver_SetDisplayRAM((uint8) 0x80, 1);
        sleep_flag = 1;               //Tells the device it is now asleep
        sleep_timer_Stop();           //Puts the timer calling the sleep isr to sleep so sleep isr isn't triggering every 2 seconds while device is sleeping
        
    }
    
CY_ISR(button_isrHandler)
    {
        /* Clear pending Interrupt */
        button_isr_ClearPending();
        Button_1_ClearInterrupt();
        
        /* Code */
        sleep_flag = 0;                 //Tells the device it is now awake
        measure_flag = 1;               //Tells the device to start measuring the heart rate

    }
    
CY_ISR(display_isrHandler)
    {
        /* Clear pending Interrupt */
        display_isr_ClearPending();
        Display_timer_ReadStatusRegister();
        
        /* Code */
        if (sleep_flag) {
            if (sleep_flash){
                LED_Driver_ClearDisplayAll();
                sleep_flash = 0;
            }
            else {
                LED_Driver_SetDisplayRAM((uint8) 0x80, 1);
                sleep_flash = 1;
            }
        }
        
        
    }

/* [] END OF FILE */
