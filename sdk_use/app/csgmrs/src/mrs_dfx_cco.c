//*****************************************************************************
//
//                  ��Ȩ���� (C), 1998-2012, ��Ϊ�������޹�˾
//
//*****************************************************************************
//  �� �� ��   : mrs_dfx_cco.c
//  �� �� ��   : V1.0 
//  ��    ��   : shenhankun/00130424
//  ��������   : 2012-01-20
//  ��������   : MRS CCO ��DFX����
//               
//  �����б�   : TODO: ...
//  �޸���ʷ   : 
//  1.��    �� : 2012-01-20
//    ��    �� : shenhankun/00130424
//    �޸����� : �����ļ� 
//
//*****************************************************************************

//*****************************************************************************
// PROJECT   : 
// SUBSYSTEM :
// MODULE    :  
// OWNER     :  
//*****************************************************************************

#include "mrs_common.h"
#include "mrs_cmd_msg.h"
#include "mrs_fw_n.h"
#include "mrs_fw_proto376_2_echo.h"
#include "mrs_fw_tools.h"
#include "mrs_srv_cco_macmap.h"
#include "mrs_srv_list.h"
#include "mrs_srv_cco.h"
#include "mrs_dfx_cco.h"
#include "mrs_srv_cco_archives.h"

#if defined(PRODUCT_CFG_PRODUCT_TYPE_CCO)

// TODO: [��ע��]�˴�����Ӧ�ø���MACͨ����ʵ���������㣬���ʹ����MAC�ṩ�Ľӿ�
// TODO: ĿǰMAC���޽ӿڣ���MAC�ṩ����Ӧ�ӿڣ�Ӧ���滻�ú�
#define PB520_PAYLOAD_MAX_SIZE          (384)

HI_PRV HI_BOOL mrsCcoDfxGetMapInf(MRS_MAC_DEVICE_MAP *pstMap, MRS_DFX_CCO_NODE_MAP_INF_STRU * pstNodeMapInf, HI_U16 *usCurrIndex);

//*****************************************************************************
// ��������: mrsCcoDfxGetMapInf
// ��������: ��ȡ��ַӳ�����Ϣ(ÿ�����10��)
//           
// ����˵��:
//   pstNodeMapInf [out] ӳ�����Ϣ
//   usCurrIndex [in/out] ��ǰ��ȡӳ�����Ϣ������λ��
//
// �� �� ֵ: 
//    HI_TRUE ����û�����ӳ�����Ϣ
//    HI_FALSE ���е�ӳ�����Ϣ�ѻ�ȡ���
// ����Ҫ��: ����ʱ�жϷ���ֵ,���Ϊ��Ļ����������,ֱ������Ϊ��
// ��    ��: liuxipeng/KF54842 [2012-02-16]
// ��    ��: cuiate/00233580   [2015-11-23]
//*****************************************************************************
/* BEGIN: PN: DTS2015111809444 MODIFY\ADD\DEL by cuiate/00233580 at 2015/11/23 */
HI_BOOL mrsCcoDfxGetMapInf(MRS_MAC_DEVICE_MAP *pstMap, MRS_DFX_CCO_NODE_MAP_INF_STRU * pstNodeMapInf, HI_U16 *usCurrIndex)
{
    HI_U8 MapNum = 0;
    HI_U16 index = 0;
    HI_U16 i     = 0;
    MRS_MAC_DEVICE_MAP * map = pstMap;
    MRS_MAC_DEVICE_PAIR * item = HI_NULL;
    
    (hi_void)memset_s(pstNodeMapInf, sizeof(MRS_DFX_CCO_NODE_MAP_INF_STRU), 0, sizeof(MRS_DFX_CCO_NODE_MAP_INF_STRU));
    pstNodeMapInf->TotalNum = map->usNumber;    
    index = *usCurrIndex;
    item = map->pstItem;
    
    for (i = index; i < pstMap->usMapSize; i++)
    {
        if (item[i].bValid)
        {
            if (MapNum >= MRS_DFX_MAC_MAP_MAX_PACK_NUM)
            {
                *usCurrIndex           = i;
                pstNodeMapInf->CurrNum = MapNum;
                return HI_TRUE;
            }

            pstNodeMapInf->stNodeMap[MapNum].Index = i;
            (hi_void)memcpy_s(pstNodeMapInf->stNodeMap[MapNum].NodeMac, HI_PLC_MAC_ADDR_LEN, item[i].aucMacAddr, HI_PLC_MAC_ADDR_LEN);
            (hi_void)memcpy_s(pstNodeMapInf->stNodeMap[MapNum].NodeMeter, HI_PLC_MAC_ADDR_LEN, item[i].stDeviceInf.aucAddr, HI_METER_ADDR_LEN);

            pstNodeMapInf->stNodeMap[MapNum].DevType = item[i].stDeviceInf.ucType;
            pstNodeMapInf->stNodeMap[MapNum].usSendFrame = 0;
            pstNodeMapInf->stNodeMap[MapNum].usRecvFrame = 0;
 
            MapNum++;
         }
    }
    
    *usCurrIndex           = i;
    pstNodeMapInf->CurrNum = MapNum;
    
    return HI_FALSE;
}
/* END:   PN: DTS2015111809444 MODIFY\ADD\DEL by cuiate/00233580 at 2015/11/23 */


HI_U32 mrsCcoDfxSendMapToHso(HI_U16 usId, HI_U32 ulOption)
{
    MRS_CCO_SRV_CTX_STRU* pstCcoCtx = mrsCcoGetContext();
    MRS_MAC_DEVICE_MAP *pstMap = HI_NULL;
    MRS_DFX_CCO_NODE_MAP_INF_STRU *pstNodeMap = HI_NULL;
    HI_BOOL Flg                       = 0;
    HI_U16 Index                      = 0;
    HI_U32 ret = HI_ERR_SUCCESS;

    pstNodeMap = (MRS_DFX_CCO_NODE_MAP_INF_STRU *)mrsToolsMalloc(sizeof(MRS_DFX_CCO_NODE_MAP_INF_STRU));
    if (!pstNodeMap)
    {
        return HI_ERR_FAILURE;
    }

    if (usId == ID_DIAG_CMD_MRS_CCO_GET_MACMAP)
    {
        pstMap = &pstCcoCtx->stMapModule.stMacMeterMap;
    }
    else
    {
        pstMap = &pstCcoCtx->stMapModule.stMacCltMap;
    }
    
    do
    {
        (hi_void)memset_s(pstNodeMap, sizeof(MRS_DFX_CCO_NODE_MAP_INF_STRU), 0, sizeof(MRS_DFX_CCO_NODE_MAP_INF_STRU));
        
        Flg = mrsCcoDfxGetMapInf(pstMap, pstNodeMap, &Index);

        ret = HI_MDM_ReportPacket(usId,
                            (HI_U8)ulOption,
                            (HI_PBYTE)pstNodeMap,
                            sizeof(MRS_DFX_CCO_NODE_MAP_INF_STRU), HI_FALSE);
    } while ((Flg)
            && (Index < pstMap->usMapSize)
            && (ret == HI_ERR_SUCCESS));
    
    mrsToolsFree(pstNodeMap);
    
//    return HI_ERR_CONSUMED; // ���� HI_DIAG_SendAckPacket ����� ���� HI_ERR_CONSUMED
    return HI_ERR_SUCCESS;
}


