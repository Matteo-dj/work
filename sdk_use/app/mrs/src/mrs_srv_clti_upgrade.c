//*****************************************************************************
//
//                  版权所有 (C), 1998-2016, 华为技术有限公司
//
//*****************************************************************************
//  文 件 名   : mrs_srv_clti_upgrade.c
//  版 本 号   : V1.0 
//  作    者   : cuiate/c00233580
//  生成日期   : 2016-05-26
//  功能描述   : I型采集器升级Demo实现
//               
//  函数列表   : TODO: ...
//  修改历史   : 
//  1.日    期 : 2016-05-26
//    作    者 : cuiate/c00233580
//    修改内容 : 创建文件 
//
//*****************************************************************************

#include "mrs_common.h"
#include "mrs_fw_n.h"
#include "mrs_fw_tools.h"
#include "mrs_fw_log.h"
#include "mrs_srv_res.h"
#include "mrs_srv_clti_upgrade.h"

HI_START_HEADER
#if defined(PRODUCT_CFG_PRODUCT_TYPE_STA)
#if defined(PRODUCT_CFG_SUPPORT_MRS_UPG_DEMO)
//*****************************************************************************
//*****************************************************************************

HI_PRV HI_U32 mrsCltIUpgMsgNotify(HI_U16 usId, HI_PBYTE pucPacket, HI_U16 usPacketSize);

HI_PRV HI_U32 mrsCltIUpgStartProc(HI_MDM_DEVICE_UPG_START_REQ_S *pstReq);

HI_PRV HI_U32 mrsCltIUpgStopProc(HI_MDM_DEVICE_UPG_STOP_REQ_S *pstReq);

HI_PRV HI_U32 mrsCltIUpgStatusProc(HI_MDM_DEVICE_UPG_STATUS_REQ_S *pstReq);

HI_PRV HI_U32 mrsCltIUpgStatusReport(HI_VOID);

typedef struct  
{
    HI_U32 ulUpgId;
    HI_U32 ulFileLength;
    HI_PBYTE pucFileData;
    HI_U32 ulOffset;
    HI_U8 ucStatus;
    HI_U8 reserved[3];
    HI_PBYTE pucCache;
} MRS_CLTI_UPG_MODULE_STRU;


HI_PRV MRS_CLTI_UPG_MODULE_STRU g_stCltIUpgCtx;

HI_PRV MRS_CLTI_UPG_MODULE_STRU *mrsGetCltIUpgCtx(HI_VOID);

//*****************************************************************************
//*****************************************************************************

// I采升级模块初始化
HI_U32 mrsCltIUpgModuleInit(HI_VOID)
{
    HI_MDM_RegisterUpgMsgNotify(HI_MDM_DEVICE_UPG_START, mrsCltIUpgMsgNotify);
    HI_MDM_RegisterUpgMsgNotify(HI_MDM_DEVICE_UPG_STOP, mrsCltIUpgMsgNotify);
    HI_MDM_RegisterUpgMsgNotify(HI_MDM_DEVICE_UPG_STATUS, mrsCltIUpgMsgNotify);

    (hi_void)memset_s(&g_stCltIUpgCtx, sizeof(g_stCltIUpgCtx), 0, sizeof(g_stCltIUpgCtx));
    g_stCltIUpgCtx.ucStatus = HI_MDM_DEVICE_UPG_STATUS_IDLE;

    return HI_ERR_SUCCESS;
}

// I采升级模块去初始化
HI_U32 mrsCltIUpgModuleDeinit(HI_VOID)
{
    MRS_CLTI_UPG_MODULE_STRU *pstUpgCtx = mrsGetCltIUpgCtx();

    MRS_StopTimer(MRS_STA_TIMER_CLTI_UPG);

    mrsToolsFree(pstUpgCtx->pucCache);
    (hi_void)memset_s(pstUpgCtx, sizeof(MRS_CLTI_UPG_MODULE_STRU), 0, sizeof(MRS_CLTI_UPG_MODULE_STRU));
    pstUpgCtx->ucStatus = HI_MDM_DEVICE_UPG_STATUS_IDLE;

    return HI_ERR_SUCCESS;
}


MRS_CLTI_UPG_MODULE_STRU *mrsGetCltIUpgCtx(HI_VOID)
{
    return &g_stCltIUpgCtx;
}
                          
