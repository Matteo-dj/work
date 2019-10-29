//*****************************************************************************
//
//                  版权所有 (C), 2001-2014, 华为技术有限公司
//
//*****************************************************************************
//  文 件 名   : mrs_srv_clti_searchmeter_csg.c
//  版 本 号   : V1.0
//  作    者   : cuiate/c00233580
//  生成日期   : 2015-12-26
//  功能描述   : TODO: ...
//
//  函数列表   : TODO: ...
//  修改历史   :
//  1.日    期 : 2015-12-26
//    作    者 : cuiate/c00233580
//    修改内容 : 创建文件
//
//*****************************************************************************

#include "mrs_common.h"
#include "mrs_fw_n.h"
#include "mrs_fw_proto_csg_echo.h"
#include "mrs_fw_proto_csg.h"
#include "mrs_fw_tools.h"
#include "mrs_fw_log.h"
#include "mrs_srv_res.h"
#include "mrs_srv_common.h"
#include "mrs_srv_sta_queue.h"
#include "mrs_srv_sta_baudrate.h"
#include "mrs_srv_parallel_sta.h"
#include "mrs_srv_sta_event.h"
#include "mrs_srv_sta.h"
#include "mrs_srv_clti_upg_csg.h"
#include "hi_mdm.h"
#include "mrs_srv_clti_searchmeter_csg.h"
#include "mrs_srv_cco_remote_upg.h"
#include "mrs_fw_nv.h"

HI_START_HEADER

#if (defined(PRODUCT_CFG_PRODUCT_TYPE_STA) && defined(PRODUCT_CFG_PRODUCT_TYPE_CSG_GD))
HI_U32 mrsCsgCltIUpgStartMsg(HI_MDM_DEVICE_UPG_START_REQ_S * pstReq);

HI_U32 mrsCsgCltIUpgStopMsg( HI_MDM_DEVICE_UPG_STOP_REQ_S * pstReq);

HI_U32 mrsCltIUpgStatusProc(HI_MDM_DEVICE_UPG_STATUS_REQ_S *pstReq);

HI_U32 mrsCltIUpgStatusReport(HI_VOID);

MRS_CSG_UPG_SEG_STRU *mrsCsgCltIUpgGetSeg(HI_VOID);

HI_U32 mrsCsgCltIUpgSetInfo(HI_MDM_DEVICE_UPG_START_REQ_S *pstReq);
HI_U32 mrsCsgCltICalcSeg(HI_U32 ulFileSize);

HI_U32 mrsCsgCltIUpgStartInd(HI_U16 usId, HI_U32 ulResult);
HI_U32 mrsCsgCltIUpgFrameEnQueue(HI_U8 *pucBuf, HI_U16 usLength, HI_U8 ucType);
HI_U32 mrsCsgCltIUpgStartProc(HI_VOID);

HI_U32 mrsCsgCltIUpgMsgNotify(HI_U32 usId, HI_PBYTE pCmd, HI_U16 usCmdSize);  

HI_U32 mrsCsgCltIUpgGetNV(HI_VOID);
HI_U32 mrsCsgCltIUpgCreatVersionFrame(HI_VOID *pParam, HI_U8 **ppFrame, HI_U16 *pusLength);
HI_U32 mrsCsgCltIUpgSetNVUpgId(HI_U32 ulUpgId);
HI_U16 mrsCsgCltICaleFileCrc16(HI_BYTE *pucBuffer, HI_U32 ulLength);

MRS_CSG_CLTI_UPG_MODULE_STRU g_stCsgCltIUpgCtx;

//升级初始化
HI_U32 mrsCsgCltIUpgInit(HI_VOID)
{
    HI_U32 ulRet = HI_ERR_SUCCESS;
    NV_APP_MR_CFG_STRU stCfg;
    
    HI_MDM_RegisterUpgMsgNotify(HI_MDM_DEVICE_UPG_START, mrsCsgCltIUpgMsgNotify);
    HI_MDM_RegisterUpgMsgNotify(HI_MDM_DEVICE_UPG_STOP, mrsCsgCltIUpgMsgNotify);
    HI_MDM_RegisterUpgMsgNotify(HI_MDM_DEVICE_UPG_STATUS, mrsCsgCltIUpgMsgNotify);

    (hi_void)memset_s(&g_stCsgCltIUpgCtx, sizeof(g_stCsgCltIUpgCtx), 0, sizeof(g_stCsgCltIUpgCtx));
    g_stCsgCltIUpgCtx.ucMaxTry = MRS_CSG_CLTI_UPG_MAXTRY;
    g_stCsgCltIUpgCtx.ucProcess = MRS_CSG_CLTI_UPG_IDLE;

    (hi_void)memset_s(&stCfg, sizeof(stCfg), 0,sizeof(NV_APP_MR_CFG_STRU));
    MRS_GetAppMrNvCfg(&stCfg);
    g_stCsgCltIUpgCtx.usCrcValue = stCfg.usTestFrameInterval_usCrcInitValue;
    
    mrsCsgCltIUpgGetNV();

    return ulRet;
}

HI_VOID mrsGetCltIUpgParam(NV_APP_MR_CFG_STRU* pstcfg)
{
    MRS_CSG_CLTI_UPG_MODULE_STRU *pstUpgCtx = mrsCsgCltIUpgGetCtx();
    pstUpgCtx->usCrcValue = pstcfg->usTestFrameInterval_usCrcInitValue;
}

