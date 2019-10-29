//*****************************************************************************
//
//                  版权所有 (C), 1998-2013, 华为技术有限公司
//
//*****************************************************************************
//  文 件 名   : mrs_srv_sta_searchmeter.c
//  版 本 号   : V1.0
//  作    者   : cuiate/00233580
//  生成日期   : 2013-08-05
//  功能描述   : 采集器/STA搜表函数及接口定义
//
//  函数列表   : TODO: ...
//  修改历史   :
//  1.日    期 : 2013-08-05
//    作    者 : cuiate/00233580
//    修改内容 : 创建文件
//
//*****************************************************************************

#include "mrs_common.h"
#include "mrs_cmd_msg.h"
#include "mrs_srv_common.h"
#include "mrs_fw_tools.h"
#include "mrs_fw_log.h"
#include "mrs_fw_nv.h"
#include "app_nv.h"
#include "mrs_srv_sta_queue.h"
#include "mrs_srv_parallel_sta.h"
#include "mrs_srv_sta_event.h"
#include "mrs_srv_sta.h"
#include "mrs_srv_sta_searchmeter.h"
#include "mrs_srv_collector.h"
#include "hi_mdm.h"
#include "mrs_srv_cltii_searchmeter.h"
#include "mrs_srv_clti_searchmeter.h"
#include "mrs_srv_baudrate_manage.h"
#include "mrs_dfx_clt.h"
#include "equip_dut_proc.h"


#if defined(PRODUCT_CFG_PRODUCT_TYPE_STA)

//*****************************************************************************
//*****************************************************************************

MRS_STA_SEARCH_CTX * g_pstStaSearchCtx = HI_NULL;

HI_PRV HI_U8 mrsStaCalcSearchResultParam(HI_U8 ucProtocol, HI_U16 usBaudRate, HI_U8 ucTry);
HI_PRV HI_VOID mrsStaUpdateFirstMeterBaudRate(HI_U8 *pucMeter, HI_U16 usBaudRate);


//*****************************************************************************
//*****************************************************************************

MRS_STA_SEARCH_CTX * mrsStaSearchCtx(HI_VOID)
{
    return g_pstStaSearchCtx;
}


HI_U32 mrsStaSearchInit(HI_VOID)
{
    HI_U32 ulRet = HI_ERR_SUCCESS;

    if (g_pstStaSearchCtx)
    {
        return HI_ERR_SUCCESS;
    }

    g_pstStaSearchCtx = (MRS_STA_SEARCH_CTX *)mrsToolsMalloc(sizeof(MRS_STA_SEARCH_CTX));
    if (!g_pstStaSearchCtx)
    {
        return HI_ERR_NOT_ENOUGH_MEMORY;
    }

    (hi_void)memset_s(g_pstStaSearchCtx, sizeof(MRS_STA_SEARCH_CTX), 0, sizeof(MRS_STA_SEARCH_CTX));

    mrsStaSearchCfgInit();
    mrsStaSearchFtmNvInit();

    if (mrsToolsIsCLTI() || mrsToolsIsIICollector())
    {
        mrsStaStartPeriodSearch(g_pstStaSearchCtx);
    }

    MRS_StartTimer(MRS_STA_TIMER_NETWORK_LOCK_CHECK, MRS_STA_TIME_NETWORK_LOCK_CHECK, HI_SYS_TIMER_PERIODIC);

    return ulRet;
}


HI_U32 mrsStaSearchCfgInit(HI_VOID)
{
    mrsNvStaSmCfgExp(ID_NV_APP_STA_SM_CFG_EXP);
    return mrsNvStaSmCfg(ID_NV_APP_SEARCHMETER_CFG);
}


HI_BOOL mrsStaAllowSearch(hi_void)
{
    return (HI_BOOL)(MRS_SEARCH_STATUS_IN_PROGRESS != mrsStaSearchGetStatus());
}


HI_VOID mrsStaStartPeriodSearch(MRS_STA_SEARCH_CTX *pstCtx)
{
    if (pstCtx->stSearchCfg.usSearchPeriod > 0)
    {
        MRS_StartTimer(MRS_STA_TIMER_SM_PERIOD, pstCtx->stSearchCfg.usSearchPeriod * 60 * 1000, HI_SYS_TIMER_PERIODIC);
    }
}


HI_VOID mrsStaSearchDfxReset(HI_VOID)
{
    mrsDfxCltResetComSearchStats();
    mrsDfxCltResetSearchLocal();
    mrsDfxCltResetSearchFrame();
}


HI_U32 mrsStaSearchStart(HI_BOOL bTryFirstFlag)
{
    MRS_STA_SEARCH_CTX *pstCtx = mrsStaSearchCtx();

    if (!pstCtx)
    {
        return HI_ERR_FAILURE;
    }

    if (!mrsStaAllowSearch())
    {
        pstCtx->stRunning.unClt.stCltII.stDetect.bNeedResume = HI_TRUE;
        return HI_ERR_BUSY;
    }

    mrsStaSearchDfxReset();

    if (mrsToolsIsIICollector())
    {
        return mrsCltiiSearchStart(pstCtx, bTryFirstFlag);
    }

    return mrsCltiSearchStart(pstCtx, bTryFirstFlag);
}


HI_U32 mrsStaSearchStop(HI_VOID)
{
    MRS_STA_SEARCH_CTX *pstCtx = mrsStaSearchCtx();

    if (!pstCtx)
    {
        return HI_ERR_FAILURE;
    }

    mrsStaSearchSetEndReason(MRS_SEARCH_END_REASON_STOP);

    if (mrsToolsIsIICollector())
    {
        return mrsCltiiSearchStop(pstCtx);
    }

    return mrsCltiSearchStop(pstCtx);
}


HI_U32 mrsStaSearchClearMeterList(MRS_STA_SEARCH_CTX *pstCtx)
{
    (hi_void)memset_s(&(pstCtx->stMeterList), sizeof(pstCtx->stMeterList), 0, sizeof(pstCtx->stMeterList));
    return HI_ERR_SUCCESS;
}


