/******************************************************************************

                  版权所有 (C), 2001-2016, 华为技术有限公司

 ******************************************************************************
  文 件 名   : drv_board.c
  版 本 号   : 初稿
  作    者   : liujian 00280874
  生成日期   : 2017年10月31日
  最近修改   :
  功能描述   : 台区识别相关GPIO接口
  函数列表   :
  修改历史   :
  1.日    期   : 2017年10月31日
    作    者   :  liujian 00280874
    修改内容   : 创建文件

******************************************************************************/
#include "tf_driver.h"
#include "hi_mdm.h"
#include "phase_driver.h"
#include "hi_ioexp.h"
#include "hi_sal.h"
#include "led_driver.h"
#include <hi_lcd.h>
#include <hi_mdm_tf.h>
/*****************************************************************************
 函 数 名  : HI_BOARD_TfGpioInit
 功能描述  : 台区识别相关GPIO初始化
 输入参数  : HI_VOID
 输出参数  : 无
 返 回 值  : HI_VOID
 本函数调用的函数  :
 调用本函数的函数  :
 访问的全局变量  :
 修改的全局变量  :
 修改的BBP寄存器列表  :
*****************************************************************************/
HI_VOID HI_BOARD_TfGpioInit(HI_VOID)
{

    #if defined(PRODUCT_CFG_PRODUCT_TYPE_NDM)
    //台区识别GPIO初始化
    HI_BOARD_ZeroCrossInGpioInit();
    #endif

    #if defined(PRODUCT_CFG_PRODUCT_TYPE_NDM)
    if(HI_TRUE == HI_DMS_IsTfHardWare())
    {
        led_tf_transmitter_led_init();
        HI_BOARD_SetTfSendEdgeModeUpGpioInit();
        HI_BOARD_SetTfSendEdgeModeDownGpioInit();
        HI_BOARD_SetTfTxOutDataGpioInit();
        HI_BOARD_SetAnCheckClearGpioInit();
        HI_BOARD_SetAnCheckGpioInit();
    }
    HI_BOARD_SetBeepGpioInit();
    #endif
}

/*****************************************************************************
 HI_BOARD_ZeroCrossInGpioInit
 功能描述  : 台区识别GPIO初始化
 输入参数  : HI_VOID
 输出参数  : 无
 返 回 值  : HI_VOID
 本函数调用的函数  :
 调用本函数的函数  :
 访问的全局变量  :
 修改的全局变量  :
 修改的BBP寄存器列表  :
*****************************************************************************/
HI_VOID HI_BOARD_ZeroCrossInGpioInit(HI_VOID)
{
    // 台区识别仪，STA端过流检测初始化
    //GPIO模式配置
    HI_MDM_IO_SetDir(ZERO_CROSS_IN_GPIO_PIN, HI_GPIO_DIRECTION_IN);
}

/*****************************************************************************
 函 数 名  : HI_BOARD_SetTfTxOutDataGpioInit
 功能描述  : GPIO20管脚初始化输出，低电平
 输入参数  : HI_VOID
 输出参数  : 无
 返 回 值  : HI_VOID
 本函数调用的函数  :
 调用本函数的函数  :
 访问的全局变量  :
 修改的全局变量  :
 修改的BBP寄存器列表  :
*****************************************************************************/
HI_VOID HI_BOARD_SetTfTxOutDataGpioInit(HI_VOID)
{
    // GPIO 20配置为GPIO模式
    HI_MDM_IO_SetMux(HI_HW_SPI_DO_IO20, (HI_U8)HI_GPIO20_SEL_GPIO20);
    // 初始化为输出
    HI_MDM_IO_SetDir(HI_GPIO_IDX_20, HI_GPIO_DIRECTION_OUT);
    //初始化为低电平
    HI_MDM_IO_SetOutputVal(HI_GPIO_IDX_20, HI_GPIO_VALUE0);
}

/*****************************************************************************
 函 数 名  : HI_BOARD_SetTfSendEdgeModeUpGpioInit
 功能描述  : 将GPIO9管脚初始化为低电平，输出
 输入参数  : 无
 输出参数  : 无
 返 回 值  : HI_VOID
 本函数调用的函数  :
 调用本函数的函数  :
 访问的全局变量  :
 修改的全局变量  :
 修改的BBP寄存器列表  :
*****************************************************************************/
HI_VOID HI_BOARD_SetTfSendEdgeModeUpGpioInit(HI_VOID)
{
    HI_MDM_IO_SetOutputVal(HI_GPIO_IDX_9, HI_GPIO_VALUE0);
    HI_MDM_IO_SetDir(HI_GPIO_IDX_9, HI_GPIO_DIRECTION_OUT);
}

/*****************************************************************************
 函 数 名  : HI_BOARD_SetTfSendEdgeModeDownGpioInit
 功能描述  :将GPIO10管脚初始化为低电平，输出
 输入参数  : 无
 输出参数  : 无
 返 回 值  : HI_VOID
 本函数调用的函数  :
 调用本函数的函数  :
 访问的全局变量  :
 修改的全局变量  :
 修改的BBP寄存器列表  :
*****************************************************************************/
HI_VOID HI_BOARD_SetTfSendEdgeModeDownGpioInit(HI_VOID)
{
    HI_MDM_IO_SetOutputVal(HI_GPIO_IDX_10, HI_GPIO_VALUE0);
    HI_MDM_IO_SetDir(HI_GPIO_IDX_10, HI_GPIO_DIRECTION_OUT);
}

