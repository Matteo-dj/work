/******************************************************************************
*    Copyright (c) 2009-2010 by czy.
*    All rights reserved.
* ***
*    Create by Cai ZhiYong. 2010-12-18
*	 Changed by Wu Xianfeng 2011.09.24
*
******************************************************************************/
#include "hi_types.h"
#include "hi_mdm_types.h"
#include "hi_platform.h"
#include "spi_ids.h"
#include "spifdrv.h"
#include <hi_stdlib.h>

#define SPI_DRV_VERSION       "0.9"
/*****************************************************************************/

#ifndef DRV_DEBUG
#define IDS_DBG_MSG(_fmt, arg...)
#else
#define IDS_DBG_MSG(_fmt, arg...)   \
    HI_UART_Printf("%s(%d): " _fmt, __FILE__, __LINE__, ##arg);
#endif

#define PAD 0
/*****************************************************************************/

#define SET_READ_STD(_dummy_, _size_, _clk_) \
    static SPI_OPERATION_S read_std_##_dummy_##_size_##_clk_ = { \
    SPI_IF_READ_STD, SPI_CMD_READ, _dummy_,PAD, _size_, _clk_ }

#define READ_STD(_dummy_, _size_, _clk_) read_std_##_dummy_##_size_##_clk_
//-----------------------------------------------------------------------------

#define SET_READ_FAST(_dummy_, _size_, _clk_) \
    static SPI_OPERATION_S read_fast_##_dummy_##_size_##_clk_ = { \
    SPI_IF_READ_FAST, SPI_CMD_FAST_READ, _dummy_,PAD, _size_, _clk_ }

#define READ_FAST(_dummy_, _size_, _clk_) read_fast_##_dummy_##_size_##_clk_
//-----------------------------------------------------------------------------

#define SET_READ_DUAL(_dummy_, _size_, _clk_) \
    static SPI_OPERATION_S read_dual_##_dummy_##_size_##_clk_ = { \
    SPI_IF_READ_DUAL, SPI_CMD_READ_DUAL, _dummy_,PAD, _size_, _clk_ }

#define READ_DUAL(_dummy_, _size_, _clk_) read_dual_##_dummy_##_size_##_clk_
//-----------------------------------------------------------------------------

#define SET_READ_QUAD(_dummy_, _size_, _clk_) \
    static SPI_OPERATION_S read_quad_##_dummy_##_size_##_clk_ = { \
    SPI_IF_READ_QUAD, SPI_CMD_READ_QUAD, _dummy_,PAD, _size_, _clk_ }

#define READ_QUAD(_dummy_, _size_, _clk_) read_quad_##_dummy_##_size_##_clk_
//-----------------------------------------------------------------------------

#define SET_READ_DUAL_ADDR(_dummy_, _size_, _clk_) \
    static SPI_OPERATION_S read_dual_addr_##_dummy_##_size_##_clk_ = { \
    SPI_IF_READ_DUAL_ADDR, SPI_CMD_READ_DUAL_ADDR, _dummy_,PAD, _size_, _clk_ }

#define READ_DUAL_ADDR(_dummy_, _size_, _clk_) read_dual_addr_##_dummy_##_size_##_clk_
//-----------------------------------------------------------------------------

#define SET_READ_QUAD_ADDR(_dummy_, _size_, _clk_) \
    static SPI_OPERATION_S read_quad_addr_##_dummy_##_size_##_clk_ = { \
    SPI_IF_READ_QUAD_ADDR, SPI_CMD_READ_QUAD_ADDR, _dummy_,PAD, _size_, _clk_ }

#define READ_QUAD_ADDR(_dummy_, _size_, _clk_) read_quad_addr_##_dummy_##_size_##_clk_
//-----------------------------------------------------------------------------

#define SET_WRITE_STD(_dummy_, _size_, _clk_) \
    static SPI_OPERATION_S write_std_##_dummy_##_size_##_clk_ = { \
    SPI_IF_WRITE_STD, SPI_CMD_PP, _dummy_,PAD, _size_, _clk_ }

#define WRITE_STD(_dummy_, _size_, _clk_) write_std_##_dummy_##_size_##_clk_
//-----------------------------------------------------------------------------

