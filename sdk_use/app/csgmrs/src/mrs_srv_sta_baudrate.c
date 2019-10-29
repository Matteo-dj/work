//*****************************************************************************
//
//                  版权所有 (C), 2001-2011, 华为技术有限公司
//
//*****************************************************************************
//  文 件 名   : mrs_srv_sta_baudrate.c
//  版 本 号   : V1.0 
//  作    者   : 聂松松/KF62735
//  生成日期   : 2012-07-11
//  功能描述   : 测试波特率
//  函数列表   : NA
//  修改历史   : 
//  1.日    期 : 2012-07-11
//    作    者 : 聂松松/KF62735
//    修改内容 : 创建文件 
//*****************************************************************************
#include "mrs_common.h"
#include "mrs_srv_common.h"
#include "mrs_srv_res.h"
#include "mrs_fw_tools.h"
#include "mrs_fw_log.h"
#include "mrs_fw_proto376_2_echo.h"
#include "mrs_srv_sta_baudrate.h"
#include "mrs_srv_sta_queue.h"
#include "mrs_srv_parallel_sta.h"
#include "mrs_srv_sta_event.h"
#include "mrs_srv_sta.h"
#include "dfx_sys.h"
#include "mrs_srv_io.h"
#include "equip_dut_proc.h"
#include "mrs_srv_sta_searchmeter.h"
#include "hi_mdm.h"
#include "mrs_srv_clti_searchmeter.h"
#include "mrs_dfx_clt.h"
#include "mrs_srv_baudrate_manage.h"
#include "mrs_srv_collector.h"

#if defined(PRODUCT_CFG_PRODUCT_TYPE_STA)

HI_PRV MRS_STA_DETECT_STRU g_stDetectCtx = {0, 0, 0};

MRS_STA_DETECT_STRU* mrsStaDetectGetContext(HI_VOID)
{
    return &g_stDetectCtx;
}


HI_VOID mrsStaDetectBaudRate(HI_VOID)
{
    HI_U16 usSendLen = 0;
    HI_U8* pucSendBuf = HI_NULL;
    static HI_BOOL s_bFail = HI_FALSE;
    MRS_STA_SRV_CTX_STRU * pstSta = mrsStaGetContext();
    MRS_STA_DETECT_STRU* pstDetect = mrsStaDetectGetContext();

    if (pstDetect->ucRetry < pstSta->detect_loop_max * 4)
    {    
        if (mrsToolsIsSTA())
        {
            P_MRS_SRV_BAUDRATE_CFG_S pstBausRateCfg = mrsSrvGetBaudRateCfg(MRS_SRV_ID_BAUDRATE_DETECT);
            HI_U16 usBaudRateListDefault[] = {MRS_SRV_BAUD_RATE_DEFAULT,MRS_SRV_BAUD_RATE_9600,MRS_SRV_BAUD_RATE_1200,MRS_SRV_BAUD_RATE_4800};
            HI_U8 ucMaxBaudRateNum = MRS_TOOLS_ALEN(usBaudRateListDefault);
            HI_U16 *usBaudRateList = usBaudRateListDefault;

            if (pstBausRateCfg)
            {
                usBaudRateList = pstBausRateCfg->usBaudRateList;
                ucMaxBaudRateNum = pstBausRateCfg->ucValidNum;
            }
            
            pstDetect->usBaudRate = usBaudRateList[(pstDetect->ucRetry / 2) % ucMaxBaudRateNum];

            MRS_ChangeBaudRate(pstDetect->usBaudRate);
            mrsDfxCltSetBaudRate(pstDetect->usBaudRate);
        }        

        pucSendBuf = mrsGetSendAddrFrame(&usSendLen);
        MRS_SendMrData(pucSendBuf, usSendLen, HI_DMS_CHL_UART_PORT_APP);        
        MRS_StartTimer(MRS_STA_TIMER_TEST, pstSta->detect_timeout, HI_SYS_TIMER_ONESHOT);
        
        pstDetect->ucRetry++;        
        return;
    }

    if (mrsToolsIsSTA())
    {
        MRS_ChangeBaudRate(MRS_SRV_BAUD_RATE_DEFAULT);
        mrsDfxCltSetBaudRate(MRS_SRV_BAUD_RATE_DEFAULT);
    }

    if (HI_FALSE == s_bFail)
    {
        HI_U8 aucMacAddr[HI_PLC_MAC_ADDR_LEN] = { 0 };
        HI_MDM_GetLocalMacAddr(aucMacAddr);
        if (mrsToolsIsTSTA())
        {
            mrsStaSetMeterToMac(aucMacAddr, STA_TYPE_INVALID, HI_MDM_NV_MAC);
        }
#if !defined(PRODUCT_CFG_PRODUCT_TYPE_CSG_GD)
        else if (mrsToolsIsCLTI())
        {
            HI_U8 aucCltAddr[HI_METER_ADDR_LEN] = {0};
            
            mrsStaSetMeterToMac(aucMacAddr, STA_TYPE_CLTI, HI_MDM_NV_MAC);
            mrsStaSetCltiType(HI_TRUE);   //设置为I采类型
            (hi_void)memset_s(aucCltAddr, HI_METER_ADDR_LEN, 0xbb, HI_METER_ADDR_LEN);
            mrsSetCollectorAddr(aucCltAddr);
            mrsCltiTryGetAddrForMAC();
        }
#endif
        else
        {
            mrsStaSetType(STA_TYPE_METER);
            mrsStaSetMeterToMac(aucMacAddr, STA_TYPE_METER, HI_MDM_NV_MAC);
        }

        HI_MDM_DevIdConfig(aucMacAddr);
        HI_MDM_LED_SetReadMeterAddrStatus(LED_READ_METER_FAIL);
        
        s_bFail = HI_TRUE;
    }
    
    pstDetect->ucRetry = 0;
    MRS_StartTimer(MRS_STA_TIMER_TEST, MRS_SEC_TO_MSEC * (pstSta->detect_period), HI_SYS_TIMER_ONESHOT);

    return;
}

