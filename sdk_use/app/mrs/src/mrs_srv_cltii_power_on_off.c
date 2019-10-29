//*****************************************************************************
//
//                  版权所有 (C), 2017-2019, 华为技术有限公司
//
//*****************************************************************************
//  文 件 名   : mrs_srv_cltii_power_on_off.c
//  版 本 号   : V1.0
//  作    者   : f00209182/00209182
//  生成日期   : 2018-4-02
//  功能描述   : 采集器停电上电上报处理
//
//  函数列表   :
//  修改历史   :
//  1.日    期 :
//    作    者 :
//    修改内容 :
//
//*****************************************************************************
#include "mrs_common.h"
#include "mrs_cmd_msg.h"
#include "mrs_dfx.h"
#include "mrs_fw_proto376_2_frame.h"
#include "mrs_fw_proto376_2_echo.h"
#include "mrs_fw_log.h"
#include "mrs_srv_common.h"
#include "mrs_srv_cco_macmap.h"
#include "mrs_srv_parallel_cco.h"
#include "mrs_power_failure.h"
#include "mrs_fw_n.h"
#include "mrs_fw_nv.h"
#include "mrs_srv_cco.h"
#include "mrs_srv_cco_event.h"
#include "mrs_fw_tools.h"
#include "mrs_srv_cltii_power_on_off.h"
#include "mrs_srv_sta_queue.h"
#include "mrs_srv_sta_baudrate.h"
#include "mrs_srv_parallel_sta.h"
#include "mrs_srv_sta_event.h"
#include "mrs_srv_sta.h"
#include "mrs_srv_sta_searchmeter.h"
#include "mrs_srv_cltii_searchmeter.h"
#include "mrs_time.h"

#if defined(PRODUCT_CFG_PRODUCT_TYPE_CCO) || defined(PRODUCT_CFG_PRODUCT_TYPE_STA)
#if defined(PRODUCT_CFG_PRODUCT_TYPE_STA)
HI_U32 mrsCltPowerOffGetDI(MRS_STA_SEARCH_CTX* pstCtx, NV_APP_MRS_SM_NODE* pstNode,
                                      HI_U8 ucProtocol);
#endif

POWER_INFO_CLT_INFO_S g_clt_power_info;

/************************************************************
                   本文件定义的全局变量
************************************************************/
HI_VOID mrsPoweOffOnInit(HI_VOID)
{
    (hi_void)memset_s(&g_clt_power_info, sizeof(g_clt_power_info), 0, sizeof(g_clt_power_info));
    mrsSrvInitList(&g_clt_power_info.stPowerOffList);
#if defined(PRODUCT_CFG_PRODUCT_TYPE_CCO)
    mrsSrvInitList(&g_clt_power_info.stPowerOnList);
#else
    mrsCltPowerSetTurnEndFlag(HI_TRUE);
#endif
}

POWER_INFO_CLT_INFO_S* mrsGetCltPowerInfo(HI_VOID)
{
    return &g_clt_power_info;
}

HI_U32 mrsCltPowerOffOnPlcProc(HI_IN MRS_EVENT_INFO_STRU* pstRcvEvtInfo, HI_U8* pMac,
                                             HI_BOOL bLocalBcFlag)
{
    HI_U32 ret = HI_ERR_SUCCESS;
    HI_U16 usMeterNum = 0;

    (hi_void)memcpy_s(&usMeterNum, sizeof(HI_U16), &pstRcvEvtInfo->ucData[METER_NUM_POSITION], sizeof(HI_U16));
    HI_DIAG_LOG_BUF(MRS_FILE_LOG_BUF_1002, HI_DIAG_MT("rx clt up pw off plc data"),
                    pstRcvEvtInfo->ucData, HI_MIN(pstRcvEvtInfo->ulDataLen, 200));
    HI_DIAG_LOG_MSG_E1(MRS_FILE_LOG_FLAG_066, HI_DIAG_MT("[rx clt up pw off plc data len]"),
                       pstRcvEvtInfo->ulDataLen);
    if (pstRcvEvtInfo->ulDataLen != (DATA_CLT_FIX_SIZE + usMeterNum * DATA_CLT_SINGLE_METER_SIZE)) {
#if defined(PRODUCT_CFG_PRODUCT_TYPE_CCO)
        mrsDfxCcoPowerRxLenErrCnt();
#else
        mrsDfxStaPowerRxPlcLenErrCnt();
#endif
        return HI_ERR_BAD_FRAME_SIZE;
    }

#if defined(PRODUCT_CFG_PRODUCT_TYPE_STA)
    if (MRS_CLT_POWER_OFF_TYPE != pstRcvEvtInfo->ucData[EVT_TYPE_POSITION]) {
        return HI_ERR_SKIP;
    } else {
        mrsDfxStaRxPlcCltPowerOffCnt();
    }
#else
    mrsCcoRespCltPowerOffOn(pMac, pstRcvEvtInfo, bLocalBcFlag);
#endif

    if (pstRcvEvtInfo->ucData[EVT_TYPE_POSITION] == MRS_CLT_POWER_ON_TYPE)
    {
        return HI_ERR_SKIP;
    }

    mrsGetCltOthersPowerFrameData((HI_U8*)pstRcvEvtInfo->ucData, usMeterNum);

#if defined(PRODUCT_CFG_PRODUCT_TYPE_CCO)
    if (mrsCltPowerGetSendFlag() == HI_FALSE) {
        mrsCltPowerSetSendFlag(HI_TRUE);
        MRS_StartTimer(MRS_CCO_TIMER_CLT_POWER_REPORT, MRS_CCO_TIME_CLT_POWER_REPORT_VALUE,
                       HI_SYS_TIMER_ONESHOT);
    }
#endif

    return ret;
}

HI_U32 mrsGetCltOthersPowerFrameData(HI_U8* pData, HI_U16 usMeterNum)
{
    HI_U8 i = 0;
    HI_U16 usOffset = DATA_CLT_FIX_SIZE;

    HI_DIAG_LOG_MSG_E1(MRS_FILE_LOG_FLAG_064, HI_DIAG_MT("[mrsGetCltOthersPowerFrameData]usMeterNum"),
                       usMeterNum);
    for (i = 0; i < usMeterNum; i++) {
        HI_U8* pBuf = pData + usOffset;

        //将采集器信息添加到链表中
        mrsCltOtherAddPowerList(&pBuf[DATA_METER_POSITION], pBuf[DATA_STATUS_POSITION],
                                pData[EVT_TYPE_POSITION]);
        usOffset += DATA_CLT_SINGLE_METER_SIZE;
    }

    return HI_ERR_SUCCESS;
}

HI_VOID mrsCltOtherAddPowerList(HI_U8 pucMeter[], HI_U8 ucStatus, HI_U8 ucPowerType)
{
    P_POWER_INFO_CLT_ITEM pstCltItem = HI_NULL;
    HI_U16 usItemSize = sizeof(POWER_INFO_CLT_ITEM);
    POWER_INFO_CLT_INFO_S* pstCltPowerInfo = mrsGetCltPowerInfo();

    //查询当前采集器信息中是否已经存在
    if (HI_TRUE == mrsCltIsExistPowerList(pucMeter, ucPowerType)) {
        return;
    }

    pstCltItem = (P_POWER_INFO_CLT_ITEM)mrsToolsMalloc(usItemSize);
    if (!pstCltItem) {
        return;
    }

    (hi_void)memset_s(pstCltItem, usItemSize, 0, usItemSize);
    (hi_void)memcpy_s(pstCltItem->aucMeter, HI_METER_ADDR_LEN, pucMeter, HI_METER_ADDR_LEN);
    pstCltItem->ucStatus = ucStatus;
#if defined(PRODUCT_CFG_PRODUCT_TYPE_CCO)
    pstCltItem->ulJoinTime = HI_MDM_GetSeconds();
#endif

    if (MRS_CLT_POWER_OFF_TYPE == ucPowerType) {
        mrsSrvListInsTail(&pstCltPowerInfo->stPowerOffList, &pstCltItem->link);
        pstCltPowerInfo->usOffMeterNum++;
        HI_DIAG_LOG_MSG_E3(MRS_FILE_LOG_FLAG_013, HI_DIAG_MT("cco/sta rx off clt add list"),
                           pstCltPowerInfo->usOffMeterNum, pucMeter[0], pucMeter[1]);
        HI_DIAG_LOG_BUF(MRS_FILE_LOG_BUF_1010, HI_DIAG_MT("cco/sta rx off clt add list addr"),
                        pucMeter, 6);
#if defined(PRODUCT_CFG_PRODUCT_TYPE_CCO)
        mrsDfxCcoCurrPowerMeterCnt(ucPowerType, pstCltPowerInfo->usOffMeterNum);
#else
        mrsDfxStaCurrPowerOffMeterCnt(pstCltPowerInfo->usOffMeterNum);
#endif
    }
#if defined(PRODUCT_CFG_PRODUCT_TYPE_CCO)
    else if (MRS_CLT_POWER_ON_TYPE == ucPowerType) {
        mrsSrvListInsTail(&pstCltPowerInfo->stPowerOnList, &pstCltItem->link);
        pstCltPowerInfo->usOnMeterNum++;
        mrsDfxCcoCurrPowerMeterCnt(ucPowerType, pstCltPowerInfo->usOnMeterNum);
    }
#endif
    else {
    }
}

