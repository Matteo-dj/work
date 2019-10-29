/**@defgroup hct_cipher ���ܽ���
 * @ingroup drivers
 */

#ifndef __HI_MDM_CIPHER_H__
#define __HI_MDM_CIPHER_H__

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif  // __cplusplus


/**
 * @ingroup hct_cipher
 * HASH�㷨����
 */
typedef enum
{
    HI_MDM_HASH_TYPE_SHA1,
    HI_MDM_HASH_TYPE_SHA256,
    HI_MDM_HASH_TYPE_HMAC_SHA1,
    HI_MDM_HASH_TYPE_HMAC_SHA256,
    HI_MDM_HASH_TYPE_MAX,
} HI_MDM_HASH_TYPE_E;

/**
 * @ingroup hct_cipher
 * HASH HMACģʽ���Ƿ�ʹ�ð�ȫ����ģʽ�����Ƿ�ʹ��Ӳ��key
 */
typedef enum
{
    HI_CIPHER_HMAC_KEY_FROM_CA,
    HI_CIPHER_HMAC_KEY_FROM_CPU,
    HI_CIPHER_HMAC_KEY_FROM_MAX,
}HASH_HMAC_KEY_FROM_E;

/**
 * @ingroup hct_cipher
 * cipher ����λ��ģʽ
 */
typedef enum hiHI_UNF_CIPHER_BIT_WIDTH_E
{
    HI_UNF_CIPHER_BIT_WIDTH_64BIT   = 0x0,  /**< 64bitλ�� */
    HI_UNF_CIPHER_BIT_WIDTH_8BIT    = 0x1,  /**< 8bitλ�� */
    HI_UNF_CIPHER_BIT_WIDTH_1BIT    = 0x2,  /**< 1bitλ�� */
    HI_UNF_CIPHER_BIT_WIDTH_128BIT  = 0x3,  /**< 128bitλ�� */
    HI_UNF_CIPHER_BIT_WIDTH_MAX
}HI_UNF_CIPHER_BIT_WIDTH_E;

/**
 * @ingroup hct_cipher
 * cipher ��Կ����
 */
typedef enum hiHI_UNF_CIPHER_KEY_LENGTH_E
{
    HI_UNF_CIPHER_KEY_AES_128BIT    = 0x0,  /**< AES���㷽ʽ�²���128bit��Կ���� */
    HI_UNF_CIPHER_KEY_AES_192BIT    = 0x1,  /**< AES���㷽ʽ�²���192bit��Կ���� */
    HI_UNF_CIPHER_KEY_AES_256BIT    = 0x2,  /**< AES���㷽ʽ�²���256bit��Կ���� */
    HI_UNF_CIPHER_KEY_DES_3KEY      = 0x2,  /**< DES���㷽ʽ�²���3��key */
    HI_UNF_CIPHER_KEY_DES_2KEY      = 0x3,  /**< DES���㷽ʽ�²���2��key  */
    HI_UNF_CIPHER_KEY_MAX,
}HI_UNF_CIPHER_KEY_LENGTH_E;

/**
 * @ingroup hct_cipher
 * cipher �����㷨
 */
typedef enum hiHI_UNF_CIPHER_ALG_E
{
    HI_UNF_CIPHER_ALG_DES           = 0x0,  /**< DES�㷨 */
    HI_UNF_CIPHER_ALG_3DES          = 0x1,  /**< 3DES�㷨 */
    HI_UNF_CIPHER_ALG_AES           = 0x2,  /**< AES�㷨 */
    HI_UNF_CIPHER_ALG_MAX           = 0x3
}HI_UNF_CIPHER_ALG_E;

/**
 * @ingroup hct_cipher
 * cipher ����ģʽ
 */
typedef enum hiHI_UNF_CIPHER_WORK_MODE_E
{
    HI_UNF_CIPHER_WORK_MODE_ECB,        /**< ECBģʽ */
    HI_UNF_CIPHER_WORK_MODE_CBC,        /**< CBCģʽ */
    HI_UNF_CIPHER_WORK_MODE_CFB,        /**< CFBģʽ */
    HI_UNF_CIPHER_WORK_MODE_OFB,        /**< OFBģʽ */
    HI_UNF_CIPHER_WORK_MODE_CTR,        /**< CTRģʽ */
    HI_UNF_CIPHER_WORK_MODE_CBC_CTS,    /**< CBC-CTSģʽ */
    HI_UNF_CIPHER_WORK_MODE_MAX
}HI_UNF_CIPHER_WORK_MODE_E;

