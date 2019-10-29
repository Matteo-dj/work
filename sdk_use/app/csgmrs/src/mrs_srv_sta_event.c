//*****************************************************************************
//
//                  版权所有 (C), 1998-2011, 华为技术有限公司
//
//*****************************************************************************
//  文 件 名   : mrs_srv_sta_event.c
//  版 本 号   : V1.0
//  作    者   : cuiate/00233580
//  生成日期   : 2013-03-20
//  功能描述   : 事件上报函数及接口
//
//  函数列表   : TODO: ...
//  修改历史   :
//  1.日    期 : 2013-03-20
//    作    者 : cuiate/00233580
//    修改内容 : 创建文件
//
//  2.日    期 : 2014-08-20
//    作    者 : liuxipeng/lKF54842
//    修改内容 : 修改文件
//
//*****************************************************************************

#include "mrs_common.h"
#include "mrs_cmd_msg.h"
#include "mrs_fw_tools.h"
#include "mrs_fw_log.h"
#include "mrs_fw_n.h"
#include "mrs_srv_common.h"
#include "mrs_fw_proto645.h"
#include "mrs_srv_sta_queue.h"
#include "mrs_srv_parallel_sta.h"
#include "mrs_srv_sta_event.h"
#include "mrs_srv_sta.h"
#include "mrs_srv_res.h"
#include "mrs_srv_io.h"
#include "mrs_fw_nv.h"
#include "mrs_srv_cltii_event.h"
#include "mrs_srv_sta_searchmeter.h"
#include "mrs_srv_clti_searchmeter.h"
#include "mrs_srv_sta_meter.h"
#include "mrs_srv_collector.h"
#include "mrs_srv_csg_plc_sta.h"
#include "mrs_power_failure.h"
#if defined(PRODUCT_CFG_PRODUCT_TYPE_STA)

#define freePlcFrameData(x) \
    do \
    { \
        mrsFreePlcFrameDataStru(x); \
        x = HI_NULL; \
    } while (0)

#define MRS_CLTI_EVT_NUM_INVALID    (0xFF)
#define MRS_CLTI_EVT_NUM_MIN        (1)
#define MRS_CLTI_EVT_NUM_MAX        (33)
#define MRS_CLTI_EVT_QRY_NUM        (8)

#define MRS_645_CTRL_CLTI_EVT       (0x1E)
#define MRS_CLTI_EVT_CTRL_WORD_QRY  (0xC3)
#define MRS_CLTI_EVT_CTRL_WORD_CLR  (0xC5)

#define MRS_CLTI_EVENT_MSG_QRY      (0x01)


HI_PRV HI_U32 mrsStaEvtNvInit(HI_U16 usId);
HI_PRV HI_BOOL mrsStaEvtGetEvtOut(HI_VOID);
HI_PRV HI_BOOL mrsStaEvtSetHardEvtOut(HI_BOOL bEvtFlg);
HI_PRV HI_U32 mrsStaCreateEvtReportPlc(HI_IN HI_PBYTE pInBuf, HI_IN HI_U16 usInLen);
HI_PRV HI_U16 mrsStaEvtGetPlcSeq(HI_VOID);
HI_PRV HI_VOID mrsStaEvtPlcSeqAddSelf(HI_VOID);
HI_PRV HI_U32 mrsStaCreateQueryFrame(HI_U8 **ppFrame, HI_U16 *pusFrameLength);
HI_PRV HI_U32 mrsStaSendQueryFrame(HI_U16 usId, HI_U8 *pFrame, HI_U16 usFrameLength);
HI_PRV HI_U32 mrsStaSendQueryEvtStatusFrame(HI_VOID);
HI_PRV HI_U32 mrsStaEvtChlNotifyProc(HI_BOOL bChlStatus, HI_U32 ulOpt, HI_BOOL *bOneShot);
HI_PRV HI_U32 mrsStaEvtCompatibility(MRS_EVENT_INFO_STRU* pstEvtTransmit, HI_U16 usEvtDataLen);
HI_PRV HI_VOID mrsStaEvtSeqNvSave(P_MRS_STA_EVT_CTX pstEvtCtx);
HI_PRV HI_U8 mrsStaGetEvtReportTimeOut(P_MRS_STA_EVT_CTX pstCtx);
HI_PRV HI_VOID mrsStaEvtSetMonStatus(HI_U8 ucStatus);
HI_PRV HI_U8 mrsStaEvtGetMonStatus(HI_VOID);
HI_PRV HI_BOOL mrsStaEvtCheckNewEvent(HI_U8 *pData, HI_U8 ucLen);
HI_PRV HI_VOID mrsStaEvtClearLastData(MRS_STA_EVT_MONITOR_INF *pstStaMonInf);
HI_PRV HI_U32 mrsStaEvtSetData(MRS_STA_EVT_MONITOR_INF *pstStaMonInf, HI_U8 *pData, HI_U8 ucLen);
HI_PRV HI_U32 mrsStaEvtMonProc(P_MRS_STA_EVT_CTX pstCtx);
HI_PRV HI_VOID mrsStaEvtMonEndProc(HI_VOID);
HI_PRV HI_BOOL mrsStaEvtGetMonWaitPlcFlag(HI_VOID);
HI_PRV HI_U32 mrsStaEvtFilterDeny(HI_U8 ucSlaveFlag);
HI_PRV HI_VOID mrsStaEvtConsDenyTimesInc(HI_VOID);
HI_PRV HI_U8 mrsStaEvtGetConsDenyTimes(HI_VOID);
HI_PRV HI_U8 mrsStaEvtGetConsDenyMaxTimes(HI_VOID);
/* BEGIN: Modified by fengxiaomin/00209182, 2015/10/23   问题单号号:DTS2015102710230 */
HI_PRV HI_U32 mrsStaEvtFilterStatusWords(HI_U8 ucLen);
HI_PRV HI_U32 mrsStaEvtTypeNvUpdate(HI_U8 ucEvtType);
HI_PRV HI_U32 mrsStaEvtMeterChgNotify(HI_BOOL bChg, HI_U8 ucSlaveFlag, HI_U32 ulParam);
HI_PRV HI_U32 mrsStaEvtFilter(MRS_645_FRAME_STRU *pst645Hrd);
/* END:   Modified by fengxiaomin/00209182, 2015/10/23 */

#if !defined(PRODUCT_CFG_PRODUCT_TYPE_CSG_GD)
HI_PRV HI_U32 mrsCltiCreateQueryFrame(HI_U8 **ppFrame, HI_U16 *pusFrameLength);
HI_PRV HI_VOID mrsCltiEvtRegFrameProc(EVT_FRAME_RX pfnRx, EVT_FRAME_TIMEOUT pfnTimeout);
HI_PRV HI_U32 mrsCltiEventQueryFrameRx(HI_U8 *pFrame, HI_U16 usFrameLength);
HI_PRV HI_U32 mrsCltiEventQueryFrameTimeout(HI_VOID);
HI_PRV HI_U32 mrsCltiEvtCtxReset(HI_VOID);
HI_PRV HI_U32 mrsCltiRxPlcAckProc(HI_VOID);
HI_PRV HI_VOID mrsCltiEvtQueryDiscardProc(MRS_QUE_MODULE *pstQueue, HI_U16 usId);


HI_PRVL HI_VOID mrsCltiEvtSetStatus(HI_U8 ucStatus)
{
    MRS_STA_EVT_CTX *pstEvtCtx = mrsStaGetEvtCtx();
    pstEvtCtx->stCltiEvtInf.ucStatus = ucStatus;
}


HI_PRVL HI_U8 mrsCltiEvtGetStatus(HI_VOID)
{
    MRS_STA_EVT_CTX *pstEvtCtx = mrsStaGetEvtCtx();
    return pstEvtCtx->stCltiEvtInf.ucStatus;
}
#endif


// STA端事件上下文
HI_PRV MRS_STA_EVT_CTX g_stStaEvtCtx = {0};

P_MRS_STA_EVT_CTX mrsStaGetEvtCtx(HI_VOID)
{
    return &g_stStaEvtCtx;
}

