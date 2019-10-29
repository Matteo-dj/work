//*****************************************************************************
//
//                  版权所有 (C), 1998-2015, 华为技术有限公司
//
//*****************************************************************************
//  文 件 名   : mrs_srv_sta_dutycycle.h
//  版 本 号   : V1.0
//  作    者   : cuiate/00233580
//  生成日期   : 2015-08-27
//  功能描述   : STA占空比策略
//
//  函数列表   : TODO: ...
//  修改历史   :
//  1.日    期 : 2015-08-27
//    作    者 : cuiate/00233580
//    修改内容 : 创建文件
//
//*****************************************************************************

#ifndef _MRS_SRV_STA_DUTYCYCLE_H_
#define _MRS_SRV_STA_DUTYCYCLE_H_

HI_START_HEADER
#if defined(PRODUCT_CFG_PRODUCT_TYPE_STA)
//*****************************************************************************
//*****************************************************************************

#define MRS_STA_DUTY_CYCLE_CFG_ITEM_MAX     (6)
#define MRS_STA_DUTY_CYCLE_RESET_TIME_MAX   (64)

#define MRS_STA_NV_RESET_TIME_NUM           (32)

// [STA模块上电时间信息]使用数据存储NV项0x40C-0x40D，结构大小与结构NV_APP_NVM_DATA_STRU相同
// 在本模块处理时，统一使用本模块自定义的结构NV_APP_STA_RESET_TIME_INFO
typedef struct
{
    HI_U32 aulResetTime[32];    // 重启事件信息，4字节存储一次时间，代表从1970-1-1 00:00:00计算的秒数。
                                // 索引0代表最近1次信息，依次类推。默认值都为0
} NV_APP_STA_RESET_TIME_INFO;


typedef struct
{
    // 占空比参数
    HI_U8 ucShortDuration;      // 持续较短时间，单位：分钟；默认值30
    HI_U8 ucShortTimes;         // 短时间重启次数阈值，默认：10（受限于存储空间，不得超过64）
    HI_U8 ucLongDuration;       // 持续较长时间，单位：小时；默认值72
    HI_U8 ucLongTimes;          // 长时间重启次数阈值，默认：30（受限于存储空间，不得超过64）

    HI_S8 cIndex;               // 记录的占空比策略索引
    HI_U8 ucDutyCycleNum;       // 占空比策略数量
    HI_BOOL bRecTimeOnly;       // 记录启动时间标识，无需查询断电记录
    HI_BOOL bCheckPowerInf;     // 是否需要检查断电记录
    NV_APP_STA_DUTY_CYCLE_CFG_ITEM astCfgItem[MRS_STA_DUTY_CYCLE_CFG_ITEM_MAX];

    HI_U32 aulResetTime[MRS_STA_DUTY_CYCLE_RESET_TIME_MAX];

    HI_U32 ulCurrentDi;         // 当前正在处理的DI
    HI_U32 ulMeterCurTime;      // 电表当前时间
    HI_U32 ulMeterPowerOnTime;  // 电表(上1次)掉电结束时刻
} MRS_SRV_STA_DUTY_CYCLE_CONTEXT;



HI_PUBLIC HI_U32 mrsStaDutyCycleInit(HI_VOID);

HI_PUBLIC HI_U32 mrsStaDutyCycleDeinit(HI_VOID);

HI_PUBLIC HI_VOID mrsStaDutyCycleSetMeterInf(HI_BOOL bChg, HI_U8 ucSlaveFlag, HI_U32 ulParam);

HI_PUBLIC HI_U32 mrsStaDutyCycleFrameRx(HI_U8 *pFrame, HI_U16 usFrameLength);

HI_PUBLIC HI_U32 mrsStaDutyCycleTimeout(HI_VOID);

HI_U32 mrsStaDutyCyclePowerInfQryExp(HI_VOID);

//*****************************************************************************
//*****************************************************************************
#endif // defined(PRODUCT_CFG_PRODUCT_TYPE_STA)
HI_END_HEADER

#endif //_MRS_SRV_STA_DUTYCYCLE_H_

