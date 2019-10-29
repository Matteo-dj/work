/******************************************************************************

  Copyright (C), 2011-2014, Hisilicon Tech. Co., Ltd.

 ******************************************************************************
  File Name     :hi_drv_cipher.h
  Version       : Initial Draft
  Author        : Hisilicon hisecurity team
  Created       : 
  Last Modified :
  Description   : 
  Function List :
  History       :
******************************************************************************/
#ifndef __HI_DRV_CIPHER_H__
#define __HI_DRV_CIPHER_H__

#include "hi_types.h"
#include "hal_cipher.h"
#ifdef __cplusplus
extern "C" {
#endif	/* __cplusplus */

#define CIPHER_SOFT_CHAN_NUM                   CIPHER_CHAN_NUM
#define CIPHER_INVALID_CHN                     (0xffffffff)
#define CIPHER_MAX_NODE_BUF_SIZE               (0xFFFF0)   //1M-16
#define CIPHER_MAX_RSA_KEY_LEN                 (512)

#define  HDCP_KEY_RAM_SIZE                     (320)
#define  HDCP_KEY_PRIME_SIZE                   (320)
#define  HDCP_KEY_TOOL_FILE_SIZE               (384)
#define  HDCP_KEY_CHIP_FILE_SIZE               (HDCP_KEY_RAM_SIZE + 12)

typedef struct hiCIPHER_TASK_S
{
    HI_DRV_CIPHER_DATA_INFO_S stData2Process;
    HI_U32 u32CallBackArg;
}HI_DRV_CIPHER_TASK_S;

typedef struct  hiCIPHER_HANDLE_S
{
    HI_HANDLE hCIHandle;
    HI_UNF_CIPHER_ATTS_S stCipherAtts;
}CIPHER_HANDLE_S;





typedef struct hiCIPHER_Config_CTRL
{
    HI_HANDLE pCIHandle;
    HI_CIPHER_CTRL_S CIpstCtrl;
}CIPHER_Config_CTRL;

typedef struct  hiCIPHER_CBCMAC_DATA_S
{
    HI_U8  u8RefCbcMac[16];
    HI_U32 u32AppLen;
}CIPHER_CBCMAC_DATA_S;

typedef struct
{
    HI_U32 u32TimeOutUs;
	HI_U32 u32RNG;
}CIPHER_RNG_S;

typedef struct
{
    HI_U8  *pu8Input;
    HI_U8  *pu8Output;
    HI_U32 u32DataLen;
    HI_U8  *pu8N;
	HI_U8  *pu8K;
    HI_U16 u16NLen;
    HI_U16 u16KLen;
}RSA_DATA_S;

#ifdef CONFIG_COMPAT 
typedef struct  hiCIPHER_COMPAT_PKG_S
{
    HI_HANDLE pCIHandle;
    HI_U32 u32PkgNum;
    HI_U32 u32PkgVia;
}CIPHER_COMPAT_PKG_S;

typedef struct hiCIPHER_COMPAT_RSA_DATA_S
{
    HI_U32 u32InputVia;
    HI_U32 u328OutputVia;
    HI_U32 u32DataLen;
    HI_U32 u328NVia;
	HI_U32 u328KVia;
    HI_U16 u16NLen;
    HI_U16 u16KLen;
}CIPHER_COMPAT_RSA_DATA_S;
#endif

//*****************************************************************************
// 函数名称: HI_DRV_CIPHER_Init
// 功能描述: CIPHER初始化
// 参数说明: 无
//
// 返 回 值: 
//           成功: HI_ERR_SUCCESS
//           失败: 其他
// 日    期: [2016-09-23]
//*****************************************************************************
HI_U32 HI_DRV_CIPHER_Init(HI_VOID);

//*****************************************************************************
// 函数名称: HI_DRV_CIPHER_CreateHandle
// 功能描述: 创建Handle，获取CIPHER通道ID
// 参数说明: 
//   enCipherType    [IN] 通道类型
//                   HI_UNF_CIPHER_TYPE_NORMAL = 0x0, // 多通道 (通道1-通道7)
//                   HI_UNF_CIPHER_TYPE_SINGLE_CHANNEL = 1, // 单通道 (通道0)
//   pHandle         [OUT]句柄
// 返 回 值: 
//           成功: HI_ERR_SUCCESS
//           失败: 其他
// 日    期: [2016-09-23]
//*****************************************************************************
HI_U32 HI_DRV_CIPHER_CreateHandle(HI_HANDLE pHandle, HI_CIPHER_TYPE_E enCipherType);

//*****************************************************************************
// 函数名称: HI_DRV_CIPHER_ConfigChn
// 功能描述: 当前通道的参数配置
// 参数说明: 
//   pHandle         [IN] 句柄
//   pConfig         [IN] CIPHER参数，具体如下
//                   u32Key[8]  加密或解密密钥
//                   u32IV[4]   加密或解密向量
//                   bKeyByCA   高安全加解密(使用硬件key)
//                   bIVChangeFlag 加密或解密向量更新至芯片寄存器
//   
//                   enAlg      加密或解密算法(参数组合场景见下表)                                                                                              /**< CNcomment:加密算法 */
//                   enBitWidth 加密或解密的位宽(参数组合场景见下表) 
//                   enWorkMode 加密或解密工作模式(参数组合场景见下表) 
//                   enKeyLen  密钥长度(参数组合场景见下表) 
// ---------------------------------------------------------------------------------------------------------------
// // AES
// // 由芯片密钥管理模块配置密钥(bKeyByCA = 1)时密钥长度只支持128位
// {HI_UNF_CIPHER_ALG_AES,HI_UNF_CIPHER_WORK_MODE_ECB,HI_UNF_CIPHER_BIT_WIDTH_128BIT,HI_UNF_CIPHER_KEY_AES_128BIT},
// {HI_UNF_CIPHER_ALG_AES,HI_UNF_CIPHER_WORK_MODE_CBC,HI_UNF_CIPHER_BIT_WIDTH_128BIT,HI_UNF_CIPHER_KEY_AES_128BIT},
// {HI_UNF_CIPHER_ALG_AES,HI_UNF_CIPHER_WORK_MODE_CFB,HI_UNF_CIPHER_BIT_WIDTH_1BIT,  HI_UNF_CIPHER_KEY_AES_128BIT},
// {HI_UNF_CIPHER_ALG_AES,HI_UNF_CIPHER_WORK_MODE_CFB,HI_UNF_CIPHER_BIT_WIDTH_8BIT,  HI_UNF_CIPHER_KEY_AES_128BIT},
// {HI_UNF_CIPHER_ALG_AES,HI_UNF_CIPHER_WORK_MODE_CFB,HI_UNF_CIPHER_BIT_WIDTH_128BIT,HI_UNF_CIPHER_KEY_AES_128BIT},
// {HI_UNF_CIPHER_ALG_AES,HI_UNF_CIPHER_WORK_MODE_OFB,HI_UNF_CIPHER_BIT_WIDTH_128BIT,HI_UNF_CIPHER_KEY_AES_128BIT},
//    
// {HI_UNF_CIPHER_ALG_AES,HI_UNF_CIPHER_WORK_MODE_ECB,HI_UNF_CIPHER_BIT_WIDTH_128BIT,HI_UNF_CIPHER_KEY_AES_192BIT},
// {HI_UNF_CIPHER_ALG_AES,HI_UNF_CIPHER_WORK_MODE_CBC,HI_UNF_CIPHER_BIT_WIDTH_128BIT,HI_UNF_CIPHER_KEY_AES_192BIT},
// {HI_UNF_CIPHER_ALG_AES,HI_UNF_CIPHER_WORK_MODE_CFB,HI_UNF_CIPHER_BIT_WIDTH_1BIT,  HI_UNF_CIPHER_KEY_AES_192BIT},
// {HI_UNF_CIPHER_ALG_AES,HI_UNF_CIPHER_WORK_MODE_CFB,HI_UNF_CIPHER_BIT_WIDTH_8BIT,  HI_UNF_CIPHER_KEY_AES_192BIT},
// {HI_UNF_CIPHER_ALG_AES,HI_UNF_CIPHER_WORK_MODE_CFB,HI_UNF_CIPHER_BIT_WIDTH_128BIT,HI_UNF_CIPHER_KEY_AES_192BIT},
// {HI_UNF_CIPHER_ALG_AES,HI_UNF_CIPHER_WORK_MODE_OFB,HI_UNF_CIPHER_BIT_WIDTH_128BIT,HI_UNF_CIPHER_KEY_AES_192BIT},
// 
// {HI_UNF_CIPHER_ALG_AES,HI_UNF_CIPHER_WORK_MODE_ECB,HI_UNF_CIPHER_BIT_WIDTH_128BIT,HI_UNF_CIPHER_KEY_AES_256BIT},
// {HI_UNF_CIPHER_ALG_AES,HI_UNF_CIPHER_WORK_MODE_CBC,HI_UNF_CIPHER_BIT_WIDTH_128BIT,HI_UNF_CIPHER_KEY_AES_256BIT},
// {HI_UNF_CIPHER_ALG_AES,HI_UNF_CIPHER_WORK_MODE_CFB,HI_UNF_CIPHER_BIT_WIDTH_1BIT,  HI_UNF_CIPHER_KEY_AES_256BIT},
// {HI_UNF_CIPHER_ALG_AES,HI_UNF_CIPHER_WORK_MODE_CFB,HI_UNF_CIPHER_BIT_WIDTH_8BIT,  HI_UNF_CIPHER_KEY_AES_256BIT},
// {HI_UNF_CIPHER_ALG_AES,HI_UNF_CIPHER_WORK_MODE_CFB,HI_UNF_CIPHER_BIT_WIDTH_128BIT,HI_UNF_CIPHER_KEY_AES_256BIT},
// {HI_UNF_CIPHER_ALG_AES,HI_UNF_CIPHER_WORK_MODE_OFB,HI_UNF_CIPHER_BIT_WIDTH_128BIT,HI_UNF_CIPHER_KEY_AES_256BIT},
// ---------------------------------------------------------------------------------------------------------------
// // DES
// // 选择DES运算时，低64位数据有效，即CIPHER_KEY[63:0]为有效数据
// {HI_UNF_CIPHER_ALG_DES,HI_UNF_CIPHER_WORK_MODE_ECB,HI_UNF_CIPHER_BIT_WIDTH_64BIT,HI_UNF_CIPHER_KEY_AES_128BIT},
// {HI_UNF_CIPHER_ALG_DES,HI_UNF_CIPHER_WORK_MODE_CBC,HI_UNF_CIPHER_BIT_WIDTH_64BIT,HI_UNF_CIPHER_KEY_AES_128BIT},
// {HI_UNF_CIPHER_ALG_DES,HI_UNF_CIPHER_WORK_MODE_CFB,HI_UNF_CIPHER_BIT_WIDTH_1BIT, HI_UNF_CIPHER_KEY_AES_128BIT},
// {HI_UNF_CIPHER_ALG_DES,HI_UNF_CIPHER_WORK_MODE_CFB,HI_UNF_CIPHER_BIT_WIDTH_8BIT, HI_UNF_CIPHER_KEY_AES_128BIT},
// {HI_UNF_CIPHER_ALG_DES,HI_UNF_CIPHER_WORK_MODE_CFB,HI_UNF_CIPHER_BIT_WIDTH_64BIT,HI_UNF_CIPHER_KEY_AES_128BIT},
// {HI_UNF_CIPHER_ALG_DES,HI_UNF_CIPHER_WORK_MODE_OFB,HI_UNF_CIPHER_BIT_WIDTH_1BIT, HI_UNF_CIPHER_KEY_AES_128BIT},
// {HI_UNF_CIPHER_ALG_DES,HI_UNF_CIPHER_WORK_MODE_OFB,HI_UNF_CIPHER_BIT_WIDTH_8BIT, HI_UNF_CIPHER_KEY_AES_128BIT},
// {HI_UNF_CIPHER_ALG_DES,HI_UNF_CIPHER_WORK_MODE_OFB,HI_UNF_CIPHER_BIT_WIDTH_64BIT,HI_UNF_CIPHER_KEY_AES_128BIT},
// ---------------------------------------------------------------------------------------------------------------
// // 3DES
// {HI_UNF_CIPHER_ALG_3DES,HI_UNF_CIPHER_WORK_MODE_ECB,HI_UNF_CIPHER_BIT_WIDTH_64BIT,HI_UNF_CIPHER_KEY_DES_2KEY},
// {HI_UNF_CIPHER_ALG_3DES,HI_UNF_CIPHER_WORK_MODE_CBC,HI_UNF_CIPHER_BIT_WIDTH_64BIT,HI_UNF_CIPHER_KEY_DES_2KEY},
// {HI_UNF_CIPHER_ALG_3DES,HI_UNF_CIPHER_WORK_MODE_CFB,HI_UNF_CIPHER_BIT_WIDTH_1BIT, HI_UNF_CIPHER_KEY_DES_2KEY},
// {HI_UNF_CIPHER_ALG_3DES,HI_UNF_CIPHER_WORK_MODE_CFB,HI_UNF_CIPHER_BIT_WIDTH_8BIT, HI_UNF_CIPHER_KEY_DES_2KEY},
// {HI_UNF_CIPHER_ALG_3DES,HI_UNF_CIPHER_WORK_MODE_CFB,HI_UNF_CIPHER_BIT_WIDTH_64BIT,HI_UNF_CIPHER_KEY_DES_2KEY},
// {HI_UNF_CIPHER_ALG_3DES,HI_UNF_CIPHER_WORK_MODE_OFB,HI_UNF_CIPHER_BIT_WIDTH_1BIT, HI_UNF_CIPHER_KEY_DES_2KEY},
// {HI_UNF_CIPHER_ALG_3DES,HI_UNF_CIPHER_WORK_MODE_OFB,HI_UNF_CIPHER_BIT_WIDTH_8BIT, HI_UNF_CIPHER_KEY_DES_2KEY},
// {HI_UNF_CIPHER_ALG_3DES,HI_UNF_CIPHER_WORK_MODE_OFB,HI_UNF_CIPHER_BIT_WIDTH_64BIT,HI_UNF_CIPHER_KEY_DES_2KEY},
//
// {HI_UNF_CIPHER_ALG_3DES,HI_UNF_CIPHER_WORK_MODE_ECB,HI_UNF_CIPHER_BIT_WIDTH_64BIT,HI_UNF_CIPHER_KEY_DES_3KEY},
// {HI_UNF_CIPHER_ALG_3DES,HI_UNF_CIPHER_WORK_MODE_CBC,HI_UNF_CIPHER_BIT_WIDTH_64BIT,HI_UNF_CIPHER_KEY_DES_3KEY},
// {HI_UNF_CIPHER_ALG_3DES,HI_UNF_CIPHER_WORK_MODE_CFB,HI_UNF_CIPHER_BIT_WIDTH_1BIT, HI_UNF_CIPHER_KEY_DES_3KEY},
// {HI_UNF_CIPHER_ALG_3DES,HI_UNF_CIPHER_WORK_MODE_CFB,HI_UNF_CIPHER_BIT_WIDTH_8BIT, HI_UNF_CIPHER_KEY_DES_3KEY},
// {HI_UNF_CIPHER_ALG_3DES,HI_UNF_CIPHER_WORK_MODE_CFB,HI_UNF_CIPHER_BIT_WIDTH_64BIT,HI_UNF_CIPHER_KEY_DES_3KEY},
// {HI_UNF_CIPHER_ALG_3DES,HI_UNF_CIPHER_WORK_MODE_OFB,HI_UNF_CIPHER_BIT_WIDTH_1BIT, HI_UNF_CIPHER_KEY_DES_3KEY},
// {HI_UNF_CIPHER_ALG_3DES,HI_UNF_CIPHER_WORK_MODE_OFB,HI_UNF_CIPHER_BIT_WIDTH_8BIT, HI_UNF_CIPHER_KEY_DES_3KEY},
// {HI_UNF_CIPHER_ALG_3DES,HI_UNF_CIPHER_WORK_MODE_OFB,HI_UNF_CIPHER_BIT_WIDTH_64BIT,HI_UNF_CIPHER_KEY_DES_3KEY},
// -------------------------------------------------------------------------------------------------------------
// 返 回 值: 
//           成功: HI_ERR_SUCCESS
//           失败: 其他
// 日    期: [2016-09-23]
//*****************************************************************************
HI_U32 HI_DRV_CIPHER_ConfigChn(HI_HANDLE pHandle, HI_CIPHER_CTRL_S *pConfig);

//*****************************************************************************
// 函数名称: HI_DRV_CIPHER_GetHandleConfig
// 功能描述: 获取当前通道的参数配置
// 参数说明: 
//   pHandle         [IN] 句柄
//   pstCipherConfig [OUT] CIPHER参数 同函数HI_DRV_CIPHER_ConfigChn
// 返 回 值: 
//           成功: HI_ERR_SUCCESS
//           失败: 其他
// 日    期: [2016-09-23]
//*****************************************************************************
HI_U32 HI_DRV_CIPHER_GetHandleConfig(HI_HANDLE pHandle, HI_CIPHER_CTRL_S *pstCipherConfig);

//*****************************************************************************
// 函数名称: HI_DRV_CIPHER_Encrypt
// 功能描述: CIPHER 单链表加密
// 参数说明: 
//   pstCIData         [IN] 加密相关参数，具体如下
//                     pCIHandle 当前通道索引
//                     u32SrcAddr 加密源地址
//                     u32DestAddr 加密目的地址
//                     u32DataLength 加密数据长度
//
// 返 回 值: 
//           成功: HI_ERR_SUCCESS
//           失败: 其他
// 日    期: [2016-09-23]
//*****************************************************************************
HI_U32 HI_DRV_CIPHER_Encrypt(CIPHER_DATA_S *pstCIData);

//*****************************************************************************
// 函数名称: HI_DRV_CIPHER_Decrypt
// 功能描述: CIPHER 单链表解密
// 参数说明: 
//   pstCIData         [IN] 解密相关参数，具体如下
//                     pCIHandle 当前通道索引
//                     u32SrcAddr 解密源地址
//                     u32DestAddr 解密目的地址
//                     u32DataLength 解密数据长度
//
// 返 回 值: 
//           成功: HI_ERR_SUCCESS
//           失败: 其他
// 日    期: [2016-09-23]
//*****************************************************************************
HI_U32 HI_DRV_CIPHER_Decrypt(CIPHER_DATA_S *pstCIData);

//*****************************************************************************
// 函数名称: HI_DRV_CIPHER_EncryptMulti
// 功能描述: CIPHER 多链表加密
// 参数说明: 
//   psPkg         [IN] 加密相关参数，具体如下
//                 pCIHandle  当前通道索引
//                 u32PkgNum  待加密链表个数
//                 pu8Pkg  加密相关参数，具体如下
//                       u32SrcAddr 加密源地址
//                       u32DestAddr 加密目的地址
//                       u32DataLength 加密数据长度

// 返 回 值: 
//           成功: HI_ERR_SUCCESS
//           失败: 其他
// 日    期: [2016-09-23]
//*****************************************************************************
HI_U32 HI_DRV_CIPHER_EncryptMulti(CIPHER_PKG_S *psPkg);

//*****************************************************************************
// 函数名称: HI_DRV_CIPHER_DecryptMulti
// 功能描述: CIPHER 多链表解密
// 参数说明: 
//   psPkg         [IN] 解密相关参数，具体如下
//                 pCIHandle  当前通道索引
//                 u32PkgNum  待解密链表个数
//                 pu8Pkg  解密相关参数，具体如下
//                       u32SrcAddr 解密源地址
//                       u32DestAddr 解密目的地址
//                       u32DataLength 解密数据长度

// 返 回 值: 
//           成功: HI_ERR_SUCCESS
//           失败: 其他
// 日    期: [2016-09-23]
//*****************************************************************************
HI_U32 HI_DRV_CIPHER_DecryptMulti(CIPHER_PKG_S *psPkg);

//*****************************************************************************
// 函数名称: HI_DRV_CIPHER_DestroyHandle
// 功能描述: 销毁Handle
// 参数说明: 
//   pHandle         [IN]句柄
// 返 回 值: 
//           成功: HI_ERR_SUCCESS
//           失败: 其他
// 日    期: [2016-09-23]
//*****************************************************************************
HI_U32 HI_DRV_CIPHER_DestroyHandle(HI_HANDLE pHandle);

//*****************************************************************************
// 函数名称: HI_DRV_CIPHER_DeInit
// 功能描述: CIPHER去初始化
// 参数说明: 无
//
// 返 回 值: 无
//
// 日    期: [2016-09-23]
//*****************************************************************************
HI_U32 HI_DRV_CIPHER_DeInit(HI_VOID);

//*****************************************************************************
// 函数名称: HI_DRV_RANDOM_GetNumber
// 功能描述: 获取随机数
// 参数说明: 
//   u32TimeOutUs         [IN]超时时间
//   pulRng               [OUT]产生的随机数
// 返 回 值: 
//           成功: HI_ERR_SUCCESS
//           失败: 其他
// 日    期: [2016-09-23]
//*****************************************************************************
HI_U32 HI_DRV_RANDOM_GetNumber(HI_U32 u32TimeOutUs, HI_U32 *pulRng);

//*****************************************************************************
// 函数名称: HI_DRV_HASH_CalcInit
// 功能描述: hash初始化
// 参数说明: 
//   pCipherHashData         [IN]hash相关参数
//                           enShaType  哈希算法类型
//                           enHMACKeyFrom 使用硬件密钥或软件密钥
//                           *pu8HMACKey  密钥  
//                           u32ShaVal[8] hash计算的结果 & hash计算的初始值
//                           u32DataPhy   待hash计算的源数据地址
//                           u32DataLen   待hash计算的源数据长度
// 返 回 值: 
//           成功: HI_ERR_SUCCESS
//           失败: 其他
// 日    期: [2016-09-23]
//*****************************************************************************
HI_U32 HI_DRV_HASH_CalcInit(HASH_DATA_S *pHashData);

//*****************************************************************************
// 函数名称: HI_DRV_HASH_CalcUpdate
// 功能描述: hash分段计算
// 参数说明: 
//   pCipherHashData         [IN]hash相关参数
//                           enShaType  哈希算法类型
//                           enHMACKeyFrom 使用硬件密钥或软件密钥
//                           *pu8HMACKey  密钥  
//                           u32ShaVal[8] hash计算的结果 & hash计算的初始值
//                           u32DataPhy   待hash计算的源数据地址
//                           u32DataLen   待hash计算的源数据长度
// 返 回 值: 
//           成功: HI_ERR_SUCCESS
//           失败: 其他
// 日    期: [2016-09-23]
//*****************************************************************************
HI_U32 HI_DRV_HASH_CalcUpdate(HASH_DATA_S *pHashData);

//*****************************************************************************
// 函数名称: HI_DRV_HASH_CalcFinal
// 功能描述: hash最终结果读取
// 参数说明: 
//   pCipherHashData         [IN]hash相关参数
//                           enShaType  哈希算法类型
//                           enHMACKeyFrom 使用硬件密钥或软件密钥
//                           *pu8HMACKey  密钥  
//                           u32ShaVal[8] hash计算的结果 & hash计算的初始值
//                           u32DataPhy   待hash计算的源数据地址
//                           u32DataLen   待hash计算的源数据长度
// 返 回 值: 
//           成功: HI_ERR_SUCCESS
//           失败: 其他
// 日    期: [2016-09-23]
//*****************************************************************************
HI_U32 HI_DRV_HASH_CalcFinal(HASH_DATA_S *pHashData);

//*****************************************************************************
// 函数名称: HI_DRV_RSA_Calc
// 功能描述: RSA计算
// 参数说明: 
//   pCipherRsaData         [IN]RSA相关参数
//                          pu8Input(明文地址)
//                          *pu8Output(密文地址地址)
//                          u32DataLen(明文长度)
//                          *pu8N(模数地址)
//	                        *pu8K(公钥或私钥地址) 
//                          u16NLen(模数长度)
//                          u16KLen(公钥或私钥长度)
// 返 回 值: 
//           成功: HI_ERR_SUCCESS
//           失败: 其他
// 日    期: [2016-09-23]
//*****************************************************************************
HI_U32 HI_DRV_RSA_Calc(RSA_DATA_S *pRsaData);

#ifdef __cplusplus
}
#endif	/* __cplusplus */

#endif	/* End of #ifndef __HI_DRV_CIPHER_H__*/

