//*****************************************************************************
//
//                  ��Ȩ���� (C), 2001-2011, ��Ϊ�������޹�˾
//
//*****************************************************************************
//  �� �� ��   : mrs_cmd_msg.h
//  �� �� ��   : V1.0
//  ��    ��   : ����/00130424
//  ��������   : 2011-10-25
//  ��������   : MRS �������Ϣ����
//  �����б�   : NA
//  �޸���ʷ   :
//  1.��    �� : 2011-10-25
//    ��    �� : ����/00130424
//    �޸����� : �����ļ�
//*****************************************************************************

#ifndef __MRS_CMD_MSG_H__
#define __MRS_CMD_MSG_H__

#include <hi_app.h>

HI_START_HEADER


//*****************************************************************************
// ��Ϣ����
// MRS��ص��ⲿ��Ϣ���ڲ��¼��綨ʱ.
//
// ��ϢID��������˵����
// [0x0000-0x0100]����ϢIDԤ����MRS����ڲ�ʹ��, ��ϢID����С��0xFFFF.
//
// shenhankun/2011-10-25
//*****************************************************************************

#define ID_MRS_MSG_PLC_CHL_STATUS       0x200   // PLCͨ��״̬
#define ID_MRS_MSG_MR_TIMER             0x201   // ��ʱ֪ͨ��Ϣ, ʹ��MRS_StartTimer������Ķ�ʱ����ʱ֪ͨ��Ϣ
#define ID_MRS_MSG_MR_DATA              0x210   // ���/������/�ɼ���������֪ͨ
#define ID_MRS_MSG_MR_DATA_CSG          0x211   // �㶫��������������֪ͨ

#define ID_MRS_MSG_HSO_FRAME            0x220   // HSO֡����
#define ID_UPG_MSG_REPORT               0x230   //�ϱ�CCO�˵�STA����
//*****************************************************************************

#define PROTO_VER_3762_2009     (0)
#define PROTO_VER_13762_2013    (1)

#define MRS_DEVICE_TYPE_COLLECTOR   DIAG_MRS_DEVICE_TYPE_COLLECTOR
#define MRS_DEVICE_TYPE_METER       DIAG_MRS_DEVICE_TYPE_METER
#define MRS_DEVICE_TYPE_BROAD_CARRIER_COMM_UNIT (2)


#define MRS_MAP_DEV_TYPE_METER      (0)
#define MRS_MAP_DEV_TYPE_CLT_I      (1)
#define MRS_MAP_DEV_TYPE_CLT_II     (2)

#define BCD2INT(x)      (HI_S32)((x)/16 * 10 + ((x) % 16))
#define UCHAR2BCD(x)    (HI_U8)((x)/10 * 16 + ((x) % 10))


typedef enum
{
    MRS_CMD_STRU_VER_R0 = 0,
    MRS_CMD_STRU_VER_R1,
    MRS_CMD_STRU_VER_R2,
    MRS_CMD_STRU_VER_R3,
} MRS_CMD_STRU_VER_E;


#define MRS_AFN13_DL_VER_R0             (MRS_CMD_STRU_VER_R0)
#define MRS_AFN13_DL_VER_R1             (MRS_CMD_STRU_VER_R1)
#define MRS_AFN13_UP_VER_R0             (MRS_CMD_STRU_VER_R0)
#define MRS_AFN13_UP_VER_R1             (MRS_CMD_STRU_VER_R1)
#define MRS_AFN13_DL_VER                (MRS_AFN13_DL_VER_R1)
#define MRS_AFN13_UP_VER                (MRS_AFN13_UP_VER_R1)

#define MRS_AFN14_DL_VER_R0             (MRS_CMD_STRU_VER_R0)
#define MRS_AFN14_DL_VER_R1             (MRS_CMD_STRU_VER_R1)
#define MRS_AFN14_UP_VER_R0             (MRS_CMD_STRU_VER_R0)
#define MRS_AFN14_UP_VER_R1             (MRS_CMD_STRU_VER_R1)
#define MRS_AFN14_DL_VER                (MRS_AFN14_DL_VER_R1)
#define MRS_AFN14_UP_VER                (MRS_AFN14_UP_VER_R1)

#define MRS_AFN_F1_DL_VER_R0            (MRS_CMD_STRU_VER_R0)
#define MRS_AFN_F1_DL_VER_R1            (MRS_CMD_STRU_VER_R1)
#define MRS_AFN_F1_UP_VER_R0            (MRS_CMD_STRU_VER_R0)
#define MRS_AFN_F1_UP_VER_R1            (MRS_CMD_STRU_VER_R1)
#define MRS_AFN_F1_DL_VER               (MRS_AFN_F1_DL_VER_R1)
#define MRS_AFN_F1_UP_VER               (MRS_AFN_F1_UP_VER_R1)

