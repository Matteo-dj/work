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
#include "mrs_srv_parallel_cco.h"
#include "mrs_srv_queue.h"
#include "mrs_srv_list.h"
#include "mrs_srv_cco_event.h"
#include "mrs_fw_nv.h"
#include "mrs_srv_cco.h"
#include "mrs_srv_cco_queue.h"
#include "mrs_power_failure.h"
#include "mrs_srv_cltii_power_on_off.h"
#include "mrs_fw_proto698_45.h"
#if defined(PRODUCT_CFG_PRODUCT_TYPE_CCO)

typedef struct
{
    HI_U8* pucAddr;    //表地址
    HI_U16 usSeq;      //序号
    HI_U8 result;      //结果
    HI_U8 padding;
}MRS_CCO_EVT_FIND_RESULT, *P_MRS_CCO_EVT_FIND_RESULT;


MRS_CCO_EVT_CTX_STRU g_CcoEvtCtx;

HI_U8 g_aucEvtFrame698[] = {
    0x68,
    0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,
    0x68,
    0x91,
    0x12,
    0x34,0x48,0x33,0x37,
    0x33,0x33,0x33,0x33,0x33,0x33,0x33,0x33,0x33,0x33,0x33,0x33,0xDD,0xDD,
    0x73,
    0x16
};

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
}

