/******************************************************************************

  Copyright (C), 2001-2011, Huawei Tech. Co., Ltd.

 ******************************************************************************
  File Name     : mpi_cipher.c
  Version       : Initial Draft
  Author        :
  Created       :
  Last Modified :
  Description   :
  Function List :
******************************************************************************/
#include "hal_cipher.h"
#include <hi_driver_internal.h>
#include <sal_inf.h>
#include <os_vx_sal_api.h>
#include "hi_drv_cipher.h"
#include "drv_cipher_define.h"
#include "drv_cipher.h"
#include <hi_mdm_types.h>
#include "hi_mpi_cipher.h"
#include <hi_low_power.h>
#include <hi_mdm_clock.h>
// 初始化次数统计
static HI_S32 g_HashInitCounter = -1;
static HI_S32 g_RsaInitCounter = -1;   

// 初始化信号量
HI_U32 g_stMpiHashInitSemHd = 0;

// mpi层hash驱动信号量
HI_U32 g_stMpiHashSemHd = {0};

// 初始化信号量保护接口
#define HI_HASH_INIT_LOCK()  	 (HI_VOID)HI_MDM_SEM_Wait(g_stMpiHashInitSemHd, HI_SYS_WAIT_FOREVER)
#define HI_HASH_INIT_UNLOCK()  	 (HI_VOID)HI_MDM_SEM_Signal(g_stMpiHashInitSemHd)

// mpi层hash驱动信号量保护
#define HI_HASH_LOCK()  	     (HI_VOID)HI_MDM_SEM_Wait(g_stMpiHashSemHd, HI_SYS_WAIT_FOREVER)
#define HI_HASH_UNLOCK()  	     (HI_VOID)HI_MDM_SEM_Signal(g_stMpiHashSemHd)

#define CHECK_CIPHER_OPEN()\
do{\
    if (g_CipherInitCounter < 0)\
    {\
        HI_ERR_CIPHER("CIPHER is not open.\n");\
        return HI_ERR_NO_INITILIZATION;\
    }\
}while(0)

#define CHECK_HASH_OPEN()\
do{\
    if (g_HashInitCounter < 0)\
    {\
        HI_ERR_CIPHER("CIPHER is not open.\n");\
        return HI_ERR_NO_INITILIZATION;\
    }\
}while(0)

#define CHECK_RSA_OPEN()\
do{\
    if (g_RsaInitCounter < 0)\
    {\
        HI_ERR_CIPHER("CIPHER is not open.\n");\
        return HI_ERR_NO_INITILIZATION;\
    }\
}while(0)

#define CHECK_GANDOM_OPEN()\
do{\
    if (g_GandomInitCounter < 0)\
    {\
        HI_ERR_CIPHER("CIPHER is not open.\n");\
        return HI_ERR_NO_INITILIZATION;\
    }\
}while(0)

#define HASH_CHANNAL_MAX_NUM   (8)
#define HASH_BLOCK_SIZE		   (64)
#define HASH_PAD_MAX_LEN	   (64)
#define HASH1_SIGNATURE_SIZE   (20)
#define HASH256_SIGNATURE_SIZE (32)

#define HASH_MMZ_TAIL_LEN	   (8*1024)			//8K
#define MAX_DATA_LEN           (0x2000) //the max data length for encryption / decryption is  8k one time
#define RSA_SIGN               (1)
#define ASN1_HASH_SHA1         "\x30\x21\x30\x09\x06\x05\x2b\x0e\x03\x02\x1a\x05\x00\x04\x14"
#define ASN1_HASH_SHA256       "\x30\x31\x30\x0d\x06\x09\x60\x86\x48\x01\x65\x03\x04\x02\x01\x05\x00\x04\x20"

typedef struct hiHASH_INFO_S
{
    HI_BOOL bIsUsed;
    HI_U8 u8Rsv[3];
    HI_HANDLE hHandle;
    HI_UNF_HASH_TYPE_E enShaType;
    HI_U32 u32TotalDataLen;
    HI_U32 u32ShaVal[8];
    HI_U32 u32ShaLen;
    HI_U8 u8LastBlock[HASH_BLOCK_SIZE];
    HI_U8 u8Mac[HASH_BLOCK_SIZE];
    HI_U32 u8LastBlockSize;
    HI_MMZ_BUF_S stMMZBuffer;
    HASH_HMAC_KEY_FROM_E enHMACKeyFrom;
}HASH_INFO_S;

static HASH_INFO_S g_stCipherHashData[HASH_CHANNAL_MAX_NUM];
HI_MMZ_BUF_S s_stMMZBuffer;//for test
static const HI_S8 EMPTY_L_SHA1[]   = "\xda\x39\xa3\xee\x5e\x6b\x4b\x0d\x32\x55\xbf\xef\x95\x60\x18\x90\xaf\xd8\x07\x09";
static const HI_S8 EMPTY_L_SHA256[] = "\xe3\xb0\xc4\x42\x98\xfc\x1c\x14\x9a\xfb\xf4\xc8\x99\x6f\xb9\x24\x27\xae\x41\xe4\x64\x9b\x93\x4c\xa4\x95\x99\x1b\x78\x52\xb8\x55";

/*----------------------------------------------*
 * external variables                           *
 *----------------------------------------------*/

/*----------------------------------------------*
 * external routine prototypes                  *
 *----------------------------------------------*/

/*----------------------------------------------*
 * internal routine prototypes                  *
 *----------------------------------------------*/

/*----------------------------------------------*
 * project-wide global variables                *
 *----------------------------------------------*/

/*----------------------------------------------*
 * module-wide global variables                 *
 *----------------------------------------------*/

/*----------------------------------------------*
 * constants                                    *
 *----------------------------------------------*/

/*----------------------------------------------*
 * macros                                       *
 *----------------------------------------------*/

/*----------------------------------------------*
 * routines' implementations                    *
 *----------------------------------------------*/

/*********************************************************
 * The function below is added for AES CBC-MAC
 *
 *********************************************************/
HI_PRV HI_U32 HASH_SemInit(HI_VOID)
{
    return HI_MDM_SEM_BCreate(&g_stMpiHashSemHd,"hash",HI_SEM_ONE);
}

HI_PRV HI_U32 HASH_SemDeInit(HI_VOID)
{
    return HI_MDM_SEM_Delete(g_stMpiHashSemHd);
}

static HI_U32 Cipher_HashMmzInit(HI_U32 ulHashMaxLen)
{

    s_stMMZBuffer.bufsize = ulHashMaxLen;
    s_stMMZBuffer.user_viraddr_original = (HI_U8*)HI_MDM_Malloc(HI_MOD_ID_DRV_CIPHER, s_stMMZBuffer.bufsize + ALIGNTYPE_64BYTE);

    if (HI_NULL == s_stMMZBuffer.user_viraddr_original)
    {
        HI_ERR_CIPHER("Can NOT get mem s_stMMZBuffer.user_viraddr_original\r\n");
        return HI_ERR_FAILURE;
    }

    s_stMMZBuffer.user_viraddr = (HI_U8*) ALIGN_LENGTH((HI_U32)s_stMMZBuffer.user_viraddr_original, ALIGNTYPE_32BYTE);
    (hi_void)memset_s((HI_U8 *)s_stMMZBuffer.user_viraddr, s_stMMZBuffer.bufsize, 0, s_stMMZBuffer.bufsize);
    return HI_ERR_SUCCESS;

}

static HI_VOID Cipher_HashMmzDeInit(HI_VOID)
{
    HI_MDM_Free(HI_MOD_ID_DRV_CIPHER, s_stMMZBuffer.user_viraddr_original);
    s_stMMZBuffer.user_viraddr = HI_NULL;
    s_stMMZBuffer.user_viraddr_original = HI_NULL;
}

static HI_U32 HashMsgPadding(HI_U8 *pu8Msg, HI_U32 u32ByteLen, HI_U32 u32TotalLen)
{
    HI_U32 u32Tmp = 0;
    HI_U32 u32PaddingLen;

    if( HI_NULL == pu8Msg )
    {
        HI_ERR_CIPHER("Error! HI_NULL pointer input!\n");
        return HI_ERR_FAILURE;
    }
        
    u32Tmp = u32TotalLen % 64;
    /* 56 = 64 - 8, 120 = 56 + 64 */
    u32PaddingLen = (u32Tmp < 56) ? (56 - u32Tmp) : (120 - u32Tmp);
    /* add 8 bytes fix data length */
    u32PaddingLen += 8;

    /* Format(binary): {data|1000...00| fix_data_len(bits)} */
    pu8Msg[u32ByteLen++] = 0x80;
    (hi_void)memset_s((HI_VOID *)&pu8Msg[u32ByteLen], u32PaddingLen - 1 - 8, 0, u32PaddingLen - 1 - 8);
    u32ByteLen += u32PaddingLen - 1 - 8;
    
    /* write 8 bytes fix data length */
    pu8Msg[u32ByteLen++] = 0x00;
    pu8Msg[u32ByteLen++] = 0x00;
    pu8Msg[u32ByteLen++] = 0x00;
    pu8Msg[u32ByteLen++] = (HI_U8)((u32TotalLen >> 29)&0x07);
	pu8Msg[u32ByteLen++] = (HI_U8)((u32TotalLen >> 21)&0xff);
	pu8Msg[u32ByteLen++] = (HI_U8)((u32TotalLen >> 13)&0xff);
	pu8Msg[u32ByteLen++] = (HI_U8)((u32TotalLen >> 5)&0xff);
	pu8Msg[u32ByteLen++] = (HI_U8)((u32TotalLen << 3)&0xff);
    
    return u32ByteLen;
}

