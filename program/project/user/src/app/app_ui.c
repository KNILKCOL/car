/*********************************************************************************************************************
* 文件名称          app_ui.c
* 功能简介          屏幕 UI 绘制组件（app 层）
********************************************************************************************************************/
#include "app_ui.h"
#include "bsp_lcd.h"
#include "bsp_pit.h"

#define APP_UI_COLOR_TITLE_PEN      (RGB565_BLACK)
#define APP_UI_COLOR_TITLE_BG       (RGB565_CYAN)
#define APP_UI_COLOR_STATUS_PEN     (RGB565_WHITE)
#define APP_UI_COLOR_STATUS_BG      (RGB565_BLUE)
#define APP_UI_COLOR_SEL_PEN        (RGB565_WHITE)
#define APP_UI_COLOR_SEL_BG         (RGB565_RED)
#define APP_UI_COLOR_NORMAL_PEN     (RGB565_BLACK)
#define APP_UI_COLOR_NORMAL_BG      (RGB565_WHITE)

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     UI 初始化
// 返回参数     void
//-------------------------------------------------------------------------------------------------------------------
void app_ui_init (void)
{
    bsp_lcd_clear(RGB565_WHITE);
    app_ui_title("CAR  CH32V307");
    app_ui_clear_body();
    app_ui_status("SYS READY");
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     绘制标题栏
// 参数说明     title        标题字符串
// 返回参数     void
//-------------------------------------------------------------------------------------------------------------------
void app_ui_title (const char *title)
{
    char   line[BSP_LCD_COLS + 1];
    uint16 len;

    if(0 == title)
    {
        return;
    }

    memset(line, ' ', BSP_LCD_COLS);
    line[BSP_LCD_COLS] = '\0';

    len = (uint16)strlen(title);
    if(len > BSP_LCD_COLS)
    {
        len = BSP_LCD_COLS;
    }
    memcpy(line, title, len);

    // 整行铺成标题背景色，形成"反白标题栏"效果
    bsp_lcd_fill_rect(0, (uint16)(BSP_LCD_TITLE_ROW * BSP_LCD_CH_H),
                      BSP_LCD_W, BSP_LCD_CH_H, APP_UI_COLOR_TITLE_BG);
    bsp_lcd_puts_color(0, BSP_LCD_TITLE_ROW, line, APP_UI_COLOR_TITLE_PEN, APP_UI_COLOR_TITLE_BG);
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     清空内容区
// 返回参数     void
//-------------------------------------------------------------------------------------------------------------------
void app_ui_clear_body (void)
{
    uint16 row;

    for(row = BSP_LCD_FIRST_ITEM_ROW; row <= BSP_LCD_MAX_ITEM_ROW; row++)
    {
        bsp_lcd_clear_row(row, APP_UI_COLOR_NORMAL_BG);
    }
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     在指定行显示字符串
// 参数说明     row          行号
// 参数说明     str          字符串
// 返回参数     void
//-------------------------------------------------------------------------------------------------------------------
void app_ui_text (uint16 row, const char *str)
{
    app_ui_text_color(row, str, APP_UI_COLOR_NORMAL_PEN, APP_UI_COLOR_NORMAL_BG);
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     在指定行显示带颜色的字符串
// 参数说明     row          行号
// 参数说明     str          字符串
// 参数说明     pen, bg      前景色 / 背景色
// 返回参数     void
//-------------------------------------------------------------------------------------------------------------------
void app_ui_text_color (uint16 row, const char *str, uint16 pen, uint16 bg)
{
    char   line[BSP_LCD_COLS + 1];
    uint16 i;
    uint16 len;

    if((row >= BSP_LCD_ROWS) || (0 == str))
    {
        return;
    }

    // 先按屏幕宽度截断，避免字符串超长导致显示错位
    memset(line, ' ', BSP_LCD_COLS);
    line[BSP_LCD_COLS] = '\0';

    len = (uint16)strlen(str);
    if(len > BSP_LCD_COLS)
    {
        len = BSP_LCD_COLS;
    }
    memcpy(line, str, len);

    bsp_lcd_clear_row(row, bg);
    bsp_lcd_puts_color(0, row, line, pen, bg);

    // 补齐剩余部分，保证整行背景色一致
    for(i = len; i < BSP_LCD_COLS; i++)
    {
        bsp_lcd_puts_color(i, row, " ", pen, bg);
    }
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     显示一行菜单项
// 参数说明     row          行号
// 参数说明     label        名称
// 参数说明     value        值字符串，可为 0
// 参数说明     selected     1-当前选中(高亮) 0-普通
// 返回参数     void
//-------------------------------------------------------------------------------------------------------------------
void app_ui_menu_item (uint16 row, const char *label, const char *value, uint8 selected)
{
    char   line[BSP_LCD_COLS + 1];
    uint16 len;
    uint16 max_label;
    uint16 i;
    uint16 pen = APP_UI_COLOR_NORMAL_PEN;
    uint16 bg  = APP_UI_COLOR_NORMAL_BG;

    if((row >= BSP_LCD_ROWS) || (0 == label))
    {
        return;
    }

    if(selected)
    {
        pen = APP_UI_COLOR_SEL_PEN;
        bg  = APP_UI_COLOR_SEL_BG;
    }

    // 组装成 ">NAME      VALUE " 这样的一行文本
    memset(line, ' ', BSP_LCD_COLS);
    line[BSP_LCD_COLS] = '\0';

    line[0] = selected ? ('>') : (' ');

    // 有值时给值留出 6 个字符的位置，避免名称和值在缓冲区里重叠
    max_label = (BSP_LCD_COLS - 2) - ((0 != value) ? (6) : (0));

    len = (uint16)strlen(label);
    if(len > max_label)
    {
        len = max_label;
    }
    memcpy(&line[1], label, len);

    if(0 != value)
    {
        len = (uint16)strlen(value);
        if(len > 6)
        {
            len = 6;
        }
        i = (uint16)(BSP_LCD_COLS - len);
        memcpy(&line[i], value, len);
    }

    app_ui_text_color(row, line, pen, bg);
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     显示一行"名称 + 整数"信息
// 参数说明     row          行号
// 参数说明     label        名称
// 参数说明     value        数值
// 返回参数     void
//-------------------------------------------------------------------------------------------------------------------
void app_ui_value_int (uint16 row, const char *label, int32 value)
{
    char line[BSP_LCD_COLS + 1];
    char num[12];
    uint16 label_len;
    uint16 num_len;

    if((row >= BSP_LCD_ROWS) || (0 == label))
    {
        return;
    }

    sprintf(num, "%d", (int)value);
    num_len = (uint16)strlen(num);

    memset(line, ' ', BSP_LCD_COLS);
    line[BSP_LCD_COLS] = '\0';

    label_len = (uint16)strlen(label);
    if(label_len > (uint16)(BSP_LCD_COLS - num_len))
    {
        label_len = (uint16)(BSP_LCD_COLS - num_len);
    }
    memcpy(line, label, label_len);
    memcpy(&line[BSP_LCD_COLS - num_len], num, num_len);

    app_ui_text(row, line);
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     绘制状态栏
// 参数说明     str          状态字符串
// 返回参数     void
//-------------------------------------------------------------------------------------------------------------------
void app_ui_status (const char *str)
{
    if(0 == str)
    {
        return;
    }

    // 走 app_ui_text_color 的统一路径：先填充状态栏底色，再截断到一行 16 个字符后显示
    app_ui_text_color(BSP_LCD_STATUS_ROW, str, APP_UI_COLOR_STATUS_PEN, APP_UI_COLOR_STATUS_BG);
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     居中显示一条提示信息
// 参数说明     line1        第一行
// 参数说明     line2        第二行，可为 0
// 返回参数     void
//-------------------------------------------------------------------------------------------------------------------
void app_ui_message (const char *line1, const char *line2)
{
    app_ui_clear_body();

    if(0 != line1)
    {
        app_ui_text(3, line1);
    }
    if(0 != line2)
    {
        app_ui_text(5, line2);
    }
}
