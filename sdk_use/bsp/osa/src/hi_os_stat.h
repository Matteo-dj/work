
#ifndef __SAL_OS_STAT_H__
#define __SAL_OS_STAT_H__
#include <hi_types.h>
#include <hi_mdm_types.h>

#if defined(PRODUCT_CFG_OSA_STAT)
#define HI_OSA_ERR_STAT_PTR()     	((HI_OSA_ERR_S*)(&g_stOsaErr[0]))
#define HI_OSA_ERR_STAT_GET(m)     	HI_OSA_ERR_STAT_PTR()->m
#define HI_OSA_ERR_STAT_SET(m,val)   memcpy_s(HI_OSA_ERR_STAT_PTR()->m,sizeof(HI_OSA_ERR_STAT_PTR()->m),val, sizeof(HI_OSA_ERR_STAT_PTR()->m))
#define HI_OSA_ERR_STAT_INC(m)       HI_OSA_ERR_STAT_PTR()->m++
#else
#define HI_OSA_ERR_STAT_PTR()     	0
#define HI_OSA_ERR_STAT_GET(m)     	1
#define HI_OSA_ERR_STAT_SET(m,val)  
#define HI_OSA_ERR_STAT_INC(m)      
#endif

typedef struct
{
    HI_U8 ulTimerErr[4];    /*0x0*/
	HI_U32 ulTimerCreateErr;
	HI_U32 ulTimerStartErr;
	HI_U32 ulTimerDeleteErr;
	HI_U8 ulTaskErr[16];
	HI_U32 ulTaskCreateErr;
	HI_U32 ulTaskDeleteErr;
	HI_U32 ulTaskSuspendErr;
    HI_U32 ulTaskResumeErr;
    HI_U32 ulTaskSetPriErr;
    HI_U32 ulTaskGetInfoErr;
    HI_U32 ulTaskRegCpupHookErr;
    
    HI_U32 ulNewTimerLo;
    HI_U32 ulNewTimer2Hi;
    HI_U32 ulNewTimer2Lo;
    HI_U32 ulKillTimerHi;   /*0x10*/
    HI_U32 ulKillTimerLo;
    HI_U32 ulHeapCreate;
    HI_U32 ulHeapDel;
    HI_U32 ulHeapAlloc;/*0x20*/
    HI_U32 ulHeapFree;
    HI_U32 ulWaitEvent;
    HI_U32 ulSetEventHi;
    HI_U32 ulSetEventLo; /*0x30*/
    HI_U32 ulCreateSem;
    HI_U32 ulDelSem;
    HI_U32 ulReleaseSem;
    HI_U32 ulWaitSem;    /*0x40*/
    HI_U32 ulNewQu;
    HI_U32 ulSendQu;
    HI_U32 ulRcvQuHi;
    HI_U32 ulRcvQuLo;/*0x50*/
    HI_U32 ulQuFull;
    HI_U32 ulQuMax;
   
    HI_U32 ulClearEventHi;/*0x60*/
    HI_U32 ulClearEventLo;
}HI_OSA_ERR_S;

HI_EXTERN HI_OSA_ERR_S g_stOsaErr;
#endif

