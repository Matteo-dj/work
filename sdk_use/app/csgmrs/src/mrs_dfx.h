//*****************************************************************************
//
//                  版权所有 (C), 1998-2011, 华为技术有限公司
//
//*****************************************************************************
//  文 件 名   : mrs_dfx.h
//  版 本 号   : V1.0 
//  作    者   : niesongsong/nkf62735
//  生成日期   : 2012-08-31
//  功能描述   : dfx功能
//               
//  函数列表   : 
//  修改历史   : 
//  1.日    期 : 2011-11-29
//    作    者 : niesongsong/nkf62735
//    修改内容 : 创建文件 
//
//*****************************************************************************
#ifndef __MRS_DFX_H__
#define __MRS_DFX_H__

#define MRS_DFX_RX_PLC   0
#define MRS_DFX_RX_UART  1
#define MRS_DFX_TX_PLC   2
#define MRS_DFX_TX_UART  3

#include "dfx_sys.h"
#include "mrs_srv_queue.h"

#define MRS_ONE_BYTE_VALUE_MAX       (255)

/* BEGIN: Modified by fengxiaomin/00209182, 2014/3/27   问题单号:DTS2014032906596 */
#define MRS_TEST_READ_METER_645_DI   (0xFFFFFFFE)
#define MRS_DAYFREEZE_TIME_DI        (0x05060001 + 0x33333333)
#define MRS_DAYFREEZE_DATA_DI        (0x05060101 + 0x33333333)
/* END:   Modified by fengxiaomin/00209182, 2014/3/27 */

#define MRS_CCO_DETECT_ALLOW_JOIN_WHITE_TOPO_EVALUATE    (1)
#define MRS_CCO_DETECT_ALLOW_JOIN_WHITE_TOPO_FORME       (2)
#define MRS_CCO_DETECT_ALLOW_JOIN_WHITE_EXTERN_RESUME    (3)

#define MRS_CCO_DETECT_FORBID_JOIN_WHITE_PLC_DISCONNECT  (1)
#define MRS_CCO_DETECT_FORBID_JOIN_WHITE_EXTERN_DISABLE  (2)

#define MRS_CCO_DETECT_END_REASON_RM_FINISH              (1)
#define MRS_CCO_DETECT_END_REASON_METER_EXIST_MAP        (2)
#define MRS_CCO_DETECT_END_REASON_WHITE_KEEP_TIMEOUT     (3)
#define MRS_CCO_DETECT_END_REASON_PLC_DISCONNECT         (4)
#define MRS_CCO_DETECT_END_REASON_EXTERN_DISABLE         (5)
#define MRS_CCO_DETECT_END_REASON_ARCHIVES_CLEAR         (6)
#define MRS_CCO_DETECT_END_REASON_OTHER                  (7)

