/******************************************************************************

  Copyright (C), 2011-2014, Hisilicon Tech. Co., Ltd.

 ******************************************************************************
  File Name     : drv_cipher.c
  Version       : Initial Draft
  Author        : Hisilicon hisecurity team
  Created       :
  Last Modified :
  Description   :
  Function List :
  History       :
******************************************************************************/
#include "hal_cipher.h"
#include "drv_cipher_define.h"
#include <hi_sal.h>
#include <sal_inf.h>
#include <os_vx_sal_api.h>
#include "hi_drv_cipher.h"
#include "drv_cipher.h"
//#include "esal_pr_int_defs.h"
#include "hal_key_ctrl.h"
#include <sal_inf_m.h>
#include <hi_low_power.h>
#include <hi_mdm_clock.h>

#define DEFAULT_CIPHER_KEY_AERA EFUSE_AERA1

typedef struct
{
	HI_U8* pu8StartVirAddr;
	HI_U32 u32StartPhyAddr; //mmz or mmu physical addr//zn no use
	HI_U32 u32Size;
} CIPHER_IV_BUFFER_S;

/*
-----------------------------------------------------------
0 | input buf list Node(16Byte) | ...  * CIPHER_MAX_LIST_NUM  | = 16*CIPHER_MAX_LIST_NUM
-----------------------------------------------------------
  | output buf list Node(16Byte)| ...  * CIPHER_MAX_LIST_NUM  |
-----------------------------------------------------------
  | IV (16Byte)                 | ...  * CIPHER_MAX_LIST_NUM  |
-----------------------------------------------------------
... * 7 Channels
*/

typedef struct hiCIPHER_PKGN_MNG_S
{
    HI_U32              u32TotalPkg;
    HI_U32              u32CurrentPtr;
    HI_U32              u32BusyCnt;
    HI_U32              u32FreeCnt;
} CIPHER_PKGN_MNG_S;

typedef struct hiCIPHER_PKG1_MNG_S
{
    HI_U32              au32Data[4];
} CIPHER_PKG1_MNG_S;

typedef union hiCIPHER_DATA_MNG_U
{
    CIPHER_PKGN_MNG_S   stPkgNMng;
    CIPHER_PKG1_MNG_S   stPkg1Mng;
}CIPHER_DATA_MNG_U;

typedef struct hiCIPHER_CHAN_S
{
    HI_U32                  chnId;
    CI_BUF_LIST_ENTRY_S     *pstInBuf;
    CI_BUF_LIST_ENTRY_S     *pstOutBuf;
    CIPHER_IV_BUFFER_S      astCipherIVValue[CIPHER_MAX_LIST_NUM];
    HI_U32                  au32WitchSoftChn[CIPHER_MAX_LIST_NUM];
    HI_U32                  au32CallBackArg[CIPHER_MAX_LIST_NUM];
    HI_BOOL                 bNeedCallback[CIPHER_MAX_LIST_NUM];
    CIPHER_DATA_MNG_U       unInData;
    CIPHER_DATA_MNG_U       unOutData;
} CIPHER_CHAN_S;

typedef struct hiCIPHER_SOFTCHAN_S
{
    HI_BOOL               bOpen;
    HI_BOOL               bIVChange;
    HI_BOOL               bKeyChange;
    HI_BOOL               bDecrypt;       /* hi_false: encrypt */
    HI_U32                u32HardWareChn;
    HI_CIPHER_CTRL_S      stCtrl;
    HI_U32                u32LastPkg;     /* save which pkg's IV we should use for next pkg */   
    HI_U32                u32PrivateData;
    funcCipherCallback    pfnCallBack;
} CIPHER_SOFTCHAN_S;

typedef struct hiCIPHER_COMM_S
{
	HI_U8 *pBufOriginal;
    HI_U8 *pBuf;
    HI_U32 bufSize;
} CIPHER_COMM_S;

typedef struct hiCIPHER_OSR_CHN_S
{
    HI_BOOL g_bSoftChnOpen;                 /* mark soft channel open or not*/
    HI_BOOL g_bDataDone;                    /* mark the data done or not */
    HI_U16 usRsv;
    struct file *pWichFile;                 /* which file need to operate */

    HI_UNF_CIPHER_DATA_S *pstDataPkg;
    HI_U32                u32DataPkgNum;
}CIPHER_OSR_CHN_S;
static CIPHER_COMM_S g_stCipherComm;// 内存
static CIPHER_CHAN_S g_stCipherChans[CIPHER_CHAN_NUM];// 驱动接口通道硬件信息上下文
static CIPHER_SOFTCHAN_S g_stCipherSoftChans[CIPHER_SOFT_CHAN_NUM];// 驱动接口通道软件信息上下文
static CIPHER_OSR_CHN_S g_stCipherOsrChn[CIPHER_SOFT_CHAN_NUM];// 驱动接口控制通道软件信息上下文
HI_U32 g_stCipherSemHd = 0;
HI_U32 g_stHashSemHd = 0;
HI_U32 g_stRsaSemHd = 0;

#define CIPHER_CHECK_HANDLE_RETURN_ERR(ChanId)\
    do \
    { \
        if (ChanId >= CIPHER_SOFT_CHAN_NUM) \
        { \
            HI_ERR_CIPHER("chan %d is too large, max: %d\r\n", ChanId, CIPHER_SOFT_CHAN_NUM); \
            return HI_ERR_INVALID_PARAMETER; \
        } \
        if (HI_FALSE == g_stCipherOsrChn[ChanId].g_bSoftChnOpen) \
        { \
            HI_ERR_CIPHER("chan %d is not open\r\n", ChanId); \
            return HI_ERR_CIPHER_INVALID_HANDLE; \
        } \
    } while (0)

#define DRV_CIPHER_LOCK()  	     (HI_VOID)HI_MDM_SEM_Wait(g_stCipherSemHd, HI_SYS_WAIT_FOREVER)
#define DRV_CIPHER_UNLOCK()  	 (HI_VOID)HI_MDM_SEM_Signal(g_stCipherSemHd)
#define DRV_HASH_LOCK()  	     (HI_VOID)HI_MDM_SEM_Wait(g_stHashSemHd, HI_SYS_WAIT_FOREVER)
#define DRV_HASH_UNLOCK()  	     (HI_VOID)HI_MDM_SEM_Signal(g_stHashSemHd)
#define DRV_RSA_LOCK()  	     (HI_VOID)HI_MDM_SEM_Wait(g_stRsaSemHd, HI_SYS_WAIT_FOREVER)
#define DRV_RSA_UNLOCK()  	     (HI_VOID)HI_MDM_SEM_Signal(g_stRsaSemHd)


HI_PRV HI_U32 DRV_CIPHER_Init(HI_VOID);
HI_PRV HI_U32 DRV_CIPHER_DeInit(HI_VOID);
HI_PRV HI_VOID DRV_CIPHER_OpenChn(HI_U32 softChnId);
HI_PRV HI_U32 DRV_CIPHER_CloseChn(HI_U32 softChnId);
HI_PRV HI_U32 DRV_CIPHER_CreatTask(HI_U32 softChnId, HI_DRV_CIPHER_TASK_S *pTask, HI_U32 *pKey, HI_U32 *pIV);
HI_PRV HI_U32 DRV_CIPHER_GetHandleConfig(HI_HANDLE pCiHandle, HI_CIPHER_CTRL_S *pstCipherConfig);
HI_PRV HI_U32 DRV_CIPHER_CreatMultiPkgTask(HI_U32 softChnId, HI_DRV_CIPHER_DATA_INFO_S *pBuf2Process, HI_U32 pkgNum, HI_BOOL isMultiIV, HI_U32 callBackArg);
HI_PRV HI_U32 DRV_CIPHER_ConfigChn(HI_HANDLE pHandle,  HI_CIPHER_CTRL_S *pConfig);
HI_PRV HI_U32 DRV_CIPHER_DestroyHandle(HI_HANDLE hCipherchn);
HI_PRV HI_U32 DRV_RANDOM_GetNumber(HI_IN HI_U32 u32TimeOutUs, HI_OUT HI_U32 *pulRng);


HI_PRV HI_HANDLE HI_HANDLE_MAKEHANDLE(HI_U32 softChnId)
{
    return g_stCipherOsrChn[softChnId].pstDataPkg;
}

HI_PRV HI_U32 HI_HANDLE_GET_CHNID(HI_HANDLE pHandle)
{
    HI_U32 i = 0;
    if (HI_NULL == pHandle)
    {
        return HI_ERR_FAILURE;
    }
        
    for (i = 0; i < CIPHER_SOFT_CHAN_NUM; i++)
    {
        if (g_stCipherOsrChn[i].pstDataPkg == (HI_UNF_CIPHER_DATA_S *)(*(HI_U32 *)pHandle))
        {
            return i;
        }
    }

    return HI_ERR_FAILURE; 
}
HI_PRV HI_U32 DRV_CIPHER_EvtInit(HI_VOID)
{
    HI_U32 event_val = 0;
    HI_U32 ret = HI_ERR_FAILURE;

    ret = HI_SYS_WaitEvt(EN_SAL_EVENT_CIPHER, HI_FALSE, &event_val,0);
    return ret;
}

HI_PRV HI_U32 DRV_CIPHER_SemInit(HI_VOID)
{
    return HI_MDM_SEM_BCreate(&g_stCipherSemHd,"cipher",HI_SEM_ONE);
}


HI_PRV HI_U32 DRV_CIPHER_SemDeInit(HI_VOID)
{
    return HI_MDM_SEM_Delete(g_stCipherSemHd);
}

HI_U32 DRV_HASH_SemInit(HI_VOID)
{
    return HI_MDM_SEM_BCreate(&g_stHashSemHd,"cipher",HI_SEM_ONE);
}

HI_U32 DRV_HASH_SemDeInit(HI_VOID)
{
    return HI_MDM_SEM_Delete(g_stHashSemHd);
}

HI_U32 DRV_RSA_SemInit(HI_VOID)
{
    return HI_MDM_SEM_BCreate(&g_stRsaSemHd,"cipher",HI_SEM_ONE);
}

HI_U32 DRV_RSA_SemDeInit(HI_VOID)
{
    return HI_MDM_SEM_Delete(g_stRsaSemHd);
}

HI_PRV HI_VOID DRV_CIPHER_UserCommCallBack(HI_U32 arg)
{
    g_stCipherOsrChn[arg].g_bDataDone = HI_TRUE;
    HI_SYS_SetEvt(EN_SAL_EVENT_CIPHER, HI_FALSE, (1 << arg));
}

HI_PRV HI_U32 DRV_CipherInitHardWareChn(HI_U32 chnId )
{
    CIPHER_CHAN_S *pChan = &g_stCipherChans[chnId];

    HAL_Cipher_SetInBufNum(chnId, CIPHER_MAX_LIST_NUM);
    HAL_Cipher_SetInBufCnt(chnId, 0);
    HAL_Cipher_SetOutBufNum(chnId, CIPHER_MAX_LIST_NUM);
    HAL_Cipher_SetOutBufCnt(chnId, 0);
    HAL_Cipher_SetAGEThreshold(chnId, CIPHER_INT_TYPE_OUT, 0);
    HAL_Cipher_SetAGEThreshold(chnId, CIPHER_INT_TYPE_IN, 0);
    HAL_Cipher_DisableInt(chnId, CIPHER_INT_TYPE_OUT | CIPHER_INT_TYPE_IN);

    /*if cipher not be reset, the current ptr may not be zero*/
    HAL_Cipher_GetSrcLstRaddr(chnId, &pChan->unInData.stPkgNMng.u32CurrentPtr);
    HAL_Cipher_GetDestLstRaddr(chnId, &pChan->unOutData.stPkgNMng.u32CurrentPtr);
    return HI_ERR_SUCCESS;
}

HI_PRV HI_U32 DRV_CipherDeInitHardWareChn(HI_U32 chnId)
{
    HAL_Cipher_DisableInt(chnId, CIPHER_INT_TYPE_OUT | CIPHER_INT_TYPE_IN);
    return HI_ERR_SUCCESS;
}
HI_PRV HI_VOID DRV_CIPHER_OpenChn(HI_U32 softChnId)
{
    CIPHER_SOFTCHAN_S *pSoftChan = &g_stCipherSoftChans[softChnId];
    CIPHER_CHAN_S *pChan;
    
    pSoftChan->u32HardWareChn = softChnId;
    pChan = &g_stCipherChans[pSoftChan->u32HardWareChn];
    HAL_Cipher_SetIntThreshold(pChan->chnId, CIPHER_INT_TYPE_OUT, CIPHER_DEFAULT_INT_NUM);
    HAL_Cipher_EnableInt(pChan->chnId, CIPHER_INT_TYPE_OUT);
    pSoftChan->bOpen = HI_TRUE;
}

HI_PRV HI_U32 DRV_CIPHER_CloseChn(HI_U32 softChnId)
{
    CIPHER_SOFTCHAN_S *pSoftChan;

    pSoftChan = &g_stCipherSoftChans[softChnId];
    pSoftChan->bOpen = HI_FALSE;
    HAL_Cipher_ClearCbcVerifyFlag(); //clear the irdeto CBC verify flag anyway
    return HI_ERR_SUCCESS;
}
#ifdef TEST_EFUSE_4_KEY
HI_EXTERN HI_U32 g_cipher_chan1_hw_key_times;
#endif
HI_PRV HI_U32 DRV_CIPHER_ConfigChn(HI_HANDLE pHandle,  HI_CIPHER_CTRL_S *pConfig)
{
    HI_U32 ret = HI_ERR_FAILURE;
    HI_BOOL bDecrypt = HI_FALSE;
    HI_U32 hardWareChn;
    HI_BOOL bIVSet;
    CIPHER_CHAN_S *pChan;
    CIPHER_SOFTCHAN_S *pSoftChan;
    HI_U32 softChnId = 0;
    
    softChnId = HI_HANDLE_GET_CHNID(pHandle);
    CIPHER_CHECK_HANDLE_RETURN_ERR(softChnId);

    if((pConfig->stModeCtrl.enAlg >= HI_UNF_CIPHER_ALG_MAX)
       ||(pConfig->stModeCtrl.enWorkMode >= HI_UNF_CIPHER_WORK_MODE_MAX)
       ||(pConfig->stModeCtrl.enBitWidth >= HI_UNF_CIPHER_BIT_WIDTH_MAX)
       ||(pConfig->stModeCtrl.enKeyLen >= HI_UNF_CIPHER_KEY_MAX))
    {
        HI_ERR_CIPHER("Cipher config failed!\r\n");
        HI_ERR_CIPHER("alg-%d, mode-%d, width-%d, keylen-%d, by_ca-%d,\r\n",
        pConfig->stModeCtrl.enAlg, pConfig->stModeCtrl.enWorkMode, pConfig->stModeCtrl.enBitWidth, 
        pConfig->stModeCtrl.enKeyLen, pConfig->bKeyByCA);
        return HI_ERR_INVALID_PARAMETER;
    }
    
    pSoftChan = &g_stCipherSoftChans[softChnId];
    hardWareChn = pSoftChan->u32HardWareChn;
    pChan = &g_stCipherChans[pSoftChan->u32HardWareChn];
    pSoftChan->pfnCallBack = DRV_CIPHER_UserCommCallBack;
    bIVSet = pConfig->bIVChangeFlag;

    ret = HAL_Cipher_Config(pChan->chnId, bDecrypt, bIVSet, pConfig);
    if (HI_ERR_SUCCESS != ret)
    {
        HI_ERR_CIPHER("Cipher config failed!\r\n");
        return ret;
    }
    
    pSoftChan->bIVChange = bIVSet;
    pSoftChan->bKeyChange = HI_TRUE;
    (hi_void)memcpy_s(&(pSoftChan->stCtrl), sizeof(HI_CIPHER_CTRL_S), pConfig, sizeof(HI_CIPHER_CTRL_S));

    /* set Key */
    if (pSoftChan->bKeyChange)
    {
        if (HI_TRUE == pConfig->bKeyByCA)
		{
			/* Used for copy protection mode */
#ifdef TEST_EFUSE_4_KEY
			ret = HAL_EFUSE_LoadCipher((HI_EFUSE_KEY_IDX_E)g_cipher_chan1_hw_key_times, hardWareChn);
#else
            ret = HAL_EFUSE_LoadCipher(DEFAULT_CIPHER_KEY_AERA,hardWareChn);
#endif
            if (HI_ERR_SUCCESS != ret)
            {
                HI_ERR_CIPHER("Load hard key failed!chn:%d,ret:%x\r\n", hardWareChn,ret);
                return ret;
            }
        }
		else
		{
			ret = HAL_Cipher_SetSoftwareKey(hardWareChn, &(pSoftChan->stCtrl));
			if (HI_ERR_SUCCESS != ret)
			{
				return ret;
			}
		}
		pSoftChan->bKeyChange = HI_FALSE;
	}

    return ret;
}

