/******************************************************************************
 *    Copyright (c) 2009-2010 by HiC
 *    All rights reserved.
 * ***
 *    Create by HiC 2010-12-15
 *
 ******************************************************************************/

/*****************************************************************************/
//#include <boot/list.h>
#include <boot/flash.h>
#include <boot/boot.h>
//#include <convert.h>

#include "boot/platform.h"

//#include "../spi.h"
#include "spi_flash.h"
#include <debug.h>
#include "spi_ids.h"
#include "hisfc350.h"
#include "platform.h"//包含PRODUCT_CFG_FLASH_PROTECT定义
#include "securec.h"

#ifdef PRODUCT_CFG_FLASH_PROTECT
#include "spi_flash_protect_drv.h"
#endif


/*****************************************************************************/
#define EIO 1//5
#define EINVAL 1//22

/*lint -e683*/
#define malloc  alloc
#define free 
/*lint  +e683*/

//#pragma weak xfree = __libc_free

#define HISFC350_SUPPORT_READ \
	SPI_IF_READ_STD

#define HISFC350_SUPPORT_MAX_DUMMY        (7)

#define HISFC350_SUPPORT_WRITE \
	SPI_IF_WRITE_STD

struct hisfc_host hisfc_host;

/*****************************************************************************/
int hisfc350_wait_ready(struct hisfc_host *host, struct hisfc_spi *spi)
{
	unsigned long regval;
	unsigned long deadline = 0;

	do {
		hisfc_write(host, HISFC350_CMD_INS, SPI_CMD_RDSR);
        /*lint -e778*/
		hisfc_write(host, HISFC350_CMD_CONFIG,
			HISFC350_CMD_CONFIG_SEL_CS(spi->chipselect)
			| HISFC350_CMD_CONFIG_DATA_CNT(1)
			| HISFC350_CMD_CONFIG_DATA_EN
			| HISFC350_CMD_CONFIG_RW_READ
			| HISFC350_CMD_CONFIG_START);
        /*lint +e778*/
		while ((hisfc_read(host, HISFC350_CMD_CONFIG) 
			& HISFC350_CMD_CONFIG_START))
			;
		regval = hisfc_read(host, HISFC350_CMD_DATABUF0);
		if (!(regval & SPI_CMD_SR_WIP))
			return 0;
		udelay(1);
	} while (deadline++ < (40 << 20));
	return 1;
}
/*****************************************************************************/

static void hisfc350_4byte_mode_enable(struct hisfc_host *host, struct hisfc_spi *spi)
{
	unsigned int regval;

	hisfc_write(host, HISFC350_CMD_INS, SPI_CMD_EN4B);
	hisfc_write(host, HISFC350_CMD_CONFIG,
		HISFC350_CMD_CONFIG_SEL_CS(spi->chipselect)
		| HISFC350_CMD_CONFIG_START);

	while ((hisfc_read(host, HISFC350_CMD_CONFIG) 
		& HISFC350_CMD_CONFIG_START))
		;

	regval = hisfc_read(host, HISFC350_GLOBAL_CONFIG);
	regval |= HISFC350_GLOBAL_CONFIG_ADDR_MODE_4B;
	hisfc_write(host, HISFC350_GLOBAL_CONFIG, regval);
}

