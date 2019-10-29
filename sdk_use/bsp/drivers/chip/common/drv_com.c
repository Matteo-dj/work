//*****************************************************************************
//
//                  版权所有 (C), 2012-2022, 华为技术有限公司
//
//*****************************************************************************
//  文 件 名   : drv_com.c
//  版 本 号   : V1.0
//  生成日期   : 2012-08-16
//  功能描述   : DRV通用相关接口
//  函数列表   : NA
//  修改历史   :
//  1.日    期 : 2012-08-16 创建
//*****************************************************************************
/* includes */
#include <hi_types.h>
#include "drv_com.h"
#include "hi_platform.h"
#include <hi_drv_com.h>
#include <hi_driver_internal.h>
#include <hi_sal_nv.h>
#include <stdlib.h>
#include <hi_sal.h>
#include <os_vx_sal_api.h>
#include "hi_ioexp.h"
#include <hi_sal_cfg.h>
#include <hi_mdm_sys.h>
#include <hi_mdm_isr.h>
#include <los_memory.h>

HI_U32 g_config_env_offset;
HI_U32 g_config_env_offset_redund;

#define GENERAL_REG_V200_MAGIC (0x39110200)

/*设置软件内存大小*/
/*初始化阶段使用，不再进行保护*/
HI_U32 g_ulSoftWareMemSize = USE_RAM_DEFAULT_SIZE;

HI_U32 HI_MDM_MEM_SetSwSize(HI_IN HI_U32 ulSize)
{
    if (HI_CFG_InitEarly_End())
        return HI_ERR_INVOKE_SEQUENCE;
    if(ulSize > HI_MDM_GetHwMemSize())
        return HI_ERR_FAILURE;
    
    g_ulSoftWareMemSize = ulSize;
    g_sys_mem_addr_end = 0x70000000 + g_ulSoftWareMemSize;
    return HI_ERR_SUCCESS;
}

/*获取软件内存大小*/
HI_U32 HI_MDM_MEM_GetSwSize(HI_VOID)
{
	return  g_ulSoftWareMemSize;       
}


HI_VOID HI_COM_Sys_Reset(HI_VOID)
{
	HI_REG_WRITE32(HI_SYSCTRL_REG_BASE + SC_SYS_RES , 0x0);
}

HI_VOID HI_COM_SetEnvAddr(HI_VOID)
{
	g_config_env_offset = *(HI_U32 *)GENERAL_REG_ENV_ADDR1;
	g_config_env_offset_redund = *(HI_U32 *)GENERAL_REG_ENV_ADDR2;
}

HI_BOOL IsV200Boot(HI_VOID)
{
	HI_U32 ulVal;
	
	HI_REG_READ32(GENERAL_REG_V200_FLAG, ulVal);
	if (GENERAL_REG_V200_MAGIC == (ulVal))
	{
		return HI_TRUE;
	}
	else
	{
		return HI_FALSE;
	}
}

HI_U32 HI_COM_GetBootVer(HI_U8 acBootVer[3])
{
	if (acBootVer == HI_NULL)
	{
		return HI_ERR_INVALID_PARAMETER;
	}
	if (IsV200Boot())
	{
		HI_U32 val = *(HI_U32*)GENERAL_REG_BOOT_VER;
		acBootVer[0] = (HI_CHAR)((val&(0x00FF0000))>>8);
		acBootVer[1] = (HI_CHAR)((val&(0x0000FF00))>>4); 
		acBootVer[2] = (HI_CHAR)(val&(0x000000FF));
		return HI_ERR_SUCCESS;
	}
	return HI_ERR_BAD_DATA;
}



HI_U32 HI_MDM_SetWorkStatus(HI_U32 ulStatus)
{
    HI_U32 ret = HI_ERR_SUCCESS;
    
    if((HI_ND_SYS_STATUS_NORMAL != ulStatus) && (HI_ND_SYS_STATUS_TEST != ulStatus))
    {
        return HI_ERR_NOT_SUPPORT_ID;
    }
    HI_NV_FTM_STARTUP_CFG_S *pStartUp = (HI_NV_FTM_STARTUP_CFG_S *)HI_MDM_Malloc(HI_MOD_ID_SAL_NVM,sizeof(HI_NV_FTM_STARTUP_CFG_S));
    if(pStartUp==HI_NULL)
    {
        return HI_ERR_MALLOC_FAILUE;
    }
    ret = HI_NVM_Read(HI_NV_FTM_STARTUP_CFG_ID,pStartUp,sizeof(HI_NV_FTM_STARTUP_CFG_S));
    if(ret!=HI_ERR_SUCCESS)
    {
        HI_MDM_Free(HI_MOD_ID_SAL_NVM,pStartUp);
        return ret;
    }
    pStartUp->ulStatus = ulStatus;
    ret = HI_NVM_WriteFactory(HI_NV_FTM_STARTUP_CFG_ID,pStartUp,sizeof(HI_NV_FTM_STARTUP_CFG_S));
    HI_MDM_Free(HI_MOD_ID_SAL_NVM,pStartUp);
    return ret;

}


