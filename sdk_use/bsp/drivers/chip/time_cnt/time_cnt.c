/*
** time_cnt driver for HI3911V200
** Created by z00193780 at 2017-03-02
**  
*/

/* includes */

#include <hi_platform.h>
#include "time_cnt.h"

HI_VOID HI_TIME_CNT_Enable(HI_VOID)
{
	HI_U32 ulVal = 0;
	
	HI_TIME_CNT_REG_READ(TIME_CNT_CTRL, ulVal);	
    HI_TIME_CNT_REG_WRITE(TIME_CNT_CTRL, (ulVal | TIME_CNT_EN));
}

HI_VOID HI_TIME_CNT_Disable(HI_VOID)
{
	HI_U32 ulVal = 0;
	
	HI_TIME_CNT_REG_READ(TIME_CNT_CTRL, ulVal);	
	ulVal &= (~TIME_CNT_EN);
    HI_TIME_CNT_REG_WRITE(TIME_CNT_CTRL, ulVal);
}

HI_PRV HI_U32 time_cnt_read_lock_enable(HI_VOID)
{
	HI_U32 ulVal = 0;
	HI_U32 i = 0;
	
	HI_TIME_CNT_REG_READ(TIME_CNT_CTRL, ulVal);	
    HI_TIME_CNT_REG_WRITE(TIME_CNT_CTRL, (ulVal | TIME_RD_EN));

	// ��ѭ���з��գ�һ���ѯ1�ξ�����ϣ�Ϊ��ֹоƬ�쳣���ӳ�ʱ�жϣ�����һ����ʱ�����룬��һ���ǲ�����ֳ�ʱ��
	for (i = 0; i < TIME_RD_EN_WAIT_MAX; i++)
	{
        HI_TIME_CNT_REG_READ(TIME_CNT_CTRL, ulVal);
		if (!(ulVal & TIME_RD_EN))
		{
			break;
		}
		
		i++;
	}

	if (i >= TIME_RD_EN_WAIT_MAX)
	{
		return HI_ERR_TIMEOUT;	
	}

	return HI_ERR_SUCCESS;
}

HI_PRV HI_U32 time_cnt_get_time(HI_U32 *pulUs, HI_U32 *pulMs, HI_U32 *pulSec)
{
	HI_U32 ulUs = 0;
	HI_U32 ulMs = 0;
	HI_U32 ulSec = 0;
	HI_U32 ulRet = HI_ERR_FAILURE;

	ulRet = time_cnt_read_lock_enable();
	if (ulRet)
	{
		return ulRet;
	}
	
	HI_TIME_CNT_REG_READ(TIME_US, ulUs);	
	HI_TIME_CNT_REG_READ(TIME_MS, ulMs);
	HI_TIME_CNT_REG_READ(TIME_S, ulSec);
	
	if (!pulUs && !pulMs && !pulSec)
	{
		return HI_ERR_INVALID_PARAMETER;
	}

	if (pulUs)
	{
		*pulUs = ulUs;
	}
	
	if (pulMs)
	{
		*pulMs = ulMs;
	}

	if (pulSec)
	{
		*pulSec = ulSec;
	}
	
	return HI_ERR_SUCCESS;
}

HI_U32 HI_TIME_CNT_GetTime(HI_U32 *pulUs, HI_U32 *pulMs, HI_U32 *pulSec)
{
	return time_cnt_get_time(pulUs, pulMs, pulSec);
}

HI_U32 HI_TIME_CNT_GetSec(HI_U32 *pulSec)
{	
	return time_cnt_get_time(HI_NULL, HI_NULL, pulSec);
}

HI_U32 HI_TIME_CNT_GetMs(HI_U32 *pulMs)
{
	// ��ת����Ҫ������Ӧ�ò���Ҫ�ɣ�
	return time_cnt_get_time(HI_NULL, pulMs, HI_NULL);
}

HI_U32 HI_TIME_CNT_GetUs(HI_U32 *pulUs)
{
	return time_cnt_get_time(pulUs, HI_NULL, HI_NULL);
}

