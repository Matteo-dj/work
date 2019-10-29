#include "eth_drv.h"
#include "hieth.h"
#include "ctrl.h"
#include "mac.h"

static inline void hieth_enable_rxcsum_drop(struct hieth_netdev_local *ld,
                bool drop)
{
        hieth_writel_bits(ld, drop, UD_REG_NAME(GLB_RX_COE_CTRL),
                        BITS_COE_IPHDR_DROP);/*lint !e514*/
        hieth_writel_bits(ld, drop, UD_REG_NAME(GLB_RX_COE_CTRL),
                        BITS_COE_PAYLOAD_DROP);/*lint !e514*/
        hieth_writel_bits(ld, drop, UD_REG_NAME(GLB_RX_COE_CTRL),
                        BITS_COE_IPV6_UDP_ZERO_DROP);/*lint !e514*/
}

static int hieth_port_reset(struct hieth_netdev_local *ld)
{
    /*soft reset*/
    /* Note: sf ip need reset twice */
    hieth_writel_bits(ld, 1, GLB_SOFT_RESET,
            BITS_ETH_SOFT_RESET_ALL);
    LOS_Msleep(1);
    hieth_writel_bits(ld, 0, GLB_SOFT_RESET,
            BITS_ETH_SOFT_RESET_ALL);
    LOS_Msleep(1);
    hieth_writel_bits(ld, 1, GLB_SOFT_RESET,
            BITS_ETH_SOFT_RESET_ALL);
    LOS_Msleep(1);
    hieth_writel_bits(ld, 0, GLB_SOFT_RESET,
            BITS_ETH_SOFT_RESET_ALL);

    return 0;
}

static int hieth_port_init(struct hieth_netdev_local *ld)
{
    /* HW DEFAULT RX-PKT-LEN-RANGE [42,1518]    */
    /* HW MAC FILTER TABLE DISABLE            */

    (void)hieth_set_endian_mode(ld, HIETH_LITTLE_ENDIAN);
    (void)hieth_set_linkstat(ld, 0);
    (void)hieth_set_negmode(ld, HIETH_NEGMODE_CPUSET);

    /*clear all interrupt status*/
    hieth_clear_irqstatus(ld, UD_BIT_NAME(BITS_IRQS_MASK));
    /*disable interrupts*/
    hieth_writel_bits(ld, 0, GLB_RW_IRQ_ENA, UD_BIT_NAME(BITS_IRQS_ENA));
    (void)hieth_irq_disable(ld, UD_BIT_NAME(BITS_IRQS_MASK));


    /*disable vlan func*/
    hieth_writel_bits(ld, 0, GLB_FWCTRL, BITS_VLAN_ENABLE);

    /*enable UpEther<->CPU*/
    hieth_writel_bits(ld, 1, GLB_FWCTRL,  BITS_FW2CPU_ENA_U);
    hieth_writel_bits(ld, 0, GLB_FWCTRL,   BITS_FWALL2CPU_U);
    hieth_writel_bits(ld, 1, GLB_MACTCTRL, BITS_BROAD2CPU_U);
    #if defined(PRODUCT_CFG_FEATURE_REMOTE_COMMUNICATION_TERMINAL)
    hieth_writel_bits(ld, 0, GLB_MACTCTRL, BITS_MACT_ENA_U);
    hieth_writel_bits(ld, 1, GLB_MACTCTRL, BITS_UNI2CPU_U);
    #else
    hieth_writel_bits(ld, 1, GLB_MACTCTRL, BITS_MACT_ENA_U);
    #endif
    hieth_writel_bits(ld, 1, GLB_MACTCTRL, UD_BIT(ld->port, BITS_MULTI2CPU));

    (void)hieth_set_mac_leadcode_cnt_limit(ld, 0);
    hieth_set_rcv_len_max(ld, HIETH_MAX_RCV_LEN);

    return 0;
}

void hieth_hw_mac_core_init(struct hieth_netdev_local *ld)
{
    HISI_NET_LOCK_INIT(&(ld->tx_lock));
    HISI_NET_LOCK_INIT(&(ld->rx_lock));

        /* reset and init port */
    (void)hieth_port_reset(ld);
    (void)hieth_port_init(ld);

#ifdef HIETH_RXCSUM_SUPPORTED
        hieth_enable_rxcsum_drop(ld, true);
#endif

#ifdef HIETH_TSO_SUPPORTED
        ld->sg_head = ld->sg_tail = 0;
        ld->txq_head = ld->txq_tail = 0;
#endif
        ld->tx_hw_cnt = 0;

        /* setup hardware */
    (void)hieth_set_hwq_depth(ld);
}

