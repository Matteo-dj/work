//*****************************************************************************
//
//                  版权所有 (C), 1998-2011, 华为技术有限公司
//
//*****************************************************************************
//  文 件 名   : mrs_srv_cco_event.c
//  版 本 号   : V1.0
//  作    者   : cuiate/00233580
//  生成日期   : 2013-03-21
//  功能描述   : 事件上报函数及接口
//
//  函数列表   : TODO: ...
//  修改历史   :
//  1.日    期 : 2013-03-21
//    作    者 : cuiate/00233580
//    修改内容 : 创建文件
//  2.日    期 : 2014-08-20
//    作    者 : fengxiaomin/00209182
//    修改内容 : 修改文件
//
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
#include "mrs_srv_list.h"
#include "mrs_srv_cco_event.h"
#include "mrs_fw_nv.h"
#include "mrs_srv_cco.h"
#include "mrs_srv_cco_queue.h"
#include "mrs_fw_proto_csg.h"
#include "mrs_fw_proto_csg_echo.h"
#include "mrs_srv_csg_plc_cco.h"
#include "mrs_power_failure.h"


#if defined(PRODUCT_CFG_PRODUCT_TYPE_CCO)


typedef struct
{
    HI_U8* pucAddr;    //表地址
    HI_U16 usSeq;      //序号
    HI_U8 result;      //结果
    HI_U8 padding;
}MRS_CCO_EVT_FIND_RESULT, *P_MRS_CCO_EVT_FIND_RESULT;


MRS_CCO_EVT_CTX_STRU g_CcoEvtCtx;

HI_U32 mrsCcoEvtCompatibility(MRS_EVENT_INFO_STRU* pstEvtTransmit, HI_U16 usEvtDataLen);
HI_BOOL mrsCcoFindRepeatEvtItem(HI_VOID *node, HI_VOID *param);
HI_U32 mrsCcoEvtRecvPlcProc(MRS_EVENT_INFO_STRU* pstEvtTransmit, HI_U16 usEvtDataLen, HI_U8 ucMac[]);
HI_U32 mrsCcoEvtIsJoinQueue(MRS_EVENT_INFO_STRU* pstEvtTransmit);
HI_U32 mrsCcoEvtRespToSta(MRS_EVENT_INFO_STRU* pstEvtTransmit, HI_U32 ulRespFlag, HI_U8 ucMac[]);
HI_U32 mrsCcoEvtAssembleUpFrame(MRS_EVENT_INFO_STRU* pstEvtTransmit, HI_PBYTE *ppBuf, HI_U16 *usBufLen);
HI_VOID mrsEvtReportAckFunc(HI_VOID * pAckParam);
HI_VOID mrsCcoEvtReportEnd(HI_VOID);
HI_VOID* mrsCcoEvtDeQueue(MRS_SRV_QUEUE *pstQueue);
HI_U8 mrsCcoFilterRepeatEvt(MRS_EVENT_INFO_STRU* pstEvtTransmit);
HI_U32 mrsCcoEvtDeFilterList(P_MRS_CCO_EVT_REPORTED_FILTER_ITEM pstItem);
HI_VOID mrsCcoEvtReportedAgedPro(P_MRS_CCO_EVT_REPORTED_FILTER_ITEM pstItem, HI_U8 *pOutRet);
HI_U8 mrsCcoFilterReportedEvt(MRS_EVENT_INFO_STRU* pstEvtTransmit);
HI_VOID mrsCcoEvtRepalceReportedItem(P_MRS_CCO_EVT_REPORTED_FILTER_ITEM pstItem, P_MRS_EVT_QUEUE_ITEM pstEvtItem);
HI_U32 mrsCcoEvtReportedAddList(P_MRS_CCO_EVT_CTX_STRU pstEvtCtx);

P_MRS_CCO_EVT_CTX_STRU mrsGetCcoEvtCtx(HI_VOID)
{
    return &g_CcoEvtCtx;
}

HI_VOID mrsCcoEvtInit(HI_VOID)
{
    MRS_LOCK_INIT(g_CcoEvtCtx.stEvtQueue.ucQueueLock);
    //NV初始化
    mrsNvCcoEvtCfg(ID_NV_APP_EVENT_CFG);
    mrsSrvInitQueue(&g_CcoEvtCtx.stEvtQueue.stQueue);
    mrsSrvInitList(&g_CcoEvtCtx.stEvtReportedList.stEvtFilterList);

#if defined(PRODUCT_CFG_PRODUCT_TYPE_CSG_GD)
    (hi_void)memset_s(&g_CcoEvtCtx.stNotice, sizeof(g_CcoEvtCtx.stNotice), 0, sizeof(g_CcoEvtCtx.stNotice));
#endif
}


HI_U32 mrsCmdCcoEvtTransmit(HI_IN HI_U16 usId, HI_PVOID pReqPacket, HI_U16 usPacketSize)
{
    HI_U32 ulRet = HI_ERR_SUCCESS;
    MRS_PLC_FRAME_DATA_STRU *pstPlcData = HI_NULL;
    MRS_EVENT_INFO_STRU* pstEvtTransmit = HI_NULL;
    HI_U16 usEvtDataLen = 0;
    HI_U8 ucMac[HI_PLC_MAC_ADDR_LEN] = {0};
    P_MRS_CCO_EVT_CTX_STRU pstEvtCtx = mrsGetCcoEvtCtx();

    HI_UNREF_PARAM(usId);
    HI_UNREF_PARAM(usPacketSize);

    pstPlcData = (MRS_PLC_FRAME_DATA_STRU *)pReqPacket;
    if (!pstPlcData || !pstPlcData->pucPayload)
    {
        return HI_ERR_FAILURE;
    }

    pstEvtTransmit = (MRS_EVENT_INFO_STRU*)pstPlcData->pucPayload;
    usEvtDataLen = pstPlcData->usPayloadLen;
    (hi_void)memcpy_s(ucMac, sizeof(ucMac), pstPlcData->ucMac, HI_PLC_MAC_ADDR_LEN);

    mrsDfxCcoEvtBakSave(pstEvtCtx->usStatBakTime);
    mrsDfxCcoEvtRxPlcCnt();
    mrsDfxCcoEvtRxPlcTime(HI_MDM_GetSeconds());
    ulRet = mrsCcoEvtCompatibility(pstEvtTransmit, usEvtDataLen);
    if (HI_ERR_SUCCESS != ulRet)
    {
        mrsDfxCcoEvtCompatibilityErrCnt();
        HI_DIAG_LOG_MSG_E1(MRS_FILE_LOG_FLAG_002, HI_DIAG_MT("compatibility error[%d]"),ulRet);
        return HI_ERR_FAILURE;
    }

    ulRet = mrsCcoEvtRecvPlcProc(pstEvtTransmit, usEvtDataLen, ucMac);

    return ulRet;
}


