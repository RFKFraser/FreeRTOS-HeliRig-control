//*****************************************************************************
//
// State_Task.c -
//      This file contains the state machine used to control the operational
//      state of the heli
//
//       Author: Group 1
//
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
#include "priorities.h"

//*****************************************************************************
// Constants
//*****************************************************************************
#define STATETASKSTACKSIZE 128


//*****************************************************************************
// Global variables
//*****************************************************************************
static int32_t g_mode = -2;      // -2 = Start Up, -1 = Yaw Ref, 0 = Landed, 1 = Landing, 2 = Flying
static int32_t aim_yaw = 0;
static int32_t aim_alt = 0;



//*****************************************************************************

// altButtons -
// Function to check for altitude button presses and adjust aim alt appropriately..
//
//*****************************************************************************
int32_t altButtons(int32_t aim_alt)
{
    if (checkButton (UP) == PUSHED) {
        aim_alt += 10;
        if (aim_alt > 100) {
            aim_alt = 100;
        }
    }

    if (checkButton (DOWN) == PUSHED) {
        aim_alt -= 10;
        if (aim_alt < 0) {
            aim_alt = 0;
        }
    }
    return aim_alt;
}


//*****************************************************************************
//
// yawButtons -
// Check for altitude button presses and adjust aim yaw appropriately..
//
//*****************************************************************************
int32_t yawButtons(int32_t aim_yaw)
{
    if (checkButton (RIGHT) == PUSHED) {
        aim_yaw += 15;
    }

    if (checkButton (LEFT) == PUSHED) {
        aim_yaw -= 15;
    }
    return aim_yaw;
}


//*****************************************************************************
//
// StateTask -
//  This task contains the state machine for the control of the Heli.
//  The state is dependant on user input through button pushes.
//
//*****************************************************************************
void StateTask( void *pvParameters )
{
const TickType_t Delay20ms = pdMS_TO_TICKS( 20 );
int32_t yaw;
int32_t alt;

    for( ;; )
    {

        if (g_mode == -2)
        {      // Start Up
            updateButtons();
            if (checkButton(MODE) == PUSHED)
            {
                g_mode = -1;
            }

        //*****************************************************************************************
        } if (g_mode == -1) {
            yaw = getYaw();
            aim_alt = 5;
            aim_yaw = yaw - 3;

        //*****************************************************************************************
        } if (g_mode == 0) {     // Landed
            updateButtons();
            if (checkButton(MODE) == PUSHED){
                aim_alt = 0;
                aim_yaw = 0;
                g_mode = 2;
            }
        //*****************************************************************************************
        } if (g_mode == 1) {     // Landing
            yaw = getYaw();
            alt = getAlt();
            aim_yaw = 0;
            if ((3 > yaw) && (yaw > -3)) {
                aim_alt = alt - 10;
            }
            if (((5 > alt) && (alt > -5)) && ((5 > yaw) && (yaw > -5))) {
                aim_alt = 0;
                aim_yaw = 0;
                g_mode = 0;
            }

        //*****************************************************************************************
        } if (g_mode == 2) {     // Flying
            // Update buttons
            updateButtons();
            if (checkButton(MODE) == RELEASED){
                g_mode = 1;
            }
            // Execute button changes
            aim_alt = altButtons(aim_alt);
            aim_yaw = yawButtons(aim_yaw);
        }

        // Setting the aim positions
        setAimAlt(aim_alt);
        setAimYaw(aim_yaw);

        vTaskDelay( Delay20ms );
    }
}

//*****************************************************************************
//
// setYawRef -
//  This function is called by the Yaw reference interrupt handler.
//  It is used to set the aim position and change the mode of the state machine
//
//*****************************************************************************

void setYawRef( void )
{
    aim_yaw = 0;
    aim_alt = 0;
    g_mode = 2;
}


//*****************************************************************************
//
// StateInit -
//  This function calls a button initilisation sequence and creates a task for
//  the state machine.
//
//*****************************************************************************

void StateInit( void )
{
    initButtons();

    //Creating the state machine task
    xTaskCreate( StateTask, "StateTask", STATETASKSTACKSIZE, NULL, tskIDLE_PRIORITY + PRIORITY_STATE_TASK, NULL );
}



