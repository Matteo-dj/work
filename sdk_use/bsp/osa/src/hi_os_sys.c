#include <hi_types.h>
#include <hi_mdm_types.h>
#include <los_sys.h>
#include <hi_stdlib.h>
#include <time.h>
#include <hi_config.h>
#include <hi_mdm_time.h>
#include "hi_os_stat.h"
#include <hi_platform.h>//g_cfg_arm_clock

OS_UDELAY_FUNC g_pfnUdelayFunc=HI_NULL;


HI_U32 ms2systick (HI_IN HI_U32 ulms, HI_IN HI_BOOL include0)
{
    HI_U32 ultick;

    //>10ms向下对齐
    if (ulms > HI_MILLISECOND_PER_TICK)
    {
        ultick = ulms / HI_MILLISECOND_PER_TICK; /*convert from ms to ticks*/
    }
    //<10ms向上对齐
    else
    {
        if ((HI_TRUE == include0) && (0 == ulms))
        {
            ultick = 0;
        }
        else
        {
            ultick = 1;
        }
    }

    return ultick;
}



HI_U32 SAL_IsTmLegal(HI_IN HI_SYS_CALENDAR_TIME_S * calendarTime)
{
    if((calendarTime->sec >= 60)||(calendarTime->min >= 60)||(calendarTime->hour >= 24)
     ||((calendarTime->day < 1)||(calendarTime->day > 31))
     ||((calendarTime->month < 1)||(calendarTime->month > 12))
     ||((calendarTime->year < 1970)||(calendarTime->year >= 2100)))
    {
        return HI_ERR_INVALID_PARAMETER;
    }

    switch (calendarTime->month)
    {
    case 2:
        if(calendarTime->year%400 == 0 || (calendarTime->year%100 != 0 && calendarTime->year % 4 == 0))
        {
            if(calendarTime->day > 29)
               return HI_ERR_INVALID_PARAMETER;
        }
        else
        {
            if(calendarTime->day > 28)
               return HI_ERR_INVALID_PARAMETER;
        }
        break;
    case 4:
    case 6:
    case 9:
    case 11:
        if(calendarTime->day > 30)
            return HI_ERR_INVALID_PARAMETER;
        break;
    default:
        break;
    }
    return HI_ERR_SUCCESS;
}
void osa_udelay(unsigned long us)
{
	unsigned long busclk;

	busclk = g_cfg_arm_clock;
	while(us--)
	{
		volatile unsigned long t = (busclk>>3)/1000000;
		while(t--)
			__asm("nop"); /*lint !e718 !e746*/
	}
}


HI_U32 HI_MDM_Udelay(HI_U32 ulus)
{
    if(g_pfnUdelayFunc)
        return g_pfnUdelayFunc(ulus);

    osa_udelay(ulus);
    return HI_ERR_SUCCESS;
    
}

HI_U32 HI_MDM_RegisterUdealyFunc(OS_UDELAY_FUNC pFunc)
{
    g_pfnUdelayFunc=pFunc;
    return HI_ERR_SUCCESS;
}

HI_U32 HI_MDM_GetTick(HI_VOID)
{
    UINT32 uwCntLo;
    uwCntLo = (UINT32)(LOS_TickCountGet()&0xffffffff);
    return uwCntLo;
}

HI_U64 HI_MDM_GetTick64(HI_VOID)
{
    return LOS_TickCountGet();
}


HI_U32 HI_MDM_GetMilliSeconds(HI_VOID)
{
    UINT32 uwCntLo;
    uwCntLo = (UINT32)(LOS_TickCountGet()&0xffffffff);
    return (uwCntLo*HI_MILLISECOND_PER_TICK);

}

HI_U32 HI_MDM_GetSeconds(HI_VOID)
{
    struct timespec tp;

    if (clock_gettime (CLOCK_MONOTONIC, &tp) == 0)
	{
	    return (HI_U32) (tp.tv_sec);
	}
    else
    {
	    return (HI_U32) (HI_ERR_FAILURE);
    }

}

HI_U32 HI_MDM_GetRealTime(HI_VOID)
{
    struct timespec tp;
    if (clock_gettime (CLOCK_REALTIME, &tp) == 0)
	{
	    return (HI_U32) (tp.tv_sec);
	}
    else
    {
	    return (HI_U32) (HI_ERR_FAILURE);
    }
}

