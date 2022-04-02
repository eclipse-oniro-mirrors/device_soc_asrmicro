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

#include <stdio.h>
#include "system_cm4.h"
#include "duet.h"
#include "duet_common.h"
#include "duet_uart.h"
#include "sns_silib.h"
#include "crys_rnd.h"
#include "crys_error.h"
#include "duet_hw_common.h"
#include "duet_rnd.h"

/*****************************************************************************/
/**********************        Public Functions      *************************/
/*****************************************************************************/

int duet_RND_Instantiation(CRYS_RND_Context_t *rndContext_ptr, CRYS_RND_WorkBuff_t *rndWorkBuff_ptr)
{
    return CRYS_RND_Instantiation(rndContext_ptr, rndWorkBuff_ptr);
}

int duet_RND_UnInstantiation(CRYS_RND_Context_t *rndContext_ptr)
{
    return CRYS_RND_UnInstantiation(rndContext_ptr);
}

int duet_RND_Reseeding(CRYS_RND_Context_t *rndContext_ptr, CRYS_RND_WorkBuff_t *rndWorkBuff_ptr)
{
    return CRYS_RND_Reseeding(rndContext_ptr, rndWorkBuff_ptr);
}

int duet_RND_GenerateVector(CRYS_RND_State_t *rndState_ptr, uint16_t outSizeBytes, uint8_t *out_ptr)
{
    return CRYS_RND_GenerateVector(rndState_ptr, outSizeBytes, out_ptr);
}

int duet_RND_SetGenerateVectorFunc(CRYS_RND_Context_t *rndContext_ptr,
                                   SaSiRndGenerateVectWorkFunc_t rndGenerateVectFunc)
{
    return CRYS_RND_SetGenerateVectorFunc(rndContext_ptr, rndGenerateVectFunc);
}

int duet_RND_GenerateVectorInRange(CRYS_RND_Context_t *rndContext_ptr, uint32_t rndSizeInBits, uint8_t *maxVect_ptr,
                                   uint8_t *rndVect_ptr)
{
    return CRYS_RND_GenerateVectorInRange(rndContext_ptr, rndSizeInBits, maxVect_ptr, rndVect_ptr);
}

int duet_RND_AddAdditionalInput(CRYS_RND_Context_t *rndContext_ptr, uint8_t *additonalInput_ptr,
                                uint16_t additonalInputSize)
{
    return CRYS_RND_AddAdditionalInput(rndContext_ptr, additonalInput_ptr, additonalInputSize);
}

int duet_RND_EnterKatMode(CRYS_RND_Context_t *rndContext_ptr, uint8_t *entrData_ptr, uint32_t entrSize, uint8_t
                          *nonce_ptr, uint32_t nonceSize, CRYS_RND_WorkBuff_t *workBuff_ptr)
{
    return CRYS_RND_EnterKatMode(rndContext_ptr, entrData_ptr, entrSize, nonce_ptr, nonceSize, workBuff_ptr);
}

void duet_RND_DisableKatMode(CRYS_RND_Context_t *rndContext_ptr)
{
    CRYS_RND_DisableKatMode(rndContext_ptr);
}