HI_U32 mrsCcoEvtCompatibility(MRS_EVENT_INFO_STRU* pstEvtTransmit, HI_U16 usEvtDataLen)
{
    HI_U32 ulRet = HI_ERR_SUCCESS;
    HI_U16 usLocalLen = 0;

    if (pstEvtTransmit->ulDirBit != MRS_PLC_UP_FLG)
    {
        return HI_ERR_BAD_FRAME;
    }

    usLocalLen = (HI_U16)(sizeof(MRS_EVENT_INFO_STRU) + pstEvtTransmit->ulDataLen);
    if (usEvtDataLen != (pstEvtTransmit->ulStructLen + pstEvtTransmit->ulDataLen))
    {
        return HI_ERR_BAD_DATA;
    }

    if (MRS_EVENT_FUN_REPORT != pstEvtTransmit->ulFunCode)
    {
        return HI_ERR_NOT_SUPPORT;
    }

    switch (pstEvtTransmit->ulInterfaceVer)
    {
    case MRS_EVENT_INF_UP_VER_R0:
    default:
        if (usLocalLen < usEvtDataLen)
        {
            return HI_ERR_FAILURE;
        }

        break;
    }

    return ulRet;
}

HI_BOOL mrsCcoFindRepeatEvtItem(HI_VOID *node, HI_VOID *param)
{
    P_MRS_EVT_QUEUE_ITEM pstItem = (P_MRS_EVT_QUEUE_ITEM)node;
    P_MRS_CCO_EVT_FIND_RESULT pstJoinItem = (P_MRS_CCO_EVT_FIND_RESULT)param;

    if ((!memcmp(pstItem->ucMeterAddr, pstJoinItem->pucAddr, HI_METER_ADDR_LEN))
        && (pstItem->usPlcSeq == pstJoinItem->usSeq))
    {
        pstJoinItem->result = MRS_CCO_FIND_REPEAT_EVT_SUCCESS;
        return HI_TRUE;
    }

    pstJoinItem->result = MRS_CCO_FIND_REPEAT_EVT_FAIL;

    return HI_FALSE;
}

HI_U8 mrsCcoFilterRepeatEvt(MRS_EVENT_INFO_STRU* pstEvtTransmit)
{
    MRS_CCO_EVT_FIND_RESULT stEvtFindRet;
    P_MRS_CCO_EVT_CTX_STRU pstEvtCtx = mrsGetCcoEvtCtx();

    (hi_void)memset_s(&stEvtFindRet, sizeof(stEvtFindRet), 0, sizeof(stEvtFindRet));
    stEvtFindRet.pucAddr = pstEvtTransmit->ucMeter;
    stEvtFindRet.usSeq = pstEvtTransmit->usSeq;
    mrsSrvTraverseQueue(&pstEvtCtx->stEvtQueue.stQueue, mrsCcoFindRepeatEvtItem, &stEvtFindRet);

    return stEvtFindRet.result;
}

//删除过滤链表中的条目
HI_U32 mrsCcoEvtDeFilterList(P_MRS_CCO_EVT_REPORTED_FILTER_ITEM pstItem)
{
    HI_U32 ulRet = HI_ERR_SUCCESS;
    P_MRS_CCO_EVT_CTX_STRU pstEvtCtx = mrsGetCcoEvtCtx();
    MRS_CCO_EVT_REPORTED_FILTER_LIST *pstEvtReportedList = &pstEvtCtx->stEvtReportedList;

    ulRet = mrsSrvListRemove(&pstEvtReportedList->stEvtFilterList, &pstItem->link);
    if (HI_ERR_SUCCESS == ulRet)
    {
        mrsToolsFree(pstItem);
        pstEvtReportedList->ucListNum--;
        mrsDfxCcoEvtListItemNum(pstEvtReportedList->ucListNum);
    }

    return ulRet;
}

HI_VOID mrsCcoEvtReportedAgedPro(P_MRS_CCO_EVT_REPORTED_FILTER_ITEM pstItem, HI_U8 *pOutRet)
{
    HI_U32 ulCurrTime = HI_MDM_GetSeconds();
    P_MRS_CCO_EVT_CTX_STRU pstEvtCtx = mrsGetCcoEvtCtx();

    if ((ulCurrTime - pstItem->ulTime) > pstEvtCtx->usFilterAgedTime * 60)
    {
        *pOutRet = MRS_CCO_FIND_REPEAT_EVT_FAIL;
        mrsCcoEvtDeFilterList(pstItem);
        mrsDfxCcoEvtRepAgedCnt();
    }
}

HI_U8 mrsCcoFilterReportedEvt(MRS_EVENT_INFO_STRU* pstEvtTransmit)
{
    P_MRS_CCO_EVT_CTX_STRU pstEvtCtx = mrsGetCcoEvtCtx();
    MRS_SRV_LINK *it, *next;
    MRS_SRV_LIST *list;
    HI_U8 ucRet = MRS_CCO_FIND_REPEAT_EVT_FAIL;

    list = &pstEvtCtx->stEvtReportedList.stEvtFilterList;
    it = list->next;
    while(it != list)
    {
        P_MRS_CCO_EVT_REPORTED_FILTER_ITEM pstItem = (P_MRS_CCO_EVT_REPORTED_FILTER_ITEM)it;
        next = it->next;
        if ((!memcmp(pstItem->ucMeterAddr, pstEvtTransmit->ucMeter, HI_METER_ADDR_LEN))
            && (pstItem->usPlcSeq == pstEvtTransmit->usSeq))
        {
            ucRet = MRS_CCO_FIND_REPEAT_EVT_SUCCESS;
            mrsCcoEvtReportedAgedPro(pstItem, &ucRet);
            HI_DIAG_LOG_MSG_E1(MRS_FILE_LOG_FLAG_011, HI_DIAG_MT("cco rx evt exist reported list,is not aged[%d]"), ucRet);
            break;
        }

        mrsCcoEvtReportedAgedPro(pstItem, &ucRet);
        it = next;
    }

    return ucRet;
}

