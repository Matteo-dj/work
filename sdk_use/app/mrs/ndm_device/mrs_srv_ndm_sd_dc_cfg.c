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

#include "mrs_srv_ndm.h"
#include "drv_dfx.h"
#include "hi_mdm_datacollect.h"
#include "hi_rtc.h"
#include <dfx_app.h>//HSOͳ��
#include "mrs_srv_ndm_collectdata.h "
HI_START_HEADER

// ����������
HI_U32 mrsNdmDcSdCheckTrigerType(HI_U16 usTrigerType, HI_BOOL bIsFollowMode)
{
    HI_U16 usCurTypeMode = 0;
    HI_U16 usCurTypeAll = usTrigerType & MRS_DC_SD_DATA_COLLECT_TYPE_ALL;

    if (bIsFollowMode)
    {
        usCurTypeMode = usTrigerType & MRS_DC_SD_DATA_COLLECT_TYPE_FOLLOW_ALL;
    }
    else
    {
        usCurTypeMode = usTrigerType & MRS_DC_SD_DATA_COLLECT_TYPE_UNFOLLOW_ALL;
    }
    if ((0 == usCurTypeMode) || (usCurTypeMode != usCurTypeAll))
    {
        return HI_ERR_FAILURE;
    }

    return HI_ERR_SUCCESS;
}


// �������AGC
HI_U32 mrsNdmDcSdCheckFixAgcGain(HI_S32 slAgcGain)
{
    if (slAgcGain < -12 || slAgcGain > 48 || (((HI_U32)slAgcGain&0x1) != 0x0))
    {
        return HI_ERR_FAILURE;
    }

    return HI_ERR_SUCCESS;
}


// �������ʱ��
HI_U32 mrsNdmDcSdCheckPeriodTime(HI_U16 usMinute, HI_BOOL bFollowMode)
{
   if (bFollowMode)
    {
        if (usMinute < MRS_DC_SD_FOLLOW_MIN_PERIOD_TIME)
        {
            return HI_ERR_FAILURE;
        }

    }
    else
    {
        if (usMinute < MRS_DC_SD_UNFOLLOW_MIN_PERIOD_TIME)
        {
            return HI_ERR_FAILURE;
        }

    }

    return HI_ERR_SUCCESS;
}


// ���ÿ��֧�ֲ�������
HI_U32 mrsNdmDcSdCheckTimesPerPeriod(HI_U8 ucTimes, HI_BOOL bFollowMode)
{
    if (0 == ucTimes)
    {
        return HI_ERR_FAILURE;
    }

    if (bFollowMode)
    {
        if (ucTimes != MRS_DC_SD_FOLLOW_TIMES_PER_PERIOD)
        {
            return HI_ERR_FAILURE;
        }
    }
    else
    {
        if (ucTimes > MRS_DC_SD_UNFOLLOW_MAX_TIMES_PER_PERIOD)
        {
            return HI_ERR_FAILURE;
        }
    }

    return HI_ERR_SUCCESS;
}


// ����������ʱ��
HI_U32 mrsNdmDcSdCheckTrigerTime(HI_U16 usMinute, HI_BOOL bFollowMode)
{
    if (bFollowMode)
    {
        if (usMinute < MRS_DC_SD_UNFOLLOW_MIN_TRIGER_TIME)
        {
            return HI_ERR_FAILURE;
        }
    }
    else
    {
        if (usMinute < MRS_DC_SD_FOLLOW_MIN_TRIGER_TIME)
        {
            return HI_ERR_FAILURE;
        }
    }

    return HI_ERR_SUCCESS;
}


