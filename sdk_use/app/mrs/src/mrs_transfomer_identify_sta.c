#include "mrs_transfomer_identify_sta.h"
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

#if defined(PRODUCT_CFG_PRODUCT_TYPE_STA)
// 数据采集状态
enum
{
    MRS_TF_COLLECT_STATUS_DEFAULT = 0,  // 默认状态
    MRS_TF_COLLECT_STATUS_COLLECTING,   // 正在采集
    MRS_TF_COLLECT_STATUS_TIMEOUT,      // 采集超时(未完成)
    MRS_TF_COLLECT_STATUS_COMPLETE,     // 采集完成
};

typedef struct
{
    HI_U8 ucCollectSeq;         // 采集序列号
    HI_U8 ucCollectStatus;      // 采集状态
    HI_U8 ucNeedCollectNum;     // 需要采集个数
    HI_U8 ucCollectPeriod;      // 采集周期，单位s

    HI_U32 ulTryStartNtb;       // 尝试起始采集NTB

    HI_U32 ulRealStartNtb;      // 实际起始采集NTB

    HI_U8  ucTryNum;            // 尝试采集次数
    HI_U8  ucHaveCollectNum;    // 已经采集个数
    HI_U8  ucRcvDataCollectSeq; // 接收数据的采集序列号
    HI_U8  ucRcvDataPhaseMask;  // 接收同一序列号的各相位数据掩码

    HI_U16 *pusData;            // 采集的数据
} MRS_TF_COLLECT_INFO_S;

typedef struct
{
    HI_U8   aucCcoAddr[HI_PLC_MAC_ADDR_LEN];    // CCO的地址，可以是主节点地址也可以是MAC地址
    HI_U16  usSeq;
} MRS_TF_NET_INFO_S;

typedef struct
{
    HI_U16  usSeq;
    HI_BOOL bSeqValid;
    HI_U8   pad;

    MRS_TF_COLLECT_INFO_S stVoltageCollect;
    MRS_TF_COLLECT_INFO_S stFreqencyCollect;
    MRS_TF_COLLECT_INFO_S stPeriodCollect;

    MRS_TF_NET_INFO_S astNetInfos[MRS_TF_MAX_MEIGHBOR_NUM];
} MRS_TF_STA_CTX_S;

HI_BOOL mrsTfStaCheckIsRcvCmd(MRS_PLC_TF_CMD_S *pstTfCmd, HI_U16 usCmdSize, HI_BOOL bIsMyNetFrame);
HI_BOOL mrsTfStaCheckIsRcvBySeq(MRS_PLC_TF_CMD_S *pstTfCmd);
HI_VOID mrsTfStaHandleRxCollectNtbData(TF_STA_REPORT_NTB_DATA_INFO_S *pstData);
HI_U32  mrsTfStaHandleCollectStartCmd(MRS_PLC_TF_CMD_S *pstTfCmd, HI_U16 usCmdSize);
HI_U32  mrsTfStaStartCollectData(MRS_PLC_TF_DATA_TYPE_E eDataType, MRS_PLC_TF_COLLECT_START_S *pstCollectInfo);
HI_VOID mrsTfStaHandleRxVoltageData(HI_U8 *aucData);
HI_VOID mrsTfStaHandleRxFreqencyData(HI_U8 *aucData);
HI_U32  mrsTfStaStartCollectPowerPeriod(MRS_PLC_TF_COLLECT_START_S *pstCollectInfo);
HI_U32  mrsTfStaHandleDataCollectCmd(MRS_PLC_TF_CMD_S *pstTfCmd, HI_U16 usCmdSize);
HI_U32  mrsTfStaHandleResultReqCmd(MRS_PLC_TF_CMD_S *pstTfCmd, HI_U16 usCmdSize);
HI_U32  mrsTfStaSendResultInd(HI_U16 usSeq, HI_U8 *pucMeterAddr, HI_U16 usTei, HI_MDM_SNR_TF_RESULT_S *pstTfResult);

MRS_TF_STA_CTX_S g_stTfStaCtx = {0};

#define MRS_TF_STA_GET_CTX()    (&g_stTfStaCtx)

HI_VOID mrsTfStaHandleMsg(HI_SYS_QUEUE_MSG_S *pstMsg)
{
    if (pstMsg->ulMsgId != EN_MRS_FW_MSG_TF)
    {
        return;
    }

    switch (pstMsg->ulParam[0])
    {
        case MRS_TF_MSG_ID_STA_REPORT_NTB_DATA:
            mrsTfStaHandleRxCollectNtbData((TF_STA_REPORT_NTB_DATA_INFO_S *)pstMsg->ulParam[1]);
            mrsToolsFree(pstMsg->ulParam[1]);
            break;

        default:
            break;
    }
}

