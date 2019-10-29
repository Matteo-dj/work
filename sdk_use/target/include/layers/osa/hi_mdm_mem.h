/**@defgroup hct_mem  内存
 * @ingroup mem
 */

/** 
* @file hi_mdm_mem.h
* 
*  Copyright (C) HiSilicon Technologies Co., Ltd. 2017. All rights reserved.  \n
* 
* 描述：内存相关接口描述。\n
* @li系统将堆空间划分成了系统内存池、动态内存池、静态内存池3个内存池，
*       其中动态内存池和静态内存池是从系统内存池申请空间后创建的,
*       因此系统内存池的总大小和使用大小包含了动态内存池和静态内存池的大小。
* @li 系统内存池的剩余空间预留给OS使用，不建议使用，调用malloc和free接口进行申请释放。
* @li 静态内存池只支持初始化阶段调用，不支持释放，调用HI_MDM_GetStaticMem接口申请。
* @li 动态内存池为平时使用的内存，调用HI_MDM_Malloc和HI_MDM_Free进行申请释放。
* @li 内存模块支持按照内存池进行内存使用情况统计和按照模块进行内存使用情况统计。目前最多支持HI_MEM_SUPPORT_STAT_MOD_COUNT(60)个模块的内存使用情况统计。
*
*/


#ifndef __HI_MDM_MEM_H__
#define __HI_MDM_MEM_H__
#include <hi_types.h>
#include <hi_mdm_types.h>
#define HI_MEM_INVALID_POOL_ID 0xffffffff   /**< 非法内存池地址ID       */

/**
 * @ingroup hct_mem
 * 内存池使用情况统计信息。
 */
typedef struct
{
	HI_U32 ulPoolId;          /**< 内存池ID       */
    //OS获取信息
    HI_U32 ulTotalSize;       /**< 内存池大小    */
    HI_U32 ulTotalUsedSize;   /**< 已使用大小    */
    HI_U32 ulTotalFreeSize;   /**< 剩余空间    */
    HI_U32 ulMaxFreeNodeSize; /**< 最大节点大小    */
    HI_U32 ulUsedNodeNum;     /**< 使用节点个数    */
    HI_U32 ulFreeNodeNum;     /**< 空闲节点个数    */

    //中间层统计
    HI_U32 peekSec;           /**< 峰值时间    */
    HI_U32 peekSize;          /**< 峰值大小    */
    HI_U32 usedSize;          /**< 当前值    */
    
    HI_U16 mallocSucTimes;    /**< 申请成功次数    */
    HI_U16 freeSucTimes;      /**< 释放成功次数    */
    
    HI_U16 mallocFailTimes;   /**< 申请失败次数    */
    HI_U16 freeFailTimes;     /**< 是否失败次数    */

    HI_U16 mallocParamErrTimes;/**< 申请空间大小非法次数   */
    HI_U16 freeParamErrTimes;  /**< 是否内存地址非法次数  */
}HI_MDM_MEM_POOL_INFO_S;

/**
 * @ingroup hct_mem
 * 整体内存信息
 */
typedef struct
{
	HI_U32  heapTotal;/**<系统内存池大小 */
    HI_U32  heapUsed;/**<系统内存池使用空间 */
    HI_U32  heapFree;/**<系统内存池剩余空间 */
	HI_U32  codeSize;/**<text段+rodata段大小 */
    HI_U32  dataSize;/**<异常处理栈大小 */
    HI_U32  bssSize;/**<bss段大小 */
    HI_U32  rodataSize;/**<不支持 */
    HI_U32  mptSize;/**<mmu页表大小 */
}HI_MDM_MEM_INFO_S;

/**
 * @ingroup hct_mem
 * 模块内存使用统计信息(包含静态内存池和动态内存池的统计)。
 */
typedef struct
{
    HI_U16 modId;               /**< 模块ID     */
    HI_U16 pad;                 /**< 保留     */
    
    HI_U32 peekSec;             /**< 峰值时间     */
    HI_U32 peekSize;            /**< 峰值大小      */
    HI_U32 usedSize;            /**< 当前值      */
    
    HI_U16 mallocSucTimes;      /**< 申请成功次数      */
    HI_U16 freeSucTimes;        /**< 释放成功次数      */
    
    HI_U16 mallocFailTimes;     /**< 申请失败次数      */
    HI_U16 freeFailTimes;       /**< 是否失败次数      */

    HI_U16 mallocParamErrTimes; /**< 申请空间大小非法     */
    HI_U16 freeParamErrTimes;   /**< 是否内存地址非法     */
}HI_MDM_MEM_MOD_INFO_S;