MRS_SRV_LIST* mrsGetCltPowerList(POWER_INFO_CLT_INFO_S* pstCltPowerInfo, HI_U8 ucPowerType)
{
    MRS_SRV_LIST* list = HI_NULL;

    if (MRS_CLT_POWER_OFF_TYPE == ucPowerType) {
        list = &pstCltPowerInfo->stPowerOffList;
    }
#if defined(PRODUCT_CFG_PRODUCT_TYPE_CCO)
    else if (MRS_CLT_POWER_ON_TYPE == ucPowerType) {
        list = &pstCltPowerInfo->stPowerOnList;
    }
#endif

    return list;
}

HI_BOOL mrsCltIsExistPowerList(HI_U8 pucMeter[], HI_U8 ucPowerType)
{
    POWER_INFO_CLT_INFO_S* pstCltPowerInfo = mrsGetCltPowerInfo();
    MRS_SRV_LIST* list = HI_NULL;
    MRS_SRV_LIST* it, *next;
#if defined(PRODUCT_CFG_PRODUCT_TYPE_CCO)
    HI_U32 ulCurrTime = HI_MDM_GetSeconds();
#endif

    list = mrsGetCltPowerList(pstCltPowerInfo, ucPowerType);
    if (!list) {
        return HI_FALSE;
    }

    it = list->next;
    while (it != list) {
        P_POWER_INFO_CLT_ITEM pstCltItem = (P_POWER_INFO_CLT_ITEM)it;

        next = it->next;
        if (0 == memcmp(pucMeter, pstCltItem->aucMeter, HI_METER_ADDR_LEN)) {
#if defined(PRODUCT_CFG_PRODUCT_TYPE_CCO)
            if ((ulCurrTime - pstCltItem->ulJoinTime) <= MRS_CCO_POWER_ITEM_OLD_TIME)
#endif
            {
#if defined(PRODUCT_CFG_PRODUCT_TYPE_CCO)
                mrsDfxCcoRxFilterPowerMeterCnt(ucPowerType);
#else
                mrsDfxStaFilterPowerOffMeterCnt();
#endif
                return HI_TRUE;
            }
#if defined(PRODUCT_CFG_PRODUCT_TYPE_CCO)
            else {
                HI_DIAG_LOG_MSG_E3(MRS_FILE_LOG_FLAG_012, HI_DIAG_MT("cco rx clt evt exist timeout"),
                                   ucPowerType, pucMeter[0], pucMeter[1]);
            }
#endif
        }

        it = next;
    }

    return HI_FALSE;
}

HI_U16 mrsCltPowerOffGetMeterNum(HI_VOID)
{
    return g_clt_power_info.usOffMeterNum;
}

HI_BOOL mrsCltPowerGetSendFlag(HI_VOID)
{
    return g_clt_power_info.is_send_flag;
}

HI_VOID mrsCltPowerSetSendFlag(HI_BOOL bSendFlag)
{
    g_clt_power_info.is_send_flag = bSendFlag;
}

#if defined(PRODUCT_CFG_PRODUCT_TYPE_STA)
HI_BOOL mrsCltPowerGetTurnEndFlag(HI_VOID)
{
    return (HI_BOOL)g_clt_power_info.turn_end_flag;
}

HI_VOID mrsCltPowerSetTurnEndFlag(HI_BOOL bTurnEndFlag)
{
    g_clt_power_info.turn_end_flag = bTurnEndFlag;
}

HI_U8 mrsCltGetPowerStatus(HI_VOID)
{
    return g_clt_power_info.stMeterPowerInfo.ucPowerStatus;
}

HI_VOID mrsCltSetPowerStatus(HI_U8 ucStatus)
{
    g_clt_power_info.stMeterPowerInfo.ucPowerStatus = ucStatus;
    mrsDfxStaCurrPowerStatus(ucStatus);
}

HI_U16 mrsCltGetPowerCurrPlcSeq(HI_VOID)
{
    return g_clt_power_info.stMeterPowerInfo.usCurrPlcSeq;
}

HI_VOID mrsCltSetPowerCurrPlcSeq(HI_U16 usPlcSeq)
{
    g_clt_power_info.stMeterPowerInfo.usCurrPlcSeq = usPlcSeq;
}

HI_U8 mrsCltGetPowerReportNum(HI_VOID)
{
    return g_clt_power_info.stMeterPowerInfo.ucReportNum;
}

HI_VOID mrsCltResetPowerReportNum(HI_VOID)
{
    g_clt_power_info.stMeterPowerInfo.ucReportNum = 0;
}

HI_VOID mrsCltResetPowerMeterBitMap(HI_VOID)
{
    g_clt_power_info.stMeterPowerInfo.ulMeterBitMap = 0;
}

HI_VOID mrsCltResetPowerReportBitMap(HI_VOID)
{
    g_clt_power_info.stMeterPowerInfo.ulReportBitMap = 0;
}

HI_U8 mrsCltGetPowerPlcSendTimes(HI_VOID)
{
    return g_clt_power_info.stMeterPowerInfo.ucPlcSendTimes;
}

HI_VOID mrsCltPowerPlcSendTimesAdd(HI_VOID)
{
    g_clt_power_info.stMeterPowerInfo.ucPlcSendTimes++;
    mrsDfxCltPowerOnTxPlcTimes();
}

HI_VOID mrsCltPowerResetPlcSendTimes(HI_VOID)
{
    g_clt_power_info.stMeterPowerInfo.ucPlcSendTimes = 0;
}

HI_U8 mrsCltPowerOnGetSendPlcFlag(HI_VOID)
{
    return g_clt_power_info.stMeterPowerInfo.ucSendPlcWaitFlag;
}

HI_VOID mrsCltPowerOnSetSendPlcFlag(HI_BOOL bWaitSendFlag)
{
    g_clt_power_info.stMeterPowerInfo.ucSendPlcWaitFlag = bWaitSendFlag;
}

HI_U8 mrsCltPowerOffGetPlcAckFlag(HI_VOID)
{
    return g_clt_power_info.stMeterPowerInfo.ucWaitPlcAckFlag;
}

HI_VOID mrsCltPowerOffSetPlcAckFlag(HI_BOOL bWaitPlcAckFlag)
{
    g_clt_power_info.stMeterPowerInfo.ucWaitPlcAckFlag = bWaitPlcAckFlag;
}

HI_U16 mrsCltGetPowerOtherPlcSeq(HI_VOID)
{
    return g_clt_power_info.stMeterPowerInfo.usOtherPlcSeq;
}

HI_VOID mrsCltSetPowerOtherPlcSeq(HI_U16 usPlcSeq)
{
    g_clt_power_info.stMeterPowerInfo.usOtherPlcSeq = usPlcSeq;
}

HI_U16 mrsCltGetPlcDataSize(POWER_INFO_CLT_INFO_S* pstCltInfo, HI_OUT HI_U16* pusNum)
{
    HI_U16 usNum = 0;
    MRS_SRV_LIST* list;
    MRS_SRV_LIST* it, *next;
    HI_U16 usDataLen = DATA_CLT_FIX_SIZE;

    list = &pstCltInfo->stPowerOffList;
    it = list->next;
    HI_DIAG_LOG_MSG_E1(MRS_FILE_LOG_FLAG_028, HI_DIAG_MT("mrsCltGetPlcDataSize OffGetMeterNum"),
                       mrsCltPowerOffGetMeterNum());
    while (it != list) {
        P_POWER_INFO_CLT_ITEM pstItem = (P_POWER_INFO_CLT_ITEM)it;

        next = it->next;
        if ((usDataLen + DATA_CLT_SINGLE_METER_SIZE) > CLT_POWER_INFO_DATA_LEN_MAX) {
            break;
        }

        if (pstItem->send_turn >= mrsPowerFailureGetCurrTurn()) {
            it = next;
            continue;
        }

        usDataLen += DATA_CLT_SINGLE_METER_SIZE;
        usNum++;
        it = next;
    }

    *pusNum = usNum;
    HI_DIAG_LOG_MSG_E2(MRS_FILE_LOG_FLAG_016, HI_DIAG_MT("mrsCltGetPlcDataSize usNum usDataLen"),
                       usNum, usDataLen);

    return usDataLen;
}

