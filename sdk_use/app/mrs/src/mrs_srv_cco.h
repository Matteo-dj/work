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
#include "mrs_srv_cco_macmap.h"
#include "mrs_fw_proto376_2_frame.h"

HI_START_HEADER


#define MRS_CCO_READ_METER_TASK_END     1 //�����������
#define MRS_CCO_SEARCH_METER_TASK_END   2 //�ѱ��������


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
    HI_U8   ucRfReportFlag : 1;// ����ˮ���Ѿ��ϱ��ı�־
	HI_U8   ucReserved : 1;
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
    HI_U8  aucMacAdd[HI_PLC_MAC_ADDR_LEN]; //�洢plc���յ�MAC��ַ
    HI_U16  ucRsv1;
}MR_READ_METER_STRU;    // ����ģ��ṹ
typedef struct
{
    HI_U8 enCurrRfState;  
    MRS_ARCHIVES_NODE_STRU stRfCurrNode;   // ��ǰ����Ľڵ�(����)
    HI_U16 usCurrRfNodeSeq; //����ģ���г����Ľڵ����
    MRS_PLC_FRAME_DATA_STRU *pstRfPlcDataDl; // ת����plc��������(CC0->STA)
    UP_READMETER_DATA * pstRfUpReadMeterData; // �ϴ���������(����)
    HI_U16 usRfMeterNum;
    
    PLC_OVERTIME_INF_STRU stRfOverTimeInf;    // PLCͨ����ʱʱ�������Ϣ
    HI_U32 ulTimeOutMaxNumNv;  // �ڵ����������ʱ����(Nv��)
    HI_U32 ulTimeOutCount;  // ��ǰ�ڵ�������������ʱ����
    HI_U32 ulTurnNumNv;     // ����ִ���(Nv��)
    HI_U32 ulCurrTurn;      // ��ǰ�ִ���
    HI_U16 usPLCTimeOut;    
    HI_U16 usRfPlcSeq;  //֡���
    HI_U16 us20F3UpSeq;  //֡���
    HI_U16 usAfn20F3UpTimeOut;      //���б��ĳ�ʱʱ��(Nv���λms)
    HI_U8  ucAfn20F3UpRetryMax;     //���б��ĳ�ʱ���Դ���(Nv��)
    HI_U8  ucCurrNodeInquireTries;  //���ִε�ǰ�ڵ���ѯ����
    HI_U8  aucMacAdd[HI_PLC_MAC_ADDR_LEN]; //�洢plc���յ�MAC��ַ
}MR_READ_WATER_METER_STRU; 

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
#if defined(MRS_SRV_PROTOCOL_CJT188) 
	PLC_OVERTIME_INF_STRU stOverTimeInfSaved; //���ڴ洢PLCͨ����ʱʱ�������Ϣ��
	HI_S8 ucAdditionTime188;	//188������ʱ��
	HI_S8 ucPlcMaxReTryS188;	//188���������Դ���
	HI_S8 ucPlcMaxReTryB188;	//188����㲥���Դ���
	HI_U8 reserved;
#endif
    HI_U16 seq;  //֡���
    HI_BOOL bIsBC;                          // �Ƿ�Ϊ�㲥ʽ����
    HI_U8 ucProtocol;
    HI_U8 seq_1376_2;
    HI_U8 from;         // �㳭����֡��Դ
                        // XR_FROM_AFN02_F1 (ת��ͨѶЭ������֡)
                        // XR_FROM_AFN13_F1 (����ز��ӽڵ�)
    HI_U8 ucFn; // beijing,02F2 ��02F1 һ�µĸ�ʽ�������в�һ���Ĵ���
    HI_U8 aucMeterAddr[HI_METER_ADDR_LEN];
}MR_ANYTIME_READMETER_STRU;    // �泭����ģ��

typedef struct
{
    HI_U8   bc_send_interval;
    HI_U8   bc_loop_max;
    HI_U8   topo_query_interval;
    HI_U8   topo_stable_times;

    HI_U8   query_max_way;
    HI_U8   query_interval;
    HI_U8   query_force_times;
    HI_U8   query_except_times;

    HI_U8   search_finish_reason;
    HI_U8   report_AA_2_master;
    HI_U16  usAfn06F4UpTimeOut;     // �ѱ�06F4�ϱ���ʱʱ�䣬��λ��ms

    HI_U8   ucAfn06F4UpRetryMax;    // �ѱ�06F4�ϱ�������Դ���
    HI_U8   ucBindNetwork;          // �Ƿ�ʹ����������
    HI_U16  usSMDurationDefault;    // �ѱ�Ĭ��ʱ������λmin

    HI_U8   ucRecoverWhiteSwitch;   // �ѱ����ʱ���Ƿ�ָ�����������״̬
    HI_U8   ucBindCmdSendInterval;  // ��������ÿ��֮��ķ��ͼ������λs
    HI_U8   ucCheckMainNodeAddr;    // ������ڵ��ַ�ı�ʱ�����½���̨��ʶ��
    HI_U8   ucAllowSMInTf;          // ̨��ʶ�����Ƿ������ѱ�

    HI_U16  usCheckMainNodeTimeout; // ������ڵ��ַ��ʱʱ��, ��λs
    HI_U8   ucSmReportWaitLockMsg;  // �ѱ��ϱ��Ƿ�ȴ���������
    HI_U8   ucCloseWhiteWhenSM;     // �ѱ�����Ƿ�رհ�����

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
    MRS_SRV_LINK link;

    METER_NODE * pstMeterNode;

    HI_U8 query_times;
    HI_U8 pad[3];
} METER_LIST_NODE;

typedef struct
{
    HI_U8  aucMac[HI_PLC_MAC_ADDR_LEN];
    HI_U16 pad;
    MRS_SRV_LINK * link;
} MRS_METER_LIST_NODE_GET_STRU;

typedef struct
{
    HI_U8   mac[HI_PLC_MAC_ADDR_LEN];
    HI_U8   query;
    HI_U8   type   : 4;
    HI_U8   result : 3;
    HI_U8   valid  : 1;
} MAC_ITEM;

typedef struct
{
    HI_U16 num;
    HI_U16 current;
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
    HI_U8  ucCurNodeReported;   // ��ǰ�ڵ����ϱ�
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

    HI_BOOL bCheckingMainAddr;      // �Ƿ����ڼ�����ڵ��ַ
    HI_BOOL bLockComplete;          // �Ƿ��������
#if defined(PRODUCT_CFG_HUNAN)
	HI_BOOL bNewMeterSearch:1;	//�Ƿ���������ѱ�����
    HI_BOOL bNodeCheck:1;	//�Ƿ��Ѽ�鵵����Ϣ
    HI_U8 ucNotInArchivesNum;
#endif
    HI_U8   pad[2];
    MRS_SRV_LIST stUnLockStaList;   // δ�ϱ�վ������
    MRS_SRV_LINK *pstCurLockNode;   // ��ǰ���ڲ�ѯ������ڵ�
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

#define MRS_CCO_LEVEL_DISABLE                   (0)
#define MRS_CCO_LEVEL_ENABLE                    (1)


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
    HI_U32 reserved;
}MRS_CCO_BROADCAST_STRU;

typedef enum
{
    MRS_EVENT_REPORT_STATUS_IDLE,
    MRS_EVENT_REPORT_STATUS_QUERYING,
    MRS_EVENT_REPORT_STATUS_REPORTING,
} MRS_EVENT_REPORT_STATUS_E;
typedef HI_U32 (* MRS_SEND_CCO_PROC_F)(HI_VOID *);
typedef HI_VOID (* MRS_SEND_CCO_PROC_RX_F)(HI_VOID *);

// ��ѯһ��֮��ļ��
#define MRS_EVENT_QUERYING_LOOP_INTERVAL    (6000)  // 6s

// ÿ�β�ѯ֮��ļ��
#define MRS_EVENT_QUERYING_INTERVAL         (1000)  // 1s

