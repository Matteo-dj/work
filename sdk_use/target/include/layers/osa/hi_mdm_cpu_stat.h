/**@defgroup hct_cpup_track  CPU
 * @ingroup osa
 */

#ifndef __HI_MDM_CPU_STAT_H__
#define __HI_MDM_CPU_STAT_H__
#include <hi_types.h>
#include <hi_mdm_types.h>

/**
 * @ingroup hct_cpup_track
 * task�����ķǷ�ֵ
 */
#define HI_CPU_STAT_INVALID_COUNT 0xFFFFFFFF

/**
 * @ingroup hct_cpup_track
 * �������±�id ת����task id
 */
#define HI_CPU_STAT_ARRY_ID_TO_TASK_ID(x) (x)

/**
 * @ingroup hct_cpup_track
 * ��task id ת���������±� id
 */
#define HI_CPU_STAT_TASK_ID_TO_ARRAY_ID(x) (x)

/**
 * @ingroup hct_cpup_track
 * һ��������Ϣ,ȡֵ�ο� #HI_TRACE_TYPE
 */
typedef struct
{
    HI_U32 ulTraceType; /**< ������Ϣ���ͣ�����#HI_TRACE_TYPE*/
    HI_U32 ulData0;     /**< �������0    */
    HI_U32 ulData1;     /**< �������1    */
    HI_U32 ulEntry;     /**< ָ������ߣ�һ��Ϊ�ص�������LR*/
    HI_U32 ulCurTime;   /**< ����ʱ��(��λ:us)*/
}HI_CPU_STAT_TRACK_ITEM_S;

/**
 * @ingroup hct_cpup_track
 * ������Ϣ��Ҫ
 */
typedef struct
{
    HI_U32 ulLockIdx;           /**< ��ǰ��Ϣ�������¼��������Ϣ���±� */
    HI_U32 ulMaxRecordCount;    /**< �ɼ�¼��Ϣ���� */
}HI_CPU_STAT_TRACK_INFO_S;

/**
 * @ingroup hct_cpup_track
 * ����ͳ������
 */
typedef enum
{
    HI_TRACE_ISR_FLAG   =   0x10,/**< ʹ���жϵ���ͳ�� */
    HI_TRACE_IRQ_FLAG   =   0x20,/**< ʹ�ܿ����ж�ͳ�� */
    HI_TRACE_TASK_FLAG  =   0x40,/**< ʹ���������ͳ�� */
    HI_TRACE_WORKQ_FLAG =   0x80,/**< ʹ���жϺ�벿����ͳ�� */
    HI_TRACE_SWTMR_FLAG =   0x100,/**< ʹ�ܶ�ʱ������ͳ�� */
    HI_TRACE_MUX_FLAG   =   0x200,/**< ʹ�ܻ���������ͳ�� */
    HI_TRACE_SEM_FLAG   =   0x400,/**< ʹ���ź�������ͳ�� */
    HI_TRACE_QUE_FLAG   =   0x800,/**< ʹ����Ϣ����ͳ�� */
    HI_TRACE_EVENT_FLAG =   0x1000/**< ʹ���¼�����ͳ�� */
}HI_TRACE_MASK;//LOS_TRACE_MASK

/**
 * @ingroup hct_cpup_track
 * ������ȡֵ��������
 */