#define MRS_GET_DEVICE_INF_DL_VER_R0    (MRS_CMD_STRU_VER_R0)
#define MRS_GET_DEVICE_INF_DL_VER_R1    (MRS_CMD_STRU_VER_R1)
#define MRS_GET_DEVICE_INF_DL_VER_R2    (MRS_CMD_STRU_VER_R2)
#define MRS_GET_DEVICE_INF_UP_VER_R0    (MRS_CMD_STRU_VER_R0)
#define MRS_GET_DEVICE_INF_UP_VER_R1    (MRS_CMD_STRU_VER_R1)
#define MRS_GET_DEVICE_INF_UP_VER_R2    (MRS_CMD_STRU_VER_R2)
#define MRS_GET_DEVICE_INF_DL_VER       (MRS_GET_DEVICE_INF_DL_VER_R2)
#define MRS_GET_DEVICE_INF_UP_VER       (MRS_GET_DEVICE_INF_UP_VER_R2)

#define MRS_START_SEARCH_DL_VER_R0      (MRS_CMD_STRU_VER_R0)
#define MRS_START_SEARCH_DL_VER_R1      (MRS_CMD_STRU_VER_R1)
#define MRS_START_SEARCH_DL_VER         (MRS_START_SEARCH_DL_VER_R1)

#define MRS_GET_METERLIST_DL_VER_R0     (MRS_CMD_STRU_VER_R0)
#define MRS_GET_METERLIST_DL_VER_R1     (MRS_CMD_STRU_VER_R1)
#define MRS_GET_METERLIST_DL_VER_R2     (MRS_CMD_STRU_VER_R2)
#define MRS_GET_METERLIST_UP_VER_R0     (MRS_CMD_STRU_VER_R0)
#define MRS_GET_METERLIST_UP_VER_R1     (MRS_CMD_STRU_VER_R1)
#define MRS_GET_METERLIST_UP_VER_R2     (MRS_CMD_STRU_VER_R2)
#define MRS_GET_METERLIST_DL_VER        (MRS_GET_METERLIST_DL_VER_R2)
#define MRS_GET_METERLIST_UP_VER        (MRS_GET_METERLIST_UP_VER_R2)

#define MRS_STOP_SEARCH_DL_VER_R0       (MRS_CMD_STRU_VER_R0)
#define MRS_STOP_SEARCH_DL_VER_R1       (MRS_CMD_STRU_VER_R1)
#define MRS_STOP_SEARCH_DL_VER          (MRS_STOP_SEARCH_DL_VER_R1)

#define MRS_BROADCAST_DL_VER_R0         (MRS_CMD_STRU_VER_R0)
#define MRS_BROADCAST_DL_VER_R1         (MRS_CMD_STRU_VER_R1)
#define MRS_BROADCAST_DL_VER            (MRS_BROADCAST_DL_VER_R1)

#define MRS_EVENT_ENABLE_DL_VER_R1      (MRS_CMD_STRU_VER_R1)
#define MRS_EVENT_ENABLE_DL_VER         (MRS_EVENT_ENABLE_DL_VER_R1)

#define MRS_EVENT_INF_DL_VER_R0         (MRS_CMD_STRU_VER_R0)
#define MRS_EVENT_INF_DL_VER_R1         (MRS_CMD_STRU_VER_R1)
#define MRS_EVENT_INF_UP_VER_R0         (MRS_CMD_STRU_VER_R0)
#define MRS_EVENT_INF_UP_VER_R1         (MRS_CMD_STRU_VER_R1)
#define MRS_EVENT_INF_DL_VER            (MRS_EVENT_INF_DL_VER_R1)
#define MRS_EVENT_INF_UP_VER            (MRS_EVENT_INF_UP_VER_R1)

#define MRS_TEST_TRANSMIT_DL_VER_R0     (MRS_CMD_STRU_VER_R0)
#define MRS_TEST_TRANSMIT_DL_VER_R1     (MRS_CMD_STRU_VER_R1)
#define MRS_TEST_TRANSMIT_DL_VER        (MRS_TEST_TRANSMIT_DL_VER_R1)

