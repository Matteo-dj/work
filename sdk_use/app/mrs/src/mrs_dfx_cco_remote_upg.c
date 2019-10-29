//*****************************************************************************
//
//                  版权所有 (C), 1998-2011, 华为技术有限公司
//
//*****************************************************************************
//  文 件 名   : mrs_dfx_cco_remote_upg.c
//  版 本 号   : V1.0 
//  作    者   : fengxiaomin/f00209182
//  生成日期   : 2014-1-15
//  功能描述   : 远程升级模块函数及接口定义
//               
//  函数列表   : TODO: ...
//  修改历史   : 
//  1.日    期 : 2014-1-15
//    作    者 : fengxiaomin/f00209182 
//    修改内容 : 创建文件 
//
//*****************************************************************************
#include "mrs_common.h"
#include "dfx_sys.h"
#include "mrs_fw_tools.h"
#include "mrs_dfx.h"
#include "dfx_sys_sdm.h"
#include "mrs_fw_proto376_2_echo.h"
#include "mrs_dfx_cco_remote_upg.h"
#include "mrs_srv_cco_remote_upg.h"
#include "mrs_srv_cco_macmap.h"
#include "mrs_srv_parallel_cco.h"
#include "mrs_srv_cco.h"
#include "mrs_time.h"


#if defined(PRODUCT_CFG_PRODUCT_TYPE_CCO)

MRS_CCO_REMOTE_UPG_DFX_S g_DfxRemoteUpgCtx;

HI_DIAG_SYS_STAT_OBJ_S g_stMrsUpgObjTbl = {HI_DSID_APP_REMOTE_UPG_INF,
                                            HI_SYS_DSID_BUSINESS_L0,
                                            HI_DSID_TYPE_QRY,
                                            1,
                                            sizeof(g_DfxRemoteUpgCtx.stRemoteUpgInfo),
                                            &g_DfxRemoteUpgCtx.stRemoteUpgInfo};


MRS_CCO_REMOTE_UPG_DFX_S* mrsDfxGetRemoteUpg(HI_VOID)
{
    return &g_DfxRemoteUpgCtx;
}

HI_U32 mrsDfxRemoteUpgInit(HI_VOID)
{
    HI_U32 ret = HI_ERR_SUCCESS;
    
    (hi_void)memset_s(&g_DfxRemoteUpgCtx, sizeof(g_DfxRemoteUpgCtx), 0, sizeof(g_DfxRemoteUpgCtx));
#ifndef SAL_HAVE_OS_WIN_VER
    //注册统计量
    ret = HI_MDM_RegisterSysStat(&g_stMrsUpgObjTbl,1);
#endif

    return ret;
}

HI_VOID mrsDfxRemoteUpgReset(HI_VOID)
{
    MRS_CCO_REMOTE_UPG_DFX_S *pstDfxRemoteUpgCtx = mrsDfxGetRemoteUpg();
    HI_U32 ulClearMrsUpgTime = pstDfxRemoteUpgCtx->stRemoteUpgInfo.ulClearMrsUpgTime;
    HI_U8 ucMrsRxClearUpgStatus = pstDfxRemoteUpgCtx->stRemoteUpgInfo.ucMrsRxClearUpgStatus;
    
    (hi_void)memset_s(pstDfxRemoteUpgCtx, sizeof(MRS_CCO_REMOTE_UPG_DFX_S), 0, sizeof(MRS_CCO_REMOTE_UPG_DFX_S));

    pstDfxRemoteUpgCtx->stRemoteUpgInfo.ulClearMrsUpgTime = ulClearMrsUpgTime;
    pstDfxRemoteUpgCtx->stRemoteUpgInfo.ucMrsRxClearUpgStatus = ucMrsRxClearUpgStatus;

    return;
}

HI_U8 mrsDfxGetUpgStatus(HI_VOID)
{
    HI_U8 ucMrsUpgStatus = MRS_DFX_CCO_REMOTE_UPG_PREPARE;
    
    if (MRS_CCO_WORK_SWITCH_LOCAL_READ_METER == mrsGetCcoWorkFlag())
    {
        ucMrsUpgStatus = MRS_DFX_CCO_LOCAL_READ_METER_PERIOD;
    }
    else if (MRS_CCO_WORK_SWITCH_SEARCH_METER == mrsGetCcoWorkFlag())
    {
        ucMrsUpgStatus = MRS_DFX_CCO_PLC_SEARCH_PERIOD;
    }
    else if (MRS_CCO_WORK_SWITCH_REMOTE_UPGRADE == mrsGetCcoWorkFlag())
    {
        ucMrsUpgStatus = MRS_DFX_CCO_REMOTE_UPG_PERIOD;
    }

    return ucMrsUpgStatus;
}

