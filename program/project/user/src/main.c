/*********************************************************************************************************************
* CH32V307VCT6 Opensourec Library 即（CH32V307VCT6 开源库）是一个基于官方 SDK 接口的第三方开源库
* Copyright (c) 2022 SEEKFREE 逐飞科技
*
* 本文件是CH32V307VCT6 开源库的一部分
*
* CH32V307VCT6 开源库 是免费软件
* 您可以根据自由软件基金会发布的 GPL（GNU General Public License，即 GNU通用公共许可证）的条款
* 即 GPL 的第3版（即 GPL3.0）或（您选择的）任何后来的版本，重新发布和/或修改它
*
* 本开源库的发布是希望它能发挥作用，但并未对其作任何的保证
* 甚至没有隐含的适销性或适合特定用途的保证
* 更多细节请参见 GPL
*
* 您应该在收到本开源库的同时收到一份 GPL 的副本
* 如果没有，请参阅<https://www.gnu.org/licenses/>
*
* 额外注明：
* 本开源库使用 GPL3.0 开源许可证协议 以上许可申明为译文版本
* 许可申明英文版在 libraries/doc 文件夹下的 GPL3_permission_statement.txt 文件中
* 许可证副本在 libraries 文件夹下 即该文件夹下的 LICENSE 文件
* 欢迎各位使用并传播本程序 但修改内容时必须保留逐飞科技的版权声明（即本声明）
*
* 文件名称          main
* 公司名称          成都逐飞科技有限公司
* 版本信息          查看 libraries/doc 文件夹内 version 文件 版本说明
* 开发环境          MounRiver Studio V1.8.1
* 适用平台          CH32V307VCT6
* 店铺链接          https://seekfree.taobao.com/
*
* 修改记录
* 日期                                      作者                             备注
* 2022-09-15        大W            first version
* 2026-10-10        zk             软件一轮考核 任务2~11 实现
********************************************************************************************************************/
#include "zf_common_headfile.h"

#include "bsp_board.h"                                                          // BSP 层：硬件宏定义
#include "app_main.h"                                                           // app 层：页面管理 + 主循环

/*
 * 程序结构说明（对应本次考核要求）：
 *
 *   bsp 层（user/inc/bsp、user/src/bsp）—— 所有涉及硬件初始化的代码都放在这里
 *       bsp_board.h   全部引脚 / 定时器 / 中断 / 周期的宏定义（唯一配置源）
 *       bsp_system.c  统一初始化入口
 *       bsp_led.c     LED 指示灯                 （任务3 / 任务6）
 *       bsp_key.c     按键读取 + 消抖 + 长短按   （任务4 / 任务9 / 任务10）
 *       bsp_exti.c    按键外部中断               （任务9）
 *       bsp_pwm.c     PWM + 无源蜂鸣器           （任务5）
 *       bsp_pit.c     定时器中断 1ms 节拍 / LED 翻转中断（任务6）
 *       bsp_lcd.c     SPI 屏幕                   （任务7）
 *       bsp_flash.c   Flash 数据存取             （任务8）
 *
 *   app 层（user/inc/app、user/src/app）—— 纯业务逻辑，不直接操作寄存器
 *       app_ui.c      屏幕 UI 组件（标题栏 / 菜单项 / 状态栏）
 *       app_param.c   参数表 + 掉电保存
 *       app_menu.c    三级菜单                   （任务10）
 *       app_task.c    任务3~任务9 的演示页面
 *       app_main.c    页面调度 + 主循环          （任务11：不重新烧录切换全部任务）
 *
 *   main() 只做两件事：初始化一次，然后不停跑主循环。
 */
int main (void)
{
    app_main_init();                    // BSP 初始化 + 应用初始化（上电直接进入任务总菜单）

    while(1)
    {
        app_main_loop();                // 处理按键事件 + 周期刷新当前页面
    }
}

