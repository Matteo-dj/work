#ifndef __HIETH_H
#define __HIETH_H

#include "los_base.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

#define HIETH_TSO_SUPPORTED
#define HIETH_TSO_DEBUG
#define HIETH_RXCSUM_SUPPORTED
#define HISI_NET_NON_USE_LOCK

#ifdef HIETH_TSO_SUPPORTED
#include "tso.h"
#endif

#define HIETH_PHY_RMII_MODE    1
#define HIETH_PHY_MII_MODE    0
#define CONFIG_HIETH_TRACE_LEVEL    18
#define HIETH_MAX_QUEUE_DEPTH    64
#define CONFIG_HIETH_HWQ_XMIT_DEPTH 12
#define HIETH_HWQ_TXQ_SIZE    (2 * CONFIG_HIETH_HWQ_XMIT_DEPTH)
#define HIETH_HWQ_RXQ_DEPTH    128

#define HIETH_MIIBUS_NAME    "himii"
#define HIETH_DRIVER_NAME    "hieth"

#define HIETH_MAX_FRAME_SIZE    (1520)

#define HIETH_MAX_MAC_FILTER_NUM    8
#define HIETH_MAX_UNICAST_ADDRESSES    2
#define HIETH_MAX_MULTICAST_ADDRESSES    (HIETH_MAX_MAC_FILTER_NUM - \
        HIETH_MAX_UNICAST_ADDRESSES)

extern VOID LOS_Udelay(UINT32 usecs);
/* Interface Mode definitions */
typedef enum {
    PHY_INTERFACE_MODE_MII,
    PHY_INTERFACE_MODE_RMII,
    PHY_INTERFACE_MODE_MAX,
} phy_interface_t;



#define FC_ACTIVE_MIN        1
#define FC_ACTIVE_DEFAULT    3
#define FC_ACTIVE_MAX        31
#define FC_DEACTIVE_MIN        1
#define FC_DEACTIVE_DEFAULT    5
#define FC_DEACTIVE_MAX        31

extern int tx_flow_ctrl_en;
extern int tx_flow_ctrl_active_threshold;
extern int tx_flow_ctrl_deactive_threshold;

/* port */
#define UP_PORT        0
#define DOWN_PORT    1
#define NO_EEE        0
#define MAC_EEE        1
#define PHY_EEE        2
#define PARTNER_EEE    2
#define DEBUG        0



#ifdef HISI_NET_USE_MUTEX
#define HISI_NET_LOCK_T(net_lock) pthread_mutex_t net_lock
#define HISI_NET_LOCK_INIT(net_lock) do { mutex_init(net_lock); } while (0)
#define HISI_NET_LOCK_GET(net_lock) do { mutex_lock(net_lock); } while (0)
#define HISI_NET_LOCK_PUT(net_lock) do { mutex_unlock(net_lock); } while (0)
#endif

#ifdef HISI_NET_USE_SPINLOCK
typedef struct {
    spinlock_t lock;
    unsigned long flags;
} HISI_NET_SPINLOCK_T;
#define HISI_NET_LOCK_T(net_lock) HISI_NET_SPINLOCK_T net_lock
#define HISI_NET_LOCK_INIT(net_lock) do { spin_lock_init(&((net_lock)->lock)); } while (0)
#define HISI_NET_LOCK_GET(net_lock) do { spin_lock_irqsave(&((net_lock)->lock), (net_lock)->flags); } while (0)
#define HISI_NET_LOCK_PUT(net_lock) do { spin_unlock_irqrestore(&(net_lock)->lock, (net_lock)->flags); } while (0)
#endif

#ifdef HISI_NET_NON_USE_LOCK
#define HISI_NET_LOCK_T(net_lock)
#define HISI_NET_LOCK_INIT(net_lock) do { } while (0)
#define HISI_NET_LOCK_GET(net_lock) do { } while (0)
#define HISI_NET_LOCK_PUT(net_lock) do { } while (0)
#endif

#define EVENT_NET_TX_RX     0x1
#define EVENT_NET_CAN_SEND  0x2

struct hiphy_info {
    char *name;
    unsigned long  phy_id;
    unsigned int  with_eee; /*1: eee supported by this phy */
    struct hiphy_driver *driver;
};