typedef struct 
{
    HI_DSID_APP_CHL_RX_TX_INF_S         app_rx_tx;        // 通道收发统计
    HI_DSID_APP_TOPO_CHANGE_INF_S       app_topo_change;  // Topo变化统计
#if defined(PRODUCT_CFG_PRODUCT_TYPE_STA)
    HI_DSID_APP_HEARTBEAT_INF_S         app_heart;        // 心跳统计
#endif
#if defined(PRODUCT_CFG_PRODUCT_TYPE_CCO)    
    HI_DSID_APP_CCO_RUN_STATUS_S        app_cco_status;   // CCO运行状态
#endif
    HI_DSID_APP_RM_INF_S                app_rm_inf;       // 读表相关信息
    HI_DSID_APP_VM_CHL_INF_S            app_vm_chl_inf;   // 模拟通道状态
#if defined(PRODUCT_CFG_PRODUCT_TYPE_STA)    
    HI_DSID_APP_STA_STATUS_S            app_sta_status;   // STA状态
#endif
    HI_DSID_APP_PLC_BC_INF_S            app_plc_bc_inf;   // 读表使用广播Plc方式统计
    HI_DSID_APP_MSG_INF_S               app_task_msg_inf; // 应用任务消息处理统计
#if defined(PRODUCT_CFG_PRODUCT_TYPE_STA)    
    HI_DSID_APP_RM_QUEUE_INF_S          app_rm_queue_inf; // 读表队列相关信息
#endif
    HI_DSID_APP_RM_RX_TX_INF_S          app_rm_rx_tx_inf; // 应用层读表通道接收统计信息
#if defined(PRODUCT_CFG_PRODUCT_TYPE_CCO)    
    HI_DSID_APP_CCO_GET_TIME_INF_S      app_get_time_inf; //获取时钟相关信息
    HI_DSID_APP_CCO_EVT_INF_S           app_cco_evt_inf; //获取时钟相关信息
    HI_DSID_APP_CCO_EVT_BAK_INF_S       app_cco_evt_inf_bak; //获取时钟相关信息
    HI_DSID_APP_CCO_REPORT_QUEUE_INF_S  app_cco_report_queue_inf; //获取时钟相关信息
    HI_DSID_APP_CCO_MEMORY_INC_INF_S    app_cco_inc_inf; //获取表档案的flash内存分配信息
#endif
#if defined(PRODUCT_CFG_PRODUCT_TYPE_STA)    
    HI_DSID_APP_STA_EVT_INF_S           app_sta_evt_inf; //获取STA事件相关信息
	HI_DSID_APP_STA_EVT_MONITOR_INF_S	app_sta_evt_mon_inf; //获取STA监控事件相关信息
#endif
#if defined(PRODUCT_CFG_PRODUCT_TYPE_CCO)    
    HI_DSID_APP_CCO_NODE_STATECHG_INF_S app_cco_node_state_chg_inf; // CCO载波从节点在线状态变化上报统计
    HI_DSID_APP_CCO_DETECT_RM_INF_S     app_cco_detect_rm_inf; // CCO探测抄表信息统计
#endif
/* BEGIN: PN: DTS2016021906890 MODIFY\ADD\DEL by cuiate/00233580 at 2016/2/21 */
    HI_DSID_APP_PLC_FRAME_STAT_INF_S    app_plc_frame_inf;  // plc报文过滤统计
/* END:   PN: DTS2016021906890 MODIFY\ADD\DEL by cuiate/00233580 at 2016/2/21 */
    HI_DSID_APP_CSG_PYSICAL_MODE_INF_S_CSG  app_csg_physicalmode_inf;//获取测试模式相关信息 
}MRS_DFX_MODULE;


// **************************************
// **************************************

HI_PUBLIC HI_U32 mrsDfxStatInit(HI_VOID);
HI_PUBLIC HI_U32 mrsDfxReset(HI_VOID);

//通道收发统计
// PLC通道接收统计 
// rx 通道接收结构体
// msg_proc 用以表示是消息发送还是处理
//          HI_TRUE : 为消息处理
//          HI_FALSE : 为消息发送
HI_PUBLIC HI_U32 mrsDfxChlPlcRx(HI_DMS_CHL_RX_S * , HI_BOOL );

// PLC通道接收错误统计
// rx 通道接收结构体
// err_cause 错误原因
// msg_proc 用以表示是消息发送还是处理
//          HI_TRUE : 为消息处理
//          HI_FALSE : 为消息发送
HI_PUBLIC HI_U32 mrsDfxChlPlcErrRx(HI_DMS_CHL_RX_S *, HI_U32 , HI_BOOL );

// PLC发送统计
HI_PUBLIC HI_U32 mrsDfxChlPlcTx(HI_DMS_CHL_TX_DATA_S *,
                            HI_DMS_CHL_TX_ADDR_S *, HI_BOOL );

// PLC发送失败统计
HI_PUBLIC HI_U32 mrsDfxChlPlcErrTx(HI_DMS_CHL_TX_DATA_S *, HI_DMS_CHL_TX_ADDR_S *, 
                                HI_U32, HI_BOOL);

// 向串口统计缓冲区中塞入收到的数据
HI_PUBLIC HI_U32 mrsDfxChlUartRxPut(UART_CHL_INF_S *, HI_U8 *,HI_U16);

// 刷新串口接收统计
HI_PUBLIC HI_U32 mrsDfxChlUartRx(HI_U8 * ,HI_U16 ,HI_BOOL);

// 刷新串口接收错误统计
HI_PUBLIC HI_U32 mrsDfxChlUartErrRx(HI_U16 ,HI_U32 ,HI_BOOL);

