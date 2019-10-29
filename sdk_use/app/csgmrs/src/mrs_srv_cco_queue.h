//*****************************************************************************
//
//                  版权所有 (C), 1998-2011, 华为技术有限公司
//
//*****************************************************************************
//  文 件 名   : mrs_srv_cco_queue.h
//  版 本 号   : V1.0 
//  作    者   : fengxiaomin/00209182
//  生成日期   : 2014-8-16
//  功能描述   : CCO与集中器之间的交互报文队列处理，目前只有等待集中器确认的帧进队列
//               
//  函数列表   : 
//  修改历史   : 
//  1.日    期 : 2014-8-16
//    作    者 : fengxiaomin/00209182
//    修改内容 : 创建文件 
//
//*****************************************************************************

#ifndef __MRS_SRV_CCO_QUEUE_H__
#define __MRS_SRV_CCO_QUEUE_H__
HI_START_HEADER

#define MRS_CCO_QUEUE_IDLE_STATUS            (0)
#define MRS_CCO_QUEUE_WAIT_ACK_STATUS        (1)

typedef struct
{
    MRS_SRV_QUEUE stCcoQueue;  //队列
    HI_U8 ucStatus;
    HI_U8 ucRetry;            //重试次数
    HI_U8 ucLock;               //锁  
    HI_U8 reserved[3];
    HI_U16 usItemNum;
} MRS_SRV_CCO_QUEUE, *P_MRS_SRV_CCO_QUEUE;

typedef struct
{
    MRS_SRV_NODE link;
    HI_BOOL bValid;
    HI_U8 ucAfn;
    HI_U16 usFn;
    HI_U16 usDataLen;
    HI_U16 usTimeOut;
    HI_U8 ucMaxTry;
    HI_U8 reserved[3];
    HI_VOID *pParam;
    HI_VOID (*MrsCcoRxRespProc)(HI_VOID *);
    HI_U32 (*MrsCcoRxTotalTimeOutProc)(HI_VOID *);
    HI_U32 (*MrsCcoRxEveryTimeOutProc)(HI_VOID *);
    HI_U8 pData[0];
}MRS_SRV_CCO_ITEM;

#define mrsActiveCcoQueue()     MRS_StartTimer(MRS_CCO_TIMER_MASTER_ACTIVE, 0, HI_SYS_TIMER_ONESHOT)

HI_VOID mrsCcoQueueInit(HI_VOID);
P_MRS_SRV_CCO_QUEUE mrsGetCcoQueue(HI_VOID);
HI_U8 mrsCcoGetQueueStatus(HI_VOID);
HI_VOID mrsCcoSetQueueStatus(HI_U8 ucStatus);
HI_VOID mrsCcoJoinQueue(MRS_SRV_CCO_ITEM *pstCcoItem);
HI_U32 mrsCcoToMasterMsgPro(HI_VOID);
HI_U32 mrsCcoQueueTimeOutPro(HI_VOID);
HI_U32 mrsCcoQueueEnd(P_MRS_SRV_CCO_QUEUE pstCcoQueue);
HI_VOID* mrsCcoDeQueue(MRS_SRV_QUEUE *pstQueue);
HI_BOOL mrsCcoQueueFindRepeatItem(HI_VOID *pNode, HI_VOID *pParam);

HI_BOOL mrsCcoQueueMatchItem(HI_VOID *pvItem, HI_U8 ucAfn, HI_U16 usFn);
HI_VOID mrsCcoQueueDeleteItem(HI_BOOL (*pfnCheckItem)(HI_VOID *), HI_VOID (*pfnFreeNode)(HI_VOID *));
HI_VOID mrsCcoQueueDeleteItemDefault(HI_BOOL (*pfnCheckItem)(HI_VOID *));
HI_U32 mrsCcoQueueGetLength(HI_VOID);


HI_END_HEADER
#endif //__MRS_SERVICE_H__

