/*
 * main.c
 */
#include <stdint.h>
#include "lm4f232h5qd.h"
int main(void) {
	
	volatile uint32_t ui32Loop;
    //
    // Enable the GPIO port that is used for the on-board LED.
    //
    SYSCTL_RCGC2_R = SYSCTL_RCGC2_GPIOG;

    //
    // Do a dummy read to insert a few cycles after enabling the peripheral.
    //
    ui32Loop = SYSCTL_RCGC2_R;

    //
    // Enable the GPIO pin for the LED (PG2).  Set the direction as output, and
    // enable the GPIO pin for digital function.
    //


    GPIO_PORTG_DIR_R = 0x04;
    GPIO_PORTG_DEN_R = 0x04;


    while(1)
     {
         //
         // Turn on the LED.
         //
         GPIO_PORTG_DATA_R |= 0x04;


         //
         // Delay for a bit.
         //

         for(ui32Loop = 0; ui32Loop < 200000; ui32Loop++)
         {
         }

         //
         // Turn off the LED.
         //
         GPIO_PORTG_DATA_R &= ~(0x04);

         //
         // Delay for a bit.
         //
         for(ui32Loop = 0; ui32Loop < 200000; ui32Loop++)
         {
         }
     }
 }


