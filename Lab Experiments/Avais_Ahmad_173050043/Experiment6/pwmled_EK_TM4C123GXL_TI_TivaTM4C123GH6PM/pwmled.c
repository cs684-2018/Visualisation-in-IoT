 
/*

* Team Id: 

* Author List: Pankaj Kumar (173050052) , Avais Ahmad (173050043)

* Filename: pwmled.c

* Theme: interfacing with TI-RTOS 

* Functions: pwmLEDFxn() , main()

* Global Variables: none

  
 */

/* XDCtools Header files */
#include <xdc/std.h>
#include <xdc/cfg/global.h>
#include <xdc/runtime/System.h>

/* BIOS Header files */
#include <ti/sysbios/BIOS.h>

/* TI-RTOS Header files */
#include <ti/drivers/GPIO.h>
#include <ti/drivers/PWM.h>

/* Example/Board Header files */
#include "Board.h"


 /*

* Function Name: pwmLEDFxn()

* Input: two argument input

* Output: none

* Description: Task periodically increments the PWM duty for the on board LED.

* Example Call: pwmLEDFxn();

*/


Void pwmLEDFxn(UArg arg0, UArg arg1)
{
    PWM_Handle pwm1;
    PWM_Handle pwm2 = NULL;
    PWM_Params params;
    uint16_t   pwmPeriod = 3000;      // Period and duty in microseconds
    uint16_t   duty = 0;
    uint16_t   dutyInc = 100;

    PWM_Params_init(&params);
    params.period = pwmPeriod;
    pwm1 = PWM_open(Board_PWM0, &params);
    if (pwm1 == NULL) {
        System_abort("Board_PWM0 did not open");
    }

    if (Board_PWM1 != Board_PWM0) {
        params.polarity = PWM_POL_ACTIVE_LOW;
        pwm2 = PWM_open(Board_PWM1, &params);
        if (pwm2 == NULL) {
            System_abort("Board_PWM1 did not open");
        }
    }

    /* Loop forever incrementing the PWM duty */
    while (1) {
        PWM_setDuty(pwm1, duty);

        if (pwm2) {
            PWM_setDuty(pwm2, duty);
        }

        duty = (duty + dutyInc);
        if (duty == pwmPeriod || (!duty)) {
            dutyInc = - dutyInc;
        }

        Task_sleep((UInt) arg0);
    }
}




/*

* Function Name: main

* Input: None

* Output: int to inform the caller that the program exited correctly or incorrectly (C code standard)

* Logic: interfacing with TI real time operating system by turning on LED 

* Example Call: Called automatically by the Operating System

*/


int main(void)
{
    /* Call board init functions. */
    Board_initGeneral();
    Board_initGPIO();
    Board_initPWM();

    /* Turn on user LED */
    GPIO_write(Board_LED0, Board_LED_ON);

    System_printf("Starting the example\nSystem provider is set to SysMin. "
                  "Halt the target to view any SysMin contents in ROV.\n");

    /* SysMin will only print to the console when you call flush or exit */
    System_flush();

    /* Start BIOS */
    BIOS_start();

    return (0);
}
