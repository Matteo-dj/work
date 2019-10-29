//*****************************************************************************
//
//				  版权所有 (C), 深圳市国电科技通信有限公司
//
//*****************************************************************************
	  //  文 件 名	 : mrs_srv_dau.c
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
#include "mrs_common.h"
#include "app_config.h"

#if defined(PRODUCT_CFG_SUPPORT_MRS_DUOL_MODE_RF)
#include "mrs_fw_log.h"
#include "mrs_fw_tools.h"
#include "mrs_srv_common.h"

#include "mrs_srv_res.h"
#include "mrs_cmd_msg.h"
#include "hi_mdm_time.h"
#include "hi_mdm_mac.h"

#include "mrs_srv_dau.h"

HI_U8 g_DauFrameSn=1;
HI_U8 mrsGetDauFrameSn(HI_VOID)
{
	return g_DauFrameSn;
}
HI_VOID mrsAddDauFrameSn(HI_VOID)
{
	g_DauFrameSn++;
	if(!g_DauFrameSn)
	{
		g_DauFrameSn++;
	}
}
HI_VOID mrsSetDauFrameSn(HI_U8 ucFrameSn)
{
	g_DauFrameSn=ucFrameSn;
}

/*
CRC-ITU   x16+x12+x5+1   1021  1000000100001 最高位省略 否则为11021
*/
//查表法求得16位CRC,高位在后低位在前
HI_U16 GetCrc16(HI_U8  *pData,HI_S32 nLength);

const unsigned short crctab16[] =
{
	0x0000, 0x1189, 0x2312, 0x329b, 0x4624, 0x57ad, 0x6536, 0x74bf,
	0x8c48, 0x9dc1, 0xaf5a, 0xbed3, 0xca6c, 0xdbe5, 0xe97e, 0xf8f7,
	0x1081, 0x0108, 0x3393, 0x221a, 0x56a5, 0x472c, 0x75b7, 0x643e,
	0x9cc9, 0x8d40, 0xbfdb, 0xae52, 0xdaed, 0xcb64, 0xf9ff, 0xe876,
	0x2102, 0x308b, 0x0210, 0x1399, 0x6726, 0x76af, 0x4434, 0x55bd,
	0xad4a, 0xbcc3, 0x8e58, 0x9fd1, 0xeb6e, 0xfae7, 0xc87c, 0xd9f5,
	0x3183, 0x200a, 0x1291, 0x0318, 0x77a7, 0x662e, 0x54b5, 0x453c,
	0xbdcb, 0xac42, 0x9ed9, 0x8f50, 0xfbef, 0xea66, 0xd8fd, 0xc974,
	0x4204, 0x538d, 0x6116, 0x709f, 0x0420, 0x15a9, 0x2732, 0x36bb,
	0xce4c, 0xdfc5, 0xed5e, 0xfcd7, 0x8868, 0x99e1, 0xab7a, 0xbaf3,
	0x5285, 0x430c, 0x7197, 0x601e, 0x14a1, 0x0528, 0x37b3, 0x263a,
	0xdecd, 0xcf44, 0xfddf, 0xec56, 0x98e9, 0x8960, 0xbbfb, 0xaa72,
	0x6306, 0x728f, 0x4014, 0x519d, 0x2522, 0x34ab, 0x0630, 0x17b9,
	0xef4e, 0xfec7, 0xcc5c, 0xddd5, 0xa96a, 0xb8e3, 0x8a78, 0x9bf1,
	0x7387, 0x620e, 0x5095, 0x411c, 0x35a3, 0x242a, 0x16b1, 0x0738,
	0xffcf, 0xee46, 0xdcdd, 0xcd54, 0xb9eb, 0xa862, 0x9af9, 0x8b70,
	0x8408, 0x9581, 0xa71a, 0xb693, 0xc22c, 0xd3a5, 0xe13e, 0xf0b7,
	0x0840, 0x19c9, 0x2b52, 0x3adb, 0x4e64, 0x5fed, 0x6d76, 0x7cff,
	0x9489, 0x8500, 0xb79b, 0xa612, 0xd2ad, 0xc324, 0xf1bf, 0xe036,
	0x18c1, 0x0948, 0x3bd3, 0x2a5a, 0x5ee5, 0x4f6c, 0x7df7, 0x6c7e,
	0xa50a, 0xb483, 0x8618, 0x9791, 0xe32e, 0xf2a7, 0xc03c, 0xd1b5,
	0x2942, 0x38cb, 0x0a50, 0x1bd9, 0x6f66, 0x7eef, 0x4c74, 0x5dfd,
	0xb58b, 0xa402, 0x9699, 0x8710, 0xf3af, 0xe226, 0xd0bd, 0xc134,
	0x39c3, 0x284a, 0x1ad1, 0x0b58, 0x7fe7, 0x6e6e, 0x5cf5, 0x4d7c,
	0xc60c, 0xd785, 0xe51e, 0xf497, 0x8028, 0x91a1, 0xa33a, 0xb2b3,
	0x4a44, 0x5bcd, 0x6956, 0x78df, 0x0c60, 0x1de9, 0x2f72, 0x3efb,
	0xd68d, 0xc704, 0xf59f, 0xe416, 0x90a9, 0x8120, 0xb3bb, 0xa232,
	0x5ac5, 0x4b4c, 0x79d7, 0x685e, 0x1ce1, 0x0d68, 0x3ff3, 0x2e7a,
	0xe70e, 0xf687, 0xc41c, 0xd595, 0xa12a, 0xb0a3, 0x8238, 0x93b1,
	0x6b46, 0x7acf, 0x4854, 0x59dd, 0x2d62, 0x3ceb, 0x0e70, 0x1ff9,
	0xf78f, 0xe606, 0xd49d, 0xc514, 0xb1ab, 0xa022, 0x92b9, 0x8330,
	0x7bc7, 0x6a4e, 0x58d5, 0x495c, 0x3de3, 0x2c6a, 0x1ef1, 0x0f78,
};

