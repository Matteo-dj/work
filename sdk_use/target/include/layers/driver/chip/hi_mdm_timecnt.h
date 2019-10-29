/**@defgroup hct_timecnt  timecnt
 * @ingroup drivers
 */

#ifndef __HI_MDM_TIMECNT_H__
#define __HI_MDM_TIMECNT_H__

/**
* @ingroup  hct_timecnt
* @brief  获取系统运行时间(单位: s、ms、us)。
*
* @par 描述: 获取系统运行时间(单位: s、ms、us)。
* @attention 无。
* @param  pUs [OUT] 类型 #HI_U32 * 单位:us。
* @param  pMs [OUT] 类型 #HI_U32 * 单位:ms。
* @param  pSec [OUT] 类型 #HI_U32 * 单位:s。
* @retval #0      success.
* @retval #非0     failed.详见hi_errno.h
* @par Dependency:
* <ul><li>hi_mdm_timecnt.h: 文件用于描述time_cnt相关接口.</li></ul>
* @see  无。
* @since HiMDL_V200R001C00
*/
#define HI_MDM_TIMECNT_GetTime(pUs, pMs, pSec) HI_TIME_CNT_GetTime(pUs, pMs, pSec)

/**
* @ingroup  hct_timecnt
* @brief   获取系统运行时间(单位: s)。
*
* @par 描述:  获取系统运行时间(单位: s)。
* @attention 无。
* @param  pSec [OUT] 类型 #HI_U32 *  单位:s。
* @retval #0      success.
* @retval #非0     failed.详见hi_errno.h
* @par Dependency:
* <ul><li>hi_mdm_timecnt.h: 文件用于描述time_cnt相关接口.</li></ul>
* @see  无。
* @since HiMDL_V200R001C00
*/
#define HI_MDM_TIMECNT_GetSec(pSec) HI_TIME_CNT_GetSec(pSec)

/**
* @ingroup  hct_timecnt
* @brief  获取系统运行时间(单位: ms)。
*
* @par 描述:    获取系统运行时间(单位: ms)。
* @attention 无。
* @param  pMs [OUT] 类型 #HI_U32 * 单位:ms。
* @retval #0      success.
* @retval #非0     failed.详见hi_errno.h
* @par Dependency:
* <ul><li>hi_mdm_timecnt.h: 文件用于描述time_cnt相关接口.</li></ul>
* @see  无。
* @since HiMDL_V200R001C00
*/
#define HI_MDM_TIMECNT_GetMs(pMs) HI_TIME_CNT_GetMs(pMs)

/**
* @ingroup  hct_timecnt
* @brief     获取系统运行时间(单位: us)。
*
* @par 描述:   获取系统运行时间(单位: us)。
* @attention 无。
* @param  pUs [OUT] 类型 #HI_U32 * 单位:us。
* @retval #0      success.
* @retval #非0     failed.详见hi_errno.h
* @par Dependency:
* <ul><li>hi_mdm_timecnt.h: 文件用于描述time_cnt相关接口.</li></ul>
* @see  无。
* @since HiMDL_V200R001C00
*/
#define HI_MDM_TIMECNT_GetUs(pUs) HI_TIME_CNT_GetUs(pUs)
#endif