#define SET_WRITE_DUAL(_dummy_, _size_, _clk_) \
    static SPI_OPERATION_S write_dual_##_dummy_##_size_##_clk_ = { \
    SPI_IF_WRITE_DUAL, SPI_CMD_WRITE_DUAL, _dummy_,PAD, _size_, _clk_ }

#define WRITE_DUAL(_dummy_, _size_, _clk_) write_dual_##_dummy_##_size_##_clk_
//-----------------------------------------------------------------------------

#define SET_WRITE_QUAD(_dummy_, _size_, _clk_) \
    static SPI_OPERATION_S write_quad_##_dummy_##_size_##_clk_ = { \
    SPI_IF_WRITE_QUAD, SPI_CMD_WRITE_QUAD, _dummy_,PAD, _size_, _clk_ }

#define WRITE_QUAD(_dummy_, _size_, _clk_) write_quad_##_dummy_##_size_##_clk_
//-----------------------------------------------------------------------------

#define SET_WRITE_DUAL_ADDR(_dummy_, _size_, _clk_) \
    static SPI_OPERATION_S write_dual_addr_##_dummy_##_size_##_clk_ = { \
    SPI_IF_WRITE_DUAL_ADDR, SPI_CMD_WRITE_DUAL_ADDR, _dummy_,PAD, _size_, _clk_ }

#define WRITE_DUAL_ADDR(_dummy_, _size_, _clk_) write_dual_addr_##_dummy_##_size_##_clk_
//-----------------------------------------------------------------------------

#define SET_WRITE_QUAD_ADDR(_dummy_, _size_, _clk_) \
    static SPI_OPERATION_S write_quad_addr_##_dummy_##_size_##_clk_ = { \
    SPI_IF_WRITE_QUAD_ADDR, SPI_CMD_WRITE_QUAD_ADDR, _dummy_,PAD, _size_, _clk_ }

#define WRITE_QUAD_ADDR(_dummy_, _size_, _clk_) write_quad_addr_##_dummy_##_size_##_clk_
//-----------------------------------------------------------------------------

#define SET_ERASE_SECTOR(_dummy_, _size_, _clk_) \
    static SPI_OPERATION_S erase_sector_##_dummy_##_size_##_clk_ = { \
    SPI_IF_ERASE_SECTOR, SPI_CMD_SE, _dummy_,PAD, _size_, _clk_ }

#define ERASE_SECTOR(_dummy_, _size_, _clk_) erase_sector_##_dummy_##_size_##_clk_
//-----------------------------------------------------------------------------

#define SET_ERASE_CHIP(_dummy_, _size_, _clk_) \
    static SPI_OPERATION_S erase_chip_##_dummy_##_size_##_clk_ = { \
    SPI_IF_ERASE_CHIP, SPI_CMD_BE, _dummy_,PAD, _size_, _clk_ }

#define ERASE_CHIP(_dummy_, _size_, _clk_) erase_chip_##_dummy_##_size_##_clk_
//-------------------------------------------------

#define SET_ERASE_SECTOR_4K(_dummy_, _size_, _clk_) \
		static SPI_OPERATION_S erase_sector_4k_##_dummy_##_size_##_clk_ = { \
		SPI_IF_ERASE_4K, SPI_CMD_SE_4K, _dummy_,PAD, _size_, _clk_ }
	
#define ERASE_SECTOR_4K(_dummy_, _size_, _clk_) erase_sector_4k_##_dummy_##_size_##_clk_


/*****************************************************************************/
SET_READ_STD(0, SPI_INFINITE, 40);
SET_READ_FAST(1, SPI_INFINITE, 86);

SET_READ_DUAL(1, SPI_INFINITE, 75);
//-----------------------------------------------------------------------------
SET_WRITE_STD(0, 256, 0);
//-----------------------------------------------------------------------------

SET_ERASE_SECTOR(0, SPI_IF_ERASE_SECTOR, 75);
SET_ERASE_SECTOR_4K(0, SPI_IF_ERASE_4K, 104);

