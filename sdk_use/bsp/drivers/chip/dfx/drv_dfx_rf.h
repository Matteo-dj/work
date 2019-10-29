
#ifndef __DRV_DFX_RF_H__
#define __DRV_DFX_RF_H__
#include <hi_types.h>
#include <hi_mdm_types.h>
HI_U32 dfx_rf_cmd_stat(HI_U16 usId, HI_IN HI_PVOID pCmdParam, HI_U16 usCmdParamSize, HI_U32 ulOption);

HI_U32 dfx_rf_cmd_order(HI_U16 usId, HI_IN HI_PVOID pCmdParam, HI_U16 usCmdParamSize, HI_U32 ulOption);

HI_U32 dfx_rf_cmd_op_enable(HI_U16 usId, HI_IN HI_PVOID pCmdParam, HI_U16 usCmdParamSize, HI_U32 ulOption);

HI_U32 dfx_rf_cmd_operate(HI_U16 usId, HI_IN HI_PVOID pCmdParam, HI_U16 usCmdParamSize, HI_U32 ulOption);
#endif

