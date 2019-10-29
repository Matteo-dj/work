/******************************************************************************
*             Copyright (C) 2018 - 2018, Hisilicon Tech. Co., Ltd.
*                           ALL RIGHTS RESERVED
* FileName: hi_low_power.h
* Description������������̬�͹���(��̬��˯��)�ӿڡ�
*
* History:
* Version                  Date         DefectNum           Description
* V200R001C00SPC030B010    2018-08-20   DTS2018082012361    ���Ӷ�̬�͹��Ľӿ�������
* V200R001C00SPC020B014    2018-08-29   DTS2018082906020    ���Ӳ���ģʽʱͶƱID��
******************************************************************************/

/**@defgroup hct_lowpower �͹���
 * @ingroup drivers
 */

#ifndef __HI_LOW_POWER_H__
#define __HI_LOW_POWER_H__
#include <hi_types.h>
#include <hi_mdm_types.h>

/**
 * @ingroup hct_lowpower
 * �Ƿ��������͹���ͶƱö��ID
 */
typedef enum
{
    HI_LOW_POWER_FORBID_ID_PHY,     /**< PHYģ��ͶƱID */
    HI_LOW_POWER_FORBID_ID_ETH,     /**< ETHģ��ͶƱID */
    HI_LOW_POWER_FORBID_ID_FLASH,   /**< FLASHģ��ͶƱID */
    HI_LOW_POWER_FORBID_ID_SPI,     /**< SPIģ��ͶƱID */
    HI_LOW_POWER_FORBID_ID_I2C,     /**< I2Cģ��ͶƱID */
    HI_LOW_POWER_FORBID_ID_DC,      /**< ����ģ��ͶƱID */
    HI_LOW_POWER_FORBID_ID_CIPHER,  /**< cipherģ��ͶƱID */
    HI_LOW_POWER_FORBID_ID_HASH,    /**< hashģ��ͶƱID */
    HI_LOW_POWER_FORBID_ID_EFUSE,   /**< efuseģ��ͶƱID */
    HI_LOW_POWER_FORBID_ID_RSA,     /**< rsaģ��ͶƱID */
    HI_LOW_POWER_FORBID_ID_TRNG,    /**< �����ģ��ͶƱID */
    HI_LOW_POWER_FORBID_ID_IR,      /**< ����ģ��ͶƱID */
    HI_LOW_POWER_FORBID_ID_TEST_MODE,/**< ����ģʽʱͶƱID */
    HI_LOW_POWER_FORBID_ID_LIMIT=32,/**< ID���� */
}HI_LOW_POWER_FORBID_ID_E;

/**
 * @ingroup hct_lowpower
 * �Ƿ��������͹��Ļص���ѯö��ID
 */
typedef enum
{
    HI_LOW_POWER_CHECK_ID_UART,/**< ����ģ��ص���ѯID */
    HI_LOW_POWER_CHECK_ID_HRTIEMR,/**< �߾��ȶ�ʱ��ģ��ص���ѯID  */
    HI_LOW_POWER_CHECK_ID_LIMIT=4,/**< ID���� */
}HI_LOW_POWER_CHECK_ID_E;


/**
* @ingroup  hct_lowpower
* @brief   �Ƿ��������͹��Ĳ�ѯ���Ӻ�����
*
* @par  ����:
* �Ƿ��������͹��Ĳ�ѯ���Ӻ�����ϵͳ����ʱ���øù��Ӻ�����ȡ�û��Ƿ��������͹���״̬��
* @attention �ޡ�
*
* @param �ޡ�
*
* @retval #HI_TRUE �������͹��ġ�
* @retval #HI_FALSE ���������͹��ġ�
*
* @par Dependency:
* @li hi_low_power.h: �ýӿ��������ڵ�ͷ�ļ���
* @li hi_errno.h: �ļ����ô����롣
* @since HiMDL_V200R001C00
*/
typedef HI_BOOL (*LOW_POWER_CHECK_FUNC)(HI_VOID);

/**
* @ingroup  hct_lowpower
* @brief   ͶƱ���������͹��ġ�
*
* @par  ����:
* ͶƱ���������͹��ġ��͹��Ĳ��Բ���һƱ����ƣ��κ�ģ�鲻�������͹��ģ��򲻽���͹��ġ�
* @attention �ޡ�
*
* @param  enId [IN] ͶƱģ��ID��
*
* @retval #HI_ERR_SUCCESS ͶƱ�ɹ���
* @retval #HI_ERR_LOW_POWER_INVALID_PARAM ��ηǷ���
*
* @par Dependency:
* @li hi_low_power.h: �ýӿ��������ڵ�ͷ�ļ���
* @li hi_errno.h: �ļ����ô����롣
* @since HiMDL_V200R001C00
*/
HI_U32 HI_LOW_POWER_Forbid(HI_LOW_POWER_FORBID_ID_E enId);