void hieth_hw_mac_core_reset(struct hieth_netdev_local *ld)
{
    unsigned int v = 0;

    //mii_clk_sel
    v = GET_UINT32(0x101e0000 + 0x28);
    v &= ~(0x3 << 1);
    WRITE_UINT32(v, 0x101e0000 + 0x28);

    //peri_rstn
    v = GET_UINT32(0x101e0000);
    v |= (1 << 7);
    WRITE_UINT32(v, 0x101e0000);


    /* enable eth clk */
    v = GET_UINT32(0x101e0000 + 0x20);
    v |= (1 << 2);
    WRITE_UINT32(v, 0x101e0000 + 0x20);


    /* sys reset */
    v = GET_UINT32(0x101e0000 + 0x2c);
    v &= ~(1 << 3);
    WRITE_UINT32(v, 0x101e0000 + 0x2c);

}

/*BOOT下已经提前复位，OS下不再复位*/
void hieth_hw_external_phy_reset(void)
{

}

static inline unsigned int _hieth_irq_enable(struct hieth_netdev_local *ld, unsigned int irqs)
{
    unsigned int old;

    old = hieth_readl(ld, (unsigned int)GLB_RW_IRQ_ENA);
    hieth_writel(ld, old | irqs, (unsigned int)GLB_RW_IRQ_ENA);
    old = hieth_readl(ld, (unsigned int)GLB_RW_IRQ_ENA);

    return old;
}

static inline unsigned int _hieth_irq_disable(struct hieth_netdev_local *ld, unsigned int irqs)
{
    unsigned int old;

    old = hieth_readl(ld, GLB_RW_IRQ_ENA);
    hieth_writel(ld, old & (~irqs), GLB_RW_IRQ_ENA);

    return old;
}

static inline unsigned int _hieth_read_irqstatus(struct hieth_netdev_local *ld)
{
    unsigned int status;

    status = hieth_readl(ld, GLB_RO_IRQ_STAT);

    return status;
}

int hieth_hw_set_macaddress(
        struct hieth_netdev_local *ld,
        int ena,
        unsigned char *mac)
{
    unsigned int reg;

 
    reg = mac[1] | (mac[0] << 8);
    hieth_writel(ld, reg, GLB_HOSTMAC_H16);

    reg = mac[5] | (mac[4]<<8) | (mac[3]<<16) | (mac[2]<<24);
    hieth_writel(ld, reg, GLB_HOSTMAC_L32);

    return 0;
}

int hieth_hw_get_macaddress(struct hieth_netdev_local *ld, unsigned char *mac)
{
    unsigned long reg;

    reg = hieth_readl(ld, GLB_HOSTMAC_H16);
    mac[0] = (reg>>8) & 0xff;
    mac[1] = reg & 0xff;

    reg = hieth_readl(ld, GLB_HOSTMAC_L32);
    mac[2] = (reg>>24) & 0xff;
    mac[3] = (reg>>16) & 0xff;
    mac[4] = (reg>>8) & 0xff;
    mac[5] = reg & 0xff;

    return 0;
}

unsigned int _test_xmit_queue_ready(struct hieth_netdev_local *ld)
{
    return hieth_readl_bits(ld,
             UD_REG_NAME(GLB_RO_QUEUE_STAT), BITS_XMITQ_RDY);
}

unsigned int hieth_irq_enable(struct hieth_netdev_local *ld, unsigned int irqs)
{
    unsigned int old;

    HISI_NET_LOCK_GET(&hieth_glb_reg_lock);
    old = _hieth_irq_enable(ld, irqs);
    HISI_NET_LOCK_PUT(&hieth_glb_reg_lock);

    return old;
}

unsigned int hieth_irq_disable(struct hieth_netdev_local *ld, unsigned int irqs)
{
    unsigned int old;

    HISI_NET_LOCK_GET(&hieth_glb_reg_lock);
    old = _hieth_irq_disable(ld, irqs);
    HISI_NET_LOCK_PUT(&hieth_glb_reg_lock);

    return old;
}

unsigned int hieth_read_irqstatus(struct hieth_netdev_local *ld)
{
    unsigned int status;

    status = _hieth_read_irqstatus(ld);

    return status;
}

unsigned int hieth_read_raw_irqstatus(struct hieth_netdev_local *ld)
{
    unsigned int status;

    status = hieth_readl(ld, GLB_RO_IRQ_STAT);

    return status;
}

void hieth_clear_irqstatus(struct hieth_netdev_local *ld, unsigned int irqs)
{
    HISI_NET_LOCK_GET(&hieth_glb_reg_lock);
    hieth_writel(ld, irqs, GLB_RW_IRQ_RAW);
    HISI_NET_LOCK_PUT(&hieth_glb_reg_lock);
}

