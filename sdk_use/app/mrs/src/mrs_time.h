//*****************************************************************************
//
//                  版权所有 (C), 1998-2013, 华为技术有限公司
//
//*****************************************************************************
//  文 件 名   : mrs_time.h
//  版 本 号   : V1.0
//  作    者   : cuiate/00233580
//  生成日期   : 2013-04-07
//  功能描述   : 时间函数及接口定义
//
//  函数列表   : TODO: ...
//  修改历史   :
//  1.日    期 : 2013-04-07
//    作    者 : cuiate/00233580
//    修改内容 : 创建文件
//
//*****************************************************************************

//*****************************************************************************
// PROJECT   :
// SUBSYSTEM :
// MODULE    :
// OWNER     :
//*****************************************************************************

#ifndef MRS_TIME_H
#define MRS_TIME_H
HI_START_HEADER

#define MRS_YEAR_BASE           (2000)
#define MRS_TIME_ROLLBACK       (0xFFFFFFFF)  // 时间翻转的最大值

#define MRS_MIN_PER_HOUR        (60)
#define MRS_SEC_PER_MIN         (60)
#define MRS_SEC_PER_HOUR        (MRS_SEC_PER_MIN * MRS_MIN_PER_HOUR)
#define MRS_MS_PER_SEC          (1000)
#define MRS_MS_PER_MIN          (MRS_MS_PER_SEC * MRS_SEC_PER_MIN)
#define MRS_MS_PER_HOUR         (MRS_MIN_PER_HOUR * MRS_SEC_PER_MIN * MRS_MS_PER_SEC)
#define MRS_US_PER_MS           (1000)
#define MRS_NTB_PER_US          (25)
#define MRS_NTB_PER_MS          (MRS_US_PER_MS * MRS_NTB_PER_US)

#define MRS_NTB_TO_MS(ntb)      ((ntb) / MRS_NTB_PER_MS)
#define MRS_MS_TO_NTB(ms)       ((HI_U32)(ms)   * MRS_NTB_PER_MS)
#define MRS_SEC_TO_MS(sec)      ((HI_U32)(sec)  * MRS_MS_PER_SEC)
#define MRS_MIN_TO_MS(min)      ((HI_U32)(min)  * MRS_MS_PER_MIN)
#define MRS_MIN_TO_SEC(min)     ((HI_U32)(min)  * MRS_SEC_PER_MIN)
#define MRS_HOUR_TO_MS(hour)    ((HI_U32)(hour) * MRS_MS_PER_HOUR)
#define MRS_HOUR_TO_SEC(hour)   ((HI_U32)(hour) * MRS_SEC_PER_HOUR)

HI_EXTERN HI_U32 MRS_BCD2Time(HI_IN HI_CONST HI_U8 ucTime[6], HI_OUT HI_SYS_CALENDAR_TIME_S *pstTime);

// ucTime[6]: 秒-分-时-日-月-年 (BCD码)
HI_EXTERN HI_U32 MRS_TimeIsValid(HI_IN HI_CONST HI_U8 ucTime[6]);

// ucTime[7]: 秒-分-时-星期-日-月-年 (BCD码)
HI_EXTERN HI_U32 MRS_TimeWeekIsValid(HI_IN HI_CONST HI_U8 ucTime[7]);

/**
 * @brief 计算两个时间的差(带翻转处理)
 */
HI_U32 mrsTimeSubDuration(HI_U32 ulTime, HI_U32 ulDuration);

/**
 * @brief 计算两个时间压缩后的差(带翻转处理，先压后差)
 */
HI_U32 mrsTimeSubDurationCompress(HI_U32 ulTime, HI_U32 ulDuration, HI_U32 ulCompressTimes);

/**
 * @brief 计算 time + duration (带翻转处理)
 */
HI_U32 mrsTimeAddDuration(HI_U32 ulTime, HI_U32 ulDuration);

// ucTime[3]: 年-月-日(BIN码)
HI_U32 MRS_TimeBinIsValid(HI_IN HI_CONST HI_U8 ucTime[3]);

HI_BOOL mrsTimeCheckRtcYearIsAfterBase(HI_U32 ulRtcTimeSec, HI_U32 ulYearBase);
HI_END_HEADER
#endif //MRS_TIME_H