static HI_U32 HASH_Init(HI_UNF_HASH_ATTS_S *pstHashAttr, HI_HANDLE pHashHandle)
{
    HI_U32 ret = HI_ERR_SUCCESS;
    HASH_DATA_S stHashData;
    HI_U32 u32SoftId;
    HASH_INFO_S *pstHashInfo = HI_NULL;

    CHECK_HASH_OPEN();

    if((HI_NULL == pstHashAttr)
     || (HI_NULL == pHashHandle)
     || (pstHashAttr->eShaType >= HI_MDM_HASH_TYPE_MAX))
    {
        HI_ERR_CIPHER("HASH_Init Invalid parameter!\n");
        return HI_ERR_CIPHER_INVALID_PARA;
    }

    HI_HASH_LOCK();

    for(u32SoftId = 0; u32SoftId < HASH_CHANNAL_MAX_NUM; u32SoftId++)
    {
        if (!g_stCipherHashData[u32SoftId].bIsUsed)
        {
            break;
        }
    }

    if(u32SoftId >= HASH_CHANNAL_MAX_NUM)
    {
        HI_ERR_CIPHER("Hash module is busy!\n");
        HI_HASH_UNLOCK();
        return HI_ERR_FAILURE;       
    }
    
    pstHashInfo = &g_stCipherHashData[u32SoftId];
    (hi_void)memset_s((HI_VOID *)pstHashInfo, sizeof(HASH_INFO_S), 0, sizeof(HASH_INFO_S));
    (hi_void)memset_s((HI_VOID *)&stHashData, sizeof(HASH_DATA_S), 0, sizeof(HASH_DATA_S));
	pstHashInfo->stMMZBuffer = s_stMMZBuffer;
    pstHashInfo->enShaType = (HI_UNF_HASH_TYPE_E)pstHashAttr->eShaType;
    pstHashInfo->enHMACKeyFrom = pstHashAttr->enHMACKeyFrom;
	if ((HI_MDM_HASH_TYPE_SHA1 == pstHashAttr->eShaType)
	  || (HI_MDM_HASH_TYPE_HMAC_SHA1 == pstHashAttr->eShaType))
	{
        pstHashInfo->u32ShaLen = HASH1_SIGNATURE_SIZE;
        pstHashInfo->u32ShaVal[0] = 0x01234567;
        pstHashInfo->u32ShaVal[1] = 0x89abcdef;
        pstHashInfo->u32ShaVal[2] = 0xfedcba98;
        pstHashInfo->u32ShaVal[3] = 0x76543210;
        pstHashInfo->u32ShaVal[4] = 0xf0e1d2c3;
	}
	else if ((HI_MDM_HASH_TYPE_SHA256 == pstHashAttr->eShaType)
		 || (HI_MDM_HASH_TYPE_HMAC_SHA256 == pstHashAttr->eShaType))
	{
        pstHashInfo->u32ShaLen = HASH256_SIGNATURE_SIZE;
        pstHashInfo->u32ShaVal[0] = 0x67e6096a; 
        pstHashInfo->u32ShaVal[1] = 0x85ae67bb; 
        pstHashInfo->u32ShaVal[2] = 0x72f36e3c; 
        pstHashInfo->u32ShaVal[3] = 0x3af54fa5; 
        pstHashInfo->u32ShaVal[4] = 0x7f520e51; 
        pstHashInfo->u32ShaVal[5] = 0x8c68059b; 
        pstHashInfo->u32ShaVal[6] = 0xabd9831f; 
        pstHashInfo->u32ShaVal[7] = 0x19cde05b; 
	}
    
    stHashData.enShaType = pstHashAttr->eShaType;
    stHashData.enHMACKeyFrom = pstHashAttr->enHMACKeyFrom;
    ret = HI_DRV_HASH_CalcInit(&stHashData);
    if(ret != HI_ERR_SUCCESS)
    {
        HI_ERR_CIPHER("Error, ioctl for hash initial failed!\n");
        HI_HASH_UNLOCK();
        return ret;
    }

    pstHashInfo->bIsUsed = HI_TRUE;
    *(HI_U32 *)pHashHandle = u32SoftId;
    HI_HASH_UNLOCK();    
    return HI_ERR_SUCCESS;
}

static HI_U32 HASH_Update(HI_HANDLE pHashHandle, HI_U8 *pu8InputData, HI_U32 u32InputDataLen)
{
    HI_U32 ret = HI_ERR_SUCCESS;
    HASH_INFO_S *pstHashInfo = HI_NULL;
	HI_U32 u32Tmp = 0;
    HI_U32 u32TotalSize = 0;
    HI_U32 u32CopySize = 0;
    HI_U32 u32BufInUsedLen = 0;
    HASH_DATA_S stHashData = {0};/*lint !e64*/
    HI_U32 i = 0;

    if((!pu8InputData) || (!pHashHandle) || ((*(HI_U32 *)pHashHandle) >= HASH_CHANNAL_MAX_NUM))
    {
        HI_ERR_CIPHER("HASH_Update Invalid parameter!\n");
        return HI_ERR_CIPHER_INVALID_PARA;
    }

    pstHashInfo = &g_stCipherHashData[*(HI_U32 *)pHashHandle];  
    if (0 == u32InputDataLen)
    {
        return HI_ERR_SUCCESS;
    }

    CHECK_HASH_OPEN();
    HI_HASH_LOCK();
    pstHashInfo->stMMZBuffer.bufsize = 0;
	pstHashInfo->u32TotalDataLen += u32InputDataLen;
    u32TotalSize = pstHashInfo->u8LastBlockSize + u32InputDataLen;
    
    if(u32TotalSize < HASH_BLOCK_SIZE)
    {
        // 总长度如果小于64， 将本次的输入数据存储到u8LastBlock，u8LastBlockSize长度加长本次数据的长度
        (hi_void)memcpy_s(pstHashInfo->u8LastBlock+pstHashInfo->u8LastBlockSize, u32InputDataLen, pu8InputData, u32InputDataLen);
        pstHashInfo->u8LastBlockSize+=u32InputDataLen;
        HI_HASH_UNLOCK();
	    return HI_ERR_SUCCESS;
    }
    else
    {
        // 总长度如果大于64， 将上次保存的数据拷贝到user_viraddr中，bufsize为上次保存的数据长度
        (hi_void)memcpy_s(pstHashInfo->stMMZBuffer.user_viraddr, 
                    pstHashInfo->u8LastBlockSize, 
                    pstHashInfo->u8LastBlock, 
                    pstHashInfo->u8LastBlockSize);
        pstHashInfo->stMMZBuffer.bufsize = pstHashInfo->u8LastBlockSize;

        /* save tail data */
        // 将上次保存的数据清零
        (hi_void)memset_s((HI_VOID *)pstHashInfo->u8LastBlock, sizeof(pstHashInfo->u8LastBlock), 0, HASH_BLOCK_SIZE);
        // 本次保存不足一块(即长度为64)的数据
        pstHashInfo->u8LastBlockSize = (u32InputDataLen + pstHashInfo->stMMZBuffer.bufsize) % HASH_BLOCK_SIZE;
        (hi_void)memcpy_s(pstHashInfo->u8LastBlock,
                    sizeof(pstHashInfo->u8LastBlock),
                    pu8InputData + (u32InputDataLen - pstHashInfo->u8LastBlockSize), 
                    pstHashInfo->u8LastBlockSize);
    }

    // u32TotalSize为本次输入的数据长度减去不足1块(即长度为64)的数据，本次需要做哈希的数据
    u32TotalSize  = u32InputDataLen - pstHashInfo->u8LastBlockSize;
    
    u32Tmp = (u32TotalSize + pstHashInfo->stMMZBuffer.bufsize + s_stMMZBuffer.bufsize - 1) / s_stMMZBuffer.bufsize;
    /* Send data down piece by piece */
    for (i = 0 ; i < u32Tmp; i++)
    {
        u32CopySize = s_stMMZBuffer.bufsize - pstHashInfo->stMMZBuffer.bufsize;
        if (u32CopySize > (u32TotalSize - u32BufInUsedLen))
        {
            u32CopySize = u32TotalSize - u32BufInUsedLen;
        }
        
        HI_INFO_CIPHER("s_stMMZBuffer_bufsize: 0x%x, u32TotalSize:0x%x, u32BufInUsedLen: 0x%x, buffersize: 0x%x\n", 
            s_stMMZBuffer.bufsize, u32TotalSize, u32BufInUsedLen, pstHashInfo->stMMZBuffer.bufsize);
        // 将输入数据拷贝至上次不足1块数据的后面
        (hi_void)memcpy_s((HI_U8 *)pstHashInfo->stMMZBuffer.user_viraddr + pstHashInfo->stMMZBuffer.bufsize,
                    u32CopySize,
    		        pu8InputData + u32BufInUsedLen, 
    		        u32CopySize);
        HI_MDM_DataCacheFlush((HI_PVOID)pstHashInfo->stMMZBuffer.user_viraddr, (HI_S32)s_stMMZBuffer.bufsize); 
	    pstHashInfo->stMMZBuffer.bufsize += u32CopySize;
        // 本次输入数据，已经拷贝的长度
        u32BufInUsedLen+=u32CopySize;
        stHashData.enShaType = pstHashInfo->enShaType;/*lint !e64*/
		stHashData.u32DataLen = pstHashInfo->stMMZBuffer.bufsize;
        stHashData.u32DataPhy = (HI_U32)pstHashInfo->stMMZBuffer.user_viraddr;// for test zn
        (hi_void)memcpy_s(stHashData.u32ShaVal, sizeof(stHashData.u32ShaVal), pstHashInfo->u32ShaVal, pstHashInfo->u32ShaLen);
		pstHashInfo->stMMZBuffer.bufsize = 0;
        stHashData.enHMACKeyFrom = pstHashInfo->enHMACKeyFrom;
		ret = HI_DRV_HASH_CalcUpdate(&stHashData);
	    if(ret != HI_ERR_SUCCESS)
	    {
	        HI_ERR_CIPHER("Error, ioctl for hash update failed!\n");
            pstHashInfo->bIsUsed = HI_FALSE;
            HI_HASH_UNLOCK();
	        return ret;
	    }

        (hi_void)memcpy_s(pstHashInfo->u32ShaVal, sizeof(pstHashInfo->u32ShaVal), stHashData.u32ShaVal, pstHashInfo->u32ShaLen);
    }
    
    if((u32BufInUsedLen + pstHashInfo->u8LastBlockSize) != u32InputDataLen)
    {
        HI_ERR_CIPHER("Error, Invalid data size: 0x%x!\n", u32BufInUsedLen); 
        pstHashInfo->bIsUsed = HI_FALSE;
        HI_HASH_UNLOCK();
        return HI_ERR_FAILURE;
    }

    HI_HASH_UNLOCK();
    return HI_ERR_SUCCESS;
}