HI_U32 mrsCcoEvtRecvPlcProc(MRS_EVENT_INFO_STRU* pstEvtTransmit, HI_U16 usEvtDataLen, HI_U8 ucMac[])
{
    HI_U32 ulRet = HI_ERR_SUCCESS;
    MRS_METER_INFO meter;
    P_MRS_EVT_QUEUE_ITEM pstEvtItem = HI_NULL;
    P_MRS_CCO_EVT_CTX_STRU pstEvtCtx = mrsGetCcoEvtCtx();

    HI_UNREF_PARAM(usEvtDataLen);

    //添加映射关系
    (hi_void)memset_s(&meter, sizeof(meter), 0, sizeof(meter));
    (hi_void)memcpy_s(meter.ucMeterAddr, sizeof(meter.ucMeterAddr), pstEvtTransmit->ucMeter, HI_METER_ADDR_LEN);

    HI_DIAG_LOG_MSG_E3(MRS_FILE_LOG_FLAG_003, HI_DIAG_MT("cco rx evt proc[%d][%d][%d]"),
                        meter.ucMeterAddr[0],meter.ucMeterAddr[1],meter.ucMeterAddr[2]);
    if (pstEvtCtx->ucReportEnable == HI_FALSE)
    {
        HI_DIAG_LOG_MSG_E0(__LINE__, HI_DIAG_MT("【CSG】【mrsCsg_AFN04_E8020403】stEvtCtx->ucReportEnable == HI_FALSE"));
        mrsDfxCcoEvtTxForbidPlcCnt();
        return mrsCcoEvtSendToSta(pstEvtTransmit, ucMac, MRS_EVENT_FUN_REPORT_DISABLE);//!!!回应否认帧 否认原因?
    }

    ulRet = mrsCcoEvtIsJoinQueue(pstEvtTransmit);
    if (HI_ERR_SUCCESS == ulRet)
    {
        HI_U8 *pucUpData = HI_NULL;
        HI_U16 usDataLen = 0;

#if !defined(PRODUCT_CFG_PRODUCT_TYPE_CSG_GD)
        ulRet = mrsCcoEvtAssembleUpFrame(pstEvtTransmit, &pucUpData, &usDataLen);
        if (HI_ERR_SUCCESS == ulRet)
#endif
        {
            HI_DIAG_LOG_MSG_E0(MRS_FILE_LOG_FLAG_059, HI_DIAG_MT("cco evt assemble ok,join evt queue"));
            pstEvtItem = (P_MRS_EVT_QUEUE_ITEM)mrsToolsMalloc(sizeof(MRS_EVT_QUEUE_ITEM) + usDataLen);
            if (!pstEvtItem)
            {
                mrsToolsFree(pucUpData);
                return HI_ERR_NO_MORE_MEMORY;
            }

            (hi_void)memset_s(pstEvtItem, sizeof(MRS_EVT_QUEUE_ITEM) + usDataLen, 0, sizeof(MRS_EVT_QUEUE_ITEM) + usDataLen);
            (hi_void)memcpy_s(pstEvtItem->ucMeterAddr, sizeof(pstEvtItem->ucMeterAddr), pstEvtTransmit->ucMeter, HI_METER_ADDR_LEN);
            pstEvtItem->usPlcSeq = pstEvtTransmit->usSeq;
            pstEvtItem->usDataLen = usDataLen;
            if(pucUpData != NULL){
                (hi_void)memcpy_s(pstEvtItem->pucData, pstEvtItem->usDataLen, pucUpData, usDataLen);            
            }
            mrsCcoEvtEnQueue(&pstEvtCtx->stEvtQueue, pstEvtItem);
            pstEvtCtx->bACDFlg = HI_TRUE;
            pstEvtCtx->stEvtQueue.usItemNum++;
            mrsActiveEvtReport();

            mrsDfxCcoEvtQueueItemNum(pstEvtCtx->stEvtQueue.usItemNum);
            mrsDfxCcoEvtFlag(pstEvtCtx->bACDFlg);
        }
#if !defined(PRODUCT_CFG_PRODUCT_TYPE_CSG_GD)
        else
        {
            ulRet = HI_ERR_FULL;
        }
#endif
        if(pucUpData != NULL){
            mrsToolsFree(pucUpData);
        }
    }

    HI_DIAG_LOG_MSG_E1(MRS_FILE_LOG_FLAG_004, HI_DIAG_MT("cco resp evt to sta ret[%d]"), ulRet);
    //应答PLC帧给STA
    mrsCcoEvtRespToSta(pstEvtTransmit, ulRet, ucMac);

    return HI_ERR_SUCCESS;
}

HI_VOID mrsCcoEvtEnQueue(MRS_CCO_EVT_QUEUE* pstEvtQueue, P_MRS_EVT_QUEUE_ITEM pstEvtItem)
{
    mrsSrvEnQueue(&pstEvtQueue->stQueue, &pstEvtItem->link);
    mrsDfxCcoEvtQueueFrontRear(&pstEvtQueue->stQueue);
}

HI_VOID* mrsCcoEvtDeQueue(MRS_SRV_QUEUE *pstQueue)
{
    MRS_SRV_NODE * pstEvtNode = HI_NULL;

    pstEvtNode = mrsSrvDeQueue(pstQueue);
    mrsDfxCcoEvtQueueFrontRear(pstQueue);

    return pstEvtNode;
}

HI_U32 mrsCcoEvtIsJoinQueue(MRS_EVENT_INFO_STRU* pstEvtTransmit)
{
    P_MRS_CCO_EVT_CTX_STRU pstEvtCtx = mrsGetCcoEvtCtx();
    HI_U8 ucFilterRet = 0;

    if (!pstEvtCtx->ucReportEnable)
    {
        mrsDfxCcoEvtRxReportDisableCnt();
        return HI_ERR_SKIP;
    }

#if !(defined(PRODUCT_CFG_PRODUCT_TYPE_CSG_GD))
    if (PROTO_VER_3762_2009 == mrsCcoGetContext()->ucProtoVer)
    {
        mrsDfxCcoEvtRxProtoErrCnt();
        return HI_ERR_SKIP;
    }
#endif

    ucFilterRet = mrsCcoFilterReportedEvt(pstEvtTransmit);
    if (MRS_CCO_FIND_REPEAT_EVT_SUCCESS == ucFilterRet)
    {
        mrsDfxCcoEvtReportedFilterCnt();
        return HI_ERR_SKIP;
    }

    if (pstEvtCtx->stEvtQueue.usItemNum >= pstEvtCtx->usEvtBufSize)
    {
        return HI_ERR_FULL;
    }

    ucFilterRet = mrsCcoFilterRepeatEvt(pstEvtTransmit);
    if (MRS_CCO_FIND_REPEAT_EVT_SUCCESS == ucFilterRet)
    {
        mrsDfxCcoEvtFilterPlcCnt();
        return HI_ERR_SKIP;
    }

    return HI_ERR_SUCCESS;
}

