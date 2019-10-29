#include <hi_types.h>
#include <hi_mdm_types.h>
#include <hi_mdm_time.h>
#include <los_memory.h>
#include <hi_mem.h>
#include <hi_stdlib.h>
#include "hi_os_mem.h"
#include "hi_os_stat.h"
#include "hi_os_sys.h"
#include <hi_asm_func.h>
#include <hi_mdm.h>
#include <hi_sys_selfrepair.h>
#include <stdlib.h>

HI_MEM_POOL_MNGR_S g_stPoolMngr = {0,};

//��ȡģ�����ṹ
#define GET_POOL_MNGR()                             (&g_stPoolMngr)

//��ȡPOOL����ṹ
#define GET_POOL_ITEM(poolId)                       (&g_stPoolMngr.pstPoolItem[(poolId)])

//��ȡPOOL һ��ͳ�������ƽṹ(����ģ�������ͳ����,��ģ��ͳ�ƱȽ�ռ�ռ�)
#define GET_POOL_ITEM_COMMON_STAT(pstPoolItem)           (&pstPoolItem->stCmnStat)

//��ȡ��ģ��ͳ�ƽṹ,�ռ��㹻������չΪÿ���ڴ��һ��
#define GET_POOL_MOD_STAT()           (&g_stPoolMngr.stModItem)


#define GET_STEP_STAT()               (&g_stPoolMngr.stStepStat)

//mallocʧ�ܺͲ����Ƿ�ͳ����++
#define POOL_MALLOC_FAIL_TIMES_ERRTIMES(pst) do{pst->mallocFailTimes++;pst->mallocParamErrTimes++;}while(0)

//mallocʧ��ͳ����++
#define POOL_MALLOC_FAIL_TIMES(pst)   do{pst->mallocFailTimes++;}while(0)

//malloc�ɹ�ͳ����++
#define POOL_MALLOC_SUC_TIMES(pst,size)    do{pst->mallocSucTimes++;}while(0)

//malloc�ɹ�ͳ����++,���з�ֵͳ��
#define POOL_MALLOC_SUC_TIMES_PEEK_STAT(pst,size) do{\
        pst->mallocSucTimes++;\
        pst->usedSize+=size;\
        if(pst->usedSize>pst->peekSize)\
        {\
            pst->peekSize=pst->usedSize;\
            pst->peekSec=HI_MDM_GetSeconds();\
        }\
    }while(0)

//freeʧ�ܺͲ����Ƿ�ͳ����++
#define POOL_FREE_FAIL_TIMES_ERRTIMES(pst) do{pst->freeFailTimes++;pst->freeParamErrTimes++;}while(0)
    
//freeʧ��ͳ����++
#define POOL_FREE_FAIL_TIMES(pst)   do{pst->freeFailTimes++;}while(0)

//free�ɹ�ͳ����++,
#define POOL_FREE_SUC_TIMES(pst,size)    do{pst->freeSucTimes++;}while(0)


//free�ɹ�,�޸ĵ�ǰ�ڴ�ʹ��ֵ
#define POOL_FREE_SUC_TIMES_PEEK_STAT(pst,size) do{\
        pst->freeSucTimes++;\
        if(pst->usedSize>=size)\
        {\
            pst->usedSize-=size;\
        }\
    }while(0)




HI_MEM_STEP_STAT_S g_mem_step_stat;


HI_U32 mem_check_calc_mem_size(HI_U32 ulUseSize)
{
    return ulUseSize+HI_MEM_STAT_TOTAL_LEN;
}

HI_U32 mem_check_set_magic(HI_U32 ulRealAddr,HI_U32 ulRealSize,HI_U32 lr)
{
    *(HI_U32*)(ulRealAddr)=HI_MEM_ST_MAGIC;
    *(HI_U32*)(ulRealAddr+4)=ulRealSize;
    *(HI_U32*)(ulRealAddr+8)=lr;
    *(HI_U32*)(ulRealAddr+ulRealSize-4)=HI_MEM_END_MAGIC;
    return ulRealAddr+HI_MEM_STAT_TOTAOL_BEFORE_LEN;
    
}

