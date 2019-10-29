/******************************************************************************
*    Copyright (c) 2009-2010 by czy.
*    All rights reserved.
* ***
*    Create by Cai ZhiYong. 2010-12-18
*    Changed by Wu Xianfeng. 2012-5-26
******************************************************************************/
#include <boot/boot.h>
#include "spi_ids.h"
#include <boot/convert.h>
#include <product.h>
#include "securec.h"

//#define SPI_DRV_VERSION       "1.10"
/*****************************************************************************/

#ifndef DISPLAY_DEBUG_INFO
#  define DBG_MSG(_fmt, arg...)
#else
#  define DBG_MSG(_fmt, arg...)   \
    printf("%s(%d): " _fmt, __FILE__, __LINE__, ##arg);
#endif

#ifndef DISPLAY_DEBUG_INFO
#  define DBG_BUG(_fmt, arg...)
#else
#define DBG_BUG(fmt, args...) do{\
    printf("%s(%d): BUG !!! " fmt, __FILE__, __LINE__, ##args); \
    while(1); \
} while (0)
#endif

/* compatible with flash chip company */
//#define FLASH_AT
//#define FLASH_Spansion
//#define FLASH_SST
//#define FLASH_ST
#define FLASH_MX
#define FLASH_Winbond
#define FLASH_cFeon

/*****************************************************************************/

#define SET_READ_STD(_dummy_, _size_, _clk_) \
    static struct spi_operation read_std_##_dummy_##_size_##_clk_ = { \
    SPI_IF_READ_STD, SPI_CMD_READ, _dummy_, 0, _size_, _clk_ }

#define READ_STD(_dummy_, _size_, _clk_) read_std_##_dummy_##_size_##_clk_
//-----------------------------------------------------------------------------

#define SET_READ_FAST(_dummy_, _size_, _clk_) \
    static struct spi_operation read_fast_##_dummy_##_size_##_clk_ = { \
    SPI_IF_READ_FAST, SPI_CMD_FAST_READ, _dummy_, 0, _size_, _clk_ }

#define READ_FAST(_dummy_, _size_, _clk_) read_fast_##_dummy_##_size_##_clk_
//-----------------------------------------------------------------------------

#define SET_READ_DUAL(_dummy_, _size_, _clk_) \
    static struct spi_operation read_dual_##_dummy_##_size_##_clk_ = { \
    SPI_IF_READ_DUAL, SPI_CMD_READ_DUAL, _dummy_, 0, _size_, _clk_ }

#define READ_DUAL(_dummy_, _size_, _clk_) read_dual_##_dummy_##_size_##_clk_
//-----------------------------------------------------------------------------

#define SET_READ_QUAD(_dummy_, _size_, _clk_) \
    static struct spi_operation read_quad_##_dummy_##_size_##_clk_ = { \
    SPI_IF_READ_QUAD, SPI_CMD_READ_QUAD, _dummy_, 0, _size_, _clk_ }

#define READ_QUAD(_dummy_, _size_, _clk_) read_quad_##_dummy_##_size_##_clk_
//-----------------------------------------------------------------------------

#define SET_READ_DUAL_ADDR(_dummy_, _size_, _clk_) \
    static struct spi_operation read_dual_addr_##_dummy_##_size_##_clk_ = { \
    SPI_IF_READ_DUAL_ADDR, SPI_CMD_READ_DUAL_ADDR, _dummy_, 0, _size_, _clk_ }

#define READ_DUAL_ADDR(_dummy_, _size_, _clk_) read_dual_addr_##_dummy_##_size_##_clk_
//-----------------------------------------------------------------------------

#define SET_READ_QUAD_ADDR(_dummy_, _size_, _clk_) \
    static struct spi_operation read_quad_addr_##_dummy_##_size_##_clk_ = { \
    SPI_IF_READ_QUAD_ADDR, SPI_CMD_READ_QUAD_ADDR, _dummy_, 0, _size_, _clk_ }

#define READ_QUAD_ADDR(_dummy_, _size_, _clk_) read_quad_addr_##_dummy_##_size_##_clk_
//-----------------------------------------------------------------------------

#define SET_WRITE_STD(_dummy_, _size_, _clk_) \
    static struct spi_operation write_std_##_dummy_##_size_##_clk_ = { \
    SPI_IF_WRITE_STD, SPI_CMD_PP, _dummy_, 0, _size_, _clk_ }

#define WRITE_STD(_dummy_, _size_, _clk_) write_std_##_dummy_##_size_##_clk_
//-----------------------------------------------------------------------------

#define SET_WRITE_DUAL(_dummy_, _size_, _clk_) \
    static struct spi_operation write_dual_##_dummy_##_size_##_clk_ = { \
    SPI_IF_WRITE_DUAL, SPI_CMD_WRITE_DUAL, _dummy_, 0, _size_, _clk_ }

#define WRITE_DUAL(_dummy_, _size_, _clk_) write_dual_##_dummy_##_size_##_clk_
//-----------------------------------------------------------------------------

#define SET_WRITE_QUAD(_dummy_, _size_, _clk_) \
    static struct spi_operation write_quad_##_dummy_##_size_##_clk_ = { \
    SPI_IF_WRITE_QUAD, SPI_CMD_WRITE_QUAD, _dummy_, 0, _size_, _clk_ }

#define WRITE_QUAD(_dummy_, _size_, _clk_) write_quad_##_dummy_##_size_##_clk_
//-----------------------------------------------------------------------------

#define SET_WRITE_DUAL_ADDR(_dummy_, _size_, _clk_) \
    static struct spi_operation write_dual_addr_##_dummy_##_size_##_clk_ = { \
    SPI_IF_WRITE_DUAL_ADDR, SPI_CMD_WRITE_DUAL_ADDR, _dummy_, 0, _size_, _clk_ }

#define WRITE_DUAL_ADDR(_dummy_, _size_, _clk_) write_dual_addr_##_dummy_##_size_##_clk_
//-----------------------------------------------------------------------------

#define SET_WRITE_QUAD_ADDR(_dummy_, _size_, _clk_) \
    static struct spi_operation write_quad_addr_##_dummy_##_size_##_clk_ = { \
    SPI_IF_WRITE_QUAD_ADDR, SPI_CMD_WRITE_QUAD_ADDR, _dummy_, 0, _size_, _clk_ }

