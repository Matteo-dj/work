//*****************************************************************************
//
//                  ��Ȩ���� (C), 1998-2009, ��Ϊ�������޹�˾
//
//*****************************************************************************
//  �� �� ��   : sal_os_api.c
//  �� �� ��   : V1.0
//  ��    ��   : ����/00130424
//  ��������   : 2009-04-17
//  ��������   : TODO: ...
//
//  �����б�   : SAL_StartTimer��SAL_StopTimer 
//  �޸���ʷ   :
//  1.��    �� : 2009-04-17
//    ��    �� : ����/00130424
//    �޸����� : �����ļ�
//  2.��    �� : 2009-05-12
//    ��    �� : ���ķ�/00149739
//    �޸����� : ���Ӻ���DO_GetCurrentTaskQueueId
//               �޸ĺ��� ODA_IsMessageAvailable ������ѯ�����Ƿ���Ч
//  3.��    �� : 2009-07-04
//    ��    �� : ���ķ�/00149739
//    �޸����� : ����g_aulODAHighMsgTable
//               �޸ĺ��� ODA_IsHiPriMsg ����ѯ�Ƿ��Ǹ����ȼ���Ϣ
//               Ŀǰʹ�õĲ��������Ż���ʽ
//  4.��    �� : 20011��10��14��
//    ��    �� : ����/00130424
//    �޸����� : ��������ΪHi3911��Ŀ���� 
//
//*****************************************************************************

//*****************************************************************************
// PROJECT   :
// SUBSYSTEM : SAL 
// MODULE    : SAL 
// OWNER     : 
//*****************************************************************************

#include "sal_common.h"
#ifdef PRODUCT_CFG_PLATFORM_HI3911
#include <hi_driver_internal.h>
#endif 

HI_U32 HI_SYS_Reboot(HI_SYS_REBOOT_CAUSE_E enCause)
{
    HI_U32 ret = HI_ERR_UNAVAILABLE;
    if((enCause <= HI_SYS_REBOOT_CAUSE_UNKNOWN) || (enCause >= HI_SYS_REBOOT_CAUSE_MAX))
    {
        ret = HI_ERR_INVALID_PARAMETER;
    }
    else
    {
        ret = HI_ERR_UNAVAILABLE;
        HI_SYS_DisableMMU();
        HI_SYS_RebootPrv((HI_U32)enCause);
        /*lint -e527*/
        HI_SYS_NEVER_RETURN();
        /*lint +e527*/

    }
    return ret;
}

HI_U32 SAL_StartHRTimerPrv(HI_OUT TIMER_HANDLE_T* phTimer, HI_PVOID pfnTimerRoutine, HI_U32 ulTimeOut)
{
    #ifdef PRODUCT_CFG_OS_WIN
    SAL_NEW_TIMER(phTimer, HI_NULL, pfnTimerRoutine, ulTimeOut, HI_SYS_TIMER_ONESHOT, HI_TRUE);
    return phTimer->data[0] ? HI_ERR_SUCCESS : HI_ERR_SYSTEM_CALL_ERROR;
    #else
    #if defined(PRODUCT_CFG_PLATFORM_HI3911) && defined(PRODUCT_CFG_FEATURE_HRTIMER)
    if(HI_IS_VALID_RAM_ADDR(phTimer))
    {
        HI_HRTIMER_TYPE_S* pstTimer = (HI_HRTIMER_TYPE_S*)phTimer;    
	
        // ADDED by s00130424 2012/12/13   ���ⵥ��: DTS2012120603282
	    // ����ͳ����
	    SAL_LOCK0();
        pstTimer->expires = ulTimeOut;
        pstTimer->func = (HI_TIMER_CALLBACK_F)pfnTimerRoutine;
        pstTimer->data = 0;
        HI_HRTIMER_Start(pstTimer);
        SAL_UNLOCK0();
        return HI_ERR_SUCCESS; // �����ӿ�û�з���ֵ��Ĭ�ϳɹ�
    }
    else 
    {
        return HI_ERR_INVALID_PARAMETER;
    }
    #else
        return HI_ERR_NOT_SUPPORT;
    #endif
    #endif
}

#ifdef PRODUCT_CFG_PRODUCT_TYPE_NDM
HI_U32 SAL_StartFastHrTimer(TIMER_HANDLE_T* pTimeHandle, HI_PVOID pfnTimerRoutine,HI_PVOID pfnLisrTimerRoutine, HI_U32 ulTimeOut,HI_U32 usr_data)
{

    HI_HRTIMER_TYPE_S *pstTimer = (HI_HRTIMER_TYPE_S *)pTimeHandle;
    if(pTimeHandle==HI_NULL||pfnTimerRoutine==HI_NULL||pfnLisrTimerRoutine==HI_NULL)
    {
        return HI_ERR_INVALID_PARAMETER;
    }
    SAL_LOCK0();
    pstTimer->expires = ulTimeOut;
    pstTimer->func = (HI_TIMER_CALLBACK_F)pfnTimerRoutine;
    pstTimer->data = usr_data;    
    pstTimer->attribute = HI_FAST_TIMER_MAGIC;
    pstTimer->funcLisr = pfnLisrTimerRoutine;
    HI_HRTIMER_Start(pstTimer);
    SAL_UNLOCK0();
    return HI_ERR_SUCCESS;
}
#endif

