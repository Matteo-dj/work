//****************************************************************************
//
//                  版权所有 (C), 2001-2011, 华为技术有限公司
//
//****************************************************************************
//  文 件 名   : file_manager_client.c
//  版 本 号   : V1.0
//  作    者   : 边海文/00208046
//  生成日期   : 2015/02/05
//  功能描述   : 用于访问3518单板进行文件管理
//  函数列表   :
//  修改历史   :
//****************************************************************************

#include <hi_types.h>
#include <hi_mdm_types.h>
#if defined (PRODUCT_CFG_ICC_FMGR_SUPPORT)
#include <icc_protocal_filemanager.h>
#include <hi_filemanager.h>
#include <hi_sal.h>
#include <sal_inf.h>
#include <hi_uart_print_debug.h>
#include <os_vx_sal_api.h>
#include <hi_icc.h>

#define HI_FMGR_TRY_CONNECT_INTERVAL 3000//ms
#define GET_MAX_NAME_LEN() 100

//支持的根目录
#define STR_NONE "\0"
#define SD_CARD_BASE_ADDR "/mnt/sdcard" //拼接上用户根目录/sd_dc
//支持的命令类型
#define STR_CMD_MKDIR_NORMAL "mkdir "
#define STR_CMD_MKDIR_PARENTS "mkdir -p "
#define HI_FMGR_DEFAULT_RCV_TIME_OUT_SEC 5
#define HI_FMGR_DEFAULT_CON_TIME_OUT_SEC 5

typedef struct
{
    HI_BOOL bInit;
    HI_U8 pad[3];
}HI_FMGR_CLIENT_CTRL_S;
HI_FMGR_CLIENT_CTRL_S g_fmgr_clinet_ctrl;


HI_PRV HI_PVOID HI_FMGR_MakeLegalPathName(HI_IN const char *usr_path_name,HI_U32 sys_ctrl,HI_U32 *ret)
{
    char *new_name = HI_NULL;
    HI_U32 new_name_len = 0;
    char *base_path_name = HI_NULL;
    char *cmd_name = HI_NULL;
    HI_U32 baseNameType = sys_ctrl&HI_FMGR_PATH_TYPE_MASK;
    HI_U32 cmdNameType =  sys_ctrl&HI_FMGR_CMD_TYPE_MASK;
    
    HI_FMGR_PrintDebug("[IN][%s %u]\r\n",__func__,__LINE__);
	if(usr_path_name==HI_NULL)
	{
	    *ret = HI_ERR_INVALID_PARAMETER;
		return HI_NULL;
	}
	if(usr_path_name[0]!='/')//暂不支持相对路径操作
	{
        *ret = HI_ERR_INVALID_PARAMETER;
		return HI_NULL;
	}
    //TODO:判断文件长度


    switch(baseNameType)
    {
        case HI_FMGR_PATH_TYPE_SD_ROOT:
            base_path_name = STR_NONE;
            break;
        case HI_FMGR_PATH_TYPE_SD_CARD:
            base_path_name = SD_CARD_BASE_ADDR;
            break;
        default:
            *ret = HI_ERR_INVALID_PARAMETER;
            return HI_NULL;
    }

    switch(cmdNameType)
    {
        case HI_FMGR_CMD_NONE:
            cmd_name = STR_NONE;
            break;
        case HI_FMGR_CMD_MK_DIR_NORMAL:
            cmd_name = STR_CMD_MKDIR_NORMAL;
            break;
        case HI_FMGR_CMD_MK_DIR_PARENTS:
            cmd_name = STR_CMD_MKDIR_PARENTS;
            break;
        default:
            *ret = HI_ERR_INVALID_PARAMETER;
            return HI_NULL;
    }

    new_name_len = (HI_U32)strlen(usr_path_name) + (HI_U32)strlen(base_path_name) + (HI_U32)strlen(cmd_name) + 1;
    new_name =  (HI_CHAR *)HI_MDM_Malloc(HI_MOD_ID_SAL_ICC, new_name_len);
    if(new_name==HI_NULL)
    {
        *ret = HI_ERR_NOT_ENOUGH_MEMORY;
        return HI_NULL;
    }

    (hi_void)sprintf_s(new_name, new_name_len, "%s%s%s",cmd_name,base_path_name,usr_path_name);

    *ret = HI_ERR_SUCCESS;

    HI_FMGR_PrintDebug("[OUT][%s %u][usr_path_name=%s][sys_ctrl=0x%x][new_name=%s][cmd_name=%s][base_path_name=%s]\r\n"
        ,__func__,__LINE__,usr_path_name,sys_ctrl,new_name,cmd_name,base_path_name);
    return new_name;
}

HI_U32 HI_FMGR_FileInfoPrv(HI_IN const char *pathName,HI_OUT HI_FMGR_STAT_IND_S *pFileInfo)
{
    HI_U32 ret = HI_ERR_SUCCESS;
    HI_S32 socket = HI_SYS_INVALID_SOCKET;
    HI_ICC_IND_PKT_S stIccInd = {0};
    HI_U32 ulUsrCmdLen = 0;
    HI_FMGR_STAT_CMD_S stUsrCmd = {0};
    HI_FMGR_STAT_IND_S stUsrInd = {0};
    HI_ICC_CMD_DATA_BUF_S stCmdBufs={0};
    HI_FMGR_PrintDebug("[IN][%s %u]\r\n",__func__,__LINE__);

    //  TODO:入参判断
    if(pathName==HI_NULL||pFileInfo==HI_NULL)
    {
        HI_FMGR_PrintErr("[ERR][%s %u]\r\n",__func__,__LINE__);
        return HI_ERR_INVALID_PARAMETER;
    }
    if(HI_ERR_SUCCESS != ICC_CHANNEL_LOCK(ICC_USR_CHANNEL_ID_FMGR,HI_SYS_WAIT_FOREVER))
    {
        return HI_ERR_WAIT_SEM_FAIL;
    }
    ulUsrCmdLen = (HI_U32)strlen(pathName)+1+sizeof(HI_FMGR_STAT_CMD_S);

    socket = HI_ICC_SocketCreate(ICC_USR_CHANNEL_ID_FMGR,HI_FMGR_DEFAULT_CON_TIME_OUT_SEC,HI_FMGR_DEFAULT_RCV_TIME_OUT_SEC);
    if(socket == HI_SYS_INVALID_SOCKET)
    {
        HI_FMGR_PrintErr("[ERR][%s %u]\r\n",__func__,__LINE__);
        (HI_VOID)ICC_CHANNEL_UNLOCK(ICC_USR_CHANNEL_ID_FMGR);
        return HI_ERR_ICC_SOCKET_CONNECT;
    }

    //发送系统数据
    stCmdBufs.ulBufCount=2;
    stCmdBufs.pBuf[0]=(HI_PBYTE)&stUsrCmd;
    stCmdBufs.ulBufLen[0]=sizeof(stUsrCmd);
    stCmdBufs.pBuf[1]=(HI_PBYTE)pathName;
    stCmdBufs.ulBufLen[1]=(HI_U32)strlen(pathName)+1;
    ret = HI_ICC_SendCmdHeadData(socket,HI_ICC_CMD_FMGR_STAT,ulUsrCmdLen,&stCmdBufs);
    if(HI_ERR_SUCCESS != ret)
    {
        HI_FMGR_PrintErr("[ERR][%s %u]\r\n",__func__,__LINE__);
        HI_ICC_SocketClose(socket);
        (HI_VOID)ICC_CHANNEL_UNLOCK(ICC_USR_CHANNEL_ID_FMGR);
        return ret;
    }


    //接收系统IND
    ret = HI_ICC_Receive(socket,(HI_PBYTE)&stIccInd,sizeof(stIccInd));
    if(HI_ERR_SUCCESS != ret)
    {
        HI_FMGR_PrintErr("[ERR][%s %u]\r\n",__func__,__LINE__);
        HI_ICC_SocketClose(socket);
        (HI_VOID)ICC_CHANNEL_UNLOCK(ICC_USR_CHANNEL_ID_FMGR);
        return ret;
    }
    if(stIccInd.ret_sys!=HI_ERR_SUCCESS)
    {
        HI_FMGR_PrintErr("[ERR][%s %u]\r\n",__func__,__LINE__);
        HI_ICC_SocketClose(socket);
        (HI_VOID)ICC_CHANNEL_UNLOCK(ICC_USR_CHANNEL_ID_FMGR);
        return stIccInd.ret_sys;
    }

    //接收用户IND
    ret = HI_ICC_Receive(socket,(HI_PBYTE)&stUsrInd,sizeof(stUsrInd));
    if(HI_ERR_SUCCESS != ret)
    {
        HI_FMGR_PrintErr("[ERR][%s %u]\r\n",__func__,__LINE__);
        HI_ICC_SocketClose(socket);
        (HI_VOID)ICC_CHANNEL_UNLOCK(ICC_USR_CHANNEL_ID_FMGR);
        return ret;
    }

    (hi_void)memcpy_s(pFileInfo, sizeof(HI_FMGR_STAT_IND_S), &stUsrInd, sizeof(stUsrInd));

//END:
    HI_ICC_SocketClose(socket);
    HI_FMGR_PrintDebug("[OUT][INFO][%s %u][socket=%d][pathName=%s][ret_val=%d][st_size=%d][st_mode=%u][st_blksize=%d][st_blocks=%d][st_ctimex=%d][st_mtimex=%d][st_atimex=%d]\r\n"
        ,__func__,__LINE__,socket,pathName,pFileInfo->ret_val
        ,pFileInfo->st_size,pFileInfo->st_mode,pFileInfo->st_blksize,pFileInfo->st_blocks
        ,pFileInfo->st_ctimex,pFileInfo->st_mtimex,pFileInfo->st_atimex);
    (HI_VOID)ICC_CHANNEL_UNLOCK(ICC_USR_CHANNEL_ID_FMGR);
    return HI_ERR_SUCCESS;
}


