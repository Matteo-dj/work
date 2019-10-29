/**@defgroup hct_uart 串口
 * @ingroup drivers
 */

#ifndef __HI_MDM_UART_H__
#define __HI_MDM_UART_H__

#include <hi_types.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @ingroup hct_uart
 * UART序号
 */
typedef enum 
{
    HI_UART_0,  /**< UART0*/ 
    HI_UART_1,  /**< UART1*/ 
    HI_UART_2,  /**< UART2*/ 
    HI_UART_3,  /**< UART3*/ 
    HI_UART_MAX
}HI_UART_PORT_E;

/**
 * @ingroup hct_uart
 * UART工作模式
 */
typedef enum 
{
    HI_UART_232 = 0,/**< 232模式*/ 
    HI_UART_IR,     /**< 红外模式*/ 
    HI_UART_485     /**< 485模式*/ 
}HI_UART_MODE_E;

/**
 * @ingroup hct_uart
 * UART控制命令
 */
#define HI_UART_CFG_SET_ATTR    0x101/**< UART 控制命令:配置UART参数        */ 
/**
 * @ingroup hct_uart
 * UART控制命令
 */
#define HI_UART_CFG_RD_BLOCK    0x102/**< UART 控制命令:锁定/解锁UART读        */ 
/**
 * @ingroup hct_uart
 * UART控制命令
 */
#define HI_UART_CFG_WD_BLOCK    0x103/**< UART 控制命令:锁定/解锁UART写        */ 
/**
 * @ingroup hct_uart
 * UART控制命令
 */
#define HI_UART_CFG_GET_ATTR    0x104/**< UART 控制命令:获取UART参数        */ 



/**
 * @ingroup hct_uart
 * UART读操作的控制命令参数
 */
#define HI_UART_RD_BLOCK    1/**< UART 控制命令参数:锁定UART读        */ 
/**
 * @ingroup hct_uart
 * UART读操作的控制命令参数
 */
#define HI_UART_RD_NONBLOCK    0/**< UART 控制命令参数:解锁UART读        */ 

/**
 * @ingroup hct_uart
 * UART读操作的控制命令参数
 */
#define HI_UART_WD_BLOCK    1/**< UART 控制命令参数:锁定UART写        */ 
/**
 * @ingroup hct_uart
 * UART读操作的控制命令参数
 */
#define HI_UART_WD_NONBLOCK    0/**< UART 控制命令参数:解锁UART写        */ 


/**
 * @ingroup hct_uart
 * UART停止位设定
 */
#define HI_UART_STOP_BIT_1 0x00/**< UART 停止位设定(寄存器值) :1位停止位       */ 
/**
 * @ingroup hct_uart
 * UART停止位设定
 */
#define HI_UART_STOP_BIT_1P5 0x04/**< UART 停止位设定(寄存器值) :1.5位停止位       */ 
/**
 * @ingroup hct_uart
 * UART停止位设定
 */
#define HI_UART_STOP_BIT_2 0x04/**< UART 停止位设定(寄存器值) :2位停止位       */ 

/**
 * @ingroup hct_uart
 * UART奇偶校验位设定
 */
#define HI_UART_PARITY_NONE 0x00/**< UART 奇偶校验设定(寄存器值) :无校验      */ 
/**
 * @ingroup hct_uart
 * UART奇偶校验位设定
 */
#define HI_UART_PARITY_ODD 0x08/**< UART 奇偶校验设定(寄存器值) :奇校验     */ 
/**
 * @ingroup hct_uart
 * UART奇偶校验位设定
 */
#define HI_UART_PARITY_EVEN 0x18/**< UART 奇偶校验设定(寄存器值) :偶校验       */ 



/**
 * @ingroup hct_uart
 * UART 设置uart参数，波特率等。
 */
#define UART_CFG_SET_ATTR    0x101
/**
 * @ingroup hct_uart
 * UART 配置uart读阻塞模式。
 */
#define UART_CFG_RD_BLOCK    0x102
/**
 * @ingroup hct_uart
 * UART 配置uart写阻塞模式。
 */
#define UART_CFG_WD_BLOCK    0x103
/**
 * @ingroup hct_uart
 * UART 获取uart配置属性。
 */
