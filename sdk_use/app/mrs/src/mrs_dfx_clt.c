//*****************************************************************************
//
//                  版权所有 (C), 1998-2014, 华为技术有限公司
//
//*****************************************************************************
//  文 件 名   : mrs_dfx_clt.c
//  版 本 号   : V1.0 
//  作    者   : liupeidong/00238134
//  生成日期   : 2014-04-03
//  功能描述   : 采集器搜表相关统计量及接口定义
//               
//  函数列表   : TODO: ...
//  修改历史   : 
//  1.日    期 : 2014-04-03
//    作    者 : liupeidong/00238134
//    修改内容 : 创建文件 
//
//*****************************************************************************
#include "mrs_common.h"
#include "mrs_cmd_msg.h"
#include "mrs_srv_common.h"
#include "mrs_fw_tools.h"
#include "mrs_fw_log.h"
#include "app_nv.h"
#include "mrs_srv_sta_queue.h"
#include "mrs_srv_parallel_sta.h"
#include "mrs_srv_sta_event.h"
#include "mrs_srv_sta.h"
#include "mrs_srv_sta_searchmeter.h"
#include "mrs_srv_clti_searchmeter.h"
#include "mrs_srv_cltii_searchmeter.h"
#include "mrs_srv_collector.h"
#include "hi_mdm.h"
#include "mrs_srv_sta_baudrate.h"
#include "mrs_dfx_clt.h"



#if defined(PRODUCT_CFG_PRODUCT_TYPE_STA) 

HI_PRV DIAG_CMD_MRS_GET_CLT_SEARCH_STATS_IND_STRU g_stCltSearchStats = { 0 };
HI_PRV DIAG_CMD_MRS_GET_CLT_SEARCH_LOCAL_IND_STRU g_stCltSearchLocal = { 0 };
HI_PRV DIAG_CMD_MRS_GET_CLT_SEARCH_LOCAL_EX_IND_STRU g_stCltSearchLocalEx = {{0}, {0,}, {0}};

    
DIAG_CMD_MRS_GET_CLT_SEARCH_STATS_IND_STRU* mrsDfxCltGetSearchStats(HI_VOID)
{
    return &g_stCltSearchStats;
}


DIAG_CMD_MRS_GET_CLT_SEARCH_LOCAL_IND_STRU* mrsDfxCltGetSearchLocal(HI_VOID)
{
    return &g_stCltSearchLocal;
}


MRS_DFX_SEARCH_FRAME_STATS_STRU* mrsDfxCltGetSearchFrame(HI_VOID)
{
    MRS_STA_SEARCH_CTX *pstSmCtx = mrsStaSearchCtx();

    return &(pstSmCtx->stFrameStats);
}


HI_VOID mrsDfxCltResetPlcSearchStats(HI_VOID)
{
    g_stCltSearchStats.ulSearchSN = 0;
    g_stCltSearchStats.usPlcStartRx = 0;
    g_stCltSearchStats.usPlcStartRxLTS = 0;
    g_stCltSearchStats.usPlcQueryRx = 0;
    g_stCltSearchStats.usPlcQueryTx = 0;
    g_stCltSearchStats.ulPlcQueryRxLTS = 0;
    g_stCltSearchStats.ulPlcQueryTxLTS = 0;
}


HI_VOID mrsDfxCltResetComSearchStats(HI_VOID)
{
    g_stCltSearchStats.ucTotalMeter = 0;
    g_stCltSearchStats.ucSearchStatus = 0;
    g_stCltSearchStats.usComStartRx = 0;
    g_stCltSearchStats.usComStartTx = 0;
    g_stCltSearchStats.ulComStartRxLTS = 0;
    g_stCltSearchStats.ulComStartTxLTS = 0;
    g_stCltSearchStats.usComQueryRx = 0;
    g_stCltSearchStats.usComQueryTx = 0;
    g_stCltSearchStats.ulComQueryRxLTS = 0;
    g_stCltSearchStats.ulComQueryTxLTS = 0;
    g_stCltSearchStats.ulComFirstQueryLTS = 0;
    g_stCltSearchStats.ulComQueryResultLTS = 0;
}


HI_VOID mrsDfxCltResetSearchLocal(HI_VOID)
{
    g_stCltSearchLocal.ulStartTime = 0;
    g_stCltSearchLocal.ulEndTime = 0;
    g_stCltSearchLocal.ucSearchStatus = 0;
    g_stCltSearchLocal.ucProtocol = 0;
    g_stCltSearchLocal.usBaudrate = 0;
    g_stCltSearchLocal.ulSearchDI = 0;
    (hi_void)memset_s(g_stCltSearchLocal.ucSearchAddr, sizeof(g_stCltSearchLocal.ucSearchAddr), 0, sizeof(g_stCltSearchLocal.ucSearchAddr));
    (hi_void)memset_s(&g_stCltSearchLocal.stProtocol07, sizeof(MRS_CLT_SEARCH_LOCAL_STATS), 0, sizeof(MRS_CLT_SEARCH_LOCAL_STATS));
    (hi_void)memset_s(&g_stCltSearchLocal.stProtocol97a, sizeof(MRS_CLT_SEARCH_LOCAL_STATS), 0, sizeof(MRS_CLT_SEARCH_LOCAL_STATS));
    (hi_void)memset_s(&g_stCltSearchLocal.stProtocol97b, sizeof(MRS_CLT_SEARCH_LOCAL_STATS), 0, sizeof(MRS_CLT_SEARCH_LOCAL_STATS));
    (hi_void)memset_s(&g_stCltSearchLocalEx, sizeof(g_stCltSearchLocalEx), 0, sizeof(g_stCltSearchLocalEx));
}


