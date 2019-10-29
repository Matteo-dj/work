//*****************************************************************************
//
//                  版权所有 (C), 1998-2011, 华为技术有限公司
//
//*****************************************************************************
//  文 件 名   : mrs_fw_nv.h
//  版 本 号   : V1.0
//  作    者   : fengxiaomin/f00209182
//  生成日期   : 2014-7-17
//  功能描述   : NV项相关处理函数及接口定义
//
//  函数列表   : TODO: ...
//  修改历史   :
//  1.日    期 : 2014-7-17
//    作    者 : fengxiaomin/f00209182
//    修改内容 : 创建文件
//
//*****************************************************************************
#ifndef __MRS_FW_NV_H__
#define __MRS_FW_NV_H__
HI_START_HEADER

#define MRS_SM_BC_SEND_INTERVAL     (30)    // 3000ms，即3秒
#define MRS_SM_BC_LOOP_MAX          (5)     // 5次
#define MRS_SM_QUERY_MAX_WAY        (10)    // 10个并发
#define MRS_SM_QUERY_INTERVAL       (2)     // 2s
#define MRS_SM_QUERY_FORCE_TIMES    (4)
#define MRS_SM_QUERY_EXCEPT_TIMES   (5)
#define MRS_SM_TOPO_QUERY_INTERVAL  (5)     // 5min
#define MRS_SM_TOPO_STABLE_TIMES    (6)     // 连续6次未变更
/* BEGIN: PN: DTS2015051601564 MODIFY\ADD\DEL by cuiate/00233580 at 2015/5/11 */
#define MRS_SM_REPORT_AA_2_MASTER   (0)
/* END:   PN: DTS2015051601564 MODIFY\ADD\DEL by cuiate/00233580 at 2015/5/11 */

#define MRS_SM_INVALID_FRM_LEN_07   (1)
#define MRS_SM_INVALID_FRM_LEN_97   (2)
#define MRS_SM_DURATION_DEFALUT             (180)
#define MRS_SM_BIND_CMD_INTERVAL            (5)     // 发送锁定报文间隔，单位s
#define MRS_SM_CHECK_MAIN_NODE_ADDR         (1)     // 主节点地址变化时是否重新台区识别
#define MRS_SM_ALLOW_SM_IN_TF               (1)     // 台区识别中是否允许搜表
#define MRS_SM_CHECK_MAIN_MODE_TIMEOUT      (300)   // 检查主节点地址超时时间，单位s
#define MRS_SM_REPORT_WAIT_LOCK_MSG         (0)     // 搜表上报是否等待锁定报文
#define MRS_SM_LEAVE_TIME_LEN_TH            (6*60) // STA离网时间门限，单位min
#define MRS_SM_BIND_CMD_SEND_INTERVAL       (3)
#define MRS_SM_BIND_CMD_SEND_MAX_WAY        (3)
#define MRS_SM_BIND_CMD_SEND_MAX_TIMES      (5)
#define MRS_TF_QUERY_INTERVAL               (5)
#define MRS_TF_QUERY_ROUND_INTERVAL         (300)
#define MRS_TF_QUERY_PERIOD                 (3600)
#define MRS_TF_QUERY_MAX_WAY                (3)
#define MRS_TF_QUERY_MAX_TRY                (50)

#define MRS_DETECT_BAUDRATE_LOOP_MAX          (4)            // 探测波特率轮次
#define MRS_DETECT_BAUDRATE_TIMEOUT           (1800)         // 探测波特率: 读通信地址单次超时时间, 单位:毫秒
#define MRS_DETECT_BAUDRATE_PERIOD            (60)           // 探测波特率: 周期, 单位: 秒
#define MRS_CLTI_SM_QUERY_INTERVAL            (30)           // 搜表过程中: 查询搜表结果的间隔 单位:秒
#define MRS_CLTI_READ_POSITIVE_A_MAX          (4)            // 上电流程中: 读NV中存储的表地址的正向有功的最大次数
#define MRS_CLTI_READ_POSITIVE_A_INTERVAL     (2)            // 上电流程中: 读NV中存储的表地址的正向有功的时间间隔 单位:秒

#define MRS_CLTII_SM_EXPLORE_BYTE_COUNT_MAX_DEFAULT       (0)                      // 多缩字节数上限默认值0
#define MRS_CLTII_SM_EXPLORE_BYTE_COUNT_MAX_UPPERBOUND    (HI_METER_ADDR_LEN - 1)  // 多缩字节数上限的最大合法值