/* Compatibility with 6 flash */
/* MX25L1605A MX25L8005 MX25l3205d
** W25Q16CV   W25Q80BV  W25Q32BV 
*/
HI_PRV SPI_INFO_S s_astSpiInfoTable[] =
{
    /* name        id                id_len chipsize(Bytes) erasesize */
    {"pclint",  {0x55, 0x55, 0x55}, 3, 0, _1M,   _4K, 3, 
        {&READ_DUAL(1, SPI_INFINITE, 75),&READ_FAST(1, SPI_INFINITE, 86),
            &READ_STD(0, SPI_INFINITE, 40),0},  
            {&WRITE_STD(0, 256, 0), 0}, 
                {&ERASE_SECTOR(0, SPI_IF_ERASE_SECTOR, 75),
                    &ERASE_SECTOR_4K(0, SPI_IF_ERASE_4K, 104), 0}},
    {"EN25F80",  {0x1c, 0x31, 0x14}, 3,  0, _1M,   _4K, 3, 
#if defined(PRODUCT_CFG_BOARD_TYPE_ASIC)&& !defined(PRODUCT_CFG_EQUIP_TEST_MODE)
        {&READ_FAST(1, SPI_INFINITE, 86), 0}, 
#else
        {&READ_STD(0, SPI_INFINITE, 40), 0}, 
#endif
            {&WRITE_STD(0, 256, 0), 0}, 
                {&ERASE_SECTOR(0, SPI_IF_ERASE_SECTOR, 75),
                    &ERASE_SECTOR_4K(0, SPI_IF_ERASE_4K, 104), 0}},
    {"EN25QH16",  {0x1c, 0x70, 0x15}, 3,  0, _2M,   _4K, 3, 
#if defined(PRODUCT_CFG_BOARD_TYPE_ASIC)&& !defined(PRODUCT_CFG_EQUIP_TEST_MODE)
        {&READ_DUAL(1, SPI_INFINITE, 75), 0}, 
#else
        {&READ_STD(0, SPI_INFINITE, 40), 0}, 
#endif
            {&WRITE_STD(0, 256, 0), 0}, 
                {&ERASE_SECTOR(0, SPI_IF_ERASE_SECTOR, 75),
                    &ERASE_SECTOR_4K(0, SPI_IF_ERASE_4K, 104), 0}},
    
    /* Macronix */
    {"MX25L8005",  {0xc2, 0x20, 0x14}, 3,  0, _1M,    _4K, 3,
#if defined(PRODUCT_CFG_BOARD_TYPE_ASIC)&& !defined(PRODUCT_CFG_EQUIP_TEST_MODE)
            {&READ_DUAL(1, SPI_INFINITE, 75), 0}, 
#else
            {&READ_STD(0, SPI_INFINITE, 40), 0}, 
#endif
            {&WRITE_STD(0, 256, 0), 0}, 
                {&ERASE_SECTOR(0, SPI_IF_ERASE_SECTOR, 75),
                    &ERASE_SECTOR_4K(0, SPI_IF_ERASE_4K, 104), 0}},
    {"MX25l3205d",  {0xc2, 0x20, 0x16}, 3,  0, _4M,    _4K, 3, 
#if defined(PRODUCT_CFG_BOARD_TYPE_ASIC)&& !defined(PRODUCT_CFG_EQUIP_TEST_MODE)
            {&READ_DUAL(1, SPI_INFINITE, 75), 0}, 
#else
            {&READ_STD(0, SPI_INFINITE, 40), 0}, 
#endif
            {&WRITE_STD(0, 256, 0), 0}, 
                {&ERASE_SECTOR(0, SPI_IF_ERASE_SECTOR, 75),
                    &ERASE_SECTOR_4K(0, SPI_IF_ERASE_4K, 104), 0}},
    {"MX25L1605A",  {0xc2, 0x20, 0x15}, 3, 0,  _2M,    _4K, 3, 
#if defined(PRODUCT_CFG_BOARD_TYPE_ASIC)&& !defined(PRODUCT_CFG_EQUIP_TEST_MODE)
            {&READ_DUAL(1, SPI_INFINITE, 75), 0}, 
#else
            {&READ_STD(0, SPI_INFINITE, 40), 0}, 
#endif
            {&WRITE_STD(0, 256, 0), 0}, 
                {&ERASE_SECTOR(0, SPI_IF_ERASE_SECTOR, 75),
                    &ERASE_SECTOR_4K(0, SPI_IF_ERASE_4K, 104), 0}},
    {"MX25L6406E",  {0xc2, 0x20, 0x17}, 3,  0, _8M,    _4K, 3,
#if defined(PRODUCT_CFG_BOARD_TYPE_ASIC)&& !defined(PRODUCT_CFG_EQUIP_TEST_MODE)
            {&READ_DUAL(1, SPI_INFINITE, 75), 0}, 
#else
            {&READ_STD(0, SPI_INFINITE, 40), 0}, 
#endif
            {&WRITE_STD(0, 256, 0), 0}, 
                {&ERASE_SECTOR(0, SPI_IF_ERASE_SECTOR, 75),
                    &ERASE_SECTOR_4K(0, SPI_IF_ERASE_4K, 104), 0}},

    {"W25Q80BV",  {0xef, 0x40, 0x14}, 3, FLASH_PROTECT_SUPPORT_TYPE_ONE, (_64K * 16),   _4K, 3, 
#if defined(PRODUCT_CFG_BOARD_TYPE_ASIC)&& !defined(PRODUCT_CFG_EQUIP_TEST_MODE)
        {&READ_DUAL(1, SPI_INFINITE, 75), 0}, 
#else
        {&READ_STD(0, SPI_INFINITE, 40), 0}, 
#endif 
            {&WRITE_STD(0, 256, 0), 0}, 
                {&ERASE_SECTOR(0, SPI_IF_ERASE_SECTOR, 75),
                    &ERASE_SECTOR_4K(0, SPI_IF_ERASE_4K, 104), 0}},
        
    {"W25Q16CV",  {0xef, 0x40, 0x15}, 3, FLASH_PROTECT_SUPPORT_TYPE_ONE,  (_64K * 32),   _4K, 3, 
#if defined(PRODUCT_CFG_BOARD_TYPE_ASIC)&& !defined(PRODUCT_CFG_EQUIP_TEST_MODE)
        {&READ_DUAL(1, SPI_INFINITE, 75), 0}, 
#else
        {&READ_STD(0, SPI_INFINITE, 40), 0}, 
#endif 
            {&WRITE_STD(0, 256, 0), 0},
                {&ERASE_SECTOR(0, SPI_IF_ERASE_SECTOR, 75),
                    &ERASE_SECTOR_4K(0, SPI_IF_ERASE_4K, 104), 0}},
    {"W25Q32BV",  {0xef, 0x40, 0x16}, 3, 0,  (_64K * 64),   _4K, 3, 
#if defined(PRODUCT_CFG_BOARD_TYPE_ASIC)&& !defined(PRODUCT_CFG_EQUIP_TEST_MODE)
        {&READ_DUAL(1, SPI_INFINITE, 75), 0}, 
#else
        {&READ_STD(0, SPI_INFINITE, 40), 0}, 
#endif 
            {&WRITE_STD(0, 256, 0), 0},
                {&ERASE_SECTOR(0, SPI_IF_ERASE_SECTOR, 75),
                    &ERASE_SECTOR_4K(0, SPI_IF_ERASE_4K, 104), 0}},
    {"W25Q64",  {0xef, 0x40, 0x17}, 3, 0,  _8M,   _4K, 3, 
#if defined(PRODUCT_CFG_BOARD_TYPE_ASIC)&& !defined(PRODUCT_CFG_EQUIP_TEST_MODE)
        {&READ_DUAL(1, SPI_INFINITE, 75), 0}, 
#else
        {&READ_STD(0, SPI_INFINITE, 40), 0}, 
#endif 
            {&WRITE_STD(0, 256, 0), 0},
                {&ERASE_SECTOR(0, SPI_IF_ERASE_SECTOR, 75),
                    &ERASE_SECTOR_4K(0, SPI_IF_ERASE_4K, 104), 0}},            
    {"W25Q128FV", {0xEF, 0x40, 0x18}, 3, 0,  _16M, _4K, 3, 
#if defined(PRODUCT_CFG_BOARD_TYPE_ASIC)&& !defined(PRODUCT_CFG_EQUIP_TEST_MODE)
        {&READ_DUAL(1, SPI_INFINITE, 75), 0}, 
#else
        {&READ_STD(0, SPI_INFINITE, 40), 0}, 
#endif 
            {&WRITE_STD(0, 256, 0), 0}, 
                {&ERASE_SECTOR(0, SPI_IF_ERASE_SECTOR, 75),
                    &ERASE_SECTOR_4K(0, SPI_IF_ERASE_4K, 104), 0}},
    {"W25Q128FW", {0xEF, 0x60, 0x18}, 3, 0,  _16M, _4K, 3, 
#if defined(PRODUCT_CFG_BOARD_TYPE_ASIC)&& !defined(PRODUCT_CFG_EQUIP_TEST_MODE)
        {&READ_DUAL(1, SPI_INFINITE, 75), 0}, 
#else
        {&READ_STD(0, SPI_INFINITE, 40), 0}, 
#endif 
            {&WRITE_STD(0, 256, 0), 0}, 
                {&ERASE_SECTOR(0, SPI_IF_ERASE_SECTOR, 75),
                    &ERASE_SECTOR_4K(0, SPI_IF_ERASE_4K, 104), 0}},
    {"MX25U25635F", {0xc2, 0x25, 0x39}, 3, 0,  _32M, _4K, 4,
#if defined(PRODUCT_CFG_BOARD_TYPE_ASIC)&& !defined(PRODUCT_CFG_EQUIP_TEST_MODE)
        {&READ_DUAL(1, SPI_INFINITE, 75), 0},
#else
		{&READ_STD(0, SPI_INFINITE, 40), 0},
#endif
        {&WRITE_STD(0, 256, 0),0},
            {&ERASE_SECTOR(0, SPI_IF_ERASE_SECTOR, 75),
                &ERASE_SECTOR_4K(0, SPI_IF_ERASE_4K, 104),0}},            
	{"FM25Q08", {0xA1, 0x40, 0x14}, 3, 0, _8M, _4K, 3,
#if defined(PRODUCT_CFG_BOARD_TYPE_ASIC)&& !defined(PRODUCT_CFG_EQUIP_TEST_MODE)
        {&READ_DUAL(1, SPI_INFINITE, 75), 0},
#else
		{&READ_STD(0, SPI_INFINITE, 40), 0},
#endif
        {&WRITE_STD(0, 256, 0),0},
            {&ERASE_SECTOR(0, SPI_IF_ERASE_SECTOR, 75),
                &ERASE_SECTOR_4K(0, SPI_IF_ERASE_4K, 104),0}},
     {"FM25Q16", {0xA1, 0x40, 0x15}, 3, 0, _16M, _4K, 3,
#if defined(PRODUCT_CFG_BOARD_TYPE_ASIC)&& !defined(PRODUCT_CFG_EQUIP_TEST_MODE)
        {&READ_DUAL(1, SPI_INFINITE, 75), 0},
#else
		{&READ_STD(0, SPI_INFINITE, 40), 0},
#endif
        {&WRITE_STD(0, 256, 0),0},
            {&ERASE_SECTOR(0, SPI_IF_ERASE_SECTOR, 75),
                &ERASE_SECTOR_4K(0, SPI_IF_ERASE_4K, 104),0}},
	 {"GD25Q80C", {0xC8, 0x40, 0x14}, 3, 0, _8M, _4K, 3,
#if defined(PRODUCT_CFG_BOARD_TYPE_ASIC)&& !defined(PRODUCT_CFG_EQUIP_TEST_MODE)
        {&READ_DUAL(1, SPI_INFINITE, 75), 0},
#else
		{&READ_STD(0, SPI_INFINITE, 40), 0},
#endif
        {&WRITE_STD(0, 256, 0),0},
            {&ERASE_SECTOR(0, SPI_IF_ERASE_SECTOR, 75),
                &ERASE_SECTOR_4K(0, SPI_IF_ERASE_4K, 104),0}},
     {"GD25Q16C", {0xC8, 0x40, 0x15}, 3, 0, _16M, _4K, 3,
#if defined(PRODUCT_CFG_BOARD_TYPE_ASIC)&& !defined(PRODUCT_CFG_EQUIP_TEST_MODE)
        {&READ_DUAL(1, SPI_INFINITE, 75), 0},
#else
		{&READ_STD(0, SPI_INFINITE, 40), 0},
#endif
        {&WRITE_STD(0, 256, 0),0},
            {&ERASE_SECTOR(0, SPI_IF_ERASE_SECTOR, 75),
                &ERASE_SECTOR_4K(0, SPI_IF_ERASE_4K, 104),0}},

	{"25LP080D", {0x9D, 0x60, 0x14}, 3, 0,_8M, _4K, 3,
#if defined(PRODUCT_CFG_BOARD_TYPE_ASIC)&& !defined(PRODUCT_CFG_EQUIP_TEST_MODE)
			{&READ_DUAL(1, SPI_INFINITE, 75), 0},
#else
			{&READ_STD(0, SPI_INFINITE, 40), 0},
#endif
			{&WRITE_STD(0, 256, 0),0},
				{&ERASE_SECTOR(0, SPI_IF_ERASE_SECTOR, 75),
					&ERASE_SECTOR_4K(0, SPI_IF_ERASE_4K, 104),0}},
		 {"25lp016d", {0x9D, 0x60, 0x15}, 3, 0, _16M, _4K, 3,
#if defined(PRODUCT_CFG_BOARD_TYPE_ASIC)&& !defined(PRODUCT_CFG_EQUIP_TEST_MODE)
			{&READ_DUAL(1, SPI_INFINITE, 75), 0},
#else
			{&READ_STD(0, SPI_INFINITE, 40), 0},
#endif
			{&WRITE_STD(0, 256, 0),0},
				{&ERASE_SECTOR(0, SPI_IF_ERASE_SECTOR, 75),
					&ERASE_SECTOR_4K(0, SPI_IF_ERASE_4K, 104),0}},

                
    {"EN25Q80B",  {0x1c, 0x30, 0x14}, 3, 0, _1M,   _4K, 3, 
#if defined(PRODUCT_CFG_BOARD_TYPE_ASIC)&& !defined(PRODUCT_CFG_EQUIP_TEST_MODE)
        {&READ_DUAL(1, SPI_INFINITE, 75), 0}, 
#else
        {&READ_STD(0, SPI_INFINITE, 40), 0}, 
#endif
            {&WRITE_STD(0, 256, 0), 0}, 
                {&ERASE_SECTOR(0, SPI_IF_ERASE_SECTOR, 75),
                    &ERASE_SECTOR_4K(0, SPI_IF_ERASE_4K, 104), 0}},
    {"EN25QH16A",  {0x1c, 0x70, 0x15}, 3, 0, _2M,   _4K, 3, 
#if defined(PRODUCT_CFG_BOARD_TYPE_ASIC)&& !defined(PRODUCT_CFG_EQUIP_TEST_MODE)
        {&READ_DUAL(1, SPI_INFINITE, 75), 0}, 
#else
        {&READ_STD(0, SPI_INFINITE, 40), 0}, 
#endif
            {&WRITE_STD(0, 256, 0), 0}, 
                {&ERASE_SECTOR(0, SPI_IF_ERASE_SECTOR, 75),
                    &ERASE_SECTOR_4K(0, SPI_IF_ERASE_4K, 104), 0}},
								
    {"unknown",  {0x00, 0x00, 0x00}, 3, 0, (_1M), _4K, 3, 
        {&READ_STD(0, SPI_INFINITE, 40), 0}, 
            {&WRITE_STD(0, 256, 0), 0}, 
                {&ERASE_SECTOR(0, SPI_IF_ERASE_SECTOR, 75),
                    &ERASE_SECTOR_4K(0, SPI_IF_ERASE_4K, 104), 0}},

    {0},
};




