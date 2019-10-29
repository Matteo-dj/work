//*****************************************************************************
//
//                  版权所有 (C), 1998-2015, 华为技术有限公司
//
//*****************************************************************************
//  文 件 名   : mrs_srv_sta_meter.c
//  版 本 号   : V1.0
//  作    者   : fengxiaomin/f00209182
//  生成日期   : 2015-10-23
//  功能描述   : STA端电表地址模块
//
//  函数列表   : TODO: ...
//  修改历史   :
//  1.日    期 : 2015-10-23
//    作    者 : fengxiaomin/f00209182
//    修改内容 : 创建文件
//
//*****************************************************************************
#include "mrs_common.h"
#include "mrs_srv_common.h"
#include "mrs_fw_tools.h"
#include "mrs_fw_log.h"
#include "mrs_srv_sta_meter.h"
#include "hi_mdm.h"
#include "mrs_dfx.h"
#include "mrs_srv_sta_queue.h"
#include "mrs_srv_parallel_sta.h"
#include "mrs_srv_sta_event.h"
#include "mrs_srv_sta.h"

#if defined(PRODUCT_CFG_PRODUCT_TYPE_STA)

#define MRS_METER_CHG_NOTIFY_NUM_MAX    (5)

typedef struct
{
    HI_U32 ulParam;
    MRS_METER_CHG_NOTIFY_PROC_F pfProc;
}MRS_METER_CHG_NOTIFY_PROC_S, *P_MRS_METER_CHG_NOTIFY_PROC_S;

typedef struct
{
    MRS_METER_CHG_NOTIFY_PROC_S stRegItem[MRS_METER_CHG_NOTIFY_NUM_MAX];
}MRS_METER_CHG_NOTIFY_CTX_S, *P_MRS_METER_CHG_NOTIFY_CTX_S;

HI_PRV HI_U32 mrsStaCheckMeterChg(HI_U8 *pucMeter, HI_U8 ucProtocol, HI_BOOL *pbChg);
HI_PRV HI_U32 mrsStaMeterInfoNvUpdate(HI_U8 *pucMeter, HI_U8 ucProtocol);

// 电表地址改变通知处理上下文
MRS_METER_CHG_NOTIFY_CTX_S g_stMrsMeterChgNotifyCtx;

P_MRS_METER_CHG_NOTIFY_CTX_S mrsGetMeterChgNotiyProcCtx(HI_VOID);

// 获取电表地址改变通知处理上下文
P_MRS_METER_CHG_NOTIFY_CTX_S mrsGetMeterChgNotiyProcCtx(HI_VOID)
{
    return &g_stMrsMeterChgNotifyCtx;
}

// 电表地址改变通知处理初始化
HI_VOID mrsMeterChgNotifyProcInit(HI_VOID)
{
    P_MRS_METER_CHG_NOTIFY_CTX_S pstChgNotifyCtx = mrsGetMeterChgNotiyProcCtx();

    (hi_void)memset_s(pstChgNotifyCtx, sizeof(MRS_METER_CHG_NOTIFY_CTX_S), 0, sizeof(MRS_METER_CHG_NOTIFY_CTX_S));

    return;
}

// 电表地址改变通知处理函数注册
HI_U32 mrsMeterChgNotifyProcReg(MRS_METER_CHG_NOTIFY_PROC_F pfProc, HI_U32 ulParam)
{
    HI_U16 usIndex = 0;
    P_MRS_METER_CHG_NOTIFY_CTX_S pstChgNotifyCtx = mrsGetMeterChgNotiyProcCtx();

    for(usIndex = 0; usIndex < MRS_METER_CHG_NOTIFY_NUM_MAX; usIndex++)
    {
        if(pstChgNotifyCtx->stRegItem[usIndex].pfProc == HI_NULL)
        {
            pstChgNotifyCtx->stRegItem[usIndex].pfProc = pfProc;
            pstChgNotifyCtx->stRegItem[usIndex].ulParam = ulParam;

            return HI_ERR_SUCCESS;
        }
    }

    return HI_ERR_FULL;
}

// 电表地址改变通知处理函数调用
HI_U32 mrsMeterChgNotifyProc(HI_BOOL bChg, HI_U8 ucSlaveFlag)
{
    HI_U16 usIndex = 0;
    P_MRS_METER_CHG_NOTIFY_CTX_S pstChgNotifyCtx = mrsGetMeterChgNotiyProcCtx();

    for(usIndex = 0; usIndex < MRS_METER_CHG_NOTIFY_NUM_MAX; usIndex++)
    {
        P_MRS_METER_CHG_NOTIFY_PROC_S pstItem = &pstChgNotifyCtx->stRegItem[usIndex];

        if(pstItem->pfProc)
        {
            pstItem->pfProc(bChg, ucSlaveFlag, pstItem->ulParam);
        }
    }

    return HI_ERR_SUCCESS;
}

