/**@defgroup hct_watchdog 看门狗
 * @ingroup drivers
 */

#ifndef __HI_MDM_WATCHDOG_H__
#define __HI_MDM_WATCHDOG_H__
#include <hi_types.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
* @ingroup  hct_watchdog
* @brief 使能看门狗。
*
* @par 描述:
* 使能看门狗。
* @attention 无。
*
* @param 无。
*
* @retval 无。
*
* @par Dependency:
* @li hi_mdm_watchdog.h: 该接口声明所在的头文件。
* @see 无。
* @since HiMDL_V200R001C00
*/
HI_EXTERN HI_VOID HI_MDM_WDG_Enable(HI_VOID);

/**
* @ingroup  hct_watchdog
* @brief 喂狗。
*
* @par 描述:
* 喂狗,重置看门狗到期时间。
* @attention 无。
*
* @param 无。
*
* @retval 无。
*
* @par Dependency:
* @li hi_mdm_watchdog.h: 该接口声明所在的头文件。
* @see 无。
* @since HiMDL_V200R001C00
*/
HI_EXTERN HI_VOID HI_MDM_WDG_Feed(HI_VOID);

/**
* @ingroup  hct_watchdog
* @brief 关闭看门狗。
*
* @par 描述:
* 该接口主要完成以下操作:
* @li 禁止WatchDog时钟使能控制位。
* @li 屏蔽WatchDog复位功能。
* @attention 无。
*
* @param 无。
*
* @retval 无。
*
* @par Dependency:
* @li hi_mdm_watchdog.h: 该接口声明所在的头文件。
* @see 无。
* @since HiMDL_V200R001C00
*/
HI_EXTERN HI_VOID HI_MDM_WDG_Disable(HI_VOID);

#ifdef __cplusplus
}
#endif
#endif
