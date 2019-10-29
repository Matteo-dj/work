
#ifndef __DFX_LOW_POWER_H__
#define __DFX_LOW_POWER_H__
#include <hi_types.h>
#include <hi_mdm_types.h>
#include <hi_low_power.h>

typedef struct
{
    HI_U32 cmd_id;
    HI_U32 data1;
    HI_U32 data2;
    HI_U32 data3;
    HI_U32 data4;
}HI_LOW_POWER_CMD_INFO_CMD_S;//测试命令

typedef struct
{
    HI_U32 ulMaskIntBits;//低功耗屏蔽哪些中断
    HI_U32 ulMaskIntBitsBackUp;

    HI_U32 last_cnt;

    HI_U32 l_sleepTimes;//判断结果sleep次数
    HI_U32 h_sleepTimes;
    
    HI_U32 l_unSleepTimes;//判断结果无法sleep次数
    HI_U32 h_unSleepTimes;

    
    HI_U32 l_totalTime;//低功耗总时间
    HI_U32 h_totalTime;//低功耗总时间
    
    HI_U32 l_t1_2_t4;//外部睡眠时间总和
    HI_U32 h_t1_2_t4;//外部睡眠时间总和
    
    HI_U32 l_t2_2_t3;//内部睡眠时间总和
    HI_U32 h_t2_2_t3;//内部睡眠时间总和
    
    HI_U32 l_t1_2_t2;//入睡时间总和
    HI_U32 h_t1_2_t2;//入睡时间总和
    
    HI_U32 l_t3_2_t4;//唤醒时间总和
    HI_U32 h_t3_2_t4;//唤醒时间总和


    HI_U32 ulLastWakeUpLisr;
    HI_U32 ulLowPowerCfg;//低功耗配置
    HI_U32 ulForbidBits;//禁止进入低功耗

    HI_U32 ulCheckStat[HI_LOW_POWER_CHECK_ID_LIMIT];//什么原因导致无法进入低功耗
    HI_U32 ulForbidStat[HI_LOW_POWER_FORBID_ID_LIMIT];//什么原因导致无法进入低功耗
}HI_LOW_POWER_CMD_INFO_IND_S;//测试命令结果
#endif

