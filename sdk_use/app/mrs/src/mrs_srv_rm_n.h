//*****************************************************************************
//
//                  ��Ȩ���� (C), 1998-2011, ��Ϊ�������޹�˾
//
//*****************************************************************************
//  �� �� ��   : mrs_srv_rm_n.h
//  ��    ��   : liuxipeng/KF54842
//  ��������   : 2014-03-20
//  ��������   : �������ģ���ڲ������������궨��(����mrs_srv_readmeter_manage.c�ļ�ʹ��)
//               
//  �����б�   :
//  �޸���ʷ   : 
//  1.��    �� : 2014-03-20
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

#ifndef __MRS_SRV_RM_N_H__
#define __MRS_SRV_RM_N_H__

HI_START_HEADER

#include "mrs_srv_readmeter_manage.h"

#if defined(PRODUCT_CFG_PRODUCT_TYPE_CCO)

// ����ģ�鳬ʱ��鶨ʱ������
#define MRS_RM_CHECK_TIMER_UNIT_BASE MRS_CCO_TIME_PARALLEL_CHECK

/* BEGIN: Modified by liuxipeng/lKF54842,2014/4/29 ���ⵥ��:DTS2014042306517 */
// ����ģ�����л�ʱ�����
#define MRS_RM_METER_SWITCH_TIME_UNIT (10)  // ����
/* END  : Modified by liuxipeng/lKF54842,2014/4/29 ���ⵥ��:DTS2014042306517 */

// ֧�ֵ����ע���������
#define MRS_RM_STRATEGY_REG_MAX_NUM    MRS_RM_TYPE_MAX_NUM

// ��ʱ��鶨ʱ��״̬
#define MRS_RM_TIMER_INACTIVE       (0)    // δ����
#define MRS_RM_TIMER_ACTIVE         (1)    // ��������

// ����ģʽ����
#define MRS_RM_NORMAL_MODE          (0)    // ����ģʽ(��ģʽ�°�����󲢷������ͷǹ㲥֡)
#define MRS_RM_BROADCAST_MODE       (1)    // �㲥ģʽ(��ģʽ��ֻ����㲥֡-Ŀǰ���ƹ㲥����Ϊ1)

// �ڵ����״̬
#define MRS_RM_NODE_UNTREATED       (1)    // δ�������
#define MRS_RM_NODE_UNINITIALIZED   (2)    // �ڵ�������δ��ʼ��
/* BEGIN: Modified by liuxipeng/lKF54842,2014/4/29 ���ⵥ��:DTS2014042306517 */
#define MRS_RM_NODE_WAIT_SEND       (3)    // �ȴ�����(���ڴ�״̬�����һ��ʱ���������)  
#define MRS_RM_NODE_SEND_REQ        (4)    // ������
#define MRS_RM_NODE_WAIT_RESP       (5)    // �ȴ�Ӧ��  
/* END  : Modified by liuxipeng/lKF54842,2014/4/29 ���ⵥ��:DTS2014042306517 */

// ��������Ĭ�ϲ���
#define MRS_PARALLEL_NUM_DEFAULT    (5)     // ��󲢷���Ĭ��ֵ
#define MRS_FRAME_NUM_DEFAULT       (32)    // ��󵥴δ�������������Ĭ��ֵ
#define MRS_ITEM_NUM_DEFAULT        (64)    // ������������Ŀ����Ĭ��ֵ

#define MRS_PARALLEL_NUM_MAX        (1024)  // ��󲢷������ֵ
#define MRS_FRAME_NUM_MAX           (100)   // ��󵥴δ����������������ֵ
#define MRS_ITEM_NUM_MAX            (1024)  // ������������Ŀ�������ֵ

#define MRS_PARALLEL_NUM_MIN        (1)     // ��󲢷�����Сֵ
#define MRS_FRAME_NUM_MIN           (1)     // ��󵥴δ���������������Сֵ
#define MRS_ITEM_NUM_MIN            (1)     // ������������Ŀ������Сֵ

/* BEGIN: Modified by liuxipeng/lKF54842,2014/4/26 ���ⵥ��:DTS2014042407354 */
#define MRS_RM_CRC_MATCH            (0)     // ʹ��CRCƥ��
#define MRS_RM_ADDR_MATCH           (1)     // ʹ�ñ��ַƥ��
/* END  : Modified by liuxipeng/lKF54842,2014/4/26 ���ⵥ��:DTS2014042407354 */