// 刷新串口接收正确帧统计
HI_PUBLIC HI_VOID mrsDfxChlUartOkFrameCnt(HI_U16, HI_BOOL);

// 刷新串口接收错误帧统计
HI_PUBLIC HI_VOID mrsDfxChlUartErrFrameCnt(HI_BOOL);

// 串口发送统计(消息发送)
HI_PUBLIC HI_U32 mrsDfxChlUartTxMsgTx(HI_U8 *, HI_U16);

// 串口发送统计(消息处理)
HI_PUBLIC HI_U32 mrsDfxChlUartTxMsgRx(HI_DMS_CHL_TX_DATA_S *);

// 串口发送成功统计(消息发送)
HI_PUBLIC HI_U32 mrsDfxChlUartOkTxMsgTx(HI_U16);

// 串口发送成功统计(消息处理)
HI_PUBLIC HI_U32 mrsDfxChlUartOkTxMsgRx(HI_DMS_CHL_TX_DATA_S *);

// 串口发送失败统计(消息发送)
HI_PUBLIC HI_U32 mrsDfxChlUartErrTxMsgTx(HI_U16 , HI_U32);

// 串口发送失败统计(消息处理)
HI_PUBLIC HI_U32 mrsDfxChlUartErrTxMsgRx(HI_DMS_CHL_TX_DATA_S *, HI_U32);

// 心跳统计
HI_PUBLIC HI_U32 mrsDfxHeartCnt(HI_VOID);

// 心跳超时统计
HI_PUBLIC HI_U32 mrsDfxHeartTimeoutCnt(HI_VOID);

// CCO运行状态更新
HI_PUBLIC HI_U32 mrsDfxCcoStatus(HI_U8 status);

// Map维护完成时标刷新
HI_PUBLIC HI_U32 mrsDfxMapReadyTime(HI_VOID);

//内存统计 
HI_PUBLIC HI_U32 mrsDfxMemAllocCnt(HI_VOID);
HI_PUBLIC HI_U32 mrsDfxMemAllocErrCnt(HI_VOID);
HI_PUBLIC HI_U32 mrsDfxMemFreeCnt(HI_VOID);
HI_PUBLIC HI_U32 mrsDfxMsgSendErrCnt(HI_VOID);

//通道改变
HI_PUBLIC HI_U32 mrsDfxChlChangeTrue(HI_U32 time);
HI_PUBLIC HI_U32 mrsDfxChlChangeFalse(HI_U32 time);
HI_PUBLIC HI_U32 mrsDfxChlChangeMsgFail(HI_U32);

// ************************
// ***读表相关统计量刷新***
// ************************

// 读表管理模块统计
HI_PUBLIC HI_VOID mrsDfxRmPlcSeq(HI_U32);
HI_PUBLIC HI_VOID mrsDfxRmTimerStatus(HI_U8);
HI_PUBLIC HI_VOID mrsDfxRmPoolId(HI_U32, HI_U8);
HI_PUBLIC HI_VOID mrsDfxRmPoolMode(HI_U32, HI_U8);
HI_PUBLIC HI_VOID mrsDfxRmPalallelNum(HI_U32, HI_U16);
HI_PUBLIC HI_VOID mrsDfxRmItemNum(HI_U32, HI_U16);
HI_PUBLIC HI_VOID mrsDfxRmJoinFail(HI_U32, HI_U32);

// 轮抄统计
HI_PUBLIC HI_VOID mrsDfxLrRoute(HI_U8);
HI_PUBLIC HI_VOID mrsDfxLrState(HI_U8);
HI_PUBLIC HI_VOID mrsDfxLrMeterNum(HI_U16);
HI_PUBLIC HI_VOID mrsDfxLrReqCnt(HI_VOID);
HI_PUBLIC HI_VOID mrsDfxLrAckCnt(HI_VOID);
HI_PUBLIC HI_VOID mrsDfxLrPlcTx(HI_VOID);
HI_PUBLIC HI_VOID mrsDfxLrPlcRx(HI_VOID);
HI_PUBLIC HI_VOID mrsDfxLrMeterTx(HI_VOID);
HI_PUBLIC HI_VOID mrsDfxLrMeterRx(HI_VOID);
HI_PUBLIC HI_VOID mrsDfxLrPlcTryCnt(HI_VOID);
HI_PUBLIC HI_VOID mrsDfxLrPlcSeqErr(HI_VOID);
HI_PUBLIC HI_VOID mrsDfxLrPlcLenErr(HI_VOID);
HI_PUBLIC HI_VOID mrsDfxLrPlcExceedCnt(HI_VOID);
HI_PUBLIC HI_VOID mrsDfxLrPlcTimeout(HI_VOID);
HI_PUBLIC HI_VOID mrsDfxLrUpDataCnt(HI_VOID);
HI_PUBLIC HI_VOID mrsDfxLrUpDataTryCnt(HI_VOID);
HI_PUBLIC HI_VOID mrsDfxLrOkCnt(HI_VOID);
HI_PUBLIC HI_VOID mrsDfxLrFailCnt(HI_VOID);

