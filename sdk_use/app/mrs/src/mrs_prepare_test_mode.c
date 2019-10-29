
#include "hi_low_power.h"
#include "hi_sal_cfg.h"
#include "app_mrs.h"
#include "mrs_prepare_test_mode.h"

//*****************************************************************************
// 函数名称: MRS_ChangeDiagUartConfig
// 功能描述: 刷新调试串口配置
//
// 参数说明:
//
// 返 回 值:
//    HI_ERR_SUCCESS: 成功修改波特率
//    其它错误码
//
// 调用要求: TODO: ...在调用完HI_MDM_UART_SetFreq接口后再调用该接口，实现分频值的重新配置。
// 调用举例: TODO: ...
//*****************************************************************************
HI_U32 MRS_RefreshDiagUartConfig(HI_VOID)
{
    HI_UART_INIT_TYPE_S initData;
    
    initData.baud_rate = 115200;
    initData.data_bits = 8;
    initData.stop_bits = 1;
    initData.parity = 0;
    
	return HI_MDM_UART_Ioctl(1, UART_CFG_SET_ATTR, (HI_U32)&initData);     
}

//*****************************************************************************
// 函数名称: MRS_PrepareTestMode
// 参数说明:
//          ulRate:待配置的业务串口波特率
//          ucParity:待配置的业务串口奇偶校验

// 功能描述: 进入测试模式前准备串口的配置。更改uart时钟源后重新初始化串口配置。
//*****************************************************************************
HI_VOID MRS_PrepareUartConfig(HI_U32 ulRate, HI_U8 ucParity)
{
    HI_LOW_POWER_Forbid(HI_LOW_POWER_FORBID_ID_TEST_MODE);
    HI_MDM_UART_SetFreq(UART_CLK_APB);
    MRS_ChangeBaudRate(ulRate, ucParity);
    MRS_RefreshDiagUartConfig();
}

//*****************************************************************************
// 函数名称: MRS_RestoreFromTestMode
// 参数说明:
//          ulRate:业务串口待恢复的波特率
//          ucParity:业务串口待恢复的奇偶校验

// 功能描述: 退出测试模式后，恢复串口的配置。
//*****************************************************************************
HI_VOID MRS_RestoreUartConfig(HI_U32 ulRate, HI_U8 ucParity)
{   
    HI_MDM_UART_SetFreq(UART_CLK_XTAL);
	MRS_ChangeBaudRate(ulRate, ucParity);
    MRS_RefreshDiagUartConfig();
    HI_LOW_POWER_UnForbid(HI_LOW_POWER_FORBID_ID_TEST_MODE,0);
}

            