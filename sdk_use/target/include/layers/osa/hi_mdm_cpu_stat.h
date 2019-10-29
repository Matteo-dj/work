/**@defgroup hct_cpup_track  CPU
 * @ingroup osa
 */

#ifndef __HI_MDM_CPU_STAT_H__
#define __HI_MDM_CPU_STAT_H__
#include <hi_types.h>
#include <hi_mdm_types.h>

/**
 * @ingroup hct_cpup_track
 * task个数的非法值
 */
#define HI_CPU_STAT_INVALID_COUNT 0xFFFFFFFF

/**
 * @ingroup hct_cpup_track
 * 将数组下标id 转换成task id
 */
#define HI_CPU_STAT_ARRY_ID_TO_TASK_ID(x) (x)

/**
 * @ingroup hct_cpup_track
 * 将task id 转换成数组下标 id
 */
#define HI_CPU_STAT_TASK_ID_TO_ARRAY_ID(x) (x)

/**
 * @ingroup hct_cpup_track
 * 一条调度信息,取值参考 #HI_TRACE_TYPE
 */
typedef struct
{
    HI_U32 ulTraceType; /**< 调度信息类型，参照#HI_TRACE_TYPE*/
    HI_U32 ulData0;     /**< 相关数据0    */
    HI_U32 ulData1;     /**< 相关数据1    */
    HI_U32 ulEntry;     /**< 指向调度者，一般为回调函数或LR*/
    HI_U32 ulCurTime;   /**< 调度时刻(单位:us)*/
}HI_CPU_STAT_TRACK_ITEM_S;

/**
 * @ingroup hct_cpup_track
 * 调度信息概要
 */
typedef struct
{
    HI_U32 ulLockIdx;           /**< 当前信息中最早记录的那条信息的下标 */
    HI_U32 ulMaxRecordCount;    /**< 可记录信息条数 */
}HI_CPU_STAT_TRACK_INFO_S;

/**
 * @ingroup hct_cpup_track
 * 调度统计类型
 */
typedef enum
{
    HI_TRACE_ISR_FLAG   =   0x10,/**< 使能中断调度统计 */
    HI_TRACE_IRQ_FLAG   =   0x20,/**< 使能开关中断统计 */
    HI_TRACE_TASK_FLAG  =   0x40,/**< 使能任务调度统计 */
    HI_TRACE_WORKQ_FLAG =   0x80,/**< 使能中断后半部操作统计 */
    HI_TRACE_SWTMR_FLAG =   0x100,/**< 使能定时器操作统计 */
    HI_TRACE_MUX_FLAG   =   0x200,/**< 使能互斥锁操作统计 */
    HI_TRACE_SEM_FLAG   =   0x400,/**< 使能信号量操作统计 */
    HI_TRACE_QUE_FLAG   =   0x800,/**< 使能消息操作统计 */
    HI_TRACE_EVENT_FLAG =   0x1000/**< 使能事件操作统计 */
}HI_TRACE_MASK;//LOS_TRACE_MASK

/**
 * @ingroup hct_cpup_track
 * 调度项取值意义描述
 */
