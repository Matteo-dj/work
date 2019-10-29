/********************************************************************************
Copyright (C), Huawei Technologies Co., Ltd. 2012-2020. All rights reserved.
*********************************************************************************
File Name     : mrs_srv_ndm_collectdata.c
Version       : Initial draft
Author        : HCT
Created Date  : 2016-10-13
Last Modified by: c00233580
Description   : TODO: ...
Function List : TODO: ...
Change History: TODO: ...
********************************************************************************/
#include <hi_types.h>
#include <hi_mdm_types.h>
#if defined(PRODUCT_CFG_PRODUCT_TYPE_NDM)

#ifdef PRODUCT_CFG_SUPPORT_CCT3911
#include <stdio.h>
#include <stdlib.h>

#include "hi_mdm_time.h"

#include "mrs_misc.h"
#include "mrs_chan.h"
#include "mrs_bso.h"
#include "mrs_sdsave.h"
#endif

#include "mrs_srv_ndm.h"
#include "drv_dfx.h"
#include "hi_mdm_datacollect.h"
#include "hi_share_memory.h"
#include "hi_mdm_mac.h"
#include "hi_rtc.h"
#include "hi_filemanager.h"
#include "mrs_srv_ndm_collectdata.h"
#include <hi_lcd.h>
HI_START_HEADER
HI_NDM_DATA_COLLECT_STATUS_MSG_S g_mrsNdmDcSdCtrl;





HI_U32 mrsNdmDcMsgProc(HI_U16 usId, HI_PVOID pMsgParam)
{
    HI_U32 * pRcvParam = (HI_U32*)pMsgParam;
    HI_U32 ulRet = HI_ERR_NOT_SUPPORT_ID;

    switch (usId)
    {
        case EN_NDM_MSG_DC_SD_START:
            ulRet = mrsNdmDcSdStart();
            break;

        case EN_NDM_MSG_DC_SD_NORMAL_TRIGER_MONITOR:
            ulRet = mrsNdmDcSdNormalTrigerPbMonitor();
            break;

        case EN_NDM_MSG_DC_SD_NORMAL_PERIOD:
            ulRet = mrsNdmDcSdNromalPeriodCollect();
            break;

        case EN_NDM_MSG_DC_SD_ZCROSS:
            ulRet = mrsNdmDcSdZcrossCollect();
            break;

        case EN_NDM_MSG_DC_SD_FOLLOW_TRIGER_MONITOR:
            ulRet = mrsNdmDcSdFollowTrigerPbMonitor();
            break;

        case EN_NDM_MSG_DC_SD_FOLLOW_PERIOD_MONITOR:
            ulRet = mrsNdmDcSdFollowPeriodMonitor();
            break;

        case EN_NDM_MSG_DC_STOP:
            ulRet = mrsNdmDcSdStopMsgHandle(usId, pMsgParam);
            break;

        case EN_NDM_MSG_DC_REBOOT_START:
            ulRet = mrsNdmDcSdRebootStart();
            break;

        case EN_NDM_MSG_DC_CHECK_TIME:
            ulRet = mrsNdmDcSdRtcSetTime();
            break;

        default:
            break;
    }

    if (pRcvParam[2] && pRcvParam[0])
    {
        mrsToolsFree(pRcvParam[0]);
    }

    return ulRet;
}

HI_U32 APP_DcMsgProc(HI_SYS_QUEUE_MSG_S *pstMsg)
{
    return mrsNdmDcMsgProc((HI_U16)pstMsg->ulParam[0], (HI_PVOID)(&pstMsg->ulParam[1]));
}


HI_U32 mrsNdmDcSendSubMsg(HI_U32 ulMsgId, HI_PVOID pData, HI_U32 ulDataSize, HI_U32 ulOption)
{
    HI_PVOID pTmpData = HI_NULL;
    HI_U32 ulRet = HI_ERR_SUCCESS;

    if (ulDataSize && HI_NULL != pData)
    {
        pTmpData = mrsToolsMalloc(ulDataSize);
        if (HI_NULL == pTmpData)
        {
            return HI_ERR_NOT_ENOUGH_MEMORY;
        }

        (hi_void)memcpy_s(pTmpData, ulDataSize,pData, (unsigned int)ulDataSize);
    }

    ulRet = APP_NdmMessageSend(MRS_NDM_MSG_Q_1,EN_NDM_MSG_DATA_COLLECT, ulMsgId, (uintptr_t)pTmpData, ulOption, ulDataSize);
    if (HI_ERR_SUCCESS != ulRet)
    {
        mrsToolsFree(pTmpData);
    }

    return ulRet;
}


// HSO�����в�ѯSD������״̬
HI_U32 mrsNdmMsgDcSdReportStat(HI_U16 usId, HI_PVOID pMsgParam)
{
    HI_U32 ulRet = HI_ERR_SUCCESS;

    HI_UNREF_PARAM(usId);
    HI_UNREF_PARAM(pMsgParam);

    ulRet = HI_MDM_ReportPacket(ID_DIAG_CMD_NDM_SD_DC_QUERY_STAT, HI_DIAG_CMD_INSTANCE_LOCAL,
        (HI_PBYTE)(&g_mrsNdmDcSdCtrl), (HI_U16)sizeof(g_mrsNdmDcSdCtrl), HI_TRUE);

    return ulRet;
}




HI_VOID mrsNdmDcKeyMsgProc(HI_BOOL bIsKeyDown, HI_U32 uMs)
{
    HI_U32 ulRet = HI_ERR_SUCCESS;
    MRS_DC_SD_MAIN_CTRL_CTX_STRU * pstMainCtrlCtx = MRS_NDM_DC_SD_GET_MAIN_CTRL_CTX();
    MRS_DC_SD_CHANGE_PHASE_REASON_E eTrigerReason =
        bIsKeyDown ? MRS_DC_SD_CHANGE_PHASE_REASON_KEY_DOWN : MRS_DC_SD_CHANGE_PHASE_REASON_KEY_UP;

    ulRet = mrsNdmDcSdChangeMainPhase(eTrigerReason, uMs);
    if (HI_ERR_SUCCESS != ulRet)
    {
        return;
    }

    switch (pstMainCtrlCtx->ucMainPhase)
    {
        case MRS_DC_SD_MAIN_PHASE_STARTING_PROMPT_ZCROSS_UP: // ��ʾ�����ز���
            mrsNdmDcSdLedDisplay(MRS_DC_SD_DISPLAY_DC_ZERO_CROSS_UP);
            break;
        case MRS_DC_SD_MAIN_PHASE_STARTING_PROMPT_ZCROSS_DOWN: // ��ʾ�½��ز���
            mrsNdmDcSdLedDisplay(MRS_DC_SD_DISPLAY_DC_ZERO_CROSS_DOWN);
            break;
        case MRS_DC_SD_MAIN_PHASE_STARING_NORMAL: // ��ʼ��ͨ����
            mrsNdmDcSdLedDisplay(MRS_DC_SD_DISPLAY_DC_NORMAL);
            mrsNdmDcSdSendStartMsg(MRS_DC_SD_START_TYPE_KEY_NORNAL);
            break;
        case MRS_DC_SD_MAIN_PHASE_STARTING_ZCROSS_UP: // ��ʼ�����ز���
            mrsNdmDcSdSendStartMsg(MRS_DC_SD_START_TYPE_KEY_ZCROSS_UP);
            break;
        case MRS_DC_SD_MAIN_PHASE_STARTING_ZCROSS_DONW: // ��ʼ�½��ز���
            mrsNdmDcSdSendStartMsg(MRS_DC_SD_START_TYPE_KEY_ZCROSS_DOWN);
            break;
        case MRS_DC_SD_MAIN_PHASE_STOPPING: // ��ʼֹͣ����
            mrsNdmDcSdStop(MRS_DC_SD_STOP_TYPE_KEY);
            break;
        default:
            break;
    }
}


