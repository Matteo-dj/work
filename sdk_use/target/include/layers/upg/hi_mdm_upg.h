/******************************************************************************
*             Copyright (C) 2018 - 2018, Hisilicon Tech. Co., Ltd.
*                           ALL RIGHTS RESERVED
* FileName: hi_mdm_upg.h
* Description：升级接口，用于实现升级功能。
*
* History:
* Version              Date         DefectNum           Description
* V200R001C00SPC010    2018-03-31   NA                  第一次版本发布。
* V200R001C00SPC011    2018-05-10   DTS2018010310207    新增2个升级接口：获取升级参数接口(HI_MDM_UPG_GetCfg)和设置升级参数接口(HI_MDM_UPG_SetCfg)。
* V200R001C00SPC012    2018-07-12   DTS2018071204569    删除接口HI_MDM_UPG_SetProtoInfo,新增接口HI_MDM_UPG_SetPrivateProto。
* V200R001C00SPC012    2018-07-12   DTS2018071204569    新增协议注册相关接口。
* V200R001C00SPC012    2018-07-12   DTS2018071204569    新增消息ID:HI_MDM_UPG_TRANS_FILE、HI_MDM_UPG_QUERY_INFO。
* V200R001C00SPC050    2019-01-21   DTS2018071204569    修改设置海思协议接口及升级时隙接口。
******************************************************************************/
/**@defgroup hct_upg 升级接口
 * @ingroup mdm
 */

#ifndef __HI_MDM_UPG_H__
#define __HI_MDM_UPG_H__

HI_START_HEADER

/**
 *@ingroup hct_upg
 * 海思升级文件长度最小值。实际海思升级文件长度不小于该值。可供APP校验升级文件用。
 */
#define HI_MDM_UPG_FILE_MIN_SIZE                       HI_UPG_FILE_HEADER_SIZE
/**
 *@ingroup hct_upg
 * 全网升级文件头长度。
 */
#define HI_MDM_UPG_CCO_FILE_BUFFER_RESERVED_SIZE       HI_MDM_UPG_FILE_MIN_SIZE
/**
 *@ingroup hct_upg
 * 非全网升级，升级文件长度最大值。非全网升级，升级文件长度不能大于该值，否则无法启动升级流程。
 */
#define HI_MDM_UPG_FILE_MAX_SIZE                       PRODUCT_CFG_UPG_FILE_MAX_SIZE
/**
 *@ingroup hct_upg
 * CCO升级文件长度最大值。CCO单点升级时，升级文件长度不能大于该值，否则无法启动升级流程。
 */
#define HI_MDM_UPG_CCO_FILE_MAX_SIZE                   PRODUCT_CFG_CCO_UPG_FILE_MAX_SIZE
/**
 *@ingroup hct_upg
 * 海思STA升级文件长度最大值。STA单点升级、全网STA升级、列表升级（列表中只包含STA）或设备升级时，升级文件长度不能大于该值，否则无法启动升级流程。
 */
#define HI_MDM_UPG_STA_FILE_MAX_SIZE                   PRODUCT_CFG_STA_UPG_FILE_MAX_SIZE

/**
 *@ingroup hct_upg
 * 非海思STA升级文件长度最大值(不小于海思STA升级文件长度最大值)。升级文件长度不能大于该值，否则无法启动升级流程。
 */
#define HI_MDM_UPG_STA_FILE_RAM_MAX_SIZE               PRODUCT_CFG_UPGFILE_CACHE_CCO_RAM_SIZE

/**
 *@ingroup hct_upg
 * 全网升级文件长度最大值。全网升级或列表升级（列表中包含CCO、STA）时，升级文件长度不能大于该值，否则无法启动升级流程。
 */
#define HI_MDM_UPG_CCO_STA_FILE_MAX_SIZE               (HI_MDM_UPG_CCO_FILE_MAX_SIZE+HI_MDM_UPG_STA_FILE_MAX_SIZE+HI_MDM_UPG_CCO_FILE_BUFFER_RESERVED_SIZE)
 /**
 *@ingroup hct_upg
 * 升级状态查询请求参数，表示上报所有升级信息：\n
 * @li 单点升级时，查询升级状态，除了想了解当前的升级状态，还想了解其他信息（例如：通信模块接收升级文件长度、升级文件版本号、模块升级耗时等），可设置成该参数。
 * @li 全网升级时，查询升级状态，除了想了解当前的升级状态，还想了解网络中所有模块所有升级信息，或所有模块升级统计信息，可设置成此参数。 
 */
#define HI_MDM_UPG_STATUS_REQ_OPT_NA      0
 /**
 *@ingroup hct_upg
 * 升级状态查询请求参数，表示上报基本升级信息。查询升级状态时，只想了解当前的升级状态，可设置成该参数。
 */
#define HI_MDM_UPG_STATUS_REQ_OPT_BS      1
 /**
 *@ingroup hct_upg
 * 升级成功状态。
 */
#define HI_MDM_UPG_STATUS_OK              0
/**
 *@ingroup hct_upg
 * 升级验证状态。
 */
#define HI_MDM_UPG_STATUS_END             1
/**
 *@ingroup hct_upg
 * 未知升级状态。当前不处于升级过程中。
 */
#define HI_MDM_UPG_STATUS_UNKNOWN         8
/**
 *@ingroup hct_upg
 * 全网正在停止升级状态。
 */
#define HI_MDM_UPG_STATUS_STOPPING        16
/**
 *@ingroup hct_upg
 * 正在接收文件升级状态。
 */
#define HI_MDM_UPG_STATUS_PROCESS        0xF0
/**
 *@ingroup hct_upg
 * 正在转发文件状态。
 */
#define HI_MDM_UPG_STATUS_TRANSMIT       0xF1
/**
 *@ingroup hct_upg
 * 升级失败状态。
 */
#define HI_MDM_UPG_STATUS_FAIL           0xFF

/**
* @ingroup  hct_upg
* @brief  该消息ID实现升级状态查询。
*
* @par   描述:
            1)先调用HI_MDM_RegisterUpgMsgNotify注册该消息ID的回调函数；\n
			2)通过HI_MDM_UPG_MsgSend发送该消息给UPG；\n
			3)UPG处理完后调用用户注册的回调函数上报处理结果。
* @attention	无。
* @param   ulMsgId [IN]  升级状态查询消息ID，取值：HI_MDM_UPG_STATUS。
* @param   pucPacket [IN]  升级状态查询请求参数(HI_MDM_UPG_STATUS_REQ_S): \n
* @li 当stReqStatus.ulOption取值为HI_MDM_UPG_STATUS_REQ_OPT_NA：全网升级回复消息ID为HI_MDM_UPG_STATUS和HI_MDM_UPG_STAT_INFO_IND（0x36C0），
						   参数分别定义为HI_MDM_UPG_STATUS_IND_S、HI_MDM_UPG_STAT_INFO_IND_S 。
* @li 当stReqStatus.ulOption取值为HI_MDM_UPG_STATUS_REQ_OPT_NA：单点升级回复消息ID为HI_MDM_UPG_STATUS，参数为HI_MDM_UPG_STATUS_IND_S 。
* @li 当stReqStatus.ulOption取值为HI_MDM_UPG_STATUS_REQ_OPT_BS: 升级回复消息ID为HI_MDM_UPG_STATUS_BS_IND（0x36C1），参数定义为HI_MDM_UPG_STATUS_BS_IND_S。
* @param   ulPacketSize [IN]  升级状态查询请求参数结构体大小。
* @retval  ulMsgId 升级状态查询消息ID，取值范围：HI_MDM_UPG_STATUS、HI_MDM_UPG_STAT_INFO_IND、HI_MDM_UPG_STATUS_BS_IND。
* @retval  pucPacket 升级状态查询应答参数，分别对应ulMsgId的结构体为：HI_MDM_UPG_STATUS_IND_S、HI_MDM_UPG_STAT_INFO_IND_S、HI_MDM_UPG_STATUS_BS_IND_S。
* @retval  usPacketSize 升级状态查询应答参数结构体大小。
* @par Dependency:
* <ul><li>hi_mdm_upg.h: 文件用于描述升级对外接口。</li></ul>
* @see HI_MDM_RegisterUpgMsgNotify | HI_MDM_UPG_MsgSend。
* @since HiMDL_V200R001C00
*/
#define HI_MDM_UPG_STATUS                   0x3660

