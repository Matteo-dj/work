//*****************************************************************************
//
//                  版权所有 (C), 1998-2014, 华为技术有限公司
//
//*****************************************************************************
//  文 件 名   : mrs_srv_clti_searchmeter.c
//  版 本 号   : V1.0 
//  作    者   : liupeidong/00238134
//  生成日期   : 2014-03-18
//  功能描述   : I型采集器搜表相关函数及接口定义
//               
//  函数列表   : TODO: ...
//  修改历史   : 
//  1.日    期 : 2014-03-18
//    作    者 : liupeidong/00238134
//    修改内容 : 创建文件 
//
//*****************************************************************************
#include "mrs_common.h"
#include "mrs_cmd_msg.h"
#include "mrs_srv_common.h"
#include "mrs_fw_tools.h"
#include "mrs_fw_log.h"
#include "app_nv.h"
#include "mrs_srv_sta_queue.h"
#include "mrs_srv_parallel_sta.h"
#include "mrs_srv_sta_event.h"
#include "mrs_srv_sta.h"
#include "mrs_srv_sta_searchmeter.h"
#include "mrs_srv_clti_searchmeter.h"
#include "mrs_srv_collector.h"
#include "hi_mdm.h"
#include "mrs_srv_sta_baudrate.h"
#include "mrs_fw_proto376_2_echo.h"
#include "mrs_dfx_clt.h"
#include "mrs_srv_common.h"

#if defined(PRODUCT_CFG_PRODUCT_TYPE_STA) 
#if defined(PRODUCT_CFG_CLTI_SUPPORT_MRS_SWITCH)

#define CLTI_SM_STOP_FRAME  {0xFE,0xFE,0xFE,0xFE,0x68,0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,0x68,0x13,0x04,0x3F,0x3E,0x3F,0x3E,0xDD,0x16}
#define CLTI_SM_START_FRAME {0xFE,0xFE,0xFE,0xFE,0x68,0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,0x68,0x13,0x04,0x3D,0x3E,0x3D,0x3E,0xD9,0x16}
#define CLTI_SM_QUERY_FRAME {0xFE,0xFE,0xFE,0xFE,0x68,0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,0x68,0x13,0x06,0x40,0x3E,0x40,0x3E,0x33,0x33,0x47,0x16}
#define CLTI_DETECT07_FRAME {0xFE,0xFE,0xFE,0xFE,0x68,0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,0x68,0x11,0x04,0x33,0x33,0x34,0x33,0xAE,0x16}
#define CLTI_DETECT97_FRAME {0xFE,0xFE,0xFE,0xFE,0x68,0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,0x68,0x01,0x02,0x43,0xC3,0xD5,0x16}
#define CLTI_SET_VER_FRAME  {0xFE,0xFE,0xFE,0xFE,0x68,0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,0x68,0x13,0x0A,0x33,0x3F,0x3D,0x3E,0x33,0x33,0x33,0x33,0x33,0x33,0x08,0x16}

#define FRAME_POS_H         (4)
#define FRAME_POS_A         (FRAME_POS_H + 1)
#define FRAME_POS_DI        (FRAME_POS_H + 10)
#define FRAME_POS_STX       (FRAME_POS_H + 14)
#define FRAME_POS_CNT       (FRAME_POS_H + 15)

#define CLTI_ACTIVE_TIME    (200)

HI_PRV HI_U32 mrsCltiSetVerInfo(HI_VOID);
HI_PRV HI_U32 mrsStaVerEnQueue(HI_U8 *pucData, HI_U16 usLen);

MRS_CLTI_SM_QUERY_CTX* mrsCltiGetQueryCtx(HI_VOID)
{
    MRS_STA_SEARCH_CTX *pstCtx = mrsStaSearchCtx();

    return &(pstCtx->stRunning.unClt.stCltI.stCltiQueryCtx);
}


HI_VOID mrsCltiSetQueryCtx(HI_U8 ucStart, HI_U8 ucCount, HI_U8 ucType)
{
    MRS_CLTI_SM_QUERY_CTX* pstQueryCtx = mrsCltiGetQueryCtx();
    
    pstQueryCtx->ucStart = ucStart;
    pstQueryCtx->ucCount = ucCount;
    pstQueryCtx->ucType = ucType;
}


HI_VOID mrsCltiResetSmRunning(HI_VOID)
{
    MRS_STA_SEARCH_CTX *pstCtx = mrsStaSearchCtx();
    (hi_void)memset_s(&pstCtx->stRunning.unClt.stCltI, sizeof(pstCtx->stRunning.unClt.stCltI), 0, sizeof(pstCtx->stRunning.unClt.stCltI)); 
}


HI_VOID mrsStaSetCltiType(HI_BOOL bRealClti)
{
    MRS_STA_SRV_CTX_STRU *pstSta = mrsStaGetContext();
    
    mrsStaSetType(STA_TYPE_CLTI);
    HI_MDM_SetSTAProductType(STA_TYPE_CLTI);
    
    if (mrsToolsIsSTA())
    {
        pstSta->is_real_clti = bRealClti;
    }
}


HI_BOOL mrsIsRealClti(HI_VOID)
{
    MRS_STA_SRV_CTX_STRU *pstSta = mrsStaGetContext();
    return (HI_BOOL)((mrsToolsIsSTA() && (STA_TYPE_CLTI == pstSta->sta_type) && (HI_TRUE == pstSta->is_real_clti)));
}

HI_BOOL mrsCltiSearchIsEnable(HI_VOID)
{
    MRS_STA_SEARCH_CTX *pstSmCtx = mrsStaSearchCtx();
    return (HI_BOOL)(mrsIsRealClti() && (pstSmCtx->stSearchCfg.bCltiSmEnable));
}

