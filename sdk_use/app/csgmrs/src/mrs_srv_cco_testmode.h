//*****************************************************************************
//
//                  ��Ȩ���� (C), 2001-2015, ��Ϊ�������޹�˾
//
//*****************************************************************************
//  �� �� ��   : mrs_srv_cco_testmode.h
//  �� �� ��   : V1.0
//  ��    ��   : cuiate/00233580
//  ��������   : 2015/09/16
//  ��������   : CCO����ģʽ
//  �����б�   :
//  �޸���ʷ   :
//  1.��    �� : 2015/09/16
//    ��    �� : cuiate/00233580
//    �޸����� : �����ļ�
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

