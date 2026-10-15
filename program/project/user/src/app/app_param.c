/*********************************************************************************************************************
* 文件名称          app_param.c
* 功能简介          参数管理（任务8 / 任务10）
********************************************************************************************************************/
#include "app_param.h"
#include "bsp_flash.h"
#include "bsp_pit.h"
#include "bsp_key.h"
#include "bsp_lcd.h"

// 参数表：3 组 x 3 个
static app_param_group_struct app_param_table[APP_PARAM_GROUP_NUM] =
{
    {
        "MOTOR",
        {
            { "SPEED",    0, 100,  1,  50,  50 },                               // 目标速度(%)
            { "DUTY",     0, 100,  1,  60,  60 },                               // 最大占空比(%)
            { "ACC",      1,  50,  1,  10,  10 },                               // 加速度
        }
    },
    {
        "DISPLAY",
        {
            { "BRIGHT",   0, 100,  5,  80,  80 },                               // 屏幕背光(0=关闭, >0=开启)
            { "REFRESH", 10, 200, 10, 100, 100 },                                // UI 刷新周期(ms)
            { "LEDBLINK",50,1000, 50, 500, 500 },                                // 定时器中断 LED 翻转周期(ms)
        }
    },
    {
        "SYSTEM",
        {
            { "BUZZER",   0,   1,  1,   1,   1 },                               // 蜂鸣器开关
            { "DEBOUNCE", 1,  50,  1,  10,  10 },                               // 按键消抖(ms)
            { "LONGTIME",200,2000,100, 800, 800 },                              // 长按判定(ms)
        }
    },
};

