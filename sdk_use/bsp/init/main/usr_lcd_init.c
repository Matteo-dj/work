#include <hi_types.h>
#include <hi_mdm_types.h>
#include <hi_lcd.h>
#include <hi_hw_id.h>
#include <hi_product_types.h>

HI_U32 usr_lcd_init(HI_VOID)
{
    //NDM形态上,非整表检测抄控器初始化电池模块。s
#ifdef PRODUCT_CFG_PRODUCT_TYPE_NDM
    HI_BOOL isMeterCheck=HI_MDM_IsMeterCheckProduct();
    if (!isMeterCheck)
    {
        return HI_DRV_LCD_Init();
    }
    return HI_ERR_SUCCESS;
#else
    return HI_ERR_SUCCESS;
#endif

}

