//*****************************************************************************
//
//                  ��Ȩ���� (C), 2001-2011, ��Ϊ�������޹�˾
//
//*****************************************************************************
//  �� �� ��   : hi_app.h
//  �� �� ��   : V1.0 
//  ��    ��   : ����/00130424
//  ��������   : 2011��09��06��
//  ��������   : app �ӿڶ���
//  �����б�   : ��
//  �޸���ʷ   : 
//  1.��    �� : 2011��09��06��
//    ��    �� : ����/00130424
//    �޸����� : �����ļ� 
//*****************************************************************************

#ifndef __HI_APP_H__
#define __HI_APP_H__

#include <hi_mdm.h>


//*****************************************************************************


//*****************************************************************************
// ��Ϣ����
// ������ϢΪ 0x0 -- 0x100 
// MRS [0x100, 0x400] FW: [0x100,0x150), SRV: [0x150, 0x400]
// UPG [0x400, 0x800] FW: [0x400,0x450), SRV: [0x450, 0x600]
//
// ��ϢID��������˵����
// [0x0000-0x0100]����ϢIDԤ����MRS����ڲ�ʹ��, ��ϢID����С��0xFFFF.
// 
// shenhankun/2011-10-25
//*****************************************************************************

// MRS ��Ϣ ID����  0x150 -- 0x300

// UPG��ϢID���� 0x400 -- 0x500

//*****************************************************************************


//*****************************************************************************
// PLC�����
// ע��: APP�ڲ����������ID����С�� HI_DFX_ID_BASE_MAC 
// ���ò��� [0x0,0x100)
// MRS ��Χ [0x100, 0x500)
// UPG��Χ  [0x500, 0x800)
//*****************************************************************************

// ���õ�PLC����� 
#define ID_APP_MSG_376_2_FRAME_INPUT   0x01
#define ID_APP_MSG_645_FRAME_INPUT     0x02
#define ID_APP_MSG_MAC_FRAME_INPUT     0x03
#define ID_APP_MSG_MAC_FRAME_OUTPUT    0x10
#define ID_APP_MSG_MR_FRAME_OUTPUT     0x11
//*****************************************************************************


//*****************************************************************************
typedef HI_U32 (*APP_FW_CMD_REQ_F)(HI_U32, HI_IN HI_PVOID, HI_U16);

typedef struct
{
    HI_U32 minId;
    HI_U32 maxId;
    APP_FW_CMD_REQ_F pfnReq;    // ������ĺ���ָ��
} APP_FW_CMD_PROC_TBL_S;

typedef HI_U32 (*APP_FW_MSG_PROC_F)(HI_U32, HI_IN HI_PVOID);

typedef struct
{
    HI_U32 minId;
    HI_U32 maxId;
    APP_FW_MSG_PROC_F pfnMsg;   // ��Ϣ����ĺ���ָ�� 
} APP_FW_MSG_PROC_TBL_S;

typedef struct  
{
    HI_BOOL bAvail; // HI_TRUE: PLCͨ������; HI_FALSE: PLCͨ��������
    HI_U8 reserved[3];
} APP_MSG_PLC_CHL_STATUS_S;

typedef struct
{
    HI_U32 msgId;
    APP_FW_MSG_PROC_F pfnMsg;
} APP_MSG_NODE_S;

typedef struct  
{
    HI_U16 usId;
    
    // ����: MAC��ַ,����ʾĿ�ĵ�ַʱ:ȡֵȫΪ��, ���ʾ���͵�CCO, ȫΪFF���ʾ�㲥,����Ϊָ���ĵ�ַ;
    // ����: ��ʾԴMAC��ַ 
    HI_U8 ucMac[HI_PLC_MAC_ADDR_LEN];
    HI_U8 reserved[2]; // padding only
    HI_U16 usPayloadLen; // ֡����
    HI_PBYTE pucPayload;// ����֡
}APP_PLC_FRAME_DATA_S;


//����ת���ṹ�� 
typedef struct  
{
    HI_U8 ucMac[HI_PLC_MAC_ADDR_LEN];  // ���ݷ��ͷ���MAC��ַ, @@ ����ȥ�� 
    HI_U16 usDataSizse; // ת�����ݳ���
    HI_U8 pucData[0];  // ת�����ݻ�����ָ��
} APP_PLC_CMD_DATA_TRANSMIT_S;

