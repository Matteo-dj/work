/*includes*/
#include "hi_mdm.h"
#include "spifdrv.h"
#include <Os_res_cfg.h>
#include <os_vx_sal_api.h>
#include <Hi_driver_internal.h>
#include <hi_low_power.h>
#include <drv_dfx_sys_val.h>
#include <spiflash.h>
#include <spif_protect_drv.h>


#define SPIF_DEV_NAME       "sfc"
#define SPIF_DEV_DH         (16)
#define SPIF_ALIGN_BYTES    (4)

HI_PRV HI_BOOL s_bSpiInitFlag = HI_FALSE;
HI_PRV SPI_FLASH_CTRL_S *s_pstSpiflashCtrl = {0};
HI_PRV HI_U8 s_aucFlashCache[PRODUCT_CFG_FLASH_BLOCK_SIZE] = {0};
HI_PRV HI_U8 s_aucFlashReadCache[PRODUCT_CFG_FLASH_BLOCK_SIZE] = {0};
HI_U32 s_stFlashSem = 0;



HI_PRV HI_VOID flash_reset(HI_BOOL bDoRst)
{
#ifdef  FLASH_LOWPOWER
    HI_U32 ulVal = 0;
    /* sys reset */
    ulVal = HI_REG_READ_VAL32(HI_SYSCTRL_REG_BASE + 0x2c);
    ulVal &= ~(1 << 4);
    ulVal |= ((HI_U32)bDoRst << 4);
    HI_REG_WRITE32(HI_SYSCTRL_REG_BASE + 0x2c, ulVal);
#endif
}

HI_PRV HI_VOID flash_clk_disable(HI_VOID)
{
#ifdef  FLASH_LOWPOWER
    HI_U32 ulVal = 0;
    ulVal = HI_REG_READ_VAL32(HI_SYSCTRL_REG_BASE + 0x20);
    ulVal &= (~(1 << 4));
    HI_REG_WRITE32(HI_SYSCTRL_REG_BASE + 0x20, ulVal);
#endif
}
HI_PRV HI_VOID flash_clk_enable(HI_VOID)
{
#ifdef  FLASH_LOWPOWER
    HI_U32 ulVal = 0;
    ulVal = HI_REG_READ_VAL32(HI_SYSCTRL_REG_BASE + 0x20);
    ulVal |= (1 << 4);
    HI_REG_WRITE32(HI_SYSCTRL_REG_BASE + 0x20, ulVal);
#endif
}
HI_PRV HI_VOID flash_sys_init(HI_VOID)
{
    /* reset chip */
    flash_reset(HI_TRUE);
    /* disable clk */
    flash_clk_disable();
    /* enable clk */
    flash_clk_enable();
    /* cancle chip reset state */
    flash_reset(HI_TRUE);
    flash_reset(HI_FALSE);
}

HI_PRV HI_VOID flash_sys_exit(HI_VOID)
{
    /* reset chip */
    flash_reset(HI_TRUE);
    /* disable clk */
    flash_clk_disable();
}


