//*****************************************************************************
//
//                  版权所有 (C), 2001-2014, 华为技术有限公司
//
//*****************************************************************************
//  文 件 名   : mrs_srv_csg_plc.c
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
#include "mrs_srv_csg_plc.h"


HI_START_HEADER
#if ((defined(PRODUCT_CFG_PRODUCT_TYPE_CCO) || defined(PRODUCT_CFG_PRODUCT_TYPE_STA)) && defined(PRODUCT_CFG_PRODUCT_TYPE_CSG_GD))

HI_PRV MRS_CSG_PLC_FUNC mrsCsgFindPlcProcFunc(MRS_CSG_PLC_FRAME_HEAD *pstPlcHead);
HI_PRV MRS_CSG_PLC_CTX_STRU g_stCsgPlcCtx = {0, HI_NULL};


MRS_CSG_PLC_CTX_STRU *mrsCsgPlcModuleCtx(HI_VOID)
{
    return &g_stCsgPlcCtx;
}


HI_VOID mrsCsgPlcRegProcTblList(HI_U32 ulFuncNum, MRS_CSG_PLC_FUNC_TBL_STRU *pstTblList)
{
    MRS_CSG_PLC_CTX_STRU *pstCtx = mrsCsgPlcModuleCtx();
    pstCtx->ulFuncNum = ulFuncNum;
    pstCtx->pstTblList = pstTblList;
}


MRS_CSG_PLC_FUNC mrsCsgFindPlcProcFunc(MRS_CSG_PLC_FRAME_HEAD *pstPlcHead)
{
    MRS_CSG_PLC_CTX_STRU *pstCtx = mrsCsgPlcModuleCtx();
    HI_U32 i = 0;

    for (i = 0; i < pstCtx->ulFuncNum; i++)
    {
        if ((pstPlcHead->usFrameType == pstCtx->pstTblList[i].ucType)
            && (pstPlcHead->ucSrvId == pstCtx->pstTblList[i].ucSrvId))
        {
            return pstCtx->pstTblList[i].pfnFunc;
        }
    }

    return HI_NULL;
}

/*lint -e547*/
#if defined(PRODUCT_CFG_PRODUCT_TYPE_STA)
#define MRS_CSG_PLC_CTRL_DIR    MRS_CSG_PLC_CTRL_DIR_DL
#endif
#if defined(PRODUCT_CFG_PRODUCT_TYPE_CCO)
#define MRS_CSG_PLC_CTRL_DIR    MRS_CSG_PLC_CTRL_DIR_UL
#endif
/*lint +e547*/

HI_BOOL mrsCsgCheckPowerFailureFrame(MRS_CSG_PLC_FRAME_HEAD *pstFrameHead)
{
    if((MRS_CSG_PLC_CTRL_DIR_UL == pstFrameHead->usDir)
        && (MRS_EVENT_FUN_REPORT_MODUL == pstFrameHead->aucData[0]))
    {
        return HI_TRUE;
    }

    return HI_FALSE;
}
HI_U32 mrsCsgPlcFrameProc(HI_PVOID pReqPacket, HI_U16 usPacketSize)
{
    MRS_PLC_FRAME_DATA_STRU *pstPlcFrame = (MRS_PLC_FRAME_DATA_STRU *)pReqPacket;
    MRS_CSG_PLC_FRAME_HEAD *pstFrameHead = HI_NULL;
    MRS_CSG_PLC_FUNC pfnFunc = HI_NULL;

    if (!pstPlcFrame || !pstPlcFrame->pucPayload)
    {
        return HI_ERR_FAILURE;
    }

    pstFrameHead = (MRS_CSG_PLC_FRAME_HEAD *)pstPlcFrame->pucPayload;

    if ((MRS_CSG_PLC_CTRL_DIR != pstFrameHead->usDir)
        && mrsCsgCheckPowerFailureFrame(pstFrameHead) == HI_FALSE)
    {
        return HI_ERR_FAILURE;
    }

    pfnFunc = mrsCsgFindPlcProcFunc(pstFrameHead);
    if (!pfnFunc)
    {
        return HI_ERR_NOT_FOUND;
    }

    MRS_BufLog("[CSG-PLC][  Rx]: ", pstPlcFrame->pucPayload, pstPlcFrame->usPayloadLen);

    return pfnFunc(pReqPacket, usPacketSize);
}


HI_U32 mrsCsgPlcCmdTransFileProc(HI_PVOID pParam, HI_U16 usSize)
{
    return HI_MDM_UPG_MsgSend(HI_MDM_UPG_TRANS_FILE, pParam, usSize);
}

HI_U32 mrsCsgPlcCmdQueryInfoProc(HI_PVOID pParam, HI_U16 usSize)
{
    return HI_MDM_UPG_MsgSend(HI_MDM_UPG_QUERY_INFO, pParam, usSize);
}

