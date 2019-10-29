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
#ifndef __INCLUDE_NET_IF_H
#define __INCLUDE_NET_IF_H

#include <sys/types.h>
#include <sys/socket.h>

/* Length of interface name.  */
#define IFNAMSIZ	16

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
#define IFF_UP    0x1   /* (n) interface is up */
#define IFF_BROADCAST 0x2   /* (i) broadcast address valid */
#define IFF_DEBUG 0x4   /* (n) turn on debugging */
#define IFF_LOOPBACK  0x8   /* (i) is a loopback net */
#define IFF_POINTOPOINT 0x10    /* (i) is a point-to-point link */
  /*      0x20       was IFF_SMART */
#define IFF_DRV_RUNNING 0x40    /* (d) resources allocated */
#define IFF_NOARP 0x80    /* (n) no address resolution protocol */
#define IFF_PROMISC 0x100   /* (n) receive all packets */
#define IFF_ALLMULTI  0x200   /* (n) receive all multicast packets */
#define IFF_DRV_OACTIVE 0x400   /* (d) tx hardware queue is full */
#define IFF_SIMPLEX 0x800   /* (i) can't hear own transmissions */
#define IFF_LINK0 0x1000    /* per link layer defined bit */
#define IFF_LINK1 0x2000    /* per link layer defined bit */
#define IFF_LINK2 0x4000    /* per link layer defined bit */
#define IFF_ALTPHYS IFF_LINK2 /* use alternate physical connection */
#define IFF_MULTICAST 0x8000    /* (i) supports multicast */
#define IFF_CANTCONFIG  0x10000   /* (i) unconfigurable using ioctl(2) */
#define IFF_PPROMISC  0x20000   /* (n) user-requested promisc mode */
#define IFF_MONITOR 0x40000   /* (n) user-requested monitor mode */
#define IFF_STATICARP 0x80000   /* (n) static ARP */
#define IFF_DYING 0x200000  /* (n) interface is winding down */
#define IFF_RENAMING  0x400000  /* (n) interface is being renamed */
#define IFF_DYNAMIC   0x800000  /* (n) dialup device with changing addresses */
#define IFF_DYNAMIC_S   0x1000000 /* (n) dialup device with changing addresses */

#define IFF_RUNNING IFF_DRV_RUNNING

/*
 * Interface request structure used for socket
 * ioctl's.  All interface ioctl's must have parameter
 * definitions which begin with ifr_name.  The
 * remainder may be interface specific.
 */
struct  ifreq {
  char  ifr_name[IFNAMSIZ];   /* if name, e.g. "en0" */
  union {
    struct  sockaddr ifru_addr;
    struct  sockaddr ifru_dstaddr;
    struct  sockaddr ifru_broadaddr;
    //struct  ifreq_buffer ifru_buffer;
    short ifru_flags[2];
    short ifru_index;
    int ifru_jid;
    int ifru_metric;
    int ifru_mtu;
    int ifru_phys;
    int ifru_media;
    //caddr_t ifru_data;
    int ifru_cap[2];
    //u_int ifru_fib;
  } ifr_ifru;
};

#define ifr_addr  ifr_ifru.ifru_addr  /* address */
#define ifr_hwaddr  ifr_ifru.ifru_addr  /* MAC address */
#define ifr_netmask  ifr_ifru.ifru_addr /* net mask */
#define ifr_dstaddr ifr_ifru.ifru_dstaddr /* other end of p-to-p link */
#define ifr_broadaddr ifr_ifru.ifru_broadaddr /* broadcast address */
//#define ifr_buffer  ifr_ifru.ifru_buffer  /* user supplied buffer with its length */
#define ifr_flags ifr_ifru.ifru_flags[0]  /* flags (low 16 bits) */
#define ifr_flagshigh ifr_ifru.ifru_flags[1]  /* flags (high 16 bits) */
#define ifr_jid   ifr_ifru.ifru_jid /* jail/vnet */
#define ifr_metric  ifr_ifru.ifru_metric  /* metric */
#define ifr_mtu   ifr_ifru.ifru_mtu /* mtu */
#define ifr_phys  ifr_ifru.ifru_phys  /* physical wire */
#define ifr_media ifr_ifru.ifru_media /* physical media */
//#define ifr_data  ifr_ifru.ifru_data  /* for use by interface */
#define ifr_reqcap  ifr_ifru.ifru_cap[0]  /* requested capabilities */
#define ifr_curcap  ifr_ifru.ifru_cap[1]  /* current capabilities */
#define ifr_index ifr_ifru.ifru_index /* interface index */
//#define ifr_fib   ifr_ifru.ifru_fib /* interface fib */

#define ifr_ifindex ifr_index

#endif /* __INCLUDE_NET_IF_H */