HI_U32 mrsCltiRx1300Proc(HI_U8* pucAddr, MRS_645_CTRL_STRU *pstCtrl)
{
    HI_PRV HI_BOOL s_bFirstTime = HI_TRUE;

    if (!s_bFirstTime)
    {
        return HI_ERR_SUCCESS;
    }

    s_bFirstTime = HI_FALSE;
    mrsStaSetCltiType(HI_TRUE);

    if (pstCtrl->ucSlaveFlag)
    {
        HI_U8 aucMac[HI_PLC_MAC_ADDR_LEN] = {0};
        MRS_STA_SRV_CTX_STRU *pstSta = mrsStaGetContext();
        
        pstSta->ucDutFlag = 1;
        HI_MDM_GetLocalMacAddr(aucMac);
        mrsStaSetMeterToMac(aucMac, STA_TYPE_CLTI, HI_MDM_NV_MAC);

        return HI_ERR_SUCCESS;
    }
    
    if (!mrsStaGetSupportVM())
    {
        HI_U8 aucCltAddr[] = {0xBB, 0xBB, 0xBB, 0xBB, 0xBB, 0xBB};
        
        (hi_void)memcpy_s(aucCltAddr, HI_METER_ADDR_LEN, pucAddr, HI_METER_ADDR_LEN);
        mrsSetCollectorAddr(aucCltAddr);
        mrsCltiWriteCLTAddress2Nv(aucCltAddr);
    }
    
    //以模块自身mac入网
    if (!mrsCltiSearchIsEnable())
    {
        HI_U8 aucMacAddr[HI_PLC_MAC_ADDR_LEN] = { 0 };
        HI_MDM_GetLocalMacAddr(aucMacAddr);

        mrsStaSetMeterToMac(aucMacAddr, STA_TYPE_CLTI, HI_MDM_NV_MAC);
        return HI_ERR_SUCCESS;
    }
    else
    {
        return mrsCltiTryGetAddrForMAC();
    }
}

//*****************************************************************************
// 函数名称: mrsCltiSetVerInfo
// 功能描述: I采模块将版本信息设置到I采
//           
// 参数说明:
//   无
//
// 返 回 值: 
//    HI_ERR_SUCCESS:  成功
//    其他: 失败
//
// 调用要求: TODO: ...
// 调用举例: TODO: ...
// 作    者: fengxiaomin/00209182 [2015-10-30]
//*****************************************************************************
HI_U32 mrsCltiSetVerInfo(HI_VOID)
{
    HI_U32 ulRet = HI_ERR_SUCCESS;
    HI_U8 aucFrame[] = CLTI_SET_VER_FRAME;
    HI_U16 usVer = 0;
    HI_U8 ucBootVer = 0;
    HI_U8 *pucCltAddr = mrsGetCollectorAddr();

    if (MRS_STA_STATE_INIT == mrsStaGetState())
    {
        return HI_ERR_SKIP;
    }
    
    do
    {
        ulRet = mrsSrvGetVerInfo(&usVer, &ucBootVer);
        if (HI_ERR_SUCCESS != ulRet)
        {
            break;
        }
        
        (hi_void)memcpy_s(aucFrame + MRS_CLTI_SET_VER_ADDR_OFFSET, sizeof(aucFrame) - MRS_CLTI_SET_VER_ADDR_OFFSET, pucCltAddr, HI_METER_ADDR_LEN);
        (hi_void)memcpy_s(aucFrame + MRS_CLTI_SET_SOFTWARE_VER_OFFSET, sizeof(aucFrame) - MRS_CLTI_SET_SOFTWARE_VER_OFFSET, &usVer, sizeof(usVer));
        aucFrame[MRS_CLTI_SET_BOOT_VER_OFFSET] = ucBootVer;
        mrs645DataEncode(aucFrame + MRS_CLTI_SET_SOFTWARE_VER_OFFSET, MRS_CLTI_VER_INFO_LEN);
        aucFrame[sizeof(aucFrame) - 2] = mrsToolsCalcCheckSum(aucFrame + MRS_PREFIX_FE_SIZE, sizeof(aucFrame) - MRS_PREFIX_FE_SIZE - 2);
        
        ulRet = mrsStaVerEnQueue(aucFrame, sizeof(aucFrame));
    } while (0);

    if (HI_ERR_SUCCESS != ulRet)
    {
        mrsCltiSmStartTx();
    }

    return ulRet;
}

//*****************************************************************************
// 函数名称: mrsStaVerEnQueue
// 功能描述: I采模块将版本信息加入队列
//           
// 参数说明:
//   pucData  数据
//   usLen    数据长度
//
// 返 回 值: 
//    HI_ERR_SUCCESS:  成功
//    其他: 失败
//
// 调用要求: TODO: ...
// 调用举例: TODO: ...
// 作    者: fengxiaomin/00209182 [2015-10-30]
//*****************************************************************************
HI_U32 mrsStaVerEnQueue(HI_U8 *pucData, HI_U16 usLen)
{
    MRS_STA_SRV_CTX_STRU *pstSta = mrsStaGetContext();
    MRS_STA_ITEM * pstItem = HI_NULL;

    HI_ASSERT(HI_NULL != pstSta);

    pstItem = (MRS_STA_ITEM *)mrsToolsMalloc(sizeof(MRS_STA_ITEM) + usLen);
    if (!pstItem)
    {
        return HI_ERR_NOT_ENOUGH_MEMORY;
    }

    (hi_void)memset_s(pstItem, sizeof(MRS_STA_ITEM) + usLen, 0, sizeof(MRS_STA_ITEM) + usLen);

    pstItem->id = MRS_STA_ITEM_ID_SET_VER_CLT_I;
    pstItem->buf = (HI_U8 *)pstItem + sizeof(MRS_STA_ITEM);
    pstItem->len = usLen;
    pstItem->from = MRS_STA_QUEUE_FROM_STA;
    pstItem->time_out = (HI_U8)MRS_MS_TO_100MS(mrsGet645FrameHeadTimeout(MRS_STA_RM_CFG_LOCAL));
    pstItem->bAutoStrategy = HI_TRUE;
    pstItem->ucFrameTimeout = (HI_U8)MRS_MS_TO_100MS(mrsGet645FrameInterval(MRS_STA_RM_CFG_LOCAL));
    pstItem->ucProtocol = METER_PROTO_645_2007;
    
    (hi_void)memcpy_s(pstItem->buf, usLen, pucData, usLen);

    if (!mrsStaTryEnQueue(&pstSta->stQueModule, pstItem))
    {
        mrsToolsFree(pstItem);
        return HI_ERR_FAILURE;
    }

    mrsStaNotifyQueue();    

    return HI_ERR_SUCCESS;
}

