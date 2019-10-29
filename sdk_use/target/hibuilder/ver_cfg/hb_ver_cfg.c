//*****************************************************************************
//
//                  版权所有 (C), 1998-2011, 华为技术有限公司
//
//*****************************************************************************
//  文 件 名   : hb_ver_cfg.c
//  版 本 号   : V1.0
//  作    者   :  chenghuanhuan/00132816
//  生成日期   : 2016-06-07
//  功能描述   :  版本号用户通过HB界面输入，生成头文件 hb_ver_cfg.h。
//				本文件用于链接时调用，用于为单板代码使用的全局变量赋值。
//
//  函数列表   : TODO: ...
//  修改历史   :
//  1.日    期 : 2016-06-07
//    作    者 : chenghuanhuan/00132816
//    修改内容 : 创建文件
//             
//
//*****************************************************************************

#include <hi_types.h>
#include <hb_ver_cfg.h>
#include <hi_mdm_types.h>

#define  PRODUCT_CFG_FILE_VER_MAJOR_MINOR   HI_MAKEU32(PRODUCT_CFG_FILE_VER_MINOR,PRODUCT_CFG_FILE_VER_MAJOR)

HI_CONST HI_PRODUCT_INFO_S g_stPlcModuleProductInfo =
{
    PRODUCT_CFG_VERSION_STR,
    PRODUCT_CFG_FULL_VERSION_STR,
    PRODUCT_CFG_FILE_VERSION_STR,
    PRODUCT_CFG_CHIP_SOLUTION_NAME,
    PRODUCT_CFG_BUILD_DATE,
    PRODUCT_CFG_BUILD_TIME
};
HI_CONST HI_UE_SOFT_VERSION_S g_stSoftVer =
{
    PRODUCT_CFG_FILE_VER_MAJOR_MINOR, // 主版本号.次版本号
    PRODUCT_CFG_FILE_VER_REVISION,   // 修正版本号
    0      // 内部版本号
};
HI_CONST HI_UE_PRODUCT_VER_S g_stProductVer =
{
    PRODUCT_CFG_VERSION_V,           // V部分
    PRODUCT_CFG_VERSION_R,           // R部分
    PRODUCT_CFG_VERSION_C,           // C部分
    PRODUCT_CFG_VERSION_B,           // B部分
    PRODUCT_CFG_VERSION_SPC,            // SPC部分
    0,    // 硬件PCB号和印制板版本号
    0        // 产品类型编号，即不同外设组合的硬件平台
};
 HI_CHAR* creationDate = PRODUCT_CFG_BUILD_DATE;

