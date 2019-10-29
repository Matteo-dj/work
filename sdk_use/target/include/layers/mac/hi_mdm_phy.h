/**@defgroup hct_phy ������ͨ������
 * @ingroup mdm
 */
/** 
* @file hi_mdm_phy.h
* 
*  Copyright (C) HiSilicon Technologies Co., Ltd. 2018. All rights reserved.  \n
* 
* History:
* Version                  Date         DefectNum           Description
* V200R001C00SPC050B011    2018-12-03   DTS2018112903669    �����ļ�֧���źŷ��͡�
*/

#ifndef __HI_MDM_PYH_H__
#define __HI_MDM_PYH_H__
#include <hi_types.h>
#include <hi_mdm_types.h>
HI_START_HEADER

#define PHY_WORK_CTRL_ENABLE            0/*ʹ��MAC��PHY����*/
#define PHY_WORK_CTRL_DISABLE           1/*��ֹMAC��PHY����*/


/**
* @ingroup  hct_phy
* @brief  line driver�Ľ�ֹ��
*
* @par ����:
* line driver�Ľ�ֹ��
* @attention �ޡ�
*
* @par Dependency:
* @li hi_mdm_phy.h: �ýӿ��������ڵ�ͷ�ļ���
* @see  HI_MDM_LineDriverEnable ��
* @since HiMDL_V200R001C00
*/
HI_EXTERN HI_VOID HI_MDM_LineDriverDisable(HI_VOID);

/**
* @ingroup  hct_phy
* @brief  line driverʹ�ܡ�
*
* @par ����:
* line driverʹ�ܡ�
* @attention �ޡ�
*
* @par Dependency:
* @li hi_mdm_phy.h: �ýӿ��������ڵ�ͷ�ļ���
* @see  HI_MDM_LineDriverDisable ��
* @since HiMDL_V200R001C00
*/
HI_EXTERN HI_VOID HI_MDM_LineDriverEnable(HI_VOID);

/**
* @ingroup  hct_phy
* @brief ����PHY�Ĺ���ģʽ���ء�
*
* @par ����:
* ����PHY�Ĺ���ģʽ���ء�
* @attention  �ýӿڽ�֧�ֳ��ر���̬ʹ�á�
*
* @param  flag [IN] phy����ģʽ��ȡֵPHY_WORK_CTRL_XXX��
* 
* @par Dependency:
* @li hi_mdm_phy.h: �ýӿ��������ڵ�ͷ�ļ���
* @see  HI_MDM_PHY_GetWorkCtrlFlag ��
* @since HiMDL_V200R001C00
*/
HI_EXTERN HI_VOID HI_MDM_PHY_SetWorkCtrlFlag(HI_U8 flag);

/**
* @ingroup  hct_phy
* @brief ��ȡPHY�Ĺ���ģʽ���ء�
*
* @par ����:
* ��ȡPHY�Ĺ���ģʽ���ء�
* @attention  �ýӿڽ�֧�ֳ��ر���̬ʹ�á�
*
* @retval phy����ģʽ  ȡֵΪPHY_WORK_CTRL_XXX��
* 
* @par Dependency:
* @li hi_mdm_phy.h: �ýӿ��������ڵ�ͷ�ļ���
* @see  HI_MDM_PHY_SetWorkCtrlFlag ��
* @since HiMDL_V200R001C00
*/
HI_EXTERN HI_U8 HI_MDM_PHY_GetWorkCtrlFlag(HI_VOID);


/**
* @ingroup  hct_phy
* @brief �����źŷ��͡�
*
* @par ����:
* �����źŷ��ͣ�ѭ������ָ���źš�
* @attention  �ýӿڽ�֧�ֳ��ر���̬ʹ�á�
*
* @param  pBuf [IN] �������ź�ָ�룬Ҫ��64�ֽڶ��롣
* @param  ulBufSize [IN] �ź��ļ���С��Ҫ��64�ֽڶ��롣
*
* @retval #HI_ERR_SUCCESS �ɹ���
* @retval ����ֵ ʧ�ܣ����hi_errno.h��
* 
* @par Dependency:
* @li hi_mdm_phy.h: �ýӿ��������ڵ�ͷ�ļ���
* @see  HI_MDM_PHY_StopSendSignal ��
* @since HiMDL_V200R001C00
*/
HI_EXTERN  HI_U32 HI_MDM_PHY_StartSendSignal(HI_U8 *pBuf,HI_U32 ulBufSize);

HI_END_HEADER
#endif

