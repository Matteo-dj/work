/******************************************************************************

                  版权所有 (C), 2001-2016, 华为技术有限公司

 ******************************************************************************
  文 件 名   : drv_board.c
  版 本 号   : 初稿
  作    者   : liujian 00280874
  生成日期   : 2017年10月31日
  最近修改   :
  功能描述   : 相位相关GPIO接口
  函数列表   :
  修改历史   :
  1.日    期   : 2017年10月31日
    作    者   :  liujian 00280874
    修改内容   : 创建文件

******************************************************************************/
#include "phase_driver.h"
#include "hi_mdm_switch.h"
#include "hi_mdm_io.h"
#include "hi_mdm.h"
#include "hi_driver.h"


/*****************************************************************************
 函 数 名  : HI_BOARD_PhaseInit
 功能描述  : 多相位初始化
 输入参数  : HI_VOID
 输出参数  : 无
 返 回 值  : HI_VOID
 本函数调用的函数  :
 调用本函数的函数  :
 访问的全局变量  :
 修改的全局变量  :
 修改的BBP寄存器列表  :
*****************************************************************************/
HI_VOID HI_BOARD_PhaseInit(HI_VOID)
{
    //多相位的TX通路初始化
    //控制信号电平配置

    HI_MDM_SWITCH_Off(HI_SWITCH_IDX_0);
    HI_MDM_SWITCH_Off(HI_SWITCH_IDX_1);
    HI_MDM_SWITCH_Off(HI_SWITCH_IDX_2);
    //多相位的RX通路初始化
    //设置GPIO的属性
    HI_MDM_IO_SetDir(RX_PHASE_A_GPIO_PIN, HI_GPIO_DIRECTION_OUT);
    HI_MDM_IO_SetDir(RX_PHASE_B_GPIO_PIN, HI_GPIO_DIRECTION_OUT);
    HI_MDM_IO_SetDir(RX_PHASE_C_GPIO_PIN, HI_GPIO_DIRECTION_OUT);
}

/*****************************************************************************
 函 数 名  : HI_BOARD_SetTxSinglePhase
 功能描述  : 多相位的TX通路配置
 输入参数  : HI_U8 phase_name : 相位名(PHASE_A、PHASE_B、PHASE_C)
             HI_U8 phase_mode : 相位工作模式(PHASE_MODE_OFF、PHASE_MODE_ON)
 输出参数  : 无
 返 回 值  : HI_VOID
 本函数调用的函数  :
 调用本函数的函数  :
 访问的全局变量  :
 修改的全局变量  :
 修改的BBP寄存器列表  :
*****************************************************************************/
HI_VOID HI_BOARD_SetTxSinglePhase(HI_U32 phase_name, HI_U8 phase_mode)
{
    if(phase_mode)
    {
        HI_MDM_SWITCH_On((HI_SWITCH_IDX_E)(phase_name-1));
    }
    else
    {
        HI_MDM_SWITCH_Off((HI_SWITCH_IDX_E)(phase_name-1));
    }
}

/*****************************************************************************
 函 数 名  : HI_BOARD_SetRxSinglePhase
 功能描述  : 多相位的RX通路配置
 输入参数  : HI_U8 phase_name : 相位名(PHASE_A、PHASE_B、PHASE_C)
             HI_U8 phase_mode : 相位工作模式(PHASE_MODE_OFF、PHASE_MODE_ON)
 输出参数  : 无
 返 回 值  : HI_VOID
 本函数调用的函数  :
 调用本函数的函数  :
 访问的全局变量  :
 修改的全局变量  :
 修改的BBP寄存器列表  :
*****************************************************************************/
HI_VOID HI_BOARD_SetRxSinglePhase(HI_U32 phase_name, HI_U8 phase_mode)
{
    HI_GPIO_VALUE_E gpio_val;

    gpio_val = (HI_GPIO_VALUE_E)((PHASE_MODE_ON == phase_mode) ?  HI_GPIO_VALUE1 : HI_GPIO_VALUE0);

    switch(phase_name)
    {
        case PHASE_A :
            HI_MDM_IO_SetOutputVal(RX_PHASE_A_GPIO_PIN, gpio_val);
            break;

        case PHASE_B :
            HI_MDM_IO_SetOutputVal(RX_PHASE_B_GPIO_PIN, gpio_val);
            break;

        case PHASE_C :
            HI_MDM_IO_SetOutputVal(RX_PHASE_C_GPIO_PIN, gpio_val);
            break;

        default :
            HI_MDM_IO_SetOutputVal(RX_PHASE_A_GPIO_PIN, gpio_val);
    }
}