typedef enum
{
    HI_TRAP_INT        = HI_TRACE_ISR_FLAG,/**< �����ж�,ulEntry=NA,ulData0=�жϺ�,ulData1=NA */

    HI_ENABLE_IRQ      = HI_TRACE_IRQ_FLAG|0,/**< ���ж�,ulEntry=LR,ulData0=NA,ulData1=NA */
    HI_DISABLE_IRQ     = HI_TRACE_IRQ_FLAG|1,/**< ���ж�,ulEntry=LR,ulData0=NA,ulData1=NA */
   
    HI_CREATE_TSK      = HI_TRACE_TASK_FLAG|0,/**< ��������,ulEntry=NA,ulData0=����ID,ulData1=NA */
    HI_SCHEDULE_TSK    = HI_TRACE_TASK_FLAG|1,/**< �������,ulEntry=NA,ulData0=����ID,ulData1=NA */
    HI_DELETE_TSK      = HI_TRACE_TASK_FLAG|2,/**< ɾ������,ulEntry=NA,ulData0=����ID,ulData1=NA */
    
    HI_ENQUEUE_WORKQ   = HI_TRACE_WORKQ_FLAG|0,/**< ����жϺ���,ulEntry=����ִ�к���,ulData0=�������ȼ�,ulData1=NA */
    HI_SCHEDULE_WORKQ  = HI_TRACE_WORKQ_FLAG|1,/**< �жϺ���ִ��,ulEntry=����ִ�к���,ulData0=�������ȼ�,ulData1=NA */
    HI_DEQUEUE_WORKQ   = HI_TRACE_WORKQ_FLAG|2,/**< ��֧��,ulEntry=NA,ulData0=NA,ulData1=NA   */     

    HI_CREATE_SWTMR    = HI_TRACE_SWTMR_FLAG|0,/**< ������ʱ��,ulEntry=��ʱ���ص�����,ulData0=��ʱ���ṹָ��(�����ע),ulData1=��ʱ����ʱʱ��(tick) */
    HI_START_SWTMR     = HI_TRACE_SWTMR_FLAG|1,/**< ������ʱ��(�����ע),ulEntry=��ʱ���ص�����,ulData0=��ʱ���ṹָ��(�����ע),ulData1=��ʱ����ʱʱ��(tick) */
    HI_MODIFY_SWTMR    = HI_TRACE_SWTMR_FLAG|2,/**< ������ʱ��,ulEntry=��ʱ���ص�����,ulData0=��ʱ���ṹָ��(�����ע),ulData1=��ʱ����ʱʱ��(tick) */
    HI_SCHDING_SWTMR   = HI_TRACE_SWTMR_FLAG|3,/**< ��ʱ���ص�����ִ��ǰ,ulEntry=��ʱ���ص�����,ulData0=��ʱ���ṹָ��(�����ע),ulData1=αID(�����ע) */
    HI_SCHDED_SWTMR    = HI_TRACE_SWTMR_FLAG|4,/**< ��ʱ���ص�����ִ�к�,ulEntry=��ʱ���ص�����,ulData0=��ʱ���ṹָ��(�����ע),ulData1=αID(�����ע) */
    HI_STOP_SWTMR      = HI_TRACE_SWTMR_FLAG|5,/**< ֹͣ��ʱ��,ulEntry=��ʱ���ص�����,ulData0=��ʱ���ṹָ��(�����ע),��ǰһ����ʱ���Ĳ�ֵ(�����ע) */
    HI_DEL_SWTMR       = HI_TRACE_SWTMR_FLAG|6,/**< ɾ����ʱ��,ulEntry=��ʱ���ص�����,ulData0=��ʱ���ṹָ��(�����ע),��ǰһ����ʱ���Ĳ�ֵ(�����ע) */
    HI_DROP_SWTMR      = HI_TRACE_SWTMR_FLAG|7,/**< ��֧��,ulEntry=NA,ulData0=NA,ulData1=NA */

    HI_MUX_CREATE      = HI_TRACE_MUX_FLAG|0,/**< ��֧��,ulEntry=NA,ulData0=NA,ulData1=NA */
    HI_MUX_PEND        = HI_TRACE_MUX_FLAG|1,/**< ��֧��,ulEntry=NA,ulData0=NA,ulData1=NA */
    HI_MUX_POST        = HI_TRACE_MUX_FLAG|2,/**< ��֧��,ulEntry=NA,ulData0=NA,ulData1=NA */
    HI_MUX_DEL         = HI_TRACE_MUX_FLAG|3,/**< ��֧��,ulEntry=NA,ulData0=NA,ulData1=NA */

    HI_SEM_CREATE      = HI_TRACE_SEM_FLAG|0,/**< ��֧��,ulEntry=NA,ulData0=NA,ulData1=NA */
    HI_SEM_PEND        = HI_TRACE_SEM_FLAG|1,/**< ��֧��,ulEntry=NA,ulData0=NA,ulData1=NA */
    HI_SEM_POST        = HI_TRACE_SEM_FLAG|2,/**< ��֧��,ulEntry=NA,ulData0=NA,ulData1=NA */
    HI_SEM_DEL         = HI_TRACE_SEM_FLAG|3,/**< ��֧��,ulEntry=NA,ulData0=NA,ulData1=NA */
    
    HI_QUE_CREATE      = HI_TRACE_QUE_FLAG|0,/**< ��֧��,ulEntry=NA,ulData0=NA,ulData1=NA */
    HI_QUE_WRITE       = HI_TRACE_QUE_FLAG|1,/**< ��֧��,ulEntry=NA,ulData0=NA,ulData1=NA */
    HI_QUE_READ        = HI_TRACE_QUE_FLAG|2,/**< ��֧��,ulEntry=NA,ulData0=NA,ulData1=NA */
    HI_QUE_DEL         = HI_TRACE_QUE_FLAG|3,/**< ��֧��,ulEntry=NA,ulData0=NA,ulData1=NA */

    HI_EVENT_CREATE    = HI_TRACE_EVENT_FLAG|0,/**< ��֧��,ulEntry=NA,ulData0=NA,ulData1=NA */
    HI_EVENT_WRITE     = HI_TRACE_EVENT_FLAG|1,/**< ��֧��,ulEntry=NA,ulData0=NA,ulData1=NA */
    HI_EVENT_READ      = HI_TRACE_EVENT_FLAG|2,/**< ��֧��,ulEntry=NA,ulData0=NA,ulData1=NA */
    HI_EVENT_CLEAR     = HI_TRACE_EVENT_FLAG|3,/**< ��֧��,ulEntry=NA,ulData0=NA,ulData1=NA */
    HI_EVENT_DEL       = HI_TRACE_EVENT_FLAG|4,/**< ��֧��,ulEntry=NA,ulData0=NA,ulData1=NA */
}HI_TRACE_TYPE;//LOS_TRACE_TYPE


