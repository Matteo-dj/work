//*****************************************************************************
//
//                  ��Ȩ���� (C), 1998-2011, ��Ϊ�������޹�˾
//
//*****************************************************************************
//  �� �� ��   : mrs_srv_anytime_readmeter.c
//  �� �� ��   : V1.0
//  ��    ��   : liuxipeng/KF54842
//  ��������   : 2012-06-04
//  ��������   : �泭����ģ�麯��ʵ��
//
//  �����б�   :
//  �޸���ʷ   :
//  1.��    �� : 2012-06-04
//    ��    �� : liuxipeng/KF54842
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
#include "mrs_fw_proto376_2_frame.h"
#include "mrs_fw_tools.h"
#include "mrs_fw_log.h"
#include "mrs_srv_common.h"
#include "mrs_srv_cco_macmap.h"
#include "mrs_srv_parallel_cco.h"
#include "mrs_srv_list.h"
#include "mrs_srv_cco_event.h"
#include "mrs_srv_cco.h"
#include "mrs_srv_cco_readmeter.h"
#include "mrs_srv_anytime_readmeter.h"
#include "mrs_dfx.h"
#include "mrs_srv_mrslog.h"
#include "mrs_srv_cco_testmode.h"
#include "mrs_srv_cco_detect_rm.h"
#include "mrs_srv_readmeter_manage.h"
#include "mrs_fw_proto188.h"
#include "mrs_srv_rf.h"
#include "mrs_srv_dau.h"

#if defined(PRODUCT_CFG_PRODUCT_TYPE_CCO)

HI_VOID FreePlcData( MRS_CCO_SRV_CTX_STRU * pstCcoCtx );
HI_VOID mrsGetDestAddr(HI_OUT HI_U8 * pucDestAddr,
                       HI_IN MRS_3762_ADDR_REALM_STRU * pstAddr,
                       HI_IN HI_U8 ucProtocol, HI_IN HI_U8 * pDataBuff, HI_U16 usDataLen);
HI_U32 mrsCreateReadMeterFramePlc(HI_IN HI_U8 * mac_addr, HI_IN HI_U8 protocol,
                                                  HI_IN HI_U8 * data_buff, HI_IN HI_U16 data_len);
HI_U32 mrsSendReadMeterFramePlc(HI_VOID);
HI_U32 mrsAFN13F1ReportAck(HI_U8 aucMacAdd[HI_PLC_MAC_ADDR_LEN]);

HI_VOID mrsSetXRStatus(MRS_ANYTIME_RM_STATE_ENUM state)
{
    MRS_CCO_SRV_CTX_STRU *cco_ctx = mrsCcoGetContext();

    cco_ctx->stAnytimeRMModel.enState = state;
    mrsDfxXrStatus(state);
}


// �ͷŵ����ݵ�PLC����
HI_VOID FreePlcData( MRS_CCO_SRV_CTX_STRU * pstCcoCtx )
{
    if(pstCcoCtx->stAnytimeRMModel.pstPlcDataDl != HI_NULL)
    {
        if(pstCcoCtx->stAnytimeRMModel.pstPlcDataDl->pucPayload != HI_NULL)
        {
            mrsToolsFree(pstCcoCtx->stAnytimeRMModel.pstPlcDataDl->pucPayload);   // �ͷ�ת�����ݿ�
            pstCcoCtx->stAnytimeRMModel.pstPlcDataDl->pucPayload = HI_NULL;
        }

        mrsToolsFree(pstCcoCtx->stAnytimeRMModel.pstPlcDataDl);   // �ͷ�PLC֡����
        pstCcoCtx->stAnytimeRMModel.pstPlcDataDl = HI_NULL;
    }
}

//*****************************************************************************
// ��������: mrsAnyTimeReadMeterInit
// ��������: �泭�����ʼ��
//
// ����˵��:
//   ��
//
// �� �� ֵ:
//   ��
//
// ����Ҫ��:
// ���þ���:
// ��    ��: liuxipeng/KF54842 [2012-06-09]
//*****************************************************************************
HI_VOID mrsAnyTimeReadMeterInit(HI_VOID)
{
    MRS_CCO_SRV_CTX_STRU * pstCcoCtx = mrsCcoGetContext();
    PLC_OVERTIME_INF_STRU* TimeOutInf = mrsGetPlcOverTimeInfAfn13();
#if defined(MRS_SRV_PROTOCOL_CJT188)
	PLC_OVERTIME_INF_STRU* TimeOutInfSaved = mrsGetPlcOverTimeInfSavedAfn13();
#endif 

    MRS_StopTimer(EN_MRS_SRV_CCO_STA_TIMER);
    MRS_StopTimer(EN_MRS_SRV_CCO_SERVICE_END_TIMER);

    TimeOutInf->Try = 0;   // ��STAͨѶ��ʱ��������
    TimeOutInf->MaxTry = TimeOutInf->MaxTryNvS;
    TimeOutInf->Loop = 0;
#if defined(MRS_SRV_PROTOCOL_CJT188)
	HI_DIAG_LOG_MSG_E1(MRS_FILE_LOG_FLAG_010, HI_DIAG_MT("MaxTryLoop value [%d]"),TimeOutInfSaved->MaxTryLoop);

	//��ԭʱ�䳭��ʱ��Ϊ�������ʱ�䡣
	TimeOutInf->LayTimeOutNv = TimeOutInfSaved->LayTimeOutNv;
	TimeOutInf->MeterTimeOutNv = TimeOutInfSaved->MeterTimeOutNv;// 
	TimeOutInf->RevisoryNv = TimeOutInfSaved->RevisoryNv;
	TimeOutInf->CtrlCmdDelayNv = TimeOutInfSaved->CtrlCmdDelayNv;//
	TimeOutInf->CltIDelayNv = TimeOutInfSaved->CltIDelayNv;
	TimeOutInf->MeterFrameInterval = TimeOutInfSaved->MeterFrameInterval;
	TimeOutInf->MaxTryLoop = TimeOutInfSaved->MaxTryLoop;
	TimeOutInf->MaxTryNvB = TimeOutInfSaved->MaxTryNvB;
	TimeOutInf->MaxTryNvS = TimeOutInfSaved->MaxTryNvS;
#endif

    mrsSetXRStatus(EN_MRS_ANYTIME_RM_IDLE);

    FreePlcData(pstCcoCtx);

    pstCcoCtx->stAnytimeRMModel.bIsBC = HI_FALSE;   // ��Ϊ�ǹ㲥ʽ����

    return;
}


// ��ȡĿ���ַ-����ʹ��645���ĵ�ַ
HI_VOID mrsGetDestAddr(HI_OUT HI_U8 * pucDestAddr,
                       HI_IN MRS_3762_ADDR_REALM_STRU * pstAddr,
                       HI_IN HI_U8 ucProtocol, HI_IN HI_U8 * pDataBuff, HI_U16 usDataLen)
{
    HI_U16 pos = 0;
    HI_U16 frame_len = 0;

    (hi_void)memcpy_s(pucDestAddr, HI_METER_ADDR_LEN, pstAddr->ucDestAdd, HI_METER_ADDR_LEN);

    if (mrsCheck645Proto(ucProtocol)
        && (HI_ERR_SUCCESS == mrsFind645Frame(pDataBuff, (HI_S16)usDataLen, &pos, &frame_len)))
    {
        (hi_void)memcpy_s(pucDestAddr, HI_METER_ADDR_LEN, pDataBuff + pos + MRS_645_FRAME_METERADD_OFFSET, HI_METER_ADDR_LEN);
    }
}


