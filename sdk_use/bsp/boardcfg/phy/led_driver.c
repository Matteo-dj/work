/******************************************************************************

                  版权所有 (C), 2001-2011, 华为技术有限公司

 ******************************************************************************
  文 件 名   : led_driver.c
  版 本 号   : 初稿
  作    者   : sunhaiyang/00238140
  生成日期   : 2017年5月4日
  最近修改   :
  功能描述   : led驱动
  函数列表   :
  修改历史   :

******************************************************************************/
#include "led_driver.h"
#include "hi_driver.h"
#include "hi_ioexp.h"
#include "hi_mdm_led.h"
#include "hi_sal.h"

led_flash_ctrl_st g_led_flash_ctrl = {{0, 0, 0, 0, {0}}, 0, {{0}}};
led_object_st g_led_cb[HI_LED_MAX_ID] = {{0, 0, 0, 0, 0, {0}, {{0}}},};

HI_VOID led_tf_transmitter_led_init(HI_VOID)
{
    #ifdef PRODUCT_CFG_PRODUCT_TYPE_NDM
    led_tf_scr_power_en_init();
    #endif
    led_set_three_phase_led_init();
    led_set_tx_err_led_init();
}

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
*****************************************************************************/
HI_VOID led_set_tx_err_led_init(HI_VOID)
{
#if (defined(PRODUCT_CFG_PRODUCT_TYPE_NDM) && defined(PRODUCT_CFG_SUPPORT_IO_EXP))
    HI_IO_EXP_SetDir(NDM_LED_TRANSMITTER_ALARM, HI_IO_EXP_DIR_OUT);

    HI_IO_EXP_SetOutPutVal(NDM_LED_TRANSMITTER_ALARM,HI_IO_EXP_OUT_VAL_LOW);
#endif
}

HI_VOID led_set_exp_io_value(HI_U8 gpio,HI_BOOL onoff)
{
#if (defined(PRODUCT_CFG_PRODUCT_TYPE_NDM) && defined(PRODUCT_CFG_SUPPORT_IO_EXP))
    HI_IO_EXP_OUT_VAL_E gpio_val = HI_IO_EXP_OUT_VAL_LOW;

    gpio_val = ((HI_TRUE == onoff) ?  HI_IO_EXP_OUT_VAL_HIGHT : HI_IO_EXP_OUT_VAL_LOW);

    HI_IO_EXP_SetOutPutVal((IO_EXP_IO_NUM_E)gpio,gpio_val);
#endif
}

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
HI_VOID led_set_three_phase_led_init(HI_VOID)
{
#if (defined(PRODUCT_CFG_PRODUCT_TYPE_NDM) && defined(PRODUCT_CFG_SUPPORT_IO_EXP))
    HI_IO_EXP_SetDir(NDM_LED_TRANSMITTER_A, HI_IO_EXP_DIR_OUT);
    HI_IO_EXP_SetDir(NDM_LED_TRANSMITTER_B, HI_IO_EXP_DIR_OUT);
    HI_IO_EXP_SetDir(NDM_LED_TRANSMITTER_C, HI_IO_EXP_DIR_OUT);
    HI_IO_EXP_SetOutPutVal(NDM_LED_TRANSMITTER_A,HI_IO_EXP_OUT_VAL_LOW);
    HI_IO_EXP_SetOutPutVal(NDM_LED_TRANSMITTER_B,HI_IO_EXP_OUT_VAL_LOW);
    HI_IO_EXP_SetOutPutVal(NDM_LED_TRANSMITTER_C,HI_IO_EXP_OUT_VAL_LOW);
#endif
}

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
HI_VOID led_set_three_phase_led_value(HI_BOOL onoff)
{
    if(HI_FALSE == onoff)
    {
        //只穿入了HI_FALSE的值，讲该分支屏蔽
        HI_MDM_LightOff(HI_LED_ID6);
        HI_MDM_LightOff(HI_LED_ID7);
        HI_MDM_LightOff(HI_LED_ID8);
    }
}

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
HI_VOID led_get_phase_mode(HI_U8 *io_fun)
{
    // 获取GPIO2复用功能
    (HI_VOID)HI_MDM_IO_GetMux(HI_HW_DN_RX_LED_IO2, &io_fun[0]);
    // 获取GPIO3复用功能
    (HI_VOID)HI_MDM_IO_GetMux(HI_HW_DN_TX_LED_IO3, &io_fun[1]);
    // 获取GPIO4复用功能
    (HI_VOID)HI_MDM_IO_GetMux(HI_HW_STATUS_LED_IO4, &io_fun[2]);
}

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
HI_VOID led_set_phase_led_mode(HI_U8 *io_fun)
{
    // GPIO2,配置为LED模式
    HI_MDM_IO_SetMux(HI_HW_DN_RX_LED_IO2, io_fun[0]);

    // GPIO3,配置为LED模式
    HI_MDM_IO_SetMux(HI_HW_DN_TX_LED_IO3, io_fun[1]);

    // GPIO4,配置为LED模式
    HI_MDM_IO_SetMux(HI_HW_STATUS_LED_IO4, io_fun[2]);
}

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
HI_U32 led_tf_scr_power_en_init(HI_VOID)
{
    HI_U32 ret = HI_ERR_FAILURE;

    ret = HI_IO_EXP_SetOutPutVal(EXP_GPIO_TF_SCR_POWER_ON, HI_IO_EXP_OUT_VAL_HIGHT);
    if(ret != HI_ERR_SUCCESS)
    {
        return ret;
    }

    return HI_IO_EXP_SetDir(EXP_GPIO_TF_SCR_POWER_ON, HI_IO_EXP_DIR_OUT);
}
#endif

