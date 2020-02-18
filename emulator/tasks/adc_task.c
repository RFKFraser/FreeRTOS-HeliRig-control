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
// Module: adc_task.c
// Author: Matthew Northcott / mno32@uclive.ac.nz
// 
//*****************************************************************************

#include <stdbool.h>
#include <stdint.h>
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "driverlib/gpio.h"
#include "driverlib/adc.h"
#include "adc_task.h"
#include "priorities.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"

#define ADC_TASK_STACK_SIZE 128 // Stack size in words
#define ADC_TASK_ITEM_SIZE sizeof(uint8_t)
#define ADC_TASK_QUEUE_SIZE 5
#define ADC_TASK_DELAY_MS 10


xQueueHandle g_pADC0Queue;
xQueueHandle g_pADC1Queue;


static void
ADCTask (void *pvParameters)
{
    uint32_t ui32ADC0Value = 0;
    uint32_t ui32ADC1Value = 0;
    uint32_t ui32Time = xTaskGetTickCount();

    while (1)
    {
        ADCProcessorTrigger(ADC0_BASE, 3); // Trigger ADC conversion
        while(!ADCIntStatus(ADC0_BASE, 3, false)) // Wait for conversion
        {
        }
        ADCIntClear(ADC0_BASE, 3); // Clear interrupt
        ADCSequenceDataGet(ADC0_BASE, 3, &ui32ADC0Value); // Get ADC value
        if (xQueueSend(g_pADC0Queue, &ui32ADC0Value, portMAX_DELAY) != pdPASS)
        {
           // Queue full - this should never happen
           while (1)
           {
           }
        }

        ADCProcessorTrigger(ADC1_BASE, 2); // Trigger ADC conversion
        while(!ADCIntStatus(ADC1_BASE, 2, false)) // Wait for conversion
        {
        }
        ADCIntClear(ADC1_BASE, 2); // Clear interrupt
        ADCSequenceDataGet(ADC1_BASE, 2, &ui32ADC1Value); // Get ADC value
        if (xQueueSend(g_pADC1Queue, &ui32ADC1Value, portMAX_DELAY) != pdPASS)
        {
           // Queue full - this should never happen
           while (1)
           {
           }
        }

        vTaskDelayUntil(&ui32Time, ADC_TASK_DELAY_MS / portTICK_RATE_MS);
    }
}


uint32_t
ADCTaskInit (void)
{
    g_pADC0Queue = xQueueCreate(ADC_TASK_QUEUE_SIZE, ADC_TASK_ITEM_SIZE);
    g_pADC1Queue = xQueueCreate(ADC_TASK_QUEUE_SIZE, ADC_TASK_ITEM_SIZE);

    if (xTaskCreate(ADCTask, (const portCHAR *)"ADC", ADC_TASK_STACK_SIZE, NULL,
                    tskIDLE_PRIORITY + PRIORITY_ADC_TASK, NULL) != pdTRUE)
    {
        return 1;
    }

    return 0;
}
