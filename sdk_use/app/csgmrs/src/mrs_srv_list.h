//*****************************************************************************
//
//                  ��Ȩ���� (C), 1998-2009, ��Ϊ�������޹�˾
//
//*****************************************************************************
//  �� �� ��   : mrs_srv_list.h
//  �� �� ��   : V1.0 
//  ��    ��   : nkf62735
//  ��������   : 2012-06-01
//  ��������   : ͨ������
//               
//  �����б�   : NA
//  �޸���ʷ   : 
//  1.��    �� : 2012-08-27
//    ��    �� : nkf62735
//    �޸����� : �����ļ� 
//
//*****************************************************************************
#ifndef __MRS_SRV_LIST__
#define __MRS_SRV_LIST__

HI_CPP_START

//�����ڱ��ڵ������
typedef struct _MRS_SRV_LINK
{
    struct _MRS_SRV_LINK *prev;
    struct _MRS_SRV_LINK *next;
}MRS_SRV_LIST,MRS_SRV_LINK;

//��������(���Է���linkָ��δ�ڽṹ�嶥�˵����ݽṹ)
#define mrs_list_entry(p, s, m) (s *)((HI_U8 *)(p)-((HI_U32)&(((s *)0)->m)))

//��ʼ������
HI_U32 mrsSrvInitList(MRS_SRV_LIST *list);
//����1,β�巨
HI_U32 mrsSrvListInsTail(MRS_SRV_LIST * list,MRS_SRV_LINK *link);
//������
HI_U32 mrsSrvListLength(MRS_SRV_LIST * list);

//����2,ͷ�巨
HI_U32 mrsSrvListInsHead(MRS_SRV_LIST * list,MRS_SRV_LINK *link);

//�������� ����visit
HI_U32 mrsSrvTraverseList(MRS_SRV_LIST * list, HI_BOOL (*visit)(HI_VOID *,HI_VOID *), HI_VOID * extra);

//ɾ������
HI_U32 mrsSrvListRemove(MRS_SRV_LIST * list,MRS_SRV_LINK *link);
//�������
HI_U32 mrsSrvListClear(MRS_SRV_LIST * list,HI_VOID (*)(HI_VOID *,HI_VOID *),HI_VOID *);

HI_CPP_END
#endif


