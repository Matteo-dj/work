#include <hi_types.h>
#include <hi_mdm_types.h>
#include <hi_mdm_datacollect.h>

HI_U32 usr_dc_init(HI_VOID)
{
    //所有形态上初始化采数驱动
    return HI_DC_Init();
}
