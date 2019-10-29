
#ifndef __DFX_UART_H__
#define __DFX_UART_H__
#include <hi_types.h>
#include <hi_mdm_types.h>
#include <hi_mdm_uart.h>


typedef struct
{
    HI_U32 ret;
    HI_UART_INIT_TYPE_S stCfg;
    HI_UART_DXF_STAT_S stStat;
}HI_DIAG_UART_STAT_IND_S;//上发

typedef struct
{
    HI_U32 uart_num; //串口号
}HI_DIAG_UART_STAT_CMD_S;  //下发

#endif

