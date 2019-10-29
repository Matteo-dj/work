/**@defgroup hct_init_early 初始化配置
 * @ingroup osa
 */
 
 /** 
 * @file hi_mdm_cfg_early.h
 * 
 *  Copyright (C) HiSilicon Technologies Co., Ltd. 2017. All rights reserved.  \n
 * 
 * 描述：该文件用于描述一阶段(OS初始话之前)启动配置相关接口。\n
 * @li 该文件包含接口仅支持在APP_InitCfgEarly流程中调用。
 * @li APP_InitCfgEarly初始化阶段，BSS段尚未未初始化，因此不能使用BSS段变量。
 * @li APP_InitCfgEarly初始化阶段，系统未初始化完成，除hi_mdm_cfg_early.h提供的接口和一些存硬件接口外，不能调用其它接口(包括内存，任务，睡眠，其它系统资源相关接口)。
 *
 */
#ifndef __HI_MDM_CFG_EARLY_H__
#define __HI_MDM_CFG_EARLY_H__

/**
 * @ingroup hct_init_early
 * 系统资源初始化配置
 */
typedef struct
{
    HI_U32 ulTskNum;/**< task个数 */
    HI_U32 ulQueNum;/**< 消息队列个数 */
    HI_U32 ulSemNum;/**< 信号量个数 */
    HI_U32 ulMuxNum;/**< 互斥锁个数 */
    HI_U32 ulSwtNum;/**< 系统定时器个数 */
    HI_U32 ulWorkqNum;/**< 中断后半段job个数 */
    HI_U32 ulEventNum;/**< 事件个数 */
}HI_CFG_OSA_RESOURCE_S;

/**
* @ingroup  hct_init_early
* @brief  配置osa资源
*
* @par 描述:
* 配置osa资源
* @attention 本接口仅支持在初始化阶段调用。
*
* @param  pCfgOsaResouce [IN] 系统资源初始化配置。
*
* @retval #HI_ERR_SUCCESS 成功。
* @retval 其他值 失败，详见hi_errno.h。
*
* @par Dependency:
* @li hi_mdm_cfg_early.h: 该接口声明所在的头文件。
* @see 无。
* @since HiMDL_V200R001C00
*/

HI_U32 HI_CFG_SetOsaResourceEarly(HI_CFG_OSA_RESOURCE_S *pCfgOsaResouce);


/**
* @ingroup  hct_init_early
* @brief  设置软件内存大小。
*
* @par 描述:
* 设置软件内存大小，不允许大于实际物理器件大小。
* @attention 本接口仅支持在初始化阶段调用。
* @param  ulSize [IN] 申请内存大小
* @retval #HI_ERR_SUCCESS 成功。
* @retval 其他值 失败，详见hi_errno.h。
* @par Dependency:
* @li hi_mdm_cfg_early.h: 该接口声明所在的头文件。
* @see  HI_MDM_MEM_GetSwSize。
* @since HiMDL_V200R001C00
*/
HI_U32 HI_MDM_MEM_SetSwSize(HI_IN HI_U32 ulSize);

#endif

