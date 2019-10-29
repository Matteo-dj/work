//*****************************************************************************
//
//                  版权所有 (C), 2001-2015, 华为技术有限公司
//
//*****************************************************************************
//  文 件 名   : mrs_srv_sta_testframe.h
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

#ifndef _MRS_SRV_STA_TESTFRAME_H_
#define _MRS_SRV_STA_TESTFRAME_H_
HI_START_HEADER

#if (defined(PRODUCT_CFG_PRODUCT_TYPE_STA) && defined(PRODUCT_CFG_SUPPORT_TEST_CONCENTRATOR))
//*****************************************************************************
//*****************************************************************************

HI_PUBLIC HI_U32 mrsStaTestModeInit(HI_VOID);

HI_PUBLIC HI_U32 mrsStaTestModeDeinit(HI_VOID);

HI_PUBLIC HI_U32 mrsStaTestModeTimeout(HI_VOID);

HI_PUBLIC HI_VOID mrsStaSetTestModeInf(HI_U16 usInterval, HI_U8 ucSnId);

HI_PUBLIC HI_VOID mrsStaAdjustTestModeSnId(HI_U16 *pusSnId);

HI_PUBLIC HI_VOID mrsStaTestModeBlackListInsert(HI_PBYTE pFrame, HI_U16 usFrameLength);

HI_PUBLIC HI_VOID mrsStaTestModeWhiteListInsert(HI_PBYTE pFrame, HI_U16 usFrameLength);

HI_PUBLIC HI_BOOL mrsStaTestModeIsInBlackList(HI_PBYTE pMeterAddr);

HI_PUBLIC HI_BOOL mrsStaTestModeIsInWhiteList(HI_PBYTE pMeterAddr);


//*****************************************************************************
//*****************************************************************************
#endif // defined(PRODUCT_CFG_PRODUCT_TYPE_STA)

HI_END_HEADER
#endif // _MRS_SRV_STA_TESTFRAME_H_

