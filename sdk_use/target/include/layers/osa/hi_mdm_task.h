/**@defgroup hct_task ����
 * @ingroup osa
 */
#ifndef __HI_MDM_TASK_H__
#define __HI_MDM_TASK_H__
#if !defined(MAC_V200_LLT)
#include "los_trace.h"
#endif
#include <hi_types.h>
#include <hi_mdm_types.h>

/**
 * @ingroup hct_task
 * �Ƿ�����ID
 */
#define HI_INVALID_TASK_ID   0xFFFFFFFF

/**
 * @ingroup hct_task
 * �����Զ�ɾ��
 */
#define HI_TASK_STATUS_DETACHED    0x0080

/**
 * @ingroup hct_task
 * �������Ƴ���
 */
#define HI_TASK_NAME_LEN     32

typedef struct
{
    HI_U32 pad[3];
}HI_EVENT_CB_S;//��ʱ����

/**
 * @ingroup hct_task
 * ������Ϣ
 */
typedef struct
{
    HI_CHAR         acName[HI_TASK_NAME_LEN];   /**< ������ */
    HI_U32          uwTaskID;					/**< ����ID */
    HI_U16          usTaskStatus;               /**< ����״̬ */
    HI_U16          usTaskPrio;                 /**< �������ȼ� */
    HI_PVOID        pTaskSem;                   /**< �ź���ָ�� */
    HI_PVOID        pTaskMux;                   /**< Mutex pointer             */
    HI_EVENT_CB_S   uwEvent;                    /**< �¼� */
    HI_U32          uwEventMask;                /**< �¼�����ֵ���������¼�����Щλ */
    HI_U32          uwStackSize;                /**< ����ջ�ߴ� */
    HI_U32          uwTopOfStack;               /**< ����ջջ�� */
    HI_U32          uwBottomOfStack;            /**< ����ջջ�� */
    HI_U32          uwSP;                       /**< ����SPָ�� */
    HI_U32          uwCurrUsed;                 /**< ����ǰʹ�õ�ջ��С*/
    HI_U32          uwPeakUsed;                 /**< Task stack peak used size           */
    HI_BOOL         bOvf;                       /**< �������ջ�Ƿ���� */
} HI_TASK_INFO_S;

typedef HI_VOID (*TSK_BODY)(HI_U32 ulParam);

/**
* @ingroup  hct_task
* @brief  ��������
*
* @par ����:  
* ��������
* @attention
* @li �ӿ��ڲ��������ռ�洢����������������Ҫ��֤�������ռ���ȫ�ֲ��ᱻ�ͷ��޸ĵġ�
* @li ��ָ��������ջ��СΪ0����ʹ��������LOSCFG_BASE_CORE_TSK_DEFAULT_STACK_SIZE(0x1000)ָ��Ĭ�ϵ�����ջ��С��
* @li ����ջ�Ĵ�С��8�ֽڴ�С���롣ȷ������ջ��С��ԭ���ǣ���֤ջ�����������¾���С(��Լ�ռ�)��
* @li ����ϵͳ����ռ��һ�������ȼ��������û�����ʹ�õ����ȼ�Ϊ2~29��Ӧ�ü�������ʹ�ô��ڵ���15�����ȼ���
* @param  pTaskId [OUT] ����ID�š�
* @param  szName[8] [IN] �������֡�
* @param  pfnTaskBody[IN] ������ں�����
* @param  ulParam[IN] ��������ʱ��Ҫ����������ڵĲ������������Ҫ���� ����ֱ����0��
* @param  ulStackSize[IN] ջ��С��
* @param  ulPriority[IN] �������ȼ���
*
* @retval #HI_ERR_SUCCESS �ɹ���
* @retval ����ֵ ʧ�ܣ����hi_errno.h��
*
* @par Dependency:
* @li hi_mdm_task.h: �ýӿ��������ڵ�ͷ�ļ���
* @see  HI_MDM_TASK_Delete
* @since HiMDL_V200R001C00
*/
HI_U32 HI_MDM_TASK_Create (HI_OUT HI_U32 * pTaskId, HI_CHAR szName[32], TSK_BODY pfnTaskBody,HI_U32 ulParam, HI_U32 ulStackSize, HI_U32 ulPriority);

/**
* @ingroup  hct_task
* @brief  ɾ������
*
* @par ����:  
* ɾ������
* @attention 
* @li ������Է�APP�������ɾ����
* @li ��ɾ������ʱҪ��֤�����������Դ���绥�������ź����ȣ��ѱ��ͷš�
* @param  ulTaskId [IN] ����ID�š�
*
* @retval #HI_ERR_SUCCESS �ɹ���
* @retval ����ֵ ʧ�ܣ����hi_errno.h��
*
* @par Dependency:
* @li hi_mdm_task.h: �ýӿ��������ڵ�ͷ�ļ���
* @see  HI_MDM_TASK_Create
* @since HiMDL_V200R001C00
*/
HI_U32 HI_MDM_TASK_Delete (HI_U32 ulTaskId);

