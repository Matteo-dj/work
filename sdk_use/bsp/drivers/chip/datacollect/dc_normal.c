#include <hi_types.h>
#include <hi_mdm_types.h>
#ifdef PRODUCT_CFG_COLLECT_DATA

#include <drv_collectdata.h>
#include <hi_mdm_sem.h>
#include <hi_sal.h>
//½Ó¿ÚËø

typedef struct dc_info
{
    HI_U32 pSemId;
    HI_CHAR szName[8];
    HI_BOOL IsInit;
    HI_BOOL pad[3];
}ST_DC_INFO;

ST_DC_INFO g_dc_info_st = {0,};

#define DC_INTERFACE_MUTEX_LOCK()   HI_MDM_SEM_Wait(g_dc_info_st.pSemId,HI_SYS_WAIT_FOREVER)
#define DC_INTERFACE_MUTEX_UNLOCK() HI_MDM_SEM_Signal(g_dc_info_st.pSemId)

HI_U32 HI_DC_Init(HI_VOID)
{
    HI_U32 ret = 0;
    if(g_dc_info_st.IsInit)
    {
        return HI_ERR_FAILURE;
    }
    
    ret = HI_MDM_SEM_BCreate(&(g_dc_info_st.pSemId),g_dc_info_st.szName,HI_SEM_ONE);
    if (ret != HI_ERR_SUCCESS)
        g_dc_info_st.IsInit = HI_FALSE;
    else
        g_dc_info_st.IsInit = HI_TRUE;
    return ret;
}


HI_U32 HI_DC_StartNormalDataCollect(HI_IN DC_INPUT_PARA_STRU * pDcInput,HI_OUT HI_BUF_S * sBuf)
{
    HI_U32 ret = HI_ERR_SUCCESS;
    if(pDcInput==HI_NULL||sBuf==HI_NULL)
    {
        return HI_ERR_INVALID_PARAMETER;
    }

    if (g_dc_info_st.IsInit == HI_FALSE)
        return HI_ERR_INITILIZATION;
    
    (HI_VOID)DC_INTERFACE_MUTEX_LOCK();

    ret = DC_Data_Collect(pDcInput,sBuf,HI_DRV_DATA_COLLECT_STEP_ALL);

    (HI_VOID)DC_INTERFACE_MUTEX_UNLOCK();
    return ret;
}
#endif
