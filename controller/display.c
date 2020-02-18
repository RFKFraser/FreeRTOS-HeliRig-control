//*****************************************************************************
//
// display.c - OLED and UART display.
//
//       Author: Group 1
//
//*****************************************************************************

#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "inc/hw_ints.h"
#include "driverlib/gpio.h"
#include "driverlib/uart.h"
#include "driverlib/sysctl.h"
#include "driverlib/systick.h"
#include "driverlib/interrupt.h"
#include "driverlib/debug.h"
#include "utils/ustdlib.h"
#include "stdio.h"
#include "stdlib.h"
#include "FreeRTOS.h"
#include "semphr.h"
#include "portmacro.h"
#include "OrbitOLED/OrbitOLEDInterface.h"

//*****************************************************************************
// Constants
//*****************************************************************************
#define SLOWTICK_RATE_HZ 4
#define MAX_STR_LEN 16
//---USB Serial comms: UART0, Rx:PA0 , Tx:PA1
#define BAUD_RATE 9600
#define UART_USB_BASE           UART0_BASE
#define UART_USB_PERIPH_UART    SYSCTL_PERIPH_UART0
#define UART_USB_PERIPH_GPIO    SYSCTL_PERIPH_GPIOA
#define UART_USB_GPIO_BASE      GPIO_PORTA_BASE
#define UART_USB_GPIO_PIN_RX    GPIO_PIN_0
#define UART_USB_GPIO_PIN_TX    GPIO_PIN_1
#define UART_USB_GPIO_PINS      UART_USB_GPIO_PIN_RX | UART_USB_GPIO_PIN_TX

//*****************************************************************************
// Global variables
//*****************************************************************************
char statusStr[MAX_STR_LEN + 1];
extern xSemaphoreHandle g_pUARTSemaphore;

//*****************************************************************************
//
// Initialisation functions for the display
//
//      Pulled from ADCdemo1.c, Author:  P.J. Bones  UCECE
//
//*****************************************************************************
void initDisplay (void)
{
    // intialise the Orbit OLED display
    OLEDInitialise ();
}



//********************************************************
//
// initialiseUSB_UART - 8 bits, 1 stop bit, no parity
//
//********************************************************
void initialiseUSB_UART (void)
{
    //
    // Enable GPIO port A which is used for UART0 pins.
    //
    SysCtlPeripheralEnable(UART_USB_PERIPH_UART);
    SysCtlPeripheralEnable(UART_USB_PERIPH_GPIO);
    //
    // Select the alternate (UART) function for these pins.
    //
    GPIOPinTypeUART(UART_USB_GPIO_BASE, UART_USB_GPIO_PINS);
    UARTConfigSetExpClk(UART_USB_BASE, SysCtlClockGet(), BAUD_RATE,
            UART_CONFIG_WLEN_8 | UART_CONFIG_STOP_ONE |
            UART_CONFIG_PAR_NONE);
    UARTFIFOEnable(UART_USB_BASE);
    UARTEnable(UART_USB_BASE);
}



//**********************************************************************
//
// Transmit a string via UART0
//
//**********************************************************************
void UARTSend (char *pucBuffer)
{
    // Loop while there are more characters to send.
    while(*pucBuffer)
    {
        // Write the next character to the UART Tx FIFO.
        UARTCharPut(UART0_BASE, *pucBuffer);
        pucBuffer++;
    }
}



//*****************************************************************************
//
// Function to display status info to OLED display.
//
//*****************************************************************************
void displayInfo(int32_t alt, int32_t main_dc, int32_t tail_dc, int32_t yaw)
{
    char string[17];  // 16 characters across the display

    usnprintf (string, sizeof(string), "Altitude = %3d%%", alt);
    // Update line 0 on display.
    OLEDStringDraw (string, 0, 0);

    usnprintf (string, sizeof(string), "Yaw (deg) = %4d", yaw);
    // Update line 1 on display.
    OLEDStringDraw (string, 0, 1);

    usnprintf (string, sizeof(string), "Main DC = %3d%%", main_dc);
    // Update line 2 on display.
    OLEDStringDraw (string, 0, 2);

    usnprintf (string, sizeof(string), "Tail DC = %3d%%", tail_dc);
    // Update line 3 on display.
    OLEDStringDraw (string, 0, 3);
}



//*****************************************************************************
//
// Function to display status info to UART.
//
//*****************************************************************************
void displayStatus(int32_t alt, int32_t alt_aim, int32_t main_dc, int32_t tail_dc, int32_t yaw, int32_t yaw_aim, int32_t g_mode)
{
    xSemaphoreTake(g_pUARTSemaphore, portMAX_DELAY);
    // Form and send a status message to the console
    usnprintf(statusStr, sizeof(statusStr), "Heli Rig Ctrl\n\r", alt, alt_aim);
    UARTSend (statusStr);
    usnprintf(statusStr, sizeof(statusStr), "ALT = %3d (%3d)\n\r", alt, alt_aim);
    UARTSend (statusStr);
    usnprintf(statusStr, sizeof(statusStr), "ALT DC = %3d%%\n\r", main_dc);
    UARTSend (statusStr);
    usnprintf(statusStr, sizeof(statusStr), "YAW = %3d (%3d)\n\r", yaw, yaw_aim);
    UARTSend (statusStr);
    usnprintf(statusStr, sizeof(statusStr), "YAW DC = %3d%%\n\r", tail_dc);
    UARTSend (statusStr);
    xSemaphoreGive(g_pUARTSemaphore);

    if (g_mode == 2) {
        usnprintf(statusStr, sizeof(statusStr), "MODE = Flying\n\r");
        UARTSend (statusStr);
    } else if (g_mode == 1) {
        usnprintf(statusStr, sizeof(statusStr), "MODE = Landing\n\r");
        UARTSend (statusStr);
    } else if (g_mode == 0) {
        usnprintf(statusStr, sizeof(statusStr), "MODE = Landed\n\r");
        UARTSend (statusStr);
    } else if (g_mode == -1) {
        usnprintf(statusStr, sizeof(statusStr), "MODE = Yaw Ref\n\r");
        UARTSend (statusStr);
    } else if (g_mode == -2) {
        usnprintf(statusStr, sizeof(statusStr), "MODE = Start Up\n\r");
        UARTSend (statusStr);
    }
}