/**
* @ingroup  hct_upg
* @brief  该消息ID用于APP转发文件传输命令业务报文给UPG。
*
* @par   描述:
			通过HI_MDM_UPG_MsgSend发送该消息给UPG；\n
* @attention	无。
* @param   ulMsgId [IN]  转发升级报文消息ID，取值：HI_MDM_UPG_TRANS_FILE。
* @param   pucPacket [IN]  转发升级报文内容。
* @param   ulPacketSize [IN]  转发升级报文大小。
* @par Dependency:
* <ul><li>hi_mdm_upg.h: 文件用于描述升级对外接口。</li></ul>
* @see HI_MDM_UPG_MsgSend。
* @since HiMDL_V200R001C00SPC012
*/
#define HI_MDM_UPG_TRANS_FILE               0x3661

/**
* @ingroup  hct_upg
* @brief  该消息ID实现启动升级。
*
* @par   描述:
            1)先调用HI_MDM_RegisterUpgMsgNotify注册该消息ID的回调函数；\n
			2)通过HI_MDM_UPG_MsgSend发送该消息给UPG；\n
			3)UPG处理完后调用用户注册的回调函数上报处理结果。
* @attention	无。
* @param   ulMsgId [IN]  升级开始消息ID，取值：HI_MDM_UPG_START。
* @param   pucPacket [IN]  升级开始请求参数(HI_MDM_UPG_START_REQ_S)。
* @param   ulPacketSize [IN]  升级开始请求参数结构体大小。
* @retval  ulMsgId 升级开始消息ID，取值：HI_MDM_UPG_START。
* @retval  pucPacket 升级开始应答参数，对应的结构体为HI_MDM_UPG_START_IND_S。
* @retval  usPacketSize 升级开始应答参数结构体大小。
* @par Dependency:
* <ul><li>hi_mdm_upg.h: 文件用于描述升级对外接口。</li></ul>
* @see HI_MDM_RegisterUpgMsgNotify | HI_MDM_UPG_MsgSend。
* @since HiMDL_V200R001C00
*/
#define HI_MDM_UPG_START                    0x3662

/**
* @ingroup  hct_upg
* @brief  该消息ID实现停止升级。
*
* @par   描述:
            1)先调用HI_MDM_RegisterUpgMsgNotify注册该消息ID的回调函数；\n
			2)通过HI_MDM_UPG_MsgSend发送该消息给UPG；\n
			3)UPG处理完后调用用户注册的回调函数上报处理结果。
* @attention	无。
* @param   ulMsgId [IN]  升级停止消息ID，取值：HI_MDM_UPG_STOP。
* @param   pucPacket [IN]  升级停止请求参数(HI_MDM_UPG_STOP_REQ_S)。
* @param   ulPacketSize [IN]  升级停止请求参数结构体大小。
* @retval  ulMsgId 升级停止消息ID，取值：HI_MDM_UPG_STOP。
* @retval  pucPacket 升级停止应答参数，对应的结构体为HI_MDM_UPG_STOP_IND_S。
* @retval  usPacketSize 升级停止应答参数结构体大小。
* @par Dependency:
* <ul><li>hi_mdm_upg.h: 文件用于描述升级对外接口。</li></ul>
* @see HI_MDM_RegisterUpgMsgNotify | HI_MDM_UPG_MsgSend。
* @since HiMDL_V200R001C00
*/
#define HI_MDM_UPG_STOP                     0x3663

/**
* @ingroup  hct_upg
* @brief  该消息ID用于APP转发从节点信息查询命令业务报文给UPG。
*
* @par   描述:
            通过HI_MDM_UPG_MsgSend发送该消息给UPG。
* @attention    无。
* @param   ulMsgId [IN]  转发从节点信息查询消息ID，取值：HI_MDM_UPG_QUERY_INFO。
* @param   pucPacket [IN]  转发升级报文内容。
* @param   ulPacketSize [IN]  转发升级报文大小。
* @par Dependency:
* <ul><li>hi_mdm_upg.h: 文件用于描述升级对外接口。</li></ul>
* @see HI_MDM_UPG_MsgSend。
* @since HiMDL_V200R001C00SPC012
*/
#define HI_MDM_UPG_QUERY_INFO               0x3666

/**
* @ingroup  hct_upg
* @brief  该消息ID实现升级状态统计信息应答。
*
* @par   描述:
            1)先调用HI_MDM_RegisterUpgMsgNotify注册该消息ID的回调函数；\n
			2)通过HI_MDM_UPG_MsgSend发送该消息给UPG；\n
			3)UPG处理完后调用用户注册的回调函数上报处理结果。
* @attention	无。
* @param   ulMsgId [IN]  升级状态查询消息ID，取值：HI_MDM_UPG_STATUS。
* @param   pucPacket [IN]  升级状态查询请求参数(HI_MDM_UPG_STATUS_REQ_S):
						   当stReqStatus.ulOption取值为HI_MDM_UPG_STATUS_REQ_OPT_NA：全网升级状态统计信息回复消息为HI_MDM_UPG_STAT_INFO_IND，
						   参数分别定义为HI_MDM_UPG_STAT_INFO_IND_S 。
* @param   ulPacketSize [IN]  升级状态查询请求参数结构体大小。
* @retval  ulMsgId 升级状态统计信息消息ID，取值：HI_MDM_UPG_STAT_INFO_IND。
* @retval  pucPacket 升级状态统计信息应答参数，对应的结构体为HI_MDM_UPG_STAT_INFO_IND_S。
* @retval  usPacketSize 升级状态统计信息应答参数结构体大小。
* @par Dependency:
* <ul><li>hi_mdm_upg.h: 文件用于描述升级对外接口。</li></ul>
* @see HI_MDM_RegisterUpgMsgNotify | HI_MDM_UPG_MsgSend。
* @since HiMDL_V200R001C00
*/
#define HI_MDM_UPG_STAT_INFO_IND            0x36C0

/**
* @ingroup  hct_upg
* @brief  该消息ID实现升级状态基本信息应答。
*
* @par   描述:
            1)先调用HI_MDM_RegisterUpgMsgNotify注册该消息ID的回调函数；\n
			2)通过HI_MDM_UPG_MsgSend发送该消息给UPG；\n
			3)UPG处理完后调用用户注册的回调函数上报处理结果。
* @attention	无。
* @param   ulMsgId [IN]  升级状态查询消息ID，取值：HI_MDM_UPG_STATUS。
* @param   pucPacket [IN]  升级状态查询请求参数(HI_MDM_UPG_STATUS_REQ_S):
						   当stReqStatus.ulOption取值为HI_MDM_UPG_STATUS_REQ_OPT_BS: 升级回复消息为HI_MDM_UPG_STATUS_BS_IND（0x36C1），参数定义为HI_MDM_UPG_STATUS_BS_IND_S。
* @param   ulPacketSize [IN]  升级状态查询请求参数结构体大小。
* @retval  ulMsgId 升级状态基本信息消息ID，取值范围：HI_MDM_UPG_STATUS_BS_IND。
* @retval  pucPacket 升级状态基本信息应答参数，对应ulMsgId的结构体为：HI_MDM_UPG_STATUS_BS_IND_S。
* @retval  usPacketSize 升级状态基本信息应答参数结构体大小。
* @par Dependency:
* <ul><li>hi_mdm_upg.h: 文件用于描述升级对外接口。</li></ul>
* @see HI_MDM_RegisterUpgMsgNotify | HI_MDM_UPG_MsgSend。
* @since HiMDL_V200R001C00
*/
#define HI_MDM_UPG_STATUS_BS_IND            0x36C1

