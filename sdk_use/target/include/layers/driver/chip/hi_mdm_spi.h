/**@defgroup hct_spi SPI
 * @ingroup drivers
 */
 
#ifndef __HI_SPI_H__
#define __HI_SPI_H__
#include <hi_types.h>
#include <hi_mdm_types.h>

/**
 * @ingroup hct_spi
 * 通信极性
 */
typedef enum
{
    HI_SPI_CFG_CLOCK_CPOL_0,
    HI_SPI_CFG_CLOCK_CPOL_1,
}HI_SPI_CFG_CLOCK_CPOL_E;

/**
 * @ingroup hct_spi
 * 通信相位
 */
typedef enum
{
    HI_SPI_CFG_CLOCK_CPHA_0,
    HI_SPI_CFG_CLOCK_CPHA_1,    
}HI_SPI_CFG_CLOCK_CPHA_E;

/**
 * @ingroup hct_spi
 * 通信协议类型
 */
typedef enum
{
    HI_SPI_CFG_FRAM_MODE_MOTOROLA,  /**< Motorola SPI帧格式*/
    HI_SPI_CFG_FRAM_MODE_TI,        /**< TI公司的同步串行帧格式*/
    HI_SPI_CFG_FRAM_MODE_MICROWIRE, /**< National Microwire帧格式*/
}HI_SPI_CFG_FRAM_MODE_E;

/**
 * @ingroup hct_spi
 * 通信位宽
 */
typedef enum
{
    HI_SPI_CFG_DATA_WIDTH_E_4BIT=0x3,/**< 4bit位宽*/
    HI_SPI_CFG_DATA_WIDTH_E_5BIT,
    HI_SPI_CFG_DATA_WIDTH_E_6BIT,
    HI_SPI_CFG_DATA_WIDTH_E_7BIT,
    HI_SPI_CFG_DATA_WIDTH_E_8BIT,
    HI_SPI_CFG_DATA_WIDTH_E_9BIT,
    HI_SPI_CFG_DATA_WIDTH_E_10BIT,
    HI_SPI_CFG_DATA_WIDTH_E_11BIT,
    HI_SPI_CFG_DATA_WIDTH_E_12BIT,
    HI_SPI_CFG_DATA_WIDTH_E_13BIT,
    HI_SPI_CFG_DATA_WIDTH_E_14BIT,
    HI_SPI_CFG_DATA_WIDTH_E_15BIT,
    HI_SPI_CFG_DATA_WIDTH_E_16BIT,
}HI_SPI_CFG_DATA_WIDTH_E;

/**
 * @ingroup hct_spi
 * 通信参数:大小端
 */
typedef enum
{
    HI_SPI_CFG_ENDIAN_LITTLE,   /**< 小端*/
    HI_SPI_CFG_ENDIAN_BIG,      /**< 大端*/
}HI_SPI_CFG_ENDIAN_E;

/**
 * @ingroup hct_spi
 * 通信参数:主从设备
 */
typedef enum
{
    HI_SPI_CFG_ROLE_MASTER, /**< 主设备*/
    HI_SPI_CFG_ROLE_SLAVE,  /**< 从设备*/
}HI_SPI_CFG_ROLE_E;

/**
 * @ingroup hct_spi
 * 通信参数:MICROWIRE帧等待使能
 */
typedef enum
{
    HI_SPI_CFG_WAIT_EN_DISABLE, /**< 禁止*/
    HI_SPI_CFG_WAIT_EN_ENABLE,  /**< 使能*/
}HI_SPI_CFG_WAIT_EN_E;

/**
 * @ingroup hct_spi
 * 通信参数:自环模式
 */
typedef enum
{
    HI_SPI_CFG_LBM_DISABLE, /**< 禁止*/
    HI_SPI_CFG_LBM_ENABLE,  /**< 使能*/
}HI_SPI_CFG_LBM_EN_E;

/**
 * @ingroup hct_spi
 * 通信参数:SPI ID号
 */
typedef enum
{
    HI_SPI_ID_0 = 0,/**< SPI0*/
    HI_SPI_ID_1,    /**< SPI1*/
    HI_SPI_ID_MAX
}HI_SPI_ID_E;