STATIC HI_CONST HI_CIPHER_MODE_CTRL_S s_stCipherAesCfg[] = 
{  
    // AES
    // 由芯片密钥管理模块配置密钥(bKeyByCA = 1)时密钥长度只支持128位
    {HI_UNF_CIPHER_ALG_AES,HI_UNF_CIPHER_WORK_MODE_ECB,HI_UNF_CIPHER_BIT_WIDTH_128BIT,HI_UNF_CIPHER_KEY_AES_128BIT},
    {HI_UNF_CIPHER_ALG_AES,HI_UNF_CIPHER_WORK_MODE_CBC,HI_UNF_CIPHER_BIT_WIDTH_128BIT,HI_UNF_CIPHER_KEY_AES_128BIT},
    {HI_UNF_CIPHER_ALG_AES,HI_UNF_CIPHER_WORK_MODE_CFB,HI_UNF_CIPHER_BIT_WIDTH_1BIT,  HI_UNF_CIPHER_KEY_AES_128BIT},
    {HI_UNF_CIPHER_ALG_AES,HI_UNF_CIPHER_WORK_MODE_CFB,HI_UNF_CIPHER_BIT_WIDTH_8BIT,  HI_UNF_CIPHER_KEY_AES_128BIT},
    {HI_UNF_CIPHER_ALG_AES,HI_UNF_CIPHER_WORK_MODE_CFB,HI_UNF_CIPHER_BIT_WIDTH_128BIT,HI_UNF_CIPHER_KEY_AES_128BIT},
    {HI_UNF_CIPHER_ALG_AES,HI_UNF_CIPHER_WORK_MODE_OFB,HI_UNF_CIPHER_BIT_WIDTH_128BIT,HI_UNF_CIPHER_KEY_AES_128BIT},
        
    {HI_UNF_CIPHER_ALG_AES,HI_UNF_CIPHER_WORK_MODE_ECB,HI_UNF_CIPHER_BIT_WIDTH_128BIT,HI_UNF_CIPHER_KEY_AES_192BIT},
    {HI_UNF_CIPHER_ALG_AES,HI_UNF_CIPHER_WORK_MODE_CBC,HI_UNF_CIPHER_BIT_WIDTH_128BIT,HI_UNF_CIPHER_KEY_AES_192BIT},
    {HI_UNF_CIPHER_ALG_AES,HI_UNF_CIPHER_WORK_MODE_CFB,HI_UNF_CIPHER_BIT_WIDTH_1BIT,  HI_UNF_CIPHER_KEY_AES_192BIT},
    {HI_UNF_CIPHER_ALG_AES,HI_UNF_CIPHER_WORK_MODE_CFB,HI_UNF_CIPHER_BIT_WIDTH_8BIT,  HI_UNF_CIPHER_KEY_AES_192BIT},
    {HI_UNF_CIPHER_ALG_AES,HI_UNF_CIPHER_WORK_MODE_CFB,HI_UNF_CIPHER_BIT_WIDTH_128BIT,HI_UNF_CIPHER_KEY_AES_192BIT},
    {HI_UNF_CIPHER_ALG_AES,HI_UNF_CIPHER_WORK_MODE_OFB,HI_UNF_CIPHER_BIT_WIDTH_128BIT,HI_UNF_CIPHER_KEY_AES_192BIT},
    
    {HI_UNF_CIPHER_ALG_AES,HI_UNF_CIPHER_WORK_MODE_ECB,HI_UNF_CIPHER_BIT_WIDTH_128BIT,HI_UNF_CIPHER_KEY_AES_256BIT},
    {HI_UNF_CIPHER_ALG_AES,HI_UNF_CIPHER_WORK_MODE_CBC,HI_UNF_CIPHER_BIT_WIDTH_128BIT,HI_UNF_CIPHER_KEY_AES_256BIT},
    {HI_UNF_CIPHER_ALG_AES,HI_UNF_CIPHER_WORK_MODE_CFB,HI_UNF_CIPHER_BIT_WIDTH_1BIT,  HI_UNF_CIPHER_KEY_AES_256BIT},
    {HI_UNF_CIPHER_ALG_AES,HI_UNF_CIPHER_WORK_MODE_CFB,HI_UNF_CIPHER_BIT_WIDTH_8BIT,  HI_UNF_CIPHER_KEY_AES_256BIT},
    {HI_UNF_CIPHER_ALG_AES,HI_UNF_CIPHER_WORK_MODE_CFB,HI_UNF_CIPHER_BIT_WIDTH_128BIT,HI_UNF_CIPHER_KEY_AES_256BIT},
    {HI_UNF_CIPHER_ALG_AES,HI_UNF_CIPHER_WORK_MODE_OFB,HI_UNF_CIPHER_BIT_WIDTH_128BIT,HI_UNF_CIPHER_KEY_AES_256BIT},

};
STATIC HI_CONST HI_CIPHER_MODE_CTRL_S s_stCipher3DesCfg[] = 
{
    // 3DES
    {HI_UNF_CIPHER_ALG_3DES,HI_UNF_CIPHER_WORK_MODE_ECB,HI_UNF_CIPHER_BIT_WIDTH_64BIT,HI_UNF_CIPHER_KEY_DES_2KEY},
    {HI_UNF_CIPHER_ALG_3DES,HI_UNF_CIPHER_WORK_MODE_CBC,HI_UNF_CIPHER_BIT_WIDTH_64BIT,HI_UNF_CIPHER_KEY_DES_2KEY},
    {HI_UNF_CIPHER_ALG_3DES,HI_UNF_CIPHER_WORK_MODE_CFB,HI_UNF_CIPHER_BIT_WIDTH_1BIT, HI_UNF_CIPHER_KEY_DES_2KEY},
    {HI_UNF_CIPHER_ALG_3DES,HI_UNF_CIPHER_WORK_MODE_CFB,HI_UNF_CIPHER_BIT_WIDTH_8BIT, HI_UNF_CIPHER_KEY_DES_2KEY},
    {HI_UNF_CIPHER_ALG_3DES,HI_UNF_CIPHER_WORK_MODE_CFB,HI_UNF_CIPHER_BIT_WIDTH_64BIT,HI_UNF_CIPHER_KEY_DES_2KEY},
    {HI_UNF_CIPHER_ALG_3DES,HI_UNF_CIPHER_WORK_MODE_OFB,HI_UNF_CIPHER_BIT_WIDTH_1BIT, HI_UNF_CIPHER_KEY_DES_2KEY},
    {HI_UNF_CIPHER_ALG_3DES,HI_UNF_CIPHER_WORK_MODE_OFB,HI_UNF_CIPHER_BIT_WIDTH_8BIT, HI_UNF_CIPHER_KEY_DES_2KEY},
    {HI_UNF_CIPHER_ALG_3DES,HI_UNF_CIPHER_WORK_MODE_OFB,HI_UNF_CIPHER_BIT_WIDTH_64BIT,HI_UNF_CIPHER_KEY_DES_2KEY},

    {HI_UNF_CIPHER_ALG_3DES,HI_UNF_CIPHER_WORK_MODE_ECB,HI_UNF_CIPHER_BIT_WIDTH_64BIT,HI_UNF_CIPHER_KEY_DES_3KEY},
    {HI_UNF_CIPHER_ALG_3DES,HI_UNF_CIPHER_WORK_MODE_CBC,HI_UNF_CIPHER_BIT_WIDTH_64BIT,HI_UNF_CIPHER_KEY_DES_3KEY},
    {HI_UNF_CIPHER_ALG_3DES,HI_UNF_CIPHER_WORK_MODE_CFB,HI_UNF_CIPHER_BIT_WIDTH_1BIT, HI_UNF_CIPHER_KEY_DES_3KEY},
    {HI_UNF_CIPHER_ALG_3DES,HI_UNF_CIPHER_WORK_MODE_CFB,HI_UNF_CIPHER_BIT_WIDTH_8BIT, HI_UNF_CIPHER_KEY_DES_3KEY},
    {HI_UNF_CIPHER_ALG_3DES,HI_UNF_CIPHER_WORK_MODE_CFB,HI_UNF_CIPHER_BIT_WIDTH_64BIT,HI_UNF_CIPHER_KEY_DES_3KEY},
    {HI_UNF_CIPHER_ALG_3DES,HI_UNF_CIPHER_WORK_MODE_OFB,HI_UNF_CIPHER_BIT_WIDTH_1BIT, HI_UNF_CIPHER_KEY_DES_3KEY},
    {HI_UNF_CIPHER_ALG_3DES,HI_UNF_CIPHER_WORK_MODE_OFB,HI_UNF_CIPHER_BIT_WIDTH_8BIT, HI_UNF_CIPHER_KEY_DES_3KEY},
    {HI_UNF_CIPHER_ALG_3DES,HI_UNF_CIPHER_WORK_MODE_OFB,HI_UNF_CIPHER_BIT_WIDTH_64BIT,HI_UNF_CIPHER_KEY_DES_3KEY},
};

HI_U32 DRV_CipherCheckAes(HI_CIPHER_MODE_CTRL_S* stCtrl)
{
    HI_S32 slRet;
    HI_U8 i;

    for (i=0; i<sizeof(s_stCipherAesCfg)/sizeof(HI_CIPHER_MODE_CTRL_S);i++)
    {
        slRet = memcmp(&(s_stCipherAesCfg[i]),stCtrl,sizeof(HI_CIPHER_MODE_CTRL_S));
        if (HI_ERR_SUCCESS != slRet)
        {
            continue;
        }
        return HI_ERR_SUCCESS;
    }
    return HI_ERR_INVALID_PARAMETER;
}
STATIC HI_U32 DRV_CipherCheck3Des(HI_CIPHER_MODE_CTRL_S* ctrl)
{
    HI_S32 slRet;
    HI_U8 i;
    
    for (i=0; i<sizeof(s_stCipher3DesCfg)/sizeof(HI_CIPHER_MODE_CTRL_S);i++)
    {
        slRet = memcmp(&(s_stCipher3DesCfg[i]),ctrl,sizeof(HI_CIPHER_MODE_CTRL_S));
        if (HI_ERR_SUCCESS != slRet)
        {
            continue;
        }
        return HI_ERR_SUCCESS;
    }
    return HI_ERR_INVALID_PARAMETER;
}

