//*****************************************************************************
//
//                  ��Ȩ���� (C), 1998-2013, ��Ϊ�������޹�˾
//
//*****************************************************************************
//  �� �� ��   : mrs_srv_sta_searchmeter.h
//  �� �� ��   : V1.0 
//  ��    ��   : cuiate/00233580
//  ��������   : 2013-08-05
//  ��������   : STA�ѱ������ӿ�����
//               
//  �����б�   : TODO: ...
//  �޸���ʷ   : 
//  1.��    �� : 2013-08-05
//    ��    �� : cuiate/00233580
//    �޸����� : �����ļ� 
//
//  2.��    �� : 2014-08-13
//    ��    �� : cuiate/00233580
//    �޸����� : �ع�����I�ɺ�II�ɵĹ���������ȡ���� 
//
//*****************************************************************************

#ifndef _MRS_SRV_STA_SEARCHMETER_H_
#define _MRS_SRV_STA_SEARCHMETER_H_
HI_START_HEADER
#if defined(PRODUCT_CFG_PRODUCT_TYPE_STA)


//*****************************************************************************
// �ṹ����
//*****************************************************************************

// �ѱ��¼�
#define MRS_SEARCH_EVENT_ADDR_CHANGED   (0)

// �ѱ��¼��������
#define MRS_SEARCH_EVENT_MAX            (10)

// NV�б�δ�ѵ��Ĵ������Ϸ�ֵ
#define MRS_MAX_VALID_NOT_FOUND_TIMES   (2)

// RX�����е���������ܵ����ֵ
#define MRS_SEARCH_MAX_METERNUM_IN_RX   (32)

// I��/II���ѱ�ʹ�ܱ�־
#define MRS_SEARCH_ENABLE_FLAG_I        (0x01)
#define MRS_SEARCH_ENABLE_FLAG_II       (0x02)


// �ѱ��¼����
typedef struct 
{
    HI_U32  ulTime;
    HI_U16  usEvent;
    HI_U16  usReserved;
} MRS_SEARCH_EVENT_ITEM;


// �ѱ��¼��ṹ
typedef struct
{
    HI_U8   ucNum;
    HI_U8   aucReserved[3];
    MRS_SEARCH_EVENT_ITEM astItem[MRS_SEARCH_EVENT_MAX];
} MRS_SEARCH_EVENT_STRU;


// �ѱ����ýṹ(��NV���ȡ������)
typedef struct
{
    HI_U32  ulSeq;
    HI_U16  usFrameTimeout;
    HI_U8   ucByteTimeout;
    HI_U8   ucMeterMax;

    HI_U8   aucNvMeter[HI_METER_ADDR_LEN];
    HI_U16  usNvDI97;
    HI_U8   ucNvProtocol;

    HI_U8   ucSearchRetry;

    HI_U8   ucInvalidFrmLen07;
    HI_U8   ucInvalidFrmLen97;

    HI_U32  ulDI07;
    HI_U16  ausDI97[3];

    HI_BOOL bEnableBcSearch97;
    HI_U8   ucExploreByteCountMax;

    HI_U16  usSearchPeriod;

    HI_U8   ucBaudRateIndex;

    HI_U8   bCltiSmEnable  : 1;     // I���ѱ�ʹ��
    HI_U8   bCltiiSmEnable : 1;     // II���ѱ�ʹ��
    HI_U8   ucJSAreaVer    : 1;     // ���հ汾
    HI_U8   pad            : 5;

    HI_U32  ulLastLockSuccSeq;      // ��һ�������ɹ�ʱ���ѱ����к�
} MRS_SEARCH_CFG_STRU;


// �շ�������
typedef struct
{
    HI_U32  ulSize;
    HI_U8   aucBuf[MRS_STA_FRAME645_BUFSIZE_MAX];
} MRS_SEARCH_BUF_RX_STRU, MRS_SEARCH_BUF_TX_STRU;


// �ѱ�����
typedef struct
{
    HI_U32  ulDI;
    HI_U16  usBaudrate;
    HI_U8   ucProtocol;
    HI_U8   ucReserved;
} MRS_SEARCH_ITEM_STRU;

