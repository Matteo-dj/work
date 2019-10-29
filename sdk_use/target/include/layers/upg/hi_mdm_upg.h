/******************************************************************************
*             Copyright (C) 2018 - 2018, Hisilicon Tech. Co., Ltd.
*                           ALL RIGHTS RESERVED
* FileName: hi_mdm_upg.h
* Description�������ӿڣ�����ʵ���������ܡ�
*
* History:
* Version              Date         DefectNum           Description
* V200R001C00SPC010    2018-03-31   NA                  ��һ�ΰ汾������
* V200R001C00SPC011    2018-05-10   DTS2018010310207    ����2�������ӿڣ���ȡ���������ӿ�(HI_MDM_UPG_GetCfg)���������������ӿ�(HI_MDM_UPG_SetCfg)��
* V200R001C00SPC012    2018-07-12   DTS2018071204569    ɾ���ӿ�HI_MDM_UPG_SetProtoInfo,�����ӿ�HI_MDM_UPG_SetPrivateProto��
* V200R001C00SPC012    2018-07-12   DTS2018071204569    ����Э��ע����ؽӿڡ�
* V200R001C00SPC012    2018-07-12   DTS2018071204569    ������ϢID:HI_MDM_UPG_TRANS_FILE��HI_MDM_UPG_QUERY_INFO��
* V200R001C00SPC050    2019-01-21   DTS2018071204569    �޸����ú�˼Э��ӿڼ�����ʱ϶�ӿڡ�
******************************************************************************/
/**@defgroup hct_upg �����ӿ�
 * @ingroup mdm
 */

#ifndef __HI_MDM_UPG_H__
#define __HI_MDM_UPG_H__

HI_START_HEADER

/**
 *@ingroup hct_upg
 * ��˼�����ļ�������Сֵ��ʵ�ʺ�˼�����ļ����Ȳ�С�ڸ�ֵ���ɹ�APPУ�������ļ��á�
 */
#define HI_MDM_UPG_FILE_MIN_SIZE                       HI_UPG_FILE_HEADER_SIZE
/**
 *@ingroup hct_upg
 * ȫ�������ļ�ͷ���ȡ�
 */
#define HI_MDM_UPG_CCO_FILE_BUFFER_RESERVED_SIZE       HI_MDM_UPG_FILE_MIN_SIZE
/**
 *@ingroup hct_upg
 * ��ȫ�������������ļ��������ֵ����ȫ�������������ļ����Ȳ��ܴ��ڸ�ֵ�������޷������������̡�
 */
#define HI_MDM_UPG_FILE_MAX_SIZE                       PRODUCT_CFG_UPG_FILE_MAX_SIZE
/**
 *@ingroup hct_upg
 * CCO�����ļ��������ֵ��CCO��������ʱ�������ļ����Ȳ��ܴ��ڸ�ֵ�������޷������������̡�
 */
#define HI_MDM_UPG_CCO_FILE_MAX_SIZE                   PRODUCT_CFG_CCO_UPG_FILE_MAX_SIZE
/**
 *@ingroup hct_upg
 * ��˼STA�����ļ��������ֵ��STA����������ȫ��STA�������б��������б���ֻ����STA�����豸����ʱ�������ļ����Ȳ��ܴ��ڸ�ֵ�������޷������������̡�
 */
#define HI_MDM_UPG_STA_FILE_MAX_SIZE                   PRODUCT_CFG_STA_UPG_FILE_MAX_SIZE

/**
 *@ingroup hct_upg
 * �Ǻ�˼STA�����ļ��������ֵ(��С�ں�˼STA�����ļ��������ֵ)�������ļ����Ȳ��ܴ��ڸ�ֵ�������޷������������̡�
 */
#define HI_MDM_UPG_STA_FILE_RAM_MAX_SIZE               PRODUCT_CFG_UPGFILE_CACHE_CCO_RAM_SIZE

/**
 *@ingroup hct_upg
 * ȫ�������ļ��������ֵ��ȫ���������б��������б��а���CCO��STA��ʱ�������ļ����Ȳ��ܴ��ڸ�ֵ�������޷������������̡�
 */
#define HI_MDM_UPG_CCO_STA_FILE_MAX_SIZE               (HI_MDM_UPG_CCO_FILE_MAX_SIZE+HI_MDM_UPG_STA_FILE_MAX_SIZE+HI_MDM_UPG_CCO_FILE_BUFFER_RESERVED_SIZE)
 /**
 *@ingroup hct_upg
 * ����״̬��ѯ�����������ʾ�ϱ�����������Ϣ��\n
 * @li ��������ʱ����ѯ����״̬���������˽⵱ǰ������״̬�������˽�������Ϣ�����磺ͨ��ģ����������ļ����ȡ������ļ��汾�š�ģ��������ʱ�ȣ��������óɸò�����
 * @li ȫ������ʱ����ѯ����״̬���������˽⵱ǰ������״̬�������˽�����������ģ������������Ϣ��������ģ������ͳ����Ϣ�������óɴ˲����� 
 */
#define HI_MDM_UPG_STATUS_REQ_OPT_NA      0
 /**
 *@ingroup hct_upg
 * ����״̬��ѯ�����������ʾ�ϱ�����������Ϣ����ѯ����״̬ʱ��ֻ���˽⵱ǰ������״̬�������óɸò�����
 */
#define HI_MDM_UPG_STATUS_REQ_OPT_BS      1
 /**
 *@ingroup hct_upg
 * �����ɹ�״̬��
 */
#define HI_MDM_UPG_STATUS_OK              0
/**
 *@ingroup hct_upg
 * ������֤״̬��
 */
#define HI_MDM_UPG_STATUS_END             1
/**
 *@ingroup hct_upg
 * δ֪����״̬����ǰ���������������С�
 */
#define HI_MDM_UPG_STATUS_UNKNOWN         8
/**
 *@ingroup hct_upg
 * ȫ������ֹͣ����״̬��
 */
#define HI_MDM_UPG_STATUS_STOPPING        16
/**
 *@ingroup hct_upg
 * ���ڽ����ļ�����״̬��
 */
#define HI_MDM_UPG_STATUS_PROCESS        0xF0
/**
 *@ingroup hct_upg
 * ����ת���ļ�״̬��
 */
#define HI_MDM_UPG_STATUS_TRANSMIT       0xF1
/**
 *@ingroup hct_upg
 * ����ʧ��״̬��
 */
#define HI_MDM_UPG_STATUS_FAIL           0xFF

/**
* @ingroup  hct_upg
* @brief  ����ϢIDʵ������״̬��ѯ��
*
* @par   ����:
            1)�ȵ���HI_MDM_RegisterUpgMsgNotifyע�����ϢID�Ļص�������\n
			2)ͨ��HI_MDM_UPG_MsgSend���͸���Ϣ��UPG��\n
			3)UPG�����������û�ע��Ļص������ϱ���������
* @attention	�ޡ�
* @param   ulMsgId [IN]  ����״̬��ѯ��ϢID��ȡֵ��HI_MDM_UPG_STATUS��
* @param   pucPacket [IN]  ����״̬��ѯ�������(HI_MDM_UPG_STATUS_REQ_S): \n
* @li ��stReqStatus.ulOptionȡֵΪHI_MDM_UPG_STATUS_REQ_OPT_NA��ȫ�������ظ���ϢIDΪHI_MDM_UPG_STATUS��HI_MDM_UPG_STAT_INFO_IND��0x36C0����
						   �����ֱ���ΪHI_MDM_UPG_STATUS_IND_S��HI_MDM_UPG_STAT_INFO_IND_S ��
* @li ��stReqStatus.ulOptionȡֵΪHI_MDM_UPG_STATUS_REQ_OPT_NA�����������ظ���ϢIDΪHI_MDM_UPG_STATUS������ΪHI_MDM_UPG_STATUS_IND_S ��
* @li ��stReqStatus.ulOptionȡֵΪHI_MDM_UPG_STATUS_REQ_OPT_BS: �����ظ���ϢIDΪHI_MDM_UPG_STATUS_BS_IND��0x36C1������������ΪHI_MDM_UPG_STATUS_BS_IND_S��
* @param   ulPacketSize [IN]  ����״̬��ѯ��������ṹ���С��
* @retval  ulMsgId ����״̬��ѯ��ϢID��ȡֵ��Χ��HI_MDM_UPG_STATUS��HI_MDM_UPG_STAT_INFO_IND��HI_MDM_UPG_STATUS_BS_IND��
* @retval  pucPacket ����״̬��ѯӦ��������ֱ��ӦulMsgId�Ľṹ��Ϊ��HI_MDM_UPG_STATUS_IND_S��HI_MDM_UPG_STAT_INFO_IND_S��HI_MDM_UPG_STATUS_BS_IND_S��
* @retval  usPacketSize ����״̬��ѯӦ������ṹ���С��
* @par Dependency:
* <ul><li>hi_mdm_upg.h: �ļ�����������������ӿڡ�</li></ul>
* @see HI_MDM_RegisterUpgMsgNotify | HI_MDM_UPG_MsgSend��
* @since HiMDL_V200R001C00
*/
#define HI_MDM_UPG_STATUS                   0x3660

