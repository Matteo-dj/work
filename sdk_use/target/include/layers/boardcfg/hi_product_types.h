
#ifndef __HI_PRODUCT_TYPES_H__
#define __HI_PRODUCT_TYPES_H__
#include <hi_types.h>
#include <hi_mdm_types.h>
#include <hi_ft_nv.h>

HI_START_HEADER

#if !defined(PRODUCT_CFG_BUILD_HSO_CODE)
//*****************************************************************************
// 函数名称: HI_MDM_GetProductType
// 功能描述: 获取模块的产品形态
//
// 参数说明:
//
//
// 返 回 值:
//         参照 hi_ft_nv.h中定义
//
// 调用要求: 产品形态来源于0xF03NV项的enProductType成员。
//*****************************************************************************
HI_EAPI HI_FTM_PRODUCT_TYPE_E HI_MDM_GetProductType(HI_VOID);

//*****************************************************************************
// 函数名称: HI_MDM_GetHwType
// 功能描述: 获取硬件平台类型
//
// 参数说明:
//
// 返 回 值:
//          参照 hi_ft_nv.h中定义
//
// 调用要求:
//
//*****************************************************************************
HI_EAPI HI_FTM_HW_PLATFORM_TYPE_E HI_MDM_GetHwType(HI_VOID);

//*****************************************************************************
// 函数名称: HI_MDM_IsProductXXX
// 功能描述: 判断产品形态是否是XXX
//                      HI_MDM_IsProductCon     是否为集中器载波模块
//                      HI_MDM_IsProductRelay       是否为中继器
//                      HI_MDM_IsProductMeter       是否为电表
//                      HI_MDM_IsProductMeterI      是否为I型采集器
//                      HI_MDM_IsProductTSta    是否为三相表模块
//                      HI_MDM_IsProductMeterII          II型采集器
//                      HI_MDM_IsProductNdm     是否为抄控器
//
// 参数说明:
//      enProductType[out]产品形态
//
// 返 回 值:
//          HI_TRUE 是
//          HI_FALSE 不是
//
// 调用要求:
//
//*****************************************************************************
HI_EAPI HI_BOOL HI_MDM_IsMeterCheckProduct(HI_VOID);
HI_EAPI HI_BOOL HI_MDM_IsProductCon(HI_FTM_PRODUCT_TYPE_E enProductType);       // 是否为集中器载波模块
HI_EAPI HI_BOOL HI_MDM_IsProductRelay(HI_FTM_PRODUCT_TYPE_E enProductType);     // 是否为中继器
HI_EAPI HI_BOOL HI_MDM_IsProductMeter(HI_FTM_PRODUCT_TYPE_E enProductType);     // 是否为电表
HI_EAPI HI_BOOL HI_MDM_IsProductMeterI(HI_FTM_PRODUCT_TYPE_E enProductType);        // 是否为I型采集器
HI_EAPI HI_BOOL HI_MDM_IsProductTSta(HI_FTM_PRODUCT_TYPE_E enProductType);      // 判断是否为三相表模块
HI_EAPI HI_BOOL HI_MDM_IsProductMeterII(HI_FTM_PRODUCT_TYPE_E enProductType);   // 是否为 II型采集器
HI_EAPI HI_BOOL HI_MDM_IsProductNdm(HI_FTM_PRODUCT_TYPE_E enProductType);       // 是否为抄控器

//*****************************************************************************
// 函数名称: HI_MDM_IsInvalidProductType
// 功能描述: 判断产品形态是否不合法。
//
// 参数说明:
//      enProductType[out]产品形态
//
// 返 回 值:
//      HI_TRUE 是
//      HI_FALSE 不是
//
// 调用要求:
//
//*****************************************************************************
HI_EAPI HI_BOOL HI_MDM_IsInvalidProductType(HI_FTM_PRODUCT_TYPE_E enProductType);

HI_BOOL HI_MDM_IsUstaProduct(HI_VOID);
#endif

HI_END_HEADER
#endif
