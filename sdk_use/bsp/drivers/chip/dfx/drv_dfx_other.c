#include <hi_types.h>
#include "drv_dfx_cmn_inc.h"
#include "drv_dfx_other.h"

#ifdef PRODUCT_CFG_PRODUCT_TYPE_NDM

HI_U32  dfx_get_work_mode(HI_U16 usId, HI_IN HI_PVOID pCmdParam, HI_U16 usCmdParamSize, HI_U32 ulOption)
{
    HI_U32 ret = HI_ERR_FAILURE;
    HI_DFX_NDM_MODE_CFG stInd = {0};
    HI_NV_FTM_STARTUP_CFG_S stStartCfg = {0};

    ret = HI_NVM_Read(HI_NV_FTM_STARTUP_CFG_ID, &stStartCfg, sizeof(HI_NV_FTM_STARTUP_CFG_S));
    if(ret != HI_ERR_SUCCESS)
    {
        stInd.ret = ret;
        HI_DIAG_ReportPacket(usId,(HI_U8)ulOption,(HI_PBYTE)(&stInd),sizeof(HI_DFX_NDM_MODE_CFG),HI_TRUE);
        return ret;
    }

    stInd.ndm_work_mode = stStartCfg.ulStatus;
    stInd.ret = HI_ERR_SUCCESS;
    HI_DIAG_ReportPacket(usId,(HI_U8)ulOption,(HI_PBYTE)(&stInd),sizeof(HI_DFX_NDM_MODE_CFG),HI_TRUE);

    return HI_ERR_SUCCESS;
}

HI_U32  dfx_set_work_mode(HI_U16 usId, HI_IN HI_PVOID pCmdParam, HI_U16 usCmdParamSize, HI_U32 ulOption)
{
    HI_U32 ret = HI_ERR_FAILURE;
    HI_DFX_NDM_MODE_CFG * pReq = (HI_DFX_NDM_MODE_CFG *)pCmdParam;
    HI_NV_FTM_STARTUP_CFG_S stStartCfg = {0};
    HI_DFX_NDM_MODE_CFG stInd = {0};

    if(HI_NULL == pReq)
    {
        return HI_ERR_FAILURE;
    }

    if((HI_ND_SYS_STATUS_NORMAL != pReq->ndm_work_mode)
        &&(HI_ND_SYS_STATUS_UPGRADE != pReq->ndm_work_mode)
        &&(HI_ND_SYS_STATUS_TEST != pReq->ndm_work_mode)
        &&(HI_ND_SYS_STATUS_DIAG != pReq->ndm_work_mode)
        &&(HI_ND_SYS_STATUS_UNKNOWN != pReq->ndm_work_mode))
    {
        return HI_ERR_INVALID_PARAMETER;
    }

    stInd.ndm_work_mode = pReq->ndm_work_mode;

    ret = HI_NVM_Read(HI_NV_FTM_STARTUP_CFG_ID, &stStartCfg, sizeof(HI_NV_FTM_STARTUP_CFG_S));
    if(ret != HI_ERR_SUCCESS)
    {
        stInd.ret = ret;
        HI_DIAG_ReportPacket(usId,(HI_U8)ulOption,(HI_PBYTE)(&stInd),sizeof(HI_DFX_NDM_MODE_CFG),HI_TRUE);
        return ret;
    }

    stStartCfg.ulStatus = pReq->ndm_work_mode;

    ret = HI_NVM_WriteFactory(HI_NV_FTM_STARTUP_CFG_ID, &stStartCfg, sizeof(HI_NV_FTM_STARTUP_CFG_S));
    if(ret != HI_ERR_SUCCESS)
    {
        stInd.ret = ret;
        HI_DIAG_ReportPacket(usId,(HI_U8)ulOption,(HI_PBYTE)(&stInd),sizeof(HI_DFX_NDM_MODE_CFG),HI_TRUE);
        return ret;
    }

    HI_DIAG_ReportPacket(usId,(HI_U8)ulOption,(HI_PBYTE)(&stInd),sizeof(HI_DFX_NDM_MODE_CFG),HI_TRUE);

    return HI_ERR_SUCCESS;
}



#endif

/* END:   Added by x00180816, 2014/3/4 */