/**
 * @ingroup hct_cipher
 * cipher ��������
 */
typedef struct
{
    HI_UNF_CIPHER_ALG_E enAlg;                      /**< �����㷨 */   
    HI_UNF_CIPHER_WORK_MODE_E enWorkMode;           /**< ����ģʽ */                                                                                                /**< CNcomment:����ģʽ */
    HI_UNF_CIPHER_BIT_WIDTH_E enBitWidth;           /**< ����λ��ģʽ*/                                                                        /**< CNcomment:���ܻ���ܵ�λ�� */
    HI_UNF_CIPHER_KEY_LENGTH_E enKeyLen;            /**< ��Կ���� */                                                                                                    /**< CNcomment:��Կ���� */
} HI_CIPHER_MODE_CTRL_S;

/**
 * @ingroup hct_cipher
 * cipher ���ܿ�����Ϣ
 */
typedef struct hiHI_CIPHER_CTRL_S
{
    HI_U32 u32Key[8];                   /**< ��Կ*/                                                                                        
    HI_U32 u32IV[4];                    /**< ����IV */                                                                        
    HI_BOOL bKeyByCA;                   /**< �Ƿ�ʹ�ø߰�ȫCA�����Ƿ�ʹ��Ӳ��Key */
    HI_BOOL bIVChangeFlag;              /**< �Ƿ�ʹ�û��ܺͽ��������� */    
    HI_U8 ucRsv[2];                     /**< ����λ */    
    HI_CIPHER_MODE_CTRL_S stModeCtrl;   /**< ģʽ���� */   
} HI_CIPHER_CTRL_S;

/**
 * @ingroup hct_cipher
 * cipher ������Ϣ
 */
typedef struct  hiCIPHER_DATA_S
{
    HI_HANDLE pCIHandle;    /**< ����� */     
    HI_U32 u32SrcAddr;      /**< ��������Դ��ַ�� */    
    HI_U32 u32DestAddr;     /**< ��������Ŀ�ĵ�ַ�� */    
    HI_U32 u32DataLength;   /**< �������ݳ��ȡ� */    
}CIPHER_DATA_S;

/**
 * @ingroup hct_cipher
 * cipher �ӽ���ͨ������ѡ��
 */
typedef enum
{
    HI_UNF_CIPHER_TYPE_NORMAL = 0x0,    /**< ��ͨ�� ͨ��1-ͨ��7*/  
    HI_UNF_CIPHER_TYPE_SINGLE_CHANNEL,  /**< ��ͨ�� ͨ��0*/ 
    HI_UNF_CIPHER_TYPE_MAX,
}HI_CIPHER_TYPE_E;

/**
 * @ingroup hct_cipher
 * cipher �ӽ���ͨ������ѡ��
 */
typedef struct
{
    HI_CIPHER_TYPE_E enCipherType;
}HI_UNF_CIPHER_ATTS_S;

/**
 * @ingroup hct_cipher
 * cipher �ӽ��ܼӽ���������Ϣ
 */
typedef struct hiHI_UNF_CIPHER_DATA_S
{
    HI_U32 u32SrcAddr;      /**< Դ���������ַ */
    HI_U32 u32DestAddr;     /**< Ŀ�����������ַ */
    HI_U32 u32ByteLength;   /**< �ӽ������ݳ��� */
} HI_UNF_CIPHER_DATA_S;

/**
 * @ingroup hct_cipher
 * cipher ������ӽ�����Ϣ
 */
typedef struct  hiCIPHER_PKG_S
{
    HI_HANDLE pCIHandle;                /**< ���*/
    HI_U32 u32PkgNum;                   /**< �����ܸ��� */
    HI_UNF_CIPHER_DATA_S* pu8Pkg;       /**< ÿ������������Ϣ */
}CIPHER_PKG_S;

/**
 * @ingroup hct_cipher
 * ��ϣ�㷨��ʼ���������
 */
typedef struct
{
    HI_U8 *pu8HMACKey;
    HI_U32 u32HMACKeyLen;
    HI_MDM_HASH_TYPE_E eShaType;
    HASH_HMAC_KEY_FROM_E enHMACKeyFrom;
} HI_MDM_HASH_ATTR_S;

