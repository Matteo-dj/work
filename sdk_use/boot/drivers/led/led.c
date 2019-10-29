#include <led.h>
#include <reg.h>
#include <product.h>

#define LED_GLITTER_CLOCK_BASE    HI_XTAL_CLOCK_BASE

#define LED_ON  (0x1)
#define LED_OFF  (0x0)
#define LED_GLITTER  (0x3)

/*config high or low effect*/
void led_init(HI_LED_MODE_CFG_TYPE_E u32LEDMode)
{

    /*LED low or high effect*/
    u32 dirvalue = 0;
    HI_REG_READ((HI_LED_REG_BASE + LED_PH_CTRL) , dirvalue);
    dirvalue &= ~((1<<10)-1);//5 led off
    if (HI_LED_HIGH == u32LEDMode)
    {
        dirvalue &= (~(u32)(0xff << 16));//led1-5,switch1-3,total:8bit
    }
    else
    {
/*
                    【屏蔽人】z00193780
                    【审核人】b00208046
                    【代码分析】二次开发接口，该分支需保留，需保留。
                    【功能影响】不影响正常业务，屏蔽告警
*/
        dirvalue |= (0xff << 16);
    }
    HI_REG_WRITE((HI_LED_REG_BASE + LED_PH_CTRL) , dirvalue);		
    HI_REG_WRITE((HI_LED_REG_BASE + LED_START) , 0x01);
}

static u32 led_control(u32 state, HI_LED_SWITCH_NUM_TYPE_E u32LEDNum)
{
    u32 dirvalue = 0;
    
    /*config light num*/
    HI_REG_READ((HI_LED_REG_BASE + LED_PH_CTRL) , dirvalue);
    dirvalue &= ( ~(u32)(0x3 << ((unsigned int)u32LEDNum << 1 ) ) );		
    dirvalue |= (state << ((u32)u32LEDNum << 1));//exp:led1 = bit[1:0]
    HI_REG_WRITE((HI_LED_REG_BASE + LED_PH_CTRL) , dirvalue);

    /*enable light*/	
    dirvalue = 0x01;		
    HI_REG_WRITE((HI_LED_REG_BASE + LED_START) , dirvalue);
    
    return 0;
}

/* function led off */
u32 led_off(HI_LED_SWITCH_NUM_TYPE_E u32LEDNum)
{
    return led_control(LED_OFF, u32LEDNum);
}

/* function led on*/
u32 led_on(HI_LED_SWITCH_NUM_TYPE_E u32LEDNum)
{
    return led_control(LED_ON, u32LEDNum);
}

/* function led cycle*/
u32 led_cycle(HI_LED_SWITCH_NUM_TYPE_E u32LEDNum, u32 u32On_t, u32 u32Off_t)
{
    u32 lig_reg = 0;	

    /*check parameters*/
    if ((u32On_t < 1)
        ||(u32On_t > ((((u32)0x1<<31) -1)/(1000*LED_GLITTER_CLOCK_BASE)))
        ||(u32Off_t < 1)
        ||(u32Off_t > ((((u32)0x1<<31) -1)/(1000*LED_GLITTER_CLOCK_BASE))))
    {
        return (u32)(-1);
    }
    u32On_t *= (u32)(1000*LED_GLITTER_CLOCK_BASE);
    u32Off_t *= (u32)(1000*LED_GLITTER_CLOCK_BASE);

    lig_reg = u32LEDNum * 0x0c;
    HI_REG_WRITE((HI_LED_REG_BASE + lig_reg + 0x04), u32On_t);
    HI_REG_WRITE((HI_LED_REG_BASE + lig_reg + 0x08), u32Off_t);

    return led_control(LED_GLITTER, u32LEDNum);

}
