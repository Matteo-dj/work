//*****************************************************************************
//
//                  版权所有 (C), 2001-2014, 华为技术有限公司
//
//*****************************************************************************
//  文 件 名   : mrs_srv_sta_csg_plc.c
//  版 本 号   : V1.0
//  作    者   : cuiate/c00233580
//  生成日期   : 2016-01-09
//  功能描述   : TODO: ...
//
//  函数列表   : TODO: ...
//  修改历史   :
//  1.日    期 : 2016-01-09
//    作    者 : cuiate/c00233580
//    修改内容 : 创建文件
//
//*****************************************************************************

#include "mrs_common.h"
#include "mrs_cmd_msg.h"
#include "mrs_fw_n.h"
#include "mrs_fw_tools.h"
#include "mrs_fw_log.h"
#include "mrs_srv_res.h"
#include "mrs_srv_common.h"
#include "mrs_srv_sta_queue.h"
#include "mrs_srv_sta_baudrate.h"
#include "mrs_srv_parallel_sta.h"
#include "mrs_srv_sta_event.h"
#include "mrs_srv_sta.h"
#include "mrs_srv_csg_plc.h"
#include "mrs_srv_csg_plc_sta.h"
#include "mrs_srv_clti_searchmeter_csg.h"
#include "mrs_srv_sta_searchmeter.h"
#include "mrs_srv_cltii_searchmeter.h"
#include "mrs_srv_sta_event.h"
#include "mrs_time.h"
#include "mrs_srv_capture.h"
#include "hi_mdm.h"
#include "mrs_power_failure.h"



HI_START_HEADER
#if (defined(PRODUCT_CFG_PRODUCT_TYPE_STA) && defined(PRODUCT_CFG_PRODUCT_TYPE_CSG_GD))


HI_PRV HI_VOID mrsStaSyncMeterList(MRS_METER_LIST_S *pstMeterList, MRS_SEARCH_METERLIST_STRU * pstSmList);
HI_PRV HI_BOOL mrsStaCsgTrasmitXrFilter(MRS_STA_SRV_CTX_STRU *pstStaCtx, HI_U8 *pucMeterAddr);

HI_PRV MRS_CSG_PLC_FUNC_TBL_STRU g_astStaPlcFuncTbl[] =
{
    {MRS_CSG_PLC_CTRL_TYPE_ACK, 0, MRS_CSG_PLC_SRV_ID_ACK_CONFIRM, {0, 0}, mrsStaCsgAckConfirmProc},
    {MRS_CSG_PLC_CTRL_TYPE_ACK, 0, MRS_CSG_PLC_SRV_ID_ACK_DENY, {0, 0}, mrsStaCsgAckDenyProc},

    {MRS_CSG_PLC_CTRL_TYPE_TRANS, 0, MRS_CSG_PLC_SRV_ID_TRANS_XR, {0, 0}, mrsStaCsgTransmitXrProc},
    {MRS_CSG_PLC_CTRL_TYPE_TRANS, 0, MRS_CSG_PLC_SRV_ID_PLC_TEST, {0, 0}, mrsStaCsgPlcTestProc},

    {MRS_CSG_PLC_CTRL_TYPE_CMD, 0, MRS_CSG_PLC_SRV_ID_CMD_SM_RESULT, {0, 0}, mrsStaCsgCmdSmResultProc},
    {MRS_CSG_PLC_CTRL_TYPE_CMD, 0, MRS_CSG_PLC_SRV_ID_CMD_DL_METERLIST, {0, 0}, mrsStaCsgCmdDlMeterListProc},
    {MRS_CSG_PLC_CTRL_TYPE_CMD, 0, MRS_CSG_PLC_SRV_ID_CMD_FILE, {0, 0}, mrsStaCsgCmdTransFileProc},
    {MRS_CSG_PLC_CTRL_TYPE_CMD, 0, MRS_CSG_PLC_SRV_ID_CMD_UPG_INFO, {0, 0}, mrsStaCsgCmdQueryInfoProc},
    {MRS_CSG_PLC_CTRL_TYPE_CMD, 0, MRS_CSG_PLC_SRV_ID_CMD_EVENT_FLAG, {0, 0}, mrsStaCsgCmdSetEventFlagProc},
    {MRS_CSG_PLC_CTRL_TYPE_CMD, 0, MRS_CSG_PLC_SRV_ID_CMD_REBOOT, {0, 0}, mrsStaCsgCmdRebootProc},
    {MRS_CSG_PLC_CTRL_TYPE_CMD, 0, MRS_CSG_PLC_SRV_ID_CMD_BIND, {0, 0}, mrsStaCsgCmdBindProc},
    {MRS_CSG_PLC_CTRL_TYPE_CMD, 0, MRS_CSG_PLC_SRV_ID_CMD_TEST, {0,0}, mrsStaCsgCmdTestProc},
    {MRS_CSG_PLC_CTRL_TYPE_REPROT, 0, MRS_CSG_PLC_SRV_ID_REPORT_EVENT, {0, 0}, mrsStaCsgEvtProc},
};


HI_U32 mrsStaCsgPlcModuleInit(HI_VOID)
{
    mrsCsgPlcRegProcTblList(MRS_TOOLS_ALEN(g_astStaPlcFuncTbl), g_astStaPlcFuncTbl);
    return HI_ERR_SUCCESS;
}


HI_U32 mrsStaCsgPlcModuleDeinit(HI_VOID)
{
    mrsCsgPlcRegProcTblList(0, HI_NULL);
    return HI_ERR_SUCCESS;
}


