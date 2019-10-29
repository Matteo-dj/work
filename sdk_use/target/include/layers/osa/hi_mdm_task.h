/**@defgroup hct_task 任务
 * @ingroup osa
 */
#ifndef __HI_MDM_TASK_H__
#define __HI_MDM_TASK_H__
#if !defined(MAC_V200_LLT)
#include "los_trace.h"
#endif
#include <hi_types.h>
#include <hi_mdm_types.h>

/**
 * @ingroup hct_task
 * 非法任务ID
 */
#define HI_INVALID_TASK_ID   0xFFFFFFFF

/**
 * @ingroup hct_task
 * 任务自动删除
 */
#define HI_TASK_STATUS_DETACHED    0x0080

/**
 * @ingroup hct_task
 * 任务名称长度
 */
#define HI_TASK_NAME_LEN     32

typedef struct
{
    HI_U32 pad[3];
}HI_EVENT_CB_S;//临时定义

/**
 * @ingroup hct_task
 * 任务信息
 */
typedef struct
{
    HI_CHAR         acName[HI_TASK_NAME_LEN];   /**< 任务名 */
    HI_U32          uwTaskID;					/**< 任务ID */
    HI_U16          usTaskStatus;               /**< 任务状态 */
    HI_U16          usTaskPrio;                 /**< 任务优先级 */
    HI_PVOID        pTaskSem;                   /**< 信号量指针 */
    HI_PVOID        pTaskMux;                   /**< Mutex pointer             */
    HI_EVENT_CB_S   uwEvent;                    /**< 事件 */
    HI_U32          uwEventMask;                /**< 事件屏蔽值，即屏蔽事件的哪些位 */
    HI_U32          uwStackSize;                /**< 任务栈尺寸 */
    HI_U32          uwTopOfStack;               /**< 任务栈栈顶 */
    HI_U32          uwBottomOfStack;            /**< 任务栈栈底 */
    HI_U32          uwSP;                       /**< 任务SP指针 */
    HI_U32          uwCurrUsed;                 /**< 任务当前使用的栈大小*/
    HI_U32          uwPeakUsed;                 /**< Task stack peak used size           */
    HI_BOOL         bOvf;                       /**< 标记任务栈是否溢出 */
} HI_TASK_INFO_S;

typedef HI_VOID (*TSK_BODY)(HI_U32 ulParam);

/**
* @ingroup  hct_task
* @brief  创建任务。
*
* @par 描述:  
* 创建任务。
* @attention
* @li 接口内部不会分配空间存储任务名，调用者需要保证任务名空间是全局不会被释放修改的。
* @li 若指定的任务栈大小为0，则使用配置项LOSCFG_BASE_CORE_TSK_DEFAULT_STACK_SIZE(0x1000)指定默认的任务栈大小。
* @li 任务栈的大小按8字节大小对齐。确定任务栈大小的原则是，保证栈不溢出的情况下尽量小(节约空间)。
* @li 由于系统任务占用一部分优先级，所以用户可以使用的优先级为2~29，应用级任务建议使用大于等于15的优先级。
* @param  pTaskId [OUT] 任务ID号。
* @param  szName[8] [IN] 任务名字。
* @param  pfnTaskBody[IN] 任务入口函数。
* @param  ulParam[IN] 创建任务时需要传给任务入口的参数。如果不需要传递 参数直接填0。
* @param  ulStackSize[IN] 栈大小。
* @param  ulPriority[IN] 任务优先级。
*
* @retval #HI_ERR_SUCCESS 成功。
* @retval 其他值 失败，详见hi_errno.h。
*
* @par Dependency:
* @li hi_mdm_task.h: 该接口声明所在的头文件。
* @see  HI_MDM_TASK_Delete
* @since HiMDL_V200R001C00
*/
HI_U32 HI_MDM_TASK_Create (HI_OUT HI_U32 * pTaskId, HI_CHAR szName[32], TSK_BODY pfnTaskBody,HI_U32 ulParam, HI_U32 ulStackSize, HI_U32 ulPriority);

