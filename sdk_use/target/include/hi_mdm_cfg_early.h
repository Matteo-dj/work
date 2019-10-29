/**@defgroup hct_init_early ��ʼ������
 * @ingroup osa
 */
 
 /** 
 * @file hi_mdm_cfg_early.h
 * 
 *  Copyright (C) HiSilicon Technologies Co., Ltd. 2017. All rights reserved.  \n
 * 
 * ���������ļ���������һ�׶�(OS��ʼ��֮ǰ)����������ؽӿڡ�\n
 * @li ���ļ������ӿڽ�֧����APP_InitCfgEarly�����е��á�
 * @li APP_InitCfgEarly��ʼ���׶Σ�BSS����δδ��ʼ������˲���ʹ��BSS�α�����
 * @li APP_InitCfgEarly��ʼ���׶Σ�ϵͳδ��ʼ����ɣ���hi_mdm_cfg_early.h�ṩ�Ľӿں�һЩ��Ӳ���ӿ��⣬���ܵ��������ӿ�(�����ڴ棬����˯�ߣ�����ϵͳ��Դ��ؽӿ�)��
 *
 */
#ifndef __HI_MDM_CFG_EARLY_H__
#define __HI_MDM_CFG_EARLY_H__

/**
 * @ingroup hct_init_early
 * ϵͳ��Դ��ʼ������
 */
typedef struct
{
    HI_U32 ulTskNum;/**< task���� */
    HI_U32 ulQueNum;/**< ��Ϣ���и��� */
    HI_U32 ulSemNum;/**< �ź������� */
    HI_U32 ulMuxNum;/**< ���������� */
    HI_U32 ulSwtNum;/**< ϵͳ��ʱ������ */
    HI_U32 ulWorkqNum;/**< �жϺ���job���� */
    HI_U32 ulEventNum;/**< �¼����� */
}HI_CFG_OSA_RESOURCE_S;

/**
* @ingroup  hct_init_early
* @brief  ����osa��Դ
*
* @par ����:
* ����osa��Դ
* @attention ���ӿڽ�֧���ڳ�ʼ���׶ε��á�
*
* @param  pCfgOsaResouce [IN] ϵͳ��Դ��ʼ�����á�
*
* @retval #HI_ERR_SUCCESS �ɹ���
* @retval ����ֵ ʧ�ܣ����hi_errno.h��
*
* @par Dependency:
* @li hi_mdm_cfg_early.h: �ýӿ��������ڵ�ͷ�ļ���
* @see �ޡ�
* @since HiMDL_V200R001C00
*/

HI_U32 HI_CFG_SetOsaResourceEarly(HI_CFG_OSA_RESOURCE_S *pCfgOsaResouce);


/**
* @ingroup  hct_init_early
* @brief  ��������ڴ��С��
*
* @par ����:
* ��������ڴ��С�����������ʵ������������С��
* @attention ���ӿڽ�֧���ڳ�ʼ���׶ε��á�
* @param  ulSize [IN] �����ڴ��С
* @retval #HI_ERR_SUCCESS �ɹ���
* @retval ����ֵ ʧ�ܣ����hi_errno.h��
* @par Dependency:
* @li hi_mdm_cfg_early.h: �ýӿ��������ڵ�ͷ�ļ���
* @see  HI_MDM_MEM_GetSwSize��
* @since HiMDL_V200R001C00
*/
HI_U32 HI_MDM_MEM_SetSwSize(HI_IN HI_U32 ulSize);

#endif