typedef struct
{
    HI_U16 usRmType;            // ҵ��������� 
    HI_U8 ucRetryCfgNo  : 4;    // �ش����ú� (MRS_RETRY_CFG_NUM_ENUM)
    HI_U8 ucRetryStrategy : 4;    // �ش����Ժ� (MRS_RM_RETRY_STRATEGY_ENUM)
    HI_U8 ucPoolNo      : 4;    // �����غ� (MRS_PARALLEL_POOL_ENUM)
    HI_U8 padding       : 4;
    MRS_RM_DATA_RESP_PROC_F pfnRespProc;    // ҵ���������Ӧ����ص�����
} MRS_RM_TACTIC_REG, *P_MRS_RM_STRATEGY_REG;

// ��������Ԫ�ؽṹ(����غ���)
typedef struct 
{
    MRS_SRV_NODE link;
    HI_U8  ucMeterAddr[HI_METER_ADDR_LEN];   // Ŀ�ĵ���ַ
    HI_U8  ucMrSeq;          // ҵ�����376.2֡���к�
    HI_U8  ucStrategyNo : 4;   // ������Ժ�
    HI_U8  ucProtocol : 4;   // ��Լ����
    HI_U8  ucFrameNum : 7;   // ����ת��֡����(pucData��֡����)
    HI_U8  bCtrlCmd   : 1;   // ����ת��֡���Ƿ���ڷѿ�����
    HI_U8  reserved;
    HI_U16 usDataLen;    // ҵ���������ת������
/* BEGIN: Modified by liuxipeng/lKF54842,2014/4/26 ���ⵥ��:DTS2014042407354 */    
    HI_U32 ulDataCrc;    // ҵ���������CRC
/* END  : Modified by liuxipeng/lKF54842,2014/4/26 ���ⵥ��:DTS2014042407354 */    
    HI_U8  pucData[0];   // ҵ���������ת������
} MRS_RM_ITEM, *P_MRS_RM_ITEM;

// �����ز����ڵ�������
typedef struct 
{
    HI_U32 ulPlcSeq;        // PLC֡���
    HI_U32 ulOverTimeTotal; // ��ʱʱ��(һ��PLCͨѶ����ʱ,����STA�˺�ʱ) ��λ:ms
    HI_U32 ulOverTimeMeter; // STA�˵��ʱʱ��
    HI_U32 ulRunOutTime;    // ��ǰPLC����������ʱ��(����PLC֡���ʱ�俪ʼ�ۻ�-�����ڵ���ulOverTimeTotal���ʾPLC��ʱ)��λ:ms
/* BEGIN: Modified by liuxipeng/lKF54842,2014/4/29 ���ⵥ��:DTS2014042306517 */    
    HI_U32 ulWaitSendTime;  // ��ǰ�ȴ�����������ʱ��(�����к���Ԫ��׼������ʱ��ʼ�ۻ�-�����ڵ���"����л����"ʱ���������)��λ:ms
/* END  : Modified by liuxipeng/lKF54842,2014/4/29 ���ⵥ��:DTS2014042306517 */
    HI_U8  ucMaxTry;        // ��ǰ��������ش�����
    HI_U8  ucTry;           // ��ǰ�����ش�����
    HI_U8  ucTryLoop;       // �ش��ִ�("�㳭"��Ч)
    HI_U8  ucIsBC;          // �㲥֡��־(���͹㲥PLC֡ʱ�˱�־��HI_TRUE)
    MRS_PLC_FRAME_DATA_STRU stPlcData; // �����͵�PLC��������֡�ṹ
} MRS_NODE_CTX, *P_MRS_NODE_CTX;

// ��������ڵ�ṹ(���������)
typedef struct 
{
    MRS_SRV_LINK link;
    HI_U8 ucDestMac[HI_PLC_MAC_ADDR_LEN];   // Ŀ��վ��MAC��ַ
    HI_U8 ucState;      // �ڵ�״̬(MRS_RM_NODE_SEND_REQ : MRS_RM_NODE_WAIT_RESP)
    HI_U8 ucItemNum;    // ���ڵ�Ԫ�ظ���
    P_MRS_NODE_CTX pstNodeCtx;  // �����ڵ�������(ֻ�ڸýڵ㴦��ʱ�Ž��г�ʼ����ʹ��)
    MRS_SRV_QUEUE  stItemQueue;  // �����ҵ�������� ָ��MRS_RM_ITEM�ṹ��
} MRS_RM_NODE, *P_MRS_RM_NODE;

