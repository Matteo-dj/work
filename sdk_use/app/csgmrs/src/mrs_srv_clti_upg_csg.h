//*****************************************************************************
//
//                  ��Ȩ���� (C), 2001-2014, ��Ϊ�������޹�˾
//
//*****************************************************************************
//  �� �� ��   : mrs_srv_clti_searchmeter_csg.h
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

#ifndef _MRS_SRV_CLTI_UPG_CSG_H_
#define _MRS_SRV_CLTI_UPG_CSG_H_

#include "mrs_srv_sta_queue.h"

HI_START_HEADER

#if (defined(PRODUCT_CFG_PRODUCT_TYPE_STA) && defined(PRODUCT_CFG_PRODUCT_TYPE_CSG_GD))
//*****************************************************************************
//*****************************************************************************

#define MRS_CSG_CLTI_UPG_MAXTRY         (3)
#define MRS_CSG_CLTI_UPG_TIMEOUT        (100)   // ��λ: 100ms

#define MRS_CLTI_SEND_BYTE_TIME_9600    (5)     // ����ÿ���ֽڵ���ʱ
#define MRS_CLTI_UPG_FIX_SEG_LEN        (180)   // (PRODUCT_CFG_FLASH_BLOCK_SIZE)

#define MRS_CSG_UPG_FIX_LEN_START       (15)
#define MRS_CSG_UPG_FIX_LEN_TRANSIMT    (6)

typedef enum 
{
    MRS_CSG_CLTI_UPG_IDLE,
    MRS_CSG_CLTI_UPG_QRY_INFOS,     // �ļ���Ϣ�鿴
    MRS_CSG_CLTI_UPG_START,         // �ļ���ʼ
    MRS_CSG_CLTI_UPG_TRANSIMIT,     // �ļ�����
    MRS_CSG_CLTI_UPG_QRY_PROCESS,   // �ļ����Ȳ�ѯ
} MRS_CSG_CLTI_UPG_FILE_PROCESS_E;


typedef struct
{
    HI_U8 ucFileId;
    HI_U8 ucReserved;
    HI_U8 aucDestAddr[HI_METER_ADDR_LEN];

    HI_U32 ulFileLength;

    HI_U16 usTotalSeg;
    HI_U16 usTotalCs;
} MRS_CSG_UPG_ITEM_STRU;


typedef struct
{
    HI_U16 usCurrNo;    // ��ǰ�κ�
    HI_U16 usLen;       // �γ���
    HI_U16 usCs;
    HI_U16 usreserved;
} MRS_CSG_UPG_SEG_STRU; // ��������Ϣ


typedef struct  
{
    HI_U8 ucTry;
    HI_U8 ucMaxTry;
    HI_U8 ucStatus;
    HI_U8 ucProcess;    // ��������
    
    HI_U16 usLastLen;
    HI_U16 usFrameLength;
    HI_U8 *pucFrame;

    HI_U16 usCrcValue;
    HI_U16 usReserved;
    
    HI_U32 ulUpgId;
    HI_U32 ulFileLen;
    HI_PBYTE pucCache;
    
    MRS_CSG_UPG_ITEM_STRU  stItem;
    MRS_CSG_UPG_SEG_STRU stCurSeg;
} MRS_CSG_CLTI_UPG_MODULE_STRU;



MRS_CSG_CLTI_UPG_MODULE_STRU *mrsCsgCltIUpgGetCtx(HI_VOID);

HI_U32 mrsCsgCltIUpgStatusMsg(HI_MDM_DEVICE_UPG_STATUS_REQ_S *pstReq);
HI_VOID mrsCsgCltiUpgRxFileProcess(HI_U8 ucResult, HI_U8 ucFileId);

HI_U32 mrsCsgCltIUpgStatusInd(HI_VOID);
HI_VOID mrsCsgCltIUpgFileStatusProc(HI_U8 ucResult);
HI_VOID mrsCsgCltIUpgSegInit(HI_VOID);
HI_U32 mrsCsgCltIUpgTimeout(HI_VOID);
HI_U32 mrsCsgCltIUpgCalcSeg(HI_VOID);
HI_U32 mrsCsgCltIUpgQryProc(HI_VOID);
HI_U32 mrsCsgCltIUpgQryInfosProc(HI_VOID);
HI_U32 mrsCsgCltIUpgTx(HI_U8 ucStatus, HI_U32 (*pfnCreateFrame)(HI_VOID *, HI_U8 **, HI_U16 *), HI_VOID *pParam);
HI_U32 mrsCsgCltICreateFileProcessFrame(HI_VOID *pParam, HI_U8 **ppFrame, HI_U16 *pusLength);
HI_U32 mrsCsgCltIUpgCreateQryFrameNoData(HI_U8 ucAfn, HI_U32 ulDi, HI_U8 **ppFrame, HI_U16 *pusLength);
HI_U32 mrsCsgCltIUpgCreatTransmitFrame(HI_VOID *pParam, HI_U8 **ppFrame, HI_U16 *pusLength);
HI_U32 mrsCsgCltIUpgCreatStartFrame(HI_VOID *pParam, HI_U8 **ppFrame, HI_U16 *pusLength);
HI_U8 mrsCsgCltIUpgGetProcess(HI_VOID);
MRS_CSG_UPG_ITEM_STRU *mrsCsgCltIUpgGetItem(HI_VOID);
HI_U32 mrsCsgCltIUpgGetTransimit(HI_U8 **ppBuffer, HI_U16 *pusLength);
HI_U32 mrsCsgCltICreateFileInfosFrame(HI_VOID *pParam, HI_U8 **ppFrame, HI_U16 *pusLength);
HI_U32 mrsCsgCltIUpgTransmitProc(HI_VOID);
HI_U32 mrsCsgCltIUpgGetStart(HI_U8 **ppBuffer, HI_U16 *pusLength);

HI_U32 mrsCsgCltIUpgJudgeRestart(MRS_CSG_UPG_ITEM_STRU *pstSrcItem, HI_U16 usCurSeg);

HI_U32 mrsCsgCltIUpgInit(HI_VOID);
HI_U32 mrsCsgCltIUpgSetNV(HI_VOID);
HI_VOID mrsCsgCltIUpgEndProc(HI_U8 ucStatus);
HI_VOID mrsGetCltIUpgParam(NV_APP_MR_CFG_STRU* pstcfg);

HI_PUBLIC HI_U32 mrsCsgCltIUpgDeinit(HI_VOID);
HI_PUBLIC HI_VOID mrsCsgCltIUpgGetCurSeg(HI_VOID);
HI_PUBLIC HI_VOID mrsCsgCltIUpgSetProcess(HI_U8 ucProcess);
HI_PUBLIC HI_VOID mrsCsgCltIUpgSetStatus(HI_U8 ucStatus);
HI_PUBLIC HI_U8 mrsCsgCltIUpgGetStatus(HI_VOID);
HI_PUBLIC HI_U32 mrsCsgCltIUpgFrameSend(MRS_STA_ITEM * pstItem);

HI_PUBLIC HI_U32 mrsCsgCltIUpgMsgProc(HI_SYS_QUEUE_MSG_S *pstMsg);

//*****************************************************************************
//*****************************************************************************
#endif

HI_END_HEADER
#endif
