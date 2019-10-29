//*****************************************************************************
//
//                  版权所有 (C), 2001-2015, 华为技术有限公司
//
//*****************************************************************************
//  文 件 名   : mrs_srv_readmeter_config.h
//  版 本 号   : V1.0
//  作    者   : cuiate/00233580
//  生成日期   : 2015/03/26
//  功能描述   : 抄表超时配置功能相关函数及变量定义
//  函数列表   :
//  修改历史   :
//  1.日    期 : 2015/03/26
//    作    者 : cuiate/00233580
//    修改内容 : 创建文件
//
//*****************************************************************************

#include "mrs_common.h"
#include "mrs_cmd_msg.h"
#include "mrs_srv_common.h"
#include "app_nv.h"
#include "dfx_app.h"
#include "mrs_fw_n.h"
#include "mrs_fw_log.h"
#include "mrs_fw_tools.h"
#include "mrs_fw_proto376_2_echo.h"
#include "mrs_srv_cco_macmap.h"
#include "mrs_srv_cco_broadcast.h"
#include "mrs_srv_list.h"
#include "mrs_srv_cco_event.h"
#include "mrs_srv_cco_remote_upg.h"
#include "mrs_srv_cco.h"
#include "mrs_srv_readmeter_manage.h"
#include "mrs_srv_readmeter_config.h"
#include "mrs_dfx_readmeter_config.h"


#if defined(PRODUCT_CFG_PRODUCT_TYPE_CCO)
//*****************************************************************************
//*****************************************************************************


HI_PRV HI_U32 mrsRmCfgParamInit(NV_MRS_RETRY_CFG_S *pstCfg);

HI_PRV HI_VOID mrsRmCfgMrParamInit(PLC_OVERTIME_INF_STRU *pstParam, NV_MRS_RETRY_CFG_S *pstParamNv, HI_S32 iCtrlCmdDefault);

HI_PRV HI_VOID mrsRmCfgMrParamResume(HI_VOID);


HI_PRV MRS_RM_CONFIG_CONTEXT_STRU g_stRmCfgCtx = {HI_FALSE, HI_FALSE, {0, 0}, DIAG_APP_MRS_TIMEOUT_MODE_DEFAULT};


HI_U32 mrsRmCfgModuleInit(HI_VOID)
{
    if (HI_TRUE == g_stRmCfgCtx.bInited)
    {
        return HI_ERR_SUCCESS;
    }

    g_stRmCfgCtx.bInited = HI_TRUE;

    return mrsRmCfgDefaultNvInit();
}


MRS_RM_CONFIG_CONTEXT_STRU *mrsRmCfgGetModuleCtx(HI_VOID)
{
    return &g_stRmCfgCtx;
}


HI_VOID mrsRmCfgSetSimuChlStatus(HI_BOOL bEnable)
{
    MRS_RM_CONFIG_CONTEXT_STRU *pCtx = mrsRmCfgGetModuleCtx();

    if (pCtx->bSimuChl == HI_TRUE)
    {
        mrsRmCfgMrParamResume();
    }

    pCtx->bSimuChl = bEnable;
}


HI_VOID mrsRmCfgMrParamResume(HI_VOID)
{
    HI_SYS_QUEUE_MSG_S stMsg = {0};
    DIAG_CMD_MRS_CCO_SET_MR_TIMEOUT_REQ_STRU *pReq = HI_NULL;
    HI_U32 ulRet = HI_ERR_SUCCESS;

    pReq = (DIAG_CMD_MRS_CCO_SET_MR_TIMEOUT_REQ_STRU *)mrsToolsMalloc(sizeof(DIAG_CMD_MRS_CCO_SET_MR_TIMEOUT_REQ_STRU));
    if (!pReq)
    {
        return;
    }

    (hi_void)memset_s(pReq, sizeof(DIAG_CMD_MRS_CCO_SET_MR_TIMEOUT_REQ_STRU), 0, sizeof(DIAG_CMD_MRS_CCO_SET_MR_TIMEOUT_REQ_STRU));
    pReq->ulMode = DIAG_APP_MRS_TIMEOUT_MODE_DEFAULT;

    stMsg.ulMsgId = EN_MRS_FW_MSG_MR_RETRY_DIAG_CMD;
    stMsg.ulParam[0] = (HI_U32)ID_DIAG_CMD_MRS_CCO_SET_MR_RETRY_CFG;
    stMsg.ulParam[1] = (HI_U32)pReq;
    stMsg.ulParam[2] = (HI_U32)sizeof(DIAG_CMD_MRS_CCO_SET_MR_TIMEOUT_REQ_STRU);
    stMsg.ulParam[3] = 0;

    ulRet = mrsSendMessage2Queue(&stMsg);
    if (HI_ERR_SUCCESS != ulRet)
    {
        mrsToolsFree(pReq);
    }
}