// 随抄统计
HI_PUBLIC HI_VOID mrsDfxXrStatus(HI_U8);
HI_PUBLIC HI_VOID mrsDfxXrUartTx(HI_VOID);
HI_PUBLIC HI_VOID mrsDfxXrUartRx(HI_VOID);
HI_PUBLIC HI_VOID mrsDfxXrPlcTx(HI_VOID);
HI_PUBLIC HI_VOID mrsDfxXrPlcRx(HI_VOID);
HI_PUBLIC HI_VOID mrsDfxXrNakCnt(HI_VOID);
HI_PUBLIC HI_VOID mrsDfxXrNak(HI_U8);
HI_PUBLIC HI_VOID mrsDfxXrPlcTryCnt(HI_VOID);
HI_PUBLIC HI_VOID mrsDfxXrPlcSeqErr(HI_VOID);
HI_PUBLIC HI_VOID mrsDfxXrPlcLenErr(HI_VOID);
HI_PUBLIC HI_VOID mrsDfxXrPlcExceedCnt(HI_VOID);
HI_PUBLIC HI_VOID mrsDfxXrPlcTimeout(HI_VOID);
HI_PUBLIC HI_VOID mrsDfxXrOkCnt(HI_VOID);
HI_PUBLIC HI_VOID mrsDfxXrFailCnt(HI_VOID);

// 并发统计
HI_PUBLIC HI_VOID mrsDfxPrUartTx(HI_VOID);
HI_PUBLIC HI_VOID mrsDfxPrUartRx(HI_VOID);
HI_PUBLIC HI_VOID mrsDfxPrPlcTx(HI_VOID);
HI_PUBLIC HI_VOID mrsDfxPrPlcRx(HI_VOID);
HI_PUBLIC HI_VOID mrsDfxPrPoolNum(HI_U16);
HI_PUBLIC HI_VOID mrsDfxPrNakCnt(HI_U8);
HI_PUBLIC HI_VOID mrsDfxPrPlcTryCnt(HI_VOID);
HI_PUBLIC HI_VOID mrsDfxPrPlcSeqErr(HI_VOID);
HI_PUBLIC HI_VOID mrsDfxPrPlcLenErr(HI_VOID);
HI_PUBLIC HI_VOID mrsDfxPrPlcTimeout(HI_VOID);
HI_PUBLIC HI_VOID mrsDfxPrOkCnt(HI_VOID);
HI_PUBLIC HI_VOID mrsDfxPrFailCnt(HI_VOID);

// ************************
// ********E***N***D*******
// ************************

// 模拟通道统计
HI_PUBLIC HI_VOID mrsDfxVmChlStatus(HI_U8);

// STA状态
HI_PUBLIC HI_VOID mrsDfxStaStatus(HI_U8);

// 虚拟表状态统计
HI_PUBLIC HI_VOID mrsDfxVmMeterStatus(HI_U8, HI_BOOL);

// 主动上报标志状态统计
HI_PUBLIC HI_VOID mrsDfxReportStatus(HI_U8);

// 改变上报标志状态统计
HI_PUBLIC HI_VOID mrsDfxChangeStatus(HI_U8);

// 刷新队列消息个数统计
HI_PUBLIC HI_VOID mrsDfxRefreshMsgQueueNum(HI_VOID);

// 消息队列发送统计
HI_PUBLIC HI_VOID mrsDfxMsgTxCnt(HI_U32);

