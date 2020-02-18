/*
 * Yaw_Task.c
 *  This file contains functions and tasks that find the Raw Yaw value,
 *  convert that value into degrees and sends the value to a queue.
 *  This file also contains the interrupt handler for the Yaw reference.
 *      Author: Group 1
 */

#include <stdbool.h>
#include <stdint.h>
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "driverlib/gpio.h"
#include "driverlib/rom.h"
#include "driverlib/pwm.h"
#include "drivers/rgb.h"
#include "drivers/buttons.h"
#include "utils/uartstdio.h"
#include "pin_map.h"
#include "driverlib/sysctl.h"
#include "driverlib/adc.h"
#include "priorities.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"
#include "pwmGen.h"
#include "State_Task.h"

//*****************************************************************************
// Constants
//*****************************************************************************
#define YAWTASKSTACKSIZE 128
#define YAWREFTASKSTACKSIZE 128
#define ENCODER_POLES 112

//*****************************************************************************
// Global variables
//*****************************************************************************
volatile int32_t g_yawCounter = 0;
static int32_t g_yawPrevB = 0;

// Initilising queues
QueueHandle_t xQueueYaw = NULL;

// Initilising the struct for the Yaw Queue
struct yaw
 {
    int32_t value;
 } xYaw;


 //*****************************************************************************
 //
 // yawRefHandler -
 //     This is an interrupt handler for the yaw reference interrupt.
 //     Resets the Yaw counters and disables the interrupt for the rest of the programs execution.
 //
 //*****************************************************************************
 void yawRefHandler (void)
 {
     setYawRef();
     g_yawCounter = 0;
     g_yawPrevB = 0;
     GPIOIntDisable(GPIO_PORTC_BASE, GPIO_PIN_4);
 }


 //*****************************************************************************
 //
 // yawIntHandler -
 //     This is an interrupt handler for the yaw interrupt.
 //
 //*****************************************************************************

 void yawIntHandler (void)
 {
     int32_t yawCurrA = GPIOPinRead(GPIO_PORTB_BASE, GPIO_PIN_0);
     int32_t yawCurrB = GPIOPinRead(GPIO_PORTB_BASE, GPIO_PIN_1) >> 1;

     if (g_yawPrevB == yawCurrA) {
         g_yawCounter += 1;
     } else {
         g_yawCounter -= 1;
     }

     g_yawPrevB = yawCurrB;

     GPIOIntClear(GPIO_PORTB_BASE, GPIO_PIN_0);
     GPIOIntClear(GPIO_PORTB_BASE, GPIO_PIN_1);
 }


 //*****************************************************************************
 //
 // vYawSenderTask -
 //     This task sends the current Yaw data to the Yaw data Queue
 //
 //*****************************************************************************

static void vYawSenderTask(void *pvParameters)
{
const TickType_t Delay10ms = pdMS_TO_TICKS( 10 );

struct yaw yaw_now;
xQueueYaw = xQueueCreate( 1, sizeof( struct yaw ) );

    for( ;; )
    {
        // Block for 10ms
        vTaskDelay( Delay10ms );

        // Convert yaw from pole count to degrees
        yaw_now.value = (g_yawCounter * 360)/(ENCODER_POLES * 2 * 2);

        // Send yaw in degrees to yaw queue
        xQueueSend( xQueueYaw, ( void * ) &yaw_now, ( TickType_t ) 10 );
    }
}


//*****************************************************************************
//
// Initialisation functions for the yaw
//
//*****************************************************************************
void YawInit (void)
{
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOC);

    GPIOPinTypeGPIOInput(GPIO_PORTB_BASE, GPIO_PIN_0);
    GPIOPinTypeGPIOInput(GPIO_PORTB_BASE, GPIO_PIN_1);
    GPIOPinTypeGPIOInput(GPIO_PORTC_BASE, GPIO_PIN_4);

    GPIOPadConfigSet(GPIO_PORTB_BASE, GPIO_PIN_0, GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD_WPD);
    GPIOPadConfigSet(GPIO_PORTB_BASE, GPIO_PIN_1, GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD_WPD);
    GPIOPadConfigSet(GPIO_PORTC_BASE, GPIO_PIN_4, GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD_WPU);

    GPIOIntDisable(GPIO_PORTB_BASE, GPIO_PIN_0);        // Disable interrupt for PB0 (in case it was enabled)
    GPIOIntDisable(GPIO_PORTB_BASE, GPIO_PIN_1);        // Disable interrupt for PB1 (in case it was enabled)
    GPIOIntDisable(GPIO_PORTC_BASE, GPIO_PIN_4);

    GPIOIntClear(GPIO_PORTB_BASE, GPIO_PIN_0);      // Clear pending interrupts for PB0
    GPIOIntClear(GPIO_PORTB_BASE, GPIO_PIN_1);      // Clear pending interrupts for PB1
    GPIOIntClear(GPIO_PORTC_BASE, GPIO_PIN_4);

    GPIOIntRegister(GPIO_PORTB_BASE, yawIntHandler);     // Register our handler function for port B
    GPIOIntRegister(GPIO_PORTC_BASE, yawRefHandler);     // Register our handler function for port C

    GPIOIntTypeSet(GPIO_PORTB_BASE, GPIO_PIN_0, GPIO_BOTH_EDGES);             // Configure PB0 for edge trigger
    GPIOIntTypeSet(GPIO_PORTB_BASE, GPIO_PIN_1, GPIO_BOTH_EDGES);             // Configure PB1 for edge trigger
    GPIOIntTypeSet(GPIO_PORTC_BASE, GPIO_PIN_4, GPIO_FALLING_EDGE);

    GPIOIntEnable(GPIO_PORTB_BASE, GPIO_PIN_0);     // Enable interrupt for PB0
    GPIOIntEnable(GPIO_PORTB_BASE, GPIO_PIN_1);     // Enable interrupt for PB1
    GPIOIntEnable(GPIO_PORTC_BASE, GPIO_PIN_4);

    //
    // Create the YAW queue sending task.
    //
    xTaskCreate( vYawSenderTask, "YawSender", 128, NULL, 1, NULL );
}
