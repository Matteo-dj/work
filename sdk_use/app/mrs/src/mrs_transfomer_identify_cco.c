#include "mrs_transfomer_identify_cco.h"
#include "mrs_transfomer_identify.h"
#include "mrs_cmd_msg.h"
#include "mrs_dfx.h"
#include "mrs_fw_proto376_2_frame.h"
#include "mrs_fw_proto376_2_echo.h"
#include "mrs_fw_log.h"
#include "mrs_srv_common.h"
#include "mrs_srv_cco_macmap.h"
#include "mrs_srv_parallel_cco.h"
#include "mrs_fw_n.h"
#include "mrs_srv_sta_event.h"
#include "mrs_srv_cco.h"
#include "mrs_srv_cco_event.h"
#include "mrs_fw_tools.h"
#include "mrs_srv_sta_queue.h"
#include "mrs_srv_parallel_sta.h"
#include "mrs_srv_sta.h"
#include "mrs_fw_proto645.h"
#include "mrs_srv_cco_archives.h"

HI_START_HEADER
#if defined(PRODUCT_CFG_PRODUCT_TYPE_CCO)

typedef struct
{
    HI_U16 usTei            : 12;
    HI_U16 bValid           : 1;    // 记录是否有效
    HI_U16 bAck             : 1;    // 是否收到应担
    HI_U16 bIsComplete      : 1;    // 是否识别完成
    HI_U16 bIsCollector     : 1;    // 是否采集器

    HI_U8  ucBelongAddrIdx;         // 归属台区地址索引
    HI_U8  ucTryTimes;              // 尝试次数
} MRS_CCO_TF_QUERY_NODE_S;

typedef struct
{
    HI_U8 aucMeterAddr[HI_PLC_MAC_ADDR_LEN];
    HI_U8 ucBelongAddrIdx;
    HI_U8 pad;
} MRS_CCO_TF_REPORT_INFO_S;

typedef struct
{
    /* 参数配置 */
    HI_U8   ucReportModeInit;
    HI_BOOL bUseNotifyMode;
    HI_BOOL bCloseWhiteTfBegin; // 启动识别时是否关闭白名单
    HI_BOOL bRecoverWhiteTfEnd; // 停止识别时是否恢复白名单

    HI_BOOL bCareReportSwitch;          // 使用06-F5上报时，是否受事件上报开关影响
    HI_BOOL bReportUnknownMainNode;     // 对于未知主节点地址，是否上报
    HI_U8   aucMainNodeAddrUnknown[HI_PLC_MAC_ADDR_LEN]; // 未知主节点地址

    HI_U8   aucReportModeMask[MRS_TF_REPORT_MODE_NUM];
    HI_U8   aucSetOffModeMask[MRS_TF_REPORT_MODE_NUM];

    HI_U8   pad1;
    HI_U8   ucQueryInterval;        // 查询间隔
    HI_U8   ucQueryMaxTry;          // 查询最大尝试次数
    HI_U8   ucQueryMaxWay;          // 一次发几帧

    HI_U16  usQueryBeginDelayTIme;  // 查询开始延迟时间，单位s
    HI_U16  usQueryRoundInterval;   // 轮次间隔，单位s

    HI_U8   ucFlagInArcInBelong;    // 上报类型定义: 在档案、属于本台区
    HI_U8   ucFlagInArcNoBelong;    // 上报类型定义: 在档案、不属于本台区
    HI_U8   ucFlagNoArcInBelong;    // 上报类型定义: 不在档案、属于本台区
    HI_U8   ucFlagNoArcNoBelong;    // 上报类型定义: 不在档案、不属于本台区

    /* 控制变量 */
    HI_BOOL bIsRcvNotify;
    HI_U8   ucNotifyMode;
    HI_U8   bWhiteListSwitch;
    HI_U8   ucNeighborNum;
    HI_U8   aucNeighborAddr[MRS_TF_MAX_MEIGHBOR_NUM][HI_PLC_MAC_ADDR_LEN];

    HI_U16 usListNum;
    HI_U16 usQueryIdx;
    MRS_CCO_TF_QUERY_NODE_S *pstQueryList;

    HI_U16 usReportNum;
    HI_U16 pad2;
    MRS_CCO_TF_REPORT_INFO_S *pstReportInfos;

    HI_U16  usTfCmdSendSeq;     // 台区户变关系识别报文发送序号
    HI_U8   ucCollectSeq;       // 采集序列号
    HI_U8   pad3;

    HI_U16  usTfCmdRcvSeq;      // 台区户变关系识别报文接收序列号
    HI_BOOL bTfCmdRcvSeqValid;  // 接收序列号是否有效
    HI_U8   pad4;

    /* 北京台区识别协议相关 */
    HI_BOOL bIsRcvNotifyBJ;     // 是否收到北京台区识别开关通知
    HI_BOOL bSwitchStatusBJ;    // 北京台区识别开关状态
    HI_U16  pad5;
} MRS_CCO_TF_CTX_S;

HI_VOID mrsTfCcoNotifyStaStartCollect(MRS_PLC_TF_DATA_TYPE_E eDataType, TF_NOTIFY_START_COLLECT_NTB_INFO_S *pstInfo);
HI_VOID mrsTfCcoTestCollectStartCmd(MRS_PLC_TF_DATA_TYPE_E eDataType);
HI_VOID mrsTfCcoSendDataCollectCmd(HI_U8 aucDstMac[HI_PLC_MAC_ADDR_LEN], MRS_PLC_TF_DATA_TYPE_E eDataType);
HI_VOID mrsTfCcoSendResultReqCmd(HI_U8 aucDstMac[HI_PLC_MAC_ADDR_LEN]);
HI_VOID mrsTfCcoNotifyNtbData(TF_NOTIFY_NTB_DATA_INFO_S *pstInfo);
HI_BOOL mrsTfCcoCheckIsRepeatReport(HI_U8 *pucMeterAddr, HI_U8 *pucBelongAddr);
HI_VOID mrsTfCcoHandleQueryNtbDataCallback(TF_QUERY_NTB_DATA_INFO_S *pstQueryData);
HI_VOID mrsTfCcoJoinRelativeMetersInQueue(HI_U8 *pucSrcMac, HI_U8 *pucMeterAddr, HI_U8 *pucBelongAddr, HI_U8 ucDeviceType, HI_U8 ucReportFlag);
HI_VOID mrsTfCcoAddReportRecord(HI_U8 *pucMeterAddr, HI_U8 *pucBelongAddr);

MRS_CCO_TF_CTX_S g_stCcoTfCtx = {0};

MRS_CCO_TF_CTX_S *mrsTfCcoGetCtx(HI_VOID)
{
    return &g_stCcoTfCtx;
}

// 测试用
HI_U32 mrsCcoGetaOnlineStaMac(HI_U8 *mac)
{
    MRS_CCO_SRV_CTX_STRU *pstCcoCtx = mrsCcoGetContext();
    HI_U16 i = 0;

    for (i = 0 ; i < HI_MAC_NETWORK_TOPO_ENTRY_MAX && i < pstCcoCtx->stTopoInf.num; i++)
    {
        if (pstCcoCtx->stTopoInf.entry[i].tei != 1)
        {
            memcpy_s(mac, HI_PLC_MAC_ADDR_LEN, pstCcoCtx->stTopoInf.entry[i].mac, HI_PLC_MAC_ADDR_LEN);
            return HI_ERR_SUCCESS;
        }
    }

    return HI_ERR_FAILURE;
}

HI_VOID mrsTfCcoHandleMsg(HI_SYS_QUEUE_MSG_S *pstMsg)
{
    HI_U8 aucTestMac[HI_PLC_MAC_ADDR_LEN] = {0};

    if (pstMsg->ulMsgId != EN_MRS_FW_MSG_TF)
    {
        return;
    }

    switch (pstMsg->ulParam[0])
    {
        case MRS_TF_MSG_ID_NOTIFY_STA_START_COLLECT:
            mrsTfCcoNotifyStaStartCollect(MRS_PLC_TF_DATA_TYPE_PERIOD, (TF_NOTIFY_START_COLLECT_NTB_INFO_S *)pstMsg->ulParam[1]);
            mrsToolsFree(pstMsg->ulParam[1]);
            break;
        case MRS_TF_MSG_ID_QUERY_STA_NTB_DATA:
            mrsTfCcoHandleQueryNtbDataCallback((TF_QUERY_NTB_DATA_INFO_S *)pstMsg->ulParam[1]);
            mrsToolsFree(pstMsg->ulParam[1]);
            break;
        case MRS_TF_MSG_ID_NOTIFY_STA_NTB_DATA:
            mrsTfCcoNotifyNtbData((TF_NOTIFY_NTB_DATA_INFO_S *)pstMsg->ulParam[1]);
            mrsToolsFree(pstMsg->ulParam[1]);
            break;

        case MRS_TF_MSG_ID_SEND_COLELC_START_CMD:
            mrsTfCcoTestCollectStartCmd((MRS_PLC_TF_DATA_TYPE_E)pstMsg->ulParam[1]);
            break;
        case MRS_TF_MSG_ID_SEND_DATA_COLELC_CMD:
            if (mrsCcoGetaOnlineStaMac(aucTestMac) == HI_ERR_SUCCESS) {
                mrsTfCcoSendDataCollectCmd(aucTestMac, (MRS_PLC_TF_DATA_TYPE_E)pstMsg->ulParam[1]);
            }
            break;
        case MRS_TF_MSG_ID_SEND_RESULT_REQ_CMD:
            if (mrsCcoGetaOnlineStaMac(aucTestMac) == HI_ERR_SUCCESS)
            {
                mrsTfCcoSendResultReqCmd(aucTestMac);
            }
            break;

        default:
            break;
    }
}

