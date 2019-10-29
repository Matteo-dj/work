#include "hieth.h"
#include "mdio.h"//包含了kernel.h的头文件要放在包含了param.h的头文件前面。
#include "string.h"
#include "mac.h"
#include "hisi_mac.h"
#include "eth_drv.h"
#include "ctrl.h"
#include "eth_phy.h"
#include "eth_phy_dev.h"
#include "netdev.h"
#include "los_task.h"
#include "los_base.h"
#include <hi_types.h>
#include <hi_stdlib.h>
#include "hi_low_power.h"
#include "hi_reg_sys.h"
#ifdef PRODUCT_CFG_FEATURE_REMOTE_COMMUNICATION_TERMINAL
#include "hi_mdm.h"
#endif

EVENT_CB_S gEthEvent;

#define PHY_ADDR_SET        0x129B5B3
#define PHY_ADDR_NOT_SET    0x5a5aa5a5
#define INVALID_PHY_ADDR    0x12F358F
#define PHY_MODE_SET        0x12B63D0
#define PHY_MODE_NOT_SET    0x5a5aa5a5
#define INVALID_PHY_MODE    0x12F5D09

int g_user_set_phy_addr = PHY_ADDR_NOT_SET;
int g_phy_addr_val = INVALID_PHY_ADDR;
int g_user_set_phy_mode = PHY_MODE_NOT_SET;
int g_phy_mode_val = INVALID_PHY_MODE;

static void _eth_drv_init(struct los_eth_driver* sc, unsigned char* mac)
{
    struct eth_drv_sc *drv_sc = (struct eth_drv_sc *)sc->driver_context;

    (drv_sc->funs->eth_drv->init)(sc, mac);
}

static void _eth_drv_recv(struct los_eth_driver *sc, struct pbuf *pbuf)
{
    struct netif *netif = &sc->ac_if;

#if ETH_PAD_SIZE
    /* reclaim the padding word */
    if (pbuf_header(pbuf, ETH_PAD_SIZE)) {
        hieth_debug(LOS_ERR_LEVEL, "pbuf pad error, drop...\n");
        (void)pbuf_free(pbuf);
        return;
    }
#endif

#if !defined(PRODUCT_CFG_FEATURE_REMOTE_COMMUNICATION_TERMINAL)
    driverif_input(netif,pbuf);
#else
    (void)MAC_Eth_Send2Plc(netif, pbuf, (HI_U8*)pbuf->payload+ETH_PAD_SIZE, pbuf->tot_len-ETH_PAD_SIZE);//偏移PAD，使得能传入真正有效数据
#endif

}

#if 1
static eth_phy_access_t hieth_phy = {
    false,
    NULL, /*lint !e64*/
    NULL  
};/*lint !e64*/
#endif

eth_ram_cfg_t hieth_desc_base;

hieth_priv_t hieth_priv_data =
{
   .intr_vector = NUM_HAL_INTERRUPT_ETH,
   .base        = HIETH_MDIO_IO_BASE,
   .phy         = &hieth_phy,
   .ram         = &hieth_desc_base,
   .total_len   = 0,
   .iterator    = 0,
};

struct hieth_netdev_local netdev_local =
{
    .iobase = (char *)HIETH_MDIO_IO_BASE,

    .depth = { .hw_xmitq = CONFIG_HIETH_HWQ_XMIT_DEPTH },
    .q_size = HIETH_HWQ_TXQ_SIZE,

    .mdio_frqdiv = HIETH_MDIO_FRQDIV,
    .tx_busy = 0,
};

void net_dma_cache_inv(void *addr, unsigned int size)
{
    unsigned int start = (uintptr_t)addr & ~(CACHE_ALIGNED_SIZE - 1);
    unsigned int end = (uintptr_t)addr + size;

    end = ALIGN(end, CACHE_ALIGNED_SIZE);

    dma_cache_inv((int)start, (int)end);
}

void net_dma_cache_clean(void *addr, unsigned int size)
{
    unsigned int start = (uintptr_t)addr & ~(CACHE_ALIGNED_SIZE - 1);
    unsigned int end = (uintptr_t)addr + size;

    end = ALIGN(end, CACHE_ALIGNED_SIZE);

    dma_cache_clean((int)start, (int)end);
}

