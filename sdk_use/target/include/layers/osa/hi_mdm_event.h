/**@defgroup hct_event �¼�
 * @ingroup osa
 */

#ifndef __HI_MDM_EVENT_H__
#define __HI_MDM_EVENT_H__
#include <hi_types.h>
#include <hi_mdm_types.h>

#define HI_INVALID_EVENT_ID           (0xffffffff)/**< ��ȡ�¼�IDʧ��         */
#define HI_EVENT_WAITMODE_AND         (4) /**< ���������¼����������ˣ����ܶ�ȡ�ɹ�       */
#define HI_EVENT_WAITMODE_OR          (2) /**< �����κ��¼������ˣ��Ϳ��Զ�ȡ�ɹ�       */ 
#define HI_EVENT_WAITMODE_CLR         (1) /**< ��ȡ�¼��ɹ����Զ������Ӧ�¼�        */ 

/**
* @ingroup  hct_event
* @brief  �����¼���
*
* @par ����:  
* �����¼���
* @attention �ޡ�
*
* @param  pEventID [OUT] �¼�ID�š�
* @param  szName[8] [IN] �¼����֡�
*
* @retval #HI_ERR_SUCCESS �ɹ���
* @retval ����ֵ ʧ�ܣ����hi_errno.h��
*
* @par Dependency:
* @li hi_mdm_event.h: �ýӿ��������ڵ�ͷ�ļ���
* @see  HI_MDM_EVENT_Delete
* @since HiMDL_V200R001C00
*/
HI_U32 HI_MDM_EVENT_Create(HI_OUT HI_U32 *pEventID,HI_CHAR szName[8]);

/**
* @ingroup  hct_event
* @brief  �����¼���
*
* @par ����:  
* �����¼���
* @attention
* @li �ýӿ�֧������������������ʹ�á�
* @li �ýӿ�֧����LISR��������ʹ�á�
*
* @param  ulEventID [IN] �¼�ID�š�
* @param  ulEventBits [IN] �¼�bitλ��
*
* @retval #HI_ERR_SUCCESS �ɹ���
* @retval ����ֵ ʧ�ܣ����hi_errno.h��
*
* @par Dependency:
* @li hi_mdm_event.h: �ýӿ��������ڵ�ͷ�ļ���
* @see  HI_MDM_EVENT_Wait
* @since HiMDL_V200R001C00
*/
HI_U32 HI_MDM_EVENT_Send(HI_U32 ulEventID,HI_U32 ulEventBits);

/**
* @ingroup  hct_event
* @brief  �ȴ��¼���
*
* @par ����:  �ȴ��¼���
* @attention �ޡ�
*
* @param  ulEventID [IN] �¼�ID�š�
* @param  ulEventMask [IN] �¼�bitλ������롣
* @param  pEventBits [OUT] �¼�bitλ��
* @param  ulTimeOut [IN] �¼���ʱʱ�䣬��λ:ms��
* @param  ulFlag [IN] ��־��ȡֵ:
* @li #HI_EVENT_WAITMODE_AND
* @li #HI_EVENT_WAITMODE_OR
* @li #HI_EVENT_WAITMODE_CLR \n
* ˵��:#HI_EVENT_WAITMODE_AND��#HI_EVENT_WAITMODE_OR����"��"#HI_EVENT_WAITMODE_CLRȥʹ�ã�\n
*��HI_EVENT_WAITMODE_AND|HI_EVENT_WAITMODE_CLR������������������"��"ȥʹ�á�
*
* @retval #HI_ERR_SUCCESS �ɹ���
* @retval ����ֵ ʧ�ܣ����hi_errno.h��
*
* @par Dependency:
* @li hi_mdm_event.h: �ýӿ��������ڵ�ͷ�ļ���
* @see  HI_MDM_EVENT_Send
* @since HiMDL_V200R001C00
*/
HI_U32 HI_MDM_EVENT_Wait (HI_U32 ulEventID, HI_U32 ulEventMask,HI_OUT HI_U32 * pEventBits,HI_U32 ulTimeOut,HI_U32 ulFlag);

/**
* @ingroup  hct_event
* @brief  ����¼���
*
* @par ����:  
* ����¼���
* @attention
* @li �ýӿ�֧������������������ʹ�á�
* @li �ýӿ�֧����LISR��������ʹ�á�
*
* @param  ulEventID [IN] ���� #HI_U32  �¼�ID�š�
* @param  ulEventBits [IN] ���� #HI_U32 �¼�bitλ��
*
* @retval #HI_ERR_SUCCESS �ɹ���
* @retval ����ֵ ʧ�ܣ����hi_errno.h��
*
* @par Dependency:
* @li hi_mdm_event.h: �ýӿ��������ڵ�ͷ�ļ���
* @see  HI_MDM_EVENT_Wait
* @since HiMDL_V200R001C00
*/
HI_U32 HI_MDM_EVENT_Clear(HI_U32 ulEventID,HI_U32 ulEventBits);

/**
* @ingroup  hct_event
* @brief  ɾ���¼���
*
* @par ����:  
* ɾ���¼���
* @attention �ޡ�
*
* @param  ulEventID [IN] �¼�ID�š�
*
* @retval #HI_ERR_SUCCESS �ɹ���
* @retval ����ֵ ʧ�ܣ����hi_errno.h��
*
* @par Dependency:
* @li hi_mdm_event.h: �ýӿ��������ڵ�ͷ�ļ���
* @see  HI_MDM_EVENT_Create
* @since HiMDL_V200R001C00
*/
HI_U32 HI_MDM_EVENT_Delete(HI_U32 ulEventID);
#endif

