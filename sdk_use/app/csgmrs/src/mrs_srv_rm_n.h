//*****************************************************************************
//
//                  版权所有 (C), 1998-2011, 华为技术有限公司
//
//*****************************************************************************
//  文 件 名   : mrs_srv_rm_n.h
//  作    者   : liuxipeng/KF54842
//  生成日期   : 2014-03-20
//  功能描述   : 读表管理模块内部函数声明及宏定义(仅供mrs_srv_readmeter_manage.c文件使用)
//               
//  函数列表   :
//  修改历史   : 
//  1.日    期 : 2014-03-20
//    作    者 : liuxipeng/KF54842
//    修改内容 : 创建文件 
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

// 读表模块超时检查定时器基数
#define MRS_RM_CHECK_TIMER_UNIT_BASE MRS_CCO_TIME_PARALLEL_CHECK

/* BEGIN: Modified by liuxipeng/lKF54842,2014/4/29 问题单号:DTS2014042306517 */
// 读表模块电表切换时间基数
#define MRS_RM_METER_SWITCH_TIME_UNIT (10)  // 毫秒
/* END  : Modified by liuxipeng/lKF54842,2014/4/29 问题单号:DTS2014042306517 */

// 支持的最大注册策略数量
#define MRS_RM_STRATEGY_REG_MAX_NUM    MRS_RM_TYPE_MAX_NUM

// 超时检查定时器状态
#define MRS_RM_TIMER_INACTIVE       (0)    // 未运行
#define MRS_RM_TIMER_ACTIVE         (1)    // 正在运行

// 工作模式定义
#define MRS_RM_NORMAL_MODE          (0)    // 正常模式(此模式下按照最大并发数发送非广播帧)
#define MRS_RM_BROADCAST_MODE       (1)    // 广播模式(此模式下只处理广播帧-目前限制广播数量为1)

// 节点控制状态
#define MRS_RM_NODE_UNTREATED       (1)    // 未经处理的
#define MRS_RM_NODE_UNINITIALIZED   (2)    // 节点上下文未初始化
/* BEGIN: Modified by liuxipeng/lKF54842,2014/4/29 问题单号:DTS2014042306517 */
#define MRS_RM_NODE_WAIT_SEND       (3)    // 等待发送(处于此状态后需等一段时间才允许发送)  
#define MRS_RM_NODE_SEND_REQ        (4)    // 请求发送
#define MRS_RM_NODE_WAIT_RESP       (5)    // 等待应答  
/* END  : Modified by liuxipeng/lKF54842,2014/4/29 问题单号:DTS2014042306517 */

// 并发控制默认参数
#define MRS_PARALLEL_NUM_DEFAULT    (5)     // 最大并发数默认值
#define MRS_FRAME_NUM_DEFAULT       (32)    // 最大单次处理的数据项个数默认值
#define MRS_ITEM_NUM_DEFAULT        (64)    // 最大读表请求条目数量默认值

#define MRS_PARALLEL_NUM_MAX        (1024)  // 最大并发数最大值
#define MRS_FRAME_NUM_MAX           (100)   // 最大单次处理的数据项个数最大值
#define MRS_ITEM_NUM_MAX            (1024)  // 最大读表请求条目数量最大值

#define MRS_PARALLEL_NUM_MIN        (1)     // 最大并发数最小值
#define MRS_FRAME_NUM_MIN           (1)     // 最大单次处理的数据项个数最小值
#define MRS_ITEM_NUM_MIN            (1)     // 最大读表请求条目数量最小值

/* BEGIN: Modified by liuxipeng/lKF54842,2014/4/26 问题单号:DTS2014042407354 */
#define MRS_RM_CRC_MATCH            (0)     // 使用CRC匹配
#define MRS_RM_ADDR_MATCH           (1)     // 使用表地址匹配
/* END  : Modified by liuxipeng/lKF54842,2014/4/26 问题单号:DTS2014042407354 */

typedef struct
{
    HI_U16 usRmType;            // 业务读表类型 
    HI_U8 ucRetryCfgNo  : 4;    // 重传配置号 (MRS_RETRY_CFG_NUM_ENUM)
    HI_U8 ucRetryStrategy : 4;    // 重传策略号 (MRS_RM_RETRY_STRATEGY_ENUM)
    HI_U8 ucPoolNo      : 4;    // 并发池号 (MRS_PARALLEL_POOL_ENUM)
    HI_U8 padding       : 4;
    MRS_RM_DATA_RESP_PROC_F pfnRespProc;    // 业务读表数据应答处理回调函数
} MRS_RM_TACTIC_REG, *P_MRS_RM_STRATEGY_REG;

