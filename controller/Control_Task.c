/*
 * Control_Task.c
 * This is the file containing the control algorithm for the PI control scheme.
 *
 *       Author: Group 1
 */

#include <stdbool.h>
#include <stdint.h>
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "driverlib/gpio.h"
#include "driverlib/rom.h"
#include "driverlib/pwm.h"
#include "drivers/rgb.h"
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
#include "buttons4.h"
#include "Yaw_Task.h"
#include "ADC_Task.h"
#include "display.h"
#include "pwmGen.h"

//*****************************************************************************
// Constants
//*****************************************************************************
// Main rotor gains
#define MAIN_ROTOR_P_GAIN 0.5 * 1000
#define MAIN_ROTOR_I_GAIN 0.18 * 1000

// Tail rotor gains
#define TAIL_ROTOR_P_GAIN 0.8 * 1000
#define TAIL_ROTOR_I_GAIN 0.1 * 1000

// Max and Min Duty cycles
#define MIN_DC 2
#define MAX_DC 98

#define SAMPLE_RATE_HZ 500


//*****************************************************************************
// Global variables
//*****************************************************************************
static int32_t g_yaw = -1;
static int32_t g_alt = -1;
static int32_t g_aimYaw = 0;
static int32_t g_aimAlt = 0;
static int32_t g_dc_main;
static int32_t g_dc_tail;
static int32_t g_mainIError = 100000;
static int32_t g_tailIError = 240000;
static uint32_t g_ulDeltaT;
static uint32_t g_ulSampCnt;
static uint32_t g_ulSampCntPrev;


// Initilisations of queue structs
struct altitude
{
    int32_t value;
};

struct yaw
{
    int32_t value;
};


//*****************************************************************************
//
// Task to receive data from the queues based on whether the queue has data in it.
//
//*****************************************************************************
void ReceiveTask( void *pvParameters )
{
extern QueueHandle_t xQueueAlt;
extern QueueHandle_t xQueueYaw;

const TickType_t Delay10ms = pdMS_TO_TICKS( 10 );

struct altitude alti_received;
struct yaw yaw_received;

    for( ;; )
    {
       if( xQueueAlt != 0 )     // If something is in the queue
       {
           // Receive a message on the created queue.  Block for 10 ticks if a
           // message is not immediately available.
           if( xQueueReceive( xQueueAlt, &( alti_received ), ( TickType_t ) 10 ) )
           {
               //Saving the value from the queue.
               g_alt = alti_received.value;
           }
       }
       if( xQueueYaw != 0 )     // If something is in the queue
       {
           // Receive a message on the created queue.  Block for 10 ticks if a
           // message is not immediately available.
           if( xQueueReceive( xQueueYaw, &( yaw_received ), ( TickType_t ) 10 ) )
           {
               //Saving the value from the queue.
               g_yaw = yaw_received.value;
           }
       }
       vTaskDelay( Delay10ms );
    }
}


//*****************************************************************************
//
// Task to display information on the OLED screen on the TIVA
//
//*****************************************************************************
void DisplayTask( void *pvParameters )
{
const TickType_t Delay10ms = pdMS_TO_TICKS( 200 );

int32_t mode_temp = 1;

    for( ;; )
    {
        //int32_t alt, int32_t main_dc, int32_t tail_dc, int32_t yaw
        displayInfo(g_alt, g_dc_main, g_dc_tail, g_yaw);

        //int32_t alt, int32_t alt_aim, int32_t main_dc, int32_t tail_dc, int32_t yaw, int32_t yaw_aim, int32_t g_mode
        displayStatus(g_alt, g_aimAlt, g_dc_main, g_dc_tail, g_yaw, g_aimYaw, mode_temp);

        vTaskDelay( Delay10ms );
    }
}


//*****************************************************************************
//
//calcDeltaT -
// Function to calculate time elapsed since last check.
//
//*****************************************************************************
void calcDeltaT(void)
{
    g_ulSampCnt = getSampCnt();
    if (g_ulSampCnt > g_ulSampCntPrev) {
            g_ulDeltaT = (g_ulSampCnt - g_ulSampCntPrev) * (1000 / SAMPLE_RATE_HZ);
    }
    g_ulSampCntPrev = g_ulSampCnt;
}


//*****************************************************************************
//
//setMainDC -
// Function to perform the PI control math for the main rotor and returns a duty cycle.
//
//*****************************************************************************
int32_t setMainDC(uint32_t alt, uint32_t aim_alt)
{
    int32_t main_dc;

    int32_t error = aim_alt - alt;
    int32_t i_error = error;

    if (error > 17) {
        error = 17;
    } if (i_error > 10) {
        i_error = 10;
    }

    g_mainIError += (i_error * g_ulDeltaT);


    main_dc = ((MAIN_ROTOR_I_GAIN * g_mainIError) + (MAIN_ROTOR_P_GAIN * error * 1000))/1000000;

    if (main_dc > MAX_DC) {
        main_dc = MAX_DC;
    } if (main_dc < MIN_DC) {
        main_dc = MIN_DC;
    }

    return main_dc;
}


//*****************************************************************************
//
//setTailDC -
// Function to perform the PI control math for the tail rotor and returns a duty cycle.
//
//*****************************************************************************
int32_t setTailDC(uint32_t yaw, uint32_t aim_yaw)
{
    int32_t tail_dc;

    int32_t error = aim_yaw - yaw;

    if (error > 30) {
        error = 30;
    }

    g_tailIError += (error * g_ulDeltaT);


    tail_dc = ((TAIL_ROTOR_I_GAIN * g_tailIError) + (TAIL_ROTOR_P_GAIN * error * 1000))/1000000;

    if (tail_dc > MAX_DC) {
        tail_dc = MAX_DC;
    } if (tail_dc < MIN_DC) {
        tail_dc = MIN_DC;
    }

    return tail_dc;
}




//*****************************************************************************
//
// Functions to hand information to and from the state machine
//

void setAimAlt(int32_t aim_alt)
{
    g_aimAlt = aim_alt;
}

void setAimYaw(int32_t aim_yaw)
{
    g_aimYaw = aim_yaw;
}

int32_t getYaw(void)
{
    return g_yaw;
}

int32_t getAlt(void)
{
    return g_alt;
}

//*****************************************************************************


//*****************************************************************************
//
//MotorControlTask -
// Task to find and set the duty cycle of the rotors
//
//*****************************************************************************
void MotorControlTask( void *pvParameters )
{
const TickType_t Delay5ms = pdMS_TO_TICKS( 5 );

    for( ;; )
    {
        calcDeltaT();
        g_dc_tail = setTailDC(g_yaw, g_aimYaw);
        g_dc_main = setMainDC(g_alt, g_aimAlt);

        setMainPWM(g_dc_main);
        setTailPWM(g_dc_tail);
        vTaskDelay( Delay5ms );
    }
}

//*****************************************************************************
//
//ReceiveTaskInit -
// Function to create the control tasks
//
//*****************************************************************************
void ReceiveTaskInit( void )
{
    xTaskCreate( ReceiveTask, "ReceiveTask", 128, NULL, 2, NULL );
    xTaskCreate( DisplayTask, "DisplayTask", 128, NULL, 1, NULL );
    xTaskCreate( MotorControlTask, "MotorControlTask", 128, NULL, 1, NULL );
}
