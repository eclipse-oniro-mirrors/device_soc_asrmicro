/*
 * Copyright (c) 2022 ASR Microelectronics (Shanghai) Co., Ltd. All rights reserved.
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <string.h>
#include <stdlib.h>
#include "duet_common.h"
#include "cmsis_os2.h"
#include "include/kal.h"
#include "target_config.h"
#include "lega_rtos.h"

#ifdef LEGA_CM4
#include "lega_cm4.h"
#else
#ifdef DUET_CM4
#include "duet_cm4.h"
#endif
#endif

#define CM4_ICSR  0xE000ED04

#define COMPRESS_LEN(x) (sizeof(size_t))

lega_task_config_t task_cfg[LEGA_TASK_CONFIG_MAX] = {
#ifdef USE_SMALL_STACK
    {LEGA_WPA3_AUTH_TASK_PRIORITY, 2560},
    {LEGA_LWIFI_TASK_PRIORITY, 2560},
#else
    {LEGA_WPA3_AUTH_TASK_PRIORITY, 13312},
    {LEGA_LWIFI_TASK_PRIORITY, 2048},
#endif
    {LEGA_UWIFI_RX_TASK_PRIORITY, 4096},
    {LEGA_UWIFI_TASK_PRIORITY, 7168},
    {LEGA_LWIP_DHCP_TASK_PRIORITY, 1536},
    {LEGA_BLE_SCHEDULER_PRIORITY, 4096},
};

OSBool lega_rtos_is_in_interrupt_context(void)
{
    unsigned int state = 0;

    state = (*(unsigned int *) CM4_ICSR) & 0x3FF; // bit0:9 is IRQ NO.

    if (state != 0) {
        return TRUE;
    }

    return  FALSE;
}

void lega_vector_table_reloc(void)
{

}
/*******************************************************************
* Name: lega_task_cfg_init
* Description: this api get wifi task config
* input param: task index defined in lega_rtos.h
* output param: config data of wifi task
* return: OSSatus value
*******************************************************************/
OSStatus lega_rtos_task_cfg_get(uint32_t index, lega_task_config_t *cfg)
{
    if (!cfg || (index >= LEGA_TASK_CONFIG_MAX)) {
        return kGeneralErr;
    }
    cfg->task_priority = task_cfg[index].task_priority;
    cfg->stack_size = task_cfg[index].stack_size;

    return kNoErr;
}
/*******************************************************************
* Name: lega_rtos_create_thread
* Description: this api call freeRTOS function "xTaskCreate" to create task
* input param: task priority, stack_size, function, name
* output param: thread
* return: OSSatus value
*******************************************************************/
OSStatus lega_rtos_create_thread(lega_thread_t *thread, uint8_t priority, const char *name,
                                  lega_thread_function_t function, uint32_t stack_size, lega_thread_arg_t arg)
{
    uint8_t autorun = 1;
    osThreadAttr_t attr = {0};
    osThreadId_t threadId;

    if (thread == NULL || name == NULL || function == NULL) {
        printf("thread create fail\r\n");
        return kGeneralErr;
    }

    attr.name = name;
    attr.attr_bits = 0U;
    attr.cb_mem = NULL;
    attr.cb_size = 0U;
    attr.stack_mem = NULL;
    attr.stack_size = stack_size;
    // here should add bound check!
    attr.priority = priority;
    if (autorun == 1) {
        threadId = osThreadNew((osThreadFunc_t)function, (void *)arg, &attr);
        if (threadId == 0) {
            printf("thread %s create fail %d\r\n", name, (int)threadId);
            return kGeneralErr;
        }
        *(unsigned int *)thread = (unsigned int)threadId;
    }
    return kNoErr;
}

/*******************************************************************
* Name: lega_rtos_delete_thread
* Description: this api calls FreeRTOS function "xTaskDelete" to delete a task
* input param: thread
* output param: none
* return:  OSStatus
*******************************************************************/
OSStatus lega_rtos_delete_thread(lega_thread_t *thread)
{
    osThreadId_t threadId;
    if (thread == NULL) {
        threadId = osThreadGetId();
    } else {
        threadId = (osThreadId_t)(*(unsigned int *)thread);
    }

    if (osThreadTerminate(threadId) != 0) {
        printf("thread delete fail\r\n");
        return kGeneralErr;
    }

    if (thread != NULL) {
        *(unsigned int *)thread = 0;
    }
    return kNoErr;
}
/////////////////////////////  semaphore  //////////////////////////////////////