HI_U32 HI_FLASH_Init(HI_VOID)
{   
    HI_U32 ulRet = HI_ERR_FAILURE;
    
    if (HI_TRUE == s_bSpiInitFlag)
    {
        return HI_ERR_SUCCESS;
    }
    
    flash_sys_init();
    s_pstSpiflashCtrl = spi_flash_probe();
    if (!s_pstSpiflashCtrl)
    {
        flash_sys_exit();
        return HI_ERR_FAILURE;
    }
    
    if(s_pstSpiflashCtrl->ulEraseSize < PRODUCT_CFG_FLASH_BLOCK_SIZE)
    {
        return HI_ERR_INVALID_PRODUCT_CONFIG;
    }  

    s_pstSpiflashCtrl->pTempBuffer = (HI_U8*)s_aucFlashCache;
    DBG_MSG("Sector size:%xh\r\n",s_pstSpiflashCtrl->ulEraseSize);
    DBG_MSG("tempbuf addr:%10p\r\n",s_pstSpiflashCtrl->pTempBuffer);
    if (s_pstSpiflashCtrl->pTempBuffer == HI_NULL)
    {
        flash_sys_exit();
        return HI_ERR_FAILURE;
    }
   
    ulRet = HI_MDM_SEM_BCreate(&s_stFlashSem, "flash_sem", HI_TRUE);
    if (HI_ERR_SUCCESS != ulRet)
    {
        return ulRet; 
    }
    
    s_bSpiInitFlag = HI_TRUE;
    HI_MDM_ReportInitEvt(HI_DEID_DRV_FLASH_INIT_OK, 1);	
    return HI_ERR_SUCCESS;
}
HI_U32 HI_FLASH_DeInit(HI_VOID)
{
    HI_U32 ulRet = HI_ERR_FAILURE;
    
    if (HI_TRUE != s_bSpiInitFlag)
    {
        //返回没有初始化
        return HI_ERR_NO_INITILIZATION;
    }
    
    ulRet = HI_MDM_SEM_Delete(s_stFlashSem);
    if (HI_ERR_SUCCESS != ulRet)
    {
        return ulRet; 
    }
    
    s_bSpiInitFlag = HI_FALSE;
    flash_sys_exit();
    return HI_ERR_SUCCESS;
}

HI_U32 flash_check_para(SPI_FLASH_CTRL_S *pstSpiFlashCtrl, HI_U32 ulByteOffAddr, HI_U32 ulByteSize)
{
    if (((ulByteOffAddr + ulByteSize) > pstSpiFlashCtrl->ulSize)
        ||((ulByteOffAddr + ulByteSize) < ulByteOffAddr)
        ||((ulByteOffAddr + ulByteSize) < ulByteSize))
    {
        HI_MDM_ReportSysEvent(HI_DEID_DRV_FLASH_OP_ADDR_BEYOND);
        return HI_ERR_FLASH_ADDR_BEYOND;
    }
    
    return HI_ERR_SUCCESS;
}

HI_U32 flash_erase_one_sector(SPI_FLASH_CTRL_S *pstSpiFlashCtrl, HI_U32 ulOpSector, HI_BOOL bDoErase)
{    
    HI_U32 ulRet = HI_ERR_FAILURE;
    
    if (bDoErase)
    {
        ulRet = pstSpiFlashCtrl->pErase(pstSpiFlashCtrl, ulOpSector * pstSpiFlashCtrl->ulEraseSize, pstSpiFlashCtrl->ulEraseSize, HI_FALSE);
        if (HI_ERR_SUCCESS != ulRet)
        {
            return ulRet;
        }
    }
    
    return HI_ERR_SUCCESS;
}