//*****************************************************************************
// ��������: mrsCreateReadMeterFramePlc
// ��������: ������������PLCת��֡
//
// ����˵��:
//   pucMacAddr Ŀ��Mac��ַ
//   ucProtocol ��Լ����
//   pDataBuff ��ת��������֡
//   usDataLen ��ת�������ݳ���
//
// �� �� ֵ:
//   ��
//
// ����Ҫ��:
// ���þ���:
// ��    ��: liuxipeng/KF54842 [2012-06-09]
//*****************************************************************************
HI_U32 mrsCreateReadMeterFramePlc(HI_IN HI_U8 * mac_addr, HI_IN HI_U8 protocol,
                                                  HI_IN HI_U8 * data_buff, HI_IN HI_U16 data_len)
{
    MRS_PLC_FRAME_DATA_STRU * plc_frame_data = HI_NULL;
    MRS_DATA_TRANSMIT_AFN13_DL_STRU * transmit_data = HI_NULL;
    MRS_CCO_SRV_CTX_STRU * cco_ctx = mrsCcoGetContext();
    HI_U8 * payload = HI_NULL;
    HI_U16 payload_len = 0;
    HI_U32 meter_timeout = 0;

    plc_frame_data = (MRS_PLC_FRAME_DATA_STRU *)mrsToolsMalloc(sizeof(MRS_PLC_FRAME_DATA_STRU));
    if(plc_frame_data != HI_NULL)
    {
        (hi_void)memset_s(plc_frame_data, sizeof(MRS_PLC_FRAME_DATA_STRU), 0, sizeof(MRS_PLC_FRAME_DATA_STRU));
    }
    else
    {
        return HI_ERR_NO_MORE_MEMORY;
    }

    payload_len = sizeof(MRS_DATA_TRANSMIT_AFN13_DL_STRU) + data_len;
	 if(mrsCheckWaterMeterProto(protocol))
    {
    	payload_len+=1;
    }
    payload = mrsToolsMalloc(payload_len);

    if(payload != HI_NULL)
    {
        transmit_data = (MRS_DATA_TRANSMIT_AFN13_DL_STRU *)payload;
        (hi_void)memset_s(transmit_data, payload_len, 0, payload_len);
    }
    else
    {
        mrsToolsFree(plc_frame_data);
        return HI_ERR_NO_MORE_MEMORY;
    }

    // ֡�������
    cco_ctx->stAnytimeRMModel.seq++;

    // �ѿ������
    mrsCtrlCmdProc(data_buff,data_len);

    // ��ʼ����ʱ�����Դ���
    mrsGetMaxTryAfn13(mac_addr);
    mrsGetPlcTimeOutAfn13(mac_addr, HI_FALSE, &meter_timeout);

    // ��֯ת�����ݰ�
    transmit_data->interface_ver = MRS_AFN13_DL_VER;
    transmit_data->stru_size     = sizeof(MRS_DATA_TRANSMIT_AFN13_DL_STRU);
    transmit_data->protocol      = protocol;        // ��ȡ��Լ����
    if(mrsCheckWaterMeterProto(protocol))
    {
    	transmit_data->data_len      = data_len+1;        // ��ȡ��ת�����ĳ���   
    	transmit_data->protocol      = WIRELESS_TRANSMISSION_PROTOCOL; 
    }
	else
	{
    	transmit_data->data_len      = data_len;        // ��ȡ��ת�����ĳ���   
	}
    transmit_data->seq           = cco_ctx->stAnytimeRMModel.seq;
    transmit_data->time_out      = (HI_U8)MRS_MS_TO_100MS(meter_timeout); // ��λΪ�ٺ���
    transmit_data->dir           = MRS_PLC_DL_FLG;  // ������֡��־
    if(mrsCheckWaterMeterProto(protocol))
    {
    	transmit_data->data[0]=protocol;
		mrsToolsMemcpy_s(transmit_data->data+1, transmit_data->data_len, data_buff, data_len); // ��ȡ��ת������������
    }
	else
	{
    	(hi_void)memcpy_s(transmit_data->data, transmit_data->data_len, data_buff, data_len); // ��ȡ��ת������������
	}

    // ��֯PLC��
    plc_frame_data->usId         = ID_MRS_CMD_DATA_TRANSMIT_AFN13_PLC;    // ��ȡPLC֡����ID
    (hi_void)memcpy_s(plc_frame_data->ucMac, sizeof(plc_frame_data->ucMac), mac_addr, HI_PLC_MAC_ADDR_LEN);  // ��ȡMac��ַ
    plc_frame_data->usPayloadLen = payload_len;       // ��ȡת�����ݰ�����
    plc_frame_data->pucPayload   = (HI_U8 *)payload;  // ��ȡת�����ݰ�ָ��

    // ��������ת��PLC��(�ط�)
    cco_ctx->stAnytimeRMModel.pstPlcDataDl = plc_frame_data;

#if defined(PRODUCT_CFG_SUPPORT_SDM_MRS_LOG)
/* BEGIN: liuxipeng/KF54842 2014/2/20   ���ⵥ��: DTS2014022003115 */
    //mrsLogFrameInfStatTx(data_buff, data_len, (HI_U8)cco_ctx->stAnytimeRMModel.bIsBC);//optimized by weichao
/* END  : liuxipeng/KF54842 2014/2/20   ���ⵥ��: DTS2014022003115 */
#endif

    return HI_ERR_SUCCESS;
}

