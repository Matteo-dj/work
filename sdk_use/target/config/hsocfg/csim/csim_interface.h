
#include "../../database/system/nv/app_nv.h"

#define HI_PLC_MAC_ADDR_LEN 6 


����Ϊ��������

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
    HI_U16 usId; // DIAG ��ID
    HI_U16 usPayloadSize;
    HI_BYTE pucPayload[0];
} MSG_DIAG_CMD_STRU;

typedef struct
{
    HI_U8 ucMrLoopBack;  // 0: ��ʹ��loopback��ʽ;  1: ��ʾʹ��loopback��ʽ;
    HI_U8 ucMrRedirect2TcpPort; // 1: ʹ��DIAG��tcp�˿����uart�˿ڷ���MR֡; 
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

typedef struct
{
    HI_U8 ucMac[6];
} DIAG_CMD_METER_INFO_QRY_ACK_STRU;

typedef struct
{
    HI_NV_FTM_PRODUCT_ID_S stProductId;
    HI_U32 bEnable; // ������Ҫ, ����Ϊ HI_U32, �����Ҫ���ӱ���������Ե���������
}DMS_PRODUCT_ID_SOFT_CFG_STRU;


typedef struct
{
    HI_U8 ucNodeMac[6];    // �ڵ�Mac��ַ
}MRS_DIAG_CMD_MAC_STRU;    // DIAGͨ��MAC��ַ�ṹ

typedef struct
{
    HI_BOOL bTypeFlg;  // �������� HI_FALSE-ɾ��;HI_TRUE-���
    HI_U8 ucNodeMacNum;    // �ڵ�Mac��ַ���� (MAX = 10)
    MRS_DIAG_CMD_MAC_STRU stNodeMac[10];    // �ڵ�Mac��ַ�ṹ
}MRS_DIAG_CMD_MAC_TOPO_STRU;    // DIAGͨ�����/ɾ��MAC���˽ṹ

