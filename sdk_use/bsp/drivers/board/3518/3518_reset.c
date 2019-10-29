#include <hi_types.h>
#include <hi_mdm_types.h>


#ifdef PRODUCT_CFG_SUPPORT_3518
#include <hi_ioexp.h>
#include <hi_mdm_task.h>
#include "3518_debug.h"

#define IO_EXP_NUM_RSTN_OUT_PUT             IO_EXP_IO_NUM_10

HI_U32  HI_3518_Reboot (HI_VOID)
{
    HI_U32 ret = HI_ERR_FAILURE;
    static HI_BOOL bFirstCall=HI_TRUE; 

    if(bFirstCall)
    {
        ret = HI_IO_EXP_SetDir((IO_EXP_IO_NUM_E)IO_EXP_NUM_RSTN_OUT_PUT, HI_IO_EXP_DIR_OUT);
        if (ret != HI_ERR_SUCCESS)
        {
            return ret;
        }
        bFirstCall=HI_FALSE;
    }


    
    ret = HI_IO_EXP_SetOutPutVal((IO_EXP_IO_NUM_E)IO_EXP_NUM_RSTN_OUT_PUT,HI_IO_EXP_OUT_VAL_LOW);
    if(ret != HI_ERR_SUCCESS)
    {
        return ret;
    }

    (HI_VOID)HI_MDM_Sleep(20);
    return HI_IO_EXP_SetOutPutVal((IO_EXP_IO_NUM_E)IO_EXP_NUM_RSTN_OUT_PUT,HI_IO_EXP_OUT_VAL_HIGHT);
}

#endif
