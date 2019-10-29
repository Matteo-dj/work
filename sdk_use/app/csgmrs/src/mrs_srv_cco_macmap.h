//*****************************************************************************
//
//                  ��Ȩ���� (C), 1998-2011, ��Ϊ�������޹�˾
//
//*****************************************************************************
//  �� �� ��   : mrs_srv_cco_macmap.h
//  �� �� ��   : V1.0 
//  ��    ��   : liuxipeng/KF54842
//  ��������   : 2011-12-13
//  ��������   : TODO: ...
//               
//  �����б�   : TODO: ...
//  �޸���ʷ   : 
//  1.��    �� : 2011-12-13
//    ��    �� : liuxipeng/KF54842
//    �޸����� : �����ļ� 
//
//*****************************************************************************
#ifndef __MRS_SRV_CCO_MACMAP_H__
#define __MRS_SRV_CCO_MACMAP_H__

HI_START_HEADER

//�ڲ�ά��һ��ӳ���CCO/MacMeterMap
#define MRS_MAC_DEVICE_MAP_NUM_MAX              (2448) //ӳ���ϵ�������
#define MRS_MAC_CLT_MAP_NUM_MAX                 (2400)

#define MRS_MAC_MAP_TTL_MIN                     (1)
#define MRS_MAC_MAP_TTL_DEFAULT                 (15)
#define MRS_MAC_MAP_TTL_MAX                     (50)
#define MRS_MAC_MAP_CHECK_TTL_TIME_MIN          (1)   //��λΪs
#define MRS_MAC_MAP_CHECK_TTL_TIME_DEFAULT      (2 * 60)   //��λΪs
#define MRS_MAC_MAP_CHECK_TTL_TIME_MAX          (2 * 60 * 60)   //��λΪs
#define MRS_MAC_MAP_MRS_TTL_MIN                 (1)
#define MRS_MAC_MAP_MRS_TTL_DEFAULT             (15)
#define MRS_MAC_MAP_MRS_TTL_MAX                 (50)

#define MRS_MAC_MAP_SEND_PLC_TIME_DEFAULT       (2000) //CCO�˷��Ͷ����ַ֡Ĭ��ʱ����
#define MRS_MAC_MAP_SEND_PLC_TIME_MAX           (1000 * 60) //CCO�˷��Ͷ����ַ֡���ʱ����
#define MRS_MAC_MAP_SEND_PLC_TIME_MIN           (50) //CCO�˷��Ͷ����ַ֡��Сʱ����
#define MRS_MAC_MAP_SEND_LAST_PLC_TIME_DEFAULT  (1000 * 3) //CCO�˷������һ֡�����ַ֡Ĭ��ʱ����
#define MRS_MAC_MAP_SEND_LAST_PLC_TIME_MAX      (1000 * 60) //CCO�˷������һ֡�����ַ֡���ʱ����
#define MRS_MAC_MAP_SEND_LAST_PLC_TIME_MIN      (50) //CCO�˷������һ֡�����ַ֡��Сʱ����
#define MRS_MAC_MAP_SEND_PLC_NUM_DEFAULT        (5)    //CCO���·������ַ֡ʱ��һ�����·�������Ĭ��ֵ
#define MRS_MAC_MAP_SEND_PLC_NUM_MAX            (100)  //CCO���·������ַ֡ʱ��һ�����·����������ֵ
#define MRS_MAC_MAP_SEND_PLC_NUM_MIN            (1)    //CCO���·������ַ֡ʱ��һ�����·���������Сֵ

#define MRS_MAC_MAP_SEND_PLC_RETRY_DEFAULT      (5)    // ���Ͷ�ȡ����ַ֡���Դ���Ĭ��ֵ
#define MRS_MAC_MAP_SEND_PLC_RETRY_MAX          (100)  // ���Ͷ�ȡ����ַ֡���Դ������ֵ
#define MRS_MAC_MAP_SEND_PLC_RETRY_MIN          (0)    // ���Ͷ�ȡ����ַ֡���Դ�����Сֵ

#define MRS_MAX_MAP_CREATE_USED_TIME            (1000 * 60 * 30)    // ӳ�����󴴽�����ʱ��

#define MRS_MAC_MAP_REFRESH_TIME_DEFAULT        (60 * 5)  // CCO������ˢ��ӳ�����ʱ��Ĭ��ֵ(��λS)
#define MRS_MAC_MAP_REFRESH_TIME_MAX            (60 * 60) // CCO������ˢ��ӳ�����ʱ�����ֵ(��λS)
#define MRS_MAC_MAP_REFRESH_TIME_MIN            (10)      // CCO������ˢ��ӳ�����ʱ����Сֵ(��λS)

/* BEGIN: Modified by cuichunyan/00276962, 2016/7/28   ���ⵥ��:   DTS2016072603716  */ 
#define MRS_MAC_MAP_MAX_REFRESH_NUM_DEFAULT     (7)  // CCO������ˢ��ӳ�����ʱ��Ĭ��ֵ(��λS)
/* END:   Modified by cuichunyan/00276962, 2016/7/28  */

