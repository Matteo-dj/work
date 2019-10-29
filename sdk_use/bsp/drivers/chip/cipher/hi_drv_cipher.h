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
// ��������: HI_DRV_CIPHER_Init
// ��������: CIPHER��ʼ��
// ����˵��: ��
//
// �� �� ֵ: 
//           �ɹ�: HI_ERR_SUCCESS
//           ʧ��: ����
// ��    ��: [2016-09-23]
//*****************************************************************************
HI_U32 HI_DRV_CIPHER_Init(HI_VOID);

//*****************************************************************************
// ��������: HI_DRV_CIPHER_CreateHandle
// ��������: ����Handle����ȡCIPHERͨ��ID
// ����˵��: 
//   enCipherType    [IN] ͨ������
//                   HI_UNF_CIPHER_TYPE_NORMAL = 0x0, // ��ͨ�� (ͨ��1-ͨ��7)
//                   HI_UNF_CIPHER_TYPE_SINGLE_CHANNEL = 1, // ��ͨ�� (ͨ��0)
//   pHandle         [OUT]���
// �� �� ֵ: 
//           �ɹ�: HI_ERR_SUCCESS
//           ʧ��: ����
// ��    ��: [2016-09-23]
//*****************************************************************************
HI_U32 HI_DRV_CIPHER_CreateHandle(HI_HANDLE pHandle, HI_CIPHER_TYPE_E enCipherType);

//*****************************************************************************
// ��������: HI_DRV_CIPHER_ConfigChn
// ��������: ��ǰͨ���Ĳ�������
// ����˵��: 
//   pHandle         [IN] ���
//   pConfig         [IN] CIPHER��������������
//                   u32Key[8]  ���ܻ������Կ
//                   u32IV[4]   ���ܻ��������
//                   bKeyByCA   �߰�ȫ�ӽ���(ʹ��Ӳ��key)
//                   bIVChangeFlag ���ܻ��������������оƬ�Ĵ���
//   
//                   enAlg      ���ܻ�����㷨(������ϳ������±�)                                                                                              /**< CNcomment:�����㷨 */
//                   enBitWidth ���ܻ���ܵ�λ��(������ϳ������±�) 
//                   enWorkMode ���ܻ���ܹ���ģʽ(������ϳ������±�) 
//                   enKeyLen  ��Կ����(������ϳ������±�) 
// ---------------------------------------------------------------------------------------------------------------
// // AES
// // ��оƬ��Կ����ģ��������Կ(bKeyByCA = 1)ʱ��Կ����ֻ֧��128λ
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
// // ѡ��DES����ʱ����64λ������Ч����CIPHER_KEY[63:0]Ϊ��Ч����
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
// �� �� ֵ: 
//           �ɹ�: HI_ERR_SUCCESS
//           ʧ��: ����
// ��    ��: [2016-09-23]
//*****************************************************************************
HI_U32 HI_DRV_CIPHER_ConfigChn(HI_HANDLE pHandle, HI_CIPHER_CTRL_S *pConfig);

//*****************************************************************************
// ��������: HI_DRV_CIPHER_GetHandleConfig
// ��������: ��ȡ��ǰͨ���Ĳ�������
// ����˵��: 
//   pHandle         [IN] ���
//   pstCipherConfig [OUT] CIPHER���� ͬ����HI_DRV_CIPHER_ConfigChn
// �� �� ֵ: 
//           �ɹ�: HI_ERR_SUCCESS
//           ʧ��: ����
// ��    ��: [2016-09-23]
//*****************************************************************************
HI_U32 HI_DRV_CIPHER_GetHandleConfig(HI_HANDLE pHandle, HI_CIPHER_CTRL_S *pstCipherConfig);

