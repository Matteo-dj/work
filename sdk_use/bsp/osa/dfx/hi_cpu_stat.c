#include <los_trace.h>
#include <hi_mdm_task.h>
#include <hi_mdm_cpu_stat.h>
#include <hi_driver_internal.h>

#define HI_TEMP_INT_COUNT 32// TODO:最终删除
#define HI_CPU_TRACK_MAX_ARRAY_COUNT 1024

/*****************************************适配宏***************************************************************************/
#define HI_CPU_PERCENT_GET_INTERVAL(curTime,lastTime)     (((curTime) <= (lastTime))?(0xFFFFFFFF - (lastTime) + (curTime)+1):((curTime) - (lastTime)))
#define HI_CPU_STAT_USR_BIT_PERCENT 0x1//bit表示占用率统计
#define HI_CPU_STAT_USR_BIT_TRACK 0x2//bit表示轨迹统计


/*****************************************结构体***************************************************************************/
typedef struct
{
    HI_U64 ullRunCnt;//累积运行时间计数
}HI_CPU_STAT_PERCENT_ITEM_S;

typedef struct
{
    HI_U32 ulLastRunTime;//上次运行时刻
    HI_U16 usLastTaskIntIdx;//上次运行任务中断号
    HI_U16 usMaxTaskIntCount;//任务+中断 最大支持个数
    
    HI_U8 *pBuf;//动态申请到的内存指针
    HI_U8  ucCurLockIdx;//本次锁定下标
    HI_U8  ucLastLockIdx;//上次锁定下标
    HI_U16 pad0;
    
    HI_U64 ullRealTotalCnt;//实际运行总时间
    HI_U64 ullLockTotalCnt[2];//锁定运行总时间
    
    HI_CPU_STAT_PERCENT_ITEM_S *pstPercentRealItems;//实际运行时间统计 ,指向usMaxTaskIntCount*sizeof(HI_CPU_STAT_PERCENT_ITEM_S)的空间
    HI_CPU_STAT_PERCENT_ITEM_S *pstPercentLockItems[2];//锁定运行时间统计,每一个指向usMaxTaskIntCount*sizeof(HI_CPU_STAT_PERCENT_ITEM_S)的空间
    HI_U32 pad1;
}HI_CPU_STAT_PERCENT_S;


typedef struct
{
    HI_U16 curRealIdx;//记录到哪个下标
    HI_U16 curLockIdx;//锁定时下标
    HI_U16 usMaxRecordCount;//最大支持记录的条数
    HI_U16 pad0;
    HI_U8 *pBuf;//动态申请到的内存指针
    HI_CPU_STAT_TRACK_ITEM_S *pstTrackRealItems;//实际记录情况
    HI_CPU_STAT_TRACK_ITEM_S *pstTrackLockItems;//锁定时记录情况
    HI_U32 pad1;
}HI_CPU_STAT_TRACK_S;


typedef struct
{
    HI_U32 ulStartBits;//统计使能情况HI_CPU_STAT_USR_BIT_XXX
    HI_U32 pad0;
    HI_CPU_STAT_PERCENT_S stPercent;//百分比统计信息
    HI_CPU_STAT_TRACK_S stTrack;//运行轨迹统计信息
}HI_CPU_STAT_MNGR_S;



/************************************************全局控制变量和其成员获取***************************************************/
HI_EXTERN HI_U32 g_uwTskMaxNum;

HI_CPU_STAT_MNGR_S g_stCpuStatMngr = {0,};
#define HI_CPU_STAT_GET_MNGR()                                      (&g_stCpuStatMngr)

#define HI_CPU_STAT_GET_PERCENT_MNGR()                              (&g_stCpuStatMngr.stPercent)
#define HI_CPU_STAT_GET_PERCENT_REAL_ITEM(itemIdx)                  (&g_stCpuStatMngr.stPercent.pstPercentRealItems[itemIdx])    
#define HI_CPU_STAT_GET_PERCENT_LOCK_ITEM(lockIdx,itemIdx)          (&g_stCpuStatMngr.stPercent.pstPercentLockItems[lockIdx][itemIdx])   

