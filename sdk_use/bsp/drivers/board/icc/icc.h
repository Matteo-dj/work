
#ifndef __ICC_H__
#define __ICC_H__
#include <hi_types.h>
#include <hi_mdm_types.h>
#include <hi_icc.h>
#include <dfx_icc.h>

#define HI_ICC_LISTEN_SOCKET_LIMIT_CONNECTION_COUNT 1//listen socket允许缓存的connect连接个数
#define HI_ICC_SERVER_DEFAULT_SEND_OUT_TIME 10//从设备socket默认发送超时时间
#define HI_ICC_SERVER_DEFAULT_RCV_OUT_TIME 10//从设备socket默认接收超时时间
#define ICC_MAX_TRY_CONNECT_FAIL_TIMES 3//尝试连接连续失败x次后，判定连接异常，切换通道状态为断开

#define HI_ICC_CMD_TBL_SIZE  (5)//ICC模块允许用户注册的命令应答项最大个数
#define HI_ICC_TEMP_BUF_SIZE (512)//通道预留缓存大小:用途1当用户无法继续处理命令时，通道使用该缓存读取socket中内容
#define HI_ICC_MAGIC_NUM 0x56127895//通道魔术字:暂未使用
#define HI_IPV4_STR_WITH_ENDDING_CODE_MAX_LEN 16//IP地址字符串格式长度

#define HI_ICC_PORT_NUM_20000 (20000)//已使用端口号(物理)，代码中除初始化配置部分其它地方不应该调用
#define HI_ICC_PORT_NUM_20001 (20001)//已使用端口号(物理)，代码中除初始化配置部分其它地方不应该调用

//*****************************************************************************
// 函数名称: HI_ICC_CMD_F
// 功能描述: 用户注册到ICC的命令处理函数，ICC收到用户命令后调用该函数
//
// 参数说明:
//      socket[in] 收到命令报文的通道端口号，从设备需要将处理结果通过该通道通知主设备
//      pIccCmdPkt[in] ICC命令报文头
//
// 返 回 值:
//      HI_ERR_SUCCESS:成功
//      HI_ERR_ICC_CMD_NOT_SUPPORT:不支持该命令或命令已作废。
//
// 调用要求:
//      用户不得返回错误码HI_ERR_ICC_CMD_NOT_FOUND
//      用户如果对socket进行操作，不得返回错误码HI_ERR_ICC_CMD_NOT_SUPPORT
//
// 调用举例:
// 作    者: 边海文20150205
//*****************************************************************************

typedef HI_U32(*HI_ICC_CMD_F) (HI_IN HI_SOCKET socket,HI_IN HI_ICC_CMD_PKT_S *pIccCmdPkt);
//ICC应答处理函数，暂不支持
typedef HI_U32(*HI_ICC_IND_F) (HI_OUT HI_SOCKET socket,HI_OUT HI_ICC_IND_PKT_S *pIccIndPkt);


typedef enum
{
    ICC_CHANGE_CONNECT_OPT_CONNECT,//切换通道状态为连接状态
    ICC_CHANGE_CONNECT_OPT_CONNECT_FAIL,//通道尝试连接失败次数+1,连续失败次数达到ICC_MAX_TRY_CONNECT_FAIL_TIMES通道变为断开状态
    ICC_CHANGE_CONNECT_OPT_DISCONNECT,//切换通道状态为断开状态
}ICC_CHANGE_CONNECT_OPT_E;//更改通道连接状态类型



typedef struct
{
    HI_U8 bConnect:1;
    HI_U8 bClient:1;
    HI_U8 bServer:1;
    HI_U8 reserver_1:5;
    HI_U8 tryConnectFailTimes;
    HI_U16 phyPortNum;
    HI_U32 ulSemId;
}ICC_CLIENT_CHANNEL_S;




typedef struct
{
    HI_U32 ulMinId;//用户命令应答ID下限
    HI_U32 ulMaxId;//用户命令应答ID上限
    HI_ICC_CMD_F pfnInputCmd;//处理用户命令入口
    HI_ICC_IND_F pfnInputInd;//处理用户应答入口
} HI_ICC_CMD_OBJ_S;//用户命令应答结构

typedef struct
{
    HI_ICC_CMD_OBJ_S cmdTbl[HI_ICC_CMD_TBL_SIZE];
    HI_U8 temp_buf[HI_ICC_TEMP_BUF_SIZE];
    HI_ICC_STAT_S errCtx;
    ICC_CLIENT_CHANNEL_S channel[ICC_CHANNEL_ID_MAX];
    HI_CHAR ucTargetIp[HI_IPV4_STR_WITH_ENDDING_CODE_MAX_LEN];//目标 IP地址 "255.255.255.255"
}HI_ICC_CTRL_S;







HI_EXTERN HI_BOOL g_icc_is_init;
HI_EXTERN HI_ICC_CTRL_S g_icc_ctrl;

#define ICC_GET_ERR_CTX()        ((HI_ICC_STAT_S *)(&(g_icc_ctrl.errCtx)))
#define ICC_GET_CTRL_CTX()       ((HI_ICC_CTRL_S *)(&(g_icc_ctrl)))

#define ICC_CHECK_NOT_INIT_RETURN(ret) do{if(!g_icc_is_init){return ret;}}while(0)
#define ICC_CHECK_NOT_INIT_RETURN_NOVAL()    do{if(!g_icc_is_init){return ;}}while(0)

HI_U32 icc_send(HI_S32 socket,HI_PBYTE buf,HI_S32 totalLen);
HI_U32 icc_receive(HI_S32 socket,HI_PBYTE buf,HI_S32 totalLen);
HI_U32 icc_close_socket(HI_SOCKET s);
HI_U32 icc_connect(HI_SOCKET s, HI_PCSTR pucIp, HI_U16 usPortNum,HI_U32 ulTimeOutsec);
HI_VOID icc_check_connect_status(HI_VOID);


#endif
