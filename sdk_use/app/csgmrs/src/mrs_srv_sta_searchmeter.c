//*****************************************************************************
//
//                  ��Ȩ���� (C), 1998-2013, ��Ϊ�������޹�˾
//
//*****************************************************************************
//  �� �� ��   : mrs_srv_sta_searchmeter.c
//  �� �� ��   : V1.0
//  ��    ��   : cuiate/00233580
//  ��������   : 2013-08-05
//  ��������   : �ɼ���/STA�ѱ������ӿڶ���
//
//  �����б�   : TODO: ...
//  �޸���ʷ   :
//  1.��    �� : 2013-08-05
//    ��    �� : cuiate/00233580
//    �޸����� : �����ļ�
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


// ��NV�еĺϷ��ڵ���ص������ĵ�NV�б���
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

            // DI�Ϸ����ж�
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
                    // 97��DI����NV���õ��ѱ�DIֵ���޸�ΪĬ��DIֵ
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
// ���ַ����Լ���͡��ϻ�ֵ�жϣ�DI�Ϸ����Ȳ��ж�
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


// ������Ϸ��ڵ����NV�б�NV�б�δ�ѵ�����������������
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

        // �ȼ��ñ��ַ�Ƿ������б���
        mrsMeterAddrPreProc(aucTempAddr, pstTempNode->aucAddr, mrsStaSearchGetNodeProtocol(pstNode));
        if (mrsToolsMemEq(aucMeterAddr, aucTempAddr, HI_METER_ADDR_LEN))
        {
            // ��δ�ѵ��Ĵ������б��иõ�ַ�Ĵ����󣬲��ٲ���
            if (ucNotFoundTimes >= mrsStaSearchGetNotFoundTimes(pstTempNode))
            {
                return HI_ERR_FAILURE;
            }

            // ���Բ��룬��ɾ���б��иñ��ַ�ڵ�
            for (ucMoveIndex = ucIndex; ucMoveIndex < pstNvList->ucTotal - 1; ucMoveIndex++)
            {
                (hi_void)memcpy_s(&pstNvList->astNvMeter[ucMoveIndex], sizeof(NV_APP_MRS_SM_NODE), &pstNvList->astNvMeter[ucMoveIndex + 1], sizeof(NV_APP_MRS_SM_NODE));
            }

            (hi_void)memset_s(&pstNvList->astNvMeter[pstNvList->ucTotal - 1], sizeof(NV_APP_MRS_SM_NODE), 0, sizeof(NV_APP_MRS_SM_NODE));
            pstNvList->ucTotal--;
            break;
        }
    }

    // ����δ�ѵ������ҵ��ô�����ĩβλ�ã��ڳ�λ�ã�����ڵ�
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

    // δ�ѵ��������������������ֱ�ӷ����б�ĩβ
    return mrsStaSearchAppendNvNode(pstNvList, pstNode);
}


// ֱ�ӽ��Ϸ��ڵ�׷�����б�ĩβ���б����򲻼�
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


// ���´洢�ѱ�����Ϣ�б��NV������
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

    // �Ƚ���ǰ�ѱ���������ʱ�б�
    (hi_void)memset_s(pstTempNvList, sizeof(MRS_SEARCH_NV_METERLIST_STRU), 0, sizeof(MRS_SEARCH_NV_METERLIST_STRU));
    mrsStaSearchGetResult(pstTempNvList, pstSmList);

    // �ټ���������NV�б�ԭ�еĵ�
    if (bSearchFinish)
    {
        // �ѱ�����������ֻ����ԭδ�ѵ�����С��2�ĵ㣬������1��������ʱ�б�
        // ԭδ�ѵ�����Ϊ2�ĵ㲻�ټ��룬���ϻ���(Ҳ�п����Ѿ����ѱ�����)
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
        // �����ѱ������������ǳ����±����NV���ѱ���;ֹͣ��δ�ѵ���������
        for (ucIndex = 0; ucIndex < pstNvList->ucTotal; ucIndex++)
        {
            mrsStaSearchNvNodeInsert(pstTempNvList, &pstNvList->astNvMeter[ucIndex]);
        }
    }

    // ����ʱ�б�д��NV
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