#define WRITE_QUAD_ADDR(_dummy_, _size_, _clk_) write_quad_addr_##_dummy_##_size_##_clk_
//-----------------------------------------------------------------------------

#define SET_ERASE_SECTOR(_dummy_, _size_, _clk_) \
    static struct spi_operation erase_sector_##_dummy_##_size_##_clk_ = { \
    SPI_IF_ERASE_SECTOR, SPI_CMD_SE, _dummy_, 0, _size_, _clk_ }

#define ERASE_SECTOR(_dummy_, _size_, _clk_) erase_sector_##_dummy_##_size_##_clk_
//-----------------------------------------------------------------------------

#define SET_ERASE_CHIP(_dummy_, _size_, _clk_) \
    static struct spi_operation erase_chip_##_dummy_##_size_##_clk_ = { \
    SPI_IF_ERASE_CHIP, SPI_CMD_BE, _dummy_, 0, _size_, _clk_ }

#define ERASE_CHIP(_dummy_, _size_, _clk_) erase_chip_##_dummy_##_size_##_clk_

#define SET_ERASE_SECTOR_4K(_dummy_, _size_, _clk_) \
		static struct spi_operation erase_sector_4k_##_dummy_##_size_##_clk_ = { \
		SPI_IF_ERASE_4K, SPI_CMD_SE_4K, _dummy_, 0, _size_, _clk_ }

#define ERASE_SECTOR_4K(_dummy_, _size_, _clk_) erase_sector_4k_##_dummy_##_size_##_clk_


/*****************************************************************************/
SET_READ_STD(0, INFINITE, 40);
SET_READ_FAST(1, INFINITE, 50);
SET_READ_DUAL(1, INFINITE, 75);
//-----------------------------------------------------------------------------
SET_WRITE_STD(0, 256, 0);
SET_WRITE_STD(0, 256, 80);
SET_WRITE_STD(0, 256, 86);
//-----------------------------------------------------------------------------
SET_ERASE_SECTOR(0, SPI_IF_ERASE_SECTOR, 0);
SET_ERASE_CHIP(0, SPI_IF_ERASE_CHIP, 0);
/*****************************************************************************/
SET_ERASE_SECTOR_4K(0, SPI_IF_ERASE_4K, 0);

static struct spi_info spi_info_table[] =
{
    /* name        id                id_len chipsize(Bytes) erasesize */
#ifdef FLASH_AT
    {"at25fs010",  {0x1f, 0x66, 0x01}, 3, 0, _128K,  _32K, 3, {&READ_STD(0, INFINITE, 0), 0},  {&WRITE_STD(0, 256, 0), 0}, {&ERASE_SECTOR(0, SPI_IF_ERASE_SECTOR, 0), 0}},
    {"at25fs040",  {0x1f, 0x66, 0x04}, 3, 0, _512K,  _64K, 3, {&READ_STD(0, INFINITE, 0), 0},  {&WRITE_STD(0, 256, 0), 0}, {&ERASE_SECTOR(0, SPI_IF_ERASE_SECTOR, 0), 0}},

    {"at25df041a", {0x1f, 0x44, 0x01}, 3, 0, _512K,  _64K, 3, {&READ_STD(0, INFINITE, 0), 0},  {&WRITE_STD(0, 256, 0), 0}, {&ERASE_SECTOR(0, SPI_IF_ERASE_SECTOR, 0), 0}},
    {"at25df641",  {0x1f, 0x48, 0x00}, 3, 0, _8M,    _64K, 3, {&READ_STD(0, INFINITE, 0), 0},  {&WRITE_STD(0, 256, 0), 0}, {&ERASE_SECTOR(0, SPI_IF_ERASE_SECTOR, 0), 0}},

    {"at26f004",   {0x1f, 0x04, 0x00}, 3, 0, _512K,  _64K, 3, {&READ_STD(0, INFINITE, 0), 0},  {&WRITE_STD(0, 256, 0), 0}, {&ERASE_SECTOR(0, SPI_IF_ERASE_SECTOR, 0), 0}},
    {"at26df081a", {0x1f, 0x45, 0x01}, 3, 0, _1M,    _64K, 3, {&READ_STD(0, INFINITE, 0), 0},  {&WRITE_STD(0, 256, 0), 0}, {&ERASE_SECTOR(0, SPI_IF_ERASE_SECTOR, 0), 0}},
    {"at26df161a", {0x1f, 0x46, 0x01}, 3, 0, _2M,    _64K, 3, {&READ_STD(0, INFINITE, 0), 0},  {&WRITE_STD(0, 256, 0), 0}, {&ERASE_SECTOR(0, SPI_IF_ERASE_SECTOR, 0), 0}},
    {"at26df321",  {0x1f, 0x47, 0x01}, 3, 0, _4M,    _64K, 3, {&READ_STD(0, INFINITE, 0), 0},  {&WRITE_STD(0, 256, 0), 0}, {&ERASE_SECTOR(0, SPI_IF_ERASE_SECTOR, 0), 0}},
#endif
#ifdef FLASH_cFeon
    {"EN25F80",  {0x1c, 0x31, 0x14}, 3, 0, _1M,   _4K, 3,
#ifdef SUPPORT_EQUIP_TEST
        {&READ_STD(0, INFINITE, 40), 0},
#else
        {&READ_FAST(1, INFINITE, 50), 0},
#endif
        {&WRITE_STD(0, 256, 0), 0},
        {&ERASE_CHIP(0, SPI_IF_ERASE_CHIP, 0) ,
            &ERASE_SECTOR(0, SPI_IF_ERASE_SECTOR, 0),
                &ERASE_SECTOR_4K(0, SPI_IF_ERASE_4K, 0),0}},
    {"EN25QH16",  {0x1c, 0x70, 0x15}, 3, 0, _2M,   _4K, 3,
#ifdef SUPPORT_EQUIP_TEST
        {&READ_STD(0, INFINITE, 40), 0},
#else
        {&READ_DUAL(1, INFINITE, 75), 0},
#endif
        {&WRITE_STD(0, 256, 0), 0},
        {&ERASE_CHIP(0, SPI_IF_ERASE_CHIP, 0) ,
            &ERASE_SECTOR(0, SPI_IF_ERASE_SECTOR, 0),
                &ERASE_SECTOR_4K(0, SPI_IF_ERASE_4K, 0),0}},
#endif /*FLASH_cFeon*/
    /* Macronix */
#ifdef FLASH_MX
    //{"mx25l4005a",  {0xc2, 0x20, 0x13}, 3, _512K,  _64K, 3, {&READ_STD(0, INFINITE, 0), 0},  {&WRITE_STD(0, 256, 0), 0}, {&ERASE_SECTOR(0, SPI_IF_ERASE_SECTOR, 0), 0}},
    {"MX25L80",  {0xc2, 0x20, 0x14}, 3, 0, _1M, _4K, 3,
#ifdef SUPPORT_EQUIP_TEST
        {&READ_STD(0, INFINITE, 40), 0},
#else
        {&READ_DUAL(1, INFINITE, 75), 0},
#endif
        {&WRITE_STD(0, 256, 86), 0},
        {&ERASE_CHIP(0, SPI_IF_ERASE_CHIP, 0),
            &ERASE_SECTOR(0, SPI_IF_ERASE_SECTOR, 0),
                &ERASE_SECTOR_4K(0, SPI_IF_ERASE_4K, 0),0}},