//*****************************************************************************
// ��������: HI_DRV_CIPHER_Encrypt
// ��������: CIPHER ���������
// ����˵��: 
//   pstCIData         [IN] ������ز�������������
//                     pCIHandle ��ǰͨ������
//                     u32SrcAddr ����Դ��ַ
//                     u32DestAddr ����Ŀ�ĵ�ַ
//                     u32DataLength �������ݳ���
//
// �� �� ֵ: 
//           �ɹ�: HI_ERR_SUCCESS
//           ʧ��: ����
// ��    ��: [2016-09-23]
//*****************************************************************************
HI_U32 HI_DRV_CIPHER_Encrypt(CIPHER_DATA_S *pstCIData);

//*****************************************************************************
// ��������: HI_DRV_CIPHER_Decrypt
// ��������: CIPHER ���������
// ����˵��: 
//   pstCIData         [IN] ������ز�������������
//                     pCIHandle ��ǰͨ������
//                     u32SrcAddr ����Դ��ַ
//                     u32DestAddr ����Ŀ�ĵ�ַ
//                     u32DataLength �������ݳ���
//
// �� �� ֵ: 
//           �ɹ�: HI_ERR_SUCCESS
//           ʧ��: ����
// ��    ��: [2016-09-23]
//*****************************************************************************
HI_U32 HI_DRV_CIPHER_Decrypt(CIPHER_DATA_S *pstCIData);

//*****************************************************************************
// ��������: HI_DRV_CIPHER_EncryptMulti
// ��������: CIPHER ���������
// ����˵��: 
//   psPkg         [IN] ������ز�������������
//                 pCIHandle  ��ǰͨ������
//                 u32PkgNum  �������������
//                 pu8Pkg  ������ز�������������
//                       u32SrcAddr ����Դ��ַ
//                       u32DestAddr ����Ŀ�ĵ�ַ
//                       u32DataLength �������ݳ���

// �� �� ֵ: 
//           �ɹ�: HI_ERR_SUCCESS
//           ʧ��: ����
// ��    ��: [2016-09-23]
//*****************************************************************************
HI_U32 HI_DRV_CIPHER_EncryptMulti(CIPHER_PKG_S *psPkg);

//*****************************************************************************
// ��������: HI_DRV_CIPHER_DecryptMulti
// ��������: CIPHER ���������
// ����˵��: 
//   psPkg         [IN] ������ز�������������
//                 pCIHandle  ��ǰͨ������
//                 u32PkgNum  �������������
//                 pu8Pkg  ������ز�������������
//                       u32SrcAddr ����Դ��ַ
//                       u32DestAddr ����Ŀ�ĵ�ַ
//                       u32DataLength �������ݳ���

// �� �� ֵ: 
//           �ɹ�: HI_ERR_SUCCESS
//           ʧ��: ����
// ��    ��: [2016-09-23]
//*****************************************************************************
HI_U32 HI_DRV_CIPHER_DecryptMulti(CIPHER_PKG_S *psPkg);

//*****************************************************************************
// ��������: HI_DRV_CIPHER_DestroyHandle
// ��������: ����Handle
// ����˵��: 
//   pHandle         [IN]���
// �� �� ֵ: 
//           �ɹ�: HI_ERR_SUCCESS
//           ʧ��: ����
// ��    ��: [2016-09-23]
//*****************************************************************************
HI_U32 HI_DRV_CIPHER_DestroyHandle(HI_HANDLE pHandle);

//*****************************************************************************
// ��������: HI_DRV_CIPHER_DeInit
// ��������: CIPHERȥ��ʼ��
// ����˵��: ��
//
// �� �� ֵ: ��
//
// ��    ��: [2016-09-23]
//*****************************************************************************
HI_U32 HI_DRV_CIPHER_DeInit(HI_VOID);

//*****************************************************************************
// ��������: HI_DRV_RANDOM_GetNumber
// ��������: ��ȡ�����
// ����˵��: 
//   u32TimeOutUs         [IN]��ʱʱ��
//   pulRng               [OUT]�����������
// �� �� ֵ: 
//           �ɹ�: HI_ERR_SUCCESS
//           ʧ��: ����
// ��    ��: [2016-09-23]
//*****************************************************************************
HI_U32 HI_DRV_RANDOM_GetNumber(HI_U32 u32TimeOutUs, HI_U32 *pulRng);

