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
} DEMO_RES_CFG_S;

HI_EXTERN DEMO_RES_CFG_S g_stDemoOsRes;


//*****************************************************************************
// CTS_PRINT0 �����ϱ� 
// CTS_PRINT1 ���1 �ַ���֧�����128�ֽ�
//
//*****************************************************************************


//*****************************************************************************

//*****************************************************************************

HI_VOID demo_fast_send_init(HI_VOID);
HI_VOID DEMO_MainTaskBody(HI_U32 unused);
HI_PRV HI_U32 demo_ObjUsrInProc(HI_U16 usId, HI_IN HI_PVOID pCmdParam, HI_U16 usCmdParamSize, HI_U32 ulOption);

#endif // __APP_COMMON_H__