    {"MX25L32",  {0xc2, 0x20, 0x16}, 3, 0, _4M, _4K, 3,
#ifdef SUPPORT_EQUIP_TEST
        {&READ_STD(0, INFINITE, 40), 0},
#else
        {&READ_DUAL(1, INFINITE, 75), 0},
#endif
        {&WRITE_STD(0, 256, 0), 0},
        {&ERASE_CHIP(0, SPI_IF_ERASE_CHIP, 0) ,
            &ERASE_SECTOR(0, SPI_IF_ERASE_SECTOR, 0),
                &ERASE_SECTOR_4K(0, SPI_IF_ERASE_4K, 0),0}},

    {"MX25L16",  {0xc2, 0x20, 0x15}, 3, 0, _2M, _4K, 3,
#ifdef SUPPORT_EQUIP_TEST
        {&READ_STD(0, INFINITE, 40), 0},
#else
        {&READ_DUAL(1, INFINITE, 75), 0},
#endif
        {&WRITE_STD(0, 256, 0), 0},
        {&ERASE_CHIP(0, SPI_IF_ERASE_CHIP, 0) ,
            &ERASE_SECTOR(0, SPI_IF_ERASE_SECTOR, 0),
                &ERASE_SECTOR_4K(0, SPI_IF_ERASE_4K, 0),0}},

    {"MX25L64",  {0xc2, 0x20, 0x17}, 3, 0, _8M,    _4K, 3,
#ifdef SUPPORT_EQUIP_TEST
        {&READ_STD(0, INFINITE, 40), 0},
#else
        {&READ_DUAL(1, INFINITE, 75), 0},
#endif
        {&WRITE_STD(0, 256, 0), 0},
        {&ERASE_CHIP(0, SPI_IF_ERASE_CHIP, 0),
            &ERASE_SECTOR(0, SPI_IF_ERASE_SECTOR,0),
                &ERASE_SECTOR_4K(0, SPI_IF_ERASE_4K, 0),0}},
#ifndef FLASH_MINI_BOOT
    {"MX25L128", {0xc2, 0x20, 0x18}, 3, 0, _16M, _64K, 3,
        {&READ_STD(0, INFINITE, 50), &READ_FAST(1, INFINITE, 108), 0},
        {&WRITE_STD(0, 256, 108), 0},
        {&ERASE_CHIP(0, SPI_IF_ERASE_CHIP, 108), &ERASE_SECTOR(0, SPI_IF_ERASE_SECTOR, 108), 0}},
    /*
     The follow chips have the same chipid, but command have some difference

    {"MX25L12836E", {0xc2, 0x20, 0x18}, 3, _16M,   _64K, 3,
        {&READ_STD(0, INFINITE, 50), &READ_FAST(1, INFINITE, 108), 0},
        {&WRITE_STD(0, 256, 108), 0},
        {&ERASE_CHIP(0, SPI_IF_ERASE_CHIP, 108), &ERASE_SECTOR(0, SPI_IF_ERASE_SECTOR, 108), 0}},

    {"MX25L12845E", {0xc2, 0x20, 0x18}, 3, _16M,   _64K, 3,
        {&READ_STD(0, INFINITE, 50), &READ_FAST(1, INFINITE, 108), 0},
        {&WRITE_STD(0, 256, 108), 0},
        {&ERASE_CHIP(0, SPI_IF_ERASE_CHIP, 108), &ERASE_SECTOR(0, SPI_IF_ERASE_SECTOR, 108), 0}},
    */

    {"MX25L25635E", {0xc2, 0x20, 0x19}, 3, 0, _32M, _64K, 4,
        {&READ_STD(0, INFINITE, 40/*50*/), &READ_FAST(1, INFINITE, 64/*80*/), &READ_DUAL_ADDR(1, INFINITE, 56/*70*/), &READ_QUAD_ADDR(2, INFINITE, 56/*70*/), 0},
        {&WRITE_STD(0, 256, 64/*80*/), 0},
        {&ERASE_CHIP(0, SPI_IF_ERASE_CHIP,64/*80*/), &ERASE_SECTOR(0, SPI_IF_ERASE_SECTOR, 64/*80*/), 0}},