/**
* @ingroup  hct_cpup_track
* @brief  ��������ͳ�ơ�
*
* @par ����:
* �û�ͨ���ýӿ���������ͳ�ơ�
* @attention �ޡ�
*
* @param  usMaxRecordCount [IN] �����Լ�¼�Ĺ켣������
*
* @retval #HI_ERR_SUCCESS �ɹ���
* @retval ����ֵ ʧ�ܣ����hi_errno.h��
*
* @par Dependency:
* @li hi_mdm_cpu_stat.h: �ýӿ��������ڵ�ͷ�ļ���
* @see  HI_MDM_CPU_TrackSetType | HI_MDM_CPU_TrackGetType | HI_MDM_CPU_TrackLock | HI_MDM_CPU_TrackGetInfo | HI_MDM_CPU_TrackGetLockInfoByIdx | HI_MDM_CPU_TrackStop
* @since HiMDL_V200R001C00
*/
HI_EXTERN HI_U32 HI_MDM_CPU_TrackStart(HI_U16 usMaxRecordCount);

/**
* @ingroup  hct_cpup_track
* @brief  ���õ���ͳ�����͡�
*
* @par ����:
* �û�ͨ���ýӿ�����ϣ��ͳ�Ƶĵ������͡�
* @attention �ޡ�
*
* @param  trace_type [IN] Ҫͳ�Ƶ����ͣ�ȡֵ�ο� #HI_TRACE_MASK��
*
* @retval #HI_ERR_SUCCESS �ɹ���
* @retval ����ֵ ʧ�ܣ����hi_errno.h��

* @par Dependency:
* @li hi_mdm_cpu_stat.h: �ýӿ��������ڵ�ͷ�ļ���
* @see  HI_MDM_CPU_TrackStart | HI_MDM_CPU_TrackGetType | HI_MDM_CPU_TrackLock | HI_MDM_CPU_TrackGetInfo | HI_MDM_CPU_TrackGetLockInfoByIdx | HI_MDM_CPU_TrackStop
* @since HiMDL_V200R001C00
*/
HI_EXTERN HI_VOID HI_MDM_CPU_TrackSetType(HI_U32 trace_type);

