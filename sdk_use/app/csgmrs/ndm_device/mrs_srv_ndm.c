/********************************************************************************
Copyright (C), Huawei Technologies Co., Ltd. 2012-2020. All rights reserved.
*********************************************************************************
File Name     : mrs_srv_ndm.c
Version       : Initial draft
Author        : HCT
Created Date  : 2016-10-11
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
#include "hi_driver.h"
#include "hi_wifi.h"
#include "hi_battery.h"
#include "hi_key.h"
#include "hi_lcd.h"
#include "hi_icc.h"
#include "hi_mdm_tf.h"
#include "hi_battery.h"
#include "hi_rtc.h"
#include "hi_filemanager.h"
#include <hi_eqt_pc_ndm.h>//ID_DIAG_CMD_MDM_XXX
#include <dfx_app.h>
#include "mrs_srv_ndm_collectdata.h"
#include "mrs_srv_ndm_disptime.h"
#include <hi_mdm_mac.h>
#include "app_ndm_diag.h"
#include "mrs_srv_ndm_n.h"

#ifdef PRODUCT_CFG_SUPPORT_CCT3911
#include <stdio.h>
#include <stdlib.h>

#include "mrs_misc.h"
#include "memLib.h"
#include "mrs_bsn.h"
#include "mrs_chan.h"
#endif

HI_START_HEADER




HI_PRV MRS_NDM_SRV_CTX_STRU *g_pstMrsNdmSrvCtx = HI_NULL;

#define MRS_NDM_GetBqUpgState()         (g_pstMrsNdmSrvCtx->stEqtCtx.stBqUpg.eStatus)
#define MRS_NDM_SetBqUpgState(a)        (g_pstMrsNdmSrvCtx->stEqtCtx.stBqUpg.eStatus = (a))
#define MRS_NDM_GetBqUpgResult()        (g_pstMrsNdmSrvCtx->stEqtCtx.stBqUpg.ulUpgResult)
#define MRS_NDM_SetBqUpgResult(a)       (g_pstMrsNdmSrvCtx->stEqtCtx.stBqUpg.ulUpgResult = (a))
#define MRS_NDM_GetDutType()            (g_pstMrsNdmSrvCtx->ucNdmType)
#define MRS_NDM_SetDutType(a)           (g_pstMrsNdmSrvCtx->ucNdmType = (a))
#define MRS_NDM_GetKeyMask()            (g_pstMrsNdmSrvCtx->stEqtCtx.stKeyTest.ulKeyMask)
#define MRS_NDM_SetKeyMask(a)           (g_pstMrsNdmSrvCtx->stEqtCtx.stKeyTest.ulKeyMask = (a))
#define MRS_NDM_LedOn(ioNum)            HI_IO_EXP_SetOutPutVal((ioNum), HI_IO_EXP_OUT_VAL_HIGHT)
#define MRS_NDM_LedOff(ioNum)           HI_IO_EXP_SetOutPutVal((ioNum), HI_IO_EXP_OUT_VAL_LOW)
#define MRS_NDM_WifiOn()                HI_WIFI_On()
#define MRS_NDM_WifiOff()               HI_WIFI_Off()
#define MRS_NDM_BeepOn()                HI_IO_EXP_SetOutPutVal(NDM_IO_BEEP_ENABLE, HI_IO_EXP_OUT_VAL_LOW)
#define MRS_NDM_BeepOff()               HI_IO_EXP_SetOutPutVal(NDM_IO_BEEP_ENABLE, HI_IO_EXP_OUT_VAL_HIGHT)
#define MRS_NDM_IoExpSetDirOut(ioNum)   HI_IO_EXP_SetDir((ioNum), HI_IO_EXP_DIR_OUT)




HI_VOID mrsNdmGlobalInit(HI_VOID)
{
    g_pstMrsNdmSrvCtx = HI_NULL;
}

HI_EXTERN HI_U32 mrsNvChangeMsgNotify(HI_U16 usId);

HI_VOID APP_NdmInit(HI_VOID)
{
    HI_U32 ret=HI_ERR_SUCCESS;
    if (g_pstMrsNdmSrvCtx)
    {
        return;
    }

    g_pstMrsNdmSrvCtx = (MRS_NDM_SRV_CTX_STRU *)mrsToolsMalloc(sizeof(MRS_NDM_SRV_CTX_STRU));
    if (!g_pstMrsNdmSrvCtx)
    {
        return;
    }
    (hi_void)memset_s(g_pstMrsNdmSrvCtx, sizeof(MRS_NDM_SRV_CTX_STRU), 0,sizeof(MRS_NDM_SRV_CTX_STRU));



    // Meter check mode, not support KEY & LCD.
    if (HI_MDM_IsNdmMeterAccess())
    {
        return;
    }
    ret=APP_NdmMessageInit();
    if(ret!=HI_ERR_SUCCESS)
    {
        return;
    }
    
    ret=APP_NdmDiagInit();
    if(ret!=HI_ERR_SUCCESS)
    {
        return;
    }
    // timer proc func
    g_stAppOsaTimer[MRS_NDM_TIMER_ZERO_CROSS].pFunc          = (APP_NDM_TIMER_PFN)mrsNdmZeroCrossTimeout;
    g_stAppOsaTimer[MRS_NDM_TIMER_PLC_TEST].pFunc            = (APP_NDM_TIMER_PFN)mrsNdmPlcTestTimeout;
    g_stAppOsaTimer[MRS_NDM_TIMER_SSID_SET].pFunc            = (APP_NDM_TIMER_PFN)mrsNdmSsidSetTimeout;
    g_stAppOsaTimer[MRS_NDM_TIMER_KEY_TEST].pFunc            = (APP_NDM_TIMER_PFN)mrsNdmKeyTestTimeout;
    g_stAppOsaTimer[MRS_NDM_TIMER_BEEP_CTRL].pFunc           = (APP_NDM_TIMER_PFN)mrsNdmBeepTimeout;
    g_stAppOsaTimer[MRS_NDM_TIMER_DISPLAY_FLASH].pFunc       = (APP_NDM_TIMER_PFN)mrsNdmDisplayFlashTimeout;
    g_stAppOsaTimer[MRS_NDM_TIMER_DISPLAY_ON].pFunc          = (APP_NDM_TIMER_PFN)mrsNdmDisplayOnTimeout;
    g_stAppOsaTimer[MRS_NDM_TIMER_DISPLAY_OFF].pFunc         = (APP_NDM_TIMER_PFN)mrsNdmDisplayOffTimeout;
    //g_stAppOsaTimer[MRS_NDM_TIMER_DC_HSO].pFunc              = (APP_NDM_TIMER_PFN)mrsNdmDcHsoTimeout;
    g_stAppOsaTimer[MRS_NDM_TIMER_SEND_SIGNAL].pFunc         = (APP_NDM_TIMER_PFN)mrsNdmSendSignalTimeout;
    g_stAppOsaTimer[MRS_NDM_TIMER_DISP_TIME].pFunc           = (APP_NDM_TIMER_PFN)mrsNdmDispTimeTimeout;
    g_stAppOsaTimer[MRS_NDM_TIMER_DC_SD_PERIOD].pFunc        = (APP_NDM_TIMER_PFN)mrsNdmDcSdSendPeriodCollectMsg;
    g_stAppOsaTimer[MRS_NDM_TIMER_DC_SD_PB_MONITR].pFunc     = (APP_NDM_TIMER_PFN)mrsNdmDcSdNormalTrigerTimeout;
    g_stAppOsaTimer[MRS_NDM_TIMER_DC_SD_REBOOT_CHECK].pFunc  = (APP_NDM_TIMER_PFN)mrsNdmDcSdRebootCheckTimeout;
    g_stAppOsaTimer[MRS_NDM_TIMER_DC_SD_FOLLOW].pFunc        = (APP_NDM_TIMER_PFN)mrsNdmDcSdFollowTimeout;
    g_stAppOsaTimer[MRS_NDM_TIMER_DC_SD_ZCROSS].pFunc        = (APP_NDM_TIMER_PFN)mrsNdmDcSdNotifyDoNextZcrossCollect;
    MRS_NDM_SetDutType((HI_DMS_IsTfHardWare()) ? MRS_NDM_TYPE_TRASMMITER : MRS_NDM_TYPE_RECEIVER);


    // register key press callback
    {
        HI_DRV_KEY_NUM_E eKey = HI_DRV_KEY_NUM_0;

        for (eKey = HI_DRV_KEY_NUM_0; eKey < HI_DRV_KEY_NUM_MAX; eKey++)
        {
            HI_DRV_KEY_Register(eKey, mrsNdmKeyPressCallback, (HI_U32)eKey);
        }
    }

    (HI_VOID)mrsNdmDispTimeInit();
    (HI_VOID)mrsNdmSendSignalInit();
    (HI_VOID)mrsNdmDcSdCheckRebootStart();

#ifdef PRODUCT_CFG_SUPPORT_CCT3911
    mrs_misc_init();
    mlib_init();
    mrs_bsn_init();
    mrs_eth_init();
    mrs_lsp_init();
#endif
}



/**
 @brief     Get NDM context.
 @attention \n
N/A
 @retval ::Context NDM context pointer
 @see \n
N/A
 */
MRS_NDM_SRV_CTX_STRU * mrsNdmGetContext(HI_VOID)
{
    return g_pstMrsNdmSrvCtx;
}



HI_PRV HI_U32 mrsNdmZeroCrossTimeout(HI_VOID)
{
    DIAG_CMD_EQUIP_ZERO_CROSS_ACK_STRU stZeroCrossAck;

    equip_Trace("ZeroCross timeout");

    (hi_void)memset_s(&stZeroCrossAck, sizeof(DIAG_CMD_EQUIP_ZERO_CROSS_ACK_STRU), 0,sizeof(DIAG_CMD_EQUIP_ZERO_CROSS_ACK_STRU));
    stZeroCrossAck.ret = HI_ERR_TIMEOUT;

    return HI_MDM_ReportPacket(ID_DIAG_CMD_MDM_ZERO_CROSS_TEST, HI_DIAG_CMD_INSTANCE_LOCAL,
        (HI_PBYTE)(&stZeroCrossAck), sizeof(DIAG_CMD_EQUIP_ZERO_CROSS_ACK_STRU), HI_TRUE);
}