HI_VOID mrsCltSetPowerOffData(HI_U8* pData, POWER_INFO_CLT_INFO_S* pstCltInfo,
                                          HI_U16 usSendMeterNum)
{
    HI_U16 usOffset = 0;
    MRS_SRV_LIST* list;
    MRS_SRV_LIST* it, *next;

    pData[usOffset++] = MRS_CLT_POWER_OFF_TYPE;
    (hi_void)memcpy_s(&pData[usOffset], sizeof(HI_U16), &usSendMeterNum, sizeof(HI_U16));
    usOffset += sizeof(HI_U16);
    list = &pstCltInfo->stPowerOffList;
    it = list->next;
    while (it != list) {
        P_POWER_INFO_CLT_ITEM pstCltItem = (P_POWER_INFO_CLT_ITEM)it;

        next = it->next;
        if (0 == usSendMeterNum) {
            break;
        }

        if (pstCltItem->send_turn >= mrsPowerFailureGetCurrTurn()) {
            it = next;
            continue;
        }

        pstCltItem->send_turn++;

        int ret = memcpy_s(&pData[usOffset], HI_METER_ADDR_LEN, pstCltItem->aucMeter, HI_METER_ADDR_LEN);
        usOffset += HI_METER_ADDR_LEN;
        pData[usOffset++] = pstCltItem->ucStatus;
        usSendMeterNum--;
        it = next;
        if(ret != 0){
            continue;
        }
    }

    if (it == list) {
        mrsCltPowerSetTurnEndFlag(HI_TRUE);
    }
}

HI_VOID mrsPowerOffFreeCltListNode(HI_VOID* pstNode, HI_VOID* pstCb)
{
    MRS_NOT_USED(pstCb);

    if (pstNode) {
        mrsToolsFree(pstNode);
    }
}

HI_U32 mrsCltSendPowerOffFrameProc(HI_VOID)
{
    HI_U32 ret = HI_ERR_SUCCESS;
    P_MRS_STA_EVT_CTX pstCtx = mrsStaGetEvtCtx();
    POWER_INFO_CLT_INFO_S* pstCltInfoList = mrsGetCltPowerInfo();
    HI_U16 usSendMeterNum = 0;
    HI_U16 usDataLen = 0;
    HI_U8* pucPayload = HI_NULL;
    HI_U16 usPayloadLen = 0;
    POWER_FAILURE_RCV_MAC_REPORT_S* pstDeviceStateInfo = HI_NULL;
    MRS_EVENT_INFO_STRU* pstEvtInfo = HI_NULL;
    MRS_PLC_FRAME_DATA_S* pstPowerPlcFrame = HI_NULL;

    usDataLen = mrsCltGetPlcDataSize(pstCltInfoList, &usSendMeterNum);
    if ((0 == usDataLen) || (0 == usSendMeterNum)) {
        return HI_ERR_FAILURE;
    }

    usPayloadLen = sizeof(MRS_EVENT_INFO_STRU) + usDataLen;
    pucPayload = (HI_U8*)mrsToolsMalloc(usPayloadLen);
    if (HI_NULL == pucPayload) {
        return HI_ERR_MALLOC_FAILUE;
    }

    (hi_void)memset_s(pucPayload, usPayloadLen, 0, usPayloadLen);
    mrsCltPowerSetTurnEndFlag(HI_FALSE);
    mrsCltSetPowerOffData(pucPayload + sizeof(MRS_EVENT_INFO_STRU), pstCltInfoList, usSendMeterNum);

    //填充帧中的固定部分
    pstEvtInfo = (MRS_EVENT_INFO_STRU*)(pucPayload);
    pstEvtInfo->ulInterfaceVer = MRS_EVENT_INF_UP_VER;
    pstEvtInfo->ulStructLen = sizeof(MRS_EVENT_INFO_STRU);
    pstEvtInfo->ulDirBit = MRS_PLC_UP_FLG;
    pstEvtInfo->ulFrmBit = MRS_EVENT_FRM_BIT_FROM;
    pstEvtInfo->ulFunCode = MRS_EVENT_FUN_REPORT_CLT;
    pstEvtInfo->ulDataLen = (HI_U32)usDataLen;

    pstCtx->usSeq++;
    mrsDfxStaEvtReportSeq(pstCtx->usSeq);
    pstEvtInfo->usSeq = pstCtx->usSeq;
    pstPowerPlcFrame = (MRS_PLC_FRAME_DATA_S*)mrsToolsMalloc(sizeof(MRS_PLC_FRAME_DATA_S));
    if (!pstPowerPlcFrame) {
        mrsToolsFree(pucPayload);
        return HI_ERR_MALLOC_FAILUE;
    }
    (hi_void)memset_s(pstPowerPlcFrame, sizeof(MRS_PLC_FRAME_DATA_S), 0, sizeof(MRS_PLC_FRAME_DATA_S));
    pstPowerPlcFrame->usId = ID_MRS_CMD_EVENT_INF_PLC;
    pstPowerPlcFrame->pucPayload = pucPayload;
    pstPowerPlcFrame->usPayloadLen = usPayloadLen;
    //如果本模块停电，填充地址为本地广播地址
    pstDeviceStateInfo = mrsGetPowerFailureMacInfo();
    if (HI_TRUE == pstDeviceStateInfo->device_power_failure) {
        pstPowerPlcFrame->ucMac[0] = 0;
        (hi_void)memset_s(pstPowerPlcFrame->ucMac + 1, HI_PLC_MAC_ADDR_LEN - 1, 0xFF,
                          HI_PLC_MAC_ADDR_LEN - 1);
        //填充信息后发送,多次发送，在内部将全局变量清零
        ret = mrsCltSendPowerOffPlcFrame(pstPowerPlcFrame);

        mrsToolsFree(pucPayload);
        mrsToolsFree(pstPowerPlcFrame);

        return ret;
    } else {
        ret = mrsCltSendOtherPowerFrame(pstPowerPlcFrame, pstEvtInfo->usSeq);
        mrsDfxStaTxOtherPowerOffPlcCnt();
        HI_DIAG_LOG_MSG_E0(MRS_FILE_LOG_FLAG_055, HI_DIAG_MT("[PCO-MR-PW-clt sendover]"));
    }

    return ret;
}

HI_U32 mrsCltPowerOffPlcTimeout(HI_VOID)
{
    NV_APP_MRS_EVT_CFG_STRU* pstPowerFailureNvCtrl = mrsGetPowerFailureNvCtrl();
    POWER_FAILURE_CTRL_S* powerFailureCtrlInfo = mrsGetPowerFailureCtrlInfo();
    POWER_INFO_CLT_INFO_S* pstCltInfo = mrsGetCltPowerInfo();

    powerFailureCtrlInfo->sta_send_times++;
    mrsDfxStaCurrPowerOffPlcTxTurn(powerFailureCtrlInfo->sta_send_times);
    HI_DIAG_LOG_MSG_E2(MRS_FILE_LOG_FLAG_019, HI_DIAG_MT("sta_send_times TurnEndFlag"),
                       powerFailureCtrlInfo->sta_send_times, mrsCltPowerGetTurnEndFlag());
    if (mrsPowerFailureGetCurrTurn() > pstPowerFailureNvCtrl->ucSendTimes) {
        if (HI_TRUE == mrsCltPowerGetTurnEndFlag()) {
            mrsCltPowerOffSendOtherEnd();
        } else {
            mrsCltPowerOffOtherFrameFree(&pstCltInfo->stMeterPowerInfo);
            mrsCltPowerOffSetPlcAckFlag(HI_FALSE);
            powerFailureCtrlInfo->sta_send_times = 1;
            mrsStaPowerFailureFrameProc();
        }

        return HI_ERR_SUCCESS;
    }

    if (pstCltInfo->stMeterPowerInfo.pstOtherReportData) {
        MRS_SendFrameProc(pstCltInfo->stMeterPowerInfo.pstOtherReportData);
        MRS_StartTimer(MRS_STA_TIMER_CLT_SEND_PLC, pstPowerFailureNvCtrl->usSendInterval,
                       HI_SYS_TIMER_ONESHOT);
        mrsCltPowerOffSetPlcAckFlag(HI_TRUE);
        mrsDfxStaTxOtherPowerOffPlcTryCnt();
    }

    return HI_ERR_SUCCESS;
}

