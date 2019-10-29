//*****************************************************************************
//
//                  版权所有 (C), 2001-2014, 华为技术有限公司
//
//*****************************************************************************
//  文 件 名   : mrs_fixed_mem.h
//  版 本 号   : V1.0 
//  作    者   : cuiate/c00233580
//  生成日期   : 2014-10-22
//  功能描述   : 内存增量管理
//               函数声明及宏定义
//               
//      该模块的主要功能是提供一组接口，对规格较大的内存先分配一块内存，后续根
//  据实际使用情况再逐步增量分配，并提供按节点操作内存的接口。
//               
//  函数列表   : 
//  修改历史   : 
//  1.日    期 : 2014-10-22
//    作    者 : cuiate/c00233580
//    修改内容 : 创建文件 
//
//*****************************************************************************

#ifndef _MRS_MEM_INCREMENT_MANAGE_H_
#define _MRS_MEM_INCREMENT_MANAGE_H_
HI_START_HEADER

// 该部分代码目前只有CCO上用，所以用CCO宏分隔
// 若其它产品形态要是用，去掉CCO宏或者增加对应的产品形态宏即可
#if defined(PRODUCT_CFG_PRODUCT_TYPE_CCO)
//*****************************************************************************
//*****************************************************************************

#define MRS_SRV_FIXED_MEM_INIT_COUNT    (1024)
#define MRS_SRV_FIXED_MEM_INC_COUNT     (128)
#define MRS_SRV_ARCHIVES_DFX_INDEX      (0)


typedef struct
{
    HI_U32 ulInitItemCnt;   // 初始分配内存的节点数量
    HI_U32 ulIncItemCnt;    // 增量分配内存的节点数量
    HI_U32 ulMaxItemCnt;    // 节点数量的最大值
    HI_U32 ulItemSize;      // 节点大小
} MRS_FIXED_MEM_PRAMS_STRU;

typedef struct
{
    MRS_SRV_LINK    stLink;
    HI_U32          ulItemCount;
    HI_U8           pucMemBuf[0];
} MRS_FIXED_MEM_ITEM_STRU;

typedef struct
{
    HI_U32 ulAllocItemCnt;
    MRS_FIXED_MEM_PRAMS_STRU stParam;
    MRS_SRV_LIST stMemList;
} MRS_FIXED_MEM_HEAD_STRU, *MRS_FIXED_MEM_HEAD_PTR;



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
HI_PUBLIC MRS_FIXED_MEM_HEAD_PTR mrsFixedMalloc(MRS_FIXED_MEM_PRAMS_STRU * pstParam, HI_U8 ucDfxIndex);

//*****************************************************************************
// 函数名称: mrsFixedFree
// 功能描述: 释放内存
// 参数说明:
//   pstFixedHead [IN]  要释放的内存指针头
//
// 返 回 值: 无
// 作    者: cuiate/c00233580 [2014-10-30]
//*****************************************************************************
HI_PUBLIC HI_VOID mrsFixedFree(MRS_FIXED_MEM_HEAD_PTR pstFixedHead);

//*****************************************************************************
// 函数名称: mrsFixedMemClear
// 功能描述: 内存清0操作
// 参数说明:
//   pstFixedHead [IN]  要清0的内存指针头
//
// 返 回 值: 无
// 作    者: cuiate/c00233580 [2014-10-30]
//*****************************************************************************
HI_PUBLIC HI_VOID mrsFixedMemClear(MRS_FIXED_MEM_HEAD_PTR pstFixedHead);

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
HI_PUBLIC HI_VOID *mrsFixedMemGetItem(MRS_FIXED_MEM_HEAD_PTR pstFixedHead, HI_U32 ulIndex);

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
//
// 作    者: cuiate/c00233580 [2014-10-30]
//*****************************************************************************
HI_PUBLIC HI_U32 mrsFixedMemSetItem(MRS_FIXED_MEM_HEAD_PTR pstFixedHead, HI_U32 ulIndex, HI_VOID *pstItem, HI_U8 ucDfxIndex);

#endif

HI_END_HEADER
#endif //_MRS_MEM_INCREMENT_MANAGE_H_


