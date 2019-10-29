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

#ifndef __LWIP_SOCKETS_H__
#define __LWIP_SOCKETS_H__

#include "lwip/opt.h"

#if LWIP_SOCKET /* don't build if not configured for use in lwipopts.h */

#include <stddef.h> /* for size_t */

//#include "lwip/ip_addr.h"
//#include "lwip/inet.h"

#ifdef __cplusplus
extern "C" {
#endif

#if !LWIP_LITEOS_COMPAT

 /**
 * @file sockets.h
 */

/**
*  @defgroup  Socket_Interfaces  Socket Interfaces
*  @ingroup User_interfaces
*   This file contains socket interfaces.
*/

/** @ingroup Socket_Interfaces
 * Defines the socket address. */
struct sockaddr_in {
  u16_t sin_family;  /**<   Specifies the socket family. */
  u16_t sin_port;     /**<   Specifies the port. */
  struct in_addr sin_addr;    /**< Specifies the address. */
  char sin_zero[8];      /**<  Indicates an 8-byte padding. */
};



/**
* @ingroup Socket_Interfaces
* @par Prototype
* @code
*   struct sockaddr {
*     u16_t sa_family;
*     char sa_data[14];
*   };
*
* @endcode
*
* @par Description
* @datastruct sa_len        Length of the address \n
* @datastruct sa_family     Socket family \n
* @datastruct sa_data[14]   Address
*/
struct sockaddr {
  u16_t sa_family;
  char sa_data[14];
};

#if PF_PKT_SUPPORT
/**
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
  u16_t sll_family;   /**< Specifies the socket family. Always AF_PACKET */              
  u16_t sll_protocol; /**< Specifies the Ethernet protocol. Physical layer protocol. */       
  u32_t sll_ifindex;  /**< Specifies the network interface index. Starts from 1. */  
  u16_t sll_hatype;   /**<  Specifies the hat type. */                      
  u8_t  sll_pkttype; /**< Specifies the packet type.*/                      
  u8_t  sll_halen;    /**< Specifies the physical layer address length. */                 
  u8_t  sll_addr[8];  /**< Specifies the physical layer address. */           
};
#endif

typedef u32_t socklen_t;

/* Socket protocol types (TCP/UDP/RAW) */
#define SOCK_STREAM     1
#define SOCK_DGRAM      2
#define SOCK_RAW        3

/*
 * Option flags per-socket. These must match the SOF_ flags in ip.h (checked in init.c)
 */
#define  SO_DEBUG       0x0001 /* Unimplemented: turn on debugging info rcrding */
#define  SO_ACCEPTCONN  0x0002 /* socket has had listen() */
#define  SO_REUSEADDR   0x0004 /* Allow local address reuse */
#define  SO_KEEPALIVE   0x0008 /* keep connections alive */
#define  SO_DONTROUTE   0x0010 /* Unimplemented: just use interface addresses */
#define  SO_BROADCAST   0x0020 /* permit to send and to receive brcast messages (see IP_SOF_brcast option) */
#define  SO_USELOOPBACK 0x0040 /* Unimplemented: bypass hardware when possible */
#define  SO_LINGER      0x0080 /* linger on close if data present */
#define  SO_OOBINLINE   0x0100 /* Unimplemented: leave received OOB data in line */
#define  SO_REUSEPORT   0x0200 /* Unimplemented: allow local address & port reuse */


#define SO_DONTLINGER   ((int)(~SO_LINGER))

/*
 * Additional options, not kept in so_options.
 */
#define SO_SNDBUF    0x1001    /* Unimplemented: send buffer size */
#define SO_RCVBUF    0x1002    /* Indicates the receive buffer size */
#define SO_SNDLOWAT  0x1003    /* Unimplemented: send low-water mark */
#define SO_RCVLOWAT  0x1004    /* Unimplemented: receive low-water mark */
#define SO_SNDTIMEO  0x1005    /* Indicates the Send timeout */
#define SO_RCVTIMEO  0x1006    /* Indicates the receive timeout */
#define SO_ERROR     0x1007    /* Gets the error status and clear */
#define SO_TYPE      0x1008    /* Get the socket type */
#define SO_CONTIMEO  0x1009    /* Unimplemented: connect timeout */
#define SO_NO_CHECK  0x100a    /* don't create UDP checksum */

/*
 * Structure used for manipulating linger option.
 */
struct linger {
       int l_onoff;                /* option on/off */
       int l_linger;               /* linger time */
};

/*
 * Level number for (get/set)sockopt() to apply to socket itself.
 */
#define  SOL_SOCKET  1    /* options for socket level */

#define AF_UNSPEC       0
#define AF_INET         2
#define PF_INET         AF_INET
#define PF_UNSPEC       AF_UNSPEC
#if PF_PKT_SUPPORT
#define PF_PACKET 17  /* Packet family.  */
#define SOL_PACKET   263  /* pakcet family socket level */
#endif

#define IPPROTO_IP      0
#define IPPROTO_ICMP  1
#define IPPROTO_TCP     6
#define IPPROTO_UDP     17
#define IPPROTO_UDPLITE 136

/* Flags we can use with send and recv. */
#define MSG_PEEK       0x01    /* Peeks at an incoming message */
#define MSG_WAITALL    0x02    /* Unimplemented: Requests that the function block until the full amount of data requested can be returned */
#define MSG_OOB        0x04    /* Unimplemented: Requests out-of-band data. The significance and semantics of out-of-band data are protocol-specific */
#define MSG_DONTWAIT   0x08    /* Nonblocking i/o for this operation only */
#define MSG_MORE       0x10    /* Sender will send more */


/*
 * Options for level IPPROTO_IP
 */
#ifndef IP_TOS
#define IP_TOS             1
#define IP_TTL             2
#endif

#endif /* !LWIP_LITEOS_COMPAT */

#if !LWIP_LITEOS_COMPAT
#if LWIP_TCP
/*
 * Options for level IPPROTO_TCP
 */
#define TCP_NODELAY    0x01    /* don't delay send to coalesce packets */
#define TCP_KEEPALIVE  0x02    /* send KEEPALIVE Signals when idle for pcb->keep_idle milliseconds */
#define TCP_KEEPIDLE   0x03    /* set pcb->keep_idle  - Same as TCP_KEEPALIVE, but use seconds for get/setsockopt */
#define TCP_KEEPINTVL  0x04    /* set pcb->keep_intvl - Use seconds for get/setsockopt */
#define TCP_KEEPCNT    0x05    /* set pcb->keep_cnt   - Use number of Signals sent for get/setsockopt */
#define TCP_QUEUE_SEQ     0x15    /* Set sequence number of repaired queue. */
#endif /* LWIP_TCP */
#endif /* !LWIP_LITEOS_COMPAT */

#if LWIP_TCP
#define TCP_KEEPALIVE  0x02    /* send KEEPALIVE probes when idle for pcb->keep_idle milliseconds */
#endif /* LWIP_TCP */

/* For socket repair options.  */
enum
{
  TCP_NO_QUEUE,
  TCP_RECV_QUEUE,
  TCP_SEND_QUEUE,
  TCP_QUEUES_NR,
};

#if LWIP_UDP && LWIP_UDPLITE
/*
 * Options for level IPPROTO_UDPLITE
 */
#define UDPLITE_SEND_CSCOV 0x01 /* sender checksum coverage */
#define UDPLITE_RECV_CSCOV 0x02 /* minimal receiver checksum coverage */
#endif /* LWIP_UDP && LWIP_UDPLITE*/

#if !LWIP_LITEOS_COMPAT
#if LWIP_IGMP
/*
 * Options and types for UDP multicast traffic handling
 */
#define IP_ADD_MEMBERSHIP  3
#define IP_DROP_MEMBERSHIP 4
#define IP_MULTICAST_TTL   5
#define IP_MULTICAST_IF    6
#define IP_MULTICAST_LOOP  7

typedef struct ip_mreq {
    struct in_addr imr_multiaddr; /* IP multicast address of group */
    struct in_addr imr_interface; /* local IP_add of interface */
} ip_mreq;
#endif /* LWIP_IGMP */
#if LWIP_RAW
#define        IP_HDRINCL        0x8     /* int; Header is included with data.  */
#endif
#endif /* !LWIP_LITEOS_COMPAT */
/*
 * The Type of Service provides an indication of the abstract
 * parameters of the quality of service desired.  These parameters are
 * to be used to guide the selection of the actual service parameters
 * when transmitting a datagram through a particular network.  Several
 * networks offer service precedence, which somehow treats high
 * precedence traffic as more important than other traffic (generally
 * by accepting only traffic above a certain precedence at time of high
 * load).  The major choice is a three way tradeoff between low-delay,
 * high-reliability, and high-throughput.
 * The use of the Delay, Throughput, and Reliability indications may
 * increase the cost (in some sense) of the service.  In many networks
 * better performance for one of these parameters is coupled with worse
 * performance on another.  Except for very unusual cases at most two
 * of these three indications should be set.
 */
#define IPTOS_TOS_MASK          0x1E
#define IPTOS_TOS(tos)          ((tos) & IPTOS_TOS_MASK)
#define IPTOS_LOWDELAY          0x10
#define IPTOS_THROUGHPUT        0x08
#define IPTOS_RELIABILITY       0x04
#define IPTOS_LOWCOST           0x02
#define IPTOS_MINCOST           IPTOS_LOWCOST

/*
 * The Network Control precedence designation is intended to be used
 * within a network only.  The actual use and control of that
 * designation is up to each network. The Internetwork Control
 * designation is intended for use by gateway control originators only.
 * If the actual use of these precedence designations is of concern to
 * a particular network, it is the responsibility of that network to
 * control the access to, and use of, those precedence designations.
 */
