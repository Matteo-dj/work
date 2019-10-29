//*****************************************************************************
//
//                  ��Ȩ���� (C), 1998-2011, ��Ϊ�������޹�˾
//
//*****************************************************************************
//  �� �� ��   : mrs_srv_cco_macmap.c
//  �� �� ��   : V1.0 
//  ��    ��   : liuxipeng/KF54842
//  ��������   : 2011-12-13
//  ��������   : Mac��ַӳ���ά��ģ�麯����ʵ��
//  �����б�   : 
//  �޸���ʷ   : 
//  1.��    �� : 2011-12-13
//    ��    �� : liuxipeng/KF54842
//    �޸����� : �����ļ� 
//
//*****************************************************************************
#include "mrs_common.h"
#include "mrs_fw_tools.h"
#include "mrs_fw_log.h"
#include "mrs_srv_res.h"
#include "mrs_srv_cco_macmap.h"
#include "mrs_fw_proto376_2_echo.h"
#include "mrs_srv_parallel_cco.h"
#include "mrs_srv_list.h"
#include "mrs_srv_cco_event.h"
#include "mrs_srv_cco.h"
#include "mrs_dfx.h"
#include "mrs_srv_common.h"
#include "mrs_srv_cco_readmeter.h"
#include "mrs_fw_proto645.h"

#if defined(PRODUCT_CFG_PRODUCT_TYPE_CCO)

HI_PRV HI_U32 mrsMapQueryMac(MRS_MAP_MODULE *module, HI_U8 mac[]);
HI_PRV HI_VOID mrsMapRefreshNewMacList(MRS_MAC_LIST * pstMacList, 
                                            HI_MAC_NETWORK_TOPO_ENTRY_S * pstTopo,
                                            HI_U16 ucTopoNum);
HI_PRV HI_BOOL mrsMapMacIsFromMeter(HI_U8 *pucMac);
HI_PRV HI_U32 mrsMapFastCreate(MRS_MAP_MODULE *module, 
                                    HI_MAC_NETWORK_TOPO_ENTRY_S *pstTopo);
HI_PRV HI_VOID mrsMapReformingNewMacList(MRS_MAC_LIST * pstMacList);

HI_PRV HI_VOID mrsMapTtlDecrease(MRS_MAC_DEVICE_MAP *pstMap);
HI_PRV HI_VOID mrsMapTtlZeroProc(MRS_MAC_DEVICE_MAP *pstMap);
HI_PRV HI_VOID mrsDeleteMacFromMap(MRS_MAC_DEVICE_MAP *pstMap, HI_U8 *pucMacAddr, HI_BOOL bRepeat);
HI_PRV HI_VOID mrsDeleteDeviceFromMap(MRS_MAC_DEVICE_MAP *pstMap, HI_U8 *pucDeviceAddr);
HI_PRV HI_U32 mrsGetMacFromMap(MRS_MAC_DEVICE_MAP *pstMacMap, HI_U8 *pucDeviceAddr, HI_U8 *pucMacAddr);
HI_PRV HI_U32 mrsRefreshMapItem(MRS_MAC_DEVICE_MAP *pstMap, HI_S16 sIndex, HI_U8 *pucMacAddr, MRS_DEVICE_INF_STRU *pstDevInf, HI_U8 ucPhase);
HI_PRV MRS_MAC_DEVICE_PAIR * mrsFindMeterInMap(MRS_MAC_DEVICE_MAP *pstMap, HI_U8 *pucDeviceAddr);
HI_PRV HI_U32 mrsMapAddMacDevice(MRS_MAC_DEVICE_MAP *pstMap, HI_U8 *pucMacAddr, MRS_DEVICE_INF_STRU *pstDevInf, HI_U8 ucPhase);
HI_PRV HI_VOID mrsMapSyncMacList(MRS_MAC_DEVICE_MAP *pstMap, MRS_MAC_LIST *pstMacList);
HI_PRV HI_U16 mrsMapGetMaxTTL(HI_VOID);
HI_PRV HI_U32 mrsMapGetDeviceAddr(HI_U8 pucMacAddr[], HI_U8 pucDeviceAddr[]);
HI_PRV HI_VOID mrsSetCcoMapDeviceInf(MRS_CCO_DEVICE_INF_STRU *pstMapDevice, MRS_DEVICE_INF_STRU *pstDevInf);
HI_PRV HI_U32 mrsSetDeviceInfo(MRS_645_FRAME_STRU *pstFrame, HI_U8 *pucMac, MRS_DEVICE_INF_STRU *pstDevice);
HI_PRV HI_U8 mrsGetMacStatus(HI_U8 ucReport);
HI_PRV HI_BOOL mrsIsMacInMaclist(MRS_MAC_LIST *pstMaclist, HI_U8 *pucMac, HI_OUT HI_U8 *pucReport);
HI_PRV HI_BOOL mrsIsMacInMap(MRS_MAC_DEVICE_MAP *pstMap, HI_U8 *pucMac, HI_OUT HI_U8 *pucStatus);

//��ʼ��ӳ���
HI_U32 mrsCcoMapInit(MRS_MAP_MODULE * module)
{
    MRS_MAC_DEVICE_PAIR * pstMeterItem = HI_NULL;
    MRS_MAC_DEVICE_PAIR * pstCltItem = HI_NULL;
    MRS_MAC_INFO * pstMacList = HI_NULL;

    do 
    {
        if (!module->bInit)
        {
            mrsGetNvMacMapInfo(module);
            
            pstMacList = (MRS_MAC_INFO *)mrsToolsMalloc(sizeof(MRS_MAC_INFO) * PRODUCT_CFG_MRS_MAX_TOPO_NUM);
            if (!pstMacList)
            {
                break;
            }
            
            (hi_void)memset_s(pstMacList,
                sizeof(MRS_MAC_INFO) * PRODUCT_CFG_MRS_MAX_TOPO_NUM,0,
                sizeof(MRS_MAC_INFO) * PRODUCT_CFG_MRS_MAX_TOPO_NUM);

            pstMeterItem = (MRS_MAC_DEVICE_PAIR*)mrsToolsMalloc(sizeof(MRS_MAC_DEVICE_PAIR) * MRS_MAC_DEVICE_MAP_NUM_MAX);
            if (!pstMeterItem)
            {
                break;
            }
            
            (hi_void)memset_s(pstMeterItem,
                sizeof(MRS_MAC_DEVICE_PAIR) * MRS_MAC_DEVICE_MAP_NUM_MAX,0,
                sizeof(MRS_MAC_DEVICE_PAIR) * MRS_MAC_DEVICE_MAP_NUM_MAX);

            module->stNewMacList.number = 0;
            module->stNewMacList.mac = pstMacList;

            module->stMacMeterMap.usNumber = 0;
            module->stMacMeterMap.usMapSize = MRS_MAC_DEVICE_MAP_NUM_MAX;
            module->stMacMeterMap.pstItem = pstMeterItem;

            (hi_void)memset_s(&module->stMacCltMap, sizeof(module->stMacCltMap), 0, sizeof(module->stMacCltMap));
            
            if (module->ucCltMapEnable)
            {
                pstCltItem = (MRS_MAC_DEVICE_PAIR*)mrsToolsMalloc(sizeof(MRS_MAC_DEVICE_PAIR) * MRS_MAC_CLT_MAP_NUM_MAX);
                if (!pstCltItem)
                {
                    break;
                }
                
                (hi_void)memset_s(pstCltItem,
                    sizeof(MRS_MAC_DEVICE_PAIR) * MRS_MAC_CLT_MAP_NUM_MAX,0,
                    sizeof(MRS_MAC_DEVICE_PAIR) * MRS_MAC_CLT_MAP_NUM_MAX);
                module->stMacCltMap.usNumber = 0;
                module->stMacCltMap.usMapSize = MRS_MAC_CLT_MAP_NUM_MAX;
                module->stMacCltMap.pstItem = pstCltItem;
            }

            module->bInit = HI_TRUE;
        }

        return mrsMapEmptyModule(module);
    } while (0);

    mrsToolsFree(pstMacList);
    mrsToolsFree(pstMeterItem);
    mrsToolsFree(pstCltItem);

    return HI_ERR_FAILURE;
}


HI_VOID mrsDeleteMacFromMap(MRS_MAC_DEVICE_MAP *pstMacMap, HI_U8 *pucMacAddr, HI_BOOL bRepeat)
{
    MRS_MAC_DEVICE_PAIR *pItem = pstMacMap->pstItem;
    HI_U16 usIndex = 0;

    if (!pItem)
    {
        return;
    }

    for (usIndex = 0; usIndex < pstMacMap->usMapSize; usIndex++, pItem++)
    {
        if (pItem->bValid && (mrsToolsMemEq(pItem->aucMacAddr, pucMacAddr, HI_PLC_MAC_ADDR_LEN) || bRepeat))
        {
            (hi_void)memset_s(pItem, sizeof(MRS_MAC_DEVICE_PAIR), 0, sizeof(MRS_MAC_DEVICE_PAIR));
            pstMacMap->usNumber--;
        }
    }
}


