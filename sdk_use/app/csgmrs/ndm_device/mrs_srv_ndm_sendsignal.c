/********************************************************************************
Copyright (C), Huawei Technologies Co., Ltd. 2012-2020. All rights reserved.
*********************************************************************************
File Name     : mrs_srv_ndm_sendsignal.c
Version       : Initial draft
Author        : HCT
Created Date  : 2016-10-13
Last Modified by: c00233580
Description   : TODO: ...
Function List : TODO: ...
Change History: TODO: ...
********************************************************************************/

#include "hi_types.h"
#if defined(PRODUCT_CFG_PRODUCT_TYPE_NDM)
#include <hi_mdm_types.h>
#include "hi_share_memory.h"  
#include "hi_algorithm.h"
#include "hi_mdm_phy.h"
#include <hi_lcd.h>
#include "mrs_srv_ndm.h"
#include "mrs_srv_ndm_sendsignal.h"

HI_START_HEADER

HI_PRV MRS_NDM_SEND_SIGNAL_CTX g_stNdmSendSignalCtx = {0, MRS_SEND_SIGNAL_MODE_NONE, 0, 0, 0};

HI_PRV MRS_NDM_SEND_SIGNAL_CTX *mrsGetSendSignalCtx(HI_VOID);
HI_PRV HI_VOID mrsNdmSendSignalDisplay(MRS_SEND_SIGNAL_DISPLAY_E eId);
HI_PRV HI_U32 mrsNdmSendSignalStart(HI_VOID);

HI_PRV HI_U32 mrsNdmSendSignalStartPhase2(HI_VOID);
HI_PRV HI_U32 mrsNdmSendSignalChangeToMode0(HI_VOID);
HI_PRV HI_U32 mrsNdmSendSignalChangeToMode1(HI_VOID);
HI_PRV HI_U32 mrsNdmSendSignalChangeToMode2(HI_VOID);
HI_PRV HI_U32 mrsNdmSendSignalMode1Timeout(HI_VOID);

#define MRS_SEND_SIGNAL_MSG(x)  APP_NdmMessageSend(MRS_NDM_MSG_Q_1,EN_NDM_MSG_SEND_SIGNAL, (x), 0, 0, 0)



HI_U32 mrsNdmSendSignalInit(HI_VOID)
{
    (hi_void)memset_s(&g_stNdmSendSignalCtx, sizeof(g_stNdmSendSignalCtx), 0, sizeof(g_stNdmSendSignalCtx));
    return HI_ERR_SUCCESS;
}


MRS_NDM_SEND_SIGNAL_CTX *mrsGetSendSignalCtx(HI_VOID)
{
    return &g_stNdmSendSignalCtx;
}


HI_VOID mrsNdmSendSignalDisplay(MRS_SEND_SIGNAL_DISPLAY_E eId)
{
    HI_LCD_DISPLAY_INFO_S stLcdInf;

    (hi_void)memset_s(&stLcdInf, sizeof(stLcdInf), 0, sizeof(stLcdInf));

    stLcdInf.blink = HI_FALSE;
    stLcdInf.tsec = HI_SYS_WAIT_FOREVER;
    stLcdInf.pri = HI_LCD_DISPLAY_PRI_4;

    switch (eId)
    {
    case MRS_SEND_SIGNAL_DISPLAY_NONE:
        HI_DRV_LCD_DisplayDisable(HI_LCD_FUNC_ID_SIGNAL_SEND);
        break;

    case MRS_SEND_SIGNAL_DISPLAY_MODE0:
        stLcdInf.data[0] = 'S';
        stLcdInf.data[1] = 'G';
        stLcdInf.data[2] = '0';
        break;

    case MRS_SEND_SIGNAL_DISPLAY_MODE1:
        stLcdInf.data[0] = 'S';
        stLcdInf.data[1] = 'G';
        stLcdInf.data[2] = '1';
        break;

    case MRS_SEND_SIGNAL_DISPLAY_MODE2:
        stLcdInf.data[0] = 'S';
        stLcdInf.data[1] = 'G';
        stLcdInf.data[2] = '2';
        break;

    case MRS_SEND_SIGNAL_DISPLAY_CHANGING:
        stLcdInf.data[0] = 'S';
        stLcdInf.data[1] = 'G';
        stLcdInf.data[2] = '-';
        break;

    case MRS_SEND_SIGNAL_DISPLAY_FAIL:
        stLcdInf.data[0] = 'E';
        stLcdInf.data[1] = '0';
        stLcdInf.data[2] = '3';
        stLcdInf.tsec = MRS_SEND_SIGNAL_FAIL_DISPLAY_TIME;
        break;

    default:
        break;
    }

    HI_DRV_LCD_DisplayPrint(HI_LCD_FUNC_ID_SIGNAL_SEND, &stLcdInf);
}