/**
 * @ingroup hct_cipher
 * RSA���ݼӽ����㷨����
 */
typedef enum hiHI_UNF_CIPHER_RSA_ENC_SCHEME_E
{ 
    HI_UNF_RSA_ENC_SCHEME_NO_PADDING,            /**<  ����� */
    HI_UNF_RSA_ENC_SCHEME_BLOCK_TYPE_0,          /**< PKCS#1��block type 0��䷽ʽ*/
    HI_UNF_RSA_ENC_SCHEME_BLOCK_TYPE_1,          /**< PKCS#1��block type 1��䷽ʽ*/
    HI_UNF_RSA_ENC_SCHEME_BLOCK_TYPE_2,          /**< PKCS#1��block type 2��䷽ʽ*/
    HI_UNF_RSA_ENC_SCHEME_RSAES_OAEP_SHA1,       /**< PKCS#1��RSAES-OAEP-SHA1��䷽ʽ*/
    HI_UNF_RSA_ENC_SCHEME_RSAES_OAEP_SHA256,     /**< PKCS#1��RSAES-OAEP-SHA256��䷽ʽ*/
    HI_UNF_RSA_ENC_SCHEME_RSAES_PKCS1_V1_5,      /**< PKCS#1��PKCS1_V1_5��䷽ʽ*/
    HI_UNF_RSA_ENC_SCHEME_MAX,
}HI_UNF_RSA_ENC_SCHEME_E;

/**
 * @ingroup hct_cipher
 * RSA˽Կ�ṹ��
 */
typedef struct
{
    HI_U8 *pu8N;                      /**< ָ��RSA��ԿN��ָ��*/
    HI_U8 *pu8D;                      /**< ָ��RSA˽ԿD��ָ��*/
    HI_U16 u16NLen;                   /**< RSA��ԿN�ĳ���, ���Ϊ512Byte*/
    HI_U16 u16DLen;                   /**< RSA˽ԿD�ĳ���, ���Ϊ512Byte*/
}HI_UNF_RSA_PRI_KEY_S;

/**
 * @ingroup hct_cipher
 * RSA��Կ�ṹ��
 */
typedef struct
{
    HI_U8  *pu8N;                    /**< ָ��RSA��ԿN��ָ��*/
	HI_U8  *pu8E;			         /**< ָ��RSA��ԿE��ָ��*/
    HI_U16 u16NLen;                  /**< RSA��ԿN�ĳ���, ���Ϊ512Byte*/
    HI_U16 u16ELen;                  /**< RSA��ԿE�ĳ���, ���Ϊ512Byte*/
}HI_UNF_RSA_PUB_KEY_S;


/**
 * @ingroup hct_cipher
 * RSA ��Կ�ӽ����㷨�����ṹ��
 */
typedef struct
{
    HI_UNF_RSA_ENC_SCHEME_E enScheme;   /**< RSA���ݼӽ����㷨����*/
    HI_UNF_RSA_PUB_KEY_S stPubKey;      /**< RSA˽Կ�ṹ�� */
}HI_RSA_PUB_ENC_S;

/**
 * @ingroup hct_cipher
 * RSA ˽Կ�����㷨�����ṹ�� 
 */
typedef struct
{
    HI_UNF_RSA_ENC_SCHEME_E enScheme; /** < RSA���ݼӽ����㷨*/
    HI_UNF_RSA_PRI_KEY_S stPriKey;    /** < RSA��Կ�ṹ�� */
}HI_RSA_PRI_ENC_S;

/**
 * @ingroup hct_cipher
 * RSA �����㷨����
 */
typedef enum hiHI_UNF_CIPHER_RSA_SIGN_SCHEME_E
{ 
    HI_UNF_RSA_SIGN_SCHEME_RSASSA_PKCS1_V15_SHA1 = 0x100, /**< CNcomment: PKCS#1 RSASSA_PKCS1_V15_SHA1ǩ���㷨*/
    HI_UNF_RSA_SIGN_SCHEME_RSASSA_PKCS1_V15_SHA256,       /**< CNcomment: PKCS#1 RSASSA_PKCS1_V15_SHA256ǩ���㷨*/
    HI_UNF_RSA_SIGN_SCHEME_RSASSA_PKCS1_PSS_SHA1,         /**< CNcomment: PKCS#1 RSASSA_PKCS1_PSS_SHA1ǩ���㷨*/
    HI_UNF_RSA_SIGN_SCHEME_RSASSA_PKCS1_PSS_SHA256,       /**< CNcomment: PKCS#1 RSASSA_PKCS1_PSS_SHA256ǩ���㷨*/
    HI_UNF_RSA_SIGN_SCHEME_MAX,
}HI_UNF_RSA_SIGN_SCHEME_E;