HI_U32 mrsCltIUpgMsgNotify(HI_U16 usId, HI_PBYTE pucPacket, HI_U16 usPacketSize)
{
    HI_MDM_MSG_APPRCV_S *pstMsgRcv = HI_NULL;
    HI_SYS_QUEUE_MSG_S stMsg = {0};
    HI_U32 ulRet = HI_ERR_SUCCESS;

    if (usId < HI_MDM_DEVICE_UPG_START || usId > HI_MDM_DEVICE_UPG_STATUS)
    {
        return HI_ERR_INVALID_ID;
    }

    pstMsgRcv = (HI_MDM_MSG_APPRCV_S *)mrsToolsMalloc(sizeof(HI_MDM_MSG_APPRCV_S));
    if (!pstMsgRcv)
    {
        return HI_ERR_NO_MORE_MEMORY;
    }

    (hi_void)memset_s(pstMsgRcv, sizeof(HI_MDM_MSG_APPRCV_S), 0, sizeof(HI_MDM_MSG_APPRCV_S));
    pstMsgRcv->usPacketSize = usPacketSize;
    pstMsgRcv->stCtrl.usTransId = HI_MDM_OBJ_ID_MRS;

    pstMsgRcv->pucPacket = (HI_PBYTE)mrsToolsMalloc(pstMsgRcv->usPacketSize);
    if (!pstMsgRcv->pucPacket)
    {
        mrsToolsFree(pstMsgRcv);
        return HI_ERR_NO_MORE_MEMORY;
    }

    (hi_void)memset_s(pstMsgRcv->pucPacket, pstMsgRcv->usPacketSize, 0, pstMsgRcv->usPacketSize);
    (hi_void)memcpy_s(pstMsgRcv->pucPacket, pstMsgRcv->usPacketSize, pucPacket, pstMsgRcv->usPacketSize);

    (hi_void)memset_s(&stMsg, sizeof(stMsg), 0, sizeof(stMsg));
    stMsg.ulMsgId = EN_MRS_FW_MSG_MDM_INPUT;
    stMsg.ulParam[0] = usId;
    stMsg.ulParam[1] = (HI_U32)pstMsgRcv;
    ulRet = mrsSendMessage2Queue(&stMsg);
    if (HI_ERR_SUCCESS != ulRet)
    {
        mrsToolsFree(pstMsgRcv->pucPacket);
        mrsToolsFree(pstMsgRcv);
    }

    return ulRet;
}


HI_U32 mrsCltIUpgMsgProc(HI_SYS_QUEUE_MSG_S *pstMsg)
{
    HI_U32 ulRet = HI_ERR_SUCCESS;
    HI_U16 usMsgId = (HI_U16)pstMsg->ulParam[0];
    HI_MDM_MSG_APPRCV_S* pstMsgRcv = (HI_MDM_MSG_APPRCV_S *)pstMsg->ulParam[1];

    if (!pstMsgRcv)
    {
        return HI_ERR_INVALID_PARAMETER;
    }

    switch (usMsgId)
    {
    case HI_MDM_DEVICE_UPG_START:
        mrsCltIUpgStartProc((HI_MDM_DEVICE_UPG_START_REQ_S *)pstMsgRcv->pucPacket);
        break;

    case HI_MDM_DEVICE_UPG_STOP:
        mrsCltIUpgStopProc((HI_MDM_DEVICE_UPG_STOP_REQ_S *)pstMsgRcv->pucPacket);
        break;

    case HI_MDM_DEVICE_UPG_STATUS:
        mrsCltIUpgStatusProc((HI_MDM_DEVICE_UPG_STATUS_REQ_S *)pstMsgRcv->pucPacket);
        break;

    default:
        ulRet = HI_ERR_INVALID_ID;
        break;
    }

    mrsToolsFree(pstMsgRcv->pucPacket);
    mrsToolsFree(pstMsgRcv);

    return ulRet;
}


HI_U32 mrsCltIUpgStartProc(HI_MDM_DEVICE_UPG_START_REQ_S *pstReq)
{
    MRS_CLTI_UPG_MODULE_STRU *pstUpgCtx = mrsGetCltIUpgCtx();
    HI_MDM_DEVICE_UPG_START_IND_S stInd = {0};

    (hi_void)memset_s(&stInd, sizeof(stInd), 0, sizeof(stInd));

    do 
    {
        if (HI_MDM_DEVICE_UPG_STATUS_PROGRESS == pstUpgCtx->ucStatus)
        {
            stInd.ulRc = HI_ERR_BUSY;
            stInd.ulUpgId = pstUpgCtx->ulUpgId;
            break;
        }

        if (!pstUpgCtx->pucCache)
        {
            pstUpgCtx->pucCache = (HI_PBYTE)mrsToolsMalloc(PRODUCT_CFG_FLASH_BLOCK_SIZE);
        }

        if (!pstUpgCtx->pucCache)
        {
            stInd.ulRc = HI_ERR_NO_MORE_MEMORY;
            stInd.ulUpgId = pstReq->ulUpgId;
            break;
        }

        pstUpgCtx->ucStatus = HI_MDM_DEVICE_UPG_STATUS_PROGRESS;
        pstUpgCtx->ulUpgId = pstReq->ulUpgId;
        pstUpgCtx->ulFileLength = pstReq->ulFileSize;
        pstUpgCtx->pucFileData = pstReq->pucFileData;
        pstUpgCtx->ulOffset = 0;

        stInd.ulRc = HI_ERR_SUCCESS;
        stInd.ulUpgId = pstUpgCtx->ulUpgId;
    } while (0);

    // TODO: 开始升级流程
    MRS_StartTimer(MRS_STA_TIMER_CLTI_UPG, MRS_STA_TIME_CLTI_UPG, HI_SYS_TIMER_ONESHOT);

    return HI_MDM_UPG_MsgSend(HI_MDM_DEVICE_UPG_START, (HI_PBYTE)&stInd, (HI_U32)sizeof(stInd));
}