HI_U32 mrsCsgCltIUpgGetNV(HI_VOID)
{
    NV_APP_CLTI_CSG_CFG_STRU stCfg = {0};
    MRS_CSG_CLTI_UPG_MODULE_STRU *pstUpgCtx = mrsCsgCltIUpgGetCtx();
    HI_U32 ulRet = HI_ERR_SUCCESS;
    
    (hi_void)memset_s(&stCfg, sizeof(stCfg), 0, sizeof(stCfg));
    ulRet = HI_MDM_NV_Read(ID_NV_APP_CLTI_CFG, (HI_PVOID)&stCfg, sizeof(stCfg));
    if(HI_ERR_SUCCESS != ulRet)
    {
        stCfg.ucStatus = 0;
        stCfg.ulUpgId = 0;
        stCfg.ucFileId = 0;
    }

    pstUpgCtx->ucStatus = stCfg.ucStatus;
    pstUpgCtx->ulUpgId = stCfg.ulUpgId;
    mrsCsgCltIUpgGetItem()->ucFileId = stCfg.ucFileId;
    return ulRet;
}


HI_U32 mrsCsgCltIUpgSetNV(HI_VOID)
{
    NV_APP_CLTI_CSG_CFG_STRU stNv = {0};
    MRS_CSG_CLTI_UPG_MODULE_STRU *pstUpgCtx = mrsCsgCltIUpgGetCtx();
    HI_U32 ulRet = HI_ERR_SUCCESS;

    ulRet = HI_MDM_NV_Read(ID_NV_APP_CLTI_CFG, (HI_PVOID)&stNv, sizeof(stNv));
    if (ulRet == HI_ERR_SUCCESS)
    {
        stNv.ucStatus = pstUpgCtx->ucStatus;
        stNv.ulUpgId = pstUpgCtx->ulUpgId;
        stNv.ucFileId = mrsCsgCltIUpgGetItem()->ucFileId;

        ulRet = HI_MDM_NV_Write(ID_NV_APP_CLTI_CFG, (HI_PVOID)&stNv, (HI_U16)sizeof(stNv));
    }

    return ulRet;
}

HI_U32 mrsCsgCltIUpgSetNVUpgId(HI_U32 ulUpgId)
{
    NV_APP_CLTI_CSG_CFG_STRU stNv = {0};
    HI_U32 ulRet = HI_ERR_SUCCESS;

    ulRet = HI_MDM_NV_Read(ID_NV_APP_CLTI_CFG, (HI_PVOID)&stNv, sizeof(stNv));
    if (ulRet == HI_ERR_SUCCESS)
    {
        stNv.ulUpgId = ulUpgId;
        stNv.ucStatus = HI_MDM_DEVICE_UPG_STATUS_IDLE;
        ulRet = HI_MDM_NV_Write(ID_NV_APP_CLTI_CFG, (HI_PVOID)&stNv, (HI_U16)sizeof(stNv));
    }

    return ulRet;
}


MRS_CSG_CLTI_UPG_MODULE_STRU *mrsCsgCltIUpgGetCtx(HI_VOID)
{
    return &g_stCsgCltIUpgCtx;
}


// I采升级模块去初始化
HI_U32 mrsCsgCltIUpgDeinit(HI_VOID)
{
    MRS_CSG_CLTI_UPG_MODULE_STRU *pstUpgCtx = mrsCsgCltIUpgGetCtx();
    MRS_CSG_UPG_ITEM_STRU * pstItem = &pstUpgCtx->stItem;

    MRS_StopTimer(MRS_STA_TIMER_CSG_CLTI_UPG);
    mrsToolsFree(pstUpgCtx->pucCache);

    pstUpgCtx->ucTry = 0;
    pstUpgCtx->ucMaxTry = MRS_CSG_CLTI_UPG_MAXTRY;
    pstUpgCtx->usLastLen = 0;

    pstUpgCtx->usFrameLength = 0;
    mrsToolsFree(pstUpgCtx->pucFrame);

    mrsCsgCltIUpgSegInit();
    (hi_void)memset_s((HI_U8 *)pstItem, sizeof(MRS_CSG_UPG_ITEM_STRU), 0, sizeof(MRS_CSG_UPG_ITEM_STRU));

    pstUpgCtx->ucStatus = HI_MDM_DEVICE_UPG_STATUS_IDLE;
    pstUpgCtx->ucProcess = MRS_CSG_CLTI_UPG_IDLE;
    return HI_ERR_SUCCESS;
}


HI_VOID mrsCsgCltIUpgSegInit(HI_VOID)
{
    MRS_CSG_UPG_SEG_STRU *pstSeg = mrsCsgCltIUpgGetSeg();
    (hi_void)memset_s((HI_U8 *)pstSeg, sizeof(MRS_CSG_UPG_SEG_STRU), 0, sizeof(MRS_CSG_UPG_SEG_STRU));
}

MRS_CSG_UPG_ITEM_STRU *mrsCsgCltIUpgGetItem(HI_VOID)
{
    return &g_stCsgCltIUpgCtx.stItem;
}

MRS_CSG_UPG_SEG_STRU *mrsCsgCltIUpgGetSeg(HI_VOID)
{
    return &g_stCsgCltIUpgCtx.stCurSeg;
}

HI_VOID mrsCsgCltIUpgGetCurSeg()
{
    MRS_CSG_UPG_SEG_STRU * pstCurSeg = mrsCsgCltIUpgGetSeg();
    pstCurSeg->usCurrNo++;
}

HI_VOID mrsCsgCltIUpgSetStatus(HI_U8 ucStatus)
{
    mrsCsgCltIUpgGetCtx()->ucStatus = ucStatus;
}


HI_U8 mrsCsgCltIUpgGetStatus(HI_VOID)
{
    return mrsCsgCltIUpgGetCtx()->ucStatus;
}

