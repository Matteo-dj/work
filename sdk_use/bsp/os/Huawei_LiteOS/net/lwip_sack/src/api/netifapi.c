/**
 * @file
 * Network Interface Sequential API module
 *
 */

/*
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

#if LWIP_NETIF_API /* don't build if not configured for use in lwipopts.h */

#include "lwip/netifapi.h"
#include "lwip/tcpip.h"

#if LWIP_AUTOIP
#include "lwip/autoip.h"
#endif /* LWIP_AUTOIP */

/**
 * Call netif_add() inside the tcpip_thread context.
 */
void
do_netifapi_netif_add(struct netifapi_msg_msg *msg)
{
  if(msg->msg.add.add_if == lwIP_TRUE) {
    if (!netif_add_secondary( msg->netif,
                    msg->msg.add.main_netif,
                    msg->msg.add.add_if,
                    msg->msg.add.ipaddr,
                    msg->msg.add.netmask,
                    msg->msg.add.gw)) {
      msg->err = ERR_IF;
    } else {
      msg->err = ERR_OK;
    }
  } else {
    if (!netif_add( msg->netif,
                    msg->msg.add.ipaddr,
                    msg->msg.add.netmask,
                    msg->msg.add.gw)) {
      msg->err = ERR_IF;
    } else {
      msg->err = ERR_OK;
    }
  }
  TCPIP_NETIFAPI_ACK(msg);
}

/**
 * Call netif_set_addr() inside the tcpip_thread context.
 */
void
do_netifapi_netif_set_addr(struct netifapi_msg_msg *msg)
{
  netif_set_addr( msg->netif,
                  msg->msg.add.ipaddr,
                  msg->msg.add.netmask,
                  msg->msg.add.gw);
  msg->err = ERR_OK;
  TCPIP_NETIFAPI_ACK(msg);
}

/**
 * Call the "errtfunc" (or the "voidfunc" if "errtfunc" is NULL) inside the
 * tcpip_thread context.
 */
void
do_netifapi_netif_common(struct netifapi_msg_msg *msg)
{
  if (msg->msg.common.errtfunc != NULL) {
    msg->err = msg->msg.common.errtfunc(msg->netif);
  } else {
    msg->err = ERR_OK;
    msg->msg.common.voidfunc(msg->netif);
  }
  TCPIP_NETIFAPI_ACK(msg);
}

#if LWIP_DHCP

/**
 * To call dhcp_set_struct() inside the tcpip_thread context.
 */
void
do_netifapi_dhcp_set_struct(struct netifapi_msg_msg *msg)
{
  dhcp_set_struct(msg->netif, msg->msg.dhcp_struct.dhcp);
  msg->err = ERR_OK;
  TCPIP_NETIFAPI_ACK(msg);
}

/**
 * To call dhcp_remove_struct() from do_netifapi_netif_common()
 */
void
do_netifapi_dhcp_remove_struct(struct netif *netif)
{
  dhcp_remove_struct(netif);
}

#endif

#if LWIP_NETIF_LINK_CALLBACK
void
do_netifapi_netif_set_link_callback(struct netifapi_msg_msg *msg)
{
    netif_set_link_callback(msg->netif, msg->msg.netif_link_cb.link_callback);
    msg->err = ERR_OK;
    TCPIP_NETIFAPI_ACK(msg);
}
#endif

void
do_netifapi_netif_set_mtu (struct netifapi_msg_msg *msg)
{
  err_t ret;
  ret = netif_set_mtu(msg->netif, msg->msg.netif_mtu.mtu);
  msg->err = ret;
  TCPIP_NETIFAPI_ACK(msg);
}

/**
 * Call netif_add() in a thread-safe way by running that function inside the
 * tcpip_thread context.
 *
 * @note for params @see netif_add()
 */
err_t
netifapi_netif_add(struct netif *netif,
                   ip_addr_t *ipaddr,
                   ip_addr_t *netmask,
                   ip_addr_t *gw)
{
  struct netifapi_msg msg;
  msg.function = do_netifapi_netif_add;
  msg.msg.netif = netif;
  msg.msg.msg.add.ipaddr  = ipaddr;
  msg.msg.msg.add.netmask = netmask;
  msg.msg.msg.add.gw      = gw;
  msg.msg.msg.add.add_if  = lwIP_FALSE;
  TCPIP_NETIFAPI(&msg);
  return msg.msg.err;
}

