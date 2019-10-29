
#ifndef __HI_MDM_TASK_INTERNAL_H__
#define __HI_MDM_TASK_INTERNAL_H__
#include <hi_types.h>
#include <hi_mdm_types.h>
#include <hi_mdm_task.h>

/**
* @ingroup  hct_task
* @brief  判断当前任务是否处在锁定状态。
*
* @par 描述:  
* 判断当前任务是否处在锁定状态。
* @attention 无。
*
* @param 无。
*
* @retval HI_TRUE 当前任务处在锁定状态。
* @retval HI_FALSE 当前任务未处在锁定状态。
*
* @par Dependency:
* @li hi_mdm_task.h: 该接口声明所在的头文件。
* @see  HI_MDM_TASK_Lock。
* @since HiMDL_V200R001C00
*/
HI_BOOL HI_MDM_TASK_IsLock(HI_VOID);

#endif//__HI_MDM_TASK_INTERNAL_H__