/**
* @ingroup  hct_mem
* @brief  申请动态内存池空间。
*
* @par 描述:
* 申请动态内存池空间。
* @attention 
* @li 同一块内存的申请模块ID和释放模块ID要相同，否则会导致内存统计信息不正确。
* @param  ulModId [IN] 调用模块ID，用户需保证ulModId的正确性，本函数不做判断。
*该ID用于统计内存使用情况，取值见hi_config.h中的HI_MOD_ID_XX中宏定义部分，
*XX替换为各个模块，如HI_MOD_ID_SAL，其中给出了常用模块的ID，用户可以自己定义ID，但宏值不可与已有的冲突。
* @param  ulSize [IN] 申请内存大小
* @retval 非HI_NULL 申请的内存指针。
* @retval HI_NULL 申请内存失败。
* @par Dependency:
* <ul><li>hi_mdm_mem.h: 文件用于描述内存相关接口.</li></ul>
* @see  HI_MDM_Free。
* @since HiMDL_V200R001C00
*/
HI_EXTERN HI_PVOID HI_MDM_Malloc(HI_U32 ulModId, HI_U32 ulSize);

/**
* @ingroup  hct_mem
* @brief  释放动态内存池空间。
*
* @par 描述:
* 释放动态内存池空间。
* @attention 无。
* @param  ulModId [IN] 调用模块ID，取值见hi_config.h中的HI_MOD_ID_XX中宏定义部分。用户需保证ulModId的正确性，本函数不做判断。
* @param  pAddr [IN] 内存首地址
* @par Dependency:
* <ul><li>hi_mdm_mem.h: 文件用于描述内存相关接口.</li></ul>
* @see  HI_MDM_Malloc。
* @since HiMDL_V200R001C00
*/
HI_EXTERN HI_VOID HI_MDM_Free(HI_U32 ulModId, HI_PVOID pAddr);

/**
* @ingroup  hct_mem
* @brief  申请静态内存池空间。
*
* @par 描述:
* 申请静态内存池空间。
* @attention 无。
* @param  ulModId [IN] 调用模块ID，取值见hi_config.h中的HI_MOD_ID_XX中宏定义部分。
* @param  ulSize [IN] 申请内存大小
* @retval 非HI_NULL 申请的内存指针。
* @retval HI_NULL 申请内存失败。
* @par Dependency:
* <ul><li>hi_mdm_mem.h: 文件用于描述内存相关接口.</li></ul>
* @see  无。
* @since HiMDL_V200R001C00
*/
HI_EXTERN HI_PVOID HI_MDM_GetStaticMem(HI_U32 ulModId, HI_U32 ulSize);



/**
* @ingroup  hct_mem
* @brief  获取软件内存大小。
*
* @par 描述:
* 获取软件内存大小。
* @attention 无。
* @retval #HI_U32类型数值 软件内存大小。
* @par Dependency:
* <ul><li>hi_mdm_mem.h: 文件用于描述内存相关接口.</li></ul>
* @see  HI_MDM_MEM_GetSwSize。
* @since HiMDL_V200R001C00
*/
HI_EXTERN HI_U32 HI_MDM_MEM_GetSwSize(HI_VOID);

/**
* @ingroup  hct_mem
* @brief  获取内存整体信息。
*
* @par 描述:
* 获取内存整体信息。
* @attention 无。
* @param  pstSysMemInfo [OUT] 类型 整体内存信息
* @retval #HI_ERR_SUCCESS 成功。
* @retval 其他值 失败，详见hi_errno.h。
* @par Dependency:
* <ul><li>hi_mdm_mem.h: 文件用于描述内存相关接口.</li></ul>
* @see  无。
* @since HiMDL_V200R001C00
*/
HI_EXTERN HI_U32 HI_MDM_MEM_GetSysInfo(HI_OUT HI_MDM_MEM_INFO_S * pstSysMemInfo);