HI_U32 mem_check_free_check(HI_U32 ulUseAddr,HI_U32 *pSize)
{
    HI_U32 ulRealSize;
    HI_U32 ulReadAddr;
    ulReadAddr=(HI_U32)ulUseAddr-HI_MEM_STAT_TOTAOL_BEFORE_LEN;
    ulRealSize=*(HI_U32*)(ulReadAddr+4);
    HI_MEM_STEP_STAT_S *pstStepStat=GET_STEP_STAT();
    HI_MEM_STEP_ITEM_S *pstStepStatItem=HI_NULL;
    
    if(*(HI_U32*)(ulReadAddr)!=HI_MEM_ST_MAGIC
        ||*(HI_U32*)(ulReadAddr+ulRealSize-4)!=HI_MEM_END_MAGIC
        )
    {
        
        pstStepStatItem=&(pstStepStat->item[pstStepStat->times%HI_MEM_STEP_SUPPORT_RECORD_TIMES]);
        pstStepStatItem->ulStMagic=*(HI_U32*)(ulReadAddr);
        pstStepStatItem->ulSize=*(HI_U32*)(ulReadAddr+4);
        pstStepStatItem->ulMyLr=*(HI_U32*)(ulReadAddr+8);
        pstStepStatItem->ulEndMagic=*(HI_U32*)(ulReadAddr+ulRealSize-4);
        pstStepStat->times++;
        *pSize=0;
    }
    else
    {
        *pSize=ulRealSize;
    } 
    
    return ulReadAddr;
}



HI_MEM_MOD_ITEM_INFO_S * get_pool_item_mod_stat_item(HI_MEM_POOL_ITEM_S *pstItem,HI_U32 ulModId,HI_BOOL bForSave)
{
    //ulPoolId�ĺϷ������ⲿ��֤
    HI_U32 i=0;
    HI_MEM_POOL_ITEM_MOD_STAT_S * pstModStat=GET_POOL_MOD_STAT();
    HI_MEM_MOD_ITEM_INFO_S *pstModStatItem=HI_NULL;

    for(i=0;i<HI_MEM_SUPPORT_STAT_MOD_COUNT;i++)
    {
        pstModStatItem=&(pstModStat->stModItem[i]);
        if(pstModStatItem->modId==ulModId)
        {
            return pstModStatItem;
        }

        if(pstModStatItem->modId==0)
        {
            if(bForSave)
            {
                pstModStatItem->modId=(HI_U16)ulModId;
                pstModStat->ulUsedCount++;
                return pstModStatItem;
            }
            else
            {
                return HI_NULL;
            }
        } 
    }    
    
    if(bForSave)
    {
        return &(pstModStat->stModItem[HI_MEM_SUPPORT_STAT_MOD_COUNT]);
    }
    else
    {
        return HI_NULL;
    }
    
}



HI_U32 HI_MDM_MEM_Init(HI_U32 ulPoolCount)
{
    HI_MEM_POOL_MNGR_S *pstMngr=GET_POOL_MNGR();
    HI_MEM_POOL_ITEM_S *pstItem=HI_NULL;
    
    if(!pstMngr->bModInit)
    {
        if (!ulPoolCount)
        {
            return HI_ERR_INVALID_PARAMETER;
        }
        
        //������Դ
        pstMngr->pstPoolItem = (HI_MEM_POOL_ITEM_S *)malloc(sizeof(HI_MEM_POOL_ITEM_S) * ulPoolCount);

        if (!pstMngr->pstPoolItem)
        {
            return HI_ERR_NOT_ENOUGH_MEMORY;
        }

        memset_s((HI_VOID *)pstMngr->pstPoolItem,sizeof(HI_MEM_POOL_ITEM_S)*ulPoolCount,0x0, sizeof(HI_MEM_POOL_ITEM_S)*ulPoolCount);

        //��ʼ��ϵͳ�ڴ�ع�������
        pstItem=GET_POOL_ITEM(0);
        pstItem->bInit = HI_TRUE;
        
        pstItem->ulOsPoolId = (HI_U32)m_aucSysMem0;
        pstItem->ulAddr = (HI_U32)m_aucSysMem0;
        pstItem->ulSize = LOS_MemPoolSizeGet((HI_PVOID)m_aucSysMem0);
        
        //ģ���ʼ���ɹ�
        pstMngr->ucPoolCount=(HI_U8)ulPoolCount;
        pstMngr->bModInit = HI_TRUE;
        return HI_ERR_SUCCESS;

    }

    return HI_ERR_MEM_RE_INIT;
}