static void hieth_set_mac(hieth_priv_t * priv, UINT8 * enaddr)
{
    (void)hieth_hw_set_macaddress(&netdev_local, 1, enaddr);
}

static void hieth_stop(struct los_eth_driver *sc)
{
}

static void
hieth_start(struct los_eth_driver *sc, unsigned char *enaddr, int flags)
{
}

extern int hieth_xmit_release_pkt(struct hieth_netdev_local *ld,
            struct los_eth_driver *sc,
            struct hieth_priv_s *priv);
            
void hisi_eth_isr(unsigned int data)
{
    unsigned int ints;
    struct los_eth_driver *sc = (struct los_eth_driver *)data;
    struct eth_drv_sc *drv_sc = (struct eth_drv_sc *)sc->driver_context;
    hieth_priv_t *priv = (hieth_priv_t *)drv_sc->driver_private;
    struct hieth_netdev_local *ld = &netdev_local;

    ints = hieth_read_irqstatus(ld);

    if (ints & HIETH_INT_MULTI_RXRDY_U)
    {
        HI_LOW_POWER_UnForbid(HI_LOW_POWER_FORBID_ID_ETH,6000);
        hal_interrupt_mask(priv->intr_vector);
        (void)LOS_EventWrite(&gEthEvent, EVENT_NET_TX_RX);
    }
    
    if (ints & HIETH_INT_TXQUE_RDY_U)
    {
        HI_LOW_POWER_UnForbid(HI_LOW_POWER_FORBID_ID_ETH,2000);
        (void)hieth_xmit_release_pkt(ld,sc,priv);
    }
    
    hieth_clear_irqstatus(ld, ints);
}

void hisi_eth_dsr(struct los_eth_driver *sc)
{
    UINT32 uwRet;
    struct eth_drv_sc *drv_sc = (struct eth_drv_sc *)sc->driver_context;
    struct netif *netif = &sc->ac_if;
    
    for(;;)
    {
        uwRet = LOS_EventRead(&gEthEvent, EVENT_NET_TX_RX, LOS_WAITMODE_OR|LOS_WAITMODE_CLR,200);
        if (uwRet == LOS_ERRNO_EVENT_READ_TIMEOUT)
        {
            phy_state_machine(sc);
        }
        else
        {
            if(uwRet & EVENT_NET_TX_RX)
            {
                if(!netif_is_link_up(netif))
                {
                    phy_state_machine(sc);
                }
                (drv_sc->funs->deliver)(sc); 
            } 
        }
    }
}

#define ETH_STACKSIZE 0x2000
unsigned int create_eth_irq_thread(struct los_eth_driver *sc)
{
    TSK_INIT_PARAM_S stSdTask;
    UINT32 uwRet;
    UINT32 uwEthTask;

    (hi_void)memset_s(&stSdTask, sizeof(TSK_INIT_PARAM_S),  0, sizeof(TSK_INIT_PARAM_S));
    stSdTask.pfnTaskEntry = (TSK_ENTRY_FUNC)hisi_eth_dsr;
    stSdTask.auwArgs[0] = (uintptr_t)sc;
    stSdTask.uwStackSize  = ETH_STACKSIZE;
    stSdTask.pcName = "eth_irq_Task";
    stSdTask.usTaskPrio = 3;
    stSdTask.uwResved   = LOS_TASK_STATUS_DETACHED;
    uwRet = LOS_TaskCreate(&uwEthTask, &stSdTask);

    if(LOS_OK != uwRet)
    {
        return uwRet;
    }
    return 0;
}


#define PHY_ADDR  1

#define ETH_DEFAULT_MACADDR {0x00,0x19,0xb1,0x6c,0xa1,0x25}

#define PHY_ID_INVALID(id)    (((id & 0x1fffffff) == 0x1fffffff) || \
                    ((id & 0xfffffff0) == 0xfffffff0) || \
                    (id == 0) || \
                    (id == 0xffff0000) || \
                    (id == 0x0000ffff))

static unsigned int scan_phy_id(struct hieth_netdev_local *ld, unsigned int addr)
{
    unsigned int phy_id;
    unsigned int val;

    val = (unsigned int)hieth_mdio_read(ld, addr, PHY_ID1);
    phy_id = val << 16;
    val = (unsigned int)hieth_mdio_read(ld, addr, PHY_ID2);
    phy_id |= val;

    return phy_id;
}


