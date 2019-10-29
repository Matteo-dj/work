
/**@defgroup hct_share_mem  �����ڴ�
 * @ingroup mem
 */
 
/** 
* @file hi_share_memory.h
* 
*  Copyright (C) HiSilicon Technologies Co., Ltd. 2018. All rights reserved.  \n
* 
* �����������ڴ�ӿڡ�\n
* @li ����1:���ϵ��ʼ���׶�����һ��ܴ���ڴ棬��ֹϵͳ���й������޷����뵽һ������ڴ档
* @li ����2:֧�ֶ�����ܷ�ʱ���øÿ���ڴ档
*
* History:
* Version                  Date         DefectNum           Description
* V200R001C00SPC050B011    2018-12-03   DTS2018112903669    ���ӹ����ڴ�ӿ�
*/

#ifndef __HI_SHARE_MEMORY_H__
#define __HI_SHARE_MEMORY_H__
#include <hi_types.h>
#include <hi_mdm_types.h>

#ifdef PRODUCT_CFG_SUPPORT_SHARE_MEMORY

/**
* @ingroup  hct_share_mem
* @brief  �����ڴ��ʼ����
*
* @par ����:  
* �����ڴ��ʼ����
* @attention �ޡ�
* 
* @param �ޡ�
*
* @retval #HI_ERR_SUCCESS �ɹ���
* @retval ����ֵ ʧ�ܣ����hi_errno.h��
*
* @par Dependency:
* @li hi_share_memory.h: �ýӿ��������ڵ�ͷ�ļ���
* @see  �ޡ�
* @since HiMDL_V200R001C00
*/
HI_U32 HI_SHARE_MEM_Init(HI_VOID);

/**
* @ingroup  hct_share_mem
* @brief  ���빲���ڴ档
*
* @par ����:  
* ���빲���ڴ档
* @attention �ޡ�
* 
* @param  ulSize [IN] Ԥ����Ĺ����ڴ��С��
*
* @retval #HI_PVOID ���뵽�Ĺ����ڴ��ַ��
*
* @par Dependency:
* @li hi_share_memory.h: �ýӿ��������ڵ�ͷ�ļ���
* @see  �ޡ�
* @since HiMDL_V200R001C00
*/
HI_PVOID HI_SHARE_MEM_GetMem(HI_U32 ulSize);

/**
* @ingroup  hct_share_mem
* @brief  �ͷŹ����ڴ档
*
* @par ����:  
* �ͷŹ����ڴ档
* @attention �ޡ�
* 
* @param  ulAddr [IN] �ͷŵ��ڴ��ַ��
*
* @par Dependency:
* @li hi_share_memory.h: �ýӿ��������ڵ�ͷ�ļ���
* @see  �ޡ�
* @since HiMDL_V200R001C00
*/
HI_VOID HI_SHARE_MEM_FreeMem(HI_PVOID ulAddr);

/**
* @ingroup  hct_share_mem
* @brief  �����ڴ��Ƿ���С�
*
* @par ����:  
* �����ڴ��Ƿ���С�
* @attention �ޡ�
* 
* @param �ޡ�
*
* @retval #HI_TRUE ���С�
* @retval #HI_FALSE ��ռ�á�
*
* @par Dependency:
* @li hi_share_memory.h: �ýӿ��������ڵ�ͷ�ļ���
* @see  �ޡ�
* @since HiMDL_V200R001C00
*/
HI_BOOL HI_SHARE_MEM_IsFreeMem(HI_VOID);
#else
/**
* @ingroup  hct_share_mem
* @brief  �����ڴ��ʼ����
*
* @par ����:  
* �����ڴ��ʼ����
* @attention �ޡ�
* 
* @param �ޡ�
*
* @retval #HI_ERR_SUCCESS �ɹ���
* @retval ����ֵ ʧ�ܣ����hi_errno.h��
*
* @par Dependency:
* @li hi_share_memory.h: �ýӿ��������ڵ�ͷ�ļ���
* @see  �ޡ�
* @since HiMDL_V200R001C00
*/
HI_PRVL HI_U32 HI_SHARE_MEM_Init(HI_VOID){return HI_ERR_SUCCESS;}
/**
* @ingroup  hct_share_mem
* @brief  ���빲���ڴ档
*
* @par ����:  
* ���빲���ڴ档
* @attention �ޡ�
* 
* @param  ulSize [IN] Ԥ����Ĺ����ڴ��С��
*
* @retval #HI_PVOID ���뵽�Ĺ����ڴ��ַ��
*
* @par Dependency:
* @li hi_share_memory.h: �ýӿ��������ڵ�ͷ�ļ���
* @see  �ޡ�
* @since HiMDL_V200R001C00
*/
HI_PRVL HI_PVOID HI_SHARE_MEM_GetMem(HI_U32 ulSize){return HI_NULL;}
/**
* @ingroup  hct_share_mem
* @brief  �ͷŹ����ڴ档
*
* @par ����:  
* �ͷŹ����ڴ档
* @attention �ޡ�
* 
* @param  ulAddr [IN] �ͷŵ��ڴ��ַ��
*
* @par Dependency:
* @li hi_share_memory.h: �ýӿ��������ڵ�ͷ�ļ���
* @see  �ޡ�
* @since HiMDL_V200R001C00
*/
HI_PRVL HI_VOID HI_SHARE_MEM_FreeMem(HI_PVOID ulAddr){return;}
/**
* @ingroup  hct_share_mem
* @brief  �����ڴ��Ƿ���С�
*
* @par ����:  
* �����ڴ��Ƿ���С�
* @attention �ޡ�
* 
* @param �ޡ�
*
* @retval #HI_TRUE ���С�
* @retval #HI_FALSE ��ռ�á�
*
* @par Dependency:
* @li hi_share_memory.h: �ýӿ��������ڵ�ͷ�ļ���
* @see  �ޡ�
* @since HiMDL_V200R001C00
*/
HI_PRVL HI_BOOL HI_SHARE_MEM_IsFreeMem(HI_VOID){return HI_FALSE;}
#endif

#endif//__HI_MDM_SHARE_MEMORY_H__

