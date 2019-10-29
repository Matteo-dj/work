//*****************************************************************************
//
//                  版权所有 (C), 1998-2015, 华为技术有限公司
//
//*****************************************************************************
//  文 件 名   : drv_lattice.c
//  版 本 号   : V1.0
//  功能描述   : 显示&按键功能接口
//  函数列表   : TODO: ...
//  修改历史   :
//  1.日    期 : 2015-01-17
//     作    者 :
//     修改内容 : 创建文件
//*****************************************************************************
#include <hi_types.h>
#if defined(PRODUCT_CFG_SUPPORT_LCD)
#include <hi_driver.h>
#include <hi_lcd.h>
#include <hi_mdm.h>//提供打印接口
#include "lcd_driver.h"
#include "lattice_driver.h"
#include <hi_mdm_sem.h>
HI_U32 g_lattice_sem;
#define HI_LATTICE_LOCK(timeOut) HI_MDM_SEM_Wait(g_lattice_sem,timeOut)
#define HI_LATTICE_UNLOCK() HI_MDM_SEM_Signal(g_lattice_sem)


static const HI_U8 s_ascii_num_table[][LATTICE_LED_ROWS] =
{
    {0x00,0x18,0x24,0x24,0x24,0x24,0x24,0x18},/*"0",0*/
    {0x00,0x10,0x18,0x10,0x10,0x10,0x10,0x38},/*"1",1*/
    {0x00,0x18,0x24,0x20,0x10,0x08,0x04,0x3C},/*"2",2*/
    {0x00,0x18,0x24,0x20,0x18,0x20,0x24,0x18},/*"3",3*/
    {0x00,0x10,0x18,0x14,0x14,0x14,0x3C,0x10},/*"4",4*/
    {0x00,0x3C,0x04,0x04,0x1C,0x20,0x20,0x1C},/*"5",5*/
    {0x00,0x18,0x24,0x04,0x1C,0x24,0x24,0x18},/*"6",6*/
    {0x00,0x3C,0x24,0x20,0x10,0x08,0x08,0x08},/*"7",7*/
    {0x00,0x18,0x24,0x24,0x18,0x24,0x24,0x18},/*"8",8*/
    {0x00,0x18,0x24,0x24,0x38,0x20,0x24,0x18},/*"9",9*/
};
static const HI_U8 s_ascii_letter_table[][LATTICE_LED_ROWS] =
{
    {0x00,0x38,0x44,0x44,0x44,0x7C,0x44,0x44},//A
    {0x00,0x3C,0x44,0x44,0x3C,0x44,0x44,0x3C},//B
    {0x00,0x78,0x04,0x04,0x04,0x04,0x04,0x78},//C
    {0x00,0x3C,0x44,0x44,0x44,0x44,0x44,0x3C},//D
    {0x00,0x7C,0x04,0x04,0x7C,0x04,0x04,0x7C},//E
    {0x00,0x7C,0x04,0x04,0x7C,0x04,0x04,0x04},//F
    {0x00,0x38,0x44,0x04,0x04,0x74,0x44,0x38},//G
    {0x00,0x44,0x44,0x44,0x7C,0x44,0x44,0x44},//H
    {0x00,0x38,0x10,0x10,0x10,0x10,0x10,0x38},//I
    {0x00,0x38,0x10,0x10,0x10,0x10,0x14,0x18},//J
    {0x00,0x44,0x24,0x14,0x0C,0x14,0x24,0x44},//K
    {0x00,0x04,0x04,0x04,0x04,0x04,0x04,0x7C},//L
    {0x00,0xC6,0xAA,0xAA,0x92,0x82,0x82,0x82},//M
    {0x00,0x42,0x46,0x4A,0x52,0x62,0x42,0x00},//N
    {0x00,0x3C,0x42,0x42,0x42,0x42,0x42,0x3C},//O
    {0x00,0x3C,0x44,0x44,0x3C,0x04,0x04,0x04},//P
    {0x00,0x3C,0x42,0x42,0x42,0x52,0x62,0x7C},//Q
    {0x00,0x3C,0x44,0x44,0x3C,0x14,0x24,0x44},//R
    {0x00,0x38,0x44,0x04,0x38,0x40,0x44,0x38},//S
    {0x00,0xFE,0x10,0x10,0x10,0x10,0x10,0x10},//T
    {0x00,0x42,0x42,0x42,0x42,0x42,0x42,0x3C},//U
    {0x00,0xC6,0x44,0x44,0x44,0x28,0x28,0x10},//V
    {0x00,0xDB,0x5A,0x5A,0x5A,0x5A,0x7E,0x24},//W
    {0x00,0x42,0x24,0x18,0x18,0x24,0x42,0x00},//X
    {0x00,0x44,0x28,0x10,0x10,0x10,0x10,0x10},//Y
    {0x00,0x7C,0x40,0x20,0x10,0x08,0x04,0x7C},//Z
    {0x3C,0x04,0x04,0x04,0x04,0x04,0x04,0x3C},//[
    {0x01,0x02,0x04,0x08,0x10,0x20,0x40,0x80},/*\*/
    {0x3C,0x20,0x20,0x20,0x20,0x20,0x20,0x3C},//]
    {0x00,0x08,0x14,0x22,0x41,0x00,0x00,0x00},//^
    {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x7E},//_
    {0x08,0x10,0x20,0x40,0x00,0x00,0x00,0x00},//`
    {0x18,0x24,0x20,0x38,0x24,0x24,0x24,0x78},//a
    {0x04,0x04,0x1C,0x24,0x24,0x24,0x18,0x00},//b
    {0x18,0x24,0x04,0x04,0x04,0x04,0x24,0x18},//c
    {0x20,0x20,0x38,0x24,0x24,0x24,0x38,0x00},//d
    {0x00,0x18,0x24,0x24,0x3c,0x04,0x24,0x18},//e
    {0x30,0x48,0x08,0x3e,0x08,0x08,0x08,0x08},//f
    {0x18,0x24,0x24,0x38,0x20,0x28,0x24,0x18},//g
    {0x04,0x04,0x04,0x1c,0x24,0x24,0x24,0x24},//h
    {0x10,0x00,0x10,0x1c,0x10,0x10,0x50,0x30},//i
    {0x10,0x00,0x1c,0x10,0x10,0x10,0x10,0x0c},//j
    {0x04,0x04,0x1c,0x24,0x24,0x1c,0x0c,0x74},//k
    {0x08,0x08,0x08,0x08,0x08,0x08,0x48,0x30},//l
    {0x00,0x45,0xAA,0xAA,0x92,0x82,0x82,0x82},//m
    {0x00,0x04,0x3c,0x44,0x44,0x44,0x44,0x44},//n
    {0x00,0x38,0x44,0x44,0x44,0x44,0x38,0x00},//o
    {0x00,0x1c,0x24,0x24,0x24,0x1c,0x04,0x04},//p
    {0x0c,0x12,0x12,0x12,0x1c,0x50,0x30,0x10},//q
    {0x04,0x74,0x0c,0x04,0x04,0x04,0x04,0x04},//r
    {0x1c,0x22,0x02,0x04,0x18,0x20,0x22,0x1c},//s
    {0x08,0x08,0x08,0x3e,0x08,0x08,0x48,0x30},//t
    {0x00,0x24,0x24,0x24,0x24,0x78,0x00,0x00},//u
    {0x00,0x23,0x22,0x22,0x22,0x22,0x14,0x08},//v
    {0x00,0xDB,0x5A,0x5A,0x5A,0x5A,0x7E,0x24},//w
    {0xc3,0x24,0x18,0x18,0x18,0x18,0x24,0xc3},//x
    {0x22,0x22,0x24,0x28,0x30,0x10,0x08,0x06},//y
    {0x00,0x7e,0x22,0x10,0x08,0x44,0x7e,0x00},//z
};