HI_VOID led_driver_init(HI_VOID)
{
    led_object_st *led_cb = led_get_cb();

    led_cb[HI_LED_ID0].led_id = HI_LED_ID0;
    led_cb[HI_LED_ID0].led_gpio = HI_LED_IDX_0;

    led_cb[HI_LED_ID1].led_id = HI_LED_ID1;
    led_cb[HI_LED_ID1].led_gpio = HI_LED_IDX_1;

    led_cb[HI_LED_ID2].led_id = HI_LED_ID2;
    led_cb[HI_LED_ID2].led_gpio = HI_LED_IDX_2;

    led_cb[HI_LED_ID3].led_id = HI_LED_ID3;
    led_cb[HI_LED_ID3].led_gpio = HI_LED_IDX_3;

    led_cb[HI_LED_ID4].led_id = HI_LED_ID4;
    led_cb[HI_LED_ID4].led_gpio = HI_LED_IDX_4;

    #if defined(PRODUCT_CFG_PRODUCT_TYPE_NDM)
    led_cb[HI_LED_ID5].is_exp_io = HI_TRUE;
    led_cb[HI_LED_ID5].led_id = HI_LED_ID5;
    led_cb[HI_LED_ID5].led_gpio = NDM_LED_TRANSMITTER_ALARM;

    led_cb[HI_LED_ID6].is_exp_io = HI_TRUE;
    led_cb[HI_LED_ID6].led_id = HI_LED_ID6;
    led_cb[HI_LED_ID6].led_gpio = NDM_LED_TRANSMITTER_A;

    led_cb[HI_LED_ID7].is_exp_io = HI_TRUE;
    led_cb[HI_LED_ID7].led_id = HI_LED_ID7;
    led_cb[HI_LED_ID7].led_gpio = NDM_LED_TRANSMITTER_B;

    led_cb[HI_LED_ID8].is_exp_io = HI_TRUE;
    led_cb[HI_LED_ID8].led_id = HI_LED_ID8;
    led_cb[HI_LED_ID8].led_gpio = NDM_LED_TRANSMITTER_C;
    #endif
}

HI_VOID led_stop_all_led_timer(HI_VOID)
{
    led_object_st *led_cb = led_get_cb();

    HI_MDM_TIMER_Stop(&led_cb[HI_LED_ID3].timer);
    HI_MDM_TIMER_Stop(&led_cb[HI_LED_ID4].timer);
}

led_object_st *led_get_cb(HI_VOID)
{
    return g_led_cb;
}

led_flash_ctrl_st *led_get_flash_ctrl(HI_VOID)
{
    return &g_led_flash_ctrl;
}

