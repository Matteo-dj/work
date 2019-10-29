//*****************************************************************************
//
//                  版权所有 (C), 1998-2015, 华为技术有限公司
//
//*****************************************************************************
//  文 件 名   : mrs_srv_cltii_event.c
//  版 本 号   : V1.0 
//  作    者   : cuiate/00233580
//  生成日期   : 2015-07-17
//  功能描述   : II采下挂电表事件 函数及接口实现
//               
//  函数列表   : TODO: ...
//  修改历史   : 
//  1.日    期 : 2015-07-17
//    作    者 : cuiate/00233580
//    修改内容 : 创建文件 
//
//*****************************************************************************

#include "mrs_common.h"
#include "mrs_cmd_msg.h"
#include "mrs_srv_common.h"
#include "mrs_fw_tools.h"
#include "mrs_fw_log.h"
#include "mrs_srv_sta_queue.h"
#include "mrs_srv_parallel_sta.h"
#include "mrs_srv_sta_event.h"
#include "mrs_srv_sta.h"

#include "mrs_srv_cltii_event.h"
#include "mrs_srv_cltii_event_n.h"
#include "mrs_srv_sta_searchmeter.h"
#include "mrs_srv_collector.h"

#include "mrs_dfx_cltii_event.h"


HI_START_HEADER
#if defined(PRODUCT_CFG_PRODUCT_TYPE_STA)

//*****************************************************************************
//*****************************************************************************

HI_PRV MRS_CLTII_EVENT_CTX_STRU *g_pstCltiiEventCtx = HI_NULL;

HI_PRV MRS_CLTII_EVENT_CMD_OBJ g_astCltiiEventCmdTbl[] = 
{
    {MRS_CLTII_EVT_MONITOR_DI_EVT_1_V07, mrsCltiiEventFrameProcStatusWord},
    {MRS_CLTII_EVT_MONITOR_DI_EVT_2_V07, mrsCltiiEventFrameProcTimesV07},
    {MRS_CLTII_EVT_MONITOR_DI_EVT_3_V07, mrsCltiiEventFrameProcTimesV07},
    {MRS_CLTII_EVT_MONITOR_DI_EVT_4_V07, mrsCltiiEventFrameProcTimesV07},

    {MRS_CLTII_EVT_MONITOR_DI_EVT_1_V97, mrsCltiiEventFrameProcTimesV97},
    {MRS_CLTII_EVT_MONITOR_DI_EVT_2_V97, mrsCltiiEventFrameProcTimesV97},
    {MRS_CLTII_EVT_MONITOR_DI_EVT_3_V97, mrsCltiiEventFrameProcTimesV97},
    {MRS_CLTII_EVT_MONITOR_DI_EVT_4_V97, mrsCltiiEventFrameProcNetStatus},
};


MRS_CLTII_EVENT_CTX_STRU * mrsGetCltiiEventCtx(HI_VOID)
{
    return g_pstCltiiEventCtx;
}


HI_U32 mrsCltiiEventModuleInit(HI_VOID)
{
    P_MRS_STA_EVT_CTX pstStaEvtCtx = mrsStaGetEvtCtx();
    
    g_pstCltiiEventCtx = (MRS_CLTII_EVENT_CTX_STRU *)mrsToolsMalloc(sizeof(MRS_CLTII_EVENT_CTX_STRU));
    if (!g_pstCltiiEventCtx)
    {
        return HI_ERR_MALLOC_FAILUE;
    }

    (hi_void)memset_s(g_pstCltiiEventCtx, sizeof(MRS_CLTII_EVENT_CTX_STRU), 0, sizeof(MRS_CLTII_EVENT_CTX_STRU));

    if (!pstStaEvtCtx->ucCltiiEnable)
    {
        // 开关关闭时，返回成功
        return HI_ERR_SUCCESS;
    }
    
    mrsCltiiEventRegisterCmd(g_astCltiiEventCmdTbl, MRS_TOOLS_ALEN(g_astCltiiEventCmdTbl));
    mrsCltiiEventInfoLoadNv();

    mrsCltiiEventMonitorCfgInit(&(g_pstCltiiEventCtx->stCfg));

    mrsStaSearchRegisterCmdNotify(mrsCltiiSearchStartMsgProc, mrsCltiiSearchFinishMsgProc, MRS_STA_NOTIFY_ID_EVENT);

    mrsDfxCltiiEventInit();

    return HI_ERR_SUCCESS;
}


HI_U32 mrsCltiiEventModuleDeinit(HI_VOID)
{
    // TODO: 
    return HI_ERR_SUCCESS;
}


HI_U32 mrsCltiiEventStartMonitor(HI_VOID)
{
    MRS_CLTII_EVENT_CTX_STRU *pstEvtCtx = mrsGetCltiiEventCtx();

    if (!pstEvtCtx)
    {
        return HI_ERR_NOT_SUPPORT;
    }

    mrsStaEvtCtxReset();
    mrsCltiiEventMonitorInit(pstEvtCtx);
    mrsCltiiEventSetMonitorStatus(MRS_CLTII_EVENT_MONITOR_WORKING);
    mrsCltiiEventMonitorItemProc(pstEvtCtx);

    mrsDfxCltiiEventMonitorStart();

    return HI_ERR_SUCCESS;
}


HI_U32 mrsCltiiEventMonitorCfgInit(MRS_CLTII_EVENT_MON_CFG *pstCfg)
{
    pstCfg->stMonInf97.bEnableEvent[0] = HI_TRUE;
    pstCfg->stMonInf97.bEnableEvent[1] = HI_TRUE;
    pstCfg->stMonInf97.bEnableEvent[2] = HI_TRUE;
    pstCfg->stMonInf97.bEnableEvent[3] = HI_FALSE;

    pstCfg->stMonInf97.ausDiList[0] = MRS_CLTII_EVT_MONITOR_DI_EVT_1_V97;
    pstCfg->stMonInf97.ausDiList[1] = MRS_CLTII_EVT_MONITOR_DI_EVT_2_V97;
    pstCfg->stMonInf97.ausDiList[2] = MRS_CLTII_EVT_MONITOR_DI_EVT_3_V97;
    pstCfg->stMonInf97.ausDiList[3] = MRS_CLTII_EVT_MONITOR_DI_EVT_4_V97;

    pstCfg->stMonInf07.bEnableEvent[0] = HI_TRUE;
    pstCfg->stMonInf07.bEnableEvent[1] = HI_TRUE;
    pstCfg->stMonInf07.bEnableEvent[2] = HI_TRUE;
    pstCfg->stMonInf07.bEnableEvent[3] = HI_TRUE;

    pstCfg->stMonInf07.aulDiList[0] = MRS_CLTII_EVT_MONITOR_DI_EVT_1_V07;
    pstCfg->stMonInf07.aulDiList[1] = MRS_CLTII_EVT_MONITOR_DI_EVT_2_V07;
    pstCfg->stMonInf07.aulDiList[2] = MRS_CLTII_EVT_MONITOR_DI_EVT_3_V07;
    pstCfg->stMonInf07.aulDiList[3] = MRS_CLTII_EVT_MONITOR_DI_EVT_4_V07;

    return HI_ERR_SUCCESS;
}


HI_U32 mrsCltiiEventMonitorInit(MRS_CLTII_EVENT_CTX_STRU *pstEvtCtx)
{
    // 初始化运行参数
    mrsCltiiEventMonitorRunStatusInit(&(pstEvtCtx->stRun));

    // 初始化参数
    mrsCltiiEventReportParamInit(&(pstEvtCtx->stParam));
    
    // 每个表对应的事件标识
    mrsCltiiEventMonitorMeterFlagReset(&(pstEvtCtx->stMeterList));

    return HI_ERR_SUCCESS;
}


HI_U32 mrsCltiiEventMonitorFlagInit(HI_VOID)
{
    MRS_CLTII_EVENT_CTX_STRU *pstEvtCtx = mrsGetCltiiEventCtx();

    if (pstEvtCtx)
    {
        mrsCltiiEventMonitorFlagInitProc(pstEvtCtx->stNvInf.ucMonFlag, &(pstEvtCtx->stCfg));
    }

    return HI_ERR_SUCCESS;
}


HI_U32 mrsCltiiEventMonitorFlagInitProc(HI_U8 ucFlag, MRS_CLTII_EVENT_MON_CFG *pstCfg)
{
    pstCfg->stMonInf97.bEnableEvent[0] = (ucFlag & MRS_CLTII_EVT_MONITOR_DI_EVT_1_FLAG) ? HI_TRUE : HI_FALSE;
    pstCfg->stMonInf97.bEnableEvent[1] = (ucFlag & MRS_CLTII_EVT_MONITOR_DI_EVT_2_FLAG) ? HI_TRUE : HI_FALSE;
    pstCfg->stMonInf97.bEnableEvent[2] = (ucFlag & MRS_CLTII_EVT_MONITOR_DI_EVT_3_FLAG) ? HI_TRUE : HI_FALSE;
    pstCfg->stMonInf97.bEnableEvent[3] = (ucFlag & MRS_CLTII_EVT_MONITOR_DI_EVT_4_FLAG) ? HI_TRUE : HI_FALSE;

    pstCfg->stMonInf07.bEnableEvent[0] = ((ucFlag >> 4) & MRS_CLTII_EVT_MONITOR_DI_EVT_1_FLAG) ? HI_TRUE : HI_FALSE;
    pstCfg->stMonInf07.bEnableEvent[1] = ((ucFlag >> 4) & MRS_CLTII_EVT_MONITOR_DI_EVT_2_FLAG) ? HI_TRUE : HI_FALSE;
    pstCfg->stMonInf07.bEnableEvent[2] = ((ucFlag >> 4) & MRS_CLTII_EVT_MONITOR_DI_EVT_3_FLAG) ? HI_TRUE : HI_FALSE;
    pstCfg->stMonInf07.bEnableEvent[3] = ((ucFlag >> 4) & MRS_CLTII_EVT_MONITOR_DI_EVT_4_FLAG) ? HI_TRUE : HI_FALSE;

    return HI_ERR_SUCCESS;
}


HI_U32 mrsCltiiEventMonitorRunStatusInit(MRS_CLTII_EVENT_RUNNING *pstRun)
{
    pstRun->ucMeterIndex = 0;
    pstRun->ucDiIndex = 0;
    return HI_ERR_SUCCESS;
}


HI_U32 mrsCltiiEventReportParamInit(MRS_CLTII_EVENT_PARAM *pstParam)
{
    pstParam->ucCurrEvtIndex = 0;
    return HI_ERR_SUCCESS;
}


HI_U32 mrsCltiiEventMonitorMeterFlagReset(MRS_CLTII_EVENT_METERLIST *pstMeterList)
{
    (hi_void)memset_s(pstMeterList->aucEventInf, sizeof(pstMeterList->aucEventInf), 0, sizeof(pstMeterList->aucEventInf));
    pstMeterList->ucEventCount = 0;

    return HI_ERR_SUCCESS;
}


HI_U32 mrsCltiiEventLoadMeterList(MRS_CLTII_EVENT_CTX_STRU *pstEvtCtx)
{
    MRS_SEARCH_METERLIST_STRU *pstSmList = mrsStaSearchResultList();
    MRS_CLTII_EVENT_METERLIST *pstTmpEvtList = HI_NULL; 
    HI_U8 i = 0;

    // 临时列表
    pstTmpEvtList = (MRS_CLTII_EVENT_METERLIST *)mrsToolsMalloc(sizeof(MRS_CLTII_EVENT_METERLIST));
    if (!pstTmpEvtList)
    {
        return HI_ERR_MALLOC_FAILUE;
    }

    (hi_void)memset_s(pstTmpEvtList, sizeof(MRS_CLTII_EVENT_METERLIST), 0, sizeof(MRS_CLTII_EVENT_METERLIST));

    for (i = 0; i < pstSmList->ucMeterNum; i++)
    {
        MRS_CLTII_EVENT_METER_INF *pstTmpMeter = pstTmpEvtList->astMeterInf + i;
        MRS_METER_ITEM_S *pstSmMeter = pstSmList->astMeter + i;

        (hi_void)memcpy_s(pstTmpMeter->aucAddr, HI_METER_ADDR_LEN, pstSmMeter->ucMeterAddr, HI_METER_ADDR_LEN);
        pstTmpMeter->ucProtocol = pstSmMeter->ucProtocol;
        pstTmpMeter->ucOption = 0;
        (hi_void)memset_s(&(pstTmpMeter->stRecordInf), sizeof(pstTmpMeter->stRecordInf), 0xFF, sizeof(pstTmpMeter->stRecordInf));

        mrsCltiiEventSyncInfFromNv(pstEvtCtx, pstTmpMeter);
    }

    pstTmpEvtList->ucMeterTotal = pstSmList->ucMeterNum;
    mrsCltiiEventSyncMeterListInf(&(pstEvtCtx->stMeterList), pstTmpEvtList);

    (hi_void)memcpy_s(&(pstEvtCtx->stMeterList), sizeof(MRS_CLTII_EVENT_METERLIST), pstTmpEvtList, sizeof(MRS_CLTII_EVENT_METERLIST));

    mrsToolsFree(pstTmpEvtList);

    return HI_ERR_SUCCESS;
}


HI_U32 mrsCltiiEventSyncInfFromNv(MRS_CLTII_EVENT_CTX_STRU *pstEvtCtx, MRS_CLTII_EVENT_METER_INF *pMeterInf)
{
    MRS_CLTII_EVENT_NV_INF *pstNvInf = &(pstEvtCtx->stNvInf);
    HI_U8 i = 0;

    for (i = 0; i < pstNvInf->ucMeterNum; i++)
    {
        MRS_CLTII_EVENT_NV_METER_INF *pstMeter = &(pstNvInf->astMeterList[i]);

        if ((mrsToolsMemEq(pstMeter->aucMeterAddr, pMeterInf->aucAddr, HI_METER_ADDR_LEN))
            && (pstMeter->ucProtocol == pMeterInf->ucProtocol))
        {
            (hi_void)memcpy_s(pMeterInf->stRecordInf.aulRecordInf, sizeof(pMeterInf->stRecordInf.aulRecordInf),
                pstMeter->aulEvent, sizeof(pstMeter->aulEvent));
            break;
        }
    }
    
    return HI_ERR_SUCCESS;
}


