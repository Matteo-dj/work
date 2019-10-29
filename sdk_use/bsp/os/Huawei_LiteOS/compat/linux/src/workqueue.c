/*----------------------------------------------------------------------------
 *      Huawei - Huawei LiteOS
 *----------------------------------------------------------------------------
 *      Name:    Workqueue.c
 *      Purpose: High-precision-software-timer-related functions
 *      Rev.:    V1.0.0
 *----------------------------------------------------------------------------
 *

 * Copyright (c) 2014, Huawei Technologies Co., Ltd.
 * All rights reserved.
 * Permission to use, copy, modify, and distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.

 *THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 *WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 *MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
 *ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 *WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 *ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
 *OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 *---------------------------------------------------------------------------*/

#include "linux/workqueue.h"
#include "los_swtmr.ph"
#include "los_event.h"
#include "los_trace.h"

struct workqueue_struct *g_pstSystemWq;
pthread_mutex_t wq_mutex = PTHREAD_MUTEX_INITIALIZER;
extern SWTMR_CTRL_S *m_pstSwtmrSortList;

static cpu_workqueue_struct *init_cpu_workqueue(struct workqueue_struct *wq, int cpu);
static int create_workqueue_thread(cpu_workqueue_struct *cwq, int cpu);
static void worker_thread(cpu_workqueue_struct *__cwq);
static void run_workqueue(cpu_workqueue_struct *cwq);

/**
 * @ingroup workqueue
 * Obtain the first work in a workqueue.
 *
 */
#define worklist_entry(ptr, type, member)  ((type *)((char *)(ptr)-((UINT32)&(((type*)0)->member))))

/**
 * @ingroup workqueue
 * Traverse a workqueue.
 *
 */
#define LIST_FOR_WORK(pos, list_object, type, field)      \
        for (pos = LOS_DL_LIST_ENTRY((list_object)->next, type, field);     \
             &pos->field != (list_object);                            \
             pos = LOS_DL_LIST_ENTRY(pos->field.next, type, field))

#define LIST_FOR_WORK_DEL(pos, pstnext, list_object, type, field)      \
                for (pos = LOS_DL_LIST_ENTRY((list_object)->next, type, field),     \
                        pstnext = LOS_DL_LIST_ENTRY(pos->field.next, type, field);     \
                     &pos->field != (list_object);                            \
                     pos = pstnext, pstnext = LOS_DL_LIST_ENTRY(pos->field.next, type, field))

void delayed_work_timer_fn(unsigned long __data);
void init_delayed_work(struct delayed_work *_work, work_func_t _func)
{
    if (NULL == _work || NULL == _func)
    {
        return;
    }
    INIT_WORK((&((_work)->work)), _func);
    (_work)->timer.function = delayed_work_timer_fn;
    (_work)->timer.data = (unsigned long)(_work);
    (_work)->timer.flag = TIMER_UNVALID;
    (_work)->work.work_status = 0;
}

static unsigned int wq_is_empty(cpu_workqueue_struct *cwq)
{
    UINTPTR uwIntSave;
    UINT32 uwRet;

    uwIntSave = LOS_IntLock();
    uwRet = list_empty(&cwq->worklist);
    LOS_IntRestore(uwIntSave);

    return uwRet;
}

