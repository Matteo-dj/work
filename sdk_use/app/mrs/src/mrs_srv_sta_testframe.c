//*****************************************************************************
//
//                  版权所有 (C), 2001-2015, 华为技术有限公司
//
//*****************************************************************************
//  文 件 名   : mrs_srv_sta_testframe.c
//  版 本 号   : V1.0
//  作    者   : cuiate/00233580
//  生成日期   : 2015/09/16
//  功能描述   : 辅助STA发送进入测试模式报文
//  函数列表   :
//  修改历史   :
//  1.日    期 : 2015/09/16
//    作    者 : cuiate/00233580
//    修改内容 : 创建文件
//
//*****************************************************************************

#include "mrs_common.h"
#include "mrs_cmd_msg.h"
#include "mrs_fw_tools.h"
#include "mrs_fw_log.h"
#include "mrs_fw_proto645.h"
#include "mrs_srv_res.h"
#include "mrs_srv_sta_testframe.h"
#include "mrs_srv_sta_queue.h"
HI_START_HEADER

#if (defined(PRODUCT_CFG_PRODUCT_TYPE_STA) && defined(PRODUCT_CFG_SUPPORT_TEST_CONCENTRATOR))
//*****************************************************************************
//*****************************************************************************

#define MRS_STA_TEST_MODE_FRAME_INTERVAL    (1000)
#define MRS_STA_TEST_MODE_SNID              (2)
#define MRS_STA_TEST_MODE_SNID_MIN          (1)
#define MRS_STA_TEST_MODE_SNID_MAX          (15)
#define MRS_STA_TEST_MODE_BLACKLIST_CNT     (128)
#define MRS_STA_TEST_MODE_WHITELIST_CNT     (128)

typedef struct
{
    MRS_PLC_FRAME_DATA_STRU *pstPlcFrame;
    HI_U16 usFrameInterval;
    HI_U8  ucSnId;
    HI_U8  padding;
    HI_U16 usBlackCount;
    HI_U16 usWhiteCount;
    HI_U8  aucBlackList[MRS_STA_TEST_MODE_BLACKLIST_CNT*HI_METER_ADDR_LEN];
    HI_U8  aucWhiteList[MRS_STA_TEST_MODE_WHITELIST_CNT*HI_METER_ADDR_LEN];
} MRS_STA_TEST_MODE_CONTEXT_STRU;


//*****************************************************************************

HI_PRV MRS_STA_TEST_MODE_CONTEXT_STRU *g_pstTestModeCtx = HI_NULL;
HI_PRV MRS_STA_TEST_MODE_CONTEXT_STRU *mrsStaTestModeContext(HI_VOID);
HI_PRV HI_BOOL mrsStaTestModeIsInList(HI_PBYTE pucList, HI_U16 usCount, HI_PBYTE pucMeterAddr);


MRS_STA_TEST_MODE_CONTEXT_STRU *mrsStaTestModeContext(HI_VOID)
{
    return g_pstTestModeCtx;
}


HI_VOID mrsStaSetTestModeInf(HI_U16 usInterval, HI_U8 ucSnId)
{
    MRS_STA_TEST_MODE_CONTEXT_STRU * pCtx = mrsStaTestModeContext();
    MRS_CMD_DATA_TRANSMIT_STRU * pstTransmit = HI_NULL;
    MRS_CMD_TEST_MODE_STRU * pstTestMode = HI_NULL;

    if (!pCtx)
    {
        return;
    }

    pCtx->usFrameInterval = (0 == usInterval) ? MRS_STA_TEST_MODE_FRAME_INTERVAL : usInterval;
    pCtx->ucSnId = ((ucSnId >= MRS_STA_TEST_MODE_SNID_MIN) && (ucSnId <= MRS_STA_TEST_MODE_SNID_MAX)) ? ucSnId : MRS_STA_TEST_MODE_SNID;

    pstTransmit = (MRS_CMD_DATA_TRANSMIT_STRU *)pCtx->pstPlcFrame->pucPayload;
    pstTestMode = (MRS_CMD_TEST_MODE_STRU *)pstTransmit->aucPayload;
    pstTestMode->ucSnId = pCtx->ucSnId;
}


HI_VOID mrsStaAdjustTestModeSnId(HI_U16 *pusSnId)
{
    MRS_STA_TEST_MODE_CONTEXT_STRU * pCtx = mrsStaTestModeContext();

    if (pCtx && ((*pusSnId < MRS_STA_TEST_MODE_SNID_MIN) || (*pusSnId > MRS_STA_TEST_MODE_SNID_MAX)))
    {
        *pusSnId = pCtx->ucSnId;
    }
}


