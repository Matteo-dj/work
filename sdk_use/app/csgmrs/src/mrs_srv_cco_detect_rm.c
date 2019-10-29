//*****************************************************************************
//
//                  版权所有 (C), 1998-2016, 华为技术有限公司
//
//*****************************************************************************
//  文 件 名   : mrs_srv_cco_detect_rm.c
//  版 本 号   : V1.0 
//  作    者   : fengxiaomin/00209182
//  生成日期   : 2016-01-13
//  功能描述   : 采集器入网探测抄表功能函数及接口
//               
//  函数列表   : TODO: ...
//  修改历史   : 
//  1.日    期 : 2016-01-19
//    作    者 : fengxiaomin/00209182
//    修改内容 : 创建文件 
//*****************************************************************************

//*****************************************************************************
// PROJECT   : 
// SUBSYSTEM :
// MODULE    :  
// OWNER     :  
//*****************************************************************************

#include "mrs_common.h"
#include "mrs_cmd_msg.h"
#include "hi_mdm_types.h"
#include "app_mrs.h"
#include "mrs_fw_tools.h"
#include "mrs_fw_log.h"
#include "mrs_fw_n.h"
#include "mrs_fw_proto376_2_echo.h"
#include "mrs_fw_proto376_2_frame.h"
#include "mrs_fw_proto645.h"
#include "mrs_srv_common.h"
#include "mrs_srv_cco_macmap.h"
#include "mrs_srv_queue.h"
#include "mrs_srv_cco_macmap.h"
#include "mrs_srv_cco_archives.h"
#include "mrs_fw_nv.h"
#include "mrs_srv_anytime_readmeter.h"
#include "mrs_srv_cco.h"
#include "mrs_srv_cco_detect_rm.h"
#include "mrs_srv_cco_detect_rm_n.h"
#include "mrs_dfx.h"

#if defined(PRODUCT_CFG_PRODUCT_TYPE_CCO)

typedef struct
{
    HI_U8 aucMacAddr[HI_PLC_MAC_ADDR_LEN];    //表地址
    HI_U8 result;      //结果
    HI_U8 padding;
}MRS_DETECT_WHITE_FIND_RESULT, *P_MRS_DETECT_WHITE_FIND_RESULT;

MRS_CCO_DETECT_RM_CTX_S g_stDetectRmCtx;

MRS_CCO_DETECT_RM_CTX_S* mrsCcoGetDetectRmCtx(HI_VOID)
{
    return &g_stDetectRmCtx;
}

HI_U32 mrsCcoDetectRmInit(HI_VOID)
{
    (hi_void)memset_s(&g_stDetectRmCtx, sizeof(g_stDetectRmCtx), 0, sizeof(g_stDetectRmCtx));
    
    mrsSetDetectRmStatus(MRS_CCO_DETECT_RM_IDLE);
    mrsCcoDetectRmNvInit();
    mrsSrvInitQueue(&g_stDetectRmCtx.stWhiteListQueue.stQueue);
	mrsSrvInitList(&g_stDetectRmCtx.stBlackList.stList);
    HI_MDM_RegisterReportRejectNode(mrsCcoRejectNodeCallback);

    return HI_ERR_SUCCESS;
}

HI_U32 mrsCcoDetectRmNvInit(HI_VOID)
{
    HI_U32 ulRet = HI_ERR_SUCCESS;
    NV_APP_MRS_STATECHG_REPORT_CFG_STRU stNv;    
    MRS_CCO_DETECT_RM_CTX_S* pstRmCtx = mrsCcoGetDetectRmCtx();
    
    (hi_void)memset_s(&stNv, sizeof(stNv), 0, sizeof(stNv));
    ulRet = HI_MDM_NV_Read(ID_NV_APP_STATE_CHG_REPORT_CFG, &stNv, sizeof(NV_APP_MRS_STATECHG_REPORT_CFG_STRU));
    if (HI_ERR_SUCCESS != ulRet)
    {
        stNv.ucDetectEnable = MRS_CCO_DETECT_ENABLE_DEFAULT;
        stNv.usWhiteAddMaxTime = MRS_CCO_WHITE_ADD_MAX_TIME_DEFAULT;
        stNv.ucRmTurn = MRS_CCO_DETECT_TURN_DEFAULT;
        stNv.aus97DI[0] = MRS_CCO_DETECT_DI_VER1997A;
        stNv.aus97DI[1] = MRS_CCO_DETECT_DI_VER1997B;
        stNv.ul07DI = MRS_CCO_DETECT_DI_VER2007;
        stNv.ucTopoWaitMaxTime = MRS_CCO_ALLOW_WHITE_MAX_TIME_DEFAULT;
        stNv.ucJoinNetMaxTime = MRS_CCO_STA_JOIN_NET_MAX_TIME_DEFAULT;
        stNv.ucBlackValidTime = MRS_CCO_BLACK_LIST_VALID_TIME_DEFAULT;
        stNv.ucStaJoinWhiteTime = MRS_CCO_JOIN_WHITE_LIST_TIME_DEFAULT;
        stNv.ucSingleMeterTime = MRS_CCO_METER_DETECT_TIME_DEFAULT;
        pstRmCtx->stParam.ucTurnTime = MRS_CCO_DETECT_TURN_TIME_DEFAULT;
    }
    
    pstRmCtx->stParam.ucEnable = stNv.ucDetectEnable ? 1 : 0;
    mrsDfxCcoDetectEnable(pstRmCtx->stParam.ucEnable);

    pstRmCtx->stParam.usTimeOut = stNv.usWhiteAddMaxTime;
    if (!pstRmCtx->stParam.usTimeOut)
    {
        pstRmCtx->stParam.usTimeOut = MRS_CCO_WHITE_ADD_MAX_TIME_DEFAULT;
    }

    pstRmCtx->stParam.ucMaxTurn = stNv.ucRmTurn;
    MRS_DATA_VALID_CHECK(pstRmCtx->stParam.ucMaxTurn, 3, MRS_CCO_DETECT_TURN_DEFAULT, 1);

    pstRmCtx->stParam.ul07DI = stNv.ul07DI;
    pstRmCtx->stParam.aus97DI[0] = stNv.aus97DI[0];
    pstRmCtx->stParam.aus97DI[1] = stNv.aus97DI[1];
    pstRmCtx->stParam.ucTopoWaitMaxTime = stNv.ucTopoWaitMaxTime;
    pstRmCtx->stParam.ucJoinNetMaxTime = stNv.ucJoinNetMaxTime;
    pstRmCtx->stParam.ucBlackValidTime = stNv.ucBlackValidTime;
    pstRmCtx->stParam.ucStaJoinWhiteTime = stNv.ucStaJoinWhiteTime;
    pstRmCtx->stParam.ucTurnTime = stNv.ucTurnTime;
    pstRmCtx->stParam.ucSingleMeterTime = stNv.ucSingleMeterTime;
    if (!pstRmCtx->stParam.ucSingleMeterTime)
    {
        pstRmCtx->stParam.ucSingleMeterTime = MRS_CCO_METER_DETECT_TIME_DEFAULT;
    }

    return ulRet;
}

HI_U8 mrsGetDetectRmEnable(HI_VOID)
{
    return mrsCcoGetDetectRmCtx()->stParam.ucEnable;
}

HI_VOID mrsSetDetectRmEnable(HI_U8 ucEnable)
{
    mrsCcoGetDetectRmCtx()->stParam.ucEnable = ucEnable;
    mrsDfxCcoDetectEnable(ucEnable);
}

HI_VOID mrsSetDetectRmStatus(HI_U8 ucStatus)
{
    mrsCcoGetDetectRmCtx()->stRunning.ucStatus = ucStatus;
    mrsDfxCcoDetectStatus(ucStatus);
}

HI_U8 mrsGetDetectRmStatus(HI_VOID)
{
    return mrsCcoGetDetectRmCtx()->stRunning.ucStatus;
}

HI_U8 mrsGetDetectRmExterStatus(HI_VOID)
{
    return mrsCcoGetDetectRmCtx()->stRunning.ucExternFlag;
}

HI_VOID mrsSetDetectRmExterStatus(HI_U8 ucFlag)
{
    mrsCcoGetDetectRmCtx()->stRunning.ucExternFlag = ucFlag;
    mrsDfxCcoDetectExternFlag(ucFlag);
}

HI_U8 mrsGetDetectOriginalEnable(HI_VOID)
{
    return mrsCcoGetDetectRmCtx()->stRunning.ucOriginalEnable;
}

HI_VOID mrsSetDetectOriginalEnable(HI_U8 ucEnable)
{
    mrsCcoGetDetectRmCtx()->stRunning.ucOriginalEnable = ucEnable;
    mrsDfxCcoDetectOriginalEnable(ucEnable);
}

HI_VOID mrsSetDetectRmFlag(HI_U8 ucFlag)
{
    mrsCcoGetDetectRmCtx()->stRunning.ucRmFlag = ucFlag;
    mrsDfxCcoDetectRmFlag(ucFlag);
}

HI_U8 mrsGetDetectRmFlag(HI_VOID)
{
    return mrsCcoGetDetectRmCtx()->stRunning.ucRmFlag;
}

HI_VOID mrsResetDetectRmDIIndex(HI_VOID)
{
    mrsCcoGetDetectRmCtx()->stRunning.uc97DIIndex = 0;
    mrsDfxCcoDetect07DIIndex(mrsCcoGetDetectRmCtx()->stRunning.uc97DIIndex);
}

HI_VOID mrsDetectRmDIIndexAddSelf(HI_VOID)
{
    mrsCcoGetDetectRmCtx()->stRunning.uc97DIIndex++;
    mrsDfxCcoDetect07DIIndex(mrsCcoGetDetectRmCtx()->stRunning.uc97DIIndex);
}

HI_U8 mrsGetDetectRmDIIndex(HI_VOID)
{
    return mrsCcoGetDetectRmCtx()->stRunning.uc97DIIndex;
}

HI_VOID mrsResetDetectRmTurn(HI_VOID)
{
    mrsCcoGetDetectRmCtx()->stRunning.ucCurrTurn = 0;
    mrsDfxCcoDetectCurrTurn(0);
}

HI_VOID mrsDetectRMTurnAddSelf(HI_VOID)
{
    mrsCcoGetDetectRmCtx()->stRunning.ucCurrTurn++;
    mrsDfxCcoDetectCurrTurn(mrsCcoGetDetectRmCtx()->stRunning.ucCurrTurn);
}

HI_VOID mrsCacheMacNumAddSelf(HI_VOID)
{
    mrsCcoGetDetectRmCtx()->stRunning.ucCacheMacNum++;
}

HI_VOID mrsResetCacheMacInfo(HI_VOID)
{
    mrsCcoGetDetectRmCtx()->stRunning.ucCacheMacNum = 0;
    (hi_void)memset_s(mrsCcoGetDetectRmCtx()->stRunning.pMacBuf,
        sizeof(mrsCcoGetDetectRmCtx()->stRunning.pMacBuf),0,
        sizeof(mrsCcoGetDetectRmCtx()->stRunning.pMacBuf));
}

HI_U8 mrsGetCacheMacNum(HI_VOID)
{
    return mrsCcoGetDetectRmCtx()->stRunning.ucCacheMacNum;
}

HI_U16 mrsGetDetectBlackListNum(HI_VOID)
{
    return mrsCcoGetDetectRmCtx()->stBlackList.usNum;
}

HI_VOID mrsDetectBlackListAddSelf(HI_VOID)
{
    mrsCcoGetDetectRmCtx()->stBlackList.usNum++;
    mrsDfxCcoDetectBlackNum(mrsCcoGetDetectRmCtx()->stBlackList.usNum);
}