struct workqueue_struct *__create_workqueue_key(char *name,
                        int singlethread,
                        int freezeable,
                        int rt,
                        struct lock_class_key *key,
                        const char *lock_name)
{
    struct workqueue_struct *wq;
    cpu_workqueue_struct *cwq;
    UINT32 uwRet;

    if (NULL == name || singlethread == 0)
    {
        return (struct workqueue_struct *)NULL;
    }

    wq = (struct workqueue_struct *)LOS_MemAlloc(m_aucSysMem0, sizeof(struct workqueue_struct));
    if (NULL == wq)
    {
        return (struct workqueue_struct *)NULL;
    }

    wq->cpu_wq = (cpu_workqueue_struct *)LOS_MemAlloc(m_aucSysMem0, sizeof(cpu_workqueue_struct));
    if (NULL == wq->cpu_wq)
    {
        (VOID)LOS_MemFree(m_aucSysMem0, (VOID *)wq);
        return (struct workqueue_struct *)NULL;
    }
    memset(wq->cpu_wq, 0, sizeof(cpu_workqueue_struct));
    wq->name = name;
    wq->singlethread = singlethread;
    wq->freezeable = freezeable;
    wq->rt = rt;
    wq->delayed_work_count =0;
    INIT_LIST_HEAD(&wq->list);
    LOS_EventInit(&wq->wq_event);/*lint !e534*/

    cwq = init_cpu_workqueue(wq, singlethread);
    uwRet = create_workqueue_thread(cwq, singlethread);
    if (uwRet)
    {
        destroy_workqueue(wq);
        wq = (struct workqueue_struct *)NULL;
    }

    return wq;
}

struct workqueue_struct * create_singlethread_workqueue(char *name)
{
    return __create_workqueue_key(name,1,0,0,(struct lock_class_key *)NULL, (const char *)NULL);
}

static cpu_workqueue_struct *init_cpu_workqueue(struct workqueue_struct *wq, int cpu)
{
    cpu_workqueue_struct *cwq = wq->cpu_wq;

    cwq->wq = wq;
    INIT_LIST_HEAD(&cwq->worklist);
    INIT_LIST_HEAD(&cwq->free_worklist);

    return cwq;
}

static int create_workqueue_thread(cpu_workqueue_struct *cwq, int cpu)
{
    struct workqueue_struct *wq = cwq->wq;
    TSK_INIT_PARAM_S os_task_init_param;
    UINT32 uwRet;

    os_task_init_param.pfnTaskEntry = (TSK_ENTRY_FUNC)worker_thread;
    os_task_init_param.uwStackSize  = LOSCFG_BASE_CORE_TSK_DEFAULT_STACK_SIZE;
    os_task_init_param.pcName       = wq->name;
    os_task_init_param.usTaskPrio   = 1;
    os_task_init_param.auwArgs[0] = (unsigned int)cwq;
    os_task_init_param.uwResved   = LOS_TASK_STATUS_DETACHED;

    uwRet = LOS_TaskCreate(&cwq->wq->wq_id, &os_task_init_param);
    if(LOS_OK != uwRet)
    {
        return LOS_NOK;
    }

    cwq->thread = (task_struct*)OS_TCB_FROM_TID(cwq->wq->wq_id);
    LOS_TaskYield();/*lint !e534*/

    return LOS_OK;
}

static void worker_thread(cpu_workqueue_struct *__cwq)
{
    cpu_workqueue_struct *cwq = __cwq;

    for (;;)
    {
        if (wq_is_empty(cwq))
        {
            LOS_EventRead(&(cwq->wq->wq_event), 0x01, LOS_WAITMODE_OR |
                LOS_WAITMODE_CLR, LOS_WAIT_FOREVER);/*lint !e534*/
        }
        run_workqueue(cwq);
    }
}

static void run_workqueue(cpu_workqueue_struct *cwq)
{
    struct work_struct *work;
    UINTPTR uwIntSave;
    work_func_t f;

    if(!wq_is_empty(cwq))
    {
        uwIntSave = LOS_IntLock();
        work = worklist_entry(cwq->worklist.next, struct work_struct, entry);/*lint !e413*/
        work->work_status |= WORK_STRUCT_RUNNING;
        list_del_init(cwq->worklist.next);
        f = work->func;
        LOS_IntRestore(uwIntSave);
        los_trace_workq(SCHEDULE_WORKQ, work->work_priority, work->data[0]);
        f(work);
        uwIntSave = LOS_IntLock();
        if (work->work_recycle == WORK_RECYCLE)
        {
            work->work_status &= ~(WORK_STRUCT_RUNNING | WORK_STRUCT_PENDING);
            list_add(&(work->entry), &(cwq->free_worklist));
        }
        LOS_IntRestore(uwIntSave);
    }
}

