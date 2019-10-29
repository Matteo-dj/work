/**@defgroup hct_dms 通道
 * @ingroup sal
 */

#ifndef __HI_MDM_DMS_H__
#define __HI_MDM_DMS_H__

#include <hi_mdm_types.h>

/**
* @ingroup  hct_dms
* @brief  初始化通信模块MDM层APP相关接口的管理信息。
*
* @par 描述:
* 初始化通信模块MDM层APP相关接口的管理信息，
*只有使用该接口初始化之后，APP才可以使用MDM的接口。
*回复报文通过 HI_NDC_Init 参数 pstChlPort 返回。
* @attention 无。
*
* @param pstAppObj [IN] APP对象。
*
* @retval #HI_ERR_SUCCESS 成功。
* @retval 其他值 失败，详见hi_errno.h。
*
* @par Dependency:
* @li hi_mdm_dms.h: 该接口声明所在的头文件。
* @see  无。
* @since HiMDL_V200R001C00
*/
HI_EXTERN HI_U32 HI_MDM_AppInit(HI_CONST HI_MDM_APPOBJ_S* pstAppObj);

/**
* @ingroup  hct_dms
* @brief  通道注册。
*
* @par 描述:
* @li 使用默认配置: HI_MDM_ChlSetup。
* @li 更改默认配置步骤: Change NV item(HI_NV_FTM_DEVICE_CFG_ID) --> HI_DMS_ChlRefresh --> HI_MDM_ChlSetup。
* @attention 无。
*
* @param  pstChlPort [IN] 通道端口对象。
* @param  usPortNum [IN] 通道端口，取值为:
* @li HI_DMS_CHL_MAC_PORT_APP      : 表示MAC通道。
* @li HI_DMS_CHL_MAC_PORT_DIAG    : 表示MAC通道。
* @li HI_DMS_CHL_UART_PORT_APP    : 表示UART的MR通道。
* @li HI_DMS_CHL_UART_PORT_IR      : 表示IR通道。
* @li HI_DMS_CHL_TCP_PORT_APP      : 表示TCP的MR通道。
* @li HI_DMS_CHL_TCP_PORT_DIAG    : 表示TCP的DIAG通道。
* @li HI_DMS_CHL_UART_PORT_DIAG  : 表示UART的DIAG通道。
*
* @retval #HI_ERR_SUCCESS 成功。
* @retval 其他值 失败，详见hi_errno.h。
*
* @par Dependency:
* @li hi_mdm_dms.h: 该接口声明所在的头文件。
* @see  无。
* @since HiMDL_V200R001C00
*/
HI_EXTERN HI_U32 HI_MDM_ChlSetup(HI_CONST HI_DMS_CHL_PORT_OBJ_S* pstChlPort, HI_U16 usPortNum);

/**
* @ingroup  hct_dms
* @brief  对已注册的通道进行查询和设置。
*
* @par 描述:
* 该函数通过指定端口，对该通道进行查询和设置。
* @attention 选择HI_DMS_PORT_CTRL_UART_CLOSE功能后，若想接管串口用于收数据，则使用前必须先将串口设置为读非阻塞模式。\n
* 再根据需要设置为阻塞模式。若接管串口的任务优先级比原来使用该串口的任务优先级高，则需要在设置为阻塞模式后睡眠一次。否则不需要睡眠。\n
* 接管示例如下:\n
* HI_U32 read_no_block = UART_RD_NONBLOCK;//读非阻塞模式\n
* HI_U32 read_block = UART_RD_BLOCK;      //读阻塞模式\n
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
* @param  usPortNum [IN] 通道端口，不支持#HI_DMS_CHL_MAC_PORT_DIAG
* @param  enOpt [IN] 功能选项。
* @param  pInputParam [IN] 指定的输入参数。不使用时传入HI_NULL。
* @param  pOutputParam [OUT] 指定的输入参数。不使用时传入HI_NULL。
*
* @retval #HI_ERR_SUCCESS 成功。
* @retval 其他值 失败，详见hi_errno.h。
*
* @par Dependency:
* @li hi_mdm_dms.h: 该接口声明所在的头文件。
* @see  无。
* @since HiMDL_V200R001C00
*/
HI_EXTERN HI_U32 HI_MDM_ChlCtrl(HI_U16 usPortNum, HI_DMS_PORT_CTRL_OPT_ID_E enOpt, HI_IN HI_PVOID pInputParam, HI_OUT HI_PVOID pOutputParam);