struct hiphy_driver {
    int (*eee_enable)(void);
};

struct hieth_netdev_local {
#ifdef HIETH_TSO_SUPPORTED
    struct dma_tx_desc *dma_tx;
    unsigned long dma_tx_phy;
    unsigned int sg_head;
    unsigned int sg_tail;
#endif
    char *iobase;           /* virtual io addr */
    unsigned long iobase_phys;    /* physical io addr */

#ifdef HIETH_TSO_SUPPORTED
    struct tx_pkt_info *txq;
    unsigned int txq_head;
    unsigned int txq_tail;
    int q_size;
#else
//    struct sk_buff_head tx_hw;    /*tx pkgs in hw*/
#endif
    int tx_hw_cnt;

    struct {
        int hw_xmitq;
    } depth;

#define SKB_SIZE        (HIETH_MAX_FRAME_SIZE)
    unsigned int phy_id;
    unsigned int mdio_frqdiv;
    int link_stat;
    int tx_busy;

    HISI_NET_LOCK_T(tx_lock); /*lint !e19*/
    HISI_NET_LOCK_T(rx_lock); /*lint !e19*/
};

extern struct hieth_netdev_local netdev_local;

#ifndef HISI_NET_NON_USE_LOCK
extern HISI_NET_LOCK_T(hieth_glb_reg_lock);
#endif

#ifdef HIETH_TSO_DEBUG
#define MAX_RECORD      (100)
struct send_pkt_info {
    u32 reg_addr;
    u32 reg_pkt_info;
    u32 status;
};
#endif

/* ***********************************************************
 *
 * Only for internal used!
 *
 * ***********************************************************
 */

/* read/write IO */
#define hieth_readl(ld, ofs)  (GET_UINT32((unsigned int)((ld)->iobase) + (unsigned int)(ofs)))
#define hieth_writel(ld, v, ofs)  (WRITE_UINT32((unsigned int)(v), (unsigned int)((ld)->iobase) + (unsigned int)(ofs)))
#define MK_BITS(shift, nbits)    ((((shift)&0x1F)<<16) | ((nbits)&0x3F))

#define hieth_writel_bits(ld, v, ofs, bits_desc) do { \
    unsigned long _bits_desc = (unsigned long)(bits_desc); \
    unsigned long _shift = (_bits_desc)>>16; \
    unsigned long _reg = hieth_readl(ld, ofs); \
    unsigned long _mask = ((_bits_desc & 0x3F) < 32) ? \
                ((((unsigned int)1<<(_bits_desc & 0x3F)) - 1)<<(_shift)) :\
                 0xffffffff; \
    hieth_writel(ld, (_reg & (~_mask)) | (((unsigned int)(v)<<(_shift)) & _mask), ofs); \
} while (0)
#define hieth_readl_bits(ld, ofs, bits_desc) ({ \
        unsigned long _bits_desc = (unsigned long)(bits_desc); \
        unsigned long _shift = (_bits_desc)>>16; \
        unsigned long _mask = ((_bits_desc & 0x3F) < 32) ?\
                 ((((unsigned int)1 << (_bits_desc & 0x3F)) - 1)<<(_shift)) :\
                 0xffffffff; \
        (hieth_readl(ld, ofs)&_mask)>>(_shift); })
        
#define HIETH_DEBUG        0
#define HIETH_DEBUG_LEVEL  LOS_ERR_LEVEL
#if HIETH_DEBUG
#define hieth_debug(level, fmt, args...) do { \
    if ((level) >= HIETH_DEBUG_LEVEL) { \
        dprintf("[hieth] "); dprintf(fmt, ##args);\
    } \
} while (0)
#else
#define hieth_debug(level, fmt, args...)
#endif

#define UD_REG_NAME(name)       (U_##name)
#define UD_BIT_NAME(name)       (name##_U)
#define UD_PHY_NAME(name)       (name##_U)

#define UD_BIT(port, name)    (name##_U)

struct hiphy_info *phy_search_ids(unsigned long phy_id);
extern struct hiphy_info phy_info_table[];

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */

#endif

/* vim: set ts=8 sw=8 tw=78: */