HI_U32 HI_FMGR_FsInfoPrv(const char *pathName,HI_FMGR_STATFS_IND_S * pFsInfo)
{
    HI_U32 ret = HI_ERR_SUCCESS;
    HI_S32 socket = HI_SYS_INVALID_SOCKET;
    HI_ICC_IND_PKT_S stIccInd = {0};
    HI_U32 ulUsrCmdLen = 0;
    HI_FMGR_STATFS_CMD_S stUsrCmd = {0};
    HI_FMGR_STATFS_IND_S stUsrInd = {0};
    HI_ICC_CMD_DATA_BUF_S stCmdBufs={0};
    HI_FMGR_PrintDebug("[IN][%s %u]\r\n",__func__,__LINE__);

    //  TODO:入参判断
    if(pathName==HI_NULL||pFsInfo==HI_NULL)
    {
        HI_FMGR_PrintErr("[ERR][%s %u]\r\n",__func__,__LINE__);
        return HI_ERR_INVALID_PARAMETER;
    }
    if(HI_ERR_SUCCESS != ICC_CHANNEL_LOCK(ICC_USR_CHANNEL_ID_FMGR,HI_SYS_WAIT_FOREVER))
    {
        return HI_ERR_WAIT_SEM_FAIL;
    }

    ulUsrCmdLen = (HI_U32)strlen(pathName)+1+sizeof(HI_FMGR_STATFS_CMD_S);

    socket = HI_ICC_SocketCreate(ICC_USR_CHANNEL_ID_FMGR,HI_FMGR_DEFAULT_CON_TIME_OUT_SEC,HI_FMGR_DEFAULT_RCV_TIME_OUT_SEC);
    if(socket == HI_SYS_INVALID_SOCKET)
    {
        HI_FMGR_PrintErr("[ERR][%s %u]\r\n",__func__,__LINE__);
        (HI_VOID)ICC_CHANNEL_UNLOCK(ICC_USR_CHANNEL_ID_FMGR);
        return HI_ERR_ICC_SOCKET_CONNECT;
    }

    //发送系统数据
    stCmdBufs.ulBufCount=2;
    stCmdBufs.pBuf[0]=(HI_PBYTE)&stUsrCmd;
    stCmdBufs.ulBufLen[0]=sizeof(stUsrCmd);
    stCmdBufs.pBuf[1]=(HI_PBYTE)pathName;
    stCmdBufs.ulBufLen[1]=(HI_U32)strlen(pathName)+1;
    ret = HI_ICC_SendCmdHeadData(socket,HI_ICC_CMD_FMGR_STAT_FS,ulUsrCmdLen,&stCmdBufs);
    if(HI_ERR_SUCCESS != ret)
    {
        HI_FMGR_PrintErr("[ERR][%s %u]\r\n",__func__,__LINE__);
        HI_ICC_SocketClose(socket);
        (HI_VOID)ICC_CHANNEL_UNLOCK(ICC_USR_CHANNEL_ID_FMGR);
        return ret;
    }


    //接收系统IND
    ret = HI_ICC_Receive(socket,(HI_PBYTE)&stIccInd,sizeof(stIccInd));
    if(HI_ERR_SUCCESS != ret)
    {
        HI_FMGR_PrintErr("[ERR][%s %u]\r\n",__func__,__LINE__);
        HI_ICC_SocketClose(socket);
        (HI_VOID)ICC_CHANNEL_UNLOCK(ICC_USR_CHANNEL_ID_FMGR);
        return ret;
    }
    if(stIccInd.ret_sys!=HI_ERR_SUCCESS)
    {
        HI_FMGR_PrintErr("[ERR][%s %u]\r\n",__func__,__LINE__);
        HI_ICC_SocketClose(socket);
        (HI_VOID)ICC_CHANNEL_UNLOCK(ICC_USR_CHANNEL_ID_FMGR);
        return stIccInd.ret_sys;
    }

    //接收用户IND
    ret = HI_ICC_Receive(socket,(HI_PBYTE)&stUsrInd,sizeof(stUsrInd));
    if(HI_ERR_SUCCESS != ret)
    {
        HI_FMGR_PrintErr("[ERR][%s %u]\r\n",__func__,__LINE__);
        HI_ICC_SocketClose(socket);
        (HI_VOID)ICC_CHANNEL_UNLOCK(ICC_USR_CHANNEL_ID_FMGR);
        return ret;
    }

    (hi_void)memcpy_s(pFsInfo, sizeof(HI_FMGR_STATFS_IND_S), &stUsrInd, sizeof(stUsrInd));

//END:
    HI_ICC_SocketClose(socket);
    HI_FMGR_PrintDebug("[OUT][INFO][%s %u][socket=%d][pathName=%s][ret_val=%d][f_type=%d][f_bsize=%u][f_blocks=%u][f_bfree=%u][f_bavail=%u][f_files=%u][f_ffree=%u][f_namelen=%d]\r\n"
        ,__func__,__LINE__,socket,pathName,pFsInfo->ret_val
        ,pFsInfo->f_type,pFsInfo->f_bsize,pFsInfo->f_blocks,pFsInfo->f_bfree
        ,pFsInfo->f_bavail,pFsInfo->f_files,pFsInfo->f_ffree,pFsInfo->f_namelen);
    (HI_VOID)ICC_CHANNEL_UNLOCK(ICC_USR_CHANNEL_ID_FMGR);
    return HI_ERR_SUCCESS;
}

HI_U32 HI_FMGR_CreatePrv(HI_IN const char *filename, HI_IN HI_U32 mode,HI_OUT HI_S32 *ret_result)
{
    HI_U32 ret = HI_ERR_SUCCESS;
    HI_S32 socket = HI_SYS_INVALID_SOCKET;
    HI_ICC_IND_PKT_S stIccInd = {0};
    HI_U32 ulUsrCmdLen = 0;
    HI_FMGR_CREATE_CMD_S stUsrCmd = {0};
    HI_FMGR_CREATE_IND_S stUsrInd = {0};
    HI_ICC_CMD_DATA_BUF_S stCmdBufs={0};
    HI_FMGR_PrintDebug("[IN][%s %u]\r\n",__func__,__LINE__);

    //  TODO:入参判断
    if(filename==HI_NULL||ret_result==HI_NULL)
    {
        HI_FMGR_PrintErr("[ERR][%s %u]\r\n",__func__,__LINE__);
        return HI_ERR_INVALID_PARAMETER;
    }
    if(HI_ERR_SUCCESS != ICC_CHANNEL_LOCK(ICC_USR_CHANNEL_ID_FMGR,HI_SYS_WAIT_FOREVER))
    {
        return HI_ERR_WAIT_SEM_FAIL;
    }
    ulUsrCmdLen = (HI_U32)strlen(filename)+1+sizeof(HI_FMGR_CREATE_CMD_S);

    socket = HI_ICC_SocketCreate(ICC_USR_CHANNEL_ID_FMGR,HI_FMGR_DEFAULT_CON_TIME_OUT_SEC,HI_FMGR_DEFAULT_RCV_TIME_OUT_SEC);
    if(socket == HI_SYS_INVALID_SOCKET)
    {
        HI_FMGR_PrintErr("[ERR][%s %u]\r\n",__func__,__LINE__);
        (HI_VOID)ICC_CHANNEL_UNLOCK(ICC_USR_CHANNEL_ID_FMGR);
        return HI_ERR_ICC_SOCKET_CONNECT;
    }

    //发送系统数据
    
    stUsrCmd.mode = mode;
    stCmdBufs.ulBufCount=2;
    stCmdBufs.pBuf[0]=(HI_PBYTE)&stUsrCmd;
    stCmdBufs.ulBufLen[0]=sizeof(stUsrCmd);
    stCmdBufs.pBuf[1]=(HI_PBYTE)filename;
    stCmdBufs.ulBufLen[1]=(HI_U32)strlen(filename)+1;
    ret = HI_ICC_SendCmdHeadData(socket,HI_ICC_CMD_FMGR_CREATE,ulUsrCmdLen,&stCmdBufs);
    if(HI_ERR_SUCCESS != ret)
    {
        HI_FMGR_PrintErr("[ERR][%s %u]\r\n",__func__,__LINE__);
        HI_ICC_SocketClose(socket);
        (HI_VOID)ICC_CHANNEL_UNLOCK(ICC_USR_CHANNEL_ID_FMGR);
        return ret;
    }



    //接收系统IND
    ret = HI_ICC_Receive(socket,(HI_PBYTE)&stIccInd,sizeof(stIccInd));
    if(HI_ERR_SUCCESS != ret)
    {
        HI_FMGR_PrintErr("[ERR][%s %u]\r\n",__func__,__LINE__);
        HI_ICC_SocketClose(socket);
        (HI_VOID)ICC_CHANNEL_UNLOCK(ICC_USR_CHANNEL_ID_FMGR);
        return ret;
    }
    if(stIccInd.ret_sys != HI_ERR_SUCCESS)
    {
        HI_FMGR_PrintErr("[ERR][%s %u]\r\n",__func__,__LINE__);
        HI_ICC_SocketClose(socket);
        (HI_VOID)ICC_CHANNEL_UNLOCK(ICC_USR_CHANNEL_ID_FMGR);
        return stIccInd.ret_sys;
    }


    //接收用户IND
    ret = HI_ICC_Receive(socket,(HI_PBYTE)&stUsrInd,sizeof(stUsrInd));
    if(HI_ERR_SUCCESS != ret)
    {
        HI_FMGR_PrintErr("[ERR][%s %u]\r\n",__func__,__LINE__);
        HI_ICC_SocketClose(socket);
        (HI_VOID)ICC_CHANNEL_UNLOCK(ICC_USR_CHANNEL_ID_FMGR);
        return ret;
    }
    *ret_result = stUsrInd.ret_result;

//END:
    HI_ICC_SocketClose(socket);
    HI_FMGR_PrintDebug("[OUT][INFO][%s %u][socket=%d][filename=%s][mode=%u][ret_result=%d]\r\n"
        ,__func__,__LINE__,socket,filename,mode,*ret_result);
    (HI_VOID)ICC_CHANNEL_UNLOCK(ICC_USR_CHANNEL_ID_FMGR);
    return HI_ERR_SUCCESS;
}