HI_BOOL led_check_led_id_is_valid(HI_U8 LedId)
{
    #if defined(PRODUCT_CFG_PRODUCT_TYPE_CCO)
    if(LedId > HI_LED_ID4)
    {
        return HI_FALSE;
    }
    #endif

    #if defined(PRODUCT_CFG_PRODUCT_TYPE_STA)
    if((LedId > HI_LED_ID4) || (LedId < HI_LED_ID3))
    {
        return HI_FALSE;
    }
    #endif

    #if defined(PRODUCT_CFG_PRODUCT_TYPE_NDM)
    if((LedId == HI_LED_ID2) || (LedId > HI_LED_ID8))
    {
        return HI_FALSE;
    }
    #endif

    return HI_TRUE;
}

HI_U32 HI_MDM_LockLed(HI_U8 LedId)
{
    led_object_st *led_cb = led_get_cb();

    if(HI_FALSE == led_check_led_id_is_valid(LedId))
    {
        return HI_ERR_FAILURE;
    }

    led_cb[LedId].is_locked = HI_TRUE;

    return HI_ERR_SUCCESS;
}

HI_U32 HI_MDM_UnLockLed(HI_U8 LedId)
{
    led_object_st *led_cb = led_get_cb();

    if(HI_FALSE == led_check_led_id_is_valid(LedId))
    {
        return HI_ERR_FAILURE;
    }

    led_cb[LedId].is_locked = HI_FALSE;

    return HI_ERR_SUCCESS;
}

HI_U32 HI_MDM_GetLedIsLock(HI_U8 LedId,HI_BOOL *IsLock)
{
    led_object_st *led_cb = led_get_cb();

    if(HI_FALSE == led_check_led_id_is_valid(LedId) || HI_NULL == IsLock)
    {
        return HI_ERR_INVALID_PARAMETER;
    }

    *IsLock = led_cb[LedId].is_locked;

    return HI_ERR_SUCCESS;
}

HI_VOID led_light_led_onoff(led_object_st *led_cb,HI_BOOL onoff)
{
    if(HI_TRUE == led_cb->is_exp_io)
    {
        led_set_exp_io_value(led_cb->led_gpio,onoff);
    }
    else
    {
        if(HI_TRUE == onoff)
        {
            HI_MDM_LED_On((HI_LED_IDX_E)(led_cb->led_gpio));
        }
        else
        {
            HI_MDM_LED_Off((HI_LED_IDX_E)(led_cb->led_gpio));
        }
    }
}

HI_U32 HI_MDM_LightOn(HI_U8 LedId)
{
    led_object_st *led_cb = led_get_cb();

    if(HI_FALSE == led_check_led_id_is_valid(LedId))
    {
        return HI_ERR_FAILURE;
    }

    led_light_led_onoff(&led_cb[LedId],HI_TRUE);

    led_cb[LedId].led_status = HI_TRUE;

    return HI_ERR_SUCCESS;
}

HI_U32 HI_MDM_LightOff(HI_U8 LedId)
{
    led_object_st *led_cb = led_get_cb();

    if(HI_FALSE == led_check_led_id_is_valid(LedId))
    {
        return HI_ERR_FAILURE;
    }

    led_light_led_onoff(&led_cb[LedId],HI_FALSE);

    led_cb[LedId].led_status = HI_FALSE;

    return HI_ERR_SUCCESS;
}