// 将NV中的合法节点加载到上下文的NV列表中
HI_U32 mrsStaSearchLoadNvResult(MRS_SEARCH_NV_METERLIST_STRU* pstNvList)
{
    HI_U16 ausNvId[] = {ID_NV_APP_SM_RESULT1, ID_NV_APP_SM_RESULT2};
    HI_U32 ulRet = HI_ERR_SUCCESS;
    HI_U8 ucIndex = 0;

    (hi_void)memset_s(pstNvList, sizeof(MRS_SEARCH_NV_METERLIST_STRU), 0, sizeof(MRS_SEARCH_NV_METERLIST_STRU));

    for (ucIndex = 0; ucIndex < sizeof(ausNvId) / sizeof(ausNvId[0]); ucIndex++)
    {
        ulRet |= mrsStaSearchGetValidNvNodes(ausNvId[ucIndex], pstNvList);
    }

    return ulRet;
}


HI_U32 mrsStaSearchGetValidNvNodes(HI_U16 usId, MRS_SEARCH_NV_METERLIST_STRU* pstNvList)
{
    NV_APP_MRS_SM_RESULT* pstNv = HI_NULL;
    NV_APP_MRS_SM_NODE* pstNode = HI_NULL;
    HI_U32 ulRet = HI_ERR_SUCCESS;
    HI_U8 ucIndex = 0;

    pstNv = mrsToolsMalloc(sizeof(NV_APP_MRS_SM_RESULT));
    if (HI_NULL == pstNv)
    {
        return HI_ERR_NOT_ENOUGH_MEMORY;
    }

    (hi_void)memset_s(pstNv, sizeof(NV_APP_MRS_SM_RESULT), 0, sizeof(NV_APP_MRS_SM_RESULT));

    ulRet = HI_MDM_NV_Read(usId, pstNv, (HI_U16)sizeof(NV_APP_MRS_SM_RESULT));
    if (HI_ERR_SUCCESS != ulRet)
    {
        mrsToolsFree(pstNv);
        return HI_ERR_FAILURE;
    }

    for (ucIndex = 0; ucIndex < NV_SM_RESULT_METER_NUM; ucIndex++)
    {
        pstNode = &(pstNv->astMeter[ucIndex]);
        if (mrsStaSearchIsNvNodeValid(pstNode))
        {
            HI_U8 ucProtocol = mrsStaSearchGetNodeProtocol(pstNode);
            NV_APP_MRS_SM_NODE stMeterNode;

            // DI合法性判断
            (hi_void)memcpy_s(&stMeterNode, sizeof(stMeterNode), pstNode, sizeof(stMeterNode));
            if (MRS_PROTO645_VER_2007 == ucProtocol)
            {
                if (0 != stMeterNode.ucOption)
                {
                    continue;
                }
            }
            else
            {
                if (!mrsCltiiSearchIsNvNodeDI97Valid(stMeterNode.ucOption))
                {
                    // 97表DI不是NV配置的搜表DI值，修改为默认DI值
                    stMeterNode.ucOption = (HI_U8)(mrsStaGetDefaultDi97());
                    if (0 == stMeterNode.ucOption)
                    {
                        continue;
                    }
                }
            }

            mrsStaSearchNvNodeInsert(pstNvList, &stMeterNode);
        }
    }

    mrsToolsFree(pstNv);
    return HI_ERR_SUCCESS;
}


/* BEGIN: PN: DTS2015042907633 MODIFY\ADD\DEL by cuiate/00233580 at 2015/5/2 */
// 表地址、规约类型、老化值判断，DI合法性先不判断
HI_BOOL mrsStaSearchIsNvNodeValid(NV_APP_MRS_SM_NODE* pstNode)
{
    HI_U8 ucProtocol = mrsStaSearchGetNodeProtocol(pstNode);
    HI_U8 ucNotFoundTimes = mrsStaSearchGetNotFoundTimes(pstNode);

    HI_U8 aucMeterAddr[HI_METER_ADDR_LEN] = {0};

    if (ucNotFoundTimes > MRS_MAX_VALID_NOT_FOUND_TIMES)
    {
        return HI_FALSE;
    }

    mrsMeterAddrPreProc(aucMeterAddr, pstNode->aucAddr, ucProtocol);
    if (!mrsToolsNormalAddr(aucMeterAddr))
    {
        return HI_FALSE;
    }

    switch(ucProtocol)
    {
    case MRS_PROTO645_VER_1997:
    case MRS_PROTO645_VER_2007:
        return HI_TRUE;

    default:
        return HI_FALSE;
    }
}
/* END:   PN: DTS2015042907633 MODIFY\ADD\DEL by cuiate/00233580 at 2015/5/2 */


HI_U8 mrsStaSearchGetNodeProtocol(NV_APP_MRS_SM_NODE* pstNode)
{
    MRS_NV_SEARCH_PARAM_S stParam = {0};
    (hi_void)memcpy_s(&stParam, sizeof(MRS_NV_SEARCH_PARAM_S), &pstNode->ucParam, sizeof(HI_U8));
    return stParam.ucProtocol;
}


HI_U8 mrsStaSearchGetNotFoundTimes(NV_APP_MRS_SM_NODE* pstNode)
{
    MRS_NV_SEARCH_PARAM_S stParam = {0};
    (hi_void)memcpy_s(&stParam, sizeof(MRS_NV_SEARCH_PARAM_S), &pstNode->ucParam, sizeof(HI_U8));
    return stParam.ucTimes;
}