//*****************************************************************************
// ��������: mrsCcoDfxGetMeterInf
// ��������: ��ȡ������Ϣ(ÿ�����MRS_DFX_METER_MAX_PACK_NUM��)
//           
// ����˵��:
//   pstMeterInf [out] ������Ϣ
//   usCurrIndex [in/out] ��ǰ��ȡ������Ϣ������λ��
//
// �� �� ֵ: 
//    HI_TRUE ����û����ı�����Ϣ
//    HI_FALSE ���еı����ѻ�ȡ���
// ����Ҫ��: ����ʱ�жϷ���ֵ,���Ϊ��Ļ����������,ֱ������Ϊ��
// ��    ��: liuxipeng/KF54842 [2012-11-14]
//*****************************************************************************
HI_PRV HI_BOOL mrsCcoDfxGetMeterInf(MRS_DFX_CCO_METERS_STRU * pstMeterInf, HI_U16 *usCurrIndex)
{
    HI_U8 ucNum = 0;
    HI_U16 i = 0;
    MRS_ARCHIVES_NODE_STRU * pstNode = HI_NULL;

    (hi_void)memset_s(pstMeterInf, sizeof(MRS_DFX_CCO_METERS_STRU), 0, sizeof(MRS_DFX_CCO_METERS_STRU));
    pstMeterInf->TotalNum = mrsSrvArchivesMeterNum();
    
    for (i = *usCurrIndex; i < pstMeterInf->TotalNum; i++)
    {
        pstNode = mrsSrvArchivesQuery(i);
        if ((HI_NULL != pstNode) && pstNode->bValid)
        {
/* BEGIN: PN: DTS2015061700719 MODIFY\ADD\DEL by cuiate/00233580 at 2015/6/17 */
            if (ucNum >= MRS_DFX_METER_MAX_PACK_NUM)
/* END:   PN: DTS2015061700719 MODIFY\ADD\DEL by cuiate/00233580 at 2015/6/17 */
            {
                *usCurrIndex = i;
                pstMeterInf->CurrNum = ucNum;
                return HI_TRUE;
            }
            
            pstMeterInf->stNodeMap[ucNum].Index = i;
            pstMeterInf->stNodeMap[ucNum].usType = pstNode->ucProtocol;
            pstMeterInf->stNodeMap[ucNum].usFlg = pstNode->ucRead;
            (hi_void)memcpy_s(pstMeterInf->stNodeMap[ucNum].NodeMeter, HI_METER_ADDR_LEN, pstNode->aucNodeAddr, HI_METER_ADDR_LEN);
            
            ucNum++;
        }
    }
    
    *usCurrIndex = i;
    pstMeterInf->CurrNum = ucNum;
    
    return HI_FALSE;
}

HI_U32 mrsCcoDfxSendMeterInfToHso(HI_U32 ulOption)
{
    MRS_DFX_CCO_METERS_STRU *pstMeterInf = HI_NULL;
    HI_BOOL Flg                       = 0;
    HI_U16 Index                      = 0;
    HI_U32 ret = HI_ERR_SUCCESS;
    
    pstMeterInf = (MRS_DFX_CCO_METERS_STRU *)mrsToolsMalloc(sizeof(MRS_DFX_CCO_METERS_STRU));
    if (!pstMeterInf)
    {
        return HI_ERR_FAILURE;
    }

    do
    {
        (hi_void)memset_s(pstMeterInf,sizeof(MRS_DFX_CCO_METERS_STRU), 0,sizeof(MRS_DFX_CCO_METERS_STRU));
        
        Flg = mrsCcoDfxGetMeterInf(pstMeterInf,&Index);

        ret = HI_MDM_ReportPacket(ID_DIAG_CMD_MRS_CCO_GET_METER_INF,
                            (HI_U8)ulOption,
                            (HI_PBYTE)pstMeterInf,
                            sizeof(MRS_DFX_CCO_METERS_STRU), HI_FALSE);

    } while ((Flg)
            && (Index < PRODUCT_CFG_MRS_MAX_NODE_NUM)
            && (ret == HI_ERR_SUCCESS));

    mrsToolsFree(pstMeterInf);
    
//    return HI_ERR_CONSUMED; // ���� HI_DIAG_SendAckPacket ����� ���� HI_ERR_CONSUMED
    return HI_ERR_SUCCESS;
}


