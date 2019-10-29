//*****************************************************************************
//
//                  ��Ȩ���� (C), 1998-2011, ��Ϊ�������޹�˾
//
//*****************************************************************************
//  �� �� ��   : mrs_srv_cco.h
//  �� �� ��   : V1.0
//  ��    ��   : liuxipeng/KF54842
//  ��������   : 2011-11-30
//  ��������   : TODO: ...
//
//  �����б�   : TODO: ...
//  �޸���ʷ   :
//  1.��    �� : 2011-11-30
//    ��    �� : liuxipeng/KF54842
//    �޸����� : �����ļ�
//
//*****************************************************************************
#ifndef __MRS_SERVICE_CCO_H__
#define __MRS_SERVICE_CCO_H__

#include "hi_mdm.h"
#include "mrs_srv_list.h"

HI_START_HEADER


#define MRS_CCO_READ_METER_TASK_END     1 //�����������
#define MRS_CCO_SEARCH_METER_TASK_END   2 //�ѱ��������

#define MRS_PARALLEL_NUM_CHECK_MODE     (7)     // �ͼ�ģʽ����󲢷���
//#define MRS_PARALLEL_NUM_MAX_DEFAULT    (5)     // Ĭ����󲢷���

#define MRS_CCO_ARCHIVES_STATUS_EFFECT     (0)
#define MRS_CCO_ARCHIVES_STATUS_WAITING    (1)

#define MRS_CCO_ARCHIVES_NUM_MINI          (7) //���������������ڸ�����ʱ,CCO�յ��������
#define MRS_LOW_POWER_AFE_METER_NUM_MAX    (2) //��cco�б�������������2ʱ�����ͷ��书��

typedef struct
{
    HI_BOOL bValid;         // ��Ч��־ HI_TRUE: ��Ч  HI_FALSE: ��Ч
    HI_U8   ucProtocol;     // ��Լ���� 00:����(δ֪) 01:97-645 02:07-645 ����:δ֪
    HI_U16  usNodeSeq;      // �����дӽڵ�(���)���
    HI_U8   aucNodeAddr[HI_METER_ADDR_LEN]; // �ӽڵ�(���)��ַ
    HI_U8   ucSave : 1;     // ���̱�־ 0:����Ҫ����  1:��Ҫ����
    HI_U8   ucRead : 2;     // ������ʶ 0:δ����  1:�ѳ���
    HI_U8   ucDetect : 1;    // ̽�Ⳮ��ǰ����״̬ 0:����Ҫ����  1:��Ҫ����
    HI_U8   ucRmTurn : 2;   // ̽�Ⳮ��ǰ��������
    HI_U8   ucReserved : 2;
    HI_U8   ucCS;           // У���
} MRS_ARCHIVES_NODE_STRU;  // �����ӽڵ�(���)��Ϣ�ṹ

typedef struct
{
    HI_S8 Try;              // ��ǰ���Դ���
    HI_S8 MaxTry;           // ������Դ���
    HI_S8 MaxTryNvS;        // ������Դ�������(����)
    HI_S8 MaxTryNvB;        // ������Դ�������(�㲥)
    HI_S32 OverTime;        // ��ʱʱ��
    HI_S32 LayTimeOutNv;    // �㼶��ʱʱ�����
    HI_S32 MeterTimeOutNv;  // ���ʱʱ�����
    HI_S32 RevisoryNv;      // CCO��PLC��ʱ����ʱ��
    HI_S32 CtrlCmdDelayNv;  // ��������������ʱ����(����)
    HI_S32 CltIDelayNv;     // I�Ͳɼ�����ʱ����
    HI_U8 IsCtrlCmdFlg;     // ���������־(����ǿ���������ȡֵΪ1)
    HI_U8 MaxTimeout;       // ���ʱʱ�� ��λ:��
    HI_S8 Loop;             // ��ǰ�ִ�
    HI_S8 MaxTryLoop;       // ��������ִ�
/* BEGIN: Modified by liuxipeng/lKF54842,2014/5/30 ���ⵥ��:DTS2014052908512 */
    HI_U16 MeterFrameInterval;  // STA��645֡���ʱ��
    HI_U8 reserved[2];
/* END  : Modified by liuxipeng/lKF54842,2014/5/30 ���ⵥ��:DTS2014052908512 */
}PLC_OVERTIME_INF_STRU;   // PLCͨ����ʱ�����Ϣ