HI_U32 flash_wrt_and_check(HI_U8* pWrite, HI_U32 ulFlashAddr, HI_U32 ulLenth)
{
	HI_U32 ulRet = HI_ERR_FAILURE;
	HI_U32 ulLenthTmp = 0;
	HI_U32 ulBufferSeek = 0;
	HI_U32 ulByteOffset = ulFlashAddr % SPIF_ALIGN_BYTES;
	HI_U32 ulflashAddr1st = 0;
	HI_U32 ulFlashOffset1st = 0;
	HI_U32 ulFlashAddr2nd = 0;
	HI_U32 ulFlashOffset2nd = 0;
	HI_U8* pMemAddr = pWrite;
	HI_U8  aucTmpBufferRead[SPIF_ALIGN_BYTES] = {0};
	SPI_FLASH_CTRL_S *pstSpiFlashCtrl = s_pstSpiflashCtrl;

	if(ulByteOffset)
	{
		/* 计算第1段的地址及长度 */
		ulflashAddr1st = ulFlashAddr;
		ulFlashOffset1st = ((SPIF_ALIGN_BYTES - ulByteOffset) >= ulLenth) ? ulLenth: (SPIF_ALIGN_BYTES - ulByteOffset);

		/* 写4字节不对齐的第一部分，并回读校验 */
		ulRet = pstSpiFlashCtrl->pWrite(pstSpiFlashCtrl, ulflashAddr1st, ulFlashOffset1st, pMemAddr, HI_FALSE);
		if (HI_ERR_SUCCESS != ulRet)
		{
			return ulRet;
		}

		ulRet = pstSpiFlashCtrl->pRead(pstSpiFlashCtrl, ulflashAddr1st, ulFlashOffset1st, aucTmpBufferRead, HI_FALSE);
		if (HI_ERR_SUCCESS != ulRet)
		{
			return ulRet;
		}

		ulRet = (HI_U32)memcmp(pMemAddr, aucTmpBufferRead, ulFlashOffset1st);
		if (HI_ERR_SUCCESS != ulRet)
		{
			return HI_ERR_FLASH_DATA_CHECK_WRONG;
		}

		/* 如果在4字节范围内，则直接返回 */
		if(ulFlashOffset1st == ulLenth)
		{
			return HI_ERR_SUCCESS;
		}

		/* 计算第2段的地址及长度，并更新内存地址 */
		ulFlashAddr2nd		= ulflashAddr1st + SPIF_ALIGN_BYTES - ulByteOffset;
		ulFlashOffset2nd	= ulLenth - ulFlashOffset1st;
		pMemAddr			= pMemAddr + ulFlashOffset1st;
	}
	else
	{
		/* 计算第2段的地址及长度 */
		ulFlashAddr2nd		= ulFlashAddr;
		ulFlashOffset2nd	= ulLenth;
	}

	/* 第2段的写 */
	ulRet = (HI_U32)pstSpiFlashCtrl->pWrite(pstSpiFlashCtrl, ulFlashAddr2nd, ulFlashOffset2nd, pMemAddr, HI_FALSE);
	if (HI_ERR_SUCCESS != ulRet)
	{
		return ulRet;
	}

	/* 第2段的回读校验，由于内存限制，每4K循环校验 */
	for(ulLenthTmp = 0; ulLenthTmp < ulFlashOffset2nd; (ulLenthTmp +=  PRODUCT_CFG_FLASH_BLOCK_SIZE))
	{
		ulBufferSeek = ((ulFlashOffset2nd - ulLenthTmp) > PRODUCT_CFG_FLASH_BLOCK_SIZE) ? PRODUCT_CFG_FLASH_BLOCK_SIZE : (ulFlashOffset2nd - ulLenthTmp);
		ulRet = pstSpiFlashCtrl->pRead(pstSpiFlashCtrl, ulFlashAddr2nd + ulLenthTmp, ulBufferSeek, s_aucFlashReadCache, HI_FALSE);
		if (HI_ERR_SUCCESS != ulRet)
		{
			return ulRet;
		}

		ulRet = (HI_U32)memcmp((pMemAddr + ulLenthTmp), s_aucFlashReadCache, ulBufferSeek);
		if (HI_ERR_SUCCESS != ulRet)
		{
			return HI_ERR_FLASH_DATA_CHECK_WRONG;
		}
	}

	return HI_ERR_SUCCESS;
}


