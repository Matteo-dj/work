//*****************************************************************************
//
//                  版权所有 (C), 1998-2015, 华为技术有限公司
//
//*****************************************************************************
//  文 件 名   : mrs_srv_sta_dutycycle.c
//  版 本 号   : V1.0
//  作    者   : cuiate/00233580
//  生成日期   : 2015-08-27
//  功能描述   : STA占空比策略
//
//  函数列表   : TODO: ...
//  修改历史   :
//  1.日    期 : 2015-08-27
//    作    者 : cuiate/00233580
//    修改内容 : 创建文件
//
//*****************************************************************************

#include "mrs_common.h"
#include "mrs_srv_common.h"
#include "mrs_fw_tools.h"
#include "mrs_fw_proto645.h"
#include "mrs_srv_res.h"
#include "app_nv.h"
#include "hi_ft_nv.h"
#include "mrs_srv_sta_queue.h"
#include "mrs_srv_parallel_sta.h"
#include "mrs_srv_sta.h"
#include "mrs_time.h"
#include "mrs_srv_sta_dutycycle.h"
#include "mrs_srv_sta_dutycycle_n.h"
#include "mrs_srv_sta_meter.h"
#include "mrs_fw_log.h"
HI_START_HEADER
#if defined(PRODUCT_CFG_PRODUCT_TYPE_STA)
//*****************************************************************************
//*****************************************************************************

HI_PRV MRS_SRV_STA_DUTY_CYCLE_CONTEXT *g_pstStaDutyCycleCtx = HI_NULL;

HI_PRV MRS_STA_DUTY_CYCLE_CMD_OBJ g_astDutyCycleCmdTbl[] =
{
    {MRS_PROTO645_DI_DATE_TIME, mrsStaDutyCycleDateTimeFrameRx, mrsStaDutyCycleDateTimeTimeout},
    {MRS_PROTO645_DI_POWER_INF, mrsStaDutyCyclePowerInfFrameRx, mrsStaDutyCyclePowerInfTimeout},
};