HI_VOID mrsDeleteDeviceFromMap(MRS_MAC_DEVICE_MAP *pstMacMap, HI_U8 *pucDeviceAddr)
{
    MRS_MAC_DEVICE_PAIR *pItem = pstMacMap->pstItem;
    HI_U16 usIndex = 0;

    if (!pItem)
    {
        return;
    }

    for (usIndex = 0; usIndex < pstMacMap->usMapSize; usIndex++, pItem++)
    {
        if (pItem->bValid && mrsToolsMemEq(pItem->stDeviceInf.aucAddr, pucDeviceAddr, HI_METER_ADDR_LEN))
        {
            (hi_void)memset_s(pItem, sizeof(MRS_MAC_DEVICE_PAIR), 0, sizeof(MRS_MAC_DEVICE_PAIR));
            pstMacMap->usNumber--;
            break;
        }
    }
}


// ɾ��ӳ����е�MAC
HI_U32 mrsMapDeleteMac(MRS_MAP_MODULE *pstModule, HI_U8 * pucMacAddr, HI_BOOL bRepeat)
{
    // �㲥��ַ����Ч��ַҲ������
    if (!pucMacAddr || mrsToolsAbnormalMac(pucMacAddr))
    {
        return HI_ERR_FAILURE;
    }

    mrsDeleteMacFromMap(&pstModule->stMacMeterMap, pucMacAddr, bRepeat);
    mrsDeleteMacFromMap(&pstModule->stMacCltMap, pucMacAddr, bRepeat);

    return HI_ERR_SUCCESS;
}


// ���New Mac List��MAC�Ѿ��ϱ�������
HI_U32 mrsMapMarkMac(MRS_MAP_MODULE *pstModule, HI_U8 *pucMacAddr, HI_U8 ucStatus)
{
    MRS_MAC_INFO *pItem = pstModule->stNewMacList.mac;
    HI_S16 sNum = pstModule->stNewMacList.number;
    HI_S16 i = 0;

    if (MRS_SEARCH_STATUS_UNKNOWN == ucStatus)
    {
        return HI_ERR_SUCCESS;
    }
    
    for (i = 0; i < sNum; i++, pItem++)
    {
        if (mrsToolsMemEq(pItem->ucMacAddr, pucMacAddr, HI_PLC_MAC_ADDR_LEN))
        {
            if (MRS_SEARCH_STATUS_IN_PROGRESS == ucStatus)
            {
                pItem->report = MRS_MAP_REPORT_CONTINUE;
            }
            else
            {
                pItem->report = MRS_MAP_REPORT_SUCCESS;
            }
            
            break;
        }
    }

    return HI_ERR_SUCCESS;
}


HI_U32 mrsGetMacFromMap(MRS_MAC_DEVICE_MAP *pstMacMap, HI_U8 *pucDeviceAddr, HI_U8 *pucMacAddr)
{
    MRS_MAC_DEVICE_PAIR *pItem = pstMacMap->pstItem;
    HI_U16 usIndex = 0;

    if (!pItem)
    {
        return HI_ERR_FAILURE;
    }

    for (usIndex = 0; usIndex < pstMacMap->usMapSize; usIndex++, pItem++)
    {
        if (pItem->bValid && mrsToolsMemEq(pItem->stDeviceInf.aucAddr, pucDeviceAddr, HI_METER_ADDR_LEN))
        {
           (hi_void)memcpy_s(pucMacAddr, HI_PLC_MAC_ADDR_LEN, pItem->aucMacAddr, HI_PLC_MAC_ADDR_LEN);
            return HI_ERR_SUCCESS;
        }
    }

    return HI_ERR_NOT_FOUND;
}


//���ݵ���ַ��ȡMAC
HI_U32 mrsMapGetMAC(HI_U8 *pucDeviceAddr, HI_U8 *pucMacAddr)
{
    MRS_MAP_MODULE *pstModule = &(mrsCcoGetContext()->stMapModule);
    HI_U32 ulRet = HI_ERR_SUCCESS;

    ulRet = mrsGetMacFromMap(&pstModule->stMacCltMap, pucDeviceAddr, pucMacAddr);
    if (HI_ERR_SUCCESS == ulRet)
    {
        return ulRet;
    }

    ulRet = mrsGetMacFromMap(&pstModule->stMacMeterMap, pucDeviceAddr, pucMacAddr);
    if (HI_ERR_SUCCESS == ulRet)
    {
        return ulRet;
    }

    if (HI_FALSE == mrsGetCcoPlcChlStatus())
    {
        ulRet = mrsGetMacFromTopo(pucDeviceAddr, pucMacAddr);
    }

    return ulRet;
}


HI_U32 mrsMapAddMeter(MRS_MAP_MODULE *pstModule, HI_U8 *pucMacAddr, MRS_DEVICE_INF_STRU *pstMeterInf, HI_U8 ucPhase)
{
    MRS_MAC_DEVICE_PAIR *pstPair = HI_NULL;

    if (MRS_MAP_DEV_TYPE_METER != pstMeterInf->ucType)
    {
        return HI_ERR_INVALID_PARAMETER;
    }

    pstPair = mrsFindMeterInMap(&pstModule->stMacCltMap, pstMeterInf->aucAddr);
    if (pstPair)
    {
        pstPair->usMrTTL = MRS_MAP_MAX_MR_TTL;
        pstPair->usPhase = ucPhase;
        return HI_ERR_SUCCESS;
    }

    if (HI_ERR_SUCCESS == mrsMapAddMacDevice(&pstModule->stMacMeterMap, pucMacAddr, pstMeterInf, ucPhase))
    {
        return HI_ERR_SUCCESS;
    }

    return HI_ERR_FAILURE;
}


HI_U32 mrsMapAddCollector(MRS_MAP_MODULE *pstModule, HI_U8 *pucMacAddr, MRS_DEVICE_INF_STRU *pstCltInf)
{
    if ((MRS_MAP_DEV_TYPE_CLT_I != pstCltInf->ucType) && (MRS_MAP_DEV_TYPE_CLT_II != pstCltInf->ucType))
    {
        return HI_ERR_INVALID_PARAMETER;
    }

    if (!pstModule->ucCltMapEnable)
    {
        return HI_ERR_SKIP;
    }
    
    if (mrsToolsCheckBCD(pstCltInf->aucAddr, HI_METER_ADDR_LEN))
    {
        // �ӵ��ӳ�����ɾ���ɼ�����ַ
        mrsDeleteDeviceFromMap(&pstModule->stMacMeterMap, pstCltInf->aucAddr);
    }

    if (HI_ERR_SUCCESS == mrsMapAddMacDevice(&pstModule->stMacCltMap, pucMacAddr, pstCltInf, 0))
    {
        return HI_ERR_SUCCESS;
    }

    return HI_ERR_FAILURE;
}


HI_U32 mrsMapAddMacDevice(MRS_MAC_DEVICE_MAP *pstMap, HI_U8 *pucMacAddr, MRS_DEVICE_INF_STRU *pstDevInf, HI_U8 ucPhase)
{
    MRS_MAC_DEVICE_PAIR *pItem = pstMap->pstItem;
    HI_S16 sIndex = 0;
    HI_S16 sEmptyPos = -1;
    HI_S16 sMeterPos = -1;
    HI_S16 sAgedPos = -1;

    if (mrsToolsAbnormalMac(pucMacAddr) || !pItem)
    {
        return HI_ERR_FAILURE;
    }

    for (sIndex = 0; sIndex < pstMap->usMapSize; sIndex++, pItem++)
    {
        if (!pItem->bValid)
        {
            if (-1 == sEmptyPos)
            {
                sEmptyPos = sIndex;
            }

            continue;
        }

        if (mrsToolsMemEq(pItem->aucMacAddr, pucMacAddr, HI_PLC_MAC_ADDR_LEN)
            && mrsToolsMemEq(pItem->stDeviceInf.aucAddr, pstDevInf->aucAddr, HI_METER_ADDR_LEN))
        {
            mrsSetCcoMapDeviceInf(&pItem->stDeviceInf, pstDevInf);
            pItem->usTTL = mrsMapGetMaxTTL();
            pItem->usMrTTL = MRS_MAP_MAX_MR_TTL;
            pItem->bValid = HI_TRUE;
            pItem->usPhase = ucPhase;

            return HI_ERR_SUCCESS;
        }

        if ((-1 == sMeterPos) && mrsToolsMemEq(pItem->stDeviceInf.aucAddr, pstDevInf->aucAddr, HI_METER_ADDR_LEN))
        {
            sMeterPos = sIndex;
            break;
        }

        if ((-1 == sAgedPos) && (!pItem->usTTL))
        {
            sAgedPos = sIndex;
        }
    }

    if ((-1 == sEmptyPos) && (-1 == sMeterPos))
    {
        return mrsRefreshMapItem(pstMap, sAgedPos, pucMacAddr, pstDevInf, ucPhase);
    }

    if (-1 != sMeterPos)
    {
        (HI_VOID)mrsRefreshMapItem(pstMap, sMeterPos, pucMacAddr, pstDevInf, ucPhase);
    }
    else
    {
        (HI_VOID)mrsRefreshMapItem(pstMap, sEmptyPos, pucMacAddr, pstDevInf, ucPhase);
        pstMap->usNumber++;
    }

    return HI_ERR_SUCCESS;
}


