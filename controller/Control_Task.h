/*
 * Control_Task.h
 *
 *  Created on: 21/08/2019
 *      Author: sma286
 */

#ifndef CONTROL_TASK_H_
#define CONTROL_TASK_H_

extern void calcDeltaT(void);
extern int32_t setMainDC(uint32_t alt, uint32_t aim_alt);
extern int32_t setTailDC(uint32_t yaw, uint32_t aim_yaw);
extern void setAimAlt(int32_t aim_alt);
extern void setAimYaw(int32_t aim_yaw);
extern uint32_t getYaw(void);
extern uint32_t getAlt(void);
extern void ReceiveTaskInit(void);

#endif /* CONTROL_TASK_H_ */