/*****************************************************************************/
static void hisfc350_bus_config(struct hisfc_host *host, struct hisfc_spi *spi,
	int en_buswrite)
{
	unsigned int regval = HISFC350_BUS_CONFIG1_READ_EN;

	if (en_buswrite)
/*
                    【屏蔽人】z00193780
                    【审核人】b00208046
                    【代码分析】驱动接口，该分支需保留，需保留。
                    【功能影响】不影响正常业务，屏蔽告警
*/        
		regval |= HISFC350_BUS_CONFIG1_WRITE_EN;

	regval |= HISFC350_BUS_CONFIG1_WRITE_INS(spi->write->cmd);
	regval |= HISFC350_BUS_CONFIG1_WRITE_DUMMY_CNT(spi->write->dummy);
	regval |= HISFC350_BUS_CONFIG1_WRITE_IF_TYPE(spi->write->iftype);

	regval |= HISFC350_BUS_CONFIG1_READ_PREF_CNT(0);
	regval |= HISFC350_BUS_CONFIG1_READ_INS(spi->read->cmd);
	regval |= HISFC350_BUS_CONFIG1_READ_DUMMY_CNT(spi->read->dummy);
	regval |= HISFC350_BUS_CONFIG1_READ_IF_TYPE(spi->read->iftype);

	hisfc_write(host, HISFC350_BUS_CONFIG1, regval);
}
/*****************************************************************************/
static void hisfc350_map_iftype_and_clock(struct hisfc_spi *spi)
{
	int ix;
	int iftype_read[] = {
		SPI_IF_READ_STD,       0,
		SPI_IF_READ_FAST,      0,
		SPI_IF_READ_DUAL,      1,
		SPI_IF_READ_DUAL_ADDR, 2,
		SPI_IF_READ_QUAD,      5,
		SPI_IF_READ_QUAD_ADDR, 6,
		0,0,
	};
	int iftype_write[] = {
		SPI_IF_WRITE_STD,       0,
		SPI_IF_WRITE_DUAL,      1,
		SPI_IF_WRITE_DUAL_ADDR, 2,
		SPI_IF_WRITE_QUAD,      5,
		SPI_IF_WRITE_QUAD_ADDR, 6,
		0,0,
	};

	for (ix = 0; iftype_write[ix]; ix += 2) {
		if (spi->write->iftype == iftype_write[ix]) {
			spi->write->iftype = (unsigned char)iftype_write[ix + 1];
			break;
		}
	}

	for (ix = 0; iftype_read[ix]; ix += 2) {
		if (spi->read->iftype == iftype_read[ix]) {
			spi->read->iftype = (unsigned char)iftype_read[ix + 1];
			break;
		}
	}

//	hisfc350_get_best_clock(&spi->read->clock);
//	hisfc350_get_best_clock(&spi->write->clock);

	DBG_MSG("map: spi->read->iftype:0x%X\n",  spi->read->iftype);
	//DBG_MSG("map: spi->read->clock:0x%X\n",   spi->read->clock);
	DBG_MSG("map: spi->write->iftype:0x%X\n", spi->write->iftype);
	//DBG_MSG("map: spi->write->clock:0x%X\n",  spi->write->clock);

	for (ix = 0; (ix < MAX_SPI_OP) && spi->erase[ix].iftype; ix++)
	{
//		hisfc350_get_best_clock(&(spi->erase[ix].clock));
		spi->erase[ix].iftype = 0;
		DBG_MSG("map: spi->erase[%d].clock:0x%X size:%u\n",
			ix, spi->erase[ix].clock, spi->erase[ix].size);
        //ultohstr(spi->erase[ix].size));
	}
}
/*****************************************************************************/
static void hisfc350_dma_transfer(unsigned int spi_start_addr, 
	unsigned char is_read, unsigned int size, 
	unsigned char chipselect, struct hisfc_host *host)
{

	hisfc_write(host, HISFC350_BUS_DMA_MEM_SADDR,
		host->dma_buffer);

	hisfc_write(host, HISFC350_BUS_DMA_FLASH_SADDR,
		spi_start_addr);

	hisfc_write(host, HISFC350_BUS_DMA_LEN, 
		HISFC350_BUS_DMA_LEN_DATA_CNT(size));

	hisfc_write(host, HISFC350_BUS_DMA_AHB_CTRL,
		HISFC350_BUS_DMA_AHB_CTRL_INCR4_EN
		| HISFC350_BUS_DMA_AHB_CTRL_INCR8_EN
		| HISFC350_BUS_DMA_AHB_CTRL_INCR16_EN);

	hisfc_write(host, HISFC350_BUS_DMA_CTRL, 
		HISFC350_BUS_DMA_CTRL_RW(is_read)
		| HISFC350_BUS_DMA_CTRL_CS(chipselect)
		| HISFC350_BUS_DMA_CTRL_START);

	while (!(hisfc_read(host, HISFC350_INT_RAW_STATUS) 
		& (HISFC350_INT_RAW_STATUS_DMA_DONE)))
		;
	hisfc_write(host, HISFC350_INT_CLEAR, HISFC350_INT_CLEAR_DMA_DONE);
}
/*****************************************************************************/
static int hisfc350_dma_read(struct spi_flash *spiflash, u32 from, 
	size_t len, void *buf)
{
	int num;
	int result = -EIO;
	unsigned char *ptr = (unsigned char *)buf;
	struct hisfc_host *host = SPIFLASH_TO_HOST(spiflash);
	struct hisfc_spi *spi = host->spi;

	if ((from + len) > spiflash->size) {
		return -EINVAL;
	}

	if (!len) {
		return 0;
	}

	if (hisfc350_wait_ready(host, spi))
		goto fail;
	hisfc350_bus_config(host, spi, FALSE);
    
//	hisfc350_set_system_clock(host, spi->read, TRUE);

	while (len > 0) {
		while (from >= spi->chipsize) {
			from -= spi->chipsize;
			spi++;
			if (hisfc350_wait_ready(host, spi))
				goto fail;
			hisfc350_bus_config(host, spi, FALSE);
//			hisfc350_set_system_clock(host, spi->read, TRUE);
		}

		num = (int)(((from + len) >= spi->chipsize)
			? (spi->chipsize - from) : len);
		while (num >= HISFC350_DMA_MAX_SIZE) {
			hisfc350_dma_transfer(from, READ, HISFC350_DMA_MAX_SIZE, (unsigned char)spi->chipselect, host);
/*
                            【屏蔽人】z00193780
                            【审核人】w00190206
                            【代码分析】boot下memcpy、memset、strcpy替换为安全接口会增加2k bytes左右的代码,并且以上接口性能分别会降低一定比例，所以建议boot下不做安全接口替换。
                            【功能影响】不影响正常业务，屏蔽告警
*/
			memcpy_s(ptr, HISFC350_DMA_MAX_SIZE, host->buffer, HISFC350_DMA_MAX_SIZE);
			ptr  += HISFC350_DMA_MAX_SIZE;
			from += HISFC350_DMA_MAX_SIZE;
			len  -= HISFC350_DMA_MAX_SIZE;
			num  -= HISFC350_DMA_MAX_SIZE;
		}

		if (num) {
 			hisfc350_dma_transfer(from, READ, (unsigned int)num, (unsigned char)spi->chipselect, host);
/*
                            【屏蔽人】z00193780
                            【审核人】w00190206
                            【代码分析】boot下memcpy、memset、strcpy替换为安全接口会增加2k bytes左右的代码,并且以上接口性能分别会降低一定比例，所以建议boot下不做安全接口替换。
                            【功能影响】不影响正常业务，屏蔽告警
*/
			memcpy_s(ptr, (unsigned int)num, host->buffer, (unsigned int)num);
			from += (u32)num;
			ptr = (unsigned char *)((uintptr_t)ptr + (unsigned int)num);// pclint
			len  -= (size_t)num;
		}
	}
	result = 0;
fail:
	
	return result;
}
/*****************************************************************************/
static unsigned char *hisfc350_read_ids(struct hisfc_host *host, 
	int chipselect, unsigned char *buffer, int numread)
{
	int regindex = 0;
	unsigned long *ptr = (unsigned long *)buffer;

	if (numread > HISFC350_REG_BUF_SIZE)
/*
                    【屏蔽人】z00193780
                    【审核人】c00233580
                    【代码分析】函数的入参合法性检查， 需保留。
                    【功能影响】不影响正常业务，屏蔽告警
*/        
		numread = HISFC350_REG_BUF_SIZE;

	hisfc_write(host, HISFC350_CMD_INS, SPI_CMD_RDID);
	hisfc_write(host, HISFC350_CMD_CONFIG,
		HISFC350_CMD_CONFIG_SEL_CS((unsigned int)chipselect)
		| HISFC350_CMD_CONFIG_RW_READ
		| HISFC350_CMD_CONFIG_DATA_EN 
		| HISFC350_CMD_CONFIG_DATA_CNT((unsigned int)numread)
		| HISFC350_CMD_CONFIG_START);

	while ((hisfc_read(host, HISFC350_CMD_CONFIG)
		& HISFC350_CMD_CONFIG_START))
		;

	numread = (int)((uint32_t)(numread + 0x03) & (~0x03));
	while (numread) {
		*ptr = hisfc_read(host, HISFC350_CMD_DATABUF0 + regindex);
		ptr      += 1;
		regindex += 4;
		numread  -= 4;
	}

	return buffer;
}
/*****************************************************************************/
int hisfc350_write_enable(struct hisfc_host *host, struct hisfc_spi *spi)
{
	unsigned int regval = 0;

	hisfc_write(host, HISFC350_CMD_INS, SPI_CMD_WREN);

	regval = HISFC350_CMD_CONFIG_SEL_CS(spi->chipselect)
		| HISFC350_CMD_CONFIG_START;
	hisfc_write(host, HISFC350_CMD_CONFIG, regval);

	while ((hisfc_read(host, HISFC350_CMD_CONFIG) 
		& HISFC350_CMD_CONFIG_START))
		;

	return 0;
}
/*****************************************************************************/
static int hisfc350_reg_erase_one_block(struct hisfc_host *host,
	struct hisfc_spi *spi, unsigned long offset, int cmdindex)
{
	unsigned int addr_en = HISFC350_CMD_CONFIG_ADDR_EN;

#ifdef TEST_BOOT_CLOSE_ELEC_C
	{	
		int ret = 0;
		static int time=0;

		if ((offset>=0xde000)&&(offset<0xef000))
		{
			time++;
			FLASH_Read(0x1ff000, &ret , 1);
			if ((ret == time))
			{
				FLASH_Erase(0x1ff000, 0x1000);
				mdelay(100);
				Watchdog_Enable(1);
				while(1);
			}
		}
	}
#endif
	
