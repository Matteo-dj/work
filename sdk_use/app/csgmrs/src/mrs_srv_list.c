//*****************************************************************************
//
//                  版权所有 (C), 1998-2009, 华为技术有限公司
//
//*****************************************************************************
//  文 件 名   : mrs_srv_list.c
//  版 本 号   : V1.0 
//  作    者   : nkf62735
//  生成日期   : 2012-08-27
//  功能描述   : 通用链表
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
#include "mrs_srv_list.h"

//初始化队列
HI_U32 mrsSrvInitList(MRS_SRV_LIST *list)
{
    list->next = list;
    list->prev = list;

    return HI_ERR_SUCCESS;
}

//插入2,头插法
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

//删除操作
HI_U32 mrsSrvListRemove(MRS_SRV_LIST * list,MRS_SRV_LINK *link)
{
    MRS_SRV_LINK * it;

    //查找
    it = list->next;

    for (it = list->next; it != list; it = it->next) 
    {
        if (link == it) 
        {
            break;
        }
    }

    //没有找到该结点
    if (it == list) 
    {
        return HI_ERR_NOT_FOUND;
    }

    link->next->prev = link->prev;
    link->prev->next = link->next;

    return HI_ERR_SUCCESS;
}

//链表长度
HI_U32 mrsSrvListLength(MRS_SRV_LIST * list)
{
    MRS_SRV_LINK * it;
    HI_U32 len = 0;
    
    //查找
    for (it = list->next; it != list; it = it->next) 
    {
        len++;
    }

    return len;
}

//遍历链表 函数visit
HI_U32 mrsSrvTraverseList(MRS_SRV_LIST * list, HI_BOOL (*visit)(HI_VOID *,HI_VOID *), HI_VOID * extra)
{
    MRS_SRV_LINK * it = HI_NULL;

    for (it = list->next; it != list; it = it->next) 
    {
        if (visit != HI_NULL) 
        {
            // 如果返回HI_TRUE，跳出循环
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

