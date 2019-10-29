/*  $FreeBSD$ */

/*
 * Copyright (c) 1995, 1999
 *  Berkeley Software Design, Inc.  All rights reserved.
 *
 * Copyright (c) <2013-2015>, <Huawei Technologies Co., Ltd>
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * THIS SOFTWARE IS PROVIDED BY Berkeley Software Design, Inc. ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL Berkeley Software Design, Inc. BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 *
 *  BSDI ifaddrs.h,v 2.5 2000/02/23 14:51:59 dab Exp
 */

/*
 *********************************************************************************
 * Notice of Export Control Law
 * ===============================================
 * Huawei LiteOS may be subject to applicable export control laws and regulations, which
 * might include those applicable to Huawei LiteOS of U.S. and the country in which you
 * are located.
 * Import, export and usage of Huawei LiteOS in any manner by you shall be in compliance
 * with such applicable export control laws and regulations.
 *********************************************************************************
 */


 /**
 *  @file ifaddrs.h
 */

#ifndef __LWIP_IFADDRS_H
#define __LWIP_IFADDRS_H

#include "lwip/netif.h"
#include "lwip/sockets.h"
#include "lwip/netif.h"
#include "arch/cc.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

#if !LWIP_LITEOS_COMPAT
#define SIOCADDRT   _IOW('R', 9, struct rtentry)

#define SIOCGIFFLAGS  _IOWR('i', 17, struct ifreq)  /* get ifnet flags */
#define SIOCSIFFLAGS   _IOW('i', 16, struct ifreq)  /* set ifnet flags */
#define SIOCGIFADDR _IOWR('i', 33, struct ifreq)  /* get ifnet address */
#define SIOCSIFADDR  _IOW('i', 12, struct ifreq)  /* set ifnet address */
#define SIOCGIFNETMASK  _IOWR('i', 37, struct ifreq)  /* get net addr mask */
#define SIOCSIFNETMASK   _IOW('i', 22, struct ifreq)  /* set net addr mask */
#define SIOCSIFHWADDR  _IOW('i', 140, struct ifreq) /* set IF name */
#define SIOCGIFHWADDR  _IOW('i', 141, struct ifreq) /* set IF name */
#define SIOCGIFNAME  _IOW('i', 142, struct ifreq) /* set IF name */
#define SIOCSIFNAME  _IOW('i', 40, struct ifreq)  /* set IF name */
#if PF_PKT_SUPPORT
#define  SIOCGIFINDEX  _IOWR('i', 32, struct ifreq) /* get IF index */
#endif /* PF_PKT_SUPPORT */

/*-
 * Interface flags are of two types: network stack owned flags, and driver
 * owned flags.  Historically, these values were stored in the same ifnet
 * flags field, but with the advent of fine-grained locking, they have been
 * broken out such that the network stack is responsible for synchronizing
 * the stack-owned fields, and the device driver the device-owned fields.
 * Both halves can perform lockless reads of the other half's field, subject
 * to accepting the involved races.
 *
 * Both sets of flags come from the same number space, and should not be
 * permitted to conflict, as they are exposed to user space via a single
 * field.
 *
 * The following symbols identify read and write requirements for fields:
 *
 * (i) if_flags field set by device driver before attach, read-only there
 *     after.
 * (n) if_flags field written only by the network stack, read by either the
 *     stack or driver.
 * (d) if_drv_flags field written only by the device driver, read by either
 *     the stack or driver.
 */
 /** (n) Indicates that the interface is up */
#define IFF_UP    0x1
/** (i) Indicates that the broadcast address valid */
#define IFF_BROADCAST 0x2
/** (n) Turns on debugging */
#define IFF_DEBUG 0x4
/** (i) A loopback net */
#define IFF_LOOPBACK  0x8
/** (i) is a point-to-point link */
#define IFF_POINTOPOINT 0x10
/*      0x20       was IFF_SMART */
/** (d) Indicates that the resources are allocated */
#define IFF_DRV_RUNNING 0x40
/**(n) No address resolution protocol */
#define IFF_NOARP 0x80
/** (n) Receive all packets */
#define IFF_PROMISC 0x100
/** (n) Receive all multicast packets */
#define IFF_ALLMULTI  0x200
/** (d) Indicates that the tx hardware queue is full */
#define IFF_DRV_OACTIVE 0x400
 /** (i) Cannot hear own transmissions */