    {"mx25l1655d",  {0xc2, 0x26, 0x15}, 3, 0, _2M,    _64K, 3, {&READ_STD(0, INFINITE, 0), 0},  {&WRITE_STD(0, 256, 0), 0}, {&ERASE_SECTOR(0, SPI_IF_ERASE_SECTOR, 0), 0}},
    {"mx25l12855e", {0xc2, 0x26, 0x18}, 3, 0, _16M,   _64K, 3, {&READ_STD(0, INFINITE, 0), 0},  {&WRITE_STD(0, 256, 0), 0}, {&ERASE_SECTOR(0, SPI_IF_ERASE_SECTOR, 0), 0}},
#endif /*FLASH_MINI_BOOT*/
#endif /*FLASH_MX*/
#ifdef FLASH_Spansion
    {"s25sl004a", {0x01, 0x02, 0x12}, 3, 0, (_64K * 8),   _64K, 3, {&READ_STD(0, INFINITE, 0), 0},  {&WRITE_STD(0, 256, 0), 0}, {&ERASE_SECTOR(0, SPI_IF_ERASE_SECTOR, 0), 0}},
    {"s25sl008a", {0x01, 0x02, 0x13}, 3, 0, (_64K * 16),  _64K, 3, {&READ_STD(0, INFINITE, 0), 0},  {&WRITE_STD(0, 256, 0), 0}, {&ERASE_SECTOR(0, SPI_IF_ERASE_SECTOR, 0), 0}},
    {"s25sl016a", {0x01, 0x02, 0x14}, 3, 0, (_64K * 32),  _64K, 3, {&READ_STD(0, INFINITE, 0), 0},  {&WRITE_STD(0, 256, 0), 0}, {&ERASE_SECTOR(0, SPI_IF_ERASE_SECTOR, 0), 0}},
    {"s25sl064a", {0x01, 0x02, 0x16}, 3, 0, (_64K * 128), _64K, 3, {&READ_STD(0, INFINITE, 0), 0},  {&WRITE_STD(0, 256, 0), 0}, {&ERASE_SECTOR(0, SPI_IF_ERASE_SECTOR, 0), 0}},

    {"S25FL064P", {0x01, 0x02, 0x16, 0x4d}, 4, 0, (_64K * 128), _64K, 3,
        {&READ_STD(0, INFINITE, 40), &READ_FAST(1, INFINITE, 104), &READ_DUAL(1, INFINITE, 80), &READ_QUAD(1, INFINITE, 80), 0},
        {&WRITE_STD(0, 256, 104), &WRITE_QUAD(0, 256, 80), 0},
        {&ERASE_CHIP(0, SPI_IF_ERASE_CHIP, 104), &ERASE_SECTOR(0, SPI_IF_ERASE_SECTOR, 104), 0}},

    /* Spansion */
    {"S25FL032A", {0x01, 0x02, 0x15}, 3, 0, (_64K * 64),  _64K, 3,
        {&READ_STD(0, INFINITE, 33), &READ_FAST(1, INFINITE, 50), 0},
        {&WRITE_STD(0, 256, 50), 0},
        {&ERASE_CHIP(0, SPI_IF_ERASE_CHIP, 50), &ERASE_SECTOR(0, SPI_IF_ERASE_SECTOR, 50), 0}},

    {"S25FL032P", {0x01, 0x02, 0x15, 0x4d}, 4, 0, (_64K * 64),  _64K, 3,
        {&READ_STD(0, INFINITE, 40), &READ_FAST(1, INFINITE, 104), &READ_DUAL(1, INFINITE, 80), &READ_QUAD(1, INFINITE, 80), &READ_DUAL_ADDR(0, INFINITE, 80), &READ_QUAD_ADDR(2, INFINITE, 80), 0},
        {&WRITE_STD(0, 256, 104), &WRITE_QUAD(0, 256, 80), 0},
        {&ERASE_CHIP(0, SPI_IF_ERASE_CHIP, 104), &ERASE_SECTOR(0, SPI_IF_ERASE_SECTOR, 104), 0}},

    {"S25FL129P0", {0x01, 0x20, 0x18, 0x4d, 0x00}, 5, 0, (_256K * 64),  _256K, 3,
        {&READ_STD(0, INFINITE, 40),&READ_FAST(1, INFINITE, 104), &READ_DUAL(1, INFINITE, 80), &READ_QUAD(1, INFINITE, 80), &READ_DUAL_ADDR(0, INFINITE, 80), &READ_QUAD_ADDR(2, INFINITE, 80), 0},
        {&WRITE_STD(0, 256, 104), &WRITE_QUAD(0, 256, 80), 0},
        {&ERASE_CHIP(0, SPI_IF_ERASE_CHIP, 104), &ERASE_SECTOR(0, SPI_IF_ERASE_SECTOR, 104), 0}},

    {"S25FL129P1", {0x01, 0x20, 0x18, 0x4d, 0x01}, 5, 0, (_64K * 256),  _64K,  3,
        {&READ_STD(0, INFINITE, 40),&READ_FAST(1, INFINITE, 104), &READ_DUAL(1, INFINITE, 80), &READ_QUAD(1, INFINITE, 80), &READ_DUAL_ADDR(0, INFINITE, 80), &READ_QUAD_ADDR(2, INFINITE, 80), 0},
        {&WRITE_STD(0, 256, 104), &WRITE_QUAD(0, 256, 80), 0},
        {&ERASE_CHIP(0, SPI_IF_ERASE_CHIP, 104), &ERASE_SECTOR(0, SPI_IF_ERASE_SECTOR, 104), 0}},
#endif
#ifdef FLASH_SST
    /* SST -- large erase sizes are "overlays", "sectors" are 4K */
    { "sst25vf040b", {0xbf, 0x25, 0x8d}, 3, 0, (_64K * 8),  _64K, 3, {&READ_STD(0, INFINITE, 0), 0},  {&WRITE_STD(0, 256, 0), 0}, {&ERASE_SECTOR(0, SPI_IF_ERASE_SECTOR, 0), 0}},
    { "sst25vf080b", {0xbf, 0x25, 0x8e}, 3, 0, (_64K * 16), _64K, 3, {&READ_STD(0, INFINITE, 0), 0},  {&WRITE_STD(0, 256, 0), 0}, {&ERASE_SECTOR(0, SPI_IF_ERASE_SECTOR, 0), 0}},
    { "sst25vf016b", {0xbf, 0x25, 0x41}, 3, 0, (_64K * 32), _64K, 3, {&READ_STD(0, INFINITE, 0), 0},  {&WRITE_STD(0, 256, 0), 0}, {&ERASE_SECTOR(0, SPI_IF_ERASE_SECTOR, 0), 0}},
    { "sst25vf032b", {0xbf, 0x25, 0x4a}, 3, 0, (_64K * 64), _64K, 3, {&READ_STD(0, INFINITE, 0), 0},  {&WRITE_STD(0, 256, 0), 0}, {&ERASE_SECTOR(0, SPI_IF_ERASE_SECTOR, 0), 0}},
    { "sst25wf512",  {0xbf, 0x25, 0x01}, 3, 0, (_64K * 1),  _64K, 3, {&READ_STD(0, INFINITE, 0), 0},  {&WRITE_STD(0, 256, 0), 0}, {&ERASE_SECTOR(0, SPI_IF_ERASE_SECTOR, 0), 0}},
    { "sst25wf010",  {0xbf, 0x25, 0x02}, 3, 0, (_64K * 2),  _64K, 3, {&READ_STD(0, INFINITE, 0), 0},  {&WRITE_STD(0, 256, 0), 0}, {&ERASE_SECTOR(0, SPI_IF_ERASE_SECTOR, 0), 0}},
    { "sst25wf020",  {0xbf, 0x25, 0x03}, 3, 0, (_64K * 4),  _64K, 3, {&READ_STD(0, INFINITE, 0), 0},  {&WRITE_STD(0, 256, 0), 0}, {&ERASE_SECTOR(0, SPI_IF_ERASE_SECTOR, 0), 0}},
    { "sst25wf040",  {0xbf, 0x25, 0x04}, 3, 0, (_64K * 8),  _64K, 3, {&READ_STD(0, INFINITE, 0), 0},  {&WRITE_STD(0, 256, 0), 0}, {&ERASE_SECTOR(0, SPI_IF_ERASE_SECTOR, 0), 0}},
#endif
#ifdef FLASH_ST
    /* ST Microelectronics -- newer production may have feature updates */
    {"m25p05",  {0x20, 0x20, 0x10}, 3, 0, (_32K * 2), _32K, 3, {&READ_STD(0, INFINITE, 0), 0},  {&WRITE_STD(0, 256, 0), 0}, {&ERASE_SECTOR(0, SPI_IF_ERASE_SECTOR, 0), 0}},
    {"m25p10",  {0x20, 0x20, 0x11}, 3, 0, (_32K * 4), _32K, 3, {&READ_STD(0, INFINITE, 0), 0},  {&WRITE_STD(0, 256, 0), 0}, {&ERASE_SECTOR(0, SPI_IF_ERASE_SECTOR, 0), 0}},

