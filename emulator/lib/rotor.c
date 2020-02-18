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
// Module: rotor.c
//
//*****************************************************************************


#include <stdint.h>
#include "rotor.h"


void
rotor_init (rotor_t *rotor, float mass, float radius, float k_torque)
{
    rotor->mass = mass;
    rotor->radius = radius;
    rotor->duty = ROTOR_DUTY_MIN;
    rotor->lift = 0.0f;
    rotor->speed = 0.0f;
    rotor->torque = 0.0f;
    rotor->inertia = mass * radius * radius;
    rotor->k_torque = k_torque;
}


void
rotor_set_duty (rotor_t *rotor, uint8_t duty)
{
    if (duty < ROTOR_DUTY_MIN)
    {
        duty = ROTOR_DUTY_MIN;
    }
    else if (duty > ROTOR_DUTY_MAX)
    {
        duty = ROTOR_DUTY_MAX;
    }

    rotor->duty = duty;
}


void
rotor_update (rotor_t *rotor, float dt)
{
    rotor->torque = rotor->k_torque * rotor->duty;

    float friction_torque = ROTOR_FRICTION_COEFFICIENT * rotor->speed;
    float alpha = (rotor->torque - friction_torque) / rotor->inertia;

    rotor->speed += alpha * dt;
    rotor->lift = ROTOR_LIFT_COEFFICIENT * rotor->radius * rotor->radius * rotor->radius * rotor->radius * rotor->speed * rotor->speed;
}
