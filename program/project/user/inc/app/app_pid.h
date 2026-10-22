#ifndef _app_pid_h_
#define _app_pid_h_

#include "zf_common_headfile.h"

extern float Kp, Ki, Kd;
extern uint8_t car_running; // ★ 发车标志位必须加 extern 声明
extern float err;

void app_pid_init(void);
void app_pid_control_loop(void);

#endif