/**@defgroup hct_cipher 加密解密
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
 * HASH算法类型
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
 * HASH HMAC模式下是否使用安全加密模式，即是否使用硬件key
 */
typedef enum
{
    HI_CIPHER_HMAC_KEY_FROM_CA,
    HI_CIPHER_HMAC_KEY_FROM_CPU,
    HI_CIPHER_HMAC_KEY_FROM_MAX,
}HASH_HMAC_KEY_FROM_E;

/**
 * @ingroup hct_cipher
 * cipher 加密位宽模式
 */
typedef enum hiHI_UNF_CIPHER_BIT_WIDTH_E
{
    HI_UNF_CIPHER_BIT_WIDTH_64BIT   = 0x0,  /**< 64bit位宽 */
    HI_UNF_CIPHER_BIT_WIDTH_8BIT    = 0x1,  /**< 8bit位宽 */
    HI_UNF_CIPHER_BIT_WIDTH_1BIT    = 0x2,  /**< 1bit位宽 */
    HI_UNF_CIPHER_BIT_WIDTH_128BIT  = 0x3,  /**< 128bit位宽 */
    HI_UNF_CIPHER_BIT_WIDTH_MAX
}HI_UNF_CIPHER_BIT_WIDTH_E;

/**
 * @ingroup hct_cipher
 * cipher 密钥长度
 */
typedef enum hiHI_UNF_CIPHER_KEY_LENGTH_E
{
    HI_UNF_CIPHER_KEY_AES_128BIT    = 0x0,  /**< AES运算方式下采用128bit密钥长度 */
    HI_UNF_CIPHER_KEY_AES_192BIT    = 0x1,  /**< AES运算方式下采用192bit密钥长度 */
    HI_UNF_CIPHER_KEY_AES_256BIT    = 0x2,  /**< AES运算方式下采用256bit密钥长度 */
    HI_UNF_CIPHER_KEY_DES_3KEY      = 0x2,  /**< DES运算方式下采用3个key */
    HI_UNF_CIPHER_KEY_DES_2KEY      = 0x3,  /**< DES运算方式下采用2个key  */
    HI_UNF_CIPHER_KEY_MAX,
}HI_UNF_CIPHER_KEY_LENGTH_E;

/**
 * @ingroup hct_cipher
 * cipher 加密算法
 */
typedef enum hiHI_UNF_CIPHER_ALG_E
{
    HI_UNF_CIPHER_ALG_DES           = 0x0,  /**< DES算法 */
    HI_UNF_CIPHER_ALG_3DES          = 0x1,  /**< 3DES算法 */
    HI_UNF_CIPHER_ALG_AES           = 0x2,  /**< AES算法 */
    HI_UNF_CIPHER_ALG_MAX           = 0x3
}HI_UNF_CIPHER_ALG_E;

/**
 * @ingroup hct_cipher
 * cipher 工作模式
 */
typedef enum hiHI_UNF_CIPHER_WORK_MODE_E
{
    HI_UNF_CIPHER_WORK_MODE_ECB,        /**< ECB模式 */
    HI_UNF_CIPHER_WORK_MODE_CBC,        /**< CBC模式 */
    HI_UNF_CIPHER_WORK_MODE_CFB,        /**< CFB模式 */
    HI_UNF_CIPHER_WORK_MODE_OFB,        /**< OFB模式 */
    HI_UNF_CIPHER_WORK_MODE_CTR,        /**< CTR模式 */
    HI_UNF_CIPHER_WORK_MODE_CBC_CTS,    /**< CBC-CTS模式 */
    HI_UNF_CIPHER_WORK_MODE_MAX
}HI_UNF_CIPHER_WORK_MODE_E;

/**
 * @ingroup hct_cipher
 * cipher 参数配置
 */
