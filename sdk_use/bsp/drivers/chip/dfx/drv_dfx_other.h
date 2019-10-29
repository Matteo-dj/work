#ifndef __DRV_DFX_OTHER_H__
#define __DRV_DFX_OTHER_H__
#include <hi_types.h>
#include <hi_mdm_types.h>
HI_U32  dfx_get_work_mode(HI_U16 usId, HI_IN HI_PVOID pCmdParam, HI_U16 usCmdParamSize, HI_U32 ulOption);

HI_U32  dfx_set_work_mode(HI_U16 usId, HI_IN HI_PVOID pCmdParam, HI_U16 usCmdParamSize, HI_U32 ulOption);


//获取以太网参数
HI_U32 dfx_get_eth_cfg(DRV_DFX_ETH_CFG* pstEthcfg);

//设置以太网参数
HI_U32 dfx_set_eth_cfg(DRV_DFX_ETH_CFG* pstEthcfg);


HI_U32 dfx_eth_cfg(HI_U16 usId, HI_IN HI_PVOID pCmdParam, HI_U16 usCmdParamSize, HI_U32 ulOption);
#endif

