/**@defgroup hct_msgqueue 消息队列
 * @ingroup osa
 */
#ifndef __HI_MDM_MSG_H__
#define __HI_MDM_MSG_H__
#include <hi_types.h>
#include <hi_mdm_types.h>
#include <hi_config.h>

/**
 * @ingroup hct_msgqueue
 * 消息个数非法值定义
 */
#define HI_ERR_MSG_INVALID_MSG_NUM 0xFFFFFFFF 


/**
 * @ingroup hct_msgqueue
 * 消息结构体定义
 */
typedef struct
{
    hi_u32 ulMsgId;                             /**< 消息ID */
    hi_u32 ulTimeStamp;                         /**< 发送消息时间，系统自动填写，调用者无需关心*/
    uintptr_t ulParam[HI_SYS_MSG_PARAM_NUM_MAX];   /**< 消息参数 */
} HI_SYS_QUEUE_MSG_S;

/**
* @ingroup  hct_msgqueue
* @brief   创建消息队列。
*
* @par  描述:
* 创建消息队列。
* @attention 无。
*
* @param  pQueueId [OUT] 所创建的消息队列句柄。
* @param  szName [IN] 消息队列名称，需要由上层来存储不释放，底层不缓存。
* @param  usMsgArrayCnt [IN] 消息队列长度。消息队列无长度限制，只要在内存池中能成功申请到内存即可。
*
* @retval #HI_ERR_SUCCESS 创建成功。
* @retval #HI_ERR_MSG_INVALID_PARAM 入参错误，句柄指针为空、名字地址为空、消息队列长度为0。
* @retval #HI_ERR_MSG_CREATE_Q_FAIL 创建信号量错误，比如内存不足，预设值消息队列使用完毕等。
*
* @par Dependency:
* @li hi_mdm_msg.h: 该接口声明所在的头文件。
* @li hi_errno.h: 文件配置错误码。
* @see HI_MDM_MSG_DeleteQueue 
* @since HiMDL_V200R001C00
*/
HI_U32 HI_MDM_MSG_CreateQueue(HI_OUT HI_U32 * pQueueId,HI_CHAR szName[8],HI_U16 usMsgArrayCnt);
/**
* @ingroup  hct_msgqueue
* @brief  删除消息队列。
*
* @par  描述:
* 删除消息队列。
* @attention 无。
*
* @param  pQueueId [IN] 消息队列句柄。
*
* @retval #HI_ERR_SUCCESS 删除成功。
* @retval #HI_ERR_MSG_Q_DELETE_FAIL 删除消息队列错误，比如ID越界，
* 消息队列未创建，消息队列正在使用中无法删除等。
*
* @par Dependency:
* @li hi_mdm_msg.h: 该接口声明所在的头文件。
* @li hi_errno.h: 文件配置错误码。
* @see HI_MDM_MSG_CreateQueue 
* @since HiMDL_V200R001C00
*/
HI_U32 HI_MDM_MSG_DeleteQueue(HI_U32 ulQueueId);
/**
* @ingroup  hct_msgqueue
* @brief   发送消息。
*
* @par 描述: 
* 发送消息。
* @attention 
* @li 当ulTimeOut为0时，该接口支持在锁任务上下文中使用。
* @li 当ulTimeOut为0时，该接口支持在LISR上下文中使用。
*
* @param  ulQueueId [IN] 消息队列句柄。
* @param  pstMsg [IN] 消息内容指针。
* @param  ulTimeOut [IN] 消息发送超时时间，立即发送就写0。
*
* @retval #HI_ERR_SUCCESS 发送成功。
* @retval #HI_ERR_MSG_SEND_FAIL 发送消息队列错误，包括入参错误，
* 消息队列未创建，发送数据大于队列创建时设置大小，中断中使用但超时时间不为0。
* @retval #HI_ERR_MSG_INVALID_PARAM 入参错误，消息队列指针为空。
*
* @par Dependency:
* @li hi_mdm_msg.h: 该接口声明所在的头文件。
* @li hi_errno.h: 文件配置错误码。
* @see HI_MDM_MSG_Wait 
* @since HiMDL_V200R001C00
*/
HI_U32 HI_MDM_MSG_Send(HI_U32 ulQueueId, HI_SYS_QUEUE_MSG_S* pstMsg,HI_U32 ulTimeOut);
/**
* @ingroup  hct_msgqueue
* @brief  接收消息。
*
* @par 描述: 
* 接收消息。
* @attention 无。
*
* @param  ulQueueId [IN] 消息队列句柄。
* @param  pstMsg [OUT] 消息内容指针。
* @param  ulTimeOut [IN] 消息接收超时时间,如果表示永久等待则使用#HI_SYS_WAIT_FOREVER。
*
* @retval #HI_ERR_SUCCESS 消息接收成功。
* @retval #HI_ERR_MSG_WAIT_FAIL 等待消息队列错误，比如入参不正确，消息队列未创建，
* 等待消息大小大于队列创建时设置大小，中断中等待超时非零的消息队列。
* @retval #HI_ERR_MSG_INVALID_PARAM 入参错误，消息队列指针为空。
* @retval #HI_ERR_MSG_WAIT_TIME_OUT 等待超时未收到消息。
*
* @par Dependency:
* @li hi_mdm_msg.h: 该接口声明所在的头文件。
* @li hi_errno.h: 文件配置错误码。
* @see HI_MDM_MSG_Send 
* @since HiMDL_V200R001C00
*/
HI_U32 HI_MDM_MSG_Wait(HI_U32 ulQueueId, HI_OUT HI_SYS_QUEUE_MSG_S* pstMsg, HI_U32 ulTimeOut);
/**
* @ingroup  hct_msgqueue
* @brief   检查消息队列是否已满。
*
* @par 描述: 
* 检查消息队列是否已满。
* @attention 
* @li 消息队列ID无效时，该接口返回HI_TRUE。
* @li 该接口支持在锁任务上下文中使用。
* @li 该接口支持在LISR上下文中使用。
*
* @param  pQueueId [IN] 消息队列句柄。
* 
* @retval #HI_TRUE  消息队列已满。
* @retval #HI_FALSE 消息队列未满。
*
* @par Dependency:
* @li hi_mdm_msg.h: 该接口声明所在的头文件。
* @see 无 
* @since HiMDL_V200R001C00
*/
HI_BOOL HI_MDM_MSG_IsQueueFull(HI_U32 ulQueueId);
/**
* @ingroup  hct_msgqueue
* @brief   获取消息队列中含有的消息个数。
*
* @par 描述:  
* 获取ID号为ulQueueId的消息队列中当前含有的消息个数。
* @attention 
* @li 该接口支持在锁任务上下文中使用。
* @li 该接口支持在LISR上下文中使用。
*
* @param  ulQueueId [IN] 消息队列句柄。
* 
* @retval #HI_ERR_MSG_INVALID_MSG_NUM  读取消息队列出错。
* @retval 其他值 消息队列中含有的消息个数。
*
* @par Dependency:
* @li hi_mdm_msg.h: 该接口声明所在的头文件。
* @see 无。
* @since HiMDL_V200R001C00
*/
HI_U32 HI_MDM_MSG_GetCurrentMsgNum(HI_U32 ulQueueId);
/**
* @ingroup  hct_msgqueue
* @brief  获取消息队列最大可缓存消息个数。
*
* @par 描述:  
* 获取ID号为ulQueueId的消息队列中最大可缓存的消息个数。 
* @attention 
* @li 该接口支持在锁任务上下文中使用。
* @li 该接口支持在LISR上下文中使用。
*
* @param  pQueueId [IN] 消息队列句柄。
*
* @retval #HI_ERR_MSG_INVALID_MSG_NUM  读取消息队列出错，入参错误、消息队列未创建。
* @retval 其他值 该消息队列可缓存的消息个数。 
*
* @par Dependency:
* @li hi_mdm_msg.h: 文件用于描述消息队列相关接口。
* @see 无。
* @since HiMDL_V200R001C00
*/
HI_U32 HI_MDM_MSG_GetTotalMsgNum(HI_U32 ulQueueId);

#endif

