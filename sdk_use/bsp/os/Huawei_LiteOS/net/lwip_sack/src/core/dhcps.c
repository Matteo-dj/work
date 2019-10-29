/**
 * @file
 * Dynamic Host Configuration Protocol Server
 *
 */

/**********************************************************************************
 * Copyright (c) <2013-2015>, <Huawei Technologies Co., Ltd>
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice, this
 * list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 * this list of conditions and the following disclaimer in the documentation and/or
 * other materials provided with the distribution.
 * 3. Neither the name of the copyright holder nor the names of its contributors
 * may be used to endorse or promote products derived from this software without
 * specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 * IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT,
 * INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY
 * OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 **********************************************************************************/

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

#if (LWIP_DHCP) && (LWIP_DHCPS) /* don't build if not configured for use in lwipopts.h */

#include "lwip/stats.h"
#include "lwip/mem.h"
#include "lwip/udp.h"
#include "lwip/ip_addr.h"
#include "lwip/netif.h"
#include "lwip/def.h"
#include "lwip/dhcp.h"
#include "lwip/sys.h"
#include "netif/etharp.h"
#include <string.h>


#define DHCPS_ADDRESS_FREE 0x0
#define DHCPS_ADDRESS_OFFERRED 0x1
#define DHCPS_ADDRESS_BOUND 0x2
#define DHCPS_ADDRESS_DECLINED 0x3


LWIP_STATIC struct pbuf *dhcps_create_base_msg(struct dhcp_msg *client_msg)
{
  struct pbuf  *srvr_msg_pbuf;
  struct dhcp_msg *srvr_msg;

  srvr_msg_pbuf = pbuf_alloc(PBUF_TRANSPORT, sizeof(struct dhcp_msg), PBUF_RAM);
  if(NULL == srvr_msg_pbuf)
  {
    LWIP_DEBUGF(DHCP_DEBUG | LWIP_DBG_TRACE | LWIP_DBG_LEVEL_SERIOUS,
    ("dhcps_create_base_msg(): could not allocate pbuf\n"));
    return NULL;
  }

  LWIP_ASSERT("dhcps_create_base_msg: check that first pbuf can hold struct dhcp_msg",
  (srvr_msg_pbuf->len >= sizeof(struct dhcp_msg)));

/* NETIF DRIVER STATUS CHANGE BEGIN */
#if DRIVER_STATUS_CHECK
  srvr_msg_pbuf->flags |= PBUF_FLAG_DHCP_BUF;
#endif
/* NETIF DRIVER STATUS CHANGE END */

  srvr_msg = (struct dhcp_msg *)srvr_msg_pbuf->payload;
  srvr_msg->op = DHCP_BOOTREPLY;
  srvr_msg->htype = client_msg->htype;
  srvr_msg->hlen = client_msg->hlen;
  srvr_msg->hops = 0;
  srvr_msg->xid = client_msg->xid;
  srvr_msg->secs = 0;
  srvr_msg->flags = client_msg->flags;
  ip_addr_set_zero(&srvr_msg->ciaddr);
  ip_addr_set_zero(&srvr_msg->yiaddr);
  ip_addr_set_zero(&srvr_msg->siaddr);
  ip_addr_copy(srvr_msg->giaddr, client_msg->giaddr);
  (void)MEMCPY_S(srvr_msg->chaddr, sizeof(srvr_msg->chaddr), client_msg->chaddr, DHCP_CHADDR_LEN); //CSEC_FIX_2302
  (void)MEMSET_S(srvr_msg->sname, sizeof(srvr_msg->sname), 0, DHCP_SNAME_LEN); //CSEC_FIX_2302
  (void)MEMSET_S(srvr_msg->file, sizeof(srvr_msg->file), 0,DHCP_FILE_LEN); //CSEC_FIX_2302
  srvr_msg->cookie = PP_HTONL(DHCP_MAGIC_COOKIE);

  return srvr_msg_pbuf;
}


/*lint -e838*/
LWIP_STATIC void remove_stale_entries(struct dhcps *dhcps)
{
  int i=0;
  u32_t curr_time = sys_now();

  for(i=0; i<LWIP_DHCPS_MAX_LEASE;i++)
  {
    /* Slot should not be free or have infinite lease time */
    if((DHCPS_ADDRESS_FREE != dhcps->leasearr[i].flags) && ((u32_t)~0 != dhcps->leasearr[i].leasetime))
    {
      if(dhcps->leasearr[i].leasetime < curr_time)
      {
        LWIP_DEBUGF(DHCP_DEBUG | LWIP_DBG_TRACE, ("remove_stale_entries: Removing Client Entry at Index = %"U32_F"\n",i));
        /*lint -e866*/
        (void)MEMSET_S(&(dhcps->leasearr[i]), sizeof(dhcps->leasearr[i]), 0, sizeof(dhcps->leasearr[i])); //CSEC_FIX_2302
        /*lint +e866*/
        dhcps->leasearr[i].flags = DHCPS_ADDRESS_FREE;
      }
    }
  }
}
/*lint +e838*/