HI_VOID mrsCsgCltIUpgSetProcess(HI_U8 ucProcess)
{
    mrsCsgCltIUpgGetCtx()->ucProcess = ucProcess;
}


HI_U8 mrsCsgCltIUpgGetProcess(HI_VOID)
{
    return mrsCsgCltIUpgGetCtx()->ucProcess;
}

HI_U32 mrsCsgCltIUpgMsgNotify(HI_U32 ulId, HI_PBYTE pCmd, HI_U16 usCmdSize)
{
    HI_MDM_MSG_APPRCV_S *pstMsgRcv = HI_NULL;
    HI_SYS_QUEUE_MSG_S stMsg = {0};
    HI_U32 ulRet = HI_ERR_SUCCESS;

    if (ulId < HI_MDM_DEVICE_UPG_START || ulId > HI_MDM_DEVICE_UPG_STATUS)
    {
        return HI_ERR_INVALID_ID;
    }

    pstMsgRcv = (HI_MDM_MSG_APPRCV_S *)mrsToolsMalloc(sizeof(HI_MDM_MSG_APPRCV_S));
    if (!pstMsgRcv)
    {
        return HI_ERR_NO_MORE_MEMORY;
    }

    (hi_void)memset_s(pstMsgRcv, sizeof(HI_MDM_MSG_APPRCV_S), 0, sizeof(HI_MDM_MSG_APPRCV_S));
    pstMsgRcv->usPacketSize = usCmdSize;
    pstMsgRcv->stCtrl.usTransId = HI_MDM_OBJ_ID_MRS;
    pstMsgRcv->pucPacket = (HI_PBYTE)mrsToolsMalloc(pstMsgRcv->usPacketSize);

    if (!pstMsgRcv->pucPacket)
    {
        mrsToolsFree(pstMsgRcv);
        return HI_ERR_NO_MORE_MEMORY;
    }

    (hi_void)memset_s(pstMsgRcv->pucPacket, pstMsgRcv->usPacketSize, 0, pstMsgRcv->usPacketSize);
    (hi_void)memcpy_s(pstMsgRcv->pucPacket, pstMsgRcv->usPacketSize, pCmd, pstMsgRcv->usPacketSize);
    (hi_void)memset_s(&stMsg, sizeof(stMsg), 0, sizeof(stMsg));

    stMsg.ulMsgId = EN_MRS_FW_MSG_MDM_INPUT;
    stMsg.ulParam[0] = ulId;
    stMsg.ulParam[1] = (HI_U32)pstMsgRcv;
    ulRet = mrsSendMessage2Queue(&stMsg);
    if (HI_ERR_SUCCESS != ulRet)
    {
        mrsToolsFree(pstMsgRcv->pucPacket);
        mrsToolsFree(pstMsgRcv);
    }

    return ulRet;
}


//接收UPG模块消息处理
HI_U32 mrsCsgCltIUpgMsgProc(HI_SYS_QUEUE_MSG_S *pstMsg)
{
    HI_U32 ulRet = HI_ERR_FAILURE;
    HI_U16 usMsgId = (HI_U16)pstMsg->ulParam[0];
    HI_MDM_MSG_APPRCV_S* pstMsgRcv = (HI_MDM_MSG_APPRCV_S *)pstMsg->ulParam[1];

    if (!pstMsgRcv)
    {
        return HI_ERR_INVALID_PARAMETER;
    }

    switch(usMsgId)
    {
    case HI_MDM_DEVICE_UPG_START:
        ulRet = mrsCsgCltIUpgStartMsg((HI_MDM_DEVICE_UPG_START_REQ_S *)pstMsgRcv->pucPacket);
        break;

    case HI_MDM_DEVICE_UPG_STOP:
        ulRet = mrsCsgCltIUpgStopMsg((HI_MDM_DEVICE_UPG_STOP_REQ_S *)pstMsgRcv->pucPacket);
        break;

    case HI_MDM_DEVICE_UPG_STATUS:
        ulRet = mrsCsgCltIUpgStatusMsg((HI_MDM_DEVICE_UPG_STATUS_REQ_S *)pstMsgRcv->pucPacket);
        break;

    default:
        ulRet = HI_ERR_INVALID_ID;
        break;
    }

    mrsToolsFree(pstMsgRcv->pucPacket);
    mrsToolsFree(pstMsgRcv);

    return ulRet;
}


//接收到UPG消息处理
HI_U32 mrsCsgCltIUpgStartMsg(HI_MDM_DEVICE_UPG_START_REQ_S * pstReq)
{
    HI_U32 ulRet = HI_ERR_SUCCESS;
    HI_U8 ucStatus = mrsCsgCltIUpgGetStatus();

    HI_DIAG_LOG_MSG_E0(__LINE__, HI_DIAG_MT("Resv message start upg file"));

    if (!pstReq || (pstReq->ulFileSize == 0))
    {
        return HI_ERR_INVALID_PARAMETER;
    }
            
    do
    {
        if (ucStatus == HI_MDM_DEVICE_UPG_STATUS_PROGRESS)
        {
            ulRet = HI_ERR_BUSY;
            break;
        }

        ulRet = mrsCsgCltIUpgSetInfo(pstReq);
        if (HI_ERR_SUCCESS != ulRet)
        {
            break;
        }
    } while(HI_FALSE);

    ulRet = mrsCsgCltIUpgStartInd(HI_MDM_DEVICE_UPG_START,ulRet);
    if (HI_ERR_SUCCESS != ulRet)
    {
        return ulRet;
    }

    mrsCsgCltIUpgSetNVUpgId(pstReq->ulUpgId);
    //停止采集器搜表
    mrsCsgCltIStopSearch();

    //设置为升级状态和结果
    mrsCsgCltIUpgSetStatus(HI_MDM_DEVICE_UPG_STATUS_PROGRESS);
    mrsCsgCltIUpgSetProcess(MRS_CSG_CLTI_UPG_QRY_INFOS);
    
    // 开始文件升级相关处理
    MRS_StartTimer(MRS_STA_TIMER_CSG_CLTI_UPG, MRS_STA_TIME_CSG_CLTI_UPG, HI_SYS_TIMER_ONESHOT);

    return ulRet;
}