HI_VOID mrsDfxCltResetSearchFrame(HI_VOID)
{
    MRS_DFX_SEARCH_FRAME_STATS_STRU* pstFrameStats = mrsDfxCltGetSearchFrame();
    
    (hi_void)memset_s(pstFrameStats, sizeof(MRS_DFX_SEARCH_FRAME_STATS_STRU), 0, sizeof(MRS_DFX_SEARCH_FRAME_STATS_STRU));
}


HI_U32 mrsDfxGetCltSearchResult(HI_U16 usId, HI_IN HI_PVOID pCmdParam, HI_U16 usCmdParamSize, HI_U32 ulOption)
{
    DIAG_CMD_MRS_GET_CLT_SEARCH_RESULT_IND_STRU *pstResult = mrsToolsMalloc(sizeof(DIAG_CMD_MRS_GET_CLT_SEARCH_RESULT_IND_STRU));
    MRS_STA_SEARCH_CTX *pstSmCtx = mrsStaSearchCtx();
    MRS_SEARCH_METERLIST_STRU * pstMeterList = &(pstSmCtx->stMeterList);
    HI_U8 ucTotal = pstMeterList->ucMeterNum;
    HI_U8 ucReport = 0;
    HI_U8 ucIndex = 0;
    HI_U16 usBaudRate = 0;

    if (!pstResult)
    {
        return HI_ERR_NOT_ENOUGH_MEMORY;
    }

    MRS_NOT_USED(pCmdParam);
    MRS_NOT_USED(usCmdParamSize);

    if (ucTotal == 0)
    {
        (hi_void)memset_s(pstResult, sizeof(DIAG_CMD_MRS_GET_CLT_SEARCH_RESULT_IND_STRU), 0, sizeof(DIAG_CMD_MRS_GET_CLT_SEARCH_RESULT_IND_STRU)); 
        pstResult->ucSearchStatus = mrsStaSearchGetStatus();
        pstResult->ucEndReason = mrsStaSearchEndReason();
        HI_MDM_ReportPacket(usId, (HI_U8)ulOption, (HI_PBYTE)pstResult, sizeof(DIAG_CMD_MRS_GET_CLT_SEARCH_RESULT_IND_STRU), HI_TRUE);
        mrsToolsFree(pstResult);
        return HI_ERR_SUCCESS;
    }

    while (ucReport < ucTotal)
    {
        (hi_void)memset_s(pstResult, sizeof(DIAG_CMD_MRS_GET_CLT_SEARCH_RESULT_IND_STRU), 0, sizeof(DIAG_CMD_MRS_GET_CLT_SEARCH_RESULT_IND_STRU));
        pstResult->ucSearchStatus = mrsStaSearchGetStatus();
        pstResult->ucEndReason = mrsStaSearchEndReason();
        pstResult->ucTotal = ucTotal;
        pstResult->ucCurrent = MRS_MIN(ucTotal - ucReport, DIAG_CMD_MRS_SEARCH_METER_NUM);
        
        for (ucIndex = 0; ucIndex < pstResult->ucCurrent; ucIndex++)
        {
            (hi_void)memcpy_s(pstResult->stMeter[ucIndex].ucAddr, sizeof(pstResult->stMeter[ucIndex].ucAddr),
                pstMeterList->astMeter[ucReport + ucIndex].ucMeterAddr, HI_METER_ADDR_LEN);
            pstResult->stMeter[ucIndex].ucProtocol = pstMeterList->astMeter[ucReport + ucIndex].ucProtocol;
            usBaudRate = pstMeterList->astMeter[ucReport + ucIndex].usBaudRate;
            pstResult->stMeter[ucIndex].ucBaudrate = (HI_U8)(usBaudRate / MRS_DFX_BAUDRATE_100);
        }

        HI_MDM_ReportPacket(usId, (HI_U8)ulOption, (HI_PBYTE)pstResult, sizeof(DIAG_CMD_MRS_GET_CLT_SEARCH_RESULT_IND_STRU), HI_TRUE);
        ucReport += pstResult->ucCurrent;
    }

    mrsToolsFree(pstResult);

    return HI_ERR_SUCCESS;
}


HI_U32 mrsDfxGetCltSearchStats(HI_U16 usId, HI_IN HI_PVOID pCmdParam, HI_U16 usCmdParamSize, HI_U32 ulOption)
{
    DIAG_CMD_MRS_GET_CLT_SEARCH_STATS_IND_STRU* pstStats = mrsDfxCltGetSearchStats();
    MRS_STA_SEARCH_CTX *pstSmCtx = mrsStaSearchCtx();
    pstStats->ucTotalMeter = pstSmCtx->stMeterList.ucMeterNum;
    pstStats->ucSearchStatus = mrsStaSearchGetStatus();

    MRS_NOT_USED(pCmdParam);
    MRS_NOT_USED(usCmdParamSize);

    HI_MDM_ReportPacket(usId, (HI_U8)ulOption, (HI_PBYTE)pstStats, sizeof(DIAG_CMD_MRS_GET_CLT_SEARCH_STATS_IND_STRU), HI_TRUE);

    return HI_ERR_SUCCESS;
}


