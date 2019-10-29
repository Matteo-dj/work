//*****************************************************************************
//
//                  版权所有 (C), 1998-2011, 华为技术有限公司
//
//*****************************************************************************
//  文 件 名   : mrs_srv_cco_remote_upg.c
//  版 本 号   : V1.0 
//  作    者   : fengxiaomin/f00209182
//  生成日期   : 2014-1-9
//  功能描述   : 远程升级模块函数及接口定义
//               
//  函数列表   : TODO: ...
//  修改历史   : 
//  1.日    期 : 2014-1-9
//    作    者 : fengxiaomin/f00209182 
//    修改内容 : 创建文件 
//
//*****************************************************************************

/* BEGIN: Added by fengxiaomin/00209182, 2014/1/9   问题单号号:DTS2014012607684 */
#include "mrs_common.h"
#include "mrs_cmd_msg.h"
#include "mrs_fw_proto645.h"
#include "mrs_fw_proto376_2.h"
#include "mrs_fw_proto376_2_echo.h"
#include "mrs_fw_proto376_2_frame.h"
#include "mrs_fw_tools.h"
#include "mrs_fw_log.h"
#include "mrs_srv_common.h"
#include "mrs_srv_res.h"
#include "app_nv.h"
#include "hi_mdm.h"
#include "mrs_srv_cco_macmap.h"
#include "mrs_srv_parallel_cco.h"
#include "mrs_srv_cco_remote_upg.h"
#include "mrs_srv_cco.h"
#include "mrs_dfx.h"
#include "hi_mdm.h"
#include "mrs_srv_cco_searchmeter.h"
#include "mrs_dfx_cco_remote_upg.h"
#include "mrs_fw_n.h"
#include "mrs_fw_nv.h"

#if defined(PRODUCT_CFG_PRODUCT_TYPE_CCO)
MRS_REMOTE_UPG_MODULE g_stMrsUpgCtx;

HI_U32 mrsUpgStartMsgRx(HI_MDM_UPG_START_IND_S * pstInd);

MRS_REMOTE_UPG_MODULE* mrsGetRemoteUpgCtx(HI_VOID)
{
    return &g_stMrsUpgCtx;
}

HI_U32 mrsCcoRemoteUpgInit(HI_VOID)
{
    (hi_void)memset_s(&g_stMrsUpgCtx, sizeof(MRS_REMOTE_UPG_MODULE), 0, sizeof(MRS_REMOTE_UPG_MODULE));
    g_stMrsUpgCtx.ucUpgPeriod = MRS_CCO_UPGRADE_IDLE_PERIOD;
    mrsNvRemoteUpgInfCfg(ID_NV_MRS_UPG_PARAM_INFO);
    
    HI_MDM_RegisterUpgMsgNotify(HI_MDM_UPG_START, mrsRemoteUpgMsgRcv);
    HI_MDM_RegisterUpgMsgNotify(HI_MDM_UPG_STOP, mrsRemoteUpgMsgRcv);
    HI_MDM_RegisterUpgMsgNotify(HI_MDM_UPG_STATUS_BS_IND, mrsRemoteUpgMsgRcv);
    
    return HI_ERR_SUCCESS;
}

HI_VOID mrsPowerOnGetUpgStatus(HI_VOID)
{
    MRS_REMOTE_UPG_MODULE *pstMrsUpgCtx = mrsGetRemoteUpgCtx();

    mrsStartQueryUpgStatus(HI_TRUE, pstMrsUpgCtx);

    return;
}


// MRS接收MDM消息的处理入口
HI_U32 mrsRemoteUpgMsgRcv(HI_U32 ulId, HI_PBYTE pucPacket, HI_U16 usPacketSize)
{
    HI_U32 ret = HI_ERR_SUCCESS;
    HI_SYS_QUEUE_MSG_S stMsg = {0};
    HI_PBYTE pucInd = HI_NULL;
    
    if ((pucPacket == HI_NULL) || (usPacketSize == 0))
    {
        return HI_ERR_INVALID_PARAMETER;
    }
    
    if (!mrsIsMdmUpgIdValid(ulId))
    {
        return HI_ERR_INVALID_ID;
    }
    
    mrsDfxRxMsgTotalNum();    
    (hi_void)memset_s(&stMsg, sizeof(stMsg), 0, sizeof(stMsg));
    
    pucInd = (HI_PBYTE)mrsToolsMalloc(usPacketSize);
    if (pucInd == HI_NULL)
    {
        return HI_ERR_MALLOC_FAILUE;
    }
    
    (hi_void)memset_s(pucInd, usPacketSize, 0, usPacketSize);
    (hi_void)memcpy_s(pucInd, usPacketSize, pucPacket, usPacketSize);
    
    stMsg.ulMsgId = EN_MRS_FW_MSG_MDM_INPUT;
    
    stMsg.ulParam[0] = ulId;
    stMsg.ulParam[1] = (HI_U32)pucInd;
    stMsg.ulParam[2] = usPacketSize;
    
    ret = mrsSendMessage2Queue(&stMsg);
    if(ret != HI_ERR_SUCCESS)
    {
        mrsDfxSendMsgFail(EN_MRS_FW_MSG_MDM_INPUT, (HI_U16)ret);
        mrsToolsFree(pucInd);
    }
    else
    {
        mrsDfxTxMrsMsgNum();
    }

    return ret;
}

HI_BOOL mrsIsMdmUpgIdValid(HI_U32 ulMdmMsgId)
{
    switch (ulMdmMsgId)
    {
        case HI_MDM_UPG_STATUS_BS_IND:
        case HI_MDM_UPG_START:
        case HI_MDM_UPG_STOP:
            break;

        default:
            mrsDfxRxMsgIdErrNum();
            return HI_FALSE;
    }

    return HI_TRUE;
}

HI_U32 mrsRemoteUpgMsgProc(HI_SYS_QUEUE_MSG_S* pstMsg)
{
    if (pstMsg == HI_NULL)
    {
        HI_DIAG_LOG_MSG_E0(__LINE__, HI_DIAG_MT("MSG RX NULL"));
        return HI_ERR_FAILURE;
    }
        
    if(pstMsg->ulParam[0] == HI_MDM_UPG_STATUS_BS_IND)
    {
        HI_MDM_UPG_STATUS_BS_IND_S *pstInd = (HI_MDM_UPG_STATUS_BS_IND_S*)pstMsg->ulParam[1];

        mrsUpgStatusMsgRx(pstInd);
    }
    else if(pstMsg->ulParam[0] == HI_MDM_UPG_STOP)
    {
        HI_MDM_UPG_STOP_IND_S* pstInd = (HI_MDM_UPG_STOP_IND_S*)pstMsg->ulParam[1];
        
        mrsUpgStopMsgRx(pstInd);
    }
    else if (pstMsg->ulParam[0] == HI_MDM_UPG_START)
    {
        HI_MDM_UPG_START_IND_S *pstInd = (HI_MDM_UPG_START_IND_S*)pstMsg->ulParam[1];
        
        mrsUpgStartMsgRx(pstInd);
    }

    mrsToolsFree(pstMsg->ulParam[1]);
    
    return HI_ERR_SUCCESS;
}


HI_U32 mrsUpgStartMsgRx(HI_MDM_UPG_START_IND_S * pstInd)
{
    MRS_REMOTE_UPG_MODULE *pstMrsUpgCtx = mrsGetRemoteUpgCtx();
     
    if (pstInd == HI_NULL)
    {
        return HI_ERR_INVALID_PARAMETER;
    }
    
    mrsDfxRxUpgMsgNum();
    mrsDfxRxUpgMsgStart(pstInd->ulRc);
    
    if (pstInd->ulRc != HI_ERR_SUCCESS)//升级失败
    {
        mrsRemoteUpgFree(pstMrsUpgCtx);
        mrsUpgRespClearFile(MRS_CCO_RESP_DENY_FALG, pstMrsUpgCtx);
        mrsRemoteUpgReset(pstMrsUpgCtx);
        
        mrsDfxMrsEndUpgFileTime();
    }

    return pstInd->ulRc;
}


