#include <hi_types.h>
#include "drv_dfx_cmn_inc.h"
#include <dfx_sal.h>//for DIAG_CMD_SDK_VER_IND_STRU
#include <hi_usr_nv.h>//for NV_MRS_USER_VER_S
#include <sal_diag.h>
#include <sal_common.h>
#include <hi_3518.h>

#if defined (PRODUCT_CFG_ICC_FMGR_SUPPORT) 
HI_U32  dfx_get_3518_ver(HI_U16 usId, HI_IN HI_PVOID pCmdParam, HI_U16 usCmdParamSize, HI_U32 ulOption)
{
    HI_U32 ret = HI_ERR_SUCCESS;
    HI_3518_UPG_VER_S stVer;
    HI_CHAR *pIndStr = HI_NULL;
    pIndStr = (HI_CHAR *)HI_MDM_Malloc(HI_MOD_ID_SAL_DFX,128);
    if(pIndStr==HI_NULL)
    {
        HI_DIAG_ReportPacket(usId,(HI_U8)ulOption,(HI_PBYTE)"\r\nnot enough memory",(HI_U16)strlen("not enough memory"),HI_TRUE);
        return HI_ERR_SUCCESS;
    }

    ret = HI_3518_GetVer(&stVer);
    if(ret!=HI_ERR_SUCCESS)
    {
        (hi_void)snprintf_s(pIndStr,128,128 - 1,"\r\nread ver fail=%u",ret);
        HI_DIAG_ReportPacket(usId,(HI_U8)ulOption,(HI_PBYTE)pIndStr,(HI_U16)strlen(pIndStr),HI_TRUE);
        HI_MDM_Free(HI_MOD_ID_SAL_DFX,pIndStr);
    }
    else
    {
        (hi_void)snprintf_s(pIndStr,128,128 - 1,"\r\ncorrect app=%u\r\nreal app=%u\r\nkernel=%s",PRODUCT_CFG_3518_APP_VER,stVer.app,stVer.kernel);
        HI_DIAG_ReportPacket(usId,(HI_U8)ulOption,(HI_PBYTE)pIndStr,(HI_U16)strlen(pIndStr),HI_TRUE);
        HI_MDM_Free(HI_MOD_ID_SAL_DFX,pIndStr);
    }
    return HI_ERR_SUCCESS;
}
#endif

HI_U32 sdkVerQry(HI_U16 usId, HI_IN HI_PVOID pCmdParam, HI_U16 usCmdParamSize, HI_U32 ulOption)
{
    HI_U32 ret = HI_ERR_SUCCESS;
    DIAG_CMD_SDK_VER_IND_STRU* pstAck = (DIAG_CMD_SDK_VER_IND_STRU* )HI_MDM_Malloc(HI_MOD_ID_SAL_DFX, sizeof(DIAG_CMD_SDK_VER_IND_STRU));

    if(pstAck == HI_NULL)
    {
        ret = HI_ERR_NOT_ENOUGH_MEMORY;
        return ret;
    }

    (hi_void)memset_s(pstAck, sizeof(DIAG_CMD_SDK_VER_IND_STRU), 0, sizeof(DIAG_CMD_SDK_VER_IND_STRU));
    pstAck->ucInterfaceVer = 0;
    pstAck->ucStruSize = sizeof(DIAG_CMD_SDK_VER_IND_STRU);
    ret = diag_GetSdkProductVer(pstAck->szSdkVersion);
    if(ret != HI_ERR_SUCCESS)
    {
        HI_MDM_Free(HI_MOD_ID_SAL_DFX, pstAck);
        return ret;
    }
    ret = HI_DIAG_ReportPacket(usId, (HI_U8)ulOption, (HI_PBYTE)pstAck, (HI_U16)sizeof(DIAG_CMD_SDK_VER_IND_STRU), HI_TRUE);
    HI_MDM_Free(HI_MOD_ID_SAL_DFX, pstAck);
    return ret;
}

HI_U32 newdiagVerQry(HI_U16 usId, HI_IN HI_PVOID pCmdParam, HI_U16 usCmdParamSize, HI_U32 ulOption)
{
#if defined(PRODUCT_CFG_SYS_FULL_VER_INFO_QRY)
    HI_U32 ret = HI_ERR_SUCCESS;
    DIAG_CMD_SOFT_NEW_VER_STRU* pstAck = (DIAG_CMD_SOFT_NEW_VER_STRU* )HI_MDM_Malloc(HI_MOD_ID_SAL_DFX, sizeof(DIAG_CMD_SOFT_NEW_VER_STRU));

    if(pstAck == HI_NULL)
    {
        ret = HI_ERR_NOT_ENOUGH_MEMORY;
        return ret;
    }
    diag_get_NewVerInfo(pstAck);
    ret = HI_DIAG_ReportPacket(usId, (HI_U8)ulOption, (HI_PBYTE)pstAck, (HI_U16)sizeof(DIAG_CMD_SOFT_NEW_VER_STRU), HI_TRUE);
    HI_MDM_Free(HI_MOD_ID_SAL_DFX, pstAck);
    return ret;
#else
    return HI_ERR_NOT_SUPPORT;
#endif
}

