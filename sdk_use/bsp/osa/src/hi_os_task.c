#include <hi_types.h>
#include <hi_mdm_types.h>
#include <los_task.h>
#include <hi_mdm_task.h>
#include "hi_os_stat.h"
#include <hi_stdlib.h>
#include "hi_os_sys.h"
#include <hi_driver_internal.h>



HI_U32 HI_MDM_TASK_CreateEx (HI_OUT HI_U32* pTaskId, HI_CHAR szName[32], TSK_BODY pfnTaskBody, HI_U32  ulParam, HI_U32 ulStackSize, HI_U32 ulPriority,HI_U32 ulOption)
{
   
    unsigned int tid = 0;
    TSK_INIT_PARAM_S my_task = {0,};
    HI_U32 ret = LOS_OK;

    /*内核接口对输入参数有判断，此处省略*/
    
    if (HI_NULL == pTaskId)
    {
        HI_OSA_ERR_STAT_INC(ulTaskErr[0]);
        return HI_ERR_TASK_INVALID_PARAM;
    }

    my_task.pcName = szName;
    my_task.pfnTaskEntry = (TSK_ENTRY_FUNC)pfnTaskBody;
    my_task.uwStackSize = ulStackSize;
    my_task.usTaskPrio = (HI_U16)ulPriority;
    my_task.auwArgs[0] = ulParam;
    my_task.uwResved = ulOption;


    ret = LOS_TaskCreate(&tid, &my_task);

    if (LOS_OK == ret)
    {
        *pTaskId = tid;
        return HI_ERR_SUCCESS;
    }
    else
    {
        HI_OSA_ERR_STAT_INC(ulTaskErr[1]);
        HI_OSA_ERR_STAT_SET(ulTaskCreateErr, ret);
        PRINTK("[task_name=%s][pfnTaskBody=0x%x][pParam=0x%x][ulStackSize=0x%x][ucPriority=%d]"
               , szName, pfnTaskBody, ulParam, ulStackSize, ulPriority);
        return HI_ERR_TASK_CREATE_FAIL;
    }
}
HI_U32 HI_MDM_TASK_Create (HI_OUT HI_U32* pTaskId, HI_CHAR szName[32], TSK_BODY pfnTaskBody, HI_U32  ulParam, HI_U32 ulStackSize, HI_U32 ulPriority)
{
    return HI_MDM_TASK_CreateEx(pTaskId, szName, pfnTaskBody,ulParam, ulStackSize, ulPriority, 0);
}

HI_U32 HI_MDM_TASK_Delete (HI_U32 ulTaskId)
{

    HI_U32 ret = LOS_OK;
    ret = LOS_TaskDelete(ulTaskId);

    if (ret != LOS_OK)
    {
        HI_OSA_ERR_STAT_INC(ulTaskErr[2]);
        HI_OSA_ERR_STAT_SET(ulTaskDeleteErr, ret);
        return HI_ERR_TASK_DELETE_FAIL;
    }

    return HI_ERR_SUCCESS;

}


HI_U32 HI_MDM_TASK_Suspend (HI_U32 ulTaskId)
{
    HI_U32 ret = LOS_OK;
    ret = LOS_TaskSuspend(ulTaskId);

    if (ret != LOS_OK)
    {
        HI_OSA_ERR_STAT_INC(ulTaskErr[3]);
        HI_OSA_ERR_STAT_SET(ulTaskSuspendErr, ret);
        return HI_ERR_TASK_SUPPEND_FAIL;
    }

    return HI_ERR_SUCCESS;
}

HI_U32 HI_MDM_TASK_Resume (HI_U32 ulTaskId)
{

    HI_U32 ret = LOS_OK;
    ret = LOS_TaskResume(ulTaskId);

    if (ret != LOS_OK)
    {
        HI_OSA_ERR_STAT_INC(ulTaskErr[4]);
        HI_OSA_ERR_STAT_SET(ulTaskResumeErr, ret);
        return HI_ERR_TASK_RESUME_FAIL;
    }

    return HI_ERR_SUCCESS;
}


HI_U32 HI_MDM_TASK_GetPriority (HI_U32 ulTaskId, HI_OUT HI_U32* pPriority)
{
    HI_U16 ulPri = 0;

    if (pPriority == HI_NULL)
    {
        HI_OSA_ERR_STAT_INC(ulTaskErr[5]);
        return HI_ERR_TASK_INVALID_PARAM;
    }

    ulPri = LOS_TaskPriGet(ulTaskId);

    if (ulPri == (HI_U16)OS_INVALID)
    {
        HI_OSA_ERR_STAT_INC(ulTaskErr[6]);

        return HI_ERR_TASK_GET_PRI_FAIL;
    }

    *pPriority = ulPri;
    return HI_ERR_SUCCESS;

}