HI_U32 HI_MDM_StartLedFlash(HI_MDM_LED_FLASH_PARA_S *led_flash_para)
{
    HI_U32 ret = HI_ERR_FAILURE;
    HI_U8 led_index = 0;
    HI_U8 led_num = 0;
    HI_U8 led_id[LED_MAX_FLASH_NUM] = {0};
    led_flash_ctrl_st *led_flash_ctrl = led_get_flash_ctrl();

    if(HI_NULL == led_flash_para)
    {
        return HI_ERR_FAILURE;
    }

    led_num = led_flash_para->led_num;

    if((led_num > LED_MAX_FLASH_NUM) || (0 == led_num) || (0 == led_flash_para->duration)
        || (0 == led_flash_para->light_on_cnt) || (0 == led_flash_para->light_off_cnt)
        || (led_flash_para->light_off_cnt >= led_flash_para->light_on_cnt))
    {
        return HI_ERR_FAILURE;
    }

    (hi_void)memcpy_s(led_id, sizeof(led_id), led_flash_para->led_id,led_num);

    while(led_index < led_num)
    {
        if(HI_FALSE == led_check_led_id_is_valid(led_id[led_index]))
        {
            return HI_ERR_FAILURE;
        }

        led_index++;
    }

    led_flash_ctrl->led_flash_para.duration = led_flash_para->duration;
    led_flash_ctrl->led_flash_para.light_off_cnt = led_flash_para->light_off_cnt;
    led_flash_ctrl->led_flash_para.light_on_cnt = led_flash_para->light_on_cnt;
    led_flash_ctrl->led_flash_para.led_num = led_num;
    led_flash_ctrl->flash_cnt = 0;

    ret = HI_MDM_TIMER_Start(&led_flash_ctrl->timer,led_flash_call_back,
          led_flash_ctrl->led_flash_para.duration,HI_TIMER_TYPE_PERIOD,0);
    if(HI_ERR_SUCCESS != ret)
    {
        return HI_ERR_FAILURE;
    }

    led_index = 0;

    while(led_index < led_num)
    {
        led_flash_ctrl->led_flash_para.led_id[led_index] = led_id[led_index];
        HI_MDM_LockLed(led_id[led_index]);
        HI_MDM_LightOn(led_id[led_index]);
        led_index++;
    }

    return HI_ERR_SUCCESS;
}


HI_VOID HI_TIMER_DEFINE(led_flash_call_back)
{
    HI_U8 light_mode = 0;
    HI_U8 led_index = 0;
    led_flash_ctrl_st *led_flash_ctrl = led_get_flash_ctrl();

    led_flash_ctrl->flash_cnt++;

    if(led_flash_ctrl->led_flash_para.light_off_cnt == led_flash_ctrl->flash_cnt)
    {
        light_mode = LED_FLASH_LIGHT_OFF;
    }
    else if(led_flash_ctrl->led_flash_para.light_on_cnt == led_flash_ctrl->flash_cnt)
    {
        led_flash_ctrl->flash_cnt = 0;
        light_mode = LED_FLASH_LIGHT_ON;
    }

    while(led_index < led_flash_ctrl->led_flash_para.led_num)
    {
        if(LED_FLASH_LIGHT_ON == light_mode)
        {
            HI_MDM_LightOn(led_flash_ctrl->led_flash_para.led_id[led_index]);
        }
        else if(LED_FLASH_LIGHT_OFF == light_mode)
        {
            HI_MDM_LightOff(led_flash_ctrl->led_flash_para.led_id[led_index]);
        }

        led_index++;
    }
}

HI_U32 HI_MDM_StopLedFlash(HI_VOID)
{
    HI_U8 led_index = 0;
    led_flash_ctrl_st *led_flash_ctrl = led_get_flash_ctrl();

    while(led_index < led_flash_ctrl->led_flash_para.led_num)
    {
        HI_MDM_LightOff(led_flash_ctrl->led_flash_para.led_id[led_index]);
        HI_MDM_UnLockLed(led_flash_ctrl->led_flash_para.led_id[led_index]);
        led_index++;
    }

    HI_MDM_TIMER_Stop(&led_flash_ctrl->timer);
    (hi_void)memset_s(&led_flash_ctrl->led_flash_para,sizeof(led_flash_para_st), 0,sizeof(led_flash_para_st));
    led_flash_ctrl->flash_cnt = 0;

    return HI_ERR_SUCCESS;
}

HI_U32 led_get_tx_led_id(HI_U8 *led_id)
{
    *led_id = HI_LED_ID4;

    #if defined(PRODUCT_CFG_PRODUCT_TYPE_STA)
    if(HI_ND_TYPE_CLTII == HI_DMS_LoadDevType())
    {
        *led_id = HI_LED_ID3;
    }
    #endif

    if(HI_FALSE == led_check_led_id_is_valid(*led_id))
    {
        return HI_ERR_FAILURE;
    }

    return HI_ERR_SUCCESS;
}

