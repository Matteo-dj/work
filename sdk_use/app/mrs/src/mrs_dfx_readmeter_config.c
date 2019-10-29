//*****************************************************************************
//
//                  版权所有 (C), 2001-2015, 华为技术有限公司
//
//*****************************************************************************
//  文 件 名   : mrs_dfx_readmeter_config.c
//  版 本 号   : V1.0
//  作    者   : cuiate/00233580
//  生成日期   : 2015/03/26
//  功能描述   : 抄表超时配置统计信息
//  函数列表   :
//  修改历史   :
//  1.日    期 : 2015/03/26
//    作    者 : cuiate/00233580
//    修改内容 : 创建文件
//
//*****************************************************************************

#include "mrs_common.h"
#include "mrs_cmd_msg.h"
#include "mrs_fw_n.h"
#include "mrs_fw_log.h"
#include "mrs_fw_tools.h"
#include "mrs_srv_readmeter_config.h"
#include "mrs_dfx_readmeter_config.h"


#if defined(PRODUCT_CFG_PRODUCT_TYPE_CCO)
//*****************************************************************************
//*****************************************************************************

HI_PRV HI_U32 mrsDiagDfxMrRetryInf(HI_U16 usId, HI_PVOID pCmdParam, HI_U16 usCmdParamSize, HI_U32 ulOption);

HI_PRV DIAG_CMD_MRS_CCO_MR_TIMEOUT_DFX_IND_STRU *mrsDfxMrRetryGetCtx(HI_VOID);


HI_PRV DIAG_CMD_MRS_CCO_MR_TIMEOUT_DFX_IND_STRU *g_pDfxMrRetryStat = HI_NULL;



HI_U32 mrsDfxMrRetryModuleInit(HI_VOID)
{
    if (HI_NULL != g_pDfxMrRetryStat)
    {
        return HI_ERR_INITILIZATION_ALREADY;
    }

    g_pDfxMrRetryStat = (DIAG_CMD_MRS_CCO_MR_TIMEOUT_DFX_IND_STRU *)mrsToolsMalloc(sizeof(DIAG_CMD_MRS_CCO_MR_TIMEOUT_DFX_IND_STRU));
    if (HI_NULL == g_pDfxMrRetryStat)
    {
        return HI_ERR_MALLOC_FAILUE;
    }

    (hi_void)memset_s(g_pDfxMrRetryStat, sizeof(DIAG_CMD_MRS_CCO_MR_TIMEOUT_DFX_IND_STRU), 0, sizeof(DIAG_CMD_MRS_CCO_MR_TIMEOUT_DFX_IND_STRU));

    return HI_ERR_SUCCESS;
}


DIAG_CMD_MRS_CCO_MR_TIMEOUT_DFX_IND_STRU *mrsDfxMrRetryGetCtx(HI_VOID)
{
    return g_pDfxMrRetryStat;
}


HI_U32 mrsDfxMrRetryDiagCmdNotify(HI_U16 usId, HI_IN HI_PVOID pCmdParam, HI_U16 usCmdParamSize, HI_U32 ulOption)
{
    HI_SYS_QUEUE_MSG_S stMsg = {0};
    HI_PVOID pParam = HI_NULL;
    HI_U16 usParamSize = usCmdParamSize;
    HI_U32 ulRet = HI_ERR_SUCCESS;

    if (usParamSize > 0)
    {
        pParam = mrsToolsMalloc(usParamSize);
        if (!pParam)
        {
            return HI_ERR_MALLOC_FAILUE;
        }

        (hi_void)memcpy_s(pParam, usParamSize, pCmdParam, usParamSize);
    }
    

    stMsg.ulMsgId = EN_MRS_FW_MSG_MR_RETRY_DIAG_CMD;
    stMsg.ulParam[0] = (HI_U32)usId;
    stMsg.ulParam[1] = (HI_U32)pParam;
    stMsg.ulParam[2] = (HI_U32)usParamSize;
    stMsg.ulParam[3] = ulOption;

    ulRet = mrsSendMessage2Queue(&stMsg);
    if (HI_ERR_SUCCESS != ulRet)
    {
        mrsToolsFree(pParam);
    }

    return ulRet;
}


