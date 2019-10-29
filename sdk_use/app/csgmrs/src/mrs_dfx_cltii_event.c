//*****************************************************************************
//
//                  版权所有 (C), 1998-2015, 华为技术有限公司
//
//*****************************************************************************
//  文 件 名   : mrs_dfx_cltii_event.c
//  版 本 号   : V1.0 
//  作    者   : cuiate/00233580
//  生成日期   : 2015-07-21
//  功能描述   : II采下挂电表事件维测信息 函数及接口实现
//               
//  函数列表   : TODO: ...
//  修改历史   : 
//  1.日    期 : 2015-07-21
//    作    者 : cuiate/00233580
//    修改内容 : 创建文件 
//
//*****************************************************************************

#include "mrs_common.h"
#include "mrs_fw_tools.h"
#include "mrs_srv_cltii_event.h"
#include "mrs_dfx_cltii_event.h"

HI_START_HEADER
#if defined(PRODUCT_CFG_PRODUCT_TYPE_STA)
#if !defined(PRODUCT_CFG_PRODUCT_TYPE_CSG_GD)

HI_PRV DIAG_CMD_MRS_GET_CLTII_EVENT_STATUS_STRU *g_pstDfxCltiiEventStatus = HI_NULL;
HI_PRV DIAG_CMD_MRS_GET_CLTII_EVENT_STATUS_STRU *mrsDfxCltiiEventGetCtx(HI_VOID);


DIAG_CMD_MRS_GET_CLTII_EVENT_STATUS_STRU *mrsDfxCltiiEventGetCtx(HI_VOID)
{
    return g_pstDfxCltiiEventStatus;
}


HI_U32 mrsDfxCltiiEventInit(HI_VOID)
{
    if (g_pstDfxCltiiEventStatus)
    {
        return HI_ERR_SUCCESS;
    }

    g_pstDfxCltiiEventStatus = (DIAG_CMD_MRS_GET_CLTII_EVENT_STATUS_STRU *)mrsToolsMalloc(sizeof(DIAG_CMD_MRS_GET_CLTII_EVENT_STATUS_STRU));
    if (!g_pstDfxCltiiEventStatus)
    {
        return HI_ERR_MALLOC_FAILUE;
    }

    (hi_void)memset_s(g_pstDfxCltiiEventStatus, sizeof(DIAG_CMD_MRS_GET_CLTII_EVENT_STATUS_STRU), 0, sizeof(DIAG_CMD_MRS_GET_CLTII_EVENT_STATUS_STRU));

    return HI_ERR_SUCCESS;
}


HI_VOID mrsDfxCltiiEventMonitorMeterInf(HI_U8 *pucMeter, HI_U8 ucProtocol)
{
    DIAG_CMD_MRS_GET_CLTII_EVENT_STATUS_STRU *pstStatus = mrsDfxCltiiEventGetCtx();

    if (pstStatus)
    {
        (hi_void)memcpy_s(pstStatus->aucLastMeterAddr, sizeof(pstStatus->aucLastMeterAddr), pucMeter, HI_METER_ADDR_LEN);
        pstStatus->ucLastMeterProtocol = ucProtocol;
    }
}


HI_VOID mrsDfxCltiiEventMonitorStart(HI_VOID)
{
    DIAG_CMD_MRS_GET_CLTII_EVENT_STATUS_STRU *pstStatus = mrsDfxCltiiEventGetCtx();

    if (pstStatus)
    {
        pstStatus->ulLastStartTS = HI_MDM_GetMilliSeconds();
        pstStatus->ucLastEventCount = 0;
        pstStatus->ucLastEventReported = 0;
		pstStatus->ucLastEventCleared = 0;
        pstStatus->ucStatus = 1;
    }
}


HI_VOID mrsDfxCltiiEventMonitorEnd(HI_VOID)
{
    DIAG_CMD_MRS_GET_CLTII_EVENT_STATUS_STRU *pstStatus = mrsDfxCltiiEventGetCtx();

    if (pstStatus)
    {
        pstStatus->ulLastEndTS = HI_MDM_GetMilliSeconds();
        pstStatus->ucStatus = 0;
    }
}


HI_VOID mrsDfxCltiiEventLastCount(HI_U8 ucCount)
{
    DIAG_CMD_MRS_GET_CLTII_EVENT_STATUS_STRU *pstStatus = mrsDfxCltiiEventGetCtx();

    if (pstStatus)
    {
        pstStatus->ucLastEventCount = ucCount;
        pstStatus->ulEventTotal += ucCount;
    }
}


