/**
 * @file
 * Sequential API Internal module
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

#if LWIP_NETCONN /* don't build if not configured for use in lwipopts.h */

#include "lwip/api_msg.h"
#include "lwip/ip.h"
#include "lwip/udp.h"
#include "lwip/tcp.h"
#include "lwip/tcp_impl.h"
#include "lwip/raw.h"

#include "lwip/memp.h"
#include "lwip/tcpip.h"
#include "lwip/igmp.h"
#include "lwip/dns.h"
#include "lwip/netif.h"

#include <string.h>

#define SET_NONBLOCKING_CONNECT(conn) { \
    (conn)->flags |= NETCONN_FLAG_IN_NONBLOCKING_CONNECT; \
}
#define CLEAR_NONBLOCKING_CONNECT(conn) { \
    (conn)->flags = ((conn)->flags & (~((u32_t) NETCONN_FLAG_IN_NONBLOCKING_CONNECT))); \
}

#define IN_NONBLOCKING_CONNECT(conn) (((conn)->flags & NETCONN_FLAG_IN_NONBLOCKING_CONNECT) != 0)

/* forward declarations */
#if LWIP_TCP
LWIP_STATIC err_t do_writemore(struct netconn *conn);
LWIP_STATIC void do_close_internal(struct netconn *conn);
#endif

#if LWIP_RAW
/**
 * Receive callback function for RAW netconns.
 * Doesn't 'eat' the packet, only references it and sends it to
 * conn->recvmbox
 *
 * @see raw.h (struct raw_pcb.recv) for parameters and return value
 */
LWIP_STATIC u8_t
recv_raw(void *arg, struct raw_pcb *pcb, struct pbuf *p,
    ip_addr_t *addr)
{
  struct pbuf *q;
  struct netbuf *buf;
  struct netconn *conn;

#if PF_PKT_SUPPORT
  /* Used below */
#else
  LWIP_UNUSED_ARG(addr);
#endif
  conn = (struct netconn *)arg;

  if ((conn != NULL) && sys_mbox_valid(&conn->recvmbox)) {
#if LWIP_SO_RCVBUF
    int recv_avail;
    SYS_ARCH_GET(conn->recv_avail, recv_avail);
    if ((recv_avail + (int)(p->tot_len)) > conn->recv_bufsize) {
      return 0;
    }
#endif /* LWIP_SO_RCVBUF */
    /* copy the whole packet into new pbufs */
    q = pbuf_alloc(PBUF_RAW, p->tot_len, PBUF_RAM);
    if(q != NULL) {
      if (pbuf_copy(q, p) != ERR_OK) {
        pbuf_free(q);
        q = NULL;
      }
    }

    if (q != NULL) {
      u16_t len;
      buf = (struct netbuf *)memp_malloc(MEMP_NETBUF);
      if (buf == NULL) {
        pbuf_free(q);
        return 0;
      }
#if PF_PKT_SUPPORT
      /* To get the pkt type filtered in ethernet_input */
      q->flags = p->flags;
#if ETH_PAD_SIZE
      /* exclude the begining two padding bytes in struct eth_hdr */
      if (conn->type == NETCONN_PKT_RAW) {
        if((ret = pbuf_header(q, -ETH_PAD_SIZE)) != 0)
        {
            (void)pbuf_free(q);
            memp_free(MEMP_NETBUF, buf);
            return ret;
        }
      }
#endif
#endif

      buf->p = q;
      buf->ptr = q;
#if PF_PKT_SUPPORT
      /* IP addr is NULL only when RAW packets are received for PF_PACKET sockets */
      if (addr == NULL) {
        ip_addr_set_any(&buf->addr);
        buf->port = eth_current_hdr()->type;
      } else {
        ip_addr_copy(buf->addr, *ip_current_src_addr());
        buf->port = pcb->proto.protocol;
      }
#else
      ip_addr_copy(buf->addr, *ip_current_src_addr());
      buf->port = pcb->protocol;
#endif

      len = q->tot_len;
      if (sys_mbox_trypost(&conn->recvmbox, buf) != ERR_OK) {
        netbuf_delete(buf);
        return 0;
      } else {
#if LWIP_SO_RCVBUF
        SYS_ARCH_INC(conn->recv_avail, len);
#endif /* LWIP_SO_RCVBUF */
        /* Register event with callback */
        API_EVENT(conn, NETCONN_EVT_RCVPLUS, len);
      }
    }
  }

  return 0; /* do not eat the packet */
}
#endif /* LWIP_RAW*/

#if LWIP_UDP
/**
 * Receive callback function for UDP netconns.
 * Posts the packet to conn->recvmbox or deletes it on memory error.
 *
 * @see udp.h (struct udp_pcb.recv) for parameters
 */
