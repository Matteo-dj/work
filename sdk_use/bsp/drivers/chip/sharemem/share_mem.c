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
// ��������: HI_SHARE_MEM_GetMem
// ��������: ��ȡԤ������õĹ����ڴ档
// ����˵��:
//          ulSize:Ҫʹ�õ��ڴ��С��ֻ����ΪԤ������ռ��Ƿ������û����������
//
// �� �� ֵ:
//           �� HI_NULL ��������ɹ�
// ����Ҫ��: Ŀǰʹ�ó�����,�����ؾ�̬��������û��ʹ���ź��������������չ��ͨ�ù����迼��ȫ�ֱ����������⡣
// ���þ���: TODO: ...
// ��    ��:
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
// ��������: HI_SHARE_MEM_FreeMem
// ��������: �ͷ����뵽�Ĺ����ڴ档
// ����˵��:
//          ulAddr:�����ڴ��ַ
//
// �� �� ֵ:
// ����Ҫ��: Ŀǰʹ�ó�����,�����ؾ�̬��������û��ʹ���ź��������������չ��ͨ�ù����迼��ȫ�ֱ����������⡣
// ���þ���: TODO: ...
// ��    ��:
//*****************************************************************************
HI_VOID HI_SHARE_MEM_FreeMem(HI_PVOID ulAddr)
{
     if(ulAddr != g_shareMemoryAddr)
        return;
     isShareMenFree = HI_TRUE;
}
//*****************************************************************************
// ��������: HI_SHARE_MEM_IsFreeMem
// ��������: �жϹ����ڴ��Ƿ����
// ����˵��:
// �� �� ֵ:
//          HI_TRUE:��������
//          HI_FALSE:�޷�����
// ����Ҫ��:
// ���þ���: TODO: ...
// ��    ��:
//*****************************************************************************
HI_BOOL HI_SHARE_MEM_IsFreeMem(HI_VOID)
{
    return isShareMenFree;
}
#endif