#define MRS_SM_FRAME_TIMEOUT        (500)
#define MRS_SM_BYTE_TIMEOUT         (60)
#define MRS_SM_MAX_METERMUM         (MRS_MAX_METERNUM)
#define MRS_SM_DI_VER2007           (0x04000401)
#define MRS_SM_DI_VER1997           (0xC032)
#define MRS_SM_DI_VER1997B          (0x9010)
#define MRS_SM_DI_VER1997C          (0x901F)
#define MRS_SM_SEARCH_BC_ENABLE     (HI_TRUE)
#define MRS_SM_SEARCH_RETRY         (1)

#define MRS_SM_SEARCH_PERIOD        (60 * 24)   // 24h, 单位min

#define MRS_CSG_CLTI_PLC_TH         (25)   // 25min
#define MRS_CSG_CLTI_QUERY_TH       (25)   // 25min

#define MRS_CCO_EVT_REPORT_RETRY_TIMES_MAX  (2)
#define MRS_CCO_EVT_REPORT_TIMEOUT          (3000) //单位:毫秒
#define MRS_CCO_EVT_FILTER_AGED_TIME        (30)   //单位:分钟
#define MRS_CCO_EVT_BUFFER_SIZE             (200)
#define MRS_CCO_EVT_STAT_BAK_TIME           (720) //单位:分钟

#define MRS_STA_EVT_REPORT_TIMEOUT_DEFAULT  (3)  //单位:秒
#define MRS_STA_EVT_REPORT_TIMES_MAX        (41)
#define MRS_STA_EVT_REPORT_TIMEOUT_MAX      (60) //单位:秒
#define MRS_STA_CONTINUOUS_DENY_TIMES_MAX   (5)

#define MRS_STA_EVT_MONITOR_INTERVAL_DEFAULT  (5)  //单位:分钟
#define MRS_CSG_UPG_CRC_INIT_VALUE            (0)

#define MRS_SM_00_TO_DEFAULT(x,d)   do { if((x) == 0) {(x) = (d);} } while(0)

#define MRS_NODE_STATE_CHG_REPORT_ENABLE_DEFAULT             (1)    // 默认允许上报
#define MRS_NODE_STATE_CHG_REPORT_REASON_POWER_OFF_DEFAULT   (1)    // 默认允许上报停电原因
#define MRS_NODE_STATE_CHG_REPORT_REASON_PLC_CHL_DEFAULT     (1)    // 默认允许上报信道原因
#define MRS_NODE_STATE_CHG_REPORT_CACHE_NUM_MAX_DEFAULT      (30)   // 上报前缓存的最多数量
#define MRS_NODE_STATE_CHG_REPORT_CACHE_TIMEOUT_DEFAULT      (50)   // 缓存中最多等待的时间，单位: 100毫秒
#define MRS_NODE_STATE_CHG_REPORT_FRAME_TIMEOUT_DEFAULT      (10000) // 上报集中器最多等待的时间，单位: 毫秒
#define MRS_NODE_STATE_CHG_REPORT_FRAME_MAX_TRY_DEFAULT      (2)    // 上报3762帧最大重试次数
#define MRS_NODE_STATE_CHG_REPORT_CACHE_NUM_MAX_LOWERBOUND   (1)    // 上报前缓存的最多数量下限
#define MRS_NODE_STATE_CHG_REPORT_CACHE_NUM_MAX_UPPERBOUND   (80)   // 上报前缓存的最多数量上限，由报文长度1024推出


/* STA meter-reading & queue default params.  CNComment: STA抄表和队列默认参数。CNend */
/* Local: STA/CltII <--> Meter, CltI Module <--> CltI.  CNcomment: 本地: STA/II采与电表之间或I采模块与I采之间 */
#define MRS_LOCAL_RM_FRAME_HEAD         (1000)  /* Local: First byte timeout.           CNcomment: 本地: 首字节超时时间. CNend */
#define MRS_LOCAL_RM_FRAME_INTERVAL     (200)   /* Local: Frame timeout.                CNcomment: 本地: 字节间超时时间. CNend */
#define MRS_LOCAL_RM_CTRL_CMD_DELAY     (3000)  /* Local: Correction of control command.CNcomment: 本地: 费控命令补偿.   CNend */
#define MRS_LOCAL_RM_METER_INTERVAL     (100)   /* Local: Interval of two 645 frame.    CNcomment: 本地: 并发抄表帧间隔. CNend */
#define MRS_LOCAL_RM_QUEUE_MAX          (16)    /* STA/CltII: Max queue length.         CNcomment: STA/II采: 队列深度.   CNend */

