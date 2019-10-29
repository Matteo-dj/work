/**@defgroup hct_uart ����
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
 * UART���
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
 * UART����ģʽ
 */
typedef enum 
{
    HI_UART_232 = 0,/**< 232ģʽ*/ 
    HI_UART_IR,     /**< ����ģʽ*/ 
    HI_UART_485     /**< 485ģʽ*/ 
}HI_UART_MODE_E;

/**
 * @ingroup hct_uart
 * UART��������
 */
#define HI_UART_CFG_SET_ATTR    0x101/**< UART ��������:����UART����        */ 
/**
 * @ingroup hct_uart
 * UART��������
 */
#define HI_UART_CFG_RD_BLOCK    0x102/**< UART ��������:����/����UART��        */ 
/**
 * @ingroup hct_uart
 * UART��������
 */
#define HI_UART_CFG_WD_BLOCK    0x103/**< UART ��������:����/����UARTд        */ 
/**
 * @ingroup hct_uart
 * UART��������
 */
#define HI_UART_CFG_GET_ATTR    0x104/**< UART ��������:��ȡUART����        */ 



/**
 * @ingroup hct_uart
 * UART�������Ŀ����������
 */
#define HI_UART_RD_BLOCK    1/**< UART �����������:����UART��        */ 
/**
 * @ingroup hct_uart
 * UART�������Ŀ����������
 */
#define HI_UART_RD_NONBLOCK    0/**< UART �����������:����UART��        */ 

/**
 * @ingroup hct_uart
 * UART�������Ŀ����������
 */
#define HI_UART_WD_BLOCK    1/**< UART �����������:����UARTд        */ 
/**
 * @ingroup hct_uart
 * UART�������Ŀ����������
 */
#define HI_UART_WD_NONBLOCK    0/**< UART �����������:����UARTд        */ 


/**
 * @ingroup hct_uart
 * UARTֹͣλ�趨
 */
#define HI_UART_STOP_BIT_1 0x00/**< UART ֹͣλ�趨(�Ĵ���ֵ) :1λֹͣλ       */ 
/**
 * @ingroup hct_uart
 * UARTֹͣλ�趨
 */
#define HI_UART_STOP_BIT_1P5 0x04/**< UART ֹͣλ�趨(�Ĵ���ֵ) :1.5λֹͣλ       */ 
/**
 * @ingroup hct_uart
 * UARTֹͣλ�趨
 */
#define HI_UART_STOP_BIT_2 0x04/**< UART ֹͣλ�趨(�Ĵ���ֵ) :2λֹͣλ       */ 

/**
 * @ingroup hct_uart
 * UART��żУ��λ�趨
 */
#define HI_UART_PARITY_NONE 0x00/**< UART ��żУ���趨(�Ĵ���ֵ) :��У��      */ 
/**
 * @ingroup hct_uart
 * UART��żУ��λ�趨
 */
#define HI_UART_PARITY_ODD 0x08/**< UART ��żУ���趨(�Ĵ���ֵ) :��У��     */ 
/**
 * @ingroup hct_uart
 * UART��żУ��λ�趨
 */
#define HI_UART_PARITY_EVEN 0x18/**< UART ��żУ���趨(�Ĵ���ֵ) :żУ��       */ 



/**
 * @ingroup hct_uart
 * UART ����uart�����������ʵȡ�
 */
#define UART_CFG_SET_ATTR    0x101
/**
 * @ingroup hct_uart
 * UART ����uart������ģʽ��
 */
#define UART_CFG_RD_BLOCK    0x102
/**
 * @ingroup hct_uart
 * UART ����uartд����ģʽ��
 */
#define UART_CFG_WD_BLOCK    0x103
/**
 * @ingroup hct_uart
 * UART ��ȡuart�������ԡ�
 */
#define UART_CFG_GET_ATTR    0x104

/**
 * @ingroup hct_uart
 *UARTά��ͳ��������
 */
#define HI_UART_LAST_RECORD_BYTE_COUNT 32 /**< UART   ά��������շ����ֽ���  */ 

/**
 * @ingroup hct_uart
 * UART ������ģʽ
 */
#define UART_RD_BLOCK    1

/**
 * @ingroup hct_uart
 * UART ��������ģʽ
 */
#define UART_RD_NONBLOCK    0

/**
 * @ingroup hct_uart
 * UART ����дģʽ
 */

