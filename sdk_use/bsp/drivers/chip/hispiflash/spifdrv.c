/******************************************************************************
 *    Copyright (c) 2009-2010 by czy.
 *    All rights reserved.
 * ***
 *      Create by Cai ZhiYong. 2010-12-15
 *      changed by wuxianfeng2011.09.14
 *      changed at 2011.10.18
 *
 ******************************************************************************/

/*****************************************************************************/
#include "hi_mdm.h"
#include "hi_platform.h"
#include <hi_driver_internal.h>
#include "spifdrv.h"
#ifdef PRODUCT_CFG_FLASH_ERASE_COUNT 
#include "drv_dfx.h"
#endif
#include "os_vx_sal_api.h"



#ifdef PRODUCT_CFG_FLASH_PROTECT
#include <spif_protect_drv.h>
#endif
/*****************************************************************************/
#define HISFC300_SUPPORT_READ             (SPI_IF_READ_STD)
#define HISFC300_SUPPORT_MAX_DUMMY        (1)
#define HISFC300_SUPPORT_WRITE            (SPI_IF_WRITE_STD)

#ifdef PRODUCT_CFG_FLASH_ERASE_COUNT 
HI_U16 g_aulFlashEraseCount[FLASH_TOTAL_SECTOR_NUM] = {0};
#endif

/*
**��ʱ(�Ǿ�׼)
*/
/*****************************************************************************/
SFC_HOST_S g_stSfc300Host = {{{0}}, HI_NULL, 0, HI_NULL, HI_NULL, 0, HI_NULL, 0, 0, {{0}}};
HI_PRV HI_CHAR s_ascCrashDmaBuf[HISFC300_DMA_MAX_SIZE + CACHE_LINE_SIZE];
/*******************************************************************************
FUNCTION:		spif_wait_ready
DESCRIPTION:	
INPUT:
OUTPUT:	
********************************************************************************/
HI_S32 spif_wait_ready(SFC_HOST_S *pstHost, SPI_FLASH_INFO_S *pstInfo, HI_BOOL bIsCrash)
{
	HI_U32 ulRegVal = 0;
	HI_U32 ulDeadline = 0;
	HI_U32 ulTimeOut = 400;
    HI_U32 ulChipSelectTemp = 0;
	if (bIsCrash)
	{
		ulTimeOut = (40 << 20);
	}

	do
	{
		hisfc_write(pstHost,SFC_REG_CMD_INS, SPI_CMD_RDSR);
        ulChipSelectTemp = (HI_U32)pstInfo->slChipSelect;
		hisfc_write(pstHost,SFC_REG_CMD_CONFIG,
				((HI_U32)SFC_CMD_CONFIG_SEL_CS(ulChipSelectTemp))
				| SFC_CMD_CONFIG_DATA_CNT(1)/*lint !e778*/
				| SFC_CMD_CONFIG_DATA_EN
				| SFC_CMD_CONFIG_RW
				| SFC_CMD_CONFIG_START);		 

		while ((hisfc_read(pstHost, SFC_REG_CMD_CONFIG) & SFC_CMD_CONFIG_START)) {;}
		ulRegVal = hisfc_read(pstHost, SFC_REG_CMD_DATABUF1);
		if (!(ulRegVal & SPI_CMD_SR_WIP))
		{
			return HI_ERR_SUCCESS;
		}
		if (bIsCrash)
		{
			HI_HRTIMER_Udelay(1);
		}
		else
		{
			HI_MDM_Sleep(10);// 1 tick,10ms
		}
	} while (ulDeadline++ < ulTimeOut);

	HI_MDM_ReportSysEvent(HI_DEID_DRV_FLASH_WAIT_TIMEOUT);
	DBG_MSG("!!! Wait spi flash ready timeout.\n");
	return HI_ERR_TIMEOUT;
}


HI_PRV HI_S32 spif_probe_ready(HI_VOID)
{
	HI_U32 ulRegVal = 0;
	HI_U32 ulDeadline = 0;
    HI_U32 ulTimeOut = (40 << 20);

	do
	{
		HI_REG_WRITE32(HI_SFC_REG_BASE + SFC_REG_CMD_INS, SPI_CMD_RDSR);
		HI_REG_WRITE32(HI_SFC_REG_BASE + SFC_REG_CMD_CONFIG,
				SFC_CMD_CONFIG_SEL_CS(1)  //Ƭѡ�̶�Ϊ1
				| SFC_CMD_CONFIG_DATA_CNT(1)/*lint !e778*/ 
				| SFC_CMD_CONFIG_DATA_EN
				| SFC_CMD_CONFIG_RW
				| SFC_CMD_CONFIG_START);		 

		while ((HI_REG_READ_VAL32(HI_SFC_REG_BASE + SFC_REG_CMD_CONFIG) & SFC_CMD_CONFIG_START))
			;
		ulRegVal = HI_REG_READ_VAL32(HI_SFC_REG_BASE + SFC_REG_CMD_DATABUF1);
		if (!(ulRegVal & SPI_CMD_SR_WIP))
			return HI_ERR_SUCCESS;

        HI_HRTIMER_Udelay(1);
	} while (ulDeadline++ < ulTimeOut);

	return HI_ERR_TIMEOUT;
}

