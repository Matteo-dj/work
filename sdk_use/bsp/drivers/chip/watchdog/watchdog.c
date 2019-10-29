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
//�޸�Ϊֱ����lisrִ�У��Է��ж��й�ס
	HI_VOID (*entry)(HI_VOID) = (HI_VOID(*)(HI_VOID)) wdg_func;
	 
	entry();
}


/* ʹ�ܿ��Ź� */
HI_U32 HI_WDG_Enable(HI_WDG_CTRL_S *pstWdgType)
{
    HI_U32 i;
    HI_U32 ulDirValue = 0;
    HI_U32 ulTime = 0;//д��Ĵ�����2�Ĵ���
    HI_U32 ulCount = 0;//����ulDirValue�Ķ������� 0x1 �ĸ���
    HI_U32 ulRet = HI_ERR_FAILURE;
    
    //�����ж�
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
    
    //�Ĵ���[3:0]��Ч�����������ֵΪ15
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
			//ulDirValue�����λΪ1���������: ulDirValue = 1000(������)ʱ��ulTime��1
            if (ulCount == 1)
            {
                ulTime -= 1;
            }
            break;
        }
    }

    /*config overtime length��ʱ���*/
    ulDirValue = ulTime;		
    HI_REG_WRITE32((HI_WDG_REG_BASE + WDT_TORR) , ulDirValue); 

    /*config impulse width��λ������*/
    HI_REG_READ32((HI_WDG_REG_BASE + WDT_CR) , ulDirValue);
    ulDirValue &= ( ~(HI_U32)(0x7 << 2 ) );
    ulDirValue |= (0x7 << 2);		
    HI_REG_WRITE32((HI_WDG_REG_BASE + WDT_CR) , ulDirValue);

    /*config mode just resetģʽ*/
    HI_REG_READ32((HI_WDG_REG_BASE + WDT_CR) , ulDirValue);
    ulDirValue &= (~(HI_U32)(0x1 << 1 ) );
    ulDirValue |= ((HI_U32)(pstWdgType->enMode)<< 1);		
    HI_REG_WRITE32((HI_WDG_REG_BASE + WDT_CR) , ulDirValue);
      
    /*system clk enable*/


    /*system reset enable*/
    //wdg_rst_en ���Ź�����ʹ��  1: ʹ��   0: ����
    HI_REG_READ32((HI_SYSCTRL_REG_BASE + SC_WDG_RST_CTRL) , ulDirValue);
    ulDirValue &= (~(HI_U32)(0x1 << 3 ) );
    ulDirValue |= (1 << 3);		
    HI_REG_WRITE32((HI_SYSCTRL_REG_BASE + SC_WDG_RST_CTRL) , ulDirValue);

    //wdg_en_ov ���Ź�ʱ��ʹ��   1: ʹ��   0����ֹ  Ĭ������Ϊ0
    HI_REG_READ32((HI_SYSCTRL_REG_BASE + SC_CTRL) , ulDirValue);
    ulDirValue &= (~(HI_U32)(0x1 << 23 ) );
    ulDirValue |= (1 << 23);		
    HI_REG_WRITE32((HI_SYSCTRL_REG_BASE + SC_CTRL) , ulDirValue);
	
    /*open watchdogʹ�ܿ��Ź�*/
    HI_REG_READ32((HI_WDG_REG_BASE + WDT_CR) , ulDirValue);
    ulDirValue &= (~(HI_U32)(0x1 << 0 ) );
    ulDirValue |= (0x01 << 0);		
    HI_REG_WRITE32((HI_WDG_REG_BASE + WDT_CR) , ulDirValue);
    
    /*reset���������������������߹�*/ 
    ulDirValue = 0x76;		
    HI_REG_WRITE32((HI_WDG_REG_BASE + WDT_CRR), ulDirValue);
    
    return HI_ERR_SUCCESS;
}

HI_VOID HI_MDM_WDG_Enable(HI_VOID)
{
    HI_U32 ulDirValue = 0;

	/*system clk enable*/
    /*system reset enable*/
    //wdg_rst_en ���Ź�����ʹ��  1: ʹ��   0: ����
    HI_REG_READ32((HI_SYSCTRL_REG_BASE + SC_WDG_RST_CTRL) , ulDirValue);
    ulDirValue &= (~(HI_U32)(0x1 << 3 ) );
    ulDirValue |= (1 << 3);		
    HI_REG_WRITE32((HI_SYSCTRL_REG_BASE + SC_WDG_RST_CTRL) , ulDirValue);

        //wdg_en_ov ���Ź�ʱ��ʹ��   1: ʹ��   0����ֹ  Ĭ������Ϊ0
    HI_REG_READ32((HI_SYSCTRL_REG_BASE + SC_CTRL) , ulDirValue);
    ulDirValue &= (~(HI_U32)(0x1 << 23 ) );
    ulDirValue |= (1 << 23);		
    HI_REG_WRITE32((HI_SYSCTRL_REG_BASE + SC_CTRL) , ulDirValue);
}




/* ι��*/
HI_VOID HI_MDM_WDG_Feed(HI_VOID)
{
    HI_U32 ulDirValue = 0;
	
    /*reset������������������,�߹�*/ 
    ulDirValue = 0x76;	
    HI_REG_WRITE32((HI_WDG_REG_BASE + WDT_CRR), ulDirValue);
}

/* ȥʹ�ܿ��Ź�*/
HI_VOID HI_MDM_WDG_Disable(HI_VOID)
{
    HI_U32 ulDirValue = 0;	

    //clkֹͣ���Ź�ʱ��
    HI_REG_READ32((HI_SYSCTRL_REG_BASE + SC_CTRL),ulDirValue);	
    ulDirValue &= (~(HI_U32)(0x1 << 23 ) );
    HI_REG_WRITE32((HI_SYSCTRL_REG_BASE + SC_CTRL) , ulDirValue);

    //reset���ο��Ź�����
    HI_REG_READ32((HI_SYSCTRL_REG_BASE + SC_WDG_RST_CTRL),ulDirValue);	
    ulDirValue &= (~(HI_U32)(0x1 << 3 ) );
    HI_REG_WRITE32((HI_SYSCTRL_REG_BASE + SC_WDG_RST_CTRL) , ulDirValue);
}



