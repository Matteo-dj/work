/**
 * @file
 * This is the IPv4 address tools implementation.
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
#include "lwip/ip_addr.h"
#include "lwip/netif.h"
#include "lwip/inet.h"

/* used by IP_ADDR_ANY and IP_ADDR_brcast in ip_addr.h */
const ip_addr_t ip_addr_any = { IPADDR_ANY };
const ip_addr_t ip_addr_broadcast = { IPADDR_BROADCAST };

/**
 * Determine if an address is a brcast address on a network interface
 *
 * @param addr address to be checked
 * @param netif the network interface against which the address is checked
 * @return returns non-zero if the address is a brcast address
 */
u8_t
ip4_addr_isbroadcast(u32_t addr, const struct netif *netif)
{
  ip_addr_t ipaddr;
  ip4_addr_set_u32(&ipaddr, addr);

  /* all ones (brcast) or all zeroes (old skool brcast) */
  if ((~addr == IPADDR_ANY) ||
      (addr == IPADDR_ANY)) {
    return 1;
  /* no brcast support on this network interface? */
  } else if ((netif->flags & NETIF_FLAG_BROADCAST) == 0) {
    /* the given address cannot be a brcast address
     * nor can we check against any brcast addresses */
    return 0;
  /* address matches network interface address exactly? => no brcast */
  } else if (addr == ip4_addr_get_u32(&netif->ip_addr)) {
    return 0;
  /*  on the same (sub) network... */
  } else if (ip_addr_netcmp(&ipaddr, &(netif->ip_addr), &(netif->netmask))
         /* ...and host identifier bits are all ones? =>... */
          && ((addr & ~ip4_addr_get_u32(&netif->netmask)) ==
           (IPADDR_BROADCAST & ~ip4_addr_get_u32(&netif->netmask)))) {
    /* => network brcast address */
    return 1;
  } else {
    return 0;
  }
}


/** Checks if a netmask is valid (starting with ones, then only zeros)
 *
 * @param netmask the IPv4 netmask to check (in network byte order!)
 * @return 1 if the netmask is valid, 0 if it is not
 */
u8_t
ip4_addr_netmask_valid(u32_t netmask)
{
  u32_t mask;
  u32_t nm_hostorder = lwip_htonl(netmask);

  /* first, check for the first zero */
  for (mask = 1UL << 31 ; mask != 0; mask >>= 1) {
    if ((nm_hostorder & mask) == 0) {
      break;
    }
  }
  /* then check that there is no one */
  for (; mask != 0; mask >>= 1) {
    if ((nm_hostorder & mask) != 0) {
      /* there is a one after the first zero -> invalid */
      return 0;
    }
  }
  /* no one after the first zero -> valid */
  return 1;
}

/* Here for now until needed in other places in lwIP */
#ifndef isprint
#define in_range(c, lo, up)  ((u8_t)c >= lo && (u8_t)c <= up)
/* PC Lint Fix: Macro isprint is not referenced */
#define isdigit(c)           in_range(c, '0', '9')
#define isxdigit(c)          (isdigit(c) || in_range(c, 'a', 'f') || in_range(c, 'A', 'F'))
#define islower(c)           in_range(c, 'a', 'z')
#define isspace(c)           (c == ' ' || c == '\f' || c == '\n' || c == '\r' || c == '\t' || c == '\v')
#endif

/**
 * Ascii internet address interpretation routine.
 * The value returned is in network order.
 *
 * @param cp IP_add in ascii represenation (e.g. "127.0.0.1")
 * @return IP_add in network order
 */
u32_t
ipaddr_addr(const char *cp)
{
  ip_addr_t val;

  LWIP_ERROR("ipaddr_addr:cp is NULL", (cp!= NULL) , return IPADDR_NONE;);
  if (ipaddr_aton(cp, &val)) {
    return ip4_addr_get_u32(&val);
  }
  return (IPADDR_NONE);
}

/**
 * Check whether "cp" is a valid ascii representation
 * of an Internet address and convert to a binary address.
 * Returns 1 if the address is valid, 0 if not.
 * This replaces inet_addr, the return value from which
 * cannot distinguish between failure and a local brcast address.
 *
 * @param cp IP_add in ascii represenation (e.g. "127.0.0.1")
 * @param addr pointer to which to save the IP_add in network order
 * @return 1 if cp could be converted to addr, 0 on failure
 */