// init ctrl context and send msg to start collect data
HI_U32 mrsNdmDcSdSendStartMsg(MRS_DC_SD_START_TYPE_E eStartType)
{
    MRS_DC_SD_MAIN_CTRL_CTX_STRU * pstMainCtrlCtx = MRS_NDM_DC_SD_GET_MAIN_CTRL_CTX();
    MRS_DC_SD_ERR_CTX_STRU * pstErrCtx = MRS_NDM_DC_SD_GET_ERR_CTX();

    // ��ճ��������̿�����Ϣ�����ȫ��������Ϣ
    (hi_void)memset_s(&g_mrsNdmDcSdCtrl.stErrCtx,sizeof(g_mrsNdmDcSdCtrl.stErrCtx), 0,    sizeof(g_mrsNdmDcSdCtrl.stErrCtx));
    (hi_void)memset_s(&g_mrsNdmDcSdCtrl.stConfigCtx,sizeof(g_mrsNdmDcSdCtrl.stConfigCtx) , 0,sizeof(g_mrsNdmDcSdCtrl.stConfigCtx));
    (hi_void)memset_s(&g_mrsNdmDcSdCtrl.stStatCtx,sizeof(g_mrsNdmDcSdCtrl.stStatCtx) , 0,sizeof(g_mrsNdmDcSdCtrl.stStatCtx));
    pstMainCtrlCtx->ucStartType = eStartType;

    if (HI_ERR_SUCCESS != mrsNdmDcSendSubMsg(EN_NDM_MSG_DC_SD_START, HI_NULL, 0, 0))
    {
        pstErrCtx->usMsgSendFailTimes++;
        mrsNdmDcSdLedDisplay(MRS_DC_SD_DISPLAY_DC_NONE);
        pstMainCtrlCtx->ucMainPhase = MRS_DC_SD_MAIN_PHASE_STOPPED;
        return HI_ERR_FAILURE;
    }
    return HI_ERR_SUCCESS;
}

// ��ʼ��������
HI_U32 mrsNdmDcSdStart(HI_VOID)
{
    MRS_DC_SD_ERR_CTX_STRU * pstErrCtx = MRS_NDM_DC_SD_GET_ERR_CTX();
    MRS_DC_SD_MAIN_CTRL_CTX_STRU * pstMainCtrlCtx = MRS_NDM_DC_SD_GET_MAIN_CTRL_CTX();
    MRS_DC_SD_CONFIG_CTX_STRU *pstConfigCtx = MRS_NDM_DC_SD_GET_CONFIG_CTX();
    HI_PVOID pTempStartAddr = HI_NULL;
    MRS_DC_SD_ALERT_TYPE_E eSdAlertType = MRS_DC_SD_ALERT_TYPE_NONE;

#ifndef PRODUCT_CFG_SUPPORT_CCT3911
    // 1.SD��״̬���
    if(HI_ERR_SUCCESS != mrsNdmDcSdCheckSdCardStat(&eSdAlertType))
    {
        goto fail_1;
    }
#endif

    // 2.��ʼ����������
    if(HI_ERR_SUCCESS != mrsNdmDcSdInitConfig((MRS_DC_SD_START_TYPE_E)pstMainCtrlCtx->ucStartType))
    {
        pstErrCtx->ucNotStartReason = MRS_DC_SD_NOT_START_REASON_NV_READ_FAIL;

        goto fail_1;
    }

    // 3.��ȡ�����ڴ�
    pTempStartAddr = HI_SHARE_MEM_GetMem(MRS_DC_SD_DEFAULT_NV_VALUE_RAM_SIZE);
    if(HI_NULL == (HI_VOID *)pTempStartAddr)
    {
        pstErrCtx->ucNotStartReason = MRS_DC_SD_NOT_START_REASON_MEM_GET_FAIL;

        goto fail_1;
    }
    pstConfigCtx->ulStartAddr = (HI_U32)pTempStartAddr;

    // 4.���NV�����Ƿ���ȷ
    if(HI_ERR_SUCCESS != mrsNdmDcSdCheckConfig())
    {
        pstErrCtx->ucNotStartReason = MRS_DC_SD_NOT_START_REASON_NV_CONFIG_WRONG;

        goto fail_2;
    }

     // 5.����׼��
    if(HI_ERR_SUCCESS != mrsNdmDcSdPrepare())
    {
        pstErrCtx->ucNotStartReason = MRS_DC_SD_NOT_START_REASON_PREPARE_FAIL;

        goto fail_2;
    }

     // 6.������Ϣ����Уʱ
    if(HI_ERR_SUCCESS != mrsNdmDcSendSubMsg(EN_NDM_MSG_DC_CHECK_TIME, HI_NULL, 0, 0))
    {
        pstErrCtx->usMsgSendFailTimes++;
        pstErrCtx->ucNotStartReason = MRS_DC_SD_NOT_START_REASON_SEND_CHECK_TIME_FAIL;

        goto fail_3;
    }

    // 7.����NV״̬
    pstConfigCtx->usNameId++;
    if(HI_ERR_SUCCESS != mrsNdmDcSdWriteConfigToNv(pstConfigCtx->ucIsSetRebootStart, pstConfigCtx->usNameId))
    {
        pstErrCtx->ucNotStartReason = MRS_DC_SD_NOT_START_REASON_NV_WRITE_FAIL;

        goto fail_3;
    }

    // 8.��ʼ����:a.�ò�����־b.�л�LEDģʽc.����������ʱ��,��Ϣ
    pstMainCtrlCtx->ucMainPhase = MRS_DC_SD_MAIN_PHASE_STARTED;

    // �澯״̬����
    mrsNdmDcSdUpdateSdCardLedAlert(eSdAlertType);

    mrsNdmDcSdCollect();

    return HI_ERR_SUCCESS;

fail_3:
    mrsNdmDcSdRestore(); // �����ָ�

fail_2:
    HI_SHARE_MEM_FreeMem((HI_PVOID)(pstConfigCtx->ulStartAddr));
    pstConfigCtx->ulStartAddr = (HI_U32)HI_NULL;
fail_1:
    mrsNdmDcSdUpdateSdCardLedAlert(eSdAlertType);

    mrsNdmDcSdLedDisplay(MRS_DC_SD_DISPLAY_DC_NONE);
    pstMainCtrlCtx->ucMainPhase = MRS_DC_SD_MAIN_PHASE_STOPPED;

    return HI_ERR_FAILURE;
}


// ����׼��: ���ݲ������ͣ�����׼���ӿ�
HI_U32 mrsNdmDcSdPrepare(HI_VOID)//can_modify_support_follow_mode
{
    return HI_ERR_SUCCESS;
}


// ���в���
HI_U32 mrsNdmDcSdCollect(HI_VOID)
{
    MRS_DC_SD_MAIN_CTRL_CTX_STRU * pstMainCtrlCtx = MRS_NDM_DC_SD_GET_MAIN_CTRL_CTX();
    MRS_DC_SD_CONFIG_CTX_STRU * pstConfigCtx = MRS_NDM_DC_SD_GET_CONFIG_CTX();
    MRS_DC_SD_STAT_CTX_STRU * pstStatCtx = MRS_NDM_DC_SD_GET_STAT_CTX();

    if (MRS_DC_SD_MAIN_PHASE_STARTED != pstMainCtrlCtx->ucMainPhase)
    {
        return HI_ERR_FAILURE;
    }

    if (MRS_DC_SD_START_TYPE_KEY_ZCROSS_UP == pstMainCtrlCtx->ucStartType ||
        MRS_DC_SD_START_TYPE_KEY_ZCROSS_DOWN == pstMainCtrlCtx->ucStartType)
    {
        (HI_VOID)mrsNdmDcSdZcrossCollect();
    }
    else if (pstConfigCtx->ucIsFollowMode)
    {
        // ��ʼ��ʱ����������������ѯ��ʱ��
        pstStatCtx->ulLastRcvPbTimeStamp = HI_MDM_GetSeconds();
        pstStatCtx->ulCurPeriodStartTime = pstStatCtx->ulLastRcvPbTimeStamp;
        APP_NdmTimerStart(MRS_NDM_TIMER_DC_SD_FOLLOW, MRS_NDM_TIME_DC_SD_FOLLOW, HI_SYS_TIMER_PERIODIC);

    }
    else
    {
        // ������Ϣ������ͨ���ڲ�
        mrsNdmDcSdSendPeriodCollectMsg();

        // ��ʼ��ʱ�����������ͨ�����ɼ��Ӷ�ʱ��,
        pstStatCtx->ulLastRcvPbTimeStamp = HI_MDM_GetSeconds();
        APP_NdmTimerStart(MRS_NDM_TIMER_DC_SD_PB_MONITR, MRS_NDM_TIME_DC_SD_PB_MONITR, HI_SYS_TIMER_PERIODIC);

    }

    return HI_ERR_SUCCESS;
}


// �����ָ�
HI_U32 mrsNdmDcSdRestore(HI_VOID)//can_modify_support_follow_mode
{
    return HI_ERR_SUCCESS;
}


