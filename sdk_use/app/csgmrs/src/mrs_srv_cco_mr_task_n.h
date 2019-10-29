//*****************************************************************************
//
//                  版权所有 (C), 2001-2014, 华为技术有限公司
//
//*****************************************************************************
//  文 件 名   : mrs_srv_cco_mr_task_n.h
//  版 本 号   : V1.0
//  作    者   : cuiate/c00233580
//  生成日期   : 2016-07-09
//  功能描述   : TODO: ...
//
//  函数列表   : TODO: ...
//  修改历史   :
//  1.日    期 : 2016-07-09
//    作    者 : cuiate/c00233580
//    修改内容 : 创建文件
//
//*****************************************************************************

#ifndef _MRS_SRV_CCO_MR_TASK_N_H_
#define _MRS_SRV_CCO_MR_TASK_N_H_
HI_START_HEADER

#if (defined(PRODUCT_CFG_PRODUCT_TYPE_CCO) && defined(PRODUCT_CFG_PRODUCT_TYPE_CSG_GD))
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------

typedef struct
{
    HI_U16 usId;
    HI_U16 reserved;
    MRS_SRV_LINK *pLink;
} MRS_CCO_TASK_ITEM_PARAM;


typedef struct
{
    HI_U16 usId;
    HI_U16 reserved;
    MRS_CCO_MR_TASK_ITEM_STRU *pstItem;
} MRS_CCO_MR_TASK_TRAVERSE_PARAM;

/*lint -e958*/
/*lint -e959*/
#pragma pack(1)
typedef struct
{
    HI_U16 usId;

    HI_U8  ucPrio   : 4;
    HI_U8  reserved : 3;
    HI_U8  ucResp   : 1;

    HI_U16 usDestAddrNum;
    HI_U8  aucDestAddrList[0];
} MRS_CCO_TASK_DETAIL_HEAD_S;

typedef struct
{
    HI_U16 usId;
    HI_U8  aucNodeAddr[HI_METER_ADDR_LEN];
    HI_U8  ucStatus;
} MRS_CCO_MR_TASK_REPORT_STATUS_S;
#pragma pack()
/*lint +e959*/
/*lint +e958*/


MRS_CCO_MR_TASK_CONTEXT *mrsCcoGetMrTaskCtx(HI_VOID);
HI_VOID mrsCcoMrTaskItemFree(HI_VOID *pNode, HI_VOID *pParam);
HI_BOOL mrsCcoMrTaskIsFull(HI_VOID);
HI_BOOL mrsCcoMrTaskIsInList(HI_U16 usId);
HI_U32 mrsCcoMrTaskMsgSend(HI_U32 ulMsgId);
HI_VOID mrsCcoMrTaskComRx(HI_VOID *pParam);
HI_U32 mrsCcoMrTaskComMaxtryTimeout(HI_VOID *pParam);
HI_VOID mrsCcoMrTaskBcNotify(HI_VOID *pParam);
HI_VOID mrsCcoMrTaskSetFlowCtrl(HI_BOOL bEnable);
HI_BOOL mrsCcoMrTaskIsFlowCtrl(HI_VOID);
HI_BOOL mrsCcoMrTaskIsReportItem(HI_VOID *pvItem);
HI_U32 mrsCcoMrTaskEveryTimeOut(HI_VOID *pParam);

HI_BOOL mrsCcoMrTaskTraverseCallback(HI_VOID *, HI_VOID *);
MRS_CCO_MR_TASK_ITEM_STRU *mrsCcoMrTaskGetItem(HI_U16 usId);

HI_U32 mrsCcoMrTaskCheckTimeout(MRS_CCO_MR_TASK_CONTEXT *pstCtx);

// 处理抄表列表
HI_U32 mrsCcoMrTaskProcList(MRS_CCO_MR_TASK_CONTEXT *pstCtx);

// 处理抄表列表: 透抄方式
HI_U32 mrsCcoMrTaskProcListXr(MRS_CCO_MR_TASK_CONTEXT *pstCtx);

// 处理抄表列表: 并发方式
HI_U32 mrsCcoMrTaskProcListPr(MRS_CCO_MR_TASK_CONTEXT *pstCtx);

// 处理抄表列表: 广播校时
HI_U32 mrsCcoMrTaskProcListBc(MRS_CCO_MR_TASK_CONTEXT *pstCtx);

HI_BOOL mrsCcoMrTaskIsBcItem(MRS_CCO_MR_TASK_ITEM_STRU *pstItem);

HI_BOOL mrsCcoMrTaskIsXrItem(MRS_CCO_MR_TASK_ITEM_STRU *pstItem);

HI_U32 mrsCcoMrTaskProcListPrUnicast(MRS_CCO_MR_TASK_CONTEXT *pstCtx);

HI_U32 mrsCcoMrTaskProcListPrMulticast(MRS_CCO_MR_TASK_ITEM_STRU *pstItem);

/* BEGIN: PN: DTS2016081300132 MODIFY\ADD\DEL by cuiate/00233580 at 2016/8/13 */
HI_BOOL mrsCcoMrTaskIsMultiListValid(HI_U8 *pucMeterList, HI_U16 usMeterNum);

HI_BOOL mrsCcoMrTaskIsMeterRepeat(HI_U8 *pucMeterList, HI_U16 usMeterIndex, HI_U16 usMeterNum);
/* END:   PN: DTS2016081300132 MODIFY\ADD\DEL by cuiate/00233580 at 2016/8/13 */


//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
#endif

HI_END_HEADER
#endif //_MRS_SRV_CCO_TASK_N_H_