#define HI_CPU_STAT_GET_TRACK_MNGR()                                (&g_stCpuStatMngr.stTrack)
#define HI_CPU_STAT_GET_TRACK_REAL_ITEM(itemIdx)                    (&g_stCpuStatMngr.stTrack.pstTrackRealItems[itemIdx])    
#define HI_CPU_STAT_GET_TRACK_LOCK_ITEM(itemIdx)                    (&g_stCpuStatMngr.stTrack.pstTrackLockItems[itemIdx])    


/****************************************************************************************************************************
PROC LOCK:用于保证在开始停止过程中,统计函数不会访问已经释放的资源;通过pstMngr->ulStartBits保护资源,申请资源后使能pstMngr->ulStartBits,
释放资源前去使能pstMngr->ulStartBits;回调函数底层有关中断保护,目前上层不需要保护。
****************************************************************************************************************************/
#define HI_CPU_STAT_PROC_LOCK()
#define HI_CPU_STAT_PROC_UNLOCK()



HI_U32 g_test_cpu_hook_count[10] = {0};

HI_U32 my_total = 0;
HI_PRV HI_VOID cpu_stat_percent_hook(LOS_TRACE_TYPE trace_type, UINT32 data0, UINT32 data1, UINT32 entry)
{
    HI_CPU_STAT_PERCENT_S *pstPercentMngr = HI_CPU_STAT_GET_PERCENT_MNGR();
    HI_U32 cur_time = 0;
    HI_U32 interval = 0;
    HI_U32 last_idx = 0;
    HI_CPU_STAT_PERCENT_ITEM_S *pstPercentRunItem = HI_NULL;
    HI_U32 ulTaskIntId = data0;

    if(trace_type != TRAP_INT)
    {
        ulTaskIntId =data0+HI_TEMP_INT_COUNT;
    }
    
    if(ulTaskIntId>=pstPercentMngr->usMaxTaskIntCount)
    {
        // TODO:统计量        
        g_test_cpu_hook_count[9]++;
        return;
    }

    g_test_cpu_hook_count[1]++;
        
    switch(trace_type)
    {
        case TRAP_INT:
        case SCHEDULE_TSK:           
            g_test_cpu_hook_count[2]++;
            (HI_VOID)HI_TIME_CNT_GetUs(&cur_time);
            interval=HI_CPU_PERCENT_GET_INTERVAL(cur_time,pstPercentMngr->ulLastRunTime);
            last_idx=pstPercentMngr->usLastTaskIntIdx;
            if(last_idx<pstPercentMngr->usMaxTaskIntCount)//初始值
            {
                pstPercentRunItem=HI_CPU_STAT_GET_PERCENT_REAL_ITEM(last_idx);
                pstPercentRunItem->ullRunCnt+=interval;
                pstPercentMngr->ullRealTotalCnt+=interval;
            }
            
            pstPercentMngr->ulLastRunTime = cur_time;
            pstPercentMngr->usLastTaskIntIdx = (HI_U16)ulTaskIntId;
            break;
            
        //case HI_TASK_SCHEDULE_TYPE_DELETE_TASK:删除任务立即清除信息
        case CREATE_TSK:
            
            g_test_cpu_hook_count[3]++;
            pstPercentRunItem=HI_CPU_STAT_GET_PERCENT_REAL_ITEM(ulTaskIntId);
            memset_s((HI_VOID *)pstPercentRunItem,sizeof(HI_CPU_STAT_PERCENT_ITEM_S),0x0,sizeof(HI_CPU_STAT_PERCENT_ITEM_S));
            break;
        default:
            ;
    }

    return;
}

HI_PRV HI_VOID cpu_stat_track_hook(LOS_TRACE_TYPE trace_type, UINT32 data0, UINT32 data1, UINT32 entry)
{
    HI_CPU_STAT_TRACK_S *pstTrackMngr=HI_CPU_STAT_GET_TRACK_MNGR();
    HI_CPU_STAT_TRACK_ITEM_S *pstTrackRunItem=HI_CPU_STAT_GET_TRACK_REAL_ITEM(pstTrackMngr->curRealIdx);

    pstTrackRunItem->ulTraceType = trace_type;    
    pstTrackRunItem->ulData0 = data0;
    pstTrackRunItem->ulData1 = data1;
    pstTrackRunItem->ulEntry = entry;
    (HI_VOID)HI_TIME_CNT_GetUs(&(pstTrackRunItem->ulCurTime));


    pstTrackMngr->curRealIdx++;
    if(pstTrackMngr->curRealIdx >= pstTrackMngr->usMaxRecordCount)
    {
        pstTrackMngr->curRealIdx=0;
    }
    return;
}

