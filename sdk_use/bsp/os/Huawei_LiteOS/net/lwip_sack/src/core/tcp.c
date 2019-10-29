/**
 * @file
 * Transmission Control Protocol for IP
 *
 * This file contains common functions for the TCP implementation, such as functinos
 * for manipulating the data structures and the TCP timer functions. TCP functions
 * related to input and output is found in tcp_in.c and tcp_out.c respectively.
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

#if LWIP_TCP /* don't build if not configured for use in lwipopts.h */

#include "lwip/def.h"
#include "lwip/mem.h"
#include "lwip/memp.h"
#include "lwip/snmp.h"
#include "lwip/tcp_int.h"
#include "lwip/tcp_impl_int.h"
#include "lwip/stats.h"
#include "lwip/tcp_sack.h"
#include "lwip/tcp_config.h"

#include <string.h>

#ifndef TCP_LOCAL_PORT_RANGE_START
/* From https://www.iana.org/assignments/port-numbers:
   "The Dynamic and/or Private Ports are those from 49152 through 65535" */
#define TCP_LOCAL_PORT_RANGE_START        0xc000
#define TCP_LOCAL_PORT_RANGE_END          0xffff
#endif

#if LWIP_TCP_KEEPALIVE
#define TCP_KEEP_DUR(pcb)   ((pcb)->keep_cnt * (pcb)->keep_intvl)
#define TCP_KEEP_INTVL(pcb) ((pcb)->keep_intvl)
#else /* LWIP_TCP_KEEPALIVE */
#define TCP_KEEP_DUR(pcb)   TCP_MAXIDLE
#define TCP_KEEP_INTVL(pcb) TCP_KEEPINTVL_DEFAULT
#endif /* LWIP_TCP_KEEPALIVE */

const char * const tcp_state_str[] = {
  "CLOSED",
  "LISTEN",
  "SYN_SENT",
  "SYN_RCVD",
  "ESTABLISHED",
  "FIN_WAIT_1",
  "FIN_WAIT_2",
  "CLOSE_WAIT",
  "CLOSING",
  "LAST_ACK",
  "TIME_WAIT"
};

/* Incremented every coarse grained timer shot (typically every 500 ms). */
u32_t tcp_ticks;
u8_t tcp_backoff[256];
 /* Times per slowtmr hits */
const u8_t tcp_persist_backoff[7] = { 3, 6, 12, 24, 48, 96, 120 };

/* The TCP PCB lists. */

/** List of all TCP PCBs bound but not yet (connected || listening) */
struct tcp_pcb *tcp_bound_pcbs;
/** List of all TCP PCBs in LISTEN state */
union tcp_listen_pcbs_t tcp_listen_pcbs;
/** List of all TCP PCBs that are in a state in which
 * they accept or send data. */
struct tcp_pcb *tcp_active_pcbs;
/** List of all TCP PCBs in TIME-WAIT state */
struct tcp_pcb *tcp_tw_pcbs;

#define NUM_TCP_PCB_LISTS               4

#if SO_REUSE
#define NUM_TCP_PCB_LISTS_NO_TIME_WAIT  3
#endif /* SO_REUSE */

/** An array with all (non-temporary) PCB lists, mainly used for smaller code size */
struct tcp_pcb ** const tcp_pcb_lists[] = {&tcp_listen_pcbs.pcbs, &tcp_bound_pcbs,
  &tcp_active_pcbs, &tcp_tw_pcbs};

/** Only used for temporary storage. */
struct tcp_pcb *tcp_tmp_pcb;

u8_t tcp_active_pcbs_changed;

/** Timer counter to handle calling slow-timer from tcp_tmr() */
static u8_t tcp_timer = 0;
static u8_t tcp_timer_ctr = 0;

/** used to update TCP stack configurations by user */
struct lwip_tcp_cfg_param_ glwip_tcp_cfg_param =
                                                  {
                                                     TCP_WND,
                                                     TCP_SNDLOWAT,
                                                     TCP_SNDQUEUELOWAT,
                                                     TCP_SND_BUF,
                                                     INITIAL_CWND,
                                                     INITIAL_SSTHRESH,
                                                     TCP_SND_QUEUELEN,
                                                     TCP_MSS,
                                                     TCP_OVERSIZE,
#if LWIP_WND_SCALE
                                                     TCP_WND_MIN,
                                                     TCP_RCV_SCALE,
#endif
                                                     TCP_TTL,
                                                     TCP_MAXRTX,
                                                     TCP_SYNMAXRTX,
                                                     TCP_FW1MAXRTX,
                                                     0,
                                                 };

LWIP_STATIC u16_t tcp_new_port(void);

#if LWIP_SACK
void tcp_connect_update_sack(struct tcp_pcb *pcb, u32_t iss);
#endif

#if LWIP_WND_SCALE
void tcp_init_ws(struct tcp_pcb *pcb);
#endif



#if LWIP_TCP


/*
  Note value of this config must not differ TCP_TTL  used in opt.h or lwipopts.h
  Interface is to just update configuration parameters in tcplibs from opt.h or lwipopts.h
  If not called, default config parameters used from lwipopts.h provided in
*/
int lwip_config_tcp_ttl(u8_t ttl)
{
  if (ttl < TCP_TTL_MIN_VALUE)
    return ERR_ARG;

  if(g_IslwIPInitialized == lwIP_TRUE) {
    return ERR_ARG;
  }

  glwip_tcp_cfg_param.tcp_ttl = ttl;
  return ERR_OK;
}

/*
  Note value of this config must not differ TCP_MAXRTX  used in opt.h or lwipopts.h
*/

int lwip_config_tcp_maxrtx(u8_t maxrtx)
{
  if (maxrtx < TCP_MAXRTX_MIN_VALUE)
    return ERR_ARG;

  if(g_IslwIPInitialized == lwIP_TRUE) {
    return ERR_ARG;
  }
  glwip_tcp_cfg_param.tcp_maxrtx = maxrtx;
  return ERR_OK;
}
/*
  Note value of this config must not differ TCP_SYNMAXRTX  used in opt.h or lwipopts.h
*/
int lwip_config_tcp_synmaxrtx(u8_t maxrtx)
{

  if(g_IslwIPInitialized == lwIP_TRUE) {
    return ERR_ARG;
  }

  if(maxrtx > 12 || maxrtx < 4)
  {
    return ERR_ARG;
  }
  glwip_tcp_cfg_param.tcp_synmaxrtx = maxrtx;
  return ERR_OK;
}

/*
  Note value of this config must not differ TCP_FW1MAXRTX  used in opt.h or lwipopts.h
*/

int lwip_config_tcp_fw1maxrtx(u8_t maxrtx)
{
  if(g_IslwIPInitialized == lwIP_TRUE) {
    return ERR_ARG;
  }
  glwip_tcp_cfg_param.tcp_fw1maxrtx = maxrtx;
  return ERR_OK;
}

#if LWIP_WND_SCALE


/*
  Note value of this config must not differ TCP_RCV_SCALE  used in opt.h or lwipopts.h
*/


int lwip_config_tcp_rcv_scale(u8_t rcv_scale)
{

  if(g_IslwIPInitialized == lwIP_TRUE) {
    return ERR_ARG;
  }

  if(rcv_scale > 14)
  {
    return ERR_ARG;
  }

  if (CFG_TCP_WND > (0xFFFFU << rcv_scale)) {
    return ERR_ARG;
  }

  if (((CFG_TCP_WND >> rcv_scale) == 0)) {
    return ERR_ARG;
  }

  glwip_tcp_cfg_param.tcp_rcv_scale = rcv_scale;
  return ERR_OK;
}

/*
  Note value of this config must not differ TCP_WND_MIN  used in opt.h or lwipopts.h
*/
int lwip_config_tcp_wnd_min(u16_t wnd)
{

  if(g_IslwIPInitialized == lwIP_TRUE) {
    return ERR_ARG;
  }

  if (wnd < CFG_TCP_MSS)
    return ERR_ARG;

  glwip_tcp_cfg_param.tcp_wnd_min = wnd;
  return ERR_OK;
}
#endif

/*
  Note value of this config must not differ TCP_WND  used in opt.h or lwipopts.h
*/
int lwip_config_tcp_wnd(u32_t wnd)
{
  if(g_IslwIPInitialized == lwIP_TRUE) {
    return ERR_ARG;
  }

  glwip_tcp_cfg_param.tcp_wnd = wnd;
  return ERR_OK;
}

/*
  Note value of this config must not differ TCP_MSS  used in opt.h or lwipopts.h
*/

int lwip_config_tcp_mss(u16_t mss)
{
  if(g_IslwIPInitialized == lwIP_TRUE) {
    return ERR_ARG;
  }

  glwip_tcp_cfg_param.tcp_mss = mss;
  return ERR_OK;
}

/*
  Note value of this config must not differ TCP_SND_BUF  used in opt.h or lwipopts.h
*/
int lwip_config_tcp_snd_buf(tcpwnd_size_t snd_buf)
{
  if(g_IslwIPInitialized == lwIP_TRUE) {
    return ERR_ARG;
  }

  glwip_tcp_cfg_param.tcp_snd_buf = snd_buf;
  return ERR_OK;
}


/*
  Note value of this config must not differ INITIAL_CWND  used in opt.h or lwipopts.h
*/
int lwip_config_initial_cwd(tcpwnd_size_t cwnd)
{
  if(g_IslwIPInitialized == lwIP_TRUE) {
    return ERR_ARG;
  }

  if (cwnd < 1 || cwnd > 200) {
    printf("lwip_config_initial_cwd: If you want to use TCP, INITIAL_CWND must greater than 4 and less than 200\n");
    return ERR_ARG;
  }

  glwip_tcp_cfg_param.initial_cwnd = cwnd;
  return ERR_OK;
}


/*
  Note value of this config must not differ INITIAL_SSTHRESH  used in opt.h or lwipopts.h
*/
int lwip_config_initial_ssthresh(tcpwnd_size_t ssthresh)
{
  if(g_IslwIPInitialized == lwIP_TRUE) {
    return ERR_ARG;
  }

  if(ssthresh < 20 || ssthresh > 2000) {
    printf("lwip_config_initial_ssthresh: If you want to use TCP, INITIAL_SSTHRESH must greater than 20 and less than 2000\n");
    return ERR_ARG;
  }
  glwip_tcp_cfg_param.inital_ssthresh = ssthresh;
  return ERR_OK;
}


#endif /* LWIP_TCP */




/**
 * Initialize this module.
 */
void
tcp_init(void)
{
  int i;

  for (i = 0; i < 255; i++) {
    /*lint -e734*/
    /* suppressing loss of preceision as RHS can never be more than 255 */
    tcp_backoff[i] = (u8_t)(1 + i);
    /*lint +e734*/
  }
  tcp_backoff[255] = 255; /* should not be 255 + 1 as tcp_backoff is of type u8_t */
}