	if (hisfc350_wait_ready(host, spi))
		return 1;

	hisfc350_write_enable(host, spi);

//	hisfc350_set_system_clock(host, &spi->erase[cmdindex], TRUE);

	hisfc_write(host, HISFC350_CMD_INS, spi->erase[cmdindex].cmd);

	if (spi->erase[cmdindex].cmd == SPI_CMD_BE)
	{
		addr_en = 0;
	}
	
	hisfc_write(host, HISFC350_CMD_ADDR,
		(offset & HISFC350_CMD_ADDR_MASK));

	hisfc_write(host, HISFC350_CMD_CONFIG,
		HISFC350_CMD_CONFIG_SEL_CS(spi->chipselect) 
		| HISFC350_CMD_CONFIG_MEM_IF_TYPE(spi->erase[cmdindex].iftype)
		| HISFC350_CMD_CONFIG_DUMMY_CNT(spi->erase[cmdindex].dummy)
		| addr_en
		| HISFC350_CMD_CONFIG_START);

	while ((hisfc_read(host, HISFC350_CMD_CONFIG)
		& HISFC350_CMD_CONFIG_START))
		;

	return 0;
}
/*****************************************************************************/
static int hisfc350_dma_write(struct spi_flash *spiflash, u32 to, size_t len,
	const void *buf)
{
	int num;
	int result = -EIO;

	unsigned char *ptr = (unsigned char *)buf;
	struct hisfc_host *host = SPIFLASH_TO_HOST(spiflash);
	struct hisfc_spi *spi = host->spi;

	if ((to + len) > spiflash->size) {
		return -EINVAL;
	}

	if (!len) {
		return 0;
	}

	if (hisfc350_wait_ready(host, spi))
		goto fail;
	hisfc350_bus_config(host, spi, FALSE);

	if (to & HISFC350_DMA_MAX_MASK) {
		num = HISFC350_DMA_MAX_SIZE - (to & HISFC350_DMA_MAX_MASK);
		if ((size_t)num > len)
			num = (int)len;
		while (to >= spi->chipsize) {
			to -= spi->chipsize;
			spi++;
			if (hisfc350_wait_ready(host, spi))
				goto fail;
			hisfc350_bus_config(host, spi, FALSE);
//			hisfc350_set_system_clock(host, spi->write, TRUE);
		}
/*
                    【屏蔽人】z00193780
                    【审核人】w00190206
                    【代码分析】boot下memcpy、memset、strcpy替换为安全接口会增加2k bytes左右的代码,并且以上接口性能分别会降低一定比例，所以建议boot下不做安全接口替换。
                    【功能影响】不影响正常业务，屏蔽告警
*/
		memcpy_s(host->buffer, (unsigned int)num, ptr, (unsigned int)num);
		hisfc350_dma_transfer(to, WRITE, (unsigned int)num, (unsigned char)spi->chipselect, (struct hisfc_host *)host);
		to  += (u32)num;
		ptr = (unsigned char *)((uintptr_t)ptr + (unsigned int)num);
		len -= (size_t)num;
	}

	while (len > 0) {
		num = (int)((len >= HISFC350_DMA_MAX_SIZE)
			? HISFC350_DMA_MAX_SIZE : len);
		while (to >= spi->chipsize) {
			to -= spi->chipsize;
			spi++;
			if (hisfc350_wait_ready(host, spi))
				goto fail;
			hisfc350_bus_config(host, spi, FALSE);
//			hisfc350_set_system_clock(host, spi->write, TRUE);
		}
/*
                    【屏蔽人】z00193780
                    【审核人】w00190206
                    【代码分析】boot下memcpy、memset、strcpy替换为安全接口会增加2k bytes左右的代码,并且以上接口性能分别会降低一定比例，所以建议boot下不做安全接口替换。
                    【功能影响】不影响正常业务，屏蔽告警
*/
		memcpy_s(host->buffer, (unsigned int)num, ptr, (unsigned int)num);
		hisfc350_dma_transfer(to, WRITE, (unsigned int)num, (unsigned char)spi->chipselect, host);

		to  += (u32)num;
		ptr = (unsigned char *)((uintptr_t)ptr + (unsigned int)num);
		len -= (size_t)num;
	}

	result = 0;
fail:
	return result;
}
/*****************************************************************************/
static int hisfc350_reg_erase(struct spi_flash *spiflash, u32 offset, size_t length)
{
	int cmdindex;
	struct hisfc_host *host = SPIFLASH_TO_HOST(spiflash);
	struct hisfc_spi *spi = host->spi;

	if (offset + length > spiflash->size) {
		return -EINVAL;
	}

	if (offset & (host->erasesize-1)) {
		return -EINVAL;
	}

	if (length & (host->erasesize-1)) {
		return -EINVAL;
	}

	while (length) {
		if (spi->chipsize <= offset) {
			offset -= spi->chipsize;
			spi++;
		}

		for (cmdindex = 0; cmdindex < MAX_SPI_OP; cmdindex++) {
			if (((spi->chipsize - offset) >= spi->erase[cmdindex].size)
				&& (length >= spi->erase[cmdindex].size)
				&& ((offset%(spi->erase[cmdindex].size)) == 0))
				break;
		}

		if (MAX_SPI_OP == cmdindex)
		{			 
			return -EINVAL;
		}
		
		if (hisfc350_reg_erase_one_block(host, spi,
			(unsigned long)offset, cmdindex))
			return -1;

		offset += spi->erase[cmdindex].size;
		length -= spi->erase[cmdindex].size;
	}

	return 0;
}
/*****************************************************************************/
static int hisfc350_map_chipsize(unsigned long chipsize)
{
	int shift = 0;
	chipsize >>= (19 - 3); /* 19: 512K; 3: Bytes -> bit */

	while (chipsize) {
		chipsize >>= 1;
		shift++;
	}
	return shift;
}
/*****************************************************************************/
static int hisfc350_spi_probe(struct hisfc_host *host)
{
	unsigned int regval=0;
	unsigned int total = 0;
	unsigned char ids[8];
	struct spi_info *spiinfo;
	struct hisfc_spi *spi = host->spi;
	int chipselect = CONFIG_HISFC350_CHIP_NUM - 1;
	//unsigned char i;

	host->num_chip = 0;

	for (; chipselect >= 0; chipselect--) {
		hisfc350_read_ids(host, chipselect, ids, 8);

		/* can't find spi flash device. */
		if (!(ids[0] | ids[1] | ids[2]) 
			|| ((ids[0] & ids[1] & ids[2]) == 0xFF))
			continue;

		//print_msg0("Flash Id:");
		//for (i=0; i<=5; i++)
		//{
		//	print_hex(ids[i], 0);
		//} 

		spiinfo = spi_serach_ids(ids);

		if (spiinfo) {
			spi->name = spiinfo->name;
			spi->chipselect = (unsigned int)chipselect;
			spi->chipsize	= spiinfo->chipsize;
			spi->erasesize	= spiinfo->erasesize;
			spi->addrcycle	= spiinfo->addrcycle;

			spi_search_rw(spiinfo, spi->read, 
				HISFC350_SUPPORT_READ,
				HISFC350_SUPPORT_MAX_DUMMY, READ);

			spi_search_rw(spiinfo, spi->write, 
				HISFC350_SUPPORT_WRITE,
				HISFC350_SUPPORT_MAX_DUMMY, WRITE);

			spi_get_erase(spiinfo, spi->erase, &spi->erasesize);
			hisfc350_map_iftype_and_clock(spi);

			regval = hisfc_read(host, HISFC350_BUS_FLASH_SIZE);
			regval &= (unsigned int)(~((unsigned int)(HISFC350_BUS_FLASH_SIZE_CS0_MASK << ((unsigned int)chipselect << 3))));
			regval |= (((unsigned int)hisfc350_map_chipsize(spi->chipsize)) << ((unsigned int)chipselect << 3));
			hisfc_write(host, HISFC350_BUS_FLASH_SIZE, regval);

			hisfc_write(host, 
				(HISFC350_BUS_BASE_ADDR_CS0 + ((unsigned int)chipselect << 2)),
				(CONFIG_HISFC350_BUFFER_BASE_ADDRESS + total));

			spi->iobase = (char *)host->iobase + total;

			if (spi->addrcycle == 4) {
				hisfc350_4byte_mode_enable(host, spi);
			}

			//print_msg0(spi->name);

			host->num_chip++;
			total += spi->chipsize;
			spi++;
#ifdef PRODUCT_CFG_FLASH_PROTECT
			hisfc350_set_protect_magic(spiinfo);
#endif
		} else
			print_msg0("Unknow Flash.");
	}

	return host->num_chip;
}
/*****************************************************************************/
static int mcm(int m, int n)
{
	unsigned int total = (unsigned int)(m * n);
	unsigned int tt;
	if (m < n) {
		tt = (unsigned int)m;
		m = n;
		n = (int)tt;
	}

	while (n) {
		m = (m % n);
		if (m < n) {
			tt = (unsigned int)m;
			m = n;
			n = (int)tt;
		}
	}

	return (int)(total / (unsigned int)m);
}
/*****************************************************************************/
static void hisfc_probe_spi_size(struct hisfc_host *host,
	struct spi_flash *spiflash)
{
	int ix = 1;
	struct hisfc_spi *spi = host->spi;

