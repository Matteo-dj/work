//*****************************************************************************
// 
//                  ��Ȩ���� (C), 1998-2011, ��Ϊ�������޹�˾
// 
//*****************************************************************************
//  �� �� ��   : mrs_srv_collector_irda.c
//  �� �� ��   : V2.0 
//  ��    ��   : fengxiaomin/f00209182
//  ��������   : 2012-08-28
//  ��������   : II�Ͳɼ�����������ʵ��
//               
//  �����б�   : TODO: ...
//  �޸���ʷ   : 
//  1.��    �� : 2012-08-28
//    ��    �� : fengxiaomin/f00209182
//    �޸����� : �����ļ� 
// 
//*****************************************************************************
#include "mrs_common.h"
#include "mrs_cmd_msg.h"
#include "mrs_fw_tools.h"
#include "mrs_fw_nv.h"
#include "mrs_fw_log.h"
#include "mrs_fw_proto645.h"
#include "mrs_fw_proto376_2_echo.h"
#include "mrs_srv_res.h"
#include "mrs_srv_collector_irda.h"
#include "mrs_srv_sta_queue.h"
#include "mrs_srv_parallel_sta.h"
#include "mrs_srv_sta_event.h"
#include "mrs_srv_sta.h"
#include "mrs_srv_sta_searchmeter.h"
#include "mrs_srv_cltii_event.h"
#include "mrs_srv_collector.h"

#if defined(PRODUCT_CFG_PRODUCT_TYPE_STA)

/* BEGIN: PN: DTS2015042907048 MODIFY\ADD\DEL by cuiate/00233580 at 2015/4/29 */
#define MRS_IRDA_CTRL14_PARAM_EXTRA_LEN (8)

#define MRS_IRDA_CTRL14_PARAM_VALID(len, expect_len) \
    (((len) == (expect_len)) || ((len) == ((expect_len) + MRS_IRDA_CTRL14_PARAM_EXTRA_LEN)))
/* END:   PN: DTS2015042907048 MODIFY\ADD\DEL by cuiate/00233580 at 2015/4/29 */


#define MRS_SM_REPORT_MAX_METERNUM      (6)

typedef struct
{
    HI_U32              ulMinDI;
    HI_U32              ulMaxDI;
    MRS_IR_645_DI_FUN   func;
} MRS_IRDA_DI_PAIR_STRU;

typedef struct
{
    HI_U8 aucMeterAddr[HI_METER_ADDR_LEN];
    HI_U8 ucProtocol;
    HI_U8 ucReserved;
} MRS_IRDA_SM_ITEM_STRU;

typedef struct
{
    HI_BOOL bValid;
    HI_U8   ucTotal;
    HI_U8   ucSent;
    HI_U8   ucSeq;
    MRS_IRDA_SM_ITEM_STRU astItem[MRS_MAX_METERNUM];
} MRS_IRDA_SM_REPORT_STRU;


//*****************************************************************************
//ȫ�ֱ���
//*****************************************************************************
HI_PRV MRS_IRDA_SM_REPORT_STRU g_SMReportCtx = {HI_FALSE, 0, 0, 0, {{{0}, 0, 0}}};
//*****************************************************************************

// ��������汾��
HI_U32 mrsIr645_11H_DI04A00101(HI_VOID *param, HI_VOID **ppOutParam, HI_U16 *pOutParamLen, HI_U8 ucPreSize);

// ��ѯ��������汾��չ
HI_U32 mrsIr645_11H_DI04A00102(HI_VOID *param, HI_VOID **ppOutParam, HI_U16 *pOutParamLen, HI_U8 ucPreSize);

// �ն˸�λ����
HI_U32 mrsIr645_11H_DI04A00201(HI_VOID *param, HI_VOID **ppOutParam, HI_U16 *pOutParamLen, HI_U8 ucPreSize);

// ע�Ἧ�����߼���ַ
HI_U32 mrsIr645_11H_DI04A00300(HI_VOID *param, HI_VOID **ppOutParam, HI_U16 *pOutParamLen, HI_U8 ucPreSize);

// II���ѱ���Ϣ
HI_U32 mrsIr645_11H_DI04A00301(HI_VOID *param, HI_VOID **ppOutParam, HI_U16 *pOutParamLen, HI_U8 ucPreSize);

// �ɼ����ʲ���
HI_U32 mrsIr645_11H_DI04A00303(HI_VOID *param, HI_VOID **ppOutParam, HI_U16 *pOutParamLen, HI_U8 ucPreSize);

// ��ѯ�ն�����ʱ��
HI_U32 mrsIr645_11H_DI04A00503(HI_VOID *param, HI_VOID **ppOutParam, HI_U16 *pOutParamLen, HI_U8 ucPreSize);

//���ɼ����ڵ������
HI_U32 mrsIr645_11H_DI04A00601(HI_VOID *param, HI_VOID **ppOutParam, HI_U16 *pOutParamLen, HI_U8 ucPreSize);

//���ɼ����ڵ������
HI_U32 mrsIr645_11H_DI04A00701(HI_VOID *param, HI_VOID **ppOutParam, HI_U16 *pOutParamLen, HI_U8 ucPreSize);

// ��ѯ�ɼ���ͣ�����,�ɼ�������N�Σ�����ʱ�̣�����ʱ��
HI_U32 mrsIr645_11H_DI04A0080X(HI_VOID *param, HI_VOID **ppOutParam, HI_U16 *pOutParamLen, HI_U8 ucPreSize);

// ��ѯ�¼�������
HI_U32 mrsIr645_11H_DI04A09901(HI_VOID *param, HI_VOID **ppOutParam, HI_U16 *pOutParamLen, HI_U8 ucPreSize);

// ��ѯ�¼�����
HI_U32 mrsIr645_11H_DI04A09902(HI_VOID *param, HI_VOID **ppOutParam, HI_U16 *pOutParamLen, HI_U8 ucPreSize);

// MAC��ַ
HI_U32 mrsIr645_11H_DI04A09903(HI_VOID *param, HI_VOID **ppOutParam, HI_U16 *pOutParamLen, HI_U8 ucPreSize);


// ע�Ἧ�����߼���ַ
HI_U32 mrsIr645_14H_DI04A00300(HI_VOID *param, HI_VOID **ppOutParam, HI_U16 *pOutParamLen, HI_U8 ucPreSize);

// �ɼ����ʲ���
HI_U32 mrsIr645_14H_DI04A00303(HI_VOID *param, HI_VOID **ppOutParam, HI_U16 *pOutParamLen, HI_U8 ucPreSize);

// Ӳ����ʼ��
HI_U32 mrsIr645_14H_DI04A00401(HI_VOID *param, HI_VOID **ppOutParam, HI_U16 *pOutParamLen, HI_U8 ucPreSize);

// ���ݳ�ʼ��
HI_U32 mrsIr645_14H_DI04A00402(HI_VOID *param, HI_VOID **ppOutParam, HI_U16 *pOutParamLen, HI_U8 ucPreSize);

// ������ʼ��
HI_U32 mrsIr645_14H_DI04A00403(HI_VOID *param, HI_VOID **ppOutParam, HI_U16 *pOutParamLen, HI_U8 ucPreSize);

//�����ն�����ʱ��
HI_U32 mrsIr645_14H_DI04A00503(HI_VOID *param, HI_VOID **ppOutParam, HI_U16 *pOutParamLen, HI_U8 ucPreSize);

HI_U32 mrsFillProductVer(HI_CHAR * buf, HI_U16 buf_len);
HI_PRV HI_U32 mrsSetProductVerExtend(HI_U8 *pBuf, HI_U16 usBufLen);


#define DI_04A00101     (0x04A00101)
#define DI_04A00102     (0x04A00102)
#define DI_04A00201     (0x04A00201)
#define DI_04A00300     (0x04A00300)
#define DI_04A00301     (0x04A00301)
#define DI_04A00303     (0x04A00303)
#define DI_04A00503     (0x04A00503)
#define DI_04A00601     (0x04A00601)
#define DI_04A00701     (0x04A00701)
#define DI_04A00800     (0x04A00800)
#define DI_04A00801     (0x04A00801)
#define DI_04A0080A     (0x04A0080A)
#define DI_04A09901     (0x04A09901)
#define DI_04A09902     (0x04A09902)
#define DI_04A09903     (0x04A09903)

/* BEGIN: PN:  MODIFY\ADD\DEL by cuichunyan/00276962 at 2015/10/08 */
#define DI_04A09904     (0x04A09904) //II��485͸��ת��
/* END:   PN:  MODIFY\ADD\DEL by cuichunyan/00276962 at 2015/10/08 */

