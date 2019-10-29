/**
 * @file
 * Sequential API Main thread module
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
#include "lwip/tcp_config.h"

#if !NO_SYS /* don't build if not configured for use in lwipopts.h */

#include "lwip/sys.h"
#include "lwip/memp.h"
#include "lwip/mem.h"
#include "lwip/pbuf.h"
#include "lwip/tcpip.h"
#include "lwip/init.h"
#include "netif/etharp.h"
#if PPPOE_SUPPORT
#include "netif/ppp_oe.h"
#endif

/* global variables */
static tcpip_init_done_fn tcpip_init_done;
static void *tcpip_init_done_arg;
static sys_dual_mbox_t dmbox;

#if LWIP_TCPIP_CORE_LOCKING
/** The global semaphore to lock the stack. */
sys_mutex_t lock_tcpip_core;
#endif /* LWIP_TCPIP_CORE_LOCKING */

/** The global flag to indicate the stack have init. */
int tcpip_init_finish = 0;


/**
 * The main lwIP thread. This thread has exclusive access to lwIP core functions
 * (unless access to them is not locked). Other threads communicate with this
 * thread using message boxes.
 *
 * It also starts all the timers to make sure they are running in the right
 * thread context.
 *
 * @param arg unused argument
 */
LWIP_STATIC void
tcpip_thread(void *arg)
{
  struct tcpip_msg *msg;
  /* NETIF DRIVER STATUS BEGIN */
  /* NETIF DRIVER STATUS END */
  LWIP_UNUSED_ARG(arg);

  if (tcpip_init_done != NULL) {
    (*tcpip_init_done)(tcpip_init_done_arg);
  }

  LOCK_TCPIP_CORE();
  /* PC Lint fix for: CDATA[while(1) ... ] */
  for(;;) {                          /* MAIN Loop */
    UNLOCK_TCPIP_CORE();
    LWIP_TCPIP_THREAD_ALIVE();
    /* NETIF DRIVER STATUS BEGIN */
    msg = NULL;

    sys_timeouts_mbox_fetch(&dmbox, (void **)&msg);
    LOCK_TCPIP_CORE();
    /* NETIF DRIVER STATUS END */

    switch (msg->type) {
#if LWIP_NETCONN
    case TCPIP_MSG_API:
      LWIP_DEBUGF(TCPIP_DEBUG, ("tcpip_thread: API message %p\n", (void *)msg));
      msg->msg.apimsg->function(&(msg->msg.apimsg->msg));
      break;
#endif /* LWIP_NETCONN */

#if !LWIP_TCPIP_CORE_LOCKING_INPUT
    case TCPIP_MSG_INPKT:
      LWIP_DEBUGF(TCPIP_DEBUG, ("tcpip_thread: PACKET %p\n", (void *)msg));
#if LWIP_ETHERNET
      if (msg->msg.inp.netif->flags & (NETIF_FLAG_ETHARP | NETIF_FLAG_ETHERNET)) {
        ethernet_input(msg->msg.inp.p, msg->msg.inp.netif);
      } else
#endif /* LWIP_ETHERNET */
      {
        ip_input(msg->msg.inp.p, msg->msg.inp.netif);
      }
      memp_free(MEMP_TCPIP_MSG_INPKT, msg);
      break;
#endif /* LWIP_TCPIP_CORE_LOCKING_INPUT */

#if LWIP_NETIF_API
    case TCPIP_MSG_NETIFAPI:
      LWIP_DEBUGF(TCPIP_DEBUG, ("tcpip_thread: Netif API message %p\n", (void *)msg));
      msg->msg.netifapimsg->function(&(msg->msg.netifapimsg->msg));
      break;
#endif /* LWIP_NETIF_API */

#if LWIP_TCPIP_TIMEOUT
    case TCPIP_MSG_TIMEOUT:
      LWIP_DEBUGF(TCPIP_DEBUG, ("tcpip_thread: TIMEOUT %p\n", (void *)msg));
      sys_timeout(msg->msg.tmo.msecs, msg->msg.tmo.h, msg->msg.tmo.arg);
      memp_free(MEMP_TCPIP_MSG_API, msg);
      break;
    case TCPIP_MSG_UNTIMEOUT:
      LWIP_DEBUGF(TCPIP_DEBUG, ("tcpip_thread: UNTIMEOUT %p\n", (void *)msg));
      sys_untimeout(msg->msg.tmo.h, msg->msg.tmo.arg);
      memp_free(MEMP_TCPIP_MSG_API, msg);
      break;
#endif /* LWIP_TCPIP_TIMEOUT */

    case TCPIP_MSG_CALLBACK:
      LWIP_DEBUGF(TCPIP_DEBUG, ("tcpip_thread: CALLBACK %p\n", (void *)msg));
      msg->msg.cb.function(msg->msg.cb.ctx);
      memp_free(MEMP_TCPIP_MSG_API, msg);
      break;

#if LWIP_TCPIP_CALLBACKMSG
    case TCPIP_MSG_CALLBACK_STATIC:
      LWIP_DEBUGF(TCPIP_DEBUG, ("tcpip_thread: CALLBACK_STATIC %p\n", (void *)msg));
      msg->msg.cb.function(msg->msg.cb.ctx);
      break;
#endif

    default:
      LWIP_DEBUGF(TCPIP_DEBUG, ("tcpip_thread: invalid message: %d\n", msg->type));
      LWIP_ASSERT("tcpip_thread: invalid message", 0);
      break;
    }
  }
}