/*
** SPIFLASH 写函数
*/
HI_U32 flash_write_prv(HI_IN HI_U32 ulByteOffAddr, HI_IN HI_U8 *pBuf, HI_IN HI_U32 ulByteSize, HI_IN HI_BOOL bDoErase)
{
	HI_U32 ulRet = HI_ERR_SUCCESS;
	HI_U32 ulOffsetInSector = 0;//块内偏移
	HI_U32 ulEndAddr = 0;//实际结束地址
	HI_U32 ulOpSector = 0;//当前操作块数
	HI_U32 ulEndInSector = 0;//sector内部结束地址
	HI_U32 ulOpSize = 0;
	HI_U32 ulBytes2Wrt = ulByteSize;
	HI_U8 *pTempBuf = HI_NULL;//用于存储读回数据，更新后写到flash中
	HI_U8 *pInBuf = (HI_U8 *)pBuf;
	SPI_FLASH_CTRL_S *pstSpiFlashCtrl = s_pstSpiflashCtrl;
	
	if(!bDoErase)
	{
		return flash_wrt_and_check(pInBuf, ulByteOffAddr, ulBytes2Wrt);
	}
	else
	{
		/* config parameter */
		ulOpSector = ulByteOffAddr / pstSpiFlashCtrl->ulEraseSize;
		ulOffsetInSector = ulByteOffAddr % pstSpiFlashCtrl->ulEraseSize;
		ulEndAddr = ulByteOffAddr + ulBytes2Wrt;//实际应该再减1;
		ulEndInSector = (ulOpSector + 1) * pstSpiFlashCtrl->ulEraseSize;//实际结束地址是再减1;
		pTempBuf = pstSpiFlashCtrl->pTempBuffer;
		DBG_MSG("WriteInfo:startadd:%08p,size:%xh\r\n", ulByteOffAddr, ulBytes2Wrt);

		/* if not aligned with sector */
		if (0 != ulOffsetInSector)
		{	
			/* buffer op */
			ulRet = pstSpiFlashCtrl->pRead(pstSpiFlashCtrl, ulOpSector * pstSpiFlashCtrl->ulEraseSize, pstSpiFlashCtrl->ulEraseSize, pTempBuf, HI_FALSE);
			if (HI_ERR_SUCCESS != ulRet)
			{
				return ulRet;
			}
		
			/* copy to temp */
			pTempBuf += ulOffsetInSector;
			ulOpSize = (ulEndInSector < ulEndAddr) ? (ulEndInSector - ulByteOffAddr) : ulBytes2Wrt;
		    (hi_void)memcpy_s(pTempBuf, ulOpSize, pInBuf, ulOpSize);
			pTempBuf = pstSpiFlashCtrl->pTempBuffer;
			ulRet = flash_erase_one_sector(pstSpiFlashCtrl, ulOpSector, bDoErase);
			if (HI_ERR_SUCCESS != ulRet)
			{
				return ulRet;
			}

			ulRet = flash_wrt_and_check(pTempBuf, ulOpSector * pstSpiFlashCtrl->ulEraseSize, pstSpiFlashCtrl->ulEraseSize);
			DBG_MSG("sector:%d,size:%xh\r\n",ulOpSector,ulOpSize);
			if (HI_ERR_SUCCESS != ulRet)
			{
				return ulRet;
			}

			/* change para */
			ulOpSector += 1;  
			ulEndInSector += pstSpiFlashCtrl->ulEraseSize;
			pInBuf += ulOpSize;
			ulBytes2Wrt -= ulOpSize;
		}

		while (ulEndInSector <= ulEndAddr)
		{
			ulRet = flash_erase_one_sector(pstSpiFlashCtrl, ulOpSector, bDoErase);
			if (HI_ERR_SUCCESS != ulRet)
			{
				return ulRet;
			}
		
			/* direct op in buffer by sector */
		    (hi_void)memcpy_s(pTempBuf, pstSpiFlashCtrl->ulEraseSize, pInBuf, pstSpiFlashCtrl->ulEraseSize);
			ulRet = flash_wrt_and_check(pTempBuf, ulOpSector * pstSpiFlashCtrl->ulEraseSize, pstSpiFlashCtrl->ulEraseSize);
			if (HI_ERR_SUCCESS != ulRet)
			{
				return ulRet;
			}
		
			DBG_MSG("sector:%d,size:%xh\r\n", ulOpSector, pstSpiFlashCtrl->ulEraseSize);
			/* config para for next op */
			ulOpSector++;
			ulEndInSector += pstSpiFlashCtrl->ulEraseSize;
			pInBuf += pstSpiFlashCtrl->ulEraseSize;
			ulBytes2Wrt -= pstSpiFlashCtrl->ulEraseSize; 
		}

		/* return if buffer translate over */
		if (0 == ulBytes2Wrt)
		{
			return HI_ERR_SUCCESS;
		}

		/* write if buffer remain */
		ulRet = pstSpiFlashCtrl->pRead(pstSpiFlashCtrl, ulOpSector * pstSpiFlashCtrl->ulEraseSize, pstSpiFlashCtrl->ulEraseSize, pTempBuf, HI_FALSE);
		if (HI_ERR_SUCCESS != ulRet)
		{
			return ulRet;
		}

		/* copy to temp */
	    (hi_void)memcpy_s(pTempBuf, ulBytes2Wrt, pInBuf, ulBytes2Wrt);
		DBG_MSG("sector:%d,size:%xh\r\n", ulOpSector, ulBytes2Wrt);
		ulRet = flash_erase_one_sector(pstSpiFlashCtrl, ulOpSector, bDoErase);
		if (HI_ERR_SUCCESS != ulRet)
		{
			return ulRet;
		}
	
		//参数与前次配置保持一致
		return flash_wrt_and_check(pTempBuf, ulOpSector * pstSpiFlashCtrl->ulEraseSize, pstSpiFlashCtrl->ulEraseSize);
	}
}

