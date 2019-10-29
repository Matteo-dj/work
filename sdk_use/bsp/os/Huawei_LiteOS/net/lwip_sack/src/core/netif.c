/**
 * @file
 * lwIP network interface abstraction
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
#include "lwip/ip_addr.h"
#include "lwip/netif.h"
#include "lwip/tcp_impl.h"
#include "lwip/snmp.h"
#include "lwip/igmp.h"
#include "netif/etharp.h"
#include "lwip/stats.h"
#if ENABLE_LOOPBACK
#include "lwip/sys.h"
#endif /* ENABLE_LOOPBACK */
#include "lwip/tcpip.h"

#if LWIP_NETIF_PROMISC
#include "netif/ifaddrs.h"
#include "lwip/raw.h"
#endif

#if LWIP_AUTOIP
#include "lwip/autoip.h"
#endif /* LWIP_AUTOIP */
#if LWIP_DHCP
#include "lwip/dhcp.h"
#endif /* LWIP_DHCP */


#include "netif/driverif.h"

#if LWIP_NETIF_STATUS_CALLBACK
#define NETIF_STATUS_CALLBACK(n) do{ if (n->status_callback) { (n->status_callback)(n); }}while(0)
#else
#define NETIF_STATUS_CALLBACK(n)
#endif /* LWIP_NETIF_STATUS_CALLBACK */

#if LWIP_NETIF_LINK_CALLBACK
#define NETIF_LINK_CALLBACK(n) do{ if (n->link_callback) { (n->link_callback)(n); }}while(0)
#else
#define NETIF_LINK_CALLBACK(n)
#endif /* LWIP_NETIF_LINK_CALLBACK */

struct netif *netif_list = NULL;
struct netif *netif_default = NULL;

#if LWIP_NETIF_PROMISC
void Stub_drv_config(struct netif *netif, u32_t config_flags, u8_t setBit)
{
  return;
}
#endif

#if LWIP_HAVE_LOOPIF
static struct netif loop_netif;

/**
 * Initialize a lwip network interface structure for a loopback interface
 *
 * @param netif the lwip network interface structure for this loopif
 * @return ERR_OK if the loopif is initialized
 *         ERR_MEM Allocation Failure
 */
LWIP_STATIC err_t
netif_loopif_init(struct netif *netif)
{
  /* initialize the snmp variables and counters inside the struct netif
   * ifSpeed: no assumption can be made!
   */
  NETIF_INIT_SNMP(netif, snmp_ifType_softwareLoopback, 0);

  STRNCPY_S(netif->name, sizeof(netif->name), LOOPBACK_IFNAME, sizeof(netif->name));

  netif->link_layer_type = LOOPBACK_IF;
  netif->output = netif_loop_output;
  return ERR_OK;
}
#endif /* LWIP_HAVE_LOOPIF */

u8_t netif_chech_index_isusing(const char* ifname, const u8_t index)
{
  struct netif *netif;

  for(netif = netif_list; netif != NULL; netif = netif->next) {
    if(index == netif->num && STRNCMP(netif->name, ifname, IFNAMSIZ) == 0)
      return 1;
  }

  return 0;
}

static u8_t netif_alloc_index(const char* name)
{
  u8_t index;

  for(index = 0; index < LWIP_NETIF_INDEX_MAX; index++)
  {
    if(netif_chech_index_isusing(name, index) == 0)
      break;
  }
  return index;
}

u8_t netif_chech_ifaceid_isusing(const char* ifname, const u8_t num, const u8_t if_id)
{
  struct netif *netif;

  for(netif = netif_list; netif != NULL; netif = netif->next) {
    if((num == netif->num) && (STRNCMP(netif->name, ifname, IFNAMSIZ) == 0) && (if_id == netif->iface_id))
      return 1;
  }

  return 0;
}

static u8_t netif_alloc_ifaceid(const char* name, const u8_t num)
{
  u8_t index;

  for(index = 0; index <= LWIP_NETIF_IFACE_ID_MAX; index++)
  {
    if(netif_chech_ifaceid_isusing(name, num, index) == 0)
      break;
  }
  return index;
}

#if PF_PKT_SUPPORT
static u8_t netif_alloc_ifindex(void)
{
  u8_t index;

  for(index = LWIP_NETIF_IFINDEX_START; index <= LWIP_NETIF_IFINDEX_MAX; index++)
  {
    if(netif_find_by_ifindex(index) == NULL)
      break;
  }

  return index;
}
#endif

void
netif_init(void)
{
#if LWIP_HAVE_LOOPIF
  ip_addr_t loop_ipaddr, loop_netmask, loop_gw;
  /*lint -e835*//*lint -e845 */
  IP4_ADDR(&loop_gw, 127,0,0,1);
  IP4_ADDR(&loop_ipaddr, 127,0,0,1);
  IP4_ADDR(&loop_netmask, 255,0,0,0);
  /*lint +e835*/ /*lint +e845 */

#if NO_SYS
  netif_add(&loop_netif, &loop_ipaddr, &loop_netmask, &loop_gw);
  netif->input = ip_input;
#else  /* NO_SYS */
  netif_add(&loop_netif, &loop_ipaddr, &loop_netmask, &loop_gw);
#endif /* NO_SYS */
#if LWIP_NETIF_PROMISC
	loop_netif.drv_config = Stub_drv_config;
#endif
  netif_set_up(&loop_netif);
#if DRIVER_STATUS_CHECK
  loop_netif.flags |= NETIF_FLAG_DRIVER_RDY;
#endif
#endif /* LWIP_HAVE_LOOPIF */
}

/**
 * Add a network interface to the list of lwIP netifs.
 *
 * @param netif a pre-allocated netif structure
 * @param ipaddr IP_add for the new netif
 * @param netmask network mask for the new netif
 * @param gw default gateway IP_add for the new netif
 * ingress packets up in the protocol layer stack.
 *
 * @return netif, or NULL if failed.
 */

struct netif *
netif_add(struct netif *netif, const ip_addr_t *ipaddr, const ip_addr_t *netmask, const ip_addr_t *gw)
{
  return netif_add_secondary(netif, NULL, lwIP_FALSE, ipaddr, netmask, gw);
}