	int total     = (int)spi->chipsize;
	int erasesize = (int)spi->erasesize;

	for (++spi; ix < host->num_chip; ix++, spi++) {
		erasesize = mcm(erasesize, (int)spi->erasesize);
		total += (int)spi->chipsize;
	}

	spiflash->size = (unsigned int)total;
	host->erasesize = (unsigned int)erasesize;
}
/*****************************************************************************/
static int hisfc350_probe(struct hisfc_host *host)
{
	struct spi_flash *spiflash = host->spiflash;

	hisfc_write(host, HISFC350_TIMING,
		HISFC350_TIMING_TCSS(0x6)
		| HISFC350_TIMING_TCSH(0x6)
		| HISFC350_TIMING_TSHSL(0xf));

	if (!hisfc350_spi_probe(host))
		return -1;

	spiflash->erase = hisfc350_reg_erase;
	spiflash->write = hisfc350_dma_write;
	spiflash->read  = hisfc350_dma_read;

	return 0;
}
void change_sfc_speed(void)
{
    unsigned int value = 0;
  
    value = readl(REG_BASE_SCTL + SC_PERI_CLKSEL);
    value &= (~(u32)(0x7 << 8));
    value |= SFC_CLK_DIV_2;
    value |= SFC_CLK_SEL;
    writel(value, REG_BASE_SCTL + SC_PERI_CLKSEL); 
}
/*****************************************************************************/
struct spi_flash *spi_flash_probe(unsigned int bus, unsigned int cs,
	unsigned int max_hz, unsigned int spi_mode)
{
	static struct hisfc_host *host = &hisfc_host;
	static struct spi_flash *spiflash = (struct spi_flash *)HI_NULL;

    HI_UNREF_PARAM(bus);
    HI_UNREF_PARAM(cs);
    HI_UNREF_PARAM(max_hz);
    HI_UNREF_PARAM(spi_mode);

	if (spiflash)
		return spiflash;
    
	memset_s(host, sizeof(struct hisfc_host), 0, sizeof(struct hisfc_host));

	host->regbase = (void *)CONFIG_HISFC350_REG_BASE_ADDRESS;
	host->iobase  = (void *)CONFIG_HISFC350_BUFFER_BASE_ADDRESS;
	host->buffer  = (char *)xmalloc(HISFC350_DMA_MAX_SIZE);
	if (!host->buffer) {
		goto fail;
	}

	host->dma_buffer = host->buffer;

	spiflash = host->spiflash;
	spiflash->name = "hi_sfc";

	if (hisfc350_probe(host)) {
		goto fail;
	}
	hisfc_probe_spi_size(host, spiflash);
success:
	return spiflash;
fail:
	if (host->buffer) {
		xfree(host->buffer);
		host->buffer = (char *)HI_NULL;
	}
	spiflash = (struct spi_flash *)HI_NULL;
	goto success;
}
/*****************************************************************************/
/*****************************************************************************
 函 数 名  : hisfc350_read_status_reg
 功能描述  : 读取flash状态寄存器值；
 输入参数  : cmd 读状态寄存器的命令
 输出参数  : 状态寄存器值
*****************************************************************************/
unsigned int hisfc350_read_status_reg(struct hisfc_host *host, struct hisfc_spi *spi, unsigned char cmd)
{    
	hisfc_write(host,HISFC350_CMD_INS, cmd);
	hisfc_write(host,HISFC350_CMD_CONFIG,
			HISFC350_CMD_CONFIG_SEL_CS(spi->chipselect)
			| HISFC350_CMD_CONFIG_DATA_CNT(1)/*lint !e778*/
			| HISFC350_CMD_CONFIG_DATA_EN
			| HISFC350_CMD_CONFIG_RW_READ
			| HISFC350_CMD_CONFIG_START);

	while ((hisfc_read(host,HISFC350_CMD_CONFIG) & HISFC350_CMD_CONFIG_START))
        ;
	return hisfc_read(host,HISFC350_CMD_DATABUF0);
}

