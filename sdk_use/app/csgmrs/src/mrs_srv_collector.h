//*****************************************************************************
//
//                  ��Ȩ���� (C), 1998-2011, ��Ϊ�������޹�˾
//
//*****************************************************************************
//  �� �� ��   : mrs_srv_collector.h
//  �� �� ��   : V2.0
//  ��    ��   : fengxiaomin/f00209182
//  ��������   : 2012-08-28
//  ��������   : TODO: ...
//
//  �����б�   : TODO: ...
//  �޸���ʷ   :
//  1.��    �� : 2012-08-28
//    ��    �� : fengxiaomin/f00209182
//    �޸����� : �����ļ�
//
//*****************************************************************************
#ifndef __MRS_SRV_COLLECTOR_H__
#define __MRS_SRV_COLLECTOR_H__
HI_START_HEADER


#define MRS_COLLECTOR_SEND_BOARD_INTERVAL_TIME    200  //�㲥Уʱ��2400������ת�����ٴ���1200������ת���ĳ�ʱʱ��
#define MRS_COLLECTOR_SEND_BOARD_END_TIME         1000  //�㲥Уʱ��1200������ת��������ʱʱ��

#define LED_RUN_STATUS_TURN_ON_OFF_TIME    1000      //���е���/���ʱ��
#define LED_RTX_TURN_ON_OFF_TIME    100      //���е���/���ʱ��
//#define LED_UART_RTX            HI_LED_NUM4   //485��������ͨ��
#define LED_RUN_STATUS          HI_LED_IDX_2   //���е�,��ʾ�ɼ�����������
//#define LED_PLC_TX      HI_LED0
//#define LED_PLC_RX      HI_LED1
#define MRS_COLLECTOR_97OR07_CONTROL_FLAG    0x10  //������97��Լ����07��Լʱ�������жϲ���

#define MRS_CLTII_TIMING_645_CTRL              (0x33)
#define MRS_CLTII_TIMING_645_UP_CTRL_INCREAZE  (0x80)
#define MRS_CLTII_TIMING_645_CTRL_MASK         (0x07)
#define MRS_CLTII_TIMING_645_CTRL_LEN          (2)
#define MRS_CLTII_TIMING_645_TRANSFER_OFFSET   (12)


typedef enum
{
    MRS_COLLECTOR_RUNLED_0_25HZ,
    MRS_COLLECTOR_RUNLED_0_50HZ,
    MRS_COLLECTOR_RUNLED_5_00HZ
} MRS_COLLECTOR_RUNLED_FREQ_E;

#define MRS_COLLECTOR_RUN_LED_NORMAL        (MRS_COLLECTOR_RUNLED_0_50HZ)
#define MRS_COLLECTOR_RUN_LED_SEARCHMETER   (MRS_COLLECTOR_RUNLED_5_00HZ)
#define MRS_COLLECTOR_RUN_LED_NO_METER      (MRS_COLLECTOR_RUNLED_0_25HZ)


typedef struct
{
    HI_U8 ucProtoType;
    HI_U8 ucTimingCnt;    //�㲥Уʱ��ͬ�����ʷ��ͱ�ʶ 0-Ӧ������2400������ 1-Ӧ������1200������
    HI_U8 ucRunLedStatus; //���е�״̬ 0-ĿǰΪ��Ӧ���� 1-ĿǰΪ����ӦϨ��
    HI_U8 ucUartLedStatus; //�����շ���״̬ 0-ĿǰΪ��Ӧ���� 1-ĿǰΪ����ӦϨ��
    HI_U32 ulTimingSendInterval;    // �㲥Уʱ֡���ͼ�� ��λ: ����
    HI_U8 ucBaudRateIndex;
/* BEGIN: PN:  MODIFY\ADD\DEL by cuichunyan/00276962 at 2015/10/21 */
    HI_U8 ucIrTrasmit645;//645͸������
/* END: PN:  MODIFY\ADD\DEL by cuichunyan/00276962 at 2015/10/21 */
    HI_U8 aucMeter[HI_METER_ADDR_LEN];
    HI_U8 aucDevice[HI_METER_ADDR_LEN];
    HI_U8 ucPowerSearchEnable;
    HI_U8 reserved;
} MRS_COLLECTOR_CTX_STRU;


// ��һ�ι㲥��������Ϣ��ֻ��¼����ʧ��ʱ��Ϣ��
typedef struct
{
    HI_U8 bValid : 1;
    HI_U8 ucIndex : 7;
    HI_U8 ucProtocol : 4;
    HI_U8 ucCount : 4;
    HI_U8 aucMeter[HI_METER_ADDR_LEN];
} MRS_CLTII_LAST_BC_METER_INF;


MRS_COLLECTOR_CTX_STRU *mrsGetCollectorCtx(HI_VOID);

//*****************************************************************************
// ��������: mrsCollectorInit
// ��������: II�Ͳɼ���ģ���ʼ��
//
// ����˵��:
//
// �� �� ֵ:
//
// ����Ҫ��:
// ���þ���:
// ��    ��: fengxiaomin/f00209182 [2012-08-28]
//*****************************************************************************
HI_VOID mrsCollectorInit(HI_VOID);