/**
 * Pass a received packet to tcpip_thread for input processing
 *
 * @param p the received packet, p->payload pointing to the Ethernet header or
 *          to an IP header (if inp doesn't have NETIF_FLAG_ETHARP or
 *          NETIF_FLAG_ETHERNET flags)
 * @param inp the network interface on which the packet was received
 */
err_t
tcpip_input(struct pbuf *p, struct netif *inp)
{
#if LWIP_TCPIP_CORE_LOCKING_INPUT
  err_t ret;
  LWIP_DEBUGF(TCPIP_DEBUG, ("tcpip_input: PACKET %p/%p\n", (void *)p, (void *)inp));
  LOCK_TCPIP_CORE();
#if LWIP_ETHERNET
  if (inp->flags & (NETIF_FLAG_ETHARP | NETIF_FLAG_ETHERNET)) {
    ret = ethernet_input(p, inp);
  } else
#endif /* LWIP_ETHERNET */
  {
    ret = ip_input(p, inp);
  }
  UNLOCK_TCPIP_CORE();
  return ret;
#else /* LWIP_TCPIP_CORE_LOCKING_INPUT */
  struct tcpip_msg *msg;
#if PF_PKT_SUPPORT
  struct eth_hdr* ethhdr;
#if LWIP_NETIF_PROMISC
  u8_t is_otherhost = lwIP_FALSE;
#endif
#endif

  if (!sys_dual_mbox_valid(&dmbox)) {
    return ERR_VAL;
  }
  msg = (struct tcpip_msg *)memp_malloc(MEMP_TCPIP_MSG_INPKT);
  if (msg == NULL) {
    return ERR_MEM;
  }

#if PF_PKT_SUPPORT
  ethhdr = (struct eth_hdr *)p->payload;
  if (ethhdr->dest.addr[0] & 1) {
    /* this might be a multicast or brcast packet */
    if (ethhdr->dest.addr[0] == LL_MULTICAST_ADDR_0) {
      if ((ethhdr->dest.addr[1] == LL_MULTICAST_ADDR_1) &&
          (ethhdr->dest.addr[2] == LL_MULTICAST_ADDR_2)) {
        /* mark the pbuf as link-layer multicast */
        p->flags |= PBUF_FLAG_LLMCAST;
      }
    } else if (eth_addr_cmp(&ethhdr->dest, &ethbroadcast)) {
      /* mark the pbuf as link-layer brcast */
      p->flags |= PBUF_FLAG_LLBCAST;
    }
  }
  else if (eth_addr_cmp(&ethhdr->dest, (struct eth_addr*)inp->hwaddr)) {
     p->flags |= PBUF_FLAG_HOST;
  } /* else will be treates as other host */
#if LWIP_NETIF_PROMISC
  else {
    is_otherhost = lwIP_TRUE;
  }

  /* If PROMISC mode disable, drop the packet for other host no need to process further */
  if(is_otherhost && !(atomic_read(&inp->flags_ext) == NETIF_FLAG_PROMISC)) {
    /* silently ignore this packet: not for us*/
      pbuf_free(p);
      memp_free(MEMP_TCPIP_MSG_INPKT, msg);
      return ERR_OK;
  }
#endif  /* LWIP_NETIF_PROMISC */
#endif /* PF_PKT_SUPPORT */


  msg->type = TCPIP_MSG_INPKT;
  msg->msg.inp.p = p;
  msg->msg.inp.netif = inp;
  if (sys_dual_mbox_trypost(&dmbox, msg) != ERR_OK) {
    memp_free(MEMP_TCPIP_MSG_INPKT, msg);
    return ERR_MEM;
  }
  return ERR_OK;
#endif /* LWIP_TCPIP_CORE_LOCKING_INPUT */
}

/**
 * Call a specific function in the thread context of
 * tcpip_thread for easy access synchronization.
 * A function called in that way may access lwIP core code
 * without fearing concurrent access.
 *
 * @param f the function to call
 * @param ctx parameter passed to f
 * @param block 1 to block until the request is posted, 0 to non-blocking mode
 * @return ERR_OK if the function was called, another err_t if not
 */