/**
* @ingroup  hct_upg
* @brief  ����ϢID����APPת���ļ���������ҵ���ĸ�UPG��
*
* @par   ����:
			ͨ��HI_MDM_UPG_MsgSend���͸���Ϣ��UPG��\n
* @attention	�ޡ�
* @param   ulMsgId [IN]  ת������������ϢID��ȡֵ��HI_MDM_UPG_TRANS_FILE��
* @param   pucPacket [IN]  ת�������������ݡ�
* @param   ulPacketSize [IN]  ת���������Ĵ�С��
* @par Dependency:
* <ul><li>hi_mdm_upg.h: �ļ�����������������ӿڡ�</li></ul>
* @see HI_MDM_UPG_MsgSend��
* @since HiMDL_V200R001C00SPC012
*/
#define HI_MDM_UPG_TRANS_FILE               0x3661

/**
* @ingroup  hct_upg
* @brief  ����ϢIDʵ������������
*
* @par   ����:
            1)�ȵ���HI_MDM_RegisterUpgMsgNotifyע�����ϢID�Ļص�������\n
			2)ͨ��HI_MDM_UPG_MsgSend���͸���Ϣ��UPG��\n
			3)UPG�����������û�ע��Ļص������ϱ���������
* @attention	�ޡ�
* @param   ulMsgId [IN]  ������ʼ��ϢID��ȡֵ��HI_MDM_UPG_START��
* @param   pucPacket [IN]  ������ʼ�������(HI_MDM_UPG_START_REQ_S)��
* @param   ulPacketSize [IN]  ������ʼ��������ṹ���С��
* @retval  ulMsgId ������ʼ��ϢID��ȡֵ��HI_MDM_UPG_START��
* @retval  pucPacket ������ʼӦ���������Ӧ�Ľṹ��ΪHI_MDM_UPG_START_IND_S��
* @retval  usPacketSize ������ʼӦ������ṹ���С��
* @par Dependency:
* <ul><li>hi_mdm_upg.h: �ļ�����������������ӿڡ�</li></ul>
* @see HI_MDM_RegisterUpgMsgNotify | HI_MDM_UPG_MsgSend��
* @since HiMDL_V200R001C00
*/
#define HI_MDM_UPG_START                    0x3662

/**
* @ingroup  hct_upg
* @brief  ����ϢIDʵ��ֹͣ������
*
* @par   ����:
            1)�ȵ���HI_MDM_RegisterUpgMsgNotifyע�����ϢID�Ļص�������\n
			2)ͨ��HI_MDM_UPG_MsgSend���͸���Ϣ��UPG��\n
			3)UPG�����������û�ע��Ļص������ϱ���������
* @attention	�ޡ�
* @param   ulMsgId [IN]  ����ֹͣ��ϢID��ȡֵ��HI_MDM_UPG_STOP��
* @param   pucPacket [IN]  ����ֹͣ�������(HI_MDM_UPG_STOP_REQ_S)��
* @param   ulPacketSize [IN]  ����ֹͣ��������ṹ���С��
* @retval  ulMsgId ����ֹͣ��ϢID��ȡֵ��HI_MDM_UPG_STOP��
* @retval  pucPacket ����ֹͣӦ���������Ӧ�Ľṹ��ΪHI_MDM_UPG_STOP_IND_S��
* @retval  usPacketSize ����ֹͣӦ������ṹ���С��
* @par Dependency:
* <ul><li>hi_mdm_upg.h: �ļ�����������������ӿڡ�</li></ul>
* @see HI_MDM_RegisterUpgMsgNotify | HI_MDM_UPG_MsgSend��
* @since HiMDL_V200R001C00
*/
#define HI_MDM_UPG_STOP                     0x3663

/**
* @ingroup  hct_upg
* @brief  ����ϢID����APPת���ӽڵ���Ϣ��ѯ����ҵ���ĸ�UPG��
*
* @par   ����:
            ͨ��HI_MDM_UPG_MsgSend���͸���Ϣ��UPG��
* @attention    �ޡ�
* @param   ulMsgId [IN]  ת���ӽڵ���Ϣ��ѯ��ϢID��ȡֵ��HI_MDM_UPG_QUERY_INFO��
* @param   pucPacket [IN]  ת�������������ݡ�
* @param   ulPacketSize [IN]  ת���������Ĵ�С��
* @par Dependency:
* <ul><li>hi_mdm_upg.h: �ļ�����������������ӿڡ�</li></ul>
* @see HI_MDM_UPG_MsgSend��
* @since HiMDL_V200R001C00SPC012
*/
#define HI_MDM_UPG_QUERY_INFO               0x3666

/**
* @ingroup  hct_upg
* @brief  ����ϢIDʵ������״̬ͳ����ϢӦ��
*
* @par   ����:
            1)�ȵ���HI_MDM_RegisterUpgMsgNotifyע�����ϢID�Ļص�������\n
			2)ͨ��HI_MDM_UPG_MsgSend���͸���Ϣ��UPG��\n
			3)UPG�����������û�ע��Ļص������ϱ���������
* @attention	�ޡ�
* @param   ulMsgId [IN]  ����״̬��ѯ��ϢID��ȡֵ��HI_MDM_UPG_STATUS��
* @param   pucPacket [IN]  ����״̬��ѯ�������(HI_MDM_UPG_STATUS_REQ_S):
						   ��stReqStatus.ulOptionȡֵΪHI_MDM_UPG_STATUS_REQ_OPT_NA��ȫ������״̬ͳ����Ϣ�ظ���ϢΪHI_MDM_UPG_STAT_INFO_IND��
						   �����ֱ���ΪHI_MDM_UPG_STAT_INFO_IND_S ��
* @param   ulPacketSize [IN]  ����״̬��ѯ��������ṹ���С��
* @retval  ulMsgId ����״̬ͳ����Ϣ��ϢID��ȡֵ��HI_MDM_UPG_STAT_INFO_IND��
* @retval  pucPacket ����״̬ͳ����ϢӦ���������Ӧ�Ľṹ��ΪHI_MDM_UPG_STAT_INFO_IND_S��
* @retval  usPacketSize ����״̬ͳ����ϢӦ������ṹ���С��
* @par Dependency:
* <ul><li>hi_mdm_upg.h: �ļ�����������������ӿڡ�</li></ul>
* @see HI_MDM_RegisterUpgMsgNotify | HI_MDM_UPG_MsgSend��
* @since HiMDL_V200R001C00
*/
#define HI_MDM_UPG_STAT_INFO_IND            0x36C0

/**
* @ingroup  hct_upg
* @brief  ����ϢIDʵ������״̬������ϢӦ��
*
* @par   ����:
            1)�ȵ���HI_MDM_RegisterUpgMsgNotifyע�����ϢID�Ļص�������\n
			2)ͨ��HI_MDM_UPG_MsgSend���͸���Ϣ��UPG��\n
			3)UPG�����������û�ע��Ļص������ϱ���������
* @attention	�ޡ�
* @param   ulMsgId [IN]  ����״̬��ѯ��ϢID��ȡֵ��HI_MDM_UPG_STATUS��
* @param   pucPacket [IN]  ����״̬��ѯ�������(HI_MDM_UPG_STATUS_REQ_S):
						   ��stReqStatus.ulOptionȡֵΪHI_MDM_UPG_STATUS_REQ_OPT_BS: �����ظ���ϢΪHI_MDM_UPG_STATUS_BS_IND��0x36C1������������ΪHI_MDM_UPG_STATUS_BS_IND_S��
* @param   ulPacketSize [IN]  ����״̬��ѯ��������ṹ���С��
* @retval  ulMsgId ����״̬������Ϣ��ϢID��ȡֵ��Χ��HI_MDM_UPG_STATUS_BS_IND��
* @retval  pucPacket ����״̬������ϢӦ���������ӦulMsgId�Ľṹ��Ϊ��HI_MDM_UPG_STATUS_BS_IND_S��
* @retval  usPacketSize ����״̬������ϢӦ������ṹ���С��
* @par Dependency:
* <ul><li>hi_mdm_upg.h: �ļ�����������������ӿڡ�</li></ul>
* @see HI_MDM_RegisterUpgMsgNotify | HI_MDM_UPG_MsgSend��
* @since HiMDL_V200R001C00
*/
#define HI_MDM_UPG_STATUS_BS_IND            0x36C1

/**
* @ingroup  hct_upg
* @brief  ����ϢIDʵ�������豸����
*
* @par   ����:
            1)�ȵ���HI_MDM_RegisterUpgMsgNotifyע�����ϢID�Ļص�������\n
			2)ͨ��HI_MDM_UPG_MsgSend���͸���Ϣ��UPG��\n
			3)UPG�����������û�ע��Ļص������ϱ���������
* @attention	�ޡ�
* @param   ulMsgId [IN]  �豸������ʼ��ϢID��ȡֵ��HI_MDM_DEVICE_UPG_START��
* @param   pucPacket [IN]  �豸������ʼ�������(HI_MDM_DEVICE_UPG_START_REQ_S)��
* @param   ulPacketSize [IN]  �豸������ʼ��������Խṹ���С��
* @retval  ulMsgId �豸������ʼ��ϢID��ȡֵ��HI_MDM_DEVICE_UPG_START��
* @retval  pucPacket �豸������ʼӦ���������Ӧ�Ľṹ��ΪHI_MDM_DEVICE_UPG_START_IND_S��
* @retval  usPacketSize �豸������ʼӦ������ṹ���С��
* @par Dependency:
* <ul><li>hi_mdm_upg.h: �ļ�����������������ӿڡ�</li></ul>
* @see HI_MDM_RegisterUpgMsgNotify | HI_MDM_UPG_MsgSend��
* @since HiMDL_V200R001C00
*/
#define HI_MDM_DEVICE_UPG_START             0x36D0

