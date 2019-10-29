/******************************************************************************

  Copyright (C), 2011-2014, Hisilicon Tech. Co., Ltd.

 ******************************************************************************
  File Name     : hal_cipher.c
  Version       : Initial Draft
  Author        : Hisilicon hisecurity team
  Created       : 
  Last Modified :
  Description   : 
  Function List :
  History       :
******************************************************************************/
#include "drv_cipher_reg.h"
#include "hal_cipher.h"
#include "hi_drv_cipher.h"
#include <hi_sal.h>
#include "hal_key_ctrl.h"
#include <os_vx_sal_api.h>
#include <hi_drv_com.h>
//#include <hi-early-debug.h>
/* Set the defualt timeout value for hash calculating (5000 ms)*/
#define HASH_MAX_DURATION (5000000)
#define HASH_TACK_CUP_MAX_DURATION (1000)

#define DEFAULT_HASH_KEY_AERA EFUSE_AERA1

#ifdef TEST_EFUSE_4_KEY
HI_U32 g_hash_hmac_hw_key_times = 0;
HI_U32 g_cipher_chan1_hw_key_times = 0;
#endif
HI_U32 HAL_Cipher_HashEnable(HI_VOID);
HI_PRV HI_U32 *HAL_Cipher_RegChanNIbufNum(HI_U32 chnId);
HI_PRV HI_U32 *HAL_Cipher_RegChanNIbufCnt(HI_U32 chnId);
HI_PRV HI_U32 *HAL_Cipher_RegChanNIemptyCnt(HI_U32 chnId);
HI_PRV HI_U32 *HAL_Cipher_RegChanNObufNum(HI_U32 chnId);
HI_PRV HI_U32 *HAL_Cipher_RegChanNObufCnt(HI_U32 chnId);
HI_PRV HI_U32 *HAL_Cipher_RegChanNOFullCnt(HI_U32 chnId);
HI_PRV HI_U32 *HAL_Cipher_RegChanNSrcLstRaddr(HI_U32 chnId);
HI_PRV HI_U32 *HAL_Cipher_RegChanNDestLstRaddr(HI_U32 chnId);
/***************************** Macro Definition ******************************/
/*process of bit*/
#define HAL_SET_BIT(src, bit)        ((src) |= (1<<bit))
#define HAL_CLEAR_BIT(src,bit)       ((src) &= ~(1<<bit))
#ifdef CIPHER_MHASH_SUPPORT
#define MAX_HASH_USER  32
#else
#define MAX_HASH_USER  1
#endif
static HI_U32 s_bIsHashUser = 0;

#define RSA_DATA_CLR            (7<<4)
#define RSA_DATA_CLR_KEY        (1<<4)
#define RSA_DATA_CLR_INPUT      (2<<4)
#define RSA_DATA_CLR_OUTPUT     (4<<4)
#define RSA_MOD_SEL		        (3 << 0)
#define RSA_MOD_SEL_OPT	        (0 << 0)
#define RSA_MOD_SEL_KEY_UPDATA	(1 << 0)
#define RSA_MOD_SEL_RAM_CLAER	(2 << 0)
#define RSA_MOD_SEL_CRC16	    (3 << 0)
#define RSA_BUSY		        (1 << 0)
#define RSA_START		        (1 << 0)
#define RSA_RTY_CNT             (50000)

#define CIPHER_ID_REG_CHANn_IBUF_NUM (1)
#define CIPHER_ID_REG_CHANn_IBUF_CNT (2)
#define CIPHER_ID_REG_CHANn_IEMPTY_CNT (3)
#define CIPHER_ID_REG_CHANn_OBUF_NUM (4)
#define CIPHER_ID_REG_CHANn_OBUF_CNT (5)
#define CIPHER_ID_REG_CHANn_OFULL_CNT (6)
#define CIPHER_ID_REG_CHANn_SRC_LST_RADDR (7)
#define CIPHER_ID_REG_CHANn_DEST_LST_RADDR (8)
//#define CIPHER_ID_REG_CHAN0_CIPHER_DIN (7)
//#define CIPHER_ID_REG_CHAN0_CIPHER_IVIN (8)
/*************************** Structure Definition ****************************/
typedef enum
{
    HASH_READY,
    REC_READY,
    DMA_READY,
}HASH_WAIT_TYPE;

typedef HI_U32* (*PFUN_CIPHER_REG_PROC)(HI_U32 chnId);

typedef struct
{
    HI_U32 ulIndex;// 处理索引
    PFUN_CIPHER_REG_PROC pCipherFun;// 应用函数
} CIPHER_REG_FUNC_TABLE_STRU;

HI_CONST CIPHER_REG_FUNC_TABLE_STRU g_stCipherRegTable[] = 
{
    {CIPHER_ID_REG_CHANn_IBUF_NUM, HAL_Cipher_RegChanNIbufNum},
    {CIPHER_ID_REG_CHANn_IBUF_CNT, HAL_Cipher_RegChanNIbufCnt},
    {CIPHER_ID_REG_CHANn_IEMPTY_CNT, HAL_Cipher_RegChanNIemptyCnt},
    {CIPHER_ID_REG_CHANn_OBUF_NUM, HAL_Cipher_RegChanNObufNum},
    {CIPHER_ID_REG_CHANn_OBUF_CNT, HAL_Cipher_RegChanNObufCnt},
    {CIPHER_ID_REG_CHANn_OFULL_CNT, HAL_Cipher_RegChanNOFullCnt},
    {CIPHER_ID_REG_CHANn_SRC_LST_RADDR, HAL_Cipher_RegChanNSrcLstRaddr},
    {CIPHER_ID_REG_CHANn_DEST_LST_RADDR, HAL_Cipher_RegChanNDestLstRaddr},
    //{CIPHER_ID_REG_CHAN0_CIPHER_DIN, HAL_Cipher_RegChan0CipherDin},
    //{CIPHER_ID_REG_CHAN0_CIPHER_IVIN, HAL_Cipher_RegChan0CipherIvin},
};

HI_PRV HI_U32 *HAL_Cipher_RegChanNIbufNum(HI_U32 chnId)
{
    return (HI_U32 *)CIPHER_REG_CHANn_IBUF_NUM(chnId);
}

HI_PRV HI_U32 *HAL_Cipher_RegChanNIbufCnt(HI_U32 chnId)
{
    return (HI_U32 *)CIPHER_REG_CHANn_IBUF_CNT(chnId);
}

HI_PRV HI_U32 *HAL_Cipher_RegChanNIemptyCnt(HI_U32 chnId)
{
    return (HI_U32 *)CIPHER_REG_CHANn_IEMPTY_CNT(chnId);
}

HI_PRV HI_U32 *HAL_Cipher_RegChanNObufNum(HI_U32 chnId)
{
    return (HI_U32 *)CIPHER_REG_CHANn_OBUF_NUM(chnId);
}

HI_PRV HI_U32 *HAL_Cipher_RegChanNObufCnt(HI_U32 chnId)
{
    return (HI_U32 *)CIPHER_REG_CHANn_OBUF_CNT(chnId);
}

HI_PRV HI_U32 *HAL_Cipher_RegChanNOFullCnt(HI_U32 chnId)
{
    return (HI_U32 *)CIPHER_REG_CHANn_OFULL_CNT(chnId);
}