#define EFUSE_AREA_SIZE (16)  /** <efuse ÿ�������С(�ֽ���)  */

/**
 * @ingroup hct_cipher
 * key������
 */
typedef enum
{
    EFUSE_AERA1 = 0,//512--(512+128)
    EFUSE_AERA2,
    EFUSE_AERA_MAX,
}HI_EFUSE_KEY_IDX_E;

/**
 * @ingroup hct_cipher
 * efuse key�����free����������
 */
typedef enum
{
    HI_EFUSE_FREE_RW_IDX_0, /**�ɶ���д��0  */
    HI_EFUSE_FREE_RW_IDX_1,
    HI_EFUSE_FREE_RW_IDX_2,
    HI_EFUSE_FREE_RW_IDX_3,
    HI_EFUSE_FREE_RW_IDX_4,
    HI_EFUSE_FREE_RW_IDX_5,
    HI_EFUSE_KEY_WO_IDX_0,  /** ��д���ɶ���0*/
    HI_EFUSE_KEY_WO_IDX_1,
    HI_EFUSE_IDX_MAX,
}HI_EFUSE_IDX_E;

/**
* @ingroup  hct_cipher
* @brief  hash ��ʼ����
*
* @par ����: 
* hash ��ʼ����
* @attention �ޡ�
*
* @param  pHandle [OUT] hash�����HI_U32����ָ�롣
* @param  pstHashAttr [IN] hash�㷨������
*
* @retval #HI_ERR_SUCCESS �ɹ���
* @retval ����ֵ ʧ�ܣ����hi_errno.h��
*
* @par Dependency:
* @li hi_mdm_cipher.h: �ýӿ��������ڵ�ͷ�ļ���
* @see  HI_MDM_HASH_CalcUpdate | HI_MDM_HASH_CalcFinal
* @since HiMDL_V200R001C00
*/
HI_EXTERN HI_U32 HI_MDM_HASH_CalcInit(HI_HANDLE pHandle, HI_MDM_HASH_ATTR_S *pstHashAttr);

/**
* @ingroup  hct_cipher
* @brief  ���������뵽HASHģ��
*
* @par ����: 
* ���������뵽HASHģ��(֧�ֶַμ���)��
* @attention hmacӲ��keyģʽ��֧�ֶַμ��㡣
*
* @param  pHandle [IN] hash�����
* @param  pu8InputData [IN] ��������Դ��ַ��
* @param  u32InputDataLen [IN] �������ݳ��ȡ�
*
* @retval #HI_ERR_SUCCESS �ɹ���
* @retval ����ֵ ʧ�ܣ����hi_errno.h��
*
* @par Dependency:
* @li hi_mdm_cipher.h: �ýӿ��������ڵ�ͷ�ļ���
* @see  HI_MDM_HASH_CalcInit | HI_MDM_HASH_CalcFinal��
* @since HiMDL_V200R001C00
*/
HI_EXTERN HI_U32 HI_MDM_HASH_CalcUpdate(HI_HANDLE hHandle, HI_U8 *pu8InputData, HI_U32 u32InputDataLen);

/**
* @ingroup  hct_cipher
* @brief  ��ȡ��������
*
* @par ����: 
* ��ȡ��������
* @attention �ޡ�
*
* @param  pHandle [IN] hash�����
* @param  pu8OutputHash [OUT] hash��������Ŀ�ĵ�ַ��
*
* @retval #HI_ERR_SUCCESS �ɹ���
* @retval ����ֵ ʧ�ܣ����hi_errno.h��
*
* @par Dependency:
* @li hi_mdm_cipher.h: �ýӿ��������ڵ�ͷ�ļ���
* @see  HI_MDM_HASH_CalcInit | HI_MDM_HASH_CalcUpdate��
* @since HiMDL_V200R001C00
*/
HI_EXTERN HI_U32 HI_MDM_HASH_CalcFinal(HI_HANDLE hHandle, HI_U8 *pu8OutputHash);

