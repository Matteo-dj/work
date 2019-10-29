/*----------------------------------------------------------------------------
 * Copyright (c) <2013-2015>, <Huawei Technologies Co., Ltd>
 * All rights reserved.
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 * 1. Redistributions of source code must retain the above copyright notice, this list of
 * conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice, this list
 * of conditions and the following disclaimer in the documentation and/or other materials
 * provided with the distribution.
 * 3. Neither the name of the copyright holder nor the names of its contributors may be used
 * to endorse or promote products derived from this software without specific prior written
 * permission.
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
 * ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *---------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------
 * Notice of Export Control Law
 * ===============================================
 * Huawei LiteOS may be subject to applicable export control laws and regulations, which might
 * include those applicable to Huawei LiteOS of U.S. and the country in which you are located.
 * Import, export and usage of Huawei LiteOS in any manner by you shall be in compliance with such
 * applicable export control laws and regulations.
 *---------------------------------------------------------------------------*/

#include "los_event.ph"
#include "pprivate.h"
#include "pthread_cond.h"
#include "stdlib.h"


int pthread_condattr_getpshared(const pthread_condattr_t *attr, int *pshared)
{
    if (attr == NULL || pshared == NULL) {
        return EINVAL;
    }

    *pshared = PTHREAD_PROCESS_PRIVATE;

    return 0;
}

int pthread_condattr_setpshared(pthread_condattr_t *attr, int pshared)
{
    if ((pshared != PTHREAD_PROCESS_PRIVATE) && (pshared != PTHREAD_PROCESS_SHARED)) {
        return EINVAL;
    }

    if (pshared != PTHREAD_PROCESS_PRIVATE) {
        return ENOSYS;
    }

    return 0;
}

int pthread_condattr_destroy(pthread_condattr_t *attr)
{
    if (attr == NULL) {
        return EINVAL;
    }

    return 0;
}

int pthread_condattr_init(pthread_condattr_t * attr)
{
    if (attr == NULL) {
        return EINVAL;
    }

    return 0;
}

int pthread_cond_destroy(pthread_cond_t *cond)
{
    int ret = ENOERR;
    if (cond == NULL) {
        return EINVAL;
    }

    if (cond->value == -1){
        return ENOERR;
    }

    if (LOS_ListEmpty(&(cond->event.stEventList))) {
        if (ENOERR != pthread_mutex_destroy(cond->mutex)) {
            PRINT_ERR("%s mutex destroy fail!\n",__FUNCTION__);
            return EINVAL;
        }
        free(cond->mutex);
        cond->mutex = NULL;

        (void)LOS_EventDestroy(&(cond->event));
    } else {
        return EBUSY;
    }

    return ret;
}

int pthread_cond_init(pthread_cond_t *cond, const pthread_condattr_t *attr)
{
    int ret = ENOERR;

    if (cond == NULL) {
        return EINVAL;
    }


    (void)LOS_EventInit(&(cond->event));//todo

    cond->mutex = (pthread_mutex_t *)malloc(sizeof(pthread_mutex_t));
    if(cond->mutex == NULL) {
        return ENOMEM;
    }

    (void)pthread_mutex_init(cond->mutex, (const pthread_mutexattr_t *)NULL);//todo

    cond->value = 0;

    return ret;
}

int pthread_cond_broadcast(pthread_cond_t *cond)
{
    int ret = ENOERR;

    if (cond == NULL) {
        return EINVAL;
    }

    (void)pthread_mutex_lock(cond->mutex);
    if (cond->value > 0){
        cond->value = 0;
        (void)pthread_mutex_unlock(cond->mutex);
        (void)LOS_EventWrite(&(cond->event), 0x01);//todo
        return ret;
    }

    (void)pthread_mutex_unlock(cond->mutex);
    return ret;
}

int pthread_cond_signal(pthread_cond_t *cond)
{
    int ret = ENOERR;

    if (cond == NULL) {
        return EINVAL;
    }

    (void)pthread_mutex_lock(cond->mutex);
    if (cond->value > 0){
        cond->value--;
        (void)pthread_mutex_unlock(cond->mutex);
        (void)osEventWriteOnce(&(cond->event), 0x01);//todo
        return ret;
    }
    (void)pthread_mutex_unlock(cond->mutex);

    return ret;
}