HI_U32 mrsDfxGetCltSearchLocal(HI_U16 usId, HI_IN HI_PVOID pCmdParam, HI_U16 usCmdParamSize, HI_U32 ulOption)
{
    DIAG_CMD_MRS_GET_CLT_SEARCH_LOCAL_IND_STRU* pstLocal = mrsDfxCltGetSearchLocal();
    pstLocal->ucSearchStatus = mrsStaSearchGetStatus();

    MRS_NOT_USED(pCmdParam);
    MRS_NOT_USED(usCmdParamSize);

    if (MRS_SEARCH_STATUS_IN_PROGRESS == pstLocal->ucSearchStatus)
    {
        pstLocal->ulEndTime = HI_MDM_GetMilliSeconds();
    }

    HI_MDM_ReportPacket(usId, (HI_U8)ulOption, (HI_PBYTE)pstLocal, sizeof(DIAG_CMD_MRS_GET_CLT_SEARCH_LOCAL_IND_STRU), HI_TRUE);

    return HI_ERR_SUCCESS;
}


HI_U32 mrsDfxGetCltSearchLocalEx(HI_U16 usId, HI_IN HI_PVOID pCmdParam, HI_U16 usCmdParamSize, HI_U32 ulOption)
{
    HI_UNREF_PARAM(pCmdParam);
    HI_UNREF_PARAM(usCmdParamSize);

    HI_MDM_ReportPacket(usId, (HI_U8)ulOption, (HI_PBYTE)(&g_stCltSearchLocalEx), sizeof(g_stCltSearchLocalEx), HI_TRUE);

    return HI_ERR_SUCCESS;
}


HI_U32 mrsDfxGetCltSearchFrame(HI_U16 usId, HI_IN HI_PVOID pCmdParam, HI_U16 usCmdParamSize, HI_U32 ulOption)
{
    DIAG_CMD_MRS_GET_CLT_SEARCH_FRAME_IND_STRU* pstPacket = mrsToolsMalloc(sizeof(DIAG_CMD_MRS_GET_CLT_SEARCH_FRAME_IND_STRU));
    MRS_DFX_SEARCH_FRAME_STATS_STRU* pstFrameStats = mrsDfxCltGetSearchFrame();
    MRS_DFX_SEARCH_FRAME_STRU* pstOneFrame = HI_NULL;
    MRS_DFX_SEARCH_FRAME_HEAD* pstOneHead = HI_NULL;
    HI_U8 ucReportTotal = MRS_MIN((HI_U8)pstFrameStats->usFrameTotal, MRS_DFX_SEARCH_FRAME_MAX);
    HI_U8 ucReported = 0;
    HI_U8 ucIndex = 0;

    if (HI_NULL == pstPacket)
    {
        return HI_ERR_NOT_ENOUGH_MEMORY;
    }

    MRS_NOT_USED(pCmdParam);
    MRS_NOT_USED(usCmdParamSize);

    if (0 == ucReportTotal)
    {
        (hi_void)memset_s(pstPacket, sizeof(DIAG_CMD_MRS_GET_CLT_SEARCH_FRAME_IND_STRU), 0, sizeof(DIAG_CMD_MRS_GET_CLT_SEARCH_FRAME_IND_STRU));
        HI_MDM_ReportPacket(usId, (HI_U8)ulOption, (HI_PBYTE)pstPacket, sizeof(DIAG_CMD_MRS_GET_CLT_SEARCH_FRAME_IND_STRU), HI_FALSE);

        mrsToolsFree(pstPacket);
        return HI_ERR_SUCCESS;
    }

    while (ucReported < ucReportTotal)
    {        
        (hi_void)memset_s(pstPacket, sizeof(DIAG_CMD_MRS_GET_CLT_SEARCH_FRAME_IND_STRU), 0, sizeof(DIAG_CMD_MRS_GET_CLT_SEARCH_FRAME_IND_STRU));
        pstPacket->ucTotal = (HI_U8)pstFrameStats->usFrameTotal;
        
        pstPacket->ucReportTotal = ucReportTotal;
        pstPacket->ucStartIndex = ucReported;
        pstPacket->ucCurrent = MRS_MIN(MRS_DFX_SEARCH_FRAME_CURRENT, ucReportTotal - ucReported);

        for (ucIndex = 0; ucIndex < pstPacket->ucCurrent; ucIndex++)
        {         
            pstOneFrame = &pstPacket->stFrame[ucIndex];
            pstOneHead = &pstFrameStats->stHead[ucReported + ucIndex];
            
            pstOneFrame->ulDI = pstOneHead->ulDI;
            (hi_void)memcpy_s(pstOneFrame->ucAddr, sizeof(pstOneFrame->ucAddr), pstOneHead->ucAddr, HI_METER_ADDR_LEN);
            pstOneFrame->ucBaudrate = pstOneHead->ucBaudrate;
            pstOneFrame->ucTimeoutCnt = (HI_U8)pstOneHead->ulByteIntervalTimeoutCnt;
            pstOneFrame->usFrameStats = pstOneHead->usFrameStats;
            pstOneFrame->usFrameHeadTS = (HI_U16)pstOneHead->ulFrameHeadTS;
            pstOneFrame->usFrameTailTS = (HI_U16)pstOneHead->ulFrameTailTS;
            pstOneFrame->ucLen = pstOneHead->ucFrameLen;
            (hi_void)memcpy_s(pstOneFrame->ucFrame, sizeof(pstOneFrame->ucFrame), (pstFrameStats->ucBuf + pstOneHead->usFrameOffset), 
                           MRS_MIN(pstOneFrame->ucLen, MRS_DFX_SEARCH_FRAME_MAX_LEN));
        }
        
        HI_MDM_ReportPacket(usId, (HI_U8)ulOption, (HI_PBYTE)pstPacket, sizeof(DIAG_CMD_MRS_GET_CLT_SEARCH_FRAME_IND_STRU), HI_FALSE);
        ucReported += pstPacket->ucCurrent;
    }

    mrsToolsFree(pstPacket);

    return HI_ERR_SUCCESS;
}