//接收到UPG消息处理
HI_U32 mrsCsgCltIUpgStopMsg(HI_MDM_DEVICE_UPG_STOP_REQ_S * pstReq)
{
    HI_U32 ulRet = HI_ERR_SUCCESS;
    MRS_CSG_CLTI_UPG_MODULE_STRU * pstUpgCtx = mrsCsgCltIUpgGetCtx();

    do
    {
        if ((HI_U8)pstReq->ulUpgId != pstUpgCtx->ulUpgId)
        {
            ulRet = HI_ERR_FAILURE;
            break;
        }

        mrsCsgCltIUpgDeinit();
        MRS_StartTimer(MRS_STA_TIMER_CSG_CLTI_SM_QUERY, 200, HI_SYS_TIMER_ONESHOT);
    } while(HI_FALSE);

    return mrsCsgCltIUpgStartInd(HI_MDM_DEVICE_UPG_STOP,ulRet);
}


//接收到UPG查询升级状态消息处理
HI_U32 mrsCsgCltIUpgStatusMsg(HI_MDM_DEVICE_UPG_STATUS_REQ_S *pstReq)
{
    HI_UNREF_PARAM(pstReq);
    return mrsCsgCltIUpgStatusInd();
}


HI_U32 mrsCsgCltIUpgGetStart(HI_U8 **ppBuffer, HI_U16 *pusLength)
{
    HI_U8 *pData = HI_NULL;
    HI_U16 usDataLen = MRS_CSG_UPG_FIX_LEN_START;
    MRS_CSG_UPG_ITEM_STRU *pstItem = mrsCsgCltIUpgGetItem();

    pData = mrsToolsMalloc(usDataLen);
    if (!pData)
    {
        return HI_ERR_NOT_ENOUGH_MEMORY;
    }

    (hi_void)memset_s(pData, usDataLen, 0, usDataLen);
    *ppBuffer = pData;
    *pusLength = usDataLen;

    *pData = pstItem->ucFileId;
    pData++;

    (hi_void)memcpy_s(pData, HI_METER_ADDR_LEN, pstItem->aucDestAddr, HI_METER_ADDR_LEN);
    pData += HI_METER_ADDR_LEN;

    (hi_void)memcpy_s(pData, sizeof(pstItem->usTotalSeg), (HI_U8 *)&pstItem->usTotalSeg, sizeof(pstItem->usTotalSeg));
    pData += sizeof(HI_U16);

    (hi_void)memcpy_s(pData, sizeof(HI_U32), (HI_U8 *)&pstItem->ulFileLength, sizeof(HI_U32));
    pData += sizeof(HI_U32);

    (hi_void)memcpy_s(pData, sizeof(pstItem->usTotalCs), (HI_U8 *)&pstItem->usTotalCs, sizeof(pstItem->usTotalCs));
    pData += sizeof(HI_U16);

    return HI_ERR_SUCCESS;
}

HI_U32 mrsCsgCltIUpgGetTransimit(HI_U8 **ppBuffer, HI_U16 *pusLength)
{
    MRS_CSG_UPG_SEG_STRU *pstSeg = mrsCsgCltIUpgGetSeg();
    HI_U8 *pData = HI_NULL;
    HI_U16 usDataLen = MRS_CSG_UPG_FIX_LEN_TRANSIMT + pstSeg->usLen;

    pData = mrsToolsMalloc(usDataLen);
    if (!pData)
    {
        return HI_ERR_NOT_ENOUGH_MEMORY;
    }

    (hi_void)memset_s(pData, usDataLen, 0, usDataLen);
    *ppBuffer = pData;
    *pusLength = usDataLen;

    (hi_void)memcpy_s(pData, sizeof(HI_U16), (HI_U8 *)&pstSeg->usCurrNo, sizeof(HI_U16));
    pData += sizeof(HI_U16);

    (hi_void)memcpy_s(pData, sizeof(HI_U16), (HI_U8 *)&pstSeg->usLen, sizeof(HI_U16));
    pData += sizeof(HI_U16);

    if (mrsCsgCltIUpgGetCtx()->pucCache)
    {
        (hi_void)memcpy_s(pData, pstSeg->usLen, mrsCsgCltIUpgGetCtx()->pucCache, pstSeg->usLen);
    }
    pData += pstSeg->usLen;

    (hi_void)memcpy_s(pData, sizeof(HI_U16), (HI_U8 *)&pstSeg->usCs, sizeof(HI_U16));
    pData += sizeof(HI_U16);

    return HI_ERR_SUCCESS;
}