typedef enum
{
    EN_MRS_SRV_CCO_READMETER_IDLE,   // ����
    EN_MRS_SRV_CCO_READMETER_ASK,    // ѯ��
    EN_MRS_SRV_CCO_READMETER_WAIT_ASK_REPLY,    // �ȴ�ѯ�ʵ�Ӧ��
    EN_MRS_SRV_CCO_READMETER_WAIT_REVISE_RESP,  // �ȴ�����ʱ��(14-F3)��Ӧ��
    EN_MRS_SRV_CCO_READMETER_SEND_PLC,          // ���Ͷ�������
    EN_MRS_SRV_CCO_READMETER_WAIT_PLC_REPLY,    // �ȴ���������Ӧ��
    EN_MRS_SRV_CCO_READMETER_REPEAT_PLC,        // �ط���������
    EN_MRS_SRV_CCO_READMETER_UPDATA,            // �ϴ���������(�ϴ���������)
    EN_MRS_SRV_CCO_READMETER_WAIT_UPDATA_ACK,   // �ȴ��ϴ���������ȷ��
    EN_MRS_SRV_CCO_READMETER_REPEAT_UPDATA,     // �ط��ϴ���������
    EN_MRS_SRV_CCO_READMETER_ERR,               // ����״̬
}MRS_MRV_CCO_READMETER_STATE_ENUM;  // ����ģ��״̬

typedef enum
{
    EN_MRS_SRV_CCO_METERINF_STATE_FIRST,     // �״��ϵ�
    EN_MRS_SRV_CCO_METERINF_STATE_NORMAL,    // ����
}MRS_MRV_CCO_METERINF_STATE_ENUM;  // ����Ϣ״̬

typedef struct
{
    HI_U16 usDataLen;
    HI_U8 ucData[0];
}UP_READMETER_DATA;

typedef struct
{
    MRS_MRV_CCO_READMETER_STATE_ENUM enCurrState;    // ����ģ��״̬
    MRS_MRV_CCO_METERINF_STATE_ENUM enMeterInfState;  // ����Ϣ״̬
    MRS_ARCHIVES_NODE_STRU stCurrNode;   // ��ǰ����Ľڵ�(����)
    MRS_PLC_FRAME_DATA_STRU *pstPlcDataDl; // ת����plc��������(CC0->STA)
    AFN_14_F1_DL_PARAM * pstAfn14F1;    // ��������ָ��(����)
    UP_READMETER_DATA * pstUpReadMeterData; // �ϴ���������(����)
    HI_U16 usMeterNum;
    HI_U16 usCurrNodeSeq; //����ģ���г����Ľڵ����
    PLC_OVERTIME_INF_STRU stOverTimeInf;    // PLCͨ����ʱʱ�������Ϣ
    HI_U32 ulTimeOutMaxNumNv;  // �ڵ����������ʱ����(Nv��)
    HI_U32 ulTimeOutCount;  // ��ǰ�ڵ�������������ʱ����
    HI_U32 ulTurnNumNv;     // ����ִ���(Nv��)
    HI_U32 ulCurrTurn;      // ��ǰ�ִ���
    HI_U16 seq;  //֡���
    HI_U16 usSuccNum;    //��Ϊ�����ɹ��Ľڵ�����
    HI_U16 usAfn06F2UpTimeOut;      //06F2���б��ĳ�ʱʱ��(Nv���λms)
    HI_U8  ucAfn06F2UpRetryMax;     //06F2���б��ĳ�ʱ���Դ���(Nv��)
    HI_U8  ucAfn14F1UpRetryMax;     //14F1���б��ĳ�ʱ���Դ���(Nv��)
    HI_U16 usAfn14F1UpTimeOut;      //14F1���б��ĳ�ʱʱ��(Nv���λms)
    HI_U8  ucCurrNodeInquireTries;  //���ִε�ǰ�ڵ���ѯ����
    HI_U8  ucRsv;
}MR_READ_METER_STRU;    // ����ģ��ṹ

typedef enum
{
    EN_MRS_ANYTIME_RM_IDLE = 0,         // ����״̬
    EN_MRS_ANYTIME_RM_WAIT_REVISE_RESP, // �ȴ���ʱ����Ӧ��״̬
    EN_MRS_ANYTIME_RM_REVISE_RESP_PROC,// ��ʱ����Ӧ����״̬
    EN_MRS_ANYTIME_RM_WAIT_CHLAVAIL,    // �ȴ�ͨ������״̬
    EN_MRS_ANYTIME_RM_WAIT_PLC,         // �ȴ�PLCӦ��״̬
    EN_MRS_ANYTIME_RM_PRE_RESUME,       // Ԥ���ָ�״̬(���ڴ�״̬��һ��ʱ��CCO״̬�ָ���EN_MRS_CCO_NORMAL)
}MRS_ANYTIME_RM_STATE_ENUM;     // �泭״̬