// 获取上报模式 (根据模式可进一步获取上报哪些差异类型)
HI_U8 mrsTfCcoGetReportMode(HI_VOID)
{
    MRS_CCO_TF_CTX_S *pstCcoTfCtx = mrsTfCcoGetCtx();

    if (HI_TRUE == pstCcoTfCtx->bUseNotifyMode && HI_TRUE == pstCcoTfCtx->bIsRcvNotify)
    {
        return pstCcoTfCtx->ucNotifyMode;
    }
    else
    {
        return pstCcoTfCtx->ucReportModeInit;
    }
}

HI_VOID mrsTfCcoHandleReportModeChange(HI_U8 ucLastMode, HI_U8 ucCurMode)
{
    MRS_CCO_TF_CTX_S *pstCcoTfCtx = mrsTfCcoGetCtx();

    if (0 == ucLastMode && ucCurMode > 0) {
        HI_MDM_SetSoftTfMode(mrsCcoGetSoftTfMode());

        if (HI_TRUE == pstCcoTfCtx->bCloseWhiteTfBegin) {
            HI_MDM_GetWhiteListSwitch(&pstCcoTfCtx->bWhiteListSwitch);
            HI_MDM_SetWhiteListSwitch(HI_FALSE, HI_FALSE, HI_MAC_WHIST_LIST_CHANGE_REASON_TF_BEGIN);
        }

        MRS_StartTimer(MRS_CCO_TIMER_TF_RESULT_QUERY_BEGIN, MRS_SEC_TO_MS(pstCcoTfCtx->usQueryBeginDelayTIme), HI_SYS_TIMER_ONESHOT);
    } else if (ucLastMode > 0 && 0 == ucCurMode) {
        HI_MDM_SetSoftTfMode(SOFT_TF_OFF);

        mrsTfCcoStopQueryProcess();

        if (HI_TRUE == pstCcoTfCtx->bRecoverWhiteTfEnd) {
            HI_MDM_SetWhiteListSwitch(pstCcoTfCtx->bWhiteListSwitch, HI_FALSE, HI_MAC_WHIST_LIST_CHANGE_REASON_TF_END);
        }
    } else {
    }
}

// NV初始化和变更处理函数
HI_U32 mrsTfCcoSetNvPara(NV_APP_CCO_SM_CFG_EXP_STRU *pstNvCfg)
{
    MRS_CCO_TF_CTX_S *pstCcoTfCtx = mrsTfCcoGetCtx();
    HI_U8 ucLastMode = mrsTfCcoGetReportMode();
    HI_U8 ucCurMode = 0;

    if (!pstNvCfg) {
        return HI_ERR_FAILURE;
    }

    pstCcoTfCtx->ucReportModeInit       = pstNvCfg->ucReportModeInit;
    pstCcoTfCtx->bUseNotifyMode         = pstNvCfg->bUseNotifyMode;
    pstCcoTfCtx->bCloseWhiteTfBegin     = pstNvCfg->bCloseWhiteTfBegin;
    pstCcoTfCtx->bRecoverWhiteTfEnd     = pstNvCfg->bRecoverWhiteTfEnd;
    pstCcoTfCtx->bCareReportSwitch      = pstNvCfg->bCareReportSwitch;
    pstCcoTfCtx->bReportUnknownMainNode = pstNvCfg->bReportUnknownMainNode;

    memcpy_s(pstCcoTfCtx->aucMainNodeAddrUnknown, HI_PLC_MAC_ADDR_LEN, pstNvCfg->aucMainNodeAddrUnknown, HI_PLC_MAC_ADDR_LEN);
    memcpy_s(pstCcoTfCtx->aucReportModeMask, MRS_TF_REPORT_MODE_NUM, pstNvCfg->aucReportModeMask, MRS_TF_REPORT_MODE_NUM);
    memcpy_s(pstCcoTfCtx->aucSetOffModeMask, MRS_TF_REPORT_MODE_NUM, pstNvCfg->aucSetOffModeMask, MRS_TF_REPORT_MODE_NUM);

    pstCcoTfCtx->ucQueryInterval        = pstNvCfg->ucQueryInterval;
    pstCcoTfCtx->ucQueryMaxTry          = pstNvCfg->ucQueryMaxTry;
    pstCcoTfCtx->ucQueryMaxWay          = pstNvCfg->ucQueryMaxWay;
    pstCcoTfCtx->usQueryBeginDelayTIme  = pstNvCfg->usQueryBeginDelayTIme;
    pstCcoTfCtx->usQueryRoundInterval   = pstNvCfg->usQueryRoundInterval;

    pstCcoTfCtx->ucFlagInArcInBelong   = pstNvCfg->ucFlagInArcInBelong;
    pstCcoTfCtx->ucFlagInArcNoBelong   = pstNvCfg->ucFlagInArcNoBelong;
    pstCcoTfCtx->ucFlagNoArcInBelong   = pstNvCfg->ucFlagNoArcInBelong;
    pstCcoTfCtx->ucFlagNoArcNoBelong   = pstNvCfg->ucFlagNoArcNoBelong;

    if (mrs_check_area_code(APP_AREA_CODE_BEIJING) == HI_TRUE) {
        HI_MDM_SetSoftTfMode(mrsCcoGetSoftTfMode());
    } else {
        ucCurMode = mrsTfCcoGetReportMode();
        mrsTfCcoHandleReportModeChange(ucLastMode, ucCurMode);
    }

    if (0 == pstCcoTfCtx->pad2) {
        if (1 <= pstNvCfg->pad3[0] && pstNvCfg->pad3[0] <= 3) {
            mrsTfSendMsg(MRS_TF_MSG_ID_SEND_COLELC_START_CMD, pstNvCfg->pad3[0], 0, 0);
        }

        if (4 <= pstNvCfg->pad3[0] && pstNvCfg->pad3[0] <= 6) {
            mrsTfSendMsg(MRS_TF_MSG_ID_SEND_DATA_COLELC_CMD, pstNvCfg->pad3[0] - 3, 0, 0);
        }

        if (7 == pstNvCfg->pad3[0]) {
            mrsTfSendMsg(MRS_TF_MSG_ID_SEND_RESULT_REQ_CMD, 0, 0, 0);
        }
    }
    pstCcoTfCtx->pad2 = (HI_U16)pstNvCfg->pad3[0];

    return HI_ERR_SUCCESS;
}

// 接收到集中器的启动/停止通知
HI_U32 mrsTfCcoHandleRcvNotify(HI_U8 ucReportMode)
{
    MRS_CCO_TF_CTX_S *pstCcoTfCtx = mrsTfCcoGetCtx();
    HI_U8 ucLastMode = mrsTfCcoGetReportMode();
    HI_U8 ucCurMode = 0;

    if (ucReportMode >= MRS_TF_REPORT_MODE_NUM) {
        return HI_ERR_FAILURE;
    }

    pstCcoTfCtx->bIsRcvNotify = HI_TRUE;
    pstCcoTfCtx->ucNotifyMode = ucReportMode;

    if (mrs_check_area_code(APP_AREA_CODE_BEIJING) == HI_FALSE) {
        ucCurMode = mrsTfCcoGetReportMode();
        HI_DIAG_LOG_MSG_E1(MRS_FILE_LOG_FLAG_001, HI_DIAG_MT("[CCO-TF] start identify [mode]"), ucCurMode);
        mrsTfCcoHandleReportModeChange(ucLastMode, ucCurMode);
    }

    return HI_ERR_SUCCESS;
}

// 创建上报的645帧
HI_U32 mrsTfCcoCreate645Frame(HI_U8 *pucMeterAddr, HI_U8 *pucMainNodeAddr, HI_U8 ucDi, HI_U8 **ppData, HI_U16 *pusDataLen)
{
    MRS_PROTO645_FRAME_INF st645Inf;
    HI_U8 aucMainNodeAddrTmp[HI_PLC_MAC_ADDR_LEN] = {0};

    if (HI_NULL == pucMeterAddr || HI_NULL == pucMainNodeAddr || HI_NULL == ppData || HI_NULL == pusDataLen)
    {
        return HI_ERR_INVALID_PARAMETER;
    }

    memcpy_s(aucMainNodeAddrTmp, HI_PLC_MAC_ADDR_LEN, pucMainNodeAddr, HI_PLC_MAC_ADDR_LEN);
    mrsHexInvert(aucMainNodeAddrTmp, HI_PLC_MAC_ADDR_LEN);

    memset_s(st645Inf.aucAddr, HI_METER_ADDR_LEN, 0, HI_METER_ADDR_LEN);
    memcpy_s(st645Inf.aucAddr, HI_METER_ADDR_LEN, pucMeterAddr, HI_METER_ADDR_LEN);
    st645Inf.ucCtrl = MRS_STA_TF_REPORT_645_CTRL;
    st645Inf.ucDiSize = MRS_STA_TF_REPORT_645_FLAG_SIZE;
    memcpy_s(&(st645Inf.ulDi), sizeof(st645Inf.ulDi), &ucDi, st645Inf.ucDiSize);
    st645Inf.ucDataLength = HI_PLC_MAC_ADDR_LEN;
    st645Inf.pData = aucMainNodeAddrTmp;

    return mrsGen645Frame(&st645Inf, ppData, pusDataLen);
}

