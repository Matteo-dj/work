/******************************************************************************

                  版权所有 (C), 2001-2011, 华为技术有限公司

 ******************************************************************************
  文 件 名   : led_driver.h
  版 本 号   : 初稿
  作    者   : sunhaiyang/00238140
  生成日期   : 2017年5月4日
  最近修改   :
  功能描述   : led驱动接口
  函数列表   :
  修改历史   :
******************************************************************************/
#ifndef LED_DRIVER_H
#define LED_DRIVER_H

#include "hi_types.h"
#include "hi_mdm_timer.h"
#include "hi_mdm.h"

#ifdef __cplusplus
extern "C"{
#endif

#define LED_TX_RX_ON_TIME            (20)      // 收发报文时长
#define LED_FLASH_LIGHT_OFF          (1)       // 闪烁点灭
#define LED_FLASH_LIGHT_ON           (2)       // 闪烁点亮
#define LED_NDM_OVER_TIME            (20)    //NDM LED灯点亮时间，单位ms

#define EXP_GPIO_TF_SCR_POWER_ON   IO_EXP_IO_NUM_15


typedef struct
{
    HI_U8 led_id;            // 指示灯id
    HI_U8 led_gpio;          // 指示灯实际gpio
    HI_U8 led_status;        // 指示灯亮状态，0:灭，1:亮
    HI_BOOL is_locked;       // 指示灯是否被占用,HI_TRUE:占用，HI_FALSE:没有占用

    HI_BOOL is_exp_io;       // 是否为扩展IO
    HI_U8 pad[3];

    HI_TIMER_HANDLE_S timer;
}led_object_st;

typedef struct
{
    HI_U8 light_on_cnt;
    HI_U8 light_off_cnt;
    HI_U16 duration;

    HI_U8 led_num;
    HI_U8 led_id[LED_MAX_FLASH_NUM];
}led_flash_para_st;

typedef struct
{
    led_flash_para_st led_flash_para;

    HI_U32 flash_cnt;

    HI_TIMER_HANDLE_S timer;
}led_flash_ctrl_st;

HI_VOID led_tf_transmitter_led_init(HI_VOID);
HI_VOID led_driver_init(HI_VOID);
led_object_st *led_get_cb(HI_VOID);
led_flash_ctrl_st *led_get_flash_ctrl(HI_VOID);
HI_BOOL led_check_led_id_is_valid(HI_U8 LedId);
HI_VOID led_light_led_onoff(led_object_st *led_cb,HI_BOOL onoff);
HI_U32 led_get_tx_led_id(HI_U8 *led_id);
HI_U32 led_get_rx_led_id(HI_U8 *led_id);
HI_VOID HI_TIMER_DEFINE(led_flash_call_back);
HI_VOID led_stop_all_led_timer(HI_VOID);
HI_VOID led_light_phase_a_led(HI_U32 LightDuration);
HI_VOID led_light_phase_b_led(HI_U32 LightDuration);
HI_VOID led_light_phase_c_led(HI_U32 LightDuration);
HI_U32 led_set_light_on(HI_U8 led_id,HI_U32 lightduration);
HI_U32 led_start_led_timer(HI_U8 led_id,HI_U32 lightduration);
HI_U32 led_start_led_id0_timer(HI_U32 lightduration);
HI_VOID HI_TIMER_DEFINE(led_id0_light_on_call_back);
HI_U32 led_start_led_id1_timer(HI_U32 lightduration);
HI_VOID HI_TIMER_DEFINE(led_id1_light_on_call_back);
HI_U32 led_start_led_id2_timer(HI_U32 lightduration);
HI_VOID HI_TIMER_DEFINE(led_id2_light_on_call_back);
HI_U32 led_start_led_id3_timer(HI_U32 lightduration);
HI_VOID HI_TIMER_DEFINE(led_id3_light_on_call_back);
HI_U32 led_start_led_id4_timer(HI_U32 lightduration);
HI_VOID HI_TIMER_DEFINE(led_id4_light_on_call_back);
HI_U32 led_start_led_id5_timer(HI_U32 lightduration);
HI_VOID HI_TIMER_DEFINE(led_id5_light_on_call_back);
HI_U32 led_start_led_id6_timer(HI_U32 lightduration);
HI_VOID HI_TIMER_DEFINE(led_id6_light_on_call_back);
HI_U32 led_start_led_id7_timer(HI_U32 lightduration);
HI_VOID HI_TIMER_DEFINE(led_id7_light_on_call_back);
HI_U32 led_start_led_id8_timer(HI_U32 lightduration);
HI_VOID HI_TIMER_DEFINE(led_id8_light_on_call_back);
/*****************************************************************************
 函 数 名  : led_set_tx_err_led_init
 功能描述  :发射机故障指示灯初始化
 输入参数  : HI_VOID
 输出参数  : 无
 返 回 值  : HI_VOID
 本函数调用的函数  :
 调用本函数的函数  :
 访问的全局变量  :
 修改的全局变量  :
 修改的BBP寄存器列表  :

 修改历史      :
  1.日    期   : 2015年11月23日
    作    者   : sunhaiyang/00238140
    修改内容   : 新生成函数

*****************************************************************************/
HI_EXTERN HI_VOID led_set_tx_err_led_init(HI_VOID);
/*****************************************************************************
 函 数 名  : led_set_exp_io_value
 功能描述  :设置扩展IO操作接口
 输入参数  : HI_U8 gpio,HI_BOOL onoff
 输出参数  : 无
 返 回 值  : HI_VOID
 本函数调用的函数  :
 调用本函数的函数  :
 访问的全局变量  :
 修改的全局变量  :
 修改的BBP寄存器列表  :
*****************************************************************************/
HI_EXTERN HI_VOID led_set_exp_io_value(HI_U8 gpio,HI_BOOL onoff);
/*****************************************************************************
 函 数 名  : led_set_three_phase_led_init
 功能描述  :初始化发射机相位序列发送指示灯为输出，低电平
 输入参数  : HI_VOID
 输出参数  : 无
 返 回 值  : HI_VOID
 本函数调用的函数  :
 调用本函数的函数  :
 访问的全局变量  :
 修改的全局变量  :
 修改的BBP寄存器列表  :
*****************************************************************************/
HI_EXTERN HI_VOID led_set_three_phase_led_init(HI_VOID);
/*****************************************************************************
 函 数 名  : led_set_three_phase_led_value
 功能描述  :配置发射机三相位指示灯点亮，点灭
 输入参数  : HI_BOOL onoff
 输出参数  : 无
 返 回 值  : HI_VOID
 本函数调用的函数  :
 调用本函数的函数  :
 访问的全局变量  :
 修改的全局变量  :
 修改的BBP寄存器列表  :
*****************************************************************************/
HI_EXTERN HI_VOID led_set_three_phase_led_value(HI_BOOL onoff);
/*****************************************************************************
 函 数 名  : led_get_phase_mode
 功能描述  : 获取GPIO2,3,4的模式
 输入参数  : HI_U8 *io_fun
 输出参数  : 无
 返 回 值  : HI_VOID
 本函数调用的函数  :
 调用本函数的函数  :
 访问的全局变量  :
 修改的全局变量  :
 修改的BBP寄存器列表  :
*****************************************************************************/
HI_EXTERN HI_VOID led_get_phase_mode(HI_U8 *io_fun);
/*****************************************************************************
 函 数 名  : led_set_phase_led_mode
 功能描述  : 恢复GPIO2,3,4的模式
 输入参数  : HI_U8 *io_fun
 输出参数  : 无
 返 回 值  : HI_VOID
 本函数调用的函数  :
 调用本函数的函数  :
 访问的全局变量  :
 修改的全局变量  :
 修改的BBP寄存器列表  :
*****************************************************************************/
HI_EXTERN HI_VOID led_set_phase_led_mode(HI_U8 *io_fun);
#ifdef PRODUCT_CFG_PRODUCT_TYPE_NDM
/*****************************************************************************
 函 数 名  : led_tf_scr_power_en_init
 功能描述  : 发射机上电控制管脚初始化为输出，高电平
 输入参数  : HI_VOID
 输出参数  : 无
 返 回 值  : HI_U32
 本函数调用的函数  :
 调用本函数的函数  :
 访问的全局变量  :
 修改的全局变量  :
 修改的BBP寄存器列表  :

 修改历史      :
  1.日    期   : 2015年11月23日
    作    者   : sunhaiyang/00238140
    修改内容   : 新生成函数

*****************************************************************************/
HI_U32 led_tf_scr_power_en_init(HI_VOID);
#endif

#ifdef __cplusplus
}
#endif

#endif
