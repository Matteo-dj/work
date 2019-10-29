/******************************************************************************

                  ��Ȩ���� (C), 2001-2011, ��Ϊ�������޹�˾

 ******************************************************************************
  �� �� ��   : led_driver.h
  �� �� ��   : ����
  ��    ��   : sunhaiyang/00238140
  ��������   : 2017��5��4��
  ����޸�   :
  ��������   : led�����ӿ�
  �����б�   :
  �޸���ʷ   :
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