typedef struct
{
    HI_U32 cmdId;
    APP_FW_CMD_REQ_F pfnReq;  // ��ǰ������ĺ���ָ�룬��֧��һ�����Ϊ�˼򻯴�����֧�ֶ�������֧�� 
    APP_PLC_FRAME_DATA_S stReq; // ��ǰ��REQ����
} APP_CMD_NODE_S;
//*****************************************************************************


//*****************************************************************************
// �㶫��������ز�������ͨ
//*****************************************************************************

//*****************************************************************************
// ������
//*****************************************************************************

// ֡������
typedef enum
{
    APP_CSG_PLC_CTRL_TYPE_ACK = 0x0,    // ȷ��/����
    APP_CSG_PLC_CTRL_TYPE_TRANS,        // ����ת��
    APP_CSG_PLC_CTRL_TYPE_CMD,          // ����
    APP_CSG_PLC_CTRL_TYPE_REPORT,       // �����ϱ�
    APP_CSG_PLC_CTRL_TYPE_TEST = 0xE,   // ��������
} APP_CSG_PLC_CTRL_TYPE_E;


// ҵ����չ���־λ
typedef enum
{
    APP_CSG_PLC_CTRL_EXTEND_BIT_N = 0x0,// ��ҵ����չ��
    APP_CSG_PLC_CTRL_EXTEND_BIT_Y       // ��ҵ����չ��
} APP_CSG_PLC_CTRL_EXTEND_BIT_E;


// ��Ӧ��־λ
typedef enum
{
    APP_CSG_PLC_CTRL_RESP_BIT_N = 0x0,  // ������Ӧ
    APP_CSG_PLC_CTRL_RESP_BIT_Y         // ��Ҫ��Ӧ
} APP_CSG_PLC_CTRL_RESP_BIT_E;


// ������־λ
typedef enum
{
    APP_CSG_PLC_CTRL_PRM_SLAVE = 0x0,   // �Ӷ�վ
    APP_CSG_PLC_CTRL_PRM_HOST           // ����վ
} APP_CSG_PLC_CTRL_PRM_E;


// ���䷽��λ
typedef enum
{
    APP_CSG_PLC_CTRL_DIR_DL = 0x0,      // CCO-->STA
    APP_CSG_PLC_CTRL_DIR_UL             // STA-->CCO
} APP_CSG_PLC_CTRL_DIR_E;


// Э��汾��
typedef enum
{
    APP_CSG_PLC_PROTO_VER1 = 0x1,
} APP_CSG_PLC_PROTO_VER_E;

#define APP_CSG_PLC_PROTO_VERSION   (APP_CSG_PLC_PROTO_VER1)


//*****************************************************************************
// ҵ��ID
//*****************************************************************************

// ȷ��/����
typedef enum
{
    APP_CSG_PLC_SRV_ID_ACK_CONFIRM = 0x00,  // ȷ��
    APP_CSG_PLC_SRV_ID_ACK_DENY,            // ����
} APP_CSG_PLC_SRV_ID_ACK_E;


// ����ת��
typedef enum
{
    APP_CSG_PLC_SRV_ID_TRANS_XR = 0x00,     // ����͸��
    APP_CSG_PLC_SRV_ID_PLC_TEST = 0xF0,     // PLC�����Ȳ���
} APP_CSG_PLC_SRV_ID_TRANS_E;


// ����
typedef enum
{
    APP_CSG_PLC_SRV_ID_CMD_SM_RESULT = 0x00,// ��ѯ�ѱ���
    APP_CSG_PLC_SRV_ID_CMD_DL_METERLIST,    // �·��ѱ��б�
    APP_CSG_PLC_SRV_ID_CMD_FILE,            // �ļ�����
    APP_CSG_PLC_SRV_ID_CMD_EVENT_FLAG,      // ����/��ֹ�ӽڵ������ϱ�
    APP_CSG_PLC_SRV_ID_CMD_REBOOT,          // �ӽڵ�����
    APP_CSG_PLC_SRV_ID_CMD_UPG_INFO,        // ��ѯ������Ϣ
    APP_CSG_PLC_SRV_ID_CMD_BIND = 0x72,     // ��������
    APP_CSG_PLC_SRV_ID_CMD_TEST = 0xF0,     // ����֡
} APP_CSG_PLC_SRV_ID_CMD_E;


// �����ϱ�
typedef enum
{
    APP_CSG_PLC_SRV_ID_REPORT_EVENT = 0x00, // �¼������ϱ�
} APP_CSG_PLC_SRV_ID_REPORT_E;