static HI_U32 HASH_Final(HI_HANDLE pHashHandle, HI_U8 *pu8OutputHash)
{
    HI_U32 ret = HI_ERR_SUCCESS;
    HASH_INFO_S *pstHashInfo = HI_NULL;
    HASH_DATA_S stHashData;
	HI_U32 u32Tmp = 0;

    if((!pu8OutputHash) || (!pHashHandle) || ((*(HI_U32 *)pHashHandle) >= HASH_CHANNAL_MAX_NUM))
    {
        HI_ERR_CIPHER("HASH_Final Invalid parameter!\n");
        return HI_ERR_FAILURE;
    }
    
    (hi_void)memset_s((HI_VOID *)&stHashData, sizeof(stHashData), 0, sizeof(stHashData));
    pstHashInfo = &g_stCipherHashData[*(HI_U32 *)pHashHandle];
    
    CHECK_HASH_OPEN();
    HI_HASH_LOCK();
    (hi_void)memcpy_s(pstHashInfo->stMMZBuffer.user_viraddr, pstHashInfo->u8LastBlockSize, pstHashInfo->u8LastBlock, pstHashInfo->u8LastBlockSize);
    pstHashInfo->stMMZBuffer.bufsize = pstHashInfo->u8LastBlockSize;
    (hi_void)memset_s((HI_VOID *)pstHashInfo->u8LastBlock, sizeof(pstHashInfo->u8LastBlock), 0, HASH_BLOCK_SIZE);
    pstHashInfo->u8LastBlockSize = 0;
    if (((HI_UNF_HASH_TYPE_HMAC_SHA1 == pstHashInfo->enShaType) || (HI_UNF_HASH_TYPE_HMAC_SHA256 == pstHashInfo->enShaType)) 
        && (HI_CIPHER_HMAC_KEY_FROM_CA == pstHashInfo->enHMACKeyFrom))
    {
        pstHashInfo->u32TotalDataLen += HASH_BLOCK_SIZE;// zn
    }
    
    u32Tmp = HashMsgPadding(pstHashInfo->stMMZBuffer.user_viraddr, pstHashInfo->stMMZBuffer.bufsize, pstHashInfo->u32TotalDataLen);
    stHashData.enShaType = (HI_MDM_HASH_TYPE_E)pstHashInfo->enShaType;
    stHashData.u32DataLen = u32Tmp;
    stHashData.u32DataPhy = (HI_U32)pstHashInfo->stMMZBuffer.user_viraddr;// for test zn
    (hi_void)memcpy_s(stHashData.u32ShaVal, pstHashInfo->u32ShaLen, pstHashInfo->u32ShaVal, pstHashInfo->u32ShaLen);
    stHashData.enHMACKeyFrom = pstHashInfo->enHMACKeyFrom;

    ret = HI_DRV_HASH_CalcFinal(&stHashData);
    if(ret != HI_ERR_SUCCESS)
    {
        HI_ERR_CIPHER("Error, ioctl for hash final failed!\n");
        pstHashInfo->bIsUsed = HI_FALSE;
        HI_HASH_UNLOCK();
        return ret;
    }

    (hi_void)memcpy_s(pu8OutputHash, pstHashInfo->u32ShaLen, stHashData.u32ShaVal, pstHashInfo->u32ShaLen);
    pstHashInfo->bIsUsed = HI_FALSE;
    HI_HASH_UNLOCK();   
    return HI_ERR_SUCCESS;
}

static HI_U32 CIPHER_HmacKeyInit(HI_UNF_HASH_ATTS_S *pstHashAttr, HI_U8 au8Hmackey[HASH_BLOCK_SIZE])
{
    HI_U32 ret = HI_ERR_SUCCESS;
    HI_U32 hHash = 0;
    HI_HANDLE pHash = (HI_HANDLE)&hHash;

    if((pstHashAttr == HI_NULL)||(pstHashAttr->pu8HMACKey == HI_NULL)||(pstHashAttr->u32HMACKeyLen == 0))
    {
        HI_ERR_CIPHER("Invalid parameters.\n");
        return HI_ERR_CIPHER_INVALID_PARA;
    }

    /*key length is less than 64bytes, copy directly*/
    if(pstHashAttr->u32HMACKeyLen <= HASH_BLOCK_SIZE)
    {
        (hi_void)memcpy_s(au8Hmackey, pstHashAttr->u32HMACKeyLen, pstHashAttr->pu8HMACKey, pstHashAttr->u32HMACKeyLen);
        return HI_ERR_SUCCESS;
    }

    /*key length more than 64bytes, calcute the hash result*/
    ret = HASH_Init(pstHashAttr, pHash);
    if(ret != HI_ERR_SUCCESS)
    {
        HI_ERR_CIPHER("HASH_Init failure, ret=%d\n", ret);
        return HI_ERR_FAILURE;
    }

    ret = HASH_Update(pHash, pstHashAttr->pu8HMACKey, pstHashAttr->u32HMACKeyLen);
    if(ret != HI_ERR_SUCCESS)
    {
        HI_ERR_CIPHER("HASH_Update failure, ret=%d\n", ret);
        return HI_ERR_FAILURE;
    }

    ret = HASH_Final(pHash,au8Hmackey);
    if(ret != HI_ERR_SUCCESS)
    {
        HI_ERR_CIPHER("HASH_Final failure, ret=%d\n", ret);
        return HI_ERR_FAILURE;
    }

    return HI_ERR_SUCCESS;
}

static HI_U32 HASH_HmacInit(HI_UNF_HASH_ATTS_S *pstHashAttr, HI_HANDLE pHashHandle)
{
    HI_U32 ret = HI_ERR_FAILURE;
    HI_U8 key_pad[HASH_BLOCK_SIZE] = {0};
    HASH_INFO_S *pstHashInfo = HI_NULL;
	HI_U32 i = 0;

    if((HI_NULL == pstHashAttr)
    || (HI_NULL == pHashHandle)
    || (pstHashAttr->eShaType >= HI_MDM_HASH_TYPE_MAX)
    || (HI_NULL == pstHashAttr->pu8HMACKey)
    || (0 == pstHashAttr->u32HMACKeyLen))
    {
        HI_ERR_CIPHER("HASH_HmacInit Invalid parameter!\n");
        return HI_ERR_CIPHER_INVALID_PARA;
    }

    CHECK_HASH_OPEN();
    
	/* Init hmac key */
    (hi_void)memset_s((HI_VOID *)key_pad, sizeof(key_pad), 0, sizeof(key_pad));
    ret = CIPHER_HmacKeyInit(pstHashAttr, key_pad);
	if ( HI_ERR_SUCCESS != ret )
	{
	    HI_ERR_CIPHER("Hmac key initial failed!\n");
		return HI_ERR_FAILURE;
	} 

	/* hash i_key_pad and message start */
	ret = HASH_Init(pstHashAttr, pHashHandle);
	if ( HI_ERR_SUCCESS != ret )
	{
	    HI_ERR_CIPHER("hash i_key_pad and message start failed!\n");
		return HI_ERR_FAILURE;
	}
    pstHashInfo = (HASH_INFO_S *)&g_stCipherHashData[*(HI_U32 *)pHashHandle];
    (hi_void)memcpy_s(pstHashInfo->u8Mac, sizeof(pstHashInfo->u8Mac), key_pad, HASH_BLOCK_SIZE);
    pstHashInfo->enHMACKeyFrom = pstHashAttr->enHMACKeyFrom;
	/* generate i_key_pad */
    for(i=0; i < HASH_BLOCK_SIZE; i++)
    {
        key_pad[i] ^= 0x36;
    }

	/* hash i_key_pad update */
    ret = HASH_Update(pHashHandle, key_pad, HASH_BLOCK_SIZE);
	if (HI_ERR_SUCCESS != ret)
	{
	    HI_ERR_CIPHER("hash i_key_pad and message update failed!\n"); 
		return HI_ERR_FAILURE;
	}

    return HI_ERR_SUCCESS;
}

static HI_U32 HASH_HmacUpdate(HI_HANDLE pHashHandle, HI_U8 *pu8InputData, HI_U32 u32InputDataLen)
{
    HI_U32 ret = HI_ERR_SUCCESS;

	ret = HASH_Update(pHashHandle, pu8InputData, u32InputDataLen);
	if ( HI_ERR_SUCCESS != ret )
	{
	    HI_ERR_CIPHER("hmac message update failed!\n"); 
		return HI_ERR_FAILURE;
	}

    return HI_ERR_SUCCESS;
}

static HI_U32 CIPHER_HmacFinal(HI_HANDLE pHashHandle, HI_U8 *pu8Output)
{
    HI_U32 ret = HI_ERR_SUCCESS;
    HASH_INFO_S *pstHashInfo = HI_NULL;
    HI_U32 hHash = 0;
	HI_HANDLE pHash = (HI_HANDLE)&hHash;
	HI_UNF_HASH_ATTS_S stHashAttr;
    HI_U8 hash_sum_1[HASH256_SIGNATURE_SIZE];
    HI_U8 key_pad[HASH_BLOCK_SIZE];
    HI_U32 i;

	if (!pu8Output || !pHashHandle || ((*(HI_U32*)pHashHandle) >= HASH_CHANNAL_MAX_NUM))
	{
   	    HI_ERR_CIPHER("Invalid param input, hmac final failed!\n"); 
		return HI_ERR_INVALID_PARAMETER;
	}

    pstHashInfo = &(g_stCipherHashData[(*(HI_U32*)pHashHandle)]);   
    (hi_void)memset_s((HI_VOID *)&stHashAttr, sizeof(stHashAttr), 0, sizeof(stHashAttr));
	stHashAttr.eShaType = pstHashInfo->enShaType;/*lint !e64*/
    stHashAttr.enHMACKeyFrom = pstHashInfo->enHMACKeyFrom;
    // u8Mac 存储的是原始key
    (hi_void)memcpy_s(key_pad, sizeof(key_pad), pstHashInfo->u8Mac, HASH_BLOCK_SIZE);

    /* hash i_key_pad+message finished */
    // 将i_key_pad+message 剩余不足64长度的补足做hash计算，结果存储到hash_sum_1
    ret = HASH_Final(pHashHandle, hash_sum_1);
    if(ret != HI_ERR_SUCCESS)
    {
        HI_ERR_CIPHER("Hash Final i_key_pad+message failure, ret=%d\n", ret);
        return HI_ERR_FAILURE;
    }

    // 将原始key ^= 0x5c，如果长度大于64就做hash计算，长度小于64，就直接存储
    /* generate o_key_pad */
    for(i=0; i < HASH_BLOCK_SIZE; i++)
    {
        key_pad[i] ^= 0x5c;
    }

    /* hash o_key_pad+hash_sum_1 start */
    ret = HASH_Init(&stHashAttr, pHash);
    if(ret != HI_ERR_SUCCESS)
    {
        HI_ERR_CIPHER("Hash Init o_key_pad+hash_sum_1 failure, ret=%d\n", ret);
        return HI_ERR_FAILURE;
    }

    /* hash o_key_pad */
    ret = HASH_Update(pHash, key_pad, HASH_BLOCK_SIZE);
    if(ret != HI_ERR_SUCCESS)
    {
        HI_ERR_CIPHER("Hash Update o_key_pad failure, ret=%d\n", ret);
        return HI_ERR_FAILURE;
    }

    /* hash hash_sum_1 */
    if (HI_MDM_HASH_TYPE_HMAC_SHA1 == stHashAttr.eShaType)/*lint !e58*/
    {
        ret = HASH_Update(pHash, (HI_U8 *)hash_sum_1, HASH1_SIGNATURE_SIZE);	
    }
    else
    {
        ret = HASH_Update(pHash, (HI_U8 *)hash_sum_1, HASH256_SIGNATURE_SIZE);
    }

    if(ret != HI_ERR_SUCCESS)
    {
        HI_ERR_CIPHER("Hash Update hash_sum_1 failure, ret=%d\n", ret);
        return HI_ERR_FAILURE;
    }

    /* hash o_key_pad+hash_sum_1 finished */
    ret = HASH_Final(pHash, pu8Output);
    if(ret != HI_ERR_SUCCESS)
    {
        HI_ERR_CIPHER("Hash Final o_key_pad+hash_sum_1 failure, ret=%d\n", ret);
        return HI_ERR_FAILURE;
    }

    return HI_ERR_SUCCESS;
}