#define MRS_MAC_MAP_MAX_REFRESH_NUM_MAX         (100) // CCO������ˢ��ӳ�����ʱ�����ֵ(��λS)
#define MRS_MAC_MAP_MAX_REFRESH_NUM_MIN         (0)      // CCO������ˢ��ӳ�����ʱ����Сֵ(��λS)

#define MRS_MAP_MAINTAIN_MODE_ACTIVE            (1)    // ӳ���ά��ģʽ(����)-�ѱ������
#define MRS_MAP_MAINTAIN_MODE_PASSIVE           (2)    // ӳ���ά��ģʽ(����)-Topo�������
#define MRS_MAP_MAINTAIN_MODE_REFRESH           (3)    // ӳ���ά��ģʽ(����ˢ��)-��ʱ������

#define MRS_MAP_REPORT_IDLE                     (0)
#define MRS_MAP_REPORT_SUCCESS                  (1)
#define MRS_MAP_REPORT_CONTINUE                 (2)

//�����Ϣ
typedef struct _MRS_METER_INFO
{
    HI_U8   ucMeterAddr[HI_METER_ADDR_LEN];   //����ַ,6�ֽ�
    HI_U8   ucProtocol;    //��Լ����   
    HI_U8   ucDeviceType;   //�豸����
    HI_U32  usSendFrame;	// ��ǰ�ڵ�(���)����645֡����
    HI_U32  usRecvFrame;    // ��ǰ�ڵ�(���)����645֡����
} MRS_METER_INFO;


//MAC��ַ��Ϣ
typedef struct _MRS_MAC_INFO
{
    HI_U8 ucMacAddr[HI_PLC_MAC_ADDR_LEN];   //MAC��ַ,6�ֽڣ�������ǰ��
    HI_U8 report;    //�ϱ���־  0-δ�ϱ�   1-���ϱ��ɹ�  2-�����ϱ�,�ǲɼ������ѱ�״̬���ϱ�
    HI_U8 retries;        //���Դ���
} MRS_MAC_INFO;

typedef struct
{
    HI_U8 aucAddr[HI_METER_ADDR_LEN];
    HI_U8 ucProtocol;
    HI_U8 ucType : 4;
	HI_U8 ucStatus : 2; //�ɼ�����ǰ�ѱ�״̬ 0-�ѱ���� 1-�ѱ��� 2-δ֪
    HI_U8 reserved: 2;
} MRS_CCO_DEVICE_INF_STRU;

// MAC-METER
typedef struct _MRS_MAC_DEVICE_PAIR
{
    HI_U16  bValid : 1;
    HI_U16  usTTL  : 6;     /* Mac-meter map TTL. CNcomment: ӳ���ϵTTL. CNend */
    HI_U16  usMrTTL: 6;     /* Meter-reading TTL. CNcomment: ����TTL.     CNend */
    HI_U16  pad    : 3;
    HI_U8   aucMacAddr[HI_PLC_MAC_ADDR_LEN];    // MAC��ַ
    MRS_CCO_DEVICE_INF_STRU     stDeviceInf;        // �豸��Ϣ
} MRS_MAC_DEVICE_PAIR;


//ӳ�����Ϣ
typedef struct _MRS_MAC_DEVICE_MAP
{
    HI_U16 usNumber;              //����
    HI_U16 usMapSize;
    MRS_MAC_DEVICE_PAIR * pstItem;  //����Ԫ��
} MRS_MAC_DEVICE_MAP;


typedef struct _MRS_MAC_LIST
{
    HI_S16 number;
    HI_U16 current;     //��ǰά����MAC
    MRS_MAC_INFO *mac;  //mac�б�
} MRS_MAC_LIST;


//ӳ���ģ��
typedef struct _MRS_MAP_MODULE
{
    HI_BOOL bInit;                       //�Ƿ��ʼ��
    HI_U8 ucState;                      //״̬
    HI_U8 ucChanges;                    //��ǰƵ��TOPO�ı����
    HI_U8 ucMapEnable;                  //CCO��ӳ���ά��ʹ�ܿ���,Ĭ��Ϊ��
    HI_U16 usSendMacPlcTime;            //CCO�˷��Ͷ����ַ֡ʱ����
    HI_U16 usSendLastMacPlcTime;        //CCO�˷������һ��Mac�����ַPLC֡���ʱ����
    HI_U8 ucMaxParallelnum;             // ���Ͷ�ȡ����ַ֡��󲢷���
    HI_U8 ucMaxRetryNum;                // ���Ͷ�ȡ����ַ֡������Դ���
    HI_U16 usCheckTtlTime;              //ˢ��ӳ���TTL��ʱ����,��λΪs
    HI_U32 first_notify;
    HI_U16 map_turn_time;
    HI_U8 max_turn_num;
    HI_U8 curr_turn;
    HI_U16 usMaxTtl;
    HI_U8 ucCltMapEnable;
    HI_U8 ucMaxMrsTtl;
    MRS_MAC_LIST stNewMacList;          // ������MAC�б�

    MRS_MAC_DEVICE_MAP stMacMeterMap;   // ӳ���
    MRS_MAC_DEVICE_MAP stMacCltMap;     // �ɼ���ӳ���,��ucCltMapEnableΪ0ʱ,pstItemΪ��,�õ�ʱҪ�п�
} MRS_MAP_MODULE;