typedef struct
{
    HI_UNF_CIPHER_ALG_E enAlg;                      /**< 加密算法 */   
    HI_UNF_CIPHER_WORK_MODE_E enWorkMode;           /**< 工作模式 */                                                                                                /**< CNcomment:工作模式 */
    HI_UNF_CIPHER_BIT_WIDTH_E enBitWidth;           /**< 加密位宽模式*/                                                                        /**< CNcomment:加密或解密的位宽 */
    HI_UNF_CIPHER_KEY_LENGTH_E enKeyLen;            /**< 密钥长度 */                                                                                                    /**< CNcomment:密钥长度 */
} HI_CIPHER_MODE_CTRL_S;

/**
 * @ingroup hct_cipher
 * cipher 加密控制信息
 */
typedef struct hiHI_CIPHER_CTRL_S
{
    HI_U32 u32Key[8];                   /**< 秘钥*/                                                                                        
    HI_U32 u32IV[4];                    /**< 向量IV */                                                                        
    HI_BOOL bKeyByCA;                   /**< 是否使用高安全CA，即是否使用硬件Key */
    HI_BOOL bIVChangeFlag;              /**< 是否使用机密和解密向量。 */    
    HI_U8 ucRsv[2];                     /**< 保留位 */    
    HI_CIPHER_MODE_CTRL_S stModeCtrl;   /**< 模式参数 */   
} HI_CIPHER_CTRL_S;

/**
 * @ingroup hct_cipher
 * cipher 数据信息
 */
typedef struct  hiCIPHER_DATA_S
{
    HI_HANDLE pCIHandle;    /**< 句柄。 */     
    HI_U32 u32SrcAddr;      /**< 解密数据源地址。 */    
    HI_U32 u32DestAddr;     /**< 解密数据目的地址。 */    
    HI_U32 u32DataLength;   /**< 解密数据长度。 */    
}CIPHER_DATA_S;

/**
 * @ingroup hct_cipher
 * cipher 加解密通道类型选择
 */
typedef enum
{
    HI_UNF_CIPHER_TYPE_NORMAL = 0x0,    /**< 多通道 通道1-通道7*/  
    HI_UNF_CIPHER_TYPE_SINGLE_CHANNEL,  /**< 单通道 通道0*/ 
    HI_UNF_CIPHER_TYPE_MAX,
}HI_CIPHER_TYPE_E;

/**
 * @ingroup hct_cipher
 * cipher 加解密通道类型选择
 */
typedef struct
{
    HI_CIPHER_TYPE_E enCipherType;
}HI_UNF_CIPHER_ATTS_S;

/**
 * @ingroup hct_cipher
 * cipher 加解密加解密数据信息
 */
typedef struct hiHI_UNF_CIPHER_DATA_S
{
    HI_U32 u32SrcAddr;      /**< 源数据物理地址 */
    HI_U32 u32DestAddr;     /**< 目的数据物理地址 */
    HI_U32 u32ByteLength;   /**< 加解密数据长度 */
} HI_UNF_CIPHER_DATA_S;

/**
 * @ingroup hct_cipher
 * cipher 多链表加解密信息
 */
typedef struct  hiCIPHER_PKG_S
{
    HI_HANDLE pCIHandle;                /**< 句柄*/
    HI_U32 u32PkgNum;                   /**< 链表总个数 */
    HI_UNF_CIPHER_DATA_S* pu8Pkg;       /**< 每个链表数据信息 */
}CIPHER_PKG_S;

/**
 * @ingroup hct_cipher
 * 哈希算法初始化输入参数
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
 * RSA数据加解密算法策略
 */