HI_U32 HI_MPI_HASH_Init(HI_VOID)
{
    return HI_MDM_SEM_BCreate(&g_stMpiHashInitSemHd,"mhash",HI_SEM_ONE);
}

HI_U32 HI_MPI_HASH_DeInit(HI_VOID)
{
    return  HI_MDM_SEM_Delete(g_stMpiHashInitSemHd);
}

HI_U32 HI_MPI_HASH_Open(HI_U32 ulHashMaxLen)
{
    HI_U32 ret = HI_ERR_FAILURE;

    HI_HASH_INIT_LOCK();
    
    if (g_HashInitCounter > 0)
    {
        g_HashInitCounter++;
        ret = HI_ERR_SUCCESS;
    }
    else
    {
        g_HashInitCounter = 1;
        
        ret = Cipher_HashMmzInit(ulHashMaxLen);
        if (HI_ERR_SUCCESS == ret)
        {
            (hi_void)memset_s((HI_VOID *)g_stCipherHashData, sizeof(g_stCipherHashData), 0, sizeof(g_stCipherHashData));
            HASH_SemInit();// mpi驱动接口的信号量保护
            DRV_HASH_SemInit();// drv驱动接口的信号量保护
        }
        else
        {
            HI_ERR_CIPHER("Hash mmz buffer initial failed!\n");
        }
    }
    
    HI_HASH_INIT_UNLOCK();
    return ret;
}

HI_VOID HI_MPI_HASH_Close(HI_VOID)
{
	HI_HASH_INIT_LOCK();
	
	if (g_HashInitCounter > 0)
	{
		g_HashInitCounter--;
		if (g_HashInitCounter == 0)
		{
			g_HashInitCounter = -1;
			Cipher_HashMmzDeInit();
			HASH_SemDeInit();
			DRV_HASH_SemDeInit();
		}

	}
	
	HI_HASH_INIT_UNLOCK();
}
HI_U32 HI_MPI_HASH_CalcInit(HI_UNF_HASH_ATTS_S *pstHashAttr, HI_HANDLE pHashHandle)
{
    HI_U32 ret=HI_ERR_SUCCESS;
    HI_MDM_SYS_CLOCK_Enable(HI_SYSCTRL_CLKEN_REG,HI_SYSCTRL_CLKEN_REG_BIT_SHIFT_SHA);
    HI_LOW_POWER_Forbid(HI_LOW_POWER_FORBID_ID_HASH);

    if((HI_NULL == pstHashAttr) || (HI_NULL == pHashHandle))
    {
        HI_ERR_CIPHER("HI_MPI_HASH_CalcInit Invalid parameter!\n");
        ret = HI_ERR_CIPHER_INVALID_PARA;
        goto end;
    }
    if((pstHashAttr->enHMACKeyFrom >= HI_CIPHER_HMAC_KEY_FROM_MAX)||(pstHashAttr->eShaType >= HI_MDM_HASH_TYPE_MAX))
    {
        ret = HI_ERR_CIPHER_INVALID_PARA;
        goto end;
    }
    if ((HI_MDM_HASH_TYPE_SHA1 == pstHashAttr->eShaType)
        || (HI_MDM_HASH_TYPE_SHA256 == pstHashAttr->eShaType)
        || (((HI_MDM_HASH_TYPE_HMAC_SHA1 == pstHashAttr->eShaType) || (HI_MDM_HASH_TYPE_HMAC_SHA256 == pstHashAttr->eShaType))
            && (HI_CIPHER_HMAC_KEY_FROM_CA == pstHashAttr->enHMACKeyFrom)))
    {
        ret = HASH_Init(pstHashAttr, pHashHandle);
        goto end;   
    }
    else if (((HI_MDM_HASH_TYPE_HMAC_SHA1 == pstHashAttr->eShaType) || (HI_MDM_HASH_TYPE_HMAC_SHA256 == pstHashAttr->eShaType))
        && (HI_CIPHER_HMAC_KEY_FROM_CPU == pstHashAttr->enHMACKeyFrom))
    {
        ret = HASH_HmacInit(pstHashAttr, pHashHandle);
        goto end;
    }

    ret=HI_ERR_FAILURE;

end:
    if(ret)
    {
        HI_LOW_POWER_UnForbid(HI_LOW_POWER_FORBID_ID_HASH, 0);
        HI_MDM_SYS_CLOCK_Disable(HI_SYSCTRL_CLKEN_REG,HI_SYSCTRL_CLKEN_REG_BIT_SHIFT_SHA);
    }
    return ret;
}

HI_U32 HI_MPI_HASH_CalcUpdate(HI_HANDLE pHashHandle, HI_U8* pu8InputData, HI_U32 u32InputDataLen)
{
    
    HI_U32 ret=HI_ERR_SUCCESS;
    HASH_INFO_S *pstHashInfo = HI_NULL;

    if(!pu8InputData || !pHashHandle || (*((HI_U32 *)pHashHandle) >= HASH_CHANNAL_MAX_NUM))
    {
        HI_ERR_CIPHER("HI_MPI_HASH_CalcUpdate Invalid parameter!\n");
        ret = HI_ERR_CIPHER_INVALID_PARA;
        goto end;
    }
    
    pstHashInfo = &g_stCipherHashData[*(HI_U32 *)pHashHandle];
    
    if ((HI_UNF_HASH_TYPE_SHA1 == pstHashInfo->enShaType)
        || (HI_UNF_HASH_TYPE_SHA256 == pstHashInfo->enShaType)
        || (((HI_UNF_HASH_TYPE_HMAC_SHA1 == pstHashInfo->enShaType) || (HI_UNF_HASH_TYPE_HMAC_SHA256 == pstHashInfo->enShaType)) 
            && (HI_CIPHER_HMAC_KEY_FROM_CA == pstHashInfo->enHMACKeyFrom)))
	{
            ret = HASH_Update(pHashHandle, pu8InputData, u32InputDataLen);
            goto end;
	}
    else if (((HI_UNF_HASH_TYPE_HMAC_SHA1 == pstHashInfo->enShaType) || (HI_UNF_HASH_TYPE_HMAC_SHA256 == pstHashInfo->enShaType)) 
            && (HI_CIPHER_HMAC_KEY_FROM_CPU == pstHashInfo->enHMACKeyFrom))
    {
        ret = HASH_HmacUpdate(pHashHandle, pu8InputData, u32InputDataLen);
        goto end;
    }
    ret= HI_ERR_FAILURE;
end:
    if(ret)
    {
        HI_LOW_POWER_UnForbid(HI_LOW_POWER_FORBID_ID_HASH, 0);
        HI_MDM_SYS_CLOCK_Disable(HI_SYSCTRL_CLKEN_REG,HI_SYSCTRL_CLKEN_REG_BIT_SHIFT_SHA);
    }
    return ret;
}

HI_U32 HI_MPI_HASH_CalcFinal(HI_HANDLE pHashHandle, HI_U8 *pu8OutputHash)
{
    
    HI_U32 ret=HI_ERR_SUCCESS;
    HASH_INFO_S *pstHashInfo = HI_NULL;

    if(!pu8OutputHash || !pHashHandle || ((*(HI_U32 *)pHashHandle) >= HASH_CHANNAL_MAX_NUM))
    {
        HI_ERR_CIPHER("HI_MPI_HASH_CalcFinal Invalid parameter!\n");
        ret=HI_ERR_CIPHER_INVALID_PARA;
        goto end;
    }

    pstHashInfo = (HASH_INFO_S*)(&g_stCipherHashData[*(HI_U32 *)pHashHandle]);
    
    if ((HI_UNF_HASH_TYPE_SHA1 == pstHashInfo->enShaType) 
        || (HI_UNF_HASH_TYPE_SHA256 == pstHashInfo->enShaType) 
        || (((HI_UNF_HASH_TYPE_HMAC_SHA1 == pstHashInfo->enShaType) || (HI_UNF_HASH_TYPE_HMAC_SHA256 == pstHashInfo->enShaType)) 
            && (HI_CIPHER_HMAC_KEY_FROM_CA == pstHashInfo->enHMACKeyFrom)))
	{
		ret=HASH_Final(pHashHandle, pu8OutputHash);
        goto end;
	}
    else if (((HI_UNF_HASH_TYPE_HMAC_SHA1 == pstHashInfo->enShaType) || (HI_UNF_HASH_TYPE_HMAC_SHA256 == pstHashInfo->enShaType)) 
                && (HI_CIPHER_HMAC_KEY_FROM_CPU == pstHashInfo->enHMACKeyFrom))
    {
        ret=CIPHER_HmacFinal(pHashHandle, pu8OutputHash);
        goto end;
    }
    
    ret=HI_ERR_FAILURE;
end:
    HI_LOW_POWER_UnForbid(HI_LOW_POWER_FORBID_ID_HASH, 0);
    HI_MDM_SYS_CLOCK_Disable(HI_SYSCTRL_CLKEN_REG,HI_SYSCTRL_CLKEN_REG_BIT_SHIFT_SHA);
    return ret;    
}