// ���NV����
HI_U32 mrsNdmDcSdCheckConfig(HI_VOID)
{
    HI_U32 ulRet = HI_ERR_SUCCESS;
    HI_BOOL bIsFllowMode = HI_FALSE;//�Ƿ���Ҫ����
    HI_BOOL bIsNoiseMode = HI_FALSE;//�Ƿ���Ҫ�ɼ�����
    HI_BOOL bIsCycleMode = HI_FALSE;//�Ƿ������ڲ��й�
    HI_BOOL bIsTrigerMode = HI_FALSE;

    MRS_DC_SD_CONFIG_CTX_STRU *pstConfigCtx = MRS_NDM_DC_SD_GET_CONFIG_CTX();

    bIsFllowMode = pstConfigCtx->ucIsFollowMode;

    if (pstConfigCtx->usTrigerType & MRS_DC_SD_DATA_COLLECT_TYPE_NOISE_ALL)
    {
        bIsNoiseMode = HI_TRUE;
    }

    if (pstConfigCtx->usTrigerType & MRS_DC_SD_DATA_COLLECT_TYPE_DEFAULT)
    {
        bIsCycleMode = HI_TRUE;
    }

    if (pstConfigCtx->usTrigerType & MRS_DC_SD_DATA_COLLECT_TYPE_TRI_ALL)
    {
        bIsTrigerMode = HI_TRUE;
    }

    // ͨ���ļ���
    ulRet |= mrsNdmDcSdCheckFolderName(pstConfigCtx->scFolderName);

    // ͨ�ô�������
    ulRet |= mrsNdmDcSdCheckTrigerType(pstConfigCtx->usTrigerType, pstConfigCtx->ucIsFollowMode);

    // ͨ��RAM ADDR SIZE
    ulRet |= mrsNdmDsSdCheckAddrAndLen(pstConfigCtx->ulStartAddr,
        pstConfigCtx->ulStartAddr + pstConfigCtx->ulRamSize, MRS_DC_SD_DEFAULT_AFTER_POINT_LEN);

    if (bIsFllowMode)
    {
        ulRet|=HI_ERR_NOT_SUPPORT;
    }

    if (bIsNoiseMode)
    {
        // ����: AGC
        ulRet |= mrsNdmDcSdCheckFixAgcGain(pstConfigCtx->slAgcGain);
    }

    if (bIsCycleMode)
    {
        // ����: ����
        ulRet |= mrsNdmDcSdCheckPeriodTime(pstConfigCtx->usPeriodTime, pstConfigCtx->ucIsFollowMode);

        // ����: ����
        ulRet |= mrsNdmDcSdCheckTimesPerPeriod(pstConfigCtx->ucTimesPerPeriod, pstConfigCtx->ucIsFollowMode);
    }

    if (bIsTrigerMode)
    {
        ulRet |= mrsNdmDcSdCheckTrigerTime(pstConfigCtx->usTrigerTime, pstConfigCtx->ucIsFollowMode);
    }

    return ulRet;
}

#define MRS_IS_VALID_FOLDERNAME_CHAR(x)     (MRS_CHAR_ISUPPER(x) || MRS_CHAR_ISLOWER(x) || MRS_CHAR_ISDIGIT(x) || ('_' == (x)))

// ���ͨ���ļ���
HI_U32 mrsNdmDcSdCheckFolderName(HI_CHAR *szFolderName)
{
    HI_U32 i = 0;

    if (HI_NULL == szFolderName)
    {
        return HI_ERR_FAILURE;
    }

    for (i = 0; i < NDM_DC_FOLDER_NAME_SIZE; i++)
    {
        if ('\0' == szFolderName[i])
        {
            break;
        }

        if (!MRS_IS_VALID_FOLDERNAME_CHAR(szFolderName[i]))
        {
            return HI_ERR_FAILURE;
        }
    }

    if (0 == i)
    {
        return HI_ERR_FAILURE;
    }
    else
    {
        return HI_ERR_SUCCESS;
    }
}





HI_U32 mrsNdmDsSdCheckAddrAndLen(HI_U32 ulStartAddr,HI_U32 ulEndAddr,HI_U32 ulAfterPointlen)
{
    if (ulStartAddr >= ulEndAddr)
    {
        return HI_ERR_FAILURE;
    }

    if (((ulStartAddr & 0x3) != 0) || ((ulEndAddr & 0x3) != 0) || ((ulAfterPointlen & 0x3) != 0) || (ulAfterPointlen == 0))
    {
        return HI_ERR_FAILURE;
    }

    if ((ulEndAddr - ulStartAddr) < (ulAfterPointlen + MRS_DC_SD_DEFAULT_BEFORE_POINT_LEN))
    {
         return HI_ERR_FAILURE;
    }

    return HI_ERR_SUCCESS;
}