HI_U32 mrsCltSendOtherPowerFrame(MRS_PLC_FRAME_DATA_S* pstReportData, HI_U16 usSeq)
{
    NV_APP_MRS_EVT_CFG_STRU* pstPowerFailureNvCtrl = mrsGetPowerFailureNvCtrl();
    POWER_INFO_CLT_INFO_S* pstCltInfo = mrsGetCltPowerInfo();

    //本地模块未停电，不填充地址，不需要上下文,直接发送出去
    MRS_SendFrameProc(pstReportData);
    MRS_StartTimer(MRS_STA_TIMER_CLT_SEND_PLC, pstPowerFailureNvCtrl->usSendInterval, HI_SYS_TIMER_ONESHOT);
    mrsCltPowerOffSetPlcAckFlag(HI_TRUE);
    mrsCltSetPowerOtherPlcSeq(usSeq);
    pstCltInfo->stMeterPowerInfo.pstOtherReportData = pstReportData;

    return HI_ERR_SUCCESS;
}

HI_VOID mrsCltPowerOffSendOtherEnd(HI_VOID)
{
    POWER_INFO_CLT_INFO_S* pstCltPowerInfo = mrsGetCltPowerInfo();
    POWER_FAILURE_CTRL_S* powerFailureCtrl = mrsGetPowerFailureCtrlInfo();
    HI_U16 usNum = 0;
    HI_U16 usDataLen = 0;

    MRS_StopTimer(MRS_STA_TIMER_CLT_SEND_PLC);
    mrsCltPowerOffSetPlcAckFlag(HI_FALSE);
    //本模块未停电时，在调用mrsStaPowerFailureFrameProc前需要将sta_send_times置为1
    powerFailureCtrl->sta_send_times = 1;
    mrsDfxStaCurrPowerOffPlcTxTurn(powerFailureCtrl->sta_send_times);
    mrsCltPowerOffOtherFrameFree(&pstCltPowerInfo->stMeterPowerInfo);

    usDataLen = mrsCltGetPlcDataSize(pstCltPowerInfo, &usNum);
    ////还有点没有完成上报
    if (usDataLen != 0 && usNum != 0) {
        mrsStaPowerFailureFrameProc();
    } else {
        mrsSrvListClear(&pstCltPowerInfo->stPowerOffList, mrsPowerOffFreeCltListNode, HI_NULL);
        mrsCltPowerSetSendFlag(HI_FALSE);
        powerFailureCtrl->isRedayToSendPowerFailure = HI_FALSE;
        pstCltPowerInfo->usOffMeterNum = 0;
        mrsDfxStaCurrPowerOffMeterCnt(pstCltPowerInfo->usOffMeterNum);
    }
}

HI_VOID mrsCltPowerOffOtherFrameFree(MRS_POWER_METER_INFO_STRU* pstMeterPowerInfo)
{
    if (pstMeterPowerInfo->pstOtherReportData) {
        mrsToolsFree(pstMeterPowerInfo->pstOtherReportData->pucPayload);
    }

    mrsToolsFree(pstMeterPowerInfo->pstOtherReportData);
}

HI_U32 mrsCltSendPowerOffPlcFrame(MRS_PLC_FRAME_DATA_S* pstReportData)
{
    HI_U32 ret = HI_ERR_SUCCESS;
    NV_APP_MRS_EVT_CFG_STRU* pstPowerFailureNvCtrl = mrsGetPowerFailureNvCtrl();
    POWER_INFO_CLT_INFO_S* pstCltPowerInfo = mrsGetCltPowerInfo();

    ret = MRS_SendFrameProc(pstReportData);

    if (mrsPowerFailureGetCurrTurn() < pstPowerFailureNvCtrl->ucSendTimes) {
        if (HI_TRUE == mrsCltPowerGetTurnEndFlag()) {
            MRS_StartTimer(MRS_STA_TIMER_POWERFAILURE, pstPowerFailureNvCtrl->usSendInterval,
                           HI_SYS_TIMER_ONESHOT);
        } else {
            MRS_StartTimer(MRS_STA_TIMER_POWERFAILURE, MRS_CLT_FRAME_SEND_INTERVAL, HI_SYS_TIMER_ONESHOT);
        }
    } else if (mrsPowerFailureGetCurrTurn() == pstPowerFailureNvCtrl->ucSendTimes) {
        if (HI_FALSE == mrsCltPowerGetTurnEndFlag()) {
            MRS_StartTimer(MRS_STA_TIMER_POWERFAILURE, MRS_CLT_FRAME_SEND_INTERVAL, HI_SYS_TIMER_ONESHOT);
        } else {
            mrsSrvListClear(&pstCltPowerInfo->stPowerOffList, mrsPowerOffFreeCltListNode, HI_NULL);
        }
    } else {
        mrsSrvListClear(&pstCltPowerInfo->stPowerOffList, mrsPowerOffFreeCltListNode, HI_NULL);
    }

    mrsDfxStaPlcEvtReportTimeStamp(HI_MDM_GetMilliSeconds());

    return ret;
}

HI_U32 mrsCltiiHandleLocalPowerOff(HI_VOID)
{
    MRS_STA_SEARCH_CTX* pstCtx = mrsStaSearchCtx();
    NV_APP_MRS_EVT_CFG_STRU* powerFailureNvCtrl = mrsGetPowerFailureNvCtrl();
    HI_U8 power_status = mrsCltGetPowerStatus();

    if (MRS_SEARCH_STATUS_IDLE != mrsStaSearchGetStatus()) {
        mrsCltiiSearchStop(pstCtx);
    }

    mrsStaSearchLoadNvResult(&pstCtx->stNvMeterList);
    mrsStaSearchSetStatus(MRS_SEARCH_STATUS_POWER_OFF_RM);
    if ((power_status >= MRS_CLT_POWER_ON_WAIT_STATUS) && 
        (power_status <= MRS_CLT_POWER_ON_PLC_SEND_STATUS)) {
        mrsCltPowerOnEnd();
    } else {
        mrsCltResetPowerMeterBitMap();
        mrsCltResetPowerReportBitMap();
    }

    mrsCltSetPowerStatus(MRS_CLT_POWER_ON_TO_OFF_STATUS);
    MRS_StartTimer(MRS_STA_TIMER_CLT_POWEROFF_WAIT_METER, powerFailureNvCtrl->ucPowerOffMeterTimeDelay * 100,
                   HI_SYS_TIMER_ONESHOT);

    return HI_ERR_SUCCESS;
}

//仅读取连续搜不到次数为0的电表
HI_U32 mrsCltiiPowerOffRm(HI_VOID)
{
    POWER_INFO_CLT_INFO_S* pstCltInfoList = mrsGetCltPowerInfo();
    MRS_POWER_METER_INFO_STRU* pstPowerInfo = &pstCltInfoList->stMeterPowerInfo;
    MRS_STA_SEARCH_CTX* pstCtx = mrsStaSearchCtx();
    MRS_SEARCH_NV_METERLIST_STRU* pstNvList = HI_NULL;
    NV_APP_MRS_SM_NODE* pstNode = HI_NULL;
    HI_U8 ucIndex = 0;
    HI_U8 ucProtocol = 0;
    HI_U32 ulDI = 0;

    if (!pstCtx) {
        return HI_ERR_FAILURE;
    }

    pstNvList = &pstCtx->stNvMeterList;
    for (ucIndex = pstNvList->ucDetectCount; ucIndex < pstNvList->ucTotal; ucIndex++) {
        pstNode = &pstNvList->astNvMeter[ucIndex];
        ucProtocol = mrsStaSearchGetNodeProtocol(pstNode);
        if ((0 == mrsStaSearchGetNotFoundTimes(pstNode))
            && ((ucProtocol >= METER_PROTO_645_1997) && (ucProtocol <= METER_PROTO_698_45))) {
            //目前支持97、07和698.45协议
            pstPowerInfo->ulReportBitMap |= 1 << ucIndex;
            break;
        }
    }

    if (!pstNode || (ucIndex == pstNvList->ucTotal)) {
        if (pstPowerInfo->ulReportBitMap) {
            mrsLocalPowerFailureWaitSend();
        }
        return HI_ERR_SKIP;
    }

    pstNvList->ucDetectCount = ucIndex;
    pstNvList->ucDetectProtocol = ucProtocol;
    pstNvList->ucBaudrateIndex = mrsCltiiSearchGetNodeBaudRateIndex(pstNode->ucParam);

    ulDI = mrsCltPowerOffGetDI(pstCtx, pstNode, ucProtocol);
    mrsCltiiInitSearchFrame(pstCtx, ulDI, ucProtocol);
    mrsCltiiCreateSearchFrame(pstCtx, pstNode->aucAddr, ucProtocol);
    mrsCltiiSearchResetRunning(pstCtx);

    return mrsCltiiRmSendFrame(pstCtx->stTx.aucBuf, (HI_U16)pstCtx->stTx.ulSize,
                               MRS_STA_ITEM_ID_RM_CLT_II, ucProtocol);
}

