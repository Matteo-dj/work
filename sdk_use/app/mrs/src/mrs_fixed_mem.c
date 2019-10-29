//*****************************************************************************
//
//                  ��Ȩ���� (C), 2001-2014, ��Ϊ�������޹�˾
//
//*****************************************************************************
//  �� �� ��   : mrs_fixed_mem.c
//  �� �� ��   : V1.0 
//  ��    ��   : cuiate/c00233580
//  ��������   : 2014-10-22
//  ��������   : �ڴ���������
//               ����ʵ��
//               
//  �����б�   : 
//  �޸���ʷ   : 
//  1.��    �� : 2014-10-22
//    ��    �� : cuiate/c00233580
//    �޸����� : �����ļ� 
//
//*****************************************************************************

#include "mrs_common.h"
#include "mrs_fw_tools.h"
#include "mrs_srv_list.h"
#include "mrs_fixed_mem.h"


#if defined(PRODUCT_CFG_PRODUCT_TYPE_CCO)
//*****************************************************************************
//*****************************************************************************

typedef struct
{
    HI_U32 ulIndex;
    HI_U32 ulvisited;
    MRS_FIXED_MEM_HEAD_PTR pstHead;
    HI_VOID *pstNode;
} MRS_FIXED_MEM_VISIT_PARAM;


//*****************************************************************************
//*****************************************************************************

HI_PRV HI_VOID mrsFixedFreeNode(HI_VOID *pstNode, HI_VOID *pstParam);

HI_PRV HI_BOOL mrsFixedClearNode(HI_VOID *pstNode, HI_VOID *pstParam);

HI_PRV HI_BOOL mrsFixedGetNode(HI_VOID *pstNode, HI_VOID *pstParam);

HI_PRV HI_U32 mrsFixedIncMalloc(MRS_FIXED_MEM_HEAD_PTR pstMemHead, HI_U8 ucDfxIndex);



//*****************************************************************************
// ��������: mrsFixedMalloc
// ��������: ���ճ�ʼ�ڵ����������ڴ棬�����������������ʵ���ڴ�ʹ������������
// ����˵��:
//   pstParam   : [IN]  �ڴ����Ĳ�����������ʼ�ڵ������ܽڵ����������ڵ�����
//                      �ڵ��С
//   ucDfxIndex : [IN]  ͳ������������(֧��0-6): 0->����������Ԥ��
// �� �� ֵ: 
//   ����ɹ�   : ���ط�����ڴ�ͷָ��ͷ
//   ����ʧ��   : HI_NULL
//
// ��    ��: cuiate/c00233580 [2014-10-30]
//*****************************************************************************
MRS_FIXED_MEM_HEAD_PTR mrsFixedMalloc(MRS_FIXED_MEM_PRAMS_STRU * pstParam, HI_U8 ucDfxIndex)
{
    MRS_FIXED_MEM_HEAD_PTR pstHead = HI_NULL;
    MRS_FIXED_MEM_ITEM_STRU *pstItem = HI_NULL;
    HI_U16 usBufLen = 0;
    HI_U8 *pucBuf = HI_NULL;

    if (HI_NULL == pstParam)
    {
        return HI_NULL;
    }
    
    pstHead = (MRS_FIXED_MEM_HEAD_PTR)mrsToolsMalloc(sizeof(MRS_FIXED_MEM_HEAD_STRU));
    
    if (HI_NULL == pstHead)
    {
        return HI_NULL;
    }

    (hi_void)memset_s(pstHead, sizeof(MRS_FIXED_MEM_HEAD_STRU), 0, sizeof(MRS_FIXED_MEM_HEAD_STRU));
    (hi_void)memcpy_s(&(pstHead->stParam), sizeof(pstHead->stParam), pstParam, sizeof(MRS_FIXED_MEM_PRAMS_STRU));
    (HI_VOID)mrsSrvInitList(&(pstHead->stMemList));

    usBufLen = sizeof(MRS_FIXED_MEM_ITEM_STRU) + (HI_U16)(pstHead->stParam.ulInitItemCnt * pstHead->stParam.ulItemSize);
    pucBuf = (HI_U8 *)mrsToolsMalloc(usBufLen);
    
    if (HI_NULL == pucBuf)
    {
        mrsDfxCcoFlashMemErrCnt(ucDfxIndex);
        mrsToolsFree(pstHead);
        return HI_NULL;
    }    
    mrsDfxCcoFlashMemMallocCnt(pstHead->stParam.ulInitItemCnt, ucDfxIndex);

    (hi_void)memset_s(pucBuf, usBufLen, 0, usBufLen);
    pstItem = (MRS_FIXED_MEM_ITEM_STRU *)pucBuf;
    pstItem->ulItemCount = pstHead->stParam.ulInitItemCnt;
    (HI_VOID)mrsSrvListInsTail(&(pstHead->stMemList), &(pstItem->stLink));

    pstHead->ulAllocItemCnt = pstHead->stParam.ulInitItemCnt;

    return pstHead;
}