HI_U32 mrsStaCsgAckConfirmProc(HI_PVOID pParam, HI_U16 usSize)
{
    MRS_STA_SRV_CTX_STRU *pstStaCtx = mrsStaGetContext();
    MRS_PLC_FRAME_DATA_STRU *pstPlcFrame = (MRS_PLC_FRAME_DATA_STRU *)pParam;
    MRS_CSG_PLC_FRAME_HEAD *pstFrameHead = (MRS_CSG_PLC_FRAME_HEAD *)pstPlcFrame->pucPayload;

    HI_UNREF_PARAM(usSize);

    if (pstStaCtx->pfnPlcAckProc)
    {
        if (HI_ERR_SUCCESS == pstStaCtx->pfnPlcAckProc(pstFrameHead))
        {
            pstStaCtx->pfnPlcAckProc = HI_NULL;
            pstStaCtx->pfnPlcDenyProc = HI_NULL;
        }
    }

    return HI_ERR_SUCCESS;
}


HI_U32 mrsStaCsgAckDenyProc(HI_PVOID pParam, HI_U16 usSize)
{
    MRS_STA_SRV_CTX_STRU *pstStaCtx = mrsStaGetContext();
    MRS_PLC_FRAME_DATA_STRU *pstPlcFrame = (MRS_PLC_FRAME_DATA_STRU *)pParam;
    MRS_CSG_PLC_FRAME_HEAD *pstFrameHead = (MRS_CSG_PLC_FRAME_HEAD *)pstPlcFrame->pucPayload;

    HI_UNREF_PARAM(usSize);

    if (pstStaCtx->pfnPlcDenyProc)
    {
        if (HI_ERR_SUCCESS == pstStaCtx->pfnPlcDenyProc(pstFrameHead))
        {
            pstStaCtx->pfnPlcAckProc = HI_NULL;
            pstStaCtx->pfnPlcDenyProc = HI_NULL;
        }
    }

    return HI_ERR_SUCCESS;
}


HI_U32 mrsStaCsgTransmitXrProc(HI_PVOID pParam, HI_U16 usSize)
{
    MRS_STA_SRV_CTX_STRU *pstStaCtx = mrsStaGetContext();
    MRS_PLC_FRAME_DATA_STRU *pstPlcFrame = (MRS_PLC_FRAME_DATA_STRU *)pParam;
    MRS_CSG_PLC_FRAME_HEAD *pstFrameHead = (MRS_CSG_PLC_FRAME_HEAD *)pstPlcFrame->pucPayload;
    MRS_CSG_TRANS_XR_DL_STRU *pstXrFrame = (MRS_CSG_TRANS_XR_DL_STRU *)pstFrameHead->aucData;
    MRS_STA_ITEM *pstItem = HI_NULL;
    HI_U16 usPayloadLen = pstXrFrame->usDataLen;
    HI_U8 *pucPayload = pstXrFrame->aucData;
    HI_U8 *pucTemp = HI_NULL;
    HI_U32 ulRet = HI_ERR_SUCCESS;

    HI_UNREF_PARAM(usSize);

    do
    {
        // 报文过滤
        if (mrsStaCsgTrasmitXrFilter(pstStaCtx, pstXrFrame->aucDstAddr))
        {
            break;
        }

        mrsDfxRmPlcDlRx(ID_MRS_CMD_CSG_DATA_TRANSMIT_PLC, (HI_U8 *)pstXrFrame, HI_FALSE);
        if ((HI_U8)pstFrameHead->usRespBit)
        {
            mrsDfxXrPlcRx();
        }

        if (mrsToolsIsCLTI() && !mrsStaGetSupportVM())
        {
            if (HI_ERR_SUCCESS != mrsCsgCltIGenTransmitFrame(pstXrFrame, &pucPayload, &usPayloadLen))
            {
                ulRet = HI_ERR_FAILURE;
                break;
            }

            pucTemp = pucPayload;
        }

        pstItem = (MRS_STA_ITEM *)mrsToolsMalloc(sizeof(MRS_STA_ITEM) + usPayloadLen);
        if (!pstItem)
        {
            ulRet = HI_ERR_NO_MORE_MEMORY;
            break;
        }

#if defined(PRODUCT_CFG_SYS_RST_SOLUTION)
        pstStaCtx->stStaResetInfo.ulRxRMPlcTimes++;
#endif

        (hi_void)memset_s(pstItem, sizeof(MRS_STA_ITEM) + usPayloadLen, 0, sizeof(MRS_STA_ITEM) + usPayloadLen);
        pstItem->from = MRS_STA_QUEUE_FROM_PLC;
        pstItem->bcFlg = pstPlcFrame->is_bc_frame;
        pstItem->time_out = pstXrFrame->ucTimeout;
        pstItem->id = MRS_STA_ITEM_ID_CSG_TRANS_XR;
        pstItem->len = usPayloadLen;
        pstItem->buf = (HI_U8 *)pstItem + sizeof(MRS_STA_ITEM);
        pstItem->seq = pstFrameHead->usSeq;
        pstItem->option = (HI_U8)pstFrameHead->usRespBit;
        (hi_void)memcpy_s(pstItem->buf, usPayloadLen, pucPayload, usPayloadLen);
        (hi_void)memcpy_s(pstItem->aucSrcAddr, HI_METER_ADDR_LEN, pstXrFrame->aucSrcAddr, HI_METER_ADDR_LEN);
        (hi_void)memcpy_s(pstItem->aucDstAddr, HI_METER_ADDR_LEN, pstXrFrame->aucDstAddr, HI_METER_ADDR_LEN);

        pstItem->bAutoStrategy = HI_TRUE;
        pstItem->ucFrameTimeout = (HI_U8)MRS_MS_TO_100MS(mrsGet645FrameInterval(MRS_STA_RM_CFG_AUTO));
        pstItem->time_out = (HI_U8)MRS_MS_TO_100MS(mrsGet645FrameHeadTimeout(MRS_STA_RM_CFG_AUTO));

        {
            HI_U16 usPos = 0;
            HI_U16 usLength = 0;

            // CTRL command
            if ((HI_ERR_SUCCESS == mrsFind645Frame(pucPayload, (HI_S16)usPayloadLen, &usPos, &usLength))
                && MRS_645_IS_CTRL_CMD(pucPayload[usPos + MRS_645_FRAME_CTRL_OFFSET]))
            {
                pstItem->time_out += (HI_U8)MRS_MS_TO_100MS(mrsGet645CtrlCmdDelay(MRS_STA_RM_CFG_AUTO));
            }
        }

        if (!mrsStaTryEnQueue(&pstStaCtx->stQueModule, pstItem))
        {
            mrsToolsFree(pstItem);
        }
#if defined(PRODUCT_CFG_SUPPORT_SDM_MRS_LOG) && defined(MRS_LOG_STA_SWITCH)
        else
        {
            mrsLogFrameInfStatTx(pstItem->buf, pstItem->len, HI_FALSE);
        }
#endif

        mrsStaNotifyQueue();
    } while (0);

    mrsToolsFree(pucTemp);

    return ulRet;
}