/*****************************************************************************
 函 数 名  : HI_BOARD_SetSwitchLevel
 功能描述  : 设置台区识别档位
 输入参数  : HI_VOID
 输出参数  : 无
 返 回 值  : HI_VOID
 本函数调用的函数  :
 调用本函数的函数  :
 访问的全局变量  :
 修改的全局变量  :
 修改的BBP寄存器列表  :
*****************************************************************************/
HI_VOID HI_BOARD_SetSwitchLevel(HI_U8 level)
{
    if(TF_RCV_MODE_240K == level)
    {
        if(HI_ND_TYPE_DCM != HI_DMS_LoadDevType())
        {
            #ifdef PRODUCT_CFG_BOARD_TYPE_ASIC_V200
            if (HI_ND_TYPE_THREE_STA != HI_DMS_LoadDevType())
            {
                HI_MDM_IO_SetOutputVal(HI_GPIO_IDX_27,HI_GPIO_VALUE0);
            }
            #endif
        }
        else
        {
            HI_MDM_IO_SetOutputVal(HI_GPIO_IDX_24,HI_GPIO_VALUE0);
        }
    }
    else if(TF_RCV_MODE_1M == level)
    {
        if(HI_ND_TYPE_DCM != HI_DMS_LoadDevType())
        {
            #ifdef PRODUCT_CFG_BOARD_TYPE_ASIC_V200
            if (HI_ND_TYPE_THREE_STA != HI_DMS_LoadDevType())
            {
                HI_MDM_IO_SetOutputVal(HI_GPIO_IDX_27, HI_GPIO_VALUE1);
            }
            #endif
        }
        else
        {
            HI_MDM_IO_SetOutputVal(HI_GPIO_IDX_24, HI_GPIO_VALUE1);
        }
    }
}

//设置发送沿模式
HI_VOID HI_BOARD_SetSendEdgeMode(HI_U8 edge_mode)
{
    //切换台区识别序列的发送沿模式，硬件时序要求先将两个GPIO置低，再将对应管脚置高
    switch(edge_mode)
    {
        case TF_SEND_EDGE_MODE_OFF:
            //上升沿开关
            HI_MDM_IO_SetOutputVal(HI_GPIO_IDX_9, HI_GPIO_VALUE0);
            //下降沿开关
            HI_MDM_IO_SetOutputVal(HI_GPIO_IDX_10, HI_GPIO_VALUE0);
            break;

        case TF_SEND_EDGE_MODE_UP:
            HI_MDM_IO_SetOutputVal(HI_GPIO_IDX_10, HI_GPIO_VALUE0);
            HI_MDM_IO_SetOutputVal(HI_GPIO_IDX_9, HI_GPIO_VALUE1);
            break;

        case TF_SEND_EDGE_MODE_DOWN:
            HI_MDM_IO_SetOutputVal(HI_GPIO_IDX_9, HI_GPIO_VALUE0);
            HI_MDM_IO_SetOutputVal(HI_GPIO_IDX_10, HI_GPIO_VALUE1);
            break;

        default:
            HI_MDM_IO_SetOutputVal(HI_GPIO_IDX_9, HI_GPIO_VALUE0);
            HI_MDM_IO_SetOutputVal(HI_GPIO_IDX_10, HI_GPIO_VALUE0);
            break;
    }
}

/*****************************************************************************
 函 数 名  : HI_BOARD_SetPhaseGpioMode
 功能描述  : 获取GPIO2,3,4的模式
 输入参数  : HI_VOID
 输出参数  : 无
 返 回 值  : HI_VOID
 本函数调用的函数  :
 调用本函数的函数  :
 访问的全局变量  :
 修改的全局变量  :
 修改的BBP寄存器列表  :
*****************************************************************************/
HI_VOID HI_BOARD_SetPhaseGpioMode(HI_VOID)
{
    // GPIO2,配置为GPIO模式
    HI_MDM_IO_SetMux(HI_HW_DN_RX_LED_IO2, (HI_U8)HI_GPIO2_SEL_GPIO2);

    // GPIO3,配置为GPIO模式
    HI_MDM_IO_SetMux(HI_HW_DN_TX_LED_IO3, (HI_U8)HI_GPIO3_SEL_GPIO3);

    // GPIO4,配置为GPIO模式
    HI_MDM_IO_SetMux(HI_HW_STATUS_LED_IO4, (HI_U8)HI_GPIO4_SEL_GPIO4);
}