static const char *phy_modes(phy_interface_t interface)
{
    switch (interface) {
        case PHY_INTERFACE_MODE_MII:
            return "mii";
        case PHY_INTERFACE_MODE_RMII:
            return "rmii";
        default:
            return "unknown";
    }
}

bool hieth_hw_init(struct los_eth_driver *p_sc)
{
    struct eth_drv_sc *drv_sc = (struct eth_drv_sc *)p_sc->driver_context;
    hieth_priv_t *priv = (hieth_priv_t *)drv_sc->driver_private;
    struct hieth_netdev_local *ld = &netdev_local;

    unsigned char enaddr[6] = ETH_DEFAULT_MACADDR;
    int phy_state = 0;
    unsigned int id = 0;
 
    //2015年2月6日15:07:05
    (hi_void)memset_s(&hieth_desc_base, sizeof(eth_ram_cfg_t), 0, sizeof(eth_ram_cfg_t));

    priv->ram->txq_info = (struct tx_pkt_info*)LOS_MemAlloc((void *)OS_SYS_MEM_ADDR, HIETH_HWQ_TXQ_SIZE * sizeof(struct tx_pkt_info));
    if (!priv->ram->txq_info) {
        hieth_debug(LOS_ERR_LEVEL, "malloc failed\n");
        goto err_out;
    }
    (hi_void)memset_s(priv->ram->txq_info, HIETH_HWQ_TXQ_SIZE * sizeof(struct tx_pkt_info), 0, HIETH_HWQ_TXQ_SIZE * sizeof(struct tx_pkt_info));
    ld->txq = priv->ram->txq_info;

    priv->ram->rx_pbuf = (struct pbuf **)LOS_MemAlloc((void *)OS_SYS_MEM_ADDR, HIETH_HWQ_RXQ_DEPTH * sizeof(struct pbuf *));
    if (!priv->ram->rx_pbuf) {
        hieth_debug(LOS_ERR_LEVEL, "malloc failed\n");
        goto err_out;
    }
    (hi_void)memset_s(priv->ram->rx_pbuf, HIETH_HWQ_RXQ_DEPTH * sizeof(struct pbuf *), 0, HIETH_HWQ_RXQ_DEPTH * sizeof(struct pbuf *));

    priv->ram->dma_tx = (struct dma_tx_desc*)LOS_MemAlloc((void *)OS_SYS_MEM_ADDR, HIETH_HWQ_TXQ_SIZE * sizeof(struct dma_tx_desc));
    if (!priv->ram->dma_tx) {
        hieth_debug(LOS_ERR_LEVEL, "malloc failed\n");
        goto err_out;
    }
    (hi_void)memset_s(priv->ram->dma_tx, HIETH_HWQ_TXQ_SIZE * sizeof(struct dma_tx_desc), 0, HIETH_HWQ_TXQ_SIZE * sizeof(struct dma_tx_desc));

    priv->ram->pbuf_info = (struct pbuf_info*)LOS_MemAlloc((void *)OS_SYS_MEM_ADDR,HIETH_HWQ_TXQ_SIZE * sizeof(struct pbuf_info));
    if (!priv->ram->pbuf_info) {
        hieth_debug(LOS_ERR_LEVEL, "malloc failed\n");
        goto err_out;
    }
    (hi_void)memset_s(priv->ram->pbuf_info, HIETH_HWQ_TXQ_SIZE * sizeof(struct pbuf_info), 0, HIETH_HWQ_TXQ_SIZE * sizeof(struct pbuf_info));

    HISI_NET_LOCK_INIT(&hieth_glb_reg_lock);

    hieth_hw_mac_core_reset(ld);
    hieth_hw_mac_core_init(ld);

    if (g_user_set_phy_mode == PHY_MODE_SET) {
        priv->phy->phy_mode = g_phy_mode_val;
        hieth_debug(LOS_INFO_LEVEL, "User set phy mode=%s\n",
            phy_modes(priv->phy->phy_mode));
    } else {
        priv->phy->phy_mode = PHY_INTERFACE_MODE_RMII;
        hieth_debug(LOS_INFO_LEVEL, "Auto set phy mode=%s\n",
            phy_modes(priv->phy->phy_mode));
    }

    if (!priv->phy->init_done) {
        hieth_hw_external_phy_reset();

        priv->phy->init_done = true;

    if (g_user_set_phy_addr == PHY_ADDR_SET) {
        priv->phy->phy_addr = g_phy_addr_val;

        id = scan_phy_id(ld, (unsigned int)priv->phy->phy_addr);
        if (PHY_ID_INVALID(id)) {
            hieth_debug(LOS_ERR_LEVEL, "Can't find PHY device - id: %x\n", id);
            priv->phy->init_done = false;
            goto err_out;
        }
    } else {
        int addr;
        hieth_debug(LOS_INFO_LEVEL, "User did not set phy addr, auto scan...\n");
        for (addr = MAX_PHY_ADDR; addr >= 0; addr--) {
            id = scan_phy_id(ld, (unsigned int)addr);
            if (PHY_ID_INVALID(id))
                continue;
            break;
        }

        if (addr < 0) {
            hieth_debug(LOS_ERR_LEVEL, "Can't find PHY device - id: %x\n", id);
            priv->phy->init_done = false;
            goto err_out;
        }

        priv->phy->phy_addr = addr;
    }
        ld->phy_id = id;
    }

    hieth_debug(LOS_INFO_LEVEL, "Detected phy addr %d, phyid: 0x%x\n", priv->phy->phy_addr, ld->phy_id);
    if (!priv->phy->init_done) {
        hieth_debug(LOS_ERR_LEVEL, "PHY state without init on PHY: %x\n", priv->phy);
        goto err_out;
    }

    (void)hieth_get_phy_stat(priv->phy, &phy_state);

    if (0 != LOS_HwiCreate(priv->intr_vector, 0, 0, (HWI_PROC_FUNC)hisi_eth_isr, (HWI_ARG_T)p_sc))
    {
        hieth_debug(LOS_ERR_LEVEL, "LOS_HwiCreate error\n");
        goto err_out;
    }
    
    hal_interrupt_unmask(priv->intr_vector);

    (void)create_eth_irq_thread(p_sc);
   
    // Initialize the upper layer driver
    _eth_drv_init(p_sc, enaddr);

    (void)hieth_hw_set_macaddress(ld, 1, enaddr);

    // clear all interrupts
    hieth_clear_irqstatus(ld, UD_BIT_NAME(BITS_IRQS_MASK));

    priv->rx_feed = 0;
    priv->rx_release = 0;
    (void)hieth_feed_hw(ld, priv);
    (void)hieth_irq_enable(ld, UD_BIT_NAME(HIETH_INT_MULTI_RXRDY)|UD_BIT_NAME(HIETH_INT_TXQUE_RDY));
    hieth_writel_bits(ld, 1, GLB_RW_IRQ_ENA, UD_BIT_NAME(BITS_IRQS_ENA));
    hieth_writel_bits(ld, 1, GLB_RW_IRQ_ENA, BITS_IRQS_ENA_ALLPORT);

    return true;
err_out:
    return false;
}

