//*****************************************************************************
//
//                  版权所有 (C), 1998-2013, 华为技术有限公司
//
//*****************************************************************************
//  文 件 名   : mrs_srv_sta_searchmeter.h
//  版 本 号   : V1.0
//  作    者   : cuiate/00233580
//  生成日期   : 2013-08-05
//  功能描述   : STA搜表函数及接口声明
//
//  函数列表   : TODO: ...
//  修改历史   :
//  1.日    期 : 2013-08-05
//    作    者 : cuiate/00233580
//    修改内容 : 创建文件
//
//  2.日    期 : 2014-08-13
//    作    者 : cuiate/00233580
//    修改内容 : 重构，把I采和II采的公共部分提取出来
//
//*****************************************************************************

#ifndef _MRS_SRV_STA_SEARCHMETER_H_
#define _MRS_SRV_STA_SEARCHMETER_H_
HI_START_HEADER
#if defined(PRODUCT_CFG_PRODUCT_TYPE_STA)


//*****************************************************************************
// 结构定义
//*****************************************************************************

// 搜表事件
#define MRS_SEARCH_EVENT_ADDR_CHANGED   (0)

// 搜表事件最大数量
#define MRS_SEARCH_EVENT_MAX            (10)

/* BEGIN: PN: DTS2015051601564 MODIFY\ADD\DEL by cuiate/00233580 at 2015/5/11 */
// 搜表结点最大数量: 07规约 97规约条读、块读
#define MRS_SEARCH_ITEM_MAX             (4)
/* END:   PN: DTS2015051601564 MODIFY\ADD\DEL by cuiate/00233580 at 2015/5/11 */

// NV中表未搜到的次数最大合法值
#define MRS_MAX_VALID_NOT_FOUND_TIMES   (2)

/* BEGIN: PN: DTS2015051601564 MODIFY\ADD\DEL by cuiate/00233580 at 2015/5/11 */
// RX缓存中电表数量可能的最大值
#define MRS_SEARCH_MAX_METERNUM_IN_RX   (32)
/* END:   PN: DTS2015051601564 MODIFY\ADD\DEL by cuiate/00233580 at 2015/5/11 */


// 搜表事件结点
typedef struct
{
    HI_U32  ulTime;
    HI_U16  usEvent;
    HI_U16  usReserved;
} MRS_SEARCH_EVENT_ITEM;


// 搜表事件结构
typedef struct
{
    HI_U8   ucNum;
    HI_U8   aucReserved[3];
    MRS_SEARCH_EVENT_ITEM astItem[MRS_SEARCH_EVENT_MAX];
} MRS_SEARCH_EVENT_STRU;


// 搜表配置结构(从NV项读取的数据)
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
    HI_U8   ucPlcTh;

    HI_U8   ucQueryTh;
    HI_U8   reserved[3];
} MRS_SEARCH_CFG_STRU;


// 收发缓冲区
typedef struct
{
    HI_U32  ulSize;
    HI_U8   aucBuf[MRS_STA_FRAME645_BUFSIZE_MAX];
} MRS_SEARCH_BUF_RX_STRU, MRS_SEARCH_BUF_TX_STRU;


// 搜表项结点
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
} MRS_SEARCH_LOOP_PARAMS_STRU;


typedef struct
{
    HI_U8 aucMeter[APP_CSG_CMD_DL_METERLIST_MAX][HI_METER_ADDR_LEN];
    HI_U8 ucMeterNum;
    HI_U8 ucMeterIdx;
    HI_U8 bNeedResume : 1;
    HI_U8 bStartFlag  : 1;
    HI_U8 reserved    : 6;
    HI_U8 reserved1;
    MRS_SRV_QUEUE stQueue;
} MRS_SEARCH_DETECT_STRU;


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

    HI_U32  ulTxDI;

    HI_U32  ulTxTimeStamp;
    HI_U32  ulRxTimeStamp;

    HI_U8   ucExploreByteCountMax;
    HI_U8   ucExploreByteCount;
    HI_U16  usDefaultDi97;

    MRS_SRV_QUEUE stQueue;
    MRS_SEARCH_VERIFY_STRU  stVerify;
    MRS_SEARCH_LOOP_PARAMS_STRU stParams;

    MRS_SEARCH_DETECT_STRU  stDetect;
} MRS_SEARCH_RUNNING_CLT_II;

typedef struct
{
    HI_U8   ucSearchStatus;
    HI_U8   ucEndReason;
    HI_BOOL bTryFisrtFlag;
    HI_BOOL bFirstOver;

    HI_BOOL bHasCheckJoinAddr;  // 是否检查过入网地址: 仅在上电时检查
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
    MRS_SEARCH_STATUS_NOTIFY_INF stStatusNotify;
} MRS_STA_SEARCH_CTX;