// HSO->���Կ���̨->���ݲɼ�->�洢�ɼ�����: ����SD����������
HI_U32 mrsNdmMsgDcSdSetConfig(HI_U16 usId, HI_PVOID pMsgParam)
{
    HI_U32 * pRecvMsgParam = (HI_U32*)pMsgParam;
    HI_SD_DC_CONFIG_SET_CMD_S *pstReq = HI_NULL;
    HI_SD_DC_CONFIG_SET_IND_S stInd;
    HI_BOOL bIsFllowMode = HI_FALSE; // �Ƿ���Ҫ����
    HI_BOOL bIsNoiseMode = HI_FALSE; // �Ƿ���Ҫ�ɼ�����
    HI_BOOL bIsCycleMode = HI_FALSE; // �Ƿ������ڲ��й�
    HI_BOOL bIsTrigerMode = HI_FALSE;
    NV_APP_MRS_NDM_DC_SD_PARAM_STRU * pCmnCfg = HI_NULL;

    HI_UNREF_PARAM(usId);

    (hi_void)memset_s(&stInd, sizeof(stInd), 0, sizeof(stInd));

    if (sizeof(HI_SD_DC_CONFIG_SET_CMD_S) != pRecvMsgParam[2])
    {
        stInd.errCode = HI_SD_DC_SET_CONFIG_ERR_CODE_INVALID_PARAM;
        HI_MDM_ReportPacket(ID_DIAG_CMD_NDM_SD_DC_SET_CONFIG, HI_DIAG_CMD_INSTANCE_LOCAL, (HI_PBYTE)&stInd, (HI_U16)sizeof(stInd), HI_TRUE);
        return HI_ERR_FAILURE;
    }

    pstReq = (HI_SD_DC_CONFIG_SET_CMD_S *)pRecvMsgParam[0];

    if (pstReq->attribute & HI_SD_DC_SET_CONFIG_Q_ATTRIBUTE_BIT_FOLLOW)
    {
        bIsFllowMode = HI_TRUE;
    }
    if (pstReq->usTrigerType & MRS_DC_SD_DATA_COLLECT_TYPE_NOISE_ALL)
    {
        bIsNoiseMode = HI_TRUE;
    }

    if (pstReq->usTrigerType & MRS_DC_SD_DATA_COLLECT_TYPE_DEFAULT)
    {
        bIsCycleMode = HI_TRUE;
    }

    if (pstReq->usTrigerType & MRS_DC_SD_DATA_COLLECT_TYPE_TRI_ALL)
    {
        bIsTrigerMode = HI_TRUE;
    }

    if(bIsFllowMode)
    {
        stInd.errCode = HI_SD_DC_SET_CONFIG_ERR_CODE_NOT_SUPPORT_FOLLOW_MODE;
        HI_MDM_ReportPacket(ID_DIAG_CMD_NDM_SD_DC_SET_CONFIG, HI_DIAG_CMD_INSTANCE_LOCAL, (HI_PBYTE)&stInd, (HI_U16)sizeof(stInd), HI_TRUE);
        return HI_ERR_FAILURE;

    }

    // ͨ��У���ļ�������
    if(HI_ERR_SUCCESS != mrsNdmDcSdCheckFolderName(pstReq->scFolderName))
    {
        stInd.errCode = HI_SD_DC_SET_CONFIG_ERR_CODE_FOLDER_NAME;
        HI_MDM_ReportPacket(ID_DIAG_CMD_NDM_SD_DC_SET_CONFIG, HI_DIAG_CMD_INSTANCE_LOCAL, (HI_PBYTE)&stInd, (HI_U16)sizeof(stInd), HI_TRUE);
        return HI_ERR_FAILURE;
    }

    // ͨ��У�鴥������
    if (HI_ERR_SUCCESS != mrsNdmDcSdCheckTrigerType(pstReq->usTrigerType, bIsFllowMode))
    {
        stInd.errCode = HI_SD_DC_SET_CONFIG_ERR_CODE_TRIGER_TYPE;
        HI_MDM_ReportPacket(ID_DIAG_CMD_NDM_SD_DC_SET_CONFIG, HI_DIAG_CMD_INSTANCE_LOCAL, (HI_PBYTE)&stInd, (HI_U16)sizeof(stInd), HI_TRUE);
        return HI_ERR_FAILURE;
    }




    // ����(����,����): У������
    if (bIsNoiseMode && (HI_ERR_SUCCESS != mrsNdmDcSdCheckFixAgcGain(pstReq->AgcGain)))
    {
        stInd.errCode = HI_SD_DC_SET_CONFIG_ERR_CODE_AGGAIN;
        HI_MDM_ReportPacket(ID_DIAG_CMD_NDM_SD_DC_SET_CONFIG, HI_DIAG_CMD_INSTANCE_LOCAL, (HI_PBYTE)&stInd, (HI_U16)sizeof(stInd), HI_TRUE);
        return HI_ERR_FAILURE;
    }

    // ����: У������
    if (bIsCycleMode && (HI_ERR_SUCCESS != mrsNdmDcSdCheckPeriodTime(pstReq->periodTime, bIsFllowMode)))
    {
        stInd.errCode = HI_SD_DC_SET_CONFIG_ERR_CODE_PERIOD_TIME;
        HI_MDM_ReportPacket(ID_DIAG_CMD_NDM_SD_DC_SET_CONFIG, HI_DIAG_CMD_INSTANCE_LOCAL, (HI_PBYTE)&stInd, (HI_U16)sizeof(stInd), HI_TRUE);
        return HI_ERR_FAILURE;
    }

    // ����: У��ÿ�ֲɼ�����
    if (bIsCycleMode && (HI_ERR_SUCCESS != mrsNdmDcSdCheckTimesPerPeriod(pstReq->TimesPerPeriod, bIsFllowMode)))
    {
        stInd.errCode = HI_SD_DC_SET_CONFIG_ERR_CODE_PER_TIMES;
        HI_MDM_ReportPacket(ID_DIAG_CMD_NDM_SD_DC_SET_CONFIG, HI_DIAG_CMD_INSTANCE_LOCAL, (HI_PBYTE)&stInd, (HI_U16)sizeof(stInd), HI_TRUE);
        return HI_ERR_FAILURE;
    }

    // ����: У�鴥��ʱ��
    if (bIsTrigerMode && (HI_ERR_SUCCESS != mrsNdmDcSdCheckTrigerTime(pstReq->trigerTime, bIsFllowMode)))
    {
        stInd.errCode = HI_SD_DC_SET_CONFIG_ERR_CODE_TRI_TIME;
        HI_MDM_ReportPacket(ID_DIAG_CMD_NDM_SD_DC_SET_CONFIG, HI_DIAG_CMD_INSTANCE_LOCAL, (HI_PBYTE)&stInd, (HI_U16)sizeof(stInd), HI_TRUE);
        return HI_ERR_FAILURE;
    }

    // дNV
    pCmnCfg = mrsToolsMalloc(sizeof(NV_APP_MRS_NDM_DC_SD_PARAM_STRU));
    if (HI_NULL == pCmnCfg)
    {
        stInd.errCode = HI_SD_DC_SET_CONFIG_ERR_CODE_SYS_CALL;
        HI_MDM_ReportPacket(ID_DIAG_CMD_NDM_SD_DC_SET_CONFIG, HI_DIAG_CMD_INSTANCE_LOCAL, (HI_PBYTE)&stInd, (HI_U16)sizeof(stInd), HI_TRUE);
        return HI_ERR_FAILURE;
    }

    if (HI_ERR_SUCCESS != HI_MDM_NV_Read(ID_NV_MRS_DC_SD_INFO, pCmnCfg, sizeof(NV_APP_MRS_NDM_DC_SD_PARAM_STRU)))
    {
        stInd.errCode = HI_SD_DC_SET_CONFIG_ERR_CODE_SYS_CALL;
        HI_MDM_ReportPacket(ID_DIAG_CMD_NDM_SD_DC_SET_CONFIG, HI_DIAG_CMD_INSTANCE_LOCAL, (HI_PBYTE)&stInd, (HI_U16)sizeof(stInd), HI_TRUE);
        mrsToolsFree(pCmnCfg);
        return HI_ERR_FAILURE;
    }

    // ͨ��: �ļ���
    (hi_void)memcpy_s(pCmnCfg->scFolderName,sizeof(pCmnCfg->scFolderName), pstReq->scFolderName, sizeof(pCmnCfg->scFolderName));

    // ͨ��: RamSize
    pCmnCfg->ulRamSize = pstReq->ramSize;

    // ͨ��: ��������
    pCmnCfg->usTrigerType = pstReq->usTrigerType;

    // ͨ��: ����
    if (pstReq->attribute & HI_SD_DC_SET_CONFIG_Q_ATTRIBUTE_BIT_CONTINU_DC)
    {
        pCmnCfg->ucCPowerOnEable = HI_TRUE;
    }
    else
    {
        pCmnCfg->ucCPowerOnEable = HI_FALSE;
    }

    // ͨ��: ����
    pCmnCfg->ucIsFollowMode = bIsFllowMode;

    if (bIsNoiseMode)
    {
        // ����(����,����): ����
        pCmnCfg->slAgcGain = pstReq->AgcGain;
    }

    if (bIsCycleMode)
    {
        // ����: ����
        pCmnCfg->usPeriodTime = pstReq->periodTime;
        // ����: ÿ�ֲɼ�����
        pCmnCfg->ucTimesPerPeriod = pstReq->TimesPerPeriod;
    }

    if (bIsTrigerMode)
    {
        // ����: ����ʱ�䳤��
        pCmnCfg->usTrigerTime = pstReq->trigerTime;
    }

    if (HI_ERR_SUCCESS != HI_MDM_NV_Write(ID_NV_MRS_DC_SD_INFO, pCmnCfg, sizeof(NV_APP_MRS_NDM_DC_SD_PARAM_STRU)))
    {
        stInd.errCode = HI_SD_DC_SET_CONFIG_ERR_CODE_SYS_CALL;
        HI_MDM_ReportPacket(ID_DIAG_CMD_NDM_SD_DC_SET_CONFIG, HI_DIAG_CMD_INSTANCE_LOCAL, (HI_PBYTE)&stInd, (HI_U16)sizeof(stInd), HI_TRUE);
        mrsToolsFree(pCmnCfg);
        return HI_ERR_FAILURE;
    }

    stInd.errCode = HI_SD_DC_SET_CONFIG_ERR_CODE_NONE;
    HI_MDM_ReportPacket(ID_DIAG_CMD_NDM_SD_DC_SET_CONFIG, HI_DIAG_CMD_INSTANCE_LOCAL, (HI_PBYTE)&stInd, (HI_U16)sizeof(stInd), HI_TRUE);
    mrsToolsFree(pCmnCfg);

    return HI_ERR_SUCCESS;
}


