 
/*

* Team Id: 

* Author List: Pankaj Kumar (173050052) , Avais Ahmad (173050043)

* Filename: main.c

* Theme: control servo motor using switch

* Functions: interruptManager() , timer_config() , main() , detectKeyPress1() , detectKeyPress2(), Timer0IntHandler()

* Global Variables: i, delay_count, debug, sw1, sw2, press_count, modeCount, ui8Adjust, ui32Load, ui32PWMClock


*/


#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "driverlib/sysctl.h"
#include "driverlib/gpio.h"
#include "driverlib/debug.h"
#include "driverlib/pwm.h"
#include "driverlib/pin_map.h"
#include "inc/hw_gpio.h"
#include "driverlib/rom.h"
#include "driverlib/interrupt.h" //added to handle interrupt
#include "driverlib/timer.h"    //for timer of interrupt
#include "inc/tm4c123gh6pm.h"	//for definition of interrupt and register assignment


#define PWM_FREQUENCY 55

//global variables
int i;
int delay_count=1;  // maintaining the delay value
int debug;			// used for debugging 
uint8_t sw1 = 1;	// for monitoring switch 1
uint8_t sw2 = 1;	// for monitoring switch 2
int press_count=0; 	// counting the key pressing
int modeCount=0;	// tells the mode
volatile uint8_t ui8Adjust;	
volatile uint32_t ui32Load;
volatile uint32_t ui32PWMClock;


/*

* Function Name: interruptManager()

* Input: none

* Output: none

* Description: managing the interrupt 

* Example Call: interruptManager();

*/



void interruptManager()
{
	 IntEnable(INT_TIMER0A);
	 TimerIntEnable(TIMER0_BASE, TIMER_TIMA_TIMEOUT);
	 IntMasterEnable();

	 TimerEnable(TIMER0_BASE,TIMER_A);
}


/*

* Function Name: timer_config()

* Input: none

* Output: none

* Description: configuring the interrupt timer 

* Example Call: timer_config();

*/



void timer_config(void)
{
	SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER0);
	TimerConfigure(TIMER0_BASE, TIMER_CFG_PERIODIC);
}



/*

* Function Name: detectKeyPress1()

* Input: none

* Output: char value to caller function whether key corrosponding to PIN 4 is pressed or not

* Description: tells whether key is pressed or not by setting the variable flag

* Example Call: detectKeyPress1();

*/