#define DI_04A00401     (0x04A00401)
#define DI_04A00402     (0x04A00402)
#define DI_04A00403     (0x04A00403)

MRS_IRDA_DI_PAIR_STRU MRS_IRDA_DI_PAIR_MAP_11H[] =
{
    {DI_04A00101, DI_04A00101, mrsIr645_11H_DI04A00101},  // ��������汾��
	{DI_04A00102, DI_04A00102, mrsIr645_11H_DI04A00102},  // ��������汾��չ
    {DI_04A00201, DI_04A00201, mrsIr645_11H_DI04A00201},  // �ն˸�λ����
    {DI_04A00300, DI_04A00300, mrsIr645_11H_DI04A00300},  // ע�Ἧ�����߼���ַ
    {DI_04A00301, DI_04A00301, mrsIr645_11H_DI04A00301},  // II���ѱ���Ϣ
    {DI_04A00303, DI_04A00303, mrsIr645_11H_DI04A00303},  // �ɼ����ʲ���
	{DI_04A00503, DI_04A00503, mrsIr645_11H_DI04A00503},  // �ն�����ʱ��
	{DI_04A00601, DI_04A00601, mrsIr645_11H_DI04A00601},  // ���ɼ����ڵ������
	{DI_04A00701, DI_04A00701, mrsIr645_11H_DI04A00701},  // ���ɼ����ڵ������
	{DI_04A00800, DI_04A0080A, mrsIr645_11H_DI04A0080X},  // �ɼ���ͣ�����,�ɼ�������N�Σ�����ʱ�̣�����ʱ��
    {DI_04A09901, DI_04A09901, mrsIr645_11H_DI04A09901},  // ��ѯ�¼�������
    {DI_04A09902, DI_04A09902, mrsIr645_11H_DI04A09902},  // ��ѯ�¼�����
    {DI_04A09903, DI_04A09903, mrsIr645_11H_DI04A09903},  // MAC��ַ
};

MRS_IRDA_DI_PAIR_STRU MRS_IRDA_DI_PAIR_MAP_12H[] =
{
    {DI_04A00301, DI_04A00301, mrsIr645_11H_DI04A00301},  // II���ѱ���Ϣ
};

MRS_IRDA_DI_PAIR_STRU MRS_IRDA_DI_PAIR_MAP_14H[] =
{
    {DI_04A00300, DI_04A00300, mrsIr645_14H_DI04A00300},  // ע�Ἧ�����߼���ַ
    {DI_04A00303, DI_04A00303, mrsIr645_14H_DI04A00303},  // �ɼ����ʲ���
    {DI_04A00401, DI_04A00401, mrsIr645_14H_DI04A00401},  // Ӳ����ʼ��
    {DI_04A00402, DI_04A00402, mrsIr645_14H_DI04A00402},  // ���ݳ�ʼ��
    {DI_04A00403, DI_04A00403, mrsIr645_14H_DI04A00403},  // ������ʼ��
	{DI_04A00503, DI_04A00503, mrsIr645_14H_DI04A00503},  // �ն�����ʱ��
};

HI_U32 mrsAssembleFrame(HI_VOID **ppFrame, HI_U16 *pFrmLen, HI_U8 *pPayload, HI_U8 ucPayloadLen, 
								 MRS_COLLECTOR_645_FRAME_STRU *p645Frame, HI_U8 ucPreSize);

HI_PRV MRS_IRDA_SM_REPORT_STRU * mrsIrDAReportSMGetContext(HI_VOID)
{
    return &g_SMReportCtx;
}


HI_PRV HI_VOID mrsIrDAReportSMContextReset(HI_VOID)
{
    (hi_void)memset_s(&g_SMReportCtx, sizeof(g_SMReportCtx), 0, sizeof(g_SMReportCtx));
}

/* BEGIN: PN: DTS2015102007386  MODIFY\ADD\DEL by cuichunyan/00276962 at 2015/10/21 */


HI_U32 mrsIrGetTransmit(MRS_COLLECTOR_645_FRAME_STRU * pst645Frame)
{
    HI_U32 ulTransmitType = MRS_CLTII_IR_TRANSMIT_TYPE_DISABLE;
    MRS_COLLECTOR_CTX_STRU *pstCltIICtx = HI_NULL;

    if ((pst645Frame->ucCtrl == MRS_645_FRAME_CONTROL_READ_DATA) 
        && (MRS_COMBIN32(pst645Frame->ucDataRealm[0], pst645Frame->ucDataRealm[1], pst645Frame->ucDataRealm[2], pst645Frame->ucDataRealm[3])) == DI_04A09904)
    {
        ulTransmitType = MRS_CLTII_IR_TRANSMIT_TYPE_SPECIAL;
    }
    else
    {
        pstCltIICtx = mrsGetCollectorCtx();
        //�ж�NV�����Ƿ��
        if (pstCltIICtx->ucIrTrasmit645 == HI_TRUE)//NV�򿪣���ת��
        {
            ulTransmitType = MRS_CLTII_IR_TRANSMIT_TYPE_ENABLE;
        }
    }
    
    return ulTransmitType;
}

HI_U32 mrsIrSendQueue(HI_U8 ucSubId,HI_PBYTE pucDatagram,HI_U8 ucDataRealmLen,HI_U8 ucBaudRateIndex)
{ 
    MRS_STA_ITEM *item = HI_NULL;
    MRS_STA_SRV_CTX_STRU * sta = mrsStaGetContext();
    
    item = (MRS_STA_ITEM *)mrsToolsMalloc(sizeof(MRS_STA_ITEM) + ucDataRealmLen);
    if (!item)
    {
        return  HI_ERR_MALLOC_FAILUE;
    }
    
    (hi_void)memset_s(item, sizeof(MRS_STA_ITEM) + ucDataRealmLen, 0, sizeof(MRS_STA_ITEM) + ucDataRealmLen);
    item->id = ID_MRS_CMD_IR_CMD;
    item->buf = (HI_U8 *)item + sizeof(MRS_STA_ITEM);
    item->from = MRS_STA_QUEUE_FROM_IR;
    item->ucbaudrate_index = ucBaudRateIndex;
    item->len = ucDataRealmLen;
    item->time_out = 20; // 2s
    item->sub_id = ucSubId;
    (hi_void)memcpy_s(item->buf, ucDataRealmLen, pucDatagram, ucDataRealmLen);

    if (!mrsStaTryEnQueue(&(sta->stQueModule), item))
    {
        mrsToolsFree(item);
    }
    
    mrsStaNotifyQueue();
    return HI_ERR_SUCCESS;
            
}
/* END: PN: DTS2015102007386  MODIFY\ADD\DEL by cuichunyan/00276962 at 2015/10/21 */