HI_U32 HI_FMGR_OpenPrv(HI_IN const char *pathname, HI_IN int flags,HI_IN HI_U32 mode,HI_OUT HI_S32 *ret_fd,HI_U32 sys_ctrl)
{
    HI_U32 ret = HI_ERR_SUCCESS;
    HI_S32 socket = HI_SYS_INVALID_SOCKET;
    HI_ICC_IND_PKT_S stIccInd = {0};
    HI_U32 ulUsrCmdLen = 0;
    HI_FMGR_OPEN_CMD_S stUsrCmd = {0};
    HI_FMGR_OPEN_IND_S stUsrInd = {0};
    HI_ICC_CMD_DATA_BUF_S stCmdBufs={0};
    HI_FMGR_PrintDebug("[IN][%s %u]\r\n",__func__,__LINE__);

    //  TODO:入参判断
    if(HI_NULL==pathname||HI_NULL==ret_fd)
    {
        HI_FMGR_PrintErr("[ERR][%s %u]\r\n",__func__,__LINE__);
        return HI_ERR_INVALID_PARAMETER;
    }
    if(HI_ERR_SUCCESS != ICC_CHANNEL_LOCK(ICC_USR_CHANNEL_ID_FMGR,HI_SYS_WAIT_FOREVER))
    {
        return HI_ERR_WAIT_SEM_FAIL;
    }
    ulUsrCmdLen = (HI_U32)strlen(pathname)+1+sizeof(HI_FMGR_OPEN_CMD_S);

    socket = HI_ICC_SocketCreate(ICC_USR_CHANNEL_ID_FMGR,HI_FMGR_DEFAULT_CON_TIME_OUT_SEC,HI_FMGR_DEFAULT_RCV_TIME_OUT_SEC);
    if(socket == HI_SYS_INVALID_SOCKET)
    {
        HI_FMGR_PrintErr("[ERR][%s %u]\r\n",__func__,__LINE__);
        (HI_VOID)ICC_CHANNEL_UNLOCK(ICC_USR_CHANNEL_ID_FMGR);
        return HI_ERR_ICC_SOCKET_CONNECT;
    }

    //发送系统数据
    stUsrCmd.mode = mode;
    stUsrCmd.flags = flags;
    stUsrCmd.sys_ctrl = sys_ctrl;
    stCmdBufs.ulBufCount=2;
    stCmdBufs.pBuf[0]=(HI_PBYTE)&stUsrCmd;
    stCmdBufs.ulBufLen[0]=sizeof(stUsrCmd);
    stCmdBufs.pBuf[1]=(HI_PBYTE)pathname;
    stCmdBufs.ulBufLen[1]=(HI_U32)strlen(pathname)+1;
    ret = HI_ICC_SendCmdHeadData(socket,HI_ICC_CMD_FMGR_OPEN,ulUsrCmdLen,&stCmdBufs);
    if(HI_ERR_SUCCESS != ret)
    {
        HI_FMGR_PrintErr("[ERR][%s %u]\r\n",__func__,__LINE__);
        HI_ICC_SocketClose(socket);
        (HI_VOID)ICC_CHANNEL_UNLOCK(ICC_USR_CHANNEL_ID_FMGR);
        return ret;
    }




    //接收系统IND
    ret = HI_ICC_Receive(socket,(HI_PBYTE)&stIccInd,sizeof(stIccInd));
    if(HI_ERR_SUCCESS != ret)
    {
        HI_FMGR_PrintErr("[ERR][%s %u]\r\n",__func__,__LINE__);
        HI_ICC_SocketClose(socket);
        (HI_VOID)ICC_CHANNEL_UNLOCK(ICC_USR_CHANNEL_ID_FMGR);
        return ret;
    }
    if(stIccInd.ret_sys != HI_ERR_SUCCESS)
    {
        HI_FMGR_PrintErr("[ERR][%s %u]\r\n",__func__,__LINE__);
        HI_ICC_SocketClose(socket);
        (HI_VOID)ICC_CHANNEL_UNLOCK(ICC_USR_CHANNEL_ID_FMGR);
        return stIccInd.ret_sys;
    }

    //接收用户IND
    ret = HI_ICC_Receive(socket,(HI_PBYTE)&stUsrInd,sizeof(stUsrInd));
    if(HI_ERR_SUCCESS != ret)
    {
        HI_FMGR_PrintErr("[ERR][%s %u]\r\n",__func__,__LINE__);
        HI_ICC_SocketClose(socket);
        (HI_VOID)ICC_CHANNEL_UNLOCK(ICC_USR_CHANNEL_ID_FMGR);
        return ret;
    }
    *ret_fd = stUsrInd.ret_fd;

//END:
    HI_ICC_SocketClose(socket);
    HI_FMGR_PrintDebug("[OUT][INFO][%s %u][socket=%d][pathname=%s][flags=%d][mode=%u][ret_fd=%d]\r\n"
        ,__func__,__LINE__,socket,pathname,flags,mode,*ret_fd);
    (HI_VOID)ICC_CHANNEL_UNLOCK(ICC_USR_CHANNEL_ID_FMGR);
    return HI_ERR_SUCCESS;
}


HI_U32 HI_FMGR_ReadPrv(HI_IN int fd, HI_OUT const void *buf, HI_IN HI_SiZE_T length,HI_OUT HI_SSiZE_T *ret_real_len)
{
    HI_U32 ret = HI_ERR_SUCCESS;
    HI_S32 socket = HI_SYS_INVALID_SOCKET;
    HI_ICC_IND_PKT_S stIccInd = {0};
    HI_U32 ulUsrCmdLen = sizeof(HI_FMGR_READ_CMD_S);
    HI_FMGR_READ_CMD_S stUsrCmd = {0};
    HI_FMGR_READ_IND_S stUsrInd = {0};
    HI_ICC_CMD_DATA_BUF_S stCmdBufs={0};
    HI_FMGR_PrintDebug("[IN][%s %u]\r\n",__func__,__LINE__);

    //  TODO:入参判断
    if(HI_NULL==buf||fd<0||ret_real_len==HI_NULL)
    {
        HI_FMGR_PrintErr("[ERR][%s %u]\r\n",__func__,__LINE__);
        return HI_ERR_INVALID_PARAMETER;
    }
    if(HI_ERR_SUCCESS != ICC_CHANNEL_LOCK(ICC_USR_CHANNEL_ID_FMGR,HI_SYS_WAIT_FOREVER))
    {
        return HI_ERR_WAIT_SEM_FAIL;
    }

    socket = HI_ICC_SocketCreate(ICC_USR_CHANNEL_ID_FMGR,HI_FMGR_DEFAULT_CON_TIME_OUT_SEC,HI_FMGR_DEFAULT_RCV_TIME_OUT_SEC);
    if(socket == HI_SYS_INVALID_SOCKET)
    {
        HI_FMGR_PrintErr("[ERR][%s %u]\r\n",__func__,__LINE__);
        (HI_VOID)ICC_CHANNEL_UNLOCK(ICC_USR_CHANNEL_ID_FMGR);
        return HI_ERR_ICC_SOCKET_CONNECT;
    }

    //发送系统数据
    stUsrCmd.fd = fd;
    stUsrCmd.length = length;
    stCmdBufs.ulBufCount=1;
    stCmdBufs.pBuf[0]=(HI_PBYTE)&stUsrCmd;
    stCmdBufs.ulBufLen[0]=sizeof(stUsrCmd);
    ret = HI_ICC_SendCmdHeadData(socket,HI_ICC_CMD_FMGR_READ,ulUsrCmdLen,&stCmdBufs);
    if(HI_ERR_SUCCESS != ret)
    {
        HI_FMGR_PrintErr("[ERR][%s %u]\r\n",__func__,__LINE__);
        HI_ICC_SocketClose(socket);
        (HI_VOID)ICC_CHANNEL_UNLOCK(ICC_USR_CHANNEL_ID_FMGR);
        return ret;
    }


    //接收系统IND
    ret = HI_ICC_Receive(socket,(HI_PBYTE)&stIccInd,sizeof(stIccInd));
    if(HI_ERR_SUCCESS != ret)
    {
        HI_FMGR_PrintErr("[ERR][%s %u]\r\n",__func__,__LINE__);
        HI_ICC_SocketClose(socket);
        (HI_VOID)ICC_CHANNEL_UNLOCK(ICC_USR_CHANNEL_ID_FMGR);
        return ret;
    }
    if(stIccInd.ret_sys != HI_ERR_SUCCESS)
    {
        HI_FMGR_PrintErr("[ERR][%s %u]\r\n",__func__,__LINE__);
        HI_ICC_SocketClose(socket);
        (HI_VOID)ICC_CHANNEL_UNLOCK(ICC_USR_CHANNEL_ID_FMGR);
        return stIccInd.ret_sys;
    }

    //接收用户数据
    ret = HI_ICC_Receive(socket,(HI_PBYTE)&stUsrInd,sizeof(stUsrInd));
    if(HI_ERR_SUCCESS != ret)
    {
        HI_FMGR_PrintErr("[ERR][%s %u]\r\n",__func__,__LINE__);
        HI_ICC_SocketClose(socket);
        (HI_VOID)ICC_CHANNEL_UNLOCK(ICC_USR_CHANNEL_ID_FMGR);
        return ret;
    }
    if((HI_U32)(stUsrInd.ret_real_read_size) > length && stUsrInd.ret_real_read_size>0)//大于0的条件必须夹，正负数比较原理
    {
        HI_FMGR_PrintErr("[ERR][%s %u][ret_real_read_size=%d][length=%d]\r\n",__func__,__LINE__
            ,stUsrInd.ret_real_read_size,length);
        HI_ICC_SocketClose(socket);
        (HI_VOID)ICC_CHANNEL_UNLOCK(ICC_USR_CHANNEL_ID_FMGR);
        return HI_ERR_ICC_BAD_DATA;
    }

    *ret_real_len = stUsrInd.ret_real_read_size;
    if(stUsrInd.ret_real_read_size<=0)
    {
        //NOTHING:读失败
    }
    else
    {
        ret = HI_ICC_Receive(socket,(HI_PBYTE)buf,stUsrInd.ret_real_read_size);
        if(HI_ERR_SUCCESS != ret)
        {
            HI_FMGR_PrintErr("[ERR][%s %u]\r\n",__func__,__LINE__);
            HI_ICC_SocketClose(socket);
            (HI_VOID)ICC_CHANNEL_UNLOCK(ICC_USR_CHANNEL_ID_FMGR);
            return ret;
        }
    }
//END:
    HI_ICC_SocketClose(socket);
    HI_FMGR_PrintDebug("[OUT][INFO][%s %u][socket=%d][fd=%d][length=%u][ret_real_len=%d]\r\n"
        ,__func__,__LINE__,socket,fd,length,*ret_real_len);
    (HI_VOID)ICC_CHANNEL_UNLOCK(ICC_USR_CHANNEL_ID_FMGR);
    return HI_ERR_SUCCESS;
}


