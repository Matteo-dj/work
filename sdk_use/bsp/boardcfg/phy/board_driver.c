/******************************************************************************

                  版权所有 (C), 2001-2016, 华为技术有限公司

 ******************************************************************************
  文 件 名   : drv_board.c
  版 本 号   : 初稿
  作    者   : liujian 00280874
  生成日期   : 2017年10月31日
  最近修改   :
  功能描述   : 板端外围电路相关GPIO接口
  函数列表   :
  修改历史   :
  1.日    期   : 2017年10月31日
    作    者   :  liujian 00280874
    修改内容   : 创建文件

******************************************************************************/
#include "board_driver.h"
#include "hi_mdm_io.h"
#include "hi_mdm_types.h"
#include "hi_sal.h"
#include "hi_driver.h"

HI_MDM_BOARD_CTRL_INFO_S g_BoardCtrlInfo = {0};

HI_MDM_BOARD_CTRL_INFO_S* HI_MDM_GetBoardCtrlInfo(HI_VOID)
{
    return &g_BoardCtrlInfo;
}

HI_VOID HI_MDM_SetBoardCtrlInfoSysStatus(HI_U32 SysStatus)
{
    HI_MDM_BOARD_CTRL_INFO_S* p_BoardCtrlInfo = HI_MDM_GetBoardCtrlInfo();

    p_BoardCtrlInfo->SysStatus = SysStatus;
}

HI_VOID HI_MDM_GpioInit(HI_VOID)
{
    HI_GPIO_IDX_E sta_gpio = HI_GPIO_IDX_0;
    HI_MDM_BOARD_CTRL_INFO_S* p_BoardCtrlInfo = HI_MDM_GetBoardCtrlInfo();

    //在工装模式下，不对这三个管脚进行初始化
    if(HI_ND_SYS_STATUS_TEST == p_BoardCtrlInfo->SysStatus)
    {
        return;
    }

    //将STA的GPIO7管脚初始化为低电平
    if(HI_TRUE == HI_MDM_IsNeedControlStaPin())
    {
        #ifdef PRODUCT_CFG_BOARD_TYPE_ASIC_V200
        sta_gpio = HI_GPIO_IDX_16;
        #else
        sta_gpio = HI_GPIO_IDX_7;
        #endif

        HI_MDM_IO_SetDir(sta_gpio, HI_GPIO_DIRECTION_OUT);
        HI_MDM_IO_SetOutputVal(sta_gpio, HI_GPIO_VALUE1);
    }
}

/*****************************************************************************
函 数 名  : HI_MDM_ControlCircuitBeforeSend
功能描述  : 发送前设置GPIO管脚电平
输入参数  : SendLength 报文发送长度
输出参数  : 无
返 回 值  : 符号数
本函数调用的函数  :
调用本函数的函数  :
访问的全局变量  :
修改的全局变量  :
修改的BBP寄存器列表  :
*****************************************************************************/
HI_VOID HI_MDM_ControlCircuitBeforeSend(HI_IN HI_U32 SendLength)
{
    #ifdef PRODUCT_CFG_VERSION_RELEASE
    // 报文发送时长大于等于5ms,将管脚电平拉高
    if(FRAME_SEND_LENGTH_TH <= SendLength)
    {
        HI_MDM_GetBoardCtrlInfo()->StaPinFlag = HI_TRUE;
        HI_MDM_StaPinControl(PIN_HIGH);
    }
    #endif
}

/*****************************************************************************
函 数 名  : HI_MDM_ControlCircuitAfterSend
功能描述  : 发送后设置GPIO管脚电平
输入参数  : HI_VOID
输出参数  : 无
返 回 值  : 符号数
本函数调用的函数  :
调用本函数的函数  :
访问的全局变量  :
修改的全局变量  :
修改的BBP寄存器列表  :
*****************************************************************************/
HI_VOID HI_MDM_ControlCircuitAfterSend(HI_VOID)
{
    // 没有发送成功，则将STA管脚GPIO7置为低电平
    #ifdef PRODUCT_CFG_VERSION_RELEASE
    HI_MDM_BOARD_CTRL_INFO_S* p_BoardCtrlInfo = HI_MDM_GetBoardCtrlInfo();
    if(HI_TRUE == p_BoardCtrlInfo->StaPinFlag)
    {
        HI_MDM_StaPinControl(PIN_LOW);
        p_BoardCtrlInfo->StaPinFlag = HI_FALSE;
    }
    #endif
}

/*****************************************************************************
函 数 名  : HI_MDM_StaPinControl
功能描述  : STA站点通过GPIO7控制管脚的电平高低
输入参数  : state 电平状态

输出参数  : 无
返 回 值  : 符号数
本函数调用的函数  :
调用本函数的函数  :
访问的全局变量  :
修改的全局变量  :
修改的BBP寄存器列表  :
*****************************************************************************/
HI_VOID HI_MDM_StaPinControl(en_pin_state State)
{
    HI_GPIO_IDX_E StaGpio = HI_GPIO_IDX_0;
    HI_MDM_BOARD_CTRL_INFO_S* p_BoardCtrlInfo = HI_MDM_GetBoardCtrlInfo();

    // 在工装模式下，不控制GPIO7
    if(HI_ND_SYS_STATUS_TEST == p_BoardCtrlInfo->SysStatus)
    {
        return;
    }

    if(HI_FALSE == HI_MDM_IsNeedControlStaPin())
    {
        return;
    }

    #ifdef PRODUCT_CFG_BOARD_TYPE_ASIC_V200
    StaGpio = HI_GPIO_IDX_16;
    #else
    StaGpio = HI_GPIO_IDX_7;
    #endif

    switch(State)
    {
        case PIN_HIGH :
            HI_MDM_IO_SetOutputVal(StaGpio,HI_GPIO_VALUE0);
            break;
        case PIN_LOW:
            HI_MDM_IO_SetOutputVal(StaGpio,HI_GPIO_VALUE1);
            break;
        default:
            break;
    }
}

/*****************************************************************************
函 数 名  : HI_MDM_IsNeedControlStaPin
功能描述  : 判断是否需要控制管脚电平
输入参数  : 无

输出参数  : 无
返 回 值  : 符号数
本函数调用的函数  :
调用本函数的函数  :
访问的全局变量  :
修改的全局变量  :
修改的BBP寄存器列表  :
*****************************************************************************/
HI_BOOL HI_MDM_IsNeedControlStaPin(HI_VOID)
{
    HI_U8 product_type = HI_ND_TYPE_STA;

    product_type = HI_DMS_LoadDevType();
    if((HI_ND_TYPE_STA != product_type) && (HI_ND_TYPE_THREE_STA != product_type))
    {
        return HI_FALSE;
    }

    return HI_TRUE;
}