/**
* @ingroup  hct_upg
* @brief  ����ϢIDʵ��ֹͣ�豸�������ù����ݲ�֧�֣���
*
* @par   ����:
            1)�ȵ���HI_MDM_RegisterUpgMsgNotifyע�����ϢID�Ļص�������\n
			2)ͨ��HI_MDM_UPG_MsgSend���͸���Ϣ��UPG��\n
			3)UPG�����������û�ע��Ļص������ϱ���������
* @attention	�ޡ�
* @param   ulMsgId [IN]  �豸����ֹͣ��ϢID��ȡֵ��HI_MDM_DEVICE_UPG_STOP��
* @param   pucPacket [IN]  �豸����ֹͣ�������(HI_MDM_DEVICE_UPG_STOP_REQ_S)��
* @param   ulPacketSize [IN]  �豸����ֹͣ��������ṹ���С��
* @retval  ulMsgId �豸����ֹͣ��ϢID��ȡֵ��HI_MDM_DEVICE_UPG_STOP��
* @retval  pucPacket �豸����ֹͣӦ���������Ӧ�Ľṹ��ΪHI_MDM_DEVICE_UPG_STOP_IND_S��
* @retval  usPacketSize �豸����ֹͣӦ������ṹ���С��
* @par Dependency:
* <ul><li>hi_mdm_upg.h: �ļ�����������������ӿڡ�</li></ul>
* @see HI_MDM_RegisterUpgMsgNotify | HI_MDM_UPG_MsgSend��
* @since HiMDL_V200R001C00
*/
#define HI_MDM_DEVICE_UPG_STOP              0x36D1

/**
* @ingroup  hct_upg
* @brief  �ú���ʵ�ֲ�ѯ�豸����״̬��
*
* @par   ����:
            1)�ȵ���HI_MDM_RegisterUpgMsgNotifyע�����ϢID�Ļص�������\n
			2)ͨ��HI_MDM_UPG_MsgSend���͸���Ϣ��UPG��\n
			3)UPG�����������û�ע��Ļص������ϱ���������
* @attention	��
* @param   ulMsgId [IN]  �豸����״̬��ѯ��ϢID��ȡֵ��HI_MDM_DEVICE_UPG_STATUS��
* @param   pucPacket [IN]  �豸����״̬��ѯ�������(HI_MDM_DEVICE_UPG_STATUS_REQ_S)��
* @param   ulPacketSize [IN]  �豸����״̬��ѯ��������ṹ���С��
* @retval  ulMsgId �豸����״̬��ѯ��ϢID��ȡֵ��HI_MDM_DEVICE_UPG_STATUS��
* @retval  pucPacket �豸����״̬��ѯӦ���������Ӧ�Ľṹ��ΪHI_MDM_DEVICE_UPG_STATUS_IND_S��
* @retval  usPacketSize �豸����״̬��ѯӦ������ṹ���С��
* @par Dependency:
* <ul><li>hi_mdm_upg.h: �ļ�����������������ӿڡ�</li></ul>
* @see HI_MDM_RegisterUpgMsgNotify | HI_MDM_UPG_MsgSend��
* @since HiMDL_V200R001C00
*/
#define HI_MDM_DEVICE_UPG_STATUS            0x36D2

/**
 *@ingroup hct_upg
 * ����ѡ���ָ����������Ĭ�ϸ������Լ��������������ļ���Section ID�Ƿ�ƥ���������Ƿ����������
 * �÷���HI_UPG_REQ_OPTION_NA | HI_UPG_VER_UPDATE_OPTION_START��
 */
#define HI_UPG_REQ_OPTION_NA         0x01

/**
 *@ingroup hct_upg
 * ����ѡ�������CCO��
 * �÷���HI_UPG_REQ_OPTION_CCO | HI_UPG_VER_UPDATE_OPTION_START��
 */
#define HI_UPG_REQ_OPTION_CCO        0x02

/**
 *@ingroup hct_upg
 * ����ѡ�������STA��������CCO��
 * �÷���HI_UPG_REQ_OPTION_STA | HI_UPG_VER_UPDATE_OPTION_START��
 */
#define HI_UPG_REQ_OPTION_STA        0x04

/**
 *@ingroup hct_upg
 * ����ѡ�ȫ���������������������CCO��STA��
 * �÷���HI_UPG_REQ_OPTION_BROADCAST | HI_UPG_VER_UPDATE_OPTION_START��
 */
#define HI_UPG_REQ_OPTION_BROADCAST  0x08

/**
 *@ingroup hct_upg
 * ����ѡ�����������
 * �÷���HI_UPG_REQ_OPTION_UNICAST | HI_UPG_VER_UPDATE_OPTION_START��
 */
#define HI_UPG_REQ_OPTION_UNICAST    0x10

/**
 *@ingroup hct_upg
 * ����ѡ��豸������
 * �÷���HI_UPG_REQ_OPTION_DEVICE | HI_UPG_REQ_OPTION_BROADCAST | HI_UPG_VER_UPDATE_OPTION_START��
 */
#define HI_UPG_REQ_OPTION_DEVICE     0x0100

/**
 *@ingroup hct_upg
 * ����ѡ��б�������
 * @li �б�����վ�����CCO��STA�÷���HI_UPG_REQ_OPTION_LIST | HI_UPG_REQ_OPTION_BROADCAST | HI_UPG_VER_UPDATE_OPTION_START��
 * @li �б�����վ��ֻ����STA�÷���HI_UPG_REQ_OPTION_LIST | HI_UPG_REQ_OPTION_STA | HI_UPG_VER_UPDATE_OPTION_START��
 */
#define HI_UPG_REQ_OPTION_LIST       0x0200

/**
 *@ingroup hct_upg
 * ����ѡ���ʼ������
 * �÷���HI_UPG_REQ_ | HI_UPG_VER_UPDATE_OPTION_START��
 */
#define HI_UPG_VER_UPDATE_OPTION_START      0x20

/**
 *@ingroup hct_upg
 * ��ȫ������CCO����STA��ʽ��
 */
#define HI_MDM_UPGRADE_NA       0x00

/**
 *@ingroup hct_upg
 * CCOͨ��ȫ����ʽ����STA��
 */
#define HI_MDM_UPGRADE_ALL      0x01
/**
 *@ingroup hct_upg
 * CCOͨ���б�ʽ����STA��
 */
#define HI_MDM_UPGRADE_LIST     0x02

 /**
* @ingroup  hct_upg
* @brief  �û�ע��������ϢID��Ӧ�Ļص�������
*
* @par   ����:
            �û�ע��������ϢID��Ӧ�Ļص�������
* @attention	�ޡ�
* @param   ulId [IN]  ��ϢID��
* @param   pucPacket [IN]  ����Ӧ�������
* @param   usPacketSize [IN]  ����Ӧ������ṹ���С��
* @retval #HI_ERR_SUCCESS �û�����ɹ���
* @retval #����ֵ �û�����ʧ�ܣ��μ�hi_error.h��
* @par Dependency:
* <ul><li>hi_mdm_upg.h: �ļ�����������������ӿڡ�</li></ul>
* @see 
* @since HiMDL_V200R001C00
*/
typedef HI_U32 (*HI_MDM_UPG_PROC_MSG_FUN)(HI_U32 ulId, HI_PBYTE pucPacket, HI_U16 usPacketSize);

/**
 *@ingroup hct_upg
 * ����״̬��ѯ���������
 */
typedef struct
{
    HI_U32 ulOption;  /**<��ѯ����״̬��Ϣ���͡�ȡֵΪHI_MDM_UPG_STATUS_REQ_OPT_BS��ʾ���ϱ���������״̬��Ϣ��ȡ����ֵ��ʾ��ѯ����������Ϣ��*/
    HI_U32 blk_per_pkt;  /**<��ѯ������δʹ�á�*/
} HI_UPG_STATUS_REQ_S;

/**
 *@ingroup hct_upg
 * ��ʼ�������������
 */