HI_EAPI HI_U32 HI_MDM_HASH_Sha256(HI_U8 *pu8InputData, HI_U32 u32InputDataLen, HI_U8 *pu8OutputHash);

/**
* @ingroup  hct_cipher
* @brief  cipher��ʼ����
*
* @par ����:  
* cipher��ʼ����
* @attention �ޡ�
* 
* @retval #HI_ERR_SUCCESS �ɹ���
* @retval ����ֵ ʧ�ܣ����hi_errno.h��
*
* @par Dependency:
* @li hi_mdm_cipher.h: �ýӿ��������ڵ�ͷ�ļ���
* @see  HI_MDM_CIPHER_DeInit
* @since HiMDL_V200R001C00
*/
HI_EXTERN HI_U32 HI_MDM_CIPHER_Init(HI_VOID);

/**
* @ingroup  hct_cipher
* @brief  cipherȥ��ʼ����
*
* @par ����:  
* cipherȥ��ʼ����
* @attention �ޡ�
* 
* @retval #HI_ERR_SUCCESS �ɹ���
* @retval ����ֵ ʧ�ܣ����hi_errno.h��
*
* @par Dependency:
* @li hi_mdm_cipher.h: �ýӿ��������ڵ�ͷ�ļ���
* @see  HI_MDM_CIPHER_Init
* @since HiMDL_V200R001C00
*/
HI_EXTERN HI_U32 HI_MDM_CIPHER_DeInit(HI_VOID);

/**
* @ingroup  hct_cipher
* @brief  ����cipher�����
*
* @par ����: 
* ����cipher�����
* @attention �ޡ�
*
* @param  pHandle [OUT] cipher�����
* @param  enCipherType [IN] ͨ��ѡ��
*
* @retval #HI_ERR_SUCCESS �ɹ���
* @retval ����ֵ ʧ�ܣ����hi_errno.h��
*
* @par Dependency:
* @li hi_mdm_cipher.h: �ýӿ��������ڵ�ͷ�ļ���
* @see  HI_MDM_CIPHER_DestroyHandle
* @since HiMDL_V200R001C00
*/
HI_EXTERN HI_U32 HI_MDM_CIPHER_CreateHandle(HI_HANDLE pHandle, HI_CIPHER_TYPE_E enCipherType);

/**
* @ingroup  hct_cipher
* @brief  AES�㷨���ܽ��ܲ������á�
*
* @par ����:   
* AES�㷨���ܽ��ܲ������á�
* @attention �ޡ�
*
* @param  pHandle [IN] cipher�����
* @param  pConfig [IN] ������Ϣ��
*
* @retval #HI_ERR_SUCCESS �ɹ���
* @retval ����ֵ ʧ�ܣ����hi_errno.h��
*
* @par Dependency:
* @li hi_mdm_cipher.h: �ýӿ��������ڵ�ͷ�ļ���
* @see  HI_MDM_CIPHER_Config3DES
* @since HiMDL_V200R001C00
*/
HI_EXTERN HI_U32 HI_MDM_CIPHER_ConfigAES(HI_HANDLE pHandle,  HI_CIPHER_CTRL_S *pConfig);

/**
* @ingroup  hct_cipher
* @brief  3DES�㷨���ܽ��ܲ������á�
*
* @par ����:   
* 3DES�㷨���ܽ��ܲ������á�
* @attention �ޡ�
*
* @param  pHandle [IN] cipher�����
* @param  pConfig [IN] ������Ϣ��
*
* @retval #HI_ERR_SUCCESS �ɹ���
* @retval ����ֵ ʧ�ܣ����hi_errno.h��
*
* @par Dependency:
* @li hi_mdm_cipher.h: �ýӿ��������ڵ�ͷ�ļ���
* @see  HI_MDM_CIPHER_ConfigAES
* @since HiMDL_V200R001C00
*/
HI_EXTERN HI_U32 HI_MDM_CIPHER_Config3DES(HI_HANDLE pHandle,  HI_CIPHER_CTRL_S *pConfig);

/**
* @ingroup  hct_cipher
* @brief  ��ȡ�������á�
*
* @par ����:  
* ��ȡ�������á�
* @attention �ޡ�
*
* @param  pHandle [IN] cipher�����
* @param  pstCipherConfig [IN] cipher�Ĳ������á�
*
* @retval #HI_ERR_SUCCESS �ɹ���
* @retval ����ֵ ʧ�ܣ����hi_errno.h��
*
* @par Dependency:
* @li hi_mdm_cipher.h: �ýӿ��������ڵ�ͷ�ļ���
* @see  �ޡ�
* @since HiMDL_V200R001C00
*/
HI_EXTERN HI_U32 HI_MDM_CIPHER_GetConfig(HI_HANDLE pHandle, HI_CIPHER_CTRL_S *pstCipherConfig);