HI_VOID mrsDetectBlackListDecSelf(HI_VOID)
{
    mrsCcoGetDetectRmCtx()->stBlackList.usNum--;
    mrsDfxCcoDetectBlackNum(mrsCcoGetDetectRmCtx()->stBlackList.usNum);
}

HI_U16 mrsGetDetectWhiteListNum(HI_VOID)
{
    return mrsCcoGetDetectRmCtx()->stWhiteListQueue.usNum;
}

HI_VOID mrsDetectWhiteListAddSelf(HI_VOID)
{
    mrsCcoGetDetectRmCtx()->stWhiteListQueue.usNum++;
    mrsDfxCcoDetectWhiteNum(mrsCcoGetDetectRmCtx()->stWhiteListQueue.usNum);
}

HI_VOID mrsResetDetectWhiteListNum(HI_VOID)
{
    mrsCcoGetDetectRmCtx()->stWhiteListQueue.usNum = 0;
    mrsDfxCcoDetectWhiteNum(mrsCcoGetDetectRmCtx()->stWhiteListQueue.usNum);
}

HI_U16 mrsGetDetectWhiteListValidNum(HI_VOID)
{
    return mrsCcoGetDetectRmCtx()->stWhiteListQueue.usValidNum;
}

HI_VOID mrsDetectWhiteListValidAddSelf(HI_VOID)
{
    mrsCcoGetDetectRmCtx()->stWhiteListQueue.usValidNum++;
    mrsDfxCcoDetectWhiteValidNum(mrsCcoGetDetectRmCtx()->stWhiteListQueue.usValidNum);
}

HI_VOID mrsResetDetectWhiteListValidNum(HI_VOID)
{
    mrsCcoGetDetectRmCtx()->stWhiteListQueue.usValidNum = 0;
    mrsDfxCcoDetectWhiteValidNum(mrsCcoGetDetectRmCtx()->stWhiteListQueue.usValidNum);
}

HI_VOID mrsSetAllowJoinWhiteFlag(HI_U8 ucFlag)
{
    mrsCcoGetDetectRmCtx()->stRunning.ucJoinWhiteFlag = ucFlag;
    mrsDfxCcoDetectJoinWhiteFlag(ucFlag);
}

HI_U8 mrsGetAllowJoinWhiteFlag(HI_VOID)
{
    return mrsCcoGetDetectRmCtx()->stRunning.ucJoinWhiteFlag;
}

HI_VOID mrsSetWaitTopoFlag(HI_U8 ucFlag)
{
    mrsCcoGetDetectRmCtx()->stRunning.ucSetWaitTopoFlag = ucFlag;
    mrsDfxCcoDetectSetWaitTopoFlag(ucFlag);
}

HI_U8 mrsGetWaitTopoFlag(HI_VOID)
{
    return mrsCcoGetDetectRmCtx()->stRunning.ucSetWaitTopoFlag;
}

HI_U8 mrsGetBlackValidTime(HI_VOID)
{
    return mrsCcoGetDetectRmCtx()->stParam.ucBlackValidTime;
}

HI_U8 mrsGetJoinWhiteTime(HI_VOID)
{
    return mrsCcoGetDetectRmCtx()->stParam.ucStaJoinWhiteTime;
}

HI_VOID mrsSetRefreshWhiteFlag(HI_U8 ucFlag)
{
    mrsCcoGetDetectRmCtx()->stRunning.ucRefreshWhiteFlag = ucFlag;
    mrsDfxCcoDetectRefreshWhiteFlag(ucFlag);
}

HI_U8 mrsGetRefreshWhiteFlag(HI_VOID)
{
    return mrsCcoGetDetectRmCtx()->stRunning.ucRefreshWhiteFlag;
}

HI_U16 mrsGetDetectPlcSeq(HI_VOID)
{
    return mrsCcoGetDetectRmCtx()->stRunning.usSeq;
}

HI_VOID mrsDetectPlcSeqAdd(HI_VOID)
{
    mrsCcoGetDetectRmCtx()->stRunning.usSeq++;
}

HI_BOOL mrsIsDetectRm(HI_VOID)
{
    if (mrsGetDetectRmEnable() && (MRS_CCO_DETECT_RM_DOING == mrsGetDetectRmStatus()) && mrsGetDetectRmFlag())
    {
        return HI_TRUE;
    }

    return HI_FALSE;
}

HI_U32 mrsDetectRmRespProc(HI_U8 *pucMeter)
{
    if (!mrsGetDetectRmEnable() || (MRS_CCO_DETECT_RM_DOING != mrsGetDetectRmStatus()))
    {
        return HI_ERR_SKIP;
    }
        
    HI_DIAG_LOG_MSG_E2(MRS_FILE_LOG_FLAG_050, HI_DIAG_MT("detect rm resp proc"), pucMeter[0], pucMeter[5]);

    mrsDetectMeterRespProc(pucMeter);
    mrsCheckDetectEndProc();
    
    return HI_ERR_SUCCESS;
}

//探测抄表抄通后表信息处理
HI_BOOL mrsDetectMeterRespProc(HI_U8 *pucMeter)
{
    HI_U16 ucArchivesNum = mrsSrvArchivesMeterNum();
    HI_U16 usIndex = 0;
        
    for (usIndex = 0; usIndex < ucArchivesNum; usIndex++)
    {
        MRS_ARCHIVES_NODE_STRU * pstTemp = mrsSrvArchivesQuery(usIndex);
        
        if ((HI_NULL == pstTemp) || !pstTemp->bValid)
        {
            continue;
        }

        if (mrsIsMeterMatch(pstTemp->aucNodeAddr, pucMeter))
        {
            HI_DIAG_LOG_MSG_E2(MRS_FILE_LOG_FLAG_057, HI_DIAG_MT("resp meter match clear detect"), pucMeter[0], pucMeter[5]);
            pstTemp->ucDetect = 0;
            pstTemp->ucRmTurn = 0;
            mrsResetDetectRmDIIndex();
            return HI_TRUE;
        }
    }
    
    HI_DIAG_LOG_MSG_E2(MRS_FILE_LOG_FLAG_049, HI_DIAG_MT("resp meter not match archives"), pucMeter[0], pucMeter[5]);
    
    return HI_FALSE;
}

HI_U32 mrsCheckDetectEndProc(HI_VOID)
{
    HI_U16 usArchivesNum = mrsSrvArchivesMeterNum();
    HI_U16 usIndex = 0;
    
    if (!mrsGetDetectRmEnable() || (MRS_CCO_DETECT_RM_DOING != mrsGetDetectRmStatus()))
    {
        return HI_ERR_SKIP;
    }

    for (usIndex = 0; usIndex < usArchivesNum; usIndex++)
    {
        MRS_ARCHIVES_NODE_STRU * pstTemp = mrsSrvArchivesQuery(usIndex);
        
        if (pstTemp == HI_NULL || !pstTemp->bValid)
        {
            continue;
        }

        if (pstTemp->ucDetect)
        {
            break;
        }
    }

    if (usIndex == usArchivesNum)
    {
        HI_DIAG_LOG_MSG_E1(MRS_FILE_LOG_FLAG_048, HI_DIAG_MT("detect flag rm finish"), usArchivesNum);
        mrsCcoDetectRmEnd();
        mrsDfxCcoDetectEndReason(MRS_CCO_DETECT_END_REASON_RM_FINISH);
    }

    return HI_ERR_SUCCESS;
}

//探测抄表中被打断后的处理,场景:透抄、12-F1、12-F3
HI_VOID mrsDetectRmBreakProc(HI_VOID)
{
    MRS_StopTimer(MRS_CCO_TIMER_RESUME_DETECT_RM);
    
    if (mrsGetDetectRmFlag())
    {
        HI_DIAG_LOG_MSG_E0(MRS_FILE_LOG_FLAG_047, HI_DIAG_MT("detect rm break"));
        mrsSetDetectRmFlag(HI_FALSE);//将探测抄表中的抄表标志置为0
        mrsResetDetectRmDIIndex();
    }
}

HI_U32 mrsResumeDetectRmSendMsg(HI_VOID)
{
    HI_SYS_QUEUE_MSG_S stMsg;

    (hi_void)memset_s(&stMsg, sizeof(stMsg), 0, sizeof(stMsg));
    stMsg.ulMsgId = EN_MRS_FW_MSG_PAUSE_RESUME_DETECT;

    return mrsSendMessage2Queue(&stMsg);
}

//恢复探测抄表
HI_VOID mrsResumeDetectRmProc(HI_VOID)
{
    if (mrsGetDetectRmEnable() 
        && (MRS_CCO_DETECT_RM_DOING == mrsGetDetectRmStatus()) 
        && (HI_ERR_SUCCESS == mrsCheckRmStatus())
        && !mrsGetDetectRmFlag())
    {
        MRS_StopTimer(MRS_CCO_TIMER_RESUME_DETECT_RM);
        HI_DIAG_LOG_MSG_E0(MRS_FILE_LOG_FLAG_046, HI_DIAG_MT("resume detect rm"));
        mrsAnyTimeReadMeterInit();
        mrsDetectRmTransmitProc();
    }
}

HI_VOID mrsCcoRejectNodeCallback (HI_IN HI_MDM_NETWORK_REJECT_ENTRY_S *pstEntry)
{
    HI_U32 ulRet = HI_ERR_SUCCESS;
    MRS_CCO_DETECT_RUNNING_S* pstRunning = &mrsCcoGetDetectRmCtx()->stRunning;
    
    ulRet = mrsCheckAllowJoinWhiteList(pstEntry);
    if (HI_ERR_SUCCESS != ulRet)
    {
        return;
    }

    MRS_WaitSem(EN_APP_SEM_MRS_REJECT_JOIN_NET, HI_SYS_WAIT_FOREVER);  // 信号量，加锁
    HI_DIAG_LOG_MSG_E3(MRS_FILE_LOG_FLAG_001, HI_DIAG_MT("reject node callback mac"), pstEntry->ucMacType, pstEntry->ucProduct, mrsGetCacheMacNum());
    
    (hi_void)memcpy_s(pstRunning->pMacBuf + mrsGetCacheMacNum() * HI_PLC_MAC_ADDR_LEN, HI_PLC_MAC_ADDR_LEN, pstEntry->aucMac, HI_PLC_MAC_ADDR_LEN);

    if (!mrsGetCacheMacNum())
    {
        MRS_StartTimer(MRS_CCO_TIMER_DETECT_REJECT_CLT_CACHE, MRS_CCO_DETECT_MAC_CACHE_TIME_VALUE, HI_SYS_TIMER_ONESHOT);
    }
    
    mrsCacheMacNumAddSelf();
    
    if (MRS_CCO_DETECT_MAC_NUM_MAX == mrsGetCacheMacNum())
    {
        MRS_StopTimer(MRS_CCO_TIMER_DETECT_REJECT_CLT_CACHE);
        mrsCcoRejectNodeSendMsg(pstRunning);
        mrsResetCacheMacInfo();
    }
    MRS_SignalSem(EN_APP_SEM_MRS_REJECT_JOIN_NET); // 信号量，解锁
}

