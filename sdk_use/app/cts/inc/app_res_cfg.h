/**
*******************************************************************************
*
*   Copyright(C), 2008-2050, Huawei Tech. Co., Ltd. ALL RIGHTS RESERVED.
*
*******************************************************************************
* @file     app_res_cfg.h
* @version  1.0
* @author   c00375526
* @date     2017/10/14
* @brief    app_res_cfg.h
*
* @par  History
* @li   c00375526, 2017/10/14, Created file
*
*/
// ����ID����, ����ʹ��Ĭ�ϸ�ֵ��ʽ, ����ʼΪ0, ������ֵ��������
typedef enum
{    
    EN_APP_TASK_CTS,   // Custom SDK Test
    EN_APP_TASK_CTS1,   // Custom SDK Test

    // TODO ... 
    
    EN_APP_TASK_MAX
} APP_TASK_ID_ENUM;
//////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////
// ��Ϣ��������
/////////////////////////////////////////////////////////////////////////
// ����ID����, ����ʹ��Ĭ�ϸ�ֵ��ʽ, ����ʼΪ0, ������ֵ��������
typedef enum
{
    EN_APP_QUEUE_CTS,
    EN_APP_QUEUE_CTS1,
    // TODO ... 
    
    EN_APP_Q_MAX 
} APP_QUEUE_ID_ENUM;



//////////////////////////////////////////////////////////////////////////
// �ź�������
// ע��ö��˳�����ö������ϸ�һ��
//////////////////////////////////////////////////////////////////////////
typedef enum
{
    EN_APP_SEM_CTS,
    
    // TODO ... 
    
    EN_APP_SEM_UPG,
    EN_APP_SEM_MAX
} APP_SEM_ENUM;

//////////////////////////////////////////////////////////////////////////
// TASK Timer ����
//////////////////////////////////////////////////////////////////////////
typedef enum
{
    EN_APP_TASK_TIMER_CTS_1,
    
    // TODO ... 
    
    EN_APP_TIMER_MAX
} APP_TIMER_ENUM;


#define ID_CTS_MSG_MR_TIMER 0x201
#define ID_CTS_MSG_MR_CUST 0x202
#define ID_CTS_MSG_MR_CUST1 0x203
#define ID_CTS_MSG_MR_CUST2 0x204



