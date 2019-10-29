
#ifndef __SAL_OS_MEM_H__
#define __SAL_OS_MEM_H__

#define HI_MEM_ST_MAGIC    0x4D421584
#define HI_MEM_END_MAGIC   0x4D468A56
#define HI_MEM_STAT_BEFORE_MAGIC_LEN (4)//ǰħ����ռ���ֽ�
#define HI_MEM_STAT_SIZE_LEN  (4)//ǰ�ڴ��Сռ���ֽ�
#define HI_MEM_STAT_LR_LEN  (4)//ǰLR ռ���ֽ�
#define HI_MEM_STAT_AFTER_MAGIC_LEN  (4)//��ħ����ռ���ֽ�
#define HI_MEM_STAT_TOTAOL_BEFORE_LEN (HI_MEM_STAT_BEFORE_MAGIC_LEN+HI_MEM_STAT_SIZE_LEN+HI_MEM_STAT_LR_LEN)
#define HI_MEM_STAT_TOTAL_LEN (HI_MEM_STAT_TOTAOL_BEFORE_LEN+HI_MEM_STAT_AFTER_MAGIC_LEN)

#define HI_MEM_SUPPORT_STAT_MOD_COUNT 60
#define HI_MEM_SUPPORT_STAT_ARRAY_COUNT (HI_MEM_SUPPORT_STAT_MOD_COUNT+1)//��ģ��ͳ��,֧��ͳ�ƶ��ٸ�ģ��,���һ����Ϊ����ʹ��




typedef HI_MDM_MEM_MOD_INFO_S HI_MEM_MOD_ITEM_INFO_S;//HI_MEM_MOD_ITEM_INFO_SΪ�ڲ�ͳ���ýṹ��,��ǰ�ڲ�ͳ���ýṹ��ṹ�����ⲿinfo


typedef struct
{
    HI_U32 ulUsedCount;
    HI_MEM_MOD_ITEM_INFO_S stModItem[HI_MEM_SUPPORT_STAT_ARRAY_COUNT];
}HI_MEM_POOL_ITEM_MOD_STAT_S;

typedef struct
{
    HI_U32 peekSec;//��ֵʱ��
    HI_U32 peekSize;//��ֵ��С
    HI_U32 usedSize;//��ǰֵ
    
    HI_U16 mallocSucTimes;//����ɹ�����
    HI_U16 freeSucTimes;//�ͷųɹ�����
    
    HI_U16 mallocFailTimes;//����ʧ�ܴ���
    HI_U16 freeFailTimes;//�Ƿ�ʧ�ܴ���

    HI_U16 mallocParamErrTimes;//����ռ��С�Ƿ�
    HI_U16 freeParamErrTimes;//�Ƿ��ڴ��ַ�Ƿ�
}HI_MEM_POOL_ITEM_COMMON_STAT_S;


typedef struct
{
    HI_BOOL bInit;
    HI_U8 pad[3];
    HI_U32 ulOsPoolId;//ϵͳID
    HI_U32 ulAddr;//�ڴ����ʼ��ַ
    HI_U32 ulSize;//�ڴ�ش�С

    HI_MEM_POOL_ITEM_COMMON_STAT_S stCmnStat;
}HI_MEM_POOL_ITEM_S;


typedef struct
{
    HI_BOOL bModInit;
    HI_U8  ucPoolCount;
    HI_U16 pad;
    HI_U32 dyn_size;
    HI_U32 bss_size;
    HI_U32 data_size;
    HI_U32 code_size;
    HI_U32 ulSwMemSize;
    HI_U32 ulHwMemSize;
    HI_MEM_POOL_ITEM_S *pstPoolItem;  /*�ڴ����Ϣ*/
    HI_MEM_POOL_ITEM_MOD_STAT_S stModItem;  /*��MODͳ���ڴ���Ϣ*/
    HI_MEM_STEP_STAT_S stStepStat; /*���ڼ�¼��������ڴ�������Ϣ�����ڶ�λ���ڴ���ڴ�й¶��������*/
}HI_MEM_POOL_MNGR_S;

HI_U32 HI_MDM_MEM_CreatePool(HI_VOID *pPoolAddr,HI_U32 ulSize,HI_U32 *pPoolId,HI_U32 ulFlag);
HI_U32 HI_MDM_MEM_FreePool(HI_U32 ulPoolId);
HI_PVOID HI_MDM_MEM_Malloc(HI_U32 ulPoolId,HI_U32 ulSize,HI_U32 ulModId,HI_U32 ulUsrLr);
HI_U32 HI_MDM_MEM_Free(HI_U32 ulPoolId, HI_PVOID pAddr, HI_U32 ulModId);
#endif

