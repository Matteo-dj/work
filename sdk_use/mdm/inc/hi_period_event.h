#ifndef __CMN_PERIOD_EVENT_H__
#define __CMN_PERIOD_EVENT_H__
#ifdef PRODUCT_CFG_PRODUCT_TYPE_NDM
#define SAL_SDM_PERIOD_TASK_PERIOD_WAIT_MSG_DEFAULT_TIME        (1*1000)//�������¼�Ĭ����ѵʱ��
#else
#define SAL_SDM_PERIOD_TASK_PERIOD_WAIT_MSG_DEFAULT_TIME        (10*1000)//�������¼�Ĭ����ѵʱ��
#endif

#define SAL_SDM_PERIOD_TASK_PERIOD_SAVE_PERIOD      (10*1000)//��ν�ɲ����ڴ洢������֮ǰʱ�䣬������ı�������ʱ�䲹������


#define SAL_SDM_PERIOD_TASK_PERIOD_CHECK_ICC_CONNECT (5*1000)//ms
#define SAL_SDM_PERIOD_TASK_PERIOD_GET_VER (5*1000)//ms

/************************************������ʱ������******************************************************************************/
/************************************����Ϊ���⿪�Žӿ�******************************************************************************/

//����������ִ��ʱ��
typedef enum
{
    SAL_SDM_PERIOD_TASK_ID_SAVE_PERIOD,
    SAL_SDM_PERIOD_TASK_ID_MAX,
}SAL_SDM_PERIOD_TASK_ID_E;
typedef HI_U32 (*sdm_period_task_func)(HI_VOID);
//*****************************************************************************
// ��������: HI_PERIOD_SetEventPeriod
// ��������: ���������¼�����
//           
// ����˵��:
//      id[in] �����¼���ӦID
//      ms[in] ����
//
// �� �� ֵ: 
//      HI_ERR_SUCCESS:�ɹ�
//
// ����Ҫ��: 
//      ���õ��¼����ڱ�������ѯ���ڵ�������
// ���þ���: 
// ��    ��: �ߺ���20150205
//*****************************************************************************
HI_EXTERN HI_U32 HI_PERIOD_SetEventPeriod(HI_IN SAL_SDM_PERIOD_TASK_ID_E id,HI_IN HI_U32 ms);
//*****************************************************************************
// ��������: HI_PERIOD_StartPeriodTimer
// ��������: �����������¼���ѯ��ʱ��
//           
// ����˵��:
//
// �� �� ֵ: 
//      HI_ERR_SUCCESS:�ɹ�
//
// ����Ҫ��: 
// ���þ���: 
// ��    ��: �ߺ���20150205
//*****************************************************************************
HI_EXTERN HI_U32 HI_PERIOD_StartPeriodTimer(HI_VOID);
//*****************************************************************************
// ��������: HI_PERIOD_RegistPeriodEvent
// ��������: ע����������ѯ�¼�
//           
// ����˵��:
//      id[in] �����¼���ӦID
//      ms[in] ����
//      call_back[in] �û��ص�����
//
// �� �� ֵ: 
//      HI_ERR_SUCCESS:�ɹ�
//      ��������
//
// ����Ҫ��: 
//      ���õ��¼����ڱ�������ѯ���ڵ�������
// ���þ���: 
// ��    ��: �ߺ���20150205
//*****************************************************************************
HI_EXTERN HI_U32 HI_PERIOD_RegistPeriodEvent(HI_IN SAL_SDM_PERIOD_TASK_ID_E enId,HI_IN HI_U32 ulms,HI_IN sdm_period_task_func call_back);

#endif