HI_U32 mrsNdmSendSignalStart(HI_VOID)
{
    MRS_NDM_SEND_SIGNAL_CTX *pstCtx = (MRS_NDM_SEND_SIGNAL_CTX *)mrsGetSendSignalCtx();

    pstCtx->ucSendMode = MRS_SEND_SIGNAL_MODE_CHANGING;

    pstCtx->ulRamAddr = (HI_U32)HI_SHARE_MEM_GetMem(MRS_SEND_SIGNAL_FILE_SIZE);
    if (pstCtx->ulRamAddr == (HI_U32)HI_NULL)
    {
        mrsNdmSendSignalDisplay(MRS_SEND_SIGNAL_DISPLAY_NONE);

        pstCtx->ucSendMode = MRS_SEND_SIGNAL_MODE_NONE;
        pstCtx->ucStartFailReason = MRS_SEND_SIGNAL_FAIL_REASON_MEMORY;

        return HI_ERR_NOT_ENOUGH_MEMORY;
    }

    mrsNdmSendSignalDisplay(MRS_SEND_SIGNAL_DISPLAY_CHANGING);

    // Send message to sub task --> generate & send signal file.
    if (HI_ERR_SUCCESS != MRS_SEND_SIGNAL_MSG(EN_NDM_MSG_SEND_SIGNAL_START_PHASE2))
    {
        HI_SHARE_MEM_FreeMem((HI_PVOID)pstCtx->ulRamAddr);
        pstCtx->ulRamAddr = (HI_U32)HI_NULL;

        mrsNdmSendSignalDisplay(MRS_SEND_SIGNAL_DISPLAY_FAIL);

        pstCtx->ucSendMode = MRS_SEND_SIGNAL_MODE_NONE;
        pstCtx->ucStartFailReason = MRS_SEND_SIGNAL_FAIL_REASON_MSG_Q;
        pstCtx->ucMsgSendFailCnt++;

        return HI_ERR_QUEUE_FULL;
    }

    return HI_ERR_SUCCESS;
}


HI_VOID mrsNdmSendSignalKeyProc(HI_BOOL bKeyPress, HI_U32 ulMs)
{
    MRS_NDM_SEND_SIGNAL_CTX *pstCtx = (MRS_NDM_SEND_SIGNAL_CTX *)mrsGetSendSignalCtx();

    if (!pstCtx || !bKeyPress || ulMs != MRS_SEND_SIGNAL_KEY_DOWN_TIME)
    {
        return;
    }

    switch (pstCtx->ucSendMode)
    {
    case MRS_SEND_SIGNAL_MODE_NONE:
        mrsNdmSendSignalStart();
        return;

    case MRS_SEND_SIGNAL_MODE_0:
        MRS_SEND_SIGNAL_MSG(EN_NDM_MSG_SEND_SIGNAL_CHANGE_MODE_1);
        break;

    case MRS_SEND_SIGNAL_MODE_1:
        MRS_SEND_SIGNAL_MSG(EN_NDM_MSG_SEND_SIGNAL_CHANGE_MODE_2);
        break;

    case MRS_SEND_SIGNAL_MODE_2:
        MRS_SEND_SIGNAL_MSG(EN_NDM_MSG_SEND_SIGNAL_CHANGE_MODE_0);
        break;

    default:
        break;
    }

    return;
}


HI_U32 mrsNdmSendSignalMsgProc(HI_U16 usId, HI_PVOID pMsgParam)
{
    HI_UNREF_PARAM(pMsgParam);

    switch (usId)
    {
    case EN_NDM_MSG_SEND_SIGNAL_START_PHASE2:
        mrsNdmSendSignalStartPhase2();
        break;

    case EN_NDM_MSG_SEND_SIGNAL_CHANGE_MODE_0:
        mrsNdmSendSignalChangeToMode0();
        break;

    case EN_NDM_MSG_SEND_SIGNAL_CHANGE_MODE_1:
        mrsNdmSendSignalChangeToMode1();
        break;

    case EN_NDM_MSG_SEND_SIGNAL_CHANGE_MODE_2:
        mrsNdmSendSignalChangeToMode2();
        break;

    case EN_NDM_MSG_SEND_SIGNAL_TIMEOUT:
        mrsNdmSendSignalMode1Timeout();
        break;

    default:
        break;
    }

    return HI_ERR_SUCCESS;
}


HI_U32 mrsNdmSendSignalStartPhase2(HI_VOID)
{
    MRS_NDM_SEND_SIGNAL_CTX *pstCtx = (MRS_NDM_SEND_SIGNAL_CTX *)mrsGetSendSignalCtx();

    // Generate signal file.
    (HI_VOID)HI_MDM_CreateTestSignalFile((HI_U8 *)pstCtx->ulRamAddr, MRS_SEND_SIGNAL_FILE_SIZE);
    HI_MDM_PHY_SetWorkCtrlFlag(PHY_WORK_CTRL_DISABLE);
    HI_MDM_PHY_StartSendSignal((HI_U8 *)pstCtx->ulRamAddr, MRS_SEND_SIGNAL_FILE_SIZE);
    (HI_VOID)mrsNdmSendSignalChangeToMode1();

    return HI_ERR_SUCCESS;
}