/*  ---------------------------------------------------------------------------------------------------------------
    // AES
    // 由芯片密钥管理模块配置密钥(bKeyByCA = 1)时密钥长度只支持128位
    {HI_UNF_CIPHER_ALG_AES,HI_UNF_CIPHER_WORK_MODE_ECB,HI_UNF_CIPHER_BIT_WIDTH_128BIT,HI_UNF_CIPHER_KEY_AES_128BIT},
    {HI_UNF_CIPHER_ALG_AES,HI_UNF_CIPHER_WORK_MODE_CBC,HI_UNF_CIPHER_BIT_WIDTH_128BIT,HI_UNF_CIPHER_KEY_AES_128BIT},
    {HI_UNF_CIPHER_ALG_AES,HI_UNF_CIPHER_WORK_MODE_CFB,HI_UNF_CIPHER_BIT_WIDTH_1BIT,  HI_UNF_CIPHER_KEY_AES_128BIT},
    {HI_UNF_CIPHER_ALG_AES,HI_UNF_CIPHER_WORK_MODE_CFB,HI_UNF_CIPHER_BIT_WIDTH_8BIT,  HI_UNF_CIPHER_KEY_AES_128BIT},
    {HI_UNF_CIPHER_ALG_AES,HI_UNF_CIPHER_WORK_MODE_CFB,HI_UNF_CIPHER_BIT_WIDTH_128BIT,HI_UNF_CIPHER_KEY_AES_128BIT},
    {HI_UNF_CIPHER_ALG_AES,HI_UNF_CIPHER_WORK_MODE_OFB,HI_UNF_CIPHER_BIT_WIDTH_128BIT,HI_UNF_CIPHER_KEY_AES_128BIT},
    
    {HI_UNF_CIPHER_ALG_AES,HI_UNF_CIPHER_WORK_MODE_ECB,HI_UNF_CIPHER_BIT_WIDTH_128BIT,HI_UNF_CIPHER_KEY_AES_192BIT},
    {HI_UNF_CIPHER_ALG_AES,HI_UNF_CIPHER_WORK_MODE_CBC,HI_UNF_CIPHER_BIT_WIDTH_128BIT,HI_UNF_CIPHER_KEY_AES_192BIT},
    {HI_UNF_CIPHER_ALG_AES,HI_UNF_CIPHER_WORK_MODE_CFB,HI_UNF_CIPHER_BIT_WIDTH_1BIT,  HI_UNF_CIPHER_KEY_AES_192BIT},
    {HI_UNF_CIPHER_ALG_AES,HI_UNF_CIPHER_WORK_MODE_CFB,HI_UNF_CIPHER_BIT_WIDTH_8BIT,  HI_UNF_CIPHER_KEY_AES_192BIT},
    {HI_UNF_CIPHER_ALG_AES,HI_UNF_CIPHER_WORK_MODE_CFB,HI_UNF_CIPHER_BIT_WIDTH_128BIT,HI_UNF_CIPHER_KEY_AES_192BIT},
    {HI_UNF_CIPHER_ALG_AES,HI_UNF_CIPHER_WORK_MODE_OFB,HI_UNF_CIPHER_BIT_WIDTH_128BIT,HI_UNF_CIPHER_KEY_AES_192BIT},
    
    {HI_UNF_CIPHER_ALG_AES,HI_UNF_CIPHER_WORK_MODE_ECB,HI_UNF_CIPHER_BIT_WIDTH_128BIT,HI_UNF_CIPHER_KEY_AES_256BIT},
    {HI_UNF_CIPHER_ALG_AES,HI_UNF_CIPHER_WORK_MODE_CBC,HI_UNF_CIPHER_BIT_WIDTH_128BIT,HI_UNF_CIPHER_KEY_AES_256BIT},
    {HI_UNF_CIPHER_ALG_AES,HI_UNF_CIPHER_WORK_MODE_CFB,HI_UNF_CIPHER_BIT_WIDTH_1BIT,  HI_UNF_CIPHER_KEY_AES_256BIT},
    {HI_UNF_CIPHER_ALG_AES,HI_UNF_CIPHER_WORK_MODE_CFB,HI_UNF_CIPHER_BIT_WIDTH_8BIT,  HI_UNF_CIPHER_KEY_AES_256BIT},
    {HI_UNF_CIPHER_ALG_AES,HI_UNF_CIPHER_WORK_MODE_CFB,HI_UNF_CIPHER_BIT_WIDTH_128BIT,HI_UNF_CIPHER_KEY_AES_256BIT},
    {HI_UNF_CIPHER_ALG_AES,HI_UNF_CIPHER_WORK_MODE_OFB,HI_UNF_CIPHER_BIT_WIDTH_128BIT,HI_UNF_CIPHER_KEY_AES_256BIT},
    ---------------------------------------------------------------------------------------------------------------
    // DES
    // 选择DES运算时，低64位数据有效，即CIPHER_KEY[63:0]为有效数据
    {HI_UNF_CIPHER_ALG_DES,HI_UNF_CIPHER_WORK_MODE_ECB,HI_UNF_CIPHER_BIT_WIDTH_64BIT,HI_UNF_CIPHER_KEY_AES_128BIT},
    {HI_UNF_CIPHER_ALG_DES,HI_UNF_CIPHER_WORK_MODE_CBC,HI_UNF_CIPHER_BIT_WIDTH_64BIT,HI_UNF_CIPHER_KEY_AES_128BIT},
    {HI_UNF_CIPHER_ALG_DES,HI_UNF_CIPHER_WORK_MODE_CFB,HI_UNF_CIPHER_BIT_WIDTH_1BIT, HI_UNF_CIPHER_KEY_AES_128BIT},
    {HI_UNF_CIPHER_ALG_DES,HI_UNF_CIPHER_WORK_MODE_CFB,HI_UNF_CIPHER_BIT_WIDTH_8BIT, HI_UNF_CIPHER_KEY_AES_128BIT},
    {HI_UNF_CIPHER_ALG_DES,HI_UNF_CIPHER_WORK_MODE_CFB,HI_UNF_CIPHER_BIT_WIDTH_64BIT,HI_UNF_CIPHER_KEY_AES_128BIT},
    {HI_UNF_CIPHER_ALG_DES,HI_UNF_CIPHER_WORK_MODE_OFB,HI_UNF_CIPHER_BIT_WIDTH_1BIT, HI_UNF_CIPHER_KEY_AES_128BIT},
    {HI_UNF_CIPHER_ALG_DES,HI_UNF_CIPHER_WORK_MODE_OFB,HI_UNF_CIPHER_BIT_WIDTH_8BIT, HI_UNF_CIPHER_KEY_AES_128BIT},
    {HI_UNF_CIPHER_ALG_DES,HI_UNF_CIPHER_WORK_MODE_OFB,HI_UNF_CIPHER_BIT_WIDTH_64BIT,HI_UNF_CIPHER_KEY_AES_128BIT},
    ---------------------------------------------------------------------------------------------------------------
    // 3DES
    {HI_UNF_CIPHER_ALG_3DES,HI_UNF_CIPHER_WORK_MODE_ECB,HI_UNF_CIPHER_BIT_WIDTH_64BIT,HI_UNF_CIPHER_KEY_DES_2KEY},
    {HI_UNF_CIPHER_ALG_3DES,HI_UNF_CIPHER_WORK_MODE_CBC,HI_UNF_CIPHER_BIT_WIDTH_64BIT,HI_UNF_CIPHER_KEY_DES_2KEY},
    {HI_UNF_CIPHER_ALG_3DES,HI_UNF_CIPHER_WORK_MODE_CFB,HI_UNF_CIPHER_BIT_WIDTH_1BIT, HI_UNF_CIPHER_KEY_DES_2KEY},
    {HI_UNF_CIPHER_ALG_3DES,HI_UNF_CIPHER_WORK_MODE_CFB,HI_UNF_CIPHER_BIT_WIDTH_8BIT, HI_UNF_CIPHER_KEY_DES_2KEY},
    {HI_UNF_CIPHER_ALG_3DES,HI_UNF_CIPHER_WORK_MODE_CFB,HI_UNF_CIPHER_BIT_WIDTH_64BIT,HI_UNF_CIPHER_KEY_DES_2KEY},
    {HI_UNF_CIPHER_ALG_3DES,HI_UNF_CIPHER_WORK_MODE_OFB,HI_UNF_CIPHER_BIT_WIDTH_1BIT, HI_UNF_CIPHER_KEY_DES_2KEY},
    {HI_UNF_CIPHER_ALG_3DES,HI_UNF_CIPHER_WORK_MODE_OFB,HI_UNF_CIPHER_BIT_WIDTH_8BIT, HI_UNF_CIPHER_KEY_DES_2KEY},
    {HI_UNF_CIPHER_ALG_3DES,HI_UNF_CIPHER_WORK_MODE_OFB,HI_UNF_CIPHER_BIT_WIDTH_64BIT,HI_UNF_CIPHER_KEY_DES_2KEY},

    {HI_UNF_CIPHER_ALG_3DES,HI_UNF_CIPHER_WORK_MODE_ECB,HI_UNF_CIPHER_BIT_WIDTH_64BIT,HI_UNF_CIPHER_KEY_DES_3KEY},
    {HI_UNF_CIPHER_ALG_3DES,HI_UNF_CIPHER_WORK_MODE_CBC,HI_UNF_CIPHER_BIT_WIDTH_64BIT,HI_UNF_CIPHER_KEY_DES_3KEY},
    {HI_UNF_CIPHER_ALG_3DES,HI_UNF_CIPHER_WORK_MODE_CFB,HI_UNF_CIPHER_BIT_WIDTH_1BIT, HI_UNF_CIPHER_KEY_DES_3KEY},
    {HI_UNF_CIPHER_ALG_3DES,HI_UNF_CIPHER_WORK_MODE_CFB,HI_UNF_CIPHER_BIT_WIDTH_8BIT, HI_UNF_CIPHER_KEY_DES_3KEY},
    {HI_UNF_CIPHER_ALG_3DES,HI_UNF_CIPHER_WORK_MODE_CFB,HI_UNF_CIPHER_BIT_WIDTH_64BIT,HI_UNF_CIPHER_KEY_DES_3KEY},
    {HI_UNF_CIPHER_ALG_3DES,HI_UNF_CIPHER_WORK_MODE_OFB,HI_UNF_CIPHER_BIT_WIDTH_1BIT, HI_UNF_CIPHER_KEY_DES_3KEY},
    {HI_UNF_CIPHER_ALG_3DES,HI_UNF_CIPHER_WORK_MODE_OFB,HI_UNF_CIPHER_BIT_WIDTH_8BIT, HI_UNF_CIPHER_KEY_DES_3KEY},
    {HI_UNF_CIPHER_ALG_3DES,HI_UNF_CIPHER_WORK_MODE_OFB,HI_UNF_CIPHER_BIT_WIDTH_64BIT,HI_UNF_CIPHER_KEY_DES_3KEY},
    -------------------------------------------------------------------------------------------------------------*/
HI_U32 HI_DRV_CIPHER_ConfigChn(HI_HANDLE pHandle,  HI_CIPHER_CTRL_S *pConfig)
{
    HI_U32 ret = HI_ERR_FAILURE;
    DRV_CIPHER_LOCK();
    ret = DRV_CIPHER_ConfigChn(pHandle, pConfig);
    DRV_CIPHER_UNLOCK();
    return ret;
}

HI_PRV HI_U32 DRV_CipherStartSinglePkgChn(HI_U32 softChnId, HI_DRV_CIPHER_DATA_INFO_S *pBuf2Process)
{
    HI_U32 ret;
    CIPHER_SOFTCHAN_S *pSoftChan = &g_stCipherSoftChans[softChnId];
    CIPHER_CHAN_S *pChan = &g_stCipherChans[pSoftChan->u32HardWareChn];

    HAL_Cipher_Config(0, pBuf2Process->bDecrypt, pSoftChan->bIVChange, &(pSoftChan->stCtrl));

    if (pSoftChan->bIVChange)
    {
        ret = HAL_Cipher_SetInIV(0, &(pSoftChan->stCtrl));
        if (HI_ERR_SUCCESS != ret)
        {
            return ret;
        }
    }
	
    if (pSoftChan->bKeyChange)
    {
        ret = HAL_Cipher_SetSoftwareKey(0, &(pSoftChan->stCtrl));
        if (HI_ERR_SUCCESS != ret)
        {
            return ret;
        }
        
        pSoftChan->bKeyChange = HI_FALSE;
    }

    HAL_Cipher_SetDataSinglePkg(pBuf2Process);
    HAL_Cipher_StartSinglePkg(pChan->chnId);
    ret = HAL_Cipher_WaitIdle();
    if (HI_ERR_SUCCESS != ret)
    {
        return HI_ERR_FAILURE;
    }
    
    HAL_Cipher_ReadDataSinglePkg(pBuf2Process->u32DataPkg);
    return HI_ERR_SUCCESS;
}

HI_PRV HI_U32 DRV_CipherStartMultiPkgChn(HI_U32 softChnId, HI_DRV_CIPHER_DATA_INFO_S *pBuf2Process, HI_U32 callBackArg)
{
    HI_U32 ret = HI_ERR_FAILURE;
    CIPHER_SOFTCHAN_S *pSoftChan = &g_stCipherSoftChans[softChnId];
    HI_U32 hardWareChn = pSoftChan->u32HardWareChn;
    CIPHER_CHAN_S *pChan = &g_stCipherChans[hardWareChn]; 
    CI_BUF_LIST_ENTRY_S *pInBuf = HI_NULL;
    CI_BUF_LIST_ENTRY_S *pOutBuf = HI_NULL;
    HI_U32 BusyCnt = 0;
    HI_U32 currentPtr = 0;

    HAL_Cipher_GetInBufCnt(hardWareChn, &BusyCnt);
    pChan->unInData.stPkgNMng.u32BusyCnt = BusyCnt;
    currentPtr = pChan->unInData.stPkgNMng.u32CurrentPtr;
    pInBuf = pChan->pstInBuf + currentPtr;
    pOutBuf = pChan->pstOutBuf + currentPtr;

    if (BusyCnt < CIPHER_MAX_LIST_NUM) /* *///test zn err: CIPHER_REG_CHANn_IBUF_NUM
    {
        /* set addr */
        pInBuf->u32DataAddr = pBuf2Process->u32src;
        pInBuf->U32DataLen = pBuf2Process->u32length;
        pInBuf->u32Flags = (pSoftChan->bIVChange << 21);// zn

        pOutBuf->u32DataAddr = pBuf2Process->u32dest;
        pOutBuf->U32DataLen = pBuf2Process->u32length;
        pOutBuf->u32Flags = (pSoftChan->bIVChange << 21);// zn

        /* set IV */
        if (pSoftChan->bIVChange)
        {
            (hi_void)memcpy_s(pChan->astCipherIVValue[currentPtr].pu8StartVirAddr, CI_IV_SIZE, pSoftChan->stCtrl.u32IV, CI_IV_SIZE);
            pInBuf->u32IVStartAddr = (HI_U32)pChan->astCipherIVValue[currentPtr].pu8StartVirAddr; 
			pInBuf->u32Flags |= (1 << CI_BUF_LIST_FLAG_IVSET_BIT);
        }
        else
        {
            pInBuf->u32Flags &= ~(1 << CI_BUF_LIST_FLAG_IVSET_BIT);
        }
        /* set Key */
        if (pSoftChan->bKeyChange)
        {
            ret = HAL_Cipher_SetSoftwareKey(hardWareChn, &(pSoftChan->stCtrl));
            if (HI_ERR_SUCCESS != ret)
            {
                return ret;
            }

            pSoftChan->bKeyChange = HI_FALSE;
        }

        HI_MDM_DataCacheFlush((HI_PVOID)g_stCipherComm.pBuf, (HI_S32)g_stCipherComm.bufSize);
        ret = HAL_Cipher_Config(hardWareChn, pBuf2Process->bDecrypt, pSoftChan->bIVChange, &(pSoftChan->stCtrl));
        //pSoftChan->bIVChange = HI_FALSE;// for test zn

        pChan->au32WitchSoftChn[currentPtr] = softChnId;
        pChan->au32CallBackArg[currentPtr] = callBackArg;
        pSoftChan->u32PrivateData = callBackArg;
        pChan->bNeedCallback[currentPtr] = HI_TRUE;
        HI_INFO_CIPHER("pkg %d set ok.\n", currentPtr);

        currentPtr++;
        if (currentPtr >=  CIPHER_MAX_LIST_NUM)
        {
            currentPtr = 0;
        }

        /* save list Node */
        pChan->unInData.stPkgNMng.u32CurrentPtr = currentPtr;
        pChan->unInData.stPkgNMng.u32TotalPkg++;
        pChan->unOutData.stPkgNMng.u32TotalPkg++;

        HAL_Cipher_GetOutBufCnt(hardWareChn, &BusyCnt);// test zn 获取出来没啥用
	    HAL_Cipher_SetInBufCnt(hardWareChn, 1); /* +1 */ // test zn wxf
        HI_INFO_CIPHER("%s %x->%x, LEN:%x\r\n", pBuf2Process->bDecrypt ? "Dec" : "ENC",
                pBuf2Process->u32src, pBuf2Process->u32dest,
                pBuf2Process->u32length );
        HAL_Cipher_SetOutBufCnt(hardWareChn, 1);// for test zn hy
    }
    else
    {
        return HI_ERR_FAILURE;
    }

    return ret;
}

