/*********************************************************************************************************************
* 文件名称          app_main.c
* 功能简介          智能车主流程
********************************************************************************************************************/
#include "app_main.h"
#include "app_ui.h"
#include "app_task.h"
#include "app_menu.h"
#include "app_param.h"
#include "bsp_system.h"
#include "bsp_pit.h"
#include "bsp_lcd.h"
// ★ 删除了 LED 和 PWM(蜂鸣器) 的包含
#include "bsp_motor.h"     // ★ 新增电机
#include "bsp_grayscale.h" // ★ 新增灰度
#include "app_pid.h"       // ★ 新增 PID

// 页面表
static const app_page_struct *app_page_table[APP_PAGE_NUMBER] = {
    /* APP_PAGE_MAIN  */ 0, 
    /* APP_PAGE_RUN   */ &app_task_page_run,     // 运行页面
    /* APP_PAGE_MENU  */ &app_menu_page,         // PID调参菜单
};

static const char *app_main_menu_text[APP_PAGE_NUMBER - 1] = {
    "T1 RUN CAR",
    "T2 PID MENU",
};

static app_page_enum app_main_page       = APP_PAGE_MAIN;
static uint8         app_main_menu_sel   = 0;
static uint32        app_main_last_refresh = 0;
static uint32        app_main_ui_period  = 100; // ★ 固定为 100ms 刷新一次

//-------------------------------------------------------------------------------------------------------------------
// 主菜单页面：整页重绘
//-------------------------------------------------------------------------------------------------------------------
static void app_main_menu_draw (void)
{
    uint8 i;

    app_ui_title("  SMART CAR");
    app_ui_clear_body();

    for(i = 0; i < (APP_PAGE_NUMBER - 1); i++)
    {
        app_ui_menu_item((uint16)(BSP_LCD_FIRST_ITEM_ROW + i), app_main_menu_text[i], 0,
                         (i == app_main_menu_sel) ? (1) : (0));
    }

    app_ui_status("U/D:SEL M:ENTER");
}

//-------------------------------------------------------------------------------------------------------------------
// 主菜单页面：按键处理
//-------------------------------------------------------------------------------------------------------------------
static void app_main_menu_key (bsp_key_enum key, bsp_key_state_enum state)
{
    if(BSP_KEY_STATE_RELEASE == state) return;

    switch(key)
    {
        case BSP_KEY_UP:
        {
            if(app_main_menu_sel > 0) app_main_menu_sel--;
            app_main_menu_draw();
            break;
        }
        case BSP_KEY_DOWN:
        {
            if(app_main_menu_sel < (APP_PAGE_NUMBER - 2)) app_main_menu_sel++;
            app_main_menu_draw();
            break;
        }
        case BSP_KEY_MID:
        {
            app_main_goto_page((app_page_enum)(app_main_menu_sel + 1));
            break;
        }
        default: break;
    }
}

//-------------------------------------------------------------------------------------------------------------------
// 应用初始化
//-------------------------------------------------------------------------------------------------------------------
void app_main_init (void)
{
    // 1. BSP 初始化 (里面包含时钟、屏幕、按键、定时器等)
    bsp_system_init();                                                          

    // 2. 智能车专属硬件初始化
    bsp_motor_init();       // 初始化电机 PWM 引脚
    bsp_grayscale_init();   // 初始化灰度探头引脚

    // 3. 应用层初始化
    app_param_init();       // 从 Flash 读取 PID 参数
    app_pid_init();         // 将读取到的参数应用到 PID 控制器中
    app_ui_init();          // 屏幕 UI 初始化

    // 4. 进入主菜单
    app_main_goto_page(APP_PAGE_MAIN);                                          
}

//-------------------------------------------------------------------------------------------------------------------
// 应用主循环
//-------------------------------------------------------------------------------------------------------------------
void app_main_loop (void)
{
    bsp_key_enum       key = BSP_KEY_NONE;
    bsp_key_state_enum state;

    // 1. 处理全部按键事件
    while(BSP_KEY_STATE_RELEASE != (state = bsp_key_get_event(&key)))
    {
        if(APP_PAGE_MAIN == app_main_page)
        {
            app_main_menu_key(key, state);
        }
        else if(0 != app_page_table[app_main_page]->key)
        {
            app_page_table[app_main_page]->key(key, state);
        }
    }

    // 2. 周期刷新当前页面
    if(bsp_pit_check_interval(&app_main_last_refresh, app_main_ui_period))
    {
        if(APP_PAGE_MAIN != app_main_page)
        {
            if(0 != app_page_table[app_main_page]->update)
            {
                app_page_table[app_main_page]->update();
            }
        }
    }
}

//-------------------------------------------------------------------------------------------------------------------
// 切换到指定页面
//-------------------------------------------------------------------------------------------------------------------
void app_main_goto_page (app_page_enum page)
{
    if(page >= APP_PAGE_NUMBER) return;

    app_main_page = page;
    app_main_last_refresh = bsp_pit_get_tick_ms();

    if(APP_PAGE_MAIN == page)
    {
        app_main_menu_draw();
        return;
    }

    if(0 != app_page_table[page])
    {
        if(0 != app_page_table[page]->init)
        {
            app_page_table[page]->init();                                       
        }
        if(0 != app_page_table[page]->draw)
        {
            app_page_table[page]->draw();                                       
        }
    }
}

void app_main_goto_main (void)
{
    app_main_goto_page(APP_PAGE_MAIN);
}

app_page_enum app_main_get_page (void)
{
    return app_main_page;
}