static void __list_add(struct list_head * _new, struct list_head * prev, struct list_head * next)
{
    next->prev = _new;
    _new->next = next;
    _new->prev = prev;
    prev->next = _new;
}

bool queue_work_on(struct workqueue_struct *wq, struct work_struct *work)
{
    struct work_struct *pstwork;
    struct work_struct *before_pstwork = NULL;
    LIST_FOR_WORK(pstwork, &wq->cpu_wq->worklist, struct work_struct, entry)/*lint !e413*/
    {
        if (work->work_priority < pstwork->work_priority)
        {
            before_pstwork = pstwork;
            break;
        }

    }
    if (before_pstwork == NULL)
    {
        __list_add(&(work->entry), wq->cpu_wq->worklist.prev, &(wq->cpu_wq->worklist));
    }
    else
    {
        __list_add(&(work->entry), before_pstwork->entry.prev, &(before_pstwork->entry));
    }

    LOS_EventWrite(&(wq->wq_event), 0x01);/*lint !e534*/
    return TRUE;
}

bool queue_work(struct workqueue_struct *wq, struct work_struct *work)
{
    bool uwRet = FALSE;
    UINTPTR uwIntSave;

    if(NULL == wq || NULL == wq->name || NULL == work)
        return FALSE;

    uwIntSave = LOS_IntLock();

    work->work_status = WORK_STRUCT_PENDING;
    uwRet = queue_work_on(wq, work);

    LOS_IntRestore(uwIntSave);
    return uwRet;
}

/**
 * cancel_work_sync - cancel a work and wait for it to finish
 * @work: the work to cancel
 *
 * Return:
 * %true if @work was pending, %false otherwise.
 */
bool cancel_work_sync(struct work_struct *work)
{
    bool uwRet = FALSE;

    if(NULL == work)
        return FALSE;

    if(!work->work_status)
    {
        uwRet = FALSE;
    }
    else if(work->work_status & WORK_STRUCT_RUNNING)
    {
        uwRet = FALSE;
    }
    else
    {
        uwRet = TRUE;
    }
    while(work->work_status)
    {
        (void)LOS_TaskDelay(LOS_MS2Tick(10));
    }
    return uwRet;
}

/**
 * flush_work - wait for a work to finish executing the last queueing instance
 * @work: the work to flush
 *
 * Return:
 * %true if flush_work() waited for the work to finish execution,
 * %false if it was already idle.
 */