// ����ʱ�����˳�������
#define MRS_RM_SCENE_UNKNOWN                     (0)   // ����ʱ�����˳���: δ֪����
#define MRS_RM_SCENE_TOPO_HAS_CLTI               (1)   // ����ʱ�����˳���: �����к���I�Ͳɼ����ز�վ��
#define MRS_RM_SCENE_TOPO_HAS_CLTII_NO_CLTI      (2)   // ����ʱ�����˳���: �����к���II�Ͳɼ���,û��I�Ͳɼ����ز�վ��
#define MRS_RM_SCENE_TOPO_HAS_NO_CLT             (3)   // ����ʱ�����˳���: �����в����ɼ����ز�վ��

#if defined(PRODUCT_CFG_PRODUCT_TYPE_CCO) && defined(PRODUCT_CFG_PRODUCT_TYPE_CCO_BROAD_BAND_UNIT)
#define MRS_CCO_FUNC_DEFAULT                                    (0)
#define MRS_CCO_FUNC_BBU                                        (0x1)

#define MRS_CCO_BBU_TIME_MULTI                                  (10)
#define MRS_CCO_BBU_MAX_BYTE_INTERVAL_TIME_ZERO                 (0)
#define MRS_CCO_BBU_MAX_BYTE_INTERVAL_TIME_DEFAULT              (500)

#define MRS_CCO_BBU_MASK             (0x01)
#endif

#if defined(PRODUCT_CFG_HUNAN)
typedef struct
{
    HI_U8  mac[HI_PLC_MAC_ADDR_LEN];          /**< MAC��ַ             */
    HI_U8  ucUpdateFlg;                       /**< ���±�־                  */
    HI_U8  aucModuleId[11];                   /**< ģ��ID                  */
}HI_MODULE_ID_S;

typedef struct
{
    HI_U32 num;			/**<����Ԫ�ظ���*/
    HI_MODULE_ID_S entry[HI_MAC_NETWORK_TOPO_ENTRY_MAX];	/**<ģ��ID��Ϣ*/
}HI_MAC_NETWORK_MODULE_ID_S;
#endif

typedef struct
{
	MRS_SRV_LINK link;
    HI_U8  ucaStaMac[HI_PLC_MAC_ADDR_LEN];
	HI_U8 ucAddressType; // 0����STA ���ͣ�1����ΪCCO
	HI_U8  ucMeterNum;
	HI_U8  ucSendIndex;
    HI_U8  pMeterData[0];
} MRS_SEARCH_WATER_METER_LIST;
typedef struct
{
    HI_U8  ucaWaterMac[7];
	HI_U8  ucRFChannelInfoNum;
	HI_U8  ucWaterMeterIndex; // ��߳�ǿֵ�ı��ַ
	HI_U8  ucWaterMaxTryCount; // ������������ʧ���л���ַ�Ĵ���
	HI_U8  ucCcoWaterMeterFlag; // ����CCO �ϵ�ˮ��ģ��
	HI_U8  ucaStaMac[2][6];
	HI_U8  ucaFieldValue[2]; // ��ǿ��Ϣ
} MRS_WATER_METER_RESULT;

//�˱������ڼ�¼��ndm��plc�㳭�����æ���ش���ʱ��
#define MRS_CCO_NDM_PLC_BUSY_PROC    (2000)
#define MRS_WATER_METER_MAX_NUM 64

