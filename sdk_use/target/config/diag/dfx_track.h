#ifndef __DFX_TRACK_H__
#define __DFX_TRACK_H__
#include <hi_types.h>
#include <hi_mdm_types.h>

typedef struct
{
    HI_U32 ulArrayCount;//轨迹统计支持记录多少条信息
    HI_U32 ulOption;//轨迹统计支持统计哪种类型轨迹(调度,关中断...)
}HI_DFX_TRACK_ST_CMD_S;//启动轨迹统计cmd

typedef struct
{
    HI_U32 ulRet;//启动错误码
}HI_DFX_TRACK_ST_IND_S;//启动轨迹统计IND


typedef struct
{
    HI_U32 pad;
}HI_DFX_TRACK_STOP_CMD_S;//停止轨迹统计cmd


typedef struct
{
    HI_U32 ulRet;//停止错误码
}HI_DFX_TRACK_STOP_IND_S;//停止轨迹统计IND

typedef struct
{
    HI_U32 bLock;//是否进行lock操作，进行lock操作表示查看新的信息，不进行lock操作表示重新查看历史信息
}HI_DFX_TRACK_GET_INFO_CMD_S;//获取轨迹统计信息cmd

typedef struct
{
    HI_U32 ulRet;//错误码
    HI_U32 ulArrayCount;//最多支持多少条轨迹统计,启动时设置
    HI_U32 ulOption;//支持哪类轨迹统计,启动是设置
    HI_U32 ulLockPos;//当前锁定的下标ID
    
}HI_DFX_TRACK_INFO_TOTAL_IND_S;//上报轨迹整体信息IND

typedef struct
{
    HI_U32 ulRet;
    HI_U32 ulSeq;//序号从0至ulArrayCount-1

    HI_U32 ulCurTime;//统计时间点
    HI_U32 ulTraceType;//统计类型
    HI_U32 ulEntry;//表示调用位置，任务主体等信息
    HI_U32 ulData0;//数据0
    HI_U32 ulData1;//数据1
}HI_DFX_TRACK_INFO_ITEM_IND_S;//上报轨迹一项信息IND

#endif