HI_VOID mrsStaSyncMeterList(MRS_METER_LIST_S *pstMeterList, MRS_SEARCH_METERLIST_STRU * pstSmList)
{
    MRS_METER_ITEM_S *pItem = HI_NULL;
    HI_U8 i = 0;
    HI_U8 j = 0;

    if (!pstMeterList || !pstSmList)
    {
        return;
    }

    for (i = 0; i < pstSmList->ucMeterNum; i++)
    {
        HI_BOOL bFound = HI_FALSE;

        for (j = 0; j < PRODUCT_CFG_MRS_MAX_METER_IN_MAC_NUM; j++)
        {
            pItem = &pstMeterList->stMeterItem[j];
            if ((pItem->ucValidFlag) && mrsToolsMemEq(pItem->ucMeterAddr, pstSmList->astMeter[i].ucMeterAddr, HI_METER_ADDR_LEN))
            {
                bFound = HI_TRUE;
                break;
            }
        }

        if (bFound)
        {
            continue;
        }

        for (j = 0; j < PRODUCT_CFG_MRS_MAX_METER_IN_MAC_NUM; j++)
        {
            pItem = &pstMeterList->stMeterItem[j];
            if (!pItem->ucValidFlag)
            {
                (hi_void)memcpy_s(pItem, sizeof(MRS_METER_ITEM_S), &(pstSmList->astMeter[i]), sizeof(MRS_METER_ITEM_S));
                pItem->usOption = MRS_STA_LOCAL_MAP_ENTRY_TTL_MAX;
                pstMeterList->ucMeterNum++;
                break;
            }
        }
    }
}


