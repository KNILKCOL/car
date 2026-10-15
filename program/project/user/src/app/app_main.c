/*********************************************************************************************************************
* 文件名称          app_main.c
* 功能简介          应用主流程（app 层）
*
* 任务11 的实现思路：
*   把 任务3 ~ 任务10 各自做成一个"页面"，统一挂在 app_page_table 里，
*   主菜单页面用五向开关选择要演示的任务。
*   这样只要烧录一次，所有考核任务都能现场演示，不需要重新烧录程序。
********************************************************************************************************************/
#include "app_main.h"
#include "app_ui.h"
#include "app_task.h"
#include "app_menu.h"
#include "app_param.h"
#include "bsp_system.h"
#include "bsp_led.h"
#include "bsp_pit.h"
#include "bsp_lcd.h"
#include "bsp_pwm.h"

// 页面表：第 0 项是主菜单，其余对应任务3 ~ 任务10
static const app_page_struct *app_page_table[APP_PAGE_NUMBER] =
{
    /* APP_PAGE_MAIN  */ 0,                                                      // 主菜单在 app_main.c 内部实现
    /* APP_PAGE_LED   */ &app_task_page_led,
    /* APP_PAGE_KEY   */ &app_task_page_key,
    /* APP_PAGE_PWM   */ &app_task_page_pwm,
    /* APP_PAGE_TIMER */ &app_task_page_timer,
    /* APP_PAGE_LCD   */ &app_task_page_lcd,
    /* APP_PAGE_FLASH */ &app_task_page_flash,
    /* APP_PAGE_EXTI  */ &app_task_page_exti,
    /* APP_PAGE_MENU  */ &app_menu_page,
};

// 主菜单条目文字（与页面表一一对应，最长 14 个字符以免被截断）
static const char *app_main_menu_text[APP_PAGE_NUMBER - 1] =
{
    "T3 LED GPIO",
    "T4 KEY READ",
    "T5 PWM BUZZER",
    "T6 TIMER INT",
    "T7 SPI LCD",
    "T8 FLASH SAVE",
    "T9 EXTI UI",
    "T10 3-LEVEL",
};

static app_page_enum app_main_page       = APP_PAGE_MAIN;                       // 当前页面
static uint8         app_main_menu_sel   = 0;                                   // 主菜单光标
static uint32        app_main_last_refresh = 0;                                 // 页面刷新计时
static uint32        app_main_ui_period  = BSP_UI_REFRESH_MS;                   // UI 刷新周期(参数可调)

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     主菜单页面：整页重绘
// 返回参数     void
//-------------------------------------------------------------------------------------------------------------------
static void app_main_menu_draw (void)
{
    uint8 i;

    app_ui_title("  CAR TASK MENU");
    app_ui_clear_body();

    for(i = 0; i < (APP_PAGE_NUMBER - 1); i++)
    {
        app_ui_menu_item((uint16)(BSP_LCD_FIRST_ITEM_ROW + i), app_main_menu_text[i], 0,
                         (i == app_main_menu_sel) ? (1) : (0));
    }

    app_ui_status("U/D:SEL M:ENTER");
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     主菜单页面：按键处理
// 返回参数     void
//-------------------------------------------------------------------------------------------------------------------
static void app_main_menu_key (bsp_key_enum key, bsp_key_state_enum state)
{
    if(BSP_KEY_STATE_RELEASE == state)
    {
        return;
    }

    switch(key)
    {
        case BSP_KEY_UP:
        {
            if(app_main_menu_sel > 0)
            {
                app_main_menu_sel--;
            }
            app_task_beep(20);
            app_main_menu_draw();
            break;
        }
        case BSP_KEY_DOWN:
        {
            if(app_main_menu_sel < (APP_PAGE_NUMBER - 2))
            {
                app_main_menu_sel++;
            }
            app_task_beep(20);
            app_main_menu_draw();
            break;
        }
        case BSP_KEY_MID:
        {
            app_task_beep(40);
            app_main_goto_page((app_page_enum)(app_main_menu_sel + 1));         // 进入选中的任务页面
            break;
        }
        case BSP_KEY_KEY2:
        {
            // 主菜单快捷入口：直接进入三级菜单（任务10）
            app_task_beep(40);
            app_main_goto_page(APP_PAGE_MENU);
            break;
        }
        default:
        {
            break;
        }
    }
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     应用初始化
// 返回参数     void
//-------------------------------------------------------------------------------------------------------------------
void app_main_init (void)
{
    //-------------------------------------------------- 1. BSP 初始化 ------------------------------------------------
    bsp_system_init();                                                          // 时钟 + 串口 + LED + 按键 + PWM + 屏幕 + 定时中断

    //-------------------------------------------------- 2. 开机自检 ------------------------------------------------
    // 依次点亮 5 个 LED + 蜂鸣器响一声，用于验证硬件焊接与各外设是否正常
    // （对应考核任务2 "完成阻容焊接，能够正常运行" 的软件验证手段）
    bsp_led_all_on();
    bsp_buzzer_set_freq(BSP_BUZZER_FREQ_HZ);
    bsp_buzzer_beep(100);
    system_delay_ms(200);
    bsp_led_all_off();

    //-------------------------------------------------- 3. 应用初始化 -----------------------------------------------
    app_param_init();                                                           // 从 Flash 读取参数（任务8）
    app_ui_init();                                                              // 屏幕 UI 初始化（任务9）

    app_main_ui_period = (uint32)app_param_get_item(1, 1)->value;               // DISPLAY/REFRESH
    app_task_common_init();
    app_main_goto_page(APP_PAGE_MAIN);                                          // 上电直接进入任务总菜单

    app_task_beep(80);                                                          // 上电提示音
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     应用主循环
// 返回参数     void
//-------------------------------------------------------------------------------------------------------------------
void app_main_loop (void)
{
    bsp_key_enum       key = BSP_KEY_NONE;
    bsp_key_state_enum state;

    //--------------------------------------------- 1. 处理全部按键事件 ---------------------------------------------
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

    //--------------------------------------------- 2. 周期刷新当前页面 ---------------------------------------------
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
// 函数简介     切换到指定页面
// 参数说明     page         页面编号
// 返回参数     void
//-------------------------------------------------------------------------------------------------------------------
void app_main_goto_page (app_page_enum page)
{
    if(page >= APP_PAGE_NUMBER)
    {
        return;
    }

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
            app_page_table[page]->init();                                       // 进入页面初始化
        }
        if(0 != app_page_table[page]->draw)
        {
            app_page_table[page]->draw();                                       // 整页重绘
        }
    }
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     返回任务总菜单
// 返回参数     void
//-------------------------------------------------------------------------------------------------------------------
void app_main_goto_main (void)
{
    app_main_goto_page(APP_PAGE_MAIN);
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     获取当前页面编号
// 返回参数     app_page_enum
//-------------------------------------------------------------------------------------------------------------------
app_page_enum app_main_get_page (void)
{
    return app_main_page;
}