HI_VOID mrsStaMeterRxNotify(HI_U8 *pucMeter, HI_U8 ucProtocol, HI_U8 ucSlaveFlag)
{
    HI_BOOL bChg = HI_FALSE;

    mrsStaCheckMeterChg(pucMeter, ucProtocol, &bChg);
    mrsMeterChgNotifyProc(bChg, ucSlaveFlag);
    HI_DIAG_LOG_MSG_E2(MRS_FILE_LOG_FLAG_001, HI_DIAG_MT("StaMeterRxNotify bChg,ucSlaveFlag"), bChg, ucSlaveFlag);

    if (bChg && !ucSlaveFlag)
    {
        //将表地址和规约写入NV
        mrsStaMeterInfoNvUpdate(pucMeter, ucProtocol);
        // 解除网络锁定
        HI_MDM_UnlockNetwork();
        // 清除网络归属
        HI_MDM_ClearBelongNetwork();
        HI_MDM_ClearSoftTfNtbResult(HI_FALSE, HI_FALSE);
    }
}

HI_U32 mrsStaCheckMeterChg(HI_U8 *pucMeter, HI_U8 ucProtocol, HI_BOOL *pbChg)
{
    HI_U32 ulRet = HI_ERR_SUCCESS;
    HI_NV_FTM_COLLECT_PARAM_FILE_S stNv;

    (hi_void)memset_s(&stNv, sizeof(stNv), 0, sizeof(stNv));
    ulRet = HI_MDM_NV_Read(HI_NV_FTM_COLLECT_PARAM_FILE, &stNv, sizeof(stNv));
    if (HI_ERR_SUCCESS != ulRet)
    {
        *pbChg = HI_TRUE;
        return HI_ERR_FAILURE;
    }

    if (!mrsToolsMemEq(pucMeter, stNv.meter, HI_METER_ADDR_LEN) || (ucProtocol != stNv.protocol))
    {
        *pbChg = HI_TRUE;
    }
    else
    {
        *pbChg = HI_FALSE;
    }

    return HI_ERR_SUCCESS;
}

HI_U32 mrsStaMeterInfoNvUpdate(HI_U8 *pucMeter, HI_U8 ucProtocol)
{
    HI_U32 ulRet = HI_ERR_SUCCESS;
    HI_NV_FTM_COLLECT_PARAM_FILE_S stNv;

    ulRet = HI_MDM_NV_Read(HI_NV_FTM_COLLECT_PARAM_FILE, (HI_PVOID)&stNv, sizeof(HI_NV_FTM_COLLECT_PARAM_FILE_S));
    if (HI_ERR_SUCCESS != ulRet)
    {
        return ulRet;
    }

    (hi_void)memcpy_s(stNv.meter, HI_METER_ADDR_LEN, pucMeter, HI_METER_ADDR_LEN);
    stNv.protocol = ucProtocol;

    return HI_MDM_NV_Write(HI_NV_FTM_COLLECT_PARAM_FILE, (HI_PVOID)&stNv, sizeof(HI_NV_FTM_COLLECT_PARAM_FILE_S));
}

HI_VOID mrsStaGetMeterAddr(HI_U8 *pucMeter)
{
    (hi_void)memcpy_s(pucMeter, HI_METER_ADDR_LEN, mrsStaGetContext()->aucMeter, HI_METER_ADDR_LEN);
}

HI_VOID mrsStaSetMeterAddr(HI_U8 *pucMeter)
{
    (hi_void)memcpy_s(mrsStaGetContext()->aucMeter, HI_METER_ADDR_LEN, pucMeter, HI_METER_ADDR_LEN);
}

HI_VOID mrsStaSetMeterProtocol(HI_U8 ucProtocol)
{
    mrsStaGetContext()->ucProtocol = ucProtocol;
}

HI_U8 mrsStaGetMeterProtocol(HI_VOID)
{
    return mrsStaGetContext()->ucProtocol;
}

HI_VOID mrsStaSetMeterBaudrate(HI_U16 usBaudrate)
{
    mrsStaGetContext()->usBaudrate = usBaudrate;
}

HI_U16 mrsStaGetMeterBaudrate(HI_VOID)
{
    return mrsStaGetContext()->usBaudrate;
}

#endif