/*****************************************************************************
 函 数 名  : HI_BOARD_SetPhaseGpioDirecInit
 功能描述  : 检测CCO单板硬件功能，设置相位的GPIO为输出
 输入参数  : HI_IN HI_U8 phase_mode
 输出参数  : 无
 返 回 值  : HI_VOID
 本函数调用的函数  :
 调用本函数的函数  :
 访问的全局变量  :
 修改的全局变量  :
 修改的BBP寄存器列表  :
*****************************************************************************/
HI_VOID HI_BOARD_SetPhaseGpioDirecInit(HI_VOID)
{
    HI_U32 index = 0;
    HI_U32 gpio_pin[PHASE_CNT] = {HI_GPIO_IDX_2,HI_GPIO_IDX_3,HI_GPIO_IDX_4};

    for(index = 0; index < PHASE_CNT; index++)
    {
        HI_MDM_IO_SetDir((HI_GPIO_IDX_E)(gpio_pin[index]), HI_GPIO_DIRECTION_OUT);
    }
}
/*****************************************************************************
 函 数 名  : HI_BOARD_SetPhaseGpioValue
 功能描述  : 检测CCO单板硬件功能，设置三相位开关，打开或者关闭
 输入参数  : HI_IN HI_U8 phase_mode
 输出参数  : 无
 返 回 值  : HI_VOID
 本函数调用的函数  :
 调用本函数的函数  :
 访问的全局变量  :
 修改的全局变量  :
 修改的BBP寄存器列表  :
*****************************************************************************/
HI_VOID HI_BOARD_SetPhaseGpioValue(HI_IN HI_U8 phase_mode)
{
    HI_U32 index = 0;
    HI_GPIO_VALUE_E gpio_val;
    HI_U32 gpio_pin[PHASE_CNT] = {HI_GPIO_IDX_2,HI_GPIO_IDX_3,HI_GPIO_IDX_4};

    gpio_val = ((PHASE_MODE_ON == phase_mode) ?  HI_GPIO_VALUE1 : HI_GPIO_VALUE0);
    for(index = 0; index < PHASE_CNT; index++)
    {
        // 设置GPIO的属性
        HI_MDM_IO_SetOutputVal((HI_GPIO_IDX_E)(gpio_pin[index]), gpio_val);
    }
}
/*****************************************************************************
 函 数 名  : HI_BOARD_SetSinglePhaseGpioValue
 功能描述  : 设置单个相位GPIO管脚的高低电平
 输入参数  : HI_IN HI_U32 gpio_pin,HI_IN HI_U8 phase_mode
 输出参数  : 无
 返 回 值  : HI_VOID
 本函数调用的函数  :
 调用本函数的函数  :
 访问的全局变量  :
 修改的全局变量  :
 修改的BBP寄存器列表  :
*****************************************************************************/
HI_VOID HI_BOARD_SetSinglePhaseGpioValue(HI_IN HI_GPIO_IDX_E gpio_pin,HI_IN HI_U8 phase_mode)
{
    HI_GPIO_VALUE_E gpio_val;

    gpio_val = ((PHASE_MODE_ON == phase_mode) ?  HI_GPIO_VALUE1 : HI_GPIO_VALUE0);

    HI_MDM_IO_SetOutputVal(gpio_pin, gpio_val);
}

/*****************************************************************************
 函 数 名  : HI_BOARD_SetBeepValue
 功能描述  :配置接收机蜂鸣器响停
 输入参数  : HI_BOOL onoff
 输出参数  : 无
 返 回 值  : HI_U32
 本函数调用的函数  :
 调用本函数的函数  :
 访问的全局变量  :
 修改的全局变量  :
 修改的BBP寄存器列表  :
*****************************************************************************/
HI_U32 HI_BOARD_SetBeepValue(HI_BOOL onoff)
{
#if (defined(PRODUCT_CFG_PRODUCT_TYPE_NDM) && defined(PRODUCT_CFG_SUPPORT_IO_EXP))
    HI_IO_EXP_OUT_VAL_E gpio_val = HI_IO_EXP_OUT_VAL_LOW;

    gpio_val = ((HI_TRUE == onoff) ?  HI_IO_EXP_OUT_VAL_LOW: HI_IO_EXP_OUT_VAL_HIGHT);

    return HI_IO_EXP_SetOutPutVal(NDM_IO_BEEP_ENABLE,gpio_val);
#else
    HI_UNREF_PARAM(onoff);
    return HI_ERR_NOT_SUPPORT;
#endif
}

/*****************************************************************************
 函 数 名  : HI_BOARD_SetBeepGpioInit
 功能描述  :初始化蜂鸣器为输出，高电平，不响
 输入参数  : HI_VOID
 输出参数  : 无
 返 回 值  : HI_U32
 本函数调用的函数  :
 调用本函数的函数  :
 访问的全局变量  :
 修改的全局变量  :
 修改的BBP寄存器列表  :
*****************************************************************************/
HI_U32 HI_BOARD_SetBeepGpioInit(HI_VOID)
{
#if (defined(PRODUCT_CFG_PRODUCT_TYPE_NDM) && defined(PRODUCT_CFG_SUPPORT_IO_EXP))
    HI_U32 ret = HI_ERR_FAILURE;
    ret = HI_IO_EXP_SetDir(NDM_IO_BEEP_ENABLE, HI_IO_EXP_DIR_OUT);
    if(ret != HI_ERR_SUCCESS)
    {
        return ret;
    }

    return HI_IO_EXP_SetOutPutVal(NDM_IO_BEEP_ENABLE,HI_IO_EXP_OUT_VAL_HIGHT);
#else
    return HI_ERR_NOT_SUPPORT;
#endif
}

/*****************************************************************************
 函 数 名  : HI_BOARD_SetNdmZeroDtcRxInit
 功能描述  :初始化接收机接收管脚为输出，低电平
 输入参数  : HI_VOID
 输出参数  : 无
 返 回 值  : HI_U32
 本函数调用的函数  :
 调用本函数的函数  :
 访问的全局变量  :
 修改的全局变量  :
 修改的BBP寄存器列表  :
*****************************************************************************/
HI_U32 HI_BOARD_SetNdmZeroDtcRxInit(HI_VOID)
{
#if (defined(PRODUCT_CFG_PRODUCT_TYPE_NDM) && defined(PRODUCT_CFG_SUPPORT_IO_EXP))
    HI_U32 ret = HI_ERR_FAILURE;

    ret = HI_IO_EXP_SetDir(IO_EXP_IO_NUM_13, HI_IO_EXP_DIR_OUT);
    if(HI_ERR_SUCCESS != ret)
    {
        return ret;
    }

    return HI_IO_EXP_SetOutPutVal(IO_EXP_IO_NUM_13,HI_IO_EXP_OUT_VAL_LOW);
#else
    return HI_ERR_NOT_SUPPORT;
#endif
}

