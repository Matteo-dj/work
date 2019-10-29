/**@defgroup hct_isr �ж�
 * @ingroup osa
 */

#ifndef __HI_MDM_ISR_H__
#define __HI_MDM_ISR_H__

/**
* @ingroup  hct_isr
* @brief  isr�ص�����������
*
* @par 
* 
* @attention �ޡ�
*
* @param  data1 [IN] ���� #HI_U32 �ص����1
* @param  data2 [IN] ���� #HI_U32 �ص����2
* @param  data3 [IN] ���� #HI_U32 �ص����3
* @param  data4 [IN] ���� #HI_U32 �ص����4
* @param  data5 [IN] ���� #HI_U32 �ص����5
*
* @retval ��.
* @par Dependency:
* <ul><li>hi_mdm_isr.h: �ļ���������isr��ؽӿ�.</li></ul>
* @see �ޡ�
* @since HiMDL_V200R001C00
*/
typedef HI_VOID (*HISR_PROC_FUNC)(HI_U32 ulData1,HI_U32 ulData2,HI_U32 ulData3,HI_U32 ulData4,HI_U32 ulData5);


/**
* @ingroup  hct_isr
* @brief  ���жϡ�
*
* @par ����:  
* ���жϺ���ִ��������ȵĺ�������HI_MDM_Sleep�����������ӿڡ�
* @attention ���жϽ�������Ԥ�ڵĶ�ʱ��Ĳ���������Ӱ���ж���Ӧ�����������������⡣
* 
* @param �ޡ�
*
* @retval #HI_U32 ���ж�ǰ���ж�״̬��
*
* @par Dependency:
* @li hi_mdm_isr.h: �ýӿ��������ڵ�ͷ�ļ���
* @see  HI_MDM_INT_Restore
* @since HiMDL_V200R001C00
*/
HI_U32 HI_MDM_INT_Lock(HI_VOID);

/**
* @ingroup  hct_isr
* @brief  �ָ����ж�ǰ��״̬��
*
* @par ����:  
*  �ָ����ж�ǰ��״̬��
* @attention ��α�������֮��Ӧ�Ĺ��ж�ʱ����Ĺ��ж�֮ǰ��CPSR��ֵ��
*
* @param  ulIntSave [IN] �ж�״̬��
*
*
* @par Dependency:
* @li hi_mdm_isr.h: �ýӿ��������ڵ�ͷ�ļ���
* @see  HI_MDM_INT_Lock
* @since HiMDL_V200R001C00
*/
HI_VOID HI_MDM_INT_Restore(HI_U32 ulIntSave);

/**
* @ingroup  hct_isr
* @brief  ʹ��ָ���жϡ�
*
* @par ����:  
* ʹ��ָ���жϡ�
* @attention �ޡ�
*
* @param  ulVector [IN] �жϺš�
*
* @retval #HI_ERR_SUCCESS �ɹ���
* @retval ����ֵ ʧ�ܣ����hi_errno.h��
*
* @par Dependency:
* @li hi_mdm_isr.h: �ýӿ��������ڵ�ͷ�ļ���
* @see  HI_MDM_IRQ_Disable
* @since HiMDL_V200R001C00
*/
HI_U32 HI_MDM_IRQ_Enable(HI_U32 ulVector);

/**
* @ingroup  hct_isr
* @brief  ȥʹ��ָ���жϡ�
*
* @par ����:  
* ȥʹ��ָ���жϡ�
* @attention �ޡ�
*
* @param  ulVector [IN] �жϺš�
*
* @retval #HI_ERR_SUCCESS �ɹ���
* @retval ����ֵ ʧ�ܣ����hi_errno.h��
*
* @par Dependency:
* @li hi_mdm_isr.h: �ýӿ��������ڵ�ͷ�ļ���
* @see  HI_MDM_IRQ_Enable
* @since HiMDL_V200R001C00
*/
HI_U32 HI_MDM_IRQ_Disable(HI_U32 ulVector);

/**
* @ingroup  hct_isr
* @brief ע���жϡ�
*
* @par ����:  
* ע���жϡ�
* @attention �жϴ�������ʱ���ܹ�����Ӱ��CPU���жϵļ�ʱ��Ӧ��
*
* @param  ulVector [IN] �жϺš�
* @param  ulPriority [IN] �ж����ȼ���
* @param  pIrqRoutine [IN] �жϻص�������
* @param  ulPara [IN] �жϲ�����
*
* @retval #HI_ERR_SUCCESS �ɹ���
* @retval ����ֵ ʧ�ܣ����hi_errno.h��
*
* @par Dependency:
* @li hi_mdm_isr.h: �ýӿ��������ڵ�ͷ�ļ���
* @see  HI_MDM_IRQ_Free
* @since HiMDL_V200R001C00
*/
HI_U32 HI_MDM_IRQ_Request (HI_U32 ulVector,HI_U16 ulPriority,HI_PVOID pIrqRoutine,HI_U32 ulPara);

/**
* @ingroup  hct_isr
* @brief  ���ע���жϡ�
*
* @par ����:  
* ���ע���жϡ�
* @attention �ޡ�
*
* @param  ulVector [IN] ���� #HI_U32  �жϺš�
*
* @retval #HI_ERR_SUCCESS �ɹ���
* @retval ����ֵ ʧ�ܣ����hi_errno.h��
*
* @par Dependency:
* @li hi_mdm_isr.h: �ýӿ��������ڵ�ͷ�ļ���
* @see  HI_MDM_IRQ_Request
* @since HiMDL_V200R001C00
*/
HI_U32 HI_MDM_IRQ_Free (HI_U32 ulVector);

/**
* @ingroup  hct_isr
* @brief  �����ж�HISR�������жϵװ벿������ص�workqueue��
*
* @par ����:  
* �����ж�HISR�������жϵװ벿������ص�workqueue��
* @attention �ޡ�
*
* @param  ulPriority [IN] �жϵװ벿���ȼ���ȡֵΪ0-15 ֵԽС��Ӧ���ȼ�Խ�ߡ�
* @param  pfnHisrFunc [IN] ��Hisr������ڡ�
* @param  ulData1 [IN] ����1��
* @param  ulData2 [IN] ����2��
* @param  ulData3 [IN] ����3��
* @param  ulData4 [IN] ����4��
* @param  ulData5 [IN] ����5��
*
* @retval #HI_ERR_SUCCESS �ɹ���
* @retval ����ֵ ʧ�ܣ����hi_errno.h��
*
* @par Dependency:
* @li hi_mdm_isr.h: �ýӿ��������ڵ�ͷ�ļ���
* @see  �ޡ�
* @since HiMDL_V200R001C00
*/
HI_U32 HI_MDM_IRQ_AddJob(HI_U32 ulPriority, HISR_PROC_FUNC pfnHisrFunc,HI_U32 ulData1,HI_U32 ulData2,HI_U32 ulData3,HI_U32 ulData4,HI_U32 ulData5);

/**
* @ingroup  hct_isr
* @brief  ����Ƿ����ж��������С�
*
* @par ����:  
* ����Ƿ����ж��������С�
* @attention �ޡ�
*
* @param �ޡ�
*
* @retval HI_TRUE ���ж������ġ�
* @retval HI_FALSE �����ж������ġ�
*
* @par Dependency:
* @li hi_mdm_isr.h: �ýӿ��������ڵ�ͷ�ļ���
* @see  �ޡ�
* @since HiMDL_V200R001C00
*/
HI_BOOL HI_MDM_IsIntContext(HI_VOID);
#endif