HI_PRV HI_U32 *HAL_Cipher_RegChanNSrcLstRaddr(HI_U32 chnId)
{
    return (HI_U32 *)CIPHER_REG_CHANn_SRC_LST_RADDR(chnId);
}
HI_PRV HI_U32 *HAL_Cipher_RegChanNDestLstRaddr(HI_U32 chnId)
{
    return (HI_U32 *)CIPHER_REG_CHANn_DEST_LST_RADDR(chnId);
}



/******************************* API declaration *****************************/

// 得到初步解析处理函数
PFUN_CIPHER_REG_PROC HAL_Cipher_RegFun(HI_U32 ulIndex)
{
    HI_U32 i = 0;
    
    for(i = 0; i < (sizeof(g_stCipherRegTable)/sizeof(g_stCipherRegTable[0])); i++)
    {
        if(g_stCipherRegTable[i].ulIndex == ulIndex)
        {
            return g_stCipherRegTable[i].pCipherFun;
        }
    }

    return HI_NULL;
}


HI_VOID HAL_CIPHER_ReadReg(HI_U32* addr, HI_U32 *pu32Val)
{
    HI_REG_READ32(addr, *pu32Val);
}

HI_VOID HAL_CIPHER_WriteReg(HI_U32* addr, HI_U32 u32Val)
{
    HI_REG_WRITE32(addr, u32Val);
}

HI_U32 HAL_Cipher_SetReg(HI_U32 ulIndex, HI_U32 ulChnId, HI_U32 ulNum)
{
    HI_U32 *regAddr = 0;
    PFUN_CIPHER_REG_PROC pFunCipherReg = HI_NULL;
    if ((ulChnId < CIPHER_PKGxN_CHAN_MIN) || (ulChnId > CIPHER_PKGxN_CHAN_MAX))
    {
        return HI_ERR_INVALID_PARAMETER;
    }

    if (ulNum > 0xffff)
    {
        HI_ERR_CIPHER("value err:%x, set to 0xffff\r\n", ulNum);
        ulNum = 0xffff;
    }
    pFunCipherReg = HAL_Cipher_RegFun(ulIndex);
    if (!pFunCipherReg)
    {
        return HI_ERR_FAILURE;
    }
    
    regAddr = pFunCipherReg(ulChnId);
    (HI_VOID)HAL_CIPHER_WriteReg(regAddr, ulNum);
    HI_INFO_CIPHER("HAL_Cipher_SetInBufCnt = %d\r\n", ulNum);
    return HI_ERR_SUCCESS;
}

HI_U32 HAL_Cipher_GetReg(HI_U32 ulIndex, HI_U32 ulChnId, HI_U32 *pNum)
{
    HI_U32 *regAddr = 0;
    HI_U32 regValue = 0;
    PFUN_CIPHER_REG_PROC pFunCipherReg = HI_NULL;

    if (CIPHER_PKGx1_CHAN == ulChnId || HI_NULL == pNum)
    {
        return HI_ERR_INVALID_PARAMETER;
    }
    
    pFunCipherReg = HAL_Cipher_RegFun(ulIndex);
    if (!pFunCipherReg)
    {
        return HI_ERR_FAILURE;
    }
    
    regAddr = pFunCipherReg(ulChnId);
    (HI_VOID)HAL_CIPHER_ReadReg(regAddr, &regValue);
    *pNum = regValue;
    HI_INFO_CIPHER(" regValue = %d\r\n", regValue);    
    return HI_ERR_SUCCESS;
}

HI_U32 HASH_WaitReady(HASH_WAIT_TYPE enType)
{
    CIPHER_SHA_STATUS_U unCipherSHAstatus;
    HI_U32 ulStartTime = 0;
    HI_U32 ulLastTime = 0;
    HI_U32 ulDuraTime = 0;

    /* wait for hash_rdy */
    HI_TIME_CNT_GetUs(&ulStartTime);
    for(;;)
    {
        unCipherSHAstatus.u32 = 0;
        (HI_VOID)HAL_CIPHER_ReadReg((HI_U32 *)CIPHER_HASH_REG_STATUS_ADDR, &unCipherSHAstatus.u32);
        if(HASH_READY == enType)
        {
            if(1 == unCipherSHAstatus.bits.hash_rdy)
            {
                break;
            }
        }
        else if (REC_READY == enType)
        {
            if(1 == unCipherSHAstatus.bits.rec_rdy)
            {
                break;
            }
        }
        else if (DMA_READY == enType)
        {
/*
                            【屏蔽人】z00193780
                            【审核人】c00233580
                            【代码分析】二次开发接口，客户有可能使用该分支，需保留。
                            【功能影响】不影响正常业务，屏蔽告警
*/
            if(1 == unCipherSHAstatus.bits.dma_rdy)
            {
                break;
            }
        }
        else
        {
            HI_ERR_CIPHER("Error! Invalid wait type!\r\n");
            return HI_ERR_FAILURE;
        }

        HI_TIME_CNT_GetUs(&ulLastTime);
        ulDuraTime = (ulLastTime - ulStartTime);
        if (ulDuraTime >= HASH_MAX_DURATION)
        { 
            HI_ERR_CIPHER("Error! Hash time out!\r\n");
            return HI_ERR_FAILURE;
        }
        else if ((ulDuraTime >= HASH_TACK_CUP_MAX_DURATION) && (ulDuraTime < HASH_MAX_DURATION))
        {
            HI_MDM_Sleep(1);
        }
		else
		{
			HI_HRTIMER_Udelay(1);
		}

    }

    return HI_ERR_SUCCESS;
}

/* check if hash module is idle or not */
static HI_U32 HAL_CIPHER_WaitHashIdle(HI_VOID)
{
    return HI_ERR_SUCCESS;
}

static HI_VOID HAL_CIPHER_MarkHashIdle(HI_VOID)
{

    return;
}

HI_U32 HAL_Cipher_SetInBufNum(HI_U32 chnId, HI_U32 num)
{
    return HAL_Cipher_SetReg(CIPHER_ID_REG_CHANn_IBUF_NUM, chnId, num);
}

HI_U32 HAL_Cipher_GetSrcLstRaddr(HI_U32 chnId, HI_U32 *addr)
{
    HI_U32 *regAddr  = 0;
    HI_U32 regValue = 0;

    if (CIPHER_PKGx1_CHAN == chnId || HI_NULL == addr)
    {
        return HI_ERR_INVALID_PARAMETER;
    }

    regAddr = (HI_U32 *)CIPHER_REG_CHANn_SRC_LST_RADDR(chnId);
    (HI_VOID)HAL_CIPHER_ReadReg(regAddr, &regValue); 
    *addr = regValue;
    HI_INFO_CIPHER("cnt=%d\r\n", regValue);    
    return HI_ERR_SUCCESS;
}

HI_U32 HAL_Cipher_GetDestLstRaddr(HI_U32 chnId, HI_U32 *addr)
{
    HI_U32 *regAddr  = 0;
    HI_U32 regValue = 0;

    if (CIPHER_PKGx1_CHAN == chnId || HI_NULL == addr)
    {
        return HI_ERR_INVALID_PARAMETER;
    }

    regAddr = (HI_U32 *)CIPHER_REG_CHANn_DEST_LST_RADDR(chnId);
    (HI_VOID)HAL_CIPHER_ReadReg(regAddr, &regValue);
    
    *addr = regValue;
    HI_INFO_CIPHER("cnt=%d\r\n", regValue);
    return HI_ERR_SUCCESS;
}

