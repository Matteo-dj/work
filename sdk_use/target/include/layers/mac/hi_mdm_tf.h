/**@defgroup hct_tf 台区识别
 * @ingroup mdm
 */

#ifndef __HI_MDM_TF_H__
#define __HI_MDM_TF_H__
#include <hi_types.h>
#include <hi_mdm_types.h>

HI_START_HEADER

#define TF_NDM_MAX_REAL_NOISE   1000     // 实时最大噪声扰动值上报门限，单位us


#if defined(PRODUCT_CFG_PRODUCT_TYPE_NDM)

/**
* @ingroup  hct_tf
* @brief  发射机上获取当前台区号和最大台区号。
*
* @par 描述:
* 发射机上获取当前台区号和最大台区号。
* @attention 无。
*
* @param  usMaxRecordCount [out] 当前台区号。
* @param  usMaxRecordCount [out] 最大台区号。
*
* @retval #HI_ERR_SUCCESS 成功。
* @retval 其他值 失败，详见hi_errno.h。
*
* @par Dependency:
* @li hi_mdm_tf.h: 该接口声明所在的头文件。
* @see  HI_MDM_TF_SetTransFormerNum 。
* @since HiMDL_V200R001C00
*/
HI_EXTERN HI_U32 HI_MDM_TF_GetTransFormerNum(HI_U8 *pucTransFormerNum,HI_U8 *pucSupportMaxTransFormerNum);

/**
* @ingroup  hct_tf
* @brief  发射机上设置台区号。
*
* @par 描述:
* 发射机上设置台区号。
* @attention 无。
*
* @param  ucTransFormerNum [out] 台区号。
*
* @retval #HI_ERR_SUCCESS 成功。
* @retval 其他值 失败，详见hi_errno.h。
*
* @par Dependency:
* @li hi_mdm_tf.h: 该接口声明所在的头文件。
* @see  HI_MDM_TF_GetTransFormerNum 。
* @since HiMDL_V200R001C00
*/
HI_EXTERN HI_U32 HI_MDM_TF_SetTransFormerNum(HI_U8 ucTransFormerNum);

/**
* @ingroup  hct_tf
* @brief  发射机获取台区识别使能状态。
*
* @par 描述:
* 发射机获取台区识别使能状态。
* @attention 无。
*
* @param  bTxModeSwitch [out] HI_TRUE:开，HI_FALSE:关。
*
* @retval #HI_ERR_SUCCESS 成功。
* @retval 其他值 失败，详见hi_errno.h。
*
* @par Dependency:
* @li hi_mdm_tf.h: 该接口声明所在的头文件。
* @see  HI_MDM_TF_SetTxModeSwitch 。
* @since HiMDL_V200R001C00
*/
HI_EXTERN HI_U32 HI_MDM_TF_GetTxModeSwitch(HI_BOOL *bTxModeSwitch);

/**
* @ingroup  hct_tf
* @brief  发射机上开(关)台区识别。
*
* @par 描述:
* 发射机上开(关)台区识别。
* @attention 无。
*
* @param  bTxModeSwitch [IN] HI_TRUE:开，HI_FALSE:关。
*
* @retval #HI_ERR_SUCCESS 成功。
* @retval 其他值 失败，详见hi_errno.h。
*
* @par Dependency:
* @li hi_mdm_tf.h: 该接口声明所在的头文件。
* @see  HI_MDM_TF_GetTxModeSwitch 。
* @since HiMDL_V200R001C00
*/
HI_EXTERN HI_U32 HI_MDM_TF_SetTxModeSwitch(HI_BOOL bTxModeSwitch);

/**
* @ingroup  hct_tf
* @brief  接收机上获取台区识别开关状态。
*
* @par 描述:
* 接收机上获取台区识别开关状态。
* @attention 无。
*
* @param  bRxModeSwitch [out] HI_TRUE:开，HI_FALSE:关。
*
* @retval #HI_ERR_SUCCESS 成功。
* @retval 其他值 失败，详见hi_errno.h。
*
* @par Dependency:
* @li hi_mdm_tf.h: 该接口声明所在的头文件。
* @see  HI_MDM_TF_SetRxModeSwitch 。
* @since HiMDL_V200R001C00
*/
HI_EXTERN HI_U32 HI_MDM_TF_GetRxModeSwitch(HI_BOOL *bRxModeSwitch);