HI_U32 mrsNdmSendSignalChangeToMode0(HI_VOID)
{
    MRS_NDM_SEND_SIGNAL_CTX *pstCtx = (MRS_NDM_SEND_SIGNAL_CTX *)mrsGetSendSignalCtx();

    (HI_VOID)HI_MDM_SetAfePgaGain(MRS_PHY_AFE_ANALOG_TX_DB_0);
    (HI_VOID)HI_MDM_LineDriverDisable();

    pstCtx->ucSendMode = MRS_SEND_SIGNAL_MODE_0;
    mrsNdmSendSignalDisplay(MRS_SEND_SIGNAL_DISPLAY_MODE0);

    return HI_ERR_SUCCESS;
}


HI_U32 mrsNdmSendSignalChangeToMode1(HI_VOID)
{
    MRS_NDM_SEND_SIGNAL_CTX *pstCtx = (MRS_NDM_SEND_SIGNAL_CTX *)mrsGetSendSignalCtx();

    pstCtx->ucSendMode = MRS_SEND_SIGNAL_MODE_1;

    if (HI_ERR_SUCCESS != APP_NdmTimerStart(MRS_NDM_TIMER_SEND_SIGNAL, MRS_SEND_SIGNAL_TIME_IN_MODE1, HI_SYS_TIMER_ONESHOT))
    {
        mrsNdmSendSignalDisplay(MRS_SEND_SIGNAL_DISPLAY_FAIL);
        pstCtx->ucStartTimerFailCnt++;

        HI_MDM_SetAfePgaGain(MRS_PHY_AFE_ANALOG_TX_DB_0);
        HI_MDM_LineDriverDisable();

        return HI_ERR_FAILURE;
    }

    (HI_VOID)HI_MDM_SetAfePgaGain(MRS_PHY_AFE_ANALOG_TX_DB_12);
    (HI_VOID)HI_MDM_LineDriverEnable();

    mrsNdmSendSignalDisplay(MRS_SEND_SIGNAL_DISPLAY_MODE1);

    return HI_ERR_SUCCESS;
}


HI_U32 mrsNdmSendSignalChangeToMode2(HI_VOID)
{
    MRS_NDM_SEND_SIGNAL_CTX *pstCtx = (MRS_NDM_SEND_SIGNAL_CTX *)mrsGetSendSignalCtx();

    (HI_VOID)HI_MDM_SetAfePgaGain(MRS_PHY_AFE_ANALOG_TX_DB_6);
    (HI_VOID)HI_MDM_LineDriverEnable();

    pstCtx->ucSendMode = MRS_SEND_SIGNAL_MODE_2;
    mrsNdmSendSignalDisplay(MRS_SEND_SIGNAL_DISPLAY_MODE2);

    return HI_ERR_SUCCESS;
}


HI_VOID mrsNdmSendSignalTimeout(HI_VOID)
{
    MRS_SEND_SIGNAL_MSG(EN_NDM_MSG_SEND_SIGNAL_TIMEOUT);
}


HI_U32 mrsNdmSendSignalMode1Timeout(HI_VOID)
{
    MRS_NDM_SEND_SIGNAL_CTX *pstCtx = (MRS_NDM_SEND_SIGNAL_CTX *)mrsGetSendSignalCtx();

    if (MRS_SEND_SIGNAL_MODE_0 == pstCtx->ucSendMode)
    {
        return HI_ERR_FAILURE;
    }

    if (HI_ERR_SUCCESS != MRS_SEND_SIGNAL_MSG(EN_NDM_MSG_SEND_SIGNAL_CHANGE_MODE_0))
    {
        pstCtx->ucMsgSendFailCnt++;

        HI_MDM_SetAfePgaGain(MRS_PHY_AFE_ANALOG_TX_DB_0);
        (HI_VOID)HI_MDM_LineDriverDisable();

        APP_NdmTimerStart(MRS_NDM_TIMER_SEND_SIGNAL, MRS_SEND_SIGNAL_RESEND_MSG_TIME, HI_SYS_TIMER_ONESHOT);

        return HI_ERR_FAILURE;
    }

    return HI_ERR_SUCCESS;
}

HI_U32 APP_SigSendMsgProc(HI_SYS_QUEUE_MSG_S *pstMsg)
{
    return mrsNdmSendSignalMsgProc((HI_U16)pstMsg->ulParam[0], (HI_PVOID)(&pstMsg->ulParam[1]));
}

HI_END_HEADER
#endif // defined(PRODUCT_CFG_PRODUCT_TYPE_NDM)    