HI_U32 mrsRmCfgFastNvInit(HI_VOID)
{
    MRS_RM_CONFIG_CONTEXT_STRU *pCtx = mrsRmCfgGetModuleCtx();
    NV_APP_PLC_OVERTIME_INF_STRU stPlcOverTimeInf;
    HI_U32 ulRet = HI_ERR_SUCCESS;

    if (DIAG_APP_MRS_TIMEOUT_MODE_FAST != pCtx->ulMode)
    {
        return HI_ERR_SUCCESS;
    }

    (hi_void)memset_s(&stPlcOverTimeInf, sizeof(stPlcOverTimeInf), 0, sizeof(stPlcOverTimeInf));
    ulRet = HI_MDM_NV_Read(ID_NV_APP_PLC_OVERTIME_INFO, &stPlcOverTimeInf, sizeof(stPlcOverTimeInf));
    if (HI_ERR_SUCCESS == ulRet)
    {
        ulRet = mrsRmCfgParamInit((NV_MRS_RETRY_CFG_S *)stPlcOverTimeInf.stPlcOverTimeParam);
    }
    else
    {
        ulRet = mrsRmCfgParamInit(HI_NULL);
    }

    return ulRet;
}


HI_U32 mrsRmCfgDefaultNvInit(HI_VOID)
{
    MRS_RM_CONFIG_CONTEXT_STRU *pCtx = mrsRmCfgGetModuleCtx();
    NV_MRS_RM_CFG_STRU stRmCfg;
    HI_U32 ulRet = HI_ERR_SUCCESS;

    if (DIAG_APP_MRS_TIMEOUT_MODE_DEFAULT != pCtx->ulMode)
    {
        return HI_ERR_SUCCESS;
    }

    (hi_void)memset_s(&stRmCfg, sizeof(stRmCfg), 0, sizeof(stRmCfg));
    ulRet = HI_MDM_NV_Read(ID_NV_MRS_RM_CFG, &stRmCfg, sizeof(stRmCfg));
    if (HI_ERR_SUCCESS == ulRet)
    {
        mrsRmCfgParamInit((NV_MRS_RETRY_CFG_S *)stRmCfg.stRetryCfg);
        mrsRmCfgParallelInit((NV_MRS_PARALLEL_CFG_S *)stRmCfg.stParallelCfg);
    }
    else
    {
        mrsRmCfgParamInit(HI_NULL);
        mrsRmCfgParallelInit(HI_NULL);
    }

    return HI_ERR_SUCCESS;
}


HI_U32 mrsRmCfgUserDefineInit(NV_MRS_RETRY_CFG_S *pstCfg)
{
    return mrsRmCfgParamInit(pstCfg);
}


