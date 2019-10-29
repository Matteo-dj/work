/* Copyright (C) 2012 mbed.org, MIT License
 * Copyright (c) <2013-2015>, <Huawei Technologies Co., Ltd>
 * All rights reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of this software
 * and associated documentation files (the "Software"), to deal in the Software without restriction,
 * including without limitation the rights to use, copy, modify, merge, publish, distribute,
 * sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all copies or
 * substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING
 * BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM,
 * DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
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

/* lwIP includes. */
#include "lwip/opt.h"
#include "lwip/debug.h"
#include "lwip/def.h"
#include "lwip/sys.h"
#include "lwip/mem.h"
#include "pthread.h"
#include "lwip/stats.h"

#include "los_config.h"
#include "arch/sys_arch.h"
#include "los_sys.ph"
#include "los_sem.ph"
#include "los_mux.ph"
#include "string.h"

#if !LWIP_LITEOS_COMPAT
static pthread_mutex_t lwprot_mutex = PTHREAD_MUTEX_INITIALIZER;
static pthread_t lwprot_thread = (pthread_t)0xDEAD;
static int lwprot_count = 0;
#endif
/*-----------------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*
 * Routine:  sys_mbox_new
 *---------------------------------------------------------------------------*
 * Description:
 *      Creates a new mailbox
 * Inputs:
 *      sys_mbox_t mbox         -- Handle of mailbox
 *      int queue_sz            -- Size of elements in the mailbox
 * Outputs:
 *      err_t                   -- ERR_OK if message posted, else ERR_MEM
 *---------------------------------------------------------------------------*/

err_t
sys_mbox_new(struct sys_mbox **mb, int size, unsigned char is_auto_expand)
{
  struct sys_mbox *mbox;

  if(size <= 0) {
    LWIP_DEBUGF(SYS_DEBUG, ("sys_mbox_new: mbox size must bigger than 0\n"));
    return ERR_MEM;
  }

  mbox = (struct sys_mbox *)mem_malloc(sizeof(struct sys_mbox));
  if (mbox == NULL) {
    goto err_handler;
  }

  (void)MEMSET_S(mbox, sizeof(struct sys_mbox), 0, sizeof(struct sys_mbox)); //CSEC_FIX_2302

  mbox->msgs = (void **)mem_malloc(sizeof(void *) * size);
  if (mbox->msgs == NULL) {
    goto err_handler;
  }

  (void)MEMSET_S(mbox->msgs, (sizeof(void *) * size), 0, (sizeof(void*) * size)); //CSEC_FIX_2302

  mbox->mbox_size = size;

  mbox->first = 0;
  mbox->last = 0;
  mbox->isFull = 0;
  mbox->isEmpty = 1;
  mbox->isAutoExpand = is_auto_expand;

  if (sys_sem_new(&(mbox->not_empty), 0) != ERR_OK) {
    goto err_handler;
  }

  if (sys_sem_new(&(mbox->not_full), 0) != ERR_OK) {
    sys_sem_free(&(mbox->not_empty));
    goto err_handler;
  }
  
  if (sys_sem_new(&(mbox->mutex), 1) != ERR_OK) {
    sys_sem_free(&(mbox->not_empty));
    sys_sem_free(&(mbox->not_full));
    goto err_handler;
  }

  SYS_STATS_INC_USED(mbox);
  *mb = mbox;
  LWIP_DEBUGF(SYS_DEBUG, ("sys_mbox_new: mbox created successfully 0x%p\n", (void *)mbox));
  return ERR_OK;

err_handler:
  if (mbox != NULL) {
    if (mbox->msgs != NULL) {
      mem_free(mbox->msgs);
      mbox->msgs = NULL;
    }
    mem_free(mbox);
  }
  return ERR_MEM;
}
/*-----------------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*
 * Routine:  sys_dual_mbox_new
 *---------------------------------------------------------------------------*
 * Description:
 *      Creates a new mailbox
 * Inputs:
 *      sys_dual_mbox_t mbox         -- Handle of mailbox
 *      int queue_sz            -- Size of elements in the mailbox
 * Outputs:
 *      err_t                   -- ERR_OK if message posted, else ERR_MEM
 *---------------------------------------------------------------------------*/

