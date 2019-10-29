
#ifndef __SPI_FLASH_PROTECT_DRV__
#define __SPI_FLASH_PROTECT_DRV__

#include "spi_ids.h"
#include "platform.h"


#ifdef PRODUCT_CFG_FLASH_PROTECT


//-----------------------------------------------------------------------------
#define SPI_PROTECT_512KB                           ((1<<4)|(1<<5))                     /* protect top 512KB */
#define SPI_PROTECT_32KB                            ((1<<4)|(1<<5)|(1<<6))              /* protect top 32KB */
#define SPI_PROTECT_SET_SRP0                        (1<<7)                              /* SRP0 */
#define SPI_PROTECT_NONE                            (0)                                 /* protect none */
#define SPI_PROTECT_MASK_512KB                      ((1<<2)|(1<<3)|(1<<4)|(1<<5)|(1<<6))/* mask for 512KB */
#define SPI_PROTECT_MASK_32KB                       ((1<<3)|(1<<4)|(1<<5)|(1<<6))       /* mask for 32KB */
#define SPI_PROTECT_MASK_NONE                       ((1<<2)|(1<<3)|(1<<4))              /* mask for none */
//-----------------------------------------------------------------------------
#define SPI_PROTECT_MAX_TIMES_WR_STATUS_REG         (3)  //写状态寄存器最大尝试次数
#define FLASH_PROTECT_MAGIC     MAKEU16('F','P')  // 0x5046  //用‘FP’的ascii码  Flash Protect  0101 0000 0100 0110
//#define FIRST_SHRINK_MAGIC  ((u_char)('S'))   //用于判断是否是第一次擦、写。0x53   0101 0011
//-----------------------------------------------------------------------------
/*****************************************************************************
 函 数 名  : spif_get_protect_magic
 功能描述  : 获取flash保护魔术字，U16类型
 输入参数  : 
 输出参数  : 无
*****************************************************************************/
#define hisfc350_get_protect_magic()        (readl(GENERAL_REG_FLASH_PROTECT_MAGIC)&0xFFFF)

void hisfc350_set_protect_magic(struct spi_info *spiinfo);
int hisfc350_shrink_protect_area(unsigned int ulOffset);


#endif




#endif
