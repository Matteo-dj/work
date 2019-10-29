#include "los_hwi.h"
extern UINT32 time_clk_read(void);

static unsigned long long cycles_2_ns(unsigned long cyc)
{
    unsigned long long ret64;
    double cyc2ns_scale = 1000000000.0/OS_TIME_TIMER_CLOCK;

    ret64 = (unsigned long long)(cyc * cyc2ns_scale);

    return ret64;
}

static volatile unsigned long long sched_clock_ns = 0;
static volatile unsigned long time_clk_last = 0;

static unsigned long update_time_clk(unsigned long *time_clk32)
{
    unsigned long time_clk_now;

    time_clk_now = time_clk_read();
    if(time_clk_now >= time_clk_last)
    {
        *time_clk32 = (unsigned long)(time_clk_now - time_clk_last);
    }
    else
    {
        *time_clk32 = (unsigned long)(time_clk_now + (0UL - time_clk_last));
    }
    return time_clk_now;
}

void update_sched_clock(unsigned int arg)
{
    unsigned long time_clk_now, time_clk32;
    unsigned int uwIntSave;

    uwIntSave = LOS_IntLock();
    time_clk_now = update_time_clk(&time_clk32);
    time_clk_last = time_clk_now;
    sched_clock_ns += cycles_2_ns(time_clk32);

    (void)LOS_IntRestore(uwIntSave);
}

static unsigned long long get_sched_clock(void)
{
    unsigned long time_clk32;
    update_time_clk(&time_clk32);/*lint !e534*/

    return  (sched_clock_ns + cycles_2_ns(time_clk32));
}

unsigned long long sched_clock(void)
{
    unsigned int uwIntSave;
    unsigned long long time_clk64;

    uwIntSave = LOS_IntLock();
    time_clk64 = get_sched_clock();
    (void)LOS_IntRestore(uwIntSave);

    return time_clk64;
}

unsigned long long hi_sched_clock(void)
{
    unsigned long long c;
    c = sched_clock();
    return c;
}