HI_U32 HI_FLASH_Write(HI_IN HI_U32 ulByteOffAddr, HI_IN HI_U8 *pBuf, HI_IN HI_U32 ulByteSize, HI_IN HI_BOOL bDoErase)
{
	HI_U32 ulRet = HI_ERR_FAILURE;
	HI_U32 ulBytes2Wrt = ulByteSize;
	SPI_FLASH_CTRL_S *pstSpiFlashCtrl = s_pstSpiflashCtrl;

	if (HI_TRUE != s_bSpiInitFlag)
	{
		return HI_ERR_NO_INITILIZATION;//返回没有初始化
	}
	
	if ((HI_NULL == pBuf) || (ulByteSize == 0))
	{
		return HI_ERR_INVALID_PARAMETER;
	}
		
	HI_DFX_MSG_FLASH_OP((HI_U8)1, ulByteOffAddr, ulBytes2Wrt);
    //protect boot
    ulRet = flash_check_para(pstSpiFlashCtrl, ulByteOffAddr, ulBytes2Wrt);
    if (HI_ERR_SUCCESS != ulRet)
    {
        DBG_MSG("Ret:%08p:Addr:%xh;Size:%xh\r\n", ulRet, ulByteOffAddr, ulBytes2Wrt);
        return ulRet;        
    }

    ulRet = FLASH_LOCK();
    if (HI_ERR_SUCCESS != ulRet)
    {
        return ulRet;        
    }
    
    HI_LOW_POWER_Forbid(HI_LOW_POWER_FORBID_ID_FLASH);

    
    //不判断返回值，假如写状态寄存器后一直busy可以接着去尝试写flash，可能会write成功的。因为flash不一定是被保护住的
#ifdef PRODUCT_CFG_FLASH_PROTECT
    (HI_VOID)spif_shrink_protect_area(ulByteOffAddr);//不判断返回值，即使解保护出错，写会回读比较    
#endif
    ulRet = flash_write_prv(ulByteOffAddr, pBuf, ulByteSize, bDoErase);
    HI_LOW_POWER_UnForbid(HI_LOW_POWER_FORBID_ID_FLASH,500);
    FLASH_UNLOCK();
    return ulRet;
}

//B017死机代码合入B019vx版本
HI_VOID flash_init_crash(HI_VOID)
{
    s_pstSpiflashCtrl = spi_flash_probe();
}

