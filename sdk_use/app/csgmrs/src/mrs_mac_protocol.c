#include "hi_mdm.h"

HI_VOID mac_protocol_init(HI_VOID)
{
    HI_MDM_LoadCsgAndV100Protocol();

    HI_MDM_ZeroCrossAutoCheck();
    HI_MDM_PhaseIdentifyRegCsgAndV100Protocol();
    HI_MDM_PowerFailureCheckRegister();
    HI_MDM_LedCtrlRegister();

    /* 台区识别版本功能注册: */
    /*
    #if defined(PRODUCT_CFG_PRODUCT_TYPE_CCO) || defined(PRODUCT_CFG_PRODUCT_TYPE_STA)
    HI_MDM_LoadSoftTfSnrComponent();
    HI_MDM_LoadSoftTfNtbDistComponent();
    #endif
    */
}
HI_VOID mac_remote_init(HI_VOID)
{
    //Add by zhangyanan/00445172  2018/09/29
}