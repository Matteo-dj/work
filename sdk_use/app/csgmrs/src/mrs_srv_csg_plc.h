//*****************************************************************************
//
//                  版权所有 (C), 2001-2014, 华为技术有限公司
//
//*****************************************************************************
//  文 件 名   : mrs_srv_csg_plc.h
//  版 本 号   : V1.0 
//  作    者   : cuiate/c00233580
//  生成日期   : 2016-01-09
//  功能描述   : TODO: ...
//               
//  函数列表   : TODO: ...
//  修改历史   : 
//  1.日    期 : 2016-01-09
//    作    者 : cuiate/c00233580
//    修改内容 : 创建文件 
//
//*****************************************************************************

#ifndef _MRS_SRV_CSG_PLC_H_
#define _MRS_SRV_CSG_PLC_H_
HI_START_HEADER

#if ((defined(PRODUCT_CFG_PRODUCT_TYPE_CCO) || defined(PRODUCT_CFG_PRODUCT_TYPE_STA)) && defined(PRODUCT_CFG_PRODUCT_TYPE_CSG_GD))
//*****************************************************************************
//*****************************************************************************

typedef HI_U32 (*MRS_CSG_PLC_FUNC)(HI_PVOID pParam, HI_U16 usSize);

typedef struct  
{
    HI_U8 ucType : 4;
    HI_U8 reserved1 : 4;
    HI_U8 ucSrvId;
    HI_U8 reserved2[2];
    MRS_CSG_PLC_FUNC pfnFunc;
} MRS_CSG_PLC_FUNC_TBL_STRU;


typedef struct
{
    HI_U32 ulFuncNum;
    MRS_CSG_PLC_FUNC_TBL_STRU *pstTblList;
} MRS_CSG_PLC_CTX_STRU;


HI_PUBLIC MRS_CSG_PLC_CTX_STRU *mrsCsgPlcModuleCtx(HI_VOID);

HI_PUBLIC HI_VOID mrsCsgPlcRegProcTblList(HI_U32 ulFuncNum, MRS_CSG_PLC_FUNC_TBL_STRU *pstTblList);

HI_PUBLIC HI_U32 mrsCsgPlcFrameProc(HI_PVOID pReqPacket, HI_U16 usPacketSize);

HI_PUBLIC HI_U32 mrsCsgPlcCmdTransFileProc(HI_PVOID pParam, HI_U16 usSize);

HI_PUBLIC HI_U32 mrsCsgPlcCmdQueryInfoProc(HI_PVOID pParam, HI_U16 usSize);

HI_PUBLIC HI_U32 mrsCsgPlcAckFrame(HI_U8 *pucMacAddr, HI_U16 usSeq, HI_BOOL bDlUl);

HI_PUBLIC HI_U32 mrsCsgPlcDenyFrame(HI_U8 *pucMacAddr, HI_U16 usSeq, HI_BOOL bDlUl, HI_U8 ucDeny);


//*****************************************************************************
//*****************************************************************************
#endif

HI_END_HEADER
#endif