HI_PRV HI_VOID cpu_stat_hook(LOS_TRACE_TYPE trace_type, UINT32 data0, UINT32 data1, UINT32 entry)
{
    HI_CPU_STAT_MNGR_S *pstMngr=HI_CPU_STAT_GET_MNGR();


    g_test_cpu_hook_count[0]++;
    HI_CPU_STAT_PROC_LOCK();
    if((pstMngr->ulStartBits&HI_CPU_STAT_USR_BIT_PERCENT)
        &&((HI_U32)trace_type&((HI_U32)TRACE_ISR_FLAG|(HI_U32)TRACE_TASK_FLAG)))
    {
        cpu_stat_percent_hook(trace_type,data0,data1,entry);
    }
    HI_CPU_STAT_PROC_UNLOCK();


    HI_CPU_STAT_PROC_LOCK();
    if(pstMngr->ulStartBits&HI_CPU_STAT_USR_BIT_TRACK)
    {
        cpu_stat_track_hook(trace_type,data0,data1,entry);
    }
    HI_CPU_STAT_PROC_UNLOCK();
    
    return;
}



HI_U32 cpu_stat_start(HI_U32 ulUsrBit)
{
    HI_U32 ret=HI_ERR_SUCCESS;
    HI_CPU_STAT_MNGR_S *pstMngr=HI_CPU_STAT_GET_MNGR();

    //非空表示已经注册回调函数
    if(pstMngr->ulStartBits)
    {
        pstMngr->ulStartBits|=ulUsrBit;
        return HI_ERR_SUCCESS;
    }

    //注册回调函数
    ret=HI_MDM_TASK_RegisterSchedulingTrack((TASK_SCHEDULE_FUNC)cpu_stat_hook);
    if(ret==HI_ERR_SUCCESS)
    {
        pstMngr->ulStartBits|=ulUsrBit;
        return HI_ERR_SUCCESS;

    }
    return ret;
}

HI_U32 cpu_stat_stop(HI_U32 ulUsrBit)
{
    HI_U32 ret=HI_ERR_SUCCESS;
    HI_CPU_STAT_MNGR_S *pstMngr=HI_CPU_STAT_GET_MNGR();

    pstMngr->ulStartBits&=(~ulUsrBit);

    //仅为空时采需要注销回调函数
    if(!pstMngr->ulStartBits)
    {
        ret=HI_MDM_TASK_RegisterSchedulingTrack(HI_NULL);        
        if(ret!=HI_ERR_SUCCESS)
        {
            pstMngr->ulStartBits|=ulUsrBit;
            return ret;
        }
    }
    
    return HI_ERR_SUCCESS;
}

HI_VOID HI_MDM_CPU_TrackSetType(HI_U32 trace_type)
{
    LOS_SetTraceType((LOS_TRACE_MASK)trace_type);
}

HI_U32  HI_MDM_CPU_TrackGetType(HI_VOID)
{
    return LOS_GetTraceType(); 
}