LWIP_STATIC void add_client_entry(struct dhcps *dhcps, unsigned int index, struct dhcp_msg *client_msg)
{
  u32_t client_lease_time = (u32_t)(LWIP_DHCPS_LEASE_TIME);

  if((dhcp_option_given(NULL, DHCP_OPTION_IDX_LEASE_TIME))
#if (~0 != LWIP_DHCPS_LEASE_TIME)
  && (dhcp_get_option_value(NULL, DHCP_OPTION_IDX_LEASE_TIME) < LWIP_DHCPS_LEASE_TIME)
#endif
  )
  {
    client_lease_time = (u32_t)dhcp_get_option_value(NULL, DHCP_OPTION_IDX_LEASE_TIME);
  }

  (void)MEMSET_S(&(dhcps->leasearr[index]), sizeof(struct dyn_lease_addr),
			0, sizeof(struct dyn_lease_addr)); //CSEC_FIX_2302
  (void)MEMCPY_S(dhcps->leasearr[index].cli_hwaddr, DHCP_CHADDR_LEN, client_msg->chaddr, client_msg->hlen); //CSEC_FIX_2302
  /** This is called only during offer message, so adding offer time. This is later updated to lease time when request message is handled */
  dhcps->leasearr[index].leasetime = sys_now() + (LWIP_DHCPS_OFFER_TIME * 1000);
  dhcps->leasearr[index].cli_addr.addr = dhcps->start_addr.addr + index;
  dhcps->leasearr[index].flags = DHCPS_ADDRESS_OFFERRED;
  dhcps->leasearr[index].proposed_leasetime = client_lease_time;
  LWIP_DEBUGF(DHCP_DEBUG | LWIP_DBG_TRACE, ("add_client_entry: Adding Client Entry at Index = %"U32_F"\n",index));

}

/*lint -e838*/
LWIP_STATIC int find_free_slot(struct dhcps *dhcps)
{
  int i = 0;

  for(i=0; i<LWIP_DHCPS_MAX_LEASE; i++)
  {
    if(dhcps->leasearr[i].flags == DHCPS_ADDRESS_FREE)
    {
      LWIP_DEBUGF(DHCP_DEBUG | LWIP_DBG_TRACE, ("find_free_slot: Found Free Slot at Index = %"U32_F"\n",i));
      return i;
    }
  }

  return -1;
}
/*lint +e838*/

/*lint -e838*/
LWIP_STATIC struct dyn_lease_addr *find_client_lease(struct dhcps *dhcps, struct dhcp_msg *client_msg)
{
  int i=0;

  for(i=0; i<LWIP_DHCPS_MAX_LEASE;i++)
  {
    if(dhcps->leasearr[i].flags !=DHCPS_ADDRESS_FREE)
    {
      if(0==memcmp(dhcps->leasearr[i].cli_hwaddr, client_msg->chaddr, client_msg->hlen))
      {
        LWIP_DEBUGF(DHCP_DEBUG | LWIP_DBG_TRACE, ("find_client_lease: Found Client Lease at Index = %"U32_F"\n",i));
        return &(dhcps->leasearr[i]);
      }
    }
  }

  return NULL;
}
/*lint +e838*/

LWIP_STATIC ip_addr_t validate_discover(struct dhcps *dhcps, struct dhcp_msg *client_msg, struct dyn_lease_addr **client_lease)
{
  ip_addr_t client_ip;
  int index = -1;

  LWIP_DEBUGF(DHCP_DEBUG | LWIP_DBG_TRACE, ("validate_discover: Validating Discover Message\n"));

  client_ip.addr = 0;

  if(NULL == (*client_lease))
  {
    LWIP_DEBUGF(DHCP_DEBUG | LWIP_DBG_TRACE, ("validate_discover: Existing Client Lease not Found\n"));
    if(dhcp_option_given(dhcps, DHCP_OPTION_IDX_REQUESTED_IP))
    {
      client_ip.addr = (u32_t)dhcp_get_option_value(dhcps, DHCP_OPTION_IDX_REQUESTED_IP);
#ifdef  LWIP_DEV_DEBUG
      LWIP_DEBUGF(DHCP_DEBUG | LWIP_DBG_TRACE, ("validate_discover function: Requested IP from client = %"U32_F"\n",client_ip.addr));
#endif

      if((client_ip.addr>=dhcps->start_addr.addr) && (client_ip.addr<=dhcps->end_addr.addr))
      {
        index = (int)(client_ip.addr - dhcps->start_addr.addr);
        if(dhcps->leasearr[index].flags != DHCPS_ADDRESS_FREE)
        {
          /** Requested IP is not available **/
#ifdef  LWIP_DEV_DEBUG
          LWIP_DEBUGF(DHCP_DEBUG | LWIP_DBG_TRACE, ("validate_discover function: Requested IP from client = %"U32_F" Not available \n",client_ip.addr));
#endif
          index = -1;
        }
      }
    }

    if(-1 == index)
    {
      index = find_free_slot(dhcps);
      if(-1 == index)
      {
        LWIP_DEBUGF(DHCP_DEBUG | LWIP_DBG_TRACE, ("validate_discover function: No Free Slot available for Storing addresses\n"));
                    client_ip.addr = 0;
        return client_ip;
      }
      client_ip.addr =  dhcps->start_addr.addr + (u32_t)index;
#ifdef  LWIP_DEV_DEBUG
      LWIP_DEBUGF(DHCP_DEBUG | LWIP_DBG_TRACE, ("validate_discover function: New IP = %"U32_F" is being assigned\n",client_ip.addr));
#endif
    }

    add_client_entry(dhcps, (unsigned int)index, client_msg);
          (*client_lease) = &(dhcps->leasearr[index]);
  }
  else
  {
    client_ip.addr = (*client_lease)->cli_addr.addr;
#ifdef  LWIP_DEV_DEBUG
    LWIP_DEBUGF(DHCP_DEBUG | LWIP_DBG_TRACE, ("validate_discover: Existing Client Lease Found. Existing IP =%"U32_F"\n",client_ip.addr));
#endif

    if((dhcp_option_given(NULL, DHCP_OPTION_IDX_LEASE_TIME))
#if (~0 != LWIP_DHCPS_LEASE_TIME)
    && (dhcp_get_option_value(NULL, DHCP_OPTION_IDX_LEASE_TIME) < LWIP_DHCPS_LEASE_TIME)
#endif
    )
    {
      /** Assign the newly requested time or else use the existing lease time as-is **/
      (*client_lease)->proposed_leasetime = (u32_t)dhcp_get_option_value(NULL, DHCP_OPTION_IDX_LEASE_TIME);
    }
  }

  return client_ip;

}


