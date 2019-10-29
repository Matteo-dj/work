/**@defgroup hct_msgqueue ��Ϣ����
 * @ingroup osa
 */
#ifndef __HI_MDM_MSG_H__
#define __HI_MDM_MSG_H__
#include <hi_types.h>
#include <hi_mdm_types.h>
#include <hi_config.h>

/**
 * @ingroup hct_msgqueue
 * ��Ϣ�����Ƿ�ֵ����
 */
#define HI_ERR_MSG_INVALID_MSG_NUM 0xFFFFFFFF 


/**
 * @ingroup hct_msgqueue
 * ��Ϣ�ṹ�嶨��
 */
typedef struct
{
    hi_u32 ulMsgId;                             /**< ��ϢID */
    hi_u32 ulTimeStamp;                         /**< ������Ϣʱ�䣬ϵͳ�Զ���д���������������*/
    uintptr_t ulParam[HI_SYS_MSG_PARAM_NUM_MAX];   /**< ��Ϣ���� */
} HI_SYS_QUEUE_MSG_S;

/**
* @ingroup  hct_msgqueue
* @brief   ������Ϣ���С�
*
* @par  ����:
* ������Ϣ���С�
* @attention �ޡ�
*
* @param  pQueueId [OUT] ����������Ϣ���о����
* @param  szName [IN] ��Ϣ�������ƣ���Ҫ���ϲ����洢���ͷţ��ײ㲻���档
* @param  usMsgArrayCnt [IN] ��Ϣ���г��ȡ���Ϣ�����޳������ƣ�ֻҪ���ڴ�����ܳɹ����뵽�ڴ漴�ɡ�
*
* @retval #HI_ERR_SUCCESS �����ɹ���
* @retval #HI_ERR_MSG_INVALID_PARAM ��δ��󣬾��ָ��Ϊ�ա����ֵ�ַΪ�ա���Ϣ���г���Ϊ0��
* @retval #HI_ERR_MSG_CREATE_Q_FAIL �����ź������󣬱����ڴ治�㣬Ԥ��ֵ��Ϣ����ʹ����ϵȡ�
*
* @par Dependency:
* @li hi_mdm_msg.h: �ýӿ��������ڵ�ͷ�ļ���
* @li hi_errno.h: �ļ����ô����롣
* @see HI_MDM_MSG_DeleteQueue 
* @since HiMDL_V200R001C00
*/
HI_U32 HI_MDM_MSG_CreateQueue(HI_OUT HI_U32 * pQueueId,HI_CHAR szName[8],HI_U16 usMsgArrayCnt);
/**
* @ingroup  hct_msgqueue
* @brief  ɾ����Ϣ���С�
*
* @par  ����:
* ɾ����Ϣ���С�
* @attention �ޡ�
*
* @param  pQueueId [IN] ��Ϣ���о����
*
* @retval #HI_ERR_SUCCESS ɾ���ɹ���
* @retval #HI_ERR_MSG_Q_DELETE_FAIL ɾ����Ϣ���д��󣬱���IDԽ�磬
* ��Ϣ����δ��������Ϣ��������ʹ�����޷�ɾ���ȡ�
*
* @par Dependency:
* @li hi_mdm_msg.h: �ýӿ��������ڵ�ͷ�ļ���
* @li hi_errno.h: �ļ����ô����롣
* @see HI_MDM_MSG_CreateQueue 
* @since HiMDL_V200R001C00
*/
HI_U32 HI_MDM_MSG_DeleteQueue(HI_U32 ulQueueId);
/**
* @ingroup  hct_msgqueue
* @brief   ������Ϣ��
*
* @par ����: 
* ������Ϣ��
* @attention 
* @li ��ulTimeOutΪ0ʱ���ýӿ�֧������������������ʹ�á�
* @li ��ulTimeOutΪ0ʱ���ýӿ�֧����LISR��������ʹ�á�
*
* @param  ulQueueId [IN] ��Ϣ���о����
* @param  pstMsg [IN] ��Ϣ����ָ�롣
* @param  ulTimeOut [IN] ��Ϣ���ͳ�ʱʱ�䣬�������;�д0��
*
* @retval #HI_ERR_SUCCESS ���ͳɹ���
* @retval #HI_ERR_MSG_SEND_FAIL ������Ϣ���д��󣬰�����δ���
* ��Ϣ����δ�������������ݴ��ڶ��д���ʱ���ô�С���ж���ʹ�õ���ʱʱ�䲻Ϊ0��
* @retval #HI_ERR_MSG_INVALID_PARAM ��δ�����Ϣ����ָ��Ϊ�ա�
*
* @par Dependency:
* @li hi_mdm_msg.h: �ýӿ��������ڵ�ͷ�ļ���
* @li hi_errno.h: �ļ����ô����롣
* @see HI_MDM_MSG_Wait 
* @since HiMDL_V200R001C00
*/
HI_U32 HI_MDM_MSG_Send(HI_U32 ulQueueId, HI_SYS_QUEUE_MSG_S* pstMsg,HI_U32 ulTimeOut);
/**
* @ingroup  hct_msgqueue
* @brief  ������Ϣ��
*
* @par ����: 
* ������Ϣ��
* @attention �ޡ�
*
* @param  ulQueueId [IN] ��Ϣ���о����
* @param  pstMsg [OUT] ��Ϣ����ָ�롣
* @param  ulTimeOut [IN] ��Ϣ���ճ�ʱʱ��,�����ʾ���õȴ���ʹ��#HI_SYS_WAIT_FOREVER��
*
* @retval #HI_ERR_SUCCESS ��Ϣ���ճɹ���
* @retval #HI_ERR_MSG_WAIT_FAIL �ȴ���Ϣ���д��󣬱�����β���ȷ����Ϣ����δ������
* �ȴ���Ϣ��С���ڶ��д���ʱ���ô�С���ж��еȴ���ʱ�������Ϣ���С�
* @retval #HI_ERR_MSG_INVALID_PARAM ��δ�����Ϣ����ָ��Ϊ�ա�
* @retval #HI_ERR_MSG_WAIT_TIME_OUT �ȴ���ʱδ�յ���Ϣ��
*
* @par Dependency:
* @li hi_mdm_msg.h: �ýӿ��������ڵ�ͷ�ļ���
* @li hi_errno.h: �ļ����ô����롣
* @see HI_MDM_MSG_Send 
* @since HiMDL_V200R001C00
*/
HI_U32 HI_MDM_MSG_Wait(HI_U32 ulQueueId, HI_OUT HI_SYS_QUEUE_MSG_S* pstMsg, HI_U32 ulTimeOut);
/**
* @ingroup  hct_msgqueue
* @brief   �����Ϣ�����Ƿ�������
*
* @par ����: 
* �����Ϣ�����Ƿ�������
* @attention 
* @li ��Ϣ����ID��Чʱ���ýӿڷ���HI_TRUE��
* @li �ýӿ�֧������������������ʹ�á�
* @li �ýӿ�֧����LISR��������ʹ�á�
*
* @param  pQueueId [IN] ��Ϣ���о����
* 
* @retval #HI_TRUE  ��Ϣ����������
* @retval #HI_FALSE ��Ϣ����δ����
*
* @par Dependency:
* @li hi_mdm_msg.h: �ýӿ��������ڵ�ͷ�ļ���
* @see �� 
* @since HiMDL_V200R001C00
*/
HI_BOOL HI_MDM_MSG_IsQueueFull(HI_U32 ulQueueId);
/**
* @ingroup  hct_msgqueue
* @brief   ��ȡ��Ϣ�����к��е���Ϣ������
*
* @par ����:  
* ��ȡID��ΪulQueueId����Ϣ�����е�ǰ���е���Ϣ������
* @attention 
* @li �ýӿ�֧������������������ʹ�á�
* @li �ýӿ�֧����LISR��������ʹ�á�
*
* @param  ulQueueId [IN] ��Ϣ���о����
* 
* @retval #HI_ERR_MSG_INVALID_MSG_NUM  ��ȡ��Ϣ���г���
* @retval ����ֵ ��Ϣ�����к��е���Ϣ������
*
* @par Dependency:
* @li hi_mdm_msg.h: �ýӿ��������ڵ�ͷ�ļ���
* @see �ޡ�
* @since HiMDL_V200R001C00
*/
HI_U32 HI_MDM_MSG_GetCurrentMsgNum(HI_U32 ulQueueId);
/**
* @ingroup  hct_msgqueue
* @brief  ��ȡ��Ϣ�������ɻ�����Ϣ������
*
* @par ����:  
* ��ȡID��ΪulQueueId����Ϣ���������ɻ������Ϣ������ 
* @attention 
* @li �ýӿ�֧������������������ʹ�á�
* @li �ýӿ�֧����LISR��������ʹ�á�
*
* @param  pQueueId [IN] ��Ϣ���о����
*
* @retval #HI_ERR_MSG_INVALID_MSG_NUM  ��ȡ��Ϣ���г�����δ�����Ϣ����δ������
* @retval ����ֵ ����Ϣ���пɻ������Ϣ������ 
*
* @par Dependency:
* @li hi_mdm_msg.h: �ļ�����������Ϣ������ؽӿڡ�
* @see �ޡ�
* @since HiMDL_V200R001C00
*/
HI_U32 HI_MDM_MSG_GetTotalMsgNum(HI_U32 ulQueueId);

#endif