HI_U32 mrsCltiiEventSyncMeterListInf(MRS_CLTII_EVENT_METERLIST *pstSrc, MRS_CLTII_EVENT_METERLIST *pstDst)
{
    HI_U8 ucIdxSrc = 0;
    HI_U8 ucIdxDst = 0;

    for (ucIdxDst = 0; ucIdxDst < pstDst->ucMeterTotal; ucIdxDst++)
    {
        MRS_CLTII_EVENT_METER_INF *pstDstMeter = pstDst->astMeterInf + ucIdxDst;

        for (ucIdxSrc = 0; ucIdxSrc < pstSrc->ucMeterTotal; ucIdxSrc++)
        {
            MRS_CLTII_EVENT_METER_INF *pstSrcMeter = pstSrc->astMeterInf + ucIdxSrc;

            if (mrsToolsMemEq(pstDstMeter->aucAddr, pstSrcMeter->aucAddr, HI_METER_ADDR_LEN)
                && (pstDstMeter->ucProtocol == pstSrcMeter->ucProtocol))
            {
                (hi_void)memcpy_s(pstDstMeter, sizeof(MRS_CLTII_EVENT_METER_INF), pstSrcMeter, sizeof(MRS_CLTII_EVENT_METER_INF));
                pstDst->aucEventInf[ucIdxDst] = pstDst->aucEventInf[ucIdxSrc];
                break;
            }
        }
    }

    return HI_ERR_SUCCESS;
}


HI_U32 mrsCltiiEventGenMonitorFrame(HI_IN MRS_PROTO645_FRAME_INF *pFrameInf, HI_OUT HI_U8 **ppFrame, HI_OUT HI_U16 *pFrameLen)
{
    return mrsGen645Frame(pFrameInf, ppFrame, pFrameLen);
}


HI_U32 mrsCltiiEventMonitorProc(MRS_CLTII_EVENT_CTX_STRU *pEvtCtx)
{
    HI_U32 ulRet = HI_ERR_SUCCESS;
    HI_U8 *pFrame = HI_NULL;
    HI_U16 usFrameLength = 0;
    MRS_CLTII_EVENT_METER_INF *pMeterInf = &(pEvtCtx->stMeterList.astMeterInf[pEvtCtx->stRun.ucMeterIndex]);
    MRS_PROTO645_FRAME_INF stFrameInf;

    (hi_void)memset_s(&stFrameInf, sizeof(stFrameInf), 0, sizeof(stFrameInf));
    (hi_void)memcpy_s(stFrameInf.aucAddr, HI_METER_ADDR_LEN, pMeterInf->aucAddr, HI_METER_ADDR_LEN);
    
    if (pMeterInf->ucProtocol == METER_PROTO_645_1997)
    {
        stFrameInf.ucCtrl = MRS_645_FRAME_CONTROL_READ_DATA97;
        stFrameInf.ucDiSize = MRS_645_FRAME_DATA_DI_SIZE_V97;
        stFrameInf.ulDi = pEvtCtx->stCfg.stMonInf97.ausDiList[pEvtCtx->stRun.ucDiIndex];
    }
    else
    {
        stFrameInf.ucCtrl = MRS_645_FRAME_CONTROL_READ_DATA;
        stFrameInf.ucDiSize = MRS_645_FRAME_DATA_DI_SIZE;
        stFrameInf.ulDi = pEvtCtx->stCfg.stMonInf07.aulDiList[pEvtCtx->stRun.ucDiIndex];
    }

    mrsDfxCltiiEventMonitorMeterInf(pMeterInf->aucAddr, pMeterInf->ucProtocol);

    ulRet = mrsCltiiEventGenMonitorFrame(&stFrameInf, &pFrame, &usFrameLength);
    if (HI_ERR_SUCCESS != ulRet)
    {
        mrsCltiiEventMonitorNextItem();
        return HI_ERR_FAILURE;
    }

    ulRet = mrsCltiiEventSendFrame(pFrame, usFrameLength, pMeterInf->ucProtocol);
    mrsToolsFree(pFrame);
    if (HI_ERR_SUCCESS != ulRet)
    {
        mrsCltiiEventMonitorNextItem();
        return HI_ERR_FAILURE;
    }

    return HI_ERR_SUCCESS;
}


HI_U32 mrsCltiiEventSendFrame(HI_U8 *pFrame, HI_U16 usFrameLength, HI_U8 ucProtocol)
{
    HI_U8 *pPayload = pFrame;
    HI_U16 usPayloadLength = usFrameLength;
    HI_U8 *pBuffer = HI_NULL;
    MRS_STA_ITEM *pstItem = HI_NULL;
    MRS_STA_SRV_CTX_STRU *pstStaCtx = mrsStaGetContext();

    pBuffer = (HI_U8 *)mrsToolsMalloc(sizeof(MRS_STA_ITEM) + usPayloadLength);
    if (!pBuffer)
    {
        return HI_ERR_MALLOC_FAILUE;
    }

    (hi_void)memset_s(pBuffer, sizeof(MRS_STA_ITEM) + usPayloadLength, 0, sizeof(MRS_STA_ITEM) + usPayloadLength);

    pstItem = (MRS_STA_ITEM *)pBuffer;
    pstItem->id = MRS_STA_ITEM_ID_CLTII_EVENT;
    pstItem->buf = pBuffer + sizeof(MRS_STA_ITEM);
    pstItem->len = usPayloadLength;
    pstItem->from = MRS_STA_QUEUE_FROM_STA;
    pstItem->time_out = (HI_U8)MRS_MS_TO_100MS(mrsGet645FrameHeadTimeout(MRS_STA_RM_CFG_LOCAL));
    pstItem->bAutoStrategy = HI_TRUE;
    pstItem->ucFrameTimeout = (HI_U8)MRS_MS_TO_100MS(mrsGet645FrameInterval(MRS_STA_RM_CFG_LOCAL));
    pstItem->ucProtocol = ucProtocol;
    (hi_void)memcpy_s(pstItem->buf, usPayloadLength, pPayload, usPayloadLength);

    if (!mrsStaTryEnQueue(&pstStaCtx->stQueModule, pstItem))
    {
        mrsToolsFree(pstItem);
        return HI_ERR_FAILURE;
    }

    mrsStaNotifyQueue();

    return HI_ERR_SUCCESS;
}


HI_U32 mrsCltiiEventFrameRx(HI_U8 *pFrame, HI_U16 usFrameLength)
{
    HI_U32 ulRet = HI_ERR_SUCCESS;
    HI_U16 usPos = 0;
    HI_U16 usLength = 0;

    if (MRS_CLTII_EVENT_MONITOR_IDLE == mrsCltiiEventGetMonitorStatus())
    {
        return HI_ERR_SUCCESS;
    }

    do
    {
        HI_U8 ucCtrl = 0;
        HI_U8 ucTxFn = MRS_645_FRAME_CONTROL_READ_DATA;
        HI_U8 ucDataLength = 0;
        HI_U32 ulRxDi = 0;
        HI_U32 ulTxDi = 0;
        HI_U8 ucDiSize = MRS_645_FRAME_DATA_DI_SIZE;
        MRS_CLTII_EVENT_CMD_FUNC pFunc = HI_NULL;
        MRS_CLTII_EVENT_CTX_STRU *pstEvtCtx = mrsGetCltiiEventCtx();
        MRS_CLTII_EVENT_METER_INF *pstMeterInf = HI_NULL;

        // 电表索引越界: 进入处理下一个流程
        if (pstEvtCtx->stRun.ucMeterIndex >= pstEvtCtx->stMeterList.ucMeterTotal)
        {
            break;
        }

        // 数据项索引越界: 进入处理下一个流程
        if (pstEvtCtx->stRun.ucDiIndex >= MRS_CLTII_EVENT_ITEM_MAX)
        {
            break;
        }

        // 未找到合法645帧: 报文丢弃，等待超时处理
        ulRet = mrsFind645Frame(pFrame, (HI_S16)usFrameLength, &usPos, &usLength);
        if (HI_ERR_SUCCESS != ulRet)
        {
            ulRet = HI_ERR_BAD_DATA;
            break;
        }

        pstMeterInf = &(pstEvtCtx->stMeterList.astMeterInf[pstEvtCtx->stRun.ucMeterIndex]);

        // 电表地址不匹配: 报文丢弃，等待超时处理
        if (!mrsMeterAddrMatch(pFrame + usPos + MRS_645_FRAME_METERADD_OFFSET, pstMeterInf->aucAddr))
        {
            ulRet = HI_ERR_BAD_DATA;
            break;
        }

        // 功能码错误: 报文丢弃，等待超时处理
        ucCtrl = pFrame[usPos + MRS_645_FRAME_CTRL_OFFSET];
        if (METER_PROTO_645_1997 == pstMeterInf->ucProtocol)
        {
            ucTxFn = MRS_645_FRAME_CONTROL_READ_DATA97;
            ucDiSize = MRS_645_FRAME_DATA_DI_SIZE_V97;
            ulTxDi = pstEvtCtx->stCfg.stMonInf97.ausDiList[pstEvtCtx->stRun.ucDiIndex];
        }
        else
        {
            ulTxDi = pstEvtCtx->stCfg.stMonInf07.aulDiList[pstEvtCtx->stRun.ucDiIndex];
        }

        if (ucTxFn != (ucCtrl & MRS_645_FRAME_CONTROL_MASK))
        {
            ulRet = HI_ERR_BAD_DATA;
            break;
        }

        // 否认帧: 处理一下个数据项/电表
        if (ucCtrl & MRS_645_FRAME_CONTROL_DENY_MASK)
        {
            break;
        }

        // 数据域长度太短: 报文丢弃，等待超时处理
        ucDataLength = pFrame[usPos + MRS_645_FRAME_LENGTH_OFFSET];
        if (ucDataLength < ucDiSize)
        {
            ulRet = HI_ERR_BAD_DATA;
            break;
        }

        mrs645DataDecode(pFrame + usPos + MRS_645_FRAME_DATA_OFFSET, ucDataLength);
        (hi_void)memcpy_s(&ulRxDi, sizeof(ulRxDi), pFrame + usPos + MRS_645_FRAME_DATA_OFFSET, ucDiSize);

        // 数据项不一致: 报文丢弃，等待超时处理
        if (ulRxDi != ulTxDi)
        {
            ulRet = HI_ERR_BAD_DATA;
            break;
        }

        // 未找到处理函数，数据DI错误: 报文丢弃，等待超时处理
        pFunc = mrsCltiiEventFindCmdFunc(ulRxDi);
        if (!pFunc)
        {
            ulRet = HI_ERR_BAD_DATA;
            break;
        }

        // 处理函数处理时出错: 报文丢弃，等待超时处理
        ulRet = pFunc(pFrame + usPos, usLength);
    } while (0);

    if (HI_ERR_SUCCESS == ulRet)
    {
        mrsCltiiEventMonitorNextItem();
    }

    return ulRet;
}


HI_U32 mrsCltiiEventFrameTimeout()
{
    return mrsCltiiEventMonitorSendMessage(MRS_CLTII_EVENT_MSG_NEXT_ITEM);
}


HI_U32 mrsCltiiEventMonitorSendMessage(HI_U32 ulMsgId)
{
    MRS_CLTII_EVENT_CTX_STRU *pstEvtCtx = mrsGetCltiiEventCtx();
    HI_U32 ulRet = HI_ERR_SUCCESS;

    if (pstEvtCtx && (MRS_CLTII_EVENT_MONITOR_WORKING == pstEvtCtx->stParam.eMonStatus))
    {
        HI_SYS_QUEUE_MSG_S stMsg;
        (hi_void)memset_s(&stMsg, sizeof(stMsg), 0, sizeof(stMsg));
        stMsg.ulMsgId = EN_MRS_FW_MSG_CLTII_EVENT_MON;
        stMsg.ulParam[0] = ulMsgId;
        ulRet = mrsSendMessage2Queue(&stMsg);
    }
        
    return ulRet;
}


HI_U32 mrsCltiiEventMonitorNextItem(HI_VOID)
{
    MRS_StartTimer(MRS_STA_TIMER_CLTII_LOCAL_RM_INTERVAL, MRS_CLTII_LOCAL_RM_INTERVAL_TIME, HI_SYS_TIMER_ONESHOT);
    return HI_ERR_SUCCESS;
}


HI_U32 mrsCltiiEventMonitorNextItemProc(HI_VOID)
{
    MRS_CLTII_EVENT_CTX_STRU *pstEvtCtx = mrsGetCltiiEventCtx();

    pstEvtCtx->stRun.ucDiIndex++;
    return mrsCltiiEventMonitorItemProc(pstEvtCtx);
}


HI_U32 mrsCltiiEventMonitorItemProc(MRS_CLTII_EVENT_CTX_STRU *pstEvtCtx)
{
    HI_U8 ucMeterIndex = pstEvtCtx->stRun.ucMeterIndex;
    HI_U8 ucMeterTotal = pstEvtCtx->stMeterList.ucMeterTotal;
    HI_U8 ucDiIndex = pstEvtCtx->stRun.ucDiIndex;
    HI_U8 ucItemMax = MRS_CLTII_EVENT_ITEM_MAX;

    while (ucMeterIndex < ucMeterTotal)
    {
        MRS_CLTII_EVENT_METER_INF *pMeterInf = &(pstEvtCtx->stMeterList.astMeterInf[ucMeterIndex]);
        HI_BOOL *pEnable = HI_NULL;

        ucItemMax = MRS_CLTII_EVENT_ITEM_MAX;
        if (METER_PROTO_645_1997 == pMeterInf->ucProtocol)
        {
            pEnable = pstEvtCtx->stCfg.stMonInf97.bEnableEvent;
        }
        else
        {
            pEnable = pstEvtCtx->stCfg.stMonInf07.bEnableEvent;

            if (pstEvtCtx->stMeterList.astMeterInf[ucMeterIndex].ucOption & MRS_CLTII_EVENT_STATUS_REPORT_SUPPORT)
            {
                ucItemMax = 1;
            }

            if (pEnable[0] && pstEvtCtx->stMeterList.astMeterInf[ucMeterIndex].ucOption & MRS_CLTII_EVENT_STATUS_REPORT_FLAG)
            {
                pstEvtCtx->stMeterList.aucEventInf[ucMeterIndex] |= MRS_CLTII_EVT_MONITOR_EVT_1_FLAG;

                ucMeterIndex++;
                ucDiIndex = 0;
                continue;
            }
        }

        while (ucDiIndex < ucItemMax)
        {
            if (pEnable[ucDiIndex])
            {
                pstEvtCtx->stRun.ucMeterIndex = ucMeterIndex;
                pstEvtCtx->stRun.ucDiIndex = ucDiIndex;

                return mrsCltiiEventMonitorProc(pstEvtCtx);
            }

            ucDiIndex++;
        }

        ucMeterIndex++;
        ucDiIndex = 0;
    }

    return mrsCltiiEventMonitorFinish();
}


