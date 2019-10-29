#include "mrs_transfomer_identify_cmn.h"
#include "mrs_transfomer_identify_sta.h"
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

#if defined(PRODUCT_CFG_PRODUCT_TYPE_CCO) || defined(PRODUCT_CFG_PRODUCT_TYPE_STA)

HI_U32 mrsTfGetNotifyNtbDataInfo(MRS_PLC_TF_CMD_S *pstTfCmd, HI_U16 usCmdSize, MRS_CMD_TF_DATA_NOTIFY_S *pstData);
HI_U16 mrsTfCountDataSeqTotalNum(MRS_CMD_TF_DATA_SEQ_S *pstDataSeq);

HI_VOID mrsTfHandleMsg(HI_SYS_QUEUE_MSG_S *pstMsg)
{
    if (pstMsg->ulMsgId != EN_MRS_FW_MSG_TF)
    {
        return;
    }

    #if defined(PRODUCT_CFG_PRODUCT_TYPE_CCO)
    mrsTfCcoHandleMsg(pstMsg);
    #endif
    #if defined(PRODUCT_CFG_PRODUCT_TYPE_STA)
    mrsTfStaHandleMsg(pstMsg);
    #endif
}

HI_U32 mrsTfSendMsg(HI_U16 usMsgId, HI_U32 ulParam1, HI_U32 ulParam2, HI_U32 ulParam3)
{
    HI_SYS_QUEUE_MSG_S stMsg = {0};

    stMsg.ulMsgId = EN_MRS_FW_MSG_TF;
    stMsg.ulParam[0] = usMsgId;
    stMsg.ulParam[1] = ulParam1;
    stMsg.ulParam[2] = ulParam2;
    stMsg.ulParam[3] = ulParam3;

    return mrsSendMessage2Queue(&stMsg);
}

HI_VOID mrsTfRcvMacReportCallback(HI_U8 ucOption, HI_U8 *pucData, HI_U16 usSize)
{
    HI_U8 *pucDataTmp = HI_NULL;
    HI_U32 ret = HI_ERR_FAILURE;

    if (pucData && usSize > 0) {
        pucDataTmp = mrsToolsMalloc(usSize);
        if (!pucDataTmp) {
            return;
        }

        memcpy_s(pucDataTmp, usSize, pucData, usSize);
    }

    switch (ucOption) {
        #if defined(PRODUCT_CFG_PRODUCT_TYPE_CCO)
        case TF_CALLBACK_OPTION_NOTIFY_START_COLLECT_NTB:
            ret = mrsTfSendMsg(MRS_TF_MSG_ID_NOTIFY_STA_START_COLLECT, (uintptr_t)pucDataTmp, usSize, 0);
            break;
        case TF_CALLBACK_OPTION_QUERY_NTB_DATA:
            ret = mrsTfSendMsg(MRS_TF_MSG_ID_QUERY_STA_NTB_DATA, (uintptr_t)pucDataTmp, usSize, 0);
            break;
        case TF_CALLBACK_OPTION_NOTIFY_NTB_DATA:
            ret = mrsTfSendMsg(MRS_TF_MSG_ID_NOTIFY_STA_NTB_DATA, (uintptr_t)pucDataTmp, usSize, 0);
            break;
        #endif

        #if defined(PRODUCT_CFG_PRODUCT_TYPE_STA)
        case TF_CALLBACK_OPTION_STA_REPORT_NTB_DATA:
            ret = mrsTfSendMsg(MRS_TF_MSG_ID_STA_REPORT_NTB_DATA, (uintptr_t)pucDataTmp, usSize, 0);
            break;
        #endif

        default:
            break;
    }

    if (pucDataTmp && HI_ERR_SUCCESS != ret) {
        mrsToolsFree(pucDataTmp);
    }
}

