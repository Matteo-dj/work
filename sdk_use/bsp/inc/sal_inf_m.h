//*****************************************************************************
//
//                  ��Ȩ���� (C), 2001-2011, ��Ϊ�������޹�˾
//
//*****************************************************************************
//  �� �� ��   : sal_m.h
//  �� �� ��   : V1.0
//  ��    ��   : ����/00130424
//  ��������   : 2011��10��10��
//  ��������   : SAL(ϵͳ�����)�ӿڶ���: 
//               1) OS�ӿ�����(Task���ź������¼������С��ڴ桢��ʱ��);
//               2) ��������(UART��Flash��DSP���߾���Timer��Cipher��DMA��GPIO ...);
//               3) ϵͳ��ʼ��, ģ���ʼ��, ����DRV��ASL��MAC��DSP������صȵ�ϵͳ����ĳ�ʼ��;
//               4) ��Դ����(���񴴽����������ڴ�ء���Ϣ���С��ź������¼�);
//  �����б�   : 
//  �޸���ʷ   : 
//  1.��    �� : 2011��10��10��
//    ��    �� : ����/00130424
//    �޸����� : �����ļ�
//
//*****************************************************************************

#ifndef __SAL_M_H__
#define __SAL_M_H__

HI_U32 sal_StartHRTimer(HI_OUT TIMER_HANDLE_T* phTimer, HI_PVOID pfnTimerRoutine, HI_U32 ulTimeOut, HI_U32 ulTimerId);
HI_U32 sal_StopHRTimer(TIMER_HANDLE_T* phTimer);


//*****************************************************************************
// �ر�/���������ж�, ���½ӿ�����, ʹ��Ƶ���ĳ����в�Ҫʹ��!!!
// ����SAL�ڲ�ʹ��!
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

