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

typedef struct
{
    HI_U16  usNum;                       // ����Ԫ�ظ���,U16
    HI_U8   aucPad[2];                   // ����λ
    MRS_SRV_QUEUE stQueue;               // ����
} MRS_CHG_FRAME_QUEUE;

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
    HI_U8   aucPad[2];                           // ����λ
    MRS_CHG_ENTRY_QUEUE  stCache;                // ԭʼ���ݻ���
    MRS_CHG_FRAME_QUEUE  stFrame;                // �����ϱ�����
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


#endif
HI_END_HEADER

#endif // __MRS_SRV_CCO_SLAVENODE_ON_OFF_LINE_H__

