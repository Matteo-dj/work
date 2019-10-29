//*****************************************************************************
//
//                  版权所有 (C), 2001-2015, 华为技术有限公司
//
//*****************************************************************************
//  文 件 名   : mrs_srv_cco_testmode.h
//  版 本 号   : V1.0
//  作    者   : cuiate/00233580
//  生成日期   : 2015/09/16
//  功能描述   : CCO测试模式
//  函数列表   :
//  修改历史   :
//  1.日    期 : 2015/09/16
//    作    者 : cuiate/00233580
//    修改内容 : 创建文件
//
//*****************************************************************************

#ifndef _MRS_SRV_CCO_TESTMODE_H_
#define _MRS_SRV_CCO_TESTMODE_H_
HI_START_HEADER

#if defined(PRODUCT_CFG_PRODUCT_TYPE_CCO)
//*****************************************************************************
//*****************************************************************************


HI_PUBLIC HI_BOOL mrsCcoIsTestMode(HI_VOID);

HI_PUBLIC HI_VOID mrsCcoSetTestModeCtrlInfo(HI_BOOL bEnable, HI_U8 ucFrameTimeout);

HI_PUBLIC HI_U32 mrsCcoTestModeFrameRx(MRS_CMD_TEST_MODE_STRU *pstTestMode);

HI_PUBLIC HI_U32 mrsCcoTestModeFrameTimeout(HI_VOID);


//*****************************************************************************
//*****************************************************************************
#endif // defined(PRODUCT_CFG_PRODUCT_TYPE_CCO)

HI_END_HEADER
#endif // _MRS_SRV_CCO_TESTMODE_H_