static const HI_U8 s_ascii_special_char[][LATTICE_LED_ROWS] =
{
    {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},/*" ",0*/
    {0x00,0x00,0x00,0x00,0x3c,0x00,0x00,0x00},/*"-",1*/
    {0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF},/*0xFF,2全亮*/
};
HI_TIMER_HANDLE_S disp_timer;


HI_U32 HI_LATTICE_LED_PrintChar (HI_IN HI_LCD_POSITION_E char_num, HI_IN HI_CHAR ascii_data);
HI_U32 HI_LATTICE_LED_PrintPicture (HI_IN HI_U32 position,HI_IN HI_CHAR pic_data);
HI_U32 HI_LATTICE_LED_Clear(HI_VOID);
HI_U32 HI_LATTICE_LED_Blink (HI_IN HI_LATTICE_BLINK_EN blink_frequence);
HI_U32 HI_LATTICE_LowPower(HI_BOOL enable);
HI_U32 display_on_lattice(HI_IN HI_LCD_FUNC_ID_E func_id,HI_IN HI_LCD_DISPLAY_INFO_S *lcd_cfg, HI_BOOL enable);
HI_VOID HI_LED_Display_TimeFlush(HI_VOID);
HI_U32 HI_LED_Display_PriFlush(HI_VOID);


