
#ifndef __APP_NDM_MSG_H__
#define __APP_NDM_MSG_H__
#include <hi_types.h>
#include <hi_mdm_types.h>

typedef enum
{
    APP_OSA_MSG_Q_ID_0,
    APP_OSA_MSG_Q_ID_1,
    APP_OSA_MSG_Q_ID_MAX,
}APP_OSA_MSG_Q_ID_E;//消息队列ID

typedef enum
{
    EN_MRS_FW_MSG_NDM_DIAG,
    EN_MRS_FW_MSG_NDM_KEY,
    EN_MRS_FW_MSG_NDM_TIMER,
    EN_MRS_FW_MSG_NV_CHANGE_NOTIFY,
    EN_NDM_MSG_BQ_UPG_START,    // EQT: BQ upgrade
    EN_NDM_MSG_DATA_COLLECT,    // Data Collect
    EN_NDM_MSG_SEND_SIGNAL,     // Send Signal
}APP_OSA_MSG_ID_E;//消息ID


HI_U32 APP_NdmMessageInit(HI_VOID);
HI_U32 APP_NdmMessageSend(HI_U32 ulQId,HI_U32 ulMsgId, HI_U32 ulParam1, HI_U32 ulParam2, HI_U32 ulParam3, HI_U32 ulParam4);



//消息队列ID映射
#define MRS_NDM_MSG_Q_0 APP_OSA_MSG_Q_ID_0
#define MRS_NDM_MSG_Q_1 APP_OSA_MSG_Q_ID_1

#endif

