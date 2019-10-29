//*****************************************************************************
//
//                  ��Ȩ���� (C), 1998-2013, ��Ϊ�������޹�˾
//
//*****************************************************************************
//  �� �� ��   : mrs_time.c
//  �� �� ��   : V1.0
//  ��    ��   : cuiate/00233580
//  ��������   : 2013-04-07
//  ��������   : ʱ�亯�����ӿ�ʵ��
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

#include "mrs_common.h"
#include "mrs_srv_common.h"
#include "mrs_fw_tools.h"
#include "mrs_time.h"


#define isleap(y)       ((((y) % 4) == 0 && ((y) % 100) != 0) || ((y) % 400) == 0)

#define TimeArea(ucMinTime,ucTime,ucMaxTime) \
    do \
    { \
        if ((ucTime < ucMinTime) || (ucTime > ucMaxTime)) \
        { \
            ret = HI_ERR_BAD_DATA; \
            return ret; \
        } \
    } while (0)


HI_U32 MRS_BCD2Time(HI_IN HI_CONST HI_U8 ucTime[6], HI_OUT HI_SYS_CALENDAR_TIME_S *pstTime)
{
    if (!pstTime)
    {
        return HI_ERR_INVALID_PARAMETER;
    }

    (hi_void)memset_s(pstTime, sizeof(HI_SYS_CALENDAR_TIME_S), 0, sizeof(HI_SYS_CALENDAR_TIME_S));
    pstTime->sec = (HI_U8)BCD2INT(ucTime[0]);
    pstTime->min = (HI_U8)BCD2INT(ucTime[1]);
    pstTime->hour = (HI_U8)BCD2INT(ucTime[2]);
    pstTime->day = (HI_U8)BCD2INT(ucTime[3]);
    pstTime->month = (HI_U8)BCD2INT(ucTime[4]);
    pstTime->year = (HI_U16)(BCD2INT(ucTime[5]) + MRS_YEAR_BASE);

    return HI_ERR_SUCCESS;
}


// ucTime[7]: ��-��-ʱ-����-��-��-�� (BCD��)
HI_U32 MRS_TimeWeekIsValid(HI_IN HI_CONST HI_U8 ucTime[7])
{
    HI_U32 ret = HI_ERR_SUCCESS;
    HI_U32 mday[12] = {0x31, 0x28, 0x31, 0x30, 0x31, 0x30, 0x31, 0x31, 0x30, 0x31, 0x30, 0x31};
    HI_U32 ulYear;
    HI_U32 ulMonth;

    if (!mrsToolsCheckBCD((HI_PBYTE)ucTime, 7))
    {
        return HI_ERR_BAD_DATA;
    }

    ulYear = BCD2INT(ucTime[6]) + MRS_YEAR_BASE;
    ulMonth = BCD2INT(ucTime[5]);

    if (ulMonth < 1 || ulMonth > 12)
    {
        return HI_ERR_BAD_DATA;
    }

    if (isleap(ulYear))
    {
        mday[1] = 0x29;
    }

    TimeArea(0x01, (ucTime[0] + 1), 0x60);
    TimeArea(0x01, (ucTime[1] + 1), 0x60);
    TimeArea(0x01, (ucTime[2] + 1), 0x24);
    TimeArea(0x01, (ucTime[3] + 1), 0x07);
    TimeArea(0x01, ucTime[4], mday[ulMonth - 1]);
    TimeArea(0x01, ucTime[5], 0x12);

    return ret;
}

// ucTime[3] YY-MM-DD(BIN)
HI_U32 MRS_TimeBinIsValid(HI_IN HI_CONST HI_U8 ucTime[3])
{
    HI_U32 ret = HI_ERR_SUCCESS;
    HI_U32 mday[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    HI_U32 ulYear;
    HI_U32 ulMonth;

    ulYear = ucTime[0] + MRS_YEAR_BASE;
    ulMonth = ucTime[1];

    if (ulMonth < 1 || ulMonth > 12)
    {
        return HI_ERR_BAD_DATA;
    }

    if (isleap(ulYear))
    {
        mday[1] = 29;
    }

    TimeArea(1, ucTime[2], mday[ulMonth - 1]);//��
    TimeArea(1, ucTime[1], 12);// ��

    return ret;
}

// ucTime[6]: ss-mm-hh-DD-MM-YY (BCD��)
HI_U32 MRS_TimeIsValid(HI_IN HI_CONST HI_U8 ucTime[6])
{
    HI_U8 aucTemp[7];

    (hi_void)memcpy_s(aucTemp, sizeof(aucTemp), ucTime, 3);
    aucTemp[3] = 0;
    (hi_void)memcpy_s(aucTemp + 4, sizeof(aucTemp) - 4, ucTime + 3, 3);

    return MRS_TimeWeekIsValid(aucTemp);
}

/**
 * @brief ��������ʱ��Ĳ�(����ת����)
 *
 * @par Description:
 *     ���� ulTime ��ȥ ulDuration ��ֵ(���Ƿ�ת)
 *
 */
HI_U32 mrsTimeSubDuration(HI_U32 ulTime, HI_U32 ulDuration)
{
    if (ulTime >= ulDuration)
    {
        return (ulTime - ulDuration);
    }
    else
    {
        return (MRS_TIME_ROLLBACK - (ulDuration - ulTime) + 1);
    }
}

/**
 * @brief ��������ʱ��ѹ����Ĳ�(����ת������ѹ���)
 *
 * @par Description:
 *     ���� ulTime ��ȥ ulDuration ��ֵ(���Ƿ�ת)
 *
 */
HI_U32 mrsTimeSubDurationCompress(HI_U32 ulTime, HI_U32 ulDuration, HI_U32 ulCompressTimes)
{
    HI_U32 ulTimeCompress = 0;
    HI_U32 ulDurationCompress = 0;
    HI_U32 ulRollbackCompress = 0;

    if (ulCompressTimes < 1)
    {
        ulCompressTimes = 1;
    }

    ulTimeCompress = ulTime / ulCompressTimes;
    ulDurationCompress = ulDuration / ulCompressTimes;
    ulRollbackCompress = MRS_TIME_ROLLBACK / ulCompressTimes;

    if (ulTimeCompress >= ulDurationCompress)
    {
        return (ulTimeCompress - ulDurationCompress);
    }
    else
    {
        return (ulRollbackCompress - (ulDurationCompress - ulTimeCompress) + 1);
    }
}

/**
 * @brief ���� time + duration (����ת����)
 *
 * @par Description:
 *     ���� ulTime ���� ulDuration ��ֵ(���Ƿ�ת)
 *
 */
HI_U32 mrsTimeAddDuration(HI_U32 ulTime, HI_U32 ulDuration)
{
    if (MRS_TIME_ROLLBACK - ulDuration >= ulTime)
    {
        return (ulTime + ulDuration);
    }
    else
    {
        return ulDuration - (MRS_TIME_ROLLBACK - ulTime) - 1;
    }
}
HI_BOOL mrsTimeCheckRtcYearIsAfterBase(HI_U32 ulRtcTimeSec, HI_U32 ulYearBase)
{
    HI_SYS_CALENDAR_TIME_S stCalendarTime = {0};
    if (HI_ERR_SUCCESS != HI_MDM_MakeSecondToCalendar(ulRtcTimeSec, &stCalendarTime))
    {
        return HI_FALSE;
    }

    return (stCalendarTime.year > ulYearBase);
}
