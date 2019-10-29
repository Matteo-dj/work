/******************************************************************************
*             Copyright (C) 2018 - 2018, Hisilicon Tech. Co., Ltd.
*                           ALL RIGHTS RESERVED
* FileName: hi_mdm_network.h
* Description：用于描述以太网接口
*
* History:
* Version                  Date         DefectNum           Description
* V200R001C00SPC050B010    2018-11-08   DTS2018102905196    增加将单板上网口PHY芯片置为低功耗状态接口。
******************************************************************************/

#ifndef __HI_MDM_NETWORK_H__
#define __HI_MDM_NETWORK_H__

HI_START_HEADER

/**@defgroup hct_ETH ETH
 * @ingroup mdm
*/

/**
 * @ingroup hct_ETH
 * 路由信息长度最大值
 */
#define ROUTE_INFO_LEN_MAX          512

 /**
 * @ingroup hct_ETH
 * 网口索引枚举
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
 * PLC的MAC地址和IP地址和子网掩码结构
 */ 
typedef struct
{
    HI_U8 aucMacAddr[HI_PLC_MAC_ADDR_LEN];      /**< 通信站点的MAC地址*/
    HI_U8 aucIpAddr[HI_IPV4_ADDR_LEN];              /**<  通信站点的IP地址  */
    HI_U8 aucNetMask[HI_IPV4_ADDR_LEN];           /**<  通信站点的子网掩码    */
    HI_U8 reserved[2];
}HI_MDM_NM_ETH_CFG_S;
/**
 *@ingroup hct_ETH
 * 获取通信模块的PLC的MAC地址和IP地址和子网掩码结构
 */
typedef struct
{
    HI_MDM_NM_ETH_CFG_S astNetCfg[HI_MDM_ETH_CFG_IDX_MAX];
}HI_MDM_NM_ETH_QRY_IND_S;
 
/**
 *@ingroup hct_ETH
 * 设置通信模块的PLC的MAC地址和IP地址和子网掩码请求结构
 */
typedef struct
{
    HI_MDM_NM_ETH_CFG_S stCfg;
    HI_MDM_ETH_CFG_IDX_E enCfgIdx;
}HI_MDM_NM_ETH_SET_REQ_S;

// 当前版本不支持
/**
 *@ingroup hct_ETH
 * 设置通信模块的PLC的MAC地址和IP地址和子网掩码结构
 */
typedef struct
{
    HI_U32 ulRc;
}HI_MDM_NM_ETH_SET_IND_S;





#if !defined(PRODUCT_CFG_BUILD_HSO_CODE)

/**
 * @ingroup hct_ETH
 * ARP信息
*/
typedef struct
{
    HI_CHAR name[16]; /**< 名字 */
    HI_U8 ipAddr[4];  /**< IP地址 */
    HI_U8 macAddr[6]; /**< MAC地址 */
    HI_U8 pad[2];     /**< 保留位 */
}HI_ARP_ITEM_S;

/**
* @ingroup  hct_ETH
* @brief  获取可存储Arp数据的最大条数。
*
* @par 描述:
* 获取可存储Arp数据的最大条数。
* @attention 仅支持任务中调用，不支持中断中调用。
*
* @param  无。
*
* @retval #HI_U32类型数值 获取到的Arp表容量。
*
* @par Dependency:
* @li hi_mdm_network.h: 该接口声明所在的头文件。
* @see  无。
* @since HiMDL_V200R001C00
*/

HI_EAPI HI_U32 HI_MDM_GetArpTableSize(HI_VOID);

/**
* @ingroup  hct_ETH
* @brief  获取Arp表中的数据。
*
* @par 描述:
* 获取Arp表中的数据。
* @attention 调用该函数前确保入参count不为空。
*
* @param  ItemInfo [OUT] 获取到的Arp数据。
* @param count [OUT] 获取到count条Arp数据。
*
* @retval #HI_ERR_SUCCESS 获取成功。
* @retval #HI_ERR_INVALID_PARAMETER 参数错误。
*
* @par Dependency:
* @li hi_mdm_network.h: 该接口声明所在的头文件。
* @see  无。
* @since HiMDL_V200R001C00
*/

HI_EAPI HI_U32  HI_MDM_GetArpInfo(HI_ARP_ITEM_S ItemInfo[],HI_OUT HI_U32 *count);

/**
* @ingroup  hct_ETH
* @brief  删除一条Arp信息。
*
* @par 描述:
* 删除一条Arp信息。
* @attention 无。
*
* @param  ipAddr [IN] 根据该ip地址删除对应Arp信息。
*
* @retval #HI_ERR_SUCCESS 删除成功。
* @retval #HI_ERR_FAILURE 删除失败。
*
* @par Dependency:
* @li hi_mdm_network.h: 该接口声明所在的头文件。
* @see  无。
* @since HiMDL_V200R001C00?
*/

HI_EAPI HI_U32  HI_MDM_ArpDel(HI_U8 ipAddr[HI_IPV4_ADDR_LEN]);

/**
 * @ingroup hct_ETH
 * 路由类型
 */
typedef enum
{
    HI_NET_DEV_PLC = 0, /**< PLC类型 */
    HI_NET_DEV_ETH,     /**< ETH类型 */
    HI_NET_DEV_MAX,     /**< 类型最大值 */
}HI_NET_DEV_E;

/**
* @ingroup  hct_ETH
* @brief  初始化以太网口。
*
* @par 描述:
* 初始化以太网口。
* @attention 
*
* @retval #HI_ERR_SUCCESS 初始化成功。
* @retval #HI_ERR_FAILURE 初始化失败。
*
* @par Dependency:
* @li hi_mdm_network.h: 该接口声明所在的头文件。
* @see  无。
* @since HiMDL_V200R001C00
*/
HI_EAPI HI_U32 HI_MDM_EthInit(HI_VOID);