#define IPTOS_PREC_MASK                 0xe0
#define IPTOS_PREC(tos)                ((tos) & IPTOS_PREC_MASK)
#define IPTOS_PREC_NETCONTROL           0xe0
#define IPTOS_PREC_INTERNETCONTROL      0xc0
#define IPTOS_PREC_CRITIC_ECP           0xa0
#define IPTOS_PREC_FLASHOVERRIDE        0x80
#define IPTOS_PREC_FLASH                0x60
#define IPTOS_PREC_IMMEDIATE            0x40
#define IPTOS_PREC_PRIORITY             0x20
#define IPTOS_PREC_ROUTINE              0x00

#if !LWIP_LITEOS_COMPAT
/*
 * Commands for ioctlsocket(),  taken from the BSD file fcntl.h.
 * lwip_ioctl only supports FIONREAD and FIONBIO, for now
 *
 * Ioctl's have the command encoded in the lower word,
 * and the size of any in or out parameters in the upper
 * word.  The high 2 bits of the upper word are used
 * to encode the in/out status of the parameter; for now
 * we restrict parameters to at most 128 bytes.
 */
#if !defined(FIONREAD) || !defined(FIONBIO)
#ifndef IOCPARM_MASK
#define IOCPARM_MASK    0x7fU           /* parameters must be < 128 bytes */
#endif
#ifndef IOC_VOID
#define IOC_VOID        0x20000000UL    /* no parameters */
#endif
#ifndef IOC_OUT
#define IOC_OUT         0x40000000UL    /* copy out parameters */
#endif
#ifndef IOC_IN
#define IOC_IN          0x80000000UL    /* copy in parameters */
#endif
#ifndef IOC_INOUT
#define IOC_INOUT       (IOC_IN|IOC_OUT)
#endif
                                        /* 0x20000000 distinguishes new &
                                           old ioctl's */
#ifndef _IO
#define _IO(x,y)        (IOC_VOID|((x)<<8)|(y))
#endif
#ifndef _IOR
#define _IOR(x,y,t)     (IOC_OUT|(((long)sizeof(t)&IOCPARM_MASK)<<16)|((x)<<8)|(y))
#endif
#ifndef _IOW
#define _IOW(x,y,t)     (IOC_IN|(((long)sizeof(t)&IOCPARM_MASK)<<16)|((x)<<8)|(y))
#endif
#ifndef _IOWR
#define _IOWR(x,y,t)     (IOC_INOUT|(((long)sizeof(t)&IOCPARM_MASK)<<16)|((x)<<8)|(y))
#endif
#endif /* !defined(FIONREAD) || !defined(FIONBIO) */

#ifndef FIONREAD
#define FIONREAD    _IOR('f', 127, unsigned long) /* get # bytes to read */
#endif
#ifndef FIONBIO
#define FIONBIO     _IOW('f', 126, unsigned long) /* set/clear non-blocking i/o */
#endif

/* Socket I/O Controls: unimplemented */
#ifndef SIOCSHIWAT
#define SIOCSHIWAT  _IOW('s',  0, unsigned long)  /* set high watermark */
#define SIOCGHIWAT  _IOR('s',  1, unsigned long)  /* get high watermark */
#define SIOCSLOWAT  _IOW('s',  2, unsigned long)  /* set low watermark */
#define SIOCGLOWAT  _IOR('s',  3, unsigned long)  /* get low watermark */
#define SIOCATMARK  _IOR('s',  7, unsigned long)  /* at oob mark? */
#endif

/* commands for fnctl */
#ifndef F_GETFL
#define F_GETFL 3
#endif
#ifndef F_SETFL
#define F_SETFL 4
#endif

/* File status flags and file access modes for fnctl,
   these are bits in an int. */
#ifndef O_NONBLOCK
#define O_NONBLOCK  4U /* nonblocking I/O */
#endif
#ifndef O_NDELAY
#define O_NDELAY    O_NONBLOCK /* same as O_NONBLOCK, for compatibility */
#endif

#ifndef SHUT_RD
  #define SHUT_RD   0
  #define SHUT_WR   1
  #define SHUT_RDWR 2
#endif

/* FD_SET used for lwip_select */
#ifndef FD_SET
  #undef  FD_SETSIZE
  /* Make FD_SETSIZE match NUM_SOCKETS in socket.c */
  #define FD_SETSIZE    MEMP_NUM_NETCONN
  #define FD_SET(n, p)  ((p)->fd_bits[(n)/8] |=  (1 << ((n) & 7)))
  #define FD_CLR(n, p)  ((p)->fd_bits[(n)/8] &= ~(1 << ((n) & 7)))
  #define FD_ISSET(n,p) ((p)->fd_bits[(n)/8] &   (1 << ((n) & 7)))
  #define FD_ZERO(p)    (void)MEMSET_S((void*)(p),sizeof(*(p)), 0,sizeof(*(p))) /*CSEC_FIX_2302*/

  typedef struct fd_set {
          unsigned char fd_bits [(FD_SETSIZE+7)/8];
        } fd_set;

#endif /* FD_SET */

/** LWIP_TIMEVAL_PRIVATE: if you want to use the struct timeval provided
 * by your system, set this to 0 and include <sys/time.h> in cc.h */
#ifndef LWIP_TIMEVAL_PRIVATE
#define LWIP_TIMEVAL_PRIVATE 1
#endif

#if LWIP_TIMEVAL_PRIVATE
struct timeval {
  long    tv_sec;         /* seconds */
  long    tv_usec;        /* and microseconds */
};
#endif /* LWIP_TIMEVAL_PRIVATE */

#endif /* !LWIP_LITEOS_COMPAT */

#if LWIP_BSD_API
#define lwip_accept         accept
#define lwip_bind             bind
#define lwip_shutdown    shutdown
#define lwip_close           closesocket
#define lwip_connect       connect
#define lwip_getsockname getsockname
#define lwip_getpeername getpeername
#define lwip_setsockopt setsockopt
#define lwip_getsockopt getsockopt
#define lwip_listen          listen
#define lwip_recv            recv
#define lwip_recvfrom    recvfrom
#define lwip_send           send
#define lwip_sendto        sendto
#define lwip_socket        socket
#define lwip_select       select
#endif

#if LWIP_POSIX_SOCKETS_IO_NAMES
#define lwip_read         read
#define lwip_write        write
#undef  lwip_close
#define lwip_close        close
#define lwip_fcntl        fcntl
#define lwip_ioctl        ioctl
#endif /* LWIP_POSIX_SOCKETS_IO_NAMES */


/*
Func Name:  lwip_accept
*/
/**
* @ingroup Socket_Interfaces
* @par Prototype
* @code
* int lwip_accept(int s, struct sockaddr *addr, socklen_t *addrlen);
* @endcode
*
* @par Purpose
*  This API is used to accept a connection on socket.
*
* @par Description
*  This API extracts the first connection request on the queue of pending connections
*  for the listening socket 's', creates a new connected socket, and returns a new
*  file descriptor  referring to that  socket. The newly created socket is not in the
*  the listening state. The original socket 's' is unaffected by this call.
*
* @param[in]     s    Specifies a socket that was created with lwip_socket(), has been bound to an address with lwip_bind(), and has issued a successful call to lwip_listen().
* @param[out]    addr   Either a null pointer, or a pointer to a sockaddr structure where the address of the connecting socket shall be returned.
* @param[out]    addrlen  Either a null pointer, if address is a null pointer, or a pointer to a socklen_t object which on input specifies the length of the supplied sockaddr structure, and on output specifies the length of the stored address.  
*
* @par Return values
*  New socket file descriptor: On success \n
*  -1: On failure \n
*
* @par Errors
*  \li The lwip_accept() function fails if: 
*     - <b>[EAGAIN/EWOULDBLOCK]</b> : \n O_NONBLOCK is set for the socket file descriptor and no connections are present to be accepted.
*     - <b> [EBADF] </b>: \n The socket argument is not a valid file descriptor.
*     - <b> [ECONNABORTED] </b>: \n A connection has been aborted.
*     - <b> [EINVAL] </b>: \n The socket is not accepting connections.
*     - <b> [ENFILE] </b>: \n The maximum number of file descriptors in the system are already open.
*     - <b> [ENOBUFS] </b>: \n No buffer space is available.
*     - <b> [ENOMEM] </b>: \n There was insufficient memory available to complete the operation.
*     - <b> [EOPNOTSUPP] </b>: \n For PF_PKT_RAW and for non TCP sockets it does not support accepting connections.
*     - <b> [ETIMEDOUT] </b>: \n It timeout waiting for a new connection. 
*  \li The lwip_accept() function may fail if: 
*      - <b> [EIO] </b>: \n Internal errors
*      - <b> [ECONNECTED] </b>: \n Connection is closed while resource being allocated, 
* @par POSIX Conformance:
*    @li Implementation deviate from POSIX.1-2008 (IEEE Std 1003.1-2008, 2016 Edition), below are the exceptions to conformance: 
*         - The new socket return by lwip_accept() does inherit socket option such SOF_REUSEADDR,SOF_KEEPALIVE, this behaviour may differ from other implementation. Portable programs should not rely on inheritance or non inheritance of file status flags or socket options, and always explicityly set all required flags or options on the socket returned from the accept().
*         - EIO and ECONNECTED are lwIP specific errno, not defined by POSIX, portable programs should not use these options in applications.
*         - ETIMEDOUT is lwIP and linux specific errno, not defined by POSIX, portable programs should not use these options in applications.
*
* @par Required Header File
* sockets.h
*
* @par Note
*  - This API does not support the PF_PACKET option.
*
* @par Related Topics
* lwip_connect
*/
int lwip_accept(int s, struct sockaddr *addr, socklen_t *addrlen);



