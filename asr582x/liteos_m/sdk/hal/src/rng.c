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
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include "lega_rnd.h"
#include "lega_hw_common.h"
#ifdef ALIOS_SUPPORT
#include "lega_rhino.h"
#else
#include "lega_rtos.h"
#endif

int32_t hal_random_num_read(void *random, void *buf, int32_t bytes)
{
    uint8_t rndTest_AddInputData[16] = {0};     //the lenth <= 48 and a multiple of 4.
    uint8_t *rand_dat_ptr            = NULL;
    int32_t ret                      = 0;
    if ((bytes > 0xFFFF) || (buf == NULL))
        return -1;    //error
    rand_dat_ptr = (uint8_t *) buf;
    memset(rand_dat_ptr, 0, bytes);
    lega_rnd_init();
    ret = lega_RND_AddAdditionalInput(rndContext_ptr, rndTest_AddInputData, sizeof(rndTest_AddInputData));
    if (ret != LEGA_HW_OK)
    {
        ret = -1;
        goto RAND_NUM_READ_FAIL;
    }
    ret = lega_RND_Reseeding(rndContext_ptr, rndWorkBuff_ptr);
    if (ret != LEGA_HW_OK)
    {
        ret = -1;
        goto RAND_NUM_READ_FAIL;
    }
    ret = lega_RND_GenerateVector(rndContext_ptr, bytes, rand_dat_ptr);
    if (ret != LEGA_HW_OK)
        ret = -1;
RAND_NUM_READ_FAIL:
    lega_rnd_deinit();
    return ret;
}