HI_U32 mrsCltIUpgStopProc(HI_MDM_DEVICE_UPG_STOP_REQ_S *pstReq)
{
    MRS_CLTI_UPG_MODULE_STRU *pstUpgCtx = mrsGetCltIUpgCtx();
    HI_MDM_DEVICE_UPG_STOP_IND_S stInd = {0};
    
    (hi_void)memset_s(&stInd, sizeof(stInd), 0, sizeof(stInd));
    stInd.ulRc = HI_ERR_SUCCESS;
    stInd.ulUpgId = pstUpgCtx->ulUpgId;

    if (HI_MDM_DEVICE_UPG_STATUS_PROGRESS == pstUpgCtx->ucStatus)
    {
        if ((0 != pstReq->ulUpgId) && (pstReq->ulUpgId != pstUpgCtx->ulUpgId))
        {
            stInd.ulRc = HI_ERR_FAILURE;
        }
        else
        {
            // TODO: 停止升级动作
            MRS_StopTimer(MRS_STA_TIMER_CLTI_UPG);

            mrsToolsFree(pstUpgCtx->pucCache);
            (hi_void)memset_s(pstUpgCtx, sizeof(MRS_CLTI_UPG_MODULE_STRU), 0, sizeof(MRS_CLTI_UPG_MODULE_STRU));
            pstUpgCtx->ucStatus = HI_MDM_DEVICE_UPG_STATUS_IDLE;
        }
    }

    return HI_MDM_UPG_MsgSend(HI_MDM_DEVICE_UPG_STOP, (HI_PBYTE)&stInd, (HI_U32)sizeof(stInd));
}


HI_U32 mrsCltIUpgStatusProc(HI_MDM_DEVICE_UPG_STATUS_REQ_S *pstReq)
{
    return mrsCltIUpgStatusReport();
}


HI_U32 mrsCltIUpgStatusReport(HI_VOID)
{
    MRS_CLTI_UPG_MODULE_STRU *pstUpgCtx = mrsGetCltIUpgCtx();
    HI_MDM_DEVICE_UPG_STATUS_IND_S stInd = {0};

    (hi_void)memset_s(&stInd, sizeof(stInd), 0, sizeof(stInd));
    stInd.ulUpgId = pstUpgCtx->ulUpgId;
    stInd.ucStatus = pstUpgCtx->ucStatus;

    return HI_MDM_UPG_MsgSend(HI_MDM_DEVICE_UPG_STATUS, (HI_PBYTE)&stInd, (HI_U32)sizeof(stInd));
}


HI_U32 mrsCltIUpgTimerProc(HI_VOID)
{
    MRS_CLTI_UPG_MODULE_STRU *pstUpgCtx = mrsGetCltIUpgCtx();
    HI_U32 ulBufLen = PRODUCT_CFG_FLASH_BLOCK_SIZE;
    HI_U32 ulRet = HI_ERR_SUCCESS;

    if ((HI_MDM_DEVICE_UPG_STATUS_PROGRESS != pstUpgCtx->ucStatus)
        || !pstUpgCtx->pucCache)
    {
        return HI_ERR_FAILURE;
    }

    do 
    {
        if (pstUpgCtx->ulFileLength <= pstUpgCtx->ulOffset)
        {
            pstUpgCtx->ucStatus = HI_MDM_DEVICE_UPG_STATUS_SUCCESS;
            break;
        }

        if (pstUpgCtx->ulOffset + PRODUCT_CFG_FLASH_BLOCK_SIZE > pstUpgCtx->ulFileLength)
        {
            ulBufLen = pstUpgCtx->ulFileLength - pstUpgCtx->ulOffset;
        }

        if (ulBufLen != HI_MDM_UPG_MemRead(pstUpgCtx->ulOffset, pstUpgCtx->pucCache, ulBufLen))
        {
            pstUpgCtx->ucStatus = HI_MDM_DEVICE_UPG_STATUS_FAIL;
            break;
        }

        pstUpgCtx->ulOffset += ulBufLen;
        MRS_StartTimer(MRS_STA_TIMER_CLTI_UPG, MRS_STA_TIME_CLTI_UPG, HI_SYS_TIMER_ONESHOT);

        return HI_ERR_SUCCESS;
    } while (0);

    // 升级结束
    ulRet = mrsCltIUpgStatusReport();

    MRS_StopTimer(MRS_STA_TIMER_CLTI_UPG);
    mrsToolsFree(pstUpgCtx->pucCache);
    pstUpgCtx->pucFileData = HI_NULL;

    return ulRet;
}


//*****************************************************************************
//*****************************************************************************
#endif
#endif
HI_END_HEADER