//*****************************************************************************
// ��������: mrsCollectorIrDARx
// ��������: ��ȡ�Ӻ�����չ���������֡
//           
// ����˵��:
//    pstRxData [IN]���յ��ĺ�������
//                            
// �� �� ֵ: 
//    �ɹ�����  HI_ERR_SUCCESS
//    ʧ�ܷ���  HI_ERR_FAILURE
//    ������������ HI_ERR_CONTINUE
// ����Ҫ��: 
// ���þ���: 
// ��    ��: fengxiaomin/f00209182 [2012-08-28]
//*****************************************************************************
HI_U32 mrsCollectorIrDARx(HI_DMS_CHL_RX_S* pstRxData)
{
    HI_U32 ret = HI_ERR_SUCCESS;
    MRS_ONE_RAW_FRAME_STRU stFrame = {0};
    MRS_COLLECTOR_645_FRAME_STRU *pst645Frame = HI_NULL;
    MRS_IR_645_DI_FUN pIrDIProc = HI_NULL;
/* BEGIN: PN: DTS2015102007386  MODIFY\ADD\DEL by cuichunyan/00276962 at 2015/10/08 */
    HI_U32 ulTransmitType = MRS_CLTII_IR_TRANSMIT_TYPE_DISABLE;
    HI_U8 ucSubId = MRS_IR_SUBID_SPECIAL_TRANSMIT;
    HI_PBYTE pucDatagram = HI_NULL;
    HI_U8 ucDataRealmLen = 0;
    HI_U8 ucBaudRateIndex = 0;
/* END: PN: DTS2015102007386  MODIFY\ADD\DEL by cuichunyan/00276962 at 2015/10/08 */
    
    HI_U8 *pData = HI_NULL;
    HI_U16 usDataLen = 0;

    if (pstRxData == HI_NULL)
    {
        ret = HI_ERR_FAILURE;
        return ret;
    }
    
    stFrame.bCanDiscardOldData = (HI_BOOL)pstRxData->bCanDiscardOldData;
    
    mrs645ProtoStreamInputCheck((HI_BOOL)pstRxData->bCanDiscardOldData, ID_MRS_IRDA_645BUF);

    // ��ȡ��������֡����
    ret = mrs645ProtoStreamInput(pstRxData->pPayload, pstRxData->usPayloadLen, &stFrame, ID_MRS_IRDA_645BUF);
    if (HI_ERR_SUCCESS != ret)
    {
        return ret;
    }

    HI_DIAG_LOG_MSG_E0(MRS_FILE_LOG_FLAG_011, HI_DIAG_MT("�յ�����645֡"));
    HI_DIAG_LOG_BUF(MRS_FILE_LOG_BUF_1000, HI_DIAG_MT("����"), stFrame.pucDatagram, stFrame.usDatagramSize);

    do
    {
        //����645֡
        pst645Frame = (MRS_COLLECTOR_645_FRAME_STRU *)mrsToolsMalloc(sizeof(MRS_COLLECTOR_645_FRAME_STRU));
        if(pst645Frame == HI_NULL)
        {
            HI_DIAG_LOG_MSG_E0(MRS_FILE_LOG_FLAG_001, HI_DIAG_MT("no memory"));
            ret = HI_ERR_FAILURE;
            break;
        }

        (hi_void)memset_s(pst645Frame, sizeof(MRS_COLLECTOR_645_FRAME_STRU), 0, sizeof(MRS_COLLECTOR_645_FRAME_STRU));

        //���645֡�����Ƿ���ȷ
        ret = mrsIrDA645Dec(stFrame.pucDatagram, stFrame.usDatagramSize, pst645Frame, MRS_CLTII_IR_FROM);
        if (ret == HI_ERR_SUCCESS) 
        {
            pIrDIProc = mrsIrFrameProc(pst645Frame->ucCtrl, pst645Frame->ucDataRealm);
            if (pIrDIProc)
            {
                ret = pIrDIProc(pst645Frame, (HI_PVOID *)&pData, &usDataLen, MRS_PREFIX_FE_SIZE);
                if (ret != HI_ERR_SUCCESS)
                {
                    mrsAssembleDFrame((HI_VOID **)&pData, &usDataLen, MRS_IR_ERR_NOT_EXIST, 
									  pst645Frame, MRS_PREFIX_FE_SIZE);
                }

                break;
            }
        }
/* BEGIN: PN: DTS2015102007386  MODIFY\ADD\DEL by cuichunyan/00276962 at 2015/10/08 */
        ulTransmitType = mrsIrGetTransmit(pst645Frame);
        if (ulTransmitType == MRS_CLTII_IR_TRANSMIT_TYPE_DISABLE)//����Ҫת��
        {
            break;
        }
      
        if (mrsSrvQueueLength(&(mrsStaGetContext()->stQueModule.stMrQueue)) != 0)//���в�Ϊ��
        {
            mrsAssembleDFrame((HI_VOID **)&pData, &usDataLen, MRS_IR_ERR_BUSY, 
							  pst645Frame, MRS_PREFIX_FE_SIZE);
            ret = HI_ERR_BUSY;
            break;
        }
       
        if (ulTransmitType == MRS_CLTII_IR_TRANSMIT_TYPE_SPECIAL)//����DIת��
        {
            ucSubId = MRS_IR_SUBID_SPECIAL_TRANSMIT;
            ucBaudRateIndex = pst645Frame->ucDataRealm[MRS_645_FRAME_DATA_DI_SIZE];
            pucDatagram = pst645Frame->ucDataRealm + MRS_645_FRAME_DATA_DI_SIZE + sizeof(HI_U8);
            ucDataRealmLen = (HI_U8)(pst645Frame->ucDataRealmLen - MRS_645_FRAME_DATA_DI_SIZE - sizeof(HI_U8));
           
        }
        else //if(ulTransmitType == MRS_CLTII_IR_TRANSMIT_TYPE_NOMAL)
        {
            ucSubId = MRS_IR_SUBID_TRANSMIT;
            pucDatagram = stFrame.pucDatagram;
            ucDataRealmLen = (HI_U8)stFrame.usDatagramSize; 
            ucBaudRateIndex = 0;//����Ĭ�ϲ����ʷ���
            
        }
        ret = mrsIrSendQueue(ucSubId, pucDatagram,ucDataRealmLen,ucBaudRateIndex);
/* END: PN: DTS2015102007386  MODIFY\ADD\DEL by cuichunyan/00276962 at 2015/10/08 */
    } while (0);

    if ((usDataLen > 0) && (pst645Frame && !mrsToolsBroadCastAddr(pst645Frame->ucAddr)))
    {
        // ��������ƻ�ͨѶ ��ʱ100ms��Ӧ
        HI_MDM_Sleep(100);
        
        MRS_SendMrData(pData, usDataLen, HI_DMS_CHL_UART_PORT_IR);
        HI_DIAG_LOG_BUF(MRS_FILE_LOG_BUF_1003, HI_DIAG_MT("ANSWER"), pData, usDataLen);
    }

    mrsToolsFree(stFrame.pucDatagram);
    mrsToolsFree(pData);
    mrsToolsFree(pst645Frame);

    return ret;
}

//*****************************************************************************
// ��������: mrsIrDA645Dec
// ��������: ����II�Ͳɼ���������յ�����645֡
//           
// ����˵��:
//    pucData     [IN] 645֡
//    usDataSize  [IN] 645֡����
//    pst645Frame [OUT]645֡�ṹ
//                            
// �� �� ֵ: 
//    �ɹ�����  HI_ERR_SUCCESS
//    ʧ�ܷ���  HI_ERR_FAILURE
//
// ����Ҫ��: 
// ���þ���: 
// ��    ��: fengxiaomin/f00209182 [2012-08-29]
//*****************************************************************************
HI_U32 mrsIrDA645Dec(HI_U8* pucData, HI_U16 usDataSize, MRS_COLLECTOR_645_FRAME_STRU *pst645Frame, HI_U8 ucFrom)
{
    HI_U8 *pucBuffer = pucData;

    HI_UNREF_PARAM(usDataSize);

    if (pucData == HI_NULL || pst645Frame == HI_NULL)
    {
        return HI_ERR_FAILURE;
    }

    pucBuffer++;
    (hi_void)memcpy_s(pst645Frame->ucAddr, HI_METER_ADDR_LEN, pucBuffer, HI_METER_ADDR_LEN); //�ɼ�����ַ

    HI_DIAG_LOG_BUF(MRS_FILE_LOG_BUF_1001, HI_DIAG_MT("��ַ"), pst645Frame->ucAddr, HI_METER_ADDR_LEN);

    pucBuffer += HI_METER_ADDR_LEN;
    pucBuffer++;
    pst645Frame->ucCtrl = *(pucBuffer++); //������
    pst645Frame->ucDataRealmLen = *(pucBuffer++);
	if(sizeof(pst645Frame->ucDataRealm) >= pst645Frame->ucDataRealmLen){
		(hi_void)memcpy_s(pst645Frame->ucDataRealm, sizeof(pst645Frame->ucDataRealm), pucBuffer, pst645Frame->ucDataRealmLen);	
	}    
    mrs645DataDecode(pst645Frame->ucDataRealm, pst645Frame->ucDataRealmLen);

	return mrsIrDACheckAddr(pst645Frame->ucAddr, ucFrom);
}

//*****************************************************************************
// ��������: mrsIrDACheckAddr
// ��������: II�Ͳɼ����ж��ʲ���
//           
// ����˵��:
//    aucAddr     [IN] �յ��������еĵ�ַ
//    ucFrom      [IN] ������Դ
//                            
// �� �� ֵ: 
//    ��ȷ����  HI_ERR_SUCCESS
//    ���󷵻�  HI_ERR_FAILURE
//
// ����Ҫ��: 
// ���þ���: 
// ��    ��: fengxiaomin/f00209182 [2015-11-16]
//*****************************************************************************
HI_U32 mrsIrDACheckAddr(HI_U8 aucAddr[], HI_U8 ucFrom)
{
    HI_U8 aucCltAddr[HI_METER_ADDR_LEN] = {0};
	
    mrsIrDAGetCltAddr(aucCltAddr);
	if (MRS_CLTII_IR_FROM == ucFrom)
	{
		if (!mrsMeterAddrMatch(aucAddr, aucCltAddr) && !mrsToolsBroadCastAddr(aucAddr))
		{
			return HI_ERR_FAILURE;
		}
	}
	else
	{
		if (mrsToolsCollectorAddr(aucAddr) || !mrsMeterAddrMatch(aucAddr, aucCltAddr))
		{
			return HI_ERR_FAILURE;
		}
	}

	return HI_ERR_SUCCESS;
}