//****************************************************************************
// �߾��ȶ�ʱ��ֹͣ
//****************************************************************************
HI_U32 SAL_StopHRTimerPrv(HI_OUT TIMER_HANDLE_T* phTimer)
{
    #ifdef PRODUCT_CFG_OS_WIN
    SAL_KILL_TIMER(phTimer);
    return HI_ERR_SYSTEM_CALL_ERROR;
    #else
    #if defined(PRODUCT_CFG_PLATFORM_HI3911) && defined(PRODUCT_CFG_FEATURE_HRTIMER)
    if(HI_IS_VALID_RAM_ADDR(phTimer))
    {
        HI_HRTIMER_TYPE_S* pstTimer = (HI_HRTIMER_TYPE_S*)phTimer;
		
	    // ADDED by s00130424 2012/12/13   ���ⵥ��: DTS2012120603282
    	// ����ͳ���� 
    	SAL_LOCK0();
        SAL_INC_STAT_SYS_QRY_U32(stOsRes, aulHrTimerStat[1]);
        HI_HRTIMER_Stop(pstTimer);
        memset_s(phTimer,sizeof( TIMER_HANDLE_T), 0,sizeof( TIMER_HANDLE_T));
        SAL_UNLOCK0();
        return HI_ERR_SUCCESS; // �����ӿ�û�з���ֵ��Ĭ�ϳɹ�
    }
    else 
    {
        return HI_ERR_INVALID_RAM_ADDR;
    }
    #else
        return HI_ERR_NOT_SUPPORT;
    #endif 
    #endif    
}

HI_U32 SAL_StartHRTimer(HI_OUT TIMER_HANDLE_T* phTimer, HI_PVOID pfnTimerRoutine, HI_U32 ulTimeOut)
{
    if(!HI_IS_VALID_CODE_ADDR(pfnTimerRoutine))
    {
        return HI_ERR_INVALID_CODE_ADDR;
    }
    if(!HI_IS_VALID_RAM_ADDR(phTimer))
    {
        return HI_ERR_INVALID_RAM_ADDR;
    }
    return SAL_StartHRTimerPrv(phTimer, pfnTimerRoutine, ulTimeOut);
}

HI_U32 SAL_StopHRTimer(HI_INOUT TIMER_HANDLE_T* phTimer)
{
    if(!HI_IS_VALID_RAM_ADDR(phTimer))
    {
        return HI_ERR_INVALID_RAM_ADDR;
    }
    return SAL_StopHRTimerPrv(phTimer);
}




//*****************************************************************************
// ���� HR Timer ���ڲ�ʹ��
//*****************************************************************************
HI_U32 sal_StartHRTimer(HI_OUT TIMER_HANDLE_T* phTimer, HI_PVOID pfnTimerRoutine, HI_U32 ulTimeOut, HI_U32 ulTimerId)
{
    #ifdef PRODUCT_CFG_OS_WIN
    return sal_StartOSTimer(phTimer, HI_NULL, (HI_PVOID)pfnTimerRoutine, ulTimeOut, HI_SYS_TIMER_ONESHOT, ulTimerId);
    #else
    #if defined(PRODUCT_CFG_PLATFORM_HI3911) && defined(PRODUCT_CFG_FEATURE_HRTIMER)
    if(!HI_IS_VALID_CODE_ADDR(pfnTimerRoutine))
    {
        return HI_ERR_INVALID_CODE_ADDR;
    }
    else if(HI_IS_VALID_RAM_ADDR(phTimer))
    {
        HI_HRTIMER_TYPE_S* pstTimer = (HI_HRTIMER_TYPE_S*)phTimer;	
        pstTimer->expires = ulTimeOut;
        pstTimer->func   = (HI_TIMER_CALLBACK_F)pfnTimerRoutine;
        pstTimer->data   = ulTimerId;
        HI_HRTIMER_Start(pstTimer);
        return HI_ERR_SUCCESS; // �����ӿ�û�з���ֵ��Ĭ�ϳɹ�
    }
    else 
    {
        return HI_ERR_INVALID_RAM_ADDR;
    }
    #else
        return HI_ERR_NOT_SUPPORT;
    #endif
    #endif
}
