//*****************************************************************************
//
//                  ��Ȩ���� (C), 2001-2014, ��Ϊ�������޹�˾
//
//*****************************************************************************
//  �� �� ��   : mrs_fw_proto_csg.h
//  �� �� ��   : V1.0 
//  ��    ��   : cuiate/c00233580
//  ��������   : 2015-12-09
//  ��������   : TODO: ...
//               
//  �����б�   : TODO: ...
//  �޸���ʷ   : 
//  1.��    �� : 2015-12-09
//    ��    �� : cuiate/c00233580
//    �޸����� : �����ļ� 
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


// ����(�㶫����)��֡�ṹ��
typedef struct
{
    // ������
    HI_U8 ucPrm      : 1;
    HI_U8 ucAddrFlag : 1;
    HI_U8 reserved   : 6;

    HI_U8 ucAfn;
    HI_U8 ucSeq;
    HI_U8 resvered1; //

    // Ӧ��������
    HI_U32 ulDI;

    // A�� 12�ֽ�
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