// 将任意合法节点加入NV列表，NV列表按未搜到次数递增次序排列
HI_U32 mrsStaSearchNvNodeInsert(MRS_SEARCH_NV_METERLIST_STRU* pstNvList, NV_APP_MRS_SM_NODE* pstNode)
{
    NV_APP_MRS_SM_NODE* pstTempNode = HI_NULL;
    HI_U8 ucNotFoundTimes = mrsStaSearchGetNotFoundTimes(pstNode);
    HI_U8 ucMoveIndex = 0;
    HI_U8 ucIndex = 0;

    HI_U8 aucMeterAddr[HI_METER_ADDR_LEN] = {0};
    HI_U8 aucTempAddr[HI_METER_ADDR_LEN] = {0};

    mrsMeterAddrPreProc(aucMeterAddr, pstNode->aucAddr, mrsStaSearchGetNodeProtocol(pstNode));
    for (ucIndex = 0; ucIndex < pstNvList->ucTotal; ucIndex++)
    {
        pstTempNode = &(pstNvList->astNvMeter[ucIndex]);

        // 先检查该表地址是否已在列表中
        mrsMeterAddrPreProc(aucTempAddr, pstTempNode->aucAddr, mrsStaSearchGetNodeProtocol(pstNode));
        if (mrsToolsMemEq(aucMeterAddr, aucTempAddr, HI_METER_ADDR_LEN))
        {
            // 若未搜到的次数比列表中该地址的次数大，不再插入
            if (ucNotFoundTimes >= mrsStaSearchGetNotFoundTimes(pstTempNode))
            {
                return HI_ERR_FAILURE;
            }

            // 可以插入，先删除列表中该表地址节点
            for (ucMoveIndex = ucIndex; ucMoveIndex < pstNvList->ucTotal - 1; ucMoveIndex++)
            {
                (hi_void)memcpy_s(&pstNvList->astNvMeter[ucMoveIndex], sizeof(NV_APP_MRS_SM_NODE), &pstNvList->astNvMeter[ucMoveIndex + 1], sizeof(NV_APP_MRS_SM_NODE));
            }

            (hi_void)memset_s(&pstNvList->astNvMeter[pstNvList->ucTotal - 1], sizeof(NV_APP_MRS_SM_NODE), 0, sizeof(NV_APP_MRS_SM_NODE));
            pstNvList->ucTotal--;
            break;
        }
    }

    // 根据未搜到次数找到该次数的末尾位置，腾出位置，插入节点
    for (ucIndex = 0; ucIndex < pstNvList->ucTotal; ucIndex++)
    {
        pstTempNode = &(pstNvList->astNvMeter[ucIndex]);
        if (ucNotFoundTimes < mrsStaSearchGetNotFoundTimes(pstTempNode))
        {
            if (pstNvList->ucTotal == MRS_MAX_METERNUM)
            {
                ucMoveIndex = pstNvList->ucTotal - 1;
            }
            else
            {
                ucMoveIndex = pstNvList->ucTotal;
                pstNvList->ucTotal++;
            }

            while(ucMoveIndex > ucIndex)
            {
                (hi_void)memcpy_s(&pstNvList->astNvMeter[ucMoveIndex], sizeof(NV_APP_MRS_SM_NODE), &pstNvList->astNvMeter[ucMoveIndex - 1], sizeof(NV_APP_MRS_SM_NODE));
                ucMoveIndex--;
            }

            (hi_void)memcpy_s(&pstNvList->astNvMeter[ucIndex], sizeof(NV_APP_MRS_SM_NODE), pstNode, sizeof(NV_APP_MRS_SM_NODE));
            return HI_ERR_SUCCESS;
        }
    }

    // 未搜到次数等于最大次数或更大，直接放在列表末尾
    return mrsStaSearchAppendNvNode(pstNvList, pstNode);
}


// 直接将合法节点追加在列表末尾，列表满则不加
HI_U32 mrsStaSearchAppendNvNode(MRS_SEARCH_NV_METERLIST_STRU* pstNvList, NV_APP_MRS_SM_NODE* pstNode)
{
    if (pstNvList->ucTotal >= MRS_MAX_METERNUM)
    {
        return HI_ERR_FULL;
    }

    (hi_void)memcpy_s(&(pstNvList->astNvMeter[pstNvList->ucTotal]), sizeof(NV_APP_MRS_SM_NODE), pstNode, sizeof(NV_APP_MRS_SM_NODE));
    pstNvList->ucTotal++;

    return HI_ERR_SUCCESS;
}


// 更新存储搜表结果信息列表的NV项内容
HI_U32 mrsStaRefreshSearchResultNv(MRS_STA_SEARCH_CTX *pstCtx, HI_BOOL bSearchFinish)
{
    MRS_SEARCH_NV_METERLIST_STRU* pstNvList = &(pstCtx->stNvMeterList);
    MRS_SEARCH_METERLIST_STRU* pstSmList = &(pstCtx->stMeterList);
    MRS_SEARCH_NV_METERLIST_STRU* pstTempNvList = HI_NULL;
    NV_APP_MRS_SM_NODE* pstNode = HI_NULL;
    HI_U32 ulRet = HI_ERR_SUCCESS;
    HI_U8 ucIndex = 0;

    if (0 == pstNvList->ucTotal)
    {
        mrsStaSearchLoadNvResult(pstNvList);
    }

    pstTempNvList = mrsToolsMalloc(sizeof(MRS_SEARCH_NV_METERLIST_STRU));
    if (HI_NULL == pstTempNvList)
    {
        return HI_ERR_NOT_ENOUGH_MEMORY;
    }

    // 先将当前搜表结果加入临时列表
    (hi_void)memset_s(pstTempNvList, sizeof(MRS_SEARCH_NV_METERLIST_STRU), 0, sizeof(MRS_SEARCH_NV_METERLIST_STRU));
    mrsStaSearchGetResult(pstTempNvList, pstSmList);

    // 再加入上下文NV列表原有的点
    if (bSearchFinish)
    {
        // 搜表正常结束，只加入原未搜到次数小于2的点，次数加1，插入临时列表
        // 原未搜到次数为2的点不再加入，被老化掉(也有可能已经在搜表结果中)
        for (ucIndex = 0; ucIndex < pstNvList->ucTotal; ucIndex++)
        {
            pstNode = &(pstNvList->astNvMeter[ucIndex]);
            if (mrsStaSearchGetNotFoundTimes(pstNode) < MRS_MAX_VALID_NOT_FOUND_TIMES)
            {
                mrsStaSearchNotFoundTimesIncrease(pstNode);
                mrsStaSearchNvNodeInsert(pstTempNvList, pstNode);
            }
        }
    }
    else
    {
        // 不是搜表正常结束，是抄回新表更新NV或搜表中途停止，未搜到次数不变
        for (ucIndex = 0; ucIndex < pstNvList->ucTotal; ucIndex++)
        {
            mrsStaSearchNvNodeInsert(pstTempNvList, &pstNvList->astNvMeter[ucIndex]);
        }
    }

    // 将临时列表写入NV
    ulRet = mrsStaSearchWriteToNv(pstTempNvList);
    mrsToolsFree(pstTempNvList);

    return ulRet;
}