HI_U32 mrsCltiiEventMonitorFinish(HI_VOID)
{
    return mrsCltiiEventMonitorSendMessage(MRS_CLTII_EVENT_MSG_FINISH);
}


HI_U32 mrsCltiiEventMonitorFinishProc(HI_VOID)
{
    MRS_CLTII_EVENT_CTX_STRU *pstEvtCtx = mrsGetCltiiEventCtx();

    // 设置监控状态
    mrsCltiiEventSetMonitorStatus(MRS_CLTII_EVENT_MONITOR_IDLE);
    mrsDfxCltiiEventMonitorEnd();

    mrsCltiiEventUpdateResult(&(pstEvtCtx->stMeterList));
    mrsCltiiEventMonInfoSaveNv();

    if (mrsCltiiEventNeedReport())
    {
        mrsCltiiEventReport();
    }

    MRS_StartTimer(MRS_STA_TIMER_CLTII_EVENT, pstEvtCtx->stNvInf.usMonInterval * 1000, HI_SYS_TIMER_ONESHOT);

    return HI_ERR_SUCCESS;
}


HI_U32 mrsCltiiEventUpdateResult(MRS_CLTII_EVENT_METERLIST *pstMeterList)
{
    MRS_CLTII_EVENT_CTX_STRU *pstEvtCtx = mrsGetCltiiEventCtx();
    HI_U8 i = 0;
    HI_U8 ucEventNum = 0;

    for (i = 0; i < pstMeterList->ucMeterTotal; i++)
    {
        MRS_CLTII_EVENT_METER_INF *pMeterInf = pstMeterList->astMeterInf + i;
        MRS_CLTII_EVENT_NV_METER_INF *pstNvInf = HI_NULL;

        pstNvInf = mrsCltiiEventFindNvMeterInf(&(pstEvtCtx->stNvInf), pMeterInf);
        if (pstNvInf)
        {
            HI_U8 j = 0;
            HI_U8 ucOffset = 0;

            if (METER_PROTO_645_2007 == pMeterInf->ucProtocol)
            {
                if (pMeterInf->ucOption & MRS_CLTII_EVENT_STATUS_REPORT_SUPPORT)
                {
                    pstMeterList->aucEventInf[i] = (pMeterInf->ucOption & MRS_CLTII_EVENT_STATUS_REPORT_FLAG) ? MRS_CLTII_EVT_MONITOR_EVT_1_FLAG : 0;
                    continue;
                }

                ucOffset = 1;
            }

            for (j = 0; j < 3; j++)
            {
                if ((pstNvInf->aulEvent[j] != MRS_CLTII_EVENT_INVALID_TIMES)
                    && (pstNvInf->aulEvent[j] != pMeterInf->stRecordInf.aulRecordInf[j]))
                {
                    pstMeterList->aucEventInf[i] |= (HI_U8)(1 << (j+ucOffset));
                }
            }
        }
    }
        
    for (i = 0; i < pstMeterList->ucMeterTotal; i++)
    {
        if (pstMeterList->aucEventInf[i])
        {
            ucEventNum++;
        }
    }

    pstMeterList->ucEventCount = ucEventNum;
    mrsDfxCltiiEventLastCount(ucEventNum);

    return HI_ERR_SUCCESS;
}


HI_BOOL mrsCltiiEventNeedReport(HI_VOID)
{
    return (mrsGetCltiiEventCtx()->stMeterList.ucEventCount > 0);
}


HI_U32 mrsCltiiEventMsgProc(HI_SYS_QUEUE_MSG_S *pstMsg)
{
    MRS_CLTII_EVENT_CTX_STRU * pstEvtCtx = mrsGetCltiiEventCtx();

    if (!pstEvtCtx || !mrsToolsIsIICollector())
    {
        return HI_ERR_NOT_SUPPORT;
    }

    switch (pstMsg->ulParam[0])
    {
        case MRS_CLTII_EVENT_MSG_NEXT_ITEM:
            return mrsCltiiEventMonitorNextItemProc();

        case MRS_CLTII_EVENT_MSG_FINISH:
            return mrsCltiiEventMonitorFinishProc();

        default:
            break;
    }
    return HI_ERR_SUCCESS;
}


HI_U32 mrsCltiiEventRegisterCmd(MRS_CLTII_EVENT_CMD_OBJ *pCmdList, HI_U16 usCmdNum)
{
    MRS_CLTII_EVENT_CTX_STRU * pEvtCtx = mrsGetCltiiEventCtx();
    MRS_CLTII_EVENT_CMD_HEAD *pCmdHdr = &(pEvtCtx->stRun.stCmdHdr);

    pCmdHdr->usCmdNum = usCmdNum;
    pCmdHdr->pCmdList = pCmdList;

    return HI_ERR_SUCCESS;
}


MRS_CLTII_EVENT_CMD_FUNC mrsCltiiEventFindCmdFunc(HI_IN HI_U32 ulDi)
{
    MRS_CLTII_EVENT_CTX_STRU *pEvtCtx = mrsGetCltiiEventCtx();
    MRS_CLTII_EVENT_CMD_HEAD *pCmdHdr = &(pEvtCtx->stRun.stCmdHdr);
    HI_U16 i = 0;

    for (i = 0; i < pCmdHdr->usCmdNum; i++)
    {
        if (ulDi == pCmdHdr->pCmdList[i].ulDi)
        {
            return pCmdHdr->pCmdList[i].pFunc;
        }
    }

    return HI_NULL;
}


HI_U32 mrsCltiiSearchStartMsgProc(HI_VOID)
{
    MRS_StopTimer(MRS_STA_TIMER_CLTII_EVENT);
    MRS_StopTimer(MRS_STA_TIMER_CLTII_LOCAL_RM_INTERVAL);

    if (MRS_CLTII_EVENT_MONITOR_IDLE != mrsCltiiEventGetMonitorStatus())
    {
        mrsCltiiEventSetMonitorStatus(MRS_CLTII_EVENT_MONITOR_IDLE);
        mrsDfxCltiiEventMonitorEnd();
    }

    return HI_ERR_SUCCESS;
}


HI_U32 mrsCltiiSearchFinishMsgProc(HI_U32 ulOption)
{
    MRS_CLTII_EVENT_CTX_STRU *pstEvtCtx = mrsGetCltiiEventCtx();

    if (MRS_SEARCH_FINISH_NORMAL == ulOption)
    {
        mrsCltiiEventNvDelSMNonExistInfo();

        // 同步搜表结果
        mrsCltiiEventLoadMeterList(pstEvtCtx);
    }

    if (!mrsCltiiSearchIsMeterListEmpty())
    {
        mrsCltiiEventStartMonitor();
    }

    return HI_ERR_SUCCESS;
}


HI_BOOL mrsCltiiSearchIsMeterListEmpty(HI_VOID)
{
    return (0 == mrsGetCltiiEventCtx()->stMeterList.ucMeterTotal);
}


HI_U32 mrsCltiiEventFrameProcStatusWord(HI_IN HI_U8 *pFrame, HI_IN HI_U16 usFrameLength)
{
    MRS_CLTII_EVENT_CTX_STRU *pstEvtCtx = mrsGetCltiiEventCtx();
    MRS_CLTII_EVENT_METERLIST *pstMeterList = &(pstEvtCtx->stMeterList);
    HI_U8 ucDataLenMin = MRS_645_FRAME_DATA_DI_SIZE + 14;   // 14 = 主动上报状态字长度(12B) + 起始分隔符AAH(1B) + 结束分隔符AAH(1B)
    HI_U8 ucDataLenMax = ucDataLenMin + 96;
    HI_U8 ucDataLength = pFrame[MRS_645_FRAME_LENGTH_OFFSET];
    
    HI_UNREF_PARAM(usFrameLength);

    if ((ucDataLength < ucDataLenMin) || (ucDataLength > ucDataLenMax))
    {
        return HI_ERR_BAD_DATA;
    }

    pstMeterList->astMeterInf[pstEvtCtx->stRun.ucMeterIndex].ucOption |= MRS_CLTII_EVENT_STATUS_REPORT_SUPPORT;

    // 12: 主动上报状态字长度
    if (mrsToolsMemIsZero(pFrame + MRS_645_FRAME_DATA_OFFSET + MRS_645_FRAME_DATA_DI_SIZE, 12))
    {
        pstMeterList->aucEventInf[pstEvtCtx->stRun.ucMeterIndex] = 0x00;
    }
    else
    {
        pstMeterList->aucEventInf[pstEvtCtx->stRun.ucMeterIndex] = 0x01;
        pstMeterList->astMeterInf[pstEvtCtx->stRun.ucMeterIndex].ucOption |= MRS_CLTII_EVENT_STATUS_REPORT_FLAG;
    }
    
    return HI_ERR_SUCCESS;
}


HI_U32 mrsCltiiEventFrameProcNetStatus(HI_IN HI_U8 *pFrame, HI_IN HI_U16 usFrameLength)
{
    MRS_CLTII_EVENT_CTX_STRU *pstEvtCtx = mrsGetCltiiEventCtx();
    MRS_CLTII_EVENT_METERLIST *pstMeterList = &(pstEvtCtx->stMeterList);
    HI_U8 ucDataLength = pFrame[MRS_645_FRAME_LENGTH_OFFSET];
    HI_U8 ucExpectedLength = MRS_645_FRAME_DATA_DI_SIZE_V97 + 1;
    
    HI_UNREF_PARAM(usFrameLength);

    if (ucDataLength != ucExpectedLength)
    {
        return HI_ERR_BAD_DATA;
    }

    if (pstMeterList->astMeterInf[pstEvtCtx->stRun.ucMeterIndex].ucOption != pFrame[MRS_645_FRAME_DATA_OFFSET + MRS_645_FRAME_DATA_DI_SIZE_V97])
    {
        pstMeterList->astMeterInf[pstEvtCtx->stRun.ucMeterIndex].ucOption = pFrame[MRS_645_FRAME_DATA_OFFSET + MRS_645_FRAME_DATA_DI_SIZE_V97];

        if (pstMeterList->astMeterInf[pstEvtCtx->stRun.ucMeterIndex].ucOption)
        {
            pstMeterList->aucEventInf[pstEvtCtx->stRun.ucMeterIndex] |= (HI_U8)(1 << pstEvtCtx->stRun.ucDiIndex);
        }
    }

    return HI_ERR_SUCCESS;
}


HI_U32 mrsCltiiEventFrameProcTimesV07(HI_IN HI_U8 *pFrame, HI_IN HI_U16 usFrameLength)
{
    MRS_CLTII_EVENT_CTX_STRU *pstEvtCtx = mrsGetCltiiEventCtx();
    MRS_CLTII_EVENT_METERLIST *pstMeterList = &(pstEvtCtx->stMeterList);
    HI_U8 ucDataLength = pFrame[MRS_645_FRAME_LENGTH_OFFSET];
    HI_U8 ucExpectedLength = MRS_645_FRAME_DATA_DI_SIZE + MRS_CLTII_EVENT_TIMES_WIDTH;
    HI_U8 *pData = pFrame + MRS_645_FRAME_DATA_OFFSET + MRS_645_FRAME_DATA_DI_SIZE;
    MRS_CLTII_EVENT_NV_METER_INF *pstNvInf = HI_NULL;
    MRS_CLTII_EVENT_METER_INF *pMeterInf = HI_NULL;
    MRS_CLTII_EVENT_RECORD_INF *pRecInf = HI_NULL;
    HI_U32 ulOldTimes = 0;
    HI_U32 ulNewTimes = 0;
    
    HI_UNREF_PARAM(usFrameLength);

    if (ucDataLength != ucExpectedLength)
    {
        return HI_ERR_BAD_DATA;
    }

    if ((pstEvtCtx->stRun.ucDiIndex == 0) || (pstEvtCtx->stRun.ucDiIndex >= MRS_CLTII_EVENT_ITEM_MAX))
    {
        return HI_ERR_INVALID_PARAMETER;
    }

    if (!mrsToolsCheckBCD(pData, 3))
    {
        return HI_ERR_BAD_DATA;
    }

    pMeterInf = &(pstMeterList->astMeterInf[pstEvtCtx->stRun.ucMeterIndex]);
    pRecInf = &(pMeterInf->stRecordInf);

    ulNewTimes = BCD2INT(pData[0]);
    ulNewTimes += BCD2INT(pData[1]) * 100;
    ulNewTimes += BCD2INT(pData[2]) * 10000;
    ulOldTimes = pRecInf->aulRecordInf[pstEvtCtx->stRun.ucDiIndex - 1];
    pRecInf->aulRecordInf[pstEvtCtx->stRun.ucDiIndex - 1] = ulNewTimes;

    if ((MRS_CLTII_EVENT_INVALID_TIMES != ulOldTimes) && (ulNewTimes != ulOldTimes))
    {
        HI_DIAG_LOG_MSG_E3(MRS_FILE_LOG_FLAG_011, HI_DIAG_MT("[0] (07) ulOldTimes-[%d], ulNewTimes-[%d] EVENT-[%d]"), ulOldTimes, ulNewTimes, pstEvtCtx->stRun.ucDiIndex);
        pstMeterList->aucEventInf[pstEvtCtx->stRun.ucMeterIndex] |= (HI_U8)(1 << pstEvtCtx->stRun.ucDiIndex);
    }

    pstNvInf = mrsCltiiEventFindNvMeterInf(&(pstEvtCtx->stNvInf), pMeterInf);
    if (pstNvInf)
    {
        ulOldTimes = pstNvInf->aulEvent[pstEvtCtx->stRun.ucDiIndex - 1];
        if ((MRS_CLTII_EVENT_INVALID_TIMES != ulOldTimes) && (ulNewTimes != ulOldTimes))
        {
            HI_DIAG_LOG_MSG_E3(MRS_FILE_LOG_FLAG_012, HI_DIAG_MT("[1] (07) ulOldTimes-[%d], ulNewTimes-[%d] EVENT-[%d]"), ulOldTimes, ulNewTimes, pstEvtCtx->stRun.ucDiIndex);
            pstMeterList->aucEventInf[pstEvtCtx->stRun.ucMeterIndex] |= (HI_U8)(1 << pstEvtCtx->stRun.ucDiIndex);
        }
    }

    return HI_ERR_SUCCESS;
}


