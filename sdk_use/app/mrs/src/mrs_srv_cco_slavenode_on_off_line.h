//*****************************************************************************
//
//                  ��Ȩ���� (C), 1998-2015, ��Ϊ�������޹�˾
//
//*****************************************************************************
//  �� �� ��   : mrs_srv_cco_slavenode_on_off_line.h
//  �� �� ��   : V1.0
//  ��    ��   : liupeidong/00238134
//  ��������   : 2015-03-10
//  ��������   : CCO�ز��ӽڵ����������״̬�ϱ�������������������ݽṹ���궨��
//
//  �����б�   : TODO: ...
//  �޸���ʷ   :
//  1.��    �� : 2015-03-10
//    ��    �� : liupeidong/00238134
//    �޸����� : �����ļ�
//
//*****************************************************************************
#ifndef __MRS_SRV_CCO_SLAVENODE_ON_OFF_LINE_H__
#define __MRS_SRV_CCO_SLAVENODE_ON_OFF_LINE_H__

HI_START_HEADER
#if defined(PRODUCT_CFG_PRODUCT_TYPE_CCO)

#define MRS_TIME_100_MILLI_SEC             (100)     // 100����

#define MRS_NODE_STATE_CHG_DATA_HEADER_LEN (2+1+2)   // �ӽڵ�״̬�仯֡������ͷ: �ϱ�����+��֡����+��֡��һ����ʼ���
#define MRS_ONE_NODE_STATE_CHG_INFO_LEN    (6+1+4+1) // �����ӽڵ�״̬�仯��Ϣ�ĳ���: MAC��ַ+״̬�仯+����ʱ��+����ԭ��

#define MRS_ONLINE_TO_OFFLINE_DURATION_DEFAULT   (0xEEEEEEEE)

#define MRS_DEAL_SCALE_OFF_INTERVAL         (100)      //���ģͣ�紦��ʱ���ж��Ƿ�Ϊ�����ϱ�̽��ɹ�ʱ����Ϊs
#define MRS_DETECT_PCO_SUCCESS_MAX_INTERVAL (420)      //̽��pco�ɹ������ʱ���� ��λs��ʱ��������1min������̽�ⲹ��,������վ��̽��ʱ��1min

typedef struct
{
    MRS_SRV_NODE stLink;
    HI_MDM_NETWORK_TOPOCHG_ENTRY_S stEntry;
} MRS_CHG_ENTRY_QUEUE_ITEM;

typedef struct
{
    MRS_SRV_NODE stLink;
    HI_U16 usFrameLen;
    HI_U8  ucEntryNum;
    HI_U8  ucReserved;
    HI_U8* pucFrame;
} MRS_CHG_FRAME_QUEUE_ITEM;

typedef struct
{
    HI_U8   ucNum;                       // ����Ԫ�ظ���,U8
    HI_U8   aucPad[3];                   // ����λ
    MRS_SRV_QUEUE stQueue;               // ����
} MRS_CHG_ENTRY_QUEUE;

