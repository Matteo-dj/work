#include "drv_dfx_cmn_inc.h"
#include "drv_dfx_os_info.h"
#include <sal_os_dfx.h>
#include <hi_mdm_mem.h>
#include <stdlib.h>


DRV_DFX_TIMER_INFO dfx_timer_info;
DRV_DFX_TASK_INFO dfx_task_info;
HI_U32 dfx_task_show(HI_U16 usId, HI_IN HI_PVOID pCmdParam, HI_U16 usCmdParamSize, HI_U32 ulOption)
{
    HI_PVOID    pInfo = HI_NULL;
    HI_S32      taskNum = 0;
    HI_U32      ret;
    DRV_DFX_TASK_INFO_UPLOAD * pInfoUpload = HI_NULL;

    pInfo = dfx_task_info.astEntrys;
    /* report structure to HSO */
    ret = HI_SYS_GetTaskNum((HI_U32 *)&taskNum);
    if (ret != HI_ERR_SUCCESS)
    {
        return HI_ERR_FAILURE;
    }
    //兼容原有版本，超过DRV_DFX_TASK_MAX暂不显示待优化
    if (taskNum > DRV_DFX_TASK_MAX)
    {
        taskNum = DRV_DFX_TASK_MAX;
    }
    (HI_VOID)HI_SYS_GetTaskInfo((HI_U32)taskNum, pInfo);
    dfx_task_info.ulCnt = (HI_U32)taskNum;

    pInfoUpload = (DRV_DFX_TASK_INFO_UPLOAD *)malloc(sizeof(DRV_DFX_TASK_INFO_UPLOAD));
    if (pInfoUpload == HI_NULL)
    {
		HI_DIAG_LOG_MSG_E0(dfx_task_show_018, HI_DIAG_MT("dfx_task_show alloc memory fail!\n"));
        return HI_ERR_MEM_POOL_REQUEST_FAIL;
    }

    (hi_void)memset_s((HI_VOID *)pInfoUpload, sizeof(DRV_DFX_TASK_INFO_UPLOAD), 0, sizeof(DRV_DFX_TASK_INFO_UPLOAD));

    pInfo = dfx_task_info.astEntrys;

    for (; taskNum > 0 ; taskNum -= DRV_DFX_TASK_UPLOAD)
    {
        pInfoUpload->ulCnt = (HI_U32)((taskNum > DRV_DFX_TASK_UPLOAD) ? DRV_DFX_TASK_UPLOAD : taskNum);
        if (taskNum <= DRV_DFX_TASK_UPLOAD)
        {
            pInfoUpload->bIsLast = HI_TRUE;
        }

        (hi_void)memcpy_s(pInfoUpload->astEntrys, DRV_DFX_TASK_UPLOAD * sizeof(DFX_OS_TASK_INFO), pInfo, DRV_DFX_TASK_UPLOAD * sizeof(DFX_OS_TASK_INFO));
        HI_DIAG_ReportPacket(usId, (HI_U8)ulOption,
            (HI_PBYTE)pInfoUpload, sizeof(DRV_DFX_TASK_INFO_UPLOAD), HI_TRUE);

        pInfo = (HI_PVOID)((HI_U32)pInfo + DRV_DFX_TASK_UPLOAD * sizeof(DFX_OS_TASK_INFO));
    }
    
    free(pInfoUpload);
    return HI_ERR_SUCCESS;
}



HI_U32 dfx_timer_show(HI_U16 usId, HI_IN HI_PVOID pCmdParam, HI_U16 usCmdParamSize, HI_U32 ulOption)
{
    HI_U32 ulCnt;
    HI_U32 ret;

    (hi_void)memset_s((HI_VOID *)dfx_timer_info.astEntrys, sizeof(dfx_timer_info.astEntrys), 0x0, sizeof(dfx_timer_info.astEntrys));
    ret = HI_SYS_GetTimerNum(&ulCnt);
    if (HI_ERR_SUCCESS != ret)
    {
        dfx_timer_info.ulCnt = 0;
    }
    else
    {
        //兼容原有版本，超过DRV_DFX_TIMER_MAX暂不显示待优化
        if (ulCnt > DRV_DFX_TIMER_MAX)
        {
            ulCnt = DRV_DFX_TIMER_MAX;
        }
        (HI_VOID)HI_SYS_GetTimerInfo(ulCnt, dfx_timer_info.astEntrys);
        dfx_timer_info.ulCnt = ulCnt;
    }
    dfx_timer_info.bIsLast = HI_TRUE;

    HI_DIAG_ReportPacket(usId, (HI_U8)ulOption,
        (HI_PBYTE)&dfx_timer_info, (HI_U16)sizeof(DRV_DFX_TIMER_INFO), HI_TRUE);

    return HI_ERR_SUCCESS;
}