/**
 * Called periodically to dispatch TCP timers.
 */
void
tcp_tmr(void)
{
  /* Call tcp_fasttmr() every 100 ms */
  tcp_fasttmr();

  if (++tcp_timer == TCP_SLOW_INTERVAL_PERIOD) {
    /* Call tcp_tmr() every 500 ms, i.e., every other timer
       tcp_tmr() is called. */
    tcp_slowtmr();
    tcp_timer = 0;
  }
}

/**
 * Closes the TX side of a connection held by the PCB.
 * For tcp_close(), a RST is sent if the application didn't receive all data
 * (tcp_recved() not called for all data passed to recv callback).
 *
 * Listening pcbs are freed and may not be referenced any more.
 * Connection pcbs are freed if not yet connected and may not be referenced
 * any more. If a connection is established (at least SYN received or in
 * a closing state), the connection is closed, and put in a closing state.
 * The pcb is then automatically freed in tcp_slowtmr(). It is therefore
 * unsafe to reference it.
 *
 * @param pcb the tcp_pcb to close
 * @return ERR_OK if connection has been closed
 *         another err_t if closing failed and pcb is not freed
 */
LWIP_STATIC err_t
tcp_close_shutdown(struct tcp_pcb *pcb, u8_t rst_on_unacked_data)
{
  err_t err;

  if (rst_on_unacked_data && ((pcb->state == ESTABLISHED) || (pcb->state == CLOSE_WAIT))) {
    if ((pcb->refused_data != NULL) || (pcb->rcv_wnd != pcb->tcpwnd)) {
      /* Not all data received by application, send RST to tell the remote
         side about this. */
      LWIP_ASSERT("pcb->flags & TF_RXCLOSED", pcb->flags & TF_RXCLOSED);

      /* don't call tcp_abort here: we must not deallocate the pcb since
         that might not be expected when calling tcp_close */
/* NETIF DRIVER STATUS BEGIN */
#if DRIVER_STATUS_CHECK
      if (pcb->drv_status == DRV_NOT_READY) {
        pcb->flags |= TF_RST_ON_DRV_WAKE;
        LWIP_DEBUGF(DRV_STS_DEBUG, ("Driver Not Ready. Delaying reset \n"));
      } else {
        tcp_rst(pcb->snd_nxt, pcb->rcv_nxt, &pcb->local_ip, &pcb->remote_ip,
          pcb->local_port, pcb->remote_port);
      }
#else
      tcp_rst(pcb->snd_nxt, pcb->rcv_nxt, &pcb->local_ip, &pcb->remote_ip,
        pcb->local_port, pcb->remote_port);
#endif
/* NETIF DRIVER STATUS END */

      tcp_pcb_purge(pcb);
      TCP_RMV_ACTIVE(pcb);
/* NETIF DRIVER STATUS BEGIN */
#if DRIVER_STATUS_CHECK
      if ((pcb->state == ESTABLISHED) || (pcb->flags & TF_RST_ON_DRV_WAKE)) {
#else
      if (pcb->state == ESTABLISHED) {
#endif
/* NETIF DRIVER STATUS END */
        /* move to TIME_WAIT since we close actively */
        pcb->state = TIME_WAIT;
        TCP_REG(&tcp_tw_pcbs, pcb);
      } else {
        /* CLOSE_WAIT: deallocate the pcb since we already sent a RST for it */
        memp_free(MEMP_TCP_PCB, pcb);
      }
      return ERR_OK;
    }
  }

  switch (pcb->state) {
  case CLOSED:
    /* Closing a pcb in the CLOSED state might seem erroneous,
     * however, it is in this state once allocated and as yet unused
     * and the user needs some way to free it should the need arise.
     * Calling tcp_close() with a pcb that has already been closed, (i.e. twice)
     * or for a pcb that has been used and then entered the CLOSED state
     * is erroneous, but this should never happen as the pcb has in those cases
     * been freed, and so any remaining handles are bogus. */
    err = ERR_OK;
    if (pcb->local_port != 0) {
      TCP_RMV(&tcp_bound_pcbs, pcb);
    }
    memp_free(MEMP_TCP_PCB, pcb);
    pcb = NULL;
    break;
  case LISTEN:
    err = ERR_OK;
    tcp_pcb_remove(&tcp_listen_pcbs.pcbs, pcb);
    memp_free(MEMP_TCP_PCB_LISTEN, pcb);
    pcb = NULL;
    break;
  case SYN_SENT:
    err = ERR_OK;
    TCP_PCB_REMOVE_ACTIVE(pcb);
    memp_free(MEMP_TCP_PCB, pcb);
    pcb = NULL;
    snmp_inc_tcpattemptfails();
    break;
  case SYN_RCVD:
    err = tcp_send_fin(pcb);
    if (err == ERR_OK) {
      snmp_inc_tcpattemptfails();
      pcb->state = FIN_WAIT_1;
    }
    break;
  case ESTABLISHED:
    err = tcp_send_fin(pcb);
    if (err == ERR_OK) {
      snmp_inc_tcpestabresets();
      pcb->state = FIN_WAIT_1;
    }
    break;
  case CLOSE_WAIT:
    err = tcp_send_fin(pcb);
    if (err == ERR_OK) {
      snmp_inc_tcpestabresets();
      pcb->state = LAST_ACK;
    }
    break;
  default:
    /* Has already been closed, do nothing. */
    err = ERR_OK;
    pcb = NULL;
    break;
  }

  if (pcb != NULL && err == ERR_OK) {
    /* To ensure all data has been sent when tcp_close returns, we have
       to make sure tcp_output doesn't fail.
       Since we don't really have to ensure all data has been sent when tcp_close
       returns (unsent data is sent from tcp timer functions, also), we don't care
       for the return value of tcp_output for now. */
    /* @todo: When implementing SO_LINGER, this must be changed somehow:
       If SOF_LINGER is set, the data should be sent and acked before close returns.
       This can only be valid for sequential APIs, not for the raw API. */
    tcp_output(pcb);
  }
  return err;
}

/**
 * Closes the connection held by the PCB.
 *
 * Listening pcbs are freed and may not be referenced any more.
 * Connection pcbs are freed if not yet connected and may not be referenced
 * any more. If a connection is established (at least SYN received or in
 * a closing state), the connection is closed, and put in a closing state.
 * The pcb is then automatically freed in tcp_slowtmr(). It is therefore
 * unsafe to reference it (unless an error is returned).
 *
 * @param pcb the tcp_pcb to close
 * @return ERR_OK if connection has been closed
 *         another err_t if closing failed and pcb is not freed
 */
err_t
tcp_close(struct tcp_pcb *pcb)
{
#if TCP_DEBUG
  LWIP_DEBUGF(TCP_DEBUG, ("tcp_close: closing in "));
  tcp_debug_print_state(pcb->state);
#endif /* TCP_DEBUG */

  if (pcb->state != LISTEN) {
    /* Set a flag not to receive any more data... */
    pcb->flags |= TF_RXCLOSED;
  }
  /* ... and close */
  return tcp_close_shutdown(pcb, 1);
}

/**
 * Causes all or part of a full-duplex connection of this PCB to be shut down.
 * This doesn't deallocate the PCB unless shutting down both sides!
 * Shutting down both sides is the same as calling tcp_close, so if it succeds,
 * the PCB should not be referenced any more.
 *
 * @param pcb PCB to shutdown
 * @param shut_rx shut down receive side if this is != 0
 * @param shut_tx shut down send side if this is != 0
 * @return ERR_OK if shutdown succeeded (or the PCB has already been shut down)
 *         another err_t on error.
 */
err_t
tcp_shutdown(struct tcp_pcb *pcb, int shut_rx, int shut_tx)
{
  if (pcb->state == LISTEN) {
    return ERR_CONN;
  }
  if (shut_rx) {
    /* shut down the receive side: set a flag not to receive any more data... */
    pcb->flags |= TF_RXCLOSED;
    if (shut_tx) {
      /* shutting down the tx AND rx side is the same as closing for the raw API */
      return tcp_close_shutdown(pcb, 1);
    }
    /* ... and free buffered data */
    if (pcb->refused_data != NULL) {
      pbuf_free(pcb->refused_data);
      pcb->refused_data = NULL;
    }
  }
  if (shut_tx) {
    /* This can't happen twice since if it succeeds, the pcb's state is changed.
       Only close in these states as the others directly deallocate the PCB */
    switch (pcb->state) {
    case SYN_RCVD:
    case ESTABLISHED:
    case CLOSE_WAIT:
      /* PC Lint FIx for: Loss of precision (arg. no. 2) (31 bits to 8 bits) */
      return tcp_close_shutdown(pcb, (u8_t)shut_rx);
    default:
      /* Not (yet?) connected, cannot shutdown the TX side as that would bring us
        into CLOSED state, where the PCB is deallocated. */
      return ERR_CONN;
    }
  }
  return ERR_OK;
}

/**
 * Abandons a connection and optionally sends a RST to the remote
 * host.  Deletes the local protocol control block. This is done when
 * a connection is killed because of shortage of memory.
 *
 * @param pcb the tcp_pcb to abort
 * @param reset boolean to indicate whether a reset should be sent
 */
void
tcp_abandon(struct tcp_pcb *pcb, int reset)
{
  u32_t seqno, ackno;
#if LWIP_CALLBACK_API
  tcp_err_fn errf;
#endif /* LWIP_CALLBACK_API */
  void *errf_arg;

  /* pcb->state LISTEN not allowed here */
  LWIP_ASSERT("don't call tcp_abort/tcp_abandon for listen-pcbs",
    pcb->state != LISTEN);
  /* Figure out on which TCP PCB list we are, and remove us. If we
     are in an active state, call the receive function associated with
     the PCB with a NULL argument, and send an RST to the remote end. */
  if (pcb->state == TIME_WAIT) {
    tcp_pcb_remove(&tcp_tw_pcbs, pcb);
    memp_free(MEMP_TCP_PCB, pcb);
  } else {
    seqno = pcb->snd_nxt;
    ackno = pcb->rcv_nxt;
#if LWIP_CALLBACK_API
    errf = pcb->errf;
#endif /* LWIP_CALLBACK_API */
    errf_arg = pcb->callback_arg;
    TCP_PCB_REMOVE_ACTIVE(pcb);
    if (pcb->unacked != NULL) {
      tcp_segs_free(pcb->unacked);
    }
    if (pcb->unsent != NULL) {
      tcp_segs_free(pcb->unsent);
    }
#if TCP_QUEUE_OOSEQ
    if (pcb->ooseq != NULL) {
      tcp_segs_free(pcb->ooseq);
    }
#endif /* TCP_QUEUE_OOSEQ */
#if LWIP_SACK_PERF_OPT
    if (pcb->fr_segs != NULL) {
      tcp_fr_segs_free(pcb->fr_segs);
    }
#endif
    if (reset) {
      LWIP_DEBUGF(TCP_RST_DEBUG, ("tcp_abandon: sending RST\n"));
      tcp_rst(seqno, ackno, &pcb->local_ip, &pcb->remote_ip, pcb->local_port, pcb->remote_port);
    }
    memp_free(MEMP_TCP_PCB, pcb);
    TCP_EVENT_ERR(errf, errf_arg, ERR_ABRT);
  }
}

/**
 * Aborts the connection by sending a RST (reset) segment to the remote
 * host. The pcb is deallocated. This function never fails.
 *
 * ATTENTION: When calling this from one of the TCP callbacks, make
 * sure you always return ERR_ABRT (and never return ERR_ABRT otherwise
 * or you will risk accessing deallocated memory or memory leaks!
 *
 * @param pcb the tcp pcb to abort
 */
void
tcp_abort(struct tcp_pcb *pcb)
{
  tcp_abandon(pcb, 1);
}

/**
 * Binds the connection to a local portnumber and IP_add. If the
 * IP_add is not given (i.e., ipaddr == NULL), the IP_add of
 * the outgoing network interface is used instead.
 *
 * @param pcb the tcp_pcb to bind (no check is done whether this pcb is
 *        already bound!)
 * @param ipaddr the local IP_add to bind to (use IP_ADDR_ANY to bind
 *        to any local address
 * @param port the local port to bind to
 * @return ERR_USE if the port is already in use
 *         ERR_VAL if bind failed because the PCB is not in a valid state
 *         ERR_OK if bound
 */
err_t
tcp_bind(struct tcp_pcb *pcb, ip_addr_t *ipaddr, u16_t port)
{
  int i;
  int max_pcb_list = NUM_TCP_PCB_LISTS;
  struct tcp_pcb *cpcb;

  LWIP_ERROR("tcp_bind: can only bind in state CLOSED", pcb->state == CLOSED, return ERR_VAL;);

#if SO_REUSE
  /* Unless the REUSEADDR flag is set,
     we have to check the pcbs in TIME-WAIT state, also.
     We do not dump TIME_WAIT pcb's; they can still be matched by incoming
     packets using both local and remote IP_add and ports to distinguish.
   */
  if (ip_get_option(pcb, SOF_REUSEADDR)) {
    max_pcb_list = NUM_TCP_PCB_LISTS_NO_TIME_WAIT;
  }
#endif /* SO_REUSE */

  if (port == 0) {
    port = tcp_new_port();
    if (port == 0) {
      return ERR_USE;
    }
  }

  /* Check if the address already is in use (on all lists) */
  for (i = 0; i < max_pcb_list; i++) {
    for(cpcb = *tcp_pcb_lists[i]; cpcb != NULL; cpcb = cpcb->next) {
      if (cpcb->local_port == port) {
#if SO_REUSE
        /* Omit checking for the same port if both pcbs have REUSEADDR set.
           For SO_REUSEADDR, the duplicate-check for a 5-tuple is done in
           tcp_connect. */
        if (!ip_get_option(pcb, SOF_REUSEADDR) ||
            !ip_get_option(cpcb, SOF_REUSEADDR))
#endif /* SO_REUSE */
        {
          if (ip_addr_isany(&(cpcb->local_ip)) ||
              ip_addr_isany(ipaddr) ||
              ip_addr_cmp(&(cpcb->local_ip), ipaddr)) {
            return ERR_USE;
          }
        }
      }
    }
  }

  if (!ip_addr_isany(ipaddr)) {
    pcb->local_ip = *ipaddr;
  }
  pcb->local_port = port;
  TCP_REG(&tcp_bound_pcbs, pcb);
  LWIP_DEBUGF(TCP_DEBUG, ("tcp_bind: bind to port %"U16_F"\n", port));
  return ERR_OK;
}
#if LWIP_CALLBACK_API
/**
 * Default accept callback if no accept callback is specified by the user.
 */
err_t
tcp_accept_null(void *arg, struct tcp_pcb *pcb, err_t err)
{
  LWIP_UNUSED_ARG(arg);
  LWIP_UNUSED_ARG(pcb);
  LWIP_UNUSED_ARG(err);

  return ERR_ABRT;
}
#endif /* LWIP_CALLBACK_API */

/**
 * Set the state of the connection to be LISTEN, which means that it
 * is able to accept incoming connections. The protocol control block
 * is reallocated in order to consume less memory. Setting the
 * connection to LISTEN is an irreversible process.
 *
 * @param pcb the original tcp_pcb
 * @param backlog the incoming connections queue limit
 * @return tcp_pcb used for listening, consumes less memory.
 *
 * @note The original tcp_pcb is freed. This function therefore has to be
 *       called like this:
 *             tpcb = tcp_listen(tpcb);
 */
struct tcp_pcb *
tcp_listen_with_backlog(struct tcp_pcb *pcb, u8_t backlog)
{
  struct tcp_pcb_listen *lpcb;

  LWIP_UNUSED_ARG(backlog);
  LWIP_ERROR("tcp_listen: pcb already connected", pcb->state == CLOSED, return NULL;);

#if SO_REUSE
  if (ip_get_option(pcb, SOF_REUSEADDR)) {
    /* Since SOF_REUSEADDR allows reusing a local address before the pcb's usage
       is declared (listen-/connection-pcb), we have to make sure now that
       this port is only used once for every local IP. */
    for(lpcb = tcp_listen_pcbs.listen_pcbs; lpcb != NULL; lpcb = lpcb->next) {
      if (lpcb->local_port == pcb->local_port) {
        if (ip_addr_cmp(&lpcb->local_ip, &pcb->local_ip)) {
          /* this address/port is already used */
          return NULL;
        }
      }
    }
  }
#endif /* SO_REUSE */
  lpcb = (struct tcp_pcb_listen *)memp_malloc(MEMP_TCP_PCB_LISTEN);
  if (lpcb == NULL) {
    return NULL;
  }
  (void)MEMSET_S(lpcb , sizeof(struct tcp_pcb_listen), 0, sizeof(struct tcp_pcb_listen));

  lpcb->callback_arg = pcb->callback_arg;
  lpcb->local_port = pcb->local_port;
  lpcb->state = LISTEN;
  lpcb->prio = pcb->prio;
  lpcb->so_options = pcb->so_options;
  ip_set_option(lpcb, SOF_ACCEPTCONN);
  lpcb->ttl = pcb->ttl;
  lpcb->tos = pcb->tos;
  ip_addr_copy(lpcb->local_ip, pcb->local_ip);
  if (pcb->local_port != 0) {
    TCP_RMV(&tcp_bound_pcbs, pcb);
  }
  memp_free(MEMP_TCP_PCB, pcb);
#if LWIP_CALLBACK_API
  lpcb->accept = tcp_accept_null;
#endif /* LWIP_CALLBACK_API */
#if TCP_LISTEN_BACKLOG
  lpcb->accepts_pending = 0;
  lpcb->backlog = (u8_t)(backlog ? backlog : 1);
#endif /* TCP_LISTEN_BACKLOG */
  TCP_REG(&tcp_listen_pcbs.pcbs, (struct tcp_pcb *)lpcb);
  return (struct tcp_pcb *)lpcb;
}

/**
 * Update the state that keeps tabs on the available window space to advertise.
 *
 * Returns how much extra window would be advertised if we sent an
 * update now.
 */
u32_t tcp_update_rcv_ann_wnd(struct tcp_pcb *pcb)
{
  u32_t new_right_edge = pcb->rcv_nxt + pcb->rcv_wnd;

  if (TCP_SEQ_GEQ(new_right_edge, pcb->rcv_ann_right_edge + LWIP_MIN((pcb->tcpwnd / 2), pcb->mss))) {
    /* we can advertise more window */
    pcb->rcv_ann_wnd = pcb->rcv_wnd;
    return new_right_edge - pcb->rcv_ann_right_edge;
  } else {
    if (TCP_SEQ_GT(pcb->rcv_nxt, pcb->rcv_ann_right_edge)) {
      /* Can happen due to other end sending out of advertised window,
       * but within actual available (but not yet advertised) window */
      pcb->rcv_ann_wnd = 0;
    } else {
      /* keep the right edge of window constant */
      u32_t new_rcv_ann_wnd = pcb->rcv_ann_right_edge - pcb->rcv_nxt;
      /* Adding for WINDOW SCALE */
#if !LWIP_WND_SCALE
      LWIP_ASSERT("new_rcv_ann_wnd <= 0xffff", new_rcv_ann_wnd <= 0xffff);
#endif
      /* Adding for WINDOW SCALE */
      pcb->rcv_ann_wnd = (tcpwnd_size_t)new_rcv_ann_wnd;
    }
    return 0;
  }
}

/**
 * This function should be called by the application when it has
 * processed the data. The purpose is to advertise a larger window
 * when the data has been processed.
 *
 * @param pcb the tcp_pcb for which data is read
 * @param len the amount of bytes that have been read by the application
 */
void
tcp_recved(struct tcp_pcb *pcb, u16_t len)
{
  u32_t wnd_inflation;

  /* pcb->state LISTEN not allowed here */
  LWIP_ASSERT("don't call tcp_recved for listen-pcbs",
    pcb->state != LISTEN);

  /* Adding for WINDOW SCALE */
#if LWIP_WND_SCALE
  LWIP_ASSERT("tcp_recved: len would wrap rcv_wnd\n",
              len <= 0xffffffffU - pcb->rcv_wnd );
#else /* LWIP_WND_SCALE */
  LWIP_ASSERT("tcp_recved: len would wrap rcv_wnd\n",
              len <= 0xffff - pcb->rcv_wnd );
#endif /* LWIP_WND_SCALE */

/* Adding for WINDOW SCALE */
  pcb->rcv_wnd += len;
  if (pcb->rcv_wnd > pcb->tcpwnd) {
    pcb->rcv_wnd = pcb->tcpwnd;
  }else if(pcb->rcv_wnd == 0) {
    /* rcv_wnd overflowed */
    if ((pcb->state == CLOSE_WAIT) || (pcb->state == LAST_ACK)) {
      /* In passive close, we allow this, since the FIN bit is added to rcv_wnd
         by the stack itself, since it is not mandatory for an application
         to call tcp_recved() for the FIN bit, but e.g. the netconn API does so. */
      pcb->rcv_wnd = pcb->tcpwnd;
    } else {
      LWIP_ASSERT("tcp_recved: len wrapped rcv_wnd\n", 0);
    }
  }

  /* PC Lint Fix for: Loss of precision (assignment) (unsigned long to int) */
  /* Following function may return negative value, so typecase to int */
  wnd_inflation = tcp_update_rcv_ann_wnd(pcb);

  /* If the change in the right edge of window is significant (default
   * watermark is TCP_WND/4), then send an explicit update now.
   * Otherwise wait for a packet to be sent in the normal course of
   * events (or more window to be available later) */
  if (wnd_inflation >= (pcb->tcpwnd / (u32_t)TCP_WND_UPDATE_THRESHOLD)) {
    tcp_ack_now(pcb);
    tcp_output(pcb);
  }

  LWIP_DEBUGF(TCP_DEBUG, ("tcp_recved: recveived %"U16_F" bytes, wnd %"U16_F" (%"U16_F").\n",
         len, pcb->rcv_wnd, pcb->tcpwnd - pcb->rcv_wnd));
}

/**
 * Allocate a new local TCP port.
 *
 * @return a new (free) local TCP port number
 */
LWIP_STATIC u16_t
tcp_new_port(void)
{
  u8_t i;
  u16_t n = 0;
  u16_t tcp_port;
  struct tcp_pcb *pcb;

again:
  tcp_port = TCP_LOCAL_PORT_RANGE_START +
             (LWIP_RAND() % (TCP_LOCAL_PORT_RANGE_END - TCP_LOCAL_PORT_RANGE_START + 1));
			 
  /* Check all PCB lists. */
  for (i = 0; i < NUM_TCP_PCB_LISTS; i++) {
    for(pcb = *tcp_pcb_lists[i]; pcb != NULL; pcb = pcb->next) {
      if (pcb->local_port == tcp_port) {
        if (++n > (TCP_LOCAL_PORT_RANGE_END - TCP_LOCAL_PORT_RANGE_START)) {
          return 0;
        }
        goto again;
      }
    }
  }
  return tcp_port;
}

/**
 * Connects to another host. The function given as the "connected"
 * argument will be called when the connection has been established.
 *
 * @param pcb the tcp_pcb used to make the connection
 * @param ipaddr the remote IP_add to connect to
 * @param port the remote tcp port to connect to
 * @param connected callback function to call when connected (or on error)
 * @return ERR_VAL if invalid arguments are given
 *         ERR_OK if connect request has been sent
 *         other err_t values if connect request couldn't be sent
 */
err_t
tcp_connect(struct tcp_pcb *pcb, ip_addr_t *ipaddr, u16_t port,
      tcp_connected_fn connected)
{
  err_t ret;
  u32_t iss;
  u16_t old_local_port;

  LWIP_ERROR("tcp_connect: can only connect from state CLOSED", pcb->state == CLOSED, return ERR_ISCONN;);

  LWIP_DEBUGF(TCP_DEBUG, ("tcp_connect to port %"U16_F"\n", port));
  if (ipaddr != NULL) {
    pcb->remote_ip = *ipaddr;
  } else {
    return ERR_VAL;
  }
  pcb->remote_port = port;

  /* check if we have a route to the remote host */
/* NETIF DRIVER STATUS BEGIN */
#if DRIVER_STATUS_CHECK
  struct netif *netif = ip_route(&(pcb->remote_ip), NULL);
  if (netif == NULL) {
    /* Don't even try to send a SYN packet if we have no route
       since that will fail. */
    return ERR_RTE;
  }

  if (!(netif->flags & NETIF_FLAG_DRIVER_RDY)) {
    pcb->drv_status = DRV_NOT_READY;
  } else {
    pcb->drv_status = DRV_READY;
  }

  if (ip_addr_isany(&(pcb->local_ip))) {
    /* Use the netif's IP_add as local address. */
    ip_addr_copy(pcb->local_ip, netif->ip_addr);
  }
#else
  if (ip_addr_isany(&(pcb->local_ip))) {
    /* no local IP_add set, yet. */
    struct netif *netif = ip_route(&(pcb->remote_ip), NULL);
    if (netif == NULL) {
      /* Don't even try to send a SYN packet if we have no route
         since that will fail. */
      return ERR_RTE;
    }
    /* Use the netif's IP_add as local address. */
    ip_addr_copy(pcb->local_ip, netif->ip_addr);
  }
#endif
/* NETIF DRIVER STATUS END */

  old_local_port = pcb->local_port;
  if (pcb->local_port == 0) {
    pcb->local_port = tcp_new_port();
    if (pcb->local_port == 0) {
      return ERR_BUF;
    }
  }
#if SO_REUSE
  if (ip_get_option(pcb, SOF_REUSEADDR)) {
    /* Since SOF_REUSEADDR allows reusing a local address, we have to make sure
       now that the 5-tuple is unique. */
    struct tcp_pcb *cpcb;
    int i;
    /* Don't check listen- and bound-PCBs, check active- and TIME-WAIT PCBs. */
    for (i = 2; i < NUM_TCP_PCB_LISTS; i++) {
      for(cpcb = *tcp_pcb_lists[i]; cpcb != NULL; cpcb = cpcb->next) {
        if ((cpcb->local_port == pcb->local_port) &&
            (cpcb->remote_port == port) &&
            ip_addr_cmp(&cpcb->local_ip, &pcb->local_ip) &&
            ip_addr_cmp(&cpcb->remote_ip, ipaddr)) {
          /* linux returns EISCONN here, but ERR_USE should be OK for us */
          return ERR_USE;
        }
      }
    }
  }
#endif /* SO_REUSE */
  iss = tcp_next_iss();
  pcb->rcv_nxt = 0;
  pcb->snd_nxt = iss;
  pcb->lastack = iss - 1;
  pcb->snd_lbb = iss - 1;
#if LWIP_WND_SCALE
  tcp_init_ws(pcb);
#else
  pcb->rcv_wnd = CFG_TCP_WND;
  pcb->rcv_ann_wnd = CFG_TCP_WND;
  pcb->tcpwnd = CFG_TCP_WND;
#endif

  pcb->rcv_ann_right_edge = pcb->rcv_nxt;
  pcb->snd_wnd = CFG_TCP_WND;
  /* As initial send MSS, we use TCP_MSS but limit it to 536.
     The send MSS is updated when an MSS option is received. */
  /*PC Lint fix: Constant value Boolean*/
  /*lint -e506*/
  pcb->mss = (u16_t)((CFG_TCP_MSS > 536) ? 536 : CFG_TCP_MSS);
  /*lint +e506*/
#if TCP_CALCULATE_EFF_SEND_MSS
  pcb->mss = tcp_eff_send_mss(pcb->mss, ipaddr);
#endif /* TCP_CALCULATE_EFF_SEND_MSS */
  pcb->cwnd = (tcpwnd_size_t)((tcpwnd_size_t)INITIAL_CWND * (tcpwnd_size_t)pcb->mss);
  /* PC Lint Fix for: Loss of precision (assignment) (20 bits to 16 bits) */
  /* (pcb->mss * 10) value wont exceed from 15000, so typecast as u16_t */
  pcb->ssthresh = (tcpwnd_size_t) ((tcpwnd_size_t)INITIAL_SSTHRESH * (tcpwnd_size_t)pcb->mss);
#if LWIP_CALLBACK_API
  pcb->connected = connected;
#else /* LWIP_CALLBACK_API */
  LWIP_UNUSED_ARG(connected);
#endif /* LWIP_CALLBACK_API */

    /* Adding for SACK */
#if LWIP_SACK
  tcp_connect_update_sack(pcb, iss);
#endif

  /* Send a SYN together with the MSS option. */
  ret = tcp_enqueue_flags(pcb, TCP_SYN);
  if (ret == ERR_OK) {
    /* SYN segment was enqueued, changed the pcbs state now */
    pcb->state = SYN_SENT;
    if (old_local_port != 0) {
      TCP_RMV(&tcp_bound_pcbs, pcb);
    }
    TCP_REG_ACTIVE(pcb);
    snmp_inc_tcpactiveopens();

    tcp_output(pcb);
  }
  return ret;
}

/**
 * Called every 500 ms and implements the retransmission timer and the timer that
 * removes PCBs that have been in TIME-WAIT for enough time. It also increments
 * various timers such as the inactivity timer in each PCB.
 *
 * Automatically called from tcp_tmr().
 */
void
tcp_slowtmr(void)
{
  struct tcp_pcb *pcb, *prev;

  u8_t pcb_remove;      /* flag if a PCB should be removed */
  u8_t pcb_reset;       /* flag if a RST should be sent when removing */
  err_t err = ERR_OK;
  tcpwnd_size_t eff_wnd;

/* NETIF DRIVER STATUS BEGIN */
#if DRIVER_STATUS_CHECK
  struct netif *netif;
#endif
/* NETIF DRIVER STATUS END */

  ++tcp_ticks;
  ++tcp_timer_ctr;

tcp_slowtmr_start:
  /* Steps through all of the active PCBs. */
  prev = NULL;
  pcb = tcp_active_pcbs;
  if (pcb == NULL) {
    LWIP_DEBUGF(TCP_DEBUG, ("tcp_slowtmr: no active pcbs\n"));
  }
/* NETIF DRIVER STATUS BEGIN */
#if DRIVER_STATUS_CHECK
  for (netif = netif_list; netif != NULL; netif = netif->next) {
    /* network mask matches? */
    if (!(netif->flags & NETIF_FLAG_DRIVER_RDY)) {
      if(netif->waketime <= DRIVER_WAKEUP_COUNT) {
        netif->waketime ++;
      }
    }
  }
#endif
/* NETIF DRIVER STATUS END */
  while (pcb != NULL) {
    LWIP_DEBUGF(TCP_DEBUG, ("tcp_slowtmr: processing active pcb\n"));
    LWIP_ASSERT("tcp_slowtmr: active pcb->state != CLOSED\n", pcb->state != CLOSED);
    LWIP_ASSERT("tcp_slowtmr: active pcb->state != LISTEN\n", pcb->state != LISTEN);
    LWIP_ASSERT("tcp_slowtmr: active pcb->state != TIME-WAIT\n", pcb->state != TIME_WAIT);
    if (pcb->last_timer == tcp_timer_ctr) {
      /* skip this pcb, we have already processed it */
      pcb = pcb->next;
      continue;
    }
    pcb->last_timer = tcp_timer_ctr;

    pcb_remove = 0;
    pcb_reset = 0;

    if (pcb->state == SYN_SENT && pcb->nrtx == CFG_TCP_SYNMAXRTX) {
      ++pcb_remove;
      LWIP_DEBUGF(TCP_DEBUG, ("tcp_slowtmr: max SYN retries reached\n"));
    } else if ((pcb->state == FIN_WAIT_1 || pcb->state == CLOSING) && pcb->nrtx == CFG_TCP_FW1MAXRTX) {
      ++pcb_remove;
      LWIP_DEBUGF(TCP_DEBUG, ("tcp_slowtmr: max DATA retries reached in FIN_WAIT_1 or CLOSING state\n"));
    } else if (pcb->nrtx == CFG_TCP_MAXRTX) {
      ++pcb_remove;
      LWIP_DEBUGF(TCP_DEBUG, ("tcp_slowtmr: max DATA retries reached\n"));
    } else {
      if (pcb->persist_backoff > 0) {
        /* If snd_wnd is zero, use persist timer to send 1 byte Signals
         * instead of using the standard retransmission mechanism. */
        pcb->persist_cnt++;
        if (pcb->persist_cnt >= tcp_persist_backoff[pcb->persist_backoff-1]) {
          pcb->persist_cnt = 0;
          if (pcb->persist_backoff < sizeof(tcp_persist_backoff)) {
            pcb->persist_backoff++;
          }
          tcp_zero_window_probe(pcb);
        }
      } else {
        /* Increase the retransmission timer if it is running */
        if(pcb->rtime >= 0) {
          ++pcb->rtime;
        }

        if (pcb->unacked != NULL && pcb->rtime >= pcb->rto) {
          /* Time for a retransmission. */
          LWIP_DEBUGF(TCP_RTO_DEBUG, ("tcp_slowtmr: rtime %"S16_F
                                      " pcb->rto %"S16_F"\n",
                                      pcb->rtime, pcb->rto));

          /* Double retransmission time-out unless we are trying to
           * connect to somebody (i.e., we are in SYN_SENT). */
          if (pcb->state != SYN_SENT) {
                /*lint -e701*/
                pcb->rto = (s16_t)((((u16_t)(pcb->sa) >> 3) + pcb->sv));
                if(pcb->rto > 2 && (pcb->state == ESTABLISHED || pcb->state == FIN_WAIT_1 || pcb->state == CLOSING))
                  pcb->rto = 2;
                else if(pcb->rto < 1 && (pcb->state == ESTABLISHED || pcb->state == FIN_WAIT_1 || pcb->state == CLOSING))
                  pcb->rto = 1;

                pcb->rto = (s16_t)(pcb->rto * tcp_backoff[pcb->nrtx]);
                /*lint +e701*/
          }

          /* Reset the retransmission timer. */
          pcb->rtime = 0;

          /* Reduce congestion window and ssthresh. */
          /* The following needs to be called AFTER cwnd is set to one
             mss - STJ */
          eff_wnd = LWIP_MIN(pcb->cwnd, pcb->snd_wnd);
          pcb->ssthresh = (eff_wnd *5) >> 3; /* divide by 5/8 */

          /* initial threshold may not be true enough to adjust to network condition
             it must be adjusted */
          if (pcb->ssthresh < (tcpwnd_size_t)(pcb->mss * 16)) {
            pcb->ssthresh = (tcpwnd_size_t)pcb->mss * 16;
          }

          pcb->cwnd = (tcpwnd_size_t)(pcb->mss * INITIAL_CWND) >> 1;

          /* The following needs to be called AFTER cwnd is set to one
             mss - STJ */
          tcp_rexmit_rto(pcb);
        }
      }
    }
/* NETIF DRIVER STATUS BEGIN */
#if DRIVER_STATUS_CHECK
    if(pcb->drv_status == DRV_NOT_READY){
      /* iterate through netifs */
      for (netif = netif_list; netif != NULL; netif = netif->next) {
        /* network mask matches? */
        if (ip_addr_netcmp(&pcb->remote_ip, &(netif->ip_addr), &(netif->netmask))) {
          if(netif->waketime > DRIVER_WAKEUP_COUNT) {
            /* waketime already incremented for a previous pcb, So, remove this pcb */
            LWIP_DEBUGF(DRV_STS_DEBUG, ("Driver Wake time max count (%d) reached. Removing PCB\n", netif->waketime));
            pcb_remove++;
          }
          break;
        }
      }
    }
#endif
/* NETIF DRIVER STATUS END */

    /* Check if this PCB has stayed too long in FIN-WAIT-2 */
    if (pcb->state == FIN_WAIT_2) {
      /* If this PCB is in FIN_WAIT_2 because of SHUT_WR don't let it time out. */
      if (pcb->flags & TF_RXCLOSED) {
        /* PCB was fully closed (either through close() or SHUT_RDWR):
           normal FIN-WAIT timeout handling. */
        if ((u32_t)(tcp_ticks - pcb->tmr) >
            TCP_FIN_WAIT_TIMEOUT / TCP_SLOW_INTERVAL) {
          ++pcb_remove;
          LWIP_DEBUGF(TCP_DEBUG, ("tcp_slowtmr: removing pcb stuck in FIN-WAIT-2\n"));
        }
      }
    }

    /* Check if KEEPALIVE should be sent */
    if(ip_get_option(pcb, SOF_KEEPALIVE) && ((pcb->state == ESTABLISHED) || (pcb->state == CLOSE_WAIT))) {
      if ((u32_t)(tcp_ticks - pcb->tmr) > (pcb->keep_idle + TCP_KEEP_DUR(pcb)) / TCP_SLOW_INTERVAL)
      {
        LWIP_DEBUGF(TCP_DEBUG, ("tcp_slowtmr: KEEPALIVE timeout. Aborting connection to %"U16_F".%"U16_F".%"U16_F".%"U16_F".\n",
                                ip4_addr1_16(&pcb->remote_ip), ip4_addr2_16(&pcb->remote_ip),
                                ip4_addr3_16(&pcb->remote_ip), ip4_addr4_16(&pcb->remote_ip)));

        ++pcb_remove;
        ++pcb_reset;
      }
      else if((u32_t)(tcp_ticks - pcb->tmr) > (pcb->keep_idle + pcb->keep_cnt_sent * TCP_KEEP_INTVL(pcb)) / TCP_SLOW_INTERVAL)
      {
        tcp_keepalive(pcb);
        pcb->keep_cnt_sent++;
      }
    }

    /* If this PCB has queued out of sequence data, but has been
       inactive for too long, will drop the data (it will eventually
       be retransmitted). */
#if TCP_QUEUE_OOSEQ
    /*lint -e737*/
    if (pcb->ooseq != NULL &&
        (long)(tcp_ticks - pcb->tmr) >= ((pcb->rto) *
        TCP_OOSEQ_TIMEOUT)) {
      tcp_segs_free(pcb->ooseq);
      pcb->ooseq = NULL;
      LWIP_DEBUGF(TCP_CWND_DEBUG, ("tcp_slowtmr: dropping OOSEQ queued data\n"));
    }
    /*lint +e737*/
#endif /* TCP_QUEUE_OOSEQ */

    /* Check if this PCB has stayed too long in SYN-RCVD */
    if (pcb->state == SYN_RCVD) {
      if ((u32_t)(tcp_ticks - pcb->tmr) >
          TCP_SYN_RCVD_TIMEOUT / TCP_SLOW_INTERVAL) {
        ++pcb_remove;
        LWIP_DEBUGF(TCP_DEBUG, ("tcp_slowtmr: removing pcb stuck in SYN-RCVD\n"));
      }
    }

    /* Check if this PCB has stayed too long in LAST-ACK */
    if (pcb->state == LAST_ACK) {
      if ((u32_t)(tcp_ticks - pcb->tmr) > 2 * TCP_MSL / TCP_SLOW_INTERVAL) {
        ++pcb_remove;
        LWIP_DEBUGF(TCP_DEBUG, ("tcp_slowtmr: removing pcb stuck in LAST-ACK\n"));
      }
    }

    /* If the PCB should be removed, do it. */
    if (pcb_remove) {
      struct tcp_pcb *pcb2;
      tcp_err_fn err_fn;
      void *err_arg;
      tcp_pcb_purge(pcb);
      /* Remove PCB from tcp_active_pcbs list. */
      if (prev != NULL) {
        LWIP_ASSERT("tcp_slowtmr: middle tcp != tcp_active_pcbs", pcb != tcp_active_pcbs);
        prev->next = pcb->next;
      } else {
        /* This PCB was the first. */
        LWIP_ASSERT("tcp_slowtmr: first pcb == tcp_active_pcbs", tcp_active_pcbs == pcb);
        tcp_active_pcbs = pcb->next;
      }

      if (pcb_reset) {
        tcp_rst(pcb->snd_nxt, pcb->rcv_nxt, &pcb->local_ip, &pcb->remote_ip, pcb->local_port, pcb->remote_port);
      }

      err_fn = pcb->errf;
      err_arg = pcb->callback_arg;
      pcb2 = pcb;
      pcb = pcb->next;

      memp_free(MEMP_TCP_PCB, pcb2);

      tcp_active_pcbs_changed = 0;
      TCP_EVENT_ERR(err_fn, err_arg, ERR_ABRT);
      if (tcp_active_pcbs_changed) {
        goto tcp_slowtmr_start;
      }
    } else {
      /* get the 'next' element now and work with 'prev' below (in case of abort) */
      prev = pcb;
      pcb = pcb->next;

      /* We check if we should poll the connection. */
      ++prev->polltmr;
      if (prev->polltmr >= prev->pollinterval) {
        prev->polltmr = 0;
        LWIP_DEBUGF(TCP_DEBUG, ("tcp_slowtmr: polling application\n"));
        tcp_active_pcbs_changed = 0;
        TCP_EVENT_POLL(prev, err);
        if (tcp_active_pcbs_changed) {
          goto tcp_slowtmr_start;
        }
        /* if err == ERR_ABRT, 'prev' is already deallocated */
        if (err == ERR_OK) {
          tcp_output(prev);
        }
      }
    }
  }

  /* Steps through all of the TIME-WAIT PCBs. */
  prev = NULL;
  pcb = tcp_tw_pcbs;
  while (pcb != NULL) {
    LWIP_ASSERT("tcp_slowtmr: TIME-WAIT pcb->state == TIME-WAIT", pcb->state == TIME_WAIT);
    pcb_remove = 0;

    /* Check if this PCB has stayed long enough in TIME-WAIT */
    if ((u32_t)(tcp_ticks - pcb->tmr) > 2 * TCP_MSL / TCP_SLOW_INTERVAL) {
      ++pcb_remove;
    }



    /* If the PCB should be removed, do it. */
    if (pcb_remove) {
      struct tcp_pcb *pcb2;
      tcp_pcb_purge(pcb);
      /* Remove PCB from tcp_tw_pcbs list. */
      if (prev != NULL) {
        LWIP_ASSERT("tcp_slowtmr: middle tcp != tcp_tw_pcbs", pcb != tcp_tw_pcbs);
        prev->next = pcb->next;
      } else {
        /* This PCB was the first. */
        LWIP_ASSERT("tcp_slowtmr: first pcb == tcp_tw_pcbs", tcp_tw_pcbs == pcb);
        tcp_tw_pcbs = pcb->next;
      }
      pcb2 = pcb;
      pcb = pcb->next;
      memp_free(MEMP_TCP_PCB, pcb2);
    } else {
      prev = pcb;
      pcb = pcb->next;
    }
  }
}

/**
 * Is called every TCP_FAST_INTERVAL (250 ms) and process data previously
 * "refused" by upper layer (application) and sends delayed ACKs.
 *
 * Automatically called from tcp_tmr().
 */
void
tcp_fasttmr(void)
{
  struct tcp_pcb *pcb;

  ++tcp_timer_ctr;

tcp_fasttmr_start:
  pcb = tcp_active_pcbs;

  while(pcb != NULL) {
    if (pcb->last_timer != tcp_timer_ctr) {
      struct tcp_pcb *next;
      pcb->last_timer = tcp_timer_ctr;
      /* send delayed ACKs */
      if (pcb->flags & TF_ACK_DELAY) {
        LWIP_DEBUGF(TCP_DEBUG, ("tcp_fasttmr: delayed ACK\n"));
        tcp_ack_now(pcb);
        tcp_output(pcb);
        pcb->flags = (tcpflags_t)(pcb->flags & (~(TF_ACK_DELAY | TF_ACK_NOW)));
      }

      next = pcb->next;

      /* If there is data which was previously "refused" by upper layer */
      if (pcb->refused_data != NULL) {
        tcp_active_pcbs_changed = 0;
        tcp_process_refused_data(pcb);
        if (tcp_active_pcbs_changed) {
          /* application callback has changed the pcb list: restart the loop */
          goto tcp_fasttmr_start;
        }
      }
      pcb = next;
    }
  }
}

/** Pass pcb->refused_data to the recv callback */
err_t
tcp_process_refused_data(struct tcp_pcb *pcb)
{
  /* Adding for WINDOW SCALE */
#if TCP_QUEUE_OOSEQ && LWIP_WND_SCALE
  struct pbuf *rest;
  while (pcb->refused_data != NULL)
#endif /* TCP_QUEUE_OOSEQ && LWIP_WND_SCALE */
  /* Adding for WINDOW SCALE */
  {
    err_t err;
    u16_t refused_flags = pcb->refused_data->flags;
    /* set pcb->refused_data to NULL in case the callback frees it and then
       closes the pcb */
    struct pbuf *refused_data = pcb->refused_data;

    /* Adding for WINDOW SCALE */
#if TCP_QUEUE_OOSEQ && LWIP_WND_SCALE
    pbuf_split_64k(refused_data, &rest);
    pcb->refused_data = rest;
#else /* TCP_QUEUE_OOSEQ && LWIP_WND_SCALE */
    pcb->refused_data = NULL;
#endif /* TCP_QUEUE_OOSEQ && LWIP_WND_SCALE */

    /* Notify again application with data previously received. */
    LWIP_DEBUGF(TCP_INPUT_DEBUG, ("tcp_input: notify kept packet\n"));
    TCP_EVENT_RECV(pcb, refused_data, ERR_OK, err);
    if (err == ERR_OK) {
      /* did refused_data include a FIN? */
      if (refused_flags & PBUF_FLAG_TCP_FIN
          /* Adding for WINDOW SCALE */
#if TCP_QUEUE_OOSEQ && LWIP_WND_SCALE
          && (rest == NULL)
#endif /* TCP_QUEUE_OOSEQ && LWIP_WND_SCALE */
         ) {
        /* correct rcv_wnd as the application won't call tcp_recved()
           for the FIN's seqno */
        if (pcb->rcv_wnd != pcb->tcpwnd) {
          pcb->rcv_wnd++;
        }
        TCP_EVENT_CLOSED(pcb, err);
        if (err == ERR_MEM) {/* TCP_EVENT_CLOSED never return ERR_ABRT, but return ERR_MEM */
          return ERR_ABRT;
        }
      }
    } else if (err == ERR_MEM){/* drop packet if out of memory, to avoid dead loop */
      /* if err == ERR_ABRT, 'pcb' is already deallocated */
      /* Drop incoming packets because pcb is "full" (only if the incoming
         segment contains data). */
      LWIP_DEBUGF(TCP_INPUT_DEBUG, ("tcp_input: drop incoming packets, because pcb is \"full\"\n"));
      return ERR_ABRT;
    } else {
      /* data is still refused, pbuf is still valid (go on for ACK-only packets) */
      /* Adding for WINDOW SCALE */
#if TCP_QUEUE_OOSEQ && LWIP_WND_SCALE
      if (rest != NULL) {
        pbuf_cat(refused_data, rest);
      }
#endif /* TCP_QUEUE_OOSEQ && LWIP_WND_SCALE */
      pcb->refused_data = refused_data;
    }
  /* Adding for WINDOW SCALE */
  }
  return ERR_OK;
}

#if LWIP_SACK_PERF_OPT
/**
 * Deallocates a list of Fast retransmitted TCP segments (tcp_seg structures).
 *
 * @param seg fr_segs list of TCP segments to free
 */
void
tcp_fr_segs_free(struct tcp_sack_fast_rxmited *seg)
{
  while (seg != NULL) {
    struct tcp_sack_fast_rxmited *next = seg->next;
    mem_free(seg);
    seg = next;
  }
}
#endif

/**
 * Deallocates a list of TCP segments (tcp_seg structures).
 *
 * @param seg tcp_seg list of TCP segments to free
 */
void
tcp_segs_free(struct tcp_seg *seg)
{
  while (seg != NULL) {
    struct tcp_seg *next = seg->next;
    tcp_seg_free(seg);
    seg = next;
  }
}

/**
 * Frees a TCP segment (tcp_seg structure).
 *
 * @param seg single tcp_seg to free
 */
void
tcp_seg_free(struct tcp_seg *seg)
{
  if (seg != NULL) {
    if (seg->p != NULL) {
      pbuf_free(seg->p);
#if TCP_DEBUG
      seg->p = NULL;
#endif /* TCP_DEBUG */
    }
    memp_free(MEMP_TCP_SEG, seg);
  }
}


#if TCP_QUEUE_OOSEQ
/**
 * Returns a copy of the given TCP segment.
 * The pbuf and data are not copied, only the pointers
 *
 * @param seg the old tcp_seg
 * @return a copy of seg
 */
struct tcp_seg *
tcp_seg_copy(struct tcp_seg *seg)
{
  struct tcp_seg *cseg;

  cseg = (struct tcp_seg *)memp_malloc(MEMP_TCP_SEG);
  if (cseg == NULL) {
    return NULL;
  }
  (void)SMEMCPY_S((u8_t *)cseg, sizeof(struct tcp_seg), (const u8_t *)seg, sizeof(struct tcp_seg)); //CSEC_FIX_2302
  pbuf_ref(cseg->p);
  return cseg;
}
#endif /* TCP_QUEUE_OOSEQ */

#if LWIP_CALLBACK_API
/**
 * Default receive callback that is called if the user didn't register
 * a recv callback for the pcb.
 */
err_t
tcp_recv_null(void *arg, struct tcp_pcb *pcb, struct pbuf *p, err_t err)
{
  LWIP_UNUSED_ARG(arg);
  if (p != NULL) {
    tcp_recved(pcb, p->tot_len);
    pbuf_free(p);
  } else if (err == ERR_OK) {
    return tcp_close(pcb);
  }
  return ERR_OK;
}
#endif /* LWIP_CALLBACK_API */

/**
 * Kills the oldest active connection that has the same or lower priority than
 * 'prio'.
 *
 * @param prio minimum priority
 */
LWIP_STATIC void
tcp_kill_prio(u8_t prio)
{
  struct tcp_pcb *pcb, *inactive;
  u32_t inactivity;
  u8_t mprio;


  mprio = TCP_PRIO_MAX;

  /* We kill the oldest active connection that has lower priority than prio. */
  inactivity = 0;
  inactive = NULL;
  for(pcb = tcp_active_pcbs; pcb != NULL; pcb = pcb->next) {
    if (pcb->prio <= prio && pcb->prio <= mprio && (u32_t)(tcp_ticks - pcb->tmr) >= inactivity) {
      inactivity = tcp_ticks - pcb->tmr;
      inactive = pcb;
      mprio = pcb->prio;
    }
  }
  if (inactive != NULL) {
    LWIP_DEBUGF(TCP_DEBUG, ("tcp_kill_prio: killing oldest PCB %p (%"S32_F")\n",
           (void *)inactive, inactivity));
    tcp_abort(inactive);
  }
}

/**
 * Kills the oldest connection that is in TIME_WAIT state.
 * Called from tcp_alloc() if no more connections are available.
 */
static void
tcp_kill_timewait(void)
{
  struct tcp_pcb *pcb, *inactive;
  u32_t inactivity;

  inactivity = 0;
  inactive = NULL;
  /* Go through the list of TIME_WAIT pcbs and get the oldest pcb. */
  for(pcb = tcp_tw_pcbs; pcb != NULL; pcb = pcb->next) {
    if ((u32_t)(tcp_ticks - pcb->tmr) >= inactivity) {
      inactivity = tcp_ticks - pcb->tmr;
      inactive = pcb;
    }
  }
  if (inactive != NULL) {
    LWIP_DEBUGF(TCP_DEBUG, ("tcp_kill_timewait: killing oldest TIME-WAIT PCB %p (%"S32_F")\n",
           (void *)inactive, (s32_t)inactivity));
    tcp_abort(inactive);
  }
}

/**
 * Allocate a new tcp_pcb structure.
 *
 * @param prio priority for the new pcb
 * @return a new tcp_pcb that initially is in state CLOSED
 */
struct tcp_pcb *
tcp_alloc(u8_t prio)
{
  struct tcp_pcb *pcb;
  u32_t iss;

  pcb = (struct tcp_pcb *)memp_malloc(MEMP_TCP_PCB);
  if (pcb == NULL) {
    /* Try killing oldest connection in TIME-WAIT. */
    LWIP_DEBUGF(TCP_DEBUG, ("tcp_alloc: killing off oldest TIME-WAIT connection\n"));
    tcp_kill_timewait();
    /* Try to allocate a tcp_pcb again. */
    pcb = (struct tcp_pcb *)memp_malloc(MEMP_TCP_PCB);
    if (pcb == NULL) {
      /* Try killing active connections with lower priority than the new one. */
      LWIP_DEBUGF(TCP_DEBUG, ("tcp_alloc: killing connection with prio lower than %d\n", prio));
      tcp_kill_prio(prio);
      /* Try to allocate a tcp_pcb again. */
      pcb = (struct tcp_pcb *)memp_malloc(MEMP_TCP_PCB);
      if (pcb != NULL) {
        /* adjust err stats: memp_malloc failed twice before */
        MEMP_STATS_DEC(err, MEMP_TCP_PCB);
      }
    }
    if (pcb != NULL) {
      /* adjust err stats: timewait PCB was freed above */
      MEMP_STATS_DEC(err, MEMP_TCP_PCB);
    }
  }
  if (pcb != NULL) {
    (void)MEMSET_S(pcb, sizeof(struct tcp_pcb), 0, sizeof(struct tcp_pcb)); //CSEC_FIX_2302
    pcb->prio = prio;
    pcb->snd_buf = CFG_TCP_SND_BUF;
    pcb->snd_queuelen = 0;
    /* Adding for WINDOW SCALE */
#if LWIP_WND_SCALE
    tcp_init_ws(pcb);
#else
    pcb->rcv_wnd = CFG_TCP_WND;
    pcb->rcv_ann_wnd = CFG_TCP_WND;
    pcb->tcpwnd = CFG_TCP_WND;
#endif
    pcb->tos = 0;
    pcb->ttl = CFG_TCP_TTL;
    /* As initial send MSS, we use TCP_MSS but limit it to 536.
       The send MSS is updated when an MSS option is received. */
    /*PC Lint fix: Constant value Boolean*/
    /*lint -e506*/
    pcb->mss = (u16_t)((CFG_TCP_MSS > 536) ? 536 : CFG_TCP_MSS);
    pcb->rcv_mss = 536;
    /*lint +e506*/
    pcb->rto = 3000 / TCP_SLOW_INTERVAL;
    pcb->sa = 0;
    pcb->sv = 3000 / TCP_SLOW_INTERVAL;
    pcb->rtime = -1;
    pcb->cwnd = (tcpwnd_size_t)((tcpwnd_size_t)INITIAL_CWND * (tcpwnd_size_t)pcb->mss);
    iss = tcp_next_iss();
    pcb->snd_wl2 = iss;
    pcb->snd_nxt = iss;
    pcb->lastack = iss;
    pcb->snd_lbb = iss;
    pcb->tmr = tcp_ticks;
    pcb->last_timer = tcp_timer_ctr;

    pcb->polltmr = 0;

#if LWIP_CALLBACK_API
    pcb->recv = tcp_recv_null;
#endif /* LWIP_CALLBACK_API */

    /* Init KEEPALIVE timer */
    pcb->keep_idle  = TCP_KEEPIDLE_DEFAULT;

#if LWIP_TCP_KEEPALIVE
    pcb->keep_intvl = TCP_KEEPINTVL_DEFAULT;
    pcb->keep_cnt   = TCP_KEEPCNT_DEFAULT;
#endif /* LWIP_TCP_KEEPALIVE */

    pcb->keep_cnt_sent = 0;
  }
  return pcb;
}

/**
 * Creates a new TCP protocol control block but doesn't place it on
 * any of the TCP PCB lists.
 * The pcb is not put on any list until binding using tcp_bind().
 *
 * @internal: Maybe there should be a idle TCP PCB list where these
 * PCBs are put on. Port reservation using tcp_bind() is implemented but
 * allocated pcbs that are not bound can't be killed automatically if wanting
 * to allocate a pcb with higher prio (@see tcp_kill_prio())
 *
 * @return a new tcp_pcb that initially is in state CLOSED
 */
struct tcp_pcb *
tcp_new(void)
{
  return tcp_alloc(TCP_PRIO_NORMAL);
}

/**
 * Used to specify the argument that should be passed callback
 * functions.
 *
 * @param pcb tcp_pcb to set the callback argument
 * @param arg void pointer argument to pass to callback functions
 */
void
tcp_arg(struct tcp_pcb *pcb, void *arg)
{
  /* This function is allowed to be called for both listen pcbs and
     connection pcbs. */
  pcb->callback_arg = arg;
}
#if LWIP_CALLBACK_API

/**
 * Used to specify the function that should be called when a TCP
 * connection receives data.
 *
 * @param pcb tcp_pcb to set the recv callback
 * @param recv callback function to call for this pcb when data is received
 */
void
tcp_recv(struct tcp_pcb *pcb, tcp_recv_fn recv)
{
  LWIP_ASSERT("invalid socket state for recv callback", pcb->state != LISTEN);
  pcb->recv = recv;
}

/**
 * Used to specify the function that should be called when TCP data
 * has been successfully delivered to the remote host.
 *
 * @param pcb tcp_pcb to set the sent callback
 * @param sent callback function to call for this pcb when data is successfully sent
 */
void
tcp_sent(struct tcp_pcb *pcb, tcp_sent_fn sent)
{
  LWIP_ASSERT("invalid socket state for sent callback", pcb->state != LISTEN);
  pcb->sent = sent;
}

/**
 * Used to specify the function that should be called when a fatal error
 * has occured on the connection.
 *
 * @param pcb tcp_pcb to set the err callback
 * @param err callback function to call for this pcb when a fatal error
 *        has occured on the connection
 */
void
tcp_err(struct tcp_pcb *pcb, tcp_err_fn err)
{
  LWIP_ASSERT("invalid socket state for err callback", pcb->state != LISTEN);
  pcb->errf = err;
}

/**
 * Used for specifying the function that should be called when a
 * LISTENing connection has been connected to another host.
 *
 * @param pcb tcp_pcb to set the accept callback
 * @param accept callback function to call for this pcb when LISTENing
 *        connection has been connected to another host
 */
void
tcp_accept(struct tcp_pcb *pcb, tcp_accept_fn accept)
{
  /* This function is allowed to be called for both listen pcbs and
     connection pcbs. */
  pcb->accept = accept;
}
#endif /* LWIP_CALLBACK_API */


/**
 * Used to specify the function that should be called periodically
 * from TCP. The interval is specified in terms of the TCP coarse
 * timer interval, which is called twice a second.
 *
 */
void
tcp_poll(struct tcp_pcb *pcb, tcp_poll_fn poll, u8_t interval)
{
  LWIP_ASSERT("invalid socket state for poll", pcb->state != LISTEN);
#if LWIP_CALLBACK_API
  pcb->poll = poll;
#else /* LWIP_CALLBACK_API */
  LWIP_UNUSED_ARG(poll);
#endif /* LWIP_CALLBACK_API */
  pcb->pollinterval = interval;
}

/**
 * Purges a TCP PCB. Removes any buffered data and frees the buffer memory
 * (pcb->ooseq, pcb->unsent and pcb->unacked are freed).
 *
 * @param pcb tcp_pcb to purge. The pcb itself is not deallocated!
 */
void
tcp_pcb_purge(struct tcp_pcb *pcb)
{
  if (pcb->state != CLOSED &&
     pcb->state != TIME_WAIT &&
     pcb->state != LISTEN) {

    LWIP_DEBUGF(TCP_DEBUG, ("tcp_pcb_purge\n"));

#if TCP_LISTEN_BACKLOG
    if (pcb->state == SYN_RCVD) {
      /* Need to find the corresponding listen_pcb and decrease its accepts_pending */
      struct tcp_pcb_listen *lpcb;
      LWIP_ASSERT("tcp_pcb_purge: pcb->state == SYN_RCVD but tcp_listen_pcbs is NULL",
        tcp_listen_pcbs.listen_pcbs != NULL);
      for (lpcb = tcp_listen_pcbs.listen_pcbs; lpcb != NULL; lpcb = lpcb->next) {
        if ((lpcb->local_port == pcb->local_port) &&
            (ip_addr_isany(&lpcb->local_ip) ||
             ip_addr_cmp(&pcb->local_ip, &lpcb->local_ip))) {
            /* port and address of the listen pcb match the timed-out pcb */
            LWIP_ASSERT("tcp_pcb_purge: listen pcb does not have accepts pending",
              lpcb->accepts_pending > 0);
            lpcb->accepts_pending--;
            break;
          }
      }
    }
#endif /* TCP_LISTEN_BACKLOG */


    if (pcb->refused_data != NULL) {
      LWIP_DEBUGF(TCP_DEBUG, ("tcp_pcb_purge: data left on ->refused_data\n"));
      pbuf_free(pcb->refused_data);
      pcb->refused_data = NULL;
    }
    if (pcb->unsent != NULL) {
      LWIP_DEBUGF(TCP_DEBUG, ("tcp_pcb_purge: not all data sent\n"));
    }
    if (pcb->unacked != NULL) {
      LWIP_DEBUGF(TCP_DEBUG, ("tcp_pcb_purge: data left on ->unacked\n"));
    }
#if TCP_QUEUE_OOSEQ
    if (pcb->ooseq != NULL) {
      LWIP_DEBUGF(TCP_DEBUG, ("tcp_pcb_purge: data left on ->ooseq\n"));
    }
    tcp_segs_free(pcb->ooseq);
    pcb->ooseq = NULL;
#endif /* TCP_QUEUE_OOSEQ */

    /* Stop the retransmission timer as it will expect data on unacked
       queue if it fires */
    pcb->rtime = -1;

    tcp_segs_free(pcb->unsent);
    tcp_segs_free(pcb->unacked);
    pcb->unacked = pcb->unsent = NULL;
#if LWIP_SACK_PERF_OPT
    tcp_fr_segs_free(pcb->fr_segs);
    pcb->fr_segs = NULL;
    pcb->last_frseg = NULL;
#endif

#if LWIP_SACK
    if (pcb->sack_seq != NULL) {
      struct _sack_seq *ptr;	
      do {
        LWIP_DEBUGF(TCP_DEBUG, ("tcp_pcb_purge: Freeing sack options data\n"));

        ptr = pcb->sack_seq->next;
        mem_free(pcb->sack_seq);
        pcb->sack_seq = ptr;
      } while (pcb->sack_seq != NULL);
      pcb->sack_seq = NULL;
    }
#endif

#if TCP_OVERSIZE
    pcb->unsent_oversize = 0;
#endif /* TCP_OVERSIZE */
  }
}

/**
 * Purges the PCB and removes it from a PCB list. Any delayed ACKs are sent first.
 *
 * @param pcblist PCB list to purge.
 * @param pcb tcp_pcb to purge. The pcb itself is NOT deallocated!
 */
void
tcp_pcb_remove(struct tcp_pcb **pcblist, struct tcp_pcb *pcb)
{
  TCP_RMV(pcblist, pcb);

  tcp_pcb_purge(pcb);

  /* if there is an outstanding delayed ACKs, send it */
  if (pcb->state != TIME_WAIT &&
     pcb->state != LISTEN &&
     pcb->flags & TF_ACK_DELAY) {
    pcb->flags |= TF_ACK_NOW;
    tcp_output(pcb);
  }

  if (pcb->state != LISTEN) {
    LWIP_ASSERT("unsent segments leaking", pcb->unsent == NULL);
    LWIP_ASSERT("unacked segments leaking", pcb->unacked == NULL);
#if TCP_QUEUE_OOSEQ
    LWIP_ASSERT("ooseq segments leaking", pcb->ooseq == NULL);
#endif /* TCP_QUEUE_OOSEQ */
  }

  pcb->state = CLOSED;

  LWIP_ASSERT("tcp_pcb_remove: tcp_pcbs_sane()", tcp_pcbs_sane());
}

/**
 * Calculates a new initial sequence number for new connections.
 *
 * @return u32_t pseudo random sequence number
 */
u32_t
tcp_next_iss(void)
{
  static u32_t iss = 6510;

  iss = iss + (u32_t)(LWIP_RAND());       /* XXX */
  return iss;
}

#if TCP_CALCULATE_EFF_SEND_MSS
/**
 * Calcluates the effective send mss that can be used for a specific IP_add
 * by using ip_route to determin the netif used to send to the address and
 * calculating the minimum of TCP_MSS and that netif's mtu (if set).
 */
u16_t
tcp_eff_send_mss(u16_t sendmss, ip_addr_t *addr)
{
  u16_t mss_s;
  u16_t mss;
  struct netif *outif;

  outif = ip_route(addr, NULL);
  if ((outif != NULL) && (outif->mtu != 0)) {
    /* PC Lint Fix for:  Operator '-' followed by operator '-' is confusing.  Use parentheses.*/
    mss = (u16_t)((outif->mtu - IP_HLEN) - TCP_HLEN);
    /* MMS_S is the maximum size for a transport-layer message that TCP may send */
    mss_s = LWIP_MIN(CFG_TCP_MSS, mss);
    /* RFC 1122, chap 4.2.2.6:
     * Eff.snd.MSS = min(SendMSS+20, MMS_S) - TCPhdrsize - IPoptionsize
     * We correct for TCP options in tcp_write(), and don't support IP options.
     */
    sendmss = LWIP_MIN(sendmss, mss_s);
  }
  return sendmss;
}
#endif /* TCP_CALCULATE_EFF_SEND_MSS */

const char*
tcp_debug_state_str(enum tcp_state s)
{
  return tcp_state_str[s];
}

#if TCP_DEBUG || TCP_INPUT_DEBUG || TCP_OUTPUT_DEBUG
/**
 * Print a tcp header for debugging purposes.
 *
 * @param tcphdr pointer to a struct tcp_hdr
 */
void
tcp_debug_print(struct tcp_hdr *tcphdr)
{
  LWIP_DEBUGF(TCP_DEBUG, ("TCP header:\n"));
  LWIP_DEBUGF(TCP_DEBUG, ("+-------------------------------+\n"));
  LWIP_DEBUGF(TCP_DEBUG, ("|    %5"U16_F"      |    %5"U16_F"      | (src port, dest port)\n",
         ntohs(tcphdr->src), ntohs(tcphdr->dest)));
  LWIP_DEBUGF(TCP_DEBUG, ("+-------------------------------+\n"));
  LWIP_DEBUGF(TCP_DEBUG, ("|           %010"U32_F"          | (seq no)\n",
          ntohl(tcphdr->seqno)));
  LWIP_DEBUGF(TCP_DEBUG, ("+-------------------------------+\n"));
  LWIP_DEBUGF(TCP_DEBUG, ("|           %010"U32_F"          | (ack no)\n",
         ntohl(tcphdr->ackno)));
  LWIP_DEBUGF(TCP_DEBUG, ("+-------------------------------+\n"));
  LWIP_DEBUGF(TCP_DEBUG, ("| %2"U16_F" |   |%"U16_F"%"U16_F"%"U16_F"%"U16_F"%"U16_F"%"U16_F"|     %5"U16_F"     | (hdrlen, flags (",
       TCPH_HDRLEN(tcphdr),
         TCPH_FLAGS(tcphdr) >> 5 & 1,
         TCPH_FLAGS(tcphdr) >> 4 & 1,
         TCPH_FLAGS(tcphdr) >> 3 & 1,
         TCPH_FLAGS(tcphdr) >> 2 & 1,
         TCPH_FLAGS(tcphdr) >> 1 & 1,
         TCPH_FLAGS(tcphdr) & 1,
         ntohs(tcphdr->wnd)));
  tcp_debug_print_flags(TCPH_FLAGS(tcphdr));
  LWIP_DEBUGF(TCP_DEBUG, ("), win)\n"));
  LWIP_DEBUGF(TCP_DEBUG, ("+-------------------------------+\n"));
  LWIP_DEBUGF(TCP_DEBUG, ("|    0x%04"X16_F"     |     %5"U16_F"     | (chksum, urgp)\n",
         ntohs(tcphdr->chksum), ntohs(tcphdr->urgp)));
  LWIP_DEBUGF(TCP_DEBUG, ("+-------------------------------+\n"));
}

/**
 * Print a tcp state for debugging purposes.
 *
 * @param s enum tcp_state to print
 */
void
tcp_debug_print_state(enum tcp_state s)
{
  LWIP_DEBUGF(TCP_DEBUG, ("State: %s\n", tcp_state_str[s]));
}

/**
 * Print tcp flags for debugging purposes.
 *
 * @param flags tcp flags, all active flags are printed
 */
void
tcp_debug_print_flags(u8_t flags)
{
  if (flags & TCP_FIN) {
    LWIP_DEBUGF(TCP_DEBUG, ("FIN "));
  }
  if (flags & TCP_SYN) {
    LWIP_DEBUGF(TCP_DEBUG, ("SYN "));
  }
  if (flags & TCP_RST) {
    LWIP_DEBUGF(TCP_DEBUG, ("RST "));
  }
  if (flags & TCP_PSH) {
    LWIP_DEBUGF(TCP_DEBUG, ("PSH "));
  }
  if (flags & TCP_ACK) {
    LWIP_DEBUGF(TCP_DEBUG, ("ACK "));
  }
  if (flags & TCP_URG) {
    LWIP_DEBUGF(TCP_DEBUG, ("URG "));
  }
  if (flags & TCP_ECE) {
    LWIP_DEBUGF(TCP_DEBUG, ("ECE "));
  }
  if (flags & TCP_CWR) {
    LWIP_DEBUGF(TCP_DEBUG, ("CWR "));
  }
  LWIP_DEBUGF(TCP_DEBUG, ("\n"));
}

/**
 * Print all tcp_pcbs in every list for debugging purposes.
 */
void
tcp_debug_print_pcbs(void)
{
  struct tcp_pcb *pcb;
  LWIP_DEBUGF(TCP_DEBUG, ("Active PCB states:\n"));
  for(pcb = tcp_active_pcbs; pcb != NULL; pcb = pcb->next) {
    LWIP_DEBUGF(TCP_DEBUG, ("Local port %"U16_F", foreign port %"U16_F" snd_nxt %"U32_F" rcv_nxt %"U32_F" ",
                       pcb->local_port, pcb->remote_port,
                       pcb->snd_nxt, pcb->rcv_nxt));
    tcp_debug_print_state(pcb->state);
  }
  LWIP_DEBUGF(TCP_DEBUG, ("Listen PCB states:\n"));
  for(pcb = (struct tcp_pcb *)tcp_listen_pcbs.pcbs; pcb != NULL; pcb = pcb->next) {
    LWIP_DEBUGF(TCP_DEBUG, ("Local port %"U16_F", foreign port %"U16_F" snd_nxt %"U32_F" rcv_nxt %"U32_F" ",
                       pcb->local_port, pcb->remote_port,
                       pcb->snd_nxt, pcb->rcv_nxt));
    tcp_debug_print_state(pcb->state);
  }
  LWIP_DEBUGF(TCP_DEBUG, ("TIME-WAIT PCB states:\n"));
  for(pcb = tcp_tw_pcbs; pcb != NULL; pcb = pcb->next) {
    LWIP_DEBUGF(TCP_DEBUG, ("Local port %"U16_F", foreign port %"U16_F" snd_nxt %"U32_F" rcv_nxt %"U32_F" ",
                       pcb->local_port, pcb->remote_port,
                       pcb->snd_nxt, pcb->rcv_nxt));
    tcp_debug_print_state(pcb->state);
  }
}

/**
 * Check state consistency of the tcp_pcb lists.
 */
s16_t
tcp_pcbs_sane(void)
{
  struct tcp_pcb *pcb;
  for(pcb = tcp_active_pcbs; pcb != NULL; pcb = pcb->next) {
    LWIP_ASSERT("tcp_pcbs_sane: active pcb->state != CLOSED", pcb->state != CLOSED);
    LWIP_ASSERT("tcp_pcbs_sane: active pcb->state != LISTEN", pcb->state != LISTEN);
    LWIP_ASSERT("tcp_pcbs_sane: active pcb->state != TIME-WAIT", pcb->state != TIME_WAIT);
  }
  for(pcb = tcp_tw_pcbs; pcb != NULL; pcb = pcb->next) {
    LWIP_ASSERT("tcp_pcbs_sane: tw pcb->state == TIME-WAIT", pcb->state == TIME_WAIT);
  }
  return 1;
}
#endif /* TCP_DEBUG */


/**
 * This is to embed the version string in TCP library.
 *
 */

char * lwip_version(void)
{
  return LWIP_VERSION_STRING;
}


/* NETIF DRIVER STATUS BEGIN */
#if DRIVER_STATUS_CHECK
void update_drv_status_to_pcbs(struct tcp_pcb *pcb_list, struct netif *netif, u8_t status)
{
  struct tcp_pcb *pcb;
  for(pcb = pcb_list; pcb != NULL; pcb = pcb->next) {
    /* network mask matches? */
    if (netif_is_up(netif)) {
      if (ip_addr_netcmp(&(pcb->remote_ip), &(netif->ip_addr), &(netif->netmask))) {
        pcb->drv_status = status;
      }
    }
  }
}


void tcpip_upd_status_to_tcp_pcbs(struct netif *netif, u8_t status)
{
  LWIP_ERROR("netif_set_driver_ready: invalid arguments",(netif != NULL), return;);

  update_drv_status_to_pcbs(tcp_active_pcbs, netif, status);
  update_drv_status_to_pcbs(tcp_tw_pcbs, netif, status);

  return;
}

void tcp_ip_flush_pcblist_on_wake_queue(struct netif *netif, struct tcp_pcb * pcb_list, u8_t status)
{
  struct tcp_pcb *pcb;
  /* iterate all PCB for that netif, and if any thing is down, change status and mark status UP */
  for(pcb = pcb_list; pcb != NULL; pcb = pcb->next) {
    if (ip_addr_netcmp(&(pcb->remote_ip), &(netif->ip_addr), &(netif->netmask))) {
       tcp_flush_pcb_on_wake_queue(pcb, status);

       /* Reset the RTO timer if its running */
       if(pcb->rtime > 0) {
         pcb->rtime = 0;
       }
    }
  }
}

void tcp_ip_event_sendplus_on_wake_queue (struct netif *netif)
{
  struct tcp_pcb *pcb;

  for(pcb = tcp_active_pcbs; pcb != NULL; pcb = pcb->next) {
    if (ip_addr_netcmp(&(pcb->remote_ip), &(netif->ip_addr), &(netif->netmask))) {
      if(pcb->sndplus && pcb->callback_arg) {
        pcb->sndplus(pcb->callback_arg, pcb);
      }
    }
  }
}

void tcpip_flush_on_wake_queue(struct netif *netif, u8_t status)
{
  tcp_ip_flush_pcblist_on_wake_queue(netif, tcp_active_pcbs, status);
  tcp_ip_flush_pcblist_on_wake_queue(netif, tcp_tw_pcbs, status);
}
#endif
/* NETIF DRIVER STATUS END */

#endif /* LWIP_TCP */