MRS_TF_COLLECT_INFO_S *mrsTfStaGetCollectCtx(MRS_PLC_TF_DATA_TYPE_E eDataType)
{
    MRS_TF_STA_CTX_S *pstTfStaCtx = MRS_TF_STA_GET_CTX();

    switch (eDataType)
    {
        case MRS_PLC_TF_DATA_TYPE_VOLTAGE:
            return &pstTfStaCtx->stVoltageCollect;

        case MRS_PLC_TF_DATA_TYPE_FREQUENCY:
            return &pstTfStaCtx->stFreqencyCollect;

        case MRS_PLC_TF_DATA_TYPE_PERIOD:
            return &pstTfStaCtx->stPeriodCollect;

        default:
            break;
    }

    return HI_NULL;
}

HI_U32 mrsCmdStaHandleTfCmd(HI_IN HI_U16 usId, HI_PVOID pReqPacket, HI_U16 usReqPacketSize)
{
    MRS_PLC_FRAME_DATA_STRU *pstPlcFrame = (MRS_PLC_FRAME_DATA_STRU *)pReqPacket;
    MRS_PLC_TF_CMD_S *pstTfCmd;
    HI_U32 ret = HI_ERR_SUCCESS;

    if (ID_MRS_CMD_TF_PLC != usId || !pstPlcFrame || !pstPlcFrame->pucPayload) {
        return HI_ERR_FAILURE;
    }

    pstTfCmd = (MRS_PLC_TF_CMD_S *)pstPlcFrame->pucPayload;
    if (mrsTfStaCheckIsRcvCmd(pstTfCmd, pstPlcFrame->usPayloadLen, pstPlcFrame->bIsMyNetFrame) == HI_FALSE) {
        return HI_ERR_SUCCESS;
    }

    HI_DIAG_LOG_MSG_E2(MRS_FILE_LOG_FLAG_001, HI_DIAG_MT("[STA-TF] rcv cmd [seq] [option]"), pstTfCmd->seq, pstTfCmd->option);

    switch (pstTfCmd->option) {
        case MRS_PLC_TF_OPTION_COLLECT_START:
            ret = mrsTfStaHandleCollectStartCmd(pstTfCmd, pstPlcFrame->usPayloadLen);
            break;
        case MRS_PLC_TF_OPTION_DATA_COLLECT:
            ret = mrsTfStaHandleDataCollectCmd(pstTfCmd, pstPlcFrame->usPayloadLen);
            break;
        case MRS_PLC_TF_OPTION_DATA_NOTIFY:
            ret = mrsTfHandleNotifyNtbDataCmd(pstPlcFrame->ucRcvFreq, pstPlcFrame->ulRcvNid, pstTfCmd, pstPlcFrame->usPayloadLen);
            HI_DIAG_LOG_BUF(MRS_FILE_LOG_BUF_1016, HI_DIAG_MT("[STA-TF]: rcv notify cmd"), pstTfCmd, pstPlcFrame->usPayloadLen);
            break;
        case MRS_PLC_TF_OPTION_RESULT_REQ:
            ret = mrsTfStaHandleResultReqCmd(pstTfCmd, pstPlcFrame->usPayloadLen);
            break;

        default:
            break;
    }

    return ret;
}

HI_BOOL mrsTfStaCheckIsRcvCmd(MRS_PLC_TF_CMD_S *pstTfCmd, HI_U16 usCmdSize, HI_BOOL bIsMyNetFrame)
{
    MRS_TF_STA_CTX_S *pstTfCtx = MRS_TF_STA_GET_CTX();
    MRS_TF_NET_INFO_S stNetInfo = {{0,},0xFFFF};
    HI_U8 ucNetIdx = MRS_TF_MAX_MEIGHBOR_NUM;
    HI_U8 i = 0;

    if (MRS_PLC_TF_OPTION_DATA_NOTIFY == pstTfCmd->option && MRS_PLC_TF_DATA_TYPE_PERIOD == pstTfCmd->data_type) {
        if (usCmdSize < sizeof(MRS_PLC_TF_CMD_S) + MRS_CMD_TF_DATA_NOTIFY_FIXED_LEN || mrsToolsInvalidMac(pstTfCmd->mac_addr)) {
            return HI_FALSE;
        }

        for (i = 0; i < MRS_TF_MAX_MEIGHBOR_NUM; i++) {
            if (mrsToolsMemEq(pstTfCmd->mac_addr, pstTfCtx->astNetInfos[i].aucCcoAddr, HI_PLC_MAC_ADDR_LEN)) {
                memcpy_s(&stNetInfo, sizeof(stNetInfo), &pstTfCtx->astNetInfos[i], sizeof(pstTfCtx->astNetInfos[i]));
                ucNetIdx = i;
                break;
            }
        }

        for (i = HI_MIN(ucNetIdx, MRS_TF_MAX_MEIGHBOR_NUM - 1); i > 0; i--) {
            memcpy_s(&pstTfCtx->astNetInfos[i], sizeof(MRS_TF_NET_INFO_S), &pstTfCtx->astNetInfos[i-1], sizeof(MRS_TF_NET_INFO_S));
        }
        memcpy_s(pstTfCtx->astNetInfos[0].aucCcoAddr, HI_PLC_MAC_ADDR_LEN, pstTfCmd->mac_addr, HI_PLC_MAC_ADDR_LEN);
        pstTfCtx->astNetInfos[0].usSeq = pstTfCmd->seq;

        if (pstTfCmd->seq == stNetInfo.usSeq) {
            return HI_FALSE;
        } else {
            if (HI_TRUE == bIsMyNetFrame) {
                pstTfCtx->usSeq = pstTfCmd->seq;
                pstTfCtx->bSeqValid = HI_TRUE;
            }
            return HI_TRUE;
        }
    } else {
        if (usCmdSize < sizeof(MRS_PLC_TF_CMD_S)) {
            return HI_FALSE;
        }

        if (HI_TRUE == bIsMyNetFrame) {
            if (HI_TRUE == pstTfCtx->bSeqValid && pstTfCtx->usSeq == pstTfCmd->seq) {
                return HI_FALSE;
            } else {
                pstTfCtx->usSeq = pstTfCmd->seq;
                pstTfCtx->bSeqValid = HI_TRUE;
                return HI_TRUE;
            }
        } else {
            return HI_FALSE;
        }
    }
}