err_t
sys_dual_mbox_new(struct sys_dual_mbox **dmb, int size)
{
  struct sys_dual_mbox *dmbox;

  if(size <= 0) {
    LWIP_DEBUGF(SYS_DEBUG, ("sys_dual_mbox_new: dmbox size must bigger than 0\n"));
    return ERR_MEM;
  }

  dmbox = (struct sys_dual_mbox *)mem_malloc(sizeof(struct sys_dual_mbox));
  if (dmbox == NULL) {
    goto err_handler;
  }

  (void)MEMSET_S(dmbox, sizeof(struct sys_dual_mbox), 0, sizeof(struct sys_dual_mbox)); //CSEC_FIX_2302


  dmbox->msgs = (void **)mem_malloc(sizeof(void *) * size);
  if (dmbox->msgs == NULL) {
    goto err_handler;
  }

  (void)MEMSET_S(dmbox->msgs, (sizeof(void *) * size), 0, (sizeof(void*) * size)); //CSEC_FIX_2302

  dmbox->mbox_size = size;

  dmbox->first = 0;
  dmbox->last = 0;
  dmbox->isFull = 0;
  dmbox->isEmpty = 1;

  dmbox->msgs_priority = (void **)mem_malloc(sizeof(void *) * TCPIP_PRTY_MBOX_SIZE);
  if (dmbox->msgs_priority == NULL) {
      goto err_handler;
  }

  (void)MEMSET_S(dmbox->msgs_priority, (sizeof(void *) * TCPIP_PRTY_MBOX_SIZE), 0, (sizeof(void*) * TCPIP_PRTY_MBOX_SIZE)); //CSEC_FIX_2302
  dmbox->mbox_size_priority = TCPIP_PRTY_MBOX_SIZE;

  dmbox->mbox_used = 0;
  dmbox->mbox_used_priority = 0;
  dmbox->mbox_total_used = 0;
  dmbox->first_p = 0;
  dmbox->last_p = 0;

  if (sys_sem_new(&(dmbox->not_empty), 0) != ERR_OK) {
    goto err_handler;
  }

  if (sys_sem_new(&(dmbox->not_full), 0) != ERR_OK) {
    sys_sem_free(&(dmbox->not_empty));
    goto err_handler;
  }
  
  if (sys_sem_new(&(dmbox->mutex), 1) != ERR_OK) {
    sys_sem_free(&(dmbox->not_empty));
    sys_sem_free(&(dmbox->not_full));
    goto err_handler;
  }

  SYS_STATS_INC_USED(dmbox);
  *dmb = dmbox;
  LWIP_DEBUGF(SYS_DEBUG, ("sys_dual_mbox_new: dmbox created successfully 0x%p\n", (void *)dmbox));
  return ERR_OK;

err_handler:
  if (dmbox != NULL) {

    if(dmbox->msgs != NULL) {
      mem_free(dmbox->msgs);
      dmbox->msgs = NULL;
    }

    if (dmbox->msgs_priority != NULL) {
      mem_free(dmbox->msgs_priority);
      dmbox->msgs_priority = NULL;
    }
    mem_free(dmbox);
  }
  return ERR_MEM;
}
/*-----------------------------------------------------------------------------------*/
void
sys_mbox_free(struct sys_mbox **mb)
{
  if ((mb != NULL) && (*mb != SYS_MBOX_NULL)) {
    struct sys_mbox *mbox = *mb;
    SYS_STATS_DEC(mbox.used);

    LWIP_DEBUGF(SYS_DEBUG, ("sys_mbox_free: going to free mbox 0x%p\n", (void *)mbox));

    sys_arch_sem_wait(&(mbox->mutex), 0);

    sys_sem_free(&(mbox->not_empty));
    sys_sem_free(&(mbox->not_full));
    sys_sem_free(&(mbox->mutex));
    mem_free(mbox->msgs);
    mbox->msgs = NULL;
    mem_free(mbox);
    *mb = NULL;

    LWIP_DEBUGF(SYS_DEBUG, ("sys_mbox_free: freed mbox\n"));
  }
}

void
sys_dual_mbox_free(struct sys_dual_mbox **dmb)
{

  if ((dmb != NULL) && (*dmb != SYS_MBOX_NULL)) {
    struct sys_dual_mbox *dmbox = *dmb;
    SYS_STATS_DEC(dmbox.used);

    LWIP_DEBUGF(SYS_DEBUG, ("sys_dual_mbox_free: going to free thread dmbox 0x%p\n", (void *)dmbox));

    sys_arch_sem_wait(&(dmbox->mutex), 0);

    sys_sem_free(&(dmbox->not_empty));
    sys_sem_free(&(dmbox->not_full));
    sys_sem_free(&(dmbox->mutex));
	
    mem_free(dmbox->msgs_priority);
    dmbox->msgs_priority = NULL;
    mem_free(dmbox->msgs);
    dmbox->msgs = NULL;
    mem_free(dmbox);
    *dmb = NULL;

    LWIP_DEBUGF(SYS_DEBUG, ("sys_dual_mbox_free: freed thread mbox\n"));
  }
}
/*---------------------------------------------------------------------------*
 * Routine:  sys_mbox_post
 *---------------------------------------------------------------------------*
 * Description:
 *      Post the "msg" to the mailbox.
 * Inputs:
 *      sys_mbox_t mbox        -- Handle of mailbox
 *      void *msg              -- Pointer to data to post
 *---------------------------------------------------------------------------*/
