//*****************************************************************************
//
//                  ��Ȩ���� (C), 1998-2011, ��Ϊ�������޹�˾
//
//*****************************************************************************
//  �� �� ��   : mrs_srv_cco_broadcast.c
//  �� �� ��   : V1.0 
//  ��    ��   : cuiate/00233580
//  ��������   : 2013-02-20
//  ��������   : �㲥ģ�麯��ʵ��
//               
//  �����б�   :
//  �޸���ʷ   : 
//  1.��    �� : 2013-02-20
//    ��    �� : cuiate/00233580
//    �޸����� : �����ļ� 
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
#include "mrs_fw_proto645.h"
#include "mrs_fw_proto376_2.h"
#include "mrs_fw_proto376_2_echo.h"
#include "mrs_fw_tools.h"
#include "mrs_fw_log.h"
#include "mrs_srv_common.h"
#include "mrs_srv_cco_broadcast.h"
#include "mrs_srv_cco_macmap.h"
#include "mrs_srv_parallel_cco.h"
#include "mrs_srv_list.h"
#include "mrs_srv_cco_event.h"
#include "mrs_srv_cco.h"
#include "mrs_time.h"
#include "hi_mdm.h"


#if defined(PRODUCT_CFG_PRODUCT_TYPE_CCO)
static HI_U32 createBroadcastFramePlc(HI_IN HI_U8 * pDataBuff, HI_IN HI_U16 usDataLen);

static HI_VOID freeBroadcastPlcData(MRS_CCO_BROADCAST_STRU *pstBroadcast);


HI_U32 mrsBroadcastProcess(HI_IN HI_U8 * pDataBuff, HI_IN HI_U16 usDataLen)
{
    HI_U32 ret = HI_ERR_SUCCESS;
    MRS_CCO_BROADCAST_STRU * pstBroadcast = mrsGetBroadcastCtx();

    pstBroadcast->stParam.ucState = (HI_U8)MRS_CCO_BROADCAST_BUSY;

    do
    {
        // ����PLC����ת����
        ret = createBroadcastFramePlc(pDataBuff, usDataLen);
        if(ret != HI_ERR_SUCCESS)
        {
            break;
        }

        ret = mrsSendBroadcastFramePlc();
    } while(0);

    if (HI_ERR_SUCCESS != ret)
    {
        freeBroadcastPlcData(pstBroadcast);
    }
 
    return ret;
}

HI_VOID mrsBroadcastReset(HI_VOID)
{
    MRS_CCO_BROADCAST_STRU * pstBroadcast = mrsGetBroadcastCtx();

    MRS_StopTimer(EN_MRS_SRV_CCO_BROADCAST_TIMER); 

    freeBroadcastPlcData(pstBroadcast);
    pstBroadcast->stParam.ucRetry= 0;
    pstBroadcast->stParam.ucState = (HI_U8)MRS_CCO_BROADCAST_IDLE;
}

HI_U32 mrsSendBroadcastFramePlc(HI_VOID)
{
    HI_U32 ret = HI_ERR_SUCCESS;
    MRS_CCO_BROADCAST_STRU * pstBroadcast = mrsGetBroadcastCtx();

    if (!pstBroadcast->pstPlcDataDl)
    {
        HI_DIAG_LOG_MSG_E0(MRS_FILE_LOG_FLAG_004, HI_DIAG_MT("BC-Ctx NULL"));
        return HI_ERR_BAD_DATA;
    }

    ret = MRS_SendPlcFrame(pstBroadcast->pstPlcDataDl);

    pstBroadcast->stParam.ucRetry++;

    if (pstBroadcast->stParam.ucRetry < pstBroadcast->stParam.ucRetryMax)
    {
        ret = MRS_StartTimer(EN_MRS_SRV_CCO_BROADCAST_TIMER, 1000 * pstBroadcast->stParam.ucInterval, HI_SYS_TIMER_ONESHOT);
        return ret;
    }

    mrsBroadcastReset();

    return ret;
}

HI_U32 createBroadcastFramePlc(HI_IN HI_U8 * pDataBuff, HI_IN HI_U16 usDataLen)
{
    MRS_PLC_FRAME_DATA_S * pstPlcFrameData = HI_NULL;
    MRS_BROADCAST_STRU * pstData = HI_NULL;
    MRS_CCO_SRV_CTX_STRU * pstCcoCtx = mrsCcoGetContext();
    HI_U8 * payload = HI_NULL;
    HI_U16 payloadlen = 0;

    pstPlcFrameData = (MRS_PLC_FRAME_DATA_S *)mrsToolsMalloc(sizeof(MRS_PLC_FRAME_DATA_S));
    if (pstPlcFrameData == HI_NULL)
    {
        return HI_ERR_MALLOC_FAILUE;
    }

    (hi_void)memset_s(pstPlcFrameData, sizeof(MRS_PLC_FRAME_DATA_S), 0, sizeof(MRS_PLC_FRAME_DATA_S));

    payloadlen = sizeof(MRS_BROADCAST_STRU) + usDataLen;
    payload = mrsToolsMalloc(payloadlen);
    if (payload == HI_NULL)
    {
        mrsToolsFree(pstPlcFrameData);
        return HI_ERR_MALLOC_FAILUE;
    }

    (hi_void)memset_s(payload, payloadlen, 0, payloadlen);
    pstData = (MRS_BROADCAST_STRU *)payload;

    // ��֯ת�����ݰ�
    (hi_void)memcpy_s(payload + sizeof(MRS_BROADCAST_STRU), usDataLen, pDataBuff, usDataLen); // ��ȡ��ת������������
    pstData->interface_ver = MRS_BROADCAST_DL_VER;
    pstData->stru_len = sizeof(MRS_BROADCAST_STRU);
    pstData->data_len = usDataLen;  // ��ȡ��ת�����ĳ��� 

    // ��֯PLC��
    pstPlcFrameData->usId = ID_MRS_CMD_BROADCAST_TRANSMIT_PLC;   // ��ȡPLC֡����ID
    (hi_void)memset_s(pstPlcFrameData->ucMac, HI_PLC_MAC_ADDR_LEN, 0xff, HI_PLC_MAC_ADDR_LEN); // �㲥Mac��ַ
    pstPlcFrameData->usPayloadLen = payloadlen;  // ��ȡת�����ݰ�����
    pstPlcFrameData->pucPayload = (HI_U8 *)payload; // ��ȡת�����ݰ�ָ��

    // ��������ת��PLC��(�ط�)
    pstCcoCtx->stBroadcast.pstPlcDataDl = pstPlcFrameData; 

    return HI_ERR_SUCCESS;
}

HI_VOID freeBroadcastPlcData(MRS_CCO_BROADCAST_STRU *pstBroadcast)
{
    if (pstBroadcast->pstPlcDataDl != HI_NULL)
    {
        if (pstBroadcast->pstPlcDataDl->pucPayload != HI_NULL)
        {
            mrsToolsFree(pstBroadcast->pstPlcDataDl->pucPayload);
            pstBroadcast->pstPlcDataDl->pucPayload = HI_NULL; 
        }

        mrsToolsFree(pstBroadcast->pstPlcDataDl);
        pstBroadcast->pstPlcDataDl = HI_NULL;
    }
}
#endif