HI_U32 mrsCcoEvtRespToSta(MRS_EVENT_INFO_STRU* pstEvtTransmit, HI_U32 ulRespFlag, HI_U8 ucMac[])
{
    HI_U32 ulRet = HI_ERR_SUCCESS;

    switch (ulRespFlag)
    {
    case HI_ERR_SUCCESS:
    case HI_ERR_SKIP:
        ulRet = mrsCcoEvtSendToSta(pstEvtTransmit, ucMac, MRS_EVENT_FUN_ACK);
        mrsDfxCcoEvtTxAckPlcCnt();
        break;

    case HI_ERR_FULL:
        ulRet = mrsCcoEvtSendToSta(pstEvtTransmit, ucMac, MRS_EVENT_FUN_CCOBUF_FULL);
        mrsDfxCcoEvtTxFullPlcCnt();
        break;
    default:
        break;
    }

    return ulRet;
}



HI_U32 mrsCcoEvtAssembleUpFrame(MRS_EVENT_INFO_STRU* pstEvtTransmit, HI_PBYTE *ppBuf, HI_U16 *usBufLen)
{
    HI_U32 ulRet = HI_ERR_SUCCESS;
    HI_U8 *pucData = HI_NULL;
    HI_U16 usDataLen = 0;
    PROTO376_2_FRAME_PRE_STRU stFrame;
    MRS_CCO_SRV_CTX_STRU *pstCtx = mrsCcoGetContext();

    HI_DIAG_LOG_MSG_E0(__LINE__, HI_DIAG_MT("【CSG】【mrsCcoEvtAssembleUpFrame】start 上报从节点事件"));
    // 06-F5: 上报从节点事件
    do
    {
        HI_U16 usPos;
        HI_U16 usFrameLen;

        // 设备类型(1) + 规约类型(1) + 报文长度(1) + 报文(pstData->data_len)
        usDataLen = (HI_U16)(pstEvtTransmit->ulDataLen + 3);
        pucData = mrsToolsMalloc(usDataLen);
        if (!pucData)
        {
            ulRet = HI_ERR_MALLOC_FAILUE;
            break;
        }

        pucData[0] = MRS_DEVICE_TYPE_METER; // 设备类型
        pucData[1] = MRS_STA_PROTOCOL_2007; // 规约类型
        pucData[2] = (HI_U8)pstEvtTransmit->ulDataLen;
        (hi_void)memcpy_s(pucData + 3, usDataLen -3, ((HI_U8*)pstEvtTransmit) + pstEvtTransmit->ulStructLen, pstEvtTransmit->ulDataLen);

        (hi_void)memset_s(&stFrame, sizeof(stFrame), 0, sizeof(stFrame));
        stFrame.ucPrm = 1;
        stFrame.ucModuleFlag = 0;
        stFrame.ucIs645 = 0;
        stFrame.ucSeq = ++(pstCtx->ucSeq);

        ulRet = mrsFind645Frame(pucData + 3, pstEvtTransmit->ulDataLen, &usPos, &usFrameLen);
        if (HI_ERR_SUCCESS == ulRet)
        {
            (hi_void)memcpy_s(stFrame.ucSrcAdd, sizeof(stFrame.ucSrcAdd), (pucData + 3) + (usPos + 1), HI_METER_ADDR_LEN);
        }

        (hi_void)memcpy_s(stFrame.ucDstAdd, sizeof(stFrame.ucDstAdd), pstCtx->ucMainNodeAddr, HI_METER_ADDR_LEN);
        mrsHexInvert(stFrame.ucDstAdd, sizeof(stFrame.ucDstAdd));

        stFrame.stAfnData.ucAFN = MRS_AFN(0x06);
        stFrame.stAfnData.ucDT[0] = MRS_AFN_FN_LO(5);
        stFrame.stAfnData.ucDT[1] = MRS_AFN_FN_HI(5);
        stFrame.stAfnData.pData = pucData;
        stFrame.stAfnData.usDataLen = usDataLen;

        ulRet = mrsCreate3762Frame(&stFrame, ppBuf, usBufLen, HI_NULL);
        if (HI_ERR_SUCCESS != ulRet)
        {
            break;
        }
    } while (0);

    mrsToolsFree(pucData);

    return ulRet;
}



HI_U32 mrsCcoEvtSendToSta(MRS_EVENT_INFO_STRU* pstEvtTransmit, HI_U8 *ucMac, HI_U8 ucFunCode)
{
    MRS_PLC_FRAME_DATA_STRU stPlcFrame;
    MRS_EVENT_INFO_STRU *pstEvt = HI_NULL;
    HI_U16 usPayloadLen = 0;
    HI_U8 *pPayload = HI_NULL;

    (hi_void)memset_s(&stPlcFrame, sizeof(stPlcFrame), 0, sizeof(stPlcFrame));
    HI_DIAG_LOG_MSG_E0(__LINE__, HI_DIAG_MT("【CSG】【mrsCcoEvtSendToSta】start"));
    HI_DIAG_LOG_BUF(__LINE__, HI_DIAG_MT("【CSG】【mrsCcoEvtSendToSta】ucFunCode = "), &ucFunCode, sizeof(ucFunCode));

    usPayloadLen = sizeof(MRS_EVENT_INFO_STRU);
    pPayload = mrsToolsMalloc(usPayloadLen);
    if (!pPayload)
    {
        return HI_ERR_MALLOC_FAILUE;
    }

    (hi_void)memset_s(pPayload, usPayloadLen, 0, usPayloadLen);

    pstEvt = (MRS_EVENT_INFO_STRU *)pPayload;
    pstEvt->ulInterfaceVer = MRS_EVENT_INF_DL_VER;
    pstEvt->ulStructLen = sizeof(MRS_EVENT_INFO_STRU);
    pstEvt->ulDirBit = MRS_PLC_DL_FLG;
    pstEvt->ulFrmBit = MRS_EVENT_FRM_BIT_TO;
    pstEvt->ulFunCode = ucFunCode;
    pstEvt->ulDataLen= 0;
    pstEvt->usSeq = pstEvtTransmit->usSeq;
    (hi_void)memcpy_s(pstEvt->ucMeter, sizeof(pstEvt->ucMeter), pstEvtTransmit->ucMeter, HI_METER_ADDR_LEN);

    stPlcFrame.usId = ID_MRS_CMD_EVENT_INF_PLC;

    stPlcFrame.pucPayload = pPayload;
    stPlcFrame.usPayloadLen = usPayloadLen;
    (hi_void)memcpy_s(stPlcFrame.ucMac, sizeof(stPlcFrame.ucMac), ucMac, HI_PLC_MAC_ADDR_LEN);

    HI_DIAG_LOG_MSG_E0(MRS_FILE_LOG_FLAG_007, HI_DIAG_MT("cco send resp plc to sta"));
    MRS_SendPlcFrame(&stPlcFrame);

    mrsToolsFree(pPayload);

    return HI_ERR_SUCCESS;
}


