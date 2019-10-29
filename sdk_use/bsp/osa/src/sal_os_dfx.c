/*****************************************************************************/
//
//                  版权所有 (C), 2015-2018, 华为技术有限公司
//
/*****************************************************************************/
//  文 件 名   : sal_os_dfx.c
//  版 本 号   : V1.0
//  生成日期   : 2016-11-10
//  功能描述   : 本文件为系统可维可测封装接口,包括:
//				死机存储、
//				任务状态查看、
//				定时器状态查看、
//				消息队列、
//				内存信息查看。
/*****************************************************************************/
#include <los_memory.h>
#include <sal_os_dfx.h>
#include <los_task.h>
#include <los_swtmr.h>
#include <workqueue.h>
#include <drv_dfx.h>
#include <hi_stdlib.h>
#include <os_vx_sal_api.h>


extern SWTMR_CTRL_S *m_pstSwtmrCBArray;
extern SWTMR_CTRL_S *m_pstSwtmrSortList; 
extern char __rodata_start;
extern char __rom_vectors_vma;
extern char __rodata_end;
extern char __rodata1_start;
extern char __rodata1_end;
extern char __ram_data_start;
extern char __ram_data_end;
extern char __text_start;
extern char __text_end;
extern char __bss_start;
extern SENCOND_PAGE stOsPage;

HI_U32  HI_SYS_DFX_Init(VOID)
{
    MMU_PARAM mPara;

    //注册死机处理函数
    SAL_OSExcHookAdd();

#if 1//TODO:为了低功耗暂时屏蔽,后续需要打开。
    memset_s((HI_VOID *)&mPara, sizeof(MMU_PARAM), 0, sizeof(MMU_PARAM));
    mPara.startAddr = 0;
    mPara.endAddr = 0x1000;
    mPara.uwFlag = ACCESS_PERM_RO_RO|CACHE_ENABLE|BUFFER_ENABLE|FIRST_SECTION;
    mPara.stPage = (SENCOND_PAGE *)&stOsPage;
    LOS_MMUParamSet(&mPara);
#endif
    //保护页表(须放置配置最后，避免加保护后配置页表导致死机)、代码段、ro数据段    
    memset_s((HI_VOID *)&mPara, sizeof(MMU_PARAM), 0, sizeof(MMU_PARAM));
    mPara.startAddr = SDRAM_BASE;
    mPara.endAddr = (unsigned long)&__rodata1_end;
    mPara.uwFlag = ACCESS_PERM_RO_RO|CACHE_ENABLE|BUFFER_ENABLE|SECOND_PAGE;
    mPara.stPage = (SENCOND_PAGE *)&stOsPage;
    LOS_MMUParamSet(&mPara);

    //初始化内存池信息
    return HI_ERR_SUCCESS;
}

HI_S32 HI_SYS_MemGetUsed(void)
{
    return (HI_S32)LOS_MemTotalUsedGet(OS_SYS_MEM_ADDR);
}

HI_S32  HI_SYS_MemGetTotal(void)
{
    return (HI_S32)((LOS_MEM_POOL_INFO *)OS_SYS_MEM_ADDR)->uwPoolSize;
}

HI_PRV HI_U32 GetTimerInfo(HI_U16 usTmrID, DFX_OS_WD_INFO *pTimerInfo)
{
    SWTMR_CTRL_S *pstSwtmr = m_pstSwtmrSortList;
        
    if (pTimerInfo == NULL)
    {
        return HI_ERR_INVALID_PARAMETER;
    }

    while (pstSwtmr != NULL)
    {
        if ((usTmrID == (HI_U32)pstSwtmr->usTimerID) && (pstSwtmr->ucState != 0))
        {
            pTimerInfo->pHandle = (HI_U32)pstSwtmr;
            pTimerInfo->wdId = pstSwtmr->usTimerID,
            pTimerInfo->status = pstSwtmr->ucState,
            pTimerInfo->mode = pstSwtmr->ucMode,
            pTimerInfo->remainTicks = pstSwtmr->uwCount,
            pTimerInfo->wdParam = pstSwtmr->uwArg,
            pTimerInfo->wdRtn = (HI_U32 *)pstSwtmr->pfnHandler;

            return HI_ERR_SUCCESS;
        }
        pstSwtmr = pstSwtmr->pstNext;
    }
    return HI_ERR_NOT_FOUND;
}

