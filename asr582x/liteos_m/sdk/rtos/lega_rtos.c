
#include <string.h>
#include <stdlib.h>
#include "lega_rtos.h"
#include "duet_common.h"
#include "cmsis_os2.h"
#include "include/kal.h"
#include "target_config.h"

#ifdef LEGA_CM4
#include "lega_cm4.h"
#else
#ifdef DUET_CM4
#include "duet_cm4.h"
#endif
#endif

#define CM4_ICSR  0xE000ED04

#define COMPRESS_LEN(x) (sizeof(size_t))

///////////////////////
OSBool lega_rtos_is_in_interrupt_context(void)
{
    unsigned int state=0;

    state=(*(unsigned int *) CM4_ICSR) & 0x3FF;   //bit0:9 is IRQ NO.

    if(state !=0)    return TRUE;

    return  FALSE;
}

/*******************************************************************
* Name: lega_rtos_create_thread
* Description: this api call freeRTOS function "xTaskCreate" to create task
* input param: task priority, stack_size, function, name
* output param: thread
* return: OSSatus value
*******************************************************************/
OSStatus lega_rtos_create_thread( lega_thread_t* thread, uint8_t priority, const char* name, lega_thread_function_t function, uint32_t stack_size, lega_thread_arg_t arg )
{
    uint8_t autorun = 1;
    osThreadAttr_t attr = {0};
    // osRunThreadAttr_t *runAttr;
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
    attr.priority  = priority -5 ;
    // attr.priority = priority - 8;//application 32 means harmo adaptor 24, liteos prio 15.
    //here should add bound check!
    if (autorun == 1) {
        threadId = osThreadNew((osThreadFunc_t)function, (void *)arg, &attr);
        if (threadId == 0) {
            printf("thread %s create fail %d\r\n",name, (int)threadId);
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
OSStatus lega_rtos_delete_thread( lega_thread_t* thread )
{
    osThreadId_t threadId;
    if (thread == NULL) {
        threadId = osThreadGetId();
    }
    else
    {
        threadId = (osThreadId_t)(*(unsigned int *)thread);
    }

    if (osThreadTerminate(threadId) != 0) {
        printf("thread delete fail\r\n");
        return kGeneralErr;
    }

    *(unsigned int *)thread = 0;
    return kNoErr;
}
//////////////////////////////  semaphore  //////////////////////////////////////

/*******************************************************************
* Name:lega_rtos_init_semaphore
* Description: create a count semaphore
* input param: init value,
* output param: lega_semaphore_t* semaphore
* return: kNoErr or kGeneralErr;
*******************************************************************/
OSStatus lega_rtos_init_semaphore( lega_semaphore_t* semaphore, int value )
{
    osSemaphoreId_t semId;

    semId = osSemaphoreNew(0xFF, (uint32_t)value, 0);
    if (semId == 0) {
        printf("sema init fail %d\r\n", (int)semId);
        return kGeneralErr;
    }

    *semaphore = (lega_semaphore_t*)semId;
    return kNoErr;
}
/*******************************************************************
* Name:lega_rtos_get_semaphore
* Description: get semaphore value
* input param:semaphore, timeout_ms
* output param:
* return:
*******************************************************************/
OSStatus lega_rtos_get_semaphore( lega_semaphore_t* semaphore, uint32_t timeout_ms )
{
    osStatus_t ret;

    ret = osSemaphoreAcquire((osSemaphoreId_t)(*semaphore), timeout_ms);
    if (ret != osOK) {
        printf("sem get fail %d\r\n",ret);
        return kGeneralErr;
    }

    return kNoErr;
}

FLASH_COMMON2_SEG OSStatus lega_rtos_set_semaphore( lega_semaphore_t* semaphore )
{
    osStatus_t ret;

    ret = osSemaphoreRelease((osSemaphoreId_t)(*semaphore));
    if (ret != osOK) {
        printf("sema set fail %d\r\n",ret);
        return kGeneralErr;
    }

    return kNoErr;
}


OSStatus lega_rtos_deinit_semaphore( lega_semaphore_t* semaphore )
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

extern osStatus_t osSemaphorePendingNoTask(osSemaphoreId_t semaphore_id);
OSBool lega_rtos_semaphore_pending_task_null( lega_semaphore_t* semaphore )
{
    osStatus_t ret;

    ret = osSemaphorePendingNoTask((osSemaphoreId_t)(*semaphore));
    if(ret == osOK)
        return TRUE;
    else
        return FALSE;
}

OSStatus lega_rtos_init_mutex( lega_mutex_t* mutex )
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

OSStatus lega_rtos_lock_mutex( lega_mutex_t* mutex, uint32_t timeout_ms )
{
    osStatus_t ret;

    ret = osMutexAcquire((osMutexId_t)(*mutex), timeout_ms);
    if (ret != osOK){
        printf("mutex lock fail %d\r\n",ret);
       return kGeneralErr;
    }

    return kNoErr;
}

OSStatus lega_rtos_unlock_mutex( lega_mutex_t* mutex )
{
    osStatus_t ret;

    ret = osMutexRelease((osMutexId_t)(*mutex));
    if (ret != osOK){
        printf("mutex unlock fail %d",ret);
        return kGeneralErr;
    }

    return kNoErr;
}

OSStatus lega_rtos_deinit_mutex( lega_mutex_t* mutex )
{
    osStatus_t ret;

    ret = osMutexDelete((osMutexId_t)(*mutex));
    if (ret != osOK){
        printf("mutex deinit fail %d\r\n",ret);
        return kGeneralErr;
    }

    *mutex = NULL;
    return kNoErr;
}

OSStatus lega_rtos_init_queue( lega_queue_t* queue, const char* name, uint32_t message_size, uint32_t number_of_messages )
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


OSStatus lega_rtos_push_to_queue( lega_queue_t* queue, void* message, uint32_t timeout_ms )
{
    osStatus_t ret;

    if(lega_rtos_is_in_interrupt_context())
    {
        ret = osMessageQueuePut((osMessageQueueId_t)(*queue), message, 0, 0);
    }
    else
    {
    ret = osMessageQueuePut((osMessageQueueId_t)(*queue), message, 0, timeout_ms);
    }
    if (ret != osOK) {
        printf("push_to_queue fail:%d\r\n",ret);
        return kGeneralErr;
    }

    return kNoErr;
}

OSStatus lega_rtos_pop_from_queue( lega_queue_t* queue, void* message, uint32_t timeout_ms )
{
    osStatus_t ret;

    ret = osMessageQueueGet((osMessageQueueId_t)(*queue), message, 0, timeout_ms);
    if (ret != osOK) {
        if(timeout_ms)
            printf("pop_from_queue fail:%d\r\n", ret);
        return kGeneralErr;
    }

    return kNoErr;
}

OSStatus lega_rtos_deinit_queue( lega_queue_t* queue )
{
    osStatus_t ret;

    ret = osMessageQueueDelete((osMessageQueueId_t)(*queue));
    if (ret != osOK) {
        printf("queue deinit fail:%d\r\n", ret);
        return kGeneralErr;
    }

    return kNoErr;
}



OSBool lega_rtos_is_queue_empty( lega_queue_t* queue )
{
    OSBool ret;
    lega_rtos_declare_critical();

    if(!lega_rtos_is_in_interrupt_context())
    {
        lega_rtos_enter_critical();
    }

    if (osMessageQueueGetCount((osMessageQueueId_t)(*queue)) == 0) {
        ret = TRUE;
    }
    else {
        ret = FALSE;;
    }

    if(!lega_rtos_is_in_interrupt_context())
    {
        lega_rtos_exit_critical();
    }

    return ret;
}

OSBool lega_rtos_is_queue_full( lega_queue_t* queue )
{
    OSBool ret;
    lega_rtos_declare_critical();

    if(!lega_rtos_is_in_interrupt_context())
    {
        lega_rtos_enter_critical();
    }

    if (osMessageQueueGetSpace((osMessageQueueId_t)(*queue)) == 0) {
        ret = TRUE;
    }
    else {
        ret = FALSE;;
    }

    if(!lega_rtos_is_in_interrupt_context())
    {
        lega_rtos_exit_critical();
    }

    return ret;
}

#if 1

uint32_t lega_rtos_ms_to_ticks(uint32_t ms)
{
    uint32_t ticks;

    uint64_t padding;

    padding = 1000 / LOSCFG_BASE_CORE_TICK_PER_SECOND;
    padding = (padding > 0) ? (padding - 1) : 0;

    ticks = ((ms + padding) * LOSCFG_BASE_CORE_TICK_PER_SECOND) / 1000;
    return ticks;
}

#if 1
OSStatus lega_rtos_init_timer( lega_timer_t* timer, uint32_t time_ms, timer_handler_t function, void* arg)
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
OSStatus lega_rtos_init_timer_name( lega_timer_t* timer, uint32_t time_ms, timer_handler_t function, void* arg,uint8_t * name)
{
    if(!timer)
    {
        printf("%s %s timer err\n", __FUNCTION__, name);
        return kGeneralErr;
    }
    timer->function = function;
    if(!arg)
        timer->arg     = name;
    else
        timer->arg      = arg;

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


OSStatus lega_rtos_start_timer( lega_timer_t* timer )
{
    if(!timer)
    {
        printf("%s timer err\n", __FUNCTION__);
        return kGeneralErr;
    }
    if (KalTimerStart((KalTimerId)(timer->handle)) == KAL_OK) {
         //printf("rtos_start_timer 0x%x\r\n",timer);
        return kNoErr;
    }

    return kGeneralErr;
}

OSStatus lega_rtos_stop_timer( lega_timer_t* timer )
{
    if(!timer)
    {
        printf("%s timer err\n", __FUNCTION__);
        return kGeneralErr;
    }
    if (KalTimerStop((KalTimerId)(timer->handle)) == KAL_OK) {
        //printf("rtos_stop_timer\r\n");
        return kNoErr;
    }

    return kGeneralErr;
}

OSStatus lega_rtos_reload_timer( lega_timer_t* timer )
{
    if(!timer)
    {
        printf("%s timer err\n", __FUNCTION__);
        return kGeneralErr;
    }
    if (KalTimerStop((KalTimerId)(timer->handle)) != KAL_OK) {
        printf("rtos_stop_timer\r\n");
        return kGeneralErr;
    }

    if (KalTimerStart((KalTimerId)(timer->handle)) != KAL_OK) {
        printf("rtos_start_timer\r\n");
        return kGeneralErr;
    }

    return kNoErr;
}

OSStatus lega_rtos_deinit_timer( lega_timer_t* timer )
{
    if(!timer)
    {
        printf("%s timer err\n", __FUNCTION__);
        return kGeneralErr;
    }
    if (KalTimerStop((KalTimerId)(timer->handle)) != KAL_OK) {
        printf("rtos_stop_timer err\r\n");
        return kGeneralErr;
    }
    if (KalTimerDelete((KalTimerId)(timer->handle)) == KAL_OK) {

        //printf("rtos_deinit_timer\r\n");
        timer->handle = 0;
        return kNoErr;
    }
    //printf("rtos_deinit_timer err\r\n");
    timer->handle = 0;

    return kGeneralErr;
}

OSBool lega_rtos_is_timer_running( lega_timer_t* timer )
{
    if(!timer)
    {
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

uint32_t lega_rtos_get_time( void )
{
    return (uint32_t)lega_rtos_ticks_to_ms((unsigned int)osKernelGetTickCount());
}

uint32_t lega_rtos_get_system_ticks(void)
{
    return (uint32_t)osKernelGetTickCount();
}

OSStatus lega_rtos_delay_milliseconds( uint32_t num_ms )
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
void *_lega_rtos_malloc(uint32_t xWantedSize,const char * function, uint32_t line)
{
   //return aos_malloc(xWantedSize);
    void *p = malloc(xWantedSize);
    if(p)
    {
        lega_malloc_cnt++;
    }
    else
    {
        if (xWantedSize != 0)
            printf("rtos malloc %d fail  %s:%d\r\n",(unsigned int)xWantedSize,function,(unsigned int)line);
    }
    while(xWantedSize)
    {
        if(xWantedSize>=4)
        {
           xWantedSize-=4;
           * (int *)(p+ xWantedSize)=0;
        }
        else
        {
           xWantedSize--;
          * (char *)(p+ xWantedSize)=0;   // clear memory alloced
        }
    }
    return p;
}

void lega_rtos_free(void *mem)
{
    if(mem)
    {
        lega_malloc_cnt--;
    }
    else
    {
        printf("rtos free null\r\n");
        return;
    }

    free(mem);
}

void lega_rtos_realloc(void *mem, uint32_t xWantedSize)
{
    if(mem)
    {
        realloc(mem, xWantedSize);
    }
    else
    {
        printf("rtos realloc null\r\n");
    }
    return;
}

void *pvPortMalloc( uint32_t xWantedSize )
{
    return lega_rtos_malloc(xWantedSize);
}

void vPortFree( void *pv )
{
    lega_rtos_free(pv);
}

UINT32 HalHwiCreate(HWI_HANDLE_T hwiNum,
                    HWI_PRIOR_T hwiPrio,
                    HWI_MODE_T mode,
                    HWI_PROC_FUNC handler,
                    HWI_ARG_T arg)
{
    return ArchHwiCreate(hwiNum,hwiPrio,mode,handler,arg);
}

UINT32 HalIntLock(VOID)
{
    return ArchIntLock();
}

VOID HalIntRestore(UINT32 intSave)
{
    ArchIntRestore(intSave);
}