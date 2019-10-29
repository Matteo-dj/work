#ifndef __DFX_CPUP_H__
#define __DFX_CPUP_H__
#include <hi_types.h>
#include <hi_mdm_types.h>

typedef struct
{
    HI_U32 ulAutoReportPeriodSec;//正是版本时设置统计多长时间的CPU占用率，超时停止CPU统计(新能考虑)。暂不使用
}HI_DFX_CPUP_ST_CMD_S;//启动CPUP统计CMD

typedef struct
{
    HI_U32 ulRet;//启动错误码
}HI_DFX_CPUP_ST_IND_S;//启动CPUP统计IND

typedef struct
{
    HI_U32 pad;
}HI_DFX_CPUP_END_CMD_S;//停止CPUP统计CMD


typedef struct
{
    HI_U32 ulRet;//停止错误码
}HI_DFX_CPUP_END_IND_S;//停止CPUP统计IND

typedef struct
{
    HI_U32 pad;
}HI_DFX_CPUP_GET_INFO_CMD_S;//获取CPUP信息CMD

typedef struct
{
    HI_U32 ulRet;//错误码
    HI_U32 ulTaskCount;//task个数
    HI_U32 ulLastTotalTimeLow;//上次统计时总耗时低32位
    HI_U32 ulLastTotalTimeHigh;//上次统计时总耗时高32位
    HI_U32 ulCurTotalTimeLow;//本次统计时总耗时低32位
    HI_U32 ulCurTotalTimeHigh;//本次统计时总耗时高32位
}HI_CPUP_DFX_INFO_TOTAL_IND_S;//上报CPUP整体信息IND

#define HI_CPUP_DFX_INFO_TASK_NAME_SIZE 32//上报数据taskname最大长度
typedef struct
{
    HI_U32 ulRet;//错误码
    HI_CHAR taskName[HI_CPUP_DFX_INFO_TASK_NAME_SIZE];//taskname or isr名
    HI_U32 ulLastItemTimeLow;//上次统计时耗时低32位
    HI_U32 ulLastItemTimeHigh;//上次统计时耗时高32位
    HI_U32 ulCurItemTimeLow;//本次统计时耗时低32位
    HI_U32 ulCurItemTimeHigh;//本次统计时耗时高32位
    HI_U32 ulPermillageTotal;//整个统计中期千分比
    HI_U32 ulPermillagePhase;//本周期统计千分比
}HI_CPUP_DFX_INFO_ITEM_IND_S;//上报CPUP每一项信息

#endif