HI_U32 mrsTfStaHandleCollectStartCmd(MRS_PLC_TF_CMD_S *pstTfCmd, HI_U16 usCmdSize)
{
    HI_DIAG_LOG_BUF(MRS_FILE_LOG_BUF_1000, HI_DIAG_MT("[STA-TF]: rcv collect start cmd"), pstTfCmd, usCmdSize);

    if (usCmdSize != sizeof(MRS_PLC_TF_CMD_S) + sizeof(MRS_PLC_TF_COLLECT_START_S))
    {
        return HI_ERR_BAD_FRAME;
    }

    switch (pstTfCmd->data_type)
    {
        case MRS_PLC_TF_DATA_TYPE_VOLTAGE:
        case MRS_PLC_TF_DATA_TYPE_FREQUENCY:
            mrsTfStaStartCollectData((MRS_PLC_TF_DATA_TYPE_E)pstTfCmd->data_type, (MRS_PLC_TF_COLLECT_START_S *)pstTfCmd->data);
            break;
        case MRS_PLC_TF_DATA_TYPE_PERIOD:
            mrsTfStaStartCollectPowerPeriod((MRS_PLC_TF_COLLECT_START_S *)pstTfCmd->data);
            break;
        default:
            break;
    }

    return HI_ERR_SUCCESS;
}

HI_U32 mrsTfStaStartCollectData(MRS_PLC_TF_DATA_TYPE_E eDataType, MRS_PLC_TF_COLLECT_START_S *pstCollectInfo)
{
    MRS_TF_COLLECT_INFO_S *pstCollectCtx = mrsTfStaGetCollectCtx(eDataType);
    HI_U32 ulRemainDuration = 0;
    HI_U32 ulDurationTmp = 0;
    HI_U32 ulCurNtb = 0;;
    HI_BOOL bIsSync = HI_FALSE;

    if (pstCollectInfo->collect_seq == pstCollectCtx->ucCollectSeq || pstCollectInfo->collect_num > MRS_TF_NTB_COLLECT_NUM_MAX) {
        return HI_ERR_FAILURE;
    }

    pstCollectCtx->ucCollectStatus  = MRS_TF_COLLECT_STATUS_DEFAULT;
    pstCollectCtx->ucCollectSeq     = pstCollectInfo->collect_seq;
    pstCollectCtx->ucNeedCollectNum = pstCollectInfo->collect_num;
    pstCollectCtx->ucCollectPeriod  = pstCollectInfo->collect_period;
    pstCollectCtx->ulTryStartNtb    = pstCollectInfo->start_ntb;

    pstCollectCtx->ulRealStartNtb   = 0;
    pstCollectCtx->ucTryNum         = 0;
    pstCollectCtx->ucHaveCollectNum = 0;

    if (0 == pstCollectCtx->ucCollectPeriod) {
        pstCollectCtx->ucCollectPeriod = MRS_TF_COLLECT_PERIOD_DEFAULT;
    }

    if (pstCollectCtx->pusData) {
        mrsToolsFree(pstCollectCtx->pusData);
        pstCollectCtx->pusData = HI_NULL;
    }

    pstCollectCtx->pusData = mrsToolsMalloc(pstCollectCtx->ucNeedCollectNum * sizeof(HI_U16));
    if (!pstCollectCtx->pusData) {
        return HI_ERR_MALLOC_FAILUE;
    }
    memset_s(pstCollectCtx->pusData, pstCollectCtx->ucNeedCollectNum * sizeof(HI_U16), 0, pstCollectCtx->ucNeedCollectNum * sizeof(HI_U16));

    ulCurNtb = HI_MDM_GetNtb(&bIsSync);
    if (HI_TRUE == bIsSync) {
        ulRemainDuration = mrsTimeSubDuration(pstCollectCtx->ulTryStartNtb, ulCurNtb);
        ulDurationTmp = mrsTimeSubDuration(ulCurNtb, pstCollectCtx->ulTryStartNtb);
    }

    if (HI_FALSE == bIsSync || ulDurationTmp <= ulRemainDuration) {
        mrsTfStaStartDataCollectProcess(eDataType);
    } else {
        ulRemainDuration = MRS_NTB_TO_MS(ulRemainDuration);

        if (MRS_PLC_TF_DATA_TYPE_VOLTAGE == eDataType) {
            MRS_StartTimer(MRS_STA_TIMER_COLLECT_VOLTAGE_BEGIN, ulRemainDuration, HI_SYS_TIMER_ONESHOT);
        } else if (MRS_PLC_TF_DATA_TYPE_FREQUENCY == eDataType) {
            MRS_StartTimer(MRS_STA_TIMER_COLLECT_FREQENCY_BEGIN, ulRemainDuration, HI_SYS_TIMER_ONESHOT);
        } else {
        }
    }

    return HI_ERR_SUCCESS;
}