//*****************************************************************************
// ��������: mrsIrDAGetDIFunc
// ��������: II�Ͳɼ�����ȡ��������DI��Ӧ�Ĵ�����
//           
// ����˵��:
//    ucCtrl      [IN] 645֡�еĿ�����
//    pDataRealm  [IN] 645֡�е�������
//    pFunc       [OUT]��ȡ���Ĵ�����
//                            
// �� �� ֵ: 
//    �ɹ�����  HI_ERR_SUCCESS
//    ʧ�ܷ���  HI_ERR_FAILURE
//
// ����Ҫ��: 
// ���þ���: 
// ��    ��: fengxiaomin/f00209182 [2015-11-16]
//*****************************************************************************
HI_U32 mrsIrDAGetDIFunc(HI_U8 ucCtrl, HI_U8 *pDataRealm, HI_U32 *pFunc)
{
    MRS_IRDA_DI_PAIR_STRU * pair = HI_NULL;
    HI_S32 size = 0;
    HI_S32 index;
    HI_U32 di = 0;

    if (HI_NULL == pDataRealm)
    {
        return HI_ERR_FAILURE;
    }

    switch (ucCtrl)
    {
        case MRS_645_FRAME_CONTROL_READ_DATA:
            pair = MRS_IRDA_DI_PAIR_MAP_11H;
            size = MRS_TOOLS_ALEN(MRS_IRDA_DI_PAIR_MAP_11H);
            break;

        case MRS_645_FRAME_CONTROL_READ_FOLLOW:
            pair = MRS_IRDA_DI_PAIR_MAP_12H;
            size = MRS_TOOLS_ALEN(MRS_IRDA_DI_PAIR_MAP_12H);
            break;

        case MRS_645_FRAME_CONTROL_WRITE_DATA:
            pair = MRS_IRDA_DI_PAIR_MAP_14H;
            size = MRS_TOOLS_ALEN(MRS_IRDA_DI_PAIR_MAP_14H);
            break;

        default:
            return HI_ERR_NOT_FOUND;
    }

    di = MRS_COMBIN32(pDataRealm[0], pDataRealm[1], pDataRealm[2], pDataRealm[3]);

    for (index = 0; index < size; index++)
    {
        if ((di >= pair[index].ulMinDI) && (di <= pair[index].ulMaxDI))
        {
            *pFunc = (HI_U32)pair[index].func;
            return HI_ERR_SUCCESS;
        }
    }

    return HI_ERR_NOT_FOUND;
}

//*****************************************************************************
// ��������: mrsIrFrameProc
// ��������: ��ÿһ�����յ�����֡������Ӧ�Ĵ���
//           
// ����˵��:
//    ucCtrl      [IN]645֡�п�����
//    pDataRealm  [IN]645֡���������ַ
//
// �� �� ֵ: 
//    �ҵ���Ӧ�Ĵ��������غ���ָ��
//    �Ҳ�����Ӧ�Ĵ���������0
//
// ����Ҫ��: 
// ���þ���: 
// ��    ��: fengxiaomin/f00209182 [2012-08-29]
//*****************************************************************************
MRS_IR_645_DI_FUN mrsIrFrameProc(HI_U8 ucCtrl, HI_U8 *pDataRealm)
{
    MRS_IR_645_DI_FUN pIrDIProc = HI_NULL;

	mrsIrDAGetDIFunc(ucCtrl, pDataRealm, (HI_U32*)&pIrDIProc);
    if ((!pIrDIProc) || (ucCtrl != MRS_645_FRAME_CONTROL_READ_FOLLOW))
    {
        mrsIrDAReportSMContextReset();
    }

    return pIrDIProc;
}


HI_VOID mrsIrDAGetCltAddr(HI_U8 *pAddr)
{
    HI_NV_FTM_COLLECT_PARAM_FILE_S stNv;

    (hi_void)memset_s(&stNv, sizeof(stNv), 0, sizeof(stNv));

    if (HI_ERR_SUCCESS == HI_MDM_NV_Read(HI_NV_FTM_COLLECT_PARAM_FILE, &stNv, sizeof(stNv)))
    {
        (hi_void)memcpy_s(pAddr, HI_METER_ADDR_LEN, stNv.CLTAddress, HI_METER_ADDR_LEN);
    }
    else
    {
        (hi_void)memcpy_s(pAddr, HI_METER_ADDR_LEN, MRS_COLLECTOR_ADDR, HI_METER_ADDR_LEN);
    }
}


//*****************************************************************************
// ��������: mrsAssembleFrame
// ��������: ��֯645��Ӧ֡
//           
// ����˵��:
//    ppFrame     [OUT]��֯�ɹ���֡
//    pFrmLen     [OUT]��֯�ɹ���֡����
//    pPayload    [IN] ����
//    ucPayloadLen[IN] �������ݳ���
//    p645Frame   [IN] ���յ��ı���
//                            
// �� �� ֵ: 
//    ��֡�ɹ�����  HI_ERR_SUCCESS
//    ��֡ʧ�ܷ���  HI_ERR_FAILURE
//
// ����Ҫ��: 
// ���þ���: 
// ��    ��: fengxiaomin/f00209182 [2012-08-29]
// ��    ��: cuiate/c00233580 [2015-05-30]
//*****************************************************************************
HI_U32 mrsAssembleFrame(HI_VOID **ppFrame, HI_U16 *pFrmLen, HI_U8 *pPayload, HI_U8 ucPayloadLen, 
								 MRS_COLLECTOR_645_FRAME_STRU *p645Frame, HI_U8 ucPreSize)
{
    HI_U16 usBufLength = MRS_645_MIN_LENGTH + ucPreSize + ucPayloadLen;
    HI_U8 * pBuffer = HI_NULL;
    HI_U16 usLength = 0;
    HI_U8 i = 0;

    if (((ucPayloadLen > 0) && (!pPayload)) || (!p645Frame))
    {
        return HI_ERR_FAILURE;
    }

    pBuffer = mrsToolsMalloc(usBufLength);
    if (!pBuffer)
    {
        return HI_ERR_MALLOC_FAILUE;
    }

    (hi_void)memset_s(pBuffer, usBufLength, 0, usBufLength);
    // ǰ��FE
    for (i = 0; i < ucPreSize; i++)
    {
        pBuffer[usLength++] = 0xFE;
    }

    // HEAD
    pBuffer[usLength++] = MRS_645_FRAME_START_FLG;

    // ADDR
    mrsIrDAGetCltAddr(pBuffer + usLength);
    usLength += HI_METER_ADDR_LEN;

    // HEAD 2
    pBuffer[usLength++] = MRS_645_FRAME_START_FLG;

    // CTRL
    pBuffer[usLength++] = p645Frame->ucCtrl | MRS_645_FRAME_CONTROL_DIR_UP;

    // LENGTH
    pBuffer[usLength++] = ucPayloadLen;

    // DATA
    for (i = 0; i < ucPayloadLen; i++)
    {
        pBuffer[usLength++] = pPayload[i] + MRS_645_FRAME_HEX33;
    }

    // CS
    pBuffer[usLength] = mrsToolsCalcCheckSum(pBuffer + ucPreSize, usLength - ucPreSize);
    usLength++;

    // END
    pBuffer[usLength++] = MRS_645_FRAME_END_FLG;

    *ppFrame = pBuffer;
    *pFrmLen = usLength;

    return HI_ERR_SUCCESS;
}


HI_U32 mrsAssembleDFrame(HI_VOID **ppFrame, HI_U16 *pFrmLen, HI_U8 ucErrCode, 
								   MRS_COLLECTOR_645_FRAME_STRU *p645Frame, HI_U8 ucPreSize)
{
    if (!p645Frame)
    {
        return HI_ERR_FAILURE;
    }

    p645Frame->ucCtrl |= MRS_645_FRAME_CONTROL_DENY_MASK;

    return mrsAssembleFrame(ppFrame, pFrmLen, &ucErrCode, sizeof(ucErrCode), p645Frame, ucPreSize);
}


