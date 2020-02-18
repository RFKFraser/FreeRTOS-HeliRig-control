//*****************************************************************************
//
// display.c - OLED and UART display.
//
// Authors: Michael Hatton  mch251  23811860
//          Michael Jopson  mjo115  86075724
//          Lucas   Toovey  lto47   89923812
//
// Last modified:   29.05.2018
//
//*****************************************************************************

#ifndef DISPLAY_H_
#define DISPLAY_H_

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
//
// Initialisation functions for the display
//
//      Pulled from ADCdemo1.c, Author:  P.J. Bones  UCECE
//
//*****************************************************************************
void initDisplay (void);

//********************************************************
//
// initialiseUSB_UART - 8 bits, 1 stop bit, no parity
//
//********************************************************
void initialiseUSB_UART (void);

//**********************************************************************
//
// Transmit a string via UART0
//
//**********************************************************************
void UARTSend (char *pucBuffer);

//*****************************************************************************
//
// Function to display status info to OLED display.
//
//*****************************************************************************
void displayInfo(int32_t alt, int32_t main_dc, int32_t tail_dc, int32_t yaw);

//*****************************************************************************
//
// Function to display status info to UART.
//
//*****************************************************************************
void displayStatus(int32_t alt, int32_t alt_aim, int32_t main_dc, int32_t tail_dc, int32_t yaw, int32_t yaw_aim, int32_t g_mode);

#endif /* DISPLAY_H_ */