/**
* @ingroup  hct_mem
* @brief  获取静态内存池统计信息。
*
* @par 描述:
* 获取静态内存池统计信息。
* @attention 无。 
* @param  pstMemPoolInfo [OUT] 类型 静态内存信息
* @retval #HI_ERR_SUCCESS 成功。
* @retval 其他值 失败，详见hi_errno.h。
* @par Dependency:
* <ul><li>hi_mdm_mem.h: 文件用于描述内存相关接口.</li></ul>
* @see  无。
* @since HiMDL_V200R001C00
*/
HI_EXTERN HI_U32 HI_MDM_MEM_GetStaticPoolInfo(HI_OUT HI_MDM_MEM_POOL_INFO_S *pstMemPoolInfo);

/**
* @ingroup  hct_mem
* @brief  获取动态内存池统计信息。
*
* @par 描述:
* 获取动态内存池统计信息。
* @attention 无。
* @param  pstSysMemInfo [OUT] 动态内存信息
* @retval #HI_ERR_SUCCESS 成功。
* @retval 其他值 失败，详见hi_errno.h。
* @par Dependency:
* <ul><li>hi_mdm_mem.h: 文件用于描述内存相关接口.</li></ul>
* @see  无。
* @since HiMDL_V200R001C00
*/
HI_EXTERN HI_U32 HI_MDM_MEM_GetDynPoolInfo(HI_OUT HI_MDM_MEM_POOL_INFO_S *pstMemPoolInfo);

/**
* @ingroup  hct_mem
* @brief  获取系统内存池统计信息。
*
* @par 描述:
* 获取系统内存池统计信息。
* @attention 无。
* @param  pstMemPoolInfo [OUT] 类型 系统预留的可分配的内存信息
* @retval #HI_ERR_SUCCESS 成功。
* @retval 其他值 失败，详见hi_errno.h。
* @par Dependency:
* <ul><li>hi_mdm_mem.h: 文件用于描述内存相关接口.</li></ul>
* @see  无。
* @since HiMDL_V200R001C00
*/
HI_EXTERN HI_U32 HI_MDM_MEM_GetSysPoolInfo(HI_OUT HI_MDM_MEM_POOL_INFO_S *pstMemPoolInfo);

/**
* @ingroup  hct_mem
* @brief  获取当前对内存有申请释放操作的模块个数。
*
* @par 描述: 
* 获取当前对内存有申请释放的模块个数。
* @attention 无。
* @retval #HI_U32类型数值 模块个数。
* @par Dependency:
* <ul><li>hi_mdm_mem.h: 文件用于描述内存相关接口.</li></ul>
* @see  HI_MDM_MEM_GetModInfoByIdx
* @since HiMDL_V200R001C00
*/
HI_EXTERN HI_U32 HI_MDM_MEM_GetModNum(HI_VOID);

/**
* @ingroup  hct_mem
* @brief  按索引获取模块内存使用情况。
*
* @par 描述: 
* 按索引获取模块内存使用情况。
* @attention 无。
* @param  ulIdx [IN] 模块索引号。取值范围[0,HI_MOD_MAX_NUM)。
* @param  pstMemModInfo [OUT] 内存使用情况。
* @retval #HI_ERR_SUCCESS 成功。
* @retval 其他值 失败，详见hi_errno.h。
* @par Dependency:
* <ul><li>hi_mdm_mem.h: 文件用于描述内存相关接口.</li></ul>
* @see  HI_MDM_MEM_GetModNum
* @since HiMDL_V200R001C00
*/
HI_EXTERN HI_U32 HI_MDM_MEM_GetModInfoByIdx(HI_U32 ulIdx,HI_MDM_MEM_MOD_INFO_S *pstMemModInfo);

/**
* @ingroup  hct_mem
* @brief  按模块ID获取模块内存使用情况。
*
* @par 描述:
* 按模块ID获取模块内存使用情况。
* @attention 无。
* @param  ulModId [IN] 模块ID号，取值见hi_config.h中的HI_MOD_ID_XX中宏定义部分。
* @param  pstMemModInfo [OUT] 内存使用情况。
* @retval #HI_ERR_SUCCESS 成功。
* @retval 其他值 失败，详见hi_errno.h。
* @par Dependency:
* <ul><li>hi_mdm_mem.h: 文件用于描述内存相关接口.</li></ul>
* @see  无。
* @since HiMDL_V200R001C00
*/
HI_EXTERN HI_U32 HI_MDM_MEM_GetModInfoByModId(HI_U32 ulModId,HI_MDM_MEM_MOD_INFO_S *pstMemModInfo);
#endif
