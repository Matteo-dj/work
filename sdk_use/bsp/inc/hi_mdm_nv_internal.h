
#ifndef __HI_MDM_NV_INTERNAL_H__
#define __HI_MDM_NV_INTERNAL_H__
#include <hi_types.h>
#include <hi_mdm_types.h>
#include <hi_mdm_nv.h>

/**
* @ingroup  hct_nv
* @brief дNV�ļ���NV�ļ���
*
* @par ����:
* дNV�ļ���NV�ļ���
* @attention 
* @li pucData��Ӧ�ڴ��С��ҪΪPRODUCT_CFG_NV_FILE_SIZE_MAX��
*
* @param  pucData [IN] NV�ļ�ָ�롣
*
* @retval #HI_ERR_SUCCESS �ɹ���
* @retval ����ֵ ʧ�ܣ����hi_errno.h��
*
* @par Dependency:
* @li hi_mdm_nv_internal.h: �ýӿ��������ڵ�ͷ�ļ���
* @see �ޡ�
* @since HiMDL_V200R001C00
*/
HI_U32 HI_MDM_NV_WriteNvFile(HI_U8 *pucData);

#endif