#define UART_WD_BLOCK    1

/**
 * @ingroup hct_uart
 * UART ������дģʽ
 */
#define UART_WD_NONBLOCK    0

/**
 * @ingroup hct_uart
 * UART ά��ͳ����
 */
typedef struct
{
    HI_U32 ulRcvIrqCnt;		/**<�����жϴ���*/
    HI_U32 ulRcvDataCnt;	/**<�����ж��н��յ��������ֽ���*/          
    HI_U32 ulSendIrqCnt;	/**<�����жϴ���*/    
    HI_U32 ulSendDataCnt;	/**<�����ж��з��͵������ֽ���*/    
    
    HI_U32 ulErrOverRun;	/**<�������������*/    
    HI_U32 ulErrParity;		/**<������żУ�������*/    
    HI_U32 ulErrFrame;		/**<����֡������*/    
    HI_U32 ulErrBreak;		/**<���մ�ϴ�����*/    
    HI_U32 ulErrBusy; 		/**<����æ������*/    

    HI_U32 ulLastRcvSn;/**<���������ݵı��*/
    HI_U8 ucLastRcvData[HI_UART_LAST_RECORD_BYTE_COUNT];/**<���ջ���*/

    HI_U32 ulLastSendSn; /**<��������ݵı��*/ 
    HI_U8 ucLastSendData[HI_UART_LAST_RECORD_BYTE_COUNT];/**<���ͻ���*/
}HI_UART_DXF_STAT_S;

/**
* @ingroup  hct_uart
* @brief  ��ȡUART�����
*
* @par ����: 
* ��ȡUART�����
* @attention ʹ��ǰ���жϷ���ֵ�Ƿ���ȷ��
*
* @param  enPortId [IN] UART�˿ںš�
* @param  fd [OUT] ���ֵ��
*
* @retval #HI_ERR_SUCCESS ��ȡ����ɹ���
* @retval ����ֵ ��ȡ���ʧ�ܣ��������hi_errno.h��
*
* @par Dependency:
* @li hi_mdm_uart.h: �ýӿ��������ڵ�ͷ�ļ���
* @see  �ޡ�
* @since HiMDL_V200R001C00
*/
HI_U32 HI_MDM_UART_Getfd(HI_UART_PORT_E enPortId, HI_U32* fd);

/**
* @ingroup  hct_uart
* @brief  ��UART��
*
* @par ����: 
* �����趨�Ĺ���ģʽ��ָ����UART��
* @attention �ޡ�
*
* @param  enPortId [IN] UART�˿ںš�
* @param enMode [IN] UART����ģʽ��
*
* @retval #HI_ERR_FAILURE  ��ʧ�ܡ�
* @retval �������� ����UART�ľ��ֵ��
*
* @par Dependency:
* @li hi_mdm_uart.h: �ýӿ��������ڵ�ͷ�ļ���
* @see  HI_MDM_UART_Close
* @since HiMDL_V200R001C00
*/
HI_U32 HI_MDM_UART_Open(HI_UART_PORT_E enPortId, HI_UART_MODE_E enMode);

/**
* @ingroup  hct_uart
* @brief  �����ݡ�
*
* @par ����: 
* ��UART���յ������ݶ�ȡ������
* @attention Ҫ�ڵ�����HI_MDM_UART_Open����֮��ʹ�ã�����HI_MDM_UART_Close�ɶ�ʹ�á�
*
* @param  fd [IN] �������ݵ�UART�����
* @param pData [OUT] �������ݵ��׵�ַ��
* @param ulDataSize [IN] Ҫ��ȡ���ݵ��ֽ�����
*
* @retval #HI_ERR_FAILURE  ������ʧ�ܡ�
* @retval �������� ʵ�ʶ������ݵ��ֽ�����
*
* @par Dependency:
* @li hi_mdm_uart.h: �ýӿ��������ڵ�ͷ�ļ���
* @see  HI_MDM_UART_Write
* @since HiMDL_V200R001C00
*/
HI_U32 HI_MDM_UART_Read(HI_S32 fd, HI_U8* pData, HI_U32 ulDataSize);