HI_PRV HI_U32 mrsNdmPlcTestTimeout(HI_VOID)
{
    DIAG_CMD_EQUIP_PLC_ACK_STRU stPlcAck;
    MRS_NDM_SRV_CTX_STRU *pstCtx = mrsNdmGetContext();

    pstCtx->stEqtCtx.stPlcTest.ulFailCount++;

    equip_Trace("PlcTest TimeOut, fail times = %d", (pstCtx->stEqtCtx.stPlcTest.ulFailCount));

    (hi_void)memset_s(&stPlcAck, sizeof(DIAG_CMD_EQUIP_PLC_ACK_STRU), 0,sizeof(DIAG_CMD_EQUIP_PLC_ACK_STRU));
    stPlcAck.ret = HI_ERR_TIMEOUT;

    return HI_MDM_ReportPacket(ID_DIAG_CMD_MDM_PLC_TEST, HI_DIAG_CMD_INSTANCE_LOCAL,
        (HI_PBYTE)(&stPlcAck), sizeof(DIAG_CMD_EQUIP_PLC_ACK_STRU), HI_TRUE);
}

HI_PRV HI_U32 mrsNdmSsidSetTimeout(HI_VOID)
{
    HI_U32 ret = HI_ERR_SUCCESS;
    DIAG_CMD_EQUIP_SSID_SET_ACK_STRU stSsidSetAck;

    (hi_void)memset_s(&stSsidSetAck, sizeof(DIAG_CMD_EQUIP_SSID_SET_ACK_STRU), 0,sizeof(DIAG_CMD_EQUIP_SSID_SET_ACK_STRU));

    ret = MRS_NDM_WifiOn();

    equip_Trace("WIFI ON, ret = 0x%2X", ret);

    if (HI_ERR_SUCCESS != ret)
    {
        equip_Trace("HI_Expio_En_WIFI ON fail, ret = 0x%2X", ret);
    }

    stSsidSetAck.ret = ret;

    return HI_MDM_ReportPacket(ID_DIAG_CMD_MDM_SSID_SET, HI_DIAG_CMD_INSTANCE_LOCAL,
        (HI_PBYTE)(&stSsidSetAck), sizeof(DIAG_CMD_EQUIP_SSID_SET_ACK_STRU), HI_TRUE);
}

HI_PRV HI_U32 mrsNdmKeyTestTimeout(HI_VOID)
{
    HI_U32 ulKeyMask = MRS_NDM_GetKeyMask();
    DIAG_CMD_EQUIP_KEY_TEST_ACK_STRU stKeyTestAck;
    MRS_NDM_SRV_CTX_STRU *pstCtx = mrsNdmGetContext();

    (hi_void)memset_s(&stKeyTestAck, sizeof(DIAG_CMD_EQUIP_KEY_TEST_ACK_STRU), 0,sizeof(DIAG_CMD_EQUIP_KEY_TEST_ACK_STRU));

    pstCtx->stEqtCtx.stKeyTest.ulTestCount++;

    if(pstCtx->stEqtCtx.stKeyTest.ulTestCount <= pstCtx->stEqtCtx.stKeyTest.ulTotalCount)
    {
        if (ulKeyMask == ((pstCtx->stEqtCtx.stKeyTest.ulKeyResponse) & ulKeyMask))
        {
            equip_Trace("Key Test Success!");
            stKeyTestAck.ret = HI_ERR_SUCCESS;
            stKeyTestAck.keyResponse = pstCtx->stEqtCtx.stKeyTest.ulKeyResponse;
            APP_NdmTimerStop(MRS_NDM_TIMER_KEY_TEST);
            return HI_MDM_ReportPacket(ID_DIAG_CMD_MDM_KEY_TEST, HI_DIAG_CMD_INSTANCE_LOCAL,
                (HI_PBYTE)(&stKeyTestAck), sizeof(DIAG_CMD_EQUIP_KEY_TEST_ACK_STRU), HI_TRUE);
        }
        return HI_ERR_SUCCESS;
    }
    else
    {
        equip_Trace("Key Test Time Out!");
        APP_NdmTimerStop(MRS_NDM_TIMER_KEY_TEST);
        stKeyTestAck.ret = HI_ERR_TIMEOUT;
        stKeyTestAck.keyResponse = pstCtx->stEqtCtx.stKeyTest.ulKeyResponse;
        return HI_MDM_ReportPacket(ID_DIAG_CMD_MDM_KEY_TEST, HI_DIAG_CMD_INSTANCE_LOCAL,
            (HI_PBYTE)(&stKeyTestAck), sizeof(DIAG_CMD_EQUIP_KEY_TEST_ACK_STRU), HI_TRUE);
    }
}

HI_PRV HI_U32 mrsNdmBeepTimeout(HI_VOID)
{
    return MRS_NDM_BeepOff();
}

HI_PRV HI_U32 mrsNdmDisplayFlashTimeout(HI_VOID)
{
    MRS_NDM_SRV_CTX_STRU *pstCtx = mrsNdmGetContext();

    pstCtx->stEqtCtx.stDisplay.ulCount++;
    if (0 == ((pstCtx->stEqtCtx.stDisplay.ulCount) % 2))
    {
        mrsNdmDisplayOnTimeout();
    }
    else
    {
        mrsNdmDisplayOffTimeout();
    }
    return HI_ERR_SUCCESS;
}

HI_PRV HI_U32 mrsNdmDisplayOnTimeout(HI_VOID)
{
    HI_LCD_DISPLAY_INFO_S stLcdInf;

    (hi_void)memset_s(&stLcdInf, sizeof(stLcdInf), 0,sizeof(stLcdInf));

    stLcdInf.blink = HI_FALSE;
    stLcdInf.tsec = HI_SYS_WAIT_FOREVER;
    stLcdInf.pri = HI_LCD_DISPLAY_PRI_4;

    stLcdInf.data[0] = (HI_CHAR)0xEE;
    stLcdInf.data[1] = (HI_CHAR)0xEE;
    stLcdInf.data[2] = (HI_CHAR)0xEE;

    HI_DRV_LCD_DisplayPrint(HI_LCD_FUNC_ID_DATACOLLECT, &stLcdInf);

    if (MRS_NDM_TYPE_RECEIVER == MRS_NDM_GetDutType())
    {
        MRS_NDM_LedOn(NDM_LED_RECVIVER_ALARM);
    }
    else
    {
        MRS_NDM_LedOn(NDM_LED_TRANSMITTER_A);
        MRS_NDM_LedOn(NDM_LED_TRANSMITTER_B);
        MRS_NDM_LedOn(NDM_LED_TRANSMITTER_C);
        MRS_NDM_LedOn(NDM_LED_TRANSMITTER_ALARM);
    }

    HI_MDM_LED_On(NDM_LED_DATA_RECV_SEND);

    return HI_ERR_SUCCESS;
}

HI_PRV HI_U32 mrsNdmDisplayOffTimeout(HI_VOID)
{
    HI_LCD_DISPLAY_INFO_S stLcdInf;

    (hi_void)memset_s(&stLcdInf, sizeof(stLcdInf), 0,sizeof(stLcdInf));

    stLcdInf.blink = HI_FALSE;
    stLcdInf.tsec = HI_SYS_WAIT_FOREVER;
    stLcdInf.pri = HI_LCD_DISPLAY_PRI_4;

    stLcdInf.data[0] = ' ';
    stLcdInf.data[1] = ' ';
    stLcdInf.data[2] = ' ';

    HI_DRV_LCD_DisplayPrint(HI_LCD_FUNC_ID_DATACOLLECT, &stLcdInf);


    if (MRS_NDM_TYPE_RECEIVER == MRS_NDM_GetDutType())
    {
        MRS_NDM_LedOff(NDM_LED_RECVIVER_ALARM);
    }
    else
    {
        MRS_NDM_LedOff(NDM_LED_TRANSMITTER_A);
        MRS_NDM_LedOff(NDM_LED_TRANSMITTER_B);
        MRS_NDM_LedOff(NDM_LED_TRANSMITTER_C);
        MRS_NDM_LedOff(NDM_LED_TRANSMITTER_ALARM);
    }

    
    HI_MDM_LED_Off(NDM_LED_DATA_RECV_SEND);

    return HI_ERR_SUCCESS;
}

HI_VOID mrsNdmIOParamInit(HI_VOID)
{
    if (MRS_NDM_TYPE_RECEIVER == MRS_NDM_GetDutType())
    {
        /* LED-Alarm */
        MRS_NDM_IoExpSetDirOut(NDM_LED_RECVIVER_ALARM);
    }
    else
    {
        /* ABC three phase status led */
        MRS_NDM_IoExpSetDirOut(NDM_LED_TRANSMITTER_A);
        MRS_NDM_IoExpSetDirOut(NDM_LED_TRANSMITTER_B);
        MRS_NDM_IoExpSetDirOut(NDM_LED_TRANSMITTER_C);
        /* LED-Alarm */
        MRS_NDM_IoExpSetDirOut(NDM_LED_TRANSMITTER_ALARM);
    }
    /* Wifi */
    MRS_NDM_IoExpSetDirOut(NDM_IO_WIFI_ENABLE);
    /* Beep */
    MRS_NDM_IoExpSetDirOut(NDM_IO_BEEP_ENABLE);
}


HI_U32 mrsNdmMsgModeQuery(HI_U16 usId, HI_PVOID pMsgParam)
{
    HI_U32 ulRet = HI_ERR_SYSTEM_CALL_ERROR;
    HI_NV_FTM_STARTUP_CFG_S stNvCfg;
    DIAG_CMD_EQUIP_MODE_QUERY_ACK_STRU stModeQueryAck;

    HI_UNREF_PARAM(usId);
    HI_UNREF_PARAM(pMsgParam);

    equip_Trace("ModeQuery>>>>>>");

    (hi_void)memset_s(&stModeQueryAck, sizeof(stModeQueryAck), 0,sizeof(stModeQueryAck));

    ulRet = HI_MDM_NV_Read(HI_NV_FTM_STARTUP_CFG_ID, &stNvCfg, sizeof(HI_NV_FTM_STARTUP_CFG_S));
    if (HI_ERR_SUCCESS == ulRet)
    {
        equip_Trace("Mode = 0x%2X", stNvCfg.ulStatus);
        stModeQueryAck.mode = stNvCfg.ulStatus;
    }
    else
    {
        equip_Trace("Mode NV_Read fail, ret = 0x%2X", ulRet);
    }

    stModeQueryAck.ret = ulRet;

    return HI_MDM_ReportPacket(ID_DIAG_CMD_MDM_MODE_QUERY, HI_DIAG_CMD_INSTANCE_LOCAL,
        (HI_PBYTE)(&stModeQueryAck), sizeof(DIAG_CMD_EQUIP_MODE_QUERY_ACK_STRU), HI_TRUE);
}