HI_U32 HAL_Cipher_GetInBufNum(HI_U32 chnId, HI_U32 *pNum)
{
    return HAL_Cipher_GetReg(CIPHER_ID_REG_CHANn_IBUF_NUM, chnId, pNum);
}


HI_U32 HAL_Cipher_SetInBufCnt(HI_U32 chnId, HI_U32 num)
{
    return HAL_Cipher_SetReg(CIPHER_ID_REG_CHANn_IBUF_CNT, chnId, num);

}

HI_U32 HAL_Cipher_GetInBufCnt(HI_U32 chnId, HI_U32 *pNum)
{
    return HAL_Cipher_GetReg(CIPHER_ID_REG_CHANn_IBUF_CNT, chnId, pNum);
}


HI_U32 HAL_Cipher_SetInBufEmpty(HI_U32 chnId, HI_U32 num)
{
    return HAL_Cipher_SetReg(CIPHER_ID_REG_CHANn_IEMPTY_CNT, chnId, num);
}

HI_U32 HAL_Cipher_GetInBufEmpty(HI_U32 chnId, HI_U32 *pNum)
{
    return HAL_Cipher_GetReg(CIPHER_ID_REG_CHANn_IEMPTY_CNT, chnId, pNum);
}

HI_U32 HAL_Cipher_SetOutBufNum(HI_U32 chnId, HI_U32 num)
{
    return HAL_Cipher_SetReg(CIPHER_ID_REG_CHANn_OBUF_NUM, chnId, num);
}

HI_U32 HAL_Cipher_GetOutBufNum(HI_U32 chnId, HI_U32 *pNum)
{
    return HAL_Cipher_GetReg(CIPHER_ID_REG_CHANn_OBUF_NUM, chnId, pNum);
}


HI_U32 HAL_Cipher_SetOutBufCnt(HI_U32 chnId, HI_U32 num)
{
    return HAL_Cipher_SetReg(CIPHER_ID_REG_CHANn_OBUF_CNT, chnId, num);
}

HI_U32 HAL_Cipher_GetOutBufCnt(HI_U32 chnId, HI_U32 *pNum)
{
    return HAL_Cipher_GetReg(CIPHER_ID_REG_CHANn_OBUF_CNT, chnId, pNum);
}


HI_U32 HAL_Cipher_SetOutBufFull(HI_U32 chnId, HI_U32 num)
{
    return HAL_Cipher_SetReg(CIPHER_ID_REG_CHANn_OFULL_CNT, chnId, num);
}

HI_U32 HAL_Cipher_GetOutBufFull(HI_U32 chnId, HI_U32 *pNum)
{
    return HAL_Cipher_GetReg(CIPHER_ID_REG_CHANn_OFULL_CNT, chnId, pNum);
}

HI_U32 HAL_Cipher_WaitIdle(HI_VOID)
{
    HI_S32 i = 0;
    HI_U32 *pu32RegAddr = 0;
    HI_U32 u32RegValue = 0;

    /* channel 0 configuration register [31-2]:reserved, [1]:ch0_busy, [0]:ch0_start 
         * [1]:channel 0 status signal, [0]:channel 0 encrypt/decrypt start signal
         */

    pu32RegAddr = (HI_U32 *)CIPHER_REG_CHAN0_CFG;
    for (i = 0; i < CIPHER_WAIT_IDEL_TIMES; i++)
    {
        (HI_VOID)HAL_CIPHER_ReadReg(pu32RegAddr, &u32RegValue);
        if (0x0 == ((u32RegValue >> 1) & 0x01))
        {
            return HI_ERR_SUCCESS;
        }
    }

    return HI_ERR_FAILURE;
}
/*
just only check for channel 0
 */
HI_BOOL HAL_Cipher_IsIdle(HI_U32 chn)
{
    HI_U32 u32RegValue = 0;

    HI_ASSERT(CIPHER_PKGx1_CHAN == chn);

    (HI_VOID)HAL_CIPHER_ReadReg((HI_U32 *)CIPHER_REG_CHAN0_CFG, &u32RegValue);
    if (0x0 == ((u32RegValue >> 1) & 0x01))
    {
        return HI_TRUE;
    }

    return HI_FALSE;
}

HI_U32 HAL_Cipher_SetDataSinglePkg(HI_DRV_CIPHER_DATA_INFO_S * info)
{
    HI_U32 *regAddr = 0;
    HI_U32 i = 0;

    regAddr = (HI_U32 *)CIPHER_REG_CHAN0_CIPHER_DIN(0);
    
    for (i = 0; i < 4; i++)
    {
        (HI_VOID)HAL_CIPHER_WriteReg(regAddr + i, info->u32DataPkg[i]);
    }
    
    return HI_ERR_SUCCESS;
}

HI_U32 HAL_Cipher_ReadDataSinglePkg(HI_U32 pData[4])
{
    HI_U32 *regAddr = 0;
    HI_U32 i = 0;

    regAddr = (HI_U32 *)CIPHER_REG_CHAN0_CIPHER_DOUT(0);

    for (i = 0; i < 4; i++)
    {
        (HI_VOID)HAL_CIPHER_ReadReg(regAddr + i, pData+i);
    }
    
    return HI_ERR_SUCCESS;
}

HI_U32 HAL_Cipher_StartSinglePkg(HI_U32 chnId)
{
    HI_U32 *pu32RegAddr = 0;
    HI_U32 u32RegValue = 0;

    HI_ASSERT(CIPHER_PKGx1_CHAN == chnId);

    pu32RegAddr = (HI_U32 *)CIPHER_REG_CHAN0_CFG;
    (HI_VOID)HAL_CIPHER_ReadReg(pu32RegAddr, &u32RegValue);
    
    u32RegValue |= 0x1;
    (HI_VOID)HAL_CIPHER_WriteReg(pu32RegAddr, u32RegValue); /* start work */
    
    return HI_ERR_SUCCESS;
}

HI_U32 HAL_Cipher_SetBufAddr(HI_U32 chnId, CIPHER_BUF_TYPE_E bufType, HI_U32 addr)
{
    HI_U32 *regAddr = 0;

    if (CIPHER_PKGx1_CHAN == chnId)
    {
        return HI_ERR_INVALID_PARAMETER;
    }

    if (CIPHER_BUF_TYPE_IN == bufType)
    {
        regAddr = (HI_U32 *)CIPHER_REG_CHANn_SRC_LST_SADDR(chnId);
    }
    else if (CIPHER_BUF_TYPE_OUT == bufType)
    {
        regAddr = (HI_U32 *)CIPHER_REG_CHANn_DEST_LST_SADDR(chnId);
    }
    else
    {
        HI_ERR_CIPHER("SetBufAddr type err:%x.\r\n", bufType);
        return HI_ERR_INVALID_PARAMETER;
    }


    HI_INFO_CIPHER("Set chn%d '%s' BufAddr to:%x.\r\n", chnId, (CIPHER_BUF_TYPE_IN == bufType)?"In":"Out",  addr);
    (HI_VOID)HAL_CIPHER_WriteReg(regAddr, addr);
    return HI_ERR_SUCCESS;
}

