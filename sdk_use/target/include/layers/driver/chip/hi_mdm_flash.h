
#ifndef __HI_MDM_FLASH__
#define __HI_MDM_FLASH__

HI_START_HEADER
#if !defined(PRODUCT_CFG_BUILD_HSO_CODE)

/**@defgroup hct_FLASH FLASH
 * @ingroup mdm
*/


/**
* @ingroup  hct_FLASH
* @brief  ����Flash���ݵ�ָ����������
*
* @par ����:
* ����Flash���ݵ�ָ����������
* @attention �ޡ�
*
* @param ulAddress [IN] ָ����Flash��ַƫ�ơ�
* @param ulSize [IN] ָ����ȡ�ĳ���, �ֽڡ�
* @param pReadSize [OUT] ʵ�ʶ�ȡ���ĳ��ȡ�ȡֵΪHI_NULL��ʾ����Ҫ����ʵ�ʶ�ȡ���ȡ�
* @param pBuffer [OUT] Ŀ�Ļ����ַ��
*
* @retval #HI_ERR_SUCCESS ���ɹ���
* @retval #����ֵ ��ʧ�ܣ����hi_errno.h��
*
* @par Dependency:
* @li hi_mdm_flash.h: �ýӿ��������ڵ�ͷ�ļ���
* @since HiMDL_V200R001C00
*/

HI_EAPI HI_U32 HI_MDM_FLASH_Read(HI_CONST HI_U32 ulAddress, HI_CONST HI_U32 ulSize, HI_U32 *pReadSize, HI_U8 *pBuffer);

/**
* @ingroup  hct_FLASH
* @brief  ������д��ָ����Flash��
*
* @par ����:
* ������д��ָ����Flash����,����ʹ�õĵ�ַ�ռ�Ϊ0x200000֮��ĵ�ַ�ռ䡣
* @attention
* @li Flash��Ե�ַ�����Ʊ�����
* @li Flash��д�������ơ�
* @li �����û�ʵ�ʿ��Ƴ��������Ƿ��Ȳ���д��
*
* @param ulAddress [IN] ָ��д��Flash��ַƫ�ơ�
* @param ulSize [IN] ��Ҫд��ĳ��ȡ�
* @param pWrittenSize [OUT] ʵ��д��ĳ��ȡ�ȡֵΪHI_NULL��ʾ����Ҫ����ʵ��д���ȡ�
* @param pBuffer [IN] ��Ҫд������ݵĻ����ַ��
* @param bAutoErase [IN] �Ƿ��Զ�����������д�롣
*
* @retval #HI_ERR_SUCCESS д�ɹ���
* @retval #����ֵ дʧ��.���hi_errno.h��
*
* @par Dependency:
* @li hi_mdm_flash.h: �ýӿ��������ڵ�ͷ�ļ���
* @since HiMDL_V200R001C00
*/

HI_EAPI HI_U32 HI_MDM_FLASH_Write(HI_CONST HI_U32 ulAddress, HI_U32 ulSize, HI_U32 *pWrittenSize, HI_CONST HI_U8 *pBuffer, HI_BOOL bAutoErase);

/**
* @ingroup  hct_FLASH
* @brief  ��ָ����Flash�������ݲ�����
*
* @par ����:
* ��ָ����Flash�������ݲ�����
* @attention Flash��Ե�ַ�����Ʊ�����Flash��д�������ơ�
*
* @param ulAddress [IN] ָ��Ҫ����Flash�ĵ�ַƫ�ơ���ַ��4K���롣
* @param ulSize [IN] ��Ҫ�����ĳ���(��λbyte)������4K������
* @param pEraseSize [OUT] ʵ�ʲ������ݴ�С��
*
* @retval #HI_ERR_SUCCESS �����ɹ���
* @retval #����ֵ ����ʧ��.���hi_errno.h��
*
* @par Dependency:
* @li hi_mdm_flash.h: �ýӿ��������ڵ�ͷ�ļ���
* @since HiMDL_V200R001C00
*/

HI_EAPI HI_U32 HI_MDM_FLASH_Erase(HI_CONST HI_U32 ulAddress, HI_CONST HI_U32 ulSize, HI_U32 *pEraseSize);

