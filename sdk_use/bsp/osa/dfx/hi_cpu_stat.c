#include <los_trace.h>
#include <hi_mdm_task.h>
#include <hi_mdm_cpu_stat.h>
#include <hi_driver_internal.h>

#define HI_TEMP_INT_COUNT 32// TODO:����ɾ��
#define HI_CPU_TRACK_MAX_ARRAY_COUNT 1024

/*****************************************�����***************************************************************************/
#define HI_CPU_PERCENT_GET_INTERVAL(curTime,lastTime)     (((curTime) <= (lastTime))?(0xFFFFFFFF - (lastTime) + (curTime)+1):((curTime) - (lastTime)))
#define HI_CPU_STAT_USR_BIT_PERCENT 0x1//bit��ʾռ����ͳ��
#define HI_CPU_STAT_USR_BIT_TRACK 0x2//bit��ʾ�켣ͳ��


/*****************************************�ṹ��***************************************************************************/
typedef struct
{
    HI_U64 ullRunCnt;//�ۻ�����ʱ�����
}HI_CPU_STAT_PERCENT_ITEM_S;

typedef struct
{
    HI_U32 ulLastRunTime;//�ϴ�����ʱ��
    HI_U16 usLastTaskIntIdx;//�ϴ����������жϺ�
    HI_U16 usMaxTaskIntCount;//����+�ж� ���֧�ָ���
    
    HI_U8 *pBuf;//��̬���뵽���ڴ�ָ��
    HI_U8  ucCurLockIdx;//���������±�
    HI_U8  ucLastLockIdx;//�ϴ������±�
    HI_U16 pad0;
    
    HI_U64 ullRealTotalCnt;//ʵ��������ʱ��
    HI_U64 ullLockTotalCnt[2];//����������ʱ��
    
    HI_CPU_STAT_PERCENT_ITEM_S *pstPercentRealItems;//ʵ������ʱ��ͳ�� ,ָ��usMaxTaskIntCount*sizeof(HI_CPU_STAT_PERCENT_ITEM_S)�Ŀռ�
    HI_CPU_STAT_PERCENT_ITEM_S *pstPercentLockItems[2];//��������ʱ��ͳ��,ÿһ��ָ��usMaxTaskIntCount*sizeof(HI_CPU_STAT_PERCENT_ITEM_S)�Ŀռ�
    HI_U32 pad1;
}HI_CPU_STAT_PERCENT_S;


typedef struct
{
    HI_U16 curRealIdx;//��¼���ĸ��±�
    HI_U16 curLockIdx;//����ʱ�±�
    HI_U16 usMaxRecordCount;//���֧�ּ�¼������
    HI_U16 pad0;
    HI_U8 *pBuf;//��̬���뵽���ڴ�ָ��
    HI_CPU_STAT_TRACK_ITEM_S *pstTrackRealItems;//ʵ�ʼ�¼���
    HI_CPU_STAT_TRACK_ITEM_S *pstTrackLockItems;//����ʱ��¼���
    HI_U32 pad1;
}HI_CPU_STAT_TRACK_S;


typedef struct
{
    HI_U32 ulStartBits;//ͳ��ʹ�����HI_CPU_STAT_USR_BIT_XXX
    HI_U32 pad0;
    HI_CPU_STAT_PERCENT_S stPercent;//�ٷֱ�ͳ����Ϣ
    HI_CPU_STAT_TRACK_S stTrack;//���й켣ͳ����Ϣ
}HI_CPU_STAT_MNGR_S;