/**
* @ingroup  hct_task
* @brief  删除任务。
*
* @par 描述:  
* 删除任务。
* @attention 
* @li 不允许对非APP任务进行删除。
* @li 在删除任务时要保证任务申请的资源（如互斥锁、信号量等）已被释放。
* @param  ulTaskId [IN] 任务ID号。
*
* @retval #HI_ERR_SUCCESS 成功。
* @retval 其他值 失败，详见hi_errno.h。
*
* @par Dependency:
* @li hi_mdm_task.h: 该接口声明所在的头文件。
* @see  HI_MDM_TASK_Create
* @since HiMDL_V200R001C00
*/
HI_U32 HI_MDM_TASK_Delete (HI_U32 ulTaskId);

/**
* @ingroup  hct_task
* @brief  任务创建的增强接口。
*
* @par 描述:  
* 该接口为任务创建的增强接口，支持创建自删除任务。
* @attention
* @li 接口内部不会分配空间存储任务名，调用者需要保证任务名空间是全局不会被释放修改的。
* @li 若指定的任务栈大小为0，则使用配置项LOSCFG_BASE_CORE_TSK_DEFAULT_STACK_SIZE(0x1000)指定默认的任务栈大小。
* @li 任务栈的大小按8字节大小对齐。确定任务栈大小的原则是，保证栈不溢出的情况下尽量小(节约空间)。
* @li 由于系统任务占用一部分优先级，所以用户可以使用的优先级为2~29，应用级任务建议使用大于等于15的优先级。
* @param  pTaskId [OUT] 任务ID号。
* @param  szName[8] [IN] 任务名字。
* @param  pfnTaskBody[IN] 任务入口函数。
* @param  ulParam[IN] 创建任务时需要传给任务入口的参数。如果不需要传递 参数直接填0。
* @param  ulStackSize[IN] 栈大小。
* @param  ulPriority[IN] 任务优先级。
* @param  ulOption[IN] 任务自动删除选项。若创建一个自删除任务，则该参数需取值HI_TASK_STATUS_DETACHED；否则取值为0。
*
* @retval #HI_ERR_SUCCESS 成功。
* @retval 其他值 失败，详见hi_errno.h。
*
* @par Dependency:
* @li hi_mdm_task.h: 该接口声明所在的头文件。
* @see 无。
* @since HiMDL_V200R001C00
*/
HI_U32 HI_MDM_TASK_CreateEx (HI_OUT HI_U32* pTaskId, HI_CHAR szName[32], TSK_BODY pfnTaskBody, HI_U32  ulParam, HI_U32 ulStackSize, HI_U32 ulPriority,HI_U32 ulOption);

/**
* @ingroup  hct_task
* @brief  挂起任务。
*
* @par 描述:  
* 挂起指定任务。
* @attention
* @li 挂起任务的时候若为当前任务且已锁任务，则不能被挂起。
* @li 不允许对非APP任务进行挂起操作。
* @li 在锁任务调度状态下，禁止任务阻塞与读事件。
* @param  ulTaskId [IN] 任务ID号。
*
* @retval #HI_ERR_SUCCESS 成功。
* @retval 其他值 失败，详见hi_errno.h。
*
* @par Dependency:
* @li hi_mdm_task.h: 该接口声明所在的头文件。
* @see  HI_MDM_TASK_Resume
* @since HiMDL_V200R001C00
*/
HI_U32 HI_MDM_TASK_Suspend (HI_U32 ulTaskId);

/**
* @ingroup  hct_task
* @brief  恢复被挂起的任务。
*
* @par 描述:  
* 恢复被挂起的任务。
* @attention 无。
*
* @param  ulTaskId [IN] 任务ID号。
*
* @retval #HI_ERR_SUCCESS 成功。
* @retval 其他值 失败，详见hi_errno.h。
*
* @par Dependency:
* @li hi_mdm_task.h: 该接口声明所在的头文件。
* @see  HI_MDM_TASK_Suspend
* @since HiMDL_V200R001C00
*/
HI_U32 HI_MDM_TASK_Resume (HI_U32 ulTaskId);

/**
* @ingroup  hct_task
* @brief  获取任务优先级。
*
* @par 描述:  
* 获取任务优先级。
* @attention 
* @li 传入的task ID对应的任务未创建或者超过最大任务数，统一返回错误。
*
* @param  ulTaskId [IN] 任务ID号。
* @param  pPriority [OUT] 任务优先级。
*
* @retval #HI_ERR_SUCCESS 成功。
* @retval 其他值 失败，详见hi_errno.h。
*
* @par Dependency:
* @li hi_mdm_task.h: 该接口声明所在的头文件。
* @see  HI_MDM_TASK_SetPriority
* @since HiMDL_V200R001C00
*/
HI_U32 HI_MDM_TASK_GetPriority (HI_U32 ulTaskId, HI_OUT HI_U32 *pPriority);

