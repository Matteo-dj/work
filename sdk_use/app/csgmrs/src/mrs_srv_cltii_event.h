//*****************************************************************************
//
//                  ��Ȩ���� (C), 1998-2015, ��Ϊ�������޹�˾
//
//*****************************************************************************
//  �� �� ��   : mrs_srv_cltii_event.h
//  �� �� ��   : V1.0 
//  ��    ��   : cuiate/00233580
//  ��������   : 2015-07-17
//  ��������   : II���¹ҵ���¼� �������ӿ�����
//               
//  �����б�   : TODO: ...
//  �޸���ʷ   : 
//  1.��    �� : 2015-07-17
//    ��    �� : cuiate/00233580
//    �޸����� : �����ļ� 
//
//*****************************************************************************

#ifndef _MRS_SRV_CLTII_EVENT_H_
#define _MRS_SRV_CLTII_EVENT_H_
HI_START_HEADER
#if defined(PRODUCT_CFG_PRODUCT_TYPE_STA)

#define MRS_CLTII_EVT_MONITOR_INTERVAL_DEFAULT  (3600)
#define MRS_CLTII_EVT_MONITOR_INTERVAL_MIN      (120)
#define MRS_CLTII_EVT_MONITOR_FLAG_DEFAULT      (0xF7)

#define MRS_CLTII_EVENT_ITEM_MAX                (4)
#define MRS_CLTII_EVENT_NV_ITEM_VALID           (0x80)
#define MRS_CLTII_EVENT_NV_PROTO_MASK           (0x0F)

#define MRS_CLTII_EVT_REPORT_INFO_NUM_MAX       (8)


typedef enum
{
    MRS_CLTII_EVENT_MSG_START = 0,
    MRS_CLTII_EVENT_MSG_STOP,
    MRS_CLTII_EVENT_MSG_NEXT_ITEM,
    MRS_CLTII_EVENT_MSG_FINISH,
} MRS_CLTII_EVENT_MSG_E;


typedef struct
{
    HI_U8 aucMeterAddr[HI_METER_ADDR_LEN];
    HI_U8 ucProtocol;
    HI_U8 ucReserved;
    HI_U32 aulEvent[3];
} MRS_CLTII_EVENT_NV_METER_INF;


typedef struct
{
    HI_U16 usMonInterval;
    HI_U8  ucMonFlag;
    HI_U8  ucMeterNum;
    MRS_CLTII_EVENT_NV_METER_INF astMeterList[MRS_MAX_METERNUM];
} MRS_CLTII_EVENT_NV_INF;


typedef struct
{
    HI_BOOL bEnableEvent[MRS_CLTII_EVENT_ITEM_MAX];
    HI_U16 ausDiList[MRS_CLTII_EVENT_ITEM_MAX];
} MRS_CLTII_EVENT_MON_INF_97;


typedef struct
{
    HI_BOOL bEnableEvent[MRS_CLTII_EVENT_ITEM_MAX];
    HI_U32 aulDiList[MRS_CLTII_EVENT_ITEM_MAX];
} MRS_CLTII_EVENT_MON_INF_07;


typedef struct 
{
    MRS_CLTII_EVENT_MON_INF_97 stMonInf97; 
    MRS_CLTII_EVENT_MON_INF_07 stMonInf07; 
} MRS_CLTII_EVENT_MON_CFG;


typedef struct
{
    HI_U32 aulRecordInf[3]; // �¼���¼(97��:�¼�1~3; 07��:�¼�2~4)
} MRS_CLTII_EVENT_RECORD_INF;


typedef struct
{
    HI_U8 aucAddr[HI_METER_ADDR_LEN];
    HI_U8 ucProtocol;
    HI_U8 ucOption;     // 97��: �¼�4��¼ 
                        // 07��: D0: ֧�������ϱ���ʶ
                        //       D1: �����ϱ���ʶ(1:���¼�)
                        //       D2~D7: 000000b
    MRS_CLTII_EVENT_RECORD_INF stRecordInf;
} MRS_CLTII_EVENT_METER_INF;


typedef struct
{
    HI_U8 ucMeterTotal;     // �������
    HI_U8 aucReserved[2];
    HI_U8 ucEventCount;     // ��ص������¼��ĵ������
    HI_U8 aucEventInf[MRS_MAX_METERNUM];    // ����¼���ʶ��Ϣ��ÿ�����ռһ���ֽڣ���λ��ʾ�¼���Ϣ
                                            // D0: �¼�1 0-�� 1-��
                                            // D1: �¼�2 0-�� 1-��
                                            // D2: �¼�3 0-�� 1-��
                                            // D3: �¼�4 0-�� 1-��
                                            // D4~D7: 0000b
    MRS_CLTII_EVENT_METER_INF astMeterInf[MRS_MAX_METERNUM];
} MRS_CLTII_EVENT_METERLIST;


typedef enum
{
    MRS_CLTII_EVENT_MONITOR_IDLE,
    MRS_CLTII_EVENT_MONITOR_WORKING,
} MRS_CLTII_EVENT_MONITOR_STATUS_E;