typedef struct
{
    HI_U32 ulOption;     /**<����ѡ������HI_UPG_VER_UPDATE_OPTION_START�����Ի�������ֵ��\n 
	                    HI_UPG_REQ_OPTION_CCO��ʾ������CCOģ�飬����STAģ�鶼��������\n
						HI_UPG_REQ_OPTION_STA��ʾ����������STAģ�飬������CCOģ�顣\n
						HI_UPG_REQ_OPTION_BROADCAST��ʾȫ�������� \n
						HI_UPG_REQ_OPTION_UNICAST��ʾ���������������������յ����������ģ�飬���ٴ��������ģ�顣\n 
						HI_UPG_REQ_OPTION_DEVICE��ʾ�豸��������ΪSTA�¹��豸������STA�յ������ļ�֮����Ҫת�����¹��豸��CCO�����HI_UPG_REQ_OPTION_BROADCASTһ��ʹ�á�\n
						HI_UPG_REQ_OPTION_LIST��ʾ�б���������Ϊѡ����CCO��STA������Ϊѡ����STA������ѡ����վ���а���CCO��STA�������HI_UPG_REQ_OPTION_BROADCASTʹ�ã�ѡ�����б���ֻ����STA�������HI_UPG_REQ_OPTION_STAʹ�á�*/
    HI_U32 ulUpgId;     /**<����ID��ID�������������б���Ψһ��    */
    HI_U32 ulFileLen;     /**<�����ļ��ܳ��ȣ���λ��byte��ȫ�������ļ��������ֵHI_MDM_UPG_CCO_STA_FILE_MAX_SIZE����ȫ�����������ļ��������ֵHI_MDM_UPG_FILE_MAX_SIZE��    */
    HI_U32 ulBlkNum;     /**<�̶�Ϊ1��    */
    HI_U32 ulUpgTimeWindow;     /**<������ʼ��������ʱ�䴰����λ��min�� �ò�����Ϊ0ʱ��ʹ��Ĭ��ֵ360min��  */
    HI_U32 ulFileCrc;     /**<�ļ�CRC������HI_MDM_Crc32�ӿڼ��㡣 */
    HI_NV_FTM_UPG_CFG_S stUpgCfg;     /**<���������ṹ�壬Ĭ����0��    */
} HI_UPG_START_REQ_S;

/**
 *@ingroup hct_upg
 * STA��CCOӦ��ʼ����������
 */
typedef struct
{
    HI_U32 ulRc;     /**<����룺HI_ERR_SUCCESS��ʾ�ɹ�������Ϊ�����롣*/
    HI_U32 ulUpgId;     /**<����ID��*/
    HI_U8 mac[HI_PLC_MAC_ADDR_LEN];  /**<ͨ��ģ���MAC��ַ��PLC��MAC��ַ����  */
    HI_U8 pad[2];
} HI_UPG_START_IND_S;

/**
 *@ingroup hct_upg
 * ��ʼ��������������б���Ϣ��
 */
typedef struct
{
    HI_U16 usStaNum;     /**<վ���б���վ����������ȡֵΪ2047��*/
    HI_U16 padding;
}HI_UPG_STA_LIST_INFO_S;

/**
 *@ingroup hct_upg
 * ֹͣ�������������
 */
typedef struct
{
    HI_U32 ulUpgId;     /**<����ID������ֹͣ������ulUpgId������������ʼ������ulUpgIdƥ�䣬���򵥰岻��Ӧֹͣ����������ֹͣ��ulUpgIdΪ0ʱ��������������Ӧֹͣ��������  */
    HI_U16 usOption;    /**<�̶�Ϊ0��    */
    HI_U16 reserved;
} HI_UPG_STOP_REQ_S;

/**
 *@ingroup hct_upg
 * ֹͣ����Ӧ�������
 */
typedef struct
{
    HI_U32 ulRc;      /**<����룺HI_ERR_SUCCESS��ʾ�ɹ�������Ϊ�����룬�μ�hi_error.h��  */
    HI_U32 ulUpgId;   /**<����ID�������������б���Ψһ�� */
} HI_UPG_STOP_IND_S;

/**
 *@ingroup hct_upg
 * ��ʼ������������������ļ���Ϣ��
 */
typedef struct
{
    HI_U16 usChecksum;      /**<У��ͣ��ο������ļ�У��ͼ������ӡ� */
    HI_U16 usFileBlkSize;   /**<�̶�Ϊ0��    */
    HI_U32 ulUpgId;      /**<����ID��    */
    HI_U16 usReqSn;      /**<�̶�Ϊ0��    */
    HI_U16 usIdx;      /**<�̶�Ϊ1��    */
    HI_U8 ucFileBlk[0];      /**<�����ļ����ݣ���.hupg�ļ�����    */
} HI_UPG_TRANSMIT_BLK_REQ_S;

/**
 *@ingroup hct_upg
 * ����״̬��ѯӦ��ṹ��汾���塣
 */
typedef enum
{
    HI_UPG_STATUS_IND_ENTRY_VER0 = 0,      /**< �汾0����Ӧ�Ľṹ��ΪHI_UPG_STATUS_IND_ENTRY_S_VER0��*/
    HI_UPG_STATUS_IND_ENTRY_VER1,          /**< �汾1����Ӧ�Ľṹ��ΪHI_UPG_STATUS_IND_ENTRY_S��*/
} HI_UPG_STATUS_IND_ENTRY_VER_E;

/**
 *@ingroup hct_upg
 * ����״̬��ѯӦ��ṹ��汾�š�
 */
#define HI_UPG_STATUS_IND_ENTRY_VER     HI_UPG_STATUS_IND_ENTRY_VER1

/**
 *@ingroup hct_upg
 * վ������״̬��Ϣ�ṹ��汾0��
 */
typedef struct
{
    HI_U16 usLastBlkIdx;          /**< ���һ��������Ч�����ļ���������š�      */
    HI_U16 ulErrorCode;          /**< ���������еĴ����롣      */
    HI_U32 ulRcvFileSize;          /**< ͨ��ģ����������ļ����ȣ���λ��byte����ʾ���յ���Ч���С�ܺͣ����ܲ���������      */
    HI_U8  usUpgStatus;          /**< ��ǰ����״̬��      */
    HI_U8  aucVer[3];          /**< VRC�汾�š�      */
    HI_U16 ausVer[2];          /**< ausVer[0]ΪԤ���ֶκ�ausVer[1]ΪSPC�汾�š�      */
    HI_U8  aucBootVer3;          /**< Boot�汾�������ֽڡ�      */
    HI_U8  ucNvCVer;          /**< NV�ļ���C�汾��      */
    HI_U8  aucBuildDataTime[6];          /**<�����ļ����������ں�ʱ��,YYMMDDhhmmss��      */
    HI_U16 ausNvVer[2];          /**< ausNvVer[0]ΪԤ���ֶκ�ausNvVer[1]ΪNV��SPC�汾�š�      */
    HI_U32 ulChipVer;          /**< оƬ���кš�      */
    HI_U16 usUpgReqSn;          /**< �鴫���SN(��δʹ�� �̶�Ϊ0)��      */
    HI_U8  ucMacAddr[HI_PLC_MAC_ADDR_LEN];          /**< ͨ��ģ���MAC��ַ��PLC��MAC��ַ����      */
    HI_U32 ulUpgEndTime;          /**< ģ��������ʱ��      */
} HI_UPG_STATUS_IND_ENTRY_S_VER0;

/**
 *@ingroup hct_upg
 * վ������״̬��Ϣ�ṹ��汾1��
 */
typedef struct
{
    HI_U16 usLastBlkIdx;  /**< ���һ��������Ч�����ļ���������š� */
    HI_U16 ulErrorCode;   /**< ���������еĴ����롣      */
    HI_U32 ulRcvFileSize; /**< ͨ��ģ����������ļ����ȣ���λ��byte����ʾ���յ���Ч���С�ܺͣ����ܲ��������� */
    HI_U8  usUpgStatus; /**< ��ǰ����״̬��
                            HI_MDM_UPG_STATUS_OK��ʾ�����ɹ���\n
							HI_MDM_UPG_STATUS_END��ʾ������֤״̬��\n
							HI_MDM_UPG_STATUS_FAIL��ʾ����ʧ�ܡ� \n
							HI_MDM_UPG_STATUS_PROCESS��ʾ���������� \n
							HI_MDM_UPG_STATUS_UNKNOWN��ʾδ֪״̬��*/
    HI_U8  aucVer[3];           /**< VRC�汾�š�      */
    HI_U16 ausVer[2];          /**< ausVer[0]ΪԤ���ֶκ�ausVer[1]ΪSPC�汾�š�      */
    HI_U8  aucBootVer3;          /**< Boot�汾�������ֽڡ�      */
    HI_U8  reserved1;
    HI_U8  aucBuildDataTime[6];      /**< �����ļ����������ں�ʱ��,YYMMDDhhmmss��      */
    HI_CHAR szManuCode[2];          /**< ���̴��롣      */
    HI_CHAR szChipCode[2];          /**< оƬ���롣      */
    HI_U16 usSwVer;          /**< �ͻ�����汾�š�      */
    HI_U16 usYear   : 7;          /**< �ͻ��汾����-�ꡣ      */
    HI_U16 usMonth  : 4;          /**< �ͻ��汾����-�¡�     */
    HI_U16 usDay    : 5;          /**< �ͻ��汾����-�ա�      */
    HI_U8  ucStruVer: 4;          /**< �ṹ��汾�š�      */
    HI_U8  reserved2 : 4;
    HI_U8  reserved3;
    HI_U8  ucMacAddr[HI_PLC_MAC_ADDR_LEN];          /**< ͨ��ģ���MAC��ַ��PLC��MAC��ַ����      */
    HI_U32 ulUpgEndTime;          /**< ģ��������ʱ��      */
} HI_UPG_STATUS_IND_ENTRY_S;

/**
 *@ingroup hct_upg
 * ȫ��������ѯ����״̬������Ϣʱһ���ϱ���վ�������
 */
#define UPG_STATUS_IND_ENTRY_NUM_MAX 8

