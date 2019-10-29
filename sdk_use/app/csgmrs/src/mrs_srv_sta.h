//*****************************************************************************
//
//                  ��Ȩ���� (C), 1998-2011, ��Ϊ�������޹�˾
//
//*****************************************************************************
//  �� �� ��   : mrs_srv_sta.h
//  �� �� ��   : V1.0
//  ��    ��   : shenhankun/00130424
//  ��������   : 2011-11-08
//  ��������   : TODO: ...
//
//  �����б�   : TODO: ...
//  �޸���ʷ   :
//  1.��    �� : 2011-11-08
//    ��    �� : shenhankun/00130424
//    �޸����� : �����ļ�
//
//*****************************************************************************
#ifndef __MRS_SERVICE_STA_H__
#define __MRS_SERVICE_STA_H__

#include "mrs_srv_sta_queue.h"
#include "mrs_srv_parallel_sta.h"

HI_START_HEADER
//STA���������ϱ�������Ӧ�ϱ�
//*****************************************************************************
//STA_PRM_STA_CCO: STA�����ϱ���STAΪ������
//STA_PRM_CCO_STA: STAΪ������Ӧ�ϱ���STAΪ�Ӷ���
//*****************************************************************************
#define STA_PRM_STA_CCO     1
#define STA_PRM_CCO_STA     0

#define  METER_CNF_ADDR     0x13

/* BEGIN: Added by fengxiaomin/00209182, 2014/2/19   ���ⵥ��:DTS2014012301705 */
#define STA_MAC_ADDRESS_END_BYTE  0xFE
/* END:   Added by fengxiaomin/00209182, 2014/2/19 */
#define MRS_CSG_PHYSICAL_TEST_MODE_LISTEN    (10)

//*****************************************************************************
//METER_CNF_ADDR   :  ����ַ��Ӧ������
//*****************************************************************************
#define  MRS_STA_REPORT_METER_ADDR_RESPOND   0  //��Ӧcco���·��Ķ����ַ֡��־
#define  MRS_STA_REPORT_METER_ADDR_ACTIVE    1  //�����ϱ����ַ��mac��ַ��־
#define  MRS_STA_REPORT_METER_ADDR_CHANGE    2  //��STA�˱��ַ�ı�ʱ�����ϱ����ַ��mac��ַ��־
#define  MRS_STA_REPORT_METER_INFO_VALID     1  //STA�������ϱ����ַ��Ч
#define  MRS_STA_REPORT_METER_INFO_INVALID   0  //STA�������ϱ����ַʧЧ

#define  MRS_STA_LOCAL_MAP_ENTRY_TTL_MAX     2048   // STA����ӳ�����б�ÿ��TTL���ֵ

#define MRS_STA_RM_CFG_LOCAL    (0)     /* Local, STA/CltII <--> Meter, CltI Module <--> CltI  CNcomment: ����: STA/II������֮���I��ģ����I��֮�䡣CNend */
#define MRS_STA_RM_CFG_CLTI     (1)     /* CltI transfer, CltI Module <--> CltI <--> Meter  CNcomment: I��ת��: I��ģ��ͨ��I��ת����������Ĭ�ϲ�����CNend */
#define MRS_STA_RM_CFG_AUTO     (0xFF)  /* Auto mode, choose 'Local' or 'CltI transfer' mode by product type  CNcomment: �Զ������ݲ�Ʒ��̬�Զ�ѡ��CNend */

#define MRS_CHECK_LEAVE_TIME_LEN_TH_VALID(leave_time_len) ((leave_time_len) > 0)

//���ַ�б���
/*********************************
  7   6   5   4   3   2   1   0
|<--��Ч��־--> |<--��Լ����-->|
*********************************/
typedef struct
{
    HI_U8  ucValidFlag:4;                   // ��Ч��־
    HI_U8  ucProtocol:4;                    // ��Լ����
    HI_U8  padding;
    HI_U16 usOption;                        // TTL/DI
    HI_U8  ucMeterAddr[HI_METER_ADDR_LEN];  // ���ַ
    HI_U16 usBaudRate;                      // ���ͨ������
}MRS_METER_ITEM_S;

