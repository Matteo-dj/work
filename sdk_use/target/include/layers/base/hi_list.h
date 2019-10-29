//****************************************************************************
//
//                  ��Ȩ���� (C), 2001-2011, ��Ϊ�������޹�˾
//
//****************************************************************************
//  �� �� ��   : hi_list.h
//  �� �� ��   : V1.0
//  ��������   : 2017��07��3��
//  ��������   : ˫������
//  �����б�   :
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
// ��ע�����ļ�������ӿ�����������ʵ����ȻΪLTOS��ʵ�֡�
//*****************************************************************************
#if !defined (MAC_V200_LLT)
typedef LOS_DL_LIST HI_LIST;

/*
����������
	��ʼ��ͷ�ڵ㣬ע��˽ڵ�����ڹ��������û���������ݽڵ�
*/
#define HI_ListInit(list)  LOS_ListInit(list)
/*
����������
	HI_ListTailInsertΪ�����һ���ڵ�֮������½ڵ�
*/
#define HI_ListTailInsert(list,node) LOS_ListTailInsert(list,node)
/*
����������
	HI_ListHeadInsertΪ���һ���ڵ�֮ǰ�����½ڵ�
*/
#define HI_ListHeadInsert(list,node) LOS_ListHeadInsert(list,node)
/*
����������
	��������ɾ��ĳ���ڵ�
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
����������
	�ж������Ƿ�Ϊ��
*/
#define HI_IsListEmpty(list)  LOS_ListEmpty(list)
/*
����������
	ȥ��ʼ����������ڵ���գ�������Ա�ڵ���β������Ȼ��һ��˫������
*/
#define HI_ListDelInit(list)  LOS_ListDelInit(list)

/*��ȡ��һ���ڵ�ָ��*/
#define  HI_LIST_FIRST(list)  LOS_DL_LIST_FIRST(list)
/*��ȡ���һ���ڵ�ָ��*/
#define  HI_LIST_LAST(pstObject)  LOS_DL_LIST_LAST(pstObject)
/*
	����������
	��ȡ��һ������˫����Ľṹ���ָ���ַ��
	������
	typedef struct
	{
		HI_U32 taskId
		...
		HI_LIST_NODE stTimerList
	}TASK_ST
	����ͨ����TimerList�ҵ���һ��timer�������PCBָ��ʱ����ʹ�������÷���
	HI_LIST_ENTRY(pstTaskCB->stTimerList.pstNext, LOS_TASK_CB, stTimerList)
*/
#define  HI_LIST_ENTRY(item, type, member)  LOS_DL_LIST_ENTRY(item, type, member)
/*
	����������
	��ȡ��Ա�ڽṹ���е�ƫ������
*/
#define  HI_OFFSET_OF_MEMBER(type, member)   LOS_OFF_SET_OF(type, member)
/*
	�����б�
		ͨ��LIST����ÿһ����Ա�ڵ����ڽṹ���ָ����ڵ�ַ��
*/
#define  HI_LIST_FOR_EACH_ENTRY(item, list, type, member)  LOS_DL_LIST_FOR_EACH_ENTRY(item, list, type, member)
/*
	�����б�
		ͨ��LIST����ÿһ����Ա�ڵ����ڽṹ���ָ����ڵ�ַ,������һ���ڵ��ָ�룬���⵱ǰ�ڵ㴦����ɺ�ɾ���ĳ�����
*/
#define  HI_LIST_FOR_EACH_ENTRY_SAFE(list, item, next, type, member)  LOS_DL_LIST_FOR_EACH_ENTRY_SAFE(item, next, list, type, member)

#define  HI_LIST_FOR_EACH_ENTRY_CONTINUE_SAFE(pitem, list, item, next, type, member)  \
                    for (item = LOS_DL_LIST_ENTRY((pitem)->pstNext, type, member), \
                            next = LOS_DL_LIST_ENTRY(item->member.pstNext, type, member); \
                             &((item)->member) != (list); \
                            item = next, next = LOS_DL_LIST_ENTRY(next->member.pstNext, type, member))

/*˫�����������ʵ��*/
#define  HI_LIST_HEAD(list)  LOS_DL_LIST_HEAD(list)
#define  HI_LIST_FOR_EACH(item, list) LOS_DL_LIST_FOR_EACH(item, list)
#define  HI_LIST_FOR_EACH_SAFE(item, next, list) LOS_DL_LIST_FOR_EACH_SAFE(item, next, list)

#endif

HI_END_HEADER
#endif //__HI_STDLIB_H__

