/*
** watchdog driver for HI3911V100
** Created by z00193780 at 2012-02-16
**  
*/

#include "watchdog.h"
#include <hi_driver.h>
#include <hi_inner_watchdog.h>
#ifndef __VX_PORTING_WATCHDOG__
#include <hi_drv_com.h>
#include <hi_platform.h>
#endif /* __VX_PORTING_WATCHDOG__ */
#include <os_vx_sal_api.h>

#ifndef NULL
#define NULL 0
#endif
//#define TERMINAL_DISPLAY
/* debug macros */    
#ifndef TERMINAL_DISPLAY
#define watchdog_error(_fmt, arg...) 
#else
#define watchdog_error(_fmt, arg...) \
    HI_UART_Printf("%s(%d): " _fmt, __FILE__, __LINE__, ##arg)
#endif

static  HI_VOID wdg_lisr(HI_VOID * wdg_func)
{
//修改为直接在lisr执行，以防中断中挂住
	HI_VOID (*entry)(HI_VOID) = (HI_VOID(*)(HI_VOID)) wdg_func;
	 
	entry();
}


/* 使能看门狗 */
HI_U32 HI_WDG_Enable(HI_WDG_CTRL_S *pstWdgType)
{
    HI_U32 i;
    HI_U32 ulDirValue = 0;
    HI_U32 ulTime = 0;//写入寄存器的2的次幂
    HI_U32 ulCount = 0;//计算ulDirValue的二进制中 0x1 的个数
    HI_U32 ulRet = HI_ERR_FAILURE;
    
    //参数判断
    HI_ASSERT((pstWdgType->enMode == HI_WDG_NORMAL) || (pstWdgType->enMode == HI_WDG_INTER));
    /*check parameters*/
    if ((pstWdgType->ulOverTime)> WDG_RESET_TIME_MAX ) 
    {
        watchdog_error("config watchdog parameters wrong!\r\n");
        return HI_ERR_WATCHDOG_PARA_ERROR;
    }
    
    if(pstWdgType->enMode == HI_WDG_INTER)
    {

 	    HI_ASSERT(pstWdgType->func);

		HI_MDM_IRQ_Disable(HI_IRQ_WDT);
    	ulRet = HI_MDM_IRQ_Request(HI_IRQ_WDT, 0,wdg_lisr, (HI_U32)pstWdgType->func);
		HI_MDM_IRQ_Enable(HI_IRQ_WDT);
        if (HI_ERR_SUCCESS != ulRet)
        {
            return ulRet;
        }
       
    }

    ulDirValue = (HI_U32)(((pstWdgType->ulOverTime)*WDG_CLK_mS)>>16);
    
    //寄存器[3:0]有效，可输入最大值为15
    for(i=0;i<15;i++)
    {
        
        if(ulDirValue)
        {
            if(ulDirValue & WDG_BIT_0_IS_1)
            {
                ulCount++;
            }
            ulTime++;
            ulDirValue = ulDirValue>>1;
        }
        else
        {
			//ulDirValue的最高位为1的情况，如: ulDirValue = 1000(二进制)时，ulTime减1
            if (ulCount == 1)
            {
                ulTime -= 1;
            }
            break;
        }
    }

    /*config overtime length超时间隔*/
    ulDirValue = ulTime;		
    HI_REG_WRITE32((HI_WDG_REG_BASE + WDT_TORR) , ulDirValue); 

    /*config impulse width复位脉冲宽度*/
    HI_REG_READ32((HI_WDG_REG_BASE + WDT_CR) , ulDirValue);
    ulDirValue &= ( ~(HI_U32)(0x7 << 2 ) );
    ulDirValue |= (0x7 << 2);		
    HI_REG_WRITE32((HI_WDG_REG_BASE + WDT_CR) , ulDirValue);

    /*config mode just reset模式*/
    HI_REG_READ32((HI_WDG_REG_BASE + WDT_CR) , ulDirValue);
    ulDirValue &= (~(HI_U32)(0x1 << 1 ) );
    ulDirValue |= ((HI_U32)(pstWdgType->enMode)<< 1);		
    HI_REG_WRITE32((HI_WDG_REG_BASE + WDT_CR) , ulDirValue);
      
    /*system clk enable*/


    /*system reset enable*/
    //wdg_rst_en 看门狗功能使能  1: 使能   0: 屏蔽
    HI_REG_READ32((HI_SYSCTRL_REG_BASE + SC_WDG_RST_CTRL) , ulDirValue);
    ulDirValue &= (~(HI_U32)(0x1 << 3 ) );
    ulDirValue |= (1 << 3);		
    HI_REG_WRITE32((HI_SYSCTRL_REG_BASE + SC_WDG_RST_CTRL) , ulDirValue);

    //wdg_en_ov 看门狗时钟使能   1: 使能   0：禁止  默认配置为0
    HI_REG_READ32((HI_SYSCTRL_REG_BASE + SC_CTRL) , ulDirValue);
    ulDirValue &= (~(HI_U32)(0x1 << 23 ) );
    ulDirValue |= (1 << 23);		
    HI_REG_WRITE32((HI_SYSCTRL_REG_BASE + SC_CTRL) , ulDirValue);
	
    /*open watchdog使能看门狗*/
    HI_REG_READ32((HI_WDG_REG_BASE + WDT_CR) , ulDirValue);
    ulDirValue &= (~(HI_U32)(0x1 << 0 ) );
    ulDirValue |= (0x01 << 0);		
    HI_REG_WRITE32((HI_WDG_REG_BASE + WDT_CR) , ulDirValue);
    
    /*reset重新启动计数器计数，踢狗*/ 
    ulDirValue = 0x76;		
    HI_REG_WRITE32((HI_WDG_REG_BASE + WDT_CRR), ulDirValue);
    
    return HI_ERR_SUCCESS;
}

HI_VOID HI_MDM_WDG_Enable(HI_VOID)
{
    HI_U32 ulDirValue = 0;

	/*system clk enable*/
    /*system reset enable*/
    //wdg_rst_en 看门狗功能使能  1: 使能   0: 屏蔽
    HI_REG_READ32((HI_SYSCTRL_REG_BASE + SC_WDG_RST_CTRL) , ulDirValue);
    ulDirValue &= (~(HI_U32)(0x1 << 3 ) );
    ulDirValue |= (1 << 3);		
    HI_REG_WRITE32((HI_SYSCTRL_REG_BASE + SC_WDG_RST_CTRL) , ulDirValue);

        //wdg_en_ov 看门狗时钟使能   1: 使能   0：禁止  默认配置为0
    HI_REG_READ32((HI_SYSCTRL_REG_BASE + SC_CTRL) , ulDirValue);
    ulDirValue &= (~(HI_U32)(0x1 << 23 ) );
    ulDirValue |= (1 << 23);		
    HI_REG_WRITE32((HI_SYSCTRL_REG_BASE + SC_CTRL) , ulDirValue);
}




/* 喂狗*/
HI_VOID HI_MDM_WDG_Feed(HI_VOID)
{
    HI_U32 ulDirValue = 0;
	
    /*reset重新启动计数器计数,踢狗*/ 
    ulDirValue = 0x76;	
    HI_REG_WRITE32((HI_WDG_REG_BASE + WDT_CRR), ulDirValue);
}

/* 去使能看门狗*/
HI_VOID HI_MDM_WDG_Disable(HI_VOID)
{
    HI_U32 ulDirValue = 0;	

    //clk停止看门狗时钟
    HI_REG_READ32((HI_SYSCTRL_REG_BASE + SC_CTRL),ulDirValue);	
    ulDirValue &= (~(HI_U32)(0x1 << 23 ) );
    HI_REG_WRITE32((HI_SYSCTRL_REG_BASE + SC_CTRL) , ulDirValue);

    //reset屏蔽看门狗功能
    HI_REG_READ32((HI_SYSCTRL_REG_BASE + SC_WDG_RST_CTRL),ulDirValue);	
    ulDirValue &= (~(HI_U32)(0x1 << 3 ) );
    HI_REG_WRITE32((HI_SYSCTRL_REG_BASE + SC_WDG_RST_CTRL) , ulDirValue);
}