/*
Func Name:  lwip_bind
*/
/**
* @ingroup Socket_Interfaces
* @par Prototype
* @code
* int lwip_bind(int s, const struct sockaddr *name, socklen_t namelen);
* @endcode
*
* @par Purpose
*  This API is used to associate a local address or name with a socket.
*
* @par Description
*  This API assigns the address specified by name to the socket referred to
*  by the file descriptor 's'. namelen specifies the size, in bytes, of the address
*  structure pointed to by name.
*
* @param[in]    s        Specifies the file descriptor of the socket to be bound. [N/A]
* @param[in]    name     Points to a sockaddr structure containing the address to be bound to the socket. The length and format of the address depend on the address family of the socket.[N/A]
* @param[in]    namelen  Specifies the length of the sockaddr structure pointed to by the address argument.[N/A]
*
*
* @par Return values
*  0: On success \n
*  -1: On failure \n
*
* @par Errors
*    @li The lwip_bind() function shall fail if: 	
*     - <b> [EADDRINUSE] </b>: \n The specified address is already in use. In UDP, the IP port is already in use or in TCP the IP port is already in use.
*     - <b>  [EBADF] </b> : \n The socket argument is not a valid file descriptor.
*     - <b> [EINVAL] </b> : The socket is already bound to an address, and the protocol does not support binding to a new address; or the socket has been shut down.
*     - <b> [EADDRNOTAVAIL] </b>: \n The specified address is not available from the local machine.
*     - <b> [EAFNOSUPPORT] </b>: \n The specified address is not a valid address for the address family of the specified socket.
*     - <b> [ENOBUFS] </b>: \n Insufficient resources were available to complete the call.
*     - <b> [EOPNOTSUPP] </b>: \n The socket type of the specified socket does not support binding to an address.
*    @li The lwip_bind() function may fail if: \n 
*     - <b> [EACCES] </b>: \n The specified address is protected and the current user does not have permission to bind to it.
*     - <b> [EINVAL] </b>: \n The address_len argument is not a valid length for the address family.
*  @par POSIX Conformance:
*     Confirming to POSIX.1-2008 (IEEE Std 1003.1-2008, 2016 Edition)
*
* @par Required Header File
* sockets.h
*
* @par Note
* - Bind must not be called multiple times.\n
*
*
* @par Related Topics
* \n
* N/A
*/
int lwip_bind(int s, const struct sockaddr *name, socklen_t namelen);



/*
Func Name:  lwip_shutdown
*/
/**
* @ingroup Socket_Interfaces
* @par Prototype
* @code
* int lwip_shutdown(int s, int how);
* @endcode
*
* @par Purpose
*  This is used to shut down socket send and receive operations.
*
* @par Description
*  This API is used to shut down the send and receive operations. lwip_bind() assigns the address specified by name to the socket referred to
*  by the file descriptor 's'. namelen specifies the size, in bytes, of the address
*  structure pointed to by name.
*
* @param[in]    s       Specifies a file descriptor referring to the socket. 
* @param[in]    how     Specifies the type of shut-down. The values are as follows: \n SHUT_RD Disables further receive operations. SHUT_WR: Disables further send operations. SHUT_RDWR: Disables further send and receive operations.
*
* @par Return values
*  0: On success \n
*  -1: On failure \n
*
* @par Errors
*    @li The lwip_shutdown() function shall fail if: 
*     - <b> [EBADF] </b> : \n The socket argument is not a valid file descriptor. 
*     - <b> [EINVAL] </b>: \n The how argument is invalid. 
*     - <b> [ENOTCONN] </b>: \n lwIP does not support half close, ENOTCONN is returned.
*     - <b> [EINPROGRESS] </b>: \n O_NONBLOCK is set for the file descriptor for the socket and the assignment cannot be immediately performed; the assignment shall be performed asynchronously.
*     - <b> [ENOBUFS] </b>:\n Insufficient resources were available in the system to perform the operation. 
*     - <b> [ENOTSOCK] </b>: \n The socket argument does not refer to a socket.
*     - <b> [EOPNOTSUPP] </b>: \n The socket type of the specified socket does not support binding to an address.
*     - <b> [EINVAL] </b>: \n If socket is not connected, lwIP shall fail with EINVAL.
*     - <b> [EOPNOTSUPP] </b>: \n Socket is not TCP, lwIP shall fail. \n
*   @li The lwip_shutdown() function may fail if:  \n
*     - <b> [EACCES] </b>: \n The specified address is protected and the current user does not have permission to bind to it.
*     - <b> [EINPROGRESS] </b>: \n If WRITE/CONNECT/CLOSE is in progress, lwip may fail.
*     - <b> [Others] </b>: \n lwIP may return failure for errors occurred prior to call of lwIP and will return appropriate error code for same.
*
* @par POSIX Conformance: 
* Implementation deviate from POSIX.1-2008 (IEEE Std 1003.1-2008, 2016 Edition), below are exceptions to conformance:
*  - lwIP does not support half shutdown, i.e. with either SHUT_RD or SHUT_WR alone, and if tried returns error ENOTCONN.
*  - lwIP does not return failure with errno ENOTCONN as expected by POSIX,intead set errno to EINVAL.
*
* @par Required Header File
* sockets.h
*
* @par Note
* - Only "SHUT_RDWR" is supported for the "how" parameter in this API. lwIP does not support closing one end of the full-duplex connection.\n
* - This API does not support the PF_PACKET option.
* - The listen socket does not support half shutdown.
* - lwIP does not support half shutdown. Any half shut-down is treated as full connection shut-down.
*
*
* @par Related Topics
* \n
* N/A
*/
int lwip_shutdown(int s, int how);



/*
Func Name:  lwip_getpeername
*/
/**
* @ingroup Socket_Interfaces
* @par Prototype
* @code
* int lwip_getpeername (int s, struct sockaddr *name, socklen_t *namelen);
* @endcode
*
* @par Purpose
*  This API is used to get name of connected peer socket.
*
* @par Description
*  This API returns the address of the peer connected to the socket 's', in the buffer pointed to by name.
*  The namelen argument should be initialized  to  indicate the amount of space pointed to by name.
*  On return it contains the actual size of the name returned (in bytes). The name is truncated if the
*  buffer provided is too small.
*
* @param[in]    s          Specifies the file descriptor referring to the connected socket. 
* @param[out]    name       Indicates the pointer to the sockaddr structure that identifies the connection.
* @param[in, out]    namelen  Specifies the size name structure. 
*
*
* @retval int   On success [0|N/A]
* @retval int   On failure [-1|N/A]
*
* @par Errors
*    @li The lwip_getpeername() function fails if: 
*     - <b> [EBADF] </b>: The socket argument is not a valid file descriptor.
*     - <b> [ENOTCONN] </b>: The socket is not connected or otherwise has not had the peer pre-specified. 
*     - <b> [EOPNOTSUPP] </b>: The operation is not supported for the socket protocol. 
*   @li The lwip_getpeername() function may fail if: 
*     - <b> [EINVAL] </b>: The Name or Namelen are NULL, then EINVAL is returned.
* @par POSIX Conformance: 
* Implementation deviate from POSIX.1-2008 (IEEE Std 1003.1-2008, 2016 Edition), below are the exceptions to conformance: 
* - lwIP does not check if connection-mode socket have been shutdown, and does not return failure with  errno [EINVAL].
*
*
* @par Required Header File
* sockets.h
*
* @par Note
* - This API does not support the PF_PACKET option.\n
*
*
* @par Related Topics
* lwip_getsockname()
*/
int lwip_getpeername (int s, struct sockaddr *name, socklen_t *namelen);



/*
Func Name:  lwip_getsockname
*/
/**
* @ingroup Socket_Interfaces
* @par Prototype
* @code
* int lwip_getsockname (int s, struct sockaddr *name, socklen_t *namelen);
* @endcode
*
* @par Purpose
*  This API is used to get name of socket.
*
* @par Description
*  This API returns the current address to which the socket 's'  is bound, in the buffer pointed to by name.
*  The namelen argument should be initialized to indicate the amount of space(in bytes) pointed to by
*  name.The returned address is truncated if the buffer provided is too small; in this case, namelen will
*  return a value greater than was supplied to the call.
*
* @param[in]   s          Specifies the file descriptor referring to connected socket. 
* @param[out]    name	   Indicates a pointer to sockaddr structure that identifies the connection. 
* @param[in,out]    namelen  Specifies the size name structure. 
*
* @retval int   On success [0|N/A]
* @retval int   On failure [-1|N/A]
*
* @par Errors
* @li The lwip_getsockname() function shall fail if: 
*    - <b> [EBADF] </b>: \n The socket argument is not a valid file descriptor.
*    - <b> [EOPNOTSUPP] </b>: \n  The operation is not supported for this socket's protocol.For PF_RAW sockets, this error is returned.
* @par POSIX conformance: 
*  Implementation deviate from POSIX.1-2008 (IEEE Std 1003.1-2008, 2016 Edition), below are the exceptions to conformance: 
*  - lwIP does not check if connection-mode socket have been shutdown, and does not return failure with  errno [EINVAL].
*
* @par Required Header File
* sockets.h
*
* @par Note
* - This API does not support the PF_PACKET option.
*
* @par Related Topics
* lwip_getpeername()
*/
int lwip_getsockname (int s, struct sockaddr *name, socklen_t *namelen);