/*****************************************************************************
 函 数 名  : HI_BOARD_SetNdmZeroDtcRxValue
 功能描述  :配置接收机接收管脚高低电平
 输入参数  : HI_BOOL gpio_value
 输出参数  : 无
 返 回 值  : HI_U32
 本函数调用的函数  :
 调用本函数的函数  :
 访问的全局变量  :
 修改的全局变量  :
 修改的BBP寄存器列表  :
*****************************************************************************/
HI_U32 HI_BOARD_SetNdmZeroDtcRxValue(HI_BOOL gpio_value)
{
#if (defined(PRODUCT_CFG_PRODUCT_TYPE_NDM) && defined(PRODUCT_CFG_SUPPORT_IO_EXP))
    HI_IO_EXP_OUT_VAL_E gpio_val = HI_IO_EXP_OUT_VAL_LOW;

    gpio_val = ((HI_TRUE == gpio_value) ?  HI_IO_EXP_OUT_VAL_HIGHT: HI_IO_EXP_OUT_VAL_LOW);

    return HI_IO_EXP_SetOutPutVal(IO_EXP_IO_NUM_13,gpio_val);
#else
    HI_UNREF_PARAM(gpio_value);
    return HI_ERR_NOT_SUPPORT;
#endif
}


/*****************************************************************************
 函 数 名  : HI_BOARD_SetAnCheckClearGpioInit
 功能描述  :上电检测是否过压，读过压标识前先通过该引脚清零标识
 输入参数  : HI_VOID
 输出参数  : 无
 返 回 值  : HI_VOID
 本函数调用的函数  :
 调用本函数的函数  :
 访问的全局变量  :
 修改的全局变量  :
 修改的BBP寄存器列表  :
*****************************************************************************/
HI_VOID HI_BOARD_SetAnCheckClearGpioInit(HI_VOID)
{
#if (defined(PRODUCT_CFG_PRODUCT_TYPE_NDM) && defined(PRODUCT_CFG_SUPPORT_IO_EXP))
    HI_IO_EXP_SetDir(IO_EXP_IO_NUM_06, HI_IO_EXP_DIR_OUT);
    HI_IO_EXP_SetOutPutVal(IO_EXP_IO_NUM_06,HI_IO_EXP_OUT_VAL_HIGHT);
#endif
}
/*****************************************************************************
 函 数 名  : HI_BOARD_SetAnCheckClearGpioValue
 功能描述  :上电检测是否过压，读过压标识前先通过该引脚清零标识
 输入参数  : HI_BOOL gpio_value
 输出参数  : 无
 返 回 值  : HI_VOID
 本函数调用的函数  :
 调用本函数的函数  :
 访问的全局变量  :
 修改的全局变量  :
 修改的BBP寄存器列表  :
*****************************************************************************/
HI_VOID HI_BOARD_SetAnCheckClearGpioValue(HI_BOOL gpio_value)
{
#if (defined(PRODUCT_CFG_PRODUCT_TYPE_NDM) && defined(PRODUCT_CFG_SUPPORT_IO_EXP))
     HI_IO_EXP_OUT_VAL_E gpio_val = HI_IO_EXP_OUT_VAL_LOW;

    gpio_val = ((HI_TRUE == gpio_value) ?  HI_IO_EXP_OUT_VAL_HIGHT : HI_IO_EXP_OUT_VAL_LOW);
    HI_IO_EXP_SetOutPutVal(IO_EXP_IO_NUM_06,gpio_val);
#else
    HI_UNREF_PARAM(gpio_value);
#endif
}
/*****************************************************************************
 函 数 名  : HI_BOARD_SetAnCheckGpioInit
 功能描述  :上电检测是否过压，读取过压信号的管脚，初始化为输入
 输入参数  : HI_VOID
 输出参数  : 无
 返 回 值  : HI_VOID
 本函数调用的函数  :
 调用本函数的函数  :
 访问的全局变量  :
 修改的全局变量  :
 修改的BBP寄存器列表  :
*****************************************************************************/
HI_VOID HI_BOARD_SetAnCheckGpioInit(HI_VOID)
{
#if (defined(PRODUCT_CFG_PRODUCT_TYPE_NDM) && defined(PRODUCT_CFG_SUPPORT_IO_EXP))
    HI_IO_EXP_SetDir(IO_EXP_IO_NUM_07, HI_IO_EXP_DIR_IN);
#endif
}
/*****************************************************************************
 函 数 名  : HI_BOARD_GetAnCheckValue
 功能描述  :上电检测是否过压，读取过压信号的管脚的电平
 输入参数  : HI_VOID
 输出参数  : 无
 返 回 值  : HI_U8
 本函数调用的函数  :
 调用本函数的函数  :
 访问的全局变量  :
 修改的全局变量  :
 修改的BBP寄存器列表  :
*****************************************************************************/
HI_U8 HI_BOARD_GetAnCheckValue(HI_VOID)
{
#if (defined(PRODUCT_CFG_PRODUCT_TYPE_NDM) && defined(PRODUCT_CFG_SUPPORT_IO_EXP))
    HI_IO_EXP_OUT_VAL_E val = HI_IO_EXP_OUT_VAL_LOW;

    HI_IO_EXP_GetInputVal(IO_EXP_IO_NUM_07,&val);

    return (HI_U8)val;
#else
    return 0;
#endif
}

/*****************************************************************************
 函 数 名  : HI_BOARD_SetOutSeqData
 功能描述  : 发送台区识别序列的对应比特
 输入参数  : HI_U8 send_bit
 输出参数  : 无
 返 回 值  : HI_U32
 本函数调用的函数  :
 调用本函数的函数  :
 访问的全局变量  :
 修改的全局变量  :
 修改的BBP寄存器列表  :
*****************************************************************************/
HI_U32 HI_BOARD_SetOutSeqData(HI_U8 send_bit)
{
    return HI_MDM_IO_SetOutputVal(TF_TX_OUT_DATA_PIN, (HI_GPIO_VALUE_E)send_bit);
}