HI_VOID mrsUpgStatusMsgRx(HI_MDM_UPG_STATUS_BS_IND_S *pstInd)
{
    MRS_REMOTE_UPG_MODULE *pstMrsUpgCtx = mrsGetRemoteUpgCtx();

    mrsDfxRxUpgMsgNum();
    MRS_StopTimer(MRS_CCO_TIMER_UPG_STATUS_QUERY_TIMEOUT);
    pstMrsUpgCtx->ucCurrStatusMsgTimes = 0;

/* BEGIN: Modified by fengxiaomin/00209182, 2014/3/27   问题单号:DTS2014031100268 */
    pstMrsUpgCtx->ucAllowStopUpgFlag = MRS_CCO_ALLOW_STOP_UPG;
/* END:   Modified by fengxiaomin/00209182, 2014/3/27 */
    if (pstInd == HI_NULL)
    {
        return;
    }

    HI_DIAG_LOG_MSG_E2(MRS_FILE_LOG_FLAG_004, HI_DIAG_MT("UpgStatus-UpgId"), pstInd->ucUpgStatus, pstInd->ulUpgId);

    mrsDfxRxUpgIdInd(pstInd->ucUpgStatus, pstInd->ulUpgId, pstMrsUpgCtx->ucUpgId, pstMrsUpgCtx->ucUpgPeriod);
    if ((HI_MDM_UPG_STATUS_OK == pstInd->ucUpgStatus) 
        || (HI_MDM_UPG_STATUS_UNKNOWN == pstInd->ucUpgStatus)
        || (HI_MDM_UPG_STATUS_FAIL == pstInd->ucUpgStatus))
    {
        if (pstMrsUpgCtx->ucUpgPeriod != MRS_CCO_UPGRADE_IDLE_PERIOD)
        {
            mrsUpgStatusEndPro(pstMrsUpgCtx);
            return;
        }

        MRS_StopTimer(MRS_CCO_TIMER_UPG_STATUS_QUERY_ACTIVE);
        return;
    }
    else if ((HI_MDM_UPG_STATUS_END == pstInd->ucUpgStatus) || (HI_MDM_UPG_STATUS_PROCESS == pstInd->ucUpgStatus))
    {
        if ((HI_U32)pstMrsUpgCtx->ucUpgId == pstInd->ulUpgId)
        {
            pstMrsUpgCtx->ucUpgPeriod = MRS_CCO_UPGRADE_LOCAL_UPG_PERIOD;
            mrsDfxRefreshUpgStatus(MRS_DFX_CCO_UPG_LOCAL_UPG_STATUS);
            return;
        }

        MRS_StopTimer(MRS_CCO_TIMER_UPG_STATUS_QUERY_ACTIVE);
        
        if (pstMrsUpgCtx->ucUpgPeriod == MRS_CCO_UPGRADE_LOCAL_UPG_PERIOD)
        {
            mrsUpgStatusEndPro(pstMrsUpgCtx);
        }

        return;
    }

    mrsDfxUpgStatusRxErr(pstInd->ucUpgStatus);

    return;
}

/* BEGIN: Modified by fengxiaomin/00209182, 2014/3/27   问题单号:DTS2014032809465 */
HI_U32 mrsUpgStatusEndPro(MRS_REMOTE_UPG_MODULE * pstMrsUpgCtx)
{
    HI_U32 ret = HI_ERR_SUCCESS;
    
    ret = mrsRemoteUpgFree(pstMrsUpgCtx);
    mrsRemoteUpgReset(pstMrsUpgCtx);
    
    mrsDfxMrsEndUpgFileTime();
    mrsDfxRefreshUpgStatus(MRS_DFX_CCO_UPG_NOMAL_FINISH_STATUS);

    return ret;
}
/* END:   Modified by fengxiaomin/00209182, 2014/3/27 */

HI_VOID mrsUpgStopMsgRx(HI_MDM_UPG_STOP_IND_S *pstInd)
{
    MRS_REMOTE_UPG_MODULE *pstMrsUpgCtx = mrsGetRemoteUpgCtx();

    mrsDfxUpgCmdRet(MRS_DFX_CCO_UPG_CMD_STOP, pstInd->stStopInd.ulRc, pstInd->stStopInd.ulUpgId);
    mrsDfxRxUpgMsgNum();

    MRS_StopTimer(MRS_CCO_TIMER_UPG_STOP_RETRY_ACTIVE);
    pstMrsUpgCtx->ucCurrStopUpgMsgTimes = 0;
    HI_DIAG_LOG_MSG_E1(MRS_FILE_LOG_FLAG_039, HI_DIAG_MT("stop upg ret"), pstInd->stStopInd.ulRc);
    
    if (MRS_CCO_UPG_CLEAR_FILE_COMMAND_RX != pstMrsUpgCtx->ucClearFileRxFlag)
    {
        if (HI_MDM_OBJ_ID_NA == pstInd->stStopInd.ulUpgId)
        {
            //收到升级ID为0的停止升级消息后，强制停止主站升级
/* BEGIN: Modified by fengxiaomin/00209182, 2014/3/27   问题单号:DTS2014032809465 */
            mrsRemoteUpgFree(pstMrsUpgCtx);
/* END:   Modified by fengxiaomin/00209182, 2014/3/27 */
            mrsRemoteUpgReset(pstMrsUpgCtx);
        }
        return;
    }

    if (HI_MDM_OBJ_ID_NA == pstInd->stStopInd.ulUpgId)
    {
        return;
    }

    if ((pstInd->stStopInd.ulRc == HI_ERR_SUCCESS) || (pstInd->stStopInd.ulRc == HI_ERR_SUM_INVALID_UPG_ID))
    {
/* BEGIN: Modified by fengxiaomin/00209182, 2014/3/27   问题单号:DTS2014032809465*/
        mrsRemoteUpgFree(pstMrsUpgCtx);
        mrsUpgRespClearFile(MRS_CCO_RESP_ACK_FALG, pstMrsUpgCtx);
        mrsRemoteUpgReset(pstMrsUpgCtx);
        return;
/* END:   Modified by fengxiaomin/00209182, 2014/3/27 */
    }
    else
    {        
        if (MRS_CCO_UPG_CLEAR_FILE_COMMAND_NEED_RESPONSION == pstMrsUpgCtx->ucClearFileRxResp)
        {
            pstMrsUpgCtx->ucClearFileRxFlag = MRS_CCO_UPG_CLEAR_FILE_COMMAND_NON_RX;
/* BEGIN: Modified by fengxiaomin/00209182, 2014/5/6   问题单DTS2014050504643 */
            pstMrsUpgCtx->ucClearFileRxResp = MRS_CCO_UPG_CLEAR_FILE_COMMAND_RESPONSION_OR_TIMEOUT;
/* END:   Modified by fengxiaomin/00209182, 2014/5/6 */
            //组织应答否认帧
            mrsUpgRespClearFile(MRS_CCO_RESP_DENY_FALG, pstMrsUpgCtx);
        }
    }

    return;
}

HI_U32 mrsUpgRespClearFile(HI_U8 ucFlag, MRS_REMOTE_UPG_MODULE *pstMrsUpgCtx)
{
    HI_U32 ret = HI_ERR_SUCCESS;
    HI_U32 ulSeg = 0;
    AFN_FN_UL_PARAM stAfnParam;
    HI_PBYTE pOutBuffer   = HI_NULL;
    HI_U16 usBufferOutLen = 0;

    if (MRS_CCO_RESP_ACK_FALG == ucFlag)
    {
        ulSeg = pstMrsUpgCtx->ulClearFileSeg;
    }
    else
    {
        ulSeg = MRS_CCO_UPG_ERROR_SEG;
    }

    (hi_void)memset_s(&stAfnParam, sizeof(stAfnParam), 0, sizeof(stAfnParam));
    stAfnParam.pstFrame = pstMrsUpgCtx->pstFrame;
    stAfnParam.ppucOutBuffer = &pOutBuffer;
    stAfnParam.pusBufferLen = &usBufferOutLen;
    stAfnParam.ucAfn = 0x15;
    stAfnParam.ucFnIndex = 0;
    
    ret = mrs3762_EasyEnc((HI_U8*)&ulSeg, sizeof(ulSeg), &stAfnParam);
    if ((ret == HI_ERR_SUCCESS) && (pOutBuffer != HI_NULL))
    {
        MRS_SendMrData(pOutBuffer, usBufferOutLen, HI_DMS_CHL_UART_PORT_APP);   // 发送返回报文到集中器
        mrsDfxTxUpgFrameNum();
    }

    mrsToolsFree(pstMrsUpgCtx->pstFrame);
    mrsToolsFree(pOutBuffer);
    pstMrsUpgCtx->pstFrame = HI_NULL;
    pOutBuffer = HI_NULL;
    pstMrsUpgCtx->ulClearFileSeg = 0;

    return ret;
}