HI_U32 mrsCsgCltIUpgCalcSeg(HI_VOID)
{
    MRS_CSG_CLTI_UPG_MODULE_STRU * pstUpg = mrsCsgCltIUpgGetCtx();
    MRS_CSG_UPG_SEG_STRU * pstUpgSeg = &pstUpg->stCurSeg;
    HI_U32 ulSegNo = pstUpgSeg->usCurrNo;
    HI_U32 ulBufLen = MRS_CLTI_UPG_FIX_SEG_LEN;

    if (!pstUpg->pucCache)
    {
        return HI_ERR_INVALID_PARAMETER;
    }

    if ((ulSegNo + 1 == pstUpg->stItem.usTotalSeg) &&(pstUpg->usLastLen != 0))
    {
        pstUpgSeg->usLen = pstUpg->usLastLen;
    }
    else
    {
        pstUpgSeg->usLen = MRS_CLTI_UPG_FIX_SEG_LEN;
    }

    if (ulBufLen != HI_MDM_UPG_MemRead(ulSegNo * MRS_CLTI_UPG_FIX_SEG_LEN, pstUpg->pucCache, ulBufLen))
    {
        pstUpg->ucStatus = HI_MDM_DEVICE_UPG_STATUS_FAIL;
        return HI_ERR_FAILURE;
    }
   
    pstUpgSeg->usCs = HI_MDM_Crc16(pstUpg->usCrcValue, pstUpg->pucCache, pstUpgSeg->usLen);

    return HI_ERR_SUCCESS;
}

HI_U32 mrsCsgCltIUpgCreatStartFrame(HI_VOID *pParam, HI_U8 **ppFrame, HI_U16 *pusLength)
{
    HI_U32 ulRet = HI_ERR_SUCCESS;
    MRS_CSG_IND_FRAME_STRU stIndFrame = {0};
    MRS_NOT_USED(pParam);

    (hi_void)memset_s(&stIndFrame, sizeof(stIndFrame), 0, sizeof(stIndFrame));

    stIndFrame.ucAfn = MRS_CSG_AFN_UPG_FILE_24H;
    stIndFrame.ulDI = MRS_CSG_DI_REQ_UPG_START;
    stIndFrame.ucSeq =  ++mrsStaGetContext()->ucSeq;
    stIndFrame.ucPrm = PROTOCSG_CTRL_PRM_BIT_REQ;
    stIndFrame.ucAddrFlag = PROTOCSG_CTRL_ADD_BIT_N;

    ulRet = mrsCsgCltIUpgGetStart(&stIndFrame.pAppData,&stIndFrame.usAppDataLen);
    if (ulRet != HI_ERR_SUCCESS)
    {
        return ulRet;
    }

    ulRet = mrsCreateCsgFrame(&stIndFrame, ppFrame, pusLength);
    mrsToolsFree(stIndFrame.pAppData);

    return ulRet;
}


HI_U32 mrsCsgCltIUpgCreatTransmitFrame(HI_VOID *pParam, HI_U8 **ppFrame, HI_U16 *pusLength)
{
    HI_U32 ulRet = HI_ERR_SUCCESS;
    MRS_CSG_IND_FRAME_STRU stIndFrame = {0};

    MRS_NOT_USED(pParam);

    (hi_void)memset_s(&stIndFrame, sizeof(stIndFrame), 0, sizeof(stIndFrame));
    stIndFrame.ucAfn = MRS_CSG_AFN_UPG_FILE_24H;
    stIndFrame.ulDI = MRS_CSG_DI_REQ_UPG_TRANSMIT;
    stIndFrame.ucSeq =  ++mrsStaGetContext()->ucSeq;
    stIndFrame.ucPrm = PROTOCSG_CTRL_PRM_BIT_REQ;
    stIndFrame.ucAddrFlag = PROTOCSG_CTRL_ADD_BIT_N;

    ulRet = mrsCsgCltIUpgGetTransimit(&stIndFrame.pAppData, &stIndFrame.usAppDataLen);
    if (HI_ERR_SUCCESS != ulRet)
    {
        return HI_ERR_FAILURE;
    }

    ulRet = mrsCreateCsgFrame(&stIndFrame, ppFrame, pusLength);
    mrsToolsFree(stIndFrame.pAppData);

    return ulRet;
}


HI_U32 mrsCsgCltIUpgCreateQryFrameNoData(HI_U8 ucAfn, HI_U32 ulDi, HI_U8 **ppFrame, HI_U16 *pusLength)
{
    MRS_STA_SRV_CTX_STRU * pstStaCtx = mrsStaGetContext();
    MRS_CSG_IND_FRAME_STRU stIndFrame;

    (hi_void)memset_s(&stIndFrame, sizeof(stIndFrame), 0, sizeof(stIndFrame));
    stIndFrame.ucAfn = ucAfn;
    stIndFrame.ulDI = ulDi;
    stIndFrame.ucSeq = ++pstStaCtx->ucSeq;
    stIndFrame.ucPrm = PROTOCSG_CTRL_PRM_BIT_REQ;
    stIndFrame.ucAddrFlag = PROTOCSG_CTRL_ADD_BIT_N;

    return mrsCreateCsgFrame(&stIndFrame, ppFrame, pusLength);
}


HI_U32 mrsCsgCltICreateFileProcessFrame(HI_VOID *pParam, HI_U8 **ppFrame, HI_U16 *pusLength)
{
    HI_UNREF_PARAM(pParam);
    return mrsCsgCltIUpgCreateQryFrameNoData(MRS_CSG_AFN_UPG_FILE_24H, MRS_CSG_DI_REQ_UPG_FILE_PROC, ppFrame, pusLength);
}