HI_PRV HI_U32 spif_4byte_mode_enable(SFC_HOST_S *pstHost, SPI_FLASH_INFO_S *pstInfo)
{
	HI_U32 ulRegVal = 0;
	HI_U32 ulChipSelectTemp = 0;
    
	if (pstInfo->ulAddrCycle != 4)
	{
		DBG_BUG("pstInfo->ulAddrCycle != 4\n", pstInfo->ulAddrCycle);
		return HI_ERR_INVALID_PARAMETER;
	}

	// дʹ��
	spif_write_enable(pstHost, pstInfo);
	hisfc_write(pstHost, SFC_REG_CMD_INS, SPI_CMD_EN4B);
    ulChipSelectTemp = (HI_U32)pstInfo->slChipSelect;
	hisfc_write(pstHost, SFC_REG_CMD_CONFIG,
			((HI_U32)SFC_CMD_CONFIG_SEL_CS(ulChipSelectTemp)) 
			| SFC_CMD_CONFIG_START);

	while ((hisfc_read(pstHost, SFC_REG_CMD_CONFIG) & SFC_CMD_CONFIG_START))
	{
		;
	}

	ulRegVal = hisfc_read(pstHost, SFC_REG_GLOBAL_CONFIG);  
	ulRegVal |= SFC_REG_GLOBAL_CONFIG_ADDR_MODE_4B;
	hisfc_write(pstHost, SFC_REG_GLOBAL_CONFIG, ulRegVal);
	return HI_ERR_SUCCESS;
}

/*******************************************************************************
FUNCTION:		spif_config
DESCRIPTION:	
INPUT:		*host :ulBaseAddr
			*spi: have or not
			*op:iftype and dummy
			clk_en:no used
			writecmd
			readcmd			
OUTPUT:		none
********************************************************************************/
HI_PRV HI_VOID spif_config(SFC_HOST_S *pstHost, SPI_FLASH_INFO_S *pstInfo,
		SPI_OPERATION_S *pstOperation, HI_S32 slClkEn, HI_S32 slWriteCmd, HI_S32 slReadCmd)
{
	HI_U32 ulRegVal = 0;

	/* set BUS_CONFIG */ 
	ulRegVal = hisfc_read(pstHost, SFC_REG_BUS_CONFIG1);
	if (pstInfo && pstOperation)
	{
		ulRegVal = SFC_BUS_CONFIG1_RD_MEM_IF_TYPE(pstOperation->ucIfType) 
			| SFC_BUS_CONFIG1_RD_DUMMY_BYTES(pstOperation->ucDummy)
			| SFC_BUS_CONFIG1_RD_PREFETCH_CNT(0)/*Ԥȡ����*/
			| SFC_BUS_CONFIG1_RD_INS(slReadCmd)
			| SFC_BUS_CONFIG1_WR_INS(slWriteCmd)
			| SFC_BUS_CONFIG1_RD_ENABLE;/*ʹ�����߶�*//*lint !e648*/
	}
    
	hisfc_write(pstHost, SFC_REG_BUS_CONFIG1, ulRegVal);

	/* set CMD_CONFIG */
	ulRegVal = hisfc_read(pstHost, SFC_REG_CMD_CONFIG); 
	if (pstInfo && pstOperation)
	{
		ulRegVal = SFC_CMD_CONFIG_MEM_IF_TYPE(pstOperation->ucIfType) 
			    | SFC_CMD_CONFIG_DUMMY_BYTE_CNT(pstOperation->ucDummy);
	}
    
	hisfc_write(pstHost, SFC_REG_CMD_CONFIG, ulRegVal);
}

/*****************************************************
* spif_map_iftype_and_clock
* ����:�趨��д�߷�ʽ
* ��spif_probeflash ����
*****************************************************/
HI_PRV HI_VOID spif_map_iftype_and_clock(IN OUT SPI_FLASH_INFO_S *pstInfo)
{
	HI_S32 i = 0;
	HI_U8 aucIfTypeRead[] = 
    {
		SPI_IF_READ_STD, 0, 
		SPI_IF_READ_FAST, 0,
		SPI_IF_READ_DUAL, 1,
		SPI_IF_READ_QUAD, 5,
		SPI_IF_READ_QUAD_ADDR, 6,
		0, 0,
	};
    
	for (i = 0; aucIfTypeRead[i]; i += 2)
	{	/* �ж϶��ߵķ�ʽ */
		if (pstInfo->stRead->ucIfType == aucIfTypeRead[i])
		{
			pstInfo->stRead->ucIfType = aucIfTypeRead[i + 1];
			break;
		}
	}

	/* std write */
	pstInfo->stWrite->ucIfType = 0;
}

