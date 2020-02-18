/**********************************************************
 *
 * pwmGen.h - Initilises and configures peripheral PWM signals
 *            Contains functions to set Main rotor and tail rotor PWM.
 *
 * Author: Group 1
 **********************************************************/

#ifndef PWMGEN_H_
#define PWMGEN_H_

#include <stdint.h>
#include <stdbool.h>

/**********************************************************
 * Constants
 **********************************************************/

// Main Rotor PWM configuration
#define PWM_START_RATE_HZ_MAIN  850
#define PWM_START_DC_MAIN    5
#define PWM_PERIOD_MAIN SysCtlClockGet() / PWM_START_RATE_HZ_MAIN
#define PWM_MIN_DC    5
#define PWM_MAX_DC    95
#define PWM_DUTY_STEP     1
#define PWM_DIVIDER_CODE   SYSCTL_PWMDIV_4

// Tail Rotor PWM configuration
#define PWM_START_RATE_HZ_TAIL  830
#define PWM_START_DC_TAIL    5
#define PWM_PERIOD_TAIL SysCtlClockGet() / PWM_START_RATE_HZ_TAIL

//  PWM Hardware Details M0PWM7 (gen 3)
//  ---Main Rotor PWM: PC5, J4-05
#define PWM_MAIN_BASE        PWM0_BASE
#define PWM_MAIN_GEN         PWM_GEN_3
#define PWM_MAIN_OUTNUM      PWM_OUT_7
#define PWM_MAIN_OUTBIT      PWM_OUT_7_BIT
#define PWM_MAIN_PERIPH_PWM  SYSCTL_PERIPH_PWM0
#define PWM_MAIN_PERIPH_GPIO SYSCTL_PERIPH_GPIOC
#define PWM_MAIN_GPIO_BASE   GPIO_PORTC_BASE
#define PWM_MAIN_GPIO_CONFIG GPIO_PC5_M0PWM7
#define PWM_MAIN_GPIO_PIN    GPIO_PIN_5

//  PWM Hardware Details M1PWM5 (gen 2)
//  ---Tail Rotor PWM: PF1, J3-10
#define PWM_TAIL_BASE        PWM1_BASE
#define PWM_TAIL_GEN         PWM_GEN_2
#define PWM_TAIL_OUTNUM      PWM_OUT_5
#define PWM_TAIL_OUTBIT      PWM_OUT_5_BIT
#define PWM_TAIL_PERIPH_PWM  SYSCTL_PERIPH_PWM1
#define PWM_TAIL_PERIPH_GPIO SYSCTL_PERIPH_GPIOF
#define PWM_TAIL_GPIO_BASE   GPIO_PORTF_BASE
#define PWM_TAIL_GPIO_CONFIG GPIO_PF1_M1PWM5
#define PWM_TAIL_GPIO_PIN    GPIO_PIN_1

/*********************************************************
 * initialisePWM
 * M0PWM7 (J4-05, PC5) is used for the main rotor motor
 *********************************************************/
void PWMinit (void);

/********************************************************
 * Function to set the main rotor duty cycle on M0PWM7
 ********************************************************/
void setMainPWM (uint32_t u32Duty);

/********************************************************
 * Function to set the tail rotor duty cycle on M1PWM5
 ********************************************************/
void setTailPWM (uint32_t u32Duty);

#endif /* PWMGEN_H_ */