/**
* @ingroup  hct_spi
* @brief  spi回调函数的类型。
*
* @par 描述:  
* spi回调函数的类型。
* @attention 无。
*
* @param  data [IN] 回调入参。
*
* @retval 无。
*
* @par Dependency:
* @li hi_mdm_spi.h: 该接口声明所在的头文件。
* @see 无。
* @since HiMDL_V200R001C00
*/
typedef HI_VOID (*spi_usr_func)(HI_U32 data);

/**
 * @ingroup hct_spi
 * 数据通信参数
 */
typedef struct
{
    HI_U16 dataWidth:4;         /**< 数据位宽,取值见HI_SPI_CFG_DATA_WIDTH_E            */
    HI_U16 framMode:2;          /**< 数据协议,取值见HI_SPI_CFG_FRAM_MODE_E           */
    HI_U16 spo:1;               /**< 极性,取值见HI_SPI_CFG_CLOCK_CPOL_E           */
    HI_U16 sph:1;               /**< 相位,取值见HI_SPI_CFG_CLOCK_CPHA_E          */
    HI_U16 reserver_1:8;        /**< 保留位         */

    HI_U16 reserver_2:4;        /**< 保留位         */
    HI_U16 endian:1;            /**< 大小端,取值见HI_SPI_CFG_ENDIAN_E         */
    HI_U16 reserver_3:3;
    HI_U16 waitVal:7;           /**< 写和读之间等待节拍:用于microwire协议        */
    HI_U16 waitEn:1;            /**< 等待使能:用于microwire协议,取值见HI_SPI_CFG_WAIT_EN_E        */
    
    HI_U32 baudRate;            /**< 波特率        */
    spi_usr_func prepareFunc;   /**< 数据通信前用户准备函数       */
    spi_usr_func restoreFunc;   /**< 数据通信后用户恢复函数       */
}HI_SPI_DEVICE_USR_CFG_S;

/**
* @ingroup  hct_spi
* @brief  通过该接口发送SPI数据，用于上层业务只发送不接收场景。
*
* @par 描述:  
* 通过该接口发送SPI数据，用于上层业务只发送不接收场景。
* @attention 无。
*
* @param  fd [IN] 设备描述符。
* @param  pSendData [IN] 发送数据指针,当数据位宽小于8bit时，
*内部当成HI_U8*指针处理，当数据位宽大于8bit时，内部按照HI_U16*指针处理。
* @param  ulDataLen [IN] 数据帧个数。
* @param  usrData [IN] 用户回调函数参数。
*
* @retval #HI_ERR_SUCCESS 成功。
* @retval 其他值 失败，详见hi_errno.h。
*
* @par Dependency:
* @li hi_mdm_spi.h: 该接口声明所在的头文件。
* @see  HI_MDM_SPI_SendData
* @since HiMDL_V200R001C00
*/
HI_EXTERN HI_U32 HI_MDM_SPI_SendData(HI_IN HI_U32 fd,HI_IN HI_PVOID pSendData,HI_IN HI_U32 ulDataLen ,HI_IN HI_U32 usrData);

/**
* @ingroup  hct_spi
* @brief  通过该接口接收SPI数据,用于上层业务只接收不发送场景。
*
* @par 描述:  
* 通过该接口接收SPI数据，用于上层业务只接收不发送场景。
* @attention 无。
*
* @param  fd [IN] 设备描述符。
* @param  pRcvData [IN] 接收数据指针，当数据位宽小于8bit时，内部当成#HI_U8*指针处理，
*当数据位宽大于8bit时,内部按照HI_U16*指针处理。
* @param  ulDataLen [IN] 数据帧个数。
* @param  usrData [IN] 用户回调函数参数。
*
* @retval #HI_ERR_SUCCESS 成功。
* @retval 其他值 失败，详见hi_errno.h。
*
* @par Dependency:
* @li hi_mdm_spi.h: 该接口声明所在的头文件。
* @see  HI_MDM_SPI_SendData
* @since HiMDL_V200R001C00
*/
HI_EXTERN HI_U32 HI_MDM_SPI_RecvData(HI_IN HI_U32 fd,HI_IN HI_PVOID pRcvData,HI_IN HI_U32 ulDataLen ,HI_IN HI_U32 usrData);