/*****************************************************************************
 函 数 名  : hisfc350_write_status_reg
 功能描述  : 写flash状态寄存器；
 输入参数  : cmd 写状态寄存器的命令;
             status_val 待写入的值;
             data_len   写入值的字节数
 输出参数  : 无
*****************************************************************************/
void hisfc350_write_status_reg(struct hisfc_host *host, struct hisfc_spi *spi, 
                                        unsigned char cmd,unsigned int status_val,unsigned char data_len)
{
    hisfc_write(host, HISFC350_CMD_INS, cmd);
    hisfc_write(host, HISFC350_CMD_DATABUF0, status_val);//先从低地址存放
    hisfc_write(host, HISFC350_CMD_CONFIG,
                HISFC350_CMD_CONFIG_SEL_CS(spi->chipselect)
                | HISFC350_CMD_CONFIG_DATA_EN
                | HISFC350_CMD_CONFIG_DATA_CNT(data_len)
                | HISFC350_CMD_CONFIG_START);

	while ((hisfc_read(host,HISFC350_CMD_CONFIG) & HISFC350_CMD_CONFIG_START))
        ;    
}
/*****************************************************************************
 函 数 名  : hisfc350_wp_protect_ctrl
 功能描述  : 拉低WP管脚，使能WP保护
 输入参数  : enable:1使能；0去使能  
 输出参数  : 无
*****************************************************************************/
void hisfc350_wp_protect_ctrl(struct hisfc_host *host, unsigned char enable)
{
    unsigned int ulconfig =0;
    
    ulconfig = hisfc_read(host, HISFC350_GLOBAL_CONFIG);
    if(enable)
    {
        ulconfig = ulconfig|HISFC350_GLOBAL_CONFIG_WRITE_PROTECT;
    }
    else
    {  
        ulconfig = ulconfig&(~HISFC350_GLOBAL_CONFIG_WRITE_PROTECT);
    }
    hisfc_write(host, HISFC350_GLOBAL_CONFIG, ulconfig);
}


