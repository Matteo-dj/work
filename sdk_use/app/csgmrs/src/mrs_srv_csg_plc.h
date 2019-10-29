//*****************************************************************************
//
//                  ��Ȩ���� (C), 2001-2014, ��Ϊ�������޹�˾
//
//*****************************************************************************
//  �� �� ��   : mrs_srv_csg_plc.h
//  �� �� ��   : V1.0 
//  ��    ��   : cuiate/c00233580
//  ��������   : 2016-01-09
//  ��������   : TODO: ...
//               
//  �����б�   : TODO: ...
//  �޸���ʷ   : 
//  1.��    �� : 2016-01-09
//    ��    �� : cuiate/c00233580
//    �޸����� : �����ļ� 
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