//===========================================================================
// This function is called for low level "control" operations
//===========================================================================
static int
hieth_control(struct los_eth_driver *sc, unsigned long key,
                    void *data, int length)
{
    struct eth_drv_sc *drv_sc = (struct eth_drv_sc *)sc->driver_context;
    hieth_priv_t *priv = (hieth_priv_t *)drv_sc->driver_private;
    UINT8 * enaddr = (UINT8 *)data;
    
    switch (key)
    {
        case ETH_DRV_SET_MAC_ADDRESS:
        {
            if(length >= ETHER_ADDR_LEN)
            {
                hieth_stop(sc);

                hieth_debug(LOS_INFO_LEVEL, "requested MAC address: %02x:%02x:%02x:%02x:%02x:%02x\n",
                             enaddr[0],enaddr[1],enaddr[2],
                             enaddr[3],enaddr[4],enaddr[5]);

                hieth_set_mac(priv, enaddr);
                hieth_start(sc,enaddr,0);
                return 0;
            }
            return 1;
        }
        default:
        {
            hieth_debug(LOS_INFO_LEVEL, "key %lx\n", key);
            return (1);
        }
    } // switch (key)
}



static int hieth_can_send(struct los_eth_driver *sc)
{
    int can_send;
    struct eth_drv_sc *drv_sc = (struct eth_drv_sc *)sc->driver_context;
    hieth_priv_t *priv = (hieth_priv_t *)drv_sc->driver_private;
    struct hieth_netdev_local *ld = &netdev_local;

    (void)hieth_xmit_release_pkt(ld, sc, priv);

    HISI_NET_LOCK_GET(&(ld->tx_lock));
    if (!_test_xmit_queue_ready(ld) ||
    ((ld->txq_head + 1) % (unsigned int)ld->q_size) == ld->txq_tail) {
        UINT32 uwRet;
        can_send = 0;
        ld->tx_busy = 1;
        HISI_NET_LOCK_PUT(&(ld->tx_lock));
        hal_interrupt_unmask(priv->intr_vector);

        uwRet = LOS_EventRead(&gEthEvent, EVENT_NET_CAN_SEND, LOS_WAITMODE_OR | LOS_WAITMODE_CLR, LOS_WAIT_FOREVER);
        if(uwRet & EVENT_NET_CAN_SEND)
        {
            can_send = 1;
            hal_interrupt_mask(priv->intr_vector);
            HISI_NET_LOCK_GET(&(ld->tx_lock));
        }
    } else {
        can_send = 1;
    }
    HISI_NET_LOCK_PUT(&(ld->tx_lock));

    return can_send;
}