// 消息队列发送失败统计
HI_PUBLIC HI_VOID mrsDfxMsgTxFailCnt(HI_U32);

// 消息队列接收统计
HI_PUBLIC HI_VOID mrsDfxMsgRxCnt(HI_SYS_QUEUE_MSG_S *);

// 消息处理耗时
HI_PUBLIC HI_VOID mrsDfxMsgPrcoTimeConsuming(HI_U32, HI_U32);

// 广播发送PLC帧统计
HI_PUBLIC HI_VOID mrsDfxPlcBcTx(HI_DMS_CHL_TX_ADDR_S *);

// 广播发送PLC帧错误统计
HI_PUBLIC HI_VOID mrsDfxPlcBcTxErr(HI_DMS_CHL_TX_ADDR_S *);

// 广播接收PLC帧统计
HI_PUBLIC HI_VOID mrsDfxPlcBcRx(HI_VOID);

// 广播读表次数统计
HI_PUBLIC HI_VOID mrsDfxBcRmCnt(HI_VOID);

// 广播读表成功次数统计
HI_PUBLIC HI_VOID mrsDfxBcRmOkCnt(HI_VOID);

// 广播读表非广播响应次数统计
HI_PUBLIC HI_VOID mrsDfxBcRmNonBcRespCnt(HI_VOID);

// 广播读表失败次数统计
HI_PUBLIC HI_VOID mrsDfxBcRmFailCnt(HI_VOID);

// 刷新读表队列深度统计
HI_PUBLIC HI_VOID mrsDfxRefreshRmQueueNum(HI_U8);

// 刷新读表队列队首状态
HI_PUBLIC HI_VOID mrsDfxRefreshRmQueueTopStatus(MRS_SRV_QUEUE *, HI_U8 );

// 读表队列满统计
HI_PUBLIC HI_VOID mrsDfxRmQueueFullCnt(HI_VOID);

/* BEGIN: DEL by liuxipeng/lKF54842, 2014/9/10 DTS: DTS2014091004895 */
// DEL EVT
/* END:   DEL by liuxipeng/lKF54842, 2014/9/10 DTS: DTS2014091004895 */

/* BEGIN: Modified by fengxiaomin/00209182, 2014/3/27   问题单号:DTS2014032906596 */
HI_DSID_APP_RM_RX_TX_INF_S * mrsDfxGetRmTxRxInf(HI_VOID);
HI_BOOL mrsIsTestDI(HI_U8 *pData, HI_U16 usDataLen);
HI_VOID mrsCcoDfxRmUartRx(HI_U8 *pData, HI_U16 usDataLen, HI_U8 ucFlag);
HI_VOID mrsStaDfxRmUartRx(HI_U16 usId, HI_U8 *pData, HI_U16 usDataLen);
HI_VOID mrsStaDfxRmRxDeny(HI_U16 usId, HI_U8 ucDenyFlag);
HI_VOID mrsDfxStaUartTimeOut(HI_U16 usId);
HI_VOID mrsStaDfxRmRxErrData(HI_U16 usId, HI_U8 *pData, HI_U16 usDataLen);
HI_VOID mrsDfxRmUartTx(HI_U8 *pData, HI_U16 usDataLen, HI_BOOL bFlag);
HI_VOID mrsDfxRmUartDenyTx(HI_U8 *pData, HI_U16 usDataLen);
HI_VOID mrsDfxRmPlcUpAfn(HI_U16 usId, HI_U8 *pData, HI_U32 ulRet);
HI_VOID mrsDfxRmPlcTx(HI_U8 *pData, HI_U16 usDataLen, HI_U32 ulRet);
HI_VOID mrsDfxRmPlcUpRx(HI_U16 usId, HI_U8* pData, HI_BOOL ucExceedFlag);
HI_VOID mrsDfxRmPlcDlRx(HI_U16 usId, HI_U8* pData, HI_BOOL bTimeoutFlag);
HI_VOID mrsDfxRmPlcRx(HI_U8 *pData, HI_U16 usDataLen);
HI_VOID mrsDfxRmPlcBcRx(HI_VOID);
HI_VOID mrsDfxRmPlcExceedRx(HI_U8 *pData, HI_U16 usDataLen);
HI_VOID mrsDfxRmPlcTimeOut(HI_U8 *pData, HI_U16 usDataLen);
/* END:   Modified by fengxiaomin/00209182, 2014/3/27 */
HI_VOID mrsDfxCurrGetTimeStatus(HI_U8 status);
HI_VOID mrsDfxGetTimeSucc(HI_BOOL ucSuccFlag);
HI_VOID mrsDfxPowerOnGetTime(HI_U8 clockBuf[6]);
HI_VOID mrsDfxGetTimeTotalCnt(HI_VOID);
HI_VOID mrsDfxGetTimeFailCnt(HI_VOID);