void
sys_mbox_post(struct sys_mbox **mb, void *msg)
{
  struct sys_mbox *mbox;
  void **realloc_msgs;
  mbox = *mb;
  LWIP_DEBUGF(SYS_DEBUG, ("sys_mbox_post: mbox 0x%p msg 0x%p\n", (void *)mbox, (void *)msg));

  sys_arch_sem_wait(&(mbox->mutex), 0);

  while (mbox->isFull) {
    LWIP_DEBUGF(SYS_DEBUG, ("sys_mbox_post : mbox 0x%p mbox 0x%p, queue is full\n", (void *)mbox, (void *)msg));
	
    sys_sem_signal(&(mbox->mutex));
    sys_arch_sem_wait(&(mbox->not_full), 0);
    sys_arch_sem_wait(&(mbox->mutex), 0);
  }

  mbox->msgs[mbox->last] = msg;

  mbox->last = (mbox->last + 1) % mbox->mbox_size;

  if (mbox->first == mbox->last) {
    if (mbox->isAutoExpand && (2*mbox->mbox_size <= MAX_MBOX_SIZE) &&
      (realloc_msgs = malloc(2 * sizeof(void *) * mbox->mbox_size)) != NULL) {
      if (mbox->first > 0) {
        (void)MEMCPY_S(realloc_msgs,
          sizeof(void *) * (mbox->mbox_size - mbox->first),
          mbox->msgs + mbox->first,
          sizeof(void *) * (mbox->mbox_size - mbox->first));
        (void)MEMCPY_S(realloc_msgs + (mbox->mbox_size - mbox->first),
          sizeof(void *) * mbox->last,
          mbox->msgs,
          sizeof(void *) * mbox->last);
      } else {
        (void)MEMCPY_S(realloc_msgs, sizeof(void *) * mbox->mbox_size,
          mbox->msgs, sizeof(void *) * mbox->mbox_size);
      }
      free(mbox->msgs);
      mbox->msgs = realloc_msgs;
      mbox->first = 0;
      mbox->last  = mbox->mbox_size;
      mbox->mbox_size *= 2;
      LWIP_DEBUGF(SYS_DEBUG, ("sys_mbox_post : mbox 0x%p is auto expanded\n", (void *)mbox));
    } else {
      mbox->isFull = 1;
    }
  }

  if (mbox->isEmpty) {
    mbox->isEmpty = 0;

    sys_sem_signal(&(mbox->not_empty));
    LWIP_DEBUGF(SYS_DEBUG, ("sys_mbox_post : mbox 0x%p msg 0x%p, signalling not empty\n", (void *)mbox, (void *)msg));
  }
  LWIP_DEBUGF(SYS_DEBUG, ("sys_mbox_post: mbox 0x%p msg 0%p posted\n", (void *)mbox, (void *)msg));
  sys_sem_signal(&(mbox->mutex));
}

/*---------------------------------------------------------------------------*
 * Routine:  sys_dual_mbox_post
 *---------------------------------------------------------------------------*
 * Description:
 *      Post the "msg" to the mailbox.
 * Inputs:
 *      sys_dual_mbox_t mbox        -- Handle of tcpip mailbox
 *      void *msg              -- Pointer to data to post
 *---------------------------------------------------------------------------*/
void
sys_dual_mbox_post(struct sys_dual_mbox **dmb, void *msg)
{
  struct sys_dual_mbox *dmbox= *dmb;

  LWIP_DEBUGF(SYS_DEBUG, ("sys_dual_mbox_post: dmbox 0x%p msg 0x%p\n", (void *)dmbox, (void *)msg));

  sys_arch_sem_wait(&(dmbox->mutex), 0);

  while (dmbox->isFull  || dmbox->mbox_total_used == dmbox->mbox_size) {
    sys_sem_signal(&(dmbox->mutex));
    sys_arch_sem_wait(&(dmbox->not_full), 0);
    sys_arch_sem_wait(&(dmbox->mutex), 0);
  }

  dmbox->msgs[dmbox->last] = msg;

  dmbox->last = (dmbox->last + 1) % dmbox->mbox_size;
  dmbox->mbox_total_used++;
  dmbox->mbox_used++;

  if (dmbox->first == dmbox->last) {
    dmbox->isFull = 1;
  }

  if (dmbox->isEmpty) {
    dmbox->isEmpty = 0;
    sys_sem_signal(&(dmbox->not_empty)); /* if signal failed, anyway it will unlock and go out */
    LWIP_DEBUGF(SYS_DEBUG, ("sys_dual_mbox_post : dmbox 0x%p msg 0x%p, signalling not empty\n", (void *)dmbox, (void *)msg));
  }
  LWIP_DEBUGF(SYS_DEBUG, ("sys_dual_mbox_post: dmbox 0x%p msg 0%p posted\n", (void *)dmbox, (void *)msg));
  sys_sem_signal(&(dmbox->mutex));
}

/*---------------------------------------------------------------------------*
 * Routine:  sys_dual_mbox_post_priority
 *---------------------------------------------------------------------------*
 * Description:
 *      Post the "msg" to the mailbox.
 * Inputs:
 *      sys_dual_mbox_t dmbox        -- Handle of tcpip mailbox
 *      void *msg              -- Pointer to data to post
 *---------------------------------------------------------------------------*/
