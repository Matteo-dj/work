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
#include "mrs_fw_proto376_2_echo.h"
#include "mrs_srv_common.h"
#include "mrs_srv_cco_macmap.h"
#include "mrs_srv_list.h"
#include "mrs_srv_cco_event.h"
#include "mrs_srv_cco.h"
#include "mrs_srv_anytime_readmeter.h"
#include "mrs_srv_csg_plc.h"
#include "mrs_srv_csg_plc_cco.h"
#include "hi_app.h"
#include "mrs_srv_cco_searchmeter.h"
#include "mrs_fw_proto_csg_echo_cco.h"
#include "mrs_srv_cco_mr_task.h"
#include "mrs_srv_readmeter_manage.h"

HI_START_HEADER
#if (defined(PRODUCT_CFG_PRODUCT_TYPE_CCO) && defined(PRODUCT_CFG_PRODUCT_TYPE_CSG_GD))

MRS_CSG_PLC_FUNC_TBL_STRU g_astCcoPlcFuncTbl[] =
{
    {MRS_CSG_PLC_CTRL_TYPE_ACK, 0, MRS_CSG_PLC_SRV_ID_ACK_CONFIRM, {0, 0}, mrsCcoCsgAckConfirmProc},
    {MRS_CSG_PLC_CTRL_TYPE_ACK, 0, MRS_CSG_PLC_SRV_ID_ACK_DENY, {0, 0}, mrsCcoCsgAckDenyProc},

    {MRS_CSG_PLC_CTRL_TYPE_TRANS, 0, MRS_CSG_PLC_SRV_ID_TRANS_XR, {0, 0}, mrsCcoCsgTransmitXrProc},

    {MRS_CSG_PLC_CTRL_TYPE_TRANS, 0, MRS_CSG_PLC_SRV_ID_PLC_TEST, {0, 0}, mrsCcoCsgPlcTestProc},

    {MRS_CSG_PLC_CTRL_TYPE_CMD, 0, MRS_CSG_PLC_SRV_ID_CMD_SM_RESULT, {0, 0}, mrsCcoCsgCmdSmResultProc},
    {MRS_CSG_PLC_CTRL_TYPE_CMD, 0, MRS_CSG_PLC_SRV_ID_CMD_DL_METERLIST, {0, 0}, HI_NULL},
    {MRS_CSG_PLC_CTRL_TYPE_CMD, 0, MRS_CSG_PLC_SRV_ID_CMD_FILE, {0, 0}, mrsCcoCsgCmdTransFileProc},
    {MRS_CSG_PLC_CTRL_TYPE_CMD, 0, APP_CSG_PLC_SRV_ID_CMD_UPG_INFO, {0, 0}, mrsCcoCsgCmdQueryInfoProc},
    {MRS_CSG_PLC_CTRL_TYPE_CMD, 0, MRS_CSG_PLC_SRV_ID_CMD_EVENT_FLAG, {0, 0}, mrsCcoCsgCmdSetEventFlagProc},
    {MRS_CSG_PLC_CTRL_TYPE_CMD, 0, MRS_CSG_PLC_SRV_ID_CMD_REBOOT, {0, 0}, mrsCcoCsgCmdRebootProc},
    {MRS_CSG_PLC_CTRL_TYPE_CMD, 0, MRS_CSG_PLC_SRV_ID_CMD_BIND, {0, 0}, mrsCcoCsgCmdBindProc},

    {MRS_CSG_PLC_CTRL_TYPE_REPROT, 0, MRS_CSG_PLC_SRV_ID_REPORT_EVENT, {0, 0}, mrsCcoCsgReportEventProc},
};


HI_U32 mrsCcoCsgPlcModuleInit(HI_VOID)
{
    mrsCsgPlcRegProcTblList(MRS_TOOLS_ALEN(g_astCcoPlcFuncTbl), g_astCcoPlcFuncTbl);
    return HI_ERR_SUCCESS;
}


