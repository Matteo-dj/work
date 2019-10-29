//*****************************************************************************
//
//                  版权所有 (C), 2001-2014, 华为技术有限公司
//
//*****************************************************************************
//  文 件 名   : mrs_srv_clti_searchmeter_csg.h
//  版 本 号   : V1.0 
//  作    者   : cuiate/c00233580
//  生成日期   : 2015-12-26
//  功能描述   : TODO: ...
//               
//  函数列表   : TODO: ...
//  修改历史   : 
//  1.日    期 : 2015-12-26
//    作    者 : cuiate/c00233580
//    修改内容 : 创建文件 
//
//*****************************************************************************

#ifndef _MRS_SRV_CLTI_SEARCHMETER_CSG_H_
#define _MRS_SRV_CLTI_SEARCHMETER_CSG_H_
HI_START_HEADER

#if (defined(PRODUCT_CFG_PRODUCT_TYPE_STA) && defined(PRODUCT_CFG_PRODUCT_TYPE_CSG_GD))
//*****************************************************************************
//*****************************************************************************

#define MRS_CSG_CLTI_SM_MAXTRY          (3)
#define MRS_CSG_CLTI_SM_TIMEOUT         (100)   // 单位: 100ms
#define MRS_CSG_CLTI_TRYFIRST_TIMEOUT   (36)    // 单位: 100ms

#define MRS_CSG_CLTI_SM_INTERVAL        (60000)

#define CLTI_TRY_DI07_FRAME         {0x68,0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,0x68,0x11,0x04,0x33,0x33,0x34,0x33,0xAE,0x16}
#define CLTI_TRY_DI97_FRAME         {0x68,0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,0x68,0x01,0x02,0x43,0xC3,0xD5,0x16}
 

typedef enum 
{
    MRS_CSG_CLTI_STATUS_IDLE,
    MRS_CSG_CLTI_STATUS_QRY_METER_NUM,
    MRS_CSG_CLTI_STATUS_QRY_METER_INF,
    MRS_CSG_CLTI_STATUS_QRY_DETECT_LIST,
    MRS_CSG_CLTI_STATUS_QRY_DETECT_STATUS,
    MRS_CSG_CLTI_STATUS_FIRST_07,
    MRS_CSG_CLTI_STATUS_FIRST_97,
} MRS_CSG_CLTI_SM_STATUS_E;


typedef struct  
{
    HI_U8 ucTry;
    HI_U8 ucMaxTry;
    HI_U8 ucStatus;
    HI_BOOL bDetectStatus;
    HI_U16 reserved;
    HI_U16 usFrameLength;
    HI_U8 *pucFrame;
} MRS_CSG_CLTI_SM_CONTEXT;


HI_PUBLIC MRS_CSG_CLTI_SM_CONTEXT *mrsCsgCltiCtx(HI_VOID);

HI_PUBLIC HI_U32 mrsCsgCltIQueryMeterNum(HI_VOID);

HI_PUBLIC HI_U32 mrsCsgCltIQueryMeterInf(HI_VOID);

HI_PUBLIC HI_U32 mrsCsgCltIQueryDetectStatus(HI_VOID);

HI_PUBLIC HI_U32 mrsCsgCltIQueryMeterNumRx(MRS_CSG_FRAME_STRU *pstFrame);

HI_PUBLIC HI_U32 mrsCsgCltIQueryMeterInfRx(MRS_CSG_FRAME_STRU *pstFrame);

HI_PUBLIC HI_U32 mrsCsgCltIQueryDetectStatusRx(MRS_CSG_FRAME_STRU *pstFrame);

HI_PUBLIC HI_U32 mrsCsgCltIFrameTimeout(HI_VOID);

HI_PUBLIC HI_U32 mrsCsgCltISearchQuery(HI_VOID);

HI_PUBLIC HI_U32 mrsCsgCltIGenTransmitFrame(MRS_CSG_TRANS_XR_DL_STRU *pstXrFrame, HI_U8 **ppOutFrame, HI_U16 *pusOutLength);

HI_PUBLIC HI_U32 mrsCsgCltISendDetectList(APP_CSG_CMD_DL_METERLIST_STRU *pstMlist);

HI_PUBLIC HI_VOID mrsCsgCltISetStatus(HI_U8 ucStatus);

HI_PUBLIC HI_U8 mrsCsgCltIGetStatus(HI_VOID);

HI_PUBLIC HI_VOID mrsCsgCltISetDetectStatus(HI_BOOL bStatus);

HI_PUBLIC HI_BOOL mrsCsgCltIIsDetectStatus(HI_VOID);

HI_PUBLIC HI_U32 mrsCsgCltITryFirstRx(HI_U8 *pucSrcAddr);

HI_PUBLIC HI_U32 mrsCsgCltITryFirst(HI_VOID);


HI_PUBLIC HI_U32 mrsCsgCltIStopSearch(HI_VOID);
//*****************************************************************************
//*****************************************************************************
#endif

HI_END_HEADER
#endif