#if defined(PRODUCT_CFG_CLTI_SUPPORT_MRS_SWITCH)
HI_U32 mrsStaDutyCycleInit(HI_VOID)
{
    HI_U32 ulRet = HI_ERR_SUCCESS;
    HI_U32 ulNvItemSizeMax = 128;
    HI_PBYTE pBuffer = HI_NULL;

    do
    {
        MRS_SRV_STA_DUTY_CYCLE_CONTEXT *pstCtx = HI_NULL;
        NV_APP_STA_DUTY_CYCLE_CFG_INFO *pstCfgInf = HI_NULL;
        HI_NV_FTM_COLLECT_PARAM_FILE_S *pstCltParam = HI_NULL;
        HI_U8 aucRawMac[HI_PLC_MAC_ADDR_LEN] = {0x00,0x00,0xc0,0xa8,0x01,0x65};
        HI_U8 aucLocalMac[HI_PLC_MAC_ADDR_LEN] = {0};
        HI_U16 ausNvId[] = {ID_NV_MRS_STA_RESET_TIME_INF_0, ID_NV_MRS_STA_RESET_TIME_INF_1};
        HI_U8 ucIndex = 0;

        if (mrsToolsIsTSTA())
        {
            (hi_void)memcpy_s(aucRawMac, HI_PLC_MAC_ADDR_LEN, "\x00\x00\xc0\xa8\x01\x1f", HI_PLC_MAC_ADDR_LEN);
        }

        HI_MDM_GetLocalMacAddr(aucLocalMac);
        if (mrsToolsMemEq(aucRawMac, aucLocalMac, HI_PLC_MAC_ADDR_LEN))
        {
            ulRet = HI_ERR_NOT_SUPPORT;
            break;
        }

        pBuffer = (HI_PBYTE)mrsToolsMalloc(ulNvItemSizeMax);
        if (!pBuffer)
        {
            ulRet = HI_ERR_MALLOC_FAILUE;
            break;
        }

        (hi_void)memset_s(pBuffer, ulNvItemSizeMax, 0, ulNvItemSizeMax);
        pstCfgInf = (NV_APP_STA_DUTY_CYCLE_CFG_INFO *)pBuffer;
        ulRet = HI_MDM_NV_Read(ID_NV_MRS_DUTY_CYCLE_CFG_INFO, pstCfgInf, sizeof(NV_APP_STA_DUTY_CYCLE_CFG_INFO));
        if (HI_ERR_SUCCESS != ulRet)
        {
            break;
        }

        if (!pstCfgInf->ucEnable)
        {
            // 特性使能开关关闭，退出
            ulRet = HI_ERR_NOT_SUPPORT;
            break;
        }

        g_pstStaDutyCycleCtx = (MRS_SRV_STA_DUTY_CYCLE_CONTEXT *)mrsToolsMalloc(sizeof(MRS_SRV_STA_DUTY_CYCLE_CONTEXT));
        if (!g_pstStaDutyCycleCtx)
        {
            ulRet = HI_ERR_MALLOC_FAILUE;
            break;
        }

        pstCtx = g_pstStaDutyCycleCtx;
        (hi_void)memset_s(pstCtx, sizeof(MRS_SRV_STA_DUTY_CYCLE_CONTEXT), 0, sizeof(MRS_SRV_STA_DUTY_CYCLE_CONTEXT));
        pstCtx->cIndex = MRS_STA_NV_DUTY_CYCLE_INVALID_INDEX;
        mrsStaDutyCycleLoadCfgInf(pstCtx, pstCfgInf);
/* BEGIN: Added by fengxiaomin/00209182, 2015/10/23   问题单号号:DTS2015102710230 */
        mrsMeterChgNotifyProcReg(mrsStaDutyCycleMeterChgNotify, 0);
/* END:   Added by fengxiaomin/00209182, 2015/10/23 */

        for (ucIndex = 0; ucIndex < MRS_MAX(pstCtx->ucLongTimes, pstCtx->ucShortTimes); ucIndex += MRS_STA_NV_RESET_TIME_NUM)
        {
            ulRet |= HI_MDM_NV_Read(ausNvId[ucIndex/MRS_STA_NV_RESET_TIME_NUM],
                                    (HI_PVOID)&(pstCtx->aulResetTime[ucIndex]),
                                    sizeof(NV_APP_STA_RESET_TIME_INFO));
        }

        if (HI_ERR_SUCCESS != ulRet)
        {
            break;
        }

        (hi_void)memset_s(pBuffer, ulNvItemSizeMax, 0, ulNvItemSizeMax);
        pstCltParam = (HI_NV_FTM_COLLECT_PARAM_FILE_S *)pBuffer;
        ulRet = HI_MDM_NV_Read(HI_NV_FTM_COLLECT_PARAM_FILE, pstCltParam, sizeof(HI_NV_FTM_COLLECT_PARAM_FILE_S));
        if (HI_ERR_SUCCESS != ulRet)
        {
            break;
        }

        mrsStaDutyCycleLoadMeterInf(pstCtx, pstCltParam);

        if ((pstCtx->cIndex < 0) || (pstCtx->cIndex >= pstCtx->ucDutyCycleNum))
        {
            pstCtx->cIndex = MRS_STA_NV_DUTY_CYCLE_INVALID_INDEX;
        }
    } while (0);

    mrsToolsFree(pBuffer);

    if (HI_ERR_SUCCESS != ulRet)
    {
        mrsToolsFree(g_pstStaDutyCycleCtx);
    }

    return ulRet;
}
#endif

HI_U32 mrsStaDutyCycleDeinit(HI_VOID)
{
    mrsToolsFree(g_pstStaDutyCycleCtx);
    return HI_ERR_SUCCESS;
}


MRS_SRV_STA_DUTY_CYCLE_CONTEXT *mrsStaDutyCycleCtx(HI_VOID)
{
    return g_pstStaDutyCycleCtx;
}

#if defined(PRODUCT_CFG_CLTI_SUPPORT_MRS_SWITCH)
HI_VOID mrsStaDutyCycleLoadCfgInf(MRS_SRV_STA_DUTY_CYCLE_CONTEXT *pstCtx, NV_APP_STA_DUTY_CYCLE_CFG_INFO *pstCfgInf)
{
    HI_U8 i;

    pstCtx->ucShortDuration = pstCfgInf->ucShortDuration;
    pstCtx->ucShortTimes = pstCfgInf->ucShortTimes;
    pstCtx->ucLongDuration = pstCfgInf->ucLongDuration;
    pstCtx->ucLongTimes = pstCfgInf->ucLongTimes;

    if (0 == pstCtx->ucShortDuration)
    {
        pstCtx->ucShortDuration = MRS_STA_DUTY_CYCLE_SHORT_DURATION_DEFAULT;
    }

    if ((0 == pstCtx->ucShortTimes) || (pstCtx->ucShortTimes > MRS_STA_DUTY_CYCLE_RESET_TIME_MAX))
    {
        pstCtx->ucShortTimes = MRS_STA_DUTY_CYCLE_SHORT_TIMES_DEFAULT;
    }

    if (0 == pstCtx->ucLongDuration)
    {
        pstCtx->ucLongDuration = MRS_STA_DUTY_CYCLE_LONG_DURATION_DEFAULT;
    }

    if ((0 == pstCtx->ucLongTimes) || (pstCtx->ucLongTimes > MRS_STA_DUTY_CYCLE_RESET_TIME_MAX))
    {
        pstCtx->ucLongTimes = MRS_STA_DUTY_CYCLE_LONG_TIMES_DEFAULT;
    }

    for (i = 0; i < MRS_TOOLS_ALEN(pstCfgInf->astCfgItem); i++)
    {
        if (!pstCfgInf->astCfgItem[i].ucValid)
        {
            // 遇到第一个无效节点，立即退出
            break;
        }

        (hi_void)memcpy_s(&pstCtx->astCfgItem[i], sizeof(NV_APP_STA_DUTY_CYCLE_CFG_ITEM),
            &pstCfgInf->astCfgItem[i], sizeof(NV_APP_STA_DUTY_CYCLE_CFG_ITEM));
    }

    pstCtx->ucDutyCycleNum = i;
}
#endif