/**
* @ingroup  hct_spi
* @brief  通过该接口发送并接收SPI数据。
*
* @par 描述:  
* 通过该接口发送并接收SPI数据。
* @attention 无。
*
* @param  fd [IN] 设备描述符。
* @param  pSendData [IN] 发送数据指针，当数据位宽小于8bit时，内部当成#HI_U8*指针处理，
*当数据位宽大于8bit时，内部按照#HI_U16*指针处理。
* @param  pRcvData [IN] 接收数据指针,当数据位宽小于8bit时，内部当成#HI_U8*指针处理，
*当数据位宽大于8bit时，内部按照#HI_U16*指针处理。
* @param  ulDataLen [IN] 数据帧个数。
* @param  usrData [IN] 用户回调函数参数。
*
* @retval #HI_ERR_SUCCESS 成功。
* @retval 其他值 失败，详见hi_errno.h。
*
* @par Dependency:
* @li hi_mdm_spi.h: 该接口声明所在的头文件。
* @see  HI_MDM_SPI_SendData
* @since HiMDL_V200R001C00
*/
HI_EXTERN HI_U32 HI_MDM_SPI_SendRecvData(HI_IN HI_U32 fd,HI_IN HI_PVOID pSendData,HI_IN HI_PVOID pRcvData,HI_IN HI_U32 ulDataLen ,HI_IN HI_U32 usrData);

/**
* @ingroup  hct_spi
* @brief  配置SPI设备通信参数。
*
* @par 描述:  
* 配置SPI设备通信参数。
* @attention 第一次使用设备通信前调用。
*
* @param  fd [IN] 设备描述符。
* @param  pUsrArg [IN] 外部设备对SPI的配置要求。
*
* @retval #HI_ERR_SUCCESS 成功。
* @retval 其他值 失败，详见hi_errno.h。
*
* @par Dependency:
* @li hi_mdm_spi.h: 该接口声明所在的头文件。
* @see  无。
* @since HiMDL_V200R001C00
*/
HI_EXTERN HI_U32 HI_MDM_SPI_Config(HI_IN HI_U32 fd ,HI_IN HI_SPI_DEVICE_USR_CFG_S *pUsrArg);

/**
* @ingroup  hct_spi
* @brief  打开一个SPI设备。
*
* @par 描述:  
* 打开一个SPI设备。
* @attention 无。
*
* @param  enSpiId [IN] SPI编号。
* @param  fd [OUT] 设备描述符。
*
* @retval #HI_ERR_SUCCESS 成功。
* @retval 其他值 失败，详见hi_errno.h。
*
* @par Dependency:
* @li hi_mdm_spi.h: 该接口声明所在的头文件。
* @see  HI_MDM_SPI_DevClose
* @since HiMDL_V200R001C00
*/
HI_EXTERN HI_U32 HI_MDM_SPI_DevOpen(HI_IN HI_SPI_ID_E enSpiId, HI_OUT HI_U32 *pFd);

/**
* @ingroup  hct_spi
* @brief  关闭一个SPI设备描述符。
*
* @par 描述: 
* 关闭一个SPI设备描述符。
* @attention 建议长期不使用该设备时关闭，不要频繁打开关闭。
*
* @param  fd [IN] 设备描述符。
*
* @retval #HI_ERR_SUCCESS 成功。
* @retval 其他值 失败，详见hi_errno.h。
*
* @par Dependency:
* @li hi_mdm_spi.h: 该接口声明所在的头文件。
* @see  HI_MDM_SPI_DevClose
* @since HiMDL_V200R001C00
*/
HI_EXTERN HI_U32 HI_MDM_SPI_DevClose(HI_IN HI_U32 fd);

/**
* @ingroup  hct_spi
* @brief  spi模块初始化。
*
* @par 描述:  
* spi模块初始化。
* @attention 无。
*
* @param  enSpiId [IN] SPI编号。
*
* @retval #HI_ERR_SUCCESS 成功。
* @retval 其他值 失败，详见hi_errno.h。
*
* @par Dependency:
* @li hi_mdm_spi.h: 该接口声明所在的头文件。
* @see 无。
* @since HiMDL_V200R001C00
*/
HI_U32 HI_MDM_SPI_Init(HI_IN HI_SPI_ID_E enSpiId);
#endif
