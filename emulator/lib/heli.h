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
// Module: heli.h
//
//*****************************************************************************

#ifndef HELI_H_
#define HELI_H_


#define GRAVITY                         9.81f

#define ROTOR_MAIN_RADIUS               0.25f
#define ROTOR_MAIN_MASS                 0.50f
#define ROTOR_MAIN_K_TORQUE             1.00f
#define ROTOR_TAIL_RADIUS               0.05f
#define ROTOR_TAIL_MASS                 0.10f
#define ROTOR_TAIL_K_TORQUE             20.0f

#define HELI_MASS                       1.00f + ROTOR_MAIN_MASS + ROTOR_TAIL_MASS
#define HELI_RADIUS                     0.40f
#define HELI_FRICTION_COEFFICIENT       0.35f
#define HELI_WEIGHT                     HELI_MASS * GRAVITY
#define HELI_INERTIA                    HELI_MASS * HELI_RADIUS * HELI_RADIUS;
#define HELI_HEIGHT_MAX                 1.00f
#define HELI_HEIGHT_MIN                 0.00f


void heli_init (void);
void heli_set_main_duty (uint8_t duty);
void heli_set_tail_duty (uint8_t duty);
void heli_update (float dt);
float heli_get_height (void);
float heli_get_yaw (void);


#endif /* HELI_H_ */