HI_U32 HI_MDM_MEM_CreatePool(HI_VOID *pPoolAddr,HI_U32 ulSize,HI_U32 *pPoolId,HI_U32 ulFlag)
{
    HI_MEM_POOL_MNGR_S *pstMngr = GET_POOL_MNGR();
    HI_MEM_POOL_ITEM_S *pstItem = HI_NULL;
    HI_U32 ret = LOS_OK;
    HI_U32 i = 0;
    HI_U32 ulPoolId = HI_MEM_INVALID_POOL_ID;

    //ģ���ʼ��������ж�
    if(!pstMngr->bModInit)
    {
        return HI_ERR_MEM_NOT_INIT;
    }

    if(pPoolAddr==HI_NULL||pPoolId==HI_NULL||ulSize==0)
    {
        return HI_ERR_MEM_INVALID_PARAM;
    }

    //���ҿ���ITEM
    for(i=0;i<pstMngr->ucPoolCount;i++)
    {
        pstItem = GET_POOL_ITEM(i);
        /*lint -e774*/
        if ((pstItem) && (pstItem->bInit == HI_FALSE))//CODEX
        /*lint +e774*/
        {
            ulPoolId=i;
            break;
        }
    }

    if (!pstItem)
    {
        return HI_ERR_NOT_FOUND;
    }
    
    if(ulPoolId==HI_MEM_INVALID_POOL_ID)
    {
        return HI_ERR_MEM_CREATE_POOL_NOT_ENOUGH_HANDLE;
    }


    //�����ڴ��
    ret = LOS_MemInit(pPoolAddr,ulSize);
    if(ret!=LOS_OK)
    {
        return HI_ERR_MEM_CREAT_POOL_FAIL;
    }

    //��ʼ��pool�����
    pstItem->ulOsPoolId = (HI_U32)pPoolAddr;
    pstItem->ulAddr = (HI_U32)pPoolAddr;
    pstItem->ulSize=ulSize;
    pstItem->bInit=HI_TRUE;

    //����pool id
    *pPoolId=ulPoolId;
    return HI_ERR_SUCCESS;
}

HI_U32 HI_MDM_MEM_FreePool(HI_U32 ulPoolId)
{
    return HI_ERR_NOT_SUPPORT;
}


HI_PVOID pool_mem_malloc(HI_MEM_POOL_ITEM_S *pstItem,HI_U32 ulSize,HI_U32 ulModId,HI_U32 lr)
{
    HI_MEM_POOL_ITEM_COMMON_STAT_S *pstItemCmnStat=GET_POOL_ITEM_COMMON_STAT(pstItem);
    HI_MEM_MOD_ITEM_INFO_S *pstItemModItemStat=get_pool_item_mod_stat_item(pstItem,ulModId,HI_TRUE);
    HI_PVOID ulRealAddr = 0;
    HI_U32 ulUseAddr=0;
    HI_U32 ulRealSize=0;

    if((ulSize==0)||(ulSize>pstItem->ulSize))
    {
        POOL_MALLOC_FAIL_TIMES_ERRTIMES(pstItemCmnStat);
        POOL_MALLOC_FAIL_TIMES_ERRTIMES(pstItemModItemStat);
        return HI_NULL;
    }

    //��������ά���size
    ulRealSize=mem_check_calc_mem_size(ulSize);

    //�����ڴ�
    ulRealAddr = LOS_MemAlloc((HI_PVOID)pstItem->ulOsPoolId, ulRealSize);

    if (ulRealAddr == HI_NULL)
    {
        POOL_MALLOC_FAIL_TIMES(pstItemCmnStat);
        POOL_MALLOC_FAIL_TIMES(pstItemModItemStat);
        g_stSelfRepaire.is_mem_notEnough = HI_TRUE;
        return HI_NULL;
    }
    else
    {
        //���ά����Ϣ,�������û�ʵ���ڴ�ָ��
        ulUseAddr = mem_check_set_magic((HI_U32)ulRealAddr, ulRealSize, lr);

        POOL_MALLOC_SUC_TIMES_PEEK_STAT(pstItemCmnStat,ulRealSize);
        POOL_MALLOC_SUC_TIMES_PEEK_STAT(pstItemModItemStat,ulRealSize);
        return (HI_PVOID)ulUseAddr;
    }
    
}




HI_U32 pool_mem_free(HI_MEM_POOL_ITEM_S *pstItem,HI_PVOID pAddr,HI_U32 ulModId)
{
    HI_MEM_POOL_ITEM_COMMON_STAT_S *pstItemCmnStat=GET_POOL_ITEM_COMMON_STAT(pstItem);
    HI_MEM_MOD_ITEM_INFO_S *pstItemModItemStat=get_pool_item_mod_stat_item(pstItem,ulModId,HI_TRUE);
    HI_U32 ret=LOS_OK;
    HI_U32 ulSize=0;
    HI_U32 ulRealAddr;


    if (!(pAddr >= (HI_PVOID)pstItem->ulAddr && pAddr < (HI_PVOID)(pstItem->ulAddr + pstItem->ulSize)))
    {
        POOL_FREE_FAIL_TIMES_ERRTIMES(pstItemCmnStat);
        POOL_FREE_FAIL_TIMES_ERRTIMES(pstItemModItemStat);

        return HI_ERR_MEM_FREE_FAIL;
    }

    //�����������ڴ�ָ��,��ȡ�ͷŵ��ڴ��С
    ulRealAddr = mem_check_free_check((HI_U32)pAddr, &ulSize);

    ret = LOS_MemFree((HI_PVOID)pstItem->ulOsPoolId, (HI_PVOID)ulRealAddr);
    if(ret!=LOS_OK)
    {
        POOL_FREE_FAIL_TIMES(pstItemCmnStat);
        POOL_FREE_FAIL_TIMES(pstItemModItemStat);
        return HI_ERR_MEM_FREE_FAIL;
    }

    POOL_FREE_SUC_TIMES_PEEK_STAT(pstItemCmnStat,ulSize);
    POOL_FREE_SUC_TIMES_PEEK_STAT(pstItemModItemStat,ulSize);
    return HI_ERR_SUCCESS;


}



