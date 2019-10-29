//*****************************************************************************
//
//                  版权所有 (C), 1998-2015, 华为技术有限公司
//
//*****************************************************************************
//  文 件 名   : mrs_srv_cco_slavenode_on_off_line.h
//  版 本 号   : V1.0
//  作    者   : liupeidong/00238134
//  生成日期   : 2015-03-10
//  功能描述   : CCO载波从节点的上线离线状态上报函数的声明、相关数据结构、宏定义
//
//  函数列表   : TODO: ...
//  修改历史   :
//  1.日    期 : 2015-03-10
//    作    者 : liupeidong/00238134
//    修改内容 : 创建文件
//
//*****************************************************************************
#ifndef __MRS_SRV_CCO_SLAVENODE_ON_OFF_LINE_H__
#define __MRS_SRV_CCO_SLAVENODE_ON_OFF_LINE_H__

HI_START_HEADER
#if defined(PRODUCT_CFG_PRODUCT_TYPE_CCO)

#define MRS_TIME_100_MILLI_SEC             (100)     // 100毫秒

#define MRS_NODE_STATE_CHG_DATA_HEADER_LEN (2+1+2)   // 从节点状态变化帧数据域头: 上报总数+本帧总数+本帧第一个起始序号
#define MRS_ONE_NODE_STATE_CHG_INFO_LEN    (6+1+4+1) // 单个从节点状态变化信息的长度: MAC地址+状态变化+离线时长+离线原因

#define MRS_ONLINE_TO_OFFLINE_DURATION_DEFAULT   (0xEEEEEEEE)

#define MRS_DEAL_SCALE_OFF_INTERVAL         (100)      //大规模停电处理时，判定是否为本次上报探测成功时间间隔为s
#define MRS_DETECT_PCO_SUCCESS_MAX_INTERVAL (420)      //探测pco成功后最大时间间隔 单位s，时间间隔补足1min，用于探测补足,补足子站点探测时间1min

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
    HI_U8   ucNum;                       // 队列元素个数,U8
    HI_U8   aucPad[3];                   // 保留位
    MRS_SRV_QUEUE stQueue;               // 队列
} MRS_CHG_ENTRY_QUEUE;

// 站点停电检测状态结构 56B
typedef struct
{
    HI_U8   ucStatus;                   // 停电检测状态，参考MRS_POWEROFF_STATUS_E
    HI_U8   ucReCheckStatus;            // 停电复核状态，参考MRS_POWERON_RECHECK_STATUS_E
    HI_U8   aucMac[HI_PLC_MAC_ADDR_LEN];

    HI_U8   ucDetectQueryCnt;           // 探测尝试次数
    HI_U8   ucRecheckQueryCnt;          // 复核尝试次数
    HI_U8   ucReportCheckScore;         // 上报情况复核积分
    HI_U8   ucRecheckFailCnt;           // 复核失败次数

    HI_U16  usMacReportOffCnt;          // 收到MAC层上报离线次数
    HI_U16  usMacReportOnCnt;           // 收到MAC层上报上线次数

    HI_BOOL bPassedDetect;              // 本周期是否有通过过停电探测
    HI_U8   ucMacReportWrongCnt;        // MAC上报的，误报次数
    HI_U8   ucMacReportMissCnt;         // MAC上报的，漏报次数
    HI_U8   ucMacReportCorrectCnt;      // MAC上报的，正确次数

    HI_U8   ucDetectReportWrongCnt;     // 探测后上报，误报次数
    HI_U8   ucDetectReportMissCnt;      // 探测后上报，漏报次数
    HI_U8   ucDetectReportCorrectCnt;   // 探测后上报，正确次数
    HI_U8   uc3762ReportWrongCnt;       // 上报集中器后的，误报次数

    HI_U8   uc3762ReportMissCnt;        // 上报集中器后的，漏报次数
    HI_U8   uc3762ReportCorrectCnt;     // 上报集中器后的，正确次数
    HI_U16  pad;

    HI_U32  ulLastMacReportWrongTime;   // 上一次MAC层误报的时间，CCO上电时间

    HI_U32  ulLastMacReportOffTime;     // 最近一次MAC层上报离线的RTX时间
    HI_U32  ulLastMacReportOnTime;      // 最近一次MAC层上报上线的RTC时间
    HI_U32  ulLastReportOffTime;        // 最近一次上报停电到集中器的RTC时间
    HI_U32  ulLastReportOnTime;         // 最近一次上报来电到集中器的RTC时间

    HI_U32  ulMeterLastPoweroffTime;    // 电表最近一次停电RTC时间
    HI_U32  ulMeterLastPoweronTime;     // 电表最近一次上电RTC时间
} MRS_POWEROFF_CHECK_ITEM_S;


