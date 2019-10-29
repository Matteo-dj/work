//*****************************************************************************
//
//                  版权所有 (C), 2012-2020, 华为技术有限公司
//
//*****************************************************************************
//  文 件 名   : mrs_dfx_diagcmd.h
//  版 本 号   : V1.0 
//  作    者   : cuiate/c00233580
//  生成日期   : 2017-08-25
//  功能描述   : 新增diag命令处理
//               
//  函数列表   : TODO: ...
//  修改历史   : 
//  1.日    期 : 2017-08-25
//    作    者 : cuiate/c00233580
//    修改内容 : 创建文件 
//
//*****************************************************************************
#ifndef __MRS_DFX_DIAGCMD_H__
#define __MRS_DFX_DIAGCMD_H__
HI_START_HEADER


HI_U32 mrsDfxOsm(HI_U16 usId, HI_PVOID pCmdParam, HI_U16 usCmdParamSize, HI_U32 ulOption);
HI_U32 mrsDfxOssm(HI_U16 usId, HI_PVOID pCmdParam, HI_U16 usCmdParamSize, HI_U32 ulOption);


HI_END_HEADER
#endif //__MRS_DFX_DIAGCMD_H__