HI_U32 mrsFillProductVer(HI_CHAR * buf, HI_U16 buf_len)
{
    HI_U32 ulRet = HI_ERR_SUCCESS;
    HI_CHAR * strVer[4] = {MRS_V_VERSION_STRING, MRS_R_VERSION_STRING, MRS_C_VERSION_STRING, MRS_SPC_VERSION_STRING};
    HI_U16 width[4] = {MRS_V_VERSION_WIDTH, MRS_R_VERSION_WIDTH, MRS_C_VERSION_WIDTH, MRS_SPC_VERSION_WIDTH};
    HI_U32 i = 0;
    HI_U16 maxLen = 0;
	NV_MRS_USER_VER_S stVer;

    (hi_void)memset_s(&stVer, sizeof(stVer), 0, sizeof(stVer));
	ulRet = mrsGetUserVerNvCfg(&stVer);
	if (HI_ERR_SUCCESS != ulRet)
	{
		return ulRet;
	}

    for (i = 0; i < MRS_TOOLS_ALEN(width); i++)
    {
        HI_CHAR * temp = HI_NULL;

        maxLen += width[i] + (HI_U16)strlen(strVer[i]);
        temp = mrsFindStrEnd(stVer.aucUserVersion, strVer[i]);
        if (temp && maxLen < buf_len)
        {
            strncat_s(buf, buf_len, strVer[i], strlen(strVer[i]));
            strncat_s(buf, buf_len, temp, width[i]);
        }
    }
    
    return HI_ERR_SUCCESS;
}

// ��������汾��
HI_U32 mrsIr645_11H_DI04A00101(HI_VOID *param, HI_VOID **ppOutParam, HI_U16 *pOutParamLen, HI_U8 ucPreSize)
{
    HI_U8 payload[36];
	HI_U32 ulDI = DI_04A00101;

    (hi_void)memset_s(payload, sizeof(payload), 0, sizeof(payload));
    (hi_void)memcpy_s(payload, sizeof(payload), &ulDI, MRS_SRV_645_DATA_INDEX_SIZE);
    mrsFillProductVer((HI_CHAR *)(payload + sizeof(HI_U32)), (HI_U16)(sizeof(payload) - sizeof(HI_U32)));
	mrsHexInvert(payload + sizeof(HI_U32), (HI_U16)(sizeof(payload) - sizeof(HI_U32)));
	
    return mrsAssembleFrame(ppOutParam, pOutParamLen, payload, sizeof(payload), 
							(MRS_COLLECTOR_645_FRAME_STRU*)param, ucPreSize);
}

//*****************************************************************************
// ��������: mrsSetProductVerExtend
// ��������: II��������չ�ĳ�������汾
//           
// ����˵��:
//    ��
//                            
// �� �� ֵ: 
//    �ɹ�����  HI_ERR_SUCCESS
//
// ����Ҫ��: 
// ���þ���: 
// ��    ��: fengxiaomin/f00209182 [2015-10-30]
//*****************************************************************************
HI_U32 mrsSetProductVerExtend(HI_U8 *pBuf, HI_U16 usBufLen)
{
	HI_U32 ulRet = HI_ERR_SUCCESS;
	HI_U8 ucOffset = 0;
	HI_U8 cCltVer[MRS_CLTII_EXTEND_VER_LEN] = {0,0,0,0,'2','C'};
	HI_MDM_SYS_INFO_S *pstVerInfo = HI_NULL;
	
    pstVerInfo = (HI_MDM_SYS_INFO_S *)HI_MDM_GetSysInfo();
	if (!pstVerInfo)
	{
		return HI_ERR_FAILURE;
	}
	
    (hi_void)memcpy_s(pBuf, usBufLen, cCltVer, MRS_CLTII_EXTEND_VER_LEN);
	ucOffset += MRS_CLTII_EXTEND_VER_LEN;
    (hi_void)memcpy_s(&pBuf[ucOffset], usBufLen - ucOffset, pstVerInfo->szManuCode, MRS_MANU_CODE_LEN);
	ucOffset += MRS_MANU_CODE_LEN;
    mrsIntToBcd((HI_U32)pstVerInfo->usDay, &pBuf[ucOffset], sizeof(HI_U8));
    ucOffset++;
    mrsIntToBcd((HI_U32)pstVerInfo->usMonth, &pBuf[ucOffset], sizeof(HI_U8));
    ucOffset++;
    mrsIntToBcd((HI_U32)pstVerInfo->usYear, &pBuf[ucOffset], sizeof(HI_U8));
    ucOffset++;
    (hi_void)memcpy_s(&pBuf[ucOffset], usBufLen - ucOffset, &pstVerInfo->usSwVer, sizeof(pstVerInfo->usSwVer));
	ucOffset += sizeof(pstVerInfo->usSwVer);
	pBuf[ucOffset++] = pstVerInfo->ucBootVer;

	return ulRet;
}

//*****************************************************************************
// ��������: mrsIr645_11H_DI04A00102
// ��������: ��ѯ��������汾��չ
//           
// ����˵��:
//    ��
//                            
// �� �� ֵ: 
//    �ɹ�����  HI_ERR_SUCCESS
//
// ����Ҫ��: 
// ���þ���: 
// ��    ��: fengxiaomin/f00209182 [2015-10-30]
//*****************************************************************************
HI_U32 mrsIr645_11H_DI04A00102(HI_VOID *param, HI_VOID **ppOutParam, HI_U16 *pOutParamLen, HI_U8 ucPreSize)
{
	HI_U32 ulRet = HI_ERR_SUCCESS;
    HI_U8 payload[36];
	HI_U32 ulDI = DI_04A00102;

    (hi_void)memset_s(payload, sizeof(payload), 0, sizeof(payload));
    (hi_void)memcpy_s(payload, sizeof(payload), &ulDI, MRS_SRV_645_DATA_INDEX_SIZE);
    ulRet = mrsSetProductVerExtend(payload + sizeof(HI_U32), (HI_U16)(sizeof(payload) - sizeof(HI_U32)));
	if (HI_ERR_SUCCESS != ulRet)
	{
		return ulRet;
	}

    return mrsAssembleFrame(ppOutParam, pOutParamLen, payload, sizeof(payload), 
							(MRS_COLLECTOR_645_FRAME_STRU*)param, ucPreSize);
}

// �ն˸�λ����
HI_U32 mrsIr645_11H_DI04A00201(HI_VOID *param, HI_VOID **ppOutParam, HI_U16 *pOutParamLen, HI_U8 ucPreSize)
{
    HI_U8 payload[6] = {0x01, 0x02, 0xA0, 0x04, 0x00, 0x00};
    HI_U16 report_times = 0;    
    HI_U32 real_times = HI_MDM_GetRelativeRstTimes(); 
    
    // ��λ�������ֵΪ9999(����Լ��)
    report_times = real_times % 10000;
    mrsIntToBcd(report_times, payload + MRS_SRV_645_DATA_INDEX_SIZE, 2);

    return mrsAssembleFrame(ppOutParam, pOutParamLen, payload, sizeof(payload), 
							(MRS_COLLECTOR_645_FRAME_STRU*)param, ucPreSize);
}


// ע�Ἧ�����߼���ַ
HI_U32 mrsIr645_11H_DI04A00300(HI_VOID *param, HI_VOID **ppOutParam, HI_U16 *pOutParamLen, HI_U8 ucPreSize)
{
    HI_U8 payload[] = {0x00, 0x03, 0xA0, 0x04, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00};
    HI_U32 ret = HI_ERR_SUCCESS;
    HI_NV_FTM_COLLECT_PARAM_FILE_S nv;

    (hi_void)memset_s(&nv, sizeof(nv), 0, sizeof(nv));

    ret = HI_MDM_NV_Read(HI_NV_FTM_COLLECT_PARAM_FILE, &nv, sizeof(nv));
    if (HI_ERR_SUCCESS != ret)
    {
        return HI_ERR_FAILURE;
    }

    (hi_void)memcpy_s(payload + MRS_SRV_645_DATA_INDEX_SIZE, sizeof(payload) - MRS_SRV_645_DATA_INDEX_SIZE, nv.CCTAddress, sizeof(nv.CCTAddress));
    *(payload + MRS_SRV_645_DATA_INDEX_SIZE + sizeof(nv.CCTAddress)) = nv.CCT_register;

    return mrsAssembleFrame(ppOutParam, pOutParamLen, payload, sizeof(payload), 
							(MRS_COLLECTOR_645_FRAME_STRU*)param, ucPreSize);
}