typedef enum hiHI_UNF_CIPHER_RSA_ENC_SCHEME_E
{ 
    HI_UNF_RSA_ENC_SCHEME_NO_PADDING,            /**<  不填充 */
    HI_UNF_RSA_ENC_SCHEME_BLOCK_TYPE_0,          /**< PKCS#1的block type 0填充方式*/
    HI_UNF_RSA_ENC_SCHEME_BLOCK_TYPE_1,          /**< PKCS#1的block type 1填充方式*/
    HI_UNF_RSA_ENC_SCHEME_BLOCK_TYPE_2,          /**< PKCS#1的block type 2填充方式*/
    HI_UNF_RSA_ENC_SCHEME_RSAES_OAEP_SHA1,       /**< PKCS#1的RSAES-OAEP-SHA1填充方式*/
    HI_UNF_RSA_ENC_SCHEME_RSAES_OAEP_SHA256,     /**< PKCS#1的RSAES-OAEP-SHA256填充方式*/
    HI_UNF_RSA_ENC_SCHEME_RSAES_PKCS1_V1_5,      /**< PKCS#1的PKCS1_V1_5填充方式*/
    HI_UNF_RSA_ENC_SCHEME_MAX,
}HI_UNF_RSA_ENC_SCHEME_E;

/**
 * @ingroup hct_cipher
 * RSA私钥结构体
 */
typedef struct
{
    HI_U8 *pu8N;                      /**< 指向RSA公钥N的指针*/
    HI_U8 *pu8D;                      /**< 指向RSA私钥D的指针*/
    HI_U16 u16NLen;                   /**< RSA公钥N的长度, 最大为512Byte*/
    HI_U16 u16DLen;                   /**< RSA私钥D的长度, 最大为512Byte*/
}HI_UNF_RSA_PRI_KEY_S;

/**
 * @ingroup hct_cipher
 * RSA公钥结构体
 */
typedef struct
{
    HI_U8  *pu8N;                    /**< 指向RSA公钥N的指针*/
	HI_U8  *pu8E;			         /**< 指向RSA公钥E的指针*/
    HI_U16 u16NLen;                  /**< RSA公钥N的长度, 最大为512Byte*/
    HI_U16 u16ELen;                  /**< RSA公钥E的长度, 最大为512Byte*/
}HI_UNF_RSA_PUB_KEY_S;


/**
 * @ingroup hct_cipher
 * RSA 公钥加解密算法参数结构体
 */
typedef struct
{
    HI_UNF_RSA_ENC_SCHEME_E enScheme;   /**< RSA数据加解密算法策略*/
    HI_UNF_RSA_PUB_KEY_S stPubKey;      /**< RSA私钥结构体 */
}HI_RSA_PUB_ENC_S;

/**
 * @ingroup hct_cipher
 * RSA 私钥解密算法参数结构体 
 */
typedef struct
{
    HI_UNF_RSA_ENC_SCHEME_E enScheme; /** < RSA数据加解密算法*/
    HI_UNF_RSA_PRI_KEY_S stPriKey;    /** < RSA公钥结构体 */
}HI_RSA_PRI_ENC_S;

/**
 * @ingroup hct_cipher
 * RSA 加密算法类型
 */
typedef enum hiHI_UNF_CIPHER_RSA_SIGN_SCHEME_E
{ 
    HI_UNF_RSA_SIGN_SCHEME_RSASSA_PKCS1_V15_SHA1 = 0x100, /**< CNcomment: PKCS#1 RSASSA_PKCS1_V15_SHA1签名算法*/
    HI_UNF_RSA_SIGN_SCHEME_RSASSA_PKCS1_V15_SHA256,       /**< CNcomment: PKCS#1 RSASSA_PKCS1_V15_SHA256签名算法*/
    HI_UNF_RSA_SIGN_SCHEME_RSASSA_PKCS1_PSS_SHA1,         /**< CNcomment: PKCS#1 RSASSA_PKCS1_PSS_SHA1签名算法*/
    HI_UNF_RSA_SIGN_SCHEME_RSASSA_PKCS1_PSS_SHA256,       /**< CNcomment: PKCS#1 RSASSA_PKCS1_PSS_SHA256签名算法*/
    HI_UNF_RSA_SIGN_SCHEME_MAX,
}HI_UNF_RSA_SIGN_SCHEME_E;

