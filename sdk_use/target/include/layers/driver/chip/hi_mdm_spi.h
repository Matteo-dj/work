/**@defgroup hct_spi SPI
 * @ingroup drivers
 */
 
#ifndef __HI_SPI_H__
#define __HI_SPI_H__
#include <hi_types.h>
#include <hi_mdm_types.h>

/**
 * @ingroup hct_spi
 * ͨ�ż���
 */
typedef enum
{
    HI_SPI_CFG_CLOCK_CPOL_0,
    HI_SPI_CFG_CLOCK_CPOL_1,
}HI_SPI_CFG_CLOCK_CPOL_E;

/**
 * @ingroup hct_spi
 * ͨ����λ
 */
typedef enum
{
    HI_SPI_CFG_CLOCK_CPHA_0,
    HI_SPI_CFG_CLOCK_CPHA_1,    
}HI_SPI_CFG_CLOCK_CPHA_E;

/**
 * @ingroup hct_spi
 * ͨ��Э������
 */
typedef enum
{
    HI_SPI_CFG_FRAM_MODE_MOTOROLA,  /**< Motorola SPI֡��ʽ*/
    HI_SPI_CFG_FRAM_MODE_TI,        /**< TI��˾��ͬ������֡��ʽ*/
    HI_SPI_CFG_FRAM_MODE_MICROWIRE, /**< National Microwire֡��ʽ*/
}HI_SPI_CFG_FRAM_MODE_E;

/**
 * @ingroup hct_spi
 * ͨ��λ��
 */
