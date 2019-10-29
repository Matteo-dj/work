#include <hi_types.h>
#include <hi_mdm_types.h>
#include <hi_sal_nv.h>
#include <hi_ft_nv.h>
#include <hi_low_power.h >
#include <hi_hw_id.h>
#include <hi_mdm_nv.h>
#include <hi_mdm_sys.h>
//*****************************************************************************
// 函数名称: user_low_power_sleep_init
// 功能描述: 3911低功耗初始策略
//*****************************************************************************
HI_U32 usr_low_power_sleep_init(HI_VOID)
{
#ifndef PRODUCT_CFG_PRODUCT_TYPE_NDM
    HI_U32 ret = HI_ERR_SUCCESS;
    HI_NV_SAL_CONFIG_S stSalCfg = {{0}, {0}, 0};

    ret = HI_MDM_NV_Read(HI_NV_SAL_CONFIG, &stSalCfg, sizeof(stSalCfg));
    if((ret==HI_ERR_SUCCESS) && (stSalCfg.bSupportDynLowPower))
    {
        HI_LOW_POWER_SleepInit(HI_TRUE);
    }
    else
    {
        HI_LOW_POWER_SleepInit(HI_FALSE);
    }
    return ret;
#else
    HI_LOW_POWER_SleepInit(HI_FALSE);
    return HI_ERR_SUCCESS;

#endif

}