/**
* @ingroup  hct_dms
* @brief  通过已注册的通道发送应用报文。
*
* @par 描述:
* 该函数通过指定端口进行发送。
* @attention 
* 1、填写入参请看入参结构体说明
*
* @param  usPortNum [IN] 通道端口  。
* @param  pFrameData [IN] DMS通道的帧数据，类型 #HI_DMS_CHL_TX_DATA_S。
* @param  pFrameCtrl [IN] DMS的帧控制信息，类型 #HI_DMS_CHL_TX_CTRL_S。
*
* @retval #HI_ERR_SUCCESS 成功。
* @retval 其他值 失败，详见hi_errno.h。
*
* @par Dependency:
* @li hi_mdm_dms.h: 该接口声明所在的头文件。
* @see  无。
* @since HiMDL_V200R001C00
*/
HI_EXTERN HI_U32 HI_MDM_ChlTx(HI_U16 usPortNum, HI_IN HI_DMS_CHL_TX_DATA_S* pFrameData, HI_IN HI_DMS_CHL_TX_CTRL_S* pFrameCtrl);

/**
* @ingroup  hct_dms
* @brief  通过该接口可以修改串口初始化配置。
*
* @par 描述:
* 通过该接口可以修改串口初始化配置。
* @attention 调用该接口后，要重新调用HI_MDM_ChlSetup接口一次，配置才会生效。
*
* @param  pstDevCfg [IN] 目标配置
* @param  chipType [IN]  芯片类型。
* @param  usportNum [IN] 端口号。取值请参见#HI_MDM_ChlSetup接口中对端口的描述。
*
* @retval #HI_ERR_SUCCESS 成功。
* @retval 其他值 失败，详见hi_errno.h。
*
* @par Dependency:
* @li hi_mdm_dms.h: 该接口声明所在的头文件。
* @see  无。
* @since HiMDL_V200R001C00
*/
HI_EXTERN HI_U32 HI_MDM_ChlUartPortRefresh(HI_FTM_DEVICE_UART_CFG_S* pstDevCfg, HI_FTM_CHIP_TYPE_E chipType, HI_U16 usportNum);

/**
* @ingroup  hct_dms
* @brief  该函数负责发送APP消息到指定的通信模块（MDM层）。
*
* @par 描述:
* 该函数负责发送APP消息到指定的通信模块（MDM层）。
* @attention 无。
*
* @param  ulMsgId [IN] 消息ID, 由MDM层提供  。
* @param  pucMsgPayload [IN] 消息用户数据。
* @param  ulMsgPayloadSize [IN] 消息用户数据长度, 单位字节。
* @param  pstCtrl [IN] 消息附加控制信息，参见HI_MDM_MSG_CTRL_S定义。
*
* @retval #HI_ERR_SUCCESS 成功。
* @retval 其他值 失败，详见hi_errno.h。
*
* @par Dependency:
* @li hi_mdm_dms.h: 该接口声明所在的头文件。
* @see  无。
* @since HiMDL_V200R001C00
*/
HI_EXTERN HI_U32 HI_MDM_MsgSend(HI_U32 ulMsgId, HI_PBYTE pucMsgPayload, HI_U32 ulMsgPayloadSize, HI_MDM_MSG_CTRL_S* pstCtrl);

/**
* @ingroup  hct_dms
* @brief 该消息ID对应PLC通道连接状态上报函数。
*
* @par   描述:
* 该函数实现MDM层主动上报PLC通道的连接状态给APP层。
* @attention 无。
*
* @param [OUT]类型 #HI_MDM_CONNECT_STATUS_IND_S  消息参数。
*
* @retval   无。
*
* @par Dependency:
* @li hi_mdm_dms.h: 该接口声明所在的头文件。
* @see  无。
* @since HiMDL_V200R001C00
*/
#define HI_MDM_CONNECT_STATUS_IND_ID        0x3600
/**
* @ingroup  hct_dms
* @brief 该消息ID对应APP之间数据传输函数。
*
* @par   描述:
* 函数实现APP层之间通过MDM层提供的通道进行数据传输。
* @attention 无。
*
* @param [IN]类型 #HI_MDM_APPDATA_TRANSMIT_DL_S  消息下行参数。
* @param [OUT]类型 #HI_MDM_APPDATA_TRANSMIT_UL_S  消息上行参数。
*
* @retval   无。
* @par Dependency:
* @li hi_mdm_dms.h: 该接口声明所在的头文件。
* @see  无。
* @since HiMDL_V200R001C00
*/
#define HI_MDM_APPDATA_TRANSMIT_ID             0x3601
#endif
