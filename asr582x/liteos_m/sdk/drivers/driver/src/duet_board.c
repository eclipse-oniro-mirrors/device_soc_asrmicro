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
#include "duet_flash.h"
#include "duet_flash_alg.h"

/* Logic partition on flash devices */
const duet_logic_partition_t duet_partitions[] = {
    [PARTITION_BOOTLOADER] =
    {
        .partition_owner            = FLASH_EMBEDDED,
        .partition_description      = "Bootloader",
        .partition_start_addr       = BOOTLOADER_FLASH_START_ADDR,
        .partition_length           = BOOTLOADER_MAX_SIZE,    // 64k bytes
        .partition_options          = PAR_OPT_READ_EN | PAR_OPT_WRITE_DIS,
    },
    [PARTITION_PARAMETER_1] =
    {
        .partition_owner            = FLASH_EMBEDDED,
        .partition_description      = "Info",
        .partition_start_addr       = INFO_FLASH_START_ADDR,
        .partition_length           = INFO_MAX_SIZE, // 4k bytes
        .partition_options          = PAR_OPT_READ_EN | PAR_OPT_WRITE_EN,
    },
    [PARTITION_PARAMETER_2] =
    {
        .partition_owner            = FLASH_EMBEDDED,
        .partition_description      = "KV",
        .partition_start_addr       = KV_FLASH_START_ADDR,
        .partition_length           = KV_MAX_SIZE, // 12k bytes
        .partition_options          = PAR_OPT_READ_EN | PAR_OPT_WRITE_EN,
    },
    [PARTITION_PARAMETER_3] =
    {
        .partition_owner            = FLASH_EMBEDDED,
        .partition_description      = "MIDEA_INFO",
        .partition_start_addr       = MIDEA_INFO_START_ADDR,
        .partition_length           = MIDEA_INFO_MAX_SIZE, // 4k bytes
        .partition_options          = PAR_OPT_READ_EN | PAR_OPT_WRITE_EN,
    },
    [PARTITION_PARAMETER_4] =
    {
        .partition_owner            = FLASH_EMBEDDED,
        .partition_description      = "MIDEA_INFO_BKUP",
        .partition_start_addr       = MIDEA_INFO_BKUP_START_ADDR,
        .partition_length           = MIDEA_INFO_BKUP_MAX_SIZE, // 4k bytes
        .partition_options          = PAR_OPT_READ_EN | PAR_OPT_WRITE_EN,
    },
    [PARTITION_APPLICATION] =
    {
        .partition_owner            = FLASH_EMBEDDED,
        .partition_description      = "Application",
        .partition_start_addr       = APP_FLASH_START_ADDR,
        .partition_length           = APP_MAX_SIZE, // 768k bytes
        .partition_options          = PAR_OPT_READ_EN | PAR_OPT_WRITE_EN,
    },
    [PARTITION_OTA_TEMP] =
    {
        .partition_owner           = FLASH_EMBEDDED,
        .partition_description     = "OTA Storage",
        .partition_start_addr      = OTA_FLASH_START_ADDR,
        .partition_length          = OTA_MAX_SIZE, // 768k bytes
        .partition_options         = PAR_OPT_READ_EN | PAR_OPT_WRITE_EN,
    },
    [PARTITION_CUSTOM_1] =
    {
        .partition_owner           = FLASH_EMBEDDED,
        .partition_description     = "FLASH Customer1",
        .partition_start_addr      = CUST1_FLASH_START_ADDR,
        .partition_length          = CUST1_MAX_SIZE, // 12k bytes
        .partition_options         = PAR_OPT_READ_EN | PAR_OPT_WRITE_EN,
    },
#ifdef MS_CONFIG_OTA_SUPPORT
    [PARTITION_OTA_MCU] =
    {
        .partition_owner           = FLASH_EMBEDDED,
        .partition_description     = "OTA MCU",
        .partition_start_addr      = OTA_MCU_FLASH_START_ADDR,
        .partition_length          = OTA_MCU_MAX_SIZE, // 192k bytes
        .partition_options         = PAR_OPT_READ_EN | PAR_OPT_WRITE_EN,
    },
    [PARTITION_OTA_PARA] =
    {
        .partition_owner           = FLASH_EMBEDDED,
        .partition_description     = "OTA PARA",
        .partition_start_addr      = OTA_PARA_START_ADDR,
        .partition_length          = OTA_PARA_MAX_SIZE, // 4k bytes
        .partition_options         = PAR_OPT_READ_EN | PAR_OPT_WRITE_EN,
    },
    [PARTITION_OTA_HEAD_PARA] =
    {
        .partition_owner           = FLASH_EMBEDDED,
        .partition_description     = "OTA HEAD PARA",
        .partition_start_addr      = OTA_HEAD_PARA_START_ADDR,
        .partition_length          = OTA_HEAD_PARA_MAX_SIZE, // 4k bytes
        .partition_options         = PAR_OPT_READ_EN | PAR_OPT_WRITE_EN,
    },
#endif
    [PARTITION_PARAMETER_5] =
    {
        .partition_owner            = FLASH_EMBEDDED,
        .partition_description      = "NVDS", // offline log
        .partition_start_addr       = NVDS_FLASH_START_ADDR,
        .partition_length           = NVDS_MAX_SIZE, // 8k bytes
        .partition_options          = PAR_OPT_READ_EN | PAR_OPT_WRITE_EN,
    },
};
#ifdef LOW_LEVEL_FLASH_RW_SUPPORT
int32_t duet_flash_get_wifi_mac(flash_mac_addr_t *addr)
{
    int32_t inBufLen = sizeof(flash_mac_addr_t);
    duet_flash_kv_get(FLASH_MAC_ADDR_TOKEN_NAME, addr, &inBufLen);
    //    printf("%s:%d addr=%02x %02x %02x %02x %02x %02x token=0x%x\n",__FUNCTION__,__LINE__,addr->mac[0],addr->mac[1],addr->mac[2],addr->mac[3],addr->mac[4],addr->mac[5],addr->token);
    return 0;
}
int32_t duet_flash_set_wifi_mac(flash_mac_addr_t *addr)
{
    //    printf("%s:%d addr=%02x %02x %02x %02x %02x %02x token=0x%x\n",__FUNCTION__,__LINE__,addr->mac[0],addr->mac[1],addr->mac[2],addr->mac[3],addr->mac[4],addr->mac[5],addr->mac,addr->token);
    return duet_flash_kv_set(FLASH_MAC_ADDR_TOKEN_NAME, addr, sizeof(flash_mac_addr_t), 0);
}
#endif