/*******************************************************************************
FUNCTION:		spif_dma_transfer
DESCRIPTION:	
INPUT: dir 1 = read��0 = write��
OUTPUT:	
********************************************************************************/
HI_PRV HI_VOID spif_dma_transfer(HI_U32 ulFlAddr, HI_U8 ucDir, HI_U32 ulSize, HI_U8 ucCs, SFC_HOST_S *pstHost)
{
    HI_MDM_DataCacheFlush((HI_PVOID)pstHost->ulDmabuffer, (HI_S32)ulSize);
	hisfc_write(pstHost, SFC_REG_BUS_DMA_MEM_SADDR, pstHost->ulDmabuffer);
	hisfc_write(pstHost, SFC_REG_BUS_DMA_FLASH_SADDR, ulFlAddr);
	hisfc_write(pstHost, SFC_REG_BUS_DMA_LEN, ulSize - 1);/* �ֽ���*/
	hisfc_write(pstHost, SFC_REG_BUS_DMA_AHB_CTRL, SFC_BUS_DMA_AHB_CTRL_INCR4_EN 
			| SFC_BUS_DMA_AHB_CTRL_INCR8_EN
			| SFC_BUS_DMA_AHB_CTRL_INCR16_EN);
	hisfc_write(pstHost, SFC_REG_BUS_DMA_CTRL, SFC_BUS_DMA_CTRL_READ(ucDir) 
			| SFC_BUS_DMA_CTRL_CS(1 - ucCs));
	hisfc_write(pstHost, SFC_REG_BUS_DMA_CTRL,hisfc_read(pstHost, SFC_REG_BUS_DMA_CTRL)
			| SFC_BUS_DMA_CTRL_START);
	/* ������ɲ������˳�ѭ��*/
	while (!(hisfc_read(pstHost, SFC_REG_INT_RAW_STATUS) & (SFC_REG_INT_RAW_STATUS_DMA_DONE)))
		;
    
	/* ���жϱ�־*/
	hisfc_write(pstHost, SFC_REG_INT_CLEAR, SFC_REG_INT_CLEAR_DMA_DONE);
    
    /*inval dcache*/
    HI_MDM_DataCacheInvalidate((HI_PVOID)pstHost->ulDmabuffer, (HI_S32)ulSize);
    
}
/*******************************************************************************
FUNCTION:		spif_dma_read
DESCRIPTION:	
INPUT:	*pstSpiFlash
		from:address of flash ?
		len
		*buf
OUTPUT:	
********************************************************************************/
HI_U32 spif_dma_read(SPI_FLASH_CTRL_S *pstSpiFlashCtrl, HI_U32 ulFrom, HI_U32 ulLen, HI_VOID *pBuf, HI_BOOL bIsCrash)
{
	HI_U8 ucCs = 0;
	HI_U32 ulNum = 0;
	HI_CHAR *ptr = (HI_CHAR *)pBuf;
	SFC_HOST_S *pstHost = SPIFLASH_TO_HOST(pstSpiFlashCtrl);
	SPI_FLASH_INFO_S *pstInfo = pstHost->stSpiFlashInfo;

	if (((ulFrom + ulLen) > pstSpiFlashCtrl->ulSize) || (!ulLen))
	{
		DBG_MSG("read area out of range.\r\n");
		return HI_ERR_INVALID_PARAMETER;
	}

	if (spif_wait_ready(pstHost, pstInfo, bIsCrash))
		goto fail;
	spif_config(pstHost, pstInfo, pstInfo->stRead, 1, 0, pstInfo->stRead->ucCmd);

	while (ulLen > 0)
	{
		while (ulFrom >= pstInfo->ulChipSize)
		{
			ulFrom -= pstInfo->ulChipSize;
			pstInfo++;
			ucCs++;
			if (!pstInfo->pName)
			{
				DBG_BUG("read memory out of range.\n");
			}
			if (spif_wait_ready(pstHost, pstInfo, bIsCrash))
				goto fail;
			spif_config(pstHost, pstInfo, pstInfo->stRead, 1, 0, pstInfo->stRead->ucCmd);
		}

		ulNum = ((ulFrom + ulLen) >= pstInfo->ulChipSize) ? (pstInfo->ulChipSize - ulFrom) : ulLen;
		while (ulNum >= HISFC300_DMA_MAX_SIZE)
		{
			/*dma read operation*/
			spif_dma_transfer(ulFrom, 1, HISFC300_DMA_MAX_SIZE, ucCs, pstHost);
		    (hi_void)memcpy_s(ptr, HISFC300_DMA_MAX_SIZE, pstHost->pBuffer, HISFC300_DMA_MAX_SIZE);
			ptr += HISFC300_DMA_MAX_SIZE;
			ulFrom += HISFC300_DMA_MAX_SIZE;
			ulLen -= HISFC300_DMA_MAX_SIZE;
			ulNum -= HISFC300_DMA_MAX_SIZE;
		}

		if (ulNum)
		{
			spif_dma_transfer(ulFrom, 1, ulNum, ucCs, pstHost);
		    (hi_void)memcpy_s(ptr, ulNum, pstHost->pBuffer, ulNum);
			ulFrom += ulNum;
			ptr += ulNum;
			ulLen -= ulNum;
		}
	}
	
	return HI_ERR_SUCCESS;
fail:
	return HI_ERR_FLASH_READ_WRONG;
}