HI_VOID mrsDfxCcoQueueCurrRetry(HI_U8 ucCurrRetry);
HI_VOID mrsDfxCcoQueueMallocFailCnt(HI_VOID);
HI_VOID mrsDfxCcoQueueCurrItem(HI_U16 usCurrItem);
HI_VOID mrsDfxCcoQueueCurrAfn(HI_U8 ucAfn);
HI_VOID mrsDfxCcoQueueCurrFn(HI_U16 ucFn);
HI_VOID mrsDfxCcoQueueJoinFailCnt(HI_VOID);
HI_VOID mrsDfxCcoQueueJoinCnt(HI_VOID);
HI_VOID mrsDfxCcoQueueReportCnt(HI_VOID);
HI_VOID mrsDfxCcoQueueRxAckCnt(HI_VOID);
HI_VOID mrsDfxCcoQueueRxDenyCnt(HI_VOID);
HI_VOID mrsDfxCcoQueueReportTimeOutCnt(HI_VOID);


HI_VOID mrsDfxCcoEvtBakSave(HI_U16 usBakTime);
HI_VOID mrsDfxCcoEvtFlag(HI_BOOL bEvtFlag);
HI_VOID mrsDfxEvtJoinCcoQueueFailCnt(HI_VOID);
HI_VOID mrsDfxEvtJoinCcoQueueMallocFailCnt(HI_VOID);
HI_VOID mrsDfxCcoEvtQueueItemNum(HI_U16 usItemNum);
HI_VOID mrsDfxCcoEvtCompatibilityErrCnt(HI_VOID);
HI_VOID mrsDfxCcoEvtReportTimeOut(HI_VOID);
HI_VOID mrsDfxCcoEvtReportCnt(HI_VOID);
HI_VOID mrsDfxCcoEvtRxAckCnt(HI_VOID);
HI_VOID mrsDfxCcoEvtRxPlcCnt(HI_VOID);
HI_VOID mrsDfxCcoEvtFilterPlcCnt(HI_VOID);
HI_VOID mrsDfxCcoEvtReportedFilterCnt(HI_VOID);
HI_VOID mrsDfxCcoEvtRxReportDisableCnt(HI_VOID);
HI_VOID mrsDfxCcoEvtRxProtoErrCnt(HI_VOID);
HI_VOID mrsDfxCcoEvtTxAckPlcCnt(HI_VOID);
HI_VOID mrsDfxCcoEvtTxForbidPlcCnt(HI_VOID);
HI_VOID mrsDfxCcoEvtTxFullPlcCnt(HI_VOID);
HI_VOID mrsDfxCcoEvtTxAllowPlcCnt(HI_VOID);
HI_VOID mrsDfxCcoForbidEvtRxPlcCnt(HI_VOID);
HI_VOID mrsDfxCcoEvtQueueFrontRear(MRS_SRV_QUEUE *pstQueue);
HI_VOID mrsDfxCcoFlashMemErrCnt(HI_U8 Index);
HI_VOID mrsDfxCcoFlashMemMallocCnt(HI_U32 ulIncCnt, HI_U8 Index);
HI_VOID mrsDfxEvtReportedListMallocFailCnt(HI_VOID);
HI_VOID mrsDfxCcoEvtListItemNum(HI_U8 ucItemNum);
HI_VOID mrsDfxEvtJoinCcoQueueTime(HI_U32 ulTime);
HI_VOID mrsDfxCcoEvtRxPlcTime(HI_U32 ulTime);
HI_VOID mrsDfxCcoEvtRepReplaceHeadCnt(HI_VOID);
HI_VOID mrsDfxCcoEvtRepReplaceSameAddrCnt(HI_VOID);
HI_VOID mrsDfxCcoEvtRepAgedCnt(HI_VOID);

