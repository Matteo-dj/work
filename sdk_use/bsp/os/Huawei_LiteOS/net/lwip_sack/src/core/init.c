/**
 * @file
 * Modules initialization
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

#include "lwip/init.h"
#include "lwip/stats.h"
#include "lwip/sys.h"
#include "lwip/mem.h"
#include "lwip/memp.h"
#include "lwip/pbuf.h"
#include "lwip/netif.h"
#include "lwip/sockets.h"
#include "lwip/ip.h"
#include "lwip/raw.h"
#include "lwip/udp.h"
#include "lwip/tcp_impl.h"
#if LWIP_SNMP
#include "lwip/snmp_msg.h"
#endif
#include "lwip/autoip.h"
#include "lwip/igmp.h"
#include "lwip/dns.h"
#include "lwip/timers.h"
#include "netif/etharp.h"
#include "lwip/api.h"
#include "lwip/tcp_config.h"

/* Compile-time sanity checks for configuration errors.
 * These can be done independently of LWIP_DEBUG, without penalty.
 */
#ifndef BYTE_ORDER
  #error "BYTE_ORDER is not defined, you have to define it in your cc.h"
#endif
#if (LWIP_NETIF_INDEX_MAX > 10)
  #error "The maximum netif index value is 10!"
#endif

#if (LWIP_NETIF_IFINDEX_MAX > 0xFE)
  #error "The maximum netif ifindex value is 0xFE!"
#endif

#if (LWIP_NETIF_IFINDEX_START < 1)
  #error "The minimum netif ifindex value is 1!"
#endif

#if (LWIP_NETIF_IFINDEX_START >=  LWIP_NETIF_IFINDEX_MAX)
  #error "IfIndex Start should not be greater than or equal to IfIndex Max!"
#endif

#if (!IP_SOF_BROADCAST && IP_SOF_BROADCAST_RECV)
  #error "If you want to use brdcast filter per pcb on recv operations, you have to define IP_SOF_BROADCAST=1 in your lwipopts.h"
#endif
#if (!LWIP_UDP && LWIP_UDPLITE)
  #error "If you want to use UDP Lite, you have to define LWIP_UDP=1 in your lwipopts.h"
#endif
#if (!LWIP_UDP && LWIP_SNMP)
  #error "If you want to use SNMP, you have to define LWIP_UDP=1 in your lwipopts.h"
#endif
#if (!LWIP_UDP && LWIP_DHCP)
  #error "If you want to use DHCP, you have to define LWIP_UDP=1 in your lwipopts.h"
#endif
#if (!LWIP_UDP && LWIP_IGMP)
  #error "If you want to use IGMP, you have to define LWIP_UDP=1 in your lwipopts.h"
#endif
#if (!LWIP_UDP && LWIP_SNMP)
  #error "If you want to use SNMP, you have to define LWIP_UDP=1 in your lwipopts.h"
#endif
#if (!LWIP_UDP && LWIP_DNS)
  #error "If you want to use DNS, you have to define LWIP_UDP=1 in your lwipopts.h"
#endif
#if (!LWIP_UDP && LWIP_SNTP)
  #error "If you want to use SNTP, you have to define LWIP_UDP=1 in your lwipopts.h"
#endif

#if !MEMP_MEM_MALLOC /* MEMP_NUM_* checks are disabled when not using the pool allocator */
#if (LWIP_ARP && ARP_QUEUEING && (MEMP_NUM_ARP_QUEUE<=0))
  #error "If you want to use ARP Queueing, you have to define MEMP_NUM_ARP_QUEUE>=1 in your lwipopts.h"
#endif
#if (LWIP_RAW && (MEMP_NUM_RAW_PCB<=0))
  #error "If you want to use RAW, you have to define MEMP_NUM_RAW_PCB>=1 in your lwipopts.h"
#endif
#if (LWIP_UDP && (MEMP_NUM_UDP_PCB<=0))
  #error "If you want to use UDP, you have to define MEMP_NUM_UDP_PCB>=1 in your lwipopts.h"
#endif
#if (LWIP_TCP && (MEMP_NUM_TCP_PCB<=0))
  #error "If you want to use TCP, you have to define MEMP_NUM_TCP_PCB>=1 in your lwipopts.h"
