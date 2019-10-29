#include <hi_types.h>
#include <hi_mdm_types.h>
#include <hi_ndm.h>
#include <hi_lcd.h>
#include "hi_ioexp.h"

//*****************************************************************************
//����˵��: 3518�汾����ȷʱ��ͨ���ýӿڽ��еĵƺ͵�����ʾ�ĸ澯
//�û�����ͨ��ɾ���ú�����ʵ�����ε����LED�澯��
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

