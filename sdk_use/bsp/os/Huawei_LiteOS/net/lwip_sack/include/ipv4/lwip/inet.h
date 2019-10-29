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

#ifndef __LWIP_INET_H__
#define __LWIP_INET_H__

#include "lwip/opt.h"
#include "lwip/def.h"
#include "lwip/ip_addr.h"

#ifdef __cplusplus
extern "C" {
#endif

#if !LWIP_LITEOS_COMPAT
/* For compatibility with BSD code */
/**
* Provides a binary notation of the IPv4 address. 
*/
struct in_addr {
  u32_t s_addr;  /**<  Specifies the s_addr. */
};

/* 255.255.255.255 */
#define INADDR_NONE         IPADDR_NONE
/* 127.0.0.1 */
#define INADDR_LOOPBACK     IPADDR_LOOPBACK
/* 0.0.0.0 */
#define INADDR_ANY          IPADDR_ANY
/* 255.255.255.255 */
#define INADDR_BROADCAST    IPADDR_BROADCAST

/* Definitions of the bits in an Internet address integer.

   On subnets, host and network parts are found according to
   the subnet mask, not these masks.  */
#define IN_CLASSA(a)        IP_CLASSA(a)
#define IN_CLASSA_NET       IP_CLASSA_NET
#define IN_CLASSA_NSHIFT    IP_CLASSA_NSHIFT
#define IN_CLASSA_HOST      IP_CLASSA_HOST
#define IN_CLASSA_MAX       IP_CLASSA_MAX

#define IN_CLASSB(b)        IP_CLASSB(b)
#define IN_CLASSB_NET       IP_CLASSB_NET
#define IN_CLASSB_NSHIFT    IP_CLASSB_NSHIFT
#define IN_CLASSB_HOST      IP_CLASSB_HOST
#define IN_CLASSB_MAX       IP_CLASSB_MAX

#define IN_CLASSC(c)        IP_CLASSC(c)
#define IN_CLASSC_NET       IP_CLASSC_NET
#define IN_CLASSC_NSHIFT    IP_CLASSC_NSHIFT
#define IN_CLASSC_HOST      IP_CLASSC_HOST
#define IN_CLASSC_MAX       IP_CLASSC_MAX

#define IN_CLASSD(d)        IP_CLASSD(d)
#define IN_CLASSD_NET       IP_CLASSD_NET     /* These ones aren't really */
#define IN_CLASSD_NSHIFT    IP_CLASSD_NSHIFT  /*   net and host fields, but */
#define IN_CLASSD_HOST      IP_CLASSD_HOST    /*   routing needn't know. */
#define IN_CLASSD_MAX       IP_CLASSD_MAX

#define IN_MULTICAST(a)     IP_MULTICAST(a)

#define IN_EXPERIMENTAL(a)  IP_EXPERIMENTAL(a)
#define IN_BADCLASS(a)      IP_BADCLASS(a)

#define IN_LOOPBACKNET      IP_LOOPBACKNET
#endif /* !LWIP_LITEOS_COMPAT */


#define inet_addr_from_ipaddr(target_inaddr, source_ipaddr) ((target_inaddr)->s_addr = ip4_addr_get_u32(source_ipaddr))
#define inet_addr_to_ipaddr(target_ipaddr, source_inaddr)   (ip4_addr_set_u32(target_ipaddr, (source_inaddr)->s_addr))
/* ATTENTION: the next define only works because both s_addr and ip_addr_t are an u32_t effectively! */
#define inet_addr_to_ipaddr_p(target_ipaddr_p, source_inaddr)   ((target_ipaddr_p) = (ip_addr_t*)&((source_inaddr)->s_addr))

/* directly map this to the lwip internal functions */

/**
* @defgroup INET_Interfaces  INET Interfaces
* This section contains information about INET interfaces.
* @ingroup User_interfaces
*/

/*
Func Name:  inet_addr
*/

/**
* @ingroup INET_Interfaces
* @par Prototype
* @code
* unsigned int  inet_addr (const char * cp);
* @endcode
*
* @par Purpose
* This function is used to convert the IPv4 address from string notation (number and dot format)
* to binary notation of network byte order.
*
* @par Description
* This function is used to convert the IPv4 address from string notation (number and dot format)
* to binary notation of network byte order.
*
* @param[in]    cp  Pointer to IPv4 address string 
*
* @par Return values
*  Valid IP_add: On success, in unsigned int (32bit) format \n
*  IPADDR_NONE : On failure \n
*
* @par Required Header File
* inet.h
*
* @par Note
* This interface will be available as function if LWIP_INET_ADDR_FUNC is enabled,
* otherwise it will be available as macro.
*
* @par Related Topics

* N/A
*/

#if LWIP_INET_ADDR_FUNC
unsigned int  inet_addr (const char * cp);
#else
#define inet_addr(cp)         ipaddr_addr(cp)
#endif

/*
Func Name:  inet_aton
*/

/**
* @ingroup INET_Interfaces
* @par Prototype
* @code
* int inet_aton(const char *cp, struct in_addr *addr)
* @endcode
*
* @par Purpose
* This function is used to convert an IPv4 address from string notation (number and dot format)
* to binary notation of network byte order and stores it in the structure that addr points to.
*
* @par Description
* This function is used to convert the IPv4 address from string notation (number and dot format)
* to binary notation of network byte order and stores it in the structure that addr points to.
*
* @param[in]    cp     Indicates a pointer to an IPv4 address string. [N/A]
* @param[out]  addr  Indicates a the generated binary notation of the IPv4 address. [N/A]
*
* @par Return values
*  1 : On success\n
*  0 : On failure \n
*
* @par Required Header File
* inet.h
*
* @par Note
* <ul><li>This interface is same as inet_addr(). The only difference is that the generated binary notation
* of IPv4 address is updated in the addr parameter, instead of returning it.</li>
* <li>This interface is available as only macro.</li></ul>
*
* @par Related Topics
* N/A
*/

#if LWIP_INET_ATON_FUNC
int inet_aton(const char *cp, struct in_addr *addr);
#else
#define inet_aton(cp,addr) ((cp == NULL) ? 0: \
                           (ipaddr_aton(cp, (ip_addr_t *)addr)))