LWIP_STATIC void handle_discover(struct netif* netif, struct dhcps *dhcps, struct dhcp_msg *client_msg, struct dyn_lease_addr *client_lease)
{
  ip_addr_t client_ip;
  struct pbuf *out_msg;
  struct dhcp_msg *srvr_msg;
  u16_t options_len = 0;
#if ETHARP_SUPPORT_STATIC_ENTRIES
  struct eth_addr ethaddr;
#endif


  LWIP_DEBUGF(DHCP_DEBUG | LWIP_DBG_TRACE, ("handle_discover: Processing Discover Message\n"));

  client_ip = validate_discover(dhcps, client_msg, &client_lease);

  if(0 == client_ip.addr)
       {
          LWIP_DEBUGF(DHCP_DEBUG | LWIP_DBG_TRACE, ("handle_discover: Returning as unable to get a proper address for client\n"));
        return;
       }

  out_msg = dhcps_create_base_msg(client_msg);

  if(NULL == out_msg)
  {
    LWIP_DEBUGF(DHCP_DEBUG | LWIP_DBG_TRACE, ("handle_discover function: Memory allocation for base message failed\n"));
    return;
  }

  srvr_msg = (struct dhcp_msg *)out_msg->payload;

  srvr_msg->yiaddr.addr = htonl(client_ip.addr);

  dhcp_common_option(srvr_msg, DHCP_OPTION_MESSAGE_TYPE, DHCP_OPTION_MESSAGE_TYPE_LEN, &options_len);
  dhcp_common_option_byte(srvr_msg, DHCP_OFFER,&options_len);

  /** netif already holds the Server ID in network order. so, no need to convert it again */
  dhcp_common_option(srvr_msg, DHCP_OPTION_SERVER_ID, DHCP_OPTION_SERVER_ID_LEN, &options_len);
  dhcp_common_option_long(srvr_msg, ntohl(netif->ip_addr.addr), &options_len);

  dhcp_common_option(srvr_msg, DHCP_OPTION_LEASE_TIME, DHCP_OPTION_LEASE_TIME_SIZE, &options_len);
  dhcp_common_option_long(srvr_msg, client_lease->proposed_leasetime, &options_len);

  dhcp_common_option(srvr_msg, DHCP_OPTION_T1, DHCP_OPTION_T1_LEN, &options_len);
  dhcp_common_option_long(srvr_msg, (client_lease->proposed_leasetime/2), &options_len);

  dhcp_common_option(srvr_msg, DHCP_OPTION_T2, DHCP_OPTION_T2_LEN, &options_len);
  dhcp_common_option_long(srvr_msg, ((client_lease->proposed_leasetime*7)/8), &options_len);

  /** No need to convert netmask into network order as it is already stored in network order **/
  dhcp_common_option(srvr_msg, DHCP_OPTION_SUBNET_MASK, DHCP_OPTION_SUBNET_MASK_SIZE, &options_len);
  dhcp_common_option_long(srvr_msg, ntohl(netif->netmask.addr), &options_len);

  dhcp_common_option_trailer(srvr_msg, &options_len);

  LWIP_DEBUGF(DHCP_DEBUG | LWIP_DBG_TRACE, ("handle_discover: realloc()ing\n"));
  pbuf_realloc(out_msg, (u16_t)((sizeof(struct dhcp_msg) - DHCP_OPTIONS_LEN) + options_len));

  if(0 != client_msg->ciaddr.addr)
       {
    LWIP_DEBUGF(DHCP_DEBUG | LWIP_DBG_TRACE, ("handle_discover: sendto(OFFER, ciaddr, DHCP_CLIENT_PORT)\n"));
    udp_sendto_if_src(dhcps->pcb, out_msg, (ip_addr_t *)&(client_msg->ciaddr), DHCP_CLIENT_PORT, netif, &(netif->ip_addr));
       }
#if !LWIP_DHCPS_DISCOVER_BROADCAST
  else if(ntohs(client_msg->flags) & DHCP_BROADCAST_FLAG)
  {
    LWIP_DEBUGF(DHCP_DEBUG | LWIP_DBG_TRACE, ("handle_discover: sendto(OFFER, IP_ADDR_BROADCAST, DHCP_CLIENT_PORT)\n"));
    udp_sendto_if_src(dhcps->pcb, out_msg, IP_ADDR_BROADCAST, DHCP_CLIENT_PORT, netif, &(netif->ip_addr));
  }
  else
  {
    client_ip.addr = htonl(client_ip.addr);

#if ETHARP_SUPPORT_STATIC_ENTRIES
    LWIP_DEBUGF(DHCP_DEBUG | LWIP_DBG_TRACE, ("handle_discover: Updating ARP Static Entry for unicast reply\n"));
    (void)MEMCPY_S(ethaddr.addr, ETHARP_HWADDR_LEN, client_msg->chaddr, client_msg->hlen); //CSEC_FIX_2302
    if(ERR_OK != etharp_add_static_entry(&client_ip, &ethaddr))
    {
      pbuf_free(out_msg);
      LWIP_DEBUGF(DHCP_DEBUG | LWIP_DBG_TRACE, ("handle_discover: Adding static entry to arp cache failed\n"));
      return;
    }
#endif

    /** Need to check and add an arp entry to make this pass through smoothly **/
    udp_sendto_if_src(dhcps->pcb, out_msg, &client_ip, DHCP_CLIENT_PORT, netif, &(netif->ip_addr));

#if ETHARP_SUPPORT_STATIC_ENTRIES
    /** We just added the entry above and checked for it's success too. so, the below function call cannot fail **/
    LWIP_DEBUGF(DHCP_DEBUG | LWIP_DBG_TRACE, ("handle_discover: Removing ARP Static Entry added for unicast reply\n"));
    (void)etharp_remove_static_entry(&client_ip);
#endif
  }
#else
  else
  {
    LWIP_DEBUGF(DHCP_DEBUG | LWIP_DBG_TRACE, ("handle_discover: sendto(OFFER, IP_ADDR_BROADCAST, DHCP_CLIENT_PORT)\n"));
    udp_sendto_if_src(dhcps->pcb, out_msg, IP_ADDR_BROADCAST, DHCP_CLIENT_PORT, netif, &(netif->ip_addr));
  }
#endif

  LWIP_DEBUGF(DHCP_DEBUG | LWIP_DBG_TRACE, ("handle_discover: deleting()ing\n"));
  pbuf_free(out_msg);

  LWIP_DEBUGF(DHCP_DEBUG | LWIP_DBG_TRACE, ("handle_discover: Sending Reply has been successful\n"));
  return;
}


