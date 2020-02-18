/*
 * ADC_Task.c
 * This file contains the initilisation and use of the ADC on the Tiva,
 * reading altitude from the ADC, converts to a percentage and sends it to a queue.
 *
 * Author: Group 1
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

//*****************************************************************************
// Constants
//*****************************************************************************
#define BUF_SIZE 10
#define ADCTASKSTACKSIZE 128
#define VOLTAGE_RANGE 1000               // Voltage range in mV
#define DISTANCE_SENSOR_STEP 4096/3300  // Bits per mV (Bit range / voltage range)
#define SAMPLE_FACTOR 32

// Declaring the queue handle
QueueHandle_t xQueueAlt = NULL;

//*****************************************************************************
// Global variables
//*****************************************************************************
static uint32_t g_ulSampCnt = 0;    // Counter for the interrupts
volatile int32_t g_ADCCount = 0;
static int32_t alt_raw;
static int32_t offset = 0;
static uint32_t ulValue;  //Buffered altitude


//Initilising the altitude struct
struct altitude
 {
    int32_t value;
 };


//*****************************************************************************
//
// Function to calculate the altitude of the helicopter as a percentage.
//
//*****************************************************************************
int32_t calcPercent(uint16_t meanVal, uint32_t offset)
{
    uint16_t step = (VOLTAGE_RANGE * DISTANCE_SENSOR_STEP)/100;
    int32_t percent;

    percent = (offset/step) - (meanVal/step);
    if (percent < 0)
    {
        percent = 0;
    }
    else if(percent > 100)
    {
        percent = 100;
    }
    return percent;
}

//*****************************************************************************
//
// AltSenderTask -
//      This task creates the altitude queue and sends the altitude to that queue.
//
//*****************************************************************************
static void vAltSenderTask( void *pvParameters)
{
const TickType_t xBlockTime = pdMS_TO_TICKS( 10 );

//Creating a queue and a struct for the altitude data
struct altitude alti_now;
xQueueAlt = xQueueCreate( 1, sizeof( struct altitude ) );


/* As per most tasks, this task is implemented within an infinite loop. */
    for( ;; )
    {
        /* Block for 200ms. */
        vTaskDelay( xBlockTime );

        // Setting the struct value to be the value read from the ADC
        alti_now.value = g_ADCCount;

        //Sending the struct with the altitude data to the queue
        xQueueSend( xQueueAlt, ( void * ) &alti_now, ( TickType_t ) 10 ); //&ADCCountSend
    }
}



//*****************************************************************************
//
// ADCTask -
//      This task gets data from the ADC and processes it ready for sending to the queue
//
//*****************************************************************************
void ADCTask( void *pvParameters )
{
    TickType_t xLastWakeTime;
    const TickType_t xDelayms = pdMS_TO_TICKS( 1 );
    xLastWakeTime = xTaskGetTickCount();


     /* As per most tasks, this task is implemented in an infinite loop. */

     for( ;; )
     {
         // Get the single sample from ADC0.  ADC_BASE is defined in
         // inc/hw_memmap.h
         ADCSequenceDataGet(ADC0_BASE, 3, &ulValue);

         //Trigger the ADC calculation for the next ADC read
         ADCProcessorTrigger(ADC0_BASE, 3);

         //Increasing the sample counter used in PID control
         g_ulSampCnt++;

         // Waiting for 30 samples to set the offset
         if (g_ulSampCnt < 30) {
             alt_raw = ulValue;
             offset = alt_raw;
         } else {
             alt_raw = ulValue;
             g_ADCCount = calcPercent(alt_raw, offset);     // Calculating the altitude as a percentage
         }

         /* The task should execute every 3 milliseconds exactly – see the
         declaration of xDelay3ms in this function. */
         vTaskDelayUntil( &xLastWakeTime, xDelayms );
     }
}

//*****************************************************************************
//
// Function to Hand the ADC sample count between files.
//
//*****************************************************************************
uint32_t getSampCnt(void)
{
    return g_ulSampCnt;
}

//*****************************************************************************
//
// Initialisation functions for the ADC
//  Creates tasks for processing ADC data and sending it to a queue.
//
//*****************************************************************************
uint32_t
ADCTaskInit (void)
{
    // The ADC0 peripheral must be enabled for configuration and use.
    SysCtlPeripheralEnable(SYSCTL_PERIPH_ADC0);

    // Enable sample sequence 3 with a processor signal trigger.  Sequence 3
    // will do a single sample when the processor sends a signal to start the
    // conversion.
    ADCSequenceConfigure(ADC0_BASE, 3, ADC_TRIGGER_PROCESSOR, 0);

    // Configure step 0 on sequence 3.  Sample channel 0 (ADC_CTL_CH0) in
    // single-ended mode (default) and configure the interrupt flag
    // (ADC_CTL_IE) to be set when the sample is done.  Tell the ADC logic
    // that this is the last conversion on sequence 3 (ADC_CTL_END).  Sequence
    // 3 has only one programmable step.  Sequence 1 and 2 have 4 steps, and
    // sequence 0 has 8 programmable steps.  Since we are only doing a single
    // conversion using sequence 3 we will only configure step 0.  For more
    // on the ADC sequences and steps, refer to the LM3S1968 datasheet.
    ADCSequenceStepConfigure(ADC0_BASE, 3, 0, ADC_CTL_CH9 | ADC_CTL_IE |
                             ADC_CTL_END);

    // Since sample sequence 3 is now configured, it must be enabled.
    ADCSequenceEnable(ADC0_BASE, 3);

    //Trigger the ADC calculation for the next ADC read
    ADCProcessorTrigger(ADC0_BASE, 3);

    // Configuring the use of hardware averaging
    ADCHardwareOversampleConfigure(ADC0_BASE, SAMPLE_FACTOR);

    //Increasing the sample counter used in PI control
    g_ulSampCnt++;

    // Enable interrupts for ADC0 sequence 3 (clears any outstanding interrupts)
    ADCIntEnable(ADC0_BASE, 3);

    // Create the ADC task.
    // Creating the Queue sender task.
    xTaskCreate( vAltSenderTask, "AltSender", ADCTASKSTACKSIZE, NULL, tskIDLE_PRIORITY + PRIORITY_ALTSEND_TASK, NULL );
    xTaskCreate( ADCTask, "ADC", ADCTASKSTACKSIZE, NULL, tskIDLE_PRIORITY + PRIORITY_ADC_TASK, NULL);

    return(0);

}

