HI_BOOL mrsTfIsRcvNeighborBroadcastFrame(HI_PVOID pFrame, HI_U16 usSize)
{
    HI_U8 *aucFrame = (HI_U8 *)pFrame;
    HI_U8 ucPortNum = 0;
    HI_U16 usCmdId = 0;
    MRS_PLC_TF_CMD_S *pstTfCmd;

    if (!pFrame || usSize < MRS_HEAD_LEN_IN_APP_FRAME + sizeof(MRS_PLC_TF_CMD_S))
    {
        return HI_FALSE;
    }

    memcpy_s(&ucPortNum, sizeof(HI_U8), aucFrame + MRS_PORT_NUM_OFFSET_IN_APP_FRAME, MRS_PORT_NUM_LEN_IN_APP_FRAME);
    memcpy_s(&usCmdId, sizeof(HI_U16), aucFrame + MRS_CMD_ID_OFFSET_IN_APP_FRAME, MRS_CMD_ID_LEN_IN_APP_FRAME);

    if (ucPortNum != MRS_CMD_TF_PORT_NUM || usCmdId != ID_MRS_CMD_TF_PLC)
    {
        return HI_FALSE;
    }

    pstTfCmd = (MRS_PLC_TF_CMD_S *)&aucFrame[MRS_HEAD_LEN_IN_APP_FRAME];
    if (pstTfCmd->option != MRS_PLC_TF_OPTION_DATA_NOTIFY || pstTfCmd->data_type != MRS_PLC_TF_DATA_TYPE_PERIOD)
    {
        return HI_FALSE;
    }
    else
    {
        return HI_TRUE;
    }
}

HI_U8 mrsTfConvertToLocalPowerEdge(HI_U8 ucPowerEdgeInCmd)
{
    switch (ucPowerEdgeInCmd) {
        case MRS_CMD_TF_EDGE_DOUBLE:
            return POWER_EDGE_DOUBLE;
        case MRS_CMD_TF_EDGE_RISE:
            return POWER_EDGE_RISE;
        case MRS_CMD_TF_EDGE_FALL:
            return POWER_EDGE_FALL;
        default:
            return POWER_EDGE_UNKNOWN;
    }
}

HI_U8 mrsTfConvertToCmdPowerEdge(HI_U8 ucPowerEdgeInLocal)
{
    switch (ucPowerEdgeInLocal) {
        case POWER_EDGE_DOUBLE:
            return MRS_CMD_TF_EDGE_DOUBLE;
        case POWER_EDGE_RISE:
            return MRS_CMD_TF_EDGE_RISE;
        case POWER_EDGE_FALL:
            return MRS_CMD_TF_EDGE_FALL;
        default:
            return MRS_CMD_TF_EDGE_UNKNOEN;
    }
}

HI_VOID mrsTfCreateNtbDiffSeq(HI_U32 *pulNtbBuf, HI_U16 usDiffNum, HI_U16 *pusDiffBuf)
{
    HI_U16 i = 0;
    HI_S32 NtbDiff = 0;

    for (i = 0; i < usDiffNum; i++) {
        NtbDiff = (HI_S32)mrsTimeSubDurationCompress(pulNtbBuf[i + 1], pulNtbBuf[i], MRS_CMD_TF_NTB_COMPRESS_TIMES);
        NtbDiff -= (MRS_POWER_PERIOD_LEN_NTB / MRS_CMD_TF_NTB_COMPRESS_TIMES);

        pusDiffBuf[i] = (NtbDiff > 0 ? 0 : 0x8000);

        NtbDiff = HI_ABS(NtbDiff);
        pusDiffBuf[i] |= ((hi_u32)NtbDiff & 0x7FFF);
    }
}

// 需要将基准NTB先填入 pulNtbBuf[0]
HI_VOID mrsTfGetNtbFromDiffSeq(HI_U16 *pusDiffBuf, HI_U16 usDiffNum, HI_U32 *pulNtbBuf)
{
    HI_U16 i = 0;
    HI_S32 NtbDiff = 0;

    for (i = 0; i < usDiffNum; i++) {
        NtbDiff = (pusDiffBuf[i] & 0x7FFF);
        if (pusDiffBuf[i] & 0x8000) {
            NtbDiff = -NtbDiff;
        }

        NtbDiff = (NtbDiff + MRS_POWER_PERIOD_LEN_NTB / MRS_CMD_TF_NTB_COMPRESS_TIMES) * MRS_CMD_TF_NTB_COMPRESS_TIMES;
        pulNtbBuf[i + 1] = mrsTimeAddDuration(pulNtbBuf[i], (HI_U32)NtbDiff);
    }
}