typedef struct
{
    MRS_SRV_NODE stLink;
    MRS_SEARCH_ITEM_STRU stItem;
} MRS_SEARCH_NODE;


typedef struct
{
    HI_U8   ucType;
    HI_U8   ucStart;
    HI_U8   ucCount;
    HI_U8   ucPad;
} MRS_CLTI_SM_QUERY_CTX;

typedef struct
{
    HI_U8   ucTryFirstTimes;
    HI_U8   ucStartRetry;
    HI_U8   ucStopRetry;
    HI_U8   ucQueryFollowRetry;

    MRS_CLTI_SM_QUERY_CTX stCltiQueryCtx;
} MRS_SEARCH_RUNNING_CLT_I;


typedef struct
{
    HI_U8 aucAddr[6];
    HI_U8 aucReserved[2];
}MRS_SEARCH_VERIFY_ITEM;


typedef struct
{
    HI_U8 ucTotal;
    HI_U8 ucVerifyCount;
    HI_U8 aucPad[2];
    
    MRS_SEARCH_VERIFY_ITEM astVerify[MRS_SEARCH_MAX_METERNUM_IN_RX];
}MRS_SEARCH_VERIFY_STRU;


typedef struct
{
    HI_U8 ucLoopValidFrame;
    HI_U8 ucLoopInvalidFrame;
    HI_U8 ucLoopRetry;
    HI_U8 ucPad;

    HI_U8 aucLoopAddr[6];
    HI_U16 usLoopRxSize;
}MRS_SEARCH_LOOP_PARAMS_STRU;


typedef struct
{
    HI_U8   ucRetry;

    HI_BOOL bFrameTimerFlag;
    HI_BOOL bByteTimerFlag;

    HI_U8   ucValidFrame;
    HI_U8   ucInvalidFrame;
    HI_BOOL bEnableBcSearch;

    HI_U8   ucSearchState;
    HI_U8   ucTxProtocol;
    HI_U8   aucTxAddr[HI_METER_ADDR_LEN];

    HI_U8   pad[2];

    HI_U32  ulTxDI;

    HI_U32  ulTxTimeStamp;
    HI_U32  ulRxTimeStamp;

    HI_U8   ucExploreByteCountMax;
    HI_U8   ucExploreByteCount;
    HI_U16  usDefaultDi97;
    
    MRS_SRV_QUEUE stQueue;
    MRS_SEARCH_VERIFY_STRU  stVerify;
    MRS_SEARCH_LOOP_PARAMS_STRU stParams;
} MRS_SEARCH_RUNNING_CLT_II;

typedef struct
{
    HI_U8   ucSearchStatus;
    HI_U8   ucEndReason;
    HI_BOOL bTryFisrtFlag;
    HI_BOOL bFirstOver;

    HI_BOOL bHasCheckJoinAddr;  // �Ƿ����������ַ: �����ϵ�ʱ���
    HI_U8   ucPad[3];

    union
    {
        MRS_SEARCH_RUNNING_CLT_I    stCltI;
        MRS_SEARCH_RUNNING_CLT_II   stCltII;
    } unClt;
} MRS_SEARCH_RUNNING_STRU;


typedef struct
{
    HI_U8   ucMeterNum;
    HI_BOOL bMeterFlag;
    HI_U8   aucReserved[2];
    MRS_METER_ITEM_S astMeter[MRS_MAX_METERNUM];
} MRS_SEARCH_METERLIST_STRU;


typedef struct
{
    HI_U8 ucTotal;
    HI_U8 ucDetectCount;
    HI_U8 ucDetectProtocol;
    HI_U8 ucBaudrateIndex;
    NV_APP_MRS_SM_NODE astNvMeter[MRS_MAX_METERNUM];
}MRS_SEARCH_NV_METERLIST_STRU;


typedef HI_U32 (*mrsSearchStartCmdFunc)(HI_VOID);
typedef HI_U32 (*mrsSearchFinishCmdFunc)(HI_U32 ulOption);
#define MRS_SEARCH_CMD_CALLBACK_MAX    (2)

typedef enum
{
    MRS_SEARCH_CMD_START = 0,
    MRS_SEARCH_CMD_FINISH,
} MRS_SEARCH_CMD_E;