/**
* @ingroup  hct_upg
* @brief  该消息ID实现启动设备升级
*
* @par   描述:
            1)先调用HI_MDM_RegisterUpgMsgNotify注册该消息ID的回调函数；\n
			2)通过HI_MDM_UPG_MsgSend发送该消息给UPG；\n
			3)UPG处理完后调用用户注册的回调函数上报处理结果。
* @attention	无。
* @param   ulMsgId [IN]  设备升级开始消息ID，取值：HI_MDM_DEVICE_UPG_START。
* @param   pucPacket [IN]  设备升级开始请求参数(HI_MDM_DEVICE_UPG_START_REQ_S)。
* @param   ulPacketSize [IN]  设备升级开始请求参数对结构体大小。
* @retval  ulMsgId 设备升级开始消息ID，取值：HI_MDM_DEVICE_UPG_START。
* @retval  pucPacket 设备升级开始应答参数，对应的结构体为HI_MDM_DEVICE_UPG_START_IND_S。
* @retval  usPacketSize 设备升级开始应答参数结构体大小。
* @par Dependency:
* <ul><li>hi_mdm_upg.h: 文件用于描述升级对外接口。</li></ul>
* @see HI_MDM_RegisterUpgMsgNotify | HI_MDM_UPG_MsgSend。
* @since HiMDL_V200R001C00
*/
#define HI_MDM_DEVICE_UPG_START             0x36D0

/**
* @ingroup  hct_upg
* @brief  该消息ID实现停止设备升级（该功能暂不支持）。
*
* @par   描述:
            1)先调用HI_MDM_RegisterUpgMsgNotify注册该消息ID的回调函数；\n
			2)通过HI_MDM_UPG_MsgSend发送该消息给UPG；\n
			3)UPG处理完后调用用户注册的回调函数上报处理结果。
* @attention	无。
* @param   ulMsgId [IN]  设备升级停止消息ID，取值：HI_MDM_DEVICE_UPG_STOP。
* @param   pucPacket [IN]  设备升级停止请求参数(HI_MDM_DEVICE_UPG_STOP_REQ_S)。
* @param   ulPacketSize [IN]  设备升级停止请求参数结构体大小。
* @retval  ulMsgId 设备升级停止消息ID，取值：HI_MDM_DEVICE_UPG_STOP。
* @retval  pucPacket 设备升级停止应答参数，对应的结构体为HI_MDM_DEVICE_UPG_STOP_IND_S。
* @retval  usPacketSize 设备升级停止应答参数结构体大小。
* @par Dependency:
* <ul><li>hi_mdm_upg.h: 文件用于描述升级对外接口。</li></ul>
* @see HI_MDM_RegisterUpgMsgNotify | HI_MDM_UPG_MsgSend。
* @since HiMDL_V200R001C00
*/
#define HI_MDM_DEVICE_UPG_STOP              0x36D1

/**
* @ingroup  hct_upg
* @brief  该函数实现查询设备升级状态。
*
* @par   描述:
            1)先调用HI_MDM_RegisterUpgMsgNotify注册该消息ID的回调函数；\n
			2)通过HI_MDM_UPG_MsgSend发送该消息给UPG；\n
			3)UPG处理完后调用用户注册的回调函数上报处理结果。
* @attention	无
* @param   ulMsgId [IN]  设备升级状态查询消息ID，取值：HI_MDM_DEVICE_UPG_STATUS。
* @param   pucPacket [IN]  设备升级状态查询请求参数(HI_MDM_DEVICE_UPG_STATUS_REQ_S)。
* @param   ulPacketSize [IN]  设备升级状态查询请求参数结构体大小。
* @retval  ulMsgId 设备升级状态查询消息ID，取值：HI_MDM_DEVICE_UPG_STATUS。
* @retval  pucPacket 设备升级状态查询应答参数，对应的结构体为HI_MDM_DEVICE_UPG_STATUS_IND_S。
* @retval  usPacketSize 设备升级状态查询应答参数结构体大小。
* @par Dependency:
* <ul><li>hi_mdm_upg.h: 文件用于描述升级对外接口。</li></ul>
* @see HI_MDM_RegisterUpgMsgNotify | HI_MDM_UPG_MsgSend。
* @since HiMDL_V200R001C00
*/
#define HI_MDM_DEVICE_UPG_STATUS            0x36D2

/**
 *@ingroup hct_upg
 * 升级选项：不指定升级对象，默认给单板自己升级，由升级文件的Section ID是否匹配来决定是否可以升级。
 * 用法：HI_UPG_REQ_OPTION_NA | HI_UPG_VER_UPDATE_OPTION_START。
 */
#define HI_UPG_REQ_OPTION_NA         0x01

/**
 *@ingroup hct_upg
 * 升级选项：仅升级CCO。
 * 用法：HI_UPG_REQ_OPTION_CCO | HI_UPG_VER_UPDATE_OPTION_START。
 */
#define HI_UPG_REQ_OPTION_CCO        0x02

/**
 *@ingroup hct_upg
 * 升级选项：仅升级STA，不升级CCO。
 * 用法：HI_UPG_REQ_OPTION_STA | HI_UPG_VER_UPDATE_OPTION_START。
 */
#define HI_UPG_REQ_OPTION_STA        0x04

/**
 *@ingroup hct_upg
 * 升级选项：全网升级，即升级对象包括CCO和STA。
 * 用法：HI_UPG_REQ_OPTION_BROADCAST | HI_UPG_VER_UPDATE_OPTION_START。
 */
#define HI_UPG_REQ_OPTION_BROADCAST  0x08

/**
 *@ingroup hct_upg
 * 升级选项：单点升级。
 * 用法：HI_UPG_REQ_OPTION_UNICAST | HI_UPG_VER_UPDATE_OPTION_START。
 */
#define HI_UPG_REQ_OPTION_UNICAST    0x10

/**
 *@ingroup hct_upg
 * 升级选项：设备升级。
 * 用法：HI_UPG_REQ_OPTION_DEVICE | HI_UPG_REQ_OPTION_BROADCAST | HI_UPG_VER_UPDATE_OPTION_START。
 */
#define HI_UPG_REQ_OPTION_DEVICE     0x0100

/**
 *@ingroup hct_upg
 * 升级选项：列表升级。
 * @li 列表升级站点包含CCO和STA用法：HI_UPG_REQ_OPTION_LIST | HI_UPG_REQ_OPTION_BROADCAST | HI_UPG_VER_UPDATE_OPTION_START。
 * @li 列表升级站点只包含STA用法：HI_UPG_REQ_OPTION_LIST | HI_UPG_REQ_OPTION_STA | HI_UPG_VER_UPDATE_OPTION_START。
 */
#define HI_UPG_REQ_OPTION_LIST       0x0200

/**
 *@ingroup hct_upg
 * 升级选项：开始升级。
 * 用法：HI_UPG_REQ_ | HI_UPG_VER_UPDATE_OPTION_START。
 */
#define HI_UPG_VER_UPDATE_OPTION_START      0x20

/**
 *@ingroup hct_upg
 * 非全网升级CCO升级STA方式。
 */
#define HI_MDM_UPGRADE_NA       0x00

/**
 *@ingroup hct_upg
 * CCO通过全网方式升级STA。
 */
#define HI_MDM_UPGRADE_ALL      0x01
/**
 *@ingroup hct_upg
 * CCO通过列表方式升级STA。
 */
#define HI_MDM_UPGRADE_LIST     0x02

 /**
* @ingroup  hct_upg
* @brief  用户注册升级消息ID对应的回调函数。
*
* @par   描述:
            用户注册升级消息ID对应的回调函数。
* @attention	无。
* @param   ulId [IN]  消息ID。
* @param   pucPacket [IN]  升级应答参数。
* @param   usPacketSize [IN]  升级应答参数结构体大小。
* @retval #HI_ERR_SUCCESS 用户处理成功。
* @retval #其他值 用户处理失败，参见hi_error.h。
* @par Dependency:
* <ul><li>hi_mdm_upg.h: 文件用于描述升级对外接口。</li></ul>
* @see 
* @since HiMDL_V200R001C00
*/
typedef HI_U32 (*HI_MDM_UPG_PROC_MSG_FUN)(HI_U32 ulId, HI_PBYTE pucPacket, HI_U16 usPacketSize);

/**
 *@ingroup hct_upg
 * 升级状态查询请求参数。
 */
typedef struct
{
    HI_U32 ulOption;  /**<查询升级状态信息类型。取值为HI_MDM_UPG_STATUS_REQ_OPT_BS表示仅上报升级基本状态信息。取其他值表示查询所有升级信息。*/
    HI_U32 blk_per_pkt;  /**<查询参数，未使用。*/
} HI_UPG_STATUS_REQ_S;

/**
 *@ingroup hct_upg
 * 开始升级请求参数。
 */
