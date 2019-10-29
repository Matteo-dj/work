
#ifndef __DRV_DFX_OS_INFO_H__
#define __DRV_DFX_OS_INFO_H__
#include <hi_types.h>
#include <hi_mdm_types.h>
HI_U32  new_mem_show_temp(HI_U16 usId, HI_IN HI_PVOID pCmdParam, HI_U16 usCmdParamSize, HI_U32 ulOption);
HI_U32 dfx_cpu_show(HI_U16 usId, HI_IN HI_PVOID pCmdParam, HI_U16 usCmdParamSize, HI_U32 ulOption); 
HI_U32 dfx_mem_show(HI_U16 usId, HI_IN HI_PVOID pCmdParam, HI_U16 usCmdParamSize, HI_U32 ulOption);    
HI_U32 dfx_task_show(HI_U16 usId, HI_IN HI_PVOID pCmdParam, HI_U16 usCmdParamSize, HI_U32 ulOption);
HI_U32 dfx_timer_show(HI_U16 usId, HI_IN HI_PVOID pCmdParam, HI_U16 usCmdParamSize, HI_U32 ulOption);
#endif