#define IFF_SIMPLEX 0x800
/** Per link layer defined bit */
#define IFF_LINK0 0x1000
/** Per link layer defined bit */
#define IFF_LINK1 0x2000
/** Per link layer defined bit */
#define IFF_LINK2 0x4000
/** Use alternate physical connection */
#define IFF_ALTPHYS IFF_LINK2
/** Supports multicast */
#define IFF_MULTICAST 0x8000
 /** (i) unconfigurable using ioctl(2) */
#define IFF_CANTCONFIG  0x10000
/** (n) User-requested promisc mode */
#define IFF_PPROMISC  0x20000
/** (n) User-requested IFF_MONITOR mode */
#define IFF_MONITOR 0x40000
 /** (n) Static ARP */
#define IFF_STATICARP 0x80000
/** (n) Interface is winding down */
#define IFF_DYING 0x200000
/** (n) Interface is being renamed */
#define IFF_RENAMING  0x400000
/** (n) Dialup device with changing addresses */
#define IFF_DYNAMIC   0x800000
/** (n) Dialup device with changing addresses */
#define IFF_DYNAMIC_S   0x1000000

#define IFF_RUNNING IFF_DRV_RUNNING
/*
 * Interface request structure used for socket
 * ioctls.  All interface ioctls must have parameter
 * definitions which begin with ifr_name.  The
 * remainder may be interface specific.
 */
struct  ifreq {
  char  ifr_name[IFNAMSIZ];   /* if name, e.g. "en0" */
  union {
    struct  sockaddr ifru_addr;
    struct  sockaddr ifru_dstaddr;
    struct  sockaddr ifru_broadaddr;
    short ifru_flags[2];
    short ifru_index;
    int ifru_jid;
    int ifru_metric;
    int ifru_mtu;
    int ifru_phys;
    int ifru_media;
    int ifru_cap[2];
  } ifr_ifru;
};

#define ifr_addr  ifr_ifru.ifru_addr  /* address */
#define ifr_hwaddr  ifr_ifru.ifru_addr  /* MAC_address */
#define ifr_netmask  ifr_ifru.ifru_addr /* net mask */
#define ifr_dstaddr ifr_ifru.ifru_dstaddr /* other end of p-to-p link */
#define ifr_broadaddr ifr_ifru.ifru_broadaddr /* brcast address */
#define ifr_flags ifr_ifru.ifru_flags[0]  /* flags (low 16 bits) */
#define ifr_flagshigh ifr_ifru.ifru_flags[1]  /* flags (high 16 bits) */
#define ifr_jid   ifr_ifru.ifru_jid /* jail/vnet */
#define ifr_metric  ifr_ifru.ifru_metric  /* metric */
#define ifr_mtu   ifr_ifru.ifru_mtu /* mtu */
#define ifr_phys  ifr_ifru.ifru_phys  /* physical wire */
#define ifr_media ifr_ifru.ifru_media /* physical media */
#define ifr_reqcap  ifr_ifru.ifru_cap[0]  /* requested capabilities */
#define ifr_curcap  ifr_ifru.ifru_cap[1]  /* current capabilities */
#define ifr_index ifr_ifru.ifru_index /* interface index */

#define ifr_ifindex ifr_index

struct ifaddrs {
  struct ifaddrs  *ifa_next;
  char    *ifa_name;
  unsigned int   ifa_flags;
  struct sockaddr *ifa_addr;
  struct sockaddr *ifa_netmask;
  struct sockaddr *ifa_dstaddr;
  void    *ifa_data;
};

/*
 * This may have been defined in <net/if.h>.  Note that if <net/if.h> is
 * to be included it must be included before this header file.
 */
#ifndef ifa_broadaddr
#define ifa_broadaddr ifa_dstaddr /* brcast address interface */
#endif

struct rtentry {
  struct sockaddr rt_dst;         /* target address               */
  struct sockaddr rt_gateway;     /* gateway addr (RTF_GATEWAY)   */
  struct sockaddr rt_genmask;     /* target network mask (IP)     */
  unsigned int    rt_flags;
};
/** route usable */
#define RTF_UP    0x1
/** destination is a gateway */
#define RTF_GATEWAY 0x2
/** host entry (net otherwise) */
#define RTF_HOST  0x4
/** host or net unreachable */
#define RTF_REJECT  0x8
/** created dynamically (by redirect) */
#define RTF_DYNAMIC 0x10
/**  modified dynamically (by redirect) */
#define RTF_MODIFIED  0x20
/** message confirmed */
#define RTF_DONE  0x40
/*      0x80       unused, was RTF_DELCLONE */
/*      0x100      unused, was RTF_CLONING */
/** external daemon resolves name */
#define RTF_XRESOLVE  0x200
/** DEPRECATED - exists ONLY for backward compatibility */
#define RTF_LLINFO  0x400
/** used by apps to add/del L2 entries */
#define RTF_LLDATA  0x400
/** manually added */
#define RTF_STATIC  0x800
 /** just discard pkts(during updates) */
