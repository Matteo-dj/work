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

#include <hi_mdm.h>
#include <hi_mdm_cpu_stat.h>
#include <hi_mdm_event.h>
#include <hi_mdm_isr.h>
#include <hi_mdm_mem.h>
#include <hi_mdm_msg.h>
#include <hi_mdm_sem.h>
#include <hi_mdm_sys.h>
#include <hi_mdm_task.h>
#include <hi_mdm_timer.h>
#include <hi_mem.h>
#include <hi_stdlib.h>
#include <hi_mdm_rst_times.h>

#include "dfx_app.h"
#include "app_config.h"


#define SAL_HAVE_RES_CFG_TYPES 
#include "app_res_cfg.h"
#undef SAL_HAVE_RES_CFG_TYPES 

#define EN_APP_SEM_MRS_ND_STATE_CHG EN_APP_SEM_1
#if defined(PRODUCT_CFG_PRODUCT_TYPE_CCO)
#define EN_APP_SEM_MRS_REJECT_JOIN_NET EN_APP_SEM_2
#define EN_APP_SEM_MRS_BLACK_LIST EN_APP_SEM_3
#endif

HI_U32 appDfxInit(HI_VOID);
HI_U32 MRS_Init(HI_VOID);


#endif // __APP_COMMON_H__

