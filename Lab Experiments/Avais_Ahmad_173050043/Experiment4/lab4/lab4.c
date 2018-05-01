
 
/*

* Team Id: 

* Author List: Pankaj Kumar (173050052) , Avais Ahmad (173050043)

* Filename: main.c

* Theme: showing the values of x,y used by UART

* Functions:  main() 

* Global Variables: horizontal, vertical, buffer , h_avg, v_avg



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
#include "driverlib/adc.h"
#include "driverlib/uart.h"


uint32_t horizontal[4];
uint32_t vertical[4];
int buffer[4];
volatile uint32_t h_avg;
volatile uint32_t v_avg;



/*

* Function Name: main

* Input: None

* Output: int to inform the caller that the program exited correctly or incorrectly (C code standard)

* Logic: calculating the average  and digital equivalent of values of x,y

* Example Call: Called automatically by the Operating System

*/



 int main(void)
 {



	 SysCtlClockSet(SYSCTL_SYSDIV_5 | SYSCTL_USE_PLL | SYSCTL_OSC_MAIN | SYSCTL_XTAL_16MHZ);

	 //Enabling the peripherals
	 SysCtlPeripheralEnable(SYSCTL_PERIPH_UART0);
     SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);
     SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
     SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOC);	//buzzer on C port
     SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOD);
     SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOE);
     SysCtlPeripheralEnable(SYSCTL_PERIPH_ADC0);
     SysCtlPeripheralEnable(SYSCTL_PERIPH_ADC1);



    //Disabling the buzzer
     GPIOPinTypeGPIOOutput(GPIO_PORTC_BASE, GPIO_PIN_4);
     GPIOPinWrite(GPIO_PORTC_BASE, GPIO_PIN_4, 0x10);

     //configuring ADC sequencer
     ADCSequenceConfigure(ADC0_BASE, 1, ADC_TRIGGER_PROCESSOR, 0);

	 ADCSequenceStepConfigure(ADC0_BASE, 1, 0, ADC_CTL_CH6);
	 ADCSequenceStepConfigure(ADC0_BASE, 1, 1, ADC_CTL_CH6);
	 ADCSequenceStepConfigure(ADC0_BASE, 1, 2, ADC_CTL_CH6);

	 ADCSequenceStepConfigure(ADC0_BASE,1,3,ADC_CTL_CH6|ADC_CTL_IE|ADC_CTL_END);

	 ADCSequenceEnable(ADC0_BASE, 1);

	 //CONFIGURING 2ND ADC sequencer
	 ADCSequenceConfigure(ADC1_BASE, 1, ADC_TRIGGER_PROCESSOR, 0);

	 ADCSequenceStepConfigure(ADC1_BASE, 1, 0, ADC_CTL_CH7);
	 ADCSequenceStepConfigure(ADC1_BASE, 1, 1, ADC_CTL_CH7);
	 ADCSequenceStepConfigure(ADC1_BASE, 1, 2, ADC_CTL_CH7);

	 ADCSequenceStepConfigure(ADC1_BASE,1,3,ADC_CTL_CH7|ADC_CTL_IE|ADC_CTL_END);

	 ADCSequenceEnable(ADC1_BASE, 1);


	 //configuring the uart
     GPIOPinConfigure(GPIO_PA0_U0RX);
     GPIOPinConfigure(GPIO_PA1_U0TX);
     GPIOPinTypeUART(GPIO_PORTA_BASE, GPIO_PIN_0 | GPIO_PIN_1);


     UARTConfigSetExpClk(UART0_BASE, SysCtlClockGet(), 115200,
    		 (UART_CONFIG_WLEN_8 | UART_CONFIG_STOP_ONE | UART_CONFIG_PAR_NONE));



     //UARTCharPut(UART0_BASE, ' ');
     while (1)
     {
    	 int temp,temp2,d,i,j,flag;

    	 ADCIntClear(ADC0_BASE, 1);  //good practice to clear the flag
    	 ADCIntClear(ADC1_BASE, 1);
    	 ADCProcessorTrigger(ADC0_BASE, 1); //adc conversion
    	 ADCProcessorTrigger(ADC1_BASE, 1);
    	 //this loop will exit when conversion is complete
    	 while(!ADCIntStatus(ADC0_BASE, 1, false));
    	 while(!ADCIntStatus(ADC1_BASE, 1, false));

    	 ADCSequenceDataGet(ADC0_BASE, 1, horizontal);	//getting value into a buffer
    	 ADCSequenceDataGet(ADC1_BASE, 1, vertical);

    	 h_avg = (horizontal[0] + horizontal[1] + horizontal[2] + horizontal[3] + 2)/4; //calculating the average
    	 v_avg = (vertical[0] + vertical[1] + vertical[2] + vertical[3] + 2)/4;


    	 //scaling
    	 h_avg=h_avg/100;
    	 v_avg=v_avg/100;

    	 temp=h_avg;

    	 UARTCharPut(UART0_BASE, 'X');
    	 UARTCharPut(UART0_BASE, ':');


    	 i=0;

    	 //making sure that null is not returend on 0
    	 if(h_avg==0)
    	 {
    		 UARTCharPut(UART0_BASE, '0');
    	 }

    	 while(temp>0)
    	 {
    		 d=(temp%10)+'0';
    		 buffer[i]=d;
    		 i++;
    		 //UARTCharPut(UART0_BASE, d);
    		 temp=temp/10;
    	 }

    	 for(j=(i-1);j>=0;j--)
    	 {
    		 UARTCharPut(UART0_BASE, buffer[j]);
    	 }

    	 //13 is ascii for carriage return
    	 UARTCharPut(UART0_BASE, 13);
    	 //UARTCharPut(UART0_BASE, ' ');


    	 UARTCharPut(UART0_BASE, 'Y');
    	 UARTCharPut(UART0_BASE, ':');
    	 //UARTCharPut(UART0_BASE, ',');
    	 temp=v_avg;


    	 i=0;



    	 if(v_avg==0)
		 {
			 UARTCharPut(UART0_BASE, '0');
		 }

		 while(temp>0)
		 {
			 d=(temp%10)+'0';
			 buffer[i]=d;
			 i++;
			 //UARTCharPut(UART0_BASE, d);
			 temp=temp/10;
		 }

		 for(j=(i-1);j>=0;j--)
		 {
			 UARTCharPut(UART0_BASE, buffer[j]);
		 }

		 //13 is ascii for carriage return
		 UARTCharPut(UART0_BASE, 13);
		 //UARTCharPut(UART0_BASE, ' ');

		 SysCtlDelay(670000*8);

     }
 }


