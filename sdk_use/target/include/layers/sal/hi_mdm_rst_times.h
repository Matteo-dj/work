/**@defgroup hct_rst_info 复位信息
 * @ingroup sal
 */

#ifndef __HI_MDM_RST_TIMES_H__
#define __HI_MDM_RST_TIMES_H__
#include <hi_types.h>
#include <hi_mdm_types.h>

/**
* @ingroup  hct_rst_info
* @brief 获取复位次数,升级时清0。
*
* @par 描述:
* 获取升级之后到现在为止的复位次数。该次数为HI_U32类型，一直累加，不会通过调用HI_MDM_ClrRelativeRstTimes接口清0。直到最大值0xFFFFFFFF后自动归0，升级软件后清0。
* @attention
* @li 非阻塞接口，支持在中断、关中断、锁任务上下文调用。
* @li OS(非BOOT)启动30秒内复位，下次启动时，该复位次数不会增加。
*
* @retval #HI_U32 复位次数。
*
* @par Dependency:
* @li hi_mdm_rst_times.h: 该接口声明所在的头文件。
* @since HiMDL_V200R001C00
*/
HI_U32 HI_MDM_GetRstTimes(HI_VOID);

/**
* @ingroup  hct_rst_info
* @brief 获取复位次数,升级时清0,调用HI_MDM_ClrRelativeRstTimes接口时清0。
*
* @par 描述:
* 获取复位次数。调用HI_MDM_ClrRelativeRstTimes接口时清0，升级时清0。
* @attention
* @li 非阻塞接口，支持在中断、关中断、锁任务上下文调用。
* @li OS(非BOOT)启动30秒内复位，下次启动时，该复位次数不会增加。
*
* @retval #HI_U32 复位次数。
*
* @par Dependency:
* @li hi_mdm_rst_times.h: 该接口声明所在的头文件。
* @since HiMDL_V200R001C00
*/
HI_U32 HI_MDM_GetRelativeRstTimes(HI_VOID);

/**
* @ingroup  hct_rst_info
* @brief 清空复位次数，调用该接口后，通过HI_MDM_GetRelativeRstTimes接口获取的复位次数清0。
*
* @par 描述:
* 清空复位次数，调用该接口后，通过HI_MDM_GetRelativeRstTimes接口获取的复位次数清0，调用HI_MDM_GetRstTimes接口获取的复位次数不会清0。
* @attention
* @li 阻塞接口，不支持在中断、关中断、锁任务上下文调用。
*
* @retval #HI_U32 复位次数。
*
* @par Dependency:
* @li hi_mdm_rst_times.h: 该接口声明所在的头文件。
* @since HiMDL_V200R001C00
*/
HI_U32 HI_MDM_ClrRelativeRstTimes(HI_VOID);

/**
* @ingroup  hct_rst_info
* @brief 使能(去使能)记录复位次数功能。
*
* @par 描述:
* 使能(去使能)记录复位次数功能。使能复位次数记录功能时，每次启动时会记录NV,正常计数复位次数。复位次数更新需要执行写Flash操作，通常在系统经常频繁上下电，不能接受上电写NV导致flash寿命快速耗尽的场景时，将bEnable设置为HI_FALSE，同时要考虑是否对业务功能造成影响。
*
* @attention
* @li 该接口需要在APP_InitCfgEarly流程中调用。
* @li 去使能后，HI_MDM_GetRstTimes和HI_MDM_GetRelativeRstTimes接口返回随机值。
* @li 关闭该功能时需要考虑对调用该接口业务的影响。
* @li 该功能默认使能。
*
* @param  bEnable [IN] HI_TRUE 使能。 HI_FALSE 关闭。
*
* @par Dependency:
* @li hi_mdm_rst_times.h: 该接口声明所在的头文件。
* @since HiMDL_V200R001C00
*/
HI_VOID HI_MDM_EnableRstTimeSave(HI_BOOL bEnable);

/**
* @ingroup  hct_rst_info
* @brief 判断是否为硬掉电。
*
* @par 描述:
* 判断死机原因是否为硬掉电导致。
*
* @param  无。
*
* @retval #HI_BOOL HI_TRUE表示硬掉电，HI_FALSE表示非硬掉电。
*
* @par Dependency:
* @li hi_mdm_rst_times.h: 该接口声明所在的头文件。
* @since HiMDL_V200R001C00
*/
HI_BOOL HI_MDM_IsPowerCut(HI_VOID);


#endif