/*
Func Name:  lwip_getsockopt
*/
/**
* @ingroup Socket_Interfaces
* @par Prototype
* @code
* int lwip_getsockopt (int s, int level, int optname, void *optval, socklen_t *optlen);
* @endcode
*
* @par Purpose
*  This API is used to get options set on socket.
*
* @par Description
*  This API retrieves the value for the option specified by the optname argument for the socket
*  specified by 's'. If the size of the optval is greater than optlen, the value stored in the object
*  pointed to by the optval argument shall be silently truncated.
*
* @param[in]    s          Specifies a socket file descriptor.  [N/A]
* @param[in]        level	    Specifies the protocol level at which the option resides.  [N/A]
* @param[in]        optname     Specifies a single option to be retrieved.  [N/A]
* @param[out]       optval      Indicates an address to store option value.  [N/A]
* @param[in,out]    optlen      Specifies the size of the option value. [N/A]
*
* @par Return values
*  0: On success \n
*  -1: On failure \n
*
* @par Errors
*   @li The lwip_getsockopt() function fails in the following conditions: 
*     - <b> [EBADF] </b>: \n The socket argument is not a valid file descriptor. 
*     - <b> [EINVAL] </b>: \n The specified option is invalid at the specified socket level.
*     - <b> [EINVAL] </b>: \n If opt or optlen are NULL.
*     - <b> [ENOPROTOOPT] </b>: \n he option is not supported by the protocol.
*     - <b> [EINVAL] </b>: Option or option length is incorrect.  
* @par POSIX Conformance: 
*  Implementation deviate from POSIX.1-2008 (IEEE Std 1003.1-2008, 2016 Edition) with below exceptions:
*  - lwIP does not check if connection-mode socket have been shutdown, and does not return failure with  errno [EINVAL].
*
* @par Required Header File
* sockets.h
*
* @par Note
* 1. Supported protocol levels are: SOL_SOCKET, IPPROTO_IP, IPPROTO_TCP.\n
* 2. Under SOL_SOCKET the options supported are: SO_ACCEPTCONN, SO_brcast,
* SO_ERROR, SO_KEEPALIVE, SO_SNDTIMEO, SO_RCVTIMEO, SO_RCVBUF, SO_REUSEADDR,
* SO_REUSEPORT, SO_TYPE, SO_NO_CHECK. For SO_SNDTIMEO, SO_RCVTIMEO, SO_RCVBUF,
* the macros LWIP_SO_SNDTIMEO, LWIP_SO_RCVTIMEO and LWIP_SO_RCVBUF should have
* been defined at compile time. For SO_REUSEADDR, SO_REUSEPORT, the macro SO_REUSE
* should have been defined at compile time.\n
* 3. Under IPPROTO_IP the options supported are: IP_TTL, IP_TOS.\n
* 4. Under IPPROTO_TCP the options supported are: TCP_NODELAY, TCP_KEEPALIVE,
* TCP_KEEPIDLE, TCP_KEEPINTVL, TCP_KEEPCNT, TCP_QUEUE_SEQ. For TCP_KEEPIDLE,
* TCP_KEEPINTVL, TCP_KEEPCNT, the macro LWIP_TCP_KEEPALIVE should have been
* defined at compile time.\n
* 5. In BSD linux, optval for SO_SNDTIMEO and SO_RCVTIMEO is struct timeval, but in LwIP it is int. \n
*
* @par Related Topics
* lwip_setsockopt()
*/
int lwip_getsockopt (int s, int level, int optname, void *optval, socklen_t *optlen);



/*
Func Name:  lwip_setsockopt
*/
/**
* @ingroup Socket_Interfaces
* @par Prototype
* @code
* int lwip_setsockopt (int s, int level, int optname, const void *optval, socklen_t optlen);
* @endcode
*
* @par Purpose
*  This API is used to set options on socket.
*
* @par Description
*  This API sets the option specified by the optname, at the protocol level specified by the level,
*  to the value pointed to by the optval for the socket associated with the file descriptor specified by 's'.
*
* @param[in]    s             Specifies a socket file descriptor. [N/A]
* @param[in]    level	   Specifies the protocol level at which the option resides. [N/A]
* @param[in]    optname    Specifies a single option to set. [N/A]
* @param[out]    optval      Indicates the address to store the option value.  [N/A]
* @param[in]    optlen      Specifies the size of option value. [N/A]
*
* @par Return values
*  0: On success \n
*  -1: On failure \n
*
* @par Errors
*   @li The lwip_setsockopt() function fails in the following conditions: 
*     - <b> [EBADF] </b>: \n The socket argument is not a valid file descriptor.
*     - <b> [EDOM] </b>: \n The send and receive timeout values are too big to fit into the timeout fields in the socket structure. 
*     - <b> [ENOPROTOOPT] </b>: \n The option is not supported by the protocol. 
*     - <b> [EINVAL]  </b>: If optval is NULL pointer, lwIP shall fail.
*   @li The lwip_setsockopt() function fails in the following conditions: 
*     - <b> [ENOMEM] </b>: \n There was insufficient memory available for the operation to complete. 		
*     - <b> [EINVAL] </b>: \n If for PF_PACKET socket type, level is not SOL_PACKET, or vice versa, lwIP shall fail.
*     - <b> [EINVAL] </b>: \n If optlen does not match optval for corresponding optname, lwIP shall fail.
* @par POSIX Conformance: 
*  Implementation deviate from POSIX.1-2008 (IEEE Std 1003.1-2008, 2016 Edition), below are exceptions to conformance:
*  - if connection oriented socket is shutdown, lwip_setsockopt does not check for this and does not return failure with errno EINVAL as expected by POSIX.
*  - If socket option is INVALID at specified socket level, lwip_setsockopt return failure with errno set to ENOPROTOOPT, which is not in conformance to POSIX.
*  - lwip_setsockopt does not check if connection is already connected for specified options if can not be set while socket is connected.
*
* @par Required Header File
* sockets.h
*
* @par Note
* 1. Supported protocol levels are: SOL_SOCKET, IPPROTO_IP, IPPROTO_TCP\n
* 2. Under SOL_SOCKET the options supported are: SO_brcast, SO_KEEPALIVE,
* SO_SNDTIMEO, SO_RCVTIMEO, SO_RCVBUF, SO_REUSEADDR, SO_REUSEPORT,
* SO_NO_CHECK. For SO_SNDTIMEO, SO_RCVTIMEO, SO_RCVBUF, the macros
* LWIP_SO_SNDTIMEO, LWIP_SO_RCVTIMEO and LWIP_SO_RCVBUF should have been
* defined at compile time. For SO_REUSEADDR, SO_REUSEPORT, the macro SO_REUSE
* should have been defined at compile time.\n
* 3. Under IPPROTO_IP the options supported are: IP_TTL, IP_TOS.\n
* 4. Under IPPROTO_TCP the options supported are: TCP_NODELAY, TCP_KEEPALIVE,
* TCP_KEEPIDLE, TCP_KEEPINTVL, TCP_KEEPCNT. For TCP_KEEPIDLE, TCP_KEEPINTVL,
* TCP_KEEPCNT, the macro LWIP_TCP_KEEPALIVE should have been defined at compile time.\n
* 5. In BSD linux, optval for SO_SNDTIMEO and SO_RCVTIMEO is struct timeval, but in LwIP it is int. \n
*
* @par Related Topics
* lwip_getsockopt()
*/
int lwip_setsockopt (int s, int level, int optname, const void *optval, socklen_t optlen);



/*
Func Name:  lwip_close
*/
/**
* @ingroup Socket_Interfaces
* @par Prototype
* @code
* int lwip_close(int s);
* @endcode
*
* @par Purpose
*  This API is used to close the socket.
*
* @par Description
*  This API closes the socket file descriptor.
*
* @param[in]    s      Specifies a socket file descriptor.  [N/A]
*
* @par Return values
*  0: On success \n
*  -1: On failure \n
* @par Errors
*   @li The lwip_close() function shall fail if: 
*     - <b> [EBADF] </b>: \n The files argument is not a open file descriptor. Invalid file descriptor.
*
* @par POSIX Conformance:
* Implementation deviate from POSIX.1-2008 (IEEE Std 1003.1-2008, 2016 Edition), below are the exceptions to conformance: 
* - does not support SO_LINGER socket option
*
* @par Required Header File
* sockets.h
*
* @par Note
* \n
* N/A
*
* @par Related Topics
* \n
* N/A
*/
int lwip_close(int s);



/*
Func Name:  lwip_connect
*/
/**
* @ingroup Socket_Interfaces
* @par Prototype
* @code
* int lwip_connect(int s, const struct sockaddr *name, socklen_t namelen);
* @endcode
*
* @par Purpose
*  This is used to initiate a connection on the socket.
*
* @par Description
*  This API connects the socket referred to by the file descriptor 's' to the address specified by name.
*
* @param[in]    s             Specifies a socket file descriptor.  [N/A]
* @param[in]    name	   Specifies a pointer to the sockaddr structure which identifies the connection.  [N/A]
* @param[in]    namelen    Specifies a size name structure.[N/A]
*
* @par Return values
*  0: On success \n
*  -1: On failure \n
*
* @par Errors
*    @li The lwip_connect() function shall fail if: 
*     - <b> [EAFNOSUPPORT] </b>: \n The specified address is not a valid address for the address family of the specified socket.
*     - <b> [EBADF] </b>: \n The socket argument is not a valid file descriptor. 
*     - <b> [ECONNREFUSED] </b>: \n The target address was not listening for connections or refused the connection request.
*     - <b> [EINPROGRESS] </b>: \n O_NONBLOCK is set for the file descriptor for the socket and the connection cannot be immediately established; the connection shall be established asynchronously.
*     - <b> [EISCONN] </b>: \n The specified socket is connection-mode and is already connected.
*     - <b> [ENETUNREACH] </b>: \n No route to the network is present.
*     - <b> [ETIMEDOUT] </b>: \n The attempt to connect timed out before a connection was made.
*     - <b> [ENETUNREACH] </b>: \n No route to the network is present. Only for TCP UDP does not validate network reachability.It is non-compliant.
*   @li  The lwip_connect() function may fail if: \n 
*     - <b> [EADDRINUSE] </b>: \n Attempt to establish a connection that uses addresses that are already in use.
*     - <b> [ECONNRESET] </b>: \n Remote host reset the connection request.
*     - <b> [EINVAL] </b>: \n The address_len argument is not a valid length for the address family; or invalid address family in the sockaddr structure.
*     - <b> [ENOBUFS] </b> : \n No buffer space is available.
*     - <b> [EOPNOTSUPP] </b> : \n The socket is listening and cannot be connected.
*     - <b> [ENETUNREACH] </b> : \n The peer is not reachable or any of network interfaces can not reach peer.
* 
*  @par POSIX Conformance:
*  Implementation deviate from POSIX.1-2008 (IEEE Std 1003.1-2008, 2016 Edition), below are the exceptions to conformance: 
*  - For UDP socket, if the sa_family member of address is AF_UNSPEC, the socket's peer address is not reset.
*  - Subsequent calls to connect() for the same socket, before the connection is established,  fails but does not set errno to [EALREADY].
*  - The specified address is not available from the local machine, lwIP does not fail with errno set [EADDRNOTAVAIL]
*  - For UDP sockets, network reachability is not validated, and it does not return failure with errno set to [ENETUNREACH]
*  - lwIP does not check if specified address has a different type than the socket bound to the specified peer address, and return [EPROTOTYPE], instead for any address family apart from AF_INET it return failure and set errno [EINVAL].
*  - lwIP does not set errno [EOPNOTSUPP] if socket is listening and can not be connected.
*
* @par Required Header File
* sockets.h
*
* @par Note
* \n
* N/A
*
* @par Related Topics
* \n
* N/A
*/
int lwip_connect(int s, const struct sockaddr *name, socklen_t namelen);