/**
* @ingroup  hct_task
* @brief  设置任务优先级。
*
* @par 描述: 
* 设置任务优先级。
* @attention 
* 不能修改非APP任务的优先级。
* @param  ulTaskId [IN] 任务ID号。
* @param  ulPriority [IN] 任务优先级。
*
* @retval #HI_ERR_SUCCESS 成功。
* @retval 其他值 失败，详见hi_errno.h。
*
* @par Dependency:
* @li hi_mdm_task.h: 该接口声明所在的头文件。
* @see  HI_MDM_TASK_GetPriority
* @since HiMDL_V200R001C00
*/
HI_U32 HI_MDM_TASK_SetPriority(HI_U32 ulTaskId, HI_U32 ulPriority);

/**
* @ingroup  hct_task
* @brief  锁任务调度。
*
* @par 描述:  
* 锁任务调度。
* @attention 
* @li 加锁期间不允许调用引发调度接口，如信号量、HI_MDM_Sleep等。
* @li 锁任务调度，并不关中断，因此任务仍可被中断打断。
* @li 锁任务调度必须和解锁任务调度配合使用。
*
* @param 无。
*
* @retval 无。
*
* @par Dependency:
* @li hi_mdm_task.h: 该接口声明所在的头文件。
* @see  HI_MDM_TASK_UnLock
* @since HiMDL_V200R001C00
*/
HI_VOID HI_MDM_TASK_Lock(HI_VOID);

/**
* @ingroup  hct_task
* @brief  解锁任务调度。
*
* @par 描述:  
* 解锁任务调度。
* @attention 无。
*
* @param 无。
*
* @retval 无。
*
* @par Dependency:
* @li hi_mdm_task.h: 该接口声明所在的头文件。
* @see  HI_MDM_TASK_Lock。
* @since HiMDL_V200R001C00
*/
HI_VOID HI_MDM_TASK_UnLock(HI_VOID);


/**
* @ingroup  hct_task
* @brief  获取当前任务ID。
*
* @par 描述:  
* 获取当前任务ID。
* @attention 无。
*
* @param 无。
*
* @retval 任务ID 当前任务ID，取值为#HI_INVALID_TASK_ID表示非法任务ID。
*
* @par Dependency:
* @li hi_mdm_task.h: 该接口声明所在的头文件。
* @see  HI_MDM_TASK_Lock。
* @since HiMDL_V200R001C00
*/
HI_U32 HI_MDM_TASK_GetCurrentID(HI_VOID);

/**
* @ingroup  hct_task
* @brief  获取任务信息。
*
* @par 描述:  
* 获取任务信息。
* @attention 无。
*
* @param ulTaskId [in]任务ID。
* @param pstTaskInfo [out]任务信息。
*
* @retval #HI_ERR_SUCCESS 成功。
* @retval 其他值 失败，详见hi_errno.h。
*
* @par Dependency:
* @li hi_mdm_task.h: 该接口声明所在的头文件。
* @see  HI_MDM_TASK_Lock。
* @since HiMDL_V200R001C00
*/
HI_U32 HI_MDM_TASK_GetTaskInfo(HI_U32 ulTaskId,HI_TASK_INFO_S *pstTaskInfo);

/**
* @ingroup  hct_task
* @brief 任务睡眠。
*
* @par 描述:
* 在中断处理函数中或者在锁任务的情况下，执行HI_MDM_Sleep操作会失败。
* @attention 
* 入参小于10ms时，当做10ms处理；大于10ms时,有效值取为整数部分，如睡眠时间设定为15ms，则实际有效值为10ms。
* @param  ulMs [IN] 睡眠时间, 单位:ms，精度为10ms。
*
* @retval #HI_ERR_SUCCESS 成功。
* @retval 其他值 失败，详见hi_errno.h。
*
* @par Dependency:
* @li hi_mdm_task.h: 该接口声明所在的头文件。
* @see 无。
* @since HiMDL_V200R001C00
*/
HI_U32 HI_MDM_Sleep(HI_U32 ulMs);
#endif