void
sys_dual_mbox_post_priority(struct sys_dual_mbox **dmb, void *msg)
{
  struct sys_dual_mbox *dmbox = *dmb;
  LWIP_DEBUGF(SYS_DEBUG, ("sys_dual_mbox_post_priority: dmbox 0x%p msg 0x%p\n", (void *)dmbox, (void *)msg));

  sys_arch_sem_wait(&(dmbox->mutex), 0);

  while (dmbox->isFull || dmbox->mbox_total_used == dmbox->mbox_size || dmbox->mbox_used_priority == dmbox->mbox_size_priority) {
    sys_sem_signal(&(dmbox->mutex));
    sys_arch_sem_wait(&(dmbox->not_full), 0);
    sys_arch_sem_wait(&(dmbox->mutex), 0);
  }

  dmbox->msgs_priority[dmbox->last_p] = msg;

  dmbox->last_p = (dmbox->last_p + 1) % dmbox->mbox_size_priority;
  dmbox->mbox_total_used++;
  dmbox->mbox_used_priority++;

  if (dmbox->first_p == dmbox->last_p || dmbox->mbox_total_used == dmbox->mbox_size) {
    dmbox->isFull = 1;
  }

  if (dmbox->isEmpty) {
    dmbox->isEmpty = 0;
    sys_sem_signal(&(dmbox->not_empty));
    LWIP_DEBUGF(SYS_DEBUG, ("sys_dual_mbox_post_priority : dmbox 0x%p msg 0x%p, signalling not empty\n", (void *)dmbox, (void *)msg));
  }
  LWIP_DEBUGF(SYS_DEBUG, ("sys_dual_mbox_post_priority: dmbox 0x%p msg 0%p posted\n", (void *)dmbox, (void *)msg));
  sys_sem_signal(&(dmbox->mutex));
}
/*---------------------------------------------------------------------------*
 * Routine:  sys_mbox_trypost
 *---------------------------------------------------------------------------*
 * Description:
 *      Try to post the "msg" to the mailbox.  Returns immediately with
 *      error if cannot.
 * Inputs:
 *      sys_mbox_t mbox         -- Handle of mailbox
 *      void *msg               -- Pointer to data to post
 * Outputs:
 *      err_t                   -- ERR_OK if message posted, else ERR_MEM
 *                                  if not.
 *---------------------------------------------------------------------------*/
err_t
sys_mbox_trypost(struct sys_mbox **mb, void *msg)
{
  struct sys_mbox *mbox;
  void **realloc_msgs;
  mbox = *mb;

  sys_arch_sem_wait(&(mbox->mutex), 0);

  LWIP_DEBUGF(SYS_DEBUG, ("sys_mbox_post: mbox 0x%"X32_F" msg 0x%"X32_F"\n", (void *)mbox, (void *)msg));

  if (mbox->isFull) {
    LWIP_DEBUGF(SYS_DEBUG, ("sys_mbox_trypost : mbox 0x%p msgx 0x%p,queue is full\n", (void *)mbox, (void *)msg));
    sys_sem_signal(&(mbox->mutex));
    return ERR_MEM;
  }

  mbox->msgs[mbox->last] = msg;

  mbox->last = (mbox->last + 1) % mbox->mbox_size;

  if (mbox->first == mbox->last) {
    if (mbox->isAutoExpand && (2*mbox->mbox_size <= MAX_MBOX_SIZE) &&
      (realloc_msgs = malloc(2 * sizeof(void *) * mbox->mbox_size)) != NULL) {
      if (mbox->first > 0) {
        (void)MEMCPY_S(realloc_msgs,
          sizeof(void *) * (mbox->mbox_size - mbox->first),
          mbox->msgs + mbox->first,
          sizeof(void *) * (mbox->mbox_size - mbox->first));
        (void)MEMCPY_S(realloc_msgs + (mbox->mbox_size - mbox->first),
          sizeof(void *) * mbox->last,
          mbox->msgs,
          sizeof(void *) * mbox->last);
      } else {
        (void)MEMCPY_S(realloc_msgs, sizeof(void *) * mbox->mbox_size,
          mbox->msgs, sizeof(void *) * mbox->mbox_size);
      }

      free(mbox->msgs);
      mbox->msgs = realloc_msgs;
      mbox->first = 0;
      mbox->last  = mbox->mbox_size;
      mbox->mbox_size *= 2;
      LWIP_DEBUGF(SYS_DEBUG, ("sys_mbox_trypost : mbox 0x%p is auto expanded\n", (void *)mbox));
    } else {
      mbox->isFull = 1;
    }
  }
  if (mbox->isEmpty) {
    mbox->isEmpty = 0;

    sys_sem_signal(&mbox->not_empty);
  }
  LWIP_DEBUGF(SYS_DEBUG, ("sys_mbox_trypost: mbox 0x%p msg 0x%p posted\n", (void *)mbox, (void *)msg));
  sys_sem_signal(&(mbox->mutex));
  return ERR_OK;
}
/*---------------------------------------------------------------------------*
 * Routine:  sys_dual_mbox_trypost
 *---------------------------------------------------------------------------*
 * Description:
 *      Try to post the "msg" to the mailbox.  Returns immediately with
 *      error if cannot.
 * Inputs:
 *      sys_dual_mbox_t dmbox         -- Handle of tcpip mailbox
 *      void *msg               -- Pointer to data to post
 * Outputs:
 *      err_t                   -- ERR_OK if message posted, else ERR_MEM
 *                                  if not.
 *---------------------------------------------------------------------------*/