HI_U16 GetCrc16(HI_U8  *pData,HI_S32 nLength)
{
	unsigned short fcs = 0xffff;    // 初始化
	while (nLength > 0)
	{
		fcs = (fcs >> 8) ^crctab16[(fcs ^ *pData) & 0xff];
		nLength--;
		*pData++;
	}
	return (unsigned short)(~fcs);// 取反
}

HI_U8 mrsRFDAUFrameHeader(HI_U8 ucFrameLen,HI_U8 ucChannelIndex,HI_U8 ucStandardIdNum)
{
	return (ucFrameLen^ucChannelIndex^ucStandardIdNum);
}
HI_U32 mrsRFDauEncode(HI_U8 **ppData,HI_U16 *ucpDataLen,MRS_RF_DAU_FRAME_STRU *pDauData)
{
	HI_U8 *pBuf= mrsToolsMalloc(256);
	HI_U8 ucOffent=0;
	HI_U16 usCrc16=0;
	if((pBuf==HI_NULL)|| (pDauData==HI_NULL))
	{
		return HI_ERR_MALLOC_FAILUE;
	}
	pBuf[ucOffent++]=pDauData->ucFrameLen;
	pBuf[ucOffent++]=pDauData->ucChannelIndex;
	pBuf[ucOffent++]=pDauData->ucStandardIdNum;
	pBuf[ucOffent++]=pDauData->ucFrameHeader;
	
	pBuf[ucOffent++]=((pDauData->ucsDataFrameCtrl >>8)&0xFF);
	pBuf[ucOffent++]=(pDauData->ucsDataFrameCtrl&0xFF);
	
	pBuf[ucOffent++]=pDauData->ucDataFrameSn;
	
	pBuf[ucOffent++]=(pDauData->usDataPinId&0xFF);
	pBuf[ucOffent++]=((pDauData->usDataPinId >>8)&0xFF);
	mrsToolsMemcpy_s(pBuf+ucOffent,7,pDauData->ucaDataDstAddr,7);
	ucOffent+=7;
	mrsToolsMemcpy_s(pBuf+ucOffent,7,pDauData->ucaDataSrcAddr,7);
	ucOffent+=7;
	pBuf[ucOffent++]=pDauData->ucDataCmdId;
	
	mrsToolsMemcpy_s(pBuf+ucOffent,pDauData->ucDataLen,pDauData->ucapDataData,pDauData->ucDataLen);
	ucOffent +=pDauData->ucDataLen;
	
	usCrc16=GetCrc16(pBuf,ucOffent);
	pBuf[ucOffent++] =(usCrc16&0xFF);
	pBuf[ucOffent++] =((usCrc16>>8)&0xFF);
	
	*ppData = pBuf;
	*ucpDataLen = ucOffent;
	
	return HI_ERR_SUCCESS;
}
MRS_RF_DAU_FRAME_STRU * mrsRFDauDecode(HI_U8 *pData,HI_U8 ucDataLen)
{
	HI_U8 ucOffent=0;
	MRS_RF_DAU_FRAME_STRU *pDauFrameData =HI_NULL;
	if((ucDataLen<26)||((pData[0]+3)!=ucDataLen))
	{
		return HI_NULL;
	}
	pDauFrameData= (MRS_RF_DAU_FRAME_STRU *)mrsToolsMalloc(sizeof(MRS_RF_DAU_FRAME_STRU));
	if(pDauFrameData==HI_NULL)
	{
		return HI_NULL;
	}
	mrsToolsMemZero_s(pDauFrameData, sizeof(MRS_RF_DAU_FRAME_STRU), sizeof(MRS_RF_DAU_FRAME_STRU));
	pDauFrameData->ucFrameLen = pData[ucOffent++];
	pDauFrameData->ucChannelIndex = pData[ucOffent++];
	pDauFrameData->ucStandardIdNum = pData[ucOffent++];
	pDauFrameData->ucFrameHeader = pData[ucOffent++];

	pDauFrameData->ucsDataFrameCtrl = (pData[ucOffent]|(pData[ucOffent+1]<<8));
	ucOffent +=2;
	pDauFrameData->ucDataFrameSn = pData[ucOffent++];
	pDauFrameData->usDataPinId = (pData[ucOffent]|(pData[ucOffent+1]<<8));
	ucOffent +=2;
	mrsToolsMemcpy_s(pDauFrameData->ucaDataDstAddr,7,pData+ucOffent,7);
	ucOffent+=7;
	mrsToolsMemcpy_s(pDauFrameData->ucaDataSrcAddr,7,pData+ucOffent,7);
	ucOffent+=7;
	pDauFrameData->ucDataCmdId = pData[ucOffent++];
	
	pDauFrameData->ucDataLen = ucDataLen-ucOffent-2;
	mrsToolsMemcpy_s(pDauFrameData->ucapDataData,MRS_RF_DAU_FRAME_LEN,pData+ucOffent,pDauFrameData->ucDataLen);
	ucOffent +=pDauFrameData->ucDataLen;

	pDauFrameData->usCRC16 = (pData[ucOffent]|(pData[ucOffent+1]<<8));
	ucOffent +=2;
	return (pDauFrameData);
}