typedef enum
{
    HI_TRAP_INT        = HI_TRACE_ISR_FLAG,/**< 进入中断,ulEntry=NA,ulData0=中断号,ulData1=NA */

    HI_ENABLE_IRQ      = HI_TRACE_IRQ_FLAG|0,/**< 关中断,ulEntry=LR,ulData0=NA,ulData1=NA */
    HI_DISABLE_IRQ     = HI_TRACE_IRQ_FLAG|1,/**< 开中断,ulEntry=LR,ulData0=NA,ulData1=NA */
   
    HI_CREATE_TSK      = HI_TRACE_TASK_FLAG|0,/**< 创建任务,ulEntry=NA,ulData0=任务ID,ulData1=NA */
    HI_SCHEDULE_TSK    = HI_TRACE_TASK_FLAG|1,/**< 任务调度,ulEntry=NA,ulData0=任务ID,ulData1=NA */
    HI_DELETE_TSK      = HI_TRACE_TASK_FLAG|2,/**< 删除任务,ulEntry=NA,ulData0=任务ID,ulData1=NA */
    
    HI_ENQUEUE_WORKQ   = HI_TRACE_WORKQ_FLAG|0,/**< 添加中断后半段,ulEntry=后半段执行函数,ulData0=后半段优先级,ulData1=NA */
    HI_SCHEDULE_WORKQ  = HI_TRACE_WORKQ_FLAG|1,/**< 中断后半段执行,ulEntry=后半段执行函数,ulData0=后半段优先级,ulData1=NA */
    HI_DEQUEUE_WORKQ   = HI_TRACE_WORKQ_FLAG|2,/**< 不支持,ulEntry=NA,ulData0=NA,ulData1=NA   */     

    HI_CREATE_SWTMR    = HI_TRACE_SWTMR_FLAG|0,/**< 创建定时器,ulEntry=定时器回调函数,ulData0=定时器结构指针(无需关注),ulData1=定时器超时时间(tick) */
    HI_START_SWTMR     = HI_TRACE_SWTMR_FLAG|1,/**< 启动定时器(无需关注),ulEntry=定时器回调函数,ulData0=定时器结构指针(无需关注),ulData1=定时器超时时间(tick) */
    HI_MODIFY_SWTMR    = HI_TRACE_SWTMR_FLAG|2,/**< 启动定时器,ulEntry=定时器回调函数,ulData0=定时器结构指针(无需关注),ulData1=定时器超时时间(tick) */
    HI_SCHDING_SWTMR   = HI_TRACE_SWTMR_FLAG|3,/**< 定时器回调函数执行前,ulEntry=定时器回调函数,ulData0=定时器结构指针(无需关注),ulData1=伪ID(无需关注) */
    HI_SCHDED_SWTMR    = HI_TRACE_SWTMR_FLAG|4,/**< 定时器回调函数执行后,ulEntry=定时器回调函数,ulData0=定时器结构指针(无需关注),ulData1=伪ID(无需关注) */
    HI_STOP_SWTMR      = HI_TRACE_SWTMR_FLAG|5,/**< 停止定时器,ulEntry=定时器回调函数,ulData0=定时器结构指针(无需关注),与前一个定时器的差值(无需关注) */
    HI_DEL_SWTMR       = HI_TRACE_SWTMR_FLAG|6,/**< 删除定时器,ulEntry=定时器回调函数,ulData0=定时器结构指针(无需关注),与前一个定时器的差值(无需关注) */
    HI_DROP_SWTMR      = HI_TRACE_SWTMR_FLAG|7,/**< 不支持,ulEntry=NA,ulData0=NA,ulData1=NA */

    HI_MUX_CREATE      = HI_TRACE_MUX_FLAG|0,/**< 不支持,ulEntry=NA,ulData0=NA,ulData1=NA */
    HI_MUX_PEND        = HI_TRACE_MUX_FLAG|1,/**< 不支持,ulEntry=NA,ulData0=NA,ulData1=NA */
    HI_MUX_POST        = HI_TRACE_MUX_FLAG|2,/**< 不支持,ulEntry=NA,ulData0=NA,ulData1=NA */
    HI_MUX_DEL         = HI_TRACE_MUX_FLAG|3,/**< 不支持,ulEntry=NA,ulData0=NA,ulData1=NA */

    HI_SEM_CREATE      = HI_TRACE_SEM_FLAG|0,/**< 不支持,ulEntry=NA,ulData0=NA,ulData1=NA */
    HI_SEM_PEND        = HI_TRACE_SEM_FLAG|1,/**< 不支持,ulEntry=NA,ulData0=NA,ulData1=NA */
    HI_SEM_POST        = HI_TRACE_SEM_FLAG|2,/**< 不支持,ulEntry=NA,ulData0=NA,ulData1=NA */
    HI_SEM_DEL         = HI_TRACE_SEM_FLAG|3,/**< 不支持,ulEntry=NA,ulData0=NA,ulData1=NA */
    
    HI_QUE_CREATE      = HI_TRACE_QUE_FLAG|0,/**< 不支持,ulEntry=NA,ulData0=NA,ulData1=NA */
    HI_QUE_WRITE       = HI_TRACE_QUE_FLAG|1,/**< 不支持,ulEntry=NA,ulData0=NA,ulData1=NA */
    HI_QUE_READ        = HI_TRACE_QUE_FLAG|2,/**< 不支持,ulEntry=NA,ulData0=NA,ulData1=NA */
    HI_QUE_DEL         = HI_TRACE_QUE_FLAG|3,/**< 不支持,ulEntry=NA,ulData0=NA,ulData1=NA */

    HI_EVENT_CREATE    = HI_TRACE_EVENT_FLAG|0,/**< 不支持,ulEntry=NA,ulData0=NA,ulData1=NA */
    HI_EVENT_WRITE     = HI_TRACE_EVENT_FLAG|1,/**< 不支持,ulEntry=NA,ulData0=NA,ulData1=NA */
    HI_EVENT_READ      = HI_TRACE_EVENT_FLAG|2,/**< 不支持,ulEntry=NA,ulData0=NA,ulData1=NA */
    HI_EVENT_CLEAR     = HI_TRACE_EVENT_FLAG|3,/**< 不支持,ulEntry=NA,ulData0=NA,ulData1=NA */
    HI_EVENT_DEL       = HI_TRACE_EVENT_FLAG|4,/**< 不支持,ulEntry=NA,ulData0=NA,ulData1=NA */
}HI_TRACE_TYPE;//LOS_TRACE_TYPE