// վ��ͣ����״̬�ṹ 56B
typedef struct
{
    HI_U8   ucStatus;                   // ͣ����״̬���ο�MRS_POWEROFF_STATUS_E
    HI_U8   ucReCheckStatus;            // ͣ�縴��״̬���ο�MRS_POWERON_RECHECK_STATUS_E
    HI_U8   aucMac[HI_PLC_MAC_ADDR_LEN];

    HI_U8   ucDetectQueryCnt;           // ̽�Ⳣ�Դ���
    HI_U8   ucRecheckQueryCnt;          // ���˳��Դ���
    HI_U8   ucReportCheckScore;         // �ϱ�������˻���
    HI_U8   ucRecheckFailCnt;           // ����ʧ�ܴ���

    HI_U16  usMacReportOffCnt;          // �յ�MAC���ϱ����ߴ���
    HI_U16  usMacReportOnCnt;           // �յ�MAC���ϱ����ߴ���

    HI_BOOL bPassedDetect;              // �������Ƿ���ͨ����ͣ��̽��
    HI_U8   ucMacReportWrongCnt;        // MAC�ϱ��ģ��󱨴���
    HI_U8   ucMacReportMissCnt;         // MAC�ϱ��ģ�©������
    HI_U8   ucMacReportCorrectCnt;      // MAC�ϱ��ģ���ȷ����

    HI_U8   ucDetectReportWrongCnt;     // ̽����ϱ����󱨴���
    HI_U8   ucDetectReportMissCnt;      // ̽����ϱ���©������
    HI_U8   ucDetectReportCorrectCnt;   // ̽����ϱ�����ȷ����
    HI_U8   uc3762ReportWrongCnt;       // �ϱ���������ģ��󱨴���

    HI_U8   uc3762ReportMissCnt;        // �ϱ���������ģ�©������
    HI_U8   uc3762ReportCorrectCnt;     // �ϱ���������ģ���ȷ����
    HI_U16  pad;

    HI_U32  ulLastMacReportWrongTime;   // ��һ��MAC���󱨵�ʱ�䣬CCO�ϵ�ʱ��

    HI_U32  ulLastMacReportOffTime;     // ���һ��MAC���ϱ����ߵ�RTXʱ��
    HI_U32  ulLastMacReportOnTime;      // ���һ��MAC���ϱ����ߵ�RTCʱ��
    HI_U32  ulLastReportOffTime;        // ���һ���ϱ�ͣ�絽��������RTCʱ��
    HI_U32  ulLastReportOnTime;         // ���һ���ϱ����絽��������RTCʱ��

    HI_U32  ulMeterLastPoweroffTime;    // ������һ��ͣ��RTCʱ��
    HI_U32  ulMeterLastPoweronTime;     // ������һ���ϵ�RTCʱ��
} MRS_POWEROFF_CHECK_ITEM_S;


// ͣ�����б�
typedef struct
{
    HI_BOOL bDetectComplete;    // �Ƿ�̽�����
    HI_BOOL bRecheckComplete;   // �Ƿ񸴺����
    HI_U16  pad;

    HI_U16  usDetectIdx;        // ��ǰ̽�⵽��һ���ڵ�
    HI_U16  usRecheckIdx;       // ��ǰ���˵���һ���ڵ�

    MRS_POWEROFF_CHECK_ITEM_S astItems[PRODUCT_CFG_MRS_MAX_TOPO_NUM];
} MRS_POWEROFF_CHECK_LIST_S;


typedef struct
{
    HI_U16  usNum;                       // ����Ԫ�ظ���,U16
    HI_U8   aucPad[2];                   // ����λ
    MRS_SRV_QUEUE stQueue;               // ����
} MRS_CHG_FRAME_QUEUE;

//ͣ����(�ǵ���ģ��)��ر���
#define MRS_POWEROFF_CHECK_HISTORY_NUM          (80) // ��ʷ��¼�洢����

// ͣ����״̬
typedef enum
{
    MRS_POWEROFF_DEFAULT = 0,               // ��ʼ״̬
    MRS_POWEROFF_WAIT_DETECTING,            // ͣ��ȴ�̽��
    MRS_POWEROFF_DETECTING,                 // ͣ��̽����
    MRS_POWEROFF_DETECT_SUCCESS,            // ͣ��̽��ɹ���δͣ��
    MRS_POWEROFF_INTERCEPT_BY_RECORD,       // ��ͣ���¼�����أ����ϱ�
    MRS_POWEROFF_INTERCEPT_BY_PCO_DETECT,   //��̽�⵽pco ̽��ɹ������أ����ϱ�
    MRS_POWEROFF_REPORTED,                  // ���ϱ�
} MRS_POWEROFF_STATUS_E;


// ���߸���״̬
typedef enum
{
    MRS_POWERON_RECHECKED,     // �������
    MRS_POWERON_RECHECKING,    // ������
} MRS_POWERON_RECHECK_STATUS_E;

// ��ʷ��¼�ڵ�ṹ 24B
typedef struct
{
    HI_U8  aucMac[HI_PLC_MAC_ADDR_LEN];
    HI_U8  ucStatus; // ͣ����״̬���ο�MRS_POWEROFF_STATUS_E
    HI_U8  pad;

    HI_U32 ulMacReportOffTime;      // MAC�ϱ����ߵ�ʱ��
    HI_U32 ulMacReportOnTime;       // MAC�ϱ����ߵ�ʱ��
    HI_U32 ulMeterLastPoweroffTime; // ������һ��ͣ��ʱ��
    HI_U32 ulMeterLastPoweronTime;  // ������һ���ϵ�ʱ��
} MRS_POWEROFF_HISTORY_ITEM_S;