/**
* @ingroup  hct_task
* @brief  ���񴴽�����ǿ�ӿڡ�
*
* @par ����:  
* �ýӿ�Ϊ���񴴽�����ǿ�ӿڣ�֧�ִ�����ɾ������
* @attention
* @li �ӿ��ڲ��������ռ�洢����������������Ҫ��֤�������ռ���ȫ�ֲ��ᱻ�ͷ��޸ĵġ�
* @li ��ָ��������ջ��СΪ0����ʹ��������LOSCFG_BASE_CORE_TSK_DEFAULT_STACK_SIZE(0x1000)ָ��Ĭ�ϵ�����ջ��С��
* @li ����ջ�Ĵ�С��8�ֽڴ�С���롣ȷ������ջ��С��ԭ���ǣ���֤ջ�����������¾���С(��Լ�ռ�)��
* @li ����ϵͳ����ռ��һ�������ȼ��������û�����ʹ�õ����ȼ�Ϊ2~29��Ӧ�ü�������ʹ�ô��ڵ���15�����ȼ���
* @param  pTaskId [OUT] ����ID�š�
* @param  szName[8] [IN] �������֡�
* @param  pfnTaskBody[IN] ������ں�����
* @param  ulParam[IN] ��������ʱ��Ҫ����������ڵĲ������������Ҫ���� ����ֱ����0��
* @param  ulStackSize[IN] ջ��С��
* @param  ulPriority[IN] �������ȼ���
* @param  ulOption[IN] �����Զ�ɾ��ѡ�������һ����ɾ��������ò�����ȡֵHI_TASK_STATUS_DETACHED������ȡֵΪ0��
*
* @retval #HI_ERR_SUCCESS �ɹ���
* @retval ����ֵ ʧ�ܣ����hi_errno.h��
*
* @par Dependency:
* @li hi_mdm_task.h: �ýӿ��������ڵ�ͷ�ļ���
* @see �ޡ�
* @since HiMDL_V200R001C00
*/
HI_U32 HI_MDM_TASK_CreateEx (HI_OUT HI_U32* pTaskId, HI_CHAR szName[32], TSK_BODY pfnTaskBody, HI_U32  ulParam, HI_U32 ulStackSize, HI_U32 ulPriority,HI_U32 ulOption);

/**
* @ingroup  hct_task
* @brief  ��������
*
* @par ����:  
* ����ָ������
* @attention
* @li ���������ʱ����Ϊ��ǰ�����������������ܱ�����
* @li ������Է�APP������й��������
* @li �����������״̬�£���ֹ������������¼���
* @param  ulTaskId [IN] ����ID�š�
*
* @retval #HI_ERR_SUCCESS �ɹ���
* @retval ����ֵ ʧ�ܣ����hi_errno.h��
*
* @par Dependency:
* @li hi_mdm_task.h: �ýӿ��������ڵ�ͷ�ļ���
* @see  HI_MDM_TASK_Resume
* @since HiMDL_V200R001C00
*/
HI_U32 HI_MDM_TASK_Suspend (HI_U32 ulTaskId);

/**
* @ingroup  hct_task
* @brief  �ָ������������
*
* @par ����:  
* �ָ������������
* @attention �ޡ�
*
* @param  ulTaskId [IN] ����ID�š�
*
* @retval #HI_ERR_SUCCESS �ɹ���
* @retval ����ֵ ʧ�ܣ����hi_errno.h��
*
* @par Dependency:
* @li hi_mdm_task.h: �ýӿ��������ڵ�ͷ�ļ���
* @see  HI_MDM_TASK_Suspend
* @since HiMDL_V200R001C00
*/
HI_U32 HI_MDM_TASK_Resume (HI_U32 ulTaskId);

/**
* @ingroup  hct_task
* @brief  ��ȡ�������ȼ���
*
* @par ����:  
* ��ȡ�������ȼ���
* @attention 
* @li �����task ID��Ӧ������δ�������߳��������������ͳһ���ش���
*
* @param  ulTaskId [IN] ����ID�š�
* @param  pPriority [OUT] �������ȼ���
*
* @retval #HI_ERR_SUCCESS �ɹ���
* @retval ����ֵ ʧ�ܣ����hi_errno.h��
*
* @par Dependency:
* @li hi_mdm_task.h: �ýӿ��������ڵ�ͷ�ļ���
* @see  HI_MDM_TASK_SetPriority
* @since HiMDL_V200R001C00
*/
HI_U32 HI_MDM_TASK_GetPriority (HI_U32 ulTaskId, HI_OUT HI_U32 *pPriority);