/*******************************************************************
* Name:lega_rtos_init_semaphore
* Description: create a count semaphore
* input param: init value,
* output param: lega_semaphore_t* semaphore
* return: kNoErr or kGeneralErr;
*******************************************************************/
OSStatus lega_rtos_init_semaphore(lega_semaphore_t *semaphore, int count)
{
    osSemaphoreId_t semId;

    semId = osSemaphoreNew(0xFF, (uint32_t)count, 0);
    if (semId == 0) {
        printf("sema init fail %d\r\n", (int)semId);
        return kGeneralErr;
    }

    *semaphore = (lega_semaphore_t *)semId;
    return kNoErr;
}
/*******************************************************************
* Name:lega_rtos_get_semaphore
* Description: get semaphore value
* input param:semaphore, timeout_ms
* output param: semaphore
* return: kNoErr or kGeneralErr
*******************************************************************/
OSStatus lega_rtos_get_semaphore(lega_semaphore_t *semaphore, uint32_t timeout_ms)
{
    osStatus_t ret;

    ret = osSemaphoreAcquire((osSemaphoreId_t)(*semaphore), timeout_ms);
    if (ret != osOK) {
        printf("sem get fail %d\r\n", ret);
        return kGeneralErr;
    }

    return kNoErr;
}

FLASH_COMMON2_SEG OSStatus lega_rtos_set_semaphore(lega_semaphore_t *semaphore)
{
    osStatus_t ret;

    ret = osSemaphoreRelease((osSemaphoreId_t)(*semaphore));
    if (ret != osOK) {
        printf("sema set fail %d\r\n", ret);
        return kGeneralErr;
    }

    return kNoErr;
}

OSStatus lega_rtos_deinit_semaphore(lega_semaphore_t *semaphore)
{
    osStatus_t ret;

    ret = osSemaphoreDelete((osSemaphoreId_t)(*semaphore));
    if (ret != osOK) {
        printf("sema deinit fail %d\r\n", ret);
        return kGeneralErr;
    }
    *semaphore = NULL;
    return kNoErr;
}

OSBool lega_rtos_semaphore_pending_task_null(lega_semaphore_t *semaphore)
{
    osStatus_t ret;

    ret = osSemaphorePendingNoTask((osSemaphoreId_t)(*semaphore));
    if (ret == osOK) {
        return TRUE;
    } else {
        return FALSE;
    }
}

OSStatus lega_rtos_init_mutex(lega_mutex_t *mutex)
{
    osMutexId_t mutexId;
    mutexId = osMutexNew(0);
    if (mutexId == 0) {
        printf("mutex init fail %d\r\n", (int)mutexId);
        return kGeneralErr;
    }

    *mutex = (lega_mutex_t)mutexId;
    return kNoErr;
}

OSStatus lega_rtos_lock_mutex(lega_mutex_t *mutex, uint32_t timeout_ms)
{
    osStatus_t ret;

    ret = osMutexAcquire((osMutexId_t)(*mutex), timeout_ms);
    if (ret != osOK) {
        printf("mutex lock fail %d\r\n", ret);
        return kGeneralErr;
    }

    return kNoErr;
}

OSStatus lega_rtos_unlock_mutex(lega_mutex_t *mutex)
{
    osStatus_t ret;

    ret = osMutexRelease((osMutexId_t)(*mutex));
    if (ret != osOK) {
        printf("mutex unlock fail %d", ret);
        return kGeneralErr;
    }

    return kNoErr;
}

OSStatus lega_rtos_deinit_mutex(lega_mutex_t *mutex)
{
    osStatus_t ret;

    ret = osMutexDelete((osMutexId_t)(*mutex));
    if (ret != osOK) {
        printf("mutex deinit fail %d\r\n", ret);
        return kGeneralErr;
    }

    *mutex = NULL;
    return kNoErr;
}

OSStatus lega_rtos_init_queue(lega_queue_t *queue, const char *name, uint32_t message_size,
                              uint32_t number_of_messages)
{
    osMessageQueueId_t queId;

    (void)name;
    queId = osMessageQueueNew(number_of_messages, message_size, 0);
    if (queId == 0) {
        printf("queue init fail %d\r\n", (int)queId);
        return kGeneralErr;
    }

    *queue = queId;
    return kNoErr;
}

OSStatus lega_rtos_push_to_queue(lega_queue_t *queue, void *message, uint32_t timeout_ms)
{
    osStatus_t ret;

    if (lega_rtos_is_in_interrupt_context()) {
        ret = osMessageQueuePut((osMessageQueueId_t)(*queue), message, 0, 0);
    } else {
        ret = osMessageQueuePut((osMessageQueueId_t)(*queue), message, 0, timeout_ms);
    }
    if (ret != osOK) {
        printf("push_to_queue fail:%d\r\n", ret);
        return kGeneralErr;
    }

    return kNoErr;
}