HI_U32 mrsNdmMsgModeSet(HI_U16 usId, HI_PVOID pMsgParam)
{
    HI_U32 ret = HI_ERR_SYSTEM_CALL_ERROR;

    HI_U32* pRecvMsgParam = HI_NULL;
    DIAG_CMD_EQUIP_MODE_SET_ACK_STRU stModeSetAck;
    DIAG_CMD_EQUIP_MODE_SET_STRU* pstModeSet = HI_NULL;

    HI_UNREF_PARAM(usId);

    equip_Trace("ModeSet>>>>>>");

    (hi_void)memset_s(&stModeSetAck,sizeof(DIAG_CMD_EQUIP_MODE_SET_ACK_STRU), 0, sizeof(DIAG_CMD_EQUIP_MODE_SET_ACK_STRU));
    pRecvMsgParam = (HI_U32*)pMsgParam;

    // pRecvMsgParam[0] is content, pRecvMsgParam[2] is content size
    if (sizeof(DIAG_CMD_EQUIP_MODE_SET_STRU) != pRecvMsgParam[2])
    {
        equip_Trace("Receive len:%2d is not expected", pRecvMsgParam[2]);
        return HI_ERR_BAD_DATA;
    }

    pstModeSet = (DIAG_CMD_EQUIP_MODE_SET_STRU*)pRecvMsgParam[0];

    ret = HI_MDM_SetWorkStatus(pstModeSet->mode);
    if (HI_ERR_SUCCESS != ret)
    {
        equip_Trace("Mode NV_Write fail:");
    }
    else
    {
        equip_Trace("ModeSet, mode = 0x%2X", pstModeSet->mode);
    }

    stModeSetAck.ret = ret;

    return HI_MDM_ReportPacket(ID_DIAG_CMD_MDM_MODE_SET, HI_DIAG_CMD_INSTANCE_LOCAL,
        (HI_PBYTE)(&stModeSetAck), sizeof(DIAG_CMD_EQUIP_MODE_SET_ACK_STRU), HI_TRUE);
}

HI_U32 mrsNdmMsgMacSet(HI_U16 usId, HI_PVOID pMsgParam)
{
    HI_U32 ret = HI_ERR_SYSTEM_CALL_ERROR;

    HI_U32* pRecvMsgParam = HI_NULL;
    HI_NV_FTM_PRODUCT_ID_S stNvCfg;
    DIAG_CMD_EQUIP_MAC_SET_ACK_STRU stMacSetAck;
    DIAG_CMD_EQUIP_MAC_SET_STRU* pstMacSet = HI_NULL;

    HI_UNREF_PARAM(usId);

    equip_Trace("MsgMacSet>>>>>>");

    (hi_void)memset_s(&stMacSetAck, sizeof(DIAG_CMD_EQUIP_MAC_SET_ACK_STRU), 0, sizeof(DIAG_CMD_EQUIP_MAC_SET_ACK_STRU));
    (hi_void)memset_s(&stNvCfg, sizeof(HI_NV_FTM_PRODUCT_ID_S), 0, sizeof(HI_NV_FTM_PRODUCT_ID_S));

    pRecvMsgParam = (HI_U32*)pMsgParam;

    // pRecvMsgParam[0] is content, pRecvMsgParam[2] is content size
    if (sizeof(DIAG_CMD_EQUIP_MAC_SET_STRU) != pRecvMsgParam[2])
    {
        equip_Trace("Receive len:%2d is not expected", pRecvMsgParam[2]);
        return HI_ERR_BAD_DATA;
    }

    pstMacSet = (DIAG_CMD_EQUIP_MAC_SET_STRU*)pRecvMsgParam[0];

    equip_Trace("plcmac = 0x%2X-0x%2X-0x%2X-0x%2X-0x%2X-0x%2X",
        pstMacSet->ucMac[0],
        pstMacSet->ucMac[1],
        pstMacSet->ucMac[2],
        pstMacSet->ucMac[3],
        pstMacSet->ucMac[4],
        pstMacSet->ucMac[5]);

    //MAC set
    ret = HI_MDM_SetPlcMac(pstMacSet->ucMac);

    if (HI_ERR_SUCCESS != ret)
    {
        equip_Trace("plcmac set fail, ret = %d", ret);
    }

    stMacSetAck.ret = ret;

    return HI_MDM_ReportPacket(ID_DIAG_CMD_MDM_MAC_SET, HI_DIAG_CMD_INSTANCE_LOCAL,
        (HI_PBYTE)(&stMacSetAck), sizeof(DIAG_CMD_EQUIP_MAC_SET_ACK_STRU), HI_TRUE);
}

HI_U32 mrsNdmMsgMacQuery(HI_U16 usId, HI_PVOID pMsgParam)
{
    HI_U32 ret = HI_ERR_SYSTEM_CALL_ERROR;

    HI_NV_FTM_PRODUCT_ID_S stNvCfg;
    DIAG_CMD_EQUIP_MAC_QUERY_ACK_STRU stMacQueryAck;

    HI_UNREF_PARAM(usId);
    HI_UNREF_PARAM(pMsgParam);

    equip_Trace("MacQuery>>>>>>");

    (hi_void)memset_s(&stMacQueryAck, sizeof(DIAG_CMD_EQUIP_MAC_QUERY_ACK_STRU), 0, sizeof(DIAG_CMD_EQUIP_MAC_QUERY_ACK_STRU));
    (hi_void)memset_s(&stNvCfg, sizeof(HI_NV_FTM_PRODUCT_ID_S), 0, sizeof(HI_NV_FTM_PRODUCT_ID_S));

    ret = HI_MDM_NV_Read(HI_NV_FTM_PRODUCT_ID, &stNvCfg, sizeof(HI_NV_FTM_PRODUCT_ID_S));

    stMacQueryAck.ret = ret;

    if (HI_ERR_SUCCESS != ret)
    {
        equip_Trace("HI_MDM_NV_Read fail: 0x%2X", HI_NV_FTM_PRODUCT_ID);
    }
    else
    {
        (hi_void)memcpy_s(stMacQueryAck.ucPlcMac, HI_PLC_MAC_ADDR_LEN,stNvCfg.ucPlcMac, HI_PLC_MAC_ADDR_LEN);
        equip_Trace("plc mac query, ret = 0x%2X; plcmac = 0x%2X-0x%2X-0x%2X-0x%2X-0x%2X-0x%2X",
            ret,
            stNvCfg.ucPlcMac[0],
            stNvCfg.ucPlcMac[1],
            stNvCfg.ucPlcMac[2],
            stNvCfg.ucPlcMac[3],
            stNvCfg.ucPlcMac[4],
            stNvCfg.ucPlcMac[5]);
    }

    return HI_MDM_ReportPacket(ID_DIAG_CMD_MDM_MAC_QUERY, HI_DIAG_CMD_INSTANCE_LOCAL,
        (HI_PBYTE)(&stMacQueryAck), sizeof(DIAG_CMD_EQUIP_MAC_QUERY_ACK_STRU), HI_TRUE);
}

HI_U32 mrsNdmMsgZeroCross(HI_U16 usId, HI_PVOID pMsgParam)
{
    HI_U32 ret = HI_ERR_SYSTEM_CALL_ERROR;

    HI_U32* pRecvMsgParam = HI_NULL;
    DIAG_CMD_EQUIP_ZERO_CROSS_ACK_STRU stZeroCrossAck;
    DIAG_CMD_EQUIP_ZERO_CROSS_STRU* pstZeroCross = HI_NULL;

    HI_UNREF_PARAM(usId);

    equip_Trace("ZeroCross>>>>>>");

    (hi_void)memset_s(&stZeroCrossAck, sizeof(DIAG_CMD_EQUIP_ZERO_CROSS_ACK_STRU), 0, sizeof(DIAG_CMD_EQUIP_ZERO_CROSS_ACK_STRU));

    pRecvMsgParam = (HI_U32*)pMsgParam;

    // pRecvMsgParam[0] is content, pRecvMsgParam[2] is content size
    if (sizeof(DIAG_CMD_EQUIP_ZERO_CROSS_STRU) != pRecvMsgParam[2])
    {
        equip_Trace("Receive len:%2d is not expected", pRecvMsgParam[2]);
        return HI_ERR_BAD_DATA;
    }

    pstZeroCross = (DIAG_CMD_EQUIP_ZERO_CROSS_STRU*)pRecvMsgParam[0];

    if((EQUIP_ZERO_CROSS_LEVEL_240K != pstZeroCross->ucLevel) && (EQUIP_ZERO_CROSS_LEVEL_1M != pstZeroCross->ucLevel))
    {
        stZeroCrossAck.ret = HI_ERR_INVALID_PARAMETER;
        return HI_MDM_ReportPacket(ID_DIAG_CMD_MDM_ZERO_CROSS_TEST, HI_DIAG_CMD_INSTANCE_LOCAL,
            (HI_PBYTE)(&stZeroCrossAck), sizeof(DIAG_CMD_EQUIP_ZERO_CROSS_ACK_STRU), HI_TRUE);
    }

    ret = HI_MDM_CollectNtb(HI_TRUE, pstZeroCross->ucLevel,PHASE_A, EQUIP_ZERO_CROSS_CRICUIT_RISE, mrsNdmMsgZeroCrossNotify);
    if (HI_ERR_SUCCESS != ret)
    {
        equip_Trace("ZeroCross Register fail, ret = 0x%2X", ret);
        stZeroCrossAck.ret = ret;
        stZeroCrossAck.ucLevel = pstZeroCross->ucLevel;
        return HI_MDM_ReportPacket(ID_DIAG_CMD_MDM_ZERO_CROSS_TEST, HI_DIAG_CMD_INSTANCE_LOCAL,
            (HI_PBYTE)(&stZeroCrossAck), sizeof(DIAG_CMD_EQUIP_ZERO_CROSS_ACK_STRU), HI_TRUE);
    }

    return APP_NdmTimerStart(MRS_NDM_TIMER_ZERO_CROSS, MRS_NDM_TIME_ZERO_CROSS, HI_SYS_TIMER_ONESHOT);
}