HI_U32 HI_FMGR_WritePrv(HI_IN int fd,HI_IN const void *buf, HI_IN HI_SiZE_T length,HI_OUT HI_SSiZE_T *ret_real_len)
{
    HI_U32 ret = HI_ERR_SUCCESS;
    HI_S32 socket = HI_SYS_INVALID_SOCKET;
    HI_ICC_IND_PKT_S stIccInd = {0};
    HI_U32 ulUsrCmdLen = sizeof(HI_FMGR_WRITE_CMD_S)+length;
    HI_FMGR_WRITE_CMD_S stUsrCmd = {0};
    HI_FMGR_WRITE_IND_S stUsrInd = {0};
    HI_ICC_CMD_DATA_BUF_S stCmdBufs={0};
    HI_FMGR_PrintDebug("[IN][%s %u]\r\n",__func__,__LINE__);

    //  TODO:入参判断
    if(HI_NULL==buf||fd<0||ret_real_len==HI_NULL)
    {
        HI_FMGR_PrintErr("[ERR][%s %u]\r\n",__func__,__LINE__);
        return HI_ERR_INVALID_PARAMETER;
    }
    if(HI_ERR_SUCCESS != ICC_CHANNEL_LOCK(ICC_USR_CHANNEL_ID_FMGR,HI_SYS_WAIT_FOREVER))
    {
        return HI_ERR_WAIT_SEM_FAIL;
    }

    socket = HI_ICC_SocketCreate(ICC_USR_CHANNEL_ID_FMGR,HI_FMGR_DEFAULT_CON_TIME_OUT_SEC,HI_FMGR_DEFAULT_RCV_TIME_OUT_SEC);
    if(socket == HI_SYS_INVALID_SOCKET)
    {
        HI_FMGR_PrintErr("[ERR][%s %u]\r\n",__func__,__LINE__);
        (HI_VOID)ICC_CHANNEL_UNLOCK(ICC_USR_CHANNEL_ID_FMGR);
        return HI_ERR_ICC_SOCKET_CONNECT;
    }

    //发送系统数据
    stUsrCmd.fd = fd;
    stUsrCmd.length = length;
    stCmdBufs.ulBufCount=2;
    stCmdBufs.pBuf[0]=(HI_PBYTE)&stUsrCmd;
    stCmdBufs.ulBufLen[0]=sizeof(stUsrCmd);
    stCmdBufs.pBuf[1]=(HI_PBYTE)buf;
    stCmdBufs.ulBufLen[1]=length;
    ret = HI_ICC_SendCmdHeadData(socket,HI_ICC_CMD_FMGR_WRITE,ulUsrCmdLen,&stCmdBufs);
    if(HI_ERR_SUCCESS != ret)
    {
        HI_FMGR_PrintErr("[ERR][%s %u]\r\n",__func__,__LINE__);
        HI_ICC_SocketClose(socket);
        (HI_VOID)ICC_CHANNEL_UNLOCK(ICC_USR_CHANNEL_ID_FMGR);
        return ret;
    }


    //接收系统IND
    ret = HI_ICC_Receive(socket,(HI_PBYTE)&stIccInd,sizeof(stIccInd));
    if(HI_ERR_SUCCESS != ret)
    {
        HI_FMGR_PrintErr("[ERR][%s %u]\r\n",__func__,__LINE__);
        HI_ICC_SocketClose(socket);
        (HI_VOID)ICC_CHANNEL_UNLOCK(ICC_USR_CHANNEL_ID_FMGR);
        return ret;
    }
    if(stIccInd.ret_sys != HI_ERR_SUCCESS)
    {
        HI_FMGR_PrintErr("[ERR][%s %u]\r\n",__func__,__LINE__);
        HI_ICC_SocketClose(socket);
        (HI_VOID)ICC_CHANNEL_UNLOCK(ICC_USR_CHANNEL_ID_FMGR);
        return stIccInd.ret_sys;
    }

    //接收用户IND
    ret = HI_ICC_Receive(socket,(HI_PBYTE)&stUsrInd,sizeof(stUsrInd));
    if(HI_ERR_SUCCESS != ret)
    {
        HI_FMGR_PrintErr("[ERR][%s %u]\r\n",__func__,__LINE__);
        HI_ICC_SocketClose(socket);
        (HI_VOID)ICC_CHANNEL_UNLOCK(ICC_USR_CHANNEL_ID_FMGR);
        return ret;
    }
    *ret_real_len = stUsrInd.ret_real_write;
//END:
    HI_ICC_SocketClose(socket);
    HI_FMGR_PrintDebug("[OUT][INFO][%s %u][socket=%d][fd=%d][length=%u][ret_real_len=%d]\r\n"
        ,__func__,__LINE__,socket,fd,length,*ret_real_len);
    (HI_VOID)ICC_CHANNEL_UNLOCK(ICC_USR_CHANNEL_ID_FMGR);
    return HI_ERR_SUCCESS;
}