/**
* @ingroup  hct_tf
* @brief  接收机上开关台区识别。
*
* @par 描述:
* 接收机上开关台区识别。
* @attention 无。
*
* @param  bRxModeSwitch [IN] HI_TRUE:开，HI_FALSE:关。
*
* @retval #HI_ERR_SUCCESS 成功。
* @retval 其他值 失败，详见hi_errno.h。
*
* @par Dependency:
* @li hi_mdm_tf.h: 该接口声明所在的头文件。
* @see  HI_MDM_TF_GetRxModeSwitch 。
* @since HiMDL_V200R001C00
*/
HI_EXTERN HI_U32 HI_MDM_TF_SetRxModeSwitch(HI_BOOL bRxModeSwitch);

/**
* @ingroup  hct_tf
* @brief  接收机上切换台区识别显示模式。
*
* @par 描述:
* 接收机上切换台区识别显示模式。
* @attention 无。
*
* @retval #HI_ERR_SUCCESS 成功。
* @retval 其他值 失败，详见hi_errno.h。
*
* @par Dependency:
* @li hi_mdm_tf.h: 该接口声明所在的头文件。
* @see  无 。
* @since HiMDL_V200R001C00
*/
HI_EXTERN HI_U32 HI_MDM_TF_SwitchStrengthDisplay(HI_VOID);
HI_EXTERN HI_U32 HI_BOARD_TF_RxNumDisPlay(HI_IN HI_U8 transformer_index);
HI_EXTERN HI_VOID HI_BOARD_TF_DisPlayInit(HI_VOID);
HI_EXTERN HI_U32 HI_BOARD_TF_ErrDisPlay(HI_U8 err_code);

HI_EXTERN HI_U32 HI_BOARD_TF_ModeDisPlay(HI_IN HI_U8 transformer_mode);
HI_EXTERN HI_U32 HI_BOARD_TF_DisablePhaseDisPlay(HI_VOID);

HI_EXTERN HI_U32 HI_BOARD_TF_TxPhaseDisPlay(HI_IN HI_U8 phase_index);
HI_EXTERN HI_U32 HI_BOARD_TF_TxNumDisPlay(HI_IN HI_U8 transformer_index);
HI_EXTERN HI_U32 HI_BOARD_TF_RxPhaseDisPlay(HI_IN HI_U8 phase_index);
HI_EXTERN HI_U32 HI_BOARD_TF_LevelDisPlay(HI_IN HI_U8 gear);
HI_EXTERN HI_U32 HI_BOARD_TF_RxNokDisPlay(HI_VOID);