// 创建上报的3762帧
HI_U32 mrsTfCcoCreateReport3762Frame(HI_U8 *pucMeterAddr, HI_U8 *pucBelongAddr, HI_U8 ucDeviceType, HI_U8 ucDi,
                                     HI_U8 **ppData, HI_U16 *pusDataLen)
{
    MRS_CCO_SRV_CTX_STRU *pstCtx = mrsCcoGetContext();
    PROTO376_2_FRAME_PRE_STRU stFrame = {0};
    HI_U8 *pucFrame645 = HI_NULL;
    HI_U16 usFrame645Len = 0;
    MRS_ARCHIVES_NODE_STRU *pstArchives = mrsSrvArchiversQueryByAddr(pucMeterAddr);
    HI_U32 ret = HI_ERR_SUCCESS;

    ret = mrsTfCcoCreate645Frame(pucMeterAddr, pucBelongAddr, ucDi, &pucFrame645, &usFrame645Len);
    if (HI_ERR_SUCCESS != ret) {
        HI_DIAG_LOG_MSG_E1(MRS_FILE_LOG_FLAG_004, HI_DIAG_MT("[CCO-TF] create 645 fail [ret]"), ret);
        return ret;
    }

    memset_s(&stFrame, sizeof(stFrame), 0, sizeof(stFrame));
    stFrame.ucPrm = 1;
    stFrame.ucModuleFlag = 0;
    stFrame.ucIs645 = 0;
    stFrame.ucSeq = ++(pstCtx->ucSeq);

    memcpy_s(stFrame.ucDstAdd, sizeof(stFrame.ucDstAdd), pstCtx->ucMainNodeAddr, HI_PLC_MAC_ADDR_LEN);
    mrsHexInvert(stFrame.ucDstAdd, sizeof(stFrame.ucDstAdd));

    stFrame.stAfnData.ucAFN = MRS_AFN(0x06);
    stFrame.stAfnData.ucDT[0] = MRS_AFN_FN_LO(5);
    stFrame.stAfnData.ucDT[1] = MRS_AFN_FN_HI(5);

    stFrame.stAfnData.usDataLen = MRS_STA_TF_REPORT_3762_FIX_SIZE + usFrame645Len;
    stFrame.stAfnData.pData = (HI_U8 *)mrsToolsMalloc(stFrame.stAfnData.usDataLen);
    if (HI_NULL == stFrame.stAfnData.pData) {
        mrsToolsFree(pucFrame645);
        return HI_ERR_MALLOC_FAILUE;
    }

    stFrame.stAfnData.pData[0] = ucDeviceType;
    stFrame.stAfnData.pData[1] = (pstArchives ? pstArchives->ucProtocol : METER_PROTO_645_2007);
    stFrame.stAfnData.pData[2] = (HI_U8)usFrame645Len;
    memcpy_s(&(stFrame.stAfnData.pData[MRS_STA_TF_REPORT_3762_FIX_SIZE]), usFrame645Len, pucFrame645, usFrame645Len);

    ret = mrsCreate3762Frame(&stFrame, ppData, pusDataLen, HI_NULL);

    mrsToolsFree(stFrame.stAfnData.pData);
    mrsToolsFree(pucFrame645);

    return ret;
}

// 根据表地址、归属主节点地址，判断该节点属于哪种类型
HI_U8 mrsTfCcoGetReport645Flag(HI_U8 *pucMeterAddr, HI_U8 *aucMainNodeAddr)
{
    MRS_CCO_SRV_CTX_STRU *pstCcoCtx = mrsCcoGetContext();
    MRS_CCO_TF_CTX_S *pstCcoTfCtx = mrsTfCcoGetCtx();

    if (mrsSrvArchivesIsExsit(pucMeterAddr) == HI_TRUE) {
        if (mrsToolsMemEq(pstCcoCtx->ucMainNodeAddr, aucMainNodeAddr, HI_PLC_MAC_ADDR_LEN)) {
            return pstCcoTfCtx->ucFlagInArcInBelong; // 在档案、属于本台区
        } else {
            return pstCcoTfCtx->ucFlagInArcNoBelong; // 在档案、不属于本台区
        }
    } else {
        if (mrsToolsMemEq(pstCcoCtx->ucMainNodeAddr, aucMainNodeAddr, HI_PLC_MAC_ADDR_LEN)) {
            return pstCcoTfCtx->ucFlagNoArcInBelong; // 不在档案、属于本台区
        } else {
            return pstCcoTfCtx->ucFlagNoArcNoBelong; // 不在档案、不属于本台区
        }
    }
}

// 判断在当前使能的模式下，该类型的站点是否需要从网络中踢除
HI_BOOL mrsTfCcoIsSetOffCurFlag(HI_U8 ucReportFlag)
{
    MRS_CCO_TF_CTX_S *pstCcoTfCtx = mrsTfCcoGetCtx();
    HI_U8 ucCurMode = mrsTfCcoGetReportMode();

    if (ucCurMode >= MRS_TF_REPORT_MODE_NUM) {
        return HI_FALSE;
    }

    if (pstCcoTfCtx->aucSetOffModeMask[ucCurMode] & (1 << ucReportFlag)) {
        return HI_TRUE;
    } else {
        return HI_FALSE;
    }
}

// 判断在当前使能的模式下，该类型的站点是否需要上报
HI_BOOL mrsTfCcoIsReportCurFlag(HI_U8 ucReportFlag)
{
    MRS_CCO_TF_CTX_S *pstCcoTfCtx = mrsTfCcoGetCtx();
    HI_U8 ucCurMode = mrsTfCcoGetReportMode();

    if (ucCurMode >= MRS_TF_REPORT_MODE_NUM) {
        return HI_FALSE;
    }

    if ((pstCcoTfCtx->aucReportModeMask[ucCurMode] & (1 << ucReportFlag)) > 0) {
        return HI_TRUE;
    } else {
        return HI_FALSE;
    }
}

// 将电表加入事件上报队列
HI_U32 mrsTfCcoJoinMeterIntoReportQueue(HI_U8 *pucMeterAddr, HI_U8 *pucBelongAddr, HI_U8 ucDeviceType, HI_U8 ucReportFlag)
{
    P_MRS_CCO_EVT_CTX_STRU pstEvtCtx = mrsGetCcoEvtCtx();
    P_MRS_EVT_QUEUE_ITEM pstEvtItem = HI_NULL;
    HI_U8 *pucUpData = HI_NULL;
    HI_U16 usUpDataLen = 0;
    HI_U32 ret = 0;

    ret = mrsTfCcoCreateReport3762Frame(pucMeterAddr, pucBelongAddr, ucDeviceType, ucReportFlag, &pucUpData, &usUpDataLen);
    if (HI_ERR_SUCCESS != ret) {
        HI_DIAG_LOG_MSG_E1(MRS_FILE_LOG_FLAG_007, HI_DIAG_MT("[CCO-TF] create 3762 fail [ret]"), ret);
        return HI_ERR_FAILURE;
    }

    pstEvtItem = (P_MRS_EVT_QUEUE_ITEM)mrsToolsMalloc(sizeof(MRS_EVT_QUEUE_ITEM) + usUpDataLen);
    if (!pstEvtItem) {
        mrsToolsFree(pucUpData);
        return HI_ERR_MALLOC_FAILUE;
    }

    memset_s(pstEvtItem, sizeof(MRS_EVT_QUEUE_ITEM) + usUpDataLen, 0, sizeof(MRS_EVT_QUEUE_ITEM) + usUpDataLen);

    pstEvtItem->usDataLen = usUpDataLen;
    memcpy_s(pstEvtItem->pucData, pstEvtItem->usDataLen, pucUpData, usUpDataLen);

    mrsCcoEvtEnQueue(&pstEvtCtx->stEvtQueue, pstEvtItem);
    pstEvtCtx->stEvtQueue.usItemNum++;

    HI_DIAG_LOG_BUF(MRS_FILE_LOG_BUF_1001, HI_DIAG_MT("[CCO-TF] report event enqueue [3762 data]"), pucUpData, usUpDataLen);
    mrsToolsFree(pucUpData);

    return HI_ERR_SUCCESS;
}

// 将识别完成的结果尝试上报集中器
HI_U32 mrsTfCcoReportDataToConcentrator(HI_U8 *pucSrcMac, HI_U8 *pucMeterAddr, HI_BOOL bIsCollector, MRS_CMD_TF_RESULT_IND_S *pstResultInd)
{
    MRS_CCO_TF_CTX_S *pstCcoTfCtx = mrsTfCcoGetCtx();
    P_MRS_CCO_EVT_CTX_STRU pstEvtCtx = mrsGetCcoEvtCtx();
    HI_U8 ucReportFlag = pstCcoTfCtx->ucFlagInArcInBelong;
    HI_U8 aucBelongAddrTmp[HI_PLC_MAC_ADDR_LEN] = {0};
    HI_U32 ret = 0;

    if (HI_TRUE == pstCcoTfCtx->bCareReportSwitch && HI_FALSE == pstEvtCtx->ucReportEnable) {
        return HI_ERR_FAILURE;
    }

    memcpy_s(aucBelongAddrTmp, HI_PLC_MAC_ADDR_LEN, pstResultInd->belong_addr, HI_PLC_MAC_ADDR_LEN);

    if (!mrsToolsNormalAddr(aucBelongAddrTmp)) {
        if (HI_TRUE == pstCcoTfCtx->bReportUnknownMainNode) {
            memcpy_s(aucBelongAddrTmp, HI_PLC_MAC_ADDR_LEN, pstCcoTfCtx->aucMainNodeAddrUnknown, HI_PLC_MAC_ADDR_LEN);
        } else {
            HI_DIAG_LOG_MSG_E0(MRS_FILE_LOG_FLAG_008, HI_DIAG_MT("[CCO-TF] Intercept Unknown MainNodeAddr"));
            return HI_ERR_SUCCESS;
        }
    }

    ucReportFlag = mrsTfCcoGetReport645Flag(pucMeterAddr, aucBelongAddrTmp);
    HI_DIAG_LOG_MSG_E1(MRS_FILE_LOG_FLAG_014, HI_DIAG_MT("[CCO-TF] [report flag]"), ucReportFlag);

    if (mrsTfCcoIsSetOffCurFlag(ucReportFlag) == HI_TRUE) {
        ret = HI_MDM_SetStaOffline(pucSrcMac, pstResultInd->tei);

        HI_DIAG_LOG_BUF(MRS_FILE_LOG_BUF_1000, HI_DIAG_MT("[CCO-TF] set offline [tei]"), pucSrcMac, HI_PLC_MAC_ADDR_LEN);
        HI_DIAG_LOG_MSG_E1(MRS_FILE_LOG_FLAG_006, HI_DIAG_MT("[CCO-TF] set offline [ret]"), ret);
    }

    if (mrsTfCcoIsReportCurFlag(ucReportFlag) == HI_FALSE) {
        return HI_ERR_SUCCESS;
    }

    mrsTfCcoJoinRelativeMetersInQueue(pucSrcMac, pucMeterAddr, aucBelongAddrTmp, bIsCollector, ucReportFlag);

    return HI_ERR_SUCCESS;
}