/**
* @ingroup  hct_cpup_track
* @brief  ��ȡ����ͳ�����͡�
*
* @par ����:
* �û�ͨ���ýӿڿ��Ի�ȡ��ǰ���õ�ͳ�����͡�
* @attention �ޡ�
*
* @retval #HI_U32������ֵ ��ǰ��ͳ�����ͣ�ȡֵ�ο� #HI_TRACE_MASK��
*
* @par Dependency:
* @li hi_mdm_cpu_stat.h: �ýӿ��������ڵ�ͷ�ļ���
* @see  HI_MDM_CPU_TrackStart | HI_MDM_CPU_TrackSetType | HI_MDM_CPU_TrackLock | HI_MDM_CPU_TrackGetInfo | HI_MDM_CPU_TrackGetLockInfoByIdx | HI_MDM_CPU_TrackStop
* @since HiMDL_V200R001C00
*/
HI_EXTERN HI_U32 HI_MDM_CPU_TrackGetType(HI_VOID);

/**
* @ingroup  hct_cpup_track
* @brief  ֹͣ����ͳ�ơ�
*
* @par ����:
* �û�ͨ�����øýӿڣ�ֹͣ����ͳ�ơ�
* @attention �ޡ�
*
* @retval #HI_ERR_SUCCESS �ɹ���
* @retval ����ֵ ʧ�ܣ����hi_errno.h��
*
* @par Dependency:
* @li hi_mdm_cpu_stat.h: �ýӿ��������ڵ�ͷ�ļ���
* @see  HI_MDM_CPU_TrackStart | HI_MDM_CPU_TrackSetType | HI_MDM_CPU_TrackGetType | HI_MDM_CPU_TrackLock | HI_MDM_CPU_TrackGetInfo | HI_MDM_CPU_TrackGetLockInfoByIdx
* @since HiMDL_V200R001C00
*/
HI_EXTERN HI_U32 HI_MDM_CPU_TrackStop(HI_VOID);

/**
* @ingroup  hct_cpup_track
* @brief  ��������ͳ����Ϣ��
*
* @par ����:
* �û���ȡͳ����Ϣ֮ǰ��Ҫ���øýӿڣ�
*����ʱ�̵�ͳ����Ϣ������һ�鱸���ڴ��У������ڴ��е���Ϣ�������ŵ��ȶ��仯��
* @attention �ޡ�
*
* @retval #HI_ERR_SUCCESS �ɹ���
* @retval ����ֵ ʧ�ܣ����hi_errno.h��
*
* @par Dependency:
* @li hi_mdm_cpu_stat.h: �ýӿ��������ڵ�ͷ�ļ���
* @see  HI_MDM_CPU_TrackStart | HI_MDM_CPU_TrackSetType|HI_MDM_CPU_TrackGetType|HI_MDM_CPU_TrackGetInfo|HI_MDM_CPU_TrackGetLockInfoByIdx|HI_MDM_CPU_TrackStop
* @since HiMDL_V200R001C00
*/
HI_EXTERN HI_U32 HI_MDM_CPU_TrackLock(HI_VOID);

/**
* @ingroup  hct_cpup_track
* @brief  ��ȡ���ȸ�Ҫ��Ϣ��
*
* @par ����:
* �û�ͨ�����øýӿڻ�ȡ��Ҫ������Ϣ��
* @attention �ޡ�
*
* @param  pInfo [OUT] ������Ϣ��Ҫ ��
*
* @retval #HI_ERR_SUCCESS �ɹ���
* @retval ����ֵ ʧ�ܣ����hi_errno.h��
*
* @par Dependency:
* @li hi_mdm_cpu_stat.h: �ýӿ��������ڵ�ͷ�ļ���
* @see  HI_MDM_CPU_TrackStart | HI_MDM_CPU_TrackSetType | HI_MDM_CPU_TrackGetType | HI_MDM_CPU_TrackLock | HI_MDM_CPU_TrackGetLockInfoByIdx | HI_MDM_CPU_TrackStop
* @since HiMDL_V200R001C00
*/
HI_EXTERN HI_U32 HI_MDM_CPU_TrackGetInfo(HI_CPU_STAT_TRACK_INFO_S *pInfo);