err_t
sys_dual_mbox_trypost(struct sys_dual_mbox **dmb, void *msg)
{
  struct sys_dual_mbox *dmbox = *dmb;

  LWIP_DEBUGF(SYS_DEBUG, ("sys_dual_mbox_trypost: dmbox 0x%p msg 0x%p \n", (void *)dmbox, (void *)msg));

  sys_arch_sem_wait(&(dmbox->mutex), 0);

  if (dmbox->isFull) {
    LWIP_DEBUGF(SYS_DEBUG, ("sys_dual_mbox_trypost : dmbox 0x%p msgx 0x%p,queue is full\n", (void *)dmbox, (void *)msg));
    sys_sem_signal(&(dmbox->mutex));
    /*performance optimization for DOS attack*/
    (void)LOS_TaskDelay(5);
    return ERR_MEM;
  }

  dmbox->msgs[dmbox->last] = msg;

  dmbox->last = (dmbox->last + 1) % dmbox->mbox_size;
  dmbox->mbox_total_used++;

  if (dmbox->first == dmbox->last) {
    dmbox->isFull = 1;
  }

  if (dmbox->isEmpty) {
    dmbox->isEmpty = 0;
    sys_sem_signal(&dmbox->not_empty);
  }
  LWIP_DEBUGF(SYS_DEBUG, ("sys_dual_mbox_trypost: dmbox 0x%p msg 0x%p posted\n", (void *)dmbox, (void *)msg));
  sys_sem_signal(&(dmbox->mutex));
  return ERR_OK;
}

u32_t
sys_arch_mbox_fetch_ext(struct sys_mbox **mb, void **msg, u32_t timeout, u8_t ignore_timeout)
{
  u32_t time_needed = 0;
  struct sys_mbox *mbox;
  mbox = *mb;

  LWIP_DEBUGF(SYS_DEBUG, ("sys_arch_mbox_fetch: mbox 0x%p msg 0x%p\n", (void *)mbox, (void *)msg));

  /* The mutex lock is quick so we don't bother with the timeout
     stuff here. */
  sys_arch_sem_wait(&(mbox->mutex), 0);

  while (mbox->isEmpty && !ignore_timeout) {
    sys_sem_signal(&(mbox->mutex));

    /* We block while waiting for a mail to arrive in the mailbox. We
       must be prepared to timeout. */
    if (timeout != 0) {
      time_needed = sys_arch_sem_wait(&(mbox->not_empty), timeout);

      if (time_needed == SYS_ARCH_TIMEOUT) {
        return SYS_ARCH_TIMEOUT;
      }
    } else {
      sys_arch_sem_wait(&(mbox->not_empty), 0);
    }

    sys_arch_sem_wait(&(mbox->mutex), 0);
  }

  if (msg != NULL) {
    *msg = mbox->msgs[mbox->first];
    LWIP_DEBUGF(SYS_DEBUG, ("sys_arch_mbox_fetch: mbox 0x%p msg 0x%p\n", \
                                                                                               (void *)mbox, (void *)*msg));
  } else {
    LWIP_DEBUGF(SYS_DEBUG, ("sys_arch_mbox_fetch: mbox 0x%p, null msg\n", (void *)mbox));
  }

  mbox->first = (mbox->first + 1) % mbox->mbox_size;

  if (mbox->first == mbox->last) {
    mbox->isEmpty = 1;
  }

  if (mbox->isFull) {
    mbox->isFull = 0;
    sys_sem_signal(&mbox->not_full);
  }
  LWIP_DEBUGF(SYS_DEBUG, ("sys_arch_mbox_fetch: mbox 0x%p msg 0x%p fetched\n", (void *)mbox, (void *)msg));
  sys_sem_signal(&(mbox->mutex));

  return time_needed;
}