HI_U32 mrsCcoEvtReportMsgProc(HI_VOID)
{
    HI_U32 ulRet = HI_ERR_SUCCESS;
    P_MRS_CCO_EVT_CTX_STRU pstEvtCtx = mrsGetCcoEvtCtx();
    MRS_CCO_EVT_QUEUE *pstEvtQueue = &pstEvtCtx->stEvtQueue;

    HI_DIAG_LOG_MSG_E0(MRS_FILE_LOG_FLAG_058, HI_DIAG_MT("cco evt try join cco queue"));
    if (MRS_TRY_LOCK(pstEvtQueue->ucQueueLock))
    {
        P_MRS_EVT_QUEUE_ITEM pstEvtItem = HI_NULL;
        MRS_SRV_CCO_ITEM *pstCcoItem = HI_NULL;

        pstEvtItem = mrsSrvQueueTop(&pstEvtQueue->stQueue);
        if (!pstEvtItem)
        {
            MRS_UNLOCK(pstEvtQueue->ucQueueLock);
            return HI_ERR_FAILURE;
        }

        pstCcoItem = (MRS_SRV_CCO_ITEM *)mrsToolsMalloc(sizeof(MRS_SRV_CCO_ITEM) + pstEvtItem->usDataLen);
        if (!pstCcoItem)
        {
            mrsCcoEvtReportEnd();
            mrsDfxEvtJoinCcoQueueMallocFailCnt();
            mrsDfxCcoQueueMallocFailCnt();
            return HI_ERR_NO_MORE_MEMORY;
        }

        HI_DIAG_LOG_MSG_E0(MRS_FILE_LOG_FLAG_057, HI_DIAG_MT("cco evt join cco queue"));
        (hi_void)memset_s(pstCcoItem, sizeof(MRS_SRV_CCO_ITEM) + pstEvtItem->usDataLen, 0, sizeof(MRS_SRV_CCO_ITEM) + pstEvtItem->usDataLen);
        //加入CCO队列
        pstCcoItem->bValid = HI_TRUE;
#if defined(PRODUCT_CFG_PRODUCT_TYPE_CSG_GD)
        pstCcoItem->ucAfn = MRS_CSG_AFN_REPORT_05H;
        pstCcoItem->usFn = (HI_U32)(MRS_CSG_DI_REPORT_EVENT&0xFFFF);
#else
        pstCcoItem->ucAfn = 0x06;
        pstCcoItem->usFn = 0x05;
#endif
        pstCcoItem->ucMaxTry = pstEvtCtx->ucMaxTry;
        pstCcoItem->usTimeOut = (HI_U16)mrsToolsGetTimeOut(pstEvtCtx->usTimeOut);//单位:ms
        pstCcoItem->usDataLen = pstEvtItem->usDataLen;
        (hi_void)memcpy_s(pstCcoItem->pData, pstCcoItem->usDataLen, pstEvtItem->pucData, pstEvtItem->usDataLen);
        pstCcoItem->MrsCcoRxRespProc = mrsEvtReportAckFunc;
        pstCcoItem->MrsCcoRxTotalTimeOutProc = mrsEvtReportTimeoutFunc;

        mrsCcoJoinQueue(pstCcoItem);

        HI_DIAG_LOG_MSG_E0(MRS_FILE_LOG_FLAG_056, HI_DIAG_MT("cco evt join cco queue success"));
        mrsActiveCcoQueue();

        mrsDfxCcoEvtReportCnt();
        mrsDfxEvtJoinCcoQueueTime(HI_MDM_GetSeconds());
    }

    return ulRet;
}

HI_VOID mrsCcoEvtRepalceReportedItem(P_MRS_CCO_EVT_REPORTED_FILTER_ITEM pstItem, P_MRS_EVT_QUEUE_ITEM pstEvtItem)
{
    MRS_SRV_LIST *list;
    P_MRS_CCO_EVT_CTX_STRU pstEvtCtx = mrsGetCcoEvtCtx();
    MRS_CCO_EVT_REPORTED_FILTER_LIST *pstEvtReportedList = &pstEvtCtx->stEvtReportedList;

    list = &pstEvtReportedList->stEvtFilterList;

    pstItem->usPlcSeq = pstEvtItem->usPlcSeq;
    pstItem->ulTime = HI_MDM_GetSeconds();
    if (HI_ERR_SUCCESS == mrsSrvListRemove(list, &pstItem->link))
    {
        mrsSrvListInsTail(list, &pstItem->link);
    }
}