/**
* @ingroup  hct_cpup_track
* @brief  ��ȡһ��������Ϣ��
*
* @par ����:
* �û�ͨ�����øýӿڻ�ȡһ��������Ϣ��
* @attention �ޡ�
*
* @param  ulIdx [IN] ������Ϣ�±�š�
* @param  pItemInfo [OUT] ������Ϣ���ݡ�
*
* @retval #HI_ERR_SUCCESS �ɹ���
* @retval ����ֵ ʧ�ܣ����hi_errno.h��
*
* @par Dependency:
* @li hi_mdm_cpu_stat.h: �ýӿ��������ڵ�ͷ�ļ���
* @see  HI_MDM_CPU_TrackStart | HI_MDM_CPU_TrackSetType | HI_MDM_CPU_TrackGetType | HI_MDM_CPU_TrackLock | HI_MDM_CPU_TrackGetInfo | HI_MDM_CPU_TrackStop
* @since HiMDL_V200R001C00
*/
HI_EXTERN HI_U32 HI_MDM_CPU_TrackGetLockInfoByIdx(HI_U32 ulIdx,HI_CPU_STAT_TRACK_ITEM_S *pItemInfo);

/**
* @ingroup  hct_cpup_track
* @brief  ����CPUռ����ͳ�ơ�
*
* @par ����:
* �û�ͨ�����øýӿ�����CPUռ����ͳ�ơ�
* @attention �ޡ�
*
* @param �ޡ�
*
* @retval #HI_ERR_SUCCESS �ɹ���
* @retval ����ֵ ʧ�ܣ����hi_errno.h��
*
* @par Dependency:
* @li hi_mdm_cpu_stat.h: �ýӿ��������ڵ�ͷ�ļ���
* @see   HI_MDM_CPU_PercentStop | HI_MDM_CPU_PercentLock | HI_MDM_CPU_PercentGetTotalTime  | HI_MDM_CPU_PercentGetTimeByTaskIdx | HI_MDM_CPU_PercentGetTaskCount
* @since HiMDL_V200R001C00
*/
HI_EXTERN HI_U32 HI_MDM_CPU_PercentStart(HI_VOID);

/**
* @ingroup  hct_cpup_track
* @brief  ֹͣCPUռ����ͳ�ơ�
*
* @par ����:
* �û�ͨ�����øýӿ�ֹͣCPUռ����ͳ�ơ�
* @attention �ޡ�
*
* @retval #HI_ERR_SUCCESS �ɹ���
* @retval ����ֵ ʧ�ܣ����hi_errno.h��
*
* @par Dependency:
* @li hi_mdm_cpu_stat.h: �ýӿ��������ڵ�ͷ�ļ���
* @see  HI_MDM_CPU_PercentStart | HI_MDM_CPU_PercentLock | HI_MDM_CPU_PercentGetTotalTime | HI_MDM_CPU_PercentGetTimeByTaskIdx | HI_MDM_CPU_PercentGetTaskCount
* @since HiMDL_V200R001C00
*/
HI_EXTERN HI_U32 HI_MDM_CPU_PercentStop(HI_VOID);

/**
* @ingroup  hct_cpup_track
* @brief  ����CPUռ����ͳ����Ϣ��
*
* @par ����:
* �û���ȡͳ����Ϣ֮ǰ��Ҫ���øýӿڣ�
*����ʱ�̵�ͳ����Ϣ������һ�鱸���ڴ��У������ڴ��е���Ϣ�������ŵ��ȶ��仯��
* @attention �ޡ�
*
* @retval #HI_ERR_SUCCESS �ɹ���
* @retval ����ֵ ʧ�ܣ����hi_errno.h��
*
* @par Dependency:
* @li hi_mdm_cpu_stat.h: �ýӿ��������ڵ�ͷ�ļ���
* @see  HI_MDM_CPU_PercentStart | HI_MDM_CPU_PercentStop | HI_MDM_CPU_PercentGetTotalTime | HI_MDM_CPU_PercentGetTimeByTaskIdx | HI_MDM_CPU_PercentGetTaskCount
* @since HiMDL_V200R001C00
*/
HI_EXTERN HI_U32 HI_MDM_CPU_PercentLock(HI_VOID);