HI_U32 mrsCcoCsgPlcModuleDeinit(HI_VOID)
{
    mrsCsgPlcRegProcTblList(0, HI_NULL);
    return HI_ERR_SUCCESS;
}


HI_U32 mrsCcoCsgPlcAckFrame(HI_U8 *pucMacAddr, HI_U16 usSeq)
{
    return mrsCsgPlcAckFrame(pucMacAddr, usSeq, HI_TRUE);
}


HI_U32 mrsCcoCsgPlcDenyFrame(HI_U8 *pucMacAddr, HI_U16 usSeq, HI_U8 ucDeny)
{
    return mrsCsgPlcDenyFrame(pucMacAddr, usSeq, HI_TRUE, ucDeny);
}


HI_U32 mrsCcoCsgAckConfirmProc(HI_PVOID pParam, HI_U16 usSize)
{
    HI_UNREF_PARAM(pParam);
    HI_UNREF_PARAM(usSize);
    return HI_ERR_NOT_SUPPORT;
}


HI_U32 mrsCcoCsgAckDenyProc(HI_PVOID pParam, HI_U16 usSize)
{
    HI_UNREF_PARAM(pParam);
    HI_UNREF_PARAM(usSize);
    return HI_ERR_NOT_SUPPORT;
}


HI_U32 mrsCcoCsgTransmitXrProc(HI_PVOID pParam, HI_U16 usSize)
{
    MRS_CCO_SRV_CTX_STRU *pstCcoCtx = mrsCcoGetContext();
    MRS_PLC_FRAME_DATA_STRU *pstPlcFrame = (MRS_PLC_FRAME_DATA_STRU *)pParam;
    MRS_CSG_PLC_FRAME_HEAD *pstFrameHead = (MRS_CSG_PLC_FRAME_HEAD *)pstPlcFrame->pucPayload;
    MRS_CSG_TRANS_XR_UL_STRU *pstXrFrame = (MRS_CSG_TRANS_XR_UL_STRU *)pstFrameHead->aucData;
    MRS_DEVICE_INF_STRU stMeter;
    MRS_CCO_MR_TASK_PROC_MODE_E eMode = mrsCcoMrTaskGetMode();

    HI_UNREF_PARAM(usSize);

#if defined(PRODUCT_CFG_SYS_RST_SOLUTION)
    pstCcoCtx->stResetStatInfo.ulRMStatTimes++;
#endif

    mrsDfxRmPlcUpRx(ID_MRS_CMD_CSG_DATA_TRANSMIT_PLC, pstFrameHead->aucData, HI_FALSE);

    if (MRS_CCO_MR_TASK_MODE_PR != eMode)   // 透抄接收
    {
        mrsDfxXrPlcRx();

        if (pstCcoCtx->status & MRS_CCO_STATUS_ANYTIME_READMETER)
        {
            mrsCsgAnyTimeReadMeterReceivePlcProcess(pstPlcFrame->pucPayload, pstPlcFrame->usPayloadLen);

            if (pstPlcFrame->is_bc_frame)
            {
                mrsDfxBcRmOkCnt();
            }
            else if (pstCcoCtx->stAnytimeRMModel.bIsBC)
            {
                mrsDfxBcRmNonBcRespCnt();
            }
        }
    }
    else    // 并发接收
    {
        HI_U32 ulRet = HI_ERR_SUCCESS;

        mrsDfxPrPlcRx();
        ulRet = mrsRmPlcPackageReceiveCsg(pstPlcFrame->pucPayload, pstPlcFrame->usPayloadLen);
        if (HI_ERR_SUCCESS != ulRet)
        {
            return HI_ERR_FAILURE;
        }
    }

    (hi_void)memset_s(&stMeter, sizeof(stMeter), 0, sizeof(stMeter));
    (hi_void)memcpy_s(stMeter.aucAddr, HI_METER_ADDR_LEN, pstXrFrame->aucSrcAddr, HI_METER_ADDR_LEN);
    stMeter.ucProtocol = MRS_PROTO645_VER_1997;
    stMeter.ucType = MRS_MAP_DEV_TYPE_METER;

    mrsMapAddMeter(&pstCcoCtx->stMapModule, pstPlcFrame->ucMac, &stMeter);

    return HI_ERR_SUCCESS;
}


