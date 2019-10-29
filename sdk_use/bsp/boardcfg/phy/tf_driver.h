#ifndef _TF_DRIVER_H_
#define _TF_DRIVER_H_

#ifdef __cplusplus
 extern "C" {
#endif

#include "hi_types.h"
#include "hi_mdm_io.h"

#define ZERO_CROSS_IN_GPIO_PIN          HI_GPIO_IDX_6                //过零中断输入GPIO

//台区识别接收机的档位切换模式
#define TF_RCV_MODE_AUTO                    0x00    // 自动切换模式
#define TF_RCV_MODE_240K                    0x01    // 固定为240K接收模式
#define TF_RCV_MODE_1M                      0x02    // 固定为1M接收模式

 //NDM发送沿模式
#define TF_SEND_EDGE_MODE_OFF                               0           //发送沿开关关闭
#define TF_SEND_EDGE_MODE_UP                                1           //上升沿发送模式
#define TF_SEND_EDGE_MODE_DOWN                              2           //下降沿发送模式

#define TF_TX_OUT_DATA_PIN                    HI_GPIO_IDX_20           //发送端的数据输出GPIO，GPIO20
#define TF_NDM_ZERO_DTC_RX_PIN        IO_EXP_IO_NUM_13     // 接收机接收管脚
#define TF_NDM_BEEP_PIN                           IO_EXP_IO_NUM_14    // 接收机蜂鸣器管脚
#define TF_NDM_PHASE_A_PIN                    IO_EXP_IO_NUM_00    // 发射机发送A相序列指示灯
#define TF_NDM_PHASE_B_PIN                    IO_EXP_IO_NUM_01    // 发射机发送B相位序列指示灯
#define TF_NDM_PHASE_C_PIN                    IO_EXP_IO_NUM_05    // 发射机发送C相位序列指示灯
#define TF_NDM_AN_CHECK_PIN                IO_EXP_IO_NUM_06    // 发射机上电检测AN过压的扩展IO0_6管脚
#define TF_NDM_AN_CHECK_READ_PIN   IO_EXP_IO_NUM_07   // 发射机上电检测AN过压，读取信号扩展IO0_7管脚
#define TF_NDM_TX_ERR_LED_PIN            IO_EXP_IO_NUM_12    // 发射机故障指示灯，扩展IO1_2管脚


typedef struct
{
    HI_U32 gpio_direction;                     // GPIO的输入，输出方向
    HI_GPIO_VALUE_E gpio_value;                //GPIO 的高低电平
}DRV_GPIO_VAL_AND_DIREC_STRU;

//台区识别NDM模式
typedef enum
{
    TF_IDENTIFY_NORMAL_MODE = 0,
    TF_IDENTIFY_RCV_MODE,
    TF_IDENTIFY_SEND_MODE,
    TF_IDENTIFY_LCD_OFF_MODE,
}TF_IDENTIFY_MODE_E;
HI_VOID HI_BOARD_TfGpioInit(HI_VOID);
HI_VOID HI_BOARD_ZeroCrossInGpioInit(HI_VOID);
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

 修改历史      :
  1.日    期   : 2016年10月17日
    作    者   : sunhaiyang/00238140
    修改内容   : 新生成函数

*****************************************************************************/
HI_EXTERN HI_VOID HI_BOARD_SetTfTxOutDataGpioInit(HI_VOID);
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
HI_EXTERN HI_VOID HI_BOARD_SetTfSendEdgeModeUpGpioInit(HI_VOID);

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
HI_EXTERN HI_VOID HI_BOARD_SetTfSendEdgeModeDownGpioInit(HI_VOID);
HI_VOID HI_BOARD_SetSwitchLevel(HI_U8 level);
HI_VOID HI_BOARD_SetSendEdgeMode(HI_U8 edge_mode);
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
HI_EXTERN HI_VOID HI_BOARD_SetPhaseGpioMode(HI_VOID);
/*****************************************************************************
 函 数 名  : HI_BOARD_SetPhaseGpioDirecInit
 功能描述  : 设置GPIO2,3,4为初始化为输出
 输入参数  : HI_VOID
 输出参数  : 无
 返 回 值  : HI_VOID
 本函数调用的函数  :
 调用本函数的函数  :
 访问的全局变量  :
 修改的全局变量  :
 修改的BBP寄存器列表  :
*****************************************************************************/
HI_EXTERN HI_VOID HI_BOARD_SetPhaseGpioDirecInit(HI_VOID);
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
HI_EXTERN HI_VOID HI_BOARD_SetPhaseGpioValue(HI_IN HI_U8 phase_mode);
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
HI_EXTERN HI_VOID HI_BOARD_SetSinglePhaseGpioValue(HI_IN HI_GPIO_IDX_E gpio_pin,HI_IN HI_U8 phase_mode);
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
HI_EXTERN HI_U32 HI_BOARD_SetBeepValue(HI_BOOL onoff);
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
HI_EXTERN HI_U32 HI_BOARD_SetBeepGpioInit(HI_VOID);
/*****************************************************************************
 函 数 名  : drv_set_ndm_zero_dtc_rx_init
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
HI_EXTERN HI_U32 HI_BOARD_SetNdmZeroDtcRxInit(HI_VOID);
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
HI_EXTERN HI_U32 HI_BOARD_SetNdmZeroDtcRxValue(HI_BOOL gpio_value);
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
HI_EXTERN HI_VOID HI_BOARD_SetAnCheckClearGpioInit(HI_VOID);
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

 修改历史      :
  1.日    期   : 2015年11月23日
    作    者   : sunhaiyang/00238140
    修改内容   : 新生成函数

*****************************************************************************/
HI_EXTERN HI_VOID HI_BOARD_SetAnCheckClearGpioValue(HI_BOOL gpio_value);
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
HI_EXTERN HI_VOID HI_BOARD_SetAnCheckGpioInit(HI_VOID);
/*****************************************************************************
 函 数 名  : drv_get_an_check_value
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
HI_EXTERN HI_U8 HI_BOARD_GetAnCheckValue(HI_VOID);
HI_U32 HI_BOARD_SetOutSeqData(HI_U8 send_bit);
HI_VOID HI_BOARD_SetPhaseSwitchCtrlMode(HI_VOID);

#if defined(PRODUCT_CFG_PRODUCT_TYPE_CCO)
HI_VOID HI_BOARD_SetCcoZeroCrossEnable(HI_U8 power_edge_type, HI_U8 phase);
#endif

/*****************************************************************************
 函 数 名  : HI_BOARD_SetThreeMeterStaGpioMode
 功能描述  : 设置三相表STA模块控制GPIO管脚模式
 输入参数  : HI_VOID
 输出参数  : 无
 返 回 值  : HI_VOID
 本函数调用的函数  :
 调用本函数的函数  :
 访问的全局变量  :
 修改的全局变量  :
 修改的BBP寄存器列表  :
*****************************************************************************/
HI_VOID HI_BOARD_SetThreeMeterStaGpioMode(HI_VOID);

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
HI_VOID HI_BOARD_SetThreeMeterStaGpioDirAndValueInit(HI_VOID);
#ifdef __cplusplus
 }
#endif

#endif