/**
 * Add a network interface to the list of lwIP netifs.
 *
 * @param netif a pre-allocated netif structure
 * @param main_netif the already exising netif structure to which an
 * additional IP is to be added
 * @param add_if indicates if the Interface to be added is the main one or additional one
 * @param ipaddr IP_add for the new netif
 * @param netmask network mask for the new netif
 * @param gw default gateway IP_add for the new netif
 * ingress packets up in the protocol layer stack.
 *
 * @return netif, or NULL if failed.
 */
struct netif *
netif_add_secondary(struct netif *netif, const struct netif *main_netif, const u8_t add_if,
            const ip_addr_t *ipaddr, const ip_addr_t *netmask, const ip_addr_t *gw)
{
  LWIP_ERROR("netif_add : invalid arguments", (netif != NULL), return NULL;);

  /* reset new interface configuration state */
  ip_addr_set_zero(&netif->ip_addr);
  ip_addr_set_zero(&netif->netmask);
  ip_addr_set_zero(&netif->gw);
#if LWIP_NETIF_PROMISC
  atomic_set(&netif->flags_ext,0);
  netif->flags_ext1 = 0;
#endif
#if LWIP_DHCP
  /* netif not under DHCP control by default */
  netif->dhcp = NULL;
#endif /* LWIP_DHCP */
#if LWIP_DHCPS
  /* netif not under DHCPS control by default */
  netif->dhcps = NULL;
#endif /* LWIP_DHCPS */
#if LWIP_AUTOIP
  /* netif not under AutoIP control by default */
  netif->autoip = NULL;
#endif /* LWIP_AUTOIP */
#if LWIP_NETIF_STATUS_CALLBACK
  netif->status_callback = NULL;
#endif /* LWIP_NETIF_STATUS_CALLBACK */
#if LWIP_NETIF_LINK_CALLBACK
  netif->link_callback = NULL;
#endif /* LWIP_NETIF_LINK_CALLBACK */
#if LWIP_IGMP
  netif->igmp_mac_filter = NULL;
#endif /* LWIP_IGMP */
#if ENABLE_LOOPBACK
  netif->loop_first = NULL;
  netif->loop_last = NULL;
#endif /* ENABLE_LOOPBACK */

  /* remember netif specific state information data */
  netif->input = tcpip_input;
  NETIF_SET_HWADDRHINT(netif, NULL);
#if ENABLE_LOOPBACK && LWIP_LOOPBACK_MAX_PBUFS
  netif->loop_cnt_current = 0;
#endif /* ENABLE_LOOPBACK && LWIP_LOOPBACK_MAX_PBUFS */

  netif_set_addr(netif, ipaddr, netmask, gw);

#if LWIP_HAVE_LOOPIF
  if (netif == &loop_netif) {
    if (netif_loopif_init(netif) != ERR_OK) {
      return NULL;
    }
    netif->num = 0;
    netif->iface_id= 0;
  } else
#endif
  {
    /* call user specified initialization function for netif */
    if (driverif_init(netif) != ERR_OK) {
      return NULL;
    }
    if(add_if == lwIP_FALSE) {
      netif->num = netif_alloc_index(netif->name);
      if(netif->num == LWIP_NETIF_INDEX_MAX) {
        LWIP_DEBUGF(NETIF_DEBUG, ("netif: interface index alloc fail"));
        return NULL;
      }
      netif->iface_id = 0;
    } else {
      if(main_netif == NULL) {
        LWIP_DEBUGF(NETIF_DEBUG, ("netif: Main netif not provided. Returning NULL\n"));
        return NULL;
      }
      netif->num = main_netif->num;
      netif->iface_id = netif_alloc_ifaceid(netif->name, netif->num);
      if(netif->iface_id > LWIP_NETIF_IFACE_ID_MAX) {
        LWIP_DEBUGF(NETIF_DEBUG, ("netif: interface id alloc fail"));
        return NULL;
      }
    }
  }

#if PF_PKT_SUPPORT
  netif->ifindex = netif_alloc_ifindex();
  if ((netif->ifindex < LWIP_NETIF_IFINDEX_START) || (netif->ifindex > LWIP_NETIF_IFINDEX_MAX)){
    LWIP_DEBUGF(NETIF_DEBUG, ("netif: interface id alloc fail"));
    return NULL;
  }
#endif
#if LWIP_NETIF_PROMISC
  if(sys_mutex_new(&netif->if_mutex) != ERR_OK) {
    return NULL;
  }
  /* Make sure to free the mutex if any error cases are added below */
#endif
  /* add this netif to the list */
  netif->next = netif_list;
  netif_list = netif;
  snmp_inc_iflist();

#if LWIP_IGMP
  /* start IGMP processing */
  if (netif->flags & NETIF_FLAG_IGMP) {
    igmp_start(netif);
  }
#endif /* LWIP_IGMP */

#ifdef  LWIP_DEV_DEBUG
  LWIP_DEBUGF(NETIF_DEBUG, ("netif: added interface %s%"U16_F":%u IP addr ",
    netif->name, netif->num, netif->iface_id));
  ip_addr_debug_print(NETIF_DEBUG, ipaddr);
  LWIP_DEBUGF(NETIF_DEBUG, (" netmask "));
  ip_addr_debug_print(NETIF_DEBUG, netmask);
  LWIP_DEBUGF(NETIF_DEBUG, (" gw "));
  ip_addr_debug_print(NETIF_DEBUG, gw);
  LWIP_DEBUGF(NETIF_DEBUG, ("\n"));
  #endif
  return netif;
}

/**
 * Change IP_add configuration for a network interface (including netmask
 * and default gateway).
 *
 * @param netif the network interface to change
 * @param ipaddr the new IP_add
 * @param netmask the new netmask
 * @param gw the new default gateway
 */
void
netif_set_addr(struct netif *netif, const ip_addr_t *ipaddr, const ip_addr_t *netmask,
    const ip_addr_t *gw)
{
  LWIP_ERROR("netif_set_addr: invalid arguments",(netif != NULL), return;);

  netif_set_ipaddr(netif, ipaddr);
  netif_set_netmask(netif, netmask);
  netif_set_gw(netif, gw);
}

/**
 * Remove a network interface from the list of lwIP netifs.
 *
 * @param netif the network interface to remove
 */