HI_VOID mrsStaDutyCycleLoadMeterInf(MRS_SRV_STA_DUTY_CYCLE_CONTEXT *pstCtx, HI_NV_FTM_COLLECT_PARAM_FILE_S *pstCltParam)
{
    if (mrsToolsNormalAddr(pstCltParam->meter) && (METER_PROTO_645_2007 == pstCltParam->protocol))
    {
        pstCtx->cIndex = pstCltParam->duty_cycle_index;
    }
}

/* BEGIN: Modified by fengxiaomin/00209182, 2015/10/23   问题单号号:DTS2015102710230 */
HI_U32 mrsStaDutyCycleMeterChgNotify(HI_BOOL bChg, HI_U8 ucSlaveFlag, HI_U32 ulParam)
{
    mrsStaDutyCycleSetMeterInf(bChg, ucSlaveFlag, ulParam);

    return HI_ERR_SUCCESS;
}

HI_VOID mrsStaDutyCycleSetMeterInf(HI_BOOL bChg, HI_U8 ucSlaveFlag, HI_U32 ulParam)
{
    MRS_SRV_STA_DUTY_CYCLE_CONTEXT *pstCtx = mrsStaDutyCycleCtx();

    if (!pstCtx)
    {
        return;
    }

    MRS_NOT_USED(ulParam);

    if (ucSlaveFlag
        || (METER_PROTO_645_2007 != mrsStaGetMeterProtocol())
        || (PHYSICAL_TEST_MODE_NONE != HI_MDM_GetPhysicalTestMode()))
    {
        // 工装模式，特性不起效
        mrsStaDutyCycleDeinit();
        return;
    }

    do
    {

        if (HI_TRUE == bChg)
        {
/* END:   Modified by fengxiaomin/00209182, 2015/10/23 */
            pstCtx->cIndex = MRS_STA_NV_DUTY_CYCLE_INVALID_INDEX;

            mrsStaDutyCycleSaveCfgIndex(pstCtx);
            mrsStaDutyCycleClearResetTimeInf(pstCtx);
            pstCtx->bRecTimeOnly = HI_TRUE;
        }
        else
        {
            if (mrsStaDutyCycleIsLowestStrategy())
            {
                // 已是最低档策略
                break;
            }

            if (mrsToolsMemIsZero((HI_U8 *)pstCtx->aulResetTime, sizeof(pstCtx->aulResetTime)))
            {
                // 时间记录为空，需要记录本次启动时间
                pstCtx->bRecTimeOnly = HI_TRUE;
            }
            else if (!mrsStaDutyCycleIsPowerOffReboot())
            {
                // 非断电重启
                break;
            }
        }

        if (HI_ERR_SUCCESS == mrsStaDutyCycleDateTimeQry())
        {
            return;
        }

        // 启动查询日期、时间失败
    } while (0);

    // 使用上一次策略
    mrsStaDutyCycleSetStrategy();


    // 若流程停止，也去读一次电表掉电信息
    mrsStaDutyCyclePowerInfQryExp();
}


