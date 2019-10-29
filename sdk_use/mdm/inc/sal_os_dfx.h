//*****************************************************************************
//
//                  ��Ȩ���� (C), 2015-2018, ��Ϊ�������޹�˾
//
/*****************************************************************************
 *  �� �� ��   : sal_os_dfx.h
 *  �� �� ��   : V1.0
 *  ��������   : 2016-11-10
 *  ��������   : ���ļ�Ϊϵͳ��ά�ɲ��װ�ӿ������ļ�,����:
 *              �����洢��
 *              ����״̬�鿴��
 *              ��ʱ��״̬�鿴��
 *              ��Ϣ���С�
 *              �ڴ���Ϣ�鿴��
*******************************************************************************/

#ifndef __SAL_OS_DFX_H__
#define __SAL_OS_DFX_H__
#include <hi_types.h>
#include <los_list.h>
#include <los_base.h>
#include <hi_sal.h>
#include <mmu_config.h>
#include <asm/platform.h>
HI_START_HEADER

typedef enum
{
    HI_SWTIMER_MODE_ONCE = 0x0,
    HI_SWTIMER_MODE_PERIOD,    
    HI_SWTIMER_MODE_OPP,    
}HI_SWTIMER_MODE_E;

typedef enum
{
    HI_SWTIMER_STATUS_UNUSED = 0x0,
    HI_SWTIMER_STATUS_CREATED,    
    HI_SWTIMER_STATUS_TICKING,    
}HI_SWTIMER_STATUS_E;

/******************************************************************************
// ��������: HI_SYS_DFX_Init
// ��������:  ��ʼ������ά�������Ϣ��
				����Ρ����ݶΡ�BSS����Ϣ�Լ�ϵͳʣ���ڴ�ռ��ʹ�������
// ����˵��:
//   			��
//
// �� �� ֵ:
//    	������
//
// ����Ҫ��: 
				�˺���Ϊϵͳ����ʱ���ã������⿪�š�
******************************************************************************/
HI_EXTERN HI_U32  HI_SYS_DFX_Init(VOID);
HI_EXTERN HI_U32 HI_SYS_GetTimerNum(HI_OUT HI_U32* pNum);
HI_EXTERN HI_U32 HI_SYS_GetTimerInfo(HI_IN HI_U32 Num,HI_OUT HI_PVOID pTimerInfo);
HI_EXTERN HI_U32 HI_SYS_GetTaskNum(HI_OUT HI_U32* pNum);
HI_EXTERN HI_U32 HI_SYS_GetTaskInfo(HI_IN HI_U32 Num,HI_OUT HI_PVOID pTaskInfo);
HI_EXTERN HI_U32 SAL_OSExcHookAdd(HI_VOID);
HI_S32 HI_SYS_MemGetUsed(HI_VOID);
HI_S32  HI_SYS_MemGetTotal(HI_VOID);

HI_END_HEADER
#endif // __SAL_OS_DFX_H__


