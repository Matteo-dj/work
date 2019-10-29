#define DRV_DFX_SYS_VAL_DECLARE
#include "drv_dfx_sys_val.h"
#include <dfx_sys.h>
#include <hi_types.h>
#include <drv_dfx.h>
#include <hi_sal.h>
/* BEGIN: Added by wuxianfeng 190206, 2012/9/1   PN:DTS2012083107237*/
#include <hi_driver_internal.h>
/* END:   Added by wuxianfeng 190206, 2012/9/1 */

#if defined(PRODUCT_CFG_SUPPORT_UPG)
/*消息打印*/
HI_VOID  HI_DFX_MSG_UPG_ReadNvErr ( HI_U32 ulNvId,HI_U32 ulErrNo)
{
	HI_DMID_DRV_UPG_ERR_READ_NV_S  temp;
	temp.ulNvId = ulNvId;
	temp.ulErrNo = ulErrNo;
   	(HI_VOID)HI_DIAG_ReportSysMessage(HI_DMID_DRV_UPG_ERR_READ_NV,
								HI_MOD_ID_DRV_UPG,
								HI_MOD_ID_DRV_UPG,
								&temp ,
								sizeof(HI_DMID_DRV_UPG_ERR_READ_NV_S),
								HI_SYS_DMID_BUSINESS_L1);
}

HI_VOID  HI_DFX_MSG_UPG_WriteNvErr ( HI_U32 ulNvId,HI_U32 ulErrNo)
{
	HI_DMID_DRV_UPG_ERR_WRITE_NV_S  temp;
	temp.ulNvId = ulNvId;
	temp.ulErrNo = ulErrNo;
   	(HI_VOID)HI_DIAG_ReportSysMessage(HI_DMID_DRV_UPG_ERR_WRITE_NV,
								HI_MOD_ID_DRV_UPG,
								HI_MOD_ID_DRV_UPG,
								&temp ,
								sizeof(HI_DMID_DRV_UPG_ERR_WRITE_NV_S),
								HI_SYS_DMID_BUSINESS_L1);
}

HI_VOID  HI_DFX_MSG_UPG_ReadFlashErr (HI_U32 ulFlashAddr,HI_S32 ulFlashSize,HI_U32 ulErrNo)
{
	HI_DMID_DRV_UPG_ERR_READ_FLASH_S  temp;
	temp.ulFlashAddr = ulFlashAddr;
	temp.ulFlashSize = ulFlashSize;
	temp.ulErrNo = ulErrNo;
   	(HI_VOID)HI_DIAG_ReportSysMessage(HI_DMID_DRV_UPG_ERR_READ_FLASH,
								HI_MOD_ID_DRV_UPG,
								HI_MOD_ID_DRV_UPG,
								&temp ,
								sizeof(HI_DMID_DRV_UPG_ERR_READ_FLASH_S),
								HI_SYS_DMID_BUSINESS_L1);
}

HI_VOID  HI_DFX_MSG_UPG_WriteFlashErr (HI_U32 ulFlashAddr,HI_S32 ulFlashSize,HI_U32 ulErrNo)
{
	HI_DMID_DRV_UPG_ERR_WRITE_FLASH_S  temp;
	temp.ulFlashAddr = ulFlashAddr;
	temp.ulFlashSize = ulFlashSize;
	temp.ulErrNo = ulErrNo;
   	(HI_VOID)HI_DIAG_ReportSysMessage(HI_DMID_DRV_UPG_ERR_WRITE_FLASH,
								HI_MOD_ID_DRV_UPG,
								HI_MOD_ID_DRV_UPG,
								&temp ,
								sizeof(HI_DMID_DRV_UPG_ERR_WRITE_FLASH_S),
								HI_SYS_DMID_BUSINESS_L1);
}

HI_VOID  HI_DFX_MSG_UPG_StartBusyErr (HI_U32  ulStartTimes,HI_U32 ulStatus)
{
	HI_DMID_DRV_UPG_ERR_BUSY_S  temp;
	temp.ulStartTimes =  ulStartTimes;
	temp.ulStatus = ulStatus;
   	(HI_VOID)HI_DIAG_ReportSysMessage(HI_DMID_DRV_UPG_ERR_BUSY,
								HI_MOD_ID_DRV_UPG,
								HI_MOD_ID_DRV_UPG,
								&temp ,
								sizeof(HI_DMID_DRV_UPG_ERR_BUSY_S),
								HI_SYS_DMID_BUSINESS_L0);
}



HI_VOID  HI_DFX_MSG_UPG_StartParaErr (HI_UPG_START_S *pstStart )
{
	(HI_VOID)HI_DIAG_ReportSysMessage(HI_DMID_DRV_UPG_ERR_START_PARA,
								HI_MOD_ID_DRV_UPG,
								HI_MOD_ID_DRV_UPG,
								(HI_DMID_DRV_UPG_ERR_START_PARA_S *)pstStart ,
								sizeof(HI_DMID_DRV_UPG_ERR_START_PARA_S),
								HI_SYS_DMID_BUSINESS_L0);
}