static app_param_store_struct app_param_store;                                  // 掉电保存数据
static uint32 app_param_prev_boot   = 0;                                        // 上次开机次数
static uint8  app_param_last_result = 0;                                        // 最后一次保存结果

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     把参数表当前值整理到保存结构体中
// 返回参数     void
//-------------------------------------------------------------------------------------------------------------------
static void app_param_collect (void)
{
    uint8 g;
    uint8 i;

    for(g = 0; g < APP_PARAM_GROUP_NUM; g++)
    {
        for(i = 0; i < APP_PARAM_PER_GROUP; i++)
        {
            app_param_store.value[g * APP_PARAM_PER_GROUP + i] = app_param_table[g].item[i].value;
        }
    }
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     把保存结构体中的值还原到参数表
// 返回参数     void
//-------------------------------------------------------------------------------------------------------------------
static void app_param_restore (void)
{
    uint8 g;
    uint8 i;

    for(g = 0; g < APP_PARAM_GROUP_NUM; g++)
    {
        for(i = 0; i < APP_PARAM_PER_GROUP; i++)
        {
            app_param_set_value(g, i, app_param_store.value[g * APP_PARAM_PER_GROUP + i]);
        }
    }
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     参数初始化
// 返回参数     void
//-------------------------------------------------------------------------------------------------------------------
void app_param_init (void)
{
    uint8 k;

    app_param_reset_default();                                                  // 先全部恢复默认值

    app_param_store.boot_count = 0;
    for(k = 0; k < APP_PARAM_TOTAL; k++)
    {
        app_param_store.value[k] = 0;
    }

    // 尝试从 Flash 读取上次保存的数据
    if(0 == bsp_flash_load((uint32 *)&app_param_store, (uint16)(sizeof(app_param_store) / 4)))
    {
        app_param_prev_boot = app_param_store.boot_count;                       // 记录"上一次"的开机次数

        if(app_param_store.boot_count < 0xFFFFFFFF)
        {
            app_param_store.boot_count++;
        }
        app_param_restore();                                                    // 恢复保存的参数
    }
    else
    {
        app_param_prev_boot = 0;
        app_param_store.boot_count = 1;                                         // 第一次运行
    }

    app_param_collect();                                                        // 同步一次

    // 立刻把开机次数写回 Flash，这样下次上电就能读到 +1 的开机次数
    app_param_last_result = app_param_save();

    app_param_apply_to_bsp();
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     从 Flash 读取参数并生效
// 返回参数     uint8        0-成功 其它-失败
//-------------------------------------------------------------------------------------------------------------------
uint8 app_param_load (void)
{
    app_param_store_struct temp;

    // 读到临时变量里，成功之后才覆盖当前值，避免读取失败把参数弄乱
    if(0 != bsp_flash_load((uint32 *)&temp, (uint16)(sizeof(temp) / 4)))
    {
        app_param_last_result = 5;
        return 5;
    }

    app_param_store = temp;
    app_param_prev_boot = app_param_store.boot_count;
    app_param_restore();
    app_param_apply_to_bsp();

    app_param_last_result = 0;
    return 0;
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     把当前参数保存到 Flash
// 返回参数     uint8        0-成功 其它-失败
//-------------------------------------------------------------------------------------------------------------------
uint8 app_param_save (void)
{
    app_param_collect();
    app_param_last_result = bsp_flash_save((uint32 *)&app_param_store,
                                           (uint16)(sizeof(app_param_store) / 4));
    return app_param_last_result;
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     把参数恢复成默认值
// 返回参数     void
//-------------------------------------------------------------------------------------------------------------------
void app_param_reset_default (void)
{
    uint8 g;
    uint8 i;

    for(g = 0; g < APP_PARAM_GROUP_NUM; g++)
    {
        for(i = 0; i < APP_PARAM_PER_GROUP; i++)
        {
            app_param_table[g].item[i].value = app_param_table[g].item[i].def_value;
        }
    }
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     获取指定参数组
// 参数说明     group        组下标
// 返回参数     app_param_group_struct*
//-------------------------------------------------------------------------------------------------------------------
app_param_group_struct *app_param_get_group (uint8 group)
{
    if(group >= APP_PARAM_GROUP_NUM)
    {
        return 0;
    }
    return &app_param_table[group];
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     获取某个参数项
// 参数说明     group        组下标
// 参数说明     index        组内下标
// 返回参数     app_param_item_struct*
//-------------------------------------------------------------------------------------------------------------------
app_param_item_struct *app_param_get_item (uint8 group, uint8 index)
{
    if((group >= APP_PARAM_GROUP_NUM) || (index >= APP_PARAM_PER_GROUP))
    {
        return 0;
    }
    return &app_param_table[group].item[index];
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     修改参数值（自动按 min/max/step 规范化）
// 参数说明     group        组下标
// 参数说明     index        组内下标
// 参数说明     value        新值
// 返回参数     int32        规范化之后的实际值
//-------------------------------------------------------------------------------------------------------------------
int32 app_param_set_value (uint8 group, uint8 index, int32 value)
{
    app_param_item_struct *item = app_param_get_item(group, index);

    if(0 == item)
    {
        return 0;
    }

    if(value < item->min)
    {
        value = item->min;
    }
    if(value > item->max)
    {
        value = item->max;
    }

    item->value = value;
    return item->value;
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     在参数值基础上增加/减少一个步长
// 参数说明     group        组下标
// 参数说明     index        组内下标
// 参数说明     dir          1-增加 -1-减少
// 返回参数     int32        调整之后的实际值
//-------------------------------------------------------------------------------------------------------------------
int32 app_param_step_value (uint8 group, uint8 index, int8 dir)
{
    app_param_item_struct *item = app_param_get_item(group, index);

    if(0 == item)
    {
        return 0;
    }

    return app_param_set_value(group, index, item->value + (int32)dir * item->step);
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     获取开机次数
// 返回参数     uint32
//-------------------------------------------------------------------------------------------------------------------
uint32 app_param_get_boot_count (void)
{
    return app_param_store.boot_count;
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     获取上一次上电时 Flash 中已经存在的开机次数
// 返回参数     uint32
//-------------------------------------------------------------------------------------------------------------------
uint32 app_param_get_prev_boot (void)
{
    return app_param_prev_boot;
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     获取最后一次 Flash 保存的结果
// 返回参数     uint8
//-------------------------------------------------------------------------------------------------------------------
uint8 app_param_get_last_result (void)
{
    return app_param_last_result;
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     把参数值同步到 BSP 层
// 说明         这是"参数真正生效"的地方：
//                 菜单里改参数 -> 立即调用本函数 -> 硬件行为随之改变
// 返回参数     void
//-------------------------------------------------------------------------------------------------------------------
void app_param_apply_to_bsp (void)
{
    // 显示组
    bsp_lcd_set_backlight((app_param_get_item(1, 0)->value > 0) ? (1) : (0));    // BRIGHT  -> 背光开关
    bsp_pit_led_set_period((uint32)app_param_get_item(1, 2)->value);            // LEDBLINK-> 定时中断 LED 周期

    // 系统组
    bsp_key_set_debounce_ms((uint16)app_param_get_item(2, 1)->value);           // DEBOUNCE-> 按键消抖时间
    bsp_key_set_long_press_ms((uint16)app_param_get_item(2, 2)->value);         // LONGTIME-> 长按判定时间

    // MOTOR 组为下一轮 PID 预留参数，本轮只做记录与掉电保存
}
