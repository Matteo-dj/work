//*****************************************************************************
//
//                  版权所有 (C), 1998-2009, 华为技术有限公司
//
//*****************************************************************************
//  文 件 名   : mrs_srv_list.h
//  版 本 号   : V1.0 
//  作    者   : nkf62735
//  生成日期   : 2012-06-01
//  功能描述   : 通用链表
//               
//  函数列表   : NA
//  修改历史   : 
//  1.日    期 : 2012-08-27
//    作    者 : nkf62735
//    修改内容 : 创建文件 
//
//*****************************************************************************
#ifndef __MRS_SRV_LIST__
#define __MRS_SRV_LIST__

HI_CPP_START

//带有哨兵节点的链表
typedef struct _MRS_SRV_LINK
{
    struct _MRS_SRV_LINK *prev;
    struct _MRS_SRV_LINK *next;
}MRS_SRV_LIST,MRS_SRV_LINK;

//访问数据(用以访问link指针未在结构体顶端的数据结构)
#define mrs_list_entry(p, s, m) (s *)((HI_U8 *)(p)-((HI_U32)&(((s *)0)->m)))

//初始化链表
HI_U32 mrsSrvInitList(MRS_SRV_LIST *list);
//插入1,尾插法
HI_U32 mrsSrvListInsTail(MRS_SRV_LIST * list,MRS_SRV_LINK *link);
//链表长度
HI_U32 mrsSrvListLength(MRS_SRV_LIST * list);

//插入2,头插法
HI_U32 mrsSrvListInsHead(MRS_SRV_LIST * list,MRS_SRV_LINK *link);

//遍历链表 函数visit
HI_U32 mrsSrvTraverseList(MRS_SRV_LIST * list, HI_BOOL (*visit)(HI_VOID *,HI_VOID *), HI_VOID * extra);

//删除操作
HI_U32 mrsSrvListRemove(MRS_SRV_LIST * list,MRS_SRV_LINK *link);
//清空链表
HI_U32 mrsSrvListClear(MRS_SRV_LIST * list,HI_VOID (*)(HI_VOID *,HI_VOID *),HI_VOID *);

//查询链表是否为空
HI_BOOL mrsSrvIsListEmpty(MRS_SRV_LIST * list);
HI_U32 mrsSrvListRemoveHeadNode(MRS_SRV_LIST * list);
HI_CPP_END
#endif