extern int hieth_xmit_gso(struct hieth_netdev_local *ld,
        hieth_priv_t *priv, struct pbuf *p);

static void
hieth_send(struct los_eth_driver *sc, struct pbuf *p)
{
    struct eth_drv_sc *drv_sc = (struct eth_drv_sc *)sc->driver_context;
    hieth_priv_t *priv = (hieth_priv_t *)drv_sc->driver_private;
    struct hieth_netdev_local *ld = &netdev_local;
    
    HI_LOW_POWER_UnForbid(HI_LOW_POWER_FORBID_ID_ETH,2000);
    HISI_NET_LOCK_GET(&(ld->tx_lock));
        /* TSO supported */
    (void)hieth_xmit_gso(ld, priv, p);

    ld->tx_hw_cnt++;
    HISI_NET_LOCK_PUT(&(ld->tx_lock));
}

static void hieth_deliver(struct los_eth_driver *sc)
{
    struct eth_drv_sc *drv_sc = (struct eth_drv_sc *)sc->driver_context;
    hieth_priv_t *priv = (hieth_priv_t *)(drv_sc->driver_private);
    struct hieth_netdev_local *ld = &netdev_local;
    unsigned int rx_pkt_info = 0;
    unsigned int rlen = 0;
    struct pbuf *pbuf;

    HISI_NET_LOCK_GET(&(ld->rx_lock));
    while (is_recv_packet(ld)) {
        rx_pkt_info = hw_get_rxpkg_info(ld);
        rlen = (rx_pkt_info >> BITS_RXPKG_LEN_OFFSET)
            & BITS_RXPKG_LEN_MASK;
        rlen -= 4;
        if (rlen > HIETH_MAX_FRAME_SIZE)
        {
            hieth_debug(LOS_ERR_LEVEL, "recv over size frame,%d.\n", rlen);
        }            

        HISI_NET_LOCK_GET(&hieth_glb_reg_lock);
        hw_set_rxpkg_finish(ld);
        HISI_NET_LOCK_PUT(&hieth_glb_reg_lock);

        if (priv->rx_release == priv->rx_feed)
        {
            hieth_debug(LOS_ERR_LEVEL, "recv fifo overflow. rx_release=%d, rx_feed=%d\n", priv->rx_release, priv->rx_feed);
        }

        pbuf = priv->ram->rx_pbuf[priv->rx_release];
        pbuf->len = pbuf->tot_len = (unsigned short)rlen;

        _eth_drv_recv(sc, pbuf);
        priv->ram->rx_pbuf[priv->rx_release] = NULL;
        priv->rx_release = (priv->rx_release + 1) % HIETH_HWQ_RXQ_DEPTH;
    }
    HISI_NET_LOCK_PUT(&(ld->rx_lock));

    (void)hieth_feed_hw(ld, priv);

    hal_interrupt_unmask(priv->intr_vector);
#ifdef INT_IO_ETH_INT_SUPPORT_REQUIRED
    drv_interrupt_unmask(priv->intr_vector);
#endif
}