HI_U32 HI_MDM_SetRealTime(HI_U32 ulSec)
{
    struct timespec tp;

    memset_s((HI_VOID *)&tp,sizeof(struct timespec),0x0,sizeof(struct timespec));
    tp.tv_sec = (int)ulSec;
    tp.tv_nsec = 0;
    if (clock_settime (CLOCK_REALTIME, &tp) == 0)
	{
	    return (HI_U32) (HI_ERR_SUCCESS);
	}
    else
    {
	    return (HI_U32) (HI_ERR_FAILURE);
    }
}



HI_U32 HI_MDM_SetCalendarTime(HI_IN HI_SYS_CALENDAR_TIME_S * calendarTime) //建议删除
{

    HI_U32 second = 0;
    if(calendarTime==HI_NULL)
    {
        return HI_ERR_INVALID_PARAMETER;
    }

    if(HI_ERR_SUCCESS != HI_MDM_MakeCalendarToSecond(calendarTime,&second))
    {
        return HI_ERR_FAILURE;
    }
    return HI_MDM_SetRealTime(second);
}


HI_U32 HI_MDM_GetCalendarTime(HI_OUT HI_SYS_CALENDAR_TIME_S * calendarTime) //建议删除
{
    HI_U32 second = HI_MDM_GetRealTime();
    if(calendarTime==HI_NULL)
    {
        return HI_ERR_INVALID_PARAMETER;
    }
    return HI_MDM_MakeSecondToCalendar(second,calendarTime);
}


HI_U32 HI_MDM_MakeSecondToCalendar(HI_IN HI_U32 second,HI_OUT HI_SYS_CALENDAR_TIME_S * calendarTime)
{
    struct tm *tmTime;
    time_t tsecond = (time_t)(second);
    if(calendarTime==HI_NULL)
    {
        return HI_ERR_INVALID_PARAMETER;
    }

    tmTime = gmtime(&tsecond);
    if(tmTime==HI_NULL)
    {
        return HI_ERR_SYSTEM_CALL_ERROR;
    }
    
    calendarTime->year = (HI_U16)(tmTime->tm_year + TM_YEAR_BASE);
    calendarTime->month = (HI_U8)(tmTime->tm_mon + 1);
    calendarTime->day= (HI_U8)tmTime->tm_mday;
    calendarTime->hour=(HI_U8) tmTime->tm_hour;
    calendarTime->min= (HI_U8)tmTime->tm_min;
    calendarTime->sec= (HI_U8)tmTime->tm_sec;

    return HI_ERR_SUCCESS;
}


HI_U32 HI_MDM_MakeCalendarToSecond(HI_IN HI_SYS_CALENDAR_TIME_S * calendarTime,HI_OUT HI_U32 *second)
{
    time_t tsecond;
    struct tm  tmTime;
    memset_s((HI_VOID *)&tmTime,sizeof(struct tm),0x0,sizeof(struct tm));
    if(calendarTime==HI_NULL||second==HI_NULL)
    {
        return HI_ERR_INVALID_PARAMETER;
    }
    /* add by y00281951 to check if the time is legal 问题单号:DTS2014072500142 */
    if( HI_ERR_INVALID_PARAMETER == SAL_IsTmLegal(calendarTime))
    {
        return HI_ERR_INVALID_PARAMETER;
    }
    /* end */
    tmTime.tm_sec = calendarTime->sec;
    tmTime.tm_min = calendarTime->min;
    tmTime.tm_hour = calendarTime->hour;
    tmTime.tm_mday = calendarTime->day;
    tmTime.tm_mon = calendarTime->month - 1;
    tmTime.tm_year = calendarTime->year -TM_YEAR_BASE;
    tsecond = mktime(&tmTime);//将时间转换成s
    if( tsecond ==(time_t)(-1))
    {
        return (HI_U32) (HI_ERR_FAILURE);
    }
    else
    {
        *second = (HI_U32)tsecond;
        return HI_ERR_SUCCESS;
    }
    
}

HI_S32 HI_MDM_DataCacheInvalidate(HI_PVOID pAddress, HI_U32 lSize)
{    
    arm926_dma_inv_range((int)pAddress, (int)((HI_U8*)pAddress + lSize));
    return 0;
}
HI_S32 HI_MDM_DataCacheFlush(HI_PVOID pAddress, HI_U32 lSize)
{
    arm926_dma_clean_range((int)pAddress, (int)((HI_U8*)pAddress + lSize));
    return 0;
}

