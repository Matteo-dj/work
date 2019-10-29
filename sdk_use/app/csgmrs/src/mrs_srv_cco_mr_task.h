//*****************************************************************************
//
//                  版权所有 (C), 2001-2014, 华为技术有限公司
//
//*****************************************************************************
//  文 件 名   : mrs_srv_cco_mr_task.h
//  版 本 号   : V1.0
//  作    者   : cuiate/c00233580
//  生成日期   : 2015-12-09
//  功能描述   : TODO: ...
//
//  函数列表   : TODO: ...
//  修改历史   :
//  1.日    期 : 2015-12-09
//    作    者 : cuiate/c00233580
//    修改内容 : 创建文件
//
//*****************************************************************************

#ifndef _MRS_SRV_CCO_MR_TASK_H_
#define _MRS_SRV_CCO_MR_TASK_H_
HI_START_HEADER

#if (defined(PRODUCT_CFG_PRODUCT_TYPE_CCO) && defined(PRODUCT_CFG_PRODUCT_TYPE_CSG_GD))

#define MRS_CCO_MR_TASK_MAX             (300)

#define MRS_CCO_MR_TASK_MASTER_TIME     (8000)
#define MRS_CCO_MR_TASK_MASTER_MAXTRY   (2)

#define MRS_CCO_MR_TASK_TYPE_UNICAST    (0)
#define MRS_CCO_MR_TASK_TYPE_MULTICAST  (1)

#define MRS_CCO_MR_TASK_FLAG_NORESP     (0)
#define MRS_CCO_MR_TASK_FLAG_RESP       (1)

#define MRS_CCO_MR_TASK_PAUSE           (0)
#define MRS_CCO_MR_TASK_ENABLE          (1)

#define MRS_CCO_MR_TASK_UNI_NORMAL      (0) // 普通抄表
#define MRS_CCO_MR_TASK_UNI_BROADCAST   (1) // 广播校时

#define MRS_CCO_MR_TASK_MULTI_NORMAL    (0) // 普通抄表
#define MRS_CCO_MR_TASK_MULTI_ALLMETER  (1) // 抄读所有表

#define MRS_CCO_REPORTLIST_THRESHOLD    (5)

#define MRS_CCO_MR_TASK_ID_MAX          (0xEFFF)

#define MRS_CCO_MR_TASK_NUM_MAX         (0xFFFF)

typedef enum
{
    MRS_CCO_MR_TASK_PRIORITY_0 = 0,
    MRS_CCO_MR_TASK_PRIORITY_1,
    MRS_CCO_MR_TASK_PRIORITY_2,
    MRS_CCO_MR_TASK_PRIORITY_3,
    MRS_CCO_MR_TASK_PRIORITY_MAX
} MRS_CCO_MR_TASK_PRIORITY_E;


typedef enum
{
    MRS_CCO_MR_TASK_MSG_NOTIFY = 0,
    MRS_CCO_MR_TASK_MSG_NEXT
} MRS_CCO_MR_TASK_MSG_E;


typedef struct
{
    HI_U16 usTimeout;
    HI_U8 ucType;
    HI_U8 reserved;
    HI_U8 aucSrcAddr[HI_METER_ADDR_LEN];
    HI_U8 aucDstAddr[HI_METER_ADDR_LEN];
} MRS_CCO_MR_TASK_PARAM_UNICAST;


typedef struct
{
    HI_U16 usSize;
    HI_U16 usCnt;
    HI_U8 pBitmap[0];
} MRS_SRV_BITMAP;


typedef struct
{
    HI_U16 usTimeout;
    HI_U16 usSubNodeNum;
    HI_U16 usIndex;
    HI_U8 ucType;
    HI_U8 reserved;
    HI_U8 *pSubNode;
    MRS_SRV_BITMAP *pBitmap;
} MRS_CCO_MR_TASK_PARAM_MULTICAST;


typedef struct
{
    MRS_SRV_LINK stLink;
    HI_U16 usId;
    HI_U8 ucType     : 3;
    HI_U8 ucRespFlag : 1;
    HI_U8 ucPriority : 4;
    HI_U8 ucFrameLen;
    HI_U8 *pFrame;
    HI_U32 ulTime;
    union
    {
        MRS_CCO_MR_TASK_PARAM_UNICAST stUnicast;
        MRS_CCO_MR_TASK_PARAM_MULTICAST stMulticast;
    } unParam;
} MRS_CCO_MR_TASK_ITEM_STRU;


typedef struct
{
    HI_U16 usTaskMax;
    HI_U16 usTaskNum;
    HI_U8 ucStatus;
    HI_BOOL bFlowCtrl;
    HI_U8 reserved[2];
    MRS_SRV_LIST stList;
    MRS_CCO_MR_TASK_ITEM_STRU *pstProcItem;
} MRS_CCO_MR_TASK_CONTEXT;