/*****************************************************************************
 函 数 名  : HI_BOARD_SetThreeMeterStaGpioMode
 功能描述  : 设置三相表STA模块控制三相过零管脚为GPIO模式
 输入参数  : HI_VOID
 输出参数  : 无
 返 回 值  : HI_VOID
 本函数调用的函数  :
 调用本函数的函数  :
 访问的全局变量  :
 修改的全局变量  :
 修改的BBP寄存器列表  :
*****************************************************************************/
HI_VOID HI_BOARD_SetThreeMeterStaGpioMode(HI_VOID)
{
    // GPIO4,配置为GPIO模式
    HI_MDM_IO_SetMux(HI_HW_STATUS_LED_IO4, (HI_U8)HI_GPIO4_SEL_GPIO4);
}

/*****************************************************************************
 函 数 名  : HI_BOARD_SetThreeMeterStaGpioDirAndValueInit
 功能描述  : 设置三相表STA模块控制方向和高低电平初始化
 输入参数  : HI_VOID
 输出参数  : 无
 返 回 值  : HI_VOID
 本函数调用的函数  :
 调用本函数的函数  :
 访问的全局变量  :
 修改的全局变量  :
 修改的BBP寄存器列表  :
*****************************************************************************/
HI_VOID HI_BOARD_SetThreeMeterStaGpioDirAndValueInit(HI_VOID)
{
    HI_MDM_IO_SetDir(HI_GPIO_IDX_4, HI_GPIO_DIRECTION_OUT);
    HI_MDM_IO_SetOutputVal(HI_GPIO_IDX_4, HI_GPIO_VALUE0);
}

/*****************************************************************************
 函 数 名  : HI_BOARD_SetPhaseSwitchCtrlMode
 功能描述  : 设置CCO三相过零切换控制初始化
 输入参数  : HI_VOID
 输出参数  : 无
 返 回 值  : HI_VOID
 本函数调用的函数  :
 调用本函数的函数  :
 访问的全局变量  :
 修改的全局变量  :
 修改的BBP寄存器列表  :
*****************************************************************************/
HI_VOID HI_BOARD_SetPhaseSwitchCtrlMode(HI_VOID)
{
    #ifdef PRODUCT_CFG_BOARD_TYPE_ASIC_V200
    HI_MDM_IO_SetMux(HI_HW_GPIO26_BACKUP, (HI_U8)HI_GPIO26_BACKUP_SEL_CURRENT_PHASE_I00);
    HI_MDM_IO_SetMux(HI_HW_UART2_TXD_IO12, (HI_U8)HI_GPIO12_SEL_CURRENT_PHASE_IO1);
    HI_MDM_IO_SetMux(HI_HW_UART2_RXD_IO11, (HI_U8)HI_GPIO11_SEL_CURRENT_PHASE_IO2);
    #endif
}

#if defined(PRODUCT_CFG_PRODUCT_TYPE_CCO)
/*****************************************************************************
 函 数 名  : HI_BOARD_SetCcoZeroCrossEnable
 功能描述  : 设置CCO过零开关
 输入参数  : HI_U8 power_edge_type : 工频周期边沿，POWER_EDGE_X
             HI_U8 phase : 相位
 输出参数  : 无
 返 回 值  : HI_VOID
 本函数调用的函数  :
 调用本函数的函数  : 通过软件控制GPIO来操作CCO的相位过零开关时，调用本函数
 访问的全局变量  :
 修改的全局变量  :
 修改的BBP寄存器列表  :
*****************************************************************************/
HI_VOID HI_BOARD_SetCcoZeroCrossEnable(HI_U8 power_edge_type, HI_U8 phase)
{
    HI_GPIO_VALUE_E gpio26_out_value = HI_GPIO_VALUE0;
    HI_GPIO_VALUE_E gpio12_out_value = HI_GPIO_VALUE0;
    HI_GPIO_VALUE_E gpio11_out_value = HI_GPIO_VALUE0;

    if(POWER_EDGE_RISE == power_edge_type)
    {
        if(PHASE_A == phase)
        {
            gpio26_out_value = HI_GPIO_VALUE1;
        }
        else if(PHASE_B == phase)
        {
            gpio12_out_value = HI_GPIO_VALUE1;
        }
        else if(PHASE_C == phase)
        {
            gpio11_out_value = HI_GPIO_VALUE1;
        }
        else
        {}
    }
    else if(POWER_EDGE_FALL == power_edge_type)
    {
        if(PHASE_A == phase)
        {
            gpio26_out_value = HI_GPIO_VALUE0;
            gpio12_out_value = HI_GPIO_VALUE1;
            gpio11_out_value = HI_GPIO_VALUE1;
        }
        else if(PHASE_B == phase)
        {
            gpio26_out_value = HI_GPIO_VALUE1;
            gpio12_out_value = HI_GPIO_VALUE1;
            gpio11_out_value = HI_GPIO_VALUE1;
        }
        else if(PHASE_C == phase)
        {
            gpio26_out_value = HI_GPIO_VALUE1;
            gpio12_out_value = HI_GPIO_VALUE0;
            gpio11_out_value = HI_GPIO_VALUE1;
        }
        else
        {}
    }
    else
    {}

    HI_MDM_IO_SetOutputVal(HI_GPIO_IDX_26, gpio26_out_value);
    HI_MDM_IO_SetOutputVal(HI_GPIO_IDX_12, gpio12_out_value);
    HI_MDM_IO_SetOutputVal(HI_GPIO_IDX_11, gpio11_out_value);
}
#endif

