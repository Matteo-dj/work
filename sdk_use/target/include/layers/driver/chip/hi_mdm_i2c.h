
#ifndef __HI_MDM_I2C_H__
#define __HI_MDM_I2C_H__
#include <hi_types.h>
#include <hi_mdm_types.h>

HI_START_HEADER
/**
 * @ingroup hct_mdm
 * I2C ID号
 */
typedef enum
{
    HI_I2C_IDX_0,
    HI_I2C_IDX_1,
    HI_I2C_IDX_MAX,
}HI_I2C_IDX_E;

/**
 * @ingroup hct_mdm
 * I2C发送/接收数据描述符
 */
typedef struct
{
	HI_U8* pSendBuf;
	HI_U32 ulSendLength;
	HI_U8* pReceiveBuf;
	HI_U32 ulReceiveLength;
}HI_I2C_DATA_S;



/**@defgroup hct_I2C I2C
 * @ingroup mdm
 */


/**
* @ingroup  hct_I2C
* @brief  I2C发送与接收数据。
*
* @par 描述:
* I2C向从机发送数据，然后接收从机数据。
* @attention 未作多任务保护（不支持多任务）。
*
* @param  enI2cId [IN] 类型 #HI_I2C_IDX_E  I2C 的ID号。
* @param  ucDevAddr [IN] 类型 #HI_U8  设备地址。
* @param  pstI2cData [IN] 类型 #HI_I2C_DATA_S *  待接收数据描述符。
*
* @retval #HI_ERR_SUCCESS 成功。
* @retval 其他值 失败，详见hi_errno.h。
*
* @par Dependency:
* @li hi_mdm_i2c.h: 该接口声明所在的头文件。
* @see  无。
* @since HiMDL_V200R001C00
*/
#define HI_MDM_I2C_SendRecvData(enI2cId, ucDevAddr, pstI2cData) HI_I2C_SendRecvData(enI2cId, ucDevAddr, pstI2cData)

/**
* @ingroup  hct_I2C
* @brief  I2C发送数据。
*
* @par 描述:
* I2C发送数据。
* @attention 未作多任务保护（不支持多任务）。
*
* @param  enI2cId [IN] 类型#HI_I2C_IDX_E  I2C的ID号。
* @param  ucDevAddr [IN] 类型#HI_U8  设备地址。
* @param  pstI2cData [IN] 类型#HI_I2C_DATA_S *  待发送数据描述符。
*
* @retval #HI_ERR_SUCCESS 成功。
* @retval 其他值 失败，详见hi_errno.h。
*
* @par Dependency:
* @li hi_mdm_i2c.h: 该接口声明所在的头文件。
* @see 无。
* @since HiMDL_V200R001C00
*/
#define HI_MDM_I2C_SendData(enI2cId, ucDevAddr, pstI2cData) HI_I2C_SendData(enI2cId, ucDevAddr, pstI2cData)

/**
* @ingroup  hct_I2C
* @brief  I2C 接收数据。
*
* @par 描述:
* I2C 接收数据。
* @attention 未作多任务保护（不支持多任务）。
*
* @param  enI2cId [IN] 类型 #HI_I2C_IDX_E  I2C的ID号。
* @param  ucDevAddr [IN] 类型 #HI_U8  设备地址。
* @param  pstI2cData [IN] 类型 #HI_I2C_DATA_S * 接收数据描述符。
*
* @retval #HI_ERR_SUCCESS 成功。
* @retval 其他值 失败，详见hi_errno.h。
*
* @par Dependency:
* @li hi_mdm_i2c.h: 该接口声明所在的头文件。
* @see 无。
* @since HiMDL_V200R001C00
*/
#define HI_MDM_I2C_RecvData(enI2cId, ucDevAddr, pstI2cData) HI_I2C_RecvData(enI2cId, ucDevAddr, pstI2cData)

/**
* @ingroup  hct_I2C
* @brief  I2C 初始化。
*
* @par 描述:
* I2C 初始化。
* @attention 无。
*
* @param  enI2cId [IN] 类型 #HI_I2C_IDX_E  I2C的ID号。
*
* @retval #HI_ERR_SUCCESS 成功。
* @retval 其他值 失败，详见hi_errno.h。
*
* @par Dependency:
* @li hi_mdm_i2c.h: 该接口声明所在的头文件。
* @see 无。
* @since HiMDL_V200R001C00
*/
#define HI_MDM_I2C_Init(enI2cId) HI_I2C_Init(enI2cId)

/**
* @ingroup  hct_I2C
* @brief  去初始化I2C。
*
* @par 描述:
* 去初始化I2C。
* @attention HI_MDM_I2C_Init调用后再使用。
*
* @param  enI2cId [IN] 类型 #HI_I2C_IDX_E  I2C的ID号。
*
* @retval #HI_ERR_SUCCESS 成功。
* @retval 其他值 失败，详见hi_errno.h。
*
* @par Dependency:
* @li hi_mdm_i2c.h: 该接口声明所在的头文件。
* @see 无。
* @since HiMDL_V200R001C00
*/
#define HI_MDM_I2C_DeInit(enI2cId) HI_I2C_DeInit(enI2cId)


HI_U32 HI_I2C_SendRecvData(HI_I2C_IDX_E enI2cId, HI_U8 ucDevAddr, HI_I2C_DATA_S *pstI2cData);
HI_U32 HI_I2C_SendData(HI_I2C_IDX_E enI2cId, HI_U8 ucDevAddr, HI_I2C_DATA_S *pstI2cData);
HI_U32 HI_I2C_ReceiveData(HI_I2C_IDX_E enI2cId, HI_U8 ucDevAddr, HI_I2C_DATA_S *pstI2cData);
HI_U32 HI_I2C_Init(HI_I2C_IDX_E enI2cId);
HI_U32 HI_I2C_DeInit(HI_I2C_IDX_E enI2cId);
HI_END_HEADER
#endif
