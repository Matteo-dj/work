//*****************************************************************************
//
//                  ��Ȩ���� (C), 1998-2011, ��Ϊ�������޹�˾
//
//*****************************************************************************
//  �� �� ��   : os_scres_cfg.h
//  �� �� ��   : V1.0 
//  ��    ��   : shenhankun/00130424
//  ��������   : 2011-10-18
//  ��������   : TODO: ...
//               
//  �����б�   : TODO: ...
//  �޸���ʷ   : 
//  1.��    �� : 2011-10-18
//    ��    �� : shenhankun/00130424
//    �޸����� : �����ļ� 
//
//*****************************************************************************

#define SAL_DUMMY()  

//////////////////////////////////////////////////////////////////////////
// ��������
// ע��ö��˳�� TASK_CFG_DEFINED/INIT_PROC_CFG_DEFINED ���ö�������Ҫ�ϸ�һ��
//////////////////////////////////////////////////////////////////////////
#ifdef SAL_HAVE_RES_CFG_TYPES
typedef enum
{    
    EN_SC_TASK_MAINMONITOR,
    EN_SC_TASK_EXPMONITOR,
    
    EN_SC_TASK_MAX
} SC_TASK_ID_ENUM;
#endif // SAL_HAVE_RES_CFG_TYPES

//////////////////////////////////////////////////////////////////////////
#if !defined(SAL_HAVE_NO_RES_DEFINED)
#ifdef SAL_HAVE_TASK_STACK_GLOBALS
#include <hi_res_types.h>

// ��˳��ִ�г�ʼ����� ������ ������ 
SAL_DUMMY()
SC_TASK_CFG_DEFINED(SC_TASK_MAINMONITOR,  PRODUCT_CFG_TASK_STACK_SIZE_SC_MAINMONITOR, PRODUCT_CFG_TASK_PRIO_SC_MAINMONITOR, SAL_ScMainTaskInit,  SAL_ScMainTaskBody,  HI_DLL_LIB_NAME("sal"))
#if defined(PRODUCT_CFG_AUTO_RESET_SYSTEM_TIMEOUT) && defined(PRODUCT_CFG_TASK_STACK_SIZE_SC_EXPMONITOR) && defined(PRODUCT_CFG_TASK_PRIO_SC_EXPMONITOR)
SC_TASK_CFG_DEFINED(SC_TASK_EXPMONITOR,  PRODUCT_CFG_TASK_STACK_SIZE_SC_EXPMONITOR, PRODUCT_CFG_TASK_PRIO_SC_EXPMONITOR, SAL_ScExpWdMonitorTaskInit,  SAL_ScExpWdMonitorTaskBody,  HI_DLL_LIB_NAME("sal"))
#endif 
//SC_INIT_PROC_CFG_DEFINED(PHY_INIT         ,   0,      0,  PHY_Init ,  HI_NULL  ,  HI_DLL_LIB_NAME("phy"))
#endif // SAL_HAVE_TASK_STACK_GLOBALS
//////////////////////////////////////////////////////////////////////////
#endif // SAL_HAVE_NO_RES_DEFINED
//////////////////////////////////////////////////////////////////////////

