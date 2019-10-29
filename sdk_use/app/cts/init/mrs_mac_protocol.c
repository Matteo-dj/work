#include "hi_mdm.h"

HI_VOID mac_protocol_init(HI_VOID)
{
    HI_MDM_LoadSgAndV100Protocol();

    HI_MDM_ZeroCrossAutoCheck();
    HI_MDM_TfpRegSgAndV100Protocol();
    HI_MDM_PhaseIdentifyRegSgAndV100Protocol();
    HI_MDM_PowerFailureCheckRegister();
    HI_MDM_LedCtrlRegister();
}
HI_VOID mac_remote_init(HI_VOID)
{
    //Add by zhangyanan/00445172  2018/09/29
}