HI_U32 mrsTfHandleNotifyNtbDataCmd(HI_U8 ucRcvFreq, HI_U32 ulRcvNid, MRS_PLC_TF_CMD_S *pstTfCmd, HI_U16 usCmdSize)
{
    MRS_CMD_TF_DATA_NOTIFY_S stCmdInfo = {0};
    TF_NOTIFY_NTB_DATA_INFO_S *pstNtbData;
    HI_U32 ulRet = HI_ERR_SUCCESS;
    HI_U16 usNtbDataSize = 0;
    HI_U8  ucBaseNtbPhaseIdx = 0;
    HI_U16 usNtbBufIdx = 0;
    HI_U16 usDiffIdx = 0;
    HI_U8 ucPhaseIdx = 0;

    if (pstTfCmd->data_type != MRS_PLC_TF_DATA_TYPE_PERIOD) {
        return HI_ERR_SUCCESS;
    }

    ulRet = mrsTfGetNotifyNtbDataInfo(pstTfCmd, usCmdSize, &stCmdInfo);
    if (HI_ERR_SUCCESS != ulRet) {
        return ulRet;
    }

    if (!stCmdInfo.data_seq1) {
        return HI_ERR_BAD_DATA;
    }

    usNtbDataSize = (HI_U16)(sizeof(TF_NOTIFY_NTB_DATA_INFO_S) + (stCmdInfo.total_num + PHASE_CNT * POWER_EDGE_CNT) * sizeof(HI_U32));
    pstNtbData = mrsToolsMalloc(usNtbDataSize);
    if (!pstNtbData) {
        return HI_ERR_MALLOC_FAILUE;
    }
    memset_s(pstNtbData, usNtbDataSize, 0, usNtbDataSize);

    memcpy_s(pstNtbData->addr, HI_PLC_MAC_ADDR_LEN, pstTfCmd->mac_addr, HI_PLC_MAC_ADDR_LEN);
    pstNtbData->freq = ucRcvFreq;
    pstNtbData->nid = ulRcvNid;
    pstNtbData->tei = stCmdInfo.tei;
    pstNtbData->collect_seq = stCmdInfo.collect_seq;
    pstNtbData->is_collect_seq_valid = HI_TRUE;
    pstNtbData->collect_edge = mrsTfConvertToLocalPowerEdge(stCmdInfo.collect_edge);

    #if defined(PRODUCT_CFG_PRODUCT_TYPE_CCO)
    if (mrsTfCcoGetCollectSeq() != pstNtbData->collect_seq) {
        pstNtbData->is_collect_seq_valid = HI_FALSE;
    }
    #endif

    usDiffIdx = 0;
    ucBaseNtbPhaseIdx = PHASE_CNT;
    for (ucPhaseIdx = 0; ucPhaseIdx < PHASE_CNT; ucPhaseIdx++) {
        if (0 == stCmdInfo.data_seq1->report_num[ucPhaseIdx]) {
            pstNtbData->phase_ntb_num[ucPhaseIdx] = 0;
            continue;
        }

        ucBaseNtbPhaseIdx = (PHASE_CNT == ucBaseNtbPhaseIdx ? ucPhaseIdx : ucBaseNtbPhaseIdx);
        pstNtbData->ntb_buf[usNtbBufIdx] = stCmdInfo.data_seq1->base_ntb + (ucPhaseIdx - ucBaseNtbPhaseIdx) * MRS_POWER_PERIOD_LEN_NTB / PHASE_CNT;

        mrsTfGetNtbFromDiffSeq(&stCmdInfo.data_seq1->value[usDiffIdx], stCmdInfo.data_seq1->report_num[ucPhaseIdx],
                                  &pstNtbData->ntb_buf[usNtbBufIdx]);

        pstNtbData->phase_ntb_num[ucPhaseIdx] = stCmdInfo.data_seq1->report_num[ucPhaseIdx] + 1;
        usNtbBufIdx += pstNtbData->phase_ntb_num[ucPhaseIdx];
        usDiffIdx += stCmdInfo.data_seq1->report_num[ucPhaseIdx];
    }

    if (POWER_EDGE_DOUBLE == pstNtbData->collect_edge && stCmdInfo.data_seq2) {
        usDiffIdx = 0;
        ucBaseNtbPhaseIdx = PHASE_CNT;
        for (ucPhaseIdx = 0; ucPhaseIdx < PHASE_CNT; ucPhaseIdx++) {
            if (0 == stCmdInfo.data_seq2->report_num[ucPhaseIdx]) {
                pstNtbData->phase_ntb_num[PHASE_CNT + ucPhaseIdx] = 0;
                continue;
            }

            ucBaseNtbPhaseIdx = (PHASE_CNT == ucBaseNtbPhaseIdx ? ucPhaseIdx : ucBaseNtbPhaseIdx);
            pstNtbData->ntb_buf[usNtbBufIdx] = stCmdInfo.data_seq2->base_ntb + (ucPhaseIdx - ucBaseNtbPhaseIdx) * MRS_POWER_PERIOD_LEN_NTB / PHASE_CNT;

            mrsTfGetNtbFromDiffSeq(&stCmdInfo.data_seq2->value[usDiffIdx], stCmdInfo.data_seq2->report_num[ucPhaseIdx],
                                      &pstNtbData->ntb_buf[usNtbBufIdx]);

            pstNtbData->phase_ntb_num[PHASE_CNT + ucPhaseIdx] = stCmdInfo.data_seq2->report_num[ucPhaseIdx] + 1;
            usNtbBufIdx += pstNtbData->phase_ntb_num[ucPhaseIdx];
            usDiffIdx += stCmdInfo.data_seq2->report_num[ucPhaseIdx];
        }
    }

    pstNtbData->total_ntb_num = usNtbBufIdx;

    HI_MDM_notifyHandleRcvNtbData(pstNtbData, usNtbDataSize);

    mrsToolsFree(pstNtbData);

    return HI_ERR_SUCCESS;
}

