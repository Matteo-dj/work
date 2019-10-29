//*****************************************************************************
//
//                  版权所有 (C), 2001-2014, 华为技术有限公司
//
//*****************************************************************************
//  文 件 名   : mrs_fw_proto_csg.h
//  版 本 号   : V1.0 
//  作    者   : cuiate/c00233580
//  生成日期   : 2015-12-09
//  功能描述   : TODO: ...
//               
//  函数列表   : TODO: ...
//  修改历史   : 
//  1.日    期 : 2015-12-09
//    作    者 : cuiate/c00233580
//    修改内容 : 创建文件 
//
//*****************************************************************************

#ifndef _MRS_FW_PROTO_CSG_H_
#define _MRS_FW_PROTO_CSG_H_
HI_START_HEADER

#if ((defined(PRODUCT_CFG_PRODUCT_TYPE_CCO) || defined(PRODUCT_CFG_PRODUCT_TYPE_STA)) && defined(PRODUCT_CFG_PRODUCT_TYPE_CSG_GD))


#define PROTOCSG_CTRL_DIR_BIT_DL    (0)
#define PROTOCSG_CTRL_DIR_BIT_UL    (1)
#define PROTOCSG_CTRL_DIR_UL        (PROTOCSG_CTRL_DIR_BIT_UL << 7)

#define PROTOCSG_CTRL_PRM_BIT_REQ   (1)
#define PROTOCSG_CTRL_PRM_BIT_IND   (0)

#define PROTOCSG_CTRL_ADD_BIT_Y     (1)
#define PROTOCSG_CTRL_ADD_BIT_N     (0)

#define PROTOCSG_CTRL_RESP_FLG_Y    (1)
#define PROTOCSG_CTRL_RESP_FLG_N    (0)


// 南网(广东电网)组帧结构体
typedef struct
{
    // 控制域
    HI_U8 ucPrm      : 1;
    HI_U8 ucAddrFlag : 1;
    HI_U8 reserved   : 6;

    HI_U8 ucAfn;
    HI_U8 ucSeq;
    HI_U8 resvered1; //

    // 应用数据域
    HI_U32 ulDI;

    // A域 12字节
    HI_U8 aucSrcAddr[HI_METER_ADDR_LEN];
    HI_U8 aucDstAddr[HI_METER_ADDR_LEN];

    HI_U8 *pAppData;
    HI_U16 usAppDataLen;
    HI_U16 resevered2;
} MRS_CSG_IND_FRAME_STRU;


HI_EXTERN HI_U32 mrsCreateCsgFrame(MRS_CSG_IND_FRAME_STRU *pstFrame, HI_OUT HI_PBYTE *ppOutBuffer, HI_OUT HI_U16 *pusBufferLen);

HI_EXTERN HI_U32 MRS_ProtoCsgEncode(HI_U8 * pucContent,HI_U16 usLen,HI_VOID * param);


#endif
HI_END_HEADER

#endif // _MRS_FW_PROTO_CSG_H_
