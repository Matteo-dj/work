#include <hi_types.h>
#include <hi_mdm_types.h>
#include <hi_ndm.h>
#include <hi_lcd.h>
#include "hi_ioexp.h"

//*****************************************************************************
//功能说明: 3518版本不正确时，通过该接口进行的灯和点阵显示的告警
//用户可以通过删除该函数中实现屏蔽点阵和LED告警。
//*****************************************************************************
HI_U32 HI_BOARD_3518VerErr(HI_VOID)
{
    HI_LCD_DISPLAY_INFO_S lcd_cfg = {{0}, 0};

    lcd_cfg.data[0] = 'E';
    lcd_cfg.data[1] = '0';
    lcd_cfg.data[2] = '2';
    lcd_cfg.pri = HI_LCD_DISPLAY_PRI_0;
    lcd_cfg.tsec = HI_LCD_DISPLAY_FOREVER;
    (HI_VOID)HI_DRV_LCD_DisplayPrint(HI_LCD_FUNC_ID_VER_ALERT, &lcd_cfg);
    if (!HI_DMS_IsTfHardWare())
    {
#ifdef PRODUCT_CFG_SUPPORT_IO_EXP
        HI_IO_EXP_SetOutPutVal(IO_EXP_IO_NUM_05,HI_IO_EXP_OUT_VAL_HIGHT);
#endif
    }
    return HI_ERR_SUCCESS;
}

