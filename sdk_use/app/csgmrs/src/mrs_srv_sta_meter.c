//*****************************************************************************
//
//                  ��Ȩ���� (C), 1998-2015, ��Ϊ�������޹�˾
//
//*****************************************************************************
//  �� �� ��   : mrs_srv_sta_meter.c
//  �� �� ��   : V1.0
//  ��    ��   : fengxiaomin/f00209182
//  ��������   : 2015-10-23
//  ��������   : STA�˵���ַģ��
//
//  �����б�   : TODO: ...
//  �޸���ʷ   :
//  1.��    �� : 2015-10-23
//    ��    �� : fengxiaomin/f00209182
//    �޸����� : �����ļ�
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

// ����ַ�ı�֪ͨ����������
MRS_METER_CHG_NOTIFY_CTX_S g_stMrsMeterChgNotifyCtx;

P_MRS_METER_CHG_NOTIFY_CTX_S mrsGetMeterChgNotiyProcCtx(HI_VOID);

// ��ȡ����ַ�ı�֪ͨ����������
P_MRS_METER_CHG_NOTIFY_CTX_S mrsGetMeterChgNotiyProcCtx(HI_VOID)
{
    return &g_stMrsMeterChgNotifyCtx;
}

// ����ַ�ı�֪ͨ�����ʼ��
HI_VOID mrsMeterChgNotifyProcInit(HI_VOID)
{
    P_MRS_METER_CHG_NOTIFY_CTX_S pstChgNotifyCtx = mrsGetMeterChgNotiyProcCtx();

    (hi_void)memset_s(pstChgNotifyCtx, sizeof(MRS_METER_CHG_NOTIFY_CTX_S), 0, sizeof(MRS_METER_CHG_NOTIFY_CTX_S));

    return;
}

// ����ַ�ı�֪ͨ������ע��
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

// ����ַ�ı�֪ͨ����������
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

HI_VOID mrsStaMeterRxNotify(HI_U8 *pucMeter, HI_U8 ucProtocol, MRS_645_CTRL_STRU *pstCtrl)
{
    HI_BOOL bChg = HI_FALSE;

    mrsStaSetMeterAddr(pucMeter);
    mrsStaSetMeterProtocol(ucProtocol);

    mrsStaCheckMeterChg(pucMeter, ucProtocol, &bChg);
    mrsMeterChgNotifyProc(bChg, pstCtrl->ucSlaveFlag);
    HI_DIAG_LOG_MSG_E2(MRS_FILE_LOG_FLAG_001, HI_DIAG_MT("StaMeterRxNotify bChg,ucSlaveFlag"),bChg,pstCtrl->ucSlaveFlag);

    if (bChg && (!pstCtrl->ucSlaveFlag))
    {
        //�����ַ�͹�Լд��NV
        mrsStaMeterInfoNvUpdate(pucMeter, ucProtocol);
        // �����������
        HI_MDM_UnlockNetwork();
        // ����������
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

#endif
