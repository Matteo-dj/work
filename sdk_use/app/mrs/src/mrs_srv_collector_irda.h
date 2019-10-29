//*****************************************************************************
// 
//                  ��Ȩ���� (C), 1998-2011, ��Ϊ�������޹�˾
// 
//*****************************************************************************
//  �� �� ��   : mrs_srv_collector_irda.h
//  �� �� ��   : V2.0 
//  ��    ��   : fengxiaomin/f00209182
//  ��������   : 2012-08-28
//  ��������   : TODO: ...
//               
//  �����б�   : TODO: ...
//  �޸���ʷ   : 
//  1.��    �� : 2012-08-28
//    ��    �� : fengxiaomin/f00209182
//    �޸����� : �����ļ� 
// 
//*****************************************************************************
#ifndef __MRS_SRV_COLLECTOR_IRDA_H__
#define __MRS_SRV_COLLECTOR_IRDA_H__


HI_START_HEADER

#if defined(PRODUCT_CFG_PRODUCT_TYPE_STA)

#define MRS_645_MIN_LENGTH              12 // 645֡��С����
#define MRS_645_CONTROL_FOLLOWBIT_ONE   0x20
#define MRS_SRV_645_DATA_INDEX_SIZE     4


#define MRS_IR_ERR_BUSY             (0x01)
#define MRS_IR_ERR_NOT_EXIST        (0x01)
#define MRS_IR_ERR_NOT_SUPPORT      (0x01)


/* BEGIN: PN:  MODIFY\ADD\DEL by cuichunyan/00276962 at 2015/10/08 */

#define MRS_CLTII_IR_TRANSMIT_TYPE_DISABLE          (0)
#define MRS_CLTII_IR_TRANSMIT_TYPE_ENABLE           (1)
#define MRS_CLTII_IR_TRANSMIT_TYPE_SPECIAL          (2)
/* END:   PN:  MODIFY\ADD\DEL by cuichunyan/00276962 at 2015/10/08 */

#define MRS_CLTII_EXTEND_VER_LEN  (6)
#define MRS_CLTII_IR_FROM         (0)
#define MRS_CLTII_TRANSFER_FROM   (1)

typedef struct
{
    HI_U8 ucAddr[6];           // ���ַ���̶�6�ֽ� 
    HI_U8 ucCtrl; // ������
    HI_U8 ucDataRealmLen; // �������С����С��200�ֽ�, 0��ʾ��������
    HI_U8 ucDataRealm[200]; // ������ 
}MRS_COLLECTOR_645_FRAME_STRU;

//���ݱ�ʶ�봦����
typedef HI_U32 (*MRS_IR_645_DI_FUN)(HI_VOID * param, HI_PVOID *ppOutParam, HI_U16 *pOutParamLen, HI_U8 ucPreSize);

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
HI_U32 mrsCollectorIrDARx(HI_DMS_CHL_RX_S* pstRxData);


HI_VOID mrsIrTransmitUp(HI_U8 subId,HI_PBYTE pucData, HI_U16 usDataSize);
HI_U32 mrsIrSendQueue(HI_U8 ucSubId,HI_PBYTE pucDatagram,HI_U8 ucDataRealmLen,HI_U8 ucBaudRateIndex);
HI_U32 mrsIrGetTransmit(MRS_COLLECTOR_645_FRAME_STRU * pst645Frame);
HI_U32 mrsCreate645SpecialTransmitUp(HI_PBYTE *ppFrame, HI_PBYTE pbuf, HI_U8 pbuflen,HI_U16 *pusLength);
HI_U32 mrsIrDA645Dec(HI_U8* pucData, HI_U16 usDataSize, MRS_COLLECTOR_645_FRAME_STRU *pst645Frame, HI_U8 ucFrom);
MRS_IR_645_DI_FUN mrsIrFrameProc(HI_U8 ucCtrl, HI_U8 *pDataRealm);
HI_U32 mrsAssembleDFrame(HI_VOID **ppFrame, HI_U16 *pFrmLen, HI_U8 ucErrCode, MRS_COLLECTOR_645_FRAME_STRU *p645Frame, HI_U8 ucPreSize);
HI_U32 mrsIrDACheckAddr(HI_U8 aucAddr[], HI_U8 ucFrom);
HI_U32 mrsIrDAGetDIFunc(HI_U8 ucCtrl, HI_U8 *pDataRealm, HI_U32 *pFunc);
HI_VOID mrsIrDAGetCltAddr(HI_U8 *pAddr);

#endif

HI_END_HEADER

#endif //__MRS_SRV_CCO_ANY_TIME_H__