/*
Func Name:  lwip_listen
*/
/**
* @ingroup Socket_Interfaces
* @par Prototype
* @code
* int lwip_listen(int s, int backlog);
* @endcode
*
* @par Purpose
*  This API is used to set a socket into listen mode.
*
* @par Description
*  This API marks the socket referred to by 's' as a passive socket, that is, as a socket that will be used
*  to accept incoming connection requests using lwip_accept().
*
* @param[in]    s              Specifies a file descriptor that refers to a socket of type SOCK_STREAM.  [N/A]
* @param[in]    backlog	   Defines the maximum length to which the queue of pending connections for sockfd may grow. If a connection request arrives when the queue is full, the client may receive an error with
*      an indication of ECONNREFUSED or, if the underlying protocol supports retransmission, the request may be ignored so that a later reattempt at connection succeeds. [N/A]
*
* @par Return values
*  0: On success \n
*  -1: On failure \n
*
* @par Errors
*   @li The lwip_listen() function shall fail if: 
*     - <b> [EBADF] </b>: \n The socket argument is not a valid file descriptor. 
*     - <b> [EDESTADDRREQ] </b>: \n The socket is not bound to a local address, and the protocol does not support listening on an unbound socket. 
*     - <b> [ECONNREFUSED] </b>: \n The target address was not listening for connections or refused the connection request.
*     - <b> [EINVAL] </b>: \n The socket is already connected. 
*     - <b> [EOPNOTSUPP] </b>: \n The socket protocol does not support listen(). 
*     - <b> [EDESTADDRREQ] </b>: \n If local_port is zero lwip_listen shall return it failure with errno [EDSTADDRREQ].
*   @li The lwip_listen() function may fail if: \n 
*     - <b> [EINVAL] </b>: \n Operation on incorrect socket
*     - <b> [EADDRINUSE] </b>: \n Operation on incorrect socket.
*     - <b> [ECONNRESET] </b>: \n Remote host reset the connection request.
*     - <b> [EINVAL] </b>: \n lwip_listen() fails if called on socket with state which is not acceptable for listen.
*     - <b> [ENOBUFS] </b> : \n Insufficient resources are available in the system to complete the call. 
*     - <b> [ENOTCONN] </b> : \n If socket resources are not allocated, or internal error it may return ENOTCONN
*     - <b> [EMEM] </b> : \n Failed to allocate memory.
*
* @par POSIX Conformance: 
*     POSIX.1-2008 (IEEE Std 1003.1-2008, 2016 Edition) 
*
* @par Required Header File
* sockets.h
*
* @par Note
* - Max value of backlog is 0xFF, and min value of backlog is 0.
* - If backlog value is 0 or less than zero, backlog value of 1 is used.
* - This API does not support the PF_PACKET socket. 
*
* @par Related Topics
* \n
* N/A
*/
int lwip_listen(int s, int backlog);



/*
Func Name:  lwip_recv
*/
/**
* @ingroup Socket_Interfaces
* @par Prototype
* @code
* int lwip_recv(int s, void *mem, size_t len, int flags);
* @endcode
*
* @par Purpose
*  This API is used to recieve a message from connected socket.
*
* @par Description
*  This API can be used to receive messages from a connection-oriented sockets only
*  because it doesnot permits the application to retrieve the source address of received data.
*
* @param[in]    s             Specifies the socket file descriptor. [N/A]
* @param[in]    mem	   Points to a buffer where the message should be stored.  [N/A]
* @param[in]    len	    Specifies the length in bytes of the buffer pointed to by the buffer argument. [N/A]
* @param[out]    flags	  Specifies the type of message reception. Values of this argument are formed by logically OR'ing zero or more of the following values: MSG_PEEK
*                             Peeks at an incoming message. The data is treated as unread and the next recv() or similar function shall still return this data. [N/A]
*
* @par Return values
*  Number of bytes recieved: On success \n
*  -1: On failure \n
*
* @par Errors
*    @li The lwip_recv() function fails if: 
*     - <b> [EAGAIN/EWOULDBLOCK] </b>: \n The socket's file descriptor is marked O_NONBLOCK and no data is waiting to be received and MSG_DONTWAIT flag is set and no data is waiting to be received.
*     - <b> [EBADF] </b>: \n The socket argument is not a valid file descriptor. 
*     - <b> [ECONNRESET] </b>: \n A connection was forcibly closed by a peer. 
*     - <b> [ENOTCONN] </b>: \n A receive is attempted on a connection-mode socket that is not connected. 
*     - <b> [EOPNOTSUPP] </b>: \n The specified flags are not supported for this socket type or protocol.
*     - <b> [ETIMEDOUT]</b>: \n The connection timed out during connection establishment, or due to a transmission timeout on active connection.     
*    @li The lwip_recv() function may fail if: 
*     - <b> [ETIMEDOUT] </b>: \n SO_RECVTIMEO socket option is set, and socket is not marked as O_NONBLOCK, (nonblocking), and non data is received for timeout duration on socket, error is returned with ETIMEDOUT set.
*     -  <b> [EIO]</b>: \n  An I/O error occurred while reading from or writing to the file system.
*     - <b> [ENOBUFS] </b>: \n Insufficient resources were available in the system to perform the operation. 
*     - <b> [ENOMEM] </b>: \n Insufficient memory was available to fulfill the request. 
*     - <b> [EINVAL]</b>: \n Invalid input parameters. If mem, len is null or flags is less than zero, lwip return failure.
*     - <b> [Other errors] </b> : \n If any error happens before call of API, lwIP shall return -1 and with specific errno set.
*     - <b> [ENOTCONN] </b>: \n Socket closed by peer for UDP of RAW.
*
* @par POSIX Conformance: 
*  Implementation deviate from POSIX.1-2008 (IEEE Std 1003.1-2008, 2016 Edition), below are exceptions to conformance:
*  -  For UDP MSG_PEEK is not suported and data is discarded even if it is set.
*  -  MSG_OOB, MSG_WAITALL flags are not supported.
*  -  Return type is int instead of ssize_t
*
* @par Required Header File
* sockets.h
*
* @par Note
* 1. TCP receive buffer is a list, which holds the segement received from peer.
* If application is calling lwip_recv to get the data, then it just tries to get the
* first entry from the list and gives back to application. This doesn't get recursively
* from list to fill the complete user buffer. \n
* 2. LwIP updates this receive buffer list, once it Gets the next expected segment.
* If there is any out of order segment which is next to the received segment, means
* it merges and puts that as one segemnt into receive buffer list.\n
* 3. if the apps's not read the packet form the socket and the recv buffered
* packets up to MAX_MBOX_SIZE, the incoming packet may be discard and the tcp
* connection may rst by the remote.
*
* @par Related Topics
* lwip_read() \n
* lwip_recvfrom()
*/
int lwip_recv(int s, void *mem, size_t len, int flags);