HI_U32 led_get_rx_led_id(HI_U8 *led_id)
{
    *led_id = HI_LED_ID3;

    if(HI_FALSE == led_check_led_id_is_valid(*led_id))
    {
        return HI_ERR_FAILURE;
    }

    return HI_ERR_SUCCESS;
}
// 点亮发送报文指示灯
HI_U32 HI_MDM_LightOnTx(HI_U32 LightDuration)
{
    HI_U8 led_id = 0;
    HI_U32 ret_value = HI_ERR_FAILURE;

    ret_value = led_get_tx_led_id(&led_id);
    if(HI_ERR_SUCCESS != ret_value)
    {
        return HI_ERR_FAILURE;
    }

    ret_value = led_set_light_on(led_id,LightDuration);

    return ret_value;
}

// 点亮接收报文指示灯
HI_U32 HI_MDM_LightOnRx(HI_U32 LightDuration)
{
    HI_U8 led_id = 0;
    HI_U32 ret_value = HI_ERR_FAILURE;

    ret_value = led_get_rx_led_id(&led_id);
    if(HI_ERR_SUCCESS != ret_value)
    {
        return HI_ERR_FAILURE;
    }

    ret_value = led_set_light_on(led_id,LightDuration);

    return ret_value;
}

HI_VOID led_light_phase_a_led(HI_U32 LightDuration)
{
    led_object_st *led_cb = led_get_cb();

    if(HI_TRUE == led_cb[HI_LED_ID2].led_status)
    {
        return;
    }

    led_set_light_on(HI_LED_ID2,LightDuration);
}

HI_VOID led_light_phase_b_led(HI_U32 LightDuration)
{
    led_object_st *led_cb = led_get_cb();

    if(HI_TRUE == led_cb[HI_LED_ID0].led_status)
    {
        return;
    }

    led_set_light_on(HI_LED_ID0,LightDuration);
}

HI_VOID led_light_phase_c_led(HI_U32 LightDuration)
{
    led_object_st *led_cb = led_get_cb();

    if(HI_TRUE == led_cb[HI_LED_ID1].led_status)
    {
        return;
    }

    led_set_light_on(HI_LED_ID1,LightDuration);
}

// 点亮发送相位指示灯
HI_U32 HI_MDM_LightPhaseOnTx(HI_U8 phase,HI_U32 LightDuration)
{
    HI_U32 ret_value = HI_ERR_FAILURE;

    if(0 == LightDuration || phase >= INVALID_PHASE)
    {
        return ret_value;
    }

    #if defined(PRODUCT_CFG_PRODUCT_TYPE_CCO)
    switch(phase)
    {
        case PHASE_A:
            led_light_phase_a_led(LightDuration);
            break;
        case PHASE_B:
            led_light_phase_b_led(LightDuration);
            break;
        case PHASE_C:
            led_light_phase_c_led(LightDuration);
            break;
        case PHASE_ALL:
            led_light_phase_a_led(LightDuration);
            led_light_phase_b_led(LightDuration);
            led_light_phase_c_led(LightDuration);
            break;
        default:
            break;
    }
    ret_value = HI_ERR_SUCCESS;
    #endif

    return ret_value;
}

HI_U32 led_set_light_on(HI_U8 led_id,HI_U32 lightduration)
{
    HI_U32 ret_value = HI_ERR_FAILURE;
    led_object_st *led_cb = led_get_cb();

    if(HI_TRUE != led_check_led_id_is_valid(led_id))
    {
        return HI_ERR_FAILURE;
    }

    if(0 == lightduration)
    {
        return HI_ERR_FAILURE;
    }

    if(HI_TRUE == led_cb[led_id].is_locked)
    {
        return HI_ERR_FAILURE;
    }

    if(HI_TRUE == led_cb[led_id].led_status)
    {
        return HI_ERR_FAILURE;
    }

    ret_value = led_start_led_timer(led_id,lightduration);
    if(HI_ERR_SUCCESS != ret_value)
    {
        return HI_ERR_FAILURE;
    }

    HI_MDM_LightOn(led_id);

    return ret_value;
}

