#include <hi_types.h>
#include <hi_mdm_types.h>
#include <hi_mem.h>
#include "hi_os_mem.h"
#include "los_memory.h"
#include "string.h"
#include <hi_stdlib.h>
#include "hi_os_sys.h"
#include <hi_asm_func.h>
#include <hi_mdm.h>
#include "sal_os_dfx.h"
#include <sal_common.h>
#include <stdlib.h>

extern char __rodata_start;
extern char __rodata_end;
extern char __text_start;
extern char __text_end;
extern char __bss_start;
extern char __ram_data_start;
extern char  __ram_data_end;
extern char __rodata1_start;
extern char __rodata1_end;

typedef enum
{
    HI_DEFAULT_MEM_POOL_DYN,
    HI_DEFAULT_MEM_POOL_STATIC,
    HI_DEFAULT_MEM_POOL_MAX,
}HI_DEFAULT_MEM_POOL_E;

typedef struct
{
    HI_U32 bInit:1;
    HI_U32 pad:31;
    HI_U32 ulPoolId;
    HI_U32 ulPoolSize;
    HI_U32 ulPoolAddr;
}HI_DEFAULT_MEM_POOL_ITEM_CTRL_S;

typedef struct
{

    HI_U32 bSysMalloc:1;
    HI_U32 bSysFree:1;
    HI_U32 pad:30;
}HI_DEFAULT_MEM_POOL_ITEM_STAT_S;


typedef struct
{
    HI_DEFAULT_MEM_POOL_ITEM_CTRL_S poolCtrl[HI_DEFAULT_MEM_POOL_MAX];
    HI_DEFAULT_MEM_POOL_ITEM_STAT_S poolStat[HI_DEFAULT_MEM_POOL_MAX];
}HI_DEFAULT_MEM_POOL_S;


HI_DEFAULT_MEM_POOL_S g_stDefaultPool;
#define GET_DYN_POOL_CTRL() (&g_stDefaultPool.poolCtrl[HI_DEFAULT_MEM_POOL_DYN])
#define GET_STATIC_POOL_CTRL() (&g_stDefaultPool.poolCtrl[HI_DEFAULT_MEM_POOL_STATIC])
#define GET_DYN_POOL_STAT() (&g_stDefaultPool.poolStat[HI_DEFAULT_MEM_POOL_DYN])
#define GET_STATIC_POOL_STAT() (&g_stDefaultPool.poolStat[HI_DEFAULT_MEM_POOL_STATIC])

HI_U32 HI_MDM_MEM_GetSysInfo(HI_OUT HI_MDM_MEM_INFO_S* pstSysMemInfo)
{
    if(NULL == pstSysMemInfo)
    {
        return HI_ERR_MEM_INVALID_PARAM;
    }
    
    pstSysMemInfo->heapUsed = (HI_U32)HI_SYS_MemGetUsed();
    pstSysMemInfo->heapTotal = (HI_U32)HI_SYS_MemGetTotal();
    pstSysMemInfo->heapFree = pstSysMemInfo->heapTotal - pstSysMemInfo->heapUsed;
    pstSysMemInfo->codeSize = ((HI_U32)&__text_end - (HI_U32)&__text_start) + ((HI_U32)&__rodata_end - (HI_U32)&__rodata_start);
    pstSysMemInfo->dataSize = (HI_U32)&__ram_data_end - (HI_U32)&__ram_data_start;
    pstSysMemInfo->bssSize = (HI_U32)&__bss_end - (HI_U32)&__bss_start;
    pstSysMemInfo->rodataSize = (HI_U32)&__rodata_end - (HI_U32)&__rodata_start + (HI_U32)&__rodata1_end - (HI_U32)&__rodata1_start;
    pstSysMemInfo->mptSize = PRODUCT_CFG_MCORE_RAM_LOW_ADDR-SDRAM_BASE; //0x8000
    return HI_ERR_SUCCESS;
}