HI_U32 mrsCsgCltIUpgTx(HI_U8 ucStatus, HI_U32 (*pfnCreateFrame)(HI_VOID *, HI_U8 **, HI_U16 *), HI_VOID *pParam)
{
    MRS_STA_SRV_CTX_STRU * pstStaCtx = mrsStaGetContext();
    MRS_CSG_CLTI_UPG_MODULE_STRU *pstCltIUpgCtx = mrsCsgCltIUpgGetCtx();
    MRS_STA_ITEM * pstItem = HI_NULL;
    HI_U32 ulRet = HI_ERR_SUCCESS;

    MRS_StopTimer(MRS_STA_TIMER_CSG_CLTI_UPG);

    pstItem = mrsSrvQueueTop(&pstStaCtx->stQueModule.stMrQueue);
    if (pstItem
        && (pstItem->id == MRS_STA_ITEM_ID_CSG_CLTI_UPG_DEVICE))
    {
        mrsStaTryDeQueue(&pstStaCtx->stQueModule, mrsStaQueueFree);
    }

    if (pstCltIUpgCtx->ucTry >= pstCltIUpgCtx->ucMaxTry)
    {   
        mrsCsgCltIUpgEndProc(HI_MDM_DEVICE_UPG_STATUS_FAIL);
        return HI_ERR_FAILURE;
    }

    mrsCsgCltIUpgSetProcess(ucStatus);

    do
    {
        if (!pstCltIUpgCtx->pucFrame)
        {
            if (pfnCreateFrame)
            {
                ulRet = pfnCreateFrame(pParam, &pstCltIUpgCtx->pucFrame, &pstCltIUpgCtx->usFrameLength);
            }
            else
            {
                ulRet = HI_ERR_NOT_SUPPORT;
            }

            if (HI_ERR_SUCCESS != ulRet)
            {
                break;
            }
        }

        ulRet = mrsCsgCltIUpgFrameEnQueue(pstCltIUpgCtx->pucFrame, pstCltIUpgCtx->usFrameLength, ucStatus);
        if (HI_ERR_SUCCESS != ulRet)
        {
            mrsToolsFree(pstCltIUpgCtx->pucFrame);
            pstCltIUpgCtx->usFrameLength = 0;
            break;
        }
    } while (0);

    if (HI_ERR_SUCCESS != ulRet)
    {
        MRS_StartTimer(MRS_STA_TIMER_CSG_CLTI_UPG, MRS_STA_TIME_CSG_CLTI_UPG, HI_SYS_TIMER_ONESHOT);//!!!MRS_STA_TIME_CSG_CLTI_UPG
    }
    
    pstCltIUpgCtx->ucTry++;

    return ulRet;
}

HI_U32 mrsCsgCltICreateFileInfosFrame(HI_VOID *pParam, HI_U8 **ppFrame, HI_U16 *pusLength)
{
    HI_UNREF_PARAM(pParam);
    return mrsCsgCltIUpgCreateQryFrameNoData(MRS_CSG_AFN_UPG_FILE_24H, MRS_CSG_DI_REQ_UPG_FILE_INFO, ppFrame, pusLength);
}

//启动文件传输
HI_U32 mrsCsgCltIUpgStartProc(HI_VOID)
{
    return mrsCsgCltIUpgTx(MRS_CSG_CLTI_UPG_START, mrsCsgCltIUpgCreatStartFrame, HI_NULL);
}

//查询文件信息
HI_U32 mrsCsgCltIUpgQryInfosProc(HI_VOID)
{
    return mrsCsgCltIUpgTx(MRS_CSG_CLTI_UPG_QRY_INFOS, mrsCsgCltICreateFileInfosFrame, HI_NULL);
}

//查询文件处理进度
HI_U32 mrsCsgCltIUpgQryProc(HI_VOID)
{
    return mrsCsgCltIUpgTx(MRS_CSG_CLTI_UPG_QRY_PROCESS, mrsCsgCltICreateFileProcessFrame, HI_NULL);
}

HI_VOID mrsCsgCltIUpgEndProc(HI_U8 ucStatus)
{
    mrsCsgCltIUpgDeinit();
    mrsCsgCltIUpgSetStatus(ucStatus);
    mrsCsgCltIUpgSetNV();
    mrsCsgCltIUpgStatusInd();
    MRS_StartTimer(MRS_STA_TIMER_CSG_CLTI_SM_QUERY, 200, HI_SYS_TIMER_ONESHOT);
}

//传输文件内容
HI_U32 mrsCsgCltIUpgTransmitProc(HI_VOID)
{
    HI_U32 ulRet = HI_ERR_SUCCESS;

    //计算文件传输段信息
    ulRet = mrsCsgCltIUpgCalcSeg();
    if (HI_ERR_SUCCESS != ulRet)
    {
        mrsCsgCltIUpgEndProc(HI_MDM_DEVICE_UPG_STATUS_FAIL);
        return ulRet;
    }

    return mrsCsgCltIUpgTx(MRS_CSG_CLTI_UPG_TRANSIMIT, mrsCsgCltIUpgCreatTransmitFrame, HI_NULL);
}


HI_U32 mrsCsgCltIUpgFrameSend(MRS_STA_ITEM * pstItem)
{
    HI_U32 ulTimeout = 0;
    HI_U32 ulRet = HI_ERR_SUCCESS;
    if (HI_NULL == pstItem)
    {
        return HI_ERR_INVALID_PARAMETER;
    }

    ulRet = MRS_SendMrData(pstItem->buf, pstItem->len, HI_DMS_CHL_UART_PORT_APP);

    ulTimeout = MRS_100MS_TO_MS(pstItem->time_out);
    ulTimeout += pstItem->len * MRS_CLTI_SEND_BYTE_TIME_9600;

    MRS_StartTimer(MRS_STA_TIMER_CSG_CLTI_UPG, ulTimeout, HI_SYS_TIMER_ONESHOT);

    return ulRet;
}

