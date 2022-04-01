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

#include "flash.h"
#include "duet_flash.h"

/* Logic partition on flash devices */
hal_logic_partition_t hal_partitions[HAL_PARTITION_MAX];
extern const duet_logic_partition_t duet_partitions[];

void flash_partition_init(void)
{
    hal_partitions[HAL_PARTITION_BOOTLOADER].partition_owner            =
        duet_partitions[PARTITION_BOOTLOADER].partition_owner;
    hal_partitions[HAL_PARTITION_BOOTLOADER].partition_description      =
        duet_partitions[PARTITION_BOOTLOADER].partition_description;
    hal_partitions[HAL_PARTITION_BOOTLOADER].partition_start_addr       =
        duet_partitions[PARTITION_BOOTLOADER].partition_start_addr;
    hal_partitions[HAL_PARTITION_BOOTLOADER].partition_length           =
        duet_partitions[PARTITION_BOOTLOADER].partition_length;
    hal_partitions[HAL_PARTITION_BOOTLOADER].partition_options          =
        duet_partitions[PARTITION_BOOTLOADER].partition_options ;

    hal_partitions[HAL_PARTITION_PARAMETER_1].partition_owner            =
        duet_partitions[PARTITION_PARAMETER_1].partition_owner;
    hal_partitions[HAL_PARTITION_PARAMETER_1].partition_description      =
        duet_partitions[PARTITION_PARAMETER_1].partition_description;
    hal_partitions[HAL_PARTITION_PARAMETER_1].partition_start_addr       =
        duet_partitions[PARTITION_PARAMETER_1].partition_start_addr;
    hal_partitions[HAL_PARTITION_PARAMETER_1].partition_length           =
        duet_partitions[PARTITION_PARAMETER_1].partition_length;
    hal_partitions[HAL_PARTITION_PARAMETER_1].partition_options          =
        duet_partitions[PARTITION_PARAMETER_1].partition_options ;

    hal_partitions[HAL_PARTITION_PARAMETER_2].partition_owner            =
        duet_partitions[PARTITION_PARAMETER_2].partition_owner ;
    hal_partitions[HAL_PARTITION_PARAMETER_2].partition_description      =
        duet_partitions[PARTITION_PARAMETER_2].partition_description;
    hal_partitions[HAL_PARTITION_PARAMETER_2].partition_start_addr       =
        duet_partitions[PARTITION_PARAMETER_2].partition_start_addr;
    hal_partitions[HAL_PARTITION_PARAMETER_2].partition_length           =
        duet_partitions[PARTITION_PARAMETER_2].partition_length;
    hal_partitions[HAL_PARTITION_PARAMETER_2].partition_options          =
        duet_partitions[PARTITION_PARAMETER_2].partition_options;

    hal_partitions[HAL_PARTITION_PARAMETER_3].partition_owner            =
        duet_partitions[PARTITION_PARAMETER_3].partition_owner ;
    hal_partitions[HAL_PARTITION_PARAMETER_3].partition_description      =
        duet_partitions[PARTITION_PARAMETER_3].partition_description;
    hal_partitions[HAL_PARTITION_PARAMETER_3].partition_start_addr       =
        duet_partitions[PARTITION_PARAMETER_3].partition_start_addr;
    hal_partitions[HAL_PARTITION_PARAMETER_3].partition_length           =
        duet_partitions[PARTITION_PARAMETER_3].partition_length;
    hal_partitions[HAL_PARTITION_PARAMETER_3].partition_options          =
        duet_partitions[PARTITION_PARAMETER_3].partition_options;

    hal_partitions[HAL_PARTITION_PARAMETER_4].partition_owner            =
        duet_partitions[PARTITION_PARAMETER_4].partition_owner ;
    hal_partitions[HAL_PARTITION_PARAMETER_4].partition_description      =
        duet_partitions[PARTITION_PARAMETER_4].partition_description;
    hal_partitions[HAL_PARTITION_PARAMETER_4].partition_start_addr       =
        duet_partitions[PARTITION_PARAMETER_4].partition_start_addr;
    hal_partitions[HAL_PARTITION_PARAMETER_4].partition_length           =
        duet_partitions[PARTITION_PARAMETER_4].partition_length;
    hal_partitions[HAL_PARTITION_PARAMETER_4].partition_options          =
        duet_partitions[PARTITION_PARAMETER_4].partition_options;

    hal_partitions[HAL_PARTITION_APPLICATION].partition_owner            =
        duet_partitions[PARTITION_APPLICATION].partition_owner;
    hal_partitions[HAL_PARTITION_APPLICATION].partition_description      =
        duet_partitions[PARTITION_APPLICATION].partition_description;
    hal_partitions[HAL_PARTITION_APPLICATION].partition_start_addr       =
        duet_partitions[PARTITION_APPLICATION].partition_start_addr;
    hal_partitions[HAL_PARTITION_APPLICATION].partition_length           =
        duet_partitions[PARTITION_APPLICATION].partition_length;
    hal_partitions[HAL_PARTITION_APPLICATION].partition_options          =
        duet_partitions[PARTITION_APPLICATION].partition_options;

    hal_partitions[HAL_PARTITION_OTA_TEMP].partition_owner               =
        duet_partitions[PARTITION_OTA_TEMP].partition_owner;
    hal_partitions[HAL_PARTITION_OTA_TEMP].partition_description         =
        duet_partitions[PARTITION_OTA_TEMP].partition_description;
    hal_partitions[HAL_PARTITION_OTA_TEMP].partition_start_addr          =
        duet_partitions[PARTITION_OTA_TEMP].partition_start_addr;
    hal_partitions[HAL_PARTITION_OTA_TEMP].partition_length              =
        duet_partitions[PARTITION_OTA_TEMP].partition_length;
    hal_partitions[HAL_PARTITION_OTA_TEMP].partition_options             =
        duet_partitions[PARTITION_OTA_TEMP].partition_options;

#ifdef MS_CONFIG_OTA_SUPPORT
    hal_partitions[HAL_PARTITION_OTA_MCU].partition_owner                =
        duet_partitions[PARTITION_OTA_MCU].partition_owner;
    hal_partitions[HAL_PARTITION_OTA_MCU].partition_description          =
        duet_partitions[PARTITION_OTA_MCU].partition_description;
    hal_partitions[HAL_PARTITION_OTA_MCU].partition_start_addr           =
        duet_partitions[PARTITION_OTA_MCU].partition_start_addr;
    hal_partitions[HAL_PARTITION_OTA_MCU].partition_length               =
        duet_partitions[PARTITION_OTA_MCU].partition_length;
    hal_partitions[HAL_PARTITION_OTA_MCU].partition_options              =
        duet_partitions[PARTITION_OTA_MCU].partition_options ;

    hal_partitions[HAL_PARTITION_OTA_PARA].partition_owner               =
        duet_partitions[PARTITION_OTA_PARA].partition_owner;
    hal_partitions[HAL_PARTITION_OTA_PARA].partition_description         =
        duet_partitions[PARTITION_OTA_PARA].partition_description;
    hal_partitions[HAL_PARTITION_OTA_PARA].partition_start_addr          =
        duet_partitions[PARTITION_OTA_PARA].partition_start_addr;
    hal_partitions[HAL_PARTITION_OTA_PARA].partition_length              =
        duet_partitions[PARTITION_OTA_PARA].partition_length;
    hal_partitions[HAL_PARTITION_OTA_PARA].partition_options             =
        duet_partitions[PARTITION_OTA_PARA].partition_options ;

    hal_partitions[HAL_PARTITION_OTA_HEAD_PARA].partition_owner          =
        duet_partitions[PARTITION_OTA_HEAD_PARA].partition_owner;
    hal_partitions[HAL_PARTITION_OTA_HEAD_PARA].partition_description    =
        duet_partitions[PARTITION_OTA_HEAD_PARA].partition_description;
    hal_partitions[HAL_PARTITION_OTA_HEAD_PARA].partition_start_addr     =
        duet_partitions[PARTITION_OTA_HEAD_PARA].partition_start_addr;
    hal_partitions[HAL_PARTITION_OTA_HEAD_PARA].partition_length         =
        duet_partitions[PARTITION_OTA_HEAD_PARA].partition_length;
    hal_partitions[HAL_PARTITION_OTA_HEAD_PARA].partition_options        =
        duet_partitions[PARTITION_OTA_HEAD_PARA].partition_options ;
#endif
}

