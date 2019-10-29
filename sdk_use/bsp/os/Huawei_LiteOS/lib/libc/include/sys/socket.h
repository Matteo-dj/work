/****************************************************************************
 * include/sys/socket.h
 *
 *   Copyright (C) 2007, 2009, 2011 Gregory Nutt. All rights reserved.
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

#ifndef __INCLUDE_SYS_SOCKET_H
#define __INCLUDE_SYS_SOCKET_H

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
 * Definitions
 ****************************************************************************/

/* Types of sockets.  */
#define SOCK_STREAM      1
#define SOCK_DGRAM       2
#define SOCK_RAW         3
#define SOCK_RDM         4
#define SOCK_SEQPACKET   5
#define SOCK_PACKET      10
#define SOCK_CLOEXEC     02000000
#define SOCK_NONBLOCK    04000

/* Address families.  */
#define AF_UNSPEC       0
#define AF_UNIX         1
#define AF_LOCAL        1
#define AF_FILE         1
#define AF_INET         2
#define AF_AX25         3
#define AF_IPX          4
#define AF_APPLETALK    5
#define AF_NETROM       6
#define AF_BRIDGE       7
#define AF_ATMPVC       8
#define AF_X25          9
#define AF_INET6        10
#define AF_ROSE         11
#define AF_DECnet       12
#define AF_NETBEUI      13
#define AF_SECURITY     14
#define AF_KEY          15
#define AF_NETLINK      16
#define AF_ROUTE        AF_NETLINK
#define AF_PACKET       17
#define AF_ASH          18
#define AF_ECONET       19
#define AF_ATMSVC       20
#define AF_RDS          21
#define AF_SNA          22
#define AF_IRDA         23
#define AF_PPPOX        24
#define AF_WANPIPE      25
#define AF_LLC          26
#define AF_CAN          29
#define AF_TIPC         30
#define AF_BLUETOOTH    31
#define AF_IUCV         32
#define AF_RXRPC        33
#define AF_ISDN         34
#define AF_PHONET       35
#define AF_IEEE802154   36
#define AF_CAIF         37
#define AF_ALG          38
#define AF_MAX          39

/* Protocol families.  */
#define PF_UNSPEC       AF_UNSPEC
#define PF_UNIX         AF_UNIX
#define PF_LOCAL        AF_LOCAL
#define PF_FILE         AF_FILE
#define PF_INET         AF_INET
#define PF_AX25         AF_AX25
#define PF_IPX          AF_IPX
#define PF_APPLETALK    AF_APPLETALK
#define PF_NETROM       AF_NETROM
#define PF_BRIDGE       AF_BRIDGE
#define PF_ATMPVC       AF_ATMPVC
#define PF_X25          AF_X25
#define PF_INET6        AF_INET6
#define PF_ROSE         AF_ROSE
#define PF_DECnet       AF_DECnet
#define PF_NETBEUI      AF_NETBEUI
#define PF_SECURITY     AF_SECURITY
#define PF_KEY          AF_KEY
#define PF_NETLINK      AF_NETLINK
#define PF_ROUTE        AF_ROUTE
#define PF_PACKET       AF_PACKET
#define PF_ASH          AF_ASH
#define PF_ECONET       AF_ECONET
#define PF_ATMSVC       AF_ATMSVC
#define PF_RDS          AF_RDS
#define PF_SNA          AF_SNA
#define PF_IRDA         AF_IRDA
#define PF_PPPOX        AF_PPPOX
#define PF_WANPIPE      AF_WANPIPE
#define PF_LLC          AF_LLC
#define PF_CAN          AF_CAN
#define PF_TIPC         AF_TIPC
#define PF_BLUETOOTH    AF_BLUETOOTH
#define PF_IUCV         AF_IUCV
#define PF_RXRPC        AF_RXRPC
#define PF_ISDN         AF_ISDN
#define PF_PHONET       AF_PHONET
#define PF_IEEE802154   AF_IEEE802154
#define PF_CAIF         AF_CAIF
#define PF_ALG          AF_ALG
#define PF_MAX          AF_MAX

/* Bits in the FLAGS argument to `send', `recv', et al. These are the bits
 * recognized by Linus, not all are supported by NuttX.
 */