// ֹͣ����: 1.�޸�״̬ 2.�ȴ��������ʱ�ͷ���Դ
HI_U32 mrsNdmDcSdStop(MRS_DC_SD_STOP_TYPE_E eStopType)
{
    MRS_DC_SD_ERR_CTX_STRU * pstErrCtx = MRS_NDM_DC_SD_GET_ERR_CTX();

    mrsNdmDcSdStopPhase1(eStopType);

    // ֹͣ�ڶ��׶η��ڲ�������,��֤����δ���ʱ��Դ�����ͷ�
    if (HI_ERR_SUCCESS != mrsNdmDcSendSubMsg(EN_NDM_MSG_DC_STOP, (HI_PVOID)&eStopType, sizeof(eStopType), 0))
    {
        pstErrCtx->usMsgSendFailTimes++;
        mrsNdmDcSdStopPhase2(eStopType);
    }
    return HI_ERR_SUCCESS;
}


// ֹͣ�������̵�һ�׶�
HI_U32 mrsNdmDcSdStopPhase1(MRS_DC_SD_STOP_TYPE_E eStopType)
{
    MRS_DC_SD_CONFIG_CTX_STRU* pstConfigCtx = MRS_NDM_DC_SD_GET_CONFIG_CTX();
    MRS_DC_SD_ERR_CTX_STRU * pstErrCtx = MRS_NDM_DC_SD_GET_ERR_CTX();

    // ֹͣ������PB������ʱ��
    if (pstConfigCtx->ucIsFollowMode)
    {
        APP_NdmTimerStop(MRS_NDM_TIMER_DC_SD_FOLLOW);
    }
    else
    {
        APP_NdmTimerStop(MRS_NDM_TIMER_DC_SD_PB_MONITR);
    }

    // 3.�л�LEDΪ����ģʽ
    mrsNdmDcSdLedDisplay(MRS_DC_SD_DISPLAY_DC_NONE);
    if(MRS_DC_SD_STOP_TYPE_KEY == eStopType || MRS_DC_SD_STOP_TYPE_COMPLETE == eStopType)
    {
        mrsNdmDcSdLedDisplay(MRS_DC_SD_DISPLAY_ALERT_NONE);
    }
    else if(MRS_DC_SD_STOP_TYPE_SD_CARD_ERR_TIMES == eStopType)
    {
        mrsNdmDcSdLedDisplay(MRS_DC_SD_DISPLAY_ALERT_SD_ERR);
    }
    else
    {
        //do nothing
    }

    // 4.��¼����ֹͣԭ��
    pstErrCtx->ucStopReason = (HI_U8)eStopType;

    return HI_ERR_SUCCESS;
}


// ֹͣ�������̵ڶ��׶�
HI_U32 mrsNdmDcSdStopPhase2(MRS_DC_SD_STOP_TYPE_E eStopType)
{
    HI_U32 ulRet = HI_ERR_SUCCESS;
    MRS_DC_SD_MAIN_CTRL_CTX_STRU * pstMainCtrlCtx = MRS_NDM_DC_SD_GET_MAIN_CTRL_CTX();
    MRS_DC_SD_CONFIG_CTX_STRU* pstConfigCtx = MRS_NDM_DC_SD_GET_CONFIG_CTX();

    // �˳���������
    mrsNdmDcSdRestore();

    // �л�LEDΪ����ģʽ:һ�׶οصƼ�ʱ��Ӧ�����׶οصƷ�ֹ�ڲ��������еƱ����޸�
    mrsNdmDcSdLedDisplay(MRS_DC_SD_DISPLAY_DC_NONE);

    // �ͷŹ����ڴ�
    HI_SHARE_MEM_FreeMem((HI_PVOID)(pstConfigCtx->ulStartAddr));
    pstConfigCtx->ulStartAddr = (HI_U32)HI_NULL;

    ulRet += mrsNdmDcSdWriteConfigToNv(HI_FALSE, pstConfigCtx->usNameId);

    pstMainCtrlCtx->ucMainPhase = MRS_DC_SD_MAIN_PHASE_STOPPED;

    if(MRS_DC_SD_STOP_TYPE_KEY == eStopType || MRS_DC_SD_STOP_TYPE_COMPLETE == eStopType)
    {
        mrsNdmDcSdLedDisplay(MRS_DC_SD_DISPLAY_ALERT_NONE);
    }
    else if(MRS_DC_SD_STOP_TYPE_SD_CARD_ERR_TIMES == eStopType)
    {
        mrsNdmDcSdLedDisplay(MRS_DC_SD_DISPLAY_ALERT_SD_ERR);
    }
    else
    {
        //do nothing
    }
#ifndef PRODUCT_CFG_SUPPORT_CCT3911
    HI_MDM_SdCardSync();
#endif

    return ulRet;
}


// ����ֹͣ������Ϣ
HI_U32 mrsNdmDcSdStopMsgHandle(HI_U16 usId, HI_PVOID pMsgParam)
{
    HI_U32* pRecvMsgParam = (HI_U32*)pMsgParam;
    MRS_DC_SD_STOP_TYPE_E eStopType;

    HI_UNREF_PARAM(usId);

    if (sizeof(eStopType) != pRecvMsgParam[2])
    {
        return HI_ERR_BAD_DATA;
    }

    (hi_void)memcpy_s(&eStopType, sizeof(eStopType),(HI_PVOID)pRecvMsgParam[0], sizeof(eStopType));

    return mrsNdmDcSdStopPhase2(eStopType);
}


// ��ͨ��: �ɼ�����
HI_U32 mrsNdmDcSdNormalCollect(DC_TRIG_TYPE_ENUM eCollectType, MRS_DC_SD_SAVE_FILE_TYPE_E eSaveFileType)
{
    HI_U32 ulErrCode = HI_ERR_SUCCESS; // ���������Ҫ���뵽�洢���ļ�����
    DC_INPUT_PARA_STRU stDcInParam;
    HI_BUF_S stBuf;

    (hi_void)memset_s(&stDcInParam, sizeof(stDcInParam), 0, sizeof(stDcInParam));
    (hi_void)memset_s(&stBuf, sizeof(stBuf), 0, sizeof(stBuf));

    mrsNdmDcGenerateInputParam(eCollectType, &stDcInParam);

    ulErrCode = HI_DC_StartNormalDataCollect(&stDcInParam, &stBuf);

    if (HI_ERR_SUCCESS != ulErrCode)
    {
        (hi_void)memset_s(&stBuf, sizeof(stBuf), 0, sizeof(stBuf));
    }

#ifdef PRODUCT_CFG_SUPPORT_CCT3911
    // �洢�������
    printf("COLLECT_SAVE AgcGain[%d]: [%u][%u][%u][%u][%u]\n",
        stDcInParam.lAgcGain,stBuf.bufAddr[0],stBuf.bufAddr[1],
        stBuf.bufSize[0],stBuf.bufSize[1],ulErrCode);
    
    HI_CHAR scFileName[MRS_DC_SD_TOTAL_PATH_NAME_LEN + 1];
    HI_U32 ulPathNameLen = MRS_DC_SD_TOTAL_PATH_NAME_LEN;

    mrsNdmDcSdGenerateFileName(eSaveFileType, ulErrCode, scFileName, &ulPathNameLen);
    printf("[%s %u]: FileName: %s\r\n",__func__,__LINE__,scFileName);

    if (stBuf.bufSize[0] != 0)
    {
        mrs_clt_info_t clt_info;

        if (Mrs_Misc_Collect_StartType_Get() == MRS_DC_SD_START_TYPE_REBOOT)
        {
            clt_info.clt_type = CLT_TYPE_REBOOT;
        }
        else
        {
            clt_info.clt_type = CLT_TYPE_SAVE;
        }
        clt_info.file_type = eSaveFileType;
        clt_info.hi_buf = (void *)&stBuf;
        
        Mrs_Sdsave_FileWrite(BSO_TYPE_BDCOLLECT, &clt_info);
    }
#else
    // �洢�������
    mrsNdmDcSdSaveFile(&stBuf, eSaveFileType, ulErrCode);
#endif

    return HI_ERR_SUCCESS;
}


// ��ͨ���ڲ�: �������ڲ���Ϣ
HI_U32 mrsNdmDcSdSendPeriodCollectMsg(HI_VOID)
{
    MRS_DC_SD_CONFIG_CTX_STRU * pstConfigCtx = MRS_NDM_DC_SD_GET_CONFIG_CTX();
    MRS_DC_SD_ERR_CTX_STRU * pstErrCtx = MRS_NDM_DC_SD_GET_ERR_CTX();

    if (0 == (pstConfigCtx->usTrigerType & MRS_DC_SD_DATA_COLLECT_TYPE_DEFAULT))
    {
        return HI_ERR_FAILURE;
    }

    if (HI_ERR_SUCCESS != mrsNdmDcSendSubMsg(EN_NDM_MSG_DC_SD_NORMAL_PERIOD, HI_NULL, 0, 0))
    {
        pstErrCtx->usMsgSendFailTimes++;
        return APP_NdmTimerStart(MRS_NDM_TIMER_DC_SD_PERIOD, MRS_DC_SD_NORMAL_PERIOD_RETRY_TIME, HI_SYS_TIMER_ONESHOT);
    }

    return HI_ERR_SUCCESS;
}