#define UART_CFG_GET_ATTR    0x104

/**
 * @ingroup hct_uart
 *UART维测统计量参数
 */
#define HI_UART_LAST_RECORD_BYTE_COUNT 32 /**< UART   维测中最后收发的字节数  */ 

/**
 * @ingroup hct_uart
 * UART 阻塞读模式
 */
#define UART_RD_BLOCK    1

/**
 * @ingroup hct_uart
 * UART 非阻塞读模式
 */
#define UART_RD_NONBLOCK    0

/**
 * @ingroup hct_uart
 * UART 阻塞写模式
 */

#define UART_WD_BLOCK    1

/**
 * @ingroup hct_uart
 * UART 非阻塞写模式
 */
#define UART_WD_NONBLOCK    0

/**
 * @ingroup hct_uart
 * UART 维测统计量
 */
typedef struct
{
    HI_U32 ulRcvIrqCnt;		/**<接收中断次数*/
    HI_U32 ulRcvDataCnt;	/**<接收中断中接收到的数据字节数*/          
    HI_U32 ulSendIrqCnt;	/**<发送中断次数*/    
    HI_U32 ulSendDataCnt;	/**<发送中断中发送的数据字节数*/    
    
    HI_U32 ulErrOverRun;	/**<接收溢出错误数*/    
    HI_U32 ulErrParity;		/**<接收奇偶校验错误数*/    
    HI_U32 ulErrFrame;		/**<接收帧错误数*/    
    HI_U32 ulErrBreak;		/**<接收打断错误数*/    
    HI_U32 ulErrBusy; 		/**<接收忙错误数*/    

    HI_U32 ulLastRcvSn;/**<最后接收数据的标号*/
    HI_U8 ucLastRcvData[HI_UART_LAST_RECORD_BYTE_COUNT];/**<接收缓存*/

    HI_U32 ulLastSendSn; /**<最后发送数据的标号*/ 
    HI_U8 ucLastSendData[HI_UART_LAST_RECORD_BYTE_COUNT];/**<发送缓存*/
}HI_UART_DXF_STAT_S;

/**
* @ingroup  hct_uart
* @brief  获取UART句柄。
*
* @par 描述: 
* 获取UART句柄。
* @attention 使用前先判断返回值是否正确。
*
* @param  enPortId [IN] UART端口号。
* @param  fd [OUT] 句柄值。
*
* @retval #HI_ERR_SUCCESS 获取句柄成功。
* @retval 其他值 获取句柄失败，错误码见hi_errno.h。
*
* @par Dependency:
* @li hi_mdm_uart.h: 该接口声明所在的头文件。
* @see  无。
* @since HiMDL_V200R001C00
*/
HI_U32 HI_MDM_UART_Getfd(HI_UART_PORT_E enPortId, HI_U32* fd);

/**
* @ingroup  hct_uart
* @brief  打开UART。
*
* @par 描述: 
* 根据设定的工作模式打开指定的UART。
* @attention 无。
*
* @param  enPortId [IN] UART端口号。
* @param enMode [IN] UART工作模式。
*
* @retval #HI_ERR_FAILURE  打开失败。
* @retval 其他整数 被打开UART的句柄值。
*
* @par Dependency:
* @li hi_mdm_uart.h: 该接口声明所在的头文件。
* @see  HI_MDM_UART_Close
* @since HiMDL_V200R001C00
*/
HI_U32 HI_MDM_UART_Open(HI_UART_PORT_E enPortId, HI_UART_MODE_E enMode);

/**
* @ingroup  hct_uart
* @brief  读数据。
*
* @par 描述: 
* 将UART接收到的数据读取出来。
* @attention 要在调用完HI_MDM_UART_Open函数之后使用，且与HI_MDM_UART_Close成对使用。
*
* @param  fd [IN] 待读数据的UART句柄。
* @param pData [OUT] 读到数据的首地址。
* @param ulDataSize [IN] 要读取数据的字节数。
*
* @retval #HI_ERR_FAILURE  读数据失败。
* @retval 其他整数 实际读到数据的字节数。
*
* @par Dependency:
* @li hi_mdm_uart.h: 该接口声明所在的头文件。
* @see  HI_MDM_UART_Write
* @since HiMDL_V200R001C00
*/
HI_U32 HI_MDM_UART_Read(HI_S32 fd, HI_U8* pData, HI_U32 ulDataSize);