HI_U32 mrsCltPowerOffGetDI(MRS_STA_SEARCH_CTX* pstCtx, NV_APP_MRS_SM_NODE* pstNode,
                                      HI_U8 ucProtocol)
{
    MRS_SEARCH_CFG_STRU* pstCfg = &(pstCtx->stSearchCfg);

    switch (ucProtocol) {
        case METER_PROTO_645_1997:
            return MRS_CLT_POWER_OFF_97_DI_10;

        default:
            return (pstCfg->ulDI07);
    }
}

HI_U32 mrsCltPowerOff97FrameProc(HI_U8* pData, HI_U16 ucLen)
{
    HI_U16 us97DI = MRS_CLT_POWER_OFF_97_DI_1F;

    (hi_void)memcpy_s(pData + MRS_SM_POS_H + MRS_SM_POS_DI,
             ucLen - MRS_SM_POS_H - MRS_SM_POS_DI, &us97DI, sizeof(us97DI));
    mrs645DataEncode(pData + MRS_SM_POS_H + MRS_SM_POS_DI, sizeof(HI_U16));
    *(pData + ucLen - 2) = mrsToolsCalcCheckSum(pData + MRS_SM_POS_H, (HI_U16)(ucLen - MRS_SM_POS_H - 2));

    return HI_ERR_SUCCESS;
}

HI_U32 mrsCltiiRmSendFrame(HI_U8* pBuf, HI_U16 usLen, HI_U16 usId, HI_U8 ucProtocol)
{
    MRS_STA_SRV_CTX_STRU* pstSta = mrsStaGetContext();
    MRS_STA_ITEM* pstItem = HI_NULL;

    pstItem = (MRS_STA_ITEM*)mrsToolsMalloc(sizeof(MRS_STA_ITEM) + usLen);
    if (!pstItem) {
        return HI_ERR_NOT_ENOUGH_MEMORY;
    }

    (hi_void)memset_s(pstItem, sizeof(MRS_STA_ITEM) + usLen, 0, sizeof(MRS_STA_ITEM) + usLen);

    pstItem->id = usId;
    pstItem->buf = (HI_U8*)pstItem + sizeof(MRS_STA_ITEM);
    pstItem->len = usLen;
    pstItem->from = MRS_STA_QUEUE_FROM_STA;
    (hi_void)memcpy_s(pstItem->buf, usLen, pBuf, usLen);
    pstItem->time_out = (HI_U8)MRS_MS_TO_100MS(mrsGet645FrameHeadTimeout(MRS_STA_RM_CFG_LOCAL));
    pstItem->bAutoStrategy = HI_TRUE;
    pstItem->ucFrameTimeout = (HI_U8)MRS_MS_TO_100MS(mrsGet645FrameInterval(MRS_STA_RM_CFG_LOCAL));
    pstItem->ucProtocol = ucProtocol;

    if (!mrsStaTryEnQueue(&pstSta->stQueModule, pstItem)) {
        mrsToolsFree(pstItem);
        return HI_ERR_FAILURE;
    }

    mrsStaNotifyQueue();

    return HI_ERR_SUCCESS;
}

HI_VOID mrsCltPowerOffRmRspProc(HI_BOOL bRmOkFlag)
{
    POWER_INFO_CLT_INFO_S* pstCltInfoList = mrsGetCltPowerInfo();
    MRS_STA_SEARCH_CTX* pstCtx = mrsStaSearchCtx();
    MRS_SEARCH_NV_METERLIST_STRU* pstNvList = &pstCtx->stNvMeterList;
    HI_U8 ucCurrIndex = pstNvList->ucDetectCount;

    pstCltInfoList->stMeterPowerInfo.ulMeterBitMap |= bRmOkFlag << ucCurrIndex;
    pstNvList->ucDetectCount++;
    if (pstNvList->ucDetectCount < HI_MIN(pstNvList->ucTotal, POWER_OFF_QUERY_METER_NUM)) {
        mrsCltiiPowerOffRm();
    } else {
        pstNvList->ucDetectCount = 0;
        mrsStaSearchSetStatus(MRS_SEARCH_STATUS_IDLE);
        mrsLocalPowerFailureWaitSend();
    }

    mrsCltOtherAddPowerList(pstNvList->astNvMeter[ucCurrIndex].aucAddr, bRmOkFlag, MRS_CLT_POWER_OFF_TYPE);
}

HI_VOID mrsCltPowerOffRmRxProc(HI_VOID)
{
    mrsCltPowerOffRmRspProc(MRS_CLT_METER_POWER_ON_STATUS);
}

HI_VOID mrsCltPowerOffRmTimeOutProc(HI_VOID)
{
    mrsCltPowerOffRmRspProc(MRS_CLT_METER_POWER_OFF_STATUS);
}

HI_VOID mrsCltLocalPowerOffPreProc(HI_VOID)
{
    POWER_FAILURE_CTRL_S* powerFailureCtrlInfo = mrsGetPowerFailureCtrlInfo();
    POWER_INFO_CLT_INFO_S* pstCltInfo = mrsGetCltPowerInfo();

    MRS_StopTimer(MRS_STA_TIMER_CLT_SEND_PLC);
    mrsCltPowerOffSetPlcAckFlag(HI_FALSE);
    powerFailureCtrlInfo->sta_send_times = 0;
    mrsDfxStaCurrPowerOffPlcTxTurn(powerFailureCtrlInfo->sta_send_times);
    mrsCltPowerOffOtherFrameFree(&pstCltInfo->stMeterPowerInfo);
    mrsCltPowerSetTurnEndFlag(HI_TRUE);
}

HI_U32 mrsCltPowerOnOffRcvAck(HI_U16 usSeq)
{
    HI_U32 ret = HI_ERR_SKIP;

    if (MRS_CLT_POWER_ON_PLC_SEND_STATUS == mrsCltGetPowerStatus()) {
        ret = mrsCltPowerOnRcvAck(usSeq);
    }

    if (HI_TRUE == mrsCltPowerOffGetPlcAckFlag()) {
        ret = mrsCltPowerOffRcvAck(usSeq);
    }

    return ret;
}

HI_U32 mrsCltPowerOffRcvAck(HI_U16 usSeq)
{
    POWER_FAILURE_CTRL_S* powerFailureCtrlInfo = mrsGetPowerFailureCtrlInfo();
    POWER_INFO_CLT_INFO_S* pstCltInfo = mrsGetCltPowerInfo();

    if (mrsCltGetPowerOtherPlcSeq() != usSeq) {
        return HI_ERR_SKIP;
    }

    MRS_StopTimer(MRS_STA_TIMER_CLT_SEND_PLC);
    if (HI_TRUE == mrsCltPowerGetTurnEndFlag()) {
        mrsCltPowerOffSendOtherEnd();
    } else {
        mrsCltPowerOffOtherFrameFree(&pstCltInfo->stMeterPowerInfo);
        mrsCltPowerOffSetPlcAckFlag(HI_FALSE);
        powerFailureCtrlInfo->sta_send_times = 1;
        mrsDfxStaCurrPowerOffPlcTxTurn(powerFailureCtrlInfo->sta_send_times);
        mrsStaPowerFailureFrameProc();
    }

    return HI_ERR_SUCCESS;
}

HI_VOID mrsCltiiSearchDetectNvPowerProc(HI_BOOL bResult, HI_U8 ucCurrIndex,
                                                         NV_APP_MRS_SM_NODE* pstNode)
{
    POWER_INFO_CLT_INFO_S* pstCltInfoList = mrsGetCltPowerInfo();
    MRS_POWER_METER_INFO_STRU* pstPowerInfo = &pstCltInfoList->stMeterPowerInfo;
    HI_U32 ulBitSet = 1 << ucCurrIndex;

    if (mrsCltGetPowerStatus() != MRS_CLT_POWER_ON_WAIT_STATUS) {
        return;
    }

    if (bResult) {
        if ((pstPowerInfo->ulMeterBitMap & ulBitSet) == 0) {
            pstPowerInfo->ulMeterBitMap |= ulBitSet;
            pstPowerInfo->ucReportNum++;
        }
    } else {
        if (0 == mrsStaSearchGetNotFoundTimes(pstNode) && ((pstPowerInfo->ulReportBitMap & ulBitSet) == 0)) {
            pstPowerInfo->ulReportBitMap |= ulBitSet;
            pstPowerInfo->ucReportNum++;
        }
    }

    HI_DIAG_LOG_MSG_E3(MRS_FILE_LOG_FLAG_018, HI_DIAG_MT("mrsCltiiSearchDetectNvPowerProc"),
                       pstPowerInfo->ucReportNum, bResult, ulBitSet);
}

