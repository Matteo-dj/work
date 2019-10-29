//*****************************************************************************
//
//                  ��Ȩ���� (C), 2001-2014, ��Ϊ�������޹�˾
//
//*****************************************************************************
//  �� �� ��   : mrs_fw_proto_csg_echo_sta.h
//  �� �� ��   : V1.0 
//  ��    ��   : cuiate/c00233580
//  ��������   : 2015-12-26
//  ��������   : TODO: ...
//               
//  �����б�   : TODO: ...
//  �޸���ʷ   : 
//  1.��    �� : 2015-12-26
//    ��    �� : cuiate/c00233580
//    �޸����� : �����ļ� 
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


// ȷ��
HI_U32 mrsCsg_AFN00_EA010001(HI_INOUT HI_VOID *pParam);

// ����
HI_U32 mrsCsg_AFN00_EA010002(HI_INOUT HI_VOID *pParam);


// ������ַ����
HI_U32 mrsCsg_AFN21_EA062101(HI_INOUT HI_VOID *pParam);

// ���ر��ַ
HI_U32 mrsCsg_AFN21_EA032102(HI_INOUT HI_VOID *pParam);

// ����ɼ�����ַ
HI_U32 mrsCsg_AFN21_EA062103(HI_INOUT HI_VOID *pParam);

// ����̽��״̬
HI_U32 mrsCsg_AFN21_EA062105(HI_INOUT HI_VOID *pParam);


// ͸���������ݵ��ɼ���Ӧ��
HI_U32 mrsCsg_AFN22_EA032201(HI_INOUT HI_VOID *pParam);


// ��ѯ���̴���Ͱ汾��Ϣ
HI_U32 mrsCsg_AFN23_EA002301(HI_INOUT HI_VOID *pParam);


// �����ļ���Ϣ
HI_U32 mrsCsg_AFN24_EA062403(HI_INOUT HI_VOID *pParam);

// �����ļ��������
HI_U32 mrsCsg_AFN24_EA062404(HI_INOUT HI_VOID *pParam);



//*****************************************************************************
//*****************************************************************************
#endif

HI_END_HEADER
#endif