HI_VOID mrsCltiWriteCLTAddress2Nv(HI_U8* pucAddr)
{
    HI_U32 ulRet = HI_ERR_SUCCESS;
    HI_NV_FTM_COLLECT_PARAM_FILE_S stNV;

    (hi_void)memset_s(&stNV, sizeof(stNV), 0, sizeof(stNV));
    ulRet = HI_MDM_NV_Read(HI_NV_FTM_COLLECT_PARAM_FILE, &stNV, (HI_U16)sizeof(stNV));
    if ((HI_ERR_SUCCESS == ulRet) && !mrsToolsMemEq(stNV.CLTAddress, pucAddr, HI_METER_ADDR_LEN))
    {
        (hi_void)memcpy_s(stNV.CLTAddress, sizeof(stNV.CLTAddress), pucAddr, HI_METER_ADDR_LEN);
        (HI_VOID)HI_MDM_NV_Write(HI_NV_FTM_COLLECT_PARAM_FILE, &stNV, (HI_U16)sizeof(stNV));
    }
}


HI_U32 mrsCltiTryReadPositiveATx_V07(HI_U8* pucAddr)
{
    HI_U8 aucFrame[] = CLTI_DETECT07_FRAME;
    
    (hi_void)memcpy_s(aucFrame + FRAME_POS_A, sizeof(aucFrame) - FRAME_POS_A, pucAddr, HI_METER_ADDR_LEN);
    aucFrame[sizeof(aucFrame) - 2] = mrsToolsCalcCheckSum(&aucFrame[FRAME_POS_H], (HI_U16)(sizeof(aucFrame) - FRAME_POS_H - 2));

    return mrsCltiFrameEnQueue(aucFrame, (HI_U16)sizeof(aucFrame), MRS_CLTI_FRAME_TYPE_POSITIVE_A);
}


HI_U32 mrsCltiTryReadPositiveATx_V97(HI_U8* pucAddr, HI_U16 usDI97)
{
    HI_U8 aucFrame[] = CLTI_DETECT97_FRAME;
    
    (hi_void)memcpy_s(aucFrame + FRAME_POS_A, sizeof(aucFrame) - FRAME_POS_A, pucAddr, HI_METER_ADDR_LEN);
    
    if (usDI97)
    {
        mrs645DataEncode((HI_U8*)&usDI97, sizeof(usDI97));
        (hi_void)memcpy_s(aucFrame + FRAME_POS_DI, sizeof(aucFrame) - FRAME_POS_DI, &usDI97, sizeof(usDI97));
    }
    
    aucFrame[sizeof(aucFrame) - 2] = mrsToolsCalcCheckSum(&aucFrame[FRAME_POS_H], (HI_U16)(sizeof(aucFrame) - FRAME_POS_H - 2));

    return mrsCltiFrameEnQueue(aucFrame, (HI_U16)sizeof(aucFrame), MRS_CLTI_FRAME_TYPE_POSITIVE_A);
}


HI_VOID mrsCltiTryReadPositiveATx(HI_VOID)
{    
    MRS_STA_SRV_CTX_STRU * pstSta = mrsStaGetContext();
    MRS_STA_SEARCH_CTX *pstSearchCtx = mrsStaSearchCtx();
    MRS_SEARCH_RUNNING_CLT_I *pstRunningClti = &(pstSearchCtx->stRunning.unClt.stCltI);
    HI_U32 ulRet = HI_ERR_SUCCESS;

    if (pstRunningClti->ucTryFirstTimes >= pstSta->read_positive_A_times)
    {
        mrsStaSearchSetFirstFlag(HI_FALSE);
        mrsCltiSetVerInfo();
        return;
    }

    pstRunningClti->ucTryFirstTimes++;
    
    if (METER_PROTO_645_1997 == pstSearchCtx->stSearchCfg.ucNvProtocol)
    {
        ulRet = mrsCltiTryReadPositiveATx_V97(pstSearchCtx->stSearchCfg.aucNvMeter, pstSearchCtx->stSearchCfg.usNvDI97);
    }
    else
    {
        ulRet = mrsCltiTryReadPositiveATx_V07(pstSearchCtx->stSearchCfg.aucNvMeter);
    }

    if (HI_ERR_SUCCESS != ulRet)
    {
        MRS_StartTimer(MRS_STA_TIMER_CLTI_POSITIVE_A, CLTI_ACTIVE_TIME, HI_SYS_TIMER_ONESHOT);
    }
}


