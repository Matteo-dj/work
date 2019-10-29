/**@defgroup hct_tf ̨��ʶ��
 * @ingroup mdm
 */

#ifndef __HI_MDM_TF_H__
#define __HI_MDM_TF_H__
#include <hi_types.h>
#include <hi_mdm_types.h>

HI_START_HEADER

#define TF_NDM_MAX_REAL_NOISE   1000     // ʵʱ��������Ŷ�ֵ�ϱ����ޣ���λus


#if defined(PRODUCT_CFG_PRODUCT_TYPE_NDM)

/**
* @ingroup  hct_tf
* @brief  ������ϻ�ȡ��ǰ̨���ź����̨���š�
*
* @par ����:
* ������ϻ�ȡ��ǰ̨���ź����̨���š�
* @attention �ޡ�
*
* @param  usMaxRecordCount [out] ��ǰ̨���š�
* @param  usMaxRecordCount [out] ���̨���š�
*
* @retval #HI_ERR_SUCCESS �ɹ���
* @retval ����ֵ ʧ�ܣ����hi_errno.h��
*
* @par Dependency:
* @li hi_mdm_tf.h: �ýӿ��������ڵ�ͷ�ļ���
* @see  HI_MDM_TF_SetTransFormerNum ��
* @since HiMDL_V200R001C00
*/
HI_EXTERN HI_U32 HI_MDM_TF_GetTransFormerNum(HI_U8 *pucTransFormerNum,HI_U8 *pucSupportMaxTransFormerNum);

/**
* @ingroup  hct_tf
* @brief  �����������̨���š�
*
* @par ����:
* �����������̨���š�
* @attention �ޡ�
*
* @param  ucTransFormerNum [out] ̨���š�
*
* @retval #HI_ERR_SUCCESS �ɹ���
* @retval ����ֵ ʧ�ܣ����hi_errno.h��
*
* @par Dependency:
* @li hi_mdm_tf.h: �ýӿ��������ڵ�ͷ�ļ���
* @see  HI_MDM_TF_GetTransFormerNum ��
* @since HiMDL_V200R001C00
*/
HI_EXTERN HI_U32 HI_MDM_TF_SetTransFormerNum(HI_U8 ucTransFormerNum);

/**
* @ingroup  hct_tf
* @brief  �������ȡ̨��ʶ��ʹ��״̬��
*
* @par ����:
* �������ȡ̨��ʶ��ʹ��״̬��
* @attention �ޡ�
*
* @param  bTxModeSwitch [out] HI_TRUE:����HI_FALSE:�ء�
*
* @retval #HI_ERR_SUCCESS �ɹ���
* @retval ����ֵ ʧ�ܣ����hi_errno.h��
*
* @par Dependency:
* @li hi_mdm_tf.h: �ýӿ��������ڵ�ͷ�ļ���
* @see  HI_MDM_TF_SetTxModeSwitch ��
* @since HiMDL_V200R001C00
*/
HI_EXTERN HI_U32 HI_MDM_TF_GetTxModeSwitch(HI_BOOL *bTxModeSwitch);

/**
* @ingroup  hct_tf
* @brief  ������Ͽ�(��)̨��ʶ��
*
* @par ����:
* ������Ͽ�(��)̨��ʶ��
* @attention �ޡ�
*
* @param  bTxModeSwitch [IN] HI_TRUE:����HI_FALSE:�ء�
*
* @retval #HI_ERR_SUCCESS �ɹ���
* @retval ����ֵ ʧ�ܣ����hi_errno.h��
*
* @par Dependency:
* @li hi_mdm_tf.h: �ýӿ��������ڵ�ͷ�ļ���
* @see  HI_MDM_TF_GetTxModeSwitch ��
* @since HiMDL_V200R001C00
*/
HI_EXTERN HI_U32 HI_MDM_TF_SetTxModeSwitch(HI_BOOL bTxModeSwitch);

