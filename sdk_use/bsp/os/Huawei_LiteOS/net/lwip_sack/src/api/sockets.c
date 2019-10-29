/**
 * @file
 * Sockets BSD-Like API module
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
 * Improved by Marc Boucher <marc@mbsi.ca> and David Haas <dhaas@alum.rpi.edu>
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

#if LWIP_SOCKET /* don't build if not configured for use in lwipopts.h */
#include "netif/ifaddrs.h"

#include "lwip/sockets.h"
#include "lwip/api.h"
#include "lwip/sys.h"
#include "lwip/igmp.h"
#include "lwip/inet.h"
#include "lwip/tcp.h"
#include "lwip/raw.h"
#include "lwip/udp.h"
#include "lwip/tcpip.h"
#include "lwip/pbuf.h"
#include "lwip/netif.h"
#include "lwip/ip_addr.h"
#include "lwip/dhcp.h"
#if LWIP_CHECKSUM_ON_COPY
#include "lwip/inet_chksum.h"
#endif

#include <string.h>
#include "securec.h"

#define NUM_SOCKETS LWIP_NUM_SOCKETS


#define LWIP_SOCKET_INACTIVE 0
#define LWIP_SOCKET_ACTIVE 1
#define LWIP_SOCKET_CLOSING 2

/** Contains all internal pointers and states used for a socket */
struct lwip_sock {
  /** sockets currently are built on netconns, each socket has one netconn */
  struct netconn *conn;
  /** data that was left from the previous read */
  void *lastdata;
  /** offset in the data that was left from the previous read */
  u16_t lastoffset;
  /** number of times data was received, set by event_callback(),
      tested by the receive and select functions */
  s16_t rcvevent;
  /** number of times data was ACKed (free send buffer), set by event_callback(),
      tested by select */
  u16_t sendevent;
  /** error happened for this socket, set by event_callback(), tested by select */
  u16_t errevent;
  /** last error that occurred on this socket */
  int err;

  /** close semaphore used to signal all events are processed, and close the socket */
  sys_sem_t close_sem;
  /** events in process counter */
  int events;
  /** socket level mutex used for multithread support */
  sys_mutex_t mutex;
  sys_mutex_t mutex_op;
  /** counter of how many threads are waiting for this socket using select */
  int select_waiting;
  /* Flag is used for marking sock close state */
  char sock_flag;
};

/** Description for a task waiting in select */
struct lwip_select_cb {
  /** Pointer to the next waiting task */
  struct lwip_select_cb *next;
  /** Pointer to the previous waiting task */
  struct lwip_select_cb *prev;
  /** readset passed to select */
  fd_set *readset;
  /** writeset passed to select */
  fd_set *writeset;
  /** unimplemented: exceptset passed to select */
  fd_set *exceptset;
  /** don't signal the same semaphore twice: set to 1 when signalled */
  int sem_signalled;
  /** semaphore to wake up a task waiting for select */
  sys_sem_t sem;
};

/** This struct is used to pass data to the set/getsockopt_internal
 * functions running in tcpip_thread context (only a void* is allowed) */
struct lwip_setgetsockopt_data {
  /** socket struct for which to change options */
  struct lwip_sock *sock;
#ifdef LWIP_DEBUG
  /** socket index for which to change options */
  int s;
#endif /* LWIP_DEBUG */
  /** level of the option to process */
  int level;
  /** name of the option to process */
  int optname;
  /** set: value to set the option to
    * get: value of the option is stored here */
  void *optval;
  /** size of *optval */
  socklen_t *optlen;
  /** if an error occures, it is temporarily stored here */
  err_t err;
};

/** This struct is used to pass data to the set/getsockopt_internal
 * functions running in tcpip_thread context (only a void* is allowed) */
struct lwip_ioctl_data {
  /** socket index for which to change options */
  int s;
  /** level of the option to process */
  unsigned long cmd;
  /** arg of cmd */
  void *arg;
  /** if an error occures, it is temporarily stored here */
  err_t err;
  /** sem that is used to synchroneously execute functions in the core context */
  sys_sem_t cb_completed;
};


/** The global array of available sockets */
static struct lwip_sock sockets[NUM_SOCKETS];
/** The global list of tasks waiting for select */
static struct lwip_select_cb *select_cb_list;
/** This counter is increased from lwip_select when the list is chagned
    and checked in event_callback to see if it has changed. */
static volatile int select_cb_ctr = 0;

#ifdef ERRNO
/** Table to quickly map an lwIP error (err_t) to a socket error
  * by using -err as an index */
static const int err_to_errno_table[] = {
  0,             /* ERR_OK          0      No error, everything OK. */
  ENOMEM,        /* ERR_MEM        -1      Out of memory error.     */
  ENOBUFS,       /* ERR_BUF        -2      Buffer error.            */
  EWOULDBLOCK,   /* ERR_TIMEOUT    -3      Timeout                  */
  EHOSTUNREACH,  /* ERR_RTE        -4      Routing problem.         */
  EINPROGRESS,   /* ERR_INPROGRESS -5      Operation in progress    */
  EINVAL,        /* ERR_VAL        -6      Illegal value.           */
  EWOULDBLOCK,   /* ERR_WOULDBLOCK -7      Operation would block.   */
  EADDRINUSE,    /* ERR_USE        -8      Address in use.          */
  EISCONN,       /* ERR_ISCONN     -9      Already connected.       */
  EMSGSIZE,      /* ERR_MSGSIZE    -10     Message too long         */
  ENODEV,        /* ERR_NODEV      -11     No such device           */
  ENXIO,         /* ERR_NODEVADDR  -12     No such device or address*/
  EADDRNOTAVAIL, /* ERR_NOADDR	  -13	  Cannot assign requested address */
  EOPNOTSUPP,	  /* ERR_OPNOTSUPP	-14 	Operation not supported on transport endpoint */
  ENOPROTOOPT,   /* ERR_NOPROTOOPT -15 	Protocol not available */
  ECONNABORTED,	/* ERR_ABRT 	  -16	  Connection aborted.	   */
  ECONNRESET,    /* ERR_RST		 -17	 Connection reset.		  */
  ENOTCONN,	  /* ERR_CLSD		-18 	Connection closed.		 */
  ENOTCONN, 	 /* ERR_CONN	   -19	   Not connected.			*/
  EIO,			/* ERR_ARG		  -20	  Illegal argument. 	   */
  -1, 		   /* ERR_IF		 -21	 Low-level netif error	  */
  EDESTADDRREQ,  /* ERR_NODEST 	-22 	Destination address required */
  ENETDOWN       /* ERR_NETDOWN    -23     Network is down          */
};

#define ERR_TO_ERRNO_TABLE_SIZE \
  (sizeof(err_to_errno_table)/sizeof(err_to_errno_table[0]))

/* PC Lint warn fix: Constant value Boolean*/
#define err_to_errno(err) \
  /*lint -e506*/((u8_t)(-(err)) < ERR_TO_ERRNO_TABLE_SIZE ? \
    err_to_errno_table[-(err)] : EIO) /*lint +e506*/

#ifndef set_errno
/* Pclint suppress - Expression-like macro 'set_errno' not parenthesized */
/*lint -e773*/
#define set_errno(err) errno = (err)
/*lint +e773*/
#endif

/* Pclint suppress - Expression-like macro 'get_errno' not parenthesized */
/*lint -e773*/
#define get_errno(err) (err) = errno
/*lint +e773*/

#else
#define err_to_errno(err) (err)
#define set_errno(err)
#define get_errno(err)
#endif /* ERRNO */

#define sock_set_errno_unsafe(sk, e) do { \
  sk->err = (e); \
} while (0)

/* Note: errno must not be updated under mutex lock, as locks might update
   errno value again
*/
static inline void lwip_sock_lock(struct lwip_sock *sock);
static inline void lwip_sock_unlock(struct lwip_sock *sock);

void sock_set_errno(struct lwip_sock *sk, int e){
    lwip_sock_lock(sk);
    sk->err = (e);
    lwip_sock_unlock(sk);
    set_errno(e);
}


#define VALIDATE_DOMAIN_TYPE(_domain,_type)  \
                    if(((_domain) == PF_PACKET) && ((_type) != SOCK_RAW))

#define VALIDATE_NETCONN_TYPE_IS_RAW(_sock)  \
            if(netconn_type((_sock->conn)) == NETCONN_PKT_RAW)

#define LWIP_SOCK_INCREMENT_EVENTS(sock)                               \
                    do                                                                                  \
                    {                                                                                     \
                         lwip_sock_lock(sock);                                               \
                         (sock)->events++;                                                       \
                         lwip_sock_unlock(sock);                                            \
                     }while(0)

void LWIP_SOCK_DECREMENT_EVENTS(struct lwip_sock *sock)
{
    lwip_sock_lock(sock);
    if(0 == --((sock)->events) && LWIP_SOCKET_CLOSING == (sock)->sock_flag)
    {
       sys_sem_signal(&(sock)->close_sem);
    }
    lwip_sock_unlock((sock));
}

#define LWIP_SOCKET_GET_SELECT_SIGNAL_EVENT(_sock,_scb,_s,_do_signal) \
        if ((_sock)->rcvevent > 0) {                \
           if ((_scb)->readset && FD_ISSET((_s), (_scb)->readset)) {            \
              _do_signal = 1;                               \
           }                                        \
        }\
        if ((_sock)->sendevent != 0) {                          \
           if (!_do_signal && (_scb)->writeset && FD_ISSET((_s), (_scb)->writeset)) {   \
              _do_signal = 1;                               \
           }                                        \
        }                                       \
        if ((_sock)->errevent != 0) {                           \
           if (!_do_signal && (_scb)->exceptset && FD_ISSET((_s), (_scb)->exceptset)) {         \
              _do_signal = 1;                                   \
           }                                                \
        }

#define LWIP_UPDATE_EVENT_COUNT(_evt, _sock)         \
        switch (_evt) {                 \
         case NETCONN_EVT_RCVPLUS:          \
          (_sock)->rcvevent++;              \
          break;                    \
         case NETCONN_EVT_RCVMINUS:         \
          (_sock)->rcvevent--;              \
          break;                    \
         case NETCONN_EVT_SENDPLUS:         \
          (_sock)->sendevent = 1;           \
          break;                    \
         case NETCONN_EVT_SENDMINUS:            \
          (_sock)->sendevent = 0;           \
          break;                    \
         case NETCONN_EVT_ERROR:            \
          (_sock)->errevent = 1;            \
          break;                    \
         default:                   \
          LWIP_ASSERT("unknown event", 0);      \
          break;                    \
        }

#define LWIP_SET_SO_ERROR(_sock,_optval)  \
                if (((_sock)->err == 0) || ((_sock)->err == EINPROGRESS)) {   \
                  sys_mutex_lock(&(_sock)->conn->conn_mutex);\
                   sock_set_errno_unsafe((_sock), err_to_errno((_sock)->conn->last_err));   \
                  sys_mutex_unlock(&(_sock)->conn->conn_mutex);\
                }                                                                       \
                *(int *)(_optval) = (_sock)->err;                                           \
                (_sock)->err = 0;

#define LWIP_UPDATE_OPTVAL(_optval_tmp , _optval)   \
                 if((_optval_tmp)) { \
                   *(int *)(_optval) = 1;  \
         } else { \
                    *(int *)(_optval) = 0; \
                 }

#define VALIDATE_GET_RAW_OPTNAME(_sock,_optname,_err)   \
                     if(((_sock)->conn->type == NETCONN_RAW) && (((_optname) != SO_BROADCAST) && ((_optname) !=SO_RCVTIMEO) && ((_optname) !=SO_RCVBUF) && ((_optname) != SO_TYPE))) \
                       {    \
                        (_err) = ENOPROTOOPT;   \
                        break;               \
                       }

#define VALIDATE_SET_RAW_OPTNAME(_sock,_optname,_err)   \
                     if(((_sock)->conn->type == NETCONN_RAW) && (((_optname)  != SO_BROADCAST) && ((_optname)  !=SO_RCVTIMEO) && ((_optname)  !=SO_RCVBUF)))  \
                     {  \
                        (_err) = ENOPROTOOPT;   \
                        break;   \
                     }

#if PF_PKT_SUPPORT
#define VALIDATE_SET_PF_PKT_OPTNAME(_s,_level,_optname,_err)   \
                     if((_level) == SOL_PACKET && ((_optname) != SO_RCVTIMEO && (_optname) != SO_RCVBUF))  \
                     {  \
                        LWIP_DEBUGF(SOCKETS_DEBUG, ("lwip_setsockopt(%d, SOL_PACKET, UNIMPL: optname=0x%x, ..)\n",  \
                            (_s), (_optname)));  \
                        (_err) = ENOPROTOOPT;  \
                        break;   \
                     }
#define VALIDATE_GET_PF_PKT_OPTNAME(_s,_level,_optname,_err)   \
                     if((_level) == SOL_PACKET && ((_optname) != SO_RCVTIMEO && (_optname) != SO_RCVBUF && (_optname) != SO_TYPE))  \
                     {  \
                        LWIP_DEBUGF(SOCKETS_DEBUG, ("lwip_setsockopt(%d, SOL_PACKET, UNIMPL: optname=0x%x, ..)\n",  \
                            (_s), (_optname)));  \
                        (_err) = ENOPROTOOPT;  \
                        break;   \
                     }

#define VALIDATE_LEVEL_PF_PACKET(_sock,_level)   \
                    if(((_sock)->conn->type == NETCONN_PKT_RAW && SOL_PACKET != (_level)) ||  \
                               ((_sock)->conn->type != NETCONN_PKT_RAW && SOL_PACKET == (_level)))
#endif

/* Forward delcaration of some functions */
LWIP_STATIC void event_callback(struct netconn *conn, enum netconn_evt evt, u16_t len);
LWIP_STATIC void lwip_getsockopt_internal(void *arg);
LWIP_STATIC void lwip_setsockopt_internal(void *arg);

/**
 * lock lwip_sock parameters from race condition from different thread in parallel
 *
 * @param *sock socket whos paramerters need to be protected.
 * @return void
 */
static inline void
lwip_sock_lock(struct lwip_sock *sock)
{
#if LWIP_COMPAT_MUTEX
    u32_t iRet;
    iRet = sys_mutex_lock(&sock->mutex);
    LWIP_ASSERT("sys_mutex_lock failed", (iRet == 0));
    LWIP_UNUSED_ARG(iRet);
#else
    sys_mutex_lock(&sock->mutex);
#endif
}

/**
 * lock lwip_sock parameters from race condition from different thread in parallel
 *
 * @param *sock socket whos paramerters need to be protected.
 * @return void
 */
static inline void
lwip_sock_lock_op(struct lwip_sock *sock)
{
#if LWIP_COMPAT_MUTEX
    u32_t iRet;


    iRet = sys_mutex_lock(&sock->mutex_op);
    LWIP_ASSERT("sys_mutex_lock failed", (iRet == 0));
    LWIP_UNUSED_ARG(iRet);
#else
    sys_mutex_lock(&sock->mutex_op);
#endif
}

/**
 * unlock lwip_sock parameters for using through different thread in parallel
 *
 * @param *sock socket.
 * @return void
 */
static inline void
lwip_sock_unlock(struct lwip_sock *sock)
{
    sys_mutex_unlock(&sock->mutex);
}

/**
 * unlock lwip_sock parameters for using through different thread in parallel
 *
 * @param *sock socket.
 * @return void
 */
static inline void
lwip_sock_unlock_op(struct lwip_sock *sock)
{
    sys_mutex_unlock(&sock->mutex_op);
}

int sock_init(void)
{
  int i;

  /* allocate a new socket identifier */
  for (i = 0; i < NUM_SOCKETS; ++i) {
    if(sys_mutex_new(&sockets[i].mutex) != ERR_OK) {
      goto FAILURE;
    }
    if(sys_mutex_new(&sockets[i].mutex_op) != ERR_OK) {
      sys_mutex_free(&sockets[i].mutex);
      goto FAILURE;
    }
  }

  return ERR_OK;
  FAILURE:

  /* Free all resources if failure happens */
  for (--i; i >= 0;i--)
  {
    sys_mutex_free(&sockets[i].mutex);
    sys_mutex_free(&sockets[i].mutex_op);
  }

  return ERR_MEM;
}

#define get_socket(s) get_socket_internal(s, LWIP_SOCKET_ACTIVE)
#define get_socket_mark_state(s, flag) get_socket_internal(s, flag)



LWIP_STATIC inline struct lwip_sock *
get_socket_index(int s)
{
/*lint -e835 */
  if ((s < LWIP_SOCKET_START_NUM) || (s >= (LWIP_SOCKET_START_NUM + NUM_SOCKETS))) {
    return NULL;
  }

  s = s - LWIP_SOCKET_START_NUM;
/*lint +e835 */
  return &sockets[s];
}



/**
 * Map a externally used socket index to the internal socket representation.
 *
 * @param s externally used socket index
 * @return struct lwip_sock for the socket or NULL if not found
 */
LWIP_STATIC struct lwip_sock *
get_socket_internal(int s, int flag)
{
  struct lwip_sock *sock;

  sock = get_socket_index(s);
  if (NULL == sock) {
    LWIP_DEBUGF(SOCKETS_DEBUG, ("get_socket(%d): invalid\n", s));
    set_errno(EBADF);
    return NULL;
  }

  sys_mutex_lock(&sock->mutex);
  if (!sock->conn) {
    sys_mutex_unlock(&sock->mutex);
    LWIP_DEBUGF(SOCKETS_DEBUG, ("get_socket(%d): not active\n", s));
    set_errno(EBADF);
    return NULL;
  }

  /* Mutex is not allocated until connection pointer is NULL,*/
  if (sock->sock_flag != LWIP_SOCKET_ACTIVE)
  {
    sys_mutex_unlock(&sock->mutex);
    set_errno(EBADF);
    return NULL;
  }

  if (flag == LWIP_SOCKET_CLOSING)
  {
    sock->sock_flag = LWIP_SOCKET_CLOSING;
  }

  sys_mutex_unlock(&sock->mutex);
  set_errno(0);
  return sock;
}

LWIP_STATIC struct lwip_sock *
get_socket_unsafe(int s, int flag)
{
  struct lwip_sock *sock;

  sock = get_socket_index(s);
  if (NULL == sock) {
    LWIP_DEBUGF(SOCKETS_DEBUG, ("get_socket(%d): invalid\n", s));
    set_errno(EBADF);
    return NULL;
  }

  if (!sock->conn) {
    LWIP_DEBUGF(SOCKETS_DEBUG, ("get_socket(%d): not active\n", s));
    set_errno(EBADF);
    return NULL;
  }

  if (sock->sock_flag != LWIP_SOCKET_ACTIVE)
  {
    set_errno(EBADF);
    return NULL;
  }

  if (flag == LWIP_SOCKET_CLOSING)
  {
    sock->sock_flag = LWIP_SOCKET_CLOSING;
  }
  set_errno(0);
  return sock;
}

/**
 * Allocate a new socket for a given netconn.
 *
 * @param newconn the netconn for which to allocate a socket
 * @param accepted 1 if socket has been created by accept(),
 *                 0 if socket has been created by socket()
 * @return the index of the new socket; -1 on error
 */
LWIP_STATIC int
alloc_socket(struct netconn *newconn, int accepted)
{
  int i;

  /* allocate a new socket identifier */
  for (i = 0; i < NUM_SOCKETS; ++i) {
    /* Protect socket array */
    /* socket shall be valid here*/
    lwip_sock_lock(&sockets[i]);
    if (!sockets[i].conn) {
      if (sys_sem_new(&sockets[i].close_sem, 0) != ERR_OK) {
        lwip_sock_unlock(&sockets[i]);
        return -1;
      }
      sockets[i].conn       = newconn;
      sockets[i].rcvevent   = 0;
      sockets[i].errevent   = 0;
      sockets[i].sendevent  = (newconn->type == NETCONN_TCP ? (accepted != 0) : 1);
      /* The socket is not yet known to anyone, so no need to protect
      after having marked it as used. */
      sockets[i].sock_flag = LWIP_SOCKET_ACTIVE;
      sockets[i].err        = 0;
      /** this parameters are added for providing MT support for socket */
      sockets[i].events= 0;
      sockets[i].lastdata   = NULL;
      sockets[i].lastoffset = 0;
      /* TCP sendbuf is empty, but the socket is not yet writable until connected
       * (unless it has been created by accept()). */
      sockets[i].select_waiting = 0;



      lwip_sock_unlock(&sockets[i]);
      /*lint -e835 */
      return (i+LWIP_SOCKET_START_NUM);
      /*lint +e835 */
    }

    lwip_sock_unlock(&sockets[i]);

  }
  return -1;
}

/** Free a socket. The socket's netconn must have been
 * delete before!
 *
 * @param sock the socket to free
 * @param is_tcp != 0 for TCP sockets, used to free lastdata
 */
