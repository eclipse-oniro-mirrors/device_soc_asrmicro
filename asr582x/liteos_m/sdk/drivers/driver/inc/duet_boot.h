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

#ifndef _DUET_BOOT_H_
#define _DUET_BOOT_H_

#define AON_RST_CHECK_REG                   (ALWAYS_ON_REGFILE + 0x10)
#define AON_RST_CHECK_FLAG                  0x00000002
#define RET_RAM_SOFT_RST_FLAG               0xAA55A501
#define RET_RAM_DS_RST_FLAG                 0xAA55A502

#define BOOT_TYPE_MAGIC_ID      0x55AA5A00
#define PWR_ON_RST              (BOOT_TYPE_MAGIC_ID + 0x00)
#define HARDWARE_PIN_RST        (BOOT_TYPE_MAGIC_ID + 0x01)
#define SOFTWARE_RST            (BOOT_TYPE_MAGIC_ID + 0x02) // including NVIC_SystemReset and WDG RST
#define DEEP_SLEEP_RST          (BOOT_TYPE_MAGIC_ID + 0x03)
#define UNKNOWN_RST             (BOOT_TYPE_MAGIC_ID + 0xFF)

void duet_cfg_boot_type(void);
void duet_set_ds_boot_type(void);
uint32_t duet_get_boot_type(void);
#endif // _DUET_BOOT_H_