#define MRS_TEST_AFNF0_DL_VER_R0        (MRS_CMD_STRU_VER_R0)
#define MRS_TEST_AFNF0_DL_VER_R1        (MRS_CMD_STRU_VER_R1)
#define MRS_TEST_AFNF0_UP_VER_R0        (MRS_CMD_STRU_VER_R0)
#define MRS_TEST_AFNF0_UP_VER_R1        (MRS_CMD_STRU_VER_R1)
#define MRS_TEST_AFNF0_DL_VER           (MRS_TEST_AFNF0_DL_VER_R1)
#define MRS_TEST_AFNF0_UP_VER           (MRS_TEST_AFNF0_UP_VER_R1)

#define MRS_CMD_TEST_MODE_DL_VER_R1     (MRS_CMD_STRU_VER_R1)
#define MRS_CMD_TEST_MODE_DL_VER        (MRS_CMD_TEST_MODE_DL_VER_R1)

#define MRS_CMD_TEST_MODE_UP_VER_R1     (MRS_CMD_STRU_VER_R1)
#define MRS_CMD_TEST_MODE_UP_VER        (MRS_CMD_TEST_MODE_UP_VER_R1)


typedef struct
{
    HI_BOOL bAvail; // HI_TRUE: PLCͨ������; HI_FALSE: PLCͨ��������
    HI_U8 reserved[3];
} MRS_MSG_PLC_CHL_STATUS_STRU;

typedef struct
{
    HI_U16 usDataSizse;
    HI_U16 reserved;
    HI_PBYTE pucData; // ����������֡
} MRS_DATA_BUFFER_STRU;

typedef MRS_DATA_BUFFER_STRU MRS_MSG_MR_3762_DATA_STRU;

//
typedef struct
{
    HI_U16   usDataSizse;
    HI_U8    ucProtocol;
    HI_U8    padding[1];
    HI_PBYTE pucData; // ����������֡
} MRS_MSG_MR_645_DATA_STRU;

typedef struct
{
    HI_U8 ucTimerId; // Timer ID
    HI_U8 reserved[3];
} MRS_MSG_MR_TIMER_STRU;


//*****************************************************************************
// �����
// ע��: APP�ڲ����������ID����С�� HI_DFX_ID_BASE_MAC
//*****************************************************************************
#define ID_MRS_CMD_DATA_TRANSMIT_AFN13_PLC  (0x0001)    // ����ת������AFN-13(PLCͨ��)
#define ID_MRS_CMD_DATA_TRANSMIT_AFN14_PLC  (0x0002)    // ����ת������AFN-14(PLCͨ��)
#define ID_MRS_CMD_PARALLEL_DATA_PLC        (0x0003)    // ������������ת������(PLCͨ��)
#define ID_MRS_CMD_BROADCAST_TRANSMIT_PLC   (0x0004)    // �㲥����ת������(PLCͨ��)
#define ID_MRS_CMD_EVENT_TRANSMIT_PLC       (0x0005)    // �ӽڵ��¼��ϱ�����(PLCͨ��)
#define ID_MRS_CMD_TEST_TRANSMIT_PLC        (0x0006)    // ��������(PLCͨ��)
#define ID_MRS_CMD_TEST_TRANSMIT_AFNF0_PLC  (0x0007)    // PLCͨ������(PLCͨ��)
#define ID_MRS_CMD_EVENT_INF_PLC            (0x0008)    // �ӽڵ��¼��������(PLCͨ��)

#define ID_MRS_CMD_GET_METERADDR_PLC        (0x0010)    // ��ȡ����ַ(PLCͨ��)
#define ID_MRS_CMD_GET_METERLIST            (0x0011)    // ��ȡ�ѱ���(PLCͨ��)
#define ID_MRS_CMD_START_SEARCH_METER       (0x0012)    // ��ʼ�ѱ�����(PLCͨ��)
#define ID_MRS_CMD_STOP_SEARCH_METER        (0x0013)    // ֹͣ�ѱ�����(PLCͨ��)
#define ID_MRS_CMD_GET_DEVICE_INF_PLC       (0x0014)    // ��ȡ�豸��Ϣ(ӳ���ϵ)

#define ID_MRS_CMD_IR_CMD                   (0x0020)

#define ID_MRS_CMD_CSG_DATA_TRANSMIT_PLC    (ID_APP_CMD_CSG_DATA_TRANSMIT_PLC)  // 0x0101

#define ID_MRS_CMD_DATA_TRANSMIT_PLC        (0x0800)    // ����ת������


#define ID_MRS_CMD_DATA_TRANSMIT_TEST_MODE  (0xFFFE)
#define ID_MRS_CMD_DATA_TRANSMIT_XR         (ID_MRS_CMD_DATA_TRANSMIT_AFN13_PLC)
#define ID_MRS_CMD_DATA_TRANSMIT_LR         (ID_MRS_CMD_DATA_TRANSMIT_AFN14_PLC)