//��ʼ��ӳ���ģ��
HI_U32 mrsCcoMapInit(MRS_MAP_MODULE *pstModule);

//���ʼ��ӳ���ģ��
HI_U32 mrsMapUnInitModule(MRS_MAP_MODULE *pstModule);

//���ӳ������New Mac List
HI_U32 mrsMapEmptyModule(MRS_MAP_MODULE *pstModule);

//����MAC��ַɾ������
HI_U32 mrsMapDeleteMac(MRS_MAP_MODULE *pstModule, HI_U8 *pucMacAddr);

//���New Mac List��MAC�Ѿ��ϱ�������
HI_U32 mrsMapMarkMac(MRS_MAP_MODULE *pstModule, HI_U8 *pucMacAddr, HI_U8 ucStatus);

//��ȡ����ӦMAC
HI_U32 mrsMapGetMAC(HI_U8 *pucDeviceAddr, HI_U8 *pucMacAddr);

//���MAC-METERӳ��(����˵�Ǹ���)
HI_U32 mrsMapAddMeter(MRS_MAP_MODULE *pstModule, HI_U8 *pucMacAddr, MRS_DEVICE_INF_STRU *pstMeterInf);
HI_U32 mrsMapAddCollector(MRS_MAP_MODULE *pstModule, HI_U8 *pucMacAddr, MRS_DEVICE_INF_STRU *pstCltInf);

//TOPO�ı䣬���¼��ӳ���
HI_U32 mrsMapRecheckMacMeter(MRS_MAP_MODULE *pstModule, HI_MAC_NETWORK_TOPO_ENTRY_S * pstEntry, HI_U16 usNum);

//MAPά������
HI_U32 mrsMapMaintain(MRS_MAP_MODULE *pstModule);

//MAP��ʱ������
HI_U32 mrsMapTimeout(MRS_MAP_MODULE *pstModule);

//��ȡӳ���ά��ʹ�ܿ���
HI_VOID mrsGetNvMacMapInfo(MRS_MAP_MODULE *pstModule);

//CCO״̬�ӽ���ӳ���״̬����������״̬�����еĹ�������
HI_VOID mrsCcoMapToNormalPro(HI_VOID);

//ӳ�����TTL��ʱ������
HI_VOID mrsMapTtlTimeoutFunc(MRS_MAP_MODULE *pstModule);

// ӳ�������ֵ��0
HI_VOID mrsMapTtlZero(MRS_MAP_MODULE *pstModule);

// ӳ����ϻ���ͣ
HI_VOID mrsMapAgingPause(HI_VOID);

// ӳ����ϻ��ָ�
HI_VOID mrsMapAgingResume(HI_VOID);

HI_U32 mrsGetDeviceAddr(HI_U8 pucMacAddr[], HI_U8 ucType, HI_U8 pucDeviceAddr[]);

HI_VOID mrsMeterSetDevInf(MRS_DEVICE_INF_STRU *pstDevInf, MRS_METER_INF_STRU *pstMeterInf);

HI_BOOL mrsMapMacIsCollector(HI_U8 ucType);

HI_U32 mrsRmMapProc(MRS_645_FRAME_STRU *pstFrame, MRS_MAP_MODULE *pstModule, HI_U8 *pucMac);

HI_U32 mrsGetMacFromMap(MRS_MAC_DEVICE_MAP *pstMacMap, HI_U8 *pucDeviceAddr, HI_U8 *pucMacAddr);

/**
 @brief  Decrease meter-reading TTL when meter-reading is faield. TTL=0, delete the meter-mac pair.
 @brief  CNcomment: ����ʧ��ʱTTL�ݼ���TTL=0ʱ����ӳ����и�ɾ����Ӧ��ӳ���ϵ��CNend
 @param[in] pstModule      Meter-Mac pair map header pointer.  CNcomment: ӳ���ϵͷָ�롣CNend
 @param[in] pucDeviceAddr  Device address pointer.             CNcomment: �豸��ַָ�롣  CNend
 */
HI_VOID mrsDecreaseMrTTL(MRS_MAP_MODULE *pstModule, HI_U8 *pucDeviceAddr);


HI_END_HEADER

#endif //__MRS_SRV_CCO_MACMAP_H__