HI_U32 HI_FMGR_ISeekPrv(HI_S32 fd, HI_OFFET_T offset, HI_S32 whence,HI_OFFET_T *ret_pos)
{
    HI_U32 ret = HI_ERR_SUCCESS;
    HI_S32 socket = HI_SYS_INVALID_SOCKET;
    HI_ICC_IND_PKT_S stIccInd = {0};
    HI_U32 ulUsrCmdLen = sizeof(HI_FMGR_ISEEK_CMD_S);
    HI_FMGR_ISEEK_CMD_S stUsrCmd = {0};
    HI_FMGR_ISEEK_IND_S stUsrInd = {0};
    HI_ICC_CMD_DATA_BUF_S stCmdBufs={0};
    HI_FMGR_PrintDebug("[IN][%s %u]\r\n",__func__,__LINE__);

    //  TODO:入参判断
    if(ret_pos==HI_NULL||fd<0)
    {
        HI_FMGR_PrintErr("[ERR][%s %u]\r\n",__func__,__LINE__);
        return HI_ERR_INVALID_PARAMETER;
    }
    if(HI_ERR_SUCCESS != ICC_CHANNEL_LOCK(ICC_USR_CHANNEL_ID_FMGR,HI_SYS_WAIT_FOREVER))
    {
        return HI_ERR_WAIT_SEM_FAIL;
    }

    socket = HI_ICC_SocketCreate(ICC_USR_CHANNEL_ID_FMGR,HI_FMGR_DEFAULT_CON_TIME_OUT_SEC,HI_FMGR_DEFAULT_RCV_TIME_OUT_SEC);
    if(socket == HI_SYS_INVALID_SOCKET)
    {
        HI_FMGR_PrintErr("[ERR][%s %u]\r\n",__func__,__LINE__);
        (HI_VOID)ICC_CHANNEL_UNLOCK(ICC_USR_CHANNEL_ID_FMGR);
        return HI_ERR_ICC_SOCKET_CONNECT;
    }

    //发送系统数据
    stUsrCmd.fd = fd;
    stUsrCmd.offset = offset;
    stUsrCmd.whence = whence;
    stCmdBufs.ulBufCount=1;
    stCmdBufs.pBuf[0]=(HI_PBYTE)&stUsrCmd;
    stCmdBufs.ulBufLen[0]=sizeof(stUsrCmd);

    ret = HI_ICC_SendCmdHeadData(socket,HI_ICC_CMD_FMGR_SEEK,ulUsrCmdLen,&stCmdBufs);
    if(HI_ERR_SUCCESS != ret)
    {
        HI_FMGR_PrintErr("[ERR][%s %u]\r\n",__func__,__LINE__);
        HI_ICC_SocketClose(socket);
        (HI_VOID)ICC_CHANNEL_UNLOCK(ICC_USR_CHANNEL_ID_FMGR);
        return ret;
    }



    //接收系统IND
    ret = HI_ICC_Receive(socket,(HI_PBYTE)&stIccInd,sizeof(stIccInd));
    if(HI_ERR_SUCCESS != ret)
    {
        HI_FMGR_PrintErr("[ERR][%s %u]\r\n",__func__,__LINE__);
        HI_ICC_SocketClose(socket);
        (HI_VOID)ICC_CHANNEL_UNLOCK(ICC_USR_CHANNEL_ID_FMGR);
        return ret;
    }
    if(stIccInd.ret_sys != HI_ERR_SUCCESS)
    {
        HI_FMGR_PrintErr("[ERR][%s %u]\r\n",__func__,__LINE__);
        HI_ICC_SocketClose(socket);
        (HI_VOID)ICC_CHANNEL_UNLOCK(ICC_USR_CHANNEL_ID_FMGR);
        return stIccInd.ret_sys;
    }

    //接收用户IND
    ret = HI_ICC_Receive(socket,(HI_PBYTE)&stUsrInd,sizeof(stUsrInd));
    if(HI_ERR_SUCCESS != ret)
    {
        HI_FMGR_PrintErr("[ERR][%s %u]\r\n",__func__,__LINE__);
        HI_ICC_SocketClose(socket);
        (HI_VOID)ICC_CHANNEL_UNLOCK(ICC_USR_CHANNEL_ID_FMGR);
        return ret;
    }
    *ret_pos = stUsrInd.ret_pos;

//END:
    HI_ICC_SocketClose(socket);
    HI_FMGR_PrintDebug("[OUT][INFO][%s %u][socket=%d][fd=%d][offset=%d][whence=%d][ret_pos=%d]\r\n"
        ,__func__,__LINE__,socket,fd,offset,whence,*ret_pos);
    (HI_VOID)ICC_CHANNEL_UNLOCK(ICC_USR_CHANNEL_ID_FMGR);
    return HI_ERR_SUCCESS;

}

HI_U32 HI_FMGR_ClosePrv(HI_IN int fd,HI_OUT HI_S32 *ret_result)
{
    HI_U32 ret = HI_ERR_SUCCESS;
    HI_S32 socket = HI_SYS_INVALID_SOCKET;
    HI_ICC_IND_PKT_S stIccInd = {0};
    HI_U32 ulUsrCmdLen = sizeof(HI_FMGR_CLOSE_CMD_S);
    HI_FMGR_CLOSE_CMD_S stUsrCmd = {0};
    HI_FMGR_CLOSE_IND_S stUsrInd = {0};
    HI_ICC_CMD_DATA_BUF_S stCmdBufs={0};
    HI_FMGR_PrintDebug("[IN][%s %u]\r\n",__func__,__LINE__);


    //  TODO:入参判断
    if(fd<0||ret_result==HI_NULL)
    {
        HI_FMGR_PrintErr("[ERR][%s %u]\r\n",__func__,__LINE__);
        return HI_ERR_INVALID_PARAMETER;
    }
    if(HI_ERR_SUCCESS != ICC_CHANNEL_LOCK(ICC_USR_CHANNEL_ID_FMGR,HI_SYS_WAIT_FOREVER))
    {
        return HI_ERR_WAIT_SEM_FAIL;
    }

    socket = HI_ICC_SocketCreate(ICC_USR_CHANNEL_ID_FMGR,HI_FMGR_DEFAULT_CON_TIME_OUT_SEC,HI_FMGR_DEFAULT_RCV_TIME_OUT_SEC);
    if(socket == HI_SYS_INVALID_SOCKET)
    {
        HI_FMGR_PrintErr("[ERR][%s %u]\r\n",__func__,__LINE__);
        (HI_VOID)ICC_CHANNEL_UNLOCK(ICC_USR_CHANNEL_ID_FMGR);
        return HI_ERR_ICC_SOCKET_CONNECT;
    }

    //发送系统数据
    
    stUsrCmd.fd = fd;
    stCmdBufs.ulBufCount=1;
    stCmdBufs.pBuf[0]=(HI_PBYTE)&stUsrCmd;
    stCmdBufs.ulBufLen[0]=sizeof(stUsrCmd);
    ret = HI_ICC_SendCmdHeadData(socket,HI_ICC_CMD_FMGR_CLOSE,ulUsrCmdLen,&stCmdBufs);
    if(HI_ERR_SUCCESS != ret)
    {
        HI_FMGR_PrintErr("[ERR][%s %u]\r\n",__func__,__LINE__);
        HI_ICC_SocketClose(socket);
        (HI_VOID)ICC_CHANNEL_UNLOCK(ICC_USR_CHANNEL_ID_FMGR);
        return ret;
    }


    //接收系统IND
    ret = HI_ICC_Receive(socket,(HI_PBYTE)&stIccInd,sizeof(stIccInd));
    if(HI_ERR_SUCCESS != ret)
    {
        HI_FMGR_PrintErr("[ERR][%s %u]\r\n",__func__,__LINE__);
        HI_ICC_SocketClose(socket);
        (HI_VOID)ICC_CHANNEL_UNLOCK(ICC_USR_CHANNEL_ID_FMGR);
        return ret;
    }
    if(stIccInd.ret_sys != HI_ERR_SUCCESS)
    {
        HI_FMGR_PrintErr("[ERR][%s %u]\r\n",__func__,__LINE__);
        HI_ICC_SocketClose(socket);
        (HI_VOID)ICC_CHANNEL_UNLOCK(ICC_USR_CHANNEL_ID_FMGR);
        return stIccInd.ret_sys;
    }

    //接收用户IND
    ret = HI_ICC_Receive(socket,(HI_PBYTE)&stUsrInd,sizeof(stUsrInd));
    if(HI_ERR_SUCCESS != ret)
    {
        HI_FMGR_PrintErr("[ERR][%s %u]\r\n",__func__,__LINE__);
        HI_ICC_SocketClose(socket);
        (HI_VOID)ICC_CHANNEL_UNLOCK(ICC_USR_CHANNEL_ID_FMGR);
        return ret;
    }
    *ret_result = stUsrInd.ret_result;

//END:
    HI_ICC_SocketClose(socket);
    HI_FMGR_PrintDebug("[OUT][INFO][%s %u][socket=%d][fd=%d][ret_result=%d]\r\n"
        ,__func__,__LINE__,socket,fd,*ret_result);
    (HI_VOID)ICC_CHANNEL_UNLOCK(ICC_USR_CHANNEL_ID_FMGR);
    return HI_ERR_SUCCESS;
}