static HI_U32 RSA_GetAttr(HI_U32 u32SchEme, 
                          HI_U16 u16NLen,
                          HI_U32 *pu32HLen,
                          HI_U32 *pu32KeyLen,
                          HI_UNF_HASH_TYPE_E *pstenHashType)
{
    HI_U32 ret = HI_ERR_SUCCESS;

    CHECK_RSA_OPEN();
    
    if ((pu32HLen == HI_NULL) || (pu32KeyLen == HI_NULL) || (pstenHashType == HI_NULL))
    {
        HI_ERR_CIPHER("para is HI_NULL.\n");
        return HI_ERR_INVALID_PARAMETER;
    }

    if (u16NLen <= 512)
    {
        *pu32KeyLen = u16NLen;
    }
    else 
    {
        HI_ERR_CIPHER("u16NLen(0x%x) is invalid\n", u16NLen);
        return HI_ERR_INVALID_PARAMETER;
    }

    *pu32HLen = 0;
    *pstenHashType = HI_UNF_HASH_TYPE_MAX;
    
    switch(u32SchEme)
    {
    case HI_UNF_RSA_ENC_SCHEME_NO_PADDING:
    case HI_UNF_RSA_ENC_SCHEME_BLOCK_TYPE_0:
    case HI_UNF_RSA_ENC_SCHEME_BLOCK_TYPE_1:         
    case HI_UNF_RSA_ENC_SCHEME_BLOCK_TYPE_2:
    case HI_UNF_RSA_ENC_SCHEME_RSAES_PKCS1_V1_5:
        *pu32HLen = 0;
        *pstenHashType = HI_UNF_HASH_TYPE_MAX;
        break;
    case HI_UNF_RSA_ENC_SCHEME_RSAES_OAEP_SHA1:     
    case HI_UNF_RSA_SIGN_SCHEME_RSASSA_PKCS1_V15_SHA1:
    case HI_UNF_RSA_SIGN_SCHEME_RSASSA_PKCS1_PSS_SHA1:
        *pu32HLen = 20;
        *pstenHashType = HI_UNF_HASH_TYPE_SHA1;
        break;
    case HI_UNF_RSA_ENC_SCHEME_RSAES_OAEP_SHA256:
    case HI_UNF_RSA_SIGN_SCHEME_RSASSA_PKCS1_V15_SHA256:
    case HI_UNF_RSA_SIGN_SCHEME_RSASSA_PKCS1_PSS_SHA256:
        *pu32HLen = 32;
        *pstenHashType = HI_UNF_HASH_TYPE_SHA256;
        break;
    default:
        HI_ERR_CIPHER("RSA scheme (0x%x) is invalid.\n", u32SchEme);
        ret = HI_ERR_CIPHER_INVALID_PARA;
    }

    return ret;
}

static HI_U32 RSA_PKCS1_MGF1(HI_UNF_HASH_TYPE_E enHashType,
                             HI_U8  *pu8Seed,  
                             HI_U32 u32Seedlen, 
                             HI_U8 *pu8Mask, HI_U32 u32MaskLen)
{
    HI_U32 Ret = HI_ERR_SUCCESS;
    HI_U32 i = 0;
    HI_U32 j = 0;
    HI_U32 u32Outlen = 0;
    HI_U8 u8Cnt[4] = {0};
    HI_U8 u8Md[32] = {0};
    HI_U32 u32MdLen = 0;
    HI_UNF_HASH_ATTS_S stHashAttr = {0};
    HI_U32 HashHandle = 0;

    CHECK_RSA_OPEN();

    if ((pu8Seed == HI_NULL) || (pu8Mask == HI_NULL))
    {
        HI_ERR_CIPHER("para is HI_NULL.\n");
        return HI_ERR_INVALID_PARAMETER;
    }

    (hi_void)memset_s((HI_VOID *)&stHashAttr, sizeof(HI_UNF_HASH_ATTS_S), 0, sizeof(HI_UNF_HASH_ATTS_S));

    /*PKCS#1 V2.1 only use sha1 function, Others allow for future expansion*/
    stHashAttr.eShaType = enHashType;/*lint !e64*/
    if( enHashType == HI_UNF_HASH_TYPE_SHA1)
    {
        u32MdLen = 20;
    }
    else if( enHashType == HI_UNF_HASH_TYPE_SHA256)
    {
        u32MdLen = 32;
    }
    else
    {
        HI_ERR_CIPHER("hash type is invalid.\n");
        return HI_ERR_CIPHER_INVALID_PARA;
    }

    for (i = 0; u32Outlen < u32MaskLen; i++) 
    {
        u8Cnt[0] = (HI_U8)((i >> 24) & 0xFF);
        u8Cnt[1] = (HI_U8)((i >> 16) & 0xFF);
        u8Cnt[2] = (HI_U8)((i >> 8)) & 0xFF;
        u8Cnt[3] = (HI_U8)(i & 0xFF);
        
        Ret = HI_MPI_HASH_CalcInit(&stHashAttr, (HI_HANDLE)&HashHandle);
        if (Ret != HI_ERR_SUCCESS)
        {
    		return Ret;
        }
        
        Ret = HI_MPI_HASH_CalcUpdate((HI_HANDLE)&HashHandle, pu8Seed, u32Seedlen);
        if (Ret != HI_ERR_SUCCESS)
        {
    		return Ret;
        }
        Ret = HI_MPI_HASH_CalcUpdate((HI_HANDLE)&HashHandle, u8Cnt, 4);
        if (Ret != HI_ERR_SUCCESS)
        {
    		return Ret;
        }
        Ret = HI_MPI_HASH_CalcFinal((HI_HANDLE)&HashHandle, u8Md);
        if (Ret != HI_ERR_SUCCESS)
        {
    		return Ret;
        }
        for(j=0; (j<u32MdLen) && (u32Outlen < u32MaskLen); j++)
		{
			pu8Mask[u32Outlen++] ^= u8Md[j];	
		}
    }

    return HI_ERR_SUCCESS;
}

static HI_U32 RSA_GetRandomNumber(HI_U8 *pu8Rand, HI_U32 u32Size)
{
    HI_U32 ret = HI_ERR_SUCCESS;
    HI_U32 i;
    HI_U32 u32Rand;

    (hi_void)memset_s((HI_VOID *)pu8Rand, u32Size, 0, u32Size);
    for(i=0; i<u32Size; i+=4)
    {
        ret = HI_DRV_RANDOM_GetNumber((HI_U32)(RSA_TIMEOUT_US_MAX), &u32Rand);
        if (ret != HI_ERR_SUCCESS)
        {
            HI_ERR_CIPHER("Get random number failed, ret = %x\n", ret);
            return HI_ERR_FAILURE;
        }
        pu8Rand[i+3]   = (HI_U8)(u32Rand >> 24)& 0xFF;
        pu8Rand[i+2] = (HI_U8)(u32Rand >> 16)& 0xFF;
        pu8Rand[i+1] = (HI_U8)(u32Rand >> 8)& 0xFF;
        pu8Rand[i+0] = (HI_U8)(u32Rand) & 0xFF;
    }

    /*non-zero random octet string*/
    for(i=0; i<u32Size; i++)
    {
        if (pu8Rand[i] == 0x0)
        {
            ret = HI_DRV_RANDOM_GetNumber((HI_U32)(RSA_TIMEOUT_US_MAX), &u32Rand);
            if (ret != HI_ERR_SUCCESS)
            {
                HI_ERR_CIPHER("Get random number failed, ret = %x\n", ret);
                return HI_ERR_FAILURE;
            }
            pu8Rand[i] = (HI_U8)(u32Rand) & 0xFF;
            i = 0;
        }
    }

    return HI_ERR_SUCCESS;
}

/*PKCS #1: EME-OAEP encoding*/
/*************************************************************
                    +----------+---------+--+-------+
               DB = |  lHash   |    PS   |01|   M   |
                    +----------+---------+--+-------+
                                   |
         +----------+              V
         |   seed   |--> MGF ---> xor
         +----------+              |
               |                   |
      +--+     V                   |
      |00|    xor <----- MGF <-----|
      +--+     |                   |
        |      |                   |
        V      V                   V
      +--+----------+----------------------------+
EM =  |00|maskedSeed|          maskedDB          |
      +--+----------+----------------------------+
       1     hlen              k - hlen- 1          

so: PS_LEN = k - hlen - 1 -  (hlen  + mlen + 1) = k - 2hlen - mlen - 2 > 0
so: mlen < k - 2hlen - 2 
*************************************************************/
static HI_U32 RSA_padding_add_PKCS1_OAEP(HI_UNF_HASH_TYPE_E enHashType,
                                  HI_U32 u32HLen,
                                  HI_U32 u32KeyLen,
                                  HI_U8 *pu8Input,
                                  HI_U32 u32InLen,
                                  HI_U8 *pu8Output,
                                  HI_U32 *pu32OutLen)
{
    HI_U32 ret = HI_ERR_SUCCESS;
    HI_U32 u32DBLen = HI_ERR_SUCCESS;
    HI_U8 *pu8DB = HI_NULL;
    HI_U8 *pu8Seed = HI_NULL;
    const HI_S8 *pu8LHASH = EMPTY_L_SHA1;

    CHECK_RSA_OPEN();

    if ((pu8Input == HI_NULL) || (pu8Output == HI_NULL) || (pu32OutLen == HI_NULL))
    {
        HI_ERR_CIPHER("para is HI_NULL.\n");
        return HI_ERR_INVALID_PARAMETER;
    }

    /*In the v2.1 of PKCS #1, L is the empty string;    */
    /*other uses outside the scope of rsa specifications*/
    if( enHashType == HI_UNF_HASH_TYPE_SHA256)
    {   
        pu8LHASH = EMPTY_L_SHA256;
    }

    if (u32InLen > u32KeyLen - 2 * u32HLen - 2)
    {
        HI_ERR_CIPHER("u32InLen is invalid.\n");
        return HI_ERR_CIPHER_INVALID_PARA;
    }

    *pu32OutLen = 0;
    pu8Output[0] = 0;
    pu8Seed = pu8Output + 1;
    pu8DB = pu8Output + u32HLen + 1;
    u32DBLen = u32KeyLen - u32HLen -1;
    (hi_void)memcpy_s(pu8DB, u32HLen, pu8LHASH, u32HLen);
    (hi_void)memset_s((HI_VOID *)&pu8DB[u32HLen], u32DBLen - u32InLen - u32HLen -1, 0, u32DBLen - u32InLen - u32HLen -1);     /*set PS with 0x00*/
    pu8DB[u32DBLen - u32InLen - 1] = 0x01;                            /*set 0x01 after PS*/
    (hi_void)memcpy_s(&pu8DB[u32DBLen - u32InLen], u32InLen, pu8Input, u32InLen);          /*set M*/
    ret = RSA_GetRandomNumber(pu8Seed, u32HLen);
    if (ret != HI_ERR_SUCCESS)
    {
        HI_ERR_CIPHER("RSA_GetRandomNumber failed, ret = %x\n", ret);
        return HI_ERR_FAILURE;
    }
    ret = RSA_PKCS1_MGF1(enHashType, pu8Seed, u32HLen, pu8DB, u32KeyLen - u32HLen - 1);
    if (ret != HI_ERR_SUCCESS)
    {
        HI_ERR_CIPHER("RSA_PKCS1_MGF1 failed, ret = %x\n", ret);
        return HI_ERR_FAILURE;
    }
    ret = RSA_PKCS1_MGF1(enHashType, pu8DB, u32KeyLen - u32HLen - 1, pu8Seed, u32HLen);
    if (ret != HI_ERR_SUCCESS)
    {
        HI_ERR_CIPHER("RSA_PKCS1_MGF1 failed, ret = %x\n", ret);
        return HI_ERR_FAILURE;
    }
    
    *pu32OutLen = u32KeyLen;
     return HI_ERR_SUCCESS;
}