LWIP_STATIC void
free_socket(struct lwip_sock *sock, int is_tcp)
{
  void *lastdata;

  lwip_sock_lock(sock);
  lastdata         = sock->lastdata;
  sock->lastdata   = NULL;
  sock->lastoffset = 0;

  /* Protect socket array */
  netconn_finish_delete(sock->conn);
  sock->conn       = NULL;

  sock->err        = 0;
  sock->sock_flag = LWIP_SOCKET_INACTIVE;
  sock->events = 0;
  lwip_sock_unlock(sock);

  sys_sem_free(&sock->close_sem);
  sys_sem_set_invalid(&sock->close_sem);


  /* Socket mutex shall not be freed, as it will remain static for future sockets as well */
  /* don't use 'sock' after this line, as another task might have allocated it */

  if (lastdata != NULL) {
    if (is_tcp) {
      pbuf_free((struct pbuf *)lastdata);
    } else {
      netbuf_delete((struct netbuf *)lastdata);
    }
  }
}

/* Below this, the well-known socket functions are implemented.
 * Use google.com or opengroup.org to get a good description :-)
 *
 * Exceptions are documented!
 */

int
lwip_accept(int s, struct sockaddr *addr, socklen_t *addrlen)
{
#if LWIP_TCP
  struct lwip_sock *sock, *nsock;
  struct lwip_sock *isock;
  struct netconn *newconn = NULL;
  ip_addr_t naddr;
  u16_t port;
  int newsock;
  struct sockaddr_in sin;
  err_t err;
  int iRetSock = -1;
  int sock_errno = 0;

  isock = get_socket_index(s);
  if (NULL == isock) {
    set_errno(EBADF);
    return iRetSock;
  }

  lwip_sock_lock(isock);
  sock = get_socket_unsafe(s, 0);
  if(NULL == sock)
  {
    lwip_sock_unlock(isock);
    set_errno(EBADF);
    return iRetSock;
  }

#if PF_PKT_SUPPORT
  if(NETCONN_PKT_RAW == sock->conn->type)
  {
    lwip_sock_unlock(sock);
    set_errno(EOPNOTSUPP);
    return iRetSock;
  }
#endif

 (sock)->events++;
  lwip_sock_unlock(isock);

  sys_mutex_lock(&sock->conn->conn_mutex);
  if (netconn_is_nonblocking(sock->conn) && (sock->rcvevent <= 0)) {
    sys_mutex_unlock(&sock->conn->conn_mutex);
    LWIP_DEBUGF(SOCKETS_DEBUG, ("lwip_accept(%d): returning EWOULDBLOCK\n", s));
    sock_errno = EWOULDBLOCK;
    sock_set_errno(sock, sock_errno);
    iRetSock = -1;
    goto RETURN;
  }
  sys_mutex_unlock(&sock->conn->conn_mutex);

  /* wait for a new connection */
  err = netconn_accept(sock->conn, &newconn);
  if (err != ERR_OK) {
    LWIP_DEBUGF(SOCKETS_DEBUG, ("lwip_accept(%d): netconn_acept failed, err=%d\n", s, err));
    if (netconn_type(sock->conn) != NETCONN_TCP) {
      sock_errno = EOPNOTSUPP;
      sock_set_errno(sock, sock_errno);
      iRetSock = -1;
      goto RETURN;
    }
    sock_errno = err_to_errno(err);
    sock_set_errno(sock, sock_errno);
    iRetSock = -1;
    goto RETURN;
  }

  LWIP_ASSERT("newconn != NULL", newconn != NULL);
  /* Prevent automatic window updates, we do this on our own! */
  netconn_set_noautorecved(newconn);

  /* get the IP_add and port of the remote host */
  err = netconn_peer(newconn, &naddr, &port);
  if (err != ERR_OK) {
    LWIP_DEBUGF(SOCKETS_DEBUG, ("lwip_accept(%d): netconn_peer failed, err=%d\n", s, err));
    netconn_delete(newconn);
    sock_errno = err_to_errno(err);
    sock_set_errno(sock, sock_errno);
    iRetSock = -1;
    goto RETURN;
  }

  /* Note that POSIX only requires us to check addr is non-NULL. addrlen must
   * not be NULL if addr is valid.
   */
  if ((NULL != addr) && (NULL !=addrlen)) {
    LWIP_ASSERT("addr valid but addrlen NULL", addrlen != NULL);
    (void)MEMSET_S(&sin, sizeof(sin), 0, sizeof(sin)); //CSEC_FIX_2302
    sin.sin_family = AF_INET;
    sin.sin_port = htons(port);
    inet_addr_from_ipaddr(&sin.sin_addr, &naddr);

    if (*addrlen > sizeof(sin))
      *addrlen = sizeof(sin);

    if(*addrlen) //CSEC_FIX_2302
    {
        (void)MEMCPY_S(addr, *addrlen, &sin, *addrlen); //CSEC_FIX_2302
    }
  }

  newsock = alloc_socket(newconn, 1);
  if (newsock == -1) {
    netconn_delete(newconn);
    sock_errno = ENFILE;
    sock_set_errno(sock, sock_errno);
    iRetSock = -1;
    goto RETURN;
  }


  /*lint -e835 */
  LWIP_ASSERT("invalid socket index", (newsock >= LWIP_SOCKET_START_NUM) && (newsock < (LWIP_SOCKET_START_NUM + NUM_SOCKETS)));
 /*lint +e835 */
  LWIP_ASSERT("newconn->callback == event_callback", newconn->callback == event_callback);
  nsock = get_socket(newsock);

  /* See event_callback: If data comes in right away after an accept, even
   * though the server task might not have created a new socket yet.
   * In that case, newconn->socket is counted down (newconn->socket--),
   * so nsock->rcvevent is >= 1 here!
   */
  sys_mutex_lock(&nsock->conn->conn_mutex);
  nsock->rcvevent = (s16_t)(nsock->rcvevent -(1 + newconn->socket));
  newconn->socket = newsock;
  sys_mutex_unlock(&nsock->conn->conn_mutex);

  LWIP_DEBUGF(SOCKETS_DEBUG, ("lwip_accept(%d) returning new sock=%d addr=", s, newsock));
  #ifdef  LWIP_DEV_DEBUG
  ip_addr_debug_print(SOCKETS_DEBUG, &naddr);
  LWIP_DEBUGF(SOCKETS_DEBUG, (" port=%"U16_F"\n", port));
  #endif
  LWIP_SOCK_DECREMENT_EVENTS(sock);
  sock_set_errno(sock, 0);
  return newsock;

  RETURN:
  LWIP_SOCK_DECREMENT_EVENTS(sock);
  set_errno(sock_errno);
  return iRetSock;
#else /* LWIP_TCP */
LWIP_UNUSED_ARG(s);
LWIP_UNUSED_ARG(addr);
LWIP_UNUSED_ARG(addrlen);
sock_errno = EPROTO;
sock_set_errno(sock, sock_errno);
return -1;
#endif /* LWIP_TCP */
}

int
lwip_bind(int s, const struct sockaddr *name, socklen_t namelen)
{
  struct lwip_sock *sock, *isock;
  ip_addr_t local_addr;
  u16_t local_port;
  err_t err;
  const struct sockaddr_in *name_in;
#if PF_PKT_SUPPORT
  const struct sockaddr_ll *name_ll;
  u8_t local_ifindex = 0;
#endif

  isock = sock = get_socket_index(s);
  if(NULL == sock)
  {
    set_errno(EBADF);
    return -1;
  }

  lwip_sock_lock(sock);
  sock = get_socket_unsafe(s,LWIP_SOCKET_ACTIVE);
  if (!sock) {
    lwip_sock_unlock(isock);
    set_errno(EBADF);
    return -1;
  }

  lwip_sock_unlock(isock);
  name_in = (const struct sockaddr_in *)(void*)name;

  /* check size, familiy and alignment of 'name' */
#if PF_PKT_SUPPORT
  if (sock->conn->type == NETCONN_PKT_RAW) {
    name_ll = (const struct sockaddr_ll *)(void*)name;

    LWIP_ERROR("lwip_bind: invalid address", ((name_ll != NULL)&&(namelen == sizeof(struct sockaddr_ll)) &&
               ((name_ll->sll_family) == PF_PACKET) && ((((mem_ptr_t)name_in) % 4) == 0 ) &&
               (name_ll->sll_ifindex <= LWIP_NETIF_IFINDEX_MAX)),
               sock_set_errno(sock, err_to_errno(ERR_VAL)); return -1;);

    local_addr.addr = IPADDR_ANY;
    local_port = name_ll->sll_protocol;
    local_ifindex = (u8_t)name_ll->sll_ifindex;
  } else
#endif
  {
    LWIP_ERROR("lwip_bind: invalid address", ((name_in != NULL)&&(namelen == sizeof(struct sockaddr_in)) &&
               ((name_in->sin_family) == AF_INET) && ((((mem_ptr_t)name_in) % 4) == 0)),
               sock_set_errno(sock, err_to_errno(ERR_VAL)); return -1;);

    inet_addr_to_ipaddr(&local_addr, &name_in->sin_addr);
    local_port = name_in->sin_port;
  }
#ifdef  LWIP_DEV_DEBUG
#if PF_PKT_SUPPORT
  LWIP_DEBUGF(SOCKETS_DEBUG, ("lwip_bind(%d, ", s));
  LWIP_DEBUGF(SOCKETS_DEBUG, (" ifindex=%u proto=%"U16_F")\n", local_ifindex, ntohs(local_port)));
#else
  LWIP_DEBUGF(SOCKETS_DEBUG, ("lwip_bind(%d, addr=", s));
  ip_addr_debug_print(SOCKETS_DEBUG, &local_addr);
  LWIP_DEBUGF(SOCKETS_DEBUG, (" port=%"U16_F")\n", ntohs(local_port)));
#endif
#endif

#if PF_PKT_SUPPORT
  err = netconn_bind(sock->conn, &local_addr, (u16_t)((NETCONNTYPE_GROUP(sock->conn->type) == NETCONN_PKT_RAW)
                 ? local_port : ntohs(local_port)), local_ifindex);
#else
  err = netconn_bind(sock->conn, &local_addr, ntohs(local_port));
#endif

  if (err != ERR_OK) {
    LWIP_DEBUGF(SOCKETS_DEBUG, ("lwip_bind(%d) failed, err=%d\n", s, err));
    sock_set_errno(sock, err_to_errno(err));
    return -1;
  }

  LWIP_DEBUGF(SOCKETS_DEBUG, ("lwip_bind(%d) succeeded\n", s));
  sock_set_errno(sock, 0);
  return 0;
}

/*lint -e838*/
int
lwip_close(int s)
{
  struct lwip_sock *sock;
  int is_tcp = 0;

  LWIP_DEBUGF(SOCKETS_DEBUG, ("lwip_close(%d)\n", s));
  sock = get_socket_mark_state(s, LWIP_SOCKET_CLOSING);
  if (NULL == sock) {
    set_errno(EBADF);
    return -1;
  }

  /** initiate the close event */
  is_tcp = netconn_type(sock->conn) == NETCONN_TCP;
  netconn_initiate_delete(sock->conn);

  /** for all events to close, before closing the socket */
  lwip_sock_lock(sock);
  if (sock->events)
  {
    lwip_sock_unlock(sock);
    sys_arch_sem_wait(&sock->close_sem, 0);
  }
  else
  {
    lwip_sock_unlock(sock);
  }

  /** once all events are processed, free netconn and socket */
  //netconn_finish_delete(sock->conn);
  free_socket(sock, is_tcp);
  set_errno(0);
  return 0;
}
/*lint +e838*/

int
lwip_connect(int s, const struct sockaddr *name, socklen_t namelen)
{
  struct lwip_sock *sock;
  err_t err;
  const struct sockaddr_in *name_in;
  ip_addr_t remote_addr;
  u16_t remote_port;

  sock = get_socket(s);
  if(NULL == sock)
  {
    set_errno(EBADF);
    return -1;
  }

#if PF_PKT_SUPPORT
  lwip_sock_lock(sock);
  if(NETCONN_PKT_RAW == sock->conn->type)
  {
    lwip_sock_unlock(sock);
    set_errno(EOPNOTSUPP);
    return -1;
  }
  lwip_sock_unlock(sock);
#endif

  name_in = (const struct sockaddr_in *)(void*)name;

  /* check size, familiy and alignment of 'name' */
  LWIP_ERROR("lwip_connect: invalid address", ((name_in != NULL) && (namelen == sizeof(struct sockaddr_in)) &&
             ((name_in->sin_family) == AF_INET) && ((((mem_ptr_t)name_in) % 4) == 0)),
             sock_set_errno(sock, err_to_errno(ERR_VAL));  return -1;);


    inet_addr_to_ipaddr(&remote_addr, &name_in->sin_addr);
    remote_port = name_in->sin_port;

    LWIP_DEBUGF(SOCKETS_DEBUG, ("lwip_connect(%d, addr=", s));
#ifdef  LWIP_DEV_DEBUG
    ip_addr_debug_print(SOCKETS_DEBUG, &remote_addr);
    LWIP_DEBUGF(SOCKETS_DEBUG, (" port=%"U16_F")\n", ntohs(remote_port)));
#endif
    err = netconn_connect(sock->conn, &remote_addr, ntohs(remote_port));

  if (err != ERR_OK) {
    LWIP_DEBUGF(SOCKETS_DEBUG, ("lwip_connect(%d) failed, err=%d\n", s, err));
    sock_set_errno(sock, err_to_errno(err));
    return -1;
  }

  LWIP_DEBUGF(SOCKETS_DEBUG, ("lwip_connect(%d) succeeded\n", s));
  sock_set_errno(sock, 0);
  return 0;
}

/**
 * Set a socket into listen mode.
 * The socket may not have been used for another connection previously.
 *
 * @param s the socket to set to listening mode
 * @param backlog (ATTENTION: needs TCP_LISTEN_BACKLOG=1)
 * @return 0 on success, non-zero on failure
 */
int
lwip_listen(int s, int backlog)
{
  struct lwip_sock *sock;
  struct lwip_sock *isock;
  int sock_errno = 0;

  err_t err;
  int retval = 0;

  LWIP_DEBUGF(SOCKETS_DEBUG, ("lwip_listen(%d, backlog=%d)\n", s, backlog));

  isock = get_socket_index(s);
  if(NULL == isock) {
    set_errno(EBADF);
    return -1;
  }

  lwip_sock_lock(isock);
  sock = get_socket_unsafe(s, LWIP_SOCKET_ACTIVE);
  if(NULL == sock)
  {
    lwip_sock_unlock(isock);
    set_errno(EBADF);
    return -1;
  }

  if (netconn_type(sock->conn) != NETCONN_TCP)
  {
    lwip_sock_unlock(sock);
    set_errno(EOPNOTSUPP);
    return -1;
  }

  if (sock->conn->pcb.tcp->local_port == 0) {
  	lwip_sock_unlock(sock);
	set_errno(EDESTADDRREQ);
	return -1;
	}

  /* limit the "backlog" parameter to fit in an u8_t */
  backlog = LWIP_MIN(LWIP_MAX(backlog, 0), 0xf);

  err = netconn_listen_with_backlog(sock->conn, (u8_t)backlog);

  if (err != ERR_OK) {
    LWIP_DEBUGF(SOCKETS_DEBUG, ("lwip_listen(%d) failed, err=%d\n", s, err));
    sock_set_errno_unsafe(sock, err_to_errno(err));
    sock_errno = err;
    retval = -1;
    goto RETURN;
  }

  sock_set_errno_unsafe(sock, 0);
RETURN:
  lwip_sock_unlock(sock);
  set_errno(err_to_errno(sock_errno));
  //set_errno(sock_errno);
  return retval;
}

int
lwip_recvfrom(int s, void *mem, size_t len, int flags,
        struct sockaddr *from, socklen_t *fromlen)
{
  struct lwip_sock *sock;
  struct lwip_sock *isock;

  void             *buf = NULL;
  struct pbuf      *p;
  u16_t            buflen, copylen;
  int              off = 0;
  int               retval = 0;
  u16_t            port;
  u8_t             done = 0;
  err_t            err;
  unsigned int acceptable_flags = 0;
  int sock_errno = 0;

  LWIP_DEBUGF(SOCKETS_DEBUG, ("lwip_recvfrom(%d, %p, %"SZT_F", 0x%x, ..)\n", s, mem, (unsigned int)len, flags));

  isock = get_socket_index(s);
  if(NULL == isock)
  {
    set_errno(EBADF);
    return -1;
  }

  lwip_sock_lock(isock);
  sock = get_socket_unsafe(s, LWIP_SOCKET_ACTIVE);
  if (NULL == sock) {
    lwip_sock_unlock(isock);
    set_errno(EBADF);
    return -1;
  }

  LWIP_ERROR("lwip_recvfrom: invalid arguments", ((mem != NULL) && (len != 0) && (flags>=0)),
    sock_set_errno_unsafe(sock, err_to_errno(ERR_VAL));lwip_sock_unlock(isock);set_errno(err_to_errno(ERR_VAL));
    return -1;);

  acceptable_flags = acceptable_flags | MSG_PEEK | MSG_DONTWAIT;
  if ((~acceptable_flags) & flags)
  	{
  	sock_set_errno_unsafe(sock, err_to_errno(ERR_OPNOTSUPP));
	lwip_sock_unlock(isock);
	set_errno(err_to_errno(ERR_OPNOTSUPP));
	return -1;
	}

  sock->events++;
  lwip_sock_unlock(isock);

  /*COVERITY: -ve check */
  /* TO DO: Jay: Have moved the flags check from here to the LWIP_ERROR macro above. Need to check and remove this commented code */
 /*return -1 if value of flags < 0*/

