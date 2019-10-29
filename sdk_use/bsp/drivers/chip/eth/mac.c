#include "hieth.h"
#include "mac.h"

static unsigned int _set_linkstat(struct hieth_netdev_local *ld,unsigned int mode)
{
    unsigned int old;

    old = hieth_readl_bits(ld, UD_REG_NAME(MAC_PORTSET), (unsigned int)BITS_MACSTAT);
    hieth_writel_bits(ld, mode, UD_REG_NAME(MAC_PORTSET), (unsigned int)BITS_MACSTAT);

    return old;
}

static unsigned int _set_negmode(struct hieth_netdev_local *ld, unsigned int mode)
{
    unsigned int old;

    old = hieth_readl_bits(ld, UD_REG_NAME(MAC_PORTSEL), BITS_NEGMODE);
    hieth_writel_bits(ld, mode, UD_REG_NAME(MAC_PORTSEL), BITS_NEGMODE);

    return old;
}

static unsigned int _get_negmode(struct hieth_netdev_local *ld)
{
    unsigned int old;

    old = hieth_readl_bits(ld, UD_REG_NAME(MAC_PORTSEL), BITS_NEGMODE);

    return old;
}


unsigned int hieth_set_linkstat(struct hieth_netdev_local *ld, unsigned int mode)
{
    unsigned int old;

    HISI_NET_LOCK_GET(&hieth_glb_reg_lock);
    old = _set_linkstat(ld, mode);
    HISI_NET_LOCK_PUT(&hieth_glb_reg_lock);

    return old;
}

unsigned int hieth_get_linkstat(struct hieth_netdev_local *ld)
{
    unsigned int old;

    HISI_NET_LOCK_GET(&hieth_glb_reg_lock);
    old = hieth_readl_bits(ld, UD_REG_NAME(MAC_RO_STAT), BITS_MACSTAT);
    HISI_NET_LOCK_PUT(&hieth_glb_reg_lock);

    return old;
}

unsigned int hieth_set_mac_leadcode_cnt_limit(struct hieth_netdev_local *ld, unsigned int cnt)
{
    unsigned int old;

    HISI_NET_LOCK_GET(&hieth_glb_reg_lock);
    old = hieth_readl_bits(ld,
             UD_REG_NAME(MAC_TX_IPGCTRL), BITS_PRE_CNT_LIMIT);
    hieth_writel_bits(ld, cnt,
             UD_REG_NAME(MAC_TX_IPGCTRL), BITS_PRE_CNT_LIMIT);
    HISI_NET_LOCK_PUT(&hieth_glb_reg_lock);

    return old;
}

unsigned int hieth_set_mac_trans_interval_bits(struct hieth_netdev_local *ld, unsigned int nbits)
{
    unsigned int old;
    unsigned int linkstat, negmode;

    HISI_NET_LOCK_GET(&hieth_glb_reg_lock);

    negmode = _set_negmode(ld, HIETH_NEGMODE_CPUSET);
    linkstat = _set_linkstat(ld, 0);
    LOS_Udelay(1000);

    old = hieth_readl_bits(ld, UD_REG_NAME(MAC_TX_IPGCTRL), BITS_IPG);
    hieth_writel_bits(ld, nbits, UD_REG_NAME(MAC_TX_IPGCTRL), BITS_IPG);
    LOS_Udelay(100);

    (void)_set_negmode(ld, negmode);
    (void)_set_linkstat(ld, linkstat);

    HISI_NET_LOCK_PUT(&hieth_glb_reg_lock);

    return old;
}

unsigned int hieth_set_mac_fc_interval(struct hieth_netdev_local *ld, unsigned int para)
{
    unsigned int old;

    HISI_NET_LOCK_GET(&hieth_glb_reg_lock);
    old = hieth_readl_bits(ld, UD_REG_NAME(MAC_TX_IPGCTRL), BITS_FC_INTER);
    hieth_writel_bits(ld, para, UD_REG_NAME(MAC_TX_IPGCTRL), BITS_FC_INTER);
    HISI_NET_LOCK_PUT(&hieth_glb_reg_lock);

    return old;
}

unsigned int hieth_set_negmode(struct hieth_netdev_local *ld, unsigned int mode)
{
    unsigned int old;

    HISI_NET_LOCK_GET(&hieth_glb_reg_lock);
    old = _set_negmode(ld, mode);
    HISI_NET_LOCK_PUT(&hieth_glb_reg_lock);

    return old;
}

unsigned int hieth_get_negmode(struct hieth_netdev_local *ld)
{
    unsigned int old;

    HISI_NET_LOCK_GET(&hieth_glb_reg_lock);
    old = _get_negmode(ld);
    HISI_NET_LOCK_PUT(&hieth_glb_reg_lock);

    return old;
}

unsigned int hieth_set_mii_mode(struct hieth_netdev_local *ld, unsigned int mode)
{
    unsigned int old;

    HISI_NET_LOCK_GET(&hieth_glb_reg_lock);
    old = hieth_readl_bits(ld, UD_REG_NAME(MAC_PORTSEL), BITS_MII_MODE);
    hieth_writel_bits(ld, mode, UD_REG_NAME(MAC_PORTSEL), BITS_MII_MODE);
    HISI_NET_LOCK_PUT(&hieth_glb_reg_lock);

    return old;
}

void hieth_set_rcv_len_max(struct hieth_netdev_local *ld, unsigned int cnt)
{
    HISI_NET_LOCK_GET(&hieth_glb_reg_lock);
    hieth_writel_bits(ld, cnt, UD_REG_NAME(MAC_SET), BITS_LEN_MAX);
    HISI_NET_LOCK_PUT(&hieth_glb_reg_lock);
}

/* vim: set ts=8 sw=8 tw=78: */
