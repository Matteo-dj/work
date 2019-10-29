/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2012-2018. All rights reserved.
 * Description: ̨��ʶ����APP�㹫���߼�����ӿ�����
 * Author: xiangchaobin/00378718
 * Create: 2018-12-29
 * Notes: ���ļ������ӿڽ�����̨��ʶ��ģ���ڲ�����
 */
#ifndef __MRS_TRANSFOMER_IDENTIFY_CMN_H__
#define __MRS_TRANSFOMER_IDENTIFY_CMN_H__

#include "mrs_common.h"

HI_START_HEADER

// ̨��ʶ��ģ����ϢID
enum
{
    MRS_TF_MSG_ID_NOTIFY_STA_START_COLLECT,     // ֪ͨվ�������ɼ�
    MRS_TF_MSG_ID_QUERY_STA_NTB_DATA,           // ֪ͨվ�������ɼ�
    MRS_TF_MSG_ID_NOTIFY_STA_NTB_DATA,          // ��֪STA NTB����
    MRS_TF_MSG_ID_STA_REPORT_NTB_DATA,          // վ���ϱ��ɼ���NTB����
    MRS_TF_MSG_ID_SEND_COLELC_START_CMD,        // ������: ���Ϳ�ʼ�ɼ���������
    MRS_TF_MSG_ID_SEND_DATA_COLELC_CMD,         // ������: ���������ռ�����
    MRS_TF_MSG_ID_SEND_RESULT_REQ_CMD,          // ������: ���ͽ����ѯ����
};

#define MRS_STA_TF_REPORT_3762_FIX_SIZE (3)
#define MRS_STA_TF_REPORT_645_CTRL      (0x9E)
#define MRS_STA_TF_REPORT_645_FLAG_SIZE (1)

#define MRS_TF_NTB_COLLECT_NUM_MAX      210
#define MRS_TF_NTB_COLLECT_NUM_MIN      6
#define MRS_TF_COLLECT_DATA_TRY_MAX     254
#define MRS_TF_COLLECT_PERIOD_DEFAULT   5

#define MRS_TF_VOLTAGE_645_DI           0x0201FF00  // ��ȡ��Ƶ��ѹ��645DI 07
#define MRS_TF_FREQENCY_645_DI          0x02800002  // ��ȡ��ƵƵ�ʵ�645DI 07

#define MRS_TF_VOLTAGE_645_DATA_LEN     6
#define MRS_TF_FREQENCY_645_DATA_LEN    2

#define MRS_TF_MAX_MEIGHBOR_NUM     (32)            // �ھ�����������

#define MRS_POWER_PERIOD_LEN_NTB        500000      // ��׼��Ƶ���ڳ��ȣ���λNTB

HI_U32 mrsTfSendMsg(HI_U16 usMsgId, HI_U32 ulParam1, HI_U32 ulParam2, HI_U32 ulParam3);

HI_U8 mrsTfConvertToLocalPowerEdge(HI_U8 ucPowerEdgeInCmd);
HI_U8 mrsTfConvertToCmdPowerEdge(HI_U8 ucPowerEdgeOfLocal);
HI_VOID mrsTfCreateNtbDiffSeq(HI_U32 *pulNtbBuf, HI_U16 usDiffNum, HI_U16 *pusDiffBuf);
HI_VOID mrsTfGetNtbFromDiffSeq(HI_U16 *pusDiffBuf, HI_U16 usDiffNum, HI_U32 *pulNtbBuf);
HI_U32 mrsTfHandleNotifyNtbDataCmd(HI_U8 ucRcvFreq, HI_U32 ulRcvNid, MRS_PLC_TF_CMD_S *pstTfCmd, HI_U16 usCmdSize);

HI_END_HEADER

#endif
