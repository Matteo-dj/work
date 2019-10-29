
#ifndef __HI_HW_ID_H__
#define __HI_HW_ID_H__
#include <hi_types.h>
#include <hi_mdm_types.h>
#include <hi_ft_nv.h>//�������,HI_FTM_PRODUCT_TYPE_E������hi_drv_hw_id.h�и�����

HI_START_HEADER

HI_EXTERN HI_FTM_PRODUCT_TYPE_E HI_MDM_GetHwProductType(HI_VOID);
HI_EXTERN HI_U32 HI_MDM_GetHwProductId(HI_OUT HI_FTM_CHIP_TYPE_E* penChipType, HI_OUT HI_FTM_PRODUCT_TYPE_E* penProductType);
HI_EXTERN HI_BOOL HI_DMS_IsTfHardWare(HI_VOID);
HI_EXTERN HI_BOOL HI_DMS_IsStaHardWare(HI_VOID);
HI_EXTERN HI_BOOL HI_DMS_IsCcoHardWare(HI_VOID);

HI_END_HEADER

#endif