HI_U32 mrsCcoCsgCmdSmResultProc(HI_PVOID pParam, HI_U16 usSize)
{
    HI_U32 ulRet = HI_ERR_SUCCESS;
    MRS_CCO_SRV_CTX_STRU *pstCcoCtx = mrsCcoGetContext();
    MRS_PLC_FRAME_DATA_STRU *pstPlcFrame = (MRS_PLC_FRAME_DATA_STRU *)pParam;
    MRS_CSG_PLC_FRAME_HEAD *pstFrameHead = (MRS_CSG_PLC_FRAME_HEAD *)pstPlcFrame->pucPayload;
    APP_CSG_CMD_SM_RESULT_UL_STRU *pstData = (APP_CSG_CMD_SM_RESULT_UL_STRU *)pstFrameHead->aucData;
    METER_NODE *pNode = HI_NULL;
    HI_U8 *pucPayload = HI_NULL;
    HI_U16 usPayloadLen = 0;
    HI_U8 i = 0;

    HI_UNREF_PARAM(usSize);

    if ((pstData->ucMeterNum > MRS_DFX_CLT_METER_MAX)
        || (pstFrameHead->usDataLength != sizeof(APP_CSG_CMD_SM_RESULT_UL_STRU) + sizeof(APP_CSG_METER_INFO) * pstData->ucMeterNum))
    {
        return HI_ERR_FAILURE;
    }

    mrsMapDeleteMac(&pstCcoCtx->stMapModule, pstPlcFrame->ucMac);
    mrsMapMarkMac(&pstCcoCtx->stMapModule, pstPlcFrame->ucMac, MRS_SEARCH_STATUS_IDLE);

    for (i = 0; i < pstData->ucMeterNum; i++)
    {
        MRS_DEVICE_INF_STRU stMeterInf;
        APP_CSG_METER_INFO *pInf = pstData->astMeterInf + i;

        // 加到映射表
        (hi_void)memset_s(&stMeterInf, sizeof(stMeterInf), 0, sizeof(stMeterInf));
        mrsMeterAddrPreProc(stMeterInf.aucAddr, pInf->aucMeterAddr, MRS_PROTO645_VER_1997);
        stMeterInf.ucProtocol = MRS_PROTO645_VER_1997;
        stMeterInf.ucType = MRS_MAP_DEV_TYPE_METER;
        mrsMapAddMeter(&pstCcoCtx->stMapModule, pstPlcFrame->ucMac, &stMeterInf);
    }

    ulRet = mrsCcoUpdateSearchResult(pstCcoCtx, pstPlcFrame->ucMac);
    if (HI_ERR_SUCCESS != ulRet)
    {
        return HI_ERR_SUCCESS;
    }

    usPayloadLen = sizeof(METER_NODE);
    usPayloadLen += sizeof(MRS_CMD_METERLIST_ITEM) * pstData->ucMeterNum;
    pucPayload = (HI_U8 *)mrsToolsMalloc(usPayloadLen);
    if (!pucPayload)
    {
        return HI_ERR_MALLOC_FAILUE;
    }


    (hi_void)memset_s(pucPayload, usPayloadLen, 0, usPayloadLen);
    pNode = (METER_NODE *)pucPayload;

    (hi_void)memcpy_s(pNode->mac, HI_PLC_MAC_ADDR_LEN, pstPlcFrame->ucMac, HI_PLC_MAC_ADDR_LEN);
    (hi_void)memcpy_s(pNode->asset, HI_METER_ADDR_LEN, MRS_BROADCAST_MAC, HI_METER_ADDR_LEN);
    (hi_void)memcpy_s(pNode->id, HI_PLC_MAC_ADDR_LEN, pNode->mac, HI_PLC_MAC_ADDR_LEN);
    mrsHexInvert(pNode->id, HI_PLC_MAC_ADDR_LEN);
    pNode->num = pstData->ucMeterNum;
    pNode->type = MRS_DEVICE_TYPE_COLLECTOR;

    for (i = 0; i < pstData->ucMeterNum; i++)
    {
        MRS_CMD_METERLIST_ITEM *pItem = (MRS_CMD_METERLIST_ITEM *)(pNode->item + sizeof(MRS_CMD_METERLIST_ITEM) * i);
        APP_CSG_METER_INFO *pInf = pstData->astMeterInf + i;

        (hi_void)memcpy_s(pItem->addr, HI_METER_ADDR_LEN, pInf->aucMeterAddr, HI_METER_ADDR_LEN);
        pItem->protocol = MRS_PROTO645_VER_1997;
        pItem->type = MRS_STA_TYPE_METER;
    }

    if (!mrsCcoSearchMacRepeatCheck(pstCcoCtx, pNode->mac))
    {
        mrsSrvEnQueue(&pstCcoCtx->stAutoSearchModel.stMeterQueue, (MRS_SRV_NODE *)pNode);
    }

    return ulRet;
}