#define EFUSE_AREA_SIZE (16)  /** <efuse 每个区域大小(字节数)  */

/**
 * @ingroup hct_cipher
 * key区域编号
 */
typedef enum
{
    EFUSE_AERA1 = 0,//512--(512+128)
    EFUSE_AERA2,
    EFUSE_AERA_MAX,
}HI_EFUSE_KEY_IDX_E;

/**
 * @ingroup hct_cipher
 * efuse key区域和free区域整体编号
 */
typedef enum
{
    HI_EFUSE_FREE_RW_IDX_0, /**可读可写区0  */
    HI_EFUSE_FREE_RW_IDX_1,
    HI_EFUSE_FREE_RW_IDX_2,
    HI_EFUSE_FREE_RW_IDX_3,
    HI_EFUSE_FREE_RW_IDX_4,
    HI_EFUSE_FREE_RW_IDX_5,
    HI_EFUSE_KEY_WO_IDX_0,  /** 可写不可读区0*/
    HI_EFUSE_KEY_WO_IDX_1,
    HI_EFUSE_IDX_MAX,
}HI_EFUSE_IDX_E;

/**
* @ingroup  hct_cipher
* @brief  hash 初始化。
*
* @par 描述: 
* hash 初始化。
* @attention 无。
*
* @param  pHandle [OUT] hash句柄，HI_U32类型指针。
* @param  pstHashAttr [IN] hash算法参数。
*
* @retval #HI_ERR_SUCCESS 成功。
* @retval 其他值 失败，详见hi_errno.h。
*
* @par Dependency:
* @li hi_mdm_cipher.h: 该接口声明所在的头文件。
* @see  HI_MDM_HASH_CalcUpdate | HI_MDM_HASH_CalcFinal
* @since HiMDL_V200R001C00
*/
HI_EXTERN HI_U32 HI_MDM_HASH_CalcInit(HI_HANDLE pHandle, HI_MDM_HASH_ATTR_S *pstHashAttr);

/**
* @ingroup  hct_cipher
* @brief  将数据输入到HASH模块
*
* @par 描述: 
* 将数据输入到HASH模块(支持分段加密)。
* @attention hmac硬件key模式不支持分段计算。
*
* @param  pHandle [IN] hash句柄。
* @param  pu8InputData [IN] 输入数据源地址。
* @param  u32InputDataLen [IN] 输入数据长度。
*
* @retval #HI_ERR_SUCCESS 成功。
* @retval 其他值 失败，详见hi_errno.h。
*
* @par Dependency:
* @li hi_mdm_cipher.h: 该接口声明所在的头文件。
* @see  HI_MDM_HASH_CalcInit | HI_MDM_HASH_CalcFinal。
* @since HiMDL_V200R001C00
*/
HI_EXTERN HI_U32 HI_MDM_HASH_CalcUpdate(HI_HANDLE hHandle, HI_U8 *pu8InputData, HI_U32 u32InputDataLen);

/**
* @ingroup  hct_cipher
* @brief  获取计算结果。
*
* @par 描述: 
* 获取计算结果。
* @attention 无。
*
* @param  pHandle [IN] hash句柄。
* @param  pu8OutputHash [OUT] hash加密数据目的地址。
*
* @retval #HI_ERR_SUCCESS 成功。
* @retval 其他值 失败，详见hi_errno.h。
*
* @par Dependency:
* @li hi_mdm_cipher.h: 该接口声明所在的头文件。
* @see  HI_MDM_HASH_CalcInit | HI_MDM_HASH_CalcUpdate。
* @since HiMDL_V200R001C00
*/
HI_EXTERN HI_U32 HI_MDM_HASH_CalcFinal(HI_HANDLE hHandle, HI_U8 *pu8OutputHash);

HI_EAPI HI_U32 HI_MDM_HASH_Sha256(HI_U8 *pu8InputData, HI_U32 u32InputDataLen, HI_U8 *pu8OutputHash);