// ��ʷ��¼�洢�ṹ 964B
typedef struct
{
    HI_U8 ucIndex;
    HI_U8 pad[3];

    MRS_POWEROFF_HISTORY_ITEM_S astRecords[MRS_POWEROFF_CHECK_HISTORY_NUM];
} MRS_POWEROFF_HISTORY_LIST_S;

typedef struct
{
    HI_U8   ucReportEnable;                      // NV�����Ƿ���վ������������Ϣ�ϱ�����
    HI_U8   ucPowerOffReasonSwitch;              // ����ԭ���ϱ�����
    HI_U8   ucPlcChlReasonSwitch;                // �ŵ�ԭ���ϱ�����
    HI_U8   ucCacheNumMax;                       // �����л�������������ﵽ��������ʼ�ϱ�
    HI_U16  usCacheTimeout;                      // �����л�����ʱ�䣬��λ: 100����
    HI_U16  usReportTimeout;                     // �ϱ���������ÿ�γ��Եĳ�ʱʱ�� ��λ: ����
    HI_U8   ucReportRetryMax;                    // �ϱ�����������������Դ���
    HI_U8   ucJoinCcoQueueLock;                  // ����CCO MRS�������е���,������������յ�ACK��ʱ�����
    HI_U8   ucPoweroffDetectMaxTries;   // ͣ��̽��������
    HI_U8   ucPoweroffDetectExpTries;   // ��MAC����������֮�ڣ�����MAC�󱨣�����Ⳣ����ô���
    HI_U32  ulMacReportWrongPeriodTh;   // MAC���������ޣ���λmin

    HI_U8   ucPoweroffDetectInterval;   // ͣ��̽��㲥���ķ��ͼ������λs
    HI_U8   ucPoweroffRecheckMaxTries;  // ͣ�縴��������
    HI_U8   ucPoweroffRecheckInterval;  // ͣ�縴�˹㲥���ķ��ͼ������λs
    HI_U8   ucReviewScoreFull;          // ͣ���⸴�����ַ���

    HI_U8   ucWrongReportReduceScore;   // ��ʱ�����ַ���
    HI_U8   ucMissReportPlusScore;      // ©��ʱ���ӷַ���
    HI_U8   ucCorrectReportPlusScore;   // ��ȷ�ϱ�ʱ���ӷַ���
    HI_U8   ucNoPassDetectPlusScore;    // ������վ��û��̽��ɹ��ļ�¼ʱ���ӷַ���

    HI_U32  ulNoPassDetectPeriod;       // վ���Ƿ���̽��ɹ���¼ʶ������ʱ������λs
    HI_U32  ulReportMaxDelayTime;       // ��ȷ�ϱ����ϱ�ʱ��-ͣ��ʱ��ܴ�ʱ�䣬��λs

    HI_U16  usMacReportCacheMaxNum;     // MAC�ϱ����л���������
    HI_U8   ucMacReportCacheMaxTime;    // MAC�ϱ����л������ʱ��
    HI_U8   ucScalePowerOffDetectInterval;//��ģͣ��̽����  ��λs

    MRS_CHG_ENTRY_QUEUE  stCache;                // ԭʼ���ݻ���
    MRS_CHG_FRAME_QUEUE  stFrame;                // �����ϱ�����
    MRS_CHG_FRAME_QUEUE stMacCache;     // MAC�ϱ����ݻ���

    MRS_POWEROFF_CHECK_LIST_S * pstCheckStateList;  // ͣ����״̬�б�
    MRS_POWEROFF_HISTORY_LIST_S * pstHistoryList;   // ��ʷ��¼

    HI_U32  ulGetPoweroffInfoSeq;       // ��ȡͣ����Ϣ�������

    HI_U8   is_scale_power_off;         //���ģͣ�綨ʱ������
    HI_U8   pad;
    HI_U16  pad1;

    // ͣ��������� ά�����ͳ����
    HI_U16  usMacReportOffSum;          // �յ�MAC��ͣ���ϱ��ܴ���
    HI_U16  usMacReportOnSum;           // �յ�MAC�������ϱ��ܴ���

    HI_U16  usDetectSuccessSum;         // ̽��ɹ��ܴ���
    HI_U16  usInterceptSum;             // ̽��ʧ�ܱ�ͣ���¼�����ܴ���
    HI_U16  us3762ReportOffSum;         // �ϱ����������ߴ���
    HI_U16  us3762ReportOnSum;          // �ϱ����������ߴ���

    HI_U16  usRecheckFailSum;           // ����ʧ���ܴ���
    HI_U16  usMacReportCorrectSum;      // MAC��ȷ�ϱ��ܴ���
    HI_U16  usMacReportWrongSum;        // MAC���ܴ���
    HI_U16  usMacReportMissSum;         // MAC©���ܴ���
    HI_U16  usDetectReportCorrectSum;   // ���������ǰ����ȷ�ϱ��ܴ���
    HI_U16  usDetectReportWrongSum;     // ���������ǰ�����ܴ���
    HI_U16  usDetectReportMissSum;      // ���������ǰ��©���ܴ���
    HI_U16  us3762ReportCorrectSum;     // ������������ȷ�ϱ��ܴ���
    HI_U16  us3762ReportWrongSum;       // �������������ܴ���
    HI_U16  us3762ReportMissSum;        // ����������©���ܴ���

} MRS_NODE_STATE_CHG_REPORT_CTX;


