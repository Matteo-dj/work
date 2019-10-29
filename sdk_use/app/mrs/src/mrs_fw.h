//*****************************************************************************
//
//                  ��Ȩ���� (C), 2001-2011, ��Ϊ�������޹�˾
//
//*****************************************************************************
//  �� �� ��   : mrs_fw.h
//  �� �� ��   : V1.0 
//  ��    ��   : ����/00130424
//  ��������   : 2011-10-25
//  ��������   : MRS������ܽӿ�, ��MRS��Service����ʵ������ 
//  �����б�   : NA
//  �޸���ʷ   : 
//  1.��    �� : 2011-10-25
//    ��    �� : ����/00130424
//    �޸����� : �����ļ� 
//*****************************************************************************


#ifndef __MRS_FW_H__
#define __MRS_FW_H__
HI_START_HEADER

#include <app_mrs.h>
#include <app_nv.h>
#include <hi_usr_nv.h>
#include "mrs_cmd_msg.h"
#include "mrs_dfx.h"

#define MRS_PROTO645_DATAGRAM_DATA_LEN_MAX     240
#define MRS_PROTO645_DATAGRAM_HEADER_LEN_MAX   12
#define MRS_PROTO645_DATAGRAM_LEN_MAX   (MRS_PROTO645_DATAGRAM_DATA_LEN_MAX\
                                        +MRS_PROTO645_DATAGRAM_HEADER_LEN_MAX)

#define HI_SYS_TIMER_ONESHOT    HI_TIMER_TYPE_ONCE
#define HI_SYS_TIMER_PERIODIC   HI_TIMER_TYPE_PERIOD

#define MRS_PROTO_MR_DATAGRAM_BUF_MAX   (2048)

//*****************************************************************************
// ��������: MRS_StartTimer MRS_StopTimer
// ��������: ����/ֹͣ��ʱ��
//           
// ����˵��:
//   ucTimeId [in] ȡֵΪ [EN_APP_TASK_TIMER_MRS_1, EN_APP_TIMER_MAX)
//   ulPeriod [in] ��ʱ����,��ʱ���ڵ��󣬻���� ��Ϣ ID_MRS_MSG_MR_TIMER ͨ��ע�����ڴ��ݸ�Service.
//   ucOption [in] ȡֵΪ HI_SYS_TIMER_PERIODIC ���� HI_SYS_TIMER_ONESHOT ��ʾ���ڻ��Ƿ�����
//
// �� �� ֵ: 
//    HI_ERR_SUCCESS : �ɹ� 
//    ����Ϊ������ 
//
// ����Ҫ��: ���������еĵ��ã��������ж��е��� 
//           ucOptionȡֵΪSAL_TIMER_ONESHOT�����Բ���Ҫ����MRS_StopTimer�ͷŶ�ʱ��
// ���þ���: TODO: ...
// ��    ��: shenhankun/00130424 [2011-11-10]
//*****************************************************************************
HI_PUBLIC HI_U32 MRS_StartTimer(HI_U8 ucTimerId, HI_U32 ulPeriod, HI_U8 ucOption);
HI_PUBLIC HI_U32 MRS_StopTimer(HI_U8 ucTimerId);

HI_PUBLIC HI_U32 MRS_SendMessage(HI_U32 ulQueueId, HI_SYS_QUEUE_MSG_S *pstMsg, HI_U32 ulTimeOut);
HI_PUBLIC HI_U32 MRS_WaitMessage(HI_U32 ulQueueId, HI_SYS_QUEUE_MSG_S *pstMsg, HI_U32 ulTimeOut);
HI_PUBLIC HI_U32 MRS_GetQueueMsgNum(HI_U32 ulQueueId);

HI_PUBLIC HI_U32 MRS_WaitSem(HI_U32 ulSemId, HI_U32 ulTimeOut);
HI_PUBLIC HI_U32 MRS_SignalSem(HI_U32 ulSemId);


// ������ṹ��
typedef struct 
{
    // ͨѶ��ʽ
    // ****************************
    // 0-����
    // 1-����ʽ·���ز�ͨ��
    // 2-�ֲ�ʽ·���ز�ͨ��
    // 3-�����߿���ز�ͨ��
    // 4~9-����
    // 10-΢��������ͨ��
    // 11~19-����
    // 20-��̫��ͨ��
    // 21~63-����
    // ****************************
    HI_U8 ucCommForm :6;

    HI_U8 ucPrm :1;   // ������־ 0-�Ӷ�վ 1-����վ
    HI_U8 ucDir :1;   // ���䷽�� 0-���� 1-����
    
    HI_U8 reserved[3];
}MRS_3762_CTRL_REALM_STRU; // 376.2-2009 ������

// ��Ϣ��ṹ��(����)
typedef struct
{
    HI_U8 ucRouteFlag      : 1;  // ·�ɱ�ʶ 0-��·�� 1-����·��
    HI_U8 ucSubsidiaryFlag : 1;  // �����ڵ��ʶ 0-�޸����ڵ� 1-�и����ڵ�
    HI_U8 ucModuleFlag     : 1;  // ͨѶģ���ʶ 0-������ģ�� 1-�ӽڵ�ģ��
    HI_U8 ucClashCheck     : 1;  // ��ͻ��� 0-�޳�ͻ��� 1-�г�ͻ���
    HI_U8 ucRelayLevel     : 4;  // �м̼��� 0~15 0-��ʾ���м�
    
    HI_U8 ucCHFlag         : 4;  // �ŵ���ʶ 0~15 0-��ʾ�������ŵ�
    HI_U8 ucCorrErrCode    : 4;  // ��������ʶ 0~15 0-δ���� 1-��ʾRS���� 2~15����
    
    HI_U8 ucResByte; // Ԥ��Ӧ���ֽ���
    HI_U8 reserved;
    
    HI_U16 usCommSpeed : 15; // ͨѶ����
    HI_U16 usSpeedUnit : 1;  // ͨѶ���ʵ�λ 0-bps 1-kbps
    
    HI_U8 ucSeq;                // �������к�
    HI_U8 reserved1;
} MRS_3762_INFO_REALM_DL_STRU; // ��Ϣ��(����)

// ��Ϣ��ṹ��(����)
typedef struct
{
    HI_U8 ucRouteFlag   : 1; // ·�ɱ�ʶ0-��·�� 1-����·��
    HI_U8 ucNone1       : 1; // 
    HI_U8 ucModuleFlag  : 1; // ͨѶģ���ʶ 0-������ģ�� 1-�ӽڵ�ģ��
    HI_U8 ucNone2       : 1; // 
    HI_U8 ucRelayLevel  : 4; // �м̼��� 0~15 0-��ʾ���м�
    
    HI_U8 ucCHFlag      : 4; // �ŵ���ʶ 0~15 0-��ʾ�������ŵ� 
    HI_U8 ucNone3       : 4; //  
    
    HI_U8 ucPhaseFlag   : 4; // ʵ�����߱�־ 0-��ȷ�� 1~3-����1��2��3��
    HI_U8 ucMeterCH     : 4; // ���ͨ������
    
    HI_U8 ucCmdSignal   : 4; // ĩ�������ź�Ʒ�� 0~15 0-��Ʒ�� 1-���Ʒ��
    HI_U8 ucResSignal   : 4; // ĩ��Ӧ���ź�Ʒ�� 0~15 0-��Ʒ�� 1-���Ʒ��
    
    HI_U8 ucEvtFlg      : 1; // �¼���־
    HI_U8 ucResrved     : 7; // Ԥ��

    HI_U8 ucSeq;            // �������к�
    HI_U8 reserved[2];
} MRS_3762_INFO_REALM_UL_STRU;  // ��Ϣ��(����)

// ��ַ��ṹ��
typedef struct 
{
    HI_U8 ucSrcAdd[HI_PLC_MAC_ADDR_LEN];        // Դ��ַ,�̶�6�ֽ�
    HI_U8 ucRelayLevel;                      // �м̼��� 0~15
    HI_U8 reserved1;
    HI_U8 ucRelayAdd[15][HI_PLC_MAC_ADDR_LEN];  // �м̵�ַ 0~15
    HI_U8 ucDestAdd[HI_PLC_MAC_ADDR_LEN];       // Ŀ���ַ,�̶�6�ֽ�
    HI_U8 reserved[2];
}MRS_3762_ADDR_REALM_STRU; 

// Ӧ��������
typedef struct
{
    HI_U8 ucAfn; // Ӧ�ù�����AFN
    HI_U8 reserved;
    HI_U16 usAppDataLen;         // ���ݵ�Ԫ��ʶ�����ݵ�Ԫ�ĳ��ȣ�����ó�ԱΪ0���ʾ������
    HI_U8 pucAppDataContent[0];  // ���ݵ�Ԫ��ʶ �� ���ݵ�Ԫ ��������
} MRS_3762_APP_DATA_REALM_STRU;