// II���ѱ���Ϣ
HI_U32 mrsIr645_11H_DI04A00301(HI_VOID *param, HI_VOID **ppOutParam, HI_U16 *pOutParamLen, HI_U8 ucPreSize)
{
    MRS_COLLECTOR_645_FRAME_STRU* frame = (MRS_COLLECTOR_645_FRAME_STRU*)param;
    MRS_IRDA_SM_REPORT_STRU * sm_report = mrsIrDAReportSMGetContext();
    HI_U8 * payload = HI_NULL;
    HI_U8 payload_len = 0;
    HI_U32 ret = HI_ERR_SUCCESS;
    HI_U8 i;
    HI_U8 this_num;
    HI_U8 seq = 0;
    HI_U8 seq_len = 0;
	HI_U32 ulDI = DI_04A00301;

    if (HI_NULL == param)
    {
        return HI_ERR_FAILURE;
    }

    if (frame->ucCtrl == MRS_645_FRAME_CONTROL_READ_DATA)
    {
        MRS_STA_SEARCH_CTX * pstSmCtx = mrsStaSearchCtx();
        MRS_SEARCH_METERLIST_STRU * pstMeterList = &(pstSmCtx->stMeterList);
        MRS_METER_ITEM_S * pItem = HI_NULL;

        pItem = pstMeterList->astMeter;
        for (i = 0; i < pstMeterList->ucMeterNum; i++, pItem++)
        {
            mrsMeterAddrPreProc(sm_report->astItem[i].aucMeterAddr, pItem->ucMeterAddr, pItem->ucProtocol);
            sm_report->astItem[i].ucProtocol = pItem->ucProtocol;
        }
/* BEGIN: PN: DTS2015061608880 MODIFY\ADD\DEL by cuiate/00233580 at 2015/6/17 */
        sm_report->ucTotal = pstMeterList->ucMeterNum;

        if (pstMeterList->ucMeterNum > MRS_SM_REPORT_MAX_METERNUM)
        {
            sm_report->bValid = HI_TRUE;
        }
/* END:   PN: DTS2015061608880 MODIFY\ADD\DEL by cuiate/00233580 at 2015/6/17 */
    }
    else if (frame->ucCtrl == MRS_645_FRAME_CONTROL_READ_FOLLOW)
    {
        if (HI_TRUE != sm_report->bValid)
        {
            return HI_ERR_FAILURE;
        }

        if (frame->ucDataRealmLen == (MRS_645_FRAME_DATA_DI_SIZE + 1))
        {
            seq = frame->ucDataRealm[MRS_645_FRAME_DATA_DI_SIZE];
            if ((seq == 0) || ((seq != sm_report->ucSeq) && (seq != sm_report->ucSeq + 1)))
            {
                return HI_ERR_FAILURE;
            }

            sm_report->ucSent = (HI_U8)(MRS_SM_REPORT_MAX_METERNUM * seq);
            seq_len = 1;
        }
        else if (frame->ucDataRealmLen != MRS_645_FRAME_DATA_DI_SIZE)
        {
            return HI_ERR_FAILURE;
        }

        if (sm_report->ucSent >= sm_report->ucTotal)
        {
            return HI_ERR_FAILURE;
        }
    }
    else
    {
        return HI_ERR_FAILURE;
    }

    this_num = MRS_MIN(sm_report->ucTotal - sm_report->ucSent, MRS_SM_REPORT_MAX_METERNUM);
    
    payload_len = (HI_U8)(MRS_SRV_645_DATA_INDEX_SIZE + 1 + 7 * this_num + seq_len);
    payload = mrsToolsMalloc(payload_len);
    if (HI_NULL == payload)
    {
        return HI_ERR_MALLOC_FAILUE;
    }

    (hi_void)memcpy_s(payload, payload_len, &ulDI, MRS_SRV_645_DATA_INDEX_SIZE);
    mrsIntToBcd(sm_report->ucTotal, &payload[4], 1);

    for (i = 0; i < this_num; i++)
    {
        (hi_void)memcpy_s(payload + MRS_SRV_645_DATA_INDEX_SIZE + 1 + 7 * i, payload_len - MRS_SRV_645_DATA_INDEX_SIZE + 1 + 7 * i,
						sm_report->astItem[i + sm_report->ucSent].aucMeterAddr, HI_METER_ADDR_LEN);
        *(payload + MRS_SRV_645_DATA_INDEX_SIZE + 1 + 7 * i + HI_METER_ADDR_LEN) = sm_report->astItem[i + sm_report->ucSent].ucProtocol;
    }

    if (seq_len > 0)
    {
        *(payload + payload_len - 1) = seq;
        sm_report->ucSeq = seq;
    }

    sm_report->ucSent += this_num;
    if (sm_report->ucSent < sm_report->ucTotal)
    {
        frame->ucCtrl |= MRS_645_CONTROL_FOLLOWBIT_ONE;
    }

    ret = mrsAssembleFrame(ppOutParam, pOutParamLen, payload, payload_len, frame, ucPreSize);
    mrsToolsFree(payload);

    return ret;
}


// �ɼ����ʲ���
HI_U32 mrsIr645_11H_DI04A00303(HI_VOID *param, HI_VOID **ppOutParam, HI_U16 *pOutParamLen, HI_U8 ucPreSize)
{
    HI_U8 payload[10] = {0x03, 0x03, 0xA0, 0x04, 0xBB, 0xBB, 0xBB, 0xBB, 0xBB, 0xBB};

    mrsIrDAGetCltAddr(payload + MRS_SRV_645_DATA_INDEX_SIZE);

    return mrsAssembleFrame(ppOutParam, pOutParamLen, payload, sizeof(payload), 
							(MRS_COLLECTOR_645_FRAME_STRU*)param, ucPreSize);
}

//*****************************************************************************
// ��������: mrsIr645_11H_DI04A00503
// ��������: ��ѯ�ն�����ʱ��
//           
// ����˵��:
//    ��
//                            
// �� �� ֵ: 
//    �ɹ�����  HI_ERR_SUCCESS
//
// ����Ҫ��: 
// ���þ���: 
// ��    ��: fengxiaomin/f00209182 [2015-10-30]
//*****************************************************************************
HI_U32 mrsIr645_11H_DI04A00503(HI_VOID *param, HI_VOID **ppOutParam, HI_U16 *pOutParamLen, HI_U8 ucPreSize)
{
    HI_UNREF_PARAM(param);
    HI_UNREF_PARAM(ppOutParam);
    HI_UNREF_PARAM(pOutParamLen);
    HI_UNREF_PARAM(ucPreSize);
    return HI_ERR_NOT_SUPPORT;
}

//*****************************************************************************
// ��������: mrsIr645_11H_DI04A00601
// ��������: ���ɼ����ڵ������
//           
// ����˵��:
//    ��
//                            
// �� �� ֵ: 
//    �ɹ�����  HI_ERR_SUCCESS
//
// ����Ҫ��: 
// ���þ���: 
// ��    ��: fengxiaomin/f00209182 [2015-11-07]
//*****************************************************************************
HI_U32 mrsIr645_11H_DI04A00601(HI_VOID *param, HI_VOID **ppOutParam, HI_U16 *pOutParamLen, HI_U8 ucPreSize)
{
    HI_UNREF_PARAM(param);
    HI_UNREF_PARAM(ppOutParam);
    HI_UNREF_PARAM(pOutParamLen);
    HI_UNREF_PARAM(ucPreSize);
    return HI_ERR_NOT_SUPPORT;
}

//*****************************************************************************
// ��������: mrsIr645_11H_DI04A00701
// ��������: ���ɼ����ڵ������
//           
// ����˵��:
//    ��
//                            
// �� �� ֵ: 
//    �ɹ�����  HI_ERR_SUCCESS
//
// ����Ҫ��: 
// ���þ���: 
// ��    ��: fengxiaomin/f00209182 [2015-11-07]
//*****************************************************************************
HI_U32 mrsIr645_11H_DI04A00701(HI_VOID *param, HI_VOID **ppOutParam, HI_U16 *pOutParamLen, HI_U8 ucPreSize)
{
    HI_UNREF_PARAM(param);
    HI_UNREF_PARAM(ppOutParam);
    HI_UNREF_PARAM(pOutParamLen);
    HI_UNREF_PARAM(ucPreSize);
    return HI_ERR_NOT_SUPPORT;
}

