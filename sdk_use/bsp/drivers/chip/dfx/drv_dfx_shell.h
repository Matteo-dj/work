
#ifndef __DRV_DFX_SHELL_H__
#define __DRV_DFX_SHELL_H__
#include <hi_types.h>
#include <hi_mdm_types.h>

typedef struct
{
    HI_U32 argc;//参数个数
    HI_CHAR *argv[10];//参数列表
    HI_CHAR *pReq;//buf指针:主要用于存储req
    HI_CHAR *pOutLineBuf;//buf指针:主要用于存储ind
}HI_SHELL_CMD_PRE_DO_HANDLE;

//*****************************************************************************
// 函数名称:  diag_shell_cmd_prepare
//
// 功能描述: 预申请资源,并将长字符串参数转成多个参数
//
// 参数说明:
//      pCmdParam:diag的pCmdParam
//      usCmdParamSize:diag的usCmdParamSize
//      ulOutLineSize:要申请的pOutLineBuf的size
// 返 回 值:
//      HI_NULL:申请资源失败
//*****************************************************************************

HI_SHELL_CMD_PRE_DO_HANDLE *diag_shell_cmd_prepare(HI_PVOID pCmdParam,HI_U32 usCmdParamSize,HI_U32 ulOutLineSize); 

HI_VOID diag_shell_cmd_restore(HI_SHELL_CMD_PRE_DO_HANDLE *pHandle);

#endif