HI_U32 HI_MDM_MEM_InitDynStaticPool(HI_U32 ulDynPoolSize,HI_U32 ulStaticPoolSize)
{
    HI_U32 ret = HI_ERR_SUCCESS;
    HI_PVOID pMenAddr=HI_NULL;
    HI_U32 ulPoolId=0;
    HI_DEFAULT_MEM_POOL_ITEM_CTRL_S *pstDynMemPoolCtrl=GET_DYN_POOL_CTRL();
    HI_DEFAULT_MEM_POOL_ITEM_CTRL_S *pstStaticMemPoolCtrl=GET_STATIC_POOL_CTRL();
    HI_U32 ulPoolSize = ulDynPoolSize + ulStaticPoolSize;
    
    if ((ulDynPoolSize & 0x3)||(ulStaticPoolSize & 0x3)||(ulPoolSize == 0) || (ulPoolSize < ulDynPoolSize))
    {
        return HI_ERR_MEM_INVALID_PARAM;
    }
    
    pMenAddr = malloc(ulPoolSize);
    if(!pMenAddr)
    {
        return HI_ERR_NOT_ENOUGH_MEMORY;
    }

    if(ulStaticPoolSize)
    {
        ret = HI_MDM_MEM_CreatePool(pMenAddr, ulStaticPoolSize, &ulPoolId,0);

        if (ret != HI_ERR_SUCCESS)
        {
            goto fail;
        }
        pstStaticMemPoolCtrl->ulPoolId=ulPoolId;
        pstStaticMemPoolCtrl->ulPoolSize=ulStaticPoolSize;
        pstStaticMemPoolCtrl->ulPoolAddr=(HI_U32)pMenAddr;
        pstStaticMemPoolCtrl->bInit=HI_TRUE;

    }

    if(ulDynPoolSize)
    {
        ret = HI_MDM_MEM_CreatePool((HI_PVOID)((HI_PBYTE)pMenAddr + ulStaticPoolSize), ulDynPoolSize, &ulPoolId, 0);

        if (ret != HI_ERR_SUCCESS)
        {
            goto fail;

        }
        pstDynMemPoolCtrl->ulPoolId=ulPoolId;
        pstDynMemPoolCtrl->ulPoolSize = ulDynPoolSize;
        pstDynMemPoolCtrl->ulPoolAddr = (HI_U32)((HI_PBYTE)pMenAddr + ulStaticPoolSize);
        pstDynMemPoolCtrl->bInit=HI_TRUE;

    }
    
/*
        【屏蔽人】z00193780
        【审核人】w00190206
        【代码分析】单板启动后内存初始化，申请后地址存储至全局变量中，作为动态内存和静态内存池，此处无需释放，不存在内存泄露问题。
        【功能影响】其他模块会使用静态内存或动态内存地址，屏蔽告警。
 */
    return HI_ERR_SUCCESS;
fail:
    if(pstDynMemPoolCtrl->bInit)
    {
        HI_MDM_MEM_FreePool(pstDynMemPoolCtrl->ulPoolId);
        memset_s(pstDynMemPoolCtrl,sizeof(HI_DEFAULT_MEM_POOL_ITEM_CTRL_S), 0,sizeof(HI_DEFAULT_MEM_POOL_ITEM_CTRL_S));
    }
    if(pstStaticMemPoolCtrl->bInit)
    {
        HI_MDM_MEM_FreePool(pstStaticMemPoolCtrl->ulPoolId);
        memset_s(pstStaticMemPoolCtrl,sizeof(HI_DEFAULT_MEM_POOL_ITEM_CTRL_S), 0,sizeof(HI_DEFAULT_MEM_POOL_ITEM_CTRL_S));
    }
    
    free(pMenAddr);    
    return ret;    
}



/*
建议使用固定ulModId来表示未内存池未初始化之前的内存申请和释放。

*/

