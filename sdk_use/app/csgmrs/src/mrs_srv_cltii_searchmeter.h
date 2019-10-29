//*****************************************************************************
//
//                  ��Ȩ���� (C), 1998-2013, ��Ϊ�������޹�˾
//
//*****************************************************************************
//  �� �� ��   : mrs_srv_cltii_searchmeter.h
//  �� �� ��   : V1.0 
//  ��    ��   : cuiate/00233580
//  ��������   : 2014-08-13
//  ��������   : II���ѱ������ӿ�����
//               
//  �����б�   : TODO: ...
//  �޸���ʷ   : 
//  1.��    �� : 2014-08-13
//    ��    �� : cuiate/00233580
//    �޸����� : �����ļ� 
//
//*****************************************************************************

#ifndef _MRS_SRV_CLTII_SEARCHMETER_H_
#define _MRS_SRV_CLTII_SEARCHMETER_H_
HI_START_HEADER
#if defined(PRODUCT_CFG_PRODUCT_TYPE_STA)

#define MRS_SM_POS_H                (4)
#define MRS_SM_POS_A                (1)
#define MRS_SM_POS_DI               (10)

#define MRS_SM_MASK_BYTE_GDW             (0xAA)             // ����07��97��Լ�ѱ�����
#define MRS_SM_ZERO_BYTE                 (0x00)             // �ѱ���λ��0x00��ʼ��λȥ��
#define MRS_BCD_BYTE_DEC_MAX_VALUE       (99)               // �ѱ��ַ���ֽ�ʮ������BCD�뿴��ʮ���Ƶ����ֵ

#define MRS_CLTII_SEARCH_STATE_IDLE                 (0)     // ���ѱ�״̬
#define MRS_CLTII_SEARCH_STATE_TRYFISRT             (1)     // �ϵ糢���ϴ��������ַ
#define MRS_CLTII_SEARCH_STATE_DETECT               (2)     // ̽���ϴ��ѱ���
#define MRS_CLTII_SEARCH_STATE_LOOP                 (3)     // ͨ���ѭ���ѱ�
#define MRS_CLTII_SEARCH_STATE_VERIFY               (4)     // �ѵ���󳭶���֤
#define MRS_CLTII_SEARCH_STATE_EXPLORE              (5)     // ����N�ֽ�
#define MRS_CLTII_SEARCH_STATE_EXPLORE_VERIFY       (6)     // ����N�ֽڹ������ѵ�������֤
#define MRS_CLTII_SEARCH_STATE_DETECT_LIST          (7)     // �ѱ��б�̽��


//*****************************************************************************
// �ӿڶ���
//*****************************************************************************

HI_U32 mrsCltiiSearchStart(MRS_STA_SEARCH_CTX *pstCtx, HI_BOOL bTryFirstFlag);

HI_U32 mrsCltiiSearchStop(MRS_STA_SEARCH_CTX *pstCtx);

HI_VOID mrsCltiiSearchSetState(HI_U8 ucState);

HI_U8 mrsCltiiSearchGetState(HI_VOID);

HI_U32 mrsCltiiSend2Meter(HI_PBYTE pstBuf, HI_U16 usLen); 

HI_U32 mrsCltiiSearchRx(HI_DMS_CHL_RX_S* pstRx);

HI_U32 mrsCltiiSearchByteTimeoutProc(MRS_STA_SEARCH_CTX *pstCtx);

HI_U32 mrsCltiiSearchFrameTimeoutProc(MRS_STA_SEARCH_CTX *pstCtx);

HI_U32 mrsCltiiSearchVMTimeoutProc(HI_VOID);

HI_BOOL mrsCltiiSearchIsNvNodeDI97Valid(HI_U8 ucOption);

HI_U8* mrsCltiiSearchTxAddr(MRS_STA_SEARCH_CTX* pstCtx);

HI_VOID mrsCltiiSearchGetTxAddr(MRS_STA_SEARCH_CTX* pstCtx, HI_OUT HI_U8* pucAddr);

HI_U8 mrsCltiiSearchTxProtocol(MRS_STA_SEARCH_CTX *pstCtx);

HI_U32 mrsCltiiSearchTxDI(MRS_STA_SEARCH_CTX *pstCtx);

HI_U8 mrsCltiiSearchGetAddrDepth(HI_U8* pucAddr, HI_U8 ucProtocol);



#if defined(PRODUCT_CFG_PRODUCT_TYPE_CSG_GD)

HI_U32 mrsCsgCltIISendDetectList(APP_CSG_CMD_DL_METERLIST_STRU *pstMlist);

#endif

//*****************************************************************************
//*****************************************************************************


#endif //defined(PRODUCT_CFG_PRODUCT_TYPE_STA)
HI_END_HEADER
#endif //_MRS_SRV_CLTII_SEARCHMETER_H_