HI_U32 mrsMapRecheckMacMeter(MRS_MAP_MODULE *pstModule, HI_MAC_NETWORK_TOPO_ENTRY_S * pstEntry, HI_U16 usNum)
{
    if (!pstEntry)
    {
        return HI_ERR_FAILURE;
    }

    if (0 == usNum)
    {
        mrsMapEmptyModule(pstModule);
        return HI_ERR_SUCCESS;
    }

    MRS_StopTimer(MRS_CCO_TIMER_MAP);
    mrsMapRefreshNewMacList(&pstModule->stNewMacList, pstEntry, usNum);

    mrsMapSyncMacList(&pstModule->stMacCltMap,   &pstModule->stNewMacList);
    mrsMapSyncMacList(&pstModule->stMacMeterMap, &pstModule->stNewMacList);

    // ʹ��NewMacList��Mac��ַ���ٴ���ӳ���
    mrsMapFastCreate(pstModule, pstEntry);

    // ��������NewMacList
    mrsMapReformingNewMacList(&pstModule->stNewMacList);

    return HI_ERR_SUCCESS;
}


//MAPά������
HI_U32 mrsMapMaintain(MRS_MAP_MODULE *module)
{
    MRS_MAC_LIST *maclist = 0;
    HI_U16 current = 0;
    HI_S16 number = 0;
    MRS_MAC_INFO *mac;
    HI_U16 usSendMacPlcTime = 0;
    HI_U16 usSendLastMacPlcTime = 0;
    HI_U8 count = 0;
    HI_BOOL ucTurnFlag = HI_FALSE;
    static HI_BOOL bFirst = HI_FALSE;
    
    if (!module) 
    {
        return HI_ERR_FAILURE;
    }

    maclist = &module->stNewMacList;
    mac = maclist->mac;
    if (!mac) 
    {
        return HI_ERR_FAILURE;
    }

    usSendMacPlcTime = module->usSendMacPlcTime;
    usSendLastMacPlcTime = module->usSendLastMacPlcTime;

    if (HI_FALSE == bFirst)
    {
        bFirst = HI_TRUE;
        if (!maclist->number)
        {
            ucTurnFlag = HI_TRUE;//������Ϊȫ�ػ����Ҷ��Ա��ַ����ʱ,cco״ֱ̬����ӳ���ά��״̬��Ϊ����״̬
        }
    }

    while (maclist->number)
    {
        current = maclist->current;
        number = maclist->number;

        if (mac[current].retries > module->ucMaxRetryNum)
        {
            HI_DIAG_LOG_MSG_E2(MRS_FILE_LOG_FLAG_003, HI_DIAG_MT("max retries"), mac[current].retries, current);
            ucTurnFlag = HI_TRUE;
            maclist->current = 0;
            break;
        }
        
        if (MRS_MAP_REPORT_SUCCESS == mac[current].report)
        {
           (hi_void)memcpy_s(&mac[current], sizeof(MRS_MAC_INFO), &mac[--number], sizeof(MRS_MAC_INFO));
            (hi_void)memset_s(&mac[number], sizeof(MRS_MAC_INFO), 0, sizeof(MRS_MAC_INFO));
            maclist->number--;
            if (!maclist->number)
            {
                HI_DIAG_LOG_MSG_E0(MRS_FILE_LOG_FLAG_006, HI_DIAG_MT("map succ"));
                ucTurnFlag = HI_TRUE;
                maclist->current = 0;
                break;
            }
            
            if (maclist->current >= maclist->number) 
            {
                MRS_StartTimer(MRS_CCO_TIMER_MAP, usSendLastMacPlcTime, HI_SYS_TIMER_ONESHOT);//MRS_CCO_TIME_MAP_L
                maclist->current = 0;
                break;
            }
        }
        else if (MRS_MAP_REPORT_SUCCESS != mac[current].report)
        {
            //�����վ��Ĳ�ѯ����Ϊ0���վ����ϱ���־��δ�ϱ�ʱ,��Ҫ���վ�㷢�Ͳ�ѯӳ�������,
            //��������ϱ����ѱ�״̬Ϊ�ѱ���ʱ,����Ҫ������,�ȴ������ֽ���ӳ���ϵʱ�ٿ�ʼ����
            if ((!mac[current].retries) || (MRS_MAP_REPORT_IDLE == mac[current].report))
            {
                mrsMapQueryMac(module,mac[current].ucMacAddr);
                count++;
            }
            
            mac[current].retries++;
            
            if (current >= (number - 1)) 
            {                
                MRS_StartTimer(MRS_CCO_TIMER_MAP, usSendLastMacPlcTime, HI_SYS_TIMER_ONESHOT);//MRS_CCO_TIME_MAP_L
                maclist->current = 0;
                break;
            } 

            maclist->current++;
            
            if (count == module->ucMaxParallelnum)
            {
                MRS_StartTimer(MRS_CCO_TIMER_MAP, usSendMacPlcTime, HI_SYS_TIMER_ONESHOT);
                break;
            }
        }
    }
    
    if (ucTurnFlag) 
    {
        if (maclist->number)
        {
            MRS_StartTimer(EN_MRS_SRV_CCO_MAP_REFRESH_TIMER, module->map_turn_time * 1000, HI_SYS_TIMER_ONESHOT);
        }
        
        mrsDfxMapReadyTime();  // ˢ��ӳ���ά�����ʱ��

        if (mrsCcoGetState() & MRS_CCO_STATUS_CREATE_MAP)
        {
            mrsCcoMapToNormalPro();
        }
    }

    return HI_ERR_SUCCESS;
}


//���ʼ��ӳ���
HI_U32 mrsMapUnInitModule(MRS_MAP_MODULE * pstModule)
{
    if (pstModule) 
    {
        mrsToolsFree(pstModule->stNewMacList.mac);
        mrsToolsFree(pstModule->stMacMeterMap.pstItem);
        mrsToolsFree(pstModule->stMacCltMap.pstItem);
        (hi_void)memset_s(pstModule, sizeof(MRS_MAP_MODULE), 0, sizeof(MRS_MAP_MODULE));
    }

    return HI_ERR_SUCCESS;
}


HI_U32 mrsMapEmptyModule(MRS_MAP_MODULE * module)
{
    if (!module->stMacMeterMap.pstItem || !module->stNewMacList.mac) 
    {
        return HI_ERR_FAILURE;
    }

    // ���������ݽṹ
    module->stMacMeterMap.usNumber = 0;
    (hi_void)memset_s(module->stMacMeterMap.pstItem,
        (HI_S32)(sizeof(MRS_MAC_DEVICE_PAIR) * module->stMacMeterMap.usMapSize),0,
        (HI_S32)(sizeof(MRS_MAC_DEVICE_PAIR) * module->stMacMeterMap.usMapSize));

    if (module->ucCltMapEnable)
    {
        module->stMacCltMap.usNumber = 0;
        (hi_void)memset_s(module->stMacCltMap.pstItem,
            (HI_S32)(sizeof(MRS_MAC_DEVICE_PAIR) * module->stMacCltMap.usMapSize),0,
            (HI_S32)(sizeof(MRS_MAC_DEVICE_PAIR) * module->stMacCltMap.usMapSize));
    }

    module->stNewMacList.number = 0;
    (hi_void)memset_s(module->stNewMacList.mac,
        (HI_S32)(sizeof(MRS_MAC_INFO) * PRODUCT_CFG_MRS_MAX_TOPO_NUM),0,
        (HI_S32)(sizeof(MRS_MAC_INFO) * PRODUCT_CFG_MRS_MAX_TOPO_NUM));
    
    return HI_ERR_SUCCESS;
}


HI_U32 mrsMapTimeout(MRS_MAP_MODULE *module)
{
    return mrsMapMaintain(module);
}

