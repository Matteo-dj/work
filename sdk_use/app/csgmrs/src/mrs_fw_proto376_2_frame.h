//*****************************************************************************
//
//                  ��Ȩ���� (C), 1998-2013, ��Ϊ�������޹�˾
//
//*****************************************************************************
//  �� �� ��   : mrs_fw_proto376_2_frame.h
//  �� �� ��   : V1.0 
//  ��    ��   : cuiate/00233580
//  ��������   : 2013-03-11
//  ��������   : 376.2��֡�ӿڶ���
//               
//  �����б�   : TODO: ...
//  �޸���ʷ   : 
//  1.��    �� : 2013-03-11
//    ��    �� : cuiate/00233580
//    �޸����� : �����ļ� 
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
    HI_U8 pading1;
    HI_U16 pading2;
    HI_U16 usDataLen;
    HI_U8 *pData;
} PROTO376_2_FRAME_AFN_AND_DATA_STRU;


typedef struct tagPROTO376_2_FRAME_PRE_STRU
{
    // C��
    HI_U8 ucPrm        : 1; // ������־ 0-�Ӷ�վ 1-����վ

    // R��
    HI_U8 ucModuleFlag : 1; // ͨѶģ���ʶ 0-������ģ�� 1-�ӽڵ�ģ��
    HI_U8 ucEvtFlg     : 1; // �¼���־

    HI_U8 ucIs645      : 1; // �Ƿ���ת��645����
    HI_U8 reserved     : 4;

    HI_U8 ucSeq;            // �������к�
    HI_U8 ucProtocol;       // ��Լ����
    HI_U8 reserved1;

    // A�� 12�ֽ�
    HI_U8 ucSrcAdd[HI_PLC_MAC_ADDR_LEN];
    HI_U8 ucDstAdd[HI_PLC_MAC_ADDR_LEN];

    PROTO376_2_FRAME_AFN_AND_DATA_STRU stAfnData;
} PROTO376_2_FRAME_PRE_STRU;

HI_U32 mrsCreate3762Frame(HI_IN PROTO376_2_FRAME_PRE_STRU *pstFrame, 
        HI_OUT HI_PBYTE *ppOutBuf, HI_OUT HI_U16 *pusBufLen, HI_IN HI_U8 ucMeterAddr[]);

HI_U32 mrsCreate3762UpFrame(HI_IN PROTO376_2_FRAME_AFN_AND_DATA_STRU * pstAfnData, 
        HI_OUT HI_PBYTE *ppOutBuf, HI_OUT HI_U16 *pusBufLen, HI_IN HI_U8 ucMeterAddr[]);

#if defined(PRODUCT_CFG_PRODUCT_TYPE_CCO_BROAD_BAND_UNIT)
HI_U32 mrsCreateFrame3762Bbu(HI_U16 usDatagramSize, HI_PBYTE pucDatagram, MRS_ONE_RAW_FRAME_STRU** ppstFrame);
HI_U32 mrsCreateAFN13F1Frame3762_Dwon_Bbu(HI_U16 usDatagramSize, HI_PBYTE pucDatagram, MRS_ONE_RAW_FRAME_STRU** ppstFrame);
HI_U32 mrsCreateBroadCastFrame3762_Down_Bbu(HI_U16 usDatagramSize, HI_PBYTE pucDatagram, MRS_ONE_RAW_FRAME_STRU** ppstFrame);
#endif

HI_END_HEADER
#endif //MRS_FW_PROTO376_2_FRAME_H