/**
* @ingroup  hct_uart
* @brief  д���ݡ�
*
* @par ����: 
* �������͵�����д��UART��
* @attention Ҫ�ڵ�����HI_MDM_UART_Open����֮��ʹ�á�
*
* @param  fd [IN] ��д���ݵ�UART�����
* @param pData [IN] ��д���ݵ��׵�ַ��
* @param ulDataSize [IN] ��д���ݵ��ֽ�����
*
* @retval #HI_ERR_FAILURE  ��������ʧ�ܡ�
* @retval �������� ʵ�ʷ������ݵ��ֽ�����
*
* @par Dependency:
* @li hi_mdm_uart.h: �ýӿ��������ڵ�ͷ�ļ���
* @see  HI_MDM_UART_Read
* @since HiMDL_V200R001C00
*/
HI_U32 HI_MDM_UART_Write(HI_S32 fd, HI_U8* pData, HI_U32 ulDataSize);

/**
* @ingroup  hct_uart
* @brief  ��UART������صĺ�����
*
* @par ����: 
* ��UART������صĺ�����
* @attention Ҫ�ڵ�����HI_MDM_UART_Open����֮��ʹ�á�
*
* @param  fd [IN] ������UART�ľ����
* @param ulRequest [IN] �������ͣ�ȡֵΪUART_CFG_XXX
* @param pAttr [IN/OUT] ����Ӧ�����������ʹ�õĲ����� 
* @li UART_CFG_SET_ATTR ���ô��ڲ����ʵ����ã�pAttrΪָ��HI_UART_INIT_TYPE_S�ṹ��ָ�롣
* @li UART_CFG_GET_ATTR ��ȡ���ڲ����ʵ����ã�pAttrΪָ��HI_UART_INIT_TYPE_S�ṹ��ָ�롣
* @li UART_CFG_RD_BLOCK ���ô��ڶ�����(������)ģʽ��pAttrΪָ��HI_U32��ָ�룬HI_U32ȡֵΪUART_RD_BLOCK��UART_RD_NONBLOCK��
* @li UART_CFG_WD_BLOCK ���ô���д����(������)ģʽ��pAttrΪָ��HI_U32��ָ��,HI_U32ȡֵΪUART_WD_BLOCK��UART_WD_NONBLOCK��
*
* @retval #HI_ERR_SUCCESS �ɹ���
* @retval ����ֵ ʧ�ܣ����hi_errno.h��
*
* @par Dependency:
* @li hi_mdm_uart.h: �ýӿ��������ڵ�ͷ�ļ���
* @see  �ޡ�
* @since HiMDL_V200R001C00
*/
HI_U32 HI_MDM_UART_Ioctl(HI_S32 fd, HI_U32 ulRequest, HI_U32 pAttr);

/**
* @ingroup  hct_uart
* @brief  �ر�UART��
*
* @par ����: 
* �ر�UART��
* @attention ��HI_MDM_UART_Open�ɶ�ʹ�á�
*
* @param  fd [IN] ���ر�UART�ľ����
*
* @retval #HI_ERR_SUCCESS �ɹ���
* @retval ����ֵ ʧ�ܣ����hi_errno.h��
*
* @par Dependency:
* @li hi_mdm_uart.h: �ýӿ��������ڵ�ͷ�ļ���
* @see  HI_MDM_UART_Open
* @since HiMDL_V200R001C00
*/
HI_U32 HI_MDM_UART_Close(HI_S32 fd);

/**
* @ingroup  hct_uart
* @brief  UARTά�⺯����
*
* @par ����: 
* ��ȡUART������ͳ����Ϣ������ά�⡣
* @attention ʹ��enPortIdֱ�ӷ��ʴ��ڣ���Ȩ�޿��ƣ�ֻ������ά�⡣
*
* @param  enPortId [IN] ����ȡά����Ϣ��UART�˿ںš�
* @param  pstCfg [OUT] ��ȡ����UART������Ϣ��
* @param  pstStat [OUT] ��ȡ����UARTͳ����Ϣ��
*
* @retval #HI_ERR_SUCCESS �ɹ���
* @retval ����ֵ ʧ�ܣ����hi_errno.h��
*
* @par Dependency:
* @li hi_mdm_uart.h: �ýӿ��������ڵ�ͷ�ļ���
* @see  HI_MDM_UART_Open
* @since HiMDL_V200R001C00
*/
HI_U32 HI_MDM_UART_DfxGetStat(HI_UART_PORT_E enPortId,HI_UART_INIT_TYPE_S *pstCfg,HI_UART_DXF_STAT_S *pstStat);

#ifdef __cplusplus
}
#endif
#endif