HI_U32 mrsStaTestModeInit(HI_VOID)
{
    HI_U16 usLength = 0;
    MRS_STA_TEST_MODE_CONTEXT_STRU *pCtx = HI_NULL;
    MRS_PLC_FRAME_DATA_STRU *pstFrame = HI_NULL;
    MRS_CMD_DATA_TRANSMIT_STRU *pstTransmit = HI_NULL;
    MRS_CMD_TEST_MODE_STRU *pstTestMode = HI_NULL;

    if (g_pstTestModeCtx)
    {
        return HI_ERR_INITILIZATION_ALREADY;
    }

    pCtx = (MRS_STA_TEST_MODE_CONTEXT_STRU *)mrsToolsMalloc(sizeof(MRS_STA_TEST_MODE_CONTEXT_STRU));
    if (!pCtx)
    {
        return HI_ERR_NOT_ENOUGH_MEMORY;
    }

    usLength = sizeof(MRS_CMD_DATA_TRANSMIT_STRU) + sizeof(MRS_CMD_TEST_MODE_STRU);
    pstTransmit = (MRS_CMD_DATA_TRANSMIT_STRU *)mrsToolsMalloc(usLength);
    if (!pstTransmit)
    {
        mrsToolsFree(pCtx);
        return HI_ERR_NOT_ENOUGH_MEMORY;
    }

    pstFrame = (MRS_PLC_FRAME_DATA_STRU *)mrsToolsMalloc(sizeof(MRS_PLC_FRAME_DATA_STRU));
    if (!pstFrame)
    {
        mrsToolsFree(pstTransmit);
        mrsToolsFree(pCtx);
        return HI_ERR_NOT_ENOUGH_MEMORY;
    }

    (hi_void)memset_s(pCtx, sizeof(MRS_STA_TEST_MODE_CONTEXT_STRU), 0, sizeof(MRS_STA_TEST_MODE_CONTEXT_STRU));
    (hi_void)memset_s(pstTransmit, usLength, 0, usLength);

    pstTestMode = (MRS_CMD_TEST_MODE_STRU *)pstTransmit->aucPayload;
    pstTestMode->usInterfaceVer = MRS_CMD_TEST_MODE_UP_VER;
    pstTestMode->usStruLen = sizeof(MRS_CMD_TEST_MODE_STRU);
    pstTestMode->ucSnId = MRS_STA_TEST_MODE_SNID;

    pstTransmit->usCmdId = ID_MRS_CMD_DATA_TRANSMIT_TEST_MODE;
    pstTransmit->usPayloadLen = sizeof(MRS_CMD_TEST_MODE_STRU);

    (hi_void)memset_s(pstFrame, sizeof(MRS_PLC_FRAME_DATA_STRU), 0, sizeof(MRS_PLC_FRAME_DATA_STRU));
    pstFrame->usId = ID_MRS_CMD_DATA_TRANSMIT_PLC;
    (hi_void)memcpy_s(pstFrame->ucMac, HI_PLC_MAC_ADDR_LEN, MRS_BROADCAST_MAC, HI_PLC_MAC_ADDR_LEN);
    pstFrame->ucOption = MRS_STA_TEST_MODE_SNID_MIN;
    pstFrame->usPayloadLen = usLength;
    pstFrame->pucPayload = (HI_PBYTE)pstTransmit;

    pCtx->pstPlcFrame = pstFrame;
    pCtx->ucSnId = MRS_STA_TEST_MODE_SNID;
    pCtx->usFrameInterval = MRS_STA_TEST_MODE_FRAME_INTERVAL;

    MRS_StartTimer(MRS_STA_TIMER_TEST_MODE_FRAME, pCtx->usFrameInterval, HI_SYS_TIMER_ONESHOT);

    g_pstTestModeCtx = pCtx;

    return HI_ERR_SUCCESS;
}


HI_U32 mrsStaTestModeDeinit(HI_VOID)
{
    if (g_pstTestModeCtx)
    {
        mrsToolsFree(g_pstTestModeCtx->pstPlcFrame->pucPayload);
        mrsToolsFree(g_pstTestModeCtx->pstPlcFrame);
        mrsToolsFree(g_pstTestModeCtx);
    }

    return HI_ERR_SUCCESS;
}