//****************************************************************************
// ��������: HI_MDM_FLASH_WriteSafe
// ��������: ������д��ָ����Flash����(�Բ�����ַ���м��),����ʹ�õĵ�ַ�ռ�Ϊ0x200000֮��ĵ�ַ�ռ䡣
//
// ����˵��:
//   ulAddress [in] : ָ��д��Flash��ַƫ��
//   ulSize [in] : ��Ҫд��ĳ���
//   pWrittensize [out] : ʵ��д��ĳ���
//   pBuffer [in] : ��Ҫд������ݵĻ����ַ
//   bAutoErase [in] : HI_TRUE: �Զ�����������д��.
//
// �� �� ֵ:
//    HI_ERR_SUCCESS : �ɹ�
//    ����ֵΪ������
// ����Ҫ��:
//    ע��Flash��Ե�ַ�����Ʊ�����
//    ע��Flash��д��������;
//    �����û�ʵ�ʿ��Ƴ��������Ƿ��Ȳ���д;
//****************************************************************************
HI_EAPI HI_U32 HI_MDM_FLASH_WriteSafe(HI_CONST HI_U32 ulAddress, HI_U32 ulSize, HI_U32 *pWrittenSize, HI_CONST HI_U8*pBuffer, HI_BOOL bAutoErase);

//****************************************************************************
// ��������: HI_MDM_FLASH_EraseSafe
// ��������: ��ָ����Flash�������ݲ���(�Բ�����ַ���м��),����ʹ�õĵ�ַ�ռ�Ϊ0x200000֮��ĵ�ַ�ռ䡣
//
// ����˵��:
//   ulAddress [in] : ָ��д��Flash��ַƫ��
//   ulSize [in] : ��Ҫ�����ĳ���(��λbyte)������4K����
//   pEraseSize [out] : ʵ�ʲ������ݴ�С
//
// �� �� ֵ:
//    HI_ERR_SUCCESS : �ɹ�
//    ����ֵΪ������
// ����Ҫ��:
//    ע��Flash��Ե�ַ�����Ʊ�����
//    ע��Flash��д��������;
//****************************************************************************
HI_EAPI HI_U32 HI_MDM_FLASH_EraseSafe(HI_CONST HI_U32 ulAddress, HI_CONST HI_U32 ulSize, HI_U32 *pEraseSize);

//*****************************************************************************
// ��������: HI_MDM_FLASH_Init
// ��������: ��ʼ��flash�豸
//
// ����˵��:
//           ��
//
// �� �� ֵ:
//           �ɹ�����HI_ERR_SUCCESS��
//           ʧ�ܷ���HI_ERR_FAILURE��
//
// ����Ҫ��:
//
// ���þ���: TODO: ...
// ��    ��:
//*****************************************************************************
#define HI_MDM_FLASH_Init() HI_FLASH_Init()

//*****************************************************************************
// ��������: HI_MDM_FLASH_DeInit
// ��������: ȥ��ʼ��flash�豸
//
// ����˵��:
//           ��
//
// �� �� ֵ:
//           �ɹ�����HI_ERR_SUCCESS��
//           ʧ�ܷ���HI_ERR_FAILURE��
//
// ����Ҫ��:
//
// ���þ���: TODO: ...
// ��    ��:
//*****************************************************************************
#define HI_MDM_FLASH_DeInit() HI_FLASH_DeInit()

/**
* @ingroup  hct_FLASH
* @brief  Flash������ؽӿڡ�
*
* @par ����:
* Flash������ؽӿڣ�Ŀǰ��ʵ�ֻ�ȡFlash��Ϣ�Ĺ��ܡ�
* @attention
* @li Flash��Ե�ַ�����Ʊ�����
* @li Flash��д�������ơ�
*
* @param  usCommand [IN] ���� #HI_U16 Ҫִ�е����ȡֵ#HI_GET_FLASH_INFOʱ��Ϊ��ȡFLASH��Ϣ��
* @param  pstFlashInfo [OUT] ���� #HI_FLASH_INFO_S *  ��ȡ����FALSH��Ϣ��
*
* @retval #HI_ERR_SUCCESS �ɹ���
* @retval #����ֵ ʧ��.���hi_errno.h��
*
* @par Dependency:
* @li hi_mdm_flash.h: �ýӿ��������ڵ�ͷ�ļ���
* @since HiMDL_V200R001C00
*/

#define HI_MDM_FLASH_Ioctl(usCommand, pstFlashInfo) HI_FLASH_Ioctl(usCommand, pstFlashInfo)

#endif

HI_END_HEADER
#endif