//*****************************************************************************
//
//                  版权所有 (C), 1998-2009, 华为技术有限公司
//
//*****************************************************************************
//  文 件 名   : mrs_srv_queue.h
//  版 本 号   : V1.0 
//  作    者   : nkf62735
//  生成日期   : 2012-06-01
//  功能描述   : 通用队列
//               
//  函数列表   : NA
//  修改历史   : 
//  1.日    期 : 2012-06-01
//    作    者 : nkf62735
//    修改内容 : 创建文件 
//
//*****************************************************************************
#ifndef __MRS_SRV_QUEUE_H__
#define __MRS_SRV_QUEUE_H__

HI_START_HEADER

typedef struct _MRS_SRV_NODE
{
    struct _MRS_SRV_NODE * next;
}MRS_SRV_NODE;

typedef struct _MRS_SRV_QUEUE
{
    struct _MRS_SRV_NODE * front; 
    struct _MRS_SRV_NODE * rear; 
}MRS_SRV_QUEUE;

//队列的一些操作
//入队列
HI_VOID mrsSrvEnQueue(MRS_SRV_QUEUE *queue,MRS_SRV_NODE * node);
//清空队列，需要提供释放函数
HI_U32 mrsSrvEmptyQueue(MRS_SRV_QUEUE *queue,HI_VOID (*free_node)(HI_VOID *));
//初始化队列
HI_U32 mrsSrvInitQueue(MRS_SRV_QUEUE *queue);
//判断队列是不是空
HI_U32 mrsSrvQueueIsEmpty(MRS_SRV_QUEUE *queue);
//出队列
HI_VOID * mrsSrvDeQueue(MRS_SRV_QUEUE *queue);
//获取最前面的队列，不出队列
HI_VOID * mrsSrvQueueTop(MRS_SRV_QUEUE *queue);
//队列的个数
HI_U32 mrsSrvQueueLength(MRS_SRV_QUEUE *queue);
//队列元素替换
MRS_SRV_NODE * mrsSrvQueueReplace(MRS_SRV_QUEUE *,MRS_SRV_NODE *,MRS_SRV_NODE *);
//遍历函数visit
HI_U32 mrsSrvTraverseQueue(MRS_SRV_QUEUE *queue,HI_BOOL (*visit)(HI_VOID *,HI_VOID *),HI_VOID *);

HI_END_HEADER
#endif