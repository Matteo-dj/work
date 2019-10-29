/**@defgroup hct_swtich  SWITCH
 * @ingroup drivers
 */

#ifndef __HI_MDM_SWITCH_H__
#define __HI_MDM_SWITCH_H__

/**
 * @ingroup hct_swtich
 * SWITCH编号
 */
typedef enum
{
    HI_SWITCH_IDX_0 = 0,/**< SWITCH0*/
    HI_SWITCH_IDX_1,    /**< SWITCH1*/
    HI_SWITCH_IDX_2,    /**< SWITCH2*/
    HI_SWITCH_IDX_MAX,
}HI_SWITCH_IDX_E;

/**
 * @ingroup hct_swtich
 * 配置SWITCH模式
 */
typedef enum
{
    HI_SWITCH_MODE_HIGH = 0,    /**< 上拉使用*/
    HI_SWITCH_MODE_LOW,         /**< 下拉使用*/
    HI_SWITCH_MODE_MAX,
}HI_SWITCH_MODE_E;

/**
* @ingroup  hct_swtich
* @brief  配置SWITCH模式。
*
* @par 描述:  
* 配置SWITCH模式。
* @attention 无。
*
* @param  enModeType [IN] 类型 #HI_SWITCH_MODE_E  0:低有效，上拉使用；1:高有效，下拉使用。
*
* @par Dependency:
* @li hi_mdm_switch.h: 该接口声明所在的头文件。
* @see  无。
* @since HiMDL_V200R001C00
*/
#define HI_MDM_SWITCH_ModeConfig(enModeType) HI_SWITCH_ModeConfig(enModeType)
    
/**
* @ingroup  hct_swtich
* @brief  将SWITCH置高。
*
* @par 描述:  
* 将SWITCH置高。
* @attention 无。
*
* @param  enId [IN] 类型 #HI_SWITCH_IDX_E SWITCH 索引。
*
* @retval #HI_ERR_SUCCESS 成功。
* @retval 其他值 失败，详见hi_errno.h。
*
* @par Dependency:
* @li hi_mdm_switch.h: 该接口声明所在的头文件。
* @see  无。
* @since HiMDL_V200R001C00
*/
#define HI_MDM_SWITCH_On(enId) HI_SWITCH_On(enId)

/**
* @ingroup  hct_swtich
* @brief  将SWITCH置低。
*
* @par 描述:  
* 将SWITCH置低。
* @attention 无。
*
* @param  enId [IN] 类型 #HI_SWITCH_IDX_E SWITCH 索引。
*
* @retval #HI_ERR_SUCCESS 成功。
* @retval 其他值 失败，详见hi_errno.h。
*
* @par Dependency:
* @li hi_mdm_switch.h: 该接口声明所在的头文件。
* @see  无。
* @since HiMDL_V200R001C00
*/
#define HI_MDM_SWITCH_Off(enId) HI_SWITCH_Off(enId)

/**
* @ingroup  hct_swtich
* @brief    Switch循环模式配置。
*
* @par 描述:
* Switch循环模式配置。
* @attention 无。
*
* @param  enId [IN] 类型 #HI_SWITCH_IDX_E switch编号。
* @param  ulOnTime [IN] 类型 #HI_U32 每个周期switch连接的时间，单位:ms。
* @param  ulOffTime [IN] 类型 #HI_U32 每个周期switch断开的时间，单位:ms。
*
* @retval #HI_ERR_SUCCESS 成功。
* @retval 其他值 失败，详见hi_errno.h。
*
* @par Dependency:
* @li hi_mdm_switch.h: 该接口声明所在的头文件。
* @see  无。
* @since HiMDL_V200R001C00
*/
#define HI_MDM_SWITCH_Loop(enId, ulOnTime, ulOffTime) HI_SWITCH_Loop(enId, ulOnTime, ulOffTime)


HI_EXTERN HI_VOID HI_SWITCH_ModeConfig(HI_SWITCH_MODE_E enModeType);
HI_EXTERN HI_U32 HI_SWITCH_On(HI_SWITCH_IDX_E enId);
HI_EXTERN HI_U32 HI_SWITCH_Off(HI_SWITCH_IDX_E enId);
HI_EXTERN HI_U32 HI_SWITCH_Loop(HI_SWITCH_IDX_E enId,HI_U32 ulOnTime,HI_U32 ulOffTime);

#endif
