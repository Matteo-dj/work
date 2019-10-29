/******************************************************************************

  Copyright (C), 2010-2014, Huawei Tech. Co., Ltd.

******************************************************************************
  File Name     : hi_mpi_cipher.h
  Version       : Initial Draft
  Author        :
  Created       :
  Last Modified :
  Description   :
  Function List :
******************************************************************************/
#ifndef __HI_MPI_CIPHER_H__
#define __HI_MPI_CIPHER_H__

#include <hi_mdm_cipher.h>

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* __cplusplus */

typedef enum
{
    HI_UNF_HASH_TYPE_SHA1,
    HI_UNF_HASH_TYPE_SHA256,
    HI_UNF_HASH_TYPE_HMAC_SHA1,
    HI_UNF_HASH_TYPE_HMAC_SHA256,
    HI_UNF_HASH_TYPE_MAX,
} HI_UNF_HASH_TYPE_E;

typedef HI_MDM_HASH_ATTR_S HI_UNF_HASH_ATTS_S;



HI_EXTERN HI_U32 HI_MPI_HASH_Init(HI_VOID);
HI_EXTERN HI_U32 HI_MPI_HASH_DeInit(HI_VOID);
HI_EXTERN HI_U32 HI_MPI_HASH_Open(HI_U32 ulHashMaxLen);
HI_EXTERN HI_VOID HI_MPI_HASH_Close(HI_VOID);
HI_EXTERN HI_U32 HI_MPI_HASH_CalcInit(HI_UNF_HASH_ATTS_S *pstHashAttr, HI_HANDLE pHashHandle);
HI_EXTERN HI_U32 HI_MPI_HASH_CalcUpdate(HI_HANDLE pHashHandle, HI_U8 *pu8InputData, HI_U32 u32InputDataLen);
HI_EXTERN HI_U32 HI_MPI_HASH_CalcFinal(HI_HANDLE pHashHandle, HI_U8 *pu8OutputHash);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

#endif /* __HI_MPI_CIPHER_H__ */

