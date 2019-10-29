#include <hi_types.h>
#include <hi_mdm_types.h>
#include <dfx_uart.h>
#include "drv_dfx_uart.h"
#include "hi_sal.h"
#include "hi_mdm_uart.h"


HI_U32  dfx_get_uart_stat(HI_U16 usId, HI_IN HI_PVOID pCmdParam, HI_U16 usCmdParamSize, HI_U32 ulOption)
{

    HI_DIAG_UART_STAT_CMD_S* pstCmdParam = (HI_DIAG_UART_STAT_CMD_S*)pCmdParam;
    HI_DIAG_UART_STAT_IND_S uart_info ={0};

        
    if( pstCmdParam->uart_num  >= HI_UART_MAX)
    {
        uart_info.ret = HI_ERR_INVALID_PARAMETER;
        HI_DIAG_ReportPacket(usId, (HI_U8)ulOption, (HI_U8*)(&uart_info), sizeof(HI_DIAG_UART_STAT_IND_S), HI_TRUE);
        return HI_ERR_INVALID_PARAMETER;
    }

    uart_info.ret = HI_MDM_UART_DfxGetStat((HI_UART_PORT_E)(pstCmdParam->uart_num), &(uart_info.stCfg), &(uart_info.stStat));
    HI_DIAG_ReportPacket(usId, (HI_U8)ulOption, (HI_U8*)(&uart_info), sizeof(HI_DIAG_UART_STAT_IND_S), HI_TRUE);
    
    return HI_ERR_SUCCESS;
}