HI_U8 mrsStaCalcSearchResultParam(HI_U8 ucProtocol, HI_U16 usBaudRate, HI_U8 ucTry)
{
    MRS_NV_SEARCH_PARAM_S stParam;
    HI_U8 ucIdx = MRS_SRV_BAUD_RATE_IDX_DEFAULT;
    HI_U8 ucParam = 0;

    if (HI_ERR_SUCCESS != mrsSrvBaudRate2Index(usBaudRate, &ucIdx))
    {
        ucIdx = MRS_SRV_BAUD_RATE_IDX_DEFAULT;
    }

    stParam.ucProtocol = ucProtocol & 0x3;
    stParam.ucTimes = ucTry & 0x7;
    stParam.ucBrIdxLo = ucIdx & 0x3;
    stParam.ucBrIdxHi = (ucIdx >> 2) & 0x1;
    (hi_void)memcpy_s(&ucParam, sizeof(ucParam), &stParam, sizeof(ucParam));

    return ucParam;
}


// 将搜表结果加入NV列表
HI_VOID mrsStaSearchGetResult(MRS_SEARCH_NV_METERLIST_STRU* pstNvList, MRS_SEARCH_METERLIST_STRU *pstSmList)
{
    NV_APP_MRS_SM_NODE* pstNode = HI_NULL;
    MRS_METER_ITEM_S* pstItem = HI_NULL;
    HI_U8 ucIndex = 0;

    for (ucIndex = 0; ucIndex < pstSmList->ucMeterNum; ucIndex++)
    {
        pstNode = &pstNvList->astNvMeter[ucIndex];
        pstItem = &pstSmList->astMeter[ucIndex];

        (hi_void)memcpy_s(pstNode->aucAddr, HI_METER_ADDR_LEN, pstItem->ucMeterAddr, HI_METER_ADDR_LEN);
        pstNode->ucParam = mrsStaCalcSearchResultParam(pstItem->ucProtocol, pstItem->usBaudRate, 0);
        pstNode->ucOption = (HI_U8)pstItem->usOption;
    }

    pstNvList->ucTotal = pstSmList->ucMeterNum;
}


HI_VOID mrsStaSearchNotFoundTimesIncrease(NV_APP_MRS_SM_NODE* pstNode)
{
    MRS_NV_SEARCH_PARAM_S stParam = {0};
    (hi_void)memcpy_s(&stParam, sizeof(MRS_NV_SEARCH_PARAM_S), &pstNode->ucParam, sizeof(HI_U8));
    stParam.ucTimes++;
    (hi_void)memcpy_s(&pstNode->ucParam, sizeof(HI_U8), &stParam, sizeof(HI_U8));
}


HI_U32 mrsStaSearchWriteToNv(MRS_SEARCH_NV_METERLIST_STRU* pstNvList)
{
    HI_U16 ausNvId[] = {ID_NV_APP_SM_RESULT1, ID_NV_APP_SM_RESULT2};
    NV_APP_MRS_SM_RESULT* pstNv = HI_NULL;
    HI_U32 ulRet = HI_ERR_SUCCESS;
    HI_U8 ucIndex = 0;
    HI_U8 ucOffset = 0;

    pstNv = mrsToolsMalloc(sizeof(NV_APP_MRS_SM_RESULT));
    if (HI_NULL == pstNv)
    {
        return HI_ERR_NOT_ENOUGH_MEMORY;
    }

    for (ucIndex = 0; ucIndex < sizeof(ausNvId) / sizeof(ausNvId[0]); ucIndex++)
    {
        (hi_void)memset_s(pstNv, sizeof(NV_APP_MRS_SM_RESULT), 0, sizeof(NV_APP_MRS_SM_RESULT));
        (hi_void)memcpy_s(pstNv, sizeof(NV_APP_MRS_SM_RESULT), pstNvList->astNvMeter + ucOffset, sizeof(NV_APP_MRS_SM_RESULT));
        ulRet |= HI_MDM_NV_Write(ausNvId[ucIndex], pstNv, (HI_U16)sizeof(NV_APP_MRS_SM_RESULT));

        ucOffset += NV_SM_RESULT_METER_NUM;
    }

    mrsToolsFree(pstNv);
    return ulRet;
}


// 将抄回来的新表加入搜表结果，非搜表状态下写入NV
HI_U32 mrsStaSearchAddRmMeter(HI_U8* pucAddr, HI_U8 ucProtocol, HI_U16 usDi97)
{
    MRS_STA_SEARCH_CTX *pstCtx = mrsStaSearchCtx();
    MRS_METER_ITEM_S stMeterInf = {0};
    HI_U32 ulRet = HI_ERR_SUCCESS;

    if ((MRS_PROTO645_VER_1997 == ucProtocol) && (0 == usDi97))
    {
        return HI_ERR_FAILURE;
    }

    (hi_void)memset_s(&stMeterInf, sizeof(stMeterInf), 0, sizeof(stMeterInf));
    (hi_void)memcpy_s(stMeterInf.ucMeterAddr, HI_METER_ADDR_LEN, pucAddr, HI_METER_ADDR_LEN);
    stMeterInf.ucValidFlag = HI_TRUE;
    stMeterInf.ucProtocol = ucProtocol;
    stMeterInf.usOption = usDi97;
    stMeterInf.usBaudRate = (HI_U16)mrsGetCurBaudRate();

    ulRet = mrsStaSearchMeterInsert(pstCtx, &stMeterInf);
    if (MRS_SEARCH_STATUS_IDLE == mrsStaSearchGetStatus())
    {
        mrsStaRefreshSearchResultNv(pstCtx, HI_FALSE);
    }

    return ulRet;
}

HI_VOID mrsStaUpdateFirstMeterBaudRate(HI_U8 *pucMeter, HI_U16 usBaudRate)
{
    MRS_STA_SEARCH_CTX *pstCtx = mrsStaSearchCtx();
    HI_U8 ucBaudRateIndex = 0;

    if (HI_ERR_SUCCESS != mrsSrvBaudRate2Index(usBaudRate, &ucBaudRateIndex))
    {
        return;
    }

    if ((mrsToolsMemEq(pstCtx->stSearchCfg.aucNvMeter, pucMeter, HI_METER_ADDR_LEN)
        && pstCtx->stSearchCfg.ucBaudRateIndex != ucBaudRateIndex))
    {
        HI_NV_FTM_COLLECT_PARAM_FILE_S stNv;
        pstCtx->stSearchCfg.ucBaudRateIndex = ucBaudRateIndex;
        if (HI_ERR_SUCCESS == HI_MDM_NV_Read(HI_NV_FTM_COLLECT_PARAM_FILE, &stNv, (HI_U16)sizeof(stNv)))
        {
            stNv.baudrate_index = ucBaudRateIndex;
            (HI_VOID)HI_MDM_NV_Write(HI_NV_FTM_COLLECT_PARAM_FILE, &stNv, (HI_U16)sizeof(stNv));
        }
    }
}