typedef enum
{
    MRS_SEARCH_FINISH_NORMAL = 0,
    MRS_SEARCH_FINISH_ABNORMAL
} MRS_SEARCH_FINISH_REASON_E;

typedef struct
{
    mrsSearchStartCmdFunc pfStartNotify;
    mrsSearchFinishCmdFunc pfFinishNotify;
} MRS_SEARCH_STATUS_NOTIFY_INF;

#define MRS_STA_NOTIFY_MAX          (2)

#define MRS_STA_NOTIFY_ID_EVENT     (0)
#define MRS_STA_NOTIFY_ID_MULTICONN (1)

typedef struct
{
    MRS_SEARCH_RUNNING_STRU stRunning;
    MRS_SEARCH_BUF_RX_STRU  stRx;
    MRS_SEARCH_BUF_TX_STRU  stTx;
    MRS_SEARCH_CFG_STRU     stSearchCfg;
    MRS_SEARCH_EVENT_STRU   stEvent;
    MRS_SEARCH_METERLIST_STRU   stMeterList;
    MRS_DFX_SEARCH_FRAME_STATS_STRU stFrameStats;
    MRS_SEARCH_NV_METERLIST_STRU stNvMeterList;
    MRS_SEARCH_STATUS_NOTIFY_INF stStatusNotify[MRS_STA_NOTIFY_MAX];
} MRS_STA_SEARCH_CTX;


// [0:1]: ���ܱ��Լ����(0͸����Լ 1-97��Լ 2-07��Լ ����-����) 
// [2:3]: ���ܱ�����������2λ 
// [4:6]: �ѱ�ʱ����δ�ѵ��Ĵ���������3��δ�ѵ��ı���б���ɾ��
// [7:7]: ���ܱ�����������1λ
typedef struct
{
    HI_U8 ucProtocol : 2;
    HI_U8 ucBrIdxLo  : 2;
    HI_U8 ucTimes    : 3;
    HI_U8 ucBrIdxHi  : 1;
} MRS_NV_SEARCH_PARAM_S;


//*****************************************************************************
// ��������
//*****************************************************************************

// �ѱ�������
MRS_STA_SEARCH_CTX *mrsStaSearchCtx(HI_VOID);

// �ѱ�ģ���ʼ��
HI_U32 mrsStaSearchInit(HI_VOID);

// �ѱ����ó�ʼ��
HI_U32 mrsStaSearchCfgInit(HI_VOID);

// �����ѱ������ĺ�ͳ����
HI_VOID mrsStaSearchDfxReset(HI_VOID);

// ��ʼ�ѱ�
HI_U32 mrsStaSearchStart(HI_BOOL bTryFirstFlag);

// ֹͣ�ѱ�
HI_U32 mrsStaSearchStop(HI_VOID);

// ����ѱ����б�
HI_U32 mrsStaSearchClearMeterList(MRS_STA_SEARCH_CTX *pstCtx);


HI_U32 mrsStaSearchLoadNvResult(MRS_SEARCH_NV_METERLIST_STRU* pstNvList);

HI_U32 mrsStaSearchGetValidNvNodes(HI_U16 usId, MRS_SEARCH_NV_METERLIST_STRU * pstNvList);

HI_BOOL mrsStaSearchIsNvNodeValid(NV_APP_MRS_SM_NODE* pstNode);

HI_U8 mrsStaSearchGetNodeProtocol(NV_APP_MRS_SM_NODE* pstNode);

HI_U8 mrsStaSearchGetNotFoundTimes(NV_APP_MRS_SM_NODE* pstNode);

HI_U32 mrsStaSearchNvNodeInsert(MRS_SEARCH_NV_METERLIST_STRU* pstNvList, NV_APP_MRS_SM_NODE* pstNode);

HI_U32 mrsStaSearchAppendNvNode(MRS_SEARCH_NV_METERLIST_STRU* pstNvList, NV_APP_MRS_SM_NODE* pstNode);

HI_U32 mrsStaRefreshSearchResultNv(MRS_STA_SEARCH_CTX *pstCtx, HI_BOOL bSearchFinish);

HI_VOID mrsStaSearchGetResult(MRS_SEARCH_NV_METERLIST_STRU* pstNvList, MRS_SEARCH_METERLIST_STRU *pstSmList);