HI_U32 flash_read_crash(HI_IN HI_U32 ulByteOffAddr, HI_IN HI_VOID *pBuf, HI_IN HI_U32 ulByteSize)
{
    HI_U32 ulRet = HI_ERR_FAILURE;
    
    if (HI_NULL == pBuf)
    {
        return HI_ERR_INVALID_PARAMETER;
    }
        
    ulRet = spif_dma_read(s_pstSpiflashCtrl, ulByteOffAddr, ulByteSize, pBuf, 1);
    if (HI_ERR_SUCCESS != ulRet)
    {
        return ulRet;
    }
    
    return HI_ERR_SUCCESS;
}

HI_U32 flash_write_crash(HI_IN HI_U32 ulByteOffAddr, HI_IN HI_VOID *pBuf, HI_IN HI_U32 ulByteSize)
{
    HI_U32 ulRet = HI_ERR_FAILURE;
    HI_U32 op_bytes = ulByteSize;

    /*test para: size and off_addr test in erase/write function*/
    if (HI_NULL == pBuf)
    {
        return HI_ERR_INVALID_PARAMETER;
    }

    if (PRODUCT_CFG_FLASH_BLOCK_SIZE != s_pstSpiflashCtrl->ulEraseSize)
    {
        s_pstSpiflashCtrl->ulEraseSize = PRODUCT_CFG_FLASH_BLOCK_SIZE;
    }
    
	//protect boot
    if(ulByteSize % s_pstSpiflashCtrl->ulEraseSize)
    {
        op_bytes = s_pstSpiflashCtrl->ulEraseSize*(1 + ulByteSize / s_pstSpiflashCtrl->ulEraseSize);
    }

    ulRet = spif_reg_erase(s_pstSpiflashCtrl, ulByteOffAddr, op_bytes, HI_TRUE);
    if (HI_ERR_SUCCESS != ulRet)
    {
        return ulRet;
    }

    ulRet = spif_dma_write(s_pstSpiflashCtrl, ulByteOffAddr, ulByteSize, pBuf, HI_TRUE);
    if (HI_ERR_SUCCESS != ulRet)
    {
        return ulRet;
    }

    return HI_ERR_SUCCESS;
}

/*  flash 从1写到0只写不擦 */
HI_U32 flash_write_1_to_0(HI_IN HI_U32 ulByteOffAddr, HI_IN HI_VOID *pBuf, HI_IN HI_U32 ulByteSize)
{
    return spif_dma_write(s_pstSpiflashCtrl, ulByteOffAddr, ulByteSize, pBuf, HI_TRUE);
}

HI_PRV HI_U32 flash_erase_prv(HI_IN HI_U32 ulByteOffAddr, HI_IN HI_U32 ulByteSize)
{
	SPI_FLASH_CTRL_S *pstSpiFlashCtrl = s_pstSpiflashCtrl;

	return pstSpiFlashCtrl->pErase(pstSpiFlashCtrl, ulByteOffAddr, ulByteSize, HI_FALSE);	 
}

