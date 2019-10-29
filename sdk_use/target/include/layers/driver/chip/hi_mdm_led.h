/**@defgroup hct_led  LED
 * @ingroup drivers
 */

#ifndef __HI_MDM_LED_H__
#define __HI_MDM_LED_H__

/**
 * @ingroup hct_led
 * LED编号
 */
typedef enum
{
    HI_LED_IDX_0,/**< LED0*/
    HI_LED_IDX_1,/**< LED1*/
    HI_LED_IDX_2,/**< LED2*/
    HI_LED_IDX_3,/**< LED3*/
    HI_LED_IDX_4,/**< LED4*/
    HI_LED_IDX_MAX,
}HI_LED_IDX_E;

/**
 * @ingroup hct_led
 * 配置LED模式
 */
typedef enum
{
	HI_LED_MODE_HIGH = 0,/**< 配置LED亮时输出高电平*/
	HI_LED_MODE_LOW = 1, /**< 配置LED亮时输出低电平*/
	HI_LED_MODE_MAX,
}HI_LED_MODE_E;

/**
* @ingroup  hct_led
* @brief  配置LED模式。
*
* @par 描述:
* 配置LED模式。
* @attention 无。
*
* @param  enModeType [IN] LED模式 取值见#HI_LED_MODE_E 。
*
*
* @par Dependency:
* @li hi_mdm_led.h: 文件用于描述IO相关接口。
* @see  无。
* @since HiMDL_V200R001C00
*/
#define HI_MDM_LED_ModeConfig(enModeType) HI_LED_ModeConfig(enModeType)

/**
* @ingroup  hct_led
* @brief  点亮LED。
*
* @par 描述:
* 点亮LED。
* @attention 无。
*
* @param  enId [IN] 类型 #HI_LED_IDX_E LED 索引。
*
* @retval #HI_ERR_SUCCESS 成功。
* @retval 其他值 失败，详见hi_errno.h。
*
* @par Dependency:
* @li hi_mdm_led.h: 文件用于描述IO相关接口。
* @see  无。
* @since HiMDL_V200R001C00
*/
#define HI_MDM_LED_On(enId) HI_LED_On(enId)

/**
* @ingroup  hct_led
* @brief   熄灭LED。
*
* @par 描述:
* 熄灭LED
* @attention 无。
*
* @param  enId [IN] 类型 #HI_LED_IDX_E LED 索引。
*
* @retval #HI_ERR_SUCCESS 成功。
* @retval 其他值 失败，详见hi_errno.h。
*
* @par Dependency:
* @li hi_mdm_led.h: 文件用于描述IO相关接口。
* @see  无。
* @since HiMDL_V200R001C00
*/
#define HI_MDM_LED_Off(enId) HI_LED_Off(enId)

/**
* @ingroup  hct_led
* @brief    循环闪烁LED。
*
* @par 描述:
* 循环闪烁LED
* @attention 无。
*
* @param  enId [IN] 类型 #HI_LED_IDX_E LED 索引。
* @param  ulOnTime [IN] 类型 #HI_U32 亮持续时间。
* @param  ulOffTime [IN] 类型 #HI_U32 灭持续时间。
*
* @retval #HI_ERR_SUCCESS 成功。
* @retval 其他值 失败，详见hi_errno.h。
*
* @par Dependency:
* @li hi_mdm_led.h: 文件用于描述IO相关接口。
* @see  无。
* @since HiMDL_V200R001C00
*/
#define HI_MDM_LED_Glitter(enId, ulOnTime, ulOffTime) HI_LED_Glitter(enId, ulOnTime, ulOffTime)

HI_EXTERN HI_VOID HI_LED_ModeConfig(HI_LED_MODE_E enModeType);
HI_EXTERN HI_U32 HI_LED_On(HI_LED_IDX_E enLedId);
HI_EXTERN HI_U32 HI_LED_Off(HI_LED_IDX_E enLedId);
HI_EXTERN HI_U32 HI_LED_Glitter(HI_LED_IDX_E enLedId, HI_U32 ulOnTime, HI_U32 ulOffTime);
#endif