    {"m25p20",  {0x20, 0x20, 0x12}, 3, 0, (_64K * 4),   _64K,  3, {&READ_STD(0, INFINITE, 0), 0},  {&WRITE_STD(0, 256, 0), 0}, {&ERASE_SECTOR(0, SPI_IF_ERASE_SECTOR, 0), 0}},
    {"m25p40",  {0x20, 0x20, 0x13}, 3, 0, (_64K * 8),   _64K,  3, {&READ_STD(0, INFINITE, 0), 0},  {&WRITE_STD(0, 256, 0), 0}, {&ERASE_SECTOR(0, SPI_IF_ERASE_SECTOR, 0), 0}},
    {"m25p80",  {0x20, 0x20, 0x14}, 3, 0, (_64K * 16),  _64K,  3, {&READ_STD(0, INFINITE, 0), 0},  {&WRITE_STD(0, 256, 0), 0}, {&ERASE_SECTOR(0, SPI_IF_ERASE_SECTOR, 0), 0}},
    {"m25p16",  {0x20, 0x20, 0x15}, 3, 0, (_64K * 32),  _64K,  3, {&READ_STD(0, INFINITE, 0), 0},  {&WRITE_STD(0, 256, 0), 0}, {&ERASE_SECTOR(0, SPI_IF_ERASE_SECTOR, 0), 0}},

    {"M25P32",  {0x20, 0x20, 0x16, 0x10}, 4, 0, _4M, _64K, 3,
        {&READ_STD(0, INFINITE, 33), &READ_FAST(1, INFINITE, 75), 0},
        {&WRITE_STD(0, 256, 75), 0},
        {&ERASE_CHIP(0, SPI_IF_ERASE_CHIP, 75), &ERASE_SECTOR(0, SPI_IF_ERASE_SECTOR, 75), 0}},

    {"m25p64",  {0x20, 0x20, 0x17}, 3, 0, (_64K * 128), _64K,  3, {&READ_STD(0, INFINITE, 0), 0},  {&WRITE_STD(0, 256, 0), 0}, {&ERASE_SECTOR(0, SPI_IF_ERASE_SECTOR, 0), 0}},

    {"M25P128", {0x20, 0x20, 0x18}, 3, 0, _16M, _256K, 3,
        {&READ_STD(0, INFINITE, 20), &READ_FAST(1, INFINITE, 50), 0},
        {&WRITE_STD(0, 256, 50), 0},
        {&ERASE_CHIP(0, SPI_IF_ERASE_CHIP, 50), &ERASE_SECTOR(0, SPI_IF_ERASE_SECTOR, 50), 0}},

    {"m45pe10", {0x20, 0x40, 0x11}, 3, 0, (_64K * 2),   _64K, 3, {&READ_STD(0, INFINITE, 0), 0},  {&WRITE_STD(0, 256, 0), 0}, {&ERASE_SECTOR(0, SPI_IF_ERASE_SECTOR, 0), 0}},
    {"m45pe80", {0x20, 0x40, 0x14}, 3, 0, (_64K * 16),  _64K, 3, {&READ_STD(0, INFINITE, 0), 0},  {&WRITE_STD(0, 256, 0), 0}, {&ERASE_SECTOR(0, SPI_IF_ERASE_SECTOR, 0), 0}},
    {"m45pe16", {0x20, 0x40, 0x15}, 3, 0, (_64K * 32),  _64K, 3, {&READ_STD(0, INFINITE, 0), 0},  {&WRITE_STD(0, 256, 0), 0}, {&ERASE_SECTOR(0, SPI_IF_ERASE_SECTOR, 0), 0}},

    {"m25pe80", {0x20, 0x80, 0x14}, 3, 0, (_64K * 16), _64K, 3, {&READ_STD(0, INFINITE, 0), 0},  {&WRITE_STD(0, 256, 0), 0}, {&ERASE_SECTOR(0, SPI_IF_ERASE_SECTOR, 0), 0}},
    {"m25pe16", {0x20, 0x80, 0x15}, 3, 0, (_64K * 32), _64K, 3, {&READ_STD(0, INFINITE, 0), 0},  {&WRITE_STD(0, 256, 0), 0}, {&ERASE_SECTOR(0, SPI_IF_ERASE_SECTOR, 0), 0}},

