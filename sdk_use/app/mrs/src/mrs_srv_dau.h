//*****************************************************************************
//
//				  版权所有 (C), 深圳市国电科技通信有限公司
//
//*****************************************************************************
	  //  文 件 名	 : mrs_srv_dau.h
	  //  版 本 号	 : V1.0 
	  //  作	者	 : luodk
	  //  生成日期	 : 2018-7-03
	  //  功能描述	 : TODO: ...
	  //			   
	  //  函数列表	 : TODO: ...
	  //  修改历史	 : 
	  //  1.日	  期 : 2018-07-03
	  //	作	  者 : luodk
	  //	修改内容 : 创建文件 
	  //
//*****************************************************************************

#ifndef __MRS_SRV_DAU_H__
#define __MRS_SRV_DAU_H__

#if defined(PRODUCT_CFG_SUPPORT_MRS_DUOL_MODE_RF)

#define MRS_RF_DAU_FRAME_LEN 240
typedef struct
{
    HI_U8 ucFrameLen;  // 长度为数据与长度加上
    HI_U8 ucChannelIndex;
    HI_U8 ucStandardIdNum;
    HI_U8 ucFrameHeader;
	
    HI_U16 ucsDataFrameCtrl;  // 帧控制域
    HI_U8 ucDataFrameSn;
    HI_U16 usDataPinId;
    HI_U8 ucaDataDstAddr[7];
	HI_U8 ucaDataSrcAddr[7];
	HI_U8 ucDataCmdId;
	HI_U8 ucDataLen;
    HI_U8 ucapDataData[MRS_RF_DAU_FRAME_LEN];
    HI_U16 usCRC16;
}MRS_RF_DAU_FRAME_STRU;

HI_U8 mrsGetDauFrameSn(HI_VOID);
HI_VOID mrsAddDauFrameSn(HI_VOID);
HI_VOID mrsSetDauFrameSn(HI_U8 ucFrameSn);

HI_U16 GetCrc16(HI_U8  *pData,HI_S32 nLength);

HI_U8 mrsRFDAUFrameHeader(HI_U8 ucFrameLen,HI_U8 ucChannelIndex,HI_U8 ucStandardIdNum);
HI_U32 mrsRFDauEncode(HI_U8 **ppData,HI_U16 *ucpDataLen,MRS_RF_DAU_FRAME_STRU *pDauData);
MRS_RF_DAU_FRAME_STRU * mrsRFDauDecode(HI_U8 *pData,HI_U8 ucDataLen);
MRS_RF_DAU_FRAME_STRU * mrsRFDauOnwWayDecode(HI_U8 *pData,HI_U8 ucDataLen);

#endif
#endif
