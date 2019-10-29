//*****************************************************************************
//
//                  版权所有 (C), 1998-2009, 华为技术有限公司
//
//*****************************************************************************
//  文 件 名   : sal_os_api.c
//  版 本 号   : V1.0
//  作    者   : 沈汉坤/00130424
//  生成日期   : 2009-04-17
//  功能描述   : TODO: ...
//
//  函数列表   : SAL_StartTimer、SAL_StopTimer 
//  修改历史   :
//  1.日    期 : 2009-04-17
//    作    者 : 沈汉坤/00130424
//    修改内容 : 创建文件
//  2.日    期 : 2009-05-12
//    作    者 : 陈文峰/00149739
//    修改内容 : 增加函数DO_GetCurrentTaskQueueId
//               修改函数 ODA_IsMessageAvailable 用来查询队列是否有效
//  3.日    期 : 2009-07-04
//    作    者 : 陈文峰/00149739
//    修改内容 : 增加g_aulODAHighMsgTable
//               修改函数 ODA_IsHiPriMsg 来查询是否是高优先级消息
//               目前使用的并非是最优化方式
//  4.日    期 : 20011年10月14日
//    作    者 : 沈汉坤/00130424
//    修改内容 : 调整适配为Hi3911项目需求 
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
	
        // ADDED by s00130424 2012/12/13   问题单号: DTS2012120603282
	    // 增加统计量
	    SAL_LOCK0();
        pstTimer->expires = ulTimeOut;
        pstTimer->func = (HI_TIMER_CALLBACK_F)pfnTimerRoutine;
        pstTimer->data = 0;
        HI_HRTIMER_Start(pstTimer);
        SAL_UNLOCK0();
        return HI_ERR_SUCCESS; // 驱动接口没有返回值，默认成功
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
// 高精度定时器停止
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
		
	    // ADDED by s00130424 2012/12/13   问题单号: DTS2012120603282
    	// 增加统计量 
    	SAL_LOCK0();
        SAL_INC_STAT_SYS_QRY_U32(stOsRes, aulHrTimerStat[1]);
        HI_HRTIMER_Stop(pstTimer);
        memset_s(phTimer,sizeof( TIMER_HANDLE_T), 0,sizeof( TIMER_HANDLE_T));
        SAL_UNLOCK0();
        return HI_ERR_SUCCESS; // 驱动接口没有返回值，默认成功
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
// 启动 HR Timer 仅内部使用
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
        return HI_ERR_SUCCESS; // 驱动接口没有返回值，默认成功
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