static void hieth_poll(struct los_eth_driver *sc)
{
    hieth_deliver(sc);
}


static int hieth_int_vector(struct los_eth_driver *sc)
{
    return NUM_HAL_INTERRUPT_ETH;
}

static const char *phy_speed_to_str(int speed)
{
    switch (speed) {
    case SPEED_10:
        return "10Mbps";
    case SPEED_100:
        return "100Mbps";
    case SPEED_1000:
        return "1Gbps";
    case SPEED_2500:
        return "2.5Gbps";
    case SPEED_10000:
        return "10Gbps";
    case SPEED_UNKNOWN:
        return "Unknown";
    default:
        return "Unsupported (update phy.c)";
    }
}

void hieth_link_status_changed(struct netif *netif)
{
    struct los_eth_driver *sc = (struct los_eth_driver *)netif->state;
    struct eth_drv_sc *drv_sc = (struct eth_drv_sc *)sc->driver_context;
    hieth_priv_t *priv = (hieth_priv_t *)drv_sc->driver_private;
    struct hieth_netdev_local *ld = &netdev_local;
    unsigned int val = 0;

    int phy_mode = 0;
    int duplex;
    int speed;

    duplex = miiphy_duplex(priv->phy);
    speed = miiphy_speed(priv->phy);

    if (netif_is_link_up(netif))
        val |= HIETH_LINKED;

    if (duplex)
        val |= HIETH_DUP_FULL;

    if (speed == SPEED_100)
        val |= HIETH_SPD_100M;

    switch (priv->phy->phy_mode) {
    case PHY_INTERFACE_MODE_MII:
        phy_mode = HIETH_PHY_MII_MODE;
        break;
    case PHY_INTERFACE_MODE_RMII:
        phy_mode = HIETH_PHY_RMII_MODE;
        break;
    default:
        hieth_debug(LOS_INFO_LEVEL, "not supported mode: %d.\n", 
            priv->phy->phy_mode);
        break;
    }

    (void)hieth_set_linkstat(ld, val);
    (void)hieth_set_mii_mode(ld, (unsigned int )phy_mode);

    if (netif_is_link_up(netif))
    {
        printf("Link is Up - %s/%s\n",
            phy_speed_to_str(speed),
            DUPLEX_FULL == duplex ? "Full" : "Half");
    }
    else
    {
        printf("Link is Down\n");
    }
}

void phy_state_machine(struct los_eth_driver * arg)
{
    struct los_eth_driver *sc = arg;
    struct netif *netif = &sc->ac_if;
    struct eth_drv_sc *drv_sc = (struct eth_drv_sc *)sc->driver_context;
    hieth_priv_t *priv = (hieth_priv_t *)drv_sc->driver_private;

    int link_status;

    link_status = miiphy_link(priv->phy);    
    if (netif_is_link_up(netif) && !link_status)
    {
        netif_set_link_down(netif);
    }
    else if (!netif_is_link_up(netif) && link_status)
    {   
        netif_set_link_up(netif);
    }
}

u8_t hieth_set_hwaddr(struct netif *netif, u8_t *addr, u8_t len)
{
    if (is_multicast_ether_addr(addr)) {
        return (u8_t)-1;
    }

    if (len != ETHER_ADDR_LEN) {
        return (u8_t)-1;
    }

    (void)hieth_hw_set_macaddress(&netdev_local, 1, addr);

    (hi_void)memcpy_s(netif->hwaddr,NETIF_MAX_HWADDR_LEN,addr, len);
    return 0;
}
#if PF_PKT_SUPPORT
void hieth_config_mode(struct netif *netif, uint32_t config_flags, u8_t setBit)
{    struct hieth_netdev_local *ld = &netdev_local;
    if (config_flags & IFF_PROMISC)
        hieth_writel_bits(ld, !!setBit, GLB_FWCTRL,
            UD_BIT(ld->port, BITS_FWALL2CPU));
    if (config_flags & IFF_ALLMULTI)
        hieth_writel_bits(ld, !!setBit, GLB_MACTCTRL,
            UD_BIT(ld->port, BITS_MULTI2CPU));
}
#endif
struct los_eth_driver hisi_eth_drv_sc;

