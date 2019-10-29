/********************************************************************************
Copyright (C), Huawei Technologies Co., Ltd. 2012-2020. All rights reserved.
*********************************************************************************
File Name     : mrs_srv_ndm_disptime.c
Version       : Initial draft
Author        : HCT
Created Date  : 2016-10-18
Last Modified by: c00233580
Description   : TODO: ...
Function List : TODO: ...
Change History: TODO: ...
********************************************************************************/
#include <hi_types.h>
#include <hi_mdm_types.h>
#if defined(PRODUCT_CFG_PRODUCT_TYPE_NDM)
#include "mrs_srv_ndm.h"
#include "hi_rtc.h"
#include "mrs_srv_ndm_disptime.h"
HI_START_HEADER


#define MRS_NDM_FORMAT_TIME_TO_LATTICE(szPage, cChar, ucParam) \
    do { \
        szPage[0] = (HI_CHAR)(cChar); \
        szPage[1] = (HI_CHAR)(((ucParam) / 10) % 10) + '0'; \
        szPage[2] = (HI_CHAR)( (ucParam)       % 10) + '0'; \
    } while (0)


MRS_NDM_DISPLAY_TIME_CTX g_stDispTimeCtx;

HI_PRV HI_U32 mrsNdmDisplayPage(HI_VOID);


/**
 @brief     Ndm disp time module init.
 @attention \n
N/A
 @retval ::HI_ERR_SUCCESS success
 @retval ::others failure
 @see \n
N/A
 */
HI_U32 mrsNdmDispTimeInit(HI_VOID)
{
    (hi_void)memset_s(&g_stDispTimeCtx, sizeof(g_stDispTimeCtx), 0, sizeof(g_stDispTimeCtx));
    return HI_ERR_SUCCESS;
}


/**
 @brief     Get NDM disp time module context.
 @attention \n
N/A
 @retval ::Context  NDM disp time module context pointer
 @see \n
N/A
 */
MRS_NDM_DISPLAY_TIME_CTX *mrsNdmDispTimeCtx(HI_VOID)
{
    return &g_stDispTimeCtx;
}


/**
 @brief     NDM display date & time key message process.
 @attention \n
N/A
 @param[in] bKeyPress  Key pressed up/down status, HI_TRUE-down, HI_FALSE-up.
 @param[in] ulMs       Key pressed duration, in milliseconds.
 @see \n
N/A
 */
HI_VOID mrsNdmDispTimeKeyMsgProc(HI_BOOL bKeyPress, HI_U32 ulMs)
{
    MRS_NDM_DISPLAY_TIME_CTX *pstCtx = mrsNdmDispTimeCtx();
    HI_SYS_CALENDAR_TIME_S stTime = {0};
    HI_U32 ulRet = HI_ERR_SUCCESS;
    HI_U32 ulDateTime = 0;

    if (pstCtx->bIsDisp || !bKeyPress || ulMs < MRS_ENABLE_DISPLAY_RTC)
    {
        return;
    }

    // Read date & time.
    ulRet = HI_RTC_ReadTime(&ulDateTime);
    if (HI_ERR_SUCCESS != ulRet)
    {
        return;
    }

    ulDateTime += MRS_TIME_AREA_CORRECT;
    ulRet = HI_MDM_MakeSecondToCalendar(ulDateTime, &stTime);
    if (HI_ERR_SUCCESS != ulRet)
    {
        return;
    }

    MRS_NDM_FORMAT_TIME_TO_LATTICE(pstCtx->szPageInf[0], 'Y', stTime.year);
    MRS_NDM_FORMAT_TIME_TO_LATTICE(pstCtx->szPageInf[1], 'M', stTime.month);
    MRS_NDM_FORMAT_TIME_TO_LATTICE(pstCtx->szPageInf[2], 'D', stTime.day);
    MRS_NDM_FORMAT_TIME_TO_LATTICE(pstCtx->szPageInf[3], 'h', stTime.hour);
    MRS_NDM_FORMAT_TIME_TO_LATTICE(pstCtx->szPageInf[4], 'm', stTime.min);
    MRS_NDM_FORMAT_TIME_TO_LATTICE(pstCtx->szPageInf[5], 's', stTime.sec);

    pstCtx->ucPageIndex = 0;
    pstCtx->bIsDisp = HI_TRUE;

    (HI_VOID)mrsNdmDisplayPage();
    (HI_VOID)APP_NdmTimerStart(MRS_NDM_TIMER_DISP_TIME, MRS_DISP_TIME_INTERVAL, HI_SYS_TIMER_ONESHOT);
}


/**
 @brief     NDM display date & time timer process function.
 @attention \n
N/A
 @see \n
N/A
 */
HI_VOID mrsNdmDispTimeTimeout(HI_VOID)
{
    MRS_NDM_DISPLAY_TIME_CTX *pstCtx = mrsNdmDispTimeCtx();

    // Display the next page. When all page were displayed, close the screen.
    pstCtx->ucPageIndex++;
    if (pstCtx->ucPageIndex >= MRS_DISP_TIME_PAGE_NUM)
    {
        HI_DRV_LCD_DisplayDisable(HI_LCD_FUNC_ID_RTC_TIME);
        pstCtx->bIsDisp = HI_FALSE;
        return;
    }

    (HI_VOID)mrsNdmDisplayPage();
    (HI_VOID)APP_NdmTimerStart(MRS_NDM_TIMER_DISP_TIME, MRS_DISP_TIME_INTERVAL, HI_SYS_TIMER_ONESHOT);
}


/**
 @brief     Ndm disp time in screen.
 @attention \n
N/A
 @retval ::HI_ERR_SUCCESS success
 @retval ::others failure
 @see \n
N/A
 */
HI_U32 mrsNdmDisplayPage(HI_VOID)
{
    MRS_NDM_DISPLAY_TIME_CTX *pstCtx = mrsNdmDispTimeCtx();

    if (pstCtx->ucPageIndex < MRS_DISP_TIME_PAGE_NUM)
    {
        HI_LCD_DISPLAY_INFO_S stLcdInf;

        (hi_void)memset_s(&stLcdInf, sizeof(stLcdInf), 0, sizeof(stLcdInf));
        (hi_void)memcpy_s(stLcdInf.data, sizeof(stLcdInf.data),pstCtx->szPageInf[pstCtx->ucPageIndex], sizeof(stLcdInf.data));
        stLcdInf.pri = HI_LCD_DISPLAY_PRI_3;
        stLcdInf.tsec = HI_LCD_DISPLAY_FOREVER;
        (HI_VOID)HI_DRV_LCD_DisplayPrint(HI_LCD_FUNC_ID_RTC_TIME, &stLcdInf);
    }

    return HI_ERR_SUCCESS;
}


HI_END_HEADER
#endif // defined(PRODUCT_CFG_PRODUCT_TYPE_NDM)    