HI_PRV HI_U32 mrsStaEvtNvInit(HI_U16 usId)
{
    P_MRS_STA_EVT_CTX pstCtx = mrsStaGetEvtCtx();
    NV_APP_MRS_EVT_CFG_STRU stNvCfg = {0};

    HI_UNREF_PARAM(usId);

    mrsSrvGetEvtNvCfg(&stNvCfg);

    //停电上报事件nv初始化
    mrsSetStaPowerFailureNvCtrl(&stNvCfg);

    HI_MDM_NM_PowerFailurReportEnable(&stNvCfg.ucPowerFailureReportEnable);

    pstCtx->ucCltiiEnable = stNvCfg.ucCltIIEnable;
    pstCtx->ucCltiEnable = stNvCfg.ucCltIEnable;
    pstCtx->ucStaEnable = stNvCfg.ucStaEvtEnable;
    pstCtx->ucTimeout = stNvCfg.ucStaTimeOut;
    pstCtx->ucMaxRetry = stNvCfg.ucStaMaxTry;
    pstCtx->ucTimeOutMax = stNvCfg.ucEvtTimeOutMax;
    pstCtx->usSeq = stNvCfg.usPlcSeq;
    pstCtx->stStaMonInf.ucMonitorInterval = stNvCfg.ucStaMonitorInterval;
    pstCtx->stFilterInf.ucConDentMaxTimes = stNvCfg.ucStaConsDenyTimes;

    mrsStaEvtSetMonStatus(MRS_STA_EVT_MONITOR_IDLE);
    mrsStaEvtSetMonWaitPlcFlag(HI_FALSE);
    mrsStaEvtSetMeterType(stNvCfg.ucMeterEvtType);
    mrsMeterChgNotifyProcReg(mrsStaEvtMeterChgNotify, 0);

#if defined(PRODUCT_CFG_PRODUCT_TYPE_CSG_GD)
    pstCtx->ucStaEnable = HI_TRUE;
#endif

    return HI_ERR_SUCCESS;
}


#if !defined(PRODUCT_CFG_PRODUCT_TYPE_CSG_GD)
HI_U32 mrsCltiEvtCtxReset(HI_VOID)
{
    P_MRS_STA_EVT_CTX pstCtx = mrsStaGetEvtCtx();

    mrsToolsFree(pstCtx->stCltiEvtInf.pucFrame);
    (hi_void)memset_s(&pstCtx->stCltiEvtInf, sizeof(pstCtx->stCltiEvtInf), 0, sizeof(pstCtx->stCltiEvtInf));
    pstCtx->stCltiEvtInf.ucTotal = MRS_CLTI_EVT_NUM_INVALID;

    return HI_ERR_SUCCESS;
}
#endif


HI_U32 mrsStaEvtCtxReset(HI_VOID)
{
    P_MRS_STA_EVT_CTX pstCtx = mrsStaGetEvtCtx();

    MRS_StopTimer(MRS_STA_TIMER_EVENT_REPORT);

    freePlcFrameData(pstCtx->pstReportData);

    pstCtx->ucRetry = 0;
    mrsDfxStaEvtRetry(pstCtx->ucRetry);

    mrsSrvChlNotifyProcClear(mrsStaEvtChlNotifyProc);

    return HI_ERR_SUCCESS;
}

HI_VOID mrsStaEvtSetMeterType(HI_U8 ucType)
{
/* BEGIN: Modified by fengxiaomin/00209182, 2015/10/23   问题单号号:DTS2015102710230 */
    mrsStaGetEvtCtx()->stFilterInf.ucMeterEvtType = ucType;
    mrsDfxStaMeterEvtType(ucType);
/* END:   Modified by fengxiaomin/00209182, 2015/10/23 */
}

HI_U8 mrsStaEvtGetMeterType(HI_VOID)
{
    return mrsStaGetEvtCtx()->stFilterInf.ucMeterEvtType;
}

HI_VOID mrsStaEvtConsDenyTimesReset(HI_VOID)
{
    mrsStaGetEvtCtx()->stFilterInf.ucStaConsDenyTimes = 0;
}

HI_VOID mrsStaEvtConsDenyTimesInc(HI_VOID)
{
    mrsStaGetEvtCtx()->stFilterInf.ucStaConsDenyTimes++;
}

HI_U8 mrsStaEvtGetConsDenyTimes(HI_VOID)
{
    return mrsStaGetEvtCtx()->stFilterInf.ucStaConsDenyTimes;
}

HI_U8 mrsStaEvtGetConsDenyMaxTimes(HI_VOID)
{
    return mrsStaGetEvtCtx()->stFilterInf.ucConDentMaxTimes;
}

/* BEGIN: Added by fengxiaomin/00209182, 2015/10/23   问题单号号:DTS2015102710230 */
//*****************************************************************************
// 函数名称: mrsStaEvtMeterChgNotify
// 功能描述: STA接收到电表地址应答后，通知事件处理
//
// 参数说明:
//   bChg   表地址改变标志
//   ucSlaveFlag  否认帧标志
//   ulParam 参数
//
// 返 回 值:
//    HI_ERR_SUCCESS:  成功
//
// 调用要求: TODO: ...
// 调用举例: TODO: ...
// 作    者: fengxiaomin/00209182 [2015-10-23]
//*****************************************************************************
HI_U32 mrsStaEvtMeterChgNotify(HI_BOOL bChg, HI_U8 ucSlaveFlag, HI_U32 ulParam)
{
    MRS_NOT_USED(ucSlaveFlag);
    MRS_NOT_USED(ulParam);

    if (bChg)
    {
        mrsStaEvtTypeNvUpdate(MRS_STA_EVT_METER_TYPE_UNKNOWN);
        mrsStaEvtSetMeterType(MRS_STA_EVT_METER_TYPE_UNKNOWN);
    }

    mrsStaEvtOutProc();

    return HI_ERR_SUCCESS;
}

//*****************************************************************************
// 函数名称: mrsStaEvtTypeNvUpdate
// 功能描述: 更新NV中的电表事件类型
//
// 参数说明:
//   ucEvtType   电表事件类型
//
// 返 回 值:
//    HI_ERR_SUCCESS:  成功
//
// 调用要求: TODO: ...
// 调用举例: TODO: ...
// 作    者: fengxiaomin/00209182 [2015-10-23]
//*****************************************************************************
HI_U32 mrsStaEvtTypeNvUpdate(HI_U8 ucEvtType)
{
    HI_U32 ulRet = HI_ERR_SUCCESS;
    NV_APP_MRS_EVT_CFG_STRU stEvtNv;

    (hi_void)memset_s(&stEvtNv, sizeof(stEvtNv), 0, sizeof(stEvtNv));
    ulRet = HI_MDM_NV_Read(ID_NV_APP_EVENT_CFG, (HI_PVOID)&stEvtNv, sizeof(NV_APP_MRS_EVT_CFG_STRU));
    if(HI_ERR_SUCCESS != ulRet)
    {
        return ulRet;
    }

    if (stEvtNv.ucMeterEvtType != ucEvtType)
    {
        stEvtNv.ucMeterEvtType = ucEvtType;
        HI_MDM_NV_Write(ID_NV_APP_EVENT_CFG, (HI_PVOID)&stEvtNv, sizeof(NV_APP_MRS_EVT_CFG_STRU));
    }

    return HI_ERR_SUCCESS;
}
/* END:   Added by fengxiaomin/00209182, 2015/10/23 */


// 设置事件管脚状态
HI_BOOL mrsStaEvtSetEvtOut(HI_BOOL bEvtFlg)
{
    P_MRS_STA_EVT_CTX pstCtx = mrsStaGetEvtCtx();

    pstCtx->bEvtOut = bEvtFlg;
    mrsDfxStaCurrEvtPinStat(bEvtFlg);

    if ((HI_FALSE == pstCtx->bEvtOut) && (MRS_STA_EVT_MONITOR_DOING == mrsStaEvtGetMonStatus()))
    {
        HI_DIAG_LOG_MSG_E0(MRS_FILE_LOG_FLAG_008, HI_DIAG_MT("evt pin low mon end"));
        mrsStaEvtMonEndProc();
    }

    return bEvtFlg;
}

// 获取事件管脚状态
HI_BOOL mrsStaEvtGetEvtOut(HI_VOID)
{
    return mrsStaGetEvtCtx()->bEvtOut;
}


// 设置事件管脚状态(硬件EvtOut)
HI_BOOL mrsStaEvtSetHardEvtOut(HI_BOOL bEvtFlg)
{
    P_MRS_STA_EVT_CTX pstCtx = mrsStaGetEvtCtx();

    pstCtx->bHardEvtOut = bEvtFlg;
    mrsDfxStaHardEvtPinStat(bEvtFlg);

    return bEvtFlg;
}

// 获取PLC帧序号
HI_PRV HI_U16 mrsStaEvtGetPlcSeq(HI_VOID)
{
    return mrsStaGetEvtCtx()->usSeq;
}

// PLC帧序号自增
HI_PRV HI_VOID mrsStaEvtPlcSeqAddSelf(HI_VOID)
{
    mrsStaGetEvtCtx()->usSeq++;
    mrsDfxStaEvtReportSeq(mrsStaGetEvtCtx()->usSeq);
    return;
}

