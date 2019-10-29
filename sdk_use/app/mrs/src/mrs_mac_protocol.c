#include "hi_mdm.h"

HI_VOID mac_protocol_init(HI_VOID)
{
    HI_MDM_LoadSgAndV100Protocol();
    HI_MDM_ZeroCrossAutoCheck();
    HI_MDM_PhaseIdentifyRegSgAndV100Protocol();
    HI_MDM_PowerFailureCheckRegister();
    HI_MDM_LedCtrlRegister();
    #if defined(PRODUCT_CFG_PRODUCT_TYPE_CCO) || defined(PRODUCT_CFG_PRODUCT_TYPE_STA)
    HI_MDM_LoadSoftTfNtbDistComponent();
    #endif

    HI_MDM_NdmDetectIdRegister();

    /* 北京版本额外注册功能参考: (依次为北京抄控器协议、台区识别SNR组件、北京NTB台区识别协议) */
    HI_MDM_NdmSgConnectRegister();
    #if defined(PRODUCT_CFG_PRODUCT_TYPE_CCO) || defined(PRODUCT_CFG_PRODUCT_TYPE_STA)
    HI_MDM_LoadSoftTfSnrComponent();
    HI_MDM_LoadSoftTfNtbProtocolBeijing();
    #endif
}
HI_VOID mac_remote_init(HI_VOID)
{
    //Add by zhangyanan/00445172  2018/09/29
}