HI_U32 mrsCsgPlcAckFrame(HI_U8 *pucMacAddr, HI_U16 usSeq, HI_BOOL bDlUl)
{
    MRS_PLC_FRAME_DATA_S stPlcFrame;
    MRS_CSG_PLC_FRAME_HEAD *pstFrameHead = HI_NULL;

    pstFrameHead = (MRS_CSG_PLC_FRAME_HEAD *)mrsToolsMalloc(sizeof(MRS_CSG_PLC_FRAME_HEAD));
    if (!pstFrameHead)
    {
        return HI_ERR_MALLOC_FAILUE;
    }

    (hi_void)memset_s(pstFrameHead, sizeof(MRS_CSG_PLC_FRAME_HEAD), 0, sizeof(MRS_CSG_PLC_FRAME_HEAD));
    pstFrameHead->usFrameType = MRS_CSG_PLC_CTRL_TYPE_ACK;
    pstFrameHead->usExtendBit = MRS_CSG_PLC_CTRL_EXTEND_BIT_N;
    pstFrameHead->usRespBit = MRS_CSG_PLC_CTRL_RESP_BIT_N;
    pstFrameHead->usPrm = MRS_CSG_PLC_CTRL_PRM_SLAVE;
    pstFrameHead->usDir = bDlUl ? MRS_CSG_PLC_CTRL_DIR_DL : MRS_CSG_PLC_CTRL_DIR_UL;
    pstFrameHead->ucSrvId = MRS_CSG_PLC_SRV_ID_ACK_CONFIRM;
    pstFrameHead->ucVersion = MRS_CSG_PLC_PROTO_VERSION;
    pstFrameHead->usSeq = usSeq;
    pstFrameHead->usDataLength = 0;

    (hi_void)memset_s(&stPlcFrame, sizeof(stPlcFrame), 0, sizeof(stPlcFrame));
    stPlcFrame.usId = ID_MRS_CMD_CSG_DATA_TRANSMIT_PLC;
    stPlcFrame.pucPayload = (HI_U8 *)pstFrameHead;
    stPlcFrame.usPayloadLen = sizeof(MRS_CSG_PLC_FRAME_HEAD);
    (hi_void)memcpy_s(stPlcFrame.ucMac, HI_PLC_MAC_ADDR_LEN, pucMacAddr, HI_PLC_MAC_ADDR_LEN);

    MRS_SendPlcFrame(&stPlcFrame);
    mrsToolsFree(pstFrameHead);

    return HI_ERR_SUCCESS;
}


HI_U32 mrsCsgPlcDenyFrame(HI_U8 *pucMacAddr, HI_U16 usSeq, HI_BOOL bDlUl, HI_U8 ucDeny)
{
    MRS_PLC_FRAME_DATA_S stPlcFrame;
    MRS_CSG_PLC_FRAME_HEAD *pstFrameHead = HI_NULL;

    pstFrameHead = (MRS_CSG_PLC_FRAME_HEAD *)mrsToolsMalloc(sizeof(MRS_CSG_PLC_FRAME_HEAD) + sizeof(HI_U8));
    if (!pstFrameHead)
    {
        return HI_ERR_MALLOC_FAILUE;
    }

    (hi_void)memset_s(pstFrameHead, sizeof(MRS_CSG_PLC_FRAME_HEAD) + sizeof(HI_U8), 0,sizeof(MRS_CSG_PLC_FRAME_HEAD) + sizeof(HI_U8));
    pstFrameHead->usFrameType = MRS_CSG_PLC_CTRL_TYPE_ACK;
    pstFrameHead->usExtendBit = MRS_CSG_PLC_CTRL_EXTEND_BIT_N;
    pstFrameHead->usRespBit = MRS_CSG_PLC_CTRL_RESP_BIT_N;
    pstFrameHead->usPrm = MRS_CSG_PLC_CTRL_PRM_SLAVE;
    pstFrameHead->usDir = bDlUl ? MRS_CSG_PLC_CTRL_DIR_DL : MRS_CSG_PLC_CTRL_DIR_UL;
    pstFrameHead->ucSrvId = MRS_CSG_PLC_SRV_ID_ACK_DENY;
    pstFrameHead->ucVersion = MRS_CSG_PLC_PROTO_VERSION;
    pstFrameHead->usSeq = usSeq;
    pstFrameHead->usDataLength = 0;
    pstFrameHead->aucData[0] = ucDeny;

    (hi_void)memset_s(&stPlcFrame, sizeof(stPlcFrame), 0, sizeof(stPlcFrame));
    stPlcFrame.usId = ID_MRS_CMD_CSG_DATA_TRANSMIT_PLC;
    stPlcFrame.pucPayload = (HI_U8 *)pstFrameHead;
    stPlcFrame.usPayloadLen = sizeof(MRS_CSG_PLC_FRAME_HEAD);
    (hi_void)memcpy_s(stPlcFrame.ucMac, HI_PLC_MAC_ADDR_LEN, pucMacAddr, HI_PLC_MAC_ADDR_LEN);

    MRS_SendPlcFrame(&stPlcFrame);
    mrsToolsFree(pstFrameHead);

    return HI_ERR_SUCCESS;
}


#endif
HI_END_HEADER