/**
* @ingroup  hct_tf
* @brief  ���ջ��ϻ�ȡ̨��ʶ�𿪹�״̬��
*
* @par ����:
* ���ջ��ϻ�ȡ̨��ʶ�𿪹�״̬��
* @attention �ޡ�
*
* @param  bRxModeSwitch [out] HI_TRUE:����HI_FALSE:�ء�
*
* @retval #HI_ERR_SUCCESS �ɹ���
* @retval ����ֵ ʧ�ܣ����hi_errno.h��
*
* @par Dependency:
* @li hi_mdm_tf.h: �ýӿ��������ڵ�ͷ�ļ���
* @see  HI_MDM_TF_SetRxModeSwitch ��
* @since HiMDL_V200R001C00
*/
HI_EXTERN HI_U32 HI_MDM_TF_GetRxModeSwitch(HI_BOOL *bRxModeSwitch);

/**
* @ingroup  hct_tf
* @brief  ���ջ��Ͽ���̨��ʶ��
*
* @par ����:
* ���ջ��Ͽ���̨��ʶ��
* @attention �ޡ�
*
* @param  bRxModeSwitch [IN] HI_TRUE:����HI_FALSE:�ء�
*
* @retval #HI_ERR_SUCCESS �ɹ���
* @retval ����ֵ ʧ�ܣ����hi_errno.h��
*
* @par Dependency:
* @li hi_mdm_tf.h: �ýӿ��������ڵ�ͷ�ļ���
* @see  HI_MDM_TF_GetRxModeSwitch ��
* @since HiMDL_V200R001C00
*/
HI_EXTERN HI_U32 HI_MDM_TF_SetRxModeSwitch(HI_BOOL bRxModeSwitch);

