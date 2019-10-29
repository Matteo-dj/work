/**@defgroup hct_nv NV
 * @ingroup sal
 */

#ifndef __HI_MDM_NV_H__
#define __HI_MDM_NV_H__

/**
* @ingroup  hct_nv
* @brief ����NVֵ��
*
* @par ����:
* ����NVֵ��
*@attention ��֧�������е��ã���֧���ж��е��á�
*
* @param  usId [IN] NV��ID��
* @param  pData [IN] NV�����ݡ�
* @param  usDataLen [IN] NV��ȡ�

* @retval #HI_ERR_SUCCESS �ɹ���
* @retval ����ֵ ʧ�ܣ����hi_errno.h��
*
* @par Dependency:
* @li hi_mdm_nv.h: �ýӿ��������ڵ�ͷ�ļ���
* @see HI_MDM_NV_Read
* @since HiMDL_V200R001C00
*/
HI_EAPI HI_U32 HI_MDM_NV_Write(HI_U16 usId, HI_CONST HI_PVOID pData, HI_U16 usDataLen);

/**
* @ingroup  hct_nv
* @brief ��ȡNVֵ��
*
* @par ����:
* ��ȡNVֵ��
* @attention ���ܲ����ǹ�����NV����֧�������е��ã���֧���ж��е��á�
*
* @param  usId [IN] NV��ID��
* @param  pItemData [OUT] NV�����ݡ�
* @param  usItemDataLen [IN] NV��ȡ�

* @retval #HI_ERR_SUCCESS �ɹ���
* @retval ����ֵ ʧ�ܣ����hi_errno.h��
*
* @par Dependency:
* @li hi_mdm_nv.h: �ýӿ��������ڵ�ͷ�ļ���
* @see HI_MDM_NV_Write
* @since HiMDL_V200R001C00
*/
HI_EAPI HI_U32 HI_MDM_NV_Read(HI_U16 usId, HI_OUT HI_PVOID pItemData, HI_U16 usItemDataLen);

/**
* @ingroup  hct_nv
* @brief NV����֪ͨ��
*
* @par ����:
* NV����֪ͨ��
* @attention 
* @li ��SAL��ʼ��������ſ��Ե��øýӿڡ�
* @li �ܹ�֧��ע��20����(��SDK�ڲ�)��
*
* @param  usMinId [IN] NV��ID��Сֵ��
* @param  usMaxId [IN] NV��ID���ֵ��
* @param  pfnChanged [in] NV��ı�Ĵ���������NV������ NVģ����Զ����ø�ע��Ľӿڡ�
*
* @retval #HI_ERR_SUCCESS �ɹ���
* @retval ����ֵ ʧ�ܣ����hi_errno.h��
*
* @par Dependency:
* @li hi_mdm_nv.h: �ýӿ��������ڵ�ͷ�ļ���
* @see �ޡ�
* @since HiMDL_V200R001C00
*/
HI_EAPI HI_U32 HI_MDM_NV_RegisterChangedNoitfyProc(HI_U16 usMinId, HI_U16 usMaxId, HI_NVM_CHANGED_NOTIFY_CALLBACK_F pfnChanged);

/**
* @ingroup  hct_nv
* @brief ˢ�¹�����NV��NV�ļ���
*
* @par ����:
* ˢ�¹�����NV��NV�ļ������ĳ���쳣����2��NV����������ͨ��NV�ļ�����NV����ʱ��ͨ���ýӿ�ˢ�µ�NV�ļ��Ĺ�����NV���ᶪʧ������NV�ᶪʧ��
* @attention 
* @li �ýӿڽ���ֻ���ڹ�װ���̵��ã���Ϊ���øýӿڻ�����NV�ļ�����д����������NV�ļ���������
* @li �ýӿڽ���ֻ���ڹ�װ���̵��ã���Ϊ���øýӿ�ˢ��(��д)NV�ļ�ʱ�쳣���磬���������NV�ļ���ʧ(����)��
* @li ������øýӿ�ʱ�쳣���磬һ��Ҫ�ٴε��øýӿڡ��ٴε��øýӿڼ�⵽NV�ļ�����ʱ����������NV�ļ���
*
* @retval #HI_ERR_SUCCESS �ɹ���
* @retval ����ֵ ʧ�ܣ����hi_errno.h��
*
* @par Dependency:
* @li hi_mdm_nv.h: �ýӿ��������ڵ�ͷ�ļ���
* @see �ޡ�
* @since HiMDL_V200R001C00
*/
HI_EAPI HI_U32 HI_MDM_NV_RefreshDataToNvFile(HI_VOID);

#endif