// 停电检测列表
typedef struct
{
    HI_BOOL bDetectComplete;    // 是否探测完成
    HI_BOOL bRecheckComplete;   // 是否复核完成
    HI_U16  pad;

    HI_U16  usDetectIdx;        // 当前探测到哪一个节点
    HI_U16  usRecheckIdx;       // 当前复核到哪一个节点

    MRS_POWEROFF_CHECK_ITEM_S astItems[PRODUCT_CFG_MRS_MAX_TOPO_NUM];
} MRS_POWEROFF_CHECK_LIST_S;


typedef struct
{
    HI_U16  usNum;                       // 队列元素个数,U16
    HI_U8   aucPad[2];                   // 保留位
    MRS_SRV_QUEUE stQueue;               // 队列
} MRS_CHG_FRAME_QUEUE;

//停电检测(非电容模块)相关变量
#define MRS_POWEROFF_CHECK_HISTORY_NUM          (80) // 历史记录存储个数

// 停电检测状态
typedef enum
{
    MRS_POWEROFF_DEFAULT = 0,               // 初始状态
    MRS_POWEROFF_WAIT_DETECTING,            // 停电等待探测
    MRS_POWEROFF_DETECTING,                 // 停电探测中
    MRS_POWEROFF_DETECT_SUCCESS,            // 停电探测成功，未停电
    MRS_POWEROFF_INTERCEPT_BY_RECORD,       // 因停电记录被拦截，不上报
    MRS_POWEROFF_INTERCEPT_BY_PCO_DETECT,   //因探测到pco 探测成功被拦截，不上报
    MRS_POWEROFF_REPORTED,                  // 已上报
} MRS_POWEROFF_STATUS_E;


// 上线复核状态
typedef enum
{
    MRS_POWERON_RECHECKED,     // 复核完成
    MRS_POWERON_RECHECKING,    // 复核中
} MRS_POWERON_RECHECK_STATUS_E;

// 历史记录节点结构 24B
typedef struct
{
    HI_U8  aucMac[HI_PLC_MAC_ADDR_LEN];
    HI_U8  ucStatus; // 停电检测状态，参考MRS_POWEROFF_STATUS_E
    HI_U8  pad;

    HI_U32 ulMacReportOffTime;      // MAC上报离线的时间
    HI_U32 ulMacReportOnTime;       // MAC上报上线的时间
    HI_U32 ulMeterLastPoweroffTime; // 电表最近一次停电时间
    HI_U32 ulMeterLastPoweronTime;  // 电表最近一次上电时间
} MRS_POWEROFF_HISTORY_ITEM_S;

// 历史记录存储结构 964B
typedef struct
{
    HI_U8 ucIndex;
    HI_U8 pad[3];

    MRS_POWEROFF_HISTORY_ITEM_S astRecords[MRS_POWEROFF_CHECK_HISTORY_NUM];
} MRS_POWEROFF_HISTORY_LIST_S;

