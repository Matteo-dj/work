/**
 * @file
 * Sequential API External module
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

/* This is the part of the API that is linked with
   the application */

#include "lwip/opt.h"

#if LWIP_NETCONN /* don't build if not configured for use in lwipopts.h */

#include "lwip/api.h"
#include "lwip/tcpip.h"
#include "lwip/memp.h"

#include "lwip/ip.h"
#include "lwip/raw.h"
#include "lwip/udp.h"
#include "lwip/tcp.h"

#include <string.h>

/**
 * Create a new netconn (of a specific type) that has a callback function.
 * The corresponding pcb is also created.
 *
 * @param t the type of 'connection' to create (@see enum netconn_type)
 * @param proto the IP protocol for RAW IP pcbs
 * @param callback a function to call on status changes (RX available, TX'ed)
 * @return a newly allocated struct netconn or
 *         NULL on memory error
 */
struct netconn*
#if PF_PKT_SUPPORT
netconn_new_with_proto_and_callback(enum netconn_type t, u16_t proto, netconn_callback callback)
#else
netconn_new_with_proto_and_callback(enum netconn_type t, u8_t proto, netconn_callback callback)
#endif
{
  struct netconn *conn;
  struct api_msg msg;

  MEMSET_S(&msg,sizeof(struct api_msg),0,sizeof(struct api_msg));
  conn = netconn_alloc(t, callback);
  if (conn != NULL) {
    msg.function = do_newconn;
    msg.msg.msg.n.proto = proto;
    msg.msg.conn = conn;
    if (TCPIP_APIMSG(&msg) != ERR_OK) {
      LWIP_ASSERT("freeing conn without freeing pcb", conn->pcb.tcp == NULL);
      LWIP_ASSERT("conn has no op_completed", sys_sem_valid(&conn->op_completed));
      LWIP_ASSERT("conn has no recvmbox", sys_mbox_valid(&conn->recvmbox));
#if LWIP_TCP
      LWIP_ASSERT("conn->acceptmbox shouldn't exist", !sys_mbox_valid(&conn->acceptmbox));
#endif /* LWIP_TCP */
      netconn_free(conn);
      return NULL;
    }
  }
  return conn;
}

/* To avoid confusion and improve portability by
  * use the keyword 'static' in addition to 'inline'.  The keyword
  * 'extern' can also be used along with the 'inline' to resolve this
  *  ambiguity.
  * As we are using this API in api_msg.c static can't be used,
  * So this should not have any issue.
  */
/*lint -e695*/
inline int netconn_mbox_is_active(struct netconn *conn)
{
  if (NULL == conn) {
    return 0;
  }

  return (atomic_read(&conn->mbox_state) == NETCONN_MBOX_ACTIVE);
}
/*lint +e695*/

/**
 * Close a netconn 'connection' and free its resources.
 * UDP and RAW connection are completely closed, TCP pcbs might still be in a waitstate
 * after this returns.
 *
 * @param conn the netconn to delete
 * @return ERR_OK if the connection was deleted
 */
err_t
netconn_delete(struct netconn *conn)
{
  struct api_msg msg;

  /* No ASSERT here because possible to get a (conn is equal to NULL) if we got an accept error */
  if (conn == NULL) {
    return ERR_OK;
  }

  msg.function = do_delconn;
  msg.msg.conn = conn;
  (void)tcpip_apimsg(&msg, &(msg.msg.conn->close_completed));

  netconn_free(conn);

  /* don't care for return value of do_delconn since it only calls void functions */

  return ERR_OK;
}

err_t
netconn_initiate_delete(struct netconn *conn)
{
  struct api_msg msg;

  /* No ASSERT here because possible to get a (conn is equal to NULL) if we got an accept error */
  if (conn == NULL) {
    return ERR_OK;
  }

  msg.function = do_delconn;
  msg.msg.conn = conn;
  (void)tcpip_apimsg(&msg, &(msg.msg.conn->close_completed));


  /* don't care for return value of do_delconn since it only calls void functions */

  return ERR_OK;
}

