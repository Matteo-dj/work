//*****************************************************************************
//
//                  ��Ȩ���� (C), 1998-2011, ��Ϊ�������޹�˾
//
//*****************************************************************************
//  �� �� ��   : app_res_cfg.h
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
#ifndef __HI_MDM_H__
#error "please include hi_mdm.h instead of this file"
#endif

#define SAL_DUMMY()

//////////////////////////////////////////////////////////////////////////
// ��������
// ע��ö��˳�� TASK_CFG_DEFINED/INIT_PROC_CFG_DEFINED ���ö�������Ҫ�ϸ�һ��
//////////////////////////////////////////////////////////////////////////
#ifdef SAL_HAVE_RES_CFG_TYPES
// ����ID����, ����ʹ��Ĭ�ϸ�ֵ��ʽ, ����ʼΪ0, ������ֵ��������
typedef enum
{
    EN_APP_TASK_APP_MRS,   // ������/���/�ɼ�����Э�鴦��
    EN_APP_TASK_MAX
} APP_TASK_ID_ENUM;
#endif // SAL_HAVE_RES_CFG_TYPES
#ifdef SAL_HAVE_TASK_STACK_GLOBALS
#include <hi_res_types.h>
// ��˳��ִ�г�ʼ����� ��������


//  TODO:�ڴ���������ĳ�ʼ��������

#endif // SAL_HAVE_TASK_STACK_GLOBALS
//////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////
// ��Ϣ��������
// ע��ö��˳�����ö������ϸ�һ��
//////////////////////////////////////////////////////////////////////////
#ifdef SAL_HAVE_RES_CFG_TYPES
// ����ID����, ����ʹ��Ĭ�ϸ�ֵ��ʽ, ����ʼΪ0, ������ֵ��������
typedef enum
{
    EN_APP_QUEUE_MRS,       // �������
    // TODO ...

    EN_APP_Q_MAX
} APP_QUEUE_ID_ENUM;
#endif // SAL_HAVE_RES_CFG_TYPES
#ifdef SAL_HAVE_QUEUE_GLOBALS
#include <hi_res_types.h>
// ���д�С����
SAL_Q_DEFINED(APP_QUEUE_MRS, PRODUCT_CFG_Q_APP_MRS_MSG_NUM)
//  TODO:  �ڴ������������Ϣ����

#endif // SAL_HAVE_QUEUE_GLOBALS
//////////////////////////////////////////////////////////////////////////


#ifdef SAL_HAVE_RES_CFG_TYPES
//////////////////////////////////////////////////////////////////////////
// �ź�������
// ע��ö��˳�����ö������ϸ�һ��
//////////////////////////////////////////////////////////////////////////
typedef enum
{
    EN_APP_SEM_1,
#if defined(PRODUCT_CFG_PRODUCT_TYPE_CCO)
	EN_APP_SEM_2,
	EN_APP_SEM_3,
#endif
    EN_APP_SEM_MAX
} APP_SEM_ENUM;
#ifdef PRODUCT_CFG_PRODUCT_TYPE_CCO
#define APP_SEM_NAME_DEFINE {\
    "APP_1_SEM", \
    "APP_2_SEM", \
    "APP_3_SEM", \
    }
#else
#define APP_SEM_NAME_DEFINE {\
    "APP_1_SEM", \
    }
#endif // PRODUCT_CFG_PRODUCT_TYPE_CCO
#endif // SAL_HAVE_RES_TYPES

