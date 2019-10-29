//*****************************************************************************
//
//                  版权所有 (C), 1998-2011, 华为技术有限公司
//
//*****************************************************************************
//  文 件 名   : app_product.c
//  版 本 号   : V1.0
//  作    者   : chenghuanhuan/00375526
//  生成日期   : 2017-8-27
//  功能描述   : 本文用于管理不同应用基于不同硬件单板设计的产品形态。
//
//  函数列表   : TODO: ...
//  修改历史   :
//  1.日    期 : 2017-8-27
//    作    者 : chenghuanhuan/00375526
//    修改内容 : 创建文件
//
//*****************************************************************************
#include <hi_types.h>
#include <hi_ft_nv.h>
#include <hi_platform.h>
#include <hi_mdm_sys.h>

#define PROD_CFG(b10,b9,b8,b7,b6,b5,b4,b3,b2,b1,b0, p) {HI_U16_BIT_VAL(0,0,b10,b9,b8,b7,b6,b5,b4,b3,b2,b1,b0), p},
#define SAL_PROD_MASK 0x7FF
#define VALID_PROD_CCO_TYPE(x)   do { HI_CLR_BIT_I(x,1); HI_CLR_BIT_I(x,3); HI_CLR_BIT_I(x,4); HI_CLR_BIT_I(x,7); HI_CLR_BIT_I(x,9);                    x&=SAL_PROD_MASK;}while(0)
#define VALID_PROD_STA_TYPE(x)   do { HI_CLR_BIT_I(x,1); HI_CLR_BIT_I(x,4); HI_CLR_BIT_I(x,5); HI_CLR_BIT_I(x,6); HI_CLR_BIT_I(x,7); HI_CLR_BIT_I(x,9); x&=SAL_PROD_MASK;}while(0)


HI_PRV HI_CONST HI_U16 g_stCcoProdType[][2] =
{
# include  "sal_chip_cco_ver_cfg.h"
};

HI_PRV HI_CONST HI_U16 g_stStaProdType[][2] =
{
# include  "sal_chip_sta_ver_cfg.h"
};

/*获取硬件形态版本*/
HI_FTM_PRODUCT_TYPE_E HI_MDM_GetHwProductType(HI_VOID)
{

    HI_FTM_PRODUCT_TYPE_E enProductType = HI_FTM_PRODUCT_TYPE_UNKOWN;
    HI_FTM_CHIP_TYPE_E enChipType = HI_FTM_CHIP_TYPE_UNKOWN;
    HI_U32 ulProVer=0;
    HI_U32 i=0;
    enChipType=HI_MDM_GetHwChipType();

    HI_REG_READ32(HI_SYSCTRL_REG_BASE + SC_HW_ID, ulProVer);


    if(enChipType == HI_FTM_CHIP_TYPE_3911C)
    {
        for(i = 0; i < HI_ARRAY_COUNT(g_stCcoProdType); i++)
        {
            VALID_PROD_CCO_TYPE(ulProVer);
            if(g_stCcoProdType[i][0] == ulProVer)
            {
                enProductType=(HI_FTM_PRODUCT_TYPE_E)g_stCcoProdType[i][1];
                return enProductType;
            }
        }
    }

    // Check STA chip product type
    else if(enChipType == HI_FTM_CHIP_TYPE_3911T)
    {
        for(i = 0; i < HI_ARRAY_COUNT(g_stStaProdType); i++)
        {
            VALID_PROD_STA_TYPE(ulProVer);
            if(g_stStaProdType[i][0] == ulProVer)
            {
                enProductType=(HI_FTM_PRODUCT_TYPE_E)g_stStaProdType[i][1];
                return enProductType;
            }
        }
    }
    return HI_FTM_PRODUCT_TYPE_UNKOWN;
    
}