/**
* @ingroup  hct_lowpower
* @brief   ͶƱ�������͹��ģ���֧��������Ч��ʱʱ�䡣
*
* @par  ����:
* ͶƱ�������͹��ģ���֧��������Ч��ʱʱ�䡣
* @attention 
* @li ���ģ��������ʱʱ����ʱȡ���ֵ�������HI_LOW_POWER_UnForbid(1,100)�ٵ���HI_LOW_POWER_UnForbid(2,50),��ʱʱ��Ϊ100tick��
* @li ��ʱʱ��ֻ��䳤�����С�������HI_LOW_POWER_UnForbid(1,100)�ٵ���HI_LOW_POWER_UnForbid(1,10),��ʱʱ�䲻���Ϊ10��ȻΪ100tick��
*
* @param  enId [IN] ͶƱģ��ID��
* @param  usAllowLowPowerTickFromNow [IN] ����͹�����ʱʱ��:�ӵ�ǰʱ�俪ʼ���ٸ�tick(10ms)�ڲ��������͹��ġ�
*
* @retval #HI_ERR_SUCCESS ͶƱ�ɹ���
* @retval #HI_ERR_LOW_POWER_INVALID_PARAM ��ηǷ���
*
* @par Dependency:
* @li hi_low_power.h: �ýӿ��������ڵ�ͷ�ļ���
* @li hi_errno.h: �ļ����ô����롣
* @since HiMDL_V200R001C00
*/
HI_U32 HI_LOW_POWER_UnForbid(HI_LOW_POWER_FORBID_ID_E enId,HI_U16 usAllowLowPowerTickFromNow);

/**
* @ingroup  hct_lowpower
* @brief   �û�ע��͹��Ļص����Ӻ�����
*
* @par  ����:
* �û�ע��͹��Ļص����Ӻ�������ϵͳ����ʱ�����û����Ӻ����������û����ؽ�������Ƿ����͹��ġ�
* @attention 
* @li �ص�������ͶƱ����ͬ��ΪһƱ��������κ��û���������HI_FALSEʱ�����������͹������̡�
* @li �ص������Ƕ�ͶƱ���Ƶ�һ�ֹ��ܲ��䡣���û��Ƿ��������͹��ĵ�״̬ʵʱ�仯���û��޷�ʵʱ���¸�״̬ʱ(�綨ʱ�����뵽��ʱ��С��n΢��Ϊ�ж�����)�����Ե��øýӿڡ���ϵͳ����ִ�е͹����ж�ʱ�������û����������жϡ�
* @li �ص�����Ч��Զ����ͶƱ���ƣ����Ӧ������ʹ��ͶƱ���ơ�
*
* @param  enId [IN] ͶƱģ��ID��
*
* @retval #HI_ERR_SUCCESS ע��ɹ���
* @retval #HI_ERR_LOW_POWER_INVALID_PARAM ��ηǷ���
*
* @par Dependency:
* @li hi_low_power.h: �ýӿ��������ڵ�ͷ�ļ���
* @li hi_errno.h: �ļ����ô����롣
* @since HiMDL_V200R001C00
*/
HI_U32 HI_LOW_POWER_RegisterCheckFunc(HI_LOW_POWER_CHECK_ID_E enId,LOW_POWER_CHECK_FUNC pfnCheckFunc);

/**
* @ingroup  hct_lowpower
* @brief   ʹ��(ȥʹ��)�͹��Ļ����ж�Դ��
*
* @par  ����:
* ���õ͹��Ļ����ж�Դ,������͹���ʱֻ��ʹ�ܵ��ж�Դ���ܻ���ϵͳ��
* @attention ��ʼ���������жϻ��ѡ�
*
* @param  ulLisrId [IN] �жϺš�
* @param  bWakeUp [IN] HI_FALSE:����͹��ĺ�ulLisrId�ŵ��жϲ�������ϵͳ��HI_TRUE:����͹��ĺ�ulLisrId�ŵ��ж�������ϵͳ��
*
* @retval #HI_ERR_SUCCESS ʹ��(ȥʹ��)�ɹ���
* @retval #HI_ERR_LOW_POWER_INVALID_PARAM ��ηǷ���
*
* @par Dependency:
* @li hi_low_power.h: �ýӿ��������ڵ�ͷ�ļ���
* @li hi_errno.h: �ļ����ô����롣
* @since HiMDL_V200R001C00
*/
HI_U32 HI_LOW_POWER_SetWakeUpLisr(HI_U32 ulLisrId,HI_BOOL bWakeUp);

/**
* @ingroup  hct_lowpower
* @brief   ��ʼ���͹��Ĳ��ԡ�
*
* @par  ����:
* ��ʼ���͹��Ĳ��ԡ�
* @attention �ޡ�
*
* @param  bInitEnable [IN] ��ʼ���͹��Ŀ��ء�HI_TRUE:ʹ�ܶ�̬�͹��Ĺ��ܡ�HI_FALSE:��ʹ�ܶ�̬�͹��Ĺ��ܡ�
*
* @retval #HI_ERR_SUCCESS ��ʼ���ɹ�
* @retval ����ֵ ʧ�ܣ����hi_errno.h��
*
* @par Dependency:
* @li hi_low_power.h: �ýӿ��������ڵ�ͷ�ļ���
* @li hi_errno.h: �ļ����ô����롣
* @since HiMDL_V200R001C00
*/
HI_U32 HI_LOW_POWER_SleepInit(HI_BOOL bInitEnable);

#endif

