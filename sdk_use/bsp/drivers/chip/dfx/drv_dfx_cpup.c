#include <hi_types.h>
#include <hi_mdm_types.h>
#include <dfx_cpup.h>
#include <hi_mdm_task.h>
#include <drv_dfx.h>
#include <hi_mdm_cpu_stat.h>
#include <hi_sal.h>
#include <hi_mdm_task.h>
HI_PRV HI_U32 report_cpup_info(HI_U32 ulOption)
{
    HI_U32 ulTaskCount;
    HI_U64 ulLastTotalTime,ulCurTotalTime,ulLastItemTime,ulCurItemTime;
    HI_U32 ret=HI_ERR_SUCCESS;
    HI_U32 i=0;
    HI_CPUP_DFX_INFO_TOTAL_IND_S stTotalInd={0};
    HI_CPUP_DFX_INFO_ITEM_IND_S stItemInd={0};
    HI_TASK_INFO_S stTaskInfo={{0}, 0};

	//锁定
    ret=HI_MDM_CPU_PercentLock();
    if(ret!=HI_ERR_SUCCESS)
    {
        stTotalInd.ulRet=ret;
        HI_DIAG_ReportPacket(ID_DIAG_CPUP_REPORT_TOTAL_INFO,(HI_U8)ulOption,(HI_PBYTE)&stTotalInd,sizeof(HI_CPUP_DFX_INFO_TOTAL_IND_S),HI_TRUE);
        return ret;
    }
    
	//获取总信息
    ret=HI_MDM_CPU_PercentGetTotalTime(&ulLastTotalTime,&ulCurTotalTime);
    if(ret!=HI_ERR_SUCCESS)
    {
        stTotalInd.ulRet=ret;
        HI_DIAG_ReportPacket(ID_DIAG_CPUP_REPORT_TOTAL_INFO,(HI_U8)ulOption,(HI_PBYTE)&stTotalInd,sizeof(HI_CPUP_DFX_INFO_TOTAL_IND_S),HI_TRUE);
        return ret;

    }

    ulTaskCount=HI_MDM_CPU_PercentGetTaskCount();
    if(ulTaskCount==HI_CPU_STAT_INVALID_COUNT)
    { 
        stTotalInd.ulRet=HI_ERR_FAILURE;
        HI_DIAG_ReportPacket(ID_DIAG_CPUP_REPORT_TOTAL_INFO,(HI_U8)ulOption,(HI_PBYTE)&stTotalInd,sizeof(stTotalInd),HI_TRUE);
        return ret;

    }
    
    stTotalInd.ulRet=HI_ERR_SUCCESS;
    stTotalInd.ulTaskCount=ulTaskCount;
    stTotalInd.ulLastTotalTimeLow=HI_LOU32(ulLastTotalTime);
    stTotalInd.ulLastTotalTimeHigh=HI_HIU32(ulLastTotalTime);
    stTotalInd.ulCurTotalTimeLow=HI_LOU32(ulCurTotalTime);
    stTotalInd.ulCurTotalTimeHigh=HI_HIU32(ulCurTotalTime);
    HI_DIAG_ReportPacket(ID_DIAG_CPUP_REPORT_TOTAL_INFO,(HI_U8)ulOption,(HI_PBYTE)&stTotalInd,sizeof(stTotalInd),HI_TRUE);


	//上报isr信息
    for(i=0;i<32;i++)
    {
        ret=HI_MDM_CPU_PercentGetTimeByTaskIdx(i,HI_TRUE,&ulLastItemTime,&ulCurItemTime);
        if(ret!=HI_ERR_SUCCESS)
        {
            (hi_void)memset_s((HI_VOID *)&stItemInd, sizeof(stItemInd), 0x0, sizeof(stItemInd));
            stItemInd.ulRet=ret;
            HI_DIAG_ReportPacket(ID_DIAG_CPUP_REPORT_ITEM_INFO,(HI_U8)ulOption,(HI_PBYTE)&stItemInd,sizeof(stItemInd),HI_TRUE);
            return ret;
                        
        }

        stItemInd.ulRet=HI_ERR_SUCCESS;
        
        (hi_void)snprintf_s(stItemInd.taskName, HI_CPUP_DFX_INFO_TASK_NAME_SIZE, HI_CPUP_DFX_INFO_TASK_NAME_SIZE - 1, "isr%u", i);
        stItemInd.ulLastItemTimeLow=HI_LOU32(ulLastItemTime);
        stItemInd.ulLastItemTimeHigh=HI_HIU32(ulLastItemTime);
        stItemInd.ulCurItemTimeLow=HI_LOU32(ulCurItemTime);
        stItemInd.ulCurItemTimeHigh=HI_HIU32(ulCurItemTime);
        stItemInd.ulPermillageTotal=(HI_U32)(ulCurItemTime*1000/ulCurTotalTime);
        stItemInd.ulPermillagePhase=(HI_U32)((ulCurItemTime-ulLastItemTime)*1000/(ulCurTotalTime-ulLastTotalTime));
        HI_DIAG_ReportPacket(ID_DIAG_CPUP_REPORT_ITEM_INFO,(HI_U8)ulOption,(HI_PBYTE)&stItemInd,sizeof(stItemInd),HI_TRUE);

    }

	//上报任务信息
    for(i=0;i<ulTaskCount;i++)
    {
        ret=HI_MDM_CPU_PercentGetTimeByTaskIdx(i,HI_FALSE,&ulLastItemTime,&ulCurItemTime);
        if(ret!=HI_ERR_SUCCESS)
        {
            (hi_void)memset_s((HI_VOID *)&stItemInd, sizeof(stItemInd), 0x0, sizeof(stItemInd));
            stItemInd.ulRet=ret;
            HI_DIAG_ReportPacket(ID_DIAG_CPUP_REPORT_ITEM_INFO,(HI_U8)ulOption,(HI_PBYTE)&stItemInd,sizeof(stItemInd),HI_TRUE);
            return ret;
        }

        ret = HI_MDM_TASK_GetTaskInfo(i,&stTaskInfo);
        if(ret!=HI_ERR_SUCCESS)
        {
            continue;
        }

        
        stItemInd.ulRet=HI_ERR_SUCCESS;
        int ret1 = 0;
        ret1 = snprintf_s(stItemInd.taskName, HI_CPUP_DFX_INFO_TASK_NAME_SIZE, HI_CPUP_DFX_INFO_TASK_NAME_SIZE - 1, "%s",stTaskInfo.acName);
        stItemInd.ulLastItemTimeLow=HI_LOU32(ulLastItemTime);
        stItemInd.ulLastItemTimeHigh=HI_HIU32(ulLastItemTime);
        stItemInd.ulCurItemTimeLow=HI_LOU32(ulCurItemTime);
        stItemInd.ulCurItemTimeHigh=HI_HIU32(ulCurItemTime);
        stItemInd.ulPermillageTotal=(HI_U32)(ulCurItemTime*1000/ulCurTotalTime);
        stItemInd.ulPermillagePhase=(HI_U32)((ulCurItemTime-ulLastItemTime)*1000/(ulCurTotalTime-ulLastTotalTime));
        HI_DIAG_ReportPacket(ID_DIAG_CPUP_REPORT_ITEM_INFO,(HI_U8)ulOption,(HI_PBYTE)&stItemInd,sizeof(stItemInd),HI_TRUE);
        if(ret1 < 0)
        {
            continue;
        }
    }

    return ret;
}