HI_PRV HI_VOID  disp_timer_isr(HI_U32 para)
{
    
    HI_MDM_EVENT_Send(g_lattice_event_handle,I2C_EVT_DISP_FLUSH_INT);
    return ;
}


HI_U32 HI_LATTICE_LED_PrintChar (HI_IN HI_LCD_POSITION_E char_num,
    HI_IN HI_CHAR ascii_data)
{
    HI_U32 ret;
    HI_CONST HI_U8 *pdata;
    HI_U8 i;
    HI_U8 data[2];
    HI_U8 addr;
    HI_U8 parity;

    //根据char_num选择基础地址
    if (HI_LCD_POSITION_1== char_num)
    {
        addr = HT16K33_I2C_ADDR1;
        parity = 0;
    }
    else if(HI_LCD_POSITION_2 == char_num)
    {
        addr = HT16K33_I2C_ADDR2;
        parity = 0;
    }
    else if(HI_LCD_POSITION_3 == char_num)
    {
        addr = HT16K33_I2C_ADDR2;
        parity = 1;
    }
    else
    {
        return HI_ERR_INVALID_PARAMETER;
    }

    if ((ascii_data >= '0')&&(ascii_data <= '9'))
    {
        pdata = &(s_ascii_num_table[ascii_data - '0'][0]);
    }
    else if ((ascii_data >= 'A')&&(ascii_data <= 'z'))
    {
        pdata = &(s_ascii_letter_table[ascii_data - 'A'][0]);
    }
    else if(ascii_data == '-')
    {
        pdata = &(s_ascii_special_char[1][0]);
    }
    else if((HI_U8)ascii_data == 0xEE)
    {
        pdata = &(s_ascii_special_char[2][0]);
    }
    else
    {
        pdata = &(s_ascii_special_char[0][0]);
    }

    for (i=0; i<LATTICE_LED_ROWS; i++)
    {
        data[0] = (HI_U8)(REG_DISPLAY_DATA_ADDR + 2*i + parity);
        data[1] = *pdata;
        ret = i2c_write_bytes(addr,data,2);
        if (HI_ERR_SUCCESS != ret)
        {
            return ret;
        }
        pdata ++;
    }

    return HI_ERR_SUCCESS;
}

HI_U32 HI_LATTICE_LED_PrintPicture (HI_IN HI_U32 position,HI_IN HI_CHAR pic_data)
{
    return HI_ERR_NOT_SUPPORT;
}

HI_U32 HI_LATTICE_LED_Clear(HI_VOID)
{
    HI_U32 ret;
    HI_U8 data2clr[LATTICE_CHIP_ROWS+1] = {0};//寄存器地址(0x0)和16字节数据

    ret = i2c_write_bytes(HT16K33_I2C_ADDR1,data2clr,LATTICE_CHIP_ROWS+1);
    if (HI_ERR_SUCCESS != ret)
    {
        return ret;
    }
    ret = i2c_write_bytes(HT16K33_I2C_ADDR2,data2clr,LATTICE_CHIP_ROWS+1);
    if (HI_ERR_SUCCESS != ret)
    {
        return ret;
    }

    return HI_ERR_SUCCESS;
}

HI_U32 HI_LATTICE_LED_Blink (HI_IN HI_LATTICE_BLINK_EN blink_frequence)
{
    HI_U32 ret;
    HI_U8 cmd;

    cmd = REG_DISPLAY_SET_ADDR|((unsigned int)blink_frequence<<1)|REG_DISPLAY_SET_STATUSON;
    ret = i2c_write_bytes(HT16K33_I2C_ADDR1,&cmd,1);
    if (HI_ERR_SUCCESS != ret)
    {
        return ret;
    }
    return HI_ERR_SUCCESS;
}

