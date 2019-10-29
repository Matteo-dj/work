/******************************************************************************
*    Copyright (c) 2009-2010 by czy.
*    All rights reserved.
* ***
*    Create by Cai ZhiYong. 2010-12-18
*    Changed by Wu Xianfeng 2011-09
******************************************************************************/

#ifndef SPI_IDSH
#define SPI_IDSH

/*****************************************************************************/
#ifndef __INCcoreh 
#define _1K             (0x400)
#define _2K             (0x800)

#define _4K             (0x1000)
#define _8K             (0x2000)
#define _16K            (0x4000)
#define _32K            (0x8000)

#define _64K            (0x10000)
#define _128K           (0x20000)
#define _256K           (0x40000)
#define _512K           (0x80000)

#define _1M             (0x100000)
#define _2M             (0x200000)
#define _4M             (0x400000)
#define _8M             (0x800000)

#define _16M            (0x1000000)
#define _32M            (0x2000000)

#endif
#define SPI_INFINITE        (0xFFFFFFFF)
/*****************************************************************************/

#define SPI_IF_READ_STD           (0x01)
#define SPI_IF_READ_FAST          (0x02)
#define SPI_IF_READ_DUAL          (0x04)
#define SPI_IF_READ_DUAL_ADDR     (0x08)
#define SPI_IF_READ_QUAD          (0x10)
#define SPI_IF_READ_QUAD_ADDR     (0x20)

#define SPI_IF_WRITE_STD          (0x01)
#define SPI_IF_WRITE_DUAL         (0x02)
#define SPI_IF_WRITE_DUAL_ADDR    (0x04)
#define SPI_IF_WRITE_QUAD         (0x08)
#define SPI_IF_WRITE_QUAD_ADDR    (0x10)

#define SPI_IF_ERASE_SECTOR       (0x01)  /* sector erase, 64K */
#define SPI_IF_ERASE_CHIP         (0x02)  /* chip erase */
#define SPI_IF_ERASE_4K           (0x04)  /* 4K */
#define SPI_IF_ERASE_8K           (0x08)  /* 8K */
/*****************************************************************************/

#define SPI_CMD_WREN                                0x06   /* Write Enable */
//-----------------------------------------------------------------------------
#define SPI_CMD_SE_4K                               0x20   /* 4KB sector Erase */ 
#define SPI_CMD_SE_32K                              0x52   /* 32KB sector Erase */ 
#define SPI_CMD_SE                                  0xD8   /* 64KB Sector Erase */
#define SPI_CMD_BE                                  0xC7   /* chip erase */
//-----------------------------------------------------------------------------
#define SPI_CMD_RDSR                                0x05   /* Read Status Register 1*/
#define SPI_CMD_RDSR2                               0x35   /* Read Status Register 2*/
#define SPI_CMD_RDSR3                               0x15   /* Read Status Register 3*/
#define SPI_CMD_WRSR1                               0x01   /* Write Status Register 1*/
#define SPI_CMD_WRSR2                               0x31   /* Write Status Register 2*/
#define SPI_CMD_WRSR3                               0x11   /* Write Status Register 3*/
#define SPI_CMD_RDID                                0x9F   /* Read Identification */
//-----------------------------------------------------------------------------
#define SPI_CMD_PP                                  0x02   /* Page Programming */
#define SPI_CMD_WRITE_DUAL                          0xA2   /* fast program dual input */
#define SPI_CMD_WRITE_QUAD                          0x32   /* fast program quad input */
#define SPI_CMD_WRITE_DUAL_ADDR                     0xD2   /* Dual I/O High Performance Write */
#define SPI_CMD_WRITE_QUAD_ADDR                     0x12   /* Quad I/O High Performance Write */
//-----------------------------------------------------------------------------
#define SPI_CMD_READ                                0x03   /* Read Data bytes */
#define SPI_CMD_FAST_READ                           0x0B   /* Read Data Bytes at Higher Speed */
#define SPI_CMD_READ_DUAL                           0x3B   /* fast read dual output */
#define SPI_CMD_READ_QUAD                           0x6B   /* fast read quad output */
#define SPI_CMD_READ_DUAL_ADDR                      0xBB   /* Dual I/O High Performance Read */
#define SPI_CMD_READ_QUAD_ADDR                      0xEB   /* Quad I/O High Performance Read */
//-----------------------------------------------------------------------------
#define SPI_CMD_SR_WIP                              1      /* Write in Progress */
#define SPI_CMD_SR_WEL                              2      /* Write Enable Latch */
//-----------------------------------------------------------------------------
#define SPI_CMD_EN4B                                0xB7  /* enter to 4 bytes mode and set 4 byte bit as '1' */
#define SPI_CMD_EX4B                                0xE9  /* exit 4 bytes mode and clear 4 byte bit as '0' */
//-----------------------------------------------------------------------------
#define FLASH_PROTECT_NOT_SUPPORT                   ((HI_U16)(0))
#define FLASH_PROTECT_SUPPORT_TYPE_ONE              HI_MAKEU16('O','E')//ONE 0x4f 0x4E   
#define FLASH_PROTECT_SUPPORT_TYPE_TWO              HI_MAKEU16('T','W')//TWO
#define FLASH_PROTECT_SUPPORT_TYPE_THREE            HI_MAKEU16('T','H')//THREE

/*****************************************************************************/

typedef struct
{
    HI_U8 ucIfType;
    HI_U8 ucCmd;
    HI_U8 ucDummy;
    HI_U8 ucPad;
    HI_U32 ulSize;
    HI_U32 ulClock;
}SPI_OPERATION_S;

#define MAX_SPI_OP (8)
typedef struct
{
    HI_CHAR *pName;
    HI_U8 aulId[8];
    HI_U16 usIdLen;
    HI_U16 usProtectType;
    HI_U32 ulChipSize;
    HI_U32 ulEraseSize;
    HI_U32 ulAddrCycle;
    SPI_OPERATION_S *pstRead[8];
    SPI_OPERATION_S *pstWrite[8];
    SPI_OPERATION_S *pstErase[8];
}SPI_INFO_S;
/*****************************************************************************/

SPI_INFO_S *spi_serach_ids(HI_U8 aucIds[8]);

HI_VOID spi_search_rw(SPI_INFO_S *stSpiInfo, SPI_OPERATION_S *stSpiOpRw,
    HI_U32 ulIfType, HI_U32 ulMaxDummy, HI_BOOL bIsRead);

HI_VOID spi_get_erase(SPI_INFO_S *stSpiInfo, SPI_OPERATION_S *stSpiOp, HI_U32 *pEraseSize);
/******************************************************************************/
#endif /* SPI_IDSH */