#endif
#if (LWIP_IGMP && (MEMP_NUM_IGMP_GROUP<=1))
  #error "If you want to use IGMP, you have to define MEMP_NUM_IGMP_GROUP>1 in your lwipopts.h"
#endif
#if ((LWIP_NETCONN || LWIP_SOCKET) && (MEMP_NUM_TCPIP_MSG_API<=0))
  #error "If you want to use Sequential API, you have to define MEMP_NUM_TCPIP_MSG_API>=1 in your lwipopts.h"
#endif
/* There must be sufficient timeouts, taking into account requirements of the subsystems. */
#if LWIP_TIMERS && (MEMP_NUM_SYS_TIMEOUT < (LWIP_TCP + IP_REASSEMBLY + LWIP_ARP + (2*LWIP_DHCP) + LWIP_AUTOIP + LWIP_IGMP + LWIP_DNS + PPP_SUPPORT))
  #error "MEMP_NUM_SYS_TIMEOUT is too low to accomodate all required timeouts"
#endif
#endif /* !MEMP_MEM_MALLOC */
/* Adding for WINDOW SCALE */
#if LWIP_WND_SCALE
#if (LWIP_TCP && (TCP_WND > 0xffffffff))
  #error "If you want to use TCP, TCP_WND must fit in an u32_t, so, you have to reduce it in your lwipopts.h"
#endif
#if (LWIP_TCP && TCP_RCV_SCALE > 14)
  #error "The maximum valid window scale value is 14!"
#endif
#if (LWIP_TCP && (TCP_WND > (0xFFFFU << TCP_RCV_SCALE)))
  #error "TCP_WND is bigger than the configured LWIP_WND_SCALE allows!"
#endif
#if (LWIP_TCP && ((TCP_WND >> TCP_RCV_SCALE) == 0))
  #error "TCP_WND is too small for the configured LWIP_WND_SCALE (results in zero window)!"
#endif
#if (LWIP_TCP && (TCP_WND_MIN > 0xffff))
  #error "If you want to use TCP, TCP_WND_MIN must fit in an u16_t, so, you have to reduce it in your lwipopts.h"
#endif
#else /* LWIP_WND_SCALE */
#if (LWIP_TCP && (TCP_WND > 0xffff))
  #error "If you want to use TCP, TCP_WND must fit in an u16_t, so, you have to reduce it in your lwipopts.h"
#endif
#endif /* LWIP_WND_SCALE */

#if (LWIP_TCP && (INITIAL_CWND < 4 || INITIAL_CWND > 200))
  #error "If you want to use TCP, LWIP_CONGCNTRL_INITIAL_CWND must greater than 4 and less than 200, you can modify in lwipopts.h"
#endif

#if (LWIP_TCP && (INITIAL_SSTHRESH < 20 || INITIAL_SSTHRESH > 2000))
  #error "If you want to use TCP, LWIP_CONGCNTRL_INITIAL_SSTHRESH must greater than 4 and less than 200, you can modify in lwipopts.h"
#endif


#if (LWIP_TCP && (TCP_SND_QUEUELEN > 0xffff))
  #error "If you want to use TCP, TCP_SND_QUEUELEN must fit in an u16_t, so, you have to reduce it in your lwipopts.h"
#endif
#if (LWIP_TCP && (TCP_SND_QUEUELEN < 2))
  #error "TCP_SND_QUEUELEN must be at least 2 for no-copy TCP writes to work"
#endif
#if (LWIP_TCP && ((TCP_MAXRTX < TCP_MAXRTX_MIN_VALUE || TCP_MAXRTX > 255)))
  #error "If you want to use TCP, TCP_MAXRTX must be greater than 3 and less or equal to 255, so, you have to reduce them in your lwipopts.h"
#endif
#if (LWIP_TCP && (TCP_SYNMAXRTX < TCP_MAXRTX_MIN_VALUE || TCP_SYNMAXRTX > 12))
  #error "If you want to use TCP, TCP_SYNMAXRTX must be greater than 3 and less or equal to 12 (due to tcp_backoff table), so, you have to reduce them in your lwipopts.h"
#endif
#if (LWIP_TCP && ((TCP_FW1MAXRTX < TCP_MAXRTX_MIN_VALUE || TCP_FW1MAXRTX > 255)))
  #error "If you want to use TCP, TCP_FW1MAXRTX must be greater than 3 and less or equal to 255, so, you have to reduce them in your lwipopts.h"
