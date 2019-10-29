/**@defgroup hct_timecnt  timecnt
 * @ingroup drivers
 */

#ifndef __HI_MDM_TIMECNT_H__
#define __HI_MDM_TIMECNT_H__

/**
* @ingroup  hct_timecnt
* @brief  ��ȡϵͳ����ʱ��(��λ: s��ms��us)��
*
* @par ����: ��ȡϵͳ����ʱ��(��λ: s��ms��us)��
* @attention �ޡ�
* @param  pUs [OUT] ���� #HI_U32 * ��λ:us��
* @param  pMs [OUT] ���� #HI_U32 * ��λ:ms��
* @param  pSec [OUT] ���� #HI_U32 * ��λ:s��
* @retval #0      success.
* @retval #��0     failed.���hi_errno.h
* @par Dependency:
* <ul><li>hi_mdm_timecnt.h: �ļ���������time_cnt��ؽӿ�.</li></ul>
* @see  �ޡ�
* @since HiMDL_V200R001C00
*/
#define HI_MDM_TIMECNT_GetTime(pUs, pMs, pSec) HI_TIME_CNT_GetTime(pUs, pMs, pSec)

/**
* @ingroup  hct_timecnt
* @brief   ��ȡϵͳ����ʱ��(��λ: s)��
*
* @par ����:  ��ȡϵͳ����ʱ��(��λ: s)��
* @attention �ޡ�
* @param  pSec [OUT] ���� #HI_U32 *  ��λ:s��
* @retval #0      success.
* @retval #��0     failed.���hi_errno.h
* @par Dependency:
* <ul><li>hi_mdm_timecnt.h: �ļ���������time_cnt��ؽӿ�.</li></ul>
* @see  �ޡ�
* @since HiMDL_V200R001C00
*/
#define HI_MDM_TIMECNT_GetSec(pSec) HI_TIME_CNT_GetSec(pSec)

/**
* @ingroup  hct_timecnt
* @brief  ��ȡϵͳ����ʱ��(��λ: ms)��
*
* @par ����:    ��ȡϵͳ����ʱ��(��λ: ms)��
* @attention �ޡ�
* @param  pMs [OUT] ���� #HI_U32 * ��λ:ms��
* @retval #0      success.
* @retval #��0     failed.���hi_errno.h
* @par Dependency:
* <ul><li>hi_mdm_timecnt.h: �ļ���������time_cnt��ؽӿ�.</li></ul>
* @see  �ޡ�
* @since HiMDL_V200R001C00
*/
#define HI_MDM_TIMECNT_GetMs(pMs) HI_TIME_CNT_GetMs(pMs)

/**
* @ingroup  hct_timecnt
* @brief     ��ȡϵͳ����ʱ��(��λ: us)��
*
* @par ����:   ��ȡϵͳ����ʱ��(��λ: us)��
* @attention �ޡ�
* @param  pUs [OUT] ���� #HI_U32 * ��λ:us��
* @retval #0      success.
* @retval #��0     failed.���hi_errno.h
* @par Dependency:
* <ul><li>hi_mdm_timecnt.h: �ļ���������time_cnt��ؽӿ�.</li></ul>
* @see  �ޡ�
* @since HiMDL_V200R001C00
*/
#define HI_MDM_TIMECNT_GetUs(pUs) HI_TIME_CNT_GetUs(pUs)
#endif