// 读表请求元素结构(缓存池横向)
typedef struct 
{
    MRS_SRV_NODE link;
    HI_U8  ucMeterAddr[HI_METER_ADDR_LEN];   // 目的电表地址
    HI_U8  ucMrSeq;          // 业务读表376.2帧序列号
    HI_U8  ucStrategyNo : 4;   // 读表策略号
    HI_U8  ucProtocol : 4;   // 规约类型
    HI_U8  ucFrameNum : 7;   // 数据转发帧个数(pucData内帧个数)
    HI_U8  bCtrlCmd   : 1;   // 数据转发帧中是否存在费控命令
    HI_U8  reserved;
    HI_U16 usDataLen;    // 业务读表数据转发长度
    HI_U32 ulDataCrc;    // 业务读表数据CRC
#if defined(PRODUCT_CFG_PRODUCT_TYPE_CSG_GD)
    HI_PVOID pvParam;
#endif
    HI_U8  pucData[0];   // 业务读表数据转发内容
} MRS_RM_ITEM, *P_MRS_RM_ITEM;

// 并发池并发节点上下文
typedef struct 
{
    HI_U32 ulPlcSeq;        // PLC帧序号
    HI_U32 ulOverTimeTotal; // 超时时间(一次PLC通讯最大耗时,包含STA端耗时) 单位:ms
    HI_U32 ulOverTimeMeter; // STA端电表超时时间
    HI_U32 ulRunOutTime;    // 当前PLC发送已消耗时间(发送PLC帧后此时间开始累积-当大于等于ulOverTimeTotal后表示PLC超时)单位:ms
/* BEGIN: Modified by liuxipeng/lKF54842,2014/4/29 问题单号:DTS2014042306517 */    
    HI_U32 ulWaitSendTime;  // 当前等待发送已消耗时间(队列中后续元素准备发送时开始累积-当大于等于"电表切换间隔"时间后允许发送)单位:ms
/* END  : Modified by liuxipeng/lKF54842,2014/4/29 问题单号:DTS2014042306517 */
    HI_U8  ucMaxTry;        // 当前策略最大重传次数
    HI_U8  ucTry;           // 当前策略重传次数
    HI_U8  ucTryLoop;       // 重传轮次("点抄"有效)
    HI_U8  ucIsBC;          // 广播帧标志(发送广播PLC帧时此标志置HI_TRUE)
    MRS_PLC_FRAME_DATA_STRU stPlcData; // 待发送的PLC下行数据帧结构
} MRS_NODE_CTX, *P_MRS_NODE_CTX;

// 读表请求节点结构(缓存池纵向)
typedef struct 
{
    MRS_SRV_LINK link;
    HI_U8 ucDestMac[HI_PLC_MAC_ADDR_LEN];   // 目的站点MAC地址
    HI_U8 ucState;      // 节点状态(MRS_RM_NODE_SEND_REQ : MRS_RM_NODE_WAIT_RESP)
    HI_U8 ucItemNum;    // 本节点元素个数
    P_MRS_NODE_CTX pstNodeCtx;  // 并发节点上下文(只在该节点处理时才进行初始化和使用)
    MRS_SRV_QUEUE  stItemQueue;  // 缓存的业务读表队列 指向MRS_RM_ITEM结构体
} MRS_RM_NODE, *P_MRS_RM_NODE;

// 并发池结构
typedef struct 
{
    HI_U8 ucMode;           // 缓存池工作模式(MRS_RM_NORMAL_MODE : MRS_RM_BROADCAST_MODE)
    HI_U8 padding;
    HI_U16 usMaxParalNumNv; // NV中并发数
    HI_U16 usParallelNum;   // 当前并发数
    HI_U16 usItemTotalNum;  // 当前总的业务读表请求数量
    MRS_PARALLEL_CFG_S stParallelCfg;
    MRS_SRV_LIST stNodeList;     // 缓存池 指向MRS_RM_NODE结构
} MRS_RM_REQ_POOL, *P_MRS_RM_REQ_POOL;

