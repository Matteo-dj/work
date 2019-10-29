//*****************************************************************************
//
//                  版权所有 (C), 1998-2011, 华为技术有限公司
//
//*****************************************************************************
//  文 件 名   : mrs_srv_common.c
//  版 本 号   : V1.0 
//  作    者   : liuxipeng/KF54842
//  生成日期   : 2014-06-26
//  功能描述   : 读表应用公用函数
//               实现读表应用公用函数(CCO & STA)及资源的实现与管理
//               
//  函数列表   : 
//  修改历史   : 
//  1.日    期 : 2014-06-26
//    作    者 : liuxipeng/KF54842
//    修改内容 : 创建文件 
//
//*****************************************************************************
#include "mrs_common.h"
#include "app_nv.h"
#include "mrs_fw_tools.h"
#include "mrs_fw_log.h"
#include "mrs_fw_n.h"
#include "mrs_srv_common.h"
#include "mrs_fw_proto376_2_echo.h"
#include "mrs_fw_nv.h"


#if defined(PRODUCT_CFG_PRODUCT_TYPE_STA)

#define MRS_SRV_CHL_NOTIFY_PROC_NUM_MAX    (5)

typedef struct
{
    HI_U32 ulParam;
    MRS_CHL_NOTIFY_PROC_F pfProc;
}MRS_SRV_CHL_NOTIFY_PROC_S, *P_MRS_SRV_CHL_NOTIFY_PROC_S;

typedef struct
{
    MRS_SRV_CHL_NOTIFY_PROC_S stRegItem[MRS_SRV_CHL_NOTIFY_PROC_NUM_MAX];
}MRS_SRV_CHL_NOTIFY_CTX_S, *P_MRS_SRV_CHL_NOTIFY_CTX_S;

// 通道改变通知处理上下文
MRS_SRV_CHL_NOTIFY_CTX_S g_stMrsChlNotifyProcCtx;

P_MRS_SRV_CHL_NOTIFY_CTX_S mrsSrvGetChlNotiyProcCtx(HI_VOID);

// 获取通道改变通知处理上下文
P_MRS_SRV_CHL_NOTIFY_CTX_S mrsSrvGetChlNotiyProcCtx(HI_VOID)
{
    return &g_stMrsChlNotifyProcCtx;
}

// 通道改变通知处理初始化
HI_VOID mrsSrvChlNotifyProcInit(HI_VOID)
{
    P_MRS_SRV_CHL_NOTIFY_CTX_S pstChlNotifyCtx = mrsSrvGetChlNotiyProcCtx();

    (hi_void)memset_s(pstChlNotifyCtx, sizeof(MRS_SRV_CHL_NOTIFY_CTX_S), 0, sizeof(MRS_SRV_CHL_NOTIFY_CTX_S));

    return;
}

// 通道改变通知处理函数注册
HI_U32 mrsSrvChlNotifyProcReg(MRS_CHL_NOTIFY_PROC_F pfProc, HI_U32 ulParam)
{
    HI_U16 usIndex = 0;
    P_MRS_SRV_CHL_NOTIFY_CTX_S pstChlNotifyCtx = mrsSrvGetChlNotiyProcCtx();
        
    for(usIndex = 0; usIndex < MRS_SRV_CHL_NOTIFY_PROC_NUM_MAX; usIndex++)
    {
        if(pstChlNotifyCtx->stRegItem[usIndex].pfProc == HI_NULL)
        {
            pstChlNotifyCtx->stRegItem[usIndex].pfProc = pfProc;
            pstChlNotifyCtx->stRegItem[usIndex].ulParam = ulParam;

            return HI_ERR_SUCCESS;
        }
    }

    return HI_ERR_FULL;
}

// 通道改变通知处理函数清理
HI_U32 mrsSrvChlNotifyProcClear(MRS_CHL_NOTIFY_PROC_F pfProc)
{
    HI_U16 usIndex = 0;
    P_MRS_SRV_CHL_NOTIFY_CTX_S pstChlNotifyCtx = mrsSrvGetChlNotiyProcCtx();
        
    for(usIndex = 0; usIndex < MRS_SRV_CHL_NOTIFY_PROC_NUM_MAX; usIndex++)
    {
        if(pstChlNotifyCtx->stRegItem[usIndex].pfProc == pfProc)
        {
            pstChlNotifyCtx->stRegItem[usIndex].ulParam = 0;
            pstChlNotifyCtx->stRegItem[usIndex].pfProc = HI_NULL;

            return HI_ERR_SUCCESS;
        }
    }

    return HI_ERR_NOT_FOUND;
}

// 通道改变通知处理函数调用
HI_U32 mrsSrvChlNotifyProc(HI_BOOL bChlStatus)
{
    HI_U16 usIndex = 0;
    P_MRS_SRV_CHL_NOTIFY_CTX_S pstChlNotifyCtx = mrsSrvGetChlNotiyProcCtx();
        
    for(usIndex = 0; usIndex < MRS_SRV_CHL_NOTIFY_PROC_NUM_MAX; usIndex++)
    {
        HI_BOOL bOneShot = HI_FALSE;
        P_MRS_SRV_CHL_NOTIFY_PROC_S pstItem = &pstChlNotifyCtx->stRegItem[usIndex];
        
        if(pstItem->pfProc != HI_NULL)
        {
            pstItem->pfProc(bChlStatus, pstItem->ulParam, &bOneShot);
            if(bOneShot)
            {
                pstItem->ulParam = 0;
                pstItem->pfProc = HI_NULL;
            }
        }
    }

    return HI_ERR_SUCCESS;
}

#endif // defined(PRODUCT_CFG_PRODUCT_TYPE_STA)


//*****************************************************************************
// 函数名称: mrsSrvGetVerInfo
// 功能描述: 获取模块的软件版本和boot版本
//           
// 参数说明:
//   pusVer     [OUT]软件版本
//   pucBootVer [OUT]boot版本
//
// 返 回 值: 
//    HI_ERR_SUCCESS:  成功
//    其他: 失败
//
// 调用要求: TODO: ...
// 调用举例: TODO: ...
// 作    者: fengxiaomin/00209182 [2015-10-31]
//*****************************************************************************
HI_U32 mrsSrvGetVerInfo(HI_U16 *pusVer, HI_U8 *pucBootVer)
{
	HI_MDM_SYS_INFO_S *pstVerInfo = HI_NULL;
	
    pstVerInfo = (HI_MDM_SYS_INFO_S *)HI_MDM_GetSysInfo();
	if (!pstVerInfo)
	{
		return HI_ERR_FAILURE;
	}

	*pusVer = pstVerInfo->usSwVer;
	*pucBootVer = pstVerInfo->ucBootVer;

	return HI_ERR_SUCCESS;
}