HI_U32 mrsDfxMrRetryDiagCmdProc(HI_SYS_QUEUE_MSG_S *pstMsg)
{
    HI_U16 usId = (HI_U16)pstMsg->ulParam[0];
    HI_PVOID pCmdParam = (HI_PVOID)pstMsg->ulParam[1];
    HI_U16 usCmdParamSize = (HI_U16)pstMsg->ulParam[2];
    HI_U32 ulOption = pstMsg->ulParam[3];
    HI_U32 ulRet = HI_ERR_SUCCESS;

    switch (usId)
    {
        case ID_DIAG_CMD_MRS_CCO_GET_MR_RETRY_CFG:
            ulRet = mrsDiagGetMrRetryCfg(usId, pCmdParam, usCmdParamSize, ulOption);
            break;

        case ID_DIAG_CMD_MRS_CCO_SET_MR_RETRY_CFG:
            ulRet = mrsDiagSetMrRetryCfg(usId, pCmdParam, usCmdParamSize, ulOption);
            break;

        case ID_DIAG_CMD_MRS_CCO_MR_RETRY_CFG_DFX_INF:
            ulRet = mrsDiagDfxMrRetryInf(usId, pCmdParam, usCmdParamSize, ulOption);
            break;

        default:
            ulRet = HI_ERR_NOT_FOUND;
            break;
    }

    mrsToolsFree(pCmdParam);

    return ulRet;
}


HI_U32 mrsDiagDfxMrRetryInf(HI_U16 usId, HI_PVOID pCmdParam, HI_U16 usCmdParamSize, HI_U32 ulOption)
{
    DIAG_CMD_MRS_CCO_MR_TIMEOUT_DFX_IND_STRU *pCtx = mrsDfxMrRetryGetCtx();

    HI_UNREF_PARAM(pCmdParam);
    HI_UNREF_PARAM(usCmdParamSize);

    HI_MDM_ReportPacket(usId, (HI_U8)ulOption, (HI_PBYTE)pCtx, (HI_U16)sizeof(DIAG_CMD_MRS_CCO_MR_TIMEOUT_DFX_IND_STRU), HI_FALSE);

    return HI_ERR_SUCCESS;
}


HI_VOID mrsDfxMrRetryModeChangeStat(HI_U32 ulReqMode, HI_U32 ulCurMode, HI_U32 ulRet, HI_BOOL bSimuChl)
{
    DIAG_CMD_MRS_CCO_MR_TIMEOUT_DFX_IND_STRU *pCtx = mrsDfxMrRetryGetCtx();
    DIAG_CMD_MRS_CCO_MR_TIMEOUT_DFX_ITEM_STRU *pItem = pCtx->astItem;

    (hi_void)memmove_s(pItem + 1, sizeof(DIAG_CMD_MRS_CCO_MR_TIMEOUT_DFX_ITEM_STRU) * (DIAG_CMD_MRS_CCO_MR_TIMEOUT_DFX_ITEM_MAX - 1),
        pItem, sizeof(DIAG_CMD_MRS_CCO_MR_TIMEOUT_DFX_ITEM_STRU) * (DIAG_CMD_MRS_CCO_MR_TIMEOUT_DFX_ITEM_MAX - 1));
    
    pItem->ucCurMode = (HI_U8)ulCurMode;
    pItem->ucReqMode = (HI_U8)ulReqMode;
    pItem->ucRet = (HI_U8)ulRet;
    pItem->ucSimuStatus = (HI_U8)bSimuChl;
    pItem->ulChangeTS = HI_MDM_GetMilliSeconds();
    pCtx->ulChangeTimes++;
}


//*****************************************************************************
//*****************************************************************************
#endif // defined(PRODUCT_CFG_PRODUCT_TYPE_CCO)