typedef struct
{
    HI_U32 ulOption;     /**<升级选项，必须带HI_UPG_VER_UPDATE_OPTION_START。可以或上以下值：\n 
	                    HI_UPG_REQ_OPTION_CCO表示仅升级CCO模块，所有STA模块都不升级。\n
						HI_UPG_REQ_OPTION_STA表示仅升级所有STA模块，不升级CCO模块。\n
						HI_UPG_REQ_OPTION_BROADCAST表示全网升级。 \n
						HI_UPG_REQ_OPTION_UNICAST表示单点升级，即仅升级接收到升级请求的模块，不再传输给其他模块。\n 
						HI_UPG_REQ_OPTION_DEVICE表示设备升级，即为STA下挂设备升级，STA收到升级文件之后，需要转发给下挂设备；CCO上配合HI_UPG_REQ_OPTION_BROADCAST一起使用。\n
						HI_UPG_REQ_OPTION_LIST表示列表升级，即为选定的CCO和STA升级或为选定的STA升级：选定的站点中包含CCO和STA，则配合HI_UPG_REQ_OPTION_BROADCAST使用；选定的列表中只包含STA，则配合HI_UPG_REQ_OPTION_STA使用。*/
    HI_U32 ulUpgId;     /**<升级ID，ID号在升级过程中必须唯一。    */
    HI_U32 ulFileLen;     /**<升级文件总长度，单位：byte。全网升级文件长度最大值HI_MDM_UPG_CCO_STA_FILE_MAX_SIZE。非全网升级升级文件长度最大值HI_MDM_UPG_FILE_MAX_SIZE。    */
    HI_U32 ulBlkNum;     /**<固定为1。    */
    HI_U32 ulUpgTimeWindow;     /**<升级开始到结束的时间窗，单位：min。 该参数设为0时，使用默认值360min。  */
    HI_U32 ulFileCrc;     /**<文件CRC，调用HI_MDM_Crc32接口计算。 */
    HI_NV_FTM_UPG_CFG_S stUpgCfg;     /**<升级参数结构体，默认填0。    */
} HI_UPG_START_REQ_S;

/**
 *@ingroup hct_upg
 * STA给CCO应答开始升级参数。
 */
typedef struct
{
    HI_U32 ulRc;     /**<结果码：HI_ERR_SUCCESS表示成功，其它为错误码。*/
    HI_U32 ulUpgId;     /**<升级ID。*/
    HI_U8 mac[HI_PLC_MAC_ADDR_LEN];  /**<通信模块的MAC地址（PLC的MAC地址）。  */
    HI_U8 pad[2];
} HI_UPG_START_IND_S;

/**
 *@ingroup hct_upg
 * 开始升级请求参数中列表信息。
 */
typedef struct
{
    HI_U16 usStaNum;     /**<站点列表中站点个数，最大取值为2047。*/
    HI_U16 padding;
}HI_UPG_STA_LIST_INFO_S;

/**
 *@ingroup hct_upg
 * 停止升级请求参数。
 */
typedef struct
{
    HI_U32 ulUpgId;     /**<升级ID。请求停止升级的ulUpgId必须与启动开始升级的ulUpgId匹配，否则单板不响应停止升级。请求停止的ulUpgId为0时，单板无条件响应停止升级请求。  */
    HI_U16 usOption;    /**<固定为0。    */
    HI_U16 reserved;
} HI_UPG_STOP_REQ_S;

/**
 *@ingroup hct_upg
 * 停止升级应答参数。
 */
typedef struct
{
    HI_U32 ulRc;      /**<结果码：HI_ERR_SUCCESS表示成功，其他为错误码，参见hi_error.h。  */
    HI_U32 ulUpgId;   /**<升级ID，在升级过程中必须唯一。 */
} HI_UPG_STOP_IND_S;

/**
 *@ingroup hct_upg
 * 开始升级请求参数中升级文件信息。
 */
typedef struct
{
    HI_U16 usChecksum;      /**<校验和，参考升级文件校验和计算例子。 */
    HI_U16 usFileBlkSize;   /**<固定为0。    */
    HI_U32 ulUpgId;      /**<升级ID。    */
    HI_U16 usReqSn;      /**<固定为0。    */
    HI_U16 usIdx;      /**<固定为1。    */
    HI_U8 ucFileBlk[0];      /**<升级文件数据（即.hupg文件）。    */
} HI_UPG_TRANSMIT_BLK_REQ_S;

/**
 *@ingroup hct_upg
 * 升级状态查询应答结构体版本定义。
 */
typedef enum
{
    HI_UPG_STATUS_IND_ENTRY_VER0 = 0,      /**< 版本0，对应的结构体为HI_UPG_STATUS_IND_ENTRY_S_VER0。*/
    HI_UPG_STATUS_IND_ENTRY_VER1,          /**< 版本1，对应的结构体为HI_UPG_STATUS_IND_ENTRY_S。*/
} HI_UPG_STATUS_IND_ENTRY_VER_E;

/**
 *@ingroup hct_upg
 * 升级状态查询应答结构体版本号。
 */
#define HI_UPG_STATUS_IND_ENTRY_VER     HI_UPG_STATUS_IND_ENTRY_VER1

/**
 *@ingroup hct_upg
 * 站点升级状态信息结构体版本0。
 */
typedef struct
{
    HI_U16 usLastBlkIdx;          /**< 最后一个连续有效升级文件块的索引号。      */
    HI_U16 ulErrorCode;          /**< 升级过程中的错误码。      */
    HI_U32 ulRcvFileSize;          /**< 通信模块接收升级文件长度，单位：byte，表示接收的有效块大小总和（可能不连续）。      */
    HI_U8  usUpgStatus;          /**< 当前升级状态。      */
    HI_U8  aucVer[3];          /**< VRC版本号。      */
    HI_U16 ausVer[2];          /**< ausVer[0]为预留字段和ausVer[1]为SPC版本号。      */
    HI_U8  aucBootVer3;          /**< Boot版本第三个字节。      */
    HI_U8  ucNvCVer;          /**< NV文件的C版本。      */
    HI_U8  aucBuildDataTime[6];          /**<升级文件构建的日期和时间,YYMMDDhhmmss。      */
    HI_U16 ausNvVer[2];          /**< ausNvVer[0]为预留字段和ausNvVer[1]为NV的SPC版本号。      */
    HI_U32 ulChipVer;          /**< 芯片序列号。      */
    HI_U16 usUpgReqSn;          /**< 块传输的SN(暂未使用 固定为0)。      */
    HI_U8  ucMacAddr[HI_PLC_MAC_ADDR_LEN];          /**< 通信模块的MAC地址（PLC的MAC地址）。      */
    HI_U32 ulUpgEndTime;          /**< 模块升级耗时。      */
} HI_UPG_STATUS_IND_ENTRY_S_VER0;

/**
 *@ingroup hct_upg
 * 站点升级状态信息结构体版本1。
 */
typedef struct
{
    HI_U16 usLastBlkIdx;  /**< 最后一个连续有效升级文件块的索引号。 */
    HI_U16 ulErrorCode;   /**< 升级过程中的错误码。      */
    HI_U32 ulRcvFileSize; /**< 通信模块接收升级文件长度，单位：byte，表示接收的有效块大小总和（可能不连续）。 */
    HI_U8  usUpgStatus; /**< 当前升级状态：
                            HI_MDM_UPG_STATUS_OK表示升级成功。\n
							HI_MDM_UPG_STATUS_END表示处于验证状态。\n
							HI_MDM_UPG_STATUS_FAIL表示升级失败。 \n
							HI_MDM_UPG_STATUS_PROCESS表示正在升级。 \n
							HI_MDM_UPG_STATUS_UNKNOWN表示未知状态。*/
    HI_U8  aucVer[3];           /**< VRC版本号。      */
    HI_U16 ausVer[2];          /**< ausVer[0]为预留字段和ausVer[1]为SPC版本号。      */
    HI_U8  aucBootVer3;          /**< Boot版本第三个字节。      */
    HI_U8  reserved1;
    HI_U8  aucBuildDataTime[6];      /**< 升级文件构建的日期和时间,YYMMDDhhmmss。      */
    HI_CHAR szManuCode[2];          /**< 厂商代码。      */
    HI_CHAR szChipCode[2];          /**< 芯片代码。      */
    HI_U16 usSwVer;          /**< 客户软件版本号。      */
    HI_U16 usYear   : 7;          /**< 客户版本日期-年。      */
    HI_U16 usMonth  : 4;          /**< 客户版本日期-月。     */
    HI_U16 usDay    : 5;          /**< 客户版本日期-日。      */
    HI_U8  ucStruVer: 4;          /**< 结构体版本号。      */
    HI_U8  reserved2 : 4;
    HI_U8  reserved3;
    HI_U8  ucMacAddr[HI_PLC_MAC_ADDR_LEN];          /**< 通信模块的MAC地址（PLC的MAC地址）。      */
    HI_U32 ulUpgEndTime;          /**< 模块升级耗时。      */
} HI_UPG_STATUS_IND_ENTRY_S;

