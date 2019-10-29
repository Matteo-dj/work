
#ifndef __HI_MDM_I2C_H__
#define __HI_MDM_I2C_H__
#include <hi_types.h>
#include <hi_mdm_types.h>

HI_START_HEADER
/**
 * @ingroup hct_mdm
 * I2C ID��
 */
typedef enum
{
    HI_I2C_IDX_0,
    HI_I2C_IDX_1,
    HI_I2C_IDX_MAX,
}HI_I2C_IDX_E;

/**
 * @ingroup hct_mdm
 * I2C����/��������������
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
* @brief  I2C������������ݡ�
*
* @par ����:
* I2C��ӻ��������ݣ�Ȼ����մӻ����ݡ�
* @attention δ�������񱣻�����֧�ֶ����񣩡�
*
* @param  enI2cId [IN] ���� #HI_I2C_IDX_E  I2C ��ID�š�
* @param  ucDevAddr [IN] ���� #HI_U8  �豸��ַ��
* @param  pstI2cData [IN] ���� #HI_I2C_DATA_S *  ������������������
*
* @retval #HI_ERR_SUCCESS �ɹ���
* @retval ����ֵ ʧ�ܣ����hi_errno.h��
*
* @par Dependency:
* @li hi_mdm_i2c.h: �ýӿ��������ڵ�ͷ�ļ���
* @see  �ޡ�
* @since HiMDL_V200R001C00
*/
#define HI_MDM_I2C_SendRecvData(enI2cId, ucDevAddr, pstI2cData) HI_I2C_SendRecvData(enI2cId, ucDevAddr, pstI2cData)

/**
* @ingroup  hct_I2C
* @brief  I2C�������ݡ�
*
* @par ����:
* I2C�������ݡ�
* @attention δ�������񱣻�����֧�ֶ����񣩡�
*
* @param  enI2cId [IN] ����#HI_I2C_IDX_E  I2C��ID�š�
* @param  ucDevAddr [IN] ����#HI_U8  �豸��ַ��
* @param  pstI2cData [IN] ����#HI_I2C_DATA_S *  ������������������
*
* @retval #HI_ERR_SUCCESS �ɹ���
* @retval ����ֵ ʧ�ܣ����hi_errno.h��
*
* @par Dependency:
* @li hi_mdm_i2c.h: �ýӿ��������ڵ�ͷ�ļ���
* @see �ޡ�
* @since HiMDL_V200R001C00
*/
#define HI_MDM_I2C_SendData(enI2cId, ucDevAddr, pstI2cData) HI_I2C_SendData(enI2cId, ucDevAddr, pstI2cData)

/**
* @ingroup  hct_I2C
* @brief  I2C �������ݡ�
*
* @par ����:
* I2C �������ݡ�
* @attention δ�������񱣻�����֧�ֶ����񣩡�
*
* @param  enI2cId [IN] ���� #HI_I2C_IDX_E  I2C��ID�š�
* @param  ucDevAddr [IN] ���� #HI_U8  �豸��ַ��
* @param  pstI2cData [IN] ���� #HI_I2C_DATA_S * ����������������
*
* @retval #HI_ERR_SUCCESS �ɹ���
* @retval ����ֵ ʧ�ܣ����hi_errno.h��
*
* @par Dependency:
* @li hi_mdm_i2c.h: �ýӿ��������ڵ�ͷ�ļ���
* @see �ޡ�
* @since HiMDL_V200R001C00
*/
#define HI_MDM_I2C_RecvData(enI2cId, ucDevAddr, pstI2cData) HI_I2C_RecvData(enI2cId, ucDevAddr, pstI2cData)

/**
* @ingroup  hct_I2C
* @brief  I2C ��ʼ����
*
* @par ����:
* I2C ��ʼ����
* @attention �ޡ�
*
* @param  enI2cId [IN] ���� #HI_I2C_IDX_E  I2C��ID�š�
*
* @retval #HI_ERR_SUCCESS �ɹ���
* @retval ����ֵ ʧ�ܣ����hi_errno.h��
*
* @par Dependency:
* @li hi_mdm_i2c.h: �ýӿ��������ڵ�ͷ�ļ���
* @see �ޡ�
* @since HiMDL_V200R001C00
*/
#define HI_MDM_I2C_Init(enI2cId) HI_I2C_Init(enI2cId)

/**
* @ingroup  hct_I2C
* @brief  ȥ��ʼ��I2C��
*
* @par ����:
* ȥ��ʼ��I2C��
* @attention HI_MDM_I2C_Init���ú���ʹ�á�
*
* @param  enI2cId [IN] ���� #HI_I2C_IDX_E  I2C��ID�š�
*
* @retval #HI_ERR_SUCCESS �ɹ���
* @retval ����ֵ ʧ�ܣ����hi_errno.h��
*
* @par Dependency:
* @li hi_mdm_i2c.h: �ýӿ��������ڵ�ͷ�ļ���
* @see �ޡ�
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
