/**@defgroup hct_systimer ϵͳ��ʱ��
 * @ingroup osa
 */
#ifndef __HI_MDM_TIMER_H__
#define __HI_MDM_TIMER_H__
#include <hi_types.h>
#include <hi_mdm_types.h>

/**
 * @ingroup hct_systimer
 * ���û�����ռ�洢����ʱ��������Ϣ��һ�������Ϊָ��ȫ�ֱ����ռ䡣�ÿռ�Ϊ�ײ�����ϲ㲻���������
 */
typedef struct
{
    HI_U32 data[5];/**< ���ڴ洢�����붨ʱ���������Ϣ               */
}HI_TIMER_HANDLE_S;

/**
 * @ingroup hct_systimer
 * ���������������Ķ�ʱ�����͡�
 */
typedef enum
{
    HI_TIMER_TYPE_ONCE,     /**< ��ʾ���ζ�ʱ��              */
    HI_TIMER_TYPE_PERIOD,   /**< ��ʾ���ڶ�ʱ��               */
    HI_TIMER_TYPE_MAX
}HI_TIMER_TYPE_E;

/**
* @ingroup  hct_systimer
* @brief  ���嶨ʱ���ص����������͡�
*
* @par ����:���嶨ʱ���ص����������͡�
* 
* @attention �ޡ�
*
* @param  ulData [IN] �ص���Ρ�
*
* @retval ��
* @par Dependency:
* @li hi_mdm_timer.h: �ýӿ��������ڵ�ͷ�ļ���
* @see HI_MDM_TIMER_Start | HI_MDM_TIMER_Stop
* @since HiMDL_V200R001C00
*/
typedef HI_VOID (*TIMER_PROC_FUNC)(HI_U32 ulData);
/**
* @ingroup  hct_systimer
* @brief  ����������ϵͳ��ʱ����
*
* @par ����:
* ��API�ӿ�ʹ��ʱ�����¼���������
* @li ���δ������ʱ�����򴴽���������
* @li ����ö�ʱ�����ں���ã���ֱ��������ʱ����
* @li �����ʱ��û�е��ڣ���ֹͣ�ö�ʱ��������������ʱ����
* @li �ýӿ�֧������������������ʹ�á�
* @li �ýӿ�֧����LISR��������ʹ�á�
* @li �ýӿ�֧���ڹ��ж���������ʹ�á�
* 
* @attention ����ʱ���ص�����ִ�����ж������ģ�
*�ص����������򵥣�����ʹ��˯�ߡ��ȴ��ź�����������ȵĺ�����
*
* @param  pstTimerHandler [IN/OUT] �����
* @param  pfnTimerRoutine [IN] ��ʱ���ص�������
* @param  ulTimeOut [IN] ��ʱ����ʱʱ�䣬��λ:ms��
* @param  enType [IN] ��ʱ�����͡�
* @param  ulData [IN] �ص��������Ρ�
*
* @retval HI_ERR_SUCCESS ������ʱ���ɹ���
* @retval ����ֵ ʧ�ܣ����hi_errno.h��
*
* @par Dependency:
* @li hi_mdm_timer.h: �ýӿ��������ڵ�ͷ�ļ���
* @see HI_MDM_TIMER_Stop | HI_MDM_TIMER_Delete
* @since HiMDL_V200R001C00
*/
HI_U32 HI_MDM_TIMER_Start (HI_TIMER_HANDLE_S *pstTimerHandler, TIMER_PROC_FUNC pfnTimerRoutine,HI_U32 ulTimeOut,HI_TIMER_TYPE_E enType,HI_U32 ulData);
/**
* @ingroup  hct_systimer
* @brief  ֹͣϵͳ��ʱ����
*
* @par ����:
* ֹͣϵͳ��ʱ����
* @attention 
* ���ӿڽ�ֹͣ��ʱ��������ɾ���ö�ʱ����
* @li �ýӿ�֧������������������ʹ�á�
* @li �ýӿ�֧����LISR��������ʹ�á�
* @li �ýӿ�֧���ڹ��ж���������ʹ�á�
*
* @param  pstTimerHandler [IN] �����
*
* @retval HI_ERR_SUCCESS ֹͣ��ʱ���ɹ���
* @retval ����ֵ ʧ�ܣ����hi_errno.h��
*
* @par Dependency:
* @li hi_mdm_timer.h: �ýӿ��������ڵ�ͷ�ļ���
* @see HI_MDM_TIMER_Start | HI_MDM_TIMER_Delete
* @since HiMDL_V200R001C00
*/
HI_U32 HI_MDM_TIMER_Stop(HI_TIMER_HANDLE_S *pstTimerHandler);
/**
* @ingroup  hct_systimer
* @brief  ɾ����ʱ����
*
* @par  ����:
* �����ʱ��δ���ڣ�����ֹͣ�ö�ʱ����ɾ����
* @attention ɾ����ʱ�����������Ƕ�̬����Ŀռ䣬��Ҫ�ͷš�
*
* @param  pstTimerHandler [IN] �����
*
* @retval HI_ERR_SUCCESS ɾ����ʱ���ɹ���
* @retval ����ֵ ʧ�ܣ����hi_errno.h��
*
* @par Dependency:
* @li hi_mdm_timer.h: �ýӿ��������ڵ�ͷ�ļ���
* @see HI_MDM_TIMER_Start | HI_MDM_TIMER_Stop
* @since HiMDL_V200R001C00
*/
HI_U32 HI_MDM_TIMER_Delete(HI_TIMER_HANDLE_S *pstTimerHandler);

#endif