err_t
netconn_finish_delete(struct netconn *conn)
{
  netconn_free(conn);
  return ERR_OK;
}


/**
 * Get the local or remote IP_add and port of a netconn.
 * For RAW netconns, this returns the protocol instead of a port!
 *
 * @param conn the netconn to query
 * @param addr a pointer to which to save the IP_add
 * @param port a pointer to which to save the port (or protocol for RAW)
 * @param local 1 to get the local IP_add, 0 to get the remote one
 * @return ERR_CONN for invalid connections
 *         ERR_OK if the information was retrieved
 */
err_t
netconn_getaddr(struct netconn *conn, ip_addr_t *addr, u16_t *port, u8_t local)
{
  struct api_msg msg;
  err_t err;

  LWIP_ERROR("netconn_getaddr: invalid conn", (conn != NULL), return ERR_VAL;);
  LWIP_ERROR("netconn_getaddr: invalid addr", (addr != NULL), return ERR_VAL;);
  LWIP_ERROR("netconn_getaddr: invalid port", (port != NULL), return ERR_VAL;);

  msg.function = do_getaddr;
  msg.msg.conn = conn;
  msg.msg.msg.ad.ipaddr = addr;
  msg.msg.msg.ad.port = port;
  msg.msg.msg.ad.local = local;
  err = TCPIP_APIMSG(&msg);

  NETCONN_SET_SAFE_ERR(conn, err);
  return err;
}

/**
 * Bind a netconn to a specific local IP_add and port.
 * Binding one netconn twice might not always be checked correctly!
 *
 * @param conn the netconn to bind
 * @param addr the local IP_add to bind the netconn to (use IP_ADDR_ANY
 *             to bind to all addresses)
 * @param port the local port to bind the netconn to (not used for RAW)
 * @param ifindex the interface index to bind the netconn to (used only for Packet Family Sockets)
 * @return ERR_OK if bound, any other err_t on failure
 */
err_t
#if PF_PKT_SUPPORT
netconn_bind(struct netconn *conn, ip_addr_t *addr, u16_t port, u8_t ifindex)
#else
netconn_bind(struct netconn *conn, ip_addr_t *addr, u16_t port)
#endif
{
  struct api_msg msg;
  err_t err;

  LWIP_ERROR("netconn_bind: invalid conn", (conn != NULL), return ERR_VAL;);

  msg.function = do_bind;
  msg.msg.conn = conn;
  msg.msg.msg.bc.ipaddr = addr;
  msg.msg.msg.bc.port = port;
#if PF_PKT_SUPPORT
  msg.msg.msg.bc.netifindex = ifindex;
#endif
  err = TCPIP_APIMSG(&msg);

  NETCONN_SET_SAFE_ERR(conn, err);
  return err;
}

/**
 * Connect a netconn to a specific remote IP_add and port.
 *
 * @param conn the netconn to connect
 * @param addr the remote IP_add to connect to
 * @param port the remote port to connect to (no used for RAW)
 * @return ERR_OK if connected, return value of tcp_/udp_/raw_connect otherwise
 */
err_t
netconn_connect(struct netconn *conn, ip_addr_t *addr, u16_t port)
{
  struct api_msg msg;
  err_t err;

  LWIP_ERROR("netconn_connect: invalid conn", (conn != NULL), return ERR_VAL;);

  msg.function = do_connect;
  msg.msg.conn = conn;
  msg.msg.msg.bc.ipaddr = addr;
  msg.msg.msg.bc.port = port;
  /* This is the only function which need to not block tcpip_thread */
  err = tcpip_apimsg(&msg, &(conn->op_completed));

  NETCONN_SET_SAFE_ERR(conn, err);
  return err;
}

/**
 * Set a TCP netconn into listen mode
 *
 * @param conn the tcp netconn to set to listen mode
 * @param backlog the listen backlog, only used if TCP_LISTEN_BACKLOG==1
 * @return ERR_OK if the netconn was set to listen (UDP and RAW netconns
 *         don't return any error (yet?))
 */
