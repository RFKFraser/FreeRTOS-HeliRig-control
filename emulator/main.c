//*****************************************************************************
//     _______   ________________ __  _____ __ __
//    / ____/ | / / ____/ ____/ // / / ___// // /
//   / __/ /  |/ / /   / __/ / // /_/ __ \/ // /_
//  / /___/ /|  / /___/ /___/__  __/ /_/ /__  __/
// /_____/_/ |_/\____/_____/  /_/  \____/  /_/
//
// ENCE464 Embedded Software Project
// Helicopter Rig Emulator
//
// Module: main.c
// Author: Matthew Northcott / mno32@uclive.ac.nz
// 
//*****************************************************************************

#include <stdbool.h>
#include <stdint.h>
#include "tasks/adc_task.h"
#include "tasks/emulate_task.h"
#include "tasks/out_task.h"
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "driverlib/gpio.h"
#include "driverlib/pin_map.h"
#include "driverlib/rom.h"
#include "driverlib/sysctl.h"
#include "driverlib/uart.h"
#include "driverlib/adc.h"
#include "driverlib/ssi.h"
#include "utils/uartstdio.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"

// The mutex that protects concurrent access of UART from multiple tasks.
xSemaphoreHandle g_pUARTSemaphore;

// The error routine that is called if the driver library encounters an error.
#ifdef DEBUG
void
__error__ (char *pcFilename, uint32_t ui32Line)
{
}

#endif

// This hook is called by FreeRTOS when an stack overflow error is detected.
void
vApplicationStackOverflowHook (xTaskHandle *pxTask, char *pcTaskName)
{
    // This function can not return, so loop forever.  Interrupts are disabled
    // on entry to this function, so no processor interrupts will interrupt
    // this loop.
    while (1)
    {
    }
}


// Configure the UART and its pins.  This must be called before UARTprintf().
void
ConfigureUART (void)
{
    // Enable the GPIO Peripheral used by the UART.
    ROM_SysCtlPeripheralEnable (SYSCTL_PERIPH_GPIOA);

    // Enable UART0
    ROM_SysCtlPeripheralEnable (SYSCTL_PERIPH_UART0);

    // Configure GPIO Pins for UART mode.
    ROM_GPIOPinConfigure (GPIO_PA0_U0RX);
    ROM_GPIOPinConfigure (GPIO_PA1_U0TX);
    ROM_GPIOPinTypeUART (GPIO_PORTA_BASE, GPIO_PIN_0 | GPIO_PIN_1);

    // Use the internal 16MHz oscillator as the UART clock source.
    UARTClockSourceSet (UART0_BASE, UART_CLOCK_PIOSC);

    // Initialize the UART for console I/O.
    UARTStdioConfig (0, 115200, 16000000);
}


void
ConfigureADC0 (void)
{
    ROM_SysCtlPeripheralEnable (SYSCTL_PERIPH_GPIOE);
    ROM_SysCtlPeripheralEnable (SYSCTL_PERIPH_ADC0);
    ROM_GPIOPinTypeADC (GPIO_PORTE_BASE, GPIO_PIN_3);

    ADCSequenceConfigure (ADC0_BASE, 3, ADC_TRIGGER_PROCESSOR, 0);
    ADCSequenceStepConfigure (ADC0_BASE, 3, 0, ADC_CTL_CH0 | ADC_CTL_IE | ADC_CTL_END);
    ADCSequenceEnable (ADC0_BASE, 3);
    ADCIntClear (ADC0_BASE, 3);
}


void
ConfigureADC1 (void)
{
    ROM_SysCtlPeripheralEnable (SYSCTL_PERIPH_GPIOE);
    ROM_SysCtlPeripheralEnable (SYSCTL_PERIPH_ADC1);
    ROM_GPIOPinTypeADC (GPIO_PORTE_BASE, GPIO_PIN_2);

    ADCSequenceConfigure (ADC1_BASE, 2, ADC_TRIGGER_PROCESSOR, 0);
    ADCSequenceStepConfigure (ADC1_BASE, 2, 0, ADC_CTL_CH0 | ADC_CTL_IE | ADC_CTL_END);
    ADCSequenceEnable (ADC1_BASE, 2);
    ADCIntClear (ADC1_BASE, 2);
}


void
ConfigureSPI (void)
{
    uint32_t pui32DataRx;

    ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_SSI0);
    ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);
    ROM_GPIOPinConfigure(GPIO_PA2_SSI0CLK);
    ROM_GPIOPinConfigure(GPIO_PA3_SSI0FSS);
    ROM_GPIOPinConfigure(GPIO_PA4_SSI0RX);
    ROM_GPIOPinConfigure(GPIO_PA5_SSI0TX);
    ROM_GPIOPinTypeSSI(GPIO_PORTA_BASE, GPIO_PIN_5 | GPIO_PIN_4 | GPIO_PIN_3 | GPIO_PIN_2);
    SSIConfigSetExpClk(SSI0_BASE, SysCtlClockGet(), SSI_FRF_MOTO_MODE_0, SSI_MODE_MASTER, 1000000, 16);
    SSIEnable(SSI0_BASE);
    while(SSIDataGetNonBlocking(SSI0_BASE, &pui32DataRx))
    {
    }
}


void
ConfigureGPIO (void)
{
    ROM_SysCtlPeripheralEnable (SYSCTL_PERIPH_GPIOF);
    ROM_GPIOPinTypeGPIOOutput (GPIO_PORTF_BASE, GPIO_PIN_3);
    ROM_GPIOPinTypeGPIOOutput (GPIO_PORTF_BASE, GPIO_PIN_1);
}

// Initialize FreeRTOS and start the initial set of tasks.
int
main (void)
{
    // Set the clocking to run at 50 MHz from the PLL.
    ROM_SysCtlClockSet (SYSCTL_SYSDIV_4 | SYSCTL_USE_PLL | SYSCTL_XTAL_16MHZ |
                        SYSCTL_OSC_MAIN);

    // Initialize the UART and configure it for 115,200, 8-N-1 operation.
    ConfigureUART ();
    ConfigureGPIO ();
    ConfigureADC0 ();
    ConfigureADC1 ();
    ConfigureSPI ();

    // Print demo introduction.
    UARTprintf ("\n\nENCE464 Helicopter Rig Emulator\n");

    // Create a mutex to guard the UART.
    g_pUARTSemaphore = xSemaphoreCreateMutex ();

    // Create an example task.
    if (ADCTaskInit () != 0)
    {
        while (1)
        {
        }
    }

    if (EmulateTaskInit () != 0)
    {
        while (1)
        {
        }
    }

    if (OutTaskInit () != 0)
    {
        while (1)
        {
        }
    }

    // Start the scheduler.  This should not return.
    vTaskStartScheduler ();

    // In case the scheduler returns for some reason, print an error and loop forever.
    while (1)
    {
    }
}