HI_VOID HAL_Cipher_Reset(HI_VOID)
{
    //操作系统控制寄存器
    return;
}

HI_U32 HAL_Cipher_GetOutIV(HI_U32 chnId, HI_CIPHER_CTRL_S* pCtrl)
{
    HI_U32 i = 0;
    HI_U32 *regAddr = 0;


    regAddr = (HI_U32 *)CIPHER_REG_CHAN_CIPHER_IVOUT(chnId);

    for (i = 0; i < 4; i++)
    {
        (HI_VOID)HAL_CIPHER_ReadReg(regAddr + i, &pCtrl->u32IV[i]);
    }

    return HI_ERR_SUCCESS;
}

HI_U32 HAL_Cipher_SetInIV(HI_U32 chnId, HI_CIPHER_CTRL_S* pCtrl)
{
    HI_U32 i = 0;
    HI_U32 *regAddr = 0;

    if (CIPHER_PKGx1_CHAN != chnId)
    {
        return HI_ERR_INVALID_PARAMETER;
    }

    regAddr = (HI_U32 *)CIPHER_REG_CHAN0_CIPHER_IVIN(0);

    for (i = 0; i < 4; i++)
    {
        (HI_VOID)HAL_CIPHER_WriteReg(regAddr + i, pCtrl->u32IV[i]);
    }

    return HI_ERR_SUCCESS;
}

HI_U32 HAL_Cipher_SetSoftwareKey(HI_U32 chnId, HI_CIPHER_CTRL_S* pCtrl)
{
    HI_U32 i = 0;
    HI_U32 *regAddr = 0;

    regAddr = (HI_U32 *)CIPHER_REG_CIPHER_KEY(chnId);

    if (HI_NULL == pCtrl)
    {
        HI_ERR_CIPHER("Error, HI_NULL pointer!\r\n");
        return HI_ERR_INVALID_PARAMETER;
    }

    if (HI_FALSE == pCtrl->bKeyByCA)
    {
        for (i = 0; i < 8; i++)
        {
            (HI_VOID)HAL_CIPHER_WriteReg(regAddr + i, pCtrl->u32Key[i]);
        }
    }

    HI_INFO_CIPHER("SetKey: chn%d,Key:%x, %x, %x, %x.\r\n", chnId, pCtrl->u32Key[0], pCtrl->u32Key[1], pCtrl->u32Key[2], pCtrl->u32Key[3]); 
    return HI_ERR_SUCCESS;
}

/*
=========channel n control register==========
[31:22]             weight [in 64bytes, just only for multi-packet channel encrypt or decrypt, otherwise reserved.]
[21:17]             reserved
[16:14]     RW    key_adder [current key sequence number]
[13]          RW    key_sel [key select control, 0-CPU keys, 1-keys from key Ladder]
[12:11]             reserved
[10:9]      RW      key_length[key length control
                                            (1).AES, 00-128 bits key, 01-192bits 10-256bits, 11-128bits
                                            (2).DES, 00-3 keys, 01-3keys, 10-3keys, 11-2keys]
[8]                     reserved
[7:6]       RW      width[bits width control
                                 (1).for DES/3DES, 00-64bits, 01-8bits, 10-1bit, 11-64bits
                                 (2).for AES, 00-128bits, 01-8bits, 10-1bit, 11-128bits]
[5:4]       RW      alg_sel[algorithm type, 00-DES, 01-3DES, 10-AES, 11-DES]
[3:1]       RW      mode[mode control, 
                                  (1).for AES, 000-ECB, 001-CBC, 010-CFB, 011-OFB, 100-CTR, others-ECB
                                  (2).for DES, 000-ECB, 001-CBC, 010-CFB, 011-OFB, others-ECB]
[0]         RW      decrypt[encrypt or decrypt control, 0 stands for encrypt, 1 stands for decrypt]
*/
HI_U32 HAL_Cipher_Config(HI_U32 chnId, HI_BOOL bDecrypt, HI_BOOL bIVChange, HI_CIPHER_CTRL_S* pCtrl)
{
    HI_U32 keyId = 0;
    HI_U32 *regAddr = 0;
    HI_U32 regValue = 0;

    HI_BOOL bKeyByCA = pCtrl->bKeyByCA;

    if (CIPHER_PKGx1_CHAN == chnId)
    {
        /* channel 0, single packet encrypt or decrypt channel */
        regAddr = (HI_U32 *)CIPHER_REG_CHAN0_CIPHER_CTRL;
    }
    else
    {
        regAddr = (HI_U32 *)CIPHER_REG_CHANn_CIPHER_CTRL(chnId);
    }

    (HI_VOID)HAL_CIPHER_ReadReg(regAddr, &regValue);

    if (HI_FALSE == bDecrypt)/* encrypt */
    {
        regValue &= ~(1 << 0);
    }
    else /* decrypt */
    {
        regValue |= 1;
    }

    /* set mode */
    regValue &= ~(0x07 << 1);/* clear bit1~bit3 */
    regValue |= (((unsigned int)pCtrl->stModeCtrl.enWorkMode & 0x7) << 1);

    /* set algorithm bits */
    regValue &= ~(0x03 << 4); /* clear algorithm bits*/
    regValue |= (((unsigned int)pCtrl->stModeCtrl.enAlg & 0x3) << 4);

    /* set bits width */
    regValue &= ~(0x03 << 6);
    regValue |= (((unsigned int)pCtrl->stModeCtrl.enBitWidth & 0x3) << 6);

    regValue &= ~(0x01 << 8);
    regValue |= ((bIVChange & 0x1) << 8);
    if (bIVChange)
    {
        HAL_Cipher_SetInIV(chnId, pCtrl);
    }

    regValue &= ~(0x03 << 9);
    regValue |= (((unsigned int)pCtrl->stModeCtrl.enKeyLen & 0x3) << 9);

    regValue &= ~(0x01 << 13);
    regValue |= ((bKeyByCA & 0x1) << 13);


    keyId = chnId;

    regValue &= ~(0x07 << 14);
    regValue |= ((keyId & 0x7) << 14);


    (HI_VOID)HAL_CIPHER_WriteReg(regAddr, regValue);
    
    return HI_ERR_SUCCESS;
}

HI_U32 HAL_Cipher_SetAGEThreshold(HI_U32 chnId, CIPHER_INT_TYPE_E intType, HI_U16 value)
{
    HI_U32 *regAddr = 0;

    if (CIPHER_PKGx1_CHAN == chnId)
    {
        return HI_ERR_INVALID_PARAMETER;
    }

    if (CIPHER_INT_TYPE_IN == intType)
    {
        regAddr = (HI_U32 *)CIPHER_REG_CHANn_IAGE_CNT(chnId);
    }
    else if (CIPHER_INT_TYPE_OUT == intType)
    {
        regAddr = (HI_U32 *)CIPHER_REG_CHANn_OAGE_CNT(chnId);
    }
    else
    {
        HI_ERR_CIPHER("SetAGEThreshold type err:%x.\r\n", intType);
        return HI_ERR_INVALID_PARAMETER;
    }
    (HI_VOID)HAL_CIPHER_WriteReg(regAddr, value);

    return HI_ERR_SUCCESS;
}