/*****************************************************************************
* spif_read_ids
* ����:��ȡspif��id
*
* ���ܣ����ؾ���ID ���ݾ���Ӳ�������޸�
*
* 1.	дINSTR�Ĵ�����ID����
* 2.	дCMDƬѡ�������ݺ�ʹ������
* 3.	˳���DATABUFFER1����ID
*
**************************************************************************/
HI_PRV HI_U8 *spif_read_ids(SFC_HOST_S *pstHost, HI_S32 slChipSelect, HI_U8 *pBuffer, HI_S32 slNumRead)
{
	HI_U32 ulRegIndex = 0;
	HI_U32 *ptr = (HI_U32 *)pBuffer;

	if (slNumRead > HISFC300_REG_BUF_SIZE)
	{
/*
                    �������ˡ�z00193780
                    ������ˡ�c00233580
                    �������������������κϷ��Լ�飬 �豣����
                    ������Ӱ�졿��Ӱ������ҵ�����θ澯
*/        
		slNumRead = HISFC300_REG_BUF_SIZE;
	}

	/* ��ȡID */
	hisfc_write(pstHost, SFC_REG_CMD_INS, SPI_CMD_RDID);
	hisfc_write(pstHost, SFC_REG_CMD_CONFIG,
			SFC_CMD_CONFIG_SEL_CS(slChipSelect)
			| SFC_CMD_CONFIG_RW
			| SFC_CMD_CONFIG_DATA_EN 
			| SFC_CMD_CONFIG_DATA_CNT(slNumRead)/*�˴��������64 �ֽ�*/
			| SFC_CMD_CONFIG_START);

	while ((hisfc_read(pstHost, SFC_REG_CMD_CONFIG) & SFC_CMD_CONFIG_START))
		;
	slNumRead = (hi_s32)((HI_U32)(slNumRead + 0x03) & (~0x03));/*��д���ݳ���N+1Bytes*/

	/*ִ��N ��*/
	while (slNumRead)
	{
		*ptr = hisfc_read(pstHost, SFC_REG_CMD_DATABUF1 + ulRegIndex);/*���64�ֽڣ�15buffer*/
		ptr += 1;
		ulRegIndex += 4;/* 4�ֽ�long */
		slNumRead -= 4;
	}
    
	return pBuffer;
}

/*******************************************************************************
FUNCTION:		Spif_Write_Enable
DESCRIPTION:	
INPUT:
OUTPUT:	
********************************************************************************/
HI_VOID spif_write_enable(SFC_HOST_S *pstHost, SPI_FLASH_INFO_S *pstInfo)
{	
	HI_U32 ulRegVal = 0;
    HI_U32 ulChipSelectTemp = 0;
    
    HI_REG_WRITE32(pstHost->ulBaseAddr + SFC_REG_CMD_INS, SPI_CMD_WREN);
    ulChipSelectTemp = (HI_U32)pstInfo->slChipSelect;
	ulRegVal = (HI_U32)(SFC_CMD_CONFIG_SEL_CS(ulChipSelectTemp) | SFC_CMD_CONFIG_START);
	hisfc_write(pstHost, SFC_REG_CMD_CONFIG, ulRegVal);
	
    while(HI_REG_READ_VAL32(pstHost->ulBaseAddr +SFC_REG_CMD_CONFIG) & SFC_CMD_CONFIG_START)
		;
}

/*********************************************************
* spif_reg_erase_one_block
* ����:ɾ��һ��block����sector�������С����cmd��ֵ��
*
********************************************************/
HI_PRV HI_U32 spif_reg_erase_one_block(SFC_HOST_S *pstHost, SPI_FLASH_INFO_S *pstInfo,
		HI_U32 ulOffset, HI_S32 slCmdIndex, HI_BOOL bIsCrash)
{
    HI_U32 ulChipSelectTemp = 0;//Ϊ���޸�codex�������Ӿֲ�����
    
	if (spif_wait_ready(pstHost, pstInfo, bIsCrash))
	{
		return HI_ERR_TIMEOUT;
	}
	spif_write_enable(pstHost, pstInfo);

	//spif_config(host, spi, &spi->erase[cmdindex], 1, 0, 0);//FIXME
	hisfc_write(pstHost, SFC_REG_CMD_INS, pstInfo->stErase[slCmdIndex].ucCmd);
	hisfc_write(pstHost, SFC_REG_CMD_ADDR, (ulOffset & SFC_CMD_ADDR_MASK));
    ulChipSelectTemp = (HI_U32)pstInfo->slChipSelect;
	hisfc_write(pstHost, SFC_REG_CMD_CONFIG,
			((HI_U32)SFC_CMD_CONFIG_SEL_CS(ulChipSelectTemp)) 
			| SFC_CMD_CONFIG_MEM_IF_TYPE(pstInfo->stErase[slCmdIndex].ucIfType)
			| SFC_CMD_CONFIG_DUMMY_BYTE_CNT(pstInfo->stErase[slCmdIndex].ucDummy)
			| SFC_CMD_CONFIG_ADDR_EN
			| SFC_CMD_CONFIG_START);

	while ((hisfc_read(pstHost, SFC_REG_CMD_CONFIG) & SFC_CMD_CONFIG_START))//erase�ӿ��˳���flashоƬ���ܻ��ڲ�������
        ;

	return HI_ERR_SUCCESS;
}