HI_U32 mrsNdmMsgPlcTest(HI_U16 usId, HI_PVOID pMsgParam)
{
    HI_U32 ret = HI_ERR_SYSTEM_CALL_ERROR;

    HI_U32* pRecvMsgParam = HI_NULL;
    DIAG_CMD_EQUIP_PLC_ACK_STRU stPlcAck;
    DIAG_CMD_EQUIP_PLC_STRU* pstPlcInfo = HI_NULL;
    HI_PBYTE pucMsgPaload  = HI_NULL;
    HI_U32 uspucMsgPaloadSize = 0;
    HI_MDM_MSG_CTRL_S stCtrl;
    MRS_NDM_SRV_CTX_STRU *pstCtx = mrsNdmGetContext();

    HI_UNREF_PARAM(usId);

    equip_Trace("PlcTest>>>>>>");

    (hi_void)memset_s(&stPlcAck, sizeof(DIAG_CMD_EQUIP_PLC_ACK_STRU), 0, sizeof(DIAG_CMD_EQUIP_PLC_ACK_STRU));
    (hi_void)memset_s(&stCtrl, sizeof(HI_MDM_MSG_CTRL_S), 0, sizeof(HI_MDM_MSG_CTRL_S));

    pRecvMsgParam = (HI_U32*)pMsgParam;

    // pRecvMsgParam[0] is content, pRecvMsgParam[2] is content size
    if (sizeof(DIAG_CMD_EQUIP_PLC_STRU) != pRecvMsgParam[2])
    {
        equip_Trace("Receive len:%2d is not expected", pRecvMsgParam[2]);
        return HI_ERR_BAD_DATA;
    }

    pstPlcInfo = (DIAG_CMD_EQUIP_PLC_STRU*)pRecvMsgParam[0];

    (hi_void)memcpy_s(pstCtx->stEqtCtx.stPlcTest.aucMac,sizeof(pstCtx->stEqtCtx.stPlcTest.aucMac), pstPlcInfo->ucMacAddr_ndm, sizeof(pstPlcInfo->ucMacAddr_ndm));
    (hi_void)memcpy_s(stCtrl.aucMacAddr[0], sizeof(stCtrl.aucMacAddr[0]),pstPlcInfo->ucMacAddr_ndm, sizeof(pstPlcInfo->ucMacAddr_ndm));

    // register MAC's callback function
    ret = HI_MDM_RegisterNdmPingFunc(MRS_NDM_EQT_PLC_TEST, mrsNdmMsgPlcRxNotify);
    if (HI_ERR_SUCCESS != ret)
    {
        equip_Trace("HI_MDM_RegisterNdmPingFunc fail, ret = 0x%2X", ret);
        HI_MDM_Sleep(1000);
    }
    equip_Trace("RegisterNdmPingFunc");

    // PLC test, point to point delivery
    stCtrl.usTransId = HI_MDM_OBJ_ID_USR1;
    stCtrl.usDULink = HI_FALSE;
    pstCtx->stEqtCtx.stPlcTest.ulPlcTxRandom = HI_MDM_GetRandNum32(0, 999);
    pucMsgPaload = (HI_PBYTE)(&pstCtx->stEqtCtx.stPlcTest.ulPlcTxRandom);
    uspucMsgPaloadSize = sizeof(HI_U32);
    ret = HI_MDM_SendNdmPing(MRS_NDM_EQT_PLC_TEST, pucMsgPaload, uspucMsgPaloadSize, &stCtrl);
    if(HI_ERR_SUCCESS != ret)
    {
        equip_Trace("PLCTest MsgSend fail, errReturn = 0x%2X", ret);
        stPlcAck.ret = HI_ERR_FAILURE;
        return HI_MDM_ReportPacket(ID_DIAG_CMD_MDM_PLC_TEST, HI_DIAG_CMD_INSTANCE_LOCAL,
            (HI_PBYTE)(&stPlcAck), sizeof(DIAG_CMD_EQUIP_PLC_ACK_STRU), HI_TRUE);
    }

    pstCtx->stEqtCtx.stPlcTest.ulTestCount++;
    equip_Trace("Plc Test times: %d, ping 0x%2X-0x%2X-0x%2X-0x%2X-0x%2X-0x%2X ......",
        pstCtx->stEqtCtx.stPlcTest.ulTestCount,
        pstCtx->stEqtCtx.stPlcTest.aucMac[0],
        pstCtx->stEqtCtx.stPlcTest.aucMac[1],
        pstCtx->stEqtCtx.stPlcTest.aucMac[2],
        pstCtx->stEqtCtx.stPlcTest.aucMac[3],
        pstCtx->stEqtCtx.stPlcTest.aucMac[4],
        pstCtx->stEqtCtx.stPlcTest.aucMac[5]);

    return APP_NdmTimerStart(MRS_NDM_TIMER_PLC_TEST, MRS_NDM_TIME_PLC_TEST, HI_SYS_TIMER_ONESHOT);
}

HI_U32 mrsNdmMsgSsidSet(HI_U16 usId, HI_PVOID pMsgParam)
{
    HI_U32 ret = HI_ERR_SUCCESS;

    HI_U32* pRecvMsgParam = HI_NULL;
    DIAG_CMD_EQUIP_SSID_SET_STRU* pstSsidSet = HI_NULL;
    DIAG_CMD_EQUIP_SSID_SET_ACK_STRU stSsidSetAck;

    HI_UNREF_PARAM(usId);

    equip_Trace("SsidSet>>>>>>");

    (hi_void)memset_s(&stSsidSetAck, sizeof(DIAG_CMD_EQUIP_SSID_SET_ACK_STRU), 0, sizeof(DIAG_CMD_EQUIP_SSID_SET_ACK_STRU));

    pRecvMsgParam = (HI_U32*)pMsgParam;

    // pRecvMsgParam[0] is content, pRecvMsgParam[2] is content size
    if (sizeof(DIAG_CMD_EQUIP_SSID_SET_STRU) != pRecvMsgParam[2])
    {
        equip_Trace("Receive len:%2d is not expected", pRecvMsgParam[2]);
        return HI_ERR_BAD_DATA;
    }

    pstSsidSet = (DIAG_CMD_EQUIP_SSID_SET_STRU*)pRecvMsgParam[0];

    equip_Trace("Rev Ssid SN: %s", pstSsidSet->ucSn);

    // set SSID
    ret = HI_WIFI_SetSsid(pstSsidSet->ucSn);
    if (HI_ERR_SUCCESS != ret)
    {
        equip_Trace("HI_SET_NDM_WIFI_SSID fail, ret = 0x%2X", ret);
        stSsidSetAck.ret = ret;
        return HI_MDM_ReportPacket(ID_DIAG_CMD_MDM_SSID_SET, HI_DIAG_CMD_INSTANCE_LOCAL,
            (HI_PBYTE)(&stSsidSetAck), sizeof(DIAG_CMD_EQUIP_SSID_SET_ACK_STRU), HI_TRUE);
    }
    // off wifi
    ret = MRS_NDM_WifiOff();
    if (HI_ERR_SUCCESS != ret)
    {
        equip_Trace("HI_Expio_En_WIFI fail, ret = 0x%2X", ret);
        stSsidSetAck.ret = ret;
        return HI_MDM_ReportPacket(ID_DIAG_CMD_MDM_SSID_SET, HI_DIAG_CMD_INSTANCE_LOCAL,
            (HI_PBYTE)(&stSsidSetAck), sizeof(DIAG_CMD_EQUIP_SSID_SET_ACK_STRU), HI_TRUE);
    }

    return APP_NdmTimerStart(MRS_NDM_TIMER_SSID_SET, MRS_NDM_TIME_SSID_SET, HI_SYS_TIMER_ONESHOT);
}

HI_U32 mrsNdmMsgSsidQuery(HI_U16 usId, HI_PVOID pMsgParam)
{
    HI_U32 ret = HI_ERR_SUCCESS;
    DIAG_CMD_EQUIP_SSID_QUERY_ACK_STRU stSsidQueryAck;

    HI_UNREF_PARAM(usId);
    HI_UNREF_PARAM(pMsgParam);

    equip_Trace("SsidQuery>>>>>>");

    (hi_void)memset_s(&stSsidQueryAck, sizeof(DIAG_CMD_EQUIP_SSID_QUERY_ACK_STRU), 0, sizeof(DIAG_CMD_EQUIP_SSID_QUERY_ACK_STRU));

    // query SSID
    ret = HI_WIFI_GetSsid(stSsidQueryAck.ucSn);
    if (HI_ERR_SUCCESS != ret)
    {
        equip_Trace("HI_GET_NDM_WIFI_SSID fail, ret = 0x%2X", ret);
    }
    else
    {
        equip_Trace("Ssid SN: %s", stSsidQueryAck.ucSn);
    }

    stSsidQueryAck.ret = ret;

    return HI_MDM_ReportPacket(ID_DIAG_CMD_MDM_SSID_QUERY, HI_DIAG_CMD_INSTANCE_LOCAL,
        (HI_PBYTE)(&stSsidQueryAck), sizeof(DIAG_CMD_EQUIP_SSID_QUERY_ACK_STRU), HI_TRUE);
}

HI_U32 mrsNdmMsgBqLoad(HI_U16 usId, HI_PVOID pMsgParam)
{
    DIAG_CMD_EQUIP_BQ_LOAD_ACK_STRU stBqLoadAck;

    HI_UNREF_PARAM(usId);
    HI_UNREF_PARAM(pMsgParam);

    (hi_void)memset_s(&stBqLoadAck, sizeof(DIAG_CMD_EQUIP_BQ_LOAD_ACK_STRU), 0, sizeof(DIAG_CMD_EQUIP_BQ_LOAD_ACK_STRU));

    equip_Trace("BqLoad>>>>>>");

    if(MRS_NDM_BQ_UPG_STATUS_BUSY == MRS_NDM_GetBqUpgState())
    {
        equip_Trace("Err:BqUpg is already started");
        stBqLoadAck.ret = HI_ERR_UPG_NOT_ALLOW;
        return HI_MDM_ReportPacket(ID_DIAG_CMD_MDM_BQ_LOAD_FROM_SD, HI_DIAG_CMD_INSTANCE_LOCAL,
            (HI_PBYTE)(&stBqLoadAck), sizeof(DIAG_CMD_EQUIP_BQ_LOAD_ACK_STRU), HI_TRUE);
    }

    return APP_NdmMessageSend(MRS_NDM_MSG_Q_1,EN_NDM_MSG_BQ_UPG_START, EN_NDM_MSG_BQ_UPG_START, 0, 0, 0);
}


HI_U32 mrsNdmMsgBqLoadResultQuery(HI_U16 usId, HI_PVOID pMsgParam)
{
    DIAG_CMD_EQUIP_BQ_RESULT_QUERY_ACK_STRU stBqLoadResultQueryAck;

    HI_UNREF_PARAM(usId);
    HI_UNREF_PARAM(pMsgParam);

    equip_Trace("BqUpg Result Query>>>>>>");

    (hi_void)memset_s(&stBqLoadResultQueryAck, sizeof(DIAG_CMD_EQUIP_BQ_RESULT_QUERY_ACK_STRU), 0, sizeof(DIAG_CMD_EQUIP_BQ_RESULT_QUERY_ACK_STRU));

    stBqLoadResultQueryAck.ret = MRS_NDM_GetBqUpgResult();
    return HI_MDM_ReportPacket(ID_DIAG_CMD_MDM_BQ_LOAD_RESULT_QUERY, HI_DIAG_CMD_INSTANCE_LOCAL,
        (HI_PBYTE)(&stBqLoadResultQueryAck), sizeof(DIAG_CMD_EQUIP_BQ_RESULT_QUERY_ACK_STRU), HI_TRUE);
}