bool flush_work(struct work_struct *work)
{
    if(NULL == work)
        return FALSE;

    if(work->work_status & WORK_STRUCT_PENDING)
    {
        while(work->work_status)
        {
            (void)LOS_TaskDelay(LOS_MS2Tick(10));
        }
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

void delayed_work_timer_fn(unsigned long __data)
{
    struct delayed_work *dwork = (struct delayed_work *)__data;
    UINTPTR uwIntSave;

    /* should have been called from irqsafe timer with irq already off */
    uwIntSave = LOS_IntLock();
    dwork->wq->delayed_work_count--;
    queue_work_on(dwork->wq, &dwork->work);/*lint !e534*/
    (void)del_timer(&dwork->timer);
    LOS_IntRestore(uwIntSave);
}



/**
 * queue_delayed_work - queue work on a workqueue after delaytime
 * @wq: workqueue to use
 * @dwork: delayable work to queue
 * @delaytime: number of ticks to wait before queueing
 *
 */
bool queue_delayed_work(struct workqueue_struct *wq, struct delayed_work *dwork, unsigned int delaytime)
{
    bool  uwRet = TRUE;
    UINTPTR uwIntSave;
    struct work_struct *pstwork;
    SWTMR_CTRL_S *pstCurSwTmr;

    if((NULL == wq) || (NULL == wq->name) || (NULL == wq->cpu_wq) || (NULL == dwork))
        return FALSE;

    dwork->wq = wq;
    if(delaytime == 0)
    {
        uwRet = queue_work(dwork->wq, &dwork->work);
        return uwRet;
    }
    else
    {
        uwIntSave = LOS_IntLock();
        pstCurSwTmr = m_pstSwtmrSortList;
        while(NULL != pstCurSwTmr)
        {
            if(((struct delayed_work *)pstCurSwTmr->uwArg) == dwork)
            {
                LOS_IntRestore(uwIntSave);
                return FALSE;
            }
            pstCurSwTmr = pstCurSwTmr->pstNext;
        }

        if (wq_is_empty(wq->cpu_wq))
        {
            uwRet = TRUE;
        }
        else
        {
            LIST_FOR_WORK(pstwork, &wq->cpu_wq->worklist, struct work_struct, entry)/*lint !e413*/
            {
                if(pstwork == &dwork->work)
                {
                    LOS_IntRestore(uwIntSave);
                    return FALSE;
                }
            }
        }
    }

    dwork->timer.expires = delaytime;
    add_timer(&dwork->timer);
    wq->delayed_work_count++;
    dwork->work.work_status = WORK_STRUCT_PENDING;

    LOS_IntRestore(uwIntSave);
    return uwRet;
}

/**
 * cancel_delayed_work - cancel a delayed work
 * @dwork: delayed_work to cancel
 *
 * Kill off a pending delayed_work.
 *
 * Return: %true if @dwork was pending and canceled; %false if it wasn't
 * pending.
 */
bool cancel_delayed_work(struct delayed_work *dwork)
{
    SWTMR_CTRL_S *pstCurSwTmr;
    struct work_struct *pstwork;
    struct work_struct *pstworknext;
    UINTPTR uwIntSave;
    bool  uwRet = FALSE;

    if ((NULL == dwork) || (NULL == dwork->wq))
        return FALSE;

    uwIntSave = LOS_IntLock();
    if(dwork->work.work_status & WORK_STRUCT_PENDING)
    {
        pstCurSwTmr = m_pstSwtmrSortList;
        while(NULL != pstCurSwTmr)
        {
            if(pstCurSwTmr->usTimerID == dwork->timer.timerid && pstCurSwTmr->ucState == OS_SWTMR_STATUS_TICKING)
            {
                del_timer(&dwork->timer);/*lint !e534*/
                dwork->work.work_status = 0;
                dwork->wq->delayed_work_count--;
                LOS_IntRestore(uwIntSave);
                return TRUE;
            }
            pstCurSwTmr = pstCurSwTmr->pstNext;
        }

        if(dwork->work.work_status & WORK_STRUCT_RUNNING)
        {
            uwRet = FALSE;
        }
        else if(dwork->work.work_status & WORK_STRUCT_PENDING)
        {
            LIST_FOR_WORK_DEL(pstwork, pstworknext, &dwork->wq->cpu_wq->worklist, struct work_struct, entry)/*lint !e413*/
            {
                if(pstwork == &dwork->work)
                {
                    list_del_init(&pstwork->entry);
                    dwork->work.work_status = 0;
                    uwRet = TRUE;
                    break;
                }
            }
        }
        LOS_IntRestore(uwIntSave);
        return uwRet;
    }
    else
    {
        LOS_IntRestore(uwIntSave);
        return FALSE;
    }
}

/**
 * cancel_delayed_work_sync - cancel a delayed work and wait for it to finish
 * @dwork: the delayed work cancel
 *
 * Return:
 * %true if @dwork was pending, %false otherwise.
 */
bool cancel_delayed_work_sync(struct delayed_work *dwork)
{
    bool  uwRet = FALSE;

    if(NULL == dwork)
        return FALSE;

    LOS_TaskLock();

    uwRet = cancel_delayed_work(dwork);

    LOS_TaskUnlock();
    return uwRet;
}

/**
 * flush_delayed_work - wait for a dwork to finish executing the last queueing
 * @dwork: the delayed work to flush
 *
 * Delayed timer is cancelled and the pending work is queued for
 * immediate execution.  Like flush_work(), this function only
 * considers the last queueing instance of @dwork.
 *
 * Return:
 * %true if flush_work() waited for the work to finish execution,
 * %false if it was already idle.
 */
bool flush_delayed_work(struct delayed_work *dwork)
{
    bool  uwRet = FALSE;
    SWTMR_CTRL_S *pstCurSwTmr;
    UINTPTR uwIntSave;

    if((NULL == dwork) || (NULL == dwork->wq))
        return FALSE;

    uwIntSave = LOS_IntLock();
    if(dwork->work.work_status & WORK_STRUCT_PENDING)
    {
        pstCurSwTmr = m_pstSwtmrSortList;
        while(NULL != pstCurSwTmr)
        {
            if(pstCurSwTmr->usTimerID == dwork->timer.timerid && pstCurSwTmr->ucState == OS_SWTMR_STATUS_TICKING)
            {
                del_timer(&dwork->timer);/*lint !e534*/
                dwork->wq->delayed_work_count--;
                queue_work(dwork->wq, &dwork->work);/*lint !e534*/
                break;
            }
            pstCurSwTmr = pstCurSwTmr->pstNext;
        }
     flush_work(&dwork->work);/*lint !e534*/
     uwRet = TRUE;
    }
    else
    {
        uwRet = FALSE;
    }

    LOS_IntRestore(uwIntSave);
    return uwRet;
}

/**
 * work_busy - test whether a work is currently pending or running
 * @work: the work to be tested
 *
 * Test whether @work is currently pending or running.  There is no
 * synchronization around this function and the test result is
 * unreliable and only useful as advisory hints or for debugging.
 *
 * Return:
 * OR'd bitmask of WORK_BUSY_* bits.
 */
unsigned int work_busy(struct work_struct *work)
{
    unsigned int uwRet = 0;

    if(NULL == work)
        return FALSE;

    if(work->work_status & WORK_STRUCT_PENDING)
    {
        uwRet |= WORK_BUSY_PENDING;
    }
    if(work->work_status & WORK_STRUCT_RUNNING)
    {
        uwRet |= WORK_BUSY_RUNNING;
    }
    return uwRet;
}

/**
 * schedule_work - put work task in global workqueue
 * @work: job to be done
 *
 * Returns %false if @work was already on the kernel-global workqueue and
 * %true otherwise.
 *
 * This puts a job in the kernel-global workqueue if it was not already
 * queued and leaves it in the same position on the kernel-global
 * workqueue otherwise.
 */
bool schedule_work(struct work_struct *work)
{
    bool  uwRet = FALSE;
    UINTPTR uwIntSave;

    if(NULL == g_pstSystemWq || NULL == g_pstSystemWq->name || NULL == work)
        return FALSE;

    uwIntSave = LOS_IntLock();

    work->work_status = WORK_STRUCT_PENDING;
    uwRet = queue_work_on(g_pstSystemWq, work);

    LOS_IntRestore(uwIntSave);
    return uwRet;
}

/**
 * schedule_delayed_work - put work task in global workqueue after delaytime
 * @dwork: job to be done
 * @delaytime: number of ticks to wait or 0 for immediate execution
 *
 * After waiting for a given time this puts a job in the kernel-global
 * workqueue.
 */
bool schedule_delayed_work(struct delayed_work *dwork, unsigned int delaytime)
{
    bool  uwRet = FALSE;
    UINTPTR uwIntSave;

    if(NULL == g_pstSystemWq || NULL == dwork)
        return FALSE;

    uwIntSave = LOS_IntLock();

    uwRet = queue_delayed_work(g_pstSystemWq, dwork, delaytime);

    LOS_IntRestore(uwIntSave);
    return uwRet;
}

/**
 *Wait until the workqueue becomes empty
 */
void drain_workqueue(struct workqueue_struct *wq)
{
    while(1)
    {
        (void)LOS_TaskDelay(LOS_MS2Tick(10));
        if(wq_is_empty(wq->cpu_wq) && wq->delayed_work_count == 0)
        {
            break;
        }
    }
}

/**
 * destroy_workqueue - safely terminate a workqueue
 * @wq: target workqueue
 *
 * Safely destroy a workqueue. All work currently pending will be done first.
 */
void destroy_workqueue(struct workqueue_struct *wq)
{
    UINTPTR uwIntSave;

    if(NULL == wq)
        return;

/* drain it before proceeding with destruction */
    drain_workqueue(wq);

    uwIntSave = LOS_IntLock();
    wq->name = (char *)NULL;
    list_del_init(&wq->list);
    (VOID)LOS_TaskDelete(wq->wq_id);
    (VOID)LOS_EventDestroy(&(wq->wq_event));
    if (wq->cpu_wq->work_array != NULL)
    {
        (VOID)LOS_MemFree(m_aucSysMem0, (VOID *)(wq->cpu_wq->work_array));
    }
    (VOID)LOS_MemFree(m_aucSysMem0, (VOID *)(wq->cpu_wq));
    (VOID)LOS_MemFree(m_aucSysMem0, (VOID *)(wq));
    LOS_IntRestore(uwIntSave);
}

UINT32 init_irq_bottom_work_list(struct workqueue_struct *pstWq, UINT32 work_num)
{
    UINT32 size;
    UINT32 i;
    cpu_workqueue_struct *cpu_wq;
    struct work_struct *work_array;

    if (0 == work_num || NULL == pstWq)  /*lint !e506*/
    {
        return LOS_NOK;
    }

    size = sizeof(struct work_struct) * work_num;
    work_array = (struct work_struct *)LOS_MemAlloc(m_aucSysMem0, size);
    if (NULL == work_array)
    {
        return LOS_NOK;
    }
    (VOID)memset((void *)work_array, 0, size);

    cpu_wq = pstWq->cpu_wq;
    cpu_wq->work_array = work_array;
    for (i = 0; i < work_num; i++)
    {
        list_add(&work_array[i].entry, &(cpu_wq->free_worklist));
    }
    return LOS_OK;
}

int irq_bottom_half(struct work_struct *in_work)
{
    struct work_struct *mywork;
    cpu_workqueue_struct *cpu_wq;

    if(NULL == in_work || NULL == in_work->func || NULL == g_pstSystemWq)
    {
        return EINVAL;
    }

    cpu_wq = g_pstSystemWq->cpu_wq;
    if (list_empty(&(cpu_wq->free_worklist)))
    {
        return ENOMEM;
    }

    mywork = list_entry(cpu_wq->free_worklist.next, struct work_struct, entry); /*lint !e413*/
    list_del(cpu_wq->free_worklist.next);

    memcpy(mywork, in_work, sizeof(struct work_struct));
    if (mywork->work_priority > WORKQ_IRQ_DEFAULT_PRI)
    {
        mywork->work_priority = WORKQ_IRQ_DEFAULT_PRI;
    }
    los_trace_workq(ENQUEUE_WORKQ, mywork->work_priority, mywork->data[0]);
    mywork->work_recycle = WORK_RECYCLE;
    if(!(queue_work(g_pstSystemWq, mywork)))
    {
        return EAGAIN;
    }

    return ENOERR; /*lint !e429*/
}


int init_default_workqueue(UINT32 work_num)
{
    g_pstSystemWq = create_workqueue("system_wq");
    return init_irq_bottom_work_list(g_pstSystemWq, work_num);
}