#define MSG_OOB        0x0001 /* Process out-of-band data.  */
#define MSG_PEEK       0x0002 /* Peek at incoming messages.  */
#define MSG_DONTROUTE  0x0004 /* Don't use local routing.  */
#define MSG_CTRUNC     0x0008 /* Control data lost before delivery.  */
#define MSG_PROXY      0x0010 /* Supply or ask second address.  */
#define MSG_TRUNC      0x0020
#define MSG_DONTWAIT   0x0040 /* Enable nonblocking IO.  */
#define MSG_EOR        0x0080 /* End of record.  */
#define MSG_WAITALL    0x0100 /* Wait for a full request.  */
#define MSG_FIN        0x0200
#define MSG_SYN        0x0400
#define MSG_CONFIRM    0x0800 /* Confirm path validity.  */
#define MSG_RST        0x1000
#define MSG_ERRQUEUE   0x2000 /* Fetch message from error queue.  */
#define MSG_NOSIGNAL   0x4000 /* Do not generate SIGPIPE.  */
#define MSG_MORE       0x8000 /* Sender will send more.  */

/* Protocol levels supported by get/setsockopt(): */

/* Protocol number used to manipulate socket-level options
   with `getsockopt' and `setsockopt'.  */
#define SOL_SOCKET      1

#define SO_DEBUG        1
#define SO_REUSEADDR    2
#define SO_TYPE         3
#define SO_ERROR        4
#define SO_DONTROUTE    5
#define SO_BROADCAST    6
#define SO_SNDBUF       7
#define SO_RCVBUF       8
#define SO_SNDBUFFORCE  32
#define SO_RCVBUFFORCE  33
#define SO_KEEPALIVE    9
#define SO_OOBINLINE    10
#define SO_NO_CHECK     11
#define SO_PRIORITY     12
#define SO_LINGER       13
#define SO_BSDCOMPAT    14
#define SO_REUSEPORT    15
#define SO_PASSCRED     16
#define SO_PEERCRED     17
#define SO_RCVLOWAT     18
#define SO_SNDLOWAT     19
#define SO_RCVTIMEO     20
#define SO_SNDTIMEO     21
/* Security levels - as per NRL IPv6 - don't actually do anything */
#define SO_SECURITY_AUTHENTICATION          22
#define SO_SECURITY_ENCRYPTION_TRANSPORT    23
#define SO_SECURITY_ENCRYPTION_NETWORK      24

#define SO_BINDTODEVICE                     25

/* Socket filtering */
#define SO_ATTACH_FILTER        26
#define SO_DETACH_FILTER        27

#define SO_PEERNAME             28
#define SO_TIMESTAMP            29
#define SCM_TIMESTAMP           SO_TIMESTAMP

#define SO_ACCEPTCONN           30

#define SO_PEERSEC              31
#define SO_PASSSEC              34
#define SO_TIMESTAMPNS          35
#define SCM_TIMESTAMPNS         SO_TIMESTAMPNS

#define SO_MARK                 36

#define SO_TIMESTAMPING         37
#define SCM_TIMESTAMPING        SO_TIMESTAMPING

#define SO_PROTOCOL             38
#define SO_DOMAIN               39

#define SO_RXQ_OVFL             40

#define SO_WIFI_STATUS          41
#define SCM_WIFI_STATUS         SO_WIFI_STATUS
#define SO_PEEK_OFF             42

/* Instruct lower device to use last 4-bytes of skb data as FCS */
#define SO_NOFCS                43

/* Below macros are not provided by Huawei LiteOS team, so keeping same values as lwip */
#define  SO_USELOOPBACK         0x0040 /* Unimplemented: bypass hardware when possible */
#define SO_CONTIMEO             0x1009    /* Unimplemented: connect timeout */


/* The following constants should be used for the second parameter of
   `shutdown'.  */
enum
{
  SHUT_RD = 0,      /* No more receptions.  */
#define SHUT_RD     SHUT_RD
  SHUT_WR,      /* No more transmissions.  */
#define SHUT_WR     SHUT_WR
  SHUT_RDWR     /* No more receptions or transmissions.  */
#define SHUT_RDWR   SHUT_RDWR
};