  do {
    LWIP_DEBUGF(SOCKETS_DEBUG, ("lwip_recvfrom: top while sock->lastdata=%p\n", sock->lastdata));
    /* Check if there is data left from the last recv operation. */
    if (sock->lastdata) {
      buf = sock->lastdata;
    } else {
      /* If this is non-blocking call, then check first */
      sys_mutex_lock(&sock->conn->conn_mutex);
      if ((((u32_t)flags & MSG_DONTWAIT) || netconn_is_nonblocking(sock->conn)) &&
          (sock->rcvevent <= 0)) {
        sys_mutex_unlock(&sock->conn->conn_mutex);
        if (off > 0) {
          /* update receive window */
          netconn_recved(sock->conn, (u32_t)off);
          /* already received data, return that */
          sock_set_errno(sock, sock_errno);
          retval = off;
          goto RETURN;
        }
        LWIP_DEBUGF(SOCKETS_DEBUG, ("lwip_recvfrom(%d): returning EWOULDBLOCK\n", s));
        sock_errno = EWOULDBLOCK;
        sock_set_errno(sock, EWOULDBLOCK);
        retval = -1;
        goto RETURN;
      }
      else
      {
        sys_mutex_unlock(&sock->conn->conn_mutex);
      }

      /* No data was left from the previous operation, so we try to get
         some from the network. */
      if (netconn_type(sock->conn) == NETCONN_TCP) {
        err = netconn_recv_tcp_pbuf(sock->conn, (struct pbuf **)&buf);
      } else {
        err = netconn_recv(sock->conn, (struct netbuf **)&buf);
      }
      LWIP_DEBUGF(SOCKETS_DEBUG, ("lwip_recvfrom: netconn_recv err=%d, netbuf=%p\n",
        err, buf));

      if (err != ERR_OK) {
        if (off > 0) {
          /* update receive window */
          netconn_recved(sock->conn, (u32_t)off);
          /* already received data, return that */
          sock_set_errno(sock, sock_errno);
          retval = off;
          goto RETURN;
        }
        /* We should really do some error checking here. */
        LWIP_DEBUGF(SOCKETS_DEBUG, ("lwip_recvfrom(%d): buf == NULL, error is \"%s\"!\n",
          s, lwip_strerr(err)));
        sock_errno = err_to_errno(err);
        sock_set_errno(sock, sock_errno);
        if (err == ERR_CLSD) {
          retval = 0;
		  sock_errno = 0;
        } else {
          retval = -1;
        }
        goto RETURN;
      }
      LWIP_ASSERT("buf != NULL", buf != NULL);
      sock->lastdata = buf;
    }

    if (netconn_type(sock->conn) == NETCONN_TCP) {
      p = (struct pbuf *)buf;
    } else {
      p = ((struct netbuf *)buf)->p;
    }
    buflen = p->tot_len;
    LWIP_DEBUGF(SOCKETS_DEBUG, ("lwip_recvfrom: buflen=%"U16_F" len=%"SZT_F" off=%d sock->lastoffset=%"U16_F"\n",
      buflen, (unsigned int)len, off, sock->lastoffset));

    buflen = (u16_t)(buflen - sock->lastoffset);

    if (len > buflen) {
      copylen = buflen;
    } else {
      copylen = (u16_t)len;
    }

    /* copy the contents of the received buffer into
    the supplied memory pointer mem */
    pbuf_copy_partial(p, (u8_t*)mem + off, copylen, sock->lastoffset);

    off += copylen;

    if (netconn_type(sock->conn) == NETCONN_TCP) {
      LWIP_ASSERT("invalid copylen, len would underflow", len >= copylen);
      len -= copylen;

      //PC Lint Error Suppressed:  non-negative quantity cannot be less than zero
      //lint -e775
      sys_mutex_lock(&sock->conn->conn_mutex);
      if ( (len <= 0) ||  (p->flags & PBUF_FLAG_PUSH) || (sock->rcvevent<= 0) || (((u32_t)flags & MSG_PEEK)!=0)) {
        done = 1;
      }
      sys_mutex_unlock(&sock->conn->conn_mutex);
      //lint +e775

    } else {
      done = 1;
    }

    /* Check to see from where the data was.*/
    if (done) {
      ip_addr_t fromaddr;
      ip_addr_t  *addr=NULL;

      if (from && fromlen) {
        struct sockaddr_in sin;
#if PF_PKT_SUPPORT
        struct sockaddr_ll sll;
        struct netbuf *nbuf = (struct netbuf *)buf;
#endif

        if (netconn_type(sock->conn) == NETCONN_TCP) {
          addr = &fromaddr;
          (void)netconn_getaddr(sock->conn, addr, &port, 0);
#if PF_PKT_SUPPORT
        } else if (netconn_type(sock->conn) == NETCONN_PKT_RAW) {
            /* get eth_hdr type*/
            port = netbuf_fromhatype((struct netbuf *)buf);
#endif
        } else {
          addr = netbuf_fromaddr((struct netbuf *)buf);
          port = netbuf_fromport((struct netbuf *)buf);
        }

#if PF_PKT_SUPPORT
        if(sock->conn->type == NETCONN_PKT_RAW) {

          if (*fromlen > sizeof(sll)) {
            *fromlen = sizeof(sll);
          }

          if(*fromlen)
          {
            (void)MEMSET_S(&sll, sizeof(sll), 0, sizeof(sll));
            sll.sll_family = PF_PACKET;
            sll.sll_hatype = port;

            if (nbuf->p->flags & PBUF_FLAG_LLBCAST) {
              sll.sll_pkttype = PACKET_BROADCAST;
            } else if (nbuf->p->flags & PBUF_FLAG_LLMCAST) {
              sll.sll_pkttype = PACKET_MULTICAST;
            } else if(nbuf->p->flags & PBUF_FLAG_HOST) {
              sll.sll_pkttype = PACKET_HOST;
            } else {
              sll.sll_pkttype = PACKET_OTHERHOST;
            }

            (void)MEMCPY_S(from, *fromlen, &sll, *fromlen);
            *fromlen = sizeof(sll);
          }

          LWIP_DEBUGF(SOCKETS_DEBUG, ("lwip_recvfrom(%d): hatype=%u", s, ntohs(sll.sll_hatype)));
          LWIP_DEBUGF(SOCKETS_DEBUG, (" packet type = %u\n", sll.sll_pkttype));
        } else
#endif
        {
          (void)MEMSET_S(&sin, sizeof(sin), 0, sizeof(sin)); //CSEC_FIX_2302
          sin.sin_family = AF_INET;
          sin.sin_port = htons(port);
          if (addr != NULL)
            inet_addr_from_ipaddr(&sin.sin_addr, addr);

        if (*fromlen > sizeof(sin)) {
          *fromlen = sizeof(sin);
        }

        if(*fromlen)
        {
            (void)MEMCPY_S(from, *fromlen, &sin, *fromlen);//CSEC_FIX_2302
        }

        LWIP_DEBUGF(SOCKETS_DEBUG, ("lwip_recvfrom(%d): addr=", s));
#ifdef  LWIP_DEV_DEBUG
        ip_addr_debug_print(SOCKETS_DEBUG, addr);
        LWIP_DEBUGF(SOCKETS_DEBUG, (" port=%"U16_F" len=%d\n", port, off));
#endif
        }
      } else {
#if SOCKETS_DEBUG
        if (netconn_type(sock->conn) == NETCONN_TCP) {
          addr = &fromaddr;
          netconn_getaddr(sock->conn, addr, &port, 0);
        } else {
          addr = netbuf_fromaddr((struct netbuf *)buf);
          port = netbuf_fromport((struct netbuf *)buf);
        }

        LWIP_DEBUGF(SOCKETS_DEBUG, ("lwip_recvfrom(%d): addr=", s));
#ifdef  LWIP_DEV_DEBUG
        ip_addr_debug_print(SOCKETS_DEBUG, addr);
        LWIP_DEBUGF(SOCKETS_DEBUG, (" port=%"U16_F" len=%d\n", port, off));
#endif
#endif /*  SOCKETS_DEBUG */
      }
    }

    /* If we don't peek the incoming message... */
    if (((u32_t)flags & MSG_PEEK) == 0) {
      /* If this is a TCP socket, check if there is data left in the
         buffer. If so, it should be saved in the sock structure for next
         time around. */
      if ((netconn_type(sock->conn) == NETCONN_TCP) && (buflen - copylen > 0)) {
        sock->lastdata = buf;
        sock->lastoffset = (u16_t) (sock->lastoffset + copylen);
        LWIP_DEBUGF(SOCKETS_DEBUG, ("lwip_recvfrom: lastdata now netbuf=%p\n", buf));
      } else {
        sock->lastdata = NULL;
        sock->lastoffset = 0;
        LWIP_DEBUGF(SOCKETS_DEBUG, ("lwip_recvfrom: deleting netbuf=%p\n", buf));
        if (netconn_type(sock->conn) == NETCONN_TCP) {
          pbuf_free((struct pbuf *)buf);
        } else {
          netbuf_delete((struct netbuf *)buf);
        }
      }
    }
  } while (!done);

  if (off > 0) {
    /* update receive window */
    netconn_recved(sock->conn, (u32_t)off);
    retval = off;
  }
  sock_set_errno(sock, sock_errno);
RETURN:
  LWIP_SOCK_DECREMENT_EVENTS(sock);
  set_errno(sock_errno);
  return retval;
}

int
lwip_read(int s, void *mem, size_t len)
{
  return lwip_recvfrom(s, mem, len, 0, NULL, NULL);
}

int
lwip_recv(int s, void *mem, size_t len, int flags)
{
  return lwip_recvfrom(s, mem, len, flags, NULL, NULL);
}


int
lwip_sendto_internal(int s, const void *data, size_t size, int flags,
       const struct sockaddr *to, socklen_t tolen,struct lwip_sock *sock);

int
lwip_send_internal(int s, const void *data, size_t size, int flags, struct lwip_sock *sock, size_t *written)
{
  u8_t write_flags;
  int err;

  write_flags = (u8_t)(NETCONN_COPY |
    (((u32_t)flags & MSG_MORE)     ? NETCONN_MORE      : 0) |
    (((u32_t)flags & MSG_DONTWAIT) ? NETCONN_DONTBLOCK : 0));
  *written = 0;

  err = netconn_write_partly(sock->conn, data, size, write_flags, written);

  LWIP_DEBUGF(SOCKETS_DEBUG, ("lwip_send_internal(%d) err=%d written=%"SZT_F"\n", s, err, *written));

  return err;
}

int
lwip_send(int s, const void *data, size_t size, int flags)
{
  struct lwip_sock *sock;
  struct lwip_sock *isock;

  int err;
  size_t written;
#if !(LWIP_UDP || LWIP_RAW)
  int retval;
#endif

  LWIP_DEBUGF(SOCKETS_DEBUG, ("lwip_send(%d, data=%p, size=%"SZT_F", flags=0x%x)\n",
                              s, data, (unsigned int)size, flags));

    /*COVERITY: Checking the val for -ve to make sure non -ve ness */
   /* TO DO: Jay: Moved the check from here to the LWIP_ERROR macro. Check & remove this commented code */
   /*return -1 if value of flags < 0*/

  isock = get_socket_index(s);
  if(NULL == isock)
  {
    set_errno(EBADF);
    return -1;
  }

  lwip_sock_lock(isock);
  sock = get_socket_unsafe(s, LWIP_SOCKET_ACTIVE);
  if (NULL == sock)
  {
    lwip_sock_unlock(isock);
    set_errno(EBADF);
    return -1;
  }

  sock->events++;
  lwip_sock_unlock(isock);

  LWIP_ERROR("lwip_send: invalid arguments", ((data != NULL)),
  sock_set_errno(sock, err_to_errno(ERR_VAL)); LWIP_SOCK_DECREMENT_EVENTS(sock);
  set_errno(err_to_errno(ERR_VAL)); return -1;);


  if (sock->conn->type != NETCONN_TCP) {
#if (LWIP_UDP || LWIP_RAW)
	 if (size > 65000) {
	 	sock_set_errno(sock, EMSGSIZE);
		LWIP_SOCK_DECREMENT_EVENTS(sock);
		set_errno(EMSGSIZE);
		return -1;
	 }

    err  = lwip_sendto_internal(s, data, size, flags, NULL, 0, sock);
	sock_set_errno(sock, err_to_errno(err));
    LWIP_SOCK_DECREMENT_EVENTS(sock);
	return (err == ERR_OK ? size : (err == ERR_MEM ? 0 : -1));
#else /* (LWIP_UDP || LWIP_RAW) */
    sock_set_errno(sock, err_to_errno(ERR_VAL));
    retval =  -1;
    LWIP_SOCK_DECREMENT_EVENTS(sock);
    set_errno(err_to_errno(ERR_VAL));

    return retval;
#endif /* (LWIP_UDP || LWIP_RAW) */
  }

  err = lwip_send_internal(s, data, size, flags, sock, &written);
  sock_set_errno(sock, err_to_errno(err));
  LWIP_SOCK_DECREMENT_EVENTS(sock);
  return (err == ERR_OK ? (int)written : -1);
}