/******************************************************
* spif_dma_write
* ����:
* to: д����ַ
* len: ����
*
********************************************************/
HI_U32 spif_dma_write(SPI_FLASH_CTRL_S *pstSpiFlashCtrl, HI_U32 ulTo, HI_U32 ulLen, HI_CONST HI_VOID *pBuf, HI_BOOL bIscrash)
{
	HI_U32 ulNum = 0;
	HI_U8 *pBuffer = (HI_U8 *)pBuf;
	SFC_HOST_S *pstHost = SPIFLASH_TO_HOST(pstSpiFlashCtrl);
	SPI_FLASH_INFO_S *pstInfo = pstHost->stSpiFlashInfo;
	HI_U8 ucCs = 0; 

	if (((ulTo + ulLen) > pstSpiFlashCtrl->ulSize) || (!ulLen))
	{
		DBG_MSG("write data out of range.\r\n");
		DBG_MSG("to:%xh;len:%xh;spi_size:%x;",ulTo, ulLen, pstSpiFlashCtrl->ulSize);
		return HI_ERR_INVALID_PARAMETER;
	}

	if (spif_wait_ready(pstHost, pstInfo, bIscrash))
		goto fail;

	spif_config(pstHost, pstInfo, pstInfo->stWrite, 1, pstInfo->stWrite->ucCmd, pstInfo->stRead->ucCmd);
	while (ulLen > 0)
	{
		ulNum = ((ulLen >= HISFC300_DMA_MAX_SIZE) ? HISFC300_DMA_MAX_SIZE : ulLen);
		while (ulTo >= pstInfo->ulChipSize)
		{
			ulTo -= pstInfo->ulChipSize;
			pstInfo++;
			ucCs++;
			if (!pstInfo->pName)
			{
				DBG_BUG("write memory out of range.\n");
			}
			if (spif_wait_ready(pstHost, pstInfo, bIscrash))
				goto fail;
			spif_config(pstHost, pstInfo, pstInfo->stWrite, 1, pstInfo->stWrite->ucCmd, 0);
		}

        (hi_void)memcpy_s(pstHost->pBuffer, ulNum, pBuffer, ulNum);
		spif_dma_transfer(ulTo, 0, ulNum, ucCs, pstHost);
		
		ulTo += ulNum;
		pBuffer += ulNum;
		ulLen -= ulNum;
	}
	
	return HI_ERR_SUCCESS;
	
fail:	
	return HI_ERR_FLASH_WRITE_WRONG;
}

