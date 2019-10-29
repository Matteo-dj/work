/**@defgroup hct_mem  �ڴ�
 * @ingroup mem
 */

/** 
* @file hi_mdm_mem.h
* 
*  Copyright (C) HiSilicon Technologies Co., Ltd. 2017. All rights reserved.  \n
* 
* �������ڴ���ؽӿ�������\n
* @liϵͳ���ѿռ仮�ֳ���ϵͳ�ڴ�ء���̬�ڴ�ء���̬�ڴ��3���ڴ�أ�
*       ���ж�̬�ڴ�غ;�̬�ڴ���Ǵ�ϵͳ�ڴ������ռ�󴴽���,
*       ���ϵͳ�ڴ�ص��ܴ�С��ʹ�ô�С�����˶�̬�ڴ�غ;�̬�ڴ�صĴ�С��
* @li ϵͳ�ڴ�ص�ʣ��ռ�Ԥ����OSʹ�ã�������ʹ�ã�����malloc��free�ӿڽ��������ͷš�
* @li ��̬�ڴ��ֻ֧�ֳ�ʼ���׶ε��ã���֧���ͷţ�����HI_MDM_GetStaticMem�ӿ����롣
* @li ��̬�ڴ��Ϊƽʱʹ�õ��ڴ棬����HI_MDM_Malloc��HI_MDM_Free���������ͷš�
* @li �ڴ�ģ��֧�ְ����ڴ�ؽ����ڴ�ʹ�����ͳ�ƺͰ���ģ������ڴ�ʹ�����ͳ�ơ�Ŀǰ���֧��HI_MEM_SUPPORT_STAT_MOD_COUNT(60)��ģ����ڴ�ʹ�����ͳ�ơ�
*
*/


#ifndef __HI_MDM_MEM_H__
#define __HI_MDM_MEM_H__
#include <hi_types.h>
#include <hi_mdm_types.h>
#define HI_MEM_INVALID_POOL_ID 0xffffffff   /**< �Ƿ��ڴ�ص�ַID       */

/**
 * @ingroup hct_mem
 * �ڴ��ʹ�����ͳ����Ϣ��
 */
typedef struct
{
	HI_U32 ulPoolId;          /**< �ڴ��ID       */
    //OS��ȡ��Ϣ
    HI_U32 ulTotalSize;       /**< �ڴ�ش�С    */
    HI_U32 ulTotalUsedSize;   /**< ��ʹ�ô�С    */
    HI_U32 ulTotalFreeSize;   /**< ʣ��ռ�    */
    HI_U32 ulMaxFreeNodeSize; /**< ���ڵ��С    */
    HI_U32 ulUsedNodeNum;     /**< ʹ�ýڵ����    */
    HI_U32 ulFreeNodeNum;     /**< ���нڵ����    */

    //�м��ͳ��
    HI_U32 peekSec;           /**< ��ֵʱ��    */
    HI_U32 peekSize;          /**< ��ֵ��С    */
    HI_U32 usedSize;          /**< ��ǰֵ    */
    
    HI_U16 mallocSucTimes;    /**< ����ɹ�����    */
    HI_U16 freeSucTimes;      /**< �ͷųɹ�����    */
    
    HI_U16 mallocFailTimes;   /**< ����ʧ�ܴ���    */
    HI_U16 freeFailTimes;     /**< �Ƿ�ʧ�ܴ���    */

    HI_U16 mallocParamErrTimes;/**< ����ռ��С�Ƿ�����   */
    HI_U16 freeParamErrTimes;  /**< �Ƿ��ڴ��ַ�Ƿ�����  */
}HI_MDM_MEM_POOL_INFO_S;

/**
 * @ingroup hct_mem
 * �����ڴ���Ϣ
 */
typedef struct
{
	HI_U32  heapTotal;/**<ϵͳ�ڴ�ش�С */
    HI_U32  heapUsed;/**<ϵͳ�ڴ��ʹ�ÿռ� */
    HI_U32  heapFree;/**<ϵͳ�ڴ��ʣ��ռ� */
	HI_U32  codeSize;/**<text��+rodata�δ�С */
    HI_U32  dataSize;/**<�쳣����ջ��С */
    HI_U32  bssSize;/**<bss�δ�С */
    HI_U32  rodataSize;/**<��֧�� */
    HI_U32  mptSize;/**<mmuҳ���С */
}HI_MDM_MEM_INFO_S;

/**
 * @ingroup hct_mem
 * ģ���ڴ�ʹ��ͳ����Ϣ(������̬�ڴ�غͶ�̬�ڴ�ص�ͳ��)��
 */
typedef struct
{
    HI_U16 modId;               /**< ģ��ID     */
    HI_U16 pad;                 /**< ����     */
    
    HI_U32 peekSec;             /**< ��ֵʱ��     */
    HI_U32 peekSize;            /**< ��ֵ��С      */
    HI_U32 usedSize;            /**< ��ǰֵ      */
    
    HI_U16 mallocSucTimes;      /**< ����ɹ�����      */
    HI_U16 freeSucTimes;        /**< �ͷųɹ�����      */
    
    HI_U16 mallocFailTimes;     /**< ����ʧ�ܴ���      */
    HI_U16 freeFailTimes;       /**< �Ƿ�ʧ�ܴ���      */

    HI_U16 mallocParamErrTimes; /**< ����ռ��С�Ƿ�     */
    HI_U16 freeParamErrTimes;   /**< �Ƿ��ڴ��ַ�Ƿ�     */
}HI_MDM_MEM_MOD_INFO_S;