// ��ͨ���ڲ�: �ɼ�����
HI_U32 mrsNdmDcSdNromalPeriodCollect(HI_VOID)
{
    MRS_DC_SD_MAIN_CTRL_CTX_STRU * pstMainCtrlCtx = MRS_NDM_DC_SD_GET_MAIN_CTRL_CTX();
    MRS_DC_SD_CONFIG_CTX_STRU * pstConfigCtx = MRS_NDM_DC_SD_GET_CONFIG_CTX();
    MRS_DC_SD_STAT_CTX_STRU * pstStatCtx = MRS_NDM_DC_SD_GET_STAT_CTX();

    if (MRS_DC_SD_MAIN_PHASE_STARTED != pstMainCtrlCtx->ucMainPhase)
    {
        return HI_ERR_FAILURE;
    }

    if (pstConfigCtx->usTrigerType & MRS_DC_SD_DATA_COLLECT_TYPE_DEFAULT)
    {
        mrsNdmDcSdNormalCollect(EN_TRIG_TYPE_NOISE, MRS_DC_SD_SAVE_FILE_TYPE_D);
    }

    // �жϱ������Ƿ�ɹ�����
    if (pstStatCtx->usFileCnt_D % pstConfigCtx->ucTimesPerPeriod != 0)
    {
        return mrsNdmDcSdSendPeriodCollectMsg();
    }
    else
    {
        return APP_NdmTimerStart(MRS_NDM_TIMER_DC_SD_PERIOD, pstConfigCtx->usPeriodTime * MS_PER_MINUTES, HI_SYS_TIMER_ONESHOT);
    }
}


// ��ͨ������: ��ѯ��ʱ��������
HI_U32 mrsNdmDcSdNormalTrigerTimeout(HI_VOID)
{
    MRS_DC_SD_CONFIG_CTX_STRU * pstConfigCtx = MRS_NDM_DC_SD_GET_CONFIG_CTX();
    MRS_DC_SD_ERR_CTX_STRU * pstErrCtx = MRS_NDM_DC_SD_GET_ERR_CTX();

    if (0 == (pstConfigCtx->usTrigerType & MRS_DC_SD_DATA_COLLECT_TYPE_TRI_ALL))
    {
        return HI_ERR_FAILURE;
    }

    if (HI_ERR_SUCCESS != mrsNdmDcSendSubMsg(EN_NDM_MSG_DC_SD_NORMAL_TRIGER_MONITOR, HI_NULL, 0, 0))
    {
        pstErrCtx->usMsgSendFailTimes++;
        return HI_ERR_FAILURE;
    }

    return HI_ERR_SUCCESS;
}


// ��ͨ������: �����յ�PB��״̬
HI_U32 mrsNdmDcSdNormalTrigerPbMonitor(HI_VOID)
{
    static HI_U32 ulLastPbCount = 0;
    HI_U32 ulCurPbCount = HI_MAC_GetRxPbStatistic();
    HI_U32 ulCurTimeSec = HI_MDM_GetSeconds();
    MRS_DC_SD_STAT_CTX_STRU * pstStatCtx = MRS_NDM_DC_SD_GET_STAT_CTX();
    MRS_DC_SD_CONFIG_CTX_STRU * pstConfigCtx = MRS_NDM_DC_SD_GET_CONFIG_CTX();
    MRS_DC_SD_MAIN_CTRL_CTX_STRU * pstMainCtrlCtx = MRS_NDM_DC_SD_GET_MAIN_CTRL_CTX();

    if (MRS_DC_SD_MAIN_PHASE_STARTED != pstMainCtrlCtx->ucMainPhase)
    {
        return HI_ERR_FAILURE;
    }

    if (ulCurPbCount != ulLastPbCount)
    {
        pstStatCtx->ulLastRcvPbTimeStamp = HI_MDM_GetSeconds();
    }
    else if (ulCurTimeSec - pstStatCtx->ulLastRcvPbTimeStamp >= (HI_U32)pstConfigCtx->usTrigerTime * SEC_PER_MINUTE)
    {
        mrsNdmDcSdTrigerCollect();
        pstStatCtx->ulLastRcvPbTimeStamp = HI_MDM_GetSeconds();
    }

    ulLastPbCount = ulCurPbCount;

    return HI_ERR_SUCCESS;
}


// ��ͨ������: �ɼ�����
HI_U32 mrsNdmDcSdTrigerCollect(HI_VOID)
{
    MRS_DC_SD_CONFIG_CTX_STRU * pstConfigCtx = MRS_NDM_DC_SD_GET_CONFIG_CTX();

    if (pstConfigCtx->usTrigerType & MRS_DC_SD_DATA_COLLECT_TYPE_NOISE)
    {
        mrsNdmDcSdNormalCollect(EN_TRIG_TYPE_NOISE, MRS_DC_SD_SAVE_FILE_TYPE_N);
    }

    if (pstConfigCtx->usTrigerType & MRS_DC_SD_DATA_COLLECT_TYPE_FC)
    {
        mrsNdmDcSdNormalCollect(EN_TRIG_TYPE_FC_START, MRS_DC_SD_SAVE_FILE_TYPE_F);
    }

    if (pstConfigCtx->usTrigerType & MRS_DC_SD_DATA_COLLECT_TYPE_STAT4)
    {
        mrsNdmDcSdNormalCollect(EN_TRIG_TYPE_STAT_FOUR, MRS_DC_SD_SAVE_FILE_TYPE_S);
    }

    return HI_ERR_SUCCESS;
}


// �����: ��ѯ��ʱ������
HI_U32 mrsNdmDcSdFollowTimeout(HI_VOID)
{
    MRS_DC_SD_CONFIG_CTX_STRU * pstConfigCtx = MRS_NDM_DC_SD_GET_CONFIG_CTX();
    MRS_DC_SD_ERR_CTX_STRU * pstErrCtx = MRS_NDM_DC_SD_GET_ERR_CTX();
    HI_U32 ulRet = HI_ERR_SUCCESS;

    if (pstConfigCtx->usTrigerType & MRS_DC_SD_DATA_COLLECT_TYPE_NOISE)
    {
        if (HI_ERR_SUCCESS != mrsNdmDcSendSubMsg(EN_NDM_MSG_DC_SD_FOLLOW_TRIGER_MONITOR, HI_NULL, 0, 0))
        {
            pstErrCtx->usMsgSendFailTimes++;
            ulRet = HI_ERR_FAILURE;
        }
    }

    if (pstConfigCtx->usTrigerType & MRS_DC_SD_DATA_COLLECT_TYPE_DEFAULT)
    {
        if (HI_ERR_SUCCESS != mrsNdmDcSendSubMsg(EN_NDM_MSG_DC_SD_FOLLOW_PERIOD_MONITOR, HI_NULL, 0, 0))
        {
            pstErrCtx->usMsgSendFailTimes++;
            ulRet = HI_ERR_FAILURE;
        }
    }

    return ulRet;
}




// ���津����: �����յ�PB��״̬
HI_U32 mrsNdmDcSdFollowTrigerPbMonitor(HI_VOID)//can_modify_support_follow_mode
{
    return HI_ERR_SUCCESS;
}


// �������ڲ�: ��ѯ��
HI_U32 mrsNdmDcSdFollowPeriodMonitor(HI_VOID)//can_modify_support_follow_mode
{
    return HI_ERR_SUCCESS;
}


// �����: ���й����
HI_U32 mrsNdmDcSdZcrossCollect(HI_VOID)//can_modify_support_follow_mode
{
    return HI_ERR_NOT_SUPPORT;
}


// �����: ֪ͨ������һ�βɼ�
HI_U32 mrsNdmDcSdNotifyDoNextZcrossCollect(HI_VOID)
{
    MRS_DC_SD_ERR_CTX_STRU * pstErrCtx = MRS_NDM_DC_SD_GET_ERR_CTX();

    if (HI_ERR_SUCCESS != mrsNdmDcSendSubMsg(EN_NDM_MSG_DC_SD_ZCROSS, HI_NULL, 0, 0))
    {
        pstErrCtx->usMsgSendFailTimes++;
        APP_NdmTimerStart(MRS_NDM_TIMER_DC_SD_ZCROSS, MRS_DC_SD_ZCROSS_RETRY_TIME, HI_SYS_TIMER_ONESHOT);

        return HI_ERR_FAILURE;
    }

    return HI_ERR_SUCCESS;
}