// 将MAC地址关联的电表都加入到上报事件队列中
HI_VOID mrsTfCcoJoinRelativeMetersInQueue(HI_U8 *pucSrcMac, HI_U8 *pucMeterAddr, HI_U8 *pucBelongAddr, HI_BOOL bIsCollector, HI_U8 ucReportFlag)
{
    MRS_CCO_SRV_CTX_STRU *pstCcoCtx = mrsCcoGetContext();
    P_MRS_CCO_EVT_CTX_STRU pstEvtCtx = mrsGetCcoEvtCtx();
    MRS_MAC_DEVICE_MAP *pstMacMap = HI_NULL;
    HI_U16 i = 0;
    HI_U32 ret = 0;
    HI_U16 usMeterNum = 0;

    // 映射表的关联的表地址
    pstMacMap = &pstCcoCtx->stMapModule.stMacMeterMap;
    for (i = 0; i < pstMacMap->usMapSize; i++) {
        if (HI_TRUE == pstMacMap->pstItem[i].bValid &&
            mrsToolsMemEq(pstMacMap->pstItem[i].aucMacAddr, pucSrcMac, HI_PLC_MAC_ADDR_LEN) &&
            mrsTfCcoCheckIsRepeatReport(pstMacMap->pstItem[i].stDeviceInf.aucAddr, pucBelongAddr) == HI_FALSE) {
            ret = mrsTfCcoJoinMeterIntoReportQueue(pstMacMap->pstItem[i].stDeviceInf.aucAddr, pucBelongAddr, MRS_DEVICE_TYPE_METER, ucReportFlag);
            if (HI_ERR_SUCCESS == ret) {
                mrsTfCcoAddReportRecord(pucMeterAddr, pucBelongAddr);
            }
            usMeterNum++;
        }
    }

    // 采集器映射表中关联的表地址
    pstMacMap = &pstCcoCtx->stMapModule.stMacCltMap;
    for (i = 0; i < pstMacMap->usMapSize; i++) {
        if (HI_TRUE == pstMacMap->pstItem[i].bValid &&
            mrsToolsMemEq(pstMacMap->pstItem[i].aucMacAddr, pucSrcMac, HI_PLC_MAC_ADDR_LEN) &&
            mrsTfCcoCheckIsRepeatReport(pstMacMap->pstItem[i].stDeviceInf.aucAddr, pucBelongAddr) == HI_FALSE) {
            ret = mrsTfCcoJoinMeterIntoReportQueue(pstMacMap->pstItem[i].stDeviceInf.aucAddr, pucBelongAddr, MRS_DEVICE_TYPE_METER, ucReportFlag);
            if (HI_ERR_SUCCESS == ret) {
                mrsTfCcoAddReportRecord(pucMeterAddr, pucBelongAddr);
            }
            usMeterNum++;
        }
    }

    // 结果报文中携带的表地址
    if (mrsTfCcoCheckIsRepeatReport(pucMeterAddr, pucBelongAddr) == HI_FALSE) {
        if (HI_TRUE == bIsCollector && 0 == usMeterNum) {
            ret = mrsTfCcoJoinMeterIntoReportQueue(pucMeterAddr, pucBelongAddr, MRS_DEVICE_TYPE_COLLECTOR, ucReportFlag);
        }
        else {
            ret = mrsTfCcoJoinMeterIntoReportQueue(pucMeterAddr, pucBelongAddr, MRS_DEVICE_TYPE_METER, ucReportFlag);
        }

        if (HI_ERR_SUCCESS == ret) {
            mrsTfCcoAddReportRecord(pucMeterAddr, pucBelongAddr);
        }
    }

    if (pstEvtCtx->stEvtQueue.usItemNum > 0) {
        pstEvtCtx->bACDFlg = HI_TRUE;
        mrsActiveEvtReport();
        mrsDfxCcoEvtQueueItemNum(pstEvtCtx->stEvtQueue.usItemNum);
        mrsDfxCcoEvtFlag(pstEvtCtx->bACDFlg);
    }
}

// 删除不在拓扑中的点
HI_VOID mrsTfCcoDeleteQueryListNode(HI_MAC_NETWORK_TOPO_S *pstTopo)
{
    MRS_CCO_TF_CTX_S *pstCcoTfCtx = mrsTfCcoGetCtx();
    HI_U16 usListIdx = 0;
    HI_U16 usTopoIdx = 0;

    for (usListIdx = 0; usListIdx < pstCcoTfCtx->usListNum; usListIdx++) {
        for (usTopoIdx = 0; usTopoIdx < pstTopo->num; usTopoIdx++) {
            if (pstCcoTfCtx->pstQueryList[usListIdx].usTei == pstTopo->entry[usTopoIdx].tei) {
                break;
            }
        }

        if (usTopoIdx >= pstTopo->num) {
            HI_DIAG_LOG_BUF(MRS_FILE_LOG_BUF_1005, HI_DIAG_MT("[CCO-TF] delete query node"),
                &pstCcoTfCtx->pstQueryList[usListIdx], sizeof(MRS_CCO_TF_QUERY_NODE_S));
            pstCcoTfCtx->pstQueryList[usListIdx].bValid = HI_FALSE;
        }
    }
}

// 规整一下: 将有效记录移到前面
HI_VOID mrsTfCcoRealignQueryListNode(HI_VOID)
{
    MRS_CCO_TF_CTX_S *pstCcoTfCtx = mrsTfCcoGetCtx();
    HI_U16 i = 0;
    HI_U16 usRealNum = 0;
    HI_U32 ulClearSize = 0;

    // 找到第一个无效点
    while (HI_TRUE == pstCcoTfCtx->pstQueryList[usRealNum].bValid && usRealNum < HI_MAC_NETWORK_TOPO_ENTRY_MAX) {
        usRealNum++;
    }

    // 把有效记录移到前面
    for (i = usRealNum + 1; i < pstCcoTfCtx->usListNum; i++) {
        if (HI_TRUE == pstCcoTfCtx->pstQueryList[i].bValid && i != usRealNum) {
            memcpy_s(&pstCcoTfCtx->pstQueryList[usRealNum], sizeof(MRS_CCO_TF_QUERY_NODE_S),
                &pstCcoTfCtx->pstQueryList[i], sizeof(MRS_CCO_TF_QUERY_NODE_S));
            usRealNum++;
        }
    }

    // 把后面的记录清零
    if (usRealNum < HI_MAC_NETWORK_TOPO_ENTRY_MAX) {
        ulClearSize = (HI_MAC_NETWORK_TOPO_ENTRY_MAX - usRealNum) * sizeof(MRS_CCO_TF_QUERY_NODE_S);
        memset_s(&pstCcoTfCtx->pstQueryList[usRealNum], ulClearSize, 0, ulClearSize);
    }

    // 更新有效记录个数
    pstCcoTfCtx->usListNum = usRealNum;
    HI_DIAG_LOG_MSG_E1(MRS_FILE_LOG_FLAG_009, HI_DIAG_MT("[CCO-TF] usRealNum"), usRealNum);
}

HI_BOOL mrsTfCcoIsProductTypeValid(HI_U8 ucType)
{
    switch (ucType)
    {
        case HI_ND_TYPE_STA:
        case HI_ND_TYPE_CLTI:
        case HI_ND_TYPE_CLTII:
        case HI_ND_TYPE_THREE_STA:
            return HI_TRUE;
        default:
            break;
    }
    return HI_FALSE;
}

// 把新增的添加进去
HI_VOID mrsTfCcoAddQueryListNode(HI_MAC_NETWORK_TOPO_S *pstTopo)
{
    MRS_CCO_TF_CTX_S *pstCcoTfCtx = mrsTfCcoGetCtx();
    HI_U16 usListNum = 0;
    HI_U32 ulTopoIdx = 0;
    HI_U32 ulListIdx = 0;

    usListNum = pstCcoTfCtx->usListNum;

    for (ulTopoIdx = 0; ulTopoIdx < pstTopo->num; ulTopoIdx++) {
        if (mrsTfCcoIsProductTypeValid(pstTopo->entry[ulTopoIdx].product_type) == HI_FALSE) {
            continue;
        }

        for (ulListIdx = 0; ulListIdx < usListNum; ulListIdx++) {
            if (pstCcoTfCtx->pstQueryList[ulListIdx].usTei == pstTopo->entry[ulTopoIdx].tei) {
                break;
            }
        }

        if (ulListIdx >= usListNum && usListNum < HI_MAC_NETWORK_TOPO_ENTRY_MAX) {
            memset_s(&pstCcoTfCtx->pstQueryList[usListNum], sizeof(MRS_CCO_TF_QUERY_NODE_S), 0, sizeof(MRS_CCO_TF_QUERY_NODE_S));
            pstCcoTfCtx->pstQueryList[usListNum].usTei = pstTopo->entry[ulTopoIdx].tei;
            pstCcoTfCtx->pstQueryList[usListNum].bValid = HI_TRUE;

            if (HI_ND_TYPE_CLTII == pstTopo->entry[ulTopoIdx].product_type || HI_ND_TYPE_CLTII == pstTopo->entry[ulTopoIdx].product_type) {
                pstCcoTfCtx->pstQueryList[usListNum].bIsCollector = HI_TRUE;
            }

            HI_DIAG_LOG_BUF(MRS_FILE_LOG_BUF_1006, HI_DIAG_MT("[CCO-TF] add query node"),
                &pstCcoTfCtx->pstQueryList[usListNum], sizeof(MRS_CCO_TF_QUERY_NODE_S));

            usListNum++;
        }
    }

    pstCcoTfCtx->usListNum = usListNum;
}

