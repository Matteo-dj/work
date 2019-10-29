//*****************************************************************************
//
//                  版权所有 (C), 2001-2015, 华为技术有限公司
//
//*****************************************************************************
//  文 件 名   : mrs_dfx_readmeter_config.h
//  版 本 号   : V1.0
//  作    者   : cuiate/00233580
//  生成日期   : 2015/03/26
//  功能描述   : 抄表超时配置统计信息
//  函数列表   :
//  修改历史   :
//  1.日    期 : 2015/03/26
//    作    者 : cuiate/00233580
//    修改内容 : 创建文件
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