typedef struct
{
    MRS_ANYTIME_RM_STATE_ENUM enState;      // �泭״̬
    MRS_PLC_FRAME_DATA_STRU *pstPlcDataDl;  // ת����plc��������(CC0->STA)
    HI_U32 ulTimeOut;                       // �泭��ʱʱ��
    PLC_OVERTIME_INF_STRU stOverTimeInf;    // PLCͨ����ʱʱ�������Ϣ
#if defined(PRODUCT_CFG_PRODUCT_TYPE_CSG_GD)
    PLC_OVERTIME_INF_STRU stCsgOverTimeInf;    // �㶫PLCͨ����ʱʱ�������Ϣ
#endif
    HI_U16 seq;  //֡���
    HI_BOOL bIsBC;                          // �Ƿ�Ϊ�㲥ʽ����
    HI_U8 ucProtocol;
    HI_U8 seq_1376_2;
    HI_U8 from;         // �㳭����֡��Դ
                        // XR_FROM_AFN02_F1 (ת��ͨѶЭ������֡)
                        // XR_FROM_AFN13_F1 (����ز��ӽڵ�)
    HI_U8 aucMeterAddr[HI_METER_ADDR_LEN];
}MR_ANYTIME_READMETER_STRU;    // �泭����ģ��

typedef struct
{
    HI_U8  bc_send_interval;
    HI_U8  bc_loop_max;

    HI_U8  topo_query_interval;
    HI_U8  topo_stable_times;

    HI_U8  query_max_way;
    HI_U8  query_interval;
    HI_U8  query_force_times;
    HI_U8  query_except_times;

    HI_U8  search_finish_reason;
/* BEGIN: PN: DTS2015051601564 MODIFY\ADD\DEL by cuiate/00233580 at 2015/5/11 */
    HI_U8  report_AA_2_master;
    HI_U16 usAfn06F4UpTimeOut;       // �ѱ�06F4�ϱ���ʱʱ�䣬��λ��ms
    HI_U8  ucAfn06F4UpRetryMax;       // �ѱ�06F4�ϱ�������Դ���
    HI_U8  ucBindCmdSendInterVal;   // �������ķ��ͼ��
    HI_U8  ucBindCmdSendMaxWay;     // ��������һ����󷢼�֡
    HI_U8  ucBindCmdSendMaxTimes;   // ������������ʹ���

    HI_U8  ucRecoverWhiteSwitch;    // �ѱ����ʱ���Ƿ�ָ��ѱ�ǰ������״̬
    HI_U8  ucCloseWhiteWhenSM;      // �ѱ�����Ƿ�رհ�����
    HI_U16 usSMDurationDefault;     // �ѱ�Ĭ��ʱ������λmin

    HI_U32  seq;
} MRS_SEARCH_METER_CFG_S;

typedef struct
{
    MRS_PLC_FRAME_DATA_STRU *plc;

    HI_U8   send_retry;
    HI_U8   retry_max;
    HI_U16  reserved;
} MRS_SEARCH_METER_FRAME_S;


typedef struct
{
    MRS_SRV_NODE link;
    HI_U8   mac[HI_PLC_MAC_ADDR_LEN];
    HI_U8   asset[HI_METER_ADDR_LEN];
    HI_U8   id[HI_PLC_MAC_ADDR_LEN];
    HI_U8   type;
    HI_U8   num;
    HI_U8   item[0];
} METER_NODE;


typedef struct
{
    HI_U8   mac[HI_PLC_MAC_ADDR_LEN];
    HI_U8   query;
    HI_U8   type   : 4;
    HI_U8   result : 3;     // �ο�MRS_CCO_SM_RESULT_E
    HI_U8   valid  : 1;
} MAC_ITEM;

typedef struct
{
    HI_U16 num;
    HI_U16 current;
    HI_U16 bind_current;
    HI_U16 pad;
    MAC_ITEM mac[PRODUCT_CFG_MRS_MAX_TOPO_NUM];
} MRS_SEARCH_METER_MAC_LIST;


// ȫ���ѱ�CCO��ά�ɲ�
typedef struct
{
    HI_U16 usUserDefinedDuration;               // �û��趨�ѱ�ʱ������λ: ��
    HI_U16 usTotal;                             // ��ǰCCO�Ѿ�����Ĵӽڵ���Ϣ������
    HI_U16 usMeterEntryNum;                     // ��ǰCCO�Ѿ�������ز����ӽڵ���Ϣ��������
    HI_U16 usCltEntryNum;                       // ��ǰCCO�Ѿ�����Ĳɼ����ӽڵ���Ϣ��������
    HI_U32 ulStartTime;                         // �ѱ�ʼ��ʱ���
    HI_U32 ulEndTime;                           // �ѱ�ֹͣ��ʱ���
} MRS_CCO_SEARCH_DFX_STRU;

