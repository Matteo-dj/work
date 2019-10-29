//*****************************************************************************
//
//                  版权所有 (C), 2001-2014, 华为技术有限公司
//
//*****************************************************************************
//  文 件 名   : mrs_fixed_mem.c
//  版 本 号   : V1.0 
//  作    者   : cuiate/c00233580
//  生成日期   : 2014-10-22
//  功能描述   : 内存增量管理
//               函数实现
//               
//  函数列表   : 
//  修改历史   : 
//  1.日    期 : 2014-10-22
//    作    者 : cuiate/c00233580
//    修改内容 : 创建文件 
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
// 函数名称: mrsFixedMalloc
// 功能描述: 按照初始节点数量分配内存，后续操作函数会根据实际内存使用逐步增量分配
// 参数说明:
//   pstParam   : [IN]  内存分配的参数，包括初始节点数、总节点数、增量节点数和
//                      节点大小
//   ucDfxIndex : [IN]  统计量分配索引(支持0-6): 0->表档案，其他预留
// 返 回 值: 
//   分配成功   : 返回分配的内存头指针头
//   分配失败   : HI_NULL
//
// 作    者: cuiate/c00233580 [2014-10-30]
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
// 函数名称: mrsFixedFree
// 功能描述: 释放内存
// 参数说明:
//   pstFixedHead [IN]  要释放的内存指针头
//
// 返 回 值: 无
// 作    者: cuiate/c00233580 [2014-10-30]
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
// 函数名称: mrsFixedMemClear
// 功能描述: 内存清0操作
// 参数说明:
//   pstFixedHead [IN]  要清0的内存指针头
//
// 返 回 值: 无
// 作    者: cuiate/c00233580 [2014-10-30]
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
// 函数名称: mrsFixedMemGetItem
// 功能描述: 根据索引返回节点的内存地址
// 参数说明:
//   pstFixedHead [IN]  内存指针头
//   ulIndex      [IN]  节点索引
//
// 返 回 值: 
//   成功: 返回节点的内存地址
//   失败: HI_NULL
//
// 作    者: cuiate/c00233580 [2014-10-30]
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
// 函数名称: mrsFixedMemSetItem
// 功能描述: 根据索引设置节点内容
// 参数说明:
//   pstFixedHead [IN]  内存指针头
//   ulIndex      [IN]  节点索引
//   pstItem      [IN]  节点内容
//   ucDfxIndex   [IN]  统计量分配索引(支持0-6): 0->表档案，其他预留

//
// 返 回 值: 
//   设置成功: HI_ERR_SUCCESS
//   设置失败: HI_ERR_FAILURE
//   参数错误: HI_ERR_INVALID_PARAMETER
//   内存不足: HI_ERR_NOT_ENOUGH_MEMORY
//
// 作    者: cuiate/c00233580 [2014-10-30]
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
// 函数名称: mrsFixedFreeNode
// 功能描述: 内存释放的回调函数，释放内存链表的一个节点的内存
// 参数说明:
//   pstNode      [IN]  节点内存头地址
//   pstParam     [IN]  用户参数
//
// 返 回 值: 无
// 作    者: cuiate/c00233580 [2014-10-30]
//*****************************************************************************
HI_VOID mrsFixedFreeNode(HI_VOID *pstNode, HI_VOID *pstParam)
{
    MRS_NOT_USED(pstParam);
    mrsToolsFree(pstNode);
}


//*****************************************************************************
// 函数名称: mrsFixedGetNode
// 功能描述: 清0内存的回调函数，清0内存链表的一个节点
// 参数说明:
//   pstNode      [IN]  节点内存头地址
//   pstParam     [IN]  用户参数
//
// 返 回 值: 固定返回HI_FALSE，使得链表遍历函数可以继续下一个节点
// 作    者: cuiate/c00233580 [2014-10-30]
//*****************************************************************************
HI_BOOL mrsFixedClearNode(HI_VOID *pstNode, HI_VOID *pstParam)
{
    MRS_FIXED_MEM_ITEM_STRU *pstItem = (MRS_FIXED_MEM_ITEM_STRU *)pstNode;
    MRS_FIXED_MEM_HEAD_PTR pstHead = (MRS_FIXED_MEM_HEAD_PTR)pstParam;

    if (pstItem && pstHead)
    {
        (hi_void)memset_s(pstItem->pucMemBuf, (HI_U16)(pstItem->ulItemCount * pstHead->stParam.ulItemSize), 0,(HI_U16)(pstItem->ulItemCount * pstHead->stParam.ulItemSize));
    }

    return HI_FALSE;
}


//*****************************************************************************
// 函数名称: mrsFixedClearNode
// 功能描述: 获取节点内存的回调函数，根据索引查找指针地址
// 参数说明:
//   pstNode      [IN]  节点内存头地址
//   pstParam     [IN]  用户参数
//
// 返 回 值: 
//   查找成功: HI_TRUE
//   查找失败: HI_FALSE
//
// 作    者: cuiate/c00233580 [2014-10-30]
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
// 函数名称: mrsFixedIncMalloc
// 功能描述: 增量分配内存
// 参数说明:
//   pstHead      [IN]  增量分配的头指针
//   ucDfxIndex   [IN]  统计量分配索引(支持0-6): 0->表档案，其他预留
//
// 返 回 值: 
//   成功: HI_ERR_SUCCESS
//   失败: HI_ERR_NOT_ENOUGH_MEMORY
//
// 作    者: cuiate/c00233580 [2014-10-30]
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