HI_U32 mrsStaDutyCycleGenFrame(HI_U32 ulDi, HI_PBYTE *ppFrame, HI_U16 *pusLength)
{
    MRS_PROTO645_FRAME_INF stFrameInf;
    MRS_SRV_STA_DUTY_CYCLE_CONTEXT *pstCtx = mrsStaDutyCycleCtx();
    if (!pstCtx)
    {
        return HI_ERR_NO_INITILIZATION;
    }

    (hi_void)memset_s(&stFrameInf, sizeof(stFrameInf), 0, sizeof(stFrameInf));
/* BEGIN: Modified by fengxiaomin/00209182, 2015/10/23   问题单号号:DTS2015102710230 */
    mrsStaGetMeterAddr(stFrameInf.aucAddr);
/* END:   Modified by fengxiaomin/00209182, 2015/10/23 */
    stFrameInf.ucCtrl = MRS_645_FRAME_CONTROL_READ_DATA;
    stFrameInf.ucDiSize = MRS_645_FRAME_DATA_DI_SIZE;
    stFrameInf.ulDi = ulDi;

    return mrsGen645Frame(&stFrameInf, ppFrame, pusLength);
}


HI_U32 mrsStaDutyCycleSendFrame(HI_PBYTE pFrame, HI_U16 usFrameLength)
{
    HI_U8 *pPayload = pFrame;
    HI_U16 usPayloadLength = usFrameLength;
    HI_U8 *pBuffer = HI_NULL;
    MRS_STA_ITEM *pstItem = HI_NULL;
    MRS_STA_SRV_CTX_STRU *pstStaCtx = mrsStaGetContext();

    pBuffer = (HI_U8 *)mrsToolsMalloc(sizeof(MRS_STA_ITEM) + usPayloadLength);
    if (!pBuffer)
    {
        return HI_ERR_MALLOC_FAILUE;
    }

    (hi_void)memset_s(pBuffer, sizeof(MRS_STA_ITEM) + usPayloadLength, 0, sizeof(MRS_STA_ITEM) + usPayloadLength);

    pstItem = (MRS_STA_ITEM *)pBuffer;
    pstItem->id = MRS_STA_ITEM_ID_DUTY_CYCLE;
    pstItem->buf = pBuffer + sizeof(MRS_STA_ITEM);
    pstItem->len = usPayloadLength;
    pstItem->from = MRS_STA_QUEUE_FROM_STA;
    pstItem->time_out = (HI_U8)MRS_MS_TO_100MS(mrsGet645FrameHeadTimeout(MRS_STA_RM_CFG_LOCAL));;
    (hi_void)memcpy_s(pstItem->buf, usPayloadLength, pPayload, usPayloadLength);
    pstItem->ucProtocol = METER_PROTO_645_2007;
    pstItem->bAutoStrategy = HI_TRUE;
    pstItem->ucFrameTimeout = (HI_U8)MRS_MS_TO_100MS(mrsGet645FrameInterval(MRS_STA_RM_CFG_LOCAL));

    if (!mrsStaTryEnQueue(&pstStaCtx->stQueModule, pstItem))
    {
        mrsToolsFree(pstItem);
        return HI_ERR_FAILURE;
    }

    mrsStaNotifyQueue();

    return HI_ERR_SUCCESS;
}


HI_U32 mrsStaDutyCycleMeterQry(HI_U32 ulDi)
{
    HI_PBYTE pFrame = HI_NULL;
    HI_U16 usFrameLength = 0;
    HI_U32 ulRet = HI_ERR_SUCCESS;
    MRS_SRV_STA_DUTY_CYCLE_CONTEXT *pstCtx = mrsStaDutyCycleCtx();

    if (!pstCtx)
    {
        return HI_ERR_NO_INITILIZATION;
    }

    ulRet = mrsStaDutyCycleGenFrame(ulDi, &pFrame, &usFrameLength);
    HI_DIAG_LOG_MSG_E1(MRS_FILE_LOG_FLAG_001, HI_DIAG_MT("sta query time send frame result ulRet = %d"),ulRet);
    if (HI_ERR_SUCCESS != ulRet)
    {
        return ulRet;
    }

    ulRet = mrsStaDutyCycleSendFrame(pFrame, usFrameLength);
    mrsToolsFree(pFrame);
    if (HI_ERR_SUCCESS != ulRet)
    {
        return ulRet;
    }

    pstCtx->ulCurrentDi = ulDi;

    return ulRet;
}


HI_U32 mrsStaDutyCycleDateTimeQry(HI_VOID)
{
    return mrsStaDutyCycleMeterQry(MRS_PROTO645_DI_DATE_TIME);
}


