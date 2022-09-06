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

#include "mbedtls/entropy_poll.h"

#ifdef MBEDTLS_ENTROPY_HARDWARE_ALT

#include "lega_rnd.h"
#include "lega_hw_common.h"

int mbedtls_hardware_poll(void *data,
                          unsigned char *output, size_t len, size_t *olen)
{
    int ret = 0;
    lega_rnd_init();
    ret = lega_RND_AddAdditionalInput(rndContext_ptr, "0123", 4);
    if (ret != LEGA_HW_OK) {
        printf("\n lega_RND_AddAdditionalInput failed with 0x%x \n", ret);
        goto rnd_err;
    }
    ret = lega_RND_Reseeding (rndContext_ptr, rndWorkBuff_ptr);
    if (ret != LEGA_HW_OK) {
        printf("\n CRYS_RND_Reseeding failed with 0x%x \n", ret);
        goto rnd_err;
    }
    ret = lega_RND_GenerateVector(rndContext_ptr, len, output);
    if (ret != LEGA_HW_OK) {
        printf("\n CRYS_RND_GenerateVector for vector 1 failed with 0x%x \n", ret);
        goto rnd_err;
    }

    *olen = len;
    lega_rnd_deinit();
rnd_err:
    return ret;
}
#if 0
#define RND_MAX_LEN 192
#define RND_TEST_LEN 192
uint32_t mbedtls_hw_rand()
{
    uint32_t ret = 0;
    uint8_t buffer[RND_MAX_LEN] = {0};
    uint32_t olen = 0;
    mbedtls_hardware_poll("0", &buffer[0], RND_TEST_LEN, &olen);
    printf("data len = %d, data = \n", (int)olen);
    for (ret = 0; ret < RND_TEST_LEN; ret++) {
        printf("0x%x ", buffer[ret]);
    }
    printf("\n");
    return ret;
}
#endif
#endif