#endif
#if (LWIP_TCP && (TCP_TTL < TCP_TTL_MIN_VALUE || TCP_TTL > 255))
  #error "If you want to use TCP, TCP_TTL must be greater than 8 and less or equal to 255, so, you have to reduce them in your lwipopts.h"
#endif
#if (LWIP_TCP && TCP_LISTEN_BACKLOG && (TCP_DEFAULT_LISTEN_BACKLOG < 0) || (TCP_DEFAULT_LISTEN_BACKLOG > 0xff))
  #error "If you want to use TCP backlog, TCP_DEFAULT_LISTEN_BACKLOG must fit into an u8_t"
#endif
#if (LWIP_NETIF_API && (NO_SYS==1))
  #error "If you want to use NETIF API, you have to define NO_SYS=0 in your lwipopts.h"
#endif
#if ((LWIP_SOCKET || LWIP_NETCONN) && (NO_SYS==1))
  #error "If you want to use Sequential API, you have to define NO_SYS=0 in your lwipopts.h"
#endif
#if (!LWIP_NETCONN && LWIP_SOCKET)
  #error "If you want to use Socket API, you have to define LWIP_NETCONN=1 in your lwipopts.h"
#endif
#if (((!LWIP_DHCP) || (!LWIP_AUTOIP)) && LWIP_DHCP_AUTOIP_COOP)
  #error "If you want to use DHCP/AUTOIP cooperation mode, you have to define LWIP_DHCP=1 and LWIP_AUTOIP=1 in your lwipopts.h"
#endif
#if (((!LWIP_DHCP) || (!LWIP_ARP)) && DHCP_DOES_ARP_CHECK)
  #error "If you want to use DHCP ARP checking, you have to define LWIP_DHCP=1 and LWIP_ARP=1 in your lwipopts.h"
#endif
#if (!LWIP_ARP && LWIP_AUTOIP)
  #error "If you want to use AUTOIP, you have to define LWIP_ARP=1 in your lwipopts.h"
#endif
#if (LWIP_SNMP && (SNMP_CONCURRENT_REQUESTS<=0))
  #error "If you want to use SNMP, you have to define SNMP_CONCURRENT_REQUESTS>=1 in your lwipopts.h"
#endif
#if (LWIP_SNMP && (SNMP_TRAP_DESTINATIONS<=0))
  #error "If you want to use SNMP, you have to define SNMP_TRAP_DESTINATIONS>=1 in your lwipopts.h"
#endif
#if (LWIP_TCP && ((LWIP_EVENT_API && LWIP_CALLBACK_API) || (!LWIP_EVENT_API && !LWIP_CALLBACK_API)))
  #error "One and exactly one of LWIP_EVENT_API and LWIP_CALLBACK_API has to be enabled in your lwipopts.h"
#endif
#if (MEM_LIBC_MALLOC && MEM_USE_POOLS)
  #error "MEM_LIBC_MALLOC and MEM_USE_POOLS may not both be simultaneously enabled in your lwipopts.h"
#endif
#if (MEM_USE_POOLS && !MEMP_USE_CUSTOM_POOLS)
  #error "MEM_USE_POOLS requires custom pools (MEMP_USE_CUSTOM_POOLS) to be enabled in your lwipopts.h"
#endif
#if (PBUF_POOL_BUFSIZE <= MEM_ALIGNMENT)
  #error "PBUF_POOL_BUFSIZE must be greater than MEM_ALIGNMENT or the offset may take the full first pbuf"
#endif
#if (DNS_LOCAL_HOSTLIST && !DNS_LOCAL_HOSTLIST_IS_DYNAMIC && !(defined(DNS_LOCAL_HOSTLIST_INIT)))
  #error "you have to define define DNS_LOCAL_HOSTLIST_INIT {{'host1', 0x123}, {'host2', 0x234}} to initialize DNS_LOCAL_HOSTLIST"
#endif
/* PC Lint FIx for: Unusual use of a Boolean expression */
/* Wrong use of boolean expression corrected */
#if PPP_SUPPORT && !PPPOS_SUPPORT && !PPPOE_SUPPORT
  #error "PPP_SUPPORT needs either PPPOS_SUPPORT or PPPOE_SUPPORT turned on"
