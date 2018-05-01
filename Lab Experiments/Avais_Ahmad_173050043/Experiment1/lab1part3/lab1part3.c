/*

* Team Id: 

* Author List: Avais Ahmad (173050043)

* Filename: lab1part3.c

* Theme: Pressing SW1 cycles through the LED delay (0.5s , 1s, 2s) 
		 Pressing SW2 change the color of LED  (RED -> GREEN -> BLUE) 

* Functions: setup(void) , led_pin_config(void) , main() , inter2() , interruptManager()

* Global Variables: ui8LED , delayMul

*/

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_types.h"
#include "inc/hw_gpio.h"
//#include "inc/tm4c123gh6pm.h"	//for interrupt
#include "inc/hw_memmap.h"
#include "driverlib/sysctl.h"
#include "driverlib/gpio.h"
#include "driverlib/interrupt.h" //added to handle interrupt
#include "inc/hw_ints.h"
#include <time.h>

//global variables
int delayMul=1;
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
    
	//enabling pullup
	GPIOPadConfigSet(GPIO_PORTF_BASE,GPIO_PIN_0,GPIO_STRENGTH_2MA,GPIO_PIN_TYPE_STD_WPU);
	GPIOPadConfigSet(GPIO_PORTF_BASE,GPIO_PIN_4,GPIO_STRENGTH_2MA,GPIO_PIN_TYPE_STD_WPU);
}


/*

* Function Name: inter2()

* Input: none

* Output: none

* Description: inter2() function will change the delay on clicking PIN 4
   			   and change the colour on clicking PIN 0

* Example Call: inter2();

*/


void inter2()
{
	if (GPIOIntStatus(GPIO_PORTF_BASE, true) & GPIO_PIN_0)
	{

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

	if (GPIOIntStatus(GPIO_PORTF_BASE, true) & GPIO_PIN_4)
	{
		if(delayMul==4)
		{
			delayMul=1;
		}
		else
		{
			delayMul=delayMul*2;
		}
	}
	while(GPIOPinRead(GPIO_PORTF_BASE,GPIO_PIN_4)==0 || GPIOPinRead(GPIO_PORTF_BASE,GPIO_PIN_0)==0 );
	GPIOIntClear (GPIO_PORTF_BASE, GPIO_PIN_4 | GPIO_PIN_0);
}


/*

* Function Name: interruptManager()

* Input: none

* Output: none

* Description: Handle the interrupt 
* Example Call: interruptManager();

*/


void interruptManager()
{
	 IntMasterEnable();


	 GPIOIntRegister(GPIO_PORTF_BASE, inter2);   // Register our handler function for port f

	 GPIOIntTypeSet(GPIO_PORTF_BASE,GPIO_PIN_4,GPIO_FALLING_EDGE );// Configure pf4 for falling edge trigger
	 GPIOIntEnable(GPIO_PORTF_BASE, GPIO_PIN_4); // Enable interrupt for  pf4

	 GPIOIntTypeSet(GPIO_PORTF_BASE,GPIO_PIN_0,GPIO_FALLING_EDGE );// Configure pf0 for falling edge trigger
	 GPIOIntEnable(GPIO_PORTF_BASE, GPIO_PIN_0); // Enable interrupt for  pf0


}



/*

* Function Name: main

* Input: None

* Output: int to inform the caller that the program exited correctly or incorrectly (C code standard)

* Logic: Iterate infinetely and cycle through delay using delay_and_color_Func().

* Example Call: Called automatically by the Operating System

*/


int main(void)
{
    
    setup();
    pin_config();
    interruptManager();


    while(1)
    {
		
		GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3, ui8LED);
		SysCtlDelay(6700000*delayMul);
		GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3, 0x00);
		SysCtlDelay(6700000*delayMul);




/*
    	// Cycle through Red, Green and Blue LEDs

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
    	    break;*/

    	}

        
    }
