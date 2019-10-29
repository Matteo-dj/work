//*****************************************************************************
//
//                  版权所有 (C), 2001-2011, 华为技术有限公司
//
//*****************************************************************************
//  文 件 名   : mrs_dfx_cco.h
//  版 本 号   : V1.0 
//  作    者   : 沈汉坤/00130424
//  生成日期   : 2011-10-25
//  功能描述   : MRS DFX CCO接口 
//  函数列表   : NA
//  修改历史   : 
//  1.日    期 : 2011-10-25
//    作    者 : 沈汉坤/00130424
//    修改内容 : 创建文件 
//*****************************************************************************

#ifndef __MRS_DFX_CCO_H__
#define __MRS_DFX_CCO_H__

#if defined(PRODUCT_CFG_PRODUCT_TYPE_CCO)

HI_START_HEADER


//////////////////////////////////////////////////////////////////////////
// DFX 内部接口
//////////////////////////////////////////////////////////////////////////
typedef struct
{
    MRS_SRV_LINK stLink;
    HI_U8 ucDevId[HI_PLC_MAC_ADDR_LEN];
    HI_U8 ucAbnormal;
    HI_U8 ucPad;
} MRS_ABNORMAL_CLTII_NODE;

// CCO保存载波电表从节点信息节点
typedef struct
{
    MRS_SRV_LINK stLink;                      // 本节点在链表中的链接
    HI_U16 usSeq;                             // 从节点序号
    HI_U8  aucMeterAddr[HI_METER_ADDR_LEN];   // 表地址
    HI_U8  ucProtoType;                       // 规约类型
    HI_U8  aucPad[3];                         // 保留位
} MRS_CARRIER_METER_NODE;

// CCO保存采集器从节点信息节点
typedef struct
{
    MRS_SRV_LINK stLink;                      // 本节点在链表中的链接
    HI_U16 usSeq;                             // 从节点序号
    HI_U8  aucMac[HI_PLC_MAC_ADDR_LEN];       // 采集器模块的入网MAC地址
    HI_U8  aucAsset[HI_METER_ADDR_LEN];       // 采集器的资产编号
    HI_U8  aucID[HI_PLC_MAC_ADDR_LEN];        // 采集器模块的自身MAC地址
    HI_U8  ucNum;                             // 采集器下接485电表的数量
    HI_U8  aucPad[3];                         // 保留位
    HI_U8  aucItem[0];                        // 存放ucNum条下接485电表信息，结构为DIAG_MRS_METER_STRU
} MRS_CLT_NODE;

typedef struct
{
    HI_U8   ucDevType;           // 设备类型
    HI_U8   ucSN;                // 下发查询的SN
    HI_U16  usTotal;             // 本次上报从节点信息的总条数
    HI_U16  usStart;             // 本次采集器/载波电表从节点信息上报在链表中的编号，编号从0开始
    HI_U16  usCount;             // 本次采集器/载波电表从节点信息链表从usStart开始上报的条目数
    HI_U16  usStartIndex;        // 当前数据包的第1条从节点信息的序号
    HI_U16  usId;                // 上报数据包的DIAG命令ID
    HI_U8   ucOption;            // 上报数据包的DIAG命令参数Option(无用)
    HI_U8   aucReserved[3];      // 保留位
} MRS_DIAG_REPORT_PARAMS;


// Hso获取映射表信息
HI_U32 mrsCcoDfxSendMapToHso(HI_U16 usId, HI_U32 ulOption);

// Hso获取表档案信息
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




