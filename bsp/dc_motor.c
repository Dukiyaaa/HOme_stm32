#include "dc_motor.h"
#include "tim.h"

int32_t speed_array[] = {20000,30000,40000}; //速度数组

void airConditioner_init(void)
{
		HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_1);
    HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_2);
}

void STM32_PWM_Start(TIM_HandleTypeDef *htim, uint32_t Channel, uint32_t duty)
{
    __HAL_TIM_SET_COMPARE(htim, Channel, duty);
}

void STM32_PWM_Stop(TIM_HandleTypeDef *htim, uint32_t Channel)
{
    __HAL_TIM_SET_COMPARE(htim, Channel, 0);
}

void airConditioner_heat(int type)
{
	printf("airConditioner 24, type = %d\n", type);
    if (type < DC_MOTOR_LOW || type > DC_MOTOR_HIGH) {
			printf("airConditioner 26\n");
        return;
    }
		printf("airConditioner 29\n");
    STM32_PWM_Stop(&htim3, TIM_CHANNEL_1);
    STM32_PWM_Stop(&htim3, TIM_CHANNEL_2);
//    HAL_Delay(10);  // 确保 PWM 停止
		printf("airConditioner 33\n");
    printf("heat\n,speed : %d\n", type);
    STM32_PWM_Start(&htim3, TIM_CHANNEL_2, speed_array[type]); // CH2 运行
    STM32_PWM_Start(&htim3, TIM_CHANNEL_1, 0);
}

void airConditioner_cool(int type)
{
    if (type < DC_MOTOR_LOW || type > DC_MOTOR_HIGH) {
        return;
    }
    STM32_PWM_Stop(&htim3, TIM_CHANNEL_1);
    STM32_PWM_Stop(&htim3, TIM_CHANNEL_2);
//    HAL_Delay(10);  // 确保 PWM 停止

    printf("cool,speed : %d\n", type);
    STM32_PWM_Start(&htim3, TIM_CHANNEL_1, speed_array[type]); // CH1 运行
    STM32_PWM_Start(&htim3, TIM_CHANNEL_2, 0);
}

void airConditioner_stop(void)
{
    printf("stop\n");
    STM32_PWM_Stop(&htim3, TIM_CHANNEL_1);
    STM32_PWM_Stop(&htim3, TIM_CHANNEL_2);
//    HAL_Delay(10);  // 确保 PWM 停止
}


uint8_t cur_state;

void airConditioner_work(uint8_t airConditioner_state)
{
    cur_state = airConditioner_state;
    int func_type = airConditioner_state & 0x1;
    int speed_type = (airConditioner_state >> 1) & 0x3;

    printf("func_type = %d, speed_type = %d\n", func_type, speed_type);
    if (speed_type == 0 || speed_type > 3) {
        airConditioner_stop();
        printf("airConditioner stop\n");
        return;
    }

    if (func_type == 0) {
        airConditioner_heat(speed_type - 1);
        printf("airConditioner heat, type = %d\n", speed_type);
    } else {
        airConditioner_cool(speed_type - 1);
        printf("airConditioner cool, type = %d\n", speed_type);
    }
}

uint8_t airConditioner_getState(void)
{
    return cur_state;
}
