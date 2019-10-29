
#ifndef __HI_MDM_TASK_INTERNAL_H__
#define __HI_MDM_TASK_INTERNAL_H__
#include <hi_types.h>
#include <hi_mdm_types.h>
#include <hi_mdm_task.h>

/**
* @ingroup  hct_task
* @brief  �жϵ�ǰ�����Ƿ�������״̬��
*
* @par ����:  
* �жϵ�ǰ�����Ƿ�������״̬��
* @attention �ޡ�
*
* @param �ޡ�
*
* @retval HI_TRUE ��ǰ����������״̬��
* @retval HI_FALSE ��ǰ����δ��������״̬��
*
* @par Dependency:
* @li hi_mdm_task.h: �ýӿ��������ڵ�ͷ�ļ���
* @see  HI_MDM_TASK_Lock��
* @since HiMDL_V200R001C00
*/
HI_BOOL HI_MDM_TASK_IsLock(HI_VOID);

#endif//__HI_MDM_TASK_INTERNAL_H__