HI_VOID mrsStaSearchNotFoundTimesIncrease(NV_APP_MRS_SM_NODE* pstNode);

HI_U32 mrsStaSearchWriteToNv(MRS_SEARCH_NV_METERLIST_STRU* pstNvList);

HI_U32 mrsStaSearchAddRmMeter(HI_U8* pucAddr, HI_U8 ucProtocol, HI_U16 usDi97);


// ��ӵ���ѱ����б�
HI_U32 mrsStaSearchMeterInsert(MRS_STA_SEARCH_CTX *pstCtx, MRS_METER_ITEM_S *pstItem);

// ����ѵ����׿��
HI_VOID mrsStaSearchCheckFirstMeterAddr(MRS_STA_SEARCH_CTX *pstCtx);

// ֹͣII�ɱ����ѱ�
HI_VOID mrsStopCltIISearchMeter(HI_VOID);

// ���������ӵ��ѱ����б���
HI_U32 mrsStaVMAddr2MeterList(HI_U8 *pucMeter, HI_U8 ucProtocol);

// ʹ���ѱ���ˢ�±��ص�ַ�б�
HI_U32 mrsStaRefreshLocalMeterList(MRS_METER_LIST_S *, MRS_SEARCH_METERLIST_STRU *);

// ���ѱ����в���ָ������ַ
// pucMeter: ����ַ
// ucProtocol: ��Լ����
// bReplace: ���97���µ�ַAAH���٣��Ƿ����µ���滻�ϵ��
// usDi97: 97���ѱ�DI��bReplace=HI_TRUEʱ��Ч
HI_BOOL mrsStaFindMeterOfSearchResult(HI_U8 *pucMeter, HI_U8 ucProtocol, HI_BOOL bReplace, HI_U16 usDi97);

HI_BOOL mrsStaIsMeterInSearchResult(MRS_METER_ITEM_S *pstMeterInf, HI_BOOL bReplace);

HI_U32 mrsStaSetMeterListToMac(MRS_SEARCH_METERLIST_STRU *, HI_U8);

HI_BOOL mrsStaAllowAddMeter(HI_VOID);

HI_VOID mrsStaSearchSetStatus(HI_U8 ucStatus);

HI_U8 mrsStaSearchGetStatus(HI_VOID);

HI_U8 mrsStaSearchEndReason(HI_VOID);

HI_VOID mrsStaSearchSetEndReason(HI_U8 ucReason);

HI_VOID mrsStaSearchSetFirstFlag(HI_BOOL bFlag);

HI_BOOL mrsStaSearchGetFirstFlag(HI_VOID);

HI_VOID mrsStaSearchSetHasCheckJoinAddrFlag(HI_BOOL bFlag);

HI_BOOL mrsStaSearchGetHasCheckJoinAddrFlag(HI_VOID);

HI_U16 mrsStaSearchGetBaudrate(HI_U8 ucProtocol);

HI_VOID mrsStaRefreshDefaultDi97(HI_VOID);

HI_U16 mrsStaGetDefaultDi97(HI_VOID);


HI_U32 mrsStaSearchRegisterCmdNotify(mrsSearchStartCmdFunc pfStartNotify, mrsSearchFinishCmdFunc pfFinishNotify, HI_U8 ucId);

HI_U32 mrsStaSearchStartNotify(HI_VOID);

HI_U32 mrsStaSearchFinishNotify(HI_U32 ulOption);

MRS_SEARCH_METERLIST_STRU * mrsStaSearchResultList(HI_VOID);

HI_U32 mrsFindMeterIndexOfSearchResult(HI_U8 pucMeter[], HI_U16 *pusIndex);
HI_U32 mrsCltGetNvSMResult(HI_OUT HI_PVOID *ppData);
HI_VOID mrsStaSearchSetMeter2Mac(HI_U8* pucMeter, HI_U8 ucProtocol, HI_U16 us97DI);
HI_U32 mrsAssetNumberAsMeterInsert(HI_VOID);


//*****************************************************************************
//*****************************************************************************


#endif //defined(PRODUCT_CFG_PRODUCT_TYPE_STA)
HI_END_HEADER
#endif //_MRS_SRV_STA_SEARCHMETER_H_