LWIP_STATIC ip_addr_t validate_request_message(struct netif* netif, struct dhcp_msg *client_msg, struct dyn_lease_addr *client_lease, ip_addr_t serverid)
{
  ip_addr_t requested_ip;

      requested_ip.addr = 0;

  LWIP_DEBUGF(DHCP_DEBUG | LWIP_DBG_TRACE, ("validate_request_message: Processing Request Message\n"));

  if((NULL != client_lease) && (DHCPS_ADDRESS_OFFERRED == client_lease->flags))
  {
    /* Now, we are in selecting state */
    LWIP_DEBUGF(DHCP_DEBUG | LWIP_DBG_TRACE, ("validate_request_message: In Selecting State\n"));

    if((0 == serverid.addr) || (0 != client_msg->ciaddr.addr) || !dhcp_option_given(dhcps, DHCP_OPTION_IDX_REQUESTED_IP))
    {
      LWIP_DEBUGF(DHCP_DEBUG | LWIP_DBG_TRACE, ("validate_request_message: Either Server ID is not present or ciaddr is not 0 or requested ip option is not present\n"));
      return requested_ip;
    }

    if(serverid.addr != netif->ip_addr.addr)
    {
      /* This message is not meant for us. The client intends to talk to some other server */
      LWIP_DEBUGF(DHCP_DEBUG | LWIP_DBG_TRACE, ("validate_request_message: Server id doesn't match with ours. Message not for us\n"));
      requested_ip.addr = 1;
      return requested_ip;
    }

    requested_ip.addr = (u32_t)dhcp_get_option_value(dhcps, DHCP_OPTION_IDX_REQUESTED_IP);

  }
  else
  {
    LWIP_DEBUGF(DHCP_DEBUG | LWIP_DBG_TRACE, ("validate_request_message: In Init-Reboot, Renew or Rebinding State\n"));

#if !LWIP_LITEOS_COMPAT
    if(0 != serverid.addr)
    {
      LWIP_DEBUGF(DHCP_DEBUG | LWIP_DBG_TRACE, ("validate_request_message: Server id should NOT present in the Init-Reboot, Renew or Rebinding state, but it it there\n"));
      return requested_ip;
    }
#endif

    /** Now, we can be either in Init-reboot state or renew state or rebinding state **/

    if(dhcp_option_given(dhcps, DHCP_OPTION_IDX_REQUESTED_IP))
    {
      /** Requested IP option is filled in. Indicates we are mostly in Init-Reboot State **/

      if(NULL == client_lease)
      {
        LWIP_DEBUGF(DHCP_DEBUG | LWIP_DBG_TRACE, ("validate_request_message: No Configuration found corresponding to request message\n"));
        requested_ip.addr = 1;
        return requested_ip;
      }

      LWIP_DEBUGF(DHCP_DEBUG | LWIP_DBG_TRACE, ("validate_request_message: Requested IP Option is present. So, considering as Init-Reboot State\n"));

      if(0 != client_msg->ciaddr.addr)
      {
        LWIP_DEBUGF(DHCP_DEBUG | LWIP_DBG_TRACE, ("validate_request_message: Error: ciaddr is filled in the Init-Reboot state. \n"));
        return requested_ip;
      }

      requested_ip.addr = (u32_t)dhcp_get_option_value(dhcps, DHCP_OPTION_IDX_REQUESTED_IP);

    }
    else
    {
      LWIP_DEBUGF(DHCP_DEBUG | LWIP_DBG_TRACE, ("validate_request_message: Requested IP Option is not present. So, considering as Renewing or Rebinding State\n"));

      if(0 == client_msg->ciaddr.addr)
      {
        LWIP_DEBUGF(DHCP_DEBUG | LWIP_DBG_TRACE, ("validate_request_message: Error: ciaddr is not filled in the Renewing or Rebinding state. \n"));
        return requested_ip;
      }

      requested_ip.addr = ntohl(client_msg->ciaddr.addr);
    }
  }

  return requested_ip;

}


