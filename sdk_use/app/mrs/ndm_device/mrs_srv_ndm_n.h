/********************************************************************************
Copyright (C), Huawei Technologies Co., Ltd. 2012-2020. All rights reserved.
*********************************************************************************
File Name     : mrs_srv_ndm_n.h
Version       : Initial draft
Author        : HCT
Created Date  : 2016-10-13
Last Modified by: c00233580
Description   : Only use in 'mrs_srv_ndm.c'
Function List : TODO: ...
Change History: TODO: ...
********************************************************************************/

#ifndef _MRS_SRV_NDM_N_H_
#define _MRS_SRV_NDM_N_H_
#include <hi_types.h>
#include <hi_mdm_types.h>
#if defined(PRODUCT_CFG_PRODUCT_TYPE_NDM)
#include <hi_ft_nv.h>//自包含from hi_mdm_sys.h
#include <hi_mdm_sys.h>//自包含from hi_mdm_mac.h
#include <hi_mdm_mac.h>//自包含from EQUIP_ZERO_NTB_NUM


#define MRS_NDM_RTC_TIME_TOLERATE   (3) // RTC可接受误差

//#define MRS_NDM_BATTERY_FULL        (100)
//#define MRS_NDM_BATTERY_EMPTY       (20)
#define MRS_NDM_LCD_KEY_TIME        (200)   //按键响应时间
#define MRS_NDM_BATTERY_FLUSH_TIME  (3*MRS_NDM_LCD_KEY_TIME)








HI_PRV HI_VOID mrsNdmKeyPressCallback(HI_BOOL bKeyPress, HI_U32 ulMs, HI_U32 ulUserData);
HI_PRV HI_VOID mrsNdmKeyTestCallback(HI_BOOL bKeyPress, HI_U32 ulMs, HI_U32 ulUserData);

HI_PRV HI_U32 mrsNdmZeroCrossTimeout(HI_VOID);
HI_PRV HI_U32 mrsNdmPlcTestTimeout(HI_VOID);
HI_PRV HI_U32 mrsNdmSsidSetTimeout(HI_VOID);
HI_PRV HI_U32 mrsNdmKeyTestTimeout(HI_VOID);
HI_PRV HI_U32 mrsNdmBeepTimeout(HI_VOID);
HI_PRV HI_U32 mrsNdmDisplayFlashTimeout(HI_VOID);
HI_PRV HI_U32 mrsNdmDisplayOnTimeout(HI_VOID);
HI_PRV HI_U32 mrsNdmDisplayOffTimeout(HI_VOID);


HI_PRV HI_VOID mrsNdmMsgZeroCrossNotify(HI_U32 ulRet, HI_U8 ucLevel, HI_U32 szNtb[EQUIP_ZERO_NTB_NUM]);
HI_PRV HI_U32 mrsNdmMsgPlcRxNotify(HI_PVOID pCmdParam, HI_U16 usCmdParamSize, HI_U32 ulOption);
HI_U32 mrsNdmMsgStartBqUpg(HI_U16 usId, HI_PVOID pMsgParam);
HI_PRV HI_U32 mrsNdmBqUpg(HI_VOID);

HI_PRV HI_U32 mrsNdmMsgStartTfCardTest(HI_VOID);

HI_PRV HI_VOID mrsNdmIOParamInit(HI_VOID);

HI_PRV HI_U32 mrsNdmSetDs1672Time(HI_U32 ulTime);


/**
 @brief     NDM display battery information.
 @attention \n
N/A
 @param[in] bKeyPress  Key pressed up/down status, HI_TRUE-down, HI_FALSE-up.
 @param[in] ulMs       Key pressed duration, in milliseconds.
 @see \n
N/A
 */
HI_VOID mrsNdmBatteryKeyMsgProc(HI_BOOL bKeyPress, HI_U32 ulMs);


/**
 @brief     NDM Tf Rx enable switch.
 @attention \n
N/A
 @param[in] bKeyPress  Key pressed up/down status, HI_TRUE-down, HI_FALSE-up.
 @param[in] ulMs       Key pressed duration, in milliseconds.
 @see \n
N/A
 */
HI_VOID mrsNdmTfRxEnableSwitchKeyMsgProc(HI_BOOL bKeyPress, HI_U32 ulMs);


/**
 @brief     NDM Tf Tx enable switch.
 @attention \n
N/A
 @param[in] bKeyPress  Key pressed up/down status, HI_TRUE-down, HI_FALSE-up.
 @param[in] ulMs       Key pressed duration, in milliseconds.
 @see \n
N/A
 */
HI_VOID mrsNdmTfTxEnableSwitchKeyMsgProc(HI_BOOL bKeyPress, HI_U32 ulMs);


/**
 @brief     NDM Tf transformer number increase.
 @attention \n
N/A
 @param[in] bKeyPress  Key pressed up/down status, HI_TRUE-down, HI_FALSE-up.
 @param[in] ulMs       Key pressed duration, in milliseconds.
 @see \n
N/A
 */
HI_VOID mrsNdmTfTransformerNumKeyMsgProc(HI_BOOL bKeyPress, HI_U32 ulMs);


/**
 @brief     NDM Tf receiver strength display.
 @attention \n
N/A
 @param[in] bKeyPress  Key pressed up/down status, HI_TRUE-down, HI_FALSE-up.
 @param[in] ulMs       Key pressed duration, in milliseconds.
 @see \n
N/A
 */
HI_VOID mrsNdmStrengthKeyMsgProc(HI_BOOL bKeyPress, HI_U32 ulMs);



#endif // defined(PRODUCT_CFG_PRODUCT_TYPE_NDM)

#endif // _MRS_SRV_NDM_N_H_