/**
 *@ingroup hct_upg
 * 全网升级查询升级状态所有信息时一次上报的站点个数。
 */
#define UPG_STATUS_IND_ENTRY_NUM_MAX 8

/**
 *@ingroup hct_upg
 * 升级状态查询应答。
 */
typedef struct
{
    HI_U32 ulUpgId;          /**< 升级过程的标识。在升级流程中，该标识不能重复。 */
    HI_U32 ulBlkSize;          /**< 固定为300。      */
    HI_U32 ulFileSize;          /**< 升级文件大小，单位：byte。      */
    HI_U32 ulNum;          /**< entry的有效个数。      */
    HI_UPG_STATUS_IND_ENTRY_S entry[0];          /**< 站点升级状态信息。  */
} HI_UPG_STATUS_IND_S;

/**
 *@ingroup hct_upg
 * 全网升级状态查询统计信息应答参数。
 */
typedef struct
{
    HI_U32 ulUpgId;          /**< 全网升级的标识。    */
    HI_U16 usOkNum;          /**< 全网升级通信模块升级成功的总个数。       */
    HI_U16 usCompleteNum;    /**< 全网升级通信模块升级验证中的总个数。     */
    HI_U16 usFailNum;        /**< 全网升级通信模块升级失败的总个数。       */
    HI_U16 usProcessNum;     /**< 全网升级通信模块正在传输过程中的总个数。 */
    HI_U8  ucUpgStatus;      /**< 当前全网升级状态：HI_MDM_UPG_STATUS_OK表示全网升级成功；HI_MDM_UPG_STATUS_END表示全网升级结束；HI_MDM_UPG_STATUS_PROCESS表示正在全网升级。*/

    HI_U8 ucUpgOption : 2;   /**< CCO升级STA方式：HI_MDM_UPGRADE_NA表示不涉及；HI_MDM_UPGRADE_ALL表示全网方式；HI_MDM_UPGRADE_LIST表示列表方式。   */
    HI_U8 pad1 : 6;
    HI_U8 pad2[2];
} HI_UPG_STAT_INFO_IND_S;

/**
 *@ingroup hct_upg
 * 升级状态请求参数。
 */
typedef struct
{
    HI_UPG_STATUS_REQ_S stReqStatus;    /**< 状态查询请求。   */
}HI_MDM_UPG_STATUS_REQ_S;

/**
 *@ingroup hct_upg
 * 升级状态应答参数结构体。
 */
typedef struct
{
    HI_UPG_STATUS_IND_S stUpgStatus;   /**< 全网或单点升级状态查询应答。    */
} HI_MDM_UPG_STATUS_IND_S;

/**
 *@ingroup hct_upg
 * 升级状态统计信息应答参数。
 */
typedef struct
{
    HI_UPG_STAT_INFO_IND_S stStatInfo;      /**< 全网升级统计信息查询应答参数。    */
} HI_MDM_UPG_STAT_INFO_IND_S;

/**
 *@ingroup hct_upg
 * 升级状态基本信息应答参数。
 */
typedef struct
{
    HI_U32 ulUpgId;           /**< 升级ID。      */
    HI_U8  ucUpgStatus;       /**<升级状态：HI_MDM_UPG_STATUS_OK表示升级成功；HI_MDM_UPG_STATUS_FAIL表示升级失败；HI_MDM_UPG_STATUS_END表示升级结束；HI_MDM_UPG_STATUS_PROCESS表示正在升级；HI_MDM_UPG_STATUS_UNKNOWN表示未知升级状态，当前不处于升级过程中。 */
    HI_U8  aucReserved[3];
} HI_MDM_UPG_STATUS_BS_IND_S;

/**
 *@ingroup hct_upg
 * 开始升级请求参数结构体。
 */
typedef struct
{
    HI_UPG_START_REQ_S        stUpgCfg;         /**< 开始升级信息。 */
    HI_UPG_STA_LIST_INFO_S    stUpgListInfo;    /**< 站点列表信息。 */
    HI_UPG_TRANSMIT_BLK_REQ_S stUpgFile;        /**< 升级文件信息。 */
} HI_MDM_UPG_START_REQ_S;

/**
 *@ingroup hct_upg
 * 开始升级应答参数。
 */
typedef struct
{
    HI_U32 ulRc;           /**< 升级开始执行结果:HI_ERR_SUCCESS表示启动升级成功。其他值表示启动升级失败，参见hi_error.h。  */
}HI_MDM_UPG_START_IND_S;

/**
 *@ingroup hct_upg
 * 停止升级请求参数。
 */
typedef struct
{
    HI_UPG_STOP_REQ_S stStopReq;    /**< 停止升级请求参数。      */
} HI_MDM_UPG_STOP_REQ_S;

/**
 *@ingroup hct_upg
 * 停止升级应答参数。
 */
typedef struct
{
    HI_UPG_STOP_IND_S stStopInd;    /**< 停止升级应答信息。      */
} HI_MDM_UPG_STOP_IND_S;

/**
 *@ingroup hct_upg
 * 启动设备升级请求参数。
 */
typedef struct
{
    HI_U32 ulUpgId;           /**< 升级ID，取值为全网升级CCO下发给STA的升级标识。      */
    HI_U32 ulFileSize;           /**< 升级文件大小，单位：byte。      */
    HI_U8 *pucFileData;           /**< 升级文件数据，pucFileData为空时，需要通过接口HI_MDM_UPG_MemRead()获取升级文件数据。      */
} HI_MDM_DEVICE_UPG_START_REQ_S;

/**
 *@ingroup hct_upg
 * 启动设备升级应答参数结构体。
 */
typedef struct
{
    HI_U32 ulUpgId;           /**< 升级ID，取值为全网升级CCO下发给STA的升级标识。      */
    HI_U32 ulRc;           /**< 开始设备升级执行结果：HI_ERR_SUCCESS表示启动成功，其他值表示启动失败。   */
} HI_MDM_DEVICE_UPG_START_IND_S;

/**
 *@ingroup hct_upg
 * 设备升级停止请求参数结构体。
 */
typedef struct
{
    HI_U32 ulUpgId;           /**< 升级ID。      */
} HI_MDM_DEVICE_UPG_STOP_REQ_S;

/**
 *@ingroup hct_upg
 * 设备升级停止应答参数结构体。
 */
typedef struct
{
    HI_U32 ulUpgId;           /**< 升级ID。      */
    HI_U32 ulRc;           /**< 停止设备升级执行结果。      */
} HI_MDM_DEVICE_UPG_STOP_IND_S;

/**
 *@ingroup hct_upg
 * 设备升级状态查询请求参数结构体。
 */
typedef struct
{
    HI_U32 ulUpgId;           /**< 升级ID，取值为全网升级CCO下发给STA的升级标识。      */
} HI_MDM_DEVICE_UPG_STATUS_REQ_S;

/**
 *@ingroup hct_upg
 * 设备升级状态定义。
 */
typedef enum
{
    HI_MDM_DEVICE_UPG_STATUS_IDLE,           /**< 空闲。      */
    HI_MDM_DEVICE_UPG_STATUS_PROGRESS,       /**< 正在升级。  */
    HI_MDM_DEVICE_UPG_STATUS_SUCCESS,        /**< 升级成功。  */
    HI_MDM_DEVICE_UPG_STATUS_FAIL,           /**< 升级失败。  */
} HI_MDM_DEVICE_UPG_STATUS_E;