/**
 * Call netif_additional_add() in a thread-safe way by running that function inside the
 * tcpip_thread context.
 *
 * @note for params @see netif_additional_add()
 */

err_t
netifapi_netif_second_if_add(struct netif *main_netif, struct netif *netif,
                   ip_addr_t *ipaddr,
                   ip_addr_t *netmask,
                   ip_addr_t *gw)
{
  struct netifapi_msg msg;
  msg.function = do_netifapi_netif_add;
  msg.msg.netif = netif;
  msg.msg.msg.add.ipaddr  = ipaddr;
  msg.msg.msg.add.netmask = netmask;
  msg.msg.msg.add.gw      = gw;

  msg.msg.msg.add.main_netif = main_netif;
  msg.msg.msg.add.add_if  = lwIP_TRUE;

  TCPIP_NETIFAPI(&msg);
  return msg.msg.err;
}

/**
 * Call netif_set_addr() in a thread-safe way by running that function inside the
 * tcpip_thread context.
 *
 * @note for params @see netif_set_addr()
 */
err_t
netifapi_netif_set_addr(struct netif *netif,
                        ip_addr_t *ipaddr,
                        ip_addr_t *netmask,
                        ip_addr_t *gw)
{
  struct netifapi_msg msg;
  msg.function = do_netifapi_netif_set_addr;
  msg.msg.netif = netif;
  msg.msg.msg.add.ipaddr  = ipaddr;
  msg.msg.msg.add.netmask = netmask;
  msg.msg.msg.add.gw      = gw;
  TCPIP_NETIFAPI(&msg);
  return msg.msg.err;
}

/**
 * call the "errtfunc" (or the "voidfunc" if "errtfunc" is NULL) in a thread-safe
 * way by running that function inside the tcpip_thread context.
 *
 * @note use only for functions where there is only "netif" parameter.
 */
err_t
netifapi_netif_common(struct netif *netif, netifapi_void_fn voidfunc,
                       netifapi_errt_fn errtfunc)
{
  struct netifapi_msg msg;
  LWIP_ERROR("netifapi_netif_common : invalid arguments", (!((voidfunc == NULL) && (errtfunc == NULL))), return ERR_ARG;);

  msg.function = do_netifapi_netif_common;
  msg.msg.netif = netif;
  msg.msg.msg.common.voidfunc = voidfunc;
  msg.msg.msg.common.errtfunc = errtfunc;
  TCPIP_NETIFAPI(&msg);
  return msg.msg.err;
}

/**
 * Remove a network interface from the list of lwIP netifs.
 *
 * @param netif the network interface to remove
 *
 * @return
 *  - ERR_OK: On success
 *  - ERR_MEM: On failure due to memory
 *  - ERR_VAL: On failure due to Illegal value
 */
err_t
netifapi_netif_remove(struct netif *netif)
{
  return netifapi_netif_common(netif, netif_remove, NULL);
}

/*
 * Bring an interface up, available for processing
 * traffic.
 *
 * @note: Enabling DHCP on a down interface will make it come
 * up once configured.
 *
 * @param netif the network interface
 *
 * @return
 *  - ERR_OK: On success
 *  - ERR_MEM: On failure due to memory
 *  - ERR_VAL: On failure due to Illegal value
 */
err_t
netifapi_netif_set_up(struct netif *netif)
{
  return netifapi_netif_common(netif, netif_set_up, NULL);
}

/*
 * Bring an interface down, disabling any traffic processing.
 *
 * @note: Enabling DHCP on a down interface will make it come
 * up once configured.
 *
 * @param netif the network interface
 *
 * @return
 *  - ERR_OK: On success
 *  - ERR_MEM: On failure due to memory
 *  - ERR_VAL: On failure due to Illegal value
 */
err_t
netifapi_netif_set_down(struct netif *netif)
{
  return netifapi_netif_common(netif, netif_set_down, NULL);
}

/*
 * Set a network interface as the default network interface
 * (used to output all packets for which no specific route is found)
 *
 * @param netif the default network interface
 *
 * @return
 *  - ERR_OK: On success
 *  - ERR_MEM: On failure due to memory
 *  - ERR_VAL: On failure due to Illegal value
 */
err_t
netifapi_netif_set_default(struct netif *netif)
{
  return netifapi_netif_common(netif, netif_set_default, NULL);
}

/*
 * Called by a driver when its link goes up
 *
 * @param netif The network interface
 *
 * @return
 *  - ERR_OK: On success
 *  - ERR_MEM: On failure due to memory
 *  - ERR_VAL: On failure due to Illegal value
 */