HI_U32 mrsMapQueryMac(MRS_MAP_MODULE *module, hi_u8 *mac)
{
    MRS_CCO_SRV_CTX_STRU *cco = mrsCcoGetContext();
    MRS_PLC_FRAME_DATA_STRU plc;
    MRS_PLC_METERLIST_DL_S frame;
    hi_u8 aucIp[HI_IPV4_ADDR_LEN] = {0};
   
    /* �ж�MAC�Ͳ�������Ч�� */
    if ((module == HI_NULL) || (mac == HI_NULL) || mrsToolsAbnormalMac(mac)) {
        return HI_ERR_FAILURE;
    }

    (hi_void)memset_s(&frame, sizeof(frame), 0, sizeof(frame));
    frame.interface_ver = MRS_GET_METERLIST_DL_VER;
    frame.stru_len = sizeof(frame);
    frame.option = MRS_SM_SEARCH_RESULT;
    frame.seq = cco->stAutoSearchModel.stSearchCfg.seq;
    frame.force_resp = 1;
    (hi_void)HI_MDM_NM_GetAddr(frame.src, aucIp);
    (hi_void)memcpy_s(frame.dst, sizeof(frame.dst), mac, HI_PLC_MAC_ADDR_LEN);

    (hi_void)memset_s(&plc, sizeof(plc), 0, sizeof(plc));
    plc.usId = ID_MRS_CMD_GET_METERLIST;
    plc.pucPayload = (hi_u8 *)&frame;
    plc.usPayloadLen = frame.stru_len;
    (hi_void)memcpy_s(plc.ucMac, sizeof(plc.ucMac), frame.dst, sizeof(frame.dst));

    return MRS_SendPlcFrame(&plc);
}

// ��ȡNV���е�ӳ���ά��ʹ�ܿ��غͷ��Ͷ����ַPLC֡ʱ����
HI_VOID mrsGetNvMacMapInfo(MRS_MAP_MODULE *module)
{
    HI_U32 ret = HI_ERR_SUCCESS;
    NV_APP_MAC_MAP_INFO_STRU stNvMapInfo = {0};
    HI_PRV HI_BOOL bFirst = HI_FALSE;

    ret = HI_MDM_NV_Read(ID_NV_APP_MAC_MAP_INFO,&stNvMapInfo
        ,sizeof(NV_APP_MAC_MAP_INFO_STRU));
    if (ret != HI_ERR_SUCCESS)
    {
        stNvMapInfo.ucMapEnable = HI_TRUE; //CCO��ӳ���ά��ʹ�ܿ���Ĭ�ϴ�
        stNvMapInfo.usSendMacPlcTime = MRS_MAC_MAP_SEND_PLC_TIME_DEFAULT;
        stNvMapInfo.usSendLastMacPlcTime = MRS_MAC_MAP_SEND_LAST_PLC_TIME_DEFAULT;
        stNvMapInfo.max_ttl = MRS_MAC_MAP_TTL_DEFAULT;
        stNvMapInfo.check_ttl_time = MRS_MAC_MAP_CHECK_TTL_TIME_DEFAULT;
        stNvMapInfo.max_parallel_num = MRS_MAC_MAP_SEND_PLC_NUM_DEFAULT;
        stNvMapInfo.max_retry_num = MRS_MAC_MAP_SEND_PLC_RETRY_DEFAULT; //���ֵ��2��Ҫ��Ҫ��Ϊ5?
        stNvMapInfo.map_turn_time = MRS_MAC_MAP_REFRESH_TIME_DEFAULT;
        stNvMapInfo.max_turn_num = MRS_MAC_MAP_MAX_REFRESH_NUM_DEFAULT;
        stNvMapInfo.ucLevelFlag_ucMaxMrsTtl = HI_FALSE;
    }
    
    MRS_DATA_VALID_CHECK(stNvMapInfo.usSendMacPlcTime,
                            MRS_MAC_MAP_SEND_PLC_TIME_MAX,
                            MRS_MAC_MAP_SEND_PLC_TIME_DEFAULT,
                            MRS_MAC_MAP_SEND_PLC_TIME_MIN);
    
    MRS_DATA_VALID_CHECK(stNvMapInfo.usSendLastMacPlcTime,
                            MRS_MAC_MAP_SEND_LAST_PLC_TIME_MAX,
                            MRS_MAC_MAP_SEND_LAST_PLC_TIME_DEFAULT,
                            MRS_MAC_MAP_SEND_LAST_PLC_TIME_MIN);
 
    MRS_DATA_VALID_CHECK(stNvMapInfo.max_parallel_num,
                            MRS_MAC_MAP_SEND_PLC_NUM_MAX,
                            MRS_MAC_MAP_SEND_PLC_NUM_DEFAULT,
                            MRS_MAC_MAP_SEND_PLC_NUM_MIN);
    
    MRS_DATA_VALID_CHECK(stNvMapInfo.max_retry_num,
                            MRS_MAC_MAP_SEND_PLC_RETRY_MAX,
                            MRS_MAC_MAP_SEND_PLC_RETRY_DEFAULT,
                            MRS_MAC_MAP_SEND_PLC_RETRY_MIN);
    
    MRS_DATA_VALID_CHECK(stNvMapInfo.max_ttl,
                            MRS_MAC_MAP_TTL_MAX,
                            MRS_MAC_MAP_TTL_DEFAULT,
                            MRS_MAC_MAP_TTL_MIN);

    MRS_DATA_VALID_CHECK(stNvMapInfo.check_ttl_time,
                            MRS_MAC_MAP_CHECK_TTL_TIME_MAX,
                            MRS_MAC_MAP_CHECK_TTL_TIME_DEFAULT,
                            MRS_MAC_MAP_CHECK_TTL_TIME_MIN);
    
    module->ucMapEnable = stNvMapInfo.ucMapEnable;
    module->usSendMacPlcTime = stNvMapInfo.usSendMacPlcTime;
    module->usSendLastMacPlcTime = stNvMapInfo.usSendLastMacPlcTime;

    module->ucMaxParallelnum = (HI_U8)stNvMapInfo.max_parallel_num;
    module->ucMaxRetryNum = (HI_U8)stNvMapInfo.max_retry_num;

    module->usCheckTtlTime = stNvMapInfo.check_ttl_time;
    module->map_turn_time = stNvMapInfo.map_turn_time;
    module->max_turn_num = stNvMapInfo.max_turn_num;

    module->usMaxTtl = (HI_U16)stNvMapInfo.max_ttl;

    if (HI_FALSE == bFirst)
    {
        module->ucCltMapEnable = stNvMapInfo.ucCltMapEnable;
        bFirst = HI_TRUE;
    }

    mrsCcoGetContext()->ucLevelFlag = (stNvMapInfo.ucLevelFlag_ucMaxMrsTtl == 1) ? MRS_CCO_LEVEL_ENABLE : MRS_CCO_LEVEL_DISABLE;
}

// CCO״̬�ӽ���ӳ���״̬����������״̬�����еĹ�������
HI_VOID mrsCcoMapToNormalPro(HI_VOID)
{
/* BEGIN: PN: DTSYYYYMMDDXXXXX MODIFY\ADD\DEL by cuiate/00233580 at 2015/6/24 */
    MRS_CCO_SRV_CTX_STRU * cco_ctx = mrsCcoGetContext();
    HI_U16 usCheckTtlTime = 0;
    HI_U32 ulStatus = mrsCcoGetState();

/* BEGIN: Modified by fengxiaomin/00209182, 2015/9/7   ���ⵥ�ź�:DTS2015091503701 */
    if ((HI_FALSE == cco_ctx->bPauseFlag) && (0 == (ulStatus & MRS_CCO_STATUS_ANYTIME_READMETER)))
    {
        if (ulStatus & MRS_CCO_STATUS_SEARCH_METER_TRANSFORMER)
        {
            mrsCcoSetState(MRS_CCO_STATUS_NORMAL | MRS_CCO_STATUS_SEARCH_METER_TRANSFORMER);
        }
        else
        {
            mrsCcoSetState(MRS_CCO_STATUS_NORMAL);
        }
        
        HI_DIAG_LOG_MSG_E0(MRS_FILE_LOG_FLAG_007, HI_DIAG_MT("Map Complete, Notify LR"));

        mrsReadMeterStateInit();    // ���¿�ʼ��������
        // ��ʱMRS_SRV_CCO_TIMER_READMETER_ACTIV_VALUEʱ������¼������ģ��
        MRS_StartTimer(EN_MRS_SRV_CCO_READMETER_ACTIV_TIMER,
                            MRS_SRV_CCO_TIMER_READMETER_ACTIV_VALUE,
                            HI_SYS_TIMER_ONESHOT);
    }
/* END:   Modified by fengxiaomin/00209182, 2015/9/7 */
    else
    {
        ulStatus &= ~MRS_CCO_STATUS_CREATE_MAP;
        mrsCcoSetState(ulStatus); 
    }
/* END:   PN: DTSYYYYMMDDXXXXX MODIFY\ADD\DEL by cuiate/00233580 at 2015/6/24 */
    
    usCheckTtlTime = cco_ctx->stMapModule.usCheckTtlTime;

    //������鴦��ӳ����и���ӳ���ϵ��TTL���ڶ�ʱ��
    MRS_StartTimer(EN_MRS_SRV_CCO_CHECK_MAC_MAP_TTL_TIMER,
                        usCheckTtlTime * 1000,
                        HI_SYS_TIMER_PERIODIC);

    return;
}