HI_U32 mrsCcoDfxSendCollectorMapToHso(HI_VOID* pCmdParam, HI_U32 ulOption)
{
    DIAG_CMD_MRS_CCO_COLLECTOR_MAP_REQ_STRU* pstReq = HI_NULL;
    DIAG_CMD_MRS_CCO_COLLECTOR_MAP_IND_STRU* pstPacket = HI_NULL;
    MRS_CCO_SRV_CTX_STRU* pstCcoCtx = mrsCcoGetContext();
    MRS_CCO_SEARCH_DFX_STRU* pstDfx = HI_NULL;
    MRS_SRV_LIST* pstHead = HI_NULL;
    MRS_SRV_LIST* pstLink = HI_NULL;
    MRS_CLT_NODE* pstClt = HI_NULL;
    HI_U16 usMapEntrySize = 0;
    HI_U16 usMapEntryNum = 0;
    HI_BOOL bReportPacket = HI_FALSE;
    HI_U16 usPayloadLen = 0;
    HI_U16 usPayloadLenMax = PB520_PAYLOAD_MAX_SIZE - sizeof(DIAG_CMD_MRS_CCO_COLLECTOR_MAP_IND_STRU);

    pstHead = mrsCcoDfxSearchResultCltList();
    if (HI_NULL == pstHead)
    {
        return HI_ERR_FAILURE;
    }

    pstDfx = &(pstCcoCtx->stAutoSearchModel.stSearchDfx);
    usMapEntryNum = pstDfx->usCltEntryNum;
    pstLink = pstHead->next;
    
    pstPacket = mrsToolsMalloc(PB520_PAYLOAD_MAX_SIZE);
    if (HI_NULL == pstPacket)
    {
        return HI_ERR_NOT_ENOUGH_MEMORY;
    }    
    (hi_void)memset_s(pstPacket, PB520_PAYLOAD_MAX_SIZE, 0, PB520_PAYLOAD_MAX_SIZE);

    do
    {
        pstReq = (DIAG_CMD_MRS_CCO_COLLECTOR_MAP_REQ_STRU*)pCmdParam;
        if (HI_NULL == pstReq)
        {
            bReportPacket = HI_TRUE;
            break;
        }
        pstPacket->sn = pstReq->sn;

        // �ɼ����ѱ�������Ϊ��
        if (0 == usMapEntryNum)
        {
            bReportPacket = HI_TRUE;
            break;
        }

        // ��ѯ�����ɼ���վ���ӳ���ϵ�������ϱ�
        if (!mrsToolsBroadCastMac(pstReq->mac))
        {
            while (pstLink != pstHead)
            {
                pstClt = (MRS_CLT_NODE*)pstLink;
                if (!mrsToolsMemEq(pstClt->aucMac, pstReq->mac, HI_PLC_MAC_ADDR_LEN))
                {
                    pstLink = pstLink->next;
                    continue;
                }
                
                usMapEntrySize = sizeof(MRS_DFX_CCO_COLLECTOR_MAP_ITEM_STRU) + pstClt->ucNum * HI_METER_ADDR_LEN;
                if (usMapEntrySize <= usPayloadLenMax)
                {
                    mrsCcoDfxCopyCollectorMapEntry(pstClt, pstPacket->payload);
                    usPayloadLen += usMapEntrySize;
                    pstPacket->num++;
                    pstPacket->total++;
                }
                
                break;
            }

            bReportPacket = HI_TRUE;
            break;
        }

        // ��ѯ���вɼ���վ���ӳ���ϵ�������ϱ�
        while (pstLink != pstHead)
        {
            if (0 == usPayloadLen)
            {
                (hi_void)memset_s(pstPacket, PB520_PAYLOAD_MAX_SIZE, 0, PB520_PAYLOAD_MAX_SIZE);
                pstPacket->sn = pstReq->sn;
                pstPacket->total = usMapEntryNum;
            }

            pstClt = (MRS_CLT_NODE*)pstLink;
            usMapEntrySize = sizeof(MRS_DFX_CCO_COLLECTOR_MAP_ITEM_STRU) + pstClt->ucNum * HI_METER_ADDR_LEN;
            if (usPayloadLen + usMapEntrySize > usPayloadLenMax)
            {
                if (0 == usPayloadLen)
                {
                    break;
                }
                
                HI_MDM_ReportPacket(ID_DIAG_CMD_MRS_CCO_GET_COLLECTOR_MAP, 
                                    (HI_U8)ulOption, 
                                    (HI_PBYTE)pstPacket, 
                                    sizeof(DIAG_CMD_MRS_CCO_COLLECTOR_MAP_IND_STRU) + usPayloadLen, 
                                    HI_FALSE);
                usPayloadLen = 0;
            }
            else
            {
                mrsCcoDfxCopyCollectorMapEntry(pstClt, pstPacket->payload + usPayloadLen);
                usPayloadLen += usMapEntrySize;
                pstPacket->num++;

                pstLink = pstLink->next;
            }
        }

        if (usPayloadLen > 0)
        {
            bReportPacket = HI_TRUE;
        }        
    } while(0);

    if (bReportPacket)
    {
        HI_MDM_ReportPacket(ID_DIAG_CMD_MRS_CCO_GET_COLLECTOR_MAP, 
                            (HI_U8)ulOption, 
                            (HI_PBYTE)pstPacket, 
                            sizeof(DIAG_CMD_MRS_CCO_COLLECTOR_MAP_IND_STRU) + usPayloadLen, 
                            HI_FALSE);
    }
    
    mrsToolsFree(pstPacket);
    return HI_ERR_SUCCESS;
}


HI_U32 mrsCcoDfxCopyCollectorMapEntry(HI_IN MRS_CLT_NODE* pstClt, HI_OUT HI_U8* pucPayload)
{
    MRS_DFX_CCO_COLLECTOR_MAP_ITEM_STRU* pstMapEntry = HI_NULL;
    DIAG_MRS_METER_STRU* pstSrc = HI_NULL;
    HI_U8* pucDestAddr = HI_NULL;
    HI_U8 ucIndex = 0;
    HI_U8 bReportAA = mrsCcoGetContext()->stAutoSearchModel.stSearchCfg.report_AA_2_master;

    if (HI_NULL == pstClt || HI_NULL == pucPayload)
    {
        return HI_ERR_FAILURE;
    }

    pstMapEntry = (MRS_DFX_CCO_COLLECTOR_MAP_ITEM_STRU*)pucPayload;
    pstMapEntry->meter_num = pstClt->ucNum;
    (hi_void)memcpy_s(pstMapEntry->mac, HI_PLC_MAC_ADDR_LEN, pstClt->aucMac, HI_PLC_MAC_ADDR_LEN);
    (hi_void)memcpy_s(pstMapEntry->asset, HI_METER_ADDR_LEN, pstClt->aucAsset, HI_METER_ADDR_LEN);
    (hi_void)memcpy_s(pstMapEntry->id, HI_PLC_MAC_ADDR_LEN, pstClt->aucID, HI_PLC_MAC_ADDR_LEN);

    pucDestAddr = pstMapEntry->payload;
    pstSrc = (DIAG_MRS_METER_STRU*)pstClt->aucItem;
    for (ucIndex = 0; ucIndex < pstClt->ucNum; ucIndex++)
    {
        if (bReportAA)
        {
            (hi_void)memcpy_s(pucDestAddr, HI_METER_ADDR_LEN, pstSrc->aucMeterAddr, HI_METER_ADDR_LEN);            
        }
        else
        {
            mrsMeterAddrPreProc(pucDestAddr, pstSrc->aucMeterAddr, pstSrc->ucProtoType);
        }

        pucDestAddr += HI_METER_ADDR_LEN;
        pstSrc++;
    }

    return HI_ERR_SUCCESS;
}