//*****************************************************************************
// ��������: mrsCreateReadMeterFrame376_2
// ��������: ����������������֡(376.2 AFN-13 F1)
//
// ����˵��:
//   ucProtocol ��Լ����
//   pDataBuff �����ص���������
//   usDataLen �����ص����ݳ���
//   ppOutBuff ��֯��ɵ�376.2֡
//   usOutLen  376.2֡����
//
// �� �� ֵ:
//   ��
//
// ����Ҫ��:
// ���þ���:
// ��    ��: liuxipeng/KF54842 [2012-06-09]
// ��    ��: cuiate/c00233580  [2013-04-01]
//*****************************************************************************
HI_U32 mrsCreateReadMeterFrame376_2(HI_IN HI_U8 ucProtocol, HI_IN HI_U8 * pDataBuff, HI_IN HI_U16 usDataLen,
                                                  HI_OUT HI_PBYTE * ppOutBuff, HI_OUT HI_U16 * usOutLen, HI_U8 ucMacAdd[HI_PLC_MAC_ADDR_LEN])
{
    MRS_CCO_SRV_CTX_STRU * pstCcoCtx = mrsCcoGetContext();
    HI_U32 ret = HI_ERR_SUCCESS;

    do
    {
        PROTO376_2_FRAME_PRE_STRU stFrame;
        HI_U8 * data_buf = HI_NULL;
        HI_U16 data_len = 0;
        HI_U16 offset = 0;
        HI_U16 comm_delay_len = 0;

        data_len = usDataLen + 2;   // ��Լ(1) + ����(1)
        if ((pstCcoCtx->ucProtoVer != PROTO_VER_3762_2009)
            && ((pstCcoCtx->stAnytimeRMModel.from == XR_FROM_AFN13_F1) || (pstCcoCtx->stAnytimeRMModel.from == XR_FROM_NDM_AFN13_F1)))
        {
            // ��ǰ���ı���ͨ������ʱ��
            comm_delay_len = 2;
        }

        data_len += comm_delay_len;
        data_buf = mrsToolsMalloc(data_len);
        if (!data_buf)
        {
            ret = HI_ERR_MALLOC_FAILUE;
            break;
        }

        (hi_void)memset_s(data_buf, data_len, 0, data_len);
        (hi_void)memset_s(&stFrame, sizeof(stFrame), 0, sizeof(stFrame));
        if (comm_delay_len > 0)
        {
            data_buf[offset++] = (MRS_AFN_COMM_DELAY % 255);
            data_buf[offset++] = (MRS_AFN_COMM_DELAY / 255);
        }

        data_buf[offset++] = ucProtocol;
        data_buf[offset++] = (HI_U8)((usDataLen > 255) ? 255 : usDataLen);
        if (usDataLen > 0)
        {
            (hi_void)memcpy_s(data_buf + offset, data_len - offset, pDataBuff, usDataLen);
            stFrame.ucPhase = mrsCcoGetStaPhase(pstCcoCtx->aucSrcMac);
            pstCcoCtx->ucPhase = stFrame.ucPhase;
        }

        stFrame.ucModuleFlag = 1;
        stFrame.ucIs645 = 0;
        stFrame.ucSeq = (HI_U8)pstCcoCtx->stAnytimeRMModel.seq_1376_2;
        stFrame.ucRelayLevel = mrsMapGetLevel(pstCcoCtx->stAnytimeRMModel.aucMeterAddr, ucMacAdd);

        (hi_void)memcpy_s(stFrame.ucSrcAdd, HI_METER_ADDR_LEN, pstCcoCtx->stAnytimeRMModel.aucMeterAddr, HI_METER_ADDR_LEN);
        if(XR_FROM_NDM_AFN13_F1 == pstCcoCtx->stAnytimeRMModel.from)
		{
            (hi_void)memcpy_s(stFrame.ucDstAdd, HI_METER_ADDR_LEN, pstCcoCtx->stCcoNdmPlcInfo.src_addr, HI_METER_ADDR_LEN);
		}
        else
        {
            (hi_void)memcpy_s(stFrame.ucDstAdd, HI_METER_ADDR_LEN, pstCcoCtx->ucMainNodeAddr, HI_METER_ADDR_LEN);
        	mrsHexInvert(stFrame.ucDstAdd, sizeof(stFrame.ucDstAdd));
        }

        if(XR_FROM_AFN02_F1 == pstCcoCtx->stAnytimeRMModel.from)
        {
            stFrame.stAfnData.ucAFN = MRS_AFN(0x02);
						if(pstCcoCtx->stAnytimeRMModel.ucFn)
						{
							stFrame.stAfnData.ucDT[0] = MRS_AFN_FN_LO(pstCcoCtx->stAnytimeRMModel.ucFn);
			        stFrame.stAfnData.ucDT[1] = MRS_AFN_FN_HI(pstCcoCtx->stAnytimeRMModel.ucFn);
							pstCcoCtx->stAnytimeRMModel.ucFn=0;
						}
						else
						{
							stFrame.stAfnData.ucDT[0] = MRS_AFN_FN_LO(1);
			        stFrame.stAfnData.ucDT[1] = MRS_AFN_FN_HI(1);
						}
        }
        else
        {
            stFrame.stAfnData.ucAFN = MRS_AFN(0x13);
        }

        stFrame.stAfnData.ucDT[0] = MRS_AFN_FN_LO(1);
        stFrame.stAfnData.ucDT[1] = MRS_AFN_FN_HI(1);
        stFrame.stAfnData.pData = data_buf;
        stFrame.stAfnData.usDataLen = data_len;

        ret = mrsCreate3762Frame(&stFrame, ppOutBuff, usOutLen, stFrame.ucSrcAdd);

        mrsToolsFree(data_buf);
    } while(0);

    return ret;
}

//*****************************************************************************
// ��������: mrsSendReadMeterFramePlc
// ��������: ����PLC����֡
//
// ����˵��:
//   ��
//
// �� �� ֵ:
//   HI_ERR_SUCCESS ���ͳɹ�
//
// ����Ҫ��:
// ���þ���:
// ��    ��: liuxipeng/KF54842 [2012-06-09]
// ��    ��: cuiate/c00233580  [2013-04-01]
//*****************************************************************************
HI_U32 mrsSendReadMeterFramePlc(HI_VOID)
{
    HI_U32 ret = HI_ERR_SUCCESS;
    MRS_CCO_SRV_CTX_STRU * pstCcoCtx = mrsCcoGetContext();
    HI_U8 mac[HI_PLC_MAC_ADDR_LEN] = {0};

    if(pstCcoCtx->stAnytimeRMModel.pstPlcDataDl == HI_NULL)
    {
        return HI_ERR_BAD_DATA;
    }

    if (pstCcoCtx->stAnytimeRMModel.bIsBC)
    {
        (hi_void)memcpy_s(mac, sizeof(mac), pstCcoCtx->stAnytimeRMModel.pstPlcDataDl->ucMac, HI_PLC_MAC_ADDR_LEN);
        (hi_void)memset_s(pstCcoCtx->stAnytimeRMModel.pstPlcDataDl->ucMac, HI_PLC_MAC_ADDR_LEN, 0xff, HI_PLC_MAC_ADDR_LEN);
    }

    ret = MRS_SendPlcFrame(pstCcoCtx->stAnytimeRMModel.pstPlcDataDl);
    if(ret == HI_ERR_SUCCESS)
    {
        mrsDfxXrPlcTx();
        if(pstCcoCtx->stAnytimeRMModel.bIsBC == HI_TRUE)
        {
            mrsDfxBcRmCnt();
        }
    }

/* BEGIN: Modified by fengxiaomin/00209182, 2014/3/27   ���ⵥ��:DTS2014032906596 */
    {
        MRS_DATA_TRANSMIT_AFN13_DL_STRU *pstAfn13 = (MRS_DATA_TRANSMIT_AFN13_DL_STRU *)
            (pstCcoCtx->stAnytimeRMModel.pstPlcDataDl->pucPayload);
        mrsDfxRmPlcTx((HI_U8*)pstAfn13 + sizeof(MRS_DATA_TRANSMIT_AFN13_DL_STRU), pstAfn13->data_len, ret);
    }
/* END:   Modified by fengxiaomin/00209182, 2014/3/27 */

    if (pstCcoCtx->stAnytimeRMModel.bIsBC)
    {
        (hi_void)memcpy_s(pstCcoCtx->stAnytimeRMModel.pstPlcDataDl->ucMac, HI_PLC_MAC_ADDR_LEN, mac, HI_PLC_MAC_ADDR_LEN);
    }

    return ret;
}

HI_U32 mrsAFN13F1CreateAckFrame(HI_U8 **ppBuf, HI_U16 *pLen, HI_U8 aucMacAdd[HI_PLC_MAC_ADDR_LEN])
{
    HI_U8 DataBuff[12] = {0};

#if defined(PRODUCT_CFG_PRODUCT_TYPE_CCO_BROAD_BAND_UNIT)
    if (mrsToolIsBbu())
    {
        //���ظ�����buffer
        mrsClear645ProtoBufferBbu(ID_MRS_BBUDA_645BUF);

        if (!mrsIsSimuChlOpened())
        {
            return HI_ERR_SKIP;
        }
    }
#endif

    return mrsCreateReadMeterFrame376_2(0, DataBuff, 0, ppBuf, pLen, aucMacAdd);
}