err_t
tcpip_callback_with_block(tcpip_callback_fn function, void *ctx, u8_t block)
{
  struct tcpip_msg *msg;

  if(!tcpip_init_finish)
    return ERR_IF;

  if (sys_dual_mbox_valid(&dmbox)) {
    msg = (struct tcpip_msg *)memp_malloc(MEMP_TCPIP_MSG_API);
    if (msg == NULL) {
      return ERR_MEM;
    }

    msg->type = TCPIP_MSG_CALLBACK;
    msg->msg.cb.function = function;
    msg->msg.cb.ctx = ctx;
    if (block) {
      sys_dual_mbox_post(&dmbox, msg);
    } else {
      if (sys_dual_mbox_trypost(&dmbox, msg) != ERR_OK) {
        memp_free(MEMP_TCPIP_MSG_API, msg);
        return ERR_MEM;
      }
    }
    return ERR_OK;
  }
  return ERR_VAL;
}



#if LWIP_NETCONN
/**
 * Call the lower part of a netconn_* function
 * This function is then running in the thread context
 * of tcpip_thread and has exclusive access to lwIP core code.
 *
 * @param apimsg a struct containing the function to call and its parameters
 * @param sem semaphore that need to be used for this apimsg event.
 * @return ERR_OK if the function was called, another err_t if not
 */
err_t
tcpip_apimsg(struct api_msg *apimsg, sys_sem_t *sem)
{
  struct tcpip_msg msg;
#ifdef LWIP_DEBUG
  /* catch functions that don't set err */
  apimsg->msg.err = ERR_VAL;
#endif
  if (sys_dual_mbox_valid(&dmbox)){
    msg.type = TCPIP_MSG_API;
    msg.msg.apimsg = apimsg;
    sys_dual_mbox_post(&dmbox, &msg);
    sys_arch_sem_wait(sem, 0);
    return apimsg->msg.err;
  }
  return ERR_VAL;
}

#if LWIP_TCPIP_CORE_LOCKING
/**
 * Call the lower part of a netconn_* function
 * This function has exclusive access to lwIP core code by locking it
 * before the function is called.
 *
 * @param apimsg a struct containing the function to call and its parameters
 * @return ERR_OK (only for compatibility fo tcpip_apimsg())
 */
err_t
tcpip_apimsg_lock(struct api_msg *apimsg)
{
#ifdef LWIP_DEBUG
  /* catch functions that don't set err */
  apimsg->msg.err = ERR_VAL;
#endif

  LOCK_TCPIP_CORE();
  apimsg->function(&(apimsg->msg));
  UNLOCK_TCPIP_CORE();
  return apimsg->msg.err;

}
#endif /* LWIP_TCPIP_CORE_LOCKING */
#endif /* LWIP_NETCONN */

#if LWIP_NETIF_API
#if !LWIP_TCPIP_CORE_LOCKING
/**
 * Much like tcpip_apimsg, but calls the lower part of a netifapi_*
 * function.
 *
 * @param netifapimsg a struct containing the function to call and its parameters
 * @return error code given back by the function that was called
 */
err_t
tcpip_netifapi(struct netifapi_msg* netifapimsg)
{
  struct tcpip_msg msg;
  if (sys_dual_mbox_valid(&dmbox)) {
    err_t err = sys_sem_new(&netifapimsg->msg.sem, 0);
    if (err != ERR_OK) {
      netifapimsg->msg.err = err;
      return err;
    }

    msg.type = TCPIP_MSG_NETIFAPI;
    msg.msg.netifapimsg = netifapimsg;
    sys_dual_mbox_post(&dmbox, &msg);
    sys_sem_wait(&netifapimsg->msg.sem);
    sys_sem_free(&netifapimsg->msg.sem);
    return netifapimsg->msg.err;
  }
  return ERR_VAL;
}

/* NETIF DRIVER STATUS BEGIN */
err_t
tcpip_netifapi_priority(struct netifapi_msg* netifapimsg)
{
  struct tcpip_msg msg;
  if (sys_dual_mbox_valid(&dmbox)) {
    err_t err = sys_sem_new(&netifapimsg->msg.sem, 0);
    if (err != ERR_OK) {
      netifapimsg->msg.err = err;
      return err;
    }

    msg.type = TCPIP_MSG_NETIFAPI;
    msg.msg.netifapimsg = netifapimsg;

    /* Post original mesasge to priority queue*/
    sys_dual_mbox_post_priority(&dmbox, &msg);


    sys_sem_wait(&netifapimsg->msg.sem);
    sys_sem_free(&netifapimsg->msg.sem);
    return netifapimsg->msg.err;
  }
  return ERR_VAL;
}
/* NETIF DRIVER STATUS END */
#else /* !LWIP_TCPIP_CORE_LOCKING */
/**
 * Call the lower part of a netifapi_* function
 * This function has exclusive access to lwIP core code by locking it
 * before the function is called.
 *
 * @param netifapimsg a struct containing the function to call and its parameters
 * @return ERR_OK (only for compatibility fo tcpip_netifapi())
 */