HI_PVOID HI_MDM_Malloc(HI_U32 ulModId, HI_U32 ulSize)
{
    
    HI_U32 ulLr=HI_ASM_GetLr();
    HI_PVOID pAddr=HI_NULL;
	HI_U32 ulLocalSize =  ALIGN_LENGTH(ulSize,ALIGNTYPE_4BYTE);
			
    HI_DEFAULT_MEM_POOL_ITEM_CTRL_S *pMemPoolCtrl=GET_DYN_POOL_CTRL();
    HI_DEFAULT_MEM_POOL_ITEM_STAT_S *pMemPoolStat=GET_DYN_POOL_STAT();
    if(pMemPoolCtrl->bInit)
    {   
        pAddr=HI_MDM_MEM_Malloc(pMemPoolCtrl->ulPoolId,ulLocalSize,ulModId,ulLr);
    }
    else
    {
        pMemPoolStat->bSysMalloc=HI_TRUE;
        pAddr=HI_MDM_MEM_Malloc((HI_U32)HI_MDM_MEM_SYS_POOL_ID,ulLocalSize,ulModId,ulLr);
    }
    
    return pAddr;
}
HI_PVOID HI_MDM_GetStaticMem(HI_U32 ulModId, HI_U32 ulSize)
{
    HI_U32 ulLr=HI_ASM_GetLr();
    HI_DEFAULT_MEM_POOL_ITEM_CTRL_S *pMemPoolCtrl=GET_STATIC_POOL_CTRL();
    HI_DEFAULT_MEM_POOL_ITEM_STAT_S *pMemPoolStat=GET_STATIC_POOL_STAT();
	HI_U32 ulLocalSize =  ALIGN_LENGTH(ulSize,ALIGNTYPE_4BYTE);
	//edb_put_str_p3("HI_MDM_GetStaticMem", ulModId, ulSize, lr);
    if(pMemPoolCtrl->bInit)
    {
        return HI_MDM_MEM_Malloc(pMemPoolCtrl->ulPoolId,ulLocalSize,ulModId,ulLr);
    }
    else
    {
        pMemPoolStat->bSysMalloc=HI_TRUE;
        return HI_MDM_MEM_Malloc((HI_U32)HI_MDM_MEM_SYS_POOL_ID,ulLocalSize,ulModId,ulLr);
    }

}


HI_VOID HI_MDM_Free(HI_U32 ulModId, HI_PVOID pAddr)
{
    HI_DEFAULT_MEM_POOL_ITEM_CTRL_S *pMemPoolCtrl=GET_DYN_POOL_CTRL();
    HI_DEFAULT_MEM_POOL_ITEM_STAT_S *pMemPoolStat=GET_DYN_POOL_STAT();

    // TODO:增加了这个判断,下面就无法统计到该种错误
    if ( pMemPoolCtrl->bInit && (pAddr >= (HI_PVOID)pMemPoolCtrl->ulPoolAddr) && (pAddr <= (HI_PVOID)((HI_PBYTE)pMemPoolCtrl->ulPoolAddr + pMemPoolCtrl->ulPoolSize)) )
    {
        HI_MDM_MEM_Free(pMemPoolCtrl->ulPoolId,pAddr,ulModId);
    }
    else
    {
        pMemPoolStat->bSysFree=HI_TRUE;
        HI_MDM_MEM_Free(HI_MDM_MEM_SYS_POOL_ID,pAddr,ulModId);
    }
}

HI_U32 HI_MDM_MEM_GetStaticPoolInfo(HI_MDM_MEM_POOL_INFO_S *pstMemPoolInfo)
{

    HI_DEFAULT_MEM_POOL_ITEM_CTRL_S *pMemPoolCtrl=GET_STATIC_POOL_CTRL();
    if(!pMemPoolCtrl->bInit)
    {
        return HI_ERR_MEM_NOT_INIT;
    }
    return HI_MDM_MEM_GetPoolInfo(pMemPoolCtrl->ulPoolId,pstMemPoolInfo);

}

HI_U32 HI_MDM_MEM_GetDynPoolInfo(HI_MDM_MEM_POOL_INFO_S *pstMemPoolInfo)
{
    
    HI_DEFAULT_MEM_POOL_ITEM_CTRL_S *pMemPoolCtrl=GET_DYN_POOL_CTRL();
    if(!pMemPoolCtrl->bInit)
    {
        return HI_ERR_MEM_NOT_INIT;
    }
    return HI_MDM_MEM_GetPoolInfo(pMemPoolCtrl->ulPoolId,pstMemPoolInfo);

}

HI_U32 HI_MDM_MEM_GetSysPoolInfo(HI_MDM_MEM_POOL_INFO_S *pstMemPoolInfo)
{
    return HI_MDM_MEM_GetPoolInfo(HI_MDM_MEM_SYS_POOL_ID,pstMemPoolInfo);
}