//*****************************************************************************
// ��������: mrsCcoDfxSendCltiiSearchErrorToHso
// ��������: �ϱ�CCO�ѱ�����쳣��II�ɽڵ���Ϣ���쳣��Ϣ
//           
// ����˵��:
//   usId [in] ��ѯID
//   ulOption [in] ��ѯѡ��
//
// �� �� ֵ: 
//    HI_ERR_NOT_ENOUGH_MEMORY ���붯̬�ڴ�ʧ��
//    HI_ERR_SUCCESS �ϱ��ɹ�
// ����Ҫ��: 
// ��    ��: liupeidong/l00238134 [2015-01-20]
//*****************************************************************************
HI_U32 mrsCcoDfxSendCltiiSearchErrorToHso(HI_U16 usId, HI_U32 ulOption)
{
    DIAG_CMD_MRS_GET_CLT_SEARCH_ABNORMAL_IND_STRU* pstAbnormal = HI_NULL;
    HI_U16 usTotal = 0;
    HI_U16 usReport = 0;
    HI_U16 usIndex = 0;
    MRS_ABNORMAL_CLTII_NODE* pstCltii = HI_NULL;
    MRS_SRV_LIST* pstHead = HI_NULL;
    MRS_SRV_LINK* pstNode = HI_NULL;

    pstHead = mrsCcoDfxAbnormalCltiiList();
    if (HI_NULL == pstHead)
    {
        return HI_ERR_FAILURE;
    }
    pstNode = pstHead->next;

    pstAbnormal = mrsToolsMalloc(sizeof(DIAG_CMD_MRS_GET_CLT_SEARCH_ABNORMAL_IND_STRU));
    if (HI_NULL == pstAbnormal)
    {
        return HI_ERR_NOT_ENOUGH_MEMORY;
    }
    
    usTotal = (HI_U16)mrsSrvListLength(pstHead);
    if (usTotal == 0)
    {
        (hi_void)memset_s(pstAbnormal, sizeof(DIAG_CMD_MRS_GET_CLT_SEARCH_ABNORMAL_IND_STRU), 0, sizeof(DIAG_CMD_MRS_GET_CLT_SEARCH_ABNORMAL_IND_STRU));         
        HI_MDM_ReportPacket(usId, (HI_U8)ulOption, (HI_PBYTE)pstAbnormal, sizeof(DIAG_CMD_MRS_GET_CLT_SEARCH_ABNORMAL_IND_STRU), HI_TRUE);
        
        mrsToolsFree(pstAbnormal);
        return HI_ERR_SUCCESS;
    }

    while (usReport < usTotal)
    {
        (hi_void)memset_s(pstAbnormal, sizeof(DIAG_CMD_MRS_GET_CLT_SEARCH_ABNORMAL_IND_STRU), 0, sizeof(DIAG_CMD_MRS_GET_CLT_SEARCH_ABNORMAL_IND_STRU));        
        pstAbnormal->usTotal = usTotal;
        pstAbnormal->usStartIndex = usReport;
        
        if (usTotal - usReport > DIAG_CMD_SEARCH_ABNORMAL_CNT)
        {
            pstAbnormal->usCurrent = DIAG_CMD_SEARCH_ABNORMAL_CNT;
        }
        else
        {
            pstAbnormal->usCurrent = usTotal - usReport;
        }

        for (usIndex = 0; usIndex < pstAbnormal->usCurrent; usIndex++)
        {
            pstCltii = (MRS_ABNORMAL_CLTII_NODE*)pstNode;
            (hi_void)memcpy_s(pstAbnormal->astAbnormal[usIndex].ucDevId, HI_PLC_MAC_ADDR_LEN, pstCltii->ucDevId, HI_PLC_MAC_ADDR_LEN);
            pstAbnormal->astAbnormal[usIndex].ucAbnormal = pstCltii->ucAbnormal;

            pstNode = pstNode->next;
        }
        
        HI_MDM_ReportPacket(usId, (HI_U8)ulOption, (HI_PBYTE)pstAbnormal, sizeof(DIAG_CMD_MRS_GET_CLT_SEARCH_ABNORMAL_IND_STRU), HI_TRUE);
        usReport += pstAbnormal->usCurrent;
    }

    mrsToolsFree(pstAbnormal);
    return HI_ERR_SUCCESS;
}


//*****************************************************************************
// ��������: mrsCcoDfxSendSearchInfoToHso
// ��������: �ϱ�CCO�ѱ�ά����Ϣ
//           
// ����˵��:
//   usId [in] ��ѯID
//   ulOption [in] ��ѯѡ��
//
// �� �� ֵ: 
//    HI_ERR_NOT_ENOUGH_MEMORY ���붯̬�ڴ�ʧ��
//    HI_ERR_SUCCESS �ϱ��ɹ�
//    HI_ERR_FAILURE �����Ĵ���
// ����Ҫ��: 
// ��    ��: liupeidong/l00238134 [2015-01-20]
// ��    ��: liupeidong/l00238134 [2015-04-15]
//*****************************************************************************
HI_U32 mrsCcoDfxSendSearchInfoToHso(HI_U16 usId, HI_U32 ulOption)
{
    DIAG_CMD_MRS_GET_CCO_SEARCH_INFO_IND_STRU* pstInfo = HI_NULL;
    MRS_CCO_SRV_CTX_STRU* pstCcoCtx = mrsCcoGetContext();
    MRS_CCO_SEARCH_DFX_STRU* pstDfx = HI_NULL;
    
    pstInfo = mrsToolsMalloc(sizeof(DIAG_CMD_MRS_GET_CCO_SEARCH_INFO_IND_STRU));
    if (HI_NULL == pstInfo)
    {
        return HI_ERR_NOT_ENOUGH_MEMORY;
    }

    pstDfx = &(pstCcoCtx->stAutoSearchModel.stSearchDfx);
    (hi_void)memset_s(pstInfo, sizeof(DIAG_CMD_MRS_GET_CCO_SEARCH_INFO_IND_STRU), 0, sizeof(DIAG_CMD_MRS_GET_CCO_SEARCH_INFO_IND_STRU));
    pstInfo->ucInterfaceVer = MRS_DFX_CCO_SEARCH_INFO_IND_VER0;
    pstInfo->ucStruSize = (HI_U8)sizeof(DIAG_CMD_MRS_GET_CCO_SEARCH_INFO_IND_STRU);
    pstInfo->ucSearchStatus = (HI_U8)((pstCcoCtx->status & MRS_CCO_STATUS_SEARCH_METER) >> 4);
    pstInfo->usTotal = pstDfx->usTotal;
    pstInfo->usMeterEntryNum = pstDfx->usMeterEntryNum;
    pstInfo->usCltEntryNum = pstDfx->usCltEntryNum;
    pstInfo->usUserDefinedDuration = pstDfx->usUserDefinedDuration;

/* BEGIN: PN: DTS2015061008265 MODIFY\ADD\DEL by cuiate/00233580 at 2015/6/23 */
    // ��ͨ�ѱ� or ��ͨ�ѱ�+̨��ʶ���ѱ�: MRS_CCO_STATUS_SEARCH_MODE_NORMAL
    // ̨��ʶ���ѱ�: MRS_CCO_STATUS_SEARCH_MODE_TRANSFORMER
    if ((pstCcoCtx->status & MRS_CCO_STATUS_SEARCH_METER_MASK) == MRS_CCO_STATUS_SEARCH_METER_TRANSFORMER)
    {
        pstInfo->ucOption = MRS_CCO_STATUS_SEARCH_MODE_TRANSFORMER;
    }
/* END:   PN: DTS2015061008265 MODIFY\ADD\DEL by cuiate/00233580 at 2015/6/23 */

    if (pstInfo->ucSearchStatus)
    {
        // �ѱ�����У��ѱ��ʱΪ��ǰʱ������ѱ�ʼʱ���֮��
        pstInfo->ulSearchDuration = HI_MDM_GetMilliSeconds() - pstDfx->ulStartTime;
    }
    else
    {
        // ���ѱ�״̬���ѱ��ʱΪ�ѱ����ʱ������ѱ�ʼʱ���֮��
        pstInfo->ulSearchDuration = pstDfx->ulEndTime - pstDfx->ulStartTime;
    }

    HI_MDM_ReportPacket(usId, (HI_U8)ulOption, (HI_PBYTE)pstInfo, sizeof(DIAG_CMD_MRS_GET_CCO_SEARCH_INFO_IND_STRU), HI_TRUE);
    mrsToolsFree(pstInfo);
    return HI_ERR_SUCCESS;
}