HI_U32 mrsNdmMsgKeyTest(HI_U16 usId, HI_PVOID pMsgParam)
{
    HI_U32 ulRet = HI_ERR_SUCCESS;

    HI_U32* pRecvMsgParam = (HI_U32 *)pMsgParam;
    DIAG_CMD_EQUIP_KEY_TEST_STRU *pstKeyTest = (DIAG_CMD_EQUIP_KEY_TEST_STRU*)pRecvMsgParam[0];
    DIAG_CMD_EQUIP_KEY_TEST_ACK_STRU stKeyTestAck;
    MRS_NDM_SRV_CTX_STRU *pstCtx = mrsNdmGetContext();
    HI_U32 ulKeyMask = 0;
    HI_DRV_KEY_NUM_E eKey = HI_DRV_KEY_NUM_0;

    HI_UNREF_PARAM(usId);

    equip_Trace("KeyTest>>>>>>");

    // pRecvMsgParam[0] is content, pRecvMsgParam[2] is content size
    if (sizeof(DIAG_CMD_EQUIP_KEY_TEST_STRU) != pRecvMsgParam[2])
    {
        equip_Trace("Receive len:%2d is not expected", pRecvMsgParam[2]);
        return HI_ERR_BAD_DATA;
    }

    (hi_void)memset_s(&stKeyTestAck, sizeof(DIAG_CMD_EQUIP_KEY_TEST_ACK_STRU), 0, sizeof(DIAG_CMD_EQUIP_KEY_TEST_ACK_STRU));

    ulKeyMask = pstKeyTest->keyMask;
    MRS_NDM_SetKeyMask(ulKeyMask);

    for (eKey = HI_DRV_KEY_NUM_0; eKey < HI_DRV_KEY_NUM_MAX; eKey++)
    {
        if (ulKeyMask & (1 << (unsigned int)eKey))
        {
            ulRet |= HI_DRV_KEY_Register(eKey, mrsNdmKeyTestCallback, eKey);
        }
        else
        {
            HI_DRV_KEY_UnRegister(eKey);
        }
    }

    if (HI_ERR_SUCCESS != ulRet)
    {
        equip_Trace("HI_DRV_KEY_Register fail, ret = 0x%2X", ulRet);
        stKeyTestAck.ret = ulRet;
        return HI_MDM_ReportPacket(ID_DIAG_CMD_MDM_KEY_TEST, HI_DIAG_CMD_INSTANCE_LOCAL,
            (HI_PBYTE)(&stKeyTestAck), sizeof(DIAG_CMD_EQUIP_KEY_TEST_ACK_STRU), HI_TRUE);
    }

    pstCtx->stEqtCtx.stKeyTest.ulKeyResponse = 0;
    pstCtx->stEqtCtx.stKeyTest.ulTestCount   = 0;
    pstCtx->stEqtCtx.stKeyTest.ulTotalCount  = pstKeyTest->totalTimes;

    return APP_NdmTimerStart(MRS_NDM_TIMER_KEY_TEST, pstKeyTest->periodTime, HI_SYS_TIMER_PERIODIC);
}

HI_U32 mrsNdmMsgDisplayTest(HI_U16 usId, HI_PVOID pMsgParam)
{
    HI_U32* pRecvMsgParam = (HI_U32*)pMsgParam;
    DIAG_CMD_EQUIP_DISPLAY_STRU *pstDisplay = (DIAG_CMD_EQUIP_DISPLAY_STRU*)pRecvMsgParam[0];
    DIAG_CMD_EQUIP_DISPLAY_ACK_STRU stDisplayAck;

    HI_UNREF_PARAM(usId);

    equip_Trace("Display>>>>>>");

    // pRecvMsgParam[0] is content, pRecvMsgParam[2] is content size
    if (sizeof(DIAG_CMD_EQUIP_DISPLAY_STRU) != pRecvMsgParam[2])
    {
        equip_Trace("Receive len:%2d is not expected", pRecvMsgParam[2]);
        return HI_ERR_BAD_DATA;
    }

    (hi_void)memset_s(&stDisplayAck, sizeof(DIAG_CMD_EQUIP_DISPLAY_ACK_STRU), 0, sizeof(DIAG_CMD_EQUIP_DISPLAY_ACK_STRU));

    pstDisplay = (DIAG_CMD_EQUIP_DISPLAY_STRU*)pRecvMsgParam[0];

    equip_Trace("state = 0x%2X, period = 0x%2X", pstDisplay->state, pstDisplay->period);

    APP_NdmTimerStop(MRS_NDM_TIMER_DISPLAY_FLASH);
    APP_NdmTimerStop(MRS_NDM_TIMER_DISPLAY_ON);
    APP_NdmTimerStop(MRS_NDM_TIMER_DISPLAY_OFF);

    if (MRS_LATTICE_STATUS_FLASH == pstDisplay->state)
    {
        APP_NdmTimerStart(MRS_NDM_TIMER_DISPLAY_FLASH, pstDisplay->period, HI_SYS_TIMER_PERIODIC);
    }
    else if (MRS_LATTICE_STATUS_ON == pstDisplay->state)
    {
        APP_NdmTimerStart(MRS_NDM_TIMER_DISPLAY_ON, pstDisplay->period, HI_SYS_TIMER_ONESHOT);
    }
    else if (MRS_LATTICE_STATUS_OFF == pstDisplay->state)
    {
        APP_NdmTimerStart(MRS_NDM_TIMER_DISPLAY_OFF, pstDisplay->period, HI_SYS_TIMER_ONESHOT);
    }
    else
    {
        equip_Trace("Not surport");
    }

    stDisplayAck.ret = HI_ERR_SUCCESS;
    return HI_MDM_ReportPacket(ID_DIAG_CMD_MDM_DISPLAY, HI_DIAG_CMD_INSTANCE_LOCAL,
        (HI_PBYTE)(&stDisplayAck), sizeof(DIAG_CMD_EQUIP_DISPLAY_ACK_STRU), HI_TRUE);
}

HI_U32 mrsNdmMsgTestVerQuery(HI_U16 usId, HI_PVOID pMsgParam)
{
    DIAG_CMD_EQUIP_VERSION_CTRL_ACK_STRU stTestVerQueryAck;

    HI_UNREF_PARAM(usId);
    HI_UNREF_PARAM(pMsgParam);

    equip_Trace("TestVerQuery>>>>>>");

    (hi_void)memset_s(&stTestVerQueryAck, sizeof(DIAG_CMD_EQUIP_VERSION_CTRL_ACK_STRU), 0, sizeof(DIAG_CMD_EQUIP_VERSION_CTRL_ACK_STRU));

    stTestVerQueryAck.ulBoardVersion = MRS_NDM_EQUIP_TEST_VERSION;

    equip_Trace("TestVersion = 0x%2X", stTestVerQueryAck.ulBoardVersion);

    return HI_MDM_ReportPacket(ID_DIAG_CMD_MDM_EQT_VERSION, HI_DIAG_CMD_INSTANCE_LOCAL,
        (HI_PBYTE)(&stTestVerQueryAck), sizeof(DIAG_CMD_EQUIP_VERSION_CTRL_ACK_STRU), HI_TRUE);
}

HI_U32 mrsNdmMsgTestInit(HI_U16 usId, HI_PVOID pMsgParam)
{
    HI_U32* pRecvMsgParam = HI_NULL;
    DIAG_CMD_EQUIP_TEST_INIT_ACK_STRU stTestInitAck;
    DIAG_CMD_EQUIP_TEST_INIT_STRU* pstTestInit = HI_NULL;
    MRS_NDM_SRV_CTX_STRU *pstCtx = mrsNdmGetContext();

    HI_UNREF_PARAM(usId);

    equip_Trace("TestInit>>>>>>");

    (hi_void)memset_s(&stTestInitAck, sizeof(DIAG_CMD_EQUIP_TEST_INIT_ACK_STRU), 0, sizeof(DIAG_CMD_EQUIP_TEST_INIT_ACK_STRU));
    pRecvMsgParam = (HI_U32*)pMsgParam;

    // pRecvMsgParam[0] is content, pRecvMsgParam[2] is content size
    if (sizeof(DIAG_CMD_EQUIP_TEST_INIT_STRU) != pRecvMsgParam[2])
    {
        equip_Trace("Receive len:%2d is not expected", pRecvMsgParam[2]);
        return HI_ERR_BAD_DATA;
    }

    pstTestInit = (DIAG_CMD_EQUIP_TEST_INIT_STRU*)pRecvMsgParam[0];

    MRS_NDM_SetDutType(pstTestInit->ucType);
    mrsNdmIOParamInit();

    (hi_void)memset_s(&(pstCtx->stEqtCtx.stPlcTest), sizeof(MRS_NDM_EQT_PLC_TEST_STRU), 0, sizeof(MRS_NDM_EQT_PLC_TEST_STRU));
    (hi_void)memset_s(&(pstCtx->stEqtCtx.stKeyTest), sizeof(MRS_NDM_EQT_KEY_TEST_STRU), 0, sizeof(MRS_NDM_EQT_KEY_TEST_STRU));
    pstCtx->stEqtCtx.stDisplay.ulCount = 0;

    APP_NdmTimerStop(MRS_NDM_TIMER_ZERO_CROSS);
    APP_NdmTimerStop(MRS_NDM_TIMER_PLC_TEST);
    APP_NdmTimerStop(MRS_NDM_TIMER_SSID_SET);
    APP_NdmTimerStop(MRS_NDM_TIMER_KEY_TEST);
    APP_NdmTimerStop(MRS_NDM_TIMER_BEEP_CTRL);
    APP_NdmTimerStop(MRS_NDM_TIMER_DISPLAY_FLASH);
    APP_NdmTimerStop(MRS_NDM_TIMER_DISPLAY_ON);
    APP_NdmTimerStop(MRS_NDM_TIMER_DISPLAY_OFF);

    stTestInitAck.ret = HI_ERR_SUCCESS;
    return HI_MDM_ReportPacket(ID_DIAG_CMD_MDM_EQT_TEST_INIT, HI_DIAG_CMD_INSTANCE_LOCAL,
        (HI_PBYTE)(&stTestInitAck), sizeof(DIAG_CMD_EQUIP_TEST_INIT_ACK_STRU), HI_TRUE);
}