/**
* @ingroup  hct_cipher
* @brief  cipher初始化。
*
* @par 描述:  
* cipher初始化。
* @attention 无。
* 
* @retval #HI_ERR_SUCCESS 成功。
* @retval 其他值 失败，详见hi_errno.h。
*
* @par Dependency:
* @li hi_mdm_cipher.h: 该接口声明所在的头文件。
* @see  HI_MDM_CIPHER_DeInit
* @since HiMDL_V200R001C00
*/
HI_EXTERN HI_U32 HI_MDM_CIPHER_Init(HI_VOID);

/**
* @ingroup  hct_cipher
* @brief  cipher去初始化。
*
* @par 描述:  
* cipher去初始化。
* @attention 无。
* 
* @retval #HI_ERR_SUCCESS 成功。
* @retval 其他值 失败，详见hi_errno.h。
*
* @par Dependency:
* @li hi_mdm_cipher.h: 该接口声明所在的头文件。
* @see  HI_MDM_CIPHER_Init
* @since HiMDL_V200R001C00
*/
HI_EXTERN HI_U32 HI_MDM_CIPHER_DeInit(HI_VOID);

/**
* @ingroup  hct_cipher
* @brief  创建cipher句柄。
*
* @par 描述: 
* 创建cipher句柄。
* @attention 无。
*
* @param  pHandle [OUT] cipher句柄。
* @param  enCipherType [IN] 通道选择。
*
* @retval #HI_ERR_SUCCESS 成功。
* @retval 其他值 失败，详见hi_errno.h。
*
* @par Dependency:
* @li hi_mdm_cipher.h: 该接口声明所在的头文件。
* @see  HI_MDM_CIPHER_DestroyHandle
* @since HiMDL_V200R001C00
*/
HI_EXTERN HI_U32 HI_MDM_CIPHER_CreateHandle(HI_HANDLE pHandle, HI_CIPHER_TYPE_E enCipherType);

/**
* @ingroup  hct_cipher
* @brief  AES算法加密解密参数配置。
*
* @par 描述:   
* AES算法加密解密参数配置。
* @attention 无。
*
* @param  pHandle [IN] cipher句柄。
* @param  pConfig [IN] 配置信息。
*
* @retval #HI_ERR_SUCCESS 成功。
* @retval 其他值 失败，详见hi_errno.h。
*
* @par Dependency:
* @li hi_mdm_cipher.h: 该接口声明所在的头文件。
* @see  HI_MDM_CIPHER_Config3DES
* @since HiMDL_V200R001C00
*/
HI_EXTERN HI_U32 HI_MDM_CIPHER_ConfigAES(HI_HANDLE pHandle,  HI_CIPHER_CTRL_S *pConfig);

/**
* @ingroup  hct_cipher
* @brief  3DES算法加密解密参数配置。
*
* @par 描述:   
* 3DES算法加密解密参数配置。
* @attention 无。
*
* @param  pHandle [IN] cipher句柄。
* @param  pConfig [IN] 配置信息。
*
* @retval #HI_ERR_SUCCESS 成功。
* @retval 其他值 失败，详见hi_errno.h。
*
* @par Dependency:
* @li hi_mdm_cipher.h: 该接口声明所在的头文件。
* @see  HI_MDM_CIPHER_ConfigAES
* @since HiMDL_V200R001C00
*/
HI_EXTERN HI_U32 HI_MDM_CIPHER_Config3DES(HI_HANDLE pHandle,  HI_CIPHER_CTRL_S *pConfig);

