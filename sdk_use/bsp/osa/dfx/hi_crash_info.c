//*****************************************************************************
//
//                  版权所有 (C), 2017-2027, 华为技术有限公司
//
//*****************************************************************************
//  文 件 名   : hi_crash_info.c
//  版 本 号   : V1.0
//  作    者   : chenghuanhuan/375526
//  生成日期   : 2017-05-04
//  功能描述   : 上次复位详细原因
//
//  函数列表   : TODO: ...
//  修改历史   :
//  1.日    期 : 2017-05-04
//    作    者 : Cui Ate/c00233580
//    修改内容 : 创建文件
//
//*****************************************************************************
#include "hi_mdm.h"
//#include "dfx_sal.h"
#include "hi_sal.h"
#include "sal_common.h"
#include <hi_mdm_crash.h>
#define HI_SAL_USER_CRASH_BUFFER_SIZE   (256)


HI_U32 HI_MDM_GetSysCrashInfo(HI_MDM_SYS_CRASH_INFO_S *pstCrashInfo)
{
    HI_PBYTE pucInfo = HI_NULL;
    HI_U16 usItemsRead = 0;
    HI_U32 ulObjSize = 0;

    if (!pstCrashInfo)
    {
        return HI_ERR_INVALID_PARAMETER;
    }

    if (sas_GetSysErrMaxSize() == 0)
    {
        return HI_ERR_NOT_SUPPORT;
    }

    pucInfo = (HI_PBYTE)HI_MDM_Malloc(HI_MOD_ID_SAL_DFX, SAL_CRASH_INFO_MAX_SIZE);
    if (!pucInfo)
    {
        return HI_ERR_NOT_ENOUGH_MEMORY;
    }

    memset_s(pucInfo, SAL_CRASH_INFO_MAX_SIZE, 0, SAL_CRASH_INFO_MAX_SIZE);

    HI_SYS_GetUserData(HI_USERDATA_ID_CRASH_HRD, 0, 1, (HI_U16)sas_GetSysErrMaxSize(), &usItemsRead, (HI_PVOID)pucInfo);
    if (!sas_CheckSdmSysErrCtx((HI_PVOID)pucInfo))
    {
        memset_s((HI_PVOID)pucInfo, SAL_CRASH_INFO_MAX_SIZE, 0, SAL_CRASH_INFO_MAX_SIZE);
    }

    sas_SetSdmSysErrRunTime((HI_PVOID)pucInfo);
    sas_GetErrR(&ulObjSize);

    (HI_VOID)memcpy_s(pstCrashInfo,sizeof(HI_MDM_SYS_CRASH_INFO_S), pucInfo, sizeof(HI_MDM_SYS_CRASH_INFO_S));

    HI_MDM_Free(HI_MOD_ID_SAL_DFX, pucInfo);
    return HI_ERR_SUCCESS;
}