HI_U32 mrsCcoEvtReportedAddList(P_MRS_CCO_EVT_CTX_STRU pstEvtCtx)
{
    HI_U32 ulRet = HI_ERR_SUCCESS;
    MRS_CCO_EVT_REPORTED_FILTER_LIST *pstEvtReportedList = &pstEvtCtx->stEvtReportedList;
    MRS_SRV_LINK *it, *next;
    MRS_SRV_LIST *list;
    P_MRS_EVT_QUEUE_ITEM pstEvtItem = HI_NULL;

    pstEvtItem = mrsSrvQueueTop(&pstEvtCtx->stEvtQueue.stQueue);
    if (!pstEvtItem)
    {
        MRS_UNLOCK(pstEvtCtx->stEvtQueue.ucQueueLock);
        return HI_ERR_FAILURE;
    }

    list = &pstEvtReportedList->stEvtFilterList;
    it = list->next;
    while(it != list)
    {
        P_MRS_CCO_EVT_REPORTED_FILTER_ITEM pstItem = (P_MRS_CCO_EVT_REPORTED_FILTER_ITEM)it;

        next = it->next;

        if (!memcmp(pstItem->ucMeterAddr, pstEvtItem->ucMeterAddr, HI_METER_ADDR_LEN))
        {
            mrsCcoEvtRepalceReportedItem(pstItem, pstEvtItem);
            mrsDfxCcoEvtRepReplaceSameAddrCnt();
            break;
        }

        it = next;
    }

    if (it == list)
    {
        P_MRS_CCO_EVT_REPORTED_FILTER_ITEM pstItem = HI_NULL;

        if (pstEvtReportedList->ucListNum >= pstEvtCtx->usEvtBufSize)
        {
            pstItem = (P_MRS_CCO_EVT_REPORTED_FILTER_ITEM)it->next;
            mrsCcoEvtRepalceReportedItem(pstItem, pstEvtItem);
            mrsDfxCcoEvtRepReplaceHeadCnt();
            return ulRet;
        }

        pstItem = (P_MRS_CCO_EVT_REPORTED_FILTER_ITEM)mrsToolsMalloc(sizeof(MRS_CCO_EVT_REPORTED_FILTER_ITEM));
        if (!pstItem)
        {
            mrsDfxEvtReportedListMallocFailCnt();
            return HI_ERR_NOT_ENOUGH_MEMORY;
        }

        (hi_void)memset_s(pstItem, sizeof(MRS_CCO_EVT_REPORTED_FILTER_ITEM), 0, sizeof(MRS_CCO_EVT_REPORTED_FILTER_ITEM));
        (hi_void)memcpy_s(pstItem->ucMeterAddr, sizeof(pstItem->ucMeterAddr), pstEvtItem->ucMeterAddr, HI_METER_ADDR_LEN);
        pstItem->usPlcSeq = pstEvtItem->usPlcSeq;
        pstItem->ulTime = HI_MDM_GetSeconds();
        mrsSrvListInsTail(list, &pstItem->link);
        pstEvtReportedList->ucListNum++;
        mrsDfxCcoEvtListItemNum(pstEvtReportedList->ucListNum);
    }

    return ulRet;
}

HI_VOID mrsEvtReportAckFunc(HI_VOID * param)
{
    MRS_NOT_USED(param);

    HI_DIAG_LOG_MSG_E0(MRS_FILE_LOG_FLAG_008, HI_DIAG_MT("evt report rx ack"));

    mrsDfxCcoEvtRxAckCnt();
    mrsCcoEvtReportEnd();

    return;
}

HI_U32 mrsEvtReportTimeoutFunc(HI_VOID * param)
{
    MRS_NOT_USED(param);
    HI_DIAG_LOG_MSG_E0(MRS_FILE_LOG_FLAG_009, HI_DIAG_MT("evt report timeout"));

    mrsDfxCcoEvtReportTimeOut();

    mrsCcoEvtReportEnd();

    return HI_ERR_SUCCESS;
}

HI_VOID mrsCcoEvtReportEnd(HI_VOID)
{
    P_MRS_EVT_QUEUE_ITEM pstEvtItem = HI_NULL;
    P_MRS_CCO_EVT_CTX_STRU pstEvtCtx = mrsGetCcoEvtCtx();
    MRS_CCO_EVT_QUEUE *pstEvtQueue = &pstEvtCtx->stEvtQueue;

    mrsCcoEvtReportedAddList(pstEvtCtx);
    pstEvtItem = mrsCcoEvtDeQueue(&pstEvtQueue->stQueue);
    if (pstEvtItem)
    {
        mrsToolsFree(pstEvtItem);
    }

    MRS_UNLOCK(pstEvtQueue->ucQueueLock);
    pstEvtQueue->usItemNum--;
    mrsDfxCcoEvtQueueItemNum(pstEvtQueue->usItemNum);
    if (!pstEvtQueue->usItemNum)
    {
        HI_DIAG_LOG_MSG_E0(MRS_FILE_LOG_FLAG_010, HI_DIAG_MT("cco evt report finish"));
        pstEvtCtx->bACDFlg = HI_FALSE;
        mrsDfxCcoEvtFlag(pstEvtCtx->bACDFlg);
        return;
    }

    mrsActiveEvtReport();

    return;
}


#if defined(PRODUCT_CFG_PRODUCT_TYPE_CSG_GD)
//*****************************************************************************
//*****************************************************************************


HI_VOID mrsCcoEvtNoticeReset(HI_VOID)
{
    MRS_CCO_EVT_CTX_STRU *pstEvtCtx = mrsGetCcoEvtCtx();

    MRS_StopTimer(MRS_CCO_TIMER_EVT_FLAG_NOTICE);

    if (pstEvtCtx->stNotice.pstPlcFrame)
    {
        mrsToolsFree(pstEvtCtx->stNotice.pstPlcFrame->pucPayload);
        mrsToolsFree(pstEvtCtx->stNotice.pstPlcFrame);
    }

    pstEvtCtx->stNotice.ucInterval = MRS_CCO_EVT_FLAG_NOTICE_INTERVAL;
    pstEvtCtx->stNotice.ucRetry = 0;
    pstEvtCtx->stNotice.ucRetryMax = MRS_CCO_EVT_FLAG_NOTICE_RETRYMAX;
}


HI_U32 mrsCcoEvtCreateNoticeFrame(HI_IN HI_U8 ucFlag, HI_IN HI_U8 *pucMacAddr, HI_OUT MRS_PLC_FRAME_DATA_S **ppstPlcFrame)
{
    MRS_CCO_SRV_CTX_STRU *pstCcoCtx = mrsCcoGetContext();
    MRS_PLC_FRAME_DATA_S *pstPlcData = HI_NULL;
    MRS_CSG_PLC_FRAME_HEAD *pstFrameHead = HI_NULL;
    HI_U8 *pucPayload = HI_NULL;
    HI_U16 usPayloadLen = sizeof(MRS_CSG_PLC_FRAME_HEAD) + sizeof(HI_U8);

    pstPlcData = (MRS_PLC_FRAME_DATA_S *)mrsToolsMalloc(sizeof(MRS_PLC_FRAME_DATA_S));
    if (!pstPlcData)
    {
        return HI_ERR_MALLOC_FAILUE;
    }

    pucPayload = (HI_U8 *)mrsToolsMalloc(usPayloadLen);
    if (!pucPayload)
    {
        mrsToolsFree(pstPlcData);
        return HI_ERR_MALLOC_FAILUE;
    }

    (hi_void)memset_s(pstPlcData, sizeof(MRS_PLC_FRAME_DATA_S), 0, sizeof(MRS_PLC_FRAME_DATA_S));
    (hi_void)memset_s(pucPayload, usPayloadLen, 0, usPayloadLen);

    pstFrameHead = (MRS_CSG_PLC_FRAME_HEAD *)pucPayload;
    pstFrameHead->usFrameType = MRS_CSG_PLC_CTRL_TYPE_CMD;
    pstFrameHead->usExtendBit = MRS_CSG_PLC_CTRL_EXTEND_BIT_N;
    pstFrameHead->usRespBit = MRS_CSG_PLC_CTRL_RESP_BIT_N;
    pstFrameHead->usPrm = MRS_CSG_PLC_CTRL_PRM_HOST;
    pstFrameHead->usDir = MRS_CSG_PLC_CTRL_DIR_DL;
    pstFrameHead->ucSrvId = MRS_CSG_PLC_SRV_ID_CMD_EVENT_FLAG;
    pstFrameHead->ucVersion = MRS_CSG_PLC_PROTO_VERSION;
    pstFrameHead->usSeq = pstCcoCtx->usPlcSeq++;
    pstFrameHead->usDataLength = sizeof(HI_U8);
    pstFrameHead->aucData[0] = ucFlag;

    pstPlcData->usId = ID_MRS_CMD_CSG_DATA_TRANSMIT_PLC;
    pstPlcData->pucPayload = pucPayload;
    pstPlcData->usPayloadLen = usPayloadLen;
    (hi_void)memcpy_s(pstPlcData->ucMac, HI_PLC_MAC_ADDR_LEN, pucMacAddr, HI_PLC_MAC_ADDR_LEN);

    *ppstPlcFrame = pstPlcData;

    return HI_ERR_SUCCESS;
}


