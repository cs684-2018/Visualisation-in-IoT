

/*
* Team Id: 

* Author List: Pankaj Kumar (173050052), Avais Ahmad (173050043)

* Filename: main.c

* Theme: RGB controller using SW1 and SW2 having 2 modes manual and auto mode.

* Functions: mode1(), mode2(), mode3(), interruptManager() , Timer0IntHandler(), 
             timer_config() , main() , detectKeyPress1() , detectKeyPress2()

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
int delay_count=1;
int delay;
uint8_t sw1 = 1;	// for monitoring switch 1
uint8_t sw2 = 1;	// for monitoring switch 2
int press_count=0; //how many times is
int modeCount=0;
volatile uint8_t ui8Adjust=254;
volatile uint32_t ui32Load;
volatile uint32_t ui32PWMClock;

//defining functions
void mode1();
void mode2();
void mode3();


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

* Logic: detecting the key pressing and calling corrosponding mode function

* Example Call: Called automatically by the Operating System

*/


int main(void)
{
	timer_config();
	interruptManager();
	//configuring interrupt timer
	uint32_t ui32Period;

	ui32Period = (SysCtlClockGet() / 10) / 2;
	TimerLoadSet(TIMER0_BASE, TIMER_A, ui32Period -1);


	//ther variable



	delay=(100000*delay_count)/2;

	//setting clock
	SysCtlClockSet(SYSCTL_SYSDIV_5|SYSCTL_USE_PLL|SYSCTL_OSC_MAIN|SYSCTL_XTAL_16MHZ);
	SysCtlPWMClockSet(SYSCTL_PWMDIV_64);


	SysCtlPeripheralEnable(SYSCTL_PERIPH_PWM1); 	//module 1 covers LED pins
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);

	//configuring all 3 LED pins
	GPIOPinTypePWM(GPIO_PORTF_BASE, GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3);
	GPIOPinConfigure(GPIO_PF1_M1PWM5);
	GPIOPinConfigure(GPIO_PF2_M1PWM6);
	GPIOPinConfigure(GPIO_PF3_M1PWM7);


	//unlocking
	HWREG(GPIO_PORTF_BASE + GPIO_O_LOCK) = GPIO_LOCK_KEY;
	HWREG(GPIO_PORTF_BASE + GPIO_O_CR) |= 0x01;
	HWREG(GPIO_PORTF_BASE + GPIO_O_LOCK) = 0;
	GPIODirModeSet(GPIO_PORTF_BASE, GPIO_PIN_4|GPIO_PIN_0, GPIO_DIR_MODE_IN);
	GPIOPadConfigSet(GPIO_PORTF_BASE, GPIO_PIN_4|GPIO_PIN_0, GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD_WPU);

	ui32PWMClock = SysCtlClockGet() / 64;
	ui32Load = (ui32PWMClock / PWM_FREQUENCY) - 1;
	PWMGenConfigure(PWM1_BASE, PWM_GEN_2, PWM_GEN_MODE_DOWN);
	PWMGenConfigure(PWM1_BASE, PWM_GEN_3, PWM_GEN_MODE_DOWN);

	//seting periods
	PWMGenPeriodSet(PWM1_BASE, PWM_GEN_2, ui32Load);
	PWMGenPeriodSet(PWM1_BASE, PWM_GEN_3, ui32Load);

	//setting PWM duty
	PWMPulseWidthSet(PWM1_BASE, PWM_OUT_5, ui8Adjust * ui32Load / 1000);		//red
	PWMPulseWidthSet(PWM1_BASE, PWM_OUT_6, 1);		//green
	PWMPulseWidthSet(PWM1_BASE, PWM_OUT_7, 1);		//blue

	//to turn on output pins
	PWMOutputState(PWM1_BASE, PWM_OUT_5_BIT|PWM_OUT_6_BIT|PWM_OUT_7_BIT, true);

	//Enabling pwn generator
	PWMGenEnable(PWM1_BASE, PWM_GEN_2);
	PWMGenEnable(PWM1_BASE, PWM_GEN_3);

	int i;  //used in for loop
	while(1)
	{
		/*//resetting
		PWMPulseWidthSet(PWM1_BASE, PWM_OUT_5, ui8Adjust * ui32Load / 1000);		//red
		PWMPulseWidthSet(PWM1_BASE, PWM_OUT_6, 1);		//green
		PWMPulseWidthSet(PWM1_BASE, PWM_OUT_7, 1);		//blue
		SysCtlDelay(delay);*/

		for(i=1;i<255;i++)
		{
		//red
		PWMPulseWidthSet(PWM1_BASE, PWM_OUT_5, (257-i)*10);
		//green
		PWMPulseWidthSet(PWM1_BASE, PWM_OUT_7, i*10 );
		SysCtlDelay(delay);
		}

		/*//resetting
		PWMPulseWidthSet(PWM1_BASE, PWM_OUT_5, 1);		//red
		PWMPulseWidthSet(PWM1_BASE, PWM_OUT_6, 1);		//blue
		PWMPulseWidthSet(PWM1_BASE, PWM_OUT_7, ui8Adjust * ui32Load / 1000);		//green
		SysCtlDelay(delay);*/

		for(i=1;i<255;i++)
		{
		//green
		PWMPulseWidthSet(PWM1_BASE, PWM_OUT_7, (257-i) *10);
		//blue
		PWMPulseWidthSet(PWM1_BASE, PWM_OUT_6, i*10);
		SysCtlDelay(delay);
		}

		PWMPulseWidthSet(PWM1_BASE, PWM_OUT_7, (257-i)*10);
		SysCtlDelay(delay);


		for(i=1;i<255;i++)
		{
		//blue
		PWMPulseWidthSet(PWM1_BASE, PWM_OUT_6, (257-i)*10);
		//red
		PWMPulseWidthSet(PWM1_BASE, PWM_OUT_5, i*10);
		SysCtlDelay(delay);
		}

	}

}	//endof main


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


		//checking weather sw1 is pressed
		if(GPIOPinRead(GPIO_PORTF_BASE,GPIO_PIN_4)==0 && detectKeyPress1()==1 )
		{
			delay_count++;
			if(delay_count>20)
				delay_count=20;

			//system remains in "release" state if the button is pressed
			while(GPIOPinRead(GPIO_PORTF_BASE,GPIO_PIN_4)==0)
			{
				if(GPIOPinRead(GPIO_PORTF_BASE,GPIO_PIN_0)==0 && detectKeyPress2()==1)
				{
					mode1();
				}
			}
		}

		//checking weather sw2 is pressed
		if(GPIOPinRead(GPIO_PORTF_BASE,GPIO_PIN_0)==0 && detectKeyPress2()==1)
		{
			delay_count--;
			if(delay_count<1)
				delay_count=1;

			while(GPIOPinRead(GPIO_PORTF_BASE,GPIO_PIN_0)==0);
		}
		delay=(100000*delay_count)/2;
		SysCtlDelay(100000*2);


	}