#ifdef PRODUCT_CFG_FLASH_ERASE_COUNT 
/* �����Ϸ����ɵ����߱�֤�����ñ�����ǰ��֤lenth��PRODUCT_CFG_FLASH_BLOCK_SIZE���� */
HI_VOID erase_cnt_stat(HI_U32 ulOffset, HI_U32 ulLenth)
{
    HI_U32 ulSector = ulOffset / PRODUCT_CFG_FLASH_BLOCK_SIZE;
    HI_U32 ulNum = ulLenth / PRODUCT_CFG_FLASH_BLOCK_SIZE;
    HI_U32 i = 0;

    for (i = 0; i < ulNum; i++)
    {
        if((ulSector + i) < FLASH_TOTAL_SECTOR_NUM)
        {
            g_aulFlashEraseCount[ulSector + i]++;       
        }
    }
}   
#endif
/***********************************************
* spif_reg_erase
*
* pstSpiFlash:
* offset:  ��Ӧ��ʼ��ַ
* length: ���ȣ����Գ���erase.size,������������������
*
*********************************************/
HI_U32 spif_reg_erase(SPI_FLASH_CTRL_S *pstSpiFlashCtrl, HI_U32 ulOffset, HI_U32 ulLength, HI_BOOL bIscrash)
{
	SFC_HOST_S *pstHost = SPIFLASH_TO_HOST(pstSpiFlashCtrl);
	SPI_FLASH_INFO_S *pstInfo = pstHost->stSpiFlashInfo;
	HI_S32 slCmdIndex = 0;
    HI_U32 ulRet = HI_ERR_FAILURE;
    
	/* �ж��Ƿ����chipsize*/
	if (ulOffset + ulLength > pstSpiFlashCtrl->ulSize)
	{
		DBG_MSG("erase area out of range of mtd.\n");
		return HI_ERR_INVALID_PARAMETER;
	}

	if (ulOffset & (pstHost->ulEraseSize - 1))
	{   
		DBG_MSG("offset:%xh;ulEraseSize:%xh", ulOffset, (pstHost->ulEraseSize - 1));
		DBG_MSG("erase start address is not alignment.\r\n");
		return HI_ERR_INVALID_PARAMETER;
	}

	if (ulLength & (pstHost->ulEraseSize - 1))
	{
		DBG_MSG("erase length is not alignment.\r\n");
		return HI_ERR_INVALID_PARAMETER;
	}
    
#ifdef PRODUCT_CFG_FLASH_ERASE_COUNT 
    erase_cnt_stat(ulOffset, ulLength);
#endif
	while (ulLength)
	{
		if (pstInfo->ulChipSize <= ulOffset)
		{
			ulOffset -= pstInfo->ulChipSize;
			pstInfo++;
			if (!pstInfo->pName)
			{
				DBG_BUG("erase memory out of range.\r\n");
			}
		}

		for (slCmdIndex = 0; slCmdIndex < MAX_SPI_OP; slCmdIndex++)
		{	/* ����len ���Ⱦ������ĸ���ѡ��erase_size */
			if ((pstInfo->ulChipSize - ulOffset) >= pstInfo->stErase[slCmdIndex].ulSize
					&& (ulLength >= pstInfo->stErase[slCmdIndex].ulSize)
					&& ((ulOffset % (pstInfo->stErase[slCmdIndex].ulSize)) == 0))
			{
				break;
			}
		}

        if (MAX_SPI_OP == slCmdIndex)
        {
            return HI_ERR_FLASH_MAX_SPI_OP;
        }

        ulRet = spif_reg_erase_one_block(pstHost, pstInfo, ulOffset, slCmdIndex, bIscrash);
        if (HI_ERR_SUCCESS != ulRet)
        {
        	return ulRet;
        }

        ulOffset += pstInfo->stErase[slCmdIndex].ulSize;
        ulLength -= pstInfo->stErase[slCmdIndex].ulSize;
	}

	return HI_ERR_SUCCESS;
}
/*****************************************************
* spif_map_chipsize
*
*
**************************************************/
HI_PRV HI_U32 spif_map_chipsize(HI_U64 ullChipSize)
{
	HI_U32 ulShift = 0;
    
	ullChipSize >>= (19 - 3); // 19: 512K; 3: Bytes -> bit

	while (ullChipSize)
	{
		ullChipSize >>= 1;
		ulShift++;
	}
    
	return ulShift;
}
/*******************************************************************************
*FUNCTION:		spif_probeflash
*DESCRIPTION:	
*INPUT:
*OUTPUT:	
* ��spif_probe ����
* //HI_PRV int hisfc300_spi_probe(struct hisfc_host *host)
********************************************************************************/
SPI_INFO_S *g_pstSpiInfo = HI_NULL;
HI_PRV HI_S32 spif_probeflash(SFC_HOST_S *pstHost)
{
	HI_S32 slChipSelect = 0;
	HI_U32 ulTotal = 0;
	HI_U8 aucIds[8] = {0};
	SPI_FLASH_INFO_S *pstInfo = pstHost->stSpiFlashInfo;
	HI_U32 ulRegVal = 0;
	HI_U32 ulRet = HI_ERR_FAILURE;
	
	pstHost->slNumChip = 0;
	
	/* ��������Ƭѡ��Ƭѡ1 ��0*/
	for (slChipSelect = (CONFIG_HI3911_CHIP_NUM - 1); slChipSelect >= 0; slChipSelect--)
	{
		/*����ID 8���ֽ�*/
		spif_read_ids(pstHost, slChipSelect, aucIds, 8);

		if (!(aucIds[0] | aucIds[1] | aucIds[2]) || ((aucIds[0] & aucIds[1] & aucIds[2]) == 0xFF) )
			continue;

		DBG_MSG("Spi(cs%d) ID: 0x%02X 0x%02X 0x%02X 0x%02X 0x%02X 0x%02X\n",
				slChipSelect,
				aucIds[0], aucIds[1], aucIds[2], aucIds[3], aucIds[4], aucIds[5]);
		/*�ȶ�ID����оƬ��info ��Ϣ*/
		g_pstSpiInfo = spi_serach_ids(aucIds);
		if (g_pstSpiInfo)
		{	/*��host ��spi�ṹ�帳ֵ */
			pstInfo->pName = g_pstSpiInfo->pName;
			pstInfo->slChipSelect = slChipSelect;
			pstInfo->ulChipSize   = g_pstSpiInfo->ulChipSize;
			pstInfo->ulEraseSize  = g_pstSpiInfo->ulEraseSize;
			pstInfo->ulAddrCycle  = g_pstSpiInfo->ulAddrCycle;
			/* ���ñ�׼spi ����,��READ_STD��Ӧ��spi read*/
			spi_search_rw(g_pstSpiInfo, pstInfo->stRead, HISFC300_SUPPORT_READ, HISFC300_SUPPORT_MAX_DUMMY, 1);
			spi_search_rw(g_pstSpiInfo, pstInfo->stWrite, HISFC300_SUPPORT_WRITE, HISFC300_SUPPORT_MAX_DUMMY, 0);
			spi_get_erase(g_pstSpiInfo, pstInfo->stErase, &pstInfo->ulEraseSize);
			spif_map_iftype_and_clock(pstInfo);
			DBG_MSG("\r\nNow spi->read->ucIfType:0x%02X, cmd:0x%02X\r\n", pstInfo->stRead->ucIfType, pstInfo->stRead->cmd);
			DBG_MSG("Now spi->write->ucIfType:0x%02X, cmd:0x%02X\r\n", pstInfo->stWrite->ucIfType, pstInfo->stWrite->cmd);

			/* set chip select.	TODO*/
			ulRegVal = hisfc_read(pstHost, SFC_REG_BUS_FLASH_SIZE);
			ulRegVal |= (spif_map_chipsize((HI_U64)(pstInfo->ulChipSize))) << (HI_U32)(slChipSelect * 4);
			hisfc_write(pstHost, SFC_REG_BUS_FLASH_SIZE,ulRegVal);		

			/* set memory map address. */
			hisfc_write(pstHost, (SFC_REG_BUS_BASE_ADDR_CS0 + ((HI_U32)slChipSelect << 2)), (SFC_BUFFER_BASE_ADDRESS + ulTotal));

			DBG_MSG("0x%08X: 0x%08X\r\n", SFC_REG_BUS_FLASH_SIZE, (hisfc_read(pstHost, SFC_REG_BUS_FLASH_SIZE) >> 4));
			DBG_MSG("0x%08X: 0x%08X\r\n", (SFC_REG_BUS_BASE_ADDR_CS0 + (slChipSelect << 2)),
			hisfc_read(pstHost, ((SFC_REG_BUS_BASE_ADDR_CS0 + (slChipSelect << 2))) >> 16));

			pstInfo->pIoBase = (HI_CHAR *)pstHost->pIobase + ulTotal;

			if (pstInfo->ulAddrCycle == 4)
			{
				DBG_MSG("entry 4 bytes modes.\r\n");
				ulRet = spif_4byte_mode_enable(pstHost, pstInfo);
				if (HI_ERR_SUCCESS != ulRet)
				{
					return (HI_S32)ulRet;
				}
			}

			DBG_MSG("%s->%d\r\n",__func__,__LINE__);
			DBG_MSG("Spi(cs%d): ", pstInfo->slChipSelect);
			DBG_MSG("Block:%uB ",  pstInfo->ulEraseSize);
			DBG_MSG("Chip:%uB ",   pstInfo->ulChipSize);
			DBG_MSG("(Name:%s)\r\n", pstInfo->pName);
			pstInfo++;
			pstHost->slNumChip++;
			ulTotal += pstInfo->ulChipSize;
		}
		else
		{
			DBG_MSG("Spi(cs%d): Unknow\r\n", slChipSelect);
		}
		
	}

	return pstHost->slNumChip;
}