err_t
netifapi_netif_set_link_up(struct netif *netif)
{
  return netifapi_netif_common(netif, netif_set_link_up, NULL);
}

/*
 * Called by a driver when its link goes down
 *
 * @param netif The network interface
 *
 * @return
 *  - ERR_OK: On success
 *  - ERR_MEM: On failure due to memory
 *  - ERR_VAL: On failure due to Illegal value
 */
err_t
netifapi_netif_set_link_down(struct netif *netif)
{
  return netifapi_netif_common(netif, netif_set_link_down, NULL);
}


#if LWIP_NETIF_LINK_CALLBACK
err_t
netifapi_netif_set_link_callback(struct netif *netif, netif_status_callback_fn link_callback)
{
  struct netifapi_msg msg;

  MEMSET_S(&msg,sizeof(struct netifapi_msg),0,sizeof(struct netifapi_msg));
  msg.function = do_netifapi_netif_set_link_callback;
  msg.msg.netif = netif;
  msg.msg.msg.netif_link_cb.link_callback = link_callback;
  TCPIP_NETIFAPI(&msg);
  return msg.msg.err;
}
#endif

err_t
netifapi_netif_set_mtu(struct netif *netif, u16_t mtu)
{
  struct netifapi_msg msg;
  LWIP_ERROR("netifapi_netif_set_mtu : invalid arguments", (netif != NULL), return ERR_ARG;);

  msg.function = do_netifapi_netif_set_mtu;
  msg.msg.netif = netif;
  msg.msg.msg.netif_mtu.mtu  = mtu;
  TCPIP_NETIFAPI(&msg);
  return msg.msg.err;
}

#if LWIP_DHCP
/*
 * Start DHCP negotiation for a network interface.
 *
 * If no DHCP client instance was attached to this interface,
 * a new client is created first. If a DHCP client instance
 * was already present, it restarts negotiation.
 *
 * @param netif The lwIP network interface
 *
 * @return lwIP error code
 * - ERR_OK - No error
 * - ERR_MEM - Out of memory
 */
err_t
netifapi_dhcp_start(struct netif *netif)
{
  return netifapi_netif_common(netif, NULL, dhcp_start);
}

/*
 * Check DHCP negotiation is done for a network interface.
 *
 * @param netif The lwIP network interface
 *
 * @return
 * - ERR_OK - if DHCP is bound
 * - ERR_MEM - if DHCP bound is still progressing
 */
err_t
netifapi_dhcp_is_bound(struct netif *netif)
{
  return netifapi_netif_common(netif, NULL, dhcp_is_bound);
}

/*
 * Remove the DHCP client from the interface.
 *
 * @param netif The network interface to stop DHCP on
 *
 * @return
 *  - ERR_OK: On success
 *  - ERR_MEM: On failure due to memory
 *  - ERR_VAL: On failure due to Illegal value
 */
err_t
netifapi_dhcp_stop(struct netif *netif)
{
  return netifapi_netif_common(netif, dhcp_stop, NULL);
}

/*
 * Removes a struct dhcp from a netif.
 *
 * ATTENTION: Only use this when not using dhcp_set_struct() to allocate the
 * struct dhcp since the memory is passed back to the heap.
 *
 * @param netif the netif from which to remove the struct dhcp
 *
 * @return
 *  - ERR_OK: On success
 *  - ERR_MEM: On failure due to memory
 *  - ERR_VAL: On failure due to Illegal value
 */
err_t
netifapi_dhcp_cleanup(struct netif *netif)
{
  return netifapi_netif_common(netif, dhcp_cleanup, NULL);
}

/*
 * Inform a DHCP server of our manual configuration.
 *
 * This informs DHCP servers of our fixed IP_add configuration
 * by sending an INFORM message. It does not involve DHCP address
 * configuration, it is just here to be nice to the network.
 *
 * @param netif The lwIP network interface
 *
 * @return
 *  - ERR_OK: On success
 *  - ERR_MEM: On failure due to memory
 *  - ERR_VAL: On failure due to Illegal value
 */
err_t
netifapi_dhcp_inform(struct netif *netif)
{
  return netifapi_netif_common(netif, dhcp_inform, NULL);
}

