/**
 * @file
 * Address Resolution Protocol module for IP over Ethernet
 *
 * Functionally, ARP is divided into two parts. The first maps an IP_add
 * to a physical address when sending a packet, and the second part answers
 * requests from other machines for our physical address.
 *
 * This implementation complies with RFC 826 (Ethernet ARP). It supports
 * Gratuitious ARP from RFC3220 (IP Mobility Support for IPv4) section 4.6
 * if an interface calls etharp_gratuitous(our_netif) upon address change.
 */

/*
 * Copyright (c) 2001-2003 Swedish Institute of Computer Science.
 * Copyright (c) 2003-2004 Leon Woestenberg <leon.woestenberg@axon.tv>
 * Copyright (c) 2003-2004 Axon Digital Design B.V., The Netherlands.
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

#if LWIP_ARP || LWIP_ETHERNET

#include "lwip/ip_addr.h"
#include "lwip/def.h"
#include "lwip/ip.h"
#include "lwip/stats.h"
#include "lwip/snmp.h"
#include "lwip/dhcp.h"
#include "lwip/autoip.h"
#include "netif/etharp.h"
#include "netif/driverif.h"
#if PF_PKT_SUPPORT
#include "lwip/raw.h"
#endif
#include "lwip/ip_addr.h"

#if PPPOE_SUPPORT
#include "netif/ppp_oe.h"
#endif /* PPPOE_SUPPORT */

#include <string.h>

const struct eth_addr ethbroadcast = {{0xff,0xff,0xff,0xff,0xff,0xff}};
const struct eth_addr ethzero = {{0,0,0,0,0,0}};

#if USE_ONLY_PRIMARY_IFACE
#define GET_NETIF_FOR_OUPUT(_netif)    \
do { \
  struct netif *_loc_netif;            \
  /* If this is primary interface then send it */ \
  if(_netif->iface_id == 0) {          \
    /* No need to change the netif */  \
    break;                             \
  }                                    \
                                       \
  /* else case - If this is secondary interface, send it in corresponding primary interface */ \
  for(_loc_netif = netif_list; _loc_netif != NULL; _loc_netif = _loc_netif->next) { \
    if(_loc_netif == netif) {          \
      continue;                        \
    }                                  \
                                       \
    /* In case  of multiple sub interfaces */ \
    if(_loc_netif->iface_id != 0) {    \
      continue;                        \
    }                                  \
                                       \
    if((!STRNCMP(_netif->name, _loc_netif->name, IFNAMSIZ) && (_netif->num == _loc_netif->num))) { \
      _netif = _loc_netif;             \
      break;                           \
    }                                  \
  }                                    \
} while(0)
#endif


#if LWIP_ARP /* don't build if not configured for use in lwipopts.h */

/** the time an ARP entry stays valid after its last update,
 *  for ARP_TMR_INTERVAL = 1000, this is
 *  (1200 * 5) seconds = 20 minutes.
 */
#define ARP_MAXAGE              1200
/** Re-request a used ARP entry 1 minute before it would expire to prevent
 *  breaking a steadily used connection because the ARP entry timed out. */
#define ARP_AGE_REREQUEST_USED  (ARP_MAXAGE - 60)

/** the time an ARP entry stays pending after first request,
 *  for ARP_TMR_INTERVAL = 1000, this is
 *  (2 * 1) seconds = 2 seconds.
 *
 *  @internal Keep this number at least 2, otherwise it might
 *  run out instantly if the timeout occurs directly after a request.
 */
#define ARP_MAXPENDING 2

#define HWTYPE_ETHERNET ETHERNET_DRIVER_IF
#define HWTYPE_WIFI WIFI_DRIVER_IF

struct etharp_entry arp_table[ARP_TABLE_SIZE];

#if !LWIP_NETIF_HWADDRHINT
static u8_t etharp_cached_entry;
#endif /* !LWIP_NETIF_HWADDRHINT */

#if LWIP_NETIF_HWADDRHINT
#define ETHARP_SET_HINT(netif, hint)  if (((netif) != NULL) && ((netif)->addr_hint != NULL))  \
                                      *((netif)->addr_hint) = (hint);
#else /* LWIP_NETIF_HWADDRHINT */
#define ETHARP_SET_HINT(netif, hint)  (etharp_cached_entry = (hint))
#endif /* LWIP_NETIF_HWADDRHINT */


/* Some checks, instead of etharp_init(): */
#if (LWIP_ARP && (ARP_TABLE_SIZE > 0x7f))
  #error "ARP_TABLE_SIZE must fit in an s8_t, you have to reduce it in your lwipopts.h"
#endif


#if ARP_QUEUEING
/**
 * Free a complete queue of etharp entries
 *
 * @param q a qeueue of etharp_q_entry's to free
 */
LWIP_STATIC void
free_etharp_q(struct etharp_q_entry *q)
{
  struct etharp_q_entry *r;
  LWIP_ASSERT("q != NULL", q != NULL);
  LWIP_ASSERT("q->p != NULL", q->p != NULL);
  while (q) {
    r = q;
    q = q->next;
    LWIP_ASSERT("r->p != NULL", (r->p != NULL));
    pbuf_free(r->p);
    memp_free(MEMP_ARP_QUEUE, r);
  }
}
#else /* ARP_QUEUEING */

/** Compatibility define: free the queued pbuf */
#define free_etharp_q(q) pbuf_free(q)

#endif /* ARP_QUEUEING */

/** Clean up ARP table entries */
LWIP_STATIC void
etharp_free_entry(int i)
{
  /* remove from SNMP ARP index tree */
  snmp_delete_arpidx_tree(arp_table[i].netif, &arp_table[i].ipaddr);
  /* and empty packet queue */
  if (arp_table[i].q != NULL) {
    /* remove all queued packets */
    LWIP_DEBUGF(ETHARP_DEBUG, ("etharp_free_entry: freeing entry %"U16_F", packet queue %p.\n", (u16_t)i, (void *)(arp_table[i].q)));
    free_etharp_q(arp_table[i].q);
    arp_table[i].q = NULL;
  }
  /* recycle entry for re-use */
  arp_table[i].state = ETHARP_STATE_EMPTY;
#ifdef LWIP_DEBUG
  /* for debugging, clean out the complete entry */
  arp_table[i].ctime = 0;
  arp_table[i].netif = NULL;
  ip_addr_set_zero(&arp_table[i].ipaddr);
  arp_table[i].ethaddr = ethzero;
#endif /* LWIP_DEBUG */
}

/**
 * Clears expired entries in the ARP table.
 *
 * This function should be called every ETHARP_TMR_INTERVAL milliseconds (5 seconds),
 * in order to expire entries in the ARP table.
 */
void
etharp_tmr(void)
{
  u8_t i;

  LWIP_DEBUGF(ETHARP_DEBUG, ("etharp_timer\n"));
  /* remove expired entries from the ARP table */
  for (i = 0; i < ARP_TABLE_SIZE; ++i) {
    u8_t state = arp_table[i].state;
    if (state != ETHARP_STATE_EMPTY
#if ETHARP_SUPPORT_STATIC_ENTRIES
      && (state != ETHARP_STATE_STATIC)
#endif /* ETHARP_SUPPORT_STATIC_ENTRIES */
      ) {
      arp_table[i].ctime++;
      if ((arp_table[i].ctime >= ARP_MAXAGE) ||
          ((arp_table[i].state == ETHARP_STATE_PENDING)  &&
           (arp_table[i].ctime >= ARP_MAXPENDING))) {
        /* pending or stable entry has become old! */
        LWIP_DEBUGF(ETHARP_DEBUG, ("etharp_timer: expired %s entry %"U16_F".\n",
             arp_table[i].state >= ETHARP_STATE_STABLE ? "stable" : "pending", (u16_t)i));
        /* clean up entries that have just been expired */
        etharp_free_entry(i);
      }
      else if (arp_table[i].state == ETHARP_STATE_STABLE_REREQUESTING) {
        /* Reset state to stable, so that the next transmitted packet will
           re-send an ARP request. */
        arp_table[i].state = ETHARP_STATE_STABLE;
      }

      /* still pending entry? (not expired) */
      if (arp_table[i].state == ETHARP_STATE_PENDING) {
        /* resend an ARP query here? */
        (void)etharp_request(arp_table[i].netif, &arp_table[i].ipaddr);
        LWIP_DEBUGF(ETHARP_DEBUG, ("etharp_timer: resend an ARP query to %s\n",
          ipaddr_ntoa(&(arp_table[i].ipaddr))));

      }
    }
  }
}

/**
 * Search the ARP table for a matching or new entry.
 *
 * If an IP_add is given, return a pending or stable ARP entry that matches
 * the address. If no match is found, create a new entry with this address set,
 * but in state ETHARP_EMPTY. The caller must check and possibly change the
 * state of the returned entry.
 *
 * If ipaddr is NULL, return a initialized new entry in state ETHARP_EMPTY.
 *
 * In all cases, attempt to create new entries from an empty entry. If no
 * empty entries are available and ETHARP_FLAG_TRY_HARD flag is set, recycle
 * old entries. Heuristic choose the least important entry for recycling.
 *
 * @param ipaddr IP_add to find in ARP cache, or to add if not found.
 * @param flags @see definition of ETHARP_FLAG_*
 * @param netif netif related to this address (used for NETIF_HWADDRHINT)
 *
 * @return The ARP entry index that matched or is created, ERR_MEM if no
 * entry is found or could be recycled.
 */
