#include "eth_drv.h"
#include "string.h"
#include "hisi_mac.h"
#include <hi_types.h>
#include <hi_stdlib.h>

// Interfaces exported to drivers
void eth_drv_send(struct netif *netif, struct pbuf *p);

extern err_t dhcp_start(struct netif *netif);
err_t lwip_dhcp_init(struct netif *netif)
{
    err_t ret;
    ret = dhcp_start(netif);
    
    return ret;
}

// This function is called during system initialization to register a
// network interface with the system.
static void
eth_drv_init(struct los_eth_driver *sc, unsigned char *enaddr)
{
    struct netif *netif = &sc->ac_if;
    struct eth_drv_sc *drv_sc = (struct eth_drv_sc*)sc->driver_context;
    ip_addr_t ipaddr, netmask, gw;

    //RFC3927
    IP4_ADDR(&gw, 169, 254, 1, 1);
    IP4_ADDR(&ipaddr, 169, 254, 1, 2);
    IP4_ADDR(&netmask, 255, 255, 0, 0);

    netif->state = sc;
    netif->drv_send = eth_drv_send;
    netif->drv_set_hwaddr = hieth_set_hwaddr;
    (hi_void)snprintf_s(netif->name, IFNAMSIZ, IFNAMSIZ - 1, "%s", "eth0");
#if PF_PKT_SUPPORT
    netif->drv_config = hieth_config_mode;
#endif
    netif->link_layer_type = ETHERNET_DRIVER_IF;
    netif->hwaddr_len = ETHARP_HWADDR_LEN;
    netif->flags |= NETIF_FLAG_UP | NETIF_FLAG_BROADCAST | NETIF_FLAG_ETHARP;
    if(NULL == netif_add(netif, &ipaddr, &netmask, &gw))
    {
        hieth_debug(LOS_ERR_LEVEL, "Add netif failed\n");
        return;
    }

    netif_set_default(netif);

    if (enaddr != 0)
    {
        // Set up hardware address
        (hi_void)memcpy_s(netif->hwaddr,NETIF_MAX_HWADDR_LEN,enaddr, ETHER_ADDR_LEN);
        // Perform any hardware initialization
        (drv_sc->funs->start) (sc, (unsigned char *)netif->hwaddr, 0);
    }

    netif_set_link_down(netif);
    netif_set_link_callback(netif, hieth_link_status_changed);
}

void
eth_drv_send(struct netif *netif, struct pbuf *p)
{
    struct los_eth_driver *sc = (struct los_eth_driver *)netif->state;
    struct eth_drv_sc *drv_sc = (struct eth_drv_sc*)sc->driver_context;
    hieth_priv_t *priv = (hieth_priv_t *)drv_sc->driver_private;

    hal_interrupt_mask(priv->intr_vector);

    while (!(drv_sc->funs->can_send) (sc))
        ;

    (drv_sc->funs->send) (sc, p);

    hal_interrupt_unmask(priv->intr_vector);
}

struct los_eth_funs eth_drv_funs_hieth = { eth_drv_init, NULL, NULL }; /*lint !e64*/

void hieth_drv_sc_init(struct los_eth_driver* sc, void* priv, const char* name,
    eth_start start, eth_stop stop, eth_control control, eth_can_send can_send,
    eth_send send_f, eth_recv recv_f, eth_deliver deliver, eth_poll poll, eth_int_vector int_vector)
{
    struct eth_hwr_funs *sc_funs;
    struct eth_drv_sc *drv_sc;

    sc_funs = (struct eth_hwr_funs *)LOS_MemAlloc((void *)OS_SYS_MEM_ADDR, sizeof(struct eth_hwr_funs));
    if(NULL == sc_funs)
    {
        hieth_debug(LOS_ERR_LEVEL, "malloc sc_funs failed\n");
        return;
    }

    drv_sc = (struct eth_drv_sc *)LOS_MemAlloc((void *)OS_SYS_MEM_ADDR, sizeof(struct eth_drv_sc));
    if(NULL == drv_sc)
    {
        hieth_debug(LOS_ERR_LEVEL, "malloc drv_sc failed\n");
        return;
    }

    sc_funs->start = start;
    sc_funs->stop = stop;
    sc_funs->control = control;
    sc_funs->can_send = can_send;
    sc_funs->send = send_f;
    sc_funs->recv = recv_f;
    sc_funs->deliver = deliver;
    sc_funs->poll = poll;
    sc_funs->int_vector = int_vector;
    sc_funs->eth_drv = &eth_drv_funs_hieth;
    sc_funs->eth_drv_old = (struct los_eth_funs *)0;
    drv_sc->funs = sc_funs;
    drv_sc->driver_private = priv;
    drv_sc->dev_name = name;
    sc->driver_context = drv_sc;
}