/*PKCS #1: block type 0,1,2 message padding*/
/*************************************************
EB = 00 || BT || PS || 00 || D

PS_LEN >= 8, mlen < u32KeyLen - 11
*************************************************/
/*

static HI_U32 RSA_padding_add_PKCS1_type(HI_U32 u32KeyLen,
                               HI_U8  u8BT,
                               HI_U8 *pu8Input,
                               HI_U32 u32InLen,
                               HI_U8 *pu8Output,
                               HI_U32 *pu32OutLen)
{
    HI_U32 ret = HI_ERR_SUCCESS;
    HI_U32 u32PLen;
    HI_U8 *pu8EB;

    if (u32InLen > u32KeyLen - 11)
    {
        HI_ERR_CIPHER("u32InLen is invalid.\n");
        return HI_ERR_CIPHER_INVALID_PARA;
    }

    *pu32OutLen =0;
    pu8EB = pu8Output;
    
    *(pu8EB++) = 0;
    *(pu8EB++) = u8BT;                 //Private Key BT (Block Type) 

    // pad out with 0xff data
    u32PLen = u32KeyLen - 3 - u32InLen;
    switch(u8BT)
    {
    case 0x00:
        (hi_void)memset_s((HI_VOID *)pu8EB, u32PLen, 0x00, u32PLen);
        break;
    case 0x01:
        (hi_void)memset_s((HI_VOID *)pu8EB, u32PLen, 0xFF, u32PLen);
        break;
    case 0x02:
        ret = RSA_GetRandomNumber(pu8EB, u32PLen);
        if (ret != HI_ERR_SUCCESS)
        {
            HI_ERR_CIPHER("RSA_GetRandomNumber failed, ret = %x\n", ret);
            return HI_ERR_FAILURE;
        }
        break;
    default:
        HI_ERR_CIPHER("BT(0x%x) is invalid.\n", u8BT);
        return HI_ERR_CIPHER_INVALID_PARA;
    }

    pu8EB += u32PLen;
    *(pu8EB++) = 0x00;
    (hi_void)memcpy_s(pu8EB, u32InLen, pu8Input, u32InLen);
    *pu32OutLen = u32KeyLen;
    return HI_ERR_SUCCESS;
}
*/

HI_U32 HI_MDM_RSA_Init(HI_VOID)
{    
    if (g_RsaInitCounter > 0)
    {
        g_RsaInitCounter++;
        return HI_ERR_SUCCESS;
    }
    else
    {
        g_RsaInitCounter = 1; 
        return DRV_RSA_SemInit();
               
    }
    
}

HI_U32 HI_MDM_RSA_DeInit(HI_VOID)
{
    if (g_RsaInitCounter > 0)
    {
        g_RsaInitCounter--;

        if (g_RsaInitCounter == 0)
        {
            g_RsaInitCounter = -1;
            return DRV_RSA_SemDeInit();
        }

    }

    return HI_ERR_SUCCESS;
}

HI_U32 HI_MDM_RSA_PublicEncrypt(HI_RSA_PUB_ENC_S *pstRsaEnc, 
								  HI_U8 *pInput, HI_U32 ulInLen, 
								  HI_U8 *pOutput, HI_U32 *pOutLen)
{
    HI_U32 ret = HI_ERR_SUCCESS;
    RSA_DATA_S stRsaData;
    HI_U32 u32HLen = 0;
    HI_U32 u32KeyLen;
    HI_UNF_HASH_TYPE_E enHashType;
    HI_U8 *pu8EM = HI_NULL;

    CHECK_RSA_OPEN();

    if ((pstRsaEnc == HI_NULL) || (pInput == HI_NULL) 
        || (pOutput == HI_NULL) || (pOutLen == HI_NULL))
    {
        HI_ERR_CIPHER("para is HI_NULL.\n");
        return HI_ERR_INVALID_PARAMETER;
    }

    if (ulInLen == 0)
    {
        HI_ERR_CIPHER("inlen is 0.\n");
        return HI_ERR_CIPHER_INVALID_PARA;
    }
  
    ret = RSA_GetAttr(pstRsaEnc->enScheme, pstRsaEnc->stPubKey.u16NLen, 
            &u32HLen, &u32KeyLen, &enHashType);
    if (ret != HI_ERR_SUCCESS)
    {
        HI_ERR_CIPHER("RSA attr config error\n");
        return ret;
    }

    if (ulInLen > u32KeyLen)
    {
        HI_ERR_CIPHER("inlen is too large.\n");
        return HI_ERR_CIPHER_INVALID_PARA;
    }

    pu8EM = (HI_U8 *)HI_MDM_Malloc(HI_MOD_ID_DRV_CIPHER, CIPHER_MAX_RSA_KEY_LEN);
    if (HI_NULL == pu8EM)
    {
        return HI_ERR_MALLOC_FAILUE;
    }
    else
    {
        (hi_void)memset_s((HI_VOID *)pu8EM, CIPHER_MAX_RSA_KEY_LEN, 0, CIPHER_MAX_RSA_KEY_LEN);
    }
    
    switch(pstRsaEnc->enScheme)
    {
    case HI_UNF_RSA_ENC_SCHEME_NO_PADDING:
        /*if u32InLen < u32KeyLen, padding 0 before input data*/
        *pOutLen = u32KeyLen;
        (hi_void)memcpy_s(pu8EM + (u32KeyLen - ulInLen), ulInLen, pInput, ulInLen);
        ret = HI_ERR_SUCCESS;
        break;
    case HI_UNF_RSA_ENC_SCHEME_RSAES_OAEP_SHA1:
    case HI_UNF_RSA_ENC_SCHEME_RSAES_OAEP_SHA256:
        ret = RSA_padding_add_PKCS1_OAEP(enHashType, u32HLen, u32KeyLen, pInput, ulInLen, pu8EM, pOutLen);
        break;
    default:
        HI_ERR_CIPHER("RSA padding mode error, mode = 0x%x.\n", pstRsaEnc->enScheme);
        ret = HI_ERR_CIPHER_INVALID_PARA;
        break;
    }

    if (ret != HI_ERR_SUCCESS)
    {
        HI_ERR_CIPHER("RSA padding error, ret = 0x%x.\n", ret);
        HI_MDM_Free(HI_MOD_ID_DRV_CIPHER, pu8EM);
        return ret;
    }

    if(*pOutLen != u32KeyLen)
    {
        HI_ERR_CIPHER("Error, u32OutLen != u32KeyLen.\n");
        HI_MDM_Free(HI_MOD_ID_DRV_CIPHER, pu8EM);
        return HI_ERR_INVALID_PARAMETER;
    }

    stRsaData.pu8N = pstRsaEnc->stPubKey.pu8N;
    stRsaData.pu8K = pstRsaEnc->stPubKey.pu8E;
    stRsaData.u16NLen = pstRsaEnc->stPubKey.u16NLen;
    stRsaData.u16KLen = pstRsaEnc->stPubKey.u16ELen;
    stRsaData.pu8Input = pu8EM;
    stRsaData.pu8Output = pOutput;
    stRsaData.u32DataLen = u32KeyLen;

    ret = HI_DRV_RSA_Calc(&stRsaData);
    HI_MDM_Free(HI_MOD_ID_DRV_CIPHER, pu8EM);
    return ret;
}

static HI_U32 RSA_padding_check_PKCS1_OAEP(HI_UNF_HASH_TYPE_E enHashType,
                                  HI_U32 u32HLen,
                                  HI_U32 u32KeyLen,
                                  HI_U8 *pu8Input, 
                                  HI_U32 u32InLen,
                                  HI_U8 *pu8Output,
                                  HI_U32 *pu32OutLen)
{
    HI_U32 ret = HI_ERR_SUCCESS;
    HI_U32 i;
    const HI_S8 *pu8LHASH = EMPTY_L_SHA1;
    HI_U8 *pu8Seed = HI_NULL;
    HI_U8 *pu8DB = HI_NULL;
    HI_U8 *pu8MaskedDB = HI_NULL;

    CHECK_RSA_OPEN();

    if ((pu8Input == HI_NULL) || (pu8Output == HI_NULL) || (pu32OutLen == HI_NULL))
    {
        HI_ERR_CIPHER("para is HI_NULL.\n");
        return HI_ERR_INVALID_PARAMETER;
    }

    if(enHashType == HI_UNF_HASH_TYPE_SHA256)
    {   
        pu8LHASH = EMPTY_L_SHA256;
    }

    if (u32InLen != u32KeyLen)
    {
        HI_ERR_CIPHER("u32InLen is invalid.\n");
        return HI_ERR_CIPHER_INVALID_PARA;
    }

    if (u32KeyLen < 2 * u32HLen + 2)
    {
        HI_ERR_CIPHER("u32InLen is invalid.\n");
        return HI_ERR_CIPHER_INVALID_PARA;
    }

    if(pu8Input[0] != 0x00)
    {
        HI_ERR_CIPHER("EM[0] != 0.\n");
        return HI_ERR_CIPHER_FAILED_DECRYPT;
    }

    *pu32OutLen = 0;
    pu8MaskedDB= pu8Input + u32HLen + 1;
    pu8Seed = pu8Input + 1;
    pu8DB = pu8Input + u32HLen + 1;
    ret = RSA_PKCS1_MGF1(enHashType, pu8MaskedDB, u32KeyLen - u32HLen - 1, pu8Seed, u32HLen);
    if (ret != HI_ERR_SUCCESS)
    {
        HI_ERR_CIPHER("RSA_PKCS1_MGF1 failed, ret = %x\n", ret);
        return HI_ERR_FAILURE;
    }
    ret = RSA_PKCS1_MGF1(enHashType, pu8Seed, u32HLen, pu8DB, u32KeyLen - u32HLen - 1);
    if (ret != HI_ERR_SUCCESS)
    {
        HI_ERR_CIPHER("RSA_PKCS1_MGF1 failed, ret = %x\n", ret);
        return HI_ERR_FAILURE;
    }
    if((u32HLen <= strlen((const hi_char *)pu8LHASH))&&(memcmp((HI_CONST HI_VOID *)pu8DB, (HI_CONST HI_VOID *)pu8LHASH, u32HLen) != 0))
    {
        HI_ERR_CIPHER("LHASH error.\n");
        return HI_ERR_CIPHER_FAILED_DECRYPT;
    }

    for(i=u32HLen; i < u32KeyLen - u32HLen - 1; i++)
    {
        if((pu8DB[i] == 0x01)) 
        {
            (hi_void)memcpy_s(pu8Output, u32KeyLen - u32HLen - i - 2, pu8DB+i+1, u32KeyLen - u32HLen - i - 2);
            *pu32OutLen = u32KeyLen - u32HLen - i - 2;
            break;
        } 
    }
    if (i >= u32KeyLen - u32HLen - 1)
    {
        HI_ERR_CIPHER("PS error.\n");
        return HI_ERR_CIPHER_FAILED_DECRYPT;
    }

    return HI_ERR_SUCCESS;
}