err_t
tcpip_netifapi_lock(struct netifapi_msg* netifapimsg)
{
  LOCK_TCPIP_CORE();
  netifapimsg->function(&(netifapimsg->msg));
  UNLOCK_TCPIP_CORE();
  return netifapimsg->msg.err;
}
#endif /* !LWIP_TCPIP_CORE_LOCKING */
#endif /* LWIP_NETIF_API */


#if LWIP_TCPIP_CALLBACKMSG
/**
 * Allocate a structure for a static callback message and initialize it.
 * This is intended to be used to send "static" messages from interrupt context.
 *
 * @param function the function to call
 * @param ctx parameter passed to function
 * @return a struct pointer to pass to tcpip_trycallback().
 */
struct tcpip_callback_msg* tcpip_callbackmsg_new(tcpip_callback_fn function, void *ctx)
{
  struct tcpip_msg *msg = (struct tcpip_msg *)memp_malloc(MEMP_TCPIP_MSG_API);
  if (msg == NULL) {
    return NULL;
  }
  msg->type = TCPIP_MSG_CALLBACK_STATIC;
  msg->msg.cb.function = function;
  msg->msg.cb.ctx = ctx;
  return (struct tcpip_callback_msg*)msg;
}

/**
 * Free a callback message allocated by tcpip_callbackmsg_new().
 *
 * @param msg the message to free
 */
void tcpip_callbackmsg_delete(struct tcpip_callback_msg* msg)
{
  memp_free(MEMP_TCPIP_MSG_API, msg);
}

/**
 * Try to post a callback-message to the tcpip_thread mbox
 * This is intended to be used to send "static" messages from interrupt context.
 *
 * @param msg pointer to the message to post
 * @return sys_mbox_trypost() return code
 */
err_t
tcpip_trycallback(struct tcpip_callback_msg* msg)
{
  if (!sys_dual_mbox_valid(&dmbox)) {
    return ERR_VAL;
  }
  return sys_dual_mbox_trypost(&dmbox, msg);
}
#endif /*LWIP_TCPIP_CALLBACKMSG*/

/**
 * Initialize this module:
 * - initialize all sub modules
 * - start the tcpip_thread
 *
 * @param initfunc a function to call when tcpip_thread is running and finished initializing
 * @param arg argument to pass to initfunc
 */
void
tcpip_init(tcpip_init_done_fn initfunc, void *arg)
{
  if(tcpip_init_finish)
    return;

#ifdef LWIP_CONFIG_PARAM
  if (LWIP_COINFIG_TCP_TTL() != ERR_OK)
  {
    return;
  }
  if (LWIP_CONFIG_TCP_MAXRTX() != ERR_OK)
  {
    return;
  }
  if (LWIP_CONFIG_TCP_SYNMAXRTX() != ERR_OK)
  {
    return;
  }
#if LWIP_WND_SCALE
  if (LWIP_CONFIG_TCP_RCV_SCALE() != ERR_OK)
  {
    return;
  }
  if (LWIP_CONFIG_TCP_WND_MIN() != ERR_OK)
  {
    return;
  }
#endif
  if (LWIP_CONFIG_TCP_WND() !=ERR_OK)
  {
    return;
  }
  if(LWIP_CONFIG_TCP_MSS() != ERR_OK)
  {
    return;
  }
  if (LWIP_CONFIG_TCP_SND_BUF() != ERR_OK)
  {
    return;
  }
  if (LWIP_CONFIG_INITIAL_CWD() != ERR_OK)
  {
    return;
  }
  if(LWIP_CONFIG_INITIAL_SSTHRESH() != ERR_OK)
  {
    return;
  }
#endif

  if (lwip_init() != 0) {
    return;
  }

  tcpip_init_done = initfunc;
  tcpip_init_done_arg = arg;
  if(sys_dual_mbox_new(&dmbox, TCPIP_MBOX_SIZE) != ERR_OK) {
    LWIP_ASSERT("failed to create tcpip_thread mbox", 0);
  }

#if LWIP_TCPIP_CORE_LOCKING
  if(sys_mutex_new(&lock_tcpip_core) != ERR_OK) {
    LWIP_ASSERT("failed to create lock_tcpip_core", 0);
  }
#endif /* LWIP_TCPIP_CORE_LOCKING */

  sys_thread_new(TCPIP_THREAD_NAME, tcpip_thread, NULL, TCPIP_THREAD_STACKSIZE, TCPIP_THREAD_PRIO);

  tcpip_init_finish = 1;
}


#endif /* !NO_SYS */
