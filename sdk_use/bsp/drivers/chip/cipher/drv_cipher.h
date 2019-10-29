/******************************************************************************

  Copyright (C), 2011-2014, Hisilicon Tech. Co., Ltd.

 ******************************************************************************
  File Name     : drv_cipher.h
  Version       : Initial Draft
  Author        : Hisilicon hisecurity team
  Created       : 
  Last Modified :
  Description   : 
  Function List :
  History       :
******************************************************************************/

#ifndef __DRV_CIPHER_H__
#define __DRV_CIPHER_H__

/* add include here */
#include "hal_cipher.h"
#include "hi_drv_cipher.h"
#include <hi_mdm.h>
#ifdef __cplusplus
extern "C" {
#endif

/***************************** Macro Definition ******************************/
#define CIPHER_DEFAULT_INT_NUM        (1)
#define CIPHER_CRYPT_WAIT_TIME_MAX    (2000)
#define RSA_TIMEOUT_US_MAX            (0xffffffff)
#define CIPHER_BLOCK_SIZE_AES         (16)
#define CIPHER_BLOCK_SIZE_DES         (8)
HI_U32 DRV_RSA_CalcCrc(HI_U16 *pu16Crc);
HI_U32 DRV_RSA_Calc_ex(RSA_DATA_S *pRsaData);
HI_U32 DRV_HASH_SemInit(HI_VOID);
HI_U32 DRV_RSA_SemInit(HI_VOID);
HI_U32 DRV_HASH_SemDeInit(HI_VOID);
HI_U32 DRV_RSA_SemDeInit(HI_VOID);

#ifdef __cplusplus
}
#endif
#endif /* __DRV_CIPHER_H__ */
