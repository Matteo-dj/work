#include <hi_types.h>
#include <hi_mdm_types.h>
#ifdef PRODUCT_CFG_SUPPORT_SHARE_MEMORY
#include <hi_mdm_isr.h>
#include <hi_mdm_mem.h>
HI_BOOL isShareMenFree = HI_FALSE;
HI_PVOID g_shareMemoryAddr = HI_NULL;
HI_U32 HI_SHARE_MEM_Init(HI_VOID)
{
    HI_U32 temp_addr = 0;
    /*lint -e506*/
    /*lint -e774*/
    if(PRODUCT_CFG_SHARE_MEMORY_SIZE!=0)
    /*lint +e774*/
    /*lint +e506*/
    {
        temp_addr = (HI_U32)HI_MDM_Malloc(HI_MOD_ID_DRV_DFX, PRODUCT_CFG_SHARE_MEMORY_SIZE+ALIGNTYPE_64BYTE);
        if(temp_addr != (HI_U32)HI_NULL)
        {
            g_shareMemoryAddr = (HI_PVOID)ALIGN_LENGTH(temp_addr,ALIGNTYPE_64BYTE);
            isShareMenFree = HI_TRUE;
            return HI_ERR_SUCCESS;
        }
        return HI_ERR_FAILURE;
    }
    return HI_ERR_SUCCESS;
}
//*****************************************************************************
// 函数名称: HI_SHARE_MEM_GetMem
// 功能描述: 获取预先申请好的共享内存。
// 参数说明:
//          ulSize:要使用的内存大小，只是作为预先申请空间是否满足用户需求的依据
//
// 返 回 值:
//           非 HI_NULL 代表申请成功
// 调用要求: 目前使用场景简单,因此相关静态变量访问没有使用信号量保护。如果扩展成通用功能需考虑全局变量保护问题。
// 调用举例: TODO: ...
// 作    者:
//*****************************************************************************
HI_PVOID HI_SHARE_MEM_GetMem(HI_U32 ulSize)
{
    HI_U32 ulIntStat;  
    if(ulSize > PRODUCT_CFG_SHARE_MEMORY_SIZE)
    {
        return HI_NULL;
    }
    ulIntStat=HI_MDM_INT_Lock();
    if(isShareMenFree)
    {
        isShareMenFree = HI_FALSE;
        HI_MDM_INT_Restore(ulIntStat);
        return g_shareMemoryAddr;
    }
    else
    {
        HI_MDM_INT_Restore(ulIntStat);
        return HI_NULL;
    }
}
//*****************************************************************************
// 函数名称: HI_SHARE_MEM_FreeMem
// 功能描述: 释放申请到的共享内存。
// 参数说明:
//          ulAddr:共享内存地址
//
// 返 回 值:
// 调用要求: 目前使用场景简单,因此相关静态变量访问没有使用信号量保护。如果扩展成通用功能需考虑全局变量保护问题。
// 调用举例: TODO: ...
// 作    者:
//*****************************************************************************
HI_VOID HI_SHARE_MEM_FreeMem(HI_PVOID ulAddr)
{
     if(ulAddr != g_shareMemoryAddr)
        return;
     isShareMenFree = HI_TRUE;
}
//*****************************************************************************
// 函数名称: HI_SHARE_MEM_IsFreeMem
// 功能描述: 判断共享内存是否可用
// 参数说明:
// 返 回 值:
//          HI_TRUE:可以申请
//          HI_FALSE:无法申请
// 调用要求:
// 调用举例: TODO: ...
// 作    者:
//*****************************************************************************
HI_BOOL HI_SHARE_MEM_IsFreeMem(HI_VOID)
{
    return isShareMenFree;
}
#endif