HI_U32 mrsRmCfgParamInit(NV_MRS_RETRY_CFG_S *pstCfg)
{
    PLC_OVERTIME_INF_STRU *pInf = HI_NULL;
    NV_MRS_RETRY_CFG_S *pXRInf = HI_NULL;
    NV_MRS_RETRY_CFG_S *pLRInf = HI_NULL;
#if defined(PRODUCT_CFG_PRODUCT_TYPE_CSG_GD)
    PLC_OVERTIME_INF_STRU *pCsgInf = mrsGetCsgPlcOverTimeInf();
#endif

    if (pstCfg)
    {
        pXRInf = &pstCfg[MRS_RM_XR];
        pLRInf = &pstCfg[MRS_RM_LR];
    }

    pInf = mrsGetPlcOverTimeInfAfn13();
    if (pInf)
    {
        mrsRmCfgMrParamInit(pInf, pXRInf, MRS_RM_CTRL_CMD_DELAY_DEFAULT);
    }

#if defined(PRODUCT_CFG_PRODUCT_TYPE_CSG_GD)	
    if (pCsgInf)
	{
        mrsRmCfgMrParamInit(pCsgInf, pXRInf, MRS_RM_CTRL_CMD_DELAY_DEFAULT);
	}
#endif

    pInf = mrsGetPlcOverTimeInfAfn14();
    if (pInf)
    {
        mrsRmCfgMrParamInit(pInf, pLRInf, 0);
    }

    // 并发抄表参数初始化
    mrsRmCfgInit(pstCfg);

    return HI_ERR_SUCCESS;
}


HI_VOID mrsRmCfgMrParamInit(PLC_OVERTIME_INF_STRU *pstParam, NV_MRS_RETRY_CFG_S *pstParamNv, HI_S32 iCtrlCmdDelay)
{
    HI_S32 iDelayDefault = iCtrlCmdDelay;
    
    if (!pstParam)
    {
        return;
    }

    MRS_DATA_VALID_CHECK(iDelayDefault, MRS_RM_CTRL_CMD_DELAY_MAX, MRS_RM_CTRL_CMD_DELAY_DEFAULT, MRS_RM_CTRL_CMD_DELAY_MIN);

    if (HI_NULL == pstParamNv)
    {
        pstParam->MaxTryNvS      = MRS_RM_PLC_RETRY_CNT_DEFAULT;
        pstParam->MaxTryNvB      = MRS_RM_PLC_BC_RETRY_CNT_DEFAULT;
        pstParam->LayTimeOutNv   = MRS_RM_PLC_LAYER_DELAY_DEFAULT;
        pstParam->MeterTimeOutNv = MRS_RM_METER_OVERTIME_DEFAULT;
        pstParam->RevisoryNv     = MRS_RM_PLC_REVISORY_DEFAULT;
        pstParam->CtrlCmdDelayNv = iDelayDefault;
        pstParam->CltIDelayNv    = MRS_RM_CLTI_DELAY_DEFAULT;
        pstParam->MaxTimeout     = MRS_RM_MAX_SERVICE_TIME_DEFAULT;
        pstParam->MaxTryLoop     = MRS_RM_PLC_RETRY_LOOP_DEFAULT;
        pstParam->MeterFrameInterval = MRS_RM_METER_FRAME_INTERVAL_DEFAULT;
        return;
    }

    pstParam->MaxTryNvS = pstParamNv->PlcMaxRetryNvS;
    pstParam->MaxTryNvB = pstParamNv->PlcMaxRetryNvB;
    pstParam->LayTimeOutNv = pstParamNv->PlcLayerDelayNv;
    pstParam->MeterTimeOutNv = pstParamNv->MeterOverTimeNv;
    pstParam->RevisoryNv = pstParamNv->PlcRevisoryNv;
    pstParam->CtrlCmdDelayNv = pstParamNv->CtrlCmdDelayNv;
    pstParam->CltIDelayNv = pstParamNv->CltIDelayNv;
    pstParam->MaxTimeout = pstParamNv->MaxServiceTime;
    pstParam->MaxTryLoop = pstParamNv->PlcMaxRetryLoop;
    pstParam->MeterFrameInterval = (HI_U16)pstParamNv->MeterFrameInterval;
    
    MRS_DATA_VALID_CHECK(pstParam->MaxTryNvS, MRS_RM_PLC_RETRY_CNT_MAX, MRS_RM_PLC_RETRY_CNT_DEFAULT, MRS_RM_PLC_RETRY_CNT_MIN);
    MRS_DATA_VALID_CHECK(pstParam->MaxTryNvB, MRS_RM_PLC_BC_RETRY_CNT_MAX, MRS_RM_PLC_BC_RETRY_CNT_DEFAULT, MRS_RM_PLC_BC_RETRY_CNT_MIN);
    MRS_DATA_VALID_CHECK(pstParam->LayTimeOutNv, MRS_RM_PLC_LAYER_DELAY_MAX, MRS_RM_PLC_LAYER_DELAY_DEFAULT, MRS_RM_PLC_LAYER_DELAY_MIN);
    MRS_DATA_VALID_CHECK(pstParam->MeterTimeOutNv, MRS_RM_METER_OVERTIME_MAX, MRS_RM_METER_OVERTIME_DEFAULT, MRS_RM_METER_OVERTIME_MIN);
    MRS_DATA_VALID_CHECK(pstParam->RevisoryNv, MRS_RM_PLC_REVISORY_MAX, MRS_RM_PLC_REVISORY_DEFAULT, MRS_RM_PLC_REVISORY_MIN);
    MRS_DATA_VALID_CHECK(pstParam->CtrlCmdDelayNv, MRS_RM_CTRL_CMD_DELAY_MAX, iDelayDefault, MRS_RM_CTRL_CMD_DELAY_MIN);
    MRS_DATA_VALID_CHECK(pstParam->MaxTryLoop, MRS_RM_PLC_RETRY_LOOP_MAX, MRS_RM_PLC_RETRY_LOOP_DEFAULT, MRS_RM_PLC_RETRY_LOOP_MIN);

    if (pstParam->MaxTimeout < MRS_RM_MAX_SERVICE_TIME_MIN)
    {
        pstParam->MaxTimeout = MRS_RM_MAX_SERVICE_TIME_DEFAULT;
    }
}