OSStatus lega_rtos_pop_from_queue(lega_queue_t *queue, void *message, uint32_t timeout_ms)
{
    osStatus_t ret;

    ret = osMessageQueueGet((osMessageQueueId_t)(*queue), message, 0, timeout_ms);
    if (ret != osOK) {
        if (timeout_ms) {
            printf("pop_from_queue fail:%d\r\n", ret);
        }
        return kGeneralErr;
    }

    return kNoErr;
}

OSStatus lega_rtos_deinit_queue(lega_queue_t *queue)
{
    osStatus_t ret;

    ret = osMessageQueueDelete((osMessageQueueId_t)(*queue));
    if (ret != osOK) {
        printf("queue deinit fail:%d\r\n", ret);
        return kGeneralErr;
    }

    return kNoErr;
}

OSBool lega_rtos_is_queue_empty(lega_queue_t *queue)
{
    OSBool ret;
    lega_rtos_declare_critical();

    if (!lega_rtos_is_in_interrupt_context()) {
        lega_rtos_enter_critical();
    }

    if (osMessageQueueGetCount((osMessageQueueId_t)(*queue)) == 0) {
        ret = TRUE;
    } else {
        ret = FALSE;
    }

    if (!lega_rtos_is_in_interrupt_context()) {
        lega_rtos_exit_critical();
    }

    return ret;
}

OSBool lega_rtos_is_queue_full(lega_queue_t *queue)
{
    OSBool ret;
    lega_rtos_declare_critical();

    if (!lega_rtos_is_in_interrupt_context()) {
        lega_rtos_enter_critical();
    }

    if (osMessageQueueGetSpace((osMessageQueueId_t)(*queue)) == 0) {
        ret = TRUE;
    } else {
        ret = FALSE;
    }

    if (!lega_rtos_is_in_interrupt_context()) {
        lega_rtos_exit_critical();
    }

    return ret;
}

#if 1

uint32_t lega_rtos_ms_to_ticks(uint32_t ms)
{
    uint32_t ticks;

    uint64_t padding;

    padding = MS_PER_SECONED / LOSCFG_BASE_CORE_TICK_PER_SECOND;
    padding = (padding > 0) ? (padding - 1) : 0;

    ticks = ((ms + padding) * LOSCFG_BASE_CORE_TICK_PER_SECOND) / MS_PER_SECONED;
    return ticks;
}

#if 1
OSStatus lega_rtos_init_timer(lega_timer_t *timer, uint32_t time_ms, timer_handler_t function, void *arg)
{
    timer->function = function;
    timer->arg      = arg;

    KalTimerType kalType = KAL_TIMER_PERIODIC;
    KalTimerId timeId;

    timeId = KalTimerCreate(function, kalType, arg, time_ms);
    if (timeId == 0) {
        printf("rtos_init_timer err %d\r\n", (int)timeId);
        return kGeneralErr;
    }
    timer->handle = timeId;

    return kNoErr;
}
#else
OSStatus lega_rtos_init_timer_name(lega_timer_t *timer, uint32_t time_ms, timer_handler_t function, void *arg,
                                   uint8_t  *name)
{
    if (!timer) {
        printf("%s %s timer err\n", __FUNCTION__, name);
        return kGeneralErr;
    }
    timer->function = function;
    if (!arg) {
        timer->arg     = name;
    } else {
        timer->arg      = arg;
    }

    KalTimerType kalType = KAL_TIMER_PERIODIC;
    KalTimerId timeId;

    timeId = KalTimerCreate(timer->function, kalType, timer->arg, time_ms);
    if (timeId == 0) {
        printf("rtos_init_timer err %d\r\n", (int)timeId);
        return kGeneralErr;
    }
    timer->handle = timeId;

    return kNoErr;
}
#endif

OSStatus lega_rtos_start_timer(lega_timer_t *timer)
{
    if (!timer) {
        printf("%s timer err\n", __FUNCTION__);
        return kGeneralErr;
    }
    if (KalTimerStart((KalTimerId)(timer->handle)) == KAL_OK) {
        return kNoErr;
    }

    return kGeneralErr;
}

OSStatus lega_rtos_stop_timer(lega_timer_t *timer)
{
    if (!timer) {
        printf("%s timer err\n", __FUNCTION__);
        return kGeneralErr;
    }
    if (KalTimerStop((KalTimerId)(timer->handle)) == KAL_OK) {
        // printf("rtos_stop_timer\r\n");
        return kNoErr;
    }

    return kGeneralErr;
}