static HI_U32 RSA_padding_check_PKCS1_V15(HI_U32 u32KeyLen,
                                          HI_U8 *pu8Input, 
                                          HI_U32 u32InLen,
                                          HI_U8 *pu8Output,
                                          HI_U32 *pu32OutLen)
{
    HI_U32 u32Index = 0;

    CHECK_RSA_OPEN();

    if ((pu8Input == HI_NULL) || (pu8Output == HI_NULL) || (pu32OutLen == HI_NULL))
    {
        HI_ERR_CIPHER("para is HI_NULL.\n");
        return HI_ERR_INVALID_PARAMETER;
    }

    if ((u32InLen != u32KeyLen) || (u32KeyLen < 11))
    {
        HI_ERR_CIPHER("u32InLen is invalid.\n");
        return HI_ERR_CIPHER_INVALID_PARA;
    }
    
    if(pu8Input[u32Index] != 0x00)
    {
        HI_ERR_CIPHER("EM[0] != 0x00.\n");
        return HI_ERR_CIPHER_FAILED_DECRYPT;
    }
    u32Index++;
    if(pu8Input[u32Index] != 0x02)
    {
        HI_ERR_CIPHER("EM[1] != 0x02.\n");
        return HI_ERR_CIPHER_FAILED_DECRYPT;
    }
    u32Index++;
    for( ; u32Index < u32KeyLen; u32Index++)
    {
        if( (u32Index >= 10) //The length of PS is large than 8 octets
            && (pu8Input[u32Index] == 0x00)) 
        {
            (hi_void)memcpy_s(pu8Output, u32KeyLen - 1 - u32Index, &pu8Input[u32Index+1], u32KeyLen - 1 - u32Index);
            *pu32OutLen = u32KeyLen - 1 - u32Index;
            break;
        } 
    }
    if (u32Index >= u32KeyLen)
    {
        HI_ERR_CIPHER("PS error.\n");
        return HI_ERR_CIPHER_FAILED_DECRYPT;
    }
  
    return HI_ERR_SUCCESS;
}

static HI_U32 RSA_padding_check_PKCS1_type(HI_U32 u32KeyLen,
                               HI_U8  u8BT,
                               HI_U8 *pu8Input, 
                               HI_U32 u32InLen,
                               HI_U8 *pu8Output,
                               HI_U32 *pu32OutLen)
{
    HI_U8 *pu8EB = HI_NULL;

    if ((pu8Input == HI_NULL) || (pu8Output == HI_NULL) || (pu32OutLen == HI_NULL))
    {
        HI_ERR_CIPHER("para is HI_NULL.\n");
        return HI_ERR_INVALID_PARAMETER;
    }
	
    if (u32InLen != u32KeyLen)
    {
        HI_ERR_CIPHER("u32InLen is invalid.\n");
        return HI_ERR_CIPHER_INVALID_PARA;
    }

    *pu32OutLen = 0x00;
    pu8EB = pu8Input;
    if(*pu8EB != 0x00)
    {
        HI_ERR_CIPHER("EB[0] != 0x00.\n");
        return HI_ERR_CIPHER_FAILED_DECRYPT;
    }
    pu8EB++;
    if(*pu8EB != u8BT)
    {
        HI_ERR_CIPHER("EB[1] != BT(0x%x).\n", u8BT);
        return HI_ERR_CIPHER_FAILED_DECRYPT;
    }
    pu8EB++;
    switch(u8BT)
    {
    case 0x00:
        for( ; pu8EB < pu8Input + u32InLen - 1; pu8EB++)
        {
            if( (*pu8EB == 0x00) && (*(pu8EB+1) != 0)) 
            {
                break;
            } 
        }
        break;
    case 0x01:
        for( ; pu8EB < pu8Input + u32InLen - 1; pu8EB++)
        {
            if(*pu8EB == 0xFF)
            {
                continue;
            }
            else if (*pu8EB == 0x00)
            {
                break;
            }
            else
            {
                pu8EB = pu8Input + u32InLen - 1;
                break;
            }
        }
        break;
    case 0x02:
        for( ; pu8EB < pu8Input + u32InLen - 1; pu8EB++)
        {
            if(*pu8EB == 0x00)
            {
                break; 
            }   
        }
        break;
    default:
        HI_ERR_CIPHER("BT(0x%x) is invalid.\n", u8BT);
        return HI_ERR_CIPHER_INVALID_PARA;
    }

    if(pu8EB >= (pu8Input + u32InLen - 1))
    {
        HI_ERR_CIPHER("PS Error.\n");
        return HI_ERR_CIPHER_FAILED_DECRYPT;
    }

    pu8EB++;
    *pu32OutLen = (HI_U32)pu8Input + u32KeyLen - (HI_U32)pu8EB;
    (hi_void)memcpy_s(pu8Output, (HI_U32)*pu32OutLen, pu8EB, (HI_U32)*pu32OutLen);
    return HI_ERR_SUCCESS;
}
HI_U32 HI_MDM_RSA_PrivateDecrypt(HI_RSA_PRI_ENC_S *pstRsaDec,								  
								   HI_U8 *pInput, HI_U32 ulInLen, 
								   HI_U8 *pOutput, HI_U32 *pOutLen)
{
    HI_U32 ret = HI_ERR_SUCCESS;
    RSA_DATA_S stRsaData;
    HI_U8 *pu8EM = HI_NULL;
    HI_U32 u32HLen;
    HI_U32 u32KeyLen;
    HI_U8 u8BT;
    HI_UNF_HASH_TYPE_E enHashType;

    CHECK_RSA_OPEN();
    
    if ((pstRsaDec == HI_NULL) || (pInput == HI_NULL) 
        || (pOutput == HI_NULL) || (pOutLen == HI_NULL))
    {
        HI_ERR_CIPHER("para is HI_NULL.\n");
        return HI_ERR_INVALID_PARAMETER;
    }

    ret = RSA_GetAttr(pstRsaDec->enScheme, pstRsaDec->stPriKey.u16NLen, &u32HLen, &u32KeyLen, &enHashType);
    if (ret != HI_ERR_SUCCESS)
    {
        HI_ERR_CIPHER("RSA attr config error\n");
        return ret;
    }

    if(ulInLen != u32KeyLen)
    {
        HI_ERR_CIPHER("Error, u32InLen != u32KeyLen.\n");
        return HI_ERR_INVALID_PARAMETER;
    }

    pu8EM = (HI_U8 *)HI_MDM_Malloc(HI_MOD_ID_DRV_CIPHER, CIPHER_MAX_RSA_KEY_LEN);
    if (HI_NULL == pu8EM)
    {
        return HI_ERR_MALLOC_FAILUE;
    }

    stRsaData.pu8N = pstRsaDec->stPriKey.pu8N;
    stRsaData.pu8K = pstRsaDec->stPriKey.pu8D;
    stRsaData.u16NLen = pstRsaDec->stPriKey.u16NLen;
    stRsaData.u16KLen = pstRsaDec->stPriKey.u16DLen;
    stRsaData.pu8Input = pInput;
    stRsaData.pu8Output = pu8EM;
    stRsaData.u32DataLen = u32KeyLen;

    ret = HI_DRV_RSA_Calc(&stRsaData);
    if (ret != HI_ERR_SUCCESS)
    {
        HI_ERR_CIPHER("CMD_CIPHER_CALCRSA failed, ret = %x\n", ret);
        HI_MDM_Free(HI_MOD_ID_DRV_CIPHER, pu8EM);
        return HI_ERR_FAILURE;
    }    
    
    switch(pstRsaDec->enScheme)
    {
    case HI_UNF_RSA_ENC_SCHEME_NO_PADDING:
        *pOutLen = ulInLen;
        (hi_void)memcpy_s(pOutput, u32KeyLen, pu8EM, u32KeyLen);
        ret = HI_ERR_SUCCESS;
        break;
    case HI_UNF_RSA_ENC_SCHEME_BLOCK_TYPE_0:
    case HI_UNF_RSA_ENC_SCHEME_BLOCK_TYPE_1:
        HI_ERR_CIPHER("RSA padding mode error, mode = 0x%x.\n", pstRsaDec->enScheme);
        HI_ERR_CIPHER("For a private key decryption operation, the block type shall be 02.\n");
        ret = HI_ERR_CIPHER_INVALID_PARA;
        break;
    case HI_UNF_RSA_ENC_SCHEME_BLOCK_TYPE_2:
        u8BT = (HI_U8)pstRsaDec->enScheme - HI_UNF_RSA_ENC_SCHEME_BLOCK_TYPE_0;
        ret = RSA_padding_check_PKCS1_type(u32KeyLen, u8BT, pu8EM, ulInLen, pOutput, pOutLen);
        break;
    case HI_UNF_RSA_ENC_SCHEME_RSAES_OAEP_SHA1:
    case HI_UNF_RSA_ENC_SCHEME_RSAES_OAEP_SHA256:
        ret = RSA_padding_check_PKCS1_OAEP(enHashType, u32HLen, u32KeyLen, pu8EM, ulInLen, pOutput, pOutLen);
        break;
    case HI_UNF_RSA_ENC_SCHEME_RSAES_PKCS1_V1_5:
        ret = RSA_padding_check_PKCS1_V15(u32KeyLen, pu8EM, ulInLen, pOutput, pOutLen);
        break;
    default:
        HI_ERR_CIPHER("RSA scheme error, scheme = 0x%x.\n", pstRsaDec->enScheme);
        ret = HI_ERR_CIPHER_INVALID_PARA;
        break;
    }

    HI_MDM_Free(HI_MOD_ID_DRV_CIPHER, pu8EM);
    if (ret != HI_ERR_SUCCESS)
    {
        HI_ERR_CIPHER("RSA padding error, ret = 0x%x.\n", ret);       
    }

    return ret;
}
HI_U32 HI_MDM_RSA_PrivateEncrypt(HI_RSA_PRI_ENC_S *pstRsaEnc, 
							 	   HI_U8 *pInput, HI_U32 ulInLen, 
								   HI_U8 *pOutput, HI_U32 *pOutLen)
{
    HI_U32 ret = HI_ERR_SUCCESS;
    RSA_DATA_S stRsaData;
    HI_U32 u32HLen;
    HI_U32 u32KeyLen;
    HI_UNF_HASH_TYPE_E enHashType;
    HI_U8 *pu8EM = HI_NULL;
    
    CHECK_RSA_OPEN();

    if ((pstRsaEnc == HI_NULL) || (pInput == HI_NULL) || (pOutput == HI_NULL) || (pOutLen == HI_NULL))
    {
        HI_ERR_CIPHER("para is HI_NULL.\n");
        return HI_ERR_INVALID_PARAMETER;
    }

    if (ulInLen == 0)
    {
        HI_ERR_CIPHER("inlen is 0.\n");
        return HI_ERR_CIPHER_INVALID_PARA;
    }
    
    ret = RSA_GetAttr(pstRsaEnc->enScheme, pstRsaEnc->stPriKey.u16NLen, 
            &u32HLen, &u32KeyLen, &enHashType);
    if (ret != HI_ERR_SUCCESS)
    {
        HI_ERR_CIPHER("RSA attr config error\n");
        return ret;
    }

    if (ulInLen > u32KeyLen)
    {
        HI_ERR_CIPHER("inlen is too large.\n");
        return HI_ERR_CIPHER_INVALID_PARA;
    }
    pu8EM = (HI_U8 *)HI_MDM_Malloc(HI_MOD_ID_DRV_CIPHER, CIPHER_MAX_RSA_KEY_LEN);
    if (HI_NULL == pu8EM)
    {
        return HI_ERR_MALLOC_FAILUE;
    }

    (hi_void)memset_s((HI_VOID *)pu8EM, CIPHER_MAX_RSA_KEY_LEN, 0, CIPHER_MAX_RSA_KEY_LEN);

    switch(pstRsaEnc->enScheme)
    {
    case HI_UNF_RSA_ENC_SCHEME_NO_PADDING:
        /*if u32InLen < u32KeyLen, padding 0 before input data*/
        *pOutLen = u32KeyLen;
        (hi_void)memcpy_s(pu8EM + (u32KeyLen - ulInLen), ulInLen, pInput, ulInLen);
        ret = HI_ERR_SUCCESS;
        break;
	/*	
    case HI_UNF_RSA_ENC_SCHEME_BLOCK_TYPE_0:
    case HI_UNF_RSA_ENC_SCHEME_BLOCK_TYPE_1:
        u8BT = (HI_U8)pstRsaEnc->enScheme - HI_UNF_RSA_ENC_SCHEME_BLOCK_TYPE_0;

                    【屏蔽人】z00193780
                    【审核人】c00233580
                    【代码分析】驱动提供接口，由客户决定是否使用。
                    【功能影响】不影响正常业务，屏蔽告警


    	ret = RSA_padding_add_PKCS1_type(u32KeyLen, u8BT, pInput, ulInLen, pu8EM, pOutLen);
    	break;
   */ 
    case HI_UNF_RSA_ENC_SCHEME_BLOCK_TYPE_2:
        HI_ERR_CIPHER("RSA padding mode error, mode = 0x%x.\n", pstRsaEnc->enScheme);
        HI_ERR_CIPHER("For a private- key encryption operation, the block type shall be 00 or 01.\n");
        ret = HI_ERR_CIPHER_INVALID_PARA;
        break;
    default:
        HI_ERR_CIPHER("RSA padding mode error, mode = 0x%x.\n", pstRsaEnc->enScheme);
        ret = HI_ERR_CIPHER_INVALID_PARA;
        break;
    }

    if (ret != HI_ERR_SUCCESS)
    {
        HI_ERR_CIPHER("RSA padding error, ret = 0x%x.\n", ret);
        HI_MDM_Free(HI_MOD_ID_DRV_CIPHER, pu8EM);
        return ret;
    }

    if(*pOutLen != u32KeyLen)
    {
        HI_ERR_CIPHER("Error, u32OutLen != u32KeyLen.\n");
        HI_MDM_Free(HI_MOD_ID_DRV_CIPHER, pu8EM);
        return HI_ERR_INVALID_PARAMETER;
    }

    stRsaData.pu8N = pstRsaEnc->stPriKey.pu8N;
    stRsaData.pu8K = pstRsaEnc->stPriKey.pu8D;
    stRsaData.u16NLen = pstRsaEnc->stPriKey.u16NLen;
    stRsaData.u16KLen = pstRsaEnc->stPriKey.u16DLen;
    stRsaData.pu8Input = pu8EM;
    stRsaData.pu8Output = pOutput;
    stRsaData.u32DataLen = u32KeyLen;   
    ret = HI_DRV_RSA_Calc(&stRsaData);
    HI_MDM_Free(HI_MOD_ID_DRV_CIPHER, pu8EM);
    return ret;
}