HI_U32 mrsDiagGetMrRetryCfg(HI_U16 usId, HI_PVOID pCmdParam, HI_U16 usCmdParamSize, HI_U32 ulOption)
{
    DIAG_CMD_MRS_CCO_GET_MR_TIMEOUT_REQ_STRU *pReq = (DIAG_CMD_MRS_CCO_GET_MR_TIMEOUT_REQ_STRU *)pCmdParam;
    HI_U16 usNvId = 0;

    HI_UNREF_PARAM(usCmdParamSize);

    if (!pReq)
    {
        return HI_ERR_INVALID_PARAMETER;
    }

    switch (pReq->ulReqMode)
    {
        case DIAG_APP_MRS_TIMEOUT_MODE_DEFAULT:
            usNvId = ID_NV_MRS_RM_CFG;
            break;

        case DIAG_APP_MRS_TIMEOUT_MODE_FAST:
            usNvId = ID_NV_APP_PLC_OVERTIME_INFO;
            break;

        default:
            return HI_ERR_NOT_SUPPORT;
    }

    {
        NV_APP_PLC_OVERTIME_INF_STRU stPlcInf;
        DIAG_CMD_MRS_CCO_GET_MR_TIMEOUT_IND_STRU *pInd;
        HI_U32 ulRet = HI_ERR_SUCCESS;

        pInd = (DIAG_CMD_MRS_CCO_GET_MR_TIMEOUT_IND_STRU *)mrsToolsMalloc(sizeof(DIAG_CMD_MRS_CCO_GET_MR_TIMEOUT_IND_STRU));
        if (!pInd)
        {
            return HI_ERR_MALLOC_FAILUE;
        }

        (hi_void)memset_s(&stPlcInf, sizeof(stPlcInf), 0, sizeof(stPlcInf));
        (hi_void)memset_s(pInd, sizeof(DIAG_CMD_MRS_CCO_GET_MR_TIMEOUT_IND_STRU), 0, sizeof(DIAG_CMD_MRS_CCO_GET_MR_TIMEOUT_IND_STRU));

        ulRet = HI_MDM_NV_Read(usNvId, &stPlcInf, sizeof(stPlcInf));
        if (HI_ERR_SUCCESS == ulRet)
        {
            mrsRmRetryCfgInit(&(pInd->astRetryCfg[MRS_RM_XR]), &(stPlcInf.stPlcOverTimeParam[MRS_RM_XR]));
            mrsRmRetryCfgInit(&(pInd->astRetryCfg[MRS_RM_LR]), &(stPlcInf.stPlcOverTimeParam[MRS_RM_LR]));
            mrsRmRetryCfgInit(&(pInd->astRetryCfg[MRS_RM_PR]), &(stPlcInf.stPlcOverTimeParam[MRS_RM_PR]));
        }
        else
        {
            mrsRmRetryCfgInit(&(pInd->astRetryCfg[MRS_RM_XR]), HI_NULL);
            mrsRmRetryCfgInit(&(pInd->astRetryCfg[MRS_RM_LR]), HI_NULL);
            mrsRmRetryCfgInit(&(pInd->astRetryCfg[MRS_RM_PR]), HI_NULL);
        }

        pInd->ulSeq = pReq->ulSeq;
        pInd->ulReqMode = pReq->ulReqMode;
        pInd->ulCurMode = mrsRmCfgGetModuleCtx()->ulMode;

        HI_MDM_ReportPacket(usId, (HI_U8)ulOption, (HI_PBYTE)pInd, (HI_U16)sizeof(DIAG_CMD_MRS_CCO_GET_MR_TIMEOUT_IND_STRU), HI_FALSE);

        mrsToolsFree(pInd);
    }

    return HI_ERR_SUCCESS;
}