HI_VOID mrsMapTtlDecrease(MRS_MAC_DEVICE_MAP *pstMap)
{
    MRS_MAC_DEVICE_PAIR *pItem = pstMap->pstItem;
    HI_U16 usIndex = 0;

    if (!pItem)
    {
        return;
    }

    for (usIndex = 0; usIndex < pstMap->usMapSize; usIndex++, pItem++)
    {
        if (pItem->usTTL > 0)
        {
            pItem->usTTL--;
        }
    }
}


// ӳ�����TTL��ʱ������
HI_VOID mrsMapTtlTimeoutFunc(MRS_MAP_MODULE *module)
{
    mrsMapTtlDecrease(&module->stMacMeterMap);
    mrsMapTtlDecrease(&module->stMacCltMap);
}


HI_VOID mrsMapTtlZeroProc(MRS_MAC_DEVICE_MAP *pstMap)
{
    MRS_MAC_DEVICE_PAIR *pItem = pstMap->pstItem;
    HI_U16 usIndex = 0;

    if (!pItem)
    {
        return;
    }

    for (usIndex = 0; usIndex < pstMap->usMapSize; usIndex++, pItem++)
    {
        pItem->usTTL = 0;
    }
}


HI_VOID mrsMapTtlZero(MRS_MAP_MODULE *pstModule)
{
    mrsMapTtlZeroProc(&pstModule->stMacMeterMap);
    mrsMapTtlZeroProc(&pstModule->stMacCltMap);
}


// ӳ����ϻ���ͣ
HI_VOID mrsMapAgingPause(HI_VOID)
{
    MRS_StopTimer(EN_MRS_SRV_CCO_CHECK_MAC_MAP_TTL_TIMER);
}


// ӳ����ϻ��ָ�
HI_VOID mrsMapAgingResume(HI_VOID)
{
    MRS_CCO_SRV_CTX_STRU *pstCtx = mrsCcoGetContext();

    MRS_StartTimer(EN_MRS_SRV_CCO_CHECK_MAC_MAP_TTL_TIMER, 
        pstCtx->stMapModule.usCheckTtlTime * 1000, 
        HI_SYS_TIMER_PERIODIC);
}


// ����ӳ�����Ŀ
HI_U32 mrsRefreshMapItem(MRS_MAC_DEVICE_MAP *pstMap, HI_S16 sIndex, HI_U8 *pucMacAddr, MRS_DEVICE_INF_STRU *pstDevInf, HI_U8 ucPhase)
{
    MRS_MAC_DEVICE_PAIR * pItem = pstMap->pstItem; 
    
    if (!pstDevInf || !pucMacAddr || mrsToolsAbnormalMac(pucMacAddr)) 
    {
        return HI_ERR_INVALID_PARAMETER;
    }

    if (!pItem || sIndex < 0 || sIndex >= pstMap->usMapSize) 
    {
        return HI_ERR_INVALID_PARAMETER;
    }

    pItem += sIndex;
   (hi_void)memcpy_s(pItem->aucMacAddr, HI_PLC_MAC_ADDR_LEN, pucMacAddr, HI_PLC_MAC_ADDR_LEN);
    mrsSetCcoMapDeviceInf(&pItem->stDeviceInf, pstDevInf);
    pItem->usTTL = mrsMapGetMaxTTL();
    pItem->usMrTTL = MRS_MAP_MAX_MR_TTL;
    pItem->bValid = HI_TRUE;
    pItem->usPhase = ucPhase;

    return HI_ERR_SUCCESS;
}

HI_VOID mrsSetCcoMapDeviceInf(MRS_CCO_DEVICE_INF_STRU *pstMapDevice, MRS_DEVICE_INF_STRU *pstDevInf)
{
   (hi_void)memcpy_s(pstMapDevice->aucAddr, HI_METER_ADDR_LEN, pstDevInf->aucAddr, HI_METER_ADDR_LEN);
    pstMapDevice->ucProtocol = pstDevInf->ucProtocol;
    pstMapDevice->ucStatus = pstDevInf->ucStatus;
    pstMapDevice->ucType = pstDevInf->ucType;
}

HI_VOID mrsMeterSetDevInf(MRS_DEVICE_INF_STRU *pstDevInf, MRS_METER_INF_STRU *pstMeterInf)
{
   (hi_void)memcpy_s(pstDevInf->aucAddr, HI_METER_ADDR_LEN, pstMeterInf->aucAddr, HI_METER_ADDR_LEN);
    pstDevInf->ucProtocol = pstMeterInf->ucProtocol;
    pstDevInf->ucType = pstMeterInf->ucType;
}

// ˢ��NewMacList�б�
HI_VOID mrsMapRefreshNewMacList(MRS_MAC_LIST *pstMacList, 
                                            HI_MAC_NETWORK_TOPO_ENTRY_S *pstTopo,
                                            HI_U16 ucTopoNum)
{
    if((!pstMacList) || (!pstTopo))
    {
        return;
    }
    
    pstMacList->number = 0;
    
    //����TOPO��new mac list��ȥ
    for (pstMacList->number = 0; pstMacList->number < ucTopoNum; pstMacList->number++)
    {
        HI_S16 usNum = pstMacList->number;
        
       (hi_void)memcpy_s(pstMacList->mac[usNum].ucMacAddr, HI_PLC_MAC_ADDR_LEN, pstTopo[usNum].mac, HI_PLC_MAC_ADDR_LEN);
        pstMacList->mac[usNum].report  = MRS_MAP_REPORT_IDLE;
        pstMacList->mac[usNum].retries = 0;
    }

    pstMacList->current = 0;

    return;
}

// �жϵ�ǰMac��ַ�Ƿ������ڵ��
HI_BOOL mrsMapMacIsFromMeter(HI_U8 *pucMac)
{
    return mrsToolsIsMacAddrFromMeter(pucMac);
}


// �ж�Ŀ��վ��Ϊ�ɼ�����Ʒ��̬
HI_BOOL mrsMapMacIsCollector(HI_U8 ucType)
{
    return (HI_BOOL)((HI_ND_TYPE_CLTI == ucType) || (HI_ND_TYPE_CLTII == ucType));
}


// ӳ�����ٴ���
// ����MAC��ַ��������ӳ���
HI_U32 mrsMapFastCreate(MRS_MAP_MODULE *module, 
                                HI_MAC_NETWORK_TOPO_ENTRY_S *pstTopo)
{
    MRS_MAC_LIST *pstMacList = HI_NULL;
    HI_S32 i = 0;
    HI_U8 ucType = HI_ND_TYPE_STA;
    
    if((!module) || (!pstTopo))
    {
        return HI_ERR_FAILURE;
    }
    
    pstMacList = &module->stNewMacList;

    HI_DIAG_LOG_MSG_E1(MRS_FILE_LOG_FLAG_009, HI_DIAG_MT("Map Fast Create Start..."), (HI_U16)pstMacList->number);

    for(i = 0; i < pstMacList->number; i++)
    {
        MRS_MAC_INFO *pstMacInf = &pstMacList->mac[i];

        if(MRS_MAP_REPORT_SUCCESS == pstMacInf->report)
        {
            continue;
        }

        ucType = pstTopo[i].product_type;
        
        if((HI_TRUE == mrsMapMacIsFromMeter(pstMacInf->ucMacAddr))
           && (HI_FALSE == mrsMapMacIsCollector(ucType))) 
        {
            MRS_DEVICE_INF_STRU stMeterInf;
            HI_U8 ucPhase = 0;

            (hi_void)memset_s(&stMeterInf, sizeof(stMeterInf), 0, sizeof(stMeterInf));

           (hi_void)memcpy_s(stMeterInf.aucAddr, HI_PLC_MAC_ADDR_LEN, pstMacInf->ucMacAddr, HI_PLC_MAC_ADDR_LEN);
            mrsHexInvert(stMeterInf.aucAddr, HI_PLC_MAC_ADDR_LEN);

            MRS_TOOLS_FE_TO_00(stMeterInf.aucAddr[0]);
            
            stMeterInf.ucProtocol = METER_PROTO_645_2007;
            stMeterInf.ucType = MRS_MAP_DEV_TYPE_METER;

            ucPhase = mrsGetPhaseFromTopo(pstTopo[i].phase_state, pstTopo[i].phase_result);
            // ���ӳ���ϵ
            mrsMapAddMeter(module, pstMacInf->ucMacAddr, &stMeterInf, ucPhase);
            mrsMapMarkMac(module, pstMacInf->ucMacAddr, MRS_SEARCH_STATUS_IDLE);

            // ��ӳɹ�-���ϱ���־(ͳһɾ��)
            pstMacInf->report = MRS_MAP_REPORT_SUCCESS;
        }
    }

    HI_DIAG_LOG_MSG_E1(MRS_FILE_LOG_FLAG_010, HI_DIAG_MT("Map Fast Create End..."), (HI_U16)pstMacList->number);
    
    return HI_ERR_SUCCESS;
}