/**
* @ingroup  hct_task
* @brief  �����������ȼ���
*
* @par ����: 
* �����������ȼ���
* @attention 
* �����޸ķ�APP��������ȼ���
* @param  ulTaskId [IN] ����ID�š�
* @param  ulPriority [IN] �������ȼ���
*
* @retval #HI_ERR_SUCCESS �ɹ���
* @retval ����ֵ ʧ�ܣ����hi_errno.h��
*
* @par Dependency:
* @li hi_mdm_task.h: �ýӿ��������ڵ�ͷ�ļ���
* @see  HI_MDM_TASK_GetPriority
* @since HiMDL_V200R001C00
*/
HI_U32 HI_MDM_TASK_SetPriority(HI_U32 ulTaskId, HI_U32 ulPriority);

/**
* @ingroup  hct_task
* @brief  ��������ȡ�
*
* @par ����:  
* ��������ȡ�
* @attention 
* @li �����ڼ䲻��������������Ƚӿڣ����ź�����HI_MDM_Sleep�ȡ�
* @li ��������ȣ��������жϣ���������Կɱ��жϴ�ϡ�
* @li ��������ȱ���ͽ�������������ʹ�á�
*
* @param �ޡ�
*
* @retval �ޡ�
*
* @par Dependency:
* @li hi_mdm_task.h: �ýӿ��������ڵ�ͷ�ļ���
* @see  HI_MDM_TASK_UnLock
* @since HiMDL_V200R001C00
*/
HI_VOID HI_MDM_TASK_Lock(HI_VOID);

/**
* @ingroup  hct_task
* @brief  ����������ȡ�
*
* @par ����:  
* ����������ȡ�
* @attention �ޡ�
*
* @param �ޡ�
*
* @retval �ޡ�
*
* @par Dependency:
* @li hi_mdm_task.h: �ýӿ��������ڵ�ͷ�ļ���
* @see  HI_MDM_TASK_Lock��
* @since HiMDL_V200R001C00
*/
HI_VOID HI_MDM_TASK_UnLock(HI_VOID);


/**
* @ingroup  hct_task
* @brief  ��ȡ��ǰ����ID��
*
* @par ����:  
* ��ȡ��ǰ����ID��
* @attention �ޡ�
*
* @param �ޡ�
*
* @retval ����ID ��ǰ����ID��ȡֵΪ#HI_INVALID_TASK_ID��ʾ�Ƿ�����ID��
*
* @par Dependency:
* @li hi_mdm_task.h: �ýӿ��������ڵ�ͷ�ļ���
* @see  HI_MDM_TASK_Lock��
* @since HiMDL_V200R001C00
*/
HI_U32 HI_MDM_TASK_GetCurrentID(HI_VOID);

/**
* @ingroup  hct_task
* @brief  ��ȡ������Ϣ��
*
* @par ����:  
* ��ȡ������Ϣ��
* @attention �ޡ�
*
* @param ulTaskId [in]����ID��
* @param pstTaskInfo [out]������Ϣ��
*
* @retval #HI_ERR_SUCCESS �ɹ���
* @retval ����ֵ ʧ�ܣ����hi_errno.h��
*
* @par Dependency:
* @li hi_mdm_task.h: �ýӿ��������ڵ�ͷ�ļ���
* @see  HI_MDM_TASK_Lock��
* @since HiMDL_V200R001C00
*/
HI_U32 HI_MDM_TASK_GetTaskInfo(HI_U32 ulTaskId,HI_TASK_INFO_S *pstTaskInfo);

/**
* @ingroup  hct_task
* @brief ����˯�ߡ�
*
* @par ����:
* ���жϴ������л����������������£�ִ��HI_MDM_Sleep������ʧ�ܡ�
* @attention 
* ���С��10msʱ������10ms��������10msʱ,��ЧֵȡΪ�������֣���˯��ʱ���趨Ϊ15ms����ʵ����ЧֵΪ10ms��
* @param  ulMs [IN] ˯��ʱ��, ��λ:ms������Ϊ10ms��
*
* @retval #HI_ERR_SUCCESS �ɹ���
* @retval ����ֵ ʧ�ܣ����hi_errno.h��
*
* @par Dependency:
* @li hi_mdm_task.h: �ýӿ��������ڵ�ͷ�ļ���
* @see �ޡ�
* @since HiMDL_V200R001C00
*/
HI_U32 HI_MDM_Sleep(HI_U32 ulMs);
#endif