/**
 *@ingroup hct_upg
 * 设备升级状态查询应答参数结构体。
 */
typedef struct
{
    HI_U32 ulUpgId;           /**< 升级ID，取值为全网升级CCO下发给STA的升级标识。      */
    HI_U8 ucStatus;           /**< 设备升级状态：\n
	                          HI_MDM_DEVICE_UPG_STATUS_IDLE表示空闲状态。\n
							  HI_MDM_DEVICE_UPG_STATUS_PROGRESS表示正在设备升级。\n
							  HI_MDM_DEVICE_UPG_STATUS_SUCCESS表示设备升级成功。\n
							  HI_MDM_DEVICE_UPG_STATUS_FAIL表示设备升级失败。   */
    HI_U8 reserved[3];
} HI_MDM_DEVICE_UPG_STATUS_IND_S;

/**
* @ingroup  hct_upg
* @brief  获取升级缓存。
*
* @par   描述:
			该函数实现获取升级缓存，用于存储升级文件及站点列表。
* @attention 调用该接口获取升级缓存成功，则在停止升级（通过HI_MDM_UPG_STOP接口）后及时调用HI_MDM_UPG_ReleaseMem释放升级缓存。
* @param  ulUpgId [IN]保留字段，默认填0。
* @param  ulFileSize [IN]升级文件大小。CCO侧调用时，该值不能超过HI_MDM_UPG_CCO_STA_FILE_MAX_SIZE，否则获取缓存失败。其他产品形态调用时，该值不能超过HI_MDM_UPG_FILE_MAX_SIZE，否则获取缓存失败。
* @param  ppstReq [OUT]升级启动消息参数。
* @param  ulOption [IN]预留字段。
* @retval #HI_ERR_SUCCESS 获取内存成功。
* @retval #其他值 获取内存失败，参见hi_error.h。
* @par Dependency:
* <ul><li>hi_mdm_upg.h: 文件用于描述升级对外接口。</li></ul>
* @see 无。
* @since HiMDL_V200R001C00
*/
HI_EAPI HI_U32 HI_MDM_UPG_ObtainMem(HI_U32 ulUpgId,HI_U32 ulFileSize, HI_OUT HI_MDM_UPG_START_REQ_S** ppstReq,HI_U32 ulOption);

/**
* @ingroup  hct_upg
* @brief  释放升级缓存。
*
* @par   描述:
			释放HI_MDM_UPG_ObtainMem接口申请的升级缓存。
* @attention	调用HI_MDM_UPG_STOP接口停止升级前，必须先调用该接口释放升级缓存。
* @param  ulUpgId [IN]保留字段，默认填0。
* @param  ppstReq [OUT]升级启动消息参数。
* @param  ulOption [IN]预留字段。
* @retval	#HI_ERR_SUCCESS 释放内存成功。
* @retval	#其他值 释放内存失败，参见hi_error.h。
* @par Dependency:
* <ul><li>hi_mdm_upg.h: 文件用于描述升级对外接口。</li></ul>
* @see 无。
* @since HiMDL_V200R001C00
*/
HI_EAPI HI_U32 HI_MDM_UPG_ReleaseMem(HI_U32 ulUpgId, HI_IN HI_MDM_UPG_START_REQ_S* ppstReq,HI_U32 ulOption);

/**
* @ingroup  hct_upg
* @brief  写升级数据到FLASH缓存区。
*
* @par   描述:
			该函数负责将升级文件写入STA升级缓存区。
* @attention	
*@li 仅限于STA。
*@li 调用该接口前必须先调用HI_MDM_UPG_ObtainMem接口获取升级缓存。
* @param  ulOffset [IN]相对升级缓存区起始地址的偏移地址。
* @param  pucBuf [IN]升级数据包指针。
* @param  ulBufLen [IN]升级数据包长度，单位：byte。
* @retval	#>0 实际写入的数据长度，单位：byte。
* @retval	#=0 数据写入失败。
* @par Dependency:
* <ul><li>hi_mdm_upg.h: 文件用于描述升级对外接口。</li></ul>
* @see 无。
* @since HiMDL_V200R001C00
*/
HI_EAPI HI_U32 HI_MDM_UPG_MemWrite(HI_U32 ulOffset, HI_U8 * pucBuf, HI_U32 ulBufLen);

/**
* @ingroup  hct_upg
* @brief  从FLASH缓存区读取数据。
*
* @par   描述:
			该函数负责从STA升级缓存区读升级文件数据。
* @attention  仅限于STA。
* @param  ulOffset [IN]相对升级缓存区起始地址的偏移地址。
* @param  pucBuf [IN升级数据包指针。
* @param  ulBufLen [IN]升级数据包长度，单位：byte。
* @retval  #>0 实际读取的数据长度，单位：byte。
* @retval  #=0 数据读取失败。
* @par Dependency:
* <ul><li>hi_mdm_upg.h: 文件用于描述升级对外接口。</li></ul>
* @see 无。
* @since HiMDL_V200R001C00
*/
HI_EAPI HI_U32 HI_MDM_UPG_MemRead(HI_U32 ulOffset, HI_U8 * pucBuf, HI_U32 ulBufLen);

/**
* @ingroup  hct_upg
* @brief  获取升级时隙。
*
* @par   描述:
			获取升级时隙。
* @attention  仅限于CCO。
* @param  lid [IN]绑定业务链路标识，当前仅限于升级业务的链路标识。允许设置的lid值:2,6。
* @retval	#HI_ERR_SUCCESS 申请升级时隙成功。
* @retval   #其他值 申请升级时隙失败,参见hi_error.h。
* @par Dependency:
* <ul><li>hi_mdm_upg.h: 文件用于描述升级对外接口。</li></ul>
* @see  无。
* @since HiMDL_V200R001C00
*/
HI_EAPI HI_U32 HI_MDM_ObtainUpgTimeSlot(HI_U8 lid);

/**
* @ingroup  hct_upg
* @brief  释放升级时隙。
*
* @par   描述:
			获取升级时隙。
* @attention 仅限于CCO。
* @param  lid [IN]绑定业务链路标识，当前仅限于升级业务的链路标识。允许设置的lid值:2,6。
* @retval	#HI_ERR_SUCCESS 释放升级时隙成功。
* @retval   #其他值 释放升级时隙失败,参见hi_error.h。
* @par Dependency:
* <ul><li>hi_mdm_upg.h: 文件用于描述升级对外接口。</li></ul>
* @see 无。
* @since HiMDL_V200R001C00
*/
HI_EAPI HI_U32 HI_MDM_ReleaseUpgTimeSlot(HI_U8 lid);

/**
* @ingroup  hct_upg
* @brief  注册升级消息处理接口。
*
* @par   描述: 
			注册升级消息处理接口。
* @attention 启动升级流程前调用。
* @param  ulMsgId [IN]消息ID。
* @param  pfnUpgMsgNotify [IN]用户注册的回调函数。
* @retval 无。
* @par Dependency:
* <ul><li>hi_mdm_upg.h: 文件用于描述升级对外接口。</li></ul>
* @see 无。
* @since HiMDL_V200R001C00
*/
HI_VOID HI_MDM_RegisterUpgMsgNotify(HI_U32 ulMsgId, HI_MDM_UPG_PROC_MSG_FUN pfnUpgMsgNotify);

/**
* @ingroup  hct_upg
* @brief  升级消息发送接口。
*
* @par   描述: 
			升级消息发送接口。
* @attention  无。
* @param  ulMsgId [IN]消息ID。
* @param  pucPacket [IN]发送的数据包内容。
* @param  ulPacketSize [IN]发送的数据包大小。
* @retval  #HI_ERR_SUCCESS 升级消息发送成功。
* @retval  #其他值 发送失败,参见hi_error.h。
* @par Dependency:
* <ul><li>hi_mdm_upg.h: 文件用于描述升级对外接口。</li></ul>
* @see 无。
* @since HiMDL_V200R001C00
*/
HI_U32 HI_MDM_UPG_MsgSend(HI_U32 ulMsgId, HI_PBYTE pucPacket, HI_U32 ulPacketSize);