HI_U32 mrsDfxGetCltLocalMap(HI_U16 usId, HI_IN HI_PVOID pCmdParam, HI_U16 usCmdParamSize, HI_U32 ulOption)
{
    MRS_STA_SRV_CTX_STRU* pstCtx = mrsStaGetContext();
    DIAG_CMD_MRS_GET_CLT_LOCAL_MAP_IND_STRU* pstPacket = HI_NULL;
    MRS_DFX_CLT_LOCAL_MAP_ENTRY* pstDest = HI_NULL;
    MRS_METER_ITEM_S* pstSrc = HI_NULL;
    HI_U8 ucTotal = 0;
    HI_U8 ucReported = 0;
    HI_U8 ucIndex = 0;
    
    MRS_NOT_USED(pCmdParam);
    MRS_NOT_USED(usCmdParamSize);

    ucTotal = MRS_MIN(pstCtx->stMeterList.ucMeterNum, PRODUCT_CFG_MRS_MAX_METER_IN_MAC_NUM);

    pstPacket = mrsToolsMalloc(sizeof(DIAG_CMD_MRS_GET_CLT_LOCAL_MAP_IND_STRU));
    if (HI_NULL == pstPacket)
    {
        return HI_ERR_NOT_ENOUGH_MEMORY;
    }

    if (0 == ucTotal)
    {
        (hi_void)memset_s(pstPacket, sizeof(DIAG_CMD_MRS_GET_CLT_LOCAL_MAP_IND_STRU), 0, sizeof(DIAG_CMD_MRS_GET_CLT_LOCAL_MAP_IND_STRU));
        HI_MDM_ReportPacket(usId, (HI_U8)ulOption, (HI_PBYTE)pstPacket, sizeof(DIAG_CMD_MRS_GET_CLT_LOCAL_MAP_IND_STRU), HI_FALSE);

        mrsToolsFree(pstPacket);
        return HI_ERR_SUCCESS;
    }

    while (ucReported < ucTotal)
    {
        (hi_void)memset_s(pstPacket, sizeof(DIAG_CMD_MRS_GET_CLT_LOCAL_MAP_IND_STRU), 0, sizeof(DIAG_CMD_MRS_GET_CLT_LOCAL_MAP_IND_STRU));
        pstPacket->ucTotal = ucTotal;
        pstPacket->ucCurrent = MRS_MIN(ucTotal - ucReported, DIAG_CMD_MRS_CLT_LOCAL_MAP_NUM);
        pstPacket->ucStartIndex = ucReported;

        pstDest = pstPacket->astMapEntry;
        pstSrc = pstCtx->stMeterList.stMeterItem + ucReported;
        for (ucIndex = 0; ucIndex < pstPacket->ucCurrent; ++ucIndex)
        {
            (hi_void)memcpy_s(pstDest->aucMeterAddr, sizeof(pstDest->aucMeterAddr), pstSrc->ucMeterAddr, HI_METER_ADDR_LEN);
            pstDest->ucProto = pstSrc->ucProtocol;
        
            pstDest++;
            pstSrc++;
        }

        HI_MDM_ReportPacket(usId, (HI_U8)ulOption, (HI_PBYTE)pstPacket, sizeof(DIAG_CMD_MRS_GET_CLT_LOCAL_MAP_IND_STRU), HI_FALSE);
        ucReported += pstPacket->ucCurrent;
    }
    
    mrsToolsFree(pstPacket);

    return HI_ERR_SUCCESS;
}


HI_VOID mrsDfxStaSetAddr2Mac(HI_U8* pucMacAddr)
{
    DIAG_CMD_MRS_GET_CLT_SEARCH_STATS_IND_STRU* pstStats = mrsDfxCltGetSearchStats();
    DIAG_CMD_MRS_GET_CLT_SEARCH_LOCAL_IND_STRU* pstLocal = mrsDfxCltGetSearchLocal();
    
    (hi_void)memcpy_s(pstStats->ucComSetMacAddr, sizeof(pstStats->ucComSetMacAddr), pucMacAddr, HI_PLC_MAC_ADDR_LEN);
    (hi_void)memcpy_s(pstLocal->ucMeter2Mac, sizeof(pstLocal->ucMeter2Mac), pucMacAddr, HI_METER_ADDR_LEN);
    pstStats->ulComSetMacAddrLTS = HI_MDM_GetMilliSeconds();
    pstLocal->ulMeter2MacLTS = HI_MDM_GetMilliSeconds();
}

#if defined(PRODUCT_CFG_CLTI_SUPPORT_MRS_SWITCH)
HI_VOID mrsDfxCltiSmComQueryRx(HI_U8* pucData)
{
    DIAG_CMD_MRS_GET_CLT_SEARCH_STATS_IND_STRU* pstStats = mrsDfxCltGetSearchStats();

    pstStats->usComQueryRx++;
    pstStats->ulComQueryRxLTS = HI_MDM_GetMilliSeconds();
    pstStats->ucTotalMeter = pucData[MRS_CLTI_REPORT_POS_TOTAL];
}
#endif

