
/**@defgroup hct_share_mem  共享内存
 * @ingroup mem
 */
 
/** 
* @file hi_share_memory.h
* 
*  Copyright (C) HiSilicon Technologies Co., Ltd. 2018. All rights reserved.  \n
* 
* 描述：共享内存接口。\n
* @li 功能1:在上电初始化阶段申请一块很大的内存，防止系统运行过程中无法申请到一整块大内存。
* @li 功能2:支持多个功能分时复用该块大内存。
*
* History:
* Version                  Date         DefectNum           Description
* V200R001C00SPC050B011    2018-12-03   DTS2018112903669    增加共享内存接口
*/

#ifndef __HI_SHARE_MEMORY_H__
#define __HI_SHARE_MEMORY_H__
#include <hi_types.h>
#include <hi_mdm_types.h>

#ifdef PRODUCT_CFG_SUPPORT_SHARE_MEMORY

/**
* @ingroup  hct_share_mem
* @brief  共享内存初始化。
*
* @par 描述:  
* 共享内存初始化。
* @attention 无。
* 
* @param 无。
*
* @retval #HI_ERR_SUCCESS 成功。
* @retval 其他值 失败，详见hi_errno.h。
*
* @par Dependency:
* @li hi_share_memory.h: 该接口声明所在的头文件。
* @see  无。
* @since HiMDL_V200R001C00
*/
HI_U32 HI_SHARE_MEM_Init(HI_VOID);

/**
* @ingroup  hct_share_mem
* @brief  申请共享内存。
*
* @par 描述:  
* 申请共享内存。
* @attention 无。
* 
* @param  ulSize [IN] 预申请的共享内存大小。
*
* @retval #HI_PVOID 申请到的共享内存地址。
*
* @par Dependency:
* @li hi_share_memory.h: 该接口声明所在的头文件。
* @see  无。
* @since HiMDL_V200R001C00
*/
HI_PVOID HI_SHARE_MEM_GetMem(HI_U32 ulSize);

/**
* @ingroup  hct_share_mem
* @brief  释放共享内存。
*
* @par 描述:  
* 释放共享内存。
* @attention 无。
* 
* @param  ulAddr [IN] 释放的内存地址。
*
* @par Dependency:
* @li hi_share_memory.h: 该接口声明所在的头文件。
* @see  无。
* @since HiMDL_V200R001C00
*/
HI_VOID HI_SHARE_MEM_FreeMem(HI_PVOID ulAddr);

/**
* @ingroup  hct_share_mem
* @brief  共享内存是否空闲。
*
* @par 描述:  
* 共享内存是否空闲。
* @attention 无。
* 
* @param 无。
*
* @retval #HI_TRUE 空闲。
* @retval #HI_FALSE 被占用。
*
* @par Dependency:
* @li hi_share_memory.h: 该接口声明所在的头文件。
* @see  无。
* @since HiMDL_V200R001C00
*/
HI_BOOL HI_SHARE_MEM_IsFreeMem(HI_VOID);
#else
/**
* @ingroup  hct_share_mem
* @brief  共享内存初始化。
*
* @par 描述:  
* 共享内存初始化。
* @attention 无。
* 
* @param 无。
*
* @retval #HI_ERR_SUCCESS 成功。
* @retval 其他值 失败，详见hi_errno.h。
*
* @par Dependency:
* @li hi_share_memory.h: 该接口声明所在的头文件。
* @see  无。
* @since HiMDL_V200R001C00
*/
HI_PRVL HI_U32 HI_SHARE_MEM_Init(HI_VOID){return HI_ERR_SUCCESS;}
/**
* @ingroup  hct_share_mem
* @brief  申请共享内存。
*
* @par 描述:  
* 申请共享内存。
* @attention 无。
* 
* @param  ulSize [IN] 预申请的共享内存大小。
*
* @retval #HI_PVOID 申请到的共享内存地址。
*
* @par Dependency:
* @li hi_share_memory.h: 该接口声明所在的头文件。
* @see  无。
* @since HiMDL_V200R001C00
*/
HI_PRVL HI_PVOID HI_SHARE_MEM_GetMem(HI_U32 ulSize){return HI_NULL;}
/**
* @ingroup  hct_share_mem
* @brief  释放共享内存。
*
* @par 描述:  
* 释放共享内存。
* @attention 无。
* 
* @param  ulAddr [IN] 释放的内存地址。
*
* @par Dependency:
* @li hi_share_memory.h: 该接口声明所在的头文件。
* @see  无。
* @since HiMDL_V200R001C00
*/
HI_PRVL HI_VOID HI_SHARE_MEM_FreeMem(HI_PVOID ulAddr){return;}
/**
* @ingroup  hct_share_mem
* @brief  共享内存是否空闲。
*
* @par 描述:  
* 共享内存是否空闲。
* @attention 无。
* 
* @param 无。
*
* @retval #HI_TRUE 空闲。
* @retval #HI_FALSE 被占用。
*
* @par Dependency:
* @li hi_share_memory.h: 该接口声明所在的头文件。
* @see  无。
* @since HiMDL_V200R001C00
*/
HI_PRVL HI_BOOL HI_SHARE_MEM_IsFreeMem(HI_VOID){return HI_FALSE;}
#endif

#endif//__HI_MDM_SHARE_MEMORY_H__