// �ϵ�����: ����Ƿ���Ҫ�ϵ����ɣ�����Ҫ������ͨ����ѯ��ʱ��
HI_U32 mrsNdmDcSdCheckRebootStart(HI_VOID)
{
    NV_APP_MRS_NDM_DC_SD_PARAM_STRU * pCfgNv = HI_NULL;
    HI_BOOL bIsCollecting = HI_FALSE;

    pCfgNv = mrsToolsMalloc(sizeof(NV_APP_MRS_NDM_DC_SD_PARAM_STRU));
    if (HI_NULL == pCfgNv)
    {
        return HI_ERR_MALLOC_FAILUE;
    }

    if (HI_ERR_SUCCESS != HI_MDM_NV_Read(ID_NV_MRS_DC_SD_INFO, pCfgNv, sizeof(NV_APP_MRS_NDM_DC_SD_PARAM_STRU)))
    {
        mrsToolsFree(pCfgNv);
        return HI_ERR_FAILURE;
    }

    bIsCollecting = pCfgNv->ucCollecting;

    pCfgNv->ucCollecting = HI_FALSE;
    HI_MDM_NV_Write(ID_NV_MRS_DC_SD_INFO, pCfgNv, sizeof(NV_APP_MRS_NDM_DC_SD_PARAM_STRU));

    if (HI_TRUE == pCfgNv->ucCPowerOnEable && HI_TRUE == bIsCollecting)
    {
        APP_NdmTimerStart(MRS_NDM_TIMER_DC_SD_REBOOT_CHECK, MRS_NDM_TIME_DC_SD_REBOOT_CHECK, HI_SYS_TIMER_PERIODIC);
    }

    mrsToolsFree(pCfgNv);
    return HI_ERR_SUCCESS;
}


// �ϵ�����: ��ѯ��ʱ������
HI_U32 mrsNdmDcSdRebootCheckTimeout(HI_VOID)
{
    mrsNdmDcSendSubMsg(EN_NDM_MSG_DC_REBOOT_START, HI_NULL, 0, 0);
    return HI_ERR_SUCCESS;
}


// �ϵ�����: ���жϵ�����
HI_U32 mrsNdmDcSdRebootStart(HI_VOID)
{
    static HI_U32 ulTryTimes = 0;

    // ��ೢ��10��
    if (ulTryTimes >= MRS_DC_SD_REBOOT_START_RETRY_TIMES)
    {
        APP_NdmTimerStop(MRS_NDM_TIMER_DC_SD_REBOOT_CHECK);
        return HI_ERR_FAILURE;
    }
    ulTryTimes++;

    // �ж�����״̬
#ifdef PRODUCT_CFG_SUPPORT_CCT3911
    if (HI_TRUE == Mrs_Misc_GetConnectStat())
#else
    if (HI_TRUE == HI_FMGR_GetConnectStat())
#endif
    {
        if(HI_ERR_SUCCESS == mrsNdmDcSdChangeMainPhase(MRS_DC_SD_CHANGE_PHASE_REASON_REBOOT_START, 0))
        {
            mrsNdmDcSdLedDisplay(MRS_DC_SD_DISPLAY_DC_NORMAL);
            mrsNdmDcSdSendStartMsg(MRS_DC_SD_START_TYPE_REBOOT);
        }

        ulTryTimes = MRS_DC_SD_REBOOT_START_RETRY_TIMES;
    }

    return HI_ERR_SUCCESS;
}


// ���ɲ����������
HI_U32 mrsNdmDcGenerateInputParam(HI_IN DC_TRIG_TYPE_ENUM eCollectType, HI_OUT DC_INPUT_PARA_STRU * pstDcInParam)
{
    MRS_DC_SD_CONFIG_CTX_STRU * pstConfigCtx = MRS_NDM_DC_SD_GET_CONFIG_CTX();

    pstDcInParam->ulStartAddr = pstConfigCtx->ulStartAddr;
    pstDcInParam->ulEndAddr   = pstConfigCtx->ulStartAddr + pstConfigCtx->ulRamSize;
    
#ifdef PRODUCT_CFG_SUPPORT_CCT3911
    pstDcInParam->lAgcGain    = Mrs_Misc_Collect_AgcGain_Get();
#else
    pstDcInParam->lAgcGain    = pstConfigCtx->slAgcGain;
#endif
    pstDcInParam->ulAfterPointlen = MRS_DC_SD_DEFAULT_AFTER_POINT_LEN;
    pstDcInParam->enDataSrc   = EN_TRIG_AD_12_BIT;
    pstDcInParam->enTrigerType = eCollectType;
    pstDcInParam->ulWaitEnvOkTime = 1000;
    pstDcInParam->ulWaitResultTimeOut = 1000;

    if (eCollectType != EN_TRIG_TYPE_NOISE)
    {
        pstDcInParam->lAgcGain = MRS_DC_SD_UN_FIX_AGCGAIN;
    }

    return HI_ERR_SUCCESS;
}


HI_U32 mrsNdmDcSdSaveFile(HI_BUF_S *pstBuf, MRS_DC_SD_SAVE_FILE_TYPE_E eSaveFileType, HI_U32 ulErrCode)
{
    MRS_DC_SD_ERR_CTX_STRU * pstErrCtx = MRS_NDM_DC_SD_GET_ERR_CTX();
    MRS_DC_SD_STAT_CTX_STRU * pstStatCtx = MRS_NDM_DC_SD_GET_STAT_CTX();
    HI_CHAR scFileName[MRS_DC_SD_TOTAL_PATH_NAME_LEN + 1];
    HI_U32 ulPathNameLen = MRS_DC_SD_TOTAL_PATH_NAME_LEN;

    // RTC Уʱ
    if (HI_ERR_SUCCESS != mrsNdmDcSdRtcSetTime())
    {
        pstErrCtx->ucSetRtcFailTimes++;
    }

    if (HI_ERR_SUCCESS == mrsNdmDcSdCheckCreatFolder()) //���Դ洢�ļ����ļ��д���
    {
        mrsNdmDcSdGenerateFileName(eSaveFileType, ulErrCode, scFileName, &ulPathNameLen);
        (HI_VOID)mrsNdmDcSdSaveFileToSdCard(pstBuf, scFileName, ulPathNameLen);
        pstStatCtx->usTotalFileCnt++;
    }
    else
    {
        mrsNdmDcSdGenerateFileName(eSaveFileType, ulErrCode, scFileName, &ulPathNameLen);
    }

    if (pstErrCtx->ucSdCardErrTimes > MRS_DC_SD_SD_CARD_MAX_ERR_TIMES)
    {
        if (HI_ERR_SUCCESS == mrsNdmDcSdChangeMainPhase(MRS_DC_SD_CHANGE_PHASE_REASON_SD_CADR_ERR, 0))
        {
            mrsNdmDcSdStop(MRS_DC_SD_STOP_TYPE_SD_CARD_ERR_TIMES);
        }
    }

    return HI_ERR_SUCCESS;
}