/**
* @ingroup  hct_ETH
* @brief  获取本地以太网口信息。
*
* @par 描述:
* 获取本地以太网口信息。
* @attention 仅支持任务中调用，不支持中断中调用。
*
* @param pstEthCfgInfo [OUT] 以太网口的配置参数。
*
* @retval #HI_ERR_SUCCESS 获取成功。
* @retval #HI_ERR_INVALID_PARAMETER 参数错误。
* @retval #HI_ERR_FAILURE 获取失败。
*
* @par Dependency:
* @li hi_mdm_network.h: 该接口声明所在的头文件。
* @see  无。
* @since HiMDL_V200R001C00
*/
HI_EAPI HI_U32 HI_MDM_GetEthConfig(HI_OUT HI_SYS_ETH_CONFIG_S* pstEthCfgInfo);

/**
* @ingroup  hct_ETH
* @brief  设置本地以太网口信息。
*
* @par 描述:
* 设置本地以太网口信息并保存，掉电不丢失。
* @attention
* @li 仅支持任务中调用，不支持中断中调用。
* @li 如果设备默认网pstEthCfgInfo->gateway为0，表示不更新网关值。
*
* @param  pstEthCfgInfo [IN] 以太网口配置参数。
*
* @retval #HI_ERR_SUCCESS 设置成功。
* @retval 其他值 详见hi_errno.h。
*
* @par Dependency:
* @li hi_mdm_network.h: 该接口声明所在的头文件。
* @see  无。
* @since HiMDL_V200R001C00
*/

HI_EAPI HI_U32 HI_MDM_SetEthConfig(HI_IN HI_SYS_ETH_CONFIG_S* pstEthCfgInfo);

/**
* @ingroup  hct_ETH
* @brief  添加路由。
*
* @par 描述:
* 添加路由。
* @attention
* @li 掉电不保存。
* @li 仅支持任务中调用，不支持中断中调用。
*
* @param  enNetDev [IN] 选择添加的路由为PLC还是ETH。
* @param  aucDstAddr [IN] 目标网络或目标主机(需满足(aucDstAddr&aucNetMask)==aucDstAddr)。
* @param  aucNetMask [IN] 掩码。
* @param aucGateway [IN] 网关值。
*
* @retval #HI_ERR_SUCCESS 添加成功。
* @retval #HI_ERR_INVALID_PARAMETER 参数错误。
* @retval #HI_ERR_FAILURE 添加失败。
*
* @par Dependency:
* @li hi_mdm_network.h: 该接口声明所在的头文件。
* @see  无。
* @since HiMDL_V200R001C00
*/

HI_EAPI HI_U32 HI_MDM_AddRoute(HI_NET_DEV_E enNetDev, HI_U8 aucDstAddr[HI_IPV4_ADDR_LEN], HI_U8 aucNetMask[HI_IPV4_ADDR_LEN],
        HI_U8 aucGateway[HI_IPV4_ADDR_LEN]);

/**
* @ingroup  hct_ETH
* @brief  删除路由。
*
* @par 描述:
* 删除路由。
* @attention
* @li 掉电不保存。
* @li 仅支持任务中调用，不支持中断中调用。
*
* @param  aucDstAddr [IN] 待删除路由地址。
*
* @retval #HI_ERR_SUCCESS 添加成功。
* @retval #HI_ERR_INVALID_PARAMETER 参数错误。
*
* @par Dependency:
* @li hi_mdm_network.h: 该接口声明所在的头文件。
* @see  无。
* @since HiMDL_V200R001C00
*/

HI_EAPI HI_U32 HI_MDM_DelRoute(HI_U8 aucDstAddr[HI_IPV4_ADDR_LEN]);

/**
* @ingroup  hct_ETH
* @brief  获取路由信息。
*
* @par 描述:
* 获取路由信息,包括网络目标、网关、网络掩码、接口信息。信息的具体结构参见《Hi3911V200 API 开发指南.doc》。
* @attention 仅支持任务中调用，不支持中断中调用。
*
* @param  buf [OUT] 路由信息。
* @param  buf_len [IN] 路由信息长度。路由信息长度的设置依据路由表数据量的大小而定，一般为512字节即可。
*
* @retval #HI_ERR_SUCCESS 获取成功。
* @retval #HI_ERR_INVALID_PARAMETER 参数错误。
*
* @par Dependency:
* @li hi_mdm_network.h: 该接口声明所在的头文件。
* @see  无。
* @since HiMDL_V200R001C00
*/
HI_U32 HI_MDM_GetRouteInfo(HI_CHAR buf[ROUTE_INFO_LEN_MAX], HI_U32 buf_len);

/**
* @ingroup  hct_ETH
* @brief   将单板上网口PHY芯片置为低功耗状态。
*
* @par  描述:
* 将单板上网口PHY芯片置为低功耗状态，以降低功耗。
* @attention
* li 调用该接口后，网口将不能工作，重启后可以恢复工作。
* li 调用该接口前，不能关闭网口时钟，否则该接口将不会执行成功。
*
* @param  无。
*
* @retval #HI_ERR_SUCCESS 设置成功。
* @retval #HI_ERR_FAILURE 设置失败。
*
* @par Dependency:
* @li hi_mdm_network.h: 该接口声明所在的头文件。
* @since HiMDL_V200R001C00
*/
HI_U32 HI_MDM_EthPhyPowerDown(HI_VOID);


#endif

HI_END_HEADER

#endif