HI_VOID mrsDfxCltSmComQueryResultLTS(HI_VOID)
{
    DIAG_CMD_MRS_GET_CLT_SEARCH_STATS_IND_STRU* pstStats = mrsDfxCltGetSearchStats();

    if (0 == pstStats->ulComQueryResultLTS)
    {
        pstStats->ulComQueryResultLTS = HI_MDM_GetMilliSeconds();
    }
}

#if defined(PRODUCT_CFG_CLTI_SUPPORT_MRS_SWITCH)
HI_VOID mrsDfxCltiSmComFrameTx(HI_U8 ucType)
{
    DIAG_CMD_MRS_GET_CLT_SEARCH_STATS_IND_STRU* pstStats = mrsDfxCltGetSearchStats();
    
    switch(ucType)
    {
        case MRS_CLTI_FRAME_TYPE_SM_QUERY_FIRST:
        case MRS_CLTI_FRAME_TYPE_SM_QUERY_FOLLOW:
            if (pstStats->ulComFirstQueryLTS == 0)
            {
                pstStats->ulComFirstQueryLTS = HI_MDM_GetMilliSeconds();
            }
            pstStats->usComQueryTx++;
            pstStats->ulComQueryTxLTS = HI_MDM_GetMilliSeconds();
            return;
            
        case MRS_CLTI_FRAME_TYPE_SM_START:
            pstStats->usComStartTx++;
            pstStats->ulComStartTxLTS = HI_MDM_GetMilliSeconds();
            return;
            
        default:
            return;
    }
}


HI_VOID mrsDfxCltiSmComStartRx(HI_VOID)
{
    DIAG_CMD_MRS_GET_CLT_SEARCH_STATS_IND_STRU* pstStats = mrsDfxCltGetSearchStats();
    
    pstStats->usComStartRx++;
    pstStats->ulComStartRxLTS = HI_MDM_GetMilliSeconds();
}
#endif

HI_VOID mrsDfxCltSmPlcQueryRx(HI_U32 ulSeq)
{
    DIAG_CMD_MRS_GET_CLT_SEARCH_STATS_IND_STRU* pstStats = mrsDfxCltGetSearchStats();

    if (pstStats->ulSearchSN != ulSeq)
    {
        mrsDfxCltResetPlcSearchStats();
    }

    pstStats->ulSearchSN = ulSeq;
    pstStats->usPlcQueryRx++;
    pstStats->ulPlcQueryRxLTS = HI_MDM_GetMilliSeconds(); 
}

HI_VOID mrsDfxCltSmPlcQueryTx(HI_VOID)
{
    DIAG_CMD_MRS_GET_CLT_SEARCH_STATS_IND_STRU* pstStats = mrsDfxCltGetSearchStats();
    
    pstStats->usPlcQueryTx++;
    pstStats->ulPlcQueryTxLTS = HI_MDM_GetMilliSeconds();
}


HI_VOID mrsDfxCltSmPlcStartRx(HI_U32 ulSeq)
{
    DIAG_CMD_MRS_GET_CLT_SEARCH_STATS_IND_STRU* pstStats = mrsDfxCltGetSearchStats();

    if (pstStats->ulSearchSN != ulSeq)
    {
        mrsDfxCltResetPlcSearchStats();
    }
    
    pstStats->ulSearchSN = ulSeq;
    pstStats->usPlcStartRx++;
    pstStats->usPlcStartRxLTS = HI_MDM_GetMilliSeconds();
}


HI_VOID mrsDfxCltSetBaudRate(HI_U16 usBaudRate)
{
    DIAG_CMD_MRS_GET_CLT_SEARCH_STATS_IND_STRU* pstStats = mrsDfxCltGetSearchStats();

    pstStats->usBaudrate = usBaudRate;
}


HI_VOID mrsDfxCltSmLocalStartTime(HI_VOID)
{
    DIAG_CMD_MRS_GET_CLT_SEARCH_LOCAL_IND_STRU* pstLocal = mrsDfxCltGetSearchLocal();

    pstLocal->ulStartTime = HI_MDM_GetMilliSeconds();
}


HI_VOID mrsDfxCltSmLocalEndTime(HI_VOID)
{
    DIAG_CMD_MRS_GET_CLT_SEARCH_LOCAL_IND_STRU* pstLocal = mrsDfxCltGetSearchLocal();

    pstLocal->ulEndTime = HI_MDM_GetMilliSeconds();
}


MRS_CLT_SEARCH_LOCAL_STATS* mrsDfxCltiiGetLocalStatsType(HI_VOID)
{
    DIAG_CMD_MRS_GET_CLT_SEARCH_LOCAL_IND_STRU* pstLocal = mrsDfxCltGetSearchLocal();
    MRS_STA_SEARCH_CTX *pstCtx = mrsStaSearchCtx();
    MRS_SEARCH_CFG_STRU *pstCfg = &(pstCtx->stSearchCfg);

    if (pstCtx->stRunning.bTryFisrtFlag)
    {
        return &pstLocal->stStartup;
    }
    
    switch(mrsCltiiSearchTxProtocol(pstCtx))
    {
        case METER_PROTO_698_45:
            return &(g_stCltSearchLocalEx.stProtocol698);

        case METER_PROTO_645_2007:
            return &pstLocal->stProtocol07;

        case METER_PROTO_645_1997:
            {
                MRS_CLT_SEARCH_LOCAL_STATS *pStats[3] = 
                {
                    &(pstLocal->stProtocol97a), 
                    &(pstLocal->stProtocol97b), 
                    &(g_stCltSearchLocalEx.stProtocol97c)
                };
                HI_U32 i = 0;
                HI_U16 usDi = (HI_U16)mrsCltiiSearchTxDI(pstCtx);

                for (i = 0; i < MRS_TOOLS_ALEN(pStats); i++)
                {
                    if (usDi == pstCfg->ausDI97[i])
                    {
                        return pStats[i];
                    }
                }

                return HI_NULL;
            }

        default:
            return HI_NULL;
    }
}