    {"N25Q128",   {0x20, 0xba, 0x18}, 3, 0, (_64K * 256), _64K, 3,
        {&READ_STD(0, INFINITE, 54), &READ_FAST(1, INFINITE, 108), &READ_DUAL(1, INFINITE, 108), &READ_QUAD(1, INFINITE, 108), &READ_DUAL_ADDR(2, INFINITE, 108), &READ_QUAD_ADDR(5, INFINITE, 108), 0},
        {&WRITE_STD(0, 256, 108), &WRITE_DUAL(0, 256, 108), &WRITE_QUAD(0, 256, 108), &WRITE_DUAL_ADDR(0, 256, 108), &WRITE_QUAD_ADDR(0, 256, 108), 0},
        {&ERASE_CHIP(0, SPI_IF_ERASE_CHIP, 108), &ERASE_SECTOR(0, SPI_IF_ERASE_SECTOR, 108), 0}},

    {"M25PX16",  {0x20, 0x71, 0x15}, 3, 0, (_64K * 32),  _64K, 3,
        {&READ_STD(0, INFINITE, 33), &READ_FAST(1, INFINITE, 75), &READ_DUAL(1, INFINITE, 75), 0},
        {&WRITE_STD(0, 256, 75), &WRITE_DUAL(0, 256, 75), 0},
        {&ERASE_CHIP(0, SPI_IF_ERASE_CHIP, 75), &ERASE_SECTOR(0, SPI_IF_ERASE_SECTOR, 75), 0}},

    {"M25PX32", {0x20, 0x71, 0x16}, 3, 0, (_64K * 64),  _64K, 3,
        {&READ_STD(0, INFINITE, 33), &READ_FAST(1, INFINITE, 75), &READ_DUAL(1, INFINITE, 75), 0},
        {&WRITE_STD(0, 256, 75), &WRITE_DUAL(0, 256, 75), 0},
        {&ERASE_CHIP(0, SPI_IF_ERASE_CHIP, 75), &ERASE_SECTOR(0, SPI_IF_ERASE_SECTOR, 75), 0}},

    {"m25px64",  {0x20, 0x71, 0x17}, 3, 0, (_64K * 128), _64K, 3, {&READ_STD(0, INFINITE, 0), 0},  {&WRITE_STD(0, 256, 0), 0}, {&ERASE_SECTOR(0, SPI_IF_ERASE_SECTOR, 0), 0}},
#endif
#ifdef FLASH_Winbond
    /* Winbond -- w25x "blocks" are 64K, "sectors" are 4KiB */
#ifndef FLASH_MINI_BOOT
    {"w25x10",  {0xef, 0x30, 0x11}, 3, 0, (_64K * 2),    _64K, 3, {&READ_STD(0, INFINITE, 0), 0},  {&WRITE_STD(0, 256, 0), 0}, {&ERASE_SECTOR(0, SPI_IF_ERASE_SECTOR, 0), 0}},
    {"w25x20",  {0xef, 0x30, 0x12}, 3, 0, (_64K * 4),    _64K, 3, {&READ_STD(0, INFINITE, 0), 0},  {&WRITE_STD(0, 256, 0), 0}, {&ERASE_SECTOR(0, SPI_IF_ERASE_SECTOR, 0), 0}},
    {"w25x40",  {0xef, 0x30, 0x13}, 3, 0, (_64K * 8),    _64K, 3, {&READ_STD(0, INFINITE, 0), 0},  {&WRITE_STD(0, 256, 0), 0}, {&ERASE_SECTOR(0, SPI_IF_ERASE_SECTOR, 0), 0}},
    {"w25x80",  {0xef, 0x30, 0x14}, 3, 0, (_64K * 16),   _64K, 3, {&READ_STD(0, INFINITE, 0), 0},  {&WRITE_STD(0, 256, 0), 0}, {&ERASE_SECTOR(0, SPI_IF_ERASE_SECTOR, 0), 0}},
    {"w25x16",  {0xef, 0x30, 0x15}, 3, 0, (_64K * 32),   _64K, 3, {&READ_STD(0, INFINITE, 0), 0},  {&WRITE_STD(0, 256, 0), 0}, {&ERASE_SECTOR(0, SPI_IF_ERASE_SECTOR, 0), 0}},
    {"w25x32",  {0xef, 0x30, 0x16}, 3, 0, (_64K * 64),   _64K, 3, {&READ_STD(0, INFINITE, 0), 0},  {&WRITE_STD(0, 256, 0), 0}, {&ERASE_SECTOR(0, SPI_IF_ERASE_SECTOR, 0), 0}},
    {"w25x64",  {0xef, 0x30, 0x17}, 3, 0, (_64K * 128),  _64K, 3, {&READ_STD(0, INFINITE, 0), 0},  {&WRITE_STD(0, 256, 0), 0}, {&ERASE_SECTOR(0, SPI_IF_ERASE_SECTOR, 0), 0}},
#endif /*FLASH_MINI_BOOT*/
    {"w25Q80BV",  {0xef, 0x40, 0x14}, 3, FLASH_PROTECT_SUPPORT_TYPE_ONE, (_64K * 16), _4K, 3,
#ifdef SUPPORT_EQUIP_TEST
        {&READ_STD(0, INFINITE, 40), 0},
#else
        {&READ_DUAL(1, INFINITE, 75), 0},
#endif
        {&WRITE_STD(0, 256, 0), 0},
        {&ERASE_CHIP(0, SPI_IF_ERASE_CHIP, 0),
            &ERASE_SECTOR(0, SPI_IF_ERASE_SECTOR, 0),
                &ERASE_SECTOR_4K(0, SPI_IF_ERASE_4K, 0),0}},
    {"W25Q16CV",  {0xef, 0x40, 0x15}, 3, FLASH_PROTECT_SUPPORT_TYPE_ONE, (_64K * 32), _4K, 3,
#ifdef SUPPORT_EQUIP_TEST
        {&READ_STD(0, INFINITE, 40), 0},
#else
        {&READ_DUAL(1, INFINITE, 75),0},
#endif
        {&WRITE_STD(0, 256, 80),0},
        {&ERASE_CHIP(0, SPI_IF_ERASE_CHIP, 0),
            &ERASE_SECTOR(0, SPI_IF_ERASE_SECTOR, 0),
                &ERASE_SECTOR_4K(0, SPI_IF_ERASE_4K, 0),0}},
    {"w25Q32BV",  {0xef, 0x40, 0x16}, 3, 0, (_64K * 64), _4K, 3,
#ifdef SUPPORT_EQUIP_TEST
        {&READ_STD(0, INFINITE, 40), 0},
#else
        {&READ_DUAL(1, INFINITE, 75), 0},
#endif
        {&WRITE_STD(0, 256, 0), 0},
        {&ERASE_CHIP(0, SPI_IF_ERASE_CHIP, 0),
            &ERASE_SECTOR(0, SPI_IF_ERASE_SECTOR, 0),
                &ERASE_SECTOR_4K(0, SPI_IF_ERASE_4K, 0),0}},
    {"w25Q64",  {0xef, 0x40, 0x17}, 3, 0, _8M, _4K, 3,
#ifdef SUPPORT_EQUIP_TEST
        {&READ_STD(0, INFINITE, 40), 0},
#else
        {&READ_DUAL(1, INFINITE, 75), 0},
#endif
        {&WRITE_STD(0, 256, 0), 0},
        {&ERASE_CHIP(0, SPI_IF_ERASE_CHIP, 0),
            &ERASE_SECTOR(0, SPI_IF_ERASE_SECTOR, 0),
                &ERASE_SECTOR_4K(0, SPI_IF_ERASE_4K, 0),0}},
    {"W25Q128FV", {0xEF, 0x40, 0x18}, 3, 0, _16M, _4K, 3,
#ifdef SUPPORT_EQUIP_TEST
        {&READ_STD(0, INFINITE, 40), 0},
#else
        {&READ_DUAL(1, INFINITE, 75), 0},
#endif
        {&WRITE_STD(0, 256, 0), 0},
        {&ERASE_CHIP(0, SPI_IF_ERASE_CHIP, 0),
            &ERASE_SECTOR(0, SPI_IF_ERASE_SECTOR, 0),
                &ERASE_SECTOR_4K(0, SPI_IF_ERASE_4K, 0),0}},
#endif /*FLASH_Winbond*/
#ifdef FLASH_Spansion
    /* Spansion */
    {"S25FL128P-0", {0x01, 0x20, 0x18, 0x03, 0x00}, 5, 0, (_256K * 64),  _256K, 3,
        {&READ_STD(0, INFINITE, 40), &READ_FAST(1, INFINITE, 104), 0},
        {&WRITE_STD(0, 256, 104), 0},
        {&ERASE_CHIP(0, SPI_IF_ERASE_CHIP, 104), &ERASE_SECTOR(0, SPI_IF_ERASE_SECTOR, 104), 0}},