HI_VOID mrsUpgLocalPro(MRS_REMOTE_UPG_MODULE *pstMrsUpgCtx)
{
    HI_U32 ret = HI_ERR_SUCCESS;

    pstMrsUpgCtx->ucUpgPeriod = MRS_CCO_UPGRADE_LOCAL_UPG_PERIOD;
    mrsDfxRefreshUpgStatus(MRS_DFX_CCO_UPG_LOCAL_UPG_STATUS);
    MRS_StopTimer(MRS_CCO_TIMER_UPG_FILE_RECEIVE);

/* BEGIN: PN: DTS2015042907476 MODIFY\ADD\DEL by cuiate/00233580 at 2015/4/29 */
    MRS_StopTimer(MRS_CCO_TIMER_UPG_WAIT_PLC_AVAILABLE);
/* END:   PN: DTS2015042907476 MODIFY\ADD\DEL by cuiate/00233580 at 2015/4/29 */

    HI_DIAG_LOG_MSG_E1(MRS_FILE_LOG_FLAG_045, HI_DIAG_MT("route flag"),mrsGetRouteFlag());
    //组网优化完成
    if (HI_TRUE == mrsGetRouteFlag() || (MRS_CCO_LOCAL_CCO_UPGRADE == pstMrsUpgCtx->ucUpgFileFlag))
    {
        pstMrsUpgCtx->ucStartUpgFlag = MRS_CCO_NEED_START_UPG_PLC_OK;
        //启动UPG模块升级
        ret = mrsRemoteUpgStart(0, pstMrsUpgCtx->pstUpgFileInfo, pstMrsUpgCtx->ulUpgradeFileLen);
        if (ret != HI_ERR_SUCCESS)
        {
/* BEGIN: Modified by fengxiaomin/00209182, 2014/3/27   问题单号:DTS2014032809465 */
            mrsRemoteUpgFree(pstMrsUpgCtx);
/* END:   Modified by fengxiaomin/00209182, 2014/3/27 */
            mrsRemoteUpgReset(pstMrsUpgCtx);
            mrsDfxMrsEndUpgFileTime();
            return;
        }

/* BEGIN: Modified by fengxiaomin/00209182, 2014/3/27   问题单号:DTS2014031100268 */
        pstMrsUpgCtx->ucAllowStopUpgFlag = MRS_CCO_DENY_STOP_UPG;
/* END:   Modified by fengxiaomin/00209182, 2014/3/27 */
        //启动查询升级状态
        mrsStartQueryUpgStatus(HI_FALSE, pstMrsUpgCtx);
    }
/* BEGIN: PN: DTS2015042907476 MODIFY\ADD\DEL by cuiate/00233580 at 2015/4/29 */
    else
    {
        pstMrsUpgCtx->ucStartUpgFlag = MRS_CCO_NEED_START_UPG_PLC_BAD;
        MRS_StartTimer(MRS_CCO_TIMER_UPG_WAIT_PLC_AVAILABLE, pstMrsUpgCtx->stUpgParam.ucWaitUpgStartTimeMax * 60 * 1000, HI_SYS_TIMER_ONESHOT);
    }
/* END:   PN: DTS2015042907476 MODIFY\ADD\DEL by cuiate/00233580 at 2015/4/29 */

    return;
}

HI_U32 mrsInitRemoteUpgNvDefault(NV_APP_MRS_UPG_PARAM_STRU *pstNvCfg)
{
    pstNvCfg->ucFileCheckMsgMaxTimes = MRS_CCO_UPG_FILE_CHECK_MSG_MAX_TIMES_DEFAULT;
    pstNvCfg->ucUpgCmdMsgMaxTimes = MRS_CCO_UPG_CMD_MSG_MAX_TIMES_DEFAULT;
    pstNvCfg->usMrsUpgFileRxTimeOut = MRS_CCO_UPG_FILE_RECEIVE_TIME_OUT_DEFAULT;
    pstNvCfg->usMrsUpgStatusQueryTime = MRS_CCO_UPG_STATUS_QUERY_TIME_DEFAULT;
    pstNvCfg->ucPowerOnStatusMaxTimes = MRS_CCO_UPG_STATUS_QUERY_POWERON_TIMES_DEFAULT;
/* BEGIN: PN: DTS2015042907476 MODIFY\ADD\DEL by cuiate/00233580 at 2015/4/29 */
    pstNvCfg->ucWaitUpgStartTimeMax = MRS_CCO_UPG_CMD_WAIT_PLC_AVAILABLE_TIME;
/* END:   PN: DTS2015042907476 MODIFY\ADD\DEL by cuiate/00233580 at 2015/4/29 */

    return HI_ERR_SUCCESS;
}

HI_VOID mrsAFN15F1ReportAck(HI_U32 ulSeg, HI_U8 ucFrameSeq)
{
    HI_U32 ret = HI_ERR_FAILURE;
    PROTO376_2_FRAME_PRE_STRU stFrame;
    HI_U8 * pData = HI_NULL;
    HI_U8 ucDataLen = 0;
    HI_U8 *pOutBuff = HI_NULL;
    HI_U16 usOutLen = 0;

    ucDataLen = 4; //段标识
    pData = (HI_U8 *)mrsToolsMalloc(ucDataLen);
    if (HI_NULL == pData)
    {
        return;
    }
    
    (hi_void)memset_s(pData, ucDataLen, 0, ucDataLen);
    (hi_void)memset_s(&stFrame, sizeof(stFrame), 0, sizeof(stFrame));
    (hi_void)memcpy_s(pData, ucDataLen, &ulSeg, ucDataLen);
    
    stFrame.ucSeq = ucFrameSeq;
    stFrame.stAfnData.ucAFN = MRS_AFN(0x15);
    stFrame.stAfnData.ucDT[0] = MRS_AFN_FN_LO(1);
    stFrame.stAfnData.ucDT[1] = MRS_AFN_FN_HI(1);
    stFrame.stAfnData.pData = pData;
    stFrame.stAfnData.usDataLen = ucDataLen;

    ret = mrsCreate3762Frame(&stFrame, &pOutBuff, &usOutLen, stFrame.ucSrcAdd);
    if ((ret == HI_ERR_SUCCESS) && (pOutBuff != HI_NULL))
    {
        MRS_SendMrData(pOutBuff, usOutLen, HI_DMS_CHL_UART_PORT_APP);   // 发送15-F1返回报文到集中器
        mrsDfxTxUpgFrameNum();
    }

    mrsToolsFree(pOutBuff);
    mrsToolsFree(pData);

    return;
}

HI_VOID mrsRemoteUpgReset(MRS_REMOTE_UPG_MODULE * pstMrsUpgCtx)
{
    MRS_CCO_REMOTE_UPG_PARAM_S stUpgParam = {0};
    HI_U8 ucUpgId = pstMrsUpgCtx->ucUpgId;
    
    (hi_void)memset_s(&stUpgParam, sizeof(stUpgParam), 0, sizeof(stUpgParam));
    (hi_void)memcpy_s(&stUpgParam, sizeof(stUpgParam), &pstMrsUpgCtx->stUpgParam, sizeof(stUpgParam));
    (hi_void)memset_s(pstMrsUpgCtx, sizeof(MRS_REMOTE_UPG_MODULE), 0, sizeof(MRS_REMOTE_UPG_MODULE));
    (hi_void)memcpy_s(&pstMrsUpgCtx->stUpgParam, sizeof(MRS_CCO_REMOTE_UPG_PARAM_S), &stUpgParam, sizeof(MRS_CCO_REMOTE_UPG_PARAM_S));
    pstMrsUpgCtx->ucUpgPeriod = MRS_CCO_UPGRADE_IDLE_PERIOD;
    pstMrsUpgCtx->ucUpgId = ucUpgId;
    //关闭定时器
    MRS_StopTimer(MRS_CCO_TIMER_UPG_FILE_RECEIVE);
    MRS_StopTimer(MRS_CCO_TIMER_UPG_STOP_RETRY_ACTIVE);
    MRS_StopTimer(MRS_CCO_TIMER_UPG_STATUS_QUERY_ACTIVE);
    MRS_StopTimer(MRS_CCO_TIMER_UPG_STATUS_QUERY_TIMEOUT);
/* BEGIN: PN: DTS2015042907476 MODIFY\ADD\DEL by cuiate/00233580 at 2015/4/29 */
    MRS_StopTimer(MRS_CCO_TIMER_UPG_WAIT_PLC_AVAILABLE);
/* END:   PN: DTS2015042907476 MODIFY\ADD\DEL by cuiate/00233580 at 2015/4/29 */

    return;
}

HI_VOID mrsRemoteUpgIdSelfAdd(HI_VOID)
{
    MRS_REMOTE_UPG_MODULE * pstMrsUpgCtx = mrsGetRemoteUpgCtx(); 
    
	pstMrsUpgCtx->ucUpgId++;  	
    if (pstMrsUpgCtx->ucUpgId > 4)
    {
        pstMrsUpgCtx->ucUpgId = 1;
    }
    
    //存储到NV 里
    mrsNvSetUpgId(pstMrsUpgCtx->ucUpgId);
}