typedef struct
{
    HI_U8   ucReportEnable;                      // NV配置是否开启站点上线离线信息上报开关
    HI_U8   ucPowerOffReasonSwitch;              // 掉电原因上报开关
    HI_U8   ucPlcChlReasonSwitch;                // 信道原因上报开关
    HI_U8   ucCacheNumMax;                       // 队列中缓存的最大个数，达到个数即开始上报
    HI_U16  usCacheTimeout;                      // 队列中缓存的最长时间，单位: 100毫秒
    HI_U16  usReportTimeout;                     // 上报给集中器每次尝试的超时时间 单位: 毫秒
    HI_U8   ucReportRetryMax;                    // 上报给集中器的最大重试次数
    HI_U8   ucJoinCcoQueueLock;                  // 加入CCO MRS公共队列的锁,加入后上锁，收到ACK或超时后解锁
    HI_U8   ucPoweroffDetectMaxTries;   // 停电探测最大次数
    HI_U8   ucPoweroffDetectExpTries;   // 在MAC误报周期门限之内，若有MAC误报，则额外尝试这么多次
    HI_U32  ulMacReportWrongPeriodTh;   // MAC误报周期门限，单位min

    HI_U8   ucPoweroffDetectInterval;   // 停电探测广播报文发送间隔，单位s
    HI_U8   ucPoweroffRecheckMaxTries;  // 停电复核最大次数
    HI_U8   ucPoweroffRecheckInterval;  // 停电复核广播报文发送间隔，单位s
    HI_U8   ucReviewScoreFull;          // 停电检测复核满分分数

    HI_U8   ucWrongReportReduceScore;   // 误报时，减分分数
    HI_U8   ucMissReportPlusScore;      // 漏报时，加分分数
    HI_U8   ucCorrectReportPlusScore;   // 正确上报时，加分分数
    HI_U8   ucNoPassDetectPlusScore;    // 本周期站点没有探测成功的记录时，加分分数

    HI_U32  ulNoPassDetectPeriod;       // 站点是否有探测成功记录识别周期时长，单位s
    HI_U32  ulReportMaxDelayTime;       // 正确上报：上报时间-停电时间≤此时间，单位s

    HI_U16  usMacReportCacheMaxNum;     // MAC上报队列缓存最大个数
    HI_U8   ucMacReportCacheMaxTime;    // MAC上报队列缓存最大时长
    HI_U8   ucScalePowerOffDetectInterval;//规模停电探测间隔  单位s

    MRS_CHG_ENTRY_QUEUE  stCache;                // 原始数据缓存
    MRS_CHG_FRAME_QUEUE  stFrame;                // 报文上报队列
    MRS_CHG_FRAME_QUEUE stMacCache;     // MAC上报数据缓存

    MRS_POWEROFF_CHECK_LIST_S * pstCheckStateList;  // 停电检测状态列表
    MRS_POWEROFF_HISTORY_LIST_S * pstHistoryList;   // 历史记录

    HI_U32  ulGetPoweroffInfoSeq;       // 获取停电信息报文序号

    HI_U8   is_scale_power_off;         //大规模停电定时器启动
    HI_U8   pad;
    HI_U16  pad1;

    // 停电检测过程中 维测汇总统计量
    HI_U16  usMacReportOffSum;          // 收到MAC层停电上报总次数
    HI_U16  usMacReportOnSum;           // 收到MAC层来电上报总次数

    HI_U16  usDetectSuccessSum;         // 探测成功总次数
    HI_U16  usInterceptSum;             // 探测失败被停电记录拦截总次数
    HI_U16  us3762ReportOffSum;         // 上报集中器离线次数
    HI_U16  us3762ReportOnSum;          // 上报集中器上线次数

    HI_U16  usRecheckFailSum;           // 复核失败总次数
    HI_U16  usMacReportCorrectSum;      // MAC正确上报总次数
    HI_U16  usMacReportWrongSum;        // MAC误报总次数
    HI_U16  usMacReportMissSum;         // MAC漏报总次数
    HI_U16  usDetectReportCorrectSum;   // 第三层过滤前，正确上报总次数
    HI_U16  usDetectReportWrongSum;     // 第三层过滤前，误报总次数
    HI_U16  usDetectReportMissSum;      // 第三层过滤前，漏报总次数
    HI_U16  us3762ReportCorrectSum;     // 给集中器，正确上报总次数
    HI_U16  us3762ReportWrongSum;       // 给集中器，误报总次数
    HI_U16  us3762ReportMissSum;        // 给集中器，漏报总次数

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

// 获取 站点对应的停电检测节点
MRS_POWEROFF_CHECK_ITEM_S * mrsGetPoweroffCheckItem(MRS_NODE_STATE_CHG_REPORT_CTX * pstCtx, HI_U8 aucMac[HI_PLC_MAC_ADDR_LEN]);

// 处理 MAC层上报站点离线
HI_U32 mrsPoweroffHandleNodeOff(MRS_NODE_STATE_CHG_REPORT_CTX * pstCtx, HI_MDM_NETWORK_TOPOCHG_ENTRY_S * pstEntry);
// 处理 MAC层上报站点上线
HI_U32 mrsPoweroffHandleNodeOn(MRS_NODE_STATE_CHG_REPORT_CTX * pstCtx, HI_MDM_NETWORK_TOPOCHG_ENTRY_S * pstEntry);
// 执行 探测流程
HI_U32 mrsPoweroffRunDetectProcess(MRS_NODE_STATE_CHG_REPORT_CTX * pstCtx);
// 检查 是否探测失败
HI_BOOL mrsPoweroffCheckIsDetectFail(MRS_NODE_STATE_CHG_REPORT_CTX * pstCtx, MRS_POWEROFF_CHECK_ITEM_S * pstItem);
// 处理 站点探测失败
HI_U32 mrsPoweroffHandleDetectFail(MRS_NODE_STATE_CHG_REPORT_CTX * pstCtx, MRS_POWEROFF_CHECK_ITEM_S * pstItem);
// 执行 复核流程
HI_U32 mrsPoweroffRunRecheckProcess(MRS_NODE_STATE_CHG_REPORT_CTX * pstCtx);
// 执行 站点上报复核
HI_U32 mrsPoweroffRecheckItem(MRS_NODE_STATE_CHG_REPORT_CTX * pstCtx, MRS_PLC_POWEROFF_INFO_UP_S * pstUpFrame,
                              MRS_POWEROFF_CHECK_ITEM_S * pstItem);
// 处理 误报
HI_U32 mrsPoweroffHandleMACReportWrong(MRS_NODE_STATE_CHG_REPORT_CTX * pstCtx, MRS_POWEROFF_CHECK_ITEM_S * pstItem);
// 处理 正确上报
HI_U32 mrsPoweroffHandleMACReportCorrect(MRS_NODE_STATE_CHG_REPORT_CTX * pstCtx, MRS_POWEROFF_CHECK_ITEM_S * pstItem);
// 处理 漏报
HI_U32 mrsPoweroffHandleMACReportMiss(MRS_NODE_STATE_CHG_REPORT_CTX * pstCtx, MRS_POWEROFF_CHECK_ITEM_S * pstItem);

// 保存 历史记录
HI_U32 mrsPoweroffSaveHistory(MRS_NODE_STATE_CHG_REPORT_CTX * pstCtx, MRS_POWEROFF_CHECK_ITEM_S * pstItem);
// 检查 各个节点的稳定性
HI_U32 mrsPoweroffStableCheck(MRS_NODE_STATE_CHG_REPORT_CTX * pstCtx);

// 生成 一个查询停电信息报文(不包含mac_list)
HI_U32 mrsCcoCreatePoweroffInfoFrame(MRS_NODE_STATE_CHG_REPORT_CTX * pstCtx, MRS_PLC_POWEROFF_INFO_DL_S ** pstFrame);
// 发送 查询停电信息报文(默认广播)
HI_U32 mrsCcoGetPoweroffInfo(MRS_PLC_POWEROFF_INFO_DL_S * pstFrame, HI_U8 aucDstMac[HI_PLC_MAC_ADDR_LEN]);
// 上报 状态变化信息到集中器
HI_U32 mrsPoweroffReportStateChange(HI_MDM_NETWORK_TOPOCHG_ENTRY_S* pstEntry);

// 获取 探测中站点个数
HI_U16 mrsPoweroffGetDetectingNum(MRS_NODE_STATE_CHG_REPORT_CTX * pstCtx);
// 获取 复核中站点个数
HI_U16 mrsPoweroffGetRecheckingNum(MRS_NODE_STATE_CHG_REPORT_CTX * pstCtx);

HI_U32 mrsNodeStateChgPowerOffProc(HI_VOID);

//查询队列中是否存在重复mac的点
HI_U32 mrsPowerOffCheckFrameQueue(HI_U8 *pstCheckMac);

//处理大规模停电函数
HI_VOID mrsPowerOffDealScaleOff(HI_VOID);

//父子站点掉线处理
HI_U32 mrsCheckNodeOffByPcoDetect(HI_U8 *poweroff_mac);

#endif
HI_END_HEADER

#endif // __MRS_SRV_CCO_SLAVENODE_ON_OFF_LINE_H__

