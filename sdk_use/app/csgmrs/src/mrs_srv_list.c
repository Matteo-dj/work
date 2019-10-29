//*****************************************************************************
//
//                  ��Ȩ���� (C), 1998-2009, ��Ϊ�������޹�˾
//
//*****************************************************************************
//  �� �� ��   : mrs_srv_list.c
//  �� �� ��   : V1.0 
//  ��    ��   : nkf62735
//  ��������   : 2012-08-27
//  ��������   : ͨ������
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
#include "mrs_srv_list.h"

//��ʼ������
HI_U32 mrsSrvInitList(MRS_SRV_LIST *list)
{
    list->next = list;
    list->prev = list;

    return HI_ERR_SUCCESS;
}

//����2,ͷ�巨
HI_U32 mrsSrvListInsHead(MRS_SRV_LIST * list,MRS_SRV_LINK *link)
{
    link->next = list->next;
    link->prev = list;

    link->next->prev = link;
    link->prev->next = link;

    return HI_ERR_SUCCESS;
}

HI_U32 mrsSrvListInsTail(MRS_SRV_LIST * list,MRS_SRV_LINK *link)
{
    link->next = list;
    link->prev = list->prev;

    link->next->prev = link;
    link->prev->next = link;

    return HI_ERR_SUCCESS;
}

//ɾ������
HI_U32 mrsSrvListRemove(MRS_SRV_LIST * list,MRS_SRV_LINK *link)
{
    MRS_SRV_LINK * it;

    //����
    it = list->next;

    for (it = list->next; it != list; it = it->next) 
    {
        if (link == it) 
        {
            break;
        }
    }

    //û���ҵ��ý��
    if (it == list) 
    {
        return HI_ERR_NOT_FOUND;
    }

    link->next->prev = link->prev;
    link->prev->next = link->next;

    return HI_ERR_SUCCESS;
}

//������
HI_U32 mrsSrvListLength(MRS_SRV_LIST * list)
{
    MRS_SRV_LINK * it;
    HI_U32 len = 0;
    
    //����
    for (it = list->next; it != list; it = it->next) 
    {
        len++;
    }

    return len;
}

//�������� ����visit
HI_U32 mrsSrvTraverseList(MRS_SRV_LIST * list, HI_BOOL (*visit)(HI_VOID *,HI_VOID *), HI_VOID * extra)
{
    MRS_SRV_LINK * it = HI_NULL;

    for (it = list->next; it != list; it = it->next) 
    {
        if (visit != HI_NULL) 
        {
            // �������HI_TRUE������ѭ��
            if(visit((HI_VOID *)it,extra) == HI_TRUE)
            {
               break;
            }
        }
    }
    
    return HI_ERR_SUCCESS;
}

HI_U32 mrsSrvListClear(MRS_SRV_LIST * list,HI_VOID (*free_node)(HI_VOID *,HI_VOID *),HI_VOID * cb)
{
    MRS_SRV_LINK *it, *next;
    for (it = list->next; it != list;) 
    {
        next = it->next;
        if (free_node != HI_NULL) 
        {
            free_node((HI_VOID *)it,cb);
        }
        it = next;
    }

    list->next = list;
    list->prev = list;

    return HI_ERR_SUCCESS;
}