HI_U32 mrsStaCreateQueryFrame(HI_U8 **ppFrame, HI_U16 *pusFrameLength)
{
    MRS_STA_SRV_CTX_STRU * pstStaCtx = mrsStaGetContext();
    MRS_METER_ITEM_S * pstMeterInf = &pstStaCtx->stMeterList.stMeterItem[0];
    MRS_PROTO645_FRAME_INF stFrameInf;

    (hi_void)memset_s(&stFrameInf, sizeof(stFrameInf), 0, sizeof(stFrameInf));

    if (pstMeterInf->ucValidFlag)
    {
        (hi_void)memcpy_s(stFrameInf.aucAddr, HI_METER_ADDR_LEN, pstMeterInf->ucMeterAddr, sizeof(stFrameInf.aucAddr));
    }
    else
    {
        (hi_void)memset_s(stFrameInf.aucAddr, HI_METER_ADDR_LEN, MRS_645_METER_ADDR_WILDCARDS, sizeof(stFrameInf.aucAddr));
    }

    stFrameInf.ucCtrl = MRS_645_FRAME_CONTROL_READ_DATA;
    stFrameInf.ucDiSize = MRS_645_FRAME_DATA_DI_SIZE;
    stFrameInf.ulDi = 0x04001501;

    return mrsGen645Frame(&stFrameInf, ppFrame, pusFrameLength);
}

#if !defined(PRODUCT_CFG_PRODUCT_TYPE_CSG_GD)
HI_U32 mrsCltiCreateQueryFrame(HI_U8 **ppFrame, HI_U16 *pusFrameLength)
{
    MRS_STA_EVT_CTX * pstEvtCtx = mrsStaGetEvtCtx();
    MRS_PROTO645_FRAME_INF stFrameInf;
    HI_U8 *pucCltAddr = mrsGetCollectorAddr();
    HI_U8 *pucPayload = HI_NULL;
    HI_U8 ucPayloadLen = 3; // 数据单元标识1B + SS(1B) + NN(1B)
    HI_U32 ulRet = HI_ERR_SUCCESS;

    if (pstEvtCtx->stCltiEvtInf.ucQuery + 1 > pstEvtCtx->stCltiEvtInf.ucTotal)
    {
        return HI_ERR_FAILURE;
    }

    pucPayload = mrsToolsMalloc(ucPayloadLen);
    if (!pucPayload)
    {
        return HI_ERR_NOT_ENOUGH_MEMORY;
    }

    (hi_void)memset_s(pucPayload, ucPayloadLen, 0, ucPayloadLen);
    pucPayload[0] = MRS_CLTI_EVT_CTRL_WORD_QRY;
    pucPayload[1] = pstEvtCtx->stCltiEvtInf.ucQuery + 1;
    pucPayload[2] = HI_MIN(pstEvtCtx->stCltiEvtInf.ucTotal - pstEvtCtx->stCltiEvtInf.ucQuery, MRS_CLTI_EVT_QRY_NUM);

    (hi_void)memset_s(&stFrameInf, sizeof(stFrameInf), 0, sizeof(stFrameInf));
    if (mrsToolsCheckBCD(pucCltAddr, HI_METER_ADDR_LEN))
    {
        (hi_void)memcpy_s(stFrameInf.aucAddr, HI_METER_ADDR_LEN, pucCltAddr, sizeof(stFrameInf.aucAddr));
    }
    else
    {
        (hi_void)memset_s(stFrameInf.aucAddr, HI_METER_ADDR_LEN, MRS_645_METER_ADDR_WILDCARDS, sizeof(stFrameInf.aucAddr));
    }

    stFrameInf.ucCtrl = MRS_645_CTRL_CLTI_EVT;
    stFrameInf.ucDataLength = ucPayloadLen;
    stFrameInf.pData = pucPayload;

    ulRet = mrsGen645Frame(&stFrameInf, ppFrame, pusFrameLength);
    mrsToolsFree(pucPayload);

    return ulRet;
}
#endif

HI_U32 mrsStaSendQueryFrame(HI_U16 usId, HI_U8 *pFrame, HI_U16 usFrameLength)
{
    MRS_STA_SRV_CTX_STRU * pstStaCtx = mrsStaGetContext();
    MRS_STA_ITEM * pItem = HI_NULL;

    HI_U8 *pucPayload = HI_NULL;
    HI_U16 usPayloadLen = sizeof(MRS_STA_ITEM) + usFrameLength;

    pucPayload = mrsToolsMalloc(usPayloadLen);
    if (!pucPayload)
    {
        return HI_ERR_NOT_ENOUGH_MEMORY;
    }

    (hi_void)memset_s(pucPayload, usPayloadLen, 0, usPayloadLen);
    pItem = (MRS_STA_ITEM *)pucPayload;
    pItem->buf = pucPayload + sizeof(MRS_STA_ITEM);
    pItem->len = usFrameLength;
    pItem->from = MRS_STA_QUEUE_FROM_STA;
    pItem->id = usId;
    pItem->time_out = (HI_U8)MRS_MS_TO_100MS(mrsGet645FrameHeadTimeout(MRS_STA_RM_CFG_LOCAL));
    pItem->bAutoStrategy = HI_TRUE;
    pItem->ucFrameTimeout = (HI_U8)MRS_MS_TO_100MS(mrsGet645FrameInterval(MRS_STA_RM_CFG_LOCAL));
    (hi_void)memcpy_s(pItem->buf, usFrameLength, pFrame, usFrameLength);

    if (!mrsStaTryEnQueue(&pstStaCtx->stQueModule, pItem))
    {
        mrsToolsFree(pItem);
    }
    else
    {
        mrsDfxStaMeterEvtQueryCnt();
    }

    //通知队列
    mrsStaNotifyQueue();

    return HI_ERR_SUCCESS;
}

// 发送查询"事件状态"帧(645-04001503)
HI_U32 mrsStaSendQueryEvtStatusFrame(HI_VOID)
{
    HI_U32 ulRet = HI_ERR_SUCCESS;
    HI_U8 *pFrame = HI_NULL;
    HI_U16 usLength = 0;

    do
    {
        HI_U16 usId = MRS_STA_ITEM_ID_EVENT;

#if !defined(PRODUCT_CFG_PRODUCT_TYPE_CSG_GD)
        if (mrsIsRealClti())
        {
            ulRet = mrsCltiCreateQueryFrame(&pFrame, &usLength);
            usId = MRS_STA_ITEM_ID_CLTI_EVENT;
            mrsCltiEvtRegFrameProc(mrsCltiEventQueryFrameRx, mrsCltiEventQueryFrameTimeout);
        }
        else
#endif
        {
            ulRet = mrsStaCreateQueryFrame(&pFrame, &usLength);
            usId = MRS_STA_ITEM_ID_EVENT;
        }

        if (HI_ERR_SUCCESS != ulRet)
        {
            break;
        }

        ulRet = mrsStaSendQueryFrame(usId, pFrame, usLength);
        mrsToolsFree(pFrame);
        if (HI_ERR_SUCCESS != ulRet)
        {
            break;
        }

#if !defined(PRODUCT_CFG_PRODUCT_TYPE_CSG_GD)
        mrsCltiEvtSetStatus(MRS_CLTI_EVT_STATUS_WAIT_QUERY_REPLY);
#endif

        return ulRet;
    } while (0);

#if !defined(PRODUCT_CFG_PRODUCT_TYPE_CSG_GD)
    mrsCltiEvtRegFrameProc(HI_NULL, HI_NULL);
#endif

    return ulRet;
}

