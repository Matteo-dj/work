
#define HI_PLC_MAC_ADDR_LEN 6

// ����Ϊ��������
typedef struct
{
    HI_BOOL bRouterAvail;
    HI_BOOL bIsCco;
    HI_U16  usTEI;  // ����12bit��Ч
    HI_U8   ucMac[HI_PLC_MAC_ADDR_LEN];
    HI_U8   ucCcoMac[HI_PLC_MAC_ADDR_LEN];
} MSG_ROUTER_STATUS_IND_STRU;


typedef struct
{
    HI_U16 usId; // DIAG ��ID
    HI_U16 usPayloadSize;
    HI_BYTE pucPayload[0];
} MSG_DIAG_CMD_STRU;

typedef struct
{
    HI_U8 ucMrLoop;  // 0: ��ʹ��loop��ʽ;  1: ��ʾʹ��loop��ʽ;
    HI_U8 ucMrByDiagPort2Uart; // 1: ʹ��DIAG��tcp�˿����uart�˿ڷ���MR֡;
    HI_U8 ucPlcFrameBypassMac; // 1: ʹ��ģ�ⷽʽ���MAC��SendFrame; 0: ��ʹ��ģ�ⷽʽ
    HI_U8 reserved; // Ԥ��
} DIAG_CMD_SAL_HOST_CTRL_REQ_STRU;


typedef struct
{
    HI_U16 usId;
    HI_U8 ucMac[HI_PLC_MAC_ADDR_LEN]; // MAC��ַ,����ʾĿ�ĵ�ַʱ:ȡֵȫΪ��, ���ʾ���͵�CCO, ȫΪFF���ʾ�㲥,����Ϊָ���ĵ�ַ;
    HI_U32 usPayloadLen; // ֡����
    HI_U8 aucPayload[0];
} DIAG_CMD_SEND_CCO_FRAME_STRU;