void
netif_remove(struct netif *netif)
{
  struct netif *loc_netif;

  if (netif == NULL) {
    return;
  }

#if LWIP_IGMP
  /* stop IGMP processing */
  if (netif->flags & NETIF_FLAG_IGMP) {
    igmp_stop(netif);
  }
#endif /* LWIP_IGMP */
#if LWIP_NETIF_PROMISC
  /* disable promiscuous mode if enabled */
  if(atomic_read(&netif->flags_ext) == NETIF_FLAG_PROMISC)
  {
    netif_update_promiscuous_mode_status(netif, 0);
  }
#endif
  if (netif_is_up(netif)) {
    /* set netif down before removing (call callback function) */
    netif_set_down(netif);
  }

  snmp_delete_ipaddridx_tree(netif);

  /*  is it the first netif? */
  if (netif_list == netif) {
    netif_list = netif->next;
  } else {
    /*  look for netif further down the list */
    struct netif * tmpNetif;
    for (tmpNetif = netif_list; tmpNetif != NULL; tmpNetif = tmpNetif->next) {
      if (tmpNetif->next == netif) {
        tmpNetif->next = netif->next;
        break;
      }
    }
    if (tmpNetif == NULL)
      return; /*  we didn't find any netif today */
  }
  snmp_dec_iflist();
  /* this netif is default? */
  if (netif_default == netif) {
    /* reset default netif */
    netif_set_default(NULL);
  }
#if LWIP_NETIF_REMOVE_CALLBACK
  if (netif->remove_callback) {
    netif->remove_callback(netif);
  }
#endif /* LWIP_NETIF_REMOVE_CALLBACK */
#if LWIP_NETIF_PROMISC
  sys_mutex_free(&netif->if_mutex);
  sys_mutex_set_invalid(&netif->if_mutex);
#endif
  LWIP_DEBUGF( NETIF_DEBUG, ("netif_remove: removed netif\n") );

  if(netif->iface_id != 0) {
    return;
  }

  loc_netif = netif_list;
  while (loc_netif != NULL) {
    if(netif == loc_netif) {
      loc_netif = loc_netif->next;
      continue;
    }
    if((!STRNCMP(netif->name, loc_netif->name, IFNAMSIZ)) && (netif->num == loc_netif->num)) {
      if (netif_is_up(loc_netif)) {
        netif_set_down(loc_netif);
        LWIP_DEBUGF( NETIF_DEBUG, ("netif_remove: Setting secondary interface as down\n") );
      }
      break;
    }
    loc_netif = loc_netif->next;
  }
}

/**
 * Find a network interface by searching for its name
 *
 * @param name the name of the netif (like netif->name) plus concatenated number
 * in ascii representation (e.g. 'en0')
 */
struct netif *
netif_find(const char *name)
{
  struct netif *netif;
  char netif_name[IFNAMSIZ];

  if (name == NULL) {
    return NULL;
  }

  MEMSET_S(netif_name, sizeof(netif_name), 0, sizeof(netif_name)); //CSEC_FIX_2302

  for(netif = netif_list; netif != NULL; netif = netif->next) {
    if (STRNCMP(name, netif->name, IFNAMSIZ) == 0) {
      LWIP_DEBUGF(NETIF_DEBUG, ("netif_find: found %s\n", name));
      return netif;
    }    
    (void)SNPRINTF_S(netif_name, IFNAMSIZ, (IFNAMSIZ-1), "%s%"U16_F":%u", netif->name, netif->num,netif->iface_id);

    if (STRNCMP(name, netif_name, IFNAMSIZ) == 0) {
      LWIP_DEBUGF(NETIF_DEBUG, ("netif_find: found %s\n", name));
      return netif;
    }
    if(netif->iface_id == 0) {
      (void)SNPRINTF_S(netif_name, IFNAMSIZ, (IFNAMSIZ-1), "%s%"U16_F, netif->name, netif->num);
      if (STRNCMP(name, netif_name, IFNAMSIZ) == 0) {
        LWIP_DEBUGF(NETIF_DEBUG, ("netif_find: found %s\n", name));
        return netif;
      }
    }
  }
  LWIP_DEBUGF(NETIF_DEBUG, ("netif_find: didn't find %s\n", name));
  return NULL;
}

/**
 * Find a network interface by searching for its ipaddress
 *
 * @param ipaddr IP_add of the netif
 * @return netif if the its found
 *         NULL if there is no netif with ipaddr
 */

struct netif *
netif_find_by_ipaddr(ip_addr_t *ipaddr)
{
  struct netif *netif;

  if (ipaddr == NULL) {
    return NULL;
  }

  for (netif = netif_list; netif != NULL; netif = netif->next) {
    if (ip_addr_cmp(&(netif->ip_addr), ipaddr)) {
      LWIP_DEBUGF(NETIF_DEBUG, ("netif_find_using_ipaddr: found\n"));
      return netif;
    }
  }
  LWIP_DEBUGF(NETIF_DEBUG, ("netif_find_using_ipaddr: didn't find\n"));
  return NULL;
}


s8_t  netif_find_dst_ipaddr(ip_addr_t *ipaddr, ip_addr_t **dst_addr)
{
	  
   struct netif *netif;
   ip_addr_t *pdst_addr;

   pdst_addr = *dst_addr;
   netif = netif_find_by_ipaddr(ipaddr);
   if(netif == NULL) {
       return -1;
   }
   if (!ip_addr_netcmp(pdst_addr, &(netif->ip_addr), &(netif->netmask)) &&
      !ip_addr_islinklocal(pdst_addr)) {
     if(!ip_addr_isany(&netif->gw)) {
        *dst_addr  = &(netif->gw);
     }
   }

   return 0;
}

#if PF_PKT_SUPPORT
/**
 * Find a network interface by searching for its interface index
 *
 * @param ifindex interface index of the netif
 * @return netif if the its found
 *         NULL if there is no netif with ifindex
 */

struct netif *
netif_find_by_ifindex(u8_t ifindex)
{
  struct netif *netif = NULL;

  if(ifindex < LWIP_NETIF_IFINDEX_START) {
    LWIP_DEBUGF(NETIF_DEBUG, ("netif_find_by_ifindex: Invalid ifindex(%u) \n", ifindex));
    return netif;
  }

  for (netif = netif_list; netif != NULL; netif = netif->next) {
    if (netif->ifindex == ifindex) {
      LWIP_DEBUGF(NETIF_DEBUG, ("netif_find_by_ifindex: found\n"));
      return netif;
    }
  }

  LWIP_DEBUGF(NETIF_DEBUG, ("netif_find_by_ifindex: didn't find\n"));
  return NULL;
}
#endif



