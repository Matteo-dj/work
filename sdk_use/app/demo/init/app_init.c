//*****************************************************************************
//
//                  版权所有 (C), 1998-2011, 华为技术有限公司
//
//*****************************************************************************
//  文 件 名   : app_init.c
//  版 本 号   : V1.0 
//  作    者   : shenhankun/00130424
//  生成日期   : 2011-12-24
//  功能描述   : APP 
//               
//  函数列表   : TODO: ...
//  修改历史   : 
//  1.日    期 : 2011-12-24
//    作    者 : shenhankun/00130424
//    修改内容 : 创建文件 
//
//*****************************************************************************

//*****************************************************************************
// PROJECT   : 
// SUBSYSTEM :
// MODULE    :  
// OWNER     :  
//*****************************************************************************
#include "app_common.h"
//#include "app_res_cfg_inc.def"
#include "hi_mdm.h"
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
DEMO_RES_CFG_S g_stDemoOsRes = {0};

HI_U32 demo_res_init(HI_VOID)
{   
    HI_U32 ret = HI_ERR_SUCCESS;
    /*创建任务*/
    ret += HI_MDM_TASK_Create(&g_stDemoOsRes.aulTaskId[EN_APP_TASK_DEMO], "DEMO", DEMO_MainTaskBody, 0,PRODUCT_CFG_TASK_STACK_SIZE_APP_CTS, PRODUCT_CFG_TASK_PRIO_APP_CTS);
    //ret += HI_MDM_TASK_Create(&g_stCtsOsRes.aulTaskId[EN_APP_TASK_CTS1], "CTS1", CTS_TestTaskBody, 0,PRODUCT_CFG_TASK_STACK_SIZE_APP_CTS, PRODUCT_CFG_TASK_PRIO_APP_CTS);

    /*建立消息队列*/
    ret += HI_MDM_MSG_CreateQueue(&g_stDemoOsRes.aulQueueId[EN_APP_QUEUE_DEMO],"DEMO",128);
    //ret += HI_MDM_MSG_CreateQueue(&g_stCtsOsRes.aulQueueId[EN_APP_QUEUE_CTS1],"CTS1",32);
    /*建立信号量*/
    ret += HI_MDM_SEM_BCreate(&g_stDemoOsRes.aulSemId[EN_APP_SEM_DEMO], "DEMO",  HI_SEM_ONE);
    //ret += HI_MDM_SEM_BCreate(&g_stCtsOsRes.aulSemId[EN_APP_SEM_UPG], "cts_upg",  HI_SEM_ONE);
    if (ret != HI_ERR_SUCCESS)
        return HI_ERR_FAILURE;
}

HI_VOID APP_Init(HI_VOID)
{
    HI_FTM_PRODUCT_TYPE_E enProductType = HI_MDM_GetProductType();

    /*初始化网口驱动*/
    #ifdef PRODUCT_CFG_BOARD_TYPE_ASIC
    if (HI_FTM_CHIP_TYPE_3911C == HI_MDM_GetHwChipType())
    #endif
    {
        if (HI_MDM_EthInit() != HI_ERR_SUCCESS)
        {
            return;
        }
    }

    demo_res_init();
    demo_fast_send_init();
}