// ��֯�ϱ�AFN13-F1Ӧ��֡(����������)
HI_U32 mrsAFN13F1ReportAck(HI_U8 aucMacAdd[HI_PLC_MAC_ADDR_LEN])
{
    HI_U32 ret = HI_ERR_SUCCESS;
    HI_U8 * pOutBuff = HI_NULL;
    HI_U16 usOutLen = 0;
	MRS_CCO_SRV_CTX_STRU * cco_ctx = mrsCcoGetContext();

    ret = mrsAFN13F1CreateAckFrame(&pOutBuff, &usOutLen, aucMacAdd);
    if ((ret == HI_ERR_SUCCESS) && (pOutBuff != HI_NULL))
    {
    	if (XR_FROM_NDM_AFN13_F1 == cco_ctx->stAnytimeRMModel.from)
		{
			mrsMsgSendPlcToNdm(pOutBuff, usOutLen); // ����"��Ӧ"֡
		} else {
	        MRS_SendMrData(pOutBuff, usOutLen, HI_DMS_CHL_UART_PORT_APP);   // ���Ͷ���ķ��ر��ĵ�������
			/* BEGIN: Modified by fengxiaomin/00209182, 2014/3/27   ���ⵥ��:DTS2014032906596 */
	        mrsDfxRmUartTx(pOutBuff, usOutLen, HI_FALSE);
			/* END:   Modified by fengxiaomin/00209182, 2014/3/27 */
		}
    }
    mrsToolsFree(pOutBuff);   // �ͷŶ�������֡

    return ret;
}

//*****************************************************************************
// ��������: mrsAnyTimeReadMeterPlcTimeOutProcess
// ��������: �泭����Plc֡��ʱ����
//
// ����˵��:
//   pstCcoCtx ȫ��������
//
// �� �� ֵ:
//   ��
//
// ����Ҫ��:
// ���þ���:
// ��    ��: liuxipeng/KF54842 [2012-06-09]
// ��    ��: cuiate/c00233580  [2013-04-01]
// ��    ��: cuiate/c00233580  [2013-07-22]
//*****************************************************************************
HI_VOID mrsAnyTimeReadMeterPlcTimeOutProcess(MRS_CCO_SRV_CTX_STRU * pstCcoCtx)
{
    HI_U32 ret = HI_ERR_SUCCESS;
    PLC_OVERTIME_INF_STRU* TimeOutInf = mrsGetPlcOverTimeInfAfn13();
    MR_ANYTIME_READMETER_STRU *anytime_readmeter = &pstCcoCtx->stAnytimeRMModel;
    MRS_PLC_FRAME_DATA_STRU *plc_data_dl = anytime_readmeter->pstPlcDataDl;

    mrsDfxXrPlcTimeout();

    if (plc_data_dl == HI_NULL)
    {
        return;
    }

    mrsDfxRmPlcDlRx(ID_MRS_CMD_DATA_TRANSMIT_AFN13_PLC, plc_data_dl->pucPayload, HI_TRUE);

    while (TimeOutInf->Loop < TimeOutInf->MaxTryLoop)
    {
        if (anytime_readmeter->bIsBC == HI_FALSE)   // ������ǹ㲥����
        {
            if (TimeOutInf->Try >= TimeOutInf->MaxTry)  // ����㲥ģʽ
            {
                TimeOutInf->Try = 0;

                if (0 == TimeOutInf->MaxTryNvB)     // �㲥���Դ���Ϊ0���ص�����ģʽ
                {
                    if (0 == TimeOutInf->MaxTryNvS)
                    {
                        TimeOutInf->Loop = 0;
                        break;
                    }

                    TimeOutInf->Loop++;
                    continue;
                }

                anytime_readmeter->bIsBC = HI_TRUE;
                mrsGetMaxTryAfn13(HI_NULL);
                mrsGetPlcTimeOutAfn13(plc_data_dl->ucMac, HI_TRUE, HI_NULL);

            }
        }
        else
        {
            mrsDfxBcRmFailCnt();    // ͳ�ƹ㲥֡����ʧ�ܴ���
        }

        if (TimeOutInf->Try < TimeOutInf->MaxTry)
        {
            TimeOutInf->Try++;

            mrsDfxXrPlcTryCnt();    // ͳ���ط���PLC֡

            ret = mrsSendReadMeterFramePlc();
            if (HI_ERR_SUCCESS == ret)
            {
                mrsSetXRStatus(EN_MRS_ANYTIME_RM_WAIT_PLC);
                MRS_StartTimer(EN_MRS_SRV_CCO_STA_TIMER, (HI_U32)TimeOutInf->OverTime, HI_SYS_TIMER_ONESHOT);
                return;
            }

            TimeOutInf->Try = 0;
            TimeOutInf->Loop = 0;
            break;
        }

        if (anytime_readmeter->bIsBC != HI_FALSE)   // �㲥����
        {
            TimeOutInf->Try = 0;
            TimeOutInf->Loop++;

            if ((TimeOutInf->MaxTryNvS > 0) && (!mrsToolsBroadCastMac(plc_data_dl->ucMac)))  // ���ǹ㲥��ַʱ�����л��������������л�
            {
                anytime_readmeter->bIsBC = HI_FALSE;
                mrsGetMaxTryAfn13(plc_data_dl->ucMac);
                mrsGetPlcTimeOutAfn13(plc_data_dl->ucMac, HI_FALSE, HI_NULL);
            }
        }
    }

    // �泭����ʧ�ܴ���ͳ��
    mrsDfxXrFailCnt();
#if 0//optimized by weichao
#if defined(PRODUCT_CFG_SUPPORT_SDM_MRS_LOG)
    if(plc_data_dl->pucPayload != HI_NULL)
    {
        MRS_DATA_TRANSMIT_AFN13_DL_STRU * plcDlData = HI_NULL;

        plcDlData = (MRS_DATA_TRANSMIT_AFN13_DL_STRU *)plc_data_dl->pucPayload;
        mrsLogPlcOverTimeStat(plcDlData->data, plcDlData->data_len, 0, HI_NULL);
    }
#endif
#endif
    if (!mrsIsDetectRm())
    {
        // ��֯�ϱ�AFN13-F1Ӧ��֡(����������)
        if (plc_data_dl ->is_bc_frame == HI_FALSE)
        {
            mrsAFN13F1ReportAck(plc_data_dl->ucMac);
        }
        else
        {
            mrsAFN13F1ReportAck(HI_NULL);
        }

        mrsDecreaseMrTTL(&(pstCcoCtx->stMapModule), anytime_readmeter->aucMeterAddr);
    }

    mrsCcoDetectRmTimeoutProc(anytime_readmeter->aucMeterAddr);
    mrsCheckDetectEndProc();

    mrsAnyTimeReadMeterFinishProc();

	//������plc�㳭���æ���ػ��崦��
	mrsCcoNdmPlcBusyProc();
	
    return ;
}