// �û�������ͷ���ṹ��
typedef struct 
{
    union
    {
       MRS_3762_INFO_REALM_DL_STRU  stInfRealmDL;          // ��Ϣ������
       MRS_3762_INFO_REALM_UL_STRU  stInfRealmUL;           // ��Ϣ������
    } uInfoRealm;
    MRS_3762_ADDR_REALM_STRU     stAddrRealm;            // ��ַ��
    MRS_3762_APP_DATA_REALM_STRU stAppDataRealm;         // Ӧ��������
}MRS_3762_USER_DATA_HRD_STRU; 


// 376.2 ֡ͷ��Ϣ�ṹ��
typedef struct 
{
    MRS_3762_CTRL_REALM_STRU stCtrl;
    MRS_3762_USER_DATA_HRD_STRU strUsrDataHrd; // �û�������ͷ������
}MRS_3762_FRAME_STRU;

typedef struct
{
	HI_U16 ret;		//��id����ֵ��0:�ɹ�
	HI_U16 flag;	//��ȡ��id��Դ��0:����nv, 1:����efuse
	HI_U8  chipId[24];
	HI_U8  mac[6];
	HI_U8  mode;
	HI_U8  rsv;
}EQUIP_CHIP_ID_R_STRU;

typedef struct
{
	HI_U32 ret;
	HI_U8  chipId[24];
}EQUIP_CHIP_ID_W_STRU;


//ID�洢������
typedef enum
{
	HI_ERR_READ_CHIP_ID_EFUSE_FAIL = 1,//��efuseʧ��
	HI_ERR_READ_CHIP_ID_NV_FAIL = 2,	   //��nvʧ��
	HI_ERR_READ_CHIP_ID_MAC_FAIL = 4,	   //��macʧ��
	HI_ERR_WRITE_CHIP_ID_NV_FAIL = 8,	   //дnvʧ��
}DUT_CHIP_ID_RW_ERRNO_ENUM;

//*****************************************************************************
// ��������: MRS_Proto3762Dec
// ��������: ����376.2Э��ı���ͷ�����ݵ�Ԫ��ʶ �� ���ݵ�Ԫ �������ݲ�����
//           
// ����˵��:
//   pucDatagram [in] ����
//   usDatagramSize [in] ���Ĵ�С����λ�ֽ�
//   ppstFrame [out] ָ�� MRS_3762_FRAME_STRU �ṹ, ���ڱ���������֡�ṹ��Ϣ. 
//                   ��ָ���ɸú�������, ��Ҫʹ�� HI_SYS_FREE(HI_MOD_ID_APP_PROTO, ppstFrame) �ͷ�. 
//   
// �� �� ֵ: 
//    HI_ERR_SUCCESS : �ɹ�
//    ����Ϊ������ 
//
// ����Ҫ��: ���յ� ID_MRS_MSG_MR_DATA ��Ϣ�󣬵��øýӿڻ�ȡ 376.2 ֡�ṹ 
// ���þ���: TODO: ...
// ��    ��: shenhankun/00130424 [2011-11-21]
//*****************************************************************************
HI_EXTERN HI_U32 MRS_Proto3762Dec(HI_IN HI_PBYTE pucDatagram, HI_U16 usDatagramSize, HI_OUT MRS_3762_FRAME_STRU** ppstFrame);


//*****************************************************************************
// ��������: MRS_Proto3762Enc
// ��������: 376.2Э�鱨����װ
//           
// ����˵��:
//   pucDatagram [out] 376.2���ģ���Ҫʹ�� HI_SYS_FREE(HI_MOD_ID_APP_PROTO, pucDatagram) �ͷ�
//   pusDatagramSize [out] 376.2���Ĵ�С
//   pstFrame [in] 376.2֡�ṹ
//
// �� �� ֵ: 
//    HI_ERR_SUCCESS : �ɹ�
//    ����Ϊ������ 
//
// ����Ҫ��: ��CCO���� MRS_SendMrData ���ͱ��ĸ������� 
// ���þ���: TODO: ...
// ��    ��: shenhankun/00130424 [2011-11-21]
//*****************************************************************************
HI_EXTERN HI_U32 MRS_Proto3762Enc(HI_OUT HI_PBYTE* pucDatagram, HI_OUT HI_U16* pusDatagramSize, HI_IN MRS_3762_FRAME_STRU* pstFrame);


// 645������ṹ��
typedef struct 
{
    HI_U8 ucFn        :5; // ����Ӧ������    
    HI_U8 ucFrameFlag :1; // ����֡��־ 0-�޺�������֡ 1-�к�������֡
    HI_U8 ucSlaveFlag :1; // ��վ�쳣��־ 0-��վ��ȷӦ�� 1-��վ���쳣��Ϣ��Ӧ��
    HI_U8 ucDir       :1; // ���ͷ��� 0-����վ����������֡ 1-�ɴ�վ������Ӧ��֡
}MRS_645_CTRL_STRU;