LWIP_STATIC void handle_request(struct netif* netif, struct dhcps *dhcps, struct dhcp_msg *client_msg, struct dyn_lease_addr * client_lease, ip_addr_t serverid )
{

  ip_addr_t requested_ip;
  struct pbuf *out_msg;
  struct dhcp_msg *srvr_msg;
  u16_t options_len=0;

#if ETHARP_SUPPORT_STATIC_ENTRIES
  struct eth_addr ethaddr;
#endif

  LWIP_DEBUGF(DHCP_DEBUG | LWIP_DBG_TRACE, ("handle_request: Processing Request Message\n"));

  requested_ip = validate_request_message(netif, client_msg, client_lease, serverid);

  if(1 == requested_ip.addr)
       {
        LWIP_DEBUGF(DHCP_DEBUG | LWIP_DBG_TRACE, ("Validation of request message failed. Returning. \n"));
        return;
       }

  out_msg = dhcps_create_base_msg(client_msg);

  if(NULL == out_msg)
  {
    LWIP_DEBUGF(DHCP_DEBUG | LWIP_DBG_TRACE, ("handle_request: Creating base message failed\n"));
    return;
  }

  srvr_msg = (struct dhcp_msg *)out_msg->payload;


  dhcp_common_option(srvr_msg, DHCP_OPTION_SERVER_ID, DHCP_OPTION_SERVER_ID_LEN, &options_len);
  dhcp_common_option_long(srvr_msg, ntohl(netif->ip_addr.addr), &options_len);


  if((NULL != client_lease) && (client_lease->cli_addr.addr == requested_ip.addr))
  {
    if((u32_t)(~0) != client_lease->proposed_leasetime)
    {
      if(DHCPS_ADDRESS_OFFERRED == client_lease->flags)
      {
        client_lease->leasetime = sys_now() + (client_lease->proposed_leasetime * 1000);
      }
      else
      {
        client_lease->leasetime += (client_lease->proposed_leasetime * 1000);
      }
    }
    else
    {
      client_lease->leasetime = client_lease->proposed_leasetime;
    }


    client_lease->flags = DHCPS_ADDRESS_BOUND;
    srvr_msg->yiaddr.addr = htonl(client_lease->cli_addr.addr);

    dhcp_common_option(srvr_msg, DHCP_OPTION_MESSAGE_TYPE, DHCP_OPTION_MESSAGE_TYPE_LEN, &options_len);
    dhcp_common_option_byte(srvr_msg, DHCP_ACK,&options_len);

    dhcp_common_option(srvr_msg, DHCP_OPTION_LEASE_TIME, DHCP_OPTION_LEASE_TIME_SIZE, &options_len);
    dhcp_common_option_long(srvr_msg, client_lease->proposed_leasetime, &options_len);

    dhcp_common_option(srvr_msg, DHCP_OPTION_T1, DHCP_OPTION_T1_LEN, &options_len);
    dhcp_common_option_long(srvr_msg, (client_lease->proposed_leasetime/2), &options_len);

    dhcp_common_option(srvr_msg, DHCP_OPTION_T2, DHCP_OPTION_T2_LEN, &options_len);
    dhcp_common_option_long(srvr_msg, ((client_lease->proposed_leasetime*7)/8), &options_len);

    dhcp_common_option(srvr_msg, DHCP_OPTION_SUBNET_MASK, DHCP_OPTION_SUBNET_MASK_SIZE, &options_len);
    dhcp_common_option_long(srvr_msg, ntohl(netif->netmask.addr), &options_len);
#ifdef  LWIP_DEV_DEBUG
    LWIP_DEBUGF(DHCP_DEBUG | LWIP_DBG_TRACE, ("handle_request: Send ACK. IP=%"U32_F" lease time=%"U32_F"\n",requested_ip.addr,client_lease->proposed_leasetime));
#endif

  }
  else
  {
    dhcp_common_option(srvr_msg, DHCP_OPTION_MESSAGE_TYPE, DHCP_OPTION_MESSAGE_TYPE_LEN, &options_len);
    dhcp_common_option_byte(srvr_msg, DHCP_NAK,&options_len);

    /** Just set this here, so that the NAK message is brcasted. The correct flags has already been added in the respose message during base message creation */
    client_msg->flags |= htons(DHCP_BROADCAST_FLAG);
    client_msg->ciaddr.addr = 0; /* This is done so that NAK Gets brcasted **/
#ifdef  LWIP_DEV_DEBUG
    LWIP_DEBUGF(DHCP_DEBUG | LWIP_DBG_TRACE, ("handle_request: Send NAK. Requested IP=%"U32_F"\n",requested_ip.addr));
#endif
  }


  requested_ip.addr = htonl(requested_ip.addr);

  dhcp_common_option_trailer(srvr_msg, &options_len);

  LWIP_DEBUGF(DHCP_DEBUG | LWIP_DBG_TRACE, ("handle_request: realloc()ing\n"));
  pbuf_realloc(out_msg, (u16_t)((sizeof(struct dhcp_msg) - DHCP_OPTIONS_LEN) + options_len));

  if(0 != client_msg->ciaddr.addr)
  {
    LWIP_DEBUGF(DHCP_DEBUG | LWIP_DBG_TRACE, ("handle_request: sendto(ACK, ciaddr, DHCP_CLIENT_PORT)\n"));
    udp_sendto_if_src(dhcps->pcb, out_msg, (ip_addr_t *)&(client_msg->ciaddr), DHCP_CLIENT_PORT, netif, &(netif->ip_addr));
  }
  else if(ntohs(client_msg->flags) & DHCP_BROADCAST_FLAG)
  {
    LWIP_DEBUGF(DHCP_DEBUG | LWIP_DBG_TRACE, ("handle_request: sending reply using brdcast \n"));
    udp_sendto_if_src(dhcps->pcb, out_msg, IP_ADDR_BROADCAST, DHCP_CLIENT_PORT, netif, &(netif->ip_addr));
  }
  else
  {

#if ETHARP_SUPPORT_STATIC_ENTRIES
    LWIP_DEBUGF(DHCP_DEBUG | LWIP_DBG_TRACE, ("handle_request: Updating ARP Static Entry for unicast reply\n"));
    (void)MEMCPY_S(ethaddr.addr, ETHARP_HWADDR_LEN, client_msg->chaddr, client_msg->hlen); //CSEC_FIX_2302
    if(ERR_OK != etharp_add_static_entry(&requested_ip, &ethaddr))
    {
      pbuf_free(out_msg);
      LWIP_DEBUGF(DHCP_DEBUG | LWIP_DBG_TRACE, ("handle_request: Adding static entry to arp cache failed\n"));
      return;
    }
#endif
    /** Need to check and add an arp entry to make this pass through smoothly **/
#ifdef  LWIP_DEV_DEBUG
    LWIP_DEBUGF(DHCP_DEBUG | LWIP_DBG_TRACE, ("handle_request: sending reply using unicast Client IP =%"U32_F"\n",requested_ip.addr));
#endif
    udp_sendto_if_src(dhcps->pcb, out_msg, &requested_ip, DHCP_CLIENT_PORT, netif, &(netif->ip_addr));

#if ETHARP_SUPPORT_STATIC_ENTRIES
    /** We just added the entry above and checked for it's success too. so, the below function call cannot fail **/
    LWIP_DEBUGF(DHCP_DEBUG | LWIP_DBG_TRACE, ("handle_request: Removing ARP Static Entry added for unicast reply\n"));
    (void)etharp_remove_static_entry(&requested_ip);
#endif
  }
  LWIP_DEBUGF(DHCP_DEBUG | LWIP_DBG_TRACE, ("handle_request: deleting()ing\n"));

  pbuf_free(out_msg);

  return;
}