/**
* @ingroup  hct_cipher
* @brief  �ͷž����
*
* @par ����:  
* �ͷž����
* @attention �ޡ�
*
* @param  pHandle [IN] cipher�����
*
* @retval #HI_ERR_SUCCESS �ɹ���
* @retval ����ֵ ʧ�ܣ����hi_errno.h��
*
* @par Dependency:
* @li hi_mdm_cipher.h: �ýӿ��������ڵ�ͷ�ļ���
* @see  HI_MDM_CIPHER_CreateHandle
* @since HiMDL_V200R001C00
*/
HI_EXTERN HI_U32 HI_MDM_CIPHER_DestroyHandle(HI_HANDLE pHandle);

/**
* @ingroup  hct_cipher
* @brief  ��ͨ����������ܡ�
*
* @par ����:   
* ��ͨ����������ܡ�
* @attention �ޡ�
*
* @param pstCIData [IN] cipher ������Ϣ��
*
* @retval #HI_ERR_SUCCESS �ɹ���
* @retval ����ֵ ʧ�ܣ����hi_errno.h��
*
* @par Dependency:
* @li hi_mdm_cipher.h: �ýӿ��������ڵ�ͷ�ļ���
* @see  HI_MDM_CIPHER_Decrypt
* @since HiMDL_V200R001C00
*/
HI_EXTERN HI_U32 HI_MDM_CIPHER_Encrypt(CIPHER_DATA_S *pstCIData);

/**
* @ingroup  hct_cipher
* @brief  ��ͨ����������ܡ�
*
* @par ����:   
* ��ͨ����������ܡ�
* @attention �ޡ�
*
* @param pstCIData [IN] cipher ������Ϣ��
*
* @retval #HI_ERR_SUCCESS �ɹ���
* @retval ����ֵ ʧ�ܣ����hi_errno.h��
*
* @par Dependency:
* @li hi_mdm_cipher.h: �ýӿ��������ڵ�ͷ�ļ���
* @see  HI_MDM_CIPHER_Encrypt
* @since HiMDL_V200R001C00
*/
HI_EXTERN HI_U32 HI_MDM_CIPHER_Decrypt(CIPHER_DATA_S *pstCIData);

/**
* @ingroup  hct_cipher
* @brief  ��ͨ����������ܡ�
*
* @par ����:   
* ��ͨ����������ܡ�
* @attention �ޡ�
*
* @param psPkg [IN] ������ӽ�����Ϣ��
*
* @retval #HI_ERR_SUCCESS �ɹ���
* @retval ����ֵ ʧ�ܣ����hi_errno.h��
*
* @par Dependency:
* @li hi_mdm_cipher.h: �ýӿ��������ڵ�ͷ�ļ���
* @see  HI_MDM_CIPHER_DecryptMulti
* @since HiMDL_V200R001C00
*/
HI_EXTERN HI_U32 HI_MDM_CIPHER_EncryptMulti(CIPHER_PKG_S *psPkg);

/**
* @ingroup  hct_cipher
* @brief  ��ͨ����������ܡ�
*
* @par ����:   
* ��ͨ����������ܡ�
* @attention �ޡ�
*
* @param psPkg [IN] ������ӽ�����Ϣ��
*
* @retval #HI_ERR_SUCCESS �ɹ���
* @retval ����ֵ ʧ�ܣ����hi_errno.h��
*
* @par Dependency:
* @li hi_mdm_cipher.h: �ýӿ��������ڵ�ͷ�ļ���
* @see  HI_MDM_CIPHER_EncryptMulti
* @since HiMDL_V200R001C00
*/
HI_EXTERN HI_U32 HI_MDM_CIPHER_DecryptMulti(CIPHER_PKG_S *psPkg);

