//*****************************************************************************
//
//                  ��Ȩ���� (C), 2001-2011, ��Ϊ�������޹�˾
//
//*****************************************************************************
//  �� �� ��   : app_common.h
//  �� �� ��   : V1.0 
//  ��    ��   : ����/00130424
//  ��������   : 2011��09��06��
//  ��������   : ASL���ýӿڶ���
//  �����б�   : ��
//  �޸���ʷ   : 
//  1.��    �� : 2011��09��06��
//    ��    �� : ����/00130424
//    �޸����� : �����ļ� 
//*****************************************************************************

#ifndef __APP_COMMON_H__
#define __APP_COMMON_H__

//*****************************************************************************
#include <hi_ft_nv.h>
#include <hi_mdm.h>

#include "app_config.h"
#include "dfx_sys.h"
#include "dfx_usr.h"
#include "hi_driver.h"
//#include "plc/os/hi_ssi.h"
//*****************************************************************************
#include <app_res_cfg.h>


typedef struct
{
    HI_U32 aulTaskId[EN_APP_TASK_MAX];
    HI_U32 aulQueueId[EN_APP_Q_MAX];
    HI_TIMER_HANDLE_S astTimerHandle[EN_APP_TIMER_MAX];
    HI_U32 aulSemId[EN_APP_SEM_MAX];
} CTS_RES_CFG_S;

HI_EXTERN CTS_RES_CFG_S g_stCtsOsRes;


//*****************************************************************************
// CTS_PRINT0 �����ϱ� 
// CTS_PRINT1 ���1 �ַ���֧�����128�ֽ�
//
#define CTS_PRINT0(x)          HI_MDM_ReportPacket((HI_U32)HI_DCID_STR, (HI_U8)0, (HI_PBYTE)x, (HI_U16)(sizeof(x)-1), HI_TRUE)
#define CTS_PRINT1(x,v1)       do{ HI_CHAR buf[128] = {0,0, }; (hi_void)sprintf_s(buf,128, x, v1); HI_MDM_ReportPacket((HI_U32)HI_DCID_STR, (HI_U8)0, (HI_PBYTE)buf, (HI_U16)128, HI_TRUE);}while(0)
#define CTS_PRINT2(x,v1,v2)    do{ HI_CHAR buf[128] = {0,0, }; (hi_void)sprintf_s(buf,128, x, v1,v2); HI_MDM_ReportPacket((HI_U32)HI_DCID_STR, (HI_U8)0, (HI_PBYTE)buf, (HI_U16)128, HI_TRUE);}while(0)
#define CTS_PRINT3(x,v1,v2,v3) do{ HI_CHAR buf[128] = {0,0, }; (hi_void)sprintf_s(buf,128, x, v1,v2,v3); HI_MDM_ReportPacket((HI_U32)HI_DCID_STR, (HI_U8)0, (HI_PBYTE)buf, (HI_U16)128, HI_TRUE);}while(0)
#define CTS_PRINT4(x,v1,v2,v3,v4) do{ HI_CHAR buf[128] = {0,0, }; (hi_void)sprintf_s(buf,128, x, v1,v2,v3,v4); HI_MDM_ReportPacket((HI_U32)HI_DCID_STR, (HI_U8)0, (HI_PBYTE)buf, (HI_U16)128, HI_TRUE);}while(0)
#define CTS_PRINT6(x,v1,v2,v3,v4,v5,v6) do{ HI_CHAR buf[128] = {0,0, }; (hi_void)sprintf_s(buf,128, x, v1,v2,v3,v4,v5,v6); HI_MDM_ReportPacket((HI_U32)HI_DCID_STR, (HI_U8)0, (HI_PBYTE)buf, (HI_U16)128, HI_TRUE);}while(0)
//*****************************************************************************

//*****************************************************************************
typedef struct
{
    HI_BOOL init;
    HI_BOOL chl_connect;
    HI_U8   state;
    
}CTS_CTRL_S;

#define HI_CTS_CheckTest(x, y)  0
#define CTS_CTRL_SIZE   8
//*****************************************************************************

HI_EXTERN HI_VOID CTS_MainTaskBody(HI_U32 unused);
HI_EXTERN HI_VOID CTS_TestTaskBody(HI_U32 unused);

//*****************************************************************************
#endif // __APP_COMMON_H__