// �����ؽṹ
typedef struct 
{
    HI_U8 ucMode;           // ����ع���ģʽ(MRS_RM_NORMAL_MODE : MRS_RM_BROADCAST_MODE)
    HI_U8 padding[3];
    HI_U16 usParallelNum;   // ��ǰ������
    HI_U16 usItemTotalNum;  // ��ǰ�ܵ�ҵ�������������
    MRS_PARALLEL_CFG_S stParallelCfg;
    MRS_SRV_LIST stNodeList;     // ����� ָ��MRS_RM_NODE�ṹ
} MRS_RM_REQ_POOL, *P_MRS_RM_REQ_POOL;

// �������ģ��ṹ
typedef struct 
{
    MRS_RETRY_CFG_S stRetryCfg[MRS_RETRY_CFG_MAX_NUM];
    MRS_RM_TACTIC_REG stStrategyReg[MRS_RM_STRATEGY_REG_MAX_NUM];
    HI_U32 ulPlcSeq;
    HI_U8  ucTimerStatus;
    HI_U8  padding[3];
    MRS_RM_REQ_POOL stPool[MRS_RM_POOL_MAX_NUM];
	MRS_SRV_LIST stParallelRmList;
} MRS_READMETER_MODULE, *P_MRS_READMETER_MODULE;


HI_PRVL P_MRS_READMETER_MODULE mrsRmGetModuleCtx(HI_VOID);

HI_PRV P_MRS_RETRY_CFG_S mrsRmGetRetryCfg(HI_U8);

HI_PRV P_MRS_RM_REQ_POOL mrsRmGetPool(HI_U8);

HI_PRV P_MRS_RM_STRATEGY_REG mrsRmGetStrategy(HI_U8);

HI_PRV HI_U32 mrsRmGetPlcSeq(HI_VOID);

HI_PRV HI_VOID mrsRmPlcSeqAddSelf(HI_VOID);

HI_PRVL HI_U8 mrsRmGetTimerStatus(HI_VOID);

HI_PRVL HI_VOID mrsRmSetTimerStatus(HI_U8);

HI_PRVL HI_VOID mrsRmSetPoolMode(P_MRS_RM_REQ_POOL, HI_U8);

HI_PRVL HI_VOID mrsRmRetryCfgValidtyCheck(P_MRS_RETRY_CFG_S);

HI_PRV HI_VOID mrsRmParallelCfgInit(P_MRS_PARALLEL_CFG_S, P_NV_MRS_PARALLEL_CFG_S);

HI_PRV HI_U32 mrsRmFindStrategy(HI_U16, P_MRS_RM_STRATEGY_REG *, HI_U8 *);
    
HI_PRV P_MRS_RM_NODE mrsRmAddNode(P_MRS_RM_REQ_POOL, HI_U8 *);

HI_PRV HI_U32 mrsRmAddItem(P_MRS_RM_NODE, P_MRS_RM_REQ_DATA, HI_U8, MRS_645BUF_INF *);

/* BEGIN: Modified by liuxipeng/lKF54842,2014/4/29 ���ⵥ��:DTS2014042306517 */
HI_PRV HI_U32 mrsRmItemDeQueue(P_MRS_RM_NODE, HI_BOOL);
/* END  : Modified by liuxipeng/lKF54842,2014/4/29 ���ⵥ��:DTS2014042306517 */

HI_PRV P_MRS_RM_NODE mrsRmFindNode(P_MRS_RM_REQ_POOL, HI_U8 *);

/* BEGIN: Modified by liuxipeng/lKF54842,2014/4/26 ���ⵥ��:DTS2014042407354 */
HI_PRV HI_BOOL mrsRmFindReqOfQueue(MRS_SRV_QUEUE *, HI_U32, HI_U8);

HI_PRV HI_BOOL mrsRmFindReqOfPool(P_MRS_RM_REQ_POOL, HI_U32, HI_U8);
/* END  : Modified by liuxipeng/lKF54842,2014/4/26 ���ⵥ��:DTS2014042407354 */

HI_PRVL HI_VOID mrsRmQueueFree(HI_VOID *);

HI_PRV HI_VOID mrsRmNodeFree(HI_VOID *, HI_VOID *);

HI_PRV HI_VOID mrsRmEmptyPool(P_MRS_RM_REQ_POOL);

HI_PRV HI_VOID mrsRmRemoveNode(MRS_SRV_LIST *, P_MRS_RM_NODE);

HI_PRV HI_U32 mrsRmNodeCtxCreate(P_MRS_RM_NODE);