//*****************************************************************************

#define MRS_STA_STATUS_RESP_OK      0     // �������Ӧ��
#define MRS_STA_STATUS_NO_RESP      1     // �����Ӧ��
#define MRS_STA_STATUS_RESP_ERR     2     // ����쳣Ӧ��
#define MRS_STA_STATUS_QUEUE_FULL   3     // ������
#define MRS_STA_STATUS_NO_READY     4     // STA��δ����

#define MRS_PLC_DL_FLG 0    // plc֡���б�־
#define MRS_PLC_UP_FLG 1    // plc֡���б�־


typedef struct
{
    HI_U8 ucMeterAdd[HI_METER_ADDR_LEN];   // 6Byte-BCD
    HI_U8 relay    : 4;
    HI_U8 quality  : 4;
    HI_U8 phase    : 3;
    HI_U8 protocol : 3;
    HI_U8 bak      : 2;
}MR_NODE_METER_ADD_STRU;      // �ڵ����ַ�ṹ


typedef struct
{
    HI_U16 interface_ver : 6; // Э��汾��
    HI_U16 stru_size : 6;     // ��֧�ֲ�ͬ�汾�����ɻ�Ϊ ucOffsetSize; //ƫ�Ƶ�������ĳ���,�̶�λ��
    HI_U16 resp_status:4;     // ֡Ӧ��״̬���������0�������Ӧ��1������쳣Ӧ��2��������3��STA����״̬δ����4
    HI_U16 protocol : 4;      // ת�����ݵĹ�Լ����(0 ~ 15)
    HI_U16 data_len : 12;     // ת�����ݳ���(0 ~ 4095)
    HI_U16 seq;               // ֡���
    HI_U8 reserved;
    HI_U8 dir : 1;            // ����λ ���� 0; ���� 1
    HI_U8 data[0];            // ת�����ݻ�����ָ��
} MRS_DATA_TRANSMIT_UP_STRU;  // ����ת�����нṹ


typedef struct
{
    HI_U16 interface_ver : 6; // Э��汾��
    HI_U16 stru_size : 6;     // ��֧�ֲ�ͬ�汾�����ɻ�Ϊ ucOffsetSize; //ƫ�Ƶ�������ĳ���,�̶�λ��
    HI_U16 noresp_flag : 1;   // δӦ�����Ա�־-��CCO����-STA�ɴ��ж�δӦ����Ƿ���Ҫ����
    HI_U16 nak_flag    : 1;   // �������Ա�־-��CCO����-STA�ɴ��ж����յ�����֡���Ƿ���Ҫ����
    HI_U16 try_num     : 2;   // ������Դ���
    HI_U16 protocol : 4;      // ת�����ݵĹ�Լ����(0 ~ 15)
    HI_U16 data_len : 12;     // ת�����ݳ���(0 ~ 4095)
    HI_U16 seq;               // ֡���
    HI_U8 time_out;           // STA�˳�ʱʱ��(100ms)(0 ~ 25500ms)
    HI_U8 dir : 1;            // ����λ ���� 0; ���� 1
    HI_U8 data[0];            // ת�����ݻ�����ָ��
} MRS_DATA_TRANSMIT_DL_STRU;  // ����ת�����нṹ

typedef MRS_DATA_TRANSMIT_UP_STRU MRS_DATA_TRANSMIT_AFN13_UP_STRU;
typedef MRS_DATA_TRANSMIT_UP_STRU MRS_DATA_TRANSMIT_AFN14_UP_STRU;
typedef MRS_DATA_TRANSMIT_DL_STRU MRS_DATA_TRANSMIT_AFN13_DL_STRU;
typedef MRS_DATA_TRANSMIT_DL_STRU MRS_DATA_TRANSMIT_AFN14_DL_STRU;


//AFNF1-F1(��������)����ת�����нṹ�� ��8���ֽڣ�
typedef struct
{
    HI_U16 interface_ver : 6;   // Э��汾��
    HI_U16 stru_size     : 6;   // �ṹ���С-�ɷ��ͷ�����-�����˽ṹ��ĳ���-���ڽ��շ��ҵ�data��λ��
    HI_U16 noresp_try_flag : 1; // δӦ�����Ա�־-��CCO����-STA�ɴ��ж�δӦ����Ƿ���Ҫ����
    HI_U16 nak_try_flag    : 1; // �������Ա�־-��CCO����-STA�ɴ��ж����յ�����֡���Ƿ���Ҫ����
    HI_U16 try_max       : 2;   // ������Դ���
    HI_U16 protocol      : 4;   // ת�����ݵĹ�Լ����(0 ~ 15)
    HI_U16 data_len      : 12;  // ת�����ݳ���(0 ~ 4095)
    HI_U16 seq;                 // ֡���
    HI_U8 time_out;             // ���ʱʱ��(100ms)(0 ~ 25500ms)
    HI_U8 gap_timer;            // ֡����(10ms)(0 ~ 2550ms)
    HI_U8 data[0];              // ת�����ݻ�����ָ��
} MRS_DATA_TRANSMIT_AFNF1_DL_STRU;


