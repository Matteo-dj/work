//*****************************************************************************
//
//                  ��Ȩ���� (C), 1998-2011, ��Ϊ�������޹�˾
//
//*****************************************************************************
//  �� �� ��   : mrs_srv_cco_readmeter.h
//  �� �� ��   : V1.0 
//  ��    ��   : liuxipeng/KF54842
//  ��������   : 2011-12-13
//  ��������   : ����ģ�麯�����ӿڶ���
//               
//  �����б�   : TODO: ...
//  �޸���ʷ   : 
//  1.��    �� : 2011-12-13
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




#ifndef __MRS_SRV_CCO_READMETER_H__
#define __MRS_SRV_CCO_READMETER_H__

HI_START_HEADER

//�����ֳ�����ģ��
#define mrsAcivReadMeter()  MRS_StartTimer(EN_MRS_SRV_CCO_READMETER_ACTIV_TIMER, 0, HI_SYS_TIMER_ONESHOT)

#define MRS_READ_FAILED_CCO 0x00    // ����ʧ��
#define MRS_READ_SUCCESS_CCO 0x01   // �����ɹ�
#define MRS_ALLOW_READ_CCO 0x02     // ���Գ���
#define MRS_READ_METER_FLAG_BIT  0x01   //������ʶΪ:��ȡFlag�ĵ�λ


//*****************************************************************************
// ��������: mrsReadMeterStateProcess
// ��������: ����״̬����
//           
// ����˵��:
//   pstCcoCtx [IN/OUT] ����״̬��Ϣ
//
// �� �� ֵ: 
//    HI_TRUE �ȴ�״̬
//    HI_FALSE ����ִ��
//
// ����Ҫ��: 
// ���þ���: 
// ��    ��: liuxipeng/KF54842 [2011-12-20]
//*****************************************************************************
HI_BOOL mrsReadMeterStateProcess(HI_INOUT MRS_CCO_SRV_CTX_STRU* pstCcoCtx);

//*****************************************************************************
// ��������: mrsReadterModelCco
// ��������: ����ģ�鴦����ں���
//           
// ����˵��:
//  This function has no arguments.
//
// �� �� ֵ: 
//
// ����Ҫ��: TODO: ...
// ���þ���: TODO: ...
// ��    ��: liuxipeng/KF54842 [2011-12-15]
//*****************************************************************************
HI_VOID mrsReadterModelCco(HI_VOID *);

//*****************************************************************************
// ��������: mrsCreatReadMeterQueueCco
// ��������: �����������(����������)
//           
// ����˵��:
//  This function has no arguments.
// �� �� ֵ: 
//    HI_ERR_SUCCESS �����ɹ�
//    HI_ERR_NOT_FOUND û���ҵ����
//    HI_ERR_NOT_ENOUGH_MEMORY �ڴ治��
// ����Ҫ��: TODO: ...
// ���þ���: TODO: ...
// ��    ��: liuxipeng/KF54842 [2011-12-14]
//*****************************************************************************
HI_U32 mrsCreatReadMeterQueueCco(HI_VOID);

//*****************************************************************************
// ��������: mrsClearReadMeterQueueCco
// ��������: �������������� 
//           
// ����˵��:
//  This function has no arguments.
//
// �� �� ֵ: 
//    ��
// ����Ҫ��: TODO: ...
// ���þ���: TODO: ...
// ��    ��: liuxipeng/KF54842 [2011-12-14]
//*****************************************************************************
HI_VOID mrsClearReadMeterQueueCco(HI_VOID);


//*****************************************************************************
// ��������: mrsInquireReadMeter
// ��������: ѯ���Ƿ񳭱�
//           
// ����˵��:
//  This function has no arguments.
//
// �� �� ֵ: 
//    ��
// ����Ҫ��: 
// ���þ���: 
// ��    ��: liuxipeng/KF54842 [2011-12-16]
//*****************************************************************************
HI_VOID mrsInquireReadMeter(HI_VOID);

//*****************************************************************************
// ��������: mrsAssembleReadMeterData
// ��������:��֯PLC��������֡
//           
// ����˵��:
//   ��
// �� �� ֵ: 
//    ��
// ����Ҫ��: TODO: ...
// ���þ���: TODO: ...
// ��    ��: liuxipeng/KF54842 [2011-12-16]
//*****************************************************************************
HI_VOID mrsAssembleReadMeterData(HI_VOID);

//*****************************************************************************
// ��������: mrsTransmit14F1FramePlc
// ��������: ת��PLC��������
//           
// ����˵��:
//   ��
// �� �� ֵ: 
//    ��
// ����Ҫ��: TODO: ...
// ���þ���: TODO: ...
// ��    ��: liuxipeng/KF54842 [2011-12-16]
//*****************************************************************************
HI_VOID mrsTransmit14F1FramePlc(HI_VOID);

//*****************************************************************************
// ��������: mrsUpReterMetreData
// ��������: �ϱ���������
//           
// ����˵��:
//    * pstUpReadMeter [in] ���ϴ��������ݵĽṹ
//
// �� �� ֵ: 
//    HI_ERR_SUCCESS �ϴ����ݳɹ�
// ����Ҫ��: TODO: ...
// ���þ���: TODO: ...
// ��    ��: liuxipeng/KF54842 [2011-12-17]
//*****************************************************************************
HI_VOID mrsUpReterMetreData(HI_VOID);

