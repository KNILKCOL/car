/*********************************************************************************************************************
* 文件名称          bsp_board.h
* 功能简介          板级硬件资源总配置文件（BSP 层）
* 硬件平台          WCH 主板 _4  (原理图: SCH_WCH主板_4_2026-10-10.pdf)
* 主控芯片          CH32V307VCT6
*
* 说明：
*   本文件是全部硬件相关的 **宏定义** 唯一来源：引脚、定时器、中断、外设参数等。
*   遵循考核要求：
*     1. 所有涉及硬件初始化的代码统一放在 bsp 层；
*     2. 能用宏定义的地方全部使用宏定义（引脚 / 定时器 / 中断 / 周期 / 阈值）。
*   任何换板、改线、改频率，只需要修改本文件即可，app 层代码无需改动。
********************************************************************************************************************/

#ifndef _bsp_board_h_
#define _bsp_board_h_

#include "zf_common_headfile.h"

/*********************************************************************************************************************
*                                            一、系统级参数
********************************************************************************************************************/
#define BSP_SYSTEM_CLOCK                (SYSTEM_CLOCK_120M)                     // 芯片主频 120MHz
#define BSP_DEBUG_ENABLE                (1)                                     // 是否初始化调试串口(USART3 PB10/PB11)

/*********************************************************************************************************************
*                                            二、LED 指示灯
* 原理图网络: LIGHT1_A11 / LIGHT2_A12 / LIGHT3_A15 / LIGHT4_C10 / LIGHT5_D1
* 硬件连接  : MCU 引脚 -> 限流电阻(RN3/RN4) -> LED 阳极 -> GND
*             => 引脚输出高电平时 LED 点亮（如实际相反，只需把下面两个宏对调）
********************************************************************************************************************/
#define BSP_LED_ON_LEVEL                (GPIO_HIGH)                             // 点亮电平
#define BSP_LED_OFF_LEVEL               (GPIO_LOW)                              // 熄灭电平
#define BSP_LED_NUM                     (5)                                     // LED 数量

#define BSP_LED1_PIN                    (A11)                                   // LIGHT1_A11  右侧 LED（任务3使用）
#define BSP_LED2_PIN                    (A12)                                   // LIGHT2_A12
#define BSP_LED3_PIN                    (A15)                                   // LIGHT3_A15
#define BSP_LED4_PIN                    (D1)                                   // LIGHT4_C10
#define BSP_LED5_PIN                    (C10)                                    // LIGHT5_D1

#define BSP_LED_RIGHT_INDEX             (1)                                     // 任务3 "右侧 LED" 在 LED 组中的编号
#define BSP_LED_DEFAULT_INDEX           (1)                                     // 默认演示 LED 编号（任务6 定时中断默认翻转的 LED）

/*********************************************************************************************************************
*                                            三、按键（五向开关 + 编码器）
* 原理图: 五向开关 U/D/L/R/M + KEY1/KEY2 接到 SN74HC148(8-3 优先编码器) U15，
*         编码输出 A2/A1/A0 分别接到 KEY_IN3_B14 / KEY_IN2_B12 / KEY_IN1_D8。
*         74HC148 输出为低有效编码，无按键按下时 A2A1A0 = 111。
*         I0 输入(编码 111)与"无按键"状态相同，原理图标注为 USELESS，故不使用。
*
*   编码值 = (A2<<2) | (A1<<1) | A0      按下 U15 的哪个输入  功能
*      0        000                         I7                  KEY1
*      1        001                         I6                  KEY2
*      2        010                         I5                  中键 M
*      3        011                         I4                  右键 R
*      4        100                         I3                  上键 U
*      5        101                         I2                  下键 D
*      6        110                         I1                  左键 L
*      7        111                         无 / I0(USELESS)    无按键
********************************************************************************************************************/
#define BSP_KEY_A0_PIN                  (D8)                                    // KEY_IN1_D8   编码最低位
#define BSP_KEY_A1_PIN                  (B12)                                   // KEY_IN2_B12  编码中间位
#define BSP_KEY_A2_PIN                  (B14)                                   // KEY_IN3_B14  编码最高位

#define BSP_KEY_GPIO_MODE               (GPI_PULL_UP)                           // 按键输入模式（原理图已有上拉，此处再开内部上拉增强抗扰）

#define BSP_KEY_DEBOUNCE_MS             (10)                                    // 消抖时长(ms)   —— 任务10 可调参数
#define BSP_KEY_LONG_PRESS_MS           (800)                                   // 长按判定时长(ms) —— 任务10 可调参数

#define BSP_KEY_CODE_NONE               (7)                                     // 无按键
#define BSP_KEY_CODE_KEY1               (0)
#define BSP_KEY_CODE_KEY2               (1)
#define BSP_KEY_CODE_MID                (2)                                     // 中(确定)
#define BSP_KEY_CODE_RIGHT              (3)                                     // 右
#define BSP_KEY_CODE_UP                 (4)                                     // 上
#define BSP_KEY_CODE_DOWN               (5)                                     // 下
#define BSP_KEY_CODE_LEFT               (6)                                     // 左

/*********************************************************************************************************************
*                                            四、外部中断（任务9）
* 说明: 编码器的 A0/A1 两个输出变化即可覆盖所有按键组合，因此使用 2 路线外部中断做事件通知，
*       主循环再用 GPIO 扫描做消抖确认（EXTI + 轮询双重保障）。
*       注: A2(B14) 的 EXTI_Line14 已被开源库中的 dm1xa 回调占用，故不在此处重复注册。
********************************************************************************************************************/
#define BSP_KEY_EXTI_ENABLE             (1)                                     // 是否启用按键外部中断
#define BSP_KEY_EXTI_TRIGGER            (EXTI_TRIGGER_BOTH)                     // 双边沿触发
#define BSP_KEY_EXTI_PIN_0              (BSP_KEY_A0_PIN)                        // D8  -> EXTI_Line8  (EXTI9_5)
#define BSP_KEY_EXTI_PIN_1              (BSP_KEY_A1_PIN)                        // B12 -> EXTI_Line12 (EXTI15_10)

