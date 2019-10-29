/******************************************************************************

                  ��Ȩ���� (C), 2001-2011, ��Ϊ�������޹�˾

 ******************************************************************************
  �� �� ��   : power_failure_driver.c
  �� �� ��   : ����
  ��    ��   : j00415756
  ��������   : 2019��1��19��
  ����޸�   :
  ��������   : power_failure����
  �����б�   :
  �޸���ʷ   :

******************************************************************************/
#include "hi_driver.h"
#include "hi_ioexp.h"
#include "hi_sal.h"
#include "power_failure_driver.h"


hi_void HI_BOARD_PowerFailureGpioInit(hi_void)
{
    power_failure_sta_gpio_init();
    pf_power_down_gpio_init();
}

hi_void power_failure_sta_gpio_init(hi_void)
{
    HI_MDM_IO_SetMux(HI_HW_GPIO28_BACKUP, HI_GPIO28_BACKUP_SEL_GPIO28);
    HI_MDM_IO_SetDir(HI_GPIO_IDX_28, HI_GPIO_DIRECTION_OUT);

    HI_MDM_IO_SetMux(HI_HW_GPIO26_BACKUP, HI_GPIO26_BACKUP_SEL_GPIO26);
    HI_MDM_IO_SetDir(HI_GPIO_IDX_26, HI_GPIO_DIRECTION_IN);
}

// ��ѹ
hi_void pf_sta_elevate_power(hi_void)
{
    HI_MDM_IO_SetOutputVal(HI_GPIO_IDX_28, HI_GPIO_VALUE1);
}

// ��ѹ
hi_void pf_sta_down_power(hi_void)
{
    HI_MDM_IO_SetOutputVal(HI_GPIO_IDX_28, HI_GPIO_VALUE0);
}

// ��ȡ��ѹ����ܽŵ�ֵ
hi_u32 pf_get_plug_module_gpio_value(HI_GPIO_VALUE_E *gpio_value)
{
    return HI_MDM_IO_GetInputVal(HI_GPIO_IDX_26,gpio_value);
}

hi_bool pf_is_plug_module(hi_void)
{
    HI_GPIO_VALUE_E gpio26_value;

    pf_get_plug_module_gpio_value(&gpio26_value);
    if(gpio26_value == (HI_GPIO_VALUE_E)HI_EDGE_RISE_LEVEL_HIGH){
        return HI_TRUE;
    }

    return HI_FALSE;
}

hi_void pf_power_down_gpio_init(hi_void)
{
    HI_MDM_IO_SetMux(HI_HW_GPIO29_BACKUP,HI_GPIO29_BACKUP_SEL_GPIO29);
    HI_MDM_IO_SetDir(HI_GPIO_IDX_29,HI_GPIO_DIRECTION_IN);
}