/**
* @ingroup  hct_mem
* @brief  ���붯̬�ڴ�ؿռ䡣
*
* @par ����:
* ���붯̬�ڴ�ؿռ䡣
* @attention 
* @li ͬһ���ڴ������ģ��ID���ͷ�ģ��IDҪ��ͬ������ᵼ���ڴ�ͳ����Ϣ����ȷ��
* @param  ulModId [IN] ����ģ��ID���û��豣֤ulModId����ȷ�ԣ������������жϡ�
*��ID����ͳ���ڴ�ʹ�������ȡֵ��hi_config.h�е�HI_MOD_ID_XX�к궨�岿�֣�
*XX�滻Ϊ����ģ�飬��HI_MOD_ID_SAL�����и����˳���ģ���ID���û������Լ�����ID������ֵ���������еĳ�ͻ��
* @param  ulSize [IN] �����ڴ��С
* @retval ��HI_NULL ������ڴ�ָ�롣
* @retval HI_NULL �����ڴ�ʧ�ܡ�
* @par Dependency:
* <ul><li>hi_mdm_mem.h: �ļ����������ڴ���ؽӿ�.</li></ul>
* @see  HI_MDM_Free��
* @since HiMDL_V200R001C00
*/
HI_EXTERN HI_PVOID HI_MDM_Malloc(HI_U32 ulModId, HI_U32 ulSize);

/**
* @ingroup  hct_mem
* @brief  �ͷŶ�̬�ڴ�ؿռ䡣
*
* @par ����:
* �ͷŶ�̬�ڴ�ؿռ䡣
* @attention �ޡ�
* @param  ulModId [IN] ����ģ��ID��ȡֵ��hi_config.h�е�HI_MOD_ID_XX�к궨�岿�֡��û��豣֤ulModId����ȷ�ԣ������������жϡ�
* @param  pAddr [IN] �ڴ��׵�ַ
* @par Dependency:
* <ul><li>hi_mdm_mem.h: �ļ����������ڴ���ؽӿ�.</li></ul>
* @see  HI_MDM_Malloc��
* @since HiMDL_V200R001C00
*/
HI_EXTERN HI_VOID HI_MDM_Free(HI_U32 ulModId, HI_PVOID pAddr);

/**
* @ingroup  hct_mem
* @brief  ���뾲̬�ڴ�ؿռ䡣
*
* @par ����:
* ���뾲̬�ڴ�ؿռ䡣
* @attention �ޡ�
* @param  ulModId [IN] ����ģ��ID��ȡֵ��hi_config.h�е�HI_MOD_ID_XX�к궨�岿�֡�
* @param  ulSize [IN] �����ڴ��С
* @retval ��HI_NULL ������ڴ�ָ�롣
* @retval HI_NULL �����ڴ�ʧ�ܡ�
* @par Dependency:
* <ul><li>hi_mdm_mem.h: �ļ����������ڴ���ؽӿ�.</li></ul>
* @see  �ޡ�
* @since HiMDL_V200R001C00
*/
HI_EXTERN HI_PVOID HI_MDM_GetStaticMem(HI_U32 ulModId, HI_U32 ulSize);



/**
* @ingroup  hct_mem
* @brief  ��ȡ����ڴ��С��
*
* @par ����:
* ��ȡ����ڴ��С��
* @attention �ޡ�
* @retval #HI_U32������ֵ ����ڴ��С��
* @par Dependency:
* <ul><li>hi_mdm_mem.h: �ļ����������ڴ���ؽӿ�.</li></ul>
* @see  HI_MDM_MEM_GetSwSize��
* @since HiMDL_V200R001C00
*/
HI_EXTERN HI_U32 HI_MDM_MEM_GetSwSize(HI_VOID);

/**
* @ingroup  hct_mem
* @brief  ��ȡ�ڴ�������Ϣ��
*
* @par ����:
* ��ȡ�ڴ�������Ϣ��
* @attention �ޡ�
* @param  pstSysMemInfo [OUT] ���� �����ڴ���Ϣ
* @retval #HI_ERR_SUCCESS �ɹ���
* @retval ����ֵ ʧ�ܣ����hi_errno.h��
* @par Dependency:
* <ul><li>hi_mdm_mem.h: �ļ����������ڴ���ؽӿ�.</li></ul>
* @see  �ޡ�
* @since HiMDL_V200R001C00
*/
HI_EXTERN HI_U32 HI_MDM_MEM_GetSysInfo(HI_OUT HI_MDM_MEM_INFO_S * pstSysMemInfo);