HI_U32 mrsTfGetNotifyNtbDataInfo(MRS_PLC_TF_CMD_S *pstTfCmd, HI_U16 usCmdSize, MRS_CMD_TF_DATA_NOTIFY_S *pstData)
{
    HI_U16 usOffset = 0;
    HI_U16 usSeq1TotalNum = 0;
    HI_U16 usSeq2TotalNum = 0;

    memset_s(pstData, sizeof(MRS_CMD_TF_DATA_NOTIFY_S), 0, sizeof(MRS_CMD_TF_DATA_NOTIFY_S));

    if (usCmdSize < sizeof(MRS_PLC_TF_CMD_S) + MRS_CMD_TF_DATA_NOTIFY_FIXED_LEN + sizeof(MRS_CMD_TF_DATA_SEQ_S))
    {
        return HI_ERR_BAD_FRAME;
    }

    memcpy_s(pstData, MRS_CMD_TF_DATA_NOTIFY_FIXED_LEN, pstTfCmd->data, MRS_CMD_TF_DATA_NOTIFY_FIXED_LEN);

    usOffset += MRS_CMD_TF_DATA_NOTIFY_FIXED_LEN;

    pstData->data_seq1 = (MRS_CMD_TF_DATA_SEQ_S *)&pstTfCmd->data[usOffset];
    usSeq1TotalNum = mrsTfCountDataSeqTotalNum(pstData->data_seq1);

    usOffset += (HI_U16)(sizeof(MRS_CMD_TF_DATA_SEQ_S) + usSeq1TotalNum * sizeof(HI_U16));

    if (MRS_CMD_TF_EDGE_DOUBLE == pstData->collect_edge
        && pstData->total_num > usSeq1TotalNum
        && usCmdSize >= sizeof(MRS_PLC_TF_CMD_S) + usOffset + sizeof(MRS_CMD_TF_DATA_SEQ_S))
    {
        pstData->data_seq2 = (MRS_CMD_TF_DATA_SEQ_S *)&pstTfCmd->data[usOffset];
        usSeq2TotalNum = mrsTfCountDataSeqTotalNum(pstData->data_seq2);

        usOffset += (HI_U16)(sizeof(MRS_CMD_TF_DATA_SEQ_S) + usSeq2TotalNum * sizeof(HI_U16));
    }

    if (usSeq1TotalNum + usSeq2TotalNum == pstData->total_num && sizeof(MRS_PLC_TF_CMD_S) + usOffset == usCmdSize)
    {
        return HI_ERR_SUCCESS;
    }
    else
    {
        return HI_ERR_BAD_FRAME;
    }
}

HI_U16 mrsTfCountDataSeqTotalNum(MRS_CMD_TF_DATA_SEQ_S *pstDataSeq)
{
    HI_U16 usTotalNum = 0;
    HI_U8 i = 0;

    for (i = 0; i < PHASE_CNT; i++)
    {
        usTotalNum += pstDataSeq->report_num[i];
    }

    return usTotalNum;
}

#endif // CCO & STA
