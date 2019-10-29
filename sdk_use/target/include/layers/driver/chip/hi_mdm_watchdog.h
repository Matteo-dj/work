/**@defgroup hct_watchdog ���Ź�
 * @ingroup drivers
 */

#ifndef __HI_MDM_WATCHDOG_H__
#define __HI_MDM_WATCHDOG_H__
#include <hi_types.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
* @ingroup  hct_watchdog
* @brief ʹ�ܿ��Ź���
*
* @par ����:
* ʹ�ܿ��Ź���
* @attention �ޡ�
*
* @param �ޡ�
*
* @retval �ޡ�
*
* @par Dependency:
* @li hi_mdm_watchdog.h: �ýӿ��������ڵ�ͷ�ļ���
* @see �ޡ�
* @since HiMDL_V200R001C00
*/
HI_EXTERN HI_VOID HI_MDM_WDG_Enable(HI_VOID);

/**
* @ingroup  hct_watchdog
* @brief ι����
*
* @par ����:
* ι��,���ÿ��Ź�����ʱ�䡣
* @attention �ޡ�
*
* @param �ޡ�
*
* @retval �ޡ�
*
* @par Dependency:
* @li hi_mdm_watchdog.h: �ýӿ��������ڵ�ͷ�ļ���
* @see �ޡ�
* @since HiMDL_V200R001C00
*/
HI_EXTERN HI_VOID HI_MDM_WDG_Feed(HI_VOID);

/**
* @ingroup  hct_watchdog
* @brief �رտ��Ź���
*
* @par ����:
* �ýӿ���Ҫ������²���:
* @li ��ֹWatchDogʱ��ʹ�ܿ���λ��
* @li ����WatchDog��λ���ܡ�
* @attention �ޡ�
*
* @param �ޡ�
*
* @retval �ޡ�
*
* @par Dependency:
* @li hi_mdm_watchdog.h: �ýӿ��������ڵ�ͷ�ļ���
* @see �ޡ�
* @since HiMDL_V200R001C00
*/
HI_EXTERN HI_VOID HI_MDM_WDG_Disable(HI_VOID);

#ifdef __cplusplus
}
#endif
#endif