/**
* @ingroup  hct_tf
* @brief  ���ջ����л�̨��ʶ����ʾģʽ��
*
* @par ����:
* ���ջ����л�̨��ʶ����ʾģʽ��
* @attention �ޡ�
*
* @retval #HI_ERR_SUCCESS �ɹ���
* @retval ����ֵ ʧ�ܣ����hi_errno.h��
*
* @par Dependency:
* @li hi_mdm_tf.h: �ýӿ��������ڵ�ͷ�ļ���
* @see  �� ��
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
* @brief  ������ϻ�ȡ��ǰ̨���ź����̨���š�
*
* @par ����:
* ������ϻ�ȡ��ǰ̨���ź����̨���š�
* @attention �ޡ�
*
* @param  usMaxRecordCount [out] ��ǰ̨���š�
* @param  usMaxRecordCount [out] ���̨���š�
*
* @retval #HI_ERR_SUCCESS �ɹ���
* @retval ����ֵ ʧ�ܣ����hi_errno.h��
*
* @par Dependency:
* @li hi_mdm_tf.h: �ýӿ��������ڵ�ͷ�ļ���
* @see  HI_MDM_TF_SetTransFormerNum ��
* @since HiMDL_V200R001C00
*/
HI_PRVL HI_U32 HI_MDM_TF_GetTransFormerNum(HI_U8 *pucTransFormerNum,HI_U8 *pucSupportMaxTransFormerNum){return HI_ERR_NOT_SUPPORT;}
/**
* @ingroup  hct_tf
* @brief  �����������̨���š�
*
* @par ����:
* �����������̨���š�
* @attention �ޡ�
*
* @param  ucTransFormerNum [out] ̨���š�
*
* @retval #HI_ERR_SUCCESS �ɹ���
* @retval ����ֵ ʧ�ܣ����hi_errno.h��
*
* @par Dependency:
* @li hi_mdm_tf.h: �ýӿ��������ڵ�ͷ�ļ���
* @see  HI_MDM_TF_GetTransFormerNum ��
* @since HiMDL_V200R001C00
*/
HI_PRVL HI_U32 HI_MDM_TF_SetTransFormerNum(HI_U8 ucTransFormerNum){return HI_ERR_NOT_SUPPORT;}
/**
* @ingroup  hct_tf
* @brief  �������ȡ̨��ʶ��ʹ��״̬��
*
* @par ����:
* �������ȡ̨��ʶ��ʹ��״̬��
* @attention �ޡ�
*
* @param  bTxModeSwitch [out] HI_TRUE:����HI_FALSE:�ء�
*
* @retval #HI_ERR_SUCCESS �ɹ���
* @retval ����ֵ ʧ�ܣ����hi_errno.h��
*
* @par Dependency:
* @li hi_mdm_tf.h: �ýӿ��������ڵ�ͷ�ļ���
* @see  HI_MDM_TF_SetTxModeSwitch ��
* @since HiMDL_V200R001C00
*/
HI_PRVL HI_U32 HI_MDM_TF_GetTxModeSwitch(HI_BOOL *bTxModeSwitch){return HI_ERR_NOT_SUPPORT;}
/**
* @ingroup  hct_tf
* @brief  ������Ͽ�(��)̨��ʶ��
*
* @par ����:
* ������Ͽ�(��)̨��ʶ��
* @attention �ޡ�
*
* @param  bTxModeSwitch [IN] HI_TRUE:����HI_FALSE:�ء�
*
* @retval #HI_ERR_SUCCESS �ɹ���
* @retval ����ֵ ʧ�ܣ����hi_errno.h��
*
* @par Dependency:
* @li hi_mdm_tf.h: �ýӿ��������ڵ�ͷ�ļ���
* @see  HI_MDM_TF_GetTxModeSwitch ��
* @since HiMDL_V200R001C00
*/
HI_PRVL HI_U32 HI_MDM_TF_SetTxModeSwitch(HI_BOOL bTxModeSwitch){return HI_ERR_NOT_SUPPORT;}
/**
* @ingroup  hct_tf
* @brief  ���ջ��ϻ�ȡ̨��ʶ�𿪹�״̬��
*
* @par ����:
* ���ջ��ϻ�ȡ̨��ʶ�𿪹�״̬��
* @attention �ޡ�
*
* @param  bRxModeSwitch [out] HI_TRUE:����HI_FALSE:�ء�
*
* @retval #HI_ERR_SUCCESS �ɹ���
* @retval ����ֵ ʧ�ܣ����hi_errno.h��
*
* @par Dependency:
* @li hi_mdm_tf.h: �ýӿ��������ڵ�ͷ�ļ���
* @see  HI_MDM_TF_SetRxModeSwitch ��
* @since HiMDL_V200R001C00
*/
HI_PRVL HI_U32 HI_MDM_TF_GetRxModeSwitch(HI_BOOL *bRxModeSwitch){return HI_ERR_NOT_SUPPORT;}
/**
* @ingroup  hct_tf
* @brief  ���ջ��Ͽ���̨��ʶ��
*
* @par ����:
* ���ջ��Ͽ���̨��ʶ��
* @attention �ޡ�
*
* @param  bRxModeSwitch [IN] HI_TRUE:����HI_FALSE:�ء�
*
* @retval #HI_ERR_SUCCESS �ɹ���
* @retval ����ֵ ʧ�ܣ����hi_errno.h��
*
* @par Dependency:
* @li hi_mdm_tf.h: �ýӿ��������ڵ�ͷ�ļ���
* @see  HI_MDM_TF_GetRxModeSwitch ��
* @since HiMDL_V200R001C00
*/
HI_PRVL HI_U32 HI_MDM_TF_SetRxModeSwitch(HI_BOOL bRxModeSwitch){return HI_ERR_NOT_SUPPORT;}
/**
* @ingroup  hct_tf
* @brief  ���ջ����л�̨��ʶ����ʾģʽ��
*
* @par ����:
* ���ջ����л�̨��ʶ����ʾģʽ��
* @attention �ޡ�
*
* @retval #HI_ERR_SUCCESS �ɹ���
* @retval ����ֵ ʧ�ܣ����hi_errno.h��
*
* @par Dependency:
* @li hi_mdm_tf.h: �ýӿ��������ڵ�ͷ�ļ���
* @see  �� ��
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