//*****************************************************************************
// ��������: mrsCcoDfxSendSlaveNodeInfoToHso
// ��������: �ϱ�CCO�����ȫ���ѱ���,aucPayload�дӽڵ���Ϣ������ţ�
//           ĩβ�ռ䲻���������ŵ���һ֡�ϱ�
//           
// ����˵��:
//   usId [in] ��ѯID
//   pCmdParam [in] ��ѯ����Я���Ĳ����ṹ
//                 DIAG_CMD_MRS_GET_CCO_SLAVENODE_INFO_REQ_STRU
//   ulOption [in] ��ѯѡ��
//
// �� �� ֵ: 
//    HI_ERR_NOT_ENOUGH_MEMORY ���붯̬�ڴ�ʧ��
//    HI_ERR_SUCCESS �ϱ����
//    HI_ERR_FAILURE �����Ĵ���
// ����Ҫ��: 
// ��    ��: liupeidong/l00238134 [2015-01-20]
// ��    ��: liupeidong/l00238134 [2015-04-15]
//*****************************************************************************
HI_U32 mrsCcoDfxSendSlaveNodeInfoToHso(HI_U16 usId, HI_VOID* pCmdParam, HI_U32 ulOption)
{
    DIAG_CMD_MRS_GET_CCO_SLAVENODE_INFO_REQ_STRU* pstReq = HI_NULL;
    DIAG_CMD_MRS_GET_CCO_SLAVENODE_INFO_IND_STRU* pstPacket = HI_NULL;
    MRS_CCO_SRV_CTX_STRU* pstCcoCtx = mrsCcoGetContext();
    MRS_CCO_SEARCH_DFX_STRU* pstDfx = HI_NULL;
    MRS_DIAG_REPORT_PARAMS stParams = {0};
    HI_U8  ucReqSN = 0;
    HI_U8  ucReqOption = 0;
    HI_U16 usReqStart = 0;
    HI_U16 usReqCount = 0;
    HI_U16 usSavedTotal = 0;
    HI_U16 usMeterEntryNum = 0;
    HI_U16 usCltEntryNum = 0;
    HI_BOOL bReportPacket = HI_FALSE;
    
    pstPacket = mrsToolsMalloc(PB520_PAYLOAD_MAX_SIZE);
    if (HI_NULL == pstPacket)
    {
        return HI_ERR_NOT_ENOUGH_MEMORY;
    }
    
    (hi_void)memset_s(pstPacket, PB520_PAYLOAD_MAX_SIZE, 0, PB520_PAYLOAD_MAX_SIZE);
    pstPacket->ucInterfaceVer = MRS_DFX_CCO_SLAVENODE_INFO_IND_VER0;
    pstPacket->ucStruSize = sizeof(DIAG_CMD_MRS_GET_CCO_SLAVENODE_INFO_IND_STRU);

    do
    {
        // ����Ϊ�գ��ϱ������ݰ�
        pstReq = (DIAG_CMD_MRS_GET_CCO_SLAVENODE_INFO_REQ_STRU*)pCmdParam;
        if (HI_NULL == pstReq)
        {
            bReportPacket = HI_TRUE;
            break;
        }

        pstPacket->ucSN = pstReq->ucSN;
        ucReqSN = pstReq->ucSN;
        ucReqOption = pstReq->ucOption;
        usReqStart = pstReq->usStart;
        usReqCount = pstReq->usCount;

        pstDfx = &(pstCcoCtx->stAutoSearchModel.stSearchDfx);
        usSavedTotal = pstDfx->usTotal;
        usMeterEntryNum = pstDfx->usMeterEntryNum;
        usCltEntryNum = pstDfx->usCltEntryNum;
        
        // �����쳣���ϱ������ݰ�: ��ǰ����ѱ���Ϊ�գ���ѯ����Ϊ0����ѯ��ʼ��������������: �豸���Ͳ���ȷ
        if ((0 == usSavedTotal) || (0 == usReqCount) || (usReqStart >= usSavedTotal) 
            || (pstReq->ucOption > DIAG_MRS_REQ_OPTION_COLLECTOR))
        {
            bReportPacket = HI_TRUE;
            break;
        }

        // �����ϱ��������
        (hi_void)memset_s(&stParams, sizeof(stParams), 0, sizeof(stParams));
        stParams.usId = usId;
        stParams.ucOption = (HI_U8)ulOption;
        stParams.ucSN = ucReqSN;

        // ����������豸���͡���㡢�������ϱ����ݰ�
        switch (ucReqOption)
        {
        case DIAG_MRS_REQ_OPTION_ALL:
            stParams.usTotal = MRS_MIN(usReqCount, usSavedTotal - usReqStart);
            if (usReqStart < usMeterEntryNum)
            {
                // ��ѯ���λ���ز���������ϱ��ز�����������������
                stParams.usStart = usReqStart;
                stParams.usCount = MRS_MIN(stParams.usTotal, usMeterEntryNum - usReqStart);
                stParams.ucDevType = DIAG_MRS_DEVICE_TYPE_METER;
                mrsCcoDfxReportSearchResult(pstPacket, &stParams);
                if (stParams.usTotal > usMeterEntryNum - usReqStart)
                {
                    // ��ѯ�յ�λ�ڲɼ��������ϱ��ɼ����������������
                    stParams.usStart = 0;
                    stParams.usCount = stParams.usTotal - (usMeterEntryNum - usReqStart);
                    stParams.ucDevType = DIAG_MRS_DEVICE_TYPE_COLLECTOR;
                    mrsCcoDfxReportSearchResult(pstPacket, &stParams);
                }
            }
            else
            {
                // ��ѯ���λ�ڲɼ����������ϱ��ɼ����������������
                stParams.usStart = usReqStart - usMeterEntryNum;
                stParams.usCount = stParams.usTotal;
                stParams.ucDevType = DIAG_MRS_DEVICE_TYPE_COLLECTOR;
                mrsCcoDfxReportSearchResult(pstPacket, &stParams);
            }
            break;
            
        case DIAG_MRS_REQ_OPTION_METER:
            if (usReqStart < usMeterEntryNum)
            {
                // ��ѯ���û�г����ز���������ȣ��ϱ��ز�����������������
                stParams.usTotal = MRS_MIN(usReqCount, usMeterEntryNum - usReqStart);
                stParams.usStart = usReqStart;
                stParams.usCount = stParams.usTotal;
                stParams.ucDevType = DIAG_MRS_DEVICE_TYPE_METER;
                mrsCcoDfxReportSearchResult(pstPacket, &stParams);
            }
            else
            {
                // ��ѯ��㳬���ز���������ȣ��ϱ������ݰ�
                bReportPacket = HI_TRUE;
            }
            break;
            
        case DIAG_MRS_REQ_OPTION_COLLECTOR:
            if (usReqStart < usCltEntryNum)
            {
                // ��ѯ���û�г����ɼ��������ȣ��ϱ��ɼ����������������
                stParams.usTotal = MRS_MIN(usReqCount, usCltEntryNum - usReqStart);
                stParams.usStart = usReqStart;
                stParams.usCount = stParams.usTotal;
                stParams.ucDevType = DIAG_MRS_DEVICE_TYPE_COLLECTOR;
                mrsCcoDfxReportSearchResult(pstPacket, &stParams);
            }
            else
            {
                // ��ѯ��㳬���ɼ��������ȣ��ϱ������ݰ�
                bReportPacket = HI_TRUE;
            }
            break;
            
        default:
            break;
        }
    } while(0);

    if (bReportPacket)
    {
        HI_MDM_ReportPacket(usId, 
                            (HI_U8)ulOption, 
                            (HI_PBYTE)pstPacket, 
                            sizeof(DIAG_CMD_MRS_GET_CCO_SLAVENODE_INFO_IND_STRU), 
                            HI_FALSE);
    }
    
    mrsToolsFree(pstPacket);
    return HI_ERR_SUCCESS;
}