SPI_INFO_S * spif_get_spiinfo(HI_VOID)
{
    return g_pstSpiInfo;

}
HI_PRV HI_U32 mcm(HI_U32 m, HI_U32 n)
{
	HI_U32 ulTotal = m * n;
	HI_U32 ulTt = 0;
    
	if (m < n)
	{
		ulTt = m;
        m = n;
        n = ulTt;
	}

	while (n)
	{
		m = (m % n);
		if (m < n)
		{
			ulTt = m;
			m = n;
			n = ulTt;
		}
	}

	return (ulTotal / m);
}
/*******************************************************************************
FUNCTION:		sfc_probe_spi_size
DESCRIPTION:	
INPUT:
OUTPUT:	
********************************************************************************/
HI_PRV HI_VOID sfc_probe_spi_size(SFC_HOST_S *pstHost, SPI_FLASH_CTRL_S *pstSpiFlashCtrl)
{
	HI_S32 i = 1;
	SPI_FLASH_INFO_S *pstInfo = pstHost->stSpiFlashInfo;
	HI_U32 ulTotal     = pstInfo->ulChipSize;
	HI_U32 ulEraseSize = pstInfo->ulEraseSize;

	for (++pstInfo; i < pstHost->slNumChip; i++, pstInfo++)
	{	
		ulEraseSize = mcm(ulEraseSize, pstInfo->ulEraseSize);
		ulTotal += pstInfo->ulChipSize;
	}

	pstSpiFlashCtrl->ulSize = ulTotal;
	pstHost->ulEraseSize = ulEraseSize;
}
/***********************************************************
* spif_probe
* ����:host�ṹ(�����������)
* ����:����ʱ�Ӳ�������������Ӧ��spiflash�ṹ��
*
* ����:�ɹ��򷵻�0��ʧ�ܷ���-1��
***********************************************************/
HI_PRV HI_S32 spif_probe(SFC_HOST_S *pstHost)
{
    HI_U32 ulGlobalConfig = 0;
	SPI_FLASH_CTRL_S *pstSpiFlashCtrl = pstHost->pstSpiFlashCtrl;

    ulGlobalConfig = HI_REG_READ_VAL32(HI_SFC_REG_BASE + SFC_REG_GLOBAL_CONFIG);
    ulGlobalConfig = ulGlobalConfig | SFC_REG_GLOBAL_CONFIG_WP_ENABLE;
    HI_REG_WRITE32(HI_SFC_REG_BASE + SFC_REG_GLOBAL_CONFIG, ulGlobalConfig);

	hisfc_write(pstHost, SFC_REG_TIMING,
			SFC_TIMING_TCSS(0x6)
			| SFC_TIMING_TCSH(0x6)
			| SFC_TIMING_TRPD(0x800)
			| SFC_TIMING_TSHSL(0xf)
			| SFC_TIMING_TSHWL(0));
    
	/* spiԪ�ض�Ӧ����Ӧ��host */
	if (!spif_probeflash(pstHost))
	{
		return (HI_S32)HI_ERR_FAILURE;
	}
    
    /* ��Ӧ��д����spiflash�ṹ�嵽host��spiflash */
	pstSpiFlashCtrl->pErase = spif_reg_erase;
	pstSpiFlashCtrl->pWrite = spif_dma_write;
	pstSpiFlashCtrl->pRead  = spif_dma_read;
	pstSpiFlashCtrl->ulEraseSize = _4K;//Ĭ��ȫ��Ϊ4k
	pstSpiFlashCtrl->ulWriteSize = pstHost->stSpiFlashInfo->stWrite->ulSize;
	pstSpiFlashCtrl->ulChips = (HI_U32)pstHost->stSpiFlashInfo->slChipSelect;
	return (HI_S32)HI_ERR_SUCCESS;
}
#ifdef PRODUCT_CFG_BOARD_TYPE_ASIC
HI_VOID change_sfc_speed(HI_VOID)
{
    HI_U32 ulValue = 0;
    
    ulValue = HI_REG_READ_VAL32(HI_SYSCTRL_REG_BASE + SC_PERI_CLKSEL);
    ulValue &= (~(HI_U32)(0x7 << 8));
    ulValue |= SFC_CLK_DIV_2;
    ulValue |= SFC_CLK_SEL;
    HI_REG_WRITE32(HI_SYSCTRL_REG_BASE + SC_PERI_CLKSEL, ulValue);
}
#endif
/***********************************************************
* spi_flash_probe
* ����:���в�������
* ����:��
* ����:��init��������
* ����:spi_flash�ṹ��
***********************************************************/
SPI_FLASH_CTRL_S *spi_flash_probe(HI_VOID)
{
	static SFC_HOST_S *pstHost = &g_stSfc300Host;
	SPI_FLASH_CTRL_S *pstSpiFlashCtrl = HI_NULL;

	/* ��ʼ��host�ṹ������Ԫ��Ϊ0*/
    (hi_void)memset_s((HI_VOID *)pstHost, sizeof(SFC_HOST_S), 0, sizeof(SFC_HOST_S));

	pstHost->ulBaseAddr = SFC_REG_BASE_ADDRESS;
	pstHost->pIobase = (HI_VOID *)SFC_BUFFER_BASE_ADDRESS;

	pstHost->pBuffer = (HI_CHAR*)(((HI_U32)s_ascCrashDmaBuf + CACHE_LINE_SIZE) & (~(HI_U32)(CACHE_LINE_SIZE - 1)));
	pstHost->ulDmabuffer = (HI_U32)(pstHost->pBuffer);	  

	pstSpiFlashCtrl = pstHost->pstSpiFlashCtrl;
	pstSpiFlashCtrl->pName = "sfc";

	if(HI_ERR_SUCCESS != spif_probe_ready())//crash ̽��
	{
		return HI_NULL;
	}

	/* ����spifʱ�Ӳ������в�����Ӧ��spiflash��host�ṹ��*/
	if (spif_probe(pstHost))
	{
		DBG_MSG("Can't find a valid spi flash chip.\n");
		pstHost->pBuffer= HI_NULL;
		pstHost->ulDmabuffer = 0x0;
		
		return HI_NULL;
	}
	
	sfc_probe_spi_size(pstHost, pstSpiFlashCtrl);
	return pstSpiFlashCtrl;
}