//*****************************************************************************
// ��������: mrsAnyTimeReadMeterReceivePlcProcess
// ��������: �泭����Plc֡���մ���
//
// ����˵��:
//   pstData PLC����ת��֡�ṹ
//
// �� �� ֵ:
//   ��
//
// ����Ҫ��:
// ���þ���:
// ��    ��: liuxipeng/KF54842 [2012-06-09]
//*****************************************************************************
HI_U32 mrsAnyTimeReadMeterReceivePlcProcess(MRS_DATA_TRANSMIT_AFN13_UP_STRU * transmit_data, HI_U8 ucMacAdd[HI_PLC_MAC_ADDR_LEN])
{
    MRS_CCO_SRV_CTX_STRU * cco_ctx = mrsCcoGetContext();
    HI_U8 protocol = 0;
    HI_U8 * data_buff = HI_NULL;
    HI_U16 data_len = 0;
    HI_U8 * out_buff = HI_NULL;
    HI_U16 out_len = 0;
    HI_U32 ret = HI_ERR_SUCCESS;

    if(transmit_data == HI_NULL)
    {
        return HI_ERR_INVALID_PARAMETER;
    }

    if(cco_ctx->stAnytimeRMModel.enState != EN_MRS_ANYTIME_RM_WAIT_PLC)
    {
        HI_DIAG_LOG_MSG_E1(MRS_FILE_LOG_FLAG_008, HI_DIAG_MT("XR status err [%d]"),cco_ctx->stAnytimeRMModel.enState);
        mrsDfxXrPlcExceedCnt();
        mrsDfxRmPlcUpRx(ID_MRS_CMD_DATA_TRANSMIT_AFN13_PLC, (HI_U8*)transmit_data, HI_TRUE);
        return HI_ERR_FAILURE;
    }

    if (transmit_data->seq != cco_ctx->stAnytimeRMModel.seq)
    {
        HI_DIAG_LOG_MSG_E2(MRS_FILE_LOG_FLAG_009, HI_DIAG_MT("XR seq err [%d][%d]"),
                        transmit_data->seq,cco_ctx->stAnytimeRMModel.seq);
        mrsDfxXrPlcSeqErr();    // ͳ�ƶ�����PLC֡��(���ڵ�)
        mrsDfxRmPlcUpRx(ID_MRS_CMD_DATA_TRANSMIT_AFN13_PLC, (HI_U8*)transmit_data, HI_TRUE);
        return HI_ERR_FAILURE;
    }
    if(transmit_data->protocol==WIRELESS_TRANSMISSION_PROTOCOL)
	{
		protocol = (HI_U8)transmit_data->data[0];   // ��ȡ��Լ����
    	data_buff = (HI_U8*)transmit_data + transmit_data->stru_size+1;   // ��ȡת������
    	data_len = transmit_data->data_len-1;   // ��ȡת�����ݳ���
	}
	else
	{
    	protocol = (HI_U8)transmit_data->protocol;   // ��ȡ��Լ����
    	data_buff = (HI_U8*)transmit_data + transmit_data->stru_size;   // ��ȡת������
    	data_len = transmit_data->data_len;   // ��ȡת�����ݳ���
	}
    if (mrsCcoIsTestMode())
    {
        HI_U16 usPos = 0;
        HI_U16 usFrameLen = 0;

        if ((HI_ERR_SUCCESS != mrsFind645Frame(data_buff, (HI_S16)data_len, &usPos, &usFrameLen))
            || (!mrsMeterAddrMatch(cco_ctx->stAnytimeRMModel.aucMeterAddr, data_buff + usPos + MRS_645_FRAME_METERADD_OFFSET)))
        {
            HI_DIAG_LOG_MSG_E0(MRS_FILE_LOG_FLAG_001, HI_DIAG_MT("[XR]����ģʽ����ַ��ƥ��"));
            return HI_ERR_FAILURE;
        }
    }
#if 0 ////optimized by weichao
#if defined(PRODUCT_CFG_SUPPORT_SDM_MRS_LOG)
    if(cco_ctx->stAnytimeRMModel.pstPlcDataDl != HI_NULL)
    {
        MRS_DATA_TRANSMIT_AFN13_DL_STRU * plcDlData = HI_NULL;

        plcDlData = (MRS_DATA_TRANSMIT_AFN13_DL_STRU *)cco_ctx->stAnytimeRMModel.pstPlcDataDl->pucPayload;
        if(plcDlData != HI_NULL && plcDlData->data_len + sizeof(MRS_DATA_TRANSMIT_AFN13_DL_STRU) == cco_ctx->stAnytimeRMModel.pstPlcDataDl->usPayloadLen)
        {
            mrsLogFrameInfStatRx(data_buff, data_len, plcDlData->data, plcDlData->data_len, 1);
        }
    }
#endif
#endif
    MRS_StopTimer(EN_MRS_SRV_CCO_STA_TIMER);    // ֹͣPlc֡��ʱ��ʱ��
    FreePlcData(cco_ctx);     // �ͷű��ݵ�Plc����֡����


    mrsDfxXrOkCnt();

    if (mrsIsDetectRm())
    {
        HI_DIAG_LOG_MSG_E0(MRS_FILE_LOG_FLAG_060, HI_DIAG_MT("detect rm rx resp"));

        mrsAnyTimeReadMeterFinishProc();
        //��̽�Ⳮ���б����г����ɹ����̽���־���
        mrsDetectRmRespProc(mrsCcoGetDetectRmCtx()->stRunning.stNode.aucNodeAddr);

        return HI_ERR_SUCCESS;
    }
    else
    {
        //��̽�Ⳮ���б����г����ɹ����̽���־���
        mrsDetectRmRespProc(cco_ctx->stAnytimeRMModel.aucMeterAddr);
    }

#if defined(PRODUCT_CFG_PRODUCT_TYPE_CCO_BROAD_BAND_UNIT)
    if (mrsToolIsBbu())
    {
        // ��buffer
        mrsClear645ProtoBufferBbu(ID_MRS_BBUDA_645BUF);
    }

    // ���NV��֧�ֽ��հ汾����������Ӧ����Ϊҵ�񴮿�
    if (mrsToolIsBbu()
        && (!mrsIsSimuChlOpened()))
    {
        // ���հ汾����������Ӧ && ҵ�񴮿�
        out_buff = data_buff;
        out_len = data_len;

        ret = HI_ERR_SUCCESS;
    }
    else
#endif
    {
    	MRS_RF_DAU_FRAME_STRU *pDauFrame=HI_NULL;
    	pDauFrame = mrsRFDauDecode(data_buff, data_len);
		if(pDauFrame!=HI_NULL)
		{
			HI_DIAG_LOG_MSG_E1(MRS_FILE_LOG_FLAG_094, HI_DIAG_MT("[RF]CCO 0x41 ucDataCmdId"),pDauFrame->ucDataCmdId);
			if(pDauFrame->ucDataCmdId == 0x41)
			{
				// ģ��ͨ�� || �ǽ��հ汾����������Ӧ
				ret = mrsCreateReadMeterFrame376_2(protocol, pDauFrame->ucapDataData+1, pDauFrame->ucapDataData[0], &out_buff, &out_len, ucMacAdd);
			}
		}
		else
		{
        // ģ��ͨ�� || �ǽ��հ汾����������Ӧ
        ret = mrsCreateReadMeterFrame376_2(protocol, data_buff, data_len, &out_buff, &out_len, ucMacAdd);

		}
	}

    if((ret == HI_ERR_SUCCESS) && (out_buff != HI_NULL))
    {
    	if (XR_FROM_NDM_AFN13_F1 == cco_ctx->stAnytimeRMModel.from)
    	{
    		mrsMsgSendPlcToNdm(out_buff, out_len); // ����"��Ӧ"֡
        } else {
	        ret = MRS_SendMrData(out_buff, out_len, HI_DMS_CHL_UART_PORT_APP); // ���Ͷ���ķ��ر��ĵ�������
	        if (ret == HI_ERR_SUCCESS)
	        {
	            mrsDfxXrUartTx();
	            mrsDfxRmUartTx(out_buff, out_len, HI_FALSE);
	        }
        }
    }

    mrsToolsFree(out_buff);   // �ͷŶ�������֡

    mrsAnyTimeReadMeterFinishProc();

	//������plc�㳭���æ���ػ��崦��
	mrsCcoNdmPlcBusyProc();

    return HI_ERR_SUCCESS;
}