// ��������NewMacList�б�(�ų���λ)
HI_VOID mrsMapReformingNewMacList(MRS_MAC_LIST *pstMacList)
{
    HI_S16 start = 0;
    HI_S16 end = 0;

    if((!pstMacList) || (pstMacList->number <= 0))
    {
        return;
    }

    HI_DIAG_LOG_MSG_E1(MRS_FILE_LOG_FLAG_011, HI_DIAG_MT("Reforming NewMacList Start..."), (HI_U16)pstMacList->number);

    start = 0;
    end = pstMacList->number - 1;

    while(start < end)
    {
        if (MRS_MAP_REPORT_SUCCESS != pstMacList->mac[start].report)
        {
            // ��ǰ����ҵ���λ
            start++;
        }
        else if (MRS_MAP_REPORT_SUCCESS == pstMacList->mac[end].report)
        {
            // �ɺ���ǰ�ҵ���Ч�ڵ�
            end--;
            pstMacList->number--;
        }
        else
        {
            // ��β����һ����Ч�ڵ��滻ǰ����һ����λ
           (hi_void)memcpy_s(&pstMacList->mac[start], sizeof(MRS_MAC_INFO), &pstMacList->mac[end], sizeof(MRS_MAC_INFO));
            (hi_void)memset_s(&pstMacList->mac[end--], sizeof(MRS_MAC_INFO), 0, sizeof(MRS_MAC_INFO));
            pstMacList->number--;
        }
    }

    if (MRS_MAP_REPORT_SUCCESS == pstMacList->mac[start].report)
    {
        (hi_void)memset_s(&pstMacList->mac[start], sizeof(MRS_MAC_INFO), 0, sizeof(MRS_MAC_INFO));
        pstMacList->number--;
    }

    HI_DIAG_LOG_MSG_E1(MRS_FILE_LOG_FLAG_012, HI_DIAG_MT("Reforming NewMacList End..."), (HI_U16)pstMacList->number);

    return;
}


HI_U16 mrsMapGetMaxTTL(HI_VOID)
{
    return mrsCcoGetContext()->stMapModule.usMaxTtl;
}


MRS_MAC_DEVICE_PAIR * mrsFindMeterInMap(MRS_MAC_DEVICE_MAP *pstMap, HI_U8 *pucDeviceAddr)
{
    MRS_MAC_DEVICE_PAIR *pstItem = pstMap->pstItem;
    HI_U16 i = 0;

    if (!pstItem)
    {
        return HI_NULL;
    }

    for (i = 0; i < pstMap->usMapSize; i++, pstItem++)
    {
        if (pstItem->bValid && mrsToolsMemEq(pstItem->stDeviceInf.aucAddr, pucDeviceAddr, HI_METER_ADDR_LEN))
        {
            return pstItem;
        }
    }

    return HI_NULL;
}


//*****************************************************************************
// ��������: mrsIsMacInMap
// ��������: �ж�mac��ַ�Ƿ���ӳ�����,����״̬���
//           
// ����˵��:
//   [IN]  pstMap     ӳ���
//   [IN]  pucMac     MAC��ַ
//   [OUT] pucStatus  ״̬
//
// �� �� ֵ: 
//   HI_TRUE   mac��ӳ�����
//   HI_FALSE  mac����ӳ�����
//
// ����Ҫ��: 
// ���þ���: 
// ��    ��: fengxiaomin/00209182 [2016-03-18]
//*****************************************************************************
HI_BOOL mrsIsMacInMap(MRS_MAC_DEVICE_MAP *pstMap, HI_U8 *pucMac, HI_OUT HI_U8 *pucStatus)
{
    MRS_MAC_DEVICE_PAIR *pItem = pstMap->pstItem;
    HI_U16 i = 0;

    if (!pItem)
    {
        return HI_FALSE;
    }

    for (i = 0; i < pstMap->usMapSize; i++, pItem++)
    {
        if (pItem->bValid && mrsToolsMemEq(pItem->aucMacAddr, pucMac, HI_PLC_MAC_ADDR_LEN))
        {
            *pucStatus = pItem->stDeviceInf.ucStatus;
            return HI_TRUE;
        }
    }

    return HI_FALSE;
}

//*****************************************************************************
// ��������: mrsIsMacInMaclist
// ��������: �ж�mac��ַ�Ƿ���maclist��,����״̬���
//           
// ����˵��:
//   [IN]  pstMaclist  maclist�б�
//   [IN]  pucMac      MAC��ַ
//   [OUT] pucReport   �ϱ���־
//
// �� �� ֵ: 
//   HI_TRUE   mac��maclist��
//   HI_FALSE  mac����maclist��
//
// ����Ҫ��: 
// ���þ���: 
// ��    ��: fengxiaomin/00209182 [2016-03-18]
//*****************************************************************************
HI_BOOL mrsIsMacInMaclist(MRS_MAC_LIST *pstMaclist, HI_U8 *pucMac, HI_OUT HI_U8 *pucReport)
{
    MRS_MAC_INFO *pItem = pstMaclist->mac;
    HI_U16 i = 0;

    if (!pItem)
    {
        return HI_FALSE;
    }

    for (i = 0; i < pstMaclist->number; i++, pItem++)
    {
        if (mrsToolsMemEq(pItem->ucMacAddr, pucMac, HI_PLC_MAC_ADDR_LEN))
        {
            *pucReport = pItem->report;
            return HI_TRUE;
        }
    }

    return HI_FALSE;
}

//*****************************************************************************
// ��������: mrsGetMacStatus
// ��������: �����ϱ���־��ȡ״̬
//           
// ����˵��:
//   [IN] pucReport   �ϱ���־
//
// �� �� ֵ: 
//   ״̬
//
// ����Ҫ��: 
// ���þ���: 
// ��    ��: fengxiaomin/00209182 [2016-03-18]
//*****************************************************************************
HI_U8 mrsGetMacStatus(HI_U8 ucReport)
{
    HI_U8 ucStatus = MRS_SEARCH_STATUS_UNKNOWN;
    
    switch (ucReport)
    {
        case MRS_MAP_REPORT_SUCCESS:
            ucStatus = MRS_SEARCH_STATUS_IDLE;
            break;
            
        case MRS_MAP_REPORT_CONTINUE:
            ucStatus = MRS_SEARCH_STATUS_IN_PROGRESS;
            break;

        default :
            break;
    }

    return ucStatus;
}

//*****************************************************************************
// ��������: mrsSetDeviceStatus
// ��������: �����豸״̬
//           
// ����˵��:
//   [IN]  pucMac         MAC��ַ
//   [OUT] pucStatus      ״̬
//
// �� �� ֵ: 
//   HI_ERR_SUCCESS  �ɹ�
//
// ����Ҫ��: 
// ���þ���: 
// ��    ��: fengxiaomin/00209182 [2016-03-18]
//*****************************************************************************
HI_U32 mrsSetDeviceStatus(HI_U8 *pucMac, HI_U8 *pucStatus)
{
    HI_U32 ulRet = HI_ERR_SUCCESS;
    HI_MAC_NETWORK_TOPO_ENTRY_S stTopoEntry;
    MRS_MAP_MODULE *pstMapModule = &mrsCcoGetContext()->stMapModule; 

    (hi_void)memset_s(&stTopoEntry, sizeof(stTopoEntry), 0, sizeof(stTopoEntry));
    ulRet = HI_MDM_QueryMacAttr(pucMac, &stTopoEntry);
    if (HI_ERR_SUCCESS != ulRet)
    {
        return ulRet;
    }
        
    if ((HI_ND_TYPE_STA == stTopoEntry.product_type) || (HI_ND_TYPE_THREE_STA == stTopoEntry.product_type))
    {
        *pucStatus = MRS_SEARCH_STATUS_IDLE;
    }
    else
    {
        HI_U8 ucReport = MRS_MAP_REPORT_IDLE;
        
        if (mrsIsMacInMaclist(&pstMapModule->stNewMacList, pucMac, &ucReport))
        {
            *pucStatus = mrsGetMacStatus(ucReport);
        }
        else
        {
            HI_U8 ucStatus = 0;
            
            if (mrsIsMacInMap(&pstMapModule->stMacMeterMap, pucMac, &ucStatus)
                || mrsIsMacInMap(&pstMapModule->stMacCltMap, pucMac, &ucStatus))
            {
                *pucStatus = ucStatus;
            }
            else
            {
                *pucStatus = MRS_SEARCH_STATUS_UNKNOWN;
            }
        }
    }

    return ulRet;
}