/**
 *@ingroup hct_upg
 * ����״̬��ѯӦ��
 */
typedef struct
{
    HI_U32 ulUpgId;          /**< �������̵ı�ʶ�������������У��ñ�ʶ�����ظ��� */
    HI_U32 ulBlkSize;          /**< �̶�Ϊ300��      */
    HI_U32 ulFileSize;          /**< �����ļ���С����λ��byte��      */
    HI_U32 ulNum;          /**< entry����Ч������      */
    HI_UPG_STATUS_IND_ENTRY_S entry[0];          /**< վ������״̬��Ϣ��  */
} HI_UPG_STATUS_IND_S;

/**
 *@ingroup hct_upg
 * ȫ������״̬��ѯͳ����ϢӦ�������
 */
typedef struct
{
    HI_U32 ulUpgId;          /**< ȫ�������ı�ʶ��    */
    HI_U16 usOkNum;          /**< ȫ������ͨ��ģ�������ɹ����ܸ�����       */
    HI_U16 usCompleteNum;    /**< ȫ������ͨ��ģ��������֤�е��ܸ�����     */
    HI_U16 usFailNum;        /**< ȫ������ͨ��ģ������ʧ�ܵ��ܸ�����       */
    HI_U16 usProcessNum;     /**< ȫ������ͨ��ģ�����ڴ�������е��ܸ����� */
    HI_U8  ucUpgStatus;      /**< ��ǰȫ������״̬��HI_MDM_UPG_STATUS_OK��ʾȫ�������ɹ���HI_MDM_UPG_STATUS_END��ʾȫ������������HI_MDM_UPG_STATUS_PROCESS��ʾ����ȫ��������*/

    HI_U8 ucUpgOption : 2;   /**< CCO����STA��ʽ��HI_MDM_UPGRADE_NA��ʾ���漰��HI_MDM_UPGRADE_ALL��ʾȫ����ʽ��HI_MDM_UPGRADE_LIST��ʾ�б�ʽ��   */
    HI_U8 pad1 : 6;
    HI_U8 pad2[2];
} HI_UPG_STAT_INFO_IND_S;

/**
 *@ingroup hct_upg
 * ����״̬���������
 */
typedef struct
{
    HI_UPG_STATUS_REQ_S stReqStatus;    /**< ״̬��ѯ����   */
}HI_MDM_UPG_STATUS_REQ_S;

/**
 *@ingroup hct_upg
 * ����״̬Ӧ������ṹ�塣
 */
typedef struct
{
    HI_UPG_STATUS_IND_S stUpgStatus;   /**< ȫ���򵥵�����״̬��ѯӦ��    */
} HI_MDM_UPG_STATUS_IND_S;

/**
 *@ingroup hct_upg
 * ����״̬ͳ����ϢӦ�������
 */
typedef struct
{
    HI_UPG_STAT_INFO_IND_S stStatInfo;      /**< ȫ������ͳ����Ϣ��ѯӦ�������    */
} HI_MDM_UPG_STAT_INFO_IND_S;

/**
 *@ingroup hct_upg
 * ����״̬������ϢӦ�������
 */
typedef struct
{
    HI_U32 ulUpgId;           /**< ����ID��      */
    HI_U8  ucUpgStatus;       /**<����״̬��HI_MDM_UPG_STATUS_OK��ʾ�����ɹ���HI_MDM_UPG_STATUS_FAIL��ʾ����ʧ�ܣ�HI_MDM_UPG_STATUS_END��ʾ����������HI_MDM_UPG_STATUS_PROCESS��ʾ����������HI_MDM_UPG_STATUS_UNKNOWN��ʾδ֪����״̬����ǰ���������������С� */
    HI_U8  aucReserved[3];
} HI_MDM_UPG_STATUS_BS_IND_S;

/**
 *@ingroup hct_upg
 * ��ʼ������������ṹ�塣
 */
typedef struct
{
    HI_UPG_START_REQ_S        stUpgCfg;         /**< ��ʼ������Ϣ�� */
    HI_UPG_STA_LIST_INFO_S    stUpgListInfo;    /**< վ���б���Ϣ�� */
    HI_UPG_TRANSMIT_BLK_REQ_S stUpgFile;        /**< �����ļ���Ϣ�� */
} HI_MDM_UPG_START_REQ_S;

/**
 *@ingroup hct_upg
 * ��ʼ����Ӧ�������
 */
typedef struct
{
    HI_U32 ulRc;           /**< ������ʼִ�н��:HI_ERR_SUCCESS��ʾ���������ɹ�������ֵ��ʾ��������ʧ�ܣ��μ�hi_error.h��  */
}HI_MDM_UPG_START_IND_S;

/**
 *@ingroup hct_upg
 * ֹͣ�������������
 */
typedef struct
{
    HI_UPG_STOP_REQ_S stStopReq;    /**< ֹͣ�������������      */
} HI_MDM_UPG_STOP_REQ_S;

/**
 *@ingroup hct_upg
 * ֹͣ����Ӧ�������
 */
typedef struct
{
    HI_UPG_STOP_IND_S stStopInd;    /**< ֹͣ����Ӧ����Ϣ��      */
} HI_MDM_UPG_STOP_IND_S;

/**
 *@ingroup hct_upg
 * �����豸�������������
 */
typedef struct
{
    HI_U32 ulUpgId;           /**< ����ID��ȡֵΪȫ������CCO�·���STA��������ʶ��      */
    HI_U32 ulFileSize;           /**< �����ļ���С����λ��byte��      */
    HI_U8 *pucFileData;           /**< �����ļ����ݣ�pucFileDataΪ��ʱ����Ҫͨ���ӿ�HI_MDM_UPG_MemRead()��ȡ�����ļ����ݡ�      */
} HI_MDM_DEVICE_UPG_START_REQ_S;

/**
 *@ingroup hct_upg
 * �����豸����Ӧ������ṹ�塣
 */
typedef struct
{
    HI_U32 ulUpgId;           /**< ����ID��ȡֵΪȫ������CCO�·���STA��������ʶ��      */
    HI_U32 ulRc;           /**< ��ʼ�豸����ִ�н����HI_ERR_SUCCESS��ʾ�����ɹ�������ֵ��ʾ����ʧ�ܡ�   */
} HI_MDM_DEVICE_UPG_START_IND_S;

/**
 *@ingroup hct_upg
 * �豸����ֹͣ��������ṹ�塣
 */
typedef struct
{
    HI_U32 ulUpgId;           /**< ����ID��      */
} HI_MDM_DEVICE_UPG_STOP_REQ_S;

/**
 *@ingroup hct_upg
 * �豸����ֹͣӦ������ṹ�塣
 */
typedef struct
{
    HI_U32 ulUpgId;           /**< ����ID��      */
    HI_U32 ulRc;           /**< ֹͣ�豸����ִ�н����      */
} HI_MDM_DEVICE_UPG_STOP_IND_S;

/**
 *@ingroup hct_upg
 * �豸����״̬��ѯ��������ṹ�塣
 */
typedef struct
{
    HI_U32 ulUpgId;           /**< ����ID��ȡֵΪȫ������CCO�·���STA��������ʶ��      */
} HI_MDM_DEVICE_UPG_STATUS_REQ_S;

/**
 *@ingroup hct_upg
 * �豸����״̬���塣
 */
typedef enum
{
    HI_MDM_DEVICE_UPG_STATUS_IDLE,           /**< ���С�      */
    HI_MDM_DEVICE_UPG_STATUS_PROGRESS,       /**< ����������  */
    HI_MDM_DEVICE_UPG_STATUS_SUCCESS,        /**< �����ɹ���  */
    HI_MDM_DEVICE_UPG_STATUS_FAIL,           /**< ����ʧ�ܡ�  */
} HI_MDM_DEVICE_UPG_STATUS_E;

/**
 *@ingroup hct_upg
 * �豸����״̬��ѯӦ������ṹ�塣
 */
typedef struct
{
    HI_U32 ulUpgId;           /**< ����ID��ȡֵΪȫ������CCO�·���STA��������ʶ��      */
    HI_U8 ucStatus;           /**< �豸����״̬��\n
	                          HI_MDM_DEVICE_UPG_STATUS_IDLE��ʾ����״̬��\n
							  HI_MDM_DEVICE_UPG_STATUS_PROGRESS��ʾ�����豸������\n
							  HI_MDM_DEVICE_UPG_STATUS_SUCCESS��ʾ�豸�����ɹ���\n
							  HI_MDM_DEVICE_UPG_STATUS_FAIL��ʾ�豸����ʧ�ܡ�   */
    HI_U8 reserved[3];
} HI_MDM_DEVICE_UPG_STATUS_IND_S;

