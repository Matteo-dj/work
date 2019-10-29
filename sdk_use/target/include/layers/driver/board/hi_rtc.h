
#ifndef __HI_RTC_H__
#define __HI_RTC_H__

#include <hi_types.h>
#include <hi_mdm_types.h>

#ifdef PRODUCT_CFG_HW_RTC_SUPPORT
HI_U32 HI_RTC_Init(HI_VOID);
HI_U32 HI_RTC_WriteTime(HI_U32 ulTime);
HI_U32 HI_RTC_ReadTime(HI_U32 *pulTime);
#else
HI_PRVL HI_U32 HI_RTC_Init(HI_VOID){return HI_ERR_NOT_SUPPORT;}
HI_PRVL HI_U32 HI_RTC_WriteTime(HI_U32 ulTime){return HI_ERR_NOT_SUPPORT;}
HI_PRVL HI_U32 HI_RTC_ReadTime(HI_U32 *pulTime){return HI_ERR_NOT_SUPPORT;}
#endif//PRODUCT_CFG_HW_RTC_SUPPORT
#endif//__HI_RTC_H__