#if defined(PRODUCT_CFG_PRODUCT_TYPE_NDM)
/*****************************************************************************
 函 数 名  : HI_BOARD_TF_DataDisPlay
 功能描述  : 抄控宝显示扰动结果到LCD界面
 输入参数  : HI_VOID
 输出参数  : 无
 返 回 值  : HI_VOID
 本函数调用的函数  :
 调用本函数的函数  :
 访问的全局变量  :
 修改的全局变量  :
 修改的BBP寄存器列表  :
*****************************************************************************/
HI_U32 HI_BOARD_TF_DataDisPlay(HI_IN HI_U16 wobble_scope)
{
    HI_U32 ret = HI_ERR_FAILURE;
    HI_LCD_DISPLAY_INFO_S lcd_cfg = {{0}, 0};
    HI_U8 index = 0;
    HI_U8 valid_index = HI_LCD_POSITION_MAX;
    HI_U8 data[HI_LCD_POSITION_MAX] = {0};

    // 扰动值>= 1000us,则显示"OFL"表示溢出
    if(wobble_scope >= TF_NDM_MAX_REAL_NOISE)
    {
        lcd_cfg.data[HI_LCD_POSITION_1] = 'O';
        lcd_cfg.data[HI_LCD_POSITION_2] = 'F';
        lcd_cfg.data[HI_LCD_POSITION_3] = 'L';
    }
    else
    {
        for(index = 0; index < HI_LCD_POSITION_MAX; index++)
        {
            data[HI_LCD_POSITION_MAX - 1- index] = (HI_U8)(wobble_scope % 10);
            wobble_scope = wobble_scope / 10;
        }

        for(index = 0; index < HI_LCD_POSITION_MAX; index++)
        {
            if(0 != data[index])
            {
                valid_index = index;
                break;
            }
        }

        for(index = valid_index; index < HI_LCD_POSITION_MAX; index++)
        {
            lcd_cfg.data[index] = (HI_CHAR) (data[index] + '0');
        }

        if(valid_index == HI_LCD_POSITION_MAX)
        {
            lcd_cfg.data[HI_LCD_POSITION_3] = (HI_CHAR)(0 + '0');
        }
    }

    lcd_cfg.blink = HI_FALSE;
    lcd_cfg.pri = HI_LCD_DISPLAY_PRI_2;
    lcd_cfg.tsec = HI_LCD_DISPLAY_FOREVER;
    ret = HI_DRV_LCD_DisplayPrint(HI_LCD_FUNC_ID_TF, &lcd_cfg);

    return ret;
}

/*****************************************************************************
 函 数 名  : HI_BOARD_TF_ModeDisPlay
 功能描述  : 设置发射机，抄控宝模式
 输入参数  : HI_VOID
 输出参数  : 无
 返 回 值  : HI_VOID
 本函数调用的函数  :
 调用本函数的函数  :
 访问的全局变量  :
 修改的全局变量  :
 修改的BBP寄存器列表  :
*****************************************************************************/
HI_U32 HI_BOARD_TF_ModeDisPlay(HI_IN HI_U8 transformer_mode)
{
    HI_U32 ret = HI_ERR_FAILURE;
    HI_LCD_DISPLAY_INFO_S lcd_cfg = {{0}, 0};

    //配置数码管显示台区识别模式
    if(TF_IDENTIFY_SEND_MODE == transformer_mode)
    {
        lcd_cfg.data[HI_LCD_POSITION_1] = 'T';
        lcd_cfg.data[HI_LCD_POSITION_2] = HI_LCD_NOT_FLUSH;
        lcd_cfg.data[HI_LCD_POSITION_3] = HI_LCD_NOT_FLUSH;
    }
    else if(TF_IDENTIFY_RCV_MODE == transformer_mode)
    {
        lcd_cfg.data[HI_LCD_POSITION_1] = 'R';
        lcd_cfg.data[HI_LCD_POSITION_2] = '-';
        lcd_cfg.data[HI_LCD_POSITION_3] = '-';
    }
    else if(TF_IDENTIFY_NORMAL_MODE == transformer_mode)
    {
        //进入正常模式或者非台区模式
        HI_DRV_LCD_DisplayDisable(HI_LCD_FUNC_ID_TF);
        return HI_ERR_SUCCESS;
    }
    else if(TF_IDENTIFY_LCD_OFF_MODE == transformer_mode)
    {
        lcd_cfg.data[0] = HI_LCD_DISP_NONE;
        lcd_cfg.data[1] = HI_LCD_DISP_NONE;
        lcd_cfg.data[2] = HI_LCD_DISP_NONE;
    }
    else
    {
        //理论不会执行
        lcd_cfg.data[0] = 'E';
        lcd_cfg.data[1] = 'R';
        lcd_cfg.data[2] = 'R';
    }

    lcd_cfg.blink = HI_TRUE;
    lcd_cfg.pri = HI_LCD_DISPLAY_PRI_2;
    lcd_cfg.tsec = HI_LCD_DISPLAY_FOREVER;
    ret = HI_DRV_LCD_DisplayPrint(HI_LCD_FUNC_ID_TF, &lcd_cfg);

    return ret;
}