/**
 * Change the IP_add of a network interface
 *
 * @param netif the network interface to change
 * @param ipaddr the new IP_add
 *
 * @note call netif_set_addr() if you also want to change netmask and
 * default gateway
 */
void
netif_set_ipaddr(struct netif *netif, const ip_addr_t *ipaddr)
{
  /* TODO: Handling of obsolete pcbs */
  /* See:  https://mail.gnu.org/archive/html/lwip-users/2003-03/msg00118.html */
#if LWIP_TCP
  struct tcp_pcb *pcb;
  struct tcp_pcb_listen *lpcb;

  /* address is actually being changed? */
  /* PC Lint Fix for: Boolean argument to equal/not equal*/
  if (ipaddr && !ip_addr_cmp(ipaddr, &(netif->ip_addr))) {
    LWIP_DEBUGF(NETIF_DEBUG | LWIP_DBG_STATE, ("netif_set_ipaddr: netif address being changed\n"));
    pcb = tcp_active_pcbs;
    while (pcb != NULL) {
      /* PCB bound to current local interface address? */
      if (ip_addr_cmp(&(pcb->local_ip), &(netif->ip_addr))
#if LWIP_AUTOIP
        /* connections to link-local addresses must persist (RFC3927 ch. 1.9) */
        && !ip_addr_islinklocal(&(pcb->local_ip))
#endif /* LWIP_AUTOIP */
        ) {
        /* this connection must be aborted */
        struct tcp_pcb *next = pcb->next;
        LWIP_DEBUGF(NETIF_DEBUG | LWIP_DBG_STATE, ("netif_set_ipaddr: aborting TCP pcb %p\n", (void *)pcb));
        tcp_abort(pcb);
        pcb = next;
      } else {
        pcb = pcb->next;
      }
    }
    for (lpcb = tcp_listen_pcbs.listen_pcbs; lpcb != NULL; lpcb = lpcb->next) {
      /* PCB bound to current local interface address? */
      if ((!(ip_addr_isany(&(lpcb->local_ip)))) &&
          (ip_addr_cmp(&(lpcb->local_ip), &(netif->ip_addr)))) {
        /* The PCB is listening to the old ipaddr and
         * is set to listen to the new one instead */
        ip_addr_set(&(lpcb->local_ip), ipaddr);
      }
    }
    pcb = tcp_bound_pcbs;
    while (pcb != NULL) {
      /* PCB bound to current local interface address? */
      if ((!(ip_addr_isany(&(pcb->local_ip)))) &&
          (ip_addr_cmp(&(pcb->local_ip), &(netif->ip_addr)))) {
        /* The PCB is listening to the old ipaddr and
         * is set to listen to the new one instead */
        ip_addr_set(&(pcb->local_ip), ipaddr);
      }
      pcb = pcb->next;
    }
  }
#endif
  snmp_delete_ipaddridx_tree(netif);
  snmp_delete_iprteidx_tree(0,netif);
  /* set new IP_add to netif */
  ip_addr_set(&(netif->ip_addr), ipaddr);
  snmp_insert_ipaddridx_tree(netif);
  snmp_insert_iprteidx_tree(0,netif);

#ifdef  LWIP_DEV_DEBUG
  LWIP_DEBUGF(NETIF_DEBUG | LWIP_DBG_TRACE | LWIP_DBG_STATE, ("netif: IP_addr of interface %s%"U16_F":%u set to %"U16_F".%"U16_F".%"U16_F".%"U16_F"\n",
    netif->name, netif->num, netif->iface_id,
    ip4_addr1_16(&netif->ip_addr),
    ip4_addr2_16(&netif->ip_addr),
    ip4_addr3_16(&netif->ip_addr),
    ip4_addr4_16(&netif->ip_addr)));
#endif
}

/**
 * Change the default gateway for a network interface
 *
 * @param netif the network interface to change
 * @param gw the new default gateway
 *
 * @note call netif_set_addr() if you also want to change IP_add and netmask
 */
void
netif_set_gw(struct netif *netif, const ip_addr_t *gw)
{
  ip_addr_set(&(netif->gw), gw);
#ifdef  LWIP_DEV_DEBUG
  LWIP_DEBUGF(NETIF_DEBUG | LWIP_DBG_TRACE | LWIP_DBG_STATE, ("netif: GW address of interface %s%u:%u set to %"U16_F".%"U16_F".%"U16_F".%"U16_F"\n",
    netif->name, netif->num, netif->iface_id,
    ip4_addr1_16(&netif->gw),
    ip4_addr2_16(&netif->gw),
    ip4_addr3_16(&netif->gw),
    ip4_addr4_16(&netif->gw)));
#endif
}

/**
 * Change the netmask of a network interface
 *
 * @param netif the network interface to change
 * @param netmask the new netmask
 *
 * @note call netif_set_addr() if you also want to change IP_add and
 * default gateway
 */
void
netif_set_netmask(struct netif *netif, const ip_addr_t *netmask)
{
  snmp_delete_iprteidx_tree(0, netif);
  /* set new netmask to netif */
  ip_addr_set(&(netif->netmask), netmask);
  snmp_insert_iprteidx_tree(0, netif);
#ifdef  LWIP_DEV_DEBUG
  LWIP_DEBUGF(NETIF_DEBUG | LWIP_DBG_TRACE | LWIP_DBG_STATE, ("netif: netmask of interface %s%"U16_F":%u set to %"U16_F".%"U16_F".%"U16_F".%"U16_F"\n",
    netif->name, netif->num, netif->iface_id,
    ip4_addr1_16(&netif->netmask),
    ip4_addr2_16(&netif->netmask),
    ip4_addr3_16(&netif->netmask),
    ip4_addr4_16(&netif->netmask)));
#endif
}

/**
 * Change the hardware address for a network interface
 *
 * @param netif the network interface to change
 * @param hw_addr the new hardware address
 * @param hw_len the length of new hardware address,
 *
 */