int
lwip_sendto_internal(int s, const void *data, size_t size, int flags,
       const struct sockaddr *to, socklen_t tolen,struct lwip_sock *sock)
{
  int err;
  u16_t short_size;
#if PF_PKT_SUPPORT
  const struct sockaddr_in *to_in = NULL;
  const struct sockaddr_ll *to_ll = NULL;
#else
  const struct sockaddr_in *to_in;
#endif
  u16_t remote_port;

#if !LWIP_TCPIP_CORE_LOCKING
  struct netbuf buf;
#endif
  unsigned int acceptable_flags = 0;

  short_size = (u16_t)size;

  /* PC Lint error fixed: Possible use of null pointer 'to' in left argument to operator '-&gt;' [Reference: file D:\build\IoT_lwIP_Build\code\current\trunk\testcode\lwip_win\src\api\sockets.c: line 828] */
  /*lint -e613*/
#if PF_PKT_SUPPORT
  if (sock->conn->type == NETCONN_PKT_RAW) {
    to_ll = (const struct sockaddr_ll *)(void *)to;
    LWIP_ERROR("lwip_sendto: invalid address", (((data != NULL) && (size!=0)) &&
               (((to_ll != NULL) && (tolen == sizeof(struct sockaddr_ll)))  &&
               ((to_ll->sll_family) == PF_PACKET) && ((((mem_ptr_t)to_ll) % 4) == 0)) &&
               (to_ll->sll_ifindex <= LWIP_NETIF_IFINDEX_MAX)),
               return ERR_VAL;);

    LWIP_ERROR("lwip_sendto: invalid flags. Should be 0", (flags == 0),
                return EOPNOTSUPP;);
  } else
#endif
  {
    to_in = (const struct sockaddr_in *)(void*)to;
    LWIP_ERROR("lwip_sendto: invalid address", (((data != NULL) && (size!=0) && (flags>=0)) &&
               (((to_in == NULL) && (tolen == 0)) || ((to_in == NULL) && (tolen != 0)) ||
               ((tolen == sizeof(struct sockaddr_in)) &&
               ((to_in->sin_family) == AF_INET) && ((((mem_ptr_t)to_in) % 4) == 0)))),/*lint !e613*/
               return ERR_VAL;);

    acceptable_flags = MSG_MORE | MSG_DONTWAIT;
    if ((~acceptable_flags) & flags) {
      return ERR_OPNOTSUPP;
    }
  }
  /*lint +e613*/

#if LWIP_TCPIP_CORE_LOCKING
  /* Should only be consider like a sample or a simple way to experiment this option (no check of "to" field...) */
    struct pbuf* p;
    ip_addr_t *remote_addr;

#if LWIP_NETIF_TX_SINGLE_PBUF
    p = pbuf_alloc(PBUF_TRANSPORT, short_size, PBUF_RAM);
    if (p != NULL) {
#if LWIP_CHECKSUM_ON_COPY
      u16_t chksum = 0;

      if (sock->conn->type != NETCONN_RAW) {
        chksum = LWIP_CHKSUM_COPY(p->payload, p->len, data, short_size); //CSEC_FIX_2302
      } else
#endif /* LWIP_CHECKSUM_ON_COPY */
      (void)MEMCPY_S(p->payload, p->len, data, size);//CSEC_FIX_2302
#else /* LWIP_NETIF_TX_SINGLE_PBUF */
    p = pbuf_alloc(PBUF_TRANSPORT, short_size, PBUF_REF);
    if (p != NULL) {
      p->payload = (void*)data;
#endif /* LWIP_NETIF_TX_SINGLE_PBUF */

      if (to_in != NULL) {
        inet_addr_to_ipaddr_p(remote_addr, &to_in->sin_addr);
        remote_port = ntohs(to_in->sin_port);
      } else {
        remote_addr = &sock->conn->pcb.ip->remote_ip;
#if LWIP_UDP
        if (NETCONNTYPE_GROUP(sock->conn->type) == NETCONN_UDP) {
          remote_port = sock->conn->pcb.udp->remote_port;
        } else
#endif /* LWIP_UDP */
        {
          remote_port = 0;
        }
      }

      LOCK_TCPIP_CORE();
      if (netconn_type(sock->conn) == NETCONN_RAW) {
#if LWIP_RAW
        err = sock->conn->last_err = raw_sendto(sock->conn->pcb.raw, p, remote_addr);
#else /* LWIP_RAW */
        err = ERR_VAL;
#endif /* LWIP_RAW */
      }
#if LWIP_UDP && LWIP_RAW
      else
#endif /* LWIP_UDP && LWIP_RAW */
      {
#if LWIP_UDP
#if LWIP_CHECKSUM_ON_COPY && LWIP_NETIF_TX_SINGLE_PBUF
        err = sock->conn->last_err = udp_sendto_chksum(sock->conn->pcb.udp, p,
          remote_addr, remote_port, 1, chksum);
#else /* LWIP_CHECKSUM_ON_COPY && LWIP_NETIF_TX_SINGLE_PBUF */
        err = sock->conn->last_err = udp_sendto(sock->conn->pcb.udp, p,
          remote_addr, remote_port);
#endif /* LWIP_CHECKSUM_ON_COPY && LWIP_NETIF_TX_SINGLE_PBUF */
#else /* LWIP_UDP */
        err = ERR_VAL;
#endif /* LWIP_UDP */
      }
      UNLOCK_TCPIP_CORE();

      pbuf_free(p);
    } else {
      err = ERR_MEM;
  }
#else /* LWIP_TCPIP_CORE_LOCKING */
  /* initialize a buffer */
  buf.p = buf.ptr = NULL;
#if LWIP_CHECKSUM_ON_COPY || PF_PKT_SUPPORT
  buf.flags = 0;
#endif /* LWIP_CHECKSUM_ON_COPY */
#if PF_PKT_SUPPORT
  buf.netifindex = 0;
#endif
  if (to_in != NULL ) {
    inet_addr_to_ipaddr(&buf.addr, &to_in->sin_addr);
    remote_port           = ntohs(to_in->sin_port);
    netbuf_fromport(&buf) = remote_port;
  } else {
#if PF_PKT_SUPPORT
    if (to_ll != NULL) {
      buf.flags |= NETBUF_FLAG_IFINDEX;
      buf.netifindex = (u8_t)to_ll->sll_ifindex;
    } else
#endif
    {
#if LWIP_UDP
      if (sock->conn->type == NETCONN_UDP){
        if (sock->conn->pcb.udp->flags & UDP_FLAGS_CONNECTED) {
          buf.addr = sock->conn->pcb.udp->remote_ip;
          remote_port           = sock->conn->pcb.udp->remote_port;
          netbuf_fromport(&buf) = remote_port;
        } else {
          LWIP_DEBUGF(SOCKETS_DEBUG, ("No address provide for UDP unconnect socket\n"));
          return ERR_NODEST;
        }
      }
#endif
#if LWIP_RAW
      if (sock->conn->type == NETCONN_RAW){
        if (sock->conn->pcb.raw->flags & RAW_FLAGS_PEER_ADDR_SET) {
          buf.addr = sock->conn->pcb.raw->remote_ip;
        } else {
          LWIP_DEBUGF(SOCKETS_DEBUG, ("No address provide for unconnect RAW socket\n"));
          return ERR_NODEST;
        }
      }
#endif
    }
  }

  LWIP_DEBUGF(SOCKETS_DEBUG, ("lwip_sendto(%d, data=%p, short_size=%"U16_F", flags=0x%x to=",
              s, data, short_size, flags));
#if PF_PKT_SUPPORT
  if (buf.flags & NETBUF_FLAG_IFINDEX) {
    LWIP_DEBUGF(SOCKETS_DEBUG, (" netifindex = %d\n", buf.netifindex));
  } else
#endif
  {
#ifdef  LWIP_DEV_DEBUG
    ip_addr_debug_print(SOCKETS_DEBUG, &buf.addr);
    LWIP_DEBUGF(SOCKETS_DEBUG, (" port=%"U16_F"\n", remote_port));
#endif
  }

  /* make the buffer point to the data that should be sent */
#if LWIP_NETIF_TX_SINGLE_PBUF
  /* assuming don't need ip frag */
  if((sock->conn->type == NETCONN_UDP && short_size <= ((1500 - PBUF_IP_HLEN) - UDP_HLEN))
#if PF_PKT_SUPPORT
    || (sock->conn->type == NETCONN_PKT_RAW && short_size <= 1500)
#endif
    || (sock->conn->type == NETCONN_RAW && short_size <= (1500 - PBUF_IP_HLEN))) {
    /* Allocate a new netbuf and copy the data into it. */
    if (netbuf_alloc(&buf, short_size) == NULL) {
      err = ERR_MEM;
    } else {
#if LWIP_CHECKSUM_ON_COPY
#if PF_PKT_SUPPORT
      if ((sock->conn->type != NETCONN_RAW) && (sock->conn->type != NETCONN_PKT_RAW)) {
#else
      if (sock->conn->type != NETCONN_RAW) {
#endif
        u16_t chksum = LWIP_CHKSUM_COPY(buf.p->payload, buf.p->len, data, short_size); //CSEC_FIX_2302
        netbuf_set_chksum(&buf, chksum);
        err = ERR_OK;
      } else
#endif /* LWIP_CHECKSUM_ON_COPY */
      {
        err = netbuf_take(&buf, data, short_size);
      }
    }
  } else
#endif /* LWIP_NETIF_TX_SINGLE_PBUF */
    err = netbuf_ref(&buf, data, short_size);
  if (err == ERR_OK) {
    /* send the data */
    err = netconn_send(sock->conn, &buf);
  }

  /* deallocated the buffer */
  netbuf_free(&buf);
#endif /* LWIP_TCPIP_CORE_LOCKING */
  return err;
}

int
lwip_sendto(int s, const void *data, size_t size, int flags,
       const struct sockaddr *to, socklen_t tolen)
{
  struct lwip_sock *sock;
  struct lwip_sock *isock;
  int err;
  int retval;
  size_t written;


  isock = get_socket_index(s);
  if(NULL == isock)
  {
    set_errno(EBADF);
    return -1;
  }

  lwip_sock_lock(isock);
  sock = get_socket_unsafe(s, LWIP_SOCKET_ACTIVE);
  if (NULL == sock)
  {
    lwip_sock_unlock(isock);
    set_errno(EBADF);
    return -1;
  }

  sock->events++;
  lwip_sock_unlock(isock);


   if (size > 65000) {
   	sock_set_errno(sock, EMSGSIZE);
	LWIP_SOCK_DECREMENT_EVENTS(sock);
	return -1;
	}

  if (sock->conn->type == NETCONN_TCP) {
#if LWIP_TCP
    retval = lwip_send_internal(s, data, size, flags, sock, &written);
    sock_set_errno(sock, err_to_errno(retval));
    LWIP_SOCK_DECREMENT_EVENTS(sock);

    return (retval == ERR_OK ? (int)written : -1);
#else /* LWIP_TCP */
    LWIP_UNUSED_ARG(flags);
    sock_set_errno(sock, err_to_errno(ERR_VAL));
    LWIP_SOCK_DECREMENT_EVENTS(sock);
    retval = -1;

    return retval;
#endif /* LWIP_TCP */
  }

  err = lwip_sendto_internal(s, data, size, flags, to, tolen, sock);
  sock_set_errno(sock, err_to_errno(err));
  LWIP_SOCK_DECREMENT_EVENTS(sock);
  return (err == ERR_OK ? size : (err == ERR_MEM ? 0 : -1));
}

int
lwip_socket(int domain, int type, int protocol)
{
  struct netconn *conn;
  int i;

#if PF_PKT_SUPPORT
  VALIDATE_DOMAIN_TYPE(domain,type)
  {
    LWIP_DEBUGF(SOCKETS_DEBUG, ("type(%d) not supported for PF_PACKET domain\n", type));
    set_errno(EPROTONOSUPPORT);
    return -1;
  }
#else
  LWIP_UNUSED_ARG(domain);
#endif
  LWIP_ERROR("domain invalid", ((domain == AF_INET) || (domain == AF_INET6) || (domain == AF_PACKET)),
  set_errno(EAFNOSUPPORT);return -1;);

  /* create a netconn */
  switch (type) {

#ifdef LWIP_RAW
  case SOCK_RAW:
#if PF_PKT_SUPPORT
    if(domain == PF_PACKET) {
      conn = netconn_new_with_proto_and_callback(NETCONN_PKT_RAW, (u16_t)protocol, event_callback);

      LWIP_DEBUGF(SOCKETS_DEBUG, ("lwip_socket(%s, SOCK_RAW, %d) = ", "PF_PACKET", protocol));
    } else {
      conn = netconn_new_with_proto_and_callback(NETCONN_RAW, (u16_t)protocol, event_callback);
      LWIP_DEBUGF(SOCKETS_DEBUG, ("lwip_socket(%s, SOCK_RAW, %d) = ",
                                 domain == PF_INET ? "PF_INET" : "UNKNOWN", protocol));
    }
#else
    conn = netconn_new_with_proto_and_callback(NETCONN_RAW, (u8_t)protocol, event_callback);
    LWIP_DEBUGF(SOCKETS_DEBUG, ("lwip_socket(%s, SOCK_RAW, %d) = ",
                               domain == PF_INET ? "PF_INET" : "UNKNOWN", protocol));
#endif
    break;
#endif

#ifdef LWIP_UDP
  case SOCK_DGRAM:
    LWIP_ERROR("protocol invalid", ((protocol == IPPROTO_IP || protocol == IPPROTO_UDPLITE) || (protocol == IPPROTO_UDP)),
        set_errno(EPROTONOSUPPORT);return -1;);

    conn = netconn_new_with_callback( (protocol == IPPROTO_UDPLITE) ?
                 NETCONN_UDPLITE : NETCONN_UDP, event_callback);
    LWIP_DEBUGF(SOCKETS_DEBUG, ("lwip_socket(%s, SOCK_DGRAM, %d) = ",
                                 domain == PF_INET ? "PF_INET" : "UNKNOWN", protocol));
    break;
#endif

#ifdef LWIP_TCP
  case SOCK_STREAM:
    LWIP_ERROR("protocol invalid", (protocol == IPPROTO_IP || protocol == IPPROTO_TCP), set_errno(EPROTONOSUPPORT);return -1;);
    conn = netconn_new_with_callback(NETCONN_TCP, event_callback);
    LWIP_DEBUGF(SOCKETS_DEBUG, ("lwip_socket(%s, SOCK_STREAM, %d) = ",
                                 domain == PF_INET ? "PF_INET" : "UNKNOWN", protocol));
    if (conn != NULL) {
      /* Prevent automatic window updates, we do this on our own! */
      netconn_set_noautorecved(conn);
    }
    break;
#endif

  default:
    LWIP_DEBUGF(SOCKETS_DEBUG, ("lwip_socket(%d, %d/UNKNOWN, %d) = -1\n",
                                 domain, type, protocol));
    set_errno(EINVAL);
    return -1;
  }

  if (!conn) {
    LWIP_DEBUGF(SOCKETS_DEBUG, ("-1 / ENOBUFS (could not create netconn)\n"));
    set_errno(ENOBUFS);
    return -1;
  }

  i = alloc_socket(conn, 0);

  if (i == -1) {
    netconn_delete(conn);
    set_errno(ENFILE);
    return -1;
  }
  conn->socket = i;
  LWIP_DEBUGF(SOCKETS_DEBUG, ("%d\n", i));
  set_errno(0);
  return i;
}

int
lwip_write(int s, const void *data, size_t size)
{
  return lwip_send(s, data, size, 0);
}

/**
 * Go through the readset and writeset lists and see which socket of the sockets
 * set in the sets has events. On return, readset, writeset and exceptset have
 * the sockets enabled that had events.
 *
 * exceptset is not used for now!!!
 *
 * @param maxfdp1 the highest socket index in the sets
 * @param readset_in:    set of sockets to check for read events
 * @param writeset_in:   set of sockets to check for write events
 * @param exceptset_in:  set of sockets to check for error events
 * @param readset_out:   set of sockets that had read events
 * @param writeset_out:  set of sockets that had write events
 * @param exceptset_out: set os sockets that had error events
 * @return number of sockets that had events (read/write/exception) (>= 0)
 */
LWIP_STATIC int
lwip_selscan(int maxfdp1, fd_set *readset_in, fd_set *writeset_in, fd_set *exceptset_in,
             fd_set *readset_out, fd_set *writeset_out, fd_set *exceptset_out)
{
  int i, nready = 0;
  fd_set lreadset, lwriteset, lexceptset;
  struct lwip_sock *sock;

  FD_ZERO(&lreadset);
  FD_ZERO(&lwriteset);
  FD_ZERO(&lexceptset);

  /* Go through each socket in each list to count number of sockets which
     currently match */
  for(i = LWIP_SOCKET_START_NUM; i < maxfdp1; i++) {
    void* lastdata = NULL;
    s16_t rcvevent = 0;
    u16_t sendevent = 0;
    u16_t errevent = 0;
    /* First get the socket's status (protected)... */
    sock = get_socket_index(i);
    if (sock != NULL) {
      sys_mutex_lock(&sock->mutex);
      if (sock->conn != NULL && sock->sock_flag != LWIP_SOCKET_INACTIVE){
        sys_mutex_lock(&sock->conn->conn_mutex);
        lastdata = sock->lastdata;
        rcvevent = sock->rcvevent;
        sendevent = sock->sendevent;
        errevent = sock->errevent;
        sys_mutex_unlock(&sock->conn->conn_mutex);
      }
      sys_mutex_unlock(&sock->mutex);
    }
    /* ... then examine it: */
    /* See if netconn of this socket is ready for read */
    if (readset_in && FD_ISSET(i, readset_in) && ((lastdata != NULL) || (rcvevent > 0))) {
      FD_SET(i, &lreadset);
      LWIP_DEBUGF(SOCKETS_DEBUG, ("lwip_selscan: fd=%d ready for reading\n", i));
      nready++;
    }
    /* See if netconn of this socket is ready for write */
    if (writeset_in && FD_ISSET(i, writeset_in) && (sendevent != 0)) {
      FD_SET(i, &lwriteset);
      LWIP_DEBUGF(SOCKETS_DEBUG, ("lwip_selscan: fd=%d ready for writing\n", i));
      nready++;
    }
    /* See if netconn of this socket had an error */
    if (exceptset_in && FD_ISSET(i, exceptset_in) && (errevent != 0)) {
      FD_SET(i, &lexceptset);
      LWIP_DEBUGF(SOCKETS_DEBUG, ("lwip_selscan: fd=%d ready for exception\n", i));
      nready++;
    }
  }
  /* copy local sets to the ones provided as arguments */
  (void)MEMCPY_S(readset_out, sizeof(fd_set), &lreadset, sizeof(fd_set));//CSEC_FIX_2302
  (void)MEMCPY_S(writeset_out, sizeof(fd_set), &lwriteset, sizeof(fd_set));//CSEC_FIX_2302
  (void)MEMCPY_S(exceptset_out, sizeof(fd_set), &lexceptset, sizeof(fd_set));//CSEC_FIX_2302

  LWIP_ASSERT("nready >= 0", nready >= 0);
  return nready;
}

/**
 * Processing exceptset is not yet implemented.
 */
int
lwip_select(int maxfdp1, fd_set *readset, fd_set *writeset, fd_set *exceptset,
            struct timeval *timeout)
{
  u32_t waitres = 0;
  int nready;
  fd_set lreadset, lwriteset, lexceptset;
  u32_t msectimeout;
  struct lwip_select_cb select_cb;
  err_t err;
  int i;
  struct lwip_sock *temp_sock = NULL;
  SYS_ARCH_DECL_PROTECT(lev);

  LWIP_DEBUGF(SOCKETS_DEBUG, ("lwip_select(%d, %p, %p, %p, tvsec=%"S32_F" tvusec=%"S32_F")\n",
                  maxfdp1, (void *)readset, (void *) writeset, (void *) exceptset,
                  timeout ? (s32_t)timeout->tv_sec : (s32_t)-1,
                  timeout ? (s32_t)timeout->tv_usec : (s32_t)-1));

  LWIP_ERROR("nfds is negative.", (maxfdp1 >= 0), set_errno(EINVAL);return -1;);

  i = LWIP_SOCKET_START_NUM;
  while (i < maxfdp1)
  {
      temp_sock = get_socket_index(i);
      if (NULL == temp_sock)
      {
        break;
      }
      sys_mutex_lock(&temp_sock->mutex);
      if (temp_sock->conn != NULL && temp_sock->sock_flag != LWIP_SOCKET_INACTIVE){
        sys_mutex_unlock(&temp_sock->mutex);
        break;
      }

      sys_mutex_unlock(&temp_sock->mutex);
      i++;
      temp_sock = NULL;
  }

  if (temp_sock == NULL) {
    LWIP_DEBUGF(SOCKETS_DEBUG, ("lwip_select: Invalid socket"));
    set_errno(EBADF);
    return -1;
  }

  /* Go through each socket in each list to count number of sockets which
     currently match */
  nready = lwip_selscan(maxfdp1, readset, writeset, exceptset, &lreadset, &lwriteset, &lexceptset);

  /* If we don't have any current events, then suspend if we are supposed to */
  if (!nready) {
    if (timeout && timeout->tv_sec == 0 && timeout->tv_usec == 0) {
      LWIP_DEBUGF(SOCKETS_DEBUG, ("lwip_select: no timeout, returning 0\n"));
      /* This is OK as the local fdsets are empty and nready is zero,
         or we would have returned earlier. */
      goto return_copy_fdsets;
    }

    /* None ready: add our semaphore to list:
       We don't actually need any dynamic memory. Our entry on the
       list is only valid while we are in this function, so it's ok
       to use local variables. */

    select_cb.next = NULL;
    select_cb.prev = NULL;
    select_cb.readset = readset;
    select_cb.writeset = writeset;
    select_cb.exceptset = exceptset;
    select_cb.sem_signalled = 0;
    err = sys_sem_new(&select_cb.sem, 0);
    if (err != ERR_OK) {
      /* failed to create semaphore */
      set_errno(ENOMEM);
      return -1;
    }

    /* Protect the select_cb_list */
    SYS_ARCH_PROTECT(lev);

    /* Put this select_cb on top of list */
    select_cb.next = select_cb_list;
    if (select_cb_list != NULL) {
      select_cb_list->prev = &select_cb;
    }

    /*PC Lint Error Fixed: Assigning address of auto variable 'select_cb' to static*/
    /* Retrieving the address assigned to static variable, no issue*/
    /*lint -e789*/
    select_cb_list = &select_cb;
    /*lint +e789*/
    /* Increasing this counter tells even_callback that the list has changed. */
    select_cb_ctr++;

    /* Now we can safely unprotect */
    SYS_ARCH_UNPROTECT(lev);

    /* Increase select_waiting for each socket we are interested in */
    for(i = LWIP_SOCKET_START_NUM; i < maxfdp1; i++) {
      if ((readset && FD_ISSET(i, readset)) ||
          (writeset && FD_ISSET(i, writeset)) ||
          (exceptset && FD_ISSET(i, exceptset))) {
        struct lwip_sock *sock;
        sock = get_socket_index(i);
        LWIP_ASSERT("Sock must not be NULL", (sock != NULL));
        sys_mutex_lock(&sock->mutex);
        if (sock->conn == NULL || sock->sock_flag == LWIP_SOCKET_INACTIVE){
          sys_mutex_unlock(&sock->mutex);
          continue;
        }

        sys_mutex_lock(&sock->conn->conn_mutex);
        sock->select_waiting++;
        LWIP_ASSERT("sock->select_waiting > 0", sock->select_waiting > 0);
        sys_mutex_unlock(&sock->conn->conn_mutex);
        sys_mutex_unlock(&sock->mutex);
      }
    }

    /* Call lwip_selscan again: there could have been events between
       the last scan (whithout us on the list) and putting us on the list! */
    nready = lwip_selscan(maxfdp1, readset, writeset, exceptset, &lreadset, &lwriteset, &lexceptset);
    if (!nready) {
      /* Still none ready, just wait to be woken */
      if (timeout == 0) {
        /* Wait forever */
        msectimeout = 0;
      } else {
        msectimeout =  (u32_t)(((timeout->tv_sec * 1000) + ((timeout->tv_usec + 500)/1000)));
        if (msectimeout == 0) {
          /* Wait 1ms at least (0 means wait forever) */
          msectimeout = 1;
        }
      }

      waitres = sys_arch_sem_wait(&select_cb.sem, msectimeout);
    }
    /* Increase select_waiting for each socket we are interested in */
    for(i = LWIP_SOCKET_START_NUM; i < maxfdp1; i++) {
      if ((readset && FD_ISSET(i, readset)) ||
          (writeset && FD_ISSET(i, writeset)) ||
          (exceptset && FD_ISSET(i, exceptset))) {
        struct lwip_sock *sock_tmp;
        sock_tmp = get_socket_index(i);
        LWIP_ASSERT("Sock must not be NULL", (sock_tmp != NULL));
        sys_mutex_lock(&sock_tmp->mutex);
        if (sock_tmp->conn == NULL || sock_tmp->sock_flag == LWIP_SOCKET_INACTIVE){
          sys_mutex_unlock(&sock_tmp->mutex);
          continue;
        }

        sys_mutex_lock(&sock_tmp->conn->conn_mutex);
        sock_tmp->select_waiting--;
        LWIP_ASSERT("sock->select_waiting >= 0", sock_tmp->select_waiting >= 0);
        sys_mutex_unlock(&sock_tmp->conn->conn_mutex);
        sys_mutex_unlock(&sock_tmp->mutex);
      }
    }
    /* Take us off the list */
    SYS_ARCH_PROTECT(lev);
    if (select_cb.next != NULL) {
      select_cb.next->prev = select_cb.prev;
    }
    if (select_cb_list == &select_cb) {
      LWIP_ASSERT("select_cb.prev == NULL", select_cb.prev == NULL);
      select_cb_list = select_cb.next;
    } else {
      LWIP_ASSERT("select_cb.prev != NULL", select_cb.prev != NULL);
      select_cb.prev->next = select_cb.next;
    }
    /* Increasing this counter tells even_callback that the list has changed. */
    select_cb_ctr++;
    SYS_ARCH_UNPROTECT(lev);

    sys_sem_free(&select_cb.sem);
    if (waitres == SYS_ARCH_TIMEOUT)  {
      /* Timeout */
      LWIP_DEBUGF(SOCKETS_DEBUG, ("lwip_select: timeout expired\n"));
      /* This is OK as the local fdsets are empty and nready is zero,
         or we would have returned earlier. */
      goto return_copy_fdsets;
    }

    /* See what's set */
    nready = lwip_selscan(maxfdp1, readset, writeset, exceptset, &lreadset, &lwriteset, &lexceptset);
  }

  LWIP_DEBUGF(SOCKETS_DEBUG, ("lwip_select: nready=%d\n", nready));
return_copy_fdsets:
  set_errno(0);
  if (readset) {
    (void)MEMCPY_S(readset, sizeof(fd_set), &lreadset, sizeof(fd_set));//CSEC_FIX_2302
  }
  if (writeset) {
    (void)MEMCPY_S(writeset, sizeof(fd_set), &lwriteset, sizeof(fd_set));//CSEC_FIX_2302
  }
  if (exceptset) {
    (void)MEMCPY_S(exceptset, sizeof(fd_set), &lexceptset, sizeof(fd_set));//CSEC_FIX_2302
  }


  return nready;
}

/**
 * Callback registered in the netconn layer for each socket-netconn.
 * Processes recvevent (data available) and wakes up tasks waiting for select.
 */
LWIP_STATIC void
event_callback(struct netconn *conn, enum netconn_evt evt, u16_t len)
{
  int s;
  struct lwip_sock *sock;
  struct lwip_select_cb *scb;
  int last_select_cb_ctr;
  SYS_ARCH_DECL_PROTECT(lev);

  LWIP_UNUSED_ARG(len);

  /* Get socket */
  if (conn) {
    s = conn->socket;
    if (s < 0) {
      /* Data comes in right away after an accept, even though
       * the server task might not have created a new socket yet.
       * Just count down (or up) if that's the case and we
       * will use the data later. Note that only receive events
       * can happen before the new socket is set up. */
      sys_mutex_lock(&conn->conn_mutex);
      if (conn->socket < 0) {
        if (evt == NETCONN_EVT_RCVPLUS) {
          conn->socket--;
        }
        sys_mutex_unlock(&conn->conn_mutex);
        return;
      }
      sys_mutex_unlock(&conn->conn_mutex);
    }

    sock = get_socket_index(s);
    if (NULL == sock) {
      return;
    }
  } else {
    return;
  }

  sys_mutex_lock(&sock->conn->conn_mutex);
  SYS_ARCH_PROTECT(lev);
  /* Set event as required */
  LWIP_UPDATE_EVENT_COUNT(evt, sock);
  if (sock->select_waiting == 0) {
    /* noone is waiting for this socket, no need to check select_cb_list */
    SYS_ARCH_UNPROTECT(lev);
    sys_mutex_unlock(&sock->conn->conn_mutex);

    return;
  }

  /* Now decide if anyone is waiting for this socket */
  /* NOTE: This code goes through the select_cb_list list multiple times
     ONLY IF a select was actually waiting. We go through the list the number
     of waiting select calls + 1. This list is expected to be small. */

  /* At this point, SYS_ARCH is still protected! */
again:
  for (scb = select_cb_list; scb != NULL; scb = scb->next) {
    if (scb->sem_signalled == 0) {
      /* semaphore not signalled yet */
      int do_signal = 0;
      /* Test this select call for our socket */
      LWIP_SOCKET_GET_SELECT_SIGNAL_EVENT(sock,scb,s, do_signal);
      if (do_signal) {
        scb->sem_signalled = 1;
        /* Don't call SYS_ARCH_UNPROTECT() before signaling the semaphore, as this might
           lead to the select thread taking itself off the list, invalidagin the semaphore. */
        sys_sem_signal(&scb->sem);
      }
    }
    /* unlock interrupts with each step */
    last_select_cb_ctr = select_cb_ctr;
    SYS_ARCH_UNPROTECT(lev);
    sys_mutex_unlock(&sock->conn->conn_mutex);



    sys_mutex_lock(&sock->conn->conn_mutex);
    /* this makes sure interrupt protection time is short */
    SYS_ARCH_PROTECT(lev);
    if (last_select_cb_ctr != select_cb_ctr) {
      /* someone has changed select_cb_list, restart at the beginning */
      goto again;
    }
  }
  SYS_ARCH_UNPROTECT(lev);
  sys_mutex_unlock(&sock->conn->conn_mutex);

}

/**
 * Unimplemented: Close one end of a full-duplex connection.
 * Currently, the full connection is closed.
 */
int
lwip_shutdown(int s, int how)
{
  struct lwip_sock *sock;
  struct lwip_sock *isock;
  err_t err;
  u8_t shut_rx = 0, shut_tx = 0;

  LWIP_DEBUGF(SOCKETS_DEBUG, ("lwip_shutdown(%d, how=%d)\n", s, how));

  isock = get_socket_index(s);
  if (NULL == isock)
  {
    set_errno(EBADF);
    return -1;
  }

  lwip_sock_lock(isock);
  sock = get_socket_unsafe(s, LWIP_SOCKET_ACTIVE);
  if(NULL == sock)
  {
    lwip_sock_unlock(isock);
    set_errno(EBADF);
    return -1;
  }
  sock->events++;
  lwip_sock_unlock(sock);

  if (netconn_type(sock->conn) != NETCONN_TCP) {
      LWIP_SOCK_DECREMENT_EVENTS(isock);
      sock_set_errno(sock, EOPNOTSUPP);
      return -1;
  }

  if (how == SHUT_RD) {
    shut_rx = 1;
  } else if (how == SHUT_WR) {
    shut_tx = 1;
  } else if(how == SHUT_RDWR) {
    shut_rx = 1;
    shut_tx = 1;
  } else {
    LWIP_SOCK_DECREMENT_EVENTS(isock);
    sock_set_errno(sock, EINVAL);
    return -1;
  }
  err = netconn_shutdown(sock->conn, shut_rx, shut_tx);
  LWIP_SOCK_DECREMENT_EVENTS(isock);
  sock_set_errno(sock, err_to_errno(err));
  return (err == ERR_OK ? 0 : -1);
}

LWIP_STATIC int
lwip_getaddrname(int s, struct sockaddr *name, socklen_t *namelen, u8_t local)
{
  struct lwip_sock *sock;
  struct lwip_sock *isock;
  struct sockaddr_in sin;
  err_t err;
  ip_addr_t naddr;
  int retval = 0;

  isock = get_socket_index(s);
  if (NULL == isock)
  {
    set_errno(EBADF);
    return -1;
  }
  lwip_sock_lock(isock);
  sock = get_socket_unsafe(s, LWIP_SOCKET_ACTIVE);
  if(NULL == sock)
  {
    lwip_sock_unlock(isock);
    set_errno(EBADF);
    return -1;
  }
#if PF_PKT_SUPPORT
  if (NETCONN_PKT_RAW == sock->conn->type)
  {
    lwip_sock_unlock(isock);
    set_errno(EOPNOTSUPP);
    return -1;
  }
#endif /* PF_PKT_SUPPORT */
  sock->events++;
  lwip_sock_unlock(isock);

  LWIP_ERROR("lwip_getaddrname: invalid arguments", ((name != NULL) && (namelen != NULL)),
    err = ERR_VAL; retval = -1; goto RETURN;);

  (void)MEMSET_S(&sin, sizeof(sin), 0, sizeof(sin)); //CSEC_FIX_2302
  sin.sin_family = AF_INET;

  lwip_sock_lock_op(sock);

  /* get the IP_add and port */
  err = netconn_getaddr(sock->conn, &naddr, &sin.sin_port, local);
  if(ERR_OK != err)
  {
    retval = -1;
    lwip_sock_unlock_op(sock);
    goto RETURN;
  }
  lwip_sock_unlock_op(sock);

  LWIP_DEBUGF(SOCKETS_DEBUG, ("lwip_getaddrname(%d, addr=", s));
 #ifdef  LWIP_DEV_DEBUG
  ip_addr_debug_print(SOCKETS_DEBUG, &naddr);
  LWIP_DEBUGF(SOCKETS_DEBUG, (" port=%"U16_F")\n", sin.sin_port));
 #endif
  sin.sin_port = htons(sin.sin_port);
  inet_addr_from_ipaddr(&sin.sin_addr, &naddr);

  if (*namelen > sizeof(sin)) {
    *namelen = sizeof(sin);
  }

  (void)MEMCPY_S(name, *namelen, &sin, *namelen);//CSEC_FIX_2302

RETURN:
  LWIP_SOCK_DECREMENT_EVENTS(sock);
  set_errno(err_to_errno(err));
  return  retval;
}

int
lwip_getpeername(int s, struct sockaddr *name, socklen_t *namelen)
{
  return lwip_getaddrname(s, name, namelen, 0);
}

int
lwip_getsockname(int s, struct sockaddr *name, socklen_t *namelen)
{
  return lwip_getaddrname(s, name, namelen, 1);
}


#ifdef LWIP_DEBUG
static err_t lwip_getsockopt_val_IPPROTO_SOL (int s, struct lwip_sock *sock,
                          socklen_t *optlen,  int optname)
#else
static err_t lwip_getsockopt_val_IPPROTO_SOL (struct lwip_sock *sock,
                          socklen_t *optlen,  int optname)
#endif
{
    err_t err = ERR_OK;
    switch (optname) {

    case SO_ACCEPTCONN:
    case SO_BROADCAST:
    /* UNIMPL case SO_DEBUG: */
    /* UNIMPL case SO_DONTROUTE: */
    case SO_KEEPALIVE:
    case SO_ERROR:
    /* SO_KEEPALIVE:UNIMPL case SO_CONTIMEO: */
#if LWIP_SO_RCVBUF
    case SO_RCVBUF:
#endif /* LWIP_SO_RCVBUF */
    /* SO_RCVBUF:UNIMPL case SO_OOBINLINE: */
    /* SO_RCVBUF:UNIMPL case SO_SNDBUF: */
    /* UNIMPL case SO_RCVLOWAT: */
    /* UNIMPL case SO_SNDLOWAT: */
#if SO_REUSE
    case SO_REUSEADDR:
    case SO_REUSEPORT:
#endif /* SO_REUSE */
    case SO_TYPE:
    /* UNIMPL case SO_USELOOPBACK: */
      if (*optlen < sizeof(int)) {
        err = EINVAL;
      }
      break;
#if LWIP_SO_SNDTIMEO
    case SO_SNDTIMEO:
      if (*optlen < sizeof(struct timeval)) {
        err = EINVAL;
      }
      break;
    /*SO_SNDTIMEO*/
#endif /* LWIP_SO_SNDTIMEO */
#if LWIP_SO_RCVTIMEO
    case SO_RCVTIMEO:
      if (*optlen < sizeof(struct timeval)) {
        err = EINVAL;
      }
      break;
#endif /* LWIP_SO_RCVTIMEO */
    case SO_NO_CHECK:
      if (*optlen < sizeof(int)) {
        err = EINVAL;
        break;
      }
#if LWIP_UDP
      if ((sock->conn->type != NETCONN_UDP) ||
          ((udp_flags(sock->conn->pcb.udp) & UDP_FLAGS_UDPLITE) != 0)) {
        /* this flag is only available for UDP, not for UDP lite */
        err = ENOPROTOOPT;
      }
#endif /* LWIP_UDP */
      break;

    default:
      LWIP_DEBUGF(SOCKETS_DEBUG, ("lwip_getsockopt(%d, SOL_SOCKET, UNIMPL: optname=0x%x, ..)\n",
                                  s, optname));
      err = ENOPROTOOPT;
      break;
    }  /* switch (optname) */

    return err;
}
#ifdef LWIP_DEBUG
static err_t lwip_getsockopt_val_IPPROTO_IP (int s, struct lwip_sock *sock,
                          socklen_t *optlen,  int optname)
#else
static err_t lwip_getsockopt_val_IPPROTO_IP(struct lwip_sock *sock,
                          socklen_t *optlen,  int optname)
#endif
{
    err_t err = ERR_OK;
#if !LWIP_IGMP
    LWIP_UNUSED_ARG(sock);
#endif

    switch (optname) {
    /* UNIMPL case IP_HDRINCL: */
    /* UNIMPL case IP_RCVDSTADDR: */
    /* UNIMPL case IP_RCVIF: */
    case IP_TTL:
    case IP_TOS:
      if (*optlen < sizeof(int)) {
        err = EINVAL;
      }
      break;
 #if LWIP_RAW
    case IP_HDRINCL:
      if (*optlen < sizeof(int)) {
        err = EINVAL;
      }
      break;
#endif
#if LWIP_IGMP
    case IP_MULTICAST_TTL:
      if (*optlen < sizeof(u8_t)) {
        err = EINVAL;
      }
      break;
    case IP_MULTICAST_IF:
      if (*optlen < sizeof(struct in_addr)) {
        err = EINVAL;
      }
      break;
    case IP_MULTICAST_LOOP:
      if (*optlen < sizeof(u8_t)) {
        err = EINVAL;
      }
      if (NETCONNTYPE_GROUP(sock->conn->type) != NETCONN_UDP) {
        err = ENOPROTOOPT;
      }
      break;
#endif /* LWIP_IGMP */

    default:
      LWIP_DEBUGF(SOCKETS_DEBUG, ("lwip_getsockopt(%d, IPPROTO_IP, UNIMPL: optname=0x%x, ..)\n",
                                  s, optname));
      err = ENOPROTOOPT;
      break;
    }  /* switch (optname) */
    return err;
}
#ifdef LWIP_DEBUG
static err_t lwip_getsockopt_val_IPPROTO_TCP (int s,int optname)
#else
static err_t lwip_getsockopt_val_IPPROTO_TCP(int optname)
#endif
{
    err_t err = ERR_OK;
    switch (optname) {
    case TCP_NODELAY:
    case TCP_KEEPALIVE:
#if LWIP_TCP_KEEPALIVE
    case TCP_KEEPIDLE:
    case TCP_KEEPINTVL:
    case TCP_KEEPCNT:
#endif /* LWIP_TCP_KEEPALIVE */
    case TCP_QUEUE_SEQ:
      break;

    default:
      LWIP_DEBUGF(SOCKETS_DEBUG, ("lwip_getsockopt(%d, IPPROTO_TCP, UNIMPL: optname=0x%x, ..)\n",
                                  s, optname));
      err = ENOPROTOOPT;
    }  /* switch (optname) */
    return err;
}
#if LWIP_UDP && LWIP_UDPLITE
#ifdef LWIP_DEBUG
static err_t lwip_getsockopt_val_IPPROTO_UDP_LITE (int s,int optname)
#else
static err_t lwip_getsockopt_val_IPPROTO_UDP_LITE(int optname)
#endif
{
    err_t err = ERR_OK;
       switch (optname) {
    case UDPLITE_SEND_CSCOV:
    case UDPLITE_RECV_CSCOV:
      break;

    default:
      LWIP_DEBUGF(SOCKETS_DEBUG, ("lwip_getsockopt(%d, IPPROTO_UDPLITE, UNIMPL: optname=0x%x, ..)\n",
                                  s, optname));
      err = ENOPROTOOPT;
    }  /* switch (optname) */
    return err;
}
#endif
int
lwip_getsockopt(int s, int level, int optname, void *optval, socklen_t *optlen)
{
  err_t err = ERR_OK;
  struct lwip_sock *sock;
  struct lwip_sock *isock;
  struct lwip_setgetsockopt_data getsockopt_data;

  isock = get_socket_index(s);
  if(NULL == isock) {
    set_errno(EBADF);
    return -1;
  }

  lwip_sock_lock(isock);
  sock = get_socket_unsafe(s, LWIP_SOCKET_ACTIVE);
  if(NULL == sock)
  {
    lwip_sock_unlock(isock);
    set_errno(EBADF);
    return -1;
  }
  if ((NULL == optval) || (NULL == optlen)) {
    sock_set_errno_unsafe(sock, EINVAL);
    lwip_sock_unlock(sock);
    set_errno(EINVAL);
    return -1;
  }

#if PF_PKT_SUPPORT
  VALIDATE_LEVEL_PF_PACKET(sock,level)
  {
    lwip_sock_unlock(sock);
    set_errno(EINVAL);
    return -1;
  }
#endif /* PF_PKT_SUPPORT  */

  sock->events++;

  /* Do length and type checks for the various options first, to keep it readable. */
  switch (level) {

/* Level: SOL_SOCKET */
  case SOL_SOCKET:
#if PF_PKT_SUPPORT
  case SOL_PACKET:
    VALIDATE_GET_PF_PKT_OPTNAME (s,level,optname,err);
#endif /* PF_PKT_SUPPORT  */
    VALIDATE_GET_RAW_OPTNAME(sock,optname,err);
#ifdef LWIP_DEBUG
    err = lwip_getsockopt_val_IPPROTO_SOL(s, sock, optlen, optname);
#else
    err = lwip_getsockopt_val_IPPROTO_SOL(sock, optlen, optname);
#endif
    break;

/* Level: IPPROTO_IP */
  case IPPROTO_IP:
#ifdef LWIP_DEBUG
    err = lwip_getsockopt_val_IPPROTO_IP(s, sock, optlen, optname);
#else
    err = lwip_getsockopt_val_IPPROTO_IP(sock, optlen, optname);
#endif
    break;
#if LWIP_TCP
/* Level: IPPROTO_TCP */
  case IPPROTO_TCP:
    if (*optlen < sizeof(int)) {
      err = EINVAL;
      break;
    }

    /* If this is no TCP socket, ignore any options. */
    if (sock->conn->type != NETCONN_TCP)
    {
      sock->events--;
      lwip_sock_unlock(sock);
      set_errno(ENOPROTOOPT);
      return -1;
    }
#ifdef LWIP_DEBUG
    err = lwip_getsockopt_val_IPPROTO_TCP(s,optname);
#else
    err = lwip_getsockopt_val_IPPROTO_TCP(optname);
#endif
    break;
#endif /* LWIP_TCP */
#if LWIP_UDP && LWIP_UDPLITE
/* Level: IPPROTO_UDPLITE */
  case IPPROTO_UDPLITE:
    if (*optlen < sizeof(int)) {
      err = EINVAL;
      break;
    }

    /* If this is no UDP lite socket, ignore any options. */
    if (sock->conn->type != NETCONN_UDPLITE) {
	  sock->events--;
      lwip_sock_unlock(sock);
      set_errno(ENOPROTOOPT);
      return -1;
    }
#ifdef LWIP_DEBUG
    err = lwip_getsockopt_val_IPPROTO_UDP_LITE(s,optname);
#else
    err = lwip_getsockopt_val_IPPROTO_UDP_LITE(optname);
#endif
    break;
#endif /* LWIP_UDP && LWIP_UDPLITE*/
/* UNDEFINED LEVEL */
  default:
      LWIP_DEBUGF(SOCKETS_DEBUG, ("lwip_getsockopt(%d, level=0x%x, UNIMPL: optname=0x%x, ..)\n",
                                  s, level, optname));
      err = ENOPROTOOPT;
  }  /* switch */


  if (err != ERR_OK) {
    sock_set_errno_unsafe(sock, err);
    sock->events--;
    lwip_sock_unlock(sock);
    set_errno(err);
    return -1;
  }

  lwip_sock_unlock(sock);

  /* Now do the actual option processing */
  getsockopt_data.sock = sock;
#ifdef LWIP_DEBUG
  getsockopt_data.s = s;
#endif /* LWIP_DEBUG */
  getsockopt_data.level = level;
  getsockopt_data.optname = optname;
  getsockopt_data.optval = optval;
  getsockopt_data.optlen = optlen;
  getsockopt_data.err = err;
  lwip_sock_lock_op(sock);

  err = tcpip_callback(lwip_getsockopt_internal, &getsockopt_data);

  if (err == ERR_OK) {
    (void)sys_arch_sem_wait(&sock->conn->op_completed, 0);
    /* maybe lwip_getsockopt_internal has changed err */
    err = getsockopt_data.err;
  }

  lwip_sock_unlock_op(sock);

  sock_set_errno(sock, err);
  LWIP_SOCK_DECREMENT_EVENTS(sock);

  return err ? -1 : 0;
}

#ifdef LWIP_DEBUG
static void lwip_getsockopt_SO_TYPE (int s, struct lwip_sock *sock,
                          const void *optval)
#else
static void lwip_getsockopt_SO_TYPE(struct lwip_sock *sock,
                          const void *optval)
#endif
{
     switch (NETCONNTYPE_GROUP(sock->conn->type)) {
#if LWIP_RAW
#if PF_PKT_SUPPORT
      case NETCONN_PKT_RAW:
#endif
      case NETCONN_RAW:
        *(int*)optval = SOCK_RAW;
        break;
#endif
#if LWIP_TCP
      case NETCONN_TCP:
        *(int*)optval = SOCK_STREAM;
        break;
#endif
#if LWIP_UDP
      case NETCONN_UDP:
        *(int*)optval = SOCK_DGRAM;
        break;
#endif
      default: /* unrecognized socket type */
        *(int*)optval = sock->conn->type;
#if PF_PKT_SUPPORT
        LWIP_DEBUGF(SOCKETS_DEBUG,
                   ("lwip_getsockopt(%d, %s, SO_TYPE): unrecognized socket type %d\n",
                     s, (NETCONN_PKT_RAW != NETCONNTYPE_GROUP(sock->conn->type)) ? "SOL_SOCKET":"SOL_PACKET",
                    *(int *)optval));
#else
        LWIP_DEBUGF(SOCKETS_DEBUG,
                    ("lwip_getsockopt(%d, SOL_SOCKET, SO_TYPE): unrecognized socket type %d\n",
                    s, *(int *)optval));
#endif
      }
      LWIP_DEBUGF(SOCKETS_DEBUG, ("lwip_getsockopt(%d, SOL_SOCKET, SO_TYPE) = %d\n",
            s, *(int *)optval));
}

#define lwip_getsockopt_setoptval(optval_tmp,optval)   if (optval_tmp) {\
                                                                                             *(int *)optval = 1;\
                                                                                            } else {\
                                                                                               *(int *)optval = 0;\
                                                                                            }\

#ifdef LWIP_DEBUG
static void lwip_getsockopt_level_SOL (int s, struct lwip_sock *sock,
                          const void *optval,  int optname)
#else
static void lwip_getsockopt_level_SOL (struct lwip_sock *sock,
                          const void *optval,  int optname)
#endif
{
#if LWIP_SO_SNDTIMEO || LWIP_SO_RCVTIMEO
    struct timeval *tv;
#endif/* LWIP_SO_SNDTIMEO || LWIP_SO_RCVTIMEO*/
    int optval_tmp;
    switch (optname) {

    /* The option flags */
    case SO_ACCEPTCONN:
      optval_tmp = (int)ip_get_option(sock->conn->pcb.ip, SOF_ACCEPTCONN);
      LWIP_DEBUGF(SOCKETS_DEBUG, ("lwip_getsockopt(%d, SOL_SOCKET, optname=0x%x, ..) = %s\n",
                                  s, SOF_ACCEPTCONN, (optval_tmp?"on":"off")));
      LWIP_UPDATE_OPTVAL(optval_tmp,optval);
      break;
    case SO_BROADCAST:
      optval_tmp = (int)ip_get_option(sock->conn->pcb.ip, SOF_BROADCAST);
      LWIP_DEBUGF(SOCKETS_DEBUG, ("lwip_getsockopt(%d, SOL_SOCKET, optname=0x%x, ..) = %s\n",
                                  s, SOF_BROADCAST, (optval_tmp?"on":"off")));
      LWIP_UPDATE_OPTVAL(optval_tmp,optval);
      break;
    /* UNIMPL case SO_DEBUG: */
    /* UNIMPL case SO_DONTROUTE: */
    case SO_KEEPALIVE:
      optval_tmp = (int)ip_get_option(sock->conn->pcb.ip, SOF_KEEPALIVE);
      LWIP_DEBUGF(SOCKETS_DEBUG, ("lwip_getsockopt(%d, SOL_SOCKET, optname=0x%x, ..) = %s\n",
                                  s, SOF_KEEPALIVE, (optval_tmp?"on":"off")));
      LWIP_UPDATE_OPTVAL(optval_tmp,optval);
      break;
    /* UNIMPL case SO_OOBINCLUDE: */
#if SO_REUSE
    case SO_REUSEADDR:
      optval_tmp = (int)ip_get_option(sock->conn->pcb.ip, SOF_REUSEADDR);
      LWIP_DEBUGF(SOCKETS_DEBUG, ("lwip_getsockopt(%d, SOL_SOCKET, optname=0x%x, ..) = %s\n",
                                  s, SOF_REUSEADDR, (optval_tmp?"on":"off")));
      LWIP_UPDATE_OPTVAL(optval_tmp,optval);
      break;
    case SO_REUSEPORT:
      optval_tmp = (int)ip_get_option(sock->conn->pcb.ip, SOF_REUSEPORT);
      LWIP_DEBUGF(SOCKETS_DEBUG, ("lwip_getsockopt(%d, SOL_SOCKET, optname=0x%x, ..) = %s\n",
                                  s, SOF_REUSEPORT, (optval_tmp?"on":"off")));
      LWIP_UPDATE_OPTVAL(optval_tmp,optval);
      break;
#endif /* SO_REUSE */
    /*case SO_USELOOPBACK: UNIMPL */

    case SO_TYPE:
#ifdef LWIP_DEBUG
    lwip_getsockopt_SO_TYPE(s, sock, optval);
#else
    lwip_getsockopt_SO_TYPE(sock, optval);
#endif
      break;

    case SO_ERROR:
      /* only overwrite ERR_OK or tempoary errors */
      LWIP_SET_SO_ERROR(sock,optval);
      LWIP_DEBUGF(SOCKETS_DEBUG, ("lwip_getsockopt(%d, SOL_SOCKET, SO_ERROR) = %d\n",
                  s, *(int *)optval));
      break;

#if LWIP_SO_SNDTIMEO
    case SO_SNDTIMEO:
      tv = (struct timeval *)optval;
      tv->tv_sec = netconn_get_sendtimeout(sock->conn)/1000;
      tv->tv_usec = (netconn_get_sendtimeout(sock->conn)%1000)*1000;
      break;
#endif /* LWIP_SO_SNDTIMEO */
#if LWIP_SO_RCVTIMEO
    case SO_RCVTIMEO:
      tv = (struct timeval *)optval;
      tv->tv_sec = netconn_get_recvtimeout(sock->conn)/1000;
      tv->tv_usec = (netconn_get_recvtimeout(sock->conn)%1000)*1000;
      break;
#endif /* LWIP_SO_RCVTIMEO */
#if LWIP_SO_RCVBUF
    case SO_RCVBUF:
      *(int *)optval = netconn_get_recvbufsize(sock->conn);
      break;
#endif /* LWIP_SO_RCVBUF */
#if LWIP_UDP
    case SO_NO_CHECK:
      *(int*)optval = (udp_flags(sock->conn->pcb.udp) & UDP_FLAGS_NOCHKSUM) ? 1 : 0;
      break;
#endif /* LWIP_UDP*/
    default:
      LWIP_ASSERT("unhandled optname", 0);
      break;
        }

}
#ifdef LWIP_DEBUG
static void lwip_getsockopt_level_IPPROTO_IP (int s, struct lwip_sock *sock,
                          const void *optval,  int optname)
#else
static void lwip_getsockopt_level_IPPROTO_IP (struct lwip_sock *sock,
                          const void *optval,  int optname)
#endif
{
 switch (optname) {
    case IP_TTL:
      *(int*)optval = sock->conn->pcb.ip->ttl;
      LWIP_DEBUGF(SOCKETS_DEBUG, ("lwip_getsockopt(%d, IPPROTO_IP, IP_TTL) = %d\n",
                  s, *(int *)optval));
      break;
#if LWIP_RAW
    case IP_HDRINCL:
      *(int*)optval = sock->conn->pcb.raw->hdrincl;
      LWIP_DEBUGF(SOCKETS_DEBUG, ("lwip_getsockopt(%d, IP_HDRINCL) = %d\n",
                  s, *(int *)optval));
      break;
#endif
    case IP_TOS:
      *(int*)optval = sock->conn->pcb.ip->tos;
      LWIP_DEBUGF(SOCKETS_DEBUG, ("lwip_getsockopt(%d, IPPROTO_IP, IP_TOS) = %d\n",
                  s, *(int *)optval));
      break;
#if LWIP_IGMP
    case IP_MULTICAST_TTL:
      *(u8_t*)optval = sock->conn->pcb.ip->ttl;
      LWIP_DEBUGF(SOCKETS_DEBUG, ("lwip_getsockopt(%d, IPPROTO_IP, IP_MULTICAST_TTL) = %d\n",
                  s, *(int *)optval));
      break;
    case IP_MULTICAST_IF:
      inet_addr_from_ipaddr((struct in_addr*)optval, &sock->conn->pcb.udp->multicast_ip);
      LWIP_DEBUGF(SOCKETS_DEBUG, ("lwip_getsockopt(%d, IPPROTO_IP, IP_MULTICAST_IF) = 0x%"X32_F"\n",
                  s, *(u32_t *)optval));
      break;
    case IP_MULTICAST_LOOP:
      if ((sock->conn->pcb.udp->flags & UDP_FLAGS_MULTICAST_LOOP) != 0) {
        *(u8_t*)optval = 1;
      } else {
        *(u8_t*)optval = 0;
      }
      LWIP_DEBUGF(SOCKETS_DEBUG, ("lwip_getsockopt(%d, IPPROTO_IP, IP_MULTICAST_LOOP) = %d\n",
                  s, *(int *)optval));
      break;
#endif /* LWIP_IGMP */
    default:
      LWIP_ASSERT("getsockopt unhandled optname", 0);
      break;
    }
}
#ifdef LWIP_DEBUG
static void lwip_getsockopt_level_IPPROTO_TCP (int s, struct lwip_sock *sock,
                          const void *optval,  int optname)
#else
static void lwip_getsockopt_level_IPPROTO_TCP (struct lwip_sock *sock,
                          const void *optval,  int optname)
#endif
{
    switch (optname) {
    case TCP_NODELAY:
      *(int*)optval = tcp_nagle_disabled(sock->conn->pcb.tcp);
      LWIP_DEBUGF(SOCKETS_DEBUG, ("lwip_getsockopt(%d, IPPROTO_TCP, TCP_NODELAY) = %s\n",
                  s, (*(int*)optval)?"on":"off") );
      break;
    case TCP_KEEPALIVE:
      *(int*)optval = (int)sock->conn->pcb.tcp->keep_idle;
      LWIP_DEBUGF(SOCKETS_DEBUG, ("lwip_getsockopt(%d, IPPROTO_IP, TCP_KEEPALIVE) = %d\n",
                  s, *(int *)optval));
      break;

#if LWIP_TCP_KEEPALIVE
    case TCP_KEEPIDLE:
      *(int*)optval = (int)(sock->conn->pcb.tcp->keep_idle/1000);
      LWIP_DEBUGF(SOCKETS_DEBUG, ("lwip_getsockopt(%d, IPPROTO_IP, TCP_KEEPIDLE) = %d\n",
                  s, *(int *)optval));
      break;
    case TCP_KEEPINTVL:
      *(int*)optval = (int)(sock->conn->pcb.tcp->keep_intvl/1000);
      LWIP_DEBUGF(SOCKETS_DEBUG, ("lwip_getsockopt(%d, IPPROTO_IP, TCP_KEEPINTVL) = %d\n",
                  s, *(int *)optval));
      break;
    case TCP_KEEPCNT:
      *(int*)optval = (int)sock->conn->pcb.tcp->keep_cnt;
      LWIP_DEBUGF(SOCKETS_DEBUG, ("lwip_getsockopt(%d, IPPROTO_IP, TCP_KEEPCNT) = %d\n",
                  s, *(int *)optval));
      break;
#endif /* LWIP_TCP_KEEPALIVE */
    case TCP_QUEUE_SEQ:
      if(*(unsigned int*)optval == TCP_RECV_QUEUE)
      {
        *(unsigned int*)optval = sock->conn->pcb.tcp->rcv_nxt;
      }
      else if(*(int*)optval == TCP_SEND_QUEUE)
      {
        *(unsigned int*)optval = sock->conn->pcb.tcp->snd_nxt;
      }
      LWIP_DEBUGF(SOCKETS_DEBUG, ("lwip_getsockopt(%d, IPPROTO_TCP, TCP_QUEUE_SEQ) = %d\n",
                  s, *(int *)optval));
      break;
    default:
      LWIP_ASSERT("unhandled optname", 0);
      break;
    }
}
#if LWIP_UDP && LWIP_UDPLITE
#ifdef LWIP_DEBUG
static void lwip_getsockopt_level_IPPROTO_UDP_LITE (int s, struct lwip_sock *sock,
                          const void *optval,  int optname)
#else
static void lwip_getsockopt_level_IPPROTO_UDP_LITE (struct lwip_sock *sock,
                          const void *optval,  int optname)
#endif
{
    switch (optname) {
    case UDPLITE_SEND_CSCOV:
      *(int*)optval = sock->conn->pcb.udp->chksum_len_tx;
      LWIP_DEBUGF(SOCKETS_DEBUG, ("lwip_getsockopt(%d, IPPROTO_UDPLITE, UDPLITE_SEND_CSCOV) = %d\n",
                  s, (*(int*)optval)) );
      break;
    case UDPLITE_RECV_CSCOV:
      *(int*)optval = sock->conn->pcb.udp->chksum_len_rx;
      LWIP_DEBUGF(SOCKETS_DEBUG, ("lwip_getsockopt(%d, IPPROTO_UDPLITE, UDPLITE_RECV_CSCOV) = %d\n",
                  s, (*(int*)optval)) );
      break;
    default:
      LWIP_ASSERT("getsockopt_internal unhandled optname", 0);
      break;
    }  /* switch (optname) */
}
#endif
LWIP_STATIC void
lwip_getsockopt_internal(void *arg)
{
  struct lwip_sock *sock;
#ifdef LWIP_DEBUG
  int s;
#endif /* LWIP_DEBUG */
  int level, optname;
  void *optval;
  struct lwip_setgetsockopt_data *getsockopt_data;

  LWIP_ASSERT("arg != NULL", arg != NULL);

  getsockopt_data = (struct lwip_setgetsockopt_data*)arg;
  sock = getsockopt_data->sock;

#ifdef LWIP_DEBUG
  s = getsockopt_data->s;
#endif /* LWIP_DEBUG */
  level = getsockopt_data->level;
  optname = getsockopt_data->optname;
  optval = getsockopt_data->optval;
  LWIP_ASSERT("Sock connection must not be NULL", (sock->conn != NULL));

  /*COVERITY: Checking the val for -ve to make sure non -ve ness */
  if(LWIP_SOCKET_CLOSING == sock->sock_flag || optname < 0)
  {
  	getsockopt_data->err = EINVAL;
    sys_sem_signal(&sock->conn->op_completed);
    return;
  }

  switch (level) {

/* Level: SOL_SOCKET */
  case SOL_SOCKET:
#if PF_PKT_SUPPORT
  case SOL_PACKET:
#endif
#ifdef LWIP_DEBUG
    lwip_getsockopt_level_SOL(s, sock, optval, optname);
#else
    lwip_getsockopt_level_SOL(sock, optval, optname);
#endif
    break;

/* Level: IPPROTO_IP */
  case IPPROTO_IP:
#ifdef LWIP_DEBUG
    lwip_getsockopt_level_IPPROTO_IP(s, sock, optval, optname);
#else
    lwip_getsockopt_level_IPPROTO_IP(sock, optval, optname);
#endif
    break;
#if LWIP_TCP
/* Level: IPPROTO_TCP */
  case IPPROTO_TCP:
#ifdef LWIP_DEBUG
    lwip_getsockopt_level_IPPROTO_TCP(s, sock, optval, optname);
#else
    lwip_getsockopt_level_IPPROTO_TCP(sock, optval, optname);
#endif
    break;
#endif /* LWIP_TCP */
#if LWIP_UDP && LWIP_UDPLITE
  /* Level: IPPROTO_UDPLITE */
  case IPPROTO_UDPLITE:
#ifdef LWIP_DEBUG
    lwip_getsockopt_level_IPPROTO_UDP_LITE(s, sock, optval, optname);
#else
    lwip_getsockopt_level_IPPROTO_UDP_LITE(sock, optval, optname);
#endif
    break;
#endif /* LWIP_UDP */
  default:
    getsockopt_data->err = ENOPROTOOPT;
    LWIP_ASSERT("getsockopt_internal unhandled level", 0);
    break;
  }
  sys_sem_signal(&sock->conn->op_completed);
}
#ifdef LWIP_DEBUG
static err_t lwip_setsockopt_val_SOL (int s, struct lwip_sock *sock,
                          socklen_t optlen,  int optname)
#else
static err_t lwip_setsockopt_val_SOL(struct lwip_sock *sock,
                          socklen_t optlen,  int optname)
#endif
{
    err_t err = ERR_OK;
    switch (optname) {

    case SO_BROADCAST:
    /* UNIMPL case SO_DEBUG: */
    /* UNIMPL case SO_DONTROUTE: */
    case SO_KEEPALIVE:
    /* UNIMPL case case SO_CONTIMEO: */
#if LWIP_SO_RCVBUF
    case SO_RCVBUF:
#endif /* LWIP_SO_RCVBUF */
    /* UNIMPL case SO_OOBINLINE: */
    /* UNIMPL case SO_SNDBUF: */
    /* UNIMPL case SO_RCVLOWAT: */
    /* UNIMPL case SO_SNDLOWAT: */
#if SO_REUSE
    case SO_REUSEPORT:
    case SO_REUSEADDR:
#endif /* SO_REUSE */
    /* UNIMPL case SO_USELOOPBACK: */
      if (optlen < sizeof(int)) {
        err = EINVAL;
      }
      break;
#if LWIP_SO_SNDTIMEO
    case SO_SNDTIMEO:
	/* UNIMPL case SO_USELOOPBACK: */
	if (optlen < sizeof(struct timeval)) {
		err = EINVAL;
		}
	break;
#endif /* LWIP_SO_SNDTIMEO */
#if LWIP_SO_RCVTIMEO
	case SO_RCVTIMEO:
		/* UNIMPL case SO_USELOOPBACK: */
		if (optlen < sizeof(struct timeval)) {
			err = EINVAL;
			}
		break;
#endif /* LWIP_SO_RCVTIMEO */

    case SO_NO_CHECK:
      if (optlen < sizeof(int)) {
        err = EINVAL;
        break;
      }
#if LWIP_UDP
      if ((sock->conn->type != NETCONN_UDP) ||
          ((udp_flags(sock->conn->pcb.udp) & UDP_FLAGS_UDPLITE) != 0)) {
        /* this flag is only available for UDP, not for UDP lite */
        err = ENOPROTOOPT;
      }
#endif /* LWIP_UDP */
      break;
    default:
      LWIP_DEBUGF(SOCKETS_DEBUG, ("lwip_setsockopt(%d, SOL_SOCKET, UNIMPL: optname=0x%x, ..)\n",
                  s, optname));
      err = ENOPROTOOPT;
    }  /* switch (optname) */
    return err;
}
#ifdef LWIP_DEBUG
static err_t lwip_setsockopt_val_IPPROTO_IP (int s, struct lwip_sock *sock,
                          socklen_t optlen,  int optname)
#else
static err_t lwip_setsockopt_val_IPPROTO_IP(struct lwip_sock *sock,
                          socklen_t optlen,  int optname)
#endif
{
    err_t err = ERR_OK;
#if !LWIP_IGMP
    LWIP_UNUSED_ARG(sock);
#endif

    switch (optname) {
    /* UNIMPL case IP_HDRINCL: */
    /* UNIMPL case IP_RCVDSTADDR: */
    /* UNIMPL case IP_RCVIF: */
    case IP_TTL:
    case IP_TOS:
 #if LWIP_RAW
    case IP_HDRINCL:
#endif
      if (optlen < sizeof(int)) {
        err = EINVAL;
      }
      break;

#if LWIP_IGMP
    case IP_MULTICAST_TTL:
    case IP_MULTICAST_LOOP:
      if (optlen < sizeof(u8_t)) {
        err = EINVAL;
      }
      if (NETCONNTYPE_GROUP(sock->conn->type) != NETCONN_UDP) {
        err = ENOPROTOOPT;
      }
      break;
    case IP_MULTICAST_IF:
      if (optlen < sizeof(struct in_addr)) {
        err = EINVAL;
      }
      if (NETCONNTYPE_GROUP(sock->conn->type) != NETCONN_UDP) {
        err = ENOPROTOOPT;
      }
      break;
    case IP_ADD_MEMBERSHIP:
    case IP_DROP_MEMBERSHIP:
      if (optlen < sizeof(struct ip_mreq)) {
        err = EINVAL;
      }
      if (NETCONNTYPE_GROUP(sock->conn->type) != NETCONN_UDP) {
        err = ENOPROTOOPT;
      }
      break;
#endif /* LWIP_IGMP */
      default:
        LWIP_DEBUGF(SOCKETS_DEBUG, ("lwip_setsockopt(%d, IPPROTO_IP, UNIMPL: optname=0x%x, ..)\n",
                    s, optname));
        err = ENOPROTOOPT;
    }
    return err;
}
#ifdef LWIP_DEBUG
static err_t  lwip_setsockopt_val_IPPROTO_TCP (int s,int optname)
#else
static err_t  lwip_setsockopt_val_IPPROTO_TCP (int  optname)
#endif
{
    err_t err = ERR_OK;
    switch (optname) {
    case TCP_NODELAY:
    case TCP_KEEPALIVE:
#if LWIP_TCP_KEEPALIVE
    case TCP_KEEPINTVL:
    case TCP_KEEPIDLE:
    case TCP_KEEPCNT:
#endif /* LWIP_TCP_KEEPALIVE */
      break;

    default:
      LWIP_DEBUGF(SOCKETS_DEBUG, ("lwip_setsockopt(%d, IPPROTO_TCP, UNIMPL: optname=0x%x, ..)\n",
                  s, optname));
      err = ENOPROTOOPT;
    }
    return err;
}
#if LWIP_UDP && LWIP_UDPLITE
#ifdef LWIP_DEBUG
static err_t lwip_setsockopt_val_UDP_LITE (int s,int optname)
#else
static err_t lwip_setsockopt_val_UDP_LITE (int optname)
#endif
{
    err_t err = ERR_OK;
    switch (optname) {
    case UDPLITE_SEND_CSCOV:
    case UDPLITE_RECV_CSCOV:
      break;

    default:
      LWIP_DEBUGF(SOCKETS_DEBUG, ("lwip_setsockopt(%d, IPPROTO_UDPLITE, UNIMPL: optname=0x%x, ..)\n",
                  s, optname));
      err = ENOPROTOOPT;
    }
    return err;
}
#endif
int
lwip_setsockopt(int s, int level, int optname, const void *optval, socklen_t optlen)
{
  struct lwip_sock *sock;
  err_t err = ERR_OK;
  struct lwip_setgetsockopt_data data;

  struct lwip_sock *isock;


  if(NULL == optval)
  {
	set_errno(EINVAL);
    return -1;
  }

  isock = get_socket_index(s);
  if(NULL == isock) {
    set_errno(EBADF);
    return -1;
  }

  lwip_sock_lock(isock);
  sock = get_socket_unsafe(s, LWIP_SOCKET_ACTIVE);
  if(NULL == sock)
  {
    lwip_sock_unlock(isock);
    set_errno(EBADF);
    return -1;
  }
#if PF_PKT_SUPPORT
  VALIDATE_LEVEL_PF_PACKET(sock,level)
  {
    lwip_sock_unlock(sock);
    set_errno(EINVAL);
    return -1;
  }
#endif /* PF_PKT_SUPPORT  */

  sock->events++;

  /* Do length and type checks for the various options first, to keep it readable. */
  switch (level) {

/* Level: SOL_SOCKET */
  case SOL_SOCKET:
#if PF_PKT_SUPPORT
  case SOL_PACKET:
    VALIDATE_SET_PF_PKT_OPTNAME (s,level,optname,err);
#endif /* PF_PKT_SUPPORT */
   VALIDATE_SET_RAW_OPTNAME (sock,optname,err);
#ifdef LWIP_DEBUG
    err = lwip_setsockopt_val_SOL(s, sock, optlen, optname);
#else
    err = lwip_setsockopt_val_SOL(sock, optlen, optname);
#endif
    break;

/* Level: IPPROTO_IP */
  case IPPROTO_IP:
#ifdef LWIP_DEBUG
    err = lwip_setsockopt_val_IPPROTO_IP(s, sock, optlen, optname);
#else
    err = lwip_setsockopt_val_IPPROTO_IP(sock, optlen, optname);
#endif
    break;

#if LWIP_TCP
/* Level: IPPROTO_TCP */
  case IPPROTO_TCP:
    if (optlen < sizeof(int)) {
      err = EINVAL;
      break;
    }

    /* If this is no TCP socket, ignore any options. */
    if (sock->conn->type != NETCONN_TCP)
	{
	  sock->events--;
      lwip_sock_unlock(isock);
      set_errno(ENOPROTOOPT);
      return -1;
    }
#ifdef LWIP_DEBUG
    err = lwip_setsockopt_val_IPPROTO_TCP(s,optname);
#else
    err = lwip_setsockopt_val_IPPROTO_TCP(optname);
#endif
    break;
#endif /* LWIP_TCP */
#if LWIP_UDP && LWIP_UDPLITE
/* Level: IPPROTO_UDPLITE */
  case IPPROTO_UDPLITE:
    if (optlen < sizeof(int)) {
      err = EINVAL;
      break;
    }

    /* If this is no UDP lite socket, ignore any options. */
    if (sock->conn->type != NETCONN_UDPLITE)
	{
	  sock->events--;
      lwip_sock_unlock(isock);
      set_errno(ENOPROTOOPT);
      return -1;
    }
#ifdef LWIP_DEBUG
    err = lwip_setsockopt_val_UDP_LITE(s,optname);
#else
    err = lwip_setsockopt_val_UDP_LITE(optname);
#endif
    break;
#endif /* LWIP_UDP && LWIP_UDPLITE */
/* UNDEFINED LEVEL */
  default:
    LWIP_DEBUGF(SOCKETS_DEBUG, ("lwip_setsockopt(%d, level=0x%x, UNIMPL: optname=0x%x, ..)\n",
                s, level, optname));
    err = ENOPROTOOPT;
  }


  if (err != ERR_OK) {
    sock->events--;
    sock_set_errno_unsafe(sock, err);
    lwip_sock_unlock(sock);
    set_errno(err);
    return -1;
  }

  lwip_sock_unlock(sock);
  /* Now do the actual option processing */
  data.sock = sock;
#ifdef LWIP_DEBUG
  data.s = s;
#endif /* LWIP_DEBUG */
  data.level = level;
  data.optname = optname;
  data.optval = (void*)optval;
  data.optlen = &optlen;
  data.err = err;
  lwip_sock_lock_op(sock);

  err = tcpip_callback(lwip_setsockopt_internal, &data);

  if (err == ERR_OK) {
    (void)sys_arch_sem_wait(&sock->conn->op_completed, 0);
    /* maybe lwip_getsockopt_internal has changed err */
    err = data.err;
  }

  lwip_sock_unlock_op(sock);

  sock_set_errno(sock, err);
  LWIP_SOCK_DECREMENT_EVENTS(isock);

  return err ? -1 : 0;
}

#ifdef LWIP_DEBUG
static void lwip_setsockopt_level_SOL (int s, struct lwip_sock *sock,  struct lwip_setgetsockopt_data *data,
                          const void *optval,  int optname)
#else
static void lwip_setsockopt_level_SOL (struct lwip_sock *sock,  struct lwip_setgetsockopt_data *data,
                          const void *optval,  int optname)
#endif
{
	struct timeval *tv;
    switch(optname){
    /* The option flags */
    case SO_BROADCAST:
      if (*(int*)optval) {
        ip_set_option(sock->conn->pcb.ip, SOF_BROADCAST);
      } else {
        ip_reset_option(sock->conn->pcb.ip, SOF_BROADCAST);
      }
      LWIP_DEBUGF(SOCKETS_DEBUG, ("lwip_setsockopt(%d, SOL_SOCKET, optname=0x%x, ..) -> %s\n",
                  s, SOF_BROADCAST, (*(int*)optval?"on":"off")));
      break;
    /* UNIMPL case SO_DEBUG: */
    /* UNIMPL case SO_DONTROUTE: */
    case SO_KEEPALIVE:
      if (*(int*)optval) {
        ip_set_option(sock->conn->pcb.ip, SOF_KEEPALIVE);
      } else {
        ip_reset_option(sock->conn->pcb.ip, SOF_KEEPALIVE);
      }
      LWIP_DEBUGF(SOCKETS_DEBUG, ("lwip_setsockopt(%d, SOL_SOCKET, optname=0x%x, ..) -> %s\n",
                  s, SOF_KEEPALIVE, (*(int*)optval?"on":"off")));
      break;
    /* UNIMPL case SO_OOBINCLUDE: */
#if SO_REUSE
    case SO_REUSEADDR:
      if (*(int*)optval) {
        ip_set_option(sock->conn->pcb.ip, SOF_REUSEADDR);
      } else {
        ip_reset_option(sock->conn->pcb.ip, SOF_REUSEADDR);
      }
      LWIP_DEBUGF(SOCKETS_DEBUG, ("lwip_setsockopt(%d, SOL_SOCKET, optname=0x%x, ..) -> %s\n",
                  s, SOF_REUSEADDR, (*(int*)optval?"on":"off")));
      break;
    case SO_REUSEPORT:
      if (*(int*)optval) {
        ip_set_option(sock->conn->pcb.ip, SOF_REUSEPORT);
      } else {
        ip_reset_option(sock->conn->pcb.ip, SOF_REUSEPORT);
      }
      LWIP_DEBUGF(SOCKETS_DEBUG, ("lwip_setsockopt(%d, SOL_SOCKET, optname=0x%x, ..) -> %s\n",
                  s, SOF_REUSEPORT, (*(int*)optval?"on":"off")));
      break;
#endif /* SO_REUSE */
    /* UNIMPL case SO_USELOOPBACK: */
#if LWIP_SO_SNDTIMEO
    case SO_SNDTIMEO:
      tv = (struct timeval *)optval;
      if (tv->tv_usec < 0 || tv->tv_usec > 1000000 || tv->tv_sec < 0) {
        /*Invalid argument*/
        data->err = EDOM;
      } else if (tv->tv_sec == 0 && tv->tv_usec > 0 && tv->tv_usec < 10000){
        /*time interval is too small*/
        data->err = EINVAL;
      } else {
        /* lwIP use milliseconds*/
        netconn_set_sendtimeout(sock->conn, tv->tv_sec*1000 + tv->tv_usec/1000);
      }
      break;
#endif /* LWIP_SO_SNDTIMEO */
#if LWIP_SO_RCVTIMEO
    case SO_RCVTIMEO:
      tv = (struct timeval *)optval;
      if (tv->tv_usec < 0 || tv->tv_usec > 1000000 || tv->tv_sec < 0) {
        /*Invalid argument*/
        data->err = EDOM;
      } else if (tv->tv_sec == 0 && tv->tv_usec > 0 && tv->tv_usec < 10000){
        /*time interval is too small*/
        data->err = EINVAL;
      } else {
        /* lwIP use milliseconds*/
        netconn_set_recvtimeout(sock->conn, tv->tv_sec*1000 + tv->tv_usec/1000);
      }
      break;
#endif /* LWIP_SO_RCVTIMEO */
#if LWIP_SO_RCVBUF
    case SO_RCVBUF:
      if ((*(int*)optval) < RECV_BUFSIZE_MIN) {
        LWIP_DEBUGF(SOCKETS_DEBUG, ("lwip_setsockopt(%d, SOL_SOCKET, optname=0x%x) Invalid value \n", s, SO_RCVBUF));
        data->err = EINVAL;
      } else {
        netconn_set_recvbufsize(sock->conn, *(int*)optval);
      }
      break;
#endif /* LWIP_SO_RCVBUF */
#if LWIP_UDP
    case SO_NO_CHECK:
      if (*(int*)optval) {
        udp_setflags(sock->conn->pcb.udp, udp_flags(sock->conn->pcb.udp) | UDP_FLAGS_NOCHKSUM);
      } else {
        udp_setflags(sock->conn->pcb.udp, udp_flags(sock->conn->pcb.udp) & (u8_t)(~UDP_FLAGS_NOCHKSUM));
      }
      break;
#endif /* LWIP_UDP */
    default:
      data->err = EINVAL;
      LWIP_ASSERT("unhandled optname", 0);
      break;
    }
}

#ifdef LWIP_DEBUG
static void lwip_setsockopt_level_IPPROTO_IP (int s, struct lwip_sock *sock,  struct lwip_setgetsockopt_data *data,
                          const void *optval,  int optname)
#else
static void lwip_setsockopt_level_IPPROTO_IP (struct lwip_sock *sock,  struct lwip_setgetsockopt_data *data,
                          const void *optval,  int optname)
#endif
{

#if !LWIP_IGMP
    LWIP_UNUSED_ARG(data);
#endif

    switch (optname) {
    case IP_TTL:
      sock->conn->pcb.ip->ttl = (u8_t)(*(int*)optval);
      LWIP_DEBUGF(SOCKETS_DEBUG, ("lwip_setsockopt(%d, IPPROTO_IP, IP_TTL, ..) -> %d\n",
                  s, sock->conn->pcb.ip->ttl));
      break;
    case IP_TOS:
      sock->conn->pcb.ip->tos = (u8_t)(*(int*)optval);
      LWIP_DEBUGF(SOCKETS_DEBUG, ("lwip_setsockopt(%d, IPPROTO_IP, IP_TOS, ..)-> %d\n",
                  s, sock->conn->pcb.ip->tos));
      break;
#if LWIP_RAW
    case IP_HDRINCL:
      sock->conn->pcb.raw->hdrincl = (*(int*)optval) ? 1:0;
      LWIP_DEBUGF(SOCKETS_DEBUG, ("lwip_setsockopt(%d, IP_HDRINCL)-> %d\n",
                  s, sock->conn->pcb.raw->hdrincl));
      break;
#endif
#if LWIP_IGMP
    case IP_MULTICAST_TTL:
      sock->conn->pcb.udp->ttl = (u8_t)(*(u8_t*)optval);
      break;
    case IP_MULTICAST_IF:
      inet_addr_to_ipaddr(&sock->conn->pcb.udp->multicast_ip, (struct in_addr*)optval);
      break;
    case IP_MULTICAST_LOOP:
      if (*(u8_t*)optval) {
        udp_setflags(sock->conn->pcb.udp, udp_flags(sock->conn->pcb.udp) | UDP_FLAGS_MULTICAST_LOOP);
      } else {
        udp_setflags(sock->conn->pcb.udp, (u8_t)(udp_flags(sock->conn->pcb.udp) & ~UDP_FLAGS_MULTICAST_LOOP));
      }
      break;
    case IP_ADD_MEMBERSHIP:
    case IP_DROP_MEMBERSHIP:
      {
        /* If this is a TCP or a RAW socket, ignore these options. */
        struct ip_mreq *imr = (struct ip_mreq *)optval;
        ip_addr_t if_addr;
        ip_addr_t multi_addr;
        inet_addr_to_ipaddr(&if_addr, &imr->imr_interface);
        inet_addr_to_ipaddr(&multi_addr, &imr->imr_multiaddr);
        if(optname == IP_ADD_MEMBERSHIP){
          data->err = igmp_joingroup(&if_addr, &multi_addr);
        } else {
          data->err = igmp_leavegroup(&if_addr, &multi_addr);
        }
        data->err = err_to_errno(data->err);
      break;
#endif /* LWIP_IGMP */
    default:
      data->err = EINVAL;
      LWIP_ASSERT("unhandled optname", 0);
      break;
    }

}

#ifdef LWIP_DEBUG
static void lwip_setsockopt_level_IPPROTO_TCP (int s, struct lwip_sock *sock,  struct lwip_setgetsockopt_data *data,
                          const void *optval,  int optname)
#else
static void lwip_setsockopt_level_IPPROTO_TCP (struct lwip_sock *sock,  struct lwip_setgetsockopt_data *data,
                          const void *optval,  int optname)
#endif
{
    s32_t val = 0;

    switch (optname) {
    case TCP_NODELAY:
      if (*(int*)optval) {
        tcp_nagle_disable(sock->conn->pcb.tcp);
      } else {
        tcp_nagle_enable(sock->conn->pcb.tcp);
      }
      LWIP_DEBUGF(SOCKETS_DEBUG, ("lwip_setsockopt(%d, IPPROTO_TCP, TCP_NODELAY) -> %s\n",
                  s, (*(int *)optval)?"on":"off") );
      break;
    case TCP_KEEPALIVE:
      val = (s32_t)(*(int*)optval);
      if (val < 1) {
        data->err = EINVAL;
      } else {
        sock->conn->pcb.tcp->keep_idle = val;
        LWIP_DEBUGF(SOCKETS_DEBUG, ("lwip_setsockopt(%d, IPPROTO_TCP, TCP_KEEPALIVE) -> %"U32_F"\n",
                    s, (u32_t)sock->conn->pcb.tcp->keep_idle));
      }
      break;

#if LWIP_TCP_KEEPALIVE
    case TCP_KEEPIDLE:
      val = (s32_t)(*(int*)optval);
      if (val < 1) {
        data->err = EINVAL;
      } else {
        sock->conn->pcb.tcp->keep_idle = 1000*val;
        LWIP_DEBUGF(SOCKETS_DEBUG, ("lwip_setsockopt(%d, IPPROTO_TCP, TCP_KEEPIDLE) -> %"U32_F"\n",
                    s, sock->conn->pcb.tcp->keep_idle));
      }
      break;
    case TCP_KEEPINTVL:
      val = (s32_t)(*(int*)optval);
      if (val < 1) {
        data->err = EINVAL;
      } else {
        sock->conn->pcb.tcp->keep_intvl = 1000*val;
        LWIP_DEBUGF(SOCKETS_DEBUG, ("lwip_setsockopt(%d, IPPROTO_TCP, TCP_KEEPINTVL) -> %"U32_F"\n",
                    s, sock->conn->pcb.tcp->keep_intvl));
      }
      break;
    case TCP_KEEPCNT:
      val = (s32_t)(*(int*)optval);
      if (val < 1) {
        data->err = EINVAL;
      } else {
        sock->conn->pcb.tcp->keep_cnt = val;
        LWIP_DEBUGF(SOCKETS_DEBUG, ("lwip_setsockopt(%d, IPPROTO_TCP, TCP_KEEPCNT) -> %"U32_F"\n",
                    s, sock->conn->pcb.tcp->keep_cnt));
      }
      break;
#endif /* LWIP_TCP_KEEPALIVE */
    default:
      data->err = EINVAL;
      LWIP_ASSERT("unhandled optname", 0);
      break;
    }
}

#if LWIP_UDP && LWIP_UDPLITE
#ifdef LWIP_DEBUG
static void lwip_setsockopt_level_IPPROTO_UDPLITE (int s, struct lwip_sock *sock,  struct lwip_setgetsockopt_data *data,
                          const void *optval,  int optname)
#else
static void lwip_setsockopt_level_IPPROTO_UDPLITE (struct lwip_sock *sock,  struct lwip_setgetsockopt_data *data,
                          const void *optval,  int optname)
#endif
{
    LWIP_UNUSED_ARG(data);

    switch (optname) {
    case UDPLITE_SEND_CSCOV:
      if ((*(int*)optval != 0) && ((*(int*)optval < 8) || (*(int*)optval > 0xffff))) {
        /* don't allow illegal values! */
        sock->conn->pcb.udp->chksum_len_tx = 8;
      } else {
        sock->conn->pcb.udp->chksum_len_tx = (u16_t)*(int*)optval;
      }
      LWIP_DEBUGF(SOCKETS_DEBUG, ("lwip_setsockopt(%d, IPPROTO_UDPLITE, UDPLITE_SEND_CSCOV) -> %d\n",
                  s, (*(int*)optval)) );
      break;
    case UDPLITE_RECV_CSCOV:
      if ((*(int*)optval != 0) && ((*(int*)optval < 8) || (*(int*)optval > 0xffff))) {
        /* don't allow illegal values! */
        sock->conn->pcb.udp->chksum_len_rx = 8;
      } else {
        sock->conn->pcb.udp->chksum_len_rx = (u16_t)*(int*)optval;
      }
      LWIP_DEBUGF(SOCKETS_DEBUG, ("lwip_setsockopt(%d, IPPROTO_UDPLITE, UDPLITE_RECV_CSCOV) -> %d\n",
                  s, (*(int*)optval)) );
      break;
    default:
      data->err = EINVAL;
      LWIP_ASSERT("unhandled optname", 0);
      break;
    }
}
#endif

LWIP_STATIC void
lwip_setsockopt_internal(void *arg)
{
  struct lwip_sock *sock;
#ifdef LWIP_DEBUG
  int s;
#endif /* LWIP_DEBUG */
  int level, optname;
  const void *optval;
  struct lwip_setgetsockopt_data *data;

  LWIP_ASSERT("arg != NULL", arg != NULL);

  data = (struct lwip_setgetsockopt_data*)arg;
  sock = data->sock;
#ifdef LWIP_DEBUG
  s = data->s;
#endif /* LWIP_DEBUG */
  level = data->level;
  optname = data->optname;
  optval = data->optval;

    /*COVERITY: Checking the val for -ve to make sure non -ve ness */
  if (optname < 0)
  {
      sys_sem_signal(&sock->conn->op_completed);
      return;
  }


  switch (level) {

/* Level: SOL_SOCKET */
  case SOL_SOCKET:
#if PF_PKT_SUPPORT
  case SOL_PACKET:
#endif
#ifdef LWIP_DEBUG
    lwip_setsockopt_level_SOL(s, sock, data, optval, optname);
#else
    lwip_setsockopt_level_SOL(sock, data, optval, optname);
#endif
    break;

/* Level: IPPROTO_IP */
  case IPPROTO_IP:
#ifdef LWIP_DEBUG
    lwip_setsockopt_level_IPPROTO_IP(s, sock, data, optval, optname);
#else
    lwip_setsockopt_level_IPPROTO_IP(sock, data, optval, optname);
#endif
    break;

#if LWIP_TCP
/* Level: IPPROTO_TCP */
  case IPPROTO_TCP:
#ifdef LWIP_DEBUG
    lwip_setsockopt_level_IPPROTO_TCP(s, sock, data, optval, optname);
#else
    lwip_setsockopt_level_IPPROTO_TCP(sock, data, optval, optname);
#endif
    break;
#endif /* LWIP_TCP*/
#if LWIP_UDP && LWIP_UDPLITE
  /* Level: IPPROTO_UDPLITE */
  case IPPROTO_UDPLITE:
#ifdef LWIP_DEBUG
    lwip_setsockopt_level_IPPROTO_UDPLITE(s, sock, data, optval, optname);
#else
    lwip_setsockopt_level_IPPROTO_UDPLITE(sock, data, optval, optname);
#endif
    break;
#endif /* LWIP_UDP */
  default:
    LWIP_ASSERT("unhandled level", 0);
    break;
  }
  sys_sem_signal(&sock->conn->op_completed);
}

#define addr_netcmp(addr1, addr2, mask) ((addr1 & mask) == \
                                         (addr2& mask))

LWIP_STATIC void lwip_ioctl_internal_SIOCADDRT(struct rtentry *rmten, struct lwip_ioctl_data *data)
{
  struct sockaddr_in *sock_in;
  struct netif *netif;

#if LWIP_HAVE_LOOPIF
  if(((struct sockaddr_in *)&rmten->rt_dst)->sin_addr.s_addr == 0 &&
    ((struct sockaddr_in *)&rmten->rt_genmask)->sin_addr.s_addr == 0 &&
    netif_default != NULL &&
    (netif_default->link_layer_type != LOOPBACK_IF))
  {
    sock_in = (struct sockaddr_in *)&rmten->rt_gateway;
    netif_default->gw.addr = sock_in->sin_addr.s_addr;
    data->err = 0;
  }
  else
#endif
  {
    sock_in = (struct sockaddr_in *)&rmten->rt_genmask;
    for(netif = netif_list; netif != NULL; netif = netif->next)
    {
      if(sock_in->sin_addr.s_addr == netif->netmask.addr &&
        addr_netcmp(((struct sockaddr_in *)&rmten->rt_dst)->sin_addr.s_addr,
        netif->ip_addr.addr, netif->netmask.addr))
        break;
    }

    if(netif == NULL)
    {
      data->err = ENODEV;
    }
    else
    {
      sock_in = (struct sockaddr_in *)&rmten->rt_gateway;
      netif->gw.addr = sock_in->sin_addr.s_addr;
      data->err = 0;
    }
  }
}

LWIP_STATIC void lwip_ioctl_internal_SIOCGIFADDR(struct ifreq *ifr, struct lwip_ioctl_data *data)
{
  struct netif *netif;
  struct sockaddr_in *sock_in;

  /* get netif ipaddr */
  netif = netif_find(ifr->ifr_name);
  if(netif == NULL)
  {
    data->err = ENODEV;
  }
  else
  {
    sock_in = (struct sockaddr_in *)&ifr->ifr_addr;
    sock_in->sin_family = AF_INET;
    sock_in->sin_addr.s_addr = netif->ip_addr.addr;
    data->err = 0;
  }
}

LWIP_STATIC void lwip_ioctl_internal_SIOCSIFADDR(struct ifreq *ifr,
    struct lwip_ioctl_data *ifaddr_data)
{
  struct netif *netif;
  struct sockaddr_in *sock_in;

  /* set netif ipaddr */
  netif = netif_find(ifr->ifr_name);
  if(netif == NULL)
  {
    ifaddr_data->err = ENODEV;
  }
#if LWIP_HAVE_LOOPIF
  else if(netif->link_layer_type == LOOPBACK_IF)
  {
    ifaddr_data->err = EPERM;
  }
#endif
  else
  {
    sock_in = (struct sockaddr_in *)&ifr->ifr_addr;
    netif->ip_addr.addr = sock_in->sin_addr.s_addr;
    ifaddr_data->err = 0;
  }
}

LWIP_STATIC void lwip_ioctl_internal_SIOCGIFNETMASK(struct ifreq *ifr, struct lwip_ioctl_data *data)
{
  struct netif *netif;
  struct sockaddr_in *sock_in;

  /* get netif netmask */
  netif = netif_find(ifr->ifr_name);
  if(netif == NULL)
  {
    data->err = ENODEV;
  }
  else
  {
    sock_in = (struct sockaddr_in *)&ifr->ifr_addr;
    sock_in->sin_family = AF_INET;
    sock_in->sin_addr.s_addr = netif->netmask.addr;
    data->err = 0;
  }
}

LWIP_STATIC void lwip_ioctl_internal_SIOCSIFNETMASK(struct ifreq *ifr, struct lwip_ioctl_data *data)
{
  struct netif *netif;
  struct sockaddr_in *sock_in;

  /* set netif netmask */
  netif = netif_find(ifr->ifr_name);
  if(netif == NULL)
  {
    data->err = ENODEV;
  }
#if LWIP_HAVE_LOOPIF
  else if(netif->link_layer_type == LOOPBACK_IF)
  {
    data->err = EPERM;
  }
#endif
  else
  {
    sock_in = (struct sockaddr_in *)&ifr->ifr_addr;
    netif->netmask.addr = sock_in->sin_addr.s_addr;
    data->err = 0;
  }
}


LWIP_STATIC void lwip_ioctl_internal_SIOCSIFHWADDR(struct ifreq *ifr, struct lwip_ioctl_data *data)
{
  struct netif *netif;

  /* set netif hw addr */
  netif = netif_find(ifr->ifr_name);
  if(netif == NULL)
  {
    data->err = ENODEV;
  }
#if LWIP_HAVE_LOOPIF
  else if(netif->link_layer_type == LOOPBACK_IF)
  {
    data->err = EPERM;
  }
#endif
  else
  {
    if (netif->drv_set_hwaddr) {
      data->err = (err_t)netif->drv_set_hwaddr(netif, (u8_t *)ifr->ifr_hwaddr.sa_data, netif->hwaddr_len);
    } else {
      data->err = EIO;
    }

  }
}

LWIP_STATIC void lwip_ioctl_internal_SIOCGIFHWADDR(struct ifreq *ifr, struct lwip_ioctl_data *data)
{
  struct netif *netif;

  /* get netif hw addr */
  netif = netif_find(ifr->ifr_name);

  if(netif == NULL)
  {
    data->err = ENODEV;
  }
  else
  {
    (void)MEMCPY_S((void *)ifr->ifr_hwaddr.sa_data, sizeof(ifr->ifr_hwaddr.sa_data),
            (void *)netif->hwaddr, netif->hwaddr_len); //CSEC_FIX_2302
    data->err = 0;
  }
}

LWIP_STATIC void lwip_ioctl_internal_SIOCSIFFLAGS(struct ifreq *ifr, struct lwip_ioctl_data *data)
{
  struct netif *netif;

  /* set netif hw addr */
  netif = netif_find(ifr->ifr_name);
  if(netif == NULL)
  {
    data->err = ENODEV;
  }
#if LWIP_HAVE_LOOPIF
  else if(netif->link_layer_type == LOOPBACK_IF)
  {
    data->err = EPERM;
  }
#endif /* LWIP_HAVE_LOOPIF */
  else
  {
    if((ifr->ifr_flags & IFF_UP) && !(netif->flags & NETIF_FLAG_UP))
      netif_set_up(netif);
    else if(!(ifr->ifr_flags & IFF_UP) && (netif->flags & NETIF_FLAG_UP))
      netif_set_down(netif);

    if((ifr->ifr_flags & IFF_RUNNING) && !(netif->flags & NETIF_FLAG_LINK_UP))
      netif_set_link_up(netif);
    else if(!(ifr->ifr_flags & IFF_RUNNING) && (netif->flags & NETIF_FLAG_LINK_UP))
      netif_set_link_down(netif);

    if(ifr->ifr_flags & IFF_BROADCAST)
      netif->flags |= NETIF_FLAG_BROADCAST;
    else
#if DRIVER_STATUS_CHECK
      netif->flags = (u16_t)(netif->flags & (~NETIF_FLAG_BROADCAST));
#else
      netif->flags = (u8_t)(netif->flags & (~NETIF_FLAG_BROADCAST));
#endif

#if PPP_SUPPORT
    if(ifr->ifr_flags & IFF_POINTOPOINT)
      netif->flags |= NETIF_FLAG_POINTTOPOINT;
    else
#if DRIVER_STATUS_CHECK
      netif->flags = (u16_t)(netif->flags & (~NETIF_FLAG_POINTTOPOINT));
#else
      netif->flags = (u8_t)(netif->flags & (~NETIF_FLAG_POINTTOPOINT));
#endif //DRIVER_STATUS_CHECK
#endif

    if(ifr->ifr_flags & IFF_NOARP)
#if DRIVER_STATUS_CHECK
      netif->flags = (u16_t)(netif->flags & (~NETIF_FLAG_ETHARP));
#else
      netif->flags = (u8_t)(netif->flags & (~NETIF_FLAG_ETHARP));
#endif
    else
      netif->flags |= NETIF_FLAG_ETHARP;

#if LWIP_IGMP
    if(ifr->ifr_flags & IFF_MULTICAST)
      netif->flags |= NETIF_FLAG_IGMP;
    else
#if DRIVER_STATUS_CHECK
      netif->flags = (u16_t)(netif->flags &  ~NETIF_FLAG_IGMP);
#else
      netif->flags = (u8_t)(netif->flags &  ~NETIF_FLAG_IGMP);
#endif
#endif

#if LWIP_DHCP
    if(ifr->ifr_flagshigh & (short)(IFF_DYNAMIC >> 16))
      dhcp_start(netif);
    else
    {
      dhcp_stop(netif);

      dhcp_cleanup(netif);
    }
#endif

#if LWIP_DHCPS
    if(ifr->ifr_flagshigh & (short)(IFF_DYNAMIC_S>> 16))
      dhcps_start(netif);
    else
      dhcps_stop(netif);
#endif
#if LWIP_NETIF_PROMISC
    if((ifr->ifr_flags & IFF_PROMISC))
    {
      netif_update_promiscuous_mode_status(netif, 1);
    }
    else
    {
      netif_update_promiscuous_mode_status(netif, 0);
    }
#endif /* LWIP_NETIF_PROMISC */
    data->err = 0;
  }
}


LWIP_STATIC void lwip_ioctl_internal_SIOCGIFFLAGS(struct ifreq *ifr, struct lwip_ioctl_data *data)
{
  struct netif *netif;

  /* set netif hw addr */
  netif = netif_find(ifr->ifr_name);
  if(netif == NULL)
  {
    data->err = ENODEV;
  }
  else
  {
    if(netif->flags & NETIF_FLAG_UP)
      ifr->ifr_flags |= IFF_UP;
    else
      ifr->ifr_flags &= ~IFF_UP;

    if(netif->flags & NETIF_FLAG_LINK_UP)
      ifr->ifr_flags |= IFF_RUNNING;
    else
      ifr->ifr_flags &= ~IFF_RUNNING;

    if(netif->flags & NETIF_FLAG_BROADCAST)
      ifr->ifr_flags |= IFF_BROADCAST;
    else
      ifr->ifr_flags &= ~IFF_BROADCAST;

#if PPP_SUPPORT
    if(netif->flags & NETIF_FLAG_POINTTOPOINT)
      ifr->ifr_flags |= IFF_POINTOPOINT;
    else
      ifr->ifr_flags &= ~IFF_POINTOPOINT;
#endif

    if(netif->flags & NETIF_FLAG_ETHARP)
      ifr->ifr_flags &= ~IFF_NOARP;
    else
      ifr->ifr_flags |= IFF_NOARP;

#if LWIP_IGMP
    if(netif->flags & NETIF_FLAG_IGMP)
      ifr->ifr_flags = (short)((unsigned short)ifr->ifr_flags | IFF_MULTICAST);
    else
      ifr->ifr_flags = (short)((unsigned short)ifr->ifr_flags & (~IFF_MULTICAST));
#endif

#if LWIP_DHCP
    if(netif->flags & NETIF_FLAG_DHCP)
      ifr->ifr_flagshigh |= (short)(IFF_DYNAMIC >> 16);
    else
      ifr->ifr_flagshigh &= ~(short)(IFF_DYNAMIC >> 16);
#endif

#if LWIP_DHCPS
    if(netif->dhcps)
      ifr->ifr_flagshigh |= (short)(IFF_DYNAMIC_S >> 16);
    else
      ifr->ifr_flagshigh &= ~(short)(IFF_DYNAMIC_S >> 16);
#endif

#if LWIP_HAVE_LOOPIF
    if(netif->link_layer_type == LOOPBACK_IF)
      ifr->ifr_flags |= IFF_LOOPBACK;
#endif
#if LWIP_NETIF_PROMISC
    if(atomic_read(&(netif->flags_ext)) == NETIF_FLAG_PROMISC)
    {
      ifr->ifr_flags |= IFF_PROMISC;
    }
    else
    {
      ifr->ifr_flags &= ~IFF_PROMISC;
    }
#endif /* LWIP_NETIF_PROMISC */

    data->err = 0;
  }
}

LWIP_STATIC void lwip_ioctl_internal_SIOCGIFNAME(struct ifreq *ifr, struct lwip_ioctl_data *data)
{
  struct netif *netif;
  s16_t index = 0;

  for(netif = netif_list; netif != NULL; netif = netif->next, index++) {
    if (ifr->ifr_index == index ) {
      break;
    }
  }

  if(netif == NULL) {
    data->err = ENODEV;
  } else {
     (void)SNPRINTF_S(ifr->ifr_name, IFNAMSIZ, (IFNAMSIZ-1), "%s", netif->name);
    data->err = 0;
  }
}

LWIP_STATIC u32_t lwip_validate_ifname(u32_t i, struct ifreq *ifr,u8_t *num,u8_t *ifid)
{
    if((i>0) && (i< IFNAMSIZ))
    {
      while(--i > 0) {
        if(ifr->ifr_name[i] >= '0' && ifr->ifr_name[i] <= '9') {
          /* eg: eth0:1  -  name = eth, num = 0, ifid = 1 */
          if(ifr->ifr_name[i-1] == ':') {
            if(i < 2) { /* To protect i-2 from getting rolled over */
              break;
            }

            *ifid = (u8_t)(ifr->ifr_name[i] - '0');
            if(ifr->ifr_name[i-2] >= '0' && ifr->ifr_name[i-2] <= '9') {
              *num = (u8_t)(ifr->ifr_name[i-2] - '0');
            } else {
              break;
            }
          } else {
            *ifid = 0;
            *num = (u8_t)(ifr->ifr_name[i] - '0');
          }
        } else
          break;
      }
    }
    return i;
}

LWIP_STATIC void lwip_ioctl_internal_SIOCSIFNAME(struct ifreq *ifr, struct lwip_ioctl_data *data)
{
  struct netif *netif;
  s16_t index = 0;
  u8_t num = 0;
  u8_t ifid = 0;
  u32_t i = 0;
  int err =0;

  for(netif = netif_list; netif != NULL; netif = netif->next) {
    if (ifr->ifr_index == index ) {
      break;
    }
  }

  if(netif == NULL)
  {
    data->err = ENODEV;
  } else if(netif->link_layer_type == LOOPBACK_IF) {
    data->err = EPERM;
  } else {
    i = STRLEN(ifr->ifr_name);
    i = lwip_validate_ifname(i,ifr,&num,&ifid);
    /*lint -e730*/
    if(i == 0 || i >= IFNAMSIZ || ((netif_chech_index_isusing(ifr->ifr_name, num) == 1) && (netif_chech_ifaceid_isusing(ifr->ifr_name, num, ifid) == 1)))
      data->err = EINVAL;
    else
    {
      err = STRNCPY_S(netif->name, sizeof(netif->name), ifr->ifr_name, STRLEN(ifr->ifr_name));
      if (err != 0) {
          data->err = (err_t)err;
          return;
      }
      netif->name[STRLEN(ifr->ifr_name)] = '\0';
      netif->num = num;
      netif->iface_id = ifid;
      data->err = 0;
    }
    /*lint +e730*/
  }
}

#if PF_PKT_SUPPORT
LWIP_STATIC void lwip_ioctl_internal_SIOCGIFINDEX(struct ifreq *ifr, struct lwip_ioctl_data *data)
{
  struct netif *netif;

  netif = netif_find(ifr->ifr_name);
  if(netif == NULL)
  {
    data->err = ENODEV;
  }
  else
  {
    ifr->ifr_index = netif->ifindex;
    data->err = 0;
  }

  return;
}

#endif

LWIP_STATIC void
lwip_ioctl_internal(void *argp)
{
  struct lwip_ioctl_data *data = (struct lwip_ioctl_data *)argp;
  struct ifreq *ifr = (struct ifreq *)data->arg;
  struct rtentry *rmten = (struct rtentry *)data->arg;

  switch (data->cmd) {
  case SIOCADDRT:
    lwip_ioctl_internal_SIOCADDRT(rmten, data);
    break;
  case SIOCGIFADDR:
    lwip_ioctl_internal_SIOCGIFADDR(ifr, data);
    break;
  case SIOCSIFADDR:
    lwip_ioctl_internal_SIOCSIFADDR(ifr, data);
    break;
  case SIOCGIFNETMASK:
    lwip_ioctl_internal_SIOCGIFNETMASK(ifr, data);
    break;
  case SIOCSIFNETMASK:
    lwip_ioctl_internal_SIOCSIFNETMASK(ifr, data);
    break;
  case SIOCSIFHWADDR:
    lwip_ioctl_internal_SIOCSIFHWADDR(ifr, data);
    break;
  case SIOCGIFHWADDR:
    lwip_ioctl_internal_SIOCGIFHWADDR(ifr, data);
    break;
  case SIOCSIFFLAGS:
   lwip_ioctl_internal_SIOCSIFFLAGS(ifr, data);
    break;
  case SIOCGIFFLAGS:
    lwip_ioctl_internal_SIOCGIFFLAGS(ifr, data);
    break;
  case SIOCGIFNAME:
    lwip_ioctl_internal_SIOCGIFNAME(ifr, data);
    break;
  case SIOCSIFNAME:
    lwip_ioctl_internal_SIOCSIFNAME(ifr, data);
    break;
#if PF_PKT_SUPPORT
  case SIOCGIFINDEX:
    lwip_ioctl_internal_SIOCGIFINDEX(ifr, data);
    break;
#endif
  default:
    data->err = ESRCH;
    LWIP_ASSERT("unhandled optname", 0);
    break;
  }

  sys_sem_signal(&data->cb_completed);
}

int
lwip_ioctl(int s, long cmd, void *argp)
{
  struct lwip_ioctl_data data;
  u8_t val;
  err_t callback_err;
#if LWIP_SO_RCVBUF
  u16_t buflen = 0;
  s32_t recv_avail;
#endif /* LWIP_SO_RCVBUF */
  struct lwip_sock *sock = NULL;
  struct lwip_sock *isock = NULL;

  if (!argp) {
    set_errno(EINVAL);
    return -1;
  }

  isock = get_socket_index(s);
  if(NULL == isock) {
    set_errno(EBADF);
    return -1;
  }

  lwip_sock_lock(isock);
  sock = get_socket_unsafe(s, 0);
  if(NULL == sock)
  {
    lwip_sock_unlock(isock);
    set_errno(EBADF);
    return -1;
  }

  switch ((u32_t)cmd) {
#if LWIP_SO_RCVBUF
  case FIONREAD:
    SYS_ARCH_GET(sock->conn->recv_avail, recv_avail);
    if (recv_avail < 0) {
      recv_avail = 0;
    }
    *((s32_t*)argp) = (s32_t)recv_avail;

    /* Check if there is data left from the last recv operation. /maq 041215 */
    if (sock->lastdata) {
      struct pbuf *p = (struct pbuf *)sock->lastdata;
      if (netconn_type(sock->conn) != NETCONN_TCP) {
        p = ((struct netbuf *)p)->p;
      }
      buflen = p->tot_len;
      buflen = (u16_t)(buflen - sock->lastoffset);

      *((s32_t*)argp) += buflen;
    }

    LWIP_DEBUGF(SOCKETS_DEBUG, ("lwip_ioctl(%d, FIONREAD, %p) = %"S32_F"\n", s, argp, *((s32_t*)argp)));
    sock_set_errno_unsafe(sock, 0);
    lwip_sock_unlock(isock);
    set_errno(0);
    return 0;
#endif /* LWIP_SO_RCVBUF */

  case FIONBIO:
    if (*(u32_t*)argp) {
      val = 1;
    } else {
      val = 0;
    }
    netconn_set_nonblocking(sock->conn, val);
    LWIP_DEBUGF(SOCKETS_DEBUG, ("lwip_ioctl(%d, FIONBIO, %d)\n", s, val));
    sock_set_errno_unsafe(sock, 0);
    lwip_sock_unlock(isock);
    set_errno(0);
    return 0;
  case SIOCADDRT:
#if PF_PKT_SUPPORT
    VALIDATE_NETCONN_TYPE_IS_RAW(sock)
    {
      sock_set_errno_unsafe(sock, EAFNOSUPPORT);
      lwip_sock_unlock(isock);
      set_errno(EAFNOSUPPORT);
      return -1;
    }
#endif
  /*lint-fallthrough */

  case SIOCGIFADDR:
  case SIOCSIFADDR:
  case SIOCGIFNETMASK:
  case SIOCSIFNETMASK:
  case SIOCSIFHWADDR:
  case SIOCGIFHWADDR:
  case SIOCGIFFLAGS:
  case SIOCSIFFLAGS:
  case SIOCGIFNAME:
  case SIOCSIFNAME:
#if PF_PKT_SUPPORT
  case SIOCGIFINDEX:
#endif
    data.s   = s;
    data.cmd = (u32_t)cmd;
    data.arg = argp;
    data.err = 0;
    if (sys_sem_new(&data.cb_completed, 0) != ERR_OK) {
        LWIP_DEBUGF(SOCKETS_DEBUG, ("lwip_ioctl(%d, SIOCGIFADDR, %p)\n", s, argp));
        sock_set_errno_unsafe(sock, ENOMEM);
        lwip_sock_unlock(isock);
        set_errno(ENOMEM);
        return -1;
    }
    sock->events++;
    lwip_sock_unlock(isock);

    callback_err = tcpip_callback(lwip_ioctl_internal, &data);

    if (callback_err == ERR_OK) {
        (void)sys_arch_sem_wait(&data.cb_completed, 0);
    } else {
        data.err = callback_err;
    }

    sys_sem_free(&data.cb_completed);
	sock_set_errno(sock, data.err);

    LWIP_SOCK_DECREMENT_EVENTS(sock);
    set_errno(data.err);
    return data.err ? -1 : 0;

  default:
    break;
  }

  sock_set_errno_unsafe(sock, EINVAL);
  lwip_sock_unlock(isock);
  LWIP_DEBUGF(SOCKETS_DEBUG, ("lwip_ioctl(%d, UNIMPL: 0x%lx, %p)\n", s, cmd, argp));
  set_errno(EINVAL);  /* not yet implemented */
  return -1;
}

/** A minimal implementation of fcntl.
 * Currently only the commands F_GETFL and F_SETFL are implemented.
 * Only the flag O_NONBLOCK is implemented.
 */
int
lwip_fcntl(int s, int cmd, int val)
{
  int ret = -1;
  struct lwip_sock *sock;
  struct lwip_sock *isock;
  int sock_errno = 0;

  if( val < 0)
  {
    set_errno(EINVAL);
    return -1;
  }

  isock = get_socket_index(s);
  if(NULL == isock) {
    set_errno(EBADF);
    return -1;
  }

  lwip_sock_lock(isock);
  sock = get_socket_unsafe(s, 0);
  if(NULL == sock)
  {
    lwip_sock_unlock(isock);
    set_errno(EBADF);
    return -1;
  }

  switch (cmd) {
  case F_GETFL:
    sys_mutex_lock(&sock->conn->conn_mutex);
    ret = netconn_is_nonblocking(sock->conn) ? O_NONBLOCK : 0;
    sys_mutex_unlock(&sock->conn->conn_mutex);
    break;
  case F_SETFL:
    if (((u32_t)val & ~O_NONBLOCK) == 0) {
      /* only O_NONBLOCK, all other bits are zero */
      netconn_set_nonblocking(sock->conn, (u32_t)val & O_NONBLOCK);
      ret = 0;
    }
    break;
  default:
    sock_errno = EINVAL;
    break;
  }

  sock_set_errno_unsafe(sock, sock_errno);
  lwip_sock_unlock(isock);
  if (ERR_OK != sock_errno)
  {
    LWIP_DEBUGF(SOCKETS_DEBUG, ("lwip_fcntl(%d, UNIMPL: %d, %d)\n", s, cmd, val));
  }
  set_errno(sock_errno);  /* not yet implemented */
  return ret;
}


/* Get dst_mac TCP / IP connections, src_ip, dst_ip, ipid, srcport, dstport,
 * Seq, ack, tcpwin, tsval, tsecr information
 *
 * @param s socket descriptor for which the details are required
 * @param conn parameter in which the acquired TCP / IP connection information is to be saved
 *
 * @note If the UDP connection, seq, ack, tcpwin, tsval, tsecr is set to 0
 * @return 0 if successful; failure to return -1
 */

int lwip_get_conn_info (int s, void *conninfo)
{
  struct lwip_sock *sock;
  struct lwip_sock *isock;
  err_t err;
  int retval = 0;


  isock = get_socket_index(s);
  if(NULL == isock) {
    set_errno(EBADF);
    return -1;
  }

  lwip_sock_lock(isock);
  sock = get_socket_unsafe(s, LWIP_SOCKET_ACTIVE);
  if(NULL == sock)
  {
    lwip_sock_unlock(isock);
    set_errno(EBADF);
    return -1;
  }
  sock->events++;
  lwip_sock_unlock(isock);
  lwip_sock_lock_op(sock);
  err = netconn_getconninfo(sock->conn, conninfo);

  if (err != ERR_OK) {
    retval = -1;
    goto RETURN;
  }

RETURN:
  lwip_sock_unlock_op(sock);
  LWIP_SOCK_DECREMENT_EVENTS(sock);
  if (err == ERR_VAL) {
      set_errno(EOPNOTSUPP);
  }
  else{
    set_errno(err_to_errno(err));
  }
  return retval;
}

#endif /* LWIP_SOCKET */
