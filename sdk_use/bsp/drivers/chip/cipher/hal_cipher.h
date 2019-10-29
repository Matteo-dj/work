/******************************************************************************

  Copyright (C), 2011-2014, Hisilicon Tech. Co., Ltd.

 ******************************************************************************
  File Name     : hal_cipher.h
  Version       : Initial Draft
  Author        : Hisilicon hisecurity team
  Created       : 
  Last Modified :
  Description   : 
  Function List :
  History       :
******************************************************************************/

#ifndef __HAL_CIPHER_H__
#define __HAL_CIPHER_H__
#include <hi_platform.h>
#include <hi_types.h>
#include <hi_driver_internal.h>
#include "drv_cipher_reg.h"
#include "drv_cipher_define.h"
#include <hi_mpi_cipher.h>
#include <hi_mdm_cipher.h>

#ifdef __cplusplus
extern "C" {
#endif
#define TEST_EFUSE_4_KEY
/***************************** Macro Definition ******************************/
#define CIPHER_CHAN_NUM                        (8)
#define CIPHER_PKGx1_CHAN                      (0)
#define CIPHER_PKGxN_CHAN_MIN                  (1)
#define CIPHER_PKGxN_CHAN_MAX                  (7)
#define CIPHER_HMAC_KEY_LEN                    (16)
#define CIPHER_MHASH_SUPPORT
#define CONFIG_RSA_HARDWARE_SUPPORT
typedef HI_VOID (*funcCipherCallback)(HI_U32);
/*************************** Structure Definition ****************************/
typedef struct hiHI_MMZ_BUF_S
{
    HI_U8* user_viraddr;
    HI_U8* user_viraddr_original;
    HI_U32 bufsize;
}HI_MMZ_BUF_S;

typedef enum hiCIPHER_INT_TYPE_E
{
    CIPHER_INT_TYPE_IN  = 					0x1,
    CIPHER_INT_TYPE_OUT = 					0x2,
} CIPHER_INT_TYPE_E;

typedef enum hiCIPHER_BUF_TYPE_E
{
    CIPHER_BUF_TYPE_IN  = 					0x1,
    CIPHER_BUF_TYPE_OUT = 					0x2,
} CIPHER_BUF_TYPE_E;

typedef enum hiCIPHER_RSA_DATA_TYPE_E
{
    CIPHER_RSA_DATA_TYPE_CONTEXT,
    CIPHER_RSA_DATA_TYPE_MODULE,
    CIPHER_RSA_DATA_TYPE_KEY,
} CIPHER_RSA_DATA_TYPE_E;

typedef enum hiCIPHER_RSA_KEY_WIDTH_E
{ 
    CIPHER_RSA_KEY_WIDTH_1K = 0x00,
    CIPHER_RSA_KEY_WIDTH_2K = 0x01,
    CIPHER_RSA_KEY_WIDTH_4K = 0x02,
}CIPHER_RSA_KEY_WIDTH_E;

typedef struct hiCIPHER_HASH_DATA_S
{
    HI_MDM_HASH_TYPE_E enShaType;
    HASH_HMAC_KEY_FROM_E enHMACKeyFrom;
    HI_U8   *pu8HMACKey;
    HI_U32  u32ShaVal[8];
    HI_U32  u32DataPhy;
    HI_U32  u32DataLen;
}HASH_DATA_S;

typedef struct hiCIPHER_DATA_INFO_S
{
    HI_U32  u32src;
    HI_U32  u32dest;
    HI_U32  u32length;
    HI_BOOL bDecrypt;
    HI_U8   u8Rsv[3];
    HI_U32  u32DataPkg[4];
}HI_DRV_CIPHER_DATA_INFO_S;

typedef enum hiCIPHER_HDCP_KEY_RAM_MODE_E
{
    CIPHER_HDCP_KEY_RAM_MODE_READ = 0x0,
    CIPHER_HDCP_KEY_RAM_MODE_WRITE,
    CIPHER_HDCP_KEY_RAM_MODE_BUTT,
}HI_DRV_CIPHER_HDCP_KEY_RAM_MODE_E;





/******************************* API declaration *****************************/
HI_U32 HAL_Cipher_GetInBufCnt(HI_U32 chnId, HI_U32 *pNum);
HI_U32 HAL_Cipher_GetInBufEmpty(HI_U32 chnId, HI_U32 *pNum);
HI_U32 HAL_Cipher_GetInBufNum(HI_U32 chnId, HI_U32 *pNum);
HI_U32 HAL_Cipher_GetOutBufCnt(HI_U32 chnId, HI_U32 *pNum);
HI_U32 HAL_Cipher_GetOutBufFull(HI_U32 chnId, HI_U32 *pNum);
HI_U32 HAL_Cipher_GetOutBufNum(HI_U32 chnId, HI_U32 *pNum);
HI_U32 HAL_Cipher_SetInBufCnt(HI_U32 chnId, HI_U32 num);
HI_U32 HAL_Cipher_SetInBufEmpty(HI_U32 chnId, HI_U32 num);
HI_U32 HAL_Cipher_SetInBufNum(HI_U32 chnId, HI_U32 num);
HI_U32 HAL_Cipher_GetSrcLstRaddr(HI_U32 chnId, HI_U32 *addr);
HI_U32 HAL_Cipher_GetDestLstRaddr(HI_U32 chnId, HI_U32 *addr);
HI_U32 HAL_Cipher_SetOutBufCnt(HI_U32 chnId, HI_U32 num);
HI_U32 HAL_Cipher_SetOutBufFull(HI_U32 chnId, HI_U32 num);
HI_U32 HAL_Cipher_SetOutBufNum(HI_U32 chnId, HI_U32 num);
HI_VOID HAL_Cipher_ClrMmuIntState(HI_U32 intStatus);
HI_U32 HAL_Cipher_SetMmuBypass(HI_U32 chnId);
HI_U32 HAL_Cipher_SetAGEThreshold(HI_U32 chnId, CIPHER_INT_TYPE_E intType, HI_U16 value);
HI_U32 HAL_Cipher_SetIntThreshold(HI_U32 chnId, CIPHER_INT_TYPE_E intType, HI_U32 value);
HI_VOID HAL_Cipher_DisableAllInt(HI_VOID);
HI_U32 HAL_Cipher_DisableInt(HI_U32 chnId, int intType);
HI_VOID HAL_Cipher_EnableInt(HI_U32 chnId, int intType);
HI_VOID HAL_Cipher_GetRawIntState(HI_U32 *pState);
HI_VOID HAL_Cipher_ClrIntState(HI_U32 intStatus);
HI_VOID HAL_Cipher_GetIntState(HI_U32 *pState);
HI_VOID HAL_Cipher_GetIntEnState(HI_U32 *pState);
HI_VOID HAL_Cipher_EnableAllSecChn(HI_VOID);
HI_VOID HAL_Cipher_SetHDMITxRxOtpSelReg(HI_VOID);
HI_U32 HAL_Cipher_Config(HI_U32 chnId, HI_BOOL bDecrypt, HI_BOOL bIVChange, HI_CIPHER_CTRL_S* pCtrl);
HI_U32 HAL_Cipher_SetBufAddr(HI_U32 chnId, CIPHER_BUF_TYPE_E bufType, HI_U32 addr);
HI_U32 HAL_Cipher_SetInIV(HI_U32 chnId, HI_CIPHER_CTRL_S* pCtrl);
HI_U32 HAL_Cipher_GetOutIV(HI_U32 chnId, HI_CIPHER_CTRL_S* pCtrl);
HI_U32 HAL_Cipher_SetSoftwareKey(HI_U32 chnId, HI_CIPHER_CTRL_S* pCtrl);
HI_U32 HAL_Cipher_SetDataSinglePkg(HI_DRV_CIPHER_DATA_INFO_S * info);
HI_U32 HAL_Cipher_StartSinglePkg(HI_U32 chnId);
HI_U32 HAL_Cipher_ReadDataSinglePkg(HI_U32 pData[4]);
HI_U32 HAL_Cipher_WaitIdle(HI_VOID);
HI_BOOL HAL_Cipher_IsIdle(HI_U32 chn);
HI_VOID HAL_Cipher_Reset(HI_VOID);
HI_U32 HAL_Hash_CalcInit(HASH_DATA_S *pHashData);
HI_U32 HAL_Hash_CalcUpdate(HASH_DATA_S *pHashData);
HI_U32 HAL_Hash_CalcFinal(HASH_DATA_S *pHashData);
HI_U32 HAL_Random_GetNumber(HI_IN HI_U32 u32TimeOutUs, HI_OUT HI_U32 *pulRng);
HI_U32 HAL_Cipher_ClearCbcVerifyFlag(HI_VOID);
HI_VOID HAL_CIPHER_ReadReg(HI_U32* addr, HI_U32 *pu32Val);
HI_VOID HAL_CIPHER_WriteReg(HI_U32* addr, HI_U32 u32Val);
HI_U32 HAL_RSA_WaitFree(HI_VOID);
HI_VOID HAL_RSA_Start(HI_VOID);
HI_VOID HAL_RSA_ClearRam(HI_VOID);
HI_VOID HAL_RSA_Crc(HI_VOID);
HI_VOID HAL_RSA_GetCrc(HI_U32 *pu32Crc);
HI_VOID HAL_RSA_ConfigMode(CIPHER_RSA_KEY_WIDTH_E enKenWidth);
HI_VOID HAL_RSA_WriteData(CIPHER_RSA_DATA_TYPE_E enDataType, HI_U8 *pu8Data, HI_U32 u32DataLen, HI_U32 u32Len);
HI_VOID HAL_RSA_ReadData(HI_U8 *pu8Data, HI_U32 u32DataLen, HI_U32 u32Len);
HI_U32 HAL_RSA_GetErrorCode(HI_VOID);
#ifdef TEST_EFUSE_4_KEY
HI_EXTERN HI_U32 g_hash_hmac_hw_key_times;
#endif
#ifdef __cplusplus
}
#endif
#endif /* __HAL_CIPHER_H__ */