/* CltI Transmit: CltI Module <--> CltI <--> Meter.  CNcomment: I采转发: I采模块通过I采转发抄读电表的默认参数。CNend */
#define MRS_CLTI_RM_FRAME_HEAD          (3600)  /* CltI Transmit: First byte timeout.           CNcomment: I采转发: 首字节超时时间. CNend */
#define MRS_CLTI_RM_FRAME_INTERVAL      (200)   /* CltI Transmit: Frame timeout.                CNcomment: I采转发: 字节间超时时间. CNend */
#define MRS_CLTI_RM_CTRL_CMD_DELAY      (2000)  /* CltI Transmit: Correction of control command.CNcomment: I采转发: 费控命令补偿.   CNend */
#define MRS_CLTI_RM_METER_INTERVAL      (100)   /* CltI Transmit: Interval of two 645 frame.    CNcomment: I采转发: 并发抄表帧间隔. CNend */
#define MRS_CLTI_RM_QUEUE_MAX           (16)    /* CltI: Max queue length.                      CNcomment: I采: 队列深度.           CNend */


HI_U32 mrsNvChangeMsgNotify(HI_U16 usId);
HI_U32 mrsNvChangeMsgProc(HI_SYS_QUEUE_MSG_S* pstMsg);
HI_U32 mrsNvStaSmCfg(HI_U16 usId);
HI_U32 mrsNvStaSmCfgExp(HI_U16 usId);
HI_U32 mrsStaSearchFtmNvInit(HI_VOID);

//*****************************************************************************
// 函数名称: MRS_GetMrNvCfg
// 功能描述: 获取抄表相关的NV配置项
//
// 参数说明:
//   pstCfg [out] NV_ASL_MR_CFG_STRU结构
//
// 返 回 值:
//    TODO: ...
//
// 调用要求: TODO: ...
// 调用举例: TODO: ...
// 作    者: shenhankun/00130424 [2011-11-10]
// 修    改: cuiate/00233580     [2013-03-18]
//*****************************************************************************
HI_U32 mrsInitAppMrNvInf(HI_VOID);
HI_U32 mrsInitStaMrNvInf(HI_VOID);
HI_U32 mrsNvCcoFtParam(HI_U16 usId);
HI_U32 mrsNvCcoSearchMeterCfg(HI_U16 usId);
HI_U32 mrsNvCcoSearchMeterCfgExp(HI_U16 usId);
HI_U32 mrsNvRmSyncCfg(HI_U16);
//Nv项改变通知
HI_U32 mrsNvDutFlagCfg(HI_U16 usId);
HI_U32 mrsNvRemoteUpgInfCfg(HI_U16 usId);
HI_U32 mrsNvSimuStateCfg(HI_U16 usId);
HI_U32 mrsNvMaxTurnNumCfg(HI_U16 usId);
HI_U32 mrsNvInitBaudRateCfg(HI_U16 usId);
HI_U32 mrsNvCltIIRefresh(HI_U16 usId);
// 获取事件相关NV项
HI_U32 mrsSrvGetEvtNvCfg(NV_APP_MRS_EVT_CFG_STRU *);
HI_U32 mrsNvStaEvtCfg(HI_U16 usId);
HI_U32 mrsNvCcoEvtCfg(HI_U16 usId);
HI_U32 mrsNvUserVerCfg(HI_U16 usId);

HI_U32 mrsGetUserVerNvCfg(NV_MRS_USER_VER_S* pstVer);

#if defined(PRODUCT_CFG_PRODUCT_TYPE_CCO) || defined(PRODUCT_CFG_PRODUCT_TYPE_STA)
HI_U32 mrsNVSetUserVerInfo(NV_MRS_USER_VER_S * pstInfo);
HI_U32 mrsNVSetSzId(HI_CHAR  szId[3]);
HI_U32 MRS_GetAppMrNvCfg( HI_OUT NV_APP_MR_CFG_STRU* pstCfg);

#endif

#if defined(PRODUCT_CFG_PRODUCT_TYPE_CCO)
HI_U32 mrsNvNodeStateChgReportCfg(HI_U16 usId);
HI_U32 mrsCcoSetProto(HI_U8 ucProto);
HI_U32 mrsNvSetUpgId(HI_U8 ucUpgId);
#endif


#if defined(PRODUCT_CFG_PRODUCT_TYPE_STA)
/**
 @brief  STA meter-reading config NV item change process.  CNcomment: STA抄表配置NV项变更处理函数。CNend
 @param[in] usId  Nv item ID (0x430).   CNcomment: NV项ID(0x430)。CNend
 @retval ::HI_ERR_SUCCESS  success.     CNcomment: 成功。CNend
 @retval ::Others          failed.      CNcomment: 失败。CNend
 */
HI_U32 mrsNvStaRmCfg(HI_U16 usId);
#endif


HI_END_HEADER
#endif //__MRS_SERVICE_H__

