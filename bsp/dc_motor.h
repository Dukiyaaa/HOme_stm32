#ifndef __DC_MOTOR_H
#define __DC_MOTOR_H
#include "main.h"

#define DC_MOTOR_LOW 0
#define DC_MOTOR_MEDIUM 1
#define DC_MOTOR_HIGH 2

void airConditioner_init(void);

void STM32_PWM_Start(TIM_HandleTypeDef *htim, uint32_t Channel, uint32_t duty);

void STM32_PWM_Stop(TIM_HandleTypeDef *htim, uint32_t Channel);

void airConditioner_heat(int type);

void airConditioner_cool(int type);
void airConditioner_stop(void);

void airConditioner_work(uint8_t airConditioner_state);

uint8_t airConditioner_getState(void);

#endif