HI_U32 mrsCltiiSearchPowerOnProc(NV_APP_MRS_SM_NODE* pstNvMeter)
{
    HI_U32 ret = HI_ERR_SUCCESS;
    POWER_INFO_CLT_INFO_S* pstCltInfoList = mrsGetCltPowerInfo();

    if (mrsCltGetPowerStatus() != MRS_CLT_POWER_ON_WAIT_STATUS) {
        return ret;
    }

    ret = mrsCltCreatePowerOnEvtFrame(pstNvMeter);
    if (ret != HI_ERR_SUCCESS) {
        mrsCltPowerOnEnd();
        return ret;
    }

    mrsCltSetPowerStatus(MRS_CLT_POWER_ON_PLC_CREATE_STATUS);

    if (pstCltInfoList->ucPowerOnFlag == HI_TRUE) {
        ret = mrsCltSendPowerOnEvtFrame();
    }

    return ret;
}

HI_U32 mrsCltSetPowerOnUpData(HI_U8* pData, NV_APP_MRS_SM_NODE* pstNvMeter)
{
    POWER_INFO_CLT_INFO_S* pstCltInfoList = mrsGetCltPowerInfo();
    MRS_POWER_METER_INFO_STRU* pstPowerInfo = &pstCltInfoList->stMeterPowerInfo;
    HI_U16 usMeterNum = (HI_U16)pstPowerInfo->ucReportNum;
    HI_U8 ucIndex = 0;
    HI_U16 usOffset = 0;

    pData[usOffset++] = MRS_CLT_POWER_ON_TYPE;
    (hi_void)memcpy_s(pData + usOffset, sizeof(HI_U16), &usMeterNum, sizeof(HI_U16));
    usOffset += sizeof(HI_U16);
    mrsDfxCltPowerOnReportMeterNum((HI_U8)usMeterNum);

    for (ucIndex = 0; ucIndex < sizeof(HI_U32) * BIT_WIZE; ucIndex++) {
        if (pstPowerInfo->ulMeterBitMap & (1 << ucIndex)) {
            (hi_void)memcpy_s(pData + usOffset, HI_METER_ADDR_LEN, pstNvMeter[ucIndex].aucAddr, HI_METER_ADDR_LEN);
            usOffset += HI_METER_ADDR_LEN;
            pData[usOffset++] = MRS_CLT_METER_POWER_ON_STATUS;
        } else if (pstPowerInfo->ulReportBitMap & (1 << ucIndex)) {
            (hi_void)memcpy_s(pData + usOffset, HI_METER_ADDR_LEN, pstNvMeter[ucIndex].aucAddr, HI_METER_ADDR_LEN);
            usOffset += HI_METER_ADDR_LEN;
            pData[usOffset++] = MRS_CLT_METER_POWER_OFF_STATUS;
        }
    }

    return HI_ERR_SUCCESS;
}

HI_U32 mrsCltCreatePowerOnEvtFrame(NV_APP_MRS_SM_NODE* pstNvMeter)
{
    HI_U32 ret = HI_ERR_SUCCESS;
    P_MRS_STA_EVT_CTX pstCtx = mrsStaGetEvtCtx();
    HI_U8* pucPayload = HI_NULL;
    HI_U16 usPayloadLen = 0;
    MRS_EVENT_INFO_STRU* pstEvtInfo = HI_NULL;
    MRS_PLC_FRAME_DATA_S* pstPlcFrame = HI_NULL;
    POWER_INFO_CLT_INFO_S* pstCltInfoList = mrsGetCltPowerInfo();
    MRS_POWER_METER_INFO_STRU* pstPowerInfo = &pstCltInfoList->stMeterPowerInfo;

    pstPlcFrame = (MRS_PLC_FRAME_DATA_S*)mrsToolsMalloc(sizeof(MRS_PLC_FRAME_DATA_S));
    if (HI_NULL == pstPlcFrame) {
        return HI_ERR_MALLOC_FAILUE;
    }

    (hi_void)memset_s(pstPlcFrame, sizeof(MRS_PLC_FRAME_DATA_S), 0, sizeof(MRS_PLC_FRAME_DATA_S));
    usPayloadLen = sizeof(MRS_EVENT_INFO_STRU) + DATA_CLT_FIX_SIZE +
                   DATA_CLT_SINGLE_METER_SIZE * pstPowerInfo->ucReportNum;
    pucPayload = (HI_U8*)mrsToolsMalloc(usPayloadLen);
    if (HI_NULL == pucPayload) {
        mrsToolsFree(pstPlcFrame);
        return HI_ERR_MALLOC_FAILUE;
    }

    (hi_void)memset_s(pucPayload, usPayloadLen, 0, usPayloadLen);
    mrsCltSetPowerOnUpData(pucPayload + sizeof(MRS_EVENT_INFO_STRU), pstNvMeter);

    //填充帧中的固定部分
    pstEvtInfo = (MRS_EVENT_INFO_STRU*)(pucPayload);
    pstEvtInfo->ulInterfaceVer = MRS_EVENT_INF_UP_VER;
    pstEvtInfo->ulStructLen = sizeof(MRS_EVENT_INFO_STRU);
    pstEvtInfo->ulDirBit = MRS_PLC_UP_FLG;
    pstEvtInfo->ulFrmBit = MRS_EVENT_FRM_BIT_FROM;
    pstEvtInfo->ulFunCode = MRS_EVENT_FUN_REPORT_CLT;
    pstEvtInfo->ulDataLen = usPayloadLen - sizeof(MRS_EVENT_INFO_STRU);

    pstCtx->usSeq++;
    mrsDfxStaEvtReportSeq(pstCtx->usSeq);
    pstEvtInfo->usSeq = pstCtx->usSeq;
    mrsCltSetPowerCurrPlcSeq(pstEvtInfo->usSeq);

    pstPlcFrame->usId = ID_MRS_CMD_EVENT_INF_PLC;
    pstPlcFrame->pucPayload = pucPayload;
    pstPlcFrame->usPayloadLen = usPayloadLen;

    if (pstCltInfoList->stMeterPowerInfo.pstReportData) {
        mrsToolsFree(pstCltInfoList->stMeterPowerInfo.pstReportData->pucPayload);
    }

    mrsToolsFree(pstCltInfoList->stMeterPowerInfo.pstReportData);

    pstPowerInfo->pstReportData = pstPlcFrame;

    return ret;
}

HI_U32 mrsCltSendPowerOnEvtFrame(HI_VOID)
{
    HI_U32 ret = HI_ERR_SUCCESS;
    POWER_INFO_CLT_INFO_S* pstCltInfoList = mrsGetCltPowerInfo();
    MRS_POWER_METER_INFO_STRU* pstPowerInfo = &pstCltInfoList->stMeterPowerInfo;
    NV_APP_MRS_EVT_CFG_STRU* powerFailureNvCtrl = mrsGetPowerFailureNvCtrl();

    if (HI_TRUE == mrsStaGetPlcChlStatus()) {
        ret = MRS_SendFrameProc(pstPowerInfo->pstReportData);
        MRS_StartTimer(MRS_STA_TIMER_POWERON_PLC_SEND, powerFailureNvCtrl->usSendInterval,
                       HI_SYS_TIMER_ONESHOT);
        mrsDfxCltPowerOnPlcTxTime(HI_MDM_GetMilliSeconds());
    } else {
        mrsSrvChlNotifyProcReg(mrsCltPowerOnChlNotifyProc, MRS_STA_EVT_CLT_POWER_ON);
        mrsCltPowerOnSetSendPlcFlag(HI_TRUE);
    }

    mrsCltSetPowerStatus(MRS_CLT_POWER_ON_PLC_SEND_STATUS);

    return ret;
}

