#ifndef __LEDH_H__
#define __LEDH_H__

#include<types.h>

typedef enum hiLED_MODE_CFG_TYPE_E
{
    HI_LED_HIGH = 0,//上拉使用
    HI_LED_LOW //下拉使用
}HI_LED_MODE_CFG_TYPE_E;

typedef enum
{
    led_phase1 = 0,// boot mainbody
	led_phase2,// start load code 
	led_phase3,// main_loop
}LED_PHASE_E;

typedef enum hiLED_SWITCH_NUM_TYPE_E
{
	HI_LED0 = 0,
	HI_LED1,
	HI_LED2,
	HI_LED3,
	HI_LED4,
	HI_SWITCH0,
	HI_SWITCH1,
	HI_SWITCH2,
	HI_LED_SWITCH_NUM_TYPE_MAX
}HI_LED_SWITCH_NUM_TYPE_E;

u32 led_on(HI_LED_SWITCH_NUM_TYPE_E u32LEDNum);
u32 led_off(HI_LED_SWITCH_NUM_TYPE_E u32LEDNum);
u32 led_cycle(HI_LED_SWITCH_NUM_TYPE_E u32LEDNum, u32 u32On_t, u32 u32Off_t);
void led_init(HI_LED_MODE_CFG_TYPE_E u32LEDMode);
#endif//__LEDH_H__