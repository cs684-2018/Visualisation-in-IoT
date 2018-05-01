
/*
* Team Id: 

* Author List: Pankaj Kumar (173050052)

* Filename: lab-0.c

* Theme: Getting started with Tiva C Series Launchpad and Code Composer Studio IDE and blink the tri-color LED 

* Functions: setup(void) , led_pin_config(void) , main()

* Global Variables: NONE

*/

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_types.h"
#include "inc/hw_memmap.h"
#include "driverlib/sysctl.h"
#include "driverlib/gpio.h"


/*

* Function Name: setup()

* Input: none

* Output: none

* Description: Set crystal frequency and enable GPIO Peripherals  

* Example Call: setup();

*/
void setup(void)
{
	SysCtlClockSet(SYSCTL_SYSDIV_4|SYSCTL_USE_PLL|SYSCTL_XTAL_16MHZ|SYSCTL_OSC_MAIN);
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
}



/*

* Function Name: led_pin_config()

* Input: none

* Output: none

* Description: Set PORTF Pin 1, Pin 2, Pin 3 as output. On this pin Red, Blue and Green LEDs are connected. 

* Example Call: led_pin_config();

*/

void led_pin_config(void)
{
	GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3);
}




/*

* Function Name: main

* Input: None

* Output: int to inform the caller that the program exited correctly or incorrectly (C code standard)

* Logic: Iterate infinetely and turn on the Red, Green and Blue led periodically

* Example Call: Called automatically by the Operating System

*/


int main(void)
{
	uint8_t ui8LED = 2;


// Call function setup() to set crystal frequency and enable GPIO Peripherals   
	setup();

// Call function led_pin_config() to Set PORTF Pin 1, Pin 2, Pin 3 as output.
	led_pin_config();

	while(1)
	{
		// Turn on the LED
		GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3, ui8LED);

		// Delay for a bit
		SysCtlDelay(20000000);

		// Cycle through Red, Green and Blue LEDs
		if (ui8LED == 8)
		{
			ui8LED = 2;
		}
		else
		{
			ui8LED = ui8LED*2;
		}
	}
}