typedef struct
{
    HI_U16 usCurrNum;           // ��ǰӳ����д���Ĵӽڵ����
    HI_U16 usCurrSeq;           // ��ǰ�ϱ��ӽڵ���Ϣ�����дӽڵ����
    HI_U8  ucReserved;          // �����ֶ�
    HI_U8  ucSendTimeCount;     // ���ѱ��з���ѯ��ʱ�ӵ��ܴ�������
    HI_U16 usReportedNum;       // ���ϱ����������ϱ��ӽڵ����

    HI_BOOL bQueryComplete;
    /* BEGIN: Added by l00238134, 2014/11/5   PN:DTS2014110801400*/
    HI_BOOL bWhiteListSwitch;
    HI_BOOL bWhiteListDelayTimer;
    HI_U8  ucWhiteListDelayTime;
    /* END:   Added by l00238134, 2014/11/5 */

    HI_U16 usStableTimes;
    HI_U16 usFrameLen;
    HI_U8 * pReportFrame;

    MR_NODE_METER_ADD_STRU * pReportedNodeInf;   //�ϱ��ӽڵ���Ϣ����

    MRS_SEARCH_METER_CFG_S stSearchCfg;
    MRS_SEARCH_METER_FRAME_S stSearchFrame;
    MRS_SEARCH_METER_MAC_LIST stMacList;
    MRS_SRV_QUEUE stMeterQueue;
    MRS_SRV_LIST stAbnormalCltii;
    MRS_SRV_LIST stSearchResultMeter;
    MRS_SRV_LIST stSearchResultClt;
    MRS_CCO_SEARCH_DFX_STRU stSearchDfx;
}MR_AUTO_SEARCH_STRU;  //�Զ��ѱ�ṹ

typedef struct
{
    HI_U16 usReadFailNum;
    HI_U16 reserved;
    MR_NODE_METER_ADD_STRU * pReportedNodeInf;
} MR_READ_FAIL_STRU;

typedef struct
{
    HI_U32 ulFileFlag;      //�ļ���־ 0x0101
    HI_U16 usVersion;       //�汾��Ϣ  0x02
    HI_U16 usMaxNodeNum;    //���֧�ֽڵ�����
    //HI_U16 usRealNodeNum; //flash����ʵ�洢�ı���Ϣ����
} MRS_FLASH_METER_INFO_FILE_HEAD_STRU; //flash�����洢�ı����ļ�ͷ�ṹ


#define MRS_CCO_STATUS_CREATE_MAP               (0x00000001)    // ӳ���ά��(����ӳ���)
#define MRS_CCO_STATUS_NORMAL                   (0x00000002)    // ����״̬(����ѯ��ʽ����)
#define MRS_CCO_STATUS_ANYTIME_READMETER        (0x00000004)    // �泭״̬(�㳭ʽ����)
#define MRS_CCO_STATUS_SEARCH_METER             (0x00000010)    // �������״̬
/* BEGIN: PN: DTS2015061008265 MODIFY\ADD\DEL by cuiate/00233580 at 2015/6/23 */
#define MRS_CCO_STATUS_SEARCH_METER_TRANSFORMER (0x00000020)    // ̨��ʶ���ѱ�״̬

#define MRS_CCO_STATUS_SEARCH_METER_MASK        (MRS_CCO_STATUS_SEARCH_METER | MRS_CCO_STATUS_SEARCH_METER_TRANSFORMER)
/* END:   PN: DTS2015061008265 MODIFY\ADD\DEL by cuiate/00233580 at 2015/6/23 */


#define MRS_CCO_NON_WORKING_STATUS              (0xFF)
#define MRS_CCO_WORKING_STATUS                  (1)

#define MRS_CCO_WORK_SWITCH_LOCAL_READ_METER    (0)
#define MRS_CCO_WORK_SWITCH_SEARCH_METER        (1)
#define MRS_CCO_WORK_SWITCH_REMOTE_UPGRADE      (2)
#define MRS_CCO_WORK_SWITCH_OTHER               (3)

#define MRS_CCO_LR_MODE_SINGLE                  (0)
#define MRS_CCO_LR_MODE_CONTINUOUS              (1)

#define MRS_CCO_SEQ_START_FROM_0                (0)
#define MRS_CCO_SEQ_START_FROM_1                (1)


#if defined(PRODUCT_CFG_SYS_RST_SOLUTION)
typedef struct
{
    HI_U32 ulRMStatTimes;       //�������ͳ��
    HI_U32 ulCcoRxUartTimes;    //CCO���ռ���������ͳ�ƴ���
}MRS_CCO_RESET_PARAM_S;//CCO������λ��ز�����Ϣ
#endif