//因按键仅能唤醒1个芯片，暂不支持低功耗，仅降低点阵显示亮度。
HI_U32 HI_LATTICE_LowPower(HI_BOOL enable)
{
    HI_U32 ret;
    HI_U8 data;

#ifndef NDM_SUPPORT_LOWPOWER
    if (HI_TRUE == enable)
    {
        data = REG_PULSE_WIDTH_ADDR|REG_PULSE_WIDTH_01DUTY16;
    }
    else
    {
        data = REG_PULSE_WIDTH_ADDR|REG_PULSE_WIDTH_16DUTY16;
    }
#else
    if (HI_TRUE == enable)
    {
        //system clock disable
        data = REG_SYSTEM_SETUP_ADDR;
    }
    else
    {
        data = REG_SYSTEM_SETUP_ADDR|REG_SYSTEM_SETUP_TURNON;
    }
#endif
    ret = i2c_write_bytes(HT16K33_I2C_ADDR1,&data,1);
    if (HI_ERR_SUCCESS != ret)
    {
        return ret;
    }
    ret = i2c_write_bytes(HT16K33_I2C_ADDR2,&data,1);
    if (HI_ERR_SUCCESS != ret)
    {
        return ret;
    }
    return HI_ERR_SUCCESS;
}



//全局变量，保存各功能模块显示信息
HI_LATTICE_LCD_CFG_ST s_lcd_cfg;
/*
** 内部接口，找到要显示的id
** 原则:优先级最高且使能，如果存在多个，选当前传入func_id
*/
static HI_U32 find_2display_id(HI_LCD_FUNC_ID_E func_id,HI_LCD_FUNC_ID_E *todisp_id,HI_LCD_DISPLAY_PRI_E *pri)
{
    HI_U32 ret = HI_ERR_FAILURE;
    HI_LCD_FUNC_ID_E id;
    HI_LCD_FUNC_ID_E disp_id = HI_LCD_FUNC_ID_MAX;
    HI_LCD_DISPLAY_PRI_E highist_pri = HI_LATTICE_LED_PRI_MIN;
    HI_BOOL flag = HI_FALSE;

    if ((todisp_id == HI_NULL)||(pri == HI_NULL))
    {
        return HI_ERR_INVALID_PARAMETER;
    }
    for (id = HI_LATTICE_FUNC_ID_MIN; id < HI_LCD_FUNC_ID_MAX; id++)
    {
        if (s_lcd_cfg.enable[id] == HI_TRUE)
        {
            if (s_lcd_cfg.disp_info[id].pri >= highist_pri)
            {
                highist_pri = s_lcd_cfg.disp_info[id].pri;
                disp_id = id;
                flag = HI_TRUE;
            }
        }
    }
    if (HI_TRUE == flag)
    {
        if ((s_lcd_cfg.disp_info[func_id].pri == highist_pri)&&
            (s_lcd_cfg.enable[func_id] == HI_TRUE))
        {
            *todisp_id = func_id;
        }
        else
        {
            *todisp_id = disp_id;
        }
        *pri = highist_pri;
        return HI_ERR_SUCCESS;
    }
    return ret;
}