// 刷新查询列表: 初始化和拓扑变化时调用
HI_U32 mrsTfCcoRefreshQueryList(HI_VOID)
{
    MRS_CCO_SRV_CTX_STRU *pstCcoCtx = mrsCcoGetContext();
    MRS_CCO_TF_CTX_S *pstCcoTfCtx = mrsTfCcoGetCtx();

    if (!pstCcoCtx || !pstCcoTfCtx->pstQueryList)
    {
        return HI_ERR_FAILURE;
    }

    mrsTfCcoDeleteQueryListNode(&pstCcoCtx->stTopoInf);

    mrsTfCcoRealignQueryListNode();

    mrsTfCcoAddQueryListNode(&pstCcoCtx->stTopoInf);

    return HI_ERR_SUCCESS;
}

HI_U32 mrsTfCcoQueryInit(HI_VOID)
{
    MRS_CCO_TF_CTX_S *pstCcoTfCtx = mrsTfCcoGetCtx();
    HI_U32 ulQueryListSize = 0;
    HI_U32 ulReportInfoSize = 0;

    memset_s(pstCcoTfCtx->aucNeighborAddr, sizeof(pstCcoTfCtx->aucNeighborAddr), 0, MRS_TF_MAX_MEIGHBOR_NUM * HI_PLC_MAC_ADDR_LEN * sizeof(HI_U8));
    memcpy_s(pstCcoTfCtx->aucNeighborAddr[0], HI_PLC_MAC_ADDR_LEN, pstCcoTfCtx->aucMainNodeAddrUnknown, HI_PLC_MAC_ADDR_LEN);
    pstCcoTfCtx->ucNeighborNum = 1;

    if (pstCcoTfCtx->pstQueryList) {
        mrsToolsFree(pstCcoTfCtx->pstQueryList);
    }
    ulQueryListSize = HI_MAC_NETWORK_TOPO_ENTRY_MAX * sizeof(MRS_CCO_TF_QUERY_NODE_S);
    pstCcoTfCtx->pstQueryList = (MRS_CCO_TF_QUERY_NODE_S *)mrsToolsMalloc(ulQueryListSize);
    if (!pstCcoTfCtx->pstQueryList) {
        return HI_ERR_MALLOC_FAILUE;
    }
    memset_s(pstCcoTfCtx->pstQueryList, ulQueryListSize, 0, ulQueryListSize);
    pstCcoTfCtx->usListNum = 0;
    pstCcoTfCtx->usQueryIdx = 0;

    if (pstCcoTfCtx->pstReportInfos) {
        mrsToolsFree(pstCcoTfCtx->pstReportInfos);
    }
    ulReportInfoSize = HI_MAC_NETWORK_TOPO_ENTRY_MAX * sizeof(MRS_CCO_TF_REPORT_INFO_S);
    pstCcoTfCtx->pstReportInfos = (MRS_CCO_TF_REPORT_INFO_S *)mrsToolsMalloc(ulReportInfoSize);
    if (!pstCcoTfCtx->pstReportInfos) {
        mrsToolsFree(pstCcoTfCtx->pstQueryList);
        return HI_ERR_MALLOC_FAILUE;
    }
    memset_s(pstCcoTfCtx->pstReportInfos, ulReportInfoSize, 0, ulReportInfoSize);

    mrsTfCcoRefreshQueryList();

    return HI_ERR_SUCCESS;
}

HI_U32 mrsTfCcoBeginQueryProcess(HI_VOID)
{
    if (mrsTfCcoQueryInit() == HI_ERR_SUCCESS) {
        return mrsTfCcoStartNewQueryRound();
    } else {
        return HI_ERR_FAILURE;
    }
}

HI_U32 mrsTfCcoStopQueryProcess(HI_VOID)
{
    MRS_CCO_TF_CTX_S *pstCcoTfCtx = mrsTfCcoGetCtx();

    pstCcoTfCtx->usListNum = 0;
    pstCcoTfCtx->usQueryIdx = 0;
    mrsToolsFree(pstCcoTfCtx->pstQueryList);

    MRS_StopTimer(MRS_CCO_TIMER_TF_RESULT_QUERY_NEXT);
    MRS_StopTimer(MRS_CCO_TIMER_TF_RESULT_QUERY_ROUND);
    MRS_StopTimer(MRS_CCO_TIMER_TF_RESULT_QUERY_BEGIN);

    return HI_ERR_SUCCESS;
}

HI_U32 mrsTfCcoStartNewQueryRound(HI_VOID)
{
    MRS_CCO_TF_CTX_S *pstCcoTfCtx = mrsTfCcoGetCtx();
    HI_U16 i = 0;

    for (i = 0; i < pstCcoTfCtx->usListNum; i++)
    {
        pstCcoTfCtx->pstQueryList[i].bAck = HI_FALSE;
        pstCcoTfCtx->pstQueryList[i].ucTryTimes = 0;
    }

    HI_DIAG_LOG_MSG_E0(MRS_FILE_LOG_FLAG_010, HI_DIAG_MT("[CCO-TF] start a new round"));

    return mrsTfCcoRunQueryProcess();
}

HI_BOOL mrsTfCcoIsRoundQueryEnd(HI_VOID)
{
    MRS_CCO_TF_CTX_S *pstCcoTfCtx = mrsTfCcoGetCtx();
    HI_U16 i = 0;

    for (i = 0; i < pstCcoTfCtx->usListNum; i++) {
        if (HI_FALSE == pstCcoTfCtx->pstQueryList[i].bValid ||
            HI_TRUE == pstCcoTfCtx->pstQueryList[i].bAck ||
            pstCcoTfCtx->pstQueryList[i].ucTryTimes >= pstCcoTfCtx->ucQueryMaxTry) {
            continue;
        }

        return HI_FALSE;
    }

    return HI_TRUE;
}

HI_U32 mrsTfCcoRunQueryProcess(HI_VOID)
{
    MRS_CCO_TF_CTX_S *pstCcoTfCtx = mrsTfCcoGetCtx();
    HI_U8 aucStaMac[HI_PLC_MAC_ADDR_LEN] = {0};
    HI_U16 usQueryCnt = 0;
    HI_U16 i = 0;

    for (i = pstCcoTfCtx->usQueryIdx; i < pstCcoTfCtx->usListNum && usQueryCnt < pstCcoTfCtx->ucQueryMaxWay; i++) {
        if (HI_TRUE != pstCcoTfCtx->pstQueryList[i].bValid ||
            HI_TRUE == pstCcoTfCtx->pstQueryList[i].bAck ||
            pstCcoTfCtx->pstQueryList[i].ucTryTimes >= pstCcoTfCtx->ucQueryMaxTry) {
            continue;
        }

        if (mrsCcoGetMacByTei(pstCcoTfCtx->pstQueryList[i].usTei, aucStaMac) != HI_ERR_SUCCESS) {
            continue;
        }

        mrsTfCcoSendResultReqCmd(aucStaMac);
        pstCcoTfCtx->pstQueryList[i].ucTryTimes++;
        usQueryCnt++;
    }

    pstCcoTfCtx->usQueryIdx = i;
    if (pstCcoTfCtx->usQueryIdx == pstCcoTfCtx->usListNum) {
        pstCcoTfCtx->usQueryIdx = 0;
    }

    if (mrsTfCcoIsRoundQueryEnd() == HI_TRUE) {
        MRS_StartTimer(MRS_CCO_TIMER_TF_RESULT_QUERY_ROUND, MRS_SEC_TO_MS(pstCcoTfCtx->usQueryRoundInterval), HI_SYS_TIMER_ONESHOT);
    } else {
        MRS_StartTimer(MRS_CCO_TIMER_TF_RESULT_QUERY_NEXT, MRS_SEC_TO_MS(pstCcoTfCtx->ucQueryInterval), HI_SYS_TIMER_ONESHOT);
    }

    return HI_ERR_SUCCESS;
}

MRS_CCO_TF_QUERY_NODE_S *mrsTfCcoGetQueryNode(HI_U16 usTei)
{
    MRS_CCO_TF_CTX_S *pstCcoTfCtx = mrsTfCcoGetCtx();
    HI_U16 i = 0;

    if (!pstCcoTfCtx->pstQueryList) {
        return HI_NULL;
    }

    for (i = 0; i < pstCcoTfCtx->usListNum; i++) {
        if (pstCcoTfCtx->pstQueryList[i].usTei == usTei) {
            return &pstCcoTfCtx->pstQueryList[i];
        }
    }

    return HI_NULL;
}

// 获取主节点地址对应的映射ID
HI_U8 mrsTfCcoGetBelongAddrIdx(HI_U8 *aucMainNodeAddr)
{
    MRS_CCO_TF_CTX_S *pstCcoTfCtx = mrsTfCcoGetCtx();
    HI_U8 i = 0;

    if (mrsToolsInvalidMac(aucMainNodeAddr))
    {
        return 0;
    }

    for (i = 0; i < pstCcoTfCtx->ucNeighborNum; i++)
    {
        if (mrsToolsMemEq(aucMainNodeAddr, pstCcoTfCtx->aucNeighborAddr[i], HI_PLC_MAC_ADDR_LEN))
        {
            return i;
        }
    }

    if (i == pstCcoTfCtx->ucNeighborNum && pstCcoTfCtx->ucNeighborNum < MRS_TF_MAX_MEIGHBOR_NUM)
    {
        memcpy_s(pstCcoTfCtx->aucNeighborAddr[i], HI_PLC_MAC_ADDR_LEN, aucMainNodeAddr, HI_PLC_MAC_ADDR_LEN);
        pstCcoTfCtx->ucNeighborNum++;
        return i;
    }

    return 0;
}