HI_U32 mrsCcoRejectNodeSendMsg(MRS_CCO_DETECT_RUNNING_S* pstRunning)
{
    HI_U32 ulRet = HI_ERR_SUCCESS;
    HI_SYS_QUEUE_MSG_S stMsg = {0};
    HI_U8 *pData = HI_NULL;
    HI_U16 usDataLen = pstRunning->ucCacheMacNum * HI_PLC_MAC_ADDR_LEN;

    pData = (HI_U8*)mrsToolsMalloc(usDataLen);
    if (!pData)
    {
        return HI_ERR_FAILURE;
    }

    (hi_void)memcpy_s(pData, usDataLen, pstRunning->pMacBuf, usDataLen);
    
    (hi_void)memset_s(&stMsg, sizeof(stMsg), 0, sizeof(stMsg));
    stMsg.ulMsgId = EN_MRS_FW_MSG_MAC_JOIN_WHITE_LIST;
    stMsg.ulParam[0] = (HI_U32)pData;
    stMsg.ulParam[1] = (HI_U32)usDataLen;

    ulRet = mrsSendMessage2Queue(&stMsg);
    if (HI_ERR_SUCCESS != ulRet)
    {
        mrsToolsFree(pData);
    }

    return ulRet;
}

HI_U32 mrsMacJoinWhiteMsgProc(HI_SYS_QUEUE_MSG_S *pstMsg)
{
    HI_U32 ulRet = HI_ERR_SUCCESS;
    HI_U8 ucRmEable = mrsGetDetectRmEnable();
    HI_U8 ucWhiteFlag = mrsGetAllowJoinWhiteFlag();
    
    if ((ucRmEable == HI_FALSE) || (MRS_CCO_DETECT_FORBID_JOIN_WHITE_LIST == ucWhiteFlag))
    {
        mrsToolsFree(pstMsg->ulParam[0]);
        return HI_ERR_SKIP;
    }

    HI_DIAG_LOG_MSG_E2(MRS_FILE_LOG_FLAG_053, HI_DIAG_MT("mac join white msg proc not allow"), ucRmEable, ucWhiteFlag);
    ulRet = mrsCcoCheckArchivesExistMap();
    if (HI_ERR_SUCCESS != ulRet)
    {
        ulRet = mrsDetectRefreshWhiteList((HI_U8*)pstMsg->ulParam[0], pstMsg->ulParam[1]);
    }
    else 
    {
        HI_DIAG_LOG_MSG_E2(MRS_FILE_LOG_FLAG_060, HI_DIAG_MT("all meter exist map"), ucRmEable, ucWhiteFlag);
    }
    
    mrsToolsFree(pstMsg->ulParam[0]);

    return ulRet;
}

HI_U32 mrsDetectWhiteListCheckProc(HI_VOID)
{
    HI_U32 ulCurrTime = HI_MDM_GetMilliSeconds();
    HI_U16 usIndex = 0;
    HI_BOOL bFlag = HI_FALSE;
    HI_U8 ucWhiteFlag = mrsGetAllowJoinWhiteFlag();
    
    if (!mrsGetDetectRmEnable() || (MRS_CCO_DETECT_FORBID_JOIN_WHITE_LIST == ucWhiteFlag))
    {
        return HI_ERR_SUCCESS;
    }
    
    for (usIndex = 0; usIndex < mrsGetDetectWhiteListNum(); usIndex++)
    {
        MRS_CCO_DETECT_WHITE_QUEUE_ITEM* pstItem = mrsDetectWhiteItemQuery(usIndex);
        
        if (!pstItem || pstItem->bValid)
        {
            continue;
        }

        if ((ulCurrTime - pstItem->ulTimeStamp) >= mrsGetJoinWhiteTime() * 60 * 1000)
        {
            pstItem->bValid = HI_TRUE;
            bFlag = HI_TRUE;
            mrsDetectWhiteListValidAddSelf();
            
            if (!mrsGetRefreshWhiteFlag())
            {
                HI_DIAG_LOG_MSG_E0(MRS_FILE_LOG_FLAG_066, HI_DIAG_MT("white check start timer"));
                MRS_StartTimer(MRS_CCO_TIMER_DETECT_REFRESH_WHITE, MRS_CCO_DETECT_REFRESH_WHITE_TIME_VALUE, HI_SYS_TIMER_PERIODIC);
                mrsSetRefreshWhiteFlag(1);
            }
        }
    }

    if (mrsGetDetectWhiteListValidNum() == mrsGetDetectWhiteListNum())
    {
        HI_DIAG_LOG_MSG_E1(MRS_FILE_LOG_FLAG_067, HI_DIAG_MT("white all valid"), mrsGetDetectWhiteListNum());
        MRS_StopTimer(MRS_CCO_TIMER_DETECT_REFRESH_WHITE);
        mrsSetRefreshWhiteFlag(0);
    }
    
    if (bFlag)
    {
        mrsSyncMeterInfToWhiteList(HI_MAC_WHIST_LIST_CHANGE_REASON_CLT_JOIN_NET);
    }

    return HI_ERR_SUCCESS;
}

HI_U32 mrsDetectWhiteListAddTimeout(HI_VOID)
{
    HI_U32 ulRet = HI_ERR_SUCCESS;
    MRS_CCO_DETECT_RUNNING_S* pstRunning = &mrsCcoGetDetectRmCtx()->stRunning;
    HI_U16 usDataLen = 0;

    MRS_WaitSem(EN_APP_SEM_MRS_REJECT_JOIN_NET, HI_SYS_WAIT_FOREVER);  // 信号量，加锁
    ulRet = mrsCcoCheckArchivesExistMap();
    if ((HI_ERR_SUCCESS != ulRet) && pstRunning->ucCacheMacNum)
    {
        usDataLen = pstRunning->ucCacheMacNum * HI_PLC_MAC_ADDR_LEN;
        mrsDetectRefreshWhiteList(pstRunning->pMacBuf, (HI_U32)usDataLen);
    }

    HI_DIAG_LOG_MSG_E2(MRS_FILE_LOG_FLAG_056, HI_DIAG_MT("mrsDetectWhiteListAddTimeout ret"), ulRet, pstRunning->ucCacheMacNum);
    mrsResetCacheMacInfo();
    MRS_SignalSem(EN_APP_SEM_MRS_REJECT_JOIN_NET); // 信号量，解锁
    
    return HI_ERR_SUCCESS;
}

HI_U32 mrsDetectRefreshWhiteList(HI_U8 *pData, HI_U32 ulDataLen)
{
    HI_U8 ucWhiteFlag = mrsGetRefreshWhiteFlag();
    HI_U8 ucRmStatus = mrsGetDetectRmStatus();

    mrsCcoSaveCltInfo(pData, ulDataLen);
    
    //启动检测白名单是否生效,3分钟后刷新表档案白名单
    if (ucWhiteFlag == HI_FALSE)
    {
        MRS_StartTimer(MRS_CCO_TIMER_DETECT_REFRESH_WHITE, MRS_CCO_DETECT_REFRESH_WHITE_TIME_VALUE, HI_SYS_TIMER_PERIODIC);
        mrsSetRefreshWhiteFlag(1);
    }

    if (MRS_CCO_DETECT_RM_IDLE == ucRmStatus)
    {
        HI_DIAG_LOG_MSG_E0(MRS_FILE_LOG_FLAG_004, HI_DIAG_MT("to white add no rm status"));
        mrsSetDetectRmStatus(MRS_CCO_WHITE_ADD_NO_RM);
        mrsDfxCcoDetectStartTime(HI_MDM_GetMilliSeconds());
    }

    if (MRS_CCO_WHITE_ADD_NO_RM == ucRmStatus)
    {
        HI_DIAG_LOG_MSG_E1(MRS_FILE_LOG_FLAG_005, HI_DIAG_MT("start white keep timer"), mrsCcoGetDetectRmCtx()->stParam.usTimeOut);
        //如果一定时间内没有白名单中的站点加入，探测入网策略结束
        MRS_StartTimer(MRS_CCO_TIMER_DETECT_WHITE_LIST_KEEP, mrsCcoGetDetectRmCtx()->stParam.usTimeOut * 60 * 1000, HI_SYS_TIMER_ONESHOT);
    }
    
    HI_DIAG_LOG_MSG_E3(MRS_FILE_LOG_FLAG_002, HI_DIAG_MT("refresh white list"), ulDataLen, ucRmStatus, ucWhiteFlag);

    return HI_ERR_SUCCESS;
}

HI_U32 mrsCcoDetectWhiteSetMacList(HI_U16 usNum, HI_MAC_VALID_MAC_LIST_STRU * pstMeterList)
{
    HI_U16 usIndex = 0;
    HI_U16 usCount = 0;

    if (!usNum)
    {
        return HI_ERR_INVALID_PARAMETER;
    }
    
    for(usIndex = 0; usIndex < mrsGetDetectWhiteListNum(); usIndex++)
    {
        MRS_CCO_DETECT_WHITE_QUEUE_ITEM *pstItem = mrsDetectWhiteItemQuery(usIndex);

        if (!pstItem || !pstItem->bValid)
        {
            continue;
        }
    
        (hi_void)memcpy_s(pstMeterList->mac_list[pstMeterList->list_num].mac, HI_PLC_MAC_ADDR_LEN, pstItem->aucMac, HI_PLC_MAC_ADDR_LEN);
        pstMeterList->list_num++;
        
        HI_DIAG_LOG_MSG_E3(MRS_FILE_LOG_FLAG_064, HI_DIAG_MT("sync detect white list"), pstItem->aucMac[0], pstItem->aucMac[4], pstItem->aucMac[5]);

        usCount++;
        if ((usCount >= usNum) || (pstMeterList->list_num >= HI_MAC_VALID_MAC_LIST_MAX))
        {
            return HI_ERR_SUCCESS;
        }
        
	}
    
    return HI_ERR_SUCCESS;
}

HI_U32 mrsDetectWhiteListKeepTimeOut(HI_VOID)
{
    HI_U8 ucRmEnable = mrsGetDetectRmEnable();
    HI_U8 ucRmStatus = mrsGetDetectRmStatus();
    
    if (ucRmEnable == HI_FALSE || MRS_CCO_DETECT_RM_IDLE == ucRmStatus)
    {
        return HI_ERR_SUCCESS;
    }

    mrsDfxCcoDetectEndReason(MRS_CCO_DETECT_END_REASON_WHITE_KEEP_TIMEOUT);
    HI_DIAG_LOG_MSG_E2(MRS_FILE_LOG_FLAG_006, HI_DIAG_MT("white keep timeout DetectRmEnd"), ucRmEnable, ucRmStatus);
    
    return mrsCcoDetectRmEnd();
}