MRS_NODE_STATE_CHG_REPORT_CTX* mrsGetChgReportCtx(HI_VOID);
HI_VOID mrsCcoNodeStateChgReportInit(HI_VOID);
HI_VOID mrsMACReportChgEntryCallback(HI_IN HI_MDM_NETWORK_TOPOCHG_ENTRY_S* pstEntry);
HI_U32 mrsSaveChgEntry(HI_MDM_NETWORK_TOPOCHG_ENTRY_S* pstEntry);
HI_U32 mrsAssembleAllCacheEntries(HI_OUT HI_U8** ppucFrame, HI_OUT HI_U16* pusFrameLen, HI_OUT HI_U8* pucEntryNum);
HI_VOID mrsOfflineReasonFilter(HI_U8 ucPowerOffReasonSwitch, HI_U8 ucPlcChlReasonSwitch,
                           HI_U8 ucChange, HI_OUT HI_U8* pucReason);
HI_U32 mrsNodeStateChgSendMsg(HI_U32 ulFramePtr, HI_U32 ulFrameLen, HI_U32 ulEntryNum, HI_U32 ulParam4);
HI_U32 mrsNodeStateChgMsgProc(HI_SYS_QUEUE_MSG_S* pstMsg);
HI_U32 mrsChgCacheTimeoutProc(HI_VOID);
HI_U32 mrsChgFrameReport(HI_U8* pucFrame, HI_U16 usFrameLen, HI_U8 ucEntryNum);
HI_U32 mrsChgFrameJoinCcoQueue(HI_VOID);
HI_VOID mrsChgReportRxRespProc(HI_VOID* pParam);
HI_U32 mrsChgReportTimeoutProc(HI_VOID* pParam);
HI_U32 mrsReportNextChgFrame(HI_VOID);

// ��ȡ վ���Ӧ��ͣ����ڵ�
MRS_POWEROFF_CHECK_ITEM_S * mrsGetPoweroffCheckItem(MRS_NODE_STATE_CHG_REPORT_CTX * pstCtx, HI_U8 aucMac[HI_PLC_MAC_ADDR_LEN]);