/*********************************************************************************************************************
*                                            五、无源蜂鸣器 与 PWM（任务5）
* 原理图: FMQ_IO_A8 -> 20Ω -> AO3400A 栅极；蜂鸣器 U2 谐振频率 2.731kHz。
*         => PA8 = TIM1_CH1，高电平驱动 MOS 导通，无源蜂鸣器必须用方波(PWM)驱动。
********************************************************************************************************************/
#define BSP_BUZZER_PWM_CHANNEL          (TIM1_PWM_MAP0_CH1_A8)                   // FMQ_IO_A8 -> TIM1_CH1
#define BSP_BUZZER_FREQ_HZ              (2731)                                  // 无源蜂鸣器谐振频率(原理图标注 2.731kHz)
#define BSP_BUZZER_DUTY_MAX             (PWM_DUTY_MAX)                          // 占空比满量程(10000)
#define BSP_BUZZER_DUTY_ON              (PWM_DUTY_MAX / 2)                      // 50% 占空比时声音最响
#define BSP_BUZZER_DUTY_OFF             (0)                                     // 占空比 0 即停止发声

#define BSP_PWM_DEMO_ENABLE             (1)                                     // 是否初始化第二路 PWM 演示输出
#define BSP_PWM_DEMO_CHANNEL            (TIM3_PWM_MAP3_CH1_C6)                   // 任务5 演示"主控板输出 PWM 波"的引脚: 舵机接口 DUOJI_PWM_C6
#define BSP_PWM_DEMO_FREQ_HZ            (1000)                                  // 演示 PWM 频率 1kHz
#define BSP_PWM_DEMO_DUTY               (PWM_DUTY_MAX / 2)                      // 演示 PWM 占空比 50%

/*********************************************************************************************************************
*                                            六、定时器中断 PIT（任务6）
* 说明: TIM6 用作 1ms 系统节拍（按键扫描 / UI 刷新 / 计时）；
*       TIM7 用作 LED 闪烁的定时中断，中断内直接翻转 LED（任务6 要求）。
********************************************************************************************************************/
#define BSP_PIT_SYSTEM_INDEX            (TIM6_PIT)                              // 系统节拍定时器
#define BSP_PIT_SYSTEM_TIM              (TIM6)                                  // 与上一行必须保持一致
#define BSP_PIT_SYSTEM_MS               (1)                                     // 节拍周期 1ms

#define BSP_PIT_LED_INDEX               (TIM7_PIT)                              // LED 定时中断定时器
#define BSP_PIT_LED_TIM                 (TIM7)                                  // 与上一行必须保持一致
#define BSP_PIT_LED_MS                  (200)                                   // LED 定时中断周期(ms)

#define BSP_LED_BLINK_PERIOD_MS         (500)                                   // 任务3 LED 闪烁半周期(ms)
#define BSP_UI_REFRESH_MS               (100)                                   // UI 刷新周期(ms)

/*********************************************************************************************************************
*                                            七、SPI 屏幕（任务7）
* 原理图: 1.8寸 TFT  ST7735  128x160
*         SCL=PB13(SPI2_SCK)  SDA=PB15(SPI2_MOSI)  RES=PB7  DC=PD7  CS=PD4  BL=PD0
*         以上引脚与开源库 zf_device_tft180.h 默认配置完全一致，故直接复用该驱动。
********************************************************************************************************************/
#define BSP_LCD_W                       (128)                                   // 屏幕宽度(竖屏)
#define BSP_LCD_H                       (160)                                   // 屏幕高度(竖屏)
#define BSP_LCD_DIR                     (TFT180_PORTAIT)                        // 显示方向
#define BSP_LCD_FONT                    (TFT180_8X16_FONT)                      // 字体(8x16: 每行 16 字符 / 共 10 行)

#define BSP_LCD_TITLE_ROW               (0)                                     // 标题栏所在行
#define BSP_LCD_FIRST_ITEM_ROW          (1)                                     // 菜单首行
#define BSP_LCD_MAX_ITEM_ROW            (8)                                     // 菜单最大行
#define BSP_LCD_STATUS_ROW              (9)                                     // 状态栏所在行

#define BSP_LCD_CH_W                    (8)                                     // 单个字符宽度(像素)
#define BSP_LCD_CH_H                    (16)                                    // 单个字符高度(像素)
#define BSP_LCD_COLS                    (BSP_LCD_W / BSP_LCD_CH_W)              // 每行字符数 = 16
#define BSP_LCD_ROWS                    (BSP_LCD_H / BSP_LCD_CH_H)              // 总行数     = 10

/*********************************************************************************************************************
*                                            八、Flash 数据存储（任务8）
* 芯片 256K Flash / 64 个扇区，每扇区 4K，每扇区 4 页，每页 1K。
* 用户参数放在最后一个扇区(63)，不会与程序区冲突。
********************************************************************************************************************/
#define BSP_FLASH_SAVE_SECTOR           (63)                                    // 参数保存扇区
#define BSP_FLASH_SAVE_PAGE             (0)                                     // 参数保存页
#define BSP_FLASH_MAGIC                 (0x5A5AA5A5)                            // 数据有效标志
#define BSP_FLASH_DATA_LEN              (32)                                    // 参与存储的 32bit 数据个数(<=256)

#endif