HI_U32 mrsStaTestModeTimeout(HI_VOID)
{
    MRS_STA_TEST_MODE_CONTEXT_STRU * pCtx = mrsStaTestModeContext();

    if (!pCtx)
    {
        return HI_ERR_NO_INITILIZATION;
    }

    MRS_SendPlcFrame(pCtx->pstPlcFrame);
    MRS_StartTimer(MRS_STA_TIMER_TEST_MODE_FRAME, pCtx->usFrameInterval, HI_SYS_TIMER_ONESHOT);

    pCtx->pstPlcFrame->ucOption++;
    if (pCtx->pstPlcFrame->ucOption > MRS_STA_TEST_MODE_SNID_MAX)
    {
        pCtx->pstPlcFrame->ucOption = MRS_STA_TEST_MODE_SNID_MIN;
    }

    return HI_ERR_SUCCESS;
}


HI_VOID mrsStaTestModeBlackListInsert(HI_PBYTE pFrame, HI_U16 usFrameLength)
{
    MRS_STA_TEST_MODE_CONTEXT_STRU * pCtx = mrsStaTestModeContext();
    HI_U32 ulRet = HI_ERR_SUCCESS;
    HI_U8 aucMeterAddr[HI_METER_ADDR_LEN];

    if (!pCtx)
    {
        return;
    }

    ulRet = mrsGetFrame645MeterAddr(pFrame, usFrameLength, aucMeterAddr);
    if (HI_ERR_SUCCESS != ulRet)
    {
        return;
    }

    if (mrsStaTestModeIsInWhiteList(aucMeterAddr))
    {
        return;
    }

    if (mrsStaTestModeIsInBlackList(aucMeterAddr))
    {
        return;
    }

    if (pCtx->usBlackCount < MRS_STA_TEST_MODE_BLACKLIST_CNT)
    {
        (hi_void)memcpy_s(pCtx->aucBlackList + pCtx->usBlackCount * HI_METER_ADDR_LEN, HI_METER_ADDR_LEN, aucMeterAddr, HI_METER_ADDR_LEN);
        pCtx->usBlackCount++;
    }

    mrsStaQueueBlackListProc(aucMeterAddr);
}


HI_VOID mrsStaTestModeWhiteListInsert(HI_PBYTE pFrame, HI_U16 usFrameLength)
{
    MRS_STA_TEST_MODE_CONTEXT_STRU * pCtx = mrsStaTestModeContext();
    HI_U32 ulRet = HI_ERR_SUCCESS;
    HI_U8 aucMeterAddr[HI_METER_ADDR_LEN];

    if (!pCtx)
    {
        return;
    }

    ulRet = mrsGetFrame645MeterAddr(pFrame, usFrameLength, aucMeterAddr);
    if (HI_ERR_SUCCESS != ulRet)
    {
        return;
    }

    if (mrsStaTestModeIsInWhiteList(aucMeterAddr))
    {
        return;
    }

    if (pCtx->usWhiteCount < MRS_STA_TEST_MODE_BLACKLIST_CNT)
    {
        (hi_void)memcpy_s(pCtx->aucWhiteList + pCtx->usWhiteCount * HI_METER_ADDR_LEN, HI_METER_ADDR_LEN, aucMeterAddr, HI_METER_ADDR_LEN);
        pCtx->usWhiteCount++;
    }
}


HI_BOOL mrsStaTestModeIsInList(HI_PBYTE pucList, HI_U16 usCount, HI_PBYTE pucMeterAddr)
{
    HI_U16 usIdx = 0;

    for (usIdx = 0; usIdx < usCount; usIdx++)
    {
        if (mrsToolsMemEq(pucMeterAddr, pucList + usIdx * HI_METER_ADDR_LEN, HI_METER_ADDR_LEN))
        {
            return HI_TRUE;
        }
    }

    return HI_FALSE;
}


HI_BOOL mrsStaTestModeIsInBlackList(HI_PBYTE pucMeterAddr)
{
    MRS_STA_TEST_MODE_CONTEXT_STRU * pCtx = mrsStaTestModeContext();

    if (!pCtx || mrsStaTestModeIsInWhiteList(pucMeterAddr))
    {
        return HI_FALSE;
    }

    return mrsStaTestModeIsInList(pCtx->aucBlackList, pCtx->usBlackCount, pucMeterAddr);
}


HI_BOOL mrsStaTestModeIsInWhiteList(HI_PBYTE pucMeterAddr)
{
    MRS_STA_TEST_MODE_CONTEXT_STRU * pCtx = mrsStaTestModeContext();

    if (!pCtx)
    {
        return HI_TRUE;
    }

    return mrsStaTestModeIsInList(pCtx->aucWhiteList, pCtx->usWhiteCount, pucMeterAddr);
}


//*****************************************************************************
//*****************************************************************************
#endif // defined(PRODUCT_CFG_PRODUCT_TYPE_STA)
HI_END_HEADER

