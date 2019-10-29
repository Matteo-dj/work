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
#ifndef __INCLUDE_NET_ROUTE_H
#define __INCLUDE_NET_ROUTE_H

#include <sys/types.h>
#include <sys/socket.h>

struct rtentry {
  struct sockaddr rt_dst;         /* target address               */
  struct sockaddr rt_gateway;     /* gateway addr                 */
  struct sockaddr rt_genmask;     /* target network mask (IP)     */
  unsigned int rt_flags;
};

#define RTF_GWFLAG_COMPAT 0x80000000  
#define RTF_RNH_LOCKED   0x40000000 /* radix node head is locked */

#define RTF_STICKY   0x10000000 /* always route dst->src */
#define RTF_MULTICAST 0x800000  /* route represents a mcast address */
#define RTF_BROADCAST 0x400000  /* route represents a bcast address */
#define RTF_LOCAL 0x200000  /* route represents a local address */
#define RTF_PINNED  0x100000  /* route is immutable */
#define RTF_FIXEDMTU  0x80000   /* MTU was explicitly specified */
#define RTF_PROTO3  0x40000
#define RTF_PROTO1  0x8000    
#define RTF_PROTO2  0x4000    
#define RTF_BLACKHOLE 0x1000    /* just discard pkts (during updates) */
#define RTF_STATIC  0x800   
#define RTF_LLINFO  0x400   
#define RTF_LLDATA  0x400   /* used by apps to add/del L2 entries */
#define RTF_XRESOLVE  0x200   /* external daemon resolves name */
#define RTF_DONE  0x40    /* message confirmed */
#define RTF_MODIFIED  0x20    /* modified dynamically (by redirect) */
#define RTF_DYNAMIC 0x10    /* created dynamically (by redirect) */
#define RTF_REJECT  0x8   /* host or net unreachable */
#define RTF_HOST  0x4   /* host entry (net otherwise) */
#define RTF_GATEWAY 0x2   /* destination is a gateway */
#define RTF_UP    0x1   /* route usable */

/* Mask of RTF flags that are allowed to be modified by RTM_CHANGE. */
#define RTF_FMASK \
  (RTF_PROTO1 | RTF_PROTO2 | RTF_PROTO3 | RTF_BLACKHOLE | \
   RTF_REJECT | RTF_STATIC | RTF_STICKY)

#endif /* __INCLUDE_NET_ROUTE_H */