HI_U32 mrsCltiiEventFrameProcTimesV97(HI_IN HI_U8 *pFrame, HI_IN HI_U16 usFrameLength)
{
    MRS_CLTII_EVENT_CTX_STRU *pstEvtCtx = mrsGetCltiiEventCtx();
    MRS_CLTII_EVENT_METERLIST *pstMeterList = &(pstEvtCtx->stMeterList);
    HI_U8 ucDataLength = pFrame[MRS_645_FRAME_LENGTH_OFFSET];
    HI_U8 ucExpectedLength = MRS_645_FRAME_DATA_DI_SIZE_V97 + MRS_CLTII_EVENT_TIMES_WIDTH97;
    HI_U8 *pData = pFrame + MRS_645_FRAME_DATA_OFFSET + MRS_645_FRAME_DATA_DI_SIZE_V97;
    MRS_CLTII_EVENT_NV_METER_INF *pstNvInf = HI_NULL;
    MRS_CLTII_EVENT_METER_INF *pMeterInf = HI_NULL;
    MRS_CLTII_EVENT_RECORD_INF *pRecInf = HI_NULL;
    HI_U32 ulOldTimes = 0;
    HI_U32 ulNewTimes = 0;
    
    HI_UNREF_PARAM(usFrameLength);

    if (ucDataLength != ucExpectedLength)
    {
        return HI_ERR_BAD_DATA;
    }

    if (pstEvtCtx->stRun.ucDiIndex >= MRS_CLTII_EVENT_ITEM_MAX - 1)
    {
        return HI_ERR_INVALID_PARAMETER;
    }

    if (!mrsToolsCheckBCD(pData, 2))
    {
        return HI_ERR_BAD_DATA;
    }

    pMeterInf = &(pstMeterList->astMeterInf[pstEvtCtx->stRun.ucMeterIndex]);
    pRecInf = &(pMeterInf->stRecordInf);

    ulNewTimes = BCD2INT(pData[0]);
    ulNewTimes += BCD2INT(pData[1]) * 100;
    ulOldTimes = pRecInf->aulRecordInf[pstEvtCtx->stRun.ucDiIndex];
    pRecInf->aulRecordInf[pstEvtCtx->stRun.ucDiIndex] = ulNewTimes;

    if ((MRS_CLTII_EVENT_INVALID_TIMES != ulOldTimes) && (ulNewTimes != ulOldTimes))
    {
        HI_DIAG_LOG_MSG_E3(MRS_FILE_LOG_FLAG_013, HI_DIAG_MT("[0] (07) ulOldTimes-[%d], ulNewTimes-[%d] EVENT-[%d]"), ulOldTimes, ulNewTimes, pstEvtCtx->stRun.ucDiIndex);
        pstMeterList->aucEventInf[pstEvtCtx->stRun.ucMeterIndex] |= (HI_U8)(1 << pstEvtCtx->stRun.ucDiIndex);
    }

    pstNvInf = mrsCltiiEventFindNvMeterInf(&(pstEvtCtx->stNvInf), pMeterInf);
    if (pstNvInf)
    {
        ulOldTimes = pstNvInf->aulEvent[pstEvtCtx->stRun.ucDiIndex];
        if ((MRS_CLTII_EVENT_INVALID_TIMES != ulOldTimes) && (ulNewTimes != ulOldTimes))
        {
            HI_DIAG_LOG_MSG_E3(MRS_FILE_LOG_FLAG_014, HI_DIAG_MT("[0] (07) ulOldTimes-[%d], ulNewTimes-[%d] EVENT-[%d]"), ulOldTimes, ulNewTimes, pstEvtCtx->stRun.ucDiIndex);
            pstMeterList->aucEventInf[pstEvtCtx->stRun.ucMeterIndex] |= (HI_U8)(1 << pstEvtCtx->stRun.ucDiIndex);
        }
    }

    return HI_ERR_SUCCESS;
}


MRS_CLTII_EVENT_NV_METER_INF *mrsCltiiEventFindNvMeterInf(MRS_CLTII_EVENT_NV_INF *pNvInf, MRS_CLTII_EVENT_METER_INF *pMeterInf)
{
    MRS_CLTII_EVENT_CTX_STRU *pstEvtCtx = mrsGetCltiiEventCtx();
    MRS_CLTII_EVENT_NV_INF *pstNvInf = &(pstEvtCtx->stNvInf);
    HI_U8 i = 0;

    /* TODO: 直接使用pNvInf, 不使用pstNvInf */    
    HI_UNREF_PARAM(pNvInf);

    for (i = 0; i < pstNvInf->ucMeterNum; i++)
    {
        MRS_CLTII_EVENT_NV_METER_INF *pstMeter = &(pstNvInf->astMeterList[i]);

        if (mrsToolsMemEq(pstMeter->aucMeterAddr, pMeterInf->aucAddr, HI_METER_ADDR_LEN)
            && (pstMeter->ucProtocol == pMeterInf->ucProtocol))
        {
            return pstMeter;
        }
    }
    
    return HI_NULL;
}



//*****************************************************************************
// 函数名称: mrsCltiiEventGetMonitorStatus
// 功能描述: 获取II采本地监控状态
//           
// 参数说明:
//   无
//
// 返 回 值: 
//    II采本地监控状态
//
// 调用要求: TODO: ...
// 调用举例: TODO: ...
// 作    者: fengxiaomin/00209182 [2015-07-18]
//*****************************************************************************
MRS_CLTII_EVENT_MONITOR_STATUS_E mrsCltiiEventGetMonitorStatus(HI_VOID)
{
    MRS_CLTII_EVENT_CTX_STRU * pstEvtCtx = mrsGetCltiiEventCtx();
    if (pstEvtCtx)
    {
        return pstEvtCtx->stParam.eMonStatus;
    }

    return MRS_CLTII_EVENT_MONITOR_IDLE;
}

//*****************************************************************************
// 函数名称: mrsCltiiEventGetMonitorStatus
// 功能描述: 设置II采本地监控状态
//           
// 参数说明:
//   无
//
// 返 回 值: 
//    II采本地监控状态
//
// 调用要求: TODO: ...
// 调用举例: TODO: ...
// 作    者: fengxiaomin/00209182 [2015-07-18]
//*****************************************************************************
HI_VOID mrsCltiiEventSetMonitorStatus(MRS_CLTII_EVENT_MONITOR_STATUS_E eStatus)
{
    MRS_CLTII_EVENT_CTX_STRU * pstEvtCtx = mrsGetCltiiEventCtx();
    if (pstEvtCtx)
    {
        pstEvtCtx->stParam.eMonStatus = eStatus;
    }
}

//*****************************************************************************
// 函数名称: mrsCltiiEventInfoLoadNv
// 功能描述: 将NV中的采集器下接表的事件信息加载到上下文中
//           
// 参数说明:
//   无
//
// 返 回 值: 
//    无
//
// 调用要求: TODO: ...
// 调用举例: TODO: ...
// 作    者: fengxiaomin/00209182 [2015-07-17]
//*****************************************************************************
HI_VOID mrsCltiiEventInfoLoadNv(HI_VOID)
{
    HI_U32 ulRet = HI_ERR_SUCCESS;
    HI_U8 ucIndex = 0;
    HI_U16 ausNvId[] = {ID_NV_APP_CLT_EVT_INFO1, ID_NV_APP_CLT_EVT_INFO2, ID_NV_APP_CLT_EVT_INFO3, ID_NV_APP_CLT_EVT_INFO4};
    NV_APP_CLTII_EVENT_INF_STRU *pstNvItem = HI_NULL;
    HI_U8 ucItemMax = sizeof(NV_APP_CLTII_EVENT_INF_STRU) / sizeof(NV_APP_CLTII_EVENT_ITEM);
    MRS_CLTII_EVENT_CTX_STRU *pstEvtCtx = mrsGetCltiiEventCtx();
    MRS_CLTII_EVENT_NV_INF *pstNvInf = &(pstEvtCtx->stNvInf);

    pstNvItem = (NV_APP_CLTII_EVENT_INF_STRU *)mrsToolsMalloc(sizeof(NV_APP_CLTII_EVENT_INF_STRU));
    if (!pstNvItem)
    {
        return;
    }

    pstNvInf->ucMeterNum = 0;
    (hi_void)memset_s(pstNvInf->astMeterList, sizeof(pstNvInf->astMeterList), 0, sizeof(pstNvInf->astMeterList));

    for (ucIndex = 0; ucIndex < MRS_TOOLS_ALEN(ausNvId); ucIndex++)
    {
        HI_U8 i = 0;

        (hi_void)memset_s(pstNvItem, sizeof(NV_APP_CLTII_EVENT_INF_STRU), 0, sizeof(NV_APP_CLTII_EVENT_INF_STRU));
        ulRet = HI_MDM_NV_Read(ausNvId[ucIndex], pstNvItem, sizeof(NV_APP_CLTII_EVENT_INF_STRU));
        if (HI_ERR_SUCCESS != ulRet)
        {
            continue;
        }

        for (i = 0; i < ucItemMax; i++)
        {
            mrsCltiiEventNvMeterInsert(&(pstNvItem->astEventItem[i]));
        }
    }

    mrsToolsFree(pstNvItem);
}


HI_U32 mrsCltiiEventNvMeterInsert(NV_APP_CLTII_EVENT_ITEM *pstItem)
{
    MRS_CLTII_EVENT_CTX_STRU *pstEvtCtx = mrsGetCltiiEventCtx();
    MRS_CLTII_EVENT_NV_INF *pstNvInf = &(pstEvtCtx->stNvInf);
    MRS_CLTII_EVENT_NV_METER_INF *pstMeter = HI_NULL;
    HI_U8 ucWidth = MRS_CLTII_EVENT_TIMES_WIDTH;
    HI_U8 i = 0;
    HI_U32 ulInvalid = MRS_CLTII_EVENT_INVALID_NV_TIMES;

    if (pstNvInf->ucMeterNum >= MRS_TOOLS_ALEN(pstNvInf->astMeterList))
    {
        return HI_ERR_FULL;
    }

    if (!(pstItem->ucOption & MRS_CLTII_EVENT_NV_ITEM_VALID))
    {
        return HI_ERR_FAILURE;
    }

    pstMeter = &(pstNvInf->astMeterList[pstNvInf->ucMeterNum]);
    (hi_void)memcpy_s(pstMeter->aucMeterAddr, HI_METER_ADDR_LEN, pstItem->aucMeterAddr, HI_METER_ADDR_LEN);
    pstMeter->ucProtocol = (pstItem->ucOption & MRS_CLTII_EVENT_NV_PROTO_MASK);

    if (METER_PROTO_645_2007 == pstMeter->ucProtocol)
    {
        ucWidth = MRS_CLTII_EVENT_TIMES_WIDTH;
        ulInvalid = MRS_CLTII_EVENT_INVALID_NV_TIMES;
    }
    else if (METER_PROTO_645_1997 == pstMeter->ucProtocol)
    {
        ucWidth = MRS_CLTII_EVENT_TIMES_WIDTH97;
        ulInvalid = MRS_CLTII_EVENT_INVALID_NV_TIMES97;
    }
    else
    {
        return HI_ERR_FAILURE;
    }

    for (i = 0; i < MRS_TOOLS_ALEN(pstMeter->aulEvent); i++)
    {
        HI_U32 ulTemp = 0;

        (hi_void)memcpy_s(&ulTemp, sizeof(ulTemp), pstItem->aucEventInf + ucWidth * i, ucWidth);
        if (ulTemp == ulInvalid)
        {
            ulTemp = MRS_CLTII_EVENT_INVALID_TIMES;
        }

        pstMeter->aulEvent[i] = ulTemp;
    }

    pstNvInf->ucMeterNum++;

    return HI_ERR_SUCCESS;
}


//*****************************************************************************
// 函数名称: mrsCltiiEventReport
// 功能描述: II采事件上报
//           
// 参数说明:
//   无
//
// 返 回 值: 
//    HI_ERR_SUCCESS: 上报成功
//    其它错误码
//
// 调用要求: TODO: ...
// 调用举例: TODO: ...
// 作    者: fengxiaomin/00209182 [2015-07-17]
//*****************************************************************************
HI_U32 mrsCltiiEventReport(HI_VOID)
{
    HI_U32 ulRet = HI_ERR_SUCCESS;
    HI_U8 *pData = HI_NULL;
    HI_U16 usDataLen = 0;

    if (!mrsGetCltiiEventCtx())
    {
        return HI_ERR_NO_INITILIZATION;
    }

    if (MRS_CLTII_EVENT_MONITOR_WORKING == mrsCltiiEventGetMonitorStatus())
    {
        return HI_ERR_BUSY;
    }
    
    ulRet = mrsCltiiEventCreateEvt645(&pData, &usDataLen);
    if (HI_ERR_SUCCESS != ulRet)
    {
        mrsToolsFree(pData);
        return ulRet;
    }
    
    ulRet = mrsStaEvtReportProc(pData, usDataLen);

    mrsToolsFree(pData);

    return ulRet;
}

//*****************************************************************************
// 函数名称: mrsCltiiEventCreateEvt645
// 功能描述: 创建II采事件上报的645帧
//           
// 参数说明:
//   ppData      645帧指针
//   pusDataLen  645帧长度
//
// 返 回 值: 
//    HI_ERR_SUCCESS: 创建645帧成功
//    其它错误码
//
// 调用要求: TODO: ...
// 调用举例: TODO: ...
// 作    者: fengxiaomin/00209182 [2015-07-17]
//*****************************************************************************
HI_U32 mrsCltiiEventCreateEvt645(HI_OUT HI_U8 **ppData, HI_OUT HI_U16 *pusDataLen)
{
    HI_U32 ulRet = HI_ERR_SUCCESS;
    HI_U8 ucCtrl = MRS_CLTII_EVT_REPORT_645_CTRL;
    MRS_645_FRAME_STRU* pstFrame = HI_NULL;
    MRS_CLTII_EVENT_CTX_STRU *pstEvtCtx = mrsGetCltiiEventCtx();
   
    pstFrame = (MRS_645_FRAME_STRU*)mrsToolsMalloc(sizeof(MRS_645_FRAME_STRU));
    if (!pstFrame)
    {
        return HI_ERR_MALLOC_FAILUE;
    }
    
    (hi_void)memset_s(pstFrame, sizeof(MRS_645_FRAME_STRU), 0, sizeof(MRS_645_FRAME_STRU));
    (hi_void)memcpy_s(&pstFrame->stCtrl, sizeof(MRS_645_CTRL_STRU), &ucCtrl, sizeof(ucCtrl));
    (hi_void)memcpy_s(pstFrame->ucAddr, HI_METER_ADDR_LEN, mrsGetCollectorAddr(), HI_METER_ADDR_LEN);
    pstFrame->ucDataRealm[MRS_CLTII_EVT_645_FLAG_OFFSET] = MRS_CLTII_EVT_REPORT_645_FLAG;
    pstFrame->ucDataRealm[MRS_CLTII_EVT_METER_TOTAL_NUM_OFFSET] = pstEvtCtx->stMeterList.ucMeterTotal;
    
    ulRet = mrsCltiiEventSetReportEvtInfo(pstEvtCtx, pstFrame->ucDataRealm + MRS_CLTII_EVT_REPORT_METER_NUM_OFFSET);
    if (HI_ERR_SUCCESS != ulRet)
    {
        mrsToolsFree(pstFrame);
        return ulRet;
    }

    pstFrame->ucDataRealmLen = (HI_U8)(MRS_CLTII_EVT_REPORT_INFO_FIX_LEN 
                + pstFrame->ucDataRealm[MRS_CLTII_EVT_REPORT_METER_NUM_OFFSET] * MRS_CLTII_EVT_REPORT_INFO_SINGLE_LEN);

    mrs645DataEncode(pstFrame->ucDataRealm, pstFrame->ucDataRealmLen);
    
    *ppData = (HI_U8*)mrsToolsMalloc(MRS_PROTO645_DATAGRAM_LEN_MAX);
    if (!*ppData)
    {
        mrsToolsFree(pstFrame);
        return HI_ERR_MALLOC_FAILUE;
    }

    (hi_void)memset_s(*ppData, MRS_PROTO645_DATAGRAM_LEN_MAX, 0, MRS_PROTO645_DATAGRAM_LEN_MAX);
    
    ulRet = MRS_Proto645Enc(*ppData, pusDataLen, pstFrame);

    mrsToolsFree(pstFrame);
    
    return ulRet;
}