err_t
netconn_listen_with_backlog(struct netconn *conn, u8_t backlog)
{
#if LWIP_TCP
  struct api_msg msg;
  err_t err;

  /* This does no harm. If TCP_LISTEN_BACKLOG is off, backlog is unused. */
  LWIP_UNUSED_ARG(backlog);

  LWIP_ERROR("netconn_listen: invalid conn", (conn != NULL), return ERR_VAL;);

  msg.function = do_listen;
  msg.msg.conn = conn;
#if TCP_LISTEN_BACKLOG
  msg.msg.msg.lb.backlog = backlog;
#endif /* TCP_LISTEN_BACKLOG */
  err = TCPIP_APIMSG(&msg);

  NETCONN_SET_SAFE_ERR(conn, err);
  return err;
#else /* LWIP_TCP */
  LWIP_UNUSED_ARG(conn);
  LWIP_UNUSED_ARG(backlog);
  return ERR_ARG;
#endif /* LWIP_TCP */
}

/**
 * Accept a new connection on a TCP listening netconn.
 *
 * @param conn the TCP listen netconn
 * @param new_conn pointer where the new connection is stored
 * @return ERR_OK if a new connection has been received or an error
 *                code otherwise
 */
err_t
netconn_accept(struct netconn *conn, struct netconn **new_conn)
{
#if LWIP_TCP
  struct netconn *newconn;
  err_t err;
#if TCP_LISTEN_BACKLOG
  struct api_msg msg;
#endif /* TCP_LISTEN_BACKLOG */

  LWIP_ERROR("netconn_accept: invalid pointer",    (new_conn != NULL),                  return ERR_VAL;);
  *new_conn = NULL;
  LWIP_ERROR("netconn_accept: invalid conn",       (conn != NULL),                      return ERR_VAL;);
  LWIP_ERROR("netconn_accept: invalid acceptmbox", sys_mbox_valid(&conn->acceptmbox)
    && netconn_mbox_is_active(conn),   return ERR_VAL;);

  if (atomic_read(&conn->is_err_fatal)) {
    /* don't recv on fatal errors: this might block the application task
       waiting on acceptmbox forever! */
    NETCONN_GET_SAFE_ERR(conn, err);
    return err;
  }

#if LWIP_SO_RCVTIMEO
  if (sys_arch_mbox_fetch(&conn->acceptmbox, (void **)&newconn, (u32_t)(conn->recv_timeout)) == SYS_ARCH_TIMEOUT) {
    /*lint -e506*/
    NETCONN_SET_SAFE_ERR(conn, ERR_TIMEOUT);
    /*lint +e506*/
    return ERR_TIMEOUT;
  }
#else
  sys_arch_mbox_fetch(&conn->acceptmbox, (void **)&newconn, 0);
#endif /* LWIP_SO_RCVTIMEO*/
  /* Register event with callback */
  API_EVENT(conn, NETCONN_EVT_RCVMINUS, 0);

  if (newconn == NULL) {
    /* connection has been aborted */
    /*lint -e506*/
    NETCONN_SET_SAFE_ERR(conn, ERR_ABRT);
    /*lint +e506*/
    return ERR_ABRT;
  }
#if TCP_LISTEN_BACKLOG
  /* Let the stack know that we have accepted the connection. */
  msg.function = do_recv;
  msg.msg.conn = conn;
  /* don't care for the return value of do_recv */
  TCPIP_RECV_APIMSG(&msg, conn, err);
#endif /* TCP_LISTEN_BACKLOG */
  LWIP_UNUSED_ARG(err);

  *new_conn = newconn;
  /* don't set conn->last_err: it's only ERR_OK, anyway */
  return ERR_OK;
#else /* LWIP_TCP */
  LWIP_UNUSED_ARG(conn);
  LWIP_UNUSED_ARG(new_conn);
  return ERR_ARG;
#endif /* LWIP_TCP */
}