HI_VOID mrsDfxRefreshClearUpg(HI_U8 ucFileFlag)
{
    MRS_CCO_REMOTE_UPG_DFX_S *pstDfxRemoteUpgCtx = mrsDfxGetRemoteUpg();

    if (MRS_CCO_CLEAR_UPG_LOAD_FILE != ucFileFlag)
    {
        return;
    }
    
    pstDfxRemoteUpgCtx->stRemoteUpgInfo.ulClearMrsUpgTime = HI_MDM_GetRealTime();
    pstDfxRemoteUpgCtx->stRemoteUpgInfo.ucMrsRxClearUpgStatus = mrsDfxGetUpgStatus();
    return;
}

HI_VOID mrsDfxMrsStartUpgFileTime(HI_VOID)
{
    MRS_CCO_REMOTE_UPG_DFX_S *pstDfxRemoteUpgCtx = mrsDfxGetRemoteUpg();

    pstDfxRemoteUpgCtx->stRemoteUpgInfo.ulMrsUpgStartTime = HI_MDM_GetRealTime();
    return;
}

HI_VOID mrsDfxMrsEndUpgFileTime(HI_VOID)
{
    MRS_CCO_REMOTE_UPG_DFX_S *pstDfxRemoteUpgCtx = mrsDfxGetRemoteUpg();

    pstDfxRemoteUpgCtx->stRemoteUpgInfo.ulMrsUpgEndTime = HI_MDM_GetRealTime();
    return;
}

HI_VOID mrsDfxUpgStartTime(HI_VOID)
{
    MRS_CCO_REMOTE_UPG_DFX_S *pstDfxRemoteUpgCtx = mrsDfxGetRemoteUpg();

    pstDfxRemoteUpgCtx->stRemoteUpgInfo.ulUpgStartTime = HI_MDM_GetRealTime();
    return;
}

HI_VOID mrsDfxUpgEndTime(HI_VOID)
{
    MRS_CCO_REMOTE_UPG_DFX_S *pstDfxRemoteUpgCtx = mrsDfxGetRemoteUpg();

    pstDfxRemoteUpgCtx->stRemoteUpgInfo.ulUpgStopTime = HI_MDM_GetRealTime();
    return;
}

HI_VOID mrsDfxStartUpgRxStop(HI_VOID)
{
    MRS_CCO_REMOTE_UPG_DFX_S *pstDfxRemoteUpgCtx = mrsDfxGetRemoteUpg();

    pstDfxRemoteUpgCtx->stRemoteUpgInfo.ucStartUpgRxStopUpg++;
    return;
}

HI_VOID mrsDfxRefreshUpgStatus(HI_U8 ucUpgStatus)
{
    MRS_CCO_REMOTE_UPG_DFX_S *pstDfxRemoteUpgCtx = mrsDfxGetRemoteUpg();

    pstDfxRemoteUpgCtx->stRemoteUpgInfo.ucMrsUpgStatus = ucUpgStatus;
    return;
}

HI_VOID mrsDfxRefreshFrameErrFlag(HI_U16 usUpgStatus)
{
    MRS_CCO_REMOTE_UPG_DFX_S *pstDfxRemoteUpgCtx = mrsDfxGetRemoteUpg();

    pstDfxRemoteUpgCtx->stRemoteUpgInfo.usMrsFrameErrFlag |= usUpgStatus;
    return;
}

HI_VOID mrsDfxUpgStatusRxErr(HI_U8 ucStatus)
{
    MRS_CCO_REMOTE_UPG_DFX_S *pstDfxRemoteUpgCtx = mrsDfxGetRemoteUpg();
    
    pstDfxRemoteUpgCtx->stRemoteUpgInfo.ucMrsRxErrUpgStatus = ucStatus;
    
    return;
}

HI_VOID mrsDfxGetUpgMemoryFail(HI_VOID)
{
    MRS_CCO_REMOTE_UPG_DFX_S *pstDfxRemoteUpgCtx = mrsDfxGetRemoteUpg();

    pstDfxRemoteUpgCtx->stRemoteUpgInfo.ucMrsGetUpgMomeryFail = MRS_DFX_UPG_GET_RELEASE_MEMORY_FAIL;

    return;
}

HI_VOID mrsDfxReleaseMemoryFail(HI_VOID)
{
    MRS_CCO_REMOTE_UPG_DFX_S *pstDfxRemoteUpgCtx = mrsDfxGetRemoteUpg();

    pstDfxRemoteUpgCtx->stRemoteUpgInfo.ucMrsReleaseMemFail = MRS_DFX_UPG_GET_RELEASE_MEMORY_FAIL;

    return;
}