//AFNF1-F1(��������)����ת�����нṹ�� ��8���ֽڣ�
typedef struct
{
    HI_U16 interface_ver : 6;   // Э��汾��
    HI_U16 stru_size     : 6;   // �ṹ���С-�ɷ��ͷ�����-�����˽ṹ��ĳ���-���ڽ��շ��ҵ�data��λ��
    HI_U16 sta_status    : 4;   // STAӦ��״̬(����Ӧ��Ϊ0)
    HI_U16 protocol      : 4;   // ת�����ݵĹ�Լ����(0 ~ 15)
    HI_U16 data_len      : 12;  // ת�����ݳ���(0 ~ 4095)
    HI_U16 seq;                 // ֡���
    HI_U16 resp_status;         // ֡Ӧ��״̬(ÿһλ��ʶ645��֡��Ӧ��״̬)
    HI_U8 data[0];              // ת�����ݻ�����ָ��
} MRS_DATA_TRANSMIT_AFNF1_UP_STRU;

typedef struct
{
    HI_U8  addr[HI_METER_ADDR_LEN]; // ����ַ
    HI_U8  protocol;                // ��Լ����
    HI_U8  type : 4;                // ģ������(���ģ��; �ɼ���ģ��)
    HI_U8  padd : 4;
    HI_U32 tx_num;                  // ����֡����
    HI_U32 rx_num;                  // ����֡����
} MRS_CMD_METER_INF;


typedef struct
{
    HI_U8 addr[HI_METER_ADDR_LEN];  // ����ַ
    HI_U8 protocol;                 // ��Լ����
    HI_U8 type : 4;                 // ģ������(���ģ��; �ɼ���ģ��)
    HI_U8 padd : 4;
} MRS_CMD_METERLIST_ITEM;


typedef struct
{
    HI_U16 interface_ver:6;        // Э��汾��
    HI_U16 stru_len : 6;
    HI_U16 reserved : 4;
    HI_U16 reserved2;

/* BEGIN: PN: DTS2016021906890 MODIFY\ADD\DEL by cuiate/00233580 at 2016/2/20 */
    HI_U8  src[HI_PLC_MAC_ADDR_LEN];
    HI_U8  dst[HI_PLC_MAC_ADDR_LEN];
/* END:   PN: DTS2016021906890 MODIFY\ADD\DEL by cuiate/00233580 at 2016/2/20 */
} MRS_GET_DEVICE_INF_DL_STRU;

typedef struct
{
    HI_U16 interface_ver : 6;   // Э��汾��
    HI_U16 stru_len : 6;
    HI_U16 reserved : 4;
    HI_U8 meter_num;            // �������
    HI_U8 reserved2;
    HI_U8 meter_list[0];        // ����ַ(MRS_CMD_METER_INF)
} MRS_GET_METETADDR_UP_STRU;

typedef struct
{
    HI_U8 aucAddr[HI_METER_ADDR_LEN];
    HI_U8 ucProtocol;
    HI_U8 ucType : 4;
    HI_U8 ucStatus : 2; //STA����ӳ������״̬��0-δ���������ڱ����ѱ�1-�Ѿ����������ѱ���ɣ� ����-����
    HI_U8 reserved: 2;
} MRS_DEVICE_INF_STRU;

typedef struct
{
    HI_U8 aucAddr[HI_METER_ADDR_LEN];
    HI_U8 ucProtocol;
    HI_U8 ucType : 4;
    HI_U8 reserved: 4;
} MRS_METER_INF_STRU;