typedef struct
{
    HI_U16 usCurrNum;           // ��ǰӳ����д���Ĵӽڵ����
    HI_U16 usCurrSeq;           // ��ǰ�ϱ��ӽڵ���Ϣ�����дӽڵ����
    HI_U8  ucCurNodeReported;   // ��ǰ�ڵ����ϱ�
    HI_U8  ucSendTimeCount;     // ���ѱ��з���ѯ��ʱ�ӵ��ܴ�������
    HI_U16 usReportedNum;       // ���ϱ����������ϱ��ӽڵ����

	HI_U16 usWaterSearchSeq;	// ���к�
	HI_U16 usSignleWaterAllNum; // ����ˮ���������
	HI_U8 ucSignleReportNum;	// �Ѿ��ϱ���ˮ������

	HI_U8 ucSetOwnerShipMeterNum;// ����ˮ��������ϵ�������
	
	HI_U8 ucReadWaterMeterDataIndex;//����ˮ��ʱ����¼ʹ�õ����ĸ���ַ 

    HI_BOOL bQueryComplete;
	HI_BOOL bStartReadMeterFlag; // ��ȡ����Ϣ
	HI_BOOL bStartSetMeterShipFlag; // ��ʼ���ñ���Ϣ
	HI_BOOL bSetMeterShipSendPLCFlag; // ��ʼ���ñ�����PLC��Ϣ

    HI_U16 usStableTimes;
	HI_U16 usWaterMeterResultNum;
	MRS_WATER_METER_RESULT stWaterMeterResult[MRS_WATER_METER_MAX_NUM]; // �������ӻ�ȡ������
    MRS_SEARCH_METER_FRAME_S stSearchWaterFrame;
    MRS_SRV_LIST stGetWaterMeterList;   // �ϱ�վ���������������MRS_SEARCH_WATER_METER_LIST
}MR_WATER_METER_SEARCH_STRU;

typedef struct
{
	HI_U8     ndm_addr[HI_PLC_MAC_ADDR_LEN];	    //��������ַ
	HI_U8     src_addr[HI_PLC_MAC_ADDR_LEN];	    //�������·�plc֡�������3762֡��Դ��ַ
	HI_U8     old_from;                             //�㳭������Դ
	HI_U8     pro_type;                             //Э������ plc֡ͷ����
	HI_U16    frame_len;			                //plc�������ݳ��� plc֡ͷ����
}MR_CCO_NDM_PLC_INFO_STRU;                       //�������·���cco��Ϣ��¼  
typedef struct
{
	HI_U8             busy_from;                            // ����ĵ㳭֡��Դ
	HI_U8             reserved[3];                          // ����
	MRS_3762_FRAME_STRU* pst_frame;                         // �������㳭����æ����ʱ�Ĳ�����¼
	MR_CCO_NDM_PLC_INFO_STRU ndm_plc_info;                  // ndm֡plcæ����ʱ����Ϣ��¼
}MR_CCO_NDM_PLC_BUSY_STRU;                              // �������������볭�����㳭afn13ʱ����æ���صĴ���ṹ��


//Matteo
typedef struct
{
	HI_U16 num;
    HI_U16 current;
    MAC_ITEM IICltmac[500];
}II_ARCHVICE;