void
netif_set_hwaddr(struct netif *netif, const unsigned char *hw_addr, int hw_len)
{
    int i;
    struct netif *loc_netif;

    LWIP_ERROR("netif_set_gw: invalid arguments",
        ((unsigned int)hw_len == NETIF_MAX_HWADDR_LEN), return;);
    for(i = 0; i < hw_len; i++)
    {
        netif->hwaddr[i] = hw_addr[i];
    }
    LWIP_DEBUGF(NETIF_DEBUG | LWIP_DBG_TRACE | LWIP_DBG_STATE,
        ("netif: HW address of interface %s%"U16_F":%u set to %02X:%02X:%02X:%02X:%02X:%02X\n",
    netif->name, netif->num, netif->iface_id,
    netif->hwaddr[0], netif->hwaddr[1], netif->hwaddr[2],
    netif->hwaddr[3], netif->hwaddr[4], netif->hwaddr[5]));

    loc_netif = netif_list;
    while (loc_netif != NULL) {
        if (loc_netif == netif) {
          loc_netif = loc_netif->next;
          continue;
        }
        if((!STRNCMP(netif->name, loc_netif->name, IFNAMSIZ)) && (netif->num == loc_netif->num)) {
            for(i = 0; i < hw_len; i++)
            {
                loc_netif->hwaddr[i] = hw_addr[i];
            }

            LWIP_DEBUGF(NETIF_DEBUG | LWIP_DBG_TRACE | LWIP_DBG_STATE,
                ("netif: HW address of interface %s%u:%u set to %02X:%02X:%02X:%02X:%02X:%02X\n",
                loc_netif->name, loc_netif->num, loc_netif->iface_id,
                loc_netif->hwaddr[0], loc_netif->hwaddr[1], loc_netif->hwaddr[2],
                loc_netif->hwaddr[3], loc_netif->hwaddr[4], loc_netif->hwaddr[5]));
            break;
        }
        loc_netif = loc_netif->next;
    }
}


/**
 * Set a network interface as the default network interface
 * (used to output all packets for which no specific route is found)
 *
 * @param netif the default network interface
 */
void
netif_set_default(struct netif *netif)
{
  if (netif == NULL) {
    /* remove default route */
    snmp_delete_iprteidx_tree(1, netif);
  } else {
    /* install default route */
    snmp_insert_iprteidx_tree(1, netif);
  }
  netif_default = netif;
  LWIP_DEBUGF(NETIF_DEBUG, ("netif: setting default interface %s%"U16_F":%u\n",
           netif ? netif->name : "NULL", netif ? netif->num : 0, netif ? netif->iface_id: 0));
}

/**
 * Bring an interface up, available for processing
 * traffic.
 *
 * @note: Enabling DHCP on a down interface will make it come
 * up once configured.
 *
 * @see dhcp_start()
 */
void netif_set_up(struct netif *netif)
{

  LWIP_ERROR("netif_set_up: invalid arguments",(netif != NULL), return;);

  if (!(netif->flags & NETIF_FLAG_UP)) {
    netif->flags |= NETIF_FLAG_UP;

    /*NETIF DRIVER STATUS BEGIN */
#if DRIVER_STATUS_CHECK
    netif->waketime = -1;
#endif
    /*NETIF DRIVER STATUS END */

#if LWIP_SNMP
    snmp_get_sysuptime(&netif->ts);
#endif /* LWIP_SNMP */

    NETIF_STATUS_CALLBACK(netif);

    if (netif->flags & NETIF_FLAG_LINK_UP) {
#if LWIP_ARP
      /* For Ethernet network interfaces, we would like to send a "gratuitous ARP" */
      if (netif->flags & (NETIF_FLAG_ETHARP)) {
        etharp_gratuitous(netif);
      }
#endif /* LWIP_ARP */

#if LWIP_IGMP
      /* resend IGMP memberships */
      if (netif->flags & NETIF_FLAG_IGMP) {
        igmp_report_groups( netif);
      }
#endif /* LWIP_IGMP */
    }
  }
}

/**
 * Bring an interface down, disabling any traffic processing.
 *
 * @note: Enabling DHCP on a down interface will make it come
 * up once configured.
 *
 * @see dhcp_start()
 */
void netif_set_down(struct netif *netif)
{

  LWIP_ERROR("netif_set_down: invalid arguments",(netif != NULL), return;);

  if (netif->flags & NETIF_FLAG_UP) {
#if DRIVER_STATUS_CHECK
    netif->flags = (u16_t)(netif->flags & (~NETIF_FLAG_UP));
#else
    netif->flags = (u8_t)(netif->flags & (~NETIF_FLAG_UP));
#endif
#if LWIP_SNMP
    snmp_get_sysuptime(&netif->ts);
#endif

#if LWIP_ARP
    if (netif->flags & NETIF_FLAG_ETHARP) {
      etharp_cleanup_netif(netif);
    }
#endif /* LWIP_ARP */
    NETIF_STATUS_CALLBACK(netif);
  }
}

#if LWIP_NETIF_STATUS_CALLBACK
/**
 * Set callback to be called when interface is brought up/down
 */
void netif_set_status_callback(struct netif *netif, netif_status_callback_fn status_callback)
{
  if (netif) {
    netif->status_callback = status_callback;
  }
}
#endif /* LWIP_NETIF_STATUS_CALLBACK */

#if LWIP_NETIF_REMOVE_CALLBACK
/**
 * Set callback to be called when the interface has been removed
 */
void
netif_set_remove_callback(struct netif *netif, netif_status_callback_fn remove_callback)
{
  if (netif) {
    netif->remove_callback = remove_callback;
  }
}
#endif /* LWIP_NETIF_REMOVE_CALLBACK */

/**
 * Called by a driver when its link goes up
 */
void netif_set_link_up(struct netif *netif )
{

  LWIP_ERROR("netif_set_link_up: invalid arguments",(netif != NULL), return;);

  if (!(netif->flags & NETIF_FLAG_LINK_UP)) {
    netif->flags |= NETIF_FLAG_LINK_UP;

#if LWIP_DHCP
    if (netif->dhcp) {
      dhcp_network_changed(netif);
    }
#endif /* LWIP_DHCP */

#if LWIP_AUTOIP
    if (netif->autoip) {
      autoip_network_changed(netif);
    }
#endif /* LWIP_AUTOIP */

    if (netif->flags & NETIF_FLAG_UP) {
#if LWIP_ARP
      /* For Ethernet network interfaces, we would like to send a "gratuitous ARP" */
      if (netif->flags & NETIF_FLAG_ETHARP) {
        etharp_gratuitous(netif);
      }
#endif /* LWIP_ARP */

#if LWIP_IGMP
      /* resend IGMP memberships */
      if (netif->flags & NETIF_FLAG_IGMP) {
        igmp_report_groups( netif);
      }
#endif /* LWIP_IGMP */
    }
    NETIF_LINK_CALLBACK(netif);
  }
}

/**
 * Called by a driver when its link goes down
 */