//*****************************************************************************
// 函数名称: mrsCltiiEventSetReportEvtInfo
// 功能描述: 设置上报事件645帧数据域中的事件信息
//           
// 参数说明:
//   pstEvtCtx 事件上下文
//   pData     645帧数据域中的事件信息指针
//
// 返 回 值: 
//    HI_ERR_SUCCESS: 设置成功
//    其它错误码
//
// 调用要求: TODO: ...
// 调用举例: TODO: ...
// 作    者: fengxiaomin/00209182 [2015-07-18]
//*****************************************************************************
HI_U32 mrsCltiiEventSetReportEvtInfo(MRS_CLTII_EVENT_CTX_STRU *pstEvtCtx, HI_U8 *pData)
{
    HI_U8 ucIndex = 0;
    HI_U8 ucOffset = 0;
    HI_U8 ucEvtMeterNum = 0;
    MRS_CLTII_EVENT_METERLIST *pstMeterList = &pstEvtCtx->stMeterList;

    ucOffset += sizeof(ucEvtMeterNum);
    
    for (ucIndex = pstEvtCtx->stParam.ucCurrEvtIndex; ucIndex < MRS_MAX_METERNUM; ucIndex++)
    {
        if (pstMeterList->aucEventInf[ucIndex])
        {
            (hi_void)memcpy_s(pData + ucOffset, HI_METER_ADDR_LEN, pstMeterList->astMeterInf[ucIndex].aucAddr, HI_METER_ADDR_LEN);
            ucOffset += HI_METER_ADDR_LEN;
            pData[ucOffset++] = pstMeterList->astMeterInf[ucIndex].ucProtocol;
            pData[ucOffset++] = pstMeterList->aucEventInf[ucIndex];

            if (ucEvtMeterNum + 1 > MRS_CLTII_EVT_REPORT_INFO_NUM_MAX)
            {
                break;
            }
            
            pstEvtCtx->stParam.aucCurrReportIndex[ucEvtMeterNum++] = ucIndex;
            
            pstEvtCtx->stParam.ucCurrEvtIndex = ucIndex + 1;
            
            if ((MRS_CLTII_EVT_REPORT_INFO_NUM_MAX == ucEvtMeterNum) 
                && (ucEvtMeterNum < pstEvtCtx->stMeterList.ucEventCount))
            {
                pstEvtCtx->stParam.ucCurrEvtIndex = ucIndex + 1;
                break;
            }
        }
    }

    pstEvtCtx->stParam.ucCurrReportNum = ucEvtMeterNum;
    if (!ucEvtMeterNum)
    {
        return HI_ERR_BAD_DATA;
    }
    
    pData[0] = ucEvtMeterNum;
    
    return HI_ERR_SUCCESS;
}

//*****************************************************************************
// 函数名称: mrsCltiiEventNvDelSMNonExistInfo
// 功能描述: II采搜表结束后根据搜表结果NV更新II采事件信息NV,删除搜表结果中不存在但事件NV中存在的表
//           
// 参数说明:
//   无
//
// 返 回 值: 
//    HI_ERR_SUCCESS: 更新成功
//    其它错误码
//
// 调用要求: TODO: ...
// 调用举例: TODO: ...
// 作    者: fengxiaomin/00209182 [2015-07-20]
//*****************************************************************************
HI_U32 mrsCltiiEventNvDelSMNonExistInfo(HI_VOID)
{
    HI_U32 ulRet = HI_ERR_SUCCESS;
    NV_APP_MRS_SM_NODE *pstMeter = HI_NULL;
    NV_APP_CLTII_EVENT_ITEM *pstEvent = HI_NULL;

    ulRet = mrsCltGetNvSMResult((HI_PVOID*)&pstMeter);
    if (HI_ERR_SUCCESS != ulRet)
    {
        mrsToolsFree(pstMeter);
        return ulRet;
    }

    ulRet = mrsCltiiEventGetNvMeterInfo((HI_PVOID*)&pstEvent);
    if (HI_ERR_SUCCESS != ulRet)
    {
        mrsToolsFree(pstMeter);
        mrsToolsFree(pstEvent);
        return ulRet;
    }

    ulRet = mrsCltiiEventDelNvInfo(pstMeter, pstEvent);
    
    mrsToolsFree(pstMeter);
    mrsToolsFree(pstEvent);

    return ulRet;
}

//*****************************************************************************
// 函数名称: mrsCltiiEventGetNvMeterInfo
// 功能描述: 获取II采事件信息NV
//           
// 参数说明:
//   ppData      搜表结果NV中的表信息
//
// 返 回 值: 
//    HI_ERR_SUCCESS: 成功
//    其它错误码
//
// 调用要求: TODO: ...
// 调用举例: TODO: ...
// 作    者: fengxiaomin/00209182 [2015-07-20]
//*****************************************************************************
HI_U32 mrsCltiiEventGetNvMeterInfo(HI_OUT HI_PVOID *ppData)
{
    HI_U32 ulRet = HI_ERR_SUCCESS;
    HI_U8 ucIndex = 0;
    HI_U16 ausEventId[] = {ID_NV_APP_CLT_EVT_INFO1,ID_NV_APP_CLT_EVT_INFO2,ID_NV_APP_CLT_EVT_INFO3,ID_NV_APP_CLT_EVT_INFO4};
    HI_U8 ucSize = sizeof(NV_APP_CLTII_EVENT_INF_STRU) / sizeof(NV_APP_CLTII_EVENT_ITEM);
    NV_APP_CLTII_EVENT_ITEM *pstEvent = HI_NULL;

    pstEvent = (NV_APP_CLTII_EVENT_ITEM*)mrsToolsMalloc(sizeof(NV_APP_CLTII_EVENT_INF_STRU)
                                                        * MRS_TOOLS_ARRAY_COUNT(ausEventId));
    if (!pstEvent)
    {
        return HI_ERR_MALLOC_FAILUE;
    }
    
    (hi_void)memset_s(pstEvent,
        sizeof(NV_APP_CLTII_EVENT_INF_STRU) * MRS_TOOLS_ARRAY_COUNT(ausEventId),0,
        sizeof(NV_APP_CLTII_EVENT_INF_STRU) * MRS_TOOLS_ARRAY_COUNT(ausEventId));
    
    for (ucIndex = 0; ucIndex < MRS_TOOLS_ARRAY_COUNT(ausEventId); ucIndex++)
    {
        ulRet = HI_MDM_NV_Read(ausEventId[ucIndex], 
                               pstEvent + ucIndex * ucSize, 
                               sizeof(NV_APP_CLTII_EVENT_INF_STRU));
        if (HI_ERR_SUCCESS != ulRet)
        {
            mrsToolsFree(pstEvent);
            return HI_ERR_FAILURE;
        }
    }

    *ppData = (HI_PVOID)pstEvent;
    
    return ulRet;
}

//*****************************************************************************
// 函数名称: mrsCltiiEventDelNvInfo
// 功能描述: 删除搜表结果中不存在但事件NV中存在的表信息
//           
// 参数说明:
//   pstMeter      搜表结果NV中的表信息
//   pstEvent      II采事件NV中的表信息
//
// 返 回 值: 
//    HI_ERR_SUCCESS: 成功
//    其它错误码
//
// 调用要求: TODO: ...
// 调用举例: TODO: ...
// 作    者: fengxiaomin/00209182 [2015-07-20]
//*****************************************************************************
HI_U32 mrsCltiiEventDelNvInfo(NV_APP_MRS_SM_NODE *pstMeter, NV_APP_CLTII_EVENT_ITEM *pstEvent)
{
    HI_U32 ulRet = HI_ERR_SUCCESS;
    HI_U8 aucChangeFlag[MRS_CLTII_EVENT_NV_INFO_NUM] = {0};

    (hi_void)memset_s(aucChangeFlag, sizeof(aucChangeFlag), 0, sizeof(aucChangeFlag));
    ulRet = mrsCltiiEventCompareSMProc(pstMeter, pstEvent, aucChangeFlag);
    if (HI_ERR_SUCCESS == ulRet)
    {
        ulRet = mrsCltiiEventInfoWriteNv(pstEvent, aucChangeFlag);
    }

    return ulRet;
}

//*****************************************************************************
// 函数名称: mrsCltiiEventCompareSMProc
// 功能描述: 比较事件NV与搜表结果NV不同,且搜表结果中不存在但事件NV中存在的表信息
//           
// 参数说明:
//   pstMeter      搜表结果NV中的表信息
//   pstEvent      II采事件NV中的表信息
//
// 返 回 值: 
//    HI_ERR_SUCCESS: 成功
//    其它错误码
//
// 调用要求: TODO: ...
// 调用举例: TODO: ...
// 作    者: fengxiaomin/00209182 [2015-07-20]
//*****************************************************************************
HI_U32 mrsCltiiEventCompareSMProc(NV_APP_MRS_SM_NODE *pstMeter, NV_APP_CLTII_EVENT_ITEM *pstEvent, 
                                              HI_U8 pucChangeFlag[])
{
    HI_U8 ucIndex1 = 0, ucIndex2 = 0;
    HI_U8 ucEventItemNum = sizeof(NV_APP_CLTII_EVENT_INF_STRU) / sizeof(NV_APP_CLTII_EVENT_ITEM);

    for (ucIndex1 = 0; ucIndex1 < MRS_MAX_METERNUM; ucIndex1++)
    {
        NV_APP_CLTII_EVENT_ITEM *pstEventItem = &pstEvent[ucIndex1];
        
        for (ucIndex2 = 0; ucIndex2 < MRS_MAX_METERNUM; ucIndex2++)
        {
            NV_APP_MRS_SM_NODE *pstSMNode = &pstMeter[ucIndex2];
            
            if (mrsToolsMemEq(pstSMNode->aucAddr, pstEventItem->aucMeterAddr, HI_METER_ADDR_LEN)    // addr
                && ((pstSMNode->ucParam & 0xF) == (pstEventItem->ucOption & MRS_CLTII_EVENT_NV_PROTO_MASK)))                 // protocol
            {
                break;
            }
        }

        if (MRS_MAX_METERNUM == ucIndex2)
        {
            HI_U8 ucChangeBlock = ucIndex1 / ucEventItemNum;
            
            pucChangeFlag[ucChangeBlock] = MRS_CLTII_EVENT_NV_NEED_SAVE;
            (hi_void)memset_s(pstEventItem, sizeof(NV_APP_CLTII_EVENT_ITEM), 0, sizeof(NV_APP_CLTII_EVENT_ITEM));
        }
    }

    return HI_ERR_SUCCESS;
}

//*****************************************************************************
// 函数名称: mrsCltiiEventInfoWriteNv
// 功能描述: 将II采事件信息写NV
//           
// 参数说明:
//   pstEvent        II采事件NV中的表信息
//   pucChangeFlag   事件改变所在NV项的标志
//
// 返 回 值: 
//    HI_ERR_SUCCESS: 成功
//    其它错误码
//
// 调用要求: TODO: ...
// 调用举例: TODO: ...
// 作    者: fengxiaomin/00209182 [2015-07-20]
//*****************************************************************************
HI_U32 mrsCltiiEventInfoWriteNv(NV_APP_CLTII_EVENT_ITEM *pstEvent, HI_U8 pucChangeFlag[])
{
    HI_U32 ulRet = HI_ERR_SUCCESS;
    HI_U8 ucIndex = 0;
    HI_U8 ucSize = sizeof(NV_APP_CLTII_EVENT_INF_STRU) / sizeof(NV_APP_CLTII_EVENT_ITEM);
    HI_U16 ausEventId[] = {ID_NV_APP_CLT_EVT_INFO1,ID_NV_APP_CLT_EVT_INFO2,ID_NV_APP_CLT_EVT_INFO3,ID_NV_APP_CLT_EVT_INFO4};

    if (mrsToolsMemIsZero(pucChangeFlag, MRS_CLTII_EVENT_NV_INFO_NUM))
    {
        return HI_ERR_SUCCESS;
    }

    for (ucIndex = 0; ucIndex < MRS_CLTII_EVENT_NV_INFO_NUM; ucIndex++)
    {
        if (pucChangeFlag[ucIndex])
        {
            ulRet |= HI_MDM_NV_Write(ausEventId[ucIndex], pstEvent + ucSize * ucIndex, sizeof(NV_APP_CLTII_EVENT_INF_STRU));
        }
    }

    mrsCltiiEventInfoLoadNv();

    return ulRet;
}