typedef struct
{
    HI_U32 status;                  // ��ǰCCO״̬
    HI_BOOL bPauseFlag;
    HI_BOOL PlcChStatus;            // ��ǰPLCͨ��״̬ HI_FALSE : ������ HI_TRUE : ����
    HI_U8   ucTopoScene;            // ���˳�������: ���ڶ���ʱʱ������ж�
    HI_BOOL bFirstRun;              // �״����б�־
    HI_BOOL bNeedGetTime;           // ��Ҫ��ȡʱ���־
    HI_U8   ucCcoRstPinEnable;      // ȥʹ��CCO��RST�ܽ�
    HI_U8 ucMainNodeAddr[6];        // �ز����ڵ��ַ
    MR_ANYTIME_READMETER_STRU stAnytimeRMModel;         // �泭����ģ��
    MR_READ_METER_STRU stReadMeterModel;                // ����ģ��
    MR_AUTO_SEARCH_STRU stAutoSearchModel;              // �Զ��ѱ�ģ��
#if defined(PRODUCT_CFG_SYS_RST_SOLUTION)
    MRS_CCO_RESET_PARAM_S stResetStatInfo;     //��λ���ͳ����Ϣ
#endif
	MR_READ_WATER_METER_STRU stReadWaterMeterModel; // ��ȡˮ��ģ��
	MR_WATER_METER_SEARCH_STRU stWaterMeterSearchModel;              // �Զ��ѱ�ģ��
    MRS_MAP_MODULE stMapModule;     //ӳ���ģ��
    HI_U8 aucSrcMac[HI_PLC_MAC_ADDR_LEN];
    HI_S16 sDutNtbAjustBase;

    HI_U8 ucSeq;                    // ����֡���
    HI_U8 ucSeqControl      : 1;    // 0:���ͳһ��0��ʼ��1:���ͳһ��1��ʼ
    HI_U8 ucFreqEnable      : 1;    // CCO��ȡ��ʱ�Ӻ�����Ƶ�ζ�ʱ��ʹ�ܿ��� 0:������;1:����
    HI_U8 ucDutBoardFlag    : 1;    // ��װ�����е㳭ʹ�ù㲥�����־ 0:������  1:����
    HI_U8 ucLrModeFlag      : 1;    // �ֳ�ģʽ 0:����ģʽ 1:����ģʽ
/* BEGIN: PN: DTS2015042907476 MODIFY\ADD\DEL by cuiate/00233580 at 2015/4/29 */
    HI_U8 ucSearchWaitFlag  : 1;    // �ѱ�ȴ�������ʶ
    HI_U8 ucLevelFlag       : 1;    // �м̼�����־:0:������ 1:����
    HI_U8 ucSetFreqFlag     : 1;
    HI_U8 ucPadding         : 1;
/* END:   PN: DTS2015042907476 MODIFY\ADD\DEL by cuiate/00233580 at 2015/4/29 */

    HI_U8 ucProtoVer;               // �淶�汾 0-09�� 1-13�� ����-����
    HI_U8 plc_rxtx_mode;            // PLCͨ���շ�ģʽ 00:�ȴ�����·���Ż���ɺ�Ž���PLCͨ�����շ�; 01:ǿ��PLCͨ���շ�(���ȴ�PLC״̬);

    HI_U8 ucFlashSaveDelay;         // �������̼���ʱ��
    HI_U8 ucPhase           : 3;
    HI_U8 reserved          : 5;

#if defined(PRODUCT_CFG_PRODUCT_TYPE_CCO_BROAD_BAND_UNIT)
    HI_U8 ucFuncCfg;
    HI_U8 ucMaxByteIntervalTimeBbu;
    HI_U8 ucRunLedStatus;
    HI_U8 reserved1;
#endif
	HI_U32 ulCcoNetSnid;
    HI_U16 usSearchDuration;        // �ѱ����ʱ�� ��λ: ����
    HI_U8  ucFreq;
    HI_U8  ucSoftTfMode; // ����̨��ʶ��ģʽ
	HI_U8  aucCode[2];
    HI_U8  area_code;
    HI_U8  pad[3];
	HI_U16  ucRfTimeSlot; // RF  ����ʱ϶ʱ�䷢��ʱ�䣬����
	HI_U16	ucRfSGDTimeSlot; // ��������RF  ����ʱ϶ʱ�䷢��ʱ�䣬����
#if defined(MRS_SRV_MODULE_ID)
	//HI_U8 aucModuleId[MRS_SRV_MODULE_ID_LEN_MAX];
#endif

    MRS_CCO_BROADCAST_STRU stBroadcast; //�㲥ģ��
    MR_READ_FAIL_STRU stReadFail;       //����ʧ�ܼ�¼

    HI_MAC_NETWORK_TOPO_S stTopoInf;       //������Ϣ
    HI_U8 *pTopoIntegerInfo;      //һ������������Ϣ��Ŀǰ����AFN=10-F21\F31\F32\F228
#if defined(PRODUCT_CFG_HUNAN)
	HI_MAC_NETWORK_MODULE_ID_S* pstModuleIdInf;
	HI_MAC_NETWORK_MODULE_ID_S* pstTempModuleIdInf;//��ʱģ��ID��Ϣ��Ŀǰ����ά��pstModualIdInf
#endif
    HI_MDM_NETWORK_STA_ID_S *pstStaIdInfo; //һ�������ڵ�ID��Ϣ��Ŀǰ����AFN=10-F33
	MR_CCO_NDM_PLC_INFO_STRU stCcoNdmPlcInfo; //�������·�ccoplcʱ�ı�����Ϣ
    MR_CCO_NDM_PLC_BUSY_STRU stCcoNdmPlcBusyInfo; //�г������·�plc֡�㳭�����æ������Ϣ��¼
    HI_U8 LowPowerMeterLastNum;//Matteo
    II_ARCHVICE *usIIArchive;//Matteo
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
#if defined(MRS_SRV_PROTOCOL_CJT188)
PLC_OVERTIME_INF_STRU *mrsGetPlcOverTimeInfSavedAfn13(HI_VOID);
#endif

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
HI_PUBLIC HI_U32 mrsCcoRxAfn13DataPro(HI_PVOID pPayload, HI_U16 usPayloadLen, HI_PVOID *ppData, HI_U16 *pusDataLen, HI_U8 ucMacAdd[HI_PLC_MAC_ADDR_LEN]);

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
HI_PUBLIC HI_U32 mrsCcoRxAfn14DataPro(HI_PVOID pPayload, HI_U16 usPayloadLen, HI_PVOID *ppData, HI_U16 *pusDataLen, HI_U8 ucMacAdd[HI_PLC_MAC_ADDR_LEN]);


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
HI_U8 mrsCcoGetStaPhase(HI_U8 *pMac);
HI_U8* mrsCcoGetSrcMac(HI_VOID);
HI_VOID mrsCcoSetCtxPhase(HI_U8 ucPhase);

// ���� ���ڵ��ַ�������
HI_VOID mrsCcoCheckMainAddrStart(MRS_CCO_SRV_CTX_STRU * pstCcoCtx);
// ���� ���ڵ��ַ�ı�
HI_VOID mrsCcoHandleMainAddrChange(MRS_CCO_SRV_CTX_STRU * pstCcoCtx);
// ���� ���ڵ��ַ��鳬ʱ
HI_VOID mrsCcoHandleCheckMainAddrTimeout(MRS_CCO_SRV_CTX_STRU * pstCcoCtx);

HI_VOID mrsCcoTopoStatusChgProc(HI_SYS_QUEUE_MSG_S* pstMsg);

HI_BOOL mrsCheckStaIsInTopoInf(HI_U8 aucMac[HI_PLC_MAC_ADDR_LEN]);
HI_U16 mrsCcoGetTeiByMac(HI_U8 *pMac);
HI_U32 mrsCcoGetMacByTei(HI_U16 usTei, HI_U8 *pucMac);

HI_MDM_SOFT_TF_MODE_EN mrsCcoGetSoftTfMode(HI_VOID);

HI_U32 mrsMsgSendPlcToNdm(HI_PBYTE pout_buffer, HI_U16 buffer_outlen);
void RfFreePLCData(MRS_PLC_FRAME_DATA_STRU *pstPlcDataDl);
HI_VOID RfFreeWaterMeterListNode(HI_VOID * pstNode, HI_VOID * pstCb);
HI_U32 mrsRfGetWaterMeterMAC(HI_U8 *pucDeviceAddr, HI_U8 *pucMacAddr);
HI_U32 mrsReportCcoUartRecvWaterMeterData(HI_VOID);
HI_U32 mrs3762_ReportAFN20_F3(HI_U8 *pData,HI_U16 usDataLen,MRS_SEND_CCO_PROC_RX_F pRxRespProc,MRS_SEND_CCO_PROC_F pTotalTimeOutProc);
HI_U32 mrsCcoSearchMeterGetWaterMeterlist(MRS_CCO_SRV_CTX_STRU *cco,HI_U8 ucFlag);
HI_U32 mrsCcoNdmPlcBusyAdd(AFN_FN_UL_PARAM * pst_afn_param, HI_U8 anytime_from);
HI_VOID mrsCcoNdmPlcTimeout(HI_VOID);
HI_VOID mrsCcoNdmPlcBusyProc(HI_VOID);

hi_bool mrs_check_area_code(hi_u8 area_code);
//Matteo
HI_U32 mrsCcoSaveIICltDeviceType( MRS_CCO_SRV_CTX_STRU  * cco,HI_PBYTE pMeterAddr,HI_U8 Type);


HI_END_HEADER
#endif //__MRS_SERVICE_H__

