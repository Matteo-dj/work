/**
******************************************************************************
* @file    delay.c 
* @author  zouliang
* @version V1.0
* @date    2011-11-09
* @brief   
******************************************************************************
*/
#include "delay.h"

__IO uint32_t TimingDelay = 0;
__IO uint32_t wv_ticks = 0;
__IO uint32_t cpu_loops_1s = 0;;

/*******************************************************************************
* Function Name  : Delayms
* Description    : Inserts a delay time.
* Input          : nTime: specifies the delay time length, in milliseconds.
* Output         : None
* Return         : None
*******************************************************************************/
void Delay(__IO u32 nTime)
{
  TimingDelay = nTime; 
  while (TimingDelay != 0);
}

/*******************************************************************************
* Function Name  : TimingDelay_Decrement
* Description    : Decrements the TimingDelay variable.
* Input          : None
* Output         : TimingDelay
* Return         : None
*******************************************************************************/
void TimingDelay_Decrement(void)
{
  if (TimingDelay > 0x00)
  { 
    TimingDelay--;
  }
  if(wv_ticks < 0xFFFFFFFF)
  {
    wv_ticks++;
  }
  else
  {
    wv_ticks = 0;
  }
}

//CPU指令周期测试指令
void CpuSpeed(u8 ticks)
{
  u32 tmp=1;
  u32 i,t0;
  u32 j=0xFFFFFFFE;
	
  t0=wv_ticks;
  for (i=0;i<=j;i++) if (wv_ticks>t0) break;               
  t0=wv_ticks+ticks;
  for (i=0;i<=j;i++) if (wv_ticks>t0) break;               
  
  if (TICKS_PER_SECOND) tmp=TICKS_PER_SECOND;
  tmp=tmp*i/(ticks+1);
  cpu_loops_1s=(u32)(tmp);
  return;
}

/*------------------us延时------------------*/
void usDelay(u32 us)
{
  u32 i,t0;

  us=us*25;
  t0=wv_ticks+5;
  for (i=0;i<=us;i++) 
  if (wv_ticks>t0) 
  {
    break;
  }
}

/*------------------ms延时------------------*/
void msDelay(u32 ms)
{
 u32 i,t0;
	
 ms=ms*cpu_loops_1s/1000L;
 t0=wv_ticks+TICKS_PER_SECOND;
 for (i=0;i<=ms;i++)
 if (wv_ticks>t0) 
 {
  break;
 }
}