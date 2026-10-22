#ifndef _bsp_pit_h_
#define _bsp_pit_h_

#include "bsp_board.h"

// 统一初始化入口
void    bsp_pit_init            (void);

// 中断服务函数（由 isr.c 调用）
void    bsp_pit_system_isr      (void);
void    bsp_pit_control_isr     (void);

// 时间获取与计时判断
uint32  bsp_pit_get_tick_ms     (void);
uint8   bsp_pit_check_interval  (uint32 *last_ms, uint32 period_ms);

#endif