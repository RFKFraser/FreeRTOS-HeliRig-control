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
// Module: rotor.h
//
//*****************************************************************************


#ifndef ROTOR_H_
#define ROTOR_H_


#define ROTOR_DUTY_MIN                  2
#define ROTOR_DUTY_MAX                  98
#define ROTOR_FRICTION_COEFFICIENT      0.150f
#define ROTOR_LIFT_COEFFICIENT          1.924f


typedef struct rotor_s {
    uint8_t duty;
    float mass;
    float radius;
    float torque;
    float speed;
    float lift;
    float inertia;
    float k_torque;
} rotor_t;


void rotor_init (rotor_t *rotor, float mass, float radius, float k_torque);
void rotor_set_duty (rotor_t *rotor, uint8_t duty);
void rotor_update (rotor_t *rotor, float dt);


#endif /* ROTOR_H_ */
