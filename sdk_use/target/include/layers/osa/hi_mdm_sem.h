/**@defgroup hct_sem �ź���
 * @ingroup osa
 */

#ifndef __HI_MDM_SEM_H__
#define __HI_MDM_SEM_H__
#include <hi_types.h>
#include <hi_mdm_types.h>

/** 
* @ingroup  hct_sem
* �ź�����ֵ
* 
* ����#HI_MDM_SEM_BCreate���ucInit��ȡֵ����ʾ�������ź����ĳ�ʼֵΪ1��
*/
#define HI_SEM_ONE  ((HI_U8)1)
/** 
* @ingroup  hct_sem
* �ź�����ֵ
* 
* ����#HI_MDM_SEM_BCreate���ucInit��ȡֵ����ʾ�������ź����ĳ�ʼֵΪ0��
*/
#define HI_SEM_ZERO  ((HI_U8)0)

/**
* @ingroup  hct_sem
* @brief  �����ź�����
*
* @par ����: 
* �����ź�����
* @attention �ޡ�
*
* @param  pSemId [OUT] �ź���ID�š�
* @param  szName[8] [IN] �ź������֡�
* @param  ucInit [IN] ֵΪ#HI_SEM_ONE�����ٽ���Դ������ֵΪ#HI_SEM_ZERO����ͬ����
*
* @retval #HI_ERR_SUCCESS      �����ɹ���
* @retval ����ֵ ����ʧ�ܣ����hi_errno.h��
*
* @par Dependency:
* @li hi_mdm_sem.h: �ýӿ��������ڵ�ͷ�ļ���
* @see  HI_MDM_SEM_Delete
* @since HiMDL_V200R001C00
*/
HI_U32 HI_MDM_SEM_BCreate (HI_U32 * pSemId, HI_CHAR szName[8], HI_U8 ucInit);

/**
* @ingroup  hct_sem
* @brief  ɾ���ź�����
*
* @par ����:  
* ɾ���ź�����
* @attention �ޡ�
*
* @param  ulSemId [IN] �ź���ID�š�
*
* @retval #HI_ERR_SUCCESS      ɾ���ɹ���
* @retval #HI_ERR_SEM_DELETE_FAIL ɾ��ʧ�ܡ�
*
* @par Dependency:
* @li hi_mdm_sem.h: �ýӿ��������ڵ�ͷ�ļ���
* @see  HI_MDM_SEM_BCreate��
* @since HiMDL_V200R001C00
*/
HI_U32 HI_MDM_SEM_Delete(HI_U32 ulSemId);

/**
* @ingroup  hct_sem
* @brief  ��ȡ�ź�����
*
* @par ����:   
* �ȴ��ź�����
* @attention �ޡ�

* @param  ulSemId [IN] �ź���ID�š�
* @param  ulTimeOut [IN] ��ʱʱ�䣬��λ:ms�������ó�HI_SYS_WAIT_FOREVER����Ϊ���õȴ���
*
* @retval #HI_ERR_SUCCESS �ɹ���
* @retval ����ֵ ʧ�ܣ����hi_errno.h��
*
* @par Dependency:
* @li hi_mdm_sem.h: �ýӿ��������ڵ�ͷ�ļ���
* @see  HI_MDM_SEM_Signal
* @since HiMDL_V200R001C00
*/
HI_U32 HI_MDM_SEM_Wait(HI_U32 ulSemId, HI_U32 ulTimeOut);

/**
* @ingroup  hct_sem
* @brief  �ͷ��ź�����
*
* @par ����:   
* �ͷ��ź�����
* @attention 
* @li �ýӿ�֧������������������ʹ�á�
* @li �ýӿ�֧����LISR��������ʹ�á�
*
* @param  ulSemId [IN] �ź���ID�š�
*
* @retval #HI_ERR_SUCCESS �ɹ���
* @retval ����ֵ ʧ�ܣ����hi_errno.h��
*
* @par Dependency:
* @li hi_mdm_sem.h: �ýӿ��������ڵ�ͷ�ļ���
* @see  HI_MDM_SEM_Wait
* @since HiMDL_V200R001C00
*/
HI_U32 HI_MDM_SEM_Signal (HI_U32 ulSemId);
#endif

