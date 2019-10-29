

/** 
* @file hi_algorithm.h
* 
*  Copyright (C) HiSilicon Technologies Co., Ltd. 2018. All rights reserved.  \n
* 
* History:
* Version                  Date         DefectNum           Description
* V200R001C00SPC050B011    2018-12-03   DTS2018112903669    �����ӿ�֧���źŷ��͡�
*/

#ifndef __HI_ALGORITHM_H__
#define __HI_ALGORITHM_H__
#include <hi_types.h>
#include <hi_mdm_types.h>
HI_START_HEADER


/**@defgroup hct_CRC CRCУ��
 * @ingroup mdm
*/

/**
* @ingroup  hct_CRC
* @brief  ����32λCRCУ��ֵ��
*
* @par ����:
* ����32λCRCУ��ֵ��
* @attention ���������߼�飬buf����Ϊ�ա�
*
* @param  ulInitData [IN] ��ʼCRC��
* @param  pbuf [IN] ��У��bufָ�롣
* @param  ulLen [IN] ��У��buf���ȡ�
*
* @retval #HI_U32������ֵ CRC����ֵ��
*
* @par Dependency:
* @li hi_algorithm.h: �ýӿ��������ڵ�ͷ�ļ���
* @see �ޡ�
* @since HiMDL_V200R001C00
*/
HI_EAPI HI_U32 HI_MDM_Crc32(HI_U32 ulInitData, HI_CONST HI_U8* pbuf, HI_U32 ulLen);
/**
* @ingroup  hct_CRC
* @brief  ����16λCRCУ��ֵ��
*
* @par ����:
* ����16λCRCУ��ֵ��
* @attention ���������߼�飬buf����Ϊ�ա�
*
* @param  usInitData [IN] CRC��ʼֵ��
* @param  pbuf [IN] ��У��bufָ�롣
* @param  ulLen [IN] ��У��buf���ȡ�
*
* @retval #HI_U16������ֵ     CRC����ֵ
*
* @par Dependency:
* @li hi_algorithm.h: �ýӿ��������ڵ�ͷ�ļ���
* @see �ޡ�
* @since HiMDL_V200R001C00
*/
HI_EAPI HI_U16 HI_MDM_Crc16(HI_U16 usInitData, HI_CONST HI_U8 *pbuf, HI_U32 ulLen);

/**@defgroup hct_randnum �����
 * @ingroup mdm
*/

/**
* @ingroup  hct_randnum
* @brief ����ͨ��ģ��MAC��ַ�͵�ǰʱ������32λ�������
*
* @par ����:
* ����ͨ��ģ��MAC��ַ�͵�ǰʱ������32λ�������
*
*@attention
* @li ���ж��е���:���������ͨ��ģ��MAC��ַ������ʱ�����ɡ�
* @li �ж��е���:����������ݵ�ǰʱ�����ɡ�
*
* @param  ulMin [IN] �������Χ���ޡ�
* @param  ulMax [IN] �������Χ���ޣ����������쳣����ֵ����Ҫ�û���֤ulMinС��ulMax��
*
* @retval #HI_U32������ֵ �������
*
* @par Dependency:
* @li hi_algorithm.h: �ýӿ��������ڵ�ͷ�ļ���
* @see HI_MDM_GetRandNum16
* @since HiMDL_V200R001C00
*/
HI_EAPI HI_U32 HI_MDM_GetRandNum32(HI_U32 ulMin, HI_U32 ulMax);

/**
* @ingroup  hct_randnum
* @brief ����ͨ��ģ��MAC��ַ�͵�ǰʱ������16λ�������
*
* @par ����:
* ����ͨ��ģ��MAC��ַ�͵�ǰʱ������16λ�������
*
* @attention
* @li ���ж��е���:���������ͨ��ģ��MAC��ַ������ʱ�����ɡ�
* @li �ж��е���:����������ݵ�ǰʱ�����ɡ�
*
* @param ulMin [IN] �������Χ���ޡ�
* @param ulMax [IN] �������Χ���ޣ����������쳣����ֵ����Ҫ�û���֤ulMinС��ulMax��
*
* @retval #HI_U16������ֵ �������
*
* @par Dependency:
* @li hi_algorithm.h: �ýӿ��������ڵ�ͷ�ļ���
* @see HI_MDM_GetRandNum32
* @since HiMDL_V200R001C00
*/

HI_EAPI HI_U16 HI_MDM_GetRandNum16(HI_U16 ulMin, HI_U16 ulMax);

/**@defgroup hct_algorithm 
 * @ingroup mdm
*/
/**
* @ingroup  hct_algorithm
* @brief ����ɨƵ�źš�
*
* @par ����:
* ���ɴ�СΪ2M��ɨƵ�źš�
*
* @attention �ýӿڽ�֧�ֳ��ر���̬ʹ�á�
*
* @pBuf ulMin [IN] ����ɨƵ�źſռ䡣
* @ulBufsize ulMax [IN] �ռ��С������Ϊ2M��
*
* @retval #HI_ERR_SUCCESS �ɹ���
* @retval ����ֵ ʧ�ܣ����hi_errno.h��
*
* @par Dependency:
* @li hi_algorithm.h: �ýӿ��������ڵ�ͷ�ļ���
* @see HI_MDM_GetRandNum32
* @since HiMDL_V200R001C00
*/
HI_EAPI HI_U32 HI_MDM_CreateTestSignalFile(HI_U8 *pBuf,HI_U32 ulBufsize);

HI_END_HEADER
#endif