HI_PRV HI_U32 DRV_CIPHER_CreatMultiPkgTask(HI_U32 softChnId, HI_DRV_CIPHER_DATA_INFO_S *pBuf2Process, HI_U32 pkgNum, HI_BOOL isMultiIV, HI_U32 callBackArg)
{
    HI_U32 ret = HI_ERR_FAILURE;
    HI_U32 BusyCnt = 0;
    HI_U32 i = 0;
    HI_U32 currentPtr = 0;
    CI_BUF_LIST_ENTRY_S *pInBuf = HI_NULL;
    CI_BUF_LIST_ENTRY_S *pOutBuf = HI_NULL;
    HI_DRV_CIPHER_DATA_INFO_S *pTmpDataPkg = pBuf2Process;
    CIPHER_SOFTCHAN_S *pSoftChan = &g_stCipherSoftChans[softChnId];
    HI_U32 hardWareChn = pSoftChan->u32HardWareChn;
    CIPHER_CHAN_S *pChan = &g_stCipherChans[hardWareChn];

    HAL_Cipher_GetInBufCnt(hardWareChn, &BusyCnt);
    HI_DEBUG_CIPHER("HAL_Cipher_GetInBufCnt, BusyCnt=%d.\n", BusyCnt);
    pChan->unInData.stPkgNMng.u32BusyCnt = BusyCnt;

    if (BusyCnt + pkgNum > CIPHER_MAX_LIST_NUM)
    {
         HI_ERR_CIPHER("%s: pkg want to do: %u, free pkg num:%u.\r\n", pBuf2Process->bDecrypt ? "Dec" : "ENC",
                pkgNum, CIPHER_MAX_LIST_NUM - BusyCnt);
         return HI_ERR_CIPHER_BUSY;
    }


    /* set Key */
    if (pSoftChan->bKeyChange)
    {
        ret = HAL_Cipher_SetSoftwareKey(hardWareChn, &(pSoftChan->stCtrl));
        if (HI_ERR_SUCCESS != ret)
        {
            return ret;
        }
        
        pSoftChan->bKeyChange = HI_FALSE;
    }

    currentPtr = pChan->unInData.stPkgNMng.u32CurrentPtr;

    for (i = 0; i < pkgNum; i++)
    {
        pTmpDataPkg = pBuf2Process + i;
        pInBuf = pChan->pstInBuf + currentPtr;
        pOutBuf = pChan->pstOutBuf + currentPtr;


        /* set addr */
        pInBuf->u32DataAddr = pTmpDataPkg->u32src;
        pInBuf->U32DataLen = pTmpDataPkg->u32length;

        pOutBuf->u32DataAddr = pTmpDataPkg->u32dest;
        pOutBuf->U32DataLen = pTmpDataPkg->u32length;

        /* set IV */
        if (pSoftChan->bIVChange)
        {
            (hi_void)memcpy_s(pChan->astCipherIVValue[currentPtr].pu8StartVirAddr, CI_IV_SIZE, pSoftChan->stCtrl.u32IV, CI_IV_SIZE);
            pInBuf->u32IVStartAddr = pChan->astCipherIVValue[currentPtr].u32StartPhyAddr;
            pInBuf->u32Flags |= (1 << CI_BUF_LIST_FLAG_IVSET_BIT);
        }
        if (isMultiIV)
        {
            pInBuf->u32Flags |= (1 << CI_BUF_LIST_FLAG_EOL_BIT);
        }

        pChan->au32WitchSoftChn[currentPtr] = softChnId;
        pChan->au32CallBackArg[currentPtr] = callBackArg;
        pSoftChan->u32PrivateData = callBackArg;
        if ((i + 1) == pkgNum)
        {
            pChan->bNeedCallback[currentPtr] = HI_TRUE ;

            /* just set each node to End_of_list, <--- changed by q46153, 20111108, only the last pkg need this. */
            pInBuf->u32Flags |= (1 << CI_BUF_LIST_FLAG_EOL_BIT);
            pOutBuf->u32Flags |= (1 << CI_BUF_LIST_FLAG_EOL_BIT);
        }
        else
        {
            pChan->bNeedCallback[currentPtr] = HI_FALSE ;
        }

        currentPtr++;
        if (currentPtr >=  CIPHER_MAX_LIST_NUM)
        {
            currentPtr = 0;
        }

        /* save list Node */
        pChan->unInData.stPkgNMng.u32CurrentPtr = currentPtr;
        pChan->unInData.stPkgNMng.u32TotalPkg++;
        pChan->unOutData.stPkgNMng.u32TotalPkg++;
    }

    HI_MDM_DataCacheFlush((HI_PVOID)g_stCipherComm.pBuf, (HI_S32)g_stCipherComm.bufSize);
    ret = HAL_Cipher_Config(hardWareChn, pTmpDataPkg->bDecrypt,
                            pSoftChan->bIVChange, &(pSoftChan->stCtrl));

    HAL_Cipher_SetIntThreshold(pChan->chnId, CIPHER_INT_TYPE_OUT, pkgNum);
    HAL_Cipher_SetIntThreshold(pChan->chnId, CIPHER_INT_TYPE_IN, pkgNum);// for test zn hy
    HAL_Cipher_GetOutBufCnt(hardWareChn, &BusyCnt);
    HAL_Cipher_SetInBufCnt(hardWareChn, pkgNum); /* commit task */
    HAL_Cipher_SetOutBufCnt(hardWareChn, pkgNum);// for test zn hy
    HI_INFO_CIPHER("%s: pkg:%x.\r\n", pTmpDataPkg->bDecrypt ? "Dec" : "ENC",   pkgNum);
    return ret;
}

HI_PRV HI_U32 DRV_CIPHER_CreatTask(HI_U32 softChnId, HI_DRV_CIPHER_TASK_S *pTask, HI_U32 *pKey, HI_U32 *pIV)
{
    HI_U32 ret;
    CIPHER_SOFTCHAN_S *pSoftChan = &g_stCipherSoftChans[softChnId];
    CIPHER_CHAN_S *pChan = &g_stCipherChans[pSoftChan->u32HardWareChn];

    if (pKey)
    {
        pSoftChan->bKeyChange = HI_TRUE;
        (hi_void)memcpy_s(pSoftChan->stCtrl.u32Key, sizeof(pSoftChan->stCtrl.u32Key), pKey, CI_KEY_SIZE);
    }

    if (pIV)
    {
        pSoftChan->bIVChange = HI_TRUE;
        (hi_void)memcpy_s(pSoftChan->stCtrl.u32IV, sizeof(pSoftChan->stCtrl.u32IV), pIV, CI_IV_SIZE);
    }

    HAL_Cipher_SetIntThreshold(pChan->chnId, CIPHER_INT_TYPE_OUT, 1);

    if (CIPHER_PKGx1_CHAN == pSoftChan->u32HardWareChn)
    {
        // 不回调 查询1000次
        ret = DRV_CipherStartSinglePkgChn(softChnId, &(pTask->stData2Process));
    }
    else
    {
        ret = DRV_CipherStartMultiPkgChn(softChnId, &(pTask->stData2Process), pTask->u32CallBackArg);
    }

    if (HI_ERR_SUCCESS != ret)
    {
        HI_ERR_CIPHER("can't create task, ERR=%#x.\r\n", ret);
        return ret;
    }
    return HI_ERR_SUCCESS;
}

HI_PRV HI_U32 DRV_CipherDataDoneSinglePkg(HI_U32 chnId)
{
    CIPHER_SOFTCHAN_S *pSoftChan;

    HI_DEBUG_CIPHER("Data DONE, hwChn:%d\n", chnId);
    pSoftChan = &g_stCipherSoftChans[chnId];

    if (pSoftChan->pfnCallBack)
    {
        pSoftChan->pfnCallBack(pSoftChan->u32PrivateData);
    }
    
    return HI_ERR_SUCCESS;
}

HI_PRV HI_U32 DRV_CipherDataDoneMultiPkg(HI_U32 chnId)
{
    HI_U32 ret = HI_ERR_FAILURE;
    HI_U32 softChnId = 0;
    HI_U32 fullCnt = 0;
    HI_U32 i, idx = 0;
    CIPHER_CHAN_S *pChan = &g_stCipherChans[chnId];
    HI_U32 currentPtr = pChan->unOutData.stPkgNMng.u32CurrentPtr;
    CIPHER_SOFTCHAN_S *pSoftChan = HI_NULL;
    CI_BUF_LIST_ENTRY_S *pInBuf = HI_NULL;
    CI_BUF_LIST_ENTRY_S *pOutBuf = HI_NULL;

    /* get the finished output data buffer count */
    ret = HAL_Cipher_GetOutBufFull(chnId, &fullCnt);
    if (HI_ERR_SUCCESS != ret)
    {
        return ret;
    }
    
    idx = currentPtr;
    if(idx >= CIPHER_MAX_LIST_NUM)
    {
        HI_ERR_CIPHER("idx error: idx=%u, chnId=%d \r\n", idx, chnId);
        return HI_ERR_FAILURE;
    }

    if (fullCnt > 0) /* have list entry */
    {
        for (i = 0; i < fullCnt; i++)
        {
            softChnId = pChan->au32WitchSoftChn[idx];
            pChan->au32WitchSoftChn[idx] = CIPHER_INVALID_CHN;

            pSoftChan = &g_stCipherSoftChans[softChnId];
            pSoftChan->u32LastPkg = idx;
            HI_DEBUG_CIPHER("softChnId=%d, idx=%u, needCallback:%d\n", softChnId, idx, pChan->bNeedCallback[idx]);
            if (pSoftChan->pfnCallBack && pChan->bNeedCallback[idx])
            {
                HI_DEBUG_CIPHER("CallBack function\n");
                pSoftChan->pfnCallBack(pSoftChan->u32PrivateData);
            }

            pInBuf = pChan->pstInBuf + idx;  /* reset the flag of each pkg */
            pInBuf->u32Flags = 0;

            pOutBuf = pChan->pstOutBuf + idx; /* reset the flag of each pkg */
            pOutBuf->u32Flags = 0;

            idx++;
            if (idx >= CIPHER_MAX_LIST_NUM)
            {
                idx = 0;
            }
        }

        pChan->unOutData.stPkgNMng.u32CurrentPtr = idx;
        HAL_Cipher_SetInBufEmpty(chnId, fullCnt);  /* -  */
        HAL_Cipher_SetOutBufFull(chnId, fullCnt);  /* -  */
        //HAL_Cipher_SetOutBufCnt(chnId, fullCnt); /* +  */ //for test zn hy
    }
    else
    {
        HI_U32 regValue = 0xabcd;

        HI_ERR_CIPHER("Data done, but fullCnt=0, chn%d\r\n", chnId);

        HAL_Cipher_GetIntState(&regValue);
        HI_ERR_CIPHER("INTSt:%#x\r\n", regValue);

        HAL_Cipher_GetIntEnState(&regValue);
        HI_ERR_CIPHER("INTEnSt:%#x\r\n", regValue);

        HAL_Cipher_GetRawIntState(&regValue);
        HI_ERR_CIPHER("INTRawSt:%#x\r\n", regValue);
        return HI_ERR_FAILURE;
    }

    return HI_ERR_SUCCESS;
}

/* interrupt routine, callback */
HI_PRV HI_VOID DRV_Cipher_Lisr(HI_S32 vector)
{
    HI_U32 i;
    HI_U32 INTValue = 0;

    HAL_Cipher_GetIntState(&INTValue);
    HAL_Cipher_ClrIntState(INTValue);

    HI_DEBUG_CIPHER(" in the isr INTValue=%#x!\n", INTValue);

    if (INTValue >> 8 & 0x1) /* single pkg */
    {
        DRV_CipherDataDoneSinglePkg(0);
    }

    for(i = CIPHER_PKGxN_CHAN_MIN; i <= CIPHER_PKGxN_CHAN_MAX; i++)
    {
        if ((INTValue >> (i+8)) & 0x1)
        {
            DRV_CipherDataDoneMultiPkg(i);
        }
    } 
}