HI_U32 HI_FMGR_AccessPrv(HI_IN const char *pathname, HI_IN HI_S32 mode,HI_OUT HI_S32 *ret_val)
{
    HI_U32 ret = HI_ERR_SUCCESS;
    HI_S32 socket = HI_SYS_INVALID_SOCKET;
    HI_ICC_IND_PKT_S stIccInd = {0};
    HI_U32 ulUsrCmdLen = 0;
    HI_FMGR_ACCESS_CMD_S stUsrCmd = {0};
    HI_FMGR_ACCESS_IND_S stUsrInd = {0};
    HI_ICC_CMD_DATA_BUF_S stCmdBufs={0};
    HI_FMGR_PrintDebug("[IN][%s %u]\r\n",__func__,__LINE__);

    //  TODO:入参判断
    if(pathname==HI_NULL||ret_val==HI_NULL)
    {
        HI_FMGR_PrintErr("[ERR][%s %u]\r\n",__func__,__LINE__);
        return HI_ERR_INVALID_PARAMETER;
    }
    ulUsrCmdLen=(HI_U32)strlen(pathname)+1+sizeof(HI_FMGR_ACCESS_CMD_S);

    if(HI_ERR_SUCCESS != ICC_CHANNEL_LOCK(ICC_USR_CHANNEL_ID_FMGR,HI_SYS_WAIT_FOREVER))
    {
        return HI_ERR_WAIT_SEM_FAIL;
    }

    socket = HI_ICC_SocketCreate(ICC_USR_CHANNEL_ID_FMGR,HI_FMGR_DEFAULT_CON_TIME_OUT_SEC,HI_FMGR_DEFAULT_RCV_TIME_OUT_SEC);
    if(socket == HI_SYS_INVALID_SOCKET)
    {
        HI_FMGR_PrintErr("[ERR][%s %u]\r\n",__func__,__LINE__);
        (HI_VOID)ICC_CHANNEL_UNLOCK(ICC_USR_CHANNEL_ID_FMGR);
        return HI_ERR_ICC_SOCKET_CONNECT;
    }

    //发送系统数据
    
    stUsrCmd.mode = mode;
    stCmdBufs.ulBufCount=2;
    stCmdBufs.pBuf[0]=(HI_PBYTE)&stUsrCmd;
    stCmdBufs.ulBufLen[0]=sizeof(stUsrCmd);
    stCmdBufs.pBuf[1]=(HI_PBYTE)pathname;
    stCmdBufs.ulBufLen[1]=(HI_U32)strlen(pathname)+1;
    ret = HI_ICC_SendCmdHeadData(socket,HI_ICC_CMD_FMGR_ACCESS,ulUsrCmdLen,&stCmdBufs);
    if(HI_ERR_SUCCESS != ret)
    {
        HI_FMGR_PrintErr("[ERR][%s %u]\r\n",__func__,__LINE__);
        HI_ICC_SocketClose(socket);
        (HI_VOID)ICC_CHANNEL_UNLOCK(ICC_USR_CHANNEL_ID_FMGR);
        return ret;
    }


    //接收系统IND
    ret = HI_ICC_Receive(socket,(HI_PBYTE)&stIccInd,sizeof(stIccInd));
    if(HI_ERR_SUCCESS != ret)
    {
        HI_FMGR_PrintErr("[ERR][%s %u]\r\n",__func__,__LINE__);
        HI_ICC_SocketClose(socket);
        (HI_VOID)ICC_CHANNEL_UNLOCK(ICC_USR_CHANNEL_ID_FMGR);
        return ret;
    }
    if(stIccInd.ret_sys != HI_ERR_SUCCESS)
    {
        HI_FMGR_PrintErr("[ERR][%s %u]\r\n",__func__,__LINE__);
        HI_ICC_SocketClose(socket);
        (HI_VOID)ICC_CHANNEL_UNLOCK(ICC_USR_CHANNEL_ID_FMGR);
        return stIccInd.ret_sys;
    }

    //接收用户IND
    ret = HI_ICC_Receive(socket,(HI_PBYTE)&stUsrInd,sizeof(stUsrInd));
    if(HI_ERR_SUCCESS != ret)
    {
        HI_FMGR_PrintErr("[ERR][%s %u]\r\n",__func__,__LINE__);
        HI_ICC_SocketClose(socket);
        (HI_VOID)ICC_CHANNEL_UNLOCK(ICC_USR_CHANNEL_ID_FMGR);
        return ret;
    }
    *ret_val = stUsrInd.ret_val;

//END:
    HI_ICC_SocketClose(socket);
    HI_FMGR_PrintDebug("[OUT][INFO][%s %u][socket=%d][pathname=%s][mode=%d][ret_val=%d]\r\n"
        ,__func__,__LINE__,socket,pathname,mode,*ret_val);
    (HI_VOID)ICC_CHANNEL_UNLOCK(ICC_USR_CHANNEL_ID_FMGR);
    return HI_ERR_SUCCESS;
}

HI_U32 HI_FMGR_SysOutPrv(HI_IN const char * cmdBuf,HI_IN HI_U32 sys_ctrl)
{
    HI_U32 ret = HI_ERR_SUCCESS;
    HI_S32 socket = HI_SYS_INVALID_SOCKET;
    HI_ICC_IND_PKT_S stIccInd = {0};
    HI_U32 ulUsrCmdLen = 0;
    HI_FMGR_SYSOUT_CMD_S stUsrCmd = {0};
    HI_FMGR_SYSOUT_IND_S stUsrInd = {0};
    HI_ICC_CMD_DATA_BUF_S stCmdBufs={0};
    HI_FMGR_PrintDebug("[IN][%s %u]\r\n",__func__,__LINE__);

    //  TODO:入参判断
    if(cmdBuf==HI_NULL)
    {
        HI_FMGR_PrintErr("[ERR][%s %u]\r\n",__func__,__LINE__);
        return HI_ERR_INVALID_PARAMETER;
    }
    if(HI_ERR_SUCCESS != ICC_CHANNEL_LOCK(ICC_USR_CHANNEL_ID_FMGR,HI_SYS_WAIT_FOREVER))
    {
        return HI_ERR_WAIT_SEM_FAIL;
    }
    ulUsrCmdLen = (HI_U32)strlen(cmdBuf)+1+sizeof(HI_FMGR_SYSOUT_CMD_S);

    socket = HI_ICC_SocketCreate(ICC_USR_CHANNEL_ID_FMGR,HI_FMGR_DEFAULT_CON_TIME_OUT_SEC,HI_FMGR_DEFAULT_RCV_TIME_OUT_SEC);
    if(socket == HI_SYS_INVALID_SOCKET)
    {
        HI_FMGR_PrintErr("[ERR][%s %u]\r\n",__func__,__LINE__);
        (HI_VOID)ICC_CHANNEL_UNLOCK(ICC_USR_CHANNEL_ID_FMGR);
        return HI_ERR_ICC_SOCKET_CONNECT;
    }

    //发送系统数据    
    stUsrCmd.sys_ctrl = sys_ctrl;
    stCmdBufs.ulBufCount=2;
    stCmdBufs.pBuf[0]=(HI_PBYTE)&stUsrCmd;
    stCmdBufs.ulBufLen[0]=sizeof(stUsrCmd);
    stCmdBufs.pBuf[1]=(HI_PBYTE)cmdBuf;
    stCmdBufs.ulBufLen[1]=(HI_U32)strlen(cmdBuf)+1;
    ret = HI_ICC_SendCmdHeadData(socket,HI_ICC_CMD_FMGR_SYSOUT,ulUsrCmdLen,&stCmdBufs);
    if(HI_ERR_SUCCESS != ret)
    {
        HI_FMGR_PrintErr("[ERR][%s %u]\r\n",__func__,__LINE__);
        HI_ICC_SocketClose(socket);
        (HI_VOID)ICC_CHANNEL_UNLOCK(ICC_USR_CHANNEL_ID_FMGR);
        return ret;
    }

    //接收系统IND
    ret = HI_ICC_Receive(socket,(HI_PBYTE)&stIccInd,sizeof(stIccInd));
    if(HI_ERR_SUCCESS != ret)
    {
        HI_FMGR_PrintErr("[ERR][%s %u]\r\n",__func__,__LINE__);
        HI_ICC_SocketClose(socket);
        (HI_VOID)ICC_CHANNEL_UNLOCK(ICC_USR_CHANNEL_ID_FMGR);
        return ret;
    }
    if(stIccInd.ret_sys != HI_ERR_SUCCESS)
    {
        HI_FMGR_PrintErr("[ERR][%s %u]\r\n",__func__,__LINE__);
        HI_ICC_SocketClose(socket);
        (HI_VOID)ICC_CHANNEL_UNLOCK(ICC_USR_CHANNEL_ID_FMGR);
        return stIccInd.ret_sys;
    }

    //接收用户IND
    ret = HI_ICC_Receive(socket,(HI_PBYTE)&stUsrInd,sizeof(stUsrInd));
    if(HI_ERR_SUCCESS != ret)
    {
        HI_FMGR_PrintErr("[ERR][%s %u]\r\n",__func__,__LINE__);
        HI_ICC_SocketClose(socket);
        (HI_VOID)ICC_CHANNEL_UNLOCK(ICC_USR_CHANNEL_ID_FMGR);
        return ret;
    }

//END:
    HI_ICC_SocketClose(socket);
    HI_FMGR_PrintDebug("[OUT][INFO][%s %u][socket=%d][cmdBuf=%s]\r\n"
        ,__func__,__LINE__,socket,cmdBuf);
    (HI_VOID)ICC_CHANNEL_UNLOCK(ICC_USR_CHANNEL_ID_FMGR);
    return HI_ERR_SUCCESS;
}