HI_U32 mrsRemoteUpgStart(HI_U16 ulId, HI_PVOID pData, HI_U32 ulFileSize)
{
    HI_U32 ret = HI_ERR_SUCCESS;
    HI_MDM_UPG_START_REQ_S* pstReq = (HI_MDM_UPG_START_REQ_S*)pData;
    MRS_REMOTE_UPG_MODULE * pstMrsUpgCtx = mrsGetRemoteUpgCtx();
    HI_U8 ucIndex = 0;
    HI_U32 ulUpgType = 0;
    HI_U16 usChecksum = 0;

    HI_UNREF_PARAM(ulId);

    ulUpgType = mrsGetUpgType(pstMrsUpgCtx->ucUpgFileFlag);

    mrsRemoteUpgIdSelfAdd();

    // 升级配置
    pstReq->stUpgCfg.ulOption        = ulUpgType | HI_UPG_VER_UPDATE_OPTION_START;
    pstReq->stUpgCfg.ulFileCrc = HI_MDM_Crc32(0, pstMrsUpgCtx->pstUpgFileInfo->stUpgFile.ucFileBlk, ulFileSize);
    pstReq->stUpgCfg.ulUpgId         = pstMrsUpgCtx->ucUpgId;
    pstReq->stUpgCfg.ulFileLen       = ulFileSize;
    pstReq->stUpgCfg.ulBlkNum = MRS_CCO_REMOTE_UPG_START_FIX_BLKNUM;
    pstReq->stUpgCfg.ulUpgTimeWindow = 0;

    usChecksum = mrsToolsCalcCheckSum2(pstMrsUpgCtx->pstUpgFileInfo->stUpgFile.ucFileBlk, (HI_U16)ulFileSize);
    // 升级文件
    pstReq->stUpgFile.ulUpgId        = pstReq->stUpgCfg.ulUpgId;
    pstReq->stUpgFile.usFileBlkSize  = 0;
    pstReq->stUpgFile.usIdx = MRS_CCO_REMOTE_UPG_START_FIX_INDEX;
    pstReq->stUpgFile.usChecksum = usChecksum;

    
    mrsDfxUpgStartTime();
    // APP层发送升级消息给MDM层
    for (ucIndex = 0; ucIndex <= 1; ucIndex++)
    {
        mrsDfxTxUpgMsgNum();
        ret = HI_MDM_UPG_MsgSend(HI_MDM_UPG_START, (HI_U8*)pstReq, sizeof(HI_MDM_UPG_START_REQ_S) + ulFileSize);
        if (HI_ERR_SUCCESS == ret)
        {
            break;
        }
        else
        {
            mrsDfxSendMsgFail(MRS_DFX_CCO_UPG_START_MSG_ID, (HI_U16)ret);
        }
    }

    mrsDfxUpgCmdRet(MRS_DFX_CCO_UPG_CMD_START, ret, 0);
    
    if (ret != HI_ERR_SUCCESS)
    {
        mrsDfxRefreshUpgStatus(MRS_DFX_CCO_UPG_START_UPG_FAIL_STATUS);
    }
    
    return ret;

}

HI_U32 mrsRemoteUpgStop(HI_U16 ulId, HI_PVOID pCmdParam, HI_U16 usCmdParamSize)
{
    HI_U32 ret = 0;
    HI_MDM_UPG_STOP_REQ_S stReq;
    HI_U8 ucIndex = 0;
    MRS_REMOTE_UPG_MODULE *pstMrsUpgCtx = (MRS_REMOTE_UPG_MODULE*)pCmdParam;

    HI_UNREF_PARAM(ulId);
    HI_UNREF_PARAM(usCmdParamSize);

    (hi_void)memset_s(&stReq, sizeof(stReq), 0, sizeof(stReq));

    stReq.stStopReq.ulUpgId = pstMrsUpgCtx->ucUpgId;
/* BEGIN: Modified by fengxiaomin/00209182, 2014/5/6   问题单DTS2014050504643 */
    pstMrsUpgCtx->ucClearFileRxResp = MRS_CCO_UPG_CLEAR_FILE_COMMAND_NEED_RESPONSION;
/* END:   Modified by fengxiaomin/00209182, 2014/5/6 */

    mrsDfxUpgEndTime();
    
    for (ucIndex = 0; ucIndex <= 1; ucIndex++)
    {
        HI_U32 ulRet = HI_ERR_SUCCESS;

        mrsDfxTxUpgMsgNum();
        ret = HI_MDM_UPG_MsgSend(HI_MDM_UPG_STOP, (HI_U8*)&stReq, sizeof(HI_MDM_UPG_STOP_REQ_S));
        if (HI_ERR_SUCCESS != ret)
        {
            mrsDfxSendMsgFail(MRS_DFX_CCO_UPG_STOP_MSG_ID, (HI_U16)ret);
            continue;
        }

        pstMrsUpgCtx->ucCurrStopUpgMsgTimes++;
        
        if (MRS_CCO_UPG_CLEAR_FILE_COMMAND_RX != pstMrsUpgCtx->ucClearFileRxFlag)
        {
            break;
        }

        ret = HI_ERR_CONTINUE;
        //开启停止升级超时定时器800ms
        ulRet = MRS_StartTimer(MRS_CCO_TIMER_UPG_STOP_RETRY_ACTIVE, 
                            MRS_CCO_UPG_CLEAR_FILE_TIMEOUT_VALUE,
                            HI_SYS_TIMER_ONESHOT);
        if (ulRet != HI_ERR_SUCCESS)
        {
            mrsDfxSendMsgFail(MRS_DFX_CCO_UPG_STOP_TIMEOUT_ID, (HI_U16)ulRet);
        }

        break;
    }
    
    return ret;
}

HI_U32 mrsRemoteUpgStatus(HI_VOID)
{
    HI_U32 ret = 0;
    HI_MDM_UPG_STATUS_REQ_S stReq;
    HI_U8 ucIndex = 0;

    (hi_void)memset_s(&stReq, sizeof(stReq), 0, sizeof(stReq));
    stReq.stReqStatus.ulOption = HI_MDM_UPG_STATUS_REQ_OPT_BS;
    
    for (ucIndex = 0; ucIndex <= 1; ucIndex++)
    {
        mrsDfxTxUpgMsgNum();
        ret = HI_MDM_UPG_MsgSend(HI_MDM_UPG_STATUS, (HI_U8*)&stReq, sizeof(HI_MDM_UPG_STATUS_REQ_S));
        if (HI_ERR_SUCCESS == ret)
        {
            break;
        }
        
        mrsDfxSendMsgFail(MRS_DFX_CCO_UPG_STATUS_QUERY_MSG_ID, (HI_U16)ret);
    }
    
    return ret;
}

HI_BOOL mrsIsUpgFileFlagValid(HI_U8 ucFileFlag, HI_U8 ucFileProperty)
{
    HI_BOOL bRet = HI_TRUE;
    
    if ((ucFileFlag == MRS_CCO_CLEAR_UPG_LOAD_FILE) || (ucFileFlag == MRS_CCO_LOCAL_CCO_UPGRADE) 
        || (ucFileFlag == MRS_CCO_BOARDCAST_UPGRADE) || (ucFileFlag == MRS_CCO_STA_UPGRADE))
    {
        bRet = HI_TRUE;
    }
    else
    {
        mrsDfxRefreshFrameErrFlag(MRS_DFX_CCO_UPG_FILE_FLAG_OVERRUN_ERR);
        return HI_FALSE;
    }

    if (ucFileProperty > MRS_CCO_UPG_END_FRAME)
    {
        mrsDfxRefreshFrameErrFlag(MRS_DFX_CCO_UPG_FILE_PROPERTY_OVERRUN_ERR);
        return HI_FALSE;
    }

    return bRet;
}

HI_BOOL mrsUpgFileLenValid(HI_U8 *pContent, HI_U8 ucDataLenOff, HI_U8 ucCurrSegOff)
{
    HI_BOOL bRet = HI_TRUE;
    HI_U8 ucFileFlag = 0;
    HI_U16 usDataLen = 0;
    HI_U32 ulTotalSegNum = 0;
    HI_U32 ulSeg = 0;

    (hi_void)memcpy_s(&usDataLen, sizeof(usDataLen), pContent + ucDataLenOff, sizeof(usDataLen));
    ucFileFlag = *(pContent + MRS_CCO_UPG_FILE_FLAG_OFFSET);
    (hi_void)memcpy_s(&ulTotalSegNum, sizeof(ulTotalSegNum), pContent + MRS_CCO_UPG_TOTAL_SEG_OFFSET, sizeof(HI_U16));
    (hi_void)memcpy_s(&ulSeg, sizeof(ulSeg), pContent + ucCurrSegOff, sizeof(ulSeg));
  
    if (ulSeg != ulTotalSegNum - 1)
    {
        bRet= mrsUpgFileStartLenValid(ulTotalSegNum, usDataLen, ucFileFlag);
    }
    else
    {
        bRet= mrsUpgFileEndLenValid(ulTotalSegNum, usDataLen, ucFileFlag);
    }

    return bRet;
}

HI_BOOL mrsUpgFileStartLenValid(HI_U32 ulTotalSegNum, HI_U16 usDataLen, HI_U8 ucFileFlag)
{
    HI_U32 ulTempLen = 0;
    
    if (usDataLen > MRS_CCO_SINGAL_UPG_DATA_LEN_MAX || !usDataLen )
    {
        mrsDfxRefreshFrameErrFlag(MRS_DFX_CCO_UPG_SINGLE_LEN_OVERRUN_ERR);
        return HI_FALSE;
    }

    do
    {
        ulTempLen = (ulTotalSegNum - 1) * usDataLen;
        if (((ucFileFlag == MRS_CCO_LOCAL_CCO_UPGRADE) && (ulTempLen > HI_MDM_UPG_CCO_FILE_MAX_SIZE))
            || ((ucFileFlag == MRS_CCO_STA_UPGRADE) && (ulTempLen > HI_MDM_UPG_STA_FILE_RAM_MAX_SIZE))
            || ((ucFileFlag == MRS_CCO_BOARDCAST_UPGRADE) && (ulTempLen > HI_MDM_UPG_CCO_STA_FILE_MAX_SIZE)))
        {
            break;
        }

        return HI_TRUE;
    } while (0);

    mrsDfxRefreshFrameErrFlag(MRS_DFX_CCO_UPG_FILE_LEN_ERR);
    mrsDfxUpgFileErrLen(ulTempLen);
    return HI_FALSE;
}

