/**@defgroup hct_dms ͨ��
 * @ingroup sal
 */

#ifndef __HI_MDM_DMS_H__
#define __HI_MDM_DMS_H__

#include <hi_mdm_types.h>

/**
* @ingroup  hct_dms
* @brief  ��ʼ��ͨ��ģ��MDM��APP��ؽӿڵĹ�����Ϣ��
*
* @par ����:
* ��ʼ��ͨ��ģ��MDM��APP��ؽӿڵĹ�����Ϣ��
*ֻ��ʹ�øýӿڳ�ʼ��֮��APP�ſ���ʹ��MDM�Ľӿڡ�
*�ظ�����ͨ�� HI_NDC_Init ���� pstChlPort ���ء�
* @attention �ޡ�
*
* @param pstAppObj [IN] APP����
*
* @retval #HI_ERR_SUCCESS �ɹ���
* @retval ����ֵ ʧ�ܣ����hi_errno.h��
*
* @par Dependency:
* @li hi_mdm_dms.h: �ýӿ��������ڵ�ͷ�ļ���
* @see  �ޡ�
* @since HiMDL_V200R001C00
*/
HI_EXTERN HI_U32 HI_MDM_AppInit(HI_CONST HI_MDM_APPOBJ_S* pstAppObj);

/**
* @ingroup  hct_dms
* @brief  ͨ��ע�ᡣ
*
* @par ����:
* @li ʹ��Ĭ������: HI_MDM_ChlSetup��
* @li ����Ĭ�����ò���: Change NV item(HI_NV_FTM_DEVICE_CFG_ID) --> HI_DMS_ChlRefresh --> HI_MDM_ChlSetup��
* @attention �ޡ�
*
* @param  pstChlPort [IN] ͨ���˿ڶ���
* @param  usPortNum [IN] ͨ���˿ڣ�ȡֵΪ:
* @li HI_DMS_CHL_MAC_PORT_APP      : ��ʾMACͨ����
* @li HI_DMS_CHL_MAC_PORT_DIAG    : ��ʾMACͨ����
* @li HI_DMS_CHL_UART_PORT_APP    : ��ʾUART��MRͨ����
* @li HI_DMS_CHL_UART_PORT_IR      : ��ʾIRͨ����
* @li HI_DMS_CHL_TCP_PORT_APP      : ��ʾTCP��MRͨ����
* @li HI_DMS_CHL_TCP_PORT_DIAG    : ��ʾTCP��DIAGͨ����
* @li HI_DMS_CHL_UART_PORT_DIAG  : ��ʾUART��DIAGͨ����
*
* @retval #HI_ERR_SUCCESS �ɹ���
* @retval ����ֵ ʧ�ܣ����hi_errno.h��
*
* @par Dependency:
* @li hi_mdm_dms.h: �ýӿ��������ڵ�ͷ�ļ���
* @see  �ޡ�
* @since HiMDL_V200R001C00
*/
HI_EXTERN HI_U32 HI_MDM_ChlSetup(HI_CONST HI_DMS_CHL_PORT_OBJ_S* pstChlPort, HI_U16 usPortNum);

/**
* @ingroup  hct_dms
* @brief  ����ע���ͨ�����в�ѯ�����á�
*
* @par ����:
* �ú���ͨ��ָ���˿ڣ��Ը�ͨ�����в�ѯ�����á�
* @attention ѡ��HI_DMS_PORT_CTRL_UART_CLOSE���ܺ�����ӹܴ������������ݣ���ʹ��ǰ�����Ƚ���������Ϊ��������ģʽ��\n
* �ٸ�����Ҫ����Ϊ����ģʽ�����ӹܴ��ڵ��������ȼ���ԭ��ʹ�øô��ڵ��������ȼ��ߣ�����Ҫ������Ϊ����ģʽ��˯��һ�Ρ�������Ҫ˯�ߡ�\n
* �ӹ�ʾ������:\n
* HI_U32 read_no_block = UART_RD_NONBLOCK;//��������ģʽ\n
* HI_U32 read_block = UART_RD_BLOCK;      //������ģʽ\n
* HI_U32 fd;\n
* ret = HI_MDM_ChlCtrl(HI_DMS_CHL_UART_PORT_APP, HI_DMS_PORT_CTRL_UART_CLOSE, HI_NULL, HI_NULL);\n
* if(ret != HI_ERR_SUCCESS)\n
* {\n
*   return HI_ERR_FAIL;\n
* }\n
* fd = HI_MDM_UART_Open(HI_UART_0, HI_UART_232);\n
* ret = HI_MDM_UART_Ioctl(fd, UART_CFG_RD_BLOCK, &read_no_block);\n
* if(ret != HI_ERR_SUCCESS)\n
* {\n
*   return HI_ERR_FAIL;\n
* }      \n
* HI_MDM_Sleep(10);\n
* ret = HI_MDM_UART_Ioctl(fd, UART_CFG_RD_BLOCK, &read_block);\n
* if(ret != HI_ERR_SUCCESS)\n
* .\n
* .\n
* .\n
* @param  usPortNum [IN] ͨ���˿ڣ���֧��#HI_DMS_CHL_MAC_PORT_DIAG
* @param  enOpt [IN] ����ѡ�
* @param  pInputParam [IN] ָ���������������ʹ��ʱ����HI_NULL��
* @param  pOutputParam [OUT] ָ���������������ʹ��ʱ����HI_NULL��
*
* @retval #HI_ERR_SUCCESS �ɹ���
* @retval ����ֵ ʧ�ܣ����hi_errno.h��
*
* @par Dependency:
* @li hi_mdm_dms.h: �ýӿ��������ڵ�ͷ�ļ���
* @see  �ޡ�
* @since HiMDL_V200R001C00
*/
HI_EXTERN HI_U32 HI_MDM_ChlCtrl(HI_U16 usPortNum, HI_DMS_PORT_CTRL_OPT_ID_E enOpt, HI_IN HI_PVOID pInputParam, HI_OUT HI_PVOID pOutputParam);

