#ifndef __HI_MEM_H__
#define __HI_MEM_H__

#include <hi_mdm_mem.h>
#define HI_MDM_MEM_SYS_POOL_ID 0
#define HI_MEM_STEP_SUPPORT_RECORD_TIMES 5//采内存统计,支持统计几次踩内存信息
typedef struct
{
    HI_U32 ulStMagic;
    HI_U32 ulSize;
    HI_U32 ulMyLr;
    HI_U32 ulEndMagic;

    HI_U32 ulBeforeLr;
}HI_MEM_STEP_ITEM_S;

typedef struct
{
    HI_U32 times;
    HI_MEM_STEP_ITEM_S item[HI_MEM_STEP_SUPPORT_RECORD_TIMES];
}HI_MEM_STEP_STAT_S;

HI_U32 HI_MDM_MEM_GetPoolInfo(HI_U32 ulPoolId,HI_MDM_MEM_POOL_INFO_S *pstMemPoolInfo);
HI_U32 HI_MDM_MEM_GetStepInfo(HI_MEM_STEP_STAT_S *pstMemStepStat);
#endif