HI_VOID mrsTfStaStartDataCollectProcess(MRS_PLC_TF_DATA_TYPE_E eDataType)
{
    MRS_TF_COLLECT_INFO_S *pstCollectCtx = mrsTfStaGetCollectCtx(eDataType);
    HI_BOOL bIsSync =  HI_FALSE;

    pstCollectCtx->ucCollectStatus = MRS_TF_COLLECT_STATUS_COLLECTING;
    pstCollectCtx->ulRealStartNtb = HI_MDM_GetNtb(&bIsSync);

    mrsTfStaRunDataCollectProcess(eDataType);
}

HI_VOID mrsTfStaRunDataCollectProcess(MRS_PLC_TF_DATA_TYPE_E eDataType)
{
    MRS_STA_SRV_CTX_STRU *pstStaCtx = mrsStaGetContext();
    MRS_TF_COLLECT_INFO_S *pstCollectCtx = mrsTfStaGetCollectCtx(eDataType);
    MRS_PROTO645_FRAME_INF stFrameInf;
    HI_U8 *aucFrame645 = HI_NULL;
    HI_U16 usFrameSize = 0;
    MRS_METER_ITEM_S *pstMeter = HI_NULL;
    MRS_STA_ITEM * item = HI_NULL;

    if (!pstCollectCtx || pstCollectCtx->ucCollectStatus != MRS_TF_COLLECT_STATUS_COLLECTING) {
        return;
    }

    if (pstCollectCtx->ucTryNum >= MRS_TF_COLLECT_DATA_TRY_MAX) {
        pstCollectCtx->ucCollectStatus = MRS_TF_COLLECT_STATUS_TIMEOUT;
        return;
    }

    pstCollectCtx->ucTryNum++;

    (hi_void)memset_s(&stFrameInf, sizeof(stFrameInf), 0, sizeof(stFrameInf));

    do {
        pstMeter = mrsStaGetFirstValidMeter();
        if (!pstMeter) {
            break;
        }

        if (MRS_PLC_TF_DATA_TYPE_VOLTAGE == eDataType) {
            stFrameInf.ulDi = MRS_TF_VOLTAGE_645_DI;
        } else if (MRS_PLC_TF_DATA_TYPE_FREQUENCY == eDataType) {
            stFrameInf.ulDi = MRS_TF_FREQENCY_645_DI;
        } else {
            break;
        }

        memcpy_s(stFrameInf.aucAddr, sizeof(stFrameInf.aucAddr), pstMeter->ucMeterAddr, HI_METER_ADDR_LEN);
        stFrameInf.ucCtrl = MRS_645_FRAME_CONTROL_READ_DATA;
        stFrameInf.ucDiSize = MRS_645_FRAME_DATA_DI_SIZE;

        if (mrsGen645Frame(&stFrameInf, &aucFrame645, &usFrameSize) != HI_ERR_SUCCESS) {
            break;
        }

        item = mrsToolsMalloc(sizeof(MRS_STA_ITEM) + usFrameSize);
        if (item) {
            memset_s(item, sizeof(MRS_STA_ITEM) + usFrameSize, 0, sizeof(MRS_STA_ITEM) + usFrameSize);

            item->from     = MRS_STA_QUEUE_FROM_STA;
            item->time_out = (HI_U8)MRS_MS_TO_100MS(mrsGet645FrameHeadTimeout(MRS_STA_RM_CFG_LOCAL));
            item->id       = ID_MRS_CMD_TF_PLC;
            item->len      = usFrameSize;
            item->buf      = (HI_U8 *)item + sizeof(MRS_STA_ITEM);
            item->ucProtocol = METER_PROTO_645_2007;
            item->try_max = mrsGet645TimeoutRetryCfg(MRS_STA_RM_CFG_AUTO);;
            item->bAutoStrategy = HI_TRUE;
            item->ucFrameTimeout = (HI_U8)MRS_MS_TO_100MS(mrsGet645FrameInterval(MRS_STA_RM_CFG_AUTO));
            (HI_VOID)memcpy_s(item->buf, usFrameSize, aucFrame645, usFrameSize);

            if (mrsStaTryEnQueue(&pstStaCtx->stQueModule, item) == HI_FALSE) {
                HI_DIAG_LOG_MSG_E0(MRS_FILE_LOG_FLAG_025, HI_DIAG_MT("[STA-TF] en queue fail"));
                mrsToolsFree(item);
            } else {
                HI_DIAG_LOG_MSG_E0(MRS_FILE_LOG_FLAG_026, HI_DIAG_MT("[STA-TF] en queue succ"));
            }

            mrsStaNotifyQueue();
        }

        mrsToolsFree(aucFrame645);
    } while(0);

    if (MRS_PLC_TF_DATA_TYPE_VOLTAGE == eDataType)
    {
        MRS_StartTimer(MRS_STA_TIMER_COLLECT_VOLTAGE, MRS_SEC_TO_MS(pstCollectCtx->ucCollectPeriod), HI_SYS_TIMER_ONESHOT);
    }
    else if (MRS_PLC_TF_DATA_TYPE_FREQUENCY == eDataType)
    {
        MRS_StartTimer(MRS_STA_TIMER_COLLECT_FREQENCY, MRS_SEC_TO_MS(pstCollectCtx->ucCollectPeriod), HI_SYS_TIMER_ONESHOT);
    }
    else
    {}
}