/*

* Function Name: mode1

* Input: None

* Output: none

* Logic: In this mode, intensity of Red LED can be controlled using SW1 and SW2.

* Example Call: mode1();

*/


void mode1()
{
	modeCount=1;
	ui32PWMClock = SysCtlClockGet() / 64;
	ui32Load = (ui32PWMClock / PWM_FREQUENCY) - 1;
	int sw2timer=0; //to calculate time for whoch it is pressed for

	while(GPIOPinRead(GPIO_PORTF_BASE,GPIO_PIN_0)==0 && GPIOPinRead(GPIO_PORTF_BASE,GPIO_PIN_4)==0)		//both switch is pressed continuously
	{
		if(sw2timer>=16)		//checking for 4 seconds time
		{
			mode3();
		}
		sw2timer++;
		SysCtlDelay(6700000/2);
	}

	while(GPIOPinRead(GPIO_PORTF_BASE,GPIO_PIN_4)==0)
	{
		if(GPIOPinRead(GPIO_PORTF_BASE,GPIO_PIN_0)==0 && detectKeyPress2()==1)
		{
			mode2();
		}
	}

	//resetting
	PWMPulseWidthSet(PWM1_BASE, PWM_OUT_5, ui8Adjust * ui32Load / 1000);		//red
	PWMPulseWidthSet(PWM1_BASE, PWM_OUT_6, 1);		//blue
	PWMPulseWidthSet(PWM1_BASE, PWM_OUT_7, 1);		//green

	while(1)
	{
		if(GPIOPinRead(GPIO_PORTF_BASE,GPIO_PIN_4)==0x00)
				{
					ui8Adjust--;
					if (ui8Adjust < 10)
					{
						ui8Adjust = 10;
					}
					PWMPulseWidthSet(PWM1_BASE, PWM_OUT_5, ui8Adjust * ui32Load / 1000);
				}

				if(GPIOPinRead(GPIO_PORTF_BASE,GPIO_PIN_0)==0x00)
				{
					ui8Adjust++;
					if (ui8Adjust > 254)
					{
						ui8Adjust = 254;
					}
					PWMPulseWidthSet(PWM1_BASE, PWM_OUT_5, ui8Adjust * ui32Load / 1000);
				}

				SysCtlDelay(100000);
			}
	}