u32_t
sys_arch_dual_mbox_fetch_ext(struct sys_dual_mbox **dmb, void **msg, u32_t timeout, u8_t ignore_timeout)
{
  u32_t time_needed = 0;
  struct sys_dual_mbox *dmbox = *dmb;

  LWIP_DEBUGF(SYS_DEBUG, ("sys_arch_dual_mbox_fetch: dmbox %p msg %p\n", (void *)dmbox, (void *)msg));

  /* The mutex lock is quick so we don't bother with the timeout
     stuff here. */
  sys_arch_sem_wait(&(dmbox->mutex), 0);

  while (dmbox->isEmpty && !ignore_timeout) {
    sys_sem_signal(&(dmbox->mutex));

    /* We block while waiting for a mail to arrive in the mailbox. We
       must be prepared to timeout. */
    if (timeout != 0) {
      time_needed = sys_arch_sem_wait(&(dmbox->not_empty), timeout);

      if (time_needed == SYS_ARCH_TIMEOUT) {
        return SYS_ARCH_TIMEOUT;
      }
    } else {
      sys_arch_sem_wait(&(dmbox->not_empty), 0);
    }

    sys_arch_sem_wait(&(dmbox->mutex), 0);
  }

  if (msg != NULL) {
    /* first check priority mbox*/
    if (dmbox->mbox_used_priority > 0) {
      *msg = dmbox->msgs_priority[dmbox->first_p];
      dmbox->mbox_total_used--;
      dmbox->mbox_used_priority--;
      dmbox->first_p = (dmbox->first_p + 1) % dmbox->mbox_size_priority;
    } else {
      *msg = dmbox->msgs[dmbox->first];
      dmbox->mbox_total_used--;
      dmbox->mbox_used--;
      dmbox->first = (dmbox->first + 1) % dmbox->mbox_size;
    }
    LWIP_DEBUGF(SYS_DEBUG, ("sys_arch_dual_mbox_fetch: dmbox %p msg %p\n", \
                                                                                               (void *)dmbox, (void *)*msg));
  } else {
    LWIP_DEBUGF(SYS_DEBUG, ("sys_arch_dual_mbox_fetch: dmbox %p, null msg\n", (void *)dmbox));
	if (dmbox->mbox_used_priority)
    {
      dmbox->first_p = (dmbox->first_p + 1) % dmbox->mbox_size_priority;
    } else if (dmbox->mbox_used) {
      dmbox->first = (dmbox->first + 1) % dmbox->mbox_size;
    }
  }

  if (dmbox->mbox_total_used == 0) {
    dmbox->isEmpty = 1;
  }

  if (dmbox->isFull) {
    dmbox->isFull = 0;
    sys_sem_signal(&dmbox->not_full);
  }
  LWIP_DEBUGF(SYS_DEBUG, ("sys_arch_dual_mbox_fetch: dmbox %p msg %p fetched\n", (void *)dmbox, (void *)msg));
  sys_sem_signal(&(dmbox->mutex));

  return time_needed;
}
/*---------------------------------------------------------------------------*
 * Routine:  sys_arch_mbox_fetch
 *---------------------------------------------------------------------------*
 * Description:
 *      Blocks the thread until a message arrives in the mailbox, but does
 *      not block the thread longer than "timeout" milliseconds (similar to
 *      the sys_arch_sem_wait() function). The "msg" argument is a result
 *      parameter that is set by the function (i.e., by doing "*msg =
 *      ptr"). The "msg" parameter maybe NULL to indicate that the message
 *      should be dropped.
 *
 *      The return values are the same as for the sys_arch_sem_wait() function:
 *      Number of milliseconds spent waiting or SYS_ARCH_TIMEOUT if there was a
 *      timeout.
 *
 *      Note that a function with a similar name, sys_mbox_fetch(), is
 *      implemented by lwIP.
 * Inputs:
 *      sys_mbox_t mbox         -- Handle of mailbox
 *      void **msg              -- Pointer to pointer to msg received
 *      u32_t timeout           -- Number of milliseconds until timeout
 * Outputs:
 *      u32_t                   -- SYS_ARCH_TIMEOUT if timeout, else number
 *                                  of milliseconds until received.
 *---------------------------------------------------------------------------*/
u32_t
sys_arch_mbox_fetch(struct sys_mbox **mb, void **msg, u32_t timeout)
{
  return sys_arch_mbox_fetch_ext(mb, msg, timeout, 0);
}

u32_t
sys_arch_dual_mbox_fetch(struct sys_dual_mbox **dmb, void **msg, u32_t timeout)
{
  return sys_arch_dual_mbox_fetch_ext(dmb, msg, timeout, 0);
}


/*---------------------------------------------------------------------------*
 * Routine:  sys_init
 *---------------------------------------------------------------------------*
 * Description:
 *      Initialize sys arch
 *---------------------------------------------------------------------------*/
void sys_init(void) {
  /* set rand seed to make random sequence diff on every startup */
  extern VOID LOS_GetCpuCycle(UINT32 *puwCntHi, UINT32 *puwCntLo);
  u32_t seedhsb, seedlsb;
  LOS_GetCpuCycle(&seedhsb, &seedlsb);
  srand(seedlsb);
}


/*---------------------------------------------------------------------------*
 * Routine:  sys_arch_protect
 *---------------------------------------------------------------------------*
 * Description:
 *      This optional function does a "fast" critical region protection and
 *      returns the previous protection level. This function is only called
 *      during very short critical regions. An embedded system which supports
 *      ISR-based drivers might want to implement this function by disabling
 *      interrupts. Task-based systems might want to implement this by using
 *      a mutex or disabling tasking. This function should support recursive
 *      calls from the same task or interrupt. In other words,
 *      sys_arch_protect() could be called while already protected. In
 *      that case the return value indicates that it is already protected.
 *
 *      sys_arch_protect() is only required if your port is supporting an
 *      OS.
 * Outputs:
 *      sys_prot_t              -- Previous protection level (not used here)
 *---------------------------------------------------------------------------*/
