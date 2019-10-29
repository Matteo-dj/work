//*****************************************************************************
//
//                  ��Ȩ���� (C), 1998-2011, ��Ϊ�������޹�˾
//
//*****************************************************************************
//  �� �� ��   : mrs_srv_sta_event.h
//  �� �� ��   : V1.0
//  ��    ��   : cuiate/00233580
//  ��������   : 2013-03-20
//  ��������   : �¼��ϱ��������ӿڶ���
//
//  �����б�   : TODO: ...
//  �޸���ʷ   :
//  1.��    �� : 2013-03-20
//    ��    �� : cuiate/00233580
//    �޸����� : �����ļ�
//
//  2.��    �� : 2014-08-20
//    ��    �� : liuxipeng/lKF54842
//    �޸����� : �޸��ļ�
//
//*****************************************************************************

//*****************************************************************************
// PROJECT   :
// SUBSYSTEM :
// MODULE    :
// OWNER     :
//*****************************************************************************

#ifndef __MRS_SRV_STA_EVENT_H__
#define __MRS_SRV_STA_EVENT_H__
HI_START_HEADER

#if defined(PRODUCT_CFG_PRODUCT_TYPE_STA)
//*****************************************************************************
//*****************************************************************************

#define MRS_STA_EVT_FROM_SIMU    (0)    // �¼�����������ģ���
#define MRS_STA_EVT_FROM_METER   (1)    // �¼�������������ʵ��
#define MRS_STA_DETECT_POWER_FAILURE_ENABLE (1)     //STA�ܹ�����ͣ����


typedef enum
{
    MRS_STA_EVT_STATUS_IDLE,        // ����
    MRS_STA_EVT_STATUS_QUERY,       // ��ѯ�¼�
    MRS_STA_EVT_STATUS_REPORT,      // �ϱ��¼�
} MRS_STA_EVT_STATUS_E;

#define MRS_STA_EVT_MONITOR_DOING    (1)
#define MRS_STA_EVT_MONITOR_IDLE     (0)

#define MRS_STA_EVT_STATUS_WORD_FLAG_LEN   (12)
#define MRS_STA_EVT_STATUS_WORD_DI         (0x04001501)
#define MRS_STA_EVT_STATUS_WORD_LEN_MIN    (14)

#define MRS_STA_EVT_METER_TYPE_UNKNOWN    (0)
#define MRS_STA_EVT_METER_TYPE_13_OLD     (1)
#define MRS_STA_EVT_METER_TYPE_13_NEW     (2)
#define MRS_STA_EVT_METER_TYPE_09         (3)

typedef struct
{
    HI_U8  ucStatus;  //0:���ڼ��, 1:���ڼ��
    HI_U8  ucMonitorInterval;//��ؼ��,��λ:����
    HI_U8  bWaitPlcFlag; //�¼��ϱ�ʱPLCͨ�������÷�����־
    HI_U8  ucDataLen; //���Ӧ��������ϱ�״̬�ֱ����е������򳤶�
    HI_U8  *pData;    //���Ӧ��������ϱ�״̬�ֱ����е�����������
}MRS_STA_EVT_MONITOR_INF;

typedef struct
{
    HI_U8  ucMeterEvtType;        //0:Ĭ��ֵ  1:13�淶�����¼���  2:13�淶���¼���  3:09�淶���
    HI_U8  ucStaConsDenyTimes; //STA�����յ�������Ӧ������������ж�����Ƿ�Ϊ09�淶��
    HI_U8  ucConDentMaxTimes;  //STA�����յ�������Ӧ�����N���ж����Ϊ09�淶��
    HI_U8  padding;
}MRS_STA_EVT_FILTER_INF;


typedef enum
{
    MRS_CLTI_EVT_STATUS_IDLE = 0,
    MRS_CLTI_EVT_STATUS_WAIT_QUERY_REPLY,
    MRS_CLTI_EVT_STATUS_WAIT_PLC_REPLY,
} MRS_CLTI_EVT_STATUS_E;

typedef HI_U32 (*EVT_FRAME_RX)(HI_U8 *, HI_U16);
typedef HI_U32 (*EVT_FRAME_TIMEOUT)(HI_VOID);

