/*
* Team Id: 

* Author List: Avais Ahmad (173050043)

* Filename: lab-1.c

* Theme: Use switch SW1 to Turn on Red LED on first switch press, 
		 Green LED on second switch press and Blue LED on third switch press.

* Functions: setup(void) , led_pin_config(void) , main()

* Global Variables: pin0, ui8LED
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


uint8_t pin0 = 0;
	uint8_t ui8LED = 2;

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

    HWREG(GPIO_PORTF_BASE+GPIO_O_CR) |= GPIO_PIN_4;

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
	GPIOPinTypeGPIOInput(GPIO_PORTF_BASE, GPIO_PIN_0|GPIO_PIN_4);
    
	GPIOPadConfigSet(GPIO_PORTF_BASE,GPIO_PIN_0,GPIO_STRENGTH_2MA,GPIO_PIN_TYPE_STD_WPU);
	GPIOPadConfigSet(GPIO_PORTF_BASE,GPIO_PIN_4,GPIO_STRENGTH_2MA,GPIO_PIN_TYPE_STD_WPU);


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
    



    setup();
    pin_config();

    while(1)
    {
		/*
			Refer to helper document for writing logic for different parts of the lab-1 problem statement
		
		*/

    	pin0= GPIOPinRead(GPIO_PORTF_BASE,GPIO_PIN_0);  //reading value of pf0


    	while(pin0==1)			//busy waiting till the button is pressed
    	{
    	pin0= GPIOPinRead(GPIO_PORTF_BASE,GPIO_PIN_0);  //reading value of pf0
    	}

    	while(pin0==0)
    	{
    		GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3, ui8LED);
    		pin0= GPIOPinRead(GPIO_PORTF_BASE,GPIO_PIN_0);  //reading value of pf0
    	}
    	GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3, 0x00);

 //calculating the next color of LED

    	switch(ui8LED)
    	{
    	case 2:
    		ui8LED=8;
    		break;
    	case 8:
    	    ui8LED=4;
    		break;
    	case 4:
    	    ui8LED=2;
    	    break;

    	}

        
    }
}