HI_U32 HAL_Cipher_SetIntThreshold(HI_U32 chnId, CIPHER_INT_TYPE_E intType, HI_U32 value)// 中断水线
{
    HI_U32 *regAddr = 0;

    if (CIPHER_PKGx1_CHAN == chnId)
    {
        return HI_ERR_INVALID_PARAMETER;
    }

    if (CIPHER_INT_TYPE_IN == intType)
    {
        regAddr = (HI_U32 *)CIPHER_REG_CHANn_INT_ICNTCFG(chnId);
    }
    else if (CIPHER_INT_TYPE_OUT == intType)
    {
        regAddr = (HI_U32 *)CIPHER_REG_CHANn_INT_OCNTCFG(chnId);
    }
    else
    {
        HI_ERR_CIPHER("SetIntThreshold type err:%x.\r\n", intType);
        return HI_ERR_INVALID_PARAMETER;
    }

    if (value > 0xffff)
    {
        HI_ERR_CIPHER("SetIntThreshold value err:%x, set to 0xffff\r\n", value);
        value = 0xffff;
    }

    (HI_VOID)HAL_CIPHER_WriteReg(regAddr, value);

    return HI_ERR_SUCCESS;
}

/*
interrupt enable
[31]-----cipher module unitary interrupt enable
[30:16]--reserved
[15] channel 7 output queue data interrupt enable
[14] channel 6 output queue data interrupt enable
[... ] channel ... output queue data interrupt enable
[9]   channel 1 output queue data interrupt enable
[8]   channel 0 data dispose finished interrupt enble
[7] channel 7 input queue data interrupt enable
[6] channel 6 input queue data interrupt enable
...
[1] channel 1 input queue data interrupt enable
[0] reserved
*/

HI_VOID HAL_Cipher_EnableInt(HI_U32 chnId, int intType)
{
    HI_U32 *regAddr = 0;
    HI_U32 regValue = 0;

    regAddr = (HI_U32 *)CIPHER_REG_INT_EN;
    (HI_VOID)HAL_CIPHER_ReadReg(regAddr, &regValue);

    regValue |= (HI_U32)((HI_U32)1 << 31); /* sum switch int_en */
    regValue |= (HI_U32)(1 << 30); /* sec_int_en */ //test zn 3911是否配置，手册中为预留

    if (CIPHER_PKGx1_CHAN == chnId)
    {
        //regValue |= (1 << 8);// ZN 通道0数据处理完成中断使能。 目前是查询方式
    }else
    {
        if (CIPHER_INT_TYPE_OUT == (CIPHER_INT_TYPE_OUT & (HI_U32)intType))
        {
            regValue |= (1 << (8 + chnId));
        }

        /* NOT else if */
        if (CIPHER_INT_TYPE_IN == (CIPHER_INT_TYPE_IN & (HI_U32)intType))
        {
/*
                            【屏蔽人】z00193780
                            【审核人】b00208046
                            【代码分析】二次开发接口，客户有可能使用该分支，需保留。
                            【功能影响】不影响正常业务，屏蔽告警
*/
            regValue |= (1 << (0 + chnId));
        }
    }

    (HI_VOID)HAL_CIPHER_WriteReg(regAddr, regValue);
    HI_INFO_CIPHER("HAL_Cipher_EnableInt: Set INT_EN:%d\r\n", regValue);
}

HI_U32 HAL_Cipher_DisableInt(HI_U32 chnId, int intType)
{
    HI_U32 *regAddr = 0;
    HI_U32 regValue = 0;

    regAddr = (HI_U32 *)CIPHER_REG_INT_EN;
    (HI_VOID)HAL_CIPHER_ReadReg(regAddr, &regValue);

    if (CIPHER_PKGx1_CHAN == chnId)
    {
        regValue &= ~(1 << 8);
    }
    else
    {
        if (CIPHER_INT_TYPE_OUT == (CIPHER_INT_TYPE_OUT & (HI_U32)intType))
        {
            regValue &= ~(HI_U32)(1 << (8 + chnId));
        }

        /* NOT else if */
        if (CIPHER_INT_TYPE_IN == (CIPHER_INT_TYPE_IN & (HI_U32)intType))
        {
            regValue &= ~(HI_U32)(1 << (0 + chnId));
        }
    }

    if (0 == (regValue & 0x7fffffff))
    {
        regValue &= ~((HI_U32)((HI_U32)1 << 31)); /* regValue = 0; sum switch int_en */
    }

    (HI_VOID)HAL_CIPHER_WriteReg(regAddr, regValue);
    HI_INFO_CIPHER("HAL_Cipher_DisableInt: Set INT_EN:%x\n", regValue);
    return HI_ERR_SUCCESS;
}

HI_VOID HAL_Cipher_DisableAllInt(HI_VOID)
{
    HI_U32 *regAddr = HI_NULL;
    HI_U32 regValue = 0;
    HI_U32 i;

    regAddr = (HI_U32 *)CIPHER_REG_INT_EN;
    (HI_VOID)HAL_CIPHER_ReadReg(regAddr, &regValue);
    for(i = CIPHER_PKGxN_CHAN_MIN; i <= CIPHER_PKGxN_CHAN_MAX; i++)
	{
	    regValue &= ~(HI_U32)(0x01 << i);
    }
    regValue &= 0x3FFFFFFF;
    (HI_VOID)HAL_CIPHER_WriteReg(regAddr, regValue);
}

/*
interrupt status register
[31:16]--reserved
[15] channel 7 output queue data interrupt enable
[14] channel 6 output queue data interrupt enable
[... ] channel ... output queue data interrupt enable
[9]   channel 1 output queue data interrupt enable
[8]   channel 0 data dispose finished interrupt enble
[7] channel 7 input queue data interrupt enable
[6] channel 6 input queue data interrupt enable
...
[1] channel 1 input queue data interrupt enable
[0] reserved
*/

HI_VOID HAL_Cipher_GetIntState(HI_U32 *pState)
{
    HI_U32 *regAddr = 0;
    HI_U32 regValue = 0;

    regAddr = (HI_U32 *)CIPHER_REG_INT_STATUS;
    (HI_VOID)HAL_CIPHER_ReadReg(regAddr, &regValue);

    if (pState)
    {
        *pState = regValue;
    }

   HI_INFO_CIPHER("HAL_Cipher_GetIntState=%x\n", regValue);
}

HI_VOID HAL_Cipher_GetIntEnState(HI_U32 *pState)
{
    HI_U32 *regAddr = 0;
    HI_U32 regValue = 0;

    regAddr = (HI_U32 *)CIPHER_REG_INT_EN;
    
    (HI_VOID)HAL_CIPHER_ReadReg(regAddr, &regValue);

    if (pState)
    {
        *pState = regValue;
    }

   HI_INFO_CIPHER("HAL_Cipher_GetIntEnState=%x\r\n", regValue);
}