typedef enum
{

    METER_PROTO_TRANSPARENT,    /* ͸��Э�� */
    METER_PROTO_645_1997,       /* DL/T 645-1997 */
    METER_PROTO_645_2007,       /* DL/T 645-2007 */
    METER_PROTO_698_45,         /* DL/T 698.45 */
    METER_PROTO_SINGLEWAY_WATERMETER, // �����ϱ�ˮ��
    METER_PROTO_BOTHWAY_WATERMETER,// ˫���ϱ�ˮ��
    METER_PROTO_GAS_METER,// ȼ����
    METER_PROTO_HOT_ETER,// �ȱ�
    METER_PROTO_MAX
} METER_PROTO_E;

#define mrsCheck645Proto(x)     (((x) == METER_PROTO_645_2007) || ((x) == METER_PROTO_645_1997))
#define mrsCheckValidProto(x)   ((x) < METER_PROTO_MAX)
#define mrsCheck69845Proto(x)   ((x) == METER_PROTO_698_45)
#define mrsCheckTransProto(x)   ((x) == METER_PROTO_TRANSPARENT)
#define mrsCheckWaterMeterProto(x)   (((x) < METER_PROTO_MAX) && ((x) >= METER_PROTO_SINGLEWAY_WATERMETER))

#define WIRELESS_TRANSMISSION_PROTOCOL 15
#define WIRELESS_SET_OWNERSHIP 15


// 645 ֡ͷ��Ϣ�ṹ��
typedef struct 
{
    HI_U8 ucVer; // Э��汾,ȡֵΪ MRS_PROTO645_VER_XXX 
    HI_U8 ucAddr[6];           // ���ַ���̶�6�ֽ� 
    MRS_645_CTRL_STRU stCtrl; // ������
    HI_U8 ucDataRealmLen; // �������С����С��200�ֽ�, 0��ʾ��������
    HI_U8 ucDataRealm[MRS_PROTO645_DATAGRAM_DATA_LEN_MAX]; // ������ 
}MRS_645_FRAME_STRU;


//*****************************************************************************
// ��������: MRS_Proto645Dec
// ��������: ����645Э�鱨��ͷ�����ݵ�Ԫ��ʶ �� ���ݵ�Ԫ �������ݲ�����
//           
// ����˵��:
//   pucDatagram [in] ����
//   usDatagramSize [in] ���Ĵ�С����λ�ֽ�
//   pstFrame [out] ָ��MRS_645_FRAME_STRU�ṹ, ���ڱ���������֡�ṹ��Ϣ.
//   
// �� �� ֵ: 
//    HI_ERR_SUCCESS : �ɹ�
//    ����Ϊ������ 
//
// ����Ҫ��: ���յ� ID_MRS_MSG_MR_DATA ��Ϣ�󣬵��øýӿڻ�ȡ645֡�ṹ 
// ���þ���: TODO: ...
// ��    ��: shenhankun/00130424 [2011-11-21]
//*****************************************************************************
HI_EXTERN HI_U32 MRS_Proto645Dec(HI_IN HI_PBYTE pucDatagram, HI_U16 usDatagramSize, HI_OUT MRS_645_FRAME_STRU* pstFrame);


//*****************************************************************************
// ��������: MRS_Proto645Enc
// ��������: 645Э�鱨����װ
//           
// ����˵��:
//   ucDatagram [out] 645�������Ļ���
//   pusDatagramSize [out] 645����ʵ�ʴ�С
//   pstFrame [in] 645֡�ṹ MRS_645_FRAME_STRU 
//
// �� �� ֵ: 
//    HI_ERR_SUCCESS : �ɹ�
//    ����Ϊ������ 
//
// ����Ҫ��: ��STA���� MRS_SendMrData ���ͱ��ĸ�������ͺ���Ҫʹ��HI_SYS_FREE(HI_MOD_ID_APP_PROTO, pucDatagram)�ͷ�֡����
// ���þ���: TODO: ...
// ��    ��: shenhankun/00130424 [2011-11-21]
//*****************************************************************************
HI_EXTERN HI_U32 MRS_Proto645Enc(HI_OUT HI_U8 ucDatagram[MRS_PROTO645_DATAGRAM_LEN_MAX], HI_OUT HI_U16* pusDatagramSize, HI_IN MRS_645_FRAME_STRU* pstFrame);





HI_END_HEADER
#endif // __MRS_FW_H__


