
#ifndef __HI_MDM_NV_INTERNAL_H__
#define __HI_MDM_NV_INTERNAL_H__
#include <hi_types.h>
#include <hi_mdm_types.h>
#include <hi_mdm_nv.h>

/**
* @ingroup  hct_nv
* @brief 写NV文件到NV文件区
*
* @par 描述:
* 写NV文件到NV文件区
* @attention 
* @li pucData对应内存大小需要为PRODUCT_CFG_NV_FILE_SIZE_MAX。
*
* @param  pucData [IN] NV文件指针。
*
* @retval #HI_ERR_SUCCESS 成功。
* @retval 其他值 失败，详见hi_errno.h。
*
* @par Dependency:
* @li hi_mdm_nv_internal.h: 该接口声明所在的头文件。
* @see 无。
* @since HiMDL_V200R001C00
*/
HI_U32 HI_MDM_NV_WriteNvFile(HI_U8 *pucData);

#endif