/**
 * Receive data: actual implementation that doesn't care whether pbuf or netbuf
 * is received
 *
 * @param conn the netconn from which to receive data
 * @param new_buf pointer where a new pbuf/netbuf is stored when received data
 * @return ERR_OK if data has been received, an error code otherwise (timeout,
 *                memory error or another error)
 */
LWIP_STATIC err_t
netconn_recv_data(struct netconn *conn, void **new_buf)
{
  void *buf = NULL;
  u16_t len;
  err_t err;
#if LWIP_TCP
  struct api_msg msg;
  u8_t noautorecved;
#endif /* LWIP_TCP */

  *new_buf = NULL;
  LWIP_ERROR("netconn_recv: invalid recvmbox", sys_mbox_valid(&conn->recvmbox)
    && netconn_mbox_is_active(conn), return ERR_CONN;);
  
  if (atomic_read(&conn->is_err_fatal)) {
    /* don't recv on fatal errors: this might block the application task
       waiting on recvmbox forever! */
    /* @todo: this does not allow us to fetch data that has been put into recvmbox
       before the fatal error occurred - is that a problem? */
    NETCONN_GET_SAFE_ERR(conn, err);
    return err;
  }

#if LWIP_SO_RCVTIMEO
  if (sys_arch_mbox_fetch(&conn->recvmbox, &buf, (u32_t)(conn->recv_timeout)) == SYS_ARCH_TIMEOUT) {
    /*lint -e506*/
    NETCONN_SET_SAFE_ERR(conn, ERR_TIMEOUT);
    /*lint +e506*/
    return ERR_TIMEOUT;
  }
#else
  sys_arch_mbox_fetch(&conn->recvmbox, &buf, 0);
#endif /* LWIP_SO_RCVTIMEO*/

#if LWIP_TCP
#if (LWIP_UDP || LWIP_RAW)
  if (conn->type == NETCONN_TCP)
#endif /* (LWIP_UDP || LWIP_RAW) */
  {
    netconn_get_noautorecved(conn, noautorecved);
    if (!noautorecved || (buf == NULL)) {
      /* Let the stack know that we have taken the data. */
      /* TODO: Speedup: Don't block and wait for the answer here
         (to prevent multiple thread-switches). */
      msg.function = do_recv;
      msg.msg.conn = conn;
      if (buf != NULL) {
        msg.msg.msg.r.len = ((struct pbuf *)buf)->tot_len;
      } else {
        msg.msg.msg.r.len = 1;
      }
      /* don't care for the return value of do_recv */
      TCPIP_RECV_APIMSG(&msg, conn, err);
    }

    /* If we are closed, we indicate that we no longer wish to use the socket */
    if (buf == NULL) {
      API_EVENT(conn, NETCONN_EVT_RCVMINUS, 0);
      /* Avoid to lose any previous error code */
      /*lint -e506*/
      NETCONN_SET_SAFE_ERR(conn, ERR_CLSD);
      /*lint +e506*/
      return ERR_CLSD;
    }
    len = ((struct pbuf *)buf)->tot_len;
  }
#endif /* LWIP_TCP */
#if LWIP_TCP && (LWIP_UDP || LWIP_RAW)
  else
#endif
#if (LWIP_UDP || LWIP_RAW)
  {
    if (buf == NULL) {
      API_EVENT(conn, NETCONN_EVT_RCVMINUS, 0);
      /* Avoid to lose any previous error code */
      /*lint -e506*/
      NETCONN_SET_SAFE_ERR(conn, ERR_CLSD);
      /*lint +e506*/
      return ERR_CLSD;
    }
    len = netbuf_len((struct netbuf *)buf);
  }
#endif

#if LWIP_SO_RCVBUF
  SYS_ARCH_DEC(conn->recv_avail, len);
#endif /* LWIP_SO_RCVBUF */
  /* Register event with callback */
  API_EVENT(conn, NETCONN_EVT_RCVMINUS, len);

  LWIP_DEBUGF(API_LIB_DEBUG, ("netconn_recv_data: received %p, len=%"U16_F"\n", buf, len));

  *new_buf = buf;
  /* don't set conn->last_err: it's only ERR_OK, anyway */
  err = ERR_OK;
  return err;
}

