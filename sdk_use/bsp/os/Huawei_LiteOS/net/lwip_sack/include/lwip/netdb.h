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
 * Author: Simon Goldschmidt
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

#ifndef __LWIP_NETDB_H__
#define __LWIP_NETDB_H__

#include "lwip/opt.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#if LWIP_SOCKET

#include <stddef.h> /* for size_t */

#include "lwip/inet.h"
#include "lwip/sockets.h"

#ifdef __cplusplus
extern "C" {
#endif

/* some rarely used options */
#ifndef LWIP_DNS_API_DECLARE_H_ERRNO
#define LWIP_DNS_API_DECLARE_H_ERRNO 1
#endif

#ifndef LWIP_DNS_API_DEFINE_ERRORS
#define LWIP_DNS_API_DEFINE_ERRORS 1
#endif

#ifndef LWIP_DNS_API_DECLARE_STRUCTS
#define LWIP_DNS_API_DECLARE_STRUCTS 1
#endif

#if LWIP_DNS_API_DEFINE_ERRORS
/** Errors used by the DNS API functions, h_errno can be one of them */
#define EAI_NONAME      200
#define EAI_SERVICE     201
#define EAI_FAIL        202
#define EAI_MEMORY      203

#define HOST_NOT_FOUND  210
#define NO_DATA         211
#define NO_RECOVERY     212
#define TRY_AGAIN       213
#endif /* LWIP_DNS_API_DEFINE_ERRORS */

#if LWIP_DNS_API_DECLARE_STRUCTS
struct hostent {
    char  *h_name;      /* Official name of the host. */
    char **h_aliases;   /* A pointer to an array of pointers to alternative host names,
                           terminated by a null pointer. */
    int    h_addrtype;  /* Address type. */
    int    h_length;    /* The length, in bytes, of the address. */
    char **h_addr_list; /* A pointer to an array of pointers to network addresses (in
                           network byte order) for the host, terminated by a null pointer. */
};

#define h_addr h_addr_list[0]

struct addrinfo {
    int               ai_flags;      /* Input flags. */
    int               ai_family;     /* Address family of socket. */
    int               ai_socktype;   /* Socket type. */
    int               ai_protocol;   /* Protocol of socket. */
    socklen_t         ai_addrlen;    /* Length of socket address. */
    struct sockaddr  *ai_addr;       /* Socket address of socket. */
    char             *ai_canonname;  /* Canonical name of service location. */
    struct addrinfo  *ai_next;       /* Pointer to next in list. */
};

/** helper struct for gethostbyname_r to access the char* buffer */
struct gethostbyname_r_helper {
  ip_addr_t *addr_list[DNS_MAX_IPADDR + 1];
  ip_addr_t addr[DNS_MAX_IPADDR];
  char *aliases;
};

#endif /* LWIP_DNS_API_DECLARE_STRUCTS */

#if LWIP_DNS

#if LWIP_DNS_API_DECLARE_H_ERRNO
/* application accessable error code set by the DNS API functions */
extern int h_errno;
#endif /* LWIP_DNS_API_DECLARE_H_ERRNO*/

/*
Func Name:  lwip_gethostbyname
*/
/**
* @defgroup lwip_gethostbyname
* @ingroup DNS_Interfaces
* @par Prototype
* @code
* struct hostent *lwip_gethostbyname(const char *name);
* @endcode
*
* @par Purpose
*  This API is used to resolve a hostname (string) into an IP_add.
*
* @par Description
*  This API is used to resolve a hostname (string) into an IP_add.
*
* @param[in]    name  The hostname that is to be resolved. [NA]
* @param[out]  [N/A]
*
* @retval hostent * On successful execution, pointer to the hostent  structure is returned.[Pointer to hostent|NA]
* @retval hostent * On failure conditions, NULL is returned.[NULL|NA]
*
* @par Required Header File
* netdb.h
*
* @par Note
* 1. This API can give more than one IP_add.\n
* 2. While parsing the multiple answer rcrds in DNS response message, if
* it encounters any malformed answer rcrd then it stops parsing and returns
* success if it has any successfully parsed rcrd or else it returns failure.\n
*
* @par Related Topics
* \n
* N/A
*/

struct hostent *lwip_gethostbyname(const char *name);

/*
Func Name:  lwip_gethostbyname_r
*/
/**
* @defgroup lwip_gethostbyname_r
* @ingroup DNS_Interfaces
* @par Prototype
* @code
* int lwip_gethostbyname_r(const char *name, struct hostent *ret, char *buf,
*                       size_t buflen, struct hostent **result, int *h_errnop);
* @endcode
*
* @par Purpose
*  This API is used to resolve a hostname (string) into an IP_add.This function is Thread-safe variant of lwip_gethostbyname.
*
* @par Description
*  This API is used to resolve a hostname (string) into an IP_add.This function is Thread-safe variant of lwip_gethostbyname.
*
* @param[in]    name   The hostname that is to be resolved. [NA]
* @param[out]  ret       The pre-allocated struct where the result is stored.[NA]
* @param[in]    buf      The pre-allocated buffer where additional data is stored.[NA]
* @param[in]    buflen  The size of the buf.[NA]
* @param[out]  result   This is a double Pointer to hostent which is set to ret on success
*                                 and set to zero on error scenario.[NA]
* @param[out]  h_errnop It stores the errors.[NA]
*
* @retval int  On successful execution. [0|NA]
* @retval int  On failure conditions. [-1|NA]
*
* @par Required Header File
* netdb.h
*
* @par Note
* 1. Additional error information is stored in *h_errnop instead of h_errno to be thread-safe.\n
* 2. This API can give more than one IP_add also.\n
* 3. While parsing the multiple answer rcrds in DNS response message, if
* it encounters any malformed answer rcrd then it stops parsing and returns
* success if it has any successfully parsed rcrd or else it returns failure.\n
*
* @par Related Topics
* \n
* N/A
*/

int lwip_gethostbyname_r(const char *name, struct hostent *ret, char *buf,
                size_t buflen, struct hostent **result, int *h_errnop);
void lwip_freeaddrinfo(struct addrinfo *ai);
int lwip_getaddrinfo(const char *nodename,
       const char *servname,
       const struct addrinfo *hints,
       struct addrinfo **res);

#if LWIP_COMPAT_SOCKETS
#define gethostbyname(name) lwip_gethostbyname(name)
#define gethostbyname_r(name, ret, buf, buflen, result, h_errnop) \
       lwip_gethostbyname_r(name, ret, buf, buflen, result, h_errnop)
#define freeaddrinfo(addrinfo) lwip_freeaddrinfo(addrinfo)
#define getaddrinfo(nodname, servname, hints, res) \
       lwip_getaddrinfo(nodname, servname, hints, res)
#endif /* LWIP_COMPAT_SOCKETS */

#endif /* LWIP_DNS */

#ifdef __cplusplus
}
#endif

#endif /* LWIP_SOCKET */

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __LWIP_NETDB_H__ */
