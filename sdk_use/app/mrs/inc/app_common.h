//*****************************************************************************
//
//                  版权所有 (C), 2001-2011, 华为技术有限公司
//
//*****************************************************************************
//  文 件 名   : app_common.h
//  版 本 号   : V1.0 
//  作    者   : 沈汉坤/00130424
//  生成日期   : 2011年09月06日
//  功能描述   : ASL共用接口定义
//  函数列表   : 无
//  修改历史   : 
//  1.日    期 : 2011年09月06日
//    作    者 : 沈汉坤/00130424
//    修改内容 : 创建文件 
//*****************************************************************************

#ifndef __APP_COMMON_H__
#define __APP_COMMON_H__

#include <hi_driver.h>
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


#if defined(PRODUCT_CFG_PRODUCT_TYPE_CCO)
#define EN_APP_SEM_MRS_ND_STATE_CHG     EN_APP_SEM_1
#define EN_APP_SEM_MRS_REJECT_JOIN_NET  EN_APP_SEM_2
#define EN_APP_SEM_MRS_BLACK_LIST       EN_APP_SEM_3
#endif

#if defined(PRODUCT_CFG_PRODUCT_TYPE_STA)
#define EN_APP_SEM_SRV_UART             EN_APP_SEM_1
#endif

HI_U32 appDfxInit(HI_VOID);
HI_U32 MRS_Init(HI_VOID);


#endif // __APP_COMMON_H__