//读正向有功电能(+A)回复报文的处理
HI_VOID mrsCltiTryReadPositiveARx(MRS_645_FRAME_STRU* pstFrame)
{
    HI_BOOL bSucceed = HI_FALSE;
    MRS_STA_SRV_CTX_STRU * pstSta = mrsStaGetContext();
    MRS_STA_SEARCH_CTX * pstSmCtx = mrsStaSearchCtx();
    MRS_STA_ITEM * pstItem = HI_NULL;
    HI_U8 aucAddr[HI_METER_ADDR_LEN] = {0};
    HI_U16 us97Di = 0;

    if ((pstFrame->ucVer != pstSmCtx->stSearchCfg.ucNvProtocol) 
        || (!mrsToolsMemEq(pstFrame->ucAddr, pstSmCtx->stSearchCfg.aucNvMeter, HI_METER_ADDR_LEN)))
    {
        return;
    }

    if (pstFrame->ucVer == METER_PROTO_645_2007)
    {
        if ((pstFrame->stCtrl.ucFn == 0x11) && (pstFrame->ucDataRealmLen >= MRS_645_FRAME_DI_LEN_V07))
        {
            HI_U32 ulDi = MRS_COMBIN32(pstFrame->ucDataRealm[0], pstFrame->ucDataRealm[1], pstFrame->ucDataRealm[2], pstFrame->ucDataRealm[3]);
            
            if (MRS_DI_POSITIVE_A_V07 == ulDi)
            {
                bSucceed = HI_TRUE;
            }
        }
    }
    else if (pstFrame->ucVer == METER_PROTO_645_1997)
    {
        if ((pstFrame->stCtrl.ucFn == 0x01) && (pstFrame->ucDataRealmLen >= MRS_645_FRAME_DI_LEN_V97))
        {
            us97Di = MRS_COMBIN16(pstFrame->ucDataRealm[0], pstFrame->ucDataRealm[1]);
            
            if ((MRS_DI_POSITIVE_A_V97 == us97Di) || (pstSmCtx->stSearchCfg.usNvDI97 == us97Di))
            {
                bSucceed = HI_TRUE;
            }
        }
    }

    mrsMeterAddrPreProc(aucAddr, pstFrame->ucAddr, pstSmCtx->stSearchCfg.ucNvProtocol);
    if (bSucceed && mrsToolsNormalAddr(aucAddr))
    {
        HI_U32 ulRet = HI_ERR_SUCCESS;
        
        MRS_StopTimer(MRS_STA_TIMER_CLTI_POSITIVE_A);
        
        pstItem = mrsSrvQueueTop(&pstSta->stQueModule.stMrQueue);
        if (pstItem && pstItem->id == MRS_STA_ITEM_ID_SEARCH_METER_CLT_I)
        {
            mrsStaStopTtlTimer(pstItem);
            mrsStaTryDeQueue(&pstSta->stQueModule, mrsStaQueueFree);
        }
    
        ulRet = mrsCltiSearchMeterInsert(pstSmCtx, pstSmCtx->stSearchCfg.aucNvMeter, pstSmCtx->stSearchCfg.ucNvProtocol, us97Di);
        if ((HI_ERR_SUCCESS == ulRet) && mrsStaAllowAddMeter())
        {
            HI_U8 aucMac[HI_METER_ADDR_LEN] = {0};
            
            mrsToolsConvertMeter2Mac(pstSmCtx->stSearchCfg.aucNvMeter, pstSmCtx->stSearchCfg.ucNvProtocol, aucMac);
            mrsStaSetMacAddr(aucMac);
            HI_MDM_DevIdConfig(aucMac);
        }
        
        mrsStaSearchSetFirstFlag(HI_FALSE);
        mrsCltiSetVerInfo();
    }
}


HI_BOOL mrsCltiGetValidMeterInfo(MRS_STA_SEARCH_CTX *pstSmCtx)
{
    HI_U32 ulRet = HI_ERR_SUCCESS;
    HI_NV_FTM_COLLECT_PARAM_FILE_S stNv;
    HI_U8 aucAddr[HI_METER_ADDR_LEN] = {0};

    (hi_void)memset_s(&stNv, sizeof(stNv), 0, sizeof(stNv));

    ulRet = HI_MDM_NV_Read(HI_NV_FTM_COLLECT_PARAM_FILE, &stNv, sizeof(stNv));
    if (HI_ERR_SUCCESS != ulRet)
    {
        return HI_FALSE;
    }

    mrsMeterAddrPreProc(aucAddr, stNv.meter, stNv.protocol);

    if (mrsToolsNormalAddr(aucAddr) && 
        ((METER_PROTO_645_1997 == stNv.protocol) || (METER_PROTO_645_2007 == stNv.protocol)))
    {
        (hi_void)memcpy_s(pstSmCtx->stSearchCfg.aucNvMeter, HI_METER_ADDR_LEN, stNv.meter, HI_METER_ADDR_LEN);
        pstSmCtx->stSearchCfg.ucNvProtocol = stNv.protocol;

        return HI_TRUE;
    }

    return HI_FALSE;
}


HI_U32 mrsCltiTryGetAddrForMAC(HI_VOID)
{
    HI_U32 ulRet = HI_ERR_SUCCESS;
    MRS_STA_SEARCH_CTX * pstSmCtx = mrsStaSearchCtx();

    if (mrsCltiGetValidMeterInfo(pstSmCtx))
    {
        mrsStaSearchSetFirstFlag(HI_TRUE);
        mrsCltiTryReadPositiveATx();
    }
    else
    {
        mrsStaSearchSetFirstFlag(HI_FALSE);
        mrsCltiSetVerInfo();
    }
    
    return ulRet;
}


