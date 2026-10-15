/*********************************************************************************************************************
* 文件名称          bsp_flash.c
* 功能简介          Flash 数据保存 BSP 驱动（任务8）
********************************************************************************************************************/
#include "bsp_flash.h"

#define BSP_FLASH_OFF_MAGIC             (0)                                     // 标志字下标
#define BSP_FLASH_OFF_LEN               (1)                                     // 数据长度下标
#define BSP_FLASH_OFF_DATA              (2)                                     // 数据起始下标

static uint32 bsp_flash_save_count = 0;                                         // 写入次数
static uint32 bsp_flash_load_count = 0;                                         // 读取次数

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     计算校验和
// 参数说明     data         数据地址
// 参数说明     len          数据个数
// 返回参数     uint32
//-------------------------------------------------------------------------------------------------------------------
static uint32 bsp_flash_checksum (const uint32 *data, uint16 len)
{
    uint32 sum = 0x12345678;
    uint16 i;

    for(i = 0; i < len; i++)
    {
        sum += data[i];
        sum ^= (data[i] << 1);
    }
    return sum;
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     把一组 32bit 数据保存到 Flash
// 参数说明     data         数据地址
// 参数说明     len          数据个数
// 返回参数     uint8        0-成功 其它-失败
//-------------------------------------------------------------------------------------------------------------------
uint8 bsp_flash_save (const uint32 *data, uint16 len)
{
    uint16 i;

    if((0 == data) || (0 == len) || (len > BSP_FLASH_DATA_LEN))
    {
        return 1;
    }

    flash_buffer_clear();                                                       // 先把缓冲区填成 0xFF

    flash_union_buffer[BSP_FLASH_OFF_MAGIC].uint32_type = BSP_FLASH_MAGIC;      // 写入标志字
    flash_union_buffer[BSP_FLASH_OFF_LEN].uint32_type   = len;                  // 写入数据长度

    for(i = 0; i < len; i++)
    {
        flash_union_buffer[BSP_FLASH_OFF_DATA + i].uint32_type = data[i];       // 写入数据
    }

    // 末尾写入校验和
    flash_union_buffer[BSP_FLASH_OFF_DATA + len].uint32_type = bsp_flash_checksum(data, len);

    flash_erase_sector(BSP_FLASH_SAVE_SECTOR, BSP_FLASH_SAVE_PAGE);             // 先擦除整个扇区

    if(0 != flash_write_page_from_buffer(BSP_FLASH_SAVE_SECTOR, BSP_FLASH_SAVE_PAGE))
    {
        return 2;
    }

    bsp_flash_save_count++;
    return 0;
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     从 Flash 读回一组 32bit 数据
// 参数说明     data         接收缓冲区
// 参数说明     len          期望读取的数据个数
// 返回参数     uint8        0-成功 其它-失败
//-------------------------------------------------------------------------------------------------------------------
uint8 bsp_flash_load (uint32 *data, uint16 len)
{
    uint16 i;
    uint16 save_len;
    uint32 sum;

    if((0 == data) || (0 == len) || (len > BSP_FLASH_DATA_LEN))
    {
        return 1;
    }

    flash_read_page_to_buffer(BSP_FLASH_SAVE_SECTOR, BSP_FLASH_SAVE_PAGE);      // 读取整页到缓冲区
    bsp_flash_load_count++;

    if(flash_union_buffer[BSP_FLASH_OFF_MAGIC].uint32_type != BSP_FLASH_MAGIC)  // 标志字不对 => 从没保存过
    {
        return 2;
    }

    save_len = (uint16)flash_union_buffer[BSP_FLASH_OFF_LEN].uint32_type;
    if(save_len != len)                                                         // 长度不匹配
    {
        return 3;
    }

    for(i = 0; i < len; i++)
    {
        data[i] = flash_union_buffer[BSP_FLASH_OFF_DATA + i].uint32_type;
    }

    sum = bsp_flash_checksum(data, len);                                        // 校验数据
    if(sum != flash_union_buffer[BSP_FLASH_OFF_DATA + len].uint32_type)
    {
        return 4;
    }

    return 0;
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     判断 Flash 中是否存在有效数据
// 返回参数     uint8        1-有有效数据 0-没有
//-------------------------------------------------------------------------------------------------------------------
uint8 bsp_flash_is_valid (void)
{
    flash_read_page_to_buffer(BSP_FLASH_SAVE_SECTOR, BSP_FLASH_SAVE_PAGE);
    return (flash_union_buffer[BSP_FLASH_OFF_MAGIC].uint32_type == BSP_FLASH_MAGIC) ? (1) : (0);
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     擦除参数区
// 返回参数     uint8        0-成功 其它-失败
//-------------------------------------------------------------------------------------------------------------------
uint8 bsp_flash_erase (void)
{
    return flash_erase_sector(BSP_FLASH_SAVE_SECTOR, BSP_FLASH_SAVE_PAGE);
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     获取 Flash 读/写 的次数统计
// 参数说明     save_count   输出参数，写入次数，可为 0
// 参数说明     load_count   输出参数，读取次数，可为 0
// 返回参数     void
//-------------------------------------------------------------------------------------------------------------------
void bsp_flash_get_count (uint32 *save_count, uint32 *load_count)
{
    if(0 != save_count)
    {
        *save_count = bsp_flash_save_count;
    }
    if(0 != load_count)
    {
        *load_count = bsp_flash_load_count;
    }
}