HI_VOID mrsDfxStaEvtRetry(HI_U8 ucRetry);
HI_VOID mrsDfxStaEvtWaitProcStat(HI_U8 ucStatus);
HI_VOID mrsDfxStaEvtReportSeq(HI_U16 usSeq);
HI_VOID mrsDfxStaHardEvtPinStat(HI_U8 ucEvtFlag);
HI_VOID mrsDfxStaCurrEvtPinStat(HI_U8 ucEvtFlag);
HI_VOID mrsDfxStaMeterEvtType(HI_U8 ucMeterEvtType);
HI_VOID mrsDfxStaEvtRespDenyCnt(HI_VOID);
HI_VOID mrsDfxStaMeterEvtQueryCnt(HI_VOID);
HI_VOID mrsDfxStaMeterEvtRespCnt(HI_VOID);
HI_VOID mrsDfxStaEvtStatusTimeOutCnt(HI_VOID);
HI_VOID mrsDfxStaEvtLastStatusRxFlag(HI_U8 ucLastRxFlag);
HI_VOID mrsDfxStaEvtPinHighCnt(HI_VOID);
HI_VOID mrsDfxStaEvtPinLowCnt(HI_VOID);
HI_VOID mrsDfxStaPlcEvtReportTimeOutCnt(HI_VOID);
HI_VOID mrsDfxStaPlcEvtReportTotalCnt(HI_VOID);
HI_VOID mrsDfxStaPlcEvtReportTimeStamp(HI_U32 ulSendTime);
HI_VOID mrsDfxStaEvtPinHighTimeStamp(HI_U32 ulTime);
HI_VOID mrsDfxStaEvtPinLowTimeStamp(HI_U32 ulTime);
HI_VOID mrsDfxStaPlcEvtAckCnt(HI_VOID);
HI_VOID mrsDfxStaPlcEvtAckTimeStamp(HI_U32 ulTime);
HI_VOID mrsDfxStaPlcEvtFullCnt(HI_VOID);
HI_VOID mrsDfxStaRxEvtAllowCnt(HI_VOID);
HI_VOID mrsDfxStaRxEvtForbidCnt(HI_VOID);
HI_VOID mrsDfxStaReadEvtRegCnt(HI_VOID);
HI_VOID mrsDfxStaEvtWaitReportRegCnt(HI_VOID);
HI_VOID mrsDfxStaPlcEvtFullTimeStamp(HI_U32 ulTime);
HI_VOID mrsDfxStaEvtRxAllowTime(HI_U32 ulTime);
HI_VOID mrsDfxStaEvtRxForbidTime(HI_U32 ulTime);

HI_VOID mrsDfxStaEvtCurrMonStatus(HI_U8 ucStatus);
HI_VOID mrsDfxStaEvtMonExistEvtCnt(HI_VOID);
HI_VOID mrsDfxStaEvtMonSetWaitPlcFlag(HI_BOOL bFlag);
HI_VOID mrsDfxStaEvtMonWaitPlcQueryCnt(HI_VOID);
HI_VOID mrsDfxStaEvtMonTimeStamp(HI_U32 ulTime);
HI_VOID mrsDfxStaEvtMonStartTimeStamp(HI_U32 ulTime);
HI_VOID mrsDfxStaEvtMonEndTimeStamp(HI_U32 ulTime);

// CCO载波从节点在线状态变化上报相关统计量
HI_U32 mrsDfxCcoSaveMACReportChgEntry(HI_MDM_NETWORK_TOPOCHG_ENTRY_S* pstEntry);
HI_VOID mrsDfxCcoChgFrameQueueStats(HI_BOOL bIncrease, HI_U8 ucEntryNum);
HI_VOID mrsDfxCcoChgJoinCcoQueueStats(HI_U8 ucEntryNum);
HI_VOID mrsDfxCcoChgReportRxResp(HI_VOID);
HI_VOID mrsDfxCcoChgReportTimeout(HI_VOID);

HI_VOID mrsDfxRefreshLastDataU8(HI_U8 *data_buf, HI_U16 num, HI_U8 new_data);
HI_VOID mrsDfxRefreshLastDataU32(HI_U32 *data_buf, HI_U16 num, HI_U32 new_data);