/**
* @ingroup  hct_cipher
* @brief  获取参数配置。
*
* @par 描述:  
* 获取参数配置。
* @attention 无。
*
* @param  pHandle [IN] cipher句柄。
* @param  pstCipherConfig [IN] cipher的参数配置。
*
* @retval #HI_ERR_SUCCESS 成功。
* @retval 其他值 失败，详见hi_errno.h。
*
* @par Dependency:
* @li hi_mdm_cipher.h: 该接口声明所在的头文件。
* @see  无。
* @since HiMDL_V200R001C00
*/
HI_EXTERN HI_U32 HI_MDM_CIPHER_GetConfig(HI_HANDLE pHandle, HI_CIPHER_CTRL_S *pstCipherConfig);

/**
* @ingroup  hct_cipher
* @brief  释放句柄。
*
* @par 描述:  
* 释放句柄。
* @attention 无。
*
* @param  pHandle [IN] cipher句柄。
*
* @retval #HI_ERR_SUCCESS 成功。
* @retval 其他值 失败，详见hi_errno.h。
*
* @par Dependency:
* @li hi_mdm_cipher.h: 该接口声明所在的头文件。
* @see  HI_MDM_CIPHER_CreateHandle
* @since HiMDL_V200R001C00
*/
HI_EXTERN HI_U32 HI_MDM_CIPHER_DestroyHandle(HI_HANDLE pHandle);

/**
* @ingroup  hct_cipher
* @brief  单通道单链表加密。
*
* @par 描述:   
* 单通道单链表加密。
* @attention 无。
*
* @param pstCIData [IN] cipher 数据信息。
*
* @retval #HI_ERR_SUCCESS 成功。
* @retval 其他值 失败，详见hi_errno.h。
*
* @par Dependency:
* @li hi_mdm_cipher.h: 该接口声明所在的头文件。
* @see  HI_MDM_CIPHER_Decrypt
* @since HiMDL_V200R001C00
*/
HI_EXTERN HI_U32 HI_MDM_CIPHER_Encrypt(CIPHER_DATA_S *pstCIData);

/**
* @ingroup  hct_cipher
* @brief  单通道单链表解密。
*
* @par 描述:   
* 单通道单链表解密。
* @attention 无。
*
* @param pstCIData [IN] cipher 数据信息。
*
* @retval #HI_ERR_SUCCESS 成功。
* @retval 其他值 失败，详见hi_errno.h。
*
* @par Dependency:
* @li hi_mdm_cipher.h: 该接口声明所在的头文件。
* @see  HI_MDM_CIPHER_Encrypt
* @since HiMDL_V200R001C00
*/
HI_EXTERN HI_U32 HI_MDM_CIPHER_Decrypt(CIPHER_DATA_S *pstCIData);

/**
* @ingroup  hct_cipher
* @brief  多通道多链表加密。
*
* @par 描述:   
* 多通道多链表加密。
* @attention 无。
*
* @param psPkg [IN] 多链表加解密信息。
*
* @retval #HI_ERR_SUCCESS 成功。
* @retval 其他值 失败，详见hi_errno.h。
*
* @par Dependency:
* @li hi_mdm_cipher.h: 该接口声明所在的头文件。
* @see  HI_MDM_CIPHER_DecryptMulti
* @since HiMDL_V200R001C00
*/
HI_EXTERN HI_U32 HI_MDM_CIPHER_EncryptMulti(CIPHER_PKG_S *psPkg);

/**
* @ingroup  hct_cipher
* @brief  多通道多链表解密。
*
* @par 描述:   
* 多通道多链表解密。
* @attention 无。
*
* @param psPkg [IN] 多链表加解密信息。
*
* @retval #HI_ERR_SUCCESS 成功。
* @retval 其他值 失败，详见hi_errno.h。
*
* @par Dependency:
* @li hi_mdm_cipher.h: 该接口声明所在的头文件。
* @see  HI_MDM_CIPHER_EncryptMulti
* @since HiMDL_V200R001C00
*/
HI_EXTERN HI_U32 HI_MDM_CIPHER_DecryptMulti(CIPHER_PKG_S *psPkg);

