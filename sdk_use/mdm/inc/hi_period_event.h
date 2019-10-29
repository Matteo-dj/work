#ifndef __CMN_PERIOD_EVENT_H__
#define __CMN_PERIOD_EVENT_H__
#ifdef PRODUCT_CFG_PRODUCT_TYPE_NDM
#define SAL_SDM_PERIOD_TASK_PERIOD_WAIT_MSG_DEFAULT_TIME        (1*1000)//周期性事件默认轮训时间
#else
#define SAL_SDM_PERIOD_TASK_PERIOD_WAIT_MSG_DEFAULT_TIME        (10*1000)//周期性事件默认轮训时间
#endif

#define SAL_SDM_PERIOD_TASK_PERIOD_SAVE_PERIOD      (10*1000)//可谓可测周期存储，延用之前时间，如果更改必须增加时间补偿代码


#define SAL_SDM_PERIOD_TASK_PERIOD_CHECK_ICC_CONNECT (5*1000)//ms
#define SAL_SDM_PERIOD_TASK_PERIOD_GET_VER (5*1000)//ms

/************************************上面暂时放在这******************************************************************************/
/************************************下面为对外开放接口******************************************************************************/

//周期性任务执行时间
typedef enum
{
    SAL_SDM_PERIOD_TASK_ID_SAVE_PERIOD,
    SAL_SDM_PERIOD_TASK_ID_MAX,
}SAL_SDM_PERIOD_TASK_ID_E;
typedef HI_U32 (*sdm_period_task_func)(HI_VOID);
//*****************************************************************************
// 函数名称: HI_PERIOD_SetEventPeriod
// 功能描述: 更改周期事件周期
//           
// 参数说明:
//      id[in] 周期事件对应ID
//      ms[in] 周期
//
// 返 回 值: 
//      HI_ERR_SUCCESS:成功
//
// 调用要求: 
//      设置的事件周期必须是轮询周期的整数倍
// 调用举例: 
// 作    者: 边海文20150205
//*****************************************************************************
HI_EXTERN HI_U32 HI_PERIOD_SetEventPeriod(HI_IN SAL_SDM_PERIOD_TASK_ID_E id,HI_IN HI_U32 ms);
//*****************************************************************************
// 函数名称: HI_PERIOD_StartPeriodTimer
// 功能描述: 启动周期性事件轮询定时器
//           
// 参数说明:
//
// 返 回 值: 
//      HI_ERR_SUCCESS:成功
//
// 调用要求: 
// 调用举例: 
// 作    者: 边海文20150205
//*****************************************************************************
HI_EXTERN HI_U32 HI_PERIOD_StartPeriodTimer(HI_VOID);
//*****************************************************************************
// 函数名称: HI_PERIOD_RegistPeriodEvent
// 功能描述: 注册周期性轮询事件
//           
// 参数说明:
//      id[in] 周期事件对应ID
//      ms[in] 周期
//      call_back[in] 用户回调函数
//
// 返 回 值: 
//      HI_ERR_SUCCESS:成功
//      其它错误
//
// 调用要求: 
//      设置的事件周期必须是轮询周期的整数倍
// 调用举例: 
// 作    者: 边海文20150205
//*****************************************************************************
HI_EXTERN HI_U32 HI_PERIOD_RegistPeriodEvent(HI_IN SAL_SDM_PERIOD_TASK_ID_E enId,HI_IN HI_U32 ulms,HI_IN sdm_period_task_func call_back);

#endif