HI_VOID mrsDfxCcoDetectEnable(HI_U8 ucEnable);
HI_VOID mrsDfxCcoDetectStatus(HI_U8 ucDetectStatus);
HI_VOID mrsDfxCcoDetectJoinWhiteFlag(HI_U8 ucJoinWhiteFlag);
HI_VOID mrsDfxCcoDetectAllowJoinWhiteReason(HI_U8 ucAllowJoinWhiteReason);
HI_VOID mrsDfxCcoDetectForbidJoinWhiteReason(HI_U8 ucForbidJoinWhiteReason);
HI_VOID mrsDfxCcoDetectAllowJoinWhiteTime(HI_U32 ulAllowJoinWhiteTime);
HI_VOID mrsDfxCcoDetectForbidJoinWhiteTime(HI_U32 ulForbidJoinWhiteTime);
HI_VOID mrsDfxCcoDetectSetWaitTopoFlag(HI_U8 ucSetWaitTopoFlag);
HI_VOID mrsDfxCcoDetectRmFlag(HI_U8 ucRmFlag);
HI_VOID mrsDfxCcoDetectExternFlag(HI_U8 ucExternFlag);
HI_VOID mrsDfxCcoDetectOriginalEnable(HI_U8 ucOriginalEnable);
HI_VOID mrsDfxCcoDetectCurrTurn(HI_U8 ucCurrTurn);
HI_VOID mrsDfxCcoDetect07DIIndex(HI_U8 uc97DIIndex);
HI_VOID mrsDfxCcoDetectWhiteNum(HI_U16 usDetectWhiteNum);
HI_VOID mrsDfxCcoTopoStatus(HI_U8 ucTopoStatus);
HI_VOID mrsDfxCcoDetectPlcDisconnectTime(HI_U32 ulTime);
HI_VOID mrsDfxCcoArchivesClearTime(HI_U32 ulTime);
HI_VOID mrsDfxCcoArchives0To1Time(HI_U32 ulTime);
HI_VOID mrsDfxCcoDetectExternDisableTime(HI_U32 ulTime);
HI_VOID mrsDfxCcoDetectExternResumeTime(HI_U32 ulTime);
HI_VOID mrsDfxCcoDetectStartTime(HI_U32 ulTime);
HI_VOID mrsDfxCcoDetectEndTime(HI_U32 ulTime);
HI_VOID mrsDfxCcoDetectEndReason(HI_U8 ucReason);
HI_VOID mrsDfxCcoDetectBlackNum(HI_U16 usBlackListNum);
HI_VOID mrsDfxCcoDetectWhiteValidNum(HI_U16 usDetectWhiteValidNum);
HI_VOID mrsDfxCcoDetectRefreshWhiteFlag(HI_U8 ucFlag);


/* BEGIN: PN: DTS2016021906890 MODIFY\ADD\DEL by cuiate/00233580 at 2016/2/21 */
HI_PUBLIC HI_VOID mrsDfxSmFrameValid(HI_VOID);
HI_PUBLIC HI_VOID mrsDfxSmFrameInvalid(HI_VOID);
HI_PUBLIC HI_VOID mrsDfxMapFrameValid(HI_VOID);
HI_PUBLIC HI_VOID mrsDfxMapFrameInvalid(HI_VOID);
/* END:   PN: DTS2016021906890 MODIFY\ADD\DEL by cuiate/00233580 at 2016/2/21 */

HI_VOID mrsDfxTestModeCnt(HI_U8 ucMode);
HI_VOID mrsDfxTestModeEnter(HI_U8 ucMode,HI_U8 ucCurMode);
HI_VOID mrsDfxTestModeExit(HI_U8 ucMode,HI_U8 ucCurMode);
HI_VOID mrsDfxTestModeAppBuf(HI_U8 *pAppData, HI_U16 usLen);
HI_VOID mrsDfxTestModeErrPbbCnt(HI_U8 ucPayloadType);
HI_VOID mrsDfxTestModeErrParseCnt(HI_U32 ulRet);
HI_VOID mrsDfxTestModeSavePayload(HI_U8 *pPayloadData, HI_U16 usLen);
HI_VOID mrsDfxRefreshTestMode(HI_U8 ucMode);

#endif