HI_U32 HI_MDM_TASK_SetPriority(HI_U32 ulTaskId, HI_U32 ulPriority)
{
    HI_U32 ret = LOS_OK;
    ret = LOS_TaskPriSet(ulTaskId, (HI_U16)ulPriority);

    if (ret != LOS_OK)
    {
        HI_OSA_ERR_STAT_INC(ulTaskErr[7]);
        HI_OSA_ERR_STAT_SET(ulTaskSetPriErr, ret);
        return HI_ERR_TASK_SET_PRI_FAIL;
    }

    return HI_ERR_SUCCESS;


}

//HI_EXTERN HI_U16 g_usLosTaskLock; 间接包含到了los_task.ph,删除该extern
HI_BOOL HI_MDM_TASK_IsLock(HI_VOID)
{
    if(g_usLosTaskLock)
    {
        return HI_TRUE;
    }
    return HI_FALSE;
}

HI_VOID HI_MDM_TASK_Lock(HI_VOID)
{
    LOS_TaskLock();
}

HI_VOID HI_MDM_TASK_UnLock(HI_VOID)
{
    LOS_TaskUnlock();
}

HI_U32 HI_MDM_Sleep(HI_U32 ulMs)
{
    HI_U32 ret = LOS_OK;
    HI_U32 ulTick = ms2systick(ulMs, HI_FALSE);
 
    ret = LOS_TaskDelay(ulTick);
    if (ret != LOS_OK)
    {
        HI_OSA_ERR_STAT_INC(ulTaskErr[8]);
        HI_OSA_ERR_STAT_SET(ulTaskSleepErr, ret);
        return HI_ERR_TASK_DELAY_FAIL;
    }

    return HI_ERR_SUCCESS;
}

HI_U32 HI_MDM_TASK_GetCurrentID(HI_VOID)
{
    HI_U32 ulTaskId = HI_INVALID_TASK_ID;
    ulTaskId = LOS_CurTaskIDGet();

    if (ulTaskId == LOS_ERRNO_TSK_ID_INVALID)
    {
        HI_OSA_ERR_STAT_INC(ulTaskErr[9]);
        return HI_INVALID_TASK_ID;
    }

    return ulTaskId;
}

HI_U32 HI_MDM_TASK_GetTaskInfo(HI_U32 ulTaskId, HI_TASK_INFO_S* pstTaskInfo) //维测用结构体变化可能性较大
{
    HI_U32 ret = LOS_OK;
    TSK_INFO_S stTaskInfo = {{0}, 0};

    /*内核接口对输入参数有判断，此处省略*/
    ret = LOS_TaskInfoGet(ulTaskId, &stTaskInfo);
    if (ret != LOS_OK)
    {
        HI_OSA_ERR_STAT_INC(ulTaskErr[10]);
        HI_OSA_ERR_STAT_SET(ulTaskGetInfoErr, ret);
        return HI_ERR_TASK_GET_INFO_FAIL;
    }

    (HI_VOID)memcpy_s(pstTaskInfo, sizeof(HI_TASK_INFO_S), &stTaskInfo, sizeof(HI_TASK_INFO_S));
    return HI_ERR_SUCCESS;
}

HI_U32 HI_MDM_TASK_RegisterSchedulingTrack(TASK_SCHEDULE_FUNC pfnHook)
{
    HI_U32 ret = LOS_OK;
    
    //if (pfnHook == HI_NULL) 函数空为功能去注册
    //{
        //HI_OSA_ERR_STAT_INC(ulTaskErr[11]);
        //return HI_ERR_TASK_INVALID_PARAM;
    //}

    /*函数参数类型需要内外保持一致*/
    ret = LOS_RegCpupHook((USERCPUPHOOK)pfnHook);
    if (ret != LOS_OK)
    {
        HI_OSA_ERR_STAT_INC(ulTaskErr[12]);
        HI_OSA_ERR_STAT_SET(ulTaskRegCpupHookErr, ret);
        return HI_ERR_TASK_REGISTER_SCHEDULE_FAIL;
    }

    return HI_ERR_SUCCESS;
}

HI_U32 HI_MDM_TASK_GetTaskId(HI_CHAR* pcName, HI_U32 *pulTaskId)
{
	LOS_TASK_CB *pstTaskCB = HI_NULL;
	TSK_INFO_S stTaskInfo;
	HI_U32 ret;
	HI_U32 ulLen;
	HI_U32 i;
		
	if ((pcName == HI_NULL)||(pulTaskId == HI_NULL))
	{
		return HI_ERR_INVALID_PARAMETER;
	}

	for (i = 0; i<g_uwTskMaxNum; i++)
	{
		pstTaskCB = ((LOS_TASK_CB *)g_pstTaskCBArray + i);
		ret = LOS_TaskInfoGet(pstTaskCB->uwTaskID, &stTaskInfo);
		if (HI_ERR_SUCCESS != ret)
		{
			continue;				 
		}
		ulLen = strlen(pstTaskCB->pcTaskName);
		if ((memcmp(pstTaskCB->pcTaskName, pcName, ulLen))||(ulLen == 0))
		{
			continue;
		}
		else
		{
			*pulTaskId = pstTaskCB->uwTaskID;
			return HI_ERR_SUCCESS;
		}
	}
	  
	return HI_ERR_NOT_FOUND;
}