    {"S25FL128P-1", {0x01, 0x20, 0x18, 0x03, 0x01}, 5, 0, (_64K * 256),  _64K, 3,
        {&READ_STD(0, INFINITE, 40), &READ_FAST(1, INFINITE, 104), 0},
        {&WRITE_STD(0, 256, 104), 0},
        {&ERASE_CHIP(0, SPI_IF_ERASE_CHIP, 104), &ERASE_SECTOR(0, SPI_IF_ERASE_SECTOR, 104), 0}},
#endif
	{"FM25Q08", {0xA1, 0x40, 0x14}, 3, 0, _8M, _4K, 3,
#if defined(PRODUCT_CFG_BOARD_TYPE_ASIC)&& !defined(PRODUCT_CFG_EQUIP_TEST_MODE)
			{&READ_DUAL(1, INFINITE, 75), 0},
#else
			{&READ_STD(0, INFINITE, 40), 0},
#endif
			{&WRITE_STD(0, 256, 0),0},
				{&ERASE_SECTOR(0, SPI_IF_ERASE_SECTOR, 0),
					&ERASE_SECTOR_4K(0, SPI_IF_ERASE_4K, 0),0}},
		 {"FM25Q16", {0xA1, 0x40, 0x15}, 3, 0,_16M, _4K, 3,
#if defined(PRODUCT_CFG_BOARD_TYPE_ASIC)&& !defined(PRODUCT_CFG_EQUIP_TEST_MODE)
			{&READ_DUAL(1, INFINITE, 75), 0},
#else
			{&READ_STD(0, INFINITE, 40), 0},
#endif
			{&WRITE_STD(0, 256, 0),0},
				{&ERASE_SECTOR(0, SPI_IF_ERASE_SECTOR, 0),
					&ERASE_SECTOR_4K(0, SPI_IF_ERASE_4K, 0),0}},
	{"GD25Q80C", {0xC8, 0x40, 0x14}, 3, 0, _8M, _4K, 3,
#if defined(PRODUCT_CFG_BOARD_TYPE_ASIC)&& !defined(PRODUCT_CFG_EQUIP_TEST_MODE)
			{&READ_DUAL(1, INFINITE, 75), 0},
#else
			{&READ_STD(0, INFINITE, 40), 0},
#endif
			{&WRITE_STD(0, 256, 0),0},
				{&ERASE_SECTOR(0, SPI_IF_ERASE_SECTOR, 0),
					&ERASE_SECTOR_4K(0, SPI_IF_ERASE_4K, 0),0}},
		 {"GD25Q16C", {0xC8, 0x40, 0x15}, 3, 0, _16M, _4K, 3,
#if defined(PRODUCT_CFG_BOARD_TYPE_ASIC)&& !defined(PRODUCT_CFG_EQUIP_TEST_MODE)
			{&READ_DUAL(1, INFINITE, 75), 0},
#else
			{&READ_STD(0, INFINITE, 40), 0},
#endif
			{&WRITE_STD(0, 256, 0),0},
				{&ERASE_SECTOR(0, SPI_IF_ERASE_SECTOR, 0),
					&ERASE_SECTOR_4K(0, SPI_IF_ERASE_4K, 0),0}},
	{"25LP080D", {0x9D, 0x60, 0x14}, 3, 0, _8M, _4K, 3,
#if defined(PRODUCT_CFG_BOARD_TYPE_ASIC)&& !defined(PRODUCT_CFG_EQUIP_TEST_MODE)
			{&READ_DUAL(1, INFINITE, 75), 0},
#else
			{&READ_STD(0, INFINITE, 40), 0},
#endif
			{&WRITE_STD(0, 256, 0),0},
				{&ERASE_SECTOR(0, SPI_IF_ERASE_SECTOR, 0),
					&ERASE_SECTOR_4K(0, SPI_IF_ERASE_4K, 0),0}},
		 {"25lp016d", {0x9D, 0x60, 0x15}, 3, 0, _16M, _4K, 3,
#if defined(PRODUCT_CFG_BOARD_TYPE_ASIC)&& !defined(PRODUCT_CFG_EQUIP_TEST_MODE)
			{&READ_DUAL(1, INFINITE, 75), 0},
#else
			{&READ_STD(0, INFINITE, 40), 0},
#endif
			{&WRITE_STD(0, 256, 0),0},
				{&ERASE_SECTOR(0, SPI_IF_ERASE_SECTOR, 0),
					&ERASE_SECTOR_4K(0, SPI_IF_ERASE_4K, 0),0}},
	{"EN25Q80B",  {0x1c, 0x30, 0x14}, 3, 0, _8M, _4K, 3,
#if defined(PRODUCT_CFG_BOARD_TYPE_ASIC)&& !defined(PRODUCT_CFG_EQUIP_TEST_MODE)
			{&READ_DUAL(1, INFINITE, 75), 0},
#else
			{&READ_STD(0, INFINITE, 40), 0},
#endif
			{&WRITE_STD(0, 256, 0),0},
				{&ERASE_SECTOR(0, SPI_IF_ERASE_SECTOR, 0),
					&ERASE_SECTOR_4K(0, SPI_IF_ERASE_4K, 0),0}},
		 {"EN25QH16A",  {0x1c, 0x70, 0x15}, 3, 0, _16M, _4K, 3,
#if defined(PRODUCT_CFG_BOARD_TYPE_ASIC)&& !defined(PRODUCT_CFG_EQUIP_TEST_MODE)
			{&READ_DUAL(1, INFINITE, 75), 0},
#else
			{&READ_STD(0, INFINITE, 40), 0},
#endif
			{&WRITE_STD(0, 256, 0),0},
				{&ERASE_SECTOR(0, SPI_IF_ERASE_SECTOR, 0),
					&ERASE_SECTOR_4K(0, SPI_IF_ERASE_4K, 0),0}},
					