//���ַ�б�
typedef struct
{
    HI_U8 ucMeterNum;               // ������
    HI_U8 ucType;                   // ģ������ (MRS_STA_TYPE_METER �������; MRS_STA_TYPE_COLLECTOR �ɼ�������)
    HI_U8 padding[2];
    MRS_METER_ITEM_S stNodeMeter;   // �����ַ��Ҳ�п���û�У�
    MRS_METER_ITEM_S stMeterItem[PRODUCT_CFG_MRS_MAX_METER_IN_MAC_NUM];
}MRS_METER_LIST_S;

#if defined(PRODUCT_CFG_SYS_RST_SOLUTION)
typedef struct
{
    HI_U32 ulRxUartTimes;   //STA���յ�����Ĵ���
    HI_U32 ulRxRMPlcTimes;     //STA/II�ɽ��ճ���PLC֡����
}MRS_STA_RESET_PARAM_S;//STA������λ��ز�����Ϣ
#endif


// �˴����ݻ������еı��������⴦��:
// 13/14���¼���ѯ����: �ڻ������л����ظ��ı���
// �㲥����: ����CCO�·��ı���
// �Ƚ�ʱ����id��seq�Ƚϣ��������ͬ�ı��ģ���ֱ�ӷ��س������(13/14���¼���ѯ)�򲻴���(�㲥����)

typedef struct
{
    HI_U32 id;          // PLCͨ��ID
    HI_U16 seq;         // �������
    HI_U16 frame_len;   // ���ĳ���
    HI_U8 valid;        // ��Ч��־ 0:��Ч 1:��Ч
    HI_U8 protocol;     // ��Լ����
    HI_U16 padding;
    HI_U32 timestamp;   // ʱ���
    HI_U8 frame_buf[MRS_STA_FRAME_BUF_ITEM_BUFSIZE];
} MRS_STA_FRAME_BUF_ITEM_STRU;

typedef struct
{
    HI_U32 max_count;                   // �洢���ĵ��������
    HI_U32 current;                     // ��ǰ����λ��
    HI_U32 timeout;                     // ������Ч��
    MRS_STA_FRAME_BUF_ITEM_STRU * item; // �洢���ĵĽṹ
} MRS_STA_FRAME_BUF_CTX;

typedef struct
{
    HI_BOOL bValid;
    HI_U8 aucAddr[HI_METER_ADDR_LEN];
    HI_U8 ucProtocol;
    HI_U16 us97DI;
    HI_U16 usId;
}MRS_STA_RM_METER_INFO;

typedef NV_MRS_STA_RM_CFG_STRU MRS_STA_RM_CFG_STRU;

