#include "step_motor.h"

void step_motor_run(uint8_t step, uint8_t dir, uint8_t speed, uint16_t angle, uint8_t sta)
{
    if (sta == 1) {
        uint16_t total_steps = (64 * angle) / 45; //下面会乘8
        for (uint16_t j = 0; j < total_steps; j++) {
            for (uint8_t i = 0; i < 8; i += (8 / step)) {
                uint8_t index = dir == 0 ? i : (8 - i) % 8; // 方向选择
                switch (index) {
                    case 0: 
											MOTOR_IN1(1); 
											MOTOR_IN2(0); 
											MOTOR_IN3(0); 
											MOTOR_IN4(0); 
										break;
                    case 1: 
											MOTOR_IN1(1); 
											MOTOR_IN2(1); 
											MOTOR_IN3(0); 
											MOTOR_IN4(0); 
											break;
                    case 2: 
											MOTOR_IN1(0); 
											MOTOR_IN2(1); 
											MOTOR_IN3(0); 
											MOTOR_IN4(0); 
											break;
                    case 3: 
											MOTOR_IN1(0); 
											MOTOR_IN2(1);
											MOTOR_IN3(1);
											MOTOR_IN4(0);
											break;
                    case 4: 
											MOTOR_IN1(0);
											MOTOR_IN2(0); 
											MOTOR_IN3(1); 
											MOTOR_IN4(0); 
											break;
                    case 5:
											MOTOR_IN1(0);
											MOTOR_IN2(0); 
											MOTOR_IN3(1); 
											MOTOR_IN4(1); 
											break;
                    case 6: 
											MOTOR_IN1(0); 
											MOTOR_IN2(0); 
											MOTOR_IN3(0); 
											MOTOR_IN4(1); 
											break;
                    case 7: 
											MOTOR_IN1(1); 
											MOTOR_IN2(0); 
											MOTOR_IN3(0); 
											MOTOR_IN4(1); 
											break;
                }
				//驱动要求大于1.8ms = 1800us 但是实测延时1000us也可以
                HAL_Delay(speed);
            }
        }
    } else {
        MOTOR_IN1(0); 
				MOTOR_IN2(0); 
				MOTOR_IN3(0); 
				MOTOR_IN4(0);
    }
}

void curtain_open(void)
{
	step_motor_run(STEP_MOTOR_BYTE, STEP_MOTOR_DIR_CLOCKWISE, STEP_MOTOR_SPEEP, STEP_MOTOR_ANGLE, STEP_MOTOR_START);
	step_motor_run(STEP_MOTOR_BYTE, STEP_MOTOR_DIR_CLOCKWISE, STEP_MOTOR_SPEEP, STEP_MOTOR_ANGLE, STEP_MOTOR_STOP);
}

uint16_t cur_angle;
void curtain_open_angle(uint16_t angle)
{
  cur_angle = angle;
	step_motor_run(STEP_MOTOR_BYTE, STEP_MOTOR_DIR_CLOCKWISE, STEP_MOTOR_SPEEP, angle, STEP_MOTOR_START);
	step_motor_run(STEP_MOTOR_BYTE, STEP_MOTOR_DIR_CLOCKWISE, STEP_MOTOR_SPEEP, angle, STEP_MOTOR_STOP);
}

void curtain_close_angle(uint16_t angle)
{
  cur_angle = angle;
	step_motor_run(STEP_MOTOR_BYTE, STEP_MOTOR_DIR_UNCLOCKWISE, STEP_MOTOR_SPEEP, angle, STEP_MOTOR_START);
	step_motor_run(STEP_MOTOR_BYTE, STEP_MOTOR_DIR_UNCLOCKWISE, STEP_MOTOR_SPEEP, angle, STEP_MOTOR_STOP);
}

uint16_t curtain_get_curangle(void)
{
    return cur_angle;
}