OSStatus lega_rtos_reload_timer(lega_timer_t *timer)
{
    if (!timer) {
        printf("%s timer err\n", __FUNCTION__);
        return kGeneralErr;
    }
    if (KalTimerStop((KalTimerId)(timer->handle)) != KAL_OK) {
        printf("rtos_reload_timer err1\r\n");
        return kGeneralErr;
    }

    if (KalTimerStart((KalTimerId)(timer->handle)) != KAL_OK) {
        printf("rtos_reload_timer err2\r\n");
        return kGeneralErr;
    }

    return kNoErr;
}

OSStatus lega_rtos_deinit_timer(lega_timer_t *timer)
{
    if (!timer) {
        printf("%s timer err\n", __FUNCTION__);
        return kGeneralErr;
    }
    if (KalTimerStop((KalTimerId)(timer->handle)) != KAL_OK) {
        printf("rtos_stop_timer err\r\n");
        return kGeneralErr;
    }
    if (KalTimerDelete((KalTimerId)(timer->handle)) == KAL_OK) {

        timer->handle = 0;
        return kNoErr;
    }
    timer->handle = 0;

    return kGeneralErr;
}

OSBool lega_rtos_is_timer_running(lega_timer_t *timer)
{
    if (!timer) {
        printf("%s timer err\n", __FUNCTION__);
        return kGeneralErr;
    }

    if (KalTimerIsRunning((KalTimerId)(timer->handle))) {

        return TRUE;
    }
    return FALSE;
}

uint32_t lega_rtos_ticks_to_ms(uint32_t ticks)
{
    uint64_t   padding;
    uint32_t time;

    padding = LOSCFG_BASE_CORE_TICK_PER_SECOND / 1000;
    padding = (padding > 0) ? (padding - 1) : 0;

    time = ((ticks + padding) * 1000) / LOSCFG_BASE_CORE_TICK_PER_SECOND;

    return time;
}

uint32_t lega_rtos_get_time(void)
{
    return (uint32_t)lega_rtos_ticks_to_ms((unsigned int)osKernelGetTickCount());
}

uint32_t lega_rtos_get_system_ticks(void)
{
    return (uint32_t)osKernelGetTickCount();
}

OSStatus lega_rtos_delay_milliseconds(uint32_t num_ms)
{
    uint32_t ticks;

    ticks = lega_rtos_ms_to_ticks(num_ms);
    if (ticks == 0) {
        ticks = 1;
    }

    osDelay(ticks);

    return kNoErr;
}

#endif
int32_t lega_malloc_cnt = 0;
void *_lega_rtos_malloc(uint32_t xWantedSize, const char *function, uint32_t line)
{
    void *p = malloc(xWantedSize);
    uint32_t size_cache = xWantedSize;
    if (p) {
        lega_malloc_cnt++;
    } else {
        if (size_cache != 0) {
            printf("rtos malloc %u fail  %s:%u\r\n", (unsigned int)xWantedSize, function, (unsigned int)line);
        }
    }
    while (size_cache) {
        if (size_cache >= (sizeof(int))) {
            size_cache -= sizeof(int);
            * (int *)(p + size_cache) = 0;
        } else {
            size_cache--;
            * (char *)(p + size_cache) = 0;   // clear memory alloced
        }
    }
    return p;
}

void lega_rtos_free(void *mem)
{
    if (mem) {
        lega_malloc_cnt--;
    } else {
        printf("rtos free null\r\n");
        return;
    }

    free(mem);
}

void lega_rtos_realloc(void *mem, uint32_t xWantedSize)
{
    if (mem) {
        realloc(mem, xWantedSize);
    } else {
        printf("rtos realloc null\r\n");
    }
    return;
}

void *pvPortMalloc(uint32_t xWantedSize)
{
    return lega_rtos_malloc(xWantedSize);
}

void vPortFree(void *pv)
{
    lega_rtos_free(pv);
}

/*******************************************************************
* Name:lega_rtos_enter_critical
* Description: get system into critical area where there is no context switch
* input param: none
* output param: none
* return:none
*******************************************************************/
lega_cpsr_t _lega_rtos_enter_critical(void)
{
    lega_cpsr_t cpsr_save = LOS_IntLock();
    return cpsr_save;
}

/*******************************************************************
* Name:lega_rtos_exit_critical
* Description: get system out of critical area
* input param: none
* output param:none
* return:none
*******************************************************************/
void _lega_rtos_exit_critical(lega_cpsr_t cpsr_store)
{
    LOS_IntRestore(cpsr_store);
}

void lega_intrpt_enter(void)
{
}

/*******************************************************************
* Name:lega_intrpt_exit
* Description: exit interrupt
* input param: none
* output param:none
* return:none
*******************************************************************/
void lega_intrpt_exit(void)
{
}