//*****************************************************************************
// ��������: mrsCcoDfxReportSearchResult
// ��������: ���������������Ӧ��������Ϊ���ݰ��ϱ�
//           
// ����˵��:
//   pstPacket   [in] �ϱ�DIAG���ݰ�������ʱ�ṹ
//   pstParams   [in] �ϱ�DIAG���ݰ��Ĳ���
//
// �� �� ֵ: 
//    HI_VOID
// ����Ҫ��: 
// ��    ��: liupeidong/l00238134 [2015-04-15]
//*****************************************************************************
HI_VOID mrsCcoDfxReportSearchResult(DIAG_CMD_MRS_GET_CCO_SLAVENODE_INFO_IND_STRU* pstPacket,
                                         MRS_DIAG_REPORT_PARAMS* pstParams)
{
    MRS_SRV_LIST* pstHead = HI_NULL;
    MRS_SRV_LINK* pstLink = HI_NULL;
    MRS_CARRIER_METER_NODE* pstMeter = HI_NULL;
    MRS_CLT_NODE* pstClt = HI_NULL;
    DIAG_MRS_CARRIER_METER_STRU* pstMeterEntry = HI_NULL;
    DIAG_MRS_CLT_STRU* pstCltEntry = HI_NULL;
    HI_U16 usEntrySize = 0;
    HI_U16 usStart = 0;
    HI_U16 usCount = 0;
    HI_U16 usPayloadLen = 0;
    HI_U16 usMeterEntrySize = sizeof(DIAG_MRS_CARRIER_METER_STRU);
    HI_U16 usPayloadLenMax = PB520_PAYLOAD_MAX_SIZE - sizeof(DIAG_CMD_MRS_GET_CCO_SLAVENODE_INFO_IND_STRU);
    HI_U8 bReportAA = mrsCcoGetContext()->stAutoSearchModel.stSearchCfg.report_AA_2_master;

    if (HI_NULL == pstPacket || HI_NULL == pstParams)
    {
        return;
    }

    // �����豸����ȡ��Ӧ������ͷ���ڵ�
    pstHead = (DIAG_MRS_DEVICE_TYPE_METER == pstParams->ucDevType)
              ? mrsCcoDfxSearchResultMeterList() 
              : mrsCcoDfxSearchResultCltList();

    if (HI_NULL == pstHead)
    {
        return;
    }

    // ���������ҵ���ѯ�����
    pstLink = pstHead->next;
    while (pstLink != pstHead)
    {
        if (usStart >= pstParams->usStart)
        {
            break;
        }
        pstLink = pstLink->next;
        usStart++;
    }

    while (pstLink != pstHead)
    {
        if (0 == usPayloadLen)
        {
            // ��д���ݰ�ͷ��
            (hi_void)memset_s(pstPacket, PB520_PAYLOAD_MAX_SIZE, 0, PB520_PAYLOAD_MAX_SIZE);
            pstPacket->ucInterfaceVer = MRS_DFX_CCO_SLAVENODE_INFO_IND_VER0;
            pstPacket->ucStruSize = sizeof(DIAG_CMD_MRS_GET_CCO_SLAVENODE_INFO_IND_STRU);
            pstPacket->ucSN = pstParams->ucSN;
            pstPacket->ucDevType = pstParams->ucDevType;
            pstPacket->usTotal =  pstParams->usTotal;
            pstPacket->usStartIndex = pstParams->usStartIndex;
        }
        
        if (usCount >= pstParams->usCount)
        {
            // �����ѯ�յ�
            break;
        }

        // ��ȡ��ǰ����ڵ�ӽڵ���Ϣռ�ÿռ��С
        if (DIAG_MRS_DEVICE_TYPE_METER == pstParams->ucDevType)
        {
            usEntrySize = usMeterEntrySize;
        }
        else
        {
            pstClt = (MRS_CLT_NODE*)pstLink;
            usEntrySize = sizeof(DIAG_MRS_CLT_STRU) + pstClt->ucNum * sizeof(DIAG_MRS_METER_STRU);            
        }

        if (usPayloadLen + usEntrySize > usPayloadLenMax)
        {
            if (0 == usPayloadLen)
            {
                // ������usEntrySize�ʹ���usPayloadLenMax�ˣ�break����ֹ��ѭ��
                break;
            }
            // �������ݰ��Ѿ��������ϱ����ݰ���payload��������
            HI_MDM_ReportPacket(pstParams->usId, 
                                pstParams->ucOption, 
                                (HI_PBYTE)pstPacket, 
                                sizeof(DIAG_CMD_MRS_GET_CCO_SLAVENODE_INFO_IND_STRU) + usPayloadLen, 
                                HI_FALSE);
            usPayloadLen = 0;
        }
        else
        {
            // �������ݰ������԰ѵ�ǰ����ڵ�����ȥ
            if (DIAG_MRS_DEVICE_TYPE_METER == pstParams->ucDevType)
            {
                // ����ڵ����ز�������payload
                pstMeter = (MRS_CARRIER_METER_NODE*)pstLink;
                pstMeterEntry = (DIAG_MRS_CARRIER_METER_STRU*)(pstPacket->aucPayload + usPayloadLen);
                
                pstMeterEntry->usSeq = pstMeter->usSeq;
                (hi_void)memcpy_s(pstMeterEntry->stMeter.aucMeterAddr, HI_METER_ADDR_LEN, pstMeter->aucMeterAddr, HI_METER_ADDR_LEN);
                pstMeterEntry->stMeter.ucProtoType = pstMeter->ucProtoType;
            }
            else
            {
                // ����ڵ��ǲɼ��������payload
                pstClt = (MRS_CLT_NODE*)pstLink;
                pstCltEntry = (DIAG_MRS_CLT_STRU*)(pstPacket->aucPayload + usPayloadLen);
                
                pstCltEntry->usSeq = pstClt->usSeq;
                (hi_void)memcpy_s(pstCltEntry->aucMac, HI_PLC_MAC_ADDR_LEN, pstClt->aucMac, HI_PLC_MAC_ADDR_LEN);
                (hi_void)memcpy_s(pstCltEntry->aucAsset, HI_METER_ADDR_LEN, pstClt->aucAsset, HI_METER_ADDR_LEN);
                (hi_void)memcpy_s(pstCltEntry->aucID, HI_PLC_MAC_ADDR_LEN, pstClt->aucID, HI_PLC_MAC_ADDR_LEN);
                pstCltEntry->ucTotal = pstClt->ucNum;
                pstCltEntry->ucNum = pstClt->ucNum;

/* BEGIN: PN: DTS2015042907341 MODIFY\ADD\DEL by cuiate/00233580 at 2015/5/18 */
                if (bReportAA)
                {
                    (hi_void)memcpy_s(pstCltEntry->aucPayload, usPayloadLenMax - usPayloadLen,
                                   pstClt->aucItem, 
                                   pstClt->ucNum * sizeof(DIAG_MRS_METER_STRU));
                }
                else
                {
                    HI_U8 i = 0;
                    DIAG_MRS_METER_STRU *pstDest = (DIAG_MRS_METER_STRU *)pstCltEntry->aucPayload;
                    DIAG_MRS_METER_STRU *pstSrc = (DIAG_MRS_METER_STRU *)pstClt->aucItem;
                    for (i = 0; i < pstClt->ucNum; i++)
                    {
                        mrsMeterAddrPreProc(pstDest->aucMeterAddr, pstSrc->aucMeterAddr, pstSrc->ucProtoType);
                        pstDest->ucProtoType = pstSrc->ucProtoType;
                        pstDest->ucPad = pstSrc->ucPad;

                        pstDest++;
                        pstSrc++;
                    }
                }
/* END:   PN: DTS2015042907341 MODIFY\ADD\DEL by cuiate/00233580 at 2015/5/18 */
            }

            usPayloadLen += usEntrySize;
            usCount++;
            pstPacket->ucCurrent++;
            pstParams->usStartIndex++;
            
            pstLink = pstLink->next;
        }
    }

    if (usPayloadLen > 0)
    {
        // �ѵ����ѯ�յ������β�����ϱ����ݰ�
        HI_MDM_ReportPacket(pstParams->usId, 
                            pstParams->ucOption, 
                            (HI_PBYTE)pstPacket, 
                            sizeof(DIAG_CMD_MRS_GET_CCO_SLAVENODE_INFO_IND_STRU) + usPayloadLen, 
                            HI_FALSE);
    }
}