/************************************************ȫ�ֿ��Ʊ��������Ա��ȡ***************************************************/
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
PROC LOCK:���ڱ�֤�ڿ�ʼֹͣ������,ͳ�ƺ�����������Ѿ��ͷŵ���Դ;ͨ��pstMngr->ulStartBits������Դ,������Դ��ʹ��pstMngr->ulStartBits,
�ͷ���Դǰȥʹ��pstMngr->ulStartBits;�ص������ײ��й��жϱ���,Ŀǰ�ϲ㲻��Ҫ������
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
        // TODO:ͳ����        
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
            if(last_idx<pstPercentMngr->usMaxTaskIntCount)//��ʼֵ
            {
                pstPercentRunItem=HI_CPU_STAT_GET_PERCENT_REAL_ITEM(last_idx);
                pstPercentRunItem->ullRunCnt+=interval;
                pstPercentMngr->ullRealTotalCnt+=interval;
            }
            
            pstPercentMngr->ulLastRunTime = cur_time;
            pstPercentMngr->usLastTaskIntIdx = (HI_U16)ulTaskIntId;
            break;
            
        //case HI_TASK_SCHEDULE_TYPE_DELETE_TASK:ɾ���������������Ϣ
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

    //�ǿձ�ʾ�Ѿ�ע��ص�����
    if(pstMngr->ulStartBits)
    {
        pstMngr->ulStartBits|=ulUsrBit;
        return HI_ERR_SUCCESS;
    }

    //ע��ص�����
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

    //��Ϊ��ʱ����Ҫע���ص�����
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

    //״̬�ж�
    if(pstMngr->ulStartBits&HI_CPU_STAT_USR_BIT_TRACK)
    {
        return HI_ERR_CPU_STAT_TRACK_RESTART;
    }

    if(usMaxRecordCount>HI_CPU_TRACK_MAX_ARRAY_COUNT)
    {
        return HI_ERR_CPU_STAT_TRACK_TOO_LARGE_COUNT;
    }

    //����ͳ�ƿռ�
    pBuf=(HI_U8*)HI_MDM_Malloc((HI_U32)HI_MOD_ID_SYS_CPUP,ulMemSize);
    if(pBuf==HI_NULL)
    {
        return HI_ERR_CPU_STAT_NOT_ENOUGH_MEMORY;
    }

    memset_s((HI_VOID *)pBuf, ulMemSize, 0x0, ulMemSize);

    //��ʼ���켣���ƽṹ
    memset_s((HI_VOID *)pstTrackMngr, sizeof(HI_CPU_STAT_TRACK_S), 0x0, sizeof(HI_CPU_STAT_TRACK_S));
    pstTrackMngr->pBuf=pBuf;
    pstTrackMngr->pstTrackRealItems=(HI_CPU_STAT_TRACK_ITEM_S *)pBuf;
    pstTrackMngr->pstTrackLockItems=(HI_CPU_STAT_TRACK_ITEM_S *)(pBuf+ulItemsSize);
    pstTrackMngr->usMaxRecordCount=usMaxRecordCount;


    //ע������
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

    //״̬�ж�
    if(!(pstMngr->ulStartBits&HI_CPU_STAT_USR_BIT_TRACK))
    {
        return HI_ERR_CPU_STAT_TRACK_RESTOP;
    }

    //ֹͣͳ�ƹ���
    ret=cpu_stat_stop(HI_CPU_STAT_USR_BIT_TRACK);
    if(ret!=HI_ERR_SUCCESS)
    {
        return ret;
    }

    //�ͷ�ͳ������Դ
    HI_MDM_Free(HI_MOD_ID_SYS_CPUP,pstTrackMngr->pBuf);
    return HI_ERR_SUCCESS;
}

HI_U32 HI_MDM_CPU_TrackLock(HI_VOID)
{
    HI_CPU_STAT_MNGR_S *pstMngr=HI_CPU_STAT_GET_MNGR();
    HI_CPU_STAT_TRACK_S *pstTrackMngr=HI_CPU_STAT_GET_TRACK_MNGR();
    HI_U32 ulIntSave=0;

    //״̬�ж�
    if(!(pstMngr->ulStartBits&HI_CPU_STAT_USR_BIT_TRACK))
    {
        return HI_ERR_CPU_STAT_TRACK_NO_INIT;
    }

    //������Դ
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

    //�ж�״̬
    if(!(pstMngr->ulStartBits&HI_CPU_STAT_USR_BIT_TRACK))
    {
        return HI_ERR_CPU_STAT_TRACK_NO_INIT;
    }
    
    //�жϲ���
    if(pInfo==HI_NULL)
    {
        return HI_ERR_CPU_STAT_INVALID_PARAM;
    }

    pInfo->ulLockIdx = pstTrackMngr->curLockIdx;
    pInfo->ulMaxRecordCount=pstTrackMngr->usMaxRecordCount;
    //�����û�������Ϣ
    return HI_ERR_SUCCESS;
}