HI_VOID mrsDfxCltiiSetLocalTxStats(HI_U8* pucAddr, HI_U8 ucProtocol, HI_U32 ulDI, HI_U32 ulBaudrate)
{
    DIAG_CMD_MRS_GET_CLT_SEARCH_LOCAL_IND_STRU* pstLocal = mrsDfxCltGetSearchLocal();
    MRS_CLT_SEARCH_LOCAL_STATS *pstStatsType = mrsDfxCltiiGetLocalStatsType();
    MRS_STA_SEARCH_CTX *pstCtx = mrsStaSearchCtx();

    pstCtx->stRunning.unClt.stCltII.ulTxTimeStamp = HI_MDM_GetMilliSeconds();

    pstLocal->ucProtocol = ucProtocol;
    pstLocal->usBaudrate = (HI_U16)ulBaudrate;
    pstLocal->ulSearchDI = ulDI;
    (hi_void)memcpy_s(pstLocal->ucSearchAddr, sizeof(pstLocal->ucSearchAddr), pucAddr, HI_METER_ADDR_LEN);
    
    if (pstStatsType)
    {
        pstStatsType->usSearch++;
        mrsDfxCltiiSetLocalTxAddrDepth(pstStatsType, pstLocal->ucSearchAddr, ucProtocol);
    }    
}


HI_VOID mrsDfxCltiiSetLocalTxAddrDepth(MRS_CLT_SEARCH_LOCAL_STATS* pstStatsType, HI_U8* pucAddr, HI_U8 ucProtocol)
{
    HI_U8 ucDepth = 0;

    switch(mrsCltiiSearchGetState())
    {
    case MRS_CLTII_SEARCH_STATE_LOOP:
    case MRS_CLTII_SEARCH_STATE_EXPLORE:
        break;
        
    default:
        return;        
    }

    // 根据当前通配符地址缩的深度判断是否有搜表异常，广播地址缩0    
    ucDepth = (mrsToolsBroadCastAddr(pucAddr) ? 0 : mrsCltiiSearchGetAddrDepth(pucAddr, ucProtocol));
    
    if (ucDepth > pstStatsType->ucMaxDepth)
    {
        pstStatsType->ucMaxDepth = ucDepth;

        if (ucDepth > MRS_DFX_SM_ABNORMAL_DEPTH)
        {
            mrsDfxCltiiSetSmAlarm(&pstStatsType->ucAbnormal);
        }
        else if (ucDepth == MRS_DFX_SM_ABNORMAL_DEPTH)
        {
            mrsDfxCltiiSetSmWarning(&pstStatsType->ucAbnormal);
        }
    }
}


HI_VOID mrsDfxCltiiSetLocalRxValid645Frame(HI_VOID)
{
    MRS_CLT_SEARCH_LOCAL_STATS *pstStatsType = mrsDfxCltiiGetLocalStatsType();

    if (pstStatsType)
    {
        pstStatsType->usValid++;
    }    
}


HI_VOID mrsDfxCltiiSetLocalRxInvalidStats(HI_U16 usLen)
{
    MRS_CLT_SEARCH_LOCAL_STATS *pstStatsType = mrsDfxCltiiGetLocalStatsType();
    
    if (pstStatsType)
    {
        switch (usLen)
        {
            case 0:
                break;

            case 1:
                pstStatsType->usInvalidLen1++;
                break;

            case 2:
                pstStatsType->usInvalidLen2++;
                break;

            case 3:
            default:
                pstStatsType->usInvalidLen3++;
                break;
        }
    }
}


MRS_DFX_SEARCH_FRAME_HEAD* mrsDfxCltiiGetCurrentFrameHead(HI_VOID)
{
    MRS_DFX_SEARCH_FRAME_STATS_STRU* pstFrameStats = mrsDfxCltGetSearchFrame();

    if (pstFrameStats->usFrameTotal >= MRS_DFX_SEARCH_FRAME_MAX)
    {
        return HI_NULL;
    }

    return &pstFrameStats->stHead[pstFrameStats->usFrameTotal];
}


