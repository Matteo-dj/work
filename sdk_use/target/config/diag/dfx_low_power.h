
#ifndef __DFX_LOW_POWER_H__
#define __DFX_LOW_POWER_H__
#include <hi_types.h>
#include <hi_mdm_types.h>
#include <hi_low_power.h>

typedef struct
{
    HI_U32 cmd_id;
    HI_U32 data1;
    HI_U32 data2;
    HI_U32 data3;
    HI_U32 data4;
}HI_LOW_POWER_CMD_INFO_CMD_S;//��������

typedef struct
{
    HI_U32 ulMaskIntBits;//�͹���������Щ�ж�
    HI_U32 ulMaskIntBitsBackUp;

    HI_U32 last_cnt;

    HI_U32 l_sleepTimes;//�жϽ��sleep����
    HI_U32 h_sleepTimes;
    
    HI_U32 l_unSleepTimes;//�жϽ���޷�sleep����
    HI_U32 h_unSleepTimes;

    
    HI_U32 l_totalTime;//�͹�����ʱ��
    HI_U32 h_totalTime;//�͹�����ʱ��
    
    HI_U32 l_t1_2_t4;//�ⲿ˯��ʱ���ܺ�
    HI_U32 h_t1_2_t4;//�ⲿ˯��ʱ���ܺ�
    
    HI_U32 l_t2_2_t3;//�ڲ�˯��ʱ���ܺ�
    HI_U32 h_t2_2_t3;//�ڲ�˯��ʱ���ܺ�
    
    HI_U32 l_t1_2_t2;//��˯ʱ���ܺ�
    HI_U32 h_t1_2_t2;//��˯ʱ���ܺ�
    
    HI_U32 l_t3_2_t4;//����ʱ���ܺ�
    HI_U32 h_t3_2_t4;//����ʱ���ܺ�


    HI_U32 ulLastWakeUpLisr;
    HI_U32 ulLowPowerCfg;//�͹�������
    HI_U32 ulForbidBits;//��ֹ����͹���

    HI_U32 ulCheckStat[HI_LOW_POWER_CHECK_ID_LIMIT];//ʲôԭ�����޷�����͹���
    HI_U32 ulForbidStat[HI_LOW_POWER_FORBID_ID_LIMIT];//ʲôԭ�����޷�����͹���
}HI_LOW_POWER_CMD_INFO_IND_S;//����������
#endif