//临时函数
#define IND_STR_SIZE 1024
HI_U32  new_mem_show_temp(HI_U16 usId, HI_IN HI_PVOID pCmdParam, HI_U16 usCmdParamSize, HI_U32 ulOption)
{
    HI_MDM_MEM_INFO_S  stSysMemInfo = {0};
    HI_MDM_MEM_POOL_INFO_S stMemPoolInfo = {0};
    HI_U32 s_help_len = 0;

    HI_U32 *p_temp_ind_str = (HI_U32 *)HI_MDM_Malloc(HI_MOD_ID_DRV_DFX, IND_STR_SIZE);
    if(NULL == p_temp_ind_str)
    {
        HI_DIAG_ReportPacket(usId, (HI_U8)ulOption, (HI_PBYTE)STR_IND_NOT_ENOUGH_MEMORY,(HI_U16)strlen(STR_IND_NOT_ENOUGH_MEMORY)+1,HI_TRUE);
        return HI_ERR_MALLOC_FAILUE;
    }
    
    s_help_len = IND_STR_SIZE;
    HI_MDM_MEM_GetSysInfo(&stSysMemInfo);
    (hi_void)memset_s((HI_VOID *)p_temp_ind_str,s_help_len, 0,s_help_len);
    (hi_void)snprintf_s((HI_CHAR *)p_temp_ind_str, s_help_len, s_help_len - 1,\
             " total= %d k ,used = %d k, free = %d k ,\n\r  text = %d k ,\t data = %d k, \t rodata = %d k ,\t bsslen = %d k mmuPageTableSize = %d k \r\n ", \
             stSysMemInfo.heapTotal / 1024, stSysMemInfo.heapUsed / 1024, stSysMemInfo.heapFree / 1024, \
             stSysMemInfo.codeSize / 1024, stSysMemInfo.dataSize / 1024, stSysMemInfo.rodataSize / 1024,\
             stSysMemInfo.bssSize / 1024,stSysMemInfo.mptSize/1024);
    HI_DIAG_ReportPacket((HI_U32)usId, (HI_U8)ulOption, (HI_PBYTE)p_temp_ind_str, (HI_U16)strlen((HI_CHAR *)p_temp_ind_str), HI_TRUE);
    
    
    HI_MDM_MEM_GetSysPoolInfo(&stMemPoolInfo);
    (hi_void)memset_s((HI_VOID *)p_temp_ind_str, s_help_len, 0, s_help_len);
    (hi_void)snprintf_s((HI_CHAR *)p_temp_ind_str, s_help_len, s_help_len - 1,\
         "SysPoll  poolId:0x%x,total:%d,used:%d,free:%d,malloctimes:%d(suc)-%d(fail),freetimes:%d(suc)-%d(fail)\r\n ", \
         stMemPoolInfo.ulPoolId, stMemPoolInfo.ulTotalSize, stMemPoolInfo.ulTotalUsedSize, stMemPoolInfo.ulTotalFreeSize, \
         stMemPoolInfo.mallocSucTimes, stMemPoolInfo.mallocFailTimes, stMemPoolInfo.freeSucTimes, \
         stMemPoolInfo.freeFailTimes);
    HI_DIAG_ReportPacket((HI_U32)usId, (HI_U8)ulOption,
                         (HI_PBYTE)p_temp_ind_str, (HI_U16)strlen((HI_CHAR *)p_temp_ind_str), HI_TRUE);

    HI_MDM_MEM_GetDynPoolInfo(&stMemPoolInfo);
    (hi_void)memset_s(p_temp_ind_str, s_help_len, 0, s_help_len);
    (hi_void)snprintf_s((HI_CHAR*)p_temp_ind_str, s_help_len, s_help_len - 1,\
             "DynPool  poolId:0x%x,total:%d,used:%d,free:%d,malloctimes:%d(suc)-%d(fail),freetimes:%d(suc)-%d(fail)\r\n ", \
             stMemPoolInfo.ulPoolId, stMemPoolInfo.ulTotalSize, stMemPoolInfo.ulTotalUsedSize, stMemPoolInfo.ulTotalFreeSize, \
             stMemPoolInfo.mallocSucTimes, stMemPoolInfo.mallocFailTimes, stMemPoolInfo.freeSucTimes, \
             stMemPoolInfo.freeFailTimes);
    HI_DIAG_ReportPacket(usId, (HI_U8)ulOption,
                         (HI_PBYTE)p_temp_ind_str, (HI_U16)strlen((HI_CHAR *)p_temp_ind_str), HI_TRUE);

    HI_MDM_MEM_GetStaticPoolInfo(&stMemPoolInfo);
    (hi_void)memset_s(p_temp_ind_str, s_help_len, 0, s_help_len);
    (hi_void)snprintf_s((HI_CHAR*)p_temp_ind_str, s_help_len, s_help_len - 1,\
             "StaticPool  poolId:0x%x,total:%d,used:%d,free:%d,malloctimes:%d(suc)-%d(fail),freetimes:%d(suc)-%d(fail)\r\n ", \
             stMemPoolInfo.ulPoolId, stMemPoolInfo.ulTotalSize, stMemPoolInfo.ulTotalUsedSize, stMemPoolInfo.ulTotalFreeSize, \
             stMemPoolInfo.mallocSucTimes, stMemPoolInfo.mallocFailTimes, stMemPoolInfo.freeSucTimes, \
             stMemPoolInfo.freeFailTimes);
    HI_DIAG_ReportPacket(usId, (HI_U8)ulOption,
                         (HI_PBYTE)p_temp_ind_str, (HI_U16)strlen((HI_CHAR *)p_temp_ind_str), HI_TRUE);

    HI_MDM_Free(HI_MOD_ID_DRV_DFX, p_temp_ind_str);

    return HI_ERR_SUCCESS;
}