sys_prot_t
sys_arch_protect(void)
{
#if LWIP_LITEOS_COMPAT
    LOS_TaskLock();
    return 0;
#else
    int iRet;

    /* Note that for the UNIX port, we are using a lightweight mutex, and our
     * own counter (which is locked by the mutex). The return code is not actually
     * used. */
    if (lwprot_thread != pthread_self())
    {
        /* We are locking the mutex where it has not been locked before *
        * or is being locked by another thread */
        iRet = pthread_mutex_lock(&lwprot_mutex);
        LWIP_ASSERT("pthread_mutex_lock failed", (iRet == 0));
        lwprot_thread = pthread_self();
        lwprot_count = 1;
    }
    else
        /* It is already locked by THIS thread */
        lwprot_count++;
    return 0;
#endif
}


/*---------------------------------------------------------------------------*
 * Routine:  sys_arch_unprotect
 *---------------------------------------------------------------------------*
 * Description:
 *      This optional function does a "fast" set of critical region
 *      protection to the value specified by pval. See the documentation for
 *      sys_arch_protect() for more information. This function is only
 *      required if your port is supporting an OS.
 * Inputs:
 *      sys_prot_t              -- Previous protection level (not used here)
 *---------------------------------------------------------------------------*/
void
sys_arch_unprotect(sys_prot_t pval)
{
#if LWIP_LITEOS_COMPAT
    LWIP_UNUSED_ARG(pval);
    LOS_TaskUnlock();
#else
    int iRet;

    LWIP_UNUSED_ARG(pval);
    if (lwprot_thread == pthread_self())
    {
        if (--lwprot_count == 0)
        {
            lwprot_thread = (pthread_t) 0xDEAD;
            iRet = pthread_mutex_unlock(&lwprot_mutex);
            LWIP_ASSERT("pthread_mutex_unlock failed", (iRet == 0));
            LWIP_UNUSED_ARG(iRet);
        }
    }
#endif
}

u32_t sys_now(void) {
    /* Lwip docs mentioned like wraparound is not a problem in this funtion */
    return (u32_t)((LOS_TickCountGet() * OS_SYS_MS_PER_SECOND) / LOSCFG_BASE_CORE_TICK_PER_SECOND);
}

sys_thread_t
sys_thread_new(const char *name, lwip_thread_fn function, void *arg, int stacksize, int prio)
{
    TSK_INIT_PARAM_S task;
    UINT32 taskid, ret;
    MEMSET_S(&task, sizeof(task), 0, sizeof(task));

    /* Create host Task */
    task.pfnTaskEntry = (TSK_ENTRY_FUNC)function;
    task.uwStackSize  = LOSCFG_BASE_CORE_TSK_DEFAULT_STACK_SIZE;
    task.pcName = (char *)name;
    task.usTaskPrio = prio;
    task.auwArgs[0] = (UINT32)arg;
    task.uwResved   = LOS_TASK_STATUS_DETACHED;
    ret = LOS_TaskCreate(&taskid, &task);
    if (LOS_OK != ret ) {
        LWIP_DEBUGF(SYS_DEBUG, ("sys_thread_new: LOS_TaskCreate error %u\n", (unsigned long)ret));
        return (sys_thread_t)-1;
    }

    return taskid;
}

#ifdef LWIP_DEBUG

/** \brief  Displays an error message on assertion

    This function will display an error message on an assertion
    to the dbg output.

    \param[in]    msg   Error message to display
    \param[in]    line  Line number in file with error
    \param[in]    file  Filename with error
 */
void assert_printf(char *msg, int line, char *file) {
    LWIP_DEBUGF(SYS_ARCH_DEBUG | LWIP_DBG_TRACE | LWIP_DBG_LEVEL_SERIOUS,("%s:%d in file %s\n", msg, line, file));
}
#endif /* LWIP_DEBUG */


/*---------------------------------------------------------------------------*
 * Routine:  sys_sem_new
 *---------------------------------------------------------------------------*
 * Description:
 *      Creates and returns a new semaphore. The "ucCount" argument specifies
 *      the initial state of the semaphore.
 *      NOTE: Currently this routine only creates counts of 1 or 0
 * Inputs:
 *      sys_sem_t sem         -- Handle of semaphore
 *      u8_t count            -- Initial count of semaphore
 * Outputs:
 *      err_t                 -- ERR_OK if semaphore created
 *---------------------------------------------------------------------------*/
err_t sys_sem_new(sys_sem_t *sem,  u8_t count)
{
    UINT32 puwSemHandle;
    UINT32 uwRet;

    if (NULL == sem)
    {
        return -1;
    }

    LWIP_ASSERT("in sys_sem_new count exceeds the limit", (count < 0xFF));

    uwRet = LOS_SemCreate(count, &puwSemHandle);
    if (uwRet != ERR_OK)
    {
        return -1;
    }

    sem->sem= GET_SEM(puwSemHandle);

    return ERR_OK;
}