// 创建事件上报PLC帧
HI_PRV HI_U32 mrsStaCreateEvtReportPlc(HI_IN HI_PBYTE pInBuf, HI_IN HI_U16 usInLen)
{
    HI_U32 ret = HI_ERR_SUCCESS;
    P_MRS_STA_EVT_CTX pstCtx = mrsStaGetEvtCtx();
    MRS_PLC_FRAME_DATA_S *pstPlcFrame = HI_NULL;
#if defined(PRODUCT_CFG_PRODUCT_TYPE_CSG_GD)
    MRS_STA_SRV_CTX_STRU *pstStaCtx = mrsStaGetContext();
    MRS_CSG_PLC_FRAME_HEAD *pstFrameHead = HI_NULL;
#else
    MRS_EVENT_INFO_STRU *pstEvtInfo = HI_NULL;
#endif
    HI_PBYTE pucPayload = HI_NULL;
    HI_U16 usPayloadLen;
    HI_U16 usPos = 0;
    HI_U16 usFrameLen = 0;

    ret = mrsFind645Frame(pInBuf, (HI_S16)usInLen, &usPos, &usFrameLen);
    if(HI_ERR_SUCCESS != ret)
    {
        return ret;
    }

    pstPlcFrame = (MRS_PLC_FRAME_DATA_S *)mrsToolsMalloc(sizeof(MRS_PLC_FRAME_DATA_S));
    if (!pstPlcFrame)
    {
        HI_DIAG_LOG_MSG_E0(MRS_FILE_LOG_FLAG_002, HI_DIAG_MT("Malloc failed"));
        return HI_ERR_MALLOC_FAILUE;
    }

    (hi_void)memset_s(pstPlcFrame, sizeof(MRS_PLC_FRAME_DATA_S), 0, sizeof(MRS_PLC_FRAME_DATA_S));

#if defined(PRODUCT_CFG_PRODUCT_TYPE_CSG_GD)
    usPayloadLen = sizeof(MRS_CSG_PLC_FRAME_HEAD) + usInLen + HI_METER_ADDR_LEN;
#else
    usPayloadLen = sizeof(MRS_EVENT_INFO_STRU) + usInLen;
#endif

    pucPayload = mrsToolsMalloc(usPayloadLen);
    if (!pucPayload)
    {
        HI_DIAG_LOG_MSG_E0(MRS_FILE_LOG_FLAG_003, HI_DIAG_MT("Malloc failed"));
        mrsToolsFree(pstPlcFrame);
        return HI_ERR_MALLOC_FAILUE;
    }

    (hi_void)memset_s(pucPayload, usPayloadLen, 0, usPayloadLen);

#if defined(PRODUCT_CFG_PRODUCT_TYPE_CSG_GD)
    mrsStaEvtPlcSeqAddSelf();
    pstCtx->usSeq = mrsStaEvtGetPlcSeq();
    pstFrameHead = (MRS_CSG_PLC_FRAME_HEAD *)pucPayload;
    pstFrameHead->usFrameType = MRS_CSG_PLC_CTRL_TYPE_REPROT;
    pstFrameHead->usExtendBit = MRS_CSG_PLC_CTRL_EXTEND_BIT_N;
    pstFrameHead->usRespBit = MRS_CSG_PLC_CTRL_RESP_BIT_Y;
    pstFrameHead->usPrm = MRS_CSG_PLC_CTRL_PRM_HOST;
    pstFrameHead->usDir = MRS_CSG_PLC_CTRL_DIR_UL;
    pstFrameHead->ucSrvId = MRS_CSG_PLC_SRV_ID_REPORT_EVENT;
    pstFrameHead->ucVersion = MRS_CSG_PLC_PROTO_VERSION;
    pstFrameHead->usSeq = pstCtx->usSeq;
    pstFrameHead->usDataLength = HI_METER_ADDR_LEN + usInLen;
    (hi_void)memcpy_s(pstFrameHead->aucData, HI_METER_ADDR_LEN, pInBuf + usPos + MRS_645_FRAME_METERADD_OFFSET, HI_METER_ADDR_LEN);
    (hi_void)memcpy_s(pstFrameHead->aucData + HI_METER_ADDR_LEN, usInLen, pInBuf, usInLen);

    pstPlcFrame->usId = ID_MRS_CMD_CSG_DATA_TRANSMIT_PLC;

    pstStaCtx->pfnPlcAckProc = mrsStaEvtPlcAck;
    pstStaCtx->pfnPlcDenyProc = mrsStaEvtPlcDeny;
#else
    pstEvtInfo = (MRS_EVENT_INFO_STRU *)pucPayload;
    pstEvtInfo->ulInterfaceVer = MRS_EVENT_INF_UP_VER;
    pstEvtInfo->ulStructLen = sizeof(MRS_EVENT_INFO_STRU);
    pstEvtInfo->ulDirBit = MRS_PLC_UP_FLG;
    pstEvtInfo->ulFrmBit = MRS_EVENT_FRM_BIT_FROM;
    pstEvtInfo->ulFunCode = MRS_EVENT_FUN_REPORT;
    pstEvtInfo->ulDataLen = usInLen;

    mrsStaEvtPlcSeqAddSelf();
    pstEvtInfo->usSeq = mrsStaEvtGetPlcSeq();

    (hi_void)memcpy_s(pstEvtInfo->ucMeter, HI_METER_ADDR_LEN, pInBuf + usPos + MRS_645_FRAME_METERADD_OFFSET, HI_METER_ADDR_LEN);
    (hi_void)memcpy_s(pstEvtInfo->ucData, usInLen, pInBuf, usInLen);

    pstPlcFrame->usId = ID_MRS_CMD_EVENT_INF_PLC;
#endif

    pstPlcFrame->pucPayload = pucPayload;
    pstPlcFrame->usPayloadLen = usPayloadLen;

    pstCtx->pstReportData = pstPlcFrame;

    return HI_ERR_SUCCESS;
}


// 通道变更通知-事件处理
HI_PRV HI_U32 mrsStaEvtChlNotifyProc(HI_BOOL bChlStatus, HI_U32 ulOpt, HI_BOOL *bOneShot)
{
    HI_U32 ulRet = HI_ERR_SUCCESS;

    if(bChlStatus == HI_TRUE)
    {
        switch(ulOpt)
        {
        case MRS_STA_EVT_STATUS_QUERY:
            mrsStaSendQueryEvtStatusFrame();
            break;

        case MRS_STA_EVT_STATUS_REPORT:
            ulRet = mrsStaSendEvtReportPlc();
            if (HI_ERR_SUCCESS == ulRet)
            {
                mrsStaEvtSetMonWaitPlcFlag(HI_TRUE);
            }
            break;

        default:
            break;
        }

        *bOneShot= HI_TRUE;
        mrsDfxStaEvtWaitProcStat(MRS_STA_EVT_STATUS_IDLE);
    }
    else
    {
        *bOneShot= HI_FALSE;
    }

    return HI_ERR_SUCCESS;
}


HI_U32 mrsStaEvtCtxInit(HI_VOID)
{
    P_MRS_STA_EVT_CTX pstCtx = mrsStaGetEvtCtx();

    (hi_void)memset_s(pstCtx, sizeof(MRS_STA_EVT_CTX), 0, sizeof(MRS_STA_EVT_CTX));

#if !defined(PRODUCT_CFG_PRODUCT_TYPE_CSG_GD)
    pstCtx->stCltiEvtInf.ucTotal = MRS_CLTI_EVT_NUM_INVALID;
#endif

    mrsStaEvtNvInit(ID_NV_APP_EVENT_CFG);

    return HI_ERR_SUCCESS;
}


// 事件管脚状态处理
HI_VOID mrsStaEvtOutProc(HI_VOID)
{
    P_MRS_STA_EVT_CTX pstCtx = mrsStaGetEvtCtx();
    HI_U8 ucMeterEvtType = mrsStaEvtGetMeterType();

#if !defined(PRODUCT_CFG_PRODUCT_TYPE_CSG_GD)
    if (mrsIsRealClti())
    {
        // I采模块事件开关未打开
        if (!pstCtx->ucCltiEnable)
        {
            return;
        }
    }
    else
#endif
    if( (pstCtx->ucStaEnable == HI_FALSE)
        || (MRS_STA_EVT_METER_TYPE_13_OLD == ucMeterEvtType)
        || (MRS_STA_EVT_METER_TYPE_09 == ucMeterEvtType))
    {
        // STA事件开关未打开 or 旧表不监控事件
        return;
    }

    if(mrsStaEvtGetEvtOut())
    {
        mrsStaEvtCtxReset();
#if !defined(PRODUCT_CFG_PRODUCT_TYPE_CSG_GD)
        mrsCltiEvtCtxReset();
#endif
        mrsStaEvtSetMonWaitPlcFlag(HI_FALSE);
        if(HI_TRUE == mrsStaGetPlcChlStatus())
        {
            mrsStaSendQueryEvtStatusFrame();
        }
        else
        {
            mrsSrvChlNotifyProcReg(mrsStaEvtChlNotifyProc, MRS_STA_EVT_STATUS_QUERY);
            mrsDfxStaEvtWaitProcStat(MRS_STA_EVT_STATUS_QUERY);
        }
    }
#if !defined(PRODUCT_CFG_PRODUCT_TYPE_CSG_GD)
    else if (mrsIsRealClti())
    {
        MRS_STA_SRV_CTX_STRU *pstStaCtx = mrsStaGetContext();

		//将STA队列中已有的I采查询命令item的子id赋值为废弃查询命令
		mrsCltiEvtQueryDiscardProc(&pstStaCtx->stQueModule, MRS_STA_ITEM_ID_CLTI_EVENT);
        mrsStaEvtCtxReset();
        mrsCltiEvtCtxReset();
        mrsStaEvtSetMonWaitPlcFlag(HI_FALSE);
    }
#endif

    return;
}


