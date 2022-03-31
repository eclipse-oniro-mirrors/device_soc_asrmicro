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

#include "duet_cm4.h"
#include "duet_common.h"
#include "duet.h"
#include "duet_boot.h"

void duet_cfg_boot_type(void)
{
    uint32_t flag1 = RETENTION_SRAM->BOOT_CFG;
    uint32_t flag2 = REG_RD(AON_RST_CHECK_REG);
    if ((RET_RAM_SOFT_RST_FLAG == flag1) && (AON_RST_CHECK_FLAG == (flag2 & AON_RST_CHECK_FLAG))) {
        RETENTION_SRAM->BOOT_TYPE = SOFTWARE_RST;
    } else if ((RET_RAM_DS_RST_FLAG == flag1) && (AON_RST_CHECK_FLAG == (flag2 & AON_RST_CHECK_FLAG))) {
        RETENTION_SRAM->BOOT_CFG = RET_RAM_SOFT_RST_FLAG;
        RETENTION_SRAM->BOOT_TYPE = DEEP_SLEEP_RST;
    } else if ((RET_RAM_SOFT_RST_FLAG == flag1) && (0 == flag2)) {
        REG_WR(AON_RST_CHECK_REG, AON_RST_CHECK_FLAG);
        RETENTION_SRAM->BOOT_TYPE = HARDWARE_PIN_RST;
    } else if (0 == flag2) {
        REG_WR(AON_RST_CHECK_REG, AON_RST_CHECK_FLAG);
        RETENTION_SRAM->BOOT_CFG = RET_RAM_SOFT_RST_FLAG;
        RETENTION_SRAM->BOOT_TYPE = PWR_ON_RST;
    } else {
        REG_WR(AON_RST_CHECK_REG, AON_RST_CHECK_FLAG);
        RETENTION_SRAM->BOOT_CFG = RET_RAM_SOFT_RST_FLAG;
        RETENTION_SRAM->BOOT_TYPE = UNKNOWN_RST;
    }
}

void duet_set_ds_boot_type(void)
{
    RETENTION_SRAM->BOOT_CFG = RET_RAM_DS_RST_FLAG;
}

uint32_t duet_get_boot_type(void)
{
    return (RETENTION_SRAM->BOOT_TYPE);
}