/*****************************************************************************
 函 数 名  : HI_BOARD_TF_TxNumDisPlay
 功能描述  : 设置发射台区编号
 输入参数  : HI_VOID
 输出参数  : 无
 返 回 值  : HI_VOID
 本函数调用的函数  :
 调用本函数的函数  :
 访问的全局变量  :
 修改的全局变量  :
 修改的BBP寄存器列表  :
*****************************************************************************/
HI_U32 HI_BOARD_TF_TxNumDisPlay(HI_IN HI_U8 transformer_index)
{
    HI_U32 ret = HI_ERR_FAILURE;
    HI_LCD_DISPLAY_INFO_S lcd_cfg = {{0}, 0};

    //配置数码管显示台区编号
    lcd_cfg.data[HI_LCD_POSITION_1] = HI_LCD_NOT_FLUSH;
    lcd_cfg.data[HI_LCD_POSITION_2] = (HI_CHAR)('1' + transformer_index);
    lcd_cfg.data[HI_LCD_POSITION_3] = HI_LCD_NOT_FLUSH;
    lcd_cfg.blink = HI_TRUE;
    lcd_cfg.pri = HI_LCD_DISPLAY_PRI_2;
    lcd_cfg.tsec = HI_LCD_DISPLAY_FOREVER;
    ret = HI_DRV_LCD_DisplayPrint(HI_LCD_FUNC_ID_TF, &lcd_cfg);

    return ret;
}

/*****************************************************************************
 函 数 名  : HI_BOARD_TF_TxPhaseDisPlay
 功能描述  : 设置发射序列对应的相位
 输入参数  : HI_VOID
 输出参数  : 无
 返 回 值  : HI_VOID
 本函数调用的函数  :
 调用本函数的函数  :
 访问的全局变量  :
 修改的全局变量  :
 修改的BBP寄存器列表  :
*****************************************************************************/
HI_U32 HI_BOARD_TF_TxPhaseDisPlay(HI_IN HI_U8 phase_index)
{
    HI_U32 ret = HI_ERR_FAILURE;
    HI_LCD_DISPLAY_INFO_S lcd_cfg = {{0}, 0};

    //配置数码管显示台区编号
    lcd_cfg.data[HI_LCD_POSITION_1] = HI_LCD_NOT_FLUSH;
    lcd_cfg.data[HI_LCD_POSITION_2] = HI_LCD_NOT_FLUSH;
    lcd_cfg.data[HI_LCD_POSITION_3] = (HI_CHAR)('A' + phase_index);
    lcd_cfg.blink = HI_TRUE;
    lcd_cfg.pri = HI_LCD_DISPLAY_PRI_2;
    lcd_cfg.tsec = HI_LCD_DISPLAY_FOREVER;
    ret = HI_DRV_LCD_DisplayPrint(HI_LCD_FUNC_ID_TF, &lcd_cfg);

    return ret;
}