typedef enum
{
    MRS_CCO_BROADCAST_IDLE,
    MRS_CCO_BROADCAST_BUSY,
} MRS_CCO_BROADCAST_STATE;

typedef struct
{
    HI_U8 ucInterval;
    HI_U8 ucRetryMax;
    HI_U8 ucRetry;
    HI_U8 ucState;
} MRS_CCO_BROADCAST_PARAM;

typedef struct
{
    MRS_CCO_BROADCAST_PARAM stParam;
    MRS_PLC_FRAME_DATA_STRU *pstPlcDataDl;  // ת����plc��������(CC0->STA)
    HI_U8 ucSeq;
    HI_U8 padding[3];
    HI_VOID (*pfnBroadcastFinishNotify)(HI_VOID*);
    HI_VOID *pUserParam;
}MRS_CCO_BROADCAST_STRU;

typedef enum
{
    MRS_EVENT_REPORT_STATUS_IDLE,
    MRS_EVENT_REPORT_STATUS_QUERYING,
    MRS_EVENT_REPORT_STATUS_REPORTING,
} MRS_EVENT_REPORT_STATUS_E;

// ��ѯһ��֮��ļ��
#define MRS_EVENT_QUERYING_LOOP_INTERVAL    (6000)  // 6s

// ÿ�β�ѯ֮��ļ��
#define MRS_EVENT_QUERYING_INTERVAL         (1000)  // 1s

// ����ʱ�����˳�������
#define MRS_RM_SCENE_UNKNOWN                     (0)   // ����ʱ�����˳���: δ֪����
#define MRS_RM_SCENE_TOPO_HAS_CLTI               (1)   // ����ʱ�����˳���: �����к���I�Ͳɼ����ز�վ��
#define MRS_RM_SCENE_TOPO_HAS_CLTII_NO_CLTI      (2)   // ����ʱ�����˳���: �����к���II�Ͳɼ���,û��I�Ͳɼ����ز�վ��
#define MRS_RM_SCENE_TOPO_HAS_NO_CLT             (3)   // ����ʱ�����˳���: �����в����ɼ����ز�վ��

#define MRS_CCO_FUNC_DEFAULT                    (0x00)
#define MRS_CCO_FUNC_BBU                        (0x01)
#define MRS_CCO_FUNC_CSG_GD                     (0x02)

#if defined(PRODUCT_CFG_PRODUCT_TYPE_CCO) && defined(PRODUCT_CFG_PRODUCT_TYPE_CCO_BROAD_BAND_UNIT)

#define MRS_CCO_BBU_TIME_MULTI                                  (10)
#define MRS_CCO_BBU_MAX_BYTE_INTERVAL_TIME_ZERO                 (0)
#define MRS_CCO_BBU_MAX_BYTE_INTERVAL_TIME_DEFAULT              (500)

#endif