HI_BOOL mrsUpgFileEndLenValid(HI_U32 ulTotalSegNum, HI_U16 usDataLen, HI_U8 ucFileFlag)
{
    HI_BOOL bRet = HI_TRUE;
    MRS_REMOTE_UPG_MODULE * pstMrsUpgCtx = mrsGetRemoteUpgCtx();

    HI_UNREF_PARAM(ulTotalSegNum);
    HI_UNREF_PARAM(usDataLen);

    do
    {

        if (((ucFileFlag == MRS_CCO_LOCAL_CCO_UPGRADE) && (pstMrsUpgCtx->ulUpgradeFileLen > HI_MDM_UPG_CCO_FILE_MAX_SIZE))
            || ((ucFileFlag == MRS_CCO_STA_UPGRADE) && (pstMrsUpgCtx->ulUpgradeFileLen > HI_MDM_UPG_STA_FILE_RAM_MAX_SIZE)))
        {
            bRet = HI_FALSE;
            break;
        }

        if ((ucFileFlag == MRS_CCO_BOARDCAST_UPGRADE) 
            && (pstMrsUpgCtx->ulUpgradeFileLen > HI_MDM_UPG_CCO_STA_FILE_MAX_SIZE))
        {
            bRet = HI_FALSE;
            break;
        }
    } while(0);

    if (!bRet)
    {
        mrsDfxUpgFileErrLen(pstMrsUpgCtx->ulUpgradeFileLen);
        mrsDfxRefreshFrameErrFlag(MRS_DFX_CCO_UPG_FILE_LEN_ERR);
    }
    
    return bRet;
}

HI_BOOL mrsIsCcoStatePermitUpg(HI_VOID)
{
    MRS_CCO_SRV_CTX_STRU * pstCcoCtx = mrsCcoGetContext();

    if ((HI_FALSE == pstCcoCtx->bPauseFlag) || (pstCcoCtx->status & MRS_CCO_STATUS_SEARCH_METER_MASK))
    {
        return HI_FALSE;
    }

    return HI_TRUE;
}

HI_U32 mrsRemoteUpgClear(MRS_REMOTE_UPG_MODULE * pstMrsUpgCtx)
{
    HI_U32 ret = HI_ERR_SUCCESS;
    HI_U8 ucUpgPeriod = pstMrsUpgCtx->ucUpgPeriod;
        
    HI_DIAG_LOG_MSG_E1(MRS_FILE_LOG_FLAG_030, HI_DIAG_MT("upg period rx stop upg"),ucUpgPeriod);
    if (MRS_CCO_UPGRADE_IDLE_PERIOD == ucUpgPeriod)
    {
        return ret;
    }

/* BEGIN: Modified by fengxiaomin/00209182, 2014/3/27   问题单号:DTS2014032809465*/
    if (MRS_CCO_UPGRADE_LOCAL_LOAD_PERIOD == ucUpgPeriod)
    {
        mrsRemoteUpgFree(pstMrsUpgCtx);
        mrsRemoteUpgReset(pstMrsUpgCtx);
        return ret;
    }
/* END:   Modified by fengxiaomin/00209182, 2014/3/27 */
    
/* BEGIN: Modified by fengxiaomin/00209182, 2014/3/27   问题单号:DTS2014031100268 */
    if (MRS_CCO_DENY_STOP_UPG == pstMrsUpgCtx->ucAllowStopUpgFlag)
    {
        mrsDfxStartUpgRxStop();
        return HI_ERR_FAILURE;
    }
/* END:   Modified by fengxiaomin/00209182, 2014/3/27 */
    
    pstMrsUpgCtx->ucClearFileRxFlag = MRS_CCO_UPG_CLEAR_FILE_COMMAND_RX;
    //停止升级
    ret = mrsRemoteUpgStop(ucUpgPeriod, pstMrsUpgCtx, 0);
    if (HI_ERR_CONTINUE == ret)
    {
        mrsDfxMrsEndUpgFileTime();
        mrsDfxRefreshUpgStatus(MRS_DFX_CCO_UPG_RX_STOP_UPG_STATUS);
    }

    return ret;
}

/* BEGIN: Modified by fengxiaomin/00209182, 2014/3/27   问题单号:DTS2014032809465 */
HI_U32 mrsRemoteUpgFree(MRS_REMOTE_UPG_MODULE * pstMrsUpgCtx)
{
    HI_U32 ret = HI_ERR_SUCCESS;

    if (pstMrsUpgCtx->pstUpgFileInfo == HI_NULL)
    {
        return ret;
    }
    
    ret |= HI_MDM_UPG_ReleaseMem(MRS_CCO_REMOTE_UPG_OBJ_ID, pstMrsUpgCtx->pstUpgFileInfo, 0);


    pstMrsUpgCtx->pstUpgFileInfo = HI_NULL;
/* BEGIN: PN: DTS2015061901503 MODIFY\ADD\DEL by fengxiaomin/00209182 at 2015/6/22 */
    pstMrsUpgCtx->ulUpgradeFileLen = 0;
/* END:   PN: DTS2015061901503 MODIFY\ADD\DEL by fengxiaomin/00209182 at 2015/6/22 */

    return ret;
}
/* END:   Modified by fengxiaomin/00209182, 2014/3/27 */


HI_U32 mrsRemoteUpgStartFramePro(HI_U8 * pContent)
{
    HI_U32 ret = HI_ERR_SUCCESS;
    MRS_REMOTE_UPG_MODULE * pstMrsUpgCtx = mrsGetRemoteUpgCtx();
    HI_U32 ulFileSize = 0;
    HI_U8 ucFileFlag = 0;
    HI_U8 * pstBuf = HI_NULL;
    
/* BEGIN: Modified by fengxiaomin/00209182, 2014/3/27   问题单号:DTS2014032809465 */
    mrsDfxRxUpgFrameNum();
/* END:   Modified by fengxiaomin/00209182, 2014/3/27 */
    if (pstMrsUpgCtx->ucUpgPeriod != MRS_CCO_UPGRADE_IDLE_PERIOD) 
    {
        return HI_ERR_SUCCESS;
    }

    if (!mrsUpgFileLenValid(pContent, MRS_CCO_UPG_DATA_LEN_OFFSET, MRS_CCO_UPG_CURRENT_SEG_OFFSET))
    {
        return HI_ERR_FAILURE;
    }
    
    ucFileFlag = *(pContent + MRS_CCO_UPG_FILE_FLAG_OFFSET);

    switch (ucFileFlag)
    {
        case MRS_CCO_LOCAL_CCO_UPGRADE:
            ulFileSize = HI_MDM_UPG_CCO_FILE_MAX_SIZE;
            break;

        case MRS_CCO_STA_UPGRADE:
            ulFileSize = HI_MDM_UPG_STA_FILE_RAM_MAX_SIZE;
            break;

        case MRS_CCO_BOARDCAST_UPGRADE:
            ulFileSize = HI_MDM_UPG_CCO_STA_FILE_MAX_SIZE;
            break;

        default:
            break;
    }
    
/* BEGIN: Modified by fengxiaomin/00209182, 2014/3/27   问题单号:DTS2014032809465 */
    mrsRemoteUpgFree(pstMrsUpgCtx);
    ret = HI_MDM_UPG_ObtainMem(MRS_CCO_REMOTE_UPG_OBJ_ID, ulFileSize, (HI_MDM_UPG_START_REQ_S**)&pstBuf, 0);
    if ((ret != HI_ERR_SUCCESS) || (pstBuf == HI_NULL))
    {
        mrsDfxGetUpgMemoryFail();
        return HI_ERR_FAILURE;
    }
/* END:   Modified by fengxiaomin/00209182, 2014/3/27 */

    pstMrsUpgCtx->pstUpgFileInfo = (HI_MDM_UPG_START_REQ_S*)pstBuf;
    (hi_void)memset_s(pstMrsUpgCtx->pstUpgFileInfo, sizeof(HI_MDM_UPG_START_REQ_S), 0, sizeof(HI_MDM_UPG_START_REQ_S));
    (hi_void)memcpy_s(&pstMrsUpgCtx->ulTotalSegNum, sizeof(pstMrsUpgCtx->ulTotalSegNum), pContent + MRS_CCO_UPG_TOTAL_SEG_OFFSET, sizeof(HI_U16));
    (hi_void)memcpy_s(&pstMrsUpgCtx->ulCurrSegNum, sizeof(pstMrsUpgCtx->ulCurrSegNum), pContent + MRS_CCO_UPG_CURRENT_SEG_OFFSET, 
        sizeof(pstMrsUpgCtx->ulCurrSegNum));
    (hi_void)memcpy_s(&pstMrsUpgCtx->usSingleSegLen, sizeof(pstMrsUpgCtx->usSingleSegLen), pContent + MRS_CCO_UPG_DATA_LEN_OFFSET, 
        sizeof(pstMrsUpgCtx->usSingleSegLen));
    pstMrsUpgCtx->ucUpgFileFlag= ucFileFlag;
    pstMrsUpgCtx->ucUpgPeriod = MRS_CCO_UPGRADE_LOCAL_LOAD_PERIOD;
    pstMrsUpgCtx->ulUpgradeFileLen = (HI_U32)pstMrsUpgCtx->usSingleSegLen;
    (hi_void)memcpy_s(pstBuf + sizeof(HI_MDM_UPG_START_REQ_S), ulFileSize, pContent + MRS_CCO_UPG_FIX_DATA_OFFSET, pstMrsUpgCtx->usSingleSegLen);
    mrsRemoteUpgRxFilePro();
    
/* BEGIN: Modified by fengxiaomin/00209182, 2014/3/27   问题单号:DTS2014032809465*/
    mrsDfxRemoteUpgReset();
    mrsDfxRxUpgFrameNum();
/* END:   Modified by fengxiaomin/00209182, 2014/3/27 */
    mrsDfxMrsStartUpgFileTime();
    mrsDfxGetUpgFileFlag(pstMrsUpgCtx->ucUpgFileFlag);
    mrsDfxRefreshUpgStatus(MRS_DFX_CCO_UPG_FILE_LOAD_STATUS);

    if (pstMrsUpgCtx->ulCurrSegNum == (pstMrsUpgCtx->ulTotalSegNum - 1))
    {
        ret = mrsUpgFileCheck(pstMrsUpgCtx);
        if (HI_ERR_SUCCESS != ret)
        {
            mrsRemoteUpgFree(pstMrsUpgCtx);
            mrsRemoteUpgReset(pstMrsUpgCtx);
            mrsDfxMrsEndUpgFileTime();
            return ret;
        }
    
    	pstMrsUpgCtx->ucFileHeadCheckFlag = MRS_CCO_UPG_FILE_CHECK_FINISH;

		mrsUpgLocalPro(pstMrsUpgCtx);
    
    }
    
    return ret;
}