// ��鴴���ļ���
HI_U32 mrsNdmDcSdCheckCreatFolder(HI_VOID)
{
    MRS_DC_SD_CONFIG_CTX_STRU * pstConfigCtx = MRS_NDM_DC_SD_GET_CONFIG_CTX();
    MRS_DC_SD_STAT_CTX_STRU * pstStatCtx = MRS_NDM_DC_SD_GET_STAT_CTX() ;
    MRS_DC_SD_ERR_CTX_STRU * pstErrCtx = MRS_NDM_DC_SD_GET_ERR_CTX();
    HI_U32 ulRet = HI_ERR_SUCCESS;
    HI_CHAR *scPathName = HI_NULL;
    HI_U32 i = 0;
    HI_S32 ret_fd = -1;

    if (pstStatCtx->usTotalFileCnt % MRS_DC_SD_MAX_FILE_COUNT_PER_FOLDER != 0)
    {
        return HI_ERR_SUCCESS;
    }

    // ���� "/sd_dc/DC_XXX/255/"�ַ������ڴ�ռ�
    scPathName = mrsToolsMalloc(MRS_DC_SD_TOTAL_FOLDER_SLANT_LEN + 1);
    if (HI_NULL == scPathName)
    {
        return HI_ERR_NOT_ENOUGH_MEMORY;
    }

    // ������ʼʱcurFolderSequenceΪ0������������curFolderSequenceΪ��ǰ�ļ������
    for (i = pstStatCtx->ucCurFolderSequence; i < MRS_DC_SD_MAX_SEQUENCE_FOLDER_NUM; i++)
    {
        
        (hi_void)snprintf_s(scPathName, MRS_DC_SD_TOTAL_FOLDER_SLANT_LEN + 1,MRS_DC_SD_TOTAL_FOLDER_SLANT_LEN, "%s/%s/%u/",
            MRS_DC_SD_BASE_FOLDER_NAME, pstConfigCtx->scFolderName, i);

        ulRet = HI_MDM_SdCardAccess(scPathName, HI_FMGR_F_OK, &ret_fd);
        if ((ulRet != HI_ERR_SUCCESS) && (ulRet != HI_ERR_FILE_MGR_ACCESS_FAIL)) // �ļ�����ʧ����ʧ��ԭ�����ļ��в�����
        {
            pstErrCtx->ucSdCardAccessErrTimes++;
            pstErrCtx->ucSdCardErrTimes++;
            mrsToolsFree(scPathName);
            return HI_ERR_FAILURE;
        }

        if (ret_fd < 0) // �ļ��в�����
        {
            ulRet = HI_MDM_SdCardMakeDir(scPathName, HI_TRUE);
            if (ulRet != HI_ERR_SUCCESS) // �����ļ��г�ʱ
            {
                pstErrCtx->ucSdCardCreatFolderErrTimes++;
                pstErrCtx->ucSdCardErrTimes++;
                mrsToolsFree(scPathName);
                return HI_ERR_FAILURE;
            }
            else // �����ļ��гɹ������ĵ�ǰ�洢�ļ������
            {
                pstStatCtx->ucCurFolderSequence = (HI_U8)i;
                mrsToolsFree(scPathName);
                return HI_ERR_SUCCESS;
            }
        }
    }

    // �ļ��и����Ѿ��ﵽ���ֵ,�޷������洢��ֱ��ֹͣ����
    pstErrCtx->ucSdCardErrTimes = MRS_DC_SD_SD_CARD_MAX_ERR_TIMES + 1;
    mrsToolsFree(scPathName);
    return HI_ERR_FAILURE;
}


// �����ļ���
HI_U32 mrsNdmDcSdGenerateFileName(HI_IN MRS_DC_SD_SAVE_FILE_TYPE_E eSaveFileType, HI_IN HI_U32 ulErrCode,
                                  HI_OUT HI_CHAR *scFileName, HI_OUT HI_U32 *pulLen)
{
    MRS_DC_SD_CONFIG_CTX_STRU * pstConfigCtx = MRS_NDM_DC_SD_GET_CONFIG_CTX();
    MRS_DC_SD_STAT_CTX_STRU * pstStatCtx = MRS_NDM_DC_SD_GET_STAT_CTX() ;
    HI_CHAR scTypeName[MRS_DC_SD_DC_TYPE_BYTE_LEN + 1];
    HI_U32 ulTime = 0;
    HI_U32 ulNameId = 0;
    HI_U32 ulSeqNum = 0;

    ulErrCode = ulErrCode % MRS_DC_SD_MAX_ERR_CODE;

    ulTime = HI_MDM_GetSeconds();
    ulTime = ulTime % MRS_DC_SD_MAX_TIME_SECOND;

    ulNameId = pstConfigCtx->usNameId % MRS_DC_SD_MAX_NAME_ID;

    (hi_void)memset_s(scTypeName, sizeof(scTypeName), 0, sizeof(scTypeName));

    switch (eSaveFileType)
    {
        case MRS_DC_SD_SAVE_FILE_TYPE_A:
            (hi_void)strcpy_s(scTypeName,MRS_DC_SD_DC_TYPE_BYTE_LEN,"A");
            pstStatCtx->usFileCnt_A++;
            ulSeqNum = (pstStatCtx->usFileCnt_A % MRS_DC_SD_MAX_SEQ_NUM);
            break;
        case MRS_DC_SD_SAVE_FILE_TYPE_P:
            (hi_void)strcpy_s(scTypeName,MRS_DC_SD_DC_TYPE_BYTE_LEN, "P");
            pstStatCtx->usFileCnt_P++;
            ulSeqNum = (pstStatCtx->usFileCnt_P % MRS_DC_SD_MAX_SEQ_NUM);
            break;
        case MRS_DC_SD_SAVE_FILE_TYPE_D:
            (hi_void)strcpy_s(scTypeName,MRS_DC_SD_DC_TYPE_BYTE_LEN, "D");
            pstStatCtx->usFileCnt_D++;
            ulSeqNum = (pstStatCtx->usFileCnt_D % MRS_DC_SD_MAX_SEQ_NUM);
            break;
        case MRS_DC_SD_SAVE_FILE_TYPE_F:
            (hi_void)strcpy_s(scTypeName,MRS_DC_SD_DC_TYPE_BYTE_LEN, "F");
            pstStatCtx->usFileCnt_F++;
            ulSeqNum = (pstStatCtx->usFileCnt_F % MRS_DC_SD_MAX_SEQ_NUM);
            break;
        case MRS_DC_SD_SAVE_FILE_TYPE_N:
            (hi_void)strcpy_s(scTypeName,MRS_DC_SD_DC_TYPE_BYTE_LEN, "N");
            pstStatCtx->usFileCnt_N++;
            ulSeqNum = pstStatCtx->usFileCnt_N % MRS_DC_SD_MAX_SEQ_NUM;
            break;
        case MRS_DC_SD_SAVE_FILE_TYPE_S:
            (hi_void)strcpy_s(scTypeName,MRS_DC_SD_DC_TYPE_BYTE_LEN, "S");
            pstStatCtx->usFileCnt_S++;
            ulSeqNum = pstStatCtx->usFileCnt_S % MRS_DC_SD_MAX_SEQ_NUM;
            break;
        case MRS_DC_SD_SAVE_FILE_TYPE_ZCROSS_FALL:
            (hi_void)strcpy_s(scTypeName,MRS_DC_SD_DC_TYPE_BYTE_LEN, "FALL");
            pstStatCtx->usFileCnt_ZCross++;
            ulSeqNum = pstStatCtx->usFileCnt_ZCross % MRS_DC_SD_MAX_SEQ_NUM;
            break;
        case MRS_DC_SD_SAVE_FILE_TYPE_ZCROSS_RISE:
            (hi_void)strcpy_s(scTypeName,MRS_DC_SD_DC_TYPE_BYTE_LEN, "RISE");
            pstStatCtx->usFileCnt_ZCross++;
            ulSeqNum = pstStatCtx->usFileCnt_ZCross % MRS_DC_SD_MAX_SEQ_NUM;
            break;
        default:
            break;
    }

    (hi_void)snprintf_s(scFileName, MRS_DC_SD_TOTAL_PATH_NAME_LEN + 1,MRS_DC_SD_TOTAL_PATH_NAME_LEN, "%s/%s/%u/%03u_%s_%06u_%03u_%04x.bin",
        MRS_DC_SD_BASE_FOLDER_NAME, pstConfigCtx->scFolderName, pstStatCtx->ucCurFolderSequence,
        ulNameId, scTypeName, ulTime, ulSeqNum, ulErrCode);
    *pulLen = (HI_U32)strlen(scFileName);

    return HI_ERR_SUCCESS;
}