HI_U32 HI_MDM_CPU_TrackStart(HI_U16 usMaxRecordCount)
{
    HI_U32 ret=HI_ERR_SUCCESS;
    HI_CPU_STAT_MNGR_S *pstMngr=HI_CPU_STAT_GET_MNGR();
    HI_CPU_STAT_TRACK_S *pstTrackMngr=HI_CPU_STAT_GET_TRACK_MNGR();
    HI_U32 ulItemsSize=(sizeof(HI_CPU_STAT_TRACK_ITEM_S)*usMaxRecordCount);
    HI_U32 ulMemSize=ulItemsSize*(2);
    HI_U8 *pBuf=HI_NULL;

    //状态判断
    if(pstMngr->ulStartBits&HI_CPU_STAT_USR_BIT_TRACK)
    {
        return HI_ERR_CPU_STAT_TRACK_RESTART;
    }

    if(usMaxRecordCount>HI_CPU_TRACK_MAX_ARRAY_COUNT)
    {
        return HI_ERR_CPU_STAT_TRACK_TOO_LARGE_COUNT;
    }

    //申请统计空间
    pBuf=(HI_U8*)HI_MDM_Malloc((HI_U32)HI_MOD_ID_SYS_CPUP,ulMemSize);
    if(pBuf==HI_NULL)
    {
        return HI_ERR_CPU_STAT_NOT_ENOUGH_MEMORY;
    }

    memset_s((HI_VOID *)pBuf, ulMemSize, 0x0, ulMemSize);

    //初始化轨迹控制结构
    memset_s((HI_VOID *)pstTrackMngr, sizeof(HI_CPU_STAT_TRACK_S), 0x0, sizeof(HI_CPU_STAT_TRACK_S));
    pstTrackMngr->pBuf=pBuf;
    pstTrackMngr->pstTrackRealItems=(HI_CPU_STAT_TRACK_ITEM_S *)pBuf;
    pstTrackMngr->pstTrackLockItems=(HI_CPU_STAT_TRACK_ITEM_S *)(pBuf+ulItemsSize);
    pstTrackMngr->usMaxRecordCount=usMaxRecordCount;


    //注册启动
    ret=cpu_stat_start(HI_CPU_STAT_USR_BIT_TRACK);
    if(ret!=HI_ERR_SUCCESS)
    {
        HI_MDM_Free(HI_MOD_ID_SYS_CPUP,pBuf);
        return ret;
    }
    return HI_ERR_SUCCESS;
}




HI_U32 HI_MDM_CPU_TrackStop(HI_VOID)
{
    HI_U32 ret=HI_ERR_SUCCESS;
    HI_CPU_STAT_MNGR_S *pstMngr=HI_CPU_STAT_GET_MNGR();
    HI_CPU_STAT_TRACK_S *pstTrackMngr=HI_CPU_STAT_GET_TRACK_MNGR();

    //状态判断
    if(!(pstMngr->ulStartBits&HI_CPU_STAT_USR_BIT_TRACK))
    {
        return HI_ERR_CPU_STAT_TRACK_RESTOP;
    }

    //停止统计功能
    ret=cpu_stat_stop(HI_CPU_STAT_USR_BIT_TRACK);
    if(ret!=HI_ERR_SUCCESS)
    {
        return ret;
    }

    //释放统计用资源
    HI_MDM_Free(HI_MOD_ID_SYS_CPUP,pstTrackMngr->pBuf);
    return HI_ERR_SUCCESS;
}

HI_U32 HI_MDM_CPU_TrackLock(HI_VOID)
{
    HI_CPU_STAT_MNGR_S *pstMngr=HI_CPU_STAT_GET_MNGR();
    HI_CPU_STAT_TRACK_S *pstTrackMngr=HI_CPU_STAT_GET_TRACK_MNGR();
    HI_U32 ulIntSave=0;

    //状态判断
    if(!(pstMngr->ulStartBits&HI_CPU_STAT_USR_BIT_TRACK))
    {
        return HI_ERR_CPU_STAT_TRACK_NO_INIT;
    }

    //拷贝资源
    ulIntSave=HI_MDM_INT_Lock();
    (HI_VOID)memcpy_s(pstTrackMngr->pstTrackLockItems,
                sizeof(HI_CPU_STAT_TRACK_ITEM_S) * pstTrackMngr->usMaxRecordCount,
                pstTrackMngr->pstTrackRealItems,
                sizeof(HI_CPU_STAT_TRACK_ITEM_S) * pstTrackMngr->usMaxRecordCount);
    pstTrackMngr->curLockIdx=pstTrackMngr->curRealIdx;
    HI_MDM_INT_Restore(ulIntSave);
    return HI_ERR_SUCCESS;   
}

HI_U32 HI_MDM_CPU_TrackGetInfo(HI_CPU_STAT_TRACK_INFO_S *pInfo)
{
    HI_CPU_STAT_MNGR_S *pstMngr=HI_CPU_STAT_GET_MNGR();
    HI_CPU_STAT_TRACK_S *pstTrackMngr=HI_CPU_STAT_GET_TRACK_MNGR();

    //判断状态
    if(!(pstMngr->ulStartBits&HI_CPU_STAT_USR_BIT_TRACK))
    {
        return HI_ERR_CPU_STAT_TRACK_NO_INIT;
    }
    
    //判断参数
    if(pInfo==HI_NULL)
    {
        return HI_ERR_CPU_STAT_INVALID_PARAM;
    }

    pInfo->ulLockIdx = pstTrackMngr->curLockIdx;
    pInfo->ulMaxRecordCount=pstTrackMngr->usMaxRecordCount;
    //返回用户所需信息
    return HI_ERR_SUCCESS;
}