//发送命令到队列
HI_U32 mrsCltiFrameEnQueue(HI_U8 *pucBuf, HI_U16 usLen, HI_U8 ucType)
{
    MRS_STA_SRV_CTX_STRU *pstSta = mrsStaGetContext();
    MRS_STA_ITEM * pstItem = HI_NULL;
    HI_U16 usPayloadLen = usLen;
    HI_U8* pucPayload = pucBuf;
    HI_U8 ucCmdType = (ucType == MRS_CLTI_FRAME_TYPE_POSITIVE_A) ? MRS_STA_RM_CFG_CLTI : MRS_STA_RM_CFG_LOCAL;

    pstItem = (MRS_STA_ITEM *)mrsToolsMalloc(sizeof(MRS_STA_ITEM) + usPayloadLen);
    if (!pstItem)
    {
        return HI_ERR_NOT_ENOUGH_MEMORY;
    }

    (hi_void)memset_s(pstItem, sizeof(MRS_STA_ITEM) + usPayloadLen, 0, sizeof(MRS_STA_ITEM) + usPayloadLen);

    pstItem->id = MRS_STA_ITEM_ID_SEARCH_METER_CLT_I;
    pstItem->buf = (HI_U8 *)pstItem + sizeof(MRS_STA_ITEM);
    pstItem->len = usPayloadLen;
    pstItem->from = MRS_STA_QUEUE_FROM_STA;
    pstItem->option = ucType;
    pstItem->time_out = (HI_U8)MRS_MS_TO_100MS(mrsGet645FrameHeadTimeout(ucCmdType));
    pstItem->bAutoStrategy = HI_TRUE;
    pstItem->ucFrameTimeout = (HI_U8)MRS_MS_TO_100MS(mrsGet645FrameInterval(ucCmdType));
    pstItem->ucProtocol = METER_PROTO_645_2007;
    (hi_void)memcpy_s(pstItem->buf, usPayloadLen, pucPayload, usPayloadLen);

    if (!mrsStaTryEnQueue(&pstSta->stQueModule, pstItem))
    {
        mrsToolsFree(pstItem);
        return HI_ERR_FAILURE;
    }

    mrsStaNotifyQueue();    

    return HI_ERR_SUCCESS;
}


HI_VOID mrsCltiFrameSendPrepare(MRS_STA_ITEM * pstItem)
{
    if (pstItem)
    {
        mrsDfxCltiSmComFrameTx(pstItem->option);
    }
}


// 超时处理函数
HI_U32 mrsCltiFrameTimeoutProc(HI_VOID)
{
    MRS_STA_SRV_CTX_STRU *pstSta = mrsStaGetContext();
    MRS_STA_ITEM * pstItem = HI_NULL;
    HI_U32 ulRet = HI_ERR_SUCCESS;
    HI_U8 ucOption = 0;

    pstItem = mrsSrvQueueTop(&(pstSta->stQueModule.stMrQueue));
    if (!pstItem)
    {
        return HI_ERR_FAILURE;
    }    

    ucOption = pstItem->option;
    
    if (pstItem->id == MRS_STA_ITEM_ID_SEARCH_METER_CLT_I)
    {
        mrsStaTryDeQueue(&pstSta->stQueModule, mrsStaQueueFree);
    }

    switch (ucOption)
    {
        case MRS_CLTI_FRAME_TYPE_POSITIVE_A:
            mrsCltiTryReadPositiveATx();
            break;
            
        case MRS_CLTI_FRAME_TYPE_SM_QUERY_FIRST:
            MRS_StartTimer(MRS_STA_TIMER_CLTI_SM_QUERY, MRS_SEC_TO_MSEC * (pstSta->query_sm_interval), HI_SYS_TIMER_ONESHOT);
            break;

        case MRS_CLTI_FRAME_TYPE_SM_QUERY_FOLLOW:
            ulRet = mrsCltiSmQueryTxRetry();
            break;
            
        case MRS_CLTI_FRAME_TYPE_SM_START:
            ulRet = mrsCltiSmStartTx();
            break;
            
        case MRS_CLTI_FRAME_TYPE_SM_STOP:
            ulRet = mrsCltiSmStopTx();
            break;
            
        default:
            ulRet = HI_ERR_FAILURE;
            break;
    }

    return ulRet;
}


HI_U32 mrsCltiSmStartTx(HI_VOID)
{
    MRS_STA_SRV_CTX_STRU * pstSta = mrsStaGetContext();
    MRS_STA_SEARCH_CTX *pstCtx = mrsStaSearchCtx();
    MRS_SEARCH_RUNNING_STRU *pstRunning = &(pstCtx->stRunning);
    MRS_SEARCH_RUNNING_CLT_I *pstRunningClti = &(pstRunning->unClt.stCltI);
    MRS_STA_ITEM * pstItem = HI_NULL;
    HI_U8 aucFrame[] = CLTI_SM_START_FRAME;
    HI_U32 ulRet = HI_ERR_SUCCESS;

    MRS_StopTimer(MRS_STA_TIMER_CLTI_POSITIVE_A);
    MRS_StopTimer(MRS_STA_TIMER_CLTI_SM_QUERY);

    pstItem = mrsSrvQueueTop(&pstSta->stQueModule.stMrQueue);
    if (pstItem && pstItem->id == MRS_STA_ITEM_ID_SEARCH_METER_CLT_I)
    {
        mrsStaStopTtlTimer(pstItem);
        mrsStaTryDeQueue(&pstSta->stQueModule, mrsStaQueueFree);
    }

    if (pstRunningClti->ucStartRetry >= MRS_CLTI_SM_START_TRY_MAX)
    {
        mrsStaSearchSetStatus(MRS_SEARCH_STATUS_IDLE);
        return HI_ERR_FAILURE;
    }

    mrsStaSearchSetStatus(MRS_SEARCH_STATUS_IN_PROGRESS);
    mrsStaSearchSetEndReason(MRS_SEARCH_END_REASON_DEFAULT);
    
    ulRet = mrsCltiFrameEnQueue(aucFrame, sizeof(aucFrame), MRS_CLTI_FRAME_TYPE_SM_START);
    if (HI_ERR_SUCCESS != ulRet)
    {
        MRS_StartTimer(MRS_STA_TIMER_CLTI_SM_START, CLTI_ACTIVE_TIME, HI_SYS_TIMER_ONESHOT);
        return ulRet;
    }

    pstRunningClti->ucStartRetry++;

    return ulRet;
}