/**
* @ingroup  hct_uart
* @brief  写数据。
*
* @par 描述: 
* 将待发送的数据写到UART。
* @attention 要在调用完HI_MDM_UART_Open函数之后使用。
*
* @param  fd [IN] 待写数据的UART句柄。
* @param pData [IN] 待写数据的首地址。
* @param ulDataSize [IN] 待写数据的字节数。
*
* @retval #HI_ERR_FAILURE  发送数据失败。
* @retval 其他整数 实际发送数据的字节数。
*
* @par Dependency:
* @li hi_mdm_uart.h: 该接口声明所在的头文件。
* @see  HI_MDM_UART_Read
* @since HiMDL_V200R001C00
*/
HI_U32 HI_MDM_UART_Write(HI_S32 fd, HI_U8* pData, HI_U32 ulDataSize);

/**
* @ingroup  hct_uart
* @brief  与UART控制相关的函数。
*
* @par 描述: 
* 与UART配置相关的函数。
* @attention 要在调用完HI_MDM_UART_Open函数之后使用。
*
* @param  fd [IN] 待操作UART的句柄。
* @param ulRequest [IN] 命令类型，取值为UART_CFG_XXX
* @param pAttr [IN/OUT] 与相应命令类型配合使用的参数。 
* @li UART_CFG_SET_ATTR 设置串口波特率等配置，pAttr为指向HI_UART_INIT_TYPE_S结构的指针。
* @li UART_CFG_GET_ATTR 获取串口波特率等配置，pAttr为指向HI_UART_INIT_TYPE_S结构的指针。
* @li UART_CFG_RD_BLOCK 设置串口读阻塞(非阻塞)模式，pAttr为指向HI_U32的指针，HI_U32取值为UART_RD_BLOCK，UART_RD_NONBLOCK。
* @li UART_CFG_WD_BLOCK 设置串口写阻塞(非阻塞)模式，pAttr为指向HI_U32的指针,HI_U32取值为UART_WD_BLOCK，UART_WD_NONBLOCK。
*
* @retval #HI_ERR_SUCCESS 成功。
* @retval 其他值 失败，详见hi_errno.h。
*
* @par Dependency:
* @li hi_mdm_uart.h: 该接口声明所在的头文件。
* @see  无。
* @since HiMDL_V200R001C00
*/
HI_U32 HI_MDM_UART_Ioctl(HI_S32 fd, HI_U32 ulRequest, HI_U32 pAttr);

/**
* @ingroup  hct_uart
* @brief  关闭UART。
*
* @par 描述: 
* 关闭UART。
* @attention 与HI_MDM_UART_Open成对使用。
*
* @param  fd [IN] 待关闭UART的句柄。
*
* @retval #HI_ERR_SUCCESS 成功。
* @retval 其他值 失败，详见hi_errno.h。
*
* @par Dependency:
* @li hi_mdm_uart.h: 该接口声明所在的头文件。
* @see  HI_MDM_UART_Open
* @since HiMDL_V200R001C00
*/
HI_U32 HI_MDM_UART_Close(HI_S32 fd);

/**
* @ingroup  hct_uart
* @brief  UART维测函数。
*
* @par 描述: 
* 获取UART参数与统计信息，用于维测。
* @attention 使用enPortId直接访问串口，无权限控制，只能用于维测。
*
* @param  enPortId [IN] 待获取维测信息的UART端口号。
* @param  pstCfg [OUT] 获取到的UART配置信息。
* @param  pstStat [OUT] 获取到的UART统计信息。
*
* @retval #HI_ERR_SUCCESS 成功。
* @retval 其他值 失败，详见hi_errno.h。
*
* @par Dependency:
* @li hi_mdm_uart.h: 该接口声明所在的头文件。
* @see  HI_MDM_UART_Open
* @since HiMDL_V200R001C00
*/
HI_U32 HI_MDM_UART_DfxGetStat(HI_UART_PORT_E enPortId,HI_UART_INIT_TYPE_S *pstCfg,HI_UART_DXF_STAT_S *pstStat);

#ifdef __cplusplus
}
#endif
#endif