HI_U32 mrsTfStaRxFrame645(MRS_645_FRAME_STRU *pst645Hrd)
{
    HI_U32 ret = HI_ERR_FAILURE;
    HI_U32 di = 0;

    if (pst645Hrd->ucDataRealmLen < sizeof(di))
    {
        return HI_ERR_BAD_FRAME;
    }

    (HI_VOID)memcpy_s(&di, sizeof(di), pst645Hrd->ucDataRealm, sizeof(di));

    if (MRS_TF_VOLTAGE_645_DI == di && sizeof(di) + MRS_TF_VOLTAGE_645_DATA_LEN == pst645Hrd->ucDataRealmLen)
    {
        mrsTfStaHandleRxVoltageData(pst645Hrd->ucDataRealm + sizeof(di));
        ret = HI_ERR_SUCCESS;
    }

    if (MRS_TF_FREQENCY_645_DI == di && sizeof(di) + MRS_TF_FREQENCY_645_DATA_LEN == pst645Hrd->ucDataRealmLen)
    {
        mrsTfStaHandleRxFreqencyData(pst645Hrd->ucDataRealm + sizeof(di));
        ret = HI_ERR_SUCCESS;
    }

    return ret;
}

HI_VOID mrsTfStaHandleRxVoltageData(HI_U8 *aucData)
{
    MRS_TF_STA_CTX_S *pstTfStaCtx = MRS_TF_STA_GET_CTX();
    MRS_TF_COLLECT_INFO_S *pstCollectCtx = &pstTfStaCtx->stVoltageCollect;
    HI_U16 ausVoltage[PHASE_CNT];

    (HI_VOID)memcpy_s(ausVoltage, PHASE_CNT * sizeof(HI_S16), aucData, MRS_TF_VOLTAGE_645_DATA_LEN);

    if (MRS_TF_COLLECT_STATUS_COLLECTING == pstCollectCtx->ucCollectStatus
        && pstCollectCtx->ucHaveCollectNum < pstCollectCtx->ucNeedCollectNum
        && pstCollectCtx->pusData)
    {
        pstCollectCtx->pusData[pstCollectCtx->ucHaveCollectNum] = ausVoltage[0];
        pstCollectCtx->ucHaveCollectNum++;

        if (pstCollectCtx->ucHaveCollectNum >= pstCollectCtx->ucNeedCollectNum)
        {
            pstCollectCtx->ucCollectStatus = MRS_TF_COLLECT_STATUS_COMPLETE;
            HI_DIAG_LOG_MSG_E1(MRS_FILE_LOG_FLAG_021, HI_DIAG_MT("[STA-TF] voltage collect complete"), ausVoltage[0]);
        }
    }
}

