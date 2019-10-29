
#ifndef __HI_MDM_FLASH__
#define __HI_MDM_FLASH__

HI_START_HEADER
#if !defined(PRODUCT_CFG_BUILD_HSO_CODE)

/**@defgroup hct_FLASH FLASH
 * @ingroup mdm
*/


/**
* @ingroup  hct_FLASH
* @brief  读出Flash数据到指定缓存区域。
*
* @par 描述:
* 读出Flash数据到指定缓存区域。
* @attention 无。
*
* @param ulAddress [IN] 指定的Flash地址偏移。
* @param ulSize [IN] 指定读取的长度, 字节。
* @param pReadSize [OUT] 实际读取到的长度。取值为HI_NULL表示不需要返回实际读取长度。
* @param pBuffer [OUT] 目的缓存地址。
*
* @retval #HI_ERR_SUCCESS 读成功。
* @retval #其他值 读失败，详见hi_errno.h。
*
* @par Dependency:
* @li hi_mdm_flash.h: 该接口声明所在的头文件。
* @since HiMDL_V200R001C00
*/

HI_EAPI HI_U32 HI_MDM_FLASH_Read(HI_CONST HI_U32 ulAddress, HI_CONST HI_U32 ulSize, HI_U32 *pReadSize, HI_U8 *pBuffer);

/**
* @ingroup  hct_FLASH
* @brief  把数据写入指定的Flash区
*
* @par 描述:
* 把数据写入指定的Flash区域,建议使用的地址空间为0x200000之后的地址空间。
* @attention
* @li Flash相对地址的限制保护。
* @li Flash擦写次数限制。
* @li 根据用户实际控制场景决定是否先擦后写。
*
* @param ulAddress [IN] 指定写入Flash地址偏移。
* @param ulSize [IN] 需要写入的长度。
* @param pWrittenSize [OUT] 实际写入的长度。取值为HI_NULL表示不需要返回实际写长度。
* @param pBuffer [IN] 需要写入的数据的缓存地址。
* @param bAutoErase [IN] 是否自动擦除并覆盖写入。
*
* @retval #HI_ERR_SUCCESS 写成功。
* @retval #其他值 写失败.详见hi_errno.h。
*
* @par Dependency:
* @li hi_mdm_flash.h: 该接口声明所在的头文件。
* @since HiMDL_V200R001C00
*/

HI_EAPI HI_U32 HI_MDM_FLASH_Write(HI_CONST HI_U32 ulAddress, HI_U32 ulSize, HI_U32 *pWrittenSize, HI_CONST HI_U8 *pBuffer, HI_BOOL bAutoErase);

/**
* @ingroup  hct_FLASH
* @brief  把指定的Flash区域数据擦除。
*
* @par 描述:
* 把指定的Flash区域数据擦除。
* @attention Flash相对地址的限制保护；Flash擦写次数限制。
*
* @param ulAddress [IN] 指定要擦除Flash的地址偏移。地址须4K对齐。
* @param ulSize [IN] 需要擦除的长度(单位byte)，须是4K倍数。
* @param pEraseSize [OUT] 实际擦除数据大小。
*
* @retval #HI_ERR_SUCCESS 擦除成功。
* @retval #其他值 擦除失败.详见hi_errno.h。
*
* @par Dependency:
* @li hi_mdm_flash.h: 该接口声明所在的头文件。
* @since HiMDL_V200R001C00
*/

HI_EAPI HI_U32 HI_MDM_FLASH_Erase(HI_CONST HI_U32 ulAddress, HI_CONST HI_U32 ulSize, HI_U32 *pEraseSize);

//****************************************************************************
// 函数名称: HI_MDM_FLASH_WriteSafe
// 功能描述: 把数据写入指定的Flash区域(对操作地址进行检查),建议使用的地址空间为0x200000之后的地址空间。
//
// 参数说明:
//   ulAddress [in] : 指定写入Flash地址偏移
//   ulSize [in] : 需要写入的长度
//   pWrittensize [out] : 实际写入的长度
//   pBuffer [in] : 需要写入的数据的缓存地址
//   bAutoErase [in] : HI_TRUE: 自动擦除并覆盖写入.
//
// 返 回 值:
//    HI_ERR_SUCCESS : 成功
//    其它值为错误码
// 调用要求:
//    注意Flash相对地址的限制保护；
//    注意Flash擦写次数限制;
//    根据用户实际控制场景决定是否先擦后写;
//****************************************************************************
HI_EAPI HI_U32 HI_MDM_FLASH_WriteSafe(HI_CONST HI_U32 ulAddress, HI_U32 ulSize, HI_U32 *pWrittenSize, HI_CONST HI_U8*pBuffer, HI_BOOL bAutoErase);

//****************************************************************************
// 函数名称: HI_MDM_FLASH_EraseSafe
// 功能描述: 把指定的Flash区域数据擦除(对操作地址进行检查),建议使用的地址空间为0x200000之后的地址空间。
//
// 参数说明:
//   ulAddress [in] : 指定写入Flash地址偏移
//   ulSize [in] : 需要擦除的长度(单位byte)，须是4K倍数
//   pEraseSize [out] : 实际擦除数据大小
//
// 返 回 值:
//    HI_ERR_SUCCESS : 成功
//    其它值为错误码
// 调用要求:
//    注意Flash相对地址的限制保护；
//    注意Flash擦写次数限制;
//****************************************************************************
HI_EAPI HI_U32 HI_MDM_FLASH_EraseSafe(HI_CONST HI_U32 ulAddress, HI_CONST HI_U32 ulSize, HI_U32 *pEraseSize);

//*****************************************************************************
// 函数名称: HI_MDM_FLASH_Init
// 功能描述: 初始化flash设备
//
// 参数说明:
//           无
//
// 返 回 值:
//           成功返回HI_ERR_SUCCESS；
//           失败返回HI_ERR_FAILURE；
//
// 调用要求:
//
// 调用举例: TODO: ...
// 作    者:
//*****************************************************************************
#define HI_MDM_FLASH_Init() HI_FLASH_Init()

//*****************************************************************************
// 函数名称: HI_MDM_FLASH_DeInit
// 功能描述: 去初始化flash设备
//
// 参数说明:
//           无
//
// 返 回 值:
//           成功返回HI_ERR_SUCCESS；
//           失败返回HI_ERR_FAILURE；
//
// 调用要求:
//
// 调用举例: TODO: ...
// 作    者:
//*****************************************************************************
#define HI_MDM_FLASH_DeInit() HI_FLASH_DeInit()

/**
* @ingroup  hct_FLASH
* @brief  Flash控制相关接口。
*
* @par 描述:
* Flash控制相关接口，目前仅实现获取Flash信息的功能。
* @attention
* @li Flash相对地址的限制保护。
* @li Flash擦写次数限制。
*
* @param  usCommand [IN] 类型 #HI_U16 要执行的命令。取值#HI_GET_FLASH_INFO时，为获取FLASH信息。
* @param  pstFlashInfo [OUT] 类型 #HI_FLASH_INFO_S *  获取到的FALSH信息。
*
* @retval #HI_ERR_SUCCESS 成功。
* @retval #其他值 失败.详见hi_errno.h。
*
* @par Dependency:
* @li hi_mdm_flash.h: 该接口声明所在的头文件。
* @since HiMDL_V200R001C00
*/

#define HI_MDM_FLASH_Ioctl(usCommand, pstFlashInfo) HI_FLASH_Ioctl(usCommand, pstFlashInfo)

#endif

HI_END_HEADER
#endif