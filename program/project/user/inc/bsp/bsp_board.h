#ifndef _bsp_board_h_
#define _bsp_board_h_

#include "zf_common_headfile.h"

/* ========================================= 一、系统级参数 ========================================= */
#define BSP_SYSTEM_CLOCK                (SYSTEM_CLOCK_120M)                     // 芯片主频 120MHz
#define BSP_DEBUG_ENABLE                (1)                                     // 是否初始化调试串口(USART3 PB10/PB11)

/* ========================================= 二、按键（五向开关 + 编码器） ========================================= */
#define BSP_KEY_A0_PIN                  (D8)                                    // KEY_IN1_D8   编码最低位
#define BSP_KEY_A1_PIN                  (B12)                                   // KEY_IN2_B12  编码中间位
#define BSP_KEY_A2_PIN                  (B14)                                   // KEY_IN3_B14  编码最高位

#define BSP_KEY_GPIO_MODE               (GPI_PULL_UP)                           // 按键输入模式
#define BSP_KEY_DEBOUNCE_MS             (10)                                    // 消抖时长(ms)
#define BSP_KEY_LONG_PRESS_MS           (800)                                   // 长按判定时长(ms)

#define BSP_KEY_CODE_NONE               (7)                                     // 无按键
#define BSP_KEY_CODE_KEY1               (0)
#define BSP_KEY_CODE_KEY2               (1)
#define BSP_KEY_CODE_MID                (2)                                     // 中(确定)
#define BSP_KEY_CODE_RIGHT              (3)                                     // 右
#define BSP_KEY_CODE_UP                 (4)                                     // 上
#define BSP_KEY_CODE_DOWN               (5)                                     // 下
#define BSP_KEY_CODE_LEFT               (6)                                     // 左

/* ========================================= 三、外部中断 ========================================= */
#define BSP_KEY_EXTI_ENABLE             (1)                                     // 是否启用按键外部中断
#define BSP_KEY_EXTI_TRIGGER            (EXTI_TRIGGER_BOTH)                     // 双边沿触发
#define BSP_KEY_EXTI_PIN_0              (BSP_KEY_A0_PIN)                        
#define BSP_KEY_EXTI_PIN_1              (BSP_KEY_A1_PIN)                        

/* ========================================= 四、定时器中断 PIT ========================================= */
#define BSP_PIT_SYSTEM_INDEX            (TIM6_PIT)                              // 系统节拍定时器
#define BSP_PIT_SYSTEM_TIM              (TIM6)                                  
#define BSP_PIT_SYSTEM_MS               (1)                                     // 系统节拍周期 1ms

// ★ 修改：将原来的 LED 定时中断改为 5ms 控制环中断
#define BSP_PIT_CONTROL_INDEX           (TIM7_PIT)                              // 控制环定时器 (原TIM7)
#define BSP_PIT_CONTROL_TIM             (TIM7)                                  
#define BSP_PIT_CONTROL_MS              (5)                                     // 控制环周期 5ms

#define BSP_UI_REFRESH_MS               (100)                                   // UI 刷新周期(ms)

/* ========================================= 五、SPI 屏幕 ========================================= */
#define BSP_LCD_W                       (128)                                   
#define BSP_LCD_H                       (160)                                   
#define BSP_LCD_DIR                     (TFT180_PORTAIT)                        
#define BSP_LCD_FONT                    (TFT180_8X16_FONT)                      

#define BSP_LCD_TITLE_ROW               (0)                                     
#define BSP_LCD_FIRST_ITEM_ROW          (1)                                     
#define BSP_LCD_MAX_ITEM_ROW            (8)                                     
#define BSP_LCD_STATUS_ROW              (9)                                     

#define BSP_LCD_CH_W                    (8)                                     
#define BSP_LCD_CH_H                    (16)                                    
#define BSP_LCD_COLS                    (BSP_LCD_W / BSP_LCD_CH_W)              
#define BSP_LCD_ROWS                    (BSP_LCD_H / BSP_LCD_CH_H)              

/* ========================================= 六、Flash 数据存储 ========================================= */
#define BSP_FLASH_SAVE_SECTOR           (63)                                    
#define BSP_FLASH_SAVE_PAGE             (0)                                     
#define BSP_FLASH_MAGIC                 (0x5A5AA5A5)                            
#define BSP_FLASH_DATA_LEN              (32)                                    

/* ========================================= 七、智能车专属：电机与灰度 ========================================= */
// 电机引脚定义 (请根据你的实际接线核对)
#define MOTOR_L_PWM_F       (TIM2_PWM_MAP0_CH1_A0) // 左电机正转 PWM
#define MOTOR_L_PWM_B       (TIM2_PWM_MAP0_CH2_A1) // 左电机反转 PWM
#define MOTOR_L_DIR_PIN     (A2)                   // 左电机方向控制
#define MOTOR_R_PWM_F       (TIM3_PWM_MAP0_CH1_A6) // 右电机正转 PWM
#define MOTOR_R_PWM_B       (TIM3_PWM_MAP0_CH2_A7) // 右电机反转 PWM
#define MOTOR_R_DIR_PIN     (A3)                   // 右电机方向控制

// 灰度探头引脚
#define GRAY_1_PIN          (B0)
#define GRAY_2_PIN          (B1)
#define GRAY_3_PIN          (B2)
#define GRAY_4_PIN          (B3)
#define GRAY_5_PIN          (B4)

#endif