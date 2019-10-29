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
#include "lwip/sys.h"
#include "lwip/tcpip.h"
#include "netif/ifaddrs.h"
#include <string.h>
#include <stdlib.h>
#include "lwip/mem.h"

extern int tcpip_init_finish;

/* struct to hold the data for one ifaddrs entry, so we can allocate
   everything at once.  */
struct ifaddrs_storage
{
  struct ifaddrs ifa;
  union
  {
    /* Save space for the biggest of the four used sockaddr types and
       avoid a lot of casts.  */
    struct sockaddr sa;
    struct sockaddr_in s4;
  } addr, netmask, dstaddr;
  char name[IFNAMSIZ];
};

struct getifaddrs_arg {
  struct ifaddrs **ifap;
  sys_sem_t       cb_completed;
  int   ret;
};

static void ifaddrs_add_tail(struct ifaddrs **ifap, struct ifaddrs *ifaddr)
{
  struct ifaddrs *temp;

  ifaddr->ifa_next = NULL;
  if(*ifap == NULL)
  {
    *ifap = ifaddr;
    return;
  }

  /* Pclint suppression : Suspicious use of ;*/
  /*lint -e722*/
  for(temp = *ifap; temp->ifa_next != NULL; temp = temp->ifa_next);
  /*lint +e722*/

  temp->ifa_next = ifaddr;
}

/*lint -e429 -e668*/
static void getifaddrs_internal(struct getifaddrs_arg *arg)
{
  struct netif *netif;
  struct ifaddrs *ifaddr;
  struct ifaddrs_storage *if_storage;
  struct sockaddr_in *addr_in;

  int iRet;

  arg->ret = -1;

  for(netif = netif_list; netif != NULL; netif = netif->next) {
    if_storage = (struct ifaddrs_storage *)mem_malloc(sizeof(struct ifaddrs_storage));
    if(if_storage == NULL)
    {
      freeifaddrs(*(arg->ifap)); /* ifap is assigned to NULL in getifaddrs, so garbage value will not be there */
      arg->ret = -1;
      break;
    }

    /* if get one or more netif info, then getifaddrs return 0(OK)*/
    arg->ret = 0;

    (void)MEMSET_S((void *)if_storage, sizeof(struct ifaddrs_storage),
			0, sizeof(*if_storage)); //CSEC_FIX_2302

    ifaddr              = &if_storage->ifa;
    ifaddr->ifa_name    = if_storage->name;
    ifaddr->ifa_addr    = &if_storage->addr.sa;
    ifaddr->ifa_netmask = &if_storage->netmask.sa;
    ifaddr->ifa_dstaddr = &if_storage->dstaddr.sa;

    if(netif->flags & NETIF_FLAG_UP)
      ifaddr->ifa_flags |= IFF_UP;

    if(netif->flags & NETIF_FLAG_ETHARP)
        ifaddr->ifa_flags = ifaddr->ifa_flags & ((unsigned int)(~IFF_NOARP));
    else
      ifaddr->ifa_flags |= IFF_NOARP;

    if(netif->flags & NETIF_FLAG_BROADCAST)
      ifaddr->ifa_flags |= IFF_BROADCAST;
#if PPP_SUPPORT
    else if(netif->flags & NETIF_FLAG_POINTTOPOINT)
      ifaddr->ifa_flags |= IFF_POINTOPOINT;
#endif

    if(netif->flags & NETIF_FLAG_DHCP)
      ifaddr->ifa_flags |= IFF_DYNAMIC;
#if LWIP_IGMP
    if(netif->flags & NETIF_FLAG_IGMP)
      ifaddr->ifa_flags |= IFF_MULTICAST;
#endif

    if(netif->flags & NETIF_FLAG_LINK_UP)
      ifaddr->ifa_flags   |= IFF_DRV_RUNNING;

#if LWIP_HAVE_LOOPIF
    if(netif->link_layer_type == LOOPBACK_IF)
    {
      ifaddr->ifa_flags   |= IFF_LOOPBACK;

      addr_in = (struct sockaddr_in *)ifaddr->ifa_addr;
      addr_in->sin_addr.s_addr = netif->ip_addr.addr;
    }
    else
#endif
    {
      addr_in = (struct sockaddr_in *)ifaddr->ifa_addr;
      addr_in->sin_family      = AF_INET;
      addr_in->sin_addr.s_addr = netif->ip_addr.addr;

      addr_in = (struct sockaddr_in *)ifaddr->ifa_netmask;
      addr_in->sin_family      = AF_INET;
      addr_in->sin_addr.s_addr = netif->netmask.addr;

      addr_in = (struct sockaddr_in *)ifaddr->ifa_broadaddr;
      addr_in->sin_family      = AF_INET;
      addr_in->sin_addr.s_addr = (netif->ip_addr.addr & netif->netmask.addr) |
                                    ~netif->netmask.addr;
    }

    iRet = SNPRINTF_S(ifaddr->ifa_name, IFNAMSIZ, (IFNAMSIZ-1),"%s%"U16_F":%u", netif->name, netif->num, netif->iface_id);

    if ((iRet > 0) && (iRet < IFNAMSIZ))
    {
      ifaddrs_add_tail(arg->ifap, ifaddr);
    }
  }
  sys_sem_signal(&arg->cb_completed);
}
/*lint +e429 +e668*/

int getifaddrs(struct ifaddrs **ifap)
{
  struct getifaddrs_arg arg;

  LWIP_ERROR("getifaddrs:ifap is NULL", (ifap!= NULL) , return ERR_ARG;);
  *ifap = NULL;

  if(!tcpip_init_finish)
    return -1;

  if (sys_sem_new(&arg.cb_completed, 0) != ERR_OK) {
    return -2;
  }
  arg.ret  = 0;
  arg.ifap = ifap;
  (void)tcpip_callback((tcpip_callback_fn)getifaddrs_internal, &arg);
  sys_arch_sem_wait(&arg.cb_completed, 0);
  sys_sem_free(&arg.cb_completed);

  return arg.ret;
}

static void freeifaddrs_iteration(struct ifaddrs *ifa)
{
  if(ifa == NULL)
    return;

  if(ifa->ifa_next != NULL)
  {
    freeifaddrs_iteration(ifa->ifa_next);
  }

  mem_free(ifa);
}

void freeifaddrs(struct ifaddrs *ifa)
{
  freeifaddrs_iteration(ifa);
}