MRS_SRV_LIST* mrsCcoDfxAbnormalCltiiList(HI_VOID)
{
    MRS_CCO_SRV_CTX_STRU* pstCcoCtx = mrsCcoGetContext();
    return &pstCcoCtx->stAutoSearchModel.stAbnormalCltii;
}


MRS_SRV_LIST* mrsCcoDfxSearchResultMeterList(HI_VOID)
{
    MRS_CCO_SRV_CTX_STRU* pstCcoCtx = mrsCcoGetContext();
    return &pstCcoCtx->stAutoSearchModel.stSearchResultMeter;
}


MRS_SRV_LIST* mrsCcoDfxSearchResultCltList(HI_VOID)
{
    MRS_CCO_SRV_CTX_STRU* pstCcoCtx = mrsCcoGetContext();
    return &pstCcoCtx->stAutoSearchModel.stSearchResultClt;
}


HI_VOID mrsCcoDfxUpdateAbnormalCltiiNodeList(MRS_PLC_METERLIST_UP_S* pstUpFrame)
{
    MRS_ABNORMAL_CLTII_NODE * pstCltiiNode = HI_NULL;
    MRS_SRV_LIST* pstList = HI_NULL;
    HI_U8 aucDevId[HI_PLC_MAC_ADDR_LEN] = { 0 };

    if (mrsToolsNormalAddr(pstUpFrame->asset))   
    {
        (hi_void)memcpy_s(aucDevId, HI_PLC_MAC_ADDR_LEN, pstUpFrame->asset, HI_PLC_MAC_ADDR_LEN);
    }
    else
    {
        (hi_void)memcpy_s(aucDevId, HI_PLC_MAC_ADDR_LEN, pstUpFrame->id, HI_PLC_MAC_ADDR_LEN);
    }

    if (mrsCcoDfxCheckDuplicateCltiiNode(aucDevId, pstUpFrame->abnormal))
    {
        return;
    }

    pstList = mrsCcoDfxAbnormalCltiiList();
    if (HI_NULL == pstList)
    {
        return;
    }

    pstCltiiNode = mrsToolsMalloc(sizeof(MRS_ABNORMAL_CLTII_NODE));
    if (HI_NULL == pstCltiiNode)
    {
        return;
    }

    (hi_void)memset_s(pstCltiiNode, sizeof(MRS_ABNORMAL_CLTII_NODE), 0, sizeof(MRS_ABNORMAL_CLTII_NODE));
    (hi_void)memcpy_s(pstCltiiNode->ucDevId, HI_PLC_MAC_ADDR_LEN, aucDevId, HI_PLC_MAC_ADDR_LEN);
    pstCltiiNode->ucAbnormal = pstUpFrame->abnormal;

    mrsSrvListInsTail(pstList, &(pstCltiiNode->stLink));
}


