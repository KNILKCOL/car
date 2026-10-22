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

#include "app_main.h"  // 依赖 main 里的结构体

// 告诉编译器有这个页面，且内容不可修改（const）
extern const app_page_struct app_task_page_run;

#endif