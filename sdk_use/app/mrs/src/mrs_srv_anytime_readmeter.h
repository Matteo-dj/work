//*****************************************************************************
//
//                  ��Ȩ���� (C), 1998-2011, ��Ϊ�������޹�˾
//
//*****************************************************************************
//  �� �� ��   : mrs_srv_anytime_readmeter.h
//  �� �� ��   : V1.0 
//  ��    ��   : liuxipeng/KF54842
//  ��������   : 2012-06-09
//  ��������   : ����ģ�麯�����ӿڶ���
//               
//  �����б�   : TODO: ...
//  �޸���ʷ   : 
//  1.��    �� : 2012-06-09
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

#ifndef __MRS_SRV_CCO_ANY_TIME_H__
#define __MRS_SRV_CCO_ANY_TIME_H__


HI_START_HEADER

/* ���ⵥ��: DTS2014010608635 BEGIN: Added by liuxipeng/lkf54842, 2014/01/07 */
// �㳭����֡��Դ 
#define	 XR_FROM_AFN02_F1  0 // ������ "ת��ͨѶЭ������֡"
#define	 XR_FROM_AFN13_F1  1 // ������ "����ز��ӽڵ�"
#define	 XR_FROM_NDM_AFN13_F1  2 // ������ "��Դ��NDM��PLC֡�����ļ���ز��ӽڵ�"
/* ���ⵥ��: DTS2014010608635 END:   Added by liuxipeng/lkf54842, 2014/01/07 */

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
HI_VOID mrsAnyTimeReadMeterInit(HI_VOID);

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
//*****************************************************************************
HI_VOID mrsAnyTimeReadMeterPlcTimeOutProcess(HI_IN HI_OUT MRS_CCO_SRV_CTX_STRU * pstCcoCtx);

//*****************************************************************************
// ��������: mrsAnyTimeReadMeterServiceEndProcess
// ��������: �泭������������ʱ����ʱ����
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
HI_VOID mrsAnyTimeReadMeterServiceEndProcess(HI_IN HI_OUT MRS_CCO_SRV_CTX_STRU * pstCcoCtx);
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
HI_U32 mrsAnyTimeReadMeterReceivePlcProcess(HI_IN MRS_DATA_TRANSMIT_AFN13_UP_STRU * pstData, HI_U8 ucMacAdd[HI_PLC_MAC_ADDR_LEN]);
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
HI_U32 mrsAnyTimeReadMeterPlcChlStatusProcess(HI_IN HI_OUT MRS_CCO_SRV_CTX_STRU * pstCcoCtx);

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
                                                    HI_IN HI_U8 * pDataBuff, HI_IN HI_U16 usDataLen);

HI_U32 mrsAnyTimeReadMeterProcessDelay(HI_IN HI_U8 * pDataBuff, HI_IN HI_U16 usDataLen);

HI_VOID mrsSetXRStatus(MRS_ANYTIME_RM_STATE_ENUM state);

HI_U32 mrsAFN13F1CreateAckFrame(HI_U8 **ppBuf, HI_U16 *pLen, HI_U8 aucMacAdd[HI_PLC_MAC_ADDR_LEN]);

HI_VOID mrsAnyTimeReadMeterFinishProc(HI_VOID);

HI_VOID mrsAnyTimeReadMeterServiceTimeout(MRS_CCO_SRV_CTX_STRU * pstCcoCtx);

HI_VOID mrsAnyTimeReadMeterFailEndProc(MRS_CCO_SRV_CTX_STRU * pstCcoCtx);

HI_U32 mrsCreateReadMeterFrame376_2(HI_IN HI_U8 ucProtocol, HI_IN HI_U8 * pDataBuff, HI_IN HI_U16 usDataLen,
                                                  HI_OUT HI_PBYTE * ppOutBuff, HI_OUT HI_U16 * usOutLen, HI_U8 ucMacAdd[HI_PLC_MAC_ADDR_LEN]);

HI_END_HEADER

#endif //__MRS_SRV_CCO_ANY_TIME_H__