//*****************************************************************************
// ��������: mrsSetDeviceInfo
// ��������: �����豸��Ϣ
//           
// ����˵��:
//   [IN]  pstFrame   645֡��Ϣ
//   [OUT] pstDevice  �豸��Ϣ
//
// �� �� ֵ: 
//   ��
//
// ����Ҫ��: 
// ���þ���: 
// ��    ��: fengxiaomin/00209182 [2016-03-18]
//*****************************************************************************
HI_U32 mrsSetDeviceInfo(MRS_645_FRAME_STRU *pstFrame, HI_U8 *pucMac, MRS_DEVICE_INF_STRU *pstDevice)
{
    HI_U32 ulRet = HI_ERR_SUCCESS;
    HI_U8 ucStatus = 0;
    
    (hi_void)memset_s(pstDevice, sizeof(MRS_DEVICE_INF_STRU), 0, sizeof(MRS_DEVICE_INF_STRU));
    pstDevice->ucProtocol = pstFrame->ucVer;
    pstDevice->ucType = MRS_MAP_DEV_TYPE_METER;
    pstDevice->ucStatus = MRS_SEARCH_STATUS_UNKNOWN;
    mrsMeterAddrULPreProc(pstDevice->aucAddr, pstFrame->ucAddr, pstFrame->ucVer, pstFrame->stCtrl.ucDir * MRS_645_FRAME_CONTROL_DIR_UP);

    ulRet = mrsSetDeviceStatus(pucMac, &ucStatus);
    if (HI_ERR_SUCCESS != ulRet)
    {
        return ulRet;
    }
    
    pstDevice->ucStatus = ucStatus;

    return HI_ERR_SUCCESS;
}

//*****************************************************************************
// ��������: mrsRmMapProc
// ��������: ����Ӧ����ӳ���ϵ����
//           
// ����˵��:
//   [IN]  pstFrame   645֡��Ϣ
//   [IN]  pstModule  ӳ���ģ��
//   [IN]  pucMac     MAC��ַ
//
// �� �� ֵ: 
//   ��
//
// ����Ҫ��: 
// ���þ���: 
// ��    ��: fengxiaomin/00209182 [2016-03-18]
//*****************************************************************************
HI_U32 mrsRmMapProc(MRS_645_FRAME_STRU *pstFrame, MRS_MAP_MODULE *pstModule, HI_U8 *pucMac, HI_U8 ucPhase)
{
    HI_U32 ulRet = HI_ERR_SUCCESS;
    MRS_DEVICE_INF_STRU stDevice;

    ulRet = mrsSetDeviceInfo(pstFrame, pucMac, &stDevice);
    if (HI_ERR_SUCCESS != ulRet)
    {
        return ulRet;
    }

    //���µ�ǰ��ӳ���
    mrsMapAddMeter(pstModule, pucMac, &stDevice, ucPhase);
    mrsMapMarkMac(pstModule, pucMac, stDevice.ucStatus);

    return ulRet;
}

HI_VOID mrsMapSyncMacList(MRS_MAC_DEVICE_MAP *pstMap, MRS_MAC_LIST *pstMacList)
{
    MRS_MAC_DEVICE_PAIR *pItem = pstMap->pstItem;
    HI_U16 i = 0;

    if (!pItem || !pstMacList->mac)
    {
        return;
    }

    for (i = 0; i < pstMap->usMapSize; i++, pItem++)
    {
        MRS_MAC_INFO *pMacInf = pstMacList->mac;
        HI_U16 j = 0;

        if (!pItem->bValid)
        {
            continue;
        }

        for (j = 0; j < pstMacList->number; j++, pMacInf++)
        {
            if (mrsToolsMemEq(pItem->aucMacAddr, pMacInf->ucMacAddr, HI_PLC_MAC_ADDR_LEN))
            {
                break;
            }
        }

        if (j < pstMacList->number)
        {
            if (MRS_SEARCH_STATUS_IDLE == pItem->stDeviceInf.ucStatus)
            {
                pMacInf->report = MRS_MAP_REPORT_SUCCESS;
            }
        }
        else
        {
            (hi_void)memset_s(pItem, sizeof(MRS_MAC_DEVICE_PAIR), 0, sizeof(MRS_MAC_DEVICE_PAIR));
            pstMap->usNumber--;
        }
    }
}

//*****************************************************************************
// ��������: mrsGetDeviceAddr
// ��������: ����mac��ȡ�豸��ַ
//
// ����˵��:
//   pucMacAddr[IN]         MAC��ַ
//   ucType[IN]             ��Ʒ����
//   pucDeviceAddr[OUT]     �豸��ַ
// �� �� ֵ:
//    �ɹ�����  HI_ERR_SUCCESS
//
// ����Ҫ��:
// ���þ���:
// ��    ��: fengxiaomin/00209182 [2015-12-08]
//*****************************************************************************
HI_U32 mrsGetDeviceAddr(HI_U8 pucMacAddr[], HI_U8 ucType, HI_U8 pucDeviceAddr[])
{
    HI_U32 ulRet = HI_ERR_SUCCESS;
    
    switch (ucType)
    {
        case HI_ND_TYPE_CCO:
           (hi_void)memcpy_s(pucDeviceAddr, HI_METER_ADDR_LEN, mrsCcoGetContext()->ucMainNodeAddr, HI_METER_ADDR_LEN);
            mrsHexInvert(pucDeviceAddr, HI_METER_ADDR_LEN);
            break;
            
        case HI_ND_TYPE_STA:
        case HI_ND_TYPE_THREE_STA:
            if (mrsToolsIsMacAddrFromMeter(pucMacAddr))
            {
               (hi_void)memcpy_s(pucDeviceAddr, HI_METER_ADDR_LEN, pucMacAddr, HI_METER_ADDR_LEN);
                mrsHexInvert(pucDeviceAddr, HI_METER_ADDR_LEN);
            }
            else
            {
                ulRet = HI_ERR_FAILURE;
            }
            break;
            
        case HI_ND_TYPE_CLTI:
        case HI_ND_TYPE_CLTII:
            ulRet = mrsMapGetDeviceAddr(pucMacAddr, pucDeviceAddr);
            break;
            
        default:
            ulRet = HI_ERR_FAILURE;
            break;
    }

    return ulRet;
}

//*****************************************************************************
// ��������: mrsGetDeviceAddr
// ��������: �Ӳɼ���ӳ���ϵ�и���mac��ַ��ȡ�ʲ���
//
// ����˵��:
//   pucMacAddr[IN]         MAC��ַ
//   pucDeviceAddr[OUT]     �豸��ַ
// �� �� ֵ:
//    �ɹ�����  HI_ERR_SUCCESS
//
// ����Ҫ��:
// ���þ���:
// ��    ��: fengxiaomin/00209182 [2015-12-08]
//*****************************************************************************
HI_U32 mrsMapGetDeviceAddr(HI_U8 pucMacAddr[], HI_U8 pucDeviceAddr[])
{
    MRS_MAC_DEVICE_MAP *pstMacMap = &(mrsCcoGetContext()->stMapModule.stMacCltMap);
    MRS_MAC_DEVICE_PAIR *pItem = pstMacMap->pstItem;
    HI_U16 usIndex = 0;

    if (!pItem)
    {
        return HI_ERR_FAILURE;
    }

    for (usIndex = 0; usIndex < pstMacMap->usMapSize; usIndex++, pItem++)
    {
        if (pItem->bValid && mrsToolsMemEq(pItem->aucMacAddr, pucMacAddr, HI_PLC_MAC_ADDR_LEN))
        {
           (hi_void)memcpy_s(pucDeviceAddr, HI_METER_ADDR_LEN, pItem->stDeviceInf.aucAddr, HI_METER_ADDR_LEN);
            return HI_ERR_SUCCESS;
        }
    }

    return HI_ERR_NOT_FOUND;
}


HI_VOID mrsDecreaseMrTTL(MRS_MAP_MODULE *pstModule, HI_U8 *pucDeviceAddr)
{
    MRS_MAC_DEVICE_PAIR * pstPair = HI_NULL;
    MRS_MAC_DEVICE_MAP *pstCurMap = 0;

    pstCurMap = &pstModule->stMacMeterMap;
    pstPair = mrsFindMeterInMap(&pstModule->stMacMeterMap, pucDeviceAddr);
    if (!pstPair)
    {
        pstCurMap = &pstModule->stMacMeterMap;
        pstPair = mrsFindMeterInMap(&pstModule->stMacCltMap, pucDeviceAddr);
        if (!pstPair)
        {
            return;
        }
    }

    if (pstPair->usMrTTL > 0)
    {
        pstPair->usMrTTL--;
    }

    if (0 == pstPair->usMrTTL)
    {
        mrsDeleteDeviceFromMap(pstCurMap, pucDeviceAddr);
    }
}