HI_U32 mrsNdmMsgNtbSyncSet(HI_U16 usId, HI_PVOID pMsgParam)
{
    HI_U32* pRecvMsgParam = HI_NULL;
    DIAG_CMD_EQUIP_NTB_SYNC_SET_ACK_STRU stNtbSyncSetAck;
    DIAG_CMD_EQUIP_NTB_SYNC_SET_STRU* pstNtbSyncSet = HI_NULL;

    HI_UNREF_PARAM(usId);

    equip_Trace("NtbSyncSet>>>>>>");

    (hi_void)memset_s(&stNtbSyncSetAck,sizeof(DIAG_CMD_EQUIP_NTB_SYNC_SET_ACK_STRU), 0, sizeof(DIAG_CMD_EQUIP_NTB_SYNC_SET_ACK_STRU));
    pRecvMsgParam = (HI_U32*)pMsgParam;

    // pRecvMsgParam[0] is content, pRecvMsgParam[2] is content size
    if (sizeof(DIAG_CMD_EQUIP_NTB_SYNC_SET_STRU) != pRecvMsgParam[2])
    {
        equip_Trace("Receive len:%2d is not expected", pRecvMsgParam[2]);
        return HI_ERR_BAD_DATA;
    }

    pstNtbSyncSet = (DIAG_CMD_EQUIP_NTB_SYNC_SET_STRU*)pRecvMsgParam[0];

    // NTB sync
    stNtbSyncSetAck.ret = HI_MDM_SetNdmNtbSynchStation(pstNtbSyncSet->ucWorkMode, pstNtbSyncSet->ucSnid, pstNtbSyncSet->ucMac);
    return HI_MDM_ReportPacket(ID_DIAG_CMD_MDM_NTB_SYNC_SET, HI_DIAG_CMD_INSTANCE_LOCAL,
        (HI_PBYTE)(&stNtbSyncSetAck), sizeof(DIAG_CMD_EQUIP_NTB_SYNC_SET_ACK_STRU), HI_TRUE);
}

HI_U32 mrsNdmMsgNtbSyncQuery(HI_U16 usId, HI_PVOID pMsgParam)
{
    DIAG_CMD_EQUIP_NTB_SYNC_QUERY_ACK_STRU stNtbSyncQueryAck;

    HI_UNREF_PARAM(usId);
    HI_UNREF_PARAM(pMsgParam);

    equip_Trace("NtbSyncQuery>>>>>>");

    (hi_void)memset_s(&stNtbSyncQueryAck, sizeof(DIAG_CMD_EQUIP_NTB_SYNC_QUERY_ACK_STRU), 0, sizeof(DIAG_CMD_EQUIP_NTB_SYNC_QUERY_ACK_STRU));

    // query NTB sync result
    stNtbSyncQueryAck.ret = HI_ERR_FAILURE;
    if (HI_MDM_CheckNtbIsSynchSuccess())
    {
        stNtbSyncQueryAck.ret = HI_ERR_SUCCESS;
    }
    return HI_MDM_ReportPacket(ID_DIAG_CMD_MDM_NTB_SYNC_QUERY, HI_DIAG_CMD_INSTANCE_LOCAL,
        (HI_PBYTE)(&stNtbSyncQueryAck), sizeof(DIAG_CMD_EQUIP_NTB_SYNC_QUERY_ACK_STRU), HI_TRUE);
}

HI_U32 mrsNdmMsgTfCardTest(HI_U16 usId, HI_PVOID pMsgParam)
{
    DIAG_CMD_EQUIP_TF_CARD_TEST_ACK_STRU stTfCardTestAck;

    HI_UNREF_PARAM(usId);
    HI_UNREF_PARAM(pMsgParam);

    equip_Trace("TfCardTest>>>>>>");

    (hi_void)memset_s(&stTfCardTestAck, sizeof(DIAG_CMD_EQUIP_TF_CARD_TEST_ACK_STRU), 0, sizeof(DIAG_CMD_EQUIP_TF_CARD_TEST_ACK_STRU));

    stTfCardTestAck.ret = mrsNdmMsgStartTfCardTest();

    return HI_MDM_ReportPacket(ID_DIAG_CMD_MDM_TF_CARD_TEST, HI_DIAG_CMD_INSTANCE_LOCAL,
        (HI_PBYTE)(&stTfCardTestAck), sizeof(DIAG_CMD_EQUIP_TF_CARD_TEST_ACK_STRU), HI_TRUE);
}

HI_VOID mrsNdmKeyTestCallback(HI_BOOL bKeyPress, HI_U32 ulMs, HI_U32 ulUserData)
{
    HI_DRV_KEY_NUM_E eKeyNum = HI_DRV_KEY_NUM_MAX;
    MRS_NDM_SRV_CTX_STRU *pstCtx = mrsNdmGetContext();

    HI_UNREF_PARAM(ulMs);

    // Only process 'Key Up' message.
    if (bKeyPress)
    {
        return;
    }

    if (ulUserData >= HI_DRV_KEY_NUM_MAX)
    {
        return;
    }

    MRS_NDM_BeepOn();
    APP_NdmTimerStart(MRS_NDM_TIMER_BEEP_CTRL, MRS_NDM_TIME_BEEP_CTRL, HI_SYS_TIMER_ONESHOT);

    eKeyNum = (HI_DRV_KEY_NUM_E)ulUserData;
    pstCtx->stEqtCtx.stKeyTest.ulKeyResponse |=  (1 << (unsigned int)eKeyNum);

    equip_Trace("Key%d, response = %2X", eKeyNum, pstCtx->stEqtCtx.stKeyTest.ulKeyResponse);
}

HI_U32 mrsNdmMsgStartBqUpg(HI_U16 usId, HI_PVOID pMsgParam)
{
    HI_U32 ret = HI_ERR_FAILURE;
    DIAG_CMD_EQUIP_BQ_LOAD_ACK_STRU stBqLoadAck;

    HI_UNREF_PARAM(usId);
    HI_UNREF_PARAM(pMsgParam);

    (hi_void)memset_s(&stBqLoadAck, sizeof(DIAG_CMD_EQUIP_BQ_LOAD_ACK_STRU), 0, sizeof(DIAG_CMD_EQUIP_BQ_LOAD_ACK_STRU));

    equip_Trace("Start Bq Upg>>>>>>");

    stBqLoadAck.ret = HI_ERR_SUCCESS;
    HI_MDM_ReportPacket(ID_DIAG_CMD_MDM_BQ_LOAD_FROM_SD, HI_DIAG_CMD_INSTANCE_LOCAL,
        (HI_PBYTE)(&stBqLoadAck), sizeof(DIAG_CMD_EQUIP_BQ_LOAD_ACK_STRU), HI_TRUE);

    MRS_NDM_SetBqUpgResult(HI_ERR_BUSY);
    MRS_NDM_SetBqUpgState(MRS_NDM_BQ_UPG_STATUS_BUSY);
    ret = mrsNdmBqUpg();
    MRS_NDM_SetBqUpgResult(ret);
    MRS_NDM_SetBqUpgState(MRS_NDM_BQ_UPG_STATUS_IDLE);

    equip_Trace("BqUpg ret=0x%2X;", ret);

    stBqLoadAck.ret = MRS_NDM_GetBqUpgResult();
    ret = HI_MDM_ReportPacket(ID_DIAG_CMD_MDM_BQ_LOAD_RESULT_QUERY, HI_DIAG_CMD_INSTANCE_LOCAL,
        (HI_PBYTE)(&stBqLoadAck), sizeof(DIAG_CMD_EQUIP_BQ_LOAD_ACK_STRU), HI_TRUE);
    return ret;
}

HI_U32 APP_BqUpgMsgProc(HI_SYS_QUEUE_MSG_S *pstMsg)
{
    return mrsNdmMsgStartBqUpg((HI_U16)pstMsg->ulParam[0], (HI_PVOID)(&pstMsg->ulParam[1]));
}


#ifdef PRODUCT_CFG_SUPPORT_CCT3911
#include "mrs_misc.h"

