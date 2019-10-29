

#include "mrs_common.h"
//#include "mrs_cmd_msg.h"
//#include "mrs_fw_tools.h"
//#include "mrs_srv_common.h"
//#include "mrs_srv_res.h"
//#include "app_nv.h"
#include "mrs_fw_sgd_flash.h"

#if defined(PRODUCT_CFG_PRODUCT_TYPE_CCO) || defined(PRODUCT_CFG_PRODUCT_TYPE_STA)
#if defined(PRODUCT_CFG_PRODUCT_TYPE_CCO)
#define FLASH_USR_CFG_ADDR  0x199000
#endif

#if defined(PRODUCT_CFG_PRODUCT_TYPE_STA)
#define FLASH_USR_CFG_ADDR  0xFE000
#endif

HI_U32 mrsSgdFlashRead(HI_U16 usId, HI_OUT HI_PVOID pData, HI_U16 usLen)
{
	HI_U32 ret = HI_ERR_FAILURE;
	HI_U16 len = 0;
	if(usId == ID_FLASH_USR_CFG)
	{
		len = (usLen < FLASH_USR_CFG_SIZE)?usLen:FLASH_USR_CFG_SIZE;
		ret = HI_MDM_FLASH_Read(FLASH_USR_CFG_ADDR, len,HI_NULL, pData);
	}
	return ret;
}

HI_U32 mrsSgdFlashWrite(HI_U16 usId, HI_PVOID pData, HI_U16 usLen)
{
	HI_U32 ret = HI_ERR_FAILURE;
	HI_U16 len = 0;
	if(usId == ID_FLASH_USR_CFG)
	{
		len = (usLen < FLASH_USR_CFG_SIZE)?usLen:FLASH_USR_CFG_SIZE;
		ret = HI_MDM_FLASH_Write(FLASH_USR_CFG_ADDR, len,HI_NULL,pData,HI_TRUE);
	}
	return ret;
}
#endif