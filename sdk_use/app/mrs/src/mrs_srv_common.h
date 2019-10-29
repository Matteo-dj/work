//*****************************************************************************
//
//                  ��Ȩ���� (C), 1998-2011, ��Ϊ�������޹�˾
//
//*****************************************************************************
//  �� �� ��   : mrs_srv_common.h
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


//*****************************************************************************
// PROJECT   : 
// SUBSYSTEM :
// MODULE    :  
// OWNER     :  
//*****************************************************************************


#ifndef __MRS_SRV_COMMON_H__
#define __MRS_SRV_COMMON_H__

#include "mrs_srv_res.h"

HI_START_HEADER


//*****************************************************************************
// ����ģ�������Ľṹ����
//*****************************************************************************

// PLCͨ������/����ģʽ����
#define MRS_PLC_RX_TX_MODE_NORMAL       0   // ��ͨģʽ(��ȴ�����·���Ż���ɺ�Ž���PLC�շ�)
#define MRS_PLC_RX_TX_MODE_FORCE        1   // ǿ��ģʽ(���ȴ�Mac����������Ż�����ǿ�ƽ����շ�)

// STAģ�����Ͷ���
typedef enum
{
    EN_MRS_STA_TYPE_METER = 0,
    EN_MRS_STA_TYPE_CLT_I,
    EN_MRS_STA_TYPE_CLT_II,
    EN_MRS_STA_TYPE_MAX,
} MRS_STA_TYPE_ENUM;

#define MRS_STA_TYPE_METER              0   // �����̬
#define MRS_STA_TYPE_COLLECTOR          1   // �ɼ�����̬

// STA-METER֮�䳬ʱʱ��Ĭ��ֵ
#define MRS_RM_METER_OVERTIME_MAX       (1000 * 10)  // ���Ӧ��ʱ(���)
#define MRS_RM_METER_OVERTIME_DEFAULT   (1800)       // ���Ӧ��ʱ(Ĭ��)
#define MRS_RM_METER_OVERTIME_MIN       (100)        // ���Ӧ��ʱ(��С)
#define MRS_CHECK_METER_FAULT_FRAME_NUM (2)          // STA�˼��������������С֡����

#define MRS_100MS_TO_MS(x)              ((x) * 100)
#define MRS_10MS_TO_MS(x)               ((x) * 10)
#define MRS_MS_TO_100MS(x)              ((x) / 100)
#define MRS_MS_TO_10MS(x)               ((x) / 10)

// �ѱ�״̬
#define MRS_SEARCH_STATUS_IDLE          (0)
#define MRS_SEARCH_STATUS_IN_PROGRESS   (1)
#define MRS_SEARCH_STATUS_UNKNOWN       (3)
#define MRS_SEARCH_STATUS_POWER_OFF_RM  (4)

#if defined(PRODUCT_CFG_PRODUCT_TYPE_STA)

// ͨ���ı�֪ͨ����������
typedef HI_U32 (* MRS_CHL_NOTIFY_PROC_F)(HI_BOOL, HI_U32, HI_BOOL *);


// ͨ���ı�֪ͨ�����ʼ��
HI_VOID mrsSrvChlNotifyProcInit(HI_VOID);

// ͨ���ı�֪ͨ������ע��
HI_U32 mrsSrvChlNotifyProcReg(MRS_CHL_NOTIFY_PROC_F, HI_U32);

// ͨ���ı�֪ͨ����������
HI_U32 mrsSrvChlNotifyProcClear(MRS_CHL_NOTIFY_PROC_F);

// ͨ���ı�֪ͨ����������
HI_U32 mrsSrvChlNotifyProc(HI_BOOL);

#endif // defined(PRODUCT_CFG_PRODUCT_TYPE_STA)


HI_U32 mrsSrvGetVerInfo(HI_U16 *pusVer, HI_U8 *pucBootVer);

#if defined(PRODUCT_CFG_PRODUCT_TYPE_CCO)
HI_S16 mrsSrvDutGetNtbAdjustBase(HI_VOID);
#endif

HI_END_HEADER
#endif //__MRS_SRV_COMMON_H__