// EVENT_OUT管脚状态处理
HI_U32 mrsStaEvtOutStatusNotify(HI_U32 ulParam, HI_BOOL bEvtOut)
{
    HI_U32 ulDelay = 0;

    if(MRS_STA_EVT_FROM_METER == ulParam)
    {
        mrsStaEvtSetHardEvtOut(bEvtOut);

        if(NV_SIMU_METER_ENABLE == mrsStaGetSupportVM())
        {
            return HI_ERR_SUCCESS;
        }
    }
    else
    {
        if(NV_SIMU_METER_DISABLE == mrsStaGetSupportVM())
        {
            return HI_ERR_SUCCESS;
        }

        ulDelay = mrsStaGetVMActiveDelay();
    }

    mrsStaEvtSetEvtOut(bEvtOut);

    if(ulDelay)
    {
        MRS_StartTimer(MRS_STA_TIMER_VM_EVT_ACTIVE,
                        ulDelay * 1000,
                        HI_SYS_TIMER_ONESHOT);

        return HI_ERR_SUCCESS;
    }

    if (
#if !defined(PRODUCT_CFG_PRODUCT_TYPE_CSG_GD)
        mrsIsRealClti() ||
#endif
        (STA_TYPE_METER == mrsStaGetType()))
    {
        mrsStaEvtOutProc();

        return HI_ERR_SUCCESS;
    }

    return HI_ERR_NOT_SUPPORT;
}

// 事件主动上报处理
HI_U32 mrsStaEvtReportProc(HI_U8 *pucData, HI_U16 usLen)
{
    HI_U32 ret = HI_ERR_SUCCESS;

    //检查参数
    if (!pucData)
    {
        return HI_ERR_FAILURE;
    }

    if (!mrsStaGetEvtCtx()->ucStaEnable)
    {
        return ret;
    }

    ret = mrsStaCreateEvtReportPlc(pucData, usLen);
    if(ret != HI_ERR_SUCCESS)
    {
        return ret;
    }

    ret = mrsStaSendEvtReportPlc();

    return ret;
}

// 发送事件上报PLC帧
HI_U32 mrsStaSendEvtReportPlc(HI_VOID)
{
    HI_U32 ret = HI_ERR_SUCCESS;
    P_MRS_STA_EVT_CTX pstCtx = mrsStaGetEvtCtx();

    if (!pstCtx->pstReportData)
    {
        return HI_ERR_BAD_DATA;
    }

    do
    {
        if(HI_FALSE == mrsStaGetPlcChlStatus())
        {
            ret = HI_ERR_NOT_CONNECT;
            break;
        }

        if (pstCtx->ucRetry >= pstCtx->ucMaxRetry)
        {
            // 等待重新组网后再次上报
            pstCtx->ucRetry = 0;
            ret = HI_ERR_NOT_CONNECT;
            break;
        }

        ret = MRS_SendPlcFrame(pstCtx->pstReportData);
        if (HI_ERR_SUCCESS == ret)
        {
            HI_U8 ucTimeOut = 0;

            ucTimeOut = mrsStaGetEvtReportTimeOut(pstCtx);
            pstCtx->ucRetry++;

            mrsDfxStaEvtRetry(pstCtx->ucRetry);
            mrsDfxStaPlcEvtReportTotalCnt();
            mrsDfxStaPlcEvtReportTimeStamp(HI_MDM_GetMilliSeconds());

            MRS_StartTimer(MRS_STA_TIMER_EVENT_REPORT,
                            ucTimeOut * 1000,
                            HI_SYS_TIMER_ONESHOT);

            ret = HI_ERR_SUCCESS;
        }

    }while(0);

    if(ret == HI_ERR_NOT_CONNECT)
    {
        mrsSrvChlNotifyProcReg(mrsStaEvtChlNotifyProc, MRS_STA_EVT_STATUS_REPORT);
        mrsDfxStaEvtWaitProcStat(MRS_STA_EVT_STATUS_REPORT);
        mrsStaEvtSetMonWaitPlcFlag(HI_FALSE);
    }

    return ret;
}

HI_U8 mrsStaGetEvtReportTimeOut(P_MRS_STA_EVT_CTX pstCtx)
{
    HI_U8 ucTimeOut = (HI_U8)(pstCtx->ucRetry * MRS_STA_EVT_REPORT_TIMEOUT_DEFAULT);

    if (!ucTimeOut)
    {
        return MRS_STA_EVT_REPORT_TIMEOUT_DEFAULT;
    }

    ucTimeOut = (ucTimeOut > pstCtx->ucTimeOutMax) ? pstCtx->ucTimeOutMax : ucTimeOut;

    return ucTimeOut;
}

// 事件上报PLC帧超时处理
HI_U32 mrsStaEvtReportPlcTimeoutProc(HI_VOID)
{
    HI_U32 ret = HI_ERR_SUCCESS;

    mrsDfxStaPlcEvtReportTimeOutCnt();
    ret = mrsStaSendEvtReportPlc();

    return ret;
}

// STA端事件PLC帧兼容性处理
HI_U32 mrsStaEvtCompatibility(MRS_EVENT_INFO_STRU* pstEvtTransmit, HI_U16 usEvtDataLen)
{
    HI_U32 ulRet = HI_ERR_SUCCESS;
    HI_U16 usLocalLen = 0;

    if (pstEvtTransmit->ulDirBit != MRS_PLC_DL_FLG)
    {
        return HI_ERR_BAD_FRAME;
    }

    usLocalLen = (HI_U16)(sizeof(MRS_EVENT_INFO_STRU) + pstEvtTransmit->ulDataLen);
    if (usEvtDataLen != (pstEvtTransmit->ulStructLen + pstEvtTransmit->ulDataLen))
    {
        return HI_ERR_BAD_DATA;
    }

    switch (pstEvtTransmit->ulInterfaceVer)
    {
    case MRS_EVENT_INF_DL_VER_R0:
    default:
        if (usLocalLen < usEvtDataLen)
        {
            return HI_ERR_FAILURE;
        }

        break;
    }

    return ulRet;
}

// 事件PLC命令帧接收处理
HI_U32 mrsCmdStaEvtInfProc(HI_IN HI_U16 usId, HI_PVOID pReqPacket, HI_U16 usReqPacketSize)
{
    HI_U32 ret = HI_ERR_SUCCESS;
    P_MRS_STA_EVT_CTX pstCtx = mrsStaGetEvtCtx();
    MRS_PLC_FRAME_DATA_STRU *pstReq = (MRS_PLC_FRAME_DATA_STRU*)pReqPacket;
    MRS_EVENT_INFO_STRU *pstEvtInf = HI_NULL;
    HI_U32 ulCurrTime = HI_MDM_GetMilliSeconds();

    HI_UNREF_PARAM(usId);
    HI_UNREF_PARAM(usReqPacketSize);

    if (!pstReq || !pstReq->pucPayload)
    {
        return HI_ERR_FAILURE;
    }

    pstEvtInf = (MRS_EVENT_INFO_STRU *)((HI_U8 *)pstReq->pucPayload);

    ret = mrsStaEvtCompatibility(pstEvtInf, pstReq->usPayloadLen);
    if(ret != HI_ERR_SUCCESS)
    {
        return ret;
    }

    if(pstEvtInf->usSeq != pstCtx->usSeq)
    {
        return HI_ERR_FAILURE;
    }

    switch(pstEvtInf->ulFunCode)
    {
    case MRS_EVENT_FUN_ACK:
        mrsDfxStaPlcEvtAckCnt();
        mrsDfxStaPlcEvtAckTimeStamp(ulCurrTime);
        mrsStaEvtCtxReset();
        mrsStaEvtSeqNvSave(pstCtx);

#if !defined(PRODUCT_CFG_PRODUCT_TYPE_CSG_GD)
        if (mrsToolsIsIICollector())
        {
            mrsCltiiEventInfoRxAckSaveNv();
            mrsCltiiEventReport();
        }
        else if (mrsIsRealClti())
        {
        	mrsCltiRxPlcAckProc();
        }
        else
#endif
        {
            mrsStaEvtMonProc(pstCtx);
        }
        break;

    case MRS_EVENT_FUN_CCOBUF_FULL:
        pstCtx->ucRetry = 0;
        mrsDfxStaEvtRetry(pstCtx->ucRetry);
        mrsDfxStaPlcEvtFullCnt();
        mrsDfxStaPlcEvtFullTimeStamp(ulCurrTime);

#if !defined(PRODUCT_CFG_PRODUCT_TYPE_CSG_GD)
        if (mrsToolsIsIICollector() && (MRS_CLTII_EVENT_MONITOR_WORKING == mrsCltiiEventGetMonitorStatus()))
        {
            mrsStaEvtCtxReset();
            break;
        }
        else
#endif
        {
            MRS_StartTimer(MRS_STA_TIMER_EVENT_REPORT, pstCtx->ucTimeOutMax * 1000, HI_SYS_TIMER_ONESHOT);
        }
        break;

    default:
        break;
    }

    return ret;
}