// д���õ�NV
HI_U32 mrsNdmDcSdWriteConfigToNv(HI_BOOL bIsColleting, HI_U16 usNameId)
{
    NV_APP_MRS_NDM_DC_SD_PARAM_STRU * pCfgNv = HI_NULL;

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

    pCfgNv->ucCollecting = bIsColleting;
    pCfgNv->usNameId = usNameId;
    if (HI_ERR_SUCCESS != HI_MDM_NV_Write(ID_NV_MRS_DC_SD_INFO, pCfgNv, sizeof(NV_APP_MRS_NDM_DC_SD_PARAM_STRU)))
    {
        mrsToolsFree(pCfgNv);
        return HI_ERR_FAILURE;
    }

    mrsToolsFree(pCfgNv);
    return HI_ERR_SUCCESS;
}


// HSO->���Կ���̨->���ݲɼ�->�洢�ɼ�����: ��ѯSD����������
HI_U32 mrsNdmMsgDcSdQueryConfig(HI_U16 usId, HI_PVOID pMsgParam)
{
    HI_SD_DC_CONFIG_QUERY_IND_S stInd;
    NV_APP_MRS_NDM_DC_SD_PARAM_STRU * pCmnCfg = HI_NULL;
    HI_U32 ulCurTimeSec = 0;

    HI_UNREF_PARAM(usId);
    HI_UNREF_PARAM(pMsgParam);

    (hi_void)memset_s(&stInd, sizeof(stInd), 0, sizeof(stInd));

    if (HI_ERR_SUCCESS != HI_RTC_ReadTime(&ulCurTimeSec))
    {
        
        stInd.errCode = HI_SD_DC_SET_CONFIG_ERR_CODE_GET_RTC_TIME_FAIL;
        HI_MDM_ReportPacket(ID_DIAG_CMD_NDM_SD_DC_QUERY_CONFIG, HI_DIAG_CMD_INSTANCE_LOCAL, (HI_PBYTE)&stInd, (HI_U16)sizeof(stInd), HI_TRUE);
    }
    stInd.curTimeSec = ulCurTimeSec;

    pCmnCfg = mrsToolsMalloc(sizeof(NV_APP_MRS_NDM_DC_SD_PARAM_STRU));
    if (HI_NULL == pCmnCfg)
    {
        stInd.errCode = HI_SD_DC_SET_CONFIG_ERR_CODE_SYS_CALL;
        HI_MDM_ReportPacket(ID_DIAG_CMD_NDM_SD_DC_QUERY_CONFIG, HI_DIAG_CMD_INSTANCE_LOCAL, (HI_PBYTE)&stInd, (HI_U16)sizeof(stInd), HI_TRUE);
        return HI_ERR_FAILURE;
    }

    if (HI_ERR_SUCCESS != HI_MDM_NV_Read(ID_NV_MRS_DC_SD_INFO, pCmnCfg, sizeof(NV_APP_MRS_NDM_DC_SD_PARAM_STRU)))
    {
        stInd.errCode = HI_SD_DC_SET_CONFIG_ERR_CODE_SYS_CALL;
        HI_MDM_ReportPacket(ID_DIAG_CMD_NDM_SD_DC_QUERY_CONFIG, HI_DIAG_CMD_INSTANCE_LOCAL, (HI_PBYTE)&stInd, (HI_U16)sizeof(stInd), HI_TRUE);
        mrsToolsFree(pCmnCfg);
        return HI_ERR_FAILURE;
    }

    stInd.AgcGain       = pCmnCfg->slAgcGain;
    stInd.usTrigerType  = pCmnCfg->usTrigerType;
    stInd.periodTime    = pCmnCfg->usPeriodTime;
    stInd.trigerTime    = pCmnCfg->usTrigerTime;
    stInd.TimesPerPeriod = pCmnCfg->ucTimesPerPeriod;
    (hi_void)memcpy_s(stInd.scFolderName,sizeof(pCmnCfg->scFolderName), pCmnCfg->scFolderName, sizeof(pCmnCfg->scFolderName));

    //����
    if (pCmnCfg->ucIsFollowMode)
    {
        stInd.attribute |= HI_SD_DC_SET_CONFIG_Q_ATTRIBUTE_BIT_FOLLOW;
    }

    if (pCmnCfg->ucCPowerOnEable)
    {
        stInd.attribute |= HI_SD_DC_SET_CONFIG_Q_ATTRIBUTE_BIT_CONTINU_DC;
    }

    if (pCmnCfg->ucIsFixFreq)
    {
        stInd.attribute |= HI_SD_DC_SET_CONFIG_Q_ATTRIBUTE_BIT_FIX_FREQ;
    }

    if (pCmnCfg->ucCollecting)
    {
        stInd.attribute |= HI_SD_DC_SET_CONFIG_Q_ATTRIBUTE_BIT_BECOLLECTING;
    }

    // MAC
    (hi_void)memcpy_s(stInd.ccomac,HI_PLC_MAC_ADDR_LEN, pCmnCfg->scCcoMac, HI_PLC_MAC_ADDR_LEN);
    stInd.freq = pCmnCfg->ucFreq;
    stInd.snid = pCmnCfg->ucSnid;

    HI_MDM_ReportPacket(ID_DIAG_CMD_NDM_SD_DC_QUERY_CONFIG, HI_DIAG_CMD_INSTANCE_LOCAL, (HI_PBYTE)&stInd, (HI_U16)sizeof(stInd), HI_TRUE);
    mrsToolsFree(pCmnCfg);

    return HI_ERR_SUCCESS;
}
HI_END_HEADER
#endif // defined(PRODUCT_CFG_PRODUCT_TYPE_NDM)