#define RTF_BLACKHOLE 0x1000
/** protocol specific routing flag */
#define RTF_PROTO2  0x4000
/** protocol specific routing flag */
#define RTF_PROTO1  0x8000
/*      0x10000      unused, was RTF_PRCLONING */
/*      0x20000      unused, was RTF_WASCLONED */
/** protocol specific routing flag */
#define RTF_PROTO3  0x40000
/** MTU was explicitly specified */
#define RTF_FIXEDMTU  0x80000
/** route is immutable */
#define RTF_PINNED  0x100000
/** Route represents a local address */
#define RTF_LOCAL 0x200000
/** Route represents a bcast address */
#define RTF_BROADCAST 0x400000
 /** Route represents a mcast address */
#define RTF_MULTICAST 0x800000
          /* 0x8000000 and up unassigned */
/** always route dst->src */
#define RTF_STICKY   0x10000000
/** radix node head is locked */
#define RTF_RNH_LOCKED   0x40000000
/** a compatibility bit for interacting with existing routing apps */
#define RTF_GWFLAG_COMPAT 0x80000000

/** Mask of RTF flags that are allowed to be modified by RTM_CHANGE. */
#define RTF_FMASK \
  (RTF_PROTO1 | RTF_PROTO2 | RTF_PROTO3 | RTF_BLACKHOLE | \
   RTF_REJECT | RTF_STATIC | RTF_STICKY)

/**
* @defgroup Network_Interfaces_Info  Network Interfaces Info
* This section contains the interfaces to get information about Network interfaces in lwIP.
* @ingroup User_interfaces
*/

/*
Func Name:  getifaddrs
*/
/**
* @ingroup   Network_Interfaces_Info
* @par Prototype
* @code
* int getifaddrs(struct ifaddrs **ifap);
* @endcode
*
* @par Purpose
* This function creates a linked list of structures describing the network
* interfaces of the local system, and stores the address of the first item
* of the list in *ifap.
*
* @par Description
* This function creates a linked list of struct ifaddrs, which holds the information
* about the network interfaces of the local system. The ifa_next field contains a
* pointer to the next structure on the list, or NULL if this is the last item of the list.
* The ifa_name points to the null-terminated interface name. The ifa_flags field
* contains the interface flags. The ifa_addr field points to a structure containing
* the interface address. The ifa_netmask field points to a structure containing
* the netmask associated with ifa_addr, if applicable, for the address family.
* Depending on whether the bit IFF_brcast or IFF_POINTOPOINT is set
* in ifa_flags (only one can be set at a time), either ifa_broadaddr will contain
* the broadcast address associated with ifa_addr (if applicable for the address
* family) or ifa_dstaddr will contain the destination address of the point-to-point
* interface.
* @param[in]    ifap  Indicates a double pointer to the ifaddrs structure.
*
* @par Return values
*  ERR_OK : on success
*  Non zero value : on failure
*
* @par Required Header File
* ifaddrs.h
*
* @par Note
* The data returned by getifaddrs() is dynamically allocated and should be freed
* using freeifaddrs() when no longer needed.\n
*
* @par Related Topics
* \n
* N/A
*/
int getifaddrs(struct ifaddrs **ifap);

/*
Func Name:  freeifaddrs
*/
/**
* @ingroup   Network_Interfaces_Info
* @par Prototype
* @code
* void freeifaddrs(struct ifaddrs *ifa);
* @endcode
*
* @par Purpose
* This function is to free the memory of the ifaddrs* structure provided by getifaddrs.
*
* @par Description
* The function getifaddrs, provides the list of network interfaces in the ifaddrs* structure.
* The application has to free the memory of the ifaddrs* structure by using this function.
*
* @param[in]    ifa                Indicates a pointer to the  ifaddrs structure.
*
* @par Return values
* None
*
* @par Required Header File
* ifaddrs.h
*
* @par Note
* \n
* N/A
*
* @par Related Topics
* \n
* N/A
*/
void freeifaddrs(struct ifaddrs *ifa);

#endif /* !LWIP_LITEIS_COMPAT */

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */

#endif /* __LWIP_IFADDRS_H */

