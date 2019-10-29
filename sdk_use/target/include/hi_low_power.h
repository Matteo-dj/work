/******************************************************************************
*             Copyright (C) 2018 - 2018, Hisilicon Tech. Co., Ltd.
*                           ALL RIGHTS RESERVED
* FileName: hi_low_power.h
* Description：用于描述动态低功耗(动态醒睡比)接口。
*
* History:
* Version                  Date         DefectNum           Description
* V200R001C00SPC030B010    2018-08-20   DTS2018082012361    增加动态低功耗接口描述。
* V200R001C00SPC020B014    2018-08-29   DTS2018082906020    增加测试模式时投票ID。
******************************************************************************/

/**@defgroup hct_lowpower 低功耗
 * @ingroup drivers
 */

#ifndef __HI_LOW_POWER_H__
#define __HI_LOW_POWER_H__
#include <hi_types.h>
#include <hi_mdm_types.h>

/**
 * @ingroup hct_lowpower
 * 是否允许进入低功耗投票枚举ID
 */
typedef enum
{
    HI_LOW_POWER_FORBID_ID_PHY,     /**< PHY模块投票ID */
    HI_LOW_POWER_FORBID_ID_ETH,     /**< ETH模块投票ID */
    HI_LOW_POWER_FORBID_ID_FLASH,   /**< FLASH模块投票ID */
    HI_LOW_POWER_FORBID_ID_SPI,     /**< SPI模块投票ID */
    HI_LOW_POWER_FORBID_ID_I2C,     /**< I2C模块投票ID */
    HI_LOW_POWER_FORBID_ID_DC,      /**< 采数模块投票ID */
    HI_LOW_POWER_FORBID_ID_CIPHER,  /**< cipher模块投票ID */
    HI_LOW_POWER_FORBID_ID_HASH,    /**< hash模块投票ID */
    HI_LOW_POWER_FORBID_ID_EFUSE,   /**< efuse模块投票ID */
    HI_LOW_POWER_FORBID_ID_RSA,     /**< rsa模块投票ID */
    HI_LOW_POWER_FORBID_ID_TRNG,    /**< 随机数模块投票ID */
    HI_LOW_POWER_FORBID_ID_IR,      /**< 红外模块投票ID */
    HI_LOW_POWER_FORBID_ID_TEST_MODE,/**< 测试模式时投票ID */
    HI_LOW_POWER_FORBID_ID_LIMIT=32,/**< ID上限 */
}HI_LOW_POWER_FORBID_ID_E;

/**
 * @ingroup hct_lowpower
 * 是否允许进入低功耗回调查询枚举ID
 */
typedef enum
{
    HI_LOW_POWER_CHECK_ID_UART,/**< 串口模块回调查询ID */
    HI_LOW_POWER_CHECK_ID_HRTIEMR,/**< 高精度定时器模块回调查询ID  */
    HI_LOW_POWER_CHECK_ID_LIMIT=4,/**< ID上限 */
}HI_LOW_POWER_CHECK_ID_E;


/**
* @ingroup  hct_lowpower
* @brief   是否允许进入低功耗查询钩子函数。
*
* @par  描述:
* 是否允许进入低功耗查询钩子函数。系统空闲时调用该钩子函数获取用户是否允许进入低功耗状态。
* @attention 无。
*
* @param 无。
*
* @retval #HI_TRUE 允许进入低功耗。
* @retval #HI_FALSE 不允许进入低功耗。
*
* @par Dependency:
* @li hi_low_power.h: 该接口声明所在的头文件。
* @li hi_errno.h: 文件配置错误码。
* @since HiMDL_V200R001C00
*/
typedef HI_BOOL (*LOW_POWER_CHECK_FUNC)(HI_VOID);

/**
* @ingroup  hct_lowpower
* @brief   投票不允许进入低功耗。
*
* @par  描述:
* 投票不允许进入低功耗。低功耗策略采用一票否决制，任何模块不允许进入低功耗，则不进入低功耗。
* @attention 无。
*
* @param  enId [IN] 投票模块ID。
*
* @retval #HI_ERR_SUCCESS 投票成功。
* @retval #HI_ERR_LOW_POWER_INVALID_PARAM 入参非法。
*
* @par Dependency:
* @li hi_low_power.h: 该接口声明所在的头文件。
* @li hi_errno.h: 文件配置错误码。
* @since HiMDL_V200R001C00
*/
HI_U32 HI_LOW_POWER_Forbid(HI_LOW_POWER_FORBID_ID_E enId);