void netif_set_link_down(struct netif *netif )
{

  LWIP_ERROR("netif_set_link_down: invalid arguments",(netif != NULL), return;);

  if (netif->flags & NETIF_FLAG_LINK_UP) {
#if DRIVER_STATUS_CHECK
    netif->flags = (u16_t)(netif->flags & (~NETIF_FLAG_LINK_UP));
#else
    netif->flags = (u8_t)(netif->flags & (~NETIF_FLAG_LINK_UP));
#endif
    NETIF_LINK_CALLBACK(netif);
  }
}

#if LWIP_NETIF_LINK_CALLBACK
/**
 * Set callback to be called when link is brought up/down
 */
void netif_set_link_callback(struct netif *netif, netif_status_callback_fn link_callback)
{
  if (netif) {
    netif->link_callback = link_callback;
  }
}
#endif /* LWIP_NETIF_LINK_CALLBACK */

#if ENABLE_LOOPBACK
/**
 * Send an IP packet to be received on the same netif (loopif-like).
 * The pbuf is simply copied and handed back to netif->input.
 * In multithreaded mode, this is done directly since netif->input must put
 * the packet on a queue.
 * In callback mode, the packet is put on an internal queue and is fed to
 * netif->input by netif_poll().
 *
 * @param netif the lwip network interface structure
 * @param p the (IP) packet to 'send'
 * @param ipaddr the IP_add to send the packet to (not used)
 * @return ERR_OK if the packet has been sent
 *         ERR_MEM if the pbuf used to copy the packet couldn't be allocated
 */
err_t
netif_loop_output(struct netif *netif, struct pbuf *p,
       ip_addr_t *ipaddr)
{
  struct pbuf *r;
  err_t err;
  struct pbuf *last;
#if LWIP_LOOPBACK_MAX_PBUFS
  u8_t clen = 0;
#endif /* LWIP_LOOPBACK_MAX_PBUFS */
  /* If we have a loopif, SNMP counters are adjusted for it,
   * if not they are adjusted for 'netif'. */
#if LWIP_SNMP
#if LWIP_HAVE_LOOPIF
  struct netif *stats_if = &loop_netif;
#else /* LWIP_HAVE_LOOPIF */
  struct netif *stats_if = netif;
#endif /* LWIP_HAVE_LOOPIF */
#endif /* LWIP_SNMP */
  SYS_ARCH_DECL_PROTECT(lev);
  LWIP_UNUSED_ARG(ipaddr);

  /* Allocate a new pbuf */
  r = pbuf_alloc(PBUF_LINK, p->tot_len, PBUF_RAM);
  if (r == NULL) {
    LINK_STATS_INC(link.memerr);
    LINK_STATS_INC(link.drop);
    LINK_STATS_INC(link.link_tx_drop);
    snmp_inc_ifoutdiscards(stats_if);
    return ERR_MEM;
  }
#if LWIP_LOOPBACK_MAX_PBUFS
  clen = pbuf_clen(r);
  /* check for overflow or too many pbuf on queue */
  if(((netif->loop_cnt_current + clen) < netif->loop_cnt_current) ||
     ((netif->loop_cnt_current + clen) > LWIP_LOOPBACK_MAX_PBUFS)) {
    pbuf_free(r);
    LINK_STATS_INC(link.memerr);
    LINK_STATS_INC(link.drop);
    LINK_STATS_INC(link.link_tx_drop);
    snmp_inc_ifoutdiscards(stats_if);
    return ERR_MEM;
  }
  netif->loop_cnt_current += clen;
#endif /* LWIP_LOOPBACK_MAX_PBUFS */

  /* Copy the whole pbuf queue p into the single pbuf r */
  if ((err = pbuf_copy(r, p)) != ERR_OK) {
    pbuf_free(r);
    LINK_STATS_INC(link.memerr);
    LINK_STATS_INC(link.drop);
    LINK_STATS_INC(link.link_tx_drop);
    snmp_inc_ifoutdiscards(stats_if);
    return err;
  }

  /* Put the packet on a linked list which Gets emptied through calling
     netif_poll(). */

  /* let last point to the last pbuf in chain r */
  /*lint -e722*/
  for (last = r; last->next != NULL; last = last->next);
  /*lint +e722*/

  SYS_ARCH_PROTECT(lev);
  if(netif->loop_first != NULL) {
    LWIP_ASSERT("if first != NULL, last must also be != NULL", netif->loop_last != NULL);
    netif->loop_last->next = r;
    netif->loop_last = last;
  } else {
    netif->loop_first = r;
    netif->loop_last = last;
  }
  SYS_ARCH_UNPROTECT(lev);

  LINK_STATS_INC(link.xmit);
  snmp_add_ifoutoctets(stats_if, p->tot_len);
  snmp_inc_ifoutucastpkts(stats_if);

#if LWIP_NETIF_LOOPBACK_MULTITHREADING
  /* For multithreading environment, schedule a call to netif_poll */
  tcpip_callback((tcpip_callback_fn)netif_poll, netif);
#endif /* LWIP_NETIF_LOOPBACK_MULTITHREADING */

  return ERR_OK;
}

/**
 * Call netif_poll() in the main loop of your application. This is to prevent
 * reentering non-reentrant functions like tcp_input(). Packets passed to
 * netif_loop_output() are put on a list that is passed to netif->input() by
 * netif_poll().
 */