/**
* @ingroup  hct_cipher
* @brief  ��efuse��free�������ݡ�
*
* @par ����:   
* ��efuse��free�������ݡ�
* @attention �ޡ�
*
* @param  enEfuseId [IN] efuse id�š�
* @param  aucData [OUT] ����Ŀ�ĵ�ַ��
*
* @retval #HI_ERR_SUCCESS �ɹ���
* @retval ����ֵ ʧ�ܣ����hi_errno.h��
*
* @par Dependency:
* @li hi_mdm_cipher.h: �ýӿ��������ڵ�ͷ�ļ���
* @see  HI_MDM_EFUSE_Write
* @since HiMDL_V200R001C00
*/
HI_EXTERN HI_U32 HI_MDM_EFUSE_Read(HI_EFUSE_IDX_E enEfuseId, HI_U8 aucData[EFUSE_AREA_SIZE]);

/**
* @ingroup  hct_cipher
* @brief  дefuse��free�����key�������ݡ�
*
* @par ����:   
* дefuse��free�����key�������ݡ�
* @attention �ޡ�
*
* @param  enEfuseId [IN] efuse id�š�
* @param  aucData [IN] ����Դ��ַ��
*
* @retval #HI_ERR_SUCCESS �ɹ���
* @retval ����ֵ ʧ�ܣ����hi_errno.h��
*
* @par Dependency:
* @li hi_mdm_cipher.h: �ýӿ��������ڵ�ͷ�ļ���
* @see  HI_MDM_EFUSE_Read
* @since HiMDL_V200R001C00
*/
HI_EXTERN HI_U32 HI_MDM_EFUSE_Write(HI_EFUSE_IDX_E enEfuseId, HI_U8 aucData[EFUSE_AREA_SIZE]);

/**
* @ingroup  hct_cipher
* @brief  RSA��ʼ����
*
* @par ����:   
* RSA��ʼ����
* @attention �ޡ�
*
* @retval #HI_ERR_SUCCESS �ɹ���
* @retval ����ֵ ʧ�ܣ����hi_errno.h��
*
* @par Dependency:
* @li hi_mdm_cipher.h: �ýӿ��������ڵ�ͷ�ļ���
* @see  HI_MDM_RSA_DeInit
* @since HiMDL_V200R001C00
*/
HI_EXTERN HI_U32 HI_MDM_RSA_Init(HI_VOID);

/**
* @ingroup  hct_cipher
* @brief  RSAȥ��ʼ����
*
* @par ����:   
* RSAȥ��ʼ����
* @attention �ޡ�
*
* @param ��
*
* @retval #HI_ERR_SUCCESS �ɹ���
* @retval ����ֵ ʧ�ܣ����hi_errno.h��
*
* @par Dependency:
* @li hi_mdm_cipher.h: �ýӿ��������ڵ�ͷ�ļ���
* @see  HI_MDM_RSA_Init
* @since HiMDL_V200R001C00
*/
HI_EXTERN HI_U32 HI_MDM_RSA_DeInit(HI_VOID);

/**
* @ingroup  hct_cipher
* @brief RSA��Կ���ܡ�
*
* @par ����:  
* RSA��Կ���ܡ�
* @attention �ޡ�
*
* @param  pstRsaEnc [IN] RSA��Կ���ܽ����㷨������
* @param  pInput [IN] ��������Դ��ַ��
* @param  ulInLen [IN] ��������Դ���ȡ�
* @param  pOutput [OUT] �������Ŀ�ĵ�ַ��
* @param  pOutLen [OUT] ���ܺ���������ݳ��ȡ�
*
* @retval #HI_ERR_SUCCESS �ɹ���
* @retval ����ֵ ʧ�ܣ����hi_errno.h��
*
* @par Dependency:
* @li hi_mdm_cipher.h: �ýӿ��������ڵ�ͷ�ļ���
* @see  HI_MDM_RSA_PrivateDecrypt
* @since HiMDL_V200R001C00
*/
HI_EXTERN HI_U32 HI_MDM_RSA_PublicEncrypt(HI_RSA_PUB_ENC_S *pstRsaEnc, 
                								  HI_U8 *pInput, HI_U32 ulInLen, 
                								  HI_U8 *pOutput, HI_U32 *pOutLen);

