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
#ifndef __HISOC_MMC_H_
#define __HISOC_MMC_H_

/************************************************************************/

#include "asm/platform.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

/************************************************************************/
//#define CONFIG_MMC_DMA_AREA_NOCACHEABLE
#define SDIO1_DET_MODE          (1U << 0)
#define SDIO0_DET_MODE          (1U << 1)

#define SDIO0_CLK_BIT_HIGH      (1U << 3)
#define SDIO0_CLK_BIT_LOW       (1U << 2)
#define SDIO0_CLK_SEL_100M      (1U << 2) /* 100MHz */
#define SDIO0_CLK_SEL_75M       (3U << 2) /* 75MHz */
#define SDIO0_CLK_SEL_50M       (0U << 2) /* 50MHz */
#define SDIO0_CLK_SEL_25M       (2U << 2) /* 25MHz */

#define SDIO1_CLK_BIT_HIGH      (1U << 11)
#define SDIO1_CLK_BIT_LOW       (1U << 10)
#define SDIO1_CLK_SEL_100M      (1U << 10) /* 100MHz */
#define SDIO1_CLK_SEL_75M       (3U << 10) /* 75MHz */
#define SDIO1_CLK_SEL_50M       (0U << 10) /* 50MHz */
#define SDIO1_CLK_SEL_25M       (2U << 10) /* 25MHz */

#define SDIO_CCLK_DS_3V3                0x60
#define SDIO_CCMD_DS_3V3                0xe0
#define SDIO_CDATA0_DS_3V3              0xe0
#define SDIO_CDATA1_DS_3V3              0xe0
#define SDIO_CDATA2_DS_3V3              0xe0
#define SDIO_CDATA3_DS_3V3              0xe0
#define SDIO_CCLK_DS_1V8                0x40
#define SDIO_CCMD_DS_1V8                0xd0
#define SDIO_CDATA0_DS_1V8              0xd0
#define SDIO_CDATA1_DS_1V8              0xd0
#define SDIO_CDATA2_DS_1V8              0xd0
#define SDIO_CDATA3_DS_1V8              0xd0

#define MULTIPLEX_EMMC
#ifdef MULTIPLEX_EMMC
#define EMMC_DDR50
#define EMMC_HS200
#endif


#define CONFIG_MMC0_CLK        148500000
#define CONFIG_MMC1_CLK        49500000

#define CONFIG_MMC0_CCLK_MIN 60000 //60KHz
#define CONFIG_MMC0_CCLK_MAX 148500000 //100MHz
#define CONFIG_MMC1_CCLK_MIN 60000 //60KHz
#define CONFIG_MMC1_CCLK_MAX 49500000 //50MHz

#define PERI_CRG49            (CRG_REG_BASE + 0xC4)
#define SDIO0_CLK_SEL_MASK              (0x3 << 10)
#define SDIO0_CLK_SEL_99M               (0x0 << 10)
#define SDIO0_CLK_SEL_74_25M            (0x1 << 10)
#define SDIO0_CLK_SEL_49_5M             (0x2 << 10)
#define SDIO0_CLK_SEL_148_5M            (0x3 << 10)
#define SDIO0_CKEN                      (0x1 << 9)
#define SDIO0_SRST_REQ                  (0x1 << 8)
#define SDIO0_RESET                     (0x1 << 8)

#define SDIO0_CLK_99M                   99000000
#define SDIO0_CLK_74_25M                74250000
#define SDIO0_CLK_49_5M                 49500000
#define SDIO0_CLK_148_5M                148500000
/* SDIO1 REG */
#define SDIO1_CLK_SEL_MASK              (0x3 << 2)
#define SDIO1_CLK_SEL_49_5M             (0x0 << 2)
#define SDIO1_CKEN                      (0x1 << 1)
#define SDIO1_RESET                     (0x1 << 0)
#define SDIO1_CLK_49_5M                 49500000



/* sdio0:sdxc pad ctrl reg */
#define SDIO0_0_CCLK    (IO_CTL_REG_BASE + 0xb4)
#define SDIO0_0_CCMD    (IO_CTL_REG_BASE + 0xb8)
#define SDIO0_0_CDATA0  (IO_CTL_REG_BASE + 0xbc)
#define SDIO0_0_CDATA1  (IO_CTL_REG_BASE + 0xc0)
#define SDIO0_0_CDATA2  (IO_CTL_REG_BASE + 0xc4)
#define SDIO0_0_CDATA3  (IO_CTL_REG_BASE + 0xc8)