// 通道变更通知-上电事件处理
HI_U32 mrsCltPowerOnChlNotifyProc(HI_BOOL bChlStatus, HI_U32 ulOpt, HI_BOOL* bOneShot)
{
    HI_U32 ret = HI_ERR_SUCCESS;

    if (bChlStatus == HI_TRUE) {
        POWER_INFO_CLT_INFO_S* pstCltInfoList = mrsGetCltPowerInfo();
        MRS_POWER_METER_INFO_STRU* pstPowerInfo = &pstCltInfoList->stMeterPowerInfo;
        NV_APP_MRS_EVT_CFG_STRU* powerFailureNvCtrl = mrsGetPowerFailureNvCtrl();

        switch (ulOpt) {
            case MRS_STA_EVT_CLT_POWER_ON:
                ret = MRS_SendFrameProc(pstPowerInfo->pstReportData);
                MRS_StartTimer(MRS_STA_TIMER_POWERON_PLC_SEND, powerFailureNvCtrl->usSendInterval,
                               HI_SYS_TIMER_ONESHOT);
                mrsDfxCltPowerOnPlcTxTime(HI_MDM_GetMilliSeconds());
                break;

            default:
                break;
        }

        *bOneShot = HI_TRUE;
    } else {
        *bOneShot = HI_FALSE;
    }

    return ret;
}

HI_U32 mrsCltPowerOnRcvAck(HI_U16 usSeq)
{
    if (mrsCltGetPowerReportNum() && (mrsCltGetPowerCurrPlcSeq() == usSeq)) {
        mrsCltPowerOnEnd();
        return HI_ERR_SUCCESS;
    }

    return HI_ERR_SKIP;
}

HI_VOID mrsCltPowerOnEnd(HI_VOID)
{
    POWER_INFO_CLT_INFO_S* pstCltInfoList = mrsGetCltPowerInfo();

    MRS_StopTimer(MRS_STA_TIMER_POWERON_PLC_SEND);
    mrsCltSetPowerStatus(MRS_CLT_POWER_IDLE_STATUS);
    mrsCltResetPowerReportNum();
    mrsCltResetPowerMeterBitMap();
    mrsCltResetPowerReportBitMap();
    mrsCltSetPowerCurrPlcSeq(0);
    mrsCltPowerResetPlcSendTimes();
    mrsSrvChlNotifyProcClear(mrsCltPowerOnChlNotifyProc);
    mrsCltPowerOnSetSendPlcFlag(HI_FALSE);
    pstCltInfoList->ucPowerOnFlag = HI_FALSE;

    if (pstCltInfoList->stMeterPowerInfo.pstReportData) {
        mrsToolsFree(pstCltInfoList->stMeterPowerInfo.pstReportData->pucPayload);
    }

    mrsToolsFree(pstCltInfoList->stMeterPowerInfo.pstReportData);
}

HI_U32 mrsCltPowerOnPlcSendTimeOut(HI_VOID)
{
    HI_U32 ret = HI_ERR_SUCCESS;
    POWER_INFO_CLT_INFO_S* pstCltInfoList = mrsGetCltPowerInfo();
    MRS_POWER_METER_INFO_STRU* pstPowerInfo = &pstCltInfoList->stMeterPowerInfo;
    NV_APP_MRS_EVT_CFG_STRU* powerFailureNvCtrl = mrsGetPowerFailureNvCtrl();

    if (MRS_CLT_POWER_ON_PLC_SEND_STATUS != mrsCltGetPowerStatus()) {
        return HI_ERR_FAILURE;
    }

    mrsCltPowerPlcSendTimesAdd();
    if (mrsCltGetPowerPlcSendTimes() < MRS_CLT_POWER_ON_PLC_SEND_TIMES_MAX) {
        if (HI_NULL == pstPowerInfo->pstReportData) {
            return HI_ERR_FAILURE;
        }

        if (HI_TRUE == mrsStaGetPlcChlStatus()) {
            ret = MRS_SendFrameProc(pstPowerInfo->pstReportData);
            MRS_StartTimer(MRS_STA_TIMER_POWERON_PLC_SEND, powerFailureNvCtrl->usSendInterval,
                           HI_SYS_TIMER_ONESHOT);
        } else {
            if (HI_FALSE == mrsCltPowerOnGetSendPlcFlag()) {
                mrsSrvChlNotifyProcReg(mrsCltPowerOnChlNotifyProc, MRS_STA_EVT_CLT_POWER_ON);
                mrsCltPowerOnSetSendPlcFlag(HI_TRUE);
            }
        }
    } else {
        mrsCltPowerOnEnd();
    }

    return ret;
}

HI_VOID mrsCltiiPowerOnproc(HI_VOID)
{
    POWER_INFO_CLT_INFO_S* pstCltInfoList = mrsGetCltPowerInfo();

    pstCltInfoList->ucPowerOnFlag = HI_TRUE;
    mrsDfxCltPowerOffOnFlag();

    if (mrsCltGetPowerStatus() == MRS_CLT_POWER_ON_PLC_CREATE_STATUS) {
        mrsCltSendPowerOnEvtFrame();
    }
}
#endif

#if defined(PRODUCT_CFG_PRODUCT_TYPE_CCO)
HI_VOID mrsCcoRespCltPowerOffOn(HI_U8* pMac, MRS_EVENT_INFO_STRU* pstRcvEvtInfo,
                                                HI_BOOL bLocalBcFlag)
{
    if (pMac &&
        ((MRS_CLT_POWER_ON_TYPE == pstRcvEvtInfo->ucData[EVT_TYPE_POSITION]) ||
        ((MRS_CLT_POWER_OFF_TYPE == pstRcvEvtInfo->ucData[EVT_TYPE_POSITION]) &&
        (HI_FALSE == bLocalBcFlag)))) {
        mrsCcoEvtSendToSta(pstRcvEvtInfo, pMac, MRS_EVENT_FUN_ACK);
        mrsDfxCcoTxAckRxPowerOnCnt();
    } else {
        mrsDfxCcoRxPowerOffCnt();
    }
}

HI_U16 mrsCcoGetCltPowerReportNum(MRS_SRV_LIST* pstPowerList)
{
    MRS_SRV_LIST* it, *next;
    HI_U16 usNum = 0;

    it = pstPowerList->next;
    while (it != pstPowerList) {
        P_POWER_INFO_CLT_ITEM pstCltItem = (P_POWER_INFO_CLT_ITEM)it;

        next = it->next;
        if (HI_TRUE == pstCltItem->bReported) {
            it = next;
            continue;
        }

        usNum++;
        it = next;
    }

    return usNum;
}

HI_U32 mrsCcoReportCltPowerFrame(MRS_SRV_LIST* pstPowerList, HI_U16 usMeterNum,
                                                  HI_U8 ucType)
{
    HI_U32 ret = HI_ERR_FAILURE;
    HI_U16 usReportTotalNum = 0;
    HI_U16 usSendPkNum = 0;
    P_MRS_CCO_EVT_CTX_STRU pstEvtCtx = mrsGetCcoEvtCtx();

    //先计算有上报的表数量
    usReportTotalNum = mrsCcoGetCltPowerReportNum(pstPowerList);
    if (0 == usReportTotalNum) {
        return HI_ERR_FAILURE;
    }

    do {
        mrsDfxCcoReportPowerMeterCnt(ucType, usReportTotalNum);
        ret = mrsCcoCreateCltPowerFrameProc(pstPowerList, usReportTotalNum, ucType, &usSendPkNum);
        if ((HI_ERR_SUCCESS != ret) && (0 == usSendPkNum)) {
            break;
        }

        pstEvtCtx->bACDFlg = HI_TRUE;
        mrsActiveEvtReport();

        mrsDfxCcoEvtQueueItemNum(pstEvtCtx->stEvtQueue.usItemNum);
        mrsDfxCcoEvtFlag(pstEvtCtx->bACDFlg);
    } while (0);

    if (usMeterNum) {
        //起定时器，90s后擦除数据
        ret = MRS_StartTimer(MRS_CCO_TIMER_CLT_POWEOFF_ERASE_DATA,
                             MRS_CCO_POWER_ITEM_OLD_TIME * 1000 + MRS_CCO_POWER_ITEM_OLD_TIME_ADD,
                             HI_SYS_TIMER_ONESHOT);
    }

    return ret;
}

