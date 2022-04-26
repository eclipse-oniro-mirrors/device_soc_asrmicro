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

#ifndef __LEGARTOS_H__
#define __LEGARTOS_H__
#include <ctype.h>
#include <stdint.h>
#include "lega_rtos_port.h"
#include <stdbool.h>
#include "los_interrupt.h"

#define LEGA_NEVER_TIMEOUT   (0xFFFFFFFF)
#define LEGA_WAIT_FOREVER    (0xFFFFFFFF)
#define LEGA_NO_WAIT         (0)

#define LEGA_TASK_CONFIG_MAX            32
#define LEGA_TASK_CONFIG_WPA3_AUTH      0
#define LEGA_TASK_CONFIG_LWIFI          1
#define LEGA_TASK_CONFIG_UWIFI_RX       2
#define LEGA_TASK_CONFIG_UWIFI          3
#define LEGA_TASK_CONFIG_DHCPS          4
#define LEGA_TASK_CONFIG_BLE            5

#define     LEGA_LWIP_DHCP_TASK_PRIORITY             (29)

#ifdef ALIOS_SUPPORT
#define     LEGA_WPA3_AUTH_TASK_PRIORITY             (16)
#define     LEGA_LWIFI_TASK_PRIORITY                 (17)
#define     LEGA_UWIFI_RX_TASK_PRIORITY              (15)
#define     LEGA_UWIFI_TASK_PRIORITY                 (20)
#define     LEGA_BLE_SCHEDULER_PRIORITY              (13) // for testing

#elif defined(HARMONYOS_SUPPORT)
#define     LEGA_WPA3_AUTH_TASK_PRIORITY             (36 - 5)
#define     LEGA_LWIFI_TASK_PRIORITY                 (35 - 5)
#define     LEGA_UWIFI_RX_TASK_PRIORITY              (37 - 5)
#define     LEGA_UWIFI_TASK_PRIORITY                 (33 - 5)
#define     LEGA_BLE_SCHEDULER_PRIORITY              (38 - 5) // for testing
#else
#define     LEGA_WPA3_AUTH_TASK_PRIORITY             (28)
#define     LEGA_LWIFI_TASK_PRIORITY                 (27)
#define     LEGA_UWIFI_RX_TASK_PRIORITY              (28)
#define     LEGA_UWIFI_TASK_PRIORITY                 (26)
#define     LEGA_BLE_SCHEDULER_PRIORITY              (29) // for testing
#endif // ALIOS_SUPPORT

#ifdef ALIOS_SUPPORT
#define CONFIG_KV_BUFFER_SIZE KV_CONFIG_TOTAL_SIZE
#elif defined(HARMONYOS_SUPPORT)
#define CONFIG_KV_BUFFER_SIZE 0x8000
#else
#define CONFIG_KV_BUFFER_SIZE 0
#endif // ALIOS_SUPPORT

#define lega_cpsr_t UINTPTR
#define MS_PER_SECONED  1000
typedef enum {
    kNoErr = 0,
    kGeneralErr,
    kTimeoutErr,
} OSStatus;

#ifndef FALSE
#define  FALSE 0
#endif

#ifndef TRUE
#define TRUE 1
#endif
typedef bool OSBool;

typedef void *lega_semaphore_t;
typedef void *lega_mutex_t;
typedef void *lega_thread_t;
typedef void *lega_queue_t;
typedef void (*timer_handler_t)(void *arg);

typedef struct {
    void           *handle;
    timer_handler_t function;
    void           *arg;
    uint8_t        *name;
} lega_timer_t;

typedef struct {
    uint8_t        task_priority;
    uint32_t       stack_size;
} lega_task_config_t;

typedef struct {
    uint32_t stackTop;
    uint32_t stackBottom;
    // add more info here
} lega_threadinfo_t;

typedef uint32_t lega_thread_arg_t;
typedef void (*lega_thread_function_t)(lega_thread_arg_t arg);

/** @defgroup LEGA_RTOS_Thread LEGA RTOS Thread Management Functions
 *  @brief Provide thread creation, delete, suspend, resume, and other RTOS management API
 *  @verbatim
 *   LEGA thread priority table
 *
 * +----------+-----------------+
 * | Priority |      Thread     |
 * |----------|-----------------|
 * |     0    |      LEGA       |   Highest priority
 * |     1    |     Network     |
 * |     2    |                 |
 * |     3    | Network worker  |
 * |     4    |                 |
 * |     5    | Default Library |
 * |          | Default worker  |
 * |     6    |                 |
 * |     7    |   Application   |
 * |     8    |                 |
 * |     9    |      Idle       |   Lowest priority
 * +----------+-----------------+
 *  @endverbatim
 * @{
 */