HI_U32 HI_MDM_CPU_TrackGetLockInfoByIdx(HI_U32 ulIdx,HI_CPU_STAT_TRACK_ITEM_S *pItemInfo)
{
    HI_CPU_STAT_MNGR_S *pstMngr=HI_CPU_STAT_GET_MNGR();
    HI_CPU_STAT_TRACK_S *pstTrackMngr=HI_CPU_STAT_GET_TRACK_MNGR();
    HI_CPU_STAT_TRACK_ITEM_S *pstTrackItem=HI_NULL;

    //判断状态
    if(!(pstMngr->ulStartBits&HI_CPU_STAT_USR_BIT_TRACK))
    {
        return HI_ERR_CPU_STAT_TRACK_NO_INIT;
    }

    //判断参数
    if(ulIdx>=pstTrackMngr->usMaxRecordCount || pItemInfo==HI_NULL)
    {
        return HI_ERR_CPU_STAT_INVALID_PARAM;
    }

    //返回用户所需信息
    pstTrackItem = HI_CPU_STAT_GET_TRACK_LOCK_ITEM(ulIdx);
    (HI_VOID)memcpy_s(pItemInfo, sizeof(HI_CPU_STAT_TRACK_ITEM_S), pstTrackItem, sizeof(HI_CPU_STAT_TRACK_ITEM_S));
    return HI_ERR_SUCCESS;

}


HI_U32 HI_MDM_CPU_PercentStart(HI_VOID)
{
    HI_U32 ret=HI_ERR_SUCCESS;
    HI_U16 usMaxTaskCount=(HI_U16)g_uwTskMaxNum;
    HI_CPU_STAT_MNGR_S *pstMngr=HI_CPU_STAT_GET_MNGR();
    HI_CPU_STAT_PERCENT_S *pstPercentMngr=HI_CPU_STAT_GET_PERCENT_MNGR();
    HI_U32 ulItemsSize=sizeof(HI_CPU_STAT_PERCENT_ITEM_S)*(usMaxTaskCount+HI_TEMP_INT_COUNT);
    HI_U32 ulMemSize=ulItemsSize*(2+1);
    HI_U8 *pBuf=HI_NULL;


    //状态判断
    if(pstMngr->ulStartBits&HI_CPU_STAT_USR_BIT_PERCENT)
    {
        return HI_ERR_CPU_STAT_PERCENT_RESTART;
    }

    //申请统计空间
    pBuf=(HI_U8 *)HI_MDM_Malloc((HI_U32)HI_MOD_ID_SYS_CPUP,ulMemSize);
    if(pBuf==HI_NULL)
    {
        return HI_ERR_FAILURE;
    }

    memset_s((HI_VOID *)pBuf, ulMemSize, 0x0, ulMemSize);

    //初始化占用率控制结构
    memset_s((HI_VOID *)pstPercentMngr, sizeof(HI_CPU_STAT_PERCENT_S), 0x0, sizeof(HI_CPU_STAT_PERCENT_S));
    pstPercentMngr->pBuf=pBuf;
    pstPercentMngr->pstPercentRealItems=(HI_CPU_STAT_PERCENT_ITEM_S *)pBuf;
    pstPercentMngr->pstPercentLockItems[0]=(HI_CPU_STAT_PERCENT_ITEM_S*)(pBuf+ulItemsSize);
    pstPercentMngr->pstPercentLockItems[1]=(HI_CPU_STAT_PERCENT_ITEM_S*)(pBuf+ulItemsSize+ulItemsSize);
    pstPercentMngr->usMaxTaskIntCount=usMaxTaskCount+HI_TEMP_INT_COUNT;
    pstPercentMngr->usLastTaskIntIdx=0xFFFF;

    //启动百分比统计
    ret=cpu_stat_start(HI_CPU_STAT_USR_BIT_PERCENT);
    if(ret!=HI_ERR_SUCCESS)
    {
        HI_MDM_Free(HI_MOD_ID_SYS_CPUP,pBuf);
        return ret;
    }
    return HI_ERR_SUCCESS;
}

