
#define HI_PLC_MAC_ADDR_LEN 6

// 以下为参数定义
typedef struct
{
    HI_BOOL bRouterAvail;
    HI_BOOL bIsCco;
    HI_U16  usTEI;  // 仅低12bit有效
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
    HI_U8 ucMrLoop;  // 0: 不使用loop方式;  1: 表示使用loop方式;
    HI_U8 ucMrByDiagPort2Uart; // 1: 使用DIAG的tcp端口替代uart端口发送MR帧;
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
