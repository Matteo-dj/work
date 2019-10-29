#include <hi_types.h>
#include <hi_mdm_types.h>
#include <hi_share_memory.h>
#include <hi_hw_id.h>
#include <hi_product_types.h>

HI_U32 usr_share_mem_init(HI_VOID)
{
    //NDM形态上,非整表检测抄控器非发射机初始化电池模块。s
#ifdef PRODUCT_CFG_PRODUCT_TYPE_NDM
    HI_BOOL isMeterCheck=HI_MDM_IsMeterCheckProduct();
    HI_BOOL isTfhw=HI_DMS_IsTfHardWare();
    if (!isMeterCheck && !isTfhw)
    {
        return HI_SHARE_MEM_Init();
    }
    return HI_ERR_SUCCESS;
#else
    return HI_SHARE_MEM_Init();
#endif

}