//*****************************************************************************
// ��������: mrsIr645_11H_DI04A0080X
// ��������: ��ѯ�ɼ���ͣ�����,�ɼ�������N�Σ�����ʱ�̣�����ʱ��
//           
// ����˵��:
//    ��
//                            
// �� �� ֵ: 
//    �ɹ�����  HI_ERR_SUCCESS
//
// ����Ҫ��: 
// ���þ���: 
// ��    ��: fengxiaomin/f00209182 [2015-10-30]
//*****************************************************************************
HI_U32 mrsIr645_11H_DI04A0080X(HI_VOID *param, HI_VOID **ppOutParam, HI_U16 *pOutParamLen, HI_U8 ucPreSize)
{
    HI_UNREF_PARAM(param);
    HI_UNREF_PARAM(ppOutParam);
    HI_UNREF_PARAM(pOutParamLen);
    HI_UNREF_PARAM(ucPreSize);
    return HI_ERR_NOT_SUPPORT;
}

// ��ѯ�¼�������
HI_U32 mrsIr645_11H_DI04A09901(HI_VOID *param, HI_VOID **ppOutParam, HI_U16 *pOutParamLen, HI_U8 ucPreSize)
{
    MRS_STA_SEARCH_CTX * pstSmCtx = mrsStaSearchCtx();
    HI_U8 payload[5] = {0x01, 0x99, 0xA0, 0x04, 0x00};

    mrsIntToBcd(pstSmCtx->stEvent.ucNum, payload + MRS_SRV_645_DATA_INDEX_SIZE, 1);

    return mrsAssembleFrame(ppOutParam, pOutParamLen, payload, sizeof(payload), 
							(MRS_COLLECTOR_645_FRAME_STRU*)param, ucPreSize);
}

