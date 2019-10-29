//*****************************************************************************
//
//                  ��Ȩ���� (C), 1998-2011, ��Ϊ�������޹�˾
//
//*****************************************************************************
//  �� �� ��   : app_init.c
//  �� �� ��   : V1.0 
//  ��    ��   : shenhankun/00130424
//  ��������   : 2011-12-24
//  ��������   : APP 
//               
//  �����б�   : TODO: ...
//  �޸���ʷ   : 
//  1.��    �� : 2011-12-24
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
#include "app_common.h"
//#include "app_res_cfg_inc.def"
#include "hi_mdm.h"
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
HI_U32 sys_endTime;
HI_SYS_APP_RES_CFG_S g_stAppOsRes = {0,};

typedef struct
{
    HI_U32 * pTaskId;
    
}HI_MDM_TASK_CFG_S;

typedef struct 
{
    HI_MDM_TASK_CFG_S *stTaskCfg; /*˳���Ÿ��������������Ϣ*/
    HI_U32 ulTaskCnt;
    
}HI_MDM_APP_RES_CFG_S;


CTS_RES_CFG_S g_stCtsOsRes = {0,};


HI_U32 cts_res_init(HI_VOID)
{   
    HI_U32 ret = HI_ERR_SUCCESS;
    /*��������*/
    ret += HI_MDM_TASK_Create(&g_stCtsOsRes.aulTaskId[EN_APP_TASK_CTS], "CTS", CTS_MainTaskBody, 0,PRODUCT_CFG_TASK_STACK_SIZE_APP_CTS, PRODUCT_CFG_TASK_PRIO_APP_CTS);
    //ret += HI_MDM_TASK_Create(&g_stCtsOsRes.aulTaskId[EN_APP_TASK_CTS1], "CTS1", CTS_TestTaskBody, 0,PRODUCT_CFG_TASK_STACK_SIZE_APP_CTS, PRODUCT_CFG_TASK_PRIO_APP_CTS);

    /*������Ϣ����*/
    ret += HI_MDM_MSG_CreateQueue(&g_stCtsOsRes.aulQueueId[EN_APP_QUEUE_CTS],"CTS",128);
    //ret += HI_MDM_MSG_CreateQueue(&g_stCtsOsRes.aulQueueId[EN_APP_QUEUE_CTS1],"CTS1",32);
    /*�����ź���*/
    ret += HI_MDM_SEM_BCreate(&g_stCtsOsRes.aulSemId[EN_APP_SEM_CTS], "cts",  HI_SEM_ONE);
    //ret += HI_MDM_SEM_BCreate(&g_stCtsOsRes.aulSemId[EN_APP_SEM_UPG], "cts_upg",  HI_SEM_ONE);
    if (ret != HI_ERR_SUCCESS)
        return HI_ERR_FAILURE;
}
    

HI_VOID APP_Init(HI_VOID)
{
    HI_FTM_PRODUCT_TYPE_E enProductType = HI_MDM_GetProductType();
    
    /*����汾��*/
    HI_MDM_CONFIG_APP_VER(CTS,1,0);   
    
    /*��ʼ����������*/
#ifdef PRODUCT_CFG_BOARD_TYPE_ASIC
    if (HI_FTM_CHIP_TYPE_3911C == HI_MDM_GetHwChipType())
#endif
    {
    
        if (HI_MDM_EthInit() != HI_ERR_SUCCESS)
		{
			return;
		}
    }

	cts_res_init();

}
HI_U32 g_SetPerformanceRet;



