    /**
 * @file
 * Ethernet Interface Skeleton
 *
 */

/*
 * Copyright (c) 2001-2004 Swedish Institute of Computer Science.
 * All rights reserved.
 * Copyright (c) <2013-2015>, <Huawei Technologies Co., Ltd>
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 * 3. The name of the author may not be used to endorse or promote products
 *    derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT
 * SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT
 * OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING
 * IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY
 * OF SUCH DAMAGE.
 *
 * This file is part of the lwIP TCP/IP stack.
 *
 * Author: Adam Dunkels <adam@sics.se>
 *
 */

 /**********************************************************************************
 * Notice of Export Control Law
 * ===============================================
 * Huawei LiteOS may be subject to applicable export control laws and regulations, which
 * might include those applicable to Huawei LiteOS of U.S. and the country in which you
 * are located.
 * Import, export and usage of Huawei LiteOS in any manner by you shall be in compliance
 * with such applicable export control laws and regulations.
 **********************************************************************************/

#include "lwip/opt.h"
#include "lwip/def.h"
#include "lwip/mem.h"
#include "lwip/pbuf.h"
#include "lwip/dhcp.h"
#include "lwip/stats.h"
#include "lwip/snmp.h"

#include "netif/etharp.h"
#if PPPOE_SUPPORT
#include "netif/ppp_oe.h"
#endif
#include "netif/driverif.h"




/**
 * This function should do the actual transmission of the packet. The packet is
 * contained in the pbuf that is passed to the function. This pbuf
 * might be chained.
 *
 * @param netif the lwip network interface structure for this driverif
 * @param p the MAC packet to send (e.g. IP packet including MAC_addresses and type)
 * @return ERR_OK if the packet could be sent
 *         an err_t value if the packet couldn't be sent
 *
 * @note Returning ERR_MEM here if a DMA queue of your MAC is full can lead to
 *       strange results. You might consider waiting for space in the DMA queue
 *       to become availale since the stack doesn't retry to send a packet
 *       dropped because of memory failure (except for the TCP timers).
 */

LWIP_STATIC err_t
driverif_output(struct netif *netif, struct pbuf *p)
{
  LWIP_DEBUGF(DRIVERIF_DEBUG, ("driverif_output : going to send packet pbuf 0x%p of length %"U16_F" through netif 0x%p\n", \
    (void *)p, p->tot_len, (void *)netif));
#if ETH_PAD_SIZE
  (void)pbuf_header(p, -ETH_PAD_SIZE); /* drop the padding word */
#endif

  netif->drv_send(netif, p);

#if ETH_PAD_SIZE
  (void)pbuf_header(p, ETH_PAD_SIZE); /* reclaim the padding word */
#endif

  LINK_STATS_INC(link.xmit);

  return ERR_OK;
}

/**
 * This function should be called by network driver to pass the input packet to LwIP.
 * Before calling this API, driver has to keep the packet in pbuf structure. Driver has to
 * call pbuf_alloc() with type as PBUF_RAM to create pbuf structure. Then driver
 * has to pass the pbuf structure to this API. This will add the pbuf into the TCPIP thread.
 * Once this packet is processed by TCPIP thread, pbuf will be freed. Driver is not required to
 * free the pbuf.
 *
 * @param netif the lwip network interface structure for this driverif
 * @param p packet in pbuf structure format
 */