/**
* @ingroup  hct_mem
* @brief  ��ȡ��̬�ڴ��ͳ����Ϣ��
*
* @par ����:
* ��ȡ��̬�ڴ��ͳ����Ϣ��
* @attention �ޡ� 
* @param  pstMemPoolInfo [OUT] ���� ��̬�ڴ���Ϣ
* @retval #HI_ERR_SUCCESS �ɹ���
* @retval ����ֵ ʧ�ܣ����hi_errno.h��
* @par Dependency:
* <ul><li>hi_mdm_mem.h: �ļ����������ڴ���ؽӿ�.</li></ul>
* @see  �ޡ�
* @since HiMDL_V200R001C00
*/
HI_EXTERN HI_U32 HI_MDM_MEM_GetStaticPoolInfo(HI_OUT HI_MDM_MEM_POOL_INFO_S *pstMemPoolInfo);

/**
* @ingroup  hct_mem
* @brief  ��ȡ��̬�ڴ��ͳ����Ϣ��
*
* @par ����:
* ��ȡ��̬�ڴ��ͳ����Ϣ��
* @attention �ޡ�
* @param  pstSysMemInfo [OUT] ��̬�ڴ���Ϣ
* @retval #HI_ERR_SUCCESS �ɹ���
* @retval ����ֵ ʧ�ܣ����hi_errno.h��
* @par Dependency:
* <ul><li>hi_mdm_mem.h: �ļ����������ڴ���ؽӿ�.</li></ul>
* @see  �ޡ�
* @since HiMDL_V200R001C00
*/
HI_EXTERN HI_U32 HI_MDM_MEM_GetDynPoolInfo(HI_OUT HI_MDM_MEM_POOL_INFO_S *pstMemPoolInfo);

/**
* @ingroup  hct_mem
* @brief  ��ȡϵͳ�ڴ��ͳ����Ϣ��
*
* @par ����:
* ��ȡϵͳ�ڴ��ͳ����Ϣ��
* @attention �ޡ�
* @param  pstMemPoolInfo [OUT] ���� ϵͳԤ���Ŀɷ�����ڴ���Ϣ
* @retval #HI_ERR_SUCCESS �ɹ���
* @retval ����ֵ ʧ�ܣ����hi_errno.h��
* @par Dependency:
* <ul><li>hi_mdm_mem.h: �ļ����������ڴ���ؽӿ�.</li></ul>
* @see  �ޡ�
* @since HiMDL_V200R001C00
*/
HI_EXTERN HI_U32 HI_MDM_MEM_GetSysPoolInfo(HI_OUT HI_MDM_MEM_POOL_INFO_S *pstMemPoolInfo);

/**
* @ingroup  hct_mem
* @brief  ��ȡ��ǰ���ڴ��������ͷŲ�����ģ�������
*
* @par ����: 
* ��ȡ��ǰ���ڴ��������ͷŵ�ģ�������
* @attention �ޡ�
* @retval #HI_U32������ֵ ģ�������
* @par Dependency:
* <ul><li>hi_mdm_mem.h: �ļ����������ڴ���ؽӿ�.</li></ul>
* @see  HI_MDM_MEM_GetModInfoByIdx
* @since HiMDL_V200R001C00
*/
HI_EXTERN HI_U32 HI_MDM_MEM_GetModNum(HI_VOID);

/**
* @ingroup  hct_mem
* @brief  ��������ȡģ���ڴ�ʹ�������
*
* @par ����: 
* ��������ȡģ���ڴ�ʹ�������
* @attention �ޡ�
* @param  ulIdx [IN] ģ�������š�ȡֵ��Χ[0,HI_MOD_MAX_NUM)��
* @param  pstMemModInfo [OUT] �ڴ�ʹ�������
* @retval #HI_ERR_SUCCESS �ɹ���
* @retval ����ֵ ʧ�ܣ����hi_errno.h��
* @par Dependency:
* <ul><li>hi_mdm_mem.h: �ļ����������ڴ���ؽӿ�.</li></ul>
* @see  HI_MDM_MEM_GetModNum
* @since HiMDL_V200R001C00
*/
HI_EXTERN HI_U32 HI_MDM_MEM_GetModInfoByIdx(HI_U32 ulIdx,HI_MDM_MEM_MOD_INFO_S *pstMemModInfo);

/**
* @ingroup  hct_mem
* @brief  ��ģ��ID��ȡģ���ڴ�ʹ�������
*
* @par ����:
* ��ģ��ID��ȡģ���ڴ�ʹ�������
* @attention �ޡ�
* @param  ulModId [IN] ģ��ID�ţ�ȡֵ��hi_config.h�е�HI_MOD_ID_XX�к궨�岿�֡�
* @param  pstMemModInfo [OUT] �ڴ�ʹ�������
* @retval #HI_ERR_SUCCESS �ɹ���
* @retval ����ֵ ʧ�ܣ����hi_errno.h��
* @par Dependency:
* <ul><li>hi_mdm_mem.h: �ļ����������ڴ���ؽӿ�.</li></ul>
* @see  �ޡ�
* @since HiMDL_V200R001C00
*/
HI_EXTERN HI_U32 HI_MDM_MEM_GetModInfoByModId(HI_U32 ulModId,HI_MDM_MEM_MOD_INFO_S *pstMemModInfo);
#endif