// 读表管理模块结构
typedef struct 
{
    MRS_RETRY_CFG_S stRetryCfg[MRS_RETRY_CFG_MAX_NUM];
    MRS_RM_TACTIC_REG stStrategyReg[MRS_RM_STRATEGY_REG_MAX_NUM];
    HI_U32 ulPlcSeq;
    HI_U8  ucTimerStatus;
    HI_U8  padding[3];
    MRS_RM_REQ_POOL stPool[MRS_RM_POOL_MAX_NUM];
} MRS_READMETER_MODULE, *P_MRS_READMETER_MODULE;


HI_PRVL P_MRS_READMETER_MODULE mrsRmGetModuleCtx(HI_VOID);

HI_PRV P_MRS_RETRY_CFG_S mrsRmGetRetryCfg(HI_U8);

HI_PRV P_MRS_RM_REQ_POOL mrsRmGetPool(HI_U8);

HI_PRV P_MRS_RM_STRATEGY_REG mrsRmGetStrategy(HI_U8);

#if !defined(PRODUCT_CFG_PRODUCT_TYPE_CSG_GD)
HI_PRV HI_U32 mrsRmGetPlcSeq(HI_VOID);
HI_PRV HI_VOID mrsRmPlcSeqAddSelf(HI_VOID);
#endif

HI_PRVL HI_U8 mrsRmGetTimerStatus(HI_VOID);

HI_PRVL HI_VOID mrsRmSetTimerStatus(HI_U8);

HI_PRVL HI_VOID mrsRmSetPoolMode(P_MRS_RM_REQ_POOL, HI_U8);

HI_PRVL HI_VOID mrsRmRetryCfgValidtyCheck(P_MRS_RETRY_CFG_S);

HI_PRV HI_VOID mrsRmParallelCfgInit(P_MRS_RM_REQ_POOL pstPool, P_MRS_PARALLEL_CFG_S, P_NV_MRS_PARALLEL_CFG_S);

HI_PRV HI_U32 mrsRmFindStrategy(HI_U16, P_MRS_RM_STRATEGY_REG *, HI_U8 *);
    
HI_PRV P_MRS_RM_NODE mrsRmAddNode(P_MRS_RM_REQ_POOL, HI_U8 *);

HI_PRV HI_U32 mrsRmAddItem(P_MRS_RM_NODE, P_MRS_RM_REQ_DATA, HI_U8, MRS_645BUF_INF *);

/* BEGIN: Modified by liuxipeng/lKF54842,2014/4/29 问题单号:DTS2014042306517 */
HI_PRV HI_U32 mrsRmItemDeQueue(P_MRS_RM_NODE, HI_BOOL);
/* END  : Modified by liuxipeng/lKF54842,2014/4/29 问题单号:DTS2014042306517 */

HI_PRV P_MRS_RM_NODE mrsRmFindNode(P_MRS_RM_REQ_POOL, HI_U8 *);

/* BEGIN: Modified by liuxipeng/lKF54842,2014/4/26 问题单号:DTS2014042407354 */
HI_PRV HI_BOOL mrsRmFindReqOfQueue(MRS_SRV_QUEUE *, HI_U32, HI_U8);

HI_PRV HI_BOOL mrsRmFindReqOfPool(P_MRS_RM_REQ_POOL, HI_U32, HI_U8);
/* END  : Modified by liuxipeng/lKF54842,2014/4/26 问题单号:DTS2014042407354 */

HI_PRVL HI_VOID mrsRmQueueFree(HI_VOID *);

HI_PRV HI_VOID mrsRmNodeFree(HI_VOID *, HI_VOID *);

HI_PRV HI_VOID mrsRmEmptyPool(P_MRS_RM_REQ_POOL);

HI_PRV HI_VOID mrsRmRemoveNode(MRS_SRV_LIST *, P_MRS_RM_NODE);

HI_PRV HI_U32 mrsRmNodeCtxCreate(P_MRS_RM_NODE);

HI_PRV HI_VOID mrsRmNodeCtxReset(P_MRS_RM_NODE);

HI_PRV HI_VOID mrsRmNodeCtxFree(P_MRS_NODE_CTX);

/* BEGIN: Modified by liuxipeng/lKF54842,2014/4/29 问题单号:DTS2014042306517 */
HI_PRV HI_U32 mrsRmNodeCtxInit(P_MRS_RM_NODE, HI_BOOL);
/* END  : Modified by liuxipeng/lKF54842,2014/4/29 问题单号:DTS2014042306517 */