HI_U32 HI_MDM_CPU_PercentStop(HI_VOID)
{
    HI_U32 ret=HI_ERR_SUCCESS;
    HI_CPU_STAT_MNGR_S *pstMngr=HI_CPU_STAT_GET_MNGR();
    HI_CPU_STAT_PERCENT_S *pstPercentMngr=HI_CPU_STAT_GET_PERCENT_MNGR();

    //状态判断
    if(!(pstMngr->ulStartBits&HI_CPU_STAT_USR_BIT_PERCENT))
    {
        return HI_ERR_CPU_STAT_PERCENT_RESTOP;
    }

    //停止百分比统计
    ret=cpu_stat_stop(HI_CPU_STAT_USR_BIT_PERCENT);
    if(ret!=HI_ERR_SUCCESS)
    {
        return ret;
    }

    //释放资源
    HI_MDM_Free(HI_MOD_ID_SYS_CPUP,pstPercentMngr->pBuf);
    return HI_ERR_SUCCESS;
    
}


HI_U32 HI_MDM_CPU_PercentLock(HI_VOID)
{
    
    HI_CPU_STAT_MNGR_S *pstMngr=HI_CPU_STAT_GET_MNGR();
    HI_CPU_STAT_PERCENT_S *pstPercentMngr=HI_CPU_STAT_GET_PERCENT_MNGR();
    HI_U32 ulIntSave=0;
    HI_U32 ucCurLockIdx=0;

    //状态判断
    if(!(pstMngr->ulStartBits&HI_CPU_STAT_USR_BIT_PERCENT))
    {
        return HI_ERR_CPU_STAT_PERCENT_NO_INIT;
    }

    //交替ucLastLockIdx和ucCurLockIdx
    if(pstPercentMngr->ucLastLockIdx==0)
    {
        pstPercentMngr->ucLastLockIdx=1;
        pstPercentMngr->ucCurLockIdx=0;
    }
    else
    {
        pstPercentMngr->ucLastLockIdx=0;
        pstPercentMngr->ucCurLockIdx=1;
    }    
    ucCurLockIdx=pstPercentMngr->ucCurLockIdx;

    //拷贝数据
    // TODO:考虑处理当前任务已经执行的时间,需要支持获取当前中断号除非该接口不支持在中断中调用
    ulIntSave=HI_MDM_INT_Lock();
    (HI_VOID)memcpy_s(pstPercentMngr->pstPercentLockItems[ucCurLockIdx],
                sizeof(HI_CPU_STAT_PERCENT_ITEM_S)*pstPercentMngr->usMaxTaskIntCount,
                pstPercentMngr->pstPercentRealItems,
                sizeof(HI_CPU_STAT_PERCENT_ITEM_S)*pstPercentMngr->usMaxTaskIntCount);
    pstPercentMngr->ullLockTotalCnt[ucCurLockIdx]=pstPercentMngr->ullRealTotalCnt;
    HI_MDM_INT_Restore(ulIntSave); 
    return HI_ERR_SUCCESS;
}

HI_U32 HI_MDM_CPU_PercentGetTaskCount(HI_VOID)//if 获取信息的接口不断增多,考虑合并
{
    HI_CPU_STAT_MNGR_S *pstMngr=HI_CPU_STAT_GET_MNGR();
    HI_CPU_STAT_PERCENT_S *pstPercentMngr=HI_CPU_STAT_GET_PERCENT_MNGR();

    //状态判断
    if(!(pstMngr->ulStartBits&HI_CPU_STAT_USR_BIT_PERCENT))
    {
        return HI_CPU_STAT_INVALID_COUNT;
    }
    
    return (HI_U32)((HI_U32)pstPercentMngr->usMaxTaskIntCount - (HI_U32)HI_TEMP_INT_COUNT);
}

