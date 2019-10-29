#ifndef __DRV_DFX_TRACK_H__
#define __DRV_DFX_TRACK_H__
#include <hi_types.h>
#include <hi_mdm_types.h>



HI_U32 dfx_track_start(HI_U16 usId, HI_IN HI_PVOID pCmdParam, HI_U16 usCmdParamSize, HI_U32 ulOption);
HI_U32 dfx_track_end(HI_U16 usId, HI_IN HI_PVOID pCmdParam, HI_U16 usCmdParamSize, HI_U32 ulOption);
HI_U32 dfx_track_get_info(HI_U16 usId, HI_IN HI_PVOID pCmdParam, HI_U16 usCmdParamSize, HI_U32 ulOption);

#endif