/**
* @ingroup  hct_cipher
* @brief  读efuse中free区域数据。
*
* @par 描述:   
* 读efuse中free区域数据。
* @attention 无。
*
* @param  enEfuseId [IN] efuse id号。
* @param  aucData [OUT] 数据目的地址。
*
* @retval #HI_ERR_SUCCESS 成功。
* @retval 其他值 失败，详见hi_errno.h。
*
* @par Dependency:
* @li hi_mdm_cipher.h: 该接口声明所在的头文件。
* @see  HI_MDM_EFUSE_Write
* @since HiMDL_V200R001C00
*/
HI_EXTERN HI_U32 HI_MDM_EFUSE_Read(HI_EFUSE_IDX_E enEfuseId, HI_U8 aucData[EFUSE_AREA_SIZE]);

/**
* @ingroup  hct_cipher
* @brief  写efuse中free区域和key区域数据。
*
* @par 描述:   
* 写efuse中free区域和key区域数据。
* @attention 无。
*
* @param  enEfuseId [IN] efuse id号。
* @param  aucData [IN] 数据源地址。
*
* @retval #HI_ERR_SUCCESS 成功。
* @retval 其他值 失败，详见hi_errno.h。
*
* @par Dependency:
* @li hi_mdm_cipher.h: 该接口声明所在的头文件。
* @see  HI_MDM_EFUSE_Read
* @since HiMDL_V200R001C00
*/
HI_EXTERN HI_U32 HI_MDM_EFUSE_Write(HI_EFUSE_IDX_E enEfuseId, HI_U8 aucData[EFUSE_AREA_SIZE]);

/**
* @ingroup  hct_cipher
* @brief  RSA初始化。
*
* @par 描述:   
* RSA初始化。
* @attention 无。
*
* @retval #HI_ERR_SUCCESS 成功。
* @retval 其他值 失败，详见hi_errno.h。
*
* @par Dependency:
* @li hi_mdm_cipher.h: 该接口声明所在的头文件。
* @see  HI_MDM_RSA_DeInit
* @since HiMDL_V200R001C00
*/
HI_EXTERN HI_U32 HI_MDM_RSA_Init(HI_VOID);

/**
* @ingroup  hct_cipher
* @brief  RSA去初始化。
*
* @par 描述:   
* RSA去初始化。
* @attention 无。
*
* @param 无
*
* @retval #HI_ERR_SUCCESS 成功。
* @retval 其他值 失败，详见hi_errno.h。
*
* @par Dependency:
* @li hi_mdm_cipher.h: 该接口声明所在的头文件。
* @see  HI_MDM_RSA_Init
* @since HiMDL_V200R001C00
*/
HI_EXTERN HI_U32 HI_MDM_RSA_DeInit(HI_VOID);

/**
* @ingroup  hct_cipher
* @brief RSA公钥加密。
*
* @par 描述:  
* RSA公钥加密。
* @attention 无。
*
* @param  pstRsaEnc [IN] RSA公钥加密解密算法参数。
* @param  pInput [IN] 输入数据源地址。
* @param  ulInLen [IN] 输入数据源长度。
* @param  pOutput [OUT] 输出数据目的地址。
* @param  pOutLen [OUT] 加密后输出的数据长度。
*
* @retval #HI_ERR_SUCCESS 成功。
* @retval 其他值 失败，详见hi_errno.h。
*
* @par Dependency:
* @li hi_mdm_cipher.h: 该接口声明所在的头文件。
* @see  HI_MDM_RSA_PrivateDecrypt
* @since HiMDL_V200R001C00
*/
HI_EXTERN HI_U32 HI_MDM_RSA_PublicEncrypt(HI_RSA_PUB_ENC_S *pstRsaEnc, 
                								  HI_U8 *pInput, HI_U32 ulInLen, 
                								  HI_U8 *pOutput, HI_U32 *pOutLen);