HI_U32 HI_FMGR_SetTimePrv(HI_IN HI_S32 timeSec,HI_OUT HI_S32 *ret_val)
{
    HI_U32 ret = HI_ERR_SUCCESS;
    HI_S32 socket = HI_SYS_INVALID_SOCKET;
    HI_ICC_IND_PKT_S stIccInd = {0};
    HI_U32 ulUsrCmdLen = sizeof(HI_FMGR_SET_TIME_CMD_S);
    HI_FMGR_SET_TIME_CMD_S stUsrCmd = {0};
    HI_FMGR_SET_TIME_IND_S stUsrInd = {0};
    HI_ICC_CMD_DATA_BUF_S stCmdBufs={0};
    HI_FMGR_PrintDebug("[IN][%s %u]\r\n",__func__,__LINE__);

    //  TODO:入参判断
    if(ret_val==HI_NULL)
    {
        HI_FMGR_PrintErr("[ERR][%s %u]\r\n",__func__,__LINE__);
        return HI_ERR_INVALID_PARAMETER;
    }
    if(HI_ERR_SUCCESS != ICC_CHANNEL_LOCK(ICC_USR_CHANNEL_ID_FMGR,HI_SYS_WAIT_FOREVER))
    {
        return HI_ERR_WAIT_SEM_FAIL;
    }
    socket = HI_ICC_SocketCreate(ICC_USR_CHANNEL_ID_FMGR,HI_FMGR_DEFAULT_CON_TIME_OUT_SEC,HI_FMGR_DEFAULT_RCV_TIME_OUT_SEC);
    if(socket == HI_SYS_INVALID_SOCKET)
    {
        HI_FMGR_PrintErr("[ERR][%s %u]\r\n",__func__,__LINE__);
        (HI_VOID)ICC_CHANNEL_UNLOCK(ICC_USR_CHANNEL_ID_FMGR);
        return HI_ERR_ICC_SOCKET_CONNECT;
    }

    //发送用户数据
    stUsrCmd.tv_sec = timeSec;
    stUsrCmd.tv_usec = 0;
    stCmdBufs.ulBufCount=1;
    stCmdBufs.pBuf[0]=(HI_PBYTE)&stUsrCmd;
    stCmdBufs.ulBufLen[0]=sizeof(stUsrCmd);
    ret = HI_ICC_SendCmdHeadData(socket,HI_ICC_CMD_FMGR_SET_TIME,ulUsrCmdLen,&stCmdBufs);
    if(HI_ERR_SUCCESS != ret)
    {
        HI_FMGR_PrintErr("[ERR][%s %u]\r\n",__func__,__LINE__);
        HI_ICC_SocketClose(socket);
        (HI_VOID)ICC_CHANNEL_UNLOCK(ICC_USR_CHANNEL_ID_FMGR);
        return ret;
    }

    //接收系统IND
    ret = HI_ICC_Receive(socket,(HI_PBYTE)&stIccInd,sizeof(stIccInd));
    if(HI_ERR_SUCCESS != ret)
    {
        HI_FMGR_PrintErr("[ERR][%s %u]\r\n",__func__,__LINE__);
        HI_ICC_SocketClose(socket);
        (HI_VOID)ICC_CHANNEL_UNLOCK(ICC_USR_CHANNEL_ID_FMGR);
        return ret;
    }
    if(stIccInd.ret_sys != HI_ERR_SUCCESS)
    {
        HI_FMGR_PrintErr("[ERR][%s %u]\r\n",__func__,__LINE__);
        HI_ICC_SocketClose(socket);
        (HI_VOID)ICC_CHANNEL_UNLOCK(ICC_USR_CHANNEL_ID_FMGR);
        return stIccInd.ret_sys;
    }

    //接收用户IND
    ret = HI_ICC_Receive(socket,(HI_PBYTE)&stUsrInd,sizeof(stUsrInd));
    if(HI_ERR_SUCCESS != ret)
    {
        HI_FMGR_PrintErr("[ERR][%s %u]\r\n",__func__,__LINE__);
        HI_ICC_SocketClose(socket);
        (HI_VOID)ICC_CHANNEL_UNLOCK(ICC_USR_CHANNEL_ID_FMGR);
        return ret;
    }
    *ret_val = stUsrInd.ret_val;

//END:
    HI_ICC_SocketClose(socket);
    HI_FMGR_PrintDebug("[OUT][INFO][%s %u][socket=%d][timeSec=%d][ret_val=%d]\r\n"
        ,__func__,__LINE__,socket,timeSec,*ret_val);
    (HI_VOID)ICC_CHANNEL_UNLOCK(ICC_USR_CHANNEL_ID_FMGR);
    return HI_ERR_SUCCESS;
}


HI_U32 HI_FMGR_ClientInitTry(HI_OUT HI_U32 *ret_result)
{
    HI_U32 ret = HI_ERR_SUCCESS;
    HI_S32 socket = HI_SYS_INVALID_SOCKET;
    HI_ICC_IND_PKT_S stIccInd = {0};
    HI_U32 ulUsrCmdLen = sizeof(HI_FMGR_INIT_CMD_S);
    HI_FMGR_INIT_CMD_S stUsrCmd = {0};
    HI_FMGR_INIT_IND_S stUsrInd = {0};
    HI_ICC_CMD_DATA_BUF_S stCmdBufs={0};
    HI_FMGR_PrintDebug("[IN][%s][%u]\r\n",__func__,__LINE__);

    //  TODO:入参判断
    if(ret_result==HI_NULL)
    {
        return HI_ERR_INVALID_PARAMETER;
    }
    if(HI_ERR_SUCCESS != ICC_CHANNEL_LOCK(ICC_USR_CHANNEL_ID_FMGR,HI_SYS_WAIT_FOREVER))
    {
        return HI_ERR_WAIT_SEM_FAIL;
    }
    socket = HI_ICC_SocketCreate(ICC_USR_CHANNEL_ID_FMGR,1,HI_FMGR_DEFAULT_RCV_TIME_OUT_SEC);
    if(socket == HI_SYS_INVALID_SOCKET)
    {
        HI_FMGR_PrintErr("[ERR][%s %u]\r\n",__func__,__LINE__);
        (HI_VOID)ICC_CHANNEL_UNLOCK(ICC_USR_CHANNEL_ID_FMGR);
        return HI_ERR_ICC_SOCKET_CONNECT;
    }

    //发送系统数据
    stCmdBufs.ulBufCount=1;
    stCmdBufs.pBuf[0]=(HI_PBYTE)&stUsrCmd;
    stCmdBufs.ulBufLen[0]=sizeof(stUsrCmd);
    ret = HI_ICC_SendCmdHeadData(socket,HI_ICC_CMD_FMGR_INIT,ulUsrCmdLen,&stCmdBufs);
    if(HI_ERR_SUCCESS != ret)
    {
        HI_FMGR_PrintErr("[ERR][%s %u]\r\n",__func__,__LINE__);
        HI_ICC_SocketClose(socket);
        (HI_VOID)ICC_CHANNEL_UNLOCK(ICC_USR_CHANNEL_ID_FMGR);
        return ret;
    }


    //接收系统IND
    ret = HI_ICC_Receive(socket,(HI_PBYTE)&stIccInd,sizeof(stIccInd));
    if(HI_ERR_SUCCESS != ret)
    {
        HI_FMGR_PrintErr("[ERR][%s %u]\r\n",__func__,__LINE__);
        HI_ICC_SocketClose(socket);
        (HI_VOID)ICC_CHANNEL_UNLOCK(ICC_USR_CHANNEL_ID_FMGR);
        return ret;
    }
    if(stIccInd.ret_sys != HI_ERR_SUCCESS)
    {
        HI_FMGR_PrintErr("[ERR][%s %u]\r\n",__func__,__LINE__);
        HI_ICC_SocketClose(socket);
        (HI_VOID)ICC_CHANNEL_UNLOCK(ICC_USR_CHANNEL_ID_FMGR);
        return stIccInd.ret_sys;
    }

    //接收用户IND
    ret = HI_ICC_Receive(socket,(HI_PBYTE)&stUsrInd,sizeof(stUsrInd));
    if(HI_ERR_SUCCESS != ret)
    {
        HI_FMGR_PrintErr("[ERR][%s %u]\r\n",__func__,__LINE__);
        HI_ICC_SocketClose(socket);
        (HI_VOID)ICC_CHANNEL_UNLOCK(ICC_USR_CHANNEL_ID_FMGR);
        return ret;
    }
    *ret_result = stUsrInd.ret_result;

//END:
    HI_ICC_SocketClose(socket);
    HI_FMGR_PrintDebug("[OUT][INFO][%s %u][socket=%d][ret_result=%u]\r\n"
        ,__func__,__LINE__,socket,*ret_result);
    (HI_VOID)ICC_CHANNEL_UNLOCK(ICC_USR_CHANNEL_ID_FMGR);
    return HI_ERR_SUCCESS;
}


HI_U32 HI_FMGR_Open(HI_IN const char * pathname,HI_IN int flags,HI_IN HI_U32 mode,HI_OUT HI_S32 * ret_fd,HI_U16 usr_ctrl)
{
    HI_U32 ret = HI_ERR_SUCCESS;
    char * new_path_name = HI_NULL;
    HI_U32 sys_ctrl = (HI_U32)usr_ctrl;
    if(g_fmgr_clinet_ctrl.bInit==HI_FALSE)
    {
        return HI_ERR_FMGR_INITING;
    }
    new_path_name = HI_FMGR_MakeLegalPathName(pathname,sys_ctrl,&ret);// TODO:pingshen 内存在外面申请，大小估一下，限制入参文件名大小
    if(new_path_name == HI_NULL)
    {
        return ret;
    }

    ret = HI_FMGR_OpenPrv(new_path_name,flags,mode,ret_fd,sys_ctrl);
    HI_MDM_Free(HI_MOD_ID_SAL_ICC,new_path_name);

    if(ret==HI_ERR_SUCCESS && *ret_fd == -1)
    {
        return HI_ERR_FILE_MGR_OPEN_FAIL;
    }
    
    return ret;
}

HI_U32 HI_FMGR_Read(HI_IN int fd,HI_OUT const void * buf,HI_IN HI_SiZE_T length,HI_OUT HI_SSiZE_T * ret_real_len)
{
    HI_U32 ret = HI_ERR_SUCCESS;
    if(g_fmgr_clinet_ctrl.bInit==HI_FALSE)
    {
        return HI_ERR_FMGR_INITING;
    }
   
    ret = HI_FMGR_ReadPrv(fd,buf,length,ret_real_len);
    if(ret==HI_ERR_SUCCESS && *ret_real_len==-1)
    {
        return HI_ERR_FILE_MGR_READ_FAIL;
    }
    return ret;
}