HI_PRV HI_VOID mrsRmNodeCtxReset(P_MRS_RM_NODE);

HI_PRV HI_VOID mrsRmNodeCtxFree(P_MRS_NODE_CTX);

/* BEGIN: Modified by liuxipeng/lKF54842,2014/4/29 ���ⵥ��:DTS2014042306517 */
HI_PRV HI_U32 mrsRmNodeCtxInit(P_MRS_RM_NODE, HI_BOOL);
/* END  : Modified by liuxipeng/lKF54842,2014/4/29 ���ⵥ��:DTS2014042306517 */

HI_PRVL HI_VOID mrsRmSetupTransmitData(P_MRS_RETRY_CFG_S, P_MRS_RM_ITEM, P_MRS_NODE_CTX, MRS_DATA_TRANSMIT_AFNF1_DL_STRU *);

HI_PRV HI_U32 mrsRmPlcPackCreate(P_MRS_RM_NODE, P_MRS_RM_ITEM, P_MRS_RETRY_CFG_S);

HI_PRV HI_VOID mrsRmGetDestMacAttr(HI_U8 *, HI_U8 *, HI_BOOL *);

HI_PRV HI_VOID mrsRmNodeRetryParamInit(P_MRS_RM_NODE, P_MRS_RETRY_CFG_S, HI_BOOL);

HI_PRV HI_U32 mrsRmSendPlcPackage(P_MRS_NODE_CTX);

HI_PRV HI_U32 mrsRmAllowJoinPool(P_MRS_RM_REQ_POOL, P_MRS_RM_REQ_DATA, MRS_645BUF_INF *);

HI_PRV HI_U32 mrsRmSendReqProcActive(P_MRS_RM_REQ_POOL);

HI_PRV HI_BOOL mrsRmNodeStatusProc(MRS_SRV_LIST *, P_MRS_RM_NODE);

HI_PRV HI_U32 mrsRmReturnRespData(P_MRS_RM_STRATEGY_REG, P_MRS_RM_ITEM, HI_U8 *, HI_U16, HI_U8 ucMacAdd[HI_PLC_MAC_ADDR_LEN] );

HI_PRV HI_U32 mrsRmPlcRetryProcStrategy01(P_MRS_RM_NODE, P_MRS_RETRY_CFG_S);

HI_PRV HI_U32 mrsRmPlcRetryProcStrategy02(P_MRS_RM_NODE, P_MRS_RETRY_CFG_S);

HI_PRV HI_U32 mrsRmPlcOverTimeProc(P_MRS_RM_REQ_POOL, P_MRS_RM_NODE);

/* BEGIN: Modified by liuxipeng/lKF54842,2014/4/29 ���ⵥ��:DTS2014042306517 */
HI_PRV HI_U32 mrsRmPlcOverTimeCheck(P_MRS_RM_REQ_POOL, P_MRS_RM_NODE, HI_BOOL *);

HI_PRV HI_U32 mrsRmPlcSendWaitCheck(P_MRS_RM_REQ_POOL, P_MRS_RM_NODE, HI_U32 *, HI_BOOL *);
/* END  : Modified by liuxipeng/lKF54842,2014/4/29 ���ⵥ��:DTS2014042306517 */

HI_PRV HI_U32 mrsRmPoolPlcRxProc(P_MRS_RM_REQ_POOL, HI_U8 *, MRS_DATA_TRANSMIT_AFNF1_UP_STRU *, HI_BOOL *, MRS_DEVICE_INF_STRU *, HI_U8 ucMacAdd[HI_PLC_MAC_ADDR_LEN]);

/* BEGIN: Modified by liuxipeng/lKF54842,2014/4/29 ���ⵥ��:DTS2014042306517 */
HI_PRV HI_U32 mrsRmPoolCheckTimerProc(P_MRS_RM_REQ_POOL, HI_U32 *, HI_U32 *);
/* END  : Modified by liuxipeng/lKF54842,2014/4/29 ���ⵥ��:DTS2014042306517 */

HI_PRV HI_U32 mrsRmPlcUpCompatibleProc(MRS_DATA_TRANSMIT_AFNF1_UP_STRU *, HI_U16, HI_PBYTE *);

HI_PRV HI_U32 mrsRmPlcRespProc(P_MRS_RM_NODE, HI_U8 *, MRS_DATA_TRANSMIT_AFNF1_UP_STRU *, MRS_DEVICE_INF_STRU *,HI_U8 ucMacAdd[HI_PLC_MAC_ADDR_LEN]);

#endif

HI_END_HEADER

#endif