LWIP_STATIC void handle_decline(struct dhcp_msg *client_msg, struct dyn_lease_addr * client_lease)
{

  LWIP_DEBUGF(DHCP_DEBUG | LWIP_DBG_TRACE, ("handle_decline: Processing Decline Message\n"));

  if((NULL !=client_lease) && (dhcp_option_given(dhcps, DHCP_OPTION_IDX_REQUESTED_IP)) &&(0 == client_msg->ciaddr.addr))
  {
    if(client_lease->cli_addr.addr == (u32_t)dhcp_get_option_value(dhcps, DHCP_OPTION_IDX_REQUESTED_IP))
    {
#ifdef  LWIP_DEV_DEBUG
      LWIP_DEBUGF(DHCP_DEBUG | LWIP_DBG_TRACE, ("handle_decline: Marking Client Entry as declined. Client IP =%"U32_F"\n", client_lease->cli_addr.addr));
#endif
      (void)MEMSET_S(client_lease->cli_hwaddr, sizeof(client_lease->cli_hwaddr), 0, DHCP_CHADDR_LEN); //CSEC_FIX_2302
      client_lease->proposed_leasetime = 0;
      client_lease->leasetime = sys_now() + (LWIP_DHCPS_DECLINE_TIME * 1000);
      client_lease->flags = DHCPS_ADDRESS_DECLINED;
    }
  }

}


LWIP_STATIC void handle_inform(struct netif* netif, struct dhcps *dhcps, struct dhcp_msg *client_msg)
{
  struct pbuf *out_msg;
  struct dhcp_msg *srvr_msg;
  u16_t options_len=0;

      LWIP_DEBUGF(DHCP_DEBUG | LWIP_DBG_TRACE, ("handle_inform: Processing Inform Message\n"));

  if(0 == client_msg->ciaddr.addr)
  {
    LWIP_DEBUGF(DHCP_DEBUG | LWIP_DBG_TRACE, ("handle_inform: ciaddr is empty. Can't send back a response\n"));
    return;
  }

  out_msg = dhcps_create_base_msg(client_msg);

  if(NULL == out_msg)
  {
    LWIP_DEBUGF(DHCP_DEBUG | LWIP_DBG_TRACE, ("handle_inform: Creating base message failed\n"));
    return;
  }

  srvr_msg = (struct dhcp_msg *)out_msg->payload;

  dhcp_common_option(srvr_msg, DHCP_OPTION_MESSAGE_TYPE, DHCP_OPTION_MESSAGE_TYPE_LEN, &options_len);
  dhcp_common_option_byte(srvr_msg, DHCP_ACK, &options_len);

  dhcp_common_option_trailer(srvr_msg, &options_len);

  LWIP_DEBUGF(DHCP_DEBUG | LWIP_DBG_TRACE, ("handle_inform: realloc()ing\n"));
  pbuf_realloc(out_msg, (u16_t)((sizeof(struct dhcp_msg) - DHCP_OPTIONS_LEN) + options_len));
#ifdef  LWIP_DEV_DEBUG
  LWIP_DEBUGF(DHCP_DEBUG | LWIP_DBG_TRACE, ("handle_inform: Send ACK to Client. Client IP=%"U32_F"\n", client_msg->ciaddr.addr));
#endif
  udp_sendto_if_src(dhcps->pcb, out_msg, (ip_addr_t *)(&(client_msg->ciaddr)), DHCP_CLIENT_PORT, netif, &(netif->ip_addr));

  LWIP_DEBUGF(DHCP_DEBUG | LWIP_DBG_TRACE, ("handle_inform: deleting pbuf\n"));
  pbuf_free(out_msg);

  return;
}

/*lint -e838*/
LWIP_STATIC void handle_client_messages(struct netif *netif, struct dhcps *dhcps, struct dhcp_msg *client_msg, ip_addr_t serverid, u8_t msg_type)
{

  struct dyn_lease_addr *client_lease = NULL;

  client_lease = find_client_lease(dhcps, client_msg);

  switch(msg_type)
  {

    case DHCP_DISCOVER:
              handle_discover(netif, dhcps, client_msg, client_lease);
              break;

    case DHCP_REQUEST:
              handle_request(netif, dhcps, client_msg,client_lease, serverid);
              break;

    case DHCP_DECLINE:
              handle_decline(client_msg, client_lease);
              break;

    case DHCP_RELEASE:
              if((NULL != client_lease) && (client_lease->cli_addr.addr == ntohl(client_msg->ciaddr.addr)))
              {
#ifdef  LWIP_DEV_DEBUG
                LWIP_DEBUGF(DHCP_DEBUG | LWIP_DBG_TRACE, ("DHCP Release: Client IPAdd =%"U32_F"\n",client_msg->ciaddr.addr));
#endif
                (void)MEMSET_S(client_lease, sizeof(struct dyn_lease_addr), 0, sizeof(struct dyn_lease_addr)); //CSEC_FIX_2302
                client_lease->flags = DHCPS_ADDRESS_FREE;
              }
              break;

    case DHCP_INFORM:
              handle_inform(netif, dhcps, client_msg);
              break;
    default:
              LWIP_DEBUGF(DHCP_DEBUG | LWIP_DBG_TRACE | LWIP_DBG_LEVEL_WARNING, ("DHCP Server. Invalid message type received %d\n", msg_type));

  }


}
/*lint +e838*/