HI_VOID mrsDfxCltiiAppendCurrentFrame(HI_U8* pucAddr, HI_U8 ucProtocol, HI_U32 ulDI, HI_U16 usBaudrate)
{
    MRS_DFX_SEARCH_FRAME_STATS_STRU* pstFrameStats = mrsDfxCltGetSearchFrame();
    MRS_DFX_SEARCH_FRAME_HEAD* pstCurrentHead = mrsDfxCltiiGetCurrentFrameHead();
    MRS_STA_SEARCH_CTX* pstSmCtx = mrsStaSearchCtx();

    HI_UNREF_PARAM(ucProtocol);
    
    if (HI_NULL == pstCurrentHead)
    {
        //已满
        return;
    }

    pstCurrentHead->ulDI = ulDI;
    (hi_void)memcpy_s(pstCurrentHead->ucAddr, sizeof(pstCurrentHead->ucAddr), pucAddr, HI_METER_ADDR_LEN);
    pstCurrentHead->ucBaudrate = (HI_U8)(usBaudrate / MRS_DFX_BAUDRATE_100);
    pstCurrentHead->ucFrameLen = (HI_U8)pstSmCtx->stRx.ulSize;
    pstCurrentHead->usFrameOffset = pstFrameStats->usBufTail;
    // 这里不增加pstCurrentHead->ucFrameTotal，因为调用该函数后还要判定这帧的usFrameStats/ulFrameHeadTS等

    if (pstFrameStats->usBufTail + pstSmCtx->stRx.ulSize >= MRS_DFX_SEARCH_BUF_SIZE)
    {
        return;
    }
    
    (hi_void)memcpy_s(pstFrameStats->ucBuf + pstFrameStats->usBufTail,
        sizeof(pstFrameStats->ucBuf) - pstFrameStats->usBufTail, pstSmCtx->stRx.aucBuf, pstCurrentHead->ucFrameLen);
    pstFrameStats->usBufTail += pstCurrentHead->ucFrameLen;
}


// 调用顺序敏感，影响mrsDfxCltiiGetCurrentFrameHead的返回值
HI_VOID mrsDfxCltiiRxFrameCount(HI_VOID)
{
    MRS_DFX_SEARCH_FRAME_STATS_STRU* pstFrameStats = mrsDfxCltGetSearchFrame();

    pstFrameStats->usFrameTotal++;
}


HI_VOID mrsDfxCltiiSetFrameTimeStats(HI_VOID)
{
    MRS_DFX_SEARCH_FRAME_HEAD* pstCurrentHead = mrsDfxCltiiGetCurrentFrameHead();
    MRS_STA_SEARCH_CTX* pstSmCtx = mrsStaSearchCtx();
    MRS_SEARCH_RUNNING_CLT_II *pstRunning = &(pstSmCtx->stRunning.unClt.stCltII);
    HI_U32 ulTxTimeStamp = pstRunning->ulTxTimeStamp;
    HI_U32 ulRxTimeStamp = HI_MDM_GetMilliSeconds();
        
    if (HI_NULL == pstCurrentHead)
    {
        return;
    }

    if (0 == pstCurrentHead->ulFrameHeadTS)
    {
        pstCurrentHead->ulFrameHeadTS = ulRxTimeStamp - ulTxTimeStamp;
        pstRunning->ulRxTimeStamp = ulRxTimeStamp;
    }
    
    pstCurrentHead->ulFrameTailTS = ulRxTimeStamp - ulTxTimeStamp;

    if (ulRxTimeStamp - pstRunning->ulRxTimeStamp > MRS_DFX_BYTE_INTERVAL_TIMEOUT)
    {
        pstCurrentHead->ulByteIntervalTimeoutCnt++;
    }
    
    pstRunning->ulRxTimeStamp = ulRxTimeStamp;
}


// 帧统计: 该U16的每4位表示一类帧的数量 [0:3]合法 [4:7]乱码 [8:11]否认 [12:15]丢弃
HI_VOID mrsDfxCltiiSetFrameStatsValid(HI_VOID)
{
    MRS_DFX_SEARCH_FRAME_HEAD* pstCurrentHead = mrsDfxCltiiGetCurrentFrameHead();
    HI_U16 usFrameStats = 0;
    HI_U8 ucValidCnt = 0;
    
    if (HI_NULL == pstCurrentHead)
    {
        return;
    }
    
    usFrameStats = pstCurrentHead->usFrameStats;
    ucValidCnt = (HI_U8)(usFrameStats & 0x000F);

    ucValidCnt++;
    usFrameStats = ((usFrameStats & 0xFFF0) | (ucValidCnt & 0x000F));
    pstCurrentHead->usFrameStats = usFrameStats;
}


HI_VOID mrsDfxCltiiSetFrameStatsInvalid(HI_VOID)
{
    MRS_DFX_SEARCH_FRAME_HEAD* pstCurrentHead = mrsDfxCltiiGetCurrentFrameHead();
    HI_U16 usFrameStats = 0;
    HI_U8 ucInvalidCnt = 0;

    if (HI_NULL == pstCurrentHead)
    {
        return;
    }
    
    usFrameStats = pstCurrentHead->usFrameStats;
    ucInvalidCnt = (HI_U8)((usFrameStats & 0x00F0) >> 4);

    ucInvalidCnt++;
    usFrameStats = ((usFrameStats & 0xFF0F) | ((ucInvalidCnt & 0x000F) << 4));
    pstCurrentHead->usFrameStats = usFrameStats;
}


HI_VOID mrsDfxCltiiSetFrameStatsDeny(HI_VOID)
{
    MRS_DFX_SEARCH_FRAME_HEAD* pstCurrentHead = mrsDfxCltiiGetCurrentFrameHead();
    HI_U16 usFrameStats = 0;
    HI_U8 ucDenyCnt = 0;
    
    if (HI_NULL == pstCurrentHead)
    {
        return;
    }
    
    usFrameStats = pstCurrentHead->usFrameStats;
    ucDenyCnt = (HI_U8)((usFrameStats & 0x0F00) >> 8);

    ucDenyCnt++;
    usFrameStats = ((usFrameStats & 0xF0FF) | ((ucDenyCnt & 0x000F) << 8));
    pstCurrentHead->usFrameStats = usFrameStats;
}