HI_PRV HI_U32 DRV_CIPHER_Init(HI_VOID)
{
    HI_U32 i = 0;
    HI_U32 j = 0;
    HI_U32 hwChnId = 0;
    HI_U32 ChnIdx = 0;
    HI_U32 ret = HI_ERR_FAILURE;
    HI_U32 bufSizeChn = 0; /* all the buffer list size, included data buffer size and IV buffer size */
    HI_U32 databufSizeChn = 0; /* max list number data buffer size */
    HI_U32 ivbufSizeChn = 0; /* all the list IV size */
    HI_U32 bufSizeTotal = 0; /* all the channel buffer size */
    HI_U32 u32ClrAllInt = 0;
    CIPHER_CHAN_S *pChan = HI_NULL;

    (hi_void)memset_s((HI_VOID *)&g_stCipherComm, sizeof(g_stCipherComm), 0, sizeof(g_stCipherComm));
    (hi_void)memset_s((HI_VOID *)g_stCipherChans, sizeof(g_stCipherChans), 0, sizeof(g_stCipherChans));
    (hi_void)memset_s((HI_VOID *)g_stCipherSoftChans, sizeof(g_stCipherSoftChans), 0, sizeof(g_stCipherSoftChans));

    g_stCipherOsrChn[CIPHER_PKGx1_CHAN].g_bSoftChnOpen = HI_FALSE;
    g_stCipherOsrChn[CIPHER_PKGx1_CHAN].g_bDataDone = HI_FALSE;
    g_stCipherOsrChn[CIPHER_PKGx1_CHAN].pWichFile = HI_NULL;
    
    DRV_CIPHER_EvtInit();
    DRV_CIPHER_SemInit();
    g_stCipherOsrChn[CIPHER_PKGx1_CHAN].pstDataPkg = HI_NULL;
    for (i = CIPHER_PKGxN_CHAN_MIN; i <= CIPHER_PKGxN_CHAN_MAX; i++)
    {
        g_stCipherOsrChn[i].g_bSoftChnOpen = HI_FALSE;
        g_stCipherOsrChn[i].g_bDataDone = HI_FALSE;
        g_stCipherOsrChn[i].pWichFile = HI_NULL;
        g_stCipherOsrChn[i].pstDataPkg = HI_NULL;
    }

/*
==========================channel-1=============================
*----------------------------------------------------------------------
*| +++++++++++++++++++                               +++++++++++++++++++  |
*| +byte1|byte2|byte3|byte4 +       ... ...                 +byte1|byte2|byte3|byte4 +  |inBuf
*| +++++++++++++++++++                               +++++++++++++++++++  |
*|             list-1                              ... ...                              list-128(MAX_LIST)    |
*----------------------------------------------------------------------
*| +++++++++++++++++++                               +++++++++++++++++++  |
*| +byte1|byte2|byte3|byte4 +       ... ...                 +byte1|byte2|byte3|byte4 +  |outBuf
*| +++++++++++++++++++                               +++++++++++++++++++  |
*|             list-1                              ... ...                              list-128(MAX_LIST)    |
*----------------------------------------------------------------------
*| +++++++++++++++++++                               +++++++++++++++++++  |
*| +byte1|byte2|byte3|byte4 +       ... ...                 +byte1|byte2|byte3|byte4 +  |keyBuf
*| +++++++++++++++++++                               +++++++++++++++++++  |
*|             list-1                              ... ...                              list-128(MAX_LIST)    |
*----------------------------------------------------------------------
=============================================================
...
...
...
==========================channel-7=============================
*----------------------------------------------------------------------
*| +++++++++++++++++++                               +++++++++++++++++++  |
*| +byte1|byte2|byte3|byte4 +       ... ...                 +byte1|byte2|byte3|byte4 +  |inBuf
*| +++++++++++++++++++                               +++++++++++++++++++  |
*|             list-1                              ... ...                              list-128(MAX_LIST)    |
*----------------------------------------------------------------------
*| +++++++++++++++++++                               +++++++++++++++++++  |
*| +byte1|byte2|byte3|byte4 +       ... ...                 +byte1|byte2|byte3|byte4 +  |outBuf
*| +++++++++++++++++++                               +++++++++++++++++++  |
*|             list-1                              ... ...                              list-128(MAX_LIST)    |
*----------------------------------------------------------------------
*| +++++++++++++++++++                               +++++++++++++++++++  |
*| +byte1|byte2|byte3|byte4 +       ... ...                 +byte1|byte2|byte3|byte4 +  |keyBuf
*| +++++++++++++++++++                               +++++++++++++++++++  |
*|             list-1                              ... ...                              list-128(MAX_LIST)    |
*----------------------------------------------------------------------
=============================================================
*/

    databufSizeChn = sizeof(CI_BUF_LIST_ENTRY_S) * CIPHER_MAX_LIST_NUM;// 2048
    ivbufSizeChn = CI_IV_SIZE * CIPHER_MAX_LIST_NUM;// 2048
    bufSizeChn = (databufSizeChn * 2) + ivbufSizeChn;/* inBuf + outBuf + keyBuf */
    bufSizeTotal = bufSizeChn * (CIPHER_PKGxN_CHAN_MAX - CIPHER_PKGxN_CHAN_MIN + 1) ; /* only 7 channels need buf */

    HAL_Cipher_DisableAllInt();

	// cache首地址对齐+大小对齐
    g_stCipherComm.pBufOriginal = (HI_U8 *)HI_MDM_Malloc(HI_MOD_ID_DRV_CIPHER, bufSizeTotal + ALIGNTYPE_64BYTE);
    if (HI_NULL == g_stCipherComm.pBufOriginal)
    {
        HI_ERR_CIPHER("Can NOT get mem for cipher, init failed, exit...\r\n");
        return HI_ERR_FAILURE;
    }
    else
    {
		// cache首地址对齐
        g_stCipherComm.pBuf = (HI_U8 *) ALIGN_LENGTH((HI_U32)g_stCipherComm.pBufOriginal, ALIGNTYPE_32BYTE);
        (hi_void)memset_s(g_stCipherComm.pBuf, bufSizeTotal, 0, bufSizeTotal);
        g_stCipherComm.bufSize = bufSizeTotal;
    }
    
    /* assign hardware channel ID from 0 to 7 */
    for (i = 0; i <= CIPHER_PKGxN_CHAN_MAX; i++)
    {
        pChan = &g_stCipherChans[i];
        pChan->chnId = i;
    }

/*
channel layout
==============================================================
|
|
==============================================================
/\                                     /\                                      /\
 |              IV buf                  |             IN buf                    |             OUT buf
startPhyAddr
==============================================================
|
|
==============================================================
/\                                     /\                                      /\
 |              IV buf                  |             IN buf                    |             OUT buf
 startVirAddr
*/
    for (i = CIPHER_PKGxN_CHAN_MIN; i <= CIPHER_PKGxN_CHAN_MAX; i++)
    {
        /* config channel from 1 to 7 */
        hwChnId = i;
        ChnIdx = i - CIPHER_PKGxN_CHAN_MIN;
        pChan = &g_stCipherChans[hwChnId];

        for (j = 0; j < CIPHER_MAX_LIST_NUM; j++)        
        {
            // 每个通道128个IV,128个IN,128个OUT
            pChan->astCipherIVValue[j].pu8StartVirAddr = (HI_U8*)(g_stCipherComm.pBuf + (ChnIdx * bufSizeChn) + (CI_IV_SIZE * j));  
        }

        pChan->pstInBuf = (CI_BUF_LIST_ENTRY_S*)(g_stCipherComm.pBuf + (ChnIdx * bufSizeChn) + ivbufSizeChn);
        pChan->pstOutBuf = (CI_BUF_LIST_ENTRY_S*)(g_stCipherComm.pBuf + (ChnIdx * bufSizeChn) + ivbufSizeChn + databufSizeChn);

        ret = HAL_Cipher_SetBufAddr(hwChnId, CIPHER_BUF_TYPE_IN, ((HI_U32)g_stCipherComm.pBuf + (ChnIdx * bufSizeChn) + ivbufSizeChn));
        if (HI_ERR_SUCCESS != ret)
        {
            return ret;
        }
        
        ret = HAL_Cipher_SetBufAddr(hwChnId, CIPHER_BUF_TYPE_OUT, ((HI_U32)g_stCipherComm.pBuf + (ChnIdx * bufSizeChn) + ivbufSizeChn + databufSizeChn));
        if (HI_ERR_SUCCESS != ret)
        {
            return ret;
        }

        ret = DRV_CipherInitHardWareChn(hwChnId);
		if (HI_ERR_SUCCESS != ret)
		{
			return ret;
		}
	}

    HI_MDM_DataCacheFlush((HI_PVOID)g_stCipherComm.pBuf, (HI_S32)g_stCipherComm.bufSize);// wxf debug// for test zn

    /* debug info */
    for (i = CIPHER_PKGxN_CHAN_MIN; i <= CIPHER_PKGxN_CHAN_MAX; i++)
    {
        HI_INFO_CIPHER("Chn%02x, IV:%x/%p In:%x/%p, Out:%x/%p.\r\n", i,
            g_stCipherChans[i].astCipherIVValue[0].u32StartPhyAddr,
            g_stCipherChans[i].astCipherIVValue[0].pu8StartVirAddr,
            g_stCipherChans[i].astCipherIVValue[0].u32StartPhyAddr + ivbufSizeChn, g_stCipherChans[i].pstInBuf,
            g_stCipherChans[i].astCipherIVValue[0].u32StartPhyAddr + ivbufSizeChn + databufSizeChn, g_stCipherChans[i].pstOutBuf );
    }

    u32ClrAllInt = 0x00;
    for (i = CIPHER_PKGxN_CHAN_MIN; i <= CIPHER_PKGxN_CHAN_MAX; i++)
    {
        u32ClrAllInt |= 0x01 << i;
        u32ClrAllInt |= 0x01 << (i+7);
    }
    u32ClrAllInt |= 0x01 << 8;
    u32ClrAllInt |= 0x01 << 16;
    HAL_Cipher_ClrIntState(u32ClrAllInt);

    /* request irq */
    if (HI_ERR_SUCCESS != HI_MDM_IRQ_Request(HI_IRQ_CIPHER, 0,DRV_Cipher_Lisr, 0))
    {
        return HI_ERR_FAILURE;
    }

    //enable VIC
    if (HI_ERR_SUCCESS != (HI_U32)HI_MDM_IRQ_Enable((HI_S32)HI_IRQ_CIPHER))
    {
        return HI_ERR_FAILURE;
    }
	
    return HI_ERR_SUCCESS;
}

HI_PRV HI_U32 DRV_CIPHER_DeInit(HI_VOID)
{
    HI_U32 i, hwChnId;
    HI_U32 u32ClrAllInt;
    HI_U32 ulRet = HI_ERR_SUCCESS;

    u32ClrAllInt = 0x0;
    for (i = CIPHER_PKGxN_CHAN_MIN; i <= CIPHER_PKGxN_CHAN_MAX; i++)
    {
        u32ClrAllInt |= 0x01 << i;
        u32ClrAllInt |= 0x01 << (i+7);
    }
    u32ClrAllInt |= 0x01 << 8;
    u32ClrAllInt |= 0x01 << 16;
    HAL_Cipher_ClrIntState(u32ClrAllInt);
    HAL_Cipher_DisableAllInt();
    HAL_Cipher_ClrIntState(u32ClrAllInt);

    for (i = CIPHER_PKGxN_CHAN_MIN; i <= CIPHER_PKGxN_CHAN_MAX; i++)
    {
        hwChnId = i;
        ulRet = DRV_CipherDeInitHardWareChn(hwChnId);
        if (ulRet)
        {
            return ulRet;
        }
            
    }

    /* free irq */
    ulRet = HI_MDM_IRQ_Disable(HI_IRQ_CIPHER);
    if (ulRet)
    {
        return ulRet;
    }
    
    HI_MDM_Free(HI_MOD_ID_DRV_CIPHER, g_stCipherComm.pBufOriginal);
    g_stCipherComm.pBuf = HI_NULL;
    g_stCipherComm.pBufOriginal = HI_NULL;
    return DRV_CIPHER_SemDeInit();
}

HI_U32 HI_DRV_CIPHER_DeInit(HI_VOID)
{
    return DRV_CIPHER_DeInit();
}

HI_U32 HI_DRV_CIPHER_Init(HI_VOID)
{    
    return DRV_CIPHER_Init();
}

HI_PRV HI_U32 DRV_CIPHER_GetHandleConfig(HI_HANDLE pCiHandle, HI_CIPHER_CTRL_S *pstCipherConfig)
{
    CIPHER_SOFTCHAN_S *pSoftChan = HI_NULL;
    HI_U32 u32SoftChanId = 0;

    if(pstCipherConfig == HI_NULL)
    {
        HI_ERR_CIPHER("Error! NULL pointer!\r\n");
        return HI_ERR_FAILURE;
    }

    u32SoftChanId = HI_HANDLE_GET_CHNID(pCiHandle);// test zn Handle 怎么转换的
    CIPHER_CHECK_HANDLE_RETURN_ERR(u32SoftChanId);

    pSoftChan = &g_stCipherSoftChans[u32SoftChanId];
    (hi_void)memcpy_s(pstCipherConfig, sizeof(HI_CIPHER_CTRL_S), &(pSoftChan->stCtrl), sizeof(HI_CIPHER_CTRL_S));
    return HI_ERR_SUCCESS;
}

HI_U32 HI_DRV_CIPHER_GetHandleConfig(HI_HANDLE pHandle, HI_CIPHER_CTRL_S *pstCipherConfig)
{
    HI_U32 ret;
    DRV_CIPHER_LOCK();
    ret = DRV_CIPHER_GetHandleConfig(pHandle, pstCipherConfig);
    DRV_CIPHER_UNLOCK();
    return ret;
}

HI_PRV HI_U32 DRV_CIPHER_CreateHandle(HI_HANDLE hCIHandle, HI_CIPHER_TYPE_E enCipherType)
{
	HI_U32 i = 0;
	HI_HANDLE hCipherchn = HI_NULL;
	HI_U32 softChnId = 0;

	if (HI_NULL == hCIHandle)
	{
	    HI_ERR_CIPHER("Invalid params!\r\n");
	    return HI_ERR_INVALID_PARAMETER;
	}

    if(HI_UNF_CIPHER_TYPE_MAX <= enCipherType)
    {
        HI_ERR_CIPHER("Invalid cipher type!\r\n");
        return HI_ERR_FAILURE;
    }
        
    if (HI_UNF_CIPHER_TYPE_NORMAL != enCipherType)
    {
        if (HI_FALSE == g_stCipherOsrChn[0].g_bSoftChnOpen)
        {
            i = CIPHER_PKGx1_CHAN;
        }
        else
        {
            i = CIPHER_INVALID_CHN;
        }
    }
    else
    {
        for(i = CIPHER_PKGxN_CHAN_MIN; i < CIPHER_SOFT_CHAN_NUM; i++)
        {
            if (HI_FALSE == g_stCipherOsrChn[i].g_bSoftChnOpen)
            {
                break;
            }
        }
    }

    if (i >= CIPHER_SOFT_CHAN_NUM)
    {
        HI_ERR_CIPHER("No more cipher chan left.\r\n");
        return HI_ERR_CIPHER_FAILED_GETHANDLE;
    }
    else
    {
        g_stCipherOsrChn[i].pstDataPkg = (HI_UNF_CIPHER_DATA_S *)HI_MDM_Malloc(HI_MOD_ID_DRV_CIPHER,  sizeof(HI_UNF_CIPHER_DATA_S) * CIPHER_MAX_LIST_NUM);
        if (HI_NULL == g_stCipherOsrChn[i].pstDataPkg)
        {
            HI_ERR_CIPHER("can NOT malloc memory for cipher.\r\n");
            return HI_ERR_CIPHER_FAILED_GETHANDLE;
        }

        softChnId = i;
        g_stCipherOsrChn[softChnId].g_bSoftChnOpen = HI_TRUE;
    }

    hCipherchn = HI_HANDLE_MAKEHANDLE(softChnId);

    DRV_CIPHER_OpenChn(softChnId);

    // test zn file 没看出来什么用[i].pWichFile = file;// test zn 后面没看见使用
    *(HI_U32 *)hCIHandle = (HI_U32)hCipherchn;
    return HI_ERR_SUCCESS;
}

HI_U32 HI_DRV_CIPHER_CreateHandle(HI_HANDLE pHandle, HI_CIPHER_TYPE_E enCipherType)
{
    HI_U32 ret = HI_ERR_FAILURE;
    DRV_CIPHER_LOCK();
    ret = DRV_CIPHER_CreateHandle(pHandle, enCipherType);
    DRV_CIPHER_UNLOCK();
    return ret;
}

HI_PRV HI_U32 DRV_CIPHER_DestroyHandle(HI_HANDLE hCipherchn)
{
    HI_U32 softChnId = 0;

    softChnId = HI_HANDLE_GET_CHNID(hCipherchn);
    CIPHER_CHECK_HANDLE_RETURN_ERR(softChnId);
    
    if (HI_FALSE == g_stCipherOsrChn[softChnId].g_bSoftChnOpen)
    {
        return HI_ERR_SUCCESS;
    }

    if (g_stCipherOsrChn[softChnId].pstDataPkg)
    {
        HI_MDM_Free(HI_MOD_ID_DRV_CIPHER, g_stCipherOsrChn[softChnId].pstDataPkg);
        g_stCipherOsrChn[softChnId].pstDataPkg = HI_NULL;
    }

    g_stCipherOsrChn[softChnId].g_bSoftChnOpen = HI_FALSE;
    g_stCipherOsrChn[softChnId].pWichFile = HI_NULL;

    return DRV_CIPHER_CloseChn(softChnId);
}

HI_U32 HI_DRV_CIPHER_DestroyHandle(HI_HANDLE pHandle)
{
    HI_U32 ret = HI_ERR_FAILURE;
    DRV_CIPHER_LOCK();
    ret = DRV_CIPHER_DestroyHandle(pHandle);
    DRV_CIPHER_UNLOCK();
    return ret;
}