// [0:1]: 电能表规约类型(0透明规约 1-97规约 2-07规约 其它-保留)
// [2:3]: 电能表波特率索引低2位
// [4:6]: 搜表时连续未搜到的次数，连续3次未搜到的表从列表中删除
// [7:7]: 电能表波特率索引高1位
typedef struct
{
    HI_U8 ucProtocol : 2;
    HI_U8 ucBrIdxLo  : 2;
    HI_U8 ucTimes    : 3;
    HI_U8 ucBrIdxHi  : 1;
} MRS_NV_SEARCH_PARAM_S;


//*****************************************************************************
// 函数定义
//*****************************************************************************

// 搜表上下文
MRS_STA_SEARCH_CTX *mrsStaSearchCtx(HI_VOID);

// 搜表模块初始化
HI_U32 mrsStaSearchInit(HI_VOID);

// 搜表配置初始化
HI_U32 mrsStaSearchCfgInit(HI_VOID);

// 重置搜表上下文和统计量
HI_VOID mrsStaSearchDfxReset(HI_VOID);

// 开始搜表
HI_U32 mrsStaSearchStart(HI_BOOL bTryFirstFlag);

// 停止搜表
HI_U32 mrsStaSearchStop(HI_VOID);

// 清空搜表结果列表
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

/* BEGIN: PN: DTS2015051601564 MODIFY\ADD\DEL by cuiate/00233580 at 2015/5/15 */
HI_U32 mrsStaSearchAddRmMeter(HI_U8* pucAddr, HI_U8 ucProtocol, HI_U16 usDi97);
/* END:   PN: DTS2015051601564 MODIFY\ADD\DEL by cuiate/00233580 at 2015/5/15 */


// 添加电表到搜表结果列表
HI_U32 mrsStaSearchMeterInsert(MRS_STA_SEARCH_CTX *pstCtx, MRS_METER_ITEM_S *pstItem);

// 检查搜到的首块表
HI_VOID mrsStaSearchCheckFirstMeterAddr(MRS_STA_SEARCH_CTX *pstCtx);

// 停止II采本地搜表
HI_VOID mrsStopCltIISearchMeter(HI_VOID);

// 把虚拟表添加到搜表结果列表中
HI_U32 mrsStaVMAddr2MeterList(HI_U8 *pucMeter, HI_U8 ucProtocol);

// 使用搜表结果刷新本地地址列表
HI_U32 mrsStaRefreshLocalMeterList(MRS_METER_LIST_S *, MRS_SEARCH_METERLIST_STRU *);

// 在搜表结果中查找指定电表地址
// pucMeter: 电表地址
// ucProtocol: 规约类型
// bReplace: 如果97表新地址AAH更少，是否用新电表替换老电表
// usDi97: 97表搜表DI，bReplace=HI_TRUE时有效
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

HI_VOID mrsStaRefreshDefaultDi97(HI_VOID);

HI_U16 mrsStaGetDefaultDi97(HI_VOID);


HI_U32 mrsStaSearchRegisterCmdNotify(mrsSearchStartCmdFunc pfStartNotify, mrsSearchFinishCmdFunc pfFinishNotify);

HI_U32 mrsStaSearchStartNotify(HI_VOID);

HI_U32 mrsStaSearchFinishNotify(HI_U32 ulOption);

MRS_SEARCH_METERLIST_STRU * mrsStaSearchResultList(HI_VOID);

HI_U32 mrsFindMeterIndexOfSearchResult(HI_U8 pucMeter[], HI_U16 *pusIndex);
HI_U32 mrsCltGetNvSMResult(HI_OUT HI_PVOID *ppData);
HI_VOID mrsStaSearchSetMeter2Mac(HI_U8* pucMeter, HI_U8 ucProtocol, HI_U16 us97DI);


HI_BOOL mrsStaAllowSearch(hi_void);

HI_VOID mrsStaStartPeriodSearch(MRS_STA_SEARCH_CTX *pstCtx);

#if defined(PRODUCT_CFG_PRODUCT_TYPE_CSG_GD)
HI_VOID mrsSetMacTimeout(HI_VOID);
HI_VOID mrsCsgSetCltAddr2Mac(HI_VOID);
#endif


//*****************************************************************************
//*****************************************************************************


#endif //defined(PRODUCT_CFG_PRODUCT_TYPE_STA)
HI_END_HEADER
#endif //_MRS_SRV_STA_SEARCHMETER_H_