HI_U8 mrsMapGetLevel(HI_U8 *pucDeviceAddr, HI_U8 aucMac[HI_PLC_MAC_ADDR_LEN])
{
    HI_U8 layer = MRS_MAX_RELAY_LEVEL;
    
    HI_U32 ulRet = HI_ERR_FAILURE;
    HI_U8 aucSrcAddr[HI_METER_ADDR_LEN] = {0};
    HI_U8 aucMacAddr[HI_PLC_MAC_ADDR_LEN] = {0};

    HI_MAC_NETWORK_TOPO_ENTRY_S stTopoEntry;
    MRS_CCO_SRV_CTX_STRU *pstCcoCtx = mrsCcoGetContext();

    if (pstCcoCtx->ucLevelFlag == MRS_CCO_LEVEL_DISABLE)
    {
        layer = 0;
        return layer;
    }
    
    if (pucDeviceAddr == HI_NULL)
    {
        HI_DIAG_LOG_MSG_E0(__LINE__, HI_DIAG_MT("[LEVEL]device null"));        
        return layer;
    }
    
   (hi_void)memcpy_s(aucSrcAddr, HI_METER_ADDR_LEN,pucDeviceAddr,HI_METER_ADDR_LEN);
    ulRet = mrsGetMacFromMap(&pstCcoCtx->stMapModule.stMacMeterMap, aucSrcAddr, aucMacAddr);
    if (HI_ERR_SUCCESS != ulRet)
    {   
        if (aucMac == HI_NULL)
        {
            HI_DIAG_LOG_MSG_E0(__LINE__, HI_DIAG_MT("[LEVEL]get map err"));        
            return layer;
        }
        
       (hi_void)memcpy_s(aucMacAddr, HI_METER_ADDR_LEN, aucMac, HI_PLC_MAC_ADDR_LEN);
    }

    (hi_void)memset_s(&stTopoEntry, sizeof(stTopoEntry), 0, sizeof(stTopoEntry));
    ulRet = HI_MDM_QueryMacAttr(aucMacAddr, &stTopoEntry);
    if (ulRet != HI_ERR_SUCCESS)
    {
        HI_DIAG_LOG_MSG_E0(MRS_FILE_LOG_FLAG_060, HI_DIAG_MT("get mac info fail"));
        return layer;
    }
    
    layer = stTopoEntry.level;

    if ((layer >= 1) && (layer <= MRS_MAX_RELAY_LEVEL))
    {
        return (HI_U8)(layer - 1);
    }

    return MRS_MAX_RELAY_LEVEL;
}

HI_U32 mrsRefreshPhaseCallBack(HI_U32 state)
{
    MRS_MAC_DEVICE_MAP *pstMacMap = &(mrsCcoGetContext()->stMapModule.stMacMeterMap);
    MRS_MAC_DEVICE_PAIR *pItem = pstMacMap->pstItem;
    HI_U16 usIndex = 0, usIndex1 = 0;
    MRS_CCO_SRV_CTX_STRU * pstCcoCtx = mrsCcoGetContext();

	HI_UNREF_PARAM(state);

    if (!pItem)
    {
        return HI_ERR_FAILURE;
    }

    mrsRefreshTopoInfo(pstCcoCtx);
    for (usIndex = 0; usIndex < pstMacMap->usMapSize; usIndex++, pItem++)
    {
        HI_MAC_NETWORK_TOPO_ENTRY_S *pstEntry = pstCcoCtx->stTopoInf.entry;

        if (!pItem->bValid)
        {
            continue;
        }

        for (usIndex1 = 1; usIndex1 < pstCcoCtx->stTopoInf.num; usIndex1++)
        {
            if (mrsToolsMemEq(pItem->aucMacAddr, pstEntry[usIndex1].mac, HI_PLC_MAC_ADDR_LEN))
            {
                pItem->usPhase = mrsGetPhaseFromTopo(pstEntry[usIndex1].phase_state, pstEntry[usIndex1].phase_result);
                break;
            }
        }
    }

    return HI_ERR_SUCCESS;
}

HI_U8 mrsGetPhaseFromMap(HI_U8 *pMeter)
{
    HI_U8 ucPhase = 0;
    MRS_MAC_DEVICE_MAP *pstMacMap = &(mrsCcoGetContext()->stMapModule.stMacMeterMap);
    MRS_MAC_DEVICE_PAIR *pItem = pstMacMap->pstItem;
    HI_U16 usIndex = 0;

    if (!pItem)
    {
        return ucPhase;
    }

    for (usIndex = 0; usIndex < pstMacMap->usMapSize; usIndex++, pItem++)
    {
        if (!pItem->bValid)
        {
            continue;
        }

        if (mrsToolsMemEq(pMeter, pItem->stDeviceInf.aucAddr, HI_METER_ADDR_LEN))
        {
            ucPhase = pItem->usPhase;
            break;
        }
    }

    return ucPhase;
}

HI_U32 mrsGetMacFromTopo(HI_U8 *pucMeterAddr, HI_U8 *pucMacAddr)
{
    HI_MAC_NETWORK_TOPO_ENTRY_S stEntry;
    HI_U8 aucTemp[HI_PLC_MAC_ADDR_LEN] = {0};

    (hi_void)memset_s(&stEntry, sizeof(stEntry), 0, sizeof(stEntry));
    mrsToolsConvertMeter2Mac(pucMeterAddr, METER_PROTO_645_2007, aucTemp);

    if (HI_ERR_SUCCESS == HI_MDM_QueryMacAttr(aucTemp, &stEntry))
    {
       (hi_void)memcpy_s(pucMacAddr, HI_PLC_MAC_ADDR_LEN, stEntry.mac, HI_PLC_MAC_ADDR_LEN);
        return HI_ERR_SUCCESS;
    }

    return HI_ERR_NOT_FOUND;
}
HI_VOID mrsRefreshMapItem188(MRS_MAP_MODULE *module, HI_S16 index, HI_U8 mac[], HI_U8 uca188Mac[],HI_U8 ucType)
{
    MRS_MAC_DEVICE_PAIR * pItem = HI_NULL; 
    
    if (!module || !uca188Mac || !mac || mrsToolsAbnormalMac(mac)) 
    {
        return;
    }

    pItem = module->stMacMeterMap.pstItem;
    if (!pItem || index < 0 || index >= module->stMacMeterMap.usMapSize) 
    {
        return;
    }
	pItem += index;
    mrsToolsMemcpy_s(pItem->aucMacAddr,6,mac,6);
    mrsToolsMemcpy_s(pItem->stDeviceInf.aucAddr,6,uca188Mac,6);
	pItem->stDeviceInf.ucType = ucType;
	pItem->usTTL = mrsMapGetMaxTTL();
    pItem->usMrTTL = pItem->usTTL;
    pItem->bValid = HI_TRUE;
    
    return;
}
HI_U32 mrsMapAddMacMeter188(MRS_MAP_MODULE *module,HI_U8 mac[],HI_U8 uca188Mac[],HI_U8 ucType)
{
    MRS_MAC_DEVICE_PAIR *pItem = HI_NULL; 
    HI_U16 index = 0;
    HI_S16 sindex = -1;  //���е�λ��
    HI_S16 smindex = -1; //���е��λ��
    HI_S16 nonactindex = -1; //�ϻ����λ��
    
    if (!module || !uca188Mac || !mac || mrsToolsAbnormalMac(mac)) 
    {
        return HI_ERR_FAILURE;
    }

    pItem = module->stMacMeterMap.pstItem;
    if (!pItem) 
    {
        return HI_ERR_FAILURE;
    }
        
    for (index = 0 ;index < module->stMacMeterMap.usMapSize;index++,pItem++)
    {
        if (mrsToolsMemEq(pItem->aucMacAddr,mac,6)
         && mrsToolsMemEq(pItem->stDeviceInf.aucAddr,uca188Mac,6)) 
        {    
            //���New Mac List�д���,��־�Ѿ��ϱ���
            mrsMapMarkMac(module,mac,3);
            // ���µ���շ�֡ͳ����Ϣ
            pItem->stDeviceInf.ucType = ucType;
            pItem->usTTL = mrsMapGetMaxTTL();
			pItem->usMrTTL = pItem->usTTL;
            return HI_ERR_SUCCESS;
        }

        if ((sindex == -1) && mrsToolsInvalidMac(pItem->aucMacAddr))
        {
            sindex = (HI_S16)index;
        }

        if ((smindex == -1) && mrsToolsMemEq(pItem->stDeviceInf.aucAddr,uca188Mac,7))
        {
            smindex = (HI_S16)index;
            break;
        }

        if((!mrsToolsInvalidMac(pItem->aucMacAddr)))
        {
            if((nonactindex == -1) && (!pItem->usTTL))
            {
                nonactindex = (HI_S16)index;
            }
        }
    }

    //û���ҵ���ͬ�ĵ����û�п���λ��
    if ((smindex == -1) && (sindex == -1))
    {
        mrsRefreshMapItem188(module,nonactindex,mac,uca188Mac,ucType);
        return HI_ERR_FULL;
    }

    //���New Mac List�д���,��־�Ѿ��ϱ���
    mrsMapMarkMac(module,mac,3);
    if (smindex != -1)
    {        
        mrsRefreshMapItem188(module,smindex,mac,uca188Mac,ucType);
    }
    //û���ҵ���Ӧ�ĵ������������ӳ���ϵ
    else 
    {
        mrsRefreshMapItem188(module,sindex,mac,uca188Mac,ucType);
        //module->stMacMeterMap.number++;
    }

    return HI_ERR_SUCCESS;
}
#endif
