/**@defgroup hct_time 时间
 * @ingroup osa
 */
#ifndef __HI_MDM_TIME_H__
#define __HI_MDM_TIME_H__
#include <hi_types.h>
#include <hi_mdm_types.h>

typedef HI_U32 (*OS_UDELAY_FUNC)(HI_U32 ulUs);

/**
* @ingroup  hct_time
* @brief  延时，微秒级。
*
* @par 描述:  
* 延时操作，占用CPU。
* @attention 
* @li 该接口支持在锁任务上下文中使用，但会影响其它任务调度。
* @li 该接口支持在LISR上下文中使用，但会影响其它中断和任务调度。
* @li 该接口支持在关中断上下文中使用，但会影响其它中断和任务调度。
*
* @param ulus [IN] 延时时间，单位:us。
*
* @retval #HI_ERR_SUCCESS 延时成功。
* @retval 其他值 失败，详见hi_errno.h。
*
* @par Dependency:
* @li hi_mdm_time.h: 该接口声明所在的头文件。
* @see  无。
* @since HiMDL_V200R001C00
*/
HI_U32 HI_MDM_Udelay(HI_U32 ulus);

/**
* @ingroup  hct_time
* @brief  获取系统tick值(32bit)。
*
* @par 描述: 
* 获取系统tick值(32bit)。
* @attention 
* @li 该接口支持在锁任务上下文中使用。
* @li 该接口支持在LISR上下文中使用。
* @li 该接口支持在关中断上下文中使用。
*
* @param 无。
*
* @retval #HI_U32类型数值 系统tick值。
*
* @par Dependency:
* @li hi_mdm_time.h: 该接口声明所在的头文件。
* @see  无。
* @since HiMDL_V200R001C00
*/
HI_U32 HI_MDM_GetTick(HI_VOID);

/**
* @ingroup  hct_time
* @brief  获取系统tick值(64bit)。
*
* @par 描述: 
* 获取系统tick值(64bit)。
* @attention 该接口调用处必须包含头文件hi_mdm_time.h，否则按照未声明接口处理，会将Tick值当做int类型返回，产生截断错误。
* @li 该接口支持在锁任务上下文中使用。
* @li 该接口支持在LISR上下文中使用。
* @li 该接口支持在关中断上下文中使用。
*
* @param 无。
*
* @retval #HI_U64类型数值 系统tick值。
*
* @par Dependency:
* @li hi_mdm_time.h: 该接口声明所在的头文件。
* @see  无。
* @since HiMDL_V200R001C00
*/
HI_U64 HI_MDM_GetTick64(HI_VOID);

/**
* @ingroup  hct_time
* @brief  获取系统时间(单位:ms)。
*
* @par 描述: 
* 获取系统时间(单位:ms)。
* @attention 
* @li 该接口支持在锁任务上下文中使用。
* @li 该接口支持在LISR上下文中使用。
* @li 该接口支持在关中断上下文中使用。
*
* @param 无。
*
* @retval #HI_U32类型数值 系统时间。
*
* @par Dependency:
* @li hi_mdm_time.h: 该接口声明所在的头文件。
* @see  无。
* @since HiMDL_V200R001C00
*/
HI_U32 HI_MDM_GetMilliSeconds(HI_VOID);

/**
* @ingroup  hct_time
* @brief  获取系统时间(单位:秒)。
*
* @par 描述: 
* 获取系统时间(单位:秒)。
* @attention 
* @li 该接口支持在锁任务上下文中使用。
* @li 该接口支持在LISR上下文中使用。
* @li 该接口支持在关中断上下文中使用。
*
* @param 无。
*
* @retval #HI_U32类型数值 系统时间。
* @retval #HI_ERR_FAILURE 获取时间失败。
*
* @par Dependency:
* @li hi_mdm_time.h: 该接口声明所在的头文件。
* @see  无。
* @since HiMDL_V200R001C00
*/
HI_U32 HI_MDM_GetSeconds(HI_VOID);

/**
* @ingroup  hct_time
* @brief  获取系统实时时间(单位:s)。
*
* @par 描述: 
* 获取系统实时时间(单位:s)。
* @attention 
* @li 该接口支持在锁任务上下文中使用。
* @li 该接口支持在LISR上下文中使用。
* @li 该接口支持在关中断上下文中使用。
*
* @param 无。
*
* @retval #HI_U32类型数值 系统实时时间。
* @retval #HI_ERR_FAILURE 获取时间失败。
*
* @par Dependency:
* @li hi_mdm_time.h: 该接口声明所在的头文件。
* @see  无。
* @since HiMDL_V200R001C00
*/
HI_U32 HI_MDM_GetRealTime(HI_VOID);