HI_U32 mrsStaDutyCycleDateTimeFrameRx(HI_U8 *pFrame, HI_U16 usFrameLength)
{
    // 期望数据长度：YYMMDDWWhhmmss(7B)
    HI_U8 ucExpectDataLength = 7;
    MRS_SRV_STA_DUTY_CYCLE_CONTEXT *pstCtx = mrsStaDutyCycleCtx();
    HI_SYS_CALENDAR_TIME_S stTime;
    HI_U8 *pData = HI_NULL;

    HI_UNREF_PARAM(usFrameLength);

    if (!pstCtx || (PHYSICAL_TEST_MODE_NONE != HI_MDM_GetPhysicalTestMode()))
    {
        return HI_ERR_NO_INITILIZATION;
    }

    if (pFrame[MRS_645_FRAME_LENGTH_OFFSET] != ucExpectDataLength + MRS_645_FRAME_DATA_DI_SIZE)
    {
        return HI_ERR_BAD_DATA;
    }

    pData = pFrame + MRS_645_FRAME_DATA_OFFSET + MRS_645_FRAME_DATA_DI_SIZE;
    if (HI_ERR_SUCCESS != MRS_TimeWeekIsValid(pData))
    {
        return HI_ERR_BAD_DATA;
    }

    stTime.sec = (HI_U8)BCD2INT(pData[0]);
    stTime.min = (HI_U8)BCD2INT(pData[1]);
    stTime.hour = (HI_U8)BCD2INT(pData[2]);
    stTime.day = (HI_U8)BCD2INT(pData[4]);
    stTime.month = (HI_U8)BCD2INT(pData[5]);
    stTime.year = (HI_U16)(BCD2INT(pData[6]) + MRS_YEAR_BASE);
    (HI_VOID)HI_MDM_MakeCalendarToSecond(&stTime, &pstCtx->ulMeterCurTime);

    if ((pstCtx->bRecTimeOnly) || (HI_ERR_SUCCESS != mrsStaDutyCyclePowerInfQry()))
    {
        mrsStaDutyCycleResetTimeProc();

        // 若流程停止，也去读一次电表掉电信息
        mrsStaDutyCyclePowerInfQryExp();
    }
    else
    {
        pstCtx->bCheckPowerInf = HI_TRUE;
    }

    return HI_ERR_SUCCESS;
}


HI_U32 mrsStaDutyCycleDateTimeTimeout(HI_VOID)
{
    HI_U32 ulRet = HI_ERR_SUCCESS;

    // 使用上一次策略
    ulRet = mrsStaDutyCycleSetStrategy();

    // 读时间不成功，也去读一次停电信息
    mrsStaDutyCyclePowerInfQryExp();


    return ulRet;
}


HI_U32 mrsStaDutyCyclePowerInfQry(HI_VOID)
{
    return mrsStaDutyCycleMeterQry(MRS_PROTO645_DI_POWER_INF);
}

// 在流程之外读取停电信息
HI_U32 mrsStaDutyCyclePowerInfQryExp(HI_VOID)
{
    if (!g_pstStaDutyCycleCtx)
    {
        g_pstStaDutyCycleCtx = (MRS_SRV_STA_DUTY_CYCLE_CONTEXT *)mrsToolsMalloc(sizeof(MRS_SRV_STA_DUTY_CYCLE_CONTEXT));
        if (!g_pstStaDutyCycleCtx)
        {
            return HI_ERR_MALLOC_FAILUE;
        }
    }

    g_pstStaDutyCycleCtx->bCheckPowerInf = HI_FALSE;
    return mrsStaDutyCycleMeterQry(MRS_PROTO645_DI_POWER_INF);
}



