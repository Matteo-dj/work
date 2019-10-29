#include <hi_types.h>
#include <hi_mdm_types.h>
#include <hi_battery.h>
#include <hi_product_types.h>
#include <hi_hw_id.h>
HI_U32 usr_battery_init(HI_VOID)
{
    //NDM形态上,非整表检测抄控器非发射机初始化电池模块。s
#ifdef PRODUCT_CFG_PRODUCT_TYPE_NDM
    HI_BOOL isMeterCheck=HI_MDM_IsMeterCheckProduct();
    HI_BOOL isTfhw=HI_DMS_IsTfHardWare();
    if (!isMeterCheck && !isTfhw)
    {
        return HI_BAT_Init();
    }
    return HI_ERR_SUCCESS;
#else
    return HI_ERR_SUCCESS;
#endif
}

