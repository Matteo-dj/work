/*----------------------------------------------------------------------------
 * Copyright (c) <2013-2015>, <Huawei Technologies Co., Ltd>
 * All rights reserved.
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 * 1. Redistributions of source code must retain the above copyright notice, this list of
 * conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice, this list
 * of conditions and the following disclaimer in the documentation and/or other materials
 * provided with the distribution.
 * 3. Neither the name of the copyright holder nor the names of its contributors may be used
 * to endorse or promote products derived from this software without specific prior written
 * permission.
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
 * ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *---------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------
 * Notice of Export Control Law
 * ===============================================
 * Huawei LiteOS may be subject to applicable export control laws and regulations, which might
 * include those applicable to Huawei LiteOS of U.S. and the country in which you are located.
 * Import, export and usage of Huawei LiteOS in any manner by you shall be in compliance with such
 * applicable export control laws and regulations.
 *---------------------------------------------------------------------------*/

#include "mmu_config.h"
#ifdef LOSCFG_LIB_LIBC
#include "string.h"
#endif /* LOSCFG_LIB_LIBC */
#include "los_printf.h"
#include "asm/dma.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

#define UNCACHEABLE                         0
#define CACHEABLE                           1
#define UNBUFFERABLE                        0
#define BUFFERABLE                          1
#define ACCESS_RW                   3
#define ACCESS_RO                   0

#define MMU_AP_STATE(flag)  (((flag) & 0x1)?ACCESS_RW:ACCESS_RO)
#define MMU_CACHE_STATE(flag)  (((flag) >> 1) & 0x1)
#define MMU_BUFFER_STATE(flag) (((flag) >> 2) & 0x1)
#define MMU_GET_AREA(flag) ((flag)&(0x1<<3))


extern void *memset(void *dst, int c, size_t n);


SENCOND_PAGE stOsPage = {0};

static void set_mem_page(MMU_PARAM *mPara)
{
    UINT32 page_start_index;
    UINT32 page_end_index;
    UINT32 length, uwCache, uwBuf, uwAP;
    UINT32 page_size = (mPara->stPage->page_type == MMU_SECOND_LEVEL_BIG_PAGE_TABLE_ID) ? MMU_64K : MMU_4K;
    UINT32 stt_base = mPara->stPage->page_descriptor_addr;

    if ((mPara->startAddr % page_size) != 0)
    {
        return;
    }

    if ((mPara->endAddr % page_size) != 0)
    {
        mPara->endAddr = (mPara->endAddr + (page_size -1)) & ~ (page_size - 1);
    }

    uwAP = MMU_AP_STATE(mPara->uwFlag);
    uwCache = MMU_CACHE_STATE(mPara->uwFlag);
    uwBuf = MMU_BUFFER_STATE(mPara->uwFlag);

    page_start_index = (mPara->startAddr - mPara->stPage->page_addr)/page_size;
    page_end_index = (mPara->endAddr - mPara->stPage->page_addr)/page_size;
    length = page_end_index - page_start_index;

    if (uwAP == ACCESS_RW)
    {
        if (page_size == MMU_64K)
        {
            X_MMU_TWO_LEVEL_PAGE64K_RW((mPara->startAddr/page_size), page_start_index, length, uwCache, uwBuf, ACCESS_RW); /*lint !e440 !e442 !e443*/
        }
        else
        {
            X_MMU_TWO_LEVEL_PAGE_RW((mPara->startAddr/page_size), page_start_index, length, uwCache, uwBuf, ACCESS_RW);
        }
    }
    else
    {
        if (page_size == MMU_64K)
        {
            X_MMU_TWO_LEVEL_PAGE64K_RO((mPara->startAddr/page_size), page_start_index, length, uwCache, uwBuf, ACCESS_RW); /*lint !e440 !e442 !e443*/
        }
        else
        {
            X_MMU_TWO_LEVEL_PAGE_RO((mPara->startAddr/page_size), page_start_index, length, uwCache, uwBuf, ACCESS_RW);
        }
    }
}

VOID LOS_SecPageEnable(SENCOND_PAGE *stPage, UINT32 uwFlag)
{
    UINT32 uwPageStart;
    UINT32 uwPageEnd;
    UINT32 ttb_base = FIRST_PAGE_DESCRIPTOR_ADDR;
    MMU_PARAM mPara;
    mPara.startAddr = stPage->page_addr;
    mPara.endAddr = stPage->page_addr + stPage->page_length;
    mPara.uwFlag = uwFlag;
    mPara.stPage = stPage;

    uwPageStart = MMU_GET_SECOND_TABLE_ADDR(mPara.startAddr);
    uwPageEnd = MMU_GET_SECOND_TABLE_ADDR(mPara.endAddr - 1);
    set_mem_page(&mPara); /*lint !e522 */
    
    dma_cache_clean((int)uwPageStart, (int)uwPageEnd);
    X_MMU_ONE_LEVEL_PAGE((stPage->page_descriptor_addr/MMU_1K), (stPage->page_addr/MMU_1M), (stPage->page_length/MMU_1M)); /*lint !e681 */
    dma_cache_clean((int)(ttb_base + mPara.startAddr/MMU_1M * 4), (int)(ttb_base + mPara.endAddr/MMU_1M * 4));
    __asm volatile ( "mov    %0, #0\n" "mcr    p15, 0, %0, c8, c7, 0\n" : : "r" (0));
}

