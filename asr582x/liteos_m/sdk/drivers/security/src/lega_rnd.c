#include <stdio.h>
#include "lega_rnd.h"
#include "lega_hw_common.h"

/*****************************************************************************/
/**********************        Public Functions      *************************/
/*****************************************************************************/
void lega_rnd_init()
{
#if defined(ALIOS_SUPPORT) || defined(SYSTEM_SUPPORT_OS)
    int ret;
    ret = lega_rtos_lock_mutex(&C310HwMutex, LEGA_WAIT_FOREVER);
    if(ret != 0)
        printf("rnd lock err\r\n");
#endif
}

void lega_rnd_deinit()
{
#if defined(ALIOS_SUPPORT) || defined(SYSTEM_SUPPORT_OS)
    int ret;
    ret = lega_rtos_unlock_mutex(&C310HwMutex);
    if(ret != 0)
        printf("rnd unlock err\r\n");
#endif
}

int lega_RND_Instantiation(
                        CRYS_RND_Context_t   *rndContext_ptr,
                        CRYS_RND_WorkBuff_t  *rndWorkBuff_ptr
){
    return CRYS_RND_Instantiation(rndContext_ptr, rndWorkBuff_ptr);
}


int lega_RND_UnInstantiation(
                        CRYS_RND_Context_t *rndContext_ptr
){
    return CRYS_RND_UnInstantiation(rndContext_ptr);
}


int lega_RND_Reseeding(
                        CRYS_RND_Context_t   *rndContext_ptr,
                        CRYS_RND_WorkBuff_t  *rndWorkBuff_ptr
){
    return CRYS_RND_Reseeding(rndContext_ptr, rndWorkBuff_ptr);
}


int lega_RND_GenerateVector(
                            CRYS_RND_State_t *rndState_ptr,
                            uint16_t   outSizeBytes,
                            uint8_t   *out_ptr
){
    return CRYS_RND_GenerateVector(rndState_ptr, outSizeBytes, out_ptr);
}


int lega_RND_SetGenerateVectorFunc(
                    CRYS_RND_Context_t *rndContext_ptr,
                    SaSiRndGenerateVectWorkFunc_t rndGenerateVectFunc
){
    return CRYS_RND_SetGenerateVectorFunc(rndContext_ptr, rndGenerateVectFunc);
}


int lega_RND_GenerateVectorInRange(
                    CRYS_RND_Context_t *rndContext_ptr,
                    uint32_t  rndSizeInBits,
                    uint8_t  *maxVect_ptr,
                    uint8_t  *rndVect_ptr
){
    return CRYS_RND_GenerateVectorInRange(rndContext_ptr, rndSizeInBits, maxVect_ptr, rndVect_ptr);
}

int lega_RND_AddAdditionalInput(
                            CRYS_RND_Context_t *rndContext_ptr,
                            uint8_t *additonalInput_ptr,
                            uint16_t additonalInputSize
){
    return CRYS_RND_AddAdditionalInput(rndContext_ptr, additonalInput_ptr, additonalInputSize);
}

int lega_RND_EnterKatMode(
            CRYS_RND_Context_t *rndContext_ptr,
            uint8_t            *entrData_ptr,
            uint32_t            entrSize,
            uint8_t            *nonce_ptr,
            uint32_t            nonceSize,
            CRYS_RND_WorkBuff_t  *workBuff_ptr
){
    return CRYS_RND_EnterKatMode(rndContext_ptr, entrData_ptr, entrSize, nonce_ptr, nonceSize, workBuff_ptr);
}

void lega_RND_DisableKatMode(
                    CRYS_RND_Context_t   *rndContext_ptr
){
    CRYS_RND_DisableKatMode(rndContext_ptr);
}