LWIP_STATIC void dhcps_recv(void *arg, struct udp_pcb *pcb, struct pbuf *p, ip_addr_t *addr, u16_t port)
{
  struct netif *netif = (struct netif *)arg;
  struct dhcps *dhcps = netif->dhcps;
  struct dhcp_msg *client_msg = (struct dhcp_msg *)p->payload;
  u8_t msg_type;
  ip_addr_t serverid;


  serverid.addr = 0;
#ifdef  LWIP_DEV_DEBUG
  LWIP_DEBUGF(DHCP_DEBUG | LWIP_DBG_TRACE, ("dhcps_recv(pbuf = %p) from DHCP Client %"U16_F".%"U16_F".%"U16_F".%"U16_F" port %"U16_F"\n", (void*)p,
  ip4_addr1_16(addr), ip4_addr2_16(addr), ip4_addr3_16(addr), ip4_addr4_16(addr), port));
#endif
  LWIP_DEBUGF(DHCP_DEBUG | LWIP_DBG_TRACE, ("pbuf->len = %"U16_F"\n", p->len));
  LWIP_DEBUGF(DHCP_DEBUG | LWIP_DBG_TRACE, ("pbuf->tot_len = %"U16_F"\n", p->tot_len));
  /* prevent warnings about unused arguments */
  LWIP_UNUSED_ARG(pcb);
  LWIP_UNUSED_ARG(addr);
  LWIP_UNUSED_ARG(port);

  (void)dhcp_clear_all_options(dhcps);

  /* Check and remove old entries on each call to dhcp_recv. This way, we don't need to maintain timers */
  remove_stale_entries(dhcps);

  if (p->len < DHCP_OPTIONS_OFS)
  {
    LWIP_DEBUGF(DHCP_DEBUG | LWIP_DBG_TRACE | LWIP_DBG_LEVEL_WARNING, ("DHCP client message or pbuf too short. pbuf len =%"U16_F" DHCP MIN Reply Len = %"U32_F"\n", p->len, DHCP_MIN_REPLY_LEN));
    goto free_pbuf_and_return;
  }

  if (client_msg->op != DHCP_BOOTREQUEST)
  {
    LWIP_DEBUGF(DHCP_DEBUG | LWIP_DBG_TRACE | LWIP_DBG_LEVEL_WARNING, ("Not a DHCP reply message, Type %"U16_F"\n", (u16_t)client_msg->op));
    goto free_pbuf_and_return;
  }

  if(client_msg->cookie != PP_HTONL(DHCP_MAGIC_COOKIE))
  {
    LWIP_DEBUGF(DHCP_DEBUG | LWIP_DBG_TRACE | LWIP_DBG_LEVEL_WARNING, ("DHCP Server. Cookie Value is incorrect. %"U32_F"\n", (u32_t)client_msg->cookie));
    goto free_pbuf_and_return;
  }

  if (client_msg->hlen != ETHARP_HWADDR_LEN)
  {
    LWIP_DEBUGF(DHCP_DEBUG | LWIP_DBG_TRACE | LWIP_DBG_LEVEL_WARNING, ("DHCP Server. Invalid hardware address length %"U16_F"\n", (u16_t)client_msg->hlen));
    goto free_pbuf_and_return;
  }

#if LWIP_DHCP_BOOTP_FILE
  if(ERR_OK != dhcp_parse_options(p, NULL))
#else
  if( ERR_OK != dhcp_parse_options(p))
#endif
  {
    LWIP_DEBUGF(DHCP_DEBUG | LWIP_DBG_TRACE | LWIP_DBG_LEVEL_WARNING, ("Parsing of Options failed in DHCP Client Message\n"));
    goto free_pbuf_and_return;
  }

  LWIP_DEBUGF(DHCP_DEBUG | LWIP_DBG_TRACE, ("Searching DHCP_OPTION_MESSAGE_TYPE\n"));
  /* obtain pointer to DHCP message type */
  if (!dhcp_option_given(dhcps, DHCP_OPTION_IDX_MSG_TYPE))
  {
    LWIP_DEBUGF(DHCP_DEBUG | LWIP_DBG_TRACE | LWIP_DBG_LEVEL_WARNING, ("DHCP_OPTION_MESSAGE_TYPE option not found\n"));
    goto free_pbuf_and_return;
  }

  /* read DHCP message type */
  msg_type = (u8_t)dhcp_get_option_value(dhcps, DHCP_OPTION_IDX_MSG_TYPE);

  if(dhcp_option_given(dhcps, DHCP_OPTION_IDX_SERVER_ID))
  {
    LWIP_DEBUGF(DHCP_DEBUG | LWIP_DBG_TRACE | LWIP_DBG_LEVEL_WARNING, ("DHCP_OPTION_SERVER_ID option found\n"));
    /* Parse options would have changed it to host order. But, we have our IP stored in netif in network order */
    serverid.addr = htonl((u32_t)dhcp_get_option_value(dhcps, DHCP_OPTION_IDX_SERVER_ID));
  }

  if((0!=serverid.addr) && ((msg_type == DHCP_DISCOVER) || (msg_type == DHCP_INFORM) ))
  {
    LWIP_DEBUGF(DHCP_DEBUG | LWIP_DBG_TRACE | LWIP_DBG_LEVEL_WARNING, ("Serverid present in DHCP_DISCOVER and DHCP_INFORM messages\n"));
    goto free_pbuf_and_return;
  }

  if((serverid.addr != netif->ip_addr.addr) && ((msg_type == DHCP_DECLINE) || (msg_type == DHCP_RELEASE)))
  {
    LWIP_DEBUGF(DHCP_DEBUG | LWIP_DBG_TRACE | LWIP_DBG_LEVEL_WARNING, ("Serverid not present in DHCP_RELEASE and DHCP_DECLINE messages\n"));
    goto free_pbuf_and_return;
  }

  handle_client_messages(netif, dhcps, client_msg, serverid, msg_type);

  free_pbuf_and_return:
  pbuf_free(p);
}