HI_U32 mrsStaDutyCyclePowerInfFrameRx(HI_U8 *pFrame, HI_U16 usFrameLength)
{
    // 期望数据长度：YYMMDDhhmmss + YYMMDDhhmmss (12B)
    HI_U8 ucExpectDataLength = 12;
    MRS_SRV_STA_DUTY_CYCLE_CONTEXT *pstCtx = mrsStaDutyCycleCtx();
    MRS_STA_SRV_CTX_STRU * pstStaCtx = mrsStaGetContext();
    HI_SYS_CALENDAR_TIME_S stTime;
    HI_U8 *pData = HI_NULL;

    HI_UNREF_PARAM(usFrameLength);

    if (!pstCtx || (PHYSICAL_TEST_MODE_NONE != HI_MDM_GetPhysicalTestMode()))
    {
        return HI_ERR_NO_INITILIZATION;
    }

    HI_DIAG_LOG_MSG_E0(MRS_FILE_LOG_FLAG_002, HI_DIAG_MT("sta rcv response frame"));
    if (pFrame[MRS_645_FRAME_LENGTH_OFFSET] != ucExpectDataLength + MRS_645_FRAME_DATA_DI_SIZE)
    {
        HI_DIAG_LOG_MSG_E1(MRS_FILE_LOG_FLAG_006, HI_DIAG_MT("length err"), ucExpectDataLength);
        return HI_ERR_BAD_DATA;
    }

    pData = pFrame + MRS_645_FRAME_DATA_OFFSET + MRS_645_FRAME_DATA_DI_SIZE;
 //   HI_DIAG_LOG_BUF(MRS_FILE_LOG_BUF_1000, HI_DIAG_MT("response data1"), pData, 6);//Matteo_optimize
 //   HI_DIAG_LOG_BUF(MRS_FILE_LOG_BUF_1001, HI_DIAG_MT("response data2"), pData + 6, 6);//Matteo_optimize
    if ((HI_ERR_SUCCESS != MRS_TimeIsValid(pData)) || (HI_ERR_SUCCESS != MRS_TimeIsValid(pData + 6)))
    {
        HI_DIAG_LOG_MSG_E0(MRS_FILE_LOG_FLAG_007, HI_DIAG_MT("time invalid"));
        return HI_ERR_BAD_DATA;
    }

    (HI_VOID)MRS_BCD2Time(pData + 6, &stTime);
    (HI_VOID)HI_MDM_MakeCalendarToSecond(&stTime, &pstStaCtx->stPowerInfo.ulMeterPowerOnTime);
    (HI_VOID)MRS_BCD2Time(pData, &stTime);
    (HI_VOID)HI_MDM_MakeCalendarToSecond(&stTime, &pstStaCtx->stPowerInfo.ulMeterPowerOffTime);
    pstStaCtx->stPowerInfo.ulLastReadPowerInfoTime = HI_MDM_GetSeconds();

    HI_DIAG_LOG_MSG_E2(MRS_FILE_LOG_FLAG_005, HI_DIAG_MT("meter cur time, meter poweron time "), pstCtx->ulMeterCurTime, pstCtx->ulMeterPowerOnTime);
    pstCtx->ulMeterPowerOnTime = pstStaCtx->stPowerInfo.ulMeterPowerOnTime;
    if (pstCtx->bCheckPowerInf)
    {
        pstCtx->ulMeterPowerOnTime = pstStaCtx->stPowerInfo.ulMeterPowerOnTime;
        pstCtx->bCheckPowerInf = HI_FALSE;
        if (pstCtx->ulMeterCurTime - pstCtx->ulMeterPowerOnTime > MRS_SEC_PER_MIN)
        {
            HI_DIAG_LOG_MSG_E0(MRS_FILE_LOG_FLAG_003, HI_DIAG_MT("reset time proc"));
            return mrsStaDutyCycleResetTimeProc();
        }
        else
        {
            HI_DIAG_LOG_MSG_E0(MRS_FILE_LOG_FLAG_004, HI_DIAG_MT("set strategy"));
            return mrsStaDutyCycleSetStrategy();
        }
    }
    else
    {
        mrsStaDutyCycleDeinit();
    }

    return HI_ERR_SUCCESS;

}


HI_U32 mrsStaDutyCyclePowerInfTimeout(HI_VOID)
{
    MRS_SRV_STA_DUTY_CYCLE_CONTEXT *pstCtx = mrsStaDutyCycleCtx();

    if (!pstCtx)
    {
        return HI_ERR_NO_INITILIZATION;
    }

    if (pstCtx->bCheckPowerInf)
    {
        pstCtx->bCheckPowerInf = HI_FALSE;
        return mrsStaDutyCycleSetStrategy();
    }
    else
    {
        return mrsStaDutyCycleDeinit();
    }
}


HI_VOID mrsStaDutyCycleClearResetTimeInf(MRS_SRV_STA_DUTY_CYCLE_CONTEXT *pstCtx)
{
    (hi_void)memset_s(pstCtx->aulResetTime, sizeof(pstCtx->aulResetTime), 0, sizeof(pstCtx->aulResetTime));
    (HI_VOID)HI_MDM_NV_Write(ID_NV_MRS_STA_RESET_TIME_INF_0, (HI_PVOID)pstCtx->aulResetTime,  sizeof(NV_APP_STA_RESET_TIME_INFO));
    (HI_VOID)HI_MDM_NV_Write(ID_NV_MRS_STA_RESET_TIME_INF_1, (HI_PVOID)(pstCtx->aulResetTime + MRS_STA_NV_RESET_TIME_NUM),  sizeof(NV_APP_STA_RESET_TIME_INFO));
}


