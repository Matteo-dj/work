#include <hi_types.h>
#include <hi_mdm_types.h>

#ifdef PRODUCT_CFG_SUPPORT_3518
#include <hi_icc.h>
#include <hi_mdm_time.h>
#include <hi_share_task.h>
#include <icc_protocal_upg.h>
#include <hi_mdm_timer.h>
#include <hi_stdlib.h>
#include <hi_board_slave_3518.h>
#include "3518_debug.h"
#include <hi_3518.h>

HI_U32 g_3518_ver_task_handle;
HI_TIMER_HANDLE_S g_3518_ver_timer;
typedef struct
{
    HI_3518_UPG_VER_S ver;
    HI_BOOL bReadeded;
    HI_BOOL bInit;
    HI_BOOL reserver[2];
}HI_3518UPG_CTRL_S;

HI_3518UPG_CTRL_S g_3518_upg_ctrl;

typedef enum
{
    HI_3518_VER_MSG_ID_READ_VER,
}HI_3518_VER_MSG_ID_E;


HI_PRV HI_VOID ver_3518_monitor_call_back(HI_U32 ulData);

HI_PRV HI_U32 ver_3518_cmd(HI_3518_UPG_VER_S * pVer)
{
    HI_U32 ret = HI_ERR_SUCCESS;
    HI_S32 socket = HI_SYS_INVALID_SOCKET;

    HI_ICC_IND_PKT_S stIccInd = {0};
    HI_3518_GET_VER_CMD_S stUsrCmd = {0};
    HI_3518_GET_VER_IND_S stUsrInd;
    HI_ICC_CMD_DATA_BUF_S stCmdBufs={0};
    HI_3518_UPG_PrintDebug("[in][%s %u]\r\n",__func__,__LINE__);
    if(pVer==HI_NULL)
    {
        return HI_ERR_INVALID_PARAMETER;
    }

    //锁通道
    if(HI_ERR_SUCCESS != ICC_CHANNEL_LOCK(ICC_USR_CHANNEL_ID_UPG,HI_SYS_WAIT_FOREVER))
    {
        HI_3518_UPG_PrintErr("[ERR][%s %u]\r\n",__func__,__LINE__);
        return HI_ERR_WAIT_SEM_FAIL;
    }

    socket = HI_ICC_SocketCreate(ICC_USR_CHANNEL_ID_UPG,2,2);
    if(socket == HI_SYS_INVALID_SOCKET)
    {
        ret = HI_ERR_ICC_SOCKET_CONNECT;
        HI_3518_UPG_PrintErr("[ERR][%s %u]\r\n",__func__,__LINE__);
        goto resume1;
    }

    stCmdBufs.ulBufCount=1;
    stCmdBufs.pBuf[0]=(HI_PBYTE)&stUsrCmd;
    stCmdBufs.ulBufLen[0]=sizeof(stUsrCmd);

    //发送命令
    ret=HI_ICC_SendCmdHeadData(socket,HI_ICC_CMD_UPG_GET_VER,sizeof(stUsrCmd),&stCmdBufs);
    if(HI_ERR_SUCCESS != ret)
    {
        HI_3518_UPG_PrintErr("[ERR][%s %u]\r\n",__func__,__LINE__);
        goto resume2;
    }



    //接收系统IND
    ret = HI_ICC_Receive(socket,(HI_PBYTE)&stIccInd,sizeof(stIccInd));
    if(HI_ERR_SUCCESS != ret)
    {
        HI_3518_UPG_PrintErr("[ERR][%s %u]\r\n",__func__,__LINE__);
        goto resume2;
    }
    if(stIccInd.ret_sys!=HI_ERR_SUCCESS)
    {
        ret  = stIccInd.ret_sys;
        HI_3518_UPG_PrintErr("[ERR][%s %u]\r\n",__func__,__LINE__);
        goto resume2;
    }

    //接收用户IND
    (hi_void)memset_s(&stUsrInd, sizeof(stUsrInd), 0, sizeof(stUsrInd));
    ret = HI_ICC_Receive(socket,(HI_PBYTE)&stUsrInd,sizeof(stUsrInd));
    if(HI_ERR_SUCCESS != ret)
    {
        HI_3518_UPG_PrintErr("[ERR][%s %u]\r\n",__func__,__LINE__);
        goto resume2;

    }
    (hi_void)memcpy_s(pVer,sizeof(HI_3518_UPG_VER_S),&stUsrInd.ver,sizeof(HI_3518_UPG_VER_S));
//END:
    ret = HI_ERR_SUCCESS;
resume2:
    HI_ICC_SocketClose(socket);
resume1:
    (HI_VOID)ICC_CHANNEL_UNLOCK(ICC_USR_CHANNEL_ID_UPG);
    return ret;
}