/**
 * Receive data (in form of a pbuf) from a TCP netconn
 *
 * @param conn the netconn from which to receive data
 * @param new_buf pointer where a new pbuf is stored when received data
 * @return ERR_OK if data has been received, an error code otherwise (timeout,
 *                memory error or another error)
 *         ERR_ARG if conn is not a TCP netconn
 */
err_t
netconn_recv_tcp_pbuf(struct netconn *conn, struct pbuf **new_buf)
{
  LWIP_ERROR("netconn_recv: invalid conn", (conn != NULL) &&
             netconn_type(conn) == NETCONN_TCP, return ERR_VAL;);

  return netconn_recv_data(conn, (void **)new_buf);
}

/**
 * Receive data (in form of a netbuf containing a packet buffer) from a netconn
 *
 * @param conn the netconn from which to receive data
 * @param new_buf pointer where a new netbuf is stored when received data
 * @return ERR_OK if data has been received, an error code otherwise (timeout,
 *                memory error or another error)
 */
err_t
netconn_recv(struct netconn *conn, struct netbuf **new_buf)
{
#if ENABLE_NETCONN_INTERFACE && LWIP_TCP
  struct netbuf *buf = NULL;
  err_t err;
#endif /* LWIP_TCP */

  *new_buf = NULL;
  LWIP_ERROR("netconn_accept: invalid recvmbox", sys_mbox_valid(&conn->recvmbox)
    && netconn_mbox_is_active(conn), return ERR_CONN;);

#if ENABLE_NETCONN_INTERFACE && LWIP_TCP
#if (LWIP_UDP || LWIP_RAW)
  if (conn->type == NETCONN_TCP)
#endif /* (LWIP_UDP || LWIP_RAW) */
  {
    struct pbuf *p = NULL;
    /* This is not a listening netconn, since recvmbox is set */

    buf = (struct netbuf *)memp_malloc(MEMP_NETBUF);
    if (buf == NULL) {
      /*lint -e506*/
      NETCONN_SET_SAFE_ERR(conn, ERR_MEM);
      /*lint +e506*/      
      return ERR_MEM;
    }

    err = netconn_recv_data(conn, (void **)&p);
    if (err != ERR_OK) {
      memp_free(MEMP_NETBUF, buf);
      return err;
    }
    LWIP_ASSERT("p != NULL", p != NULL);

    buf->p = p;
    buf->ptr = p;
    buf->port = 0;
    ip_addr_set_any(&buf->addr);
    *new_buf = buf;
    /* don't set conn->last_err: it's only ERR_OK, anyway */
    return ERR_OK;
  }
#endif /* LWIP_TCP */
#if ENABLE_NETCONN_INTERFACE && (LWIP_TCP && (LWIP_UDP || LWIP_RAW)) /* LWIP_TCP */
  else
#endif
  {
#if (LWIP_UDP || LWIP_RAW)
    return netconn_recv_data(conn, (void **)new_buf);
#endif
  }
}

/**
 * TCP: update the receive window: by calling this, the application
 * tells the stack that it has processed data and is able to accept
 * new data.
 * ATTENTION: use with care, this is mainly used for sockets!
 * Can only be used when calling netconn_set_noautorecved(conn, 1) before.
 *
 * @param conn the netconn for which to update the receive window
 * @param length amount of data processed (ATTENTION: this must be accurate!)
 */
void
netconn_recved(struct netconn *conn, u32_t length)
{
#if LWIP_TCP
  err_t err;
  u8_t noautorecved;

  if(conn == NULL)
  {
    return;
  }

  netconn_get_noautorecved(conn, noautorecved);
  if ((conn->type == NETCONN_TCP) &&
      (noautorecved)) {
    struct api_msg msg;
    /* Let the stack know that we have taken the data. */
    /* TODO: Speedup: Don't block and wait for the answer here
       (to prevent multiple thread-switches). */
    msg.function = do_recv;
    msg.msg.conn = conn;
    msg.msg.msg.r.len = length;
    /* don't care for the return value of do_recv */
    TCPIP_RECV_APIMSG(&msg, conn, err);
    LWIP_UNUSED_ARG(err);
  }
#else /* LWIP_TCP */
  LWIP_UNUSED_ARG(conn);
  LWIP_UNUSED_ARG(length);
#endif /* LWIP_TCP */
}



