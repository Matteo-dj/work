//*****************************************************************************
//
//                  版权所有 (C), 1998-2012, 华为技术有限公司
//
//*****************************************************************************
//  文 件 名   : drv_dfx_sys_val.h																							
//  版 本 号   : V1.0 
//  作    者   : chenghuanhuan/00132816
//  生成日期   : 2012-08-30
//  功能描述   : TODO: ...
//               
//  函数列表   : TODO: ...
//  修改历史   : 
//  1.日    期 : 2012-08-30
//    作    者 : c00132816
//    修改内容 : 创建文件 
//
//*****************************************************************************

//*****************************************************************************
// PROJECT   : 
// SUBSYSTEM :
// MODULE    :  
// OWNER     :  
//*****************************************************************************
#if defined(DRV_DFX_SYS_VAL_DECLARE)
#ifndef __DRV_DFX_SYS_VAL_H__
#define __DRV_DFX_SYS_VAL_H__

#include <hi_driver_internal.h>
#include <dfx_sys.h>
/*消息打印*/
#if defined(PRODUCT_CFG_SUPPORT_UPG)
HI_EXTERN HI_VOID  HI_DFX_MSG_UPG_ReadNvErr ( HI_U32 ulNvId,HI_U32 ulErrNo);
HI_EXTERN HI_VOID  HI_DFX_MSG_UPG_WriteNvErr ( HI_U32 ulNvId,HI_U32 ulErrNo);
HI_EXTERN HI_VOID  HI_DFX_MSG_UPG_ReadFlashErr (HI_U32 ulFlashAddr,HI_S32 ulFlashSize,HI_U32 ulErrNo);
HI_EXTERN HI_VOID  HI_DFX_MSG_UPG_WriteFlashErr (HI_U32 ulFlashAddr,HI_S32 ulFlashSize,HI_U32 ulErrNo);
HI_EXTERN HI_VOID  HI_DFX_MSG_UPG_StartBusyErr (HI_U32  ulStartTimes,HI_U32 ulStatus);
HI_EXTERN HI_VOID  HI_DFX_MSG_UPG_StartParaErr (HI_UPG_START_S *pstStart );
HI_EXTERN HI_VOID  HI_DFX_MSG_UPG_UpdateParaErr (HI_U32 ulStartUpgId,HI_U32 ulUpgId,HI_U16 usBlkIdx,HI_U16 usBlkSize,HI_U8* pucFileBlk );
HI_EXTERN HI_VOID  HI_DFX_MSG_UPG_UpdateVerErr ( HI_U32 ulMagic,HI_UPG_FILE_VER_S  ulRunTimeFileVer,HI_UPG_FILE_VER_S  ulBackUpFileVer );
HI_EXTERN HI_VOID  HI_DFX_MSG_UPG_UpdateCrcErr (HI_U32 ulNewCrcData,HI_U32 ulFileCrcData );
#endif
HI_EXTERN HI_VOID  HI_DFX_MSG_FLASH_Probe (HI_BYTE* ucId,HI_CHAR* pname );
HI_EXTERN HI_VOID  HI_DFX_MSG_FLASH_OP (HI_U8 uctype,HI_U32 uladdr, HI_U32 ullen );
HI_EXTERN HI_VOID  HI_DFX_MSG_UART_CHANGE_PARA (HI_U32 ulport, HI_UART_INIT_TYPE_S * initstruct);
HI_EXTERN HI_VOID  HI_DFX_MSG_SPI_TIMEOUT (HI_U32 ulport, HI_U32 uldirection, HI_U32 ultimeout);
/*统计量全局变量*/
HI_EXTERN    HI_DSID_DRV_ETH_INFO_S g_drv_eth_info;
HI_EXTERN    HI_DSID_DRV_HRT_INFO_S g_drv_hrt_info;
// TODO
#endif //__DRV_DFX_SYS_VAL_H__
#else

#include "spifdrv.h"

HI_EXTERN SFC_HOST_S g_stSfc300Host;
HI_DSID_DRV_ETH_INFO_S g_drv_eth_info = {HI_OBJ64_S_INIT_DEFINED,0,};
HI_DSID_DRV_HRT_INFO_S g_drv_hrt_info = {HI_OBJ64_S_INIT_DEFINED,0,};

/*统计量*/


// STAT. TBL
HI_PRV HI_CONST HI_DIAG_SYS_STAT_OBJ_S g_stDrvStatObjTbl[] = 
{
    {HI_DSID_DRV_FLASH_INFO,    HI_SYS_DMID_BUSINESS_L2,   HI_DSID_TYPE_CUSTOM,  1, 
        sizeof(HI_DSID_DRV_FLASH_INFO_S), &(g_stSfc300Host.stSpiFlashInfo[0])},
        
#if defined(PRODUCT_CFG_SUPPORT_UPG)
    {HI_DSID_DRV_ETH_INFO,HI_SYS_DMID_BUSINESS_L1,HI_DSID_TYPE_QRY,1,sizeof(HI_DSID_DRV_ETH_INFO_S),&g_drv_eth_info},
	{HI_DSID_DRV_HRT_INFO,HI_SYS_DMID_BUSINESS_L1,HI_DSID_TYPE_QRY,1,sizeof(HI_DSID_DRV_HRT_INFO_S),&g_drv_hrt_info},
#endif
};


#define DRV_INIT_STAT_TBL()\
{\
    HI_DIAG_UpdateSysStatCounter(&g_drv_eth_info);\
    HI_DIAG_UpdateSysStatCounter(&g_drv_hrt_info);\
    HI_DIAG_RegisterSysStat(g_stDrvStatObjTbl, HI_ARRAY_COUNT(g_stDrvStatObjTbl));\
}

#endif