#ifdef SAL_HAVE_RES_CFG_TYPES
//////////////////////////////////////////////////////////////////////////
// TASK Timer ����
//////////////////////////////////////////////////////////////////////////
typedef enum
{
    EN_APP_TASK_TIMER_MRS_1,
    EN_APP_TASK_TIMER_MRS_2,
    EN_APP_TASK_TIMER_MRS_3,
    EN_APP_TASK_TIMER_MRS_4,
    EN_APP_TASK_TIMER_MRS_5,
    EN_APP_TASK_TIMER_MRS_6,
    EN_APP_TASK_TIMER_MRS_7,
    EN_APP_TASK_TIMER_MRS_8,
    EN_APP_TASK_TIMER_MRS_9,
    EN_APP_TASK_TIMER_MRS_10,
    EN_APP_TASK_TIMER_MRS_11,
    EN_APP_TASK_TIMER_MRS_12,
    EN_APP_TASK_TIMER_MRS_13,
    EN_APP_TASK_TIMER_MRS_14,
    EN_APP_TASK_TIMER_MRS_15,
    EN_APP_TASK_TIMER_MRS_16,
    EN_APP_TASK_TIMER_MRS_17,
    EN_APP_TASK_TIMER_MRS_18,
    EN_APP_TASK_TIMER_MRS_19,
    EN_APP_TASK_TIMER_MRS_20,
    EN_APP_TASK_TIMER_MRS_21,
    EN_APP_TASK_TIMER_MRS_22,
    EN_APP_TASK_TIMER_MRS_23,
    EN_APP_TASK_TIMER_MRS_24,
    EN_APP_TASK_TIMER_MRS_25,
    EN_APP_TASK_TIMER_MRS_26,
    EN_APP_TASK_TIMER_MRS_27,
    EN_APP_TASK_TIMER_MRS_28,
    EN_APP_TASK_TIMER_MRS_29,
    EN_APP_TASK_TIMER_MRS_30,
    EN_APP_TASK_TIMER_MRS_31,
    EN_APP_TASK_TIMER_MRS_32,
    EN_APP_TASK_TIMER_MRS_33,
    EN_APP_TASK_TIMER_MRS_34,
    EN_APP_TASK_TIMER_MRS_35,
    EN_APP_TASK_TIMER_MRS_36,
    EN_APP_TASK_TIMER_MRS_37,
    EN_APP_TASK_TIMER_MRS_38,
    EN_APP_TASK_TIMER_MRS_39,
    EN_APP_TASK_TIMER_MRS_40,
	EN_APP_TASK_TIMER_MRS_41,
	EN_APP_TASK_TIMER_MRS_42,
    EN_APP_TASK_TIMER_MRS_43,
    EN_APP_TASK_TIMER_MRS_44,
    EN_APP_TASK_TIMER_MRS_45,
    EN_APP_TASK_TIMER_MRS_46,
    EN_APP_TASK_TIMER_MRS_47,
#ifdef PRODUCT_CFG_PRODUCT_TYPE_CCO
    EN_APP_TASK_TIMER_MRS_48,
    EN_APP_TASK_TIMER_MRS_49,
    EN_APP_TASK_TIMER_MRS_50,
    EN_APP_TASK_TIMER_MRS_51,
    EN_APP_TASK_TIMER_MRS_52,
    EN_APP_TASK_TIMER_MRS_53,
    EN_APP_TASK_TIMER_MRS_54,
    EN_APP_TASK_TIMER_MRS_55,
    EN_APP_TASK_TIMER_MRS_56,
#endif
	EN_APP_TASK_TIMER_MRS_57,
	EN_APP_TASK_TIMER_MRS_58,
	EN_APP_TASK_TIMER_MRS_59,
	EN_APP_TASK_TIMER_MRS_60,
	EN_APP_TASK_TIMER_MRS_61,
	EN_APP_TASK_TIMER_MRS_62,

	EN_APP_TASK_TIMER_MRS_63,
	EN_APP_TASK_TIMER_MRS_64,
	EN_APP_TASK_TIMER_MRS_65,
	EN_APP_TASK_TIMER_MRS_66,
	EN_APP_TASK_TIMER_MRS_67,
	EN_APP_TASK_TIMER_MRS_68,
	EN_APP_TASK_TIMER_MRS_69,
	EN_APP_TASK_TIMER_MRS_70,
	EN_APP_TASK_TIMER_MRS_71,
	EN_APP_TASK_TIMER_MRS_72,
	EN_APP_TASK_TIMER_MRS_73,
	EN_APP_TASK_TIMER_MRS_74,

	//CJJ-HPLC:2018-9-7
	EN_APP_TASK_TIMER_MRS_80,
	//CJJ-HPLC:2018-9-7 
	EN_APP_TASK_TIMER_MRS_81,//Matteo

  EN_APP_TIMER_MAX
} APP_TIMER_ENUM;

#endif // SAL_HAVE_RES_TYPES

