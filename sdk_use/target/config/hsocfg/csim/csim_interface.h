
#include "../../database/system/nv/app_nv.h"

#define HI_PLC_MAC_ADDR_LEN 6 


以下为参数定义

typedef struct
{
    HI_BOOL bRouterAvail;
    HI_BOOL bIsCco;
    HI_U16  usTEI;  
    HI_U8   ucMac[HI_PLC_MAC_ADDR_LEN];
    HI_U8   ucCcoMac[HI_PLC_MAC_ADDR_LEN];
} MSG_ROUTER_STATUS_IND_STRU;


typedef struct
{
    HI_U16 usId; // DIAG 包ID
    HI_U16 usPayloadSize;
    HI_BYTE pucPayload[0];
} MSG_DIAG_CMD_STRU;

typedef struct
{
    HI_U8 ucMrLoopBack;  // 0: 不使用loopback方式;  1: 表示使用loopback方式;
    HI_U8 ucMrRedirect2TcpPort; // 1: 使用DIAG的tcp端口替代uart端口发送MR帧; 
    HI_U8 ucPlcFrameBypassMac; // 1: 使用模拟方式替代MAC的SendFrame; 0: 不使用模拟方式
    HI_U8 reserved; // 预留
} DIAG_CMD_SAL_HOST_CTRL_REQ_STRU;


typedef struct
{
    HI_U16 usId;
    HI_U8 ucMac[HI_PLC_MAC_ADDR_LEN]; // MAC地址,当表示目的地址时:取值全为零, 则表示发送到CCO, 全为FF则表示广播,其它为指定的地址;
    HI_U32 usPayloadLen; // 帧长度
    HI_U8 aucPayload[0]; 
} DIAG_CMD_SEND_CCO_FRAME_STRU;

typedef struct
{
    HI_U8 ucMac[6];
} DIAG_CMD_METER_INFO_QRY_ACK_STRU;

typedef struct
{
    HI_NV_FTM_PRODUCT_ID_S stProductId;
    HI_U32 bEnable; // 对齐需要, 声明为 HI_U32, 如果需要增加变量，则可以调整该类型
}DMS_PRODUCT_ID_SOFT_CFG_STRU;


typedef struct
{
    HI_U8 ucNodeMac[6];    // 节点Mac地址
}MRS_DIAG_CMD_MAC_STRU;    // DIAG通道MAC地址结构

typedef struct
{
    HI_BOOL bTypeFlg;  // 操作类型 HI_FALSE-删除;HI_TRUE-添加
    HI_U8 ucNodeMacNum;    // 节点Mac地址个数 (MAX = 10)
    MRS_DIAG_CMD_MAC_STRU stNodeMac[10];    // 节点Mac地址结构
}MRS_DIAG_CMD_MAC_TOPO_STRU;    // DIAG通道添加/删除MAC拓扑结构