HI_VOID mrsDfxGetUpgFileFlag(HI_U8 ucUpgFileFlag)
{
    MRS_CCO_REMOTE_UPG_DFX_S *pstDfxRemoteUpgCtx = mrsDfxGetRemoteUpg();
    HI_U8 ucFlag = 0;
    
    if (MRS_CCO_LOCAL_CCO_UPGRADE == ucUpgFileFlag)
    {
        ucFlag = MRS_DFX_CCO_UPG_LOCAL_CCO;
    }
    else if (MRS_CCO_BOARDCAST_UPGRADE == ucUpgFileFlag)
    {
        ucFlag = MRS_DFX_CCO_UPG_CCO_STA_BOARDCAST;
    }
    else if (MRS_CCO_STA_UPGRADE == ucUpgFileFlag)
    {
        ucFlag = MRS_DFX_CCO_UPG_ALL_STA;
    }

    pstDfxRemoteUpgCtx->stRemoteUpgInfo.ucUpgFileType = ucFlag;
}

HI_VOID mrsDfxUpgCmdRet(HI_U8 ucCmd, HI_U32 ulRet, HI_U32 ulId)
{
    MRS_CCO_REMOTE_UPG_DFX_S *pstDfxRemoteUpgCtx = mrsDfxGetRemoteUpg();
    HI_U8 ucRet = MRS_DFX_CCO_UPG_RET_SCCESS;

    HI_UNREF_PARAM(ulId);

    if (ulRet != HI_ERR_SUCCESS)
    {
        ucRet = MRS_DFX_CCO_UPG_RET_FAIL;
    }

    if (MRS_DFX_CCO_UPG_CMD_START == ucCmd)
    {
        pstDfxRemoteUpgCtx->stRemoteUpgInfo.ucUpgStartRet = ucRet;
    }
    else if (MRS_DFX_CCO_UPG_CMD_STOP == ucCmd)
    {
        pstDfxRemoteUpgCtx->stRemoteUpgInfo.ucUpgStopRet = ucRet;
    }

    return;
}

HI_VOID mrsDfxUpgFileErrLen(HI_U32 ulFileLen)
{
    MRS_CCO_REMOTE_UPG_DFX_S *pstDfxRemoteUpgCtx = mrsDfxGetRemoteUpg();
    
    pstDfxRemoteUpgCtx->stRemoteUpgInfo.ulMrsUpgLenErrSize = ulFileLen;

    return;
}

HI_VOID mrsDfxRxUpgFrameNum(HI_VOID)
{
    MRS_CCO_REMOTE_UPG_DFX_S *pstDfxRemoteUpgCtx = mrsDfxGetRemoteUpg();
    
    pstDfxRemoteUpgCtx->stRemoteUpgInfo.usMrsRxUpgFrameNum++;

    return;
}

HI_VOID mrsDfxTxUpgFrameNum(HI_VOID)
{
    MRS_CCO_REMOTE_UPG_DFX_S *pstDfxRemoteUpgCtx = mrsDfxGetRemoteUpg();
    
    pstDfxRemoteUpgCtx->stRemoteUpgInfo.usMrsTxUpgFrameNum++;

    return;
}

HI_VOID mrsDfxRxMsgTotalNum(HI_VOID)
{
    MRS_CCO_REMOTE_UPG_DFX_S *pstDfxRemoteUpgCtx = mrsDfxGetRemoteUpg();
    
    pstDfxRemoteUpgCtx->stRemoteUpgInfo.usMrsMsgRxTotalTimes++;

    return;
}

HI_VOID mrsDfxTxMrsMsgNum(HI_VOID)
{
    MRS_CCO_REMOTE_UPG_DFX_S *pstDfxRemoteUpgCtx = mrsDfxGetRemoteUpg();
    
    pstDfxRemoteUpgCtx->stRemoteUpgInfo.usMrsMsgTxMrsTimes++;

    return;
}

HI_VOID mrsDfxRxMsgIdErrNum(HI_VOID)
{
    MRS_CCO_REMOTE_UPG_DFX_S *pstDfxRemoteUpgCtx = mrsDfxGetRemoteUpg();
    
    pstDfxRemoteUpgCtx->stRemoteUpgInfo.usMrsMsgRxIdErrTimes++;

    return;
}

