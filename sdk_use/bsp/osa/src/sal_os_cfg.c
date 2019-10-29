#include <hi_types.h>
#include <hi_mdm_types.h>
#include <los_sys.h>
#include <hi_mdm_cfg_early.h>
#include <hi_stdlib.h>

__attribute__((section(".data"))) HI_CFG_OSA_RESOURCE_S g_stOsaResource;



HI_U32 HI_CFG_SetOsaResourceEarly(HI_CFG_OSA_RESOURCE_S *pCfgOsaResouce)
{
    SYS_CONFIG_S os_config;

    os_config.uwTskNum = pCfgOsaResouce->ulTskNum;
    os_config.uwQueNum = pCfgOsaResouce->ulQueNum;
    os_config.uwSemNum = pCfgOsaResouce->ulSemNum;
    os_config.uwMuxNum = pCfgOsaResouce->ulMuxNum;
    os_config.uwSwtNum = pCfgOsaResouce->ulSwtNum;
    os_config.uwWorkqNum = pCfgOsaResouce->ulWorkqNum;
    memcpy_s(&g_stOsaResource,sizeof(HI_CFG_OSA_RESOURCE_S),pCfgOsaResouce,sizeof(HI_CFG_OSA_RESOURCE_S));    
    LOS_set_config(&os_config);
    return HI_ERR_SUCCESS;
}