int hisi_eth_set_phy_mode(const char *phy_mode)
{
    int i;

    for (i = 0; i < PHY_INTERFACE_MODE_MAX; i++)
        if (!strcasecmp(phy_mode, phy_modes((phy_interface_t)i))) {
        g_user_set_phy_mode = PHY_MODE_SET;
        g_phy_mode_val = i;
            return 0;
        }

    return -1;
}

int hisi_eth_set_phy_addr(unsigned int phy_addr)
{
    if (phy_addr > MAX_PHY_ADDR) {
        return -1;
    }

    g_user_set_phy_addr = PHY_ADDR_SET;
    g_phy_addr_val = (int)phy_addr;

    return 0;
}

void hisi_eth_init(void)
{
    static int _init = FALSE;

    hieth_drv_sc_init(&hisi_eth_drv_sc, &hieth_priv_data, "eth1",
        hieth_start, hieth_stop, hieth_control, hieth_can_send,
        hieth_send, (eth_recv)NULL, hieth_deliver, hieth_poll, hieth_int_vector);

    if (_init)
        return;
    (void)LOS_EventInit(&gEthEvent);
    (void)hieth_hw_init(&hisi_eth_drv_sc);

    // Done
    _init = TRUE;
    (void)netif_set_up(&(hisi_eth_drv_sc.ac_if));
}

unsigned int hisi_eth_phy_powerdown(void)//静默状态下，约有10mw功耗收益。
{
    int val = 0;
    int ret = 0;
    struct hieth_netdev_local *ld = &netdev_local;
    
    val = hieth_mdio_read(ld, 1, PHY_BMCR);
    if(val < 0)
    {
        return  (unsigned int)(-1);
    }
    val = (HI_U32)val | BMCR_POWER_DOWN;
    ret = hieth_mdio_write(ld, 1, PHY_BMCR,val);
    if(ret < 0)
    {
        return  (unsigned int)(-1);
    }
    return 0;
}

#define ETH_MIN_FEED_QUEUE_NUM      3

void hieth_reinit_hw_queue(void)
{
    struct eth_drv_sc *drv_sc = (struct eth_drv_sc *)hisi_eth_drv_sc.driver_context;
    hieth_priv_t *priv = (hieth_priv_t *)drv_sc->driver_private;
    struct hieth_netdev_local *ld = &netdev_local;   
    struct pbuf *pbuf;
    unsigned char b_rx_buf_null = HI_FALSE; 
    unsigned int i = 0;
    unsigned int j = 0;
    unsigned int cnt;
    unsigned int feed_cnt;
 
    feed_cnt = HI_MIN((HIETH_MAX_QUEUE_DEPTH-CONFIG_HIETH_HWQ_XMIT_DEPTH-hw_recv_cnt_inuse(ld)), ETH_MIN_FEED_QUEUE_NUM);    
    i = priv->rx_release;//i为新feed
    for(cnt=0; cnt < feed_cnt; cnt++)
    {
        if(!priv->ram->rx_pbuf[i])//旧队列不足
        {    
            b_rx_buf_null = HI_TRUE;
            break;
        }
        pbuf = priv->ram->rx_pbuf[i];  
        net_dma_cache_inv(pbuf->payload, HIETH_MAX_FRAME_SIZE);
        hieth_writel(ld, (unsigned int)pbuf->payload,
                 UD_REG_NAME(GLB_IQ_ADDR));  
        i = (i+1)%HIETH_HWQ_RXQ_DEPTH;        
    }
    if(HI_TRUE == b_rx_buf_null)
    {
       (void)malloc_new_mem(ld, priv, feed_cnt-cnt);//目标feed队列个数-已经接过来个数
    }
    else
    {      
        priv->rx_feed = i;//若原来队列大于目标feed个数，其余等待释放 
        for(j=priv->rx_feed; priv->ram->rx_pbuf[j]!=NULL; j = (j+1)%HIETH_HWQ_RXQ_DEPTH)
        {
            pbuf_free(priv->ram->rx_pbuf[j]);
            priv->ram->rx_pbuf[j] = NULL;
        }
    }
}



