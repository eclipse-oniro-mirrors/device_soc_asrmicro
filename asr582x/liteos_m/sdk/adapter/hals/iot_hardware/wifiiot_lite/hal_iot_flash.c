/*
 * Copyright (c) 2020 Huawei Device Co., Ltd.
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

#include "iot_errno.h"
#include "iot_flash.h"
#include "duet_flash.h"
#include "duet_common.h"
#include "lega_rtos.h"

static int param_part = PARTITION_PARAMETER_2;
unsigned int IoTFlashRead(unsigned int flashOffset, unsigned int size, unsigned char *ramData)
{
    return duet_flash_read(param_part, (uint32_t *)&flashOffset, ramData, size);
}

unsigned int IoTFlashWrite(unsigned int flashOffset, unsigned int size,
                           const unsigned char *ramData, unsigned char doErase)
{
    int32_t ret;
    lega_rtos_declare_critical();
    lega_enter_critical_expble();

    if (doErase) {
        ret = duet_flash_erase_write(param_part, (uint32_t *)&flashOffset, ramData, size);
    } else {
        ret = duet_flash_write(param_part, (uint32_t *)&flashOffset, ramData, size);
    }

    lega_exit_critical_expble();
    return ret;
}

unsigned int IoTFlashErase(unsigned int flashOffset, unsigned int size)
{
    int32_t ret;
    lega_rtos_declare_critical();
    lega_enter_critical_expble();

    ret = duet_flash_erase(param_part, flashOffset, size);

    lega_exit_critical_expble();
    return ret;
}

unsigned int IoTFlashInit(void)
{
    return duet_flash_init();
}

unsigned int IoTFlashDeinit(void)
{
    return IOT_SUCCESS;
}