HI_U32 display_on_lattice(HI_IN HI_LCD_FUNC_ID_E func_id,HI_IN HI_LCD_DISPLAY_INFO_S *lcd_cfg, HI_BOOL enable)
{
    HI_LCD_DISPLAY_INFO_S *p_lcd_cfg;
    HI_LCD_FUNC_ID_E id;//显示功能计数
    HI_LCD_DISPLAY_PRI_E pri;
    HI_LCD_POSITION_E led_num;//显示点阵计数
    HI_U32 ret;

    s_lcd_cfg.enable[func_id] = enable;
    p_lcd_cfg = &(s_lcd_cfg.disp_info[func_id]);

 	/* 刷新缓存 */
    p_lcd_cfg->blink = lcd_cfg->blink;
    p_lcd_cfg->pri = lcd_cfg->pri;
    p_lcd_cfg->tsec = lcd_cfg->tsec;
    if (enable == HI_TRUE)
    {
        for (led_num =(HI_LCD_POSITION_E)0; led_num<HI_LCD_POSITION_MAX; led_num++)
        {
            if (lcd_cfg->data[led_num] != HI_LCD_NOT_FLUSH)
            {
                p_lcd_cfg->data[led_num] = lcd_cfg->data[led_num];
            }
        }
    }
    else
    {
        (hi_void)memset_s(p_lcd_cfg, sizeof(HI_LCD_DISPLAY_INFO_S), 0x0, sizeof(HI_LCD_DISPLAY_INFO_S));
    }

    /* 找到使能且最高优先级的id和优先级pri */
    ret = find_2display_id(func_id, &id, &pri);
    KEY_DBG_PRINT2("find id = %x pri = %x",id,pri);
    if (ret != HI_ERR_SUCCESS)
    {
        /* 找不到可显示id则清空显示并退出 */
        HI_LATTICE_LED_Blink(HI_LATTICE_BLINK_OFF);
        HI_LATTICE_LED_Clear();
    }
    else
    {
        /* 入参相同id则刷新 */
        if (id == s_lcd_cfg.current_id)
        {
            KEY_DBG_PRINT2("id = %x pri = %x",id,pri);
            /* 当前id，仅刷新需要更新，避免闪屏 */
            for (led_num=(HI_LCD_POSITION_E)0; led_num<HI_LCD_POSITION_MAX; led_num++)
            {
                if (lcd_cfg->data[led_num] != HI_LCD_NOT_FLUSH)
                {
                    HI_LATTICE_LED_PrintChar(led_num,s_lcd_cfg.disp_info[id].data[led_num]);
                    KEY_DBG_PRINT1("printchar:%c",s_lcd_cfg.disp_info[id].data[led_num]);
                }
            }
        }
        /* 不同id但优先级不小于当前显示id优先级则全部刷新 */
        else if (pri >= s_lcd_cfg.disp_info[s_lcd_cfg.current_id].pri)
        {
            KEY_DBG_PRINT2("id != %x pri = %x",id,pri);
            for (led_num=(HI_LCD_POSITION_E)0; led_num < HI_LCD_POSITION_MAX; led_num++)
            {
                HI_LATTICE_LED_PrintChar(led_num,s_lcd_cfg.disp_info[id].data[led_num]);
            }
            s_lcd_cfg.current_id = id;
        }
        else
        {
            /* 其他值直接返回 */
            return HI_ERR_SUCCESS;
        }
        /* 刷新闪烁 */
        if (s_lcd_cfg.disp_info[id].blink == HI_TRUE)
        {
            HI_LATTICE_LED_Blink(HI_LATTICE_BLINK_NORMAL);
        }
        else
        {
            HI_LATTICE_LED_Blink(HI_LATTICE_BLINK_OFF);
        }
    }
    return HI_ERR_SUCCESS;
}
HI_VOID HI_LED_Display_TimeFlush(HI_VOID)
{
    HI_LCD_FUNC_ID_E id;
    HI_LCD_DISPLAY_INFO_S lcd_cfg = {{0}, 0};

    if (HI_ERR_SUCCESS != HI_LATTICE_LOCK(HI_SYS_WAIT_FOREVER))
    {
        return;
    }
    for (id = HI_LATTICE_FUNC_ID_MIN; id<HI_LCD_FUNC_ID_MAX; id++)
    {
        if ((s_lcd_cfg.enable[id] == HI_TRUE)&&(s_lcd_cfg.disp_info[id].tsec!= 0))
        {
            s_lcd_cfg.disp_info[id].tsec --;
            if (0 == s_lcd_cfg.disp_info[id].tsec)
            {
                KEY_DBG_PRINT1("TimeFlush id = %x",id);
                (HI_VOID)display_on_lattice(id, &lcd_cfg, HI_FALSE);
            }
        }
    }
    (HI_VOID)HI_LATTICE_UNLOCK();
}

