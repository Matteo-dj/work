//*****************************************************************************
//
//                  ��Ȩ���� (C), 1998-2011, ��Ϊ�������޹�˾
//
//*****************************************************************************
//  �� �� ��   : mrs_srv_readmeter_manage.h
//  ��    ��   : liuxipeng/KF54842
//  ��������   : 2014-02-28
//  ��������   : �������ģ���ⲿ�����������궨��
//
//  �����б�   :
//  �޸���ʷ   :
//  1.��    �� : 2014-02-28
//    ��    �� : liuxipeng/KF54842
//    �޸����� : �����ļ�
//
//*****************************************************************************

//*****************************************************************************
// PROJECT   :
// SUBSYSTEM :
// MODULE    :
// OWNER     :
//*****************************************************************************

#ifndef __MRS_SRV_READMETER_MANAGE_H__
#define __MRS_SRV_READMETER_MANAGE_H__

HI_START_HEADER

// ҵ��������Ͷ���
#define MRS_RM_TYPE_AFN02H_F1  (0x0201)    // "����ת��"ҵ�����
#define MRS_RM_TYPE_AFN13H_F1  (0x1301)    // "�㳭"ҵ����� (����ز��ӽڵ�)
#define MRS_RM_TYPE_AFN14H_F1  (0x1401)    // "�ֳ�"ҵ����� (·�����ݳ���)
#define MRS_RM_TYPE_AFNF1H_F1  (0xF101)    // "����"ҵ����� (�Զ���)
#define MRS_RM_TYPE_MAX_NUM    (4)         // ֧�ֵĶ����������ֵ


// ҵ������ش�����
typedef enum
{
    EN_MRS_RM_RETRY_STRATEGY_01 = 0,  // ����1-�����ڵ㳭"AFN=13H-F1"
    EN_MRS_RM_RETRY_STRATEGY_02,      // ����2-�������ֳ�������������ת��
    MRS_RM_RETRY_TACTIC_MAX
} MRS_RM_RETRY_STRATEGY_ENUM;

// �ش���������
typedef enum
{
    EN_MRS_RETRY_CFG_01 = 0,        // "�㳭"-"����ת��"�Ƽ�ʹ��
    EN_MRS_RETRY_CFG_02,            // "�ֳ�"�Ƽ�ʹ��
    EN_MRS_RETRY_CFG_03             // "����"�Ƽ�ʹ��
} MRS_RETRY_CFG_NUM_ENUM;

// ����������
typedef enum
{
    EN_MRS_PARALLEL_POOL_HIGH = 0,  // "�㳭"�����Ƽ�ʹ��
    EN_MRS_PARALLEL_POOL_LOW        // �������������Ƽ�ʹ��
} MRS_PARALLEL_POOL_ENUM;


#define MRS_RM_MAC_MAX_LAYER        (8)     // MAC֧�ֵ����㼶,���Mac���㼶һ��
#define MRS_RM_MAC_MIN_LAYER        (1)     // MAC֧�ֵ���С�㼶,���Mac��С�㼶һ��
#define MRS_RM_DEFAULT_LAYER_BACST  (3)     // �㲥Ĭ�ϲ㼶,���ڼ���㲥��ʱ


// �ش�����Ĭ�ϼ��߽�ֵ����
#define MRS_RM_METER_FRAME_INTERVAL_MAX         (1000 * 10)     // STA��645֡�����ʱ��
#define MRS_RM_METER_FRAME_INTERVAL_DEFAULT     (50)            // STA��645֡Ĭ�ϼ��ʱ��
#define MRS_RM_METER_FRAME_INTERVAL_MIN         (0)             // STA��645֡��С���ʱ��

#define MRS_RM_PLC_LAYER_DELAY_MAX              (10000)
#define MRS_RM_PLC_LAYER_DELAY_DEFAULT          (500)
#define MRS_RM_PLC_LAYER_DELAY_MIN              (0)

