
#ifndef __DRV_DFX_FLASH__
#define __DRV_DFX_FLASH__

#include <hi_types.h>
#include <hi_mdm_types.h>

HI_U32 dfx_get_flash_erase_stat(HI_U16 usId, HI_IN HI_PVOID pCmdParam, HI_U16 usCmdParamSize, HI_U32 ulOption);

#ifdef PRODUCT_CFG_FLASH_PROTECT
HI_U32 dfx_flash_protect_op(HI_U16 usId, HI_IN HI_PVOID pCmdParam, HI_U16 usCmdParamSize, HI_U32 ulOption);
#endif

#endif