/* sdio0:eMMC pad ctrl reg */
#define SDIO0_1_CCLK    (IO_CTL_REG_BASE + 0xcc)
#define SDIO0_1_CCMD    (IO_CTL_REG_BASE + 0xd8)
#define SDIO0_1_CDATA0  (IO_CTL_REG_BASE + 0xdc)
#define SDIO0_1_CDATA1  (IO_CTL_REG_BASE + 0xf8)
#define SDIO0_1_CDATA2  (IO_CTL_REG_BASE + 0xfc)
#define SDIO0_1_CDATA3  (IO_CTL_REG_BASE + 0xd4)
#define SDIO0_1_CDATA4  (IO_CTL_REG_BASE + 0xd0)
#define SDIO0_1_CDATA5  (IO_CTL_REG_BASE + 0xec)
#define SDIO0_1_CDATA6  (IO_CTL_REG_BASE + 0xe8)
#define SDIO0_1_CDATA7  (IO_CTL_REG_BASE + 0xf0)


/* sdio1 pad ctrl reg */
#define SDIO1_CCLK      (IO_CTL_REG_BASE + 0x10)
#define SDIO1_CCMD      (IO_CTL_REG_BASE + 0x28)
#define SDIO1_CDATA0    (IO_CTL_REG_BASE + 0x20)
#define SDIO1_CDATA1    (IO_CTL_REG_BASE + 0x1c)
#define SDIO1_CDATA2    (IO_CTL_REG_BASE + 0x34)
#define SDIO1_CDATA3    (IO_CTL_REG_BASE + 0x24)


/* sdio0:sdxc pad ctrl config */
#define SDIO0_0_CCLK_DS_3V3             0xe0
#define SDIO0_0_CCMD_DS_3V3             0xe0
#define SDIO0_0_CDATA0_DS_3V3           0xe0
#define SDIO0_0_CDATA1_DS_3V3           0xe0
#define SDIO0_0_CDATA2_DS_3V3           0xe0
#define SDIO0_0_CDATA3_DS_3V3           0xe0

#define SDIO0_0_CCLK_DS_1V8             0xb0
#define SDIO0_0_CCMD_DS_1V8             0xd0
#define SDIO0_0_CDATA0_DS_1V8           0xd0
#define SDIO0_0_CDATA1_DS_1V8           0xd0
#define SDIO0_0_CDATA2_DS_1V8           0xd0
#define SDIO0_0_CDATA3_DS_1V8           0xd0
/* sdio0:emmc pad ctrl config */
#define SDIO0_1_CCLK_DS                 0x40
#define SDIO0_1_CCMD_DS                 0x20
#define SDIO0_1_CDATA0_DS               0x20
#define SDIO0_1_CDATA1_DS               0x20
#define SDIO0_1_CDATA2_DS               0x20
#define SDIO0_1_CDATA3_DS               0x20
#define SDIO0_1_CDATA4_DS               0x20
#define SDIO0_1_CDATA5_DS               0x20
#define SDIO0_1_CDATA6_DS               0x20
#define SDIO0_1_CDATA7_DS               0x20


/* sdio1 pad ctrl config */
#define SDIO1_CCLK_DS           0xd0
#define SDIO1_CCMD_DS           0xa0
#define SDIO1_CDATA0_DS         0xa0
#define SDIO1_CDATA1_DS         0xa0
#define SDIO1_CDATA2_DS         0xd0
#define SDIO1_CDATA3_DS         0xa0

extern struct mmc_host s_mmc[3];
extern struct himci_host s_host[2];
extern struct himci_slot s_slot[3];


static void himci_clk_ctrl(struct himci_host *host, bool enable)
{
    unsigned int tmp_reg = 0;
    if (enable) {
        /* enable CRG clk */
        tmp_reg = himci_readl(PERI_CRG49);
        if (host->id == 0)
            tmp_reg |= SDIO0_CKEN;
        else
            tmp_reg |= SDIO1_CKEN;

        himci_writel(tmp_reg, PERI_CRG49);
    } else {
        /* disable CRG clk */
        tmp_reg = himci_readl(PERI_CRG49);
        if (host->id == 0)
            tmp_reg &= ~SDIO0_CKEN;
        else
            tmp_reg &= ~SDIO1_CKEN;

        himci_writel(tmp_reg, PERI_CRG49);
    }
}