HI_PVOID HI_MDM_MEM_Malloc(HI_U32 ulPoolId,HI_U32 ulSize,HI_U32 ulModId,HI_U32 ulUsrLr)
{
    HI_U32 ulLr=HI_ASM_GetLr();
    HI_MEM_POOL_MNGR_S *pstMngr=GET_POOL_MNGR();
    HI_MEM_POOL_ITEM_S *pstItem=HI_NULL;

    // TODO:����ulModId����������

    if(!pstMngr->bModInit||ulPoolId>=pstMngr->ucPoolCount)
    {
        return HI_NULL;
    }

    pstItem=GET_POOL_ITEM(ulPoolId);
    if((!pstItem->bInit))
    {
        return HI_NULL;
    }

    if(ulUsrLr!=0)
    {
        ulLr=ulUsrLr;
    }

    return pool_mem_malloc(pstItem,ulSize,ulModId,ulLr);


}


HI_U32 HI_MDM_MEM_Free(HI_U32 ulPoolId,HI_PVOID pAddr,HI_U32 ulModId)
{
    HI_MEM_POOL_MNGR_S *pstMngr=GET_POOL_MNGR();
    HI_MEM_POOL_ITEM_S *pstItem=HI_NULL;


    //�����Ϸ���У��1
    if(pstMngr->bModInit == HI_FALSE||ulPoolId>=pstMngr->ucPoolCount)
    {
        return HI_ERR_MEM_NOT_INIT;
    }

    //�����Ϸ���У��2
    pstItem=GET_POOL_ITEM(ulPoolId);
    if((!pstItem->bInit))
    {
        return HI_ERR_MEM_INVALID_PARAM;
    }
    
    return pool_mem_free(pstItem,pAddr,ulModId);
}


HI_U32 HI_MDM_MEM_GetHwSize(HI_VOID)
{
    return g_stPoolMngr.ulHwMemSize;
}


HI_PRV HI_U32 pool_mem_get_info(HI_MEM_POOL_ITEM_S* pstItem, HI_MDM_MEM_POOL_INFO_S* pstMemPoolInfo)
{
    HI_U32 ret=LOS_OK;
    LOS_MEM_POOL_STATUS stPoolStatus={0};
    HI_MEM_POOL_ITEM_COMMON_STAT_S *pstItemCmnStat=GET_POOL_ITEM_COMMON_STAT(pstItem);


    //��ϵͳ��ȡ��Ϣ
    ret = LOS_MemInfoGet((HI_PVOID)pstItem->ulOsPoolId, &stPoolStatus);

    if (ret != LOS_OK)
    {
        return HI_ERR_MEM_GET_INFO_FAIL;
    }
    pstMemPoolInfo->ulTotalUsedSize = stPoolStatus.uwTotalUsedSize;
    pstMemPoolInfo->ulTotalFreeSize = stPoolStatus.uwTotalFreeSize;
    pstMemPoolInfo->ulMaxFreeNodeSize = stPoolStatus.uwMaxFreeNodeSize;
    pstMemPoolInfo->ulUsedNodeNum = stPoolStatus.uwUsedNodeNum;
    pstMemPoolInfo->ulFreeNodeNum = stPoolStatus.uwFreeNodeNum;
    pstMemPoolInfo->ulTotalSize = LOS_MemPoolSizeGet((HI_PVOID)pstItem->ulOsPoolId);


    //�м�ͳ�ƽ��
    pstMemPoolInfo->peekSec=pstItemCmnStat->peekSec;
    pstMemPoolInfo->peekSize=pstItemCmnStat->peekSize;
    pstMemPoolInfo->usedSize=pstItemCmnStat->usedSize;
    pstMemPoolInfo->mallocSucTimes=pstItemCmnStat->mallocSucTimes;
    pstMemPoolInfo->freeSucTimes=pstItemCmnStat->freeSucTimes;
    pstMemPoolInfo->mallocFailTimes=pstItemCmnStat->mallocFailTimes;
    pstMemPoolInfo->freeFailTimes=pstItemCmnStat->freeFailTimes;
    pstMemPoolInfo->mallocParamErrTimes=pstItemCmnStat->mallocParamErrTimes;
    pstMemPoolInfo->freeParamErrTimes=pstItemCmnStat->freeParamErrTimes;
    return HI_ERR_SUCCESS;

}
HI_U32 HI_MDM_MEM_GetPoolInfo(HI_U32 ulPoolId,HI_MDM_MEM_POOL_INFO_S *pstMemPoolInfo)
{
    HI_MEM_POOL_MNGR_S *pstMngr=GET_POOL_MNGR();
    HI_MEM_POOL_ITEM_S *pstItem=HI_NULL;

    //�����Ϸ���У��1
    if(pstMngr->bModInit == HI_FALSE||ulPoolId>=pstMngr->ucPoolCount)
    {
        return HI_ERR_MEM_NOT_INIT;
    }

    //�����Ϸ���У��2
    pstItem=GET_POOL_ITEM(ulPoolId);
    if((!pstItem->bInit) || (pstMemPoolInfo==HI_NULL))
    {
        return HI_ERR_MEM_INVALID_PARAM;
    }

    return pool_mem_get_info(pstItem,pstMemPoolInfo);
}