HI_U32 mrsNdmDcSdSaveFileToSdCard(HI_BUF_S *pstBuf, HI_CHAR *scFileName, HI_U32 ulFileNameLen)
{
    MRS_DC_SD_ERR_CTX_STRU * pstErrCtx = MRS_NDM_DC_SD_GET_ERR_CTX();
    MRS_DC_SD_STAT_CTX_STRU * pstStatCtx = MRS_NDM_DC_SD_GET_STAT_CTX() ;
    HI_U32 ulRet = HI_ERR_SUCCESS;
    HI_U32 ulTotalSize = 0;
    HI_U32 ulFreeSize = 0;
    HI_S32 ret_fd = -1;
    HI_S32 slRealWriteLen = 0;
    HI_S32 slRetClose = 0;
    HI_BOOL bExist = HI_FALSE;

    HI_UNREF_PARAM(ulFileNameLen);

    do
    {
        ulRet = HI_MDM_OpenSdCardFile(scFileName, HI_FMGR_O_RDWR + HI_FMGR_O_APPEND + HI_FMGR_O_CREAT + HI_FMGR_O_SYNC, 777, &ret_fd);
        if (ulRet != HI_ERR_SUCCESS || ret_fd < 0)
        {
            pstErrCtx->ucSdCardErrTimes++;
            pstErrCtx->ucSdCardOpenErrTimes++;
            break;
        }

        // дbuff1
        if (pstBuf->bufAddr[0] != (HI_U32)HI_NULL)
        {
            ulRet = HI_MDM_WriteSdCardFile(ret_fd, (HI_PVOID)pstBuf->bufAddr[0], pstBuf->bufSize[0], &slRealWriteLen);
            if (ulRet != HI_ERR_SUCCESS || (HI_U32)slRealWriteLen != pstBuf->bufSize[0])
            {
                pstErrCtx->ucSdCardErrTimes++;
                pstErrCtx->ucSdCardWriteErrTimes++;
                break;
            }
        }

        // дbuff2
        if (pstBuf->bufAddr[1] != (HI_U32)HI_NULL)
        {

            ulRet = HI_MDM_WriteSdCardFile(ret_fd, (HI_PVOID)pstBuf->bufAddr[1], pstBuf->bufSize[1], &slRealWriteLen);
            if (ulRet != HI_ERR_SUCCESS || (HI_U32)slRealWriteLen != pstBuf->bufSize[1])
            {
                pstErrCtx->ucSdCardErrTimes++;
                pstErrCtx->ucSdCardWriteErrTimes++;
                break;
            }
        }
    } while(0);

    if (ret_fd >= 0)
    {
        ulRet = HI_MDM_CloseSdCardFile(ret_fd, &slRetClose);
        if (ulRet != HI_ERR_SUCCESS || slRetClose < 0)
        {
            pstErrCtx->ucSdCardErrTimes++;
            pstErrCtx->ucSdCardCloseErrTimes++;
        }
    }

    ulRet = HI_MDM_GetSdCardInfo(&bExist, &ulTotalSize, &ulFreeSize);
    if (ulRet != HI_ERR_SUCCESS || bExist != HI_TRUE) // ��ȡSD��״̬ʧ��,
    {
        pstErrCtx->ucSdCardErrTimes++;
        pstErrCtx->ucSdCardGetInfoErrTimes++;
        pstStatCtx->ulSdFreeSize = 0xFFFFFFFF;
        pstStatCtx->ulSdTotalSize = 0xFFFFFFFF;
    }
    else // TODO:SD������λʱ����ֱ��ֹͣ������
    {
        pstStatCtx->ulSdFreeSize = ulFreeSize;
        pstStatCtx->ulSdTotalSize = ulTotalSize;
        if (ulFreeSize <= MRS_DC_SD_SD_CARD_ALERT_SIZE)
        {
            mrsNdmDcSdLedDisplay(MRS_DC_SD_DISPLAY_ALERT_FULL);
        }
        else
        {
            mrsNdmDcSdLedDisplay(MRS_DC_SD_DISPLAY_ALERT_NONE);
        }
    }

    return HI_ERR_SUCCESS;
}


// SD��״̬���
HI_U32 mrsNdmDcSdCheckSdCardStat(HI_OUT MRS_DC_SD_ALERT_TYPE_E * eSdAlertType)
{
    HI_U32 ulRet = HI_ERR_SUCCESS;
    MRS_DC_SD_ERR_CTX_STRU * pstErrCtx = MRS_NDM_DC_SD_GET_ERR_CTX();
    MRS_DC_SD_STAT_CTX_STRU * pstStatCtx = MRS_NDM_DC_SD_GET_STAT_CTX() ;
    HI_U32 ulTotalSize = 0;
    HI_U32 ulFreeSize = 0;
    HI_BOOL bExist = HI_FALSE;

    *eSdAlertType = MRS_DC_SD_ALERT_TYPE_NONE;

    ulRet = HI_MDM_GetSdCardInfo(&bExist,&ulTotalSize,&ulFreeSize);

    if(ulRet != HI_ERR_SUCCESS)
    {
        pstErrCtx->ucNotStartReason = MRS_DC_SD_NOT_START_REASON_SD_API_CALL_FAIL;
        pstStatCtx->ulSdTotalSize = 0xFFFFFFFF;
        pstStatCtx->ulSdFreeSize = 0xFFFFFFFF;
        *eSdAlertType = MRS_DC_SD_ALERT_TYPE_SD_CARD_ERR;
        return HI_ERR_FAILURE;
    }

    if(HI_FALSE == bExist)
    {
        pstErrCtx->ucNotStartReason = MRS_DC_SD_NOT_START_REASON_SD_NOT_EXIST;
        *eSdAlertType = MRS_DC_SD_ALERT_TYPE_SD_CARD_ERR;
        return HI_ERR_FAILURE;
    }

    if(ulFreeSize <= MRS_DC_SD_NEED_SD_CARD_MIN_SIZE)
    {
        pstErrCtx->ucNotStartReason = MRS_DC_SD_NOT_START_REASON_SD_NO_SPACE;
        pstStatCtx->ulSdFreeSize = ulFreeSize;
        pstStatCtx->ulSdTotalSize = ulTotalSize;
        *eSdAlertType = MRS_DC_SD_ALERT_TYPE_SD_CARD_ERR;
        return HI_ERR_FAILURE;
    }
    else if(ulFreeSize <= MRS_DC_SD_SD_CARD_ALERT_SIZE)
    {
        pstStatCtx->ulSdFreeSize = ulFreeSize;
        pstStatCtx->ulSdTotalSize = ulTotalSize;
        *eSdAlertType = MRS_DC_SD_ALERT_TYPE_SD_CARD_FULL;
    }
    else
    {
        pstStatCtx->ulSdFreeSize = ulFreeSize;
        pstStatCtx->ulSdTotalSize = ulTotalSize;
    }

    return HI_ERR_SUCCESS;
}


// ��ʼ����������
HI_U32 mrsNdmDcSdInitConfig(MRS_DC_SD_START_TYPE_E eStartType)
{
    NV_APP_MRS_NDM_DC_SD_PARAM_STRU * pCfgNv = HI_NULL;
    MRS_DC_SD_CONFIG_CTX_STRU* pstConfigCtx = MRS_NDM_DC_SD_GET_CONFIG_CTX();

    pCfgNv = mrsToolsMalloc(sizeof(NV_APP_MRS_NDM_DC_SD_PARAM_STRU));
    if (HI_NULL == pCfgNv)
    {
        return HI_ERR_MALLOC_FAILUE;
    }

    if (HI_ERR_SUCCESS != HI_MDM_NV_Read(ID_NV_MRS_DC_SD_INFO, pCfgNv, sizeof(NV_APP_MRS_NDM_DC_SD_PARAM_STRU)))
    {
        mrsToolsFree(pCfgNv);
        return HI_ERR_FAILURE;
    }

    pstConfigCtx->ulRamSize         = pCfgNv->ulRamSize;
    pstConfigCtx->slAgcGain         = pCfgNv->slAgcGain;
    pstConfigCtx->usTrigerType      = pCfgNv->usTrigerType;
    pstConfigCtx->usPeriodTime      = pCfgNv->usPeriodTime;
    pstConfigCtx->usTrigerTime      = pCfgNv->usTrigerTime;
    pstConfigCtx->ucTimesPerPeriod  = pCfgNv->ucTimesPerPeriod;
    pstConfigCtx->ucCPowerOnEable   = pCfgNv->ucCPowerOnEable;
    pstConfigCtx->ucIsFollowMode    = pCfgNv->ucIsFollowMode;
    pstConfigCtx->ucFreq            = pCfgNv->ucFreq;
    pstConfigCtx->ucSnid            = pCfgNv->ucSnid;
    pstConfigCtx->ucIsFixFreq       = pCfgNv->ucIsFixFreq;
    pstConfigCtx->usNameId          = pCfgNv->usNameId;
    (hi_void)memcpy_s(pstConfigCtx->scCcoMac,HI_PLC_MAC_ADDR_LEN,     pCfgNv->scCcoMac,     HI_PLC_MAC_ADDR_LEN);
    (hi_void)memcpy_s(pstConfigCtx->scFolderName,sizeof(pCfgNv->scFolderName), pCfgNv->scFolderName, sizeof(pCfgNv->scFolderName));
    pstConfigCtx->scFolderName[sizeof(pstConfigCtx->scFolderName) - 1] = '\0';

    if ((pCfgNv->ucCPowerOnEable) &&
        ((eStartType != MRS_DC_SD_START_TYPE_KEY_ZCROSS_UP) && (eStartType != MRS_DC_SD_START_TYPE_KEY_ZCROSS_DOWN)))
    {
        pstConfigCtx->ucIsSetRebootStart = HI_TRUE;
    }
    else
    {
        pstConfigCtx->ucIsSetRebootStart = HI_FALSE;
    }

    if(MRS_DC_SD_START_TYPE_KEY_ZCROSS_UP == eStartType)
    {
        pstConfigCtx->ucZCrossType = MRS_DC_SD_ZCROSS_ISR_TYPE_RISE;
    }
    else if(MRS_DC_SD_START_TYPE_KEY_ZCROSS_DOWN == eStartType)
    {
        pstConfigCtx->ucZCrossType = MRS_DC_SD_ZCROSS_ISR_TYPE_FALL;
    }

    mrsToolsFree(pCfgNv);
    return HI_ERR_SUCCESS;
}