HI_U32 led_start_led_timer(HI_U8 led_id,HI_U32 lightduration)
{
    HI_U32 ret_value = HI_ERR_FAILURE;

    switch(led_id)
    {
        case HI_LED_ID0:
            ret_value = led_start_led_id0_timer(lightduration);
            break;
        case HI_LED_ID1:
            ret_value = led_start_led_id1_timer(lightduration);
            break;
        case HI_LED_ID2:
            ret_value = led_start_led_id2_timer(lightduration);
            break;
        case HI_LED_ID3:
            ret_value = led_start_led_id3_timer(lightduration);
            break;
        case HI_LED_ID4:
            ret_value = led_start_led_id4_timer(lightduration);
            break;
        case HI_LED_ID5:
            ret_value = led_start_led_id5_timer(lightduration);
            break;
        case HI_LED_ID6:
            ret_value = led_start_led_id6_timer(lightduration);
            break;
        case HI_LED_ID7:
            ret_value = led_start_led_id7_timer(lightduration);
            break;
        case HI_LED_ID8:
            ret_value = led_start_led_id8_timer(lightduration);
            break;
        default:
            break;
    }

    return ret_value;
}

HI_U32 led_start_led_id0_timer(HI_U32 lightduration)
{
    HI_U32 ret_value = HI_ERR_FAILURE;
    led_object_st *led_cb = led_get_cb();

    ret_value = HI_MDM_TIMER_Start(&led_cb[HI_LED_ID0].timer,led_id0_light_on_call_back,
                lightduration,HI_TIMER_TYPE_ONCE, 0);
    if(HI_ERR_SUCCESS != ret_value)
    {
        return HI_ERR_FAILURE;
    }

    return ret_value;
}

HI_VOID HI_TIMER_DEFINE(led_id0_light_on_call_back)
{
    led_object_st *led_cb = led_get_cb();

    HI_MDM_TIMER_Stop(&led_cb[HI_LED_ID0].timer);
    HI_MDM_LightOff(HI_LED_ID0);
}

HI_U32 led_start_led_id1_timer(HI_U32 lightduration)
{
    HI_U32 ret_value = HI_ERR_FAILURE;
    led_object_st *led_cb = led_get_cb();

    ret_value = HI_MDM_TIMER_Start(&led_cb[HI_LED_ID1].timer,led_id1_light_on_call_back,
                lightduration,HI_TIMER_TYPE_ONCE, 0);
    if(HI_ERR_SUCCESS != ret_value)
    {
        return HI_ERR_FAILURE;
    }

    return ret_value;
}

HI_VOID HI_TIMER_DEFINE(led_id1_light_on_call_back)
{
    led_object_st *led_cb = led_get_cb();

    HI_MDM_TIMER_Stop(&led_cb[HI_LED_ID1].timer);
    HI_MDM_LightOff(HI_LED_ID1);
}

HI_U32 led_start_led_id2_timer(HI_U32 lightduration)
{
    HI_U32 ret_value = HI_ERR_FAILURE;
    led_object_st *led_cb = led_get_cb();

    ret_value = HI_MDM_TIMER_Start(&led_cb[HI_LED_ID2].timer,led_id2_light_on_call_back,
                lightduration,HI_TIMER_TYPE_ONCE, 0);
    if(HI_ERR_SUCCESS != ret_value)
    {
        return HI_ERR_FAILURE;
    }

    return ret_value;
}

HI_VOID HI_TIMER_DEFINE(led_id2_light_on_call_back)
{
    led_object_st *led_cb = led_get_cb();

    HI_MDM_TIMER_Stop(&led_cb[HI_LED_ID2].timer);
    HI_MDM_LightOff(HI_LED_ID2);
}

HI_U32 led_start_led_id3_timer(HI_U32 lightduration)
{
    HI_U32 ret_value = HI_ERR_FAILURE;
    led_object_st *led_cb = led_get_cb();

    ret_value = HI_MDM_TIMER_Start(&led_cb[HI_LED_ID3].timer,led_id3_light_on_call_back,
                lightduration,HI_TIMER_TYPE_ONCE, 0);
    if(HI_ERR_SUCCESS != ret_value)
    {
        return HI_ERR_FAILURE;
    }

    return ret_value;
}

HI_VOID HI_TIMER_DEFINE(led_id3_light_on_call_back)
{
    led_object_st *led_cb = led_get_cb();

    HI_MDM_TIMER_Stop(&led_cb[HI_LED_ID3].timer);
    HI_MDM_LightOff(HI_LED_ID3);
}