int pthread_cond_timedwait(pthread_cond_t *cond,
        pthread_mutex_t *mutex,
        struct timespec *abstime)
{
    int abs_ticks;
    int ret;

    PTHREAD_TESTCANCEL();
    if (cond == NULL || mutex == NULL || abstime == NULL) {
        return EINVAL;
    }

    if (cond->value == -1) {
        ret = pthread_cond_init(cond, (pthread_condattr_t *)NULL);
        if (ret != ENOERR) {
            return ret;
        }
    }

    (void)pthread_mutex_lock(cond->mutex);
    cond->value++;
    (void)pthread_mutex_unlock(cond->mutex);

    if ((abstime->tv_sec == 0) && (abstime->tv_nsec == 0)) {
        return ETIMEDOUT;
    }

    if (abstime->tv_sec < 0 ||
            abstime->tv_nsec < 0 ||
            abstime->tv_nsec > 999999999L) {
        return EINVAL;
    }

    abs_ticks = abstime->tv_sec * LOSCFG_BASE_CORE_TICK_PER_SECOND \
                + (abstime->tv_nsec / 1000000) / (1000 / LOSCFG_BASE_CORE_TICK_PER_SECOND);

    if ((abstime->tv_nsec) && (abs_ticks == 0))
    {
        abs_ticks = 1;
    }

    if (pthread_mutex_unlock(mutex) != ENOERR){ /*lint !e455*/
        PRINT_ERR("%s is not support %d\n",__FUNCTION__,__LINE__);
    }
    ret = (int)LOS_EventRead(&(cond->event), 0x0f, LOS_WAITMODE_OR | LOS_WAITMODE_CLR, abs_ticks);

    if (pthread_mutex_lock(mutex) != ENOERR){
        PRINT_ERR("%s is not support %d\n",__FUNCTION__,__LINE__);
    }

    switch (ret) {
        case 0:
        case 1:
            ret = ENOERR;
            break;
        case LOS_ERRNO_EVENT_READ_TIMEOUT:{
            (void)pthread_mutex_lock(cond->mutex);
            if(cond->value > 0)
                cond->value--;
            (void)pthread_mutex_unlock(cond->mutex);
            ret = ETIMEDOUT;
            break;
        }
        default:{
            (void)pthread_mutex_lock(cond->mutex);
            if(cond->value > 0)
                cond->value--;
            (void)pthread_mutex_unlock(cond->mutex);
            ret = EINVAL;
            break;
        }
    }

    PTHREAD_TESTCANCEL();
    return ret;  /*lint !e454*/
}

int pthread_cond_wait(pthread_cond_t *cond, pthread_mutex_t *mutex)
{
    int ret = 0;

    if (cond == NULL || mutex == NULL) {
        return EINVAL;
    }

    if (cond->value == -1) {
        ret = pthread_cond_init(cond, (pthread_condattr_t *)NULL);
        if (ret != ENOERR) {
            return ret;
        }
    }

    (void)pthread_mutex_lock(cond->mutex);
    cond->value ++;
    (void)pthread_mutex_unlock(cond->mutex);

    if (pthread_mutex_unlock(mutex) != ENOERR){ /*lint !e455*/
        PRINT_ERR("%s is not support %d\n",__FUNCTION__,__LINE__);
    }

    ret = (int)LOS_EventRead(&(cond->event), 0x0f, LOS_WAITMODE_OR | LOS_WAITMODE_CLR, LOS_WAIT_FOREVER);

    if (pthread_mutex_lock(mutex) != ENOERR){
        PRINT_ERR("%s is not support%d\n",__FUNCTION__,__LINE__);
    }

    switch (ret) {
        case 0:
        case 1:
            ret = ENOERR;
            break;
        default:{
            (void)pthread_mutex_lock(cond->mutex);
            if (cond->value > 0)
                cond->value --;
            (void)pthread_mutex_unlock(cond->mutex);
            ret = EINVAL;
            break;
        }
    }

    return ret; /*lint !e454*/
}
