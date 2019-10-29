//*****************************************************************************
//
//                  版权所有 (C), 1998-2009, 华为技术有限公司
//
//*****************************************************************************
//  文 件 名   : mrs_srv_queue.c
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
#include "mrs_common.h"
#include "mrs_srv_common.h"
#include "mrs_srv_queue.h"

//初始化队列
HI_U32 mrsSrvInitQueue(MRS_SRV_QUEUE *queue)
{
    queue->front = HI_NULL;
    queue->rear = HI_NULL;
    return HI_ERR_SUCCESS;
}

//判断队列是不是空
HI_U32 mrsSrvQueueIsEmpty(MRS_SRV_QUEUE *queue)
{
    return (mrsSrvQueueTop(queue) == HI_NULL);
}

//获取最前面的队列，不出队列
HI_VOID * mrsSrvQueueTop(MRS_SRV_QUEUE *queue)
{
    return queue->front;
}

//入队列
HI_VOID mrsSrvEnQueue(MRS_SRV_QUEUE *queue,MRS_SRV_NODE *node)
{
    node->next = HI_NULL;
    //空队列
    if (mrsSrvQueueIsEmpty(queue)) 
    {
        queue->front = node;
        queue->rear = node;
    }
    else 
    {
        queue->rear->next = node;
        queue->rear = node;
    }
}

//出队列
HI_VOID * mrsSrvDeQueue(MRS_SRV_QUEUE *queue)
{    
    MRS_SRV_NODE * node = HI_NULL;

    //不能是空队列
    if (!mrsSrvQueueIsEmpty(queue))
    {
        node = queue->front;

        if (queue->front == queue->rear) 
        {
            queue->rear = HI_NULL;
            queue->front = HI_NULL;
        } 
        else 
        {
            queue->front = queue->front->next;
        }
    }
    return node;
}

//清空队列，需要提供释放函数
HI_U32 mrsSrvEmptyQueue(MRS_SRV_QUEUE *queue,HI_VOID (*free_node)(HI_VOID *))
{
    while(!mrsSrvQueueIsEmpty(queue))
    {
        MRS_SRV_NODE * nd = HI_NULL;
        nd = mrsSrvDeQueue(queue);

        if (free_node != HI_NULL)
        {
            free_node((HI_VOID *)nd);
        }
    }
    return HI_ERR_SUCCESS;
}

//队列的长度
HI_U32 mrsSrvQueueLength(MRS_SRV_QUEUE *queue)
{
    MRS_SRV_NODE * it;
    HI_U32 num = 0;
    for (it = queue->front;it != HI_NULL;it = it->next)
    {
        num++;
    }
    return num;
}

//替换队列元素
MRS_SRV_NODE * mrsSrvQueueReplace(MRS_SRV_QUEUE * queue,MRS_SRV_NODE * nd1,MRS_SRV_NODE *nd2)
{
    MRS_SRV_NODE * it = HI_NULL;
    MRS_SRV_NODE temp = {0};
    
    if (queue && nd1 && nd2) 
    {
        temp.next = queue->front;

        for (it = &temp;it != HI_NULL;it = it->next)
        {
            if (it->next == nd1) 
            {
                it->next = nd2;
                nd2->next = nd1->next;
                break;
            }
        }

        //如果是对首
        if (nd1 == queue->front) 
        {
            queue->front = nd2;
        }
        //如果是对尾
        if (nd1 == queue->rear) 
        {
            queue->rear = nd2;
        }        
    }

    if(it == HI_NULL)
    {
        return HI_NULL;
    }
    
    return nd1;
}

//遍历函数visit
HI_U32 mrsSrvTraverseQueue(MRS_SRV_QUEUE *queue,HI_BOOL (*visit)(HI_VOID *,HI_VOID *),HI_VOID * extra)
{
    MRS_SRV_NODE * it;

    for (it = queue->front;it != HI_NULL;it = it->next)
    {
        if (visit != HI_NULL) 
        {
            //如果返回HI_TRUE，跳出循环
            if(visit((HI_VOID *)it,extra) == HI_TRUE)
            {
               break;
            }
        }
    }
    return HI_ERR_SUCCESS;
}
