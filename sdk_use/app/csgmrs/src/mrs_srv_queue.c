//*****************************************************************************
//
//                  ��Ȩ���� (C), 1998-2009, ��Ϊ�������޹�˾
//
//*****************************************************************************
//  �� �� ��   : mrs_srv_queue.c
//  �� �� ��   : V1.0 
//  ��    ��   : nkf62735
//  ��������   : 2012-06-01
//  ��������   : ͨ�ö���
//               
//  �����б�   : NA
//  �޸���ʷ   : 
//  1.��    �� : 2012-06-01
//    ��    �� : nkf62735
//    �޸����� : �����ļ� 
//
//*****************************************************************************
#include "mrs_common.h"
#include "mrs_srv_common.h"
#include "mrs_srv_queue.h"

//��ʼ������
HI_U32 mrsSrvInitQueue(MRS_SRV_QUEUE *queue)
{
    queue->front = HI_NULL;
    queue->rear = HI_NULL;
    return HI_ERR_SUCCESS;
}

//�ж϶����ǲ��ǿ�
HI_U32 mrsSrvQueueIsEmpty(MRS_SRV_QUEUE *queue)
{
    return (mrsSrvQueueTop(queue) == HI_NULL);
}

//��ȡ��ǰ��Ķ��У���������
HI_VOID * mrsSrvQueueTop(MRS_SRV_QUEUE *queue)
{
    return queue->front;
}

//�����
HI_VOID mrsSrvEnQueue(MRS_SRV_QUEUE *queue,MRS_SRV_NODE *node)
{
    node->next = HI_NULL;
    //�ն���
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

//������
HI_VOID * mrsSrvDeQueue(MRS_SRV_QUEUE *queue)
{    
    MRS_SRV_NODE * node = HI_NULL;

    //�����ǿն���
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

//��ն��У���Ҫ�ṩ�ͷź���
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

//���еĳ���
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

//�滻����Ԫ��
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

        //����Ƕ���
        if (nd1 == queue->front) 
        {
            queue->front = nd2;
        }
        //����Ƕ�β
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

//��������visit
HI_U32 mrsSrvTraverseQueue(MRS_SRV_QUEUE *queue,HI_BOOL (*visit)(HI_VOID *,HI_VOID *),HI_VOID * extra)
{
    MRS_SRV_NODE * it;

    for (it = queue->front;it != HI_NULL;it = it->next)
    {
        if (visit != HI_NULL) 
        {
            //�������HI_TRUE������ѭ��
            if(visit((HI_VOID *)it,extra) == HI_TRUE)
            {
               break;
            }
        }
    }
    return HI_ERR_SUCCESS;
}
