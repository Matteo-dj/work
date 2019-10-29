/**@defgroup hct_phy 电力线通信驱动
 * @ingroup mdm
 */
/** 
* @file hi_mdm_phy.h
* 
*  Copyright (C) HiSilicon Technologies Co., Ltd. 2018. All rights reserved.  \n
* 
* History:
* Version                  Date         DefectNum           Description
* V200R001C00SPC050B011    2018-12-03   DTS2018112903669    新增文件支持信号发送。
*/

#ifndef __HI_MDM_PYH_H__
#define __HI_MDM_PYH_H__
#include <hi_types.h>
#include <hi_mdm_types.h>
HI_START_HEADER

#define PHY_WORK_CTRL_ENABLE            0/*使能MAC对PHY控制*/
#define PHY_WORK_CTRL_DISABLE           1/*禁止MAC对PHY控制*/


/**
* @ingroup  hct_phy
* @brief  line driver的禁止。
*
* @par 描述:
* line driver的禁止。
* @attention 无。
*
* @par Dependency:
* @li hi_mdm_phy.h: 该接口声明所在的头文件。
* @see  HI_MDM_LineDriverEnable 。
* @since HiMDL_V200R001C00
*/
HI_EXTERN HI_VOID HI_MDM_LineDriverDisable(HI_VOID);

/**
* @ingroup  hct_phy
* @brief  line driver使能。
*
* @par 描述:
* line driver使能。
* @attention 无。
*
* @par Dependency:
* @li hi_mdm_phy.h: 该接口声明所在的头文件。
* @see  HI_MDM_LineDriverDisable 。
* @since HiMDL_V200R001C00
*/
HI_EXTERN HI_VOID HI_MDM_LineDriverEnable(HI_VOID);

/**
* @ingroup  hct_phy
* @brief 设置PHY的工作模式开关。
*
* @par 描述:
* 设置PHY的工作模式开关。
* @attention  该接口仅支持抄控宝形态使用。
*
* @param  flag [IN] phy工作模式。取值PHY_WORK_CTRL_XXX。
* 
* @par Dependency:
* @li hi_mdm_phy.h: 该接口声明所在的头文件。
* @see  HI_MDM_PHY_GetWorkCtrlFlag 。
* @since HiMDL_V200R001C00
*/
HI_EXTERN HI_VOID HI_MDM_PHY_SetWorkCtrlFlag(HI_U8 flag);

/**
* @ingroup  hct_phy
* @brief 获取PHY的工作模式开关。
*
* @par 描述:
* 获取PHY的工作模式开关。
* @attention  该接口仅支持抄控宝形态使用。
*
* @retval phy工作模式  取值为PHY_WORK_CTRL_XXX。
* 
* @par Dependency:
* @li hi_mdm_phy.h: 该接口声明所在的头文件。
* @see  HI_MDM_PHY_SetWorkCtrlFlag 。
* @since HiMDL_V200R001C00
*/
HI_EXTERN HI_U8 HI_MDM_PHY_GetWorkCtrlFlag(HI_VOID);


/**
* @ingroup  hct_phy
* @brief 启动信号发送。
*
* @par 描述:
* 启动信号发送，循环发送指定信号。
* @attention  该接口仅支持抄控宝形态使用。
*
* @param  pBuf [IN] 被发送信号指针，要求64字节对齐。
* @param  ulBufSize [IN] 信号文件大小，要求64字节对齐。
*
* @retval #HI_ERR_SUCCESS 成功。
* @retval 其他值 失败，详见hi_errno.h。
* 
* @par Dependency:
* @li hi_mdm_phy.h: 该接口声明所在的头文件。
* @see  HI_MDM_PHY_StopSendSignal 。
* @since HiMDL_V200R001C00
*/
HI_EXTERN  HI_U32 HI_MDM_PHY_StartSendSignal(HI_U8 *pBuf,HI_U32 ulBufSize);

HI_END_HEADER
#endif

