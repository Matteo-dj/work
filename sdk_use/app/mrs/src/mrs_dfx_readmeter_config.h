//*****************************************************************************
//
//                  ��Ȩ���� (C), 2001-2015, ��Ϊ�������޹�˾
//
//*****************************************************************************
//  �� �� ��   : mrs_dfx_readmeter_config.h
//  �� �� ��   : V1.0
//  ��    ��   : cuiate/00233580
//  ��������   : 2015/03/26
//  ��������   : ����ʱ����ͳ����Ϣ
//  �����б�   :
//  �޸���ʷ   :
//  1.��    �� : 2015/03/26
//    ��    �� : cuiate/00233580
//    �޸����� : �����ļ�
//
//*****************************************************************************

#ifndef _MRS_DFX_READMETER_CONFIG_H_
#define _MRS_DFX_READMETER_CONFIG_H_
HI_START_HEADER

#if defined(PRODUCT_CFG_PRODUCT_TYPE_CCO)
//*****************************************************************************
//*****************************************************************************


HI_PUBLIC HI_U32 mrsDfxMrRetryModuleInit(HI_VOID);

HI_PUBLIC HI_U32 mrsDfxMrRetryDiagCmdNotify(HI_U16 usId, HI_IN HI_PVOID pCmdParam, HI_U16 usCmdParamSize, HI_U32 ulOption);

HI_PUBLIC HI_U32 mrsDfxMrRetryDiagCmdProc(HI_SYS_QUEUE_MSG_S *pstMsg);

HI_PUBLIC HI_VOID mrsDfxMrRetryModeChangeStat(HI_U32 ulReqMode, HI_U32 ulCurMode, HI_U32 ulRet, HI_BOOL bSimuChl);


//*****************************************************************************
//*****************************************************************************
#endif // defined(PRODUCT_CFG_PRODUCT_TYPE_CCO)

HI_END_HEADER
#endif // _MRS_DFX_READMETER_CONFIG_H_