/****************************************************************************
 * Type Definitions
 ****************************************************************************/

 /* sockaddr_storage structure. This structure must be (1) large enough to
  * accommodate all supported protocol-specific address structures, and (2)
  * aligned at an appropriate boundary so that pointers to it can be cast
  * as pointers to protocol-specific address structures and used to access
  * the fields of those structures without alignment problems
  */

#ifdef CONFIG_NET_IPv6
struct sockaddr_storage
{
  sa_family_t ss_family;       /* Address family */
  char        ss_data[18];     /* 18-bytes of address data */
};
#else
struct sockaddr_storage
{
  sa_family_t ss_family;       /* Address family */
  char        ss_data[14];     /* 14-bytes of address data */
};
#endif

/* The sockaddr structure is used to define a socket address which is used
 * in the bind(), connect(), getpeername(), getsockname(), recvfrom(), and
 * sendto() functions.
 */

struct sockaddr
{
  sa_family_t sa_family;       /* Address family: See AF_* definitions */
  char        sa_data[14];     /* 14-bytes of address data */
};

/* Used with the SO_LINGER socket option */

struct linger
{
  int  l_onoff;   /* Indicates whether linger option is enabled. */
  int  l_linger;  /* Linger time, in seconds. */
};

/****************************************************************************
 * Public Function Prototypes
 ****************************************************************************/

/**
* @defgroup struct sockaddr_ll
* @ingroup Socket_Interfaces
* @par Prototype
* @code
*   struct sockaddr_ll {
*     u16_t sll_family;
*     u16_t sll_protocol;
*     u32_t sll_ifindex;
*     u16_t sll_hatype;
*     u8_t  sll_pkttype;
*     u8_t  sll_halen;
*     u8_t  sll_addr[8];
*   };
*
* @endcode
*
* @par Description
* @datastruct sll_family    Socket Family\n
* @datastruct sll_protocol  Ethernet Protocol\n
* @datastruct sll_ifindex   Network Interface Index. Starts from 1\n
* @datastruct sll_hatype    Header type\n
* @datastruct sll_pkttype   Ethernet Packet Type\n
* @datastruct sll_halen     Physical Layer Address Length\n
* @datastruct sll_addr[8]   Physical Layer Address\n
*/

struct sockaddr_ll {
  uint16_t sll_family;   /* Always AF_PACKET */
  uint16_t sll_protocol; /* Physical layer protocol */
  uint32_t sll_ifindex;  /* Interface number. Starts from 1 */
  uint16_t sll_hatype;   /* Header type */
  uint8_t  sll_pkttype;  /* Packet type */
  uint8_t  sll_halen;    /* Length of address */
  uint8_t  sll_addr[8];  /* Physical layer address */
};


#define SOL_PACKET   263  /* pakcet family socket level */
#define  SIOCGIFINDEX  _IOWR('i', 32, struct ifreq) /* get IF index */

int socket(int domain, int type, int protocol);
int bind(int sockfd, const struct sockaddr *addr, socklen_t addrlen);
int connect(int sockfd, const struct sockaddr *addr, socklen_t addrlen);

int listen(int sockfd, int backlog);
int accept(int sockfd, struct sockaddr *addr, socklen_t *addrlen);

ssize_t send(int sockfd, const void *buf, size_t len, int flags);
ssize_t sendto(int sockfd, const void *buf, size_t len, int flags,
               const struct sockaddr *to, socklen_t tolen);

ssize_t recv(int sockfd, void *buf, size_t len, int flags);
ssize_t recvfrom(int sockfd, void *buf, size_t len, int flags,
                 struct sockaddr *from, socklen_t *fromlen);

int setsockopt(int sockfd, int level, int option,
               const void *value, socklen_t value_len);
int getsockopt(int sockfd, int level, int option,
               void *value, socklen_t *value_len);

int getsockname(int sockfd, struct sockaddr *addr,
                socklen_t *addrlen);

int getpeername(int sockfd, struct sockaddr *addr,
                socklen_t *addrlen);

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */
#endif /* __INCLUDE_SYS_SOCKET_H */