HI_PRV HI_U32 DRV_CIPHER_Encrypt(CIPHER_DATA_S *pstCIData)
{
    HI_U32 ret = HI_ERR_FAILURE;
    HI_U32 softChnId = 0;
    HI_DRV_CIPHER_TASK_S stCITask;
    HI_U32 u32BlockSize = CIPHER_BLOCK_SIZE_DES; 
    HI_U32 event_val = 0;

    (hi_void)memset_s((HI_VOID *)&stCITask, sizeof(stCITask), 0, sizeof(stCITask));
    softChnId = HI_HANDLE_GET_CHNID(pstCIData->pCIHandle);
    CIPHER_CHECK_HANDLE_RETURN_ERR(softChnId);
    
    HI_DEBUG_CIPHER("softChnId = %d\r\n", softChnId);
    if (g_stCipherSoftChans[softChnId].stCtrl.stModeCtrl.enAlg == HI_UNF_CIPHER_ALG_AES)
    {
        u32BlockSize = CIPHER_BLOCK_SIZE_AES;
        if ((pstCIData->u32DataLength == 0) 
            || (pstCIData->u32DataLength % u32BlockSize != 0))
        {
            HI_ERR_CIPHER("Invalid encrypt length, must be multiple of 16 bytes!\r\n");
            return HI_ERR_FAILURE;
        }
    }

    HI_MDM_DataCacheFlush((HI_PVOID)pstCIData->u32SrcAddr, (HI_S32)pstCIData->u32DataLength);
    HI_MDM_DataCacheInvalidate((HI_PVOID)pstCIData->u32DestAddr, (HI_S32)pstCIData->u32DataLength);
    if ( 0 != softChnId )
    {
        stCITask.stData2Process.u32src = pstCIData->u32SrcAddr;
        stCITask.stData2Process.u32dest = pstCIData->u32DestAddr;
        stCITask.stData2Process.u32length = pstCIData->u32DataLength;
        stCITask.stData2Process.bDecrypt = HI_FALSE;
        stCITask.u32CallBackArg = softChnId;

        HI_INFO_CIPHER("Start to Encrypt, chnNum = %x!\r\n", softChnId);

        g_stCipherOsrChn[softChnId].g_bDataDone = HI_FALSE;

        ret = DRV_CIPHER_CreatTask(softChnId,&stCITask, HI_NULL, HI_NULL);
        if (HI_ERR_SUCCESS != ret)
        {
            return ret;
        }

        ret = HI_SYS_WaitEvt(EN_SAL_EVENT_CIPHER, HI_FALSE, &event_val, CIPHER_CRYPT_WAIT_TIME_MAX);
        if (ret != HI_ERR_SUCCESS)
        {
            return ret;
        }
        else
        {
            if ((HI_FALSE == g_stCipherOsrChn[softChnId].g_bDataDone) || (0 == (event_val & (1 << softChnId))))
            {
                return HI_ERR_FAILURE;
            }
        }
        HI_INFO_CIPHER("Encrypt OK, chnNum = %x!\r\n", softChnId);
        ret = HI_ERR_SUCCESS;
    }
    else
    {
        HI_U32 u32Offset = 0;
        HI_U8 *pu8SrcVir = (HI_U8 *)pstCIData->u32SrcAddr;
        HI_U8 *pu8DestVir = (HI_U8 *)pstCIData->u32DestAddr;
        while(u32Offset < pstCIData->u32DataLength)
        {
            (hi_void)memcpy_s((HI_U8 *)(stCITask.stData2Process.u32DataPkg), 
                        sizeof(stCITask.stData2Process.u32DataPkg), 
                        pu8SrcVir + u32Offset, 
                        u32BlockSize);
            stCITask.stData2Process.u32length = u32BlockSize;
            stCITask.stData2Process.bDecrypt = HI_FALSE;
            stCITask.u32CallBackArg = softChnId;

            ret = DRV_CIPHER_CreatTask(softChnId, &stCITask, HI_NULL, HI_NULL);
            if (HI_ERR_SUCCESS != ret)
            {
                HI_ERR_CIPHER("Cipher create task failed!\r\n");
                ret = HI_ERR_FAILURE;
                break;
            }

            (hi_void)memcpy_s((HI_VOID *)(pu8DestVir + u32Offset), u32BlockSize, (HI_VOID *)stCITask.stData2Process.u32DataPkg, u32BlockSize);
            u32Offset += u32BlockSize;
        }
    }

    return ret;
}

HI_U32 HI_DRV_CIPHER_Encrypt(CIPHER_DATA_S *pstCIData)
{
    HI_U32 ret = HI_ERR_FAILURE;
    HI_U32 u32TotalSize = 0;
    CIPHER_DATA_S stCIData;
    if ((HI_NULL == pstCIData)
        ||(HI_NULL == ((HI_VOID *)pstCIData->u32SrcAddr))
        ||(HI_NULL == ((HI_VOID *)pstCIData->u32DestAddr))
        ||(0 == pstCIData->u32DataLength))
    {
        return HI_ERR_INVALID_PARAMETER;
    }

    DRV_CIPHER_LOCK();
    stCIData.pCIHandle = pstCIData->pCIHandle;
    HI_LOW_POWER_Forbid(HI_LOW_POWER_FORBID_ID_CIPHER);
    while (u32TotalSize < pstCIData->u32DataLength)// 每次解析的最大长度
    {
        stCIData.u32DataLength = pstCIData->u32DataLength - u32TotalSize;
        if (stCIData.u32DataLength > CIPHER_MAX_NODE_BUF_SIZE)
        {
            stCIData.u32DataLength = CIPHER_MAX_NODE_BUF_SIZE;
        }

        stCIData.u32SrcAddr  = pstCIData->u32SrcAddr  + u32TotalSize;
        stCIData.u32DestAddr = pstCIData->u32DestAddr + u32TotalSize;
        u32TotalSize += stCIData.u32DataLength;

        ret = DRV_CIPHER_Encrypt(&stCIData);
        if (ret != HI_ERR_SUCCESS)
        {
            break;
        }
    }
    HI_LOW_POWER_UnForbid(HI_LOW_POWER_FORBID_ID_CIPHER,0);
    DRV_CIPHER_UNLOCK();
    return ret;
}

HI_PRV HI_U32 DRV_CIPHER_Decrypt(CIPHER_DATA_S *pstCIData)
{
    HI_U32 ret = HI_ERR_FAILURE;
    HI_U32 softChnId = 0;
    HI_DRV_CIPHER_TASK_S stCITask;
    HI_U32 u32BlockSize = CIPHER_BLOCK_SIZE_DES;
    HI_U32 event_val = 0;

    if(HI_NULL == pstCIData)
    {
        HI_ERR_CIPHER("Invalid param!\r\n");
        return HI_ERR_FAILURE;
    }

    (hi_void)memset_s((HI_VOID *)&stCITask, sizeof(stCITask), 0, sizeof(stCITask));
    softChnId = HI_HANDLE_GET_CHNID(pstCIData->pCIHandle);
    CIPHER_CHECK_HANDLE_RETURN_ERR(softChnId);

    if (g_stCipherSoftChans[softChnId].stCtrl.stModeCtrl.enAlg == HI_UNF_CIPHER_ALG_AES)
    {
        u32BlockSize = CIPHER_BLOCK_SIZE_AES;
        if ( (pstCIData->u32DataLength == 0) 
            || (pstCIData->u32DataLength % u32BlockSize != 0))
        {
            HI_ERR_CIPHER("Invalid encrypt length, must be multiple of 16 bytes!\r\n");
            return HI_ERR_FAILURE;
        }
    }

    HI_MDM_DataCacheFlush((HI_PVOID)pstCIData->u32SrcAddr, (HI_S32)pstCIData->u32DataLength);
    HI_MDM_DataCacheInvalidate((HI_PVOID)pstCIData->u32DestAddr, (HI_S32)pstCIData->u32DataLength);

    if (0 != softChnId)
    {
        stCITask.stData2Process.u32src = pstCIData->u32SrcAddr;
        stCITask.stData2Process.u32dest = pstCIData->u32DestAddr;
        stCITask.stData2Process.u32length = pstCIData->u32DataLength;
        stCITask.stData2Process.bDecrypt = HI_TRUE;
        stCITask.u32CallBackArg=softChnId;
        HI_INFO_CIPHER("Start to Decrypt, chnNum = %x!\r\n", softChnId);
        g_stCipherOsrChn[softChnId].g_bDataDone = HI_FALSE;
        ret = DRV_CIPHER_CreatTask(softChnId,&stCITask,HI_NULL,HI_NULL);
        if (HI_ERR_SUCCESS != ret)
        {
            HI_ERR_CIPHER("Cipher create task failed!\r\n");
            return ret;
        }
     
        ret = HI_SYS_WaitEvt(EN_SAL_EVENT_CIPHER, HI_FALSE, &event_val, CIPHER_CRYPT_WAIT_TIME_MAX);
        if (ret != HI_ERR_SUCCESS)
        {
            HI_DEBUG_CIPHER("DE HI_SYS_WaitEvt fail : %d\r\n", ret);
            return ret;
        }
        else
        {
            if ((g_stCipherOsrChn[softChnId].g_bDataDone == HI_FALSE) || (0 == (event_val & (1 << softChnId))))
            {
                HI_DEBUG_CIPHER("g_stCipherOsrChn[softChnId].g_bDataDone : %d\r\n", ret);
                return HI_ERR_FAILURE;
            }
        }
        
        HI_INFO_CIPHER("Decrypt OK, chnNum = %x!\r\n", softChnId);
    }
    else
    {
        HI_U32 u32Offset = 0;
        HI_U8 *pu8SrcVir = (HI_U8 *)pstCIData->u32SrcAddr;
        HI_U8 *pu8DestVir = (HI_U8 *)pstCIData->u32DestAddr;

        while(u32Offset < pstCIData->u32DataLength)
        {
            (hi_void)memcpy_s((HI_U8 *)(stCITask.stData2Process.u32DataPkg), 
                        sizeof(stCITask.stData2Process.u32DataPkg), 
                        pu8SrcVir + u32Offset, 
                        u32BlockSize);
            stCITask.stData2Process.u32length = u32BlockSize;
            stCITask.stData2Process.bDecrypt = HI_TRUE;
            stCITask.u32CallBackArg = softChnId;

            ret = DRV_CIPHER_CreatTask(softChnId, &stCITask, HI_NULL, HI_NULL);
            if (HI_ERR_SUCCESS != ret)
            {
                HI_ERR_CIPHER("Cipher create task failed!\r\n");
                break;
            }

            (hi_void)memcpy_s(pu8DestVir + u32Offset, u32BlockSize, (HI_U8 *)(stCITask.stData2Process.u32DataPkg), u32BlockSize);
            u32Offset += u32BlockSize;
        }

    }

    return ret;
}

HI_U32 HI_DRV_CIPHER_Decrypt(CIPHER_DATA_S *pstCIData)
{
    HI_U32 ret = HI_ERR_FAILURE;
    HI_U32 u32TotalSize = 0;
    CIPHER_DATA_S stCIData;

    if ((HI_NULL == pstCIData)  
        ||(HI_NULL == ((HI_VOID *)pstCIData->u32SrcAddr))
        ||(HI_NULL == ((HI_VOID *)pstCIData->u32DestAddr))
        ||(0 == pstCIData->u32DataLength))
    {
        return HI_ERR_INVALID_PARAMETER;
    }

    DRV_CIPHER_LOCK();
    HI_LOW_POWER_Forbid(HI_LOW_POWER_FORBID_ID_CIPHER);
    stCIData.pCIHandle = pstCIData->pCIHandle;   
    while (u32TotalSize < pstCIData->u32DataLength)
    {
        HI_DEBUG_CIPHER("u32TotalSize : %d\r\n", u32TotalSize);
        HI_DEBUG_CIPHER("pstCIData->u32DataLength : %d\r\n", pstCIData->u32DataLength); 
        stCIData.u32DataLength = pstCIData->u32DataLength - u32TotalSize;
        HI_DEBUG_CIPHER("stCIData.u32DataLength : %d\r\n", stCIData.u32DataLength); 
        if (stCIData.u32DataLength > CIPHER_MAX_NODE_BUF_SIZE)
        {
            stCIData.u32DataLength = CIPHER_MAX_NODE_BUF_SIZE;
        }

        stCIData.u32SrcAddr  = pstCIData->u32SrcAddr  + u32TotalSize;
        stCIData.u32DestAddr = pstCIData->u32DestAddr + u32TotalSize;
        u32TotalSize += stCIData.u32DataLength;
        HI_DEBUG_CIPHER("u32TotalSize : %d\r\n", u32TotalSize);
        ret = DRV_CIPHER_Decrypt(&stCIData);
        if (ret != HI_ERR_SUCCESS)
        {
            break;
        }
    }
    HI_LOW_POWER_UnForbid(HI_LOW_POWER_FORBID_ID_CIPHER, 0);
    DRV_CIPHER_UNLOCK();
    return ret;
}


HI_U32 HI_DRV_CIPHER_EncryptMulti(CIPHER_PKG_S *psPkg)
{
	HI_U32 i = 0;
	HI_U32 softChnId = 0;
	static HI_DRV_CIPHER_DATA_INFO_S  tmpData[CIPHER_MAX_LIST_NUM];
	HI_UNF_CIPHER_DATA_S *pTmp = HI_NULL;
	HI_U32 pkgNum = 0;
	HI_U32 ret = HI_ERR_FAILURE;
	HI_U32 event_val = 0;
	
	if ((HI_NULL == psPkg) 
        ||(HI_NULL == psPkg->pu8Pkg))
	{
		HI_ERR_CIPHER("Invalid params!\r\n");
		return HI_ERR_INVALID_PARAMETER;
	}

    softChnId = HI_HANDLE_GET_CHNID(psPkg->pCIHandle);
    CIPHER_CHECK_HANDLE_RETURN_ERR(softChnId);
    if (CIPHER_PKGx1_CHAN == softChnId)
    {
        HI_ERR_CIPHER("CIPHER_PKGx1_CHAN.\r\n");
        return HI_ERR_CIPHER_INVALID_PARA;
    }

    pkgNum = psPkg->u32PkgNum;
    if (pkgNum > CIPHER_MAX_LIST_NUM)
    {
        HI_ERR_CIPHER("Error: you send too many pkg(%d), must < %d.\r\n",pkgNum, CIPHER_MAX_LIST_NUM);
        return HI_ERR_CIPHER_INVALID_PARA;
    }

    if (pkgNum == 0)
    {
        HI_ERR_CIPHER("Error: you send 0 pkg, must >= 1.\r\n");
        return HI_ERR_CIPHER_INVALID_PARA;
    }

    DRV_CIPHER_LOCK();
    HI_LOW_POWER_Forbid(HI_LOW_POWER_FORBID_ID_CIPHER);
    (hi_void)memcpy_s(g_stCipherOsrChn[softChnId].pstDataPkg,(pkgNum * sizeof(HI_UNF_CIPHER_DATA_S)), psPkg->pu8Pkg, pkgNum * sizeof(HI_UNF_CIPHER_DATA_S));

    for (i = 0; i < pkgNum; i++)
    {
        pTmp = g_stCipherOsrChn[softChnId].pstDataPkg + i;
        if((pTmp->u32ByteLength == 0)
            ||((HI_PVOID)pTmp->u32SrcAddr == HI_NULL)
            ||((HI_PVOID)pTmp->u32DestAddr == HI_NULL)
            ||((g_stCipherSoftChans[softChnId].stCtrl.stModeCtrl.enAlg == HI_UNF_CIPHER_ALG_AES) && (pTmp->u32ByteLength % 16 != 0)))
        {
                HI_ERR_CIPHER("Invalid encrypt length, must be multiple of 16 bytes!\r\n");
                HI_LOW_POWER_UnForbid(HI_LOW_POWER_FORBID_ID_CIPHER, 0);
                DRV_CIPHER_UNLOCK();
                return HI_ERR_FAILURE;
            }

        tmpData[i].bDecrypt = HI_FALSE;
        tmpData[i].u32src = pTmp->u32SrcAddr;
        tmpData[i].u32dest = pTmp->u32DestAddr;
        tmpData[i].u32length = pTmp->u32ByteLength;
        HI_MDM_DataCacheFlush((HI_PVOID)pTmp->u32SrcAddr, (HI_S32)pTmp->u32ByteLength);
        HI_MDM_DataCacheInvalidate((HI_PVOID)pTmp->u32DestAddr, (HI_S32)pTmp->u32ByteLength); 
    }

    HI_INFO_CIPHER("Start to DecryptMultiPkg, chnNum = %x, pkgNum=%d!\r\n", softChnId, pkgNum);
    g_stCipherOsrChn[softChnId].g_bDataDone = HI_FALSE;
    ret = DRV_CIPHER_CreatMultiPkgTask(softChnId, tmpData, pkgNum, HI_TRUE, softChnId);
    if (HI_ERR_SUCCESS != ret)
    {
        HI_ERR_CIPHER("Cipher create multi task failed!\r\n");
        HI_LOW_POWER_UnForbid(HI_LOW_POWER_FORBID_ID_CIPHER, 0);
        DRV_CIPHER_UNLOCK();
        return ret;
    }

    ret = HI_SYS_WaitEvt(EN_SAL_EVENT_CIPHER, HI_FALSE, &event_val, CIPHER_CRYPT_WAIT_TIME_MAX);
    if (ret != HI_ERR_SUCCESS)
    {
        HI_DEBUG_CIPHER("HI_SYS_WaitEvt fail : %d\r\n", ret);
        HI_LOW_POWER_UnForbid(HI_LOW_POWER_FORBID_ID_CIPHER, 0);
        DRV_CIPHER_UNLOCK();
        return ret;
    }
    else
    {
        if ((g_stCipherOsrChn[softChnId].g_bDataDone == HI_FALSE) || (0 == (event_val & (1 << softChnId))))
        {
            HI_DEBUG_CIPHER("g_stCipherOsrChn[softChnId].g_bDataDone fail : %d\r\n", g_stCipherOsrChn[softChnId].g_bDataDone);
            HI_LOW_POWER_UnForbid(HI_LOW_POWER_FORBID_ID_CIPHER, 0);
            DRV_CIPHER_UNLOCK();
            return HI_ERR_FAILURE;
        }
    }

    HI_INFO_CIPHER("Decrypt OK, chnNum = %x!\r\n", softChnId);
    HI_LOW_POWER_UnForbid(HI_LOW_POWER_FORBID_ID_CIPHER, 0);
    DRV_CIPHER_UNLOCK();
    return HI_ERR_SUCCESS;
}