/*
 * Set a statically allocated struct dhcp to work with.
 * Using this prevents dhcp_start to allocate it using mem_malloc.
 *
 * @param netif the netif for which to set the struct dhcp
 * @param dhcp (uninitialised) dhcp struct allocated by the application
 *
 * @return
 *  - ERR_OK: On success
 *  - ERR_MEM: On failure due to memory
 *  - ERR_VAL: On failure due to Illegal value
 */

err_t
netifapi_dhcp_set_struct(struct netif *netif, struct dhcp *dhcp)
{
  struct netifapi_msg msg;

  MEMSET_S(&msg,sizeof(struct netifapi_msg),0,sizeof(struct netifapi_msg));
  msg.function = do_netifapi_dhcp_set_struct;
  msg.msg.netif = netif;
  msg.msg.msg.dhcp_struct.dhcp = dhcp;
  TCPIP_NETIFAPI(&msg);
  return msg.msg.err;
}

/*
 * To remove the statically assigned dhcp structure
 *
 * @param netif the network interface
 *
 * @return
 *  - ERR_OK: On success
 *  - ERR_MEM: On failure due to memory
 *  - ERR_VAL: On failure due to Illegal value
 */
err_t
netifapi_dhcp_remove_struct(struct netif *netif)
{

  LWIP_ERROR("netifapi_dhcp_remove_struct:netif is NULL", (netif != NULL) , return ERR_ARG;);
  return netifapi_netif_common(netif, do_netifapi_dhcp_remove_struct, NULL);
}

#if LWIP_DHCPS
/*
 * To start DHCP Server
 *
 * @param netif The lwIP network interface
 *
 * @return lwIP error code
 * - ERR_OK - No error
 * - ERR_MEM - Out of memory
 */
err_t
netifapi_dhcps_start(struct netif *netif)
{
  return netifapi_netif_common(netif, NULL, dhcps_start);
}

/*
 * To stop DHCP Server
 *
 * @param netif The lwIP network interface
 *
 * @return lwIP error code
 * - ERR_OK - No error
 * - ERR_MEM - Out of memory
 */
err_t
netifapi_dhcps_stop(struct netif *netif)
{
  return netifapi_netif_common(netif, dhcps_stop, NULL);
}
#endif /* LWIP_DHCPS */

#endif /* LWIP_DHCP */

#if LWIP_AUTOIP
/**
 * Call autoip_start() in a thread-safe way by running that function inside the
 * tcpip_thread context.
 *
 * @note use only for functions where there is only "netif" parameter.
 */
err_t
netifapi_autoip_start(struct netif *netif)
{
  return netifapi_netif_common(netif, NULL, autoip_start);
}

/**
 * Call autoip_stop() in a thread-safe way by running that function inside the
 * tcpip_thread context.
 *
 * @note use only for functions where there is only "netif" parameter.
 */
err_t
netifapi_autoip_stop(struct netif *netif)
{
  return netifapi_netif_common(netif, NULL, autoip_stop);
}
#endif /* LWIP_AUTOIP */

/* NETIF DRIVER STATUS BEGIN */
#if DRIVER_STATUS_CHECK
void
do_netifapi_netif_wake_queue(struct netifapi_msg_msg *msg)
{
  msg->err = netif_wake_queue(msg->netif);

  /* finish error handling here*/
  TCPIP_NETIFAPI_PRIORITY_ACK(msg);
}



err_t
netifapi_wake_queue(struct netif *netif)
{
  LWIP_ERROR("netifapi_wake_queue:netif is NULL", (netif != NULL) , return ERR_ARG;);

  struct netifapi_msg msg;
  msg.function = do_netifapi_netif_wake_queue;
  msg.msg.netif = netif;
  TCPIP_NETIFAPI_PRIORITY(&msg);
  return msg.msg.err;
}

void
do_netifapi_netif_stop_queue(struct netifapi_msg_msg *msg)
{
  msg->err = netif_stop_queue(msg->netif);

  /* finish error handling here*/
  TCPIP_NETIFAPI_PRIORITY_ACK(msg);
}


err_t
netifapi_stop_queue(struct netif *netif)
{
  LWIP_ERROR("netifapi_stop_queue:netif is NULL", (netif != NULL) , return ERR_ARG;);
  struct netifapi_msg msg;
  msg.function = do_netifapi_netif_stop_queue;
  msg.msg.netif = netif;
  TCPIP_NETIFAPI_PRIORITY(&msg);
  return msg.msg.err;
}
#endif
/* NETIF DRIVER STATUS END */
#endif /* LWIP_NETIF_API */