/**
* @ingroup  hct_cpup_track
* @brief  启动调度统计。
*
* @par 描述:
* 用户通过该接口启动调度统计。
* @attention 无。
*
* @param  usMaxRecordCount [IN] 最多可以记录的轨迹条数。
*
* @retval #HI_ERR_SUCCESS 成功。
* @retval 其他值 失败，详见hi_errno.h。
*
* @par Dependency:
* @li hi_mdm_cpu_stat.h: 该接口声明所在的头文件。
* @see  HI_MDM_CPU_TrackSetType | HI_MDM_CPU_TrackGetType | HI_MDM_CPU_TrackLock | HI_MDM_CPU_TrackGetInfo | HI_MDM_CPU_TrackGetLockInfoByIdx | HI_MDM_CPU_TrackStop
* @since HiMDL_V200R001C00
*/
HI_EXTERN HI_U32 HI_MDM_CPU_TrackStart(HI_U16 usMaxRecordCount);

/**
* @ingroup  hct_cpup_track
* @brief  设置调度统计类型。
*
* @par 描述:
* 用户通过该接口配置希望统计的调度类型。
* @attention 无。
*
* @param  trace_type [IN] 要统计的类型，取值参考 #HI_TRACE_MASK。
*
* @retval #HI_ERR_SUCCESS 成功。
* @retval 其他值 失败，详见hi_errno.h。

* @par Dependency:
* @li hi_mdm_cpu_stat.h: 该接口声明所在的头文件。
* @see  HI_MDM_CPU_TrackStart | HI_MDM_CPU_TrackGetType | HI_MDM_CPU_TrackLock | HI_MDM_CPU_TrackGetInfo | HI_MDM_CPU_TrackGetLockInfoByIdx | HI_MDM_CPU_TrackStop
* @since HiMDL_V200R001C00
*/
HI_EXTERN HI_VOID HI_MDM_CPU_TrackSetType(HI_U32 trace_type);

/**
* @ingroup  hct_cpup_track
* @brief  获取调度统计类型。
*
* @par 描述:
* 用户通过该接口可以获取当前配置的统计类型。
* @attention 无。
*
* @retval #HI_U32类型数值 当前的统计类型，取值参考 #HI_TRACE_MASK。
*
* @par Dependency:
* @li hi_mdm_cpu_stat.h: 该接口声明所在的头文件。
* @see  HI_MDM_CPU_TrackStart | HI_MDM_CPU_TrackSetType | HI_MDM_CPU_TrackLock | HI_MDM_CPU_TrackGetInfo | HI_MDM_CPU_TrackGetLockInfoByIdx | HI_MDM_CPU_TrackStop
* @since HiMDL_V200R001C00
*/
HI_EXTERN HI_U32 HI_MDM_CPU_TrackGetType(HI_VOID);

/**
* @ingroup  hct_cpup_track
* @brief  停止调度统计。
*
* @par 描述:
* 用户通过调用该接口，停止调度统计。
* @attention 无。
*
* @retval #HI_ERR_SUCCESS 成功。
* @retval 其他值 失败，详见hi_errno.h。
*
* @par Dependency:
* @li hi_mdm_cpu_stat.h: 该接口声明所在的头文件。
* @see  HI_MDM_CPU_TrackStart | HI_MDM_CPU_TrackSetType | HI_MDM_CPU_TrackGetType | HI_MDM_CPU_TrackLock | HI_MDM_CPU_TrackGetInfo | HI_MDM_CPU_TrackGetLockInfoByIdx
* @since HiMDL_V200R001C00
*/
HI_EXTERN HI_U32 HI_MDM_CPU_TrackStop(HI_VOID);