HI_U32 mrsCltiSmStopTx(HI_VOID)
{
    MRS_STA_SEARCH_CTX *pstCtx = mrsStaSearchCtx();
    MRS_SEARCH_RUNNING_CLT_I *pstRunningClti = &(pstCtx->stRunning.unClt.stCltI);
    HI_U8 aucFrame[] = CLTI_SM_STOP_FRAME;
    HI_U32 ulRet = HI_ERR_SUCCESS;

    if (pstRunningClti->ucStopRetry >= MRS_CLTI_SM_STOP_TRY_MAX)
    {
        return HI_ERR_FAILURE;
    }

    ulRet = mrsCltiFrameEnQueue(aucFrame, sizeof(aucFrame), MRS_CLTI_FRAME_TYPE_SM_STOP);
    if (HI_ERR_SUCCESS != ulRet)
    {
        MRS_StartTimer(MRS_STA_TIMER_CLTI_SM_STOP, CLTI_ACTIVE_TIME, HI_SYS_TIMER_ONESHOT);
        return ulRet;
    }
   
    pstRunningClti->ucStopRetry++;

    return HI_ERR_SUCCESS;
}


HI_U32 mrsCltiSmStopCmd(hi_void)
{
    MRS_STA_ITEM * pstItem = HI_NULL;
    MRS_STA_SRV_CTX_STRU * pstSta = mrsStaGetContext();
    
    MRS_StopTimer(MRS_STA_TIMER_CLTI_SM_QUERY);

    pstItem = mrsSrvQueueTop(&pstSta->stQueModule.stMrQueue);
    if (pstItem && pstItem->id == MRS_STA_ITEM_ID_SEARCH_METER_CLT_I)
    {
        mrsStaStopTtlTimer(pstItem);
        mrsStaTryDeQueue(&pstSta->stQueModule, mrsStaQueueFree);
    }

    mrsStaSearchSetStatus(MRS_SEARCH_STATUS_IDLE);
    
    if (NV_SIMU_METER_ENABLE == mrsStaGetSupportVM())
    {
        return HI_ERR_SKIP;
    }
    
    return mrsCltiSmStopTx();
}


HI_U32 mrsCltiSmQueryTx(HI_U8 ucStart, HI_U8 ucCount, HI_U8 ucType)
{
    HI_U8 aucFrame[] = CLTI_SM_QUERY_FRAME;
    MRS_STA_SEARCH_CTX *pstCtx = mrsStaSearchCtx();
    MRS_SEARCH_RUNNING_CLT_I *pstRunningClti = &(pstCtx->stRunning.unClt.stCltI);

    if (0 == ucStart)
    {
        return HI_ERR_SUCCESS;
    }

    aucFrame[FRAME_POS_STX] = ucStart + MRS_645_FRAME_HEX33;
    aucFrame[FRAME_POS_CNT] = ucCount + MRS_645_FRAME_HEX33;
    aucFrame[sizeof(aucFrame) - 2] = mrsToolsCalcCheckSum(&aucFrame[FRAME_POS_H], (HI_U16)(sizeof(aucFrame) - FRAME_POS_H - 2));

    mrsCltiSetQueryCtx(ucStart, ucCount, ucType);

    if (MRS_CLTI_FRAME_TYPE_SM_QUERY_FOLLOW == ucType)
    {
        if (pstRunningClti->ucQueryFollowRetry >= MRS_CLTI_SM_QUERY_FOLLOW_TRY_MAX)
        {
            return HI_ERR_SUCCESS;
        }
        else
        {
            pstRunningClti->ucQueryFollowRetry++;
        }
    }

    return mrsCltiFrameEnQueue(aucFrame, sizeof(aucFrame), ucType);
}


HI_U32 mrsCltiSmQueryTxRetry(HI_VOID)
{
    MRS_CLTI_SM_QUERY_CTX* pstQueryCtx = mrsCltiGetQueryCtx();
    HI_U32 ulRet = HI_ERR_SUCCESS;

    ulRet = mrsCltiSmQueryTx(pstQueryCtx->ucStart, pstQueryCtx->ucCount, pstQueryCtx->ucType);
    if (HI_ERR_SUCCESS != ulRet)
    {
        MRS_StartTimer(MRS_STA_TIMER_CLTI_SM_QUERY, CLTI_ACTIVE_TIME, HI_SYS_TIMER_ONESHOT);
    }

    return ulRet;
}


HI_VOID mrsCltiSmFrameRx(MRS_645_FRAME_STRU* pstFrame)
{
    MRS_STA_SRV_CTX_STRU *pstSta = mrsStaGetContext();
    MRS_STA_ITEM *pstItem = HI_NULL;
    HI_U8 ucCltiTxType = 0;
    HI_U32 ulDi = 0;
    
    if (pstFrame->ucDataRealmLen < MRS_645_FRAME_DI_LEN_V07)
    {
        return;
    }

    pstItem = mrsSrvQueueTop(&pstSta->stQueModule.stMrQueue);
    if (pstItem && pstItem->id == MRS_STA_ITEM_ID_SEARCH_METER_CLT_I)
    {
        ucCltiTxType = pstItem->option;
    }
    else
    {
        return;
    }

    ulDi = MRS_COMBIN32(pstFrame->ucDataRealm[0], pstFrame->ucDataRealm[1], pstFrame->ucDataRealm[2], pstFrame->ucDataRealm[3]);
    switch (ulDi)
    {
    case MRS_DI_CLTI_SM_START:   // START
        if (MRS_CLTI_FRAME_TYPE_SM_START == ucCltiTxType)
        {
            mrsCltiSmStartRx(pstFrame);
        }
        break;

    case MRS_DI_CLTI_SM_STOP:   // STOP
        if (MRS_CLTI_FRAME_TYPE_SM_STOP == ucCltiTxType)
        {
            mrsCltiSmStopRx(pstFrame);
        }
        break;

    case MRS_DI_CLTI_SM_QUERY:   // QUERY
        if (MRS_CLTI_FRAME_TYPE_SM_QUERY_FIRST == ucCltiTxType || MRS_CLTI_FRAME_TYPE_SM_QUERY_FOLLOW == ucCltiTxType)
        {
            mrsCltiSmQueryRx(pstFrame);
        }
        break;

    default:
        break;
    }
}


