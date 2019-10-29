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

typedef struct
{
    HI_U16  usNum;                       // 队列元素个数,U16
    HI_U8   aucPad[2];                   // 保留位
    MRS_SRV_QUEUE stQueue;               // 队列
} MRS_CHG_FRAME_QUEUE;

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
    HI_U8   aucPad[2];                           // 保留位
    MRS_CHG_ENTRY_QUEUE  stCache;                // 原始数据缓存
    MRS_CHG_FRAME_QUEUE  stFrame;                // 报文上报队列
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