//*****************************************************************************
// PLC֡ͷ
//*****************************************************************************

#define ID_APP_CMD_CSG_DATA_TRANSMIT_PLC    (0x0101)

typedef struct
{
    // ������
    HI_U16 usFrameType : 4; // ֡������
    HI_U16 reserved    : 8;
    HI_U16 usExtendBit : 1; // ҵ����չ���ʶλ 0-��ҵ����չ�� 1-��ҵ����չ��
    HI_U16 usRespBit   : 1; // ��Ӧ��ʶλ 0-����ҪӦ�� 1-��ҪӦ��
    HI_U16 usPrm       : 1; // ������־λ 0-��֡�������ԴӶ�վ 1-��֡������������վ
    HI_U16 usDir       : 1; // ���䷽��λ 0-���з��� 1-���з���

    // ҵ���ʶ
    HI_U8  ucSrvId;

    // Ӧ�ð汾��
    HI_U8  ucVersion;

    // ֡���
    HI_U16 usSeq;

    // ֡��
    HI_U16 usDataLength;

    HI_U8 aucData[0];
} APP_CSG_PLC_FRAME_HEAD;


// ����ת��������͸������֡
typedef struct
{
    HI_U8 aucSrcAddr[HI_METER_ADDR_LEN];    // Դ��ַ
    HI_U8 aucDstAddr[HI_METER_ADDR_LEN];    // Ŀ�ĵ�ַ
    HI_U8 ucTimeout;        // �豸��ʱʱ��
    HI_U8 reserved;
    HI_U16 usDataLen;       // ת�����ݳ���
    HI_U8 aucData[0];       // ת����������
} APP_CSG_TRANS_XR_DL_STRU;

// PLC̽�ⱨ��
typedef struct
{
    HI_U16 usDataLen;       // ת�����ݳ���
    HI_U16 usExpLen;       // �������ձ���
    HI_U8 aucData[0];       // ת����������
} APP_CSG_PLC_TEST_STRU;


// ����ת��������͸������֡
typedef struct
{
    HI_U8 aucSrcAddr[HI_METER_ADDR_LEN];    // Դ��ַ
    HI_U8 aucDstAddr[HI_METER_ADDR_LEN];    // Ŀ�ĵ�ַ
    HI_U8 reserved[2];
    HI_U16 usDataLen;       // ת�����ݳ���
    HI_U8 aucData[0];       // ת����������
} APP_CSG_TRANS_XR_UL_STRU;


// ����ҵ��: ��ѯ�ѱ�������֡
typedef struct
{
    HI_U8 aucMeterAddr[HI_METER_ADDR_LEN];
    HI_U8 reserved[2];
} APP_CSG_METER_INFO;


typedef struct
{
    HI_U8 ucMeterNum;
    HI_U8 reserved[3];
    APP_CSG_METER_INFO astMeterInf[0];
} APP_CSG_CMD_SM_RESULT_UL_STRU;


// ����: �·��ѱ��б�
#define APP_CSG_CMD_DL_METERLIST_MAX    (16)
typedef APP_CSG_CMD_SM_RESULT_UL_STRU   APP_CSG_CMD_DL_METERLIST_STRU;


typedef enum
{
    APP_CSG_ID_FILE_INFO = 0x00,    // �·��ļ���Ϣ
    APP_CSG_ID_FILE_DATA,           // �·��ļ�����
    APP_CSG_ID_FILE_QUERY,          // ��ѯ�ļ����ݰ�����״̬
    APP_CSG_ID_FILE_FINISH,         // �ļ��������֪ͨ
    APP_CSG_ID_FILE_U2B,            // �ļ����䱨�ĵ���ת���ع㲥
} APP_CSG_ID_FILE_E;


typedef enum
{
    APP_CSG_PLC_COMM_TIMEOUT = 0x00,
    APP_CSG_PLC_NOT_SUPPORT_SRV_ID,
    APP_CSG_PLC_CCO_BUSY,
    APP_CSG_PLC_METER_NO_RESPONSE,
    APP_CSG_PLC_FORMAT_ERROR,
    APP_CSG_PLC_DENY_OTHER = 0xFF,
} APP_CSG_PLC_DENY_E;


