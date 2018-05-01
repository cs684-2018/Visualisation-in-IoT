/*

* Team Id: 

* Author List: Pankaj Kumar (173050052)

* Filename: lab-1.c

* Theme: Pressing SW1 cycles through the LED delay (0.5s , 1s, 2s) 
		 Pressing SW2 change the color of LED  (RED -> GREEN -> BLUE) 

* Functions: setup(void) , led_pin_config(void) , main() , delay_and_color_Func() , FirstFunc()

* Global Variables: colorVal , delay

*/

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_types.h"
#include "inc/hw_gpio.h"
#include "inc/hw_memmap.h"
#include "driverlib/sysctl.h"
#include "driverlib/interrupt.h"
#include "driverlib/gpio.h"
#include "inc/hw_ints.h"
#include <time.h>

uint8_t colorVal=2;
int delay=1;

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
    HWREG(GPIO_PORTF_BASE + GPIO_O_CR) |= GPIO_PIN_4 ;
    HWREG(GPIO_PORTF_BASE + GPIO_O_LOCK)= 0 ;



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
	GPIOPadConfigSet(GPIO_PORTF_BASE, GPIO_PIN_0|GPIO_PIN_4 , GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD_WPU);
}


/*

* Function Name: delay_and_color_Func()

* Input: none

* Output: none

* Description: Delay_and_color function will change the delay on clicking PIN 4
   			   and change the colour on clicking PIN 0

* Example Call: delay_and_color_Func();

*/


void delay_and_color_Func(){
	if(GPIOIntStatus(GPIO_PORTF_BASE,true) & GPIO_PIN_0){
		if(colorVal==2)
			colorVal=8;  // setting green color
		else if(colorVal==8)
			colorVal=4;  // setting blue color
		else if(colorVal==4)
			colorVal=2;	// setting red color
	}
	if(GPIOIntStatus(GPIO_PORTF_BASE,true) & GPIO_PIN_4){
		if(delay==4)delay=1;
		else delay *= 2;  // increasing delay
	}
	while(GPIOPinRead(GPIO_PORTF_BASE,GPIO_PIN_4)==0||GPIOPinRead(GPIO_PORTF_BASE,GPIO_PIN_0)==0){
		GPIOIntClear(GPIO_PORTF_BASE,GPIO_PIN_4|GPIO_PIN_0);
	}
}


/*

* Function Name: FirstFunc()

* Input: none

* Output: none

* Description: Handle the interrupt 
* Example Call: FirstFunc();

*/


void FirstFunc(){
	IntMasterEnable();
	GPIOIntRegister(GPIO_PORTF_BASE,delay_and_color_Func);
	GPIOIntTypeSet(GPIO_PORTF_BASE,GPIO_PIN_4,GPIO_FALLING_EDGE);
	GPIOIntEnable(GPIO_PORTF_BASE,GPIO_PIN_4);
	GPIOIntTypeSet(GPIO_PORTF_BASE,GPIO_PIN_0,GPIO_FALLING_EDGE);
	GPIOIntEnable(GPIO_PORTF_BASE,GPIO_PIN_0);
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
    FirstFunc();

			while(1)
			{

						GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3,colorVal );
						
						// Delay for a bit

						SysCtlDelay(6700000*delay); 
						GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3, 0);
						
						// Delay for a bit

						SysCtlDelay(6700000*delay);


			}
}