//*****************************************************************************
// 函数名称: mrsCltiiEventInfoRxAckSaveNv
// 功能描述: II采收到CCO确认后将事件信息写NV
//           
// 参数说明:
//   无
//
// 返 回 值: 
//    HI_ERR_SUCCESS: 成功
//    其它错误码
//
// 调用要求: TODO: ...
// 调用举例: TODO: ...
// 作    者: fengxiaomin/00209182 [2015-07-20]
//*****************************************************************************
HI_U32 mrsCltiiEventInfoRxAckSaveNv(HI_VOID)
{
    HI_U32 ulRet = HI_ERR_SUCCESS;
    HI_U8 ucIndex1 = 0, ucIndex2 = 0;
    HI_U8 aucChangeFlag[MRS_CLTII_EVENT_NV_INFO_NUM] = {0};
    MRS_CLTII_EVENT_CTX_STRU *pstEvtCtx = mrsGetCltiiEventCtx();
    MRS_CLTII_EVENT_PARAM *pstParam = HI_NULL;
    MRS_CLTII_EVENT_METERLIST *pstMeterList = HI_NULL;
    NV_APP_CLTII_EVENT_ITEM *pstEvent = HI_NULL;

    if (!pstEvtCtx)
    {
        return HI_ERR_NO_INITILIZATION;
    }

    ulRet = mrsCltiiEventGetNvMeterInfo((HI_PVOID)&pstEvent);
    if (HI_ERR_SUCCESS != ulRet)
    {
        mrsToolsFree(pstEvent);
        return ulRet;
    }

    (hi_void)memset_s(aucChangeFlag, sizeof(aucChangeFlag), 0, sizeof(aucChangeFlag));
    
    pstMeterList = &pstEvtCtx->stMeterList;
    pstParam = &pstEvtCtx->stParam;
    
    for (ucIndex1 = 0; ucIndex1 < pstParam->ucCurrReportNum; ucIndex1++)
    {
        HI_U8 ucMeterIndex = pstParam->aucCurrReportIndex[ucIndex1];

        for (ucIndex2 = 0; ucIndex2 < MRS_MAX_METERNUM; ucIndex2++)
        {
            if (mrsToolsMemEq(pstMeterList->astMeterInf[ucMeterIndex].aucAddr, 
                pstEvent[ucIndex2].aucMeterAddr, 
                HI_METER_ADDR_LEN))
            {
                HI_DIAG_LOG_MSG_E3(MRS_FILE_LOG_FLAG_004, HI_DIAG_MT("rx ack update"), pstEvent[ucIndex2].aucMeterAddr[0], 
                    pstEvent[ucIndex2].aucMeterAddr[1],pstEvent[ucIndex2].aucMeterAddr[2]);
                mrsCltiiEventItemRxAckUpdate(&pstEvent[ucIndex2], ucMeterIndex, ucIndex2, aucChangeFlag);
                break;
            }
        }

        if (MRS_MAX_METERNUM == ucIndex2)
        {
            mrsCltiiEventAddMeterInfo(pstEvent, ucMeterIndex, aucChangeFlag, MRS_CLTII_EVENT_RX_PLC_ACK);
        }
    }

    ulRet = mrsCltiiEventInfoWriteNv(pstEvent, aucChangeFlag);

    mrsDfxCltiiEventLastReported(pstParam->ucCurrReportNum);

    pstParam->ucCurrReportNum = 0;
    (hi_void)memset_s(pstParam->aucCurrReportIndex, sizeof(pstParam->aucCurrReportIndex), 0, sizeof(pstParam->aucCurrReportIndex));
    
    mrsToolsFree(pstEvent);
    
    return ulRet;
}

//*****************************************************************************
// 函数名称: mrsCltiiEventRxClearSaveNv
// 功能描述: II采收到清除事件标识命令后将事件信息写NV
//           
// 参数说明:
//   无
//
// 返 回 值: 
//    HI_ERR_SUCCESS: 成功
//    其它错误码
//
// 调用要求: TODO: ...
// 调用举例: TODO: ...
// 作    者: fengxiaomin/00209182 [2015-12-01]
//*****************************************************************************
HI_U32 mrsCltiiEventRxClearSaveNv(HI_U8 ucMeterIndex)
{
    HI_U32 ulRet = HI_ERR_SUCCESS;
    HI_U8 ucIndex = 0;
    HI_U8 aucChangeFlag[MRS_CLTII_EVENT_NV_INFO_NUM] = {0};
    MRS_CLTII_EVENT_CTX_STRU *pstEvtCtx = mrsGetCltiiEventCtx();
    MRS_CLTII_EVENT_METERLIST *pstMeterList = HI_NULL;
    NV_APP_CLTII_EVENT_ITEM *pstEvent = HI_NULL;

    if (!pstEvtCtx)
    {
        return HI_ERR_NO_INITILIZATION;
    }

    ulRet = mrsCltiiEventGetNvMeterInfo((HI_PVOID)&pstEvent);
    if (HI_ERR_SUCCESS != ulRet)
    {
        mrsToolsFree(pstEvent);
        return ulRet;
    }

    (hi_void)memset_s(aucChangeFlag, sizeof(aucChangeFlag), 0, sizeof(aucChangeFlag));
    
    pstMeterList = &pstEvtCtx->stMeterList;
    
    for (ucIndex = 0; ucIndex < MRS_MAX_METERNUM; ucIndex++)
    {
        if (mrsToolsMemEq(pstMeterList->astMeterInf[ucMeterIndex].aucAddr, 
            pstEvent[ucIndex].aucMeterAddr, 
            HI_METER_ADDR_LEN))
        {
            HI_DIAG_LOG_MSG_E3(MRS_FILE_LOG_FLAG_010, HI_DIAG_MT("rx clear update"), pstEvent[ucIndex].aucMeterAddr[0], 
                pstEvent[ucIndex].aucMeterAddr[1],pstEvent[ucIndex].aucMeterAddr[2]);
            mrsCltiiEventItemRxAckUpdate(&pstEvent[ucIndex], ucMeterIndex, ucIndex, aucChangeFlag);
            break;
        }
    }

    if (MRS_MAX_METERNUM == ucIndex)
    {
        mrsCltiiEventAddMeterInfo(pstEvent, ucMeterIndex, aucChangeFlag, MRS_CLTII_EVENT_RX_PLC_CLEAR);
    }

    ulRet = mrsCltiiEventInfoWriteNv(pstEvent, aucChangeFlag);

    mrsDfxCltiiEventLastCleared();

    mrsToolsFree(pstEvent);
    
    return ulRet;

}

//*****************************************************************************
// 函数名称: mrsCltiiEventMonInfoSaveNv
// 功能描述: II采本地监控后,将事件NV中没有的电表信息写入NV
//           
// 参数说明:
//   无
//
// 返 回 值: 
//    HI_ERR_SUCCESS: 成功
//    其它错误码
//
// 调用要求: TODO: ...
// 调用举例: TODO: ...
// 作    者: fengxiaomin/00209182 [2015-07-20]
//*****************************************************************************
HI_U32 mrsCltiiEventMonInfoSaveNv(HI_VOID)
{
    HI_U32 ulRet = HI_ERR_SUCCESS;
    HI_U8 ucIndex1 = 0, ucIndex2 = 0;
    HI_U8 aucChangeFlag[MRS_CLTII_EVENT_NV_INFO_NUM] = {0};
    MRS_CLTII_EVENT_CTX_STRU *pstEvtCtx = mrsGetCltiiEventCtx();
    MRS_CLTII_EVENT_METERLIST *pstMeterList = HI_NULL;
    NV_APP_CLTII_EVENT_ITEM *pstEvent = HI_NULL;

    if (!pstEvtCtx)
    {
        return HI_ERR_NO_INITILIZATION;
    }

    ulRet = mrsCltiiEventGetNvMeterInfo((HI_PVOID)&pstEvent);
    if (HI_ERR_SUCCESS != ulRet)
    {
        mrsToolsFree(pstEvent);
        return ulRet;
    }

    (hi_void)memset_s(aucChangeFlag, sizeof(aucChangeFlag), 0, sizeof(aucChangeFlag));
    
    pstMeterList = &pstEvtCtx->stMeterList;
    
    for (ucIndex1 = 0; ucIndex1 < pstMeterList->ucMeterTotal; ucIndex1++)
    {
        for (ucIndex2 = 0; ucIndex2 < MRS_MAX_METERNUM; ucIndex2++)
        {
            if (mrsToolsMemEq(pstMeterList->astMeterInf[ucIndex1].aucAddr, 
                pstEvent[ucIndex2].aucMeterAddr, 
                HI_METER_ADDR_LEN))
            {
                HI_DIAG_LOG_MSG_E3(MRS_FILE_LOG_FLAG_005, HI_DIAG_MT("monitor update"), pstEvent[ucIndex2].aucMeterAddr[0], 
                    pstEvent[ucIndex2].aucMeterAddr[1],pstEvent[ucIndex2].aucMeterAddr[2]);
                mrsCltiiEventItemMonUpdate(&pstEvent[ucIndex2], ucIndex1, ucIndex2, aucChangeFlag);
                break;
            }
        }

        HI_DIAG_LOG_MSG_E3(MRS_FILE_LOG_FLAG_001, HI_DIAG_MT("MonInfoSaveNv meter"), pstMeterList->astMeterInf[ucIndex1].aucAddr[0], 
            pstMeterList->astMeterInf[ucIndex1].aucAddr[1],pstMeterList->astMeterInf[ucIndex1].aucAddr[2]);
        HI_DIAG_LOG_MSG_E3(MRS_FILE_LOG_FLAG_002, HI_DIAG_MT("MonInfoSaveNv eventinfo"), pstMeterList->astMeterInf[ucIndex1].stRecordInf.aulRecordInf[0], 
            pstMeterList->astMeterInf[ucIndex1].stRecordInf.aulRecordInf[1],pstMeterList->astMeterInf[ucIndex1].stRecordInf.aulRecordInf[2]);
        HI_DIAG_LOG_MSG_E2(MRS_FILE_LOG_FLAG_003, HI_DIAG_MT("MonInfoSaveNv index"), ucIndex1,ucIndex2);
        
        if (MRS_MAX_METERNUM == ucIndex2)
        {
            mrsCltiiEventAddMeterInfo(pstEvent, ucIndex1, aucChangeFlag, MRS_CLTII_EVENT_LOCAL_MONITOR_END);
        }
    }

    ulRet = mrsCltiiEventInfoWriteNv(pstEvent, aucChangeFlag);
    
    mrsToolsFree(pstEvent);
    
    return ulRet;
}

//*****************************************************************************
// 函数名称: mrsCltiiEventItemRxAckUpdate
// 功能描述: 收到确认后更新一条表事件信息
//           
// 参数说明:
//   无
//
// 返 回 值: 
//    HI_ERR_SUCCESS: 成功
//    其它错误码
//
// 调用要求: TODO: ...
// 调用举例: TODO: ...
// 作    者: fengxiaomin/00209182 [2015-07-20]
//*****************************************************************************
HI_U32 mrsCltiiEventItemRxAckUpdate(NV_APP_CLTII_EVENT_ITEM *pstEvent, HI_U8 ucMeterIndex, HI_U8 ucEventIndex, HI_U8 pucChangeFlag[])
{
    HI_U32 ulRet = HI_ERR_SUCCESS;
    MRS_CLTII_EVENT_CTX_STRU *pstEvtCtx = mrsGetCltiiEventCtx();
    MRS_CLTII_EVENT_METERLIST *pstMeterList = &pstEvtCtx->stMeterList;

    if (METER_PROTO_645_2007 == pstMeterList->astMeterInf[ucMeterIndex].ucProtocol)
    {
        ulRet = mrsCltiiEvent07ItemUpdate(pstEvent, ucMeterIndex, ucEventIndex, pucChangeFlag);
    }
    else if (METER_PROTO_645_1997 == pstMeterList->astMeterInf[ucMeterIndex].ucProtocol)
    {
        ulRet = mrsCltiiEvent97ItemUpdate(pstEvent, ucMeterIndex, ucEventIndex, pucChangeFlag);
    }
    else
    {
        return HI_ERR_FAILURE;
    }

    return ulRet;
}

//*****************************************************************************
// 函数名称: mrsCltiiEventItemMonUpdate
// 功能描述: 本地监控完成后更新一条表事件信息
//           
// 参数说明:
//   无
//
// 返 回 值: 
//    HI_ERR_SUCCESS: 成功
//    其它错误码
//
// 调用要求: TODO: ...
// 调用举例: TODO: ...
// 作    者: fengxiaomin/00209182 [2015-07-20]
//*****************************************************************************
HI_U32 mrsCltiiEventItemMonUpdate(NV_APP_CLTII_EVENT_ITEM *pstEvent, HI_U8 ucMeterIndex, HI_U8 ucEventIndex, HI_U8 pucChangeFlag[])
{
    HI_U8 ucIndex = 0;
    HI_U8 ucWidth = 0;
    HI_U8 ucEventItemNum = sizeof(NV_APP_CLTII_EVENT_INF_STRU) / sizeof(NV_APP_CLTII_EVENT_ITEM);
    HI_U8 ucChangeBlock = ucEventIndex / ucEventItemNum;
    MRS_CLTII_EVENT_CTX_STRU *pstEvtCtx = mrsGetCltiiEventCtx();
    MRS_CLTII_EVENT_METERLIST *pstMeterList = &pstEvtCtx->stMeterList;

    if (METER_PROTO_645_2007 == pstMeterList->astMeterInf[ucMeterIndex].ucProtocol)
    {
        ucWidth = MRS_CLTII_EVENT_TIMES_WIDTH;
    }
    else if (METER_PROTO_645_1997 == pstMeterList->astMeterInf[ucMeterIndex].ucProtocol)
    {
        ucWidth = MRS_CLTII_EVENT_TIMES_WIDTH97;
    }
    else
    {
        return HI_ERR_FAILURE;
    }
    
    for (ucIndex = 0; ucIndex < MRS_CLTII_EVENT_RECORD_INDEX_MAX; ucIndex++)
    {
        if (mrsToolsMemIsFF(pstEvent->aucEventInf + ucWidth * ucIndex, ucWidth)
            && (MRS_CLTII_EVENT_RECORD_MASK != pstMeterList->astMeterInf[ucMeterIndex].stRecordInf.aulRecordInf[ucIndex]))
        {
            HI_DIAG_LOG_MSG_E1(MRS_FILE_LOG_FLAG_006, HI_DIAG_MT("monitor update item"), pstMeterList->astMeterInf[ucMeterIndex].stRecordInf.aulRecordInf[ucIndex]);
            (hi_void)memcpy_s(pstEvent->aucEventInf + ucWidth * ucIndex, sizeof(pstEvent->aucEventInf) - ucWidth * ucIndex, 
                           &pstMeterList->astMeterInf[ucMeterIndex].stRecordInf.aulRecordInf[ucIndex], 
                           ucWidth);
        }
    }

    pucChangeFlag[ucChangeBlock] = MRS_CLTII_EVENT_NV_NEED_SAVE;
    
    return HI_ERR_SUCCESS;
}