void LOS_MMUParamSet(MMU_PARAM *mPara)
{
    UINT32 uwPageStart;
    UINT32 uwPageEnd;
    UINT32 uwTtb = 0;
    UINT32 uwItemStart = MMU_GET_FIRST_TABLE_ADDR(mPara->startAddr);
    UINT32 uwItemEnd = MMU_GET_FIRST_TABLE_ADDR(mPara->endAddr - 1);
    UINT32 uwItemTemp = uwItemStart;
    UINT32 uwCache, uwBuf, uwAP,uwTableType;

    if (uwItemStart > uwItemEnd)
    {
        PRINT_ERR("wrong addr input\n");
        return;
    }

    uwAP = MMU_AP_STATE(mPara->uwFlag);
    uwCache = MMU_CACHE_STATE(mPara->uwFlag);
    uwBuf = MMU_BUFFER_STATE(mPara->uwFlag);
    uwTableType = MMU_GET_AREA(mPara->uwFlag);

    if (uwTableType == SECOND_PAGE)
    {
        if (mPara->startAddr < mPara->stPage->page_addr || mPara->endAddr > (mPara->stPage->page_length + mPara->stPage->page_addr))
        {
            PRINT_ERR("addr input not belongs to this second page\n");
            return;
        }
        while (uwItemTemp <= uwItemEnd)
        {
            if ((*(UINT32 *)uwItemTemp & 0x3) != MMU_FIRST_LEVEL_PAGE_TABLE_ID)
            {
                PRINT_ERR("not all mem belongs to second page(4K or 64K every item)\n");
                return;
            }
            uwItemTemp += 4;
        }

        set_mem_page(mPara); /*lint !e522*/
        uwPageStart = MMU_GET_SECOND_TABLE_ADDR(mPara->startAddr);
        uwPageEnd = MMU_GET_SECOND_TABLE_ADDR(mPara->endAddr - 1);
        dma_cache_clean((int)uwPageStart, (int)uwPageEnd);
    }
    else  if (uwTableType == FIRST_SECTION)
    {
        while (uwItemTemp <= uwItemEnd)
        {
            if ((*(UINT32 *)uwItemTemp & 0x3) != MMU_FIRST_LEVEL_SECTION_ID)
            {
                PRINT_ERR("not all mem belongs to first section(1M every item)\n");
                return;
            }
            uwItemTemp += 4;
        }

        uwItemTemp = uwItemStart;
        while (uwItemTemp <= uwItemEnd)
        {
            union MMU_FIRST_LEVEL_DESCRIPTOR desc;
            desc.word = (*(UINT32 *)uwItemTemp);
            desc.section.c = (uwCache);
            desc.section.b = (uwBuf);
            desc.section.ap = (uwAP);
            desc.section.domain = 1;
            (*(UINT32 *)uwItemTemp)= desc.word;
            uwItemTemp += 4;
        }
        dma_cache_clean((int)uwItemStart, (int)uwItemEnd);
    }

    __asm volatile ( "mov    %0, #0\n" "mcr    p15, 0, %0, c8, c7, 0\n" : : "r" (uwTtb));
}/*lint !e529 !e438*/

