/******************************************************************************
*    Copyright (c) 2009-2010 by czy.
*    All rights reserved.
* ***
*       Create by Cai ZhiYong. 2010-12-16
*       changed by Wu Xianfeng 2011-09
*       changed at 2011.10.18
*
******************************************************************************/

#ifndef SFC3911H
#define SFC3911H
#include <hi_types.h>
#include "spi_ids.h"
#include "los_typedef.h"
#include "hi_driver.h"

#define DRV_DFX_SYS_VAL_DECLARE
#define SF_INVALID_DRIVER               ((HI_U32)(-2))
#define SF_INVALID_OPERATION            ((HI_U32)(-3))
#define SF_INVALID_SIZE                 ((HI_U32)(-5))
/******************************************************************************/
#define CONFIG_HI3911_CHIP_NUM		    (2)
/*****************************************************************************/
#define HISFC300_REG_BASE_LEN           (0x100)/*没用到*/
#define HISFC300_DMA_MAX_SIZE           (2048)/* 这个值有待确认 */
#define HISFC300_DMA_MAX_MASK           (0x7FF)/* 与上面的值共同确定DMA不超过2048*/
#define HISFC300_BUFFER_BASE_LEN        (0x4000000) /* 64MB *//*没用到*/

#define HISFC300_MAX_READY_WAIT_JIFFIES (40 * HZ)//no
/*****************************************************************************/
#define HISFC300_REG_BUF_SIZE            (256)/* 我的理解应该是64 */
#define HISFC300_REG_BUF_MASK            (0xFF)/* 用于reg_write , 保留*/ 
#define HISFC300_MEM_READ_SLICE          (512)/* bus_read用到, 保留*/

#define HISFC300_BUS_CONFIG2                           0x0204//no
#define HISFC300_BUS_BASE_ADDR_CS1                     0x0218//no
#define HISFC300_BUS_ALIAS_ADDR                        0x021C//no
#define HISFC300_BUS_ALIAS_CS                          0x0220//no
#define HISFC300_CMD_DATABUF64                         0x04FC//no

/*****************************************************************************/
#define IN
#define OUT
#define INOUT
#define SYSTEM_CLOCK_MHZ g_cfg_arm_clock
#define SFC_CLK_DIV_2   (2<<8)
#define SFC_CLK_SEL     (1<<4)
/***************SFC of Hi3911V100********************************************/
#define SFC_REG_BASE_ADDRESS            HI_SFC_REG_BASE
#define SFC_BUFFER_BASE_ADDRESS         HI_SFC_MEM_BASE

#define SFC_REG_GLOBAL_CONFIG			0x0100
#define SFC_REG_GLOBAL_CONFIG_ADDR_MODE_4B        (1 << 2)
#define SFC_REG_TIMING					0x0110
#define SFC_REG_GLOBAL_CONFIG_WP_ENABLE     (1<<1)
#define SFC_TIMING_TSHSL(_n)				((_n) & 0xF)
#define SFC_TIMING_TSHWL(_n)				(((_n) & 0xF) << 4)
#define SFC_TIMING_TCSS(_n)					(((_n) & 0x7) << 8)
#define SFC_TIMING_TCSH(_n)					(((_n) & 0x7) << 12) 
#define SFC_TIMING_TRPD(_n)					(((_n) & 0xFFF) << 16)
#define SFC_REG_INT_RAW_STATUS			0x0120/*中断原始状态寄存器 */
#define SFC_REG_INT_RAW_STATUS_DMA_DONE		(1<<1)
#define SFC_REG_INT_STATUS				0x0124
#define SFC_REG_INT_MASK				0x0128
#define SFC_REG_INT_CLEAR				0x012C
#define SFC_REG_INT_CLEAR_DMA_DONE			(1<<1)
#define SFC_REG_VERSION					0x01F8
#define SFC_REG_VERSION_SEL				0x01FC
#define SFC_REG_BUS_CONFIG1				0x0200
#define SFC_BUS_CONFIG1_WR_INS(_n)			(((unsigned int)_n & 0xFF) << 22)
#define SFC_BUS_CONFIG1_RD_INS(_n)			(((unsigned int)_n & 0xFF) << 8)
#define SFC_BUS_CONFIG1_RD_PREFETCH_CNT(_n)	((_n & 0x3) << 6)    
#define SFC_BUS_CONFIG1_RD_DUMMY_BYTES(_n)	(((unsigned int)_n & 0x7) << 3)
#define SFC_BUS_CONFIG1_RD_MEM_IF_TYPE(_n)	((unsigned int)_n & 0x7)
#define SFC_BUS_CONFIG1_RD_ENABLE			((HI_U32)(1 << 31))

#define SFC_REG_BUS_FLASH_SIZE			0x0210
#define SFC_REG_BUS_BASE_ADDR_CS0		0x0214

#define SFC_REG_BUS_DMA_CTRL			0X0240
#define SFC_BUS_DMA_CTRL_START				(1<<0)
#define SFC_BUS_DMA_CTRL_READ(_dir)			((_dir)<<1)
#define SFC_BUS_DMA_CTRL_CS(_cs)			(((_cs) & 0x01) << 4)
#define SFC_REG_BUS_DMA_MEM_SADDR		0X0244/*DMA DDR start address R*/
#define SFC_REG_BUS_DMA_FLASH_SADDR		0X0248
#define SFC_REG_BUS_DMA_LEN				0x024C
#define SFC_REG_BUS_DMA_AHB_CTRL		0X0250
#define SFC_BUS_DMA_AHB_CTRL_INCR4_EN		(1<<0)
#define SFC_BUS_DMA_AHB_CTRL_INCR8_EN		(1<<1)
#define SFC_BUS_DMA_AHB_CTRL_INCR16_EN		(1<<2)


#define SFC_REG_CMD_CONFIG				0x0300/* 命令操作方式配置寄存器*/
#define SFC_CMD_CONFIG_MEM_IF_TYPE(_n)		(((_n) & 0x07) << 17)
#define SFC_CMD_CONFIG_DATA_CNT(_n)			((((unsigned int)_n - 1) & 0xFF) << 9)
#define SFC_CMD_CONFIG_RW					(1<<8)
#define SFC_CMD_CONFIG_DATA_EN				(1<<7)	
#define SFC_CMD_CONFIG_DUMMY_BYTE_CNT(_n)	(((_n) & 0x07) << 4)
#define SFC_CMD_CONFIG_ADDR_EN				(1<<3)
#define SFC_CMD_CONFIG_SEL_CS(_cs)			(((unsigned int)_cs & 0x01) << 1)
#define SFC_CMD_CONFIG_START				(1 << 0)
#define SFC_REG_CMD_INS					0x0308/* 命令操作方式指令寄存器*/

#define SFC_REG_CMD_ADDR				0x030C
#define SFC_CMD_ADDR_MASK				(0xFFFFFFFF)
#define	SFC_REG_CMD_DATABUF1			0x0400

typedef struct SPI_FLASH_CTRL
{
	HI_CONST HI_CHAR *pName;

	HI_U32	ulSize;//所有片选上的flash容量之和
	HI_U32	ulEraseSize;
	HI_U32	ulWriteSize;//用用flash信息中的0下标的写size
	HI_U32	ulChips;		//用flash信息中的0下标，作为ctrl中的片选值
	HI_U32 (*pRead)(struct SPI_FLASH_CTRL *pstSpiFlashCtrl, HI_U32 ulOffset, HI_U32 ulLen, HI_VOID *pbuf, HI_BOOL bIscrash);//增加死机读flash接口20130219
	HI_U32 (*pWrite)(struct SPI_FLASH_CTRL *pstSpiFlashCtrl, HI_U32 ulOffset, HI_U32 ulLen, HI_CONST HI_VOID *pbuf, HI_BOOL bIscrash);
	HI_U32 (*pErase)(struct SPI_FLASH_CTRL *pstSpiFlashCtrl, HI_U32 ulOffset, HI_U32 ulLen, HI_BOOL bIscrash);
    HI_U8 *pTempBuffer;
}SPI_FLASH_CTRL_S;
SPI_FLASH_CTRL_S *spi_flash_probe(HI_VOID);

typedef struct
{
    HI_CHAR *pName;
    HI_S32 slChipSelect; 
    HI_U32 ulChipSize;
    HI_U32 ulEraseSize;
    HI_VOID *pIoBase;
    HI_U32 ulAddrCycle;
    SPI_OPERATION_S stRead[1];
    SPI_OPERATION_S stWrite[1];
    SPI_OPERATION_S stErase[8];
}SPI_FLASH_INFO_S;

typedef struct 
{
    SPI_FLASH_CTRL_S pstSpiFlashCtrl[1];
    HI_VOID *pIobase;
    HI_U32 ulBaseAddr;
    HI_VOID *pLock;
    HI_VOID *pCfgReg;
    HI_U32 ulEraseSize;
    

    HI_CHAR *pBuffer;
    HI_U32 ulDmabuffer;
    HI_S32 slNumChip;
    SPI_FLASH_INFO_S stSpiFlashInfo[CONFIG_HI3911_CHIP_NUM + 1];//数目是否应该直接为NUM?
}SFC_HOST_S;

#define SPIFLASH_TO_HOST(_pstSpiFlashCtrl)          ((SFC_HOST_S *)(_pstSpiFlashCtrl))
/*****************************************************************************/
#define hisfc_read(_host, _reg) \
    HI_REG_READ_VAL32(_host->ulBaseAddr +(_reg))
    
#define hisfc_write(_host, _reg, _value) \
    HI_REG_WRITE32(_host->ulBaseAddr + (_reg), (_value))
    
/*****************************************************************************/
#ifndef DRV_DEBUG

#define DBG_MSG(_fmt, arg...)
#define DBG_WARN(_fmt, arg...)

#else

#define DBG_MSG(_fmt, arg...) \
    HI_UART_Printf("%s(%d): " _fmt, __FILE__, __LINE__, ##arg)
#define DBG_WARN(_fmt, arg...) \
    HI_UART_Printf("%s(%d): " _fmt, __FILE__, __LINE__, ##arg)

#endif

#ifndef TERMINAL_DISPLAY
#define DBG_BUG(fmt, args...)
#else
#define DBG_BUG(fmt, args...) do{\
    HI_UART_Printf("%s(%d): BUG !!! " fmt, __FILE__, __LINE__, ##args); \
    for(;;)\
    ; \
} while (0)
#endif 
/***********************************/
HI_U32 spif_dma_read(SPI_FLASH_CTRL_S *pstSpiFlashCtrl, HI_U32 ulFrom, HI_U32 ulLen, HI_VOID *pBuf, HI_BOOL bIsCrash);
HI_U32 spif_reg_erase(SPI_FLASH_CTRL_S *pstSpiFlashCtrl, HI_U32 ulOffset, HI_U32 ulLength, HI_BOOL bIscrash);
HI_U32 spif_dma_write(SPI_FLASH_CTRL_S *pstSpiFlashCtrl, HI_U32 ulTo, HI_U32 ulLen, HI_CONST HI_VOID *pBuf, HI_BOOL bIscrash);
HI_VOID spif_write_enable(SFC_HOST_S *pstHost, SPI_FLASH_INFO_S *pstInfo);

#ifdef PRODUCT_CFG_FLASH_PROTECT

HI_EXTERN SFC_HOST_S g_stSfc300Host;
HI_S32 spif_wait_ready(SFC_HOST_S *pstHost, SPI_FLASH_INFO_S *pstInfo, HI_BOOL bIsCrash);
HI_U32 spif_read_status_reg(SFC_HOST_S *pstHost, SPI_FLASH_INFO_S *pstInfo, HI_U8 cmd);
HI_VOID spif_write_status_reg(SFC_HOST_S *pstHost, SPI_FLASH_INFO_S *pstInfo, 
                                        HI_U8 cmd,unsigned int status_val,HI_U8 data_len);
HI_VOID spif_wp_protect_ctrl(SFC_HOST_S *pstHost,HI_BOOL enable);
SPI_INFO_S * spif_get_spiinfo(HI_VOID);
                                        
#endif

#endif /* SFC3911H */

