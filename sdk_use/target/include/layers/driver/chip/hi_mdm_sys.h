/**@defgroup hct_sys 系统
 * @ingroup drivers
 */

#ifndef __HI_MDM_SYS_H__
#define __HI_MDM_SYS_H__
#include <hi_types.h>
#include <hi_mdm_types.h>
#ifdef __cplusplus
extern "C" {
#endif

#define HI_MDM_APPDATA_TRANSMIT_USER_OPT_MAX            1           /**< 用户自定义数据大小         */

/**
* @ingroup hct_sys
 * 表示内存大小
 */
typedef enum
{
    HI_MEMORY_SIZE_UNKOWN = 0,
    HI_MEMORY_SIZE_2M = 0x200000,  //2M
    HI_MEMORY_SIZE_8M = 0x800000,  //8M
    HI_MEMORY_SIZE_16M= 0x1000000, //16M
    HI_MEMORY_SIZE_32M= 0x2000000, //32M
} HI_MEMORY_SIZE_E;

/**
* @ingroup hct_sys
 * 表示芯片版本信息
 */
typedef enum
{
    HI_CHIP_VERSION_UNKNOW  = 0,/**< 未知芯片版本*/
    HI_CHIP_VERSION_V100,       /**< V100芯片*/
    HI_CHIP_VERSION_V200,       /**< V200芯片*/
}HI_HW_CHIP_VER_E;

/**
 *@ingroup hct_sys
 * PLC通道状态结构体
 */
typedef struct
{
    HI_U8 ucChlStatus;          /**<  通道状态，取值参考HI_MDM_CHL_STATUS_XX        */
    HI_U8 reserved[3];
}HI_MDM_CONNECT_STATUS_IND_S;

/**
 *@ingroup hct_sys
 * 下行数据包格式
 */
typedef struct
{
    HI_U32 aulUser[HI_MDM_APPDATA_TRANSMIT_USER_OPT_MAX];       /**< 用户自定义数据         */
    HI_U32 ulDataSize;                      /**< 单次传输的数据量            */
    HI_U8 aucData[0];                       /**< 单次传输的数据                 */
}HI_MDM_APPDATA_TRANSMIT_DL_S;

/**
 *@ingroup hct_sys
 * 上行数据包格式
 */
typedef struct
{
    HI_U32 aulUser[HI_MDM_APPDATA_TRANSMIT_USER_OPT_MAX];   /**< 用户自定义数据         */
    HI_U32 ulDataSize;      /**< 单次传输的数据量            */
    HI_U8 aucData[0];       /**< 单次传输的数据                 */
}HI_MDM_APPDATA_TRANSMIT_UL_S;

/**
 * @ingroup hct_sys
* @brief  MDM消息相关结构体
*/
typedef struct
{
    HI_U8 aucMacAddr[HI_PLC_RELAY_NUM_MAX][HI_PLC_MAC_ADDR_LEN];    /**< MAC地址      */
    HI_U8 aucNdmMacAddr[HI_PLC_MAC_ADDR_LEN];           /**< 预留给MDM内部使用字段，初始化为0       */

    HI_U32 ulSn;                    /**<消息请求序列号，如不需要，可置0，通信模块MDM层不修改该域的取值，直接返回给APP层，APP层用于识别是否丢帧     */
    HI_U16 usTransId;           /**<传输ID号，取值为ulObjId，有效范围1~4，当前固定为1，暂不支持2~4      */
    HI_U16 usControl  :15;      /**<预留  */
    HI_U16 usDULink   :1;       /**<表示消息方向，HI_FALSE表示下行，HI_TRUE表示上行  */
} HI_MDM_MSG_CTRL_S;

/**
 * @ingroup hct_sys
* @brief  用户信息设置
*/
typedef struct
{
    HI_U16  usSwVer;        /**< 软件版本号，BCD码格式*/
    HI_U16  usYear  : 7;    /**< 版本日期-年，BIN码格式*/
    HI_U16  usMonth : 4;    /**< 版本日期-月，BIN码格式*/
    HI_U16  usDay   : 5;    /**< 版本日期-日，BIN码格式*/
    HI_CHAR szChipCode[2];  /**< 芯片代码，ASCII码格式*/
    HI_CHAR szManuCode[2];  /**< 厂商代码，ASCII码格式*/
    HI_U8   ucBootVer;      /**< Bootloader版本，BIN码格式*/
    HI_U8   reserved[3];    /**< 预留字节*/
}HI_MDM_USER_INFO_S;

/**
* @ingroup  hct_sys
* @brief  MDM响应APP消息的接口函数指针
*
* @par  描述:MDM响应APP消息的接口函数指针
* @attention
* <ul><li></li>使用之前要先注册</ul>
* @param  ulMdmMsgId [IN] 类型 #HI_U32，MDM消息响应ID。
* @param  pMdmMsg [IN] 类型 #HI_PVOID，表示APP接收的MDM消息，定义参见HI_MDM_MSG_APPRCV_S数据结构。
* @param  pData [IN] 类型 #HI_PVOID，表示APP层和MDM层其它的交互数据，可选。
* @param  ulOption[IN] 类型 #HI_PVOID，预留。
* @retval   HI_ERR_SUCCESS表示设置成功，HI_ERR_FAILURE表示设置失败
* @par Dependency:
* <ul><li>hi_mdm.h: 文件用于描述链路层对外接口.</li></ul>
* @see
* @since HiMDL_V200R001C00
*/
typedef HI_U32 (*HI_MDM_APPMSG_F)(HI_U32 ulMdmMsgId, HI_PVOID pMdmMsg, HI_PVOID pData, HI_U32 ulOption);

/**
 * @ingroup hct_sys
* @brief  APP对象
*/
typedef struct
{
    HI_U32 ulObjId;                 /**<对象ID号        */
    HI_MDM_APPMSG_F pfnAppMsgRcv;   /**<MDM响应APP消息的接口函数定义        */
} HI_MDM_APPOBJ_S;



/**
* @ingroup  hct_sys
* @brief 获取硬件内存大小。
*
* @par 描述:
* 获取硬件内存大小。
* @attention 当前仅适配Hi3911 V200类型，V100或FPGA需要根据实际情况来实现。

* @param 无。

* @retval #HI_MEMORY_SIZE_E  硬件内存大小枚举。

* @par Dependency:
* @li hi_mdm_sys.h:该接口声明所在的头文件。
* @see 无。
* @since HiMDL_V200R001C00
*/
HI_EXTERN HI_MEMORY_SIZE_E  HI_MDM_GetHwMemSize(HI_VOID);

/**
* @ingroup  hct_sys
* @brief 获取硬件芯片版本号。
*
* @par 描述:
* 获取硬件芯片版本号。
* @attention 无。
*
* @param 无。
*
* @retval #HI_HW_CHIP_VER_E类型数值 硬件芯片版本号枚举。
*
* @par Dependency:
* @li hi_mdm_sys.h:该接口声明所在的头文件。
* @see 无。
* @since HiMDL_V200R001C00
*/
HI_EXTERN HI_HW_CHIP_VER_E HI_MDM_GetHwChipVer(HI_VOID);

/**
* @ingroup  hct_sys
* @brief 获取硬件芯片类型。
*
* @par 描述:
* 获取硬件芯片类型。
* @attention 无。
*
* @param 无。
*
* @retval #HI_FTM_CHIP_TYPE_E  硬件芯片类型枚举。

* @par Dependency:
* @li hi_mdm_sys.h:该接口声明所在的头文件。
* @see 无。
* @since HiMDL_V200R001C00
*/
HI_EXTERN HI_FTM_CHIP_TYPE_E HI_MDM_GetHwChipType(HI_VOID);

/**
* @ingroup  hct_sys
* @brief  设置ARM总线时钟频率和APB总线时钟频率对应全局变量(当前不支持)。
*
* @par 描述:
* 设置ARM总线时钟频率和APB总线时钟频率对应全局变量(不设置寄存器)。
* @attention
* @li 在初始化阶段调用，且在HI_MDM_UART_SetFreq等配置驱动时钟之前。
* @li 在客户自行修改芯片时钟寄存器后使用，用户须保证其正确性。
*
* @param  ulAhb [IN] 类型 #HI_U32  ARM总线时钟频率。
* @param  ulApb [IN] 类型 #HI_U32  APB总线时钟频率。

* @retval #0      success.
* @retval #非0     failed.详见hi_errno.h
*
* @par Dependency:
* @li hi_mdm_sys.h:该接口声明所在的头文件。
* @see  无。
* @since HiMDL_V200R001C00
*/
HI_EXTERN HI_U32 HI_MDM_SetArmAndApbClock(HI_U32 ulAhb,HI_U32 ulApb);

/**
* @ingroup  hct_sys
* @brief 将工作模式写到工厂区NV中。
*
* @par 描述:
* 将工作模式写到工厂区NV中。
* @attention 
* @li 不支持中断中调用。
* @li 该接口在工装检测阶段使用，测试时将模式设置为测试模式，测试结束后设置成正常模式。
* @ll 模块重启后，会从NV中读取工作模式，并将当前模式设为该模式。
*
* @param  ulStatus [IN] 要写入的工作模式，取值:
* @li #HI_ND_SYS_STATUS_NORMAL:正常模式
* @li #HI_ND_SYS_STATUS_TEST:测试模式
*
* @retval #HI_ERR_SUCCESS 成功。
* @retval 其他值 失败，详见hi_errno.h。
*
* @par Dependency:
* @li hi_mdm_sys.h: 该接口声明所在的头文件。
* @see 无。
* @since HiMDL_V200R001C00
*/
HI_EXTERN HI_U32 HI_MDM_SetWorkStatus(HI_U32 ulStatus);

/**
* @ingroup  hct_sys
* @brief  重启单板。
*
* @par 描述:
* 重启单板。
* @attention 该接口受延时重启功能影响。
*
* @param  enCause [IN] 重启Id，客户预留ID范围[0x9000,0xa000)。重启ID用于查看系统重启原因时使用。
*
* @retval #HI_ERR_SUCCESS 成功。
* @retval 其他值 失败，详见hi_errno.h。
*
* @par Dependency:
* @li hi_mdm_sys.h: 该接口声明所在的头文件。
* @see  无。
* @since HiMDL_V200R001C00
*/
HI_EXTERN HI_U32 HI_MDM_Reboot(HI_SYS_REBOOT_CAUSE_E enCause);

/**
* @ingroup  hct_sys
* @brief  主动重启接口调用后，单板重启的时间设置，此时间段内保存系统信息。
*
* @par 描述:
* 主动重启接口调用后，单板重启的时间设置。此时间段内保存系统信息。
* @attention 无。
*
* @param  ulWdgTimeCfg [IN] 重启时间，单位:ms，参数设置范围:2000-30000。
*
* @retval #HI_ERR_SUCCESS 成功。
* @retval 其他值 失败，详见hi_errno.h。
*
* @par Dependency:
* @li hi_mdm_sys.h: 该接口声明所在的头文件。
* @see  无。
* @since HiMDL_V200R001C00
*/
HI_EXTERN HI_U32 HI_MDM_SetRebootDelay(HI_U32 ulWdgTimeCfg);

/**
* @ingroup  hct_sys
* @brief  延时重启功能使能。
*
* @par 描述:
* 延时重启功能使能。
* @attention 无。
*
* @param  enDelayCon [IN] 延时重启事件ID，取值范围[0,8)。
* @param  ms [IN] 延时重启时间(单位:ms)，最大支持5分钟。
*
* @retval #HI_ERR_SUCCESS 成功。
* @retval 其他值 失败，详见hi_errno.h。
*
* @par Dependency:
* @li hi_mdm_sys.h: 该接口声明所在的头文件。
* @see  无。
* @since HiMDL_V200R001C00
*/
HI_EXTERN HI_U32 HI_MDM_RstDelayEnable(HI_SYS_RST_DELAY_CON_E enDelayCon,HI_U32 ms);

/**
* @ingroup  hct_sys
* @brief  去延时重启。
*
* @par 描述:
* 去延时重启。
* @attention 无。
*
* @param  enDelayCon [IN] 延时重启事件ID,取值范围[0,8)。
*
* @retval #HI_ERR_SUCCESS 成功。
* @retval 其他值 失败，详见hi_errno.h。
*
* @par Dependency:
* @li hi_mdm_sys.h: 该接口声明所在的头文件。
* @see  无。
* @since HiMDL_V200R001C00
*/
HI_EXTERN HI_U32 HI_MDM_RstDelayDisable (HI_SYS_RST_DELAY_CON_E enDelayCon);

/**
* @ingroup  hct_sys
* @brief  主动复位注册函数。
*
* @par 描述:
* 主动复位注册函数。
* @attention 无。
*
* @param  pfnRstNotify [IN] 统计量查询函数。系统周期性调用该函数，
*查询统计量，检测到统计量超过预设时间没有变化进行主动复位。
* @param  enTrigerCon [IN] 主动复位事件ID。

* @param  reserved [IN] 保留值，暂不使用，调用该接口时传值为0即可。
*
* @retval #HI_ERR_SUCCESS 成功。
* @retval 其他值 失败，详见hi_errno.h。
*
* @par Dependency:
* @li hi_mdm_sys.h: 该接口声明所在的头文件。
* @see  无。
* @since HiMDL_V200R001C00
*/
HI_EXTERN HI_U32 HI_MDM_RstRegisterNotify (HI_CONST HI_SYS_RST_NOTIFY_F pfnRstNotify, HI_SYS_RST_TRIGGER_CON_E enTrigerCon,HI_U32 reserved);

/**
* @ingroup  hct_sys
* @brief  获取通信模块的产品ID, 包括MAC地址、芯片类型、产品形态等信息。
*
* @par 描述:
* 芯片类型、产品形态先从NV项中读取，如果读取失败则从芯片的寄存器中读取出来。
* @attention 无。
*
* @param  aucPlcMac[HI_PLC_MAC_ADDR_LEN] [OUT] 通信模块的MAC地址  。
* @param  penChipType [OUT] 通信模块中主芯片类型, 不需要该参数则传入HI_NULL。
* @param  penProductType [OUT] 通信模块的定制的产品形态, 不需要该参数则传入HI_NULL。
* @param  penHwPlatType [OUT] 该项功能已舍弃。
*
* @retval #HI_ERR_SUCCESS 成功。
* @retval 其他值 失败，详见hi_errno.h。
*
* @par Dependency:
* @li hi_mdm_sys.h: 该接口声明所在的头文件。
* @see  无。
* @since HiMDL_V200R001C00
*/
HI_EXTERN HI_U32 HI_MDM_GetProductId(HI_U8 aucPlcMac[HI_PLC_MAC_ADDR_LEN], HI_OUT HI_FTM_CHIP_TYPE_E* penChipType,HI_OUT HI_FTM_PRODUCT_TYPE_E* penProductType,HI_OUT HI_FTM_HW_PLATFORM_TYPE_E* penHwPlatType);

/**
* @ingroup  hct_sys
* @brief  获取模块的芯片类型，包括头端芯片类型、终端芯片类型。
*
* @par 描述:
* 芯片类型先从NV项中读取，如果读取失败则从芯片的寄存器中读取出来。
* @attention 无。
*
* @param 无。
*
* @retval #HI_FTM_CHIP_TYPE_3911C 头端芯片。
* @retval #HI_FTM_CHIP_TYPE_3911T 终端芯片。
*
* @par Dependency:
* @li hi_mdm_sys.h: 该接口声明所在的头文件。
* @see  无。
* @since HiMDL_V200R001C00
*/
HI_EXTERN HI_FTM_CHIP_TYPE_E HI_MDM_GetChipType(HI_VOID);

/**
* @ingroup  hct_sys
* @brief 设置APP版本信息。
*
* @par 描述:
* 该函数负责设置子模块版本信息，用户使用HSO的ver命令可以查看APP子模块版本信息。
* @attention 该接口需在APP初始化前调用。
*
* @param x [IN] 类型 #HI_U8 。应用x版本，最大可占用3byte。
* @param y [IN] 类型 #HI_U8 。应用y版本，最大可占用3byte。
* @param z [IN] 类型 #HI_U8 。应用z版本，最大可占用3byte。
*
* @retval #HI_ERR_SUCCESS 成功。
* @retval 其他值 失败，详见hi_errno.h。
*
* @par Dependency:
* @li hi_mdm_sys.h: 该接口声明所在的头文件。
* @see  无。
* @since HiMDL_V200R001C00
*/
#define HI_MDM_CONFIG_APP_VER(x,y,z)  HI_MDM_DfxSetVerInfo(HI_DFX_APP_VER, HI_SAL_SYS_DFX_VER_STR(x,y,z), HI_SAL_SYS_DFX_VER_BUILD_DATETIME())

/**
* @ingroup  hct_sys
* @brief  获取产品版本信息包括 芯片名称, 版本号等. 字符串格式。
*
* @par 描述:
* 获取产品版本信息包括 芯片名称, 版本号等. 字符串格式。
* @attention 无。
*
* @param 无。
*
* @retval #HI_PRODUCT_INFO_S*  类型地址 版本信息的地址 。
*
* @par Dependency:
* @li hi_mdm_sys.h: 该接口声明所在的头文件。
* @see  无。
* @since HiMDL_V200R001C00
*/
HI_EXTERN HI_CONST HI_PRODUCT_INFO_S* HI_MDM_GetProductVerInfo(HI_VOID);

/**
* @ingroup  hct_sys
* @brief  获取boot版本号
*
* @par 描述: 获取boot的版本号信息 boot版本号为xx.yy.zz 分别对应acBootVer的成员0、1、2(均16进制).
* @attention 无。
* @retval #0      success.
* @retval #非0     failed.详见hi_errno.h
* @par Dependency:
* <ul><li>hi_mdm_sys.h: 文件用于描述系统相关接口.</li></ul>
* @see  无。
* @since HiMDL_V200R001C00SPC010
*/
HI_EAPI HI_U32 HI_MDM_GetBootVer(HI_OUT HI_U8 acBootVer[3]);

/**
* @ingroup  hct_sys
* @brief  设置用户信息。
*
* @par 描述: 设置用户信息，包括软件版本号等。
* @attention 无。
* @retval #0      success.
* @retval #非0     failed.详见hi_errno.h
* @par Dependency:
* <ul><li>hi_mdm_sys.h: 文件用于描述系统相关接口.</li></ul>
* @see  无。
* @since HiMDL_V200R001C00SPC010
*/
HI_EAPI HI_U32 HI_MDM_SetUserInfo(HI_MDM_USER_INFO_S *pstUserInfo);

/**
* @ingroup  hct_sys
* @brief  获取用户信息。
*
* @par 描述: 获取用户信息，包括软件版本号等。
* @attention 无。
* @retval #0      success.
* @retval #非0     failed.详见hi_errno.h
* @par Dependency:
* <ul><li>hi_mdm_sys.h: 文件用于描述系统相关接口.</li></ul>
* @see  无。
* @since HiMDL_V200R001C00SPC010
*/
HI_EAPI HI_U32 HI_MDM_GetUserInfo(HI_MDM_USER_INFO_S *pstUserInfo);

/**
* @ingroup  hct_sys
* @brief  配置获取系统信息, 包括 系统状态、版本号等。
*
* @par 描述:
* 系统状态信息中部分内容不是静态的，需要实时调用该接口获取系统状态信息以便同步。
* @attention 无。
*
* @param 无。
*
* @retval #HI_MDM_SYS_INFO_S* 类型地址 系统信息的地址。
*
* @par Dependency:
* @li hi_mdm_sys.h: 该接口声明所在的头文件。
* @see  无。
* @since HiMDL_V200R001C00
*/
HI_EXTERN HI_CONST HI_MDM_SYS_INFO_S* HI_MDM_GetSysInfo(HI_VOID);


#ifdef __cplusplus
}
#endif
#endif