unsigned int hieth_set_endian_mode(struct hieth_netdev_local *ld, unsigned int mode)
{
    unsigned int old;

    old = hieth_readl_bits(ld, GLB_ENDIAN_MOD, BITS_ENDIAN);
    hieth_writel_bits(ld, mode, GLB_ENDIAN_MOD, BITS_ENDIAN);

    return old;
}

int hieth_set_hwq_depth(struct hieth_netdev_local *ld)
{

    if ((ld->depth.hw_xmitq) > HIETH_MAX_QUEUE_DEPTH) {
        hieth_debug(LOS_ERR_LEVEL, "Depth of txt queque is invalid:%d\n", ld->depth.hw_xmitq);
        return -1;
    }

    hieth_writel_bits(ld, ld->depth.hw_xmitq, UD_REG_NAME(GLB_QLEN_SET),
            BITS_TXQ_DEP);
    hieth_writel_bits(ld, HIETH_MAX_QUEUE_DEPTH - ld->depth.hw_xmitq,
            UD_REG_NAME(GLB_QLEN_SET), BITS_RXQ_DEP);

    return 0;
}

unsigned int hieth_hw_xmitq_ready(struct hieth_netdev_local *ld)
{
    unsigned int ret;

    ret = _test_xmit_queue_ready(ld);

    return ret;
}

extern EVENT_CB_S gEthEvent;

int hieth_xmit_release_pkt(struct hieth_netdev_local *ld,
                struct los_eth_driver *sc,
                struct hieth_priv_s *priv)
{
    int ret = 0;
    struct tx_pkt_info *txq_cur;
    int tx_reclaim_cnt = 0;
    struct pbuf_info *pbuf;
    u32 i;
    int tx_hw_cnt;

    HISI_NET_LOCK_GET(&(ld->tx_lock));
    tx_hw_cnt=(int)hw_xmitq_cnt_inuse(ld);
    while ( tx_hw_cnt < ld->tx_hw_cnt) {
        txq_cur = ld->txq + ld->txq_tail;
        pbuf = priv->ram->pbuf_info + ld->txq_tail;
        for (i = 0; i < pbuf->sg_len; i++) {
            pbuf_dma_free(pbuf->dma_info[i]);
        }

        txq_cur->tx_addr = 0;

        ld->txq_tail = (ld->txq_tail + 1) % (unsigned int)ld->q_size;

        tx_reclaim_cnt++;
        ld->tx_hw_cnt--;
        tx_hw_cnt=(int)hw_xmitq_cnt_inuse(ld);
    }

    if (tx_reclaim_cnt && ld->tx_busy) {
        ld->tx_busy = 0;
        (void)LOS_EventWrite(&gEthEvent, EVENT_NET_CAN_SEND);
    }

    HISI_NET_LOCK_PUT(&(ld->tx_lock));
    return ret;
}

int hieth_xmit_gso(struct hieth_netdev_local *ld,
        hieth_priv_t *priv, struct pbuf *p)
{
    struct tx_pkt_info *txq_cur;
    struct pbuf *p_iter;
    int send_pkt_len;
    int sg_len;
    u32 buf_addr, buf_len;
    int i;
    struct pbuf_info *pb_info;

    struct dma_tx_desc *dma_sg;

    send_pkt_len = p->tot_len;
    if (send_pkt_len > HIETH_MAX_FRAME_SIZE)
    {
        hieth_debug(LOS_ERR_LEVEL, "xmit len overflow:%d\n", send_pkt_len);
    }

    pb_info = &(priv->ram->pbuf_info[ld->txq_head]);
    sg_len = 0;
    for (p_iter = p; p_iter != NULL; p_iter = p_iter->next)
    {
        if (sg_len >= MAX_ETH_DRV_SG) {
            hieth_debug(LOS_ERR_LEVEL, "sg list num err, %d.\n", sg_len);
            return -1;
        }
        (void)pbuf_dma_ref(p_iter->dma_info);
        
        pb_info->dma_info[sg_len] = p_iter->dma_info;
        sg_len++;
    }
    pb_info->sg_len = (unsigned int)sg_len;

    txq_cur = ld->txq + ld->txq_head;
    txq_cur->tx.val = 0;
    txq_cur->tx.info.tso_flag = 0;
    txq_cur->tx.info.coe_flag = 0;

    if (sg_len == 1) {
        txq_cur->tx.info.sg_flag = 0;
        net_dma_cache_clean((void *)p->payload, (unsigned int)send_pkt_len);
        txq_cur->tx_addr = (unsigned int)p->payload;
    } else {
        txq_cur->tx.info.sg_flag = 1;
        txq_cur->tx.info.nfrags_num = (unsigned int)(sg_len - 1);

        dma_sg = priv->ram->dma_tx + ld->txq_head;
        dma_sg->total_len = (unsigned int)send_pkt_len;

        buf_addr = (u32)p->payload;
        buf_len = p->len;
        net_dma_cache_clean((void *)buf_addr, buf_len);
        dma_sg->linear_addr = buf_addr;
        dma_sg->linear_len = buf_len;
        p_iter = p->next;
        for(i=1; i<sg_len; i++)
        {
            buf_addr = (u32)p_iter->payload;
            buf_len = p_iter->len;
            net_dma_cache_clean((void *)buf_addr, buf_len);
            dma_sg->frags[i-1].addr = buf_addr;
            dma_sg->frags[i-1].size = buf_len;
            p_iter = p_iter->next;
        }

        net_dma_cache_clean((void *)dma_sg, sizeof(struct dma_tx_desc));
        txq_cur->tx_addr = (uintptr_t)dma_sg;
    }

    txq_cur->tx.info.data_len = (unsigned int)(send_pkt_len + FCS_BYTES);

    hw_xmitq_pkg(ld, txq_cur->tx_addr, txq_cur->tx.val);

    ld->txq_head = (ld->txq_head + 1) % (unsigned int)ld->q_size;

    return 0;
}