#endif
#if !LWIP_ETHERNET && (LWIP_ARP || PPPOE_SUPPORT)
  #error "LWIP_ETHERNET needs to be turned on for LWIP_ARP or PPPOE_SUPPORT"
#endif
#if LWIP_IGMP && !defined(LWIP_RAND)
  #error "When using IGMP, LWIP_RAND() needs to be defined to a random-function returning an u32_t random value"
#endif
#if LWIP_TCPIP_CORE_LOCKING_INPUT && !LWIP_TCPIP_CORE_LOCKING
  #error "When using LWIP_TCPIP_CORE_LOCKING_INPUT, LWIP_TCPIP_CORE_LOCKING must be enabled, too"
#endif
#if LWIP_TCP && LWIP_NETIF_TX_SINGLE_PBUF && !TCP_OVERSIZE
  #error "LWIP_NETIF_TX_SINGLE_PBUF needs TCP_OVERSIZE enabled to create single-pbuf TCP packets"
#endif
#if IP_FRAG && IP_FRAG_USES_STATIC_BUF && LWIP_NETIF_TX_SINGLE_PBUF
  #error "LWIP_NETIF_TX_SINGLE_PBUF does not work with IP_FRAG_USES_STATIC_BUF==1 as that creates pbuf queues"
#endif
#if LWIP_NETCONN && LWIP_TCP
#if NETCONN_COPY != TCP_WRITE_FLAG_COPY
  #error "NETCONN_COPY != TCP_WRITE_FLAG_COPY"
#endif
#if NETCONN_MORE != TCP_WRITE_FLAG_MORE
  #error "NETCONN_MORE != TCP_WRITE_FLAG_MORE"
#endif
#endif /* LWIP_NETCONN && LWIP_TCP */

#if LWIP_ENABLE_LOS_SHELL_CMD && !LWIP_NETIF_API
  #error "LWIP_NETIF_API needs to be turned on for LWIP_ENABLE_LOS_SHELL_CMD"
#endif
#if LWIP_DHCPS && !ETHARP_SUPPORT_STATIC_ENTRIES
  #error "ETHARP_SUPPORT_STATIC_ENTRIES needs to be turned on for LWIP_DHCPS"
#endif
#if LWIP_ENABLE_LOS_SHELL_CMD && !LWIP_RAW
  #error "LWIP_RAW needs to be turned on for LWIP_ENABLE_LOS_SHELL_CMD, as ping command uses raw socket"
#endif
#if LWIP_TFTP && !LWIP_UDP
  #error "LWIP_UDP needs to be turned on for LWIP_TFTP, as tftp runs over UDP socket"
#endif
#if (DNS_TABLE_SIZE <= 0)
  #error "DNS_TABLE_SIZE should be configured as greater than zero"
#endif
#if (DNS_MAX_IPADDR <= 0)
  #error "DNS_MAX_IPADDR should be configured as greater than zero"
#endif
#if (DNS_MAX_SERVERS <= 0)
  #error "DNS_MAX_SERVERS should be configured as greater than zero"
#endif
#if (DNS_MAX_LABLE_LENGTH != 63)
  #error "DNS_MAX_LABLE_LENGTH should be 63 as per RFC"
#endif
#if (DNS_MAX_NAME_LENGTH != 256)
  #error "DNS_MAX_NAME_LENGTH should be 256 as per RFC"
#endif

/* Compile-time checks for deprecated options.
 */
#ifdef MEMP_NUM_TCPIP_MSG
  #error "MEMP_NUM_TCPIP_MSG option is deprecated. Remove it from your lwipopts.h."
#endif
#ifdef MEMP_NUM_API_MSG
  #error "MEMP_NUM_API_MSG option is deprecated. Remove it from your lwipopts.h."
#endif
#ifdef TCP_REXMIT_DEBUG
  #error "TCP_REXMIT_DEBUG option is deprecated. Remove it from your lwipopts.h."
#endif
#ifdef RAW_STATS
  #error "RAW_STATS option is deprecated. Remove it from your lwipopts.h."
#endif
#ifdef ETHARP_QUEUE_FIRST
  #error "ETHARP_QUEUE_FIRST option is deprecated. Remove it from your lwipopts.h."
