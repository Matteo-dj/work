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

    /* �����汾����ע�Ṧ�ܲο�: (����Ϊ����������Э�顢̨��ʶ��SNR���������NTB̨��ʶ��Э��) */
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