#else
HI_PRVL HI_U32 HI_BOARD_TF_RxNumDisPlay(HI_IN HI_U8 transformer_index){return HI_ERR_NOT_SUPPORT;}
/**
* @ingroup  hct_tf
* @brief  发射机上获取当前台区号和最大台区号。
*
* @par 描述:
* 发射机上获取当前台区号和最大台区号。
* @attention 无。
*
* @param  usMaxRecordCount [out] 当前台区号。
* @param  usMaxRecordCount [out] 最大台区号。
*
* @retval #HI_ERR_SUCCESS 成功。
* @retval 其他值 失败，详见hi_errno.h。
*
* @par Dependency:
* @li hi_mdm_tf.h: 该接口声明所在的头文件。
* @see  HI_MDM_TF_SetTransFormerNum 。
* @since HiMDL_V200R001C00
*/
HI_PRVL HI_U32 HI_MDM_TF_GetTransFormerNum(HI_U8 *pucTransFormerNum,HI_U8 *pucSupportMaxTransFormerNum){return HI_ERR_NOT_SUPPORT;}
/**
* @ingroup  hct_tf
* @brief  发射机上设置台区号。
*
* @par 描述:
* 发射机上设置台区号。
* @attention 无。
*
* @param  ucTransFormerNum [out] 台区号。
*
* @retval #HI_ERR_SUCCESS 成功。
* @retval 其他值 失败，详见hi_errno.h。
*
* @par Dependency:
* @li hi_mdm_tf.h: 该接口声明所在的头文件。
* @see  HI_MDM_TF_GetTransFormerNum 。
* @since HiMDL_V200R001C00
*/
HI_PRVL HI_U32 HI_MDM_TF_SetTransFormerNum(HI_U8 ucTransFormerNum){return HI_ERR_NOT_SUPPORT;}
/**
* @ingroup  hct_tf
* @brief  发射机获取台区识别使能状态。
*
* @par 描述:
* 发射机获取台区识别使能状态。
* @attention 无。
*
* @param  bTxModeSwitch [out] HI_TRUE:开，HI_FALSE:关。
*
* @retval #HI_ERR_SUCCESS 成功。
* @retval 其他值 失败，详见hi_errno.h。
*
* @par Dependency:
* @li hi_mdm_tf.h: 该接口声明所在的头文件。
* @see  HI_MDM_TF_SetTxModeSwitch 。
* @since HiMDL_V200R001C00
*/
HI_PRVL HI_U32 HI_MDM_TF_GetTxModeSwitch(HI_BOOL *bTxModeSwitch){return HI_ERR_NOT_SUPPORT;}
/**
* @ingroup  hct_tf
* @brief  发射机上开(关)台区识别。
*
* @par 描述:
* 发射机上开(关)台区识别。
* @attention 无。
*
* @param  bTxModeSwitch [IN] HI_TRUE:开，HI_FALSE:关。
*
* @retval #HI_ERR_SUCCESS 成功。
* @retval 其他值 失败，详见hi_errno.h。
*
* @par Dependency:
* @li hi_mdm_tf.h: 该接口声明所在的头文件。
* @see  HI_MDM_TF_GetTxModeSwitch 。
* @since HiMDL_V200R001C00
*/
HI_PRVL HI_U32 HI_MDM_TF_SetTxModeSwitch(HI_BOOL bTxModeSwitch){return HI_ERR_NOT_SUPPORT;}
/**
* @ingroup  hct_tf
* @brief  接收机上获取台区识别开关状态。
*
* @par 描述:
* 接收机上获取台区识别开关状态。
* @attention 无。
*
* @param  bRxModeSwitch [out] HI_TRUE:开，HI_FALSE:关。
*
* @retval #HI_ERR_SUCCESS 成功。
* @retval 其他值 失败，详见hi_errno.h。
*
* @par Dependency:
* @li hi_mdm_tf.h: 该接口声明所在的头文件。
* @see  HI_MDM_TF_SetRxModeSwitch 。
* @since HiMDL_V200R001C00
*/
HI_PRVL HI_U32 HI_MDM_TF_GetRxModeSwitch(HI_BOOL *bRxModeSwitch){return HI_ERR_NOT_SUPPORT;}
/**
* @ingroup  hct_tf
* @brief  接收机上开关台区识别。
*
* @par 描述:
* 接收机上开关台区识别。
* @attention 无。
*
* @param  bRxModeSwitch [IN] HI_TRUE:开，HI_FALSE:关。
*
* @retval #HI_ERR_SUCCESS 成功。
* @retval 其他值 失败，详见hi_errno.h。
*
* @par Dependency:
* @li hi_mdm_tf.h: 该接口声明所在的头文件。
* @see  HI_MDM_TF_GetRxModeSwitch 。
* @since HiMDL_V200R001C00
*/
HI_PRVL HI_U32 HI_MDM_TF_SetRxModeSwitch(HI_BOOL bRxModeSwitch){return HI_ERR_NOT_SUPPORT;}
/**
* @ingroup  hct_tf
* @brief  接收机上切换台区识别显示模式。
*
* @par 描述:
* 接收机上切换台区识别显示模式。
* @attention 无。
*
* @retval #HI_ERR_SUCCESS 成功。
* @retval 其他值 失败，详见hi_errno.h。
*
* @par Dependency:
* @li hi_mdm_tf.h: 该接口声明所在的头文件。
* @see  无 。
* @since HiMDL_V200R001C00
*/
HI_PRVL HI_U32 HI_MDM_TF_SwitchStrengthDisplay(HI_VOID){return HI_ERR_NOT_SUPPORT;}
HI_PRVL HI_VOID HI_BOARD_TF_DisPlayInit(HI_VOID){return;}
HI_PRVL HI_U32 HI_BOARD_TF_ErrDisPlay(HI_U8 err_code){return HI_ERR_NOT_SUPPORT;}
HI_PRVL HI_U32 HI_BOARD_TF_ModeDisPlay(HI_IN HI_U8 transformer_mode){return HI_ERR_NOT_SUPPORT;}
HI_PRVL HI_U32 HI_BOARD_TF_DisablePhaseDisPlay(HI_VOID){return HI_ERR_NOT_SUPPORT;}
HI_PRVL HI_U32 HI_BOARD_TF_TxPhaseDisPlay(HI_IN HI_U8 phase_index){return HI_ERR_NOT_SUPPORT;}
HI_PRVL HI_U32 HI_BOARD_TF_TxNumDisPlay(HI_IN HI_U8 transformer_index){return HI_ERR_NOT_SUPPORT;}
HI_PRVL HI_U32 HI_BOARD_TF_RxPhaseDisPlay(HI_IN HI_U8 phase_index){return HI_ERR_NOT_SUPPORT;}
HI_PRVL HI_U32 HI_BOARD_TF_LevelDisPlay(HI_IN HI_U8 gear){return HI_ERR_NOT_SUPPORT;}
HI_PRVL HI_U32 HI_BOARD_TF_RxNokDisPlay(HI_VOID){return HI_ERR_NOT_SUPPORT;}

#endif

HI_END_HEADER
#endif