HI_BOOL mrsIsSegValid(HI_U8 * pContent, HI_U8 ucSegOffset, MRS_REMOTE_UPG_MODULE * pstMrsUpgCtx)
{
    HI_U32 ulSeg = 0;

    (hi_void)memcpy_s(&ulSeg, sizeof(ulSeg), pContent + ucSegOffset, sizeof(ulSeg));
    
    if (ulSeg != pstMrsUpgCtx->ulCurrSegNum + 1)
    {
        return HI_FALSE;
    }

    return HI_TRUE;
}

HI_U32 mrsRemoteUpgMiddleFramePro(HI_U8 * pContent)
{
    HI_U32 ret = HI_ERR_SUCCESS;
    MRS_REMOTE_UPG_MODULE * pstMrsUpgCtx = mrsGetRemoteUpgCtx();
    HI_U32 ulOffsetLen = 0;

    //判断0段是否存在
    if (pstMrsUpgCtx->ucUpgPeriod == MRS_CCO_UPGRADE_IDLE_PERIOD)
    {
        return HI_ERR_FAILURE;
    }
    
    if (!mrsIsSegValid(pContent, MRS_CCO_UPG_CURRENT_SEG_OFFSET, pstMrsUpgCtx))
    {
        return HI_ERR_SUCCESS;
    }
    
    if (HI_ERR_SUCCESS != mrsUpgMiddleEndFrameValid(pContent, MRS_CCO_UPG_DATA_LEN_OFFSET, 
        MRS_CCO_UPG_CURRENT_SEG_OFFSET))
    {
        return HI_ERR_FAILURE;
    }

    ulOffsetLen = (pstMrsUpgCtx->ulCurrSegNum + 1) * pstMrsUpgCtx->usSingleSegLen;
    pstMrsUpgCtx->ulUpgradeFileLen += pstMrsUpgCtx->usSingleSegLen;
    (hi_void)memcpy_s(pstMrsUpgCtx->pstUpgFileInfo->stUpgFile.ucFileBlk + ulOffsetLen, pstMrsUpgCtx->usSingleSegLen,
        pContent + MRS_CCO_UPG_FIX_DATA_OFFSET, pstMrsUpgCtx->usSingleSegLen);
    
    (hi_void)memcpy_s(&pstMrsUpgCtx->ulCurrSegNum, sizeof(pstMrsUpgCtx->ulCurrSegNum), 
            pContent + MRS_CCO_UPG_CURRENT_SEG_OFFSET, sizeof(pstMrsUpgCtx->ulCurrSegNum));
    
    return ret;
}

HI_U32 mrsRemoteUpgEndFramePro(HI_U8 * pContent)
{
    HI_U32 ret = HI_ERR_SUCCESS;
    MRS_REMOTE_UPG_MODULE * pstMrsUpgCtx = mrsGetRemoteUpgCtx();
    HI_U16 usDataLen = 0;
    HI_U32 ulOffsetLen = 0;

    //判断0段是否存在
    if (pstMrsUpgCtx->ucUpgPeriod == MRS_CCO_UPGRADE_IDLE_PERIOD)
    {
        return HI_ERR_FAILURE;
    }

    if (!mrsIsSegValid(pContent, MRS_CCO_UPG_CURRENT_SEG_OFFSET, pstMrsUpgCtx))
    {
        return HI_ERR_SUCCESS;
    }

    if (HI_ERR_SUCCESS != mrsUpgMiddleEndFrameValid(pContent, MRS_CCO_UPG_DATA_LEN_OFFSET, 
        MRS_CCO_UPG_CURRENT_SEG_OFFSET))
    {
        return HI_ERR_FAILURE;
    }

    (hi_void)memcpy_s(&usDataLen, sizeof(usDataLen), pContent + MRS_CCO_UPG_DATA_LEN_OFFSET, sizeof(usDataLen));
    pstMrsUpgCtx->ulUpgradeFileLen += usDataLen;
    pstMrsUpgCtx->usEndSegLen = usDataLen;
    if (!mrsUpgFileLenValid(pContent, MRS_CCO_UPG_DATA_LEN_OFFSET, MRS_CCO_UPG_CURRENT_SEG_OFFSET))
    {
/* BEGIN: PN: DTS2015061901503 MODIFY\ADD\DEL by fengxiaomin/00209182 at 2015/6/22 */
        pstMrsUpgCtx->ulUpgradeFileLen -= usDataLen;
        pstMrsUpgCtx->usEndSegLen = 0;
        return HI_ERR_FAILURE;
    }

    (hi_void)memcpy_s(&pstMrsUpgCtx->ulCurrSegNum, sizeof(pstMrsUpgCtx->ulCurrSegNum), 
        pContent + MRS_CCO_UPG_CURRENT_SEG_OFFSET, sizeof(pstMrsUpgCtx->ulCurrSegNum));

    ulOffsetLen = (pstMrsUpgCtx->ulCurrSegNum) * pstMrsUpgCtx->usSingleSegLen;
    (hi_void)memcpy_s(pstMrsUpgCtx->pstUpgFileInfo->stUpgFile.ucFileBlk + ulOffsetLen, usDataLen, 
        pContent + MRS_CCO_UPG_FIX_DATA_OFFSET, usDataLen);
/* END:   PN: DTS2015061901503 MODIFY\ADD\DEL by fengxiaomin/00209182 at 2015/6/22 */
    
	ret = mrsUpgFileCheck(pstMrsUpgCtx);
    if (HI_ERR_SUCCESS != ret)
    {
        pstMrsUpgCtx->ulUpgradeFileLen -= usDataLen;
        pstMrsUpgCtx->usEndSegLen = 0;
        HI_DIAG_LOG_MSG_E1(__LINE__, HI_DIAG_MT("FILE CHECK ERR"),ret);
        mrsDfxRefreshFrameErrFlag(MRS_DFX_CCO_UPG_FILE_CRC_ERR);
        return ret;
    }
    
    pstMrsUpgCtx->ucFileHeadCheckFlag = MRS_CCO_UPG_FILE_CHECK_FINISH;

	mrsUpgLocalPro(pstMrsUpgCtx);

    return ret;
}

HI_U32 mrsGetUpgType(HI_U8 ucFileFlag)
{
    HI_U32 ret = HI_UPG_REQ_OPTION_NA;

    switch (ucFileFlag)
    {
    case MRS_CCO_LOCAL_CCO_UPGRADE:
        ret = HI_UPG_REQ_OPTION_CCO;
        break;
        
    case MRS_CCO_BOARDCAST_UPGRADE:
        ret = HI_UPG_REQ_OPTION_BROADCAST;
        break;
        
    case MRS_CCO_STA_UPGRADE:
        ret = HI_UPG_REQ_OPTION_STA;
        break;
        
    default:
        break;
    }

    return ret;
}