HI_VOID mrsStaEvtSeqNvSave(P_MRS_STA_EVT_CTX pstEvtCtx)
{
    HI_U32 ulRet = HI_ERR_SUCCESS;
    NV_APP_MRS_EVT_CFG_STRU stEvtNvCfg = {0};

    ulRet = HI_MDM_NV_Read(ID_NV_APP_EVENT_CFG,
                            (HI_PVOID)&stEvtNvCfg,
                            sizeof(NV_APP_MRS_EVT_CFG_STRU));
    if (HI_ERR_SUCCESS != ulRet)
    {
        return;
    }

    stEvtNvCfg.usPlcSeq = pstEvtCtx->usSeq;
    HI_MDM_NV_Write(ID_NV_APP_EVENT_CFG,
                    (HI_PVOID)&stEvtNvCfg,
                    sizeof(NV_APP_MRS_EVT_CFG_STRU));
}

//*****************************************************************************
// 函数名称: mrsStaEvtFilterDeny
// 功能描述: STA接收到电表应答的否认帧后的过滤处理
//
// 参数说明:
//   ucSlaveFlag   645帧否认标志
//
// 返 回 值:
//    HI_ERR_SUCCESS:  过滤
//    HI_ERR_CONTINUE: 不过滤
//
// 调用要求: TODO: ...
// 调用举例: TODO: ...
// 作    者: fengxiaomin/00209182 [2015-10-09]
//*****************************************************************************
HI_U32 mrsStaEvtFilterDeny(HI_U8 ucSlaveFlag)
{
    HI_U32 ulRet = HI_ERR_SUCCESS;

    if (!ucSlaveFlag)
    {
        mrsStaEvtConsDenyTimesReset();
        return HI_ERR_CONTINUE;
    }

    switch (mrsStaEvtGetMeterType())
    {
/* BEGIN: Modified by fengxiaomin/00209182, 2015/10/23   问题单号号:DTS2015102710230 */
        case MRS_STA_EVT_METER_TYPE_13_OLD:
        case MRS_STA_EVT_METER_TYPE_13_NEW:
            mrsStaEvtConsDenyTimesReset();
            ulRet = HI_ERR_CONTINUE;
            break;

        case MRS_STA_EVT_METER_TYPE_UNKNOWN:
            mrsStaEvtConsDenyTimesInc();

            if (mrsStaEvtGetConsDenyTimes() >= mrsStaEvtGetConsDenyMaxTimes())
            {
                mrsStaEvtSetMeterType(MRS_STA_EVT_METER_TYPE_09);
            }
            break;

        case MRS_STA_EVT_METER_TYPE_09:
/* END:   Modified by fengxiaomin/00209182, 2015/10/23 */
        default :
            break;
    }

    return ulRet;
}


/* BEGIN: Added by fengxiaomin/00209182, 2015/10/23   问题单号号:DTS2015102710230 */
//*****************************************************************************
// 函数名称: mrsStaEvtFilterStatusWords
// 功能描述: STA接收到电表应答的事件主动上报状态字后的13规范非新事件表过滤
//
// 参数说明:
//   usLen      645帧数据域长度
//
// 返 回 值:
//    HI_ERR_SUCCESS: 需要过滤
//    HI_ERR_CONTINUE:不过滤
//
// 调用要求: TODO: ...
// 调用举例: TODO: ...
// 作    者: fengxiaomin/00209182 [2015-10-22]
//*****************************************************************************
HI_U32 mrsStaEvtFilterStatusWords(HI_U8 ucLen)
{
    HI_U8 ucDataLenMin = MRS_645_FRAME_DATA_DI_SIZE + MRS_STA_EVT_STATUS_WORD_LEN_MIN;

    if (ucLen >= ucDataLenMin)
    {
        return HI_ERR_CONTINUE;
    }

    if (MRS_STA_EVT_METER_TYPE_13_OLD != mrsStaEvtGetMeterType())
    {
        mrsStaEvtSetMeterType(MRS_STA_EVT_METER_TYPE_13_OLD);
        mrsStaEvtTypeNvUpdate(MRS_STA_EVT_METER_TYPE_13_OLD);
    }

    return HI_ERR_SUCCESS;
}

//*****************************************************************************
// 函数名称: mrsStaEvtFilter
// 功能描述: STA接收到电表应答的事件主动上报状态字后的过滤处理
//
// 参数说明:
//   pst645Hrd  645帧解析后的信息
//
// 返 回 值:
//    HI_ERR_SUCCESS: 需要过滤
//    HI_ERR_CONTINUE:不过滤
//
// 调用要求: TODO: ...
// 调用举例: TODO: ...
// 作    者: fengxiaomin/00209182 [2015-10-22]
//*****************************************************************************
HI_U32 mrsStaEvtFilter(MRS_645_FRAME_STRU *pst645Hrd)
{
    HI_U32 ulRet = HI_ERR_SUCCESS;

    if (pst645Hrd->stCtrl.ucSlaveFlag)
    {
        mrsDfxStaEvtRespDenyCnt();
    }

    if (MRS_STA_EVT_METER_TYPE_13_NEW == mrsStaEvtGetMeterType())
    {
        return HI_ERR_CONTINUE;
    }

    ulRet = mrsStaEvtFilterDeny(pst645Hrd->stCtrl.ucSlaveFlag);
    if (HI_ERR_CONTINUE != ulRet)
    {
        return ulRet;
    }

    ulRet = mrsStaEvtFilterStatusWords(pst645Hrd->ucDataRealmLen);
    if (HI_ERR_CONTINUE != ulRet)
    {
        return ulRet;
    }

    if (MRS_STA_EVT_METER_TYPE_13_NEW != mrsStaEvtGetMeterType())
    {
        mrsStaEvtSetMeterType(MRS_STA_EVT_METER_TYPE_13_NEW);
    }

    return HI_ERR_CONTINUE;
}
/* END:   Added by fengxiaomin/00209182, 2015/10/23 */

//*****************************************************************************
// 函数名称: mrsStaEvtProc
// 功能描述: STA接收到电表应答的事件主动上报状态字后的处理
//
// 参数说明:
//   pData      645帧报文
//   usLen      645帧报文长度
//   pst645Hrd  645帧解析后的信息
//
// 返 回 值:
//    HI_ERR_SUCCESS: 成功
//    其它错误码
//
// 调用要求: TODO: ...
// 调用举例: TODO: ...
// 作    者: fengxiaomin/00209182 [2015-08-04]
//*****************************************************************************
HI_U32 mrsStaEvtProc(HI_U8 *pData, HI_U16 usLen, MRS_645_FRAME_STRU *pst645Hrd)
{
    HI_U32 ulRet = HI_ERR_SUCCESS;
    P_MRS_STA_EVT_CTX pstCtx = mrsStaGetEvtCtx();

    mrsDfxStaMeterEvtRespCnt();

    ulRet = mrsStaEvtFilter(pst645Hrd);
    if (HI_ERR_CONTINUE != ulRet)
    {
        return ulRet;
    }

    if (MRS_STA_EVT_MONITOR_DOING == mrsStaEvtGetMonStatus())
    {
        if (HI_FALSE == mrsStaEvtCheckNewEvent(pst645Hrd->ucDataRealm, pst645Hrd->ucDataRealmLen))
        {
            HI_DIAG_LOG_MSG_E1(MRS_FILE_LOG_FLAG_005, HI_DIAG_MT("monitor no evt"), ulRet);
            return HI_ERR_SKIP;
        }

        mrsDfxStaEvtMonExistEvtCnt();
        mrsDfxStaEvtMonTimeStamp(HI_MDM_GetMilliSeconds());
        MRS_StopTimer(MRS_STA_TIMER_LOCAL_MONITOR);
    }

    mrsStaEvtClearLastData(&pstCtx->stStaMonInf);
    mrsStaEvtSetData(&pstCtx->stStaMonInf, pst645Hrd->ucDataRealm, pst645Hrd->ucDataRealmLen);

    return mrsStaEvtReportProc(pData, usLen);
}

