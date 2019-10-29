
/******************************************************************************

                  ��Ȩ���� (C), 2001-2011, ��Ϊ�������޹�˾

 ******************************************************************************
  ��������   : 2017��11��1��
  ��������   : ����ϵͳ�Ĵ���
  �޸���ʷ   :
******************************************************************************/
// TODO:���ļ���Ҫ֧�ֻ����������,�������зǻ�����,���зǻ�������ʹ��#ifndef HI_CODE_LANGUAGE_ASM����


#ifndef __HI_REG_SYS_H__
#define __HI_REG_SYS_H__

/************************************����ַ******************************************/
//����ַ
#define HI_SYSCTRL_REG_BASE                 (0x101e0000)


/************************************�Ĵ���ƫ�Ƶ�ַ******************************************/
//�Ĵ���ƫ�Ƶ�ַ,������ʽ����
#define SC_CTRL             (0x00)
#define SC_SYS_RES          (0x04)
#define SC_PLLCTRL          (0x14)
#define SC_PERI_CLKEN       (0x20)
#define SC_PERI_SRST        (0x2c)
#define SC_PERI_CLKSEL      (0x28)
#define SC_WDG_RST_CTRL     (0x3c)
#define SC_SLEEP_CTRL       (0x48)
#define SC_VERSION_ID       (0x50)
#define SC_HW_ID            (0x54)
#define SC_PHY_WRAP_CLK_EN  (0x74)
#define ENET_SLEEP_CTRL     (0x90)


/************************************�Ĵ������Ե�ַ******************************************/
//HI+GROUP NAME+REG NAME+REG
#define HI_SYSCTRL_CLKEN_REG                (HI_SYSCTRL_REG_BASE+0x20)
#define HI_SYSCTRL_PHY_WRAP_CLK_EN_REG      (HI_SYSCTRL_REG_BASE+0x74) 


/************************************ʱ��ʹ�ܼĴ���1����(HI_SYSCTRL_CLKEN_REG)******************************************/
#define HI_SYSCTRL_CLKEN_REG_BIT_SHIFT_SHA       (1<<0) 
#define HI_SYSCTRL_CLKEN_REG_BIT_SHIFT_RSA       (1<<1)
#define HI_SYSCTRL_CLKEN_REG_BIT_SHIFT_ETH       (1<<2)/*����ʱ��*/
#define HI_SYSCTRL_CLKEN_REG_BIT_SHIFT_MDDRC     (1<<3)
#define HI_SYSCTRL_CLKEN_REG_BIT_SHIFT_SFC       (1<<4)
#define HI_SYSCTRL_CLKEN_REG_BIT_SHIFT_CIPHER    (1<<5)
#define HI_SYSCTRL_CLKEN_REG_BIT_SHIFT_DT        (1<<6)
#define HI_SYSCTRL_CLKEN_REG_BIT_SHIFT_RNG       (1<<7)
#define HI_SYSCTRL_CLKEN_REG_BIT_SHIFT_DDR       (1<<8)
#define HI_SYSCTRL_CLKEN_REG_BIT_SHIFT_TIMER0    (1<<9)
#define HI_SYSCTRL_CLKEN_REG_BIT_SHIFT_TIMER1    (1<<10)
#define HI_SYSCTRL_CLKEN_REG_BIT_SHIFT_TIMER2    (1<<11)
#define HI_SYSCTRL_CLKEN_REG_BIT_SHIFT_TIMER3    (1<<12)
#define HI_SYSCTRL_CLKEN_REG_BIT_SHIFT_UART0     (1<<13)
#define HI_SYSCTRL_CLKEN_REG_BIT_SHIFT_UART1     (1<<14)
#define HI_SYSCTRL_CLKEN_REG_BIT_SHIFT_UART2     (1<<15)
#define HI_SYSCTRL_CLKEN_REG_BIT_SHIFT_UART3     (1<<16)
#define HI_SYSCTRL_CLKEN_REG_BIT_SHIFT_I2C       (1<<17)
#define HI_SYSCTRL_CLKEN_REG_BIT_SHIFT_GPIO      (1<<18)
#define HI_SYSCTRL_CLKEN_REG_BIT_SHIFT_ARM       (1<<19)
#define HI_SYSCTRL_CLKEN_REG_BIT_SHIFT_BUS       (1<<20)
#define HI_SYSCTRL_CLKEN_REG_BIT_SHIFT_STC       (1<<21)
#define HI_SYSCTRL_CLKEN_REG_BIT_SHIFT_APB       (1<<22)
#define HI_SYSCTRL_CLKEN_REG_BIT_SHIFT_MII       (1<<23)/*mii_rxʱ�Ӻ�mii_txʱ��*/
#define HI_SYSCTRL_CLKEN_REG_BIT_SHIFT_1M        (1<<24)
#define HI_SYSCTRL_CLKEN_REG_BIT_SHIFT_SSP       (1<<25)
#define HI_SYSCTRL_CLKEN_REG_BIT_SHIFT_LED       (1<<26)
#define HI_SYSCTRL_CLKEN_REG_BIT_SHIFT_TIMER4    (1<<27)
#define HI_SYSCTRL_CLKEN_REG_BIT_SHIFT_TIMER5    (1<<28)
#define HI_SYSCTRL_CLKEN_REG_BIT_SHIFT_EFUSE     (1<<29)
#define HI_SYSCTRL_CLKEN_REG_BIT_SHIFT_LVAIADC   (1<<30)/*̨��ʶ��ADC����ʱ��*/
#define HI_SYSCTRL_CLKEN_REG_BIT_SHIFT_ADC_PWM   (1<<31)

/************************************ʱ��ʹ�ܼĴ���2����(HI_SYSCTRL_PHY_WRAP_CLK_EN)******************************************/
#define HI_SYSCTRL_PHY_WRAP_CLK_EN_REG_BIT_SHIFT_LVAI           (1<<0)/*̨��ʶ���߼���ʱ��*/
#define HI_SYSCTRL_PHY_WRAP_CLK_EN_REG_BIT_SHIFT_PLCSC          (1<<1) 
#define HI_SYSCTRL_PHY_WRAP_CLK_EN_REG_BIT_SHIFT_PWM            (1<<2)  
#define HI_SYSCTRL_PHY_WRAP_CLK_EN_REG_BIT_SHIFT_LVAIADC_OUT    (1<<5)/*̨��ʶ��ADC���ʱ��*/ 
#define HI_SYSCTRL_PHY_WRAP_CLK_EN_REG_BIT_SHIFT_DA_DATA        (1<<6)
#define HI_SYSCTRL_PHY_WRAP_CLK_EN_REG_BIT_SHIFT_TIME_CNT       (1<<7)
#define HI_SYSCTRL_PHY_WRAP_CLK_EN_REG_BIT_SHIFT_I2C2           (1<<8)
#define HI_SYSCTRL_PHY_WRAP_CLK_EN_REG_BIT_SHIFT_SSP2           (1<<9)

/************************************������λ����Ĵ�������(SC_PERI_SRST)******************************************/
#define HI_SYSCTRL_SRST_REG_BIT_SHIFT_ETH       (1<<3)/*Ethernet��λ����*/

#endif