HI_VOID  HI_DFX_MSG_UPG_UpdateParaErr (
												HI_U32 ulStartUpgId,
												HI_U32 ulUpgId,
												HI_U16 usBlkIdx,
												HI_U16 usBlkSize,
												HI_U8* pucFileBlk )
{
	HI_DMID_DRV_UPG_ERR_UPDATE_PARA_S temp;
	temp.ulStartUpgId = ulStartUpgId;
	temp.ulUpgId = ulUpgId;
	temp.usBlkIdx = usBlkIdx;
	temp.usBlkSize = usBlkSize;
	temp.pucFileBlk = pucFileBlk;
	(HI_VOID)HI_DIAG_ReportSysMessage(HI_DMID_DRV_UPG_ERR_UPDATE_PARA,
								HI_MOD_ID_DRV_UPG,
								HI_MOD_ID_DRV_UPG,
								&temp,
								sizeof(HI_DMID_DRV_UPG_ERR_UPDATE_PARA_S),
								HI_SYS_DMID_BUSINESS_L0);
}

HI_VOID  HI_DFX_MSG_UPG_UpdateVerErr ( HI_U32 ulMagic,
												HI_UPG_FILE_VER_S  ulRunTimeFileVer,
    												HI_UPG_FILE_VER_S  ulBackUpFileVer )
{
	HI_DMID_DRV_UPG_ERR_UPDATE_VERSION_S  temp;
    
	temp.ulMagic = ulMagic;
    (hi_void)memcpy_s(&(temp.ulBackUpFileVer), sizeof(HI_UPG_FILE_VER_S), &ulBackUpFileVer, sizeof(HI_UPG_FILE_VER_S));
    (hi_void)memcpy_s(&(temp.ulRunTimeFileVer), sizeof(HI_UPG_FILE_VER_S), &ulRunTimeFileVer, sizeof(HI_UPG_FILE_VER_S));
	(HI_VOID)HI_DIAG_ReportSysMessage(HI_DMID_DRV_UPG_ERR_UPDATE_VERSION,
								HI_MOD_ID_DRV_UPG,
								HI_MOD_ID_DRV_UPG,
								&temp,
								sizeof(HI_DMID_DRV_UPG_ERR_UPDATE_VERSION_S),
								HI_SYS_DMID_BUSINESS_L0);
}

HI_VOID  HI_DFX_MSG_UPG_UpdateCrcErr (HI_U32 ulNewCrcData,HI_U32 ulFileCrcData )
{
	HI_DMID_DRV_UPG_ERR_UPDATE_CRC_S  temp;
	temp.ulNewCrcData = ulNewCrcData;
	temp.ulFileCrcData = ulFileCrcData;
	(HI_VOID)HI_DIAG_ReportSysMessage(HI_DMID_DRV_UPG_ERR_UPDATE_CRC,
								HI_MOD_ID_DRV_UPG,
								HI_MOD_ID_DRV_UPG,
								&temp,
								sizeof(HI_DMID_DRV_UPG_ERR_UPDATE_CRC_S),
								HI_SYS_DMID_BUSINESS_L0);
}
#endif
/* BEGIN: Added by wuxianfeng 190206, 2012/9/1   PN:DTS2012083107237*/
HI_VOID  HI_DFX_MSG_FLASH_Probe (HI_BYTE* ucId, HI_CHAR* pname )
{
	HI_DMID_DRV_FLASH_PROBE_S  temp;
	int i;
	for (i=0; i<3; i++)
	{
		temp.ucId[i] = *ucId;
		ucId ++;
	}
	for (i=0; i<10; i++)
	{	 
		temp.cName[i] = *pname;
		pname ++;
	}

	(HI_VOID)HI_DIAG_ReportSysMessage(HI_DMID_DRV_FLASH_PROBE,
								HI_MOD_ID_DRV_FLASH,
								HI_MOD_ID_DRV_FLASH,
								&temp,
								sizeof(HI_DMID_DRV_FLASH_PROBE_S),
								HI_SYS_DMID_BUSINESS_L1);//不保存至flash
}
  
HI_VOID  HI_DFX_MSG_FLASH_OP (HI_U8 uctype,HI_U32 uladdr,
    HI_U32 ullen )
{
	HI_DMID_DRV_FLASH_OP_S  temp = {0};
	temp.uctype = uctype;
	temp.uladdr = uladdr;
    temp.ullen  = ullen;
	(HI_VOID)HI_DIAG_ReportSysMessage(HI_DMID_DRV_FLASH_OP,
								HI_MOD_ID_DRV_FLASH,
								HI_MOD_ID_DRV_FLASH,
								&temp,
								sizeof(HI_DMID_DRV_FLASH_OP_S),
								HI_SYS_DMID_BUSINESS_L2);
}
 


HI_VOID  HI_DFX_MSG_SPI_TIMEOUT (HI_U32 ulport, HI_U32 uldirection, HI_U32 ultimeout)
{
	HI_DMID_DRV_SPI_TIMEOUT_S  temp = {0};
    
	temp.ulport = ulport;
    temp.uldirection = uldirection;
    temp.ultimeout = ultimeout;
    
	(HI_VOID)HI_DIAG_ReportSysMessage(HI_DMID_DRV_SPI_TIMEOUT,
								HI_MOD_ID_DRV_SPI,
								HI_MOD_ID_DRV_SPI,
								&temp,
								sizeof(HI_DMID_DRV_SPI_TIMEOUT_S),
								HI_SYS_DMID_BUSINESS_L0);//默认保存至flash
}
/* END:   Added by wuxianfeng 190206, 2012/9/1 */