/*lint -e838*/
LWIP_STATIC s8_t
etharp_find_entry(ip_addr_t *ipaddr, u8_t flags)
{
  s8_t old_pending = ARP_TABLE_SIZE, old_stable = ARP_TABLE_SIZE;
  s8_t empty = ARP_TABLE_SIZE, i = 0;
  u16_t age_pending = 0, age_stable = 0;
  /* oldest entry with packets on queue */
  s8_t old_queue = ARP_TABLE_SIZE;
  /* its age */
  u16_t age_queue = 0;

  /**
   * a) do a search through the cache, remember candidates
   * b) select candidate entry
   * c) create new entry
   */

  /* a) in a single search sweep, do all of this
   * 1) remember the first empty entry (if any)
   * 2) remember the oldest stable entry (if any)
   * 3) remember the oldest pending entry without queued packets (if any)
   * 4) remember the oldest pending entry with queued packets (if any)
   * 5) search for a matching IP entry, either pending or stable
   *    until 5 matches, or all entries are searched for.
   */

  for (i = 0; i < ARP_TABLE_SIZE; ++i) {
    u8_t state = arp_table[i].state;
    /* no empty entry found yet and now we do find one? */
    if ((empty == ARP_TABLE_SIZE) && (state == ETHARP_STATE_EMPTY)) {
      LWIP_DEBUGF(ETHARP_DEBUG, ("etharp_find_entry: found empty entry %"U16_F"\n", (u16_t)i));
      /* remember first empty entry */
      empty = i;
    } else if (state != ETHARP_STATE_EMPTY) {
      LWIP_ASSERT("state == ETHARP_STATE_PENDING || state >= ETHARP_STATE_STABLE",
        state == ETHARP_STATE_PENDING || state >= ETHARP_STATE_STABLE);
      /* if given, does IP_add match IP_add in ARP entry? */
      if (ipaddr && ip_addr_cmp(ipaddr, &arp_table[i].ipaddr)) {
        LWIP_DEBUGF(ETHARP_DEBUG | LWIP_DBG_TRACE, ("etharp_find_entry: found matching entry %"U16_F"\n", (u16_t)i));
        /* found exact IP_add match, simply bail out */
        return i;
      }
      /* pending entry? */
      if (state == ETHARP_STATE_PENDING) {
        /* pending with queued packets? */
        if (arp_table[i].q != NULL) {
          if (arp_table[i].ctime >= age_queue) {
            old_queue = i;
            age_queue = arp_table[i].ctime;
          }
        } else
        /* pending without queued packets? */
        {
          if (arp_table[i].ctime >= age_pending) {
            old_pending = i;
            age_pending = arp_table[i].ctime;
          }
        }
      /* stable entry? */
      } else if (state >= ETHARP_STATE_STABLE) {
#if ETHARP_SUPPORT_STATIC_ENTRIES
        /* don't rcrd old_stable for static entries since they never expire */
        if (state < ETHARP_STATE_STATIC)
#endif /* ETHARP_SUPPORT_STATIC_ENTRIES */
        {
          /* remember entry with oldest stable entry in oldest, its age in maxtime */
          if (arp_table[i].ctime >= age_stable) {
            old_stable = i;
            age_stable = arp_table[i].ctime;
          }
        }
      }
    }
  }
  /* { we have no match } => try to create a new entry */
  /* don't create new entry, only search? */
  if (((flags & ETHARP_FLAG_FIND_ONLY) != 0) ||
      /* or no empty entry found and not allowed to recycle? */
      ((empty == ARP_TABLE_SIZE) && ((flags & ETHARP_FLAG_TRY_HARD) == 0))) {
    LWIP_DEBUGF(ETHARP_DEBUG | LWIP_DBG_TRACE, ("etharp_find_entry: no empty entry found and not allowed to recycle\n"));
    return (s8_t)ERR_MEM;
  }

  /* b) choose the least destructive entry to recycle:
   * 1) empty entry
   * 2) oldest stable entry
   * 3) oldest pending entry without queued packets
   * 4) oldest pending entry with queued packets
   *
   * { ETHARP_FLAG_TRY_HARD is set at this point }
   */

  /* 1) empty entry available? */
  if (empty < ARP_TABLE_SIZE) {
    i = empty;
    LWIP_DEBUGF(ETHARP_DEBUG | LWIP_DBG_TRACE, ("etharp_find_entry: selecting empty entry %"U16_F"\n", (u16_t)i));
  } else {
    /* 2) found recyclable stable entry? */
    if (old_stable < ARP_TABLE_SIZE) {
      /* recycle oldest stable*/
      i = old_stable;
      LWIP_DEBUGF(ETHARP_DEBUG | LWIP_DBG_TRACE, ("etharp_find_entry: selecting oldest stable entry %"U16_F"\n", (u16_t)i));
      /* no queued packets should exist on stable entries */
      LWIP_ASSERT("arp_table[i].q == NULL", arp_table[i].q == NULL);
    /* 3) found recyclable pending entry without queued packets? */
    } else if (old_pending < ARP_TABLE_SIZE) {
      /* recycle oldest pending */
      i = old_pending;
      LWIP_DEBUGF(ETHARP_DEBUG | LWIP_DBG_TRACE, ("etharp_find_entry: selecting oldest pending entry %"U16_F" (without queue)\n", (u16_t)i));
    /* 4) found recyclable pending entry with queued packets? */
    } else if (old_queue < ARP_TABLE_SIZE) {
      /* recycle oldest pending (queued packets are free in etharp_free_entry) */
      i = old_queue;
      LWIP_DEBUGF(ETHARP_DEBUG | LWIP_DBG_TRACE, ("etharp_find_entry: selecting oldest pending entry %"U16_F", freeing packet queue %p\n", (u16_t)i, (void *)(arp_table[i].q)));
      /* no empty or recyclable entries found */
    } else {
      LWIP_DEBUGF(ETHARP_DEBUG | LWIP_DBG_TRACE, ("etharp_find_entry: no empty or recyclable entries found\n"));
      return (s8_t)ERR_MEM;
    }

    /* { empty or recyclable entry found } */
    LWIP_ASSERT("i < ARP_TABLE_SIZE", i < ARP_TABLE_SIZE);
    etharp_free_entry(i);
  }

  LWIP_ASSERT("i < ARP_TABLE_SIZE", i < ARP_TABLE_SIZE);
  LWIP_ASSERT("arp_table[i].state == ETHARP_STATE_EMPTY",
    arp_table[i].state == ETHARP_STATE_EMPTY);

  /* IP_add given? */
  if (ipaddr != NULL) {
    /* set IP_add*/
    ip_addr_copy(arp_table[i].ipaddr, *ipaddr);
  }
  arp_table[i].ctime = 0;
  return (err_t)i;
}
/*lint +e838*/

/**
 * Send an IP packet on the network using netif->linkoutput
 * The ethernet header is filled in before sending.
 *
 * @params netif the lwIP network interface on which to send the packet
 * @params p the packet to send, p->payload pointing to the (uninitialized) ethernet header
 * @params src the source MAC_address to be copied into the ethernet header
 * @params dst the destination MAC_address to be copied into the ethernet header
 * @return ERR_OK if the packet was sent, any other err_t on failure
 */
LWIP_STATIC err_t
etharp_send_ip(struct netif *netif, struct pbuf *p, struct eth_addr *src, struct eth_addr *dst)
{
  struct eth_hdr *ethhdr = (struct eth_hdr *)p->payload;

  LWIP_ASSERT("netif->hwaddr_len must be the same as ETHARP_HWADDR_LEN for etharp!",
              (netif->hwaddr_len == ETHARP_HWADDR_LEN));
  (void)ETHADDR32_COPY(&ethhdr->dest, sizeof(struct eth_addr), dst); //CSEC_FIX_2302
  (void)ETHADDR16_COPY(&ethhdr->src, sizeof(struct eth_addr), src);//CSEC_FIX_2302
  /*lint -e845 */
  ethhdr->type = PP_HTONS(ETHTYPE_IP);
  /*lint +e845 */
  LWIP_DEBUGF(ETHARP_DEBUG | LWIP_DBG_TRACE, ("etharp_send_ip: sending packet %p\n", (void *)p));

#if USE_ONLY_PRIMARY_IFACE
  GET_NETIF_FOR_OUPUT(netif);
#endif

  /* send the packet */
  return netif->linkoutput(netif, p);
}

struct etharp_entry* etharp_get_arp_tbl(void)
{
    return arp_table;
}

/**
 * Update (or insert) a IP/MAC_address pair in the ARP cache.
 *
 * If a pending entry is resolved, any queued packets will be sent
 * at this point.
 *
 * @param netif netif related to this entry (used for NETIF_ADDRHINT)
 * @param ipaddr IP_add of the inserted ARP entry.
 * @param ethaddr Ethernet address of the inserted ARP entry.
 * @param flags @see definition of ETHARP_FLAG_*
 *
 * @return
 * - ERR_OK Succesfully updated ARP cache.
 * - ERR_MEM If we could not add a new ARP entry when ETHARP_FLAG_TRY_HARD was set.
 * - ERR_ARG Non-unicast address given, those will not appear in ARP cache.
 *
 * @see pbuf_free()
 */