HI_U32 HI_LED_Display_PriFlush(HI_VOID)
{
    HI_LCD_FUNC_ID_E id;
    HI_LCD_DISPLAY_PRI_E current_pri;
    HI_U32 ret;

    ret = HI_LATTICE_LOCK(HI_SYS_WAIT_FOREVER);
    if (HI_ERR_SUCCESS != ret)
    {
        return ret;
    }
    current_pri = s_lcd_cfg.disp_info[s_lcd_cfg.current_id].pri;
    KEY_DBG_PRINT1("current pri = %x",current_pri);
    /* 寻找下一个相同优先级的id并显示 */
    for (id = (HI_LCD_FUNC_ID_E)(s_lcd_cfg.current_id + 1); id < HI_LCD_FUNC_ID_MAX;
        id++)
    {
        if ((s_lcd_cfg.enable[id] == HI_TRUE)&&(s_lcd_cfg.current_id != id))
        {
            KEY_DBG_PRINT2("PriFlush id = %x pri = %x",id,s_lcd_cfg.disp_info[id].pri);
            if (current_pri == s_lcd_cfg.disp_info[id].pri)
            {
                (HI_VOID)display_on_lattice(id, &(s_lcd_cfg.disp_info[id]),HI_TRUE);
                (HI_VOID)HI_LATTICE_UNLOCK();
                return HI_ERR_SUCCESS;
            }
        }
    }
    for (id = HI_LATTICE_FUNC_ID_MIN; id < s_lcd_cfg.current_id; id++)
    {
        if ((s_lcd_cfg.enable[id] == HI_TRUE)&&(s_lcd_cfg.current_id != id))
        {
            KEY_DBG_PRINT2("PriFlush id = %x pri = %x",id,s_lcd_cfg.disp_info[id].pri);
            if (current_pri == s_lcd_cfg.disp_info[id].pri)
            {
                (HI_VOID)display_on_lattice(id, &(s_lcd_cfg.disp_info[id]),HI_TRUE);
                (HI_VOID)HI_LATTICE_UNLOCK();
                return HI_ERR_SUCCESS;
            }
        }
    }
    (HI_VOID)HI_LATTICE_UNLOCK();
    return HI_ERR_SUCCESS;
}

HI_U32 HI_DRV_LCD_DisplayPrint(HI_IN HI_LCD_FUNC_ID_E func_id,HI_IN HI_LCD_DISPLAY_INFO_S *lcd_cfg)
{
    HI_U32 ret = HI_ERR_SUCCESS;

    if (lcd_cfg == HI_NULL)
    {
        return HI_ERR_INVALID_PARAMETER;
    }

    ret = HI_LATTICE_LOCK(HI_SYS_WAIT_FOREVER);
    if (HI_ERR_SUCCESS != ret)
    {
        return ret;
    }
    ret = display_on_lattice(func_id, lcd_cfg, HI_TRUE);
    (HI_VOID)HI_LATTICE_UNLOCK();

    return ret;
}
HI_U32 HI_DRV_LCD_DisplayDisable(HI_IN HI_LCD_FUNC_ID_E func_id)
{
    HI_U32 ret = HI_ERR_SUCCESS;
    HI_LCD_DISPLAY_INFO_S lcd_cfg;
    (hi_void)memset_s(&lcd_cfg, sizeof(HI_LCD_DISPLAY_INFO_S), 0x0, sizeof(HI_LCD_DISPLAY_INFO_S));
    ret = HI_LATTICE_LOCK(HI_SYS_WAIT_FOREVER);
    if (HI_ERR_SUCCESS != ret)
    {
        return ret;
    }
    ret = display_on_lattice(func_id, &lcd_cfg,HI_FALSE);
    (HI_VOID)HI_LATTICE_UNLOCK();

    return ret;
}

HI_U32 HI_LCD_ProcessEvt(HI_U32 ulEventBit)
{
    HI_U32 ret = HI_ERR_SUCCESS;
    static HI_U8 s_tsect = 0;

    if (I2C_EVT_IOEXP_INT & ulEventBit)
    {
        //读回IO信息
        //预期电平则执行回调函数
    }
    if (I2C_EVT_DISP_FLUSH_INT & ulEventBit)
    {
        s_tsect ++;
        HI_LED_Display_TimeFlush();
        if (s_tsect == HI_LATTICE_SAME_PRI_FLUSH_TIME)
        {
            s_tsect = 0;
            HI_LED_Display_PriFlush();
        }
    }    
    return ret;

}

HI_U32 HI_DRV_LCD_Init()
{
    HI_U32 ret = HI_ERR_SUCCESS;
    ret = HI_LATTICE_Init();
    if(ret!=HI_ERR_SUCCESS)
    {
        return ret;
    }
    HI_MDM_SEM_BCreate(&g_lattice_sem,"lcd",1);
    
    HI_LATTICE_LED_Clear();
    HI_MDM_TIMER_Start(&disp_timer,disp_timer_isr,LATTICE_LED_FLUSH_TIME,HI_TIMER_TYPE_PERIOD,0);
    return HI_ERR_SUCCESS;
}
#endif