#endif

/*
Func Name:  inet_ntoa
*/

/**
* @ingroup INET_Interfaces
* @par Prototype
* @code
* char *inet_ntoa (struct in_addr in);
* @endcode
*
* @par Purpose
* This function is used to convert an IPv4 address from binary notation (network byte order)
* to string notation (number and dot format).
*
* @par Description
* This function is used to convert the IPv4 address from binary notation (network byte order)
* to string notation (number and dot format).
*
* @param[in]    in     Indicates a pointer to the binary notation of an IPv4 address. [N/A]
*
* @par Return values
*  Valid pointer : On success, returns a pointer to the string notation of the IPv4 address. \n
*  NULL : On failure \n
*
* @par Required Header File
* inet.h
*
* @par Note
* This interface is available as a function if LWIP_INET_NTOA_FUNC is enabled,
* otherwise it will be available as macro.
*
* @par Related Topics
* N/A
*/

#if LWIP_INET_NTOA_FUNC
char *inet_ntoa (struct in_addr in);
#else
#define inet_ntoa(addr)       ipaddr_ntoa((ip_addr_t*)&(addr))
#endif


/*
Func Name:  inet_ntoa_r
*/

/**
* @ingroup INET_Interfaces
* @par Prototype
* @code
* cchar *inet_ntoa_r(const ip_addr_t addr, char *buf, int buflen)
* @endcode
*
* @par Purpose
* This function is used to convert an IPv4 address from binary notation (network byte order)
* to string notation (number and dot format). This is the reentrant API of inet_ntoa.
*
* @par Description
* This function is used to convert an IPv4 address from binary notation (network byte order)
* to string notation (number and dot format). This is the reentrant API of inet_ntoa. The
* generated string notation IPv4 address is updated in the buffer passed by the user.
*
* @param[in]   addr     Indicates the binary notation of an IPv4 address. [N/A]
* @param[out]  buf   Indicates a pointer to a user buffer, in which the output string format of the IPv4 address is updated. [N/A]
* @param[in]    buflen  Indicates the length of the user buffer. [N/A]
*
* @par Return values
*  Valid pointer : On success, returns a pointer to the buffer passed by the user in buf. \n
*  NULL : On failure \n
*
* @par Required Header File
* inet.h
*
* @par Note
* This interface is available as only macro.
*
* @par Related Topics
* N/A
*/

#define inet_ntoa_r(addr, buf, buflen) ipaddr_ntoa_r((ip_addr_t*)&(addr), buf, buflen)

#ifdef __cplusplus
}
#endif

#endif /* __LWIP_INET_H__ */
