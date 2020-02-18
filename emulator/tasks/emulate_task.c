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
// Module: emulate_task.c
// Author: Matthew Northcott / mno32@uclive.ac.nz
// 
//*****************************************************************************

#include <stdbool.h>
#include <stdint.h>
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "driverlib/gpio.h"
#include "adc_task.h"
#include "emulate_task.h"
#include "priorities.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "lib/heli.h"

#define EMULATE_TASK_STACK_SIZE 128 // Stack size in words
#define EMULATE_TASK_ITEM_SIZE sizeof(uint8_t)
#define EMULATE_TASK_QUEUE_SIZE 5
#define EMULATE_TASK_DELAY_MS 10


extern xQueueHandle g_pADC0Queue;
extern xQueueHandle g_pADC1Queue;


static void
EmulateTask (void *pvParameters)
{
    uint32_t ui32Time = xTaskGetTickCount();
    uint16_t ui16ADC0Value = 0;
    uint16_t ui16ADC1Value = 0;
    uint8_t ui8DutyMain = 0;
    uint8_t ui8DutyTail = 0;

    heli_init ();

    while (1)
    {
        if (xQueueReceive(g_pADC0Queue, &ui16ADC0Value, 0) == pdPASS)
        {
            ui8DutyMain = 100 * ui16ADC0Value / 4096.0f;
        }

        if (xQueueReceive(g_pADC1Queue, &ui16ADC1Value, 0) == pdPASS)
        {
            ui8DutyTail = 100 * ui16ADC1Value / 4096.0f;
        }

        heli_set_main_duty (ui8DutyMain);
        heli_set_tail_duty (ui8DutyTail);

        heli_update (EMULATE_TASK_DELAY_MS / 1000.0f);
        vTaskDelayUntil(&ui32Time, EMULATE_TASK_DELAY_MS / portTICK_RATE_MS);
    }
}


uint32_t
EmulateTaskInit (void)
{
    if (xTaskCreate(EmulateTask, (const portCHAR *)"Emulate", EMULATE_TASK_STACK_SIZE, NULL,
                    tskIDLE_PRIORITY + PRIORITY_EMULATE_TASK, NULL) != pdTRUE)
    {
        return 1;
    }

    return 0;
}