HI_VOID mrsDfxCltiiEventLastReported(HI_U8 ucCount)
{
    DIAG_CMD_MRS_GET_CLTII_EVENT_STATUS_STRU *pstStatus = mrsDfxCltiiEventGetCtx();

    if (pstStatus)
    {
        pstStatus->ucLastEventReported += ucCount;
    }
}

HI_VOID mrsDfxCltiiEventLastCleared(HI_VOID)
{
    DIAG_CMD_MRS_GET_CLTII_EVENT_STATUS_STRU *pstStatus = mrsDfxCltiiEventGetCtx();

    if (pstStatus)
    {
        pstStatus->ucLastEventCleared++;
    }
}

HI_U32 mrsDfxGetCltiiEventStatus(HI_U16 usId, HI_IN HI_PVOID pCmdParam, HI_U16 usCmdParamSize, HI_U32 ulOption)
{
    DIAG_CMD_MRS_GET_CLTII_EVENT_STATUS_STRU *pstStatus = mrsDfxCltiiEventGetCtx();

    if (!pstStatus)
    {
        return HI_ERR_SUCCESS;
    }

    if (pstStatus->ucStatus)
    {
        pstStatus->ulLastEndTS = HI_MDM_GetMilliSeconds();
    }

    HI_MDM_ReportPacket(usId, (HI_U8)ulOption, (HI_PBYTE)pstStatus, sizeof(DIAG_CMD_MRS_GET_CLTII_EVENT_STATUS_STRU), HI_FALSE);

    return HI_ERR_SUCCESS;
}


HI_U32 mrsDfxGetCltiiEventInfo(HI_U16 usId, HI_IN HI_PVOID pCmdParam, HI_U16 usCmdParamSize, HI_U32 ulOption)
{
    MRS_CLTII_EVENT_CTX_STRU *pstEvtCtx = mrsGetCltiiEventCtx();
    MRS_CLTII_EVENT_METERLIST *pstMeterList = HI_NULL;
    DIAG_CMD_MRS_GET_CLTII_EVENT_INF_STRU *pstInf = HI_NULL;
    HI_U8 ucIndex = 0;

    if (!pstEvtCtx)
    {
        return HI_ERR_SUCCESS;
    }

    pstInf = (DIAG_CMD_MRS_GET_CLTII_EVENT_INF_STRU *)mrsToolsMalloc(sizeof(DIAG_CMD_MRS_GET_CLTII_EVENT_INF_STRU));
    if (!pstInf)
    {
        return HI_ERR_MALLOC_FAILUE;
    }

    pstMeterList = &(pstEvtCtx->stMeterList);

    do
    {
        HI_U8 i = 0;

        (hi_void)memset_s(pstInf, sizeof(DIAG_CMD_MRS_GET_CLTII_EVENT_INF_STRU), 0, sizeof(DIAG_CMD_MRS_GET_CLTII_EVENT_INF_STRU));
        pstInf->ucTotal = pstMeterList->ucMeterTotal;
        pstInf->ucCurrent = MRS_MIN(pstMeterList->ucMeterTotal - ucIndex, DIAG_CMD_MRS_CLT_EVENT_REPORT_MAX);

        for (i = 0; i < pstInf->ucCurrent; i++)
        {
            DIAG_CMD_MRS_CLTII_EVENT_STRU *pstEvt = pstInf->astEvent + i;

            (hi_void)memcpy_s(pstEvt->aucMeterAddr, sizeof(pstEvt->aucMeterAddr), pstMeterList->astMeterInf[ucIndex].aucAddr, HI_METER_ADDR_LEN);
            pstEvt->ucProtocol = pstMeterList->astMeterInf[ucIndex].ucProtocol;
            pstEvt->ucEventFlag = pstMeterList->aucEventInf[ucIndex];
            ucIndex++;
        }

        HI_MDM_ReportPacket(usId, (HI_U8)ulOption, (HI_PBYTE)pstInf, sizeof(DIAG_CMD_MRS_GET_CLTII_EVENT_INF_STRU), HI_FALSE);
    } while (ucIndex < pstMeterList->ucMeterTotal);

    mrsToolsFree(pstInf);

    return HI_ERR_SUCCESS;
}

#endif // !defined(PRODUCT_CFG_PRODUCT_TYPE_CSG_GD)
#endif // defined(PRODUCT_CFG_PRODUCT_TYPE_STA)
HI_END_HEADER

