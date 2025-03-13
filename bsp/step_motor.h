#ifndef __STEP_MOTOR_H
#define __STEP_MOTOR_H
#include "main.h"

#define MOTOR_IN1(x)   HAL_GPIO_WritePin(GPIOB, GPIO_PIN_3, (GPIO_PinState)(x))
#define MOTOR_IN2(x)   HAL_GPIO_WritePin(GPIOB, GPIO_PIN_4, (GPIO_PinState)(x))
#define MOTOR_IN3(x)   HAL_GPIO_WritePin(GPIOB, GPIO_PIN_5, (GPIO_PinState)(x))
#define MOTOR_IN4(x)   HAL_GPIO_WritePin(GPIOB, GPIO_PIN_6, (GPIO_PinState)(x))

#define STEPMOTOR_MAXSPEED        1  
#define STEPMOTOR_MINSPEED        5  

#define STEP_MOTOR_BYTE           4
#define STEP_MOTOR_DIR_CLOCKWISE  1
#define STEP_MOTOR_DIR_UNCLOCKWISE  0
#define STEP_MOTOR_SPEEP          2
#define STEP_MOTOR_ANGLE          180
#define STEP_MOTOR_START          1
#define STEP_MOTOR_STOP           0

void curtain_open(void);
void curtain_open_angle(uint16_t angle);
void curtain_close_angle(uint16_t angle);
uint16_t curtain_get_curangle(void);
#endif