HI_U32 mrsStaCsgCmdSmResultProc(HI_PVOID pParam, HI_U16 usSize)
{
    MRS_PLC_FRAME_DATA_STRU *pstPlcFrame = (MRS_PLC_FRAME_DATA_STRU *)pParam;
    MRS_CSG_PLC_FRAME_HEAD *pstFrameHead = (MRS_CSG_PLC_FRAME_HEAD *)pstPlcFrame->pucPayload;

    MRS_STA_SRV_CTX_STRU *pstStaCtx = mrsStaGetContext();
    MRS_STA_SEARCH_CTX * pstSmCtx = mrsStaSearchCtx();
    MRS_METER_LIST_S *pMeterList = &pstStaCtx->stMeterList;
    MRS_PLC_FRAME_DATA_STRU stPlcUlFrame = {0};
    MRS_CSG_PLC_FRAME_HEAD *pstUlHead = HI_NULL;
    APP_CSG_CMD_SM_RESULT_UL_STRU *pstData = HI_NULL;
    HI_U8 *pucPayload = HI_NULL;
    HI_U16 usPayloadLen = 0;
    HI_U8 i = 0;
    HI_U8 ucIndex = 0;
    HI_U32 ulRet = HI_ERR_SUCCESS;

    HI_UNREF_PARAM(usSize);

    if (!mrsStaGetSupportVM())
    {
        // I采上电5分钟之内，不应答查询搜表结果
        if (mrsToolsIsCLTI() && (HI_MDM_GetSeconds() < MRS_MIN_TO_SEC(pstSmCtx->stSearchCfg.ucPlcTh)))
        {
            return HI_ERR_BUSY;
        }
        // II采上电第一次搜表未完成，不应答查询搜表结果
        else if (mrsToolsIsIICollector() && (!pstSmCtx->stRunning.bFirstOver))
        {
            return HI_ERR_BUSY;
        }
    }

    mrsStaSyncMeterList(&pstStaCtx->stMeterList, &pstSmCtx->stMeterList);

    usPayloadLen = sizeof(MRS_CSG_PLC_FRAME_HEAD) + sizeof(APP_CSG_CMD_SM_RESULT_UL_STRU) + sizeof(APP_CSG_METER_INFO) * pMeterList->ucMeterNum;
    pucPayload = (HI_U8 *)mrsToolsMalloc(usPayloadLen);
    if (!pucPayload)
    {
        return HI_ERR_MALLOC_FAILUE;
    }

    (hi_void)memset_s(pucPayload, usPayloadLen, 0, usPayloadLen);
    pstUlHead = (MRS_CSG_PLC_FRAME_HEAD *)pucPayload;
    pstUlHead->usFrameType = MRS_CSG_PLC_CTRL_TYPE_CMD;
    pstUlHead->usExtendBit = MRS_CSG_PLC_CTRL_EXTEND_BIT_N;
    pstUlHead->usRespBit = MRS_CSG_PLC_CTRL_RESP_BIT_N;
    pstUlHead->usPrm = MRS_CSG_PLC_CTRL_PRM_SLAVE;
    pstUlHead->usDir = MRS_CSG_PLC_CTRL_DIR_UL;
    pstUlHead->ucSrvId = MRS_CSG_PLC_SRV_ID_CMD_SM_RESULT;
    pstUlHead->ucVersion = MRS_CSG_PLC_PROTO_VERSION;
    pstUlHead->usSeq = pstFrameHead->usSeq;
    pstUlHead->usDataLength = usPayloadLen - sizeof(MRS_CSG_PLC_FRAME_HEAD);

    pstData = (APP_CSG_CMD_SM_RESULT_UL_STRU *)pstUlHead->aucData;
    pstData->ucMeterNum = pMeterList->ucMeterNum;

    for (i = 0; (i < PRODUCT_CFG_MRS_MAX_METER_IN_MAC_NUM) && (ucIndex < pMeterList->ucMeterNum); i++)
    {
        MRS_METER_ITEM_S *pstItem = pMeterList->stMeterItem + i;
        if (pstItem->ucValidFlag)
        {
            APP_CSG_METER_INFO *pMeter = pstData->astMeterInf + ucIndex;
            (hi_void)memcpy_s(pMeter->aucMeterAddr, HI_METER_ADDR_LEN, pstItem->ucMeterAddr, HI_METER_ADDR_LEN);
            ucIndex++;
        }
    }

    stPlcUlFrame.usId = ID_MRS_CMD_CSG_DATA_TRANSMIT_PLC;
    stPlcUlFrame.pucPayload = pucPayload;
    stPlcUlFrame.usPayloadLen = usPayloadLen;

    ulRet = MRS_SendPlcFrame(&stPlcUlFrame);
    mrsToolsFree(pucPayload);

    return ulRet;
}

HI_U8 mrsStaGetPLCDenyCode(HI_U32 ulRet)
{
    if (HI_ERR_BUSY == ulRet)
    {
        return APP_CSG_PLC_CCO_BUSY;
    }

    return APP_CSG_PLC_DENY_OTHER;
}

HI_U32 mrsStaCsgCmdDlMeterListProc(HI_PVOID pParam, HI_U16 usSize)
{
    MRS_PLC_FRAME_DATA_STRU *pstPlcFrame = (MRS_PLC_FRAME_DATA_STRU *)pParam;
    MRS_CSG_PLC_FRAME_HEAD *pstFrameHead = (MRS_CSG_PLC_FRAME_HEAD *)pstPlcFrame->pucPayload;
    APP_CSG_CMD_DL_METERLIST_STRU *pstMList = (APP_CSG_CMD_DL_METERLIST_STRU *)pstFrameHead->aucData;
    HI_U8 i = 0;
    HI_U32 ulRet = HI_ERR_SUCCESS;

    HI_UNREF_PARAM(usSize);

    // 仅在采集器场景支持
    if ((!mrsToolsIsCLTI() && !mrsToolsIsIICollector()) || mrsStaGetSupportVM() || (HI_FALSE == HI_MDM_IsStaJoinNet()))
    {
        return HI_ERR_NOT_SUPPORT;
    }

    // 从节点数量不在(0, 16] or 长度不对 判定为异常
    if ((0 == pstMList->ucMeterNum) || (APP_CSG_CMD_DL_METERLIST_MAX < pstMList->ucMeterNum)
        || (pstFrameHead->usDataLength != sizeof(APP_CSG_CMD_DL_METERLIST_STRU) + pstMList->ucMeterNum * sizeof(APP_CSG_METER_INFO)))
    {
        return HI_ERR_BAD_DATA;
    }

    // 从节点必须为BCD码
    for (i = 0; i < pstMList->ucMeterNum; i++)
    {
        if (!mrsToolsNormalAddr(pstMList->astMeterInf[i].aucMeterAddr))
        {
            return HI_ERR_BAD_DATA;
        }
    }

    if (mrsToolsIsCLTI())
    {
        ulRet = mrsCsgCltISendDetectList(pstMList);
    }
    else
    {
        ulRet = mrsCsgCltIISendDetectList(pstMList);
    }

    if (pstFrameHead->usRespBit == MRS_CSG_PLC_CTRL_RESP_BIT_Y)
    {
        HI_U8 ucMac[HI_PLC_MAC_ADDR_LEN]= {0};

        if (HI_ERR_SUCCESS == ulRet)
        {
            mrsCsgPlcAckFrame(ucMac, pstFrameHead->usSeq, HI_FALSE);
        }
        else
        {
            mrsCsgPlcDenyFrame(ucMac, pstFrameHead->usSeq, HI_FALSE, mrsStaGetPLCDenyCode(ulRet));
        }
    }

    return ulRet;
}


