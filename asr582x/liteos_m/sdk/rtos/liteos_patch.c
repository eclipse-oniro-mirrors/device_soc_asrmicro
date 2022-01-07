#include "cmsis_os2.h"
#include "kal.h"
#include "los_event.h"
#include "los_membox.h"
#include "los_memory.h"
#include "los_interrupt.h"
#include "los_mux.h"
#include "los_queue.h"
#include "los_sem.h"
#include "los_swtmr.h"
#include "los_task.h"
#include "los_timer.h"
#include "los_debug.h"

UINT32 LOS_SemPendingNoTask(UINT32 semHandle)
{
    UINT32 intSave,retVal;
    LosSemCB *semOnCheck = GET_SEM(semHandle);

    if (semHandle >= LOSCFG_BASE_IPC_SEM_LIMIT) {
        return LOS_ERRNO_SEM_INVALID;
    }
    intSave = LOS_IntLock();
    if(LOS_ListEmpty(&semOnCheck->semList))
        retVal = LOS_OK;
    else
        retVal = LOS_NOK;
    LOS_IntRestore(intSave);
    return retVal;
}

osStatus_t osSemaphorePendingNoTask(osSemaphoreId_t semaphore_id)
{
    UINT32 uwRet;

    if (OS_INT_ACTIVE) {
        return osErrorISR;
    }

    if (semaphore_id == NULL) {
        return osErrorParameter;
    }

    uwRet = LOS_SemPendingNoTask(((LosSemCB *)semaphore_id)->semID);
    if(uwRet == LOS_OK)
        return osOK;
    else
        return osError;
}