#include <hi_types.h>
#include <hi_mdm_types.h>

#if defined (PORDUCT_CFG_SUPPORT_ICC)
#include <hi_icc.h>
#include <hi_errno.h>
#include <sal_inf.h>
#include <hi_uart_print_debug.h>
#include <hi_period_event.h>
#include <hi_share_task.h>
#include "icc_debug.h"
#include "icc.h"

HI_ICC_CTRL_S g_icc_ctrl;
HI_U32 g_icc_task_handle;
HI_TIMER_HANDLE_S g_icc_timer;
HI_BOOL g_icc_is_init;

typedef enum
{
    HI_ICC_MSG_ID_CHECK_CONNECT_STATUS,
}HI_ICC_MSG_ID_E;





HI_PRV HI_VOID icc_task_body(HI_SYS_QUEUE_MSG_S *pMsg)
{
    switch(pMsg->ulMsgId)
    {
        case HI_ICC_MSG_ID_CHECK_CONNECT_STATUS:
        {
            icc_check_connect_status();
            break;
        }
        default:
            break;
    }
}


HI_PRV HI_VOID icc_timer_call_back(HI_U32 ulData)
{
    HI_SYS_QUEUE_MSG_S stMsg={0};
    stMsg.ulMsgId=HI_ICC_MSG_ID_CHECK_CONNECT_STATUS;
    HI_SHARE_TASKQ_SendMsg(HI_SHARE_TASK_ID_REFRESH_ICC_CONNECT_STAT,g_icc_task_handle,&stMsg); 
    if(HI_MDM_GetSeconds()>100)
    {
        HI_MDM_TIMER_Start(&g_icc_timer,icc_timer_call_back,60*1000,HI_TIMER_TYPE_ONCE,0);
    }
    else
    {
        HI_MDM_TIMER_Start(&g_icc_timer,icc_timer_call_back,5000,HI_TIMER_TYPE_ONCE,0);
    }
}



//客户端相关初始化
HI_U32 icc_monitor_init(HI_VOID)
{
    HI_U32 ret =HI_ERR_SUCCESS;

    //创建ICC 任务
    ret=HI_SHARE_TASKQ_Create(HI_SHARE_TASK_ID_REFRESH_ICC_CONNECT_STAT,&g_icc_task_handle,icc_task_body);
    if(ret!=HI_ERR_SUCCESS)
    {
        return ret;
    }

    //创建ICC 定时器
    ret=HI_MDM_TIMER_Start(&g_icc_timer,icc_timer_call_back,5000,HI_TIMER_TYPE_ONCE,0);
    return ret;
}

//早期初始化
HI_U32 HI_ICC_Init(HI_U8 aucClintIp[4])
{
    HI_U32 ret = HI_ERR_SUCCESS;
    ICC_CLIENT_CHANNEL_S *pChannel = HI_NULL;
    HI_U32 ulTempSemId;

    (hi_void)memset_s(&g_icc_ctrl,sizeof(HI_ICC_CTRL_S), 0,sizeof(HI_ICC_CTRL_S));
    
    HI_ICC_CTRL_S *pIccCtx = ICC_GET_CTRL_CTX();

    (hi_void)snprintf_s(pIccCtx->ucTargetIp,HI_IPV4_STR_WITH_ENDDING_CODE_MAX_LEN,HI_IPV4_STR_WITH_ENDDING_CODE_MAX_LEN - 1,"%u.%u.%u.%u"
        ,aucClintIp[0],aucClintIp[1],aucClintIp[2],aucClintIp[3]);

    
    ret=HI_MDM_SEM_BCreate(&ulTempSemId,"icc",1);
    if(ret!=HI_ERR_SUCCESS)
    {
        return ret;
    }
    
    pChannel = &(pIccCtx->channel[ICC_CHANNEL_ID_20000]);
    pChannel->bConnect = HI_FALSE;
    pChannel->bClient = HI_TRUE;
    pChannel->bServer = HI_FALSE;
    pChannel->phyPortNum = HI_ICC_PORT_NUM_20000;
    pChannel->ulSemId = ulTempSemId;

    pChannel = &(pIccCtx->channel[ICC_CHANNEL_ID_20001]);
    pChannel->bConnect = HI_FALSE;
    pChannel->bClient = HI_TRUE;
    pChannel->bServer = HI_FALSE;
    pChannel->phyPortNum = HI_ICC_PORT_NUM_20001;
    pChannel->ulSemId = ulTempSemId;

    ret=icc_monitor_init();
    if(ret!=HI_ERR_SUCCESS)
    {
        return ret;
    }
    
    g_icc_is_init=HI_TRUE;
    return HI_ERR_SUCCESS;
}


#endif

