/*********************************************************************************************************************
* 文件名称          app_main.h
* 功能简介          应用主流程（app 层）：页面管理 + 主循环（任务11 的"不重新烧录"核心）
* 说明              main() 里只做两件事：
*                       1. app_main_init()  初始化 BSP 和应用
*                       2. while(1) app_main_loop()
*                   所有任务页面都挂在 app_page_table 上，通过五向开关切换，
*                   因此全部考核任务可以在同一次烧录中完成。
********************************************************************************************************************/
#ifndef _app_main_h_
#define _app_main_h_

#include "bsp_board.h"
#include "bsp_key.h"

// 1. 页面编号枚举
typedef enum
{
    APP_PAGE_MAIN = 0,  // 主菜单
    APP_PAGE_RUN,       // 运行页面
    APP_PAGE_MENU,      // PID调参菜单
    
    APP_PAGE_NUMBER     // 总页面数
} app_page_enum;

// 2. 页面控制结构体
typedef struct
{
    const char *name;
    void (*init)(void);
    void (*draw)(void);
    void (*update)(void);
    void (*key)(bsp_key_enum key, bsp_key_state_enum state);
} app_page_struct;

// 3. 全局函数声明
void app_main_init(void);
void app_main_loop(void);
void app_main_goto_page(app_page_enum page);
void app_main_goto_main(void);
app_page_enum app_main_get_page(void);

#endif