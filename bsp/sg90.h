#ifndef __SG90_H
#define __SG90_H
#include "main.h"

#define DOOR_OPEN_ANGLE 180
#define DOOR_CLOSE_ANGLE 0

#define SERVO_MIN 500   // 0° 对应的 CCR 值
#define SERVO_MAX 2500  // 180° 对应的 CCR 值

void sg90_init(void);
void set_sg90_angle(uint16_t angle);
int door_open(void) ;
int door_close(void);
uint8_t door_get_curstate(void);
#endif