void
netif_poll(struct netif *netif)
{
  struct pbuf *in;
  /* If we have a loopif, SNMP counters are adjusted for it,
   * if not they are adjusted for 'netif'. */
#if LWIP_SNMP
#if LWIP_HAVE_LOOPIF
  struct netif *stats_if = &loop_netif;
#else /* LWIP_HAVE_LOOPIF */
  struct netif *stats_if = netif;
#endif /* LWIP_HAVE_LOOPIF */
#endif /* LWIP_SNMP */
  SYS_ARCH_DECL_PROTECT(lev);

  do {
    /* Get a packet from the list. With SYS_LIGHTWEIGHT_PROT=1, this is protected */
    SYS_ARCH_PROTECT(lev);
    in = netif->loop_first;
    if (in != NULL) {
      struct pbuf *in_end = in;
#if LWIP_LOOPBACK_MAX_PBUFS
      u8_t clen = pbuf_clen(in);
      /* adjust the number of pbufs on queue */
      LWIP_ASSERT("netif->loop_cnt_current underflow",
        ((netif->loop_cnt_current - clen) < netif->loop_cnt_current));
      netif->loop_cnt_current -= clen;
#endif /* LWIP_LOOPBACK_MAX_PBUFS */
      while (in_end->len != in_end->tot_len) {
        LWIP_ASSERT("bogus pbuf: len != tot_len but next == NULL!", in_end->next != NULL);
        in_end = in_end->next;
      }
      /* 'in_end' now points to the last pbuf from 'in' */
      if (in_end == netif->loop_last) {
        /* this was the last pbuf in the list */
        netif->loop_first = netif->loop_last = NULL;
      } else {
        /* pop the pbuf off the list */
        netif->loop_first = in_end->next;
        LWIP_ASSERT("should not be null since first != last!", netif->loop_first != NULL);
      }
      /* De-queue the pbuf from its successors on the 'loop_' list. */
      in_end->next = NULL;
    }
    SYS_ARCH_UNPROTECT(lev);

    if (in != NULL) {
      LINK_STATS_INC(link.recv);
      snmp_add_ifinoctets(stats_if, in->tot_len);
      snmp_inc_ifinucastpkts(stats_if);
      /* loopback packets are always IP packets! */
      if (ip_input(in, netif) != ERR_OK) {
        LWIP_ASSERT("ip_input should not fail, if fails free pbug", ERR_OK);
      }
      /* Don't reference the packet any more! */
      in = NULL;
    }
  /* go on while there is a packet on the list */
  } while (netif->loop_first != NULL);
}

#if !LWIP_NETIF_LOOPBACK_MULTITHREADING
/**
 * Calls netif_poll() for every netif on the netif_list.
 */
void
netif_poll_all(void)
{
  struct netif *netif = netif_list;
  /* loop through netifs */
  while (netif != NULL) {
    netif_poll(netif);
    /* proceed to next network interface */
    netif = netif->next;
  }
}
#endif /* !LWIP_NETIF_LOOPBACK_MULTITHREADING */
#endif /* ENABLE_LOOPBACK */

/**
 * Change IP_add configuration for a network interface (including netmask
 * and default gateway).
 *
 * @param netif the network interface to change
 * @param netif_mtu the new MTU for the interface
 */

err_t
netif_set_mtu(struct netif *netif, u16_t netif_mtu)
{
  /* As per RFC 791, "Every internet module must be able to forward a datagram of 68
   * octets without further fragmentation.  This is because an internet header
   * may be up to 60 octets, and the minimum fragment is 8 octets." */
  LWIP_ERROR("netif_set_mtu: invalid arguments",(netif != NULL), return ERR_ARG;);
  LWIP_ERROR("netif_set_mtu: invalid arguments",(netif_mtu >= IP_FRAG_MIN_MTU) && (netif_mtu <= IP_FRAG_MAX_MTU), return ERR_ARG;);

  netif->mtu = netif_mtu;
  LWIP_DEBUGF(NETIF_DEBUG | LWIP_DBG_STATE, ("netif: MTU of interface %s%"U16_F":%u is changed to %d\n",
                netif->name, netif->num, netif->iface_id, netif->mtu));

  return ERR_OK;
}


/* NETIF DRIVER STATUS BEGIN */
#if DRIVER_STATUS_CHECK

err_t
netif_stop_queue(struct netif *netif)
{
  struct netif *loc_netif;

  LWIP_ERROR("netif_set_driver_ready: invalid arguments",(netif != NULL), return ERR_ARG;);

  if (!(netif->flags & NETIF_FLAG_DRIVER_RDY)) {
    LWIP_DEBUGF(NETIF_DEBUG, ("netif already stopped \n"));
    return ERR_ARG;
  }

  netif->flags = (u16_t)(netif->flags & ~NETIF_FLAG_DRIVER_RDY);

  /* Check for the second interface which has same driver as the given netif */
  loc_netif = netif_list;
  while(loc_netif != NULL) {
    if(netif == loc_netif) {
      loc_netif = loc_netif->next;
      continue;
    }

    if((!STRNCMP(netif->name, loc_netif->name, IFNAMSIZ)) && (netif->num == loc_netif->num)) {
      loc_netif->flags = (u16_t)(loc_netif->flags & ~NETIF_FLAG_DRIVER_RDY);
      break;
    }
    loc_netif = loc_netif->next;
  }

  /* Start the waketimer for this netif */
  netif->waketime = 0;

  tcpip_upd_status_to_tcp_pcbs(netif, DRV_NOT_READY);

  if (loc_netif != NULL)  {
    /* Start the waketimer for this netif */
    loc_netif->waketime = 0;
    tcpip_upd_status_to_tcp_pcbs(loc_netif, DRV_NOT_READY);
  }

  return ERR_OK;
}

err_t
netif_wake_queue(struct netif *netif)
{
  struct netif *loc_netif;

  LWIP_ERROR("netif_set_driver_ready: invalid arguments",(netif != NULL), return ERR_ARG;);


  if (netif->flags & NETIF_FLAG_DRIVER_RDY) {
    LWIP_DEBUGF(NETIF_DEBUG, ("netif already in ready state \n"));
    return ERR_ARG;
  }

  netif->flags |= NETIF_FLAG_DRIVER_RDY;

  tcp_ip_event_sendplus_on_wake_queue(netif);

  /* Check for the second interface which has same driver as the given netif */
  loc_netif = netif_list;
  while(loc_netif != NULL) {
    if(netif == loc_netif) {
      loc_netif = loc_netif->next;
      continue;
    }

    if((!STRNCMP(netif->name, loc_netif->name, IFNAMSIZ)) && (netif->num == loc_netif->num)) {
      loc_netif->flags |= NETIF_FLAG_DRIVER_RDY;
      tcp_ip_event_sendplus_on_wake_queue(loc_netif);
      break;
    }
    loc_netif = loc_netif->next;
  }

  /* Stop the waketimer for this netif */
  netif->waketime = -1;

  tcpip_upd_status_to_tcp_pcbs(netif, DRV_READY);

  /* Need to send the buffered TCP segments */
  tcpip_flush_on_wake_queue(netif, DRV_READY);

  if (loc_netif != NULL)  {
    /* Stop the waketimer for this netif */
    loc_netif->waketime = -1;

    tcpip_upd_status_to_tcp_pcbs(loc_netif, DRV_READY);

    /* Need to send the buffered TCP segments */
    tcpip_flush_on_wake_queue(loc_netif, DRV_READY);
  }

  return ERR_OK;
}
#endif
/* NETIF DRIVER STATUS END */

