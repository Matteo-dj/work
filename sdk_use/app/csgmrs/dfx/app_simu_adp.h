//*****************************************************************************
//
//                  版权所有 (C), 1998-2011, 华为技术有限公司
//
//*****************************************************************************
//  文 件 名   : app_simu_adp.h
//  版 本 号   : V1.0 
//  作    者   : shenhankun/00130424
//  生成日期   : 2011-12-24
//  功能描述   : TODO: ...
//               
//  函数列表   : TODO: ...
//  修改历史   : 
//  1.日    期 : 2011-12-24
//    作    者 : shenhankun/00130424
//    修改内容 : 创建文件 
//
//*****************************************************************************

//*****************************************************************************
// PROJECT   : 
// SUBSYSTEM :
// MODULE    :  
// OWNER     :  
//*****************************************************************************



#ifndef __APP_SIMU_H__
#define __APP_SIMU_H__
#include <hi_types.h>
HI_START_HEADER
 

HI_U32 appSimuInit(HI_VOID);
// HSO-->
HI_U32 SimRxInput(HI_U16 usId, HI_IN HI_PVOID pCmdParam, HI_U16 usCmdParamSize, HI_U32 ulOption);
HI_U32 SimRxCtrl(HI_U16 usId, HI_U16 usPkSn, HI_PVOID pCache, HI_U32 ulOption);

 
HI_END_HEADER 
#endif //__APP_M_H__


