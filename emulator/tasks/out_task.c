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
// Module: out_task.c
// 
//*****************************************************************************

#include <stdbool.h>
#include <stdint.h>
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "driverlib/gpio.h"
#include "driverlib/ssi.h"
#include "out_task.h"
#include "priorities.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "lib/heli.h"


#define OUT_TASK_STACK_SIZE 128 // Stack size in words
#define OUT_TASK_ITEM_SIZE sizeof(uint8_t)
#define OUT_TASK_QUEUE_SIZE 5
#define OUT_TASK_DELAY_MS 10


static void
OutTask (void *pvParameters)
{
    uint32_t ui32Time = xTaskGetTickCount();

    float fYaw = 0.0f;
    uint16_t ui16DACValue = 0;

    while (1)
    {
        // Output GPIO pin when at reference position
        fYaw = heli_get_yaw ();
        if (-3.0f < fYaw && fYaw < 3.0f)
        {
            GPIOPinWrite (GPIO_PORTF_BASE, GPIO_PIN_3, GPIO_PIN_3); // Turn LED on
            GPIOPinWrite (GPIO_PORTF_BASE, GPIO_PIN_1, 0);
        }
        else
        {
            GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_3, 0); // Turn LED off
            GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1, GPIO_PIN_1);
        }

        // Output height via DAC
        ui16DACValue = 4095.0f * (heli_get_height () / HELI_HEIGHT_MAX);
        SSIDataPut (SSI0_BASE, 0x3000 | ui16DACValue);
        while (SSIBusy (SSI0_BASE))
        {
        }

        vTaskDelayUntil(&ui32Time, OUT_TASK_DELAY_MS / portTICK_RATE_MS);
    }
}


uint32_t
OutTaskInit (void)
{
    if (xTaskCreate(OutTask, (const portCHAR *)"Out", OUT_TASK_STACK_SIZE, NULL,
                    tskIDLE_PRIORITY + PRIORITY_OUT_TASK, NULL) != pdTRUE)
    {
        return 1;
    }

    return 0;
}
