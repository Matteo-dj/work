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
#define MRS_SECONDS_PER_MINUTE  (60)
#define MRS_MS_PER_SECOND       (1000)
#define MRS_MIN_TO_SEC(x)       ((x) * MRS_SECONDS_PER_MINUTE)
#define MRS_SEC_TO_MS(x)        ((x) * MRS_MS_PER_SECOND)
#define MRS_MIN_TO_MS(x)        MRS_SEC_TO_MS(MRS_MIN_TO_SEC(x))

HI_EXTERN HI_U32 MRS_BCD2Time(HI_IN HI_CONST HI_U8 ucTime[6], HI_OUT HI_SYS_CALENDAR_TIME_S *pstTime);

// ucTime[6]: 秒-分-时-日-月-年 (BCD码)
HI_EXTERN HI_U32 MRS_TimeIsValid(HI_IN HI_CONST HI_U8 ucTime[6]);

// ucTime[7]: 秒-分-时-星期-日-月-年 (BCD码)
HI_EXTERN HI_U32 MRS_TimeWeekIsValid(HI_IN HI_CONST HI_U8 ucTime[7]);


// ucTime[3]: 年-月-日(BIN码)
HI_U32 MRS_TimeBinIsValid(HI_IN HI_CONST HI_U8 ucTime[3]);

HI_END_HEADER
#endif //MRS_TIME_H