HI_U32 HI_FLASH_Erase(HI_IN HI_U32 ulByteOffAddr,HI_IN HI_U32 ulByteSize)
{
	HI_U32 ulRet = HI_ERR_FAILURE;
	SPI_FLASH_CTRL_S *pstSpiFlashCtrl = s_pstSpiflashCtrl;

	if (HI_TRUE != s_bSpiInitFlag)
	{
		return HI_ERR_NO_INITILIZATION;
	}
	//protect boot
	if	(0 == ulByteSize)
	{
		return HI_ERR_INVALID_PARAMETER;
	}

	 

	ulRet = flash_check_para(pstSpiFlashCtrl, ulByteOffAddr, ulByteSize);
	if (HI_ERR_SUCCESS != ulRet)
	{
		DBG_MSG("Ret:%08p:Addr:%xh;Size:%xh\r\n",ulRet,ulByteOffAddr,ulByteSize);
		return ulRet;		
	}
	
	if ((ulByteOffAddr%pstSpiFlashCtrl->ulEraseSize)||(ulByteSize%pstSpiFlashCtrl->ulEraseSize))
	{
		return HI_ERR_INVALID_PARAMETER;
	}
	
	HI_DFX_MSG_FLASH_OP((HI_U8)2, ulByteOffAddr, ulByteSize );
	ulRet = FLASH_LOCK();
	if (HI_ERR_SUCCESS != ulRet)
	{
		return ulRet;		 
	}
	HI_LOW_POWER_Forbid(HI_LOW_POWER_FORBID_ID_FLASH);   
#ifdef PRODUCT_CFG_FLASH_PROTECT
    ulRet = spif_shrink_protect_area(ulByteOffAddr);
    if(HI_ERR_SUCCESS != ulRet)
    {
        HI_LOW_POWER_UnForbid(HI_LOW_POWER_FORBID_ID_FLASH,1000);
        FLASH_UNLOCK(); 
        return HI_ERR_FAILURE;//解保护出错的话，就让擦接口返错，防止出现:实际擦失败，但是接口返回成功，导致一些容错机制或者保护机制被绕过去了。
    }
#endif
    // TODO:暂时返错处理，防止失败了，但是擦接口返回成功,原来机制若有恢复机制就执行不到了

    ulRet = flash_erase_prv(ulByteOffAddr, ulByteSize);
    HI_LOW_POWER_UnForbid(HI_LOW_POWER_FORBID_ID_FLASH,1000);
    FLASH_UNLOCK();    
    return ulRet; 
}

/*
** SPIFLASH 读函数
*/
HI_PRV HI_U32 flash_read_prv(HI_IN HI_U32 ulByteOffAddr, HI_OUT HI_U8 *pBuf, HI_IN HI_U32 ulByteSize)
{
    HI_U8 *pOutBuf = pBuf;
    HI_U32 ulByteOffset = ulByteOffAddr % SPIF_ALIGN_BYTES;
    HI_U32 ulFlashAddr1st = 0;
    HI_U32 ulFlashOffset1st = 0;
    HI_U32 ulFlashAddr2nd = 0;
    HI_U32 ulFlashOffset2nd = 0;
    HI_U32 ulFlashByteSize = ulByteSize;
    HI_U32 ulRet = HI_ERR_FAILURE;
    SPI_FLASH_CTRL_S *pstSpiFlashCtrl = s_pstSpiflashCtrl;

    if(ulByteOffset)
    {
        /* 计算第1段的地址及长度 */
        ulFlashAddr1st = ulByteOffAddr;
        ulFlashOffset1st = ((SPIF_ALIGN_BYTES - ulByteOffset) >= ulFlashByteSize) ? ulFlashByteSize : (SPIF_ALIGN_BYTES - ulByteOffset);

        /* 读出第1段 */
        ulRet = pstSpiFlashCtrl->pRead(pstSpiFlashCtrl, ulFlashAddr1st, ulFlashOffset1st, pOutBuf, HI_FALSE);
        if (HI_ERR_SUCCESS != ulRet)
        {
            return ulRet;
        }

        /* 如果第一段在4字节范围内，则直接返回 */
        if(ulFlashOffset1st == ulFlashByteSize)
        {
            return HI_ERR_SUCCESS;
        }

        /* 计算第2段的地址及长度 */
        ulFlashAddr2nd = ulFlashAddr1st + SPIF_ALIGN_BYTES - ulByteOffset;
        ulFlashOffset2nd = ulFlashByteSize - ulFlashOffset1st;

        /* 读出第2段 */
        ulRet = pstSpiFlashCtrl->pRead(pstSpiFlashCtrl, ulFlashAddr2nd, ulFlashOffset2nd, (pOutBuf + ulFlashOffset1st), HI_FALSE);
    }
    else
    {
        ulRet = pstSpiFlashCtrl->pRead(pstSpiFlashCtrl, ulByteOffAddr, ulFlashByteSize, pOutBuf, HI_FALSE);
    }

    return ulRet;
}