//*****************************************************************************
// ��������: mrsFixedFree
// ��������: �ͷ��ڴ�
// ����˵��:
//   pstFixedHead [IN]  Ҫ�ͷŵ��ڴ�ָ��ͷ
//
// �� �� ֵ: ��
// ��    ��: cuiate/c00233580 [2014-10-30]
//*****************************************************************************
HI_VOID mrsFixedFree(MRS_FIXED_MEM_HEAD_PTR pstMemHead)
{
    if (HI_NULL == pstMemHead)
    {
        return;
    }

    mrsSrvListClear(&(pstMemHead->stMemList), mrsFixedFreeNode, HI_NULL);

    mrsToolsFree(pstMemHead);
}


//*****************************************************************************
// ��������: mrsFixedMemClear
// ��������: �ڴ���0����
// ����˵��:
//   pstFixedHead [IN]  Ҫ��0���ڴ�ָ��ͷ
//
// �� �� ֵ: ��
// ��    ��: cuiate/c00233580 [2014-10-30]
//*****************************************************************************
HI_VOID mrsFixedMemClear(MRS_FIXED_MEM_HEAD_PTR pstMemHead)
{
    if (HI_NULL == pstMemHead)
    {
        return;
    }

    (HI_VOID)mrsSrvTraverseList(&(pstMemHead->stMemList), mrsFixedClearNode, pstMemHead);
}


//*****************************************************************************
// ��������: mrsFixedMemGetItem
// ��������: �����������ؽڵ���ڴ��ַ
// ����˵��:
//   pstFixedHead [IN]  �ڴ�ָ��ͷ
//   ulIndex      [IN]  �ڵ�����
//
// �� �� ֵ: 
//   �ɹ�: ���ؽڵ���ڴ��ַ
//   ʧ��: HI_NULL
//
// ��    ��: cuiate/c00233580 [2014-10-30]
//*****************************************************************************
HI_VOID *mrsFixedMemGetItem(MRS_FIXED_MEM_HEAD_PTR pstMemHead, HI_U32 ulIndex)
{
    MRS_FIXED_MEM_VISIT_PARAM stParam;

    if ((HI_NULL == pstMemHead) || (ulIndex >= pstMemHead->stParam.ulMaxItemCnt))
    {
        return HI_NULL;
    }

    stParam.pstHead = pstMemHead;
    stParam.ulIndex = ulIndex;
    stParam.ulvisited = 0;
    stParam.pstNode = HI_NULL;

    (HI_VOID)mrsSrvTraverseList(&(pstMemHead->stMemList), mrsFixedGetNode, &stParam);

    return stParam.pstNode;
}


