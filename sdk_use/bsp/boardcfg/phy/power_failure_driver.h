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
#ifndef POWER_FAILURE_DRIVER_H
#define POWER_FAILURE_DRIVER_H

#include "hi_types.h"
#include "hi_mdm_timer.h"
#include "hi_mdm.h"

#ifdef __cplusplus
extern "C"{
#endif

hi_void power_failure_sta_gpio_init(hi_void);
hi_void pf_sta_elevate_power(hi_void);
hi_void pf_sta_down_power(hi_void);
hi_u32 pf_get_plug_module_gpio_value(HI_GPIO_VALUE_E *gpio_value);
hi_bool pf_is_plug_module(hi_void);
hi_void pf_power_down_gpio_init(hi_void);

#ifdef __cplusplus
}
#endif

#endif
