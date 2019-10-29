#include <hi_types.h>
#include <hi_mdm_types.h>
#if defined (PRODUCT_CFG_BQ_SUPPORT)
#include <hi_mdm_timer.h>
#include <hi_share_task.h>
#include <hi_battery.h>
#include <hi_driver.h>//i2c
#include <hi_ndm.h>//HI_ND_DEVICE_INFO_IND_S 移动需要考虑是否能够找到对应结构体
#include <hi_sal.h>
#include "battery.h"

HI_U32 g_battery_module_handle;
HI_TIMER_HANDLE_S g_battery_timer_handle;



HI_U32 battery_msg_send(HI_SYS_QUEUE_MSG_S *pMsg)
{
    return HI_SHARE_TASKQ_SendMsg(HI_SHARE_TASK_ID_REFRESH_BQ_INFO,g_battery_module_handle,pMsg);
}

HI_PRV HI_VOID battery_timer_call_back(HI_U32 ulData)
{
    HI_SYS_QUEUE_MSG_S stMsg={0};
    stMsg.ulMsgId=HI_BATTERY_MSG_ID_REFRESH_INFO;
    battery_msg_send(&stMsg);
    if(HI_MDM_GetSeconds()<60)
    {
        HI_MDM_TIMER_Start(&g_battery_timer_handle,battery_timer_call_back,5000,HI_TIMER_TYPE_ONCE,0);
    }
    else
    {
        HI_MDM_TIMER_Start(&g_battery_timer_handle,battery_timer_call_back,60*1000,HI_TIMER_TYPE_ONCE,0);
    }
}

HI_VOID battery_init_proc(HI_VOID)
{
    //I2C初始化
    HI_I2C_Init(HI_I2C_IDX_0);
    battery_init();
    HI_MDM_TIMER_Start(&g_battery_timer_handle,battery_timer_call_back,3000,HI_TIMER_TYPE_ONCE,0);

}

HI_VOID battery_task_body(HI_SYS_QUEUE_MSG_S *pMsg)
{
    switch(pMsg->ulMsgId)
    {
        case HI_BATTERY_MSG_ID_REFRESH_INFO:
        {
            battery_updata_chip_val(HI_FALSE);
            break;
        }
        case HI_BATTERY_MSG_ID_INIT:
        {
            battery_init_proc();
            break;
        }
        default:
            break;
    }
}


HI_U32 HI_BAT_Init(HI_VOID)
{
    HI_U32 ret=HI_ERR_SUCCESS;
    HI_SYS_QUEUE_MSG_S stMsg={0};
    ret=HI_SHARE_TASKQ_Create(HI_SHARE_TASK_ID_REFRESH_BQ_INFO,&g_battery_module_handle,battery_task_body);
    if(ret!=HI_ERR_SUCCESS)
    {
        return ret; 
    }
    stMsg.ulMsgId=HI_BATTERY_MSG_ID_INIT;
    return battery_msg_send(&stMsg);
}


/***********************************************查询接口***************************************************************/
HI_U32 HI_BAT_ReportBatteryPercent(HI_U16 usId, HI_IN HI_PVOID pCmdParam, HI_U16 usCmdParamSize, HI_U32 ulOption)
{
    HI_ND_DEVICE_INFO_IND_S devInfo = {0};

    (hi_void)memset_s((HI_VOID *)&devInfo, sizeof(devInfo), 0x0, sizeof(devInfo));
    HI_U8 ucBatPercent = 0;
    HI_BAT_PERCENT_ERR_E enErrCode = HI_BAT_PERCENT_ERR_NONE;


    (HI_VOID)HI_BAT_GetBatPercent(&ucBatPercent, &enErrCode);
    if(enErrCode==HI_BAT_PERCENT_ERR_INITING)//与HSO确认返回初始化错误时HSO显示电池电量x,并且间隔5s查询一次连续查询5次
    {
        devInfo.ucStatus = HI_ND_DEVICE_INFO_BAT_STATUS_BAT_INIT_ING;
    }
    else if(enErrCode==HI_BAT_PERCENT_ERR_VER_WRONG)
    {
        devInfo.ucStatus = HI_ND_DEVICE_INFO_BAT_STATUS_BAT_NO_FIRMWARE;

    }
    else if(enErrCode==HI_BAT_PERCENT_ERR_NOT_EXIST_BATTERY)
    {
        devInfo.ucStatus = HI_ND_DEVICE_INFO_BAT_STATUS_BAT_NO_EXISTT;

    }
    else
    {
        devInfo.ucStatus = HI_ND_DEVICE_INFO_BAT_STATUS_NORMAL;
        devInfo.ucBatteryPercent = ucBatPercent;

    }
    HI_DIAG_ReportPacket(usId,(HI_U8)ulOption, (HI_PBYTE)(&devInfo), sizeof(HI_ND_DEVICE_INFO_IND_S), HI_TRUE);
    return HI_ERR_SUCCESS;
}
#endif

