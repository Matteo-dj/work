//*****************************************************************************
//
//                  ��Ȩ���� (C), 1998-2013, ��Ϊ�������޹�˾
//
//*****************************************************************************
//  �� �� ��   : mrs_time.h
//  �� �� ��   : V1.0 
//  ��    ��   : cuiate/00233580
//  ��������   : 2013-04-07
//  ��������   : ʱ�亯�����ӿڶ���
//               
//  �����б�   : TODO: ...
//  �޸���ʷ   : 
//  1.��    �� : 2013-04-07
//    ��    �� : cuiate/00233580
//    �޸����� : �����ļ� 
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

// ucTime[6]: ��-��-ʱ-��-��-�� (BCD��)
HI_EXTERN HI_U32 MRS_TimeIsValid(HI_IN HI_CONST HI_U8 ucTime[6]);

// ucTime[7]: ��-��-ʱ-����-��-��-�� (BCD��)
HI_EXTERN HI_U32 MRS_TimeWeekIsValid(HI_IN HI_CONST HI_U8 ucTime[7]);


// ucTime[3]: ��-��-��(BIN��)
HI_U32 MRS_TimeBinIsValid(HI_IN HI_CONST HI_U8 ucTime[3]);

HI_END_HEADER
#endif //MRS_TIME_H
