/******************************************************************************
*             Copyright (C) 2018 - 2018, Hisilicon Tech. Co., Ltd.
*                           ALL RIGHTS RESERVED
* FileName: hi_mdm_network.h
* Description������������̫���ӿ�
*
* History:
* Version                  Date         DefectNum           Description
* V200R001C00SPC050B010    2018-11-08   DTS2018102905196    ���ӽ�����������PHYоƬ��Ϊ�͹���״̬�ӿڡ�
******************************************************************************/

#ifndef __HI_MDM_NETWORK_H__
#define __HI_MDM_NETWORK_H__

HI_START_HEADER

/**@defgroup hct_ETH ETH
 * @ingroup mdm
*/

/**
 * @ingroup hct_ETH
 * ·����Ϣ�������ֵ
 */
#define ROUTE_INFO_LEN_MAX          512

 /**
 * @ingroup hct_ETH
 * ��������ö��
 */
typedef enum
{
    HI_MDM_ETH_CFG_IDX_STA_CONN = 0,
    HI_MDM_ETH_CFG_IDX_STA_NDM,
    HI_MDM_ETH_CFG_IDX_STA_HSO,
    HI_MDM_ETH_CFG_IDX_MAX
}HI_MDM_ETH_CFG_IDX_E;

/**
 *@ingroup hct_ETH
 * PLC��MAC��ַ��IP��ַ����������ṹ
 */ 
typedef struct
{
    HI_U8 aucMacAddr[HI_PLC_MAC_ADDR_LEN];      /**< ͨ��վ���MAC��ַ*/
    HI_U8 aucIpAddr[HI_IPV4_ADDR_LEN];              /**<  ͨ��վ���IP��ַ  */
    HI_U8 aucNetMask[HI_IPV4_ADDR_LEN];           /**<  ͨ��վ�����������    */
    HI_U8 reserved[2];
}HI_MDM_NM_ETH_CFG_S;
/**
 *@ingroup hct_ETH
 * ��ȡͨ��ģ���PLC��MAC��ַ��IP��ַ����������ṹ
 */
typedef struct
{
    HI_MDM_NM_ETH_CFG_S astNetCfg[HI_MDM_ETH_CFG_IDX_MAX];
}HI_MDM_NM_ETH_QRY_IND_S;
 
/**
 *@ingroup hct_ETH
 * ����ͨ��ģ���PLC��MAC��ַ��IP��ַ��������������ṹ
 */
typedef struct
{
    HI_MDM_NM_ETH_CFG_S stCfg;
    HI_MDM_ETH_CFG_IDX_E enCfgIdx;
}HI_MDM_NM_ETH_SET_REQ_S;

// ��ǰ�汾��֧��
/**
 *@ingroup hct_ETH
 * ����ͨ��ģ���PLC��MAC��ַ��IP��ַ����������ṹ
 */
typedef struct
{
    HI_U32 ulRc;
}HI_MDM_NM_ETH_SET_IND_S;





#if !defined(PRODUCT_CFG_BUILD_HSO_CODE)

/**
 * @ingroup hct_ETH
 * ARP��Ϣ
*/
typedef struct
{
    HI_CHAR name[16]; /**< ���� */
    HI_U8 ipAddr[4];  /**< IP��ַ */
    HI_U8 macAddr[6]; /**< MAC��ַ */
    HI_U8 pad[2];     /**< ����λ */
}HI_ARP_ITEM_S;

/**
* @ingroup  hct_ETH
* @brief  ��ȡ�ɴ洢Arp���ݵ����������
*
* @par ����:
* ��ȡ�ɴ洢Arp���ݵ����������
* @attention ��֧�������е��ã���֧���ж��е��á�
*
* @param  �ޡ�
*
* @retval #HI_U32������ֵ ��ȡ����Arp��������
*
* @par Dependency:
* @li hi_mdm_network.h: �ýӿ��������ڵ�ͷ�ļ���
* @see  �ޡ�
* @since HiMDL_V200R001C00
*/

HI_EAPI HI_U32 HI_MDM_GetArpTableSize(HI_VOID);