HI_U32 mrsCcoSaveCltInfo(HI_U8 *pData, HI_U32 ulDataLen)
{
    MRS_CCO_DETECT_RM_CTX_S* pstRmCtx = mrsCcoGetDetectRmCtx();
    MRS_CCO_DETECT_WHITE_QUEUE_ITEM *pstItem = HI_NULL;
    HI_U8 ucIndex = 0;
    HI_U16 usSaveNum = HI_MAC_VALID_MAC_LIST_MAX - mrsSrvArchivesMeterNum();
    HI_U16 usCount = 0;

    if (!pData || !ulDataLen)
    {
        return HI_ERR_INVALID_PARAMETER;
    }

    usSaveNum = (HI_U16)MRS_MIN(usSaveNum, ulDataLen / HI_PLC_MAC_ADDR_LEN);
    
    for (ucIndex = 0; ucIndex < ulDataLen / HI_PLC_MAC_ADDR_LEN; ucIndex++)
    {
        MRS_DETECT_WHITE_FIND_RESULT stFindRet;

        (hi_void)memset_s(&stFindRet, sizeof(stFindRet), 0, sizeof(stFindRet));
        (hi_void)memcpy_s(stFindRet.aucMacAddr, sizeof(stFindRet.aucMacAddr), pData + ucIndex * HI_PLC_MAC_ADDR_LEN, HI_PLC_MAC_ADDR_LEN);
        mrsSrvTraverseQueue(&pstRmCtx->stWhiteListQueue.stQueue, mrsCcoFindRepeatMacItem, &stFindRet);
        if (MRS_CCO_FIND_REPEAT_MAC_SUCCESS == stFindRet.result)
        {
            continue;
        }
        
        pstItem = (MRS_CCO_DETECT_WHITE_QUEUE_ITEM*)mrsToolsMalloc(sizeof(MRS_CCO_DETECT_WHITE_QUEUE_ITEM));
        if (!pstItem)
        {
            return HI_ERR_MALLOC_FAILUE;
        }
        
        (hi_void)memset_s(pstItem, sizeof(MRS_CCO_DETECT_WHITE_QUEUE_ITEM), 0, sizeof(MRS_CCO_DETECT_WHITE_QUEUE_ITEM));
        (hi_void)memcpy_s(pstItem->aucMac, sizeof(pstItem->aucMac), pData + ucIndex * HI_PLC_MAC_ADDR_LEN, HI_PLC_MAC_ADDR_LEN);
        pstItem->ulTimeStamp = HI_MDM_GetMilliSeconds();
        
        mrsSrvEnQueue(&pstRmCtx->stWhiteListQueue.stQueue, &pstItem->stLink);
        mrsDetectWhiteListAddSelf();
        usCount++;
        
        if (usCount >= usSaveNum)
        {
            HI_DIAG_LOG_MSG_E2(MRS_FILE_LOG_FLAG_003, HI_DIAG_MT("SaveCltInfo"), mrsGetDetectWhiteListNum(), usSaveNum);
            return HI_ERR_FULL;
        }
    }
    
    return HI_ERR_SUCCESS;
}

HI_BOOL mrsCcoFindRepeatMacItem(HI_VOID *node, HI_VOID *param)
{
    MRS_CCO_DETECT_WHITE_QUEUE_ITEM* pstItem = (MRS_CCO_DETECT_WHITE_QUEUE_ITEM*)node;
    P_MRS_DETECT_WHITE_FIND_RESULT pstJoinItem = (P_MRS_DETECT_WHITE_FIND_RESULT)param;
    
    if (!memcmp(pstItem->aucMac, pstJoinItem->aucMacAddr, HI_PLC_MAC_ADDR_LEN))
    {
        pstJoinItem->result = MRS_CCO_FIND_REPEAT_MAC_SUCCESS;
        return HI_TRUE;
    }

    pstJoinItem->result = MRS_CCO_FIND_REPEAT_MAC_FAIL;
    
    return HI_FALSE;
}

MRS_CCO_DETECT_WHITE_QUEUE_ITEM* mrsDetectWhiteItemQuery(HI_U16 usIndex)
{
    MRS_CCO_DETECT_RM_CTX_S* pstRmCtx = mrsCcoGetDetectRmCtx();
    HI_U16 usIndex2 = 0;
    MRS_SRV_NODE *pstNode = HI_NULL;

    if (usIndex >= mrsGetDetectWhiteListNum())
    {
        return HI_NULL;
    }
    
    for (pstNode = pstRmCtx->stWhiteListQueue.stQueue.front; pstNode != HI_NULL; pstNode = pstNode->next)
    {
        if (usIndex2 == usIndex)
        {
            return (MRS_CCO_DETECT_WHITE_QUEUE_ITEM*)pstNode;
        }
        
        usIndex2++;
    }

    return HI_NULL;
}

HI_U32 mrsCheckAllowJoinWhiteList(HI_IN HI_MDM_NETWORK_REJECT_ENTRY_S *pstEntry)
{
    HI_BOOL bRet = HI_FALSE;
    HI_U8 ucRmEnable = mrsGetDetectRmEnable();
    HI_U8 ucWhiteFlag = mrsGetAllowJoinWhiteFlag();

    if (!ucRmEnable || (MRS_CCO_DETECT_FORBID_JOIN_WHITE_LIST == ucWhiteFlag))
    {
        return HI_ERR_FAILURE;
    }
    
    if ((mrsSrvArchivesMeterNum() + mrsGetDetectWhiteListNum()) >= HI_MAC_VALID_MAC_LIST_MAX)
    {
        HI_DIAG_LOG_MSG_E2(MRS_FILE_LOG_FLAG_051, HI_DIAG_MT("meter white list full"), mrsSrvArchivesMeterNum(), mrsCcoGetDetectRmCtx()->stWhiteListQueue.usNum);
        return HI_ERR_FAILURE;
    }
    
    if ((HI_MDM_METER_MAC == pstEntry->ucMacType) || !mrsMapMacIsCollector(pstEntry->ucProduct) || !mrsSrvArchivesMeterNum())
    {
        HI_DIAG_LOG_MSG_E3(MRS_FILE_LOG_FLAG_059, HI_DIAG_MT("reject node add white2"), pstEntry->ucMacType, mrsSrvArchivesMeterNum(), pstEntry->ucProduct);
        return HI_ERR_FAILURE;
    }

    MRS_WaitSem(EN_APP_SEM_MRS_BLACK_LIST, HI_SYS_WAIT_FOREVER);  // 信号量，加锁
    bRet = mrsDetectIsStaExistBlackList(pstEntry->aucMac);
    MRS_SignalSem(EN_APP_SEM_MRS_BLACK_LIST); // 信号量，解锁
    if (bRet)
    {
        HI_DIAG_LOG_MSG_E0(MRS_FILE_LOG_FLAG_061, HI_DIAG_MT("node exist black list"));
        return HI_ERR_FAILURE;
    }
    
    return HI_ERR_SUCCESS;
}

//场景:上电load完表档案后、PLC由可用变为不可用后;
//如果有表档案,启动20分钟机制,如果表档案为空,设置状态等待表档案设置后再启动20分钟机制
HI_VOID mrsDetectWaitTopoInit(HI_VOID)
{
    if (!mrsGetDetectRmEnable())
    {
        return;
    }
    
    MRS_StopTimer(MRS_CCO_TIMER_DETECT_WAIT_TOPO);
    
    if (!mrsSrvArchivesMeterNum())
    {
        HI_DIAG_LOG_MSG_E0(MRS_FILE_LOG_FLAG_007, HI_DIAG_MT("detect wait topo meter null"));
        mrsSetWaitTopoFlag(MRS_CCO_DETECT_METER_EMPTY_STATUS);
        return;
    }
    
    HI_DIAG_LOG_MSG_E0(MRS_FILE_LOG_FLAG_008, HI_DIAG_MT("detect wait topo start wait timer"));
    MRS_StartTimer(MRS_CCO_TIMER_DETECT_WAIT_TOPO, mrsCcoGetDetectRmCtx()->stParam.ucTopoWaitMaxTime * 60 * 1000, HI_SYS_TIMER_ONESHOT);

    return;
}

//20分钟等待拓扑组网超时处理,设置允许加入表档案白名单标志
HI_U32 mrsDetectWaitTopoTimeOut(HI_VOID)
{   
    HI_U8 ucRmEnable = mrsGetDetectRmEnable();
    HI_U8 ucWhiteFlag = mrsGetAllowJoinWhiteFlag();
    HI_U8 ucTopoFlag = mrsGetWaitTopoFlag();
    if (!ucRmEnable)
    {
        return HI_ERR_SKIP;
    }

    if (ucWhiteFlag > 0 || (MRS_CCO_DETECT_WAIT_TOPO_IDLE_STATUS != ucTopoFlag))
    {
        return HI_ERR_SUCCESS;
    }

    HI_DIAG_LOG_MSG_E3(MRS_FILE_LOG_FLAG_009, HI_DIAG_MT("detect wait topo timeout"), ucRmEnable, ucWhiteFlag, ucTopoFlag);

    if (mrsSrvArchivesMeterNum() && (mrsCcoGetContext()->stTopoInf.num <= 1))
    {
        mrsSetAllowJoinWhiteFlag(MRS_CCO_DETECT_ALLOW_JOIN_WHITE_LIST);
        mrsDfxCcoDetectAllowJoinWhiteReason(MRS_CCO_DETECT_ALLOW_JOIN_WHITE_TOPO_FORME);
        mrsDfxCcoDetectAllowJoinWhiteTime(HI_MDM_GetMilliSeconds());
        HI_DIAG_LOG_MSG_E0(MRS_FILE_LOG_FLAG_045, HI_DIAG_MT("detect wait topo timeout set allow"));
    }

    return HI_ERR_SUCCESS;
}

//表档案变动后通知MAC时探测流程处理
HI_U32 mrsCcoDetectMeterChgToMacProc(HI_VOID)
{
    HI_U8 ucRmEnable = mrsGetDetectRmEnable();
    HI_U8 ucTopoFlag = mrsGetWaitTopoFlag();
    HI_U8 ucRmStatus = mrsGetDetectRmStatus();
    
    if (!ucRmEnable)
    {
        return HI_ERR_SKIP;
    }

    if (MRS_CCO_DETECT_RM_IDLE == ucRmStatus)
    {
        MRS_WaitSem(EN_APP_SEM_MRS_BLACK_LIST, HI_SYS_WAIT_FOREVER);  // 信号量，加锁
        mrsCcoDetectSaveBlackList();//必须放在清空新加入的白名单之前
        MRS_SignalSem(EN_APP_SEM_MRS_BLACK_LIST); // 信号量，解锁
        mrsResetDetectWhiteListNum();
        mrsResetDetectWhiteListValidNum();
        mrsSrvEmptyQueue(&mrsCcoGetDetectRmCtx()->stWhiteListQueue.stQueue, mrsDetectQueueFree);
    }
    
    if (!mrsSrvArchivesMeterNum())
    {
        MRS_StopTimer(MRS_CCO_TIMER_DETECT_WAIT_TOPO);
        mrsSetWaitTopoFlag(MRS_CCO_DETECT_METER_EMPTY_STATUS);
        return HI_ERR_SUCCESS;
    }

    if (MRS_CCO_DETECT_NEED_WAIT_TOPO_STATUS == ucTopoFlag)
    {
        if (!mrsGetPlcChlStatus())
        {
            MRS_StartTimer(MRS_CCO_TIMER_DETECT_WAIT_TOPO, mrsCcoGetDetectRmCtx()->stParam.ucTopoWaitMaxTime * 60 * 1000, HI_SYS_TIMER_ONESHOT);
        }
        
        mrsSetWaitTopoFlag(MRS_CCO_DETECT_WAIT_TOPO_IDLE_STATUS);
    }
    
    HI_DIAG_LOG_MSG_E3(MRS_FILE_LOG_FLAG_010, HI_DIAG_MT("detect meter chg to mac proc"), mrsSrvArchivesMeterNum(), ucTopoFlag, mrsGetPlcChlStatus());

    if (MRS_CCO_DETECT_RM_DOING == ucRmStatus)
    {
        HI_DIAG_LOG_MSG_E0(MRS_FILE_LOG_FLAG_044, HI_DIAG_MT("detect meter chg to mac flag proc"));
        mrsCcoCheckArchivesMapProc();
    }
    
    return HI_ERR_SUCCESS;
}