HI_U32 mrsNdmBqUpg(HI_VOID)
{
    return Mrs_Misc_BqUpg();
}
#else
HI_U32 mrsNdmBqUpg(HI_VOID)
{
    HI_U32 ret = HI_ERR_FAILURE;
    MRS_NDM_BQ_UPG_FILE_HEAD *pUpgFile = HI_NULL;
    MRS_NDM_BQ_UPG_FILE_INFO bq_upg_file_info_current = {0};

    HI_S32 ret_fd = 0;
    HI_S32 ret_close = 0;
    HI_SSiZE_T ret_real_len = 0;
    HI_CHAR bq_upg_filename[] = "/bq_upg_file.bqfs";

    pUpgFile = HI_MDM_Malloc(HI_MOD_ID_NDM_SRV, MRS_BQ_UPG_FILE_SIZE); // CRC + main ver + sub ver + upg file ¡Ö 188K
    if(HI_NULL == pUpgFile)
    {
        return HI_ERR_MALLOC_FAILUE;
    }
    (hi_void)memset_s(pUpgFile, MRS_BQ_UPG_FILE_SIZE, 0, MRS_BQ_UPG_FILE_SIZE);

    ret = HI_MDM_OpenSdCardFile(bq_upg_filename, HI_FMGR_O_RDONLY, 777, &ret_fd);
    if(HI_ERR_SUCCESS != ret)
    {
        equip_Trace("OpenFile fail! ret=%d", ret);
        HI_MDM_Free(HI_MOD_ID_NDM_SRV, pUpgFile);
        pUpgFile = HI_NULL;
        return ret;
    }

    if(ret_fd < 0)
    {
        equip_Trace("OpenFile fail! ret_fd=%d", ret_fd);
        HI_MDM_Free(HI_MOD_ID_NDM_SRV, pUpgFile);
        pUpgFile = HI_NULL;
        return HI_ERR_FAILURE;
    }
    equip_Trace("OpenFile OK");

    ret = HI_MDM_ReadSdCardFile(ret_fd, pUpgFile, MRS_BQ_UPG_FILE_SIZE, &ret_real_len);
    if(ret != HI_ERR_SUCCESS)
    {
        equip_Trace("ReadFile fail!");
        HI_MDM_Free(HI_MOD_ID_NDM_SRV, pUpgFile);
        pUpgFile = HI_NULL;
        return ret;
    }
    equip_Trace("ReadFile OK");

    ret = HI_MDM_CloseSdCardFile(ret_fd,&ret_close);
    if(ret != HI_ERR_SUCCESS || ret_close < 0)
    {
        equip_Trace("CloseFile fail!");
        HI_MDM_Free(HI_MOD_ID_NDM_SRV, pUpgFile);
        pUpgFile = HI_NULL;
        return ret;
    }
    equip_Trace("CloseFile OK");

    if((HI_U32)ret_real_len != pUpgFile->ulDataSize + sizeof(MRS_NDM_BQ_UPG_FILE_HEAD))
    {
        equip_Trace("Read file len check Err: Read file len=%d; File info len=%d", ret_real_len, pUpgFile->ulDataSize);
        return HI_ERR_FAILURE;
    }
    equip_Trace("FileSize Check OK");

    bq_upg_file_info_current.ulCrc = HI_MDM_Crc32(0, pUpgFile->aucData, pUpgFile->ulDataSize);// crc = 0x758a0a1b;
    if(bq_upg_file_info_current.ulCrc != pUpgFile->ulCrc)
    {
        equip_Trace("Crc32 fail!");
        HI_MDM_Free(HI_MOD_ID_NDM_SRV, pUpgFile);
        pUpgFile = HI_NULL;
        return HI_ERR_FAILURE;
    }

    equip_Trace("[sd_bq_info][size=%u][crc=%u][main=%u][sub=%u]",
        pUpgFile->ulDataSize, pUpgFile->ulCrc, pUpgFile->usMainVer, pUpgFile->ucSubVer);

    ret = HI_BAT_UpgradeChip((HI_PVOID)pUpgFile->aucData, (HI_U32)pUpgFile->ulDataSize);
    if(HI_ERR_SUCCESS != ret)
    {
        equip_Trace("UpgradeCellChip fail!");
        HI_MDM_Free(HI_MOD_ID_NDM_SRV, pUpgFile);
        pUpgFile = HI_NULL;
        return ret;
    }

    // compare version, confirm is upg success
    ret = HI_BAT_ReadChipVer(&(bq_upg_file_info_current.usMainVer), &(bq_upg_file_info_current.ucSubVer));
    if(HI_ERR_SUCCESS != ret)
    {
        HI_MDM_Free(HI_MOD_ID_NDM_SRV, pUpgFile);
        pUpgFile = HI_NULL;
        return ret;
    }

    if((bq_upg_file_info_current.usMainVer == pUpgFile->usMainVer)
        &&(bq_upg_file_info_current.ucSubVer == pUpgFile->ucSubVer))
    {
        HI_MDM_Free(HI_MOD_ID_NDM_SRV, pUpgFile);
        pUpgFile = HI_NULL;
        return HI_ERR_SUCCESS;
    }
    else
    {
        equip_Trace("Ver Check ERR! Read main_ver=%u, Read sub_ver=%u",
            bq_upg_file_info_current.usMainVer, bq_upg_file_info_current.ucSubVer);
        HI_MDM_Free(HI_MOD_ID_NDM_SRV, pUpgFile);
        pUpgFile = HI_NULL;
        return HI_ERR_FAILURE;
    }
}
#endif

HI_VOID mrsNdmMsgZeroCrossNotify(HI_U32 ulRet, HI_U8 ucLevel, HI_U32 szNtb[EQUIP_ZERO_NTB_NUM])
{
    DIAG_CMD_EQUIP_ZERO_CROSS_ACK_STRU stZeroCrossAck;

    equip_Trace("ZeroCrossNotify<<<");

    (hi_void)memset_s(&stZeroCrossAck, sizeof(DIAG_CMD_EQUIP_ZERO_CROSS_ACK_STRU), 0, sizeof(DIAG_CMD_EQUIP_ZERO_CROSS_ACK_STRU));
    APP_NdmTimerStop(MRS_NDM_TIMER_ZERO_CROSS);
    equip_Trace("ZeroCross ret   = 0x%2X, level = %d", ulRet, ucLevel);

    stZeroCrossAck.ret = ulRet;
    stZeroCrossAck.ucLevel = ucLevel;
    (hi_void)memcpy_s(stZeroCrossAck.szNtb,sizeof(HI_U32)*EQUIP_ZERO_NTB_NUM, szNtb, sizeof(HI_U32)*EQUIP_ZERO_NTB_NUM);

    HI_MDM_ReportPacket(ID_DIAG_CMD_MDM_ZERO_CROSS_TEST, HI_DIAG_CMD_INSTANCE_LOCAL,
        (HI_PBYTE)(&stZeroCrossAck), sizeof(DIAG_CMD_EQUIP_ZERO_CROSS_ACK_STRU), HI_TRUE);
    return;
}

HI_U32 mrsNdmMsgPlcRxNotify(HI_PVOID pCmdParam, HI_U16 usCmdParamSize, HI_U32 ulOption)
{
    DIAG_CMD_EQUIP_PLC_ACK_STRU stPlcAck;
    HI_U32 plcRxRandom = 0;

    HI_UNREF_PARAM(usCmdParamSize);
    HI_UNREF_PARAM(ulOption);

    equip_Trace("PlcRxNotify<<<");
    if (HI_NULL == pCmdParam)
    {
        equip_Trace("Data error: pCmdParam is NULL!");
        return HI_ERR_INVALID_PARAMETER;
    }

    plcRxRandom = *(HI_U32 *)pCmdParam;

    (hi_void)memset_s(&stPlcAck, sizeof(DIAG_CMD_EQUIP_PLC_ACK_STRU), 0, sizeof(DIAG_CMD_EQUIP_PLC_ACK_STRU));

    if (g_pstMrsNdmSrvCtx->stEqtCtx.stPlcTest.ulPlcTxRandom == plcRxRandom)
    {
        stPlcAck.ret = HI_ERR_SUCCESS;
        g_pstMrsNdmSrvCtx->stEqtCtx.stPlcTest.ulSuccessCount++;
    }
    else
    {
        equip_Trace("PLC Test Fail, not expect!");
        g_pstMrsNdmSrvCtx->stEqtCtx.stPlcTest.ulFailCount++;
        stPlcAck.ret = HI_ERR_FAILURE;
    }
    APP_NdmTimerStop(MRS_NDM_TIMER_PLC_TEST);

    return HI_MDM_ReportPacket(ID_DIAG_CMD_MDM_PLC_TEST, HI_DIAG_CMD_INSTANCE_LOCAL,
        (HI_PBYTE)(&stPlcAck), sizeof(DIAG_CMD_EQUIP_PLC_ACK_STRU), HI_TRUE);
}

HI_U32 mrsNdmMsgStartTfCardTest(HI_VOID)
{
    HI_U32 testResult = HI_ERR_FAILURE;
    HI_U32 ret = HI_ERR_FAILURE;
    HI_CHAR szFileBuf[2] = {'a','b'};

    HI_S32 ret_fd = 0;
    HI_S32 ret_close = 0;
    HI_SSiZE_T ret_real_len = 0;
    HI_CHAR filename[] = "/tf_card_test_file.txt";


    ret = HI_MDM_OpenSdCardFile(filename, HI_FMGR_O_RDWR + HI_FMGR_O_CREAT, 777, &ret_fd);
    if(HI_ERR_SUCCESS != ret)
    {
        equip_Trace("OpenFile fail! ret=%d", ret);
        return ret;
    }

    if(ret_fd < 0)
    {
        equip_Trace("OpenFile fail! ret_fd=%d", ret_fd);
        return HI_ERR_FAILURE;
    }
    equip_Trace("OpenFile OK");

    do
    {
        testResult = HI_MDM_ReadSdCardFile(ret_fd, szFileBuf, sizeof(szFileBuf), &ret_real_len);
        if(testResult == HI_ERR_SUCCESS)
        {
            equip_Trace("ReadFile success!");
            break;
        }

        testResult = HI_MDM_WriteSdCardFile(ret_fd, szFileBuf, sizeof(szFileBuf), &ret_real_len);
        if(testResult == HI_ERR_SUCCESS)
        {
            equip_Trace("WriteFile success!");
            break;
        }

    }while(0);

    ret = HI_MDM_CloseSdCardFile(ret_fd,&ret_close);
    if(ret != HI_ERR_SUCCESS || ret_close < 0)
    {
        equip_Trace("CloseFile fail");
        return ret;
    }

    return testResult;

}


HI_VOID mrsNdmKeyPressCallback(HI_BOOL bKeyPress, HI_U32 ulMs, HI_U32 eKey)
{
    if (eKey >= HI_DRV_KEY_NUM_MAX)
    {
        return;
    }

    // Send key press message to main task.
    APP_NdmMessageSend(MRS_NDM_MSG_Q_0, EN_MRS_FW_MSG_NDM_KEY,eKey, (HI_U32)bKeyPress, ulMs, 0);
}


/**
 @brief     NDM key message process.
 @attention \n
N/A
 @param[in] pstMsg  Message pointer.
 @retval ::HI_ERR_SUCCESS success
 @retval ::others failure
 @see \n
N/A
 */
HI_U32 mrsNdmKeyMsgProc(HI_IN HI_SYS_QUEUE_MSG_S* pstMsg)
{
    HI_DRV_KEY_NUM_E eKey = (HI_DRV_KEY_NUM_E)pstMsg->ulParam[0];

    if (MRS_NDM_TYPE_RECEIVER == MRS_NDM_GetDutType())  // receiver
    {
        switch (eKey)
        {
        case HI_DRV_KEY_NUM_0:  // transformer rx enable switch
            mrsNdmTfRxEnableSwitchKeyMsgProc((HI_BOOL)pstMsg->ulParam[1], pstMsg->ulParam[2]);
            break;

        case HI_DRV_KEY_NUM_1:  // MRS_KEY_DATA_COLLECT
            mrsNdmDcKeyMsgProc((HI_BOOL)pstMsg->ulParam[1], pstMsg->ulParam[2]);
            break;

        case HI_DRV_KEY_NUM_4:  // receiver strength display
            mrsNdmStrengthKeyMsgProc((HI_BOOL)pstMsg->ulParam[1], pstMsg->ulParam[2]);
            break;

        case HI_DRV_KEY_NUM_5:  // battery
            mrsNdmBatteryKeyMsgProc((HI_BOOL)pstMsg->ulParam[1], pstMsg->ulParam[2]);
            break;

        case HI_DRV_KEY_NUM_8:  // MRS_KEY_SIGNAL_SEND
            mrsNdmSendSignalKeyProc((HI_BOOL)pstMsg->ulParam[1], pstMsg->ulParam[2]);
            break;

        case HI_DRV_KEY_NUM_9:  // MRS_KEY_DATE_DISP ¹¦ÄÜ1
            mrsNdmDispTimeKeyMsgProc((HI_BOOL)pstMsg->ulParam[1], pstMsg->ulParam[2]);
            break;

        default:
            break;
        }
    }
    else    // receiver
    {
        switch (eKey)
        {
        case HI_DRV_KEY_NUM_0:  // transformer tx enable switch
            mrsNdmTfTxEnableSwitchKeyMsgProc((HI_BOOL)pstMsg->ulParam[1], pstMsg->ulParam[2]);
            break;

        case HI_DRV_KEY_NUM_1:  // transformer num
            mrsNdmTfTransformerNumKeyMsgProc((HI_BOOL)pstMsg->ulParam[1], pstMsg->ulParam[2]);
            break;

        default:
            break;
        }
    }

    return HI_ERR_SUCCESS;
}