//*****************************************************************************
// ��������: mrsSendMrData
// ��������: ���Ͷ������ݸ����
//
// ����˵��:
//   HI_U8 type    �ز�ģ������-STA/II�Ͳɼ���
//   HI_U8 *data   645����֡
//   HI_U16 len    645����֡����
//
// �� �� ֵ:
//    �ɹ�����  HI_ERR_SUCCESS
//
// ����Ҫ��:
// ���þ���:
// ��    ��: fengxiaomin/f00209182 [2012-08-28]
//*****************************************************************************
HI_U32 mrsSendData2Meter(HI_U8 *pData, HI_U16 usLen, HI_U8 *ucRetry, HI_U32 over_time);


//*****************************************************************************
// ��������: mrsCollectorSendMrPro
// ��������: II�Ͳɼ������Ͷ�������ǰ�Ĵ�����
//
// ����˵��:
//   HI_U8 *data   645����֡
//
// �� �� ֵ:
//    �ɹ�����  HI_ERR_SUCCESS
//
// ����Ҫ��:
// ���þ���:
// ��    ��: fengxiaomin/f00209182 [2012-08-28]
//*****************************************************************************
HI_VOID mrsCollectorSendMrPro(HI_U8 *data, HI_U16 usDataLen);


//*****************************************************************************
// ��������: mrsBoardTimingSend
// ��������: ת��II�Ͳɼ������յĹ㲥Уʱ֡
//
// ����˵��:
//   HI_U8 *pTimingData    II�Ͳɼ������յĹ㲥Уʱ֡
//   HI_U16 usDataLen     II�Ͳɼ������յĹ㲥Уʱ֡����
//   HI_U8 *ucRetry       ת�����Դ���
//
// �� �� ֵ:
//    �ɹ�����  HI_ERR_SUCCESS
//
// ����Ҫ��:
// ���þ���:
// ��    ��: fengxiaomin/f00209182 [2012-08-28]
//*****************************************************************************
HI_U32 mrsBoardTimingSendPro(HI_U8 *pTimingData, HI_U16 usDataLen, HI_U8 *pucRetry);

//*****************************************************************************
// ��������: mrsRunLedPro
// ��������: ʹ���е�LED������˸
//
// ����˵��:
//    ucFirstFlag   �����״��ϵ��ִ�л�����������֮��ִ��
// �� �� ֵ:
//    �ɹ�����  HI_ERR_SUCCESS
//
// ����Ҫ��:
// ���þ���:
// ��    ��: fengxiaomin/f00209182 [2012-08-28]
//*****************************************************************************
HI_VOID mrsRunLedPro(HI_U8 ucFirstFlag);

//*****************************************************************************
// ��������: mrsUARTRTxLedPro
// ��������: 485��������ͨ��ʱʹ��LED����˸
//
// ����˵��:
//
// �� �� ֵ:
//    �ɹ�����  HI_ERR_SUCCESS
//
// ����Ҫ��:
// ���þ���:
// ��    ��: fengxiaomin/f00209182 [2012-08-28]
//*****************************************************************************
HI_VOID mrsUARTRTxLedPro(HI_VOID);

//*****************************************************************************
// ��������: mrsCollectorSendMrData
// ��������: II�ɷ��ͳ������ݸ����
//
// ����˵��:
//
// �� �� ֵ:
//    �ɹ�����  HI_ERR_SUCCESS
//
// ����Ҫ��:
// ���þ���:
// ��    ��: fengxiaomin/f00209182 [2013-07-18]
//*****************************************************************************
HI_U32 mrsCollectorSendMrData(HI_U8 *pCollectorData, HI_U16 usMrDataLen, HI_U32 over_time);


HI_VOID mrsCollectorSetRunLedTime(MRS_COLLECTOR_RUNLED_FREQ_E led_freq);

HI_U32 mrsCltiiTimingPreProc(MRS_PLC_FRAME_DATA_STRU *pstFrame, HI_PVOID *pData, HI_U16 *pusLen, HI_BOOL *bFlag);

HI_VOID mrsCltiiSetBaudRateIndex(HI_U8 ucIndex);

HI_U8 mrsCltiiGetBaudRateIndex(HI_VOID);

HI_BOOL mrsIsCltiiTiming(HI_VOID);

HI_U32 mrsCreateTimingUpFrame(HI_U8 **ppData, HI_U16 *pusDataLen, MRS_MSG_MR_645_DATA_STRU* pstData);

HI_U32 mrsCltiiTimingTimeOutProc(MRS_STA_ITEM *pstItem);

HI_U32 mrsCltiiGetMeterProtocol(HI_U8 pucMeter[], HI_U8 *pucProtocol);

HI_U32 mrsCltiiTimingProc(HI_U8 *pData, HI_U8 *pFrame, HI_U8 *pucRetry, HI_U32 ulTimeout);

HI_VOID mrsSetCollectorAddr(HI_U8 *pucDeviceAddr);

HI_U8 *mrsGetCollectorAddr(HI_VOID);

HI_U32 mrsCltiiProtoProc(HI_U8 *pData, HI_U16 usDataLen, HI_U16 usId, HI_U16 usSeq);

HI_U32 mrsCltiiCheckAddr(HI_U8 *pAddr);

HI_U32 mrsCltiiCheckMeterAddr(HI_U8 *pAddr);


MRS_CLTII_LAST_BC_METER_INF *mrsCltiiGetLastBcMeterInf(HI_VOID);

HI_VOID mrsCltiiClearLastBcMeterInf(HI_VOID);


HI_END_HEADER
#endif //__MRS_SRV_COLLECTOR_H__