HI_U32 mrsUpgMiddleEndFrameValid(HI_U8 * pContent, HI_U8 ucDataLenOff, HI_U8 ucCurrSegOff)
{
    HI_U32 ret = HI_ERR_SUCCESS;
    MRS_REMOTE_UPG_MODULE * pstMrsUpgCtx = mrsGetRemoteUpgCtx();
    HI_U16 usDataLen = 0;
    HI_U32 ulTotalSegNum = 0;
    HI_U32 ulSeg = 0;

    if (pstMrsUpgCtx->ucUpgFileFlag != *(pContent + MRS_CCO_UPG_FILE_FLAG_OFFSET))
    {
        mrsDfxRefreshFrameErrFlag(MRS_DFX_CCO_UPG_FILE_FLAG_DIFF_START_ERR);
        return HI_ERR_FAILURE;
    }
    
    (hi_void)memcpy_s(&ulTotalSegNum, sizeof(HI_U16), pContent + MRS_CCO_UPG_TOTAL_SEG_OFFSET, sizeof(HI_U16));
    if (ulTotalSegNum != pstMrsUpgCtx->ulTotalSegNum)
    {
        mrsDfxRefreshFrameErrFlag(MRS_DFX_CCO_UPG_TOTAL_SEG_DIFF_START_ERR);
        return HI_ERR_FAILURE;
    }

    (hi_void)memcpy_s(&ulSeg, sizeof(ulSeg), pContent + ucCurrSegOff, sizeof(ulSeg));
    (hi_void)memcpy_s(&usDataLen, sizeof(usDataLen), pContent + ucDataLenOff, sizeof(usDataLen));
    if (ulSeg != ulTotalSegNum - 1)
    {
        if ((usDataLen != pstMrsUpgCtx->usSingleSegLen) || (!usDataLen))
        {
            mrsDfxRefreshFrameErrFlag(MRS_DFX_CCO_UPG_MIDDLE_FRAME_LEN_DIFF_START_ERR);
            return HI_ERR_FAILURE;
        }
    }
    else
    {
        if (usDataLen > pstMrsUpgCtx->usSingleSegLen)
        {
            mrsDfxRefreshFrameErrFlag(MRS_DFX_CCO_UPG_END_FRAME_LEN_OVER_START_ERR);
            return HI_ERR_FAILURE;
        }
    }

    return ret;
}

HI_VOID mrsUpgStopTimeOutPro(HI_VOID)
{
    MRS_REMOTE_UPG_MODULE * pstMrsUpgCtx = mrsGetRemoteUpgCtx();
    HI_U8 ucStopMaxTimes = pstMrsUpgCtx->stUpgParam.ucUpgCmdMsgMaxTimes;

    if (pstMrsUpgCtx->ucCurrStopUpgMsgTimes < ucStopMaxTimes)
    {
        HI_DIAG_LOG_MSG_E1(MRS_FILE_LOG_FLAG_022, HI_DIAG_MT("upg stop timeout"),
            pstMrsUpgCtx->ucCurrStopUpgMsgTimes);
        mrsRemoteUpgStop(pstMrsUpgCtx->ucUpgPeriod, pstMrsUpgCtx, 0);
    }
    else
    {
        pstMrsUpgCtx->ucCurrStopUpgMsgTimes = 0;
        pstMrsUpgCtx->ucClearFileRxFlag = MRS_CCO_UPG_CLEAR_FILE_COMMAND_NON_RX;
/* BEGIN: Modified by fengxiaomin/00209182, 2014/5/6   问题单DTS2014050504643 */
        pstMrsUpgCtx->ucClearFileRxResp = MRS_CCO_UPG_CLEAR_FILE_COMMAND_RESPONSION_OR_TIMEOUT;
/* END:   Modified by fengxiaomin/00209182, 2014/5/6 */
        //组织应答否认帧
        mrsUpgRespClearFile(MRS_CCO_RESP_DENY_FALG, pstMrsUpgCtx);
    }

    return;
}

HI_VOID mrsUpgStatusTimeOutPro(HI_VOID)
{
    mrsGetRemoteUpgCtx()->ucAllowStopUpgFlag = MRS_CCO_ALLOW_STOP_UPG;
    mrsRemoteUpgStatus();

    return;
}

HI_VOID mrsRemoteUpgRxFilePro(HI_VOID)
{
    HI_U32 ret = HI_ERR_SUCCESS;
    MRS_REMOTE_UPG_MODULE * pstMrsUpgCtx = mrsGetRemoteUpgCtx();
    
    if (pstMrsUpgCtx->ucUpgPeriod == MRS_CCO_UPGRADE_LOCAL_LOAD_PERIOD)
    {
       //启动10分钟超时定时器
        ret = MRS_StartTimer(MRS_CCO_TIMER_UPG_FILE_RECEIVE, 
                            pstMrsUpgCtx->stUpgParam.usMrsUpgFileRxTimeOut * 1000,
                            HI_SYS_TIMER_ONESHOT);

       if (ret != HI_ERR_SUCCESS)
       {
            mrsDfxSendMsgFail(MRS_DFX_CCO_UPG_TEN_MINUTE_RX_TIMEOUT_ID, (HI_U16)ret);
       }
    }

    return;
}

HI_U8 mrsGetRemoteUpgStatus(HI_VOID)
{
    MRS_REMOTE_UPG_MODULE * pstMrsUpgCtx = mrsGetRemoteUpgCtx();

    return pstMrsUpgCtx->ucUpgPeriod;
}

HI_VOID mrsUpgRxFileTimeOutPro(HI_VOID)
{
    MRS_REMOTE_UPG_MODULE * pstMrsUpgCtx = mrsGetRemoteUpgCtx();
    
    HI_DIAG_LOG_MSG_E0(MRS_FILE_LOG_FLAG_023, HI_DIAG_MT("mrsUpgRxFileTimeOutPro"));
/* BEGIN: Modified by fengxiaomin/00209182, 2014/3/27   问题单号:DTS2014032809465 */
    mrsRemoteUpgFree(pstMrsUpgCtx);
/* END:   Modified by fengxiaomin/00209182, 2014/3/27 */
    mrsRemoteUpgReset(pstMrsUpgCtx);
    
    mrsDfxMrsEndUpgFileTime();
    mrsDfxRefreshUpgStatus(MRS_DFX_CCO_UPG_TEN_MINUTE_EXIT_STATUS);

    return;
}

HI_BOOL mrsIsCcoUpgStatus(HI_VOID)
{
    MRS_REMOTE_UPG_MODULE * pstMrsUpgCtx = mrsGetRemoteUpgCtx();

    if (pstMrsUpgCtx->ucUpgPeriod != MRS_CCO_UPGRADE_IDLE_PERIOD)
    {
        return HI_TRUE;
    }

    return HI_FALSE;
}

HI_VOID mrsStartQueryUpgStatus(HI_U8 ucPowerFlag, MRS_REMOTE_UPG_MODULE * pstMrsUpgCtx)
{
    HI_U32 ret = HI_ERR_SUCCESS;

    mrsRemoteUpgStatus();
    if (ucPowerFlag)
    {
        ret = MRS_StartTimer(MRS_CCO_TIMER_UPG_STATUS_QUERY_TIMEOUT, 
                            MRS_CCO_UPG_STATUS_QUERY_TIME_OUT * 1000,
                            HI_SYS_TIMER_ONESHOT);
        if (ret != HI_ERR_SUCCESS)
        {
            mrsDfxSendMsgFail(MRS_DFX_CCO_UPG_STATUS_TIMEOUT_ID, (HI_U16)ret);
        }
        
        pstMrsUpgCtx->ucCurrStatusMsgTimes++;
    }
    
    ret = MRS_StartTimer(MRS_CCO_TIMER_UPG_STATUS_QUERY_ACTIVE, 
                        pstMrsUpgCtx->stUpgParam.usMrsUpgStatusQueryTime * 1000,
                        HI_SYS_TIMER_PERIODIC);

    if (ret != HI_ERR_SUCCESS)
    {
        mrsDfxSendMsgFail(MRS_DFX_CCO_UPG_QUERY_STATUS_TIMER_ID, (HI_U16)ret);
    }
    
    return;
}

HI_VOID mrsPowerOnStatusTimeOut(HI_VOID)
{
    MRS_REMOTE_UPG_MODULE * pstMrsUpgCtx = mrsGetRemoteUpgCtx();
    
    HI_DIAG_LOG_MSG_E1(MRS_FILE_LOG_FLAG_021, HI_DIAG_MT("mrsPowerOnStatusTimeOut"),
        pstMrsUpgCtx->ucCurrStatusMsgTimes);
    if (pstMrsUpgCtx->ucCurrStatusMsgTimes < pstMrsUpgCtx->stUpgParam.ucPowerOnStatusMaxTimes)
    {
        mrsStartQueryUpgStatus(HI_TRUE, pstMrsUpgCtx);
    }
    else
    {
        MRS_StopTimer(MRS_CCO_TIMER_UPG_STATUS_QUERY_ACTIVE);
    }

    return;
}

HI_BOOL mrsIsAllowCcoReset(HI_VOID)
{
    MRS_REMOTE_UPG_MODULE * pstMrsUpgCtx = mrsGetRemoteUpgCtx();

    if (MRS_CCO_UPGRADE_LOCAL_UPG_PERIOD == pstMrsUpgCtx->ucUpgPeriod)
    {
        return HI_FALSE;
    }

    return HI_TRUE;
}

