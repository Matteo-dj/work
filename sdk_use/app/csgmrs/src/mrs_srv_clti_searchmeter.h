//*****************************************************************************
//
//                  ��Ȩ���� (C), 1998-2014, ��Ϊ�������޹�˾
//
//*****************************************************************************
//  �� �� ��   : mrs_srv_clti_searchmeter.h
//  �� �� ��   : V1.0 
//  ��    ��   : liupeidong/00238134
//  ��������   : 2014-03-18
//  ��������   : I�Ͳɼ����ѱ���غ������ӿڶ���
//               
//  �����б�   : TODO: ...
//  �޸���ʷ   : 
//  1.��    �� : 2014-03-18
//    ��    �� : liupeidong/00238134
//    �޸����� : �����ļ� 
//
//*****************************************************************************
#ifndef __MRS_SRV_CLTI_SEARCHMETER_H__
#define __MRS_SRV_CLTI_SEARCHMETER_H__
HI_START_HEADER
#if defined(PRODUCT_CFG_PRODUCT_TYPE_STA)

#define MRS_SEC_TO_MSEC                       (1000)         // ��->����

#define MRS_645_FRAME_DI_LEN_V97              (2)
#define MRS_645_FRAME_DI_LEN_V07              (4)
#define MRS_CLTI_QUERY_RX_DATA_INFO_LEN       (7)            // I���ѱ���Ӧ��֡��������(DI+�ѱ�״̬+������+��֡����)�ĳ���
#define MRS_CLTI_QUERY_RX_ADDR_PROTO_LEN      (7)            // I���ѱ���Ӧ��֡��������ÿ�������Լ�ĳ���

#define MRS_CLTI_REPORT_POS_STATUS            (4)            // �ѱ����ϱ�֡�������±�: �ѱ�״̬
#define MRS_CLTI_REPORT_POS_TOTAL             (5)            // �ѱ����ϱ�֡�������±�: �ѱ�����
#define MRS_CLTI_REPORT_POS_THIS              (6)            // �ѱ����ϱ�֡�������±�: ��֡�ϱ�����
#define MRS_CLTI_REPORT_POS_LIST              (7)            // �ѱ����ϱ�֡�������±�: �ϱ����͹�Լ�б�

#define MRS_DI_POSITIVE_A_V97                 (0x9010)       // VER97 �������й�����(+A)
#define MRS_DI_POSITIVE_A_V07                 (0x00010000)   // VER07 �������й�����(+A)
#define MRS_DI_CLTI_SM_START                  (0x0B0A0B0A)   // ��ʼ�ѱ�
#define MRS_DI_CLTI_SM_STOP                   (0x0B0C0B0C)   // ֹͣ�ѱ�
#define MRS_DI_CLTI_SM_QUERY                  (0x0B0D0B0D)   // ��ѯ�ѱ���

#define MRS_CLTI_SM_FINISH                    (0x00)         // I���ѱ����
#define MRS_CLTI_SM_IN_PROGRESS               (0x01)         // I���ѱ������

#define MRS_CLTI_SM_REPORT_MAX                (16)           // I���ѱ���ÿ֡�ϱ������������

#define MRS_CLTI_FRAME_TYPE_SM_START          (1)            // I���ѱ�֡����: ��ʼ�ѱ�
#define MRS_CLTI_FRAME_TYPE_SM_STOP           (2)            // I���ѱ�֡����: ֹͣ�ѱ�
#define MRS_CLTI_FRAME_TYPE_SM_QUERY_FIRST    (3)            // I���ѱ�֡����: ��ѯ�ѱ��� ��1֡
#define MRS_CLTI_FRAME_TYPE_SM_QUERY_FOLLOW   (4)            // I���ѱ�֡����: ��ѯ�ѱ��� ����֡
#define MRS_CLTI_FRAME_TYPE_POSITIVE_A        (5)            // I���ϵ糢���ϴ�������ַ: �������й�����(+A)

#define MRS_CLTI_SEND_BYTE_TIME_2400          (5)            // ����ÿ���ֽڵ���ʱ
 
