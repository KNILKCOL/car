#include "bsp_motor.h"

void bsp_motor_init(void) {
    pwm_init(MOTOR_L_PWM_F, 10000, 0); // Ä¬ÈÏ10kHz
    pwm_init(MOTOR_L_PWM_B, 10000, 0);
    pwm_init(MOTOR_R_PWM_F, 10000, 0);
    pwm_init(MOTOR_R_PWM_B, 10000, 0);
    
    gpio_init(MOTOR_L_DIR_PIN, GPO, GPIO_LOW, GPO_PUSH_PULL);
    gpio_init(MOTOR_R_DIR_PIN, GPO, GPIO_LOW, GPO_PUSH_PULL);
}

void bsp_motor_set_pwm_freq(uint32 freq_hz) {
    pwm_set_freq(MOTOR_L_PWM_F, freq_hz, 0);
    pwm_set_freq(MOTOR_L_PWM_B, freq_hz, 0);
    pwm_set_freq(MOTOR_R_PWM_F, freq_hz, 0);
    pwm_set_freq(MOTOR_R_PWM_B, freq_hz, 0);
}

void bsp_motor_set_speed(float left, float right) {
    // ---------------- ×óÂÖÂß¼­ ----------------
    if(left >= 0) {
        gpio_set_level(MOTOR_L_DIR_PIN, GPIO_HIGH);
        pwm_set_duty(MOTOR_L_PWM_F, (uint32)(left * (PWM_DUTY_MAX / 100.0f)));
        pwm_set_duty(MOTOR_L_PWM_B, 0);
    } else {
        gpio_set_level(MOTOR_L_DIR_PIN, GPIO_LOW);
        pwm_set_duty(MOTOR_L_PWM_F, 0);
        pwm_set_duty(MOTOR_L_PWM_B, (uint32)(-left * (PWM_DUTY_MAX / 100.0f)));
    }
    
    // ---------------- ÓÒÂÖÂß¼­ ----------------
    if(right >= 0) {
        gpio_set_level(MOTOR_R_DIR_PIN, GPIO_HIGH);
        pwm_set_duty(MOTOR_R_PWM_F, (uint32)(right * (PWM_DUTY_MAX / 100.0f)));
        pwm_set_duty(MOTOR_R_PWM_B, 0);
    } else {
        gpio_set_level(MOTOR_R_DIR_PIN, GPIO_LOW);
        pwm_set_duty(MOTOR_R_PWM_F, 0);
        pwm_set_duty(MOTOR_R_PWM_B, (uint32)(-right * (PWM_DUTY_MAX / 100.0f)));
    }
}