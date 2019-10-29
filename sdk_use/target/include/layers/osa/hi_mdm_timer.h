/**@defgroup hct_systimer 系统定时器
 * @ingroup osa
 */
#ifndef __HI_MDM_TIMER_H__
#define __HI_MDM_TIMER_H__
#include <hi_types.h>
#include <hi_mdm_types.h>

/**
 * @ingroup hct_systimer
 * 由用户申请空间存储本定时器管理信息，一般情况下为指向全局变量空间。该空间为底层管理，上层不允许操作。
 */
typedef struct
{
    HI_U32 data[5];/**< 用于存储所申请定时器的相关信息               */
}HI_TIMER_HANDLE_S;

/**
 * @ingroup hct_systimer
 * 用于设置所创建的定时器类型。
 */
typedef enum
{
    HI_TIMER_TYPE_ONCE,     /**< 表示单次定时器              */
    HI_TIMER_TYPE_PERIOD,   /**< 表示周期定时器               */
    HI_TIMER_TYPE_MAX
}HI_TIMER_TYPE_E;

/**
* @ingroup  hct_systimer
* @brief  定义定时器回调函数的类型。
*
* @par 描述:定义定时器回调函数的类型。
* 
* @attention 无。
*
* @param  ulData [IN] 回调入参。
*
* @retval 无
* @par Dependency:
* @li hi_mdm_timer.h: 该接口声明所在的头文件。
* @see HI_MDM_TIMER_Start | HI_MDM_TIMER_Stop
* @since HiMDL_V200R001C00
*/
typedef HI_VOID (*TIMER_PROC_FUNC)(HI_U32 ulData);
/**
* @ingroup  hct_systimer
* @brief  创建并启动系统定时器。
*
* @par 描述:
* 本API接口使用时分以下几个场景：
* @li 如果未创建定时器，则创建并启动。
* @li 如果该定时器到期后调用，则直接启动定时器。
* @li 如果定时器没有到期，则停止该定时器，重新启动定时器。
* @li 该接口支持在锁任务上下文中使用。
* @li 该接口支持在LISR上下文中使用。
* @li 该接口支持在关中断上下文中使用。
* 
* @attention 本定时器回调函数执行于中断上下文，
*回调函数尽量简单，不能使用睡眠、等待信号量等引起调度的函数。
*
* @param  pstTimerHandler [IN/OUT] 句柄。
* @param  pfnTimerRoutine [IN] 定时器回调函数。
* @param  ulTimeOut [IN] 定时器超时时间，单位:ms。
* @param  enType [IN] 定时器类型。
* @param  ulData [IN] 回调函数传参。
*
* @retval HI_ERR_SUCCESS 启动定时器成功。
* @retval 其他值 失败，详见hi_errno.h。
*
* @par Dependency:
* @li hi_mdm_timer.h: 该接口声明所在的头文件。
* @see HI_MDM_TIMER_Stop | HI_MDM_TIMER_Delete
* @since HiMDL_V200R001C00
*/
HI_U32 HI_MDM_TIMER_Start (HI_TIMER_HANDLE_S *pstTimerHandler, TIMER_PROC_FUNC pfnTimerRoutine,HI_U32 ulTimeOut,HI_TIMER_TYPE_E enType,HI_U32 ulData);
/**
* @ingroup  hct_systimer
* @brief  停止系统定时器。
*
* @par 描述:
* 停止系统定时器。
* @attention 
* 本接口仅停止定时器，并不删除该定时器。
* @li 该接口支持在锁任务上下文中使用。
* @li 该接口支持在LISR上下文中使用。
* @li 该接口支持在关中断上下文中使用。
*
* @param  pstTimerHandler [IN] 句柄。
*
* @retval HI_ERR_SUCCESS 停止定时器成功。
* @retval 其他值 失败，详见hi_errno.h。
*
* @par Dependency:
* @li hi_mdm_timer.h: 该接口声明所在的头文件。
* @see HI_MDM_TIMER_Start | HI_MDM_TIMER_Delete
* @since HiMDL_V200R001C00
*/
HI_U32 HI_MDM_TIMER_Stop(HI_TIMER_HANDLE_S *pstTimerHandler);
/**
* @ingroup  hct_systimer
* @brief  删除定时器。
*
* @par  描述:
* 如果定时器未到期，则先停止该定时器再删除。
* @attention 删除定时器后，如果句柄是动态申请的空间，需要释放。
*
* @param  pstTimerHandler [IN] 句柄。
*
* @retval HI_ERR_SUCCESS 删除定时器成功。
* @retval 其他值 失败，详见hi_errno.h。
*
* @par Dependency:
* @li hi_mdm_timer.h: 该接口声明所在的头文件。
* @see HI_MDM_TIMER_Start | HI_MDM_TIMER_Stop
* @since HiMDL_V200R001C00
*/
HI_U32 HI_MDM_TIMER_Delete(HI_TIMER_HANDLE_S *pstTimerHandler);

#endif