// ��ѯ�¼�����
HI_U32 mrsIr645_11H_DI04A09902(HI_VOID *param, HI_VOID **ppOutParam, HI_U16 *pOutParamLen, HI_U8 ucPreSize)
{
    MRS_COLLECTOR_645_FRAME_STRU* frame = (MRS_COLLECTOR_645_FRAME_STRU*)param;
    MRS_STA_SEARCH_CTX * pstSmCtx = mrsStaSearchCtx();
    MRS_SEARCH_EVENT_ITEM * item = HI_NULL;
    HI_U8 payload[11] = {0x02, 0x99, 0xA0, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
    HI_U8 index = 0;

    if (HI_NULL == param)
    {
        return HI_ERR_FAILURE;
    }

    index = *(frame->ucDataRealm + MRS_SRV_645_DATA_INDEX_SIZE);
    index = (HI_U8)BCD2INT(index);

    if ((index == 0) || (index > pstSmCtx->stEvent.ucNum))
    {
        return HI_ERR_FAILURE;
    }

    item = pstSmCtx->stEvent.astItem;

    *(payload + MRS_SRV_645_DATA_INDEX_SIZE) = *(frame->ucDataRealm + MRS_SRV_645_DATA_INDEX_SIZE);
    (hi_void)memcpy_s(&payload[MRS_SRV_645_DATA_INDEX_SIZE + 1],
					sizeof(payload) - MRS_SRV_645_DATA_INDEX_SIZE - 1,
					&(item[index-1].ulTime), sizeof(item[index-1].ulTime));
    (hi_void)memcpy_s(&payload[MRS_SRV_645_DATA_INDEX_SIZE + 5],
					sizeof(payload) - MRS_SRV_645_DATA_INDEX_SIZE - 5,
					&(item[index-1].usEvent), sizeof(item[index-1].usEvent));

    return mrsAssembleFrame(ppOutParam, pOutParamLen, payload, sizeof(payload), frame, ucPreSize);
}

// MAC��ַ
HI_U32 mrsIr645_11H_DI04A09903(HI_VOID *param, HI_VOID **ppOutParam, HI_U16 *pOutParamLen, HI_U8 ucPreSize)
{
    HI_U8 payload[10] = {0x03, 0x99, 0xA0, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

    HI_MDM_GetLocalMacAddr(payload + MRS_SRV_645_DATA_INDEX_SIZE);
    return mrsAssembleFrame(ppOutParam, pOutParamLen, payload, sizeof(payload), 
							(MRS_COLLECTOR_645_FRAME_STRU*)param, ucPreSize);
}


/* BEGIN: PN: DTS2015042907048 MODIFY\ADD\DEL by cuiate/00233580 at 2015/4/29 */
// ע�Ἧ�����߼���ַ
HI_U32 mrsIr645_14H_DI04A00300(HI_VOID *param, HI_VOID **ppOutParam, HI_U16 *pOutParamLen, HI_U8 ucPreSize)
{
    MRS_COLLECTOR_645_FRAME_STRU* frame = (MRS_COLLECTOR_645_FRAME_STRU*)param;
    HI_NV_FTM_COLLECT_PARAM_FILE_S nv;
    HI_U32 ret = HI_ERR_SUCCESS;
    HI_U16 offset = 0;

    if ((HI_NULL == param) 
        || !MRS_IRDA_CTRL14_PARAM_VALID(frame->ucDataRealmLen, sizeof(nv.CCTAddress) + MRS_645_FRAME_DATA_DI_SIZE))
    {
        return HI_ERR_FAILURE;
    }

    offset = frame->ucDataRealmLen - sizeof(nv.CCTAddress);

    (hi_void)memset_s(&nv, sizeof(nv), 0, sizeof(nv));
    ret = HI_MDM_NV_Read(HI_NV_FTM_COLLECT_PARAM_FILE, &nv, sizeof(nv));
    if (HI_ERR_SUCCESS != ret)
    {
        return HI_ERR_FAILURE;
    }

    (hi_void)memcpy_s(nv.CCTAddress, sizeof(nv.CCTAddress), frame->ucDataRealm + offset, sizeof(nv.CCTAddress));
    nv.CCT_register = 1;

    ret = HI_MDM_NV_Write(HI_NV_FTM_COLLECT_PARAM_FILE, &nv, sizeof(nv));
    if (HI_ERR_SUCCESS != ret)
    {
        return HI_ERR_FAILURE;
    }

    return mrsAssembleFrame(ppOutParam, pOutParamLen, HI_NULL, 0, frame, ucPreSize);
}
/* END:   PN: DTS2015042907048 MODIFY\ADD\DEL by cuiate/00233580 at 2015/4/29 */


/* BEGIN: PN: DTS2015042907048 MODIFY\ADD\DEL by cuiate/00233580 at 2015/4/29 */
// �ɼ����ʲ���
HI_U32 mrsIr645_14H_DI04A00303(HI_VOID *param, HI_VOID **ppOutParam, HI_U16 *pOutParamLen, HI_U8 ucPreSize)
{
    MRS_COLLECTOR_645_FRAME_STRU* frame = (MRS_COLLECTOR_645_FRAME_STRU*)param;
    HI_NV_FTM_COLLECT_PARAM_FILE_S nv;
    HI_U32 ret = HI_ERR_SUCCESS;
    HI_U16 offset = 0;

    if ((HI_NULL == param) 
        || !MRS_IRDA_CTRL14_PARAM_VALID(frame->ucDataRealmLen, sizeof(nv.CLTAddress) + MRS_645_FRAME_DATA_DI_SIZE))
    {
        return HI_ERR_FAILURE;
    }

    offset = frame->ucDataRealmLen - sizeof(nv.CLTAddress);

    if (!mrsToolsNormalAddr(frame->ucDataRealm + offset) && !mrsToolsCollectorAddr(frame->ucDataRealm + offset))
    {
        return HI_ERR_FAILURE;
    }

    (hi_void)memset_s(&nv, sizeof(nv), 0, sizeof(nv));
    ret = HI_MDM_NV_Read(HI_NV_FTM_COLLECT_PARAM_FILE, &nv, sizeof(nv));
    if (HI_ERR_SUCCESS != ret)
    {
        return HI_ERR_FAILURE;
    }

    (hi_void)memcpy_s(nv.CLTAddress, sizeof(nv.CLTAddress), frame->ucDataRealm + offset, sizeof(nv.CLTAddress));

    ret = HI_MDM_NV_Write(HI_NV_FTM_COLLECT_PARAM_FILE, &nv, sizeof(nv));
    if (HI_ERR_SUCCESS != ret)
    {
        return HI_ERR_FAILURE;
    }
    
    mrsSetCollectorAddr(nv.CLTAddress);

    return mrsAssembleFrame(ppOutParam, pOutParamLen, HI_NULL, 0, frame, ucPreSize);
}
/* END:   PN: DTS2015042907048 MODIFY\ADD\DEL by cuiate/00233580 at 2015/4/29 */


/* BEGIN: PN: DTS2015042907048 MODIFY\ADD\DEL by cuiate/00233580 at 2015/4/29 */
// Ӳ����ʼ��
HI_U32 mrsIr645_14H_DI04A00401(HI_VOID *param, HI_VOID **ppOutParam, HI_U16 *pOutParamLen, HI_U8 ucPreSize)
{
    HI_U32 ret = HI_ERR_SUCCESS;
    MRS_COLLECTOR_645_FRAME_STRU* frame = (MRS_COLLECTOR_645_FRAME_STRU*)param;
    static HI_U32 receive = 0;

    if ((HI_NULL == param) 
        || !MRS_IRDA_CTRL14_PARAM_VALID(frame->ucDataRealmLen, MRS_645_FRAME_DATA_DI_SIZE))
    {
        return HI_ERR_FAILURE;
    }

    ret = mrsAssembleFrame(ppOutParam, pOutParamLen, HI_NULL, 0, frame, ucPreSize);
    if (ret == HI_ERR_SUCCESS)
    {
        //����ִ��Ӳ����ʼ����ʱ��
        if (receive != 0)
        {
            return ret;
        }
        
        MRS_StartTimer(MRS_COLLECTOR_TIMER_IRDA_RESET, MRS_COLLECTOR_TIME_IRDA_RESET, HI_SYS_TIMER_ONESHOT);
        receive = 1;
    }

    return ret;
}
/* END:   PN: DTS2015042907048 MODIFY\ADD\DEL by cuiate/00233580 at 2015/4/29 */


/* BEGIN: PN: DTS2015042907048 MODIFY\ADD\DEL by cuiate/00233580 at 2015/4/29 */
// ���ݳ�ʼ��
HI_U32 mrsIr645_14H_DI04A00402(HI_VOID *param, HI_VOID **ppOutParam, HI_U16 *pOutParamLen, HI_U8 ucPreSize)
{
    MRS_COLLECTOR_645_FRAME_STRU* frame = (MRS_COLLECTOR_645_FRAME_STRU*)param;
    MRS_STA_SEARCH_CTX * pstSmCtx = mrsStaSearchCtx();
    HI_NV_FTM_COLLECT_PARAM_FILE_S nv;
    HI_U32 ret = HI_ERR_SUCCESS;

    if ((HI_NULL == param) 
        || !MRS_IRDA_CTRL14_PARAM_VALID(frame->ucDataRealmLen, MRS_645_FRAME_DATA_DI_SIZE))
    {
        return HI_ERR_FAILURE;
    }

    // 1.����¼���¼
    {
        (hi_void)memset_s(&(pstSmCtx->stEvent), sizeof(pstSmCtx->stEvent), 0, sizeof(pstSmCtx->stEvent));
    }

    // 2.�ն˸�λ��������
    {
        (hi_void)memset_s(&nv, sizeof(nv), 0, sizeof(nv));
        ret = HI_MDM_NV_Read(HI_NV_FTM_COLLECT_PARAM_FILE, &nv, sizeof(nv));
        if (HI_ERR_SUCCESS != ret)
        {
            return HI_ERR_FAILURE;
        }

        nv.reset_times = 0;
     
        ret = HI_MDM_NV_Write(HI_NV_FTM_COLLECT_PARAM_FILE, &nv, sizeof(nv));
        if (HI_ERR_SUCCESS != ret)
        {
            return HI_ERR_FAILURE;
        }
    }

    // 3.�ѱ������
    mrsStaSearchClearMeterList(pstSmCtx);

    return mrsAssembleFrame(ppOutParam, pOutParamLen, HI_NULL, 0, frame, ucPreSize);
}
/* END:   PN: DTS2015042907048 MODIFY\ADD\DEL by cuiate/00233580 at 2015/4/29 */


/* BEGIN: PN: DTS2015042907048 MODIFY\ADD\DEL by cuiate/00233580 at 2015/4/29 */
// ������ʼ��
HI_U32 mrsIr645_14H_DI04A00403(HI_VOID *param, HI_VOID **ppOutParam, HI_U16 *pOutParamLen, HI_U8 ucPreSize)
{
    HI_U32 ret = HI_ERR_SUCCESS;
    MRS_COLLECTOR_645_FRAME_STRU* frame = (MRS_COLLECTOR_645_FRAME_STRU*)param;
    HI_NV_FTM_COLLECT_PARAM_FILE_S nv;

    if ((HI_NULL == param) 
        || !MRS_IRDA_CTRL14_PARAM_VALID(frame->ucDataRealmLen, MRS_645_FRAME_DATA_DI_SIZE))
    {
        return HI_ERR_FAILURE;
    }

    (hi_void)memset_s(&nv, sizeof(nv), 0, sizeof(nv));
    ret = HI_MDM_NV_Read(HI_NV_FTM_COLLECT_PARAM_FILE, &nv, sizeof(nv));
    if (HI_ERR_SUCCESS != ret)
    {
        return HI_ERR_FAILURE;
    }

    (hi_void)memset_s(nv.CCTAddress, sizeof(nv.CCTAddress), 0xFF, sizeof(nv.CCTAddress));
    nv.CCT_register = 0;

    ret = HI_MDM_NV_Write(HI_NV_FTM_COLLECT_PARAM_FILE, &nv, sizeof(nv));
    if (HI_ERR_SUCCESS != ret)
    {
        return HI_ERR_FAILURE;
    }

    return mrsAssembleFrame(ppOutParam, pOutParamLen, HI_NULL, 0, frame, ucPreSize);
}
/* END:   PN: DTS2015042907048 MODIFY\ADD\DEL by cuiate/00233580 at 2015/4/29 */

//*****************************************************************************
// ��������: mrsIr645_14H_DI04A00503
// ��������: �����ն�����ʱ��
//           
// ����˵��:
//    ��
//                            
// �� �� ֵ: 
//    �ɹ�����  HI_ERR_SUCCESS
//
// ����Ҫ��: 
// ���þ���: 
// ��    ��: fengxiaomin/f00209182 [2015-10-30]
//*****************************************************************************
HI_U32 mrsIr645_14H_DI04A00503(HI_VOID *param, HI_VOID **ppOutParam, HI_U16 *pOutParamLen, HI_U8 ucPreSize)
{
    HI_UNREF_PARAM(param);
    HI_UNREF_PARAM(ppOutParam);
    HI_UNREF_PARAM(pOutParamLen);
    HI_UNREF_PARAM(ucPreSize);
	return HI_ERR_NOT_SUPPORT;
}

/* BEGIN: PN: DTS2015102007386  MODIFY\ADD\DEL by cuichunyan/00276962 at 2015/10/21 */
HI_U32 mrsCreate645SpecialTransmitUp(HI_PBYTE *ppFrame, HI_PBYTE pbuf, HI_U8 pbuflen,HI_U16 *pusLength)
{
    MRS_PROTO645_FRAME_INF stFrameInf;
    HI_U8 CLTAddress[HI_METER_ADDR_LEN] = {0};

    mrsIrDAGetCltAddr(CLTAddress);
    (hi_void)memset_s(&stFrameInf, sizeof(stFrameInf), 0, sizeof(stFrameInf));
    //�ɼ�����ַ
    (hi_void)memcpy_s(stFrameInf.aucAddr, HI_METER_ADDR_LEN, CLTAddress, HI_METER_ADDR_LEN);
    stFrameInf.ucCtrl = MRS_645_FRAME_CONTROL_DIR_UP|MRS_645_FRAME_CONTROL_READ_DATA;
    stFrameInf.ucDiSize = MRS_645_FRAME_DATA_DI_SIZE;
    stFrameInf.ulDi = DI_04A09904;
    stFrameInf.pData = pbuf; 
    stFrameInf.ucDataLength = pbuflen;
       
    return mrsGen645Frame(&stFrameInf, ppFrame, pusLength);
    
}


HI_VOID mrsIrTransmitUp(HI_U8 subId,HI_PBYTE pucData, HI_U16 usDataSize)
{
    if (subId == MRS_IR_SUBID_SPECIAL_TRANSMIT)
    {
        HI_PBYTE pFrame = HI_NULL;
        HI_U16 usFrameLength = 0;
        
        if (mrsCreate645SpecialTransmitUp(&pFrame,pucData,(HI_U8)usDataSize, &usFrameLength) == HI_ERR_SUCCESS)
        {
            MRS_SendMrData(pFrame, usFrameLength, HI_DMS_CHL_UART_PORT_IR); 
        }
        
        
        mrsToolsFree(pFrame);
            
    }
    else// if (subId == MRS_IR_SUBID_TRANSMIT)
    {
        MRS_SendMrData(pucData, usDataSize, HI_DMS_CHL_UART_PORT_IR);
    }
}
/* END: PN: DTS2015102007386  MODIFY\ADD\DEL by cuichunyan/00276962 at 2015/10/21 */


#endif // STA

