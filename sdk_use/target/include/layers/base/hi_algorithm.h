

/** 
* @file hi_algorithm.h
* 
*  Copyright (C) HiSilicon Technologies Co., Ltd. 2018. All rights reserved.  \n
* 
* History:
* Version                  Date         DefectNum           Description
* V200R001C00SPC050B011    2018-12-03   DTS2018112903669    新增接口支持信号发送。
*/

#ifndef __HI_ALGORITHM_H__
#define __HI_ALGORITHM_H__
#include <hi_types.h>
#include <hi_mdm_types.h>
HI_START_HEADER


/**@defgroup hct_CRC CRC校验
 * @ingroup mdm
*/

/**
* @ingroup  hct_CRC
* @brief  生成32位CRC校验值。
*
* @par 描述:
* 生成32位CRC校验值。
* @attention 入参需调用者检查，buf不能为空。
*
* @param  ulInitData [IN] 初始CRC。
* @param  pbuf [IN] 被校验buf指针。
* @param  ulLen [IN] 被校验buf长度。
*
* @retval #HI_U32类型数值 CRC检验值。
*
* @par Dependency:
* @li hi_algorithm.h: 该接口声明所在的头文件。
* @see 无。
* @since HiMDL_V200R001C00
*/
HI_EAPI HI_U32 HI_MDM_Crc32(HI_U32 ulInitData, HI_CONST HI_U8* pbuf, HI_U32 ulLen);
/**
* @ingroup  hct_CRC
* @brief  生成16位CRC校验值。
*
* @par 描述:
* 生成16位CRC校验值。
* @attention 入参需调用者检查，buf不能为空。
*
* @param  usInitData [IN] CRC初始值。
* @param  pbuf [IN] 被校验buf指针。
* @param  ulLen [IN] 被校验buf长度。
*
* @retval #HI_U16类型数值     CRC检验值
*
* @par Dependency:
* @li hi_algorithm.h: 该接口声明所在的头文件。
* @see 无。
* @since HiMDL_V200R001C00
*/
HI_EAPI HI_U16 HI_MDM_Crc16(HI_U16 usInitData, HI_CONST HI_U8 *pbuf, HI_U32 ulLen);

/**@defgroup hct_randnum 随机数
 * @ingroup mdm
*/

/**
* @ingroup  hct_randnum
* @brief 根据通信模块MAC地址和当前时间生成32位随机数。
*
* @par 描述:
* 根据通信模块MAC地址和当前时间生成32位随机数。
*
*@attention
* @li 非中断中调用:随机数根据通信模块MAC地址和启动时间生成。
* @li 中断中调用:随机数仅根据当前时间生成。
*
* @param  ulMin [IN] 随机数范围下限。
* @param  ulMax [IN] 随机数范围上限，本函数无异常返回值，需要用户保证ulMin小于ulMax。
*
* @retval #HI_U32类型数值 随机数。
*
* @par Dependency:
* @li hi_algorithm.h: 该接口声明所在的头文件。
* @see HI_MDM_GetRandNum16
* @since HiMDL_V200R001C00
*/
HI_EAPI HI_U32 HI_MDM_GetRandNum32(HI_U32 ulMin, HI_U32 ulMax);

/**
* @ingroup  hct_randnum
* @brief 根据通信模块MAC地址和当前时间生成16位随机数。
*
* @par 描述:
* 根据通信模块MAC地址和当前时间生成16位随机数。
*
* @attention
* @li 非中断中调用:随机数根据通信模块MAC地址和启动时间生成。
* @li 中断中调用:随机数仅根据当前时间生成。
*
* @param ulMin [IN] 随机数范围下限。
* @param ulMax [IN] 随机数范围上限，本函数无异常返回值，需要用户保证ulMin小于ulMax。
*
* @retval #HI_U16类型数值 随机数。
*
* @par Dependency:
* @li hi_algorithm.h: 该接口声明所在的头文件。
* @see HI_MDM_GetRandNum32
* @since HiMDL_V200R001C00
*/

HI_EAPI HI_U16 HI_MDM_GetRandNum16(HI_U16 ulMin, HI_U16 ulMax);

/**@defgroup hct_algorithm 
 * @ingroup mdm
*/
/**
* @ingroup  hct_algorithm
* @brief 生成扫频信号。
*
* @par 描述:
* 生成大小为2M的扫频信号。
*
* @attention 该接口仅支持抄控宝形态使用。
*
* @pBuf ulMin [IN] 生成扫频信号空间。
* @ulBufsize ulMax [IN] 空间大小，必须为2M。
*
* @retval #HI_ERR_SUCCESS 成功。
* @retval 其他值 失败，详见hi_errno.h。
*
* @par Dependency:
* @li hi_algorithm.h: 该接口声明所在的头文件。
* @see HI_MDM_GetRandNum32
* @since HiMDL_V200R001C00
*/
HI_EAPI HI_U32 HI_MDM_CreateTestSignalFile(HI_U8 *pBuf,HI_U32 ulBufsize);

HI_END_HEADER
#endif
