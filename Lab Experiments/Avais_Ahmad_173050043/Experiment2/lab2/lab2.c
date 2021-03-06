/*
* Team Id: 

* Author List: Avais Ahmad (173050043)

* Filename: lab2.c

* Theme: Use SW1 to change the color of the led (R⇒ G⇒ B⇒ R. . . .) just once pressed instead of long press
		 And count SW2 on each button press.
		 
* Functions: setup(void) , led_pin_config(void) , main() , detectkeypress() , Timer0IntHandler()

* Global Variables: ui8LED , sw1, sw2 , key_val , sw2status , press_count
*
*/

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_types.h"
#include "inc/hw_gpio.h"
#include "inc/tm4c123gh6pm.h"	//for definition of interrupt and register assignment
#include "inc/hw_memmap.h"
#include "driverlib/sysctl.h"
#include "driverlib/gpio.h"
#include "driverlib/interrupt.h" //added to handle interrupt
#include "inc/hw_ints.h"
#include "driverlib/timer.h"    //for timer of interrupt
#include <time.h>

//global variables
//int delayMul=1;
uint8_t ui8LED = 2;		//for monitoring the LED color
uint8_t sw1 = 1;	// for monitoring switch 1
uint8_t sw2 = 1;	// for monitoring switch 2
int press_count=0; //how many times is
int sw2status=0; 	//to count no of times sw2 is pressed


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

* Function Name: timer_config()

* Input: none

* Output: none

* Description: Configure the time

* Example Call: timer_config();

*/


void timer_config(void)
{
	SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER0);
	TimerConfigure(TIMER0_BASE, TIMER_CFG_PERIODIC);
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
	 IntEnable(INT_TIMER0A);
	 TimerIntEnable(TIMER0_BASE, TIMER_TIMA_TIMEOUT);
	 IntMasterEnable();

	 TimerEnable(TIMER0_BASE,TIMER_A);

	 /*GPIOIntRegister(GPIO_PORTF_BASE, inter2);   // Register our handler function for port f

	 GPIOIntTypeSet(GPIO_PORTF_BASE,GPIO_PIN_4,GPIO_FALLING_EDGE );// Configure pf4 for falling edge trigger
	 GPIOIntEnable(GPIO_PORTF_BASE, GPIO_PIN_4); // Enable interrupt for  pf4

	 GPIOIntTypeSet(GPIO_PORTF_BASE,GPIO_PIN_0,GPIO_FALLING_EDGE );// Configure pf0 for falling edge trigger
	 GPIOIntEnable(GPIO_PORTF_BASE, GPIO_PIN_0); // Enable interrupt for  pf0

*/
}


/*

* Function Name: detectkeypress()

* Input: none

* Output: integer value to caller function whether key corrosponding to PIN 0 is pressed or not

* Description: tells whether key is pressed or not by setting the variable flag

* Example Call: detectkeypress();

*/


unsigned char detectKeyPress()
{
	int flag=0;			//return 1 if it is confirmed that key is pressed

	sw1=GPIOPinRead(GPIO_PORTF_BASE,GPIO_PIN_4);
	sw2=GPIOPinRead(GPIO_PORTF_BASE,GPIO_PIN_0);
	if(sw1==0  || sw2==0)
	{
		press_count++;
		if(press_count==2)
		{
			press_count=0;
			flag=1;
		}
	}

	return flag;
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
    
    setup();
    pin_config();
    timer_config();
    interruptManager();

    uint32_t ui32Period;

    ui32Period = (SysCtlClockGet() / 10) / 2;
    TimerLoadSet(TIMER0_BASE, TIMER_A, ui32Period -1);


    GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3, ui8LED);

    while(1);	//for infinte execution of the code

        
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

	//system goes to "release" state
	//checking weather sw1 is pressed
	if(GPIOPinRead(GPIO_PORTF_BASE,GPIO_PIN_4)==0 && detectKeyPress()==1 )
	{
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
				break;
			 }

		GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3, ui8LED);

		//system remains in "release" state if the button is pressed
		while(GPIOPinRead(GPIO_PORTF_BASE,GPIO_PIN_4)==0);
	}


	//system goes to "release" state
	//checking weather sw2 is pressed
	if(GPIOPinRead(GPIO_PORTF_BASE,GPIO_PIN_0)==0 && detectKeyPress()==1 )
	{
		sw2status++;
		//system remains in "release" state if the button is pressed
		while(GPIOPinRead(GPIO_PORTF_BASE,GPIO_PIN_0)==0);
	}


	//system goes to "idle" state whe the button is released


}