//*****************************************************************************
// 函数名称: mrsCltiiEvent07ItemUpdate
// 功能描述: 更新一条07规约表事件信息
//           
// 参数说明:
//   无
//
// 返 回 值: 
//    HI_ERR_SUCCESS: 成功
//    其它错误码
//
// 调用要求: TODO: ...
// 调用举例: TODO: ...
// 作    者: fengxiaomin/00209182 [2015-07-20]
//*****************************************************************************
HI_U32 mrsCltiiEvent07ItemUpdate(NV_APP_CLTII_EVENT_ITEM *pstEvent, HI_U8 ucMeterIndex, HI_U8 ucEventIndex, HI_U8 pucChangeFlag[])
{
    HI_U8 ucIndex =0;
    HI_U8 ucEventItemNum = sizeof(NV_APP_CLTII_EVENT_INF_STRU) / sizeof(NV_APP_CLTII_EVENT_ITEM);
    HI_U8 ucChangeBlock = ucEventIndex / ucEventItemNum;
    MRS_CLTII_EVENT_CTX_STRU *pstEvtCtx = mrsGetCltiiEventCtx();
    MRS_CLTII_EVENT_METERLIST *pstMeterList = &pstEvtCtx->stMeterList;
    
    if (pstMeterList->astMeterInf[ucMeterIndex].ucOption)
    {
        pstMeterList->astMeterInf[ucMeterIndex].ucOption &= ~MRS_CLTII_EVENT_STATUS_REPORT_FLAG;
        pstMeterList->aucEventInf[ucMeterIndex] = 0;
        return HI_ERR_SKIP;
    }

    for (ucIndex = 0; ucIndex < MRS_CLTII_EVENT_RECORD_INDEX_MAX; ucIndex++)
    {
        HI_U8 ucEvtIndex = (HI_U8)(MRS_CLTII_EVENT_07_PROTOCOL_FLAG_BASE << ucIndex);
        
        if (ucEvtIndex & pstMeterList->aucEventInf[ucMeterIndex])
        {
            (hi_void)memcpy_s(pstEvent->aucEventInf + MRS_CLTII_EVENT_TIMES_WIDTH * ucIndex, 
                            sizeof(pstEvent->aucEventInf) - MRS_CLTII_EVENT_TIMES_WIDTH * ucIndex,
                           &pstMeterList->astMeterInf[ucMeterIndex].stRecordInf.aulRecordInf[ucIndex], 
                           MRS_CLTII_EVENT_TIMES_WIDTH);
            pucChangeFlag[ucChangeBlock] = MRS_CLTII_EVENT_NV_NEED_SAVE;
            pstMeterList->aucEventInf[ucMeterIndex] &= (~ucEvtIndex);
        }
    }
    

    return HI_ERR_SUCCESS;
}

//*****************************************************************************
// 函数名称: mrsCltiiEvent97ItemUpdate
// 功能描述: 更新一条97规约表事件信息
//           
// 参数说明:
//   无
//
// 返 回 值: 
//    HI_ERR_SUCCESS: 成功
//    其它错误码
//
// 调用要求: TODO: ...
// 调用举例: TODO: ...
// 作    者: fengxiaomin/00209182 [2015-07-20]
//*****************************************************************************
HI_U32 mrsCltiiEvent97ItemUpdate(NV_APP_CLTII_EVENT_ITEM *pstEvent, HI_U8 ucMeterIndex, HI_U8 ucEventIndex, HI_U8 pucChangeFlag[])
{
    HI_U8 ucIndex =0;
    HI_U8 ucEventItemNum = sizeof(NV_APP_CLTII_EVENT_INF_STRU) / sizeof(NV_APP_CLTII_EVENT_ITEM);
    HI_U8 ucChangeBlock = ucEventIndex / ucEventItemNum;
    MRS_CLTII_EVENT_CTX_STRU *pstEvtCtx = mrsGetCltiiEventCtx();
    MRS_CLTII_EVENT_METERLIST *pstMeterList = &pstEvtCtx->stMeterList;
    
    if (pstMeterList->astMeterInf[ucMeterIndex].ucOption 
        && (pstMeterList->aucEventInf[ucMeterIndex] & MRS_CLTII_EVT_MONITOR_EVT_4_FLAG))
    {
        pstMeterList->aucEventInf[ucMeterIndex] &= ~MRS_CLTII_EVT_MONITOR_EVT_4_FLAG;
    }
    
    for (ucIndex = 0; ucIndex < MRS_CLTII_EVENT_RECORD_INDEX_MAX; ucIndex++)
    {
        HI_U8 ucEvtIndex = (HI_U8)(MRS_CLTII_EVENT_97_PROTOCOL_FLAG_BASE << ucIndex);
        
        if (ucEvtIndex & pstMeterList->aucEventInf[ucMeterIndex])
        {
            (hi_void)memcpy_s(pstEvent->aucEventInf + MRS_CLTII_EVENT_TIMES_WIDTH97 * ucIndex,
                            sizeof(pstEvent->aucEventInf) - MRS_CLTII_EVENT_TIMES_WIDTH97 * ucIndex,
                           &pstMeterList->astMeterInf[ucMeterIndex].stRecordInf.aulRecordInf[ucIndex], 
                           MRS_CLTII_EVENT_TIMES_WIDTH97);
            pucChangeFlag[ucChangeBlock] = MRS_CLTII_EVENT_NV_NEED_SAVE;
            pstMeterList->aucEventInf[ucMeterIndex] &= (~ucEvtIndex);
        }
    }
        
    return HI_ERR_SUCCESS;
}

//*****************************************************************************
// 函数名称: mrsCltiiEventAddMeterInfo
// 功能描述: 添加一条表事件信息到II采事件信息中
//           
// 参数说明:
//   无
//
// 返 回 值: 
//    HI_ERR_SUCCESS: 成功
//    其它错误码
//
// 调用要求: TODO: ...
// 调用举例: TODO: ...
// 作    者: fengxiaomin/00209182 [2015-07-20]
//*****************************************************************************
HI_U32 mrsCltiiEventAddMeterInfo(NV_APP_CLTII_EVENT_ITEM *pstEvent, HI_U8 ucMeterIndex, 
                                          HI_U8 pucChangeFlag[], HI_U8 ucFrom)
{
    HI_U32 ulRet = HI_ERR_SUCCESS;
    HI_U8 ucIndex = 0;
    MRS_CLTII_EVENT_CTX_STRU *pstEvtCtx = mrsGetCltiiEventCtx();
    MRS_CLTII_EVENT_METERLIST *pstMeterList = &pstEvtCtx->stMeterList;

    for (ucIndex = 0; ucIndex < MRS_MAX_METERNUM; ucIndex++)
    {
        NV_APP_CLTII_EVENT_ITEM stEventItem;

        (hi_void)memcpy_s(&stEventItem, sizeof(stEventItem), &pstEvent[ucIndex], sizeof(NV_APP_CLTII_EVENT_ITEM));
        
        if (!(stEventItem.ucOption & MRS_CLTII_EVENT_ITEM_VALID_MASK))
        {   
            ulRet = mrsCltiiEventNewItemAdd(&stEventItem, ucMeterIndex, ucIndex, pucChangeFlag, ucFrom);
            if (HI_ERR_SUCCESS == ulRet)
            {
                (hi_void)memcpy_s(pstEvent[ucIndex].aucEventInf, sizeof(pstEvent[ucIndex].aucEventInf),
                    stEventItem.aucEventInf, sizeof(stEventItem.aucEventInf));
                (hi_void)memcpy_s(pstEvent[ucIndex].aucMeterAddr, HI_METER_ADDR_LEN,
                               pstMeterList->astMeterInf[ucMeterIndex].aucAddr, 
                               HI_METER_ADDR_LEN);
                pstEvent[ucIndex].ucOption = pstMeterList->astMeterInf[ucMeterIndex].ucProtocol;
                pstEvent[ucIndex].ucOption |= MRS_CLTII_EVENT_ITEM_VALID_MASK;
            }

            break;
        }
    }

    return ulRet;
}

//*****************************************************************************
// 函数名称: mrsCltiiEventNewItemAdd
// 功能描述: II采NV中的事件信息增加新条目
//           
// 参数说明:
//   无
//
// 返 回 值: 
//    HI_ERR_SUCCESS: 成功
//    其它错误码
//
// 调用要求: TODO: ...
// 调用举例: TODO: ...
// 作    者: fengxiaomin/00209182 [2015-12-01]
//*****************************************************************************
HI_U32 mrsCltiiEventNewItemAdd(NV_APP_CLTII_EVENT_ITEM *pstEvent, HI_U8 ucMeterIndex, 
                                          HI_U8 ucEventIndex, HI_U8 pucChangeFlag[], HI_U8 ucFrom)
{
    HI_U8 ucWidth = 0, ucBase = 0;
    HI_U8 ucIndex = 0;
    HI_U8 ucEventItemNum = sizeof(NV_APP_CLTII_EVENT_INF_STRU) / sizeof(NV_APP_CLTII_EVENT_ITEM);
    HI_U8 ucChangeBlock = ucEventIndex / ucEventItemNum;
    MRS_CLTII_EVENT_CTX_STRU *pstEvtCtx = mrsGetCltiiEventCtx();
    MRS_CLTII_EVENT_METERLIST *pstMeterList = &pstEvtCtx->stMeterList;

    if (METER_PROTO_645_2007 == pstMeterList->astMeterInf[ucMeterIndex].ucProtocol)
    {
        if (((MRS_CLTII_EVENT_RX_PLC_ACK == ucFrom) || (MRS_CLTII_EVENT_RX_PLC_CLEAR == ucFrom))
            && (pstMeterList->aucEventInf[ucMeterIndex] & MRS_CLTII_EVT_MONITOR_EVT_1_FLAG))
        {
            pstMeterList->astMeterInf[ucMeterIndex].ucOption &= ~MRS_CLTII_EVENT_STATUS_REPORT_FLAG;
            pstMeterList->aucEventInf[ucMeterIndex] &= ~MRS_CLTII_EVT_MONITOR_EVT_1_FLAG;
        }
        
        ucWidth = MRS_CLTII_EVENT_TIMES_WIDTH;
        ucBase = MRS_CLTII_EVENT_07_PROTOCOL_FLAG_BASE;
    }
    else if (METER_PROTO_645_1997 == pstMeterList->astMeterInf[ucMeterIndex].ucProtocol)
    {
        if (((MRS_CLTII_EVENT_RX_PLC_ACK == ucFrom) || (MRS_CLTII_EVENT_RX_PLC_CLEAR == ucFrom))
            && (pstMeterList->aucEventInf[ucMeterIndex] & MRS_CLTII_EVT_MONITOR_EVT_4_FLAG))
        {
            pstMeterList->aucEventInf[ucMeterIndex] &= ~MRS_CLTII_EVT_MONITOR_EVT_4_FLAG;
        }
        
        ucWidth = MRS_CLTII_EVENT_TIMES_WIDTH97;
        ucBase = MRS_CLTII_EVENT_97_PROTOCOL_FLAG_BASE;
    }
    else
    {
        return HI_ERR_FAILURE;
    }

    (hi_void)memset_s(pstEvent->aucEventInf, sizeof(pstEvent->aucEventInf), 0xFF, MRS_CLTII_EVENT_RECORD_INDEX_MAX * ucWidth);
    
    for (ucIndex = 0; ucIndex < MRS_CLTII_EVENT_RECORD_INDEX_MAX; ucIndex++)
    {
        if (pstMeterList->aucEventInf[ucMeterIndex] 
            || (MRS_CLTII_EVENT_RX_PLC_ACK == ucFrom)
            || (MRS_CLTII_EVENT_LOCAL_MONITOR_END == ucFrom))
        {
            (hi_void)memcpy_s(pstEvent->aucEventInf + ucWidth * ucIndex, 
                            sizeof(pstEvent->aucEventInf) - ucWidth * ucIndex, 
                           &pstMeterList->astMeterInf[ucMeterIndex].stRecordInf.aulRecordInf[ucIndex], 
                           ucWidth);
        }

        if ((MRS_CLTII_EVENT_RX_PLC_ACK == ucFrom) || (MRS_CLTII_EVENT_RX_PLC_CLEAR == ucFrom))
        {
            HI_U8 ucEvtIndex = (HI_U8)(ucBase << ucIndex);

            if (ucEvtIndex & pstMeterList->aucEventInf[ucMeterIndex])
            {
                pstMeterList->aucEventInf[ucMeterIndex] &= (~ucEvtIndex);
            }
        }
    }
    
    pucChangeFlag[ucChangeBlock] = MRS_CLTII_EVENT_NV_NEED_SAVE;

    return HI_ERR_SUCCESS;
}

//*****************************************************************************
// 函数名称: mrsCltiiEventPlcProc
// 功能描述: II采接收到扩展的清除事件帧处理
//           
// 参数说明:
//   
//
// 返 回 值: 
//    HI_ERR_SUCCESS: 成功
//    其它错误码
//
// 调用要求: TODO: ...
// 调用举例: TODO: ...
// 作    者: fengxiaomin/00209182 [2015-07-23]
//*****************************************************************************
HI_U32 mrsCltiiEventPlcProc(HI_U8 *pData, HI_U16 usDataLen, MRS_STA_ITEM* pstItem)
{
    HI_U32 ulRet = HI_ERR_SUCCESS;
    HI_U16 usPos = 0, usFrameLen = 0;

    ulRet = mrsFind645Frame(pData, (HI_S16)usDataLen, &usPos, &usFrameLen);
    if (HI_ERR_SUCCESS != ulRet)
    {
        return HI_ERR_SKIP;
    }
    
    ulRet = mrsCltiiEventCheck(pData + usPos, usFrameLen);
    if (HI_ERR_SUCCESS != ulRet)
    {
        return HI_ERR_SKIP;
    }

    ulRet = mrsCltiiEventRespProc(pData + usPos, usFrameLen, pstItem);
    
    return ulRet;
}