#define MRS_RM_PLC_BC_OVERTIME_MAX              (30000)
#define MRS_RM_PLC_BC_OVERTIME_DEFAULT          (MRS_RM_PLC_LAYER_DELAY_DEFAULT * MRS_RM_DEFAULT_LAYER_BACST)
#define MRS_RM_PLC_BC_OVERTIME_MIN              (1000)

#define MRS_RM_PLC_REVISORY_MAX                 (1000 * 10)     // PLCͨѶ��ʱ����ֵ(���)
#define MRS_RM_PLC_REVISORY_DEFAULT             (0)             // PLCͨѶ��ʱ����ֵ(Ĭ��)
#define MRS_RM_PLC_REVISORY_MIN                 (-1000 * 10)    // PLCͨѶ��ʱ����ֵ(��С)

#define MRS_RM_CTRL_CMD_DELAY_MAX               (1000 * 10)     // �ѿ���������ʱʱ��
#define MRS_RM_CTRL_CMD_DELAY_DEFAULT           (2000)          // �ѿ������Ĭ�ϳ�ʱʱ��
#define MRS_RM_CTRL_CMD_DELAY_MIN               (0)             // �ѿ��������С��ʱʱ��

#define MRS_RM_CLTI_DELAY_MAX                   (1000 * 10)     // I�Ͳɼ����������ʱʱ��
#define MRS_RM_CLTI_DELAY_DEFAULT               (1800)          // I�Ͳɼ�������Ĭ�ϳ�ʱʱ��
#define MRS_RM_CLTI_DELAY_MIN                   (0)             // I�Ͳɼ���������С��ʱʱ��

#define MRS_RM_PLC_RETRY_LOOP_MAX               (10)            // [�㳭]PLCͨ������+�㲥��������(���)
#define MRS_RM_PLC_RETRY_LOOP_DEFAULT           (1)             // [�㳭]PLCͨ������+�㲥��������(Ĭ��)
#define MRS_RM_PLC_RETRY_LOOP_MIN               (1)             // [�㳭]PLCͨ������+�㲥��������(��С)

#define MRS_RM_PLC_RETRY_CNT_MAX                (10)            // PLCͨ���������Դ���(���)
#define MRS_RM_PLC_RETRY_CNT_DEFAULT            (2)             // PLCͨ���������Դ���(Ĭ��)
#define MRS_RM_PLC_RETRY_CNT_MIN                (0)             // PLCͨ���������Դ���(��С)

#define MRS_RM_PLC_BC_RETRY_CNT_MAX             (10)            // PLCͨ���㲥���Դ���(���)
#define MRS_RM_PLC_BC_RETRY_CNT_DEFAULT         (1)             // PLCͨ���㲥���Դ���(Ĭ��)
#define MRS_RM_PLC_BC_RETRY_CNT_MIN             (-10)           // PLCͨ���㲥���Դ���(��С)

#define MRS_RM_METER_RETRY_CNT_DEFAULT          (1)             // ���˳�ʱ�ش�����
#define MRS_RM_METER_RETRY_CFG_DEFAULT          (3)             // ���˳�ʱ�ش�����Ĭ��ֵ(Bit0~1; Bit1~1)
#define MRS_RM_MAX_SERVICE_TIME_DEFAULT         (60)            // [�㳭]���γ���������ʱ��Ĭ��ֵ ��λ:S
#define MRS_RM_MAX_SERVICE_TIME_MIN             (3)             // [�㳭]���γ���������ʱ��(��С) ��λ:S
#define MRS_RM_MAX_SERVICE_TIME_SUB             (2)             // [�㳭]���γ���������ʱ��ʹ��ʱӦ��ȥ��ʱ�� ��λ:S

#define MRS_RM_CSG_MAX_SERVICE_TIME_DEFAULT     (20)            // �����ȼ����γ���������ʱ��Ĭ��ֵ ��λ:S

