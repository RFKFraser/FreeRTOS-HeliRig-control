/**********************************************************
 *
 * pwmGen.c - Initilises and configures peripheral PWM signals
 *            Contains functions to set Main rotor and tail rotor PWM.
 *
 * Author: Group 1
 *
 **********************************************************/

#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_memmap.h"
#include "inc/hw_GPIO.h"
#include "inc/hw_types.h"
#include "pwmGen.h"
#include "driverlib/pin_map.h" //Needed for pin configure
#include "driverlib/debug.h"
#include "driverlib/gpio.h"
#include "driverlib/pwm.h"
#include "driverlib/systick.h"
#include "driverlib/sysctl.h"
#include "driverlib/interrupt.h"

/*******************************************
 *      Local prototypes
 *******************************************/

void PWMinit (void);
void setMainPWM (uint32_t u32Duty);
void setTailPWM (uint32_t u32Duty);


/*********************************************************
 * initialisePWM
 * M0PWM7 (J4-05, PC5) is used for the main rotor motor
 *********************************************************/
void
PWMinit (void)
{
    // Initialise Main Rotor PWM
    SysCtlPeripheralEnable(PWM_MAIN_PERIPH_PWM);
    SysCtlPeripheralEnable(PWM_MAIN_PERIPH_GPIO);
    GPIOPinConfigure(PWM_MAIN_GPIO_CONFIG);
    GPIOPinTypePWM(PWM_MAIN_GPIO_BASE, PWM_MAIN_GPIO_PIN);
    PWMGenConfigure(PWM_MAIN_BASE, PWM_MAIN_GEN,
                    PWM_GEN_MODE_UP_DOWN | PWM_GEN_MODE_NO_SYNC);

    // Set the initial PWM parameters
    setMainPWM (PWM_START_DC_MAIN);
    PWMGenEnable(PWM_MAIN_BASE, PWM_MAIN_GEN);

    // Initialise Tail Rotor PWM
    SysCtlPeripheralEnable(PWM_TAIL_PERIPH_PWM);
    SysCtlPeripheralEnable(PWM_TAIL_PERIPH_GPIO);
    GPIOPinConfigure(PWM_TAIL_GPIO_CONFIG);
    GPIOPinTypePWM(PWM_TAIL_GPIO_BASE, PWM_TAIL_GPIO_PIN);
    PWMGenConfigure(PWM_TAIL_BASE, PWM_TAIL_GEN,
                    PWM_GEN_MODE_UP_DOWN | PWM_GEN_MODE_NO_SYNC);

    // Set the initial PWM parameters
    setTailPWM (PWM_START_DC_TAIL);
    PWMGenEnable(PWM_TAIL_BASE, PWM_TAIL_GEN);

    // Disable the output.  Repeat this call with 'true' to turn O/P on.
    PWMOutputState(PWM_MAIN_BASE, PWM_MAIN_OUTBIT, false);
    PWMOutputState(PWM_TAIL_BASE, PWM_TAIL_OUTBIT, false);

    // Initialisation is complete, so turn on the output.
    PWMOutputState(PWM_MAIN_BASE, PWM_MAIN_OUTBIT, true);
    PWMOutputState(PWM_TAIL_BASE, PWM_TAIL_OUTBIT, true);
}



/********************************************************
 * Function to set the main rotor duty cycle on M0PWM7
 ********************************************************/
void setMainPWM (uint32_t duty_cycle)
{
    PWMGenPeriodSet(PWM_MAIN_BASE, PWM_MAIN_GEN, PWM_PERIOD_MAIN);
    PWMPulseWidthSet(PWM_MAIN_BASE, PWM_MAIN_OUTNUM,
                     PWM_PERIOD_MAIN * duty_cycle / 100);
}



/********************************************************
 * Function to set the tail rotor duty cycle on M1PWM5
 ********************************************************/
void setTailPWM (uint32_t duty_cycle)
{
    PWMGenPeriodSet(PWM_TAIL_BASE, PWM_TAIL_GEN, PWM_PERIOD_TAIL);
    PWMPulseWidthSet(PWM_TAIL_BASE, PWM_TAIL_OUTNUM,
                     PWM_PERIOD_TAIL * duty_cycle / 100);
}



