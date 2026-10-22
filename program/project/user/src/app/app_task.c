/*********************************************************************************************************************
* 文件名称          app_task.c
* 功能简介          各考核任务的演示页面实现（app 层）
********************************************************************************************************************/
#include "app_task.h"
#include "app_ui.h"
#include "app_main.h"
#include "app_param.h"
#include "app_pid.h"
#include "bsp_motor.h"

#define PAGE_RUN_NAME "CAR RUNNING"

static void task_run_init(void) {
    car_running = 0;
    bsp_motor_set_speed(0, 0);
}

static void task_run_draw(void) {
    app_ui_title(PAGE_RUN_NAME);
    app_ui_clear_body();
    app_ui_text(1, "STATE: STOPPED");
    app_ui_text(2, "ERR: 0.00");
    app_ui_text(3, "BASE SPEED: 40");
    app_ui_text(5, "M: START / K1: STOP");
    app_ui_status("U/D for Menu");
}

static void task_run_update(void) {
    char buf[20];
    sprintf(buf, "ERR: %.2f", err);
    app_ui_text(2, buf); // 实时显示偏差
}

static void task_run_key(bsp_key_enum key, bsp_key_state_enum state) {
    if(state == BSP_KEY_STATE_RELEASE) return;
    
    if(key == BSP_KEY_MID) {
        car_running = 1;
        app_ui_text(1, "STATE: RUNNING");
    } else if(key == BSP_KEY_KEY1) {
        car_running = 0;
        bsp_motor_set_speed(0, 0);
        app_ui_text(1, "STATE: STOPPED");
        app_main_goto_main(); // 返回主菜单
    }
}

// 页面注册
const app_page_struct app_task_page_run = {
    PAGE_RUN_NAME, task_run_init, task_run_draw, task_run_update, task_run_key
};