HI_U32 mrsStaCsgCmdTransFileProc(HI_PVOID pParam, HI_U16 usSize)
{
    HI_U32 ulRet = HI_ERR_SUCCESS;
    MRS_PLC_FRAME_DATA_STRU *pstPlcFrame = (MRS_PLC_FRAME_DATA_STRU *)pParam;
    MRS_CSG_PLC_FRAME_HEAD *pstFrameHead = (MRS_CSG_PLC_FRAME_HEAD *)pstPlcFrame->pucPayload;

    if (HI_FALSE == HI_MDM_IsStaJoinNet())
    {
        return HI_ERR_SUCCESS;
    }

    ulRet = mrsCsgPlcCmdTransFileProc(pParam, usSize);

    if (pstFrameHead->usRespBit == MRS_CSG_PLC_CTRL_RESP_BIT_Y)
    {
        HI_U8 ucMac[HI_PLC_MAC_ADDR_LEN]= {0};

        if (ulRet == HI_ERR_SUCCESS)
        {
            ulRet = mrsCsgPlcAckFrame(ucMac, pstFrameHead->usSeq, HI_FALSE);
        }
        else
        {
            ulRet = mrsCsgPlcDenyFrame(ucMac, pstFrameHead->usSeq, HI_FALSE, mrsStaGetPLCDenyCode(ulRet));
        }
    }

    return ulRet;
}

HI_U32 mrsStaCsgCmdQueryInfoProc(HI_PVOID pParam, HI_U16 usSize)
{
    return mrsCsgPlcCmdQueryInfoProc(pParam, usSize);
}

HI_U32 mrsStaCsgCmdSetEventFlagProc(HI_PVOID pParam, HI_U16 usSize)
{
    P_MRS_STA_EVT_CTX pstCtx = mrsStaGetEvtCtx();
    MRS_PLC_FRAME_DATA_STRU *pstPlcFrame = (MRS_PLC_FRAME_DATA_STRU *)pParam;
    MRS_CSG_PLC_FRAME_HEAD *pstFrameHead = (MRS_CSG_PLC_FRAME_HEAD *)pstPlcFrame->pucPayload;
    HI_U8 *pucData = pstFrameHead->aucData;
    HI_BOOL bEnable = (*pucData) ? HI_TRUE : HI_FALSE;
    HI_U32 ulRet = HI_ERR_SUCCESS;

    HI_UNREF_PARAM(usSize);

    if (HI_FALSE == HI_MDM_IsStaJoinNet())
    {
        return HI_ERR_SUCCESS;
    }

    do
    {
        if (mrsToolsIsCLTI() || mrsToolsIsIICollector())
        {
            ulRet = HI_ERR_SUCCESS;
            break;
        }

        if (STA_TYPE_METER != mrsStaGetType())
        {
            ulRet = HI_ERR_SKIP;
            break;
        }

        if (!pstCtx->ucStaEnable && bEnable)
        {
            pstCtx->ucStaEnable = bEnable;
            mrsStaEvtOutProc();
            ulRet = HI_ERR_SUCCESS;
            break;
        }

        pstCtx->ucStaEnable = bEnable;

    } while(0);

    if (pstFrameHead->usRespBit == MRS_CSG_PLC_CTRL_RESP_BIT_Y)
    {
        HI_U8 ucMac[HI_PLC_MAC_ADDR_LEN]= {0};

        if (ulRet == HI_ERR_SUCCESS)
        {
            ulRet = mrsCsgPlcAckFrame(ucMac, pstFrameHead->usSeq, HI_FALSE);
        }
        else
        {
            ulRet = mrsCsgPlcDenyFrame(ucMac, pstFrameHead->usSeq, HI_FALSE, mrsStaGetPLCDenyCode(ulRet));
        }
    }

    return ulRet;
}


HI_U32 mrsStaCsgCmdRebootProc(HI_PVOID pParam, HI_U16 usSize)
{
    MRS_PLC_FRAME_DATA_STRU *pstPlcFrame = (MRS_PLC_FRAME_DATA_STRU *)pParam;
    MRS_CSG_PLC_FRAME_HEAD *pstFrameHead = (MRS_CSG_PLC_FRAME_HEAD *)pstPlcFrame->pucPayload;
    HI_U32 ulRet = HI_ERR_SUCCESS;
    HI_U32 ulPeriod = MRS_SRV_STA_SYSTEM_RESET_TIMER_VALUE;

    HI_UNREF_PARAM(usSize);

    if (HI_FALSE == HI_MDM_IsStaJoinNet())
    {
        return HI_ERR_SUCCESS;
    }

    do
    {
        if (pstFrameHead->usDataLength != sizeof(HI_U32))
        {
            ulRet = HI_ERR_INVALID_PARAMETER;
            break;
        }

        if (pstFrameHead->aucData[0] != 0)
        {
            ulPeriod = MRS_SEC_TO_MS(pstFrameHead->aucData[0]);
        }

        ulRet = MRS_StartTimer(MRS_STA_TIMER_CSG_REBOOT, ulPeriod, HI_SYS_TIMER_ONESHOT);
    }while(HI_FALSE);

    if (pstFrameHead->usRespBit == MRS_CSG_PLC_CTRL_RESP_BIT_Y)
    {
        HI_U8 ucMac[HI_PLC_MAC_ADDR_LEN]= {0};

        if (ulRet == HI_ERR_SUCCESS)
        {
            ulRet = mrsCsgPlcAckFrame(ucMac, pstFrameHead->usSeq, HI_FALSE);
        }
        else
        {
            ulRet = mrsCsgPlcDenyFrame(ucMac, pstFrameHead->usSeq, HI_FALSE, mrsStaGetPLCDenyCode(ulRet));
        }
    }

    return ulRet;
}