HI_U32 mrsCcoCsgCmdTransFileProc(HI_PVOID pParam, HI_U16 usSize)
{
    return mrsCsgPlcCmdTransFileProc(pParam, usSize);
}

HI_U32 mrsCcoCsgCmdQueryInfoProc(HI_PVOID pParam, HI_U16 usSize)
{
    return mrsCsgPlcCmdQueryInfoProc(pParam, usSize);
}

HI_U32 mrsCcoCsgCmdSetEventFlagProc(HI_PVOID pParam, HI_U16 usSize)
{
    HI_UNREF_PARAM(pParam);
    HI_UNREF_PARAM(usSize);
    return HI_ERR_NOT_SUPPORT;
}


HI_U32 mrsCcoCsgCmdRebootProc(HI_PVOID pParam, HI_U16 usSize)
{
    HI_UNREF_PARAM(pParam);
    HI_UNREF_PARAM(usSize);
    return HI_ERR_NOT_SUPPORT;
}

HI_U32 mrsCcoCsgCmdBindProc(HI_PVOID pParam, HI_U16 usSize)
{
    MRS_PLC_FRAME_DATA_STRU *pstPlcFrame = (MRS_PLC_FRAME_DATA_STRU *)pParam;
    MRS_CCO_SRV_CTX_STRU *pstCcoCtx = mrsCcoGetContext();

    HI_UNREF_PARAM(usSize);

    HI_DIAG_LOG_BUF(MRS_FILE_LOG_BUF_1000, HI_DIAG_MT("[RCV BIND RESP] mac"), pstPlcFrame->ucMac, HI_PLC_MAC_ADDR_LEN);

    mrsCcoUpdateBindResult(pstCcoCtx, pstPlcFrame->ucMac);

    return HI_ERR_SUCCESS;
}

// CCO端收到事件主动上报
HI_U32 mrsCcoCsgReportEventProc(HI_PVOID pParam, HI_U16 usSize)
{
    HI_UNREF_PARAM(usSize);
    return mrsCcoEvtPlcFrameProc((MRS_PLC_FRAME_DATA_S *)pParam);
}

//CCO收到PLC灵敏度测试报文
HI_U32 mrsCcoCsgPlcTestProc(HI_PVOID pParam, HI_U16 usSize)
{
    return mrsCsgPlcTestProc(pParam, usSize);
}
#endif
HI_END_HEADER