HI_U32 mrsDiagSetMrRetryCfg(HI_U16 usId, HI_PVOID pCmdParam, HI_U16 usCmdParamSize, HI_U32 ulOption)
{
    DIAG_CMD_MRS_CCO_SET_MR_TIMEOUT_REQ_STRU *pReq = (DIAG_CMD_MRS_CCO_SET_MR_TIMEOUT_REQ_STRU *)pCmdParam;
    MRS_RM_CONFIG_CONTEXT_STRU *pCtx = mrsRmCfgGetModuleCtx();
    HI_U32 ulCurMode = pCtx->ulMode;
    HI_U32 ulRet = HI_ERR_SUCCESS;

    HI_UNREF_PARAM(usCmdParamSize);

    if (!pReq)
    {
        return HI_ERR_INVALID_PARAMETER;
    }

    switch (pReq->ulMode)
    {
        case DIAG_APP_MRS_TIMEOUT_MODE_DEFAULT:
            if (pCtx->ulMode != pReq->ulMode)
            {
                pCtx->ulMode = DIAG_APP_MRS_TIMEOUT_MODE_DEFAULT;
                mrsRmCfgDefaultNvInit();
            }
            break;

        case DIAG_APP_MRS_TIMEOUT_MODE_FAST:
            if (HI_TRUE != pCtx->bSimuChl)
            {
                ulRet = HI_ERR_NOT_SUPPORT;
                break;
            }
            pCtx->ulMode = DIAG_APP_MRS_TIMEOUT_MODE_FAST;
            mrsRmCfgFastNvInit();
            break;

        case DIAG_APP_MRS_TIMEOUT_MODE_USERDEFINE:
            if (HI_TRUE != pCtx->bSimuChl)
            {
                ulRet = HI_ERR_NOT_SUPPORT;
                break;
            }
            pCtx->ulMode = DIAG_APP_MRS_TIMEOUT_MODE_USERDEFINE;
            mrsRmCfgUserDefineInit(pReq->astRetryCfg);
            break;

        default:
            ulRet = HI_ERR_NOT_SUPPORT;
            break;
    }

    {
        DIAG_CMD_MRS_CCO_SET_MR_TIMEOUT_IND_STRU stInd;
        stInd.ulSeq = pReq->ulSeq;
        stInd.ulRet = ulRet;

        HI_MDM_ReportPacket(usId, (HI_U8)ulOption, (HI_PBYTE)&stInd, (HI_U16)sizeof(DIAG_CMD_MRS_CCO_SET_MR_TIMEOUT_IND_STRU), HI_FALSE);
    }

    mrsDfxMrRetryModeChangeStat(pReq->ulMode, ulCurMode, ulRet, pCtx->bSimuChl);

    return HI_ERR_SUCCESS;
}


//*****************************************************************************
//*****************************************************************************
#endif // defined(PRODUCT_CFG_PRODUCT_TYPE_CCO)

