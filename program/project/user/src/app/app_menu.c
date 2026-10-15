/*********************************************************************************************************************
* 文件名称          app_menu.c
* 功能简介          三级菜单（任务10）
********************************************************************************************************************/
#include "app_menu.h"
#include "app_ui.h"
#include "app_main.h"
#include "app_param.h"
#include "bsp_pit.h"

#define PAGE_MENU_NAME              "T10 3-LEVEL MENU"

// 菜单当前所处的层级
typedef enum
{
    APP_MENU_LEVEL_GROUP = 0,                                                   // 第 1 级：选参数组
    APP_MENU_LEVEL_ITEM,                                                        // 第 2 级：选参数项
    APP_MENU_LEVEL_EDIT,                                                        // 第 3 级：编辑参数值
}app_menu_level_enum;

static app_menu_level_enum app_menu_level = APP_MENU_LEVEL_GROUP;
static uint8 app_menu_group = 0;                                                // 第 1 级光标
static uint8 app_menu_item  = 0;                                                // 第 2 级光标
static int32 app_menu_edit_backup = 0;                                          // 第 3 级进入前的值（用于放弃修改）

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     三级菜单：进入
// 返回参数     void
//-------------------------------------------------------------------------------------------------------------------
static void app_menu_init (void)
{
    app_menu_level = APP_MENU_LEVEL_GROUP;
    app_menu_group = 0;
    app_menu_item  = 0;
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     三级菜单：整页重绘
// 返回参数     void
//-------------------------------------------------------------------------------------------------------------------
static void app_menu_draw (void)
{
    char val[24];
    uint8 i;
    app_param_group_struct *group;
    app_param_item_struct  *item;

    app_ui_title(PAGE_MENU_NAME);
    app_ui_clear_body();

    switch(app_menu_level)
    {
        //------------------------------------------ 第 1 级：参数组 ------------------------------------------
        case APP_MENU_LEVEL_GROUP:
        {
            for(i = 0; i < APP_PARAM_GROUP_NUM; i++)
            {
                group = app_param_get_group(i);
                app_ui_menu_item((uint16)(BSP_LCD_FIRST_ITEM_ROW + i), group->name, 0,
                                 (i == app_menu_group) ? (1) : (0));
            }
            app_ui_text(5, "LEVEL 1/3  GROUP");
            app_ui_text(6, "PARAMS SAVED");
            app_ui_status("M:ENTER  K1:BACK");
            break;
        }

        //------------------------------------------ 第 2 级：参数项 ------------------------------------------
        case APP_MENU_LEVEL_ITEM:
        {
            group = app_param_get_group(app_menu_group);
            for(i = 0; i < APP_PARAM_PER_GROUP; i++)
            {
                item = &group->item[i];
                sprintf(val, "%d", (int)item->value);
                app_ui_menu_item((uint16)(BSP_LCD_FIRST_ITEM_ROW + i), item->name, val,
                                 (i == app_menu_item) ? (1) : (0));
            }
            sprintf(val, "GROUP:%s", group->name);
            app_ui_text(5, val);
            app_ui_text(6, "LEVEL 2/3  ITEM");
            app_ui_status("M:EDIT  K1:BACK");
            break;
        }

        //------------------------------------------ 第 3 级：编辑 --------------------------------------------
        case APP_MENU_LEVEL_EDIT:
        default:
        {
            item = app_param_get_item(app_menu_group, app_menu_item);

            sprintf(val, "%s  (L3 EDIT)", item->name);
            app_ui_text(1, val);

            sprintf(val, "%d", (int)item->value);
            app_ui_menu_item(3, "VALUE", val, 1);

            sprintf(val, "MIN=%-4d MAX=%d", (int)item->min, (int)item->max);
            app_ui_text(4, val);

            sprintf(val, "STEP=%-3d DEF=%d", (int)item->step, (int)item->def_value);
            app_ui_text(5, val);

            app_ui_text(6, "LEVEL 3/3  EDIT");
            app_ui_text(7, "HOLD MID=DEFAULT");
            app_ui_status("U/R+ D/L- M:SAVE");
            break;
        }
    }
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     三级菜单：周期刷新
// 返回参数     void
//-------------------------------------------------------------------------------------------------------------------
static void app_menu_update (void)
{
    // 菜单内容只在按键时改变，这里不重复刷新（避免闪烁）
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     三级菜单：按键处理
// 返回参数     void
//-------------------------------------------------------------------------------------------------------------------
static void app_menu_key (bsp_key_enum key, bsp_key_state_enum state)
{
    app_param_item_struct *item;
    uint8 redraw = 0;

    if(BSP_KEY_STATE_RELEASE == state)
    {
        return;
    }

    switch(app_menu_level)
    {
        //------------------------------------------ 第 1 级：参数组 ------------------------------------------
        case APP_MENU_LEVEL_GROUP:
        {
            if(BSP_KEY_UP == key)
            {
                if(app_menu_group > 0)
                {
                    app_menu_group--;
                }
                redraw = 1;
            }
            else if(BSP_KEY_DOWN == key)
            {
                if(app_menu_group < (APP_PARAM_GROUP_NUM - 1))
                {
                    app_menu_group++;
                }
                redraw = 1;
            }
            else if(BSP_KEY_MID == key)
            {
                app_menu_item  = 0;
                app_menu_level = APP_MENU_LEVEL_ITEM;                           // 进入第 2 级
                app_task_beep(30);
                redraw = 1;
            }
            else if(BSP_KEY_KEY1 == key)
            {
                app_main_goto_main();
                return;
            }
            break;
        }

        //------------------------------------------ 第 2 级：参数项 ------------------------------------------
        case APP_MENU_LEVEL_ITEM:
        {
            if(BSP_KEY_UP == key)
            {
                if(app_menu_item > 0)
                {
                    app_menu_item--;
                }
                redraw = 1;
            }
            else if(BSP_KEY_DOWN == key)
            {
                if(app_menu_item < (APP_PARAM_PER_GROUP - 1))
                {
                    app_menu_item++;
                }
                redraw = 1;
            }
            else if(BSP_KEY_MID == key)
            {
                item = app_param_get_item(app_menu_group, app_menu_item);
                app_menu_edit_backup = item->value;                             // 备份，便于放弃修改
                app_menu_level = APP_MENU_LEVEL_EDIT;                           // 进入第 3 级
                app_task_beep(30);
                redraw = 1;
            }
            else if(BSP_KEY_KEY1 == key)
            {
                app_menu_level = APP_MENU_LEVEL_GROUP;                          // 返回第 1 级
                app_task_beep(20);
                redraw = 1;
            }
            break;
        }

        //------------------------------------------ 第 3 级：编辑 --------------------------------------------
        case APP_MENU_LEVEL_EDIT:
        default:
        {
            if((BSP_KEY_UP == key) || (BSP_KEY_RIGHT == key))
            {
                app_param_step_value(app_menu_group, app_menu_item, 1);         // 加一个步长
                redraw = 1;
            }
            else if((BSP_KEY_DOWN == key) || (BSP_KEY_LEFT == key))
            {
                app_param_step_value(app_menu_group, app_menu_item, -1);        // 减一个步长
                redraw = 1;
            }
            else if(BSP_KEY_MID == key)
            {
                item = app_param_get_item(app_menu_group, app_menu_item);

                if(BSP_KEY_STATE_LONG == state)
                {
                    // 长按中键：恢复该参数默认值
                    app_param_set_value(app_menu_group, app_menu_item, item->def_value);
                    app_task_beep(80);
                }
                else
                {
                    // 短按中键：保存到 Flash 并让参数立即生效，然后返回第 2 级
                    app_param_apply_to_bsp();
                    app_param_save();
                    app_menu_level = APP_MENU_LEVEL_ITEM;
                    app_task_beep(50);
                }
                redraw = 1;
            }
            else if(BSP_KEY_KEY1 == key)
            {
                // 放弃修改：恢复进入第 3 级之前的值
                app_param_set_value(app_menu_group, app_menu_item, app_menu_edit_backup);
                app_menu_level = APP_MENU_LEVEL_ITEM;
                app_task_beep(20);
                redraw = 1;
            }
            break;
        }
    }

    if(redraw)
    {
        app_menu_draw();
    }
}

const app_page_struct app_menu_page =
{
    PAGE_MENU_NAME, app_menu_init, app_menu_draw, app_menu_update, app_menu_key
};
