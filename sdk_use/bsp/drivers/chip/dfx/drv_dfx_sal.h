
#ifndef __DRV_DFX_SAL_H__
#define __DRV_DFX_SAL_H__
#include <hi_types.h>
#include <hi_mdm_types.h>
HI_U32 newdiagSysObjQry(HI_U16 usId, HI_IN HI_PVOID pCmdParam, HI_U16 usCmdParamSize, HI_U32 ulOption);
HI_U32 dfx_system_rst(HI_U16 usId, HI_IN HI_PVOID pCmdParam, HI_U16 usCmdParamSize, HI_U32 ulOption);
HI_U32  dfx_3518_system_rst(HI_U16 usId, HI_IN HI_PVOID pCmdParam, HI_U16 usCmdParamSize, HI_U32 ulOption);

#endif