//*****************************************************************************
// ��������: mrsFixedMemSetItem
// ��������: �����������ýڵ�����
// ����˵��:
//   pstFixedHead [IN]  �ڴ�ָ��ͷ
//   ulIndex      [IN]  �ڵ�����
//   pstItem      [IN]  �ڵ�����
//   ucDfxIndex   [IN]  ͳ������������(֧��0-6): 0->����������Ԥ��

//
// �� �� ֵ: 
//   ���óɹ�: HI_ERR_SUCCESS
//   ����ʧ��: HI_ERR_FAILURE
//   ��������: HI_ERR_INVALID_PARAMETER
//   �ڴ治��: HI_ERR_NOT_ENOUGH_MEMORY
//
// ��    ��: cuiate/c00233580 [2014-10-30]
//*****************************************************************************
HI_U32 mrsFixedMemSetItem(MRS_FIXED_MEM_HEAD_PTR pstMemHead, HI_U32 ulIndex, HI_VOID *pstNode, HI_U8 ucDfxIndex)
{
    MRS_FIXED_MEM_VISIT_PARAM stParam;

    if ((HI_NULL == pstMemHead) || (HI_NULL == pstNode) || (ulIndex >= pstMemHead->stParam.ulMaxItemCnt))
    {
        return HI_ERR_INVALID_PARAMETER;
    }

    while (ulIndex >= pstMemHead->ulAllocItemCnt)
    {
        HI_U32 ulRet = HI_ERR_SUCCESS;

        ulRet = mrsFixedIncMalloc(pstMemHead, ucDfxIndex);
        if (HI_ERR_SUCCESS != ulRet)
        {
            return HI_ERR_NOT_ENOUGH_MEMORY;
        }
    }

    stParam.pstHead = pstMemHead;
    stParam.ulIndex = ulIndex;
    stParam.ulvisited = 0;
    stParam.pstNode = HI_NULL;

    (HI_VOID)mrsSrvTraverseList(&(pstMemHead->stMemList), mrsFixedGetNode, &stParam);

    if (stParam.pstNode)
    {
        (hi_void)memcpy_s(stParam.pstNode, pstMemHead->stParam.ulItemSize, pstNode, (HI_U16)pstMemHead->stParam.ulItemSize);
        return HI_ERR_SUCCESS;
    }

    return HI_ERR_FAILURE;
}


//*****************************************************************************
// ��������: mrsFixedFreeNode
// ��������: �ڴ��ͷŵĻص��������ͷ��ڴ������һ���ڵ���ڴ�
// ����˵��:
//   pstNode      [IN]  �ڵ��ڴ�ͷ��ַ
//   pstParam     [IN]  �û�����
//
// �� �� ֵ: ��
// ��    ��: cuiate/c00233580 [2014-10-30]
//*****************************************************************************
HI_VOID mrsFixedFreeNode(HI_VOID *pstNode, HI_VOID *pstParam)
{
    MRS_NOT_USED(pstParam);
    mrsToolsFree(pstNode);
}


//*****************************************************************************
// ��������: mrsFixedGetNode
// ��������: ��0�ڴ�Ļص���������0�ڴ������һ���ڵ�
// ����˵��:
//   pstNode      [IN]  �ڵ��ڴ�ͷ��ַ
//   pstParam     [IN]  �û�����
//
// �� �� ֵ: �̶�����HI_FALSE��ʹ����������������Լ�����һ���ڵ�
// ��    ��: cuiate/c00233580 [2014-10-30]
//*****************************************************************************
HI_BOOL mrsFixedClearNode(HI_VOID *pstNode, HI_VOID *pstParam)
{
    MRS_FIXED_MEM_ITEM_STRU *pstItem = (MRS_FIXED_MEM_ITEM_STRU *)pstNode;
    MRS_FIXED_MEM_HEAD_PTR pstHead = (MRS_FIXED_MEM_HEAD_PTR)pstParam;

    if (pstItem && pstHead)
    {
        (hi_void)memset_s(pstItem->pucMemBuf, (HI_U16)(pstItem->ulItemCount * pstHead->stParam.ulItemSize),
            0,(HI_U16)(pstItem->ulItemCount * pstHead->stParam.ulItemSize));
    }

    return HI_FALSE;
}