HI_VOID HAL_Cipher_GetRawIntState(HI_U32 *pState)
{
    HI_U32 *regAddr = 0;
    HI_U32 regValue = 0;

    regAddr = (HI_U32 *)CIPHER_REG_INT_RAW;
    
    (HI_VOID)HAL_CIPHER_ReadReg(regAddr, &regValue);

    if (pState)
    {
        *pState = regValue;
    }

    HI_INFO_CIPHER("HAL_Cipher_GetRawIntState=%x\r\n", regValue);
}

HI_VOID HAL_Cipher_ClrIntState(HI_U32 intStatus)
{
    HI_U32 *regAddr;
    HI_U32 regValue;

    regAddr = (HI_U32 *)CIPHER_REG_INT_RAW;
    regValue = intStatus;
    (HI_VOID)HAL_CIPHER_WriteReg(regAddr, regValue);
}

HI_VOID HAL_Cipher_EnableAllSecChn(HI_VOID)
{
	HI_U32 *regAddr = (HI_U32 *)CIPHER_REG_SEC_CHN_CFG;
	HI_U32 regValue = 0;
    HI_U32 i;

    for(i=CIPHER_PKGxN_CHAN_MIN; i<=CIPHER_PKGxN_CHAN_MAX; i++)
	{
	    regValue |= 0x01 << i;
    }
    regValue |= 0x01;
    (HI_VOID)HAL_CIPHER_WriteReg(regAddr, regValue);
	return;
}

HI_VOID HAL_Cipher_SetHDMITxRxOtpSelReg(HI_VOID)
{
    HI_U32 *pu32Addr = 0;
    HI_U32 u32Value = 0;

    pu32Addr = (HI_U32 *)PERI_HDMI_RX_TX_OTP_SEL_REG_ADDR;
    (HI_VOID)HAL_CIPHER_ReadReg(pu32Addr,&u32Value);

    u32Value &= 0xFFFFFFFE;
    (HI_VOID)HAL_CIPHER_WriteReg(pu32Addr, u32Value);

    return;
}

//#ifdef CIPHER_MHASH_SUPPORT
static HI_U32 HAL_Hash_Small2Large(HI_U32 u32SamllVal)
{
    HI_U32 u32LargeVal = 0;

    u32LargeVal  = (u32SamllVal >> 24) & 0xFF;
    u32LargeVal |= ((u32SamllVal >> 16) & 0xFF) << 8;
    u32LargeVal |= ((u32SamllVal >> 8) & 0xFF) << 16;
    u32LargeVal |= ((u32SamllVal) & 0xFF) << 24;

    return u32LargeVal;
}
//#endif

HI_VOID HAL_Cipher_FreeHash(HI_VOID)
{
    HAL_CIPHER_MarkHashIdle();

    if (s_bIsHashUser > 0)
    {
        s_bIsHashUser--;
    }
    else
    {
        s_bIsHashUser = 0;
    }
}

HI_U32 HAL_Cipher_AllocHash(HI_VOID)
{
    HI_U32 ret = HI_ERR_SUCCESS;
    if (s_bIsHashUser >= MAX_HASH_USER)
    {
        HI_ERR_CIPHER("Error! Hash is Busy!\r\n");
        return HI_ERR_FAILURE;
    }


    ret = HAL_CIPHER_WaitHashIdle();
    if(HI_ERR_SUCCESS != ret)
    {
        HI_ERR_CIPHER("Time out!Hash is busy now!\r\n");
        return HI_ERR_FAILURE;
    }
    s_bIsHashUser++;
    return HI_ERR_SUCCESS;
}

HI_U32 HAL_Hash_CalcInit(HASH_DATA_S *pHashData)
{
    HI_U32 ret = HI_ERR_SUCCESS;
    CIPHER_SHA_CTRL_U unCipherSHACtrl;
    CIPHER_SHA_START_U unCipherSHAStart;
    HI_U32 i;
#ifdef CIPHER_MHASH_SUPPORT
    HI_U32 *pu32RegAddr =HI_NULL;
    HI_U32 u32ShaVal =HI_ERR_SUCCESS;
#endif

    if(HI_NULL == pHashData)
    {
        HI_ERR_CIPHER("Error! HI_NULL pointer input!\r\n");
        return HI_ERR_FAILURE;
    }
    
    ret = HAL_Cipher_AllocHash();
    if(HI_ERR_SUCCESS != ret)
    {
        return HI_ERR_FAILURE;
    }

    /* wait for hash_rdy */
    ret = HASH_WaitReady(HASH_READY);
    if(HI_ERR_SUCCESS != ret)
    {
        (HI_VOID)HAL_Cipher_FreeHash();
        return HI_ERR_FAILURE;
    }

    /* set hmac-sha key */
    if(((HI_MDM_HASH_TYPE_HMAC_SHA1 == pHashData->enShaType) || (HI_MDM_HASH_TYPE_HMAC_SHA256 == pHashData->enShaType)))
    {
		// 驱动接口需要配置寄存器
        if (HI_CIPHER_HMAC_KEY_FROM_CPU == pHashData->enHMACKeyFrom)
        {
        }
        else if (HI_CIPHER_HMAC_KEY_FROM_CA == pHashData->enHMACKeyFrom)
        {
#ifdef TEST_EFUSE_4_KEY
			ret = HAL_EFUSE_LoadHash((HI_EFUSE_KEY_IDX_E)g_hash_hmac_hw_key_times);
#else
            ret = HAL_EFUSE_LoadHash(DEFAULT_HASH_KEY_AERA);
#endif            
            if(HI_ERR_SUCCESS != ret)
            {
                HI_ERR_CIPHER("HAL_EFUSE_LoadHash ret = %d\r\n",ret);
                (HI_VOID)HAL_Cipher_FreeHash();
                return HI_ERR_FAILURE;
            }

        }
        else
        {
            HAL_Cipher_FreeHash();
            return HI_ERR_INVALID_PARAMETER;            
        }
    }

    /* write total len low and high */
#ifdef CIPHER_MHASH_SUPPORT
// for test zn
    //HI_U32 temp = (64 + 64 + 24);
    (HI_VOID)HAL_CIPHER_WriteReg((HI_U32 *)CIPHER_HASH_REG_TOTALLEN_LOW_ADDR, pHashData->u32DataLen);
#else
    (HI_VOID)HAL_CIPHER_WriteReg((HI_U32 *)CIPHER_HASH_REG_TOTALLEN_LOW_ADDR, 0xFFFFFFFF);
#endif
    (HI_VOID)HAL_CIPHER_WriteReg((HI_U32 *)CIPHER_HASH_REG_TOTALLEN_HIGH_ADDR, 0);

    /* config sha_ctrl : read by dma first, and by cpu in the hash final function */
    unCipherSHACtrl.u32 = 0;
    (HI_VOID)HAL_CIPHER_ReadReg((HI_U32 *)CIPHER_HASH_REG_CTRL_ADDR, &unCipherSHACtrl.u32);
    unCipherSHACtrl.bits.read_ctrl = 0;


    if(HI_MDM_HASH_TYPE_SHA1 == pHashData->enShaType)
    {
        unCipherSHACtrl.bits.hardkey_hmac_flag = 0;
        unCipherSHACtrl.bits.sha_sel= 0x0;
    }
    else if(HI_MDM_HASH_TYPE_SHA256 == pHashData->enShaType)
    {
        unCipherSHACtrl.bits.hardkey_hmac_flag = 0;
        unCipherSHACtrl.bits.sha_sel= 0x1;
    }
    else if(HI_MDM_HASH_TYPE_HMAC_SHA1 == pHashData->enShaType)
    {
        if (HI_CIPHER_HMAC_KEY_FROM_CA == pHashData->enHMACKeyFrom)
        {
            unCipherSHACtrl.bits.hardkey_hmac_flag = 1;
            unCipherSHACtrl.bits.hardkey_sel = pHashData->enHMACKeyFrom;
        }
        else
        {
            unCipherSHACtrl.bits.hardkey_hmac_flag = 0;
        }
		
        unCipherSHACtrl.bits.sha_sel= 0x0;
		
    }
    else if(HI_MDM_HASH_TYPE_HMAC_SHA256 == pHashData->enShaType)
    {
        if (HI_CIPHER_HMAC_KEY_FROM_CA == pHashData->enHMACKeyFrom)
        {
            unCipherSHACtrl.bits.hardkey_hmac_flag = 1;
            unCipherSHACtrl.bits.hardkey_sel = pHashData->enHMACKeyFrom;// hy api
        }
        else
        {
            unCipherSHACtrl.bits.hardkey_hmac_flag = 0;// for api
        }
        
        unCipherSHACtrl.bits.sha_sel= 0x1;
    }
    else
    {
        HI_ERR_CIPHER("Invalid hash type input!\r\n");
        (HI_VOID)HAL_Cipher_FreeHash();
        return HI_ERR_FAILURE;
    }
    unCipherSHACtrl.bits.small_end_en = 1;
#ifdef CIPHER_MHASH_SUPPORT
    //for test zn
    if (HI_CIPHER_HMAC_KEY_FROM_CA == pHashData->enHMACKeyFrom)
    {
        unCipherSHACtrl.bits.sha_init_update_en = 0;
    }
    else
    {
        unCipherSHACtrl.bits.sha_init_update_en = 1;
    
        pu32RegAddr = (HI_U32 *)CIPHER_HASH_REG_INIT1_UPDATE;
        for(i=0; i<8; i++)
        {
            u32ShaVal = HAL_Hash_Small2Large(pHashData->u32ShaVal[i]);
            (HI_VOID)HAL_CIPHER_WriteReg(pu32RegAddr+i, u32ShaVal);
            //HI_PRINT ("IV[%d] 0x%x\n", i, u32ShaVal);
        }
    }
#endif
    (HI_VOID)HAL_CIPHER_WriteReg((HI_U32 *)CIPHER_HASH_REG_CTRL_ADDR, unCipherSHACtrl.u32);    
    
    /* config sha_start */
    unCipherSHAStart.u32 = 0;
    unCipherSHAStart.bits.sha_start = 1;
    (HI_VOID)HAL_CIPHER_WriteReg((HI_U32 *)CIPHER_HASH_REG_START_ADDR, unCipherSHAStart.u32);
    
    return HI_ERR_SUCCESS;
}