/*lint -e838*/
err_t dhcps_start(struct netif *netif)
{

  struct dhcps *dhcps = NULL;

  LWIP_ERROR("netif != NULL", (netif != NULL), return ERR_ARG;);
  dhcps = netif->dhcps;

  LWIP_DEBUGF(DHCP_DEBUG | LWIP_DBG_TRACE | LWIP_DBG_STATE, ("dhcps_start(netif=%p) %s%"U16_F"\n", (void*)netif, netif->name, (u16_t)netif->num));

  if (netif->mtu < DHCP_MAX_MSG_LEN_MIN_REQUIRED)
  {
    LWIP_DEBUGF(DHCP_DEBUG | LWIP_DBG_TRACE, ("dhcps_start(): Cannot use this netif with DHCP: MTU is too small. MTU =%"U16_F", DHCP Msg Len Required =%"U32_F"\n", netif->mtu, DHCP_MAX_MSG_LEN_MIN_REQUIRED));
    return ERR_MEM;
  }

  if(dhcps != NULL)
  {
    LWIP_DEBUGF(DHCP_DEBUG | LWIP_DBG_TRACE, ("dhcps_start(): DHCP Server is already started\n"));
    return ERR_MEM;
  }

  LWIP_DEBUGF(DHCP_DEBUG | LWIP_DBG_TRACE, ("dhcps_start(): starting new DHCP Server\n"));
  dhcps = (struct dhcps *)mem_malloc(sizeof(struct dhcps));
  if (dhcps == NULL)
  {
    LWIP_DEBUGF(DHCP_DEBUG | LWIP_DBG_TRACE, ("dhcps_start(): could not allocate dhcp\n"));
    return ERR_MEM;
  }

  LWIP_DEBUGF(DHCP_DEBUG | LWIP_DBG_TRACE, ("dhcps_start(): allocated dhcp"));

  (void)MEMSET_S(dhcps, sizeof(struct dhcps), 0, sizeof(struct dhcps)); //CSEC_FIX_2302

  dhcps->pcb = udp_new();
  if(NULL == dhcps->pcb)
  {
    LWIP_DEBUGF(DHCP_DEBUG  | LWIP_DBG_TRACE, ("dhcp_start(): could not allocate pcb\n"));
    mem_free((void *)dhcps);
    return ERR_MEM;
  }

  dhcps->start_addr.addr = ntohl(netif->ip_addr.addr) + LWIP_DHCPS_START_LEASE_ADDR;
  dhcps->end_addr.addr = dhcps->start_addr.addr + LWIP_DHCPS_MAX_LEASE;


  dhcps->pcb->so_options|=SOF_BROADCAST;
  udp_bind(dhcps->pcb, IP_ADDR_ANY, DHCP_SERVER_PORT);
  udp_connect(dhcps->pcb, IP_ADDR_ANY,DHCP_CLIENT_PORT );

  udp_recv(dhcps->pcb, dhcps_recv, netif);
  LWIP_DEBUGF(DHCP_DEBUG | LWIP_DBG_TRACE, ("dhcps_start(): starting DHCPS Successfully\n"));
#ifdef  LWIP_DEV_DEBUG
  LWIP_DEBUGF(DHCP_DEBUG | LWIP_DBG_TRACE, ("dhcps_start(): DHCPS Conf:: START_LEASE ADDR = %"U32_F" MAX_LEASE = %"U32_F" netif addr = %"U32_F" dhcps start addr%"U32_F" dhcp end addr%"U32_F"\n",LWIP_DHCPS_START_LEASE_ADDR, LWIP_DHCPS_MAX_LEASE,netif->ip_addr.addr,dhcps->start_addr.addr,dhcps->end_addr.addr));
#endif
  LWIP_DEBUGF(DHCP_DEBUG | LWIP_DBG_TRACE, ("dhcps_start(): DHCPS Lease Conf:: Lease Time = %"U32_F" Offer Time = %"U32_F"\n",LWIP_DHCPS_LEASE_TIME, LWIP_DHCPS_OFFER_TIME ));

  netif->dhcps = dhcps;
  return ERR_OK;
}
/*lint +e838*/

void dhcps_stop(struct netif *netif)
{

  LWIP_ERROR("dhcps_stop: netif != NULL", (netif != NULL), return;);

  if(NULL !=netif->dhcps)
  {
    LWIP_DEBUGF(DHCP_DEBUG | LWIP_DBG_TRACE, ("dhcps_stop(): Stopping DHCP Server\n"));
    if (netif->dhcps->pcb != NULL)
    {
      LWIP_DEBUGF(DHCP_DEBUG | LWIP_DBG_TRACE, ("dhcps_stop(): Removing UDP PCB\n"));
      udp_remove(netif->dhcps->pcb);
      netif->dhcps->pcb = NULL;
    }

    mem_free(netif->dhcps);
    netif->dhcps = NULL;
  }
}

#endif