typedef struct
{
    HI_U8 ucTotal;
    HI_U8 ucQuery;
    HI_U8 ucStatus;
    HI_U8 ucFrameLen;
    HI_U8 *pucFrame;
    EVT_FRAME_RX        pfnRx;
    EVT_FRAME_TIMEOUT   pfnTimeout;
} MRS_CLTI_EVT_INF;

typedef struct
{
    HI_BOOL bHardEvtOut;
    HI_BOOL bEvtOut;
    HI_U8  ucCltiEnable;
    HI_U8  padding;
    HI_U8  ucCltiiEnable;
    HI_U8  ucStaEnable;
    HI_U8  ucTimeout;
    HI_U8  ucMaxRetry;
    HI_U8  ucTimeOutMax;
    HI_U8  ucRetry;
    HI_U16 usSeq;
    MRS_PLC_FRAME_DATA_S *pstReportData;
    MRS_STA_EVT_MONITOR_INF stStaMonInf;
    MRS_STA_EVT_FILTER_INF  stFilterInf;
#if !defined(PRODUCT_CFG_PRODUCT_TYPE_CSG_GD)
    MRS_CLTI_EVT_INF        stCltiEvtInf;
#endif

#if defined(PRODUCT_CFG_PRODUCT_TYPE_CSG_GD)
    HI_U32 (*MrsStaRxRespProc)(HI_VOID *, HI_U16 usSize);
#endif

} MRS_STA_EVT_CTX, *P_MRS_STA_EVT_CTX;


// �¼������ĳ�ʼ��
HI_U32 mrsStaEvtCtxInit(HI_VOID);

HI_U32 mrsStaEvtCtxReset(HI_VOID);

HI_VOID mrsStaEvtSetMeterType(HI_U8 ucType);

HI_U8 mrsStaEvtGetMeterType(HI_VOID);

HI_VOID mrsStaEvtConsDenyTimesReset(HI_VOID);

// EVENT_OUT�ܽ�״̬����
HI_U32 mrsStaEvtOutStatusNotify(HI_U32, HI_BOOL);

// �¼��ϱ�����
HI_U32 mrsStaEvtReportProc(HI_U8 *, HI_U16 );

// �����¼��ϱ�PLC֡
HI_U32 mrsStaSendEvtReportPlc(HI_VOID);

// �¼��ϱ���ʱ��ʱ������
HI_U32 mrsStaEvtReportPlcTimeoutProc(HI_VOID);

// �¼�PLC����֡���մ���
HI_U32 mrsCmdStaEvtInfProc(HI_U16, HI_PVOID, HI_U16);

P_MRS_STA_EVT_CTX mrsStaGetEvtCtx(HI_VOID);

// �¼��ܽŴ���
HI_VOID mrsStaEvtOutProc(HI_VOID);

HI_BOOL mrsStaEvtSetEvtOut(HI_BOOL bEvtFlg);

HI_U32 mrsStaEvtMonTimeOutProc(HI_VOID);

HI_U32 mrsStaEvtProc(HI_U8 *pData, HI_U16 usLen, MRS_645_FRAME_STRU *pst645Hrd);

HI_VOID mrsStaEvtSetMonWaitPlcFlag(HI_BOOL bFlag);

HI_U32 mrsCltiEventFrameRx(HI_U8 *pFrame, HI_U16 usFrameLength, HI_U8 ucSubId);

HI_U32 mrsCltiEventFrameTimeout(HI_VOID);



#if defined(PRODUCT_CFG_PRODUCT_TYPE_CSG_GD)

HI_PUBLIC HI_U32 mrsStaEvtPlcAck(MRS_CSG_PLC_FRAME_HEAD *pstFrameHead);
HI_PUBLIC HI_U32 mrsStaEvtPlcDeny(MRS_CSG_PLC_FRAME_HEAD *pstFrameHead);
HI_PUBLIC HI_U32 mrsStaEvtPlcTimeout(HI_VOID);

#endif

//*****************************************************************************
//*****************************************************************************
#endif

HI_END_HEADER
#endif //__MRS_SRV_STA_EVENT_H__

