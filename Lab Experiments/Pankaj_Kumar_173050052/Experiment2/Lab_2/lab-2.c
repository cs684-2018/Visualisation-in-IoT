/*
* Team Id: 

* Author List: Pankaj Kumar (173050052)

* Filename: lab-2.c

* Theme: Use SW1 to change the color of the led (R⇒ G⇒ B⇒ R. . . .) just once pressed instead of long press
		 And count SW2 on each button press.
		 
* Functions: setup(void) , led_pin_config(void) , main() , detectkeypress() , detectkeypress1(), Timer0IntHandler()

* Global Variables: color , curr_val , key_val , sw2status , flag , fl
*
*/

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include "inc/tm4c123gh6pm.h"
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "driverlib/sysctl.h"
#include "driverlib/interrupt.h"
#include "driverlib/gpio.h"
#include "driverlib/timer.h"
#include "inc/hw_gpio.h"
#include "inc/hw_ints.h"
#include <time.h>

uint8_t color=2;
int curr_val=1,flag=0;
int key_val=1,sw2status=0,fl=0;


/*

* Function Name: detectkeypress()

* Input: none

* Output: integer value to caller function whether key corrosponding to PIN 0 is pressed or not

* Description: tells whether key is pressed or not by setting the variable flag

* Example Call: detectkeypress();

*/

int detectkeypress(){
	if(!(GPIOPinRead(GPIO_PORTF_BASE,GPIO_PIN_0))){
		if(curr_val==2){
			flag=1;
			curr_val=0;
		}
		else{
			curr_val++;
			flag=0;
		}
	}
	return flag;
}


/*

* Function Name: detectkeypress1()

* Input: none

* Output: integer value to caller function whether key corrosponding to PIN 4 is pressed or not

* Description: tells whether key is pressed or not by setting the variable fl

* Example Call: detectkeypress1();

*/


int detectkeypress2(){
	if(!(GPIOPinRead(GPIO_PORTF_BASE,GPIO_PIN_4))){
		if(key_val==2){
			fl=1;
			key_val=0;
		}
		else{
			key_val++;
			fl=0;
		}
	}
	return fl;
}

/*

* Function Name: setup()

* Input: none

* Output: none

* Description: Set crystal frequency,enable GPIO Peripherals and unlock Port F pin 0 (PF0)

* Example Call: setup();

*/

void setup(void)
{
	SysCtlClockSet(SYSCTL_SYSDIV_5|SYSCTL_USE_PLL|SYSCTL_XTAL_16MHZ|SYSCTL_OSC_MAIN);
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
	//unlock PF0 based on requirement

	HWREG(GPIO_PORTF_BASE + GPIO_O_LOCK)= GPIO_LOCK_KEY ;
	    HWREG(GPIO_PORTF_BASE + GPIO_O_CR) |= GPIO_PIN_0 ;
	    HWREG(GPIO_PORTF_BASE + GPIO_O_CR) |= GPIO_PIN_4 ;
	    HWREG(GPIO_PORTF_BASE + GPIO_O_LOCK)= 0 ;

    GPIOPadConfigSet(GPIO_PORTF_BASE, GPIO_PIN_0 | GPIO_PIN_4 , GPIO_STRENGTH_2MA,    GPIO_PIN_TYPE_STD_WPU);
}



/*

* Function Name: led_pin_config()

* Input: none

* Output: none

* Description: Set Port F Pin 1, Pin 2, Pin 3 as output. On this pin Red, Blue and Green LEDs are connected.
			   Set Port F Pin 0 and 4 as input, enable pull up on both these pins.

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

* Logic: Iterate infinetely and calls function Timer0IntHandler() on interrupt to handle SW1 and SW2 event

* Example Call: Called automatically by the Operating System

*/



int main(void)
{
	uint32_t ui32Period;  // variable to handle the time
	setup();
	led_pin_config();


	SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER0);
	TimerConfigure(TIMER0_BASE, TIMER_CFG_PERIODIC);
	ui32Period = (SysCtlClockGet() / 10) / 2;  // calculating the time value


	TimerLoadSet(TIMER0_BASE, TIMER_A, ui32Period -1);
	IntEnable(INT_TIMER0A);

	TimerIntEnable(TIMER0_BASE, TIMER_TIMA_TIMEOUT);
	IntMasterEnable();
	TimerEnable(TIMER0_BASE, TIMER_A);

	GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3, color);
	while(1){

	}
}


/*

* Function Name: Timer0IntHandler()

* Input: none

* Output: none

* Description: function to set the color of LED on pressing SW1 and counting the SW2 value.

* Example Call: Timer0IntHandler();

*/


void Timer0IntHandler(void)
{
	// Clear the timer interrupt
	TimerIntClear(TIMER0_BASE, TIMER_TIMA_TIMEOUT);

// Read the current state of the GPIO pin and
// write back the opposite state
	if(!(GPIOPinRead(GPIO_PORTF_BASE,GPIO_PIN_4)) && detectkeypress2()){
		sw2status++;
		while(!(GPIOPinRead(GPIO_PORTF_BASE,GPIO_PIN_4)));
	}

if(!(GPIOPinRead(GPIO_PORTF_BASE,GPIO_PIN_0)) && detectkeypress())
{
			if(color==2)
				color=8;  // setting green color
			else if(color==8)
				color=4;  // setting blue color
			else if(color==4)
				color=2;	// setting red color
			while(!(GPIOPinRead(GPIO_PORTF_BASE,GPIO_PIN_0)))
				GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3, color);
}


}