HI_CHAR *spif_get_name(HI_VOID)
{
    return g_stSfc300Host.stSpiFlashInfo[0].pName;
}
/*****************************************************************************
 �� �� ��  : spif_read_status_reg
 ��������  : ��ȡflash״̬�Ĵ���ֵ��
 �������  : cmd ��״̬�Ĵ���������
 �������  : ״̬�Ĵ���ֵ
*****************************************************************************/
HI_U32 spif_read_status_reg(SFC_HOST_S *pstHost, SPI_FLASH_INFO_S *pstInfo, HI_U8 cmd)
{   
    HI_U32 ulChipSelectTemp = (HI_U32)pstInfo->slChipSelect;
    
	hisfc_write(pstHost,SFC_REG_CMD_INS, cmd);
	hisfc_write(pstHost,SFC_REG_CMD_CONFIG,
			((HI_U32)SFC_CMD_CONFIG_SEL_CS(ulChipSelectTemp))
			| SFC_CMD_CONFIG_DATA_CNT(1)/*lint !e778*/
			| SFC_CMD_CONFIG_DATA_EN
			| SFC_CMD_CONFIG_RW
			| SFC_CMD_CONFIG_START);
			
	while ((hisfc_read(pstHost,SFC_REG_CMD_CONFIG) & SFC_CMD_CONFIG_START))
		;
	return hisfc_read(pstHost,SFC_REG_CMD_DATABUF1);
}

/*****************************************************************************
 �� �� ��  : spif_write_status_reg
 ��������  : дflash״̬�Ĵ�����
 �������  : cmd д״̬�Ĵ���������;
             status_val ��д���ֵ;
             data_len   д��ֵ���ֽ���
 �������  : ��
*****************************************************************************/
HI_VOID spif_write_status_reg(SFC_HOST_S *pstHost, SPI_FLASH_INFO_S *pstInfo, 
                                        HI_U8 cmd,unsigned int status_val,HI_U8 data_len)
{
    HI_U32 ulChipSelectTemp = (HI_U32)pstInfo->slChipSelect;
    
    hisfc_write(pstHost, SFC_REG_CMD_INS, cmd);
    hisfc_write(pstHost, SFC_REG_CMD_DATABUF1, status_val);
    hisfc_write(pstHost, SFC_REG_CMD_CONFIG,
                ((HI_U32)SFC_CMD_CONFIG_SEL_CS(ulChipSelectTemp))
                | SFC_CMD_CONFIG_DATA_EN
                | SFC_CMD_CONFIG_DATA_CNT(data_len)
                | SFC_CMD_CONFIG_START);

	while ((hisfc_read(pstHost,SFC_REG_CMD_CONFIG) & SFC_CMD_CONFIG_START))
		;    
}
/*****************************************************************************
 �� �� ��  : spif_wp_protect_ctrl
 ��������  : ����WP�ܽţ�ʹ��WP����
 �������  : enable:HI_TUREʹ�ܣ�HI_FALSEȥʹ��  
 �������  : ��
*****************************************************************************/
HI_VOID spif_wp_protect_ctrl(SFC_HOST_S *pstHost,HI_BOOL enable)
{
    HI_U32 ulconfig =0;
    
    ulconfig = hisfc_read(pstHost, SFC_REG_GLOBAL_CONFIG);
    if(enable)
    {
        ulconfig = ulconfig|SFC_REG_GLOBAL_CONFIG_WP_ENABLE;
    }
    else
    {  
        ulconfig = ulconfig&(~SFC_REG_GLOBAL_CONFIG_WP_ENABLE);
    }
    
    hisfc_write(pstHost, SFC_REG_GLOBAL_CONFIG, ulconfig);
}