/**
* @ingroup  hct_upg
* @brief  ��ȡ�������档
*
* @par   ����:
			�ú���ʵ�ֻ�ȡ�������棬���ڴ洢�����ļ���վ���б�
* @attention ���øýӿڻ�ȡ��������ɹ�������ֹͣ������ͨ��HI_MDM_UPG_STOP�ӿڣ���ʱ����HI_MDM_UPG_ReleaseMem�ͷ��������档
* @param  ulUpgId [IN]�����ֶΣ�Ĭ����0��
* @param  ulFileSize [IN]�����ļ���С��CCO�����ʱ����ֵ���ܳ���HI_MDM_UPG_CCO_STA_FILE_MAX_SIZE�������ȡ����ʧ�ܡ�������Ʒ��̬����ʱ����ֵ���ܳ���HI_MDM_UPG_FILE_MAX_SIZE�������ȡ����ʧ�ܡ�
* @param  ppstReq [OUT]����������Ϣ������
* @param  ulOption [IN]Ԥ���ֶΡ�
* @retval #HI_ERR_SUCCESS ��ȡ�ڴ�ɹ���
* @retval #����ֵ ��ȡ�ڴ�ʧ�ܣ��μ�hi_error.h��
* @par Dependency:
* <ul><li>hi_mdm_upg.h: �ļ�����������������ӿڡ�</li></ul>
* @see �ޡ�
* @since HiMDL_V200R001C00
*/
HI_EAPI HI_U32 HI_MDM_UPG_ObtainMem(HI_U32 ulUpgId,HI_U32 ulFileSize, HI_OUT HI_MDM_UPG_START_REQ_S** ppstReq,HI_U32 ulOption);

/**
* @ingroup  hct_upg
* @brief  �ͷ��������档
*
* @par   ����:
			�ͷ�HI_MDM_UPG_ObtainMem�ӿ�������������档
* @attention	����HI_MDM_UPG_STOP�ӿ�ֹͣ����ǰ�������ȵ��øýӿ��ͷ��������档
* @param  ulUpgId [IN]�����ֶΣ�Ĭ����0��
* @param  ppstReq [OUT]����������Ϣ������
* @param  ulOption [IN]Ԥ���ֶΡ�
* @retval	#HI_ERR_SUCCESS �ͷ��ڴ�ɹ���
* @retval	#����ֵ �ͷ��ڴ�ʧ�ܣ��μ�hi_error.h��
* @par Dependency:
* <ul><li>hi_mdm_upg.h: �ļ�����������������ӿڡ�</li></ul>
* @see �ޡ�
* @since HiMDL_V200R001C00
*/
HI_EAPI HI_U32 HI_MDM_UPG_ReleaseMem(HI_U32 ulUpgId, HI_IN HI_MDM_UPG_START_REQ_S* ppstReq,HI_U32 ulOption);

/**
* @ingroup  hct_upg
* @brief  д�������ݵ�FLASH��������
*
* @par   ����:
			�ú������������ļ�д��STA������������
* @attention	
*@li ������STA��
*@li ���øýӿ�ǰ�����ȵ���HI_MDM_UPG_ObtainMem�ӿڻ�ȡ�������档
* @param  ulOffset [IN]���������������ʼ��ַ��ƫ�Ƶ�ַ��
* @param  pucBuf [IN]�������ݰ�ָ�롣
* @param  ulBufLen [IN]�������ݰ����ȣ���λ��byte��
* @retval	#>0 ʵ��д������ݳ��ȣ���λ��byte��
* @retval	#=0 ����д��ʧ�ܡ�
* @par Dependency:
* <ul><li>hi_mdm_upg.h: �ļ�����������������ӿڡ�</li></ul>
* @see �ޡ�
* @since HiMDL_V200R001C00
*/
HI_EAPI HI_U32 HI_MDM_UPG_MemWrite(HI_U32 ulOffset, HI_U8 * pucBuf, HI_U32 ulBufLen);

/**
* @ingroup  hct_upg
* @brief  ��FLASH��������ȡ���ݡ�
*
* @par   ����:
			�ú��������STA�����������������ļ����ݡ�
* @attention  ������STA��
* @param  ulOffset [IN]���������������ʼ��ַ��ƫ�Ƶ�ַ��
* @param  pucBuf [IN�������ݰ�ָ�롣
* @param  ulBufLen [IN]�������ݰ����ȣ���λ��byte��
* @retval  #>0 ʵ�ʶ�ȡ�����ݳ��ȣ���λ��byte��
* @retval  #=0 ���ݶ�ȡʧ�ܡ�
* @par Dependency:
* <ul><li>hi_mdm_upg.h: �ļ�����������������ӿڡ�</li></ul>
* @see �ޡ�
* @since HiMDL_V200R001C00
*/
HI_EAPI HI_U32 HI_MDM_UPG_MemRead(HI_U32 ulOffset, HI_U8 * pucBuf, HI_U32 ulBufLen);

/**
* @ingroup  hct_upg
* @brief  ��ȡ����ʱ϶��
*
* @par   ����:
			��ȡ����ʱ϶��
* @attention  ������CCO��
* @param  lid [IN]��ҵ����·��ʶ����ǰ����������ҵ�����·��ʶ���������õ�lidֵ:2,6��
* @retval	#HI_ERR_SUCCESS ��������ʱ϶�ɹ���
* @retval   #����ֵ ��������ʱ϶ʧ��,�μ�hi_error.h��
* @par Dependency:
* <ul><li>hi_mdm_upg.h: �ļ�����������������ӿڡ�</li></ul>
* @see  �ޡ�
* @since HiMDL_V200R001C00
*/
HI_EAPI HI_U32 HI_MDM_ObtainUpgTimeSlot(HI_U8 lid);

/**
* @ingroup  hct_upg
* @brief  �ͷ�����ʱ϶��
*
* @par   ����:
			��ȡ����ʱ϶��
* @attention ������CCO��
* @param  lid [IN]��ҵ����·��ʶ����ǰ����������ҵ�����·��ʶ���������õ�lidֵ:2,6��
* @retval	#HI_ERR_SUCCESS �ͷ�����ʱ϶�ɹ���
* @retval   #����ֵ �ͷ�����ʱ϶ʧ��,�μ�hi_error.h��
* @par Dependency:
* <ul><li>hi_mdm_upg.h: �ļ�����������������ӿڡ�</li></ul>
* @see �ޡ�
* @since HiMDL_V200R001C00
*/
HI_EAPI HI_U32 HI_MDM_ReleaseUpgTimeSlot(HI_U8 lid);

/**
* @ingroup  hct_upg
* @brief  ע��������Ϣ����ӿڡ�
*
* @par   ����: 
			ע��������Ϣ����ӿڡ�
* @attention ������������ǰ���á�
* @param  ulMsgId [IN]��ϢID��
* @param  pfnUpgMsgNotify [IN]�û�ע��Ļص�������
* @retval �ޡ�
* @par Dependency:
* <ul><li>hi_mdm_upg.h: �ļ�����������������ӿڡ�</li></ul>
* @see �ޡ�
* @since HiMDL_V200R001C00
*/
HI_VOID HI_MDM_RegisterUpgMsgNotify(HI_U32 ulMsgId, HI_MDM_UPG_PROC_MSG_FUN pfnUpgMsgNotify);

/**
* @ingroup  hct_upg
* @brief  ������Ϣ���ͽӿڡ�
*
* @par   ����: 
			������Ϣ���ͽӿڡ�
* @attention  �ޡ�
* @param  ulMsgId [IN]��ϢID��
* @param  pucPacket [IN]���͵����ݰ����ݡ�
* @param  ulPacketSize [IN]���͵����ݰ���С��
* @retval  #HI_ERR_SUCCESS ������Ϣ���ͳɹ���
* @retval  #����ֵ ����ʧ��,�μ�hi_error.h��
* @par Dependency:
* <ul><li>hi_mdm_upg.h: �ļ�����������������ӿڡ�</li></ul>
* @see �ޡ�
* @since HiMDL_V200R001C00
*/
HI_U32 HI_MDM_UPG_MsgSend(HI_U32 ulMsgId, HI_PBYTE pucPacket, HI_U32 ulPacketSize);

/**
* @ingroup  hct_upg
* @brief  ע�������ļ��Ϸ���У��ӿڡ�
*
* @par   ����: 
			ע�������ļ��Ϸ���У��ӿڡ�
* @attention �ޡ�
* @param  pfnUpgFileCheck [IN]�û��Զ��������ļ�У��ӿڡ�
* @param  pParam [IN]�ص�ʱ���ظ��ص�������
* @retval   #HI_ERR_SUCCESS ע��ɹ���
* @retval   #����ֵ ע��ʧ�ܣ��μ�hi_error.h��
* @par Dependency:
* <ul><li>hi_mdm_upg.h: �ļ�����������������ӿڡ�</li></ul>
* @see �ޡ�
* @since HiMDL_V200R001C00SPC010
*/
HI_U32 HI_MDM_RegisterUpgFileVerifyFct(
    HI_BOOL (*pfnUpgFileCheck)(HI_UPG_FILE_HEADER_S *pstHeader, HI_VOID *pParam),
    HI_VOID *pParam);

/**
* @ingroup  hct_upg
* @brief  ���ú�˼����Э�����͡�
*
* @par   ����: 
			���ú�˼����Э�����͡�
* @attention	
*@li ������CCO��
*@li ������������ǰ���á��û���Ҫʹ�ú�˼����Э�飬����ô˽ӿڡ�
*@li ��֧��ע���������Э�鳡����
* @retval  #HI_ERR_SUCCESS ���óɹ���
* @retval  #����ֵ ����ʧ�ܡ�
* @par Dependency:
* <ul><li>hi_mdm_upg.h: �ļ�����������������ӿڡ�</li></ul>
* @see �ޡ�
* @since HiMDL_V200R001C00SPC012
*/
HI_U32 HI_MDM_UPG_SetPrivateProto(HI_VOID);