/*---------------------------------------------------------------------------*
 * Routine:  sys_arch_sem_wait
 *---------------------------------------------------------------------------*
 * Description:
 *      Blocks the thread while waiting for the semaphore to be
 *      signaled. If the "timeout" argument is non-zero, the thread should
 *      only be blocked for the specified time (measured in
 *      milliseconds).
 *
 *      If the timeout argument is non-zero, the return value is the number of
 *      milliseconds spent waiting for the semaphore to be signaled. If the
 *      semaphore wasn't signaled within the specified time, the return value is
 *      SYS_ARCH_TIMEOUT. If the thread didn't have to wait for the semaphore
 *      (i.e., it was already signaled), the function may return zero.
 *
 *      Notice that lwIP implements a function with a similar name,
 *      sys_sem_wait(), that uses the sys_arch_sem_wait() function.
 * Inputs:
 *      sys_sem_t sem           -- Semaphore to wait on
 *      u32_t timeout           -- Number of milliseconds until timeout
 * Outputs:
 *      u32_t                   -- Time elapsed or SYS_ARCH_TIMEOUT.
 *---------------------------------------------------------------------------*/
u32_t sys_arch_sem_wait(sys_sem_t *sem, u32_t timeout)
{
    int retval = 0;
    u64_t u64StartTick;
    u64_t u64EndTick;

    if (!sem)
    {
        return (u32_t)-1;
    }

    u64StartTick = LOS_TickCountGet();

    if(timeout == 0)
    {
      timeout = LOS_WAIT_FOREVER;
    }
    else
    {
      timeout = LOS_MS2Tick(timeout);
      if(!timeout)
        timeout = 1;
    }
    retval = LOS_SemPend(sem->sem->usSemID, timeout);
    if (retval != ERR_OK)
    {
        return SYS_ARCH_TIMEOUT;
    }

    u64EndTick = LOS_TickCountGet();
    /* Here milli second will not come more than 32 bit because timeout received as 32 bit millisecond only */
    return (u32_t)(((u64EndTick - u64StartTick) * OS_SYS_MS_PER_SECOND) / LOSCFG_BASE_CORE_TICK_PER_SECOND);
}


/*---------------------------------------------------------------------------*
 * Routine:  sys_sem_signal
 *---------------------------------------------------------------------------*
 * Description:
 *      Signals (releases) a semaphore
 * Inputs:
 *      sys_sem_t sem           -- Semaphore to signal
 *---------------------------------------------------------------------------*/
void sys_sem_signal(sys_sem_t *sem)
{
    UINT32    uwRet;

    if (!sem)
    {
        return;
    }

    uwRet = LOS_SemPost(sem->sem->usSemID);
    if (uwRet != ERR_OK)
    {
        return;
    }

    return;
}


/*---------------------------------------------------------------------------*
 * Routine:  sys_sem_free
 *---------------------------------------------------------------------------*
 * Description:
 *      Deallocates a semaphore
 * Inputs:
 *      sys_sem_t sem           -- Semaphore to free
 *---------------------------------------------------------------------------*/

void sys_sem_free(sys_sem_t *sem)
{
    UINT32    uwRet;

    if (!sem)
    {
        return;
    }

    uwRet = LOS_SemDelete(sem->sem->usSemID);
    LWIP_ASSERT("LOS_SemDelete failed", (uwRet == 0));

    LWIP_UNUSED_ARG(uwRet);

    return;
}


#if !LWIP_COMPAT_MUTEX
/** Create a new mutex
 * @param mutex pointer to the mutex to create
 * @return a new mutex */
err_t sys_mutex_new(sys_mutex_t *mutex)
{
    u32_t ret;
    ret = LOS_MuxCreate(mutex);
    LWIP_ASSERT("sys_mutex_new failed", (ret == LOS_OK));
    return ret;
}
/** Lock a mutex
 * @param mutex the mutex to lock */
void sys_mutex_lock(sys_mutex_t *mutex)
{
    u32_t ret;
    ret = LOS_MuxPend(*mutex, LOS_WAIT_FOREVER);
    LWIP_ASSERT("sys_mutex_lock failed", (ret == LOS_OK));
    LWIP_UNUSED_ARG(ret);
    return;
}
/** Unlock a mutex
 * @param mutex the mutex to unlock */
void sys_mutex_unlock(sys_mutex_t *mutex)
{
    u32_t ret;
    ret = LOS_MuxPost(*mutex);
    LWIP_ASSERT("sys_mutex_unlock failed", (ret == LOS_OK));
    LWIP_UNUSED_ARG(ret);
    return;
}
/** Delete a semaphore
 * @param mutex the mutex to delete */
void sys_mutex_free(sys_mutex_t *mutex)
{
    u32_t ret;
    ret = LOS_MuxDelete(*mutex);
    LWIP_ASSERT("sys_mutex_free failed", (ret == LOS_OK));
    LWIP_UNUSED_ARG(ret);
    return;
}
#endif