HI_U32 led_start_led_id4_timer(HI_U32 lightduration)
{
    HI_U32 ret_value = HI_ERR_FAILURE;
    led_object_st *led_cb = led_get_cb();

    ret_value = HI_MDM_TIMER_Start(&led_cb[HI_LED_ID4].timer,led_id4_light_on_call_back,
                lightduration,HI_TIMER_TYPE_ONCE, 0);
    if(HI_ERR_SUCCESS != ret_value)
    {
        return HI_ERR_FAILURE;
    }

    return ret_value;
}

HI_VOID HI_TIMER_DEFINE(led_id4_light_on_call_back)
{
    led_object_st *led_cb = led_get_cb();

    HI_MDM_TIMER_Stop(&led_cb[HI_LED_ID4].timer);
    HI_MDM_LightOff(HI_LED_ID4);
}

HI_U32 led_start_led_id5_timer(HI_U32 lightduration)
{
    HI_U32 ret_value = HI_ERR_FAILURE;
    led_object_st *led_cb = led_get_cb();

    ret_value = HI_MDM_TIMER_Start(&led_cb[HI_LED_ID5].timer,led_id5_light_on_call_back,
                lightduration,HI_TIMER_TYPE_ONCE, 0);
    if(HI_ERR_SUCCESS != ret_value)
    {
        return HI_ERR_FAILURE;
    }

    return ret_value;
}

HI_VOID HI_TIMER_DEFINE(led_id5_light_on_call_back)
{
    led_object_st *led_cb = led_get_cb();

    HI_MDM_TIMER_Stop(&led_cb[HI_LED_ID5].timer);
    HI_MDM_LightOff(HI_LED_ID5);
}


HI_U32 led_start_led_id6_timer(HI_U32 lightduration)
{
    HI_U32 ret_value = HI_ERR_FAILURE;
    led_object_st *led_cb = led_get_cb();

    ret_value = HI_MDM_TIMER_Start(&led_cb[HI_LED_ID6].timer,led_id6_light_on_call_back,
                lightduration,HI_TIMER_TYPE_ONCE, 0);
    if(HI_ERR_SUCCESS != ret_value)
    {
        return HI_ERR_FAILURE;
    }

    return ret_value;
}

HI_VOID HI_TIMER_DEFINE(led_id6_light_on_call_back)
{
    led_object_st *led_cb = led_get_cb();

    HI_MDM_TIMER_Stop(&led_cb[HI_LED_ID6].timer);
    HI_MDM_LightOff(HI_LED_ID6);
}


HI_U32 led_start_led_id7_timer(HI_U32 lightduration)
{
    HI_U32 ret_value = HI_ERR_FAILURE;
    led_object_st *led_cb = led_get_cb();

    ret_value = HI_MDM_TIMER_Start(&led_cb[HI_LED_ID7].timer,led_id7_light_on_call_back,
                lightduration,HI_TIMER_TYPE_ONCE, 0);
    if(HI_ERR_SUCCESS != ret_value)
    {
        return HI_ERR_FAILURE;
    }

    return ret_value;
}

HI_VOID HI_TIMER_DEFINE(led_id7_light_on_call_back)
{
    led_object_st *led_cb = led_get_cb();

    HI_MDM_TIMER_Stop(&led_cb[HI_LED_ID7].timer);
    HI_MDM_LightOff(HI_LED_ID7);
}

HI_U32 led_start_led_id8_timer(HI_U32 lightduration)
{
    HI_U32 ret_value = HI_ERR_FAILURE;
    led_object_st *led_cb = led_get_cb();

    ret_value = HI_MDM_TIMER_Start(&led_cb[HI_LED_ID8].timer,led_id8_light_on_call_back,
                lightduration,HI_TIMER_TYPE_ONCE, 0);
    if(HI_ERR_SUCCESS != ret_value)
    {
        return HI_ERR_FAILURE;
    }

    return ret_value;
}

HI_VOID HI_TIMER_DEFINE(led_id8_light_on_call_back)
{
    led_object_st *led_cb = led_get_cb();

    HI_MDM_TIMER_Stop(&led_cb[HI_LED_ID8].timer);
    HI_MDM_LightOff(HI_LED_ID8);
}

