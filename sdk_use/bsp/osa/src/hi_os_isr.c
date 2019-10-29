
#include <hi_types.h>
#include <hi_mdm_types.h>
#include <hi_mdm.h>
#include <los_hwi.h >
#include "hi_os_stat.h"
#include <hi_mdm_isr.h>
#include <workqueue.h>//g_pstSystemWq
#include <hi_asm_func.h>
#include <los_trace.h>
#include <errno.h>
#include <hi_sys_selfrepair.h>
//#include <hal_platform_ints.h>//hal_interrupt_unmask
HI_U32 HI_MDM_INT_LockTmp(HI_U32 lr)
{
    HI_U32 ulSaveCpsr=LOS_IntLock();
    los_trace_irq((HI_U32)ENABLE_IRQ,lr);
    return ulSaveCpsr;
}
HI_VOID HI_MDM_INT_RestoreTmp(HI_U32 lr,HI_U32 ulIntSave)
{
    los_trace_irq((HI_U32)DISABLE_IRQ,lr);
    LOS_IntRestore(ulIntSave);    
}


HI_U32 HI_MDM_INT_Lock (HI_VOID)
{
    HI_U32 lr=HI_ASM_GetLr();
    HI_U32 ulSaveCpsr=LOS_IntLock();
    los_trace_irq((HI_U32)ENABLE_IRQ,lr);
    return ulSaveCpsr;
}
HI_VOID HI_MDM_INT_Restore (HI_U32 ulIntSave)
{
    HI_U32 lr=HI_ASM_GetLr();
    los_trace_irq((HI_U32)DISABLE_IRQ,lr);
    LOS_IntRestore(ulIntSave);    
}

HI_U32 HI_MDM_IRQ_Enable(HI_U32 ulVector)
{
    hal_interrupt_unmask(ulVector);
    return HI_ERR_SUCCESS;
}
HI_U32 HI_MDM_IRQ_Disable(HI_U32 ulVector)
{
    hal_interrupt_mask(ulVector);
    return HI_ERR_SUCCESS;
}

HI_U32 HI_MDM_IRQ_Request (HI_U32 ulVector,HI_U16 ulPriority,HI_PVOID pIrqRoutine,HI_U32 ulPara)
{
    HI_U32 ret = LOS_OK;
    if(ulVector>=OS_HWI_MAX_NUM||pIrqRoutine==HI_NULL)
    {
        return HI_ERR_ISR_INVALID_PARAM;
    }

    ret = LOS_HwiCreate(ulVector, 0, 0, (HWI_PROC_FUNC)pIrqRoutine, ulPara);
    if(ret!=LOS_OK&&ret!=OS_ERRNO_HWI_ALREADY_CREATED)
    {
        return HI_ERR_ISR_REQ_IRQ_FAIL;
    }
    return HI_ERR_SUCCESS;
}
HI_U32 HI_MDM_IRQ_Free (HI_U32 ulVector)
{
    LOS_HwiDelete(ulVector,0);
    return HI_ERR_SUCCESS;
}

VOID irq_bottom_half_handler(struct work_struct * work)
{
    HISR_PROC_FUNC f;

    f = (HISR_PROC_FUNC)work->data[0];
    f((HI_U32)work->data[1], (HI_U32)work->data[2], (HI_U32)work->data[3], 
    (HI_U32)work->data[4],(HI_U32)work->data[5]);
}

unsigned int g_drop_job_cnt = 0;
HI_U32 HI_MDM_IRQ_AddJob(HI_U32 ulPriority, HISR_PROC_FUNC pfnHisrFunc,HI_U32 ulData1,HI_U32 ulData2,HI_U32 ulData3,HI_U32 ulData4,HI_U32 ulData5)
{
    struct work_struct work;
    HI_S32 ret = 0;

	if ((HI_NULL == pfnHisrFunc)||(ulPriority > WORKQ_IRQ_DEFAULT_PRI))
	{
		return HI_ERR_ISR_INVALID_PARAM;
	}

    work.func = irq_bottom_half_handler;
    work.data[0] = (HI_S32)pfnHisrFunc;
    work.data[1] = (HI_S32)ulData1;
    work.data[2] = (HI_S32)ulData2;
    work.data[3] = (HI_S32)ulData3;
    work.data[4] = (HI_S32)ulData4;
    work.data[5] = (HI_S32)ulData5;

    work.work_priority = (HI_U16)ulPriority;

    ret =  irq_bottom_half(&work);
    switch (ret) {
    case ENOERR:
        return HI_ERR_SUCCESS;
    case ENOMEM:
        g_stSelfRepaire.is_jobadd_exhaust = HI_TRUE;
        g_drop_job_cnt++;
        return HI_ERR_ISR_ADD_JOB_SYS_FAIL;
    default:
        g_drop_job_cnt++;
        return HI_ERR_ISR_ADD_JOB_SYS_FAIL;
    }
}

HI_BOOL HI_MDM_IsIntContext(HI_VOID)
{
    if (OS_INT_ACTIVE)
        return HI_TRUE;
    else
        return HI_FALSE;
}
