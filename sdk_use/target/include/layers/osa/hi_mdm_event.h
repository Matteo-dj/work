/**@defgroup hct_event 事件
 * @ingroup osa
 */

#ifndef __HI_MDM_EVENT_H__
#define __HI_MDM_EVENT_H__
#include <hi_types.h>
#include <hi_mdm_types.h>

#define HI_INVALID_EVENT_ID           (0xffffffff)/**< 获取事件ID失败         */
#define HI_EVENT_WAITMODE_AND         (4) /**< 掩码所有事件均被发生了，才能读取成功       */
#define HI_EVENT_WAITMODE_OR          (2) /**< 掩码任何事件发生了，就可以读取成功       */ 
#define HI_EVENT_WAITMODE_CLR         (1) /**< 读取事件成功后自动清除对应事件        */ 

/**
* @ingroup  hct_event
* @brief  创建事件。
*
* @par 描述:  
* 创建事件。
* @attention 无。
*
* @param  pEventID [OUT] 事件ID号。
* @param  szName[8] [IN] 事件名字。
*
* @retval #HI_ERR_SUCCESS 成功。
* @retval 其他值 失败，详见hi_errno.h。
*
* @par Dependency:
* @li hi_mdm_event.h: 该接口声明所在的头文件。
* @see  HI_MDM_EVENT_Delete
* @since HiMDL_V200R001C00
*/
HI_U32 HI_MDM_EVENT_Create(HI_OUT HI_U32 *pEventID,HI_CHAR szName[8]);

/**
* @ingroup  hct_event
* @brief  发送事件。
*
* @par 描述:  
* 发送事件。
* @attention
* @li 该接口支持在锁任务上下文中使用。
* @li 该接口支持在LISR上下文中使用。
*
* @param  ulEventID [IN] 事件ID号。
* @param  ulEventBits [IN] 事件bit位。
*
* @retval #HI_ERR_SUCCESS 成功。
* @retval 其他值 失败，详见hi_errno.h。
*
* @par Dependency:
* @li hi_mdm_event.h: 该接口声明所在的头文件。
* @see  HI_MDM_EVENT_Wait
* @since HiMDL_V200R001C00
*/
HI_U32 HI_MDM_EVENT_Send(HI_U32 ulEventID,HI_U32 ulEventBits);

/**
* @ingroup  hct_event
* @brief  等待事件。
*
* @par 描述:  等待事件。
* @attention 无。
*
* @param  ulEventID [IN] 事件ID号。
* @param  ulEventMask [IN] 事件bit位标记掩码。
* @param  pEventBits [OUT] 事件bit位。
* @param  ulTimeOut [IN] 事件超时时间，单位:ms。
* @param  ulFlag [IN] 标志。取值:
* @li #HI_EVENT_WAITMODE_AND
* @li #HI_EVENT_WAITMODE_OR
* @li #HI_EVENT_WAITMODE_CLR \n
* 说明:#HI_EVENT_WAITMODE_AND和#HI_EVENT_WAITMODE_OR可以"或"#HI_EVENT_WAITMODE_CLR去使用，\n
*如HI_EVENT_WAITMODE_AND|HI_EVENT_WAITMODE_CLR，但是这两个不可以"或"去使用。
*
* @retval #HI_ERR_SUCCESS 成功。
* @retval 其他值 失败，详见hi_errno.h。
*
* @par Dependency:
* @li hi_mdm_event.h: 该接口声明所在的头文件。
* @see  HI_MDM_EVENT_Send
* @since HiMDL_V200R001C00
*/
HI_U32 HI_MDM_EVENT_Wait (HI_U32 ulEventID, HI_U32 ulEventMask,HI_OUT HI_U32 * pEventBits,HI_U32 ulTimeOut,HI_U32 ulFlag);

/**
* @ingroup  hct_event
* @brief  清除事件。
*
* @par 描述:  
* 清除事件。
* @attention
* @li 该接口支持在锁任务上下文中使用。
* @li 该接口支持在LISR上下文中使用。
*
* @param  ulEventID [IN] 类型 #HI_U32  事件ID号。
* @param  ulEventBits [IN] 类型 #HI_U32 事件bit位。
*
* @retval #HI_ERR_SUCCESS 成功。
* @retval 其他值 失败，详见hi_errno.h。
*
* @par Dependency:
* @li hi_mdm_event.h: 该接口声明所在的头文件。
* @see  HI_MDM_EVENT_Wait
* @since HiMDL_V200R001C00
*/
HI_U32 HI_MDM_EVENT_Clear(HI_U32 ulEventID,HI_U32 ulEventBits);

/**
* @ingroup  hct_event
* @brief  删除事件。
*
* @par 描述:  
* 删除事件。
* @attention 无。
*
* @param  ulEventID [IN] 事件ID号。
*
* @retval #HI_ERR_SUCCESS 成功。
* @retval 其他值 失败，详见hi_errno.h。
*
* @par Dependency:
* @li hi_mdm_event.h: 该接口声明所在的头文件。
* @see  HI_MDM_EVENT_Create
* @since HiMDL_V200R001C00
*/
HI_U32 HI_MDM_EVENT_Delete(HI_U32 ulEventID);
#endif