HI_U32 mrsCcoCreateCltPowerFrameProc(MRS_SRV_LIST* pstPowerList, HI_U16 usReportTotalNum,
                                     HI_U8 ucType, HI_U16* pusSendPkNum)
{
    HI_U32 ret = HI_ERR_FAILURE;
    HI_U16 usPkNum = 0;
    HI_U16 usIndex = 0;
    HI_U16 usDataLen = 0;
    HI_U8* pData = HI_NULL;
    P_MRS_EVT_QUEUE_ITEM pstEvtItem = HI_NULL;
    P_MRS_CCO_EVT_CTX_STRU pstEvtCtx = mrsGetCcoEvtCtx();

    usPkNum = (HI_U16)((usReportTotalNum + MRS_CCO_REPORT_CLT_METER_NUM - 1) / MRS_CCO_REPORT_CLT_METER_NUM);
    for (usIndex = 0; usIndex < usPkNum; usIndex++) {
        HI_U8 ucSendNum = MRS_CCO_REPORT_CLT_METER_NUM;

        if (usIndex == (usPkNum - 1)) {
            ucSendNum = (HI_U8)(usReportTotalNum - usIndex * MRS_CCO_REPORT_CLT_METER_NUM);
        }

        ret = mrsCcoCreateCltPowerFrame(pstPowerList, ucSendNum, &pData, &usDataLen, ucType);
        if (HI_ERR_SUCCESS != ret) {
            break;
        }

        HI_DIAG_LOG_MSG_E4(MRS_FILE_LOG_FLAG_059, HI_DIAG_MT("cco clt pwevt ok,join evt queue"),
                           usReportTotalNum, ucSendNum, usPkNum, ucSendNum);
        pstEvtItem = (P_MRS_EVT_QUEUE_ITEM)mrsToolsMalloc(sizeof(MRS_EVT_QUEUE_ITEM) + usDataLen);
        if (!pstEvtItem) {
            mrsToolsFree(pData);
            ret = HI_ERR_NO_MORE_MEMORY;
            break;
        }

        (hi_void)memset_s(pstEvtItem, sizeof(MRS_EVT_QUEUE_ITEM) + usDataLen, 0,
                          sizeof(MRS_EVT_QUEUE_ITEM) + usDataLen);
        pstEvtItem->usDataLen = usDataLen;
        (hi_void)memcpy_s(pstEvtItem->pucData, pstEvtItem->usDataLen, pData, usDataLen);

        mrsCcoEvtEnQueue(&pstEvtCtx->stEvtQueue, pstEvtItem);
        pstEvtCtx->stEvtQueue.usItemNum++;
        mrsToolsFree(pData);
        mrsDfxCcoReportPowerFrameCnt(ucType);
    }

    *pusSendPkNum = usIndex;

    return ret;
}

HI_VOID mrsCcoDelCltPowerOldItem(MRS_SRV_LIST* pstPowerList, HI_U16* pusMeterNum)
{
    MRS_SRV_LIST* it, *next;
    HI_U32 ulCurrTime = HI_MDM_GetSeconds();

    it = pstPowerList->next;
    while (it != pstPowerList) {
        HI_U32 ret = HI_ERR_SUCCESS;

        P_POWER_INFO_CLT_ITEM pstCltItem = (P_POWER_INFO_CLT_ITEM)it;

        next = it->next;
        if ((HI_FALSE == pstCltItem->bReported) ||
            ((HI_TRUE == pstCltItem->bReported) &&
            ((ulCurrTime - pstCltItem->ulJoinTime) <= MRS_CCO_POWER_ITEM_OLD_TIME))) {
            it = next;
            continue;
        }

        ret = mrsSrvListRemove(pstPowerList, &pstCltItem->link);
        if (HI_ERR_SUCCESS == ret) {
            mrsToolsFree(pstCltItem);
            (*pusMeterNum)--;
        }

        it = next;
    }
}

HI_VOID mrsCcoDelCltPowerListItem(HI_VOID)
{
    POWER_INFO_CLT_INFO_S* pstCltInfoList = mrsGetCltPowerInfo();

    mrsCcoDelCltPowerOldItem(&pstCltInfoList->stPowerOffList, &pstCltInfoList->usOffMeterNum);
    mrsCcoDelCltPowerOldItem(&pstCltInfoList->stPowerOnList, &pstCltInfoList->usOnMeterNum);
    HI_DIAG_LOG_MSG_E2(MRS_FILE_LOG_FLAG_029, HI_DIAG_MT("cco del clt pwevt,curr off on num"),
                       pstCltInfoList->usOffMeterNum, pstCltInfoList->usOnMeterNum);

    mrsDfxCcoCurrPowerMeterCnt(MRS_CLT_POWER_OFF_TYPE, pstCltInfoList->usOffMeterNum);
    mrsDfxCcoCurrPowerMeterCnt(MRS_CLT_POWER_ON_TYPE, pstCltInfoList->usOnMeterNum);
}

HI_U32 mrsCcoCreateCltPowerFrame(MRS_SRV_LIST* pstPowerList, HI_U8 ucSendNum, HI_U8** ppBuf,
                                 HI_U16* pusBufLen, HI_U8 ucType)
{
    HI_U32 ret = HI_ERR_SUCCESS;
    MRS_CCO_SRV_CTX_STRU* pstCtx = mrsCcoGetContext();
    HI_PBYTE pData = HI_NULL;
    HI_U16 usDataLen = 0;
    PROTO376_2_FRAME_PRE_STRU stFrame;
    HI_U16 usOffset = 0;

    do {
        //填充可变部分
        // 设备类型(1) + 规约类型(1) + 报文长度(1) + 报文(pstData->data_len)
        usDataLen = (HI_U16)(DATA_FIX_SIZE + POWER_DATA_FIX_SIZE + ucSendNum * DATA_CLT_SINGLE_METER_SIZE);
        pData = (HI_U8*)mrsToolsMalloc(usDataLen);
        if (!pData) {
            ret = HI_ERR_MALLOC_FAILUE;
            break;
        }

        (hi_void)memset_s(pData, usDataLen, 0, usDataLen);
        //填充固定部分
        pData[0] = MRS_DEVICE_TYPE_COLLECTOR; // 设备类型
        pData[1] = MRS_CCO_REPORT_POWER_PROTOCOL; // 规约类型
        pData[2] = (HI_U8)(POWER_DATA_FIX_SIZE + ucSendNum * DATA_CLT_SINGLE_METER_SIZE);
        usOffset = DATA_FIX_SIZE;
        pData[usOffset++] = ucType;
        //填充可变部分
        mrsCcoSetReportCltPowerData(pstPowerList, pData, usOffset, ucSendNum);

        (hi_void)memset_s(&stFrame, sizeof(stFrame), 0, sizeof(stFrame));
        stFrame.ucPrm = 1;
        stFrame.ucModuleFlag = 0;
        stFrame.ucIs645 = 0;
        stFrame.ucSeq = ++(pstCtx->ucSeq);

        stFrame.stAfnData.ucAFN = MRS_AFN(0x06);
        stFrame.stAfnData.ucDT[0] = MRS_AFN_FN_LO(5);
        stFrame.stAfnData.ucDT[1] = MRS_AFN_FN_HI(5);
        stFrame.stAfnData.pData = pData;
        stFrame.stAfnData.usDataLen = usDataLen;

        ret = mrsCreate3762Frame(&stFrame, ppBuf, pusBufLen, HI_NULL);
        if (HI_ERR_SUCCESS != ret) {
            break;
        }
    } while (0);

    mrsToolsFree(pData);

    return ret;
}

HI_VOID mrsCcoSetReportCltPowerData(MRS_SRV_LIST* pstPowerList, HI_PBYTE pData,
                                                     HI_U16 usOffset, HI_U8 ucSendNum)
{
    MRS_SRV_LIST* it, *next;

    it = pstPowerList->next;
    while (it != pstPowerList) {
        P_POWER_INFO_CLT_ITEM pstCltItem = (P_POWER_INFO_CLT_ITEM)it;

        if ((0 == ucSendNum) || !pstCltItem) {
            break;
        }

        next = it->next;
        if (HI_TRUE == pstCltItem->bReported) {
            it = next;
            continue;
        }

        pstCltItem->bReported = HI_TRUE;
        (hi_void)memcpy_s(&pData[usOffset], HI_METER_ADDR_LEN, pstCltItem->aucMeter, HI_METER_ADDR_LEN);
        usOffset += HI_METER_ADDR_LEN;
        pData[usOffset++] = pstCltItem->ucStatus;

        ucSendNum--;
        it = next;
    }
}

HI_U32 mrsCcoReportCltPowerOffOnEvt(HI_VOID)
{
    HI_U32 ret = HI_ERR_SUCCESS;
    POWER_INFO_CLT_INFO_S* pstCltInfoList = mrsGetCltPowerInfo();

    ret = mrsCcoReportCltPowerFrame(&pstCltInfoList->stPowerOffList, pstCltInfoList->usOffMeterNum,
                                    MRS_CCO_REPORT_POWER_OFF_TYPE);
    ret |= mrsCcoReportCltPowerFrame(&pstCltInfoList->stPowerOnList, pstCltInfoList->usOnMeterNum,
                                     MRS_CCO_REPORT_POWER_ON_TYPE);
    mrsCltPowerSetSendFlag(HI_FALSE);
    return ret;
}
#endif
#endif