HI_U32 mrsCsgCltIUpgTimeout(HI_VOID)
{
    HI_U32 ulRet = HI_ERR_FAILURE;
    HI_U8 ucProcess = mrsCsgCltIUpgGetCtx()->ucProcess;

    if ((HI_MDM_DEVICE_UPG_STATUS_PROGRESS != mrsCsgCltIUpgGetCtx()->ucStatus))
    {
        return HI_ERR_FAILURE;
    }

    switch (ucProcess)
    {
    case MRS_CSG_CLTI_UPG_START:
        ulRet = mrsCsgCltIUpgStartProc();
        break;

    case MRS_CSG_CLTI_UPG_TRANSIMIT:
        ulRet = mrsCsgCltIUpgTransmitProc();
        break;

    case MRS_CSG_CLTI_UPG_QRY_INFOS://查询文件信息(上电
        ulRet = mrsCsgCltIUpgQryInfosProc();//什么时候启动此查询
        break;

    case MRS_CSG_CLTI_UPG_QRY_PROCESS://查询文件处理进度
        ulRet = mrsCsgCltIUpgQryProc();
        break;

    default:
        break;
    }

    return ulRet;
}


HI_U32 mrsCsgCltIUpgFrameEnQueue(HI_U8 *pucBuf, HI_U16 usLength, HI_U8 ucType)
{
    MRS_STA_SRV_CTX_STRU *pstStaCtx = mrsStaGetContext();
    MRS_STA_ITEM * pstItem = HI_NULL;
    HI_U16 usPayloadLen = usLength;
    HI_U8* pucPayload = pucBuf;
    HI_U8 ucPreFECnt = MRS_PREFIX_FE_SIZE;

    pstItem = (MRS_STA_ITEM *)mrsToolsMalloc(sizeof(MRS_STA_ITEM) + usPayloadLen + ucPreFECnt);
    if (!pstItem)
    {
        return HI_ERR_NOT_ENOUGH_MEMORY;
    }

    (hi_void)memset_s(pstItem, sizeof(MRS_STA_ITEM) + usPayloadLen + ucPreFECnt, 0, sizeof(MRS_STA_ITEM) + usPayloadLen + ucPreFECnt);

    pstItem->id = MRS_STA_ITEM_ID_CSG_CLTI_UPG_DEVICE;
    pstItem->time_out = MRS_CSG_CLTI_UPG_TIMEOUT;
    pstItem->buf = (HI_U8 *)pstItem + sizeof(MRS_STA_ITEM);
    pstItem->len = usPayloadLen + ucPreFECnt;
    pstItem->from = MRS_STA_QUEUE_FROM_STA;
    pstItem->option = ucType;

    (hi_void)memset_s(pstItem->buf, pstItem->len, 0xFE, ucPreFECnt);
    (hi_void)memcpy_s(pstItem->buf + ucPreFECnt, pstItem->len - ucPreFECnt, pucPayload, usPayloadLen);

    if (!mrsStaTryEnQueue(&pstStaCtx->stQueModule, pstItem))
    {
        mrsToolsFree(pstItem);
        return HI_ERR_FAILURE;
    }

    mrsStaNotifyQueue();

    return HI_ERR_SUCCESS;
}


HI_U32 mrsCsgCltICalcSeg(HI_U32 ulFileSize)
{
    MRS_CSG_UPG_ITEM_STRU *pstItem = mrsCsgCltIUpgGetItem();
    pstItem->usTotalSeg = (HI_U16)(ulFileSize/MRS_CLTI_UPG_FIX_SEG_LEN);

    if (ulFileSize % MRS_CLTI_UPG_FIX_SEG_LEN == 0)
    {
        mrsCsgCltIUpgGetCtx()->usLastLen = 0;
    }
    else
    {
        pstItem->usTotalSeg++;
        mrsCsgCltIUpgGetCtx()->usLastLen = ulFileSize % MRS_CLTI_UPG_FIX_SEG_LEN;
    }

    return HI_ERR_SUCCESS;
}

//设置升级信息
HI_U32 mrsCsgCltIUpgSetInfo(HI_MDM_DEVICE_UPG_START_REQ_S *pstReq)
{
    MRS_CSG_CLTI_UPG_MODULE_STRU * pstUpgCtx = mrsCsgCltIUpgGetCtx();
    MRS_CSG_UPG_ITEM_STRU * pstUpgItem = mrsCsgCltIUpgGetItem();
    HI_U32 ulBufLen = MRS_CLTI_UPG_FIX_SEG_LEN;

    pstUpgCtx->ulFileLen = pstReq->ulFileSize;

    if (!pstUpgCtx->pucCache)
    {
        pstUpgCtx->pucCache = mrsToolsMalloc(ulBufLen);
        if (!pstUpgCtx->pucCache)
        {
            return HI_ERR_MALLOC_FAILUE;
        }

        (hi_void)memset_s(pstUpgCtx->pucCache, ulBufLen, 0, ulBufLen);
    }

    if (!pstReq->pucFileData)
    {
        if (ulBufLen != HI_MDM_UPG_MemRead(0, pstUpgCtx->pucCache, ulBufLen))
        {
            pstUpgCtx->ucStatus = HI_MDM_DEVICE_UPG_STATUS_FAIL;
            return HI_ERR_FAILURE;
        }
    }
    else
    {
        (hi_void)memcpy_s(pstUpgCtx->pucCache, ulBufLen, pstReq->pucFileData, ulBufLen);
    }

    pstUpgItem->usTotalCs = mrsCsgCltICaleFileCrc16(pstReq->pucFileData, pstUpgCtx->ulFileLen); 
    
    pstUpgItem->ulFileLength = pstUpgCtx->ulFileLen;
    (hi_void)memcpy_s(pstUpgItem->aucDestAddr, HI_METER_ADDR_LEN, MRS_BROADCAST_ADDR, HI_METER_ADDR_LEN);

    pstUpgCtx->ulUpgId = pstReq->ulUpgId;
    mrsCsgCltICalcSeg(pstUpgCtx->ulFileLen);
    HI_DIAG_LOG_MSG_E2(__LINE__, HI_DIAG_MT("pstReq info  "),mrsCsgCltIUpgGetItem()->ulFileLength,mrsCsgCltIUpgGetItem()->usTotalSeg);

    return HI_ERR_SUCCESS;
}