void
recv_udp(void *arg, struct udp_pcb *pcb, struct pbuf *p,
   ip_addr_t *addr, u16_t port)
{
  struct netbuf *buf;
  struct netconn *conn;
#if LWIP_SO_RCVBUF
  int recv_avail;
#endif /* LWIP_SO_RCVBUF */

  u16_t len;

  LWIP_UNUSED_ARG(pcb); /* only used for asserts... */
  LWIP_ASSERT("recv_udp must have a pcb argument", pcb != NULL);
  LWIP_ASSERT("recv_udp must have an argument", arg != NULL);
  conn = (struct netconn *)arg;

#if LWIP_SO_RCVBUF
  SYS_ARCH_GET(conn->recv_avail, recv_avail);
  /*lint -e774*/
  if ((conn == NULL) || !sys_mbox_valid(&conn->recvmbox) ||
      ((recv_avail + (int)(p->tot_len)) > conn->recv_bufsize)) {
  /*lint +e774*/
#else  /* LWIP_SO_RCVBUF */
  /* PC Lint Fix for: Boolean within 'left side of || within if' always evaluates to False */
  /* This is because NULL check for arg (which is assigned to conn) ias already done above. */
  /*lint -e774*/
  if ((conn == NULL) || !sys_mbox_valid(&conn->recvmbox)) {
  /*lint +e774*/
#endif /* LWIP_SO_RCVBUF */
    pbuf_free(p);
    return;
  }
  LWIP_ASSERT("recv_udp: recv for wrong pcb!", conn->pcb.udp == pcb);
  buf = (struct netbuf *)memp_malloc(MEMP_NETBUF);
  if (buf == NULL) {
    pbuf_free(p);
    return;
  } else {
    buf->p = p;
    buf->ptr = p;
    ip_addr_set(&buf->addr, addr);
    buf->port = port;
#if LWIP_NETBUF_RECVINFO
    {
      const struct ip_hdr* iphdr = ip_current_header();
      /* get the UDP header - always in the first pbuf, ensured by udp_input */
      const struct udp_hdr* udphdr = (void*)(((char*)iphdr) + IPH_LEN(iphdr));
#if LWIP_CHECKSUM_ON_COPY
      buf->flags = NETBUF_FLAG_DESTADDR;
#endif /* LWIP_CHECKSUM_ON_COPY */
      ip_addr_set(&buf->toaddr, ip_current_dest_addr());
      buf->toport_chksum = udphdr->dest;
    }
#endif /* LWIP_NETBUF_RECVINFO */
  }

  len = p->tot_len;
  if (sys_mbox_trypost(&conn->recvmbox, buf) != ERR_OK) {
    netbuf_delete(buf);
    return;
  } else {
#if LWIP_SO_RCVBUF
    SYS_ARCH_INC(conn->recv_avail, len);
#endif /* LWIP_SO_RCVBUF */
    /* Register event with callback */
    API_EVENT(conn, NETCONN_EVT_RCVPLUS, len);
  }
}
#endif /* LWIP_UDP */

#if LWIP_TCP
/**
 * Receive callback function for TCP netconns.
 * Posts the packet to conn->recvmbox, but doesn't delete it on errors.
 *
 * @see tcp.h (struct tcp_pcb.recv) for parameters and return value
 */
LWIP_STATIC err_t
recv_tcp(void *arg, struct tcp_pcb *pcb, struct pbuf *p, err_t err)
{
  struct netconn *conn;
  u16_t len;

  LWIP_UNUSED_ARG(pcb);
  LWIP_ASSERT("recv_tcp must have a pcb argument", pcb != NULL);
  LWIP_ASSERT("recv_tcp must have an argument", arg != NULL);
  conn = (struct netconn *)arg;
  LWIP_ASSERT("recv_tcp: recv for wrong pcb!", conn->pcb.tcp == pcb);

  if (!sys_mbox_valid(&conn->recvmbox)) {
    /* recvmbox already deleted */
    if (p != NULL) {
      tcp_recved(pcb, p->tot_len);
      pbuf_free(p);
    }
    return ERR_OK;
  }
  /* Unlike for UDP or RAW pcbs, don't check for available space
     using recv_avail since that could break the connection
     (data is already ACKed) */

  /* don't overwrite fatal errors! */
  NETCONN_SET_SAFE_ERR(conn, err);

  if (p != NULL) {
    len = p->tot_len;
  } else {
    len = 0;
  }

  if (sys_mbox_trypost(&conn->recvmbox, p) != ERR_OK) {
    /* don't deallocate p: it is presented to us later again from tcp_fasttmr! */
    return ERR_MEM;
  } else {
#if LWIP_SO_RCVBUF
    SYS_ARCH_INC(conn->recv_avail, len);
#endif /* LWIP_SO_RCVBUF */
    /* Register event with callback */
    API_EVENT(conn, NETCONN_EVT_RCVPLUS, len);
  }

  return ERR_OK;
}

/**
 * Poll callback function for TCP netconns.
 * Wakes up an application thread that waits for a connection to close
 * or data to be sent. The application thread then takes the
 * appropriate action to go on.
 *
 * Signals the conn->sem.
 *
 * @see tcp.h (struct tcp_pcb.poll) for parameters and return value
 */
LWIP_STATIC err_t
poll_tcp(void *arg, struct tcp_pcb *pcb)
{
  struct netconn *conn = (struct netconn *)arg;

  LWIP_UNUSED_ARG(pcb);
  LWIP_ASSERT("conn != NULL", (conn != NULL));

  if (conn->state == NETCONN_WRITE) {
    do_writemore(conn);
  } else if (conn->state == NETCONN_CLOSE) {
    do_close_internal(conn);
  }

  /* @todo: implement connect timeout here? */

  /* Did a nonblocking write fail before? Then check available write-space. */
  if (conn->flags & NETCONN_FLAG_CHECK_WRITESPACE) {
    /* If the queued byte- or pbuf-count drops below the configured low-water limit,
       let select mark this pcb as writable again. */
    if ((conn->pcb.tcp != NULL) && (tcp_sndbuf(conn->pcb.tcp) > CFG_TCP_SNDLOWAT) &&
      (tcp_sndqueuelen(conn->pcb.tcp) < CFG_TCP_SNDQUEUELOWAT)) {
      conn->flags = conn->flags & (~((u32_t)NETCONN_FLAG_CHECK_WRITESPACE));
      API_EVENT(conn, NETCONN_EVT_SENDPLUS, 0);
    }
  }

  return ERR_OK;
}

/**
 * Sent callback function for TCP netconns.
 * Signals the conn->sem and calls API_EVENT.
 * netconn_write waits for conn->sem if send buffer is low.
 *
 * @see tcp.h (struct tcp_pcb.sent) for parameters and return value
 */
LWIP_STATIC err_t
sent_tcp(void *arg, struct tcp_pcb *pcb, u16_t len)
{
  struct netconn *conn = (struct netconn *)arg;

  LWIP_UNUSED_ARG(pcb);
  if(conn == NULL)
    return ERR_ARG;

  LWIP_ASSERT("conn != NULL", (conn != NULL));
  if (conn->state == NETCONN_WRITE) {
    do_writemore(conn);
  } else if (conn->state == NETCONN_CLOSE) {
    do_close_internal(conn);
  }

  /* PC Lint Fix for: Boolean within 'if' always evaluates to True */
  /* conn NULL check is already done in assert, thats why this Warning */
  /*lint -e774*/
  /* If the queued byte- or pbuf-count drops below the configured low-water limit,
     let select mark this pcb as writable again. */
  if ((conn->pcb.tcp != NULL) && (tcp_sndbuf(conn->pcb.tcp) > CFG_TCP_SNDLOWAT) &&
    (tcp_sndqueuelen(conn->pcb.tcp) < CFG_TCP_SNDQUEUELOWAT)) {
    conn->flags = conn->flags & (~((u32_t)NETCONN_FLAG_CHECK_WRITESPACE));
    API_EVENT(conn, NETCONN_EVT_SENDPLUS, len);
  }
  /*lint -e774*/

  return ERR_OK;
}

/**
 * Error callback function for TCP netconns.
 * Signals conn->sem, posts to all conn mboxes and calls API_EVENT.
 * The application thread has then to decide what to do.
 *
 * @see tcp.h (struct tcp_pcb.err) for parameters
 */
LWIP_STATIC void
err_tcp(void *arg, err_t err)
{
  struct netconn *conn;
  int old_state;

  conn = (struct netconn *)arg;
  LWIP_ASSERT("conn != NULL", (conn != NULL));

  conn->pcb.tcp = NULL;

  /* no check since this is always fatal! */
  NETCONN_SET_SAFE_FATAL_ERR(conn, err);

  /* reset conn->state now before waking up other threads */
  old_state = conn->state;
  conn->state = NETCONN_NONE;

  /* Notify the user layer about a connection error. Used to signal
     select. */
  API_EVENT(conn, NETCONN_EVT_ERROR, 0);
  /* Try to release selects pending on 'read' or 'write', too.
     They will get an error if they actually try to read or write. */
  API_EVENT(conn, NETCONN_EVT_RCVPLUS, 0);
  API_EVENT(conn, NETCONN_EVT_SENDPLUS, 0);

  /* pass NULL-message to recvmbox to wake up pending recv */
  if (sys_mbox_valid(&conn->recvmbox)) {
    /* use trypost to prevent deadlock */
    (void)sys_mbox_trypost(&conn->recvmbox, NULL);
  }
  /* pass NULL-message to acceptmbox to wake up pending accept */
  if (sys_mbox_valid(&conn->acceptmbox)) {
    /* use trypost to preven deadlock */
    (void)sys_mbox_trypost(&conn->acceptmbox, NULL);
  }

  if ((old_state == NETCONN_WRITE) || (old_state == NETCONN_CLOSE) ||
      (old_state == NETCONN_CONNECT)) {
    /* calling do_writemore/do_close_internal is not necessary
       since the pcb has already been deleted! */
    int was_nonblocking_connect = IN_NONBLOCKING_CONNECT(conn);
    CLEAR_NONBLOCKING_CONNECT(conn);

    if (!was_nonblocking_connect) {
      /* set error return code */
      LWIP_ASSERT("conn->current_msg != NULL", conn->current_msg != NULL);
      conn->current_msg->err = err;
      conn->current_msg = NULL;
      /* wake up the waiting task */
      if (old_state == NETCONN_WRITE)
      {
        sys_sem_signal(&conn->send_completed);
        return;
      }

      if (old_state == NETCONN_CONNECT)
      {
        sys_sem_signal(&conn->op_completed);
      }

      if (old_state == NETCONN_CLOSE)
      {
        sys_sem_signal(&conn->close_completed);
      }
    }
  } else {
    LWIP_ASSERT("conn->current_msg == NULL", conn->current_msg == NULL);
  }
}

void update_tcp_sndplus_event(void *arg, struct tcp_pcb *pcb)
{
  struct netconn *conn = (struct netconn *)arg;

  LWIP_UNUSED_ARG(pcb);
  LWIP_ASSERT("conn != NULL", (conn != NULL));

  if (netconn_is_nonblocking(conn)) {
    API_EVENT(conn, NETCONN_EVT_SENDPLUS, 0);
  }
  return;
}


/**
 * Setup a tcp_pcb with the correct callback function pointers
 * and their arguments.
 *
 * @param conn the TCP netconn to setup
 */
LWIP_STATIC void
setup_tcp(struct netconn *conn)
{
  struct tcp_pcb *pcb;

  pcb = conn->pcb.tcp;
  tcp_arg(pcb, conn);
  tcp_recv(pcb, recv_tcp);
  tcp_sent(pcb, sent_tcp);
  tcp_poll(pcb, poll_tcp, 4);
  tcp_err(pcb, err_tcp);

/* NETIF DRIVER STATUS BEGIN */
#if DRIVER_STATUS_CHECK
  pcb->sndplus =  update_tcp_sndplus_event;
#endif
/* NETIF DRIVER STATUS END*/

}

/**
 * Accept callback function for TCP netconns.
 * Allocates a new netconn and posts that to conn->acceptmbox.
 *
 * @see tcp.h (struct tcp_pcb_listen.accept) for parameters and return value
 */
LWIP_STATIC err_t
accept_function(void *arg, struct tcp_pcb *newpcb, err_t err)
{
  struct netconn *newconn;
  struct netconn *conn = (struct netconn *)arg;

  LWIP_DEBUGF(API_MSG_DEBUG, ("accept_function: newpcb->tate: %s\n", tcp_debug_state_str(newpcb->state)));


  if (conn->type != NETCONN_TCP)
      return ERR_VAL;


  if (!sys_mbox_valid(&conn->acceptmbox) || !netconn_mbox_is_active(conn)) {
    LWIP_DEBUGF(API_MSG_DEBUG, ("accept_function: acceptmbox already deleted or getting deleted\n"));
    return ERR_VAL;
  }

  /* We have to set the callback here even though
   * the new socket is unknown. conn->socket is marked as -1. */
  newconn = netconn_alloc(conn->type, conn->callback);
  if (newconn == NULL) {
    return ERR_MEM;
  }
  newconn->pcb.tcp = newpcb;
  setup_tcp(newconn);
  /* no protection: when creating the pcb, the netconn is not yet known
     to the application thread */
  newconn->last_err = err;

  if (sys_mbox_trypost(&conn->acceptmbox, newconn) != ERR_OK) {
    /* When returning != ERR_OK, the pcb is aborted in tcp_process(),
       so do nothing here! */
    /* remove all references to this netconn from the pcb */
    struct tcp_pcb* pcb = newconn->pcb.tcp;
    tcp_arg(pcb, NULL);
    tcp_recv(pcb, NULL);
    tcp_sent(pcb, NULL);
    tcp_poll(pcb, NULL, 4);
    tcp_err(pcb, NULL);
    /* remove reference from to the pcb from this netconn */
    newconn->pcb.tcp = NULL;
    /* no need to drain since we know the recvmbox is empty. */
    /* netconn_free accepts acceptbox state to be inactive */
    NETCONN_SET_SAFE_MBOX_STATE(newconn, NETCONN_MBOX_DELETING);
    netconn_free(newconn);
    return ERR_MEM;
  } else {
    /* Register event with callback */
    API_EVENT(conn, NETCONN_EVT_RCVPLUS, 0);
  }

  return ERR_OK;
}
#endif /* LWIP_TCP */

/**
 * Create a new pcb of a specific type.
 * Called from do_newconn().
 *
 * @param msg the api_msg_msg describing the connection type
 * @return msg->conn->err, but the return value is currently ignored
 */
LWIP_STATIC void
pcb_new(struct api_msg_msg *msg)
{
  LWIP_ASSERT("pcb_new: pcb already allocated", msg->conn->pcb.tcp == NULL);

  /* Allocate a PCB for this connection */
  switch(NETCONNTYPE_GROUP(msg->conn->type)) {
#if LWIP_RAW
  case NETCONN_RAW:
#if PF_PKT_SUPPORT
    msg->conn->pcb.raw = raw_new((u8_t)msg->msg.n.proto);
#else
    msg->conn->pcb.raw = raw_new(msg->msg.n.proto);
#endif
    if(msg->conn->pcb.raw == NULL) {
      msg->err = ERR_MEM;
      break;
    }
    raw_recv(msg->conn->pcb.raw, recv_raw, msg->conn);
    break;
#if PF_PKT_SUPPORT
  case NETCONN_PKT_RAW:
    msg->conn->pcb.pkt_raw = raw_pkt_new(msg->msg.n.proto);
    if(msg->conn->pcb.pkt_raw == NULL) {
      msg->err = ERR_MEM;
      break;
    }
    raw_recv(msg->conn->pcb.pkt_raw, recv_raw, msg->conn);
    break;
#endif
#endif /* LWIP_RAW */
#if LWIP_UDP
  case NETCONN_UDP:
    msg->conn->pcb.udp = udp_new();
    if(msg->conn->pcb.udp == NULL) {
      msg->err = ERR_MEM;
      break;
    }
#if LWIP_UDPLITE
    if (msg->conn->type==NETCONN_UDPLITE) {
      udp_setflags(msg->conn->pcb.udp, UDP_FLAGS_UDPLITE);
    }
#endif /* LWIP_UDPLITE */
    if (msg->conn->type==NETCONN_UDPNOCHKSUM) {
      udp_setflags(msg->conn->pcb.udp, UDP_FLAGS_NOCHKSUM);
    }
    udp_recv(msg->conn->pcb.udp, recv_udp, msg->conn);
    break;
#endif /* LWIP_UDP */
#if LWIP_TCP
  case NETCONN_TCP:
    msg->conn->pcb.tcp = tcp_new();
    if(msg->conn->pcb.tcp == NULL) {
      msg->err = ERR_MEM;
      break;
    }
    setup_tcp(msg->conn);
    break;
#endif /* LWIP_TCP */
  default:
    /* Unsupported netconn type, e.g. protocol disabled */
    msg->err = ERR_VAL;
    break;
  }
}

/**
 * Create a new pcb of a specific type inside a netconn.
 * Called from netconn_new_with_proto_and_callback.
 *
 * @param msg the api_msg_msg describing the connection type
 */
void
do_newconn(struct api_msg_msg *msg)
{
  msg->err = ERR_OK;
  if(msg->conn->pcb.tcp == NULL) {
    pcb_new(msg);
  }
  /* Else? This "new" connection already has a PCB allocated. */
  /* Is this an error condition? Should it be deleted? */
  /* We currently just are happy and return. */

  TCPIP_APIMSG_ACK(msg);
}

/**
 * Create a new netconn (of a specific type) that has a callback function.
 * The corresponding pcb is NOT created!
 *
 * @param t the type of 'connection' to create (@see enum netconn_type)
 * @param proto the IP protocol for RAW IP pcbs
 * @param callback a function to call on status changes (RX available, TX'ed)
 * @return a newly allocated struct netconn or
 *         NULL on memory error
 */
struct netconn*
netconn_alloc(int t, netconn_callback callback)
{
  struct netconn *conn;
  int size;

  conn = (struct netconn *)memp_malloc(MEMP_NETCONN);
  if (conn == NULL) {
    return NULL;
  }

  MEMSET_S(conn, sizeof(struct netconn), 0, sizeof(struct netconn));
  conn->last_err = ERR_OK;
  conn->type = t;
  conn->pcb.tcp = NULL;

#if (DEFAULT_RAW_RECVMBOX_SIZE == DEFAULT_UDP_RECVMBOX_SIZE) && \
    (DEFAULT_RAW_RECVMBOX_SIZE == DEFAULT_TCP_RECVMBOX_SIZE)
  size = DEFAULT_RAW_RECVMBOX_SIZE;
#else
  switch(NETCONNTYPE_GROUP(t)) {
#if LWIP_RAW
#if PF_PKT_SUPPORT
  case NETCONN_PKT_RAW:
#endif
  case NETCONN_RAW:
    size = DEFAULT_RAW_RECVMBOX_SIZE;
    break;
#endif /* LWIP_RAW */
#if LWIP_UDP
  case NETCONN_UDP:
    size = DEFAULT_UDP_RECVMBOX_SIZE;
    break;
#endif /* LWIP_UDP */
#if LWIP_TCP
  case NETCONN_TCP:
    size = DEFAULT_TCP_RECVMBOX_SIZE;
    break;
#endif /* LWIP_TCP */
  default:
    LWIP_ASSERT("netconn_alloc: undefined netconn_type", 0);
    goto free_and_return;
  }
#endif
  if (sys_mutex_new(&conn->conn_mutex) != ERR_OK) {
    goto free_and_return;
  }
  /* adding recv and send sem for protection of single socket over multithread */
  if (sys_sem_new(&conn->recv_completed, 0) != ERR_OK) {
    goto free_and_return1;
  }

  if (sys_sem_new(&conn->send_completed, 0) != ERR_OK) {
    goto free_and_return2;
  }

  if (sys_sem_new(&conn->op_completed, 0) != ERR_OK) {
    goto free_and_return3;
  }

  if (sys_sem_new(&conn->close_completed, 0) != ERR_OK) {
    goto free_and_return4;
  }

  if (sys_mbox_new(&conn->recvmbox, size, MBOX_NO_EXPAND) != ERR_OK) {
    goto free_and_return5;
  }

  if(sys_mutex_new(&conn->send_mutex) != ERR_OK) {
    goto free_and_return6;
  }

#if LWIP_TCP
  sys_mbox_set_invalid(&conn->acceptmbox);
#endif

  conn->mbox_state = NETCONN_MBOX_ACTIVE;
  conn->state        = NETCONN_NONE;
#if LWIP_SOCKET
  /* initialize socket to -1 since 0 is a valid socket */
  conn->socket       = -1;
#endif /* LWIP_SOCKET */
  conn->callback     = callback;
#if LWIP_TCP
  conn->current_msg  = NULL;
  conn->write_offset = 0;
#endif /* LWIP_TCP */
#if LWIP_SO_SNDTIMEO
  conn->send_timeout = 0;
#endif /* LWIP_SO_SNDTIMEO */
#if LWIP_SO_RCVTIMEO
  conn->recv_timeout = 0;
#endif /* LWIP_SO_RCVTIMEO */
#if LWIP_SO_RCVBUF
  conn->recv_bufsize = RECV_BUFSIZE_DEFAULT;
  conn->recv_avail   = 0;
#endif /* LWIP_SO_RCVBUF */
  conn->flags = 0;
  conn->flags_usr = 0;

  atomic_set((volatile UINT32 *)&conn->flags_blocking,0);


  atomic_set((volatile UINT32 *)&conn->recv_count ,0);
  atomic_set((volatile UINT32 *)&conn->is_err_fatal,0);

  return conn;
free_and_return6:
    sys_mbox_free(&conn->recvmbox);
free_and_return5:
  sys_sem_free(&conn->close_completed);
free_and_return4:
  sys_sem_free(&conn->op_completed);
free_and_return3:
  sys_sem_free(&conn->send_completed);
free_and_return2:
  sys_sem_free(&conn->recv_completed);

free_and_return1:
  sys_mutex_free(&conn->conn_mutex);
free_and_return:
  memp_free(MEMP_NETCONN, conn);
  return NULL;
}

/**
 * Delete a netconn and all its resources.
 * The pcb is NOT freed (since we might not be in the right thread context do this).
 *
 * @param conn the netconn to free
 */
void
netconn_free(struct netconn *conn)
{
  LWIP_ASSERT("PCB must be deallocated outside this function", conn->pcb.tcp == NULL);


  /** mbox should be in deleting state here */
  LWIP_ASSERT("acceptmbox must be inactive before calling this function",
    !netconn_mbox_is_active(conn));

  if (sys_mbox_valid(&conn->recvmbox))
  {
    sys_mbox_free(&conn->recvmbox);
    sys_mbox_set_invalid(&conn->recvmbox);
  }
#if LWIP_TCP
  if (sys_mbox_valid(&conn->acceptmbox))
  {
    sys_mbox_free(&conn->acceptmbox);
    sys_mbox_set_invalid(&conn->acceptmbox);
  }
#endif /* LWIP_TCP */

  sys_sem_free(&conn->op_completed);
  sys_sem_set_invalid(&conn->op_completed);

  sys_sem_free(&conn->send_completed);
  sys_sem_set_invalid(&conn->send_completed);

  sys_sem_free(&conn->recv_completed);
  sys_sem_set_invalid(&conn->recv_completed);

  sys_sem_free(&conn->close_completed);
  sys_sem_set_invalid(&conn->close_completed);

  sys_mutex_free(&conn->send_mutex);
  sys_mutex_set_invalid(&conn->send_mutex);

  sys_mutex_free(&conn->conn_mutex);
  sys_mutex_set_invalid(&conn->conn_mutex);

  memp_free(MEMP_NETCONN, conn);
}

/**
 * Delete rcvmbox and acceptmbox of a netconn and free the left-over data in
 * these mboxes
 *
 * @param conn the netconn to free
 * @bytes_drained bytes drained from recvmbox
 * @accepts_drained pending connections drained from acceptmbox
 */
LWIP_STATIC void
netconn_drain(struct netconn *conn)
{
  void *mem = NULL;
#if LWIP_TCP
  struct pbuf *p;
#endif /* LWIP_TCP */

  /* This runs in tcpip_thread, so we don't need to lock against rx packets */

  /* Delete and drain the recvmbox. */
  if (sys_mbox_valid(&conn->recvmbox)) {
    while (sys_arch_mbox_fetch(&conn->recvmbox, &mem, 1) != SYS_MBOX_EMPTY) {
#if LWIP_TCP
      if (conn->type == NETCONN_TCP) {
        if(mem != NULL) {
          p = (struct pbuf*)mem;
          /* pcb might be set to NULL already by err_tcp() */
          if (conn->pcb.tcp != NULL) {
            tcp_recved(conn->pcb.tcp, p->tot_len);
          }
          pbuf_free(p);
        }
      } else
#endif /* LWIP_TCP */
      {
        netbuf_delete((struct netbuf *)mem);
      }
    }

    /* pass NULL-message to recvmbox to wake up pending recv */
    /* use trypost to prevent deadlock */
    (void)sys_mbox_trypost(&conn->recvmbox, NULL);
  }

  /* Delete and drain the acceptmbox. */
#if LWIP_TCP
  if (sys_mbox_valid(&conn->acceptmbox)) {
    while (sys_arch_mbox_fetch(&conn->acceptmbox, &mem, 1) != SYS_MBOX_EMPTY) {
      struct netconn *newconn = (struct netconn *)mem;
      /* Only tcp pcbs have an acceptmbox, so no need to check conn->type */
      /* pcb might be set to NULL already by err_tcp() */
      if (conn->pcb.tcp != NULL) {
        tcp_accepted(conn->pcb.tcp);
      }
      /* drain recvmbox */
      netconn_drain(newconn);
      if (newconn->pcb.tcp != NULL) {
        tcp_abort(newconn->pcb.tcp);
        newconn->pcb.tcp = NULL;
      }
      netconn_free(newconn);
    }

    /* pass NULL-message to acceptmbox to wake up pending accept */
    /* use trypost to preven deadlock */
    (void)sys_mbox_trypost(&conn->acceptmbox, NULL);
  }
#endif /* LWIP_TCP */
  NETCONN_SET_SAFE_MBOX_STATE(conn, NETCONN_MBOX_DELETING);

}

#if LWIP_TCP
/**
 * Internal helper function to close a TCP netconn: since this sometimes
 * doesn't work at the first attempt, this function is called from multiple
 * places.
 *
 * @param conn the TCP netconn to close
 */
LWIP_STATIC void
do_close_internal(struct netconn *conn)
{
  err_t err;
  u8_t shut, shut_rx, shut_tx, ucClose;

  LWIP_ASSERT("invalid conn", (conn != NULL));
  LWIP_ASSERT("this is for tcp netconns only", (conn->type == NETCONN_TCP));
  LWIP_ASSERT("conn must be in state NETCONN_CLOSE", (conn->state == NETCONN_CLOSE));
  LWIP_ASSERT("pcb already closed", (conn->pcb.tcp != NULL));
  LWIP_ASSERT("conn->current_msg != NULL", conn->current_msg != NULL);

  shut = conn->current_msg->msg.sd.shut;
  shut_rx = shut & NETCONN_SHUT_RD;
  shut_tx = shut & NETCONN_SHUT_WR;
  /* shutting down both ends is the same as closing */
  ucClose = shut == NETCONN_SHUT_RDWR;

  /* Set back some callback pointers */
  if (ucClose) {
    tcp_arg(conn->pcb.tcp, NULL);
  }
  if (conn->pcb.tcp->state == LISTEN) {
    tcp_accept(conn->pcb.tcp, NULL);
  } else {
    /* some callbacks have to be reset if tcp_close is not successful */
    if (shut_rx) {
      tcp_recv(conn->pcb.tcp, NULL);
      tcp_accept(conn->pcb.tcp, NULL);
    }
    if (shut_tx) {
      tcp_sent(conn->pcb.tcp, NULL);
    }
    if (ucClose) {
      tcp_poll(conn->pcb.tcp, NULL, 4);
      tcp_err(conn->pcb.tcp, NULL);
    }
  }
  /* Try to close the connection */
  if (ucClose) {
    if (!atomic_read(&conn->is_err_fatal)) {
	  NETCONN_SET_SAFE_FATAL_ERR(conn, ERR_CLSD);
    }

    err = tcp_close(conn->pcb.tcp);
  } else {
    err = tcp_shutdown(conn->pcb.tcp, shut_rx, shut_tx);
  }
  if (err == ERR_OK) {
    /* Closing succeeded */
    sys_mutex_lock(&conn->conn_mutex);
    conn->current_msg->err = ERR_OK;
    conn->current_msg = NULL;

    sys_mutex_unlock(&conn->conn_mutex);

    conn->state = NETCONN_NONE;
    if (ucClose) {
      /* Set back some callback pointers as conn is going away */
      conn->pcb.tcp = NULL;
      /* Trigger select() in socket layer. Make sure everybody notices activity
       on the connection, error first! */
      API_EVENT(conn, NETCONN_EVT_ERROR, 0);
    }
    if (shut_rx) {
      API_EVENT(conn, NETCONN_EVT_RCVPLUS, 0);
    }
    if (shut_tx) {
      API_EVENT(conn, NETCONN_EVT_SENDPLUS, 0);
    }
    /* wake up the application task */

    sys_sem_signal(&conn->close_completed);

    /* NOTE : op_completed is last instruction to this function*/
  } else {
    /* Closing failed, restore some of the callbacks */
    /* Closing of listen pcb will never fail! */
    LWIP_ASSERT("Closing a listen pcb may not fail!", (conn->pcb.tcp->state != LISTEN));
    tcp_sent(conn->pcb.tcp, sent_tcp);
    tcp_poll(conn->pcb.tcp, poll_tcp, 4);
    tcp_err(conn->pcb.tcp, err_tcp);
    tcp_arg(conn->pcb.tcp, conn);
    /* don't restore recv callback: we don't want to receive any more data */

    /* tcp_shutdown can return ERR_CON if lwip_shutdown is called with SHUT_WR */
    /* in this case below sem signal is added to avoid hand in application thread */
    if (shut_tx) {
      sys_mutex_lock(&conn->conn_mutex);
      conn->current_msg->err = err;
      conn->current_msg = NULL;
      conn->state = NETCONN_NONE;
      sys_mutex_unlock(&conn->conn_mutex);
      /* wake up the application task */
      sys_sem_signal(&conn->close_completed);


    }
  }
  /* If closing didn't succeed, we get called again either
     from poll_tcp or from sent_tcp */
}
#endif /* LWIP_TCP */

/**
 * Delete the pcb inside a netconn.
 * Called from netconn_delete.
 *
 * @param msg the api_msg_msg pointing to the connection
 */
void
do_delconn(struct api_msg_msg *msg)
{
  /* @todo TCP: abort running write/connect? */
 atomic_inc(&msg->conn->recv_count);
 if ((msg->conn->state != NETCONN_NONE) &&
     (msg->conn->state != NETCONN_LISTEN) &&
     (msg->conn->state != NETCONN_CONNECT)&&
     (msg->conn->state != NETCONN_WRITE)) {
    /* this only happens for TCP netconns */
    LWIP_ASSERT("msg->conn->type == NETCONN_TCP", msg->conn->type == NETCONN_TCP);
    msg->err = ERR_INPROGRESS;
  } else if (msg->conn->state == NETCONN_CONNECT && !IN_NONBLOCKING_CONNECT(msg->conn))
  {
      /* Pending message for connect needs to be cleared if blocking connect*/
      tcp_abort(msg->conn->pcb.tcp);
  }
  else {
    LWIP_ASSERT("blocking connect in progress",
      (msg->conn->state != NETCONN_CONNECT) || IN_NONBLOCKING_CONNECT(msg->conn));
    /* Drain and delete mboxes */
    netconn_drain(msg->conn);
    if (msg->conn->state == NETCONN_WRITE){
      tcp_abort(msg->conn->pcb.tcp);
      /* this must be last instruction, other close thread might wake up and
       and cleanup resources */

      goto DELCONN_COMPLETE;
    }

    if (msg->conn->pcb.tcp != NULL) {

      switch (NETCONNTYPE_GROUP(msg->conn->type)) {
#if LWIP_RAW
      case NETCONN_RAW:
        raw_remove(msg->conn->pcb.raw);
        msg->conn->pcb.raw = NULL;
        break;
#if PF_PKT_SUPPORT
      case NETCONN_PKT_RAW:
        raw_pkt_remove(msg->conn->pcb.pkt_raw);
        break;
#endif
#endif /* LWIP_RAW */
#if LWIP_UDP
      case NETCONN_UDP:
        msg->conn->pcb.udp->recv_arg = NULL;
        udp_remove(msg->conn->pcb.udp);
        msg->conn->pcb.udp = NULL;
        break;
#endif /* LWIP_UDP */
#if LWIP_TCP
      case NETCONN_TCP:

        LWIP_ASSERT("already writing or closing", msg->conn->current_msg == NULL &&
          msg->conn->write_offset == 0);
        msg->conn->state = NETCONN_CLOSE;
        msg->msg.sd.shut = NETCONN_SHUT_RDWR;
        msg->conn->current_msg = msg;
        do_close_internal(msg->conn);
        /* API_EVENT is called inside do_close_internal, before releasing
           the application thread, so we can return at this point! */
        return;
#endif /* LWIP_TCP */
      default:
        break;
      }
      msg->conn->pcb.tcp = NULL;
    }
    /* tcp netconns don't come here! */

    /* @todo: this lets select make the socket readable and writable,
       which is wrong! errfd instead? */
    API_EVENT(msg->conn, NETCONN_EVT_RCVPLUS, 0);
    API_EVENT(msg->conn, NETCONN_EVT_SENDPLUS, 0);
  }

  DELCONN_COMPLETE:
  /* wake up the application task */
  if (atomic_dec_and_test(&msg->conn->recv_count)){
    sys_sem_signal(&msg->conn->recv_completed);
  }

  if (sys_sem_valid(&msg->conn->close_completed)) {
    sys_sem_signal(&msg->conn->close_completed);
  }
}

/**
 * Bind a pcb contained in a netconn
 * Called from netconn_bind.
 *
 * @param msg the api_msg_msg pointing to the connection and containing
 *            the IP_add and port to bind to
 */
void
do_bind(struct api_msg_msg *msg)
{
  if (atomic_read(&msg->conn->is_err_fatal) || msg->conn->state == NETCONN_CLOSED) {
    sys_mutex_lock(&msg->conn->conn_mutex);
    msg->err = (err_t)((msg->conn->state == NETCONN_CLOSED)?ERR_VAL:
	    msg->conn->last_err);
    sys_mutex_unlock(&msg->conn->conn_mutex);
  } else {
    /* PCB should not be NULL and IP_add should be either ANY or valid in netif */
    /* Here new IPaddress check is added to fail bind on IP_add which is not present in netif */
    if (msg->conn->pcb.tcp == NULL) {
	  msg->err = ERR_VAL;
	} else {
	  if (ip_addr_isany(msg->msg.bc.ipaddr) || (netif_find_by_ipaddr(msg->msg.bc.ipaddr) != NULL)) {
	    switch (NETCONNTYPE_GROUP(msg->conn->type)) {
#if LWIP_RAW
        case NETCONN_RAW:
          msg->err = raw_bind(msg->conn->pcb.raw, msg->msg.bc.ipaddr);
          break;
#if PF_PKT_SUPPORT
        case NETCONN_PKT_RAW:
          msg->err = raw_pkt_bind(msg->conn->pcb.pkt_raw, msg->msg.bc.netifindex, msg->msg.bc.port);
          break;
#endif
#endif /* LWIP_RAW */
#if LWIP_UDP
        case NETCONN_UDP:
          msg->err = udp_bind(msg->conn->pcb.udp, msg->msg.bc.ipaddr, msg->msg.bc.port);
          break;
#endif /* LWIP_UDP */
#if LWIP_TCP
        case NETCONN_TCP:
          msg->err = tcp_bind(msg->conn->pcb.tcp, msg->msg.bc.ipaddr, msg->msg.bc.port);
          break;
#endif /* LWIP_TCP */
 	    default:
 	      msg->err = ERR_OPNOTSUPP;
	      break;
	    }
	  } else {
	    msg->err = ERR_NOADDR;
      }
    }
  }
  TCPIP_APIMSG_ACK(msg);
}

#if LWIP_TCP
/**
 * TCP callback function if a connection (opened by tcp_connect/do_connect) has
 * been established (or reset by the remote host).
 *
 * @see tcp.h (struct tcp_pcb.connected) for parameters and return values
 */
LWIP_STATIC err_t
do_connected(void *arg, struct tcp_pcb *pcb, err_t err)
{
  struct netconn *conn;
  int was_blocking;

  LWIP_UNUSED_ARG(pcb);

  conn = (struct netconn *)arg;
  LWIP_ASSERT("conn != NULL", conn != NULL);

  was_blocking = IN_NONBLOCKING_CONNECT(conn);



  LWIP_ASSERT("conn->state == NETCONN_CONNECT", conn->state == NETCONN_CONNECT);
  LWIP_ASSERT("(conn->current_msg != NULL) || conn->in_non_blocking_connect",
    (conn->current_msg != NULL) || was_blocking);

  if (conn->current_msg != NULL) {
    conn->current_msg->err = err;
  }

  if ((conn->type == NETCONN_TCP) && (err == ERR_OK)) {
    setup_tcp(conn);
  }

  was_blocking = !IN_NONBLOCKING_CONNECT(conn);
  CLEAR_NONBLOCKING_CONNECT(conn);
  conn->current_msg = NULL;
  conn->state = NETCONN_NONE;
  if (!was_blocking) {
    /*lint -e506*/
    NETCONN_SET_SAFE_ERR(conn, ERR_OK);
    /*lint +e506*/
  }
  API_EVENT(conn, NETCONN_EVT_SENDPLUS, 0);

  if (was_blocking) {
    sys_sem_signal(&conn->op_completed);
  }
  return ERR_OK;
}
#endif /* LWIP_TCP */

/**
 * Connect a pcb contained inside a netconn
 * Called from netconn_connect.
 *
 * @param msg the api_msg_msg pointing to the connection and containing
 *            the IP_add and port to connect to
 */
void
do_connect(struct api_msg_msg *msg)
{
  if(atomic_read(&msg->conn->is_err_fatal))
  {
    sys_mutex_lock(&msg->conn->conn_mutex);
    msg->err = msg->conn->last_err;
    sys_mutex_unlock(&msg->conn->conn_mutex);
    sys_sem_signal(&msg->conn->op_completed);
    return;
  }

  if (msg->conn->pcb.tcp == NULL) {
    /* This may happen when calling netconn_connect() a second time */
    msg->err = ERR_CLSD;
  } else {
    switch (NETCONNTYPE_GROUP(msg->conn->type)) {
#if LWIP_RAW
      case NETCONN_RAW:
        msg->err = raw_connect(msg->conn->pcb.raw, msg->msg.bc.ipaddr);
        break;
#endif /* LWIP_RAW */
#if LWIP_UDP
      case NETCONN_UDP:
        msg->err = udp_connect(msg->conn->pcb.udp, msg->msg.bc.ipaddr, msg->msg.bc.port);
        break;
#endif /* LWIP_UDP */
#if LWIP_TCP
      case NETCONN_TCP:
        /* Prevent connect while doing any other action. */
        if (msg->conn->state != NETCONN_NONE) {
          msg->err = ERR_ISCONN;
          } else {
            setup_tcp(msg->conn);
            msg->err = tcp_connect(msg->conn->pcb.tcp, msg->msg.bc.ipaddr,
            msg->msg.bc.port, do_connected);
            if (msg->err == ERR_OK) {
              u8_t non_blocking = netconn_is_nonblocking(msg->conn);


              msg->conn->state = NETCONN_CONNECT;
              if (non_blocking == 0) {
                CLEAR_NONBLOCKING_CONNECT(msg->conn);
              } else {
                  SET_NONBLOCKING_CONNECT(msg->conn);
              }
              if (non_blocking) {
                msg->err = ERR_INPROGRESS;
                } else {
                  msg->conn->current_msg = msg;
                  /* sys_sem_signal() is called from do_connected (or err_tcp()),
                  * when the connection is established! */
                  return;
                }
			  }
           }
        break;
#endif /* LWIP_TCP */
    default:
      /*lint -e506*/
      LWIP_ERROR("Invalid netconn type", 0, do{ msg->err = ERR_VAL; }while(0););
      /*lint +e506*/
      break;
    }
  }
  sys_sem_signal(&msg->conn->op_completed);
}


#if LWIP_TCP
/**
 * Set a TCP pcb contained in a netconn into listen mode
 * Called from netconn_listen.
 *
 * @param msg the api_msg_msg pointing to the connection
 */
void
do_listen(struct api_msg_msg *msg)
{
  if (atomic_read(&msg->conn->is_err_fatal)) {
    sys_mutex_lock(&msg->conn->conn_mutex);
    msg->err = msg->conn->last_err;
    sys_mutex_unlock(&msg->conn->conn_mutex);

  } else {
    msg->err = ERR_CONN;
    if (msg->conn->pcb.tcp != NULL) {
      if (msg->conn->type == NETCONN_TCP) {
        if (msg->conn->state == NETCONN_NONE) {
#if TCP_LISTEN_BACKLOG
          struct tcp_pcb* lpcb = tcp_listen_with_backlog(msg->conn->pcb.tcp, msg->msg.lb.backlog);
#else  /* TCP_LISTEN_BACKLOG */
          struct tcp_pcb* lpcb = tcp_listen(msg->conn->pcb.tcp);
#endif /* TCP_LISTEN_BACKLOG */
          if (lpcb == NULL) {
            /* in this case, the old pcb is still allocated */
            msg->err = ERR_MEM;
          } else {
            /* delete the recvmbox and allocate the acceptmbox */
            if (sys_mbox_valid(&msg->conn->recvmbox)) {
              /** @todo: should we drain the recvmbox here? */
              sys_mbox_free(&msg->conn->recvmbox);
              sys_mbox_set_invalid(&msg->conn->recvmbox);
            }
            msg->err = ERR_OK;
            if (!sys_mbox_valid(&msg->conn->acceptmbox)) {
              msg->err = sys_mbox_new(&msg->conn->acceptmbox, DEFAULT_ACCEPTMBOX_SIZE, MBOX_NO_EXPAND);
            }
            if (msg->err == ERR_OK) {
              msg->conn->state = NETCONN_LISTEN;
              msg->conn->pcb.tcp = lpcb;
              tcp_arg(msg->conn->pcb.tcp, msg->conn);
              tcp_accept(msg->conn->pcb.tcp, accept_function);
            } else {
              /* since the old pcb is already deallocated, free lpcb now */
              tcp_close(lpcb);
              msg->conn->pcb.tcp = NULL;
            }
          }
 	   	} else {
 	   	msg->err = ERR_VAL;
        }
      } else {
        msg->err = ERR_ARG;
      }
    }
  }
  TCPIP_APIMSG_ACK(msg);
}
#endif /* LWIP_TCP */

/**
 * Send some data on a RAW or UDP pcb contained in a netconn
 * Called from netconn_send
 *
 * @param msg the api_msg_msg pointing to the connection
 */
void
do_send(struct api_msg_msg *msg)
{
  if (!(atomic_read(&msg->conn->is_err_fatal))) {
    msg->err = ERR_CONN;
    if (msg->conn->pcb.tcp != NULL) {
      switch (NETCONNTYPE_GROUP(msg->conn->type)) {
#if LWIP_RAW
      case NETCONN_RAW:
        if (ip_addr_isany(&msg->msg.b->addr)) {
		   if (!(msg->conn->pcb.raw->flags & RAW_FLAGS_PEER_ADDR_SET)){
		   	msg->err = ERR_NODEST;
			break;
			}
          msg->err = raw_send(msg->conn->pcb.raw, msg->msg.b->p);
        } else {
          msg->err = raw_sendto(msg->conn->pcb.raw, msg->msg.b->p, &msg->msg.b->addr);
        }
        break;
#if PF_PKT_SUPPORT
      case NETCONN_PKT_RAW:
        /* Check if its for sending RAW packets for PF_PACKET family */
        if (msg->msg.b->flags & NETBUF_FLAG_IFINDEX) {
          msg->err = raw_pkt_sendto(msg->conn->pcb.pkt_raw, msg->msg.b->p, msg->msg.b->netifindex);
        } else {
          msg->err = raw_pkt_sendto(msg->conn->pcb.pkt_raw, msg->msg.b->p, 0);
        }
        break;
#endif
#endif
#if LWIP_UDP
      case NETCONN_UDP:
#if LWIP_CHECKSUM_ON_COPY
        if (ip_addr_isany(&msg->msg.b->addr)) {
			if (!(msg->conn->pcb.udp->flags & UDP_FLAGS_PEER_ADDR_SET))
				{
				msg->err = ERR_NODEST;
				break;
				}
          msg->err = udp_send_chksum(msg->conn->pcb.udp, msg->msg.b->p,
            msg->msg.b->flags & NETBUF_FLAG_CHKSUM, msg->msg.b->toport_chksum);
        } else {
          msg->err = udp_sendto_chksum(msg->conn->pcb.udp, msg->msg.b->p,
            &msg->msg.b->addr, msg->msg.b->port,
            msg->msg.b->flags & NETBUF_FLAG_CHKSUM, msg->msg.b->toport_chksum);
        }
#else /* LWIP_CHECKSUM_ON_COPY */
        if (ip_addr_isany(&msg->msg.b->addr)) {
          msg->err = udp_send(msg->conn->pcb.udp, msg->msg.b->p);
        } else {
          msg->err = udp_sendto(msg->conn->pcb.udp, msg->msg.b->p, &msg->msg.b->addr, msg->msg.b->port);
        }
#endif /* LWIP_CHECKSUM_ON_COPY */
        break;
#endif /* LWIP_UDP */
      default:
        break;
      }
    }
  }
  else{
    sys_mutex_lock(&msg->conn->conn_mutex);
    msg->err = msg->conn->last_err;
    sys_mutex_unlock(&msg->conn->conn_mutex);
  }
  TCPIP_SEND_APIMSG_ACK(msg);
}

#if LWIP_TCP
/**
 * Indicate data has been received from a TCP pcb contained in a netconn
 * Called from netconn_recv
 *
 * @param msg the api_msg_msg pointing to the connection
 */
void
do_recv(struct api_msg_msg *msg)
{
  atomic_dec(&msg->conn->recv_count);
  if (!atomic_read(&msg->conn->is_err_fatal)){
    msg->err = ERR_OK;
    if (msg->conn->pcb.tcp != NULL) {
      if (msg->conn->type == NETCONN_TCP) {
#if TCP_LISTEN_BACKLOG
      if (msg->conn->pcb.tcp->state == LISTEN) {
        tcp_accepted(msg->conn->pcb.tcp);
      } else
#endif /* TCP_LISTEN_BACKLOG */
        {
          u32_t remaining = msg->msg.r.len;
          do {
            u16_t recved = (u16_t)((remaining > 0xffff) ? 0xffff : (u16_t)remaining);
            tcp_recved(msg->conn->pcb.tcp, recved);
            remaining -= recved;
          }while(remaining != 0);
        }
      }
    }
  }
  else{
    sys_mutex_lock(&msg->conn->conn_mutex);
    msg->err = msg->conn->last_err;
    sys_mutex_unlock(&msg->conn->conn_mutex);
  }
  TCPIP_RECV_APIMSG_ACK(msg);
}

/**
 * See if more data needs to be written from a previous call to netconn_write.
 * Called initially from do_write. If the first call can't send all data
 * (because of low memory or empty send-buffer), this function is called again
 * from sent_tcp() or poll_tcp() to send more data. If all data is sent, the
 * blocking application thread (waiting in netconn_write) is released.
 *
 * @param conn netconn (that is currently in state NETCONN_WRITE) to process
 * @return ERR_OK
 *         ERR_MEM if LWIP_TCPIP_CORE_LOCKING=1 and sending hasn't yet finished
 */
LWIP_STATIC err_t
do_writemore(struct netconn *conn)
{
  err_t err;
  void *dataptr;
  /* Adding for WINDOW SCALE */
  tcpwnd_size_t len, available;
  u8_t write_finished = 0;
  size_t diff;
  u8_t dontblock;
  u8_t dontblock_sock;
  u8_t apiflags;

  LWIP_ASSERT("conn != NULL", conn != NULL);
  LWIP_ASSERT("conn->state == NETCONN_WRITE", (conn->state == NETCONN_WRITE));
  LWIP_ASSERT("conn->current_msg != NULL", conn->current_msg != NULL);
  LWIP_ASSERT("conn->pcb.tcp != NULL", conn->pcb.tcp != NULL);
  LWIP_ASSERT("conn->write_offset < conn->current_msg->msg.w.len",
    conn->write_offset < conn->current_msg->msg.w.len);

  if(atomic_read(&conn->is_err_fatal))
  {
    NETCONN_GET_SAFE_ERR(conn, err);
    sys_sem_signal(&conn->send_completed);
    return err;
  }

  dontblock_sock = netconn_is_nonblocking(conn);
  apiflags = conn->current_msg->msg.w.apiflags;

  dontblock = dontblock_sock || (apiflags & NETCONN_DONTBLOCK);

/* NETIF DRIVER STATUS BEGIN */
#if DRIVER_STATUS_CHECK
  if(dontblock && (conn->pcb.tcp->drv_status == DRV_NOT_READY)) {
    if (dontblock_sock) {
      LWIP_DEBUGF(DRV_STS_DEBUG, ("Driver Not Ready. So sending SENDMINUS event\n"));
      API_EVENT(conn, NETCONN_EVT_SENDMINUS, 0);
    }

    LWIP_DEBUGF(DRV_STS_DEBUG, ("Driver Not Ready. So returning ERR_WOULDBLOCK\n"));

    err = ERR_WOULDBLOCK;

    sys_mutex_lock(&conn->conn_mutex);
    conn->current_msg->err = err;
    conn->current_msg = NULL;
    sys_mutex_unlock(&conn->conn_mutex);

    conn->state = NETCONN_NONE;

    sys_sem_signal(&conn->send_completed);
    return err;
  }
#endif
/* NETIF DRIVER STATUS END */

#if LWIP_SO_SNDTIMEO
  if ((conn->send_timeout != 0) &&
      ((s32_t)(sys_now() - conn->current_msg->msg.w.time_started) >= conn->send_timeout)) {
    write_finished = 1;
    if (conn->write_offset == 0) {
      /* nothing has been written */
      err = ERR_WOULDBLOCK;
      conn->current_msg->msg.w.len = 0;
    } else {
      /* partial write */
      err = ERR_OK;
      conn->current_msg->msg.w.len = conn->write_offset;
    }
  } else
#endif /* LWIP_SO_SNDTIMEO */
  {
    dataptr = (u8_t*)conn->current_msg->msg.w.dataptr + conn->write_offset;
    diff = conn->current_msg->msg.w.len - conn->write_offset;
    if (diff > 0xffffUL) { /* max_u16_t */
      len = 0xffff;
#if LWIP_TCPIP_CORE_LOCKING
      conn->flags |= NETCONN_FLAG_WRITE_DELAYED;
#endif
      apiflags |= TCP_WRITE_FLAG_MORE;
    } else {
      len = (u16_t)diff;
    }
    available = tcp_sndbuf(conn->pcb.tcp);
    if (available < len) {
      /* don't try to write more than sendbuf */
      len = available;
      if (dontblock){
        if (!len) {
          err = ERR_WOULDBLOCK;
          goto err_mem;
        }
      } else {
#if LWIP_TCPIP_CORE_LOCKING
        conn->flags |= NETCONN_FLAG_WRITE_DELAYED;
#endif
        apiflags |= TCP_WRITE_FLAG_MORE;
      }
    }
    LWIP_ASSERT("do_writemore: invalid length!", ((conn->write_offset + len) <= conn->current_msg->msg.w.len));
    err = tcp_write(conn->pcb.tcp, dataptr, len, apiflags);
    /* if OK or memory error, check available space */
    if ((err == ERR_OK) || (err == ERR_MEM)) {
err_mem:
      if (dontblock && (len < conn->current_msg->msg.w.len)) {
        /* non-blocking write did not write everything: mark the pcb non-writable
           and let poll_tcp check writable space to mark the pcb writable again */
        /* pclint fix for window scaling changes, currently event_callback() is not */
        /* using len so truncating to u16_t is not a problem */
        API_EVENT(conn, NETCONN_EVT_SENDMINUS, (u16_t)len);
        conn->flags |= NETCONN_FLAG_CHECK_WRITESPACE;
      } else if ((tcp_sndbuf(conn->pcb.tcp) <= CFG_TCP_SNDLOWAT) ||
                 (tcp_sndqueuelen(conn->pcb.tcp) >= CFG_TCP_SNDQUEUELOWAT)) {
        /* The queued byte- or pbuf-count exceeds the configured low-water limit,
           let select mark this pcb as non-writable. */
        API_EVENT(conn, NETCONN_EVT_SENDMINUS,(u16_t) len);
      }
    }

    if (err == ERR_OK) {
      conn->write_offset += len;
      if ((conn->write_offset == conn->current_msg->msg.w.len) || dontblock) {
        /* return sent length */
        conn->current_msg->msg.w.len = conn->write_offset;
        /* everything was written */
        write_finished = 1;
        conn->write_offset = 0;
      }
      tcp_output(conn->pcb.tcp);
    } else if ((err == ERR_MEM) && !dontblock) {
      /* If ERR_MEM, we wait for sent_tcp or poll_tcp to be called
         we do NOT return to the application thread, since ERR_MEM is
         only a temporary error! */

      /* tcp_write returned ERR_MEM, try tcp_output anyway */
      tcp_output(conn->pcb.tcp);

#if LWIP_TCPIP_CORE_LOCKING
      conn->flags |= NETCONN_FLAG_WRITE_DELAYED;
#endif
    } else {
      /* On errors != ERR_MEM, we don't try writing any more but return
         the error to the application thread. */
      write_finished = 1;
      conn->current_msg->msg.w.len = 0;
    }
  }
  if (write_finished) {
    /* everything was written: set back connection state
       and back to application task */
    conn->current_msg->err = err;
    conn->current_msg = NULL;
    conn->state = NETCONN_NONE;
#if LWIP_TCPIP_CORE_LOCKING
    if ((conn->flags & NETCONN_FLAG_WRITE_DELAYED) != 0)
#endif
    {
      sys_sem_signal(&conn->send_completed);
    }
  }
#if LWIP_TCPIP_CORE_LOCKING
  else
    return ERR_MEM;
#endif
  return ERR_OK;
}
#endif /* LWIP_TCP */

/**
 * Send some data on a TCP pcb contained in a netconn
 * Called from netconn_write
 *
 * @param msg the api_msg_msg pointing to the connection
 */
void
do_write(struct api_msg_msg *msg)
{
  if (!atomic_read(&msg->conn->is_err_fatal)) {
    if (msg->conn->type == NETCONN_TCP) {
#if LWIP_TCP
      if (msg->conn->state != NETCONN_NONE) {
        /* netconn is connecting, closing or in blocking write */
        msg->err = ERR_INPROGRESS;

      } else if (msg->conn->pcb.tcp != NULL) {
        msg->conn->state = NETCONN_WRITE;
        /* set all the variables used by do_writemore */
        LWIP_ASSERT("already writing or closing", msg->conn->current_msg == NULL &&
          msg->conn->write_offset == 0);
        LWIP_ASSERT("msg->msg.w.len != 0", msg->msg.w.len != 0);
        msg->conn->current_msg = msg;
        msg->conn->write_offset = 0;
#if LWIP_TCPIP_CORE_LOCKING
        msg->conn->flags &= ~NETCONN_FLAG_WRITE_DELAYED;
        if (do_writemore(msg->conn) != ERR_OK) {
          LWIP_ASSERT("state!", msg->conn->state == NETCONN_WRITE);
          UNLOCK_TCPIP_CORE();
          sys_arch_sem_wait(&msg->conn->send_completed, 0);
          LOCK_TCPIP_CORE();
          LWIP_ASSERT("state!", msg->conn->state == NETCONN_NONE);
        }
#else /* LWIP_TCPIP_CORE_LOCKING */
        do_writemore(msg->conn);
#endif /* LWIP_TCPIP_CORE_LOCKING */
        /* for both cases: if do_writemore was called, don't ACK the APIMSG
           since do_writemore ACKs it! */
        return;
      } else {
        msg->err = ERR_CONN;
      }
#else /* LWIP_TCP */
      msg->err = ERR_VAL;
#endif /* LWIP_TCP */
#if (LWIP_UDP || LWIP_RAW)
    } else {
      msg->err = ERR_VAL;
#endif /* (LWIP_UDP || LWIP_RAW) */
    }
  }
  else{
    sys_mutex_lock(&msg->conn->conn_mutex);
    msg->err = msg->conn->last_err;
    sys_mutex_unlock(&msg->conn->conn_mutex);
  }

  TCPIP_SEND_APIMSG_ACK(msg);
}



void do_getconninfo (struct api_msg_msg *msg)
{
  struct tcp_pcb *tcp = NULL;
  struct udp_pcb *udp = NULL;
  ip_addr_t *dst_addr;
  ip_addr_t *src_addr;

  ip_addr_t *ip_add;
  struct tcpip_conn  *conn_info;
  struct eth_addr *tdst_mac;
  struct eth_addr invalid_mac = {{0,0,0,0,0,0}};
  s8_t ret;


  conn_info = msg->msg.conn_info;
  tdst_mac= &conn_info->dst_mac;
  if (msg->conn->type == NETCONN_UDP){
    udp = msg->conn->pcb.udp;
    if (NULL == udp)
    {
      msg->err = ERR_CONN;
      goto RETURN;
    }

    ip_addr_copy(conn_info->dst_ip, udp->remote_ip);
    ip_addr_copy(conn_info->src_ip, udp->local_ip);
    conn_info->srcport = udp->local_port;
    conn_info->dstport = udp->remote_port;
    conn_info->seqnum  = 0; /* Last sent Sequence number */
    conn_info->acknum  = 0;       /* Last  acknowledged number sent  */
    conn_info->tcpwin  = 0;
    conn_info->last_payload_len = udp->last_payload_len;
    conn_info->tsval   = 0 ;
    conn_info->tsecr = 0;
    conn_info->ipid = get_ipid();
    dst_addr = &msg->conn->pcb.udp->remote_ip;
    src_addr = &msg->conn->pcb.udp->local_ip;

  }
  else if (msg->conn->type == NETCONN_TCP) {
    tcp = msg->conn->pcb.tcp;
    if(tcp == NULL) {
      msg->err = ERR_CONN;
      goto RETURN;
    }
    ip_addr_copy(conn_info->dst_ip, tcp->remote_ip);
    ip_addr_copy(conn_info->src_ip, tcp->local_ip);
    conn_info->srcport = tcp->local_port;
    conn_info->dstport = tcp->remote_port;
    if(tcp->state == SYN_SENT ) {
      conn_info->seqnum  = tcp->lastack; /*  seqnum of last ACKED byte */
    }else{
      conn_info->seqnum  = (u32_t)(tcp->lastack-1);
    }
    conn_info->acknum  = tcp->rcv_nxt;       /* Last  acknowledged number sent  */
    conn_info->tcpwin  = tcp->snd_wnd;
    conn_info->last_payload_len = tcp->last_payload_len;
    conn_info->tsval   = 0 ;
    conn_info->tsecr = 0;
    conn_info->ipid = get_ipid();
    dst_addr = &msg->conn->pcb.tcp->remote_ip;
    src_addr = &msg->conn->pcb.tcp->local_ip;
  }else{
    msg->err = ERR_VAL;
    goto RETURN;
  }

  ret  = netif_find_dst_ipaddr(src_addr,&dst_addr);

  if(ret == 0) {
     ret = etharp_find_addr(NULL,dst_addr,&tdst_mac,&ip_add);
     if(ret == -1) {
        tdst_mac = &invalid_mac;
     }
  }
  else {
    tdst_mac = &invalid_mac;
  }
  MEMCPY_S(&conn_info->dst_mac, sizeof(conn_info->dst_mac),tdst_mac , sizeof(conn_info->dst_mac));
  msg->err = ERR_OK;

RETURN:
  TCPIP_APIMSG_ACK(msg);
  return;

}


/**
 * Return a connection's local or remote address
 * Called from netconn_getaddr
 *
 * @param msg the api_msg_msg pointing to the connection
 */
void
do_getaddr(struct api_msg_msg *msg)
{
  if (msg->conn->pcb.ip != NULL) {
    *(msg->msg.ad.ipaddr) = (msg->msg.ad.local ? msg->conn->pcb.ip->local_ip :
                             msg->conn->pcb.ip->remote_ip);

    msg->err = ERR_OK;
    switch (NETCONNTYPE_GROUP(msg->conn->type)) {
#if LWIP_RAW
    case NETCONN_RAW:
      if (msg->msg.ad.local) {
#if PF_PKT_SUPPORT
        *(msg->msg.ad.port) = msg->conn->pcb.raw->proto.protocol;
#else
        *(msg->msg.ad.port) = msg->conn->pcb.raw->protocol;
#endif
      } else {
        /* return an error as connecting is only a helper for upper layers */
        msg->err = ERR_OPNOTSUPP;
      }
      break;
#endif /* LWIP_RAW */
#if LWIP_UDP
    case NETCONN_UDP:
      if (msg->msg.ad.local) {
        *(msg->msg.ad.port) = msg->conn->pcb.udp->local_port;
      } else {
        if ((msg->conn->pcb.udp->flags & UDP_FLAGS_CONNECTED) == 0) {
          msg->err = ERR_CONN;
        } else {
          *(msg->msg.ad.port) = msg->conn->pcb.udp->remote_port;
        }
      }
      break;
#endif /* LWIP_UDP */
#if LWIP_TCP
    case NETCONN_TCP:
      *(msg->msg.ad.port) = (u16_t)(msg->msg.ad.local?msg->conn->pcb.tcp->local_port:msg->conn->pcb.tcp->remote_port);
      break;
#endif /* LWIP_TCP */
    default:
      msg->err = ERR_OPNOTSUPP;
      LWIP_ASSERT("invalid netconn_type", 0);
      break;
    }
  } else {
    msg->err = ERR_CONN;
  }
  TCPIP_APIMSG_ACK(msg);
}

/**
 * Close a TCP pcb contained in a netconn
 * Called from netconn_close_shutdown
 *
 * @param msg the api_msg_msg pointing to the connection
 */
void
do_close(struct api_msg_msg *msg)
{
#if LWIP_TCP
  /* @todo: abort running write/connect? */
  if ((msg->conn->state != NETCONN_NONE) && (msg->conn->state != NETCONN_LISTEN)) {
    /* this only happens for TCP netconns */
    LWIP_ASSERT("msg->conn->type == NETCONN_TCP", msg->conn->type == NETCONN_TCP);
    msg->err = ERR_INPROGRESS;
  } else if ((msg->conn->pcb.tcp != NULL) && (msg->conn->type == NETCONN_TCP)) {
    if ((msg->msg.sd.shut != NETCONN_SHUT_RDWR) && (msg->conn->state == NETCONN_LISTEN)) {
      /* LISTEN doesn't support half shutdown */
      msg->err = ERR_CONN;
    } else {
      if (msg->msg.sd.shut & NETCONN_SHUT_RD) {
        /* Drain and delete mboxes */
        netconn_drain(msg->conn);
      }
      LWIP_ASSERT("already writing or closing", msg->conn->current_msg == NULL &&
        msg->conn->write_offset == 0);
       msg->conn->state = NETCONN_CLOSE;
      sys_mutex_lock(&msg->conn->conn_mutex);
      msg->conn->current_msg = msg;
      sys_mutex_unlock(&msg->conn->conn_mutex);
      do_close_internal(msg->conn);
      /* for tcp netconns, do_close_internal ACKs the message */
      return;
    }
  } else
#endif /* LWIP_TCP */
  {
    msg->err = ERR_VAL;
  }
  TCPIP_CLOSE_APIMSG_ACK(msg);
}


#if LWIP_DNS
/**
 * Callback function that is called when DNS name is resolved
 * (or on timeout). A waiting application thread is waked up by
 * signaling the semaphore.
 */
LWIP_STATIC void
do_dns_found(const char *name, ip_addr_t *ipaddr, u32_t count, void *arg)
{
  struct dns_api_msg *msg = (struct dns_api_msg*)arg;
  u32_t i;
  LWIP_ASSERT("DNS response for wrong host name", strcmp(msg->name, name) == 0);
  LWIP_UNUSED_ARG(name);

  if (ipaddr == NULL) {
    /* timeout or memory error */
    *msg->err = ERR_VAL;
  } else {
    /* address was resolved */
    *msg->err = ERR_OK;
    for(i = 0; i < count; i++) {
      msg->addr[i] = ipaddr[i];
    }
    *msg->count = count;
  }
  /* wake up the application task waiting in netconn_gethostbyname */
  sys_sem_signal(msg->sem);
}

/**
 * Execute a DNS query
 * Called from netconn_gethostbyname
 *
 * @param arg the dns_api_msg pointing to the query
 */
void
do_gethostbyname(void *arg)
{
  struct dns_api_msg *msg = (struct dns_api_msg*)arg;

  *msg->err = dns_gethostbyname(msg->name, msg->addr, msg->count,do_dns_found, msg);
  if (*msg->err != ERR_INPROGRESS) {
    /* on error or immediate success, wake up the application
     * task waiting in netconn_gethostbyname */
    sys_sem_signal(msg->sem);
  }
}
#endif /* LWIP_DNS */

#endif /* LWIP_NETCONN */
