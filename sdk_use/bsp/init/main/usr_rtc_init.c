#include <hi_types.h>
#include <hi_mdm_types.h>
#include <hi_rtc.h>
#include <hi_mdm_time.h>
#include <hi_product_types.h>
HI_U32 usr_rtc_init(HI_VOID)
{
#ifdef PRODUCT_CFG_PRODUCT_TYPE_NDM
    HI_U32 time = 0;

    if(HI_TRUE != HI_MDM_IsMeterCheckProduct())
    {
        (HI_VOID)HI_RTC_Init();//³­¿ØÆ÷RTC³õÊ¼»¯
    }

    if (HI_ERR_SUCCESS == HI_RTC_ReadTime(&time))
    {
        HI_MDM_SetRealTime(time + 60 * 60 * 8 );
    }
    
    return HI_ERR_SUCCESS;
#else
    return HI_ERR_SUCCESS;
#endif

}
