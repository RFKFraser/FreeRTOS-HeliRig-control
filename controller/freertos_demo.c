//*****************************************************************************
/*
 * Freertos_demo.c
 * This is the file containing the FreeRtos where the main is.
 *
 * Author: Group 1
 */

//*****************************************************************************

#include <stdbool.h>
#include <stdint.h>
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "inc/hw_ints.h"
#include "driverlib/adc.h"
#include "driverlib/pwm.h"
#include "driverlib/gpio.h"
#include "driverlib/uart.h"
#include "driverlib/sysctl.h"
#include "driverlib/systick.h"
#include "driverlib/interrupt.h"
#include "driverlib/debug.h"
#include "pin_map.h"
#include "driverlib/rom.h"
#include "utils/uartstdio.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"
#include "pwmGen.h"
#include "ADC_Task.h"
#include "buttons4.h"
#include "Yaw_Task.h"
#include "Control_Task.h"
#include "display.h"
#include "State_Task.h"

//*****************************************************************************
// Constants
//*****************************************************************************
#define VOLTAGE_RANGE 800                       // Voltage range in mV
#define DISTANCE_SENSOR_STEP 4096/3300          // Bits per mV (Bit range / voltage range)
#define ENCODER_POLES 112                       // Number of encoder poles
#define SYSTICKTASKSTACKSIZE        128         // Stack size in words

//*****************************************************************************
// Global variables
//*****************************************************************************

//*****************************************************************************
//

//!
//! This application utilizes FreeRTOS to perform the tasks in a concurrent
//! fashion.
//! - The task files used in the project are listed as follow
//! 1. ADC_Task     -- Task that reads the ADC for the Altitude of the heli
//! 2. Control_Task -- Task that handles the controls of heli (pwm set etc)
//! 3. State_Task   -- Task that handles the state withcing of the state machine
//! 4. Yaw_Task -- Task that reads and calculate the Yaw from 2 phase encoders
//!
//! In addition to the tasks, this application also uses the following FreeRTOS
//! resources:
//! - 2 Queues are made to transfer information between tasks. In this case, 
//!   the queues contain the alititude and yaw which are sent from their
//!   respective tasks. The Queues are then received in the Control_Task task.
//!   Which are then used for altitude and yaw control.
//!
//! - A Semaphore (Mutex) to guard the resource, UART, from access by multiple 
//!   tasks at the same time.
//
//*****************************************************************************


//*****************************************************************************
//
// The mutex that protects concurrent access of UART from multiple tasks.
//
//*****************************************************************************
xSemaphoreHandle g_pUARTSemaphore;

//*****************************************************************************
//
// The error routine that is called if the driver library encounters an error.
//
//*****************************************************************************
#ifdef DEBUG
void
__error__(char *pcFilename, uint32_t ui32Line)
{
}
#endif

//*****************************************************************************
//
// This hook is called by FreeRTOS when an stack overflow error is detected.
//
//*****************************************************************************
void
vApplicationStackOverflowHook(xTaskHandle *pxTask, char *pcTaskName)
{
    // This function can not return, so loop forever.  Interrupts are disabled
    // on entry to this function, so no processor interrupts will interrupt
    // this loop.
    while(1)
    {
    }
}

//*****************************************************************************
//
// Configure the UART and its pins.  This must be called before UARTprintf().
//
//*****************************************************************************
void
ConfigureUART(void)
{
    // Enable the GPIO Peripheral used by the UART.
    ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);
    
    // Enable UART0
    ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_UART0);
    
    // Configure GPIO Pins for UART mode.
    ROM_GPIOPinConfigure(GPIO_PA0_U0RX);
    ROM_GPIOPinConfigure(GPIO_PA1_U0TX);
    ROM_GPIOPinTypeUART(GPIO_PORTA_BASE, GPIO_PIN_0 | GPIO_PIN_1);
    
    // Use the internal 16MHz oscillator as the UART clock source.
    UARTClockSourceSet(UART0_BASE, UART_CLOCK_PIOSC);
    
    // Initialize the UART for console I/O.
    UARTStdioConfig(0, 115200, 16000000);
}

//*****************************************************************************
// Initialisation functions for the clock (incl. SysTick)
//
//*****************************************************************************
void initClock (void)
{
    // Set the clocking to run at 50 MHz fr
    om the PLL.
    ROM_SysCtlClockSet(SYSCTL_SYSDIV_4 | SYSCTL_USE_PLL | SYSCTL_XTAL_16MHZ |
                      SYSCTL_OSC_MAIN);
}

//*****************************************************************************
//
// Initialize FreeRTOS and task sets,
// starts the initial set of tasks.
//
//*****************************************************************************
int
main(void)
{
    //Initilisations of task sets
    initClock ();
    PWMinit();
    initButtons();
    YawInit();
    ADCTaskInit();
    ReceiveTaskInit();
    ConfigureUART();
    initDisplay();
    StateInit();
    IntMasterEnable();

    // Create a mutex to guard the UART.
    g_pUARTSemaphore = xSemaphoreCreateMutex();

    // Start the scheduler.  This should not return.
    vTaskStartScheduler();

    // In case the scheduler returns for some reason, loop forever.
    while(1)
    {
    }
}