HI_VOID mrsStaDutyCycleSaveResetTime(MRS_SRV_STA_DUTY_CYCLE_CONTEXT *pstCtx)
{
    HI_U16 ausNvId[] = {ID_NV_MRS_STA_RESET_TIME_INF_0, ID_NV_MRS_STA_RESET_TIME_INF_1};
    HI_U8 ucIndex = 0;

    (hi_void)memmove_s(pstCtx->aulResetTime + 1, sizeof(HI_U32) * (MRS_STA_DUTY_CYCLE_RESET_TIME_MAX - 1),
        pstCtx->aulResetTime, sizeof(HI_U32) * (MRS_STA_DUTY_CYCLE_RESET_TIME_MAX - 1));
    pstCtx->aulResetTime[0] = pstCtx->ulMeterCurTime;

    for(ucIndex = 0; ucIndex < MRS_MAX(pstCtx->ucLongTimes, pstCtx->ucShortTimes); ucIndex += MRS_STA_NV_RESET_TIME_NUM)
    {
        (HI_VOID)HI_MDM_NV_Write(ausNvId[ucIndex/MRS_STA_NV_RESET_TIME_NUM],
                                 (HI_PVOID)&pstCtx->aulResetTime[ucIndex],
                                 sizeof(NV_APP_STA_RESET_TIME_INFO));
    }
}

HI_BOOL mrsStaDutyCycleIsLowestStrategy(HI_VOID)
{
    MRS_SRV_STA_DUTY_CYCLE_CONTEXT *pstCtx = mrsStaDutyCycleCtx();
    if (pstCtx)
    {
        return ((pstCtx->cIndex + 1) == pstCtx->ucDutyCycleNum);
    }

    return HI_TRUE;
}


HI_BOOL mrsStaDutyCycleIsPowerOffReboot(HI_VOID)
{
    return HI_MDM_IsPowerCut();
}


HI_U32 mrsStaDutyCycleSetStrategy(HI_VOID)
{
    MRS_SRV_STA_DUTY_CYCLE_CONTEXT *pstCtx = mrsStaDutyCycleCtx();
    NV_APP_STA_DUTY_CYCLE_CFG_ITEM stCtrlParam;

    if (!pstCtx)
    {
        return HI_ERR_NO_INITILIZATION;
    }

    (hi_void)memset_s(&stCtrlParam, sizeof(stCtrlParam), 0, sizeof(stCtrlParam));
    if ((pstCtx->cIndex >= 0) && (pstCtx->cIndex < pstCtx->ucDutyCycleNum))
    {
        NV_APP_STA_DUTY_CYCLE_CFG_ITEM *pCfg = pstCtx->astCfgItem + pstCtx->cIndex;
        (hi_void)memcpy_s(&stCtrlParam, sizeof(NV_APP_STA_DUTY_CYCLE_CFG_ITEM), pCfg, sizeof(NV_APP_STA_DUTY_CYCLE_CFG_ITEM));
    }

    HI_MDM_SetMacDutyCycleCtrlParam(stCtrlParam.ucValid, stCtrlParam.ucDutyCycle, stCtrlParam.usMaxSendLength);
    mrsStaDutyCycleDeinit();

    return HI_ERR_SUCCESS;
}


HI_U32 mrsStaDutyCycleStrategyDecrease(HI_VOID)
{
    MRS_SRV_STA_DUTY_CYCLE_CONTEXT *pstCtx = mrsStaDutyCycleCtx();

    if (!pstCtx)
    {
        return HI_ERR_NO_INITILIZATION;
    }

    if (pstCtx->cIndex + 1 < pstCtx->ucDutyCycleNum)
    {
        pstCtx->cIndex++;
    }

    mrsStaDutyCycleSaveCfgIndex(pstCtx);
    mrsStaDutyCycleClearResetTimeInf(pstCtx);
    mrsStaDutyCycleSaveResetTime(pstCtx);

    return mrsStaDutyCycleSetStrategy();
}


HI_VOID mrsStaDutyCycleSaveCfgIndex(MRS_SRV_STA_DUTY_CYCLE_CONTEXT *pstCtx)
{
    HI_NV_FTM_COLLECT_PARAM_FILE_S stCltParam;

    (hi_void)memset_s(&stCltParam, sizeof(stCltParam), 0, sizeof(stCltParam));

    if (HI_ERR_SUCCESS != HI_MDM_NV_Read(HI_NV_FTM_COLLECT_PARAM_FILE, &stCltParam, sizeof(stCltParam)))
    {
        return;
    }

    if (pstCtx->cIndex != stCltParam.duty_cycle_index)
    {
        stCltParam.duty_cycle_index = pstCtx->cIndex;
        (HI_VOID)HI_MDM_NV_Write(HI_NV_FTM_COLLECT_PARAM_FILE, &stCltParam, sizeof(stCltParam));
    }
}