/**
* @ingroup  hct_cpup_track
* @brief  ��ȡCPUռ����ͳ����ʱ�䡣
*
* @par ����:
* �û�ͨ���ýӿڻ�ȡCPUռ����ͳ����ʱ�䡣
* @attention �ޡ�
*
* @param  pLastTotalTime [OUT] �����ڶ�������ʱ����ʱ��(��λ:us)��
* @param  pCurTotalTime [OUT] ������һ������ʱ����ʱ��(��λ:us)��
*
* @retval #HI_ERR_SUCCESS �ɹ���
* @retval ����ֵ ʧ�ܣ����hi_errno.h��
*
* @par Dependency:
* @li hi_mdm_cpu_stat.h: �ýӿ��������ڵ�ͷ�ļ���
* @see  HI_MDM_CPU_PercentStart | HI_MDM_CPU_PercentStop | HI_MDM_CPU_PercentLock  | HI_MDM_CPU_PercentGetTimeByTaskIdx | HI_MDM_CPU_PercentGetTaskCount
* @since HiMDL_V200R001C00
*/
HI_EXTERN HI_U32 HI_MDM_CPU_PercentGetTotalTime(HI_U64 *pLastTotalTime,HI_U64 *pCurTotalTime);



/**
* @ingroup  hct_cpup_track
* @brief  ��������ͳ���±�(�жϺ�)��ȡһ��ͳ����Ϣ��
*
* @par ����:
* �û�ͨ���ýӿڻ�ȡһ������(�ж�)��CPUʹ�������
* @attention �ޡ�
*
* @param  ulTaskIdx [IN] �����±���жϺš�
* @param  bIsr [IN] HI_TRUE��ʾulTaskIntIdΪ�жϺţ�HI_FALSE��ʾulTaskIntIdΪ����ID��
* @param  pLastTimeCnt [OUT] �����ڶ�������ʱ������(�ж�)ռ�õ�CPUʱ��(��λ:us)��
* @param  pCurTimeCnt [OUT] ������һ������ʱ������(�ж�)ռ�õ�CPUʱ��(��λ:us)��
*
* @retval #HI_ERR_SUCCESS �ɹ���
* @retval ����ֵ ʧ�ܣ����hi_errno.h��
*
* @par Dependency:
* @li hi_mdm_cpu_stat.h: �ýӿ��������ڵ�ͷ�ļ���
* @see  HI_MDM_CPU_PercentStart | HI_MDM_CPU_PercentStop | HI_MDM_CPU_PercentLock | HI_MDM_CPU_PercentGetTotalTime | HI_MDM_CPU_PercentGetTaskCount
* @since HiMDL_V200R001C00
*/
HI_EXTERN HI_U32 HI_MDM_CPU_PercentGetTimeByTaskIdx(HI_U32 ulTaskIdx,HI_BOOL bIsr,HI_U64 *pLastTimeCnt,HI_U64 *pCurTimeCnt);

/**
* @ingroup  hct_cpup_track
* @brief  ��ȡͳ�������������
*
* @par ����:
* ͨ���ýӿڻ�ȡ֧��ͳ�Ƶ����������ʵ��ͳ�Ƶ��������С�ڵ����ܸ�����
* @attention �ޡ�
*
* @retval #HI_U32������ֵ �����ܸ�����
* @retval #HI_CPU_STAT_INVALID_COUNT ��ȡʧ�ܡ�
*
* @par Dependency:
* @li hi_mdm_cpu_stat.h: �ýӿ��������ڵ�ͷ�ļ���
* @see  HI_MDM_CPU_PercentStart | HI_MDM_CPU_PercentStop | HI_MDM_CPU_PercentLock | HI_MDM_CPU_PercentGetTotalTime | HI_MDM_CPU_PercentGetTimeByTaskIdx 
* @since HiMDL_V200R001C00
*/
HI_EXTERN HI_U32 HI_MDM_CPU_PercentGetTaskCount(HI_VOID);

#endif
