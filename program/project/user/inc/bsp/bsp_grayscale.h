#ifndef _bsp_grayscale_h_
#define _bsp_grayscale_h_

#include "zf_common_headfile.h" // 引入逐飞库基础头文件

void bsp_grayscale_init(void);
float bsp_grayscale_get_error(void); // ★ 必须有这一句，app_pid.c 才能认识它
#define GRAY_1_PIN          (B0)
#define GRAY_2_PIN          (B1)
#define GRAY_3_PIN          (B2)
#define GRAY_4_PIN          (B3)
#define GRAY_5_PIN          (B4)

#endif