HI_U32 HI_DRV_CIPHER_DecryptMulti(CIPHER_PKG_S *psPkg)
{
    HI_U32 i;
    HI_U32 softChnId = 0;
    static HI_DRV_CIPHER_DATA_INFO_S  tmpData[CIPHER_MAX_LIST_NUM];
    HI_UNF_CIPHER_DATA_S *pTmp = HI_NULL;
    HI_U32 pkgNum = 0;
    HI_U32 ret = HI_ERR_FAILURE;
    HI_U32 event_val = 0;

    if ((HI_NULL == psPkg)
        ||(HI_NULL == psPkg->pu8Pkg))
    {
        HI_ERR_CIPHER("Invalid params!\r\n");
        return HI_ERR_CIPHER_INVALID_PARA;
    }
    
    softChnId = HI_HANDLE_GET_CHNID(psPkg->pCIHandle);
    CIPHER_CHECK_HANDLE_RETURN_ERR(softChnId);
    if (CIPHER_PKGx1_CHAN == softChnId)
    {
        HI_ERR_CIPHER("CIPHER_PKGx1_CHAN.\r\n");
        return HI_ERR_CIPHER_INVALID_PARA;
    }
    pkgNum = psPkg->u32PkgNum;
    if (pkgNum > CIPHER_MAX_LIST_NUM)
    {
        HI_ERR_CIPHER("Error: you send too many pkg(%d), must < %d.\r\n",pkgNum, CIPHER_MAX_LIST_NUM);
        return HI_ERR_CIPHER_INVALID_PARA;
    }
    if (pkgNum == 0)
    {
        HI_ERR_CIPHER("Error: you send 0 pkg, must >= 1.\r\n");
        return HI_ERR_CIPHER_INVALID_PARA;
    }

    DRV_CIPHER_LOCK();
    HI_LOW_POWER_Forbid(HI_LOW_POWER_FORBID_ID_CIPHER);
    (hi_void)memcpy_s(g_stCipherOsrChn[softChnId].pstDataPkg,(pkgNum * sizeof(HI_UNF_CIPHER_DATA_S)), psPkg->pu8Pkg, pkgNum * sizeof(HI_UNF_CIPHER_DATA_S));
    for (i = 0; i < pkgNum; i++)
    {
        pTmp = g_stCipherOsrChn[softChnId].pstDataPkg + i;
        if((pTmp->u32ByteLength == 0)
            ||((HI_PVOID)pTmp->u32SrcAddr == HI_NULL)
            ||((HI_PVOID)pTmp->u32DestAddr == HI_NULL)
            ||((g_stCipherSoftChans[softChnId].stCtrl.stModeCtrl.enAlg == HI_UNF_CIPHER_ALG_AES) && (pTmp->u32ByteLength % 16 != 0)))
        {

                HI_ERR_CIPHER("Invalid encrypt length!\r\n");
                HI_LOW_POWER_UnForbid(HI_LOW_POWER_FORBID_ID_CIPHER, 0);
                DRV_CIPHER_UNLOCK();
                return HI_ERR_FAILURE;
            }
        
        tmpData[i].bDecrypt = HI_TRUE;
        tmpData[i].u32src = pTmp->u32SrcAddr;
        tmpData[i].u32dest = pTmp->u32DestAddr;
        tmpData[i].u32length = pTmp->u32ByteLength;
        HI_MDM_DataCacheFlush((HI_PVOID)pTmp->u32SrcAddr, (HI_S32)pTmp->u32ByteLength);
        HI_MDM_DataCacheInvalidate((HI_PVOID)pTmp->u32DestAddr, (HI_S32)pTmp->u32ByteLength); 
    }

    HI_INFO_CIPHER("Start to DecryptMultiPkg, chnNum = %x, pkgNum=%d!\r\n", softChnId, pkgNum);
    g_stCipherOsrChn[softChnId].g_bDataDone = HI_FALSE;
    ret = DRV_CIPHER_CreatMultiPkgTask(softChnId, tmpData, pkgNum, HI_TRUE, softChnId);
    if (HI_ERR_SUCCESS != ret)
    {
        HI_ERR_CIPHER("Cipher create multi task failed!\r\n");
        HI_LOW_POWER_UnForbid(HI_LOW_POWER_FORBID_ID_CIPHER, 0);
        DRV_CIPHER_UNLOCK();
        return ret;
    }

    ret = HI_SYS_WaitEvt(EN_SAL_EVENT_CIPHER, HI_FALSE, &event_val, CIPHER_CRYPT_WAIT_TIME_MAX);
    if (ret != HI_ERR_SUCCESS)
    {
        HI_DEBUG_CIPHER("HI_SYS_WaitEvt fail : %d\r\n", ret);
        HI_LOW_POWER_UnForbid(HI_LOW_POWER_FORBID_ID_CIPHER, 0);
        DRV_CIPHER_UNLOCK();
        return ret;
    }
    else
    {
        if ((g_stCipherOsrChn[softChnId].g_bDataDone == HI_FALSE)||(0 == (event_val & (1 << softChnId))))
        {
            HI_DEBUG_CIPHER("g_stCipherOsrChn[softChnId].g_bDataDone fail : %d\r\n", g_stCipherOsrChn[softChnId].g_bDataDone);
            HI_LOW_POWER_UnForbid(HI_LOW_POWER_FORBID_ID_CIPHER, 0);
            DRV_CIPHER_UNLOCK();
            return HI_ERR_FAILURE;
        }
    }
    
    HI_INFO_CIPHER("Decrypt OK, chnNum = %x!\r\n", softChnId);
    HI_LOW_POWER_UnForbid(HI_LOW_POWER_FORBID_ID_CIPHER, 0);
    DRV_CIPHER_UNLOCK();
    return HI_ERR_SUCCESS;
}

HI_PRV HI_U32 DRV_RANDOM_GetNumber(HI_IN HI_U32 u32TimeOutUs, HI_OUT HI_U32 *pu32Rng)
{
    return HAL_Random_GetNumber(u32TimeOutUs, pu32Rng);
}




HI_U32 HI_MDM_GetRealRandom(HI_U32 ulMinVal,HI_U32 ulMaxVal,HI_OUT HI_U32 *pulValue)
{
    HI_U32 tempVal=0;
    HI_U32 ret=HI_ERR_SUCCESS;

    if(!pulValue)
    {
        return HI_ERR_INVALID_PARAMETER;
    }

    //上下边界一致，不用随机
    if(ulMinVal==ulMaxVal)
    {
        *pulValue=ulMinVal;
        return HI_ERR_SUCCESS;
    }

    //下边界大于上边界，返回错误
    if(ulMinVal>ulMaxVal)
    {
        *pulValue=ulMinVal;
        return HI_ERR_INVALID_PARAMETER;
    }

    ret=HI_DRV_RANDOM_GetNumber(1000000,&tempVal);
    if(ret==HI_ERR_SUCCESS)
    {
        if(ulMaxVal-ulMinVal==HI_ALL_F_32)//HI_ALL_F_32+1越界，特殊处理
        {
            *pulValue=tempVal;
            return HI_ERR_SUCCESS;
        }
        else
        {
            *pulValue=ulMinVal+(tempVal%(ulMaxVal+1-ulMinVal));
            return HI_ERR_SUCCESS;
        }
    }
    else
    {
        *pulValue=ulMinVal;
        return ret;
    }
        
}

HI_U32 HI_DRV_RANDOM_GetNumber(HI_IN HI_U32 u32TimeOutUs, HI_OUT HI_U32 *pu32Rng)
{
    HI_U32 ret = HI_ERR_FAILURE;

    if(HI_NULL == pu32Rng)
    {
        HI_ERR_CIPHER("Invalid params!\r\n");
        return HI_ERR_INVALID_PARAMETER;
    }
    HI_MDM_SYS_CLOCK_Enable(HI_SYSCTRL_CLKEN_REG,HI_SYSCTRL_CLKEN_REG_BIT_SHIFT_RNG);
    HI_LOW_POWER_Forbid(HI_LOW_POWER_FORBID_ID_TRNG);  
    ret = DRV_RANDOM_GetNumber(u32TimeOutUs, pu32Rng);
    HI_LOW_POWER_UnForbid(HI_LOW_POWER_FORBID_ID_TRNG, 0);
    HI_MDM_SYS_CLOCK_Disable(HI_SYSCTRL_CLKEN_REG,HI_SYSCTRL_CLKEN_REG_BIT_SHIFT_RNG);
    return ret;
}

HI_PRV HI_U32 Hash_CalcInit(HASH_DATA_S *pHashData)
{
    if(HI_NULL == pHashData)
    {
        HI_ERR_CIPHER("Error, Null pointer input!\r\n");
        return HI_ERR_FAILURE;
    }

    {
        return HI_ERR_SUCCESS;
    }
}

HI_PRV HI_U32 Hash_CalcUpdate(HASH_DATA_S *pHashData)
{
    HI_U32 ret = HI_ERR_FAILURE;

    if(HI_NULL == pHashData)
    {
        HI_ERR_CIPHER("Error, Null pointer input!\r\n");
        return HI_ERR_FAILURE;
    }

    {
        ret = HAL_Hash_CalcInit(pHashData);
        if(HI_ERR_SUCCESS != ret)
        {
            HI_ERR_CIPHER("HAL_Hash_Init failed! ret = 0x%08x\r\n", ret);
            return HI_ERR_FAILURE;
        }
        
        ret = HAL_Hash_CalcUpdate(pHashData);
        if(HI_ERR_SUCCESS != ret)
        {
            HI_ERR_CIPHER("HAL_Hash_Update failed! ret = 0x%08x\r\n", ret);
            return HI_ERR_FAILURE;
        }
        
        ret = HAL_Hash_CalcFinal(pHashData);
        if(HI_ERR_SUCCESS != ret)
        {
            HI_ERR_CIPHER("HAL_Hash_Final failed! ret = 0x%08x\r\n", ret);
            return HI_ERR_FAILURE;
        }

    }

    return ret;
}

HI_PRV HI_U32 Hash_CalcFinal(HASH_DATA_S *pHashData)
{
    HI_U32 ret = HI_ERR_FAILURE;

    if(HI_NULL == pHashData)
    {
        HI_ERR_CIPHER("Error, Null pointer input!\r\n");
        return HI_ERR_FAILURE;
    }
    {
        ret = Hash_CalcUpdate(pHashData);
        if(HI_ERR_FAILURE == ret)
        {
            HI_ERR_CIPHER("Cipher hash final failed!\r\n");
            return HI_ERR_FAILURE;
        }
    }

    return ret;
}

HI_U32 HI_DRV_HASH_CalcInit(HASH_DATA_S *pHashData)
{
    HI_U32 ret = HI_ERR_SUCCESS;

    if( HI_NULL == pHashData )
    {
        HI_ERR_CIPHER("Error, Null pointer input!\r\n");
        return HI_ERR_FAILURE;
    }
 
    DRV_HASH_LOCK();
    ret = Hash_CalcInit(pHashData);

    DRV_HASH_UNLOCK();
    return ret;
}

HI_U32 HI_DRV_HASH_CalcUpdate(HASH_DATA_S *pHashData)
{
    HI_U32 s32Ret = HI_ERR_SUCCESS;
 
    DRV_HASH_LOCK();
    s32Ret = Hash_CalcUpdate(pHashData);
    DRV_HASH_UNLOCK();
    return s32Ret;
}

HI_U32 HI_DRV_HASH_CalcFinal(HASH_DATA_S *pHashData)
{
    HI_U32 s32Ret = HI_ERR_SUCCESS;
 
    DRV_HASH_LOCK();
    s32Ret = Hash_CalcFinal(pHashData);
    DRV_HASH_UNLOCK();
    return s32Ret;
}


HI_PRV HI_U32 DRV_RSA_CheckData(HI_U8 *N, HI_U8 *E, HI_U8 *MC, HI_U32 u32Len)
{
    HI_U32 i;

    /*MC > 0*/
    for(i=0; i<u32Len; i++)
    {
        if(MC[i] > 0)
        {
            break;
        }
    }
    if(i>=u32Len)
    {
        HI_ERR_CIPHER("RSA M/C is zero, error!\r\n");
        return HI_ERR_INVALID_PARAMETER; 
    }

    /*MC < N*/
    for(i=0; i<u32Len; i++)
    {
        if(MC[i] < N[i])
        {
            break;
        }
    }
    if(i>=u32Len)
    {
        HI_ERR_CIPHER("RSA M/C is larger than N, error!\r\n");
        return HI_ERR_INVALID_PARAMETER; 
    }

    /*E > 1*/
    for(i=0; i<u32Len; i++)
    {
        if(E[i] > 0)
        {
            break;
        }
    }
    if(i>=u32Len)
    {
        HI_ERR_CIPHER("RSA D/E is zero, error!\r\n");
        return HI_ERR_INVALID_PARAMETER; 
    }
    
    return HI_ERR_SUCCESS;
}

