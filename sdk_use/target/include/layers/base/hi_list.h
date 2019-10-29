//****************************************************************************
//
//                  版权所有 (C), 2001-2011, 华为技术有限公司
//
//****************************************************************************
//  文 件 名   : hi_list.h
//  版 本 号   : V1.0
//  生成日期   : 2017年07月3日
//  功能描述   : 双向链表
//  函数列表   :
//****************************************************************************
#ifndef __HI_LIST_H__
#define __HI_LIST_H__
#include <hi_types.h>

#if !defined (MAC_V200_LLT)
#include <los_list.h>
#endif
#if defined (MAC_V200_LLT)
#include <slist_v2.h>
#endif
//****************************************************************************
HI_START_HEADER

//****************************************************************************
//*****************************************************************************
// 备注：本文件对链表接口重新命名，实现依然为LTOS中实现。
//*****************************************************************************
#if !defined (MAC_V200_LLT)
typedef LOS_DL_LIST HI_LIST;

/*
功能描述：
	初始化头节点，注意此节点仅用于管理，不是用户输入的数据节点
*/
#define HI_ListInit(list)  LOS_ListInit(list)
/*
功能描述：
	HI_ListTailInsert为向最后一个节点之后插入新节点
*/
#define HI_ListTailInsert(list,node) LOS_ListTailInsert(list,node)
/*
功能描述：
	HI_ListHeadInsert为向第一个节点之前插入新节点
*/
#define HI_ListHeadInsert(list,node) LOS_ListHeadInsert(list,node)
/*
功能描述：
	从链表中删除某个节点
*/
INLINE VOID HI_ListDelete(LOS_DL_LIST *pstNode)
{
    if ((pstNode->pstPrev == NULL)||(pstNode->pstNext == NULL)) {\
        return;
    }
    pstNode->pstNext->pstPrev = pstNode->pstPrev;
    pstNode->pstPrev->pstNext = pstNode->pstNext;
    pstNode->pstNext = (LOS_DL_LIST *)NULL;
    pstNode->pstPrev = (LOS_DL_LIST *)NULL;
}

/*
功能描述：
	判断链表是否为空
*/
#define HI_IsListEmpty(list)  LOS_ListEmpty(list)
/*
功能描述：
	去初始化链表，管理节点清空，其他成员节点首尾相连仍然是一个双向链表。
*/
#define HI_ListDelInit(list)  LOS_ListDelInit(list)

/*获取第一个节点指针*/
#define  HI_LIST_FIRST(list)  LOS_DL_LIST_FIRST(list)
/*获取最后一个节点指针*/
#define  HI_LIST_LAST(pstObject)  LOS_DL_LIST_LAST(pstObject)
/*
	功能描述：
	获取到一个包含双链表的结构体的指针地址。
	举例：
	typedef struct
	{
		HI_U32 taskId
		...
		HI_LIST_NODE stTimerList
	}TASK_ST
	当想通过该TimerList找到下一个timer的任务的PCB指针时可以使用下面用法：
	HI_LIST_ENTRY(pstTaskCB->stTimerList.pstNext, LOS_TASK_CB, stTimerList)
*/
#define  HI_LIST_ENTRY(item, type, member)  LOS_DL_LIST_ENTRY(item, type, member)
/*
	功能描述：
	获取成员在结构体中的偏移量。
*/
#define  HI_OFFSET_OF_MEMBER(type, member)   LOS_OFF_SET_OF(type, member)
/*
	功能列表：
		通过LIST遍历每一个成员节点所在结构体的指针入口地址。
*/
#define  HI_LIST_FOR_EACH_ENTRY(item, list, type, member)  LOS_DL_LIST_FOR_EACH_ENTRY(item, list, type, member)
/*
	功能列表：
		通过LIST遍历每一个成员节点所在结构体的指针入口地址,保存下一个节点的指针，避免当前节点处理完成后删除的场景。
*/
#define  HI_LIST_FOR_EACH_ENTRY_SAFE(list, item, next, type, member)  LOS_DL_LIST_FOR_EACH_ENTRY_SAFE(item, next, list, type, member)

#define  HI_LIST_FOR_EACH_ENTRY_CONTINUE_SAFE(pitem, list, item, next, type, member)  \
                    for (item = LOS_DL_LIST_ENTRY((pitem)->pstNext, type, member), \
                            next = LOS_DL_LIST_ENTRY(item->member.pstNext, type, member); \
                             &((item)->member) != (list); \
                            item = next, next = LOS_DL_LIST_ENTRY(next->member.pstNext, type, member))

/*双向链表操作简单实现*/
#define  HI_LIST_HEAD(list)  LOS_DL_LIST_HEAD(list)
#define  HI_LIST_FOR_EACH(item, list) LOS_DL_LIST_FOR_EACH(item, list)
#define  HI_LIST_FOR_EACH_SAFE(item, next, list) LOS_DL_LIST_FOR_EACH_SAFE(item, next, list)

#endif

HI_END_HEADER
#endif //__HI_STDLIB_H__