HI_U32 mrsCsgCltIUpgStartInd(HI_U16 usId, HI_U32 ulResult)
{
    HI_MDM_DEVICE_UPG_START_IND_S stInd;
	
    (hi_void)memset_s(&stInd, sizeof(stInd), 0, sizeof(HI_MDM_DEVICE_UPG_START_IND_S));
    stInd.ulRc = ulResult;
    stInd.ulUpgId = mrsCsgCltIUpgGetCtx()->ulUpgId;

    return HI_MDM_UPG_MsgSend(usId, (HI_U8*)&stInd, sizeof(stInd));
}

HI_U32 mrsCsgCltIUpgStatusInd(HI_VOID)
{
    MRS_CSG_CLTI_UPG_MODULE_STRU * pstUpg = mrsCsgCltIUpgGetCtx();
    HI_MDM_DEVICE_UPG_START_IND_S stInd;
	
    (hi_void)memset_s(&stInd, sizeof(stInd), 0, sizeof(HI_MDM_DEVICE_UPG_STATUS_IND_S));
    stInd.ulRc = pstUpg->ucStatus;
    stInd.ulUpgId = pstUpg->ulUpgId;

    return  HI_MDM_UPG_MsgSend(HI_MDM_DEVICE_UPG_STATUS, (HI_PBYTE)&stInd, (HI_U32)sizeof(stInd));
}

//根据文件处理进度设置升级状态
HI_VOID mrsCsgCltIUpgFileStatusProc(HI_U8 ucResult)
{
    HI_U8 ucStatus = ((ucResult == HI_ERR_SUCCESS) ? HI_MDM_DEVICE_UPG_STATUS_SUCCESS : HI_MDM_DEVICE_UPG_STATUS_PROGRESS);
    mrsCsgCltIUpgSetStatus(ucStatus);
}


HI_U32 mrsCsgCltIUpgJudgeRestart(MRS_CSG_UPG_ITEM_STRU *pstSrcItem, HI_U16 usCurSeg)
{
    MRS_CSG_CLTI_UPG_MODULE_STRU *pstUpgCtx = mrsCsgCltIUpgGetCtx();
    MRS_CSG_UPG_ITEM_STRU * pstItem = mrsCsgCltIUpgGetItem();

    //文件信息一致性判断
    if ((pstItem->ucFileId == pstSrcItem->ucFileId)
        && (pstItem->usTotalCs == pstSrcItem->usTotalCs)
        && (pstItem->usTotalSeg == pstSrcItem->usTotalSeg))
    {
        // 重新从M段传输
        if (usCurSeg >= pstItem->usTotalSeg)
        {
            return HI_ERR_SUCCESS;
        }

        mrsCsgCltIUpgGetSeg()->usCurrNo = usCurSeg;
        mrsCsgCltIUpgSetProcess(MRS_CSG_CLTI_UPG_TRANSIMIT);
    }
    else
    {
        // 文件不一致 开始重新传输
        mrsCsgCltIUpgSegInit();
        mrsCsgCltIUpgSetProcess(MRS_CSG_CLTI_UPG_START);
        pstItem->ucFileId++;
    }

    pstUpgCtx->ucTry = 0;
    pstUpgCtx->usFrameLength = 0;
    mrsToolsFree(pstUpgCtx->pucFrame);

    mrsCsgCltIUpgSetStatus(HI_MDM_DEVICE_UPG_STATUS_PROGRESS);
    MRS_StartTimer(MRS_STA_TIMER_CSG_CLTI_UPG, MRS_STA_TIME_CSG_CLTI_UPG, HI_SYS_TIMER_ONESHOT);

    return HI_ERR_CONTINUE;
}


HI_U16 mrsCsgCltICaleFileCrc16(HI_BYTE *pucBuffer, HI_U32 ulLength)
{
    HI_U16 usCrc = mrsCsgCltIUpgGetCtx()->usCrcValue;

    if ((pucBuffer == HI_NULL) && (0 != ulLength))
    {
        HI_U32 ulFileLen = ulLength;
        HI_U32 ulOffset = 0;
        HI_U8 *pucData = mrsToolsMalloc(PRODUCT_CFG_FLASH_BLOCK_SIZE);

        if (!pucData)
        {
            return 0;
        }

        while (ulFileLen > 0)
        {
            HI_U32 ulReadSize = (ulFileLen > PRODUCT_CFG_FLASH_BLOCK_SIZE) ? PRODUCT_CFG_FLASH_BLOCK_SIZE : ulFileLen;

            (hi_void)memset_s(pucData, PRODUCT_CFG_FLASH_BLOCK_SIZE, 0, PRODUCT_CFG_FLASH_BLOCK_SIZE);
            if (ulReadSize != HI_MDM_UPG_MemRead(ulOffset, pucData, ulReadSize))
            {
                mrsToolsFree(pucData);
                return 0;
            }

            usCrc = HI_MDM_Crc16(usCrc, (HI_CONST HI_U8 *)pucData, ulReadSize);

            ulOffset += ulReadSize;
            ulFileLen -= ulReadSize;
        }

        mrsToolsFree(pucData);
        return usCrc;
    }

    return HI_MDM_Crc16(usCrc, (HI_CONST HI_U8 *)pucBuffer, ulLength);
}


#endif
HI_END_HEADER

