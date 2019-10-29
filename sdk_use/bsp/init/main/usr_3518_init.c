#include <hi_types.h>
#include <hi_mdm_types.h>
#include <hi_cmn_nv.h>
#include <hi_icc.h>
#include <hi_filemanager.h>
#include <hi_3518.h>
#include <hi_product_types.h>
#include <hi_mdm_nv.h>

HI_U32 usr_3518_init(HI_VOID)
{
    
    //NDM形态上,非整表检测抄控器初始化3518相关期间
#ifdef PRODUCT_CFG_PRODUCT_TYPE_NDM
    HI_U32 ret = HI_ERR_SUCCESS;
    HI_BOOL isMeterCheck=HI_MDM_IsMeterCheckProduct();
    HI_NV_CMN_CFG_ID_S stCmnNvCfg = {{0}};
    HI_U8 aucDefaultIp[4]={172,16,192,254};
    if(isMeterCheck==HI_FALSE)
    {
        if(HI_ERR_SUCCESS==HI_MDM_NV_Read(HI_NV_CMN_CFG_ID, &stCmnNvCfg, sizeof(HI_NV_CMN_CFG_ID_S)))
        {
            ret|=HI_ICC_Init(stCmnNvCfg.ucTargetIp); 
        }
        else
        {
            ret|=HI_ICC_Init(aucDefaultIp); 
        }
        
        ret|=HI_FMGR_Init();
        ret|=HI_3518_VerInit();

    }
    return ret;
#else
    return HI_ERR_SUCCESS;
#endif
}