HI_U32 mrsStaSearchMeterInsert(MRS_STA_SEARCH_CTX *pstCtx, MRS_METER_ITEM_S *pstItem)
{
    MRS_SEARCH_METERLIST_STRU *pstMeterList = &(pstCtx->stMeterList);

    mrsDfxCltSmComQueryResultLTS();

    if (mrsStaSearchGetFirstFlag())
    {
        if (!mrsToolsMemEq(pstCtx->stSearchCfg.aucNvMeter, pstItem->ucMeterAddr, HI_METER_ADDR_LEN))
        {
            return HI_ERR_FAILURE;
        }
    }

    if (HI_FALSE == mrsStaSearchGetHasCheckJoinAddrFlag())
    {
        if (!mrsToolsMemEq(pstCtx->stSearchCfg.aucNvMeter, pstItem->ucMeterAddr, HI_METER_ADDR_LEN))
        {
            HI_MDM_UnlockNetwork();
            HI_MDM_ClearBelongNetwork();
            HI_MDM_ClearSoftTfNtbResult(HI_FALSE, HI_FALSE);
        }
        mrsStaSearchSetHasCheckJoinAddrFlag(HI_TRUE);
    }

    if (mrsStaIsMeterInSearchResult(pstItem, HI_TRUE))
    {
        mrsStaUpdateFirstMeterBaudRate(pstItem->ucMeterAddr, pstItem->usBaudRate);
        return HI_ERR_FAILURE;
    }

    if (pstMeterList->ucMeterNum < pstCtx->stSearchCfg.ucMeterMax)
    {
        MRS_METER_ITEM_S *pstMeter = &(pstMeterList->astMeter[pstMeterList->ucMeterNum]);
        (hi_void)memcpy_s(pstMeter, sizeof(MRS_METER_ITEM_S), pstItem, sizeof(MRS_METER_ITEM_S));
        pstMeterList->ucMeterNum++;
    }

    pstMeterList->bMeterFlag = HI_TRUE;
    if (mrsStaAllowAddMeter())
    {
        mrsStaSetMeterListToMac(pstMeterList, HI_FALSE);
    }

    return (HI_U32)((pstMeterList->ucMeterNum >= pstCtx->stSearchCfg.ucMeterMax) ? HI_ERR_FULL : HI_ERR_SUCCESS);
}


HI_VOID mrsStaSearchCheckFirstMeterAddr(MRS_STA_SEARCH_CTX *pstCtx)
{
    HI_U8 ucIdx = 0;
    HI_U8 ucNum = pstCtx->stMeterList.ucMeterNum;
    MRS_METER_ITEM_S * pstItem = pstCtx->stMeterList.astMeter;

    if (ucNum == 0)
    {
        return;
    }

    for (ucIdx = 0; ucIdx < ucNum; ucIdx++)
    {
        if (mrsToolsMemEq(pstItem[ucIdx].ucMeterAddr, pstCtx->stSearchCfg.aucNvMeter, HI_METER_ADDR_LEN))
        {
            mrsStaSearchSetMeter2Mac(pstItem[ucIdx].ucMeterAddr, pstItem[ucIdx].ucProtocol, 0);
            break;
        }
    }

    if (ucIdx == ucNum)
    {
        mrsStaSearchSetMeter2Mac(pstItem[0].ucMeterAddr, pstItem[0].ucProtocol, 0);
    }
}


HI_VOID mrsStaSearchSetMeter2Mac(HI_U8 * pucMeter, HI_U8 ucProtocol, HI_U16 us97DI)
{
    HI_U8 aucMac[HI_PLC_MAC_ADDR_LEN] = {0};

    if (!pucMeter)
    {
        return;
    }

    mrsToolsConvertMeter2Mac(pucMeter, ucProtocol, aucMac);

    if (mrsIsRealClti())
    {
        mrsStaSetMeterToMac(aucMac, STA_TYPE_CLTI, HI_MDM_METER_MAC);
    }
    else
    {
        mrsStaSetMeterToMac(aucMac, STA_TYPE_INVALID, HI_MDM_METER_MAC);
    }

    mrsStaSaveJoinMeterAddr(aucMac, ucProtocol, us97DI);
    mrsSetSyncMacFlg(HI_TRUE);

    mrsSetCollectDevAddr(HI_NV_FTM_COLLECT_PARAM_FILE);
    return;
}


HI_U32 mrsStaVMAddr2MeterList(HI_U8 *pucMeter, HI_U8 ucProtocol)
{
    MRS_STA_SEARCH_CTX * pstCtx = mrsStaSearchCtx();
    MRS_METER_ITEM_S * pstItem = pstCtx->stMeterList.astMeter;

    if (!pucMeter)
    {
        return HI_ERR_FAILURE;
    }

    if (!mrsToolsCheckBCD(pucMeter, HI_METER_ADDR_LEN))
    {
        return HI_ERR_FAILURE;
    }

    mrsStaSearchClearMeterList(pstCtx);

    (hi_void)memcpy_s(pstItem->ucMeterAddr, HI_METER_ADDR_LEN, pucMeter, HI_METER_ADDR_LEN);
    pstItem->ucValidFlag = HI_TRUE;
    pstItem->ucProtocol = ucProtocol;
    pstCtx->stMeterList.ucMeterNum = 1;

    return HI_ERR_SUCCESS;
}


HI_VOID mrsStopCltIISearchMeter(HI_VOID)
{
    MRS_STA_SRV_CTX_STRU * pstSta = mrsStaGetContext();
    MRS_STA_ITEM  *pstItem = mrsSrvQueueTop(&pstSta->stQueModule.stMrQueue);
    MRS_STA_SEARCH_CTX * pstCtx = mrsStaSearchCtx();
    MRS_SEARCH_RUNNING_CLT_II *pstRunning = &(pstCtx->stRunning.unClt.stCltII);

    if ((pstItem != HI_NULL) && (MRS_STA_ITEM_ID_SEARCH_METER_CLT_II == pstItem->id))
    {
        mrsStaTryDeQueue(&pstSta->stQueModule, mrsStaQueueFree);
    }

    MRS_StopTimer(MRS_STA_TIMER_SM_FRAME);
    MRS_StopTimer(MRS_STA_TIMER_SM_BYTE);

    pstRunning->bFrameTimerFlag = HI_FALSE;
    pstRunning->bByteTimerFlag = HI_FALSE;

    mrsStaSearchSetStatus(MRS_SEARCH_STATUS_IDLE);
}


