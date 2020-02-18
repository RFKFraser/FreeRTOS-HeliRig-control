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
// Module: heli.c
// Author: Matthew Northcott / mno32@uclive.ac.nz
//
//*****************************************************************************

#include <math.h>
#include <stdint.h>
#include "heli.h"
#include "rotor.h"


static rotor_t rotor_main;
static rotor_t rotor_tail;
static float height = 0;
static float y_velocity = 0;
static float theta = 0;
static float angular_velocity = 0;


void
heli_init (void)
{
    rotor_init (&rotor_main, ROTOR_MAIN_MASS, ROTOR_MAIN_RADIUS, ROTOR_MAIN_K_TORQUE);
    rotor_init (&rotor_tail, ROTOR_TAIL_MASS, ROTOR_TAIL_RADIUS, ROTOR_MAIN_K_TORQUE);
    height = 0;
    y_velocity = 0;
    angular_velocity = 0;
}


void
heli_set_main_duty (uint8_t duty)
{
    rotor_set_duty (&rotor_main, duty);
}


void
heli_set_tail_duty (uint8_t duty)
{
    rotor_set_duty (&rotor_tail, duty);
}


void
heli_update (float dt)
{
    /* VERTICAL MOVEMENT */
    float friction_force = HELI_FRICTION_COEFFICIENT * y_velocity;
    float net_force = rotor_main.lift - HELI_WEIGHT - friction_force;

    float a = net_force / HELI_MASS;

    if (height <= 0)
    {
        if (a < 0)
        {
            a = 0;
        }

        if (y_velocity < 0)
        {
            y_velocity = 0;
        }
    }

    y_velocity += a * dt;
    height += y_velocity * dt;

    /* ROTATIONAL MOVEMENT */
    float rotor_torque = HELI_RADIUS * rotor_tail.lift;
    float friction_torque = HELI_FRICTION_COEFFICIENT * angular_velocity;
    float reaction_torque = rotor_main.torque;
    float net_torque = rotor_torque - friction_torque - reaction_torque;
    float alpha = net_torque / HELI_INERTIA;

    angular_velocity += alpha * dt;
    theta += angular_velocity * dt;
}


float heli_get_height (void)
{
    return height;
}

float heli_get_yaw (void)
{
    float yaw = fmod (114.59 * theta, 360.0f);

    if (yaw > 180.0f)
    {
        yaw -= 360.0f;
    }

    return yaw;
}