static void himci_clk_sel(struct himci_host *host, unsigned int clk)
{
    unsigned int reg_value;
    if (0 == host->id) {
        reg_value = himci_readl(PERI_CRG49);
        reg_value &= ~SDIO0_CLK_SEL_MASK;
        if (clk >= SDIO0_CLK_148_5M) {
            host->hclk = SDIO0_CLK_148_5M;
            reg_value |= SDIO0_CLK_SEL_148_5M;
        }
        else if (clk >= SDIO0_CLK_99M) {
            host->hclk = SDIO0_CLK_99M;
            reg_value |= SDIO0_CLK_SEL_99M;
        }
        else if (clk >= SDIO0_CLK_74_25M) {
            host->hclk = SDIO0_CLK_74_25M;
            reg_value |= SDIO0_CLK_SEL_74_25M;
        }
        else if (clk >= SDIO0_CLK_49_5M) {
            host->hclk = SDIO0_CLK_49_5M;
            reg_value |= SDIO0_CLK_SEL_49_5M;
        }
        else {
            host->hclk = SDIO0_CLK_49_5M;
            reg_value |= SDIO0_CLK_SEL_49_5M;
        }
        himci_writel(reg_value, PERI_CRG49);

    } else if (1 == host->id) {
        reg_value = himci_readl(PERI_CRG49);
        reg_value &= ~SDIO1_CLK_SEL_MASK;
        if (CONFIG_MMC1_CLK >= SDIO1_CLK_49_5M ) {
            host->hclk = SDIO1_CLK_49_5M;
            reg_value |= SDIO1_CLK_SEL_49_5M;
        }
        himci_writel(reg_value, PERI_CRG49);
    }
}

static void hi_mci_sys_ctrl_init(struct himci_host *host)
{
    unsigned int reg_value;

    himci_clk_ctrl(host, false);

    if (0 == host->id) {
        himci_clk_sel(host, CONFIG_MMC0_CLK);

        himci_clk_ctrl(host, true);

        reg_value = himci_readl(PERI_CRG49);
        reg_value |= SDIO0_RESET;
        himci_writel(reg_value, PERI_CRG49);

        LOS_Udelay(1000);
        reg_value = himci_readl(PERI_CRG49);
        reg_value &= ~SDIO0_RESET;
        himci_writel(reg_value, PERI_CRG49);

    } else if (1 == host->id) {
        himci_clk_sel(host, CONFIG_MMC1_CLK);

        himci_clk_ctrl(host, true);

        /* SDIO soft reset */
        reg_value = himci_readl(PERI_CRG49);
        reg_value |= SDIO1_RESET;
        himci_writel(reg_value, PERI_CRG49);

        LOS_Udelay(1000);
        reg_value &= ~SDIO1_RESET;
        himci_writel(reg_value, PERI_CRG49);
    } else {
        himci_error("himci host id error!");
    }

}

static void hi_mci_ctr_reset(struct himci_host *host)
{
    unsigned int reg_value;

    reg_value = himci_readl(PERI_CRG49);

    if (0 == host->id) {
        reg_value |= SDIO0_RESET;
    } else if (1 == host->id) {
        reg_value |= SDIO1_RESET;
    } else {
        himci_error("himci host id error!");
        return;
    }

    himci_writel(reg_value, PERI_CRG49);
}

static void hi_mci_ctr_undo_reset(struct himci_host *host)
{
        unsigned int reg_value;

    reg_value = himci_readl(PERI_CRG49);

    if (0 == host->id) {
        reg_value &= ~(SDIO0_RESET);
    } else if (1 == host->id) {
        reg_value &= ~(SDIO1_RESET);
    } else {
        himci_error("himci host id error!");
        return;
    }

        himci_writel(reg_value, PERI_CRG49);
}

extern void hisd_irq(int irq, void *data);
extern void hi_mci_init_card(struct himci_host *host);

extern  int g_sdioirq_disable;

#define MMC_MAX_NUM (3)
static struct himci_host * hi_mci_host_configure()
{
    int i;
    struct mmc_host *mmc = &s_mmc[0];
    struct himci_host *host = &s_host[0];
    struct himci_slot *slot = &s_slot[0];

    memset(mmc, 0, sizeof(struct mmc_host) * 3);
    memset(slot, 0, sizeof(struct himci_slot)* 3);
    memset(host, 0, sizeof(struct himci_host) * 2);

    //id = 0 for emmc
    mmc[0].f_min = CONFIG_MMC0_CCLK_MIN;
    mmc[0].f_max = CONFIG_MMC0_CCLK_MAX;

    // id = 1 for sd0
    mmc[1].f_min = CONFIG_MMC0_CCLK_MIN;
    mmc[1].f_max = CONFIG_MMC0_CCLK_MAX;