extern HI_MMZ_BUF_S s_stMMZBuffer;
HI_U32 HAL_Hash_CalcUpdate(HASH_DATA_S *pHashData)
{
    HI_U32 ret = HI_ERR_SUCCESS;

    if( HI_NULL == pHashData )
    {
        HI_ERR_CIPHER("Error, HI_NULL pointer input!\r\n");
        (HI_VOID)HAL_Cipher_FreeHash();
        return HI_ERR_FAILURE;
    }

    ret= HASH_WaitReady(REC_READY);
    if(HI_ERR_SUCCESS != ret)
    {
        (HI_VOID)HAL_Cipher_FreeHash();
        return HI_ERR_FAILURE;
    }
    HI_MDM_DataCacheFlush((HI_PVOID)s_stMMZBuffer.user_viraddr, (HI_S32)s_stMMZBuffer.bufsize);
    (HI_VOID)HAL_CIPHER_WriteReg((HI_U32 *)CIPHER_HASH_REG_DMA_START_ADDR, pHashData->u32DataPhy);
    (HI_VOID)HAL_CIPHER_WriteReg((HI_U32 *)CIPHER_HASH_REG_DMA_LEN, pHashData->u32DataLen);
    ret  = HASH_WaitReady(REC_READY);
    if(HI_ERR_SUCCESS != ret)
    {
        (HI_VOID)HAL_Cipher_FreeHash();
        return HI_ERR_FAILURE;
    }

    return HI_ERR_SUCCESS;
}

HI_U32 HAL_Hash_CalcFinal(HASH_DATA_S *pHashData)
{
    HI_U32 ret = HI_ERR_SUCCESS;
    CIPHER_SHA_STATUS_U unCipherSHAStatus;
    HI_U32 *pu32RegAddr = HI_NULL;
    HI_U32 i = 0;

    if( HI_NULL == pHashData )
    {
        HI_ERR_CIPHER("Error, HI_NULL pointer input!\r\n");
        (HI_VOID)HAL_Cipher_FreeHash();
        return HI_ERR_FAILURE;
    }

    /* wait for rec_ready instead of hash_ready */
    ret= HASH_WaitReady(REC_READY);
    if(HI_ERR_SUCCESS != ret)
    {
        (HI_VOID)HAL_Cipher_FreeHash();
        return HI_ERR_FAILURE;
    }

    /* read digest */
    unCipherSHAStatus.u32 = 0;
    (HI_VOID)HAL_CIPHER_ReadReg((HI_U32 *)CIPHER_HASH_REG_STATUS_ADDR, &unCipherSHAStatus.u32);

    pu32RegAddr = (HI_U32 *)CIPHER_HASH_REG_SHA_OUT1;
    if( (0x00 == unCipherSHAStatus.bits.error_state) && (0x00 == unCipherSHAStatus.bits.len_err))
    {
        if( (HI_MDM_HASH_TYPE_SHA1 == pHashData->enShaType)
         || (HI_MDM_HASH_TYPE_HMAC_SHA1 == pHashData->enShaType))
        {
            
            for(i = 0; i < 5; i++)
            {
                (HI_VOID)HAL_CIPHER_ReadReg(pu32RegAddr+i, &pHashData->u32ShaVal[i]);
            }
        }
        else if((HI_MDM_HASH_TYPE_SHA256 == pHashData->enShaType )
              || (HI_MDM_HASH_TYPE_HMAC_SHA256 == pHashData->enShaType))
        {
            for(i = 0; i < 8; i++)
            {
				(HI_VOID)HAL_CIPHER_ReadReg(pu32RegAddr+i, &pHashData->u32ShaVal[i]);
    		}
        }
        else
        {
            HI_ERR_CIPHER("Invalid hash type : %d!\r\n", pHashData->enShaType);
            (HI_VOID)HAL_Cipher_FreeHash();
            return HI_ERR_FAILURE;
        }
    }
    else
    {
        HI_ERR_CIPHER("Error! SHA Status Reg: error_state = %d!\r\n", unCipherSHAStatus.bits.error_state);
        (HI_VOID)HAL_Cipher_FreeHash();
        return HI_ERR_FAILURE;
    }

    HAL_Cipher_FreeHash();
    
    return HI_ERR_SUCCESS;
}