/**
* @ingroup  hct_ETH
* @brief  ��ȡArp���е����ݡ�
*
* @par ����:
* ��ȡArp���е����ݡ�
* @attention ���øú���ǰȷ�����count��Ϊ�ա�
*
* @param  ItemInfo [OUT] ��ȡ����Arp���ݡ�
* @param count [OUT] ��ȡ��count��Arp���ݡ�
*
* @retval #HI_ERR_SUCCESS ��ȡ�ɹ���
* @retval #HI_ERR_INVALID_PARAMETER ��������
*
* @par Dependency:
* @li hi_mdm_network.h: �ýӿ��������ڵ�ͷ�ļ���
* @see  �ޡ�
* @since HiMDL_V200R001C00
*/

HI_EAPI HI_U32  HI_MDM_GetArpInfo(HI_ARP_ITEM_S ItemInfo[],HI_OUT HI_U32 *count);

/**
* @ingroup  hct_ETH
* @brief  ɾ��һ��Arp��Ϣ��
*
* @par ����:
* ɾ��һ��Arp��Ϣ��
* @attention �ޡ�
*
* @param  ipAddr [IN] ���ݸ�ip��ַɾ����ӦArp��Ϣ��
*
* @retval #HI_ERR_SUCCESS ɾ���ɹ���
* @retval #HI_ERR_FAILURE ɾ��ʧ�ܡ�
*
* @par Dependency:
* @li hi_mdm_network.h: �ýӿ��������ڵ�ͷ�ļ���
* @see  �ޡ�
* @since HiMDL_V200R001C00?
*/

HI_EAPI HI_U32  HI_MDM_ArpDel(HI_U8 ipAddr[HI_IPV4_ADDR_LEN]);

/**
 * @ingroup hct_ETH
 * ·������
 */
typedef enum
{
    HI_NET_DEV_PLC = 0, /**< PLC���� */
    HI_NET_DEV_ETH,     /**< ETH���� */
    HI_NET_DEV_MAX,     /**< �������ֵ */
}HI_NET_DEV_E;

/**
* @ingroup  hct_ETH
* @brief  ��ʼ����̫���ڡ�
*
* @par ����:
* ��ʼ����̫���ڡ�
* @attention 
*
* @retval #HI_ERR_SUCCESS ��ʼ���ɹ���
* @retval #HI_ERR_FAILURE ��ʼ��ʧ�ܡ�
*
* @par Dependency:
* @li hi_mdm_network.h: �ýӿ��������ڵ�ͷ�ļ���
* @see  �ޡ�
* @since HiMDL_V200R001C00
*/
HI_EAPI HI_U32 HI_MDM_EthInit(HI_VOID);


/**
* @ingroup  hct_ETH
* @brief  ��ȡ������̫������Ϣ��
*
* @par ����:
* ��ȡ������̫������Ϣ��
* @attention ��֧�������е��ã���֧���ж��е��á�
*
* @param pstEthCfgInfo [OUT] ��̫���ڵ����ò�����
*
* @retval #HI_ERR_SUCCESS ��ȡ�ɹ���
* @retval #HI_ERR_INVALID_PARAMETER ��������
* @retval #HI_ERR_FAILURE ��ȡʧ�ܡ�
*
* @par Dependency:
* @li hi_mdm_network.h: �ýӿ��������ڵ�ͷ�ļ���
* @see  �ޡ�
* @since HiMDL_V200R001C00
*/
HI_EAPI HI_U32 HI_MDM_GetEthConfig(HI_OUT HI_SYS_ETH_CONFIG_S* pstEthCfgInfo);

/**
* @ingroup  hct_ETH
* @brief  ���ñ�����̫������Ϣ��
*
* @par ����:
* ���ñ�����̫������Ϣ�����棬���粻��ʧ��
* @attention
* @li ��֧�������е��ã���֧���ж��е��á�
* @li ����豸Ĭ����pstEthCfgInfo->gatewayΪ0����ʾ����������ֵ��
*
* @param  pstEthCfgInfo [IN] ��̫�������ò�����
*
* @retval #HI_ERR_SUCCESS ���óɹ���
* @retval ����ֵ ���hi_errno.h��
*
* @par Dependency:
* @li hi_mdm_network.h: �ýӿ��������ڵ�ͷ�ļ���
* @see  �ޡ�
* @since HiMDL_V200R001C00
*/

HI_EAPI HI_U32 HI_MDM_SetEthConfig(HI_IN HI_SYS_ETH_CONFIG_S* pstEthCfgInfo);

