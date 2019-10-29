/**@defgroup hct_time ʱ��
 * @ingroup osa
 */
#ifndef __HI_MDM_TIME_H__
#define __HI_MDM_TIME_H__
#include <hi_types.h>
#include <hi_mdm_types.h>

typedef HI_U32 (*OS_UDELAY_FUNC)(HI_U32 ulUs);

/**
* @ingroup  hct_time
* @brief  ��ʱ��΢�뼶��
*
* @par ����:  
* ��ʱ������ռ��CPU��
* @attention 
* @li �ýӿ�֧������������������ʹ�ã�����Ӱ������������ȡ�
* @li �ýӿ�֧����LISR��������ʹ�ã�����Ӱ�������жϺ�������ȡ�
* @li �ýӿ�֧���ڹ��ж���������ʹ�ã�����Ӱ�������жϺ�������ȡ�
*
* @param ulus [IN] ��ʱʱ�䣬��λ:us��
*
* @retval #HI_ERR_SUCCESS ��ʱ�ɹ���
* @retval ����ֵ ʧ�ܣ����hi_errno.h��
*
* @par Dependency:
* @li hi_mdm_time.h: �ýӿ��������ڵ�ͷ�ļ���
* @see  �ޡ�
* @since HiMDL_V200R001C00
*/
HI_U32 HI_MDM_Udelay(HI_U32 ulus);

/**
* @ingroup  hct_time
* @brief  ��ȡϵͳtickֵ(32bit)��
*
* @par ����: 
* ��ȡϵͳtickֵ(32bit)��
* @attention 
* @li �ýӿ�֧������������������ʹ�á�
* @li �ýӿ�֧����LISR��������ʹ�á�
* @li �ýӿ�֧���ڹ��ж���������ʹ�á�
*
* @param �ޡ�
*
* @retval #HI_U32������ֵ ϵͳtickֵ��
*
* @par Dependency:
* @li hi_mdm_time.h: �ýӿ��������ڵ�ͷ�ļ���
* @see  �ޡ�
* @since HiMDL_V200R001C00
*/
HI_U32 HI_MDM_GetTick(HI_VOID);

/**
* @ingroup  hct_time
* @brief  ��ȡϵͳtickֵ(64bit)��
*
* @par ����: 
* ��ȡϵͳtickֵ(64bit)��
* @attention �ýӿڵ��ô��������ͷ�ļ�hi_mdm_time.h��������δ�����ӿڴ����ὫTickֵ����int���ͷ��أ������ضϴ���
* @li �ýӿ�֧������������������ʹ�á�
* @li �ýӿ�֧����LISR��������ʹ�á�
* @li �ýӿ�֧���ڹ��ж���������ʹ�á�
*
* @param �ޡ�
*
* @retval #HI_U64������ֵ ϵͳtickֵ��
*
* @par Dependency:
* @li hi_mdm_time.h: �ýӿ��������ڵ�ͷ�ļ���
* @see  �ޡ�
* @since HiMDL_V200R001C00
*/
HI_U64 HI_MDM_GetTick64(HI_VOID);

/**
* @ingroup  hct_time
* @brief  ��ȡϵͳʱ��(��λ:ms)��
*
* @par ����: 
* ��ȡϵͳʱ��(��λ:ms)��
* @attention 
* @li �ýӿ�֧������������������ʹ�á�
* @li �ýӿ�֧����LISR��������ʹ�á�
* @li �ýӿ�֧���ڹ��ж���������ʹ�á�
*
* @param �ޡ�
*
* @retval #HI_U32������ֵ ϵͳʱ�䡣
*
* @par Dependency:
* @li hi_mdm_time.h: �ýӿ��������ڵ�ͷ�ļ���
* @see  �ޡ�
* @since HiMDL_V200R001C00
*/
HI_U32 HI_MDM_GetMilliSeconds(HI_VOID);

/**
* @ingroup  hct_time
* @brief  ��ȡϵͳʱ��(��λ:��)��
*
* @par ����: 
* ��ȡϵͳʱ��(��λ:��)��
* @attention 
* @li �ýӿ�֧������������������ʹ�á�
* @li �ýӿ�֧����LISR��������ʹ�á�
* @li �ýӿ�֧���ڹ��ж���������ʹ�á�
*
* @param �ޡ�
*
* @retval #HI_U32������ֵ ϵͳʱ�䡣
* @retval #HI_ERR_FAILURE ��ȡʱ��ʧ�ܡ�
*
* @par Dependency:
* @li hi_mdm_time.h: �ýӿ��������ڵ�ͷ�ļ���
* @see  �ޡ�
* @since HiMDL_V200R001C00
*/
HI_U32 HI_MDM_GetSeconds(HI_VOID);

/**
* @ingroup  hct_time
* @brief  ��ȡϵͳʵʱʱ��(��λ:s)��
*
* @par ����: 
* ��ȡϵͳʵʱʱ��(��λ:s)��
* @attention 
* @li �ýӿ�֧������������������ʹ�á�
* @li �ýӿ�֧����LISR��������ʹ�á�
* @li �ýӿ�֧���ڹ��ж���������ʹ�á�
*
* @param �ޡ�
*
* @retval #HI_U32������ֵ ϵͳʵʱʱ�䡣
* @retval #HI_ERR_FAILURE ��ȡʱ��ʧ�ܡ�
*
* @par Dependency:
* @li hi_mdm_time.h: �ýӿ��������ڵ�ͷ�ļ���
* @see  �ޡ�
* @since HiMDL_V200R001C00
*/
HI_U32 HI_MDM_GetRealTime(HI_VOID);