HI_VOID mrsDfxRxUpgMsgNum(HI_VOID)
{
    MRS_CCO_REMOTE_UPG_DFX_S *pstDfxRemoteUpgCtx = mrsDfxGetRemoteUpg();
    
    pstDfxRemoteUpgCtx->stRemoteUpgInfo.usMsgRxUpgTimes++;

    return;
}

HI_VOID mrsDfxRxUpgMsgStart(HI_U32 ulRet)
{
    MRS_CCO_REMOTE_UPG_DFX_S *pstDfxRemoteUpgCtx = mrsDfxGetRemoteUpg();
    pstDfxRemoteUpgCtx->stRemoteUpgInfo.ulMrsMsgRxStartRet = ulRet;
    pstDfxRemoteUpgCtx->stRemoteUpgInfo.ulMrsMsgRxStartTime = HI_MDM_GetRealTime();

    return;
}

HI_VOID mrsDfxRxUpgIdErrMsgNum(HI_VOID)
{
    MRS_CCO_REMOTE_UPG_DFX_S *pstDfxRemoteUpgCtx = mrsDfxGetRemoteUpg();
    
    pstDfxRemoteUpgCtx->stRemoteUpgInfo.usMsgRxUpgIdErrTimes++;

    return;
}

HI_VOID mrsDfxRxUpgIdInd(HI_U32 ulStatus, HI_U32 ulIndUpgId, HI_U32 ulUpgId, HI_U8 ucPeriod)
{
    MRS_CCO_REMOTE_UPG_DFX_S *pstDfxRemoteUpgCtx = mrsDfxGetRemoteUpg();
    
    pstDfxRemoteUpgCtx->stRemoteUpgInfo.ulIndRxStatus = ulStatus;
    pstDfxRemoteUpgCtx->stRemoteUpgInfo.ulIndUpgId = ulIndUpgId;
    pstDfxRemoteUpgCtx->stRemoteUpgInfo.ulIndUpgTs = HI_MDM_GetMilliSeconds();
    pstDfxRemoteUpgCtx->stRemoteUpgInfo.ucPeriod = ucPeriod;
    pstDfxRemoteUpgCtx->stRemoteUpgInfo.ulUpgId = ulUpgId;
}

HI_VOID mrsDfxTxUpgMsgNum(HI_VOID)
{
    MRS_CCO_REMOTE_UPG_DFX_S *pstDfxRemoteUpgCtx = mrsDfxGetRemoteUpg();
    
    pstDfxRemoteUpgCtx->stRemoteUpgInfo.usMsgTxUpgTimes++;

    return;
}

HI_VOID mrsDfxRxDrvMsgNum(HI_VOID)
{
    MRS_CCO_REMOTE_UPG_DFX_S *pstDfxRemoteUpgCtx = mrsDfxGetRemoteUpg();
    
    pstDfxRemoteUpgCtx->stRemoteUpgInfo.ucMsgRxDrvTimes++;

    return;
}

HI_VOID mrsDfxTxDrvMsgNum(HI_VOID)
{
    MRS_CCO_REMOTE_UPG_DFX_S *pstDfxRemoteUpgCtx = mrsDfxGetRemoteUpg();
    
    pstDfxRemoteUpgCtx->stRemoteUpgInfo.ucMsgTxDrvTimes++;

    return;
}

HI_VOID mrsDfxTxMsgFailInfo(HI_U16 usId, HI_U32 ulRet)
{
    MRS_CCO_REMOTE_UPG_DFX_S *pstDfxRemoteUpgCtx = mrsDfxGetRemoteUpg();
    
    pstDfxRemoteUpgCtx->stRemoteUpgInfo.usMsgTxFailId |= usId;
    pstDfxRemoteUpgCtx->stRemoteUpgInfo.ucMsgTxFailNum++;

    mrsDfxRefreshLastDataU32(pstDfxRemoteUpgCtx->stRemoteUpgInfo.pMsgFailRet, 
        MRS_TOOLS_ALEN(pstDfxRemoteUpgCtx->stRemoteUpgInfo.pMsgFailRet), ulRet);
}

HI_U16 mrsDfxGetFrameErrFlag(HI_VOID)
{
    MRS_CCO_REMOTE_UPG_DFX_S *pstDfxRemoteUpgCtx = mrsDfxGetRemoteUpg();

    return pstDfxRemoteUpgCtx->stRemoteUpgInfo.usMrsFrameErrFlag;
}

HI_VOID mrsDfxSendMsgFail(HI_U16 usId, HI_U16 usRet)
{
    mrsDfxTxMsgFailInfo(usId, usRet);
}


#endif
/* END:   Added by fengxiaomin/00209182, 2014/1/16 */

