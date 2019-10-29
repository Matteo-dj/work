#ifndef __DFX_TRACK_H__
#define __DFX_TRACK_H__
#include <hi_types.h>
#include <hi_mdm_types.h>

typedef struct
{
    HI_U32 ulArrayCount;//�켣ͳ��֧�ּ�¼��������Ϣ
    HI_U32 ulOption;//�켣ͳ��֧��ͳ���������͹켣(����,���ж�...)
}HI_DFX_TRACK_ST_CMD_S;//�����켣ͳ��cmd

typedef struct
{
    HI_U32 ulRet;//����������
}HI_DFX_TRACK_ST_IND_S;//�����켣ͳ��IND


typedef struct
{
    HI_U32 pad;
}HI_DFX_TRACK_STOP_CMD_S;//ֹͣ�켣ͳ��cmd


typedef struct
{
    HI_U32 ulRet;//ֹͣ������
}HI_DFX_TRACK_STOP_IND_S;//ֹͣ�켣ͳ��IND

typedef struct
{
    HI_U32 bLock;//�Ƿ����lock����������lock������ʾ�鿴�µ���Ϣ��������lock������ʾ���²鿴��ʷ��Ϣ
}HI_DFX_TRACK_GET_INFO_CMD_S;//��ȡ�켣ͳ����Ϣcmd

typedef struct
{
    HI_U32 ulRet;//������
    HI_U32 ulArrayCount;//���֧�ֶ������켣ͳ��,����ʱ����
    HI_U32 ulOption;//֧������켣ͳ��,����������
    HI_U32 ulLockPos;//��ǰ�������±�ID
    
}HI_DFX_TRACK_INFO_TOTAL_IND_S;//�ϱ��켣������ϢIND

typedef struct
{
    HI_U32 ulRet;
    HI_U32 ulSeq;//��Ŵ�0��ulArrayCount-1

    HI_U32 ulCurTime;//ͳ��ʱ���
    HI_U32 ulTraceType;//ͳ������
    HI_U32 ulEntry;//��ʾ����λ�ã������������Ϣ
    HI_U32 ulData0;//����0
    HI_U32 ulData1;//����1
}HI_DFX_TRACK_INFO_ITEM_IND_S;//�ϱ��켣һ����ϢIND

#endif

