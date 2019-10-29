#include <hi_types.h>
#include <hi_mdm_types.h>
#include <hi_mdm_msg.h>
#include <hi_mdm_timer.h>
#include "app_ndm_tools.h"
#include "app_ndm_timer.h"
#include "app_ndm_msg.h"

APP_NDM_TIMER_S g_stAppOsaTimer[APP_OSA_TIMER_ID_MAX];

HI_U32 APP_NdmTimerMessageProc(HI_SYS_QUEUE_MSG_S *pstMsg)
{
    if(pstMsg->ulParam[0] >= APP_OSA_TIMER_ID_MAX||g_stAppOsaTimer[pstMsg->ulParam[0]].pFunc==HI_NULL)
    {
        return HI_ERR_FAILURE;
    }
    g_stAppOsaTimer[pstMsg->ulParam[0]].pFunc();
    return HI_ERR_SUCCESS;
}

HI_VOID APP_NdmTimerCallBack(HI_U32 ulTimeId)
{
    if (ulTimeId < APP_OSA_TIMER_ID_MAX)
    {
        (HI_VOID)APP_NdmMessageSend(MRS_NDM_MSG_Q_0,EN_MRS_FW_MSG_NDM_TIMER,ulTimeId,0,0,0);
    }
    return;
}

HI_U32 APP_NdmTimerStart(HI_U8 ucTimerId,HI_U32 ulPeriod, HI_U8 ucOption)
{
    if (ucTimerId >= APP_OSA_TIMER_ID_MAX)
    {
        return HI_ERR_INVALID_ID;
    }
    return HI_MDM_TIMER_Start(&g_stAppOsaTimer[ucTimerId].handle, APP_NdmTimerCallBack, ulPeriod, (HI_TIMER_TYPE_E)ucOption, ucTimerId);
}


HI_U32 APP_NdmTimerStop(HI_U8 ucTimerId)
{
    if (ucTimerId >= APP_OSA_TIMER_ID_MAX)
    {
        return HI_ERR_INVALID_ID;
    }
    return HI_MDM_TIMER_Stop(&g_stAppOsaTimer[ucTimerId].handle);
}


