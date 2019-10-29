/**@defgroup hct_sem 信号量
 * @ingroup osa
 */

#ifndef __HI_MDM_SEM_H__
#define __HI_MDM_SEM_H__
#include <hi_types.h>
#include <hi_mdm_types.h>

/** 
* @ingroup  hct_sem
* 信号量初值
* 
* 函数#HI_MDM_SEM_BCreate入参ucInit的取值，表示创建的信号量的初始值为1。
*/
#define HI_SEM_ONE  ((HI_U8)1)
/** 
* @ingroup  hct_sem
* 信号量初值
* 
* 函数#HI_MDM_SEM_BCreate入参ucInit的取值，表示创建的信号量的初始值为0。
*/
#define HI_SEM_ZERO  ((HI_U8)0)

/**
* @ingroup  hct_sem
* @brief  创建信号量。
*
* @par 描述: 
* 创建信号量。
* @attention 无。
*
* @param  pSemId [OUT] 信号量ID号。
* @param  szName[8] [IN] 信号量名字。
* @param  ucInit [IN] 值为#HI_SEM_ONE用作临界资源保护；值为#HI_SEM_ZERO用作同步。
*
* @retval #HI_ERR_SUCCESS      创建成功。
* @retval 其他值 创建失败，详见hi_errno.h。
*
* @par Dependency:
* @li hi_mdm_sem.h: 该接口声明所在的头文件。
* @see  HI_MDM_SEM_Delete
* @since HiMDL_V200R001C00
*/
HI_U32 HI_MDM_SEM_BCreate (HI_U32 * pSemId, HI_CHAR szName[8], HI_U8 ucInit);

/**
* @ingroup  hct_sem
* @brief  删除信号量。
*
* @par 描述:  
* 删除信号量。
* @attention 无。
*
* @param  ulSemId [IN] 信号量ID号。
*
* @retval #HI_ERR_SUCCESS      删除成功。
* @retval #HI_ERR_SEM_DELETE_FAIL 删除失败。
*
* @par Dependency:
* @li hi_mdm_sem.h: 该接口声明所在的头文件。
* @see  HI_MDM_SEM_BCreate。
* @since HiMDL_V200R001C00
*/
HI_U32 HI_MDM_SEM_Delete(HI_U32 ulSemId);

/**
* @ingroup  hct_sem
* @brief  获取信号量。
*
* @par 描述:   
* 等待信号量。
* @attention 无。

* @param  ulSemId [IN] 信号量ID号。
* @param  ulTimeOut [IN] 超时时间，单位:ms。若设置成HI_SYS_WAIT_FOREVER，则为永久等待。
*
* @retval #HI_ERR_SUCCESS 成功。
* @retval 其他值 失败，详见hi_errno.h。
*
* @par Dependency:
* @li hi_mdm_sem.h: 该接口声明所在的头文件。
* @see  HI_MDM_SEM_Signal
* @since HiMDL_V200R001C00
*/
HI_U32 HI_MDM_SEM_Wait(HI_U32 ulSemId, HI_U32 ulTimeOut);

/**
* @ingroup  hct_sem
* @brief  释放信号量。
*
* @par 描述:   
* 释放信号量。
* @attention 
* @li 该接口支持在锁任务上下文中使用。
* @li 该接口支持在LISR上下文中使用。
*
* @param  ulSemId [IN] 信号量ID号。
*
* @retval #HI_ERR_SUCCESS 成功。
* @retval 其他值 失败，详见hi_errno.h。
*
* @par Dependency:
* @li hi_mdm_sem.h: 该接口声明所在的头文件。
* @see  HI_MDM_SEM_Wait
* @since HiMDL_V200R001C00
*/
HI_U32 HI_MDM_SEM_Signal (HI_U32 ulSemId);
#endif

