#include "asm/semaphore.h"
#include "linux/wait.h"
#include "poll.h"

int _sema_init(losMutexDef_t *sem,unsigned int value)
{
    UINT32 puwSemHandle;
    UINT32 uwRet;


    if (NULL == sem)
    {
        return -1;
    }

    if( value > OS_SEM_COUNT_MAX )
    {
        return -1;
    }

    uwRet = LOS_SemCreate(value, &puwSemHandle);

    if (uwRet != LOS_OK)
    {
        return -1;
    }

    sem->sem= GET_SEM(puwSemHandle);

    return(0);
}


int _sema_lock(losMutexDef_t *sem)
{
    int retval = 0;

    if (!sem)
    {
        return -1;
    }

    if((unsigned int)sem->sem == 0xffffffff)
    {
        _sema_init(sem,sem->count);/*lint !e534*/
    }

    retval = LOS_SemPend(sem->sem->usSemID, LOS_WAIT_FOREVER);
    //if( !sema->wait() ) retval = EINTR;

    if(retval == LOS_OK)
        return 0;

    retval = -1;

    return(retval);
}

// -------------------------------------------------------------------------
// Decrement value if >0, return -1 if not.
//函数功能: 申请等待一个信号量
//参数说明:
//out  :
//in   :  申请等待的信号量
//返回值: 若成功则返回0,否则返回-1,并设置errno.
//注意： 使用场景:只有当前信号量未被申请等待时调用该API才会申请等待的(即当前信号量值大于0的时候才会去申请等待的,否则不会去申请等待的)

 int _sema_trylock(losMutexDef_t *sem)
{
    int retval = 0;


    if (!sem)
    {
        return -1;
    }

    // Check that whether the semaphore is waited or not
    if(!LOS_ListEmpty(&sem->sem->stSemList) || ( sem->sem->uwSemCount <= 0 ))
    {
        return -1;
    }
    retval = LOS_SemPend(sem->sem->usSemID, LOS_WAIT_FOREVER);
    if(retval == LOS_OK)
        return 0;

    retval = -1;

    return (retval);
}

// -------------------------------------------------------------------------
// Increment value and wake a waiter if one is present.
//函数功能: 申请释放一个指定的信号量
//参数说明:
//out  :
//in   :  申请释放的信号量
//返回值: 若成功则返回0,否则返回-1,并设置errno.
//注意： 每申请释放一次,则信号量的值将被加1

 int _sema_unlock  (losMutexDef_t *sem)
{
    UINT32    uwRet;

    if (!sem)
    {
        return -1;
    }

    uwRet = LOS_SemPost(sem->sem->usSemID);

    if (uwRet != LOS_OK)
    {
        return -1;
    }

    return 0;
}

int _sema_destory(losMutexDef_t *sem)
{
    UINT32 uwRet;

    if(!sem)
    {
        return -1;
    }
    if((unsigned int)sem->sem == 0xffffffff)
    {
        return -1;
    }
    uwRet = LOS_SemDelete(sem->sem->usSemID);

    if (uwRet != LOS_OK)
    {
        return -1;
    }

    return 0;

}

void __init_waitqueue_head(wait_queue_head_t *wait)
{
    if (NULL == wait)
    {
        return;
    }
    LOS_EventInit(&wait->stEvent);/*lint !e534*/
    (VOID)pthread_mutex_init(&wait->mutex, (const pthread_mutexattr_t *)NULL);
    LOS_ListInit(&wait->poll_queue);
}

void __wake_up_interruptible(wait_queue_head_t *wait)
{
    if (NULL == wait)
    {
        return;
    }
    LOS_EventWrite(&wait->stEvent, 0x1);/*lint !e534*/
    notify_poll(wait);
}
// -------------------------------------------------------------------------
// EOF ipc.c