#if !defined(PRODUCT_CFG_PRODUCT_TYPE_CSG_GD)
HI_PRVL HI_VOID mrsRmSetupTransmitData(P_MRS_RETRY_CFG_S, P_MRS_RM_ITEM, P_MRS_NODE_CTX, MRS_DATA_TRANSMIT_AFNF1_DL_STRU *);
#endif

HI_PRV HI_U32 mrsRmPlcPackCreate(P_MRS_RM_NODE, P_MRS_RM_ITEM, P_MRS_RETRY_CFG_S);

HI_PRV HI_VOID mrsRmGetDestMacAttr(HI_U8 *, HI_U8 *, HI_BOOL *);

HI_PRV HI_VOID mrsRmNodeRetryParamInit(P_MRS_RM_NODE, P_MRS_RETRY_CFG_S, HI_BOOL);

HI_PRV HI_U32 mrsRmSendPlcPackage(P_MRS_NODE_CTX);

HI_PRV HI_U32 mrsRmAllowJoinPool(P_MRS_RM_REQ_POOL, P_MRS_RM_REQ_DATA, MRS_645BUF_INF *);

HI_PRV HI_U32 mrsRmSendReqProcActive(P_MRS_RM_REQ_POOL);

HI_PRV HI_BOOL mrsRmNodeStatusProc(MRS_SRV_LIST *, P_MRS_RM_NODE);

HI_PRV HI_U32 mrsRmReturnRespData(P_MRS_RM_STRATEGY_REG, P_MRS_RM_ITEM, HI_U8 *, HI_U16 );

HI_PRV HI_U32 mrsRmPlcRetryProcStrategy01(P_MRS_RM_NODE, P_MRS_RETRY_CFG_S);

HI_PRV HI_U32 mrsRmPlcRetryProcStrategy02(P_MRS_RM_NODE, P_MRS_RETRY_CFG_S);

HI_PRV HI_U32 mrsRmPlcOverTimeProc(P_MRS_RM_REQ_POOL, P_MRS_RM_NODE);

/* BEGIN: Modified by liuxipeng/lKF54842,2014/4/29 问题单号:DTS2014042306517 */
HI_PRV HI_U32 mrsRmPlcOverTimeCheck(P_MRS_RM_REQ_POOL, P_MRS_RM_NODE, HI_BOOL *);

HI_PRV HI_U32 mrsRmPlcSendWaitCheck(P_MRS_RM_REQ_POOL, P_MRS_RM_NODE, HI_U32 *, HI_BOOL *);
/* END  : Modified by liuxipeng/lKF54842,2014/4/29 问题单号:DTS2014042306517 */

#if !defined(PRODUCT_CFG_PRODUCT_TYPE_CSG_GD)
HI_PRV HI_U32 mrsRmPoolPlcRxProc(P_MRS_RM_REQ_POOL, HI_U8 *, MRS_DATA_TRANSMIT_AFNF1_UP_STRU *, HI_BOOL *);
#endif

/* BEGIN: Modified by liuxipeng/lKF54842,2014/4/29 问题单号:DTS2014042306517 */
HI_PRV HI_U32 mrsRmPoolCheckTimerProc(P_MRS_RM_REQ_POOL, HI_U32 *, HI_U32 *);
/* END  : Modified by liuxipeng/lKF54842,2014/4/29 问题单号:DTS2014042306517 */

#if !defined(PRODUCT_CFG_PRODUCT_TYPE_CSG_GD)
HI_PRV HI_U32 mrsRmPlcUpCompatibleProc(MRS_DATA_TRANSMIT_AFNF1_UP_STRU *, HI_U16, HI_PBYTE *);
HI_PRV HI_U32 mrsRmPlcRespProc(P_MRS_RM_NODE, HI_U8 *, MRS_DATA_TRANSMIT_AFNF1_UP_STRU *);
#endif


#if defined(PRODUCT_CFG_PRODUCT_TYPE_CSG_GD)
HI_PRV HI_U32 mrsRmPoolPlcRxProcCsg(P_MRS_RM_REQ_POOL, HI_U8 *, HI_U16, HI_BOOL *);
HI_PRV HI_U32 mrsRmPlcRespProcCsg(P_MRS_RM_NODE, HI_U8 *, HI_U16);
#endif


#endif

HI_END_HEADER

#endif