/**
* @ingroup  hct_cpup_track
* @brief  锁定调度统计信息。
*
* @par 描述:
* 用户获取统计信息之前需要调用该接口，
*将该时刻的统计信息锁定到一块备份内存中，备份内存中的信息不会随着调度而变化。
* @attention 无。
*
* @retval #HI_ERR_SUCCESS 成功。
* @retval 其他值 失败，详见hi_errno.h。
*
* @par Dependency:
* @li hi_mdm_cpu_stat.h: 该接口声明所在的头文件。
* @see  HI_MDM_CPU_TrackStart | HI_MDM_CPU_TrackSetType|HI_MDM_CPU_TrackGetType|HI_MDM_CPU_TrackGetInfo|HI_MDM_CPU_TrackGetLockInfoByIdx|HI_MDM_CPU_TrackStop
* @since HiMDL_V200R001C00
*/
HI_EXTERN HI_U32 HI_MDM_CPU_TrackLock(HI_VOID);

/**
* @ingroup  hct_cpup_track
* @brief  获取调度概要信息。
*
* @par 描述:
* 用户通过调用该接口获取概要调度信息。
* @attention 无。
*
* @param  pInfo [OUT] 调度信息概要 。
*
* @retval #HI_ERR_SUCCESS 成功。
* @retval 其他值 失败，详见hi_errno.h。
*
* @par Dependency:
* @li hi_mdm_cpu_stat.h: 该接口声明所在的头文件。
* @see  HI_MDM_CPU_TrackStart | HI_MDM_CPU_TrackSetType | HI_MDM_CPU_TrackGetType | HI_MDM_CPU_TrackLock | HI_MDM_CPU_TrackGetLockInfoByIdx | HI_MDM_CPU_TrackStop
* @since HiMDL_V200R001C00
*/
HI_EXTERN HI_U32 HI_MDM_CPU_TrackGetInfo(HI_CPU_STAT_TRACK_INFO_S *pInfo);

/**
* @ingroup  hct_cpup_track
* @brief  获取一条调度信息。
*
* @par 描述:
* 用户通过调用该接口获取一条调度信息。
* @attention 无。
*
* @param  ulIdx [IN] 调度信息下标号。
* @param  pItemInfo [OUT] 调度信息内容。
*
* @retval #HI_ERR_SUCCESS 成功。
* @retval 其他值 失败，详见hi_errno.h。
*
* @par Dependency:
* @li hi_mdm_cpu_stat.h: 该接口声明所在的头文件。
* @see  HI_MDM_CPU_TrackStart | HI_MDM_CPU_TrackSetType | HI_MDM_CPU_TrackGetType | HI_MDM_CPU_TrackLock | HI_MDM_CPU_TrackGetInfo | HI_MDM_CPU_TrackStop
* @since HiMDL_V200R001C00
*/
HI_EXTERN HI_U32 HI_MDM_CPU_TrackGetLockInfoByIdx(HI_U32 ulIdx,HI_CPU_STAT_TRACK_ITEM_S *pItemInfo);

/**
* @ingroup  hct_cpup_track
* @brief  启动CPU占用率统计。
*
* @par 描述:
* 用户通过调用该接口启动CPU占用率统计。
* @attention 无。
*
* @param 无。
*
* @retval #HI_ERR_SUCCESS 成功。
* @retval 其他值 失败，详见hi_errno.h。
*
* @par Dependency:
* @li hi_mdm_cpu_stat.h: 该接口声明所在的头文件。
* @see   HI_MDM_CPU_PercentStop | HI_MDM_CPU_PercentLock | HI_MDM_CPU_PercentGetTotalTime  | HI_MDM_CPU_PercentGetTimeByTaskIdx | HI_MDM_CPU_PercentGetTaskCount
* @since HiMDL_V200R001C00
*/
HI_EXTERN HI_U32 HI_MDM_CPU_PercentStart(HI_VOID);

/**
* @ingroup  hct_cpup_track
* @brief  停止CPU占用率统计。
*
* @par 描述:
* 用户通过调用该接口停止CPU占用率统计。
* @attention 无。
*
* @retval #HI_ERR_SUCCESS 成功。
* @retval 其他值 失败，详见hi_errno.h。
*
* @par Dependency:
* @li hi_mdm_cpu_stat.h: 该接口声明所在的头文件。
* @see  HI_MDM_CPU_PercentStart | HI_MDM_CPU_PercentLock | HI_MDM_CPU_PercentGetTotalTime | HI_MDM_CPU_PercentGetTimeByTaskIdx | HI_MDM_CPU_PercentGetTaskCount
* @since HiMDL_V200R001C00
*/
HI_EXTERN HI_U32 HI_MDM_CPU_PercentStop(HI_VOID);

