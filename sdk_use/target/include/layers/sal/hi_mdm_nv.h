/**@defgroup hct_nv NV
 * @ingroup sal
 */

#ifndef __HI_MDM_NV_H__
#define __HI_MDM_NV_H__

/**
* @ingroup  hct_nv
* @brief 设置NV值。
*
* @par 描述:
* 设置NV值。
*@attention 仅支持任务中调用，不支持中断中调用。
*
* @param  usId [IN] NV项ID。
* @param  pData [IN] NV项数据。
* @param  usDataLen [IN] NV项长度。

* @retval #HI_ERR_SUCCESS 成功。
* @retval 其他值 失败，详见hi_errno.h。
*
* @par Dependency:
* @li hi_mdm_nv.h: 该接口声明所在的头文件。
* @see HI_MDM_NV_Read
* @since HiMDL_V200R001C00
*/
HI_EAPI HI_U32 HI_MDM_NV_Write(HI_U16 usId, HI_CONST HI_PVOID pData, HI_U16 usDataLen);

/**
* @ingroup  hct_nv
* @brief 读取NV值。
*
* @par 描述:
* 读取NV值。
* @attention 仅能操作非工厂区NV；仅支持任务中调用，不支持中断中调用。
*
* @param  usId [IN] NV项ID。
* @param  pItemData [OUT] NV项数据。
* @param  usItemDataLen [IN] NV项长度。

* @retval #HI_ERR_SUCCESS 成功。
* @retval 其他值 失败，详见hi_errno.h。
*
* @par Dependency:
* @li hi_mdm_nv.h: 该接口声明所在的头文件。
* @see HI_MDM_NV_Write
* @since HiMDL_V200R001C00
*/
HI_EAPI HI_U32 HI_MDM_NV_Read(HI_U16 usId, HI_OUT HI_PVOID pItemData, HI_U16 usItemDataLen);

/**
* @ingroup  hct_nv
* @brief NV项变更通知。
*
* @par 描述:
* NV项变更通知。
* @attention 
* @li 在SAL初始化结束后才可以调用该接口。
* @li 总共支持注册20个表(含SDK内部)。
*
* @param  usMinId [IN] NV项ID最小值。
* @param  usMaxId [IN] NV项ID最大值。
* @param  pfnChanged [in] NV项改变的处理函数，即NV项变更后 NV模块会自动调用该注册的接口。
*
* @retval #HI_ERR_SUCCESS 成功。
* @retval 其他值 失败，详见hi_errno.h。
*
* @par Dependency:
* @li hi_mdm_nv.h: 该接口声明所在的头文件。
* @see 无。
* @since HiMDL_V200R001C00
*/
HI_EAPI HI_U32 HI_MDM_NV_RegisterChangedNoitfyProc(HI_U16 usMinId, HI_U16 usMaxId, HI_NVM_CHANGED_NOTIFY_CALLBACK_F pfnChanged);

/**
* @ingroup  hct_nv
* @brief 刷新工厂区NV到NV文件。
*
* @par 描述:
* 刷新工厂区NV到NV文件。如果某种异常导致2个NV索引均错误，通过NV文件生成NV索引时，通过该接口刷新到NV文件的工厂区NV不会丢失，否则NV会丢失。
* @attention 
* @li 该接口建议只是在工装流程调用，因为调用该接口会增加NV文件区擦写次数，减少NV文件区寿命。
* @li 该接口建议只是在工装流程调用，因为调用该接口刷新(擦写)NV文件时异常掉电，反而会造成NV文件丢失(错误)。
* @li 如果调用该接口时异常掉电，一定要再次调用该接口。再次调用该接口检测到NV文件错误时会重新生成NV文件。
*
* @retval #HI_ERR_SUCCESS 成功。
* @retval 其他值 失败，详见hi_errno.h。
*
* @par Dependency:
* @li hi_mdm_nv.h: 该接口声明所在的头文件。
* @see 无。
* @since HiMDL_V200R001C00
*/
HI_EAPI HI_U32 HI_MDM_NV_RefreshDataToNvFile(HI_VOID);

#endif