/**
* @ingroup  hct_time
* @brief  ����ϵͳʵʱʱ�䡣
*
* @par ����: 
* ����ϵͳʵʱʱ�䡣
* @attention 
* @li �ýӿ�֧������������������ʹ�á�
* @li �ýӿ�֧����LISR��������ʹ�á�
* @li �ýӿ�֧���ڹ��ж���������ʹ�á�
*
* @param ulSec [IN] ��ϵͳʵʱʱ������Ϊ��ֵ��
*
* @retval #HI_ERR_SUCCESS ���óɹ���
* @retval #HI_ERR_FAILURE ����ʧ�ܡ�
*
* @par Dependency:
* @li hi_mdm_time.h: �ýӿ��������ڵ�ͷ�ļ���
* @see  �ޡ�
* @since HiMDL_V200R001C00
*/
HI_U32 HI_MDM_SetRealTime(HI_U32 ulSec);

/**
* @ingroup  hct_time
* @brief  ���õ�ǰǽ��ʱ�䡣
*
* @par ����: 
* ���õ�ǰǽ��ʱ�䣬ǽ��ʱ����RTCʱ�����һ�ֱ�ʾ��ʽ��
* @attention 
* @li �ýӿ�֧������������������ʹ�á�
* @li �ýӿ�֧����LISR��������ʹ�á�
* @li �ýӿ�֧���ڹ��ж���������ʹ�á�
*
* @param  calendarTime [IN] Ҫ���õ�ʱ�䣬��Ч��Χ 1970-01-01~2105-12-31��
*
* @retval #HI_ERR_SUCCESS  �ɹ���
* @retval ����ֵ ʧ�ܣ����hi_errno.h��
*
* @par Dependency:
* @li hi_mdm_time.h: �ýӿ��������ڵ�ͷ�ļ���
* @see  �ޡ�
* @since HiMDL_V200R001C00
*/
HI_U32 HI_MDM_SetCalendarTime(HI_IN HI_SYS_CALENDAR_TIME_S * calendarTime);

/**
* @ingroup  hct_time
* @brief   ��ȡ��ǰǽ��ʱ�䡣
*
* @par ����: 
* ��ȡ��ǰǽ��ʱ�䣬ǽ��ʱ����RTCʱ�����һ�ֱ�ʾ��ʽ��
* @attention 
* @li �ýӿ�֧������������������ʹ�á�
* @li �ýӿ�֧����LISR��������ʹ�á�
* @li �ýӿ�֧���ڹ��ж���������ʹ�á�
*
* @param  calendarTime [OUT] ��ǰǽ��ʱ�䣬δ��������´�1970-1-1 0:0:0��ʼ��ʱ��
*
* @retval #HI_ERR_SUCCESS �ɹ���
* @retval ����ֵ ʧ�ܣ����hi_errno.h��
*
* @par Dependency:
* @li hi_mdm_time.h: �ýӿ��������ڵ�ͷ�ļ���
* @see  �ޡ�
* @since HiMDL_V200R001C00
*/
HI_U32 HI_MDM_GetCalendarTime(HI_OUT HI_SYS_CALENDAR_TIME_S * calendarTime);

/**
* @ingroup  hct_time
* @brief   ����ֵת����ǽ��ʱ�䡣
*
* @par ����:  
* ����ֵת����ǽ��ʱ�䡣
* @attention 
* @li �ýӿ�֧������������������ʹ�á�
* @li �ýӿ�֧����LISR��������ʹ�á�
* @li �ýӿ�֧���ڹ��ж���������ʹ�á�
*
* @param  second [IN] ʱ��(��λ:s) ��
* @param  calendarTime [OUT] ǽ��ʱ�䡣
*
* @retval #HI_ERR_SUCCESS �ɹ���
* @retval ����ֵ ʧ�ܣ����hi_errno.h��
*
* @par Dependency:
* @li hi_mdm_time.h: �ýӿ��������ڵ�ͷ�ļ���
* @see  �ޡ�
* @since HiMDL_V200R001C00
*/
HI_U32 HI_MDM_MakeSecondToCalendar(HI_IN HI_U32 second,HI_OUT HI_SYS_CALENDAR_TIME_S * calendarTime);

/**
* @ingroup  hct_time
* @brief   ��ǽ��ʱ��ת�����롣
*
* @par ����:  
* ��ǽ��ʱ��ת�����롣
* @attention 
* @li �ýӿ�֧������������������ʹ�á�
* @li �ýӿ�֧����LISR��������ʹ�á�
* @li �ýӿ�֧���ڹ��ж���������ʹ�á�
*
* @param  calendarTime [IN] ǽ��ʱ�䣬��Ч��Χ 1970-01-01~2105-12-31��
* @param  second [OUT] RTCʱ��(��λ:s) ��
*
* @retval #HI_ERR_SUCCESS �ɹ���
* @retval ����ֵ ʧ�ܣ����hi_errno.h��
*
* @par Dependency:
* @li hi_mdm_time.h: �ýӿ��������ڵ�ͷ�ļ���
* @see  �ޡ�
* @since HiMDL_V200R001C00
*/
HI_U32 HI_MDM_MakeCalendarToSecond(HI_IN HI_SYS_CALENDAR_TIME_S * calendarTime,HI_OUT HI_U32 *second);

#endif