/**
* @ingroup  hct_time
* @brief  设置系统实时时间。
*
* @par 描述: 
* 设置系统实时时间。
* @attention 
* @li 该接口支持在锁任务上下文中使用。
* @li 该接口支持在LISR上下文中使用。
* @li 该接口支持在关中断上下文中使用。
*
* @param ulSec [IN] 将系统实时时间设置为该值。
*
* @retval #HI_ERR_SUCCESS 设置成功。
* @retval #HI_ERR_FAILURE 设置失败。
*
* @par Dependency:
* @li hi_mdm_time.h: 该接口声明所在的头文件。
* @see  无。
* @since HiMDL_V200R001C00
*/
HI_U32 HI_MDM_SetRealTime(HI_U32 ulSec);

/**
* @ingroup  hct_time
* @brief  设置当前墙上时间。
*
* @par 描述: 
* 设置当前墙上时间，墙上时间是RTC时间的另一种表示格式。
* @attention 
* @li 该接口支持在锁任务上下文中使用。
* @li 该接口支持在LISR上下文中使用。
* @li 该接口支持在关中断上下文中使用。
*
* @param  calendarTime [IN] 要设置的时间，有效范围 1970-01-01~2105-12-31。
*
* @retval #HI_ERR_SUCCESS  成功。
* @retval 其他值 失败，详见hi_errno.h。
*
* @par Dependency:
* @li hi_mdm_time.h: 该接口声明所在的头文件。
* @see  无。
* @since HiMDL_V200R001C00
*/
HI_U32 HI_MDM_SetCalendarTime(HI_IN HI_SYS_CALENDAR_TIME_S * calendarTime);

/**
* @ingroup  hct_time
* @brief   获取当前墙上时间。
*
* @par 描述: 
* 获取当前墙上时间，墙上时间是RTC时间的另一种表示格式。
* @attention 
* @li 该接口支持在锁任务上下文中使用。
* @li 该接口支持在LISR上下文中使用。
* @li 该接口支持在关中断上下文中使用。
*
* @param  calendarTime [OUT] 当前墙上时间，未设置情况下从1970-1-1 0:0:0开始计时。
*
* @retval #HI_ERR_SUCCESS 成功。
* @retval 其他值 失败，详见hi_errno.h。
*
* @par Dependency:
* @li hi_mdm_time.h: 该接口声明所在的头文件。
* @see  无。
* @since HiMDL_V200R001C00
*/
HI_U32 HI_MDM_GetCalendarTime(HI_OUT HI_SYS_CALENDAR_TIME_S * calendarTime);

/**
* @ingroup  hct_time
* @brief   将秒值转换成墙上时间。
*
* @par 描述:  
* 将秒值转换成墙上时间。
* @attention 
* @li 该接口支持在锁任务上下文中使用。
* @li 该接口支持在LISR上下文中使用。
* @li 该接口支持在关中断上下文中使用。
*
* @param  second [IN] 时间(单位:s) 。
* @param  calendarTime [OUT] 墙上时间。
*
* @retval #HI_ERR_SUCCESS 成功。
* @retval 其他值 失败，详见hi_errno.h。
*
* @par Dependency:
* @li hi_mdm_time.h: 该接口声明所在的头文件。
* @see  无。
* @since HiMDL_V200R001C00
*/
HI_U32 HI_MDM_MakeSecondToCalendar(HI_IN HI_U32 second,HI_OUT HI_SYS_CALENDAR_TIME_S * calendarTime);

/**
* @ingroup  hct_time
* @brief   将墙上时间转换成秒。
*
* @par 描述:  
* 将墙上时间转换成秒。
* @attention 
* @li 该接口支持在锁任务上下文中使用。
* @li 该接口支持在LISR上下文中使用。
* @li 该接口支持在关中断上下文中使用。
*
* @param  calendarTime [IN] 墙上时间，有效范围 1970-01-01~2105-12-31。
* @param  second [OUT] RTC时间(单位:s) 。
*
* @retval #HI_ERR_SUCCESS 成功。
* @retval 其他值 失败，详见hi_errno.h。
*
* @par Dependency:
* @li hi_mdm_time.h: 该接口声明所在的头文件。
* @see  无。
* @since HiMDL_V200R001C00
*/
HI_U32 HI_MDM_MakeCalendarToSecond(HI_IN HI_SYS_CALENDAR_TIME_S * calendarTime,HI_OUT HI_U32 *second);

#endif