/**
* @ingroup  hct_ETH
* @brief  ���·�ɡ�
*
* @par ����:
* ���·�ɡ�
* @attention
* @li ���粻���档
* @li ��֧�������е��ã���֧���ж��е��á�
*
* @param  enNetDev [IN] ѡ����ӵ�·��ΪPLC����ETH��
* @param  aucDstAddr [IN] Ŀ�������Ŀ������(������(aucDstAddr&aucNetMask)==aucDstAddr)��
* @param  aucNetMask [IN] ���롣
* @param aucGateway [IN] ����ֵ��
*
* @retval #HI_ERR_SUCCESS ��ӳɹ���
* @retval #HI_ERR_INVALID_PARAMETER ��������
* @retval #HI_ERR_FAILURE ���ʧ�ܡ�
*
* @par Dependency:
* @li hi_mdm_network.h: �ýӿ��������ڵ�ͷ�ļ���
* @see  �ޡ�
* @since HiMDL_V200R001C00
*/

HI_EAPI HI_U32 HI_MDM_AddRoute(HI_NET_DEV_E enNetDev, HI_U8 aucDstAddr[HI_IPV4_ADDR_LEN], HI_U8 aucNetMask[HI_IPV4_ADDR_LEN],
        HI_U8 aucGateway[HI_IPV4_ADDR_LEN]);

/**
* @ingroup  hct_ETH
* @brief  ɾ��·�ɡ�
*
* @par ����:
* ɾ��·�ɡ�
* @attention
* @li ���粻���档
* @li ��֧�������е��ã���֧���ж��е��á�
*
* @param  aucDstAddr [IN] ��ɾ��·�ɵ�ַ��
*
* @retval #HI_ERR_SUCCESS ��ӳɹ���
* @retval #HI_ERR_INVALID_PARAMETER ��������
*
* @par Dependency:
* @li hi_mdm_network.h: �ýӿ��������ڵ�ͷ�ļ���
* @see  �ޡ�
* @since HiMDL_V200R001C00
*/

HI_EAPI HI_U32 HI_MDM_DelRoute(HI_U8 aucDstAddr[HI_IPV4_ADDR_LEN]);

/**
* @ingroup  hct_ETH
* @brief  ��ȡ·����Ϣ��
*
* @par ����:
* ��ȡ·����Ϣ,��������Ŀ�ꡢ���ء��������롢�ӿ���Ϣ����Ϣ�ľ���ṹ�μ���Hi3911V200 API ����ָ��.doc����
* @attention ��֧�������е��ã���֧���ж��е��á�
*
* @param  buf [OUT] ·����Ϣ��
* @param  buf_len [IN] ·����Ϣ���ȡ�·����Ϣ���ȵ���������·�ɱ��������Ĵ�С������һ��Ϊ512�ֽڼ��ɡ�
*
* @retval #HI_ERR_SUCCESS ��ȡ�ɹ���
* @retval #HI_ERR_INVALID_PARAMETER ��������
*
* @par Dependency:
* @li hi_mdm_network.h: �ýӿ��������ڵ�ͷ�ļ���
* @see  �ޡ�
* @since HiMDL_V200R001C00
*/
HI_U32 HI_MDM_GetRouteInfo(HI_CHAR buf[ROUTE_INFO_LEN_MAX], HI_U32 buf_len);

/**
* @ingroup  hct_ETH
* @brief   ������������PHYоƬ��Ϊ�͹���״̬��
*
* @par  ����:
* ������������PHYоƬ��Ϊ�͹���״̬���Խ��͹��ġ�
* @attention
* li ���øýӿں����ڽ����ܹ�������������Իָ�������
* li ���øýӿ�ǰ�����ܹر�����ʱ�ӣ�����ýӿڽ�����ִ�гɹ���
*
* @param  �ޡ�
*
* @retval #HI_ERR_SUCCESS ���óɹ���
* @retval #HI_ERR_FAILURE ����ʧ�ܡ�
*
* @par Dependency:
* @li hi_mdm_network.h: �ýӿ��������ڵ�ͷ�ļ���
* @since HiMDL_V200R001C00
*/
HI_U32 HI_MDM_EthPhyPowerDown(HI_VOID);


#endif

HI_END_HEADER

#endif