OSBool lega_rtos_is_in_interrupt_context(void);

#define lega_rtos_declare_critical() lega_cpsr_t critical_cpsr

/** @brief Enter a critical session, all interrupts are disabled
  *
  * @return    none
  */
lega_cpsr_t _lega_rtos_enter_critical(void);
#define lega_rtos_enter_critical()           \
    do {                                     \
        critical_cpsr = _lega_rtos_enter_critical();  \
    } while (0)
/** @brief Exit a critical session, all interrupts are enabled
  *
  * @return    none
  */
void _lega_rtos_exit_critical(lega_cpsr_t cpsr_store);
#define lega_rtos_exit_critical()        \
    do {                                 \
        _lega_rtos_exit_critical(critical_cpsr); \
    } while (0)
/**
  * @}
  */

/** @brief Creates and starts a new thread
  *
  * @param thread     : Pointer to variable that will receive the thread handle (can be null)
  * @param priority   : A priority number.
  * @param name       : a text name for the thread (can be null)
  * @param function   : the main thread function
  * @param stack_size : stack size for this thread
  * @param arg        : argument which will be passed to thread function
  *
  * @return    kNoErr          : on success.
  * @return    kGeneralErr     : if an error occurred
  */
OSStatus lega_rtos_create_thread(lega_thread_t *thread, uint8_t priority, const char *name,
                                 lega_thread_function_t function, uint32_t stack_size, lega_thread_arg_t arg);

/** @brief   Deletes a terminated thread
  *
  * @param   thread     : the handle of the thread to delete, , NULL is the current thread
  *
  * @return  kNoErr        : on success.
  * @return  kGeneralErr   : if an error occurred
  */
OSStatus lega_rtos_delete_thread(lega_thread_t *thread);

/** @defgroup LEGA_RTOS_SEM LEGA RTOS Semaphore Functions
  * @brief Provide management APIs for semaphore such as init,set,get and dinit.
  * @{
  */

/** @brief    Initialises a counting semaphore and set count to 0
  *
  * @param    semaphore : a pointer to the semaphore handle to be initialised
  * @param    count     : the max count number of this semaphore
  *
  * @return   kNoErr        : on success.
  * @return   kGeneralErr   : if an error occurred
  */
OSStatus lega_rtos_init_semaphore(lega_semaphore_t *semaphore, int count);

/** @brief    Set (post/put/increment) a semaphore
  *
  * @param    semaphore : a pointer to the semaphore handle to be set
  *
  * @return   kNoErr        : on success.
  * @return   kGeneralErr   : if an error occurred
  */
OSStatus lega_rtos_set_semaphore(lega_semaphore_t *semaphore);

/** @brief    Get (wait/decrement) a semaphore
  *
  * @Details  Attempts to get (wait/decrement) a semaphore. If semaphore is at zero already,
  *           then the calling thread will be suspended until another thread sets the
  *           semaphore with @ref lega_rtos_set_semaphore
  *
  * @param    semaphore : a pointer to the semaphore handle
  * @param    timeout_ms: the number of milliseconds to wait before returning
  *
  * @return   kNoErr        : on success.
  * @return   kGeneralErr   : if an error occurred
  */
OSStatus lega_rtos_get_semaphore(lega_semaphore_t *semaphore, uint32_t timeout_ms);

/** @brief    De-initialise a semaphore
  *
  * @Details  Deletes a semaphore created with @ref lega_rtos_init_semaphore
  *
  * @param    semaphore : a pointer to the semaphore handle
  *
  * @return   kNoErr        : on success.
  * @return   kGeneralErr   : if an error occurred
  */
OSStatus lega_rtos_deinit_semaphore(lega_semaphore_t *semaphore);

/** @brief    if the task num pending by this semaphore is 0
  *
  * @Details  if the task num pending by this semaphore is 0
  *
  * @param    semaphore : a pointer to the semaphore handle
  *
  * @return   TRUE    : pending task num is 0
  * @return   FALSE   : pending task num is not 0
  */
OSBool lega_rtos_semaphore_pending_task_null(lega_semaphore_t *semaphore);

/**
  * @}
  */

/** @defgroup LEGA_RTOS_MUTEX LEGA RTOS Mutex Functions
  * @brief Provide management APIs for Mutex such as init,lock,unlock and dinit.
  * @{
  */