//*****************************************************************************
// 函数名称: mrsCltiiEventRespProc
// 功能描述: II采应答PLC事件命令
//           
// 参数说明:
//   无
//
// 返 回 值: 
//    HI_ERR_SUCCESS: 成功
//    其它错误码
//
// 调用要求: TODO: ...
// 调用举例: TODO: ...
// 作    者: fengxiaomin/00209182 [2015-12-01]
//*****************************************************************************
HI_U32 mrsCltiiEventRespProc(HI_U8 *pFrame, HI_U16 usFrameLen, MRS_STA_ITEM* pstItem)
{
    HI_U32 ulRet = HI_ERR_SUCCESS;
    HI_U8 ucFlag = 0;
    HI_U8 *pData = HI_NULL;
    HI_U8 ucDataLen = 0;
    HI_U8 *p645Frame = HI_NULL;
    HI_U16 us645Len = 0;
    
    HI_UNREF_PARAM(usFrameLen);

    do
    {
        ucFlag =  pFrame[MRS_645_FRAME_DATA_OFFSET] - MRS_645_FRAME_HEX33;
        HI_DIAG_LOG_MSG_E1(MRS_FILE_LOG_FLAG_019, HI_DIAG_MT("CltiiEventResp ucFlag"), ucFlag);
        if (MRS_CLTII_EVENT_QUERY_FLAG == ucFlag)
        {
            ulRet = mrsCltiiEventQueryProc(pFrame, (HI_PVOID*)&pData, &ucDataLen);
        }
        else if (MRS_CLTII_EVENT_CLEAR_FLAG == ucFlag)
        {
            ulRet = mrsCltiiEventClearProc(pFrame);
        }
        else
        {
            return HI_ERR_SKIP;
        }
        
        if (HI_ERR_SUCCESS != ulRet)
        {
            HI_DIAG_LOG_MSG_E1(MRS_FILE_LOG_FLAG_020, HI_DIAG_MT("Proc ret"), ulRet);
            break;
        }

        ulRet = mrsCltiiEventCreateUpFrame(&p645Frame, &us645Len, pData, ucDataLen, pFrame + MRS_645_FRAME_METERADD_OFFSET);
        if (HI_ERR_SUCCESS == ulRet)
        {
            ulRet = mrsCltiiEventRespCCO(p645Frame, us645Len, pstItem);
        }
    } while (0);

    mrsToolsFree(pData);
    mrsToolsFree(p645Frame);

    return ulRet;
}

//*****************************************************************************
// 函数名称: mrsCltiiEventQueryProc
// 功能描述: II采处理PLC查询事件命令
//           
// 参数说明:
//   无
//
// 返 回 值: 
//    HI_ERR_SUCCESS: 成功
//    其它错误码
//
// 调用要求: TODO: ...
// 调用举例: TODO: ...
// 作    者: fengxiaomin/00209182 [2015-12-01]
//*****************************************************************************
HI_U32 mrsCltiiEventQueryProc(HI_U8 *pFrame, HI_PVOID *ppData, HI_U8 *pucDataLen)
{
    HI_U8 ucIndex = 0, ucQueryIndex = 0, ucQueryNum = 0;
    HI_U8 ucNum = 0, ucReportNum = 0;
    HI_U8 ucBufLen = 0, ucOffset = 0;
    HI_U8 ucEvtSeq = 0;
    HI_U8 *pBuf = HI_NULL;
    MRS_CLTII_EVENT_METERLIST *pstMeterList = &mrsGetCltiiEventCtx()->stMeterList;
    
    ucQueryIndex = pFrame[MRS_645_FRAME_DATA_OFFSET + MRS_CLTII_EVENT_QUERY_INDEX_OFFSET] - MRS_645_FRAME_HEX33;
    ucQueryNum = pFrame[MRS_645_FRAME_DATA_OFFSET + MRS_CLTII_EVENT_QUERY_NUM_OFFSET] - MRS_645_FRAME_HEX33;
    
    if (!ucQueryIndex || (ucQueryIndex > MRS_MAX_METERNUM)
        || !ucQueryNum || (ucQueryNum > MRS_MAX_METERNUM))
    {
        return HI_ERR_BAD_DATA;
    }
    
    ucNum = HI_MIN(ucQueryNum, MRS_CLTII_EVT_REPORT_INFO_NUM_MAX);
    ucBufLen = (HI_U8)(MRS_CLTII_EVENT_QUERY_UP_FIX_LEN + ucNum * MRS_CLTII_EVT_REPORT_INFO_SINGLE_LEN);//数据单元标识(1) + 电表总数量(1) + 本次上报事件表数量(1)
    pBuf = mrsToolsMalloc(ucBufLen);
    if (!pBuf)
    {
        return HI_ERR_MALLOC_FAILUE;
    }
    
    (hi_void)memset_s(pBuf, ucBufLen, 0, ucBufLen);
    pBuf[ucOffset++] = MRS_CLTII_EVENT_QUERY_FLAG;
    pBuf[ucOffset++] = pstMeterList->ucMeterTotal;
    ucOffset++;  //本次上报的数量在最后赋值

    for (ucIndex = 0; ucIndex < MRS_MAX_METERNUM; ucIndex++)
    {
        if (pstMeterList->aucEventInf[ucIndex])
        {
            ucEvtSeq++;
            if ((ucEvtSeq >= ucQueryIndex) && (ucEvtSeq < (ucQueryIndex + ucNum)))
            {
                int ret = memcpy_s(&pBuf[ucOffset], ucBufLen - ucOffset, pstMeterList->astMeterInf[ucIndex].aucAddr, HI_METER_ADDR_LEN);
                ucOffset += HI_METER_ADDR_LEN;
                pBuf[ucOffset++] = pstMeterList->astMeterInf[ucIndex].ucProtocol;
                pBuf[ucOffset++] = pstMeterList->aucEventInf[ucIndex];
                ucReportNum++;
				if(ret != 0){
					continue;
				}
				
            }
        }
    }
    
    pBuf[MRS_CLTII_EVENT_QUERY_REPORT_NUM_OFFSET] = ucReportNum;
    ucBufLen = (HI_U8)(MRS_CLTII_EVENT_QUERY_UP_FIX_LEN + ucReportNum * MRS_CLTII_EVT_REPORT_INFO_SINGLE_LEN);
    *ppData = pBuf;
    *pucDataLen = ucBufLen;

    return HI_ERR_SUCCESS;
}

//*****************************************************************************
// 函数名称: mrsCltiiEventClearProc
// 功能描述: II采处理PLC清除事件标识命令
//           
// 参数说明:
//   无
//
// 返 回 值: 
//    HI_ERR_SUCCESS: 成功
//    其它错误码
//
// 调用要求: TODO: ...
// 调用举例: TODO: ...
// 作    者: fengxiaomin/00209182 [2015-12-01]
//*****************************************************************************
HI_U32 mrsCltiiEventClearProc(HI_U8 *pFrame)
{
    HI_U8 aucAddr[HI_METER_ADDR_LEN] = {0};
    HI_U8 ucEvtFlag = 0, ucIndex = 0;
    MRS_CLTII_EVENT_METERLIST *pstMeterList = &mrsGetCltiiEventCtx()->stMeterList;

    (hi_void)memcpy_s(aucAddr, HI_METER_ADDR_LEN,
                   &pFrame[MRS_645_FRAME_DATA_OFFSET + MRS_CLTII_EVENT_CLEAR_ADDR_OFFSET], 
                   HI_METER_ADDR_LEN);
    mrs645DataDecode(aucAddr, HI_METER_ADDR_LEN);
    ucEvtFlag = pFrame[MRS_645_FRAME_DATA_OFFSET + MRS_CLTII_EVENT_CLEAR_FLAG_OFFSET] - MRS_645_FRAME_HEX33;

    for (ucIndex = 0; ucIndex < MRS_MAX_METERNUM; ucIndex++)
    {
        if (mrsToolsMemEq(aucAddr, pstMeterList->astMeterInf[ucIndex].aucAddr, HI_METER_ADDR_LEN))
        {
            if (pstMeterList->aucEventInf[ucIndex])
            {
                HI_U8 ucEventInf = pstMeterList->aucEventInf[ucIndex];
                //将要清除的事件标识位置为1，在mrsCltiiEventRxClearSaveNv中会清除掉pstMeterList->aucEventInf中bit位为1的标识
                pstMeterList->aucEventInf[ucIndex] = ucEventInf & (~(0x0F & ucEvtFlag));
                if (pstMeterList->aucEventInf[ucIndex])
                {
                    mrsCltiiEventRxClearSaveNv(ucIndex);
                }
                
                pstMeterList->aucEventInf[ucIndex] = ucEventInf & (0x0F & ucEvtFlag);
            }
            
            break;
        }
    }

    return HI_ERR_SUCCESS;
}

//*****************************************************************************
// 函数名称: mrsCltiiEventRespCCO
// 功能描述: II采将事件的确认帧应答给CCO
//           
// 参数说明:
//   
//
// 返 回 值: 
//    HI_ERR_SUCCESS: 成功
//    其它错误码
//
// 调用要求: TODO: ...
// 调用举例: TODO: ...
// 作    者: fengxiaomin/00209182 [2015-07-23]
//*****************************************************************************
HI_U32 mrsCltiiEventRespCCO(HI_U8 *pData, HI_U16 usDataLen, MRS_STA_ITEM* pstItem)
{
    HI_U32 ulRet = HI_ERR_SUCCESS;
    HI_U8 ucProtocol = METER_PROTO_645_2007;
    MRS_STA_ITEM stItem;

    if (HI_ERR_SUCCESS == mrsCltiiCheckMeterAddr(pData + MRS_645_FRAME_METERADD_OFFSET))
    {
        ulRet = mrsCltiiGetMeterProtocol(pData + MRS_645_FRAME_METERADD_OFFSET, &ucProtocol);
        if (HI_ERR_SUCCESS != ulRet)
        {
            ucProtocol = METER_PROTO_645_2007;
        }
    }

    (hi_void)memset_s(&stItem, sizeof(stItem), 0, sizeof(stItem));
    stItem.id = pstItem->id;
    stItem.seq = pstItem->seq;
    (hi_void)memcpy_s(stItem.dev_mac, HI_PLC_MAC_ADDR_LEN, pstItem->dev_mac, HI_PLC_MAC_ADDR_LEN);
	
    return mrsStaTransmit2CCO(pData, usDataLen, ucProtocol, &stItem);
}

//*****************************************************************************
// 函数名称: mrsCltiiEventCreateAckUpFrame
// 功能描述: II采创建事件的确认帧
//           
// 参数说明:
//   
//
// 返 回 值: 
//    HI_ERR_SUCCESS: 成功
//    其它错误码
//
// 调用要求: TODO: ...
// 调用举例: TODO: ...
// 作    者: fengxiaomin/00209182 [2015-07-23]
//*****************************************************************************
HI_U32 mrsCltiiEventCreateUpFrame(HI_U8 **pp645Frame, HI_U16 *pusFrameLen, 
                                             HI_U8 *pData, HI_U8 ucDataLen, HI_U8 *pAddr)
{
    HI_U32 ulRet = HI_ERR_SUCCESS;
    MRS_645_FRAME_STRU* pstFrame = HI_NULL;
    HI_U8 ucCtrl = MRS_CLTII_EVENT_CLEAR_CTRL_UP;
    
    if (!pp645Frame || !pusFrameLen)
    {
        return HI_ERR_INVALID_PARAMETER;
    }
        
    *pp645Frame = (HI_U8*)mrsToolsMalloc(MRS_PROTO645_DATAGRAM_LEN_MAX);
    if (!*pp645Frame)
    {
        return HI_ERR_MALLOC_FAILUE;
    }

    (hi_void)memset_s(*pp645Frame, MRS_PROTO645_DATAGRAM_LEN_MAX, 0, MRS_PROTO645_DATAGRAM_LEN_MAX);
   
    pstFrame = (MRS_645_FRAME_STRU*)mrsToolsMalloc(sizeof(MRS_645_FRAME_STRU));
    if (!pstFrame)
    {
        mrsToolsFree(*pp645Frame);
        return HI_ERR_MALLOC_FAILUE;
    }
    
    (hi_void)memset_s(pstFrame, sizeof(MRS_645_FRAME_STRU), 0, sizeof(MRS_645_FRAME_STRU));
    (hi_void)memcpy_s(&pstFrame->stCtrl, sizeof(MRS_645_CTRL_STRU), &ucCtrl, sizeof(ucCtrl));
    (hi_void)memcpy_s(pstFrame->ucAddr, HI_METER_ADDR_LEN, pAddr, HI_METER_ADDR_LEN);
    pstFrame->ucDataRealmLen = ucDataLen;
    if (pstFrame->ucDataRealmLen)
    {
        (hi_void)memcpy_s(pstFrame->ucDataRealm, sizeof(pstFrame->ucDataRealm), pData, pstFrame->ucDataRealmLen);
        mrs645DataEncode(pstFrame->ucDataRealm, pstFrame->ucDataRealmLen);
    }

    ulRet = MRS_Proto645Enc(*pp645Frame, pusFrameLen, pstFrame);
    
    mrsToolsFree(pstFrame);

    return ulRet;
}

//*****************************************************************************
// 函数名称: mrsCltiiEventCheck
// 功能描述: 检查是否是II采事件PLC命令
//           
// 参数说明:
//   
//
// 返 回 值: 
//    HI_ERR_SUCCESS: 成功
//    其它错误码
//
// 调用要求: TODO: ...
// 调用举例: TODO: ...
// 作    者: fengxiaomin/00209182 [2015-07-23]
//*****************************************************************************
HI_U32 mrsCltiiEventCheck(HI_U8 *pData, HI_U16 usDataLen)
{
    HI_U32 ulRet = HI_ERR_SUCCESS;
    HI_U8 ucCtrl = 0, ucDataLen = 0, ucFlag = 0;
    
    HI_UNREF_PARAM(usDataLen);

    ulRet = mrsCltiiCheckAddr(pData + MRS_645_FRAME_METERADD_OFFSET);
    if (HI_ERR_SUCCESS != ulRet)
    {
        return HI_ERR_FAILURE;
    }

    ucCtrl = *(pData + MRS_645_FRAME_CTRL_OFFSET);
    ucDataLen = *(pData + MRS_645_FRAME_LENGTH_OFFSET);
    ucFlag = *(pData + MRS_645_FRAME_DATA_OFFSET) - MRS_645_FRAME_HEX33;
    
    if ((MRS_CLTII_EVENT_CLEAR_CTRL_DOWN == ucCtrl) 
        && (((MRS_CLTII_EVENT_QUERY_FLAG == ucFlag) && (MRS_CLTII_EVT_REPORT_INFO_FIX_LEN == ucDataLen))
            || ((MRS_CLTII_EVENT_CLEAR_FLAG == ucFlag) && (MRS_CLTII_EVENT_CLEAR_FIX_LEN == ucDataLen))))
    {
        return HI_ERR_SUCCESS;
    }
    
    return HI_ERR_FAILURE;
}

#endif // defined(PRODUCT_CFG_PRODUCT_TYPE_STA)
HI_END_HEADER