/*
Func Name:  lwip_read
*/
/**
* @ingroup Socket_Interfaces
* @par Prototype
* @code
* int lwip_read(int s, void *mem, size_t len);
* @endcode
*
* @par Purpose
*  This API is used to read bytes from a socket file descriptor.
*
* @par Description
*  This API is used on a connection-oriented socket to receive data. If the received message is larger than
*  the supplied memory area, the excess data is silently discarded.
*
* @param[in]    s             Specifies a socket file descriptor. [N/A]
* @param[out]    mem	    Specifies the buffer to store the received data.    [N/A]
* @param[in]    len	          Specifies the number of bytes of data to receive.  [N/A]
*
* @par Return values
*  Number of bytes recieved: On success \n
*  -1: On failure \n
*
* @par Errors
*    @li The lwip_read() function fails if: 
*     - <b> [EBADF] </b>: The fildes argument is not a valid file descriptor open for reading. 
*     - <b> [EWOULDBLOCK] </b>: The file is a socket, the O_NONBLOCK flag is set for the file descriptor, and the thread would be delayed in the read operation. 
*     - <b> [ECONNRESET] </b> : A read was attempted on a socket and the connection was forcibly closed by its peer.
*     - <b> [ENOTCONN] </b> : A read was attempted on a socket that is not connected.
*     - <b> [ETIMEDOUT] </b>: A read was attempted on a socket and a transmission timeout occurred. 
*    @li The lwip_read() function may fail if: 
*     - <b> [EIO] </b>: A physical I/O error has occurred. 
*     - <b> [ENOBUFS] </b> : Insufficient resources were available in the system to perform the operation. 
*     - <b> [ENOMEM] </b> : Insufficient memory was available to fulfill the request. 
*     - <b> [EINVAL] </b> : lwIP is non-compliant to posix here from given description, does return failure if nbyte is zero and set EINVAL.
*     - <b> [Other Error] </b> : If any error happens before call of API, lwIP shall return -1 and with specific errno set. 
* @par POSIX Conformance: 
*  Implementation deviates from POSIX.1-2008 (IEEE Std 1003.1-2008, 2016 Edition) and does not confirm to below points mentioned in POSIX.1-2008 documentation:
* -  For UDP MSG_PEEK is not supported and data is discarded even if it is set.
* -  MSG_OOB, MSG_WAITALL flags are not supported.
* -  Return type is int instead of ssize_t.
*
* @par Required Header File
* sockets.h
*
* @par Note
* \n
* N/A
*
* @par Related Topics
* lwip_recv() \n
* lwip_recvfrom()
*/
int lwip_read(int s, void *mem, size_t len);



/*
Func Name:  lwip_recvfrom
*/
/**
* @ingroup Socket_Interfaces
* @par Prototype
* @code
* int lwip_recvfrom(int s, void *mem, size_t len, int flags, struct sockaddr *from, socklen_t *fromlen);
* @endcode
*
* @par Purpose
*  This API is used to recieve a message from a connected and non-connected sockets.
*
* @par Description
*  This API can be used to receive messages from a connection-oriented and connectionless sockets
*  because it permits the application to retrieve the source address of received data.
*
* @param[in]    s             Specifies the socket file descriptor. [N/A]
* @param[out]    mem	   Points to the buffer where the message should be stored.   [N/A]
* @param[in]    len	          Specifies the length in bytes of the buffer pointed to by the buffer argument.[N/A]
* @param[in]    flags	   Specifies the type of message reception. Values of this argument are formed by logically OR'ing zero or more of the following values: MSG_PEEK Peeks at an incoming message. The data is treated as unread and the next recvfrom() or similar function shall still return this data. [N/A]
* @param[out]    from       A null pointer, or points to a sockaddr structure in which the sending address is to be stored. The length and format of the address depend on the address family of the socket. [N/A]
* @param[out]    fromlen    Either a null pointer, if address is a null pointer, or a pointer to a socklen_t object which on input specifies the length of the supplied sockaddr structure, and on output specifies the length of the stored address. [N/A]
*
* @par Return values
*  Number of bytes recieved: On success \n
*  -1: On failure \n
*
* @par Required Header File
* sockets.h
*
* @par Note
* 1. if the apps's not read the packet form the socket and the recv buffered
* packets up to MAX_MBOX_SIZE, the incoming packet may be discard by lwIP.
* \n
* N/A
*
* @par Related Topics
* lwip_read
* lwip_recv
*/
int lwip_recvfrom(int s, void *mem, size_t len, int flags,
      struct sockaddr *from, socklen_t *fromlen);



/*
Func Name:  lwip_send
*/
/**
* @ingroup Socket_Interfaces
* @par Prototype
* @code
* int lwip_send(int s, const void *dataptr, size_t size, int flags);
* @endcode
*
* @par Purpose
*  This API is used to send a message to connected socket.
*
* @par Description
*  This API initiate transmission of a message from the specified socket to its peer.
*  This API will send a message only when the socket is connected.
*
* @param[in]    s          Specifies the socket file descriptor.  [N/A]
* @param[in]    dataptr	   Specifies a buffer containing message to send.   [N/A]
* @param[in]    size	   Specifies the length of the message to send. [N/A]
* @param[in]    flags	   Indicates the types of message transmission. [N/A]
*
* @par Return values
*  Number of bytes sent: On success \n
*  -1: On failure \n
*
* @par Errors
*   @li The lwip_send() function fails if: 
*     - <b> [EWOULDBLOCK] </b> : The socket's file descriptor is marked O_NONBLOCK and the requested operation would block.
*     - <b> [EWOULDBLOCK] </b> : For socket not marked with O_NONBLOCK, and socket set with option SP_SENDTIMEO, and elapsed time is more than timeout value, lwip shall fail.
*     - <b> [EBADF] </b> : 	The socket argument is not a valid file descriptor. 
*     - <b> [ECONNRESET] </b> : A connection was forcibly closed by a peer.
*     - <b> [EDESTADDRREQ] </b> : The socket is not connection-mode and does not have its peer address set, and no destination address was specified.
*     - <b> [EMSGSIZE] </b>: If packet is larger than MTU for PF_PACKET/ SOCK_RAW, lwip shall return failure.
*     - <b> [ENOTCONN] </b> : The socket is not connected.
*     - <b> [EOPNOTSUPP] </b>: SOF_BROADCAST flag is set and destination address is broadcast, lwip shall return failure.
*     - <b> [EOPNOTSUPP] </b>: If flag is not supported, error is returned with errno set.
*     - <b> [EPIPE] </b>: The socket is shut down for writing, or the socket is connection-mode and is no longer connected. In the latter case, and if the socket is of type SOCK_STREAM .
*   @li The lwip_send() function may fail if: 
*     - <b> [EIO] </b>: An I/O error occurred while reading from or writing to the file system.
*     - <b> [ENETDOWN] </b>: The local network interface used to reach the destination is down.
*     - <b> [ENETUNREACH] </b> : No route to the network is present.
*     - <b> [ENOBUFS] </b>: Insufficient resources were available in the system to perform the operation.
*     - <b> [EINVAL] </b>: The dest_len argument is not a valid length for the address family.
*     - <b> [ENOMEM] </b>: Insufficient memory was available to fulfill the request.
*     - <b> [NODEVADDR] </b>: For PF_PACKET SOCK_RAW socket, if given network interface index does not exist, lwIP may fail.
*     - <b> [EINPROGRESS] </b>: If connect, close or write is in progress, and write can not be done.
*     - <b> [Other] </b>: lwIP may return failure for errors occurred prior to call of lwIP and will return appropriate error code for same.
*     - <b> [MSG_DONTWAIT] </b>: Ensure non-blocking behaviour.
* @par POSIX Conformance: 
*  Implementation deviate from POSIX.1-2008 (IEEE Std 1003.1-2008, 2016 Edition), below are exceptions to conformance:
*   - MSG_EOR, MSG_OOB, MSG_NOSIGNAL,  are not supported, lwip fails with errnor set to EOPNOTSUPP.
*   -  Return type is int instead of ssize_t
*
*
* @par Required Header File
* sockets.h
*
* @par Note
* UDP connection can send only a maximum data of length 65000. Sending more than that
* will get truncated to 65000. \n
* 
*
* @par Related Topics
* lwip_write()  \n
* lwip_sendto()
*/
int lwip_send(int s, const void *dataptr, size_t size, int flags);



/*
Func Name:  lwip_sendto
*/
/**
* @ingroup Socket_Interfaces
* @par Prototype
* @code
* int lwip_sendto(int s, const void *dataptr, size_t size, int flags, const struct sockaddr *to, socklen_t tolen);
* @endcode
*
* @par Purpose
*  This API is used to send a message to a connected and non-connected sockets.
*
* @par Description
*  This API can be used to receive messages from a connection-oriented and connectionless sockets
*  If the socket is connectionless-mode, the message shall be sent to the address specified by 'to'.
*  If the socket is connection-mode, destination address in 'to' shall be ignored.
*
* @param[in]    s          Specifies a socket file descriptor. [N/A]
* @param[in]    dataptr	   Specifies a buffer containing the message to send. [N/A]
* @param[in]    size	   Specifies the length of the message to send. [N/A]
* @param[in]    flags	   Indicates the types of message transmission. [N/A]
* @param[in]    to         Specifies a pointer to the sockaddr structure that contains the destination address. [N/A]
* @param[in]    tolen      Specifies the size of the to structure.[N/A]
*
* @par Return values
*  Number of bytes sent: On success \n
*  -1: On failure \n
*
* @par Errors
*    @li The lwip_send() function fails if: 
*     - <b> [EWOULDBLOCK] </b> : The socket's file descriptor is marked O_NONBLOCK and the requested operation would block.
*     - <b> [EWOULDBLOCK] </b> : For socket not marked with O_NONBLOCK, and socket set with option SP_SENDTIMEO, and elapsed time is more than timeout value, lwip shall fail.
*     - <b> [EBADF] </b> : 	The socket argument is not a valid file descriptor. 
*     - <b> [ECONNRESET] </b> : A connection was forcibly closed by a peer.
*     - <b> [EDESTADDRREQ] </b> : The socket is not connection-mode and does not have its peer address set, and no destination address was specified.
*     - <b> [EMSGSIZE] </b>: If packet is larger than MTU for SOCK_RAW, lwip shall return failure.
*     - <b> [ENOTCONN] </b> : The socket is not connected.
*     - <b> [EOPNOTSUPP] </b>: SOF_BROADCAST flag is set and destination address is broadcast, lwIP shall return failure.
*     - <b> [EOPNOTSUPP] </b>: If flag is not supported, error is returned with errno set.
*     - <b> [EPIPE] </b>: The socket is shut down for writing, or the socket is connection-mode and is no longer connected. In the latter case, and if the socket is of type SOCK_STREAM .
*   @li The lwip_send() function may fail if: 
*     - <b> [EIO] </b>: An I/O error occurred while reading from or writing to the file system.
*     - <b> [ENETDOWN] </b>: The local network interface used to reach the destination is down.
*     - <b> [ENETUNREACH] </b> : No route to the network is present.
*     - <b> [ENOBUFS] </b>: Insufficient resources were available in the system to perform the operation.
*     - <b> [EINVAL] </b>: The dest_len argument is not a valid length for the address family.
*     - <b> [ENOMEM] </b>: Insufficient memory was available to fulfill the request.
*     - <b> [NODEVADDR] </b>: For PF_PACKET SOCK_RAW socket, if given network interface index does not exist, lwIP may fail.
*     - <b> [EINPROGRESS] </b>: If connect, close or write is in progress, and write can not be done.
*     - <b> [Other] </b>: lwIP may return failure for errors occurred prior to call of lwIP and will return appropriate error code for same.
*     - <b> [MSG_DONTWAIT] </b>: Ensure non-blocking behaviour.
*     - <b> [MSG_MORE] </b>: Sender will send more
* @par POSIX Conformance: 
*  Implementation deviate from POSIX.1-2008 (IEEE Std 1003.1-2008, 2016 Edition), below are exceptions to conformance:
*  - For UDP/RAW socket, lwip_sendto() override peer address set in connect by address provided in sendto, and does not return failure with EISCONN.
*  - TCP ignore destination address provided in lwip_sendto(), and use connection set up by lwip_connect()
*  - MSG_EOR, MSG_OOB, MSG_NOSIGNAL,  are not suported, lwip fails with errnor set to EOPNOTSUPP.
*  -  return type is int instead of ssize_t
*
* @par Required Header File
* sockets.h
*
* @par Note
* UDP connection can send only a maximum data of length 65000. Sending more than that
* will get truncated to 65000. \n
*
* @par Related Topics
* lwip_write() \n
* lwip_send()
*/
int lwip_sendto(int s, const void *dataptr, size_t size, int flags,
    const struct sockaddr *to, socklen_t tolen);



