/*********************************************************************************************************************
* 文件名称          bsp_pit.c
* 功能简介          定时器中断 PIT 驱动
********************************************************************************************************************/
#include "bsp_pit.h"
#include "app_pid.h"
#include "bsp_key.h"

static volatile uint32 bsp_pit_tick_ms = 0; // 系统节拍计数

// 初始化定时器
void bsp_pit_init (void)
{
    // TIM6: 1ms 系统节拍 (用于按键扫描、UI刷新)
    pit_ms_init(BSP_PIT_SYSTEM_INDEX, BSP_PIT_SYSTEM_MS);

    // TIM7: 5ms 控制环 (用于跑 PID、控制电机)
    // ★ 修正：宏名称改为 BSP_PIT_CONTROL_INDEX
    pit_ms_init(BSP_PIT_CONTROL_INDEX, BSP_PIT_CONTROL_MS); 
    
    bsp_pit_tick_ms = 0;
}

// 1ms 中断服务函数
void bsp_pit_system_isr (void)
{
    if(TIM_GetITStatus(BSP_PIT_SYSTEM_TIM, TIM_IT_Update) == RESET) return;
    TIM_ClearITPendingBit(BSP_PIT_SYSTEM_TIM, TIM_IT_Update);

    bsp_pit_tick_ms++;
    bsp_key_scan_1ms(); // 按键扫描
}

// 5ms 中断服务函数 (需要在 isr.c 里同步改名字)
void bsp_pit_control_isr (void)
{
    // ★ 修正：宏名称改为 BSP_PIT_CONTROL_TIM
    if(TIM_GetITStatus(BSP_PIT_CONTROL_TIM, TIM_IT_Update) == RESET) return;
    TIM_ClearITPendingBit(BSP_PIT_CONTROL_TIM, TIM_IT_Update);
    
    // 直接调用 PID 控制环
    app_pid_control_loop(); 
}

// 保留这两个时间函数
uint32 bsp_pit_get_tick_ms (void)
{
    return bsp_pit_tick_ms;
}

uint8 bsp_pit_check_interval (uint32 *last_ms, uint32 period_ms)
{
    uint32 now = bsp_pit_tick_ms;
    if((now - *last_ms) >= period_ms) {
        *last_ms = now;
        return 1;
    }
    return 0;
}