/**
* @ingroup  hct_upg
* @brief  �����������ע��ص�������
*
* @par   ����: 
			�����������ע��ص�������
* @attention ������CCO��
* @param  �ޡ�
* @retval �ޡ�
* @par Dependency:
* <ul><li>hi_mdm_upg.h: �ļ�����������������ӿڡ�</li></ul>
* @see �ޡ�
* @since HiMDL_V200R001C00SPC010
*/
typedef HI_VOID (*HI_MDM_pfnAffirmNotify)(HI_VOID *pParam);

/**
* @ingroup  hct_upg
* @brief  ע�������������֪ͨ�ӿڡ�
*
* @par   ����: 
			ȫ������ʱע��CCO��STA�������֪ͨ�ӿڡ�
* @attention  ������CCO��
* @param  pfnCallback [IN]�����������֪ͨ�ӿڡ�
* @param  pParam [IN]�û�ע��Ļص�������
* @retval #HI_ERR_SUCCESS ע��ɹ���
* @retval #HI_ERR_SUCCESS ע��ʧ�ܡ�
* @par Dependency:
* <ul><li>hi_mdm_upg.h: �ļ�����������������ӿڡ�</li></ul>
* @see �ޡ�
* @since HiMDL_V200R001C00SPC010
*/
HI_U32 HI_MDM_RegisterAffirmNotify(HI_MDM_pfnAffirmNotify pfnCallback, HI_VOID *pParam);

/**
 *@ingroup hct_upg
 * ��ʱ�ȴ����á�
 */
typedef struct
{
    HI_BOOL bEnable;      /**<����������ϵ������ϵ��վ��������ڿ�ʼ����ʱվ��������Ƿ�������ʱ�ȴ����ԡ�Ĭ��ֵΪ1��ȡֵΪ0��ʾ��������ʱ�ȴ����ԣ�ȡ����ֵ��ʾ���øò��ԡ�*/
    HI_U8 ucPeriod;   /**<���ڼ��վ���Ƿ���ȫ�Ķ�ʱ������ʱ������λ:min��Ĭ��Ϊ2min��*/
    HI_U8 ucTimerCnt; /**<һ�ְ����Ķ�ʱ�����ڸ�����Ĭ��ֵΪ10���ڵȴ���һ��ʱ����վ�����û�仯�����ټ�����ʱ�ȴ���������Ϊ0ʱ��ȡĬ��ֵ10��*/
    HI_U8 ucLoopCnt;  /**<��ʱ�ȴ�������Ĭ��ֵΪ3���ﵽ��������󣬲��ٵȴ���*/
}UPG_DELAY_WAIT_CFG_STRU;

/**
 *@ingroup hct_upg
 * ��ѯվ����Ϣ���á�
 */
typedef struct
{
    HI_U8 ucParallelNum; /**<������ѯվ�������Ĭ��ֵΪ5��������Ϊ0ʱ��ȡĬ��ֵ5��*/
    HI_U8 reserved[3];
    HI_U16 usLoopInterval;  /**<�ּ������λ:s��Ĭ��ֵΪ60��*/
    HI_U16 usInterval;     /**<����֡�������λ:ms��Ĭ��ֵΪ2000��*/
}UPG_QRY_STA_INFO_CFG_STRU;

/**
 *@ingroup hct_upg
 * ȫ���㲥���á�
 */
typedef struct
{
    HI_U8   succ_ratio;              /**<�������վ��ٷֱȣ�Ĭ��ֵ:100��*/
    HI_U8   exception_max_count;/**<���̵����EXCEPTION������Ĭ��ֵ:2��*/
    HI_U8   query_switch;       /**<�״��Ƿ��ѯ���أ�Ĭ��ֵ:0����ʾ���ֲ���ѯ��ȡֵΪ1��ʾ���ֲ�ѯ��ȡ����ֵ��ʾ���ֲ���ѯ��*/
    HI_U8   query_count;        /**<����ѯ������Ĭ��ֵ:5��*/

    HI_U16  data_loop_max;      /**<���ݷ�������ִΣ�Ĭ��ֵ:4��*/
    HI_U16  data_send_interval; /**<���Ͱ�֮�䷢�ͼ������λ:ms��Ĭ��ֵ:200��*/
    HI_U16  query_send_interval;/**<��ѯ֡���ͼ������λ:ms��Ĭ��ֵ:500��*/
    HI_U16  query_max_ways;     /**<��󲢷���ѯ����Ĭ��ֵ:3����ȡֵ���ڵ���10ʱ�������0ʱ��ȡ���ֵ9��*/
}BROADCAST_CFG_STRU;

/**
 *@ingroup hct_upg
 * ȫ�����֪ͨ���á�
 */
typedef struct
{
    HI_U8 delayed_for_send_restart;        /**<CCO���������������ʱ�����������λ:s��Ĭ��ֵ:70��*/
    HI_U8 delayed_for_recv_to_restart;      /**<STA�յ������������ʱ�����������λ:s��Ĭ��ֵ:60��*/
    HI_U8 send_count;               /**<���ͼ������Ĭ��ֵ:60����ֵΪ0ʱ��ֻ����1�֡�*/
    HI_U8 reserved1;           
    HI_U16 send_interval;           /**<���Ͱ�֮�䷢�ͼ������λ:ms��Ĭ��ֵ:500��*/
    HI_U16 reserved2;
}AFFIRM_CFG_STRU;

/**
 *@ingroup hct_upg
 * ��ʼ����֡���á�
 */
typedef struct
{
    HI_U8 send_count;         /**<�㲥���ͼ������Ĭ��ֵ:5����ֵΪ0ʱ��ֻ����1�֡�*/
    HI_U8 bc_delayed;        /**<�㲥����start֡�ȴ���÷������ݣ���λ:s��Ĭ��ֵ:30��*/  
    HI_U16 bc_send_interval; /**<�㲥���Ͱ�֮�䷢�ͼ������λ:ms��Ĭ��ֵ:500��*/
    HI_U8 reserved;
    HI_U8 delayed;        /**<��������start֡�ȴ���÷������ݣ���λ:s��Ĭ��ֵ:12��*/    
    HI_U16 send_interval; /**<�������Ͱ�֮�䷢�ͼ������λ:ms��Ĭ��ֵ:200��*/
    HI_U32 send_ways;     /**<��������ʱ����·����Ĭ��ֵ:5����ȡֵ���ڵ���10ʱ�������0ʱ��ȡ���ֵ9��*/
}NOTIFY2START_CFG_STRU;

/**
 *@ingroup hct_upg
 * ����ת���ع㲥���á�
 */
typedef struct
{
    HI_U8  succ_ratio;              /**<�������վ��ٷֱȣ�Ĭ��ֵ:100��*/
    HI_U8  exception_max_count;     /**<���̵����EXCEPTION������Ĭ��ֵ:5��*/
    HI_U8  query_switch;           /**<�״��Ƿ��ѯ���أ�Ĭ��ֵ:1����ʾ���ֲ�ѯ��ȡ����ֵ��ʾ���ֲ���ѯ��*/
    HI_U8  query_count;            /**<����ѯ������Ĭ��ֵ:3��*/

    HI_U16 data_loop_max;          /**<���ݷ�������ִΣ�Ĭ��ֵ:1��*/
    HI_U16 data_max_ways;          /**<ͬʱ���������·����Ĭ��ֵ:3����ȡֵ���ڵ���10ʱ�������0ʱ��ȡ���ֵ9��*/
    HI_U16 data_send_interval;     /**<���Ͱ�֮�䷢�ͼ������λ:ms��Ĭ��ֵ:200��*/
    HI_U16 query_send_interval;    /**<��ѯ֡���ͼ������λ:ms��Ĭ��ֵ:600��*/
    HI_U16 query_max_ways;         /**<��󲢷���ѯ����Ĭ��ֵ:3����ȡֵ���ڵ���10ʱ�������0ʱ��ȡ���ֵ9��*/
    HI_U16 reverved;             
}U2B_CFG_STRU;

/**
 *@ingroup hct_upg
 * �����������á�
 */
typedef struct
{
    HI_U8   succ_ratio;          /**<�������վ��ٷֱȣ�Ĭ��ֵ:100��*/
    HI_U8   exception_max_count; /**<���̵����EXCEPTION������Ĭ��ֵ:5��*/
    HI_U8   query_switch;        /**<�״��Ƿ��ѯ���أ�Ĭ��ֵ:1����ʾ���ֲ�ѯ��ȡ����ֵ��ʾ���ֲ���ѯ��*/
    HI_U8   query_count;        /**<����ѯ���������exception_max_countʹ�ã�Ĭ��ֵ:3��*/

    HI_U16  data_loop_max;     /**<���ݷ�������ִΣ����ﵽ����ʱ��exception��1����exception�ﵽexception_max_count�󽫸�վ����Ϊexception��Ĭ��ֵ:10��*/
    HI_U16  data_max_ways;     /**<ͬʱ���������·����Ĭ��ֵ:3����ȡֵ���ڵ���10ʱ�������0ʱ��ȡ���ֵ9��*/
    HI_U16  data_send_interval; /**<���Ͱ�֮�䷢�ͼ������λ:ms��Ĭ��ֵ:200��*/
    HI_U16  query_send_interval; /**<��ѯ֡���ͼ������λ:ms��Ĭ��ֵ:700��*/
    HI_U16  query_max_ways;       /**<��󲢷���ѯ����Ĭ��ֵ:3����ȡֵ���ڵ���10ʱ�������0ʱ��ȡ���ֵ9��*/
    HI_U16 reverved;                 
}UNICAST_CFG_STRU;

