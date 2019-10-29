#include <hi_types.h>
#include <hi_config.h>
#include <hi_platform.h>
#include <hi_hw_id.h>
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


typedef struct
{
    HI_BOOL bReaded;
    HI_U8 pad[3];
    HI_FTM_PRODUCT_TYPE_E enProductType;
}HI_DRV_PRODUCT_TYPE_S;

__attribute__((section(".data"))) HI_DRV_PRODUCT_TYPE_S g_stDrvProductType;


/*获取硬件形态版本*/
HI_FTM_PRODUCT_TYPE_E HI_MDM_GetHwProductTypePrv(HI_VOID)
{

    HI_FTM_PRODUCT_TYPE_E enProductType = (HI_FTM_PRODUCT_TYPE_E)HI_FTM_PRODUCT_TYPE_VALUE_UNKOWN;
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
    return (HI_FTM_PRODUCT_TYPE_E)HI_FTM_PRODUCT_TYPE_VALUE_UNKOWN;
    
}


HI_FTM_PRODUCT_TYPE_E HI_MDM_GetHwProductType(HI_VOID)
{
    if(!g_stDrvProductType.bReaded)
    {
        g_stDrvProductType.enProductType = HI_MDM_GetHwProductTypePrv();
        
        g_stDrvProductType.bReaded=HI_TRUE;
    }
    return g_stDrvProductType.enProductType;
}

HI_U32 HI_MDM_GetHwProductId(HI_OUT HI_FTM_CHIP_TYPE_E* penChipType, HI_OUT HI_FTM_PRODUCT_TYPE_E* penProductType)// TODO:后续删除
{

    if((HI_NULL == penChipType)&&(HI_NULL == penProductType))
    {
        return HI_ERR_INVALID_PARAMETER;
    }

    if(penChipType)
        *penChipType=HI_MDM_GetHwChipType();

    if(penProductType)
        *penProductType=HI_MDM_GetHwProductType();
    
    return HI_ERR_SUCCESS;
}


HI_BOOL HI_DMS_IsTfHardWare(HI_VOID)
{
    HI_FTM_PRODUCT_TYPE_E enProductType = HI_MDM_GetHwProductType();
    if(HI_FTM_PRODUCT_TYPE_VALUE_D44==enProductType)
    {
        return HI_TRUE;
    }
    else
    {
        return HI_FALSE;
    }
    
}

HI_BOOL HI_DMS_IsStaHardWare(HI_VOID)
{
    HI_FTM_PRODUCT_TYPE_E enProductType=HI_MDM_GetHwProductType();
    if ((enProductType >= HI_FTM_PRODUCT_TYPE_VALUE_M101) && (enProductType <= HI_FTM_PRODUCT_TYPE_VALUE_M108))
    {
        return HI_TRUE;
    }
    else
    {
        return HI_FALSE;
    }
}


HI_BOOL HI_DMS_IsCcoHardWare(HI_VOID)
{
    HI_FTM_PRODUCT_TYPE_E enProductType=HI_MDM_GetHwProductType();
    if ((enProductType >= HI_FTM_PRODUCT_TYPE_VALUE_C01) && (enProductType <= HI_FTM_PRODUCT_TYPE_VALUE_C18))
    {
        return HI_TRUE;
    }
    else
    {
        return HI_FALSE;
    }
}