/**
* @ingroup  hct_cipher
* @brief RSA˽Կ���ܡ�
*
* @par ����:  
* RSA˽Կ���ܡ�
* @attention �ޡ�
*
* @param  pstRsaEnc [IN] RSA˽Կ���ܽ����㷨������
* @param  pInput [IN] ��������Դ��ַ��
* @param  ulInLen [IN] ��������Դ���ȡ�
* @param  pOutput [OUT] �������Ŀ�ĵ�ַ��
* @param  pOutLen [OUT] ������ݳ��ȡ�
*
* @retval #HI_ERR_SUCCESS �ɹ���
* @retval ����ֵ ʧ�ܣ����hi_errno.h��
*
* @par Dependency:
* @li hi_mdm_cipher.h: �ýӿ��������ڵ�ͷ�ļ���
* @see  HI_MDM_RSA_PublicEncrypt
* @since HiMDL_V200R001C00
*/
HI_EXTERN HI_U32 HI_MDM_RSA_PrivateDecrypt(HI_RSA_PRI_ENC_S *pstRsaDec,								  
                								   HI_U8 *pInput, HI_U32 ulInLen, 
                								   HI_U8 *pOutput, HI_U32 *pOutLen);

/**
* @ingroup  hct_cipher
* @brief RSA˽Կ���ܡ�
*
* @par ����:  
* RSA˽Կ���ܡ�
* @attention �ޡ�
*
* @param  pstRsaEnc [IN] RSA˽Կ���ܽ����㷨������
* @param  pInput [IN] ��������Դ��ַ��
* @param  ulInLen [IN] ��������Դ���ȡ�
* @param  pOutput [OUT] �������Ŀ�ĵ�ַ��
* @param  pOutLen [OUT] ������ݳ��ȡ�
*
* @retval #HI_ERR_SUCCESS �ɹ���
* @retval ����ֵ ʧ�ܣ����hi_errno.h��
*
* @par Dependency:
* @li hi_mdm_cipher.h: �ýӿ��������ڵ�ͷ�ļ���
* @see  HI_MDM_RSA_PublicDecrypt
* @since HiMDL_V200R001C00
*/
HI_EXTERN HI_U32 HI_MDM_RSA_PrivateEncrypt(HI_RSA_PRI_ENC_S *pstRsEnc, 
                							 	   HI_U8 *pInput, HI_U32 ulInLen, 
                								   HI_U8 *pOutput, HI_U32 *pOutLen);

/**
* @ingroup  hct_cipher
* @brief RSA��Կ���ܡ�
*
* @par ����:  
* RSA��Կ���ܡ�
* @attention �ޡ�
*
* @param  pstRsaEnc [IN] RSA˽Կ���ܽ����㷨������
* @param  pInput [IN] ��������Դ��ַ��
* @param  ulInLen [IN] ��������Դ���ȡ�
* @param  pOutput [OUT] �������Ŀ�ĵ�ַ��
* @param  pOutLen [OUT] ������ݳ��ȡ�
*
* @retval #HI_ERR_SUCCESS �ɹ���
* @retval ����ֵ ʧ�ܣ����hi_errno.h��
*
* @par Dependency:
* @li hi_mdm_cipher.h: �ýӿ��������ڵ�ͷ�ļ���
* @see  HI_MDM_RSA_PrivateEncrypt
* @since HiMDL_V200R001C00
*/
HI_EXTERN HI_U32 HI_MDM_RSA_PublicDecrypt(HI_RSA_PUB_ENC_S *pstRsaDec,
                    							   HI_U8 *pInput, HI_U32 ulInLen,
                    							   HI_U8 *pOutput, HI_U32 *pOutLen);

/**
* @ingroup  hct_randnum
* @brief ���������ģ������Դ����32λ�������
*
* @par ����:
* ���������ģ������Դ����32λ�������
* @attention �ޡ�
*
* @param ulMinVal [IN] �����Χ���ޡ�
* @param ulMaxVal [IN] �����Χ���ޡ�
* @param pulValue [OUT] ��������������̳��ִ�����pulValue�ǿ�ָ��ʱ����������ֵulMinVal��
*
* @retval #HI_ERR_SUCCESS �ɹ���
* @retval ����ֵ ʧ�ܣ����hi_errno.h��
*
* @par Dependency:
* @li hi_mdm_cipher.h: �ýӿ��������ڵ�ͷ�ļ���
* @see HI_DRV_RANDOM_GetNumber
* @since HiMDL_V200R001C00
*/
HI_EXTERN HI_U32 HI_MDM_GetRealRandom(HI_U32 ulMinVal,HI_U32 ulMaxVal,HI_OUT HI_U32 *pulValue);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif  // __cplusplus

#endif // __HI_MDM_CIPHER_H__