/*
Func Name:  lwip_socket
*/
/**
* @ingroup Socket_Interfaces
* @par Prototype
* @code
* int lwip_socket(int domain, int type, int protocol);
* @endcode
*
* @par Purpose
*  This API is used to allocate a socket .
*
* @par Description
*  This API is used to create an endpoint for communication and returns a file descriptor.
*
* @param[in]    domain    Specifies a protocol family. [N/A]
* @param[in]    type	   Specifies the socket type.  [SOCK_RAW|SOCK_DGRAM,SOCK_STREAM] [N/A]
* @param[in]    protocol   Specifies the protocol to be used with the socket. [N/A]
*
* @par Return values
* Valid socket file descriptor: On success \n
*  -1: On failure \n
*
* @par Errors
*    @li The lwip_socket() function fails if: 
*     - <b>[EPROTONOSUPPORT] </b>: \n The file is neither a pipe, nor a FIFO or a socket, the O_NONBLOCK flag is set for the file descriptor, and the thread would be delayed in the read operation. 
*     - <b> [ENFILE] </b>: \n  The fd argument is not a valid file descriptor open for reading. 
*     -  <b> [EINVAL] </b> :  \n If type is not SOCK_DGRAM, SOCK_STEAM, SOCK_RAW, lwip shall fail.
*    @li The lwip_socket() function may fail if: \n
*    -  <b> [ENOBUFS] </b>:  Insufficient resources were available in the system to perform the operation.  \n
* @par POSIX Conformance: 
*  Implementation deviate from POSIX.1-2008 (IEEE Std 1003.1-2008, 2016 Edition), below are exceptions to conformance:
*  - lwip_socket does not validate domain, it assumes it to be AF_INET(PF_INET) if not PF_PACKET, as per POSIX it should return failure with errno [EAFNOSUPPORT] but not.
*  - For socket type not recognised by lwip_socket it returns failure with errno EINVAL instead of EPROTOTYPE.
*
* @par Required Header File
* sockets.h
*
* @par Note
*   - PF_PACKET is supported only for SOCK_RAW.
*
* @par Related Topics
* \n
* N/A
*/
int lwip_socket(int domain, int type, int protocol);



/*
Func Name:  lwip_write
*/
/**
* @ingroup Socket_Interfaces
* @par Prototype
* @code
* int lwip_write(int s, const void *dataptr, size_t size);
* @endcode
*
* @par Purpose
*  This API is used to write data bytes to a socket file descriptor.
*
* @par Description
*  This API is used on a connection-oriented sockets to send data.
*
* @param[in]    s           Specifies a socket file descriptor. [N/A]
* @param[in]    dataptr	   Specifies a buffer containing the message to send. [N/A]
* @param[in]    size	   Specifies the length of the message to send. [N/A]
*
* @par Return values
*  Number of bytes sent: On success \n
*  -1: On failure \n
*
* @par Errors
*    @li The lwip_write() function shall fail if: 
*     - <b>[EAGAIN] </b>: \n The file is a socket, the O_NONBLOCK flag is set for the file descriptor, and the thread would be delayed in the write() operation.
*     - <b>[EBADF] </b>: \n The fildes argument is not a valid file descriptor open for writing. 
*     - <b>[ECONNRESET] </b> : \n A write was attempted on a socket that is not connected.  
*     - <b>[EPIPE] </b>: \n A write was attempted on a socket that is shut down for writing, or is no longer connected. In the latter case, if the socket is of type SOCK_STREAM, a SIGPIPE signal shall also be sent to the thread. 
*     - <b>[EMSGSIZE] </b>: \n The message is too large to be sent all at once, as the socket requires . If length is larger than 65000, lwip shall fail.
*     - <b>[EMSGSIZE] </b>: \n If packet is larger than MTU for PF_PACKET/ SOCK_RAW, lwip shall return failure.
*     - <b>[EOPNOTSUPP] </b>: \n SOF_BROADCAST flag is set and destination address is broadcast, lwip shall return failure.
*     - <b>[EDESTADDRREQ] </b>: \n The socket is not connection-mode and no peer address is set.Connect is not called for sockets which are not connection-mode.
*    @li The lwip_write() function may fail if:     
*     - <b>[EIO] </b>: \n A physical I/O error has occurred. 
*     - <b>[ENOBUFS] </b>: \n Insufficient resources were available in the system to perform the operation.
*     - <b>[ENETDOWN] </b>: \n The local network interface used to reach the destination is down.Network interface is down for PF_PACKET SOCK_RAW socket.
*     - <b>[ENETUNREACH] </b> : \n A write was attempted on a socket and no route to the network is present. 
*     - <b>[ENETUNREACH] </b>: \n No route to the network is present.
*     - <b>[ENOBUFS] </b>: \n Insufficient resources were available in the system to perform the operation.
*     - <b>[EINPROGRESS] </b>:  \n If connect, close or write is in progress, and write can not be done.
*     - <b>[Other] </b>: \n lwIP may return failure for errors occurred prior to call of lwIP and will return appropriate error code for same.
*     - <b>[ENOMEM] </b>: Insufficient memory was available to fulfill the request.
* @par POSIX Conformance: 
* Implementation deviate from POSIX.1-2008 (IEEE Std 1003.1-2008, 2016 Edition), below are exceptions to conformance:
* - Return type is int instead of ssize_t 
* - lwIP set EWOULDBLOCK instead of EAGAIN, wherever EAGAIN is set to errno.
*
* @par Required Header File
* sockets.h
*
* @par Note
* - For socket not marked with O_NONBLOCK, and socket set with option SP_SENDTIMEO, and elapsed time is more than timeout value, lwip shall fail with errno EAGAIN.
*
* @par Related Topics
* lwip_send() \n
* lwip_sendto()
*/
int lwip_write(int s, const void *dataptr, size_t size);



/*
Func Name:  lwip_select
*/
/**
* @ingroup Socket_Interfaces
* @par Prototype
* @code
* int lwip_select(int maxfdp1, fd_set *readset, fd_set *writeset, fd_set *exceptset, struct timeval *timeout);
* @endcode
*
* @par Purpose
*  This API is used to keeep tabs on multiple file descriptors, waiting until one or more of the file descriptors
*  become "ready" for some of I/O operations.
*
* @par Description
*  This API is  used to examine  the file descriptor sets whose addresses are passed in the readset, writeset, and exceptset
*  parameters to see whether some of their descriptors are ready for reading, are ready for writing, or have an exceptional
*  condition pending, respectively.
*
* @param[in]    maxfdp1    Specifies a range of file descriptors [N/A]
* @param[in,out]    readset	      Specifies a pointer to struct fd_set, and specifies the descriptor to check for being ready to read.  [N/A]
* @param[in,out]    writeset	    Specifies a pointer to struct fd_set, and specifies the descriptor to check for being ready to write.  [N/A]
* @param[in,out]    exceptset    Specifies a pointer to struct fd_set, and specifies the descriptor to check for pending error conditions.  [N/A]
* @param[in]    timeout       Specifies a pointer to struct timeval, for timeout application. [N/A]
*
* @par Return values
*  Socket file descriptor: On success \n
*  -1: On failure \n
*
* @par POSIX Conformance: 
*  Implementation deviate from POSIX.1-2008 (IEEE Std 1003.1-2008, 2016 Edition), below are exceptions to conformance:
*   -  lwip_select does not validate if nfds argument, does not check if argument is less than zero or greater than FD_SETSIZE. ** To be fixed new version
*   - As per POSIX, Implementations may place limitations on the maximum timeout interval supported. All implementations shall support a maximum timeout interval of at least 31 days, but  lwIP limits value until any overflow happens,  timer with very high value might result in unspecified behavior.

* @par Errors
* The lwip_select() function shall fail if: 
*     - <b>[EBADF] </b>: \n
*             One or more of the file descriptor sets specified a file descriptor that is not a valid open file descriptor. 
*     - <b>[EINVAL] </b>: \n
*              An invalid timeout interval was specified. \n		  
* The lwip_select() function may fail if:  \n
*     - <b> [ENOMEM] </b>: \n
*             Insufficient resources or memory, memory allocation failed.
*
* @par Required Header File
* sockets.h
*
* @par Note
*- lwip_select() does not update the timeout argument to indicate how much time was left.
*- FD_SETSIZE is compile time configurable in lwIP, and application must ensure it does not violate this boundary, lwIP does not validate this at runtime. **to be fixed in next version.
*
* @par Multithreaded Application 
*      If a file descriptor being monitored by select() is closed in another
*      thread, the result is unspecified. lwIP may return without setting any fdset.
*      On some UNIX systems, select()  unblocks and returns, with an indication that the file descriptor is
*      ready (a subsequent I/O operation will likely fail with an error,
*      unless another the file descriptor reopened between the time select()
*      returned and the I/O operations was performed).  On Linux (and some
*      other systems), closing the file descriptor in another thread has no
*      effect on select().  In summary, any application that relies on a
*      particular behaviour in this scenario must be considered buggy.
*
* @par Related Topics
* \n
* N/A
*/
int lwip_select(int maxfdp1, fd_set *readset, fd_set *writeset, fd_set *exceptset,
                struct timeval *timeout);