typedef struct
{
    MRS_CLTII_EVENT_MONITOR_STATUS_E eMonStatus;  // ���ؼ���¼�״̬ 0-����, 1-���ڼ��
    HI_U8   ucCurrEvtIndex;  // ��ǰ�������¼���ʶ��Ϣ
    HI_U8   ucCltiiAddr[HI_METER_ADDR_LEN];  // �ɼ�����ַ--�ʲ����
    HI_U8   ucCurrReportNum;  // �����ϱ�������
    HI_U8   aucCurrReportIndex[MRS_CLTII_EVT_REPORT_INFO_NUM_MAX];  // �����ϱ��ı���Ϣ�ı�ʶ 
} MRS_CLTII_EVENT_PARAM;



typedef HI_U32 (*MRS_CLTII_EVENT_CMD_FUNC)(HI_U8 *pFrame, HI_U16 usFrameLength);

typedef struct
{
    HI_U32 ulDi;
    MRS_CLTII_EVENT_CMD_FUNC pFunc;
} MRS_CLTII_EVENT_CMD_OBJ;


typedef struct
{
    HI_U16 usCmdNum;
    HI_U16 usReserved;
    MRS_CLTII_EVENT_CMD_OBJ *pCmdList;
} MRS_CLTII_EVENT_CMD_HEAD;


typedef struct
{
    HI_U8 ucMeterIndex;
    HI_U8 ucDiIndex;
    HI_U8 aucReserved[2];

    MRS_CLTII_EVENT_CMD_HEAD stCmdHdr;
} MRS_CLTII_EVENT_RUNNING;


typedef struct
{
    MRS_CLTII_EVENT_NV_INF      stNvInf; 
    MRS_CLTII_EVENT_MON_CFG     stCfg;
    MRS_CLTII_EVENT_METERLIST   stMeterList;
    MRS_CLTII_EVENT_PARAM       stParam;
    MRS_CLTII_EVENT_RUNNING     stRun;
} MRS_CLTII_EVENT_CTX_STRU;


typedef struct
{
    HI_U8 aucMeterAddr[6];  // ����ַ
    HI_U8 ucOption;         // [0:3]: ����Լ���ͣ�1-97��Լ��2-07��Լ������-��Ч
                            // [4:6]: ����
                            // [7:7]: ��Ч�Ա�ʶ��1-��Ч��0-��Ч
    HI_U8 aucEventInf[9];   // �¼���Ϣ
                            // 97��Լ: BYTE[0-1] EVENT1; [2-3] EVENT2; [4-5] EVENT3; [6-8] ����
                            // 07��Լ: BYTE[0-2] EVENT2; [3-5] EVENT3; [6-8] EVENT4;
} NV_APP_CLTII_EVENT_ITEM;


// �ɼ����¼��洢ʹ�����ݴ洢NV��0x40C-0x40F���ṹ��С��ṹNV_APP_NVM_DATA_STRU��ͬ
// �ڱ�ģ�鴦��ʱ��ͳһʹ�ñ�ģ���Զ���ĽṹNV_APP_CLTII_EVENT_INF_STRU
typedef struct
{
    NV_APP_CLTII_EVENT_ITEM astEventItem[8];
} NV_APP_CLTII_EVENT_INF_STRU;



MRS_CLTII_EVENT_CTX_STRU * mrsGetCltiiEventCtx(HI_VOID);

HI_VOID mrsCltiiEventSetCltAddr(HI_U8 pucAddr[HI_METER_ADDR_LEN]);

HI_U32 mrsCltiiEventModuleInit(HI_VOID);

HI_U32 mrsCltiiEventModuleDeinit(HI_VOID);

HI_U32 mrsCltiiEventStartMonitor(HI_VOID);

HI_U32 mrsCltiiEventFrameRx(HI_U8 *pFrame, HI_U16 usFrameLength);

HI_U32 mrsCltiiEventFrameTimeout(HI_VOID);

HI_U32 mrsCltiiEventMsgProc(HI_SYS_QUEUE_MSG_S *pstMsg);

MRS_CLTII_EVENT_MONITOR_STATUS_E mrsCltiiEventGetMonitorStatus(HI_VOID);

HI_VOID mrsCltiiEventSetMonitorStatus(MRS_CLTII_EVENT_MONITOR_STATUS_E eStatus);

HI_U32 mrsCltiiEventReport(HI_VOID);

HI_U32 mrsCltiiEventInfoRxAckSaveNv(HI_VOID);

HI_U32 mrsCltiiEventPlcProc(HI_U8 *pData, HI_U16 usDataLen, HI_U16 usId, HI_U16 usSeq);

HI_U32 mrsCltiiEventMonitorFlagInit(HI_VOID);

HI_U32 mrsCltiiEventMonitorSendMessage(HI_U32 ulMsgId);


#endif // defined(PRODUCT_CFG_PRODUCT_TYPE_STA)
HI_END_HEADER
#endif //_MRS_SRV_CLTII_EVENT_H_