/* BEGIN: Add by xiangchaobin/00378718, 2018/7/11  PN: */
// 锁定命令
HI_U32 mrsStaCsgCmdBindProc(HI_PVOID pParam, HI_U16 usSize)
{
    HI_U32 ulRet = HI_ERR_SUCCESS;
    MRS_PLC_FRAME_DATA_STRU *pstPlcFrame = (MRS_PLC_FRAME_DATA_STRU *)pParam;
    MRS_CSG_PLC_FRAME_HEAD *pstFrameHead = (MRS_CSG_PLC_FRAME_HEAD *)pstPlcFrame->pucPayload;
    MRS_PLC_FRAME_DATA_STRU stPlcUlFrame = {0};
    MRS_CSG_PLC_FRAME_HEAD *pstUlHead = HI_NULL;
    HI_U8 *pucPayload = HI_NULL;
    HI_U16 usPayloadLen = sizeof(MRS_CSG_PLC_FRAME_HEAD);

    HI_UNREF_PARAM(usSize);

    ulRet = mrsStaBindNetwork();
    if (HI_ERR_SUCCESS != ulRet)
    {
        HI_DIAG_LOG_MSG_E1(MRS_FILE_LOG_FLAG_016, HI_DIAG_MT("[CCO-SM]-BIND bind fail ret = []"), ulRet);
    }

    pucPayload = (HI_U8 *)mrsToolsMalloc(usPayloadLen);
    if (!pucPayload)
    {
        return HI_ERR_MALLOC_FAILUE;
    }

    (hi_void)memset_s(pucPayload, usPayloadLen, 0, usPayloadLen);
    pstUlHead = (MRS_CSG_PLC_FRAME_HEAD *)pucPayload;
    pstUlHead->usFrameType = MRS_CSG_PLC_CTRL_TYPE_CMD;
    pstUlHead->usExtendBit = MRS_CSG_PLC_CTRL_EXTEND_BIT_N;
    pstUlHead->usRespBit = MRS_CSG_PLC_CTRL_RESP_BIT_N;
    pstUlHead->usPrm = MRS_CSG_PLC_CTRL_PRM_SLAVE;
    pstUlHead->usDir = MRS_CSG_PLC_CTRL_DIR_UL;
    pstUlHead->ucSrvId = MRS_CSG_PLC_SRV_ID_CMD_BIND;
    pstUlHead->ucVersion = MRS_CSG_PLC_PROTO_VERSION;
    pstUlHead->usSeq = pstFrameHead->usSeq;
    pstUlHead->usDataLength = usPayloadLen - sizeof(MRS_CSG_PLC_FRAME_HEAD);

    stPlcUlFrame.usId = ID_MRS_CMD_CSG_DATA_TRANSMIT_PLC;
    stPlcUlFrame.pucPayload = pucPayload;
    stPlcUlFrame.usPayloadLen = usPayloadLen;

    ulRet = MRS_SendPlcFrame(&stPlcUlFrame);
    mrsToolsFree(pucPayload);

    return ulRet;
}
/* END: Add by xiangchaobin/00378718, 2018/7/11  PN: */

/* BEGIN: Modified by cuichunyan/00276962, 2016/8/25  PN: */
//宽带互联互通测试帧
HI_U32 mrsStaCsgCmdTestProc(HI_PVOID pParam, HI_U16 usSize)
{
    MRS_PLC_FRAME_DATA_STRU *pstPlcFrame = (MRS_PLC_FRAME_DATA_STRU *)pParam;
    MRS_CSG_PLC_FRAME_HEAD *pstFrameHead = (MRS_CSG_PLC_FRAME_HEAD *)pstPlcFrame->pucPayload;

    APP_CSG_CMD_TEST_MODE_STRU * pstInfo = (APP_CSG_CMD_TEST_MODE_STRU *)pstFrameHead->aucData;
    HI_U32 ulRet = HI_ERR_SUCCESS;

    HI_UNREF_PARAM(usSize);

    do
    {
        if ((pstInfo == HI_NULL)
            || (pstFrameHead->usDataLength != (sizeof(APP_CSG_CMD_TEST_MODE_STRU) + pstInfo->usPayloadLen)))
        {
            ulRet = HI_ERR_INVALID_PARAMETER;
            break;
        }

        if ((HI_MDM_GetSeconds() < MRS_CSG_PHYSICAL_TEST_MODE_LISTEN) && (pstInfo->ucId == APP_CSG_PLC_TEST_MODE))
        {
            HI_MDM_EnterPhyTestMode(PHYSICAL_TEST_MODE_LOOPBACK);
            HI_MDM_SetWorkFreq(pstInfo->aucPayload[0] & MRS_PLC_TEST_FREQ_MASK);
            MRS_StartTimer(MRS_STA_TIMER_MODE_PHYSICAL_TEST, MRS_MIN_TO_MS(MRS_TEST_MODE_DURATION), HI_SYS_TIMER_ONESHOT);//设置10分钟定时器 nv 读取
        }
    }while(HI_FALSE);

    if (pstFrameHead->usRespBit == MRS_CSG_PLC_CTRL_RESP_BIT_Y)
    {
        HI_U8 ucMac[HI_PLC_MAC_ADDR_LEN]= {0};

        if (HI_ERR_SUCCESS == ulRet)
        {
            ulRet = mrsCsgPlcAckFrame(ucMac, pstFrameHead->usSeq, HI_FALSE);
        }
        else
        {
            ulRet = mrsCsgPlcDenyFrame(ucMac, pstFrameHead->usSeq, HI_FALSE, mrsStaGetPLCDenyCode(ulRet));
        }
    }

    return ulRet;

}
/* END: Modified by cuichunyan/00276962, 2016/8/25  */