/**
* @ingroup  hct_upg
* @brief  注册升级文件合法性校验接口。
*
* @par   描述: 
			注册升级文件合法性校验接口。
* @attention 无。
* @param  pfnUpgFileCheck [IN]用户自定义升级文件校验接口。
* @param  pParam [IN]回调时传回给回调函数。
* @retval   #HI_ERR_SUCCESS 注册成功。
* @retval   #其他值 注册失败，参见hi_error.h。
* @par Dependency:
* <ul><li>hi_mdm_upg.h: 文件用于描述升级对外接口。</li></ul>
* @see 无。
* @since HiMDL_V200R001C00SPC010
*/
HI_U32 HI_MDM_RegisterUpgFileVerifyFct(
    HI_BOOL (*pfnUpgFileCheck)(HI_UPG_FILE_HEADER_S *pstHeader, HI_VOID *pParam),
    HI_VOID *pParam);

/**
* @ingroup  hct_upg
* @brief  设置海思升级协议类型。
*
* @par   描述: 
			设置海思升级协议类型。
* @attention	
*@li 仅限于CCO。
*@li 必须启动升级前调用。用户需要使用海思升级协议，则调用此接口。
*@li 不支持注册国网升级协议场景。
* @retval  #HI_ERR_SUCCESS 设置成功。
* @retval  #其他值 设置失败。
* @par Dependency:
* <ul><li>hi_mdm_upg.h: 文件用于描述升级对外接口。</li></ul>
* @see 无。
* @since HiMDL_V200R001C00SPC012
*/
HI_U32 HI_MDM_UPG_SetPrivateProto(HI_VOID);

/**
* @ingroup  hct_upg
* @brief  升级传输完成注册回调函数。
*
* @par   描述: 
			升级传输完成注册回调函数。
* @attention 仅限于CCO。
* @param  无。
* @retval 无。
* @par Dependency:
* <ul><li>hi_mdm_upg.h: 文件用于描述升级对外接口。</li></ul>
* @see 无。
* @since HiMDL_V200R001C00SPC010
*/
typedef HI_VOID (*HI_MDM_pfnAffirmNotify)(HI_VOID *pParam);

/**
* @ingroup  hct_upg
* @brief  注册升级传输完成通知接口。
*
* @par   描述: 
			全网升级时注册CCO给STA传输完成通知接口。
* @attention  仅限于CCO。
* @param  pfnCallback [IN]升级传输完成通知接口。
* @param  pParam [IN]用户注册的回调参数。
* @retval #HI_ERR_SUCCESS 注册成功。
* @retval #HI_ERR_SUCCESS 注册失败。
* @par Dependency:
* <ul><li>hi_mdm_upg.h: 文件用于描述升级对外接口。</li></ul>
* @see 无。
* @since HiMDL_V200R001C00SPC010
*/
HI_U32 HI_MDM_RegisterAffirmNotify(HI_MDM_pfnAffirmNotify pfnCallback, HI_VOID *pParam);

/**
 *@ingroup hct_upg
 * 延时等待配置。
 */
typedef struct
{
    HI_BOOL bEnable;      /**<升级重启或断电续传上电后，站点个数少于开始升级时站点个数，是否启用延时等待策略。默认值为1。取值为0表示不启用延时等待策略，取其他值表示启用该策略。*/
    HI_U8 ucPeriod;   /**<定期检查站点是否入全的定时器周期时长，单位:min。默认为2min。*/
    HI_U8 ucTimerCnt; /**<一轮包含的定时器周期个数。默认值为10。在等待的一轮时间内站点个数没变化，则不再继续延时等待。当设置为0时，取默认值10。*/
    HI_U8 ucLoopCnt;  /**<延时等待轮数。默认值为3。达到最大轮数后，不再等待。*/
}UPG_DELAY_WAIT_CFG_STRU;

/**
 *@ingroup hct_upg
 * 查询站点信息配置。
 */
typedef struct
{
    HI_U8 ucParallelNum; /**<并发查询站点个数。默认值为5。当设置为0时，取默认值5。*/
    HI_U8 reserved[3];
    HI_U16 usLoopInterval;  /**<轮间隔，单位:s。默认值为60。*/
    HI_U16 usInterval;     /**<并发帧间隔，单位:ms。默认值为2000。*/
}UPG_QRY_STA_INFO_CFG_STRU;

/**
 *@ingroup hct_upg
 * 全网广播配置。
 */
typedef struct
{
    HI_U8   succ_ratio;              /**<传输完成站点百分比，默认值:100。*/
    HI_U8   exception_max_count;/**<容忍的最大EXCEPTION次数，默认值:2。*/
    HI_U8   query_switch;       /**<首次是否查询开关，默认值:0，表示首轮不查询。取值为1表示首轮查询，取其他值表示首轮不查询。*/
    HI_U8   query_count;        /**<最大查询次数，默认值:5。*/

    HI_U16  data_loop_max;      /**<数据发送最大轮次，默认值:4。*/
    HI_U16  data_send_interval; /**<包和包之间发送间隔，单位:ms，默认值:200。*/
    HI_U16  query_send_interval;/**<查询帧发送间隔，单位:ms，默认值:500。*/
    HI_U16  query_max_ways;     /**<最大并发查询数，默认值:3。当取值大于等于10时，或等于0时，取最大值9。*/
}BROADCAST_CFG_STRU;

/**
 *@ingroup hct_upg
 * 全网完成通知配置。
 */
typedef struct
{
    HI_U8 delayed_for_send_restart;        /**<CCO发送重启命令后延时多久重启，单位:s，默认值:70。*/
    HI_U8 delayed_for_recv_to_restart;      /**<STA收到重启命令后延时多久重启，单位:s，默认值:60。*/
    HI_U8 send_count;               /**<发送几轮命令，默认值:60。该值为0时，只发送1轮。*/
    HI_U8 reserved1;           
    HI_U16 send_interval;           /**<包和包之间发送间隔，单位:ms，默认值:500。*/
    HI_U16 reserved2;
}AFFIRM_CFG_STRU;

/**
 *@ingroup hct_upg
 * 开始升级帧配置。
 */
typedef struct
{
    HI_U8 send_count;         /**<广播发送几轮命令，默认值:5。该值为0时，只发送1轮。*/
    HI_U8 bc_delayed;        /**<广播发送start帧等待多久发送数据，单位:s，默认值:30。*/  
    HI_U16 bc_send_interval; /**<广播包和包之间发送间隔，单位:ms，默认值:500。*/
    HI_U8 reserved;
    HI_U8 delayed;        /**<单播发送start帧等待多久发送数据，单位:s，默认值:12。*/    
    HI_U16 send_interval; /**<单播包和包之间发送间隔，单位:ms，默认值:200。*/
    HI_U32 send_ways;     /**<单播并发时并发路数，默认值:5。当取值大于等于10时，或等于0时，取最大值9。*/
}NOTIFY2START_CFG_STRU;

/**
 *@ingroup hct_upg
 * 单播转本地广播配置。
 */
typedef struct
{
    HI_U8  succ_ratio;              /**<传输完成站点百分比，默认值:100。*/
    HI_U8  exception_max_count;     /**<容忍的最大EXCEPTION次数，默认值:5。*/
    HI_U8  query_switch;           /**<首次是否查询开关，默认值:1，表示首轮查询。取其他值表示首轮不查询。*/
    HI_U8  query_count;            /**<最大查询次数，默认值:3。*/

    HI_U16 data_loop_max;          /**<数据发送最大轮次，默认值:1。*/
    HI_U16 data_max_ways;          /**<同时并发的最大路数，默认值:3。当取值大于等于10时，或等于0时，取最大值9。*/
    HI_U16 data_send_interval;     /**<包和包之间发送间隔，单位:ms，默认值:200。*/
    HI_U16 query_send_interval;    /**<查询帧发送间隔，单位:ms，默认值:600。*/
    HI_U16 query_max_ways;         /**<最大并发查询数，默认值:3。当取值大于等于10时，或等于0时，取最大值9。*/
    HI_U16 reverved;             
}U2B_CFG_STRU;

/**
 *@ingroup hct_upg
 * 并发单播配置。
 */