/**
* @ingroup  hct_lowpower
* @brief   投票允许进入低功耗，并支持配置生效延时时间。
*
* @par  描述:
* 投票允许进入低功耗，并支持配置生效延时时间。
* @attention 
* @li 多个模块设置延时时，延时取最大值。如调用HI_LOW_POWER_UnForbid(1,100)再调用HI_LOW_POWER_UnForbid(2,50),延时时间为100tick。
* @li 延时时间只会变长不会减小。如调用HI_LOW_POWER_UnForbid(1,100)再调用HI_LOW_POWER_UnForbid(1,10),延时时间不会变为10依然为100tick。
*
* @param  enId [IN] 投票模块ID。
* @param  usAllowLowPowerTickFromNow [IN] 进入低功耗延时时间:从当前时间开始多少个tick(10ms)内不允许进入低功耗。
*
* @retval #HI_ERR_SUCCESS 投票成功。
* @retval #HI_ERR_LOW_POWER_INVALID_PARAM 入参非法。
*
* @par Dependency:
* @li hi_low_power.h: 该接口声明所在的头文件。
* @li hi_errno.h: 文件配置错误码。
* @since HiMDL_V200R001C00
*/
HI_U32 HI_LOW_POWER_UnForbid(HI_LOW_POWER_FORBID_ID_E enId,HI_U16 usAllowLowPowerTickFromNow);

/**
* @ingroup  hct_lowpower
* @brief   用户注册低功耗回调钩子函数。
*
* @par  描述:
* 用户注册低功耗回调钩子函数。当系统空闲时调用用户钩子函数，根据用户返回结果决定是否进入低功耗。
* @attention 
* @li 回调机制与投票机制同样为一票否决，当任何用户函数返回HI_FALSE时，都不会进入低功耗流程。
* @li 回调机制是对投票机制的一种功能补充。当用户是否允许进入低功耗的状态实时变化且用户无法实时更新该状态时(如定时器距离到期时间小于n微妙为判断条件)，可以调用该接口。当系统空闲执行低功耗判断时，调用用户函数进行判断。
* @li 回调机制效率远低于投票机制，因此应尽可能使用投票机制。
*
* @param  enId [IN] 投票模块ID。
*
* @retval #HI_ERR_SUCCESS 注册成功。
* @retval #HI_ERR_LOW_POWER_INVALID_PARAM 入参非法。
*
* @par Dependency:
* @li hi_low_power.h: 该接口声明所在的头文件。
* @li hi_errno.h: 文件配置错误码。
* @since HiMDL_V200R001C00
*/
HI_U32 HI_LOW_POWER_RegisterCheckFunc(HI_LOW_POWER_CHECK_ID_E enId,LOW_POWER_CHECK_FUNC pfnCheckFunc);

/**
* @ingroup  hct_lowpower
* @brief   使能(去使能)低功耗唤醒中断源。
*
* @par  描述:
* 设置低功耗唤醒中断源,当进入低功耗时只有使能的中断源才能唤醒系统。
* @attention 初始允许所有中断唤醒。
*
* @param  ulLisrId [IN] 中断号。
* @param  bWakeUp [IN] HI_FALSE:进入低功耗后ulLisrId号的中断不允许唤醒系统。HI_TRUE:进入低功耗后ulLisrId号的中断允许唤醒系统。
*
* @retval #HI_ERR_SUCCESS 使能(去使能)成功。
* @retval #HI_ERR_LOW_POWER_INVALID_PARAM 入参非法。
*
* @par Dependency:
* @li hi_low_power.h: 该接口声明所在的头文件。
* @li hi_errno.h: 文件配置错误码。
* @since HiMDL_V200R001C00
*/
HI_U32 HI_LOW_POWER_SetWakeUpLisr(HI_U32 ulLisrId,HI_BOOL bWakeUp);

/**
* @ingroup  hct_lowpower
* @brief   初始化低功耗策略。
*
* @par  描述:
* 初始化低功耗策略。
* @attention 无。
*
* @param  bInitEnable [IN] 初始化低功耗开关。HI_TRUE:使能动态低功耗功能。HI_FALSE:不使能动态低功耗功能。
*
* @retval #HI_ERR_SUCCESS 初始化成功
* @retval 其他值 失败，详见hi_errno.h。
*
* @par Dependency:
* @li hi_low_power.h: 该接口声明所在的头文件。
* @li hi_errno.h: 文件配置错误码。
* @since HiMDL_V200R001C00
*/
HI_U32 HI_LOW_POWER_SleepInit(HI_BOOL bInitEnable);

#endif