typedef struct
{
    HI_U16 interface_ver : 6;
    HI_U16 stru_len : 6;
    HI_U16 reserved : 4;
    HI_U8 meter_num;            // �������
    HI_U8 reserved2;
    MRS_DEVICE_INF_STRU stDeviceInf; //�豸��Ϣ
/* BEGIN: PN: DTS2016021906890 MODIFY\ADD\DEL by cuiate/00233580 at 2016/2/20 */
    HI_U8  src[HI_PLC_MAC_ADDR_LEN];
    HI_U8  dst[HI_PLC_MAC_ADDR_LEN];
/* END:   PN: DTS2016021906890 MODIFY\ADD\DEL by cuiate/00233580 at 2016/2/20 */
    HI_U8 meter_list[0];        // ����Ϣ(MRS_METER_INF_STRU)
} MRS_GET_DEVICE_INF_UP_STRU;

typedef enum
{
    MRS_SM_SEARCH_RESULT,
    MRS_SM_START_SEARCH,
} MRS_SM_OPTION_E;


/* BEGIN: PN: DTS2016021906890 MODIFY\ADD\DEL by cuiate/00233580 at 2016/2/20 */
typedef struct
{
    HI_U16 interface_ver : 6;   // Э��汾��
    HI_U16 stru_len : 6;        // �ṹ�峤��
    HI_U16 force_resp : 1;      // ǿ��Ӧ���־
    HI_U16 option : 3;          // �ѱ����(MRS_SM_OPTION_E): ��дMRS_SM_START_SEARCH
    HI_U16 reserved;
    HI_U32 seq;                 // �ѱ����к�:ÿ���ѱ����Ψһ��һ�����,�����ѱ����++
} MRS_PLC_START_SEARCH_METER_S;


typedef struct
{
    HI_U16 interface_ver : 6;   // Э��汾��
    HI_U16 stru_len : 6;        // �ṹ�峤��
    HI_U16 force_resp : 1;      // ǿ��Ӧ���־
    HI_U16 option : 3;          // �ѱ����(MRS_SM_OPTION_E): ��дMRS_SM_SEARCH_RESULT
    HI_U16 reserved;
    HI_U32 seq;                 // �ѱ����к�:ÿ���ѱ����Ψһ��һ�����,�����ѱ����++
    HI_U8  src[HI_PLC_MAC_ADDR_LEN];
    HI_U8  dst[HI_PLC_MAC_ADDR_LEN];
} MRS_PLC_METERLIST_DL_S;
/* END:   PN: DTS2016021906890 MODIFY\ADD\DEL by cuiate/00233580 at 2016/2/20 */

// ��ȡ�ѱ�����ӳ���(����)
typedef struct
{
    HI_U16 interface_ver : 6;       // Э��汾��
    HI_U16 stru_len : 6;            // �ṹ�峤��
    HI_U16 status   : 1;            // ״̬�ֶΣ�ǿ��Ӧ��ʱʹ��
    HI_U16 option   : 3;            // ����ѡ��(MRS_SM_OPTION_E)
    HI_U8  meter_num;               // �������
    HI_U8  type;                    // 0:��� 1:�ɼ���

    HI_U8  asset[HI_METER_ADDR_LEN];// STA/�ɼ�����ַ
    HI_U8  id[HI_METER_ADDR_LEN];

    HI_U32 seq;                     // �ѱ����к�:ÿ���ѱ����Ψһ��һ�����,�������ѱ����Żش�CCO

    HI_U8  abnormal;                // II�ɿ�ά�ɲ�: �ϱ��ѱ��쳣���� ��λ��0:���� 1:�澯 2:Ԥ�� 4:��II������
    HI_U8  pad[3];

/* BEGIN: PN: DTS2016021906890 MODIFY\ADD\DEL by cuiate/00233580 at 2016/2/20 */
    HI_U8  src[HI_PLC_MAC_ADDR_LEN];
    HI_U8  dst[HI_PLC_MAC_ADDR_LEN];
/* END:   PN: DTS2016021906890 MODIFY\ADD\DEL by cuiate/00233580 at 2016/2/20 */

    HI_U8  meter_list[0];           // ����ַ�������ݽṹ(MRS_CMD_METERLIST_ITEM)
} MRS_PLC_METERLIST_UP_S;


// ֹͣ�ѱ�(����)
typedef struct
{
    HI_U16 interface_ver : 6;       // Э��汾��
    HI_U16 stru_len : 6;            // �ṹ�峤��
    HI_U16 reserved1: 4;
    HI_U16 reserved2;
    HI_U32 seq;                     // �ѱ����к�:��д�����ѱ�����
} MRS_PLC_STOP_SEARCH_METER_S;

typedef struct
{
    HI_BOOL bDir;   // 0-��������(CCO to STA) 1-��������(STA to CCO)
    HI_BOOL bFlg;   // HI_TRUE:ȷ��; HI_FALSE:����
    HI_U16 usId;    // ��ȷ�ϻ���ϵ�ID��(0xFFFF��Ϊ������ID��)
} MRS_ID_ACK_DENY_STRU;


