/********************************************************************************
Copyright (C), Huawei Technologies Co., Ltd. 2012-2020. All rights reserved.
*********************************************************************************
File Name     : mrs_srv_ndm_disptime.h
Version       : Initial draft
Author        : HCT
Created Date  : 2016-10-18
Last Modified by: c00233580
Description   : TODO: ...
Function List : TODO: ...
Change History: TODO: ...
********************************************************************************/

#ifndef _MRS_SRV_NDM_DISPTIME_H_
#define _MRS_SRV_NDM_DISPTIME_H_
#include <hi_types.h>
#include <hi_mdm_types.h>
#if defined(PRODUCT_CFG_PRODUCT_TYPE_NDM)
#include <hi_lcd.h>
HI_START_HEADER

// Press key 'display' for 2000ms, then display the date & time.
#define MRS_ENABLE_DISPLAY_RTC      (2000)
#define MRS_DISP_TIME_INTERVAL      (1000)
#define MRS_TIME_AREA_CORRECT       (8*3600)
#define MRS_DISP_TIME_PAGE_NUM      (6) // YMDHMS
#define MRS_DISP_CHAR_PER_PAGE      (HI_LCD_POSITION_MAX)


typedef struct  
{
    HI_U8 bIsDisp;
    HI_U8 ucPageIndex;
    HI_CHAR szPageInf[MRS_DISP_TIME_PAGE_NUM][MRS_DISP_CHAR_PER_PAGE];
} MRS_NDM_DISPLAY_TIME_CTX;


/**
 @brief     Ndm disp time module init.
 @attention \n
N/A
 @retval ::HI_ERR_SUCCESS success
 @retval ::others failure
 @see \n
N/A
 */
HI_U32 mrsNdmDispTimeInit(HI_VOID);


/**
 @brief     Get NDM disp time module context.
 @attention \n
N/A
 @retval ::Context  NDM disp time module context pointer
 @see \n
N/A
 */
MRS_NDM_DISPLAY_TIME_CTX *mrsNdmDispTimeCtx(HI_VOID);


/**
 @brief     NDM display date & time key message process.
 @attention \n
N/A
 @param[in] bKeyPress  Key pressed up/down status, HI_TRUE-down, HI_FALSE-up.
 @param[in] ulMs       Key pressed duration, in milliseconds.
 @see \n
N/A
 */
HI_VOID mrsNdmDispTimeKeyMsgProc(HI_BOOL bKeyPress, HI_U32 ulMs);


/**
 @brief     NDM display date & time refresh screen.
 @attention \n
N/A
 @see \n
N/A
 */
HI_VOID mrsNdmDispTimeTimeout(HI_VOID);



HI_END_HEADER
#endif // defined(PRODUCT_CFG_PRODUCT_TYPE_NDM)
#endif // _MRS_SRV_NDM_DISPTIME_H_