HI_U32 mrsCltiSmStartRx(MRS_645_FRAME_STRU *pstFrame)
{
    MRS_STA_SRV_CTX_STRU *pstSta = mrsStaGetContext();
    MRS_STA_ITEM *pstItem = HI_NULL;
    HI_U32 ulRet = HI_ERR_FAILURE;
    
    if (pstFrame->ucDataRealmLen != MRS_645_FRAME_DI_LEN_V07)
    {
        return HI_ERR_BAD_DATA;
    }

    pstItem = mrsSrvQueueTop(&pstSta->stQueModule.stMrQueue);
    if (pstItem && pstItem->id == MRS_STA_ITEM_ID_SEARCH_METER_CLT_I)
    {
        mrsStaStopTtlTimer(pstItem);
        mrsStaTryDeQueue(&pstSta->stQueModule, mrsStaQueueFree);
    }

    mrsDfxCltiSmComStartRx();
    MRS_StopTimer(MRS_STA_TIMER_CLTI_SM_START);
   
    if (MRS_SEARCH_STATUS_IN_PROGRESS == mrsStaSearchGetStatus())
    {
        ulRet = mrsCltiSmQueryTx(1, MRS_CLTI_SM_REPORT_MAX, MRS_CLTI_FRAME_TYPE_SM_QUERY_FIRST);
        if (HI_ERR_SUCCESS != ulRet)
        {
            MRS_StartTimer(MRS_STA_TIMER_CLTI_SM_QUERY, CLTI_ACTIVE_TIME, HI_SYS_TIMER_ONESHOT);
        }
    }
    
    return ulRet;
}


HI_U32 mrsCltiSmStopRx(MRS_645_FRAME_STRU *pstFrame)
{
    MRS_STA_SRV_CTX_STRU *pstSta = mrsStaGetContext();
    MRS_STA_ITEM * pstItem = HI_NULL;
 
    if (pstFrame->ucDataRealmLen != MRS_645_FRAME_DI_LEN_V07)
    {
        return HI_ERR_BAD_DATA;
    }

    MRS_StopTimer(MRS_STA_TIMER_CLTI_SM_STOP);

    pstItem = mrsSrvQueueTop(&pstSta->stQueModule.stMrQueue);
    if (pstItem && pstItem->id == MRS_STA_ITEM_ID_SEARCH_METER_CLT_I)
    {
        mrsStaStopTtlTimer(pstItem);
        mrsStaTryDeQueue(&pstSta->stQueModule, mrsStaQueueFree);
    }

    return HI_ERR_SUCCESS;
}


HI_U32 mrsCltiSaveSmResult(HI_U8* pucData)
{
    MRS_STA_SEARCH_CTX * pstSmCtx = mrsStaSearchCtx();
    MRS_SEARCH_METERLIST_STRU *pstMeterList = &(pstSmCtx->stMeterList);
    MRS_CLTI_SM_QUERY_CTX* pstQueryCtx = mrsCltiGetQueryCtx();
    HI_U8 ucCurrent = pucData[MRS_CLTI_REPORT_POS_THIS];
    HI_U8 ucOffset = MRS_CLTI_REPORT_POS_LIST;
    HI_U8 ucProtocol = METER_PROTO_TRANSPARENT;
    HI_U8 ucSaved = 0;
    HI_U8 ucIndex = 0;

    if (MRS_CLTI_FRAME_TYPE_SM_QUERY_FIRST == pstQueryCtx->ucType)
    {
        mrsStaSearchClearMeterList(pstSmCtx);
    }

    ucSaved = pstMeterList->ucMeterNum;
    if (ucSaved >= MRS_MAX_METERNUM)
    {
        return HI_ERR_FULL;
    }

    if (ucSaved + ucCurrent > MRS_MAX_METERNUM)
    {
        ucCurrent = MRS_MAX_METERNUM - ucSaved;
    }

    for (ucIndex = 0; ucIndex < ucCurrent; ++ucIndex)
    {
        HI_U8 *pucMeterAddr = HI_NULL;

        pucMeterAddr = pucData + ucOffset;
        ucProtocol = pucData[ucOffset + HI_METER_ADDR_LEN];
        mrsCltiSearchMeterInsert(pstSmCtx, pucMeterAddr, ucProtocol, 0);
        
        ucOffset += (HI_METER_ADDR_LEN + sizeof(ucProtocol));
    }

    return HI_ERR_SUCCESS;
}


HI_BOOL mrsCheckCltiSmQueryRxData(HI_U8* pucData, HI_U8 ucLen)
{
    HI_U8 ucTotal = 0;
    HI_U8 ucCurrent = 0;
    HI_U8 ucActual = 0;

    if ((ucLen < MRS_CLTI_QUERY_RX_DATA_INFO_LEN) || (((ucLen - MRS_CLTI_QUERY_RX_DATA_INFO_LEN) % MRS_CLTI_QUERY_RX_ADDR_PROTO_LEN) != 0))
    {
        return HI_FALSE;
    }

    ucTotal = pucData[MRS_CLTI_REPORT_POS_TOTAL];
    ucCurrent = pucData[MRS_CLTI_REPORT_POS_THIS];
    ucActual = (ucLen - MRS_CLTI_QUERY_RX_DATA_INFO_LEN) / (MRS_CLTI_QUERY_RX_ADDR_PROTO_LEN);

    // 不允许总数少于标称本帧数，不允许实际上报少于标称本帧数
/* BEGIN: Modified by fengxiaomin/00209182, 2014/4/28   问题单DTS2014042605733 */
    if ((ucTotal < ucCurrent) || (ucActual < ucCurrent))
/* END:   Modified by fengxiaomin/00209182, 2014/4/28 */
    {
        return HI_FALSE;
    }

    return HI_TRUE;    
}