// 使用搜表结果刷新本地地址列表
HI_U32 mrsStaRefreshLocalMeterList(MRS_METER_LIST_S *pstMeterList,
                                                MRS_SEARCH_METERLIST_STRU * pstSmList)
{
    HI_U32 i = 0;
    MRS_METER_ITEM_S * pstItem = HI_NULL;

    if(!pstMeterList || !pstSmList)
    {
        return HI_ERR_FAILURE;
    }

    (hi_void)memset_s(pstMeterList, sizeof(MRS_METER_LIST_S), 0, sizeof(MRS_METER_LIST_S));

    for (i = 0; i < pstSmList->ucMeterNum; i++)
    {
        pstItem = &(pstMeterList->stMeterItem[i]);
        (hi_void)memcpy_s(pstItem, sizeof(MRS_METER_ITEM_S), &pstSmList->astMeter[i], sizeof(MRS_METER_ITEM_S));
        pstItem->usOption = MRS_STA_LOCAL_MAP_ENTRY_TTL_MAX;
    }

    pstMeterList->ucMeterNum = pstSmList->ucMeterNum;
    pstMeterList->ucType = MRS_STA_TYPE_COLLECTOR;

    return HI_ERR_SUCCESS;
}


// 在搜表结果中查找指定电表地址
// 找到返回:HI_TRUE;否则返回:HI_FALSE
HI_BOOL mrsStaFindMeterOfSearchResult(HI_U8 *pucMeter, HI_U8 ucProtocol, HI_BOOL bReplace, HI_U16 usDi97)
{
    MRS_STA_SEARCH_CTX * pstCtx = mrsStaSearchCtx();
    MRS_SEARCH_METERLIST_STRU * pstMeterList = &(pstCtx->stMeterList);
    HI_U8 i = 0;

    for (i = 0; i < pstMeterList->ucMeterNum; i++)
    {
        MRS_METER_ITEM_S * pstItem = &(pstMeterList->astMeter[i]);

        // 已存在与当前地址匹配的表，该地址判定为已存在
        if (mrsMeterAddrMatch(pucMeter, pstItem->ucMeterAddr))
        {
            return HI_TRUE;
        }

        // 97表，新表地址AAH更少，用新表信息替换已存在的表信息
        // [注]: 旧表地址不能匹配新表地址，但新表地址能匹配旧表地址: 说明新表地址AAH更少
        if (bReplace && (pstItem->ucProtocol == MRS_PROTO645_VER_1997)
            && mrsMeterAddrMatch(pstItem->ucMeterAddr, pucMeter))
        {
            (hi_void)memcpy_s(pstItem->ucMeterAddr, HI_METER_ADDR_LEN, pucMeter, HI_METER_ADDR_LEN);
            pstItem->ucValidFlag = HI_TRUE;
            pstItem->ucProtocol = ucProtocol;
            pstItem->usOption = usDi97;

            return HI_TRUE;
        }
    }

    return HI_FALSE;
}

HI_BOOL mrsStaIsMeterInSearchResult(MRS_METER_ITEM_S *pstMeterInf, HI_BOOL bReplace)
{
    MRS_STA_SEARCH_CTX * pstCtx = mrsStaSearchCtx();
    MRS_SEARCH_METERLIST_STRU * pstMeterList = &(pstCtx->stMeterList);
    HI_U8 i = 0;

    for (i = 0; i < pstMeterList->ucMeterNum; i++)
    {
        MRS_METER_ITEM_S * pstItem = &(pstMeterList->astMeter[i]);

        // 已存在与当前地址匹配的表，该地址判定为已存在
        if (mrsMeterAddrMatch(pstMeterInf->ucMeterAddr, pstItem->ucMeterAddr))
        {
            if (bReplace)
            {
                (hi_void)memcpy_s(pstItem, sizeof(MRS_METER_ITEM_S), pstMeterInf, sizeof(MRS_METER_ITEM_S));
            }
            return HI_TRUE;
        }

        // 97表，新表地址AAH更少，用新表信息替换已存在的表信息
        // [注]: 旧表地址不能匹配新表地址，但新表地址能匹配旧表地址: 说明新表地址AAH更少
        if (bReplace && (pstItem->ucProtocol == MRS_PROTO645_VER_1997)
            && mrsMeterAddrMatch(pstItem->ucMeterAddr, pstMeterInf->ucMeterAddr))
        {
            (hi_void)memcpy_s(pstItem, sizeof(MRS_METER_ITEM_S), pstMeterInf, sizeof(MRS_METER_ITEM_S));
            return HI_TRUE;
        }
    }

    return HI_FALSE;
}


//*****************************************************************************
// 函数名称: mrsFindMeterIndexOfSearchResult
// 功能描述: 在搜表结果中查找某块表所在列表中的下标
//
// 参数说明:
//   pucMeter      表地址
//   pusIndex      下标
//
// 返 回 值:
//    HI_ERR_SUCCESS: 成功找到
//    其它错误码
//
// 调用要求: TODO: ...
// 调用举例: TODO: ...
// 作    者: fengxiaomin/00209182 [2015-07-20]
//*****************************************************************************
HI_U32 mrsFindMeterIndexOfSearchResult(HI_U8 pucMeter[], HI_U16 *pusIndex)
{
    MRS_STA_SEARCH_CTX * pstCtx = mrsStaSearchCtx();
    MRS_SEARCH_METERLIST_STRU * pstMeterList = &(pstCtx->stMeterList);
    HI_U8 ucIndex = 0;

    if (!pusIndex)
    {
        return HI_ERR_INVALID_PARAMETER;
    }

    for (ucIndex = 0; ucIndex < pstMeterList->ucMeterNum; ucIndex++)
    {
        MRS_METER_ITEM_S * pstItem = &(pstMeterList->astMeter[ucIndex]);

        // 已存在与当前地址匹配的表，该地址判定为已存在
        if (mrsMeterAddrMatch(pucMeter, pstItem->ucMeterAddr))
        {
            *pusIndex = (HI_U16)ucIndex;

            return HI_ERR_SUCCESS;
        }
    }

    return HI_ERR_NOT_FOUND;
}

