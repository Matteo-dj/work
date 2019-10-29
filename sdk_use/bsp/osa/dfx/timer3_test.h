#ifndef __HI_HRTIMER4_H__
#define __HI_HRTIMER4_H__

#define UINT32 unsigned int

#define TIMER_REG_BASE	(0x101E203c)


#define TIMER0_LOADCOUNT_REG		(0x00)
#define TIMER0_CURRCOUNT_REG		(0x04)
#define TIMER0_CONTROL_REG		(0x08)
#define TIMER0_EOI_REG			(0x0c)
#define TIMER0_INTSTATUS_REG		(0x10)

#define  TIMER0_ENABLE_BIT		(1 << 0)
#define  TIMER0_ONESHOT_BIT		(1 << 1)
#define  TIMER0_INTEN_BIT		(1 << 2)
#define  TIMER0_LOCK_BIT		(1 << 3)


//extern HI_U32  HI_UART_Printf(HI_CONST HI_CHAR *fmt, ...);
/******************************************************
*
*Only for test!
*
*
*******************************************************/
UINT32 Timer_start_count =0xFFFFFFFF;
UINT32 Timer_end_count =0;


#define Timer_writel(reg, val)\
	*(volatile unsigned int *)(TIMER_REG_BASE + (reg)) = (unsigned int)(val)

#define Timer_readl(reg)\
	*(volatile unsigned int *)(TIMER_REG_BASE + (reg))

VOID  Running_Time_Calc_Start()
{
	UINT32 val;
/*************** disable INT***************************/
	val = Timer_readl(TIMER0_CONTROL_REG);
	val |= TIMER0_INTEN_BIT;
	Timer_writel(TIMER0_CONTROL_REG, val);

/*************** disable timer4************************/	
	val = Timer_readl(TIMER0_CONTROL_REG);
	val &= ~TIMER0_ENABLE_BIT;
	Timer_writel(TIMER0_CONTROL_REG, val);
	
/*************** setting srart count*********************/
	Timer_writel(TIMER0_LOADCOUNT_REG, 0xFFFFFFFF);

/*************** setting running mode*******************/
	val = Timer_readl(TIMER0_CONTROL_REG);
	val &=~ TIMER0_ONESHOT_BIT;
	Timer_writel(TIMER0_CONTROL_REG, val);
	
/*************** enable timer4*************************/
	val = Timer_readl(TIMER0_CONTROL_REG);
	val |= TIMER0_ENABLE_BIT;
	Timer_writel(TIMER0_CONTROL_REG, val);
///////////add to read back the start value///////////////
/*************** lock timer4************************/	
	val = Timer_readl(TIMER0_CONTROL_REG);
	val |= TIMER0_LOCK_BIT;
	Timer_writel(TIMER0_CONTROL_REG, val);

    while(Timer_readl(TIMER0_CONTROL_REG)&TIMER0_LOCK_BIT);

/*************** read timer4************************/
	Timer_start_count = Timer_readl(TIMER0_CURRCOUNT_REG);
////////////////end ///////////////
	
}

UINT32 Running_Time_Calc_end()
{
	UINT32 val;
	//UINT32 running_time =0;
	float  running_time =0;

/*************** lock timer4************************/	
	val = Timer_readl(TIMER0_CONTROL_REG);
	val |= TIMER0_LOCK_BIT;
	Timer_writel(TIMER0_CONTROL_REG, val);

    while(Timer_readl(TIMER0_CONTROL_REG)&TIMER0_LOCK_BIT);

/*************** read timer4************************/
	Timer_end_count = Timer_readl(TIMER0_CURRCOUNT_REG);

/*************** disable timer4************************/	
	val = Timer_readl(TIMER0_CONTROL_REG);
	val &= ~TIMER0_ENABLE_BIT;
	Timer_writel(TIMER0_CONTROL_REG, val);
/* BEGIN: Modified by wuxianfeng190206, 2012/3/14   ÎÊÌâµ¥ºÅ:DTS2012031207871 */
    running_time =  ((float)(Timer_start_count-Timer_end_count)) / 25; 
/* END:   Modified by wuxianfeng190206, 2012/3/14 */
    printf("start running time is (%d)us! \r\n",(UINT32)running_time);
    return (UINT32)running_time;
       
}

#endif