HI_U32 HI_MDM_CPU_TrackGetLockInfoByIdx(HI_U32 ulIdx,HI_CPU_STAT_TRACK_ITEM_S *pItemInfo)
{
    HI_CPU_STAT_MNGR_S *pstMngr=HI_CPU_STAT_GET_MNGR();
    HI_CPU_STAT_TRACK_S *pstTrackMngr=HI_CPU_STAT_GET_TRACK_MNGR();
    HI_CPU_STAT_TRACK_ITEM_S *pstTrackItem=HI_NULL;

    //�ж�״̬
    if(!(pstMngr->ulStartBits&HI_CPU_STAT_USR_BIT_TRACK))
    {
        return HI_ERR_CPU_STAT_TRACK_NO_INIT;
    }

    //�жϲ���
    if(ulIdx>=pstTrackMngr->usMaxRecordCount || pItemInfo==HI_NULL)
    {
        return HI_ERR_CPU_STAT_INVALID_PARAM;
    }

    //�����û�������Ϣ
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


    //״̬�ж�
    if(pstMngr->ulStartBits&HI_CPU_STAT_USR_BIT_PERCENT)
    {
        return HI_ERR_CPU_STAT_PERCENT_RESTART;
    }

    //����ͳ�ƿռ�
    pBuf=(HI_U8 *)HI_MDM_Malloc((HI_U32)HI_MOD_ID_SYS_CPUP,ulMemSize);
    if(pBuf==HI_NULL)
    {
        return HI_ERR_FAILURE;
    }

    memset_s((HI_VOID *)pBuf, ulMemSize, 0x0, ulMemSize);

    //��ʼ��ռ���ʿ��ƽṹ
    memset_s((HI_VOID *)pstPercentMngr, sizeof(HI_CPU_STAT_PERCENT_S), 0x0, sizeof(HI_CPU_STAT_PERCENT_S));
    pstPercentMngr->pBuf=pBuf;
    pstPercentMngr->pstPercentRealItems=(HI_CPU_STAT_PERCENT_ITEM_S *)pBuf;
    pstPercentMngr->pstPercentLockItems[0]=(HI_CPU_STAT_PERCENT_ITEM_S*)(pBuf+ulItemsSize);
    pstPercentMngr->pstPercentLockItems[1]=(HI_CPU_STAT_PERCENT_ITEM_S*)(pBuf+ulItemsSize+ulItemsSize);
    pstPercentMngr->usMaxTaskIntCount=usMaxTaskCount+HI_TEMP_INT_COUNT;
    pstPercentMngr->usLastTaskIntIdx=0xFFFF;

    //�����ٷֱ�ͳ��
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

    //״̬�ж�
    if(!(pstMngr->ulStartBits&HI_CPU_STAT_USR_BIT_PERCENT))
    {
        return HI_ERR_CPU_STAT_PERCENT_RESTOP;
    }

    //ֹͣ�ٷֱ�ͳ��
    ret=cpu_stat_stop(HI_CPU_STAT_USR_BIT_PERCENT);
    if(ret!=HI_ERR_SUCCESS)
    {
        return ret;
    }

    //�ͷ���Դ
    HI_MDM_Free(HI_MOD_ID_SYS_CPUP,pstPercentMngr->pBuf);
    return HI_ERR_SUCCESS;
    
}


HI_U32 HI_MDM_CPU_PercentLock(HI_VOID)
{
    
    HI_CPU_STAT_MNGR_S *pstMngr=HI_CPU_STAT_GET_MNGR();
    HI_CPU_STAT_PERCENT_S *pstPercentMngr=HI_CPU_STAT_GET_PERCENT_MNGR();
    HI_U32 ulIntSave=0;
    HI_U32 ucCurLockIdx=0;

    //״̬�ж�
    if(!(pstMngr->ulStartBits&HI_CPU_STAT_USR_BIT_PERCENT))
    {
        return HI_ERR_CPU_STAT_PERCENT_NO_INIT;
    }

    //����ucLastLockIdx��ucCurLockIdx
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

    //��������
    // TODO:���Ǵ���ǰ�����Ѿ�ִ�е�ʱ��,��Ҫ֧�ֻ�ȡ��ǰ�жϺų��Ǹýӿڲ�֧�����ж��е���
    ulIntSave=HI_MDM_INT_Lock();
    (HI_VOID)memcpy_s(pstPercentMngr->pstPercentLockItems[ucCurLockIdx],
                sizeof(HI_CPU_STAT_PERCENT_ITEM_S)*pstPercentMngr->usMaxTaskIntCount,
                pstPercentMngr->pstPercentRealItems,
                sizeof(HI_CPU_STAT_PERCENT_ITEM_S)*pstPercentMngr->usMaxTaskIntCount);
    pstPercentMngr->ullLockTotalCnt[ucCurLockIdx]=pstPercentMngr->ullRealTotalCnt;
    HI_MDM_INT_Restore(ulIntSave); 
    return HI_ERR_SUCCESS;
}

HI_U32 HI_MDM_CPU_PercentGetTaskCount(HI_VOID)//if ��ȡ��Ϣ�Ľӿڲ�������,���Ǻϲ�
{
    HI_CPU_STAT_MNGR_S *pstMngr=HI_CPU_STAT_GET_MNGR();
    HI_CPU_STAT_PERCENT_S *pstPercentMngr=HI_CPU_STAT_GET_PERCENT_MNGR();

    //״̬�ж�
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

    //״̬�ж�
    if(!(pstMngr->ulStartBits&HI_CPU_STAT_USR_BIT_PERCENT))
    {
        return HI_ERR_CPU_STAT_PERCENT_NO_INIT;
    }

    //�����ж�
    if(pLastTotalTime==HI_NULL||pCurTotalTime==HI_NULL)
    {
        return HI_ERR_CPU_STAT_INVALID_PARAM;
    }


    //�����û�������Ϣ
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

    //�ж�״̬
    if(!(pstMngr->ulStartBits&HI_CPU_STAT_USR_BIT_PERCENT))
    {
        return HI_ERR_CPU_STAT_PERCENT_NO_INIT;
    }

    //�޶����
    if(!bIsr)
    {
        ulTaskIntId+=HI_TEMP_INT_COUNT;
    }

    //�����ж�
    if(pLastTimeCnt==HI_NULL || pCurTimeCnt==HI_NULL || ulTaskIntId>=pstPercentMngr->usMaxTaskIntCount)
    {
        return HI_ERR_CPU_STAT_INVALID_PARAM;
    }

    //�������ݸ��û�
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