HI_U32 HI_MDM_CPU_PercentGetTotalTime(HI_U64 *pLastTotalTime,HI_U64 *pCurTotalTime)
{
    HI_CPU_STAT_MNGR_S *pstMngr=HI_CPU_STAT_GET_MNGR();
    HI_CPU_STAT_PERCENT_S *pstPercentMngr=HI_CPU_STAT_GET_PERCENT_MNGR();

    //状态判断
    if(!(pstMngr->ulStartBits&HI_CPU_STAT_USR_BIT_PERCENT))
    {
        return HI_ERR_CPU_STAT_PERCENT_NO_INIT;
    }

    //参数判断
    if(pLastTotalTime==HI_NULL||pCurTotalTime==HI_NULL)
    {
        return HI_ERR_CPU_STAT_INVALID_PARAM;
    }


    //返回用户所需信息
    *pLastTotalTime=pstPercentMngr->ullLockTotalCnt[pstPercentMngr->ucLastLockIdx];
    *pCurTotalTime=pstPercentMngr->ullLockTotalCnt[pstPercentMngr->ucCurLockIdx];
    return HI_ERR_SUCCESS;
}

HI_U32 HI_MDM_CPU_PercentGetTimeByTaskIdx(HI_U32 ulTaskIntId,HI_BOOL bIsr,HI_U64 *pLastTimeCnt,HI_U64 *pCurTimeCnt)
{
    HI_CPU_STAT_MNGR_S *pstMngr=HI_CPU_STAT_GET_MNGR();
    HI_CPU_STAT_PERCENT_S *pstPercentMngr=HI_CPU_STAT_GET_PERCENT_MNGR();
    HI_CPU_STAT_PERCENT_ITEM_S *pstLastLockItem=HI_NULL;
    HI_CPU_STAT_PERCENT_ITEM_S *pstCurLockItem=HI_NULL;

    //判断状态
    if(!(pstMngr->ulStartBits&HI_CPU_STAT_USR_BIT_PERCENT))
    {
        return HI_ERR_CPU_STAT_PERCENT_NO_INIT;
    }

    //修订入参
    if(!bIsr)
    {
        ulTaskIntId+=HI_TEMP_INT_COUNT;
    }

    //参数判断
    if(pLastTimeCnt==HI_NULL || pCurTimeCnt==HI_NULL || ulTaskIntId>=pstPercentMngr->usMaxTaskIntCount)
    {
        return HI_ERR_CPU_STAT_INVALID_PARAM;
    }

    //返回数据给用户
    pstLastLockItem = HI_CPU_STAT_GET_PERCENT_LOCK_ITEM(pstPercentMngr->ucLastLockIdx,ulTaskIntId);
    pstCurLockItem = HI_CPU_STAT_GET_PERCENT_LOCK_ITEM(pstPercentMngr->ucCurLockIdx,ulTaskIntId);
    *pLastTimeCnt = pstLastLockItem->ullRunCnt;
    *pCurTimeCnt = pstCurLockItem->ullRunCnt;
    return HI_ERR_SUCCESS;
}

void dump_cpup(HI_VOID)
{
    HI_U32 i = 0;
    HI_U64 cur_time = 0;
    HI_U64 last_time = 0;
    HI_U64 cur_total = 0;
    HI_U64 last_total = 0;
    HI_CPU_STAT_TRACK_ITEM_S track_item = {0};
    
    (HI_VOID)HI_MDM_CPU_PercentLock();
    (HI_VOID)HI_MDM_CPU_PercentGetTotalTime(&last_total, &cur_total);
    dprintf("CPUP:\r\n");
    dprintf("last_total:%lld, cur_total:%lld\r\n", last_total, cur_total);
    for (i = 0; i < 32; i++)
    {
        (HI_VOID)HI_MDM_CPU_PercentGetTimeByTaskIdx(i,(HI_BOOL)true,&last_time,&cur_time);
        dprintf("INT[%d]%lld, %lld\r\n", i, last_time, cur_time);
    }

    for (i = 0; i < 32; i++)
    {
        (HI_VOID)HI_MDM_CPU_PercentGetTimeByTaskIdx(i,(HI_BOOL)false,&last_time,&cur_time);
        dprintf("task[%d]%lld, %lld\r\n", i, last_time, cur_time);
    }

    (HI_VOID)HI_MDM_CPU_TrackLock();
    dprintf("TRACK:\r\n");
    for (i = 0; i < 64; i++)
    {
        memset_s((HI_VOID *)&track_item, sizeof(track_item), 0, sizeof(track_item));
        if (!HI_MDM_CPU_TrackGetLockInfoByIdx(i, &track_item))
        {
            dprintf("%x %x %x %x\r\n", track_item.ulTraceType, track_item.ulData0, track_item.ulData1, track_item.ulCurTime);
        }
    }
}

