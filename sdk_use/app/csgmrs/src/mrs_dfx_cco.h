//*****************************************************************************
//
//                  ��Ȩ���� (C), 2001-2011, ��Ϊ�������޹�˾
//
//*****************************************************************************
//  �� �� ��   : mrs_dfx_cco.h
//  �� �� ��   : V1.0 
//  ��    ��   : ����/00130424
//  ��������   : 2011-10-25
//  ��������   : MRS DFX CCO�ӿ� 
//  �����б�   : NA
//  �޸���ʷ   : 
//  1.��    �� : 2011-10-25
//    ��    �� : ����/00130424
//    �޸����� : �����ļ� 
//*****************************************************************************

#ifndef __MRS_DFX_CCO_H__
#define __MRS_DFX_CCO_H__

#if defined(PRODUCT_CFG_PRODUCT_TYPE_CCO)

HI_START_HEADER


//////////////////////////////////////////////////////////////////////////
// DFX �ڲ��ӿ�
//////////////////////////////////////////////////////////////////////////
typedef struct
{
    MRS_SRV_LINK stLink;
    HI_U8 ucDevId[HI_PLC_MAC_ADDR_LEN];
    HI_U8 ucAbnormal;
    HI_U8 ucPad;
} MRS_ABNORMAL_CLTII_NODE;

// CCO�����ز����ӽڵ���Ϣ�ڵ�
typedef struct
{
    MRS_SRV_LINK stLink;                      // ���ڵ��������е�����
    HI_U16 usSeq;                             // �ӽڵ����
    HI_U8  aucMeterAddr[HI_METER_ADDR_LEN];   // ���ַ
    HI_U8  ucProtoType;                       // ��Լ����
    HI_U8  aucPad[3];                         // ����λ
} MRS_CARRIER_METER_NODE;

// CCO����ɼ����ӽڵ���Ϣ�ڵ�
typedef struct
{
    MRS_SRV_LINK stLink;                      // ���ڵ��������е�����
    HI_U16 usSeq;                             // �ӽڵ����
    HI_U8  aucMac[HI_PLC_MAC_ADDR_LEN];       // �ɼ���ģ�������MAC��ַ
    HI_U8  aucAsset[HI_METER_ADDR_LEN];       // �ɼ������ʲ����
    HI_U8  aucID[HI_PLC_MAC_ADDR_LEN];        // �ɼ���ģ�������MAC��ַ
    HI_U8  ucNum;                             // �ɼ����½�485��������
    HI_U8  aucPad[3];                         // ����λ
    HI_U8  aucItem[0];                        // ���ucNum���½�485�����Ϣ���ṹΪDIAG_MRS_METER_STRU
} MRS_CLT_NODE;

typedef struct
{
    HI_U8   ucDevType;           // �豸����
    HI_U8   ucSN;                // �·���ѯ��SN
    HI_U16  usTotal;             // �����ϱ��ӽڵ���Ϣ��������
    HI_U16  usStart;             // ���βɼ���/�ز����ӽڵ���Ϣ�ϱ��������еı�ţ���Ŵ�0��ʼ
    HI_U16  usCount;             // ���βɼ���/�ز����ӽڵ���Ϣ�����usStart��ʼ�ϱ�����Ŀ��
    HI_U16  usStartIndex;        // ��ǰ���ݰ��ĵ�1���ӽڵ���Ϣ�����
    HI_U16  usId;                // �ϱ����ݰ���DIAG����ID
    HI_U8   ucOption;            // �ϱ����ݰ���DIAG�������Option(����)
    HI_U8   aucReserved[3];      // ����λ
} MRS_DIAG_REPORT_PARAMS;


// Hso��ȡӳ�����Ϣ
HI_U32 mrsCcoDfxSendMapToHso(HI_U16 usId, HI_U32 ulOption);

// Hso��ȡ������Ϣ
HI_U32 mrsCcoDfxSendMeterInfToHso(HI_U32 ulOption);

HI_U32 mrsCcoDfxSendCollectorMapToHso(HI_VOID* pCmdParam, HI_U32 ulOption);

HI_U32 mrsCcoDfxCopyCollectorMapEntry(HI_IN MRS_CLT_NODE* pstClt, HI_OUT HI_U8* pucPayload);

HI_U32 mrsCcoDfxSendCltiiSearchErrorToHso(HI_U16 usId, HI_U32 ulOption);

HI_U32 mrsCcoDfxSendSearchInfoToHso(HI_U16 usId, HI_U32 ulOption);

HI_U32 mrsCcoDfxSendSlaveNodeInfoToHso(HI_U16 usId, HI_VOID* pstParam, HI_U32 ulOption);

HI_VOID mrsCcoDfxReportSearchResult(DIAG_CMD_MRS_GET_CCO_SLAVENODE_INFO_IND_STRU* pstInfo,
                                         MRS_DIAG_REPORT_PARAMS* pstParams);

MRS_SRV_LIST* mrsCcoDfxAbnormalCltiiList(HI_VOID);
MRS_SRV_LIST* mrsCcoDfxSearchResultMeterList(HI_VOID);
MRS_SRV_LIST* mrsCcoDfxSearchResultCltList(HI_VOID);
HI_VOID mrsCcoDfxUpdateAbnormalCltiiNodeList(MRS_PLC_METERLIST_UP_S * pstUpFrame);
HI_VOID mrsCcoDfxUpdateSearchResultList(METER_NODE* pstNode, HI_U16 usSeq);
HI_BOOL mrsCcoDfxCheckDuplicateCltiiNode(HI_U8* pucDevId, HI_U8 ucAbnormal);
HI_VOID mrsCcoDfxClearList(MRS_SRV_LIST* pstListHead);
HI_VOID mrsCcoDfxFreeListNode(HI_VOID *pstNode, HI_VOID *pstCb);

HI_VOID mrsCcoDfxSearchSetDuration(MRS_CCO_SEARCH_DFX_STRU* pstDfx, HI_U16 usSearchDuration);
HI_VOID mrsCcoDfxSearchStartTime(MRS_CCO_SEARCH_DFX_STRU* pstDfx);
HI_VOID mrsCcoDfxSearchEndTime(MRS_CCO_SEARCH_DFX_STRU* pstDfx);
HI_U32 mrsCcoDfxSendTimerSaveFlashToHso(HI_U32 ulOption);

HI_END_HEADER

#endif // PRODUCT_CFG_PRODUCT_TYPE_CCO
#endif // __MRS_DFX_CCO__H__