/**
* @ingroup  hct_cpup_track
* @brief  锁定CPU占用率统计信息。
*
* @par 描述:
* 用户获取统计信息之前需要调用该接口，
*将该时刻的统计信息锁定到一块备份内存中，备份内存中的信息不会随着调度而变化。
* @attention 无。
*
* @retval #HI_ERR_SUCCESS 成功。
* @retval 其他值 失败，详见hi_errno.h。
*
* @par Dependency:
* @li hi_mdm_cpu_stat.h: 该接口声明所在的头文件。
* @see  HI_MDM_CPU_PercentStart | HI_MDM_CPU_PercentStop | HI_MDM_CPU_PercentGetTotalTime | HI_MDM_CPU_PercentGetTimeByTaskIdx | HI_MDM_CPU_PercentGetTaskCount
* @since HiMDL_V200R001C00
*/
HI_EXTERN HI_U32 HI_MDM_CPU_PercentLock(HI_VOID);

/**
* @ingroup  hct_cpup_track
* @brief  获取CPU占用率统计总时间。
*
* @par 描述:
* 用户通过该接口获取CPU占用率统计总时间。
* @attention 无。
*
* @param  pLastTotalTime [OUT] 倒数第二次锁定时的总时间(单位:us)。
* @param  pCurTotalTime [OUT] 倒数第一次锁定时的总时间(单位:us)。
*
* @retval #HI_ERR_SUCCESS 成功。
* @retval 其他值 失败，详见hi_errno.h。
*
* @par Dependency:
* @li hi_mdm_cpu_stat.h: 该接口声明所在的头文件。
* @see  HI_MDM_CPU_PercentStart | HI_MDM_CPU_PercentStop | HI_MDM_CPU_PercentLock  | HI_MDM_CPU_PercentGetTimeByTaskIdx | HI_MDM_CPU_PercentGetTaskCount
* @since HiMDL_V200R001C00
*/
HI_EXTERN HI_U32 HI_MDM_CPU_PercentGetTotalTime(HI_U64 *pLastTotalTime,HI_U64 *pCurTotalTime);



/**
* @ingroup  hct_cpup_track
* @brief  根据任务统计下标(中断号)获取一条统计信息。
*
* @par 描述:
* 用户通过该接口获取一个任务(中断)的CPU使用情况。
* @attention 无。
*
* @param  ulTaskIdx [IN] 任务下标或中断号。
* @param  bIsr [IN] HI_TRUE表示ulTaskIntId为中断号，HI_FALSE表示ulTaskIntId为任务ID。
* @param  pLastTimeCnt [OUT] 倒数第二次锁定时该任务(中断)占用的CPU时间(单位:us)。
* @param  pCurTimeCnt [OUT] 倒数第一次锁定时该任务(中断)占用的CPU时间(单位:us)。
*
* @retval #HI_ERR_SUCCESS 成功。
* @retval 其他值 失败，详见hi_errno.h。
*
* @par Dependency:
* @li hi_mdm_cpu_stat.h: 该接口声明所在的头文件。
* @see  HI_MDM_CPU_PercentStart | HI_MDM_CPU_PercentStop | HI_MDM_CPU_PercentLock | HI_MDM_CPU_PercentGetTotalTime | HI_MDM_CPU_PercentGetTaskCount
* @since HiMDL_V200R001C00
*/
HI_EXTERN HI_U32 HI_MDM_CPU_PercentGetTimeByTaskIdx(HI_U32 ulTaskIdx,HI_BOOL bIsr,HI_U64 *pLastTimeCnt,HI_U64 *pCurTimeCnt);

/**
* @ingroup  hct_cpup_track
* @brief  获取统计总任务个数。
*
* @par 描述:
* 通过该接口获取支持统计的任务个数，实际统计的任务个数小于等于总个数。
* @attention 无。
*
* @retval #HI_U32类型数值 任务总个数。
* @retval #HI_CPU_STAT_INVALID_COUNT 获取失败。
*
* @par Dependency:
* @li hi_mdm_cpu_stat.h: 该接口声明所在的头文件。
* @see  HI_MDM_CPU_PercentStart | HI_MDM_CPU_PercentStop | HI_MDM_CPU_PercentLock | HI_MDM_CPU_PercentGetTotalTime | HI_MDM_CPU_PercentGetTimeByTaskIdx 
* @since HiMDL_V200R001C00
*/
HI_EXTERN HI_U32 HI_MDM_CPU_PercentGetTaskCount(HI_VOID);

#endif
