//*****************************************************************************
//
//                  版权所有 (C), 2015-2018, 华为技术有限公司
//
/*****************************************************************************
 *  文 件 名   : sal_os_dfx.h
 *  版 本 号   : V1.0
 *  生成日期   : 2016-11-10
 *  功能描述   : 本文件为系统可维可测封装接口适配文件,包括:
 *              死机存储、
 *              任务状态查看、
 *              定时器状态查看、
 *              消息队列、
 *              内存信息查看。
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
// 函数名称: HI_SYS_DFX_Init
// 功能描述:  初始化所有维测相关信息。
				代码段、数据段、BSS段信息以及系统剩余内存空间的使用情况。
// 参数说明:
//   			无
//
// 返 回 值:
//    	错误码
//
// 调用要求: 
				此函数为系统启动时调用，不对外开放。
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