//STA���е�������
typedef struct
{
    HI_U8   ucState;            // STA����״̬

    HI_U8   macState:1;         // mac��״̬
    HI_U8   plcState:1;         // plc��״̬
    HI_U8   acdstate:1;         // ACD״̬
    HI_U8   IsOldFrame:1;       // ����Ԫ���¾ɱ�־
    HI_U8   staRepFlag:1;       // STA�������ϱ����ַ��ά��ӳ����أ�Ĭ��Ϊ�ر�
    HI_U8   meterChangeFlag:1;  // STA�˱��ַ�ı��,�ϱ�CCO����  0:������  1:����    Ĭ��Ϊ??????

    HI_U8   sync_mac_flg : 1;   // ͬ��mac��ַ��־(�ϵ��ֻͬ��һ��):��־ΪFALSEʱ����ͬ��,ͬ����ɺ���ΪTRUE
                                // �ϵ��ȡ����ַ��ʱ�󽫱�־��ΪTRUE,����������mac��ַͬ��
    HI_U8   ucDutFlag : 1;
    HI_U8   reserved;

    HI_U8   ucSeq;

/* BEGIN: Added by fengxiaomin/00209182, 2015/10/23   ���ⵥ�ź�:DTS2015102710230 */
    HI_U8   aucMeter[HI_METER_ADDR_LEN];  // ����ַ
    HI_U8   ucProtocol;                   // ��Լ����
/* END:   Added by fengxiaomin/00209182, 2015/10/23 */

    HI_U8   plc_rxtx_mode;      // PLCͨ���շ�ģʽ 00:�ȴ�����·���Ż���ɺ�Ž���PLCͨ�����շ�; 01:ǿ��PLCͨ���շ�(���ȴ�PLC״̬);

    HI_U32  addr_Fail_wait_time;// ��ȡ����ַʧ�ܺ�ȴ�ʱ�� ��λ:S
    HI_U8   addr_try_num;       // ��ȡ����ַ���Դ���
    HI_U8   sta_type;           // STA ����: 0 �Ƿ���1 ���2 I�Ͳɼ���

    HI_U8   ucMacAddr[HI_PLC_MAC_ADDR_LEN];

    HI_U16  detect_timeout;                  // STA�ϵ�����: ̽�Ⲩ���ʳ�ʱʱ�� ��λ:����
    HI_U16  detect_period;                   // STA�ϵ�����: һ��̽��ѭ���������ٴο�ʼѭ���ĵȴ�ʱ�� ��λ:��
    HI_U8   detect_loop_max;                 // STA�ϵ�����: ̽�Ⲩ�����ִ�
    HI_U8   read_positive_A_times;           // I���ϵ�����: ����NV�е�������й����ܴ���
    HI_U8   read_positive_A_interval;        // I���ϵ�����: ��ѯI�Ͳɼ�����1���ѱ���ʱ���� ��λ:��
    HI_U8   query_sm_interval;               // I���ѱ�: �����в�ѯ�ѱ���ʱ���� ��λ:��
    HI_BOOL is_real_clti;                    // �Ƿ�Ϊ������I��: ���ѱ��ܺ����ѱ��ܶ���������I�ɣ�̽�Ⲩ����ʧ�ܲ�������I��

    HI_U8   ucSoftTfMode;       // ����̨��ʶ�����ܿ���
    HI_U16  usBindDuration;     // ��CCO Neteork�ĳ���ʱ�䣬��λ�����ӣ����45.5�죻ȡֵ0xFFFFʱ����ʾ����������ʵ������136�꣨U32��ʾ�����������

    HI_U16  usLeaveTimeLenTh;   // ����ʱ�����ޣ���λmin
    HI_U8   ucStaRstPinEnable;  // SAT��RST�ܽ�ʹ��
    HI_U8   pad;

    HI_U32  ulRstShielTime;      // ����RST�ܽ�ʱ������λs

#if defined(PRODUCT_CFG_SYS_RST_SOLUTION)
    MRS_STA_RESET_PARAM_S stStaResetInfo;  //STA�˸�λ�����Ϣ
#endif

    MRS_METER_LIST_S  stMeterList;      // ����ַ
    MRS_QUE_MODULE    stQueModule;      // �������ģ��

    MRS_PARALLEL_STA_CTX stParallelCtx; // ��������������

    MRS_STA_FRAME_BUF_CTX stFrameBufCtx;// ֡�����������(���˻�����)
    MRS_STA_RM_METER_INFO stRmMeterInfo;

    MRS_STA_RM_CFG_STRU stRmCfg;    /* Parameters of queue and meter-reading. CNcomment: ���кͳ������. CNend */

#if defined(PRODUCT_CFG_PRODUCT_TYPE_CSG_GD)
    HI_U32 (*pfnPlcAckProc)(MRS_CSG_PLC_FRAME_HEAD*);
    HI_U32 (*pfnPlcDenyProc)(MRS_CSG_PLC_FRAME_HEAD*);

    HI_U16 usPlcSeq;
    HI_U16 reserved3;
#endif
} MRS_STA_SRV_CTX_STRU;

//��ȡ����������
HI_PUBLIC MRS_STA_SRV_CTX_STRU * mrsStaGetContext(HI_VOID);
HI_PUBLIC MRS_STA_SRV_CTX_STRU * mrsStaSaveContext(MRS_STA_SRV_CTX_STRU *x);
// ��ȡSTA�˲�������ȫ��������
HI_PUBLIC MRS_PARALLEL_STA_CTX * mrsStaGetParallelCtx(HI_VOID);
//��ȡMAC��ַ

HI_PUBLIC HI_U8 * mrsStaGetMacAddr(HI_U8 macAddr[6]);



