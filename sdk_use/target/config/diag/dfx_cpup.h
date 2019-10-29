#ifndef __DFX_CPUP_H__
#define __DFX_CPUP_H__
#include <hi_types.h>
#include <hi_mdm_types.h>

typedef struct
{
    HI_U32 ulAutoReportPeriodSec;//���ǰ汾ʱ����ͳ�ƶ೤ʱ���CPUռ���ʣ���ʱֹͣCPUͳ��(���ܿ���)���ݲ�ʹ��
}HI_DFX_CPUP_ST_CMD_S;//����CPUPͳ��CMD

typedef struct
{
    HI_U32 ulRet;//����������
}HI_DFX_CPUP_ST_IND_S;//����CPUPͳ��IND

typedef struct
{
    HI_U32 pad;
}HI_DFX_CPUP_END_CMD_S;//ֹͣCPUPͳ��CMD


typedef struct
{
    HI_U32 ulRet;//ֹͣ������
}HI_DFX_CPUP_END_IND_S;//ֹͣCPUPͳ��IND

typedef struct
{
    HI_U32 pad;
}HI_DFX_CPUP_GET_INFO_CMD_S;//��ȡCPUP��ϢCMD

typedef struct
{
    HI_U32 ulRet;//������
    HI_U32 ulTaskCount;//task����
    HI_U32 ulLastTotalTimeLow;//�ϴ�ͳ��ʱ�ܺ�ʱ��32λ
    HI_U32 ulLastTotalTimeHigh;//�ϴ�ͳ��ʱ�ܺ�ʱ��32λ
    HI_U32 ulCurTotalTimeLow;//����ͳ��ʱ�ܺ�ʱ��32λ
    HI_U32 ulCurTotalTimeHigh;//����ͳ��ʱ�ܺ�ʱ��32λ
}HI_CPUP_DFX_INFO_TOTAL_IND_S;//�ϱ�CPUP������ϢIND

#define HI_CPUP_DFX_INFO_TASK_NAME_SIZE 32//�ϱ�����taskname��󳤶�
typedef struct
{
    HI_U32 ulRet;//������
    HI_CHAR taskName[HI_CPUP_DFX_INFO_TASK_NAME_SIZE];//taskname or isr��
    HI_U32 ulLastItemTimeLow;//�ϴ�ͳ��ʱ��ʱ��32λ
    HI_U32 ulLastItemTimeHigh;//�ϴ�ͳ��ʱ��ʱ��32λ
    HI_U32 ulCurItemTimeLow;//����ͳ��ʱ��ʱ��32λ
    HI_U32 ulCurItemTimeHigh;//����ͳ��ʱ��ʱ��32λ
    HI_U32 ulPermillageTotal;//����ͳ������ǧ�ֱ�
    HI_U32 ulPermillagePhase;//������ͳ��ǧ�ֱ�
}HI_CPUP_DFX_INFO_ITEM_IND_S;//�ϱ�CPUPÿһ����Ϣ

#endif