HI_U32 mrsRemoteUpgFrameRxPro(HI_U8 *pContent, HI_U32 ulSeg, HI_U16 usTotalSegNum)
{
    HI_U32 ret = HI_ERR_SUCCESS;
    
    if (0 == ulSeg)
    {
        ret = mrsRemoteUpgStartFramePro(pContent);
    }
    else if (ulSeg < (HI_U32)(usTotalSegNum - 1))
    {
        ret = mrsRemoteUpgMiddleFramePro(pContent);
    }
    else
    {
        ret = mrsRemoteUpgEndFramePro(pContent);
    }

    return ret;
}


HI_U32 mrsRemoteUpgFileCheckCrc(HI_U8* buffer, HI_U32 len)
{
    // TODO:功能待实现
    /* BEGIN: Added by w00190206, 2013/5/23   PN:112233*/
    HI_U32 ret_crc = 0;
    HI_U32 aulRetSHA256[8] = {0};
    HI_U32 ret, i;
    HI_U8  md[32] = {0};
    HI_CHAR szManuCode[2] = {0};

    if (buffer == HI_NULL)
    {
        return HI_ERR_INVALID_PARAMETER;
    }

    /* 对buffer偏移datetime后数据进行crc校验 */
    ret_crc = HI_MDM_Crc32(0, (HI_U8*)buffer + HI_FIELDOFFSET(HI_UPG_FILE_HEADER_S, szDateTime),
        len -HI_FIELDOFFSET(HI_UPG_FILE_HEADER_S, szDateTime));
	ret_crc = HI_MDM_Crc32(ret_crc, (const HI_U8 *)buffer, 4);  /* 增加ulMagic的crc校验 */
    if (ret_crc != ((HI_UPG_FILE_HEADER_S *)buffer)->ulChecksum)
    {
        HI_DIAG_LOG_MSG_E2(__LINE__, HI_DIAG_MT("[ERR]HEAD CHECK"),ret_crc,((HI_UPG_FILE_HEADER_S *)buffer)->ulChecksum);
        return HI_ERR_FAILURE;
    }

    /* 判断是否需要作SHA256校验，老版本szManuCode为0不用作，全网升级文件ucFmtVer为0xA1，不用作 */
    ret = (HI_U32)memcmp((HI_CHAR*)(((HI_UPG_FILE_HEADER_S*)buffer)->szManuCode), szManuCode, 2);
    if((ret)&&(((HI_UPG_FILE_HEADER_S*)buffer)->ucFmtVer != HI_UPGA_FILE_FMT_VER_BUPG))
    {
        HI_MDM_HASH_Sha256((HI_U8*)(buffer + sizeof(HI_UPG_FILE_HEADER_S)), len - sizeof(HI_UPG_FILE_HEADER_S), md);
        for(i = 0; i<8; i++)
        {
            aulRetSHA256[i] = (md[4*i]<<24 | md[4*i+1]<<16 | md[4*i+2]<<8 | md[4*i+3]);
        }
        ret = (HI_U32)memcmp(aulRetSHA256, (HI_U32 *)(((HI_UPG_FILE_HEADER_S *)buffer)->aulSHA256), sizeof(aulRetSHA256));
        if(ret)
        {
            HI_DIAG_LOG_MSG_E1(__LINE__, HI_DIAG_MT("[ERR]HEAD CRC 256 CHECK"),aulRetSHA256);
            return HI_ERR_FAILURE;
        }
    }


    /* END:   Added by w00190206, 2013/5/23 */
    return HI_ERR_SUCCESS;
}


HI_U32 mrsRemoteUpgCheckHead(HI_IN HI_U32 ulOpt, HI_IN HI_PVOID pBuff)
{
    HI_U32 ulRet = HI_ERR_FAILURE;
    HI_UPG_FILE_HEADER_S* pstHead = (HI_UPG_FILE_HEADER_S*)pBuff;
    HI_U32 i;
    
    if (pBuff == HI_NULL)
    {
        return HI_ERR_INVALID_PARAMETER;
    }
    
    if (pstHead->ucNumberOfCfgSections != 0)
    {
        HI_U8 ucCcoSecNum = 0;
        HI_U8 ucStaSecNum = 0;

        for (i = 0; i < pstHead->ucNumberOfCfgSections; i++)
        {
            //计算cco个数
            if ((pstHead->stSecCfg.astCfg[i].ulId >= HI_FTM_PRODUCT_TYPE_C01)&&(pstHead->stSecCfg.astCfg[i].ulId <= HI_FTM_PRODUCT_TYPE_C18))
            {
                ucCcoSecNum ++;
            }
            //计算any个数
            else if ((pstHead->stSecCfg.astCfg[i].ulId == HI_FTM_PRODUCT_TYPE_ANY)
                || ((pstHead->stSecCfg.astCfg[i].ulId >= HI_FTM_PRODUCT_TYPE_M101)&&(pstHead->stSecCfg.astCfg[i].ulId <= HI_FTM_PRODUCT_TYPE_M108))//STA CLTI
                || ((pstHead->stSecCfg.astCfg[i].ulId >= HI_FTM_PRODUCT_TYPE_M111)&&(pstHead->stSecCfg.astCfg[i].ulId <= HI_FTM_PRODUCT_TYPE_M118))// CLTII
                || ((pstHead->stSecCfg.astCfg[i].ulId >= HI_FTM_PRODUCT_TYPE_T31)&&(pstHead->stSecCfg.astCfg[i].ulId <= HI_FTM_PRODUCT_TYPE_T34)))// STA_T
            {
                ucStaSecNum ++;
            } 

        }
        
        //全网cco sta 升级场景
        //判断规则 :cco和any个数大于等于1
        if (ulOpt & HI_UPG_REQ_OPTION_BROADCAST)
        {
            if ((ucCcoSecNum >= 1)&&(ucStaSecNum >= 1)&&(pstHead->ucFmtVer == HI_UPGA_FILE_FMT_VER_BUPG))
            {
                return HI_ERR_SUCCESS;
            }        
        }
        //全网sta/stanv升级场景
        //判断规则 :any个数大于等于1且无cco section
        else if (ulOpt & HI_UPG_REQ_OPTION_STA)
        {
            if ((ucCcoSecNum == 0)&&(ucStaSecNum >= 1))
            {
                return HI_ERR_SUCCESS;
            }
        }
        //cco /cconv单点升级
        //判断规则 :cco/any个数大于等于1且无其他section
        else if (ulOpt & HI_UPG_REQ_OPTION_CCO)
        {
            if ((ucCcoSecNum >= 1)&&(ucStaSecNum == 0))
            {
                return HI_ERR_SUCCESS;
            }
        }
        else
        {
            return HI_ERR_FAILURE;
        }
        
        
    }
    else /*2. boot 升级场景*/
    {
        for (i = 0; i < pstHead->ucNumberOfCodeSections; i++)
        {
            //判断规则boot :code section含有boot ID,则为boot升级文件
            if (pstHead->stSecCode.astCfg[i].ulId == HI_UPG_FILE_SECTION_ID_BOOT_ANY)
            {
                if (0 != (ulOpt & (HI_UPG_REQ_OPTION_CCO|HI_UPG_REQ_OPTION_BROADCAST|HI_UPG_REQ_OPTION_STA|HI_UPG_REQ_OPTION_UNICAST)))
                {
                    return HI_ERR_SUCCESS;
                }
            }
        }    
    }

    return ulRet;
}

HI_U32 mrsUpgFileCheck(MRS_REMOTE_UPG_MODULE * pstMrsUpgCtx)
{
    HI_U32 ulRet = HI_ERR_FAILURE;
    HI_U8* pbuffer = (HI_U8*)(pstMrsUpgCtx->pstUpgFileInfo) + sizeof(HI_MDM_UPG_START_REQ_S);
    HI_U32 ulFilelen = pstMrsUpgCtx->ulUpgradeFileLen;
    
    //小于文件头，认为非海思文件
    if (ulFilelen <= HI_MDM_UPG_FILE_MIN_SIZE)
    {
        HI_DIAG_LOG_MSG_E1(__LINE__, HI_DIAG_MT("[INFO]MIN LOAD"),ulFilelen);
        return HI_ERR_SUCCESS;
    }

    ulRet = mrsRemoteUpgFileCheckCrc(pbuffer, ulFilelen);
    if (ulRet != HI_ERR_SUCCESS)//非海思文件，默认发给UPG继续升级
    {
        HI_DIAG_LOG_MSG_E0(__LINE__, HI_DIAG_MT("[INFO] LOAD"));
        return HI_ERR_SUCCESS;
    }

    ulRet = mrsRemoteUpgCheckHead(mrsGetUpgType(pstMrsUpgCtx->ucUpgFileFlag), pbuffer);
    if (ulRet != HI_ERR_SUCCESS)
    {
        HI_DIAG_LOG_MSG_E1(__LINE__, HI_DIAG_MT("[ERR]FILE HEAD CHECK"),ulRet);

        ulRet = HI_ERR_FAILURE;
    }
    
    return ulRet;

}

#endif // CCO
/* END:   Added by fengxiaomin/00209182, 2014/1/16 */