/*****************************************************************************
 函 数 名  : HI_BOARD_TF_RxNumDisPlay
 功能描述  : 设置抄控宝识别的结果的台区编号显示
 输入参数  : HI_VOID
 输出参数  : 无
 返 回 值  : HI_VOID
 本函数调用的函数  :
 调用本函数的函数  :
 访问的全局变量  :
 修改的全局变量  :
 修改的BBP寄存器列表  :
*****************************************************************************/
HI_U32 HI_BOARD_TF_RxNumDisPlay(HI_IN HI_U8 transformer_index)
{
    HI_U32 ret = HI_ERR_FAILURE;
    HI_LCD_DISPLAY_INFO_S lcd_cfg = {{0}, 0};

    //配置数码管显示台区编号
    lcd_cfg.data[HI_LCD_POSITION_1] = (HI_CHAR)('1' + transformer_index);
    lcd_cfg.data[HI_LCD_POSITION_2] = HI_LCD_NOT_FLUSH;
    lcd_cfg.data[HI_LCD_POSITION_3] = HI_LCD_NOT_FLUSH;
    lcd_cfg.pri = HI_LCD_DISPLAY_PRI_2;
    lcd_cfg.tsec = HI_LCD_DISPLAY_FOREVER;
    ret = HI_DRV_LCD_DisplayPrint(HI_LCD_FUNC_ID_TF, &lcd_cfg);

    return ret;
}
/*****************************************************************************
 函 数 名  : HI_BOARD_TF_RxPhaseDisPlay
 功能描述  : 设置抄控宝识别的结果的相位显示
 输入参数  : HI_VOID
 输出参数  : 无
 返 回 值  : HI_VOID
 本函数调用的函数  :
 调用本函数的函数  :
 访问的全局变量  :
 修改的全局变量  :
 修改的BBP寄存器列表  :
*****************************************************************************/
HI_U32 HI_BOARD_TF_RxPhaseDisPlay(HI_IN HI_U8 phase_index)
{
    HI_U32 ret = HI_ERR_FAILURE;
    HI_LCD_DISPLAY_INFO_S lcd_cfg = {{0}, 0};

    //配置数码管显示台区编号
    lcd_cfg.data[HI_LCD_POSITION_1] = HI_LCD_NOT_FLUSH;
    lcd_cfg.data[HI_LCD_POSITION_2] = (HI_CHAR)('A' + phase_index);
    lcd_cfg.data[HI_LCD_POSITION_3] = HI_LCD_NOT_FLUSH;
    lcd_cfg.pri = HI_LCD_DISPLAY_PRI_2;
    lcd_cfg.tsec = HI_LCD_DISPLAY_FOREVER;
    ret = HI_DRV_LCD_DisplayPrint(HI_LCD_FUNC_ID_TF, &lcd_cfg);

    return ret;
}
/*****************************************************************************
 函 数 名  : HI_BOARD_TF_LevelDisPlay
 功能描述  : 设置抄控宝接收成功时档位显示，K,M
 输入参数  : HI_VOID
 输出参数  : 无
 返 回 值  : HI_VOID
 本函数调用的函数  :
 调用本函数的函数  :
 访问的全局变量  :
 修改的全局变量  :
 修改的BBP寄存器列表  :
*****************************************************************************/
HI_U32 HI_BOARD_TF_LevelDisPlay(HI_IN HI_U8 gear)
{
    HI_U32 ret = HI_ERR_FAILURE;
    HI_LCD_DISPLAY_INFO_S lcd_cfg = {{0}, 0};

    //配置数码管显示台区识别接收档位
    lcd_cfg.data[HI_LCD_POSITION_1] = HI_LCD_NOT_FLUSH;
    lcd_cfg.data[HI_LCD_POSITION_2] = HI_LCD_NOT_FLUSH;
    if(TF_RCV_MODE_1M == gear)
    {
        lcd_cfg.data[HI_LCD_POSITION_3] = 'M';
    }
    else if(TF_RCV_MODE_240K == gear)
    {
        lcd_cfg.data[HI_LCD_POSITION_3] = 'K';
    }

    lcd_cfg.pri = HI_LCD_DISPLAY_PRI_2;
    lcd_cfg.tsec = HI_LCD_DISPLAY_FOREVER;
    ret = HI_DRV_LCD_DisplayPrint(HI_LCD_FUNC_ID_TF, &lcd_cfg);

    return ret;
}
/*****************************************************************************
 函 数 名  : HI_BOARD_TF_RxNokDisPlay
 功能描述  : 设置抄控宝识别失败显示NOK
 输入参数  : HI_VOID
 输出参数  : 无
 返 回 值  : HI_VOID
 本函数调用的函数  :
 调用本函数的函数  :
 访问的全局变量  :
 修改的全局变量  :
 修改的BBP寄存器列表  :
*****************************************************************************/
HI_U32 HI_BOARD_TF_RxNokDisPlay(HI_VOID)
{
    HI_U32 ret = HI_ERR_FAILURE;
    HI_LCD_DISPLAY_INFO_S lcd_cfg = {{0}, 0};

    //配置数码管显示'NOK'
    lcd_cfg.data[HI_LCD_POSITION_1] = 'N';
    lcd_cfg.data[HI_LCD_POSITION_2] = 'O';
    lcd_cfg.data[HI_LCD_POSITION_3] = 'K';
    lcd_cfg.blink = HI_FALSE;
    lcd_cfg.pri = HI_LCD_DISPLAY_PRI_2;
    lcd_cfg.tsec = HI_LCD_DISPLAY_FOREVER;
    ret = HI_DRV_LCD_DisplayPrint(HI_LCD_FUNC_ID_TF, &lcd_cfg);

    return ret;
}
/*****************************************************************************
 函 数 名  : HI_BOARD_TF_DisablePhaseDisPlay
 功能描述  : 发射机发送扰动时，对应相位没有扰动，显示"-"
 输入参数  : HI_VOID
 输出参数  : 无
 返 回 值  : HI_VOID
 本函数调用的函数  :
 调用本函数的函数  :
 访问的全局变量  :
 修改的全局变量  :
 修改的BBP寄存器列表  :
*****************************************************************************/
HI_U32 HI_BOARD_TF_DisablePhaseDisPlay(HI_VOID)
{
    HI_U32 ret = HI_ERR_FAILURE;
    HI_LCD_DISPLAY_INFO_S lcd_cfg = {{0}, 0};

    //配置数码管显示相位
    lcd_cfg.data[HI_LCD_POSITION_1] = HI_LCD_NOT_FLUSH;
    lcd_cfg.data[HI_LCD_POSITION_2] = HI_LCD_NOT_FLUSH;
    lcd_cfg.data[HI_LCD_POSITION_3] = (HI_CHAR)('-');
    lcd_cfg.blink = HI_TRUE;
    lcd_cfg.pri = HI_LCD_DISPLAY_PRI_2;
    lcd_cfg.tsec = HI_LCD_DISPLAY_FOREVER;
    ret = HI_DRV_LCD_DisplayPrint(HI_LCD_FUNC_ID_TF, &lcd_cfg);

    return ret;
}
/*****************************************************************************
 函 数 名  : HI_BOARD_TF_ErrDisPlay
 功能描述  : 发射机出现异常时，LCD界面显示错误码
 输入参数  : HI_VOID
 输出参数  : 无
 返 回 值  : HI_VOID
 本函数调用的函数  :
 调用本函数的函数  :
 访问的全局变量  :
 修改的全局变量  :
 修改的BBP寄存器列表  :
*****************************************************************************/
HI_U32 HI_BOARD_TF_ErrDisPlay(HI_U8 err_code)
{
    HI_U32 ret = HI_ERR_FAILURE;
    HI_LCD_DISPLAY_INFO_S lcd_cfg = {{0}, 0};

    lcd_cfg.data[HI_LCD_POSITION_1] = 'E';
    lcd_cfg.data[HI_LCD_POSITION_2] = (HI_CHAR)('0' + (err_code /10));
    lcd_cfg.data[HI_LCD_POSITION_3] = (HI_CHAR)('0' + (err_code % 10));

    lcd_cfg.blink = HI_FALSE;
    lcd_cfg.pri = HI_LCD_DISPLAY_PRI_2;
    lcd_cfg.tsec = HI_LCD_DISPLAY_FOREVER;
    ret = HI_DRV_LCD_DisplayPrint(HI_LCD_FUNC_ID_TF, &lcd_cfg);

    return ret;
}

/*****************************************************************************
 函 数 名  : HI_BOARD_TF_DisPlayInit
 功能描述  : 发射机，抄控宝进入正常模式，或非台区模式
 输入参数  : HI_VOID
 输出参数  : 无
 返 回 值  : HI_VOID
 本函数调用的函数  :
 调用本函数的函数  :
 访问的全局变量  :
 修改的全局变量  :
 修改的BBP寄存器列表  :
*****************************************************************************/
HI_VOID HI_BOARD_TF_DisPlayInit(HI_VOID)
{
    HI_DRV_LCD_DisplayDisable(HI_LCD_FUNC_ID_TF);
}
#endif