//*****************************************************************************
// 函数名称: mrsCltGetNvSMResult
// 功能描述: 获取搜表结果NV中的表信息
//
// 参数说明:
//   ppData      搜表结果NV中的表信息
//
// 返 回 值:
//    HI_ERR_SUCCESS: 成功
//    其它错误码
//
// 调用要求: TODO: ...
// 调用举例: TODO: ...
// 作    者: fengxiaomin/00209182 [2015-07-20]
//*****************************************************************************
HI_U32 mrsCltGetNvSMResult(HI_OUT HI_PVOID *ppData)
{
    HI_U32 ulRet = HI_ERR_SUCCESS;
    HI_U8 ucIndex = 0;
    HI_U16 ausSMResultId[] = {ID_NV_APP_SM_RESULT1,ID_NV_APP_SM_RESULT2};
    NV_APP_MRS_SM_NODE *pstMeter = HI_NULL;

    pstMeter = (NV_APP_MRS_SM_NODE*)mrsToolsMalloc(sizeof(NV_APP_MRS_SM_RESULT)
                                                   * MRS_TOOLS_ARRAY_COUNT(ausSMResultId));
    if (!pstMeter)
    {
        return HI_ERR_MALLOC_FAILUE;
    }

    (hi_void)memset_s(pstMeter, sizeof(NV_APP_MRS_SM_RESULT) * MRS_TOOLS_ARRAY_COUNT(ausSMResultId),0,
        sizeof(NV_APP_MRS_SM_RESULT) * MRS_TOOLS_ARRAY_COUNT(ausSMResultId));

    for (ucIndex = 0; ucIndex < MRS_TOOLS_ARRAY_COUNT(ausSMResultId); ucIndex++)
    {
        ulRet = HI_MDM_NV_Read(ausSMResultId[ucIndex],
                               pstMeter + ucIndex * NV_SM_RESULT_METER_NUM,
                               sizeof(NV_APP_MRS_SM_RESULT));
        if (HI_ERR_SUCCESS != ulRet)
        {
            mrsToolsFree(pstMeter);
            return HI_ERR_FAILURE;
        }
    }

    *ppData = (HI_PVOID)pstMeter;

    return ulRet;
}

HI_U32 mrsStaSetMeterListToMac(MRS_SEARCH_METERLIST_STRU *pstMList, HI_U8 ucClear)
{
    HI_U32 ret = HI_ERR_SUCCESS;
    HI_MAC_ATTR_STRU *pstMeterInf = HI_NULL;
    HI_U16 usInfLen = 0;
    HI_U16 i = 0;
    HI_U16 num = 0;

    if(!pstMList)
    {
        return HI_ERR_BAD_DATA;
    }

    num = (ucClear == HI_FALSE) ? 1 : pstMList->ucMeterNum;
    usInfLen = sizeof(HI_MAC_ATTR_STRU) + (HI_U16)(sizeof(HI_MAC_ENTRY_STRU) * num);
    pstMeterInf = mrsToolsMalloc(usInfLen);
    if(!pstMeterInf)
    {
        return HI_ERR_NO_MORE_MEMORY;
    }

    (hi_void)memset_s(pstMeterInf, usInfLen, 0, usInfLen);
    if (ucClear == HI_FALSE)
    {
        i = pstMList->ucMeterNum - 1;
        mrsToolsConvertMeter2Mac(pstMList->astMeter[i].ucMeterAddr, pstMList->astMeter[i].ucProtocol, pstMeterInf->mac_list[0].mac);
    }
    else
    {
        for (i = 0; i < pstMList->ucMeterNum; i++)
        {
            mrsToolsConvertMeter2Mac(pstMList->astMeter[i].ucMeterAddr, pstMList->astMeter[i].ucProtocol, pstMeterInf->mac_list[i].mac);
        }
    }

    pstMeterInf->list_num = (HI_U8)num;
    pstMeterInf->type = HI_MDM_METER_MAC;
    pstMeterInf->clear_flag = ucClear;

    if ((HI_FALSE == mrsGetSyncMacFlg()) && (pstMeterInf->list_num > 0))
    {
        pstMeterInf->clear_flag = HI_TRUE;
        mrsDfxStaSetAddr2Mac(pstMeterInf->mac_list[0].mac);
        mrsStaSetMacAddr(pstMeterInf->mac_list[0].mac);
        mrsSetSyncMacFlg(HI_TRUE);
    }

    ret = HI_MDM_SetBackupMacAddr(pstMeterInf);

    mrsToolsFree(pstMeterInf);

    return ret;
}


HI_BOOL mrsStaAllowAddMeter(HI_VOID)
{
    MRS_STA_SRV_CTX_STRU * pstSta = mrsStaGetContext();

    if (pstSta->plcState == HI_TRUE)
    {
        HI_U8 aucLocalMac[HI_PLC_MAC_ADDR_LEN] = {0};
        HI_U8 aucCltMac[HI_PLC_MAC_ADDR_LEN] = {0};

        mrsStaGetMacAddr(aucLocalMac);
        MRS_TOOLS_FE_TO_00(aucLocalMac[MRS_MAC_ADDR_INDEX_5]);

        mrsToolsConvertMeter2Mac(mrsGetCollectorAddr(), MRS_PROTO645_VER_2007, aucCltMac);

        if (mrsToolsCheckBCD(aucLocalMac, HI_PLC_MAC_ADDR_LEN)
            && !mrsToolsMemEq(aucCltMac, aucLocalMac, HI_PLC_MAC_ADDR_LEN))
        {
            return HI_FALSE;
        }
    }

    return HI_TRUE;
}


HI_VOID mrsStaSearchSetStatus(HI_U8 ucStatus)
{
    MRS_STA_SEARCH_CTX * pstCtx = mrsStaSearchCtx();

    pstCtx->stRunning.ucSearchStatus = ucStatus;

    if (ucStatus == MRS_SEARCH_STATUS_IN_PROGRESS)
    {
        mrsDfxCltSmLocalStartTime();
    }
    else if (ucStatus == MRS_SEARCH_STATUS_IDLE)
    {
        mrsDfxCltSmLocalEndTime();

        if (mrsToolsIsIICollector())
        {
            mrsCltiiSearchSetState(MRS_CLTII_SEARCH_STATE_IDLE);
        }
    }
}


HI_U8 mrsStaSearchGetStatus(HI_VOID)
{
    MRS_STA_SEARCH_CTX * pstCtx = mrsStaSearchCtx();

    return pstCtx->stRunning.ucSearchStatus;
}


