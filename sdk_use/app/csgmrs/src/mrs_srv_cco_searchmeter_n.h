//*****************************************************************************
//
//                  ��Ȩ���� (C), 1998-2011, ��Ϊ�������޹�˾
//
//*****************************************************************************
//  �� �� ��   : mrs_srv_cco_searchmeter_n.h
//  �� �� ��   : V1.0
//  ��    ��   :
//  ��������   :
//  ��������   : CCO�ѱ�ģ���ڲ��������ӿڶ���
//
//  �����б�   : TODO: ...
//  �޸���ʷ   :
//  1.��    �� : 2015-11-23
//    ��    �� : cuiate/c00233580
//    �޸����� : �����ļ�
//
//*****************************************************************************

#ifndef __MRS_SRV_CCO_SEARCHMETER_N__H__
#define __MRS_SRV_CCO_SEARCHMETER_N__H__
HI_START_HEADER

#if defined(PRODUCT_CFG_PRODUCT_TYPE_CCO)
//*****************************************************************************
//*****************************************************************************

#define MRS_SM_STABLE_TIME_RESET(cco)       do {(cco)->stAutoSearchModel.usStableTimes = 0;} while(0)
#define MRS_SM_STABLE_TIME_INCREASE(cco)    do {(cco)->stAutoSearchModel.usStableTimes++;} while(0)


// �ӽڵ�����(1���ֽ�)
// �ӽڵ�ͨ�ŵ�ַ(6���ֽ�)
// ͨ��Э��(1���ֽ�)
// ���(2���ֽ�)
// �豸����(1���ֽ�)
// �½Ӵӽڵ�����(1���ֽ�)
// �����ϱ����ڵ�����(1���ֽ�)
#define SIZEOF_REPORT_FRAME_STRU    (1+6+1+2+1+1+1)


typedef struct
{
    HI_U8   aucMac[HI_PLC_MAC_ADDR_LEN];
    HI_BOOL bExist;
    HI_U8   padding;
} MRS_CCO_SEARCH_QUEUE_VISIT_STRU;

#if defined(PRODUCT_CFG_PRODUCT_TYPE_CSG_GD)
HI_PRV HI_U32 mrsCsgSearchAssembleReportFrame(MRS_CCO_SRV_CTX_STRU * pstCco, METER_NODE* pstNode);
#endif
#if !defined(PRODUCT_CFG_PRODUCT_TYPE_CSG_GD) || defined(PRODUCT_CFG_FEATURE_UT)
HI_PRV HI_U32 mrsCcoSearchAssembleReportFrame(MRS_CCO_SRV_CTX_STRU* pstCco, METER_NODE* pstNode);
#endif

HI_PRV HI_U32 mrsSearchAssembleReportFrame(MRS_CCO_SRV_CTX_STRU * pstCco, METER_NODE* pstNode);

HI_PRV HI_VOID mrsCcoSearchReportFrameEnQueue(HI_VOID);

HI_PRV HI_U32 Add2ReportedList(MRS_CCO_SRV_CTX_STRU * cco, MRS_CMD_METERLIST_ITEM * item);

HI_PRV HI_U32 mrsCcoSearchMeterReset(MRS_CCO_SRV_CTX_STRU * cco);

#if !defined(PRODUCT_CFG_PRODUCT_TYPE_CSG_GD)
HI_PRV HI_U32 mrsCcoCreateStartSearchMeterPlcFrame(MRS_CCO_SRV_CTX_STRU * cco);
#endif

HI_PRV HI_U32 mrsCcoCreateStopSearchMeterPlcFrame(MRS_CCO_SRV_CTX_STRU * cco);

HI_PRV HI_U32 mrsCcoSearchMeterMacListInit(MRS_CCO_SRV_CTX_STRU *cco, HI_MAC_NETWORK_TOPO_S * topo);

HI_PRV HI_VOID mrsCcoSearchMeterMacListRefresh(MRS_CCO_SRV_CTX_STRU *cco, HI_MAC_NETWORK_TOPO_S * topo);

HI_PRV HI_U32 mrsCcoSearchMeterGetTopo(MRS_CCO_SRV_CTX_STRU * cco, HI_MAC_NETWORK_TOPO_S ** topo);

HI_PRV HI_U32 mrsCcoSearchMeterSaveTopo(HI_MAC_NETWORK_TOPO_S * saved_topo, HI_MAC_NETWORK_TOPO_S * new_topo);

HI_PRV HI_U32 mrsCcoCheckTopoChanged(HI_MAC_NETWORK_TOPO_S * old_topo, HI_MAC_NETWORK_TOPO_S * new_topo, HI_BOOL * changed);

HI_PRV HI_U32 mrsCcoCheckSearchFinished(MRS_CCO_SRV_CTX_STRU * cco);

HI_PRV HI_U32 mrsCcoUpdateSearchQueryTimes(MRS_CCO_SRV_CTX_STRU * cco, HI_U8 * mac);

HI_PRV HI_U32 mrsCcoGetMeterList(MRS_CCO_SRV_CTX_STRU  * cco, MAC_ITEM * item);

HI_PRV HI_VOID mrsCcoQueueFree(HI_VOID * p);

// �������վ���ѱ����Ƿ��ѯ���
HI_PRV HI_U32 mrsCcoCheckQueryComplete(MRS_CCO_SRV_CTX_STRU * cco);

HI_PRV HI_VOID mrsCcoSearchReportSlaveNodeInfoNormal(HI_VOID);

HI_PRV HI_VOID mrsCcoSearchReportSlaveNodeInfoTransformer(HI_VOID);

HI_PRV HI_U32 mrsStopSearchMeterTransformer(HI_VOID);

HI_PRV HI_U16 mrsCcoSearchMeterMacListDelete(MRS_SEARCH_METER_MAC_LIST *pMacList, HI_MAC_NETWORK_TOPO_S *pTopo);

HI_PRV HI_VOID mrsCcoSearchMeterMacListRealign(MRS_SEARCH_METER_MAC_LIST *pMacList);

HI_PRV HI_VOID mrsCcoSearchMeterMacListAdd(MRS_SEARCH_METER_MAC_LIST *pMacList, HI_MAC_NETWORK_TOPO_S *pTopo);

HI_PRV HI_U32 mrsCcoSearchFastGetMeterList(MRS_CCO_SRV_CTX_STRU *pCcoCtx, HI_BOOL bRepeatCheck);

HI_PRV HI_VOID mrsCcoSearchFastMeterEnQueue(MRS_CCO_SRV_CTX_STRU *pCcoCtx, HI_U8 *pMacAddr);

HI_PRV HI_BOOL mrsCcoSearchMeterIsValidProduct(HI_U8 product_type);

HI_PRV HI_U32 mrsGetCltNodeType(HI_U8 *pucMacAddr, HI_U8 *pucType);

HI_PRV HI_BOOL mrsCcoSearchVisitQueue(HI_VOID *p, HI_VOID *q);

HI_PRV HI_BOOL mrsCcoSearchVisitCltList(HI_VOID *p, HI_VOID *q);

HI_PRV HI_BOOL mrsCcoSearchVisitMeterList(HI_VOID *p, HI_VOID *q);

HI_PRV HI_U32 mrsCcoRecoverWhiteSwitch(MRS_CCO_SRV_CTX_STRU* pstCcoCtx);

//*****************************************************************************
//*****************************************************************************
#endif // defined(PRODUCT_CFG_PRODUCT_TYPE_CCO)

HI_END_HEADER
#endif //__MRS_SRV_CCO_SEARCHMETER_N__H__