typedef struct
{
    HI_U32 status;                  // ��ǰCCO״̬
    HI_BOOL bPauseFlag;
    HI_BOOL PlcChStatus;            // ��ǰPLCͨ��״̬ HI_FALSE : ������ HI_TRUE : ����
    HI_U8   ucTopoScene;            // ���˳�������: ���ڶ���ʱʱ������ж�
    HI_U8   ucCheckMode;            // �ͼ�ģʽ 0-���ͼ�ģʽ 1-�ͼ�ģʽ

    HI_U16  usPlcSeq;               // PLC�������-�㶫����

    HI_U8 ucMainNodeAddr[6];        // �ز����ڵ��ַ
    MR_ANYTIME_READMETER_STRU stAnytimeRMModel;         // �泭����ģ��
    MR_READ_METER_STRU stReadMeterModel;                // ����ģ��
    MR_AUTO_SEARCH_STRU stAutoSearchModel;              // �Զ��ѱ�ģ��
#if defined(PRODUCT_CFG_SYS_RST_SOLUTION)
    MRS_CCO_RESET_PARAM_S stResetStatInfo;     //��λ���ͳ����Ϣ
#endif

    MRS_MAP_MODULE stMapModule;     //ӳ���ģ��

    HI_U8 ucSeq;                    // ����֡���
    HI_U8 ucSeqControl      : 1;    // 0:���ͳһ��0��ʼ��1:���ͳһ��1��ʼ
    HI_U8 ucFreqEnable      : 1;    // CCO��ȡ��ʱ�Ӻ�����Ƶ�ζ�ʱ��ʹ�ܿ��� 0:������;1:����
    HI_U8 ucDutBoardFlag    : 1;    // ��װ�����е㳭ʹ�ù㲥�����־ 0:������  1:����
    HI_U8 ucLrModeFlag      : 1;    // �ֳ�ģʽ 0:����ģʽ 1:����ģʽ
    HI_U8 ucSearchWaitFlag  : 1;    // �ѱ�ȴ�������ʶ
    HI_U8 bFirstRun         : 1;    // �״����б�־
    HI_U8 bNeedGetTime      : 1;    // ��Ҫ��ȡʱ���־
    HI_U8 bCsgParallel      : 1;    // �㶫��������ʹ�ܱ�־

    HI_U8 ucProtoVer;               // �淶�汾 0-09�� 1-13�� ����-����
    HI_U8 plc_rxtx_mode;            // PLCͨ���շ�ģʽ 00:�ȴ�����·���Ż���ɺ�Ž���PLCͨ�����շ�; 01:ǿ��PLCͨ���շ�(���ȴ�PLC״̬);

    HI_U8 ucFlashSaveDelay;         // �������̼���ʱ��
    HI_U8 ucArchiveStatus   : 2;    // ������Ч��־ 0-Ĭ����Ч  1-�ȴ���Ч  2-����Ч
    HI_U8 ucSetPowerFlag    : 1;    // ���÷��书�ʱ�־
    HI_U8 reserved          : 5;
    HI_U8 ucFuncCfg;
    HI_U8 ucRecoverPowerTime;       // CCO�ָ����书������ʱ�䣬Ĭ��15����
    HI_U8 ucSoftTfMode;             // ����̨��ʶ�����ܿ���: 0�رգ�1��(���������أ�վ�������л�����)��2��(���������أ�վ�㲻�����л�����)
    HI_U8 ucCcoRstPinEnable;        // CCO��RST�ܽ�ʹ��

#if defined(PRODUCT_CFG_PRODUCT_TYPE_CCO_BROAD_BAND_UNIT)
    HI_U8 ucMaxByteIntervalTimeBbu;
    HI_U8 ucRunLedStatus;
    HI_U8 reserved1[2];
#endif

    HI_U16 usSearchDuration;        // �ѱ����ʱ�� ��λ: ����

    MRS_CCO_BROADCAST_STRU stBroadcast; //�㲥ģ��
    MR_READ_FAIL_STRU stReadFail;       //����ʧ�ܼ�¼

    HI_MAC_NETWORK_TOPO_S stTopoInf;       //������Ϣ
    HI_U8 *pTopoIntegerInfo;      //һ������������Ϣ��Ŀǰ����AFN=10-F21
    HI_U8 LowPowerMeterLastNum;//Matteo
}MRS_CCO_SRV_CTX_STRU;  // ����ģ�������Ľṹ CCO


// ��ȡCCO��ȫ�������ĵĽṹָ��
HI_PUBLIC MRS_CCO_SRV_CTX_STRU* mrsCcoGetContext(HI_VOID);

// CCO������״̬��(������ӳ���״̬)�ָ�Ϊ��ͨ״̬
HI_PUBLIC HI_VOID mrsRecoverNormalState(MRS_CCO_SRV_CTX_STRU* pstCcoCtx);

//��ȡ��ǰ��ά����MAC
HI_PUBLIC HI_U32 mrsMapGetCurrentMac(MRS_MAP_MODULE *module,HI_U8 mac[]);

HI_PUBLIC HI_BOOL mrsGetNeedGetTimeFlg(HI_VOID);

HI_PUBLIC HI_VOID mrsSetNeedGetTimeFlg(HI_BOOL flg);

// ��ȡPLC��ʱ��Ϣ�ṹAFN-13
HI_PUBLIC PLC_OVERTIME_INF_STRU *mrsGetPlcOverTimeInfAfn13(HI_VOID);

// ��ȡPLC��ʱ��Ϣ�ṹAFN-14
HI_PUBLIC PLC_OVERTIME_INF_STRU *mrsGetPlcOverTimeInfAfn14(HI_VOID);

// ��ȡPLCͨ����ʱʱ��-AFN13
HI_PUBLIC HI_U32 mrsGetPlcTimeOutAfn13(HI_U8 *mac, HI_BOOL force_bc, HI_U32 *m_time);

// ��ȡPLCͨ����ʱʱ��-AFN14
HI_PUBLIC HI_U32 mrsGetPlcTimeOutAfn14(HI_U8 *mac, HI_BOOL force_bc, HI_U32 *m_time);

// ��ȡPLCͨ��������Դ���-AFN13
HI_PUBLIC HI_U8 mrsGetMaxTryAfn13(HI_U8 *mac);

// ��ȡPLCͨ��������Դ���-AFN14
HI_PUBLIC HI_U8 mrsGetMaxTryAfn14(HI_U8 *mac);

