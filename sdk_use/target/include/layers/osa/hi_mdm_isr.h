/**@defgroup hct_isr 中断
 * @ingroup osa
 */

#ifndef __HI_MDM_ISR_H__
#define __HI_MDM_ISR_H__

/**
* @ingroup  hct_isr
* @brief  isr回调函数的类型
*
* @par 
* 
* @attention 无。
*
* @param  data1 [IN] 类型 #HI_U32 回调入参1
* @param  data2 [IN] 类型 #HI_U32 回调入参2
* @param  data3 [IN] 类型 #HI_U32 回调入参3
* @param  data4 [IN] 类型 #HI_U32 回调入参4
* @param  data5 [IN] 类型 #HI_U32 回调入参5
*
* @retval 无.
* @par Dependency:
* <ul><li>hi_mdm_isr.h: 文件用于描述isr相关接口.</li></ul>
* @see 无。
* @since HiMDL_V200R001C00
*/
typedef HI_VOID (*HISR_PROC_FUNC)(HI_U32 ulData1,HI_U32 ulData2,HI_U32 ulData3,HI_U32 ulData4,HI_U32 ulData5);


/**
* @ingroup  hct_isr
* @brief  关中断。
*
* @par 描述:  
* 关中断后不能执行引起调度的函数，如HI_MDM_Sleep或其他阻塞接口。
* @attention 关中断仅保护可预期的短时间的操作，否则影响中断响应，可能引起性能问题。
* 
* @param 无。
*
* @retval #HI_U32 关中断前的中断状态。
*
* @par Dependency:
* @li hi_mdm_isr.h: 该接口声明所在的头文件。
* @see  HI_MDM_INT_Restore
* @since HiMDL_V200R001C00
*/
HI_U32 HI_MDM_INT_Lock(HI_VOID);

/**
* @ingroup  hct_isr
* @brief  恢复关中断前的状态。
*
* @par 描述:  
*  恢复关中断前的状态。
* @attention 入参必须是与之对应的关中断时保存的关中断之前的CPSR的值。
*
* @param  ulIntSave [IN] 中断状态。
*
*
* @par Dependency:
* @li hi_mdm_isr.h: 该接口声明所在的头文件。
* @see  HI_MDM_INT_Lock
* @since HiMDL_V200R001C00
*/
HI_VOID HI_MDM_INT_Restore(HI_U32 ulIntSave);

/**
* @ingroup  hct_isr
* @brief  使能指定中断。
*
* @par 描述:  
* 使能指定中断。
* @attention 无。
*
* @param  ulVector [IN] 中断号。
*
* @retval #HI_ERR_SUCCESS 成功。
* @retval 其他值 失败，详见hi_errno.h。
*
* @par Dependency:
* @li hi_mdm_isr.h: 该接口声明所在的头文件。
* @see  HI_MDM_IRQ_Disable
* @since HiMDL_V200R001C00
*/
HI_U32 HI_MDM_IRQ_Enable(HI_U32 ulVector);

/**
* @ingroup  hct_isr
* @brief  去使能指定中断。
*
* @par 描述:  
* 去使能指定中断。
* @attention 无。
*
* @param  ulVector [IN] 中断号。
*
* @retval #HI_ERR_SUCCESS 成功。
* @retval 其他值 失败，详见hi_errno.h。
*
* @par Dependency:
* @li hi_mdm_isr.h: 该接口声明所在的头文件。
* @see  HI_MDM_IRQ_Enable
* @since HiMDL_V200R001C00
*/
HI_U32 HI_MDM_IRQ_Disable(HI_U32 ulVector);

/**
* @ingroup  hct_isr
* @brief 注册中断。
*
* @par 描述:  
* 注册中断。
* @attention 中断处理程序耗时不能过长，影响CPU对中断的及时响应。
*
* @param  ulVector [IN] 中断号。
* @param  ulPriority [IN] 中断优先级。
* @param  pIrqRoutine [IN] 中断回调函数。
* @param  ulPara [IN] 中断参数。
*
* @retval #HI_ERR_SUCCESS 成功。
* @retval 其他值 失败，详见hi_errno.h。
*
* @par Dependency:
* @li hi_mdm_isr.h: 该接口声明所在的头文件。
* @see  HI_MDM_IRQ_Free
* @since HiMDL_V200R001C00
*/
HI_U32 HI_MDM_IRQ_Request (HI_U32 ulVector,HI_U16 ulPriority,HI_PVOID pIrqRoutine,HI_U32 ulPara);

/**
* @ingroup  hct_isr
* @brief  清除注册中断。
*
* @par 描述:  
* 清除注册中断。
* @attention 无。
*
* @param  ulVector [IN] 类型 #HI_U32  中断号。
*
* @retval #HI_ERR_SUCCESS 成功。
* @retval 其他值 失败，详见hi_errno.h。
*
* @par Dependency:
* @li hi_mdm_isr.h: 该接口声明所在的头文件。
* @see  HI_MDM_IRQ_Request
* @since HiMDL_V200R001C00
*/
HI_U32 HI_MDM_IRQ_Free (HI_U32 ulVector);

/**
* @ingroup  hct_isr
* @brief  创建中断HISR函数，中断底半部程序挂载到workqueue。
*
* @par 描述:  
* 创建中断HISR函数，中断底半部程序挂载到workqueue。
* @attention 无。
*
* @param  ulPriority [IN] 中断底半部优先级，取值为0-15 值越小对应优先级越高。
* @param  pfnHisrFunc [IN] 中Hisr函数入口。
* @param  ulData1 [IN] 参数1。
* @param  ulData2 [IN] 参数2。
* @param  ulData3 [IN] 参数3。
* @param  ulData4 [IN] 参数4。
* @param  ulData5 [IN] 参数5。
*
* @retval #HI_ERR_SUCCESS 成功。
* @retval 其他值 失败，详见hi_errno.h。
*
* @par Dependency:
* @li hi_mdm_isr.h: 该接口声明所在的头文件。
* @see  无。
* @since HiMDL_V200R001C00
*/
HI_U32 HI_MDM_IRQ_AddJob(HI_U32 ulPriority, HISR_PROC_FUNC pfnHisrFunc,HI_U32 ulData1,HI_U32 ulData2,HI_U32 ulData3,HI_U32 ulData4,HI_U32 ulData5);

/**
* @ingroup  hct_isr
* @brief  检查是否在中断上下文中。
*
* @par 描述:  
* 检查是否在中断上下文中。
* @attention 无。
*
* @param 无。
*
* @retval HI_TRUE 在中断上下文。
* @retval HI_FALSE 不在中断上下文。
*
* @par Dependency:
* @li hi_mdm_isr.h: 该接口声明所在的头文件。
* @see  无。
* @since HiMDL_V200R001C00
*/
HI_BOOL HI_MDM_IsIntContext(HI_VOID);
#endif