    {"unknown",  {0x00, 0x00, 0x00}, 3, 0, (_64K * 16), _4K, 3,
        {&READ_STD(0, INFINITE, 40), 0},
        {&WRITE_STD(0, 256, 80),0},
        {&ERASE_CHIP(0, SPI_IF_ERASE_CHIP, 0),
            &ERASE_SECTOR(0, SPI_IF_ERASE_SECTOR, 0),
                &ERASE_SECTOR_4K(0, SPI_IF_ERASE_4K, 0),0}},

    {0},
};

struct spi_info *spi_serach_ids(unsigned char ids[8])
{
    struct spi_info *info;
    struct spi_info *fit_info = (struct spi_info *)HI_NULL;

    for (info = spi_info_table; info->name; info++)
    {
        if (memcmp(info->id, ids, info->id_len))
            continue;

        if (fit_info == NULL
            || fit_info->id_len < info->id_len)
        {
            fit_info = info;
        }
    }

    if (fit_info == NULL)
        fit_info = info - 1;

    return fit_info;
}
/*****************************************************************************/

void spi_search_rw(struct spi_info *spiinfo, struct spi_operation *spiop_rw,
    unsigned int iftype, unsigned int max_dummy, int is_read)
{
    int ix = 0;
    struct spi_operation **spiop, **fitspiop;

    HI_UNREF_PARAM(iftype);

    for (fitspiop = spiop = (is_read ? spiinfo->read : spiinfo->write);
        (*spiop) && ix < MAX_SPI_OP; spiop++, ix++)
    {
        DBG_MSG("dump[%d] %s iftype:0x%x\n", ix,
            (is_read ? "read" : "write"),
            (*spiop)->iftype);
/* Changed by wuxianfeng, 2012/6/6 for choose best iftype */
        //if (((*spiop)->iftype & iftype)
        //    && ((*spiop)->dummy <= max_dummy)
        if (((*spiop)->dummy <= max_dummy)
            && (*fitspiop)->iftype < (*spiop)->iftype)
        {
            fitspiop = spiop;
        }
    }
/*
        【屏蔽人】z00193780
        【审核人】w00190206
        【代码分析】boot下memcpy、memset、strcpy替换为安全接口会增加2k bytes左右的代码,并且以上接口性能分别会降低一定比例，所以建议boot下不做安全接口替换。
        【功能影响】不影响正常业务，屏蔽告警
*/
    memcpy_s(spiop_rw, sizeof(struct spi_operation), (*fitspiop), sizeof(struct spi_operation));
}
/*****************************************************************************/

void spi_get_erase(struct spi_info *spiinfo, struct spi_operation *spiop_erase,
    unsigned int *erasesize)
{
    int ix;

    (*erasesize) = spiinfo->erasesize;
    for (ix = 0; ix < MAX_SPI_OP; ix++)
    {
        if (spiinfo->erase[ix] == NULL)
            break;
/*
                    【屏蔽人】z00193780
                    【审核人】w00190206
                    【代码分析】boot下memcpy、memset、strcpy替换为安全接口会增加2k bytes左右的代码,并且以上接口性能分别会降低一定比例，所以建议boot下不做安全接口替换。
                    【功能影响】不影响正常业务，屏蔽告警
*/
        memcpy_s(&spiop_erase[ix], sizeof(struct spi_operation), spiinfo->erase[ix],
            sizeof(struct spi_operation));

        switch (spiop_erase[ix].size)
        {
        case SPI_IF_ERASE_4K:
            spiop_erase[ix].size = _4K;
            break;
        case SPI_IF_ERASE_SECTOR:
            spiop_erase[ix].size = _64K;
            break;
        case SPI_IF_ERASE_CHIP:
            spiop_erase[ix].size = spiinfo->chipsize;
            break;
        default:
            ;
        }
        DBG_MSG("erase[%d]->erasesize:0x%x, cmd:0x%x\n",
            ix, (spiop_erase[ix].size), spiop_erase[ix].cmd);

        if ((int)(spiop_erase[ix].size) < _2K)
        {
            DBG_BUG("erase block size mistaken: spi->erase[%d].size:0x%xB\n",
                ix, (spiop_erase[ix].size));
        }

        if (spiop_erase[ix].size < (*erasesize))
        {
            (*erasesize) = spiop_erase[ix].size;
        }
    }
}