// ��ȡ�ڵ�Plc��ʱ������(������ת�ڵ�)
HI_PUBLIC HI_U32 mrsGetPlcTimeOutMaxNum(HI_VOID);

HI_PUBLIC MRS_CCO_BROADCAST_STRU *mrsGetBroadcastCtx(HI_VOID);


// ��������: mrsSyncMeterInfToWhiteList
HI_PUBLIC HI_U32 mrsSyncMeterInfToWhiteList(HI_MAC_WHIST_LIST_CHANGE_REASON_ENUM enReason);

/* BEGIN: PN: DTS2015061008265 MODIFY\ADD\DEL by cuiate/00233580 at 2015/6/23 */
// ̨��ʶ�������ѱ�
HI_PUBLIC HI_U32 mrsCcoStartSearchMeterTransformer(HI_U32 ulDuration);

// ̨��ʶ��ֹͣ�ѱ�
HI_PUBLIC HI_U32 mrsCcoStopSearchMeterTransformer(HI_VOID);

// ֹͣ��ͨ�ѱ�
HI_PUBLIC HI_U32 mrsCcoStopSearchMeterNormal(HI_U8 ucReason);
/* END:   PN: DTS2015061008265 MODIFY\ADD\DEL by cuiate/00233580 at 2015/6/23 */

// ��ȡ��������ִ�����
HI_PUBLIC HI_U32 mrsGetMaxTurnNum(HI_VOID);
// ���ö�������ִ�����
HI_PUBLIC HI_VOID mrsSetMaxTurnNum(HI_U32 x);

// ��ȡ��ǰ�����ִ���
HI_PUBLIC HI_U32 mrsGetCurrTurnNum(HI_VOID);
// ���õ�ǰ�����ִ���
HI_PUBLIC HI_VOID mrsSetCurrTurnNum(HI_U32 x);
// ����ǰ�ִ�����
HI_PUBLIC HI_VOID mrsCurrTurnGrow(HI_VOID);

// ǿ���л�Ϊ·����ͣ״̬
HI_PUBLIC HI_VOID mrsForceSwitchPause(HI_VOID);

HI_PUBLIC HI_U32 mrsInitPlcOverTimeInf(HI_VOID);

//*****************************************************************************
// ��������: mrsCcoRxAfn13DataPro
// ��������: �㳭��CCO�յ�PLC���ݰ���Ĵ���
//
// ����˵��:
//   pPayload       [in]   AFN-13���ݰ�
//   usPayloadLen   [in]   AFN-13���ݰ�����
//   ppData         [out]  ת���������е�����
//   pusDataLen     [out]  ת���������е����ݳ���
//
// �� �� ֵ:
//   HI_ERR_SUCCESS  �ɹ�
//   HI_ERR_FAILURE  ʧ��
//
// ����Ҫ��: TODO: ...
// ���þ���: TODO: ...
// ��    ��: fengxiaomin/f00209182 [2013-02-04]
// ��    ��: cuiate/00233580 [2015-09-16]
//*****************************************************************************
HI_PUBLIC HI_U32 mrsCcoRxAfn13DataPro(HI_PVOID pPayload, HI_U16 usPayloadLen, HI_PVOID *ppData, HI_U16 *pusDataLen);

//*****************************************************************************
// ��������: mrsCcoRxAfn14DataPro
// ��������: �ֳ���CCO�յ�PLC���ݰ���Ĵ���
//
// ����˵��:
//   pPayload       [in]   AFN-14���ݰ�
//   usPayloadLen   [in]   AFN-14���ݰ�����
//   ppData         [out]  ת���������е�����
//   pusDataLen     [out]  ת���������е����ݳ���
//
// �� �� ֵ:
//   HI_ERR_SUCCESS  �ɹ�
//   HI_ERR_FAILURE  ʧ��
//
// ����Ҫ��: TODO: ...
// ���þ���: TODO: ...
// ��    ��: fengxiaomin/f00209182 [2013-02-04]
// ��    ��: cuiate/00233580 [2015-09-16]
//*****************************************************************************
HI_PUBLIC HI_U32 mrsCcoRxAfn14DataPro(HI_PVOID pPayload, HI_U16 usPayloadLen, HI_PVOID *ppData, HI_U16 *pusDataLen);


/* BEGIN: PN: DTS2015042907476 MODIFY\ADD\DEL by cuiate/00233580 at 2015/4/29 */
HI_PUBLIC HI_BOOL mrsGetCcoPlcChlStatus(HI_VOID);
/* END:   PN: DTS2015042907476 MODIFY\ADD\DEL by cuiate/00233580 at 2015/4/29 */