HI_U32 mrsCltiSmQueryRx(MRS_645_FRAME_STRU *pst645Hrd)
{
    MRS_STA_SRV_CTX_STRU *pstSta = mrsStaGetContext();
    MRS_STA_ITEM * pstItem = HI_NULL;
    HI_U32 ulRet = HI_ERR_SUCCESS;
    HI_U8 *pucData = pst645Hrd->ucDataRealm;
    HI_U8 ucLen = pst645Hrd->ucDataRealmLen;
    HI_U8 ucStatus = 0;

    if (!mrsCheckCltiSmQueryRxData(pucData, ucLen))
    {
        return HI_ERR_BAD_FRAME;
    }

    mrsDfxCltiSmComQueryRx(pucData);
    MRS_StopTimer(MRS_STA_TIMER_CLTI_SM_QUERY);

    pstItem = mrsSrvQueueTop(&pstSta->stQueModule.stMrQueue);
    if (pstItem && pstItem->id == MRS_STA_ITEM_ID_SEARCH_METER_CLT_I)
    {
        mrsStaStopTtlTimer(pstItem);
        mrsStaTryDeQueue(&pstSta->stQueModule, mrsStaQueueFree);
    }    

    ulRet = mrsCltiSaveSmResult(pucData);
    
    ucStatus = pucData[MRS_CLTI_REPORT_POS_STATUS];
    switch(ucStatus)
    {
    case MRS_CLTI_SM_IN_PROGRESS:
        MRS_StartTimer(MRS_STA_TIMER_CLTI_SM_QUERY, MRS_SEC_TO_MSEC * (pstSta->query_sm_interval), HI_SYS_TIMER_ONESHOT);
        return ulRet;

    case MRS_CLTI_SM_FINISH:
        return mrsCltiSearchFinish(pucData);
        
    default:
        return HI_ERR_BAD_FRAME;
    }
}


HI_U32 mrsCltiSearchFinish(HI_U8* pucData)
{
    MRS_STA_SEARCH_CTX * pstSmCtx = mrsStaSearchCtx();
    MRS_STA_SRV_CTX_STRU *pstSta = mrsStaGetContext();
    HI_U32 ulRet = HI_ERR_SUCCESS;
    HI_U8 ucSaved = 0;
    HI_U8 ucTotal = 0;
    HI_U8 ucCount = 0;

    mrsStaSearchSetStatus(MRS_SEARCH_STATUS_IDLE);
    mrsStaSearchSetEndReason(MRS_SEARCH_END_REASON_FINISH);

    ucSaved = pstSmCtx->stMeterList.ucMeterNum;
    ucTotal = MRS_MIN(pucData[MRS_CLTI_REPORT_POS_TOTAL], MRS_MAX_METERNUM);
    
    if (ucSaved >= ucTotal)
    {
        mrsStaSearchCheckFirstMeterAddr(pstSmCtx);
        mrsStaRefreshLocalMeterList(&pstSta->stMeterList, &pstSmCtx->stMeterList);
        mrsStaSetMeterListToMac(&pstSmCtx->stMeterList, HI_TRUE);
        pstSmCtx->stRunning.bFirstOver = HI_TRUE;
        
        return HI_ERR_SUCCESS;
    }
    
    ucCount = MRS_MIN(ucTotal - ucSaved, MRS_CLTI_SM_REPORT_MAX);
    ulRet = mrsCltiSmQueryTx(ucSaved + 1, ucCount, MRS_CLTI_FRAME_TYPE_SM_QUERY_FOLLOW);
    if (HI_ERR_SUCCESS != ulRet)
    {
        MRS_StartTimer(MRS_STA_TIMER_CLTI_SM_QUERY, CLTI_ACTIVE_TIME, HI_SYS_TIMER_ONESHOT);
    }

    return ulRet;
}


HI_U32 mrsCltiSearchMeterInsert(MRS_STA_SEARCH_CTX *pstCtx, HI_U8 *pucMeter, HI_U8 ucProtocol, HI_U16 usSearchDi97)
{
    MRS_METER_ITEM_S stMeterInf = {0};

    (hi_void)memset_s(&stMeterInf, sizeof(stMeterInf), 0, sizeof(stMeterInf));
    (hi_void)memcpy_s(stMeterInf.ucMeterAddr, HI_METER_ADDR_LEN, pucMeter, HI_METER_ADDR_LEN);
    stMeterInf.ucProtocol = ucProtocol;
    stMeterInf.ucValidFlag = HI_TRUE;
    stMeterInf.usOption = usSearchDi97;

    return mrsStaSearchMeterInsert(pstCtx, &stMeterInf);
}


HI_U32 mrsCltiSearchStart(MRS_STA_SEARCH_CTX *pstCtx, HI_BOOL bTryFirstFlag)
{
    MRS_NOT_USED(pstCtx);
    MRS_NOT_USED(bTryFirstFlag);

    if (NV_SIMU_METER_ENABLE == mrsStaGetSupportVM())
    {
        return HI_ERR_SKIP;
    }
    
    mrsStaSearchClearMeterList(pstCtx);
    mrsCltiResetSmRunning();
    
    return mrsCltiSmStartTx();
}


HI_U32 mrsCltiSearchStop(hi_void)
{
    return mrsCltiSmStopCmd();
}

#endif
#endif