HI_VOID mrsTfStaHandleRxFreqencyData(HI_U8 *aucData)
{
    MRS_TF_STA_CTX_S *pstTfStaCtx = MRS_TF_STA_GET_CTX();
    MRS_TF_COLLECT_INFO_S *pstCollectCtx = &pstTfStaCtx->stFreqencyCollect;
    HI_U16 usFreqency;

    (HI_VOID)memcpy_s(&usFreqency, sizeof(HI_U16), aucData, MRS_TF_FREQENCY_645_DATA_LEN);

    if (MRS_TF_COLLECT_STATUS_COLLECTING == pstCollectCtx->ucCollectStatus
        && pstCollectCtx->ucHaveCollectNum < pstCollectCtx->ucNeedCollectNum
        && pstCollectCtx->pusData)
    {
        pstCollectCtx->pusData[pstCollectCtx->ucHaveCollectNum] = usFreqency;
        pstCollectCtx->ucHaveCollectNum++;

        if (pstCollectCtx->ucHaveCollectNum >= pstCollectCtx->ucNeedCollectNum)
        {
            pstCollectCtx->ucCollectStatus = MRS_TF_COLLECT_STATUS_COMPLETE;
            HI_DIAG_LOG_MSG_E1(MRS_FILE_LOG_FLAG_022, HI_DIAG_MT("[STA-TF] freqency collect complete"), usFreqency);
        }
    }
}

HI_U32 mrsTfStaStartCollectPowerPeriod(MRS_PLC_TF_COLLECT_START_S *pstCollectInfo)
{
    MRS_TF_COLLECT_INFO_S *pstCollectCtx = mrsTfStaGetCollectCtx(MRS_PLC_TF_DATA_TYPE_PERIOD);


    if (((pstCollectInfo->collect_seq == pstCollectCtx->ucCollectSeq)
          && (pstCollectInfo->start_ntb == pstCollectCtx->ulTryStartNtb))
          || (pstCollectInfo->collect_num > MRS_TF_NTB_COLLECT_NUM_MAX)) {
        HI_DIAG_LOG_MSG_E3(MRS_FILE_LOG_FLAG_024, HI_DIAG_MT("[STA-TF] period start fail [seq] [num] [start_ntb]"),
            pstCollectInfo->collect_seq, pstCollectInfo->collect_num, pstCollectInfo->start_ntb);
        return HI_ERR_FAILURE;
    }

    pstCollectCtx->ucCollectStatus  = MRS_TF_COLLECT_STATUS_DEFAULT;
    pstCollectCtx->ucCollectSeq     = pstCollectInfo->collect_seq;
    pstCollectCtx->ucNeedCollectNum = pstCollectInfo->collect_num;
    pstCollectCtx->ulTryStartNtb    = pstCollectInfo->start_ntb;

    pstCollectCtx->ulRealStartNtb   = 0;
    pstCollectCtx->ucTryNum         = 0;
    pstCollectCtx->ucHaveCollectNum = 0;

    if (pstCollectCtx->pusData) {
        mrsToolsFree(pstCollectCtx->pusData);
        pstCollectCtx->pusData = HI_NULL;
    }

    pstCollectCtx->pusData = mrsToolsMalloc(pstCollectCtx->ucNeedCollectNum * sizeof(HI_U16));
    if (!pstCollectCtx->pusData) {
        return HI_ERR_MALLOC_FAILUE;
    }
    memset_s(pstCollectCtx->pusData, pstCollectCtx->ucNeedCollectNum * sizeof(HI_U16), 0, pstCollectCtx->ucNeedCollectNum * sizeof(HI_U16));

    pstCollectCtx->ucCollectStatus = MRS_TF_COLLECT_STATUS_COLLECTING;

    return HI_MDM_notifyStaCollectNtbData(pstCollectCtx->ucCollectSeq, pstCollectCtx->ulTryStartNtb, pstCollectCtx->ucNeedCollectNum);
}

HI_VOID mrsTfStaHandleRxCollectNtbData(TF_STA_REPORT_NTB_DATA_INFO_S *pstData)
{
    MRS_TF_COLLECT_INFO_S *pstCollectCtx = mrsTfStaGetCollectCtx(MRS_PLC_TF_DATA_TYPE_PERIOD);

    if (MRS_TF_COLLECT_STATUS_COLLECTING != pstCollectCtx->ucCollectStatus || pstData->collect_num < 1) {
        return;
    }

    pstCollectCtx->ucHaveCollectNum = HI_MIN(pstData->collect_num - 1, MRS_TF_NTB_COLLECT_NUM_MAX);
    pstCollectCtx->ucHaveCollectNum = HI_MIN(pstCollectCtx->ucNeedCollectNum, pstCollectCtx->ucHaveCollectNum);
    pstCollectCtx->ulRealStartNtb = pstData->ntb_buf[0];

    mrsTfCreateNtbDiffSeq(pstData->ntb_buf, pstCollectCtx->ucHaveCollectNum, pstCollectCtx->pusData);

    pstCollectCtx->ucCollectStatus = MRS_TF_COLLECT_STATUS_COMPLETE;
    HI_DIAG_LOG_MSG_E0(MRS_FILE_LOG_FLAG_023, HI_DIAG_MT("[STA-TF] period collect complete"));
}