//*****************************************************************************
// ��������: mrsAnyTimeReadMeterPlcChlStatusProcess
// ��������: �泭����ͨ���ı���Ϣ����
//
// ����˵��:
//   pstCcoCtx ȫ��������
//
// �� �� ֵ:
//   ��
//
// ����Ҫ��:
// ���þ���:
// ��    ��: liuxipeng/KF54842 [2012-06-09]
//*****************************************************************************
HI_U32 mrsAnyTimeReadMeterPlcChlStatusProcess(MRS_CCO_SRV_CTX_STRU * pstCcoCtx)
{
    HI_U32 ret = HI_ERR_SUCCESS;
    PLC_OVERTIME_INF_STRU* TimeOutInf = mrsGetPlcOverTimeInfAfn13();

    if(pstCcoCtx == HI_NULL)
    {
        return HI_ERR_FAILURE;
    }

    if(pstCcoCtx->stAnytimeRMModel.enState != EN_MRS_ANYTIME_RM_WAIT_CHLAVAIL)
    {
        return HI_ERR_FAILURE;
    }

    if(pstCcoCtx->stAnytimeRMModel.pstPlcDataDl == HI_NULL)
    {
        return HI_ERR_FAILURE;
    }

    TimeOutInf->Try = 0;
    TimeOutInf->Loop = 0;

    // ���÷���Plc����
    ret = mrsSendReadMeterFramePlc();
    if(ret == HI_ERR_SUCCESS)
    {
        mrsSetXRStatus(EN_MRS_ANYTIME_RM_WAIT_PLC);
        MRS_StartTimer(EN_MRS_SRV_CCO_STA_TIMER,
                            (HI_U32)TimeOutInf->OverTime, HI_SYS_TIMER_ONESHOT); // ������ʱ��
    }

    return ret;
}