// ��ȡCCO����״̬(HI_TRUE : �Ѿ������Զ���; HI_FALSE : δ����)
HI_PUBLIC HI_BOOL mrsGetCcoReadyStatus(HI_U8 ucFlag);

// �� �� ֵ: PLC�������ݰ�ͳ����
//
// ����Ҫ��:
// ���þ���:
// ��    ��: fengxiaomin/00209182 [2013-03-02]
//*****************************************************************************
#if defined(PRODUCT_CFG_SYS_RST_SOLUTION)
HI_PUBLIC HI_U32 mrsCcoRMPlcStat(HI_VOID);
#endif

//*****************************************************************************
// ��������: mrsCcoRxUartStat
// ��������: CCO���ռ��������ݰ�ͳ��
// ����˵��: ��
//
// �� �� ֵ: CCO���ռ���������ͳ����
//
// ����Ҫ��:
// ���þ���:
// ��    ��: fengxiaomin/00209182 [2013-03-02]
//*****************************************************************************
#if defined(PRODUCT_CFG_SYS_RST_SOLUTION)
HI_PUBLIC HI_U32 mrsCcoRxUartStat(HI_VOID);
#endif

// Map��ά������
// option :
// MRS_MAP_MAINTAIN_MODE_ACTIVE          ����ά��
// MRS_MAP_MAINTAIN_MODE_PASSIVE         ӳ���ά��ģʽ(����)-Topo�������
// MRS_MAP_MAINTAIN_MODE_REFRESH         ӳ���ά��ģʽ(����ˢ��)-��ʱ������
HI_PUBLIC HI_U32 mrsMapMaintainProcess(HI_U8 option);


HI_PUBLIC HI_U32 mrsCcoReadFailInsert(HI_U8 MeterAddr[HI_METER_ADDR_LEN], HI_U8 protocol);
/* BEGIN: PN: DTS2015042907341 MODIFY\ADD\DEL by cuiate/00233580 at 2015/4/29 */
HI_PUBLIC HI_U32 mrsCcoReadFailRemove(HI_U8 MeterAddr[HI_METER_ADDR_LEN], HI_U8 protocol);
/* END:   PN: DTS2015042907341 MODIFY\ADD\DEL by cuiate/00233580 at 2015/4/29 */
HI_PUBLIC HI_U32 mrsSeqChangeNotify(HI_U16 usId);
HI_PUBLIC HI_U8 mrsGetCcoWorkFlag(HI_VOID);
HI_PUBLIC HI_VOID mrsCcoStatusInit(MRS_CCO_SRV_CTX_STRU *pstCcoCtx);
HI_PUBLIC HI_VOID mrsCcoReadFailReset(HI_VOID);
HI_PUBLIC HI_U32 mrsRefreshTopoInfo(MRS_CCO_SRV_CTX_STRU *pstCcoCtx);
HI_PUBLIC HI_VOID mrsCcoPowerOnPro(HI_VOID);
HI_PUBLIC HI_VOID mrsGetCcoMrParam(NV_APP_MR_CFG_STRU* cfg);
HI_PUBLIC HI_U32 mrsInitPlcTimeOutMaxNum(HI_VOID);

// ��ȡ����ʱ�����˳�������
HI_PUBLIC HI_U8 mrsGetRmTopoScene(HI_VOID);

// ��ȡ�㲥����ʱʱ���Ƿ��I�ɲ����ı�ʶ
HI_PUBLIC HI_BOOL mrsRmBcTimeoutCltIDelayFlag(HI_VOID);

#if defined(PRODUCT_CFG_PRODUCT_TYPE_CCO_BROAD_BAND_UNIT)
HI_VOID mrsRunLedProBbu(HI_VOID);
#endif

HI_VOID mrsCcoTopoStatusChgEntryCallback(HI_MDM_TOPO_STATUS_CHG_ENTRY_S* pstEntry);
HI_U32 mrsDiagCcoModeNotify(HI_SYS_QUEUE_MSG_S* pstMsg);
#if defined(PRODUCT_CFG_PRODUCT_TYPE_CSG_GD)
PLC_OVERTIME_INF_STRU *mrsGetCsgPlcOverTimeInf(HI_VOID);
HI_VOID mrsSetCsgPlcOverTimeInf(HI_U8 ucPriority);
#endif

HI_U8 mrsGetCheckMode(HI_VOID);
HI_VOID mrsSetArchivesStatus(HI_U8 ucStatus);
HI_U8 mrsGetArchivesStatus(HI_VOID);
HI_U32 mrsCheckMeterNum(HI_VOID);

HI_VOID mrsCcoTopoStatusChgProc(HI_SYS_QUEUE_MSG_S* pstMsg);

HI_END_HEADER
#endif //__MRS_SERVICE_H__