/**
 *@ingroup hct_upg
 * Ĭ�ϲ������ã�����ȫ���㲥������ȫ�����֪ͨ��������ʼ����֡�����ȡ�
 */
typedef struct
{
    HI_U8 broadcast_switch; /**<�Ƿ����ȫ���㲥�Ŀ��أ�Ĭ��ֵ:1����ʾ�򿪸ò��ԡ�ȡ��1��ֵ��ʾ�رոò��ԡ�*/
    HI_U8 u2b_switch;       /**<�Ƿ���õ���ת���ع㲥�Ŀ��أ�Ĭ��ֵ:1����ʾ�򿪸ò��ԡ�ȡ��1��ֵ��ʾ�رոò��ԡ�*/
    HI_U8 unicast_switch;   /**<�Ƿ���ò��������Ŀ��أ�Ĭ��ֵ:1����ʾ�򿪸ò��ԡ�ȡ��1��ֵ��ʾ�رոò��ԡ�*/
    HI_U8 reserved1;
    
    BROADCAST_CFG_STRU bc_cfg; /**<ȫ���㲥���á�*/
    AFFIRM_CFG_STRU affirm_cfg; /**<ȫ�����֪ͨ���á�*/
    NOTIFY2START_CFG_STRU notify2start_cfg; /**<��ʼ����֡���á�*/

    HI_U8 query_loop_interval; /**<����һ���ѯ���ĺ�ȴ�ʱ�䣬��λ:s��Ĭ��ֵ:12��*/
    HI_U8 data_loop_interval; /**<����һ��������ɺ�ȴ���ʱ�䣬��λ:s��Ĭ��ֵ:1��*/
    HI_U8 switch_schedule_delay; /**<���Ե����л�ʱ�䣬��λ:100ms��Ĭ��ֵ:12��*/
    HI_U8 reserved2;
}UPG_SCHEDL_DEFAULT_CFG_STRU;

/**
 *@ingroup hct_upg
 * �߼��������ã���������ת���ع㲥�������������á�
 */
typedef struct
{
    U2B_CFG_STRU u2b_cfg;      /**<����ת���ع㲥���á�*/
    UNICAST_CFG_STRU uc_cfg;   /**<�����������á�*/
}UPG_SCHEDL_ADVANCE_CFG_STRU;

/**
 *@ingroup hct_upg
 * ����������
 */
typedef struct
{
    UPG_SCHEDL_DEFAULT_CFG_STRU stDefCfg;  /**<Ĭ�ϲ������á�*/
    UPG_SCHEDL_ADVANCE_CFG_STRU stAdvanceCfg; /**<�߼��������á�*/
    UPG_DELAY_WAIT_CFG_STRU stDelayWaitCfg; /**<��ʱ�ȴ����á�*/
    UPG_QRY_STA_INFO_CFG_STRU stQryStaCfg; /**<��ѯվ����Ϣ���á�*/
}HI_MDM_UPG_CFG;

/**
* @ingroup  hct_upg
* @brief  ��ȡ�������ò�����
*
* @par   ����:
			�ú������ڻ�ȡ�������ò�����
* @attention �ޡ�
* @param  pstCfg [OUT]�������ò�����
* @retval #HI_ERR_SUCCESS ��ȡ�ɹ���
* @retval #����ֵ ��ȡʧ�ܣ��μ�hi_error.h��
* @par Dependency:
* <ul><li>hi_mdm_upg.h: �ļ�����������������ӿڡ�</li></ul>
* @see �ޡ�
* @since HiMDL_V200R001C00SPC010
*/
HI_EAPI HI_U32 HI_MDM_UPG_GetCfg(HI_MDM_UPG_CFG *pstCfg);

/**
* @ingroup  hct_upg
* @brief  �����������ò�����
*
* @par   ����:
			�ú������������������ò�����
* @attention 
*@li ������CCO��
*@li ��Ҫ��APP�������ʼ���ӿ��е��á�����ʹ�õĲ����뵱ǰ���еĳ���ƥ�䡣����û������øýӿڣ���ʹ��Ĭ�ϵĲ����� 
*@li Ĭ�ϲ������ú͸߼���������������CCO��������ǰ���ַ����ݹ�����ʹ�á�
*@li ��ʱ�ȴ�����������CCO����������ϵ�������������ɺ�վ������ȿ�ʼ����ʱվ������ٳ�����
*@li ��ѯվ����Ϣ����������CCO��������������ɺ�
*@li ʱ����ز���������Ϊ0ʱ��ȡĬ��ֵ10ms��
* @param  pstCfg [IN]�������ò�����
* @retval #HI_ERR_SUCCESS ���óɹ���
* @retval #����ֵ ����ʧ�ܣ��μ�hi_error.h��
* @par Dependency:
* <ul><li>hi_mdm_upg.h: �ļ�����������������ӿڡ�</li></ul>
* @see �ޡ�
* @since HiMDL_V200R001C00SPC010
*/
HI_EAPI HI_U32 HI_MDM_UPG_SetCfg(HI_MDM_UPG_CFG *pstCfg);

/**
* @ingroup  hct_upg
* @brief  ��ʼ������Э�顣
*
* @par   ����: 
			��ʼ������Э�顣
* @attention 
*@li ������APP��ӿ�(APP_Init)�е���,����Ҫ�ڵ��������������ο����ӿ�ǰ���á�
*@li ������Ӧ�ò��ʼ��֮ǰ���á�
*@li ���ó�ʼ���ӿں󣬱������ƥ���ȥ��ʼ���ӿڣ������ȥ��ʼ��ʧ�ܡ�
*@li ���ó�ʼ���ӿں��ٴ˵��ó�ʼ���ӿ�ǰ��������ȥ��ʼ����
* @param �ޡ�
* @retval #HI_ERR_SUCCESS ��ʼ���ɹ���
* @retval #����ֵ ��ʼ��ʧ�ܣ��μ�hi_error.h��
* @par Dependency:
* <ul><li>hi_mdm_upg.h: �ļ�����������������ӿڡ�</li></ul>
* @see �ޡ�
* @since HiMDL_V200R001C00SPC012
*/
HI_EAPI HI_U32 HI_MDM_InitCsgProtocol(HI_VOID);

/**
* @ingroup  hct_upg
* @brief  ����Э��ȥ��ʼ����
*
* @par   ����: 
		  ����Э��ȥ��ʼ����
* @attention �ޡ�
* @param �ޡ�
* @retval #HI_ERR_SUCCESS ȥ��ʼ���ɹ���
* @retval #����ֵ ȥ��ʼ��ʧ�ܣ��μ�hi_error.h��
* @par Dependency:
* <ul><li>hi_mdm_upg.h: �ļ�����������������ӿڡ�</li></ul>
* @see �ޡ�
* @since HiMDL_V200R001C00SPC012
*/
HI_EAPI HI_U32 HI_MDM_DeinitCsgProtocol(HI_VOID);

/**
* @ingroup  hct_upg
* @brief  ��ʼ������Э�顣
*
* @par   ����: 
			��ʼ������Э�顣
* @attention
*@li ������APP��ӿ�(APP_Init)�е���,����Ҫ�ڵ��������������ο����ӿ�ǰ���á�
*@li ������Ӧ�ò��ʼ��֮ǰ���á�
*@li ���ó�ʼ���ӿں󣬱������ƥ���ȥ��ʼ���ӿڣ������ȥ��ʼ��ʧ�ܡ�
*@li ���ó�ʼ���ӿں��ٴ˵��ó�ʼ���ӿ�ǰ��������ȥ��ʼ����
* @param �ޡ�
* @retval #HI_ERR_SUCCESS ��ʼ���ɹ���
* @retval #����ֵ ��ʼ��ʧ�ܣ��μ�hi_error.h��
* @par Dependency:
* <ul><li>hi_mdm_upg.h: �ļ�����������������ӿڡ�</li></ul>
* @see �ޡ�
* @since HiMDL_V200R001C00SPC012
*/
HI_EAPI HI_U32 HI_MDM_InitStdProtocol(HI_VOID);

/**
* @ingroup  hct_upg
* @brief  ����Э��ȥ��ʼ����
*
* @par   ����: 
		  ����Э��ȥ��ʼ����
* @attention �ޡ�
* @param �ޡ�
* @retval #HI_ERR_SUCCESS ȥ��ʼ���ɹ���
* @retval #����ֵ ȥ��ʼ��ʧ�ܣ��μ�hi_error.h��
* @par Dependency:
* <ul><li>hi_mdm_upg.h: �ļ�����������������ӿڡ�</li></ul>
* @see �ޡ�
* @since HiMDL_V200R001C00SPC012
*/
HI_EAPI HI_U32 HI_MDM_DeinitStdProtocol(HI_VOID);


HI_END_HEADER
#endif // __HI_MDM_UPG_H__
