/**
 * @file
 * Network buffer management
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

#include "lwip/netbuf.h"
#include "lwip/memp.h"

#include <string.h>


/**
 * Deallocate a netbuf allocated
 *
 * @param buf pointer to a netbuf allocated
 */
void
netbuf_delete(struct netbuf *buf)
{
  if (buf != NULL) {
    if (buf->p != NULL) {
      pbuf_free(buf->p);
      buf->p = buf->ptr = NULL;
    }
    memp_free(MEMP_NETBUF, buf);
  }
}

/**
 * Allocate memory for a packet buffer for a given netbuf.
 *
 * @param buf the netbuf for which to allocate a packet buffer
 * @param size the size of the packet buffer to allocate
 * @return pointer to the allocated memory
 *         NULL if no memory could be allocated
 */
void *
netbuf_alloc(struct netbuf *buf, u16_t size)
{
  LWIP_ERROR("netbuf_alloc: invalid buf", (buf != NULL), return NULL;);

  /* Deallocate any previously allocated memory. */
  if (buf->p != NULL) {
    pbuf_free(buf->p);
  }
  buf->p = pbuf_alloc(PBUF_TRANSPORT, size, PBUF_RAM);
  if (buf->p == NULL) {
    buf->ptr = NULL;
    return NULL;
  }
  LWIP_ASSERT("check that first pbuf can hold size",
             (buf->p->len >= size));
  buf->ptr = buf->p;
  return buf->p->payload;
}


/**
 * Free the packet buffer included in a netbuf
 *
 * @param buf pointer to the netbuf which contains the packet buffer to free
 */
void
netbuf_free(struct netbuf *buf)
{
  LWIP_ERROR("netbuf_free: invalid buf", (buf != NULL), return;);
  if (buf->p != NULL) {
    pbuf_free(buf->p);
  }
  buf->p = buf->ptr = NULL;
}

/**
 * Let a netbuf reference existing (non-volatile) data.
 *
 * @param buf netbuf which should reference the data
 * @param dataptr pointer to the data to reference
 * @param size size of the data
 * @return ERR_OK if data is referenced
 *         ERR_MEM if data couldn't be referenced due to lack of memory
 */
err_t
netbuf_ref(struct netbuf *buf, const void *dataptr, u16_t size)
{
  LWIP_ERROR("netbuf_ref: invalid buf", (buf != NULL), return ERR_ARG;);
  if (buf->p != NULL) {
    pbuf_free(buf->p);
  }
#if (MEM_MALLOC_DMA_ALIGN == 1)
  buf->p = pbuf_alloc(PBUF_TRANSPORT, 0, PBUF_REF);
#else
  buf->p = pbuf_alloc(PBUF_RAW, size, PBUF_RAM);
#endif
  if (buf->p == NULL) {
    buf->ptr = NULL;
    return ERR_MEM;
  }

#if (MEM_MALLOC_DMA_ALIGN == 1)
  buf->p->payload = (void*)dataptr;
  buf->p->len = buf->p->tot_len = size;
#else
  (void)MEMCPY_S(buf->p->payload, buf->p->len, dataptr, size);
#endif
  buf->ptr = buf->p;
  return ERR_OK;
}

#endif /* LWIP_NETCONN */