void hal_mmu_init(void) {
    UINT32 ttb_base = FIRST_PAGE_DESCRIPTOR_ADDR + 0x0;
    UINT32 uwReg = 0;

    // Set the TTB register
    __asm volatile ("mcr  p15,0,%0,c2,c0,0" : : "r"(ttb_base) /*:*/);

/*****************************************************************************************************
说明:存储器访问权限有两种方式记录:域控制和页表控制，
其中以域控制为主。有16种域的配置方式可选:
寄存器格式如下:
| 30     28    26    24    22    20  18    16    14    12   10     8     6     4     2      0|
-----------------------------------------------------------------------
|D15|D14|D13|D12|D11|D10|D09|D08|D07|D06|D05|D04|D03|D02|D01|D00|
-----------------------------------------------------------------------
-----------------------------------------------------------------------
meaning              value        discription
-----------------------------------------------------------------------
manager              11          access not checked
reserved             10          like no access.
client               01          access checked by page table
no access            00          any access generates a domain default
-----------------------------------------------------------------------
下图配置0域为管理域，1域为客户域。
*****************************************************************************************************/
    // Set the Domain Access Control Register
    uwReg = ACCESS_TYPE_MANAGER(0)    |
        ACCESS_TYPE_CLIENT(1)  |
        ACCESS_TYPE_NO_ACCESS(2)  |
        ACCESS_TYPE_NO_ACCESS(3)  |
        ACCESS_TYPE_NO_ACCESS(4)  |
        ACCESS_TYPE_NO_ACCESS(5)  |
        ACCESS_TYPE_NO_ACCESS(6)  |
        ACCESS_TYPE_NO_ACCESS(7)  |
        ACCESS_TYPE_NO_ACCESS(8)  |
        ACCESS_TYPE_NO_ACCESS(9)  |
        ACCESS_TYPE_NO_ACCESS(10) |
        ACCESS_TYPE_NO_ACCESS(11) |
        ACCESS_TYPE_NO_ACCESS(12) |
        ACCESS_TYPE_NO_ACCESS(13) |
        ACCESS_TYPE_NO_ACCESS(14) |
        ACCESS_TYPE_NO_ACCESS(15);
    __asm volatile ("mcr  p15,0,%0,c3,c0,0" : : "r"(uwReg));

#ifdef LOSCFG_KERNEL_RUNSTOP
    extern INT32 g_swResumeFromImg;
    if (g_swResumeFromImg == 1) return;
#endif

    // First clear all TT entries - ie Set them to Faulting
    memset((void *)ttb_base, 0, MMU_16K);

/***************************************************************************************************
一级页表配置:段项用来描述仅支持一级页表的配置；
粗页表和细页表都描述的是二级页表的属性。

粗页表可以表示到4k的粒度，每1M需要( 256个 *4B)的二级页表空间来描述。
细页表可以表示到1k的粒度，每1M需要4K的二级页表空间来描述。
SBZ表示should be zero:
段项:
31 30 29 28 27 26 25 24 23 22 21 20 19 18 17 16 15 14 13 12 11 10 9 8 7 6  5 4 3 2 1 0
|       base_addr                  |          SBZ           | AP |0|  domain|1|C|B|1|0|

粗页表:
31 30 29 28 27 26 25 24 23 22 21 20 19 18 17 16 15 14 13 12 11 10 9 8 7 6 5 4 3 2 1 0
|       base_addr                                                |0|domain |1|SBZ|0|1|

细页表:
31 30 29 28 27 26 25 24 23 22 21 20 19 18 17 16 15 14 13 12 11 10 9 8 7 6 5 4 3 2 1 0
|       base_addr                                          | SBZ   |domain |1|SBZ|1|1|
****************************************************************************************************
域模式、AP位和S/R 决定该页的访问权限:
----------------------------------------------------
特权模式           用户模式     AP        S       R 
-----------------------------------------------------------
no access         no access   00         0       0
read only         no access   00         1       0               
read only         read only   00         0       1        ------>当前使用模式，用户模式生效
no access         no access   00         1       1   
read/write        no access   01         *       *
read/write        read only   10         *       *        -------->用户模式，用户模式依然可以读写。 
read/write        read/write  11         *       *
****************************************************************************************************
C和B表示可cache和buffer选项，具体含义如下:
---------------------------------------------
             指令cache
---------------------------------------------
cache 位           页属性
---------------------------------------------
0                  不使用cache
1                  使用cache
*********************************************
             数据cache
---------------------------------------------
cache 位    写缓冲器位        页属性
--------------------------------------------- 
0               0            no cache ,no buffer
0               1            no cache,have buffer
1               0            cache,write through
1               1            cache,write back
****************************************************************************************************/
    // set all mem 4G as uncacheable & rw first
    X_MMU_SECTION(0, 0, (MMU_4G/MMU_1M + 1), UNCACHEABLE, UNBUFFERABLE, ACCESS_RW);

    /**************************************************************************************************
    *    set table as your config
    *    1: LITEOS_CACHE_ADDR ~ LITEOS_CACHE_ADDR + LITEOS_CACHE_LENGTH ---- set as section(1M) and cacheable & rw
    ****************************************************************************************************/
    X_MMU_SECTION((LITEOS_CACHE_ADDR/MMU_1M), (LITEOS_CACHE_ADDR/MMU_1M), (LITEOS_CACHE_LENGTH/MMU_1M), CACHEABLE, BUFFERABLE, ACCESS_RW);
}/*lint !e438  !e550*/

VOID osSecPageInit(VOID)
{
    extern unsigned long __rodata1_end;
    stOsPage.page_addr = SYS_MEM_BASE;
    stOsPage.page_length = ((((unsigned long)&__rodata1_end - SYS_MEM_BASE) + MMU_1M - 1)& ~ (MMU_1M - 1));
    stOsPage.page_descriptor_addr = (UINT32)SECOND_PAGE_DESCRIPTOR_ADDR;
    stOsPage.page_type = MMU_SECOND_LEVEL_SMALL_PAGE_TABLE_ID;

    LOS_SecPageEnable(&stOsPage, BUFFER_ENABLE|CACHE_ENABLE|ACCESS_PERM_RW_RW);
}

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */
