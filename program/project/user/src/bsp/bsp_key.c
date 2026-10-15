/*********************************************************************************************************************
* 文件名称          bsp_key.c
* 功能简介          按键 BSP 驱动（任务4 / 任务9 / 任务10）
*
* 原理：
*   五向开关 + KEY1/KEY2 一共 7 个按键，通过 SN74HC148（8-3 优先编码器）编码成 3 位，
*   分别接入 KEY_IN1_D8(A0) / KEY_IN2_B12(A1) / KEY_IN3_B14(A2)。
*   74HC148 输出低有效编码：
*       无按键(或有 I0 输入) -> 111 = 7
*       按下 U15 的 I7 ~ I1  -> 000 ~ 110 = 0 ~ 6
*   本驱动把 GPIO 读回的 3bit 直接转换为按键枚举，并做消抖 + 短按/长按判定。
********************************************************************************************************************/
#include "bsp_key.h"
#include "bsp_exti.h"

#define BSP_KEY_EVENT_QUEUE_LEN         (8)                                     // 按键事件队列长度（2 的整数次幂）

// 事件队列（由 1ms 中断写入，主循环读出）
static volatile bsp_key_state_enum bsp_key_event_state[BSP_KEY_EVENT_QUEUE_LEN];
static volatile bsp_key_enum       bsp_key_event_val  [BSP_KEY_EVENT_QUEUE_LEN];
static volatile uint8              bsp_key_event_head = 0;
static volatile uint8              bsp_key_event_tail = 0;
static volatile uint32             bsp_key_event_count = 0;

static uint8            bsp_key_last_code  = BSP_KEY_CODE_NONE;                 // 上一次采样到的编码
static uint8            bsp_key_stable_code = BSP_KEY_CODE_NONE;                // 消抖后的编码
static uint16           bsp_key_change_ms  = 0;                                 // 电平变化持续计数(ms)

static bsp_key_enum     bsp_key_pressed    = BSP_KEY_NONE;                      // 当前按下的按键
static uint16           bsp_key_press_ms   = 0;                                 // 当前按键已按住时长
static uint8            bsp_key_long_fired = 0;                                 // 本次按下是否已经上报过长按