#define MRS_CLTI_SM_START_TRY_MAX             (2)            // ��ʼ��ѯ֡���Դ���
#define MRS_CLTI_SM_STOP_TRY_MAX              (2)            // ֹͣ��ѯ֡���Դ���
#define MRS_CLTI_SM_QUERY_FOLLOW_TRY_MAX      (3)            // ��ѯ����֡�����Դ�������ʱ�ѱ���ֹͣ     

#define MRS_CLTI_SET_VER_ADDR_OFFSET          (MRS_PREFIX_FE_SIZE + MRS_645_FRAME_1ST_HEAD_LEN)
#define MRS_CLTI_SET_SOFTWARE_VER_OFFSET      (18)
#define MRS_CLTI_SET_BOOT_VER_OFFSET          (20)
#define MRS_CLTI_SET_VER_DI                   (0x0B0A0C00)
#define MRS_CLTI_VER_INFO_LEN                 (3)

MRS_CLTI_SM_QUERY_CTX* mrsCltiGetQueryCtx(HI_VOID);
HI_VOID mrsCltiSetQueryCtx(HI_U8, HI_U8, HI_U8);
HI_VOID mrsCltiResetSmRunning(HI_VOID);
HI_U32 mrsCltiRx1300Proc(HI_U8* pucAddr, MRS_645_CTRL_STRU *pstCtrl);
HI_VOID mrsStaSetCltiType(HI_BOOL);
HI_BOOL mrsIsRealClti(HI_VOID);
HI_VOID mrsCltiWriteCLTAddress2Nv(HI_U8*);
HI_BOOL mrsCltiGetValidMeterInfo(MRS_STA_SEARCH_CTX *pstCtx);
HI_U32 mrsCltiTryGetAddrForMAC(HI_VOID);
HI_U32 mrsCltiTryReadPositiveATx_V07(HI_U8*);
HI_U32 mrsCltiTryReadPositiveATx_V97(HI_U8*, HI_U16);
HI_VOID mrsCltiTryReadPositiveATx(HI_VOID);

HI_U32 mrsCltiFrameEnQueue(HI_U8*, HI_U16, HI_U8);
HI_VOID mrsCltiFrameSendPrepare(MRS_STA_ITEM * pstItem);
HI_U32 mrsCltiFrameTimeoutProc(HI_VOID);

HI_U32 mrsCltiSmStartTx(HI_VOID);
HI_U32 mrsCltiSmStopTx(HI_VOID);
HI_U32 mrsCltiSmQueryTx(HI_U8, HI_U8, HI_U8);
HI_U32 mrsCltiSmQueryTxRetry(HI_VOID);

HI_VOID mrsCltiTryReadPositiveARx(MRS_645_FRAME_STRU*);
HI_VOID mrsCltiSmFrameRx(MRS_645_FRAME_STRU*);
HI_U32 mrsCltiSmStartRx(MRS_645_FRAME_STRU*);
HI_U32 mrsCltiSmStopRx(MRS_645_FRAME_STRU*);
HI_U32 mrsCltiSmQueryRx(MRS_645_FRAME_STRU *pst645Hrd);
HI_BOOL mrsCheckCltiSmQueryRxData(HI_U8*, HI_U8);
HI_U32 mrsCltiSaveSmResult(HI_U8*);
HI_U32 mrsCltiSmStopCmd(MRS_STA_SEARCH_CTX *pstCtx);
HI_U32 mrsCltiSearchFinish(HI_U8* pucData);

HI_U32 mrsCltiSearchStart(MRS_STA_SEARCH_CTX *pstCtx, HI_BOOL bTryFirstFlag);

HI_U32 mrsCltiSearchStop(MRS_STA_SEARCH_CTX *pstCtx);

HI_U32 mrsCltiSearchMeterInsert(MRS_STA_SEARCH_CTX *pstCtx, HI_U8 *pucMeter, HI_U8 ucProtocol);


#endif // defined(PRODUCT_CFG_PRODUCT_TYPE_STA)
HI_END_HEADER
#endif // __MRS_SRV_CLTI_SEARCHMETER_H__