HI_U32 DRV_RSA_ClearRam(HI_VOID)
{
    HI_U32 ret = HI_ERR_SUCCESS;
    
    ret = HAL_RSA_WaitFree();
    if( HI_ERR_SUCCESS != ret )
    {
        HI_ERR_CIPHER("RSA is busy and timeout,error!\r\n");
        return HI_ERR_FAILURE;
    }
    
    HAL_RSA_ClearRam();
    HAL_RSA_Start();
    ret = HAL_RSA_WaitFree();
    if( HI_ERR_SUCCESS != ret )
    {
        HI_ERR_CIPHER("RSA is busy and timeout,error!\r\n");
        return HI_ERR_FAILURE;
    }

    return HI_ERR_SUCCESS;
}

HI_U32 DRV_RSA_CalcCrc(HI_U16 *pu16Crc)
{
    HI_U32 ret = HI_ERR_SUCCESS;
    HI_U32 u32Val;
    
    ret = HAL_RSA_WaitFree();
    if( HI_ERR_SUCCESS != ret )
    {
        HI_ERR_CIPHER("RSA is busy and timeout,error!\r\n");
        return HI_ERR_FAILURE;
    }
    HAL_RSA_Crc();
    HAL_RSA_Start();
    ret = HAL_RSA_WaitFree();
    if( HI_ERR_SUCCESS != ret )
    {
        HI_ERR_CIPHER("RSA is busy and timeout,error!\r\n");
        return HI_ERR_FAILURE;
    }

    HAL_RSA_GetCrc(&u32Val);

    *pu16Crc = (HI_U16)u32Val;

    return HI_ERR_SUCCESS;
}
HI_U32 DRV_RSA_Calc_ex(RSA_DATA_S *pRsaData)
{
    HI_U32 ret = HI_ERR_SUCCESS;
    HI_U32 u32ErrorCode;
    HI_U32 u32KeyLen;
    CIPHER_RSA_KEY_WIDTH_E enKeyWidth;

    if ((pRsaData->pu8Input == HI_NULL) ||(pRsaData->pu8Output== HI_NULL)
        || (pRsaData->pu8N == HI_NULL) || (pRsaData->pu8K == HI_NULL)) 
    {
        HI_ERR_CIPHER("para is null.\r\n");
        return HI_ERR_INVALID_PARAMETER;
    }
    
    if ((pRsaData->u16NLen == 0) || (pRsaData->u16KLen == 0)) 
    {
        HI_ERR_CIPHER("RSA K size is zero.\r\n");
        return HI_ERR_INVALID_PARAMETER;
    }

    if(pRsaData->u32DataLen != pRsaData->u16NLen)
    {
        HI_ERR_CIPHER("Error, DataLen != u16NLen!\r\n");
        return HI_ERR_INVALID_PARAMETER; 
    }
    
    if (pRsaData->u16NLen <= 128)
    {
        u32KeyLen = 128;
        enKeyWidth = CIPHER_RSA_KEY_WIDTH_1K;
    }
    else if (pRsaData->u16NLen <= 256)
    {
        u32KeyLen = 256;
        enKeyWidth = CIPHER_RSA_KEY_WIDTH_2K;
    }
    else if (pRsaData->u16NLen <= 512)
    {
        u32KeyLen = 512;
        enKeyWidth = CIPHER_RSA_KEY_WIDTH_4K;
    }
    else 
    {
        HI_ERR_CIPHER("u16NLen(0x%x) is invalid\r\n", pRsaData->u16NLen);
        return HI_ERR_INVALID_PARAMETER;
    }

    ret = DRV_RSA_CheckData(pRsaData->pu8N, pRsaData->pu8K, pRsaData->pu8Input, u32KeyLen);
    if( HI_ERR_SUCCESS != ret )
    {
        HI_ERR_CIPHER("RSA data invalid!\r\n");
        return HI_ERR_INVALID_PARAMETER;
    }

    ret = DRV_RSA_ClearRam();
    if( HI_ERR_SUCCESS != ret )
    {
        HI_ERR_CIPHER("RSA clear ram error!\r\n");
        return HI_ERR_FAILURE;
    }

    /*Config Mode*/
    HAL_RSA_ConfigMode(enKeyWidth);

    /*Write N,E,M*/
    HAL_RSA_WriteData(CIPHER_RSA_DATA_TYPE_MODULE, pRsaData->pu8N, pRsaData->u16NLen, u32KeyLen);
    HAL_RSA_WriteData(CIPHER_RSA_DATA_TYPE_KEY, pRsaData->pu8K, pRsaData->u16NLen, u32KeyLen);
    HAL_RSA_WriteData(CIPHER_RSA_DATA_TYPE_CONTEXT, pRsaData->pu8Input, pRsaData->u16NLen, u32KeyLen);

    /*Sart*/
    HAL_RSA_Start();
    ret = HAL_RSA_WaitFree();
    if( HI_ERR_SUCCESS != ret )
    {
        HI_ERR_CIPHER("RSA is busy and timeout,error!\r\n");
        return HI_ERR_FAILURE;
    }
    u32ErrorCode = HAL_RSA_GetErrorCode();
    if( 0 != u32ErrorCode )
    {
        HI_ERR_CIPHER("HAL_RSA_GetErrorCode err: 0x%x!\r\n", u32ErrorCode);
        return HI_ERR_FAILURE;
    }
  
    /*Get result*/
    HAL_RSA_ReadData(pRsaData->pu8Output, pRsaData->u16NLen, u32KeyLen);

    ret = DRV_RSA_ClearRam();
    if( HI_ERR_SUCCESS != ret )
    {
        HI_ERR_CIPHER("RSA clear ram error!\r\n");
        return HI_ERR_FAILURE;
    }
    
    return HI_ERR_SUCCESS; 
}

HI_PRV HI_U32 DRV_RSA_Calc(RSA_DATA_S *pRsaData)
{
    HI_U8 *pN = HI_NULL;
    HI_U8 *pK = HI_NULL;
    HI_U8 *pM = HI_NULL;
    HI_U32 ret = HI_ERR_FAILURE;
    HI_U16 u32KeyLen;
    RSA_DATA_S stRsaData;
    HI_U8 *p =HI_NULL;

    if(pRsaData == HI_NULL)
    {
        HI_ERR_CIPHER("Invalid params!\r\n");
        return HI_ERR_INVALID_PARAMETER;
    }

    if ((pRsaData->pu8Input == HI_NULL) ||(pRsaData->pu8Output== HI_NULL)
        || (pRsaData->pu8N == HI_NULL) || (pRsaData->pu8K == HI_NULL)) 
    {
        HI_ERR_CIPHER("para is null.\r\n");
        return HI_ERR_INVALID_PARAMETER;
    }

    if(pRsaData->u32DataLen != pRsaData->u16NLen)
    {
        HI_ERR_CIPHER("Error, DataLen != u16NLen!\r\n");
        return HI_ERR_INVALID_PARAMETER; 
    }

    if(pRsaData->u16KLen > pRsaData->u16NLen)
    {
        HI_ERR_CIPHER("Error, KLen > u16NLen!\r\n");
        return HI_ERR_INVALID_PARAMETER; 
    }
    
    pN = (HI_U8 *)HI_MDM_Malloc(HI_MOD_ID_DRV_CIPHER, CIPHER_MAX_RSA_KEY_LEN);
    if (HI_NULL == pN)
    {
        return HI_ERR_MALLOC_FAILUE;
    }

    (hi_void)memset_s((HI_VOID *)pN, CIPHER_MAX_RSA_KEY_LEN, 0, CIPHER_MAX_RSA_KEY_LEN);

    pK = (HI_U8 *)HI_MDM_Malloc(HI_MOD_ID_DRV_CIPHER, CIPHER_MAX_RSA_KEY_LEN);
    if (HI_NULL == pK)
    {
        HI_MDM_Free(HI_MOD_ID_DRV_CIPHER, pN);
        return HI_ERR_MALLOC_FAILUE;
    }

    (hi_void)memset_s((HI_VOID *)pK, CIPHER_MAX_RSA_KEY_LEN, 0, CIPHER_MAX_RSA_KEY_LEN);

    pM = (HI_U8 *)HI_MDM_Malloc(HI_MOD_ID_DRV_CIPHER, CIPHER_MAX_RSA_KEY_LEN);
    if (HI_NULL == pM)
    {
        HI_MDM_Free(HI_MOD_ID_DRV_CIPHER, pN);
        HI_MDM_Free(HI_MOD_ID_DRV_CIPHER, pK);
        return HI_ERR_MALLOC_FAILUE;
    }

    (hi_void)memset_s((HI_VOID *)pM, CIPHER_MAX_RSA_KEY_LEN, 0, CIPHER_MAX_RSA_KEY_LEN);

    /*Only support the key width of 1024,2048 and 4096*/
    if (pRsaData->u16NLen <= 128)
    {
        u32KeyLen = 128;
    }
    else if (pRsaData->u16NLen <= 256)
    {
        u32KeyLen = 256;
    }
    else if (pRsaData->u16NLen <= 512)
    {
        u32KeyLen = 512;
    }
    else 
    {
        HI_MDM_Free(HI_MOD_ID_DRV_CIPHER, pN);
        HI_MDM_Free(HI_MOD_ID_DRV_CIPHER, pK);
        HI_MDM_Free(HI_MOD_ID_DRV_CIPHER, pM);
        return HI_ERR_INVALID_PARAMETER;
    }

    /*if dataLen < u32KeyLen, padding 0 before data*/
    p = pN + (u32KeyLen - pRsaData->u16NLen);
    (hi_void)memcpy_s(p, pRsaData->u16NLen, pRsaData->pu8N, pRsaData->u16NLen);
    
    p = pK + (u32KeyLen - pRsaData->u16KLen);
    (hi_void)memcpy_s(p, pRsaData->u16KLen, pRsaData->pu8K, pRsaData->u16KLen);
    
    p = pM + (u32KeyLen - pRsaData->u32DataLen);
    (hi_void)memcpy_s(p, pRsaData->u32DataLen, pRsaData->pu8Input, pRsaData->u32DataLen);   
    
    stRsaData.pu8N = pN;
    stRsaData.pu8K = pK;
    stRsaData.pu8Input = pM;
    stRsaData.u16NLen = u32KeyLen;
    stRsaData.u16KLen = u32KeyLen;
    stRsaData.u32DataLen = u32KeyLen;
    stRsaData.pu8Output = pM;
    
    ret = DRV_RSA_Calc_ex(&stRsaData);
    HI_MDM_Free(HI_MOD_ID_DRV_CIPHER, pN);
    HI_MDM_Free(HI_MOD_ID_DRV_CIPHER, pK);
    if( HI_ERR_SUCCESS != ret )
    {
        HI_MDM_Free(HI_MOD_ID_DRV_CIPHER, pM);
        return HI_ERR_FAILURE;
    }
    
    (hi_void)memcpy_s(pRsaData->pu8Output, pRsaData->u16NLen, pM + (u32KeyLen - pRsaData->u16NLen), pRsaData->u16NLen);
    HI_MDM_Free(HI_MOD_ID_DRV_CIPHER, pM);
    return ret;  
}

HI_U32 HI_DRV_RSA_Calc(RSA_DATA_S *pRsaData)
{
    HI_U32 ret = HI_ERR_SUCCESS;

    if(pRsaData == HI_NULL)
    {
        HI_ERR_CIPHER("Invalid params!\r\n");
        return HI_ERR_INVALID_PARAMETER;
    }

    DRV_RSA_LOCK();
    HI_LOW_POWER_Forbid(HI_LOW_POWER_FORBID_ID_RSA);
    ret = DRV_RSA_Calc(pRsaData);

    HI_LOW_POWER_UnForbid(HI_LOW_POWER_FORBID_ID_RSA, 0);
    DRV_RSA_UNLOCK();
    return ret;  
}

HI_U32 HI_MDM_CIPHER_Init(HI_VOID)
{
    return HI_DRV_CIPHER_Init();
}

HI_U32 HI_MDM_CIPHER_DeInit(HI_VOID)
{
    return HI_DRV_CIPHER_DeInit();
}

HI_U32 HI_MDM_CIPHER_CreateHandle(HI_HANDLE pHandle, HI_CIPHER_TYPE_E enCipherType)
{
    return HI_DRV_CIPHER_CreateHandle(pHandle, enCipherType);
}

HI_U32 HI_MDM_CIPHER_GetConfig(HI_HANDLE pHandle, HI_CIPHER_CTRL_S *pstCipherConfig)
{
    return HI_DRV_CIPHER_GetHandleConfig(pHandle, pstCipherConfig);
}

HI_U32 HI_MDM_CIPHER_DestroyHandle(HI_HANDLE pHandle)
{
    return HI_DRV_CIPHER_DestroyHandle(pHandle);
}

HI_U32 HI_MDM_CIPHER_Encrypt(CIPHER_DATA_S *pstCIData)
{
    return HI_DRV_CIPHER_Encrypt(pstCIData);
}

HI_U32 HI_MDM_CIPHER_Decrypt(CIPHER_DATA_S *pstCIData)
{
    return HI_DRV_CIPHER_Decrypt(pstCIData);
}

HI_U32 HI_MDM_CIPHER_EncryptMulti(CIPHER_PKG_S *psPkg)
{
    return HI_DRV_CIPHER_EncryptMulti(psPkg);
}
    
HI_U32 HI_MDM_CIPHER_DecryptMulti(CIPHER_PKG_S *psPkg)
{
    return HI_DRV_CIPHER_DecryptMulti(psPkg);
}

HI_U32 HI_MDM_CIPHER_ConfigAES(HI_HANDLE pHandle,  HI_CIPHER_CTRL_S *pConfig)
{
    HI_U32 ulRet = HI_ERR_FAILURE;

    if (!pConfig)
    {
        return HI_ERR_INVALID_PARAMETER;
    }

    ulRet = DRV_CipherCheckAes(&(pConfig->stModeCtrl));
    if (ulRet)
    {
        return ulRet;
    }
    
    return HI_DRV_CIPHER_ConfigChn(pHandle, pConfig);
}

HI_U32 HI_MDM_CIPHER_Config3DES(HI_HANDLE pHandle,  HI_CIPHER_CTRL_S *pConfig)
{
    HI_U32 ulRet = HI_ERR_FAILURE;

    if (!pConfig)
    {
        return HI_ERR_INVALID_PARAMETER;
    }    
    
    ulRet = DRV_CipherCheck3Des(&(pConfig->stModeCtrl));
    if (ulRet)
    {
        return ulRet;
    }
    
    return HI_DRV_CIPHER_ConfigChn(pHandle, pConfig);
}


