/*********************************************************************************************************************
* 文件名称          app_task.c
* 功能简介          各考核任务的演示页面实现（app 层）
********************************************************************************************************************/
#include "app_task.h"
#include "app_ui.h"
#include "app_main.h"
#include "app_param.h"
#include "bsp_led.h"
#include "bsp_key.h"
#include "bsp_pwm.h"
#include "bsp_pit.h"
#include "bsp_lcd.h"
#include "bsp_flash.h"
#include "bsp_exti.h"

#define APP_TASK_BUF_LEN            (24)

static char app_task_buf[APP_TASK_BUF_LEN];                                     // 显示用临时缓冲区

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     蜂鸣器提示音（受 SYSTEM/BUZZER 参数控制）
// 参数说明     ms           持续时间(ms)
// 返回参数     void
//-------------------------------------------------------------------------------------------------------------------
void app_task_beep (uint32 ms)
{
    app_param_item_struct *item = app_param_get_item(2, 0);                     // SYSTEM 组 BUZZER 参数

    if((0 != item) && (item->value > 0))
    {
        bsp_buzzer_beep_start(ms);                                              // 非阻塞，避免卡住界面
    }
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     任务演示公共初始化
// 返回参数     void
//-------------------------------------------------------------------------------------------------------------------
void app_task_common_init (void)
{
    bsp_led_all_off();
}

/*********************************************************************************************************************
*                                       任务3：GPIO 控制右侧 LED
********************************************************************************************************************/
#define PAGE_LED_NAME               "T3 LED GPIO"

static uint8  task_led_sel       = 1;                                           // 当前选中的 LED 编号
static uint8  task_led_blink     = 0;                                           // 闪烁演示开关
static uint32 task_led_last_ms   = 0;                                           // 闪烁计时

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     任务3 页面：进入
// 返回参数     void
//-------------------------------------------------------------------------------------------------------------------
static void task_led_init (void)
{
    task_led_sel     = BSP_LED_RIGHT_INDEX;                                     // 默认选中"右侧 LED"
    task_led_blink   = 0;
    task_led_last_ms = bsp_pit_get_tick_ms();
    bsp_led_all_off();
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     任务3 页面：整页重绘
// 返回参数     void
//-------------------------------------------------------------------------------------------------------------------
static void task_led_draw (void)
{
    uint8 i;

    app_ui_title(PAGE_LED_NAME);
    app_ui_clear_body();

    for(i = 1; i <= BSP_LED_NUM; i++)
    {
        sprintf(app_task_buf, "LED%d", i);
        app_ui_menu_item(i, app_task_buf, bsp_led_get(i) ? ("ON ") : ("OFF"), (i == task_led_sel) ? (1) : (0));
    }

    app_ui_menu_item(BSP_LED_NUM + 1, "BLINK", task_led_blink ? ("ON ") : ("OFF"), 0);
    app_ui_text(BSP_LED_NUM + 2, "RIGHT LED = A11");
    app_ui_text(BSP_LED_NUM + 3, "U/D SEL MID=TOG");
    app_ui_status("K2 BLINK K1=BACK");
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     任务3 页面：周期刷新
// 返回参数     void
//-------------------------------------------------------------------------------------------------------------------
static void task_led_update (void)
{
    if(0 == task_led_blink)
    {
        return;
    }

    // 用系统节拍做的软件定时闪烁（与任务6 的"定时器中断控制"形成对比）
    if(bsp_pit_check_interval(&task_led_last_ms, BSP_LED_BLINK_PERIOD_MS))
    {
        bsp_led_toggle(task_led_sel);

        sprintf(app_task_buf, "LED%d", task_led_sel);
        app_ui_menu_item(task_led_sel, app_task_buf,
                         bsp_led_get(task_led_sel) ? ("ON ") : ("OFF"),
                         (task_led_sel == task_led_sel) ? (1) : (0));
    }
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     任务3 页面：按键处理
// 返回参数     void
//-------------------------------------------------------------------------------------------------------------------
static void task_led_key (bsp_key_enum key, bsp_key_state_enum state)
{
    if(BSP_KEY_STATE_RELEASE == state)
    {
        return;
    }

    switch(key)
    {
        case BSP_KEY_UP:
        {
            if(task_led_sel > 1)
            {
                task_led_sel--;
            }
            app_task_beep(20);
            task_led_draw();
            break;
        }
        case BSP_KEY_DOWN:
        {
            if(task_led_sel < BSP_LED_NUM)
            {
                task_led_sel++;
            }
            app_task_beep(20);
            task_led_draw();
            break;
        }
        case BSP_KEY_MID:
        {
            bsp_led_set(task_led_sel, (bsp_led_get(task_led_sel) ? (0) : (1)));
            app_task_beep(30);
            task_led_draw();
            break;
        }
        case BSP_KEY_KEY2:
        {
            task_led_blink = (0 == task_led_blink) ? (1) : (0);
            app_task_beep(40);
            task_led_draw();
            break;
        }
        case BSP_KEY_KEY1:
        {
            app_main_goto_main();                                               // 返回主菜单（任务11）
            break;
        }
        default:
        {
            break;
        }
    }
}

const app_page_struct app_task_page_led =
{
    PAGE_LED_NAME, task_led_init, task_led_draw, task_led_update, task_led_key
};

/*********************************************************************************************************************
*                                       任务4：正确读取按键状态
********************************************************************************************************************/
#define PAGE_KEY_NAME               "T4 KEY READ"

static uint32          task_key_last_event_ms = 0;
static bsp_key_enum    task_key_last_key      = BSP_KEY_NONE;
static bsp_key_state_enum task_key_last_state = BSP_KEY_STATE_RELEASE;

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     任务4 页面：进入
// 返回参数     void
//-------------------------------------------------------------------------------------------------------------------
static void task_key_init (void)
{
    task_key_last_key = BSP_KEY_NONE;
    task_key_last_state = BSP_KEY_STATE_RELEASE;
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     任务4 页面：整页重绘
// 返回参数     void
//-------------------------------------------------------------------------------------------------------------------
static void task_key_draw (void)
{
    app_ui_title(PAGE_KEY_NAME);
    app_ui_clear_body();

    app_ui_text(1, "3BIT CODE A2A1A0");
    app_ui_text(2, "D8=A0 B12=A1 B14");
    app_ui_text(3, "74HC148 ENCODER");
    app_ui_text(4, "U D L R M K1 K2");
    app_ui_text(5, "U/D NOT USED");
    app_ui_text(6, "PRESS ANY KEY...");
    app_ui_status("HOLD KEY = LONG");
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     任务4 页面：周期刷新（实时显示按键状态，这是任务4 的核心）
// 返回参数     void
//-------------------------------------------------------------------------------------------------------------------
static void task_key_update (void)
{
    uint8 code = bsp_key_read_code();                                           // 直接读 GPIO 得到的原始编码
    bsp_key_enum pressed = bsp_key_get_pressed();

    // 第 1 行：原始编码 + 解析出的按键
    sprintf(app_task_buf, "CODE:%d  KEY:%s", code, bsp_key_name(bsp_key_code_to_key(code)));
    app_ui_text(1, app_task_buf);

    // 第 2 行：已消抖的按下状态
    sprintf(app_task_buf, "PRESSED:%-5s", bsp_key_name(pressed));
    app_ui_text(2, app_task_buf);

    // 第 3 行：按住时长（长按判定）
    if(BSP_KEY_NONE != pressed)
    {
        sprintf(app_task_buf, "HOLD:%4d MS", bsp_key_get_press_ms(pressed));
    }
    else
    {
        sprintf(app_task_buf, "HOLD:   0 MS");
    }
    app_ui_text(3, app_task_buf);

    // 第 4 行：累计事件次数
    sprintf(app_task_buf, "EVENT CNT:%6d", (int)bsp_key_get_event_count());
    app_ui_text(4, app_task_buf);

    // 第 5 行：最近一次事件（短按 / 长按）
    if(BSP_KEY_STATE_RELEASE == task_key_last_state)
    {
        sprintf(app_task_buf, "LAST: NONE");
    }
    else
    {
        sprintf(app_task_buf, "LAST:%-5s %s", bsp_key_name(task_key_last_key),
                (BSP_KEY_STATE_LONG == task_key_last_state) ? ("LONG") : ("SHORT"));
    }
    app_ui_text(5, app_task_buf);

    // 第 6 行：消抖时间（可由任务10 的参数修改）
    sprintf(app_task_buf, "DEBOUNCE:%3d MS", bsp_key_get_debounce_ms());
    app_ui_text(6, app_task_buf);

    (void)task_key_last_event_ms;
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     任务4 页面：按键处理
// 返回参数     void
//-------------------------------------------------------------------------------------------------------------------
static void task_key_key (bsp_key_enum key, bsp_key_state_enum state)
{
    if(BSP_KEY_STATE_RELEASE == state)
    {
        return;
    }

    // 记录最近一次事件用于显示
    task_key_last_key   = key;
    task_key_last_state = state;
    task_key_last_event_ms = bsp_pit_get_tick_ms();

    if(BSP_KEY_KEY1 == key)
    {
        app_main_goto_main();
    }
}

const app_page_struct app_task_page_key =
{
    PAGE_KEY_NAME, task_key_init, task_key_draw, task_key_update, task_key_key
};

/*********************************************************************************************************************
*                                       任务5：PWM 输出 + 无源蜂鸣器
********************************************************************************************************************/
#define PAGE_PWM_NAME               "T5 PWM BUZZER"

static uint32 task_pwm_freq    = BSP_BUZZER_FREQ_HZ;                            // 蜂鸣器频率
static uint32 task_pwm_duty    = 50;                                            // 蜂鸣器占空比(%)
static uint32 task_pwm_demo_duty = 50;                                          // 第二路 PWM 占空比(%)
static uint32 task_pwm_sweep_ms = 0;                                            // PWM 扫频计时

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     任务5 页面：进入
// 返回参数     void
//-------------------------------------------------------------------------------------------------------------------
static void task_pwm_init (void)
{
    task_pwm_freq      = BSP_BUZZER_FREQ_HZ;
    task_pwm_duty      = 50;
    task_pwm_demo_duty = 50;

    bsp_buzzer_set_freq(task_pwm_freq);
    bsp_buzzer_off();
    bsp_pwm_demo_set(BSP_PWM_DEMO_FREQ_HZ, BSP_PWM_DEMO_DUTY);
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     任务5 页面：整页重绘
// 返回参数     void
//-------------------------------------------------------------------------------------------------------------------
static void task_pwm_draw (void)
{
    char val[10];

    app_ui_title(PAGE_PWM_NAME);
    app_ui_clear_body();

    app_ui_text(1, "FMQ A8 TIM1_CH1");

    sprintf(val, "%dHZ", (int)task_pwm_freq);
    app_ui_menu_item(2, "FREQ", val, 0);

    sprintf(val, "%d%%", (int)task_pwm_duty);
    app_ui_menu_item(3, "DUTY", val, 0);

    app_ui_menu_item(4, "BUZZER", bsp_buzzer_is_on() ? ("ON ") : ("OFF"), 0);

    sprintf(app_task_buf, "DEMO C6 %3d %%", (int)task_pwm_demo_duty);
    app_ui_text(5, app_task_buf);

    app_ui_text(6, "PASSIVE 2.731KHZ");
    app_ui_status("U/D F L/R D M=ON");
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     任务5 页面：周期刷新（PWM 扫频演示，证明 PWM 真的在输出）
// 返回参数     void
//-------------------------------------------------------------------------------------------------------------------
static void task_pwm_update (void)
{
    // 每 300ms 让第二路 PWM 占空比变化一次，用示波器/万用表可以看到输出在变化
    if(bsp_pit_check_interval(&task_pwm_sweep_ms, 300))
    {
        task_pwm_demo_duty += 10;
        if(task_pwm_demo_duty > 100)
        {
            task_pwm_demo_duty = 0;
        }
        bsp_pwm_demo_set_duty(task_pwm_demo_duty * (PWM_DUTY_MAX / 100));

        sprintf(app_task_buf, "DEMO C6 %3d %%", (int)task_pwm_demo_duty);
        app_ui_text(5, app_task_buf);
    }
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     任务5 页面：按键处理
// 返回参数     void
//-------------------------------------------------------------------------------------------------------------------
static void task_pwm_key (bsp_key_enum key, bsp_key_state_enum state)
{
    if(BSP_KEY_STATE_RELEASE == state)
    {
        return;
    }

    switch(key)
    {
        case BSP_KEY_UP:
        {
            if(task_pwm_freq < 10000)
            {
                task_pwm_freq += 100;
            }
            bsp_buzzer_set_freq(task_pwm_freq);
            break;
        }
        case BSP_KEY_DOWN:
        {
            if(task_pwm_freq > 100)
            {
                task_pwm_freq -= 100;
            }
            bsp_buzzer_set_freq(task_pwm_freq);
            break;
        }
        case BSP_KEY_RIGHT:
        {
            if(task_pwm_duty < 100)
            {
                task_pwm_duty += 5;
            }
            bsp_buzzer_set_duty(task_pwm_duty * (PWM_DUTY_MAX / 100));
            break;
        }
        case BSP_KEY_LEFT:
        {
            if(task_pwm_duty > 5)
            {
                task_pwm_duty -= 5;
            }
            bsp_buzzer_set_duty(task_pwm_duty * (PWM_DUTY_MAX / 100));
            break;
        }
        case BSP_KEY_MID:
        {
            if(bsp_buzzer_is_on())
            {
                bsp_buzzer_off();
            }
            else
            {
                bsp_buzzer_set_freq(task_pwm_freq);
                bsp_buzzer_set_duty(task_pwm_duty * (PWM_DUTY_MAX / 100));
                bsp_buzzer_on();
            }
            break;
        }
        case BSP_KEY_KEY2:
        {
            // 用蜂鸣器播放一段"无源蜂鸣器换频率"的效果，验证频率可调
            bsp_buzzer_set_freq(1046);  bsp_buzzer_on();  system_delay_ms(150);
            bsp_buzzer_set_freq(1318);  system_delay_ms(150);
            bsp_buzzer_set_freq(1568);  system_delay_ms(150);
            bsp_buzzer_set_freq(2093);  system_delay_ms(200);
            bsp_buzzer_off();
            bsp_buzzer_set_freq(task_pwm_freq);
            break;
        }
        case BSP_KEY_KEY1:
        {
            bsp_buzzer_off();
            app_main_goto_main();
            break;
        }
        default:
        {
            break;
        }
    }

    task_pwm_draw();
}

const app_page_struct app_task_page_pwm =
{
    PAGE_PWM_NAME, task_pwm_init, task_pwm_draw, task_pwm_update, task_pwm_key
};

/*********************************************************************************************************************
*                                       任务6：定时器中断控制 LED
********************************************************************************************************************/
#define PAGE_TIMER_NAME             "T6 TIMER IT LED"

static uint8  task_timer_enable = 1;
static uint8  task_timer_led    = 1;

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     任务6 页面：进入（打开定时中断翻转 LED）
// 返回参数     void
//-------------------------------------------------------------------------------------------------------------------
static void task_timer_init (void)
{
    task_timer_enable = 1;
    task_timer_led    = BSP_LED_DEFAULT_INDEX;

    bsp_pit_led_set_index(task_timer_led);
    bsp_pit_led_enable(task_timer_enable);
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     任务6 页面：整页重绘
// 返回参数     void
//-------------------------------------------------------------------------------------------------------------------
static void task_timer_draw (void)
{
    app_ui_title(PAGE_TIMER_NAME);
    app_ui_clear_body();

    app_ui_text(1, "TIM7 UPDATE IRQ");
    app_ui_menu_item(2, "STATE", task_timer_enable ? ("ON ") : ("OFF"), 0);

    sprintf(app_task_buf, "%d MS", (int)bsp_pit_led_get_period());
    app_ui_menu_item(3, "PERIOD", app_task_buf, 0);

    sprintf(app_task_buf, "LED%d", task_timer_led);
    app_ui_menu_item(4, "TARGET", app_task_buf, 0);

    app_ui_text(6, "TOGGLED IN ISR");
    app_ui_text(7, "NO CODE IN MAIN");
    app_ui_status("M:ON/OFF K2:LED");
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     任务6 页面：周期刷新（显示中断里累计的翻转次数）
// 返回参数     void
//-------------------------------------------------------------------------------------------------------------------
static void task_timer_update (void)
{
    sprintf(app_task_buf, "TOGGLE CNT %6d", (int)bsp_pit_led_get_count());
    app_ui_text(5, app_task_buf);
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     任务6 页面：按键处理
// 返回参数     void
//-------------------------------------------------------------------------------------------------------------------
static void task_timer_key (bsp_key_enum key, bsp_key_state_enum state)
{
    uint32 period;

    if(BSP_KEY_STATE_RELEASE == state)
    {
        return;
    }

    switch(key)
    {
        case BSP_KEY_MID:
        {
            task_timer_enable = (0 == task_timer_enable) ? (1) : (0);
            bsp_pit_led_enable(task_timer_enable);
            app_task_beep(30);
            break;
        }
        case BSP_KEY_KEY2:
        {
            task_timer_led++;
            if(task_timer_led > BSP_LED_NUM)
            {
                task_timer_led = 1;
            }
            bsp_pit_led_set_index(task_timer_led);
            app_task_beep(30);
            break;
        }
        case BSP_KEY_UP:
        {
            period = bsp_pit_led_get_period();
            if(period < 1000)
            {
                bsp_pit_led_set_period(period + 50);
            }
            break;
        }
        case BSP_KEY_DOWN:
        {
            period = bsp_pit_led_get_period();
            if(period > 50)
            {
                bsp_pit_led_set_period(period - 50);
            }
            break;
        }
        case BSP_KEY_KEY1:
        {
            bsp_pit_led_enable(0);                                              // 离开页面时关闭，避免影响其它任务显示
            app_main_goto_main();
            break;
        }
        default:
        {
            break;
        }
    }

    task_timer_draw();
}

const app_page_struct app_task_page_timer =
{
    PAGE_TIMER_NAME, task_timer_init, task_timer_draw, task_timer_update, task_timer_key
};

/*********************************************************************************************************************
*                                       任务7：SPI 驱动屏幕
********************************************************************************************************************/
#define PAGE_LCD_NAME               "T7 SPI LCD"

static uint32 task_lcd_run_count = 0;

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     任务7 页面：进入（跑一次屏幕测试图案）
// 返回参数     void
//-------------------------------------------------------------------------------------------------------------------
static void task_lcd_init (void)
{
    bsp_lcd_set_backlight(1);                                                   // 防止背光被参数关掉导致看不见
    bsp_lcd_draw_test_pattern();
    app_ui_init();
    task_lcd_run_count = 1;
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     任务7 页面：整页重绘
// 返回参数     void
//-------------------------------------------------------------------------------------------------------------------
static void task_lcd_draw (void)
{
    app_ui_title(PAGE_LCD_NAME);
    app_ui_clear_body();

    app_ui_text(1, "ST7735 1.8INCH");
    app_ui_text(2, "SPI2 SCL=PB13");
    app_ui_text(3, "SPI2 SDA=PB15");
    app_ui_text(4, "RES=PB7  DC=PD7");
    app_ui_text(5, "CS=PD4   BL=PD0");
    app_ui_text(6, "HARD SPI 72MHZ");
    sprintf(app_task_buf, "TEST RUN:%3d", (int)task_lcd_run_count);
    app_ui_text(7, app_task_buf);
    app_ui_status("MID=RERUN RGB");
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     任务7 页面：周期刷新
// 返回参数     void
//-------------------------------------------------------------------------------------------------------------------
static void task_lcd_update (void)
{
    // 本页面不需要动态刷新
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     任务7 页面：按键处理
// 返回参数     void
//-------------------------------------------------------------------------------------------------------------------
static void task_lcd_key (bsp_key_enum key, bsp_key_state_enum state)
{
    if(BSP_KEY_STATE_RELEASE == state)
    {
        return;
    }

    switch(key)
    {
        case BSP_KEY_MID:
        {
            bsp_lcd_draw_test_pattern();                                        // 重新跑一次 RGB 测试
            task_lcd_run_count++;
            app_ui_init();
            task_lcd_draw();
            break;
        }
        case BSP_KEY_KEY1:
        {
            app_main_goto_main();
            break;
        }
        default:
        {
            break;
        }
    }
}

const app_page_struct app_task_page_lcd =
{
    PAGE_LCD_NAME, task_lcd_init, task_lcd_draw, task_lcd_update, task_lcd_key
};

/*********************************************************************************************************************
*                                       任务8：Flash 保存并读取数据
********************************************************************************************************************/
#define PAGE_FLASH_NAME             "T8 FLASH SAVE"

static uint8 task_flash_last_result = 0xFF;
static uint8 task_flash_last_op     = 0;                                        // 0-无 1-保存 2-读取

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     任务8 页面：进入
// 返回参数     void
//-------------------------------------------------------------------------------------------------------------------
static void task_flash_init (void)
{
    task_flash_last_result = app_param_get_last_result();
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     任务8 页面：整页重绘
// 返回参数     void
//-------------------------------------------------------------------------------------------------------------------
static void task_flash_draw (void)
{
    app_ui_title(PAGE_FLASH_NAME);
    app_ui_clear_body();

    app_ui_text(1, "MCU FLASH 256K");
    app_ui_text(2, "SECTOR 63 PAGE 0");
    // 行 3 ~ 行 6 由 task_flash_update 周期刷新（BOOT NOW / BOOT PREV / SAVE / LOAD / LAST）
    app_ui_text(7, "POWER OFF AND ON");
    app_ui_text(8, "MAGIC+LEN+DATA");
    app_ui_status("M=SAVE K2=LOAD");
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     任务8 页面：周期刷新（显示掉电保存的关键证据）
// 返回参数     void
//-------------------------------------------------------------------------------------------------------------------
static void task_flash_update (void)
{
    uint32 save_cnt = 0;
    uint32 load_cnt = 0;

    bsp_flash_get_count(&save_cnt, &load_cnt);

    // 开机次数：本次上电的次数 与 上次 Flash 中记录的次数
    sprintf(app_task_buf, "BOOT NOW :%6d", (int)app_param_get_boot_count());
    app_ui_text(3, app_task_buf);

    sprintf(app_task_buf, "BOOT PREV:%6d", (int)app_param_get_prev_boot());
    app_ui_text(4, app_task_buf);

    // 读写次数 + 最后一次操作结果
    sprintf(app_task_buf, "SAVE:%3d LOAD:%3d", (int)save_cnt, (int)load_cnt);
    app_ui_text(5, app_task_buf);

    if(0 == task_flash_last_op)
    {
        sprintf(app_task_buf, "LAST: --      ");
    }
    else
    {
        sprintf(app_task_buf, "LAST: %s %s",
                (1 == task_flash_last_op) ? ("SAVE") : ("LOAD"),
                (0 == task_flash_last_result) ? ("OK") : ("ERR"));
    }
    app_ui_text(6, app_task_buf);
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     任务8 页面：按键处理
// 返回参数     void
//-------------------------------------------------------------------------------------------------------------------
static void task_flash_key (bsp_key_enum key, bsp_key_state_enum state)
{
    if(BSP_KEY_STATE_RELEASE == state)
    {
        return;
    }

    switch(key)
    {
        case BSP_KEY_MID:
        {
            // 把当前（可能在任务10 里改过的）参数保存到 Flash
            task_flash_last_result = app_param_save();
            task_flash_last_op     = 1;
            app_task_beep(60);
            break;
        }
        case BSP_KEY_KEY2:
        {
            // 从 Flash 读回数据并生效（任务8 "能够读取保存的数据"）
            task_flash_last_result = app_param_load();
            task_flash_last_op     = 2;
            app_task_beep(60);
            break;
        }
        case BSP_KEY_KEY1:
        {
            app_main_goto_main();
            break;
        }
        default:
        {
            break;
        }
    }
}

const app_page_struct app_task_page_flash =
{
    PAGE_FLASH_NAME, task_flash_init, task_flash_draw, task_flash_update, task_flash_key
};

/*********************************************************************************************************************
*                                       任务9：外部中断读按键 + 屏幕 UI 交互
********************************************************************************************************************/
#define PAGE_EXTI_NAME              "T9 EXTI UI"

static int32 task_exti_counter = 0;                                             // 由按键操作的界面变量
static uint8 task_exti_enable  = 1;

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     任务9 页面：进入
// 返回参数     void
//-------------------------------------------------------------------------------------------------------------------
static void task_exti_init (void)
{
    task_exti_counter = 0;
    task_exti_enable  = 1;
    bsp_exti_enable(1);
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     任务9 页面：整页重绘
// 返回参数     void
//-------------------------------------------------------------------------------------------------------------------
static void task_exti_draw (void)
{
    app_ui_title(PAGE_EXTI_NAME);
    app_ui_clear_body();

    app_ui_text(1, "EXTI L8=D8 L12=B");
    app_ui_text(2, "BOTH EDGE TRIG");
    app_ui_menu_item(3, "EXTI", task_exti_enable ? ("ENABLE ") : ("DISABLE"), 0);
    app_ui_text(6, "KEYS EDIT COUNT");
    app_ui_text(7, "U/R +1   D/L -1");
    app_ui_status("M=RESET K1=BACK");
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     任务9 页面：周期刷新（显示外部中断次数 + 界面交互变量）
// 返回参数     void
//-------------------------------------------------------------------------------------------------------------------
static void task_exti_update (void)
{
    sprintf(app_task_buf, "ISR CNT :%7d", (int)bsp_exti_get_count());
    app_ui_text(4, app_task_buf);

    sprintf(app_task_buf, "COUNTER :%7d", (int)task_exti_counter);
    app_ui_text(5, app_task_buf);
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     任务9 页面：按键处理
// 返回参数     void
//-------------------------------------------------------------------------------------------------------------------
static void task_exti_key (bsp_key_enum key, bsp_key_state_enum state)
{
    if(BSP_KEY_STATE_RELEASE == state)
    {
        return;
    }

    switch(key)
    {
        case BSP_KEY_UP:
        case BSP_KEY_RIGHT:
        {
            task_exti_counter++;
            app_task_beep(20);
            break;
        }
        case BSP_KEY_DOWN:
        case BSP_KEY_LEFT:
        {
            task_exti_counter--;
            app_task_beep(20);
            break;
        }
        case BSP_KEY_MID:
        {
            task_exti_counter = 0;
            app_task_beep(40);
            break;
        }
        case BSP_KEY_KEY2:
        {
            task_exti_enable = (0 == task_exti_enable) ? (1) : (0);
            bsp_exti_enable(task_exti_enable);
            app_main_goto_page(APP_PAGE_EXTI);                                  // 重绘当前页
            break;
        }
        case BSP_KEY_KEY1:
        {
            app_main_goto_main();
            break;
        }
        default:
        {
            break;
        }
    }
}

const app_page_struct app_task_page_exti =
{
    PAGE_EXTI_NAME, task_exti_init, task_exti_draw, task_exti_update, task_exti_key
};