//*****************************************************************************
// ��������: mrsAnyTimeReadMeterProcess
// ��������: �泭���������
//
// ����˵��:
//   pstAddr ��ַ��
//   ucProtocol ��Լ����
//   pDataBuff ������������
//   usDataLen ���������ݵĳ���
//
// �� �� ֵ:
//   ��
//
// ����Ҫ��:
// ���þ���:
// ��    ��: liuxipeng/KF54842 [2012-06-09]
//*****************************************************************************
HI_U32 mrsAnyTimeReadMeterProcess(HI_IN MRS_3762_USER_DATA_HRD_STRU * pstUsrDataHrd, HI_IN HI_U8 ucProtocol,
                                                    HI_IN HI_U8 * pDataBuff, HI_IN HI_U16 usDataLen)
{
    HI_U32 ret = HI_ERR_SUCCESS;
    HI_U8 ucDestAddr[HI_PLC_MAC_ADDR_LEN+1] = {0};    // Ŀ����ַ
    HI_U8 nodeMac[HI_PLC_MAC_ADDR_LEN] = {0};
	HI_U16 pos = 0;
    HI_U16 frame_len = 0;
	HI_U8 bCJT188Pro = HI_FALSE;
	
    MRS_CCO_SRV_CTX_STRU * pstCcoCtx = mrsCcoGetContext();
    PLC_OVERTIME_INF_STRU * TimeOutInf = mrsGetPlcOverTimeInfAfn13();
    MRS_3762_ADDR_REALM_STRU *pstAddr = &pstUsrDataHrd->stAddrRealm;

    HI_DIAG_LOG_BUF(MRS_FILE_LOG_BUF_1000, HI_DIAG_MT("͸��-Ŀ���ַ"), pstAddr->ucDestAdd, HI_METER_ADDR_LEN);
    // ��ȡĿ�����ַ
    mrsGetDestAddr(pstCcoCtx->stAnytimeRMModel.aucMeterAddr, pstAddr, ucProtocol, pDataBuff, usDataLen);
    mrsMeterAddrPreProc(ucDestAddr, pstCcoCtx->stAnytimeRMModel.aucMeterAddr, ucProtocol);
    HI_DIAG_LOG_BUF(MRS_FILE_LOG_BUF_1001, HI_DIAG_MT("͸��-����ַ"), pstCcoCtx->stAnytimeRMModel.aucMeterAddr, HI_METER_ADDR_LEN);
#if defined(MRS_SRV_PROTOCOL_CJT188)
	if((mrsCheckTransProto(ucProtocol) && (mrsFind188Frame(pDataBuff, (HI_S16)usDataLen,&pos, &frame_len) == HI_ERR_SUCCESS))
		||mrsCheckWaterMeterProto(ucProtocol) )
	{
		bCJT188Pro = HI_TRUE;
	}
	if(bCJT188Pro == HI_TRUE)
	{
		pstCcoCtx->stAnytimeRMModel.stOverTimeInf.LayTimeOutNv = 0;
		pstCcoCtx->stAnytimeRMModel.stOverTimeInf.MeterTimeOutNv = (3 + pstCcoCtx->stAnytimeRMModel.ucAdditionTime188) * 1000;// 13s
		pstCcoCtx->stAnytimeRMModel.stOverTimeInf.RevisoryNv = 200;
		pstCcoCtx->stAnytimeRMModel.stOverTimeInf.CtrlCmdDelayNv = 0;//0s
		pstCcoCtx->stAnytimeRMModel.stOverTimeInf.CltIDelayNv = 500;
		pstCcoCtx->stAnytimeRMModel.stOverTimeInf.MeterFrameInterval = 50;
		pstCcoCtx->stAnytimeRMModel.stOverTimeInf.MaxTryLoop = 1;
    	pstCcoCtx->stAnytimeRMModel.stOverTimeInf.MaxTryNvS =  pstCcoCtx->stAnytimeRMModel.ucPlcMaxReTryS188;
   	 	pstCcoCtx->stAnytimeRMModel.stOverTimeInf.MaxTryNvB = pstCcoCtx->stAnytimeRMModel.ucPlcMaxReTryB188;
	}
#endif
    // ����ӳ���ϵ
    do{
	    if(bCJT188Pro)
	    {
	    	ucProtocol=METER_PROTO_BOTHWAY_WATERMETER;
	    	mrsToolsMemcpy_s(ucDestAddr,HI_PLC_MAC_ADDR_LEN+1,pDataBuff+pos+2,7);
			HI_DIAG_LOG_BUF(MRS_FILE_LOG_BUF_1001, HI_DIAG_MT("͸��-����ַ"), ucDestAddr, HI_METER_ADDR_LEN);
	    	ret =mrsRfGetWaterMeterMAC(ucDestAddr, nodeMac);
			if(ret == HI_ERR_SUCCESS)
			{
				pstCcoCtx->stAnytimeRMModel.bIsBC = HI_FALSE;
				break;
			}
			else if(ret == HI_ERR_CCO_DATA)
			{
				HI_U8	*pTempData = (HI_U8 *)mrsToolsMalloc(usDataLen+4);
				if(pTempData!=HI_NULL)
				{
					pTempData[0]=0;
					pTempData[1]=0;
					pTempData[2]=usDataLen;
					mrsToolsMemcpy_s(pTempData+3,usDataLen,pDataBuff,usDataLen);
					mrsCcoRFDau0x4XEncodeFrame(pDataBuff, usDataLen+3,ucDestAddr,60,0x40);
					mrsToolsFree(pTempData);
					return HI_ERR_SUCCESS;
				}
			}
			/*
			else
			{
				mrsToolsMemset_s(nodeMac, HI_PLC_MAC_ADDR_LEN, 0xFF, HI_PLC_MAC_ADDR_LEN);   // û���ҵ���Ӧ��MAC��ַ(ʹ�ù㲥MAC)
	        	pstCcoCtx->stAnytimeRMModel.bIsBC = HI_TRUE;   // ��Ϊ�㲥ʽ����
			}
			*/
	    }

	    ret = mrsMapGetMAC(ucDestAddr, nodeMac);
	    if(ret == HI_ERR_SUCCESS)
	    {
	        pstCcoCtx->stAnytimeRMModel.bIsBC = HI_FALSE;   // ��Ϊ�ǹ㲥ʽ����
	    }
		else
	    {
	        mrsToolsMemset_s(nodeMac, HI_PLC_MAC_ADDR_LEN, 0xFF, HI_PLC_MAC_ADDR_LEN);   // û���ҵ���Ӧ��MAC��ַ(ʹ�ù㲥MAC)
	        pstCcoCtx->stAnytimeRMModel.bIsBC = HI_TRUE;   // ��Ϊ�㲥ʽ����
	    }
    }while(0);
    if ((pstCcoCtx->ucDutBoardFlag)
        || (pstCcoCtx->stAnytimeRMModel.stOverTimeInf.MaxTryNvB == -1))
    {
        (hi_void)memset_s(nodeMac, HI_PLC_MAC_ADDR_LEN, 0xFF, HI_PLC_MAC_ADDR_LEN);   // û���ҵ���Ӧ��MAC��ַ(ʹ�ù㲥MAC)
        pstCcoCtx->stAnytimeRMModel.bIsBC = HI_TRUE;   // ��Ϊ�㲥ʽ����
    }

    do
    {
        // ����PLC����ת����
        if(bCJT188Pro)
    	{
		    HI_U8 ucChannelInfo=0;
			HI_U8 *pDauData=HI_NULL;
			HI_U16 ucDauDataLen=0;
			HI_U16 i=0;
			MR_WATER_METER_SEARCH_STRU *pWaterMeter=&(pstCcoCtx->stWaterMeterSearchModel);
			for(i=0;i<pWaterMeter->usWaterMeterResultNum;i++)
			{
				if(0==mrsToolsMemcmp(pDataBuff+pos+2,pWaterMeter->stWaterMeterResult[i].ucaWaterMac,7))
				{
					ucChannelInfo = pWaterMeter->stWaterMeterResult[i].ucRFChannelInfoNum*2;
					break;
				}
			}
			HI_U8	*pData = (HI_U8 *)mrsToolsMalloc(usDataLen+3);
			if(pData!=HI_NULL)
			{
				pData[0]=0;
				pData[1]=0;
				
				pData[2]=usDataLen;
				mrsToolsMemcpy_s(pData+3,usDataLen,pDataBuff,usDataLen);
				HI_DIAG_LOG_MSG_E1(MRS_FILE_LOG_FLAG_092, HI_DIAG_MT("[RF]CCO ucChannelInfo"),ucChannelInfo);
				
				HI_DIAG_LOG_BUF(MRS_FILE_LOG_BUF_1011, HI_DIAG_MT("[RF]SRC nodeMac"), nodeMac, HI_METER_ADDR_LEN);
				ret=mrsCCORFDauEncodeFrame(&pDauData,&ucDauDataLen,pData,usDataLen+3,nodeMac,pDataBuff+pos+2,0x40,ucChannelInfo);
				if(ret ==HI_ERR_SUCCESS)
				{
					ret = mrsCreateReadMeterFramePlc(nodeMac, ucProtocol, pDauData, ucDauDataLen);
				}
				else
				{
					ret = mrsCreateReadMeterFramePlc(nodeMac, ucProtocol, pDataBuff, usDataLen);
				}
				mrsToolsFree(pData);
				mrsToolsFree(pDauData);
			}
			else
			{
				ret = mrsCreateReadMeterFramePlc(nodeMac, ucProtocol, pDataBuff, usDataLen);
				HI_DIAG_LOG_MSG_E0(MRS_FILE_LOG_FLAG_093, HI_DIAG_MT("[RF]CCO mrsCCoRFDauEncodeFrame err"));
			}
			
    	}
		else
		{
        	ret = mrsCreateReadMeterFramePlc(nodeMac, ucProtocol, pDataBuff, usDataLen);
		}
        if(ret != HI_ERR_SUCCESS)
        {
            break;
        }

        // ����Plc���ݰ�
        ret = mrsSendReadMeterFramePlc();
        if(ret == HI_ERR_SUCCESS)
        {
            mrsSetXRStatus(EN_MRS_ANYTIME_RM_WAIT_PLC);
            MRS_StartTimer(EN_MRS_SRV_CCO_STA_TIMER,
                                (HI_U32)TimeOutInf->OverTime, HI_SYS_TIMER_ONESHOT); // ������ʱ��
        }

    } while(0);

    if(ret == HI_ERR_SUCCESS)
    {
        HI_U32 status = mrsCcoGetState();

        status &= ~MRS_CCO_STATUS_NORMAL;
        status |= MRS_CCO_STATUS_ANYTIME_READMETER;
        mrsCcoSetState(status);

        // �������������ʱ��(��ʱ���Ὣ�泭״̬��ΪԤ�ָ�״̬)
        MRS_StartTimer(EN_MRS_SRV_CCO_SERVICE_END_TIMER, (pstCcoCtx->stAnytimeRMModel.stOverTimeInf.MaxTimeout - MRS_RM_MAX_SERVICE_TIME_SUB) * 1000, HI_SYS_TIMER_ONESHOT);
    }
    else
    {
        mrsAnyTimeReadMeterFinishProc();
        HI_DIAG_LOG_MSG_E0(MRS_FILE_LOG_FLAG_052, HI_DIAG_MT("any read fail"));
    }

    return ret;
}