#if defined(PRODUCT_CFG_PRODUCT_TYPE_CSG_GD)
#define MRS_DETECT_RETRY_MAX    (3)
HI_VOID mrsCltIDetectBaudRate(HI_VOID)
{
    HI_U8 aucDetectFrame[] = {0xFE,0xFE,0xFE,0xFE,0x68,0x0C,0x00,0xC0,0x21,0x00,0x03,0x21,0x06,0xEA,0xF5,0x16};
    HI_U16 usSendLen = (HI_U16)sizeof(aucDetectFrame);
    HI_U8* pucSendBuf = aucDetectFrame;
    MRS_STA_SRV_CTX_STRU * pstSta = mrsStaGetContext();
    MRS_STA_DETECT_STRU* pstDetect = mrsStaDetectGetContext();
    static HI_BOOL s_bFail = HI_FALSE;

    if (MRS_SRV_BAUD_RATE_9600 != pstDetect->usBaudRate)
    {
        pstDetect->usBaudRate = MRS_SRV_BAUD_RATE_9600;
        MRS_ChangeBaudRate(pstDetect->usBaudRate);
        mrsDfxCltSetBaudRate(pstDetect->usBaudRate);
    }

    if (pstDetect->ucRetry < MRS_DETECT_RETRY_MAX)
    {    
        MRS_SendMrData(pucSendBuf, usSendLen, HI_DMS_CHL_UART_PORT_APP);        
        MRS_StartTimer(MRS_STA_TIMER_TEST, pstSta->detect_timeout, HI_SYS_TIMER_ONESHOT);
        
        pstDetect->ucRetry++;        
        return;
    }

    if (HI_FALSE == s_bFail)
    {
        HI_U8 aucMacAddr[HI_PLC_MAC_ADDR_LEN] = { 0 };
        HI_MDM_GetLocalMacAddr(aucMacAddr);
        mrsStaSetMeterToMac(aucMacAddr, STA_TYPE_CLTI, HI_MDM_NV_MAC);
        mrsStaSetCltiType(HI_TRUE);
       
        HI_MDM_DevIdConfig(aucMacAddr);
        HI_MDM_LED_SetReadMeterAddrStatus(LED_READ_METER_FAIL);

        s_bFail = HI_TRUE;
    }

    mrsStaSearchStart(HI_TRUE);
    
    return;
}
#endif

//*****************************************************************************
// 函数名称: mrsStaSearchMeter
// 功能描述: 测试表（采集器）波特率    
// 参数说明:
//   HI_VOID
// 返 回 值: 
//   HI_VOID
// 调用要求: 无
// 调用举例: 无
// 作    者: niesongsong/nkf62735 [2012-05-29]
//*****************************************************************************
HI_VOID mrsStaSearchMeter(HI_VOID)
{
    if (MRS_STA_STATE_INIT != mrsStaGetState())
    {
        return;
    }

    if(mrsStaGetSupportVM())
    {
        mrsStaSetState(MRS_STA_STATE_MR);
        MRS_StartTimer(MRS_STA_TIMER_HEART, MRS_STA_TIME_HEART, HI_SYS_TIMER_PERIODIC);
        HI_MDM_LED_SetReadMeterAddrStatus(LED_READ_METER_SUCCESS);

        return;
    }

    if (mrsToolsIsIICollector())
    {
        MRS_COLLECTOR_CTX_STRU *pstCltIICtx = mrsGetCollectorCtx();
        
        mrsStaSetState(MRS_STA_STATE_MR);

        if (pstCltIICtx->ucPowerSearchEnable)
        {
            HI_MDM_LED_SetReadMeterAddrStatus(LED_READ_METER_SUCCESS);
            mrsStaSearchStart(HI_TRUE);
        }
        else
        {
            HI_MDM_LED_SetReadMeterAddrStatus(LED_READ_METER_FAIL);
        }
        
        return;
    }

#if defined(PRODUCT_CFG_PRODUCT_TYPE_CSG_GD)
    if (mrsToolsIsCLTI())
    {
        mrsCltIDetectBaudRate();
    }
    else
#endif
    {
        mrsStaDetectBaudRate();
    }
}

#endif