// 运行期可调参数（由任务10 的三级菜单修改，初值来自 bsp_board.h 中的宏定义）
static uint16           bsp_key_debounce_ms   = BSP_KEY_DEBOUNCE_MS;
static uint16           bsp_key_long_press_ms = BSP_KEY_LONG_PRESS_MS;

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     把按键事件压入队列（中断中调用）
// 参数说明     key          按键
// 参数说明     state        事件类型
// 返回参数     void
//-------------------------------------------------------------------------------------------------------------------
static void bsp_key_push_event (bsp_key_enum key, bsp_key_state_enum state)
{
    uint8 next = (uint8)((bsp_key_event_head + 1) % BSP_KEY_EVENT_QUEUE_LEN);

    if(next == bsp_key_event_tail)                                              // 队列满则丢弃最旧事件
    {
        bsp_key_event_tail = (uint8)((bsp_key_event_tail + 1) % BSP_KEY_EVENT_QUEUE_LEN);
    }

    bsp_key_event_state[bsp_key_event_head] = state;
    bsp_key_event_val[bsp_key_event_head] = key;
    bsp_key_event_head = next;
    bsp_key_event_count++;
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     按键初始化
// 返回参数     void
//-------------------------------------------------------------------------------------------------------------------
void bsp_key_init (void)
{
    // 3 路编码输入：上拉输入（原理图上已有上拉电阻，这里再开内部上拉增强抗干扰）
    gpio_init(BSP_KEY_A0_PIN, GPI, GPIO_HIGH, BSP_KEY_GPIO_MODE);
    gpio_init(BSP_KEY_A1_PIN, GPI, GPIO_HIGH, BSP_KEY_GPIO_MODE);
    gpio_init(BSP_KEY_A2_PIN, GPI, GPIO_HIGH, BSP_KEY_GPIO_MODE);

#if BSP_KEY_EXTI_ENABLE
    bsp_exti_init();                                                            // 任务9：外部中断读取按键
#endif

    bsp_key_last_code  = BSP_KEY_CODE_NONE;
    bsp_key_stable_code = BSP_KEY_CODE_NONE;
    bsp_key_change_ms  = 0;
    bsp_key_pressed    = BSP_KEY_NONE;
    bsp_key_press_ms   = 0;
    bsp_key_long_fired = 0;
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     直接读取 3 路 GPIO 组合出的原始编码
// 返回参数     uint8       0 ~ 7
//-------------------------------------------------------------------------------------------------------------------
uint8 bsp_key_read_code (void)
{
    uint8 code = 0;

    if(gpio_get_level(BSP_KEY_A0_PIN))
    {
        code |= 0x01;                                                           // A0 -> bit0
    }
    if(gpio_get_level(BSP_KEY_A1_PIN))
    {
        code |= 0x02;                                                           // A1 -> bit1
    }
    if(gpio_get_level(BSP_KEY_A2_PIN))
    {
        code |= 0x04;                                                           // A2 -> bit2
    }

    return code;
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     把编码值转换为按键枚举
// 参数说明     code         编码值 0 ~ 7
// 返回参数     bsp_key_enum
//-------------------------------------------------------------------------------------------------------------------
bsp_key_enum bsp_key_code_to_key (uint8 code)
{
    switch(code)
    {
        case BSP_KEY_CODE_KEY1:  return BSP_KEY_KEY1;
        case BSP_KEY_CODE_KEY2:  return BSP_KEY_KEY2;
        case BSP_KEY_CODE_MID:   return BSP_KEY_MID;
        case BSP_KEY_CODE_RIGHT: return BSP_KEY_RIGHT;
        case BSP_KEY_CODE_UP:    return BSP_KEY_UP;
        case BSP_KEY_CODE_DOWN:  return BSP_KEY_DOWN;
        case BSP_KEY_CODE_LEFT:  return BSP_KEY_LEFT;
        default:                 return BSP_KEY_NONE;
    }
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     按键扫描（1ms 定时器中断调用）
// 备注信息     先消抖，再判定短按 / 长按；长按按住不放时会按 BSP_KEY_LONG_PRESS_MS 周期重复上报，
//              这样可以用于菜单里连续调节参数。
// 返回参数     void
//-------------------------------------------------------------------------------------------------------------------
void bsp_key_scan_1ms (void)
{
    uint8 code = bsp_key_read_code();

    //-------------------------------------------- 第一步：消抖 --------------------------------------------
    if(code != bsp_key_last_code)
    {
        bsp_key_last_code = code;
        bsp_key_change_ms = 0;
    }
    else if(bsp_key_change_ms < bsp_key_debounce_ms)
    {
        bsp_key_change_ms++;
    }

    if(bsp_key_change_ms < bsp_key_debounce_ms)
    {
        return;                                                                 // 抖动期间不做任何判定
    }

    //-------------------------------------------- 第二步：状态判定 --------------------------------------------
    if(code != bsp_key_stable_code)
    {
        bsp_key_stable_code = code;

        if(BSP_KEY_CODE_NONE != code)                                           // 新按下的按键
        {
            bsp_key_pressed    = bsp_key_code_to_key(code);
            bsp_key_press_ms   = 0;
            bsp_key_long_fired = 0;
        }
        else                                                                    // 松开
        {
            if(0 == bsp_key_long_fired)
            {
                bsp_key_push_event(bsp_key_pressed, BSP_KEY_STATE_SHORT);       // 松手时未满长按 -> 短按
            }
            bsp_key_pressed  = BSP_KEY_NONE;
            bsp_key_press_ms = 0;
        }
        return;
    }

    //-------------------------------------------- 第三步：长按计时 --------------------------------------------
    if(BSP_KEY_NONE != bsp_key_pressed)
    {
        if(bsp_key_press_ms < 0xFFFF)
        {
            bsp_key_press_ms++;
        }

        if(bsp_key_press_ms >= bsp_key_long_press_ms)
        {
            if(0 == bsp_key_long_fired)
            {
                bsp_key_long_fired = 1;
                bsp_key_push_event(bsp_key_pressed, BSP_KEY_STATE_LONG);
                bsp_key_press_ms = 0;                                           // 重置计时，实现长按重复上报
            }
            else if(0 == (bsp_key_press_ms % bsp_key_long_press_ms))
            {
                bsp_key_push_event(bsp_key_pressed, BSP_KEY_STATE_LONG);
            }
        }
    }
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     取出一条按键事件
// 参数说明     key          输出参数，返回事件对应的按键
// 返回参数     bsp_key_state_enum
//-------------------------------------------------------------------------------------------------------------------
bsp_key_state_enum bsp_key_get_event (bsp_key_enum *key)
{
    bsp_key_state_enum state;

    if(0 == key)
    {
        return BSP_KEY_STATE_RELEASE;
    }

    if(bsp_key_event_head == bsp_key_event_tail)
    {
        *key = BSP_KEY_NONE;
        return BSP_KEY_STATE_RELEASE;
    }

    state = bsp_key_event_state[bsp_key_event_tail];
    *key  = bsp_key_event_val[bsp_key_event_tail];
    bsp_key_event_tail = (uint8)((bsp_key_event_tail + 1) % BSP_KEY_EVENT_QUEUE_LEN);

    return state;
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     查询某个按键当前是否处于按下状态
// 参数说明     key          按键
// 返回参数     uint8        1-按下 0-松开
//-------------------------------------------------------------------------------------------------------------------
uint8 bsp_key_is_pressed (bsp_key_enum key)
{
    return (bsp_key_pressed == key) ? (1) : (0);
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     查询当前按下的按键
// 返回参数     bsp_key_enum
//-------------------------------------------------------------------------------------------------------------------
bsp_key_enum bsp_key_get_pressed (void)
{
    return bsp_key_pressed;
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     查询某个按键已经按住的时长
// 参数说明     key          按键
// 返回参数     uint16       毫秒
//-------------------------------------------------------------------------------------------------------------------
uint16 bsp_key_get_press_ms (bsp_key_enum key)
{
    if(bsp_key_pressed == key)
    {
        return bsp_key_press_ms;
    }
    return 0;
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     获取已经处理过的按键事件总次数
// 返回参数     uint32
//-------------------------------------------------------------------------------------------------------------------
uint32 bsp_key_get_event_count (void)
{
    return bsp_key_event_count;
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     获取按键名称字符串（供屏幕显示，仅使用 ASCII 字符）
// 参数说明     key          按键
// 返回参数     const char*  名称
//-------------------------------------------------------------------------------------------------------------------
const char* bsp_key_name (bsp_key_enum key)
{
    static const char *name_table[BSP_KEY_NUMBER] =
    {
        "NONE ",
        "KEY1 ",
        "KEY2 ",
        "MID  ",
        "RIGHT",
        "UP   ",
        "DOWN ",
        "LEFT ",
    };

    if(key >= BSP_KEY_NUMBER)
    {
        return name_table[0];
    }
    return name_table[key];
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     动态修改按键消抖时间（任务10 三级菜单可调参数）
// 参数说明     ms           消抖时间(ms)，0 表示使用宏定义默认值
// 返回参数     void
//-------------------------------------------------------------------------------------------------------------------
void bsp_key_set_debounce_ms (uint16 ms)
{
    if(0 == ms)
    {
        bsp_key_debounce_ms = BSP_KEY_DEBOUNCE_MS;
    }
    else if(ms > 200)
    {
        bsp_key_debounce_ms = 200;
    }
    else
    {
        bsp_key_debounce_ms = ms;
    }
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     获取当前按键消抖时间
// 返回参数     uint16       ms
//-------------------------------------------------------------------------------------------------------------------
uint16 bsp_key_get_debounce_ms (void)
{
    return bsp_key_debounce_ms;
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     动态修改长按判定时间（任务10 三级菜单可调参数）
// 参数说明     ms           长按判定时间(ms)，0 表示使用宏定义默认值
// 返回参数     void
//-------------------------------------------------------------------------------------------------------------------
void bsp_key_set_long_press_ms (uint16 ms)
{
    if(0 == ms)
    {
        bsp_key_long_press_ms = BSP_KEY_LONG_PRESS_MS;
    }
    else if(ms < 100)
    {
        bsp_key_long_press_ms = 100;
    }
    else
    {
        bsp_key_long_press_ms = ms;
    }
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     获取当前长按判定时间
// 返回参数     uint16       ms
//-------------------------------------------------------------------------------------------------------------------
uint16 bsp_key_get_long_press_ms (void)
{
    return bsp_key_long_press_ms;
}