HI_PRV HI_CHAR *ConvertTskStatus(HI_U16 usTaskStatus)
{
    if (OS_TASK_STATUS_RUNNING & usTaskStatus)
    {
        {
            return (HI_CHAR *)"Running";
        }
    }
    else if (OS_TASK_STATUS_READY &  usTaskStatus)
    {
        return (HI_CHAR *)"Ready";
    }
    else
    {
        if (OS_TASK_STATUS_PEND_SEM & usTaskStatus)
        {
            return (HI_CHAR *)"SemPend";
        }
        else if (OS_TASK_STATUS_PEND_MUT & usTaskStatus)
        {
            return (HI_CHAR *)"MutPend";
        }
        else if (OS_TASK_STATUS_PEND_QUEUE& usTaskStatus)
        {
            return (HI_CHAR *)"QueuePend";
        }
        else if (OS_TASK_STATUS_SWTMR_WAIT & usTaskStatus)
        {
            return (HI_CHAR *)"SwtmrWait";
        }
        else if (OS_TASK_STATUS_EVENT_READ & usTaskStatus)
        {
            return (HI_CHAR *)"EventRead";
        }
        else if (OS_TASK_STATUS_EVENT & usTaskStatus)
        {
            if (OS_TASK_STATUS_TIMEOUT & usTaskStatus)
            {
                return (HI_CHAR *)"EventTimeOut";
            }
            else
            {
                return (HI_CHAR *)"Event";
            }
        }
        else if (OS_TASK_STATUS_DELAY &  usTaskStatus)
        {
            return (HI_CHAR *)"Delay";
        }
        else if (OS_TASK_STATUS_TIMEOUT &  usTaskStatus)
        {
            if (OS_TASK_STATUS_PEND & usTaskStatus)
            {
                return (HI_CHAR *)"PendTimeOut";
            }
            else if (OS_TASK_STATUS_SUSPEND & usTaskStatus)
            {
                return (HI_CHAR *)"SuspendTimeOut";
            }
            return (HI_CHAR *)"TimeOut";
        }
        else if (OS_TASK_STATUS_PEND & usTaskStatus)
        {
            return (HI_CHAR *)"Pend";
        }
        else if (OS_TASK_STATUS_SUSPEND & usTaskStatus)
        {
            return (HI_CHAR *)"Suspend";
        }
    }

    return (HI_CHAR *)"Impossible";
}

HI_PRV HI_U32 ConvertTskInfo(DFX_OS_TASK_INFO *pTaskInfo, TSK_INFO_S *pstTaskCB)
{
    strncpy_s(pTaskInfo->name, sizeof(pTaskInfo->name), pstTaskCB->acName, sizeof(pTaskInfo->name) - 1);
    pTaskInfo->id = (HI_U32*)(pstTaskCB->uwTaskID);
    pTaskInfo->pri = pstTaskCB->usTaskPrio;
    strncpy_s(pTaskInfo->status, sizeof(pTaskInfo->status), ConvertTskStatus(pstTaskCB->usTaskStatus), (sizeof(pTaskInfo->status) - 1));
    pTaskInfo->usedmem = osTaskMemUsage(pstTaskCB->uwTaskID);
    pTaskInfo->sp = pstTaskCB->uwSP;
    if (pstTaskCB->pTaskSem != NULL)
    {
        pTaskInfo->semID = ((SEM_CB_S *)pstTaskCB->pTaskSem)->usSemID;
    }
    else
    {
        pTaskInfo->semID = 0xFFFF;
    }
    pTaskInfo->pc = pstTaskCB->uwPC;
    pTaskInfo->lr = pstTaskCB->uwLR;
    pTaskInfo->cpsr = pstTaskCB->uwPSR;
    
    pTaskInfo->evtmask = pstTaskCB->uwEventMask;
    pTaskInfo->stack.end = pstTaskCB->uwTopOfStack;
    pTaskInfo->stack.base = pstTaskCB->uwBottomOfStack;
    pTaskInfo->stack.high = pstTaskCB->uwPeakUsed;
    pTaskInfo->stack.size = pstTaskCB->uwStackSize;
    pTaskInfo->stack.cur = pstTaskCB->uwCurrUsed;
    pTaskInfo->stack.margin = pTaskInfo->stack.size - pTaskInfo->stack.cur;
    
    return HI_ERR_SUCCESS;
}