/* BEGIN: PN: DTS2015040900760 MODIFY\ADD\DEL by cuiate/00233580 at 2015/5/12 */
HI_U32 mrsAnyTimeReadMeterProcessDelay(HI_IN HI_U8 * pDataBuff, HI_IN HI_U16 usDataLen)
{
    HI_U32 ret = HI_ERR_SUCCESS;
    HI_U8 nodeMac[HI_PLC_MAC_ADDR_LEN] = {0};
    MRS_CCO_SRV_CTX_STRU * pstCcoCtx = mrsCcoGetContext();
    PLC_OVERTIME_INF_STRU * TimeOutInf = mrsGetPlcOverTimeInfAfn13();
    HI_U8 aucAddr[HI_METER_ADDR_LEN];

    // ��ȡĿ�����ַ
    HI_U16 pos = 0;
    HI_U16 len = usDataLen;

    (hi_void)memset_s(nodeMac, HI_PLC_MAC_ADDR_LEN, 0xFF, HI_PLC_MAC_ADDR_LEN);
    (hi_void)memset_s(pstCcoCtx->stAnytimeRMModel.aucMeterAddr, HI_METER_ADDR_LEN, 0, HI_METER_ADDR_LEN);
    pstCcoCtx->stAnytimeRMModel.bIsBC = HI_TRUE;

    ret = mrsFind645Frame(pDataBuff, (HI_S16)usDataLen, &pos, &len);
    if (HI_ERR_SUCCESS == ret)
    {
        HI_U8 ucProtocol = mrsToolsGet645Ver(pDataBuff[pos + MRS_645_FRAME_CTRL_OFFSET] & MRS_645_FRAME_CONTROL_MASK);

        (hi_void)memcpy_s(pstCcoCtx->stAnytimeRMModel.aucMeterAddr, HI_METER_ADDR_LEN,
            pDataBuff + pos + MRS_645_FRAME_METERADD_OFFSET, HI_METER_ADDR_LEN);
        mrsMeterAddrPreProc(aucAddr, pDataBuff + pos + MRS_645_FRAME_METERADD_OFFSET, ucProtocol);
    }
    else
    {
        (hi_void)memcpy_s(aucAddr, HI_METER_ADDR_LEN, pstCcoCtx->stAnytimeRMModel.aucMeterAddr, HI_METER_ADDR_LEN);
    }

    // ����ӳ���ϵ
    ret = mrsMapGetMAC(aucAddr, nodeMac);
    if (HI_ERR_SUCCESS == ret)
    {
        pstCcoCtx->stAnytimeRMModel.bIsBC = HI_FALSE;   // ��Ϊ�ǹ㲥ʽ����
    }

    do
    {
        // ����PLC����ת����
        ret = mrsCreateReadMeterFramePlc(nodeMac, pstCcoCtx->stAnytimeRMModel.ucProtocol, pDataBuff, usDataLen);
        if(HI_ERR_SUCCESS != ret)
        {
            break;
        }

        // ����Plc���ݰ�
        ret = mrsSendReadMeterFramePlc();
        if(HI_ERR_SUCCESS == ret)
        {
            mrsSetXRStatus(EN_MRS_ANYTIME_RM_WAIT_PLC);
            MRS_StartTimer(EN_MRS_SRV_CCO_STA_TIMER,
                                (HI_U32)TimeOutInf->OverTime, HI_SYS_TIMER_ONESHOT); // ������ʱ��
        }

    }while(0);

    if(ret != HI_ERR_SUCCESS)    // ���ִ���
    {
        FreePlcData(pstCcoCtx);  // ��ձ��ݵ�������

        mrsAnyTimeReadMeterFailEndProc(pstCcoCtx);
    }

    return ret;
}
/* END:   PN: DTS2015040900760 MODIFY\ADD\DEL by cuiate/00233580 at 2015/5/12 */

//*****************************************************************************
// ��������: mrsAnyTimeReadMeterFinishProc
// ��������: �泭�����������
//
// ����˵��:
//   ��
//
// �� �� ֵ:
//   ��
//
// ����Ҫ��:
// ���þ���:
// ��    ��: fengxiaomin/00209182 [2016-03-18]
//*****************************************************************************
HI_VOID mrsAnyTimeReadMeterFinishProc(HI_VOID)
{
    HI_U32 status = mrsCcoGetState();

    status &= ~MRS_CCO_STATUS_ANYTIME_READMETER;
    mrsCcoSetState(status);

    mrsAnyTimeReadMeterInit();  // �泭ģ���ʼ��(��ʼ��״̬-�ͷ��ڴ�-ֹͣ��Ӧ��ʱ��)
    mrsReadMeterResume();

    HI_MDM_SetTfNtbSendProcessCtrl(HI_MDM_NTB_SEND_PROCESS_RESUME, HI_MDM_NTB_SEND_CTRL_REASON_READ_METER);

    if (mrsIsDetectRm())
    {
        mrsSetDetectRmFlag(HI_FALSE);
        mrsResumeDetectRmSendMsg();
    }
    else
    {
        //������͸��������ָ�̽�Ⳮ��,����3�붨ʱ��
        MRS_StartTimer(MRS_CCO_TIMER_RESUME_DETECT_RM, MRS_CCO_TIME_RESUME_DETECT_RM_DEFAULT, HI_SYS_TIMER_ONESHOT);
    }

}

//*****************************************************************************
// ��������: mrsAnyTimeReadMeterServiceTimeout
// ��������: �泭�����ʱ�䳬ʱ����
//
// ����˵��:
//   pstCcoCtx ȫ��������
//
// �� �� ֵ:
//   ��
//
// ����Ҫ��:
// ���þ���:
// ��    ��: fengxiaomin/00209182 [2016-03-18]
//*****************************************************************************
HI_VOID mrsAnyTimeReadMeterServiceTimeout(MRS_CCO_SRV_CTX_STRU * pstCcoCtx)
{
    if (pstCcoCtx->status & MRS_CCO_STATUS_ANYTIME_READMETER)
    {
        mrsDecreaseMrTTL(&pstCcoCtx->stMapModule, pstCcoCtx->stAnytimeRMModel.aucMeterAddr);
        mrsAnyTimeReadMeterFailEndProc(pstCcoCtx);
    }
}

//*****************************************************************************
// ��������: mrsAnyTimeReadMeterFailEndProc
// ��������: �泭����ʧ�ܽ�������
//
// ����˵��:
//   pstCcoCtx ȫ��������
//
// �� �� ֵ:
//   ��
//
// ����Ҫ��:
// ���þ���:
// ��    ��: fengxiaomin/00209182 [2016-03-18]
//*****************************************************************************
HI_VOID mrsAnyTimeReadMeterFailEndProc(MRS_CCO_SRV_CTX_STRU * pstCcoCtx)
{
    MRS_PLC_FRAME_DATA_STRU * pstPlcFrame = pstCcoCtx->stAnytimeRMModel.pstPlcDataDl;
#if 0//optimized by weichao
#if defined(PRODUCT_CFG_SUPPORT_SDM_MRS_LOG)
    MRS_DATA_TRANSMIT_AFN13_DL_STRU * plcDlData = HI_NULL;

    if(pstPlcFrame != HI_NULL)
    {
        plcDlData = (MRS_DATA_TRANSMIT_AFN13_DL_STRU *)pstPlcFrame->pucPayload;
        if(plcDlData != HI_NULL)
        {
            mrsLogPlcOverTimeStat(plcDlData->data, plcDlData->data_len, 0, HI_NULL);
        }
    }
#endif
#endif
    if(EN_MRS_ANYTIME_RM_WAIT_REVISE_RESP != pstCcoCtx->stAnytimeRMModel.enState)
    {
        if ((pstPlcFrame != HI_NULL) && (pstPlcFrame->is_bc_frame == HI_FALSE))
        {
            mrsAFN13F1ReportAck(pstPlcFrame->ucMac);
        }
        else
        {
            mrsAFN13F1ReportAck(HI_NULL);
        }
    }

    mrsReadMeterStateInit();    // ��ʱ����ģ���ʼ��

    mrsAnyTimeReadMeterFinishProc();

    return;
}

#endif