HI_U32 HI_FLASH_Read(HI_IN HI_U32 ulByteOffAddr, HI_OUT HI_U8 *pBuf, HI_IN HI_U32 ulByteSize)
{
    SPI_FLASH_CTRL_S *pstSpiFlashCtrl = s_pstSpiflashCtrl;
    HI_U32 ulRet = HI_ERR_FAILURE;

    if (HI_TRUE != s_bSpiInitFlag)
    {
        return HI_ERR_NO_INITILIZATION;
    }

    /* test parameter */
    if ((HI_NULL == pBuf) || (0 == ulByteSize))
    {
        return HI_ERR_INVALID_PARAMETER;
    }

     
    HI_DFX_MSG_FLASH_OP((HI_U8)0, ulByteOffAddr, ulByteSize);
    ulRet = flash_check_para(pstSpiFlashCtrl, ulByteOffAddr, ulByteSize);
    if (HI_ERR_SUCCESS != ulRet)
    {
    	DBG_MSG("Ret:%08p:Addr:%xh;Size:%xh\r\n", ulRet, ulByteOffAddr, ulByteSize);
        return ulRet;
    }
    
    ulRet = FLASH_LOCK();
    if (HI_ERR_SUCCESS != ulRet)
    {
        return ulRet;        
    }
        
    HI_LOW_POWER_Forbid(HI_LOW_POWER_FORBID_ID_FLASH);
    ulRet = flash_read_prv(ulByteOffAddr, pBuf, ulByteSize);
    HI_LOW_POWER_UnForbid(HI_LOW_POWER_FORBID_ID_FLASH,50);
    FLASH_UNLOCK();



    
    return ulRet;
}

HI_U32 flash_wrt_phy_ioctl(HI_U16 usCommand, HI_VOID *pBuf)
{
    HI_U32 ulRet = HI_ERR_SUCCESS;
    SPI_FLASH_CTRL_S *pstSpiFlashCtrl = s_pstSpiflashCtrl;
    HI_FLASH_INFO_S *disk_info = (HI_FLASH_INFO_S *)pBuf;
    if (pstSpiFlashCtrl != HI_NULL)
    {
        if ((pstSpiFlashCtrl->ulEraseSize == 0) || (pstSpiFlashCtrl->ulWriteSize == 0) || (pstSpiFlashCtrl->ulSize == 0))
        {
            return HI_ERR_FLASH_INVALID_SIZE;
        }

        switch (usCommand)
        {
            case HI_GET_FLASH_INFO:
                disk_info->ulTotalSize= pstSpiFlashCtrl->ulSize;
                disk_info->ulSectorSize= pstSpiFlashCtrl->ulEraseSize;
                disk_info->ulPageSize= pstSpiFlashCtrl->ulWriteSize;
                break;
            default:
/*
                                        【屏蔽人】z00193780
                                        【审核人】c00233580
                                        【代码分析】二次开发接口，需要进行参数判断，usCommand参数有可能会扩展， 需保留。
                                        【功能影响】不影响正常业务，屏蔽告警
*/                
                ulRet = HI_ERR_INVALID_PARAMETER;
                break;
        }
    }

    return ulRet;
}

HI_U32 HI_FLASH_Ioctl(HI_IN HI_U16 usCommand, HI_OUT HI_FLASH_INFO_S *pstFlashInfo)
{
    HI_U32 ulRet = HI_ERR_FAILURE;

    if (HI_TRUE != s_bSpiInitFlag)
    {
        return HI_ERR_NO_INITILIZATION;
    }

    if (HI_NULL == pstFlashInfo)
    {
        return HI_ERR_INVALID_PARAMETER;
    }

    ulRet = FLASH_LOCK();
    if (HI_ERR_SUCCESS != ulRet)
    {
        return ulRet;        
    }
    
    ulRet = flash_wrt_phy_ioctl(usCommand, pstFlashInfo);   
    FLASH_UNLOCK();
    return ulRet;
}