HI_U32 mrsStaCsgTransmitXr2CCO(HI_U8 *pucData, HI_U16 usDataLen, MRS_STA_ITEM *pstItem)
{
    MRS_PLC_FRAME_DATA_STRU stPlcFrame = {0};
    MRS_CSG_PLC_FRAME_HEAD *pstFrameHead = HI_NULL;
    MRS_CSG_TRANS_XR_UL_STRU *pstTransData = HI_NULL;
    HI_U8 *pucPayload = HI_NULL;
    HI_U16 usPayloadLen = 0;
    HI_U32 ulRet = HI_ERR_SUCCESS;

    usPayloadLen = (HI_U16)(sizeof(MRS_CSG_PLC_FRAME_HEAD) + sizeof(MRS_CSG_TRANS_XR_UL_STRU) + usDataLen);
    pucPayload = (HI_U8 *)mrsToolsMalloc(usPayloadLen);
    if (!pucPayload)
    {
        return HI_ERR_NO_MORE_MEMORY;
    }

    (hi_void)memset_s(pucPayload, usPayloadLen, 0, usPayloadLen);

    pstFrameHead = (MRS_CSG_PLC_FRAME_HEAD *)pucPayload;
    pstFrameHead->usFrameType = MRS_CSG_PLC_CTRL_TYPE_TRANS;
    pstFrameHead->usExtendBit = MRS_CSG_PLC_CTRL_EXTEND_BIT_N;
    pstFrameHead->usRespBit = MRS_CSG_PLC_CTRL_RESP_BIT_N;
    pstFrameHead->usPrm = MRS_CSG_PLC_CTRL_PRM_SLAVE;
    pstFrameHead->usDir = MRS_CSG_PLC_CTRL_DIR_UL;
    pstFrameHead->ucSrvId = MRS_CSG_PLC_SRV_ID_TRANS_XR;
    pstFrameHead->ucVersion = MRS_CSG_PLC_PROTO_VERSION;
    pstFrameHead->usSeq = pstItem->seq;
    pstFrameHead->usDataLength = sizeof(MRS_CSG_TRANS_XR_UL_STRU) + usDataLen;

    pstTransData = (MRS_CSG_TRANS_XR_UL_STRU *)pstFrameHead->aucData;
    (hi_void)memcpy_s(pstTransData->aucSrcAddr, HI_METER_ADDR_LEN, pstItem->aucDstAddr, HI_METER_ADDR_LEN);
    (hi_void)memcpy_s(pstTransData->aucDstAddr, HI_METER_ADDR_LEN, pstItem->aucSrcAddr, HI_METER_ADDR_LEN);

    pstTransData->usDataLen = usDataLen;
    (hi_void)memcpy_s(pstTransData->aucData, usDataLen, pucData, usDataLen);

    (hi_void)memset_s(&stPlcFrame, sizeof(stPlcFrame), 0, sizeof(stPlcFrame));
    stPlcFrame.usId = ID_MRS_CMD_CSG_DATA_TRANSMIT_PLC;
    stPlcFrame.usPayloadLen = usPayloadLen;
    stPlcFrame.pucPayload = pucPayload;

    ulRet = MRS_SendPlcFrame(&stPlcFrame);
    if (HI_ERR_SUCCESS == ulRet)
    {
        mrsDfxXrPlcTx();
        mrsDfxRmPlcUpAfn(stPlcFrame.usId, pucData, ulRet);
    }

    mrsToolsFree(pucPayload);

    return ulRet;
}