/**
* @ingroup  hct_cipher
* @brief RSA私钥解密。
*
* @par 描述:  
* RSA私钥解密。
* @attention 无。
*
* @param  pstRsaEnc [IN] RSA私钥加密解密算法参数。
* @param  pInput [IN] 输入数据源地址。
* @param  ulInLen [IN] 输入数据源长度。
* @param  pOutput [OUT] 输出数据目的地址。
* @param  pOutLen [OUT] 输出数据长度。
*
* @retval #HI_ERR_SUCCESS 成功。
* @retval 其他值 失败，详见hi_errno.h。
*
* @par Dependency:
* @li hi_mdm_cipher.h: 该接口声明所在的头文件。
* @see  HI_MDM_RSA_PublicEncrypt
* @since HiMDL_V200R001C00
*/
HI_EXTERN HI_U32 HI_MDM_RSA_PrivateDecrypt(HI_RSA_PRI_ENC_S *pstRsaDec,								  
                								   HI_U8 *pInput, HI_U32 ulInLen, 
                								   HI_U8 *pOutput, HI_U32 *pOutLen);

/**
* @ingroup  hct_cipher
* @brief RSA私钥加密。
*
* @par 描述:  
* RSA私钥加密。
* @attention 无。
*
* @param  pstRsaEnc [IN] RSA私钥加密解密算法参数。
* @param  pInput [IN] 输入数据源地址。
* @param  ulInLen [IN] 输入数据源长度。
* @param  pOutput [OUT] 输出数据目的地址。
* @param  pOutLen [OUT] 输出数据长度。
*
* @retval #HI_ERR_SUCCESS 成功。
* @retval 其他值 失败，详见hi_errno.h。
*
* @par Dependency:
* @li hi_mdm_cipher.h: 该接口声明所在的头文件。
* @see  HI_MDM_RSA_PublicDecrypt
* @since HiMDL_V200R001C00
*/
HI_EXTERN HI_U32 HI_MDM_RSA_PrivateEncrypt(HI_RSA_PRI_ENC_S *pstRsEnc, 
                							 	   HI_U8 *pInput, HI_U32 ulInLen, 
                								   HI_U8 *pOutput, HI_U32 *pOutLen);

/**
* @ingroup  hct_cipher
* @brief RSA公钥解密。
*
* @par 描述:  
* RSA公钥解密。
* @attention 无。
*
* @param  pstRsaEnc [IN] RSA私钥加密解密算法参数。
* @param  pInput [IN] 输入数据源地址。
* @param  ulInLen [IN] 输入数据源长度。
* @param  pOutput [OUT] 输出数据目的地址。
* @param  pOutLen [OUT] 输出数据长度。
*
* @retval #HI_ERR_SUCCESS 成功。
* @retval 其他值 失败，详见hi_errno.h。
*
* @par Dependency:
* @li hi_mdm_cipher.h: 该接口声明所在的头文件。
* @see  HI_MDM_RSA_PrivateEncrypt
* @since HiMDL_V200R001C00
*/
HI_EXTERN HI_U32 HI_MDM_RSA_PublicDecrypt(HI_RSA_PUB_ENC_S *pstRsaDec,
                    							   HI_U8 *pInput, HI_U32 ulInLen,
                    							   HI_U8 *pOutput, HI_U32 *pOutLen);

/**
* @ingroup  hct_randnum
* @brief 根据随机数模块的随机源生成32位随机数。
*
* @par 描述:
* 根据随机数模块的随机源生成32位随机数。
* @attention 无。
*
* @param ulMinVal [IN] 随机范围下限。
* @param ulMaxVal [IN] 随机范围上限。
* @param pulValue [OUT] 随机结果。随机过程出现错误且pulValue非空指针时，随机结果赋值ulMinVal。
*
* @retval #HI_ERR_SUCCESS 成功。
* @retval 其他值 失败，详见hi_errno.h。
*
* @par Dependency:
* @li hi_mdm_cipher.h: 该接口声明所在的头文件。
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

