#include "app_pid.h"
#include "bsp_motor.h"
#include "bsp_grayscale.h"
#include "app_param.h"

float Kp = 15.0f, Ki = 0.0f, Kd = 10.0f;
float err = 0, last_err = 0, integral = 0;
uint8_t car_running = 0; // 发车标志位

void app_pid_init(void) {
    // 初始化时从参数表读取一次 PID
    // 注意：参数表里的值在 app_param.c 里，读出来是 int32
    Kp = (float)app_param_get_item(0, 0)->value;
    Ki = (float)app_param_get_item(0, 1)->value;
    Kd = (float)app_param_get_item(0, 2)->value;
}

// 这个函数将被放入 5ms 的定时器中断中
void app_pid_control_loop(void) {
    // 1. 停车保护
    if(!car_running) {
        bsp_motor_set_speed(0, 0);
        return;
    }

    // 2. 获取偏差
    err = bsp_grayscale_get_error();
    
    // 3. 丢线保护 (如果探头全白，99.0)
    if(err >= 99.0f) {
        err = last_err; // 惯性保持，按上一次的偏差打角
    }

    // 4. PID 计算
    integral += err;
    if(integral > 500) integral = 500;   // 积分限幅
    if(integral < -500) integral = -500;

    float steer = (Kp * err) + (Ki * integral) + (Kd * (err - last_err));
    last_err = err;

    // 5. 差速融合
    int base_spd = 40; // 基础速度，可以从 app_param 动态获取
    float left_pwm = base_spd + steer;
    float right_pwm = base_spd - steer;

    // 6. ★ 极其重要的限幅保护：正负都要限制在 -100 到 100 之间
    // 否则如果转急弯，左侧可能算出 150，右侧算出 -50，会导致电机驱动失控
    if(left_pwm > 100) left_pwm = 100;
    if(left_pwm < -100) left_pwm = -100;
    
    if(right_pwm > 100) right_pwm = 100;
    if(right_pwm < -100) right_pwm = -100;
    
    // 7. 输出给底层电机
    bsp_motor_set_speed(left_pwm, right_pwm);
}