HI_U32 HI_MDM_RSA_PublicDecrypt(HI_RSA_PUB_ENC_S *pstRsaDec,
							   HI_U8 *pInput, HI_U32 ulInLen,
							   HI_U8 *pOutput, HI_U32 *pOutLen)
{
    HI_U32 ret = HI_ERR_SUCCESS;
    RSA_DATA_S stRsaData;
    HI_U8 *pu8EM = HI_NULL;
    HI_U32 u32HLen;
    HI_U32 u32KeyLen;
    HI_U8  u8BT;
    HI_UNF_HASH_TYPE_E enHashType;

    CHECK_RSA_OPEN();

    if ((pstRsaDec == HI_NULL) || (pInput == HI_NULL) 
        || (pOutput == HI_NULL) || (pOutLen == HI_NULL))
    {
        HI_ERR_CIPHER("para is HI_NULL.\n");
        return HI_ERR_INVALID_PARAMETER;
    }

    ret = RSA_GetAttr(pstRsaDec->enScheme, pstRsaDec->stPubKey.u16NLen, 
            &u32HLen, &u32KeyLen, &enHashType);
    if (ret != HI_ERR_SUCCESS)
    {
        HI_ERR_CIPHER("RSA attr config error\n");
        return ret;
    }

    if(ulInLen != u32KeyLen)
    {
        HI_ERR_CIPHER("Error, u32InLen != u32KeyLen.\n");
        return HI_ERR_INVALID_PARAMETER;
    }

    pu8EM = (HI_U8 *)HI_MDM_Malloc(HI_MOD_ID_DRV_CIPHER, CIPHER_MAX_RSA_KEY_LEN);
    if (HI_NULL == pu8EM)
    {
        return HI_ERR_MALLOC_FAILUE;
    }
    
    stRsaData.pu8N = pstRsaDec->stPubKey.pu8N;
    stRsaData.pu8K = pstRsaDec->stPubKey.pu8E;
    stRsaData.u16NLen = pstRsaDec->stPubKey.u16NLen;
    stRsaData.u16KLen = pstRsaDec->stPubKey.u16ELen;
    stRsaData.pu8Input = pInput;
    stRsaData.pu8Output = pu8EM;
    stRsaData.u32DataLen = u32KeyLen;

    ret = HI_DRV_RSA_Calc(&stRsaData);
    if (ret != HI_ERR_SUCCESS)
    {
        HI_ERR_CIPHER("CMD_CIPHER_CALCRSA failed, ret = %x\n", ret);
        HI_MDM_Free(HI_MOD_ID_DRV_CIPHER, pu8EM);
        return HI_ERR_FAILURE;
    }    
    
    switch(pstRsaDec->enScheme)
    {
    case HI_UNF_RSA_ENC_SCHEME_NO_PADDING:
        *pOutLen = ulInLen;
        (hi_void)memcpy_s(pOutput, u32KeyLen, pu8EM, u32KeyLen);
        ret = HI_ERR_SUCCESS;
        break;
    case HI_UNF_RSA_ENC_SCHEME_BLOCK_TYPE_0:
    case HI_UNF_RSA_ENC_SCHEME_BLOCK_TYPE_1:
        u8BT = ((HI_U8)pstRsaDec->enScheme - HI_UNF_RSA_ENC_SCHEME_BLOCK_TYPE_0);
        ret = RSA_padding_check_PKCS1_type(u32KeyLen, u8BT, pu8EM, ulInLen, pOutput, pOutLen);
        break;
    case HI_UNF_RSA_ENC_SCHEME_BLOCK_TYPE_2:
        HI_ERR_CIPHER("RSA padding mode error, mode = 0x%x.\n", pstRsaDec->enScheme);
        HI_ERR_CIPHER("For a public key decryption operation, the block type shall be 00 or 01.\n");
        ret = HI_ERR_CIPHER_INVALID_PARA;
        break;
    default:
        HI_ERR_CIPHER("RSA scheme error, scheme = 0x%x.\n", pstRsaDec->enScheme);
        ret = HI_ERR_CIPHER_INVALID_PARA;
        break;
    }

    HI_MDM_Free(HI_MOD_ID_DRV_CIPHER, pu8EM);
    if (ret != HI_ERR_SUCCESS)
    {
        HI_ERR_CIPHER("RSA padding error, ret = 0x%x.\n", ret);
    }
    
    return ret;
}

