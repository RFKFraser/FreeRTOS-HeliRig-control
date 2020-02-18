/*
 * ADC_Task.h
 * This file contains the initilisation and use of the ADC on the Tiva,
 * reading altitude from the ADC, converts to a percentage and sends it to a queue.
 *
 * Author: Group 1
 */

#ifndef ADC_TASK_H_
#define ADC_TASK_H_

extern int32_t calcPercent(uint16_t meanVal, uint32_t offset);
extern uint32_t getSampCnt(void);
extern uint32_t ADCTaskInit(void);

#endif /* ADC_TASK_H_ */