HI_U32 mrsStaCsgPlcTestProc(HI_PVOID pParam, HI_U16 usSize)
{
    MRS_PLC_FRAME_DATA_STRU *pstPlcFrame = (MRS_PLC_FRAME_DATA_STRU *)pParam;
    MRS_CSG_PLC_FRAME_HEAD *pstRecvFrameHead = HI_NULL;
    APP_CSG_PLC_TEST_STRU * pstRecvPlcTestFrame = HI_NULL;
    MRS_PLC_FRAME_DATA_STRU stPlcFrame = {0};
    MRS_CSG_PLC_FRAME_HEAD *pstFrameHead = HI_NULL;
    HI_U16 usPayloadLen = 0;
    HI_U8 *pucPayload = HI_NULL;
    HI_U32 ulRet = HI_ERR_SUCCESS;

    if (!pParam)
    {
        return HI_ERR_INVALID_PARAMETER;
    }

    pstRecvFrameHead = (MRS_CSG_PLC_FRAME_HEAD *)pstPlcFrame->pucPayload;
    pstRecvPlcTestFrame = (APP_CSG_PLC_TEST_STRU *)pstRecvFrameHead->aucData;
	if(usSize != sizeof(MRS_PLC_FRAME_DATA_STRU) + sizeof(MRS_CSG_PLC_FRAME_HEAD) + sizeof(APP_CSG_PLC_TEST_STRU) + pstRecvPlcTestFrame->usExpLen){
        return HI_ERR_INVALID_PARAMETER;
	}
    usPayloadLen = (HI_U16)(sizeof(MRS_CSG_PLC_FRAME_HEAD)  + sizeof(APP_CSG_PLC_TEST_STRU) + pstRecvPlcTestFrame->usExpLen);

    pucPayload = mrsToolsMalloc(usPayloadLen);
    if (!pucPayload)
    {
        return HI_ERR_MALLOC_FAILUE;
    }

    (hi_void)memset_s(pucPayload, usPayloadLen, 0, usPayloadLen);

    pstFrameHead = (MRS_CSG_PLC_FRAME_HEAD *)pucPayload;
    pstFrameHead->usFrameType = MRS_CSG_PLC_CTRL_TYPE_TRANS;
    pstFrameHead->usExtendBit = MRS_CSG_PLC_CTRL_EXTEND_BIT_N;
    pstFrameHead->usRespBit = MRS_CSG_PLC_CTRL_RESP_BIT_N;
    pstFrameHead->usPrm = MRS_CSG_PLC_CTRL_PRM_SLAVE;
    pstFrameHead->usDir = MRS_CSG_PLC_CTRL_DIR_UL;
    pstFrameHead->ucSrvId = MRS_CSG_PLC_SRV_ID_PLC_TEST;
    pstFrameHead->ucVersion = MRS_CSG_PLC_PROTO_VERSION;
    pstFrameHead->usSeq = pstRecvFrameHead->usSeq;
    pstFrameHead->usDataLength = sizeof(APP_CSG_PLC_TEST_STRU) + pstRecvPlcTestFrame->usExpLen;

    (hi_void)memcpy_s(pstFrameHead->aucData, pstFrameHead->usDataLength, pstRecvPlcTestFrame, pstFrameHead->usDataLength);
    (hi_void)memset_s(&stPlcFrame, sizeof(stPlcFrame), 0, sizeof(stPlcFrame));
    stPlcFrame.usId = ID_MRS_CMD_CSG_DATA_TRANSMIT_PLC;
    stPlcFrame.usPayloadLen = usPayloadLen;
    stPlcFrame.pucPayload = pucPayload;

    ulRet = MRS_SendPlcFrame(&stPlcFrame);
    if (HI_ERR_SUCCESS == ulRet)
    {
        mrsDfxXrPlcTx();
    }

    mrsToolsFree(pucPayload);
    return ulRet;
}


HI_BOOL mrsStaCsgTrasmitXrFilter(MRS_STA_SRV_CTX_STRU *pstStaCtx, HI_U8 *pucMeterAddr)
{
    HI_BOOL bResult = HI_FALSE;
    HI_U16 usIndex = 0;

    if (HI_FALSE == HI_MDM_IsStaJoinNet())
    {
        return HI_TRUE;
    }

    if (!mrsStaGetSupportVM())
    {
        // 模拟表处于关闭状态
        if (mrsToolsIsCLTI() || mrsToolsIsIICollector())
        {
            // 采集器产品形态-不过滤
            return HI_FALSE;
        }

        if (mrsToolsBroadCastAddr(pucMeterAddr))
        {
            // 广播地址-不过滤
            return HI_FALSE;
        }
    }

    bResult = mrsFindMeterList(&pstStaCtx->stMeterList, pucMeterAddr, &usIndex);
    return !bResult;
}

HI_U32 mrsStaCsgEvtProc(HI_PVOID pParam, HI_U16 usSize)
{
    MRS_PLC_FRAME_DATA_STRU *pstPlcFrame = (MRS_PLC_FRAME_DATA_STRU *)pParam;
    MRS_CSG_PLC_FRAME_HEAD *pstFrameHead = (MRS_CSG_PLC_FRAME_HEAD *)pstPlcFrame->pucPayload;
    HI_U32 ret = HI_ERR_SUCCESS;
    POWER_FAILURE_CTRL_S *power_failure_bitmap = mrsGetPowerFailureCtrlInfo();
    POWER_FAILURE_RCV_MAC_REPORT_S *deviceInfo = mrsGetPowerFailureMacInfo();
    NV_APP_MRS_EVT_CFG_STRU *powerFailureNvCtrl = mrsGetPowerFailureNvCtrl();

    HI_UNREF_PARAM(usSize);
    
    //停电标志为0时不进行处理
    if(MRS_EVENT_FUN_REPORT_MODUL == pstFrameHead->aucData[0]
        && MRS_CSG_PLC_CTRL_DIR_UL == pstFrameHead->usDir
        && MRS_STA_DETECT_POWER_FAILURE_ENABLE == powerFailureNvCtrl->ucPowerFailureReportEnable)
    {

        if(pstPlcFrame->usPayloadLen != pstFrameHead->usDataLength + sizeof(MRS_CSG_PLC_FRAME_HEAD)
            || pstFrameHead->usDataLength <= DATA_FIX_SIZE)
        {
            return HI_ERR_BAD_DATA;
        }

        HI_DIAG_LOG_BUF(MRS_FILE_LOG_BUF_1000, HI_DIAG_MT("[PCO-MR]-PW get other data"),
                        pstFrameHead->aucData,pstFrameHead->usDataLength);

        mrsGetOthersPowerFailureFrameData(pstFrameHead);

        if(HI_FALSE == deviceInfo->devicePowerFailure && HI_FALSE == power_failure_bitmap->isRedayToSend)
        {
            HI_DIAG_LOG_MSG_E0(MRS_FILE_LOG_FLAG_067, HI_DIAG_MT("[PCO-MR-PW,in handle other pwf]"));

            ret = mrsHandleOthersPowerFailureFrame();
            return ret;
        }
    }

    return ret;
}


#endif
HI_END_HEADER
