//*****************************************************************************
//
//                  版权所有 (C), 2001-2011, 华为技术有限公司
//
//*****************************************************************************
//  文 件 名   : sal_m.h
//  版 本 号   : V1.0
//  作    者   : 沈汉坤/00130424
//  生成日期   : 2011年10月10日
//  功能描述   : SAL(系统适配层)接口定义: 
//               1) OS接口适配(Task、信号量、事件、队列、内存、定时器);
//               2) 驱动适配(UART、Flash、DSP、高精度Timer、Cipher、DMA、GPIO ...);
//               3) 系统初始化, 模块初始化, 包括DRV、ASL、MAC、DSP代码加载等的系统级别的初始化;
//               4) 资源管理(任务创建和启动、内存池、消息队列、信号量、事件);
//  函数列表   : 
//  修改历史   : 
//  1.日    期 : 2011年10月10日
//    作    者 : 沈汉坤/00130424
//    修改内容 : 创建文件
//
//*****************************************************************************

#ifndef __SAL_M_H__
#define __SAL_M_H__

HI_U32 sal_StartHRTimer(HI_OUT TIMER_HANDLE_T* phTimer, HI_PVOID pfnTimerRoutine, HI_U32 ulTimeOut, HI_U32 ulTimerId);
HI_U32 sal_StopHRTimer(TIMER_HANDLE_T* phTimer);


//*****************************************************************************
// 关闭/打开是所有中断, 以下接口慎用, 使用频繁的场景中不要使用!!!
// 仅供SAL内部使用!
//*****************************************************************************
#define SAL_LOCK0()                    HI_U32 __intlevel0 = HI_MDM_INT_Lock();
#define SAL_UNLOCK0()                  HI_MDM_INT_Restore(__intlevel0);
#define SAL_LOCK()                     { HI_U32 __intlevel = HI_MDM_INT_Lock();
#define SAL_UNLOCK()                   HI_MDM_INT_Restore(__intlevel);}
//*****************************************************************************

//*****************************************************************************
#if defined(SAL_HAVE_SIMU_WIN_VER)
HI_EXTERN HI_U32 SAL_CombineFilePath(HI_IN HI_CHAR szFileName[80], HI_OUT HI_CHAR szFilePath[256]);
HI_EXTERN HI_U32 SAL_GetIniCfgValueInt(HI_PCSTR pszSection, HI_PCSTR pszKeyName, HI_U32 ulDefaultValue);
#else
#define SAL_CombineFilePath(szFileName, szFilePath)                     (0) 
#define SAL_GetIniCfgValueInt(pszSection, pszKeyName, ulDefaultValue)   (0) 
#endif 
//*****************************************************************************

//*****************************************************************************
#if defined(PRODUCT_CFG_HAVE_FEATURE_SYS_ERR_INFO)
HI_EXTERN HI_VOID SAL_SaveSystemResetInfo(HI_U32 id);
#else
HI_PRVL HI_VOID SAL_SaveSystemResetInfo(HI_U32 id) {}
#endif
HI_EXTERN HI_BOOL diagGetSave2FlashRamSwtCfg(HI_VOID);
HI_EXTERN HI_BOOL diagGetRamDumpMsgPrintFlashSwtCfg(HI_VOID);
HI_EXTERN HI_U32 HI_SYS_DfxTaskTest(HI_PCSTR pszInitEntryName, HI_BOOL bStartTask, HI_U16 usId, HI_U32 ulOption);
HI_EXTERN HI_PVOID HI_SYS_GetDiagModeCache(HI_VOID);
HI_EXTERN HI_U16 HI_SYS_GetDiagModeCacheSize(HI_VOID);
//*****************************************************************************


//*****************************************************************************
HI_PRVL HI_BOOL IsMsgExceptionErrorCode(HI_U32 ret)
{
    switch (ret)
    {
        case HI_ERR_INVALID_PARAMETER: return HI_TRUE;
        case HI_ERR_SYSTEM_CALL_ERROR: return HI_TRUE;
        default: return HI_FALSE;
    }
}
//*****************************************************************************


//*****************************************************************************
#endif // __SAL_M_H__