HI_U32 HAL_Random_GetNumber(HI_IN HI_U32 u32TimeOutUs, HI_OUT HI_U32 *pu32Rng)
{
    HI_U32 u32RngStat = 0;
    HI_U32 u32TimeOut = 0;

	if(HI_NULL == pu32Rng)
	{
        HI_ERR_CIPHER("Invalid params!\r\n");
        return HI_ERR_INVALID_PARAMETER;
	}
    
    (HI_VOID)HAL_CIPHER_WriteReg((HI_U32 *)HISEC_COM_TRNG_CTRL, 0x80a6);//zn 3911配置什么模式

    if(0 == u32TimeOutUs)
	{
	    (HI_VOID)HAL_CIPHER_ReadReg((HI_U32 *)HISEC_COM_TRNG_DATA_ST, &u32RngStat);//test zn 代码中7比 文档和寄存器手册中是8比特
	    if((u32RngStat & 0xFF) == 0)
	    {
            HI_ERR_CIPHER("HI_ERR_RNG_NO_AVAILABLE_RNG\r\n");
	        return HI_ERR_RNG_NO_AVAILABLE_RNG;
	    }
	}
	else
	{
	    while((u32TimeOut++) < u32TimeOutUs)
	    {
	    	/* low 3bit(RNG_data_count[2:0]), indicate how many RNGs in the fifo is available now */
	        (HI_VOID)HAL_CIPHER_ReadReg((HI_U32 *)HISEC_COM_TRNG_DATA_ST, &u32RngStat);
	        if((u32RngStat & 0xFF) > 0)//随机数fifo中随机数的个数
	        {
	            break;
	        }
            HI_MDM_Udelay(1);
	    }
        if (u32TimeOut > u32TimeOutUs)
        {
            (HI_VOID)HAL_CIPHER_ReadReg((HI_U32 *)HISEC_COM_TRNG_CTRL, &u32RngStat);
            if (u32RngStat & HISEC_COM_TRNG_OSA_ERR)
            {
                return HI_ERR_RNG_LOW_OSA;
            }
            else
            {
                return HI_ERR_TIMEOUT;
            }
        }
	}

    (HI_VOID)HAL_CIPHER_ReadReg((HI_U32 *)HISEC_COM_TRNG_FIFO_DATA, pu32Rng);   
    //(HI_VOID)HAL_CIPHER_WriteReg(HISEC_COM_TRNG_DATA_ST, 0x00);//test zn是否需要清零

    return HI_ERR_SUCCESS;
}

HI_U32 HAL_Cipher_ClearCbcVerifyFlag()
{
    (HI_VOID)HAL_CIPHER_WriteReg((HI_U32 *)CIPHER_REG_HL_APP_CBC_CTRL, 0x0);

    return HI_ERR_SUCCESS;
}

HI_VOID HAL_RSA_Start(HI_VOID)
{
    HAL_CIPHER_WriteReg((HI_U32 *)SEC_RSA_START_REG, 0x01);
}

HI_U32 HAL_RSA_WaitFree(HI_VOID)
{
	HI_U32 u32Value;
    HI_U32 u32TryCount = 0;

	do {

        HAL_CIPHER_ReadReg((HI_U32 *)SEC_RSA_BUSY_REG, &u32Value);
        if ((u32Value & RSA_BUSY) == 0)
        {
            return HI_ERR_SUCCESS;
        }
        u32TryCount++;
        HI_MDM_Sleep(10);
	} while (u32TryCount < RSA_RTY_CNT);

	return HI_ERR_FAILURE;
}

HI_VOID HAL_RSA_ClearRam(HI_VOID)
{
	HI_U32 u32Value;

    HAL_CIPHER_ReadReg((HI_U32 *)SEC_RSA_MOD_REG, &u32Value);
    u32Value &= 0x0c;
    u32Value |= RSA_DATA_CLR_INPUT | RSA_DATA_CLR_OUTPUT | RSA_DATA_CLR_KEY | RSA_MOD_SEL_RAM_CLAER;
    HAL_CIPHER_WriteReg((HI_U32 *)SEC_RSA_MOD_REG, u32Value);
}

HI_VOID HAL_RSA_Crc(HI_VOID)
{
	HI_U32 u32Value;

    HAL_CIPHER_ReadReg((HI_U32 *)SEC_RSA_MOD_REG, &u32Value);
    u32Value &= 0x0C; //don't chang sec_rsa_key_width
    u32Value |= 0x03;
    HAL_CIPHER_WriteReg((HI_U32 *)SEC_RSA_MOD_REG, u32Value);
}

HI_VOID HAL_RSA_GetCrc(HI_U32 *pu32Crc)
{
    HAL_CIPHER_ReadReg((HI_U32 *)SEC_RSA_CRC16_REG, pu32Crc);
}

HI_VOID HAL_RSA_ConfigMode(CIPHER_RSA_KEY_WIDTH_E enKenWidth)
{
	HI_U32 u32Value;

    u32Value = ((HI_U32)enKenWidth << 2) | RSA_MOD_SEL_OPT;
    HAL_CIPHER_WriteReg((HI_U32 *)SEC_RSA_MOD_REG, u32Value);
}

HI_VOID HAL_RSA_WriteData(CIPHER_RSA_DATA_TYPE_E enDataType, HI_U8 *pu8Data, HI_U32 u32DataLen, HI_U32 u32Len)
{
	HI_U32 u32Value = 0;
    HI_U32 *u32Reg = HI_NULL;
    HI_U8 *pPos = HI_NULL;
    HI_U32 i= 0;

    if (enDataType == CIPHER_RSA_DATA_TYPE_CONTEXT)
    {
        u32Reg = (HI_U32 *)SEC_RSA_WDAT_REG;
    }
    else
    {
        u32Reg = (HI_U32 *)SEC_RSA_WSEC_REG;
    }

    pPos = pu8Data;
    for(i=0; i<u32Len; i+=4)
    {
        u32Value  = (HI_U32)pPos[0];
        u32Value |= ((HI_U32)pPos[1]) << 8;
        u32Value |= ((HI_U32)pPos[2]) << 16;
        u32Value |= ((HI_U32)pPos[3]) << 24;
        HAL_CIPHER_WriteReg(u32Reg, u32Value); 
        pPos+=4; 
    }
}

HI_VOID HAL_RSA_ReadData(HI_U8 *pu8Data, HI_U32 u32DataLen, HI_U32 u32Klen)
{
	HI_U32 u32Value;
    HI_U8 *pPos;
    HI_U32 i = 0;

    pPos = pu8Data;
    for(i=0; i<u32Klen; i+=4)
    {
        HAL_CIPHER_ReadReg((HI_U32 *)SEC_RSA_RRSLT_REG, &u32Value);  
        pPos[0] = (HI_U8)(u32Value & 0xFF);
        pPos[1] = (HI_U8)((u32Value >> 8) & 0xFF);
        pPos[2] = (HI_U8)((u32Value >> 16) & 0xFF);
        pPos[3] = (HI_U8)((u32Value >> 24) & 0xFF);
        pPos+=4; 
    }  
}

HI_U32 HAL_RSA_GetErrorCode(HI_VOID)
{
	HI_U32 u32Value;

    HAL_CIPHER_ReadReg((HI_U32 *)SEC_RSA_ERROR_REG, &u32Value);

    return u32Value;
}

