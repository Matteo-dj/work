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
#ifndef LED_DRIVER_H
#define LED_DRIVER_H

#include "hi_types.h"
#include "hi_mdm_timer.h"
#include "hi_mdm.h"

#ifdef __cplusplus
extern "C"{
#endif

#define LED_TX_RX_ON_TIME            (20)      // �շ�����ʱ��
#define LED_FLASH_LIGHT_OFF          (1)       // ��˸����
#define LED_FLASH_LIGHT_ON           (2)       // ��˸����
#define LED_NDM_OVER_TIME            (20)    //NDM LED�Ƶ���ʱ�䣬��λms

#define EXP_GPIO_TF_SCR_POWER_ON   IO_EXP_IO_NUM_15


typedef struct
{
    HI_U8 led_id;            // ָʾ��id
    HI_U8 led_gpio;          // ָʾ��ʵ��gpio
    HI_U8 led_status;        // ָʾ����״̬��0:��1:��
    HI_BOOL is_locked;       // ָʾ���Ƿ�ռ��,HI_TRUE:ռ�ã�HI_FALSE:û��ռ��

    HI_BOOL is_exp_io;       // �Ƿ�Ϊ��չIO
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
 �� �� ��  : led_set_tx_err_led_init
 ��������  :���������ָʾ�Ƴ�ʼ��
 �������  : HI_VOID
 �������  : ��
 �� �� ֵ  : HI_VOID
 ���������õĺ���  :
 ���ñ������ĺ���  :
 ���ʵ�ȫ�ֱ���  :
 �޸ĵ�ȫ�ֱ���  :
 �޸ĵ�BBP�Ĵ����б�  :

 �޸���ʷ      :
  1.��    ��   : 2015��11��23��
    ��    ��   : sunhaiyang/00238140
    �޸�����   : �����ɺ���

*****************************************************************************/
HI_EXTERN HI_VOID led_set_tx_err_led_init(HI_VOID);
/*****************************************************************************
 �� �� ��  : led_set_exp_io_value
 ��������  :������չIO�����ӿ�
 �������  : HI_U8 gpio,HI_BOOL onoff
 �������  : ��
 �� �� ֵ  : HI_VOID
 ���������õĺ���  :
 ���ñ������ĺ���  :
 ���ʵ�ȫ�ֱ���  :
 �޸ĵ�ȫ�ֱ���  :
 �޸ĵ�BBP�Ĵ����б�  :
*****************************************************************************/
HI_EXTERN HI_VOID led_set_exp_io_value(HI_U8 gpio,HI_BOOL onoff);
/*****************************************************************************
 �� �� ��  : led_set_three_phase_led_init
 ��������  :��ʼ���������λ���з���ָʾ��Ϊ������͵�ƽ
 �������  : HI_VOID
 �������  : ��
 �� �� ֵ  : HI_VOID
 ���������õĺ���  :
 ���ñ������ĺ���  :
 ���ʵ�ȫ�ֱ���  :
 �޸ĵ�ȫ�ֱ���  :
 �޸ĵ�BBP�Ĵ����б�  :
*****************************************************************************/
HI_EXTERN HI_VOID led_set_three_phase_led_init(HI_VOID);
/*****************************************************************************
 �� �� ��  : led_set_three_phase_led_value
 ��������  :���÷��������λָʾ�Ƶ���������
 �������  : HI_BOOL onoff
 �������  : ��
 �� �� ֵ  : HI_VOID
 ���������õĺ���  :
 ���ñ������ĺ���  :
 ���ʵ�ȫ�ֱ���  :
 �޸ĵ�ȫ�ֱ���  :
 �޸ĵ�BBP�Ĵ����б�  :
*****************************************************************************/
HI_EXTERN HI_VOID led_set_three_phase_led_value(HI_BOOL onoff);
/*****************************************************************************
 �� �� ��  : led_get_phase_mode
 ��������  : ��ȡGPIO2,3,4��ģʽ
 �������  : HI_U8 *io_fun
 �������  : ��
 �� �� ֵ  : HI_VOID
 ���������õĺ���  :
 ���ñ������ĺ���  :
 ���ʵ�ȫ�ֱ���  :
 �޸ĵ�ȫ�ֱ���  :
 �޸ĵ�BBP�Ĵ����б�  :
*****************************************************************************/
HI_EXTERN HI_VOID led_get_phase_mode(HI_U8 *io_fun);
/*****************************************************************************
 �� �� ��  : led_set_phase_led_mode
 ��������  : �ָ�GPIO2,3,4��ģʽ
 �������  : HI_U8 *io_fun
 �������  : ��
 �� �� ֵ  : HI_VOID
 ���������õĺ���  :
 ���ñ������ĺ���  :
 ���ʵ�ȫ�ֱ���  :
 �޸ĵ�ȫ�ֱ���  :
 �޸ĵ�BBP�Ĵ����б�  :
*****************************************************************************/
HI_EXTERN HI_VOID led_set_phase_led_mode(HI_U8 *io_fun);
#ifdef PRODUCT_CFG_PRODUCT_TYPE_NDM
/*****************************************************************************
 �� �� ��  : led_tf_scr_power_en_init
 ��������  : ������ϵ���ƹܽų�ʼ��Ϊ������ߵ�ƽ
 �������  : HI_VOID
 �������  : ��
 �� �� ֵ  : HI_U32
 ���������õĺ���  :
 ���ñ������ĺ���  :
 ���ʵ�ȫ�ֱ���  :
 �޸ĵ�ȫ�ֱ���  :
 �޸ĵ�BBP�Ĵ����б�  :

 �޸���ʷ      :
  1.��    ��   : 2015��11��23��
    ��    ��   : sunhaiyang/00238140
    �޸�����   : �����ɺ���

*****************************************************************************/
HI_U32 led_tf_scr_power_en_init(HI_VOID);
#endif

#ifdef __cplusplus
}
#endif

#endif