HI_PRV HI_U32 get_mem_mod_info(HI_MDM_MEM_MOD_INFO_S *pDstInfo,HI_MEM_MOD_ITEM_INFO_S *pSrcInfo)
{
    if ((!pDstInfo) || (!pSrcInfo))
    {
        return HI_ERR_INVALID_PARAMETER;
    }
    
    return (HI_U32)memcpy_s((HI_VOID*)pDstInfo, sizeof(HI_MDM_MEM_MOD_INFO_S), (HI_VOID*)pSrcInfo, sizeof(HI_MDM_MEM_MOD_INFO_S));
}

HI_U32 HI_MDM_MEM_GetModNum(HI_VOID)
{
    HI_MEM_POOL_ITEM_MOD_STAT_S * pstModStat = GET_POOL_MOD_STAT();
    return pstModStat->ulUsedCount;
}
    
HI_U32 HI_MDM_MEM_GetModInfoByIdx(HI_U32 ulIdx,HI_MDM_MEM_MOD_INFO_S *pstMemModInfo)
{
    
    HI_MEM_POOL_ITEM_MOD_STAT_S * pstModStat=GET_POOL_MOD_STAT();
    HI_MEM_MOD_ITEM_INFO_S *pstModStatItem=HI_NULL;
    if(ulIdx>=HI_MEM_SUPPORT_STAT_MOD_COUNT||pstMemModInfo==HI_NULL)
    {
        return HI_ERR_MEM_INVALID_PARAM;
    }
    pstModStatItem=&(pstModStat->stModItem[ulIdx]);
    get_mem_mod_info(pstMemModInfo,pstModStatItem);
    return HI_ERR_SUCCESS;
}

HI_U32 HI_MDM_MEM_GetModInfoByModId(HI_U32 ulModId,HI_MDM_MEM_MOD_INFO_S *pstMemModInfo)
{
    HI_MEM_MOD_ITEM_INFO_S *pstModStatItem = get_pool_item_mod_stat_item(HI_NULL, ulModId, HI_FALSE);
    
    if((!pstMemModInfo) || (!pstModStatItem))
    {
        return HI_ERR_MEM_INVALID_PARAM;
    }
    
    get_mem_mod_info(pstMemModInfo, pstModStatItem);
    return HI_ERR_SUCCESS;
}
HI_U32 HI_MDM_MEM_GetStepInfo(HI_MEM_STEP_STAT_S *pstMemStepInfo)
{
    HI_MEM_STEP_STAT_S *pstStepSrcInfo= GET_STEP_STAT();
    
    if(pstMemStepInfo==HI_NULL)
    {
        return HI_ERR_MEM_INVALID_PARAM;
    }

    (HI_VOID)memcpy_s(pstMemStepInfo, sizeof(HI_MEM_STEP_STAT_S), pstStepSrcInfo, sizeof(HI_MEM_STEP_STAT_S));
    return HI_ERR_SUCCESS;
}


HI_U32 HI_MDM_MEM_Config(HI_U32 TotalSize)
{
    // TODO:
    return HI_ERR_NOT_SUPPORT;
}