// ���ѱ�������NV�б�
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


// �����������±�����ѱ��������ѱ�״̬��д��NV
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


// ʹ���ѱ���ˢ�±��ص�ַ�б�
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


// ���ѱ����в���ָ������ַ
// �ҵ�����:HI_TRUE;���򷵻�:HI_FALSE
HI_BOOL mrsStaFindMeterOfSearchResult(HI_U8 *pucMeter, HI_U8 ucProtocol, HI_BOOL bReplace, HI_U16 usDi97)
{
    MRS_STA_SEARCH_CTX * pstCtx = mrsStaSearchCtx();
    MRS_SEARCH_METERLIST_STRU * pstMeterList = &(pstCtx->stMeterList);
    HI_U8 i = 0;

    for (i = 0; i < pstMeterList->ucMeterNum; i++)
    {
        MRS_METER_ITEM_S * pstItem = &(pstMeterList->astMeter[i]);

        // �Ѵ����뵱ǰ��ַƥ��ı��õ�ַ�ж�Ϊ�Ѵ���
        if (mrsMeterAddrMatch(pucMeter, pstItem->ucMeterAddr))
        {
            return HI_TRUE;
        }

        // 97���±��ַAAH���٣����±���Ϣ�滻�Ѵ��ڵı���Ϣ
        // [ע]: �ɱ��ַ����ƥ���±��ַ�����±��ַ��ƥ��ɱ��ַ: ˵���±��ַAAH����
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

        // �Ѵ����뵱ǰ��ַƥ��ı��õ�ַ�ж�Ϊ�Ѵ���
        if (mrsMeterAddrMatch(pstMeterInf->ucMeterAddr, pstItem->ucMeterAddr))
        {
            if (bReplace)
            {
                (hi_void)memcpy_s(pstItem, sizeof(MRS_METER_ITEM_S), pstMeterInf, sizeof(MRS_METER_ITEM_S));
            }
            return HI_TRUE;
        }

        // 97���±��ַAAH���٣����±���Ϣ�滻�Ѵ��ڵı���Ϣ
        // [ע]: �ɱ��ַ����ƥ���±��ַ�����±��ַ��ƥ��ɱ��ַ: ˵���±��ַAAH����
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
// ��������: mrsFindMeterIndexOfSearchResult
// ��������: ���ѱ����в���ĳ��������б��е��±�
//
// ����˵��:
//   pucMeter      ���ַ
//   pusIndex      �±�
//
// �� �� ֵ:
//    HI_ERR_SUCCESS: �ɹ��ҵ�
//    ����������
//
// ����Ҫ��: TODO: ...
// ���þ���: TODO: ...
// ��    ��: fengxiaomin/00209182 [2015-07-20]
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

        // �Ѵ����뵱ǰ��ַƥ��ı��õ�ַ�ж�Ϊ�Ѵ���
        if (mrsMeterAddrMatch(pucMeter, pstItem->ucMeterAddr))
        {
            *pusIndex = (HI_U16)ucIndex;

            return HI_ERR_SUCCESS;
        }
    }

    return HI_ERR_NOT_FOUND;
}

//*****************************************************************************
// ��������: mrsCltGetNvSMResult
// ��������: ��ȡ�ѱ���NV�еı���Ϣ
//
// ����˵��:
//   ppData      �ѱ���NV�еı���Ϣ
//
// �� �� ֵ:
//    HI_ERR_SUCCESS: �ɹ�
//    ����������
//
// ����Ҫ��: TODO: ...
// ���þ���: TODO: ...
// ��    ��: fengxiaomin/00209182 [2015-07-20]
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