/**
* @ingroup  hct_dms
* @brief  ͨ����ע���ͨ������Ӧ�ñ��ġ�
*
* @par ����:
* �ú���ͨ��ָ���˿ڽ��з��͡�
* @attention 
* 1����д����뿴��νṹ��˵��
*
* @param  usPortNum [IN] ͨ���˿�  ��
* @param  pFrameData [IN] DMSͨ����֡���ݣ����� #HI_DMS_CHL_TX_DATA_S��
* @param  pFrameCtrl [IN] DMS��֡������Ϣ������ #HI_DMS_CHL_TX_CTRL_S��
*
* @retval #HI_ERR_SUCCESS �ɹ���
* @retval ����ֵ ʧ�ܣ����hi_errno.h��
*
* @par Dependency:
* @li hi_mdm_dms.h: �ýӿ��������ڵ�ͷ�ļ���
* @see  �ޡ�
* @since HiMDL_V200R001C00
*/
HI_EXTERN HI_U32 HI_MDM_ChlTx(HI_U16 usPortNum, HI_IN HI_DMS_CHL_TX_DATA_S* pFrameData, HI_IN HI_DMS_CHL_TX_CTRL_S* pFrameCtrl);

/**
* @ingroup  hct_dms
* @brief  ͨ���ýӿڿ����޸Ĵ��ڳ�ʼ�����á�
*
* @par ����:
* ͨ���ýӿڿ����޸Ĵ��ڳ�ʼ�����á�
* @attention ���øýӿں�Ҫ���µ���HI_MDM_ChlSetup�ӿ�һ�Σ����òŻ���Ч��
*
* @param  pstDevCfg [IN] Ŀ������
* @param  chipType [IN]  оƬ���͡�
* @param  usportNum [IN] �˿ںš�ȡֵ��μ�#HI_MDM_ChlSetup�ӿ��жԶ˿ڵ�������
*
* @retval #HI_ERR_SUCCESS �ɹ���
* @retval ����ֵ ʧ�ܣ����hi_errno.h��
*
* @par Dependency:
* @li hi_mdm_dms.h: �ýӿ��������ڵ�ͷ�ļ���
* @see  �ޡ�
* @since HiMDL_V200R001C00
*/
HI_EXTERN HI_U32 HI_MDM_ChlUartPortRefresh(HI_FTM_DEVICE_UART_CFG_S* pstDevCfg, HI_FTM_CHIP_TYPE_E chipType, HI_U16 usportNum);

/**
* @ingroup  hct_dms
* @brief  �ú���������APP��Ϣ��ָ����ͨ��ģ�飨MDM�㣩��
*
* @par ����:
* �ú���������APP��Ϣ��ָ����ͨ��ģ�飨MDM�㣩��
* @attention �ޡ�
*
* @param  ulMsgId [IN] ��ϢID, ��MDM���ṩ  ��
* @param  pucMsgPayload [IN] ��Ϣ�û����ݡ�
* @param  ulMsgPayloadSize [IN] ��Ϣ�û����ݳ���, ��λ�ֽڡ�
* @param  pstCtrl [IN] ��Ϣ���ӿ�����Ϣ���μ�HI_MDM_MSG_CTRL_S���塣
*
* @retval #HI_ERR_SUCCESS �ɹ���
* @retval ����ֵ ʧ�ܣ����hi_errno.h��
*
* @par Dependency:
* @li hi_mdm_dms.h: �ýӿ��������ڵ�ͷ�ļ���
* @see  �ޡ�
* @since HiMDL_V200R001C00
*/
HI_EXTERN HI_U32 HI_MDM_MsgSend(HI_U32 ulMsgId, HI_PBYTE pucMsgPayload, HI_U32 ulMsgPayloadSize, HI_MDM_MSG_CTRL_S* pstCtrl);

/**
* @ingroup  hct_dms
* @brief ����ϢID��ӦPLCͨ������״̬�ϱ�������
*
* @par   ����:
* �ú���ʵ��MDM�������ϱ�PLCͨ��������״̬��APP�㡣
* @attention �ޡ�
*
* @param [OUT]���� #HI_MDM_CONNECT_STATUS_IND_S  ��Ϣ������
*
* @retval   �ޡ�
*
* @par Dependency:
* @li hi_mdm_dms.h: �ýӿ��������ڵ�ͷ�ļ���
* @see  �ޡ�
* @since HiMDL_V200R001C00
*/
#define HI_MDM_CONNECT_STATUS_IND_ID        0x3600
/**
* @ingroup  hct_dms
* @brief ����ϢID��ӦAPP֮�����ݴ��亯����
*
* @par   ����:
* ����ʵ��APP��֮��ͨ��MDM���ṩ��ͨ���������ݴ��䡣
* @attention �ޡ�
*
* @param [IN]���� #HI_MDM_APPDATA_TRANSMIT_DL_S  ��Ϣ���в�����
* @param [OUT]���� #HI_MDM_APPDATA_TRANSMIT_UL_S  ��Ϣ���в�����
*
* @retval   �ޡ�
* @par Dependency:
* @li hi_mdm_dms.h: �ýӿ��������ڵ�ͷ�ļ���
* @see  �ޡ�
* @since HiMDL_V200R001C00
*/
#define HI_MDM_APPDATA_TRANSMIT_ID             0x3601
#endif