/*

* Function Name: mode2

* Input: None

* Output: none

* Logic: In this mode, intensity of Blue LED can be controlled using SW1 and SW2.

* Example Call: mode2();

*/	

void mode2()
{
	while(GPIOPinRead(GPIO_PORTF_BASE,GPIO_PIN_0)==0);
	modeCount=2;
	ui32PWMClock = SysCtlClockGet() / 64;
	ui32Load = (ui32PWMClock / PWM_FREQUENCY) - 1;

	//resetting
	PWMPulseWidthSet(PWM1_BASE, PWM_OUT_5, 1);		//red
	PWMPulseWidthSet(PWM1_BASE, PWM_OUT_6, ui8Adjust * ui32Load / 1000);		//blue
	PWMPulseWidthSet(PWM1_BASE, PWM_OUT_7, 1);		//green
	while(1)
	{
		if(GPIOPinRead(GPIO_PORTF_BASE,GPIO_PIN_4)==0x00)
				{
					ui8Adjust--;
					if (ui8Adjust < 10)
					{
						ui8Adjust = 10;
					}
					PWMPulseWidthSet(PWM1_BASE, PWM_OUT_6, ui8Adjust * ui32Load / 1000);
				}

				if(GPIOPinRead(GPIO_PORTF_BASE,GPIO_PIN_0)==0x00)
				{
					ui8Adjust++;
					if (ui8Adjust > 254)
					{
						ui8Adjust = 254;
					}
					PWMPulseWidthSet(PWM1_BASE, PWM_OUT_6, ui8Adjust * ui32Load / 1000);
				}

				SysCtlDelay(100000);
			}
	}


/*

* Function Name: mode3

* Input: None

* Output: none

* Logic: In this mode, intensity of Green LED can be controlled using SW1 and SW2.

* Example Call: mode3();

*/

void mode3()
{
	modeCount=3;
	ui32PWMClock = SysCtlClockGet() / 64;
	ui32Load = (ui32PWMClock / PWM_FREQUENCY) - 1;

	//resetting
	PWMPulseWidthSet(PWM1_BASE, PWM_OUT_5, 1);		//red
	PWMPulseWidthSet(PWM1_BASE, PWM_OUT_6, 1);		//blue
	PWMPulseWidthSet(PWM1_BASE, PWM_OUT_7, ui8Adjust * ui32Load / 1000);		//green
	while(1)
	{
		if(GPIOPinRead(GPIO_PORTF_BASE,GPIO_PIN_4)==0x00)
				{
					ui8Adjust--;
					if (ui8Adjust < 10)
					{
						ui8Adjust = 10;
					}
					PWMPulseWidthSet(PWM1_BASE, PWM_OUT_7, ui8Adjust * ui32Load / 1000);
				}

				if(GPIOPinRead(GPIO_PORTF_BASE,GPIO_PIN_0)==0x00)
				{
					ui8Adjust++;
					if (ui8Adjust > 254)
					{
						ui8Adjust = 254;
					}
					PWMPulseWidthSet(PWM1_BASE, PWM_OUT_7, ui8Adjust * ui32Load / 1000);
				}

				SysCtlDelay(100000);
			}
	}
