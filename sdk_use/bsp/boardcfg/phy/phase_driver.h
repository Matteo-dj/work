#ifndef _PHASE_DRIVER_H_
#define _PHASE_DRIVER_H_

#ifdef __cplusplus
 extern "C" {
#endif

#include "hi_types.h"


//多相位控制模式
#define PHASE_MODE_OFF                  0                   //相位关闭
#define PHASE_MODE_ON                   1                   //相位打开

//多相位的RX通路控制
//接收位开关对应的GPIO管脚
#define RX_PHASE_A_GPIO_PIN             HI_GPIO_IDX_17
#define RX_PHASE_B_GPIO_PIN             HI_GPIO_IDX_18
#define RX_PHASE_C_GPIO_PIN             HI_GPIO_IDX_19


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
HI_EXTERN HI_VOID HI_BOARD_PhaseInit(HI_VOID);

/*****************************************************************************
 函 数 名  : drv_set_tx_single_phase
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
HI_EXTERN HI_VOID HI_BOARD_SetTxSinglePhase(HI_U32 phase_name, HI_U8 phase_mode);
/*****************************************************************************
 函 数 名  : drv_set_rx_single_phase
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
HI_EXTERN HI_VOID HI_BOARD_SetRxSinglePhase(HI_U32 phase_name, HI_U8 phase_mode);

#ifdef __cplusplus
 }
#endif

#endif