//*****************************************************************************
// 函数名称: mrsStaEvtCheckNewEvent
// 功能描述: STA对于收到的645帧检查是否有新事件
//
// 参数说明:
//   pData      645帧数据域内容
//   usLen      645帧数据域长度
//
// 返 回 值:
//    HI_TRUE: 有新事件
//    其它错误码
//
// 调用要求: TODO: ...
// 调用举例: TODO: ...
// 作    者: fengxiaomin/00209182 [2015-08-04]
//*****************************************************************************
HI_BOOL mrsStaEvtCheckNewEvent(HI_U8 *pData, HI_U8 ucLen)
{
    HI_BOOL bRet = HI_FALSE;
    P_MRS_STA_EVT_CTX pstCtx = mrsStaGetEvtCtx();
    HI_U8 ucDataLenMin = MRS_645_FRAME_DATA_DI_SIZE + MRS_STA_EVT_STATUS_WORD_FLAG_LEN;

    if (!pData || (ucLen < ucDataLenMin))
    {
        return HI_FALSE;
    }

    do
    {
        //先判断数据域中的事件标志位是否为全00
        if (mrsToolsMemIsEq(pData + MRS_645_FRAME_DATA_DI_SIZE, MRS_STA_EVT_STATUS_WORD_FLAG_LEN, MRS_645_FRAME_HEX33))
        {
            mrsStaEvtClearLastData(&pstCtx->stStaMonInf);
            mrsStaEvtSetData(&pstCtx->stStaMonInf, pData, ucLen);
            break;
        }

        if (!pstCtx->stStaMonInf.pData || (pstCtx->stStaMonInf.ucDataLen < ucDataLenMin))
        {
            HI_DIAG_LOG_MSG_E2(MRS_FILE_LOG_FLAG_013, HI_DIAG_MT("reported evt invalid"), pstCtx->stStaMonInf.pData,
                            pstCtx->stStaMonInf.ucDataLen);
            bRet = HI_TRUE;
            break;
        }

        if ((ucLen != pstCtx->stStaMonInf.ucDataLen) || (memcmp(pstCtx->stStaMonInf.pData, pData, ucLen)))
        {
            HI_DIAG_LOG_MSG_E0(MRS_FILE_LOG_FLAG_004, HI_DIAG_MT("monitor new evt"));
            bRet = HI_TRUE;
            break;
        }
    } while (0);

    return bRet;
}

//*****************************************************************************
// 函数名称: mrsStaEvtClearLastData
// 功能描述: STA清除上一次上报过的事件信息
//
// 参数说明:
//   pstStaMonInf      STA监控信息
//
// 返 回 值:
//    无
//
// 调用要求: TODO: ...
// 调用举例: TODO: ...
// 作    者: fengxiaomin/00209182 [2015-08-04]
//*****************************************************************************
HI_VOID mrsStaEvtClearLastData(MRS_STA_EVT_MONITOR_INF *pstStaMonInf)
{
    if (pstStaMonInf->ucDataLen)
    {
        pstStaMonInf->ucDataLen = 0;
        mrsToolsFree(pstStaMonInf->pData);
    }
}

//*****************************************************************************
// 函数名称: mrsStaEvtSetData
// 功能描述: STA设置本次上报过的事件信息
//
// 参数说明:
//   pstStaMonInf      STA监控信息
//   pData             本次上报过的事件数据
//   ucLen             本次上报过的事件数据长度
//
// 返 回 值:
//    HI_ERR_SUCCESS: 成功
//    其它错误码
//
// 调用要求: TODO: ...
// 调用举例: TODO: ...
// 作    者: fengxiaomin/00209182 [2015-08-04]
//*****************************************************************************
HI_U32 mrsStaEvtSetData(MRS_STA_EVT_MONITOR_INF *pstStaMonInf, HI_U8 *pData, HI_U8 ucLen)
{
    if (ucLen)
    {
        pstStaMonInf->pData = (HI_U8*)mrsToolsMalloc(ucLen);
        if (!pstStaMonInf->pData)
        {
            return HI_ERR_MALLOC_FAILUE;
        }

        pstStaMonInf->ucDataLen = ucLen;
        (hi_void)memcpy_s(pstStaMonInf->pData, ucLen, pData, pstStaMonInf->ucDataLen);
    }

    return HI_ERR_SUCCESS;
}

HI_VOID mrsStaEvtSetMonStatus(HI_U8 ucStatus)
{
    mrsStaGetEvtCtx()->stStaMonInf.ucStatus = ucStatus;
    mrsDfxStaEvtCurrMonStatus(ucStatus);
}

HI_U8 mrsStaEvtGetMonStatus(HI_VOID)
{
    return mrsStaGetEvtCtx()->stStaMonInf.ucStatus;
}

HI_U32 mrsStaEvtMonProc(P_MRS_STA_EVT_CTX pstCtx)
{
    do
    {
        if(STA_TYPE_METER != mrsStaGetType() || mrsStaGetSupportVM())
        {
            mrsStaEvtClearLastData(&pstCtx->stStaMonInf);
            break;
        }

        if (HI_FALSE == pstCtx->bHardEvtOut)
        {
            mrsStaEvtClearLastData(&pstCtx->stStaMonInf);
            HI_DIAG_LOG_MSG_E0(MRS_FILE_LOG_FLAG_006, HI_DIAG_MT("evt pin low no mon"));
            break;
        }

        if (MRS_STA_EVT_MONITOR_IDLE == mrsStaEvtGetMonStatus())
        {
            mrsDfxStaEvtMonStartTimeStamp(HI_MDM_GetMilliSeconds());
        }

        mrsStaEvtSetMonStatus(MRS_STA_EVT_MONITOR_DOING);

        if (HI_TRUE == mrsStaEvtGetMonWaitPlcFlag())
        {
            HI_DIAG_LOG_MSG_E0(MRS_FILE_LOG_FLAG_012, HI_DIAG_MT("StaEvtMonProc waitplc true"));
            mrsStaSendQueryEvtStatusFrame();
            mrsDfxStaEvtMonWaitPlcQueryCnt();
        }

        //STA启动本地监控定时器
        MRS_StartTimer(MRS_STA_TIMER_LOCAL_MONITOR,
                       pstCtx->stStaMonInf.ucMonitorInterval * 60 * 1000,
                       HI_SYS_TIMER_ONESHOT);

        HI_DIAG_LOG_MSG_E1(MRS_FILE_LOG_FLAG_007, HI_DIAG_MT("StaEvtMonProc start mon evt"), pstCtx->stStaMonInf.ucMonitorInterval);
    } while (0);

    mrsStaEvtSetMonWaitPlcFlag(HI_FALSE);

    return HI_ERR_SUCCESS;
}

HI_U32 mrsStaEvtMonTimeOutProc(HI_VOID)
{
    P_MRS_STA_EVT_CTX pstCtx = mrsStaGetEvtCtx();

    if (HI_FALSE == pstCtx->bHardEvtOut)
    {
        HI_DIAG_LOG_MSG_E0(MRS_FILE_LOG_FLAG_009, HI_DIAG_MT("monitor timeout evt pin low"));
        mrsStaEvtMonEndProc();
        return HI_ERR_SUCCESS;
    }

    mrsStaSendQueryEvtStatusFrame();
    //STA启动本地监控定时器
    MRS_StartTimer(MRS_STA_TIMER_LOCAL_MONITOR,
                   pstCtx->stStaMonInf.ucMonitorInterval * 60 * 1000,
                   HI_SYS_TIMER_ONESHOT);

    HI_DIAG_LOG_MSG_E0(MRS_FILE_LOG_FLAG_010, HI_DIAG_MT("Mon timeout start mon evt"));


    return HI_ERR_SUCCESS;
}

HI_VOID mrsStaEvtMonEndProc(HI_VOID)
{
    P_MRS_STA_EVT_CTX pstCtx = mrsStaGetEvtCtx();

    if (MRS_STA_EVT_MONITOR_IDLE == mrsStaEvtGetMonStatus())
    {
        return;
    }

    MRS_StopTimer(MRS_STA_TIMER_LOCAL_MONITOR);
    mrsStaEvtClearLastData(&pstCtx->stStaMonInf);
    mrsStaEvtSetMonStatus(MRS_STA_EVT_MONITOR_IDLE);
    mrsDfxStaEvtMonEndTimeStamp(HI_MDM_GetMilliSeconds());

    HI_DIAG_LOG_MSG_E0(MRS_FILE_LOG_FLAG_011, HI_DIAG_MT("monitor end"));
}

HI_VOID mrsStaEvtSetMonWaitPlcFlag(HI_BOOL bFlag)
{
    mrsStaGetEvtCtx()->stStaMonInf.bWaitPlcFlag = bFlag;
    mrsDfxStaEvtMonSetWaitPlcFlag(bFlag);
}

HI_BOOL mrsStaEvtGetMonWaitPlcFlag(HI_VOID)
{
    return mrsStaGetEvtCtx()->stStaMonInf.bWaitPlcFlag;
}


#if !defined(PRODUCT_CFG_PRODUCT_TYPE_CSG_GD)
HI_VOID mrsCltiEvtRegFrameProc(EVT_FRAME_RX pfnRx, EVT_FRAME_TIMEOUT pfnTimeout)
{
    MRS_STA_EVT_CTX *pstEvtCtx = mrsStaGetEvtCtx();
    pstEvtCtx->stCltiEvtInf.pfnRx = pfnRx;
    pstEvtCtx->stCltiEvtInf.pfnTimeout = pfnTimeout;
}