//*****************************************************************************
// ��������: mrsFixedClearNode
// ��������: ��ȡ�ڵ��ڴ�Ļص�������������������ָ���ַ
// ����˵��:
//   pstNode      [IN]  �ڵ��ڴ�ͷ��ַ
//   pstParam     [IN]  �û�����
//
// �� �� ֵ: 
//   ���ҳɹ�: HI_TRUE
//   ����ʧ��: HI_FALSE
//
// ��    ��: cuiate/c00233580 [2014-10-30]
//*****************************************************************************
HI_BOOL mrsFixedGetNode(HI_VOID *pstNode, HI_VOID *pstExtraParam)
{
    MRS_FIXED_MEM_ITEM_STRU *pstItem = (MRS_FIXED_MEM_ITEM_STRU *)pstNode;
    MRS_FIXED_MEM_VISIT_PARAM *pstParam = (MRS_FIXED_MEM_VISIT_PARAM *)pstExtraParam;

    if (pstParam->ulIndex < pstItem->ulItemCount + pstParam->ulvisited)
    {
        pstParam->pstNode = pstItem->pucMemBuf 
            + (pstParam->ulIndex - pstParam->ulvisited) * pstParam->pstHead->stParam.ulItemSize;
        return HI_TRUE;
    }

    pstParam->ulvisited += pstItem->ulItemCount;

    return HI_FALSE;
}


//*****************************************************************************
// ��������: mrsFixedIncMalloc
// ��������: ���������ڴ�
// ����˵��:
//   pstHead      [IN]  ���������ͷָ��
//   ucDfxIndex   [IN]  ͳ������������(֧��0-6): 0->����������Ԥ��
//
// �� �� ֵ: 
//   �ɹ�: HI_ERR_SUCCESS
//   ʧ��: HI_ERR_NOT_ENOUGH_MEMORY
//
// ��    ��: cuiate/c00233580 [2014-10-30]
//*****************************************************************************
HI_U32 mrsFixedIncMalloc(MRS_FIXED_MEM_HEAD_PTR pstHead, HI_U8 ucDfxIndex)
{
    MRS_FIXED_MEM_ITEM_STRU * pstItem = HI_NULL;
    
    HI_U32 ulIncCnt = MRS_MIN(pstHead->stParam.ulIncItemCnt, pstHead->stParam.ulMaxItemCnt - pstHead->ulAllocItemCnt);
    HI_U8 *pucBuf = (HI_U8 *)mrsToolsMalloc(sizeof(MRS_FIXED_MEM_ITEM_STRU) + ulIncCnt * pstHead->stParam.ulItemSize);
   
    if (HI_NULL == pucBuf)
    {
        mrsDfxCcoFlashMemErrCnt(ucDfxIndex);
        return HI_ERR_NOT_ENOUGH_MEMORY;
    }
    mrsDfxCcoFlashMemMallocCnt(ulIncCnt, ucDfxIndex);

    (hi_void)memset_s(pucBuf, (HI_U16)(sizeof(MRS_FIXED_MEM_ITEM_STRU) + ulIncCnt * pstHead->stParam.ulItemSize),
        0,(HI_U16)(sizeof(MRS_FIXED_MEM_ITEM_STRU) + ulIncCnt * pstHead->stParam.ulItemSize));
    pstItem = (MRS_FIXED_MEM_ITEM_STRU *)pucBuf;
    pstItem->ulItemCount = ulIncCnt;
    (HI_VOID)mrsSrvListInsTail(&(pstHead->stMemList), &(pstItem->stLink));
    pstHead->ulAllocItemCnt += ulIncCnt;

    return HI_ERR_SUCCESS;
}


//*****************************************************************************
//*****************************************************************************

#endif //defined(PRODUCT_CFG_PRODUCT_TYPE_CCO)


