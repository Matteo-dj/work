#include <hi_types.h>
#include <hi_mdm_types.h>
#include <hi_mdm_msg.h>
#include <hi_mdm_task.h>
#include "app_ndm_tools.h"
#include "app_ndm_msg.h"
#include "app_ndm_diag.h"

HI_U32 g_MrsNdmMsgQid[2] = {0};

HI_EXTERN HI_U32 APP_NdmTimerMessageProc(HI_SYS_QUEUE_MSG_S *pstMsg);
HI_EXTERN HI_U32 mrsNdmKeyMsgProc(HI_SYS_QUEUE_MSG_S *pstMsg);
HI_EXTERN HI_U32 mrsNvChangeMsgProc(HI_SYS_QUEUE_MSG_S *pstMsg);
HI_EXTERN HI_U32 APP_NdmDiagMsgProc(HI_SYS_QUEUE_MSG_S *pstMsg);
HI_EXTERN HI_U32 APP_BqUpgMsgProc(HI_SYS_QUEUE_MSG_S *pstMsg);
HI_EXTERN HI_U32 APP_DcMsgProc(HI_SYS_QUEUE_MSG_S *pstMsg);
HI_EXTERN HI_U32 APP_SigSendMsgProc(HI_SYS_QUEUE_MSG_S *pstMsg);


HI_U32 APP_NdmMessageWait(HI_U32 ulQueueId, HI_SYS_QUEUE_MSG_S* pstMsg, HI_U32 ulTimeOut)
{
    HI_U32 ret = HI_ERR_SUCCESS;
    ret = HI_MDM_MSG_Wait(ulQueueId, pstMsg, ulTimeOut);

    if (ret != HI_ERR_SUCCESS)
    {
        return ret;
    }

    if (pstMsg->ulMsgId == EN_MRS_FW_MSG_NDM_TIMER)
    {
        APP_NdmTimerMessageProc(pstMsg);
    }
    else if (pstMsg->ulMsgId == EN_MRS_FW_MSG_NDM_KEY)
    {
        mrsNdmKeyMsgProc(pstMsg);
    }
    else if (pstMsg->ulMsgId == EN_MRS_FW_MSG_NV_CHANGE_NOTIFY)
    {
        mrsNvChangeMsgProc(pstMsg);
    }
    else if(pstMsg->ulMsgId == EN_MRS_FW_MSG_NDM_DIAG)//DIAG消息处理模块
    {
        APP_NdmDiagMsgProc(pstMsg);
    }
    else if(pstMsg->ulMsgId == EN_NDM_MSG_BQ_UPG_START)//BQ升级模块
    {
        APP_BqUpgMsgProc(pstMsg);

    }
    else if(pstMsg->ulMsgId == EN_NDM_MSG_DATA_COLLECT)//采数模块
    {
        APP_DcMsgProc(pstMsg);

    }
    else if(pstMsg->ulMsgId == EN_NDM_MSG_SEND_SIGNAL)//信号发送模块
    {
        APP_SigSendMsgProc(pstMsg);

    }

    return HI_ERR_SUCCESS;
}


HI_VOID APP_NdmMessageTaskBody(HI_U32 id)
{
    HI_SYS_QUEUE_MSG_S stMsg={0};
    for(;;)
    {
        APP_NdmMessageWait(g_MrsNdmMsgQid[id],&stMsg,HI_SYS_WAIT_FOREVER);
    }
}

HI_U32 APP_NdmMessageSend(HI_U32 ulQId,HI_U32 ulMsgId, HI_U32 ulParam1, HI_U32 ulParam2, HI_U32 ulParam3, HI_U32 ulParam4)
{
    HI_SYS_QUEUE_MSG_S stMsg = {0};
    stMsg.ulMsgId = ulMsgId;
    stMsg.ulParam[0] = ulParam1;
    stMsg.ulParam[1] = ulParam2;
    stMsg.ulParam[2] = ulParam3;
    stMsg.ulParam[3] = ulParam4;
    HI_U32 ulTimeOut = 0;
#ifdef PRODUCT_CFG_SUPPORT_CCT3911
    if (EN_MRS_FW_MSG_NDM_KEY == ulMsgId)
    {
        ulTimeOut = stMsg.ulParam[2];
    }
#endif
    return HI_MDM_MSG_Send(g_MrsNdmMsgQid[ulQId], &stMsg, ulTimeOut);
}

HI_U32 APP_NdmMessageInit(HI_VOID)
{
    HI_U32 ulTaskid[2] = {0};
    HI_U32 ret = HI_ERR_SUCCESS;
    ret |= HI_MDM_MSG_CreateQueue(&g_MrsNdmMsgQid[0], "ndm1", 64);
    ret |= HI_MDM_MSG_CreateQueue(&g_MrsNdmMsgQid[1], "ndm2", 16);
    ret |= HI_MDM_TASK_Create(&ulTaskid[0], "ndm1", APP_NdmMessageTaskBody, 0, 0x1000, 25);
    ret |= HI_MDM_TASK_Create(&ulTaskid[0], "ndm2", APP_NdmMessageTaskBody, 1, 0x1000, 26);
    return ret;
}