err_t
etharp_update_arp_entry(struct netif *netif, ip_addr_t *ipaddr, struct eth_addr *ethaddr, u8_t flags)
{
  s8_t i;
  LWIP_ASSERT("netif->hwaddr_len == ETHARP_HWADDR_LEN", netif->hwaddr_len == ETHARP_HWADDR_LEN);
#ifdef  LWIP_DEV_DEBUG
  LWIP_DEBUGF(ETHARP_DEBUG | LWIP_DBG_TRACE, ("etharp_update_arp_entry: %"U16_F".%"U16_F".%"U16_F".%"U16_F" - %02"X16_F":%02"X16_F":%02"X16_F":%02"X16_F":%02"X16_F":%02"X16_F"\n",
    ip4_addr1_16(ipaddr), ip4_addr2_16(ipaddr), ip4_addr3_16(ipaddr), ip4_addr4_16(ipaddr),
    ethaddr->addr[0], ethaddr->addr[1], ethaddr->addr[2],
    ethaddr->addr[3], ethaddr->addr[4], ethaddr->addr[5]));
#endif
  /* non-unicast address? */
  if (ip_addr_isany(ipaddr) ||
      ip_addr_isbroadcast(ipaddr, netif) ||
      /*lint -e845 */
      ip_addr_ismulticast(ipaddr)) {
      /*lint +e845 */
    LWIP_DEBUGF(ETHARP_DEBUG | LWIP_DBG_TRACE, ("etharp_update_arp_entry: will not add non-unicast IP_addr to ARP cache\n"));
    return ERR_ARG;
  }
  /* find or create ARP entry */
  i = etharp_find_entry(ipaddr, flags);
  /* bail out if no entry could be found */
  if (i < 0) {
    return (err_t)i;
  }

#if ETHARP_SUPPORT_STATIC_ENTRIES
  if (flags & ETHARP_FLAG_STATIC_ENTRY) {
    /* rcrd static type */
    arp_table[i].state = ETHARP_STATE_STATIC;
  } else
#endif /* ETHARP_SUPPORT_STATIC_ENTRIES */
  {
    /* mark it stable */
    arp_table[i].state = ETHARP_STATE_STABLE;
  }

  /* rcrd network interface */
  arp_table[i].netif = netif;
  /* insert in SNMP ARP index tree */
  snmp_insert_arpidx_tree(netif, &arp_table[i].ipaddr);

  LWIP_DEBUGF(ETHARP_DEBUG | LWIP_DBG_TRACE, ("etharp_update_arp_entry: updating stable entry %"S16_F"\n", (s16_t)i));
  /* update address */
  (void)ETHADDR32_COPY(&arp_table[i].ethaddr, sizeof(struct eth_addr), ethaddr); //CSEC_FIX_2302
  /* reset time stamp */
  arp_table[i].ctime = 0;
  /* this is where we will send out queued packets! */
#if ARP_QUEUEING
  while (arp_table[i].q != NULL) {
    struct pbuf *p;
    /* remember remainder of queue */
    struct etharp_q_entry *q = arp_table[i].q;
    /* pop first item off the queue */
    arp_table[i].q = q->next;
    /* get the packet pointer */
    p = q->p;
    /* now queue entry can be freed */
    memp_free(MEMP_ARP_QUEUE, q);
#else /* ARP_QUEUEING */
  if (arp_table[i].q != NULL) {
    struct pbuf *p = arp_table[i].q;
    arp_table[i].q = NULL;
#endif /* ARP_QUEUEING */
    /* send the queued IP packet */
    (void)etharp_send_ip(netif, p, (struct eth_addr*)(netif->hwaddr), ethaddr);
    /* free the queued IP packet */
    pbuf_free(p);
  }
  return ERR_OK;
}

/**
 * delete a IP/MAC_address pair in the ARP cache.
 *
 * If a pending entry is resolved, any queued packets will be sent
 * at this point.
 *
 * @param netif netif related to this entry (used for NETIF_ADDRHINT)
 * @param ipaddr IP_add of the inserted ARP entry.
 *
 * @return
 * - ERR_OK Succesfully updated ARP cache..
 * - ERR_ARG Non-unicast address given, or the entry is not found, or the entry
 *   state is not ETHARP_STATE_STABLE.
 *
 */
err_t
etharp_delete_arp_entry(struct netif *netif, ip_addr_t *ipaddr)
{
  s8_t i;
#ifdef  LWIP_DEV_DEBUG
  LWIP_DEBUGF(ETHARP_DEBUG | LWIP_DBG_TRACE, ("etharp_delete_arp_entry: %"U16_F".%"U16_F".%"U16_F".%"U16_F"\n",
    ip4_addr1_16(ipaddr), ip4_addr2_16(ipaddr), ip4_addr3_16(ipaddr), ip4_addr4_16(ipaddr)));
#endif
  /* non-unicast address? */
  if (ip_addr_isany(ipaddr) ||
      ip_addr_isbroadcast(ipaddr, netif) ||
      /*lint -e845 */
      ip_addr_ismulticast(ipaddr)) {
      /*lint +e845 */
    LWIP_DEBUGF(ETHARP_DEBUG | LWIP_DBG_TRACE, ("etharp_update_arp_entry: will not add non-unicast IP_addr to ARP cache\n"));
    return ERR_ARG;
  }
  /* find or create ARP entry */
  i = etharp_find_entry(ipaddr, ETHARP_FLAG_FIND_ONLY);
  /* bail out if no entry could be found */
  if (i < 0) {
    return ERR_ARG;
  }
  if (arp_table[i].state != ETHARP_STATE_STABLE) {
    /* entry wasn't a static entry, cannot remove it */
    return ERR_ARG;
  }
  /* entry found, free it */
  etharp_free_entry(i);
  return ERR_OK;
}

#if ETHARP_SUPPORT_STATIC_ENTRIES
/** Add a new static entry to the ARP table. If an entry exists for the
 * specified IP_add, this entry is overwritten.
 * If packets are queued for the specified IP_add, they are sent out.
 *
 * @param ipaddr IP_add for the new static entry
 * @param ethaddr ethernet address for the new static entry
 * @return @see return values of etharp_add_static_entry
 */
err_t
etharp_add_static_entry(ip_addr_t *ipaddr, struct eth_addr *ethaddr)
{
  struct netif *netif;
#ifdef  LWIP_DEV_DEBUG
  LWIP_DEBUGF(ETHARP_DEBUG | LWIP_DBG_TRACE, ("etharp_add_static_entry: %"U16_F".%"U16_F".%"U16_F".%"U16_F" - %02"X16_F":%02"X16_F":%02"X16_F":%02"X16_F":%02"X16_F":%02"X16_F"\n",
    ip4_addr1_16(ipaddr), ip4_addr2_16(ipaddr), ip4_addr3_16(ipaddr), ip4_addr4_16(ipaddr),
    ethaddr->addr[0], ethaddr->addr[1], ethaddr->addr[2],
    ethaddr->addr[3], ethaddr->addr[4], ethaddr->addr[5]));
#endif

  netif = ip_route(ipaddr, NULL);
  if (netif == NULL) {
    return ERR_RTE;
  }

  return etharp_update_arp_entry(netif, ipaddr, ethaddr, ETHARP_FLAG_TRY_HARD | ETHARP_FLAG_STATIC_ENTRY);
}

/** Remove a static entry from the ARP table previously added with a call to
 * etharp_add_static_entry.
 *
 * @param ipaddr IP_add of the static entry to remove
 * @return ERR_OK: entry removed
 *         ERR_MEM: entry wasn't found
 *         ERR_ARG: entry wasn't a static entry but a dynamic one
 */
err_t
etharp_remove_static_entry(ip_addr_t *ipaddr)
{
  s8_t i;
#ifdef  LWIP_DEV_DEBUG
  LWIP_DEBUGF(ETHARP_DEBUG | LWIP_DBG_TRACE, ("etharp_remove_static_entry: %"U16_F".%"U16_F".%"U16_F".%"U16_F"\n",
    ip4_addr1_16(ipaddr), ip4_addr2_16(ipaddr), ip4_addr3_16(ipaddr), ip4_addr4_16(ipaddr)));
#endif

  /* find or create ARP entry */
  i = etharp_find_entry(ipaddr, ETHARP_FLAG_FIND_ONLY);
  /* bail out if no entry could be found */
  if (i < 0) {
    return (err_t)i;
  }

  if (arp_table[i].state != ETHARP_STATE_STATIC) {
    /* entry wasn't a static entry, cannot remove it */
    return ERR_ARG;
  }
  /* entry found, free it */
  etharp_free_entry(i);
  return ERR_OK;
}
#endif /* ETHARP_SUPPORT_STATIC_ENTRIES */

/**
 * Remove all ARP table entries of the specified netif.
 *
 * @param netif points to a network interface
 */
void etharp_cleanup_netif(struct netif *netif)
{
  u8_t i;

  for (i = 0; i < ARP_TABLE_SIZE; ++i) {
    u8_t state = arp_table[i].state;
    if ((state != ETHARP_STATE_EMPTY) && (arp_table[i].netif == netif)) {
      etharp_free_entry(i);
    }
  }
}

/**
 * Finds (stable) ethernet/IP_add pair from ARP table
 * using interface and IP_add index.
 * @note the addresses in the ARP table are in network order!
 *
 * @param netif points to interface index
 * @param ipaddr points to the (network order) IP_add index
 * @param eth_ret points to return pointer
 * @param ip_ret points to return pointer
 * @return table index if found, -1 otherwise
 */
s8_t
etharp_find_addr(struct netif *netif, ip_addr_t *ipaddr,
         struct eth_addr **eth_ret, ip_addr_t **ip_ret)
{
  s8_t i;

  LWIP_ASSERT("eth_ret != NULL && ip_ret != NULL",
    eth_ret != NULL && ip_ret != NULL);

  LWIP_UNUSED_ARG(netif);

  i = etharp_find_entry(ipaddr, ETHARP_FLAG_FIND_ONLY);
  if((i >= 0) && (arp_table[i].state >= ETHARP_STATE_STABLE)) {
      *eth_ret = &arp_table[i].ethaddr;
      *ip_ret = &arp_table[i].ipaddr;
      return i;
  }
  return -1;
}

#if ETHARP_TRUST_IP_MAC
/**
 * Updates the ARP table using the given IP packet.
 *
 * Uses the incoming IP packet's source address to update the
 * ARP cache for the local network. The function does not alter
 * or free the packet. This function must be called before the
 * packet p is passed to the IP layer.
 *
 * @param netif The lwIP network interface on which the IP packet pbuf arrived.
 * @param p The IP packet that arrived on netif.
 *
 * @return NULL
 *
 * @see pbuf_free()
 */
LWIP_STATIC void
etharp_ip_input(struct netif *netif, struct pbuf *p)
{
  struct eth_hdr *ethhdr;
  struct ip_hdr *iphdr;
  ip_addr_t iphdr_src;
  struct netif *loc_netif;

  LWIP_ERROR("netif != NULL", (netif != NULL), return;);

  /* Only insert an entry if the source IP_add of the
     incoming IP packet comes from a host on the local network. */
  ethhdr = (struct eth_hdr *)p->payload;

#if ETHARP_SUPPORT_VLAN
  if (ethhdr->type == PP_HTONS(ETHTYPE_VLAN)) {
    /* drop the packet as IP header size is small*/
    if (p->len < SIZEOF_ETH_HDR + SIZEOF_VLAN_HDR + IP_HLEN) {
      return ;
    }

    iphdr = (struct ip_hdr *)((u8_t*)ethhdr + SIZEOF_ETH_HDR + SIZEOF_VLAN_HDR);
  }  else {
#endif /* ETHARP_SUPPORT_VLAN */

    /* drop the packet as IP header size is small*/
    if (p->len < SIZEOF_ETH_HDR + IP_HLEN) {
      return ;
    }

    iphdr = (struct ip_hdr *)((u8_t*)ethhdr + SIZEOF_ETH_HDR);
#if ETHARP_SUPPORT_VLAN
  }
#endif

  ip_addr_copy(iphdr_src, iphdr->src);

  /* source is not on the local network? */
  if (!ip_addr_netcmp(&iphdr_src, &(netif->ip_addr), &(netif->netmask))) {
    loc_netif = netif_list;
    while(loc_netif != NULL) {
      if(loc_netif == netif) {
        loc_netif = loc_netif->next;
        continue;
      }

      if((!STRNCMP(loc_netif->name, netif->name, IFNAMSIZ)) && (loc_netif->num == netif->num)) {
        netif = loc_netif;
        break;
      }
      loc_netif = loc_netif->next;
    }
    if(loc_netif == NULL) {
      /* do nothing */
      return;
    }
  }

  LWIP_DEBUGF(ETHARP_DEBUG | LWIP_DBG_TRACE, ("etharp_ip_input: updating ETHARP table.\n"));
  /* update the source IP_add in the cache, if present */
  /* @todo We could use ETHARP_FLAG_TRY_HARD if we think we are going to talk
   * back soon (for example, if the destination IP_add is ours. */
  etharp_update_arp_entry(netif, &iphdr_src, &(ethhdr->src), ETHARP_FLAG_FIND_ONLY);
}
#endif /* ETHARP_TRUST_IP_MAC */

/**
 * Responds to ARP requests to us. Upon ARP replies to us, add entry to cache
 * send out queued IP packets. Updates cache with snooped address pairs.
 *
 * Should be called for incoming ARP packets. The pbuf in the argument
 * is freed by this function.
 *
 * @param netif The lwIP network interface on which the ARP packet pbuf arrived.
 * @param ethaddr Ethernet address of netif.
 * @param p The ARP packet that arrived on netif. Is freed by this function.
 *
 * @return NULL
 *
 * @see pbuf_free()
 */
LWIP_STATIC void
etharp_arp_input(struct netif *netif, struct eth_addr *ethaddr, struct pbuf *p)
{
  struct etharp_hdr *hdr;
  struct eth_hdr *ethhdr;
  /* these are aligned properly, whereas the ARP header fields might not be */
  ip_addr_t sipaddr, dipaddr;
  u8_t for_us;
#if LWIP_AUTOIP
  const u8_t * ethdst_hwaddr;
#endif /* LWIP_AUTOIP */

  struct netif *loc_netif;

  LWIP_ERROR("netif != NULL", (netif != NULL), return;);

  /* drop short ARP packets: we have to check for p->len instead of p->tot_len here
     since a struct etharp_hdr is pointed to p->payload, so it musn't be chained! */
  if (p->len < SIZEOF_ETHARP_PACKET) {
    LWIP_DEBUGF(ETHARP_DEBUG | LWIP_DBG_TRACE | LWIP_DBG_LEVEL_WARNING,
      ("etharp_arp_input: packet dropped, too short (%"S16_F"/%"S16_F")\n", p->tot_len,
      (s16_t)SIZEOF_ETHARP_PACKET));
    ETHARP_STATS_INC(etharp.lenerr);
    ETHARP_STATS_INC(etharp.drop);
    pbuf_free(p);
    return;
  }

  ethhdr = (struct eth_hdr *)p->payload;
  hdr = (struct etharp_hdr *)((u8_t*)ethhdr + SIZEOF_ETH_HDR);
#if ETHARP_SUPPORT_VLAN
  if (ethhdr->type == PP_HTONS(ETHTYPE_VLAN)) {
    hdr = (struct etharp_hdr *)(((u8_t*)ethhdr) + SIZEOF_ETH_HDR + SIZEOF_VLAN_HDR);
  }
#endif /* ETHARP_SUPPORT_VLAN */

  /* RFC 826 "Packet Reception": */
  if (((hdr->hwtype != PP_HTONS(HWTYPE_ETHERNET)) &&  (hdr->hwtype != PP_HTONS(HWTYPE_WIFI))) ||
      (hdr->hwlen != ETHARP_HWADDR_LEN) ||
      (hdr->protolen != sizeof(ip_addr_t)) ||
      /*lint -e845 */
      (hdr->proto != PP_HTONS(ETHTYPE_IP)))  {
      /*lint +e845 */
    LWIP_DEBUGF(ETHARP_DEBUG | LWIP_DBG_TRACE | LWIP_DBG_LEVEL_WARNING,
      ("etharp_arp_input: packet dropped, wrong hw type, hwlen, proto, protolen or ethernet type (%"U16_F"/%"U16_F"/%"U16_F"/%"U16_F")\n",
      hdr->hwtype, hdr->hwlen, hdr->proto, hdr->protolen));
    ETHARP_STATS_INC(etharp.proterr);
    ETHARP_STATS_INC(etharp.drop);
    pbuf_free(p);
    return;
  }
  ETHARP_STATS_INC(etharp.recv);

#if LWIP_AUTOIP
  /* We have to check if a host already has configured our random
   * created link local address and continously check if there is
   * a host with this IP-address so we can detect collisions */
  autoip_arp_reply(netif, hdr);
#endif /* LWIP_AUTOIP */

  /* Copy struct ip_addr2 to aligned ip_addr, to support compilers without
   * structure packing (not using structure copy which breaks strict-aliasing rules). */
  (void)IPADDR2_COPY(&sipaddr, sizeof(sipaddr), &hdr->sipaddr, sizeof(sipaddr)); //CSEC_FIX_2302
  (void)IPADDR2_COPY(&dipaddr, sizeof(dipaddr), &hdr->dipaddr, sizeof(dipaddr)); //CSEC_FIX_2302

  /* this interface is not configured? */
  if (ip_addr_isany(&netif->ip_addr)) {
    for_us = 0;
  } else {
    /* ARP packet directed to us? */
    for_us = (u8_t)ip_addr_cmp(&dipaddr, &(netif->ip_addr));
  }
  if (for_us == 0){
    loc_netif = netif_list;
    while(loc_netif != NULL) {
      if (netif == loc_netif) {
        loc_netif = loc_netif->next;
        continue;
      }

      if((!STRNCMP(loc_netif->name, netif->name, IFNAMSIZ)) && (loc_netif->num == netif->num)) {
        for_us = (u8_t)ip_addr_cmp(&dipaddr, &(loc_netif->ip_addr));
        if (for_us) {
          netif = loc_netif;
          break;
        }
      }
      loc_netif = loc_netif->next;
    }
  }


  /* ARP message directed to us?
      -> add IP_add in ARP cache; assume requester wants to talk to us,
         can result in directly sending the queued packets for this host.
     ARP message not directed to us?
      ->  update the source IP_add in the cache, if present */
  (void)etharp_update_arp_entry(netif, &sipaddr, &(hdr->shwaddr),
                   for_us ? ETHARP_FLAG_TRY_HARD : ETHARP_FLAG_FIND_ONLY);

  /* now act on the message itself */
  switch (hdr->opcode) {
  /* ARP request? */
  /*lint -e845 */
  case PP_HTONS(ARP_REQUEST):
    /*lint +e845 */
    /* ARP request. If it asked for our address, we send out a
     * reply. In any case, we time-stamp any existing ARP entry,
     * and possiby send out an IP packet that was queued on it. */

    LWIP_DEBUGF (ETHARP_DEBUG | LWIP_DBG_TRACE, ("etharp_arp_input: incoming ARP request\n"));
    /* ARP request for our address? */
    if (for_us) {

      LWIP_DEBUGF(ETHARP_DEBUG | LWIP_DBG_TRACE, ("etharp_arp_input: replying to ARP request for our IP_addr\n"));
      /* Re-use pbuf to send ARP reply.
         Since we are re-using an existing pbuf, we can't call etharp_raw since
         that would allocate a new pbuf. */
      hdr->opcode = htons(ARP_REPLY);

      (void)IPADDR2_COPY(&hdr->dipaddr, sizeof(hdr->dipaddr), &hdr->sipaddr, sizeof(hdr->dipaddr)); //CSEC_FIX_2302
      (void)IPADDR2_COPY(&hdr->sipaddr, sizeof(hdr->sipaddr), &netif->ip_addr, sizeof(hdr->sipaddr)); //CSEC_FIX_2302

      LWIP_ASSERT("netif->hwaddr_len must be the same as ETHARP_HWADDR_LEN for etharp!",
                  (netif->hwaddr_len == ETHARP_HWADDR_LEN));
#if LWIP_AUTOIP
      /* If we are using Link-Local, all ARP packets that contain a Link-Local
       * 'sender IP_add' MUST be sent using link-layer brcast instead of
       * link-layer unicast. (See RFC3927 Section 2.5, last paragraph) */
      /*lint -e845 */
      ethdst_hwaddr = ip_addr_islinklocal(&netif->ip_addr) ? (u8_t*)(ethbroadcast.addr) : hdr->shwaddr.addr;
       /*lint +e845 */
#endif /* LWIP_AUTOIP */

      (void)ETHADDR16_COPY(&hdr->dhwaddr, sizeof(struct eth_addr), &hdr->shwaddr); //CSEC_FIX_2302
#if LWIP_AUTOIP
      (void)ETHADDR16_COPY(&ethhdr->dest, sizeof(struct eth_addr), ethdst_hwaddr); //CSEC_FIX_2302
#else  /* LWIP_AUTOIP */
      (void)ETHADDR16_COPY(&ethhdr->dest, sizeof(struct eth_addr), &hdr->shwaddr); //CSEC_FIX_2302
#endif /* LWIP_AUTOIP */
      (void)ETHADDR16_COPY(&hdr->shwaddr, sizeof(struct eth_addr), ethaddr); //CSEC_FIX_2302
      (void)ETHADDR16_COPY(&ethhdr->src, sizeof(struct eth_addr), ethaddr); //CSEC_FIX_2302

      /* hwtype, hwaddr_len, proto, protolen and the type in the ethernet header
         are already correct, we tested that before */

#if USE_ONLY_PRIMARY_IFACE
      GET_NETIF_FOR_OUPUT(netif);
#endif

      /* return ARP reply */
      (void)netif->linkoutput(netif, p);
    /* we are not configured? */
    } else if (ip_addr_isany(&netif->ip_addr)) {
      /* { for_us == 0 and netif->ip_addr.addr == 0 } */
      LWIP_DEBUGF(ETHARP_DEBUG | LWIP_DBG_TRACE, ("etharp_arp_input: we are unconfigured, ARP request ignored.\n"));
    /* request was not directed to us */
    } else {
      /* { for_us == 0 and netif->ip_addr.addr != 0 } */
      LWIP_DEBUGF(ETHARP_DEBUG | LWIP_DBG_TRACE, ("etharp_arp_input: ARP request was not for us.\n"));
    }
    break;
    /*lint -e845 */
  case PP_HTONS(ARP_REPLY):
    /*lint +e845 */
    /* ARP reply. We already updated the ARP cache earlier. */
    LWIP_DEBUGF(ETHARP_DEBUG | LWIP_DBG_TRACE, ("etharp_arp_input: incoming ARP reply\n"));
    if (ip_addr_cmp(&sipaddr, &(netif->ip_addr)))
    {
        LWIP_DEBUGF(ETHARP_DEBUG | LWIP_DBG_TRACE, ("etharp_arp_input: Ip conflict.\n"));
        if (LOS_SemPost(uarpsemlock))
            LWIP_DEBUGF(ETHARP_DEBUG | LWIP_DBG_TRACE, ("etharp_arp_input: uarpsemlock post failed.\n"));
    }

#if (LWIP_DHCP && DHCP_DOES_ARP_CHECK)
    /* DHCP wants to know about ARP replies from any host with an
     * IP_add also offered to us by the DHCP server. We do not
     * want to take a duplicate IP_add on a single network.
     * @todo How should we handle redundant (fail-over) interfaces? */
    dhcp_arp_reply(netif, &sipaddr);
#endif /* (LWIP_DHCP && DHCP_DOES_ARP_CHECK) */
    break;
  default:
    LWIP_DEBUGF(ETHARP_DEBUG | LWIP_DBG_TRACE, ("etharp_arp_input: ARP unknown opcode type %"S16_F"\n", htons(hdr->opcode)));
    ETHARP_STATS_INC(etharp.err);
    break;
  }
  /* free ARP packet */
  pbuf_free(p);
}

/** Just a small helper function that sends a pbuf to an ethernet address
 * in the arp_table specified by the index 'arp_idx'.
 */
LWIP_STATIC err_t
etharp_output_to_arp_index(struct netif *netif, struct pbuf *q, u8_t arp_idx)
{
  LWIP_ASSERT("arp_table[arp_idx].state >= ETHARP_STATE_STABLE",
              arp_table[arp_idx].state >= ETHARP_STATE_STABLE);
  /* if arp table entry is about to expire: re-request it,
     but only if its state is ETHARP_STATE_STABLE to prevent flooding the
     network with ARP requests if this address is used frequently. */
  if ((arp_table[arp_idx].state == ETHARP_STATE_STABLE) &&
      (arp_table[arp_idx].ctime >= ARP_AGE_REREQUEST_USED)) {
    if (etharp_request(netif, &arp_table[arp_idx].ipaddr) == ERR_OK) {
      arp_table[arp_idx].state = ETHARP_STATE_STABLE_REREQUESTING;
    }
  }

  return etharp_send_ip(netif, q, (struct eth_addr*)(netif->hwaddr),
    &arp_table[arp_idx].ethaddr);
}

/**
 * Resolve and fill-in Ethernet address header for outgoing IP packet.
 *
 * For IP multicast and brcast, corresponding Ethernet addresses
 * are selected and the packet is transmitted on the link.
 *
 * For unicast addresses, the packet is submitted to etharp_query(). In
 * case the IP_add is outside the local network, the IP_add of
 * the gateway is used.
 *
 * @param netif The lwIP network interface which the IP packet will be sent on.
 * @param q The pbuf(s) containing the IP packet to be sent.
 * @param ipaddr The IP_add of the packet destination.
 *
 * @return
 * - ERR_RTE No route to destination (no gateway to external networks),
 * or the return type of either etharp_query() or etharp_send_ip().
 */
err_t
etharp_output(struct netif *netif, struct pbuf *q, ip_addr_t *ipaddr)
{
  struct eth_addr *dest;
  struct eth_addr mcastaddr;
  ip_addr_t *dst_addr = ipaddr;

  LWIP_ASSERT("netif != NULL", netif != NULL);
  LWIP_ASSERT("q != NULL", q != NULL);
  LWIP_ASSERT("ipaddr != NULL", ipaddr != NULL);

/* NETIF DRIVER STATUS BEGIN */
#if DRIVER_STATUS_CHECK
  if(netif_is_ready(netif) == 0) {
    LWIP_DEBUGF(ETHARP_DEBUG, ("etharp_output: netif driver not ready\n"));
    /* Do not drop if it is a DHCP buffer */
    if(!(q->flags & PBUF_FLAG_DHCP_BUF)) {
      /* Drop the packet here and return success */
      return ERR_OK;
    }
  }
#endif
/* NETIF DRIVER STATUS END */

  /* make room for Ethernet header - should not fail */
  if (pbuf_header(q, sizeof(struct eth_hdr)) != 0) {
    /* bail out */
    LWIP_DEBUGF(ETHARP_DEBUG | LWIP_DBG_TRACE | LWIP_DBG_LEVEL_SERIOUS,
      ("etharp_output: could not allocate room for header.\n"));
    LINK_STATS_INC(link.lenerr);
    return ERR_BUF;
  }

  /* Determine on destination hardware address. brcasts and multicasts
   * are special, other IP_add are looked up in the ARP table. */

  /* brcast destination IP_add */
  if (ip_addr_isbroadcast(ipaddr, netif)) {
    /* brcast on Ethernet also */
    dest = (struct eth_addr *)&ethbroadcast;
  /* multicast destination IP_add */
    /*lint -e845 */
  } else if (ip_addr_ismulticast(ipaddr)) {
    /*lint +e845 */
    /* Hash IP multicast address to MAC_address.*/
    mcastaddr.addr[0] = LL_MULTICAST_ADDR_0;
    mcastaddr.addr[1] = LL_MULTICAST_ADDR_1;
    mcastaddr.addr[2] = LL_MULTICAST_ADDR_2;
    mcastaddr.addr[3] = ip4_addr2(ipaddr) & 0x7f;
    mcastaddr.addr[4] = ip4_addr3(ipaddr);
    mcastaddr.addr[5] = ip4_addr4(ipaddr);
    /* destination Ethernet address is multicast */
    dest = &mcastaddr;
  /* unicast destination IP_add */
  } else {
    s8_t i;
    /* outside local network? if so, this can neither be a global brcast nor
       a subnet brcast. */
    /*lint -e845 */
    if (!ip_addr_netcmp(ipaddr, &(netif->ip_addr), &(netif->netmask)) &&
        !ip_addr_islinklocal(ipaddr)) {
    /*lint +e845 */
#if LWIP_AUTOIP
      struct ip_hdr *iphdr = (struct ip_hdr*)((u8_t*)q->payload +
        sizeof(struct eth_hdr));
      /* According to RFC 3297, chapter 2.6.2 (Forwarding Rules), a packet with
         a link-local source address must always be "directly to its destination
         on the same physical link. The host MUST NOT send the packet to any
         router for forwarding". */
      if (!ip_addr_islinklocal(&iphdr->src))
#endif /* LWIP_AUTOIP */
      {
        struct netif *loc_netif;

        loc_netif = netif_list;
        while (loc_netif != NULL) {
          if(loc_netif == netif) {
            loc_netif = loc_netif->next;
            continue;
          }
          if((!STRNCMP(netif->name, loc_netif->name, IFNAMSIZ)) && (netif->num == loc_netif->num)) {
            if(ip_addr_netcmp(ipaddr, &(loc_netif->ip_addr), &(loc_netif->netmask))) {
              /* Netmask of the other IP in the interface matches */
              break;
            }
          }
          loc_netif = loc_netif->next;
        }

        if(loc_netif == NULL)
        {
          /* interface has default gateway? */
          if (!ip_addr_isany(&netif->gw)) {
            /* send to hardware address of default gateway IP_add */
            dst_addr = &(netif->gw);
          /* no default gateway available */
          } else {
            /* no route to destination error (default gateway missing) */
            return ERR_RTE;
          }
        }
      }
    }

/*PC Lint Fix: Did not expect positive indentation from line 944*/
/*lint -e539*/
#if LWIP_NETIF_HWADDRHINT
    if (netif->addr_hint != NULL) {
      /* per-pcb cached entry was given */
      u8_t etharp_cached_entry = *(netif->addr_hint);
      if (etharp_cached_entry < ARP_TABLE_SIZE) {
#endif /* LWIP_NETIF_HWADDRHINT */
        if ((arp_table[etharp_cached_entry].state >= ETHARP_STATE_STABLE) &&
            (ip_addr_cmp(dst_addr, &arp_table[etharp_cached_entry].ipaddr))) {
          /* the per-pcb-cached entry is stable and the right one! */
          ETHARP_STATS_INC(etharp.cachehit);
          return etharp_output_to_arp_index(netif, q, etharp_cached_entry);
        }
#if LWIP_NETIF_HWADDRHINT
      }
    }
#endif /* LWIP_NETIF_HWADDRHINT */
/*lint +e539*/

    /* find stable entry: do this here since this is a critical path for
       throughput and etharp_find_entry() is kind of slow */
    for (i = 0; i < ARP_TABLE_SIZE; i++) {
      if ((arp_table[i].state >= ETHARP_STATE_STABLE) &&
          (ip_addr_cmp(dst_addr, &arp_table[i].ipaddr))) {
        /* found an existing, stable entry */
        /* PC Lint Fix for: Loss of sign (assignment) (signed char to unsigned char) */
        /* possible value of i : 0 to 10, so typecast to unsigned char */
        ETHARP_SET_HINT(netif, (u8_t)i);
        return etharp_output_to_arp_index(netif, q, (u8_t)i);
      }
    }
    /* no stable entry found, use the (slower) query function:
       queue on destination Ethernet address belonging to ipaddr */
    return etharp_query(netif, dst_addr, q);
  }

  /* continuation for multicast/brcast destinations */
  /* obtain source Ethernet address of the given interface */
  /* send packet directly on the link */
  return etharp_send_ip(netif, q, (struct eth_addr*)(netif->hwaddr), dest);
}

/**
 * Send an ARP request for the given IP_add and/or queue a packet.
 *
 * If the IP_add was not yet in the cache, a pending ARP cache entry
 * is added and an ARP request is sent for the given address. The packet
 * is queued on this entry.
 *
 * If the IP_add was already pending in the cache, a new ARP request
 * is sent for the given address. The packet is queued on this entry.
 *
 * If the IP_add was already stable in the cache, and a packet is
 * given, it is directly sent and no ARP request is sent out.
 *
 * If the IP_add was already stable in the cache, and no packet is
 * given, an ARP request is sent out.
 *
 * @param netif The lwIP network interface on which ipaddr
 * must be queried for.
 * @param ipaddr The IP_add to be resolved.
 * @param q If non-NULL, a pbuf that must be delivered to the IP_add.
 * q is not freed by this function.
 *
 * @note q must only be ONE packet, not a packet queue!
 *
 * @return
 * - ERR_BUF Could not make room for Ethernet header.
 * - ERR_MEM Hardware address unknown, and no more ARP entries available
 *   to query for address or queue the packet.
 * - ERR_MEM Could not queue packet due to memory shortage.
 * - ERR_RTE No route to destination (no gateway to external networks).
 * - ERR_ARG Non-unicast address given, those will not appear in ARP cache.
 *
 */
err_t
etharp_query(struct netif *netif, ip_addr_t *ipaddr, struct pbuf *q)
{
  struct eth_addr * srcaddr = (struct eth_addr *)netif->hwaddr;
  err_t result = ERR_MEM;
  s8_t i; /* ARP entry index */

  /* non-unicast address? */
  if (ip_addr_isbroadcast(ipaddr, netif) ||
      /*lint -e845 */
      ip_addr_ismulticast(ipaddr) ||
      ip_addr_isany(ipaddr)) {
    LWIP_DEBUGF(ETHARP_DEBUG | LWIP_DBG_TRACE, ("etharp_query: will not add non-unicast IP_addr to ARP cache\n"));
      /*lint +e845 */
    return ERR_ARG;
  }

  /* find entry in ARP cache, ask to create entry if queueing packet */
  i = etharp_find_entry(ipaddr, ETHARP_FLAG_TRY_HARD);

  /* could not find or create entry? */
  if (i < 0) {
    LWIP_DEBUGF(ETHARP_DEBUG | LWIP_DBG_TRACE, ("etharp_query: could not create ARP entry\n"));
    if (q) {
      LWIP_DEBUGF(ETHARP_DEBUG | LWIP_DBG_TRACE, ("etharp_query: packet dropped\n"));
      ETHARP_STATS_INC(etharp.memerr);
    }
    return (err_t)i;
  }

  /* do we have a pending entry? or an implicit query request? */
  if ((arp_table[i].state == ETHARP_STATE_EMPTY) || (q == NULL)) {
    /* try to resolve it; send out ARP request */
    arp_table[i].netif  = netif;
    arp_table[i].state = ETHARP_STATE_PENDING;
    ip_addr_copy(arp_table[i].ipaddr, *ipaddr);
    /* Since ARP request will be re-send in etharp_tmr if failed, We don't
       check the ret value and still queue the pbuf */
    result = etharp_request(netif, ipaddr);
    if (q == NULL) {
      return result;
    }
  }

  /* { i is either a STABLE or (new or existing) PENDING entry } */
  LWIP_ASSERT("arp_table[i].state == PENDING or STABLE",
  ((arp_table[i].state == ETHARP_STATE_PENDING) ||
   (arp_table[i].state >= ETHARP_STATE_STABLE)));

  /* packet given? */
  LWIP_ASSERT("q != NULL", q != NULL);
  /* stable entry? */
  if (arp_table[i].state >= ETHARP_STATE_STABLE) {
    /* we have a valid IP->Ethernet address mapping */
    /* PC Lint Fix for: Loss of sign (assignment) (signed char to unsigned char) */
    /* possible value of i : > 0, so typecast to unsigned char */
    ETHARP_SET_HINT(netif, (u8_t)i);
    /* send the packet */
    result = etharp_send_ip(netif, q, srcaddr, &(arp_table[i].ethaddr));
  /* pending entry? (either just created or already pending */
  } else if (arp_table[i].state == ETHARP_STATE_PENDING) {
    /* entry is still pending, queue the given packet 'q' */
    struct pbuf *p;
    u16_t plen = 0;
    /* IF q includes a PBUF_REF, PBUF_POOL or PBUF_RAM, we have no choice but
     * to copy the whole queue into a new PBUF_RAM (see bug #11400)
     * PBUF_ROMs can be left as they are, since ROM must not get changed. */
    p = q;
    while (p) {
      LWIP_ASSERT("no packet queues allowed!", (p->len != p->tot_len) || (p->next == 0));
      if(p->type != PBUF_ROM) {
        plen = p->tot_len;
        break;
      }
      p = p->next;
    }
    if(plen) {
      /* copy the whole packet into new pbufs */
      /* PC Lint Fix for: Possible use of null pointer 'p' in left argument to operator '-&gt */
      /* In below statement, p->tot_len is replaced by plen (value assigned above) */
#if LWIP_NETIF_TX_SINGLE_PBUF
      p = pbuf_alloc(PBUF_LINK, q->len, PBUF_RAM);
#else
      p = pbuf_alloc(PBUF_RAW, q->tot_len, PBUF_RAM);
#endif
      if(p != NULL) {
        if (pbuf_copy(p, q) != ERR_OK) {
          pbuf_free(p);
          p = NULL;
        }
      }
    } else {
      /* referencing the old pbuf is enough */
      p = q;
      pbuf_ref(p);
    }
    /* packet could be taken over? */
    if (p != NULL) {
      /* queue packet ... */
#if ARP_QUEUEING
      struct etharp_q_entry *new_entry;
      /* allocate a new arp queue entry */
      new_entry = (struct etharp_q_entry *)memp_malloc(MEMP_ARP_QUEUE);
      if (new_entry != NULL) {
        new_entry->next = 0;
        new_entry->p = p;
        if(arp_table[i].q != NULL) {
          /* queue was already existent, append the new entry to the end */
          struct etharp_q_entry *r;
          r = arp_table[i].q;
          while (r->next != NULL) {
            r = r->next;
          }
          r->next = new_entry;
        } else {
          /* queue did not exist, first item in queue */
          arp_table[i].q = new_entry;
        }
        LWIP_DEBUGF(ETHARP_DEBUG | LWIP_DBG_TRACE, ("etharp_query: queued packet %p on ARP entry %"S16_F"\n", (void *)q, (s16_t)i));
        result = ERR_OK;
      } else {
        /* the pool MEMP_ARP_QUEUE is empty */
        pbuf_free(p);
        LWIP_DEBUGF(ETHARP_DEBUG | LWIP_DBG_TRACE, ("etharp_query: could not queue a copy of PBUF_REF packet %p (out of memory)\n", (void *)q));
        result = ERR_MEM;
      }
#else /* ARP_QUEUEING */
      /* always queue one packet per ARP request only, freeing a previously queued packet */
      if (arp_table[i].q != NULL) {
        LWIP_DEBUGF(ETHARP_DEBUG | LWIP_DBG_TRACE, ("etharp_query: dropped previously queued packet %p for ARP entry %"S16_F"\n", (void *)q, (s16_t)i));
        pbuf_free(arp_table[i].q);
      }
      arp_table[i].q = p;
      result = ERR_OK;
      LWIP_DEBUGF(ETHARP_DEBUG | LWIP_DBG_TRACE, ("etharp_query: queued packet %p on ARP entry %"S16_F"\n", (void *)q, (s16_t)i));
#endif /* ARP_QUEUEING */
    } else {
      ETHARP_STATS_INC(etharp.memerr);
      LWIP_DEBUGF(ETHARP_DEBUG | LWIP_DBG_TRACE, ("etharp_query: could not queue a copy of PBUF_REF packet %p (out of memory)\n", (void *)q));
      result = ERR_MEM;
    }
  }
  return result;
}

/**
 * Send a raw ARP packet (opcode and all addresses can be modified)
 *
 * @param netif the lwip network interface on which to send the ARP packet
 * @param ethsrc_addr the source MAC_address for the ethernet header
 * @param ethdst_addr the destination MAC_address for the ethernet header
 * @param hwsrc_addr the source MAC_address for the ARP protocol header
 * @param ipsrc_addr the source IP_add for the ARP protocol header
 * @param hwdst_addr the destination MAC_address for the ARP protocol header
 * @param ipdst_addr the destination IP_add for the ARP protocol header
 * @param opcode the type of the ARP packet
 * @return ERR_OK if the ARP packet has been sent
 *         ERR_MEM if the ARP packet couldn't be allocated
 *         any other err_t on failure
 */
/*lint -e438 -e838*/
#if !LWIP_AUTOIP
LWIP_STATIC
#endif /* LWIP_AUTOIP */
err_t
etharp_raw(struct netif *netif, const struct eth_addr *ethsrc_addr,
           const struct eth_addr *ethdst_addr,
           const struct eth_addr *hwsrc_addr, const ip_addr_t *ipsrc_addr,
           const struct eth_addr *hwdst_addr, const ip_addr_t *ipdst_addr,
           const u16_t opcode)
{
  struct pbuf *p;
  err_t result = ERR_OK;
  struct eth_hdr *ethhdr;
  struct etharp_hdr *hdr;
#if LWIP_AUTOIP
  const u8_t * ethdst_hwaddr;
#endif /* LWIP_AUTOIP */

  LWIP_ASSERT("netif != NULL", netif != NULL);

  /* allocate a pbuf for the outgoing ARP request packet */
  p = pbuf_alloc(PBUF_LINK, SIZEOF_ETHARP_HDR, PBUF_RAM);
  /* could allocate a pbuf for an ARP request? */
  if (p == NULL) {
    LWIP_DEBUGF(ETHARP_DEBUG | LWIP_DBG_TRACE | LWIP_DBG_LEVEL_SERIOUS,
      ("etharp_raw: could not allocate pbuf for ARP request.\n"));
    ETHARP_STATS_INC(etharp.memerr);
    return ERR_MEM;
  }

  if( pbuf_header(p, SIZEOF_ETH_HDR) ) {
    LWIP_DEBUGF(ETHARP_DEBUG | LWIP_DBG_TRACE | LWIP_DBG_LEVEL_SERIOUS,
      ("etharp_raw: not enough room for arp header in pbuf\n"));
    pbuf_free(p);
    ETHARP_STATS_INC(etharp.memerr);
    return ERR_MEM;
  }
  LWIP_ASSERT("check that first pbuf can hold struct etharp_hdr",
              (p->len >= SIZEOF_ETHARP_PACKET));

  ethhdr = (struct eth_hdr *)p->payload;
  hdr = (struct etharp_hdr *)((u8_t*)ethhdr + SIZEOF_ETH_HDR);
  LWIP_DEBUGF(ETHARP_DEBUG | LWIP_DBG_TRACE, ("etharp_raw: sending raw ARP packet.\n"));
  hdr->opcode = htons(opcode);

  LWIP_ASSERT("netif->hwaddr_len must be the same as ETHARP_HWADDR_LEN for etharp!",
              (netif->hwaddr_len == ETHARP_HWADDR_LEN));
#if LWIP_AUTOIP
  /* If we are using Link-Local, all ARP packets that contain a Link-Local
   * 'sender IP_add' MUST be sent using link-layer brcast instead of
   * link-layer unicast. (See RFC3927 Section 2.5, last paragraph) */
  /*lint -e845 */
  ethdst_hwaddr = ip_addr_islinklocal(ipsrc_addr) ? (u8_t*)(ethbroadcast.addr) : ethdst_addr->addr;
  /*lint +e845 */
#endif /* LWIP_AUTOIP */
  /* Write the ARP MAC-Addresses */
  (void)ETHADDR16_COPY(&hdr->shwaddr, sizeof(struct eth_addr), hwsrc_addr); //CSEC_FIX_2302
  (void)ETHADDR16_COPY(&hdr->dhwaddr, sizeof(struct eth_addr), hwdst_addr); //CSEC_FIX_2302
  /* Write the Ethernet MAC-Addresses */
#if LWIP_AUTOIP
  (void)ETHADDR16_COPY(&ethhdr->dest, sizeof(struct eth_addr), ethdst_hwaddr); //CSEC_FIX_2302
#else  /* LWIP_AUTOIP */
  (void)ETHADDR16_COPY(&ethhdr->dest, sizeof(struct eth_addr), ethdst_addr); //CSEC_FIX_2302
#endif /* LWIP_AUTOIP */
  (void)ETHADDR16_COPY(&ethhdr->src, sizeof(struct eth_addr), ethsrc_addr); //CSEC_FIX_2302
  /* Copy struct ip_addr2 to aligned ip_addr, to support compilers without
   * structure packing. */
  (void)IPADDR2_COPY(&hdr->sipaddr, sizeof(hdr->sipaddr), ipsrc_addr, sizeof(hdr->sipaddr)); //CSEC_FIX_2302
  (void)IPADDR2_COPY(&hdr->dipaddr, sizeof(hdr->dipaddr), ipdst_addr, sizeof(hdr->dipaddr)); //CSEC_FIX_2302

#if LWIP_ALWAYS_SEND_HWTYPE_AS_ETHER_IN_ARP
  /*lint -e845*/
  hdr->hwtype = PP_HTONS(HWTYPE_ETHERNET);
  /*lint +e845*/
#else
  hdr->hwtype = PP_HTONS(netif->link_layer_type);
#endif

  /*lint -e845 */
  hdr->proto = PP_HTONS(ETHTYPE_IP);
  /*lint +e845 */

  /* set hwlen and protolen */
  hdr->hwlen = ETHARP_HWADDR_LEN;
  hdr->protolen = sizeof(ip_addr_t);

#if LWIP_ALWAYS_SEND_HWTYPE_AS_ETHER_IN_ARP
  ethhdr->type = PP_HTONS(ETHTYPE_ARP);
#else
  ethhdr->type = PP_HTONS(netif->link_layer_type);
#endif

#if USE_ONLY_PRIMARY_IFACE
  GET_NETIF_FOR_OUPUT(netif);
#endif

  /* send ARP query */
  result = netif->linkoutput(netif, p);
  ETHARP_STATS_INC(etharp.xmit);
  /* free ARP query packet */
  pbuf_free(p);
  p = NULL;
  /* could not allocate pbuf for ARP request */

  return result;
}
/*lint +e438 +e838*/

/**
 * Send an ARP request packet asking for ipaddr.
 *
 * @param netif the lwip network interface on which to send the request
 * @param ipaddr the IP_add for which to ask
 * @return ERR_OK if the request has been sent
 *         ERR_MEM if the ARP packet couldn't be allocated
 *         any other err_t on failure
 */
err_t
etharp_request(struct netif *netif, ip_addr_t *ipaddr)
{
  LWIP_DEBUGF(ETHARP_DEBUG | LWIP_DBG_TRACE, ("etharp_request: sending ARP request.\n"));
  return etharp_raw(netif, (struct eth_addr *)netif->hwaddr, &ethbroadcast,
                    (struct eth_addr *)netif->hwaddr, &netif->ip_addr, &ethzero,
                    ipaddr, ARP_REQUEST);
}
#endif /* LWIP_ARP */

/**
 * Process received ethernet frames. Using this function instead of directly
 * calling ip_input and passing ARP frames through etharp in ethernetif_input,
 * the ARP cache is protected from concurrent access.
 *
 * @param p the recevied packet, p->payload pointing to the ethernet header
 * @param netif the network interface on which the packet was received
 */
err_t
ethernet_input(struct pbuf *p, struct netif *netif)
{
  struct eth_hdr* ethhdr;
  u16_t type;
#if LWIP_ARP || ETHARP_SUPPORT_VLAN
  s16_t ip_hdr_offset = SIZEOF_ETH_HDR;
#endif /* LWIP_ARP || ETHARP_SUPPORT_VLAN */
#if LWIP_NETIF_PROMISC
  u8_t is_otherhost = lwIP_FALSE;
#endif

  if (p->len <= SIZEOF_ETH_HDR) {
    /* a packet with only an ethernet header (or less) is not valid for us */
    ETHARP_STATS_INC(etharp.proterr);
    ETHARP_STATS_INC(etharp.drop);
    goto free_and_return;
  }

  /* points to packet payload, which starts with an Ethernet header */
  ethhdr = (struct eth_hdr *)p->payload;
  LWIP_DEBUGF(ETHARP_DEBUG | LWIP_DBG_TRACE,
    ("ethernet_input: dest:%"X8_F":%"X8_F":%"X8_F":%"X8_F":%"X8_F":%"X8_F", src:%"X8_F":%"X8_F":%"X8_F":%"X8_F":%"X8_F":%"X8_F", type:%"X16_F"\n",
     (unsigned)ethhdr->dest.addr[0], (unsigned)ethhdr->dest.addr[1], (unsigned)ethhdr->dest.addr[2],
     (unsigned)ethhdr->dest.addr[3], (unsigned)ethhdr->dest.addr[4], (unsigned)ethhdr->dest.addr[5],
     (unsigned)ethhdr->src.addr[0], (unsigned)ethhdr->src.addr[1], (unsigned)ethhdr->src.addr[2],
     (unsigned)ethhdr->src.addr[3], (unsigned)ethhdr->src.addr[4], (unsigned)ethhdr->src.addr[5],
     (unsigned)htons(ethhdr->type)));
     
  if (ethhdr->src.addr[0] & 1) {
    /* src mac should valid for us */
    goto free_and_return;
  }

#if PF_PKT_SUPPORT
  if(raw_pkt_input(p, netif)) {
    LWIP_DEBUGF(TCPIP_DEBUG, ("ethernet_input: packet eaten by raw_pkt_input \n"));
  }

#if LWIP_NETIF_PROMISC
  if (!(p->flags & (PBUF_FLAG_LLMCAST | PBUF_FLAG_LLBCAST | PBUF_FLAG_HOST))) {
    is_otherhost = lwIP_TRUE;
  }

  /* If PROMISC mode enable, drop the packet for other host no need to process further */
  if((netif->flags & NETIF_FLAG_PROMISC) && is_otherhost) {
    /* silently ignore this packet: not for us*/
      pbuf_free(p);
      return ERR_OK;
  }
#endif /* LWIP_NETIF_PROMISC */
#endif

  type = ethhdr->type;
#if ETHARP_SUPPORT_VLAN
  if (type == PP_HTONS(ETHTYPE_VLAN)) {
    struct eth_vlan_hdr *vlan = (struct eth_vlan_hdr*)(((char*)ethhdr) + SIZEOF_ETH_HDR);
    if (p->len <= SIZEOF_ETH_HDR + SIZEOF_VLAN_HDR) {
      /* a packet with only an ethernet/vlan header (or less) is not valid for us */
      ETHARP_STATS_INC(etharp.proterr);
      ETHARP_STATS_INC(etharp.drop);
      goto free_and_return;
    }
#if defined(ETHARP_VLAN_CHECK) || defined(ETHARP_VLAN_CHECK_FN) /* if not, allow all VLANs */
#ifdef ETHARP_VLAN_CHECK_FN
    if (!ETHARP_VLAN_CHECK_FN(ethhdr, vlan)) {
#elif defined(ETHARP_VLAN_CHECK)
    if (VLAN_ID(vlan) != ETHARP_VLAN_CHECK) {
#endif
      /* silently ignore this packet: not for our VLAN */
      pbuf_free(p);
      return ERR_OK;
    }
#endif /* defined(ETHARP_VLAN_CHECK) || defined(ETHARP_VLAN_CHECK_FN) */
    type = vlan->tpid;
    ip_hdr_offset = SIZEOF_ETH_HDR + SIZEOF_VLAN_HDR;
  }
#endif /* ETHARP_SUPPORT_VLAN */

#if LWIP_ARP_FILTER_NETIF
  netif = LWIP_ARP_FILTER_NETIF_FN(p, netif, htons(type));
#endif /* LWIP_ARP_FILTER_NETIF*/

#if PF_PKT_SUPPORT
  /* Moved above */
#else
  if (ethhdr->dest.addr[0] & 1) {
    /* this might be a multicast or brcast packet */
    if (ethhdr->dest.addr[0] == LL_MULTICAST_ADDR_0) {
      if ((ethhdr->dest.addr[1] == LL_MULTICAST_ADDR_1) &&
          (ethhdr->dest.addr[2] == LL_MULTICAST_ADDR_2)) {
        /* mark the pbuf as link-layer multicast */
        p->flags |= PBUF_FLAG_LLMCAST;
      }
    } else if (eth_addr_cmp(&ethhdr->dest, &ethbroadcast)) {
      /* mark the pbuf as link-layer brcast */
      p->flags |= PBUF_FLAG_LLBCAST;
    }
  }
#endif
  switch (type) {
#if LWIP_ARP
    /* IP packet? */
    /*lint -e845 */
    case PP_HTONS(ETHTYPE_IP):
    /*lint +e845 */
      if (!(netif->flags & NETIF_FLAG_ETHARP)) {
        goto free_and_return;
      }
#if ETHARP_TRUST_IP_MAC
      /* update ARP table */
      etharp_ip_input(netif, p);
#endif /* ETHARP_TRUST_IP_MAC */
      /* skip Ethernet header */
      if(pbuf_header(p, (s16_t)(-ip_hdr_offset))) {
        LWIP_ASSERT("Can't move over header in packet", 0);
        goto free_and_return;
      } else {
        /* pass to IP layer */
        (void)ip_input(p, netif);
      }
      break;

    case PP_HTONS(ETHTYPE_ARP):
      if (!(netif->flags & NETIF_FLAG_ETHARP)) {
        goto free_and_return;
      }
      /* pass p to ARP module */
      etharp_arp_input(netif, (struct eth_addr*)(netif->hwaddr), p);
      break;
#endif /* LWIP_ARP */
#if PPPOE_SUPPORT
    case PP_HTONS(ETHTYPE_PPPOEDISC): /* PPP Over Ethernet Discovery Stage */
      pppoe_disc_input(netif, p);
      break;

    case PP_HTONS(ETHTYPE_PPPOE): /* PPP Over Ethernet Session Stage */
      pppoe_data_input(netif, p);
      break;
#endif /* PPPOE_SUPPORT */

    default:
      ETHARP_STATS_INC(etharp.proterr);
      ETHARP_STATS_INC(etharp.drop);
      goto free_and_return;
  }

  /* This means the pbuf is freed or consumed,
     so the caller doesn't have to free it again */
  return ERR_OK;

free_and_return:
  pbuf_free(p);
  return ERR_OK;
}
#endif /* LWIP_ARP || LWIP_ETHERNET */