#define MRS_EVENT_DI_ARRAY  "\x34\x48\x33\x37"  //"\x01\x15\x00\x04"
#define MRS_EVENT_DI_LEN    (4)
#define MRS_EVENT_CTRL      (0x11)


typedef struct
{
    HI_U16 interface_ver : 6;   // Э��汾��
    HI_U16 stru_len      : 6;   // ƫ�Ƶ�������ĳ��ȣ��̶�λ��
    HI_U16 reserved      : 4;
    HI_U16 seq           : 4;
    HI_U16 data_len      : 12;  // ���ݳ���
    HI_U8 data[0];
} MRS_BROADCAST_STRU;

typedef enum
{
    MRS_EVENT_DIR_BIT_CCO2STA,  // CCO->STA
    MRS_EVENT_DIR_BIT_STA2CCO,  // STA->CCO
} MRS_EVENT_DIR_BIT_E;

typedef enum
{
    MRS_EVENT_FRM_BIT_TO,   // �������ԴӶ�վ
    MRS_EVENT_FRM_BIT_FROM, // ������������վ
} MRS_EVENT_FRM_BIT_E;


#define MRS_EVENT_FUN_ACK               (1)  // �¼�������: ȷ��(����)
#define MRS_EVENT_FUN_REPORT_ENABLE     (2)  // �¼�������: �����¼������ϱ�(����)
#define MRS_EVENT_FUN_REPORT_DISABLE    (3)  // �¼�������: ��ֹ�¼������ϱ�(����)
#define MRS_EVENT_FUN_CCOBUF_FULL       (4)  // �¼�������: CCO�໺����(����)

#define MRS_EVENT_FUN_REPORT            (1)  // �¼�������: �����ϱ�(����)
#define MRS_EVENT_FUN_REPORT_MODUL      (30)    //�¼������룬�����ϱ�(����)��ģ�鴥�� �͹����汾��ͬ


typedef struct
{
    HI_U32 ulInterfaceVer   : 6;   // Э��汾��
    HI_U32 ulStructLen      : 6;   // ƫ�Ƶ�������ĳ��ȣ��̶�λ��
    HI_U32 ulDirBit         : 1;   // ����λ: 0:CCO����STA; 1:STA����CCO
    HI_U32 ulFrmBit         : 1;   // ����λ: 0:�������ԴӶ�վ; 1:������������վ
    HI_U32 ulFunCode        : 6;   // ������:
    HI_U32 ulDataLen        : 12;  // ת�����ݳ���
    HI_U16 usSeq;                  // ֡���
    HI_U8  ucMeter[6];             // ����ַ: ����:���޶���
                                   //           ����:�����¼��ĵ���ַ
    HI_U8  ucData[0];              // ת�����ݻ�����ָ��
} MRS_EVENT_INFO_STRU;        /* ��ӦPLC ID Ϊ ID_MRS_CMD_EVENT_INF_PLC */


typedef struct
{
    HI_U16 interface_ver : 6;
    HI_U16 stru_len      : 6;
    HI_U16 timeout       : 4;
    HI_U16 protocol      : 4;
    HI_U16 data_len      : 12;
    HI_U8 data[0];
} MRS_TEST_TRANSMIT_STRU;


typedef struct
{
    HI_U16 interface_ver : 6;
    HI_U16 stru_len      : 6;
    HI_U16 reserved1     : 4;
    HI_U16 data_len;
    HI_U16 expect_len;
    HI_U16 reserved2;
    HI_U8  data[0];
} MRS_TEST_TRANSMIT_AFNF0_STRU;


typedef struct
{
    HI_U16 usCmdId;
    HI_U16 usPayloadLen;
    HI_U8 aucPayload[0];
} MRS_CMD_DATA_TRANSMIT_STRU;


typedef struct
{
    HI_U16 usInterfaceVer : 6;
    HI_U16 usStruLen : 6;
    HI_U16 usPadding1 : 4;
    HI_U8 ucSnId;
    HI_U8 ucPadding2;
} MRS_CMD_TEST_MODE_STRU;


// ֡������
#define MRS_CSG_PLC_CTRL_TYPE_ACK           (APP_CSG_PLC_CTRL_TYPE_ACK)
#define MRS_CSG_PLC_CTRL_TYPE_TRANS         (APP_CSG_PLC_CTRL_TYPE_TRANS)
#define MRS_CSG_PLC_CTRL_TYPE_CMD           (APP_CSG_PLC_CTRL_TYPE_CMD)
#define MRS_CSG_PLC_CTRL_TYPE_REPROT        (APP_CSG_PLC_CTRL_TYPE_REPORT)
#define MRS_CSG_PLC_CTRL_TYPE_TEST          (APP_CSG_PLC_CTRL_TYPE_TEST)

