/* SPDX-License-Identifier: GPL-2.0+ */
/*
 * Board initialization for EP93xx
 *
 * Copyright (C) 2013
 * Sergey Kostanbaev <sergey.kostanbaev <at> fairwaves.ru>
 *
 * Copyright (C) 2009
 * Matthias Kaehlcke <matthias <at> kaehlcke.net>
 *
 * (C) Copyright 2002 2003
 * Network Audio Technologies, Inc. <www.netaudiotech.com>
 * Adam Bezanson <bezanson <at> netaudiotech.com>
 */

/* delay execution before timers are initialized */

#include <types.h>
#include <product.h>
#include <reg.h>

#define MSDELAY_TIME_MAX ((unsigned int)((0xFFFFFFFF/CONFIG_TIMER_CLOCK)*1000))
/*lint -e529*/
void udelay(u32 usecs)
{
	/* loop takes 4 cycles at 40.0ns (fastest case) */
    /* loop num = ns/ns_perloop; ns_perloop = ns_pernop*4; ns_pernop = ns/XTAL_CLOCK */
/*
        �������ˡ�z00193780
        ������ˡ�w00190206
        �����������������ʼ�����󱨡�
        ������Ӱ�졿��Ӱ������ҵ�����θ澯
*/  
    
	register u32 loops = (usecs * 1000) / 16 + 1;//ʵ���160->16

	__asm__ volatile ("1:\n"
			"subs %0, %1, #1\n"
			"bne 1b" : "=r" (loops) : "0" (loops));  
}
/*lint +e529*/

//���ܴ���171��
void mdelay(u32 ms)//�����û��ʵ�֣�����Ż�
{
	u32 us;
	
	if (ms > MSDELAY_TIME_MAX)
		us = 0xFFFFFFFF;
	else if(ms == 0)
/*
					�������ˡ�z00193780
					������ˡ�c00233580
					�����������ͨ�ýӿڣ��п���ʹ�ø÷�֧���豣����
					������Ӱ�졿��Ӱ������ҵ�����θ澯
*/

		us = 1;
	else
		us = ms*1000;
	
	udelay(us);
}

//���Դ���
void timecnt_enable(void)
{
	u32 ulVal = 0;
	
	HI_REG_READ(HI_TIMECNT_REG_BASE + TIME_CNT_CTRL, ulVal);	
    HI_REG_WRITE(HI_TIMECNT_REG_BASE + TIME_CNT_CTRL, (ulVal | TIME_CNT_EN));
}

void timecnt_disable(void)
{
	u32 ulVal = 0;
	
	HI_REG_READ(HI_TIMECNT_REG_BASE + TIME_CNT_CTRL, ulVal);	
	ulVal &= (~TIME_CNT_EN);
    HI_REG_WRITE(HI_TIMECNT_REG_BASE + TIME_CNT_CTRL, ulVal);
}

static u32 timecnt_read_lock_enable(void)
{
	u32 ulVal = 0;
	u32 i = 0;
	
	HI_REG_READ(HI_TIMECNT_REG_BASE + TIME_CNT_CTRL, ulVal);	
    HI_REG_WRITE(HI_TIMECNT_REG_BASE + TIME_CNT_CTRL, (ulVal | TIME_RD_EN));

	// ��ѭ���з��գ�һ���ѯ1�ξ�����ϣ�Ϊ��ֹоƬ�쳣���ӳ�ʱ�жϣ�����һ����ʱ�����룬��һ���ǲ�����ֳ�ʱ��
	for (i = 0; i < TIME_RD_EN_WAIT_MAX; i++)
	{
        HI_REG_READ(HI_TIMECNT_REG_BASE + TIME_CNT_CTRL, ulVal);
		if (!(ulVal & TIME_RD_EN))
		{
			break;
		}
		
		i++;
	}

	if (i >= TIME_RD_EN_WAIT_MAX)
	{
		return (u32)(-1);	
	}

	return (u32)(-1);
}

u32 timecnt_time(void)
{
	u32 ulUs = 0;

	(void)timecnt_read_lock_enable();	
	HI_REG_READ(HI_TIMECNT_REG_BASE + TIME_US, ulUs);	
	
	return ulUs;
}