    //id = 2 for sd1
    mmc[2].f_min = CONFIG_MMC1_CCLK_MIN;
    mmc[2].f_max = CONFIG_MMC1_CCLK_MAX;

#ifdef MULTIPLEX_EMMC
    mmc[0].caps |= MMC_CAP_8_BIT_DATA | MMC_CAP_HW_RESET;
#ifdef EMMC_DDR50
    mmc[0].caps |= MMC_CAP_UHS_DDR50;
#endif

#ifdef EMMC_HS200
    mmc[0].caps2 |= MMC_CAP2_HS200;
#endif
#endif

    slot[0].port = 1;
    slot[1].port = 0;
    slot[2].port = 0;

    host[0].id = 0;
    host[1].id = 1;

    for(i = 0; i < MMC_MAX_NUM; i++)
    {
        mmc[i].index = i;
        mmc[i].max_segs = 1024;
        mmc[i].max_blk_size = 512;
        mmc[i].max_blk_count = 2048;
        mmc[i].max_seg_size = mmc[i].max_blk_size * mmc[i].max_blk_count;
        mmc[i].max_req_size = mmc[i].max_blk_size * mmc[i].max_blk_count;
        mmc[i].caps |= MMC_CAP_4_BIT_DATA
                | MMC_CAP_SD_HIGHSPEED
                | MMC_CAP_MMC_HIGHSPEED
                | MMC_CAP_UHS_SDR12
                | MMC_CAP_SDIO_IRQ
                | MMC_CAP_UHS_SDR25
                | MMC_CAP_UHS_SDR50
                /* | MMC_CAP_UHS_DDR50 */
                | MMC_CAP_UHS_SDR104;

    if (g_sdioirq_disable)
        mmc[i].caps &= ~MMC_CAP_SDIO_IRQ;

        mmc[i].ocr_avail = MMC_VDD_32_33 | MMC_VDD_33_34;
        mmc[i].ocr = mmc[i].ocr_avail;
        LOS_EventInit(&mmc[i].sdio_event);

        mmc[i].private = &slot[i];
        slot[i].mmc = &mmc[i];
        slot[i].phase_drv = DRV_PHASE_SHIFT;
        slot[i].phase_smp = SMPL_PHASE_SHIFT;

        slot[i].card_status = CARD_UNPLUGED;
        slot[i].power_status = POWER_OFF;

        if (i == 2)
            host = &s_host[1];
        else
            host = &s_host[0];
        slot[i].host = host;
        LOS_SemCreate(1, &slot[i].sem_id);

        if(!host->used) {
            PRINTK("init\n");
            if((i == 0) || (i == 1))
                host->base = (void *)SDIO0_REG_BASE;
            else
                host->base = (void *)SDIO1_REG_BASE;

#ifndef CONFIG_MMC_DMA_AREA_NOCACHEABLE
            host->dma_vaddr =  (unsigned int *)memalign(CACHE_ALIGNED_SIZE, HIMCI_PAGE_SIZE);
            if (!host->dma_vaddr) {
                himci_error("no mem for himci dma!\n");
                return (struct himci_host *)-1;
            }
#else
            if(i == 0)
                host->dma_vaddr = (unsigned int *)MMC_USE_MEM_START;
            else
                host->dma_vaddr = (unsigned int *)(MMC_USE_MEM_START + HIMCI_PAGE_SIZE + 0x800);

#endif
            host->dma_paddr = (dma_addr_t)(host->dma_vaddr);
            hi_mci_sys_ctrl_init(host);
            hi_mci_init_card(host);
            if((i == 0) || (i == 1)) {
                request_irq(NUM_HAL_INTERRUPT_SDIO, hisd_irq, 0, "MMC-SDIO", host);
                hal_interrupt_unmask(NUM_HAL_INTERRUPT_SDIO);
            } else {
                request_irq(NUM_HAL_INTERRUPT_SDIO1, hisd_irq, 0, "MMC-SDIO1", host);
                hal_interrupt_unmask(NUM_HAL_INTERRUPT_SDIO1);
            }
            LOS_EventInit(&host->himci_event);
            spin_lock_init(&host->lock);
            host->used++;

        } else {
            slot[i].hclk = host->hclk;
            slot[i].cclk = 0;
        }
        Create_Intr_Thread(&slot[i]);
        himci_trace(6,"mmc[%0d] init done!\n", i);
    }
    return 0;
}


#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */
#endif

