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
#include "mrs_fw_proto376_2_echo.h"
#include "mrs_fw_log.h"
#include "mrs_srv_res.h"
#include "mrs_srv_common.h"
#include "mrs_srv_sta_queue.h"
#include "mrs_srv_sta_baudrate.h"
#include "mrs_srv_parallel_sta.h"
#include "mrs_srv_sta_event.h"
#include "mrs_srv_sta.h"
#include "mrs_srv_sta_searchmeter.h"
#include "mrs_srv_clti_searchmeter.h"
#include "mrs_srv_clti_searchmeter_csg.h"
#include "mrs_time.h"
HI_START_HEADER

#if (defined(PRODUCT_CFG_PRODUCT_TYPE_STA) && defined(PRODUCT_CFG_PRODUCT_TYPE_CSG_GD))

HI_PRV MRS_CSG_CLTI_SM_CONTEXT g_stCsgCltICtx = {0, MRS_CSG_CLTI_SM_MAXTRY, MRS_CSG_CLTI_STATUS_IDLE, HI_FALSE, 0, 0, HI_NULL};
HI_PRV HI_U32 mrsCsgCltIFrameEnQueue(HI_U8 *pucBuf, HI_U16 usLength, HI_U8 ucType);

HI_PRV HI_U32 mrsCsgCltICreateQryFrameNoData(HI_U8 ucAfn, HI_U32 ulDi, HI_U8 **ppFrame, HI_U16 *pusLength);
HI_PRV HI_U32 mrsCsgCltICreateMeterNumFrame(HI_VOID *pParam, HI_U8 **ppFrame, HI_U16 *pusLength);
HI_PRV HI_U32 mrsCsgCltICreateMeterInfFrame(HI_VOID *pParam, HI_U8 **ppFrame, HI_U16 *pusLength);
HI_PRV HI_U32 mrsCsgCltICreateDetectListFrame(HI_VOID *pParam, HI_U8 **ppFrame, HI_U16 *pusLength);
HI_PRV HI_U32 mrsCsgCltICreateDetectStatusFrame(HI_VOID *pParam, HI_U8 **ppFrame, HI_U16 *pusLength);
HI_PRV HI_U32 mrsCsgCltIQueryTx(HI_U8 ucStatus, HI_U32 (*pfnCreateFrame)(HI_VOID *, HI_U8 **, HI_U16 *), HI_VOID *pParam);
HI_PRV HI_U32 mrsCsgCltITryRead97(HI_VOID);
HI_PRV HI_U32 mrsCsgCltITryRead07(HI_VOID);
HI_PRV HI_U32 mrsCsgCltITryFirstProc(HI_U8 ucStatus);



MRS_CSG_CLTI_SM_CONTEXT *mrsCsgCltiCtx(HI_VOID)
{
    return &g_stCsgCltICtx;
}


HI_VOID mrsCsgCltISetStatus(HI_U8 ucStatus)
{
    mrsCsgCltiCtx()->ucStatus = ucStatus;
}


HI_U8 mrsCsgCltIGetStatus(HI_VOID)
{
    return mrsCsgCltiCtx()->ucStatus;
}


HI_VOID mrsCsgCltISetDetectStatus(HI_BOOL bStatus)
{
    mrsCsgCltiCtx()->bDetectStatus = bStatus;
}


HI_BOOL mrsCsgCltIIsDetectStatus(HI_VOID)
{
    return mrsCsgCltiCtx()->bDetectStatus;
}


HI_U32 mrsCsgCltIFrameEnQueue(HI_U8 *pucBuf, HI_U16 usLength, HI_U8 ucType)
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
    pstItem->id = MRS_STA_ITEM_ID_CSG_CLTI_SEARCH_METER;
    if ((MRS_CSG_CLTI_STATUS_FIRST_07 == ucType)
        ||(MRS_CSG_CLTI_STATUS_FIRST_97 == ucType))
    {
        pstItem->time_out = (HI_U8)MRS_MS_TO_100MS(mrsGet645FrameHeadTimeout(MRS_STA_RM_CFG_CLTI));
        pstItem->ucFrameTimeout = (HI_U8)MRS_MS_TO_100MS(mrsGet645FrameInterval(MRS_STA_RM_CFG_CLTI));
    }
    else
    {
        pstItem->time_out = (HI_U8)MRS_MS_TO_100MS(mrsGet645FrameHeadTimeout(MRS_STA_RM_CFG_LOCAL));
        pstItem->ucFrameTimeout = (HI_U8)MRS_MS_TO_100MS(mrsGet645FrameInterval(MRS_STA_RM_CFG_LOCAL));
    }

    pstItem->buf = (HI_U8 *)pstItem + sizeof(MRS_STA_ITEM);
    pstItem->len = usPayloadLen + ucPreFECnt;
    pstItem->from = MRS_STA_QUEUE_FROM_STA;
    pstItem->option = ucType;
    pstItem->bAutoStrategy = HI_TRUE;

#if (MRS_PREFIX_FE_SIZE > 0)
    (hi_void)memset_s(pstItem->buf, pstItem->len, 0xFE, ucPreFECnt);
#endif
    (hi_void)memcpy_s(pstItem->buf + ucPreFECnt, pstItem->len - ucPreFECnt, pucPayload, usPayloadLen);

    if (!mrsStaTryEnQueue(&pstStaCtx->stQueModule, pstItem))
    {
        mrsToolsFree(pstItem);
        return HI_ERR_FAILURE;
    }
    
    mrsStaNotifyQueue();
    
    return HI_ERR_SUCCESS;
}


HI_U32 mrsCsgCltIFrameTimeout(HI_VOID)
{
    MRS_STA_SRV_CTX_STRU *pstStaCtx = mrsStaGetContext();
    MRS_STA_ITEM * pstItem = HI_NULL;
    HI_U32 ulRet = HI_ERR_FAILURE;
    HI_U8 ucOption = 0;

    pstItem = mrsSrvQueueTop(&pstStaCtx->stQueModule.stMrQueue);
    if (!pstItem)
    {
        return HI_ERR_FAILURE;
    }

    ucOption = pstItem->option;
    if (pstItem->id == MRS_STA_ITEM_ID_CSG_CLTI_SEARCH_METER)
    {
        mrsStaTryDeQueue(&pstStaCtx->stQueModule, mrsStaQueueFree);
    }

    switch (ucOption)
    {
    case MRS_CSG_CLTI_STATUS_QRY_METER_NUM:
        ulRet = mrsCsgCltIQueryMeterNum();
        break;

    case MRS_CSG_CLTI_STATUS_QRY_METER_INF:
        ulRet = mrsCsgCltIQueryMeterInf();
        break;

    case MRS_CSG_CLTI_STATUS_QRY_DETECT_LIST:
        ulRet = mrsCsgCltISendDetectList(HI_NULL);
        break;

    case MRS_CSG_CLTI_STATUS_QRY_DETECT_STATUS:
        ulRet = mrsCsgCltIQueryDetectStatus();
        break;

    case MRS_CSG_CLTI_STATUS_FIRST_07:
        ulRet = mrsCsgCltITryRead07();
        break;

    case MRS_CSG_CLTI_STATUS_FIRST_97:
        ulRet = mrsCsgCltITryRead97();
        break;

    default:
        break;
    }

    return ulRet;
}


HI_U32 mrsCsgCltIQueryTx(HI_U8 ucStatus, HI_U32 (*pfnCreateFrame)(HI_VOID *, HI_U8 **, HI_U16 *), HI_VOID *pParam)
{
    MRS_STA_SRV_CTX_STRU * pstStaCtx = mrsStaGetContext();
    MRS_CSG_CLTI_SM_CONTEXT *pstCltISmCtx = mrsCsgCltiCtx();
    MRS_STA_ITEM * pstItem = HI_NULL;
    HI_U32 ulRet = HI_ERR_SUCCESS;

    MRS_StopTimer(MRS_STA_TIMER_CSG_CLTI_QUEUE);
    MRS_StopTimer(MRS_STA_TIMER_CSG_CLTI_SM_QUERY);

    pstItem = mrsSrvQueueTop(&pstStaCtx->stQueModule.stMrQueue);
    if (pstItem && pstItem->id == MRS_STA_ITEM_ID_CSG_CLTI_SEARCH_METER)
    {
        mrsStaTryDeQueue(&pstStaCtx->stQueModule, mrsStaQueueFree);
        mrsCsgCltISetStatus(MRS_CSG_CLTI_STATUS_IDLE);
        mrsCsgCltISetDetectStatus(HI_FALSE);
        mrsToolsFree(pstCltISmCtx->pucFrame);
        pstCltISmCtx->usFrameLength = 0;
        pstCltISmCtx->ucTry = 0;
    }

    if (pstCltISmCtx->ucTry >= pstCltISmCtx->ucMaxTry)
    {
        mrsCsgCltISetDetectStatus(HI_FALSE);
        mrsToolsFree(pstCltISmCtx->pucFrame);
        pstCltISmCtx->usFrameLength = 0;
        pstCltISmCtx->ucTry = 0;

        mrsCsgCltISetStatus(MRS_CSG_CLTI_STATUS_QRY_METER_INF);
        MRS_StartTimer(MRS_STA_TIMER_CSG_CLTI_SM_QUERY, 200, HI_SYS_TIMER_ONESHOT);
        return HI_ERR_FAILURE;
    }

    mrsCsgCltISetStatus(ucStatus);

    do
    {
        if (!pstCltISmCtx->pucFrame)
        {
            if (pfnCreateFrame)
            {
                ulRet = pfnCreateFrame(pParam, &pstCltISmCtx->pucFrame, &pstCltISmCtx->usFrameLength);
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

        ulRet = mrsCsgCltIFrameEnQueue(pstCltISmCtx->pucFrame, pstCltISmCtx->usFrameLength, ucStatus);
        if (HI_ERR_SUCCESS != ulRet)
        {
            if (MRS_CSG_CLTI_STATUS_QRY_DETECT_LIST == ucStatus)
            {
                mrsStaTryDeQueue(&pstStaCtx->stQueModule, mrsStaQueueFree);
                ulRet = mrsCsgCltIFrameEnQueue(pstCltISmCtx->pucFrame, pstCltISmCtx->usFrameLength, ucStatus);
            }
        }
        
        if (HI_ERR_SUCCESS != ulRet)
        {
            mrsToolsFree(pstCltISmCtx->pucFrame);
            pstCltISmCtx->usFrameLength = 0;
            break;
        }
    } while (0);

    if (HI_ERR_SUCCESS != ulRet)
    {
        MRS_StartTimer(MRS_STA_TIMER_CSG_CLTI_SM_QUERY, 200, HI_SYS_TIMER_ONESHOT);
        return ulRet;
    }
    
    pstCltISmCtx->ucTry++;

    return HI_ERR_SUCCESS;
}


HI_U32 mrsCsgCltICreateQryFrameNoData(HI_U8 ucAfn, HI_U32 ulDi, HI_U8 **ppFrame, HI_U16 *pusLength)
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


HI_U32 mrsCsgCltICreateMeterNumFrame(HI_VOID *pParam, HI_U8 **ppFrame, HI_U16 *pusLength)
{
    HI_UNREF_PARAM(pParam);
    return mrsCsgCltICreateQryFrameNoData(MRS_CSG_AFN_MANAGE_METER_21H, MRS_CSG_DI_REQ_METER_NUM, ppFrame, pusLength);
}


HI_U32 mrsCsgCltIQueryMeterNum(HI_VOID)
{
    return mrsCsgCltIQueryTx(MRS_CSG_CLTI_STATUS_QRY_METER_NUM, mrsCsgCltICreateMeterNumFrame, HI_NULL);
}


HI_U32 mrsCsgCltICreateMeterInfFrame(HI_VOID *pParam, HI_U8 **ppFrame, HI_U16 *pusLength)
{
    MRS_STA_SRV_CTX_STRU * pstStaCtx = mrsStaGetContext();
    MRS_CSG_IND_FRAME_STRU stIndFrame;
    HI_U32 ulRet = HI_ERR_SUCCESS;

    MRS_NOT_USED(pParam);

    (hi_void)memset_s(&stIndFrame, sizeof(stIndFrame), 0, sizeof(stIndFrame));
    stIndFrame.ucAfn = MRS_CSG_AFN_MANAGE_METER_21H;
    stIndFrame.ulDI = MRS_CSG_DI_REQ_METER_INF;
    stIndFrame.ucSeq = ++pstStaCtx->ucSeq;
    stIndFrame.ucPrm = PROTOCSG_CTRL_PRM_BIT_REQ;
    stIndFrame.ucAddrFlag = PROTOCSG_CTRL_ADD_BIT_N;
    stIndFrame.usAppDataLen = sizeof(HI_U8) + sizeof(HI_U8);    // start seq + count
    stIndFrame.pAppData = (HI_U8 *)mrsToolsMalloc(stIndFrame.usAppDataLen);
    if (!stIndFrame.pAppData)
    {
        return HI_ERR_MALLOC_FAILUE;
    }

    // 从0开始读取32个
    stIndFrame.pAppData[0] = 0;
    stIndFrame.pAppData[1] = 32;
    
    ulRet = mrsCreateCsgFrame(&stIndFrame, ppFrame, pusLength);
    mrsToolsFree(stIndFrame.pAppData);

    return ulRet;
}


HI_U32 mrsCsgCltITryFirst(HI_VOID)
{
    HI_U8 ucStatus = mrsCsgCltIGetStatus();

    switch (ucStatus)
    {
    case MRS_CSG_CLTI_STATUS_FIRST_07:
        mrsCsgCltITryRead07();
        break;

    case MRS_CSG_CLTI_STATUS_FIRST_97:
        mrsCsgCltITryRead97();
        break;

    default:
        break;
    }

    return HI_ERR_SUCCESS;
}

HI_U32 mrsCsgCltIQueryMeterInf(HI_VOID)
{
    HI_U8 ucStatus = mrsCsgCltIGetStatus();

    mrsStaSearchSetFirstFlag(HI_FALSE);

    switch (ucStatus)
    {
    case MRS_CSG_CLTI_STATUS_QRY_DETECT_LIST:
    case MRS_CSG_CLTI_STATUS_QRY_DETECT_STATUS:
    case MRS_CSG_CLTI_STATUS_QRY_METER_NUM:
        MRS_StartTimer(MRS_STA_TIMER_SM_PERIOD_NO_METER, MRS_CSG_CLTI_SM_INTERVAL, HI_SYS_TIMER_ONESHOT);
        return HI_ERR_SUCCESS;

    default:
        break;
    }

    return mrsCsgCltIQueryTx(MRS_CSG_CLTI_STATUS_QRY_METER_INF, mrsCsgCltICreateMeterInfFrame, HI_NULL);
}


HI_U32 mrsCsgCltIQueryMeterInfRx(MRS_CSG_FRAME_STRU *pstFrame)
{
    MRS_STA_SRV_CTX_STRU * pstStaCtx = mrsStaGetContext();
    MRS_STA_ITEM * pstItem = HI_NULL;
    MRS_CSG_CLTI_SM_CONTEXT *pstCltISmCtx = mrsCsgCltiCtx();
    MRS_STA_SEARCH_CTX * pstSmCtx = mrsStaSearchCtx();
    HI_U8 *pPayload = pstFrame->aucAppData;
    HI_U16 usPayloadLen = pstFrame->usAppDataLen;
    HI_U8 ucMeterNum = pstFrame->aucAppData[0];
    HI_U8 i = 0;

    if (MRS_CSG_CLTI_STATUS_QRY_METER_INF != pstCltISmCtx->ucStatus)
    {
        return HI_ERR_BAD_DATA;
    }

    if (usPayloadLen != (ucMeterNum * HI_METER_ADDR_LEN + 1))
    {
        return HI_ERR_BAD_DATA;
    }

    pstItem = mrsSrvQueueTop(&pstStaCtx->stQueModule.stMrQueue);
    if (!pstItem || (pstItem->id != MRS_STA_ITEM_ID_CSG_CLTI_SEARCH_METER))
    {
        return HI_ERR_FAILURE;
    }

    mrsStaStopTtlTimer(pstItem);
    mrsStaTryDeQueue(&pstStaCtx->stQueModule, mrsStaQueueFree);

    MRS_StopTimer(MRS_STA_TIMER_CSG_CLTI_QUEUE);
    MRS_StopTimer(MRS_STA_TIMER_CSG_CLTI_SM_QUERY);
    MRS_StopTimer(MRS_STA_TIMER_SM_PERIOD_NO_METER);

    mrsToolsFree(pstCltISmCtx->pucFrame);
    pstCltISmCtx->usFrameLength = 0;
    pstCltISmCtx->ucTry = 0;
    pstCltISmCtx->ucStatus = MRS_CSG_CLTI_STATUS_IDLE;

    mrsStaSearchStop();

    if (0 == ucMeterNum)
    {
        MRS_StartTimer(MRS_STA_TIMER_SM_PERIOD_NO_METER, MRS_CSG_CLTI_SM_INTERVAL, HI_SYS_TIMER_ONESHOT);
        return HI_ERR_SUCCESS;
    }

    pPayload++;
    for (i = 0; i < ucMeterNum; i++)
    {
        mrsCltiSearchMeterInsert(pstSmCtx, pPayload, MRS_PROTO645_VER_1997);
        pPayload += HI_METER_ADDR_LEN;
    }

    mrsStaSearchCheckFirstMeterAddr(pstSmCtx);
    mrsStaRefreshLocalMeterList(&pstStaCtx->stMeterList, &pstSmCtx->stMeterList);
    mrsStaSetMeterListToMac(&pstSmCtx->stMeterList, HI_TRUE);
    pstSmCtx->stRunning.bFirstOver = HI_TRUE;

    // 上电10分钟之内，不断的查询搜表结果
    if (HI_MDM_GetSeconds() < MRS_MIN_TO_SEC(pstSmCtx->stSearchCfg.ucQueryTh))
    {
        MRS_StartTimer(MRS_STA_TIMER_SM_PERIOD_NO_METER, MRS_CSG_CLTI_SM_INTERVAL, HI_SYS_TIMER_ONESHOT);
    }

    return HI_ERR_SUCCESS;
}


HI_U32 mrsCsgCltISearchQuery(HI_VOID)
{
    HI_U8 ucStatus = mrsCsgCltIGetStatus();

    switch (ucStatus)
    {
    case MRS_CSG_CLTI_STATUS_QRY_METER_NUM:
        mrsCsgCltIQueryMeterNum();
        break;

    case MRS_CSG_CLTI_STATUS_QRY_METER_INF:
        mrsCsgCltIQueryMeterInf();
        break;

    case MRS_CSG_CLTI_STATUS_QRY_DETECT_LIST:
        mrsCsgCltISendDetectList(HI_NULL);
        break;

    case MRS_CSG_CLTI_STATUS_QRY_DETECT_STATUS:
        mrsCsgCltIQueryDetectStatus();
        break;

    case MRS_CSG_CLTI_STATUS_FIRST_07:
        mrsCsgCltITryRead07();
        break;

    case MRS_CSG_CLTI_STATUS_FIRST_97:
        mrsCsgCltITryRead97();
        break;

    default:
        break;
    }

    return HI_ERR_SUCCESS;
}


HI_U32 mrsCsgCltIGenTransmitFrame(MRS_CSG_TRANS_XR_DL_STRU *pstXrFrame, HI_U8 **ppOutFrame, HI_U16 *pusOutLength)
{
    MRS_CSG_IND_FRAME_STRU stIndFrame;
    MRS_STA_SRV_CTX_STRU * pstStaCtx = mrsStaGetContext();
    HI_U32 ulRet = HI_ERR_SUCCESS;

    (hi_void)memset_s(&stIndFrame, sizeof(stIndFrame), 0, sizeof(stIndFrame));
    stIndFrame.ucAfn = MRS_CSG_AFN_TRANSMIT_22H;
    stIndFrame.ulDI = MRS_CSG_DI_REQ_TRANSMIT;
    stIndFrame.ucSeq = ++pstStaCtx->ucSeq;
    stIndFrame.ucPrm = PROTOCSG_CTRL_PRM_BIT_REQ;
    stIndFrame.ucAddrFlag = PROTOCSG_CTRL_ADD_BIT_Y;
    (hi_void)memcpy_s(stIndFrame.aucDstAddr, HI_METER_ADDR_LEN, pstXrFrame->aucDstAddr, HI_METER_ADDR_LEN);
    (hi_void)memcpy_s(stIndFrame.aucSrcAddr, HI_METER_ADDR_LEN, pstXrFrame->aucSrcAddr, HI_METER_ADDR_LEN);

    stIndFrame.usAppDataLen = pstXrFrame->usDataLen + 2;
    stIndFrame.pAppData = mrsToolsMalloc(stIndFrame.usAppDataLen);
    if (!stIndFrame.pAppData)
    {
        return HI_ERR_NOT_ENOUGH_MEMORY;
    }

    (hi_void)memcpy_s(stIndFrame.pAppData, stIndFrame.usAppDataLen, &pstXrFrame->usDataLen, sizeof(pstXrFrame->usDataLen));
    (hi_void)memcpy_s(stIndFrame.pAppData + sizeof(pstXrFrame->usDataLen),
					stIndFrame.usAppDataLen - sizeof(pstXrFrame->usDataLen),
					pstXrFrame->aucData, pstXrFrame->usDataLen);

    ulRet = mrsCreateCsgFrame(&stIndFrame, ppOutFrame, pusOutLength);
    mrsToolsFree(stIndFrame.pAppData);
    
    return ulRet;
}   


HI_U32 mrsCsgCltICreateDetectListFrame(HI_VOID *pParam, HI_U8 **ppFrame, HI_U16 *pusLength)
{
    MRS_STA_SRV_CTX_STRU * pstStaCtx = mrsStaGetContext();
    APP_CSG_CMD_DL_METERLIST_STRU *pstMlist = (APP_CSG_CMD_DL_METERLIST_STRU *)pParam;
    MRS_CSG_IND_FRAME_STRU stIndFrame;
    HI_U32 ulRet = HI_ERR_SUCCESS;
    HI_U8 ucOffset = 0;
    HI_U8 i = 0;

    if (!pParam)
    {
        return HI_ERR_INVALID_PARAMETER;
    }

    (hi_void)memset_s(&stIndFrame, sizeof(stIndFrame), 0, sizeof(stIndFrame));
    stIndFrame.ucAfn = MRS_CSG_AFN_MANAGE_METER_21H;
    stIndFrame.ulDI = MRS_CSG_DI_REQ_DETECT_LIST;
    stIndFrame.ucSeq = ++pstStaCtx->ucSeq;
    stIndFrame.ucPrm = PROTOCSG_CTRL_PRM_BIT_REQ;
    stIndFrame.ucAddrFlag = PROTOCSG_CTRL_ADD_BIT_N;
    stIndFrame.usAppDataLen = sizeof(pstMlist->ucMeterNum) + pstMlist->ucMeterNum * HI_METER_ADDR_LEN;
    stIndFrame.pAppData = (HI_U8 *)mrsToolsMalloc(stIndFrame.usAppDataLen);
    if (!stIndFrame.pAppData)
    {
        return HI_ERR_MALLOC_FAILUE;
    }

    (hi_void)memset_s(stIndFrame.pAppData, stIndFrame.usAppDataLen, 0, stIndFrame.usAppDataLen);
    stIndFrame.pAppData[ucOffset++] = pstMlist->ucMeterNum;

    for (i = 0; i < pstMlist->ucMeterNum; i++)
    {
        (hi_void)memcpy_s(stIndFrame.pAppData + ucOffset, stIndFrame.usAppDataLen - ucOffset,
						pstMlist->astMeterInf[i].aucMeterAddr, HI_METER_ADDR_LEN);
        ucOffset += HI_METER_ADDR_LEN;
    }
    
    ulRet = mrsCreateCsgFrame(&stIndFrame, ppFrame, pusLength);
    mrsToolsFree(stIndFrame.pAppData);

    return ulRet;
}


HI_U32 mrsCsgCltISendDetectList(APP_CSG_CMD_DL_METERLIST_STRU *pstMlist)
{
    HI_U32 ulRet = HI_ERR_SUCCESS;

    if (pstMlist && mrsCsgCltIIsDetectStatus())
    {
        return HI_ERR_BUSY;
    }

    ulRet = mrsCsgCltIQueryTx(MRS_CSG_CLTI_STATUS_QRY_DETECT_LIST, mrsCsgCltICreateDetectListFrame, (HI_VOID *)pstMlist);
    if (HI_ERR_SUCCESS == ulRet)
    {
        mrsCsgCltISetDetectStatus(HI_TRUE);
    }

    return ulRet;
}


HI_U32 mrsCsgCltICreateDetectStatusFrame(HI_VOID *pParam, HI_U8 **ppFrame, HI_U16 *pusLength)
{
    HI_UNREF_PARAM(pParam);
    return mrsCsgCltICreateQryFrameNoData(MRS_CSG_AFN_MANAGE_METER_21H, MRS_CSG_DI_REQ_DETECT_STATUS, ppFrame, pusLength);
}


HI_U32 mrsCsgCltIQueryDetectStatus(HI_VOID)
{
    return mrsCsgCltIQueryTx(MRS_CSG_CLTI_STATUS_QRY_DETECT_STATUS, mrsCsgCltICreateDetectStatusFrame, HI_NULL);
}


HI_U32 mrsCsgCltIQueryDetectStatusRx(MRS_CSG_FRAME_STRU *pstFrame)
{
    MRS_STA_SRV_CTX_STRU * pstStaCtx = mrsStaGetContext();
    MRS_STA_ITEM * pstItem = HI_NULL;
    MRS_CSG_CLTI_SM_CONTEXT *pstCltISmCtx = mrsCsgCltiCtx();

    if (MRS_CSG_CLTI_STATUS_QRY_DETECT_STATUS != mrsCsgCltIGetStatus())
    {
        return HI_ERR_FAILURE;
    }

    pstItem = mrsSrvQueueTop(&pstStaCtx->stQueModule.stMrQueue);
    if (!pstItem || pstItem->id != MRS_STA_ITEM_ID_CSG_CLTI_SEARCH_METER)
    {
        return HI_ERR_FAILURE;
    }

    mrsStaStopTtlTimer(pstItem);
    mrsStaTryDeQueue(&pstStaCtx->stQueModule, mrsStaQueueFree);

    MRS_StopTimer(MRS_STA_TIMER_CSG_CLTI_QUEUE);
    MRS_StopTimer(MRS_STA_TIMER_CSG_CLTI_SM_QUERY);
    MRS_StopTimer(MRS_STA_TIMER_CSG_QRY_DETECT_STATUS);

    mrsToolsFree(pstCltISmCtx->pucFrame);
    pstCltISmCtx->usFrameLength = 0;
    pstCltISmCtx->ucTry = 0;
    pstCltISmCtx->ucStatus = MRS_CSG_CLTI_STATUS_IDLE;
    pstCltISmCtx->bDetectStatus = HI_FALSE;

    mrsCsgCltISetDetectStatus((HI_BOOL)pstFrame->aucAppData[0]);
    if (pstFrame->aucAppData[0])
    {
        MRS_StartTimer(MRS_STA_TIMER_CSG_QRY_DETECT_STATUS, MRS_STA_TIME_CSG_QRY_DETECT_STATUS, HI_SYS_TIMER_ONESHOT);
    }
    else
    {
        return mrsCsgCltIQueryMeterInf();
    }
    
    return HI_ERR_SUCCESS;
}




#define MRS_FIRST_DETECT_RETRY_MAX    (3)
HI_U32 mrsCsgCltITryRead97(HI_VOID)
{ 
    MRS_CSG_CLTI_SM_CONTEXT *pstCltISmCtx = mrsCsgCltiCtx();

    if (pstCltISmCtx->ucTry >= pstCltISmCtx->ucMaxTry)
    { 
        pstCltISmCtx->ucTry = 0;

        mrsToolsFree(pstCltISmCtx->pucFrame);
        pstCltISmCtx->usFrameLength = 0;

        mrsCsgCltISetStatus(MRS_CSG_CLTI_STATUS_IDLE);
        mrsCsgCltIQueryMeterInf();
        return HI_ERR_SUCCESS;
    }

    return mrsCsgCltITryFirstProc(MRS_CSG_CLTI_STATUS_FIRST_97);
}


//抄读上一次入网地址 先07  后97
HI_U32 mrsCsgCltITryRead07(HI_VOID)
{
    MRS_CSG_CLTI_SM_CONTEXT *pstCltISmCtx = mrsCsgCltiCtx();
    
    if (pstCltISmCtx->ucTry >= pstCltISmCtx->ucMaxTry)
    {  
        pstCltISmCtx->ucTry = 0;  

        mrsToolsFree(pstCltISmCtx->pucFrame);
        pstCltISmCtx->usFrameLength = 0;

        mrsCsgCltISetStatus(MRS_CSG_CLTI_STATUS_FIRST_97);
        mrsCsgCltITryFirst();    
        return HI_ERR_SUCCESS;
    }

    return mrsCsgCltITryFirstProc(MRS_CSG_CLTI_STATUS_FIRST_07);
}

HI_U32 mrsCsgCltITryFirstProc(HI_U8 ucStatus)
{
    HI_U32 ulRet = HI_ERR_SUCCESS;

    MRS_STA_SRV_CTX_STRU * pstStaCtx = mrsStaGetContext();
    MRS_STA_ITEM * pstItem = HI_NULL;
    MRS_CSG_CLTI_SM_CONTEXT *pstCltISmCtx = mrsCsgCltiCtx();

    MRS_StopTimer(MRS_STA_TIMER_CSG_CLTI_QUEUE);
    MRS_StopTimer(MRS_STA_TIMER_CSG_CLTI_SM_QUERY);

    pstItem = mrsSrvQueueTop(&pstStaCtx->stQueModule.stMrQueue);
    if (pstItem && pstItem->id == MRS_STA_ITEM_ID_CSG_CLTI_SEARCH_METER)
    {
        mrsStaTryDeQueue(&pstStaCtx->stQueModule, mrsStaQueueFree);
        mrsToolsFree(pstCltISmCtx->pucFrame);
        pstCltISmCtx->usFrameLength = 0;
    }

    if (!pstCltISmCtx->pucFrame)
    {
        HI_U8 aucFrame97[] = CLTI_TRY_DI97_FRAME;
        HI_U8 aucFrame07[] = CLTI_TRY_DI07_FRAME;
        HI_U8 *pucFrame = HI_NULL;
        HI_U16 usFrameLength = 0;
        HI_U8 *pucPayload = HI_NULL;
        HI_U16 usPayloadLen = 0;

        MRS_STA_SEARCH_CTX *pstSearchCtx = mrsStaSearchCtx();
        MRS_CSG_TRANS_XR_DL_STRU * pstXrFrame = HI_NULL;

        if (MRS_CSG_CLTI_STATUS_FIRST_97 == ucStatus)
        {
            pucFrame = aucFrame97;
            usFrameLength = sizeof(aucFrame97);
            (hi_void)memcpy_s(aucFrame97 + MRS_645_FRAME_METERADD_OFFSET, usFrameLength - MRS_645_FRAME_METERADD_OFFSET,
							pstSearchCtx->stSearchCfg.aucNvMeter, HI_METER_ADDR_LEN);
            aucFrame97[usFrameLength - 2] = (HI_U8)mrsToolsCalcCheckSum(aucFrame97, usFrameLength - 2);
        }
        else
        {
            pucFrame = aucFrame07;
            usFrameLength = sizeof(aucFrame07);
            (hi_void)memcpy_s(aucFrame07 + MRS_645_FRAME_METERADD_OFFSET, usFrameLength - MRS_645_FRAME_METERADD_OFFSET,
							pstSearchCtx->stSearchCfg.aucNvMeter, HI_METER_ADDR_LEN);
            aucFrame07[usFrameLength - 2] = (HI_U8)mrsToolsCalcCheckSum(aucFrame07, usFrameLength - 2);
        }
        
        usPayloadLen = sizeof(MRS_CSG_TRANS_XR_DL_STRU) + usFrameLength;
        pucPayload = mrsToolsMalloc(usPayloadLen);
        if (!pucPayload)
        {
            return HI_ERR_NO_MORE_MEMORY;
        }
        
        (hi_void)memset_s(pucPayload, usPayloadLen, 0, usPayloadLen);
        pstXrFrame = (MRS_CSG_TRANS_XR_DL_STRU *)pucPayload;
        
        (hi_void)memcpy_s(pstXrFrame->aucDstAddr, HI_METER_ADDR_LEN, pstSearchCtx->stSearchCfg.aucNvMeter, HI_METER_ADDR_LEN);
        pstXrFrame->usDataLen = usFrameLength;
        (hi_void)memcpy_s(pstXrFrame->aucData, usFrameLength, pucFrame, usFrameLength);

        ulRet = mrsCsgCltIGenTransmitFrame(pstXrFrame, &pstCltISmCtx->pucFrame, &pstCltISmCtx->usFrameLength);
        mrsToolsFree(pucPayload);
        if (HI_ERR_SUCCESS != ulRet)
        {
            MRS_StartTimer(MRS_STA_TIMER_CSG_CLTI_SM_QUERY, 1000, HI_SYS_TIMER_ONESHOT);
            return ulRet;
        }
    }

    ulRet = mrsCsgCltIFrameEnQueue(pstCltISmCtx->pucFrame, pstCltISmCtx->usFrameLength, ucStatus);
    if (HI_ERR_SUCCESS != ulRet)
    {
        mrsStaTryDeQueue(&pstStaCtx->stQueModule, mrsStaQueueFree);
        ulRet = mrsCsgCltIFrameEnQueue(pstCltISmCtx->pucFrame, pstCltISmCtx->usFrameLength, ucStatus);
    }

    pstCltISmCtx->ucTry++;

    if (HI_ERR_SUCCESS != ulRet)
    {
        MRS_StartTimer(MRS_STA_TIMER_CSG_CLTI_SM_QUERY, 1000, HI_SYS_TIMER_ONESHOT);
    }
 
    return ulRet;
}
 

HI_U32 mrsCsgCltITryFirstRx(HI_U8 *pucSrcAddr)
{
    MRS_CSG_CLTI_SM_CONTEXT *pstCltISmCtx = mrsCsgCltiCtx();
    MRS_STA_SEARCH_CTX *pstSmCtx = mrsStaSearchCtx();

    MRS_StopTimer(MRS_STA_TIMER_CSG_CLTI_QUEUE);

    mrsCltiSearchMeterInsert(pstSmCtx, pucSrcAddr, MRS_PROTO645_VER_1997);

    mrsToolsFree(pstCltISmCtx->pucFrame);
    pstCltISmCtx->usFrameLength = 0;
    pstCltISmCtx->ucTry = 0;

    mrsCsgCltISetStatus(MRS_CSG_CLTI_STATUS_IDLE);
    mrsCsgCltIQueryMeterInf();

    return HI_ERR_SUCCESS;
}

HI_U32 mrsCsgCltIStopSearch()
{
    MRS_STA_SRV_CTX_STRU * pstStaCtx = mrsStaGetContext();
    MRS_CSG_CLTI_SM_CONTEXT *pstCltISmCtx = mrsCsgCltiCtx();
    MRS_STA_ITEM * pstItem = HI_NULL;

    MRS_StopTimer(MRS_STA_TIMER_CSG_CLTI_QUEUE);
    MRS_StopTimer(MRS_STA_TIMER_CSG_CLTI_SM_QUERY);

    pstItem = mrsSrvQueueTop(&pstStaCtx->stQueModule.stMrQueue);
    if (pstItem && (pstItem->id == MRS_STA_ITEM_ID_CSG_CLTI_SEARCH_METER))
    {
        mrsStaTryDeQueue(&pstStaCtx->stQueModule, mrsStaQueueFree);

        mrsCsgCltISetDetectStatus(HI_FALSE);
        mrsToolsFree(pstCltISmCtx->pucFrame);
        pstCltISmCtx->usFrameLength = 0;
        pstCltISmCtx->ucTry = 0;
    }

    return HI_ERR_SUCCESS;
}

#endif

HI_END_HEADER