HI_VOID mrsDfxCltiiSetFrameStatsDiscard(HI_VOID)
{
    MRS_DFX_SEARCH_FRAME_HEAD* pstCurrentHead = mrsDfxCltiiGetCurrentFrameHead();
    HI_U16 usFrameStats = 0;
    HI_U8 ucDiscardCnt = 0;
    
    if (HI_NULL == pstCurrentHead)
    {
        return;
    }
    
    usFrameStats = pstCurrentHead->usFrameStats;
    ucDiscardCnt = (HI_U8)((usFrameStats & 0xF000) >> 12);

    ucDiscardCnt++;
    usFrameStats = ((usFrameStats & 0x0FFF) | ((ucDiscardCnt & 0x000F) << 12));
    pstCurrentHead->usFrameStats = usFrameStats;
}

// 异常类型(置位)：0:正常 1:告警 2:预警 4:多II采连接
HI_VOID mrsDfxCltiiSetSmAlarm(HI_U8* pucAbnormal)
{
    *pucAbnormal &= 0xFD;
    *pucAbnormal |= 0x01;
}


HI_VOID mrsDfxCltiiSetSmWarning(HI_U8* pucAbnormal)
{
    *pucAbnormal |= 0x02;
}


HI_VOID mrsDfxCltiiSetSmMultiCltiiConnect(HI_VOID)
{
    MRS_CLT_SEARCH_LOCAL_STATS *pstStatsType = mrsDfxCltiiGetLocalStatsType();

    if (pstStatsType)
    {
        pstStatsType->ucAbnormal |= 0x04;
    }
}

/* BEGIN: Modified by cuichunyan/c00276962 2016/11/10  问题单号:cltii R485 MultiConnect */    
HI_U32 mrsDfxGetRS485MultiConn(HI_U16 usId, HI_IN HI_PVOID pCmdParam, HI_U16 usCmdParamSize, HI_U32 ulOption)
{
    DIAG_CMD_MRS_GET_RS485_MULTI_CONNECT_REQ_STRU * pstReq = (DIAG_CMD_MRS_GET_RS485_MULTI_CONNECT_REQ_STRU * )pCmdParam;
    DIAG_CMD_MRS_GET_RS485_MULTI_CONNECT_IND_STRU *pstInd = HI_NULL;
    
    MRS_CLTII_R485_MULTI_CONN_STRU * pstMultiInfo = &(mrsGetCollectorCtx()->stMultiConn);
    HI_U8 ucPayLen = 0;
    HI_U8 ucStructSizeInd = sizeof(DIAG_CMD_MRS_GET_RS485_MULTI_CONNECT_IND_STRU);

    if (pstReq->ucStructSize != usCmdParamSize)//版本号判断
    {
        return HI_ERR_FAILURE;
    }

    ucPayLen = (HI_U8)(pstMultiInfo->ucRecordTimes * sizeof(MRS_RS485_MULTI_CONNECT_ITEM));        

    pstInd = (DIAG_CMD_MRS_GET_RS485_MULTI_CONNECT_IND_STRU *)mrsToolsMalloc(ucPayLen + ucStructSizeInd);
    if (pstInd == HI_NULL)
    {
        return HI_ERR_MALLOC_FAILUE;
    }

    (hi_void)memset_s(pstInd, ucStructSizeInd + ucPayLen, 0, ucStructSizeInd + ucPayLen);

    pstInd->ucVer = MRS_CMD_STRU_VER_R1;
    pstInd->ucStructSize = ucStructSizeInd;
    pstInd->ucRecordTimes = pstMultiInfo->ucRecordTimes;
    pstInd->ulTotalTimes = pstMultiInfo->ulTotalTimes;
    pstInd->ulUserQryTime = HI_MDM_GetSeconds();
    if (pstInd->ucRecordTimes > 0)
    {
        (hi_void)memcpy_s((HI_PBYTE)pstInd->astRecord, ucPayLen,(HI_PBYTE)pstMultiInfo->stRecord, ucPayLen);
    }

    HI_MDM_ReportPacket(usId, (HI_U8)ulOption, (HI_PBYTE)pstInd, pstInd->ucStructSize + ucPayLen, HI_TRUE);

    mrsToolsFree(pstInd);
    return HI_ERR_SUCCESS;
}

HI_U32 mrsDfxClrRS485MultiConn(HI_U16 usId, HI_IN HI_PVOID pCmdParam, HI_U16 usCmdParamSize, HI_U32 ulOption)
{
    DIAG_CMD_MRS_ClEAR_RS485_MULTI_CONNECT_REQ_STRU * pstReq = (DIAG_CMD_MRS_ClEAR_RS485_MULTI_CONNECT_REQ_STRU *)pCmdParam;
    DIAG_CMD_MRS_ClEAR_RS485_MULTI_CONNECT_IND_STRU stInd = {0};

    if (pstReq->ucStructSize != usCmdParamSize)     
    {
        return HI_ERR_FAILURE;
    }

    mrsInitMultiConnCfg();
    stInd.ucVer = MRS_CMD_STRU_VER_R1;
    stInd.ulErrorCode = HI_ERR_SUCCESS;
    stInd.ucStructSize = sizeof(DIAG_CMD_MRS_ClEAR_RS485_MULTI_CONNECT_IND_STRU);
    HI_MDM_ReportPacket(usId, (HI_U8)ulOption, (HI_PBYTE)&stInd, stInd.ucStructSize, HI_TRUE);

    return HI_ERR_SUCCESS;
}

/* END  : Modified by cuichunyan/c00276962 2016/11/10  问题单号:cltii R485 MultiConnect */

#endif  // PRODUCT_CFG_PRODUCT_TYPE_STA