HI_U32 HI_FMGR_Write(HI_IN int fd,HI_IN const void * buf,HI_IN HI_SiZE_T length,HI_OUT HI_SSiZE_T * ret_real_len)
{
    HI_U32 ret = HI_ERR_SUCCESS;
    if(g_fmgr_clinet_ctrl.bInit==HI_FALSE)
    {
        return HI_ERR_FMGR_INITING;
    }
    ret  = HI_FMGR_WritePrv(fd,buf,length,ret_real_len);
    if(ret==HI_ERR_SUCCESS && *ret_real_len==-1)
    {
        return HI_ERR_FILE_MGR_WRITE_FAIL;
    }
    return ret;
}
HI_U32 HI_FMGR_LSeek(HI_S32 fd,HI_OFFET_T offset,HI_S32 whence,HI_OFFET_T * ret_pos)
{
    HI_U32 ret = HI_ERR_SUCCESS;
    if(g_fmgr_clinet_ctrl.bInit==HI_FALSE)
    {
        return HI_ERR_FMGR_INITING;
    }
    ret = HI_FMGR_ISeekPrv(fd,offset,whence,ret_pos);
    if(ret==HI_ERR_SUCCESS && *ret_pos == -1)
    {
       return HI_ERR_FILE_MGR_LSEEK_FAIL;
    }
    return ret;
}
HI_U32 HI_FMGR_Close(HI_IN int fd,HI_OUT HI_S32 * ret_result)
{
    HI_U32 ret = HI_ERR_SUCCESS;
    if(g_fmgr_clinet_ctrl.bInit==HI_FALSE)
    {
        return HI_ERR_FMGR_INITING;
    }
    ret = HI_FMGR_ClosePrv(fd,ret_result);
    if(ret==HI_ERR_SUCCESS&& *ret_result==-1)
    {
        return HI_ERR_FILE_MGR_CLOSE_FAIL;
    }
    return ret;
}
HI_U32 HI_FMGR_Access(HI_IN const char * pathname,HI_IN HI_S32 mode,HI_OUT HI_S32 * ret_val,HI_IN HI_U16 usr_ctrl)
{
    HI_U32 ret = HI_ERR_SUCCESS;
    char * new_path_name = HI_NULL;
    HI_U32 sys_ctrl = (HI_U32)usr_ctrl;
    if(g_fmgr_clinet_ctrl.bInit==HI_FALSE)
    {
        return HI_ERR_FMGR_INITING;
    }
    new_path_name = HI_FMGR_MakeLegalPathName(pathname,sys_ctrl,&ret);
    if(new_path_name == HI_NULL)
    {
        return ret;
    }
    ret = HI_FMGR_AccessPrv(new_path_name,mode,ret_val);
    HI_MDM_Free(HI_MOD_ID_SAL_ICC,new_path_name);
    if(ret==HI_ERR_SUCCESS && *ret_val==-1)
    {
        return HI_ERR_FILE_MGR_ACCESS_FAIL;
    }
    return ret;
}

HI_U32 HI_FMGR_SetTime(HI_IN HI_S32 timeSec)
{
    HI_S32  ret_val = 0;
    HI_U32 ret = HI_ERR_SUCCESS;
    if(g_fmgr_clinet_ctrl.bInit==HI_FALSE)
    {
        return HI_ERR_FMGR_INITING;
    }
    ret = HI_FMGR_SetTimePrv(timeSec, &ret_val);
    if(ret!=HI_ERR_SUCCESS)
    {
        return ret;
    }
    if(ret_val < 0)
    {
        return HI_ERR_ICC_CALL_API_FAIL;
    }
    return HI_ERR_SUCCESS;
}

//TODO:增加判断执行结果的流程
HI_U32 HI_FMGR_MakeDir(HI_IN const char * pathname,HI_IN HI_BOOL bForce,HI_IN HI_U16 usr_ctrl)
{
    HI_U32 ret = HI_ERR_SUCCESS;
    char * new_mk_name = HI_NULL;
    HI_U32 sys_ctrl = (HI_U32)usr_ctrl;
    if(g_fmgr_clinet_ctrl.bInit==HI_FALSE)
    {
        return HI_ERR_FMGR_INITING;
    }
    if(bForce)
    {
        new_mk_name = HI_FMGR_MakeLegalPathName(pathname,sys_ctrl|HI_FMGR_CMD_MK_DIR_PARENTS,&ret);
    }
    else
    {
/*
                    【屏蔽人】z00193780
                    【审核人】b00208046
                    【代码分析】驱动接口，该分支需保留，需保留。
                    【功能影响】不影响正常业务，屏蔽告警
*/
        new_mk_name = HI_FMGR_MakeLegalPathName(pathname,sys_ctrl|HI_FMGR_CMD_MK_DIR_NORMAL,&ret);
    }
    if(new_mk_name == HI_NULL)
    {
        return ret;
    }
    
    ret = HI_FMGR_SysOutPrv(new_mk_name,sys_ctrl);
    HI_MDM_Free(HI_MOD_ID_SAL_ICC,new_mk_name);
    return ret;
}


HI_U32 HI_FMGR_FileInfo(HI_IN const char * pathName,HI_OUT HI_FMGR_STAT_IND_S * pFileInfo,HI_IN HI_U16 usr_ctrl)
{
    HI_U32 ret = HI_ERR_SUCCESS;
    char * new_path_name = HI_NULL;
    if(g_fmgr_clinet_ctrl.bInit==HI_FALSE)
    {
        return HI_ERR_FMGR_INITING;
    }
    new_path_name = HI_FMGR_MakeLegalPathName(pathName,usr_ctrl,&ret);
    if(new_path_name == HI_NULL)
    {
        return ret;
    }

    ret = HI_FMGR_FileInfoPrv(new_path_name,pFileInfo);
    HI_MDM_Free(HI_MOD_ID_SAL_ICC,new_path_name);
    if(ret==HI_ERR_SUCCESS&&pFileInfo->ret_val==-1)
    {
        return HI_ERR_FILE_MGR_STAT_FAIL;
    }
    return ret;
}

HI_U32 HI_FMGR_FsInfo(HI_IN const char * pathName,HI_OUT HI_FMGR_STATFS_IND_S * pFsInfo,HI_IN HI_U16 usr_ctrl)
{
    HI_U32 ret = HI_ERR_SUCCESS;
    char * new_path_name = HI_NULL;
    if(g_fmgr_clinet_ctrl.bInit==HI_FALSE)
    {
        return HI_ERR_FMGR_INITING;
    }
    new_path_name = HI_FMGR_MakeLegalPathName(pathName,usr_ctrl,&ret);
    if(new_path_name == HI_NULL)
    {
        return ret;
    }

    ret = HI_FMGR_FsInfoPrv(new_path_name,pFsInfo);
    HI_MDM_Free(HI_MOD_ID_SAL_ICC,new_path_name);
    if(ret==HI_ERR_SUCCESS&&pFsInfo->ret_val==-1)
    {
        return HI_ERR_FILE_MGT_STATFS_FAIL;
    }
    return ret;

}

HI_U32  HI_FMGR_Sync(HI_VOID)
{
    return HI_FMGR_SysOutPrv("sync",HI_FMGR_PATH_TYPE_SD_ROOT);
}

HI_U32 HI_SD_GetSdCardInfo(HI_OUT HI_BOOL *pExist,HI_OUT HI_U32 *pTotalSize,HI_OUT HI_U32 *pFreeSize)
{
    HI_U32 ret = HI_ERR_SUCCESS;
    HI_FMGR_STATFS_IND_S pFsInfo = {0};
    if(pExist==HI_NULL||pTotalSize==HI_NULL||pFreeSize==HI_NULL)
    {
        return HI_ERR_INVALID_PARAMETER;
    }
    *pExist = HI_FALSE;
    *pTotalSize = 0;
    *pFreeSize = 0;
    ret = HI_FMGR_FsInfo(SD_CARD_BASE_ADDR,&pFsInfo,HI_FMGR_PATH_TYPE_SD_ROOT);
    if(ret == HI_ERR_FILE_MGT_STATFS_FAIL)//SD卡路径不存在
    {
        *pExist = HI_FALSE;
        return HI_ERR_SUCCESS;
    }
    else if(ret == HI_ERR_SUCCESS)//SD卡路径存在
    {

        //TODO: 查一下f_bsize 什么时候会是负数
        *pTotalSize = ((HI_U32)pFsInfo.f_bsize/1024)*pFsInfo.f_blocks;
        *pFreeSize = ((HI_U32)pFsInfo.f_bsize/1024)*pFsInfo.f_bfree;
        *pExist = HI_TRUE;
        return HI_ERR_SUCCESS;
    }
    else//获取SD卡信息失败,可能通信异常
    {
        return ret;
    }
}


HI_BOOL HI_FMGR_GetConnectStat(HI_VOID)
{
    HI_BOOL bChannelConnect = HI_FALSE;
    bChannelConnect = HI_ICC_GetConnectStat(ICC_USR_CHANNEL_ID_FMGR);
    if(bChannelConnect==HI_TRUE && g_fmgr_clinet_ctrl.bInit==HI_TRUE)
    {
        return HI_TRUE;
    }
    else
    {
        return HI_FALSE;
    }
}


HI_U32 HI_FMGR_Init(HI_VOID)
{
    g_fmgr_clinet_ctrl.bInit = HI_TRUE;
    return HI_ERR_SUCCESS;
}
#endif