typedef struct
{
    HI_U8   succ_ratio;          /**<传输完成站点百分比，默认值:100。*/
    HI_U8   exception_max_count; /**<容忍的最大EXCEPTION次数，默认值:5。*/
    HI_U8   query_switch;        /**<首次是否查询开关，默认值:1，表示首轮查询。取其他值表示首轮不查询。*/
    HI_U8   query_count;        /**<最大查询次数，配合exception_max_count使用，默认值:3。*/

    HI_U16  data_loop_max;     /**<数据发送最大轮次，当达到该数时，exception加1，当exception达到exception_max_count后将该站点置为exception。默认值:10。*/
    HI_U16  data_max_ways;     /**<同时并发的最大路数，默认值:3。当取值大于等于10时，或等于0时，取最大值9。*/
    HI_U16  data_send_interval; /**<包和包之间发送间隔，单位:ms，默认值:200。*/
    HI_U16  query_send_interval; /**<查询帧发送间隔，单位:ms，默认值:700。*/
    HI_U16  query_max_ways;       /**<最大并发查询数，默认值:3。当取值大于等于10时，或等于0时，取最大值9。*/
    HI_U16 reverved;                 
}UNICAST_CFG_STRU;

/**
 *@ingroup hct_upg
 * 默认策略配置，包括全网广播参数、全网完成通知参数、开始升级帧参数等。
 */
typedef struct
{
    HI_U8 broadcast_switch; /**<是否采用全网广播的开关，默认值:1，表示打开该策略。取非1的值表示关闭该策略。*/
    HI_U8 u2b_switch;       /**<是否采用单播转本地广播的开关，默认值:1，表示打开该策略。取非1的值表示关闭该策略。*/
    HI_U8 unicast_switch;   /**<是否采用并发单播的开关，默认值:1，表示打开该策略。取非1的值表示关闭该策略。*/
    HI_U8 reserved1;
    
    BROADCAST_CFG_STRU bc_cfg; /**<全网广播配置。*/
    AFFIRM_CFG_STRU affirm_cfg; /**<全网完成通知配置。*/
    NOTIFY2START_CFG_STRU notify2start_cfg; /**<开始升级帧配置。*/

    HI_U8 query_loop_interval; /**<发送一组查询报文后等待时间，单位:s，默认值:12。*/
    HI_U8 data_loop_interval; /**<发送一轮数据完成后等待的时间，单位:s，默认值:1。*/
    HI_U8 switch_schedule_delay; /**<策略调度切换时间，单位:100ms，默认值:12。*/
    HI_U8 reserved2;
}UPG_SCHEDL_DEFAULT_CFG_STRU;

/**
 *@ingroup hct_upg
 * 高级策略配置，包括单播转本地广播及并发单播配置。
 */
typedef struct
{
    U2B_CFG_STRU u2b_cfg;      /**<单播转本地广播配置。*/
    UNICAST_CFG_STRU uc_cfg;   /**<并发单播配置。*/
}UPG_SCHEDL_ADVANCE_CFG_STRU;

/**
 *@ingroup hct_upg
 * 升级参数。
 */
typedef struct
{
    UPG_SCHEDL_DEFAULT_CFG_STRU stDefCfg;  /**<默认策略配置。*/
    UPG_SCHEDL_ADVANCE_CFG_STRU stAdvanceCfg; /**<高级策略配置。*/
    UPG_DELAY_WAIT_CFG_STRU stDelayWaitCfg; /**<延时等待配置。*/
    UPG_QRY_STA_INFO_CFG_STRU stQryStaCfg; /**<查询站点信息配置。*/
}HI_MDM_UPG_CFG;

/**
* @ingroup  hct_upg
* @brief  获取升级配置参数。
*
* @par   描述:
			该函数用于获取升级配置参数。
* @attention 无。
* @param  pstCfg [OUT]升级配置参数。
* @retval #HI_ERR_SUCCESS 获取成功。
* @retval #其他值 获取失败，参见hi_error.h。
* @par Dependency:
* <ul><li>hi_mdm_upg.h: 文件用于描述升级对外接口。</li></ul>
* @see 无。
* @since HiMDL_V200R001C00SPC010
*/
HI_EAPI HI_U32 HI_MDM_UPG_GetCfg(HI_MDM_UPG_CFG *pstCfg);

/**
* @ingroup  hct_upg
* @brief  设置升级配置参数。
*
* @par   描述:
			该函数用于设置升级配置参数。
* @attention 
*@li 仅限于CCO。
*@li 需要在APP层任务初始化接口中调用。具体使用的参数与当前运行的程序匹配。如果用户不调用该接口，则使用默认的参数。 
*@li 默认策略配置和高级策略配置适用于CCO升级重启前，分发数据过程中使用。
*@li 延时等待配置适用于CCO升级重启或断电续传，组网完成后站点个数比开始升级时站点个数少场景。
*@li 查询站点信息配置适用于CCO升级重启组网完成后。
*@li 时间相关参数，设置为0时，取默认值10ms。
* @param  pstCfg [IN]升级配置参数。
* @retval #HI_ERR_SUCCESS 设置成功。
* @retval #其他值 设置失败，参见hi_error.h。
* @par Dependency:
* <ul><li>hi_mdm_upg.h: 文件用于描述升级对外接口。</li></ul>
* @see 无。
* @since HiMDL_V200R001C00SPC010
*/
HI_EAPI HI_U32 HI_MDM_UPG_SetCfg(HI_MDM_UPG_CFG *pstCfg);

/**
* @ingroup  hct_upg
* @brief  初始化南网协议。
*
* @par   描述: 
			初始化南网协议。
* @attention 
*@li 必须在APP层接口(APP_Init)中调用,且需要在调用其他升级二次开发接口前调用。
*@li 必须在应用层初始化之前调用。
*@li 调用初始化接口后，必须调用匹配的去初始化接口，否则会去初始化失败。
*@li 调用初始化接口后，再此调用初始化接口前，必须先去初始化。
* @param 无。
* @retval #HI_ERR_SUCCESS 初始化成功。
* @retval #其他值 初始化失败，参见hi_error.h。
* @par Dependency:
* <ul><li>hi_mdm_upg.h: 文件用于描述升级对外接口。</li></ul>
* @see 无。
* @since HiMDL_V200R001C00SPC012
*/
HI_EAPI HI_U32 HI_MDM_InitCsgProtocol(HI_VOID);

/**
* @ingroup  hct_upg
* @brief  南网协议去初始化。
*
* @par   描述: 
		  南网协议去初始化。
* @attention 无。
* @param 无。
* @retval #HI_ERR_SUCCESS 去初始化成功。
* @retval #其他值 去初始化失败，参见hi_error.h。
* @par Dependency:
* <ul><li>hi_mdm_upg.h: 文件用于描述升级对外接口。</li></ul>
* @see 无。
* @since HiMDL_V200R001C00SPC012
*/
HI_EAPI HI_U32 HI_MDM_DeinitCsgProtocol(HI_VOID);

/**
* @ingroup  hct_upg
* @brief  初始化国网协议。
*
* @par   描述: 
			初始化国网协议。
* @attention
*@li 必须在APP层接口(APP_Init)中调用,且需要在调用其他升级二次开发接口前调用。
*@li 必须在应用层初始化之前调用。
*@li 调用初始化接口后，必须调用匹配的去初始化接口，否则会去初始化失败。
*@li 调用初始化接口后，再此调用初始化接口前，必须先去初始化。
* @param 无。
* @retval #HI_ERR_SUCCESS 初始化成功。
* @retval #其他值 初始化失败，参见hi_error.h。
* @par Dependency:
* <ul><li>hi_mdm_upg.h: 文件用于描述升级对外接口。</li></ul>
* @see 无。
* @since HiMDL_V200R001C00SPC012
*/
HI_EAPI HI_U32 HI_MDM_InitStdProtocol(HI_VOID);

/**
* @ingroup  hct_upg
* @brief  国网协议去初始化。
*
* @par   描述: 
		  国网协议去初始化。
* @attention 无。
* @param 无。
* @retval #HI_ERR_SUCCESS 去初始化成功。
* @retval #其他值 去初始化失败，参见hi_error.h。
* @par Dependency:
* <ul><li>hi_mdm_upg.h: 文件用于描述升级对外接口。</li></ul>
* @see 无。
* @since HiMDL_V200R001C00SPC012
*/
HI_EAPI HI_U32 HI_MDM_DeinitStdProtocol(HI_VOID);


HI_END_HEADER
#endif // __HI_MDM_UPG_H__
