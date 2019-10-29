/**
 * @file
 * TCP Window Scale module 
 *
 */
 
/**********************************************************************************
 * Copyright (c) <2013-2015>, <Huawei Technologies Co., Ltd>
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice, this
 * list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 * this list of conditions and the following disclaimer in the documentation and/or
 * other materials provided with the distribution.
 * 3. Neither the name of the copyright holder nor the names of its contributors
 * may be used to endorse or promote products derived from this software without
 * specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 * IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT,
 * INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY
 * OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 **********************************************************************************/

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

#if LWIP_WND_SCALE
#include "lwip/tcp_int.h"
#include "lwip/tcp_impl_int.h"
#include "lwip/def.h"
#include "lwip/pbuf.h"
#include "lwip/tcp_config.h"




/**
 * This parses the window scaling option which received in each packet
 *
 * @param pcb tcp control block of the connection
 * @param snd_scale shift count received from peer
 * @param tcp_rcv_scale shift count configured in TCP_RCV_SCALE
 *
 */

void tcp_parseopt_ws(struct tcp_pcb *pcb, u8_t snd_scale, u8_t tcp_rcv_scale)
{
  /* An WND_SCALE option with the right option length. */
  pcb->snd_scale = snd_scale;

  if (pcb->snd_scale > 14U) {
    pcb->snd_scale = 14U;
  }

  pcb->rcv_scale = tcp_rcv_scale;
  pcb->flags |= TF_WND_SCALE;

  if (pcb->state == SYN_SENT) {
    /* TCP client receives window scaling option in SYNACK, need to update window to max window */
    pcb->tcpwnd = ACTUAL_WND_SIZE(pcb, CFG_TCP_WND);
    pcb->rcv_wnd = ACTUAL_WND_SIZE(pcb, CFG_TCP_WND);
    pcb->rcv_ann_wnd = ACTUAL_WND_SIZE(pcb, CFG_TCP_WND);
  }
}

/**
 * This enables the Window scale flag in seg option flags.
 * This is done for SYN and SYNACK packets
 * So that tcp_ouput_segment() will add window scale option
 *
 * @param pcb tcp control block of the connection
 * @param snd_scale shift count received from peer
 * @param tcp_rcv_scale shift count configured in TCP_RCV_SCALE
 *
 */

void tcp_enqueue_flags_ws(struct tcp_pcb *pcb, u8_t *optflags)
{
  /* Windscale option needs to send in below two cases */
  /* 1) while sending SYNACK - WS flag is enabled in pcb->flags if WS option is received from peer(client) in SYN */
  /* 2) while sending SYN - this function is called from tcp_enqueue_flags() only for sending SYN or SYNACK */
  /* if pcb->state is not SYN_RCVD then we are sending SYN. */
  if ((pcb->state != SYN_RCVD) || (pcb->flags & TF_WND_SCALE)) {
    (*optflags) |= TF_SEG_OPTS_WND_SCALE;
  }
}


/**
 * Build a window scale option (3 bytes long) at the specified options pointer)
 *
 * @param opts option pointer where to store the window scale option
 * @param tcp_rcv_scale shift count configured in TCP_RCV_SCALE
 */
void tcp_build_wnd_scale_option(u32_t *opts, u32_t tcp_rcv_scale)
{
  /* Pad with one NOP option to make everything nicely aligned */
  opts[0] = PP_HTONL(0x01030300 | tcp_rcv_scale);
}

/**
 * This is to initialize Window scale related variables in tcp_pcb
 *
 * @param pcb tcp control block of the current connection
 *
 * @note This is called in tcp_alloc() and tcp_connect()
 */

void tcp_init_ws(struct tcp_pcb *pcb)
{
  /* snd_scale and rcv_scale are zero unless both sides agree to use scaling */
  pcb->snd_scale = 0;
  pcb->rcv_scale = 0;

  pcb->rcv_wnd = CFG_TCP_WND_MIN;
  pcb->rcv_ann_wnd = CFG_TCP_WND_MIN;
  pcb->tcpwnd = CFG_TCP_WND_MIN;
}
#endif

