/*
** watchdog driver for HI3911V100
** Created by z00193780 at 2012-02-16
** Changed by w00190206 at 2017-08-19 for miniboot 
*/

/* includes */
#include <reg.h>
#include <types.h>
#include <watchdog.h>


/* function HI_Watchdog_Enable 
 * parauint: us
 * real range:(1M for example:65ms~4295s)
 * 内部使用，此函数忽略进行入参判断  0 < u32over_time < 0x7FFFFFFF/WDG_CLK_uS
 */
u32 watchdog_enable(u32 u32over_time)
{
    u32 dirvalue = 0;
    u32 i;
    u32 time = 0; //写入寄存器的2的次幂
    u32 count = 0; //计算dirvalue的二进制中 0x1 的个数
    
    /*config impulse width*/
    HI_REG_READ((HI_WDG_REG_BASE + WDT_CR) , dirvalue);
    dirvalue |= (0x7 << 2);	
    HI_REG_WRITE((HI_WDG_REG_BASE + WDT_CR) , dirvalue);

    //模式选择寄存器，boot下只产生复位信号
    HI_REG_READ((HI_WDG_REG_BASE + WDT_CR) , dirvalue);
    dirvalue &= (~(u32)(0x1 << 1));
    HI_REG_WRITE((HI_WDG_REG_BASE + WDT_CR) , dirvalue);

    /*config overtime length*/ 
    dirvalue = (u32)(((u64)u32over_time*(u64)WDG_CLK_uS)>>16);   

    //寄存器[3:0]有效，可输入最大值为15
    for(i=0;i<15;i++)
    {
        
        if(dirvalue)
        {
            if(dirvalue & WDG_BIT_0_IS_1)
            {
                count++;
            }
            time ++;
            dirvalue = dirvalue>>1;
        }
        else
        {
			//dirvalue的最高位为1的情况，如: dirvalue = 1000(二进制)时，time减1
            if (count == 1)
            {
                time -= 1;
            }
            break;
        }
    }
    
    HI_REG_WRITE((HI_WDG_REG_BASE + WDT_TORR) , time); 
    
    /*system reset enable*/
    HI_REG_READ((sysctrl_base_addr + SC_WDG_RST_CTRL) , dirvalue);
    dirvalue |= (1 << 3);
    HI_REG_WRITE((sysctrl_base_addr + SC_WDG_RST_CTRL) , dirvalue);


    HI_REG_READ((sysctrl_base_addr + SC_CTRL) , dirvalue);
    dirvalue &= (~(u32)(0x1 << 23 ) );
    dirvalue |= (1 << 23);		
    HI_REG_WRITE((sysctrl_base_addr + SC_CTRL) , dirvalue);

    /*open watchdog*/
    HI_REG_READ((HI_WDG_REG_BASE + WDT_CR) , dirvalue);
    dirvalue |= (0x01 << 0);		
    HI_REG_WRITE((HI_WDG_REG_BASE + WDT_CR) , dirvalue);
    
    /*reset*/ 		
    HI_REG_WRITE((HI_WDG_REG_BASE + WDT_CRR), 0x76);
    
    return 0;
}
void watchdog_feed(void)
{
    u32 dirvalue = 0;
	
    /*reset*/ 
    dirvalue = 0x76;		
    HI_REG_WRITE((HI_WDG_REG_BASE + WDT_CRR), dirvalue);
}
void watchdog_disable(void)
{
    u32 dirvalue = 0;	

    //clk
    HI_REG_READ((sysctrl_base_addr + SC_CTRL),dirvalue);	
    dirvalue &= (~(u32)(0x1 << 23 ) );
    HI_REG_WRITE((sysctrl_base_addr + SC_CTRL) , dirvalue);

    //reset
    HI_REG_READ((sysctrl_base_addr + SC_WDG_RST_CTRL),dirvalue);	
    dirvalue &= (~(u32)(0x1 << 3 ) );
    HI_REG_WRITE((sysctrl_base_addr + SC_WDG_RST_CTRL) , dirvalue);
}