// 北京局专用: 收到台区识别开关的处理
HI_VOID mrsTfCcoHandleRcv05F130(HI_BOOL bSwitchStatus)
{
    MRS_CCO_TF_CTX_S *pstCcoTfCtx = mrsTfCcoGetCtx();

    // 从关到开: 设置识别状态为未完成，重新等待识别完成
    if (HI_TRUE == pstCcoTfCtx->bIsRcvNotifyBJ && HI_FALSE == pstCcoTfCtx->bSwitchStatusBJ && HI_TRUE == bSwitchStatus)
    {
        HI_MDM_SetUseLastTfStatus(HI_FALSE);
    }

    pstCcoTfCtx->bIsRcvNotifyBJ = HI_TRUE;
    pstCcoTfCtx->bSwitchStatusBJ = bSwitchStatus;
}

// 检查是否重复上报 (同一块表，归属不同主节点地址的情况，不算重复上报)
HI_BOOL mrsTfCcoCheckIsRepeatReport(HI_U8 *pucMeterAddr, HI_U8 *pucBelongAddr)
{
    MRS_CCO_TF_CTX_S *pstCcoTfCtx = mrsTfCcoGetCtx();
    HI_U8 ucBelongAddrIdx = mrsTfCcoGetBelongAddrIdx(pucBelongAddr);
    HI_U16 i = 0;

    for (i = 0; i < pstCcoTfCtx->usReportNum; i++) {
        if (mrsToolsMemEq(pucMeterAddr, pstCcoTfCtx->pstReportInfos[i].aucMeterAddr, HI_PLC_MAC_ADDR_LEN)) {
            if (ucBelongAddrIdx == pstCcoTfCtx->pstReportInfos[i].ucBelongAddrIdx) {
                return HI_TRUE;
            } else {
                return HI_FALSE;
            }
        }
    }

    return HI_FALSE;
}

// 添加上报记录
HI_VOID mrsTfCcoAddReportRecord(HI_U8 *pucMeterAddr, HI_U8 *pucBelongAddr)
{
    MRS_CCO_TF_CTX_S *pstCcoTfCtx = mrsTfCcoGetCtx();
    HI_U8 ucBelongAddrIdx = mrsTfCcoGetBelongAddrIdx(pucBelongAddr);
    HI_U16 i = 0;

    // 如果有对应记录，更新
    for (i = 0; i < pstCcoTfCtx->usReportNum; i++) {
        if (mrsToolsMemEq(pucMeterAddr, pstCcoTfCtx->pstReportInfos[i].aucMeterAddr, HI_PLC_MAC_ADDR_LEN)) {
            pstCcoTfCtx->pstReportInfos[i].ucBelongAddrIdx = ucBelongAddrIdx;
            return;
        }
    }

    // 无对应记录，新增
    if (pstCcoTfCtx->usReportNum < HI_MAC_NETWORK_TOPO_ENTRY_MAX) {
        memcpy_s(pstCcoTfCtx->pstReportInfos[pstCcoTfCtx->usReportNum].aucMeterAddr, HI_PLC_MAC_ADDR_LEN, pucMeterAddr, HI_PLC_MAC_ADDR_LEN);
        pstCcoTfCtx->pstReportInfos[pstCcoTfCtx->usReportNum].ucBelongAddrIdx = ucBelongAddrIdx;
        pstCcoTfCtx->usReportNum++;
    }
}

// CCO收到STA上报台区识别结果的处理: 判定类型，上报集中器
HI_U32 mrsTfCcoHandleResultInd(HI_U8 *aucSrcMac, MRS_PLC_TF_CMD_S *pstTfCmd, HI_U16 usCmdSize)
{
    MRS_CMD_TF_RESULT_IND_S *pstResultInd = (MRS_CMD_TF_RESULT_IND_S *)pstTfCmd->data;
    MRS_CCO_TF_QUERY_NODE_S *pstQueryInfo = HI_NULL;
    HI_U8 aucMeterAddr[HI_PLC_MAC_ADDR_LEN] = {0};
    HI_U8 ucBelongAddrIdx = 0;

    HI_DIAG_LOG_BUF(MRS_FILE_LOG_BUF_1011, HI_DIAG_MT("[CCO-TF]: rcv result ind"), pstTfCmd, usCmdSize);

    if (usCmdSize < sizeof(MRS_PLC_TF_CMD_S) + MRS_CMD_TF_RESULT_IND_DATA_SIZE) {
        HI_DIAG_LOG_MSG_E0(MRS_FILE_LOG_FLAG_012, HI_DIAG_MT("[CCO-TF] invalid frame"));
        return HI_ERR_BAD_FRAME_SIZE;
    }

    pstQueryInfo = mrsTfCcoGetQueryNode(pstResultInd->tei);
    if (!pstQueryInfo) {
        return HI_ERR_FAILURE;
    }

    ucBelongAddrIdx = mrsTfCcoGetBelongAddrIdx(pstResultInd->belong_addr);

    HI_DIAG_LOG_MSG_E2(MRS_FILE_LOG_FLAG_013, HI_DIAG_MT("[CCO-TF] [last_idx] [cur_idx]"), pstQueryInfo->ucBelongAddrIdx, ucBelongAddrIdx);

    pstQueryInfo->bAck = HI_TRUE;
    pstQueryInfo->bIsComplete = pstResultInd->complete_flag;
    pstQueryInfo->ucBelongAddrIdx = ucBelongAddrIdx;

    memcpy_s(aucMeterAddr, HI_PLC_MAC_ADDR_LEN, pstTfCmd->mac_addr, HI_PLC_MAC_ADDR_LEN);
    mrsHexInvert(aucMeterAddr, HI_PLC_MAC_ADDR_LEN);

    if (HI_TRUE == pstResultInd->complete_flag) {
        mrsTfCcoReportDataToConcentrator(aucSrcMac, aucMeterAddr, pstQueryInfo->bIsCollector, pstResultInd);
    }

    return HI_ERR_SUCCESS;
}

HI_U8 mrsTfCcoGetCollectSeq(HI_VOID)
{
    MRS_CCO_TF_CTX_S *pstTfCtx = mrsTfCcoGetCtx();

    return pstTfCtx->ucCollectSeq;
}

HI_U32 mrsCmdCcoHandleTfCmd(HI_IN HI_U16 usId, HI_PVOID pReqPacket, HI_U16 usReqPacketSize)
{
    MRS_CCO_TF_CTX_S *pstTfCtx = mrsTfCcoGetCtx();
    MRS_PLC_FRAME_DATA_STRU *pstPlcFrame = (MRS_PLC_FRAME_DATA_STRU *)pReqPacket;
    MRS_PLC_TF_CMD_S *pstTfCmd;

    if (!pstPlcFrame || !pstPlcFrame->pucPayload) {
        return HI_ERR_FAILURE;
    }

    pstTfCmd = (MRS_PLC_TF_CMD_S *)pstPlcFrame->pucPayload;

    HI_DIAG_LOG_MSG_E2(MRS_FILE_LOG_FLAG_015, HI_DIAG_MT("[CCO-TF]: rcv cmd [seq] [option]"), pstTfCmd->seq, pstTfCmd->option);

    if (HI_TRUE == pstTfCtx->bTfCmdRcvSeqValid && pstTfCtx->usTfCmdRcvSeq == pstTfCmd->seq) {
        return HI_ERR_SUCCESS;
    }

    pstTfCtx->usTfCmdRcvSeq = pstTfCmd->seq;
    pstTfCtx->bTfCmdRcvSeqValid = HI_TRUE;

    switch (pstTfCmd->option) {
        case MRS_PLC_TF_OPTION_DATA_NOTIFY:
            HI_DIAG_LOG_BUF(MRS_FILE_LOG_BUF_1010, HI_DIAG_MT("[CCO-TF]: rcv data notify"), pstTfCmd, HI_MIN(64, pstPlcFrame->usPayloadLen));
            mrsTfHandleNotifyNtbDataCmd(pstPlcFrame->ucRcvFreq, pstPlcFrame->ulRcvNid, pstTfCmd, pstPlcFrame->usPayloadLen);
            break;
        case MRS_PLC_TF_OPTION_RESULT_IND:
            mrsTfCcoHandleResultInd(pstPlcFrame->ucMac, pstTfCmd, pstPlcFrame->usPayloadLen);
            break;
        default:
            break;
    }

    return HI_ERR_SUCCESS;
}