// ��������(Ӧ��)�ṹ
typedef struct
{
    HI_U8 * ucAddr;     // Ŀ�ĵ���ַ
    HI_U16  usRmType;   // ҵ���������
    HI_U8   ucProtocol; // ����ת��֡��Լ����
    HI_U8   ucMrSeq;    // ҵ�����376.2֡���к�
    HI_U16  usDataLen;  // ҵ���������ת������
    HI_U8   reserved[2];
    HI_U8 * pucData;    // ҵ���������ת������

#if defined(PRODUCT_CFG_PRODUCT_TYPE_CSG_GD)
    HI_VOID* pvParam;
#endif
} MRS_RM_REQ_DATA, *P_MRS_RM_REQ_DATA, MRS_RM_RESP_DATA, *P_MRS_RM_RESP_DATA;

/* BEGIN: PN: DTS2015033100426 MODIFY\ADD\DEL by cuiate/00233580 at 2015/3/27 */
typedef NV_MRS_RETRY_CFG_S      MRS_RETRY_CFG_S, *P_MRS_RETRY_CFG_S;
typedef NV_MRS_PARALLEL_CFG_S   MRS_PARALLEL_CFG_S, *P_MRS_PARALLEL_CFG_S, *P_NV_MRS_PARALLEL_CFG_S;
/* END:   PN: DTS2015033100426 MODIFY\ADD\DEL by cuiate/00233580 at 2015/3/27 */


// ��������Ӧ����ص���������
typedef HI_U32 (* MRS_RM_DATA_RESP_PROC_F)(P_MRS_RM_RESP_DATA);


// �������ģ���ʼ��
HI_PUBLIC HI_U32 mrsRmModuleInit(HI_VOID);


// ҵ��������ע��
HI_PUBLIC HI_U32 mrsRmRegisterStrategy(HI_U16, MRS_RETRY_CFG_NUM_ENUM, MRS_RM_RETRY_STRATEGY_ENUM,
                                            MRS_PARALLEL_POOL_ENUM, MRS_RM_DATA_RESP_PROC_F);

// ����������뻺���
HI_PUBLIC HI_U32 mrsRmReqJoinPool(P_MRS_RM_REQ_DATA);

// PLC����������
HI_PUBLIC HI_U32 mrsRmPlcSendReqProc(HI_SYS_QUEUE_MSG_S *);

#if defined(PRODUCT_CFG_PRODUCT_TYPE_CSG_GD)
HI_PUBLIC HI_U32 mrsRmPlcPackageReceiveCsg(HI_U8 *pucPayload, HI_U16 usPayloadLen);
HI_PUBLIC HI_VOID mrsRmRemoveItemFromPool(HI_VOID *pParam);
#else
// PLC������
HI_PUBLIC HI_U32 mrsRmPlcPackageReceive(MRS_DATA_TRANSMIT_AFNF1_UP_STRU *, HI_U16);
#endif

// ����ģ�鳬ʱ��鶨ʱ������
HI_PUBLIC HI_VOID mrsRmCheckTimerProc(HI_VOID);
HI_PUBLIC HI_U16 mrsGetNvParallelNum(HI_VOID);

// ��λ�������ģ��(�������·�AFN=12-F1·������ʱ����)
HI_PUBLIC HI_VOID mrsRmModuleReset(HI_VOID);

/* BEGIN: PN: DTS2015033100426 MODIFY\ADD\DEL by cuiate/00233580 at 2015/3/26 */
HI_PUBLIC HI_VOID mrsRmCfgInit(NV_MRS_RETRY_CFG_S *);
HI_PUBLIC HI_VOID mrsRmCfgParallelInit(NV_MRS_PARALLEL_CFG_S *);
HI_PUBLIC HI_VOID mrsRmRetryCfgInit(P_MRS_RETRY_CFG_S, NV_MRS_RETRY_CFG_S *);
/* END:   PN: DTS2015033100426 MODIFY\ADD\DEL by cuiate/00233580 at 2015/3/26 */
HI_PUBLIC HI_U32 mrsGetMacFromTopo(HI_U8 *pucMeterAddr, HI_U8 *pucMacAddr);
HI_PUBLIC HI_VOID mrsSetParallelNum(HI_U16 usNum);
HI_PUBLIC HI_U16 mrsGetParallelNum(HI_VOID);

HI_END_HEADER

#endif // __MRS_SRV_READMETER_MANAGE_H__