/*****************************************************************************
* spi_serach_ids
* 功能:比对ID
*
*********************************************************************************/
SPI_INFO_S *spi_serach_ids(HI_U8 aucIds[8])
{
	SPI_INFO_S *pstInfo = HI_NULL;
	SPI_INFO_S *pstFitInfo = HI_NULL;

	for (pstInfo = s_astSpiInfoTable; pstInfo->pName; pstInfo++)
	{
		if(memcmp((HI_CHAR *)pstInfo->aulId, (HI_CHAR *)aucIds, pstInfo->usIdLen))
		{
			continue;
		}
		
		if ((pstFitInfo == HI_NULL) || (pstFitInfo->usIdLen < pstInfo->usIdLen))
		{
			pstFitInfo = pstInfo;
		}
	}
	if(pstFitInfo == HI_NULL)
	{
		pstFitInfo = pstInfo-1;
	}
	
	return pstFitInfo;
}
/*****************************************************************************
* spi_search_rw
* 功能: 将info信息的readwrite对应到spi_operation 中
*
*************************************************************************/
HI_VOID spi_search_rw(SPI_INFO_S *stSpiInfo, SPI_OPERATION_S *stSpiOpRw,
    HI_U32 ulIfType, HI_U32 ulMaxDummy, HI_BOOL bIsRead)
{
    HI_S32 i = 0;
    SPI_OPERATION_S **pstSpiOp = HI_NULL;
    SPI_OPERATION_S **pstFitSpiOp = HI_NULL;

    for (pstFitSpiOp = pstSpiOp = (bIsRead ? stSpiInfo->pstRead: stSpiInfo->pstWrite);
        (*pstSpiOp) && i < MAX_SPI_OP; pstSpiOp++, i++)
    {
        IDS_DBG_MSG("dump[%d] %s iftype:0x%02X\r\n", i, (bIsRead ? "read" : "write"), (*pstSpiOp)->ucIfType);

        if (((*pstSpiOp)->ucIfType & ulIfType)
            && ((*pstSpiOp)->ucDummy <= ulMaxDummy)
            && (*pstFitSpiOp)->ucIfType < (*pstSpiOp)->ucIfType)
        {
            pstFitSpiOp = pstSpiOp; //取较大的iftype              
        }
    }

    (hi_void)memcpy_s(stSpiOpRw, sizeof(SPI_OPERATION_S), (*pstFitSpiOp), sizeof(SPI_OPERATION_S));
}
/*****************************************************************************
* spi_get_erase
* 功能: 
* 参数:
*		
*
***************************************************************************/
HI_VOID spi_get_erase(SPI_INFO_S *stSpiInfo, SPI_OPERATION_S *stSpiOp, HI_U32 *pEraseSize)
{
    HI_S32 i = 0;

    (*pEraseSize) = stSpiInfo->ulEraseSize;
    for (i = 0; i < MAX_SPI_OP; i++)
    {
        if (stSpiInfo->pstErase[i] == HI_NULL)
        {
            break;
        }

        (hi_void)memcpy_s(&stSpiOp[i], sizeof(SPI_OPERATION_S), stSpiInfo->pstErase[i], sizeof(SPI_OPERATION_S));

        switch (stSpiOp[i].ulSize)
        {
        case SPI_IF_ERASE_SECTOR:
            stSpiOp[i].ulSize = _64K;
            break;
        case SPI_IF_ERASE_CHIP:
            stSpiOp[i].ulSize = stSpiInfo->ulChipSize;
            break;
		case SPI_IF_ERASE_4K:
			stSpiOp[i].ulSize = _4K;
			break;
		default:
			break;
        }
        
		IDS_DBG_MSG("erase[%d]->erasesize:%u, cmd:0x%02X\n",i, stSpiOp[i].ulSize, stSpiOp[i].ucCmd);

        if ((HI_S32)(stSpiOp[i].ulSize) < _2K)
        {
			DBG_BUG("erase block size mistaken: spi->erase[%d].size:%u\n",i, stSpiOp[i].ulSize);
        }

        if (stSpiOp[i].ulSize < (*pEraseSize))
        {
            (*pEraseSize) = stSpiOp[i].ulSize;
        }
        
    }
}