// RTCУʱ
HI_U32 mrsNdmDcSdRtcSetTime(HI_VOID)
{
    HI_U32 ulRet = HI_ERR_SUCCESS;
    HI_U32 ulRtcTime = 0;

    ulRet = HI_RTC_ReadTime(&ulRtcTime);
    if (ulRet == HI_ERR_SUCCESS)
    {
        return HI_MDM_SetFileSysTime((HI_S32)ulRtcTime);
    }

    return ulRet;
}


HI_U32 mrsNdmDcSdChangeMainPhase(MRS_DC_SD_CHANGE_PHASE_REASON_E eTrigerReason, HI_U32 ulKeyDownUpTime)
{
    MRS_DC_SD_MAIN_CTRL_CTX_STRU * pstMainCtrlCtx = MRS_NDM_DC_SD_GET_MAIN_CTRL_CTX();
    HI_U32 ulRet = HI_ERR_FAILURE;

    HI_UNREF_PARAM(ulKeyDownUpTime);

    /*
        ��������:ʱ��1s��״̬Ϊֹͣ-->��ʾ�����ز���
        ��������:ʱ��3s��״̬Ϊ��ʾ�����ز���-->��ʾ�½��ز���

        ����̧��:ʱ��<=1s(���û��ʾ����1���Ӱ���ͨ��������ɵ����Ǳ�Ҫ����)��״̬Ϊֹͣ-->��ͨ����������
        ����̧��:ʱ��<=3s(�Ǳ�Ҫ)��״̬Ϊ��ʾ�����ز���-->�����ز���������
        ����̧��:ʱ��>3s(�Ǳ�Ҫ)��״̬Ϊ��ʾ�½��ز���-->�½��ز���������
        ����̧��:״̬Ϊ�����ѿ�ʼ-->����ֹͣ��

        ��������:״̬Ϊֹͣ-->��������������
    */

    if (MRS_DC_SD_CHANGE_PHASE_REASON_KEY_DOWN == eTrigerReason)
    {
        ulRet = HI_ERR_FAILURE;
    }
    else if (MRS_DC_SD_CHANGE_PHASE_REASON_KEY_UP == eTrigerReason)
    {

        if (MRS_DC_SD_MAIN_PHASE_STOPPED == pstMainCtrlCtx->ucMainPhase) // ����̧��ʱ��С��1s���ҵ�ǰ״̬Ϊֹͣ-->��ͨ����
        {
            ulRet = HI_ERR_SUCCESS;
            pstMainCtrlCtx->ucMainPhase = MRS_DC_SD_MAIN_PHASE_STARING_NORMAL;

        }
        else if (MRS_DC_SD_MAIN_PHASE_STARTED == pstMainCtrlCtx->ucMainPhase) // ����̧���ҵ�ǰ״̬����������
        {
            ulRet = HI_ERR_SUCCESS;
            pstMainCtrlCtx->ucMainPhase = MRS_DC_SD_MAIN_PHASE_STOPPING;
        }
    }
    else if (MRS_DC_SD_CHANGE_PHASE_REASON_REBOOT_START == eTrigerReason)
    {
        if (MRS_DC_SD_MAIN_PHASE_STOPPED == pstMainCtrlCtx->ucMainPhase)
        {
            ulRet = HI_ERR_SUCCESS;
            pstMainCtrlCtx->ucMainPhase = MRS_DC_SD_MAIN_PHASE_STARTING_REBOOT;
        }
    }
    else if (MRS_DC_SD_CHANGE_PHASE_REASON_SD_CADR_ERR == eTrigerReason ||
        MRS_DC_SD_CHANGE_PHASE_REASON_COMPLETE == eTrigerReason)
    {
        if (MRS_DC_SD_MAIN_PHASE_STARTED == pstMainCtrlCtx->ucMainPhase)
        {
            ulRet = HI_ERR_SUCCESS;
            pstMainCtrlCtx->ucMainPhase = MRS_DC_SD_MAIN_PHASE_STOPPING;
        }
    }

    return ulRet;
}


HI_VOID mrsNdmDcSdLedDisplay(MRS_DC_SD_DISPLAY_E eDisplayId)
{
#ifndef PRODUCT_CFG_SUPPORT_CCT3911
    HI_LCD_DISPLAY_INFO_S stLcdCfg;

    (hi_void)memset_s(&stLcdCfg, sizeof(stLcdCfg), 0, sizeof(stLcdCfg));

    switch (eDisplayId)
    {
    case MRS_DC_SD_DISPLAY_DC_NONE:
        (HI_VOID)HI_DRV_LCD_DisplayDisable(HI_LCD_FUNC_ID_DATACOLLECT);
        break;
    case MRS_DC_SD_DISPLAY_DC_NORMAL:
        stLcdCfg.data[0] = 'C';
        stLcdCfg.data[1] = '0';
        stLcdCfg.data[2] = '1';
        stLcdCfg.pri     = HI_LCD_DISPLAY_PRI_1;
        stLcdCfg.tsec    = HI_LCD_DISPLAY_FOREVER;
        (HI_VOID)HI_DRV_LCD_DisplayPrint(HI_LCD_FUNC_ID_DATACOLLECT, &stLcdCfg);
        break;
    case MRS_DC_SD_DISPLAY_DC_ZERO_CROSS_UP:
        stLcdCfg.data[0] = 'C';
        stLcdCfg.data[1] = '0';
        stLcdCfg.data[2] = '2';
        stLcdCfg.pri     = HI_LCD_DISPLAY_PRI_1;
        stLcdCfg.tsec    = HI_LCD_DISPLAY_FOREVER;
        (HI_VOID)HI_DRV_LCD_DisplayPrint(HI_LCD_FUNC_ID_DATACOLLECT, &stLcdCfg);
        break;
    case MRS_DC_SD_DISPLAY_DC_ZERO_CROSS_DOWN:
        stLcdCfg.data[0] = 'C';
        stLcdCfg.data[1] = '0';
        stLcdCfg.data[2] = '3';
        stLcdCfg.pri     = HI_LCD_DISPLAY_PRI_1;
        stLcdCfg.tsec    = HI_LCD_DISPLAY_FOREVER;
        (HI_VOID)HI_DRV_LCD_DisplayPrint(HI_LCD_FUNC_ID_DATACOLLECT, &stLcdCfg);
        break;
    case MRS_DC_SD_DISPLAY_ALERT_FULL:
        stLcdCfg.data[0] = 'F';
        stLcdCfg.data[1] = 'U';
        stLcdCfg.data[2] = 'L';
        stLcdCfg.pri     = HI_LCD_DISPLAY_PRI_1;
        stLcdCfg.tsec    = HI_LCD_DISPLAY_FOREVER;
        (HI_VOID)HI_DRV_LCD_DisplayPrint(HI_LCD_FUNC_ID_CD_ALERT, &stLcdCfg);
        break;
    case MRS_DC_SD_DISPLAY_ALERT_SD_ERR:
        stLcdCfg.data[0] = 'E';
        stLcdCfg.data[1] = '0';
        stLcdCfg.data[2] = '1';
        stLcdCfg.pri     = HI_LCD_DISPLAY_PRI_0;
        stLcdCfg.tsec    = HI_LCD_DISPLAY_FOREVER;
        (HI_VOID)HI_DRV_LCD_DisplayPrint(HI_LCD_FUNC_ID_CD_ALERT, &stLcdCfg);
        break;
    case MRS_DC_SD_DISPLAY_ALERT_NONE:
        (HI_VOID) HI_DRV_LCD_DisplayDisable(HI_LCD_FUNC_ID_CD_ALERT);
        break;
    default:
        break;
    }
#endif
    return ;
}


HI_VOID mrsNdmDcSdUpdateSdCardLedAlert(MRS_DC_SD_ALERT_TYPE_E eSdAlertType)
{
    if(eSdAlertType == MRS_DC_SD_ALERT_TYPE_SD_CARD_FULL)
    {
        mrsNdmDcSdLedDisplay(MRS_DC_SD_DISPLAY_ALERT_FULL);
    }
    else if(eSdAlertType == MRS_DC_SD_ALERT_TYPE_SD_CARD_ERR)
    {
        mrsNdmDcSdLedDisplay(MRS_DC_SD_DISPLAY_ALERT_SD_ERR);
    }
    else
    {
        mrsNdmDcSdLedDisplay(MRS_DC_SD_DISPLAY_ALERT_NONE);
    }
}




HI_END_HEADER
#endif // defined(PRODUCT_CFG_PRODUCT_TYPE_NDM)