HI_VOID mrsTfCcoNotifyStaStartCollect(MRS_PLC_TF_DATA_TYPE_E eDataType, TF_NOTIFY_START_COLLECT_NTB_INFO_S *pstInfo)
{
    MRS_CCO_SRV_CTX_STRU *pstCcoCtx = mrsCcoGetContext();
    MRS_CCO_TF_CTX_S *pstTfCtx = mrsTfCcoGetCtx();
    MRS_PLC_FRAME_DATA_STRU stPlcFrame;
    MRS_PLC_TF_CMD_S *pstTfCmd;
    MRS_PLC_TF_COLLECT_START_S *pstCollectInfo;
    HI_U8 *aucPayload = HI_NULL;
    HI_U16 usPayloadLen = sizeof(MRS_PLC_TF_CMD_S) + sizeof(MRS_PLC_TF_COLLECT_START_S);
    HI_U32 ret = HI_ERR_SUCCESS;

    if (!pstInfo) {
        return;
    }

    aucPayload = mrsToolsMalloc(usPayloadLen);
    if (!aucPayload) {
        return;
    }
    memset_s(aucPayload, usPayloadLen, 0, usPayloadLen);

    pstTfCmd = (MRS_PLC_TF_CMD_S *)aucPayload;
    pstTfCmd->interface_ver = MRS_CMD_STRU_VER_R1;
    pstTfCmd->stru_len      = sizeof(MRS_PLC_TF_CMD_S);
    pstTfCmd->dir           = MRS_PLC_DL_FLG;
    pstTfCmd->from          = MRS_PLC_FROM_START;
    pstTfCmd->phase         = PHASE_ALL;
    pstTfCmd->seq           = pstTfCtx->usTfCmdSendSeq++;
    pstTfCmd->data_type     = eDataType;
    pstTfCmd->option        = MRS_PLC_TF_OPTION_COLLECT_START;
    (HI_VOID)memcpy_s(pstTfCmd->mac_addr, HI_PLC_MAC_ADDR_LEN, pstCcoCtx->ucMainNodeAddr, HI_PLC_MAC_ADDR_LEN);

    pstCollectInfo = (MRS_PLC_TF_COLLECT_START_S *)pstTfCmd->data;
    pstCollectInfo->start_ntb   = pstInfo->start_ntb;
    pstCollectInfo->collect_num = pstInfo->collect_num;

    if (HI_TRUE == pstInfo->is_incarease_collect_seq) {
        pstTfCtx->ucCollectSeq++;
    }
    pstCollectInfo->collect_seq = pstTfCtx->ucCollectSeq;

    memset_s(&stPlcFrame, sizeof(MRS_PLC_FRAME_DATA_STRU), 0, sizeof(MRS_PLC_FRAME_DATA_STRU));
    stPlcFrame.usId = ID_MRS_CMD_TF_PLC;
    stPlcFrame.pucPayload = aucPayload;
    stPlcFrame.usPayloadLen = usPayloadLen;
    memcpy_s(stPlcFrame.ucMac, HI_PLC_MAC_ADDR_LEN, pstInfo->dest_mac, HI_PLC_MAC_ADDR_LEN);

    ret = MRS_SendPlcFrame(&stPlcFrame);
    HI_DIAG_LOG_BUF(MRS_FILE_LOG_BUF_1012, HI_DIAG_MT("[CCO-TF]: notify collect start"), aucPayload, usPayloadLen);
    HI_DIAG_LOG_MSG_E1(MRS_FILE_LOG_FLAG_016, HI_DIAG_MT("[CCO-TF]: send notify collect start ret"), ret);
    mrsToolsFree(aucPayload);
}

HI_VOID mrsTfCcoTestCollectStartCmd(MRS_PLC_TF_DATA_TYPE_E eDataType)
{
    TF_NOTIFY_START_COLLECT_NTB_INFO_S info = {0};
    HI_BOOL bIsSync = HI_FALSE;

    info.start_ntb = HI_MDM_GetNtb(&bIsSync) + MRS_MS_TO_NTB(30*1000);
    info.collect_num = 0x15;
    info.is_incarease_collect_seq = HI_TRUE;
    memset_s(info.dest_mac, HI_PLC_MAC_ADDR_LEN, 0xFF, HI_PLC_MAC_ADDR_LEN);

    mrsTfCcoNotifyStaStartCollect(eDataType, &info);
}

HI_VOID mrsTfCcoNotifyNtbDataInOneFrame(TF_NOTIFY_NTB_DATA_INFO_S *pstInfo, HI_U8 ucPowerEdgeCnt,
                                        HI_U16 usTotalDiffNum, HI_U8 ucPhaseDiffNum, HI_U8 ucSendPhase)
{
    MRS_CCO_TF_CTX_S *pstTfCtx = mrsTfCcoGetCtx();
    MRS_PLC_FRAME_DATA_STRU stPlcFrame;
    MRS_PLC_TF_CMD_S *pstTfCmd;
    MRS_CMD_TF_DATA_NOTIFY_S *pstDataNotify;
    MRS_CMD_TF_DATA_SEQ_S *pstDataSeqTmp;
    HI_U8 *aucPayload = HI_NULL;
    HI_U16 usPayloadLen = sizeof(MRS_PLC_TF_CMD_S) + MRS_CMD_TF_DATA_NOTIFY_FIXED_LEN;
    HI_U32 ret = HI_ERR_SUCCESS;
    HI_U16 usBaseNtbIdx = 0;
    HI_U16 usDataSeqOffset = 0;
    HI_U16 usDiffIdx = 0;
    HI_U8 ucPhaseIdx = 0;

    usPayloadLen += (HI_U16)(ucPowerEdgeCnt * MRS_CMD_TF_DATA_SEQ_FIXED_LEN + usTotalDiffNum * sizeof(HI_U16));
    aucPayload = mrsToolsMalloc(usPayloadLen);
    if (!aucPayload) {
        return;
    }
    memset_s(aucPayload, usPayloadLen, 0, usPayloadLen);

    pstTfCmd = (MRS_PLC_TF_CMD_S *)aucPayload;
    pstTfCmd->interface_ver = MRS_CMD_STRU_VER_R1;
    pstTfCmd->stru_len      = sizeof(MRS_PLC_TF_CMD_S);
    pstTfCmd->dir           = MRS_PLC_DL_FLG;
    pstTfCmd->from          = MRS_PLC_FROM_START;
    pstTfCmd->phase         = ucSendPhase;
    pstTfCmd->seq           = pstTfCtx->usTfCmdSendSeq++;
    pstTfCmd->data_type     = MRS_PLC_TF_DATA_TYPE_PERIOD;
    pstTfCmd->option        = MRS_PLC_TF_OPTION_DATA_NOTIFY;
    (HI_VOID)memcpy_s(pstTfCmd->mac_addr, HI_PLC_MAC_ADDR_LEN, pstInfo->addr, HI_PLC_MAC_ADDR_LEN);

    pstDataNotify = (MRS_CMD_TF_DATA_NOTIFY_S *)pstTfCmd->data;
    pstDataNotify->tei = 1;
    pstDataNotify->collect_edge = mrsTfConvertToCmdPowerEdge(pstInfo->collect_edge);
    if (HI_TRUE == pstInfo->is_increase_collect_seq) {
        pstTfCtx->ucCollectSeq++;
    }
    pstDataNotify->collect_seq = pstTfCtx->ucCollectSeq;
    pstDataNotify->total_num = 0;

    usDataSeqOffset = MRS_CMD_TF_DATA_NOTIFY_FIXED_LEN;
    pstDataSeqTmp = (MRS_CMD_TF_DATA_SEQ_S *)(pstTfCmd->data + usDataSeqOffset);
    pstDataSeqTmp->base_ntb = pstInfo->ntb_buf[0];
    usBaseNtbIdx = 0;
    usDiffIdx = 0;
    for (ucPhaseIdx = 0; ucPhaseIdx < PHASE_CNT; ucPhaseIdx++) {
        if (0 == pstInfo->phase_ntb_num[ucPhaseIdx]) {
            continue;
        }

        pstDataSeqTmp->report_num[ucPhaseIdx] = HI_MIN(pstInfo->phase_ntb_num[ucPhaseIdx] - 1, ucPhaseDiffNum);

        mrsTfCreateNtbDiffSeq(&pstInfo->ntb_buf[usBaseNtbIdx], pstDataSeqTmp->report_num[ucPhaseIdx],
                                 &pstDataSeqTmp->value[usDiffIdx]);

        pstDataNotify->total_num += pstDataSeqTmp->report_num[ucPhaseIdx];
        usDiffIdx += pstDataSeqTmp->report_num[ucPhaseIdx];
        usBaseNtbIdx += pstInfo->phase_ntb_num[ucPhaseIdx];
    }

    if (POWER_EDGE_DOUBLE == pstInfo->collect_edge) {
        usDataSeqOffset += (MRS_CMD_TF_DATA_SEQ_FIXED_LEN + pstDataNotify->total_num * sizeof(HI_U16));
        pstDataSeqTmp = (MRS_CMD_TF_DATA_SEQ_S *)(pstTfCmd->data + usDataSeqOffset);
        usDiffIdx = 0;
        pstDataSeqTmp->base_ntb = pstInfo->ntb_buf[usBaseNtbIdx];
        for (ucPhaseIdx = 0; ucPhaseIdx < PHASE_CNT; ucPhaseIdx++) {
            if (0 == pstInfo->phase_ntb_num[PHASE_CNT + ucPhaseIdx]) {
                continue;
            }

            pstDataSeqTmp->report_num[ucPhaseIdx] = HI_MIN(pstInfo->phase_ntb_num[PHASE_CNT + ucPhaseIdx] - 1, ucPhaseDiffNum);

            mrsTfCreateNtbDiffSeq(&pstInfo->ntb_buf[usBaseNtbIdx], pstDataSeqTmp->report_num[ucPhaseIdx],
                                     &pstDataSeqTmp->value[usDiffIdx]);

            pstDataNotify->total_num += pstDataSeqTmp->report_num[ucPhaseIdx];
            usDiffIdx += pstDataSeqTmp->report_num[ucPhaseIdx];
            usBaseNtbIdx += pstInfo->phase_ntb_num[PHASE_CNT + ucPhaseIdx];
        }
    }

    memset_s(&stPlcFrame, sizeof(MRS_PLC_FRAME_DATA_STRU), 0, sizeof(MRS_PLC_FRAME_DATA_STRU));
    stPlcFrame.usId = ID_MRS_CMD_TF_PLC;
    stPlcFrame.pucPayload = aucPayload;
    stPlcFrame.usPayloadLen = usPayloadLen;
    stPlcFrame.is_broadcast_send = HI_TRUE;
    memset_s(stPlcFrame.ucMac, HI_PLC_MAC_ADDR_LEN, 0xFF, HI_PLC_MAC_ADDR_LEN);

    ret = MRS_SendPlcFrame(&stPlcFrame);
    HI_DIAG_LOG_BUF(MRS_FILE_LOG_BUF_1013, HI_DIAG_MT("[CCO-TF]: notify ntb data"), aucPayload, HI_MIN(64, usPayloadLen));
    HI_DIAG_LOG_MSG_E1(MRS_FILE_LOG_FLAG_017, HI_DIAG_MT("[CCO-TF]: send notify ntb data ret"), ret);
    mrsToolsFree(aucPayload);
}

