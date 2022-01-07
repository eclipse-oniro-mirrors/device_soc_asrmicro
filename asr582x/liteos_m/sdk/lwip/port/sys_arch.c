/*
 * Copyright (c) 2013-2019, Huawei Technologies Co., Ltd. All rights reserved.
 * Copyright (c) 2020, Huawei Device Co., Ltd. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice, this list of
 *    conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice, this list
 *    of conditions and the following disclaimer in the documentation and/or other materials
 *    provided with the distribution.
 *
 * 3. Neither the name of the copyright holder nor the names of its contributors may be used
 *    to endorse or promote products derived from this software without specific prior written
 *    permission.
 *
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
 */

#include <arch/sys_arch.h>
#include <lwip/sys.h>
#include <lwip/debug.h>
#include "lwip/def.h"
#include "lwip/mem.h"
#include "lwip/stats.h"
#include <los_task.h>
#include <los_tick.h>
#include <los_queue.h>
#include <los_sem.h>
#include <los_config.h>

#define ROUND_UP_DIV(val, div) (((val) + (div) - 1) / (div))

/* This is the number of threads that can be started with sys_thread_new() */
#define SYS_THREAD_MAX 5


static u16_t s_nextthread = 0;

/**
 * Thread and System misc
 */

sys_thread_t sys_thread_new(const char *name, lwip_thread_fn thread, void *arg, int stackSize, int prio)
{
    sys_thread_t CreatedTask;
    OSStatus result;

   if ( s_nextthread < SYS_THREAD_MAX )
   {
      result = lega_rtos_create_thread(&CreatedTask,(uint8_t)prio,name,(lega_thread_function_t)thread,(uint32_t)stackSize,(uint32_t)arg);
#if 0
    UINT32 taskID = LOS_ERRNO_TSK_ID_INVALID;
    UINT32 ret;
    TSK_INIT_PARAM_S task = {0};

    /* Create host Task */
    task.pfnTaskEntry = (TSK_ENTRY_FUNC)thread;
    task.uwStackSize = stackSize;
    task.pcName = (char *)name;
    task.usTaskPrio = prio;
    task.uwArg = (UINTPTR)arg;
    task.uwResved = LOS_TASK_STATUS_DETACHED;
    ret = LOS_TaskCreate(&taskID, &task);
#endif

       // For each task created, store the task handle (pid) in the timers array.
       // This scheme doesn't allow for threads to be deleted

       if(result == kNoErr)
       {
           return CreatedTask;
       }
       else
       {
           return NULL;
       }
   }
   else
   {
      return NULL;
   }
}

void sys_init(void)
{
#if 0
    /* set rand seed to make random sequence diff on every startup */
    UINT32 seedhsb, seedlsb;
    LOS_GetCpuCycle(&seedhsb, &seedlsb);
    srand(seedlsb);
#endif
    // keep track of how many threads have been created
    s_nextthread = 0;
}

u32_t sys_now(void)
{
    return lega_rtos_get_time();
}

/**
 * Protector
 */

sys_prot_t sys_arch_protect(void)
{
    LOS_TaskLock();

    return 0; /* return value is unused */
}

void sys_arch_unprotect(sys_prot_t pval)
{
    LWIP_UNUSED_ARG(pval);

    LOS_TaskUnlock();
}


/**
 * MessageBox
 */

err_t sys_mbox_new(sys_mbox_t *mbox, int size)
{
    CHAR qName[] = "lwIP";
    UINT32 ret = LOS_QueueCreate(qName, (UINT16)size, mbox, 0, sizeof(void *));
    switch (ret) {
        case LOS_OK:
#if SYS_STATS
      ++lwip_stats.sys.mbox.used;
      if (lwip_stats.sys.mbox.max < lwip_stats.sys.mbox.used) {
         lwip_stats.sys.mbox.max = lwip_stats.sys.mbox.used;
      }
#endif /* SYS_STATS */
            return ERR_OK;
        case LOS_ERRNO_QUEUE_CB_UNAVAILABLE:
        case LOS_ERRNO_QUEUE_CREATE_NO_MEMORY:
            return ERR_MEM;
        default:
            break;
    }
#if SYS_STATS
      lwip_stats.sys.mbox.err++;
#endif /* SYS_STATS */
    LWIP_DEBUGF(SYS_DEBUG, ("%s: LOS_QueueCreate error %u\n", __FUNCTION__, ret));
    return ERR_ARG;
}

void sys_mbox_post(sys_mbox_t *mbox, void *msg)
{
    /* Caution: the second parameter is NOT &msg */
    UINT32 ret = LOS_QueueWrite(*mbox, msg, sizeof(char *), LOS_WAIT_FOREVER);
    if (ret != LOS_OK) {
#if SYS_STATS
      lwip_stats.sys.mbox.err++;
#endif /* SYS_STATS */
        LWIP_DEBUGF(SYS_DEBUG, ("%s: LOS_QueueWrite error %u\n", __FUNCTION__, ret));
    }
}

err_t sys_mbox_trypost(sys_mbox_t *mbox, void *msg)
{
    /* Caution: the second parameter is NOT &msg */
    UINT32 ret = LOS_QueueWrite(*mbox, msg, sizeof(char *), 0);
    switch (ret) {
        case LOS_OK:
            return ERR_OK;
        case LOS_ERRNO_QUEUE_ISFULL:
            return ERR_MEM;
        default:
            break;
    }
#if SYS_STATS
      lwip_stats.sys.mbox.err++;
#endif /* SYS_STATS */
    LWIP_DEBUGF(SYS_DEBUG, ("%s: LOS_QueueWrite error %u\n", __FUNCTION__, ret));
    return ERR_ARG;
}

err_t sys_mbox_trypost_fromisr(sys_mbox_t *mbox, void *msg);