int
ipaddr_aton(const char *cp, ip_addr_t *addr)
{
  u32_t val;
  u64_t temp_val;
  u8_t base;
  char c;
  u32_t parts[4];
  u32_t *pp = parts;

  c = *cp;
  for (;;) {
    /*
     * Gather numbers up to ``.''.
     * Values are specified as for C:
     * 0x=hex, 0=octal, 1-9=decimal.
     */
    if (!isdigit(c))
      return (0);
    temp_val = 0;
    val = 0;
    base = 10;
    if (c == '0') {
      c = *++cp;
      if (c == 'x' || c == 'X') {
        base = 16;
        c = *++cp;
      } else
        base = 8;
    }
    for (;;) {
      if (isdigit(c)) {
        /* PC Lint fix for: Loss of sign in promotion from int to unsigned long */
        /* possible values of c : 0 to 9, so typecast to unsigned long */
        temp_val = (temp_val * base) + (u32_t)((int)(c - '0'));
        if(temp_val > 0xFFFFFFFF)
            return(0);
        val =  (u32_t)temp_val;
        c = *++cp;
      } else if (base == 16 && isxdigit(c)) {
        /* PC Lint fix for: Loss of sign in promotion from int to unsigned long */
        /* possible values of c : 0 to 9, so typecast to unsigned long */
        temp_val = (temp_val << 4) | (u32_t)((int)(c + 10 - (islower(c) ? 'a' : 'A')));
        if(temp_val > 0xFFFFFFFF)
            return(0);
        val = (u32_t)temp_val;
        c = *++cp;
      } else
        break;
    }
    if (c == '.') {
      /*
       * Internet format:
       *  a.b.c.d
       *  a.b.c   (with c treated as 16 bits)
       *  a.b (with b treated as 24 bits)
       */
      if (pp >= parts + 3) {
        return (0);
      }
      *pp++ = val;
      c = *++cp;
    } else
      break;
  }
  /*
   * Check for trailing characters.
   */
  if (c != '\0' && !isspace(c)) {
    return (0);
  }
  /*
   * Concoct the address according to
   * the number of parts specified.
   */
  /* PC Lint Fix for: Operator '-' followed by operator '+' is confusing.  Use parentheses */
  switch ((pp - parts) + 1) {

  case 0:
    return (0);       /* initial nondigit */

  case 1:             /* a -- 32 bits */
    break;

  case 2:             /* a.b -- 8.24 bits */
    if (val > 0xffffffUL || parts[0] > 0xff) {
      return (0);
    }
    val |= parts[0] << 24;
    break;

  case 3:             /* a.b.c -- 8.8.16 bits */
    if (val > 0xffff || parts[0] > 0xff || parts[1] > 0xff) {
      return (0);
    }
    val |= (parts[0] << 24) | (parts[1] << 16);
    break;

  case 4:             /* a.b.c.d -- 8.8.8.8 bits */
    if (val > 0xff || parts[0] > 0xff || parts[1] > 0xff || parts[2] > 0xff) {
      return (0);
    }
    val |= (parts[0] << 24) | (parts[1] << 16) | (parts[2] << 8);
    break;
  default:
    LWIP_ASSERT("unhandled", 0);
    break;
  }
  if (addr) {
    ip4_addr_set_u32(addr, htonl(val));
  }
  return (1);
}

/**
 * Convert numeric IP_add into decimal dotted ASCII representation.
 * returns ptr to static buffer; not reentrant!
 *
 * @param addr IP_add in network order to convert
 * @return pointer to a global static (!) buffer that holds the ASCII
 *         represenation of addr
 */
char *
ipaddr_ntoa(const ip_addr_t *addr)
{
  static char str[16];
  return ipaddr_ntoa_r(addr, str, 16);
}

/**
 * Same as ipaddr_ntoa, but reentrant since a user-supplied buffer is used.
 *
 * @param addr IP_add in network order to convert
 * @param buf target buffer where the string is stored
 * @param buflen length of buf
 * @return either pointer to buf which now holds the ASCII
 *         representation of addr or NULL if buf was too small
 */
char *ipaddr_ntoa_r(const ip_addr_t *addr, char *buf, int buflen)
{
  u32_t s_addr;
  char inv[4];
  char *rp;
  u8_t *ap;
  u8_t rem;
  u8_t n;
  u8_t i;
  int len = 0;

  LWIP_ERROR("Either addr or buf is NULL", ((addr != NULL) && (buf != NULL)),
      return NULL;);

  s_addr = ip4_addr_get_u32(addr);

  rp = buf;
  ap = (u8_t *)&s_addr;
  for(n = 0; n < 4; n++) {
    i = 0;
    do {
      rem = *ap % (u8_t)10;
      *ap /= (u8_t)10;
      /* PC Lint Fix for: Loss of precision (assignment) 8 bits to 7 bits */
      inv[i++] = (char)('0' + (char)rem);
    } while(*ap);
    while(i--) {
      if (len++ >= buflen) {
        return NULL;
      }
      *rp++ = inv[i];
    }
    if (len++ >= buflen) {
      return NULL;
    }
    *rp++ = '.';
    ap++;
  }
  *--rp = 0;
  return buf;
}

#if LWIP_INET_ADDR_FUNC

/**
 * Ascii internet address interpretation routine.
 * The value returned is in network order.
 *
 * @param cp IP_add in ascii represenation (e.g. "127.0.0.1")
 * @return IP_add in network order
 */
unsigned int  inet_addr (const char * cp)
{
     return ipaddr_addr(cp);
}

#endif

#if LWIP_INET_NTOA_FUNC

/**
 * Convert numeric IP_add into decimal dotted ASCII representation.
 * returns ptr to static buffer; not reentrant!
 *
 * @param in IP_add in network order to convert
 * @return pointer to a global static (!) buffer that holds the ASCII
 *         represenation of addr
 */

char *inet_ntoa (struct in_addr in)
{
    return ipaddr_ntoa((ip_addr_t *)&in);
}

#endif

#if LWIP_INET_ATON_FUNC

/**
 * Convert numeric IP_add into decimal dotted ASCII representation.
 * returns ptr to static buffer; not reentrant!
 *
 * @param in IP_add in network order to convert
 * @return pointer to a global static (!) buffer that holds the ASCII
 *         represenation of addr
 */

int inet_aton(const char *cp, struct in_addr *addr)
{
    LWIP_ERROR("inet_aton:cp is NULL", (cp != NULL) , return 0;);
    return ipaddr_aton(cp, (ip_addr_t*)addr);
}


#endif