//*****************************************************************************
// ��������: HI_DRV_HASH_CalcInit
// ��������: hash��ʼ��
// ����˵��: 
//   pCipherHashData         [IN]hash��ز���
//                           enShaType  ��ϣ�㷨����
//                           enHMACKeyFrom ʹ��Ӳ����Կ�������Կ
//                           *pu8HMACKey  ��Կ  
//                           u32ShaVal[8] hash����Ľ�� & hash����ĳ�ʼֵ
//                           u32DataPhy   ��hash�����Դ���ݵ�ַ
//                           u32DataLen   ��hash�����Դ���ݳ���
// �� �� ֵ: 
//           �ɹ�: HI_ERR_SUCCESS
//           ʧ��: ����
// ��    ��: [2016-09-23]
//*****************************************************************************
HI_U32 HI_DRV_HASH_CalcInit(HASH_DATA_S *pHashData);

//*****************************************************************************
// ��������: HI_DRV_HASH_CalcUpdate
// ��������: hash�ֶμ���
// ����˵��: 
//   pCipherHashData         [IN]hash��ز���
//                           enShaType  ��ϣ�㷨����
//                           enHMACKeyFrom ʹ��Ӳ����Կ�������Կ
//                           *pu8HMACKey  ��Կ  
//                           u32ShaVal[8] hash����Ľ�� & hash����ĳ�ʼֵ
//                           u32DataPhy   ��hash�����Դ���ݵ�ַ
//                           u32DataLen   ��hash�����Դ���ݳ���
// �� �� ֵ: 
//           �ɹ�: HI_ERR_SUCCESS
//           ʧ��: ����
// ��    ��: [2016-09-23]
//*****************************************************************************
HI_U32 HI_DRV_HASH_CalcUpdate(HASH_DATA_S *pHashData);

//*****************************************************************************
// ��������: HI_DRV_HASH_CalcFinal
// ��������: hash���ս����ȡ
// ����˵��: 
//   pCipherHashData         [IN]hash��ز���
//                           enShaType  ��ϣ�㷨����
//                           enHMACKeyFrom ʹ��Ӳ����Կ�������Կ
//                           *pu8HMACKey  ��Կ  
//                           u32ShaVal[8] hash����Ľ�� & hash����ĳ�ʼֵ
//                           u32DataPhy   ��hash�����Դ���ݵ�ַ
//                           u32DataLen   ��hash�����Դ���ݳ���
// �� �� ֵ: 
//           �ɹ�: HI_ERR_SUCCESS
//           ʧ��: ����
// ��    ��: [2016-09-23]
//*****************************************************************************
HI_U32 HI_DRV_HASH_CalcFinal(HASH_DATA_S *pHashData);

//*****************************************************************************
// ��������: HI_DRV_RSA_Calc
// ��������: RSA����
// ����˵��: 
//   pCipherRsaData         [IN]RSA��ز���
//                          pu8Input(���ĵ�ַ)
//                          *pu8Output(���ĵ�ַ��ַ)
//                          u32DataLen(���ĳ���)
//                          *pu8N(ģ����ַ)
//	                        *pu8K(��Կ��˽Կ��ַ) 
//                          u16NLen(ģ������)
//                          u16KLen(��Կ��˽Կ����)
// �� �� ֵ: 
//           �ɹ�: HI_ERR_SUCCESS
//           ʧ��: ����
// ��    ��: [2016-09-23]
//*****************************************************************************
HI_U32 HI_DRV_RSA_Calc(RSA_DATA_S *pRsaData);

#ifdef __cplusplus
}
#endif	/* __cplusplus */

#endif	/* End of #ifndef __HI_DRV_CIPHER_H__*/