HI_PRV HI_VOID ver_3518_refresh_ver(HI_VOID)
{
    HI_3518_UPG_VER_S stVer={0};
    
    if(HI_TRUE == HI_ICC_GetConnectStat(ICC_USR_CHANNEL_ID_UPG))
    {
        if(HI_ERR_SUCCESS==ver_3518_cmd(&stVer))
        {
            (hi_void)memcpy_s(&(g_3518_upg_ctrl.ver),sizeof(HI_3518_UPG_VER_S),&stVer,sizeof(HI_3518_UPG_VER_S));
            if(g_3518_upg_ctrl.ver.app!=PRODUCT_CFG_3518_APP_VER)
            {
                HI_BOARD_3518VerErr();//版本不正确告警
            }
            g_3518_upg_ctrl.bInit=HI_TRUE;
            g_3518_upg_ctrl.bReadeded=HI_TRUE;
            return;
        }      
    }

    //超过100秒未读取到版本号
    if(HI_MDM_GetSeconds()>=30 && g_3518_upg_ctrl.bInit==HI_FALSE)
    {
        g_3518_upg_ctrl.bInit=HI_TRUE;
        HI_BOARD_3518VerErr();//版本不正确告警
    }

    //初始化完成,不在启动定时器读取版本号。
    if(g_3518_upg_ctrl.bInit==HI_FALSE)
    {
        (HI_VOID)HI_MDM_TIMER_Start(&g_3518_ver_timer,ver_3518_monitor_call_back,5000,HI_TIMER_TYPE_ONCE,0);
    }

    
}

HI_PRV HI_VOID ver_3518_task_body(HI_SYS_QUEUE_MSG_S *pMsg)
{
    switch(pMsg->ulMsgId)
    {
        case HI_3518_VER_MSG_ID_READ_VER:
        {
            ver_3518_refresh_ver();
            break;
        }
        default:
            break;
    }
}

HI_PRV HI_VOID ver_3518_monitor_call_back(HI_U32 ulData)
{
    HI_SYS_QUEUE_MSG_S stMsg={0};
    stMsg.ulMsgId=HI_3518_VER_MSG_ID_READ_VER;
    HI_SHARE_TASKQ_SendMsg(HI_SHARE_TASK_ID_REFRESH_3518_VER,g_3518_ver_task_handle,&stMsg);        
}



//客户端相关初始化
HI_U32 HI_3518_VerInit(HI_VOID)
{
    HI_U32 ret =HI_ERR_SUCCESS;

    //创建ICC 任务
    ret=HI_SHARE_TASKQ_Create(HI_SHARE_TASK_ID_REFRESH_3518_VER,&g_3518_ver_task_handle,ver_3518_task_body);
    if(ret!=HI_ERR_SUCCESS)
    {
        return ret;
    }

    //创建ICC 定时器
    ret=HI_MDM_TIMER_Start(&g_3518_ver_timer,ver_3518_monitor_call_back,5000,HI_TIMER_TYPE_ONCE,0);
    return ret;
}


HI_U32 HI_3518_GetVer(HI_3518_UPG_VER_S *pVer)
{
    if(pVer==HI_NULL)
    {
        return HI_ERR_INVALID_PARAMETER;
    }
    if(g_3518_upg_ctrl.bInit==HI_FALSE)
    {
        return HI_ERR_INITILIZATION;
    }
    if(g_3518_upg_ctrl.bReadeded==HI_FALSE)
    {
        return HI_ERR_FAILURE;
    }
    (hi_void)memcpy_s(pVer,sizeof(HI_3518_UPG_VER_S),&(g_3518_upg_ctrl.ver),sizeof(HI_3518_UPG_VER_S));
    return HI_ERR_SUCCESS;
}

#endif