HI_U32 mrsCltiEventFrameRx(HI_U8 *pFrame, HI_U16 usFrameLength, HI_U8 ucSubId)
{
    HI_U32 ulRet = HI_ERR_SUCCESS;
    MRS_STA_EVT_CTX *pstEvtCtx = mrsStaGetEvtCtx();

	if ((MRS_CLTI_EVT_QUERY_DISCARD == ucSubId)
		|| (MRS_CLTI_EVT_STATUS_WAIT_QUERY_REPLY != mrsCltiEvtGetStatus()))
	{
		return HI_ERR_SUCCESS;
	}

    if (pstEvtCtx->stCltiEvtInf.pfnRx)
    {
        ulRet = pstEvtCtx->stCltiEvtInf.pfnRx(pFrame, usFrameLength);
        if (HI_ERR_SUCCESS == ulRet)
        {
            mrsCltiEvtRegFrameProc(HI_NULL, HI_NULL);
        }
    }

    return ulRet;
}


HI_U32 mrsCltiEventQueryFrameRx(HI_U8 *pFrame, HI_U16 usFrameLength)
{
	HI_U32 ulRet = HI_ERR_SUCCESS;
    MRS_STA_EVT_CTX *pstEvtCtx = mrsStaGetEvtCtx();
    HI_U8 ucCtrl = pFrame[MRS_645_FRAME_CTRL_OFFSET];
    HI_U8 *pData = HI_NULL;
    HI_U8 ucDataLen = pFrame[MRS_645_FRAME_LENGTH_OFFSET];
    HI_U8 ucEvtCtrl = (HI_U8)(pFrame[MRS_645_FRAME_DATA_OFFSET + 0] - MRS_645_FRAME_HEX33);
    HI_U8 ucTotal   = (HI_U8)(pFrame[MRS_645_FRAME_DATA_OFFSET + 1] - MRS_645_FRAME_HEX33);
    HI_U8 ucNum     = (HI_U8)(pFrame[MRS_645_FRAME_DATA_OFFSET + 2] - MRS_645_FRAME_HEX33);

    if (MRS_645_CTRL_CLTI_EVT != (ucCtrl & MRS_645_FRAME_CONTROL_MASK))
    {
        return HI_ERR_BAD_DATA;
    }

    if (ucCtrl & MRS_645_FRAME_CONTROL_DENY_MASK)
    {
        return HI_ERR_BAD_DATA;
    }

    if (MRS_CLTI_EVT_CTRL_WORD_QRY != ucEvtCtrl)
    {
        return HI_ERR_BAD_DATA;
    }

    // 控制字1字节
    // 采集器下挂电表/I采总数量1字节(HEX格式)+本帧上报有事件的电表数量1字节(HEX格式)+N*8(6字节电表地址+1字节规约（1:97规约 2：07规约）+1字节事件状态字）
    if (ucDataLen != 3 + ucNum * 8)
    {
        return HI_ERR_BAD_DATA;
    }

    if (ucNum == 0)
    {
        mrsStaEvtCtxReset();
        mrsCltiEvtCtxReset();
        mrsStaEvtSetMonWaitPlcFlag(HI_FALSE);
        return HI_ERR_SUCCESS;
    }

    if ((ucTotal < (pstEvtCtx->stCltiEvtInf.ucQuery + ucNum)) || (ucTotal > MRS_CLTI_EVT_NUM_MAX))
    {
        return HI_ERR_BAD_DATA;
    }

    pData = mrsToolsMalloc(usFrameLength);
    if (!pData)
    {
        return HI_ERR_NOT_ENOUGH_MEMORY;
    }

    mrsToolsFree(pstEvtCtx->stCltiEvtInf.pucFrame);
    (hi_void)memcpy_s(pData, usFrameLength, pFrame, usFrameLength);
    pstEvtCtx->stCltiEvtInf.ucTotal = ucTotal;
    pstEvtCtx->stCltiEvtInf.ucQuery += ucNum;
    pstEvtCtx->stCltiEvtInf.pucFrame = pData;
    pstEvtCtx->stCltiEvtInf.ucFrameLen = (HI_U8)usFrameLength;

	ulRet = mrsStaEvtReportProc(pFrame, usFrameLength);
	if (HI_ERR_SUCCESS == ulRet)
	{
		mrsCltiEvtSetStatus(MRS_CLTI_EVT_STATUS_WAIT_PLC_REPLY);
	}
	else
	{
        mrsStaEvtCtxReset();
        mrsCltiEvtCtxReset();
        mrsStaEvtSetMonWaitPlcFlag(HI_FALSE);
	}

	return HI_ERR_SUCCESS;
}


//*****************************************************************************
// 函数名称: mrsCltiEvtQueryDiscardProc
// 功能描述: I采模块将STA队列中id为查询事件的子id设置为废弃查询
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
HI_VOID mrsCltiEvtQueryDiscardProc(MRS_QUE_MODULE *pstQueue, HI_U16 usId)
{
	MRS_STA_ITEM * pstItem = (MRS_STA_ITEM *)mrsSrvQueueTop(&pstQueue->stMrQueue);

	if (!pstItem)
	{
		return;
	}

	while (pstItem)
	{
		if (pstItem->id == usId)
		{
			pstItem->sub_id = MRS_CLTI_EVT_QUERY_DISCARD;
		}
		pstItem = (MRS_STA_ITEM *)pstItem->link.next;
	}

	return;
}

//*****************************************************************************
// 函数名称: mrsCltiRxPlcAckProc
// 功能描述: I采模块收到CCO应答确认后的处理
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
HI_U32 mrsCltiRxPlcAckProc(HI_VOID)
{
	if (MRS_CLTI_EVT_STATUS_WAIT_PLC_REPLY != mrsCltiEvtGetStatus())
	{
		return HI_ERR_FAILURE;
	}

	if (HI_ERR_SUCCESS != mrsStaSendQueryEvtStatusFrame())
	{
		mrsStaEvtCtxReset();
		mrsCltiEvtCtxReset();
		mrsStaEvtSetMonWaitPlcFlag(HI_FALSE);
	}

	return HI_ERR_SUCCESS;
}


HI_U32 mrsCltiEventFrameTimeout(HI_VOID)
{
    HI_U32 ulRet = HI_ERR_SUCCESS;
    MRS_STA_EVT_CTX *pstEvtCtx = mrsStaGetEvtCtx();

    if (pstEvtCtx->stCltiEvtInf.pfnTimeout)
    {
        ulRet = pstEvtCtx->stCltiEvtInf.pfnTimeout();
    }

    mrsCltiEvtRegFrameProc(HI_NULL, HI_NULL);
    return ulRet;
}


HI_U32 mrsCltiEventQueryFrameTimeout(HI_VOID)
{
    mrsStaEvtCtxReset();
    mrsCltiEvtCtxReset();
    mrsStaEvtSetMonWaitPlcFlag(HI_FALSE);
    return HI_ERR_SUCCESS;
}
#endif


#if defined(PRODUCT_CFG_PRODUCT_TYPE_CSG_GD)

HI_U32 mrsStaEvtPlcAck(MRS_CSG_PLC_FRAME_HEAD *pstFrameHead)
{
    P_MRS_STA_EVT_CTX pstCtx = mrsStaGetEvtCtx();

    if (pstCtx->usSeq != pstFrameHead->usSeq)
    {
        return HI_ERR_FAILURE;
    }

    mrsDfxStaPlcEvtAckCnt();
    mrsDfxStaPlcEvtAckTimeStamp(HI_MDM_GetMilliSeconds());
    mrsStaEvtCtxReset();
    mrsStaEvtSeqNvSave(pstCtx);
    mrsStaEvtMonProc(pstCtx);

    return HI_ERR_SUCCESS;
}


HI_U32 mrsStaEvtPlcDeny(MRS_CSG_PLC_FRAME_HEAD *pstFrameHead)
{
    P_MRS_STA_EVT_CTX pstCtx = mrsStaGetEvtCtx();

    if (pstCtx->usSeq != pstFrameHead->usSeq)
    {
        return HI_ERR_FAILURE;
    }

    if (pstFrameHead->aucData[0] == APP_CSG_PLC_CCO_BUSY)
    {
        pstCtx->ucRetry = 0;
        mrsDfxStaEvtRetry(pstCtx->ucRetry);
        mrsDfxStaPlcEvtFullCnt();
        mrsDfxStaPlcEvtFullTimeStamp(HI_MDM_GetMilliSeconds());

        MRS_StartTimer(MRS_STA_TIMER_EVENT_REPORT, pstCtx->ucTimeOutMax * 1000, HI_SYS_TIMER_ONESHOT);
        return HI_ERR_CONTINUE;
    }
    else
    {
        mrsStaEvtCtxReset();
        mrsStaEvtSeqNvSave(pstCtx);
        mrsStaEvtMonProc(pstCtx);
    }

    return HI_ERR_SUCCESS;
}

#endif


#endif

