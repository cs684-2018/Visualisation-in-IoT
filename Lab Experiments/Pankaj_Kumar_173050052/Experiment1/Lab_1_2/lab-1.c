/*
* Team Id: 

* Author List: Pankaj Kumar (173050052)

* Filename: lab-1.c

* Theme: Count how many times SW2 is pressed. 

* Functions: setup(void) , led_pin_config(void) , main()

* Global Variables: NONE
*
*/

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_types.h"
#include "inc/hw_gpio.h"
#include "inc/hw_memmap.h"
#include "driverlib/sysctl.h"
#include "driverlib/gpio.h"
#include "inc/hw_ints.h"
#include <time.h>


/*

* Function Name: setup()

* Input: none

* Output: none

* Description: Set crystal frequency,enable GPIO Peripherals and unlock Port F pin 0 (PF0)

* Example Call: setup();

*/

void setup(void)
{
    SysCtlClockSet(SYSCTL_SYSDIV_4|SYSCTL_USE_PLL|SYSCTL_XTAL_16MHZ|SYSCTL_OSC_MAIN);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
    
	//unlock PF0 based on requirement

    HWREG(GPIO_PORTF_BASE + GPIO_O_LOCK)= GPIO_LOCK_KEY ;
    HWREG(GPIO_PORTF_BASE + GPIO_O_CR) |= GPIO_PIN_0 ;
    HWREG(GPIO_PORTF_BASE + GPIO_O_LOCK)= 0 ;

    GPIOPadConfigSet(GPIO_PORTF_BASE, GPIO_PIN_0 , GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD_WPU);

}



/*

* Function Name: pin_config()

* Input: none

* Output: none

* Description: Set Port F Pin 1, Pin 2, Pin 3 as output. On this pin Red, Blue and Green LEDs are connected.
			   Set Port F Pin 0 and 4 as input, enable pull up on both these pins.

* Example Call: pin_config();

*/

void pin_config(void)
{
	GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3);
	GPIOPinTypeGPIOInput(GPIO_PORTF_BASE, GPIO_PIN_0);
}




/*

* Function Name: main

* Input: None

* Output: int to inform the caller that the program exited correctly or incorrectly (C code standard)

* Logic: Iterate infinetely and count number of times SW2 is pressed.

* Example Call: Called automatically by the Operating System

*/



int main(void)
{
    
	uint8_t pinVal=1;    // variable to hold the pinRead

	int turn=0;   		// Tells whether previously button is pressed or not

	int sw2status=0;   // count number of SW2 pressed
    
    setup();
    pin_config();

			while(1)
			{
					
				pinVal= GPIOPinRead(GPIO_PORTF_BASE,GPIO_PIN_0);

				if(pinVal==0 && !turn){

					// if n=button is not pressed then pin value will be zero
				   // And turn also zero So make turn equal to 1 and increase the count

					  turn=1;sw2status++;
					  
					}

				else if(pinVal==1){

					// if button is released make turn again Zero

					GPIOPinWrite(GPIO_PORTF_BASE,GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3, 0);
					turn=0;
				}

			}
}