// ҵ����չ���ʶλ
#define MRS_CSG_PLC_CTRL_EXTEND_BIT_N       (APP_CSG_PLC_CTRL_EXTEND_BIT_N)
#define MRS_CSG_PLC_CTRL_EXTEND_BIT_Y       (APP_CSG_PLC_CTRL_EXTEND_BIT_Y)

// ��Ӧ��ʶλ
#define MRS_CSG_PLC_CTRL_RESP_BIT_N         (APP_CSG_PLC_CTRL_RESP_BIT_N)
#define MRS_CSG_PLC_CTRL_RESP_BIT_Y         (APP_CSG_PLC_CTRL_RESP_BIT_Y)

// ������־λ
#define MRS_CSG_PLC_CTRL_PRM_SLAVE          (APP_CSG_PLC_CTRL_PRM_SLAVE)
#define MRS_CSG_PLC_CTRL_PRM_HOST           (APP_CSG_PLC_CTRL_PRM_HOST)

// ���䷽��λ
#define MRS_CSG_PLC_CTRL_DIR_DL             (APP_CSG_PLC_CTRL_DIR_DL)
#define MRS_CSG_PLC_CTRL_DIR_UL             (APP_CSG_PLC_CTRL_DIR_UL)

#define MRS_CSG_PLC_PROTO_VERSION           (APP_CSG_PLC_PROTO_VERSION)


// ȷ��/����
#define MRS_CSG_PLC_SRV_ID_ACK_CONFIRM      (APP_CSG_PLC_SRV_ID_ACK_CONFIRM)        // ȷ��
#define MRS_CSG_PLC_SRV_ID_ACK_DENY         (APP_CSG_PLC_SRV_ID_ACK_DENY)           // ����

// ����ת��
#define MRS_CSG_PLC_SRV_ID_TRANS_XR         (APP_CSG_PLC_SRV_ID_TRANS_XR)           // ����͸��

// PLC�����Ȳ���
#define MRS_CSG_PLC_SRV_ID_PLC_TEST         (APP_CSG_PLC_SRV_ID_PLC_TEST)           //PLC�����Ȳ���

// ����
#define MRS_CSG_PLC_SRV_ID_CMD_SM_RESULT    (APP_CSG_PLC_SRV_ID_CMD_SM_RESULT)      // ��ѯ�ѱ���
#define MRS_CSG_PLC_SRV_ID_CMD_DL_METERLIST (APP_CSG_PLC_SRV_ID_CMD_DL_METERLIST)   // �·��ѱ��б�
#define MRS_CSG_PLC_SRV_ID_CMD_FILE         (APP_CSG_PLC_SRV_ID_CMD_FILE)           // �ļ�����
#define MRS_CSG_PLC_SRV_ID_CMD_EVENT_FLAG   (APP_CSG_PLC_SRV_ID_CMD_EVENT_FLAG)     // ����/��ֹ�ӽڵ��¼�
#define MRS_CSG_PLC_SRV_ID_CMD_REBOOT       (APP_CSG_PLC_SRV_ID_CMD_REBOOT)         // �ӽڵ�����
#define MRS_CSG_PLC_SRV_ID_CMD_UPG_INFO     (APP_CSG_PLC_SRV_ID_CMD_UPG_INFO)       // ��ѯ������Ϣ
#define MRS_CSG_PLC_SRV_ID_CMD_BIND         (APP_CSG_PLC_SRV_ID_CMD_BIND)           // ��������
#define MRS_CSG_PLC_SRV_ID_CMD_TEST         (APP_CSG_PLC_SRV_ID_CMD_TEST)           // ����֡

// �����ϱ�
#define MRS_CSG_PLC_SRV_ID_REPORT_EVENT     (APP_CSG_PLC_SRV_ID_REPORT_EVENT)       // �¼������ϱ�


typedef APP_CSG_PLC_FRAME_HEAD      MRS_CSG_PLC_FRAME_HEAD;
typedef APP_CSG_TRANS_XR_DL_STRU    MRS_CSG_TRANS_XR_DL_STRU;
typedef APP_CSG_TRANS_XR_UL_STRU    MRS_CSG_TRANS_XR_UL_STRU;


HI_END_HEADER
#endif // __MRS_CMD_MSG_H__

