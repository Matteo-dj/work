
#ifndef __SAL_OS_MEM_H__
#define __SAL_OS_MEM_H__

#define HI_MEM_ST_MAGIC    0x4D421584
#define HI_MEM_END_MAGIC   0x4D468A56
#define HI_MEM_STAT_BEFORE_MAGIC_LEN (4)//前魔术字占用字节
#define HI_MEM_STAT_SIZE_LEN  (4)//前内存大小占用字节
#define HI_MEM_STAT_LR_LEN  (4)//前LR 占用字节
#define HI_MEM_STAT_AFTER_MAGIC_LEN  (4)//后魔术字占用字节
#define HI_MEM_STAT_TOTAOL_BEFORE_LEN (HI_MEM_STAT_BEFORE_MAGIC_LEN+HI_MEM_STAT_SIZE_LEN+HI_MEM_STAT_LR_LEN)
#define HI_MEM_STAT_TOTAL_LEN (HI_MEM_STAT_TOTAOL_BEFORE_LEN+HI_MEM_STAT_AFTER_MAGIC_LEN)

#define HI_MEM_SUPPORT_STAT_MOD_COUNT 60
#define HI_MEM_SUPPORT_STAT_ARRAY_COUNT (HI_MEM_SUPPORT_STAT_MOD_COUNT+1)//按模块统计,支持统计多少个模块,最后一个作为其它使用




typedef HI_MDM_MEM_MOD_INFO_S HI_MEM_MOD_ITEM_INFO_S;//HI_MEM_MOD_ITEM_INFO_S为内部统计用结构体,当前内部统计用结构体结构等于外部info


typedef struct
{
    HI_U32 ulUsedCount;
    HI_MEM_MOD_ITEM_INFO_S stModItem[HI_MEM_SUPPORT_STAT_ARRAY_COUNT];
}HI_MEM_POOL_ITEM_MOD_STAT_S;

typedef struct
{
    HI_U32 peekSec;//峰值时间
    HI_U32 peekSize;//峰值大小
    HI_U32 usedSize;//当前值
    
    HI_U16 mallocSucTimes;//申请成功次数
    HI_U16 freeSucTimes;//释放成功次数
    
    HI_U16 mallocFailTimes;//申请失败次数
    HI_U16 freeFailTimes;//是否失败次数

    HI_U16 mallocParamErrTimes;//申请空间大小非法
    HI_U16 freeParamErrTimes;//是否内存地址非法
}HI_MEM_POOL_ITEM_COMMON_STAT_S;


typedef struct
{
    HI_BOOL bInit;
    HI_U8 pad[3];
    HI_U32 ulOsPoolId;//系统ID
    HI_U32 ulAddr;//内存池起始地址
    HI_U32 ulSize;//内存池大小

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
    HI_MEM_POOL_ITEM_S *pstPoolItem;  /*内存池信息*/
    HI_MEM_POOL_ITEM_MOD_STAT_S stModItem;  /*按MOD统计内存信息*/
    HI_MEM_STEP_STAT_S stStepStat; /*用于记录最近几次内存申请信息，用于定位踩内存和内存泄露处等问题*/
}HI_MEM_POOL_MNGR_S;

HI_U32 HI_MDM_MEM_CreatePool(HI_VOID *pPoolAddr,HI_U32 ulSize,HI_U32 *pPoolId,HI_U32 ulFlag);
HI_U32 HI_MDM_MEM_FreePool(HI_U32 ulPoolId);
HI_PVOID HI_MDM_MEM_Malloc(HI_U32 ulPoolId,HI_U32 ulSize,HI_U32 ulModId,HI_U32 ulUsrLr);
HI_U32 HI_MDM_MEM_Free(HI_U32 ulPoolId, HI_PVOID pAddr, HI_U32 ulModId);
#endif