void hisi_eth_prepare_reset(void)
{
    struct hieth_netdev_local *ld = &netdev_local;
    unsigned int ulRegVal = 0;

    hieth_writel_bits(ld, 0, U_MAC_PORTSET,BITS_LINK_STATUS_DIO);/*lint !e514*/   //置为无连接状态  0x10180208 = 0X04   
    hieth_writel_bits(ld, 0, GLB_RW_IRQ_ENA, BITS_MUTI_RX_INT);/*lint !e514*/   //多包中断禁止  0x10181334 = 0X40

    HI_REG_READ32(HI_SYSCTRL_REG_BASE+SC_PERI_SRST, ulRegVal);//bit 2
    ulRegVal |= HI_SYSCTRL_SRST_REG_BIT_SHIFT_ETH;
    HI_REG_WRITE32(HI_SYSCTRL_REG_BASE+SC_PERI_SRST, ulRegVal);//bit 3

    HI_REG_READ32(HI_SYSCTRL_REG_BASE+SC_PERI_CLKEN, ulRegVal);//bit 2
    ulRegVal &= ~HI_SYSCTRL_CLKEN_REG_BIT_SHIFT_ETH;
    ulRegVal &= ~HI_SYSCTRL_CLKEN_REG_BIT_SHIFT_MII;
    HI_REG_WRITE32(HI_SYSCTRL_REG_BASE+SC_PERI_CLKEN, ulRegVal);    
}

void hisi_eth_finish_reset(void)
{
    struct hieth_netdev_local *ld = &netdev_local;
    unsigned int ulRegVal = 0;
 
    HI_REG_READ32(HI_SYSCTRL_REG_BASE+SC_PERI_CLKEN, ulRegVal);//bit 2
    ulRegVal |= HI_SYSCTRL_CLKEN_REG_BIT_SHIFT_ETH;
    ulRegVal |= HI_SYSCTRL_CLKEN_REG_BIT_SHIFT_MII;
    HI_REG_WRITE32(HI_SYSCTRL_REG_BASE+SC_PERI_CLKEN, ulRegVal);

    HI_REG_READ32(HI_SYSCTRL_REG_BASE+SC_PERI_SRST, ulRegVal);//bit 2
    ulRegVal &= ~HI_SYSCTRL_SRST_REG_BIT_SHIFT_ETH;
    HI_REG_WRITE32(HI_SYSCTRL_REG_BASE+SC_PERI_SRST, ulRegVal);//bit 3

    hieth_writel_bits(ld, 1, GLB_RW_IRQ_ENA, BITS_MUTI_RX_INT);/*lint !e514*/   //多包中断使能  0x10181334 = 0Xc0
    hieth_writel_bits(ld, 1, U_MAC_PORTSET,BITS_LINK_STATUS_DIO);/*lint !e514*/   //置为有连接状态  0x10180208

    hieth_reinit_hw_queue();
}

/*******************************************************************
*函数功能:将组帧好的数据通过网口发送出去
*说明:入参pData要调用者保证释放
*
********************************************************************/
unsigned int hisi_eth_send_data(const void *pData,HI_U16 usLen)
{
    struct netif *netif = &hisi_eth_drv_sc.ac_if;
    struct pbuf *pbuf;

    if ((usLen > HIETH_MAX_FRAME_SIZE) || (usLen==0) || (pData == HI_NULL))
    {
        return HI_ERR_INVALID_PARAMETER;
    }
    pbuf = pbuf_alloc(PBUF_TRANSPORT, usLen, PBUF_RAM);//是PBUF_TRANSPORT吗?
    if(!pbuf)
    {
        return HI_ERR_FAILURE;
    }

    (hi_void)memcpy_s(pbuf->payload, usLen, pData, usLen);

    netif->drv_send(netif, pbuf);//hieth_send 

    pbuf_free(pbuf);
    return HI_ERR_SUCCESS;
}

void hisi_eth_pkt_free(void *pbuf)
{
    pbuf_free(pbuf);
}

void hisi_eth_switch_send2lwip_stack(void *netif, void *pbuf)
{ 
    driverif_input(netif,pbuf);
}