u32_t sys_arch_mbox_fetch(sys_mbox_t *mbox, void **msg, u32_t timeoutMs)
{
    void *ignore = 0; /* if msg==NULL, the fetched msg should be dropped */
    UINT64 tick = ROUND_UP_DIV((UINT64)timeoutMs * LOSCFG_BASE_CORE_TICK_PER_SECOND, OS_SYS_MS_PER_SECOND);
    UINT32 ret = LOS_QueueRead(*mbox, msg ? msg : &ignore, sizeof(void *), tick ? (UINT32)tick : LOS_WAIT_FOREVER);
    switch (ret) {
        case LOS_OK:
            return ERR_OK;
        case LOS_ERRNO_QUEUE_ISEMPTY:
        case LOS_ERRNO_QUEUE_TIMEOUT:
            return SYS_ARCH_TIMEOUT;
        default:
            break;
    }
    LWIP_DEBUGF(SYS_DEBUG, ("%s: LOS_QueueRead error %u\n", __FUNCTION__, ret));
    return SYS_ARCH_TIMEOUT; /* Errors should be treated as timeout */
}

u32_t sys_arch_mbox_tryfetch(sys_mbox_t *mbox, void **msg)
{
    void *ignore = 0; /* if msg==NULL, the fetched msg should be dropped */
    UINT32 ret = LOS_QueueRead(*mbox, msg ? msg : &ignore, sizeof(void *), 0);
    switch (ret) {
        case LOS_OK:
            return ERR_OK;
        case LOS_ERRNO_QUEUE_ISEMPTY:
            return SYS_MBOX_EMPTY;
        case LOS_ERRNO_QUEUE_TIMEOUT:
            return SYS_ARCH_TIMEOUT;
        default:
            break;
    }
    LWIP_DEBUGF(SYS_DEBUG, ("%s: LOS_QueueRead error %u\n", __FUNCTION__, ret));
    return SYS_MBOX_EMPTY; /* Errors should be treated as timeout */
}

void sys_mbox_free(sys_mbox_t *mbox)
{
    (void)LOS_QueueDelete(*mbox);
#if SYS_STATS
     --lwip_stats.sys.mbox.used;
#endif /* SYS_STATS */
}

int sys_mbox_valid(sys_mbox_t *mbox)
{
    QUEUE_INFO_S queueInfo;
    return LOS_OK == LOS_QueueInfoGet(*mbox, &queueInfo);
}

void sys_mbox_set_invalid(sys_mbox_t *mbox)
{
    *mbox = LOSCFG_BASE_IPC_QUEUE_LIMIT;
}


/**
 * Semaphore
 */

err_t sys_sem_new(sys_sem_t *sem, u8_t count)
{
    UINT32 ret = LOS_SemCreate(count, sem);
    if (ret != LOS_OK) {
#if SYS_STATS
      ++lwip_stats.sys.sem.err;
#endif /* SYS_STATS */
        return ERR_ARG;
    }
#if SYS_STATS
    ++lwip_stats.sys.sem.used;
     if (lwip_stats.sys.sem.max < lwip_stats.sys.sem.used) {
        lwip_stats.sys.sem.max = lwip_stats.sys.sem.used;
    }
#endif /* SYS_STATS */
    return ERR_OK;
}

void sys_sem_signal(sys_sem_t *sem)
{
    (void)LOS_SemPost(*sem);
}

u32_t sys_arch_sem_wait(sys_sem_t *sem, u32_t timeoutMs)
{
    UINT64 tick = ROUND_UP_DIV((UINT64)timeoutMs * LOSCFG_BASE_CORE_TICK_PER_SECOND, OS_SYS_MS_PER_SECOND);
    UINT32 ret = LOS_SemPend(*sem, tick ? (UINT32)tick : LOS_WAIT_FOREVER); // timeoutMs 0 means wait forever
    switch (ret) {
        case LOS_OK:
            return ERR_OK;
        case LOS_ERRNO_SEM_TIMEOUT:
            return SYS_ARCH_TIMEOUT;
        default:
            break;
    }
    LWIP_DEBUGF(SYS_DEBUG, ("%s: LOS_SemPend error %u\n", __FUNCTION__, ret));
    return SYS_ARCH_TIMEOUT; /* Errors should be treated as timeout */
}

void sys_sem_free(sys_sem_t *sem)
{
    (void)LOS_SemDelete(*sem);
#if SYS_STATS
      --lwip_stats.sys.sem.used;
#endif /* SYS_STATS */
}

int sys_sem_valid(sys_sem_t *sem)
{
    return *sem != LOSCFG_BASE_IPC_SEM_LIMIT;
}

void sys_sem_set_invalid(sys_sem_t *sem)
{
    *sem = LOSCFG_BASE_IPC_SEM_LIMIT;
}


/**
 * Mutex
 */

err_t sys_mutex_new(sys_mutex_t *mutex)
{
    err_t ret = ERR_MEM;
    int stat = lega_rtos_init_mutex(mutex);

    if (stat == 0) {
        ret = ERR_OK;
    }
    return ret;
}

void sys_mutex_lock(sys_mutex_t *mutex)
{
    lega_rtos_lock_mutex(mutex, LEGA_WAIT_FOREVER);
}

void sys_mutex_unlock(sys_mutex_t *mutex)
{
    lega_rtos_unlock_mutex(mutex);
}

void sys_mutex_free(sys_mutex_t *mutex)
{
    lega_rtos_deinit_mutex(mutex);
}

int sys_mutex_valid(sys_mutex_t *mutex)
{
    if(*mutex!=NULL)
        return 1;
    else
        return 0;
}

void sys_mutex_set_invalid(sys_mutex_t *mutex)
{
    *mutex=NULL;
}