HI_VOID mrsTfCcoNotifyNtbData(TF_NOTIFY_NTB_DATA_INFO_S *pstInfo)
{
    HI_U8  ucSendPowerEdgeCnt = 0;
    HI_U8  ucPhaseNum = 0;
    HI_U16 usTotalDiffNum = 0;
    HI_U8  ucPhaseDiffNum = 0xFF;
    HI_U8  ucSendPhase = PHASE_ALL;
    HI_U8  i = 0;

    if (!pstInfo || 0 == pstInfo->total_ntb_num || pstInfo->collect_edge >= POWER_EDGE_UNKNOWN) {
        return;
    }

    ucSendPowerEdgeCnt = (POWER_EDGE_DOUBLE == pstInfo->collect_edge ? POWER_EDGE_CNT : 1);

    for (i = 0; i < ucSendPowerEdgeCnt * PHASE_CNT; i++) {
        if (pstInfo->phase_ntb_num[i] > 0) {
            ucPhaseNum++;
            usTotalDiffNum += (pstInfo->phase_ntb_num[i] - 1);
            ucPhaseDiffNum = HI_MIN(pstInfo->phase_ntb_num[i] - 1, ucPhaseDiffNum);
        }
    }

    if (0 == ucPhaseNum) {
        return;
    }

    if (ucSendPowerEdgeCnt > 1) {
        usTotalDiffNum = HI_MIN(MRS_CMD_TF_MAX_DIFF_NUM_2EDGE, usTotalDiffNum);
    } else {
        usTotalDiffNum = HI_MIN(MRS_CMD_TF_MAX_DIFF_NUM_1EDGE, usTotalDiffNum);
    }

    if (ucPhaseDiffNum * ucPhaseNum < usTotalDiffNum) {
        usTotalDiffNum = ucPhaseDiffNum * ucPhaseNum;
    }

    if (usTotalDiffNum / ucPhaseNum < ucPhaseDiffNum) {
        ucPhaseDiffNum = (HI_U8)(usTotalDiffNum / ucPhaseNum);
    }

    usTotalDiffNum = ucPhaseDiffNum * ucPhaseNum;

    if (1 == ucPhaseNum) {
        if (pstInfo->phase_ntb_num[0] > 0 || pstInfo->phase_ntb_num[PHASE_CNT] > 0) {
            ucSendPhase = PHASE_A;
        } else if (pstInfo->phase_ntb_num[1] > 0 || pstInfo->phase_ntb_num[PHASE_CNT + 1] > 0) {
            ucSendPhase = PHASE_B;
        } else if (pstInfo->phase_ntb_num[2] > 0 || pstInfo->phase_ntb_num[PHASE_CNT + 2] > 0) {
            ucSendPhase = PHASE_C;
        }
    }

    mrsTfCcoNotifyNtbDataInOneFrame(pstInfo, ucSendPowerEdgeCnt, usTotalDiffNum, ucPhaseDiffNum, ucSendPhase);
}

HI_VOID mrsTfCcoSendDataCollectCmd(HI_U8 aucDstMac[HI_PLC_MAC_ADDR_LEN], MRS_PLC_TF_DATA_TYPE_E eDataType)
{
    MRS_CCO_SRV_CTX_STRU *pstCcoCtx = mrsCcoGetContext();
    MRS_CCO_TF_CTX_S *pstTfCtx = mrsTfCcoGetCtx();
    MRS_PLC_FRAME_DATA_STRU stPlcFrame;
    MRS_PLC_TF_CMD_S *pstTfCmd;
    HI_U8 *aucPayload = HI_NULL;
    HI_U16 usPayloadLen = sizeof(MRS_PLC_TF_CMD_S);
    HI_U32 ret = HI_ERR_SUCCESS;

    aucPayload = mrsToolsMalloc(usPayloadLen);
    if (!aucPayload)
    {
        return;
    }
    memset_s(aucPayload, usPayloadLen, 0, usPayloadLen);

    pstTfCmd = (MRS_PLC_TF_CMD_S *)aucPayload;
    pstTfCmd->interface_ver = MRS_CMD_STRU_VER_R1;
    pstTfCmd->stru_len      = sizeof(MRS_PLC_TF_CMD_S);
    pstTfCmd->dir           = MRS_PLC_DL_FLG;
    pstTfCmd->from          = MRS_PLC_FROM_START;
    pstTfCmd->phase         = PHASE_ALL;
    pstTfCmd->seq           = pstTfCtx->usTfCmdSendSeq++;
    pstTfCmd->data_type     = eDataType;
    pstTfCmd->option        = MRS_PLC_TF_OPTION_DATA_COLLECT;
    (HI_VOID)memcpy_s(pstTfCmd->mac_addr, HI_PLC_MAC_ADDR_LEN, pstCcoCtx->ucMainNodeAddr, HI_PLC_MAC_ADDR_LEN);

    memset_s(&stPlcFrame, sizeof(MRS_PLC_FRAME_DATA_STRU), 0, sizeof(MRS_PLC_FRAME_DATA_STRU));
    stPlcFrame.usId = ID_MRS_CMD_TF_PLC;
    stPlcFrame.pucPayload = aucPayload;
    stPlcFrame.usPayloadLen = usPayloadLen;
    memcpy_s(stPlcFrame.ucMac, HI_PLC_MAC_ADDR_LEN, aucDstMac, HI_PLC_MAC_ADDR_LEN);

    ret = MRS_SendPlcFrame(&stPlcFrame);
    HI_DIAG_LOG_BUF(MRS_FILE_LOG_BUF_1014, HI_DIAG_MT("[CCO-TF]: send data collect"), aucPayload, usPayloadLen);
    HI_DIAG_LOG_MSG_E1(MRS_FILE_LOG_FLAG_018, HI_DIAG_MT("[CCO-TF]: send data collect ret"), ret);
    mrsToolsFree(aucPayload);
}

HI_VOID mrsTfCcoHandleQueryNtbDataCallback(TF_QUERY_NTB_DATA_INFO_S *pstQueryData)
{
    if (pstQueryData) {
        mrsTfCcoSendDataCollectCmd(pstQueryData->dest_mac, MRS_PLC_TF_DATA_TYPE_PERIOD);
    }
}

HI_VOID mrsTfCcoSendResultReqCmd(HI_U8 aucDstMac[HI_PLC_MAC_ADDR_LEN])
{
    MRS_CCO_SRV_CTX_STRU *pstCcoCtx = mrsCcoGetContext();
    MRS_CCO_TF_CTX_S *pstTfCtx = mrsTfCcoGetCtx();
    MRS_PLC_FRAME_DATA_STRU stPlcFrame;
    MRS_PLC_TF_CMD_S *pstTfCmd;
    HI_U8 *aucPayload = HI_NULL;
    HI_U16 usPayloadLen = sizeof(MRS_PLC_TF_CMD_S);
    HI_U32 ret = HI_ERR_SUCCESS;

    aucPayload = mrsToolsMalloc(usPayloadLen);
    if (!aucPayload)
    {
        return;
    }
    memset_s(aucPayload, usPayloadLen, 0, usPayloadLen);

    pstTfCmd = (MRS_PLC_TF_CMD_S *)aucPayload;
    pstTfCmd->interface_ver = MRS_CMD_STRU_VER_R1;
    pstTfCmd->stru_len      = sizeof(MRS_PLC_TF_CMD_S);
    pstTfCmd->dir           = MRS_PLC_DL_FLG;
    pstTfCmd->from          = MRS_PLC_FROM_START;
    pstTfCmd->phase         = PHASE_ALL;
    pstTfCmd->seq           = pstTfCtx->usTfCmdSendSeq++;
    pstTfCmd->data_type     = MRS_PLC_TF_DATA_TYPE_PERIOD;
    pstTfCmd->option        = MRS_PLC_TF_OPTION_RESULT_REQ;
    (HI_VOID)memcpy_s(pstTfCmd->mac_addr, HI_PLC_MAC_ADDR_LEN, pstCcoCtx->ucMainNodeAddr, HI_PLC_MAC_ADDR_LEN);

    memset_s(&stPlcFrame, sizeof(MRS_PLC_FRAME_DATA_STRU), 0, sizeof(MRS_PLC_FRAME_DATA_STRU));
    stPlcFrame.usId = ID_MRS_CMD_TF_PLC;
    stPlcFrame.pucPayload = aucPayload;
    stPlcFrame.usPayloadLen = usPayloadLen;
    memcpy_s(stPlcFrame.ucMac, HI_PLC_MAC_ADDR_LEN, aucDstMac, HI_PLC_MAC_ADDR_LEN);

    ret = MRS_SendPlcFrame(&stPlcFrame);
    HI_DIAG_LOG_BUF(MRS_FILE_LOG_BUF_1015, HI_DIAG_MT("[CCO-TF]: send result req"), aucPayload, usPayloadLen);
    HI_DIAG_LOG_MSG_E1(MRS_FILE_LOG_FLAG_019, HI_DIAG_MT("[CCO-TF]: send result req ret"), ret);
    mrsToolsFree(aucPayload);
}

// 国网标志: 台区识别是否启动 (收到05F6)
HI_BOOL mrsTfCcoIsEnable(HI_VOID)
{
    MRS_CCO_TF_CTX_S *pstTfCtx = mrsTfCcoGetCtx();

    if (pstTfCtx->ucNotifyMode > 0) {
        return HI_TRUE;
    } else {
        return HI_FALSE;
    }
}

#endif // CCO
HI_END_HEADER