HI_U32 mrsCcoEvtNoticeSendFrame(HI_VOID)
{
    MRS_CCO_EVT_CTX_STRU *pstEvtCtx = mrsGetCcoEvtCtx();

    if (!pstEvtCtx->stNotice.pstPlcFrame)
    {
        return HI_ERR_BAD_DATA;
    }

    MRS_SendPlcFrame(pstEvtCtx->stNotice.pstPlcFrame);

    pstEvtCtx->stNotice.ucRetry++;
    if (pstEvtCtx->stNotice.ucRetry < pstEvtCtx->stNotice.ucRetryMax)
    {
        MRS_StartTimer(MRS_CCO_TIMER_EVT_FLAG_NOTICE, pstEvtCtx->stNotice.ucInterval * 1000, HI_SYS_TIMER_ONESHOT);
    }
    else
    {
        mrsCcoEvtNoticeReset();
    }

    return HI_ERR_SUCCESS;
}


HI_U32 mrsCcoEvtFlagNotice(HI_IN HI_U8 ucFlag)
{
    HI_U32 ulRet = HI_ERR_SUCCESS;
    MRS_CCO_EVT_CTX_STRU *pstEvtCtx = mrsGetCcoEvtCtx();

    mrsCcoEvtNoticeReset();

    ulRet = mrsCcoEvtCreateNoticeFrame(ucFlag, (HI_U8 *)MRS_BROADCAST_MAC, &pstEvtCtx->stNotice.pstPlcFrame);
    if (HI_ERR_SUCCESS != ulRet)
    {
        return ulRet;
    }

    return mrsCcoEvtNoticeSendFrame();
}


HI_U32 mrsCcoEvtFlagDisableNotice(HI_IN HI_U8 *pucMacAddr)
{
    MRS_PLC_FRAME_DATA_S *pstPlcFrame = HI_NULL;
    HI_U32 ulRet = HI_ERR_SUCCESS;

    ulRet = mrsCcoEvtCreateNoticeFrame(MRS_EVENT_REPORT_DISABLE, pucMacAddr, &pstPlcFrame);
    if (HI_ERR_SUCCESS != ulRet)
    {
        return ulRet;
    }

    MRS_SendPlcFrame(pstPlcFrame);

    mrsToolsFree(pstPlcFrame->pucPayload);
    mrsToolsFree(pstPlcFrame);

    return HI_ERR_SUCCESS;
}


HI_U32 mrsCcoEvtFilterCheck(HI_U8 *pucMeterAddr, HI_U16 usSeq)
{
    MRS_EVENT_INFO_STRU stEvtInf;

    (hi_void)memset_s(&stEvtInf, sizeof(stEvtInf), 0, sizeof(stEvtInf));
    stEvtInf.ulInterfaceVer = MRS_EVENT_INF_UP_VER;
    stEvtInf.ulStructLen = sizeof(MRS_EVENT_INFO_STRU);
    stEvtInf.ulDirBit = MRS_EVENT_DIR_BIT_STA2CCO;
    stEvtInf.ulFrmBit = MRS_EVENT_FRM_BIT_FROM;
    stEvtInf.ulFunCode = MRS_EVENT_FUN_REPORT;
    stEvtInf.ulDataLen = 0;
    stEvtInf.usSeq = usSeq;
    (hi_void)memcpy_s(stEvtInf.ucMeter, HI_METER_ADDR_LEN, pucMeterAddr, HI_METER_ADDR_LEN);

    return mrsCcoEvtIsJoinQueue(&stEvtInf);
}


HI_U32 mrsCcoEvtCreateReportFrame(MRS_CSG_PLC_FRAME_HEAD *pstFrameHead, HI_U8 **ppOutFrame, HI_U16 *pusOutLength)
{
    MRS_CCO_SRV_CTX_STRU *pstCcoCtx = mrsCcoGetContext();
    MRS_CSG_IND_FRAME_STRU stIndFrame = {0};
    HI_U32 ulRet = HI_ERR_SUCCESS;

    (hi_void)memset_s(&stIndFrame, sizeof(stIndFrame), 0, sizeof(stIndFrame));
    stIndFrame.ucPrm = PROTOCSG_CTRL_PRM_BIT_REQ;
    stIndFrame.ucAddrFlag = PROTOCSG_CTRL_ADD_BIT_Y;
    stIndFrame.ucSeq = ++pstCcoCtx->ucSeq;

    (hi_void)memcpy_s(stIndFrame.aucSrcAddr, HI_METER_ADDR_LEN, pstFrameHead->aucData, HI_METER_ADDR_LEN);
    (hi_void)memcpy_s(stIndFrame.aucDstAddr, HI_METER_ADDR_LEN, pstCcoCtx->ucMainNodeAddr, HI_METER_ADDR_LEN);

    stIndFrame.ucAfn = MRS_CSG_AFN_REPORT_05H;
    stIndFrame.ulDI = MRS_CSG_DI_REPORT_EVENT;
    stIndFrame.usAppDataLen = pstFrameHead->usDataLength + 1 - HI_METER_ADDR_LEN;
    stIndFrame.pAppData = (HI_U8 *)mrsToolsMalloc(stIndFrame.usAppDataLen);
    if (!stIndFrame.pAppData)
    {
        return HI_ERR_MALLOC_FAILUE;
    }

    stIndFrame.pAppData[0] = (HI_U8)(pstFrameHead->usDataLength - HI_METER_ADDR_LEN);
    (hi_void)memcpy_s(stIndFrame.pAppData + 1,  pstFrameHead->usDataLength - HI_METER_ADDR_LEN,
					pstFrameHead->aucData + HI_METER_ADDR_LEN, pstFrameHead->usDataLength - HI_METER_ADDR_LEN);

    ulRet = mrsCreateCsgFrame(&stIndFrame, ppOutFrame, pusOutLength);
    mrsToolsFree(stIndFrame.pAppData);

    return ulRet;
}


