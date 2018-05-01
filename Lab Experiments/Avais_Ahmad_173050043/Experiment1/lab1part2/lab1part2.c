/*
* Team Id: 

* Author List: Avais Ahmad (173050043)

* Filename: lab1part2.c

* Theme: Count the number of times SW2 is pressed. 

* Functions: setup(void) , led_pin_config(void) , main()

* Global Variables: sw2Status
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

int sw2Status=0;


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

    //enabling the buttons
    //ROM_SysCtlPeripheralEnable(BUTTONS_GPIO_PERIPH);

    //unlocking
    HWREG(GPIO_PORTF_BASE+GPIO_O_LOCK) = GPIO_LOCK_KEY;

    HWREG(GPIO_PORTF_BASE+GPIO_O_CR) |= GPIO_PIN_0;

    //HWREG(GPIO_PORTF_BASE+GPIO_O_CR) |= GPIO_PIN_4;

    HWREG(GPIO_PORTF_BASE+GPIO_O_LOCK) = 0;


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
    
	GPIOPadConfigSet(GPIO_PORTF_BASE,GPIO_PIN_0,GPIO_STRENGTH_2MA,GPIO_PIN_TYPE_STD_WPU);
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
    
	uint32_t pin0 ;


    setup();
    pin_config();

    while(1)
    {
		

    	pin0= GPIOPinRead(GPIO_PORTF_BASE,GPIO_PIN_0);  //reading value of sw2


    	if(pin0==0)
    	{
    		sw2Status=sw2Status +1;

    		//busy wait here till button is released
    		while(pin0==0)
    		{
    			pin0= GPIOPinRead(GPIO_PORTF_BASE,GPIO_PIN_0);  //reading value of sw2
    		}
    	}


        
    }
}