/** @brief    Initialises a mutex
  *
  * @Details  A mutex is different to a semaphore in that a thread that already holds
  *           the lock on the mutex can request the lock again (nested) without causing
  *           it to be suspended.
  *
  * @param    mutex : a pointer to the mutex handle to be initialised
  *
  * @return   kNoErr        : on success.
  * @return   kGeneralErr   : if an error occurred
  */
OSStatus lega_rtos_init_mutex(lega_mutex_t *mutex);

/** @brief    Obtains the lock on a mutex
  *
  * @Details  Attempts to obtain the lock on a mutex. If the lock is already held
  *           by another thead, the calling thread will be suspended until the mutex
  *           lock is released by the other thread.
  *
  * @param    mutex : a pointer to the mutex handle to be locked
  *
  * @return   kNoErr        : on success.
  * @return   kGeneralErr   : if an error occurred
  */
OSStatus lega_rtos_lock_mutex(lega_mutex_t *mutex, uint32_t timeout_ms);

/** @brief    Releases the lock on a mutex
  *
  * @Details  Releases a currently held lock on a mutex. If another thread
  *           is waiting on the mutex lock, then it will be resumed.
  *
  * @param    mutex : a pointer to the mutex handle to be unlocked
  *
  * @return   kNoErr        : on success.
  * @return   kGeneralErr   : if an error occurred
  */
OSStatus lega_rtos_unlock_mutex(lega_mutex_t *mutex);

/** @brief    De-initialise a mutex
  *
  * @Details  Deletes a mutex created with @ref lega_rtos_init_mutex
  *
  * @param    mutex : a pointer to the mutex handle
  *
  * @return   kNoErr        : on success.
  * @return   kGeneralErr   : if an error occurred
  */
OSStatus lega_rtos_deinit_mutex(lega_mutex_t *mutex);
/**
  * @}
  */

/** @defgroup LEGA_RTOS_QUEUE LEGA RTOS FIFO Queue Functions
  * @brief Provide management APIs for FIFO such as init,push,pop and dinit.
  * @{
  */

/** @brief    Initialises a FIFO queue
  *
  * @param    queue : a pointer to the queue handle to be initialised
  * @param    name  : a text string name for the queue (NULL is allowed)
  * @param    message_size : size in bytes of objects that will be held in the queue
  * @param    number_of_messages : depth of the queue - i.e. max number of objects in the queue
  *
  * @return   kNoErr        : on success.
  * @return   kGeneralErr   : if an error occurred
  */
OSStatus lega_rtos_init_queue(lega_queue_t *queue, const char *name, uint32_t message_size,
                              uint32_t number_of_messages);

/** @brief    Pushes an object onto a queue
  *
  * @param    queue : a pointer to the queue handle
  * @param    message : the object to be added to the queue. Size is assumed to be
  *                  the size specified in @ref lega_rtos_init_queue
  * @param    timeout_ms: the number of milliseconds to wait before returning
  *
  * @return   kNoErr        : on success.
  * @return   kGeneralErr   : if an error or timeout occurred
  */
OSStatus lega_rtos_push_to_queue(lega_queue_t *queue, void *message, uint32_t timeout_ms);

/** @brief    Pops an object off a queue
  *
  * @param    queue : a pointer to the queue handle
  * @param    message : pointer to a buffer that will receive the object being
  *                     popped off the queue. Size is assumed to be
  *                     the size specified in @ref lega_rtos_init_queue , hence
  *                     you must ensure the buffer is long enough or memory
  *                     corruption will result
  * @param    timeout_ms: the number of milliseconds to wait before returning
  *
  * @return   kNoErr        : on success.
  * @return   kGeneralErr   : if an error or timeout occurred
  */
OSStatus lega_rtos_pop_from_queue(lega_queue_t *queue, void *message, uint32_t timeout_ms);

/** @brief    De-initialise a queue created with @ref lega_rtos_init_queue
  *
  * @param    queue : a pointer to the queue handle
  *
  * @return   kNoErr        : on success.
  * @return   kGeneralErr   : if an error occurred
  */
OSStatus lega_rtos_deinit_queue(lega_queue_t *queue);

/** @brief    Check if a queue is empty
  *
  * @param    queue : a pointer to the queue handle
  *
  * @return   true  : queue is empty.
  * @return   false : queue is not empty.
  */
OSBool lega_rtos_is_queue_empty(lega_queue_t *queue);

/** @brief    Check if a queue is full
  *
  * @param    queue : a pointer to the queue handle
  *
  * @return   true  : queue is empty.
  * @return   false : queue is not empty.
  */