// ���õ���ַ��Mac��
HI_U32 mrsStaUpdateTopAddrInf(HI_IN HI_U8 ucAddr[HI_METER_ADDR_LEN], HI_IN HI_U8 ucProtocol);
// ������ַת��������Ϊ��̫����ַ
HI_U32 mrsSetEthIp(HI_U8 maddr[6]);
// ����STA��ģ��Mac��ַ
HI_U8 * mrsStaSetMacAddr(HI_U8 macAddr[6]);
/* BEGIN: PN: DTS2015051601564 MODIFY\ADD\DEL by cuiate/00233580 at 2015/5/15 */
HI_U32 mrsStaUpdateAddr(HI_U8 ucMeterAddr[HI_METER_ADDR_LEN], HI_U8 ucProtocol);
HI_U32 mrsStaUpdateAddrProc(HI_U8 ucMeterAddr[HI_METER_ADDR_LEN], HI_U8 ucProtocol, HI_U16 usDi97);
/* END:   PN: DTS2015051601564 MODIFY\ADD\DEL by cuiate/00233580 at 2015/5/15 */

/**
 @brief     Add meter to local meterlist.  CNcomment: ��ӵ�������б�CNend
 @param[in] pstItem  meter information pointer.  CNcomment: �����Ϣָ�롣CNend
 */
HI_VOID mrsStaAddMeter2LocalList(MRS_METER_ITEM_S *pstItem);

// ���ұ��ַ�б�-�ҵ���ͬ�ĵ�ַ����HI_TRUE�������Ӧ�������±�
HI_BOOL mrsFindMeterList(MRS_METER_LIST_S * pMeterList,
                            HI_U8 addr[HI_METER_ADDR_LEN], HI_U16 * index);

// ��ȡCCO֡�¾ɱ�־
HI_PUBLIC HI_BOOL mrsIsOldFrame(HI_VOID);

// ����CCO֡�¾ɱ�־
HI_PUBLIC HI_VOID mrsSetFrameFlg(HI_BOOL flg);

//*****************************************************************************
// ��������: mrsGetRepFlagNv
// ��������: ��ȡ�����ϱ����ַ����---NV��
//
// ����˵��:
//   HI_VOID
//
// �� �� ֵ:
//    HI_VOID
//
// ����Ҫ��: TODO: ...
// ���þ���: TODO: ...
// ��    ��: fengxiaomin/00209182 [2013-02-21]
//*****************************************************************************
HI_VOID mrsGetRepFlagNv(HI_VOID);


// �� �� ֵ: STA���յ����ͳ����
//
// ����Ҫ��:
// ���þ���:
// ��    ��: fengxiaomin/00209182 [2013-03-02]
//*****************************************************************************
#if defined(PRODUCT_CFG_SYS_RST_SOLUTION)
HI_U32 mrsStaRxUartStat(HI_VOID);
HI_U32 mrsRxRMPlcStat(HI_VOID);
#endif

HI_U32 mrsDutUartDataPro(MRS_645_FRAME_STRU *pst645Hrd);
HI_BOOL mrsStaPlcRetryFiltrate(MRS_STA_ITEM * item);
HI_U32 mrsStaFrameBufInsert(HI_U32 id, HI_U16 seq, HI_U16 frame_len, HI_U8 * frame_data, HI_U8 protocol);
HI_U32 mrsStaFrameBufFind(HI_U32 id, HI_U16 seq, HI_U16 *frame_len, HI_U8 ** frame_data, HI_U8 *protocol);
HI_U8* mrsGetSendAddrFrame(HI_U16 * len);

HI_BOOL mrsGetSyncMacFlg(HI_VOID);
HI_VOID mrsSetSyncMacFlg(HI_BOOL flg);
HI_VOID mrsStaPowerOnPro(HI_VOID);
HI_U32 mrsSetCollectDevAddr(HI_U16 usId);

// ˢ���׽ڵ���Ϣ
HI_VOID mrsStaRefreshTopNodeInf(HI_U8 *, HI_U8 *);

// ���ñ��ַ��Mac�㲢���²�Ʒ����
HI_U32 mrsStaSetMeterToMac(HI_U8 *, HI_U8, HI_U8);

// ͬ��Mac��������ַ
HI_U32 mrsStaSyncMacJoinNetAddr(HI_VOID);