typedef enum
{
    HI_SPI_CFG_DATA_WIDTH_E_4BIT=0x3,/**< 4bitλ��*/
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
 * ͨ�Ų���:��С��
 */
typedef enum
{
    HI_SPI_CFG_ENDIAN_LITTLE,   /**< С��*/
    HI_SPI_CFG_ENDIAN_BIG,      /**< ���*/
}HI_SPI_CFG_ENDIAN_E;

/**
 * @ingroup hct_spi
 * ͨ�Ų���:�����豸
 */
typedef enum
{
    HI_SPI_CFG_ROLE_MASTER, /**< ���豸*/
    HI_SPI_CFG_ROLE_SLAVE,  /**< ���豸*/
}HI_SPI_CFG_ROLE_E;

/**
 * @ingroup hct_spi
 * ͨ�Ų���:MICROWIRE֡�ȴ�ʹ��
 */
typedef enum
{
    HI_SPI_CFG_WAIT_EN_DISABLE, /**< ��ֹ*/
    HI_SPI_CFG_WAIT_EN_ENABLE,  /**< ʹ��*/
}HI_SPI_CFG_WAIT_EN_E;

/**
 * @ingroup hct_spi
 * ͨ�Ų���:�Ի�ģʽ
 */
typedef enum
{
    HI_SPI_CFG_LBM_DISABLE, /**< ��ֹ*/
    HI_SPI_CFG_LBM_ENABLE,  /**< ʹ��*/
}HI_SPI_CFG_LBM_EN_E;

/**
 * @ingroup hct_spi
 * ͨ�Ų���:SPI ID��
 */
typedef enum
{
    HI_SPI_ID_0 = 0,/**< SPI0*/
    HI_SPI_ID_1,    /**< SPI1*/
    HI_SPI_ID_MAX
}HI_SPI_ID_E;

/**
* @ingroup  hct_spi
* @brief  spi�ص����������͡�
*
* @par ����:  
* spi�ص����������͡�
* @attention �ޡ�
*
* @param  data [IN] �ص���Ρ�
*
* @retval �ޡ�
*
* @par Dependency:
* @li hi_mdm_spi.h: �ýӿ��������ڵ�ͷ�ļ���
* @see �ޡ�
* @since HiMDL_V200R001C00
*/
typedef HI_VOID (*spi_usr_func)(HI_U32 data);

/**
 * @ingroup hct_spi
 * ����ͨ�Ų���
 */
typedef struct
{
    HI_U16 dataWidth:4;         /**< ����λ��,ȡֵ��HI_SPI_CFG_DATA_WIDTH_E            */
    HI_U16 framMode:2;          /**< ����Э��,ȡֵ��HI_SPI_CFG_FRAM_MODE_E           */
    HI_U16 spo:1;               /**< ����,ȡֵ��HI_SPI_CFG_CLOCK_CPOL_E           */
    HI_U16 sph:1;               /**< ��λ,ȡֵ��HI_SPI_CFG_CLOCK_CPHA_E          */
    HI_U16 reserver_1:8;        /**< ����λ         */

    HI_U16 reserver_2:4;        /**< ����λ         */
    HI_U16 endian:1;            /**< ��С��,ȡֵ��HI_SPI_CFG_ENDIAN_E         */
    HI_U16 reserver_3:3;
    HI_U16 waitVal:7;           /**< д�Ͷ�֮��ȴ�����:����microwireЭ��        */
    HI_U16 waitEn:1;            /**< �ȴ�ʹ��:����microwireЭ��,ȡֵ��HI_SPI_CFG_WAIT_EN_E        */
    
    HI_U32 baudRate;            /**< ������        */
    spi_usr_func prepareFunc;   /**< ����ͨ��ǰ�û�׼������       */
    spi_usr_func restoreFunc;   /**< ����ͨ�ź��û��ָ�����       */
}HI_SPI_DEVICE_USR_CFG_S;

/**
* @ingroup  hct_spi
* @brief  ͨ���ýӿڷ���SPI���ݣ������ϲ�ҵ��ֻ���Ͳ����ճ�����
*
* @par ����:  
* ͨ���ýӿڷ���SPI���ݣ������ϲ�ҵ��ֻ���Ͳ����ճ�����
* @attention �ޡ�
*
* @param  fd [IN] �豸��������
* @param  pSendData [IN] ��������ָ��,������λ��С��8bitʱ��
*�ڲ�����HI_U8*ָ�봦��������λ�����8bitʱ���ڲ�����HI_U16*ָ�봦��
* @param  ulDataLen [IN] ����֡������
* @param  usrData [IN] �û��ص�����������
*
* @retval #HI_ERR_SUCCESS �ɹ���
* @retval ����ֵ ʧ�ܣ����hi_errno.h��
*
* @par Dependency:
* @li hi_mdm_spi.h: �ýӿ��������ڵ�ͷ�ļ���
* @see  HI_MDM_SPI_SendData
* @since HiMDL_V200R001C00
*/
HI_EXTERN HI_U32 HI_MDM_SPI_SendData(HI_IN HI_U32 fd,HI_IN HI_PVOID pSendData,HI_IN HI_U32 ulDataLen ,HI_IN HI_U32 usrData);

/**
* @ingroup  hct_spi
* @brief  ͨ���ýӿڽ���SPI����,�����ϲ�ҵ��ֻ���ղ����ͳ�����
*
* @par ����:  
* ͨ���ýӿڽ���SPI���ݣ������ϲ�ҵ��ֻ���ղ����ͳ�����
* @attention �ޡ�
*
* @param  fd [IN] �豸��������
* @param  pRcvData [IN] ��������ָ�룬������λ��С��8bitʱ���ڲ�����#HI_U8*ָ�봦��
*������λ�����8bitʱ,�ڲ�����HI_U16*ָ�봦��
* @param  ulDataLen [IN] ����֡������
* @param  usrData [IN] �û��ص�����������
*
* @retval #HI_ERR_SUCCESS �ɹ���
* @retval ����ֵ ʧ�ܣ����hi_errno.h��
*
* @par Dependency:
* @li hi_mdm_spi.h: �ýӿ��������ڵ�ͷ�ļ���
* @see  HI_MDM_SPI_SendData
* @since HiMDL_V200R001C00
*/
HI_EXTERN HI_U32 HI_MDM_SPI_RecvData(HI_IN HI_U32 fd,HI_IN HI_PVOID pRcvData,HI_IN HI_U32 ulDataLen ,HI_IN HI_U32 usrData);

/**
* @ingroup  hct_spi
* @brief  ͨ���ýӿڷ��Ͳ�����SPI���ݡ�
*
* @par ����:  
* ͨ���ýӿڷ��Ͳ�����SPI���ݡ�
* @attention �ޡ�
*
* @param  fd [IN] �豸��������
* @param  pSendData [IN] ��������ָ�룬������λ��С��8bitʱ���ڲ�����#HI_U8*ָ�봦��
*������λ�����8bitʱ���ڲ�����#HI_U16*ָ�봦��
* @param  pRcvData [IN] ��������ָ��,������λ��С��8bitʱ���ڲ�����#HI_U8*ָ�봦��
*������λ�����8bitʱ���ڲ�����#HI_U16*ָ�봦��
* @param  ulDataLen [IN] ����֡������
* @param  usrData [IN] �û��ص�����������
*
* @retval #HI_ERR_SUCCESS �ɹ���
* @retval ����ֵ ʧ�ܣ����hi_errno.h��
*
* @par Dependency:
* @li hi_mdm_spi.h: �ýӿ��������ڵ�ͷ�ļ���
* @see  HI_MDM_SPI_SendData
* @since HiMDL_V200R001C00
*/
HI_EXTERN HI_U32 HI_MDM_SPI_SendRecvData(HI_IN HI_U32 fd,HI_IN HI_PVOID pSendData,HI_IN HI_PVOID pRcvData,HI_IN HI_U32 ulDataLen ,HI_IN HI_U32 usrData);

/**
* @ingroup  hct_spi
* @brief  ����SPI�豸ͨ�Ų�����
*
* @par ����:  
* ����SPI�豸ͨ�Ų�����
* @attention ��һ��ʹ���豸ͨ��ǰ���á�
*
* @param  fd [IN] �豸��������
* @param  pUsrArg [IN] �ⲿ�豸��SPI������Ҫ��
*
* @retval #HI_ERR_SUCCESS �ɹ���
* @retval ����ֵ ʧ�ܣ����hi_errno.h��
*
* @par Dependency:
* @li hi_mdm_spi.h: �ýӿ��������ڵ�ͷ�ļ���
* @see  �ޡ�
* @since HiMDL_V200R001C00
*/
HI_EXTERN HI_U32 HI_MDM_SPI_Config(HI_IN HI_U32 fd ,HI_IN HI_SPI_DEVICE_USR_CFG_S *pUsrArg);

/**
* @ingroup  hct_spi
* @brief  ��һ��SPI�豸��
*
* @par ����:  
* ��һ��SPI�豸��
* @attention �ޡ�
*
* @param  enSpiId [IN] SPI��š�
* @param  fd [OUT] �豸��������
*
* @retval #HI_ERR_SUCCESS �ɹ���
* @retval ����ֵ ʧ�ܣ����hi_errno.h��
*
* @par Dependency:
* @li hi_mdm_spi.h: �ýӿ��������ڵ�ͷ�ļ���
* @see  HI_MDM_SPI_DevClose
* @since HiMDL_V200R001C00
*/
HI_EXTERN HI_U32 HI_MDM_SPI_DevOpen(HI_IN HI_SPI_ID_E enSpiId, HI_OUT HI_U32 *pFd);

/**
* @ingroup  hct_spi
* @brief  �ر�һ��SPI�豸��������
*
* @par ����: 
* �ر�һ��SPI�豸��������
* @attention ���鳤�ڲ�ʹ�ø��豸ʱ�رգ���ҪƵ���򿪹رա�
*
* @param  fd [IN] �豸��������
*
* @retval #HI_ERR_SUCCESS �ɹ���
* @retval ����ֵ ʧ�ܣ����hi_errno.h��
*
* @par Dependency:
* @li hi_mdm_spi.h: �ýӿ��������ڵ�ͷ�ļ���
* @see  HI_MDM_SPI_DevClose
* @since HiMDL_V200R001C00
*/
HI_EXTERN HI_U32 HI_MDM_SPI_DevClose(HI_IN HI_U32 fd);

/**
* @ingroup  hct_spi
* @brief  spiģ���ʼ����
*
* @par ����:  
* spiģ���ʼ����
* @attention �ޡ�
*
* @param  enSpiId [IN] SPI��š�
*
* @retval #HI_ERR_SUCCESS �ɹ���
* @retval ����ֵ ʧ�ܣ����hi_errno.h��
*
* @par Dependency:
* @li hi_mdm_spi.h: �ýӿ��������ڵ�ͷ�ļ���
* @see �ޡ�
* @since HiMDL_V200R001C00
*/
HI_U32 HI_MDM_SPI_Init(HI_IN HI_SPI_ID_E enSpiId);
#endif
