/**@defgroup hct_rst_info ��λ��Ϣ
 * @ingroup sal
 */

#ifndef __HI_MDM_RST_TIMES_H__
#define __HI_MDM_RST_TIMES_H__
#include <hi_types.h>
#include <hi_mdm_types.h>

/**
* @ingroup  hct_rst_info
* @brief ��ȡ��λ����,����ʱ��0��
*
* @par ����:
* ��ȡ����֮������Ϊֹ�ĸ�λ�������ô���ΪHI_U32���ͣ�һֱ�ۼӣ�����ͨ������HI_MDM_ClrRelativeRstTimes�ӿ���0��ֱ�����ֵ0xFFFFFFFF���Զ���0�������������0��
* @attention
* @li �������ӿڣ�֧�����жϡ����жϡ������������ĵ��á�
* @li OS(��BOOT)����30���ڸ�λ���´�����ʱ���ø�λ�����������ӡ�
*
* @retval #HI_U32 ��λ������
*
* @par Dependency:
* @li hi_mdm_rst_times.h: �ýӿ��������ڵ�ͷ�ļ���
* @since HiMDL_V200R001C00
*/
HI_U32 HI_MDM_GetRstTimes(HI_VOID);

/**
* @ingroup  hct_rst_info
* @brief ��ȡ��λ����,����ʱ��0,����HI_MDM_ClrRelativeRstTimes�ӿ�ʱ��0��
*
* @par ����:
* ��ȡ��λ����������HI_MDM_ClrRelativeRstTimes�ӿ�ʱ��0������ʱ��0��
* @attention
* @li �������ӿڣ�֧�����жϡ����жϡ������������ĵ��á�
* @li OS(��BOOT)����30���ڸ�λ���´�����ʱ���ø�λ�����������ӡ�
*
* @retval #HI_U32 ��λ������
*
* @par Dependency:
* @li hi_mdm_rst_times.h: �ýӿ��������ڵ�ͷ�ļ���
* @since HiMDL_V200R001C00
*/
HI_U32 HI_MDM_GetRelativeRstTimes(HI_VOID);

/**
* @ingroup  hct_rst_info
* @brief ��ո�λ���������øýӿں�ͨ��HI_MDM_GetRelativeRstTimes�ӿڻ�ȡ�ĸ�λ������0��
*
* @par ����:
* ��ո�λ���������øýӿں�ͨ��HI_MDM_GetRelativeRstTimes�ӿڻ�ȡ�ĸ�λ������0������HI_MDM_GetRstTimes�ӿڻ�ȡ�ĸ�λ����������0��
* @attention
* @li �����ӿڣ���֧�����жϡ����жϡ������������ĵ��á�
*
* @retval #HI_U32 ��λ������
*
* @par Dependency:
* @li hi_mdm_rst_times.h: �ýӿ��������ڵ�ͷ�ļ���
* @since HiMDL_V200R001C00
*/
HI_U32 HI_MDM_ClrRelativeRstTimes(HI_VOID);

/**
* @ingroup  hct_rst_info
* @brief ʹ��(ȥʹ��)��¼��λ�������ܡ�
*
* @par ����:
* ʹ��(ȥʹ��)��¼��λ�������ܡ�ʹ�ܸ�λ������¼����ʱ��ÿ������ʱ���¼NV,����������λ��������λ����������Ҫִ��дFlash������ͨ����ϵͳ����Ƶ�����µ磬���ܽ����ϵ�дNV����flash�������ٺľ��ĳ���ʱ����bEnable����ΪHI_FALSE��ͬʱҪ�����Ƿ��ҵ�������Ӱ�졣
*
* @attention
* @li �ýӿ���Ҫ��APP_InitCfgEarly�����е��á�
* @li ȥʹ�ܺ�HI_MDM_GetRstTimes��HI_MDM_GetRelativeRstTimes�ӿڷ������ֵ��
* @li �رոù���ʱ��Ҫ���ǶԵ��øýӿ�ҵ���Ӱ�졣
* @li �ù���Ĭ��ʹ�ܡ�
*
* @param  bEnable [IN] HI_TRUE ʹ�ܡ� HI_FALSE �رա�
*
* @par Dependency:
* @li hi_mdm_rst_times.h: �ýӿ��������ڵ�ͷ�ļ���
* @since HiMDL_V200R001C00
*/
HI_VOID HI_MDM_EnableRstTimeSave(HI_BOOL bEnable);

/**
* @ingroup  hct_rst_info
* @brief �ж��Ƿ�ΪӲ���硣
*
* @par ����:
* �ж�����ԭ���Ƿ�ΪӲ���絼�¡�
*
* @param  �ޡ�
*
* @retval #HI_BOOL HI_TRUE��ʾӲ���磬HI_FALSE��ʾ��Ӳ���硣
*
* @par Dependency:
* @li hi_mdm_rst_times.h: �ýӿ��������ڵ�ͷ�ļ���
* @since HiMDL_V200R001C00
*/
HI_BOOL HI_MDM_IsPowerCut(HI_VOID);


#endif

