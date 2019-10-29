//*****************************************************************************
//
//                  ��Ȩ���� (C), 1998-2009, ��Ϊ�������޹�˾
//
//*****************************************************************************
//  �� �� ��   : mrs_srv_queue.h
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

//���е�һЩ����
//�����
HI_VOID mrsSrvEnQueue(MRS_SRV_QUEUE *queue,MRS_SRV_NODE * node);
//��ն��У���Ҫ�ṩ�ͷź���
HI_U32 mrsSrvEmptyQueue(MRS_SRV_QUEUE *queue,HI_VOID (*free_node)(HI_VOID *));
//��ʼ������
HI_U32 mrsSrvInitQueue(MRS_SRV_QUEUE *queue);
//�ж϶����ǲ��ǿ�
HI_U32 mrsSrvQueueIsEmpty(MRS_SRV_QUEUE *queue);
//������
HI_VOID * mrsSrvDeQueue(MRS_SRV_QUEUE *queue);
//��ȡ��ǰ��Ķ��У���������
HI_VOID * mrsSrvQueueTop(MRS_SRV_QUEUE *queue);
//���еĸ���
HI_U32 mrsSrvQueueLength(MRS_SRV_QUEUE *queue);
//����Ԫ���滻
MRS_SRV_NODE * mrsSrvQueueReplace(MRS_SRV_QUEUE *,MRS_SRV_NODE *,MRS_SRV_NODE *);
//��������visit
HI_U32 mrsSrvTraverseQueue(MRS_SRV_QUEUE *queue,HI_BOOL (*visit)(HI_VOID *,HI_VOID *),HI_VOID *);

HI_END_HEADER
#endif