#endif
#ifdef ETHARP_ALWAYS_INSERT
  #error "ETHARP_ALWAYS_INSERT option is deprecated. Remove it from your lwipopts.h."
#endif

#ifndef LWIP_DISABLE_TCP_SANITY_CHECKS
#define LWIP_DISABLE_TCP_SANITY_CHECKS  0
#endif
#ifndef LWIP_DISABLE_MEMP_SANITY_CHECKS
#define LWIP_DISABLE_MEMP_SANITY_CHECKS 0
#endif

/* MEMP sanity checks */
#if !LWIP_DISABLE_MEMP_SANITY_CHECKS
#if LWIP_NETCONN
#if MEMP_MEM_MALLOC
#if !MEMP_NUM_NETCONN && LWIP_SOCKET
#error "lwip_sanity_check: WARNING: MEMP_NUM_NETCONN cannot be 0 when using sockets!"
#endif
#else /* MEMP_MEM_MALLOC */
#if MEMP_NUM_NETCONN > (MEMP_NUM_TCP_PCB+MEMP_NUM_TCP_PCB_LISTEN+MEMP_NUM_UDP_PCB+MEMP_NUM_RAW_PCB+DEFAULT_ACCEPTMBOX_SIZE)
#error "lwip_sanity_check: WARNING: MEMP_NUM_NETCONN should be less than the sum of MEMP_NUM_{TCP,RAW,UDP}_PCB+MEMP_NUM_TCP_PCB_LISTEN. If you know what you are doing, define LWIP_DISABLE_MEMP_SANITY_CHECKS to 1 to disable this error."
#endif
#endif /* MEMP_MEM_MALLOC */
#endif /* LWIP_NETCONN */
#endif /* !LWIP_DISABLE_MEMP_SANITY_CHECKS */

/* TCP sanity checks */
#if !LWIP_DISABLE_TCP_SANITY_CHECKS
#if LWIP_TCP
#if !MEMP_MEM_MALLOC && (MEMP_NUM_TCP_SEG < TCP_SND_QUEUELEN)
  #error "lwip_sanity_check: WARNING: MEMP_NUM_TCP_SEG should be at least as big as TCP_SND_QUEUELEN. If you know what you are doing, define LWIP_DISABLE_TCP_SANITY_CHECKS to 1 to disable this error."
#endif
#if TCP_SND_BUF < (2 * TCP_MSS)
  #error "lwip_sanity_check: WARNING: TCP_SND_BUF must be at least as much as (2 * TCP_MSS) for things to work smoothly. If you know what you are doing, define LWIP_DISABLE_TCP_SANITY_CHECKS to 1 to disable this error."
#endif
#if TCP_SND_QUEUELEN < (2 * (TCP_SND_BUF / TCP_MSS))
  #error "lwip_sanity_check: WARNING: TCP_SND_QUEUELEN must be at least as much as (2 * TCP_SND_BUF/TCP_MSS) for things to work. If you know what you are doing, define LWIP_DISABLE_TCP_SANITY_CHECKS to 1 to disable this error."
#endif
#if TCP_SNDLOWAT >= TCP_SND_BUF
  #error "lwip_sanity_check: WARNING: TCP_SNDLOWAT must be less than TCP_SND_BUF. If you know what you are doing, define LWIP_DISABLE_TCP_SANITY_CHECKS to 1 to disable this error."
#endif
#if TCP_SNDQUEUELOWAT >= TCP_SND_QUEUELEN
  #error "lwip_sanity_check: WARNING: TCP_SNDQUEUELOWAT must be less than TCP_SND_QUEUELEN. If you know what you are doing, define LWIP_DISABLE_TCP_SANITY_CHECKS to 1 to disable this error."
#endif
#if !MEMP_MEM_MALLOC && (TCP_WND > (PBUF_POOL_SIZE * (PBUF_POOL_BUFSIZE - (PBUF_LINK_HLEN + PBUF_IP_HLEN + PBUF_TRANSPORT_HLEN))))
  #error "lwip_sanity_check: WARNING: TCP_WND is larger than space provided by PBUF_POOL_SIZE * (PBUF_POOL_BUFSIZE - protocol headers). If you know what you are doing, define LWIP_DISABLE_TCP_SANITY_CHECKS to 1 to disable this error."