//启动cpu统计
HI_U32 dfx_cpup_start(HI_U16 usId, HI_IN HI_PVOID pCmdParam, HI_U16 usCmdParamSize, HI_U32 ulOption)
{
    HI_U32 ret=HI_ERR_SUCCESS;
    HI_DFX_CPUP_ST_IND_S stInd={0};

    ret=HI_MDM_CPU_PercentStart();
    stInd.ulRet=ret;
    HI_DIAG_ReportPacket(usId,(HI_U8)ulOption,(HI_PBYTE)&stInd,sizeof(HI_DFX_CPUP_ST_IND_S),HI_TRUE);

    return HI_ERR_SUCCESS;
}
//停止cpu统计
HI_U32 dfx_cpup_end(HI_U16 usId, HI_IN HI_PVOID pCmdParam, HI_U16 usCmdParamSize, HI_U32 ulOption)
{
    HI_DFX_CPUP_END_IND_S stInd={0};
    stInd.ulRet = HI_MDM_CPU_PercentStop();
    HI_DIAG_ReportPacket(usId,(HI_U8)ulOption,(HI_PBYTE)&stInd,sizeof(HI_DFX_CPUP_END_IND_S),HI_TRUE);
    return HI_ERR_SUCCESS;
}
//获取cpup信息
HI_U32 dfx_cpup_get_info(HI_U16 usId, HI_IN HI_PVOID pCmdParam, HI_U16 usCmdParamSize, HI_U32 ulOption)
{
    report_cpup_info(ulOption);
    return HI_ERR_SUCCESS;
}