/**
 * Send data over a UDP or RAW netconn (that is already connected).
 *
 * @param conn the UDP or RAW netconn over which to send data
 * @param buf a netbuf containing the data to send
 * @return ERR_OK if data was sent, any other err_t on error
 */
err_t
netconn_send(struct netconn *conn, struct netbuf *buf)
{
  struct api_msg msg;
  err_t err;

  LWIP_ERROR("netconn_send: invalid conn",  (conn != NULL), return ERR_VAL;);

  LWIP_DEBUGF(API_LIB_DEBUG, ("netconn_send: sending %"U16_F" bytes\n", buf->p->tot_len));
  msg.function = do_send;
  msg.msg.conn = conn;
  msg.msg.msg.b = buf;
  TCPIP_SEND_APIMSG(&msg, err);
  /*lint -e506*/
  NETCONN_SET_SAFE_ERR(conn, err);
  /*lint +e506*/  
  return err;
}

/**
 * Send data over a TCP netconn.
 *
 * @param conn the TCP netconn over which to send data
 * @param dataptr pointer to the application buffer that contains the data to send
 * @param size size of the application data to send
 * @param apiflags combination of following flags :
 * - NETCONN_COPY: data will be copied into memory belonging to the stack
 * - NETCONN_MORE: for TCP connection, PSH flag will be set on last segment sent
 * - NETCONN_DONTBLOCK: only write the data if all dat can be written at once
 * @param bytes_written pointer to a location that receives the number of written bytes
 * @return ERR_OK if data was sent, any other err_t on error
 */
err_t
netconn_write_partly(struct netconn *conn, const void *dataptr, size_t size,
                     u8_t apiflags, size_t *bytes_written)
{
  struct api_msg msg;
  err_t err;
  u8_t dontblock;

  LWIP_ERROR("netconn_write: invalid conn",  (conn != NULL), return ERR_VAL;);
  LWIP_ERROR("netconn_write: invalid conn->type",  (conn->type == NETCONN_TCP), return ERR_VAL;);
  if (size == 0) {
    return ERR_OK;
  }

  /** protection is added to avoid data race contidition of netconn flags */
  NETCONN_APIFLAGS_IS_NONBLOCKING(conn, apiflags, dontblock);

  if (dontblock && !bytes_written) {
    /* This implies netconn_write() cannot be used for non-blocking send, since
       it has no way to return the number of bytes written. */
    return ERR_VAL;
  }

  /* non-blocking write sends as much  */
  msg.function = do_write;
  msg.msg.conn = conn;
  msg.msg.msg.w.dataptr = dataptr;
  msg.msg.msg.w.apiflags = apiflags;
  msg.msg.msg.w.len = size;
#if LWIP_SO_SNDTIMEO
  if (conn->send_timeout != 0) {
    /* get the time we started, which is later compared  */
    msg.msg.msg.w.time_started = sys_now();
  } else {
    msg.msg.msg.w.time_started = 0;
  }
#endif /* LWIP_SO_SNDTIMEO */

  /* For locking the core: this _can_ be delayed on low memory/low send buffer,
     but if it is, this is done inside api_msg.c:do_write(), so we can use the
     non-blocking version here. */
  TCPIP_SEND_APIMSG(&msg, err);
  if ((err == ERR_OK) && (bytes_written != NULL)) {
    if (dontblock
#if LWIP_SO_SNDTIMEO
        || (conn->send_timeout != 0)
#endif /* LWIP_SO_SNDTIMEO */
       ) {
      /* nonblocking write: maybe the data has been sent partly */
      *bytes_written = msg.msg.msg.w.len;
    } else {
      /* blocking call succeeded: all data has been sent if it */
      *bytes_written = size;
    }
  }
  /*lint -e506*/
  NETCONN_SET_SAFE_ERR(conn, err);
  /*lint +e506*/  
  return err;
}