#endif
#if TCP_WND < TCP_MSS
  #error "lwip_sanity_check: WARNING: TCP_WND is smaller than MSS. If you know what you are doing, define LWIP_DISABLE_TCP_SANITY_CHECKS to 1 to disable this error."
#endif
#endif /* LWIP_TCP */
#endif /* !LWIP_DISABLE_TCP_SANITY_CHECKS */


int     g_IslwIPInitialized = lwIP_FALSE;

int     g_IslwIPSecSspRegistered = lwIP_FALSE;
STlwIPSecFuncSsp g_stlwIPSecSspCallbacks;

int lwIPRegSecSspCbk(const PTlwIPSecFuncSsp pstSecureFuncSsp)
{
    if (NULL == pstSecureFuncSsp)
    {
        return ERR_VAL;
    }

    if ((NULL == pstSecureFuncSsp->pfMemcpy_s)
        || (NULL == pstSecureFuncSsp->pfMemMove_s)
        || (NULL == pstSecureFuncSsp->pfMemset_s)
        || (NULL == pstSecureFuncSsp->pfStrNCat_s)
        || (NULL == pstSecureFuncSsp->pfStrCat_s)
        || (NULL == pstSecureFuncSsp->pfStrNCpy_s)
        || (NULL == pstSecureFuncSsp->pfSnprintf_s)
        || (NULL == pstSecureFuncSsp->pfRand))
        {
             return ERR_VAL;
        }

    g_stlwIPSecSspCallbacks.pfMemcpy_s = pstSecureFuncSsp->pfMemcpy_s;
    g_stlwIPSecSspCallbacks.pfMemset_s =  pstSecureFuncSsp->pfMemset_s;
    g_stlwIPSecSspCallbacks.pfMemMove_s = pstSecureFuncSsp->pfMemMove_s;
    g_stlwIPSecSspCallbacks.pfStrNCat_s = pstSecureFuncSsp->pfStrNCat_s;
    g_stlwIPSecSspCallbacks.pfStrCat_s   = pstSecureFuncSsp->pfStrCat_s;
    g_stlwIPSecSspCallbacks.pfStrNCpy_s = pstSecureFuncSsp->pfStrNCpy_s;
    g_stlwIPSecSspCallbacks.pfSnprintf_s = pstSecureFuncSsp->pfSnprintf_s;
    g_stlwIPSecSspCallbacks.pfRand        = pstSecureFuncSsp->pfRand;
    g_IslwIPSecSspRegistered = lwIP_TRUE;
    return ERR_OK;
}
/* SNTP sanity checks */
#if ((SNTP_MAX_REQUEST_RETRANSMIT > 255) || (SNTP_MAX_REQUEST_RETRANSMIT < 0))
 #error "SNTP_MAX_REQUEST_RETRANSMIT should be between 0 to 255"
#endif

/**
 * Perform Sanity check of user-configurable values, and initialize all modules.
 */
int
lwip_init(void)
{

   if (lwIP_FALSE == g_IslwIPSecSspRegistered)
   {
        return ERR_VAL;
   }

  /* Modules initialization */
  stats_init();
#if !NO_SYS
  sys_init();
#endif /* !NO_SYS */
  mem_init();
  memp_init();
  pbuf_init();
  netif_init();
  /*lint -e746*/
  if(ERR_OK != sock_init())
  /*lint +e746*/  
  {
    return ERR_MEM;
  }
  ip_init();
#if LWIP_ARP
  etharp_init();
#endif /* LWIP_ARP */
#if LWIP_RAW
  raw_init();
#endif /* LWIP_RAW */
#if LWIP_UDP
  udp_init();
#endif /* LWIP_UDP */
#if LWIP_TCP
 /* all tcp configuration has to be configure before tcp_init() */
 tcp_init();
#endif /* LWIP_TCP */
#if LWIP_SNMP
  snmp_init();
#endif /* LWIP_SNMP */
#if LWIP_AUTOIP
  autoip_init();
#endif /* LWIP_AUTOIP */
#if LWIP_IGMP
  igmp_init();
#endif /* LWIP_IGMP */
#if LWIP_DNS
  dns_init();
#endif /* LWIP_DNS */

#if LWIP_TIMERS
  sys_timeouts_init();
#endif /* LWIP_TIMERS */

  g_IslwIPInitialized  = lwIP_TRUE;

  return 0;
}
