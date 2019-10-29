//*****************************************************************************
//
//                  版权所有 (C), 2001-2014, 华为技术有限公司
//
//*****************************************************************************
//  文 件 名   : mrs_fw_proto_csg_echo_sta.h
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

#ifndef _MRS_FW_PROTO_CSG_ECHO_STA_H_
#define _MRS_FW_PROTO_CSG_ECHO_STA_H_
HI_START_HEADER

#if (defined(PRODUCT_CFG_PRODUCT_TYPE_STA) && defined(PRODUCT_CFG_PRODUCT_TYPE_CSG_GD))
//*****************************************************************************
//*****************************************************************************

#define MRS_CSG_UPG_IND_LEN_FILE_INFO           (17)
#define MRS_CSG_UPG_IND_LEN_PROCESS   			(2)

typedef struct
{
    HI_U8 ucStartFlag;
    HI_U8 ucLenLo;
    HI_U8 ucLenHi;

    HI_U8 ucReserved : 5;
    HI_U8 ucAddrFlag : 1;
    HI_U8 ucPrm : 1;
    HI_U8 ucDir : 1;

    HI_U8 aucSrcAddr[HI_METER_ADDR_LEN];
    HI_U8 aucDstAddr[HI_METER_ADDR_LEN];

    HI_U8 ucAfn;
    HI_U8 ucSeq;

    HI_U8 ucDiLo;
    HI_U8 ucDiMl;
    HI_U8 ucDiMh;
    HI_U8 ucDiHi;

    HI_U8 ucFrameLenLo;
    HI_U8 ucFrameLenHi;

    HI_U8 aucData[0];
} MRS_CSG_TRANSIMIT_UL_STRU;


HI_PUBLIC HI_U32 mrsStaProtoCsgEchoInit(HI_VOID);

HI_PUBLIC HI_U32 mrsStaProtoCsgEchoDeinit(HI_VOID);


// 确认
HI_U32 mrsCsg_AFN00_EA010001(HI_INOUT HI_VOID *pParam);

// 否认
HI_U32 mrsCsg_AFN00_EA010002(HI_INOUT HI_VOID *pParam);


// 请求表地址个数
HI_U32 mrsCsg_AFN21_EA062101(HI_INOUT HI_VOID *pParam);

// 返回表地址
HI_U32 mrsCsg_AFN21_EA032102(HI_INOUT HI_VOID *pParam);

// 请求采集器地址
HI_U32 mrsCsg_AFN21_EA062103(HI_INOUT HI_VOID *pParam);

// 请求探测状态
HI_U32 mrsCsg_AFN21_EA062105(HI_INOUT HI_VOID *pParam);


// 透传上行数据到采集器应答
HI_U32 mrsCsg_AFN22_EA032201(HI_INOUT HI_VOID *pParam);


// 查询厂商代码和版本信息
HI_U32 mrsCsg_AFN23_EA002301(HI_INOUT HI_VOID *pParam);


// 请求文件信息
HI_U32 mrsCsg_AFN24_EA062403(HI_INOUT HI_VOID *pParam);

// 请求文件处理进度
HI_U32 mrsCsg_AFN24_EA062404(HI_INOUT HI_VOID *pParam);



//*****************************************************************************
//*****************************************************************************
#endif

HI_END_HEADER
#endif