HI_U32 mrsCcoEvtPlcFrameProc(MRS_PLC_FRAME_DATA_STRU *pstPlcData)
{
    HI_U32 ulRet = HI_ERR_SUCCESS;
    MRS_CSG_PLC_FRAME_HEAD * pstFrameHead = (MRS_CSG_PLC_FRAME_HEAD *)pstPlcData->pucPayload;
    P_MRS_CCO_EVT_CTX_STRU pstEvtCtx = mrsGetCcoEvtCtx();
    POWER_FAILURE_CTRL_S *power_failure_bitmap = mrsGetPowerFailureCtrlInfo();
    NV_APP_MRS_EVT_CFG_STRU *powerFailureNvCtrl = mrsGetPowerFailureNvCtrl();

    HI_DIAG_LOG_MSG_E1(MRS_FILE_LOG_FLAG_067, HI_DIAG_MT("[CCO-MR-PW]"), pstFrameHead->aucData[0]);

    if(pstFrameHead->usDataLength <= DATA_FIX_SIZE ||
    pstFrameHead->usDataLength + sizeof(MRS_CSG_PLC_FRAME_HEAD) != pstPlcData->usPayloadLen)
    {
        return HI_ERR_BAD_DATA;
    }
    //停电模块功能码处理
    if(MRS_EVENT_FUN_REPORT_MODUL == pstFrameHead->aucData[0]
        &&  MRS_CCO_DETECT_POWER_FAILURE_ENABLE == powerFailureNvCtrl->ucPowerFailureReportEnable)
    {

        //接收到模块功能后，获取数据信息到全局变量中，进行整合
        HI_DIAG_LOG_BUF(MRS_FILE_LOG_BUF_1000, HI_DIAG_MT("[CCO-MR]-PW get other data"),
                        pstFrameHead->aucData,pstFrameHead->usDataLength);

        mrsGetOthersPowerFailureFrameData(pstFrameHead);

        if(HI_FALSE == power_failure_bitmap->isRedayToSend)
        {
            HI_DIAG_LOG_MSG_E0(MRS_FILE_LOG_FLAG_066, HI_DIAG_MT("[CCO-MR-PW]"));
            ulRet = mrsCcoEvtReport();
        }

        return ulRet;
    }

    if (pstFrameHead->usDataLength < HI_METER_ADDR_LEN + MRS_645_FRAME_LENGTH_MIN)
    {
        return HI_ERR_BAD_DATA;
    }

    mrsDfxCcoEvtBakSave(pstEvtCtx->usStatBakTime);
    mrsDfxCcoEvtRxPlcCnt();
    mrsDfxCcoEvtRxPlcTime(HI_MDM_GetSeconds());

    if (pstEvtCtx->ucReportEnable == HI_FALSE)
    {
        mrsCcoCsgPlcDenyFrame(pstPlcData->ucMac, pstFrameHead->usSeq, APP_CSG_PLC_DENY_OTHER);
        mrsCcoEvtFlagDisableNotice(pstPlcData->ucMac);
        return HI_ERR_SUCCESS;
    }

    ulRet = mrsCcoEvtFilterCheck(pstFrameHead->aucData, pstFrameHead->usSeq);
    if (ulRet == HI_ERR_SUCCESS)
    {
        HI_U8 *pucData = HI_NULL;
        HI_U16 usDataLen = 0;

        do
        {
            MRS_EVT_QUEUE_ITEM *pstItem = HI_NULL;

            ulRet = mrsCcoEvtCreateReportFrame(pstFrameHead, &pucData, &usDataLen);
            if (HI_ERR_SUCCESS != ulRet)
            {
                ulRet = HI_ERR_FULL;
                break;
            }

            pstItem = (MRS_EVT_QUEUE_ITEM *)mrsToolsMalloc(sizeof(MRS_EVT_QUEUE_ITEM) + usDataLen);
            if (!pstItem)
            {
                mrsToolsFree(pucData);
                return HI_ERR_MALLOC_FAILUE;
            }

            (hi_void)memset_s(pstItem, sizeof(MRS_EVT_QUEUE_ITEM) + usDataLen, 0, sizeof(MRS_EVT_QUEUE_ITEM) + usDataLen);
            pstItem->usPlcSeq = pstFrameHead->usSeq;
            (hi_void)memcpy_s(pstItem->ucMeterAddr, HI_METER_ADDR_LEN, pstFrameHead->aucData, HI_METER_ADDR_LEN);
            pstItem->usDataLen = usDataLen;
            (hi_void)memcpy_s(pstItem->pucData, pstItem->usDataLen, pucData, usDataLen);
            mrsCcoEvtEnQueue(&pstEvtCtx->stEvtQueue, pstItem);

            pstEvtCtx->bACDFlg = HI_TRUE;
            pstEvtCtx->stEvtQueue.usItemNum++;
            mrsActiveEvtReport();

            mrsDfxCcoEvtQueueItemNum(pstEvtCtx->stEvtQueue.usItemNum);
            mrsDfxCcoEvtFlag(pstEvtCtx->bACDFlg);

            mrsToolsFree(pucData);
        } while (0);
    }

    switch (ulRet)
    {
    case HI_ERR_SUCCESS:
    case HI_ERR_SKIP:
        mrsCcoCsgPlcAckFrame(pstPlcData->ucMac, pstFrameHead->usSeq);
        mrsDfxCcoEvtTxAckPlcCnt();
        break;

    case HI_ERR_FULL:
        mrsDfxCcoEvtTxFullPlcCnt();
        mrsCcoCsgPlcDenyFrame(pstPlcData->ucMac, pstFrameHead->usSeq, APP_CSG_PLC_CCO_BUSY);
        break;

    default:
        break;
    }

    return ulRet;
}


//*****************************************************************************
//*****************************************************************************
#endif // defined(PRODUCT_CFG_PRODUCT_TYPE_CSG_GD)


#endif
