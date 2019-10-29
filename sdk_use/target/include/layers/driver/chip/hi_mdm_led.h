/**@defgroup hct_led  LED
 * @ingroup drivers
 */

#ifndef __HI_MDM_LED_H__
#define __HI_MDM_LED_H__

/**
 * @ingroup hct_led
 * LED���
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
 * ����LEDģʽ
 */
typedef enum
{
	HI_LED_MODE_HIGH = 0,/**< ����LED��ʱ����ߵ�ƽ*/
	HI_LED_MODE_LOW = 1, /**< ����LED��ʱ����͵�ƽ*/
	HI_LED_MODE_MAX,
}HI_LED_MODE_E;

/**
* @ingroup  hct_led
* @brief  ����LEDģʽ��
*
* @par ����:
* ����LEDģʽ��
* @attention �ޡ�
*
* @param  enModeType [IN] LEDģʽ ȡֵ��#HI_LED_MODE_E ��
*
*
* @par Dependency:
* @li hi_mdm_led.h: �ļ���������IO��ؽӿڡ�
* @see  �ޡ�
* @since HiMDL_V200R001C00
*/
#define HI_MDM_LED_ModeConfig(enModeType) HI_LED_ModeConfig(enModeType)

/**
* @ingroup  hct_led
* @brief  ����LED��
*
* @par ����:
* ����LED��
* @attention �ޡ�
*
* @param  enId [IN] ���� #HI_LED_IDX_E LED ������
*
* @retval #HI_ERR_SUCCESS �ɹ���
* @retval ����ֵ ʧ�ܣ����hi_errno.h��
*
* @par Dependency:
* @li hi_mdm_led.h: �ļ���������IO��ؽӿڡ�
* @see  �ޡ�
* @since HiMDL_V200R001C00
*/
#define HI_MDM_LED_On(enId) HI_LED_On(enId)

/**
* @ingroup  hct_led
* @brief   Ϩ��LED��
*
* @par ����:
* Ϩ��LED
* @attention �ޡ�
*
* @param  enId [IN] ���� #HI_LED_IDX_E LED ������
*
* @retval #HI_ERR_SUCCESS �ɹ���
* @retval ����ֵ ʧ�ܣ����hi_errno.h��
*
* @par Dependency:
* @li hi_mdm_led.h: �ļ���������IO��ؽӿڡ�
* @see  �ޡ�
* @since HiMDL_V200R001C00
*/
#define HI_MDM_LED_Off(enId) HI_LED_Off(enId)

/**
* @ingroup  hct_led
* @brief    ѭ����˸LED��
*
* @par ����:
* ѭ����˸LED
* @attention �ޡ�
*
* @param  enId [IN] ���� #HI_LED_IDX_E LED ������
* @param  ulOnTime [IN] ���� #HI_U32 ������ʱ�䡣
* @param  ulOffTime [IN] ���� #HI_U32 �����ʱ�䡣
*
* @retval #HI_ERR_SUCCESS �ɹ���
* @retval ����ֵ ʧ�ܣ����hi_errno.h��
*
* @par Dependency:
* @li hi_mdm_led.h: �ļ���������IO��ؽӿڡ�
* @see  �ޡ�
* @since HiMDL_V200R001C00
*/
#define HI_MDM_LED_Glitter(enId, ulOnTime, ulOffTime) HI_LED_Glitter(enId, ulOnTime, ulOffTime)

HI_EXTERN HI_VOID HI_LED_ModeConfig(HI_LED_MODE_E enModeType);
HI_EXTERN HI_U32 HI_LED_On(HI_LED_IDX_E enLedId);
HI_EXTERN HI_U32 HI_LED_Off(HI_LED_IDX_E enLedId);
HI_EXTERN HI_U32 HI_LED_Glitter(HI_LED_IDX_E enLedId, HI_U32 ulOnTime, HI_U32 ulOffTime);
#endif
