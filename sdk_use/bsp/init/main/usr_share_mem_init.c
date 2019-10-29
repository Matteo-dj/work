#include <hi_types.h>
#include <hi_mdm_types.h>
#include <hi_share_memory.h>
#include <hi_hw_id.h>
#include <hi_product_types.h>

HI_U32 usr_share_mem_init(HI_VOID)
{
    //NDM��̬��,�������Ⳮ�����Ƿ������ʼ�����ģ�顣s
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
