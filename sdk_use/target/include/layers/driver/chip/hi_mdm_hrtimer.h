
/**@defgroup hct_hrtimer 高精定时器
 * @ingroup drivers
 */

#ifndef __HI_MDM_HRTIMER_H__
#define __HI_MDM_HRTIMER_H__

#define HI_FAST_TIMER_MAGIC 0x55667788

struct HI_hrtimer_list_node_TYPE_S
{
        struct HI_hrtimer_list_node_TYPE_S *prv;
        struct HI_hrtimer_list_node_TYPE_S *next;
        HI_VOID *parent;
};//内部使用的链表结构，接口使用者无需关心

/**
 * @ingroup hct_hrtimer
 * 高精定时器配置参数，空间由使用者申请，定时器运行生命周期内不允许修改或释放。
 * 用户在第一次且仅第一次启动定时器之前需要memset该空间为0,之后不允许修改该空间内容(特别是定时器运行期间)。 
 */
typedef struct hiHRTimer_TYPE_S
{
	HI_U32 expires;                             /**< 超时时间，单位:微秒(us) */
	HI_VOID (*func)(HI_U32 arg);                /**< 回调函数 */
	HI_U32 data;                                /**< 回调函数参数 */
	/********* DO NOT UPDATE THESE STAFF *****/
#ifdef PRODUCT_CFG_PRODUCT_TYPE_NDM
    HI_U32 attribute;                           /**< 内部使用，用户不要输入 */
    HI_VOID (*funcLisr)(HI_U32 arg);            /**< 内部使用，用户不要输入 */
#endif
	HI_U32 remaining_time;                      /**< 内部使用，用户不要输入 */
	struct HI_hrtimer_list_node_TYPE_S node;    /**< 内部使用，用户不要输入 */
}HI_HRTIMER_TYPE_S;


/**
* @ingroup  hct_hrtimer
* @brief  创建一个高精度定时器。
*
* @par 描述:
* 创建一个高精度定时器，如果定时器已经启动，调用该接口时会停止当前定时器重新启动。
* @attention 
* li 定时器最大个数限制为100个。
* li 入参timer必须为全局变量。
* li 本定时器回调函数执行于中断上下文，回调函数尽量简单，不能使用睡眠、等待信号量等引起调度的函数。
*
* @param pTimer [IN] 类型#HI_HRTIMER_TYPE_S*。定时器参数。
*
* @retval 无。
*
* @par Dependency:
* @li hi_mdm_hrtimer.h: 该接口声明所在的头文件。
* @see  无。
* @since HiMDL_V200R001C00
*/
#define HI_MDM_HRTIMER_Start(pTimer)         HI_HRTIMER_Start(pTimer)

/**
* @ingroup  hct_hrtimer
* @brief 停止指定的高精度定时器。
*
* @par 描述:
* 停止指定的高精度定时器，如果调用该接口时定时器已经停止，不会有有任何效果。
* @attention 
*
* @param pTimer [IN] 类型#HI_HRTIMER_TYPE_S*。定时器参数，为创建该高精定时器传入的全局变量。
*
* @retval 无。
*
* @par Dependency:
* @li hi_mdm_hrtimer.h: 该接口声明所在的头文件。
* @see  无。
* @since HiMDL_V200R001C00
*/
#define HI_MDM_HRTIMER_Stop(pTimer)         HI_HRTIMER_Stop(pTimer)

/**
* @ingroup  hct_hrtimer
* @brief  修改高精度定时器参数
*
* @par 描述:
* 修改高精度定时器参数，使用用户输入的超时时间expires，回调函数func，回调参数data值，替换timer的初始设置值。
* @attention 无。
*
* @param pTimer [IN] 类型#HI_HRTIMER_TYPE_S *。 要修改的定时器指针，为创建该高精定时器传入的全局变量，不能为空。
* @param expires [IN] 类型#HI_U32。 超时时间，单位:微秒(us)。
* @param func [IN] 类型#HI_VOID (*func)(HI_U32 arg); 回调函数，不能为空。
* @param data [IN] 类型#HI_U32。回调函数的参数。
*
* @retval 无。
*
* @par Dependency:
* @li hi_stdlib.h: 该接口声明所在的头文件。
* @see  无。
* @since HiMDL_V200R001C00
*/
#define HI_MDM_HRTIMER_Modify(pTimer, expires, func, data)           HI_HRTIMER_Modify(pTimer, expires, func, data)


#endif