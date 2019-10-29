//*****************************************************************************
//
//                  版权所有 (C), 1998-2013, 华为技术有限公司
//
//*****************************************************************************
//  文 件 名   : mrs_fw_proto376_2_frame.h
//  版 本 号   : V1.0 
//  作    者   : cuiate/00233580
//  生成日期   : 2013-03-11
//  功能描述   : 376.2组帧接口定义
//               
//  函数列表   : TODO: ...
//  修改历史   : 
//  1.日    期 : 2013-03-11
//    作    者 : cuiate/00233580
//    修改内容 : 创建文件 
//
//*****************************************************************************

//*****************************************************************************
// PROJECT   : 
// SUBSYSTEM :
// MODULE    :  
// OWNER     :  
//*****************************************************************************

#ifndef MRS_FW_PROTO376_2_FRAME_H
#define MRS_FW_PROTO376_2_FRAME_H

HI_START_HEADER


#define PROTO3762_START_FLG     (0x68)
#define PROTO3762_END_FLG       (0x16)


typedef struct tagPROTO376_2_FRAME_AFN_AND_DATA_STRU
{
    HI_U8 ucAFN;        // AFN
    HI_U8 ucDT[2];      // DT
    HI_U8 ucPhaseFlag;
    HI_U16 pading2;
    HI_U16 usDataLen;
    HI_U8 *pData;
} PROTO376_2_FRAME_AFN_AND_DATA_STRU;


typedef struct tagPROTO376_2_FRAME_PRE_STRU
{
    // C域
    HI_U8 ucPrm        : 1; // 启动标志 0-从动站 1-启动站

    // R域
    HI_U8 ucModuleFlag : 1; // 通讯模块标识 0-集中器模块 1-子节点模块
    HI_U8 ucEvtFlg     : 1; // 事件标志

    HI_U8 ucIs645      : 1; // 是否是转发645数据
    HI_U8 ucRelayLevel : 4;

    HI_U8 ucSeq;            // 报文序列号
    HI_U8 ucProtocol;       // 规约类型
    HI_U8 ucPhase      : 4; // 相位
    HI_U8 reserved1    : 4; 

    // A域 12字节
    HI_U8 ucSrcAdd[HI_PLC_MAC_ADDR_LEN];
    HI_U8 ucDstAdd[HI_PLC_MAC_ADDR_LEN];

    PROTO376_2_FRAME_AFN_AND_DATA_STRU stAfnData;
} PROTO376_2_FRAME_PRE_STRU;

HI_U32 mrsCreate3762Frame(HI_IN PROTO376_2_FRAME_PRE_STRU *pstFrame, 
        HI_OUT HI_PBYTE *ppOutBuf, HI_OUT HI_U16 *pusBufLen, HI_IN HI_U8 ucMeterAddr[]);

HI_U32 mrsCreate3762UpFrame(HI_IN PROTO376_2_FRAME_AFN_AND_DATA_STRU * pstAfnData, 
        HI_OUT HI_PBYTE *ppOutBuf, HI_OUT HI_U16 *pusBufLen, HI_IN HI_U8 ucMeterAddr[], HI_U8 ucMacAdd[HI_PLC_MAC_ADDR_LEN]);

#if defined(PRODUCT_CFG_PRODUCT_TYPE_CCO_BROAD_BAND_UNIT)
HI_U32 mrsCreateFrame3762Bbu(HI_U16 usDatagramSize, HI_PBYTE pucDatagram, MRS_ONE_RAW_FRAME_STRU** ppstFrame);
HI_U32 mrsCreateAFN13F1Frame3762_Dwon_Bbu(HI_U16 usDatagramSize, HI_PBYTE pucDatagram, MRS_ONE_RAW_FRAME_STRU** ppstFrame);
HI_U32 mrsCreateBroadCastFrame3762_Down_Bbu(HI_U16 usDatagramSize, HI_PBYTE pucDatagram, MRS_ONE_RAW_FRAME_STRU** ppstFrame);
#endif

#if defined(PRODUCT_CFG_SUPPORT_MRS_DUOL_MODE_RF)
HI_U32 mrsCreate3762DownFrame(HI_IN PROTO376_2_FRAME_AFN_AND_DATA_STRU * pstAfnData, 
        HI_OUT HI_PBYTE *ppOutBuf, HI_OUT HI_U16 *pusBufLen, HI_IN HI_U8 ucMeterAddr[], HI_U8 aucMacAdd[HI_PLC_MAC_ADDR_LEN]);
HI_U32 mrsCreate3762FrameDown(HI_IN PROTO376_2_FRAME_PRE_STRU *pstFrame, 
          HI_OUT HI_PBYTE *ppOutBuffer, HI_OUT HI_U16 *pusBufferLen, HI_IN HI_U8 ucMeterAddr[]);

#endif

HI_END_HEADER
#endif //MRS_FW_PROTO376_2_FRAME_H