HI_U32 mrsStaDutyCycleFrameRx(HI_U8 *pFrame, HI_U16 usFrameLength)
{
    MRS_SRV_STA_DUTY_CYCLE_CONTEXT *pstCtx = mrsStaDutyCycleCtx();
    HI_U32 ulRet = HI_ERR_SUCCESS;
    HI_U16 usPos = 0;
    HI_U16 usLength = 0;
    HI_U8 ucCtrl = 0;
    HI_U8 ucDataLength = 0;
    HI_U32 ulDi = 0;
    HI_U8 i = 0;

    if (!pstCtx)
    {
        return HI_ERR_NO_INITILIZATION;
    }

    ulRet = mrsFind645Frame(pFrame, (HI_S16)usFrameLength, &usPos, &usLength);
    if (HI_ERR_SUCCESS != ulRet)
    {
        return HI_ERR_BAD_DATA;
    }

    // 功能码
    ucCtrl = pFrame[usPos + MRS_645_FRAME_CTRL_OFFSET];
    if (MRS_645_FRAME_CONTROL_READ_DATA != (ucCtrl & MRS_645_FRAME_CONTROL_MASK))
    {
        return HI_ERR_BAD_DATA;
    }

    // 否认帧
    if (ucCtrl & MRS_645_FRAME_CONTROL_DENY_MASK)
    {
        return HI_ERR_BAD_DATA;
    }

    // 数据域长度
    ucDataLength = pFrame[usPos + MRS_645_FRAME_LENGTH_OFFSET];
    if (ucDataLength < MRS_645_FRAME_DATA_DI_SIZE)
    {
        return HI_ERR_BAD_DATA;
    }

    mrs645DataDecode(pFrame + usPos + MRS_645_FRAME_DATA_OFFSET, ucDataLength);
    (hi_void)memcpy_s(&ulDi, sizeof(HI_U32), pFrame + usPos + MRS_645_FRAME_DATA_OFFSET, MRS_645_FRAME_DATA_DI_SIZE);

    if (ulDi != pstCtx->ulCurrentDi)
    {
        return HI_ERR_BAD_DATA;
    }

    for (i = 0; i < MRS_TOOLS_ALEN(g_astDutyCycleCmdTbl); i++)
    {
        if (ulDi == g_astDutyCycleCmdTbl[i].ulDi)
        {
            if (g_astDutyCycleCmdTbl[i].pRxFunc)
            {
                return g_astDutyCycleCmdTbl[i].pRxFunc(pFrame + usPos, usLength);
            }

            break;
        }
    }

    return HI_ERR_BAD_DATA;
}


HI_U32 mrsStaDutyCycleTimeout(HI_VOID)
{
    MRS_SRV_STA_DUTY_CYCLE_CONTEXT *pstCtx = mrsStaDutyCycleCtx();
    HI_U8 i;

    if (!pstCtx)
    {
        return HI_ERR_NO_INITILIZATION;
    }

    for (i = 0; i < MRS_TOOLS_ALEN(g_astDutyCycleCmdTbl); i++)
    {
        if (pstCtx->ulCurrentDi == g_astDutyCycleCmdTbl[i].ulDi)
        {
            if (g_astDutyCycleCmdTbl[i].pTimeoutFunc)
            {
                g_astDutyCycleCmdTbl[i].pTimeoutFunc();
            }

            break;
        }
    }

    return HI_ERR_SUCCESS;
}


HI_U32 mrsStaDutyCycleResetTimeProc(HI_VOID)
{
    MRS_SRV_STA_DUTY_CYCLE_CONTEXT *pstCtx = mrsStaDutyCycleCtx();

    if (!pstCtx)
    {
        return HI_ERR_NO_INITILIZATION;
    }

    if (((pstCtx->ulMeterCurTime - pstCtx->aulResetTime[pstCtx->ucShortTimes - 1]) <= (HI_U32)pstCtx->ucShortDuration * 60)
        || ((pstCtx->ulMeterCurTime - pstCtx->aulResetTime[pstCtx->ucLongTimes - 1]) <= (HI_U32)pstCtx->ucLongDuration * 60 * 60))
    {
        return mrsStaDutyCycleStrategyDecrease();
    }

    mrsStaDutyCycleSaveResetTime(pstCtx);

    return mrsStaDutyCycleSetStrategy();
}


//*****************************************************************************
//*****************************************************************************
#endif // defined(PRODUCT_CFG_PRODUCT_TYPE_STA)
HI_END_HEADER