HI_U32 mrsTfStaHandleDataCollectCmd(MRS_PLC_TF_CMD_S *pstTfCmd, HI_U16 usCmdSize)
{
    MRS_TF_COLLECT_INFO_S *pstCollectCtx = mrsTfStaGetCollectCtx((MRS_PLC_TF_DATA_TYPE_E)pstTfCmd->data_type);
    MRS_PLC_FRAME_DATA_STRU stPlcFrame = {0};
    MRS_PLC_TF_CMD_S *pstUpFrame;
    MRS_CMD_TF_DATA_NOTIFY_SING_EDGE_S *pstData;
    HI_U8 *pucPayload = HI_NULL;
    HI_U16 usPayloadLen = 0;
    HI_U8 ucReportNum = 0;
    HI_MDM_NETWORK_ATTRIBUTE_S stStaInfo = {0};
    HI_U32 ret = HI_ERR_SUCCESS;

    HI_DIAG_LOG_BUF(MRS_FILE_LOG_BUF_1001, HI_DIAG_MT("[STA-TF]: rcv data collect cmd"), pstTfCmd, usCmdSize);

    if (sizeof(MRS_PLC_TF_CMD_S) != usCmdSize || !pstCollectCtx) {
        return HI_ERR_BAD_FRAME;
    }

    if (MRS_TF_COLLECT_STATUS_COMPLETE == pstCollectCtx->ucCollectStatus) {
        ucReportNum = HI_MIN(pstCollectCtx->ucHaveCollectNum, MRS_TF_NTB_COLLECT_NUM_MAX);
    }

    HI_MDM_GetNetworkAttribute(&stStaInfo);

    usPayloadLen = sizeof(MRS_PLC_TF_CMD_S) + sizeof(MRS_CMD_TF_DATA_NOTIFY_SING_EDGE_S) + ucReportNum * sizeof(HI_U16);
    pucPayload = mrsToolsMalloc(usPayloadLen);
    if (!pucPayload) {
        return HI_ERR_MALLOC_FAILUE;
    }
    memset_s(pucPayload, usPayloadLen, 0, usPayloadLen);

    pstUpFrame = (MRS_PLC_TF_CMD_S *)pucPayload;
    pstUpFrame->interface_ver = MRS_CMD_STRU_VER_R1;
    pstUpFrame->stru_len      = sizeof(MRS_PLC_TF_CMD_S);
    pstUpFrame->dir           = MRS_PLC_UP_FLG;
    pstUpFrame->from          = MRS_PLC_FROM_FOLLOW;
    pstUpFrame->phase         = PHASE_ALL;
    pstUpFrame->seq           = pstTfCmd->seq;
    pstUpFrame->data_type     = pstTfCmd->data_type;
    pstUpFrame->option        = MRS_PLC_TF_OPTION_DATA_NOTIFY;
    (HI_VOID)memcpy_s(pstUpFrame->mac_addr, HI_PLC_MAC_ADDR_LEN, stStaInfo.MyMac, HI_PLC_MAC_ADDR_LEN);

    pstData = (MRS_CMD_TF_DATA_NOTIFY_SING_EDGE_S *)pstUpFrame->data;
    pstData->tei            = stStaInfo.MyTei;
    pstData->collect_edge   = MRS_CMD_TF_EDGE_RISE;
    pstData->collect_seq    = pstCollectCtx->ucCollectSeq;
    pstData->total_num      = ucReportNum;
    pstData->base_ntb       = pstCollectCtx->ulRealStartNtb;
    pstData->notify_num[0]  = ucReportNum;
    (HI_VOID)memcpy_s(pstData->value, ucReportNum * sizeof(HI_U16), pstCollectCtx->pusData, ucReportNum * sizeof(HI_U16));

    memset_s(&stPlcFrame, sizeof(stPlcFrame), 0, sizeof(stPlcFrame));
    stPlcFrame.usId = ID_MRS_CMD_TF_PLC;
    stPlcFrame.pucPayload = pucPayload;
    stPlcFrame.usPayloadLen = usPayloadLen;

    ret = MRS_SendPlcFrame(&stPlcFrame);
    HI_DIAG_LOG_BUF(MRS_FILE_LOG_BUF_1002, HI_DIAG_MT("[STA-TF]: send data collect cmd"), pucPayload, HI_MIN(64, usPayloadLen));
    HI_DIAG_LOG_MSG_E1(MRS_FILE_LOG_FLAG_002, HI_DIAG_MT("[STA-TF]: send data collect ret"), ret);
    mrsToolsFree(pucPayload);

    return HI_ERR_SUCCESS;
}