//*****************************************************************************
// ��������: mrsCcoDfxUpdateSearchResultList
// ��������: CCOˢ�±�����ѱ����������³�Ա��������β��
//           
// ����˵��:
//   pstNode   [in] ���ز��ӽڵ��ѱ���ԭʼ����
//   usSeq     [in] ���ز��ӽڵ��ϱ��ѱ��������
//
// �� �� ֵ: 
//    HI_VOID
// ����Ҫ��: 
// ��    ��: liupeidong/l00238134 [2015-01-20]
// ��    ��: liupeidong/l00238134 [2015-04-17]
//*****************************************************************************
HI_VOID mrsCcoDfxUpdateSearchResultList(METER_NODE* pstNode, HI_U16 usSeq)
{
    MRS_CCO_SRV_CTX_STRU* pstCcoCtx = mrsCcoGetContext();
    MRS_CARRIER_METER_NODE* pstMeter = HI_NULL;
    MRS_CLT_NODE* pstClt = HI_NULL;
    MRS_CMD_METERLIST_ITEM* pstItem = HI_NULL;
    DIAG_MRS_METER_STRU* pstTemp = HI_NULL;
    MRS_SRV_LIST* pstList = HI_NULL;
    HI_U16 usCltEntryLen = 0;
    HI_U8 ucIndex = 0;

    // �����豸���ͣ�����ռ䣬����Ӧ�Ĵӽڵ���Ϣ׷�ӵ�����β��
    if (MRS_DEVICE_TYPE_COLLECTOR == pstNode->type)
    {
        pstList = mrsCcoDfxSearchResultCltList();
        if (HI_NULL == pstList)
        {
            return;
        }

        usCltEntryLen = sizeof(MRS_CLT_NODE) + pstNode->num * sizeof(MRS_CMD_METERLIST_ITEM);        
        pstClt = mrsToolsMalloc(usCltEntryLen);
        if (HI_NULL == pstClt)
        {
            return;
        }

        (hi_void)memset_s(pstClt, usCltEntryLen, 0, usCltEntryLen);
        pstClt->usSeq = usSeq;
        (hi_void)memcpy_s(pstClt->aucMac, HI_PLC_MAC_ADDR_LEN, pstNode->mac, HI_PLC_MAC_ADDR_LEN);
        (hi_void)memcpy_s(pstClt->aucAsset, HI_METER_ADDR_LEN, pstNode->asset, HI_METER_ADDR_LEN);
        (hi_void)memcpy_s(pstClt->aucID, HI_PLC_MAC_ADDR_LEN, pstNode->id, HI_PLC_MAC_ADDR_LEN);
        pstClt->ucNum = pstNode->num;

        pstItem = (MRS_CMD_METERLIST_ITEM*)pstNode->item;
        pstTemp = (DIAG_MRS_METER_STRU*)pstClt->aucItem;
        for (ucIndex = 0; ucIndex < pstNode->num; ucIndex++)
        {
            (hi_void)memcpy_s(pstTemp->aucMeterAddr, HI_METER_ADDR_LEN, pstItem->addr, HI_METER_ADDR_LEN);
            pstTemp->ucProtoType = pstItem->protocol;
            pstItem++;
            pstTemp++;
        }

        mrsSrvListInsTail(pstList, &(pstClt->stLink));
        pstCcoCtx->stAutoSearchModel.stSearchDfx.usCltEntryNum++;
    }
    else if (MRS_DEVICE_TYPE_METER == pstNode->type)
    {
        pstList = mrsCcoDfxSearchResultMeterList();
        if (HI_NULL == pstList)
        {
            return;
        }        

        pstMeter = mrsToolsMalloc(sizeof(MRS_CARRIER_METER_NODE));
        if (HI_NULL == pstMeter)
        {
            return;
        }

        (hi_void)memset_s(pstMeter, sizeof(MRS_CARRIER_METER_NODE), 0, sizeof(MRS_CARRIER_METER_NODE));
        pstMeter->usSeq = usSeq;
        pstItem = (MRS_CMD_METERLIST_ITEM *)pstNode->item;
        (hi_void)memcpy_s(pstMeter->aucMeterAddr, HI_METER_ADDR_LEN, pstItem->addr, HI_METER_ADDR_LEN);
        pstMeter->ucProtoType = pstItem->protocol;

        mrsSrvListInsTail(pstList, &(pstMeter->stLink));
        pstCcoCtx->stAutoSearchModel.stSearchDfx.usMeterEntryNum++;
    }
    pstCcoCtx->stAutoSearchModel.stSearchDfx.usTotal++;
}


HI_BOOL mrsCcoDfxCheckDuplicateCltiiNode(HI_U8* pucDevId, HI_U8 ucAbnormal)
{
    MRS_SRV_LIST* pstHead = HI_NULL;
    MRS_SRV_LINK* pstNode = HI_NULL;
    MRS_ABNORMAL_CLTII_NODE* pstCltii = HI_NULL;

    pstHead = mrsCcoDfxAbnormalCltiiList();
    if (HI_NULL == pstHead)
    {
        return HI_FALSE;
    }
    
    pstNode = pstHead->next;
    while (pstNode != pstHead)
    {
        pstCltii = (MRS_ABNORMAL_CLTII_NODE*)pstNode;
        if (mrsToolsMemEq(pucDevId, pstCltii->ucDevId, HI_PLC_MAC_ADDR_LEN))
        {
            pstCltii->ucAbnormal = ucAbnormal;
            return HI_TRUE;
        }
        
        pstNode = pstNode->next;
    }
    
    return HI_FALSE;  
}


HI_VOID mrsCcoDfxClearList(MRS_SRV_LIST* pstListHead)
{
    mrsSrvListClear(pstListHead, mrsCcoDfxFreeListNode, HI_NULL);
}


HI_VOID mrsCcoDfxFreeListNode(HI_VOID *pstNode, HI_VOID *pstCb)
{
    MRS_NOT_USED(pstCb);

    mrsToolsFree(pstNode);
}


HI_VOID mrsCcoDfxSearchSetDuration(MRS_CCO_SEARCH_DFX_STRU* pstDfx, HI_U16 usSearchDuration)
{
    pstDfx->usUserDefinedDuration = usSearchDuration;     
}


HI_VOID mrsCcoDfxSearchStartTime(MRS_CCO_SEARCH_DFX_STRU* pstDfx)
{
    pstDfx->ulStartTime = HI_MDM_GetMilliSeconds();        
}


HI_VOID mrsCcoDfxSearchEndTime(MRS_CCO_SEARCH_DFX_STRU* pstDfx)
{
    pstDfx->ulEndTime = HI_MDM_GetMilliSeconds();
}

/* BEGIN: PN:  MODIFY\ADD\DEL by cuichunyan/00276962 at 2015/11/11 */

HI_U32 mrsCcoDfxSendTimerSaveFlashToHso(HI_U32 ulOption)
{
    DIAG_CMD_MRS_CCO_GET_PARAM_INF_STRU stCcoInf = {0};
    
    (hi_void)memset_s(&stCcoInf, sizeof(DIAG_CMD_MRS_CCO_GET_PARAM_INF_STRU), 0, sizeof(DIAG_CMD_MRS_CCO_GET_PARAM_INF_STRU));

    stCcoInf.ucFlashSaveDelay = mrsCcoGetContext()->ucFlashSaveDelay;

    HI_MDM_ReportPacket(ID_DIAG_CMD_MRS_CCO_GET_PARAM,
                        (HI_U8)ulOption,
                        (HI_PBYTE)&stCcoInf,
                        sizeof(DIAG_CMD_MRS_CCO_GET_PARAM_INF_STRU), HI_FALSE);

    return HI_ERR_SUCCESS;
}
/* END: PN:  MODIFY\ADD\DEL by cuichunyan/00276962 at 2015/11/11 */

#endif