HI_U32 mrsCmdCcoEvtTransmit(HI_IN HI_U16 usId, HI_PVOID pReqPacket, HI_U16 usPacketSize)
{
    HI_U32 ulRet = HI_ERR_SUCCESS;
    MRS_PLC_FRAME_DATA_STRU *pstPlcData = HI_NULL;
    MRS_EVENT_INFO_STRU* pstEvtTransmit = HI_NULL;
    HI_U16 usEvtDataLen = 0;
    HI_U8 ucMac[HI_PLC_MAC_ADDR_LEN] = {0};
    P_MRS_CCO_EVT_CTX_STRU pstEvtCtx = mrsGetCcoEvtCtx();
    POWER_FAILURE_CTRL_S *powerFailureCtrlInfo = mrsGetPowerFailureCtrlInfo();
    NV_APP_MRS_EVT_CFG_STRU *powerFailureNvCtrl = mrsGetPowerFailureNvCtrl();

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

    //停电模块功能码处理
    if(((MRS_EVENT_FUN_REPORT_MODUL == pstEvtTransmit->ulFunCode) ||
        (MRS_EVENT_FUN_REPORT_CLT == pstEvtTransmit->ulFunCode)) &&
        (MRS_CCO_DETECT_POWER_FAILURE_ENABLE == powerFailureNvCtrl->ucPowerFailureReportEnable))
    {
        if(((pstEvtTransmit->ulStructLen + pstEvtTransmit->ulDataLen) != usEvtDataLen) ||
            ((pstEvtTransmit->ulDataLen <= DATA_FIX_SIZE) &&
            (MRS_EVENT_FUN_REPORT_MODUL == pstEvtTransmit->ulFunCode)) ||
            ((pstEvtTransmit->ulDataLen < DATA_CLT_MIN_SIZE) &&
            (MRS_EVENT_FUN_REPORT_CLT == pstEvtTransmit->ulFunCode)))
        {
            return HI_ERR_BAD_FRAME;
        }

        //接收到模块功能后，获取数据信息到全局变量中，进行整合
        HI_DIAG_LOG_BUF(MRS_FILE_LOG_BUF_1000, HI_DIAG_MT("[CCO-MR]-PW get other data"),
                        pstEvtTransmit->ucData, HI_MIN(pstEvtTransmit->ulDataLen,200));

        mrsGetOthersPowerFailureFrameData(pstEvtTransmit, ucMac, pstPlcData->is_local_bc_frame);

        if(pstEvtTransmit->ulFunCode == MRS_EVENT_FUN_REPORT_MODUL){
            // 判断是否需要回复sack给该站点
            if(!pstPlcData->is_local_bc_frame && mrsCcoNotReplyAck(ucMac, pstEvtTransmit->ucData[0]) != HI_TRUE)
            {
                mrsCcoCreatePowerFailureRspAck(pstEvtTransmit, ucMac);
            }

            if(HI_FALSE == powerFailureCtrlInfo->isRedayToSendPowerFailure &&
                pstEvtTransmit->ucData[0] == STA_POWER_FAILURE)
            {
                HI_DIAG_LOG_MSG_E0(MRS_FILE_LOG_FLAG_066, HI_DIAG_MT("[CCO-MR-PW]"));
                ulRet = mrsCcoEvtReport(pstEvtTransmit->ucData[0]);
            }

            if(HI_FALSE == powerFailureCtrlInfo->isRedayToSendPowerOn &&
                pstEvtTransmit->ucData[0] == STA_POWER_ON)
            {
            	return ulRet;
            }

        }
		if(pstPlcData->is_local_bc_frame==HI_FALSE)
		{
			//应答PLC帧给STA
    		mrsCcoEvtRespToSta(pstEvtTransmit, HI_ERR_SUCCESS, ucMac);
		}
        return ulRet;
    }

    mrsDfxCcoEvtBakSave(pstEvtCtx->usStatBakTime);
    mrsDfxCcoEvtRxPlcCnt();
    mrsDfxCcoEvtRxPlcTime(HI_MDM_GetSeconds());
    ulRet = mrsCcoEvtCompatibility(pstEvtTransmit, usEvtDataLen);
    if (HI_ERR_SUCCESS != ulRet)
    {
        mrsDfxCcoEvtCompatibilityErrCnt();
        HI_DIAG_LOG_MSG_E1(MRS_FILE_LOG_FLAG_002, HI_DIAG_MT("compatibility error[%d]"), ulRet);
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

    //上报CCO,需要功能码支持
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
            HI_DIAG_LOG_MSG_E1(MRS_FILE_LOG_FLAG_011, HI_DIAG_MT("cco rx evt exist reported list,is not aged[%d]"),
                               ucRet);
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
    ulRet = mrsCcoEvtIsJoinQueue(pstEvtTransmit);
    if (HI_ERR_SUCCESS == ulRet)
    {
        HI_U8 *pucUpData = HI_NULL;
        HI_U16 usDataLen = 0;

        ulRet = mrsCcoEvtAssembleUpFrame(pstEvtTransmit, &pucUpData, &usDataLen);
        if (HI_ERR_SUCCESS == ulRet)
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
            (hi_void)memcpy_s(pstEvtItem->pucData, pstEvtItem->usDataLen, pucUpData, usDataLen);

            mrsCcoEvtEnQueue(&pstEvtCtx->stEvtQueue, pstEvtItem);
            pstEvtCtx->bACDFlg = HI_TRUE;
            pstEvtCtx->stEvtQueue.usItemNum++;
            mrsActiveEvtReport();

            mrsDfxCcoEvtQueueItemNum(pstEvtCtx->stEvtQueue.usItemNum);
            mrsDfxCcoEvtFlag(pstEvtCtx->bACDFlg);
        }
        else
        {
            ulRet = HI_ERR_FULL;
        }

        mrsToolsFree(pucUpData);
    }

    HI_DIAG_LOG_MSG_E1(MRS_FILE_LOG_FLAG_004, HI_DIAG_MT("cco resp evt to sta ret[%d]"), ulRet);

    if (pstEvtCtx->ucReportEnable == HI_FALSE)//禁止上报
    {
        return mrsCcoEvtSendToSta(pstEvtTransmit, ucMac, MRS_EVENT_FUN_REPORT_DISABLE);
    }

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
    MRS_CCO_SRV_CTX_STRU * pstCcoCtx = mrsCcoGetContext();
    P_MRS_CCO_EVT_CTX_STRU pstEvtCtx = mrsGetCcoEvtCtx();
    HI_U8 ucFilterRet = 0;

    if (!pstEvtCtx->ucReportEnable)
    {
        mrsDfxCcoEvtRxReportDisableCnt();
        return HI_ERR_SKIP;
    }

    if (PROTO_VER_3762_2009 == pstCcoCtx->ucProtoVer)
    {
        mrsDfxCcoEvtRxProtoErrCnt();
        return HI_ERR_SKIP;
    }

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

    // 06-F5: 上报从节点事件
    do
    {
        HI_U16 usPos = 0;
        HI_U16 usFrameLen = 0;
        HI_U8 *pucPayload = HI_NULL;
        HI_U8 ucPayloadLen = 0;
        HI_U8 ucProtocol = METER_PROTO_645_2007;

        if (0 == pstEvtTransmit->ulDataLen)
        {
            /*组事件报文，上报给集中器，事件位都为0 */
            pucPayload = g_aucEvtFrame698;
            ucPayloadLen = (HI_U8)sizeof(g_aucEvtFrame698);
            (hi_void)memcpy_s(pucPayload + MRS_645_FRAME_METERADD_OFFSET, HI_METER_ADDR_LEN, pstEvtTransmit->ucMeter, HI_METER_ADDR_LEN);
            pucPayload[ucPayloadLen - 2] = mrsToolsCalcCheckSum(pucPayload, ucPayloadLen - 2);
            ucProtocol = METER_PROTO_698_45;
        }
        else
        {
            pucPayload = ((HI_U8*)pstEvtTransmit) + pstEvtTransmit->ulStructLen;
            ucPayloadLen = (HI_U8)pstEvtTransmit->ulDataLen;
            ucProtocol = METER_PROTO_645_2007;
        }

        // 设备类型(1) + 规约类型(1) + 报文长度(1) + 报文(pstData->data_len)
        usDataLen = (HI_U16)(ucPayloadLen + 3);
        pucData = mrsToolsMalloc(usDataLen);
        if (!pucData)
        {
            ulRet = HI_ERR_MALLOC_FAILUE;
            break;
        }

        pucData[0] = MRS_DEVICE_TYPE_METER; // 设备类型
        pucData[1] = ucProtocol; // 规约类型
        pucData[2] = (HI_U8)ucPayloadLen;
        (hi_void)memcpy_s(pucData + 3, usDataLen - 3, pucPayload, ucPayloadLen);

        (hi_void)memset_s(&stFrame, sizeof(stFrame), 0, sizeof(stFrame));
        stFrame.ucPrm = 1;
        stFrame.ucModuleFlag = 0;
        stFrame.ucIs645 = 0;
        stFrame.ucSeq = ++(pstCtx->ucSeq);

        ulRet = mrsFind645Frame(pucData + 3, ucPayloadLen, &usPos, &usFrameLen);
        if (HI_ERR_SUCCESS == ulRet)
        {
            (hi_void)memcpy_s(stFrame.ucSrcAdd, sizeof(stFrame.ucSrcAdd), (pucData + 3) + (usPos + 1), HI_METER_ADDR_LEN);
        }
        else
        {
            HI_U8 aucAddr[HI_METER_ADDR_LEN] = {0};
            ulRet = mrsFind698MeterAddr(pucData + 3, ucPayloadLen, aucAddr);
            if (HI_ERR_SUCCESS == ulRet)
            {
                (hi_void)memcpy_s(stFrame.ucSrcAdd, sizeof(stFrame.ucSrcAdd), aucAddr, HI_METER_ADDR_LEN);
            }
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
        pstCcoItem->ucAfn = 0x06;
        pstCcoItem->usFn = 0x05;
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

#endif