HI_U32 mrsTfStaHandleResultReqCmd(MRS_PLC_TF_CMD_S *pstTfCmd, HI_U16 usCmdSize)
{
    HI_MDM_SNR_TF_RESULT_S stTfResult = {0};
    HI_MDM_NETWORK_ATTRIBUTE_S stStaInfo = {0};

    HI_DIAG_LOG_BUF(MRS_FILE_LOG_BUF_1003, HI_DIAG_MT("[STA-TF]: rcv result req cmd"), pstTfCmd, usCmdSize);

    if (sizeof(MRS_PLC_TF_CMD_S) != usCmdSize) {
        return HI_ERR_BAD_FRAME;
    }

    HI_MDM_GetNetworkAttribute(&stStaInfo);
    HI_MDM_GetSnrTfResult(&stTfResult);

    return mrsTfStaSendResultInd(pstTfCmd->seq, stStaInfo.MyMac, stStaInfo.MyTei, &stTfResult);
}

HI_U32 mrsTfStaSendResultInd(HI_U16 usSeq, HI_U8 *pucMeterAddr, HI_U16 usTei, HI_MDM_SNR_TF_RESULT_S *pstTfResult)
{
    MRS_PLC_FRAME_DATA_STRU stPlcFrame = {0};
    MRS_PLC_TF_CMD_S *pstUpFrame;
    MRS_CMD_TF_RESULT_IND_S *pstResultInd;
    HI_U8 *pucPayload = HI_NULL;
    HI_U16 usPayloadLen = 0;
    HI_U32 ret = HI_ERR_SUCCESS;

    usPayloadLen = sizeof(MRS_PLC_TF_CMD_S) + MRS_CMD_TF_RESULT_IND_DATA_SIZE;
    pucPayload = mrsToolsMalloc(usPayloadLen);
    if (!pucPayload) {
        return HI_ERR_MALLOC_FAILUE;
    }
    memset_s(pucPayload, usPayloadLen, 0, usPayloadLen);

    pstUpFrame = (MRS_PLC_TF_CMD_S *)pucPayload;
    pstUpFrame->interface_ver = MRS_CMD_STRU_VER_R1;
    pstUpFrame->stru_len      = sizeof(MRS_PLC_TF_CMD_S);
    pstUpFrame->dir           = MRS_PLC_UP_FLG;
    pstUpFrame->from          = MRS_PLC_FROM_FOLLOW;
    pstUpFrame->phase         = PHASE_ALL;
    pstUpFrame->seq           = usSeq;
    pstUpFrame->data_type     = MRS_PLC_TF_DATA_TYPE_PERIOD;
    pstUpFrame->option        = MRS_PLC_TF_OPTION_RESULT_IND;
    (HI_VOID)memcpy_s(pstUpFrame->mac_addr, HI_PLC_MAC_ADDR_LEN, pucMeterAddr, HI_PLC_MAC_ADDR_LEN);

    pstResultInd = (MRS_CMD_TF_RESULT_IND_S *)pstUpFrame->data;
    pstResultInd->tei = usTei;

    if (HI_TRUE == pstTfResult->bIsTfComplete && HI_TRUE == pstTfResult->bIsMainNodeAddr) {
        pstResultInd->complete_flag = HI_TRUE;
        pstResultInd->result = (HI_TRUE == pstTfResult->bIsBelongJoinNet ? MRS_TF_RESULT_TYPE_CUR_NET : MRS_TF_RESULT_TYPE_OTHER_NET);
        memcpy_s(pstResultInd->belong_addr, HI_PLC_MAC_ADDR_LEN, pstTfResult->aucMainNodeAddr, HI_PLC_MAC_ADDR_LEN);
    } else {
        pstResultInd->complete_flag = HI_FALSE;
    }

    memset_s(&stPlcFrame, sizeof(stPlcFrame), 0, sizeof(stPlcFrame));
    stPlcFrame.usId = ID_MRS_CMD_TF_PLC;
    stPlcFrame.pucPayload = pucPayload;
    stPlcFrame.usPayloadLen = usPayloadLen;

    ret = MRS_SendPlcFrame(&stPlcFrame);
    HI_DIAG_LOG_BUF(MRS_FILE_LOG_BUF_1004, HI_DIAG_MT("[STA-TF]: send result ind cmd"), pucPayload, usPayloadLen);
    HI_DIAG_LOG_MSG_E1(MRS_FILE_LOG_FLAG_003, HI_DIAG_MT("[STA-TF]: send cmd ret"), ret);
    mrsToolsFree(pucPayload);

    return HI_ERR_SUCCESS;
}

#endif // STA