unsigned char detectKeyPress1()
{
	int flag=0;			//return 1 if it is confirmed that key is pressed

	sw1=GPIOPinRead(GPIO_PORTF_BASE,GPIO_PIN_4);
	//sw2=GPIOPinRead(GPIO_PORTF_BASE,GPIO_PIN_0);
	if(sw1==0 )
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

* Function Name: detectKeyPress2()

* Input: none

* Output: char value to caller function whether key corrosponding to PIN 0 is pressed or not

* Description: tells whether key is pressed or not by setting the variable flag

* Example Call: detectKeyPress2();

*/

unsigned char detectKeyPress2()
{
	int flag=0;			//return 1 if it is confirmed that key is pressed

	//sw1=GPIOPinRead(GPIO_PORTF_BASE,GPIO_PIN_4);
	sw2=GPIOPinRead(GPIO_PORTF_BASE,GPIO_PIN_0);
	if(sw2==0)
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

* Logic: detecting the key pressing and calculating rotation angle 

* Example Call: Called automatically by the Operating System

*/



int main(void)
{
	timer_config();
	interruptManager();

	uint32_t ui32Period;

	ui32Period = (SysCtlClockGet() / 10) / 2;
	TimerLoadSet(TIMER0_BASE, TIMER_A, ui32Period -1);

	volatile uint32_t ui32Load;
	volatile uint32_t ui32PWMClock;
	volatile uint8_t ui8Adjust;

	ui8Adjust = 83;

	//setting clock
	SysCtlClockSet(SYSCTL_SYSDIV_5|SYSCTL_USE_PLL|SYSCTL_OSC_MAIN|SYSCTL_XTAL_16MHZ);
	SysCtlPWMClockSet(SYSCTL_PWMDIV_64);

	//enabling desired peripherals
	SysCtlPeripheralEnable(SYSCTL_PERIPH_PWM1);
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOD);
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);

	GPIOPinTypePWM(GPIO_PORTD_BASE, GPIO_PIN_0);
	GPIOPinConfigure(GPIO_PD0_M1PWM0);

	//unlocking the switch
	HWREG(GPIO_PORTF_BASE + GPIO_O_LOCK) = GPIO_LOCK_KEY;
	HWREG(GPIO_PORTF_BASE + GPIO_O_CR) |= 0x01;  HWREG(GPIO_PORTF_BASE + GPIO_O_LOCK) = 0;
	GPIODirModeSet(GPIO_PORTF_BASE, GPIO_PIN_4|GPIO_PIN_0, GPIO_DIR_MODE_IN);
	GPIOPadConfigSet(GPIO_PORTF_BASE, GPIO_PIN_4|GPIO_PIN_0, GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD_WPU);

	//calcuulating clock and load
	ui32PWMClock = SysCtlClockGet() / 64;
	ui32Load = (ui32PWMClock / PWM_FREQUENCY) - 1;


	//configuring all pins
	PWMGenConfigure(PWM1_BASE, PWM_GEN_0, PWM_GEN_MODE_DOWN);
	PWMGenPeriodSet(PWM1_BASE, PWM_GEN_0, ui32Load);

	//setting pulse width
	PWMPulseWidthSet(PWM1_BASE, PWM_OUT_0, ui8Adjust * ui32Load / 1000);
	PWMOutputState(PWM1_BASE, PWM_OUT_0_BIT, true);

	//enabling
	PWMGenEnable(PWM1_BASE, PWM_GEN_0);

	while(1)
	{
		if(GPIOPinRead(GPIO_PORTF_BASE,GPIO_PIN_4)==0 && detectKeyPress1()==1 )
			{
				debug=2;

				ui8Adjust=ui8Adjust-20;
						if (ui8Adjust < 56)
						{
							ui8Adjust = 56;
						}
					
				//system remains in "release" state if the button is pressed
				while(GPIOPinRead(GPIO_PORTF_BASE,GPIO_PIN_4)==0)
				{

				debug=10;
				PWMPulseWidthSet(PWM1_BASE, PWM_OUT_0, ui8Adjust * ui32Load / 1000);
				}
				
			}


			//system goes to "release" state
			//checking whether sw2 is pressed
			if(GPIOPinRead(GPIO_PORTF_BASE,GPIO_PIN_0)==0 && detectKeyPress2()==1 )
			{
				debug=3;
				ui8Adjust=ui8Adjust+20;
				
				if (ui8Adjust > 111)
						{
							debug=4;
							ui8Adjust = 111;
						}

				//system remains in "release" state if the button is pressed
				while(GPIOPinRead(GPIO_PORTF_BASE,GPIO_PIN_0)==0)
				{
					debug=5;
				PWMPulseWidthSet(PWM1_BASE, PWM_OUT_0, ui8Adjust * ui32Load / 1000);
				}
		//SysCtlDelay(100000);
	}
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
	debug=1;
	// Clear the timer interrupt
	TimerIntClear(TIMER0_BASE, TIMER_TIMA_TIMEOUT);

	//PWMPulseWidthSet(PWM1_BASE, PWM_OUT_0, 40 * ui32Load / 1000);
	//system goes to "release" state
	//checking weather sw1 is pressed
	if(GPIOPinRead(GPIO_PORTF_BASE,GPIO_PIN_4)==0 && detectKeyPress1()==1 )
	{
		debug=2;

		ui8Adjust=ui8Adjust-10;
				if (ui8Adjust < 56)
				{
					ui8Adjust = 56;
				}
		
		//system remains in "release" state if the button is pressed
		while(GPIOPinRead(GPIO_PORTF_BASE,GPIO_PIN_4)==0)
		{

		debug=10;
		//PWMPulseWidthSet(PWM1_BASE, PWM_OUT_0, 50 * ui32Load / 1000);
		}

	}


	//system goes to "release" state
	//checking weather sw2 is pressed
	if(GPIOPinRead(GPIO_PORTF_BASE,GPIO_PIN_0)==0 && detectKeyPress2()==1 )
	{
		debug=3;
		ui8Adjust=ui8Adjust+10;
		//PWMPulseWidthSet(PWM1_BASE, PWM_OUT_0, 90 * ui32Load / 1000);
		if (ui8Adjust > 111)
				{
					debug=4;
					ui8Adjust = 111;
				}

		//system remains in "release" state if the button is pressed
		while(GPIOPinRead(GPIO_PORTF_BASE,GPIO_PIN_0)==0)
		{
			debug=5;
		}
		
	}

	//system goes to "idle" state whe the button is released


}