int hieth_feed_hw(struct hieth_netdev_local *ld, hieth_priv_t *priv)
{
    int cnt = 0;
    struct pbuf *pbuf;

    HISI_NET_LOCK_GET(&(ld->rx_lock));

    while (hieth_readl_bits(ld, UD_REG_NAME(GLB_RO_QUEUE_STAT),
                 BITS_RECVQ_RDY)) {

        if (((priv->rx_feed + 1) % HIETH_HWQ_RXQ_DEPTH) == priv->rx_release)
            break;

        pbuf = pbuf_alloc(PBUF_RAW, (HIETH_MAX_FRAME_SIZE + ETH_PAD_SIZE), PBUF_RAM);
        if (!pbuf)
            break;

#if ETH_PAD_SIZE
        /* drop the padding word */
        if (pbuf_header(pbuf, -ETH_PAD_SIZE)) {
            hieth_debug(LOS_ERR_LEVEL, "pbuf pad error, drop...\n");
            (void)pbuf_free(pbuf);
            break;
        }
#endif
        priv->ram->rx_pbuf[priv->rx_feed] = pbuf;
        net_dma_cache_inv(pbuf->payload, HIETH_MAX_FRAME_SIZE);

        hieth_writel(ld, (unsigned int)pbuf->payload,
                 UD_REG_NAME(GLB_IQ_ADDR));

        priv->rx_feed = (priv->rx_feed + 1) % HIETH_HWQ_RXQ_DEPTH;

        cnt++;
    }

    HISI_NET_LOCK_PUT(&(ld->rx_lock));

    return cnt;
}

/* vim: set ts=8 sw=8 tw=78: */



unsigned int malloc_new_mem(struct hieth_netdev_local *ld, hieth_priv_t *priv, unsigned int malloc_cnt)
{
    struct pbuf *pbuf;
    unsigned int cnt = 0;

    HISI_NET_LOCK_GET(&(ld->rx_lock));
    
    while(cnt<malloc_cnt)
    {
        if (((priv->rx_feed + 1) % HIETH_HWQ_RXQ_DEPTH) == priv->rx_release)
            break;
 
        pbuf = pbuf_alloc(PBUF_RAW, (HIETH_MAX_FRAME_SIZE + ETH_PAD_SIZE), PBUF_RAM);
        if (!pbuf)
            break;

#if ETH_PAD_SIZE
        /* drop the padding word */
        if (pbuf_header(pbuf, -ETH_PAD_SIZE)) {
            hieth_debug(LOS_ERR_LEVEL, "pbuf pad error, drop...\n");
            (void)pbuf_free(pbuf);
            break;
        }
#endif

        priv->ram->rx_pbuf[priv->rx_feed] = pbuf;
        net_dma_cache_inv(pbuf->payload, HIETH_MAX_FRAME_SIZE);
        hieth_writel(ld, (unsigned int)pbuf->payload,
                 UD_REG_NAME(GLB_IQ_ADDR));

        priv->rx_feed = (priv->rx_feed + 1) % HIETH_HWQ_RXQ_DEPTH;    
        cnt++;
    }
    
    HISI_NET_LOCK_PUT(&(ld->rx_lock));
    
    return cnt;

}