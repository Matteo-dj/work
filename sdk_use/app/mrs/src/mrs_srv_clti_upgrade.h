//*****************************************************************************
//
//                  版权所有 (C), 1998-2016, 华为技术有限公司
//
//*****************************************************************************
//  文 件 名   : mrs_srv_clti_upgrade.h
//  版 本 号   : V1.0 
//  作    者   : cuiate/c00233580
//  生成日期   : 2016-05-26
//  功能描述   : I型采集器升级Demo
//               
//  函数列表   : TODO: ...
//  修改历史   : 
//  1.日    期 : 2016-05-26
//    作    者 : cuiate/c00233580
//    修改内容 : 创建文件 
//
//*****************************************************************************

#ifndef _MRS_SRV_CLTI_UPGRADE_H_
#define _MRS_SRV_CLTI_UPGRADE_H_

#if defined(PRODUCT_CFG_SUPPORT_MRS_UPG_DEMO)
HI_START_HEADER
//*****************************************************************************
//*****************************************************************************

// I采升级模块初始化
HI_U32 mrsCltIUpgModuleInit(HI_VOID);

// I采升级模块去初始化
HI_U32 mrsCltIUpgModuleDeinit(HI_VOID);

// I采升级消息处理
HI_U32 mrsCltIUpgMsgProc(HI_SYS_QUEUE_MSG_S *pstMsg);

// I采升级定时器处理
HI_U32 mrsCltIUpgTimerProc(HI_VOID);


//*****************************************************************************
HI_END_HEADER
#endif // defined(PRODUCT_CFG_SUPPORT_MRS_UPG_DEMO)

#endif //_MRS_SRV_CLTI_UPGRADE_H_
