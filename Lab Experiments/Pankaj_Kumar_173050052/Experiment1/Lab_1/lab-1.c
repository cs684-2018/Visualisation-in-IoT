/*
* Team Id: 

* Author List: Pankaj Kumar (173050052)

* Filename: lab-1.c

* Theme: Use switch SW1 to Turn on Red LED on first switch press, 
		 Green LED on second switch press and Blue LED on third switch press.

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
    HWREG(GPIO_PORTF_BASE + GPIO_O_CR) |= 0x01 ;
    HWREG(GPIO_PORTF_BASE + GPIO_O_LOCK)= 0 ;

    GPIOPadConfigSet(GPIO_PORTF_BASE, GPIO_PIN_0 | GPIO_PIN_4 , GPIO_STRENGTH_2MA,    GPIO_PIN_TYPE_STD_WPU);

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
	GPIOPinTypeGPIOInput(GPIO_PORTF_BASE, GPIO_PIN_0|GPIO_PIN_4);
}



/*

* Function Name: main

* Input: None

* Output: int to inform the caller that the program exited correctly or incorrectly (C code standard)

* Logic: Iterate infinetely and turn on the Red, Green and Blue led on SW1 click.

* Example Call: Called automatically by the Operating System

*/

int main(void)
{
    
	uint8_t pinVal=0;
	uint8_t counter=0,turn=0;
	int sw2=0;
    setup();
    pin_config();

    while(1){


			while(1)
			{

				//uint32_t pinVal=0;	
			   // variable to hold the pinRead
				pinVal= GPIOPinRead(GPIO_PORTF_BASE,GPIO_PIN_0);

				if(pinVal==0 && counter==0){
					  turn=1;
					  
					  // turn on Red LED

					  GPIOPinWrite(GPIO_PORTF_BASE,GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3, 2);	
					}
				else if(pinVal==0 && counter==1){
					  turn=1;

					  // turn on Green LED

					  GPIOPinWrite(GPIO_PORTF_BASE,GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3, 8);	
					}
				else if(pinVal==0 && counter==2){
					  turn=1;

					  // turn on Blue LED

					  GPIOPinWrite(GPIO_PORTF_BASE,GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3, 4);	
					}
				else{

					// turn off LED

					GPIOPinWrite(GPIO_PORTF_BASE,GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3, 0);
					break;
				}
		
				

			}
			

			// finding the next LED to be turned on.

			if(counter==0 && turn){
				counter=1;turn=0;sw2++;
			}
			else if(counter==1 && turn){
				counter=2;turn=0;sw2++;
			}
			else if(counter==2 && turn) {
				counter=0;turn=0;sw2++;
			}

    }
}
