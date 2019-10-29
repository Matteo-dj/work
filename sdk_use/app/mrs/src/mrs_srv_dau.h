//*****************************************************************************
//
//				  ��Ȩ���� (C), �����й���Ƽ�ͨ�����޹�˾
//
//*****************************************************************************
	  //  �� �� ��	 : mrs_srv_dau.h
	  //  �� �� ��	 : V1.0 
	  //  ��	��	 : luodk
	  //  ��������	 : 2018-7-03
	  //  ��������	 : TODO: ...
	  //			   
	  //  �����б�	 : TODO: ...
	  //  �޸���ʷ	 : 
	  //  1.��	  �� : 2018-07-03
	  //	��	  �� : luodk
	  //	�޸����� : �����ļ� 
	  //
//*****************************************************************************

#ifndef __MRS_SRV_DAU_H__
#define __MRS_SRV_DAU_H__

#if defined(PRODUCT_CFG_SUPPORT_MRS_DUOL_MODE_RF)

#define MRS_RF_DAU_FRAME_LEN 240
typedef struct
{
    HI_U8 ucFrameLen;  // ����Ϊ�����볤�ȼ���
    HI_U8 ucChannelIndex;
    HI_U8 ucStandardIdNum;
    HI_U8 ucFrameHeader;
	
    HI_U16 ucsDataFrameCtrl;  // ֡������
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