#define APP_CSG_FILE_FLAG_CLEAR     (0x00)
#define APP_CSG_FILE_FLAG_STA       (0x02)
#define APP_CSG_FILE_FLAG_CLTI      (0x03)


// �ļ�������Ϣͷ�ṹ
typedef struct
{
    HI_U8 ucInfoID;         // �ļ�������ϢID
    HI_U8 reserved[3];
    HI_U8 aucInfo[0];       // �ļ�������Ϣ
} APP_CSG_CMD_FILE_HEAD;

// �ļ�����
typedef struct
{
    HI_U8 ucFileFlag;       // �ļ�����
    HI_U8 reserved;
    HI_U8 aucDstAddr[HI_METER_ADDR_LEN];
    HI_U32 ulFileCrc;       // �ļ���У��CRC32
    HI_U32 ulFileLength;    // �ļ���С
    HI_U16 usTotalSegment;  // �ļ��ܶ���n
    HI_U16 usTimeWindow;    // �ļ�����ʱ�䴰
    HI_U32 ulFileID;        // �ļ�����ID
} APP_CSG_CMD_FILE_INFO_DL_STRU;

typedef struct
{
    HI_U8 ucLength;         // ��չ������������: ���ڱ�ʶ���ұ������չ����������
    HI_CHAR szCode[2];      // ���ұ��벿��ռ2�ֽڣ���ʽΪASCII��
    HI_U8 ucUpgOption;      // ����ѡ��: [7:7]1-ǿ������
    HI_U16 usForceBsn;      // ǿ���������к�
    HI_U16 reserved;
} APP_CSG_CMD_FILE_INFO_EXTEND_STRU;

typedef struct
{
    HI_U32 ulFileID;        // �ļ�����ID
    HI_U16 usRet;           // �����
    HI_U16 usErrCode;       // �������
} APP_CSG_CMD_FILE_INFO_UL_STRU;

#pragma pack(2)
// �ļ�����(����)
typedef struct
{
    HI_U16 usSegmentSn;     // �ļ��κ�
    HI_U16 usTotalSegment;  // �ļ��ܶ���n
    HI_U32 ulFileID;        // �ļ�����ID
    HI_U16 usDataLength;    // �ļ��γ���L
    HI_U8 aucData[0];       // �ļ�������
} APP_CSG_CMD_FILE_DATA_DL_STRU;
#pragma pack()

// �ļ�����(����)
typedef struct
{
    HI_U32 ulFileID;        // �ļ�����ID
    HI_U32 ulRet;           // �����
} APP_CSG_CMD_FILE_DATA_UL_STRU;

// �ļ�������״̬(����)
typedef struct
{
    HI_U32 ulFileID;
    HI_U16 usSegmentStart;
    HI_U16 usSegmentCount;
} APP_CSG_CMD_FILE_STATUS_DL_STRU;

// �ļ�������״̬(����)
typedef struct
{
    HI_U32 ulFileID;
    HI_U16 usSegmentStart;
    HI_U8 ucFileRcvStatus;
    HI_U8 reserved;
    HI_U8 aucBitmap[0];
} APP_CSG_CMD_FILE_STATUS_UL_STRU;

// �ļ��������֪ͨ(����)
#pragma pack(2)
typedef struct
{
    HI_U32 ulFileID;
    HI_U16 usDelayResetTime;
} APP_CSG_CMD_FILE_FINISH_DL_STRU;
#pragma pack()

// �ļ��������֪ͨ(����)
typedef APP_CSG_CMD_FILE_DATA_UL_STRU   APP_CSG_CMD_FILE_FINISH_UL_STRU;
typedef enum
{
    APP_CSG_MODE_PHYSICAL_LOOPBACK = 0,     //�ػ�����
    APP_CSG_MODE_PHYSICAL_TRANSIMIT,        //͸��ת������
    APP_CSG_MODE_PHYSICAL_MAX,
} APP_CSG_PHYSICAL_TEST_MODE;        // ��������ģʽ
#define APP_CSG_PLC_TEST_MODE               (0)
typedef struct
{
    HI_U8 ucId; //����ID  0-����ػ�����ģʽ
    HI_U8 ucReserved; //Ԥ��
    HI_U16 usPayloadLen;    //����������
    HI_U8 aucPayload[0]; //������
} APP_CSG_CMD_TEST_MODE_STRU;//���������ͨ ����֡�ṹ
//*****************************************************************************
//*****************************************************************************



//*****************************************************************************
#endif // __APP_COMMON_H__

