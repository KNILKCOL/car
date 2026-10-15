/*********************************************************************************************************************
* 文件名称          app_task.h
* 功能简介          各考核任务的演示页面（app 层）
*
* 任务对应关系：
*   任务3  GPIO 控制右侧 LED          -> app_page_led
*   任务4  正确读取按键状态            -> app_page_key
*   任务5  PWM 输出 + 无源蜂鸣器       -> app_page_pwm
*   任务6  定时器中断控制 LED          -> app_page_timer
*   任务7  SPI 驱动屏幕                -> app_page_lcd
*   任务8  Flash 保存并读取数据        -> app_page_flash
*   任务9  外部中断读按键 + 屏幕 UI    -> app_page_exti
*   任务10 三级菜单记录参数            -> app_menu_page (见 app_menu.h)
*   任务11 以上全部任务不重新烧录即可完成 -> 主菜单页面(app_main.c) + 本文件各页面
********************************************************************************************************************/
#ifndef _app_task_h_
#define _app_task_h_

#include "bsp_board.h"
#include "bsp_key.h"

// 页面接口：每个任务页面都实现这 4 个函数
typedef struct
{
    const char *name;                                                           // 页面标题（显示在标题栏，也是主菜单条目名）

    void (*init)   (void);                                                      // 进入页面时调用一次
    void (*draw)   (void);                                                      // 整页重绘
    void (*update) (void);                                                      // 周期刷新（动态数据）
    void (*key)    (bsp_key_enum key, bsp_key_state_enum state);                // 按键事件处理
}app_page_struct;

//-------------------------------------------------------------------------------------------------------------------
// 各任务页面对象（供 app_main.c 组装页面表）
//-------------------------------------------------------------------------------------------------------------------
extern const app_page_struct app_task_page_led;                                 // 任务3
extern const app_page_struct app_task_page_key;                                 // 任务4
extern const app_page_struct app_task_page_pwm;                                 // 任务5
extern const app_page_struct app_task_page_timer;                               // 任务6
extern const app_page_struct app_task_page_lcd;                                 // 任务7
extern const app_page_struct app_task_page_flash;                               // 任务8
extern const app_page_struct app_task_page_exti;                                // 任务9

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     任务演示公共初始化（进入任务总菜单前调用一次）
// 返回参数     void
//-------------------------------------------------------------------------------------------------------------------
void app_task_common_init (void);

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     蜂鸣器提示音（受 SYSTEM/BUZZER 参数控制，参数为 0 时静音）
// 参数说明     ms           持续时间(ms)
// 返回参数     void
//-------------------------------------------------------------------------------------------------------------------
void app_task_beep        (uint32 ms);

#endif