//11-F1中表档案从0个变为非0个后执行
HI_VOID mrsCcoDetectEmptyArchivesAddProc(HI_VOID)
{
    HI_U8 ucRmEnable = mrsGetDetectRmEnable();
    HI_U8 ucTopoFlag = mrsGetWaitTopoFlag();
    
    mrsDfxCcoArchives0To1Time(HI_MDM_GetMilliSeconds());
    
    if (!ucRmEnable)
    {
        return;
    }
    
    if (MRS_CCO_DETECT_METER_EMPTY_STATUS == ucTopoFlag)
    {
        mrsSetWaitTopoFlag(MRS_CCO_DETECT_NEED_WAIT_TOPO_STATUS);
    }

    HI_DIAG_LOG_MSG_E2(MRS_FILE_LOG_FLAG_011, HI_DIAG_MT("meter null add proc"), ucTopoFlag,ucRmEnable);

}

HI_VOID mrsDetectTopoChgPreProc(HI_U8 ucStatus)
{
    HI_U8 ucRmStatus = mrsGetDetectRmStatus();
    if (HI_MDM_MAC_NETWORK_EVALUATE_END != ucStatus)
    {
        return;
    
    }
    
    MRS_StopTimer(MRS_CCO_TIMER_DETECT_WAIT_TOPO);
    mrsSetAllowJoinWhiteFlag(MRS_CCO_DETECT_ALLOW_JOIN_WHITE_LIST);
    mrsDfxCcoDetectAllowJoinWhiteReason(MRS_CCO_DETECT_ALLOW_JOIN_WHITE_TOPO_EVALUATE);
    mrsDfxCcoDetectAllowJoinWhiteTime(HI_MDM_GetMilliSeconds());
    HI_DIAG_LOG_MSG_E1(MRS_FILE_LOG_FLAG_012, HI_DIAG_MT("optimize end proc"), ucRmStatus);
}

HI_U32 mrsCcoDetectTopoChgProc(HI_U8 ucStatus)
{
    if (!mrsGetDetectRmEnable() || (HI_MDM_MAC_NETWORK_FORMED == ucStatus))
    {
        return HI_ERR_SUCCESS;
    }

    mrsDetectTopoChgPreProc(ucStatus);
    
    return mrsDetectRmStartProc(ucStatus);
}

HI_U32 mrsDetectRmStartProc(HI_U8 ucStatus)
{
    HI_U32 ulRet = HI_ERR_SKIP;

    ulRet = mrsDetectCheckStart();
    if (HI_ERR_SUCCESS != ulRet)
    {
        return ulRet;
    }

    if (HI_MDM_MAC_NETWORK_EVALUATE_END == ucStatus)
    {
        HI_DIAG_LOG_MSG_E0(MRS_FILE_LOG_FLAG_041, HI_DIAG_MT("detect rm start"));
        mrsSetDetectRmStatus(MRS_CCO_DETECT_RM_DOING);
        ulRet = mrsCcoDetectRmStart();
    }
    else if (HI_MDM_MAC_STA_JOIN_NET == ucStatus)
    {
        HI_DIAG_LOG_MSG_E1(MRS_FILE_LOG_FLAG_040, HI_DIAG_MT("detect rm wait start"), mrsCcoGetDetectRmCtx()->stParam.ucJoinNetMaxTime);
     	mrsSetDetectRmStatus(MRS_CCO_DETECT_RM_WAITING);
        ulRet = MRS_StartTimer(MRS_CCO_TIMER_DETECT_JOIN_NET_CACHE, mrsCcoGetDetectRmCtx()->stParam.ucJoinNetMaxTime * 60 * 1000, HI_SYS_TIMER_ONESHOT);
    }

    return ulRet;
}

HI_U32 mrsDetectCheckStart(HI_VOID)
{
    HI_U32 ulRet = HI_ERR_SKIP;
    
    if (MRS_CCO_WHITE_ADD_NO_RM != mrsGetDetectRmStatus())
    {
        return HI_ERR_SKIP;
    }
    
    ulRet = mrsCheckTopoDetectWhiteExist();
    if (HI_ERR_SUCCESS != ulRet)
    {
        HI_DIAG_LOG_MSG_E0(MRS_FILE_LOG_FLAG_043, HI_DIAG_MT("topo not exist detect mac"));
        return ulRet;
    }

    MRS_StopTimer(MRS_CCO_TIMER_DETECT_WHITE_LIST_KEEP);
    ulRet = mrsCcoCheckArchivesMapProc();
    if (HI_ERR_SUCCESS == ulRet)
    {
        HI_DIAG_LOG_MSG_E0(MRS_FILE_LOG_FLAG_042, HI_DIAG_MT("all meter exist map detect end"));
        mrsCcoDetectRmEnd();
        mrsDfxCcoDetectEndReason(MRS_CCO_DETECT_END_REASON_METER_EXIST_MAP);
        return HI_ERR_SKIP;
    }

    return HI_ERR_SUCCESS;
}

HI_U32 mrsDetectJoinNetTimeout(HI_VOID)
{
    HI_U8 ucRmEnable = mrsGetDetectRmEnable();
    
    if (!ucRmEnable)
    {
        return HI_ERR_SUCCESS;
    }
    
    HI_DIAG_LOG_MSG_E1(MRS_FILE_LOG_FLAG_039, HI_DIAG_MT("detect rm wait timeout"), ucRmEnable);

    mrsSetDetectRmStatus(MRS_CCO_DETECT_RM_DOING);
    
    return mrsCcoDetectRmStart();
}

//检查拓扑中是否有探测机制中追加的表档案白名单地址
HI_U32 mrsCheckTopoDetectWhiteExist(HI_VOID)
{
    HI_U16 usIndex = 0;
    MRS_SRV_QUEUE *pstQueue = &mrsCcoGetDetectRmCtx()->stWhiteListQueue.stQueue;
    MRS_SRV_NODE *pstNode = HI_NULL;
    MRS_CCO_SRV_CTX_STRU * pstCcoCtx = mrsCcoGetContext();

    for (pstNode = pstQueue->front; pstNode != HI_NULL; pstNode = pstNode->next)
    {
        MRS_CCO_DETECT_WHITE_QUEUE_ITEM *pstItem = (MRS_CCO_DETECT_WHITE_QUEUE_ITEM*)pstNode;

        if (!pstItem->bValid)
        {
            continue;
        }
        
        for (usIndex = 1; usIndex < pstCcoCtx->stTopoInf.num; usIndex++)
        {//从STA站点开始
            if (mrsToolsMemEq(pstItem->aucMac, pstCcoCtx->stTopoInf.entry[usIndex].mac, HI_PLC_MAC_ADDR_LEN))
            {
                return HI_ERR_SUCCESS;
            }
        }
    }

    return HI_ERR_NOT_FOUND;
}

//检查表档案中的表是否都在映射表中
HI_U32 mrsCcoCheckArchivesExistMap(HI_VOID)
{
    MRS_CCO_SRV_CTX_STRU* pstCcoCtx = mrsCcoGetContext();

    if (mrsSrvArchivesMeterNum() > pstCcoCtx->stMapModule.stMacMeterMap.usNumber)
    {
        return HI_ERR_FAILURE;
    }

    return mrsCcoCheckArchivesMapProc();
}

HI_U32 mrsCcoCheckArchivesMapProc(HI_VOID)
{
    MRS_MAC_DEVICE_MAP *pstMapInfo = &mrsCcoGetContext()->stMapModule.stMacMeterMap;
    HI_U16 usIndex = 0, usIndex2 = 0;
    HI_BOOL bFlag = HI_FALSE;
    HI_U16 ucArchivesNum = mrsSrvArchivesMeterNum();
    
    if (!ucArchivesNum)
    {
        return HI_ERR_SUCCESS;
    }
    
    for (usIndex = 0; usIndex < ucArchivesNum; usIndex++)
    {
        HI_U8 aucNodeAddr[HI_METER_ADDR_LEN] = {0};
        HI_U8 aucMapAddr[HI_METER_ADDR_LEN] = {0};
        MRS_ARCHIVES_NODE_STRU * pstTemp = mrsSrvArchivesQuery(usIndex);
        
        if (pstTemp == HI_NULL || !pstTemp->bValid)
        {
            continue;
        }
        
        mrsMeterAddrPreProc(aucNodeAddr, pstTemp->aucNodeAddr, pstTemp->ucProtocol);
        for (usIndex2 = 0; usIndex2 < pstMapInfo->usMapSize; usIndex2++)
        {
            if (!pstMapInfo->pstItem[usIndex2].bValid)
            {
                continue;
            }
            
            mrsMeterAddrPreProc(aucMapAddr, pstMapInfo->pstItem[usIndex2].stDeviceInf.aucAddr, pstMapInfo->pstItem[usIndex2].stDeviceInf.ucProtocol);
            if (mrsToolsMemEq(aucNodeAddr, aucMapAddr, HI_METER_ADDR_LEN))
            {
                pstTemp->ucDetect = 0;
                pstTemp->ucRmTurn = 0;
                break;
            }
        }

        if (pstMapInfo->usMapSize == usIndex2)
        {
            bFlag = HI_TRUE;
            
            if (!pstTemp->ucDetect)
            {
                pstTemp->ucDetect = 1;
                pstTemp->ucRmTurn = 0;
            }
        }
    }

    if (HI_FALSE == bFlag)
    {
        return HI_ERR_SUCCESS;
    }

    return HI_ERR_FAILURE;
}

HI_U32 mrsCcoDetectRmStart(HI_VOID)
{
    HI_U32 ulRet = HI_ERR_SKIP;
    
#if !defined(PRODUCT_CFG_PRODUCT_TYPE_CSG_GD)
    mrsDetectRMTurnAddSelf();
#endif

    ulRet = mrsCcoCheckArchivesMapProc();
    if (HI_ERR_SUCCESS == ulRet)
    {
        HI_DIAG_LOG_MSG_E0(MRS_FILE_LOG_FLAG_037, HI_DIAG_MT("detect rm meter exist map end"));
        mrsCcoDetectRmEnd();
        mrsDfxCcoDetectEndReason(MRS_CCO_DETECT_END_REASON_METER_EXIST_MAP);
        return ulRet;
    }
    
    mrsSetDetectRmFlag(HI_FALSE);
    mrsResetDetectRmDIIndex();

#if defined(PRODUCT_CFG_PRODUCT_TYPE_CSG_GD)
    ulRet = mrsDetectSendPlcProc();
    if (HI_ERR_SUCCESS != ulRet)
    {
        HI_DIAG_LOG_MSG_E1(MRS_FILE_LOG_FLAG_017, HI_DIAG_MT("detect send plc exit end"), ulRet);
        mrsCcoDetectRmEnd();
        mrsDfxCcoDetectEndReason(MRS_CCO_DETECT_END_REASON_OTHER);
    }
#else
    ulRet = mrsCheckRmStatus();
    if (HI_ERR_SUCCESS != ulRet)
    {
        return ulRet;
    }
    
    ulRet = mrsDetectRmTransmitProc();
#endif

    return ulRet;
}