// 双向水表和单向水表用的还是不同的协议，太坑人了
MRS_RF_DAU_FRAME_STRU * mrsRFDauOnwWayDecode(HI_U8 *pData,HI_U8 ucDataLen)
{
	HI_U8 ucOffent=0;
	MRS_RF_DAU_FRAME_STRU *pDauFrameData =HI_NULL;
	if((ucDataLen<26)||((pData[0]+3)!=ucDataLen))
	{
		return HI_NULL;
	}
	pDauFrameData= (MRS_RF_DAU_FRAME_STRU *)mrsToolsMalloc(sizeof(MRS_RF_DAU_FRAME_STRU));
	if(pDauFrameData==HI_NULL)
	{
		return HI_NULL;
	}
	mrsToolsMemZero_s(pDauFrameData, sizeof(MRS_RF_DAU_FRAME_STRU), sizeof(MRS_RF_DAU_FRAME_STRU));
	pDauFrameData->ucFrameLen = pData[ucOffent++];
	pDauFrameData->ucChannelIndex = pData[ucOffent++];
	pDauFrameData->ucStandardIdNum = pData[ucOffent++];
	pDauFrameData->ucFrameHeader = pData[ucOffent++];

	pDauFrameData->ucsDataFrameCtrl = (pData[ucOffent]|(pData[ucOffent+1]<<8));
	ucOffent +=2;
	pDauFrameData->ucDataFrameSn = pData[ucOffent++];
	pDauFrameData->usDataPinId = (pData[ucOffent]|(pData[ucOffent+1]<<8));
	ucOffent +=2;
	mrsToolsMemcpy_s(pDauFrameData->ucaDataDstAddr,7,pData+ucOffent,6);
	ucOffent+=6;
	mrsToolsMemcpy_s(pDauFrameData->ucaDataSrcAddr,7,pData+ucOffent,6);
	ucOffent+=6;
	pDauFrameData->ucDataCmdId = pData[ucOffent++];
	
	pDauFrameData->ucDataLen = ucDataLen-ucOffent-2;
	mrsToolsMemcpy_s(pDauFrameData->ucapDataData,MRS_RF_DAU_FRAME_LEN,pData+ucOffent,pDauFrameData->ucDataLen);
	ucOffent +=pDauFrameData->ucDataLen;

	pDauFrameData->usCRC16 = (pData[ucOffent]|(pData[ucOffent+1]<<8));
	ucOffent +=2;
	return (pDauFrameData);
}



#endif