/**
 * Close ot shutdown a TCP netconn (doesn't delete it).
 *
 * @param conn the TCP netconn to close or shutdown
 * @param how fully close or only shutdown one side?
 * @return ERR_OK if the netconn was closed, any other err_t on error
 */
LWIP_STATIC err_t
netconn_close_shutdown(struct netconn *conn, u8_t how)
{
  struct api_msg msg;
  err_t err;

  LWIP_ERROR("netconn_close_shutdown: invalid conn",  (conn != NULL), return ERR_VAL;);

  msg.function = do_close;
  msg.msg.conn = conn;
  /* shutting down both ends is the same as closing */
  msg.msg.msg.sd.shut = how;
  /* because of the LWIP_TCPIP_CORE_LOCKING implementation of do_close,
     don't use TCPIP_APIMSG here */
  err = tcpip_apimsg(&msg, &msg.msg.conn->close_completed);
  /*lint -e506*/
  NETCONN_SET_SAFE_ERR(conn, err);
  /*lint +e506*/  
  return err;
}



/**
 * Shut down one or both sides of a TCP netconn (doesn't delete it).
 *
 * @param conn the TCP netconn to shut down
 * @return ERR_OK if the netconn was closed, any other err_t on error
 */
err_t
netconn_shutdown(struct netconn *conn, u8_t shut_rx, u8_t shut_tx)
{
  return netconn_close_shutdown(conn, (u8_t)((shut_rx ? NETCONN_SHUT_RD : 0) | (shut_tx ? NETCONN_SHUT_WR : 0)));
}




#if LWIP_DNS
/**
 * Execute a DNS query, only one IP_add is returned
 *
 * @param name a string representation of the DNS host name to query
 * @param addr a preallocated ip_addr_t where to store the resolved IP_add
 * @return ERR_OK: resolving succeeded
 *         ERR_MEM: memory error, try again later
 *         ERR_ARG: dns client not initialized or invalid hostname
 *         ERR_VAL: dns server response was invalid
 */
err_t
netconn_gethostbyname(const char *name, ip_addr_t *addr, u32_t *count)
{
  struct dns_api_msg msg;
  err_t err;
  sys_sem_t sem;

  LWIP_ERROR("netconn_gethostbyname: invalid name", (name != NULL), return ERR_VAL;);
  LWIP_ERROR("netconn_gethostbyname: invalid addr", (addr != NULL), return ERR_VAL;);

  err = sys_sem_new(&sem, 0);
  if (err != ERR_OK) {
    return err;
  }

  msg.name = name;
  msg.addr = addr;
  msg.err = &err;
  msg.sem = &sem;
  msg.count = count;

  tcpip_callback(do_gethostbyname, &msg);
  sys_sem_wait(&sem);
  sys_sem_free(&sem);

  return err;
}
#endif /* LWIP_DNS*/

/**
 * Get the connection information
 *
 * @param conn the netconn to query
 * @param conninfo a pointer to which to save the connection info
 * @return ERR_CONN for invalid connections
 *         ERR_OK if the information was retrieved
 */
err_t netconn_getconninfo(struct netconn *conn, void  * conn_info)
{
  struct api_msg msg;
  err_t err;
  struct tcpip_conn *conninfo;

  conninfo = (struct tcpip_conn *)conn_info;
  LWIP_ERROR("netconn_getconninfo: invalid conn", (conn != NULL), return ERR_VAL;);
  LWIP_ERROR("netconn_getconninfo: invalid conninfo", (conninfo != NULL), return ERR_VAL;);

  msg.function = do_getconninfo;
  msg.msg.conn = conn;
  msg.msg.msg.conn_info = conninfo;
  err = TCPIP_APIMSG(&msg);
  /*lint -e506*/
  NETCONN_SET_SAFE_ERR(conn, err);
  /*lint +e506*/  
  return err;
}

#endif /* LWIP_NETCONN */
