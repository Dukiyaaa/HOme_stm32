#include "sg90.h"
#include "tim.h"

void sg90_init(void)
{
    HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_1);
    HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_2);
    HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_4);
}

//设置SG90角度
void set_sg90_angle(uint16_t angle)
{
    if (angle > 180) angle = 180;  // 限制角度范围
    uint16_t pwm_value = (angle * (SERVO_MAX - SERVO_MIN) / 180) + SERVO_MIN;
    __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_1, pwm_value);  // 更新 PWM
}

uint8_t door_state;
int door_open(void) 
{
    if(door_state == 1) {
        printf("[ERROR]door has opened, can not open again!!!\n");
        return 0;
    }
    printf("door is openning\n");
    door_state = 1;
    set_sg90_angle(DOOR_OPEN_ANGLE);
	camera_ture_left();
    return 1;
}

int door_close(void)
{
    if(door_state == 0) {
        printf("[ERROR]door has closed, can not close again!!!\n");
        return 0;
    }
    printf("door is closeing\n");
    door_state = 0;
    set_sg90_angle(DOOR_CLOSE_ANGLE);
	camera_ture_right();
    return 1;
}

uint8_t door_get_curstate(void)
{
    return door_state;
}

void set_sg90_3_angle(uint16_t angle)
{
    if (angle > 180) angle = 180;  // 限制角度范围
    uint16_t pwm_value = (angle * (SERVO_MAX - SERVO_MIN) / 180) + SERVO_MIN;
    __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_2, pwm_value);  // 更新 PWM
}

void set_sg90_2_angle(uint16_t angle)
{
    if (angle > 180) angle = 180;  // 限制角度范围
    uint16_t pwm_value = (angle * (SERVO_MAX - SERVO_MIN) / 180) + SERVO_MIN;
    __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_4, pwm_value);  // 更新 PWM
}

void camera_ture_left(void)
{
	set_sg90_2_angle(90);
}

void camera_ture_right(void)
{
	set_sg90_2_angle(0);
}