//*****************************************************************************
// ��������: mrsReadMeterStateInit
// ��������: ����ģ��״̬��ʼ��
//           
// ����˵��:
//    ��
// �� �� ֵ: 
//    ��
// ����Ҫ��: TODO: ...
// ���þ���: TODO: ...
// ��    ��: liuxipeng/KF54842 [2011-12-16]
//*****************************************************************************
HI_VOID mrsReadMeterStateInit(HI_VOID);

//*****************************************************************************
// ��������: mrsReadMeterStaTimerProcess
// ��������: ��������״̬��STAͨѶ��ʱ����ʱ����
//           
// ����˵��:
//   pstCcoCtx [in/out] ��ǰ״̬
//
// �� �� ֵ: 
//    ��
//
// ����Ҫ��: 
// ���þ���: 
// ��    ��: liuxipeng/KF54842 [2012-06-08]
//*****************************************************************************
HI_VOID mrsReadMeterStaTimerProcess( MRS_CCO_SRV_CTX_STRU* pstCcoCtx );

//*****************************************************************************
// ��������: mrsReadMeterMusterTimerProcess
// ��������: ��������״̬�¼�����ͨѶ��ʱ����ʱ����
//           
// ����˵��:
//   pstCcoCtx [in/out] ��ǰ״̬
//
// �� �� ֵ: 
//    ��
//
// ����Ҫ��: 
// ���þ���: 
// ��    ��: liuxipeng/KF54842 [2012-06-08]
//*****************************************************************************
HI_U32 mrsReadMeterMusterTimerProcess(HI_VOID*);

//*****************************************************************************
// ��������: mrsReadMeterActiveTimer
// ��������: ����ģ�鼤�ʱ������
//           
// ����˵��:
//   pstCcoCtx [in/out] ��ǰ״̬
//
// �� �� ֵ: 
//    ��
//
// ����Ҫ��: 
// ���þ���: 
// ��    ��: liuxipeng/KF54842 [2012-06-08]
//*****************************************************************************
HI_VOID mrsReadMeterActiveTimer( MRS_CCO_SRV_CTX_STRU* pstCcoCtx );


HI_U32 mrsReadMeterFirstRunProcess(MRS_CCO_SRV_CTX_STRU* pstCcoCtx);

HI_U32 mrsMeterInfChangeProcess(MRS_CCO_SRV_CTX_STRU* pstCcoCtx);

HI_VOID mrsWaitAskReplyProcess(MRS_CCO_SRV_CTX_STRU* pstCcoCtx, HI_BOOL *bLoopFlg);

HI_VOID mrsWaitPlcReplyProcess(MRS_CCO_SRV_CTX_STRU* pstCcoCtx, HI_BOOL *bLoopFlg);

HI_VOID mrsWaitUpDataAckProcess(MRS_CCO_SRV_CTX_STRU* pstCcoCtx, HI_BOOL *bLoopFlg);


//*****************************************************************************
// ��������: mrsReadMeterFindValidNode
// ��������: ������Ч�ڵ㣺������ҳɹ���ֱ�ӷ��أ����򽫵�ǰ����ڵ���Ϣ��Ϊ��
//           
// ����˵��:
//   pstCcoCtx [in/out] ��ǰ״̬
//
// �� �� ֵ: 
//   HI_ERR_SUCCESS ���ҵ���Ч�ڵ�
//   HI_ERR_NOT_FOUND û�в��ҵ���Ч�ڵ�
//   0xFFFFFFFF��ַӳ��������ȡʧ��
//
// ����Ҫ��: TODO: ...
// ���þ���: TODO: ...
// ��    ��: fengxiaomin/f00209182 [2012-05-26]
//*****************************************************************************
HI_U32 mrsReadMeterFindValidNode(MRS_CCO_SRV_CTX_STRU * pstCcoCtx);

//*****************************************************************************
// ��������: mrsCurrNodeValid
// ��������: �жϳ�������е�ǰ�ڵ��Ƿ���Ч
//
// ����˵��:
//
// �� �� ֵ:
//   HI_FALSE ��ǰ�ڵ���Ч
//   HI_TRUE ��ǰ��Ч�ڵ���Ч
//
// ����Ҫ��: TODO: ...
// ���þ���: TODO: ...
// ��    ��: fengxiaomin/f00209182 [2012-09-04]
//*****************************************************************************
HI_BOOL mrsCurrNodeValid(HI_VOID);
//*****************************************************************************
// ��������: mrsReadMeterReceivePlcProcess
// ��������: ���ն���PLC���ݴ���
//
// ����˵��:
//          pstData ת������������
//          pstCcoCtx ȫ��������
// �� �� ֵ:
//          �μ������б�
//
// ����Ҫ��: ��
// ���þ���: ��
// ��    ��: liuxipeng/lKF54842 [2012-09-17]
//*****************************************************************************
HI_U32 mrsReadMeterReceivePlcProcess(MRS_DATA_TRANSMIT_AFN14_UP_STRU * pstData,
                                                MRS_CCO_SRV_CTX_STRU * pstCcoCtx);

HI_VOID mrsSetReadMeterState(MRS_MRV_CCO_READMETER_STATE_ENUM state);

#if defined(PRODUCT_CFG_PRODUCT_TYPE_CCO_BROAD_BAND_UNIT)
HI_VOID mrsInitMrStateBbu(HI_VOID);
#endif

HI_U32 mrsReadMeterTotalMusterTimerProcess(HI_VOID *param);
HI_VOID mrsReadMeterResume(HI_VOID);

HI_END_HEADER
 


#endif //__MRS_SRV_CCO_READMETER_H__


