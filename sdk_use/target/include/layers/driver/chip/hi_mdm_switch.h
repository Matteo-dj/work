/**@defgroup hct_swtich  SWITCH
 * @ingroup drivers
 */

#ifndef __HI_MDM_SWITCH_H__
#define __HI_MDM_SWITCH_H__

/**
 * @ingroup hct_swtich
 * SWITCH���
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
 * ����SWITCHģʽ
 */
typedef enum
{
    HI_SWITCH_MODE_HIGH = 0,    /**< ����ʹ��*/
    HI_SWITCH_MODE_LOW,         /**< ����ʹ��*/
    HI_SWITCH_MODE_MAX,
}HI_SWITCH_MODE_E;

/**
* @ingroup  hct_swtich
* @brief  ����SWITCHģʽ��
*
* @par ����:  
* ����SWITCHģʽ��
* @attention �ޡ�
*
* @param  enModeType [IN] ���� #HI_SWITCH_MODE_E  0:����Ч������ʹ�ã�1:����Ч������ʹ�á�
*
* @par Dependency:
* @li hi_mdm_switch.h: �ýӿ��������ڵ�ͷ�ļ���
* @see  �ޡ�
* @since HiMDL_V200R001C00
*/
#define HI_MDM_SWITCH_ModeConfig(enModeType) HI_SWITCH_ModeConfig(enModeType)
    
/**
* @ingroup  hct_swtich
* @brief  ��SWITCH�øߡ�
*
* @par ����:  
* ��SWITCH�øߡ�
* @attention �ޡ�
*
* @param  enId [IN] ���� #HI_SWITCH_IDX_E SWITCH ������
*
* @retval #HI_ERR_SUCCESS �ɹ���
* @retval ����ֵ ʧ�ܣ����hi_errno.h��
*
* @par Dependency:
* @li hi_mdm_switch.h: �ýӿ��������ڵ�ͷ�ļ���
* @see  �ޡ�
* @since HiMDL_V200R001C00
*/
#define HI_MDM_SWITCH_On(enId) HI_SWITCH_On(enId)

/**
* @ingroup  hct_swtich
* @brief  ��SWITCH�õ͡�
*
* @par ����:  
* ��SWITCH�õ͡�
* @attention �ޡ�
*
* @param  enId [IN] ���� #HI_SWITCH_IDX_E SWITCH ������
*
* @retval #HI_ERR_SUCCESS �ɹ���
* @retval ����ֵ ʧ�ܣ����hi_errno.h��
*
* @par Dependency:
* @li hi_mdm_switch.h: �ýӿ��������ڵ�ͷ�ļ���
* @see  �ޡ�
* @since HiMDL_V200R001C00
*/
#define HI_MDM_SWITCH_Off(enId) HI_SWITCH_Off(enId)

/**
* @ingroup  hct_swtich
* @brief    Switchѭ��ģʽ���á�
*
* @par ����:
* Switchѭ��ģʽ���á�
* @attention �ޡ�
*
* @param  enId [IN] ���� #HI_SWITCH_IDX_E switch��š�
* @param  ulOnTime [IN] ���� #HI_U32 ÿ������switch���ӵ�ʱ�䣬��λ:ms��
* @param  ulOffTime [IN] ���� #HI_U32 ÿ������switch�Ͽ���ʱ�䣬��λ:ms��
*
* @retval #HI_ERR_SUCCESS �ɹ���
* @retval ����ֵ ʧ�ܣ����hi_errno.h��
*
* @par Dependency:
* @li hi_mdm_switch.h: �ýӿ��������ڵ�ͷ�ļ���
* @see  �ޡ�
* @since HiMDL_V200R001C00
*/
#define HI_MDM_SWITCH_Loop(enId, ulOnTime, ulOffTime) HI_SWITCH_Loop(enId, ulOnTime, ulOffTime)


HI_EXTERN HI_VOID HI_SWITCH_ModeConfig(HI_SWITCH_MODE_E enModeType);
HI_EXTERN HI_U32 HI_SWITCH_On(HI_SWITCH_IDX_E enId);
HI_EXTERN HI_U32 HI_SWITCH_Off(HI_SWITCH_IDX_E enId);
HI_EXTERN HI_U32 HI_SWITCH_Loop(HI_SWITCH_IDX_E enId,HI_U32 ulOnTime,HI_U32 ulOffTime);

#endif
