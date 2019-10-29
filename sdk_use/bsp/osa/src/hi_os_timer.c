#include <hi_types.h>
#include <hi_mdm_types.h>
#include <los_swtmr.h>
#include <hi_mdm_timer.h>
#include "hi_os_stat.h"
#include "hi_os_sys.h"
#include <hi_sys_selfrepair.h>

#define HI_TIMER_CREATE_MAGIC 0x478ADE56

HI_U32 HI_MDM_TIMER_Start (HI_TIMER_HANDLE_S *pstTimerHandler, TIMER_PROC_FUNC pfnTimerRoutine,HI_U32 ulTimeOut,HI_TIMER_TYPE_E enType,HI_U32 ulData)
{
    HI_U32 uwInterval=0;
    HI_U32 ret=LOS_OK;
    HI_U32 uvIntSave=0;
    enum enSwTmrType tmr_type; 

    if(pstTimerHandler==HI_NULL||pfnTimerRoutine==HI_NULL||ulTimeOut==0)
    {
    	HI_OSA_ERR_STAT_INC(ulTimerErr[0]);
        return HI_ERR_TIMER_INVALID_PARAM;
    }
    
    uwInterval=ms2systick(ulTimeOut, HI_FALSE);
    switch (enType)
    {
        case HI_TIMER_TYPE_ONCE:/*产品不再使用once模式，统一把once模式定义为os底层的no selfdelete模式.*/
            tmr_type = LOS_SWTMR_MODE_NO_SELFDELETE;
            break;
        case HI_TIMER_TYPE_PERIOD:
/*
                    【屏蔽人】z00193780
                    【审核人】c00233580
                    【代码分析】二次开发接口，客户有可能使用该分支，需保留。
                    【功能影响】不影响正常业务，屏蔽告警
*/         
            tmr_type = LOS_SWTMR_MODE_PERIOD;
            break;
        default:
            HI_OSA_ERR_STAT_INC(ulTimerErr[0]);
            return HI_ERR_TIMER_INVALID_PARAM;     
    }
    
    uvIntSave = LOS_IntLock();
    if(pstTimerHandler->data[0]!=HI_TIMER_CREATE_MAGIC)
    {
        ret=LOS_SwtmrCreate(uwInterval,tmr_type,(SWTMR_PROC_FUNC)pfnTimerRoutine, (HI_U16*)(&pstTimerHandler->data[1]),ulData);
        if(ret!=LOS_OK)
        {
            LOS_IntRestore(uvIntSave);
            
            if(ret == LOS_ERRNO_SWTMR_MAXSIZE)
            {
                g_stSelfRepaire.is_timer_exhaust = HI_TRUE;
            }
            HI_OSA_ERR_STAT_INC(ulTimerErr[1]);
            HI_OSA_ERR_STAT_SET(ulTimerCreateErr,ret);
            return HI_ERR_TIMER_CREATE_TIMER_FAIL;
        }     
        pstTimerHandler->data[0]=HI_TIMER_CREATE_MAGIC;
    }
 
    ret= LOS_SwtmrModify((HI_U16)(pstTimerHandler->data[1]), uwInterval,tmr_type,(SWTMR_PROC_FUNC)pfnTimerRoutine, ulData);
    if(ret!=LOS_OK)
    {
        LOS_SwtmrDelete((HI_U16)(pstTimerHandler->data[1]));
        pstTimerHandler->data[0]=0;
        LOS_IntRestore(uvIntSave);
        HI_OSA_ERR_STAT_INC(ulTimerErr[2]);
        HI_OSA_ERR_STAT_SET(ulTimerStartErr,ret);
        return HI_ERR_TIMER_CREATE_TIMER_FAIL;
    }
    
    LOS_IntRestore(uvIntSave);
   
    return HI_ERR_SUCCESS;   
}

HI_U32 HI_MDM_TIMER_Stop(HI_TIMER_HANDLE_S *pstTimerHandler)
{
    if(pstTimerHandler==HI_NULL)
    {
    	HI_OSA_ERR_STAT_INC(ulTimerErr[0]);
        return HI_ERR_TIMER_INVALID_PARAM;
    }

    HI_U32 uvIntSave = LOS_IntLock();
    
    if(pstTimerHandler->data[0]==HI_TIMER_CREATE_MAGIC)
    {
        LOS_SwtmrStop((HI_U16)(pstTimerHandler->data[1])); /*暂时不要处理返回值，因对已停止或未创建的定时器调用stop，目前os层返回错误*/
    }
    LOS_IntRestore(uvIntSave);

    return HI_ERR_SUCCESS;
}

HI_U32 HI_MDM_TIMER_Delete(HI_TIMER_HANDLE_S *pstTimerHandler)
{
    if(pstTimerHandler==HI_NULL)
    {
    	HI_OSA_ERR_STAT_INC(ulTimerErr[0]);
        return HI_ERR_TIMER_INVALID_PARAM;
    }

    HI_U32 uvIntSave = LOS_IntLock();
    
    if(pstTimerHandler->data[0]==HI_TIMER_CREATE_MAGIC)
    {
        LOS_SwtmrDelete((HI_U16)(pstTimerHandler->data[1]));
        pstTimerHandler->data[0]=0;
    }
    LOS_IntRestore(uvIntSave);

    return HI_ERR_SUCCESS;
}
