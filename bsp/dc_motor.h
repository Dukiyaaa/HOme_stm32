#ifndef __DC_MOTOR_H
#define __DC_MOTOR_H
#include "main.h"

#define DC_MOTOR_LOW 1
#define DC_MOTOR_MEDIUM 2
#define DC_MOTOR_HIGH 3

void airConditioner_init(void);

void STM32_PWM_Start(TIM_HandleTypeDef *htim, uint32_t Channel, uint32_t duty);

void STM32_PWM_Stop(TIM_HandleTypeDef *htim, uint32_t Channel);

uint8_t airConditioner_heat(uint8_t type);

uint8_t airConditioner_cool(uint8_t type);
uint8_t airConditioner_stop(void);

void airConditioner_work(uint8_t airConditioner_state);

uint8_t airConditioner_getState(void);

#endif