#if LWIP_NETIF_PROMISC
/* Enables the promiscuous mode of the netif, if no pkt_raw_pcb are listening over it. */
static void netif_enable_promisc_mode(struct netif *netif)
{
  struct netif *loc_netif;
  u8_t activ_flag = lwIP_FALSE;

  /* Check the status of secondary netif */
  for (loc_netif = netif_list; loc_netif != NULL; loc_netif = loc_netif->next)
  {
    if (netif == loc_netif)
    {
      continue;
    }
    if((!STRNCMP(netif->name, loc_netif->name, IFNAMSIZ)) && (netif->num == loc_netif->num))
    {
      if (loc_netif->flags_ext1 & NETIF_FLAG_PROMISC_RUNNING)
      {
        activ_flag = lwIP_TRUE;
        break;
      }
    }
  }

  if (pkt_raw_pcbs_using_netif(netif->ifindex) >= 1)
  {
    netif->flags_ext1 |= NETIF_FLAG_PROMISC_RUNNING;

    /* enable promiscusous mode */
    if((!activ_flag) && (netif->drv_config))
    {
      netif->drv_config(netif, IFF_PROMISC, 1);
    }
    else
    {
      LWIP_DEBUGF(NETIF_DEBUG | LWIP_DBG_STATE, ("netif: promiscuous mode support not found %s\n",
                  netif->name));
    }
  }

  return;
}

/* disables promiscuous mode of the netif, if no pkt_raw_pcb are listening over it. */
static void netif_disable_promisc_mode(struct netif *netif)
{
  struct netif *loc_netif;
  u8_t activ_flag = lwIP_FALSE;

  /* Check the status of secondary netif */
  for (loc_netif = netif_list; loc_netif != NULL; loc_netif = loc_netif->next)
  {
    if (netif == loc_netif)
    {
      continue;
    }
    if((!STRNCMP(netif->name, loc_netif->name, IFNAMSIZ)) && (netif->num == loc_netif->num)) {
      if (loc_netif->flags_ext1 & NETIF_FLAG_PROMISC_RUNNING)
      {
        activ_flag = lwIP_TRUE;
        break;
      }
    }
  }

  /* disable promiscusous mode */
  if((!activ_flag) && (netif->drv_config))
  {
    netif->drv_config(netif, IFF_PROMISC, 0);
  }

  netif->flags_ext1 &= (u8_t)(~(NETIF_FLAG_PROMISC_RUNNING));

  return;
}

/**
 * Enable promiscuous mode on interface
 *
 * @note: Enabling promiscuous mode on interface will allow receiving all packets.
 * return 0 on success
 *
 */
void netif_update_promiscuous_mode_status(struct netif *netif, u8_t  set)
{
  /* return back if promiscuous mode is already enabled on netif */
  if((set && ((atomic_read(&netif->flags_ext) == NETIF_FLAG_PROMISC)))
    || (!set && !(atomic_read(&netif->flags_ext) == NETIF_FLAG_PROMISC)))
  {
    LWIP_DEBUGF(NETIF_DEBUG | LWIP_DBG_STATE, ("netif: promiscuous mode is alredy %s\n", (set?"enabled":"disabled")));
    return;
  }

  if(netif->drv_config)
  {
    if(set)
    {
      atomic_set(&netif->flags_ext,NETIF_FLAG_PROMISC);

      netif_enable_promisc_mode(netif);
    }
    else
    {
      atomic_set(&netif->flags_ext ,0);

      netif_disable_promisc_mode(netif);
    }
    LWIP_DEBUGF(NETIF_DEBUG | LWIP_DBG_STATE, ("netif: promiscuous mode %s on interface %s\n",
               set ? "enabled":"disabled", netif->name));
  }
  else
  {
    LWIP_DEBUGF(NETIF_DEBUG | LWIP_DBG_STATE, ("netif: promiscuous mode support not found %s\n",
               netif->name));
  }
}


void netif_stop_promisc_mode(u8_t ifindex)
{
  struct netif *tmpNetif;

  /* Raw socket without bind and promisc enabled on interface.*/
  if(!ifindex)
  {
    /* loop through netif's and disable promisc mode if this is the only PCB*/
    for (tmpNetif = netif_list; tmpNetif != NULL; tmpNetif = tmpNetif->next)
    {
      if((atomic_read(&tmpNetif->flags_ext) == NETIF_FLAG_PROMISC) && (!pkt_raw_pcbs_using_netif(tmpNetif->ifindex)))
      {
        netif_disable_promisc_mode(tmpNetif);
      }
    }
  }
  /* RAW socket withbind and promisc enabled on interface */
  else
  {
    tmpNetif = netif_find_by_ifindex(ifindex);
    if((tmpNetif && (atomic_read(&tmpNetif->flags_ext) == NETIF_FLAG_PROMISC)) && (!pkt_raw_pcbs_using_netif(tmpNetif->ifindex)))
    {
      netif_disable_promisc_mode(tmpNetif);
    }
  }
}

/* Enables the promiscuous mode of the netif, if no pkt_raw_pcb are listening over it. */
static void netif_start_promisc_if_not_running(struct netif *netif)
{
  if ((!(netif->flags_ext1 & NETIF_FLAG_PROMISC_RUNNING)))
  {
    netif_enable_promisc_mode(netif);
  }

  return;
}

void netif_start_promisc_mode(u8_t ifindex)
{
  struct netif *tmpNetif;

  /* Raw socket without bind and promisc enabled on interface.*/
  if(!ifindex)
  {
    /* loop through netif's and start promisc mode if there is atleast one PCB binded to the netif*/
    for (tmpNetif = netif_list; tmpNetif != NULL; tmpNetif = tmpNetif->next) {
      if (atomic_read(&tmpNetif->flags_ext) == NETIF_FLAG_PROMISC)
      {
        netif_start_promisc_if_not_running(tmpNetif);
      }
    }
  }
  /* RAW socket withbind and promisc enabled on interface */
  else
  {
    tmpNetif = netif_find_by_ifindex(ifindex);
    if(tmpNetif && (atomic_read(&tmpNetif->flags_ext) == NETIF_FLAG_PROMISC))
    {
      netif_start_promisc_if_not_running(tmpNetif);
    }
  }

  return;
}

#endif /*LWIP_NETIF_PROMISC */

