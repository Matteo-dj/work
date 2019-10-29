//*****************************************************************************
//
//                  版权所有 (C), 1998-2011, 华为技术有限公司
//
//*****************************************************************************
//  文 件 名   : mrs_srv_cco_searchmeter.h
//  版 本 号   : V1.0
//  作    者   :
//  生成日期   :
//  功能描述   : 搜表模块函数及接口定义
//
//  函数列表   : TODO: ...
//  修改历史   :
//  1.日    期 : 2012-05-18
//    作    者 : fengxiaomin/f00209182
//    修改内容 : 创建文件
//
//*****************************************************************************

//*****************************************************************************
// PROJECT   :
// SUBSYSTEM :
// MODULE    :
// OWNER     :
//*****************************************************************************


#ifndef __MRS_SRV_CCO_SEARCHMETER_H__
#define __MRS_SRV_CCO_SEARCHMETER_H__

HI_START_HEADER

#define SERIES_FIND_INVALID_NODE_MAX_COUNT  108         //连续找到无效节点计数
#define SCAN_MAP_CYCLE_TIMER_VAL            (1000 * 5)  //搜表时周期性扫描MAP定时器
#define MRS_SEARCH_REPORT_NODE_NUM          1           //每帧搜表结果上报时的节点数量
#define MRS_COLLECTOR_DEFAULT_PROTOCOL      0           //采集器默认通信协议类型

// 节点查询结果
typedef enum
{
    MRS_CCO_SM_RESULT_QUERYING,     // 正在查询搜表结果
    MRS_CCO_SM_RESULT_QUERY_SUC,    // 搜表结果查询成功
    MRS_CCO_SM_RESULT_BIND_SUC,     // 锁定成功
} MRS_CCO_SM_RESULT_E;

typedef enum
{
    MRS_CCO_SEARCH_FINISH_NORMAL,
    MRS_CCO_SEARCH_FINISH_TIMEOUT,
    MRS_CCO_SEARCH_FINISH_STOP,
    MRS_CCO_SEARCH_FINISH_ROUTE_RESET,
    MRS_CCO_SEARCH_FINISH_ROUTE_RESUME,
} MRS_CCO_SERACH_FINISH_REASON_E;


//*****************************************************************************
// 函数名称: mrsCcoSearchMeterInit
// 功能描述: 搜表模块初始化
//
// 参数说明:
//   pstCcoCtx [in/out] 当前状态
//
// 返 回 值:
//   HI_ERR_SUCCESS 成功
//   0xFFFFFFFF地址映射表错误或读取失败
// 调用要求: TODO: ...
// 调用举例: TODO: ...
// 作    者: fengxiaomin/f00209182 [2012-05-24]
//*****************************************************************************
HI_U32 mrsCcoSearchMeterInit(MRS_CCO_SRV_CTX_STRU * pstCcoCtx, HI_U32 ulSearchMode);


//*****************************************************************************
// 函数名称: mrsCcoSearchReportSlaveNodeInfo
// 功能描述: 上报表信息处理函数
//
// 参数说明:
//   This function has no arguments.
//
// 返 回 值:
//
// 调用要求: TODO: ...
// 调用举例: TODO: ...
// 作    者: fengxiaomin/f00209182 [2012-05-18]
//*****************************************************************************
HI_VOID mrsCcoSearchReportSlaveNodeInfo(HI_VOID);


//*****************************************************************************
// 函数名称: mrsReportTimeOutFun
// 功能描述: 上报超时处理函数
//
// 参数说明:
//   pstCcoCtx [in/out] 当前状态
//
// 返 回 值:
//
// 调用要求: TODO: ...
// 调用举例: TODO: ...
// 作    者: fengxiaomin/f00209182 [2012-05-22]
//*****************************************************************************
HI_U32 mrsReportTimeOutFun(HI_VOID*);


//*****************************************************************************
// 函数名称: mrsAckFun
// 功能描述: 上报确认处理函数
//
// 参数说明:
//   This function has no arguments.
//
// 返 回 值:
//
// 调用要求: TODO: ...
// 调用举例: TODO: ...
// 作    者: fengxiaomin/f00209182 [2012-05-22]
//*****************************************************************************
HI_VOID mrsAckFun(HI_VOID * pAckParam);

//*****************************************************************************
// 函数名称: mrsStopSearchMeter
// 功能描述: 停止搜表流程
//
// 参数说明:
//   This function has no arguments.
//
// 返 回 值:
//   HI_ERR_SUCCESS 查找到有效节点
//   0xFFFFFFFF地址映射表错误或读取失败
// 调用要求: TODO: ...
// 调用举例: TODO: ...
// 作    者: fengxiaomin/f00209182 [2012-05-24]
//*****************************************************************************
HI_U32 mrsStopSearchMeter(HI_VOID);


//AFN06 F3 主动上报路由工况变动信息
HI_U32 mrs3762_ReportAFN06_F3(HI_U8 status);

HI_U32 mrsReportRouterInfs(HI_U8 status);

HI_U32 mrsCsg_ReportRouterInfs(HI_U8 status);

HI_VOID mrsReportAFN06F3AckProcess(HI_VOID *param);

/* BEGIN: PN: DTS2015042907476 MODIFY\ADD\DEL by cuiate/00233580 at 2015/4/29 */
HI_VOID mrsSearchMeterPrepare(MRS_CCO_SRV_CTX_STRU* pstCcoCtx);

HI_U32 mrsSearchMeterProc(MRS_CCO_SRV_CTX_STRU* pstCcoCtx);
/* END:   PN: DTS2015042907476 MODIFY\ADD\DEL by cuiate/00233580 at 2015/4/29 */

HI_U32 mrsCcoStartSearchMeter(MRS_CCO_SRV_CTX_STRU * cco);

HI_U32 mrsCcoStopSearchMeter(MRS_CCO_SRV_CTX_STRU * cco);

HI_U32 mrsCcoSearchMeterTimeoutProcess(HI_VOID);

HI_U32 mrsCcoSendSearchMeterPlcFrame(MRS_CCO_SRV_CTX_STRU * cco);

HI_U32 mrsCcoSearchMeterGetMeterlist(MRS_CCO_SRV_CTX_STRU *cco);

HI_U32 mrsCcoSearchMeterTopoQueryProc(MRS_CCO_SRV_CTX_STRU *cco);

HI_U32 mrsCcoSearchMeterSendBindCmd(MRS_CCO_SRV_CTX_STRU *cco);

HI_U32 mrsCcoSearchMeterSaveFinishReason(MRS_CCO_SRV_CTX_STRU *cco, HI_U8 reason);

HI_U32 mrsCmdCcoGetMeterList(HI_U16 id, HI_PVOID req_packet, HI_U16 req_packet_size);

HI_U32 mrsReportTotalTimeOutFun(HI_VOID * param);

HI_VOID mrsCcoSmRestoreWhitelistSwitch(HI_VOID);

HI_VOID mrsCcoSmWhitelistSwitchTimerReset(HI_VOID);

HI_BOOL mrsCcoSearchMacRepeatCheck(MRS_CCO_SRV_CTX_STRU *pCcoCtx, HI_U8 *pMacAddr);

HI_U32 mrsCcoUpdateSearchResult(MRS_CCO_SRV_CTX_STRU *pCcoCtx, HI_U8 *pMacAddr);

HI_VOID mrsCcoUpdateBindResult(MRS_CCO_SRV_CTX_STRU * cco, HI_U8 * mac);

HI_END_HEADER



#endif //__MRS_SRV_CCO_SEARCHMETER_H__

