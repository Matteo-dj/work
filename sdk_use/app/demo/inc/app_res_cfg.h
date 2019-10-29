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
// 任务ID定义, 必须使用默认赋值方式, 即起始为0, 且所有值保持连续
typedef enum
{    
    EN_APP_TASK_DEMO,   // Custom SDK Test
    EN_APP_TASK_DEMO1,   // Custom SDK Test

    // TODO ... 
    
    EN_APP_TASK_MAX
} APP_TASK_ID_ENUM;
//////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////
// 消息队列配置
/////////////////////////////////////////////////////////////////////////
// 队列ID定义, 必须使用默认赋值方式, 即起始为0, 且所有值保持连续
typedef enum
{
    EN_APP_QUEUE_DEMO,
    EN_APP_QUEUE_DEMO1,
    // TODO ... 
    
    EN_APP_Q_MAX 
} APP_QUEUE_ID_ENUM;



//////////////////////////////////////////////////////////////////////////
// 信号量配置
// 注意枚举顺和配置定义行严格一致
//////////////////////////////////////////////////////////////////////////
typedef enum
{
    EN_APP_SEM_DEMO,
    
    // TODO ... 
    
    EN_APP_SEM_UPG,
    EN_APP_SEM_MAX
} APP_SEM_ENUM;

//////////////////////////////////////////////////////////////////////////
// TASK Timer 配置
//////////////////////////////////////////////////////////////////////////
typedef enum
{
    EN_APP_TASK_TIMER_DEMO_1,
    
    // TODO ... 
    
    EN_APP_TIMER_MAX
} APP_TIMER_ENUM;


#define ID_DEMO_MSG_MR_TIMER 0x201
#define ID_DEMO_MSG_MR_CUST 0x202
#define ID_DEMO_MSG_MR_CUST1 0x203
#define ID_DEMO_MSG_MR_CUST2 0x204



