#ifndef __MRS_FW_SGD_FLASH_H__
#define __MRS_FW_SGD_FLASH_H__
HI_START_HEADER

#define  ID_FLASH_USR_CFG                0x0001
#define  FLASH_USR_CFG_SIZE  64 


typedef struct
{
	HI_U8 aucCode[3];
	HI_U8 reserved[61];
}FLASH_USR_CFG_INFO_S;

HI_U32 mrsSgdFlashRead(HI_U16 usId, HI_OUT HI_PVOID pData, HI_U16 usLen);
	
HI_U32 mrsSgdFlashWrite(HI_U16 usId, HI_PVOID pData, HI_U16 usLen);

HI_END_HEADER
#endif //__MRS_SERVICE_H__

