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
    pstBroadcast->pfnBroadcastFinishNotify = HI_NULL;
    pstBroadcast->pUserParam = HI_NULL;
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

    if (pstBroadcast->pfnBroadcastFinishNotify)
    {
        pstBroadcast->pfnBroadcastFinishNotify(pstBroadcast->pUserParam);
    }

    mrsBroadcastReset();

    return ret;
}

#if defined(PRODUCT_CFG_PRODUCT_TYPE_CSG_GD)
HI_U32 createBroadcastFramePlc(HI_IN HI_U8 * pDataBuff, HI_IN HI_U16 usDataLen)
{
    MRS_PLC_FRAME_DATA_S *pstPlcFrame = HI_NULL;
    MRS_CCO_SRV_CTX_STRU * pstCcoCtx = mrsCcoGetContext();
    MRS_CSG_PLC_FRAME_HEAD * pstFrameHead = HI_NULL;
    MRS_CSG_TRANS_XR_DL_STRU *pstTransData = HI_NULL;
    HI_U8 *pucPayload = HI_NULL;
    HI_U16 usPayloadLen = 0;

    pstPlcFrame = (MRS_PLC_FRAME_DATA_S *)mrsToolsMalloc(sizeof(MRS_PLC_FRAME_DATA_S));
    if (!pstPlcFrame)
    {
        return HI_ERR_NO_MORE_MEMORY;
    }

    usPayloadLen = (HI_U16)(sizeof(MRS_CSG_PLC_FRAME_HEAD) + sizeof(MRS_CSG_TRANS_XR_DL_STRU) + usDataLen);
    pucPayload = (HI_U8 *)mrsToolsMalloc(usPayloadLen);
    if (!pucPayload)
    {
        mrsToolsFree(pstPlcFrame);
        return HI_ERR_NO_MORE_MEMORY;
    }

    (hi_void)memset_s(pucPayload, usPayloadLen, 0, usPayloadLen);

    pstCcoCtx->stAnytimeRMModel.seq  = pstCcoCtx->usPlcSeq++;
   
    pstFrameHead = (MRS_CSG_PLC_FRAME_HEAD *)pucPayload;
    pstFrameHead->usFrameType = MRS_CSG_PLC_CTRL_TYPE_TRANS;
    pstFrameHead->ucSrvId = MRS_CSG_PLC_SRV_ID_TRANS_XR;
    pstFrameHead->ucVersion = MRS_CSG_PLC_PROTO_VERSION;
    pstFrameHead->usDir = MRS_CSG_PLC_CTRL_DIR_DL;
    pstFrameHead->usExtendBit = MRS_CSG_PLC_CTRL_EXTEND_BIT_N;
    pstFrameHead->usPrm = MRS_CSG_PLC_CTRL_PRM_HOST;
    pstFrameHead->usRespBit = MRS_CSG_PLC_CTRL_RESP_BIT_N;
    pstFrameHead->usSeq = pstCcoCtx->stAnytimeRMModel.seq;
    pstFrameHead->usDataLength = sizeof(MRS_CSG_TRANS_XR_DL_STRU) + usDataLen;

    pstTransData = (MRS_CSG_TRANS_XR_DL_STRU *)pstFrameHead->aucData;
    (hi_void)memcpy_s(pstTransData->aucSrcAddr, HI_METER_ADDR_LEN, pstCcoCtx->ucMainNodeAddr, HI_METER_ADDR_LEN);
    (hi_void)memset_s(pstTransData->aucDstAddr, HI_METER_ADDR_LEN, 0x99, HI_METER_ADDR_LEN);
    pstTransData->usDataLen = usDataLen;
    (hi_void)memcpy_s(pstTransData->aucData, usDataLen, pDataBuff, usDataLen);

    (hi_void)memset_s(pstPlcFrame, sizeof(MRS_PLC_FRAME_DATA_S), 0, sizeof(MRS_PLC_FRAME_DATA_S));
    pstPlcFrame->usId = ID_MRS_CMD_CSG_DATA_TRANSMIT_PLC;
    (hi_void)memset_s(pstPlcFrame->ucMac, HI_PLC_MAC_ADDR_LEN, 0xff, HI_PLC_MAC_ADDR_LEN);
    pstPlcFrame->usPayloadLen = usPayloadLen;
    pstPlcFrame->pucPayload = pucPayload;

    // ��������ת��PLC��(�ط�)
    pstCcoCtx->stBroadcast.pstPlcDataDl = pstPlcFrame; 

    return HI_ERR_SUCCESS;
}

#else

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

    (hi_void)memcpy_s(payload + sizeof(MRS_BROADCAST_STRU), usDataLen, pDataBuff, usDataLen);
    pstData->interface_ver = MRS_BROADCAST_DL_VER;
    pstData->stru_len = sizeof(MRS_BROADCAST_STRU);
    pstData->data_len = usDataLen;
    pstData->seq = pstCcoCtx->stBroadcast.ucSeq++;

    pstPlcFrameData->usId = ID_MRS_CMD_BROADCAST_TRANSMIT_PLC;
    (hi_void)memset_s(pstPlcFrameData->ucMac, HI_PLC_MAC_ADDR_LEN, 0xff, HI_PLC_MAC_ADDR_LEN); // �㲥Mac��ַ
    pstPlcFrameData->usPayloadLen = payloadlen;
    pstPlcFrameData->pucPayload = (HI_U8 *)payload;

    pstCcoCtx->stBroadcast.pstPlcDataDl = pstPlcFrameData; 

    return HI_ERR_SUCCESS;
}
#endif


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


HI_VOID mrsBroadcastRegFinishNotify(HI_VOID (*pfnBroadcastFinishNotify)(HI_VOID*), HI_VOID *pParam)
{
    MRS_CCO_BROADCAST_STRU * pstBroadcast = mrsGetBroadcastCtx();

    pstBroadcast->pfnBroadcastFinishNotify = pfnBroadcastFinishNotify;
    pstBroadcast->pUserParam = pParam;
}

#endif