OSBool lega_rtos_is_queue_full(lega_queue_t *queue);

/**
  * @}
  */

/** @defgroup LEGA_RTOS_TIMER LEGA RTOS Timer Functions
  * @brief Provide management APIs for timer such as init,start,stop,reload and dinit.
  * @{
  */

#if 1
/**
  * @brief     Initialize a RTOS timer
  *
  * @note      Timer does not start running until @ref lega_start_timer is called
  *
  * @param     timer    : a pointer to the timer handle to be initialised
  * @param     time_ms  : Timer period in milliseconds
  * @param     function : the callback handler function that is called each time the
  *                       timer expires
  * @param     arg      : an argument that will be passed to the callback function
  *
  * @return    kNoErr        : on success.
  * @return    kGeneralErr   : if an error occurred
  */
#ifdef USE_TIMER_NAME
OSStatus lega_rtos_init_timer_name(lega_timer_t *timer, uint32_t time_ms, timer_handler_t function, void *arg,
                                   uint8_t  *name);
#define lega_rtos_init_timer(timer,time_ms,function,arg) lega_rtos_init_timer_name(timer,time_ms,function,arg,__FUNCTION__)
#else
OSStatus lega_rtos_init_timer(lega_timer_t *timer, uint32_t time_ms, timer_handler_t function, void *arg);
#endif

/** @brief    Starts a RTOS timer running
  *
  * @note     Timer must have been previously initialised with @ref lega_rtos_init_timer
  *
  * @param    timer    : a pointer to the timer handle to start
  *
  * @return   kNoErr        : on success.
  * @return   kGeneralErr   : if an error occurred
  */
OSStatus lega_rtos_start_timer(lega_timer_t *timer);

/** @brief    Stops a running RTOS timer
  *
  * @note     Timer must have been previously started with @ref lega_rtos_init_timer
  *
  * @param    timer    : a pointer to the timer handle to stop
  *
  * @return   kNoErr        : on success.
  * @return   kGeneralErr   : if an error occurred
  */
OSStatus lega_rtos_stop_timer(lega_timer_t *timer);

/** @brief    Reloads a RTOS timer that has expired
  *
  * @note     This is usually called in the timer callback handler, to
  *           reschedule the timer for the next period.
  *
  * @param    timer    : a pointer to the timer handle to reload
  *
  * @return   kNoErr        : on success.
  * @return   kGeneralErr   : if an error occurred
  */
OSStatus lega_rtos_reload_timer(lega_timer_t *timer);

/** @brief    De-initialise a RTOS timer
  *
  * @note     Deletes a RTOS timer created with @ref lega_rtos_init_timer
  *
  * @param    timer : a pointer to the RTOS timer handle
  *
  * @return   kNoErr        : on success.
  * @return   kGeneralErr   : if an error occurred
  */
OSStatus lega_rtos_deinit_timer(lega_timer_t *timer);

/** @brief    Check if an RTOS timer is running
  *
  * @param    timer : a pointer to the RTOS timer handle
  *
  * @return   true        : if running.
  * @return   false       : if not running
  */
OSBool lega_rtos_is_timer_running(lega_timer_t *timer);

/**
  * @brief    Gets time in miiliseconds since RTOS start
  *
  * @note:    Since this is only 32 bits, it will roll over every 49 days, 17 hours.
  *
  * @returns  Time in milliseconds since RTOS started.
  */
uint32_t lega_rtos_get_time(void);

/**
  * @}
  */

/** @brief    Suspend current thread for a specific time
 *
 * @param     num_ms : A time interval (Unit: millisecond)
 *
 * @return    kNoErr.
 */
OSStatus lega_rtos_delay_milliseconds(uint32_t num_ms);
#endif

#define lega_rtos_malloc(s) _lega_rtos_malloc(s, __FUNCTION__, __LINE__)
void *_lega_rtos_malloc(uint32_t xWantedSize, const char *function, uint32_t line);
void lega_rtos_free(void *mem);

void lega_system_reset(void);
/** @brief  return the current system version
 *  @return    system version
 */
const char *lega_rtos_get_system_version(void);
void lega_intrpt_enter(void);
void lega_intrpt_exit(void);
uint32_t lega_rtos_get_system_ticks(void);
uint32_t lega_rtos_int_disable(void);
void lega_rtos_int_enable(uint32_t int_mask);
void lega_rtos_systick_reconfig(void);
int lega_rtos_running(void);
OSStatus lega_rtos_get_threadinfo(lega_thread_t *thread, lega_threadinfo_t *info);

#endif // __LEGARTOS_H__