HI_U32 HI_SYS_GetTimerNum(HI_U32* pNum)
{
    SWTMR_CTRL_S *pstSwtmr = m_pstSwtmrSortList;
    HI_U32 timer_count = 0;

    if (pNum == HI_NULL)
    {
        return HI_ERR_INVALID_PARAMETER;
    }

    while (pstSwtmr != NULL)
    {
        if (pstSwtmr->ucState != HI_SWTIMER_STATUS_UNUSED)
        {
            timer_count ++;
        }
        pstSwtmr = pstSwtmr->pstNext;
    }
    *pNum = timer_count;
    
    return HI_ERR_SUCCESS;
}


HI_U32 HI_SYS_GetTimerInfo(HI_IN HI_U32 Num, HI_PVOID pTimerInfo)
{
    SWTMR_CTRL_S *pstSwtmr = m_pstSwtmrSortList;
    DFX_OS_WD_INFO *pInfoGet = (DFX_OS_WD_INFO *)pTimerInfo;
    HI_U32 timer_count = 0;
    HI_U32 ret;

    if ((pTimerInfo == NULL)||(Num == 0))
    {
        return HI_ERR_INVALID_PARAMETER;
    }

    while (pstSwtmr != NULL && timer_count <= g_uwTskMaxNum)
    {
        if (pstSwtmr->ucState != HI_SWTIMER_STATUS_UNUSED)
        {
            ret = GetTimerInfo(pstSwtmr->usTimerID, pInfoGet);
            if (HI_ERR_SUCCESS != ret)
            {
                //错误统计
                continue;
            }
            
            /* increase the record pointer */
            pInfoGet ++;
        }
        pstSwtmr = pstSwtmr->pstNext;
        timer_count ++;
        if (timer_count >= Num)
        {
            break;
        }
    }

    return HI_ERR_SUCCESS;
}

HI_U32 HI_SYS_GetTaskNum(HI_U32* pNum)
{
    LOS_TASK_CB *pstTaskCB = HI_NULL;
    HI_U32 task_count = 0;
    HI_U32 i;

    if (pNum == HI_NULL)
    {
        return HI_ERR_INVALID_PARAMETER;
    }

    for (i = 0; i < g_uwTskMaxNum; i++, pstTaskCB++)
    {
        pstTaskCB = ((LOS_TASK_CB *)g_pstTaskCBArray + i);
        if ((pstTaskCB->usTaskStatus & OS_TASK_STATUS_UNUSED)||(pstTaskCB->pcTaskName == NULL))
        {
            continue;
        }
        task_count ++;
    }
    *pNum = task_count;
    return HI_ERR_SUCCESS;  
}


HI_U32 HI_SYS_GetTaskInfo(HI_U32 Num, HI_PVOID pTaskInfo)
{
    LOS_TASK_CB *pstTaskCB = HI_NULL;
    DFX_OS_TASK_INFO *pInfoGet = (DFX_OS_TASK_INFO *)pTaskInfo;
    TSK_INFO_S stTaskInfo = {{0}, 0};
    HI_U32 ret;
    HI_U32 i;
        
    if ((pTaskInfo == NULL)||(Num == 0))
    {
        PRINTK("SAL_GetTaskInfo Para Err.\r\n");
        return HI_ERR_INVALID_PARAMETER;
    }

    for (i = 0; i<g_uwTskMaxNum; i++)
    {
        pstTaskCB = ((LOS_TASK_CB *)g_pstTaskCBArray + i);
        ret = LOS_TaskInfoGet(pstTaskCB->uwTaskID, &stTaskInfo);
        if (HI_ERR_SUCCESS != ret)
        {
            PRINTK("LOS_TaskInfoGet Err:%d Pcb:%x\r\n",ret, &stTaskInfo);
            continue;                
        }
        ret = ConvertTskInfo(pInfoGet, &stTaskInfo);
        if (HI_ERR_SUCCESS != ret)
        {
            PRINTK("ConvertTskInfo Err:%d Pcb:%x\r\n",ret, &stTaskInfo);
            continue;
        }
        pInfoGet ++;
    }
      
    return HI_ERR_SUCCESS;
}

HI_U32 SAL_WDG_STORE_INIT (HI_VOID)
{
	return HI_ERR_NOT_SUPPORT;
}