// ���� MAC���ϱ�վ������
HI_U32 mrsPoweroffHandleNodeOff(MRS_NODE_STATE_CHG_REPORT_CTX * pstCtx, HI_MDM_NETWORK_TOPOCHG_ENTRY_S * pstEntry);
// ���� MAC���ϱ�վ������
HI_U32 mrsPoweroffHandleNodeOn(MRS_NODE_STATE_CHG_REPORT_CTX * pstCtx, HI_MDM_NETWORK_TOPOCHG_ENTRY_S * pstEntry);
// ִ�� ̽������
HI_U32 mrsPoweroffRunDetectProcess(MRS_NODE_STATE_CHG_REPORT_CTX * pstCtx);
// ��� �Ƿ�̽��ʧ��
HI_BOOL mrsPoweroffCheckIsDetectFail(MRS_NODE_STATE_CHG_REPORT_CTX * pstCtx, MRS_POWEROFF_CHECK_ITEM_S * pstItem);
// ���� վ��̽��ʧ��
HI_U32 mrsPoweroffHandleDetectFail(MRS_NODE_STATE_CHG_REPORT_CTX * pstCtx, MRS_POWEROFF_CHECK_ITEM_S * pstItem);
// ִ�� ��������
HI_U32 mrsPoweroffRunRecheckProcess(MRS_NODE_STATE_CHG_REPORT_CTX * pstCtx);
// ִ�� վ���ϱ�����
HI_U32 mrsPoweroffRecheckItem(MRS_NODE_STATE_CHG_REPORT_CTX * pstCtx, MRS_PLC_POWEROFF_INFO_UP_S * pstUpFrame,
                              MRS_POWEROFF_CHECK_ITEM_S * pstItem);
// ���� ��
HI_U32 mrsPoweroffHandleMACReportWrong(MRS_NODE_STATE_CHG_REPORT_CTX * pstCtx, MRS_POWEROFF_CHECK_ITEM_S * pstItem);
// ���� ��ȷ�ϱ�
HI_U32 mrsPoweroffHandleMACReportCorrect(MRS_NODE_STATE_CHG_REPORT_CTX * pstCtx, MRS_POWEROFF_CHECK_ITEM_S * pstItem);
// ���� ©��
HI_U32 mrsPoweroffHandleMACReportMiss(MRS_NODE_STATE_CHG_REPORT_CTX * pstCtx, MRS_POWEROFF_CHECK_ITEM_S * pstItem);

// ���� ��ʷ��¼
HI_U32 mrsPoweroffSaveHistory(MRS_NODE_STATE_CHG_REPORT_CTX * pstCtx, MRS_POWEROFF_CHECK_ITEM_S * pstItem);
// ��� �����ڵ���ȶ���
HI_U32 mrsPoweroffStableCheck(MRS_NODE_STATE_CHG_REPORT_CTX * pstCtx);

// ���� һ����ѯͣ����Ϣ����(������mac_list)
HI_U32 mrsCcoCreatePoweroffInfoFrame(MRS_NODE_STATE_CHG_REPORT_CTX * pstCtx, MRS_PLC_POWEROFF_INFO_DL_S ** pstFrame);
// ���� ��ѯͣ����Ϣ����(Ĭ�Ϲ㲥)
HI_U32 mrsCcoGetPoweroffInfo(MRS_PLC_POWEROFF_INFO_DL_S * pstFrame, HI_U8 aucDstMac[HI_PLC_MAC_ADDR_LEN]);
// �ϱ� ״̬�仯��Ϣ��������
HI_U32 mrsPoweroffReportStateChange(HI_MDM_NETWORK_TOPOCHG_ENTRY_S* pstEntry);

// ��ȡ ̽����վ�����
HI_U16 mrsPoweroffGetDetectingNum(MRS_NODE_STATE_CHG_REPORT_CTX * pstCtx);
// ��ȡ ������վ�����
HI_U16 mrsPoweroffGetRecheckingNum(MRS_NODE_STATE_CHG_REPORT_CTX * pstCtx);

HI_U32 mrsNodeStateChgPowerOffProc(HI_VOID);

//��ѯ�������Ƿ�����ظ�mac�ĵ�
HI_U32 mrsPowerOffCheckFrameQueue(HI_U8 *pstCheckMac);

//������ģͣ�纯��
HI_VOID mrsPowerOffDealScaleOff(HI_VOID);

//����վ����ߴ���
HI_U32 mrsCheckNodeOffByPcoDetect(HI_U8 *poweroff_mac);

#endif
HI_END_HEADER

#endif // __MRS_SRV_CCO_SLAVENODE_ON_OFF_LINE_H__