HI_U32 mrsDetectRmTransmitProc(HI_VOID)
{
    HI_U32 ulRet = HI_ERR_SUCCESS;
    HI_U8 * pDataBuff = HI_NULL;
    HI_U16 usDataLen = 0;
    MRS_ARCHIVES_NODE_STRU *pstNode = &mrsCcoGetDetectRmCtx()->stRunning.stNode;
    HI_U8 ucReason = MRS_CCO_DETECT_END_REASON_OTHER;

    do
    {
        ulRet = mrsDetectRmFindValidNode();
        if (HI_ERR_SUCCESS != ulRet)
        {
            ucReason = MRS_CCO_DETECT_END_REASON_RM_FINISH;
            break;
        }
        
        ulRet = mrsDetectAssemble645Frame(pstNode, &pDataBuff, &usDataLen);
        if (HI_ERR_SUCCESS == ulRet)
        {
            HI_DIAG_LOG_BUF(MRS_FILE_LOG_BUF_1000, HI_DIAG_MT("detect 645"), pDataBuff, usDataLen);
            ulRet = mrsAnyTimeReadMeterProcess(pstNode->aucNodeAddr, pstNode->ucProtocol, pDataBuff, usDataLen);
        }

        mrsToolsFree(pDataBuff);
    } while (0);

    if (HI_ERR_SUCCESS != ulRet)
    {
        HI_DIAG_LOG_MSG_E1(MRS_FILE_LOG_FLAG_036, HI_DIAG_MT("detect rm transmit fail end"), ulRet);
        mrsCcoDetectRmEnd();
        mrsDfxCcoDetectEndReason(ucReason);
    }
    else
    {
        mrsSetDetectRmFlag(HI_TRUE);//超时或抄通后、或集中器下发透抄帧、路由恢复和路由重启后赋值为false 
    }
    
    return ulRet;
}

HI_U32 mrsDetectAssemble645Frame(MRS_ARCHIVES_NODE_STRU *pstNode, HI_U8 **ppData, HI_U16 *pusDataLen)
{
    MRS_PROTO645_FRAME_INF stFrameInf;
    MRS_CCO_DETECT_RM_CTX_S* pstRmCtx = mrsCcoGetDetectRmCtx();

    (hi_void)memset_s(&stFrameInf, sizeof(stFrameInf), 0, sizeof(stFrameInf));
    (hi_void)memcpy_s(stFrameInf.aucAddr, sizeof(stFrameInf.aucAddr), pstNode->aucNodeAddr, HI_METER_ADDR_LEN);
    if (MRS_PROTO645_VER_1997 == pstNode->ucProtocol)
    {
        stFrameInf.ucCtrl = MRS_645_FRAME_CONTROL_READ_DATA97;
        stFrameInf.ucDiSize = MRS_645_FRAME_DATA_DI_SIZE_V97;
        (hi_void)memcpy_s(&stFrameInf.ulDi, sizeof(stFrameInf.ulDi), &pstRmCtx->stParam.aus97DI[pstRmCtx->stRunning.uc97DIIndex], stFrameInf.ucDiSize);
    }
    else
    {
        stFrameInf.ucCtrl = MRS_645_FRAME_CONTROL_READ_DATA;
        stFrameInf.ucDiSize = MRS_645_FRAME_DATA_DI_SIZE;
        (hi_void)memcpy_s(&stFrameInf.ulDi, sizeof(stFrameInf.ulDi), &pstRmCtx->stParam.ul07DI, stFrameInf.ucDiSize);
    }
    
    return mrsGen645Frame(&stFrameInf, ppData, pusDataLen);
}

HI_U32 mrsDetectRmFindValidNode(HI_VOID)
{
    HI_U32 ulRet = HI_ERR_SUCCESS;
    MRS_CCO_DETECT_RM_CTX_S* pstRmCtx = mrsCcoGetDetectRmCtx();
    HI_U16 usIndex = 0;
    HI_U8 ucIndex2 = 0;
    HI_U16 usArchivesNum = mrsSrvArchivesMeterNum();

    for (ucIndex2 = pstRmCtx->stRunning.ucCurrTurn; ucIndex2 <= pstRmCtx->stParam.ucMaxTurn; ucIndex2++)
    {
        for (usIndex = 0; usIndex < usArchivesNum; usIndex++)
        {

            MRS_ARCHIVES_NODE_STRU * pstItem = mrsSrvArchivesQuery(usIndex);
            HI_U8 aucMac[HI_PLC_MAC_ADDR_LEN] = {0};
        
            if (pstItem == HI_NULL || !pstItem->bValid || !pstItem->ucDetect)
            {
                continue;
            }
        
            // 查找映射关系
            ulRet = mrsMapGetMAC(pstItem->aucNodeAddr, aucMac);
            if(ulRet == HI_ERR_SUCCESS)
            {
                HI_DIAG_LOG_MSG_E2(MRS_FILE_LOG_FLAG_015, HI_DIAG_MT("node detect exist map"), pstItem->aucNodeAddr[0],pstItem->aucNodeAddr[5]);
                pstItem->ucDetect = 0;
                pstItem->ucRmTurn = 0;
                continue;
            }
            
            if (pstItem->ucRmTurn < pstRmCtx->stRunning.ucCurrTurn)
            {
                (hi_void)memcpy_s(&pstRmCtx->stRunning.stNode, sizeof(pstRmCtx->stRunning.stNode), pstItem, sizeof(MRS_ARCHIVES_NODE_STRU));
                HI_DIAG_LOG_MSG_E3(MRS_FILE_LOG_FLAG_013, HI_DIAG_MT("find node meter"), pstItem->aucNodeAddr[0],pstItem->aucNodeAddr[5],pstItem->ucProtocol);
                return HI_ERR_SUCCESS;
            }
        }
        
        if (usIndex == usArchivesNum)
        {
            pstRmCtx->stRunning.ucCurrTurn++;
            HI_DIAG_LOG_MSG_E2(MRS_FILE_LOG_FLAG_014, HI_DIAG_MT("find meter turn add"), pstRmCtx->stRunning.ucCurrTurn, usArchivesNum);
        }
    }
    
    return HI_ERR_NOT_FOUND;
}

//探测抄表超时后节点处理
HI_U32 mrsCcoDetectRmTimeoutProc(HI_U8 *pucMeter)
{
    HI_U8 ucRmEnable = mrsGetDetectRmEnable();
    HI_U8 ucRmStatus = mrsGetDetectRmStatus();
    HI_U8 ucRmFlag = mrsGetDetectRmFlag();


    if (!ucRmEnable || (MRS_CCO_DETECT_RM_DOING != ucRmStatus))
    {
        return HI_ERR_SKIP;
    }

    if (ucRmFlag)
    {
        mrsCcoDetectRmNodeProc();
    }
    else
    {
        mrsDetectArchivesMeterTurnAdd(pucMeter);
    }

    HI_DIAG_LOG_MSG_E3(MRS_FILE_LOG_FLAG_052, HI_DIAG_MT("detect rm timeout"), ucRmEnable,ucRmStatus , ucRmFlag);

    return HI_ERR_SUCCESS;
}

//探测抄表超时或收到探测抄表的应答后节点处理
HI_U32 mrsCcoDetectRmNodeProc(HI_VOID)
{
    MRS_ARCHIVES_NODE_STRU * pstNode = &mrsCcoGetDetectRmCtx()->stRunning.stNode;

    if (MRS_PROTO645_VER_1997 == pstNode->ucProtocol)
    {
        if (mrsGetDetectRmDIIndex() < (MRS_BROADCAST_RM_97_DI_NUM - 1))
        {
            mrsDetectRmDIIndexAddSelf();
            return HI_ERR_SUCCESS;
        }
        
        mrsResetDetectRmDIIndex();
    }

    mrsDetectArchivesMeterTurnAdd(pstNode->aucNodeAddr);
    
    return HI_ERR_SUCCESS;
}

HI_VOID mrsDetectArchivesMeterTurnAdd(HI_U8* pucMeter)
{
    HI_U16 ucArchivesNum = mrsSrvArchivesMeterNum();
    HI_U16 usIndex = 0;
    
    
    for (usIndex = 0; usIndex < ucArchivesNum; usIndex++)
    {
        MRS_ARCHIVES_NODE_STRU * pstTemp = mrsSrvArchivesQuery(usIndex);
        
        if (pstTemp == HI_NULL || !pstTemp->bValid || !pstTemp->ucDetect)
        {
            continue;
        }

        if (mrsIsMeterMatch(pstTemp->aucNodeAddr, pucMeter))
        {
            pstTemp->ucRmTurn++;
            if (pstTemp->ucRmTurn >= mrsCcoGetDetectRmCtx()->stParam.ucMaxTurn)
            {
                pstTemp->ucDetect = 0;
                pstTemp->ucRmTurn = 0;
            }
            
            return;
        }
    }   

    return;
}

HI_U32 mrsCheckRmStatus(HI_VOID)
{
    MRS_CCO_SRV_CTX_STRU * pstCcoCtx = mrsCcoGetContext();

    if (pstCcoCtx->status & (MRS_CCO_STATUS_NORMAL | MRS_CCO_STATUS_ANYTIME_READMETER))
    {
        HI_DIAG_LOG_MSG_E1(MRS_FILE_LOG_FLAG_038, HI_DIAG_MT("detect rm cco rm status"), pstCcoCtx->status);
        return HI_ERR_SKIP;
    }
    
    return HI_ERR_SUCCESS;
}

HI_VOID mrsDetectLrRmTimeout(MRS_ARCHIVES_NODE_STRU *pstNode)
{
    if (mrsGetDetectRmEnable() && (MRS_CCO_DETECT_RM_DOING == mrsGetDetectRmStatus()) && pstNode->ucDetect)
    {
        pstNode->ucRmTurn++;
    }

    if (pstNode->ucRmTurn >= mrsCcoGetDetectRmCtx()->stParam.ucMaxTurn)
    {
        pstNode->ucDetect = 0;
        pstNode->ucRmTurn = 0;
    }

    mrsCheckDetectEndProc();
}


/**
 @brief  Detect module timout function, called when parallel meter-reading timeout. 
 @brief  CNcomment: 探测模块超时处理函数，并发抄表超时时调用。CNend
 @param[in] pMeterAddr  Meter address.  CNcomment: 电表地址。CNend
 */
HI_VOID mrsDetectPrRmTimeout(HI_U8 *pucMeter)

{
    MRS_ARCHIVES_NODE_STRU *pstNode = mrsSrvArchiversQueryByAddr(pucMeter);

    if (pstNode)
    {
        mrsDetectLrRmTimeout(pstNode);
    }
}


//将探测机制中信息清零 场景:探测机制结束;表档案由非空变为空后执行
HI_U32 mrsCcoDetectRmReset(HI_VOID)
{//不能将允许加入白名单标志清零
    HI_U8 ucRmStatus = mrsGetDetectRmStatus();
    
    if (!mrsGetDetectRmEnable())
    {
        return HI_ERR_SKIP;
    }

    if (ucRmStatus)
    {
        mrsDfxCcoDetectEndTime(HI_MDM_GetMilliSeconds());
    }
    
    MRS_StopTimer(MRS_CCO_TIMER_DETECT_WHITE_LIST_KEEP);
    MRS_StopTimer(MRS_CCO_TIMER_DETECT_REJECT_CLT_CACHE);
    MRS_StopTimer(MRS_CCO_TIMER_DETECT_WAIT_TOPO);
    MRS_StopTimer(MRS_CCO_TIMER_DETECT_JOIN_NET_CACHE);
    MRS_StopTimer(MRS_CCO_TIMER_DETECT_REFRESH_WHITE);
    MRS_StopTimer(MRS_CCO_TIMER_RESUME_DETECT_RM);
    MRS_StopTimer(MRS_CCO_TIMER_SEND_METER_LIST_PLC);
    HI_DIAG_LOG_MSG_E1(MRS_FILE_LOG_FLAG_035, HI_DIAG_MT("detect rm reset"), ucRmStatus);
    mrsSetDetectRmStatus(MRS_CCO_DETECT_RM_IDLE);
    mrsResetDetectRmDIIndex();
    mrsResetDetectRmTurn();
    mrsSetRefreshWhiteFlag(0);
    
    MRS_WaitSem(EN_APP_SEM_MRS_REJECT_JOIN_NET, HI_SYS_WAIT_FOREVER);  // 信号量，加锁
    mrsResetCacheMacInfo();
    MRS_SignalSem(EN_APP_SEM_MRS_REJECT_JOIN_NET); // 信号量，解锁
    
    return HI_ERR_SUCCESS;
}