HI_U8 mrsStaSearchEndReason(HI_VOID)
{
    MRS_STA_SEARCH_CTX * pstCtx = mrsStaSearchCtx();

    return pstCtx->stRunning.ucEndReason;
}


HI_VOID mrsStaSearchSetEndReason(HI_U8 ucReason)
{
    MRS_STA_SEARCH_CTX * pstCtx = mrsStaSearchCtx();

    pstCtx->stRunning.ucEndReason = ucReason;
}


HI_VOID mrsStaSearchSetFirstFlag(HI_BOOL bFlag)
{
    MRS_STA_SEARCH_CTX * pstCtx = mrsStaSearchCtx();

    pstCtx->stRunning.bTryFisrtFlag = bFlag;

    if(bFlag && mrsToolsIsIICollector())
    {
        mrsCltiiSearchSetState(MRS_CLTII_SEARCH_STATE_TRYFISRT);
    }
}


HI_BOOL mrsStaSearchGetFirstFlag(HI_VOID)
{
    MRS_STA_SEARCH_CTX * pstCtx = mrsStaSearchCtx();

    return pstCtx->stRunning.bTryFisrtFlag;
}

HI_VOID mrsStaSearchSetHasCheckJoinAddrFlag(HI_BOOL bFlag)
{
    MRS_STA_SEARCH_CTX * pstCtx = mrsStaSearchCtx();

    pstCtx->stRunning.bHasCheckJoinAddr = bFlag;
}


HI_BOOL mrsStaSearchGetHasCheckJoinAddrFlag(HI_VOID)
{
    MRS_STA_SEARCH_CTX * pstCtx = mrsStaSearchCtx();

    return pstCtx->stRunning.bHasCheckJoinAddr;
}

HI_VOID mrsStaRefreshDefaultDi97(HI_VOID)
{
    if (mrsToolsIsIICollector())
    {
        MRS_STA_SEARCH_CTX *pstSmCtx = mrsStaSearchCtx();
        MRS_SEARCH_CFG_STRU *pstSmCfg = &(pstSmCtx->stSearchCfg);
        HI_U32 i = 0;

        pstSmCtx->stRunning.unClt.stCltII.usDefaultDi97 = 0;
        for (i = 0; i < MRS_TOOLS_ALEN(pstSmCfg->ausDI97); i++)
        {
            if (0 != pstSmCfg->ausDI97[i])
            {
                pstSmCtx->stRunning.unClt.stCltII.usDefaultDi97 = pstSmCfg->ausDI97[i];
                break;
            }
        }
    }
}

HI_U16 mrsStaGetDefaultDi97(HI_VOID)
{
    if (mrsToolsIsIICollector())
    {
        MRS_STA_SEARCH_CTX *pstSmCtx = mrsStaSearchCtx();

        return pstSmCtx->stRunning.unClt.stCltII.usDefaultDi97;
    }

    return 0;
}


HI_U32 mrsStaSearchRegisterCmdNotify(mrsSearchStartCmdFunc pfStartNotify, mrsSearchFinishCmdFunc pfFinishNotify)
{
    MRS_SEARCH_STATUS_NOTIFY_INF *pstNotifyInf = &(mrsStaSearchCtx()->stStatusNotify);

    pstNotifyInf->pfStartNotify = pfStartNotify;
    pstNotifyInf->pfFinishNotify = pfFinishNotify;

    return HI_ERR_SUCCESS;
}


HI_U32 mrsStaSearchStartNotify(HI_VOID)
{
    HI_U32 ulRet = HI_ERR_SUCCESS;
    MRS_SEARCH_STATUS_NOTIFY_INF *pstNotifyInf = &(mrsStaSearchCtx()->stStatusNotify);

    if (pstNotifyInf->pfStartNotify)
    {
        ulRet = pstNotifyInf->pfStartNotify();
    }

    return ulRet;
}


HI_U32 mrsStaSearchFinishNotify(HI_U32 ulOption)
{
    HI_U32 ulRet = HI_ERR_SUCCESS;
    MRS_SEARCH_STATUS_NOTIFY_INF *pstNotifyInf = &(mrsStaSearchCtx()->stStatusNotify);

    if (pstNotifyInf->pfFinishNotify)
    {
        ulRet = pstNotifyInf->pfFinishNotify(ulOption);
    }

    return ulRet;
}


MRS_SEARCH_METERLIST_STRU * mrsStaSearchResultList(HI_VOID)
{
    return &(mrsStaSearchCtx()->stMeterList);
}

#if defined(PRODUCT_CFG_PRODUCT_TYPE_CSG_GD)
HI_VOID mrsSetMacTimeout(HI_VOID)
{
    if (mrsToolsIsMeter())
    {
        HI_U8 aucMacAddr[HI_PLC_MAC_ADDR_LEN] = { 0 };

        if (HI_TRUE == mrsGetSyncMacFlg())
        {
            return;
        }

        HI_MDM_GetLocalMacAddr(aucMacAddr);
        mrsStaSetType(STA_TYPE_METER);
        mrsStaSetMeterToMac(aucMacAddr, STA_TYPE_METER, HI_MDM_NV_MAC);
    }
    else
    {
        mrsCsgSetCltAddr2Mac();
    }
}

HI_VOID mrsCsgSetCltAddr2Mac(HI_VOID)
{
    HI_U8 aucLocalMac[HI_PLC_MAC_ADDR_LEN] = {0};
    HI_U8 aucCltMac[HI_PLC_MAC_ADDR_LEN] = {0};
    HI_U8 *pCltAddr = mrsGetCollectorAddr();

    if (mrsToolsMemIsZero(pCltAddr, HI_METER_ADDR_LEN) || !mrsToolsNormalAddr(pCltAddr))
    {
        return;
    }

    mrsStaGetMacAddr(aucLocalMac);
    if (mrsToolsCheckBCD(aucLocalMac, HI_PLC_MAC_ADDR_LEN))
    {
        return;
    }

    mrsToolsConvertMeter2Mac(pCltAddr, MRS_PROTO645_VER_2007, aucCltMac);

    if (mrsToolsIsCLTI())
    {
        mrsStaSetMeterToMac(aucCltMac, STA_TYPE_CLTI, HI_MDM_CLT_MAC);
    }
    else
    {
        mrsStaSetMeterToMac(aucCltMac, STA_TYPE_INVALID, HI_MDM_CLT_MAC);
    }
}
#endif


#endif // STA

