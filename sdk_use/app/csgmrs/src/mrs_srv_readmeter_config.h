//*****************************************************************************
//
//                  版权所有 (C), 2001-2015, 华为技术有限公司
//
//*****************************************************************************
//  文 件 名   : mrs_srv_readmeter_config.h
//  版 本 号   : V1.0
//  作    者   : cuiate/00233580
//  生成日期   : 2015/03/26
//  功能描述   : 抄表超时配置功能相关函数及结构声明
//  函数列表   :
//  修改历史   :
//  1.日    期 : 2015/03/26
//    作    者 : cuiate/00233580
//    修改内容 : 创建文件
//
//*****************************************************************************

#ifndef _MRS_SRV_READMETER_CONFIG_H_
#define _MRS_SRV_READMETER_CONFIG_H_
HI_START_HEADER

#if defined(PRODUCT_CFG_PRODUCT_TYPE_CCO)
//*****************************************************************************
//*****************************************************************************

#define MRS_RM_CFG_INFO_MAX (DIAG_APP_MRS_TIMEOUT_MODE_MAX)

#define MRS_RM_XR   (0)
#define MRS_RM_LR   (1)
#define MRS_RM_PR   (2)

typedef struct
{
    HI_BOOL bInited;
    HI_BOOL bSimuChl;
    HI_U8   aucPadding[2];
    HI_U32  ulMode;
} MRS_RM_CONFIG_CONTEXT_STRU;



HI_PUBLIC HI_U32 mrsRmCfgModuleInit(HI_VOID);

HI_PUBLIC MRS_RM_CONFIG_CONTEXT_STRU *mrsRmCfgGetModuleCtx(HI_VOID);

HI_PUBLIC HI_VOID mrsRmCfgSetSimuChlStatus(HI_BOOL bEnable);

HI_PUBLIC HI_U32 mrsRmCfgFastNvInit(HI_VOID);

HI_PUBLIC HI_U32 mrsRmCfgDefaultNvInit(HI_VOID);

HI_PUBLIC HI_U32 mrsRmCfgUserDefineInit(NV_MRS_RETRY_CFG_S *pstCfg);

HI_PUBLIC HI_U32 mrsDiagGetMrRetryCfg(HI_U16 usId, HI_PVOID pCmdParam, HI_U16 usCmdParamSize, HI_U32 ulOption);

HI_PUBLIC HI_U32 mrsDiagSetMrRetryCfg(HI_U16 usId, HI_PVOID pCmdParam, HI_U16 usCmdParamSize, HI_U32 ulOption);


//*****************************************************************************
//*****************************************************************************
#endif // defined(PRODUCT_CFG_PRODUCT_TYPE_CCO)

HI_END_HEADER
#endif // _MRS_SRV_READMETER_CONFIG_H_