void
driverif_input(struct netif *netif, struct pbuf *p)
{
  u16_t ethhdr_type;
  struct eth_hdr* ethhdr;
  err_t ret = ERR_VAL;

  LWIP_ERROR("driverif_input : invalid arguments", ( (netif != NULL) && (p != NULL) ), return;);

  LWIP_DEBUGF(DRIVERIF_DEBUG, ("driverif_input : going to receive input packet. netif 0x%p, pbuf 0x%p, packet_length %"U16_F"\n", \
    (void *)netif, (void *)p, p->tot_len));

  /* points to packet payload, which starts with an Ethernet header */
  ethhdr = (struct eth_hdr *)p->payload;
  ethhdr_type = ntohs(ethhdr->type);

#if PF_PKT_SUPPORT
#if  !(DRIVERIF_DEBUG & LWIP_DBG_OFF)
  LWIP_UNUSED_ARG(ethhdr_type);
#endif
  LWIP_DEBUGF(DRIVERIF_DEBUG, ("driverif_input : received packet of type %"U16_F"\n", ethhdr_type));
  /* full packet send to tcpip_thread to process */
  if (netif->input) {
    ret = netif->input(p, netif);
  }

  if (ret != ERR_OK) {
     LWIP_DEBUGF(DRIVERIF_DEBUG, ("driverif_input: IP input error\n"));
     pbuf_free(p);
     LINK_STATS_INC(link.drop);
     LINK_STATS_INC(link.link_rx_drop);
     if (ret == ERR_MEM)
     {
          LINK_STATS_INC(link.link_rx_overrun);
     }
   } else {
     LINK_STATS_INC(link.recv);
   }

#else
  switch (ethhdr_type) {
  /* IP or ARP packet? */
  case ETHTYPE_IP:
  case ETHTYPE_ARP:
#if PPPOE_SUPPORT
  /* PPPoE packet? */
  case ETHTYPE_PPPOEDISC:
  case ETHTYPE_PPPOE:
#endif /* PPPOE_SUPPORT */
    LWIP_DEBUGF(DRIVERIF_DEBUG, ("driverif_input : received packet of type %"U16_F"\n", ethhdr_type));
    /* full packet send to tcpip_thread to process */

    if (netif->input) {
      ret = netif->input(p, netif);
    }

    if (ret != ERR_OK) {
       LWIP_DEBUGF(DRIVERIF_DEBUG, ("driverif_input: IP input error\n"));
       pbuf_free(p);
       LINK_STATS_INC(link.drop);
       LINK_STATS_INC(link.link_rx_drop);
       if (ret == ERR_MEM)
       {
            LINK_STATS_INC(link.link_rx_overrun);
       }
     } else {
       LINK_STATS_INC(link.recv);
     }
    break;

  default:
    LWIP_DEBUGF(DRIVERIF_DEBUG, ("driverif_input : received packet is of unsupported type %"U16_F"\n", ethhdr_type));
    pbuf_free(p);
    LINK_STATS_INC(link.drop);
    LINK_STATS_INC(link.link_rx_drop);
    break;
  }
#endif
  LWIP_DEBUGF(DRIVERIF_DEBUG, ("driverif_input : received packet is processed\n"));
}

/**
 * Should be called at the beginning of the program to set up the
 * network interface. It calls the function low_level_init() to do the
 * actual setup of the hardware.
 *
 * This function should be passed as a parameter to netif_add().
 *
 * @param netif the lwip network interface structure for this driverif
 * @return ERR_OK if the loopif is initialized
 *         ERR_MEM on Allocation Failure
 *         any other err_t on error
 */
err_t
driverif_init(struct netif *netif)
{
  u16_t link_layer_type;
  
  link_layer_type = netif->link_layer_type;
  LWIP_ERROR("driverif_init : invalid link_layer_type in netif", \
    ((link_layer_type == ETHERNET_DRIVER_IF) || (link_layer_type == WIFI_DRIVER_IF)), \
    return ERR_IF;);

  LWIP_ERROR("driverif_init : netif hardware length is greater than maximum supported", \
    (netif->hwaddr_len <= NETIF_MAX_HWADDR_LEN), return ERR_IF;);

  LWIP_ERROR("driverif_init : drv_send is null", (netif->drv_send != NULL), return ERR_IF;);
#if LWIP_NETIF_PROMISC
  LWIP_ERROR("driverif_init : drv_config is null", (netif->drv_config != NULL), return ERR_IF;);
#endif


#if LWIP_NETIF_HOSTNAME
  /* Initialize interface hostname */
  netif->hostname = "lwip";
#endif /* LWIP_NETIF_HOSTNAME */

  /*
   * Initialize the snmp variables and counters inside the struct netif.
   * The last argument should be replaced with your link speed, in units
   * of bits per second.
   */
  NETIF_INIT_SNMP(netif, snmp_ifType_ethernet_csmacd, LINK_SPEED_OF_YOUR_NETIF_IN_BPS);
    if (!netif->output)
    {
        netif->output = etharp_output;
    }

    if(!netif->linkoutput)
    {
        netif->linkoutput = driverif_output;
    }
    
    if (strlen(netif->name) == 0) 
    {
        if (ETHERNET_DRIVER_IF == link_layer_type) {
            (void)SNPRINTF_S(netif->name, IFNAMSIZ, IFNAMSIZ-1, "%s%u", ETHERNET_IFNAME, netif->num);
        } else if (WIFI_DRIVER_IF == link_layer_type) {
            (void)SNPRINTF_S(netif->name, IFNAMSIZ, IFNAMSIZ-1, "%s%u", WIFI_IFNAME, netif->num);
        } else {
            (void)SNPRINTF_S(netif->name, IFNAMSIZ, IFNAMSIZ-1, "%s%u", PLC_IFNAME, netif->num);
        }
    }

  /* maximum transfer unit */
    if(!netif->mtu)
    {
        netif->mtu = IP_FRAG_MAX_MTU;
    }

  LWIP_DEBUGF(DRIVERIF_DEBUG, ("driverif_init : Initialized netif 0x%p\n", (void *)netif));
  return ERR_OK;
}