/*
Func Name:  lwip_ioctl
*/
/**
* @ingroup Socket_Interfaces
* @par Prototype
* @code
* int lwip_ioctl(int s, long cmd, void *argp);
* @endcode
*
* @par Purpose
*  This API is used as a control device.
*
* @par Description
*  This API is used as a control device.
*
* @param[in]    s        Specifies an open socket file descriptor. [N/A]
* @param[in]    cmd    Specifies a device-dependent request code.  [FIONREAD|FIONBIO|N/A]
* @param[in]    argp   Specifies additional information, if required.  [N/A]
*
*
* @par Return values
*  0: On success \n
*  -1: On failure \n
*
*
* @par Errors
*    @li The lwip_ioctl() function shall fail if: 
*     - <b> [EBADF] </b> : The fd argument is not a valid open file descriptor. 
*     - <b> [EINVAL] </b> : The request or arg argument is not valid for this device.
*     - <b> [ENOMEM] </b> : If any resource allocation fails, ENOMEM is set as errno.
*    @li The lwip_ioctl() function may fail if: 
*     - <b> [EAFNOSUPPORT] </b>: If socket created with PF_PACKET, SOCK_RAW is called with SIOCADDRT.
*
* @par Required Header File
* sockets.h
*
* @par Note
* 1. Linux API supports variable argument support. But LwIP API supports only one void * as
* 3rd argument. \n
* 2. Options supported by this api are :
* SIOCADDRT: set IF gateway, soft-route is not support by lwIP yet.\n
* SIOCGIFADDR: get ifnet address.\n
* SIOCGIFFLAGS: get ifnet flags.\n
* SIOCSIFFLAGS: set ifnet flags.\n
*   IFF_UP interface is up.\n
*   IFF_brcast brcast address valid.\n
*   IFF_LOOPBACK is a loopback net.\n
*   IFF_POINTOPOINT is a point-to-point link.\n
*   IFF_DRV_RUNNING resources allocated.\n
*   IFF_NOARP no address resolution protocol.\n
*   IFF_MULTICAST supports multicast.\n
*   IFF_DYNAMIC dialup device with changing addresses.\n
*   IFF_DYNAMIC_S dialup device with changing addresses.\n
* SIOCGIFADDR: get ifnet address.\n
* SIOCSIFADDR: set ifnet address.\n
* SIOCGIFNETMASK: get net addr mask.\n
* SIOCSIFNETMASK : set net addr mask.\n
* SIOCSIFHWADDR: set IF mac_address.\n
* SIOCGIFHWADDR: get IF mac_address\n
* SIOCGIFNAME: set IF name.\n
* SIOCSIFNAME: set IF name.\n
* FIONBIO: set/clear non-blocking i/o.\n
* 3. For FIONREAD option, argp should point to an application variable of type signed int. \n
* 4. For PF_PACKET sockets SIOCADDRT option is not supported. The option supported are 
* FIONBIO,
* SIOCGIFADDR,
* SIOCSIFADDR,
* SIOCGIFNETMASK, 
* SIOCSIFNETMASK,
* SIOCSIFHWADDR, 
* SIOCGIFHWADDR, 
* SIOCGIFFLAGS, 
* SIOCSIFFLAGS, 
* SIOCGIFNAME, 
* SIOCSIFNAME, 
* SIOCGIFINDEX
*
* @par Related Topics
* \n
* N/A
*/
int lwip_ioctl(int s, long cmd, void *argp);



/*
Func Name:  lwip_fcntl
*/
/**
* @ingroup Socket_Interfaces
* @par Prototype
* @code
* int lwip_fcntl(int s, int cmd, int val);
* @endcode
*
* @par Purpose
*  This API is used to manipulate file descriptor.
*
* @par Description
*  This API is used to manipulate file descriptor.
*
* @param[in]    s        Indicates the socket file descriptor. [N/A]
* @param[in]    cmd   Indicates a command to select an operation [F_GETFL|F_SETFL|N/A]
* @param[in]    val     Indicates an additional flag, to set non-blocking. [N/A]
*
*
* @par Return values
*  Postitive value: On success \n
*  -1: On failure \n
*
* @par Errors
*    @li The fcntl() function shall fail if: 
*     - <b> [EBADF] </b> : \n The field argument is not a valid open file descriptor, o. 
*     - <b> [EINVAL] </b> : \n The cmd argument is invalid, or cmd not supported by implementation. Only F_GETFL and F_SETFL are supported by lwIP.
* @par POSIX Conformance: 
*  Implementation deviate from POSIX.1-2008 (IEEE Std 1003.1-2008, 2016 Edition), below are the exceptions to conformance: 
*   - Function prototype does not support variable arguments like POSIX or linux fcntl API.  
*   - The arg values to F_GETFL, and F_SETFL all represent flag values to allow for future growth. Applications using these functions should do a read-modify-write operation on them, rather than assuming that only the values defined by POSIX.1-2008 are valid. It is a common error to forget this, particularly in the case of F_SETFD. Some implementations set additional file status flags to advise the application of default behavior, even though the application did not request these flags.
*
* @par Required Header File
* sockets.h
*
* @par Note
* 1.  Function prototype does not support variable arguments like linux fcntl API.\n
* 2. Only F_GETFL & F_SETFL commands are supported. For F_SETFL, only O_NONBLOCK is supported for val. \n
* 3. PF_PACKET sockets supports the F_SETFL and F_GETFL option. 
*
* @par Related Topics
* \n
* N/A
*/
int lwip_fcntl(int s, int cmd, int val);

/* internal function,
   Call this function to intialise global socket resources
*/
int sock_init(void);

/*
Func Name:  lwip_get_conn_info
*/
/**
* @ingroup Socket_Interfaces
* @par Prototype
* @code
* int lwip_get_conn_info (int s, struct tcpip_conn * conninfo);
* @endcode
*
* @par Purpose
*  This API is used to get tcp or udp connection information.
*
* @par Description
*   This API is used to get tcp or udp connection information.
*
* @param[in]    s        socket file descriptor [N/A]
* @param[out]  conninfo  Connection information  details of given socket
*
* @par Return values
*    0: On success \n
*    Negative value: On failure \n
*
* @par Required Header File
* sockets.h
*
* @par Note
* 1. This function called to get tcp or udp  connection information.
* 2. The void pointer is of type tcpip_conn.
*
* @par Related Topics
* \n
* N/A
*/
int lwip_get_conn_info (int s, void *conninfo);

#if !(LWIP_BSD_API)
#if LWIP_COMPAT_SOCKETS
#define accept(a,b,c)         lwip_accept(a,b,c)
#define bind(a,b,c)           lwip_bind(a,b,c)
#define shutdown(a,b)         lwip_shutdown(a,b)
#define closesocket(s)        lwip_close(s)
#define connect(a,b,c)        lwip_connect(a,b,c)
#define getsockname(a,b,c)    lwip_getsockname(a,b,c)
#define getpeername(a,b,c)    lwip_getpeername(a,b,c)
#define setsockopt(a,b,c,d,e) lwip_setsockopt(a,b,c,d,e)
#define getsockopt(a,b,c,d,e) lwip_getsockopt(a,b,c,d,e)
#define listen(a,b)           lwip_listen(a,b)
#define recv(a,b,c,d)         lwip_recv(a,b,c,d)
#define recvfrom(a,b,c,d,e,f) lwip_recvfrom(a,b,c,d,e,f)
#define send(a,b,c,d)         lwip_send(a,b,c,d)
#define sendto(a,b,c,d,e,f)   lwip_sendto(a,b,c,d,e,f)
#define socket(a,b,c)         lwip_socket(a,b,c)
#define select(a,b,c,d,e)     lwip_select(a,b,c,d,e)
#define ioctlsocket(a,b,c)    lwip_ioctl(a,b,c)

#if LWIP_POSIX_SOCKETS_IO_NAMES
#define read(a,b,c)           lwip_read(a,b,c)
#define write(a,b,c)          lwip_write(a,b,c)
#define close(s)              lwip_close(s)
#define fcntl(a,b,c)          lwip_fcntl(a,b,c)
#endif /* LWIP_POSIX_SOCKETS_IO_NAMES */

void sock_get_errno(int sk,unsigned int *err);

#endif /* LWIP_COMPAT_SOCKETS */
#endif

#ifdef __cplusplus
}
#endif

#endif /* LWIP_SOCKET */

#endif /* __LWIP_SOCKETS_H__ */
