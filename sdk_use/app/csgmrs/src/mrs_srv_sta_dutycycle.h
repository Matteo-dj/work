//*****************************************************************************
//
//                  ��Ȩ���� (C), 1998-2015, ��Ϊ�������޹�˾
//
//*****************************************************************************
//  �� �� ��   : mrs_srv_sta_dutycycle.h
//  �� �� ��   : V1.0 
//  ��    ��   : cuiate/00233580
//  ��������   : 2015-08-27
//  ��������   : STAռ�ձȲ���
//               
//  �����б�   : TODO: ...
//  �޸���ʷ   : 
//  1.��    �� : 2015-08-27
//    ��    �� : cuiate/00233580
//    �޸����� : �����ļ� 
//
//*****************************************************************************

#ifndef _MRS_SRV_STA_DUTYCYCLE_H_
#define _MRS_SRV_STA_DUTYCYCLE_H_

HI_START_HEADER
#if defined(PRODUCT_CFG_PRODUCT_TYPE_STA)
//*****************************************************************************
//*****************************************************************************

#define MRS_STA_DUTY_CYCLE_CFG_ITEM_MAX     (6)
#define MRS_STA_DUTY_CYCLE_RESET_TIME_MAX   (64)

#define MRS_STA_NV_RESET_TIME_NUM           (32)

// [STAģ���ϵ�ʱ����Ϣ]ʹ�����ݴ洢NV��0x40C-0x40D���ṹ��С��ṹNV_APP_NVM_DATA_STRU��ͬ
// �ڱ�ģ�鴦��ʱ��ͳһʹ�ñ�ģ���Զ���ĽṹNV_APP_STA_RESET_TIME_INFO
typedef struct
{
    HI_U32 aulResetTime[32];    // �����¼���Ϣ��4�ֽڴ洢һ��ʱ�䣬�����1970-1-1 00:00:00�����������
                                // ����0�������1����Ϣ���������ơ�Ĭ��ֵ��Ϊ0
} NV_APP_STA_RESET_TIME_INFO;


typedef struct
{
    // ռ�ձȲ���
    HI_U8 ucShortDuration;      // �����϶�ʱ�䣬��λ�����ӣ�Ĭ��ֵ30
    HI_U8 ucShortTimes;         // ��ʱ������������ֵ��Ĭ�ϣ�10�������ڴ洢�ռ䣬���ó���64��
    HI_U8 ucLongDuration;       // �����ϳ�ʱ�䣬��λ��Сʱ��Ĭ��ֵ72
    HI_U8 ucLongTimes;          // ��ʱ������������ֵ��Ĭ�ϣ�30�������ڴ洢�ռ䣬���ó���64��

    HI_S8 cIndex;               // ��¼��ռ�ձȲ�������
    HI_U8 ucDutyCycleNum;       // ռ�ձȲ�������
    HI_BOOL bRecTimeOnly;       // ��¼����ʱ���ʶ�������ѯ�ϵ��¼
    HI_U8 reserved;
    NV_APP_STA_DUTY_CYCLE_CFG_ITEM astCfgItem[MRS_STA_DUTY_CYCLE_CFG_ITEM_MAX];

    HI_U32 aulResetTime[MRS_STA_DUTY_CYCLE_RESET_TIME_MAX];

    HI_U32 ulCurrentDi;         // ��ǰ���ڴ����DI
    HI_U32 ulMeterCurTime;      // ���ǰʱ��
    HI_U32 ulMeterPowerOnTime;  // ���(��1��)�������ʱ��
} MRS_SRV_STA_DUTY_CYCLE_CONTEXT;



HI_PUBLIC HI_U32 mrsStaDutyCycleInit(HI_VOID);

HI_PUBLIC HI_U32 mrsStaDutyCycleDeinit(HI_VOID);

HI_PUBLIC HI_VOID mrsStaDutyCycleSetMeterInf(HI_BOOL bChg, HI_U8 ucSlaveFlag, HI_U32 ulParam);

HI_PUBLIC HI_U32 mrsStaDutyCycleFrameRx(HI_U8 *pFrame, HI_U16 usFrameLength);

HI_PUBLIC HI_U32 mrsStaDutyCycleTimeout(HI_VOID);


//*****************************************************************************
//*****************************************************************************
#endif // defined(PRODUCT_CFG_PRODUCT_TYPE_STA)
HI_END_HEADER

#endif //_MRS_SRV_STA_DUTYCYCLE_H_