// ���ѱ����б��л�ȡָ�����ַ��Ϣ
MRS_METER_ITEM_S *mrsStaGetMeterInfFromSmResult(HI_U8 *);

// �洢�������ַ��Nv��
HI_VOID mrsStaSaveJoinMeterAddr(HI_U8 *, HI_U8, HI_U16);


HI_VOID mrsGetStaMrParam(NV_APP_MR_CFG_STRU* cfg);

HI_U32 mrsStaTransmitHsoCmd(HI_U16 , HI_PVOID , HI_U16 , HI_U32 );
HI_U32 mrsStaTransmitRemoteCmd(HI_U16 , HI_PVOID , HI_U16 , HI_U32 );
// ����STA��Ʒ����(��ͨ���ģ��:I�Ͳɼ���ģ��)
HI_VOID mrsStaSetType(HI_U8);
// ��ȡSTA��Ʒ����
HI_U8 mrsStaGetType(HI_VOID);
// ��ȡ��ǰPLCͨ��״̬
HI_BOOL mrsStaGetPlcChlStatus(HI_VOID);

HI_U32 mrsStaSerialRespRMPLC(MRS_STA_ITEM *pstItem, MRS_MSG_MR_645_DATA_STRU* pstData);

//��645֡����ת��CCO
HI_U32 mrsStaTransmit2CCO(HI_U8 * data, HI_U16 len, HI_U8 protocol, MRS_STA_ITEM * item);

// �Ƿ����������
HI_BOOL mrsStaBindNetworkEnable(HI_VOID);

// ��������
HI_U32 mrsStaBindNetwork(HI_VOID);

// ���: ���ڴ�����ʱ���ڣ�����ʱ��ռ�ȳ������ޣ���������
HI_U32 mrsNetworkCheckTimeout(HI_VOID);
// ���: ������ʱ�Σ�����ʱ���Ƿ�ﵽ����
HI_BOOL mrsCheckReachLeaveThInLockNet(HI_MDM_LOCK_INFO_STRU * stLockInfo);
// �����ɹ�����������
HI_U32 mrsRejoinNetworkAfterUnlock(HI_VOID);

//��645֡ת����HSO
HI_U32 mrsStaDiag2HSO(HI_U8 * data, HI_U16 len,HI_U8 histance);

// Return the specified meter information of the local meter list. CNcomment: ���ص�ַ�б��л�ȡָ�����ַ��Ϣ. CNend
// pucMeter - Meter address
MRS_METER_ITEM_S * mrsStaGetLocalMeterInf(HI_U8 *pucMeter);

// Return 645 frame head timeout by type.
// ucType : MRS_STA_RM_CFG_LOCAL/MRS_STA_RM_CFG_CLTI/MRS_STA_RM_CFG_AUTO
HI_U16 mrsGet645FrameHeadTimeout(HI_U8 ucType);

// Return 645 frame interval timeout by type.
// ucType : MRS_STA_RM_CFG_LOCAL/MRS_STA_RM_CFG_CLTI/MRS_STA_RM_CFG_AUTO
HI_U16 mrsGet645FrameInterval(HI_U8 ucType);

// Return parallel-reading 645 interval timeout by type.
// ucType : MRS_STA_RM_CFG_LOCAL/MRS_STA_RM_CFG_CLTI/MRS_STA_RM_CFG_AUTO
HI_U16 mrsGet645MeterInterval(HI_U8 ucType);

// Return 645 CTRL frame correction by type.
// ucType : MRS_STA_RM_CFG_LOCAL/MRS_STA_RM_CFG_CLTI/MRS_STA_RM_CFG_AUTO
HI_U16 mrsGet645CtrlCmdDelay(HI_U8 ucType);

// Return STA queue max length.
HI_U8 mrsGetStaQueueMax(HI_VOID);

// Compare Dl & Ul frame, check wheher Ul frame match DL frame or not.
HI_BOOL mrsStaIsMr645FrameMatch(HI_U8 * pDlFrame, HI_U16 usDlLength, HI_U8 * pUlFrame, HI_U16 usUlLength);



HI_END_HEADER
#endif // __MRS_SERVICE_STA_H__
