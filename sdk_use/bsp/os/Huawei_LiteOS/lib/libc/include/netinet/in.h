/****************************************************************************
 * include/netinet/in.h
 *
 *   Copyright (C) 2007, 2009-2010 Gregory Nutt. All rights reserved.
 *   Author: Gregory Nutt <gnutt@nuttx.org>
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 * 3. Neither the name NuttX nor the names of its contributors may be
 *    used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 * COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
 * OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 * AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 * ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 ****************************************************************************/

#ifndef __INCLUDE_NETINET_IN_H
#define __INCLUDE_NETINET_IN_H

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include "sys/types.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

/* Values for protocol argument to socket() */

#define IPPROTO_IP            0    /* Dummy protocol for TCP */
#define IPPROTO_ICMP          1    /* Internet Control Message Protocol */
#define IPPROTO_TCP           6    /* Transmission Control Protocol */
#define IPPROTO_UDP           17   /* User Datagram Protocol */
#define IPPROTO_UDPLITE       136  /* UDP-Lite (RFC 3828) */

/* Internet address.  */
typedef uint32_t in_addr_t;
struct in_addr
{
  in_addr_t s_addr;
};

/****************************************************************************
 * Public Type Definitions
 ****************************************************************************/

 /* Definitions of the bits in an Internet address integer.

   On subnets, host and network parts are found according to
   the subnet mask, not these masks.  */
#define IN_CLASSA_MAX       IP_CLASSA_MAX
#define IN_CLASSA_HOST      IP_CLASSA_HOST
#define IN_CLASSA_NSHIFT    IP_CLASSA_NSHIFT
#define IN_CLASSA_NET       IP_CLASSA_NET
#define IN_CLASSA(a)        IP_CLASSA(a)

#define IN_CLASSB_MAX       IP_CLASSB_MAX
#define IN_CLASSB_HOST      IP_CLASSB_HOST
#define IN_CLASSB_NSHIFT    IP_CLASSB_NSHIFT
#define IN_CLASSB_NET       IP_CLASSB_NET
#define IN_CLASSB(b)        IP_CLASSB(b)

#define IN_CLASSC_MAX       IP_CLASSC_MAX
#define IN_CLASSC_HOST      IP_CLASSC_HOST
#define IN_CLASSC_NSHIFT    IP_CLASSC_NSHIFT
#define IN_CLASSC_NET       IP_CLASSC_NET
#define IN_CLASSC(c)        IP_CLASSC(c)

#define IN_CLASSD_MAX       IP_CLASSD_MAX
#define IN_CLASSD_HOST      IP_CLASSD_HOST    /*   routing needn't know. */
#define IN_CLASSD_NSHIFT    IP_CLASSD_NSHIFT  /*   net and host fields, but */
#define IN_CLASSD_NET       IP_CLASSD_NET     /* These ones aren't really */
#define IN_CLASSD(d)        IP_CLASSD(d)

#define IN_LOOPBACKNET      IP_LOOPBACKNET
#define IN_BADCLASS(a)      IP_BADCLASS(a)
#define IN_EXPERIMENTAL(a)  IP_EXPERIMENTAL(a)
#define IN_MULTICAST(a)     IP_MULTICAST(a)

/* Special values of in_addr_t */

#define INADDR_ANY            ((in_addr_t)0x00000000UL) /* Address to accept any incoming messages */
#define INADDR_BROADCAST      ((in_addr_t)0xffffffffUL) /* Address to send to all hosts */
#define INADDR_NONE           ((in_addr_t)0xffffffffUL) /* Address indicating an error return */
#define INADDR_LOOPBACK       ((in_addr_t)0x7f000001UL) /* Inet 127.0.0.1.  */

/* Defines for Multicast INADDR.  */
#define INADDR_UNSPEC_GROUP ((in_addr_t) 0xe0000000) /* 224.0.0.0 */
#define INADDR_ALLHOSTS_GROUP   ((in_addr_t) 0xe0000001) /* 224.0.0.1 */
#define INADDR_ALLRTRS_GROUP    ((in_addr_t) 0xe0000002) /* 224.0.0.2 */
#define INADDR_MAX_LOCAL_GROUP  ((in_addr_t) 0xe00000ff) /* 224.0.0.255 */

/* Options for use with `getsockopt' and `setsockopt' at the IP level.
   The first word in the comment at the right is the data type used;
   "bool" means a boolean value stored in an `int'.  */
#define        IP_OPTIONS      4       /* ip_opts; IP per-packet options.  */
#define        IP_HDRINCL      3       /* int; Header is included with data.  */
#define        IP_TOS          1       /* int; IP type of service and precedence.  */
#define        IP_TTL          2       /* int; IP time to live.  */
#define        IP_RECVOPTS     6       /* bool; Receive all IP options w/datagram.  */
/* For BSD compatibility.  */
#define        IP_RECVRETOPTS  IP_RETOPTS       /* bool; Receive IP options for response.  */
#define        IP_RETOPTS      7       /* ip_opts; Set/get IP per-packet options.  */
#define IP_MULTICAST_IF 32  /* in_addr; set/get IP multicast i/f */
#define IP_MULTICAST_TTL 33 /* u_char; set/get IP multicast ttl */
#define IP_MULTICAST_LOOP 34    /* i_char; set/get IP multicast loopback */
#define IP_ADD_MEMBERSHIP 35    /* ip_mreq; add an IP group membership */
#define IP_DROP_MEMBERSHIP 36   /* ip_mreq; drop an IP group membership */
#define IP_UNBLOCK_SOURCE 37    /* ip_mreq_source: unblock data from source */
#define IP_BLOCK_SOURCE 38  /* ip_mreq_source: block data from source */
#define IP_ADD_SOURCE_MEMBERSHIP 39 /* ip_mreq_source: join source group */
#define IP_DROP_SOURCE_MEMBERSHIP 40 /* ip_mreq_source: leave source group */
#define IP_MSFILTER 41

/* IPv4 multicast request.  */
struct ip_mreq
{
    /* IP multicast address of group.  */
    struct in_addr imr_multiaddr;

    /* Local IP address of interface.  */
    struct in_addr imr_interface;
};

struct ip_mreq_source
{
    /* IP multicast address of group.  */
    struct in_addr imr_multiaddr;

    /* IP address of source.  */
    struct in_addr imr_interface;

    /* IP address of interface.  */
    struct in_addr imr_sourceaddr;
};

struct sockaddr_in
{
  sa_family_t     sin_family;  /* Address family: AF_INET */
  uint16_t        sin_port;    /* Port in network byte order */
  struct in_addr  sin_addr;    /* Internet address */
  char            sin_zero[8];
};

static inline uint32_t ntohl (uint32_t x)
{
    uint32_t t;
    t = x ^ ((x << 16) | (x >> 16)); /* eor r1,r0,r0,ror #16 */
    x = (x << 24) | (x >> 8);       /* mov r0,r0,ror #8      */
    t &= ~0x00FF0000;           /* bic r1,r1,#0x00FF0000 */
    x ^= (t >> 8);              /* eor r0,r0,r1,lsr #8   */

    return x;
}

static inline uint16_t ntohs (uint16_t x)
{
    return ((x & 0xff) << 8) | ((x & 0xff00) >> 8);
}

static inline uint32_t htonl (uint32_t x)
{
    return ntohl(x);
}

static inline uint16_t htons (uint16_t x)
{
    return ntohs(x);
}

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */

#endif /* __INCLUDE_NETINET_IN_H */