HI_U32 mrsNdmSetDs1672Time(HI_U32 ulTime)
{
    HI_U32 ulReadTime;
    HI_U32 ulRet = HI_ERR_SUCCESS;

    ulRet = HI_RTC_WriteTime(ulTime);
    if (ulRet != HI_ERR_SUCCESS)
    {
        return HI_ERR_FAILURE;
    }

    ulRet = HI_RTC_ReadTime(&ulReadTime);
    if (ulRet != HI_ERR_SUCCESS)
    {
        return HI_ERR_FAILURE;
    }

    if ((ulReadTime >= (ulTime + MRS_NDM_RTC_TIME_TOLERATE))
        || (ulReadTime < ulTime))
    {
        return HI_ERR_FAILURE;
    }

    return HI_ERR_SUCCESS;

}


HI_U32 mrsNdmMsgRtcTime(HI_U16 usId, HI_PVOID pMsgParam)
{
    HI_U32 ulRtcTime = 0;
    HI_U32 ulRet = HI_ERR_SUCCESS;
    HI_U32 *pParam = (HI_U32 *)pMsgParam;
    DRV_DFX_NDM_RTC *pstRtcTime = (DRV_DFX_NDM_RTC *)(pParam[0]);

    HI_UNREF_PARAM(usId);

    do
    {
        pstRtcTime->ulRet = HI_ERR_SUCCESS;

        if (HI_TRUE == HI_MDM_IsNdmMeterAccess())
        {
            pstRtcTime->ulRet = HI_ERR_RTC_ERR_INIT;
            break;
        }

        ulRet = HI_RTC_ReadTime(&ulRtcTime);
        if (HI_ERR_SUCCESS != ulRet)
        {
            pstRtcTime->ulRet = HI_ERR_RTC_ERR_READ;
            ulRet = HI_ERR_FAILURE;
            break;
        }

        if (((pstRtcTime->ulRtcSecNum - MRS_NDM_RTC_TIME_TOLERATE) < ulRtcTime)
            && ((pstRtcTime->ulRtcSecNum + MRS_NDM_RTC_TIME_TOLERATE) > ulRtcTime))
        {
            break;
        }

        ulRet = mrsNdmSetDs1672Time(pstRtcTime->ulRtcSecNum);
        if (HI_ERR_SUCCESS != ulRet)
        {
            pstRtcTime->ulRet = HI_ERR_RTC_ERR_WRITE;
            ulRet = HI_ERR_FAILURE;
            break;
        }
    } while (0);

    HI_MDM_ReportPacket(ID_DIAG_CMD_NDM_RTC_TIME, HI_DIAG_CMD_INSTANCE_LOCAL, (HI_PBYTE)pstRtcTime, (HI_U16)sizeof(DRV_DFX_NDM_RTC), HI_TRUE);
    return ulRet;
}


/**
 @brief     NDM display battery information.
 @attention \n
N/A
 @param[in] bKeyPress  Key pressed up/down status, HI_TRUE-down, HI_FALSE-up.
 @param[in] ulMs       Key pressed duration, in milliseconds.
 @see \n
N/A
 */
HI_VOID mrsNdmBatteryKeyMsgProc(HI_BOOL bKeyPress, HI_U32 ulMs)
{
    HI_U32 ret = HI_ERR_SUCCESS;

    HI_UNREF_PARAM(ulMs);

    if (bKeyPress)
    {
        if (ulMs % MRS_NDM_BATTERY_FLUSH_TIME)
        {
            HI_U8 ucBatPercent = 0;
            HI_LCD_DISPLAY_INFO_S stLcdInf;
            HI_BAT_PERCENT_ERR_E enErrcode=HI_BAT_PERCENT_ERR_NONE;
            
            (hi_void)memset_s(&stLcdInf, sizeof(stLcdInf), 0, sizeof(stLcdInf));

            ret = HI_BAT_GetBatPercent(&ucBatPercent,&enErrcode);
            if (HI_ERR_SUCCESS != ret || enErrcode!=HI_BAT_PERCENT_ERR_NONE)
            {
                stLcdInf.data[0] = '-';
                stLcdInf.data[1] = '-';
                stLcdInf.data[2] = '-';
            }
            else
            {
                stLcdInf.data[0] = (HI_CHAR)((ucBatPercent / 100) + '0');
                ucBatPercent = (ucBatPercent % 100);
                stLcdInf.data[1] = (HI_CHAR)((ucBatPercent / 10 ) + '0');
                stLcdInf.data[2] = (HI_CHAR)((ucBatPercent % 10 ) + '0');
            }

            stLcdInf.pri = HI_LCD_DISPLAY_PRI_4;
            stLcdInf.tsec = HI_LCD_DISPLAY_FOREVER;

            (HI_VOID)HI_DRV_LCD_DisplayPrint(HI_LCD_FUNC_ID_BATTERY, &stLcdInf);
        }
    }
    else
    {
        (HI_VOID)HI_DRV_LCD_DisplayDisable(HI_LCD_FUNC_ID_BATTERY);
    }
}


/**
 @brief     NDM Tf Rx enable switch.
 @attention \n
N/A
 @param[in] bKeyPress  Key pressed up/down status, HI_TRUE-down, HI_FALSE-up.
 @param[in] ulMs       Key pressed duration, in milliseconds.
 @see \n
N/A
 */
HI_VOID mrsNdmTfRxEnableSwitchKeyMsgProc(HI_BOOL bKeyPress, HI_U32 ulMs)
{
    HI_LCD_DISPLAY_INFO_S stLcdInf;
    HI_BOOL bCurRxMode=HI_FALSE;
    HI_BOOL bSetRxMode=HI_FALSE;

    HI_UNREF_PARAM(ulMs);

    if (bKeyPress)
    {
        return;
    }

    if(HI_ERR_SUCCESS!=HI_MDM_TF_GetRxModeSwitch(&bCurRxMode))
    {
        return;
    }

    
    bSetRxMode=(bCurRxMode==HI_FALSE)?HI_TRUE:HI_FALSE;
    
    if(HI_ERR_SUCCESS!=HI_MDM_TF_SetRxModeSwitch(bSetRxMode))
    {
        return;
    }

    if (bSetRxMode)
    {
        (hi_void)memset_s(&stLcdInf, sizeof(stLcdInf), 0, sizeof(stLcdInf));
        stLcdInf.data[0] = 'R';
        stLcdInf.data[1] = '-';
        stLcdInf.data[2] = '-';
        stLcdInf.blink = HI_TRUE;
        stLcdInf.pri = HI_LCD_DISPLAY_PRI_2;
        stLcdInf.tsec = HI_LCD_DISPLAY_FOREVER;
        (HI_VOID)HI_DRV_LCD_DisplayPrint(HI_LCD_FUNC_ID_TF, &stLcdInf);
    }
    else
    {
        (HI_VOID)HI_DRV_LCD_DisplayDisable(HI_LCD_FUNC_ID_TF);
    }
}


/**
 @brief     NDM Tf Tx enable switch.
 @attention \n
N/A
 @param[in] bKeyPress  Key pressed up/down status, HI_TRUE-down, HI_FALSE-up.
 @param[in] ulMs       Key pressed duration, in milliseconds.
 @see \n
N/A
 */


HI_VOID mrsNdmTfTxEnableSwitchKeyMsgProc(HI_BOOL bKeyPress, HI_U32 ulMs)
{
    HI_BOOL bCurMode=HI_FALSE;

    HI_UNREF_PARAM(ulMs);

    if (bKeyPress)
    {
        return;
    }

    if(HI_ERR_SUCCESS==HI_MDM_TF_GetTxModeSwitch(&bCurMode))
    {
        HI_MDM_TF_SetTxModeSwitch((bCurMode==HI_FALSE)?HI_TRUE:HI_FALSE);
    }
}


/**
 @brief     NDM Tf transformer number increase.
 @attention \n
N/A
 @param[in] bKeyPress  Key pressed up/down status, HI_TRUE-down, HI_FALSE-up.
 @param[in] ulMs       Key pressed duration, in milliseconds.
 @see \n
N/A
 */
HI_VOID mrsNdmTfTransformerNumKeyMsgProc(HI_BOOL bKeyPress, HI_U32 ulMs)
{
    HI_LCD_DISPLAY_INFO_S stLcdInf;
    HI_U8 ucCurNum=0;
    HI_U8 ucMaxNum=0;

    HI_UNREF_PARAM(ulMs);

    if (bKeyPress)
    {
        return;
    }

    if(HI_ERR_SUCCESS!=HI_MDM_TF_GetTransFormerNum(&ucCurNum,&ucMaxNum))
    {
        return;
    }

    ucCurNum++;
    if(ucCurNum>ucMaxNum)
    {
        ucCurNum=0;
    }
    
    if(HI_ERR_SUCCESS!=HI_MDM_TF_SetTransFormerNum(ucCurNum))
    {
        return;
    }


    (hi_void)memset_s(&stLcdInf, sizeof(stLcdInf), 0, sizeof(stLcdInf));
    stLcdInf.data[1] = (HI_CHAR)(((ucCurNum+1) % 10) + '0');
    stLcdInf.pri = HI_LCD_DISPLAY_PRI_2;
    stLcdInf.tsec = HI_LCD_DISPLAY_FOREVER;
    (HI_VOID)HI_DRV_LCD_DisplayPrint(HI_LCD_FUNC_ID_TF, &stLcdInf);
}


/**
 @brief     NDM Tf receiver strength display.
 @attention \n
N/A
 @param[in] bKeyPress  Key pressed up/down status, HI_TRUE-down, HI_FALSE-up.
 @param[in] ulMs       Key pressed duration, in milliseconds.
 @see \n
N/A
 */
HI_VOID mrsNdmStrengthKeyMsgProc(HI_BOOL bKeyPress, HI_U32 ulMs)
{
    HI_UNREF_PARAM(ulMs);

    if (bKeyPress)
    {
        return;
    }

    HI_MDM_TF_SwitchStrengthDisplay();
}


HI_END_HEADER
#endif // defined(PRODUCT_CFG_PRODUCT_TYPE_NDM)