HI_VOID mrsDetectQueueFree(HI_VOID * p)
{
    mrsToolsFree(p);
}

HI_U32 mrsCcoDetectRmEnd(HI_VOID)//各种结束以及PLC通道不可用时调用,如果通道由可用变为不可用时,如果等待拓扑状态是2,则应
{//不能将允许加入白名单标志清零
    if (!mrsGetDetectRmEnable() || !mrsGetDetectRmStatus())
    {
        return HI_ERR_SUCCESS;
    }
    
    HI_DIAG_LOG_MSG_E0(MRS_FILE_LOG_FLAG_055, HI_DIAG_MT("cco detect end"));
    mrsCcoDetectRmReset();
    
    MRS_WaitSem(EN_APP_SEM_MRS_BLACK_LIST, HI_SYS_WAIT_FOREVER);  // 信号量，加锁
    mrsCcoDetectSaveBlackList();//必须放在清空新加入的白名单之前
    MRS_SignalSem(EN_APP_SEM_MRS_BLACK_LIST); // 信号量，解锁
    mrsResetDetectWhiteListNum();
    mrsResetDetectWhiteListValidNum();
    mrsSrvEmptyQueue(&mrsCcoGetDetectRmCtx()->stWhiteListQueue.stQueue, mrsDetectQueueFree);
    
    mrsArchivesDetectInfoReset();

    return mrsSyncMeterInfToWhiteList(HI_MAC_WHIST_LIST_CHANGE_REASON_APP_CON);
}

HI_U32 mrsCcoDetectSaveBlackList(HI_VOID)
{
    HI_U16 usIndex = 0, usIndex2 = 0;
    MRS_CCO_SRV_CTX_STRU *pstCcoCtx = mrsCcoGetContext();
    HI_MAC_NETWORK_TOPO_S *pstTopoInf = &pstCcoCtx->stTopoInf;

    mrsRefreshTopoInfo(pstCcoCtx);
    
    for (usIndex = 0; usIndex < mrsGetDetectWhiteListNum(); usIndex++)
    {
        MRS_CCO_DETECT_WHITE_QUEUE_ITEM *pstItem = mrsDetectWhiteItemQuery(usIndex);
        
        if (!pstItem || !pstItem->bValid)
        {
            continue;
        }

        for (usIndex2 = 0; usIndex2 < pstTopoInf->num; usIndex2++)
        {
            if (mrsToolsMemEq(pstItem->aucMac, pstTopoInf->entry[usIndex2].mac, HI_PLC_MAC_ADDR_LEN))
            {
                mrsCcoDetectBlackAdd(pstItem->aucMac);
                break;
            }
        }
    }
    
    HI_DIAG_LOG_MSG_E1(MRS_FILE_LOG_FLAG_062, HI_DIAG_MT("save detect black list"), mrsGetDetectBlackListNum());

    if (mrsGetDetectBlackListNum())
    {
        //启用X分钟定时器,超时后删除超过X分钟的站点
        MRS_StartTimer(MRS_CCO_TIMER_DETECT_BLACK_LIST, mrsGetBlackValidTime() * 60 * 1000 + 100, HI_SYS_TIMER_ONESHOT);
    }
    
    return HI_ERR_SUCCESS;
}

HI_U32 mrsCcoDetectBlackAdd(HI_U8 *pucMac)
{
    MRS_CCO_DETECT_RM_CTX_S* pstRmCtx = mrsCcoGetDetectRmCtx();
    MRS_DETECT_WHITE_FIND_RESULT stFindRet;
    MRS_CCO_DETECT_BLACK_LIST_ITEM *pstItem = HI_NULL;

    (hi_void)memset_s(&stFindRet, sizeof(stFindRet), 0, sizeof(stFindRet));
    (hi_void)memcpy_s(stFindRet.aucMacAddr, sizeof(stFindRet.aucMacAddr), pucMac, HI_PLC_MAC_ADDR_LEN);
    mrsSrvTraverseList(&pstRmCtx->stBlackList.stList, mrsDetectBlackListFindRepeatItem, &stFindRet);
    if (MRS_CCO_FIND_REPEAT_MAC_SUCCESS == stFindRet.result)
    {
        return HI_ERR_EXIST;
    }
    
    pstItem = (MRS_CCO_DETECT_BLACK_LIST_ITEM*)mrsToolsMalloc(sizeof(MRS_CCO_DETECT_BLACK_LIST_ITEM));
    if (!pstItem)
    {
        return HI_ERR_MALLOC_FAILUE;
    }
    
    (hi_void)memset_s(pstItem, sizeof(MRS_CCO_DETECT_BLACK_LIST_ITEM), 0, sizeof(MRS_CCO_DETECT_BLACK_LIST_ITEM));
    (hi_void)memcpy_s(pstItem->aucMac, sizeof(pstItem->aucMac), pucMac, HI_PLC_MAC_ADDR_LEN);
    pstItem->ulTimeStamp = HI_MDM_GetMilliSeconds();
    
    mrsSrvListInsTail(&pstRmCtx->stBlackList.stList, &pstItem->stLink);
    mrsDetectBlackListAddSelf();

    return HI_ERR_SUCCESS;
}

MRS_CCO_DETECT_BLACK_LIST_ITEM* mrsDetectBlackInfoQuery(HI_U16 usIndex)
{
    MRS_CCO_DETECT_RM_CTX_S* pstRmCtx = mrsCcoGetDetectRmCtx();
    HI_U16 usIndex2 = 0;
    MRS_SRV_LINK *pstNode = HI_NULL;

    if (usIndex >= mrsGetDetectBlackListNum())
    {
        return HI_NULL;
    }
    
    for (pstNode = pstRmCtx->stBlackList.stList.next; pstNode != &pstRmCtx->stBlackList.stList; pstNode = pstNode->next)
    {
        if (usIndex2 == usIndex)
        {
            return (MRS_CCO_DETECT_BLACK_LIST_ITEM*)pstNode;
        }
        
        usIndex2++;
    }

    return HI_NULL;
}

HI_VOID mrsDetectBlackListTimeout(HI_VOID)
{
    HI_S16 sIndex = 0;

    MRS_WaitSem(EN_APP_SEM_MRS_BLACK_LIST, HI_SYS_WAIT_FOREVER);  // 信号量，加锁
    for (sIndex = 0; sIndex < mrsGetDetectBlackListNum(); sIndex++)
    {
        MRS_CCO_DETECT_BLACK_LIST_ITEM* pstItem = mrsDetectBlackInfoQuery((HI_U16)sIndex);
        HI_U32 ulCurrTime = HI_MDM_GetMilliSeconds();
        
        if (!pstItem)
        {
            continue;
        }

        if ((ulCurrTime - pstItem->ulTimeStamp) >= mrsGetBlackValidTime() * 60 * 1000)
        {
            HI_U32 ulRet = HI_ERR_SUCCESS;
            
            //从链表中删除
            ulRet = mrsDetectBlackRemove(pstItem);
            if (HI_ERR_SUCCESS == ulRet)
            {
                sIndex--;
            }
        }
    }
    MRS_SignalSem(EN_APP_SEM_MRS_BLACK_LIST); // 信号量，解锁

    HI_DIAG_LOG_MSG_E1(MRS_FILE_LOG_FLAG_063, HI_DIAG_MT("detect black timeout"), mrsGetDetectBlackListNum());
}

HI_U32 mrsDetectBlackRemove(MRS_CCO_DETECT_BLACK_LIST_ITEM* pstItem)
{
    HI_U32 ulRet = HI_ERR_SUCCESS;
    
    ulRet = mrsSrvListRemove(&mrsCcoGetDetectRmCtx()->stBlackList.stList, &pstItem->stLink);
    if (HI_ERR_SUCCESS == ulRet)
    {
        mrsToolsFree(pstItem);
        mrsDetectBlackListDecSelf();
    }

    return ulRet;
}

HI_BOOL mrsDetectIsStaExistBlackList(HI_U8 *pucMacAddr)
{
    HI_S16 sIndex = 0;

    for (sIndex = 0; sIndex < mrsGetDetectBlackListNum(); sIndex++)
    {
        MRS_CCO_DETECT_BLACK_LIST_ITEM* pstItem = mrsDetectBlackInfoQuery((HI_U16)sIndex);
        HI_U32 ulCurrTime = HI_MDM_GetMilliSeconds();
        
        if (!pstItem)
        {
            continue;
        }

        if ((ulCurrTime - pstItem->ulTimeStamp) >= mrsGetBlackValidTime() * 60 * 1000)
        {
            HI_U32 ulRet = HI_ERR_SUCCESS;
            
            //从链表中删除
            ulRet = mrsDetectBlackRemove(pstItem);
            if (HI_ERR_SUCCESS == ulRet)
            {
                sIndex--;
            }
            continue;
        }
        
        if (mrsToolsMemEq(pstItem->aucMac, pucMacAddr, HI_PLC_MAC_ADDR_LEN))
        {
            return HI_TRUE;
        }
    }

    return HI_FALSE;
}

HI_BOOL mrsDetectBlackListFindRepeatItem(HI_VOID *node, HI_VOID *param)
{
    MRS_CCO_DETECT_BLACK_LIST_ITEM* pstItem = (MRS_CCO_DETECT_BLACK_LIST_ITEM*)node;
    P_MRS_DETECT_WHITE_FIND_RESULT pstJoinItem = (P_MRS_DETECT_WHITE_FIND_RESULT)param;
    
    if (!memcmp(pstItem->aucMac, pstJoinItem->aucMacAddr, HI_PLC_MAC_ADDR_LEN))
    {
        pstJoinItem->result = MRS_CCO_FIND_REPEAT_MAC_SUCCESS;
        pstItem->ulTimeStamp = HI_MDM_GetMilliSeconds();
        
        return HI_TRUE;
    }

    pstJoinItem->result = MRS_CCO_FIND_REPEAT_MAC_FAIL;
    
    return HI_FALSE;
}

//清空表档案中的探测信息
HI_VOID mrsArchivesDetectInfoReset(HI_VOID)
{
    HI_U16 usIndex = 0;
    HI_U16 usArchivesNum = mrsSrvArchivesMeterNum();

    HI_DIAG_LOG_MSG_E1(MRS_FILE_LOG_FLAG_054, HI_DIAG_MT("clear meter detect flag"), usArchivesNum);
    for (usIndex = 0; usIndex < usArchivesNum; usIndex++)
    {

        MRS_ARCHIVES_NODE_STRU * pstItem = mrsSrvArchivesQuery(usIndex);

        if (pstItem != HI_NULL)
        {
            pstItem->ucDetect = 0;
            pstItem->ucRmTurn = 0;
        }
    }
}

HI_VOID mrsCcoDetectPlcDisconnectProc(HI_VOID)
{
    HI_U8 ucRmStatus = mrsGetDetectRmStatus();
    
    if (!mrsGetDetectRmEnable())
    {
        return;
    }
    
    mrsDfxCcoDetectPlcDisconnectTime(HI_MDM_GetMilliSeconds());

    if (ucRmStatus)
    {
        HI_DIAG_LOG_MSG_E1(MRS_FILE_LOG_FLAG_034, HI_DIAG_MT("plc disconnect detect end"), ucRmStatus);
        mrsCcoDetectRmEnd();
        mrsDfxCcoDetectEndReason(MRS_CCO_DETECT_END_REASON_PLC_DISCONNECT);
    }
    
    if (MRS_CCO_DETECT_ALLOW_JOIN_WHITE_LIST == mrsGetAllowJoinWhiteFlag())
    {
        //还要将允许加入白名单标志清零
        mrsSetAllowJoinWhiteFlag(MRS_CCO_DETECT_FORBID_JOIN_WHITE_LIST);
        mrsDfxCcoDetectForbidJoinWhiteReason(MRS_CCO_DETECT_FORBID_JOIN_WHITE_PLC_DISCONNECT);
        mrsDfxCcoDetectForbidJoinWhiteTime(HI_MDM_GetMilliSeconds());
    }
    
    mrsDetectWaitTopoInit();
}

