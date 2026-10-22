#ifndef _bsp_motor_h_
#define _bsp_motor_h_

#include "zf_common_headfile.h"

void bsp_motor_init(void);
void bsp_motor_set_pwm_freq(uint32 freq_hz);
void bsp_motor_set_speed(float left_speed, float right_speed); // ★ 必须有这一句
// ================= 电机引脚定义 =================
#define MOTOR_L_PWM_F       (TIM2_PWM_MAP0_CH1_A0) // 左电机正转 PWM
#define MOTOR_L_PWM_B       (TIM2_PWM_MAP0_CH2_A1) // 左电机反转 PWM
#define MOTOR_L_DIR_PIN     (A2)                   // 左电机方向控制

#define MOTOR_R_PWM_F       (TIM3_PWM_MAP0_CH1_A6) // 右电机正转 PWM
#define MOTOR_R_PWM_B       (TIM3_PWM_MAP0_CH2_A7) // 右电机反转 PWM
#define MOTOR_R_DIR_PIN     (A3)                   // 右电机方向控制

// ================= 灰度探头引脚定义 =================
#define GRAY_1_PIN          (B0)
#define GRAY_2_PIN          (B1)
#define GRAY_3_PIN          (B2)
#define GRAY_4_PIN          (B3)
#define GRAY_5_PIN          (B4)

#endif