typedef enum
{
    MRS_CCO_MR_TASK_MODE_NA,
    MRS_CCO_MR_TASK_MODE_XR,    // 透抄
    MRS_CCO_MR_TASK_MODE_BC,    // 广播
    MRS_CCO_MR_TASK_MODE_PR,    // 并发
} MRS_CCO_MR_TASK_PROC_MODE_E;



HI_EXTERN HI_U32 mrsCcoMrTaskInit(HI_VOID);

HI_EXTERN HI_U32 mrsCcoMrTaskDeinit(HI_VOID);

HI_EXTERN HI_U32 mrsCcoMrTaskInsertUnicast(MRS_CSG_FRAME_STRU *pFrameData, HI_U8 *pucDenyCode);

HI_EXTERN HI_U32 mrsCcoMrTaskInsertMulticast(MRS_CSG_FRAME_STRU *pFrameData, HI_U8 *pucDenyCode);

HI_EXTERN HI_U32 mrsCcoMrTaskInsert(MRS_CCO_MR_TASK_ITEM_STRU *pstNode);

HI_EXTERN HI_U32 mrsCcoMrTaskDelete(HI_U16 usId);

HI_EXTERN HI_U32 mrsCcoMrTaskDeleteAll(HI_VOID);

HI_EXTERN HI_U16 mrsCcoMrTaskMax(HI_VOID);

HI_EXTERN HI_U16 mrsCcoMrTaskRemain(HI_VOID);

HI_EXTERN HI_U16 mrsCcoMrTaskNum(HI_VOID);

HI_EXTERN HI_U32 mrsCcoMrTaskMsgProc(HI_SYS_QUEUE_MSG_S *pstMsg);

HI_EXTERN HI_U32 mrsCcoMrTaskNotify(HI_VOID);

HI_EXTERN HI_U32 mrsCcoMrTaskActive(HI_VOID);

HI_EXTERN HI_U32 mrsCcoMrTaskProc(HI_VOID);

HI_EXTERN HI_U32 mrsCcoMrTaskReportNoResponse(HI_VOID);

HI_EXTERN HI_U32 mrsCcoMrTaskReportXrFrame(HI_U8 *pucFrame, HI_U16 usLength);

HI_EXTERN HI_U32 mrsCcoMrTaskReportFrameProc(MRS_CCO_MR_TASK_ITEM_STRU *pstItem, HI_U8 *pucFrame, HI_U16 usLength);

HI_EXTERN HI_U32 mrsCcoMrTaskGetIdBuffer(HI_U16 usStart, HI_U8 ucTaskNum, HI_U8 **ppBuffer, HI_U16 *pusLength);

HI_EXTERN HI_U32 mrsCcoMrTaskGetTaskInfo(HI_U16 usId, HI_U8 **ppBuffer, HI_U16 *pusLength);

HI_EXTERN HI_VOID mrsCcoMrTaskSetStatus(HI_U8 ucStatus);

HI_EXTERN HI_BOOL mrsCcoMrTaskIsEnabled(HI_VOID);

// 获取抄表处理方式
HI_EXTERN MRS_CCO_MR_TASK_PROC_MODE_E mrsCcoMrTaskGetMode(HI_VOID);

HI_EXTERN HI_VOID mrsCcoMrTaskRemoveNode(MRS_CCO_MR_TASK_ITEM_STRU *pstItem);

HI_EXTERN HI_U32 mrsCcoMrTaskReportItemNak(MRS_CCO_MR_TASK_ITEM_STRU *pstItem, HI_U8 *pucAddr, HI_U8 ucErrCode);

HI_EXTERN HI_BOOL mrsCcoMrTaskIsItemTimeout(MRS_CCO_MR_TASK_ITEM_STRU *pstItem);


HI_EXTERN HI_U32 mrsSrvAllocBits(MRS_SRV_BITMAP **pstBitmap, HI_U16 usSize);

HI_EXTERN HI_VOID mrsSrvFreeBits(MRS_SRV_BITMAP **pstBitmap);

HI_EXTERN HI_U32 mrsSrvBitSet(MRS_SRV_BITMAP *pstBitmap, HI_U16 usPos);

HI_EXTERN HI_BOOL mrsSrvBitTest(MRS_SRV_BITMAP *pstBitmap, HI_U16 usPos);

HI_EXTERN HI_BOOL mrsSrvBitIsFull(MRS_SRV_BITMAP *pstBitmap);
HI_EXTERN HI_BOOL mrsCcoMrTaskIsChlAvailable(HI_VOID);


#endif

HI_END_HEADER
#endif //_MRS_SRV_CCO_TASK_H_