HI_U32 mrsDetectRxModeNotify(HI_U8 ucMode)
{
    if (DIAG_CMD_MRS_MODE_ONEKEY_DIAGNOSTIC == ucMode)
    {
        mrsDetectRmExterDisable();
    }
    else
    {
        mrsDetectRmExterResume();
    }

    return HI_ERR_SUCCESS;
}

HI_VOID mrsDetectRmExterDisable(HI_VOID)
{
    HI_U8 ucRmEnable = mrsGetDetectRmEnable();
    HI_U8 ucRmStatus = mrsGetDetectRmStatus();
    
    if (ucRmEnable == 0 || !mrsIsSimuChlOpened())
    {
        return;
    }

    if (ucRmStatus)
    {
        mrsDfxCcoDetectEndReason(MRS_CCO_DETECT_END_REASON_EXTERN_DISABLE);
        if (mrsGetDetectRmFlag())
        {
            mrsAnyTimeReadMeterInit();
        }
    }
    
    HI_DIAG_LOG_MSG_E1(MRS_FILE_LOG_FLAG_033, HI_DIAG_MT("extern disable detect end"), ucRmStatus);
    mrsCcoDetectRmEnd();
    //还要将允许加入白名单标志清零
    mrsSetAllowJoinWhiteFlag(MRS_CCO_DETECT_FORBID_JOIN_WHITE_LIST);
    mrsDfxCcoDetectForbidJoinWhiteReason(MRS_CCO_DETECT_FORBID_JOIN_WHITE_EXTERN_DISABLE);
    mrsDfxCcoDetectForbidJoinWhiteTime(HI_MDM_GetMilliSeconds());
    mrsSetDetectRmExterStatus(HI_TRUE);
    mrsSetDetectOriginalEnable(ucRmEnable);
    mrsSetDetectRmEnable(HI_FALSE);
    mrsDfxCcoDetectExternDisableTime(HI_MDM_GetMilliSeconds());
}

HI_VOID mrsDetectRmExterResume(HI_VOID)
{
    HI_U8 ucRmExStatus = mrsGetDetectRmExterStatus();
    HI_U8 ucRmEnable = mrsGetDetectRmEnable();
    HI_U8 ucTopoStatus = mrsGetTopoStatus();
    
    if (!ucRmExStatus || ucRmEnable)
    {
        return;
    }

    mrsSetDetectRmExterStatus(HI_FALSE);
    mrsSetDetectRmEnable(mrsGetDetectOriginalEnable());
    mrsSetDetectOriginalEnable(HI_FALSE);
    mrsDfxCcoDetectExternResumeTime(HI_MDM_GetMilliSeconds());
    HI_DIAG_LOG_MSG_E3(MRS_FILE_LOG_FLAG_032, HI_DIAG_MT("detect extern resume"), ucRmExStatus, ucRmEnable, ucTopoStatus);

    if (HI_MDM_MAC_NETWORK_EVALUATE_END == ucTopoStatus)
    {
        mrsSetAllowJoinWhiteFlag(MRS_CCO_DETECT_ALLOW_JOIN_WHITE_LIST);
        mrsDfxCcoDetectAllowJoinWhiteReason(MRS_CCO_DETECT_ALLOW_JOIN_WHITE_EXTERN_RESUME);
        mrsDfxCcoDetectAllowJoinWhiteTime(HI_MDM_GetMilliSeconds());
    }
    else if (!ucTopoStatus)
    {
        mrsDetectWaitTopoInit();
    }
}

#if defined(PRODUCT_CFG_PRODUCT_TYPE_CSG_GD)
HI_U32 mrsDetectGetMeterList(APP_CSG_METER_INFO **ppInfo, HI_U8 *pucNum)
{
    HI_U8 ucCount = 0;
    HI_U16 usIndex2 = 0;
    HI_U16 usArchivesNum = mrsSrvArchivesMeterNum();
    MRS_CCO_DETECT_RM_CTX_S* pstRmCtx = mrsCcoGetDetectRmCtx();

    *ppInfo = (APP_CSG_METER_INFO*)mrsToolsMalloc(APP_CSG_CMD_DL_METERLIST_MAX * sizeof(APP_CSG_METER_INFO));
    if (!*ppInfo)
    {
        return HI_ERR_MALLOC_FAILUE;
    }

    (hi_void)memset_s(*ppInfo, APP_CSG_CMD_DL_METERLIST_MAX * sizeof(APP_CSG_METER_INFO), 0, APP_CSG_CMD_DL_METERLIST_MAX * sizeof(APP_CSG_METER_INFO));
    
    for (usIndex2 = 0; usIndex2 < usArchivesNum; usIndex2++)
    {
        MRS_ARCHIVES_NODE_STRU * pstItem = mrsSrvArchivesQuery(usIndex2);
        
        if ((pstItem == HI_NULL) || !pstItem->bValid || !pstItem->ucDetect || pstItem->ucRead)
        {
            continue;
        }
        
        if (pstItem->ucRmTurn <= pstRmCtx->stRunning.ucCurrTurn)
        {
            (hi_void)memcpy_s((*ppInfo)[ucCount].aucMeterAddr, HI_METER_ADDR_LEN, pstItem->aucNodeAddr, HI_METER_ADDR_LEN);
            pstItem->ucRmTurn++;
            pstItem->ucRead = HI_TRUE;
            ucCount++;
            if (ucCount == APP_CSG_CMD_DL_METERLIST_MAX)
            {
                break;
            }
            else
            {
                continue;
            }
        }
    }

    if (ucCount)
    {
        *pucNum = ucCount;
        MRS_StartTimer(MRS_CCO_TIMER_SEND_METER_LIST_PLC, pstRmCtx->stParam.ucSingleMeterTime * ucCount * 1000, HI_SYS_TIMER_ONESHOT);
        HI_DIAG_LOG_BUF(MRS_FILE_LOG_BUF_1001, HI_DIAG_MT("plc meter list"), *ppInfo, ucCount * sizeof(APP_CSG_METER_INFO));
        return HI_ERR_SUCCESS;
    }
    
    mrsDetectRMTurnAddSelf();
    mrsSrvArchivesReadFlagClear();
    mrsMapMaintainProcess(MRS_MAP_MAINTAIN_MODE_PASSIVE);
    
    if (pstRmCtx->stRunning.ucCurrTurn < pstRmCtx->stParam.ucMaxTurn)
    {
        MRS_StartTimer(MRS_CCO_TIMER_SEND_METER_LIST_PLC, pstRmCtx->stParam.ucTurnTime * 60 * 1000, HI_SYS_TIMER_ONESHOT);
    }
    else
    {
        HI_DIAG_LOG_MSG_E1(MRS_FILE_LOG_FLAG_016, HI_DIAG_MT("maxturn finish"), pstRmCtx->stRunning.ucCurrTurn);
        mrsCcoDetectRmEnd();
        mrsDfxCcoDetectEndReason(MRS_CCO_DETECT_END_REASON_RM_FINISH);
    }
    
    return HI_ERR_SUCCESS;
}

HI_U32 mrsDetectSendPlcProc(HI_VOID)
{
    MRS_PLC_FRAME_DATA_STRU stPlcUlFrame = {0};
    MRS_CSG_PLC_FRAME_HEAD *pstUlHead = HI_NULL;
    APP_CSG_CMD_DL_METERLIST_STRU *pstData = HI_NULL;
    APP_CSG_METER_INFO *pstMeterInfo = HI_NULL;
    HI_U8 ucNum = 0;
    HI_U8 *pucPayload = HI_NULL;
    HI_U16 usPayloadLen = 0;
    HI_U8 ucIndex = 0;
    HI_U32 ulRet = HI_ERR_SUCCESS;

    ulRet = mrsDetectGetMeterList(&pstMeterInfo, &ucNum);
    if (HI_ERR_SUCCESS != ulRet)
    {
        mrsToolsFree(pstMeterInfo);
        return ulRet;
    }

    if (!ucNum)
    {
        mrsToolsFree(pstMeterInfo);
        return ulRet;
    }
    
    usPayloadLen = sizeof(MRS_CSG_PLC_FRAME_HEAD) + sizeof(APP_CSG_CMD_DL_METERLIST_STRU) + sizeof(APP_CSG_METER_INFO) * ucNum;
    pucPayload = (HI_U8 *)mrsToolsMalloc(usPayloadLen);
    if (!pucPayload)
    {
        mrsToolsFree(pstMeterInfo);
        return HI_ERR_MALLOC_FAILUE;
    }

    (hi_void)memset_s(pucPayload, usPayloadLen, 0, usPayloadLen);
    pstUlHead = (MRS_CSG_PLC_FRAME_HEAD *)pucPayload;
    pstUlHead->usFrameType = MRS_CSG_PLC_CTRL_TYPE_CMD;
    pstUlHead->usExtendBit = MRS_CSG_PLC_CTRL_EXTEND_BIT_N;
    pstUlHead->usRespBit = MRS_CSG_PLC_CTRL_RESP_BIT_N;
    pstUlHead->usPrm = MRS_CSG_PLC_CTRL_PRM_HOST;
    pstUlHead->usDir = MRS_CSG_PLC_CTRL_DIR_DL;
    pstUlHead->ucSrvId = MRS_CSG_PLC_SRV_ID_CMD_DL_METERLIST;
    pstUlHead->ucVersion = MRS_CSG_PLC_PROTO_VERSION;
    mrsDetectPlcSeqAdd();
    pstUlHead->usSeq = mrsGetDetectPlcSeq();
    pstUlHead->usDataLength = usPayloadLen - sizeof(MRS_CSG_PLC_FRAME_HEAD);

    pstData = (APP_CSG_CMD_DL_METERLIST_STRU *)pstUlHead->aucData;
    pstData->ucMeterNum = ucNum;

    for (ucIndex = 0; ucIndex < ucNum; ucIndex++)
    {
        (hi_void)memcpy_s(pstData->astMeterInf[ucIndex].aucMeterAddr, HI_METER_ADDR_LEN, pstMeterInfo[ucIndex].aucMeterAddr, HI_METER_ADDR_LEN);
    }

    stPlcUlFrame.usId = ID_MRS_CMD_CSG_DATA_TRANSMIT_PLC;
    stPlcUlFrame.pucPayload = pucPayload;
    stPlcUlFrame.usPayloadLen = usPayloadLen;
    stPlcUlFrame.is_bc_frame = HI_TRUE;
    (hi_void)memset_s(stPlcUlFrame.ucMac, HI_PLC_MAC_ADDR_LEN, 0xFF, HI_PLC_MAC_ADDR_LEN);

    ulRet = MRS_SendPlcFrame(&stPlcUlFrame);
    mrsToolsFree(pucPayload);
    mrsToolsFree(pstMeterInfo);

    return ulRet;
}
#endif //PRODUCT_CFG_PRODUCT_TYPE_CSG_GD

#endif //PRODUCT_CFG_PRODUCT_TYPE_CCO
