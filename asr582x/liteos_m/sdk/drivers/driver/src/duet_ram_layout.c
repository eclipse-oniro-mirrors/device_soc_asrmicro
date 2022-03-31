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

#include "duet.h"
#include "duet_ram_layout.h"

#define WIFI_ADC_SHARED (32 * 1024)

uint32_t duet_ram_layout_init(Tcm_Config_Type tcm_config, Wifi_Ram_Config_Type wifi_config,
                              Bt_Ram_Config_Type bt_config)
{
    uint32_t tmp_value, reg_tcm = 0, reg_wifi = 0, reg_ble = 0;

    if (tcm_config < 0 || tcm_config >= ITCM_DTCM_NUM) {
        return -1;
    }
    if (wifi_config < 0 || wifi_config >= WIFI_RAM_NUM) {
        return -1;
    }
    if (bt_config < 0 || bt_config >= BT_RAM_NUM) {
        return -1;
    }

    switch (tcm_config) {
        case ITCM_DTCM_32_192:
            reg_tcm = 0;
            break;
        case ITCM_DTCM_96_128:
            reg_tcm = 1;
            break;
        default:
            return -1;
    }

    switch (wifi_config) {
        case WIFI_RAM_0:
            reg_wifi = 0;
            break;
        case WIFI_RAM_32:
            reg_wifi = 1;
            break;
        case WIFI_RAM_64:
            reg_wifi = 2;
            break;
        case WIFI_RAM_96:
            reg_wifi = 4;
            break;
        default:
            return -1;
    }

    switch (bt_config) {
        case BT_RAM_0:
            reg_ble = 0;
            break;
        case BT_RAM_16:
            reg_ble = 1;
            break;
        case BT_RAM_32:
            reg_ble = 3;
            break;
        default:
            return -1;
    }

    tmp_value = REG_RD(0X40000000);
    REG_WR(0X40000000, (tmp_value & ~0x00000001) | reg_tcm);

    tmp_value = REG_RD(0X4000002C) & (~0x0000001f);
    tmp_value |= (reg_ble << 3);
    tmp_value |= (reg_wifi);
    REG_WR(0X4000002C, (tmp_value));

    return 0;
}

uint32_t duet_get_ram_layout(Ram_Layout_Type *ram_layout)
{
    uint32_t reg_tcm = 0, reg_wifi = 0, reg_bt = 0;

    if (ram_layout == 0) {
        return -1;
    }

    reg_tcm = REG_RD(0X40000000) & 0x00000001;
    if (reg_tcm == 0) {
        ram_layout->itcm_addr = 0x00080000;
        ram_layout->itcm_size = (32 * 1024);
        ram_layout->dtcm_addr = 0x20FD0000;
        ram_layout->dtcm_size = (192 * 1024);
    } else {
        ram_layout->itcm_addr = 0x00080000;
        ram_layout->itcm_size = (96 * 1024);
        ram_layout->dtcm_addr = 0x20FE0000;
        ram_layout->dtcm_size = (128 * 1024);
    }

    ram_layout->soc_addr  = 0x21000000;
    ram_layout->wifi_addr = 0x60000000;
    ram_layout->bt_addr   = 0x62008000;

    reg_wifi = REG_RD(0X4000002C) & 0x0000007;
    if (reg_wifi == 1) {
        ram_layout->wifi_size = (32 * 1024);
    } else if (reg_wifi == 2) {
        ram_layout->wifi_size = (64 * 1024);
    } else if (reg_wifi == 4) {
        ram_layout->wifi_size = (96 * 1024);
    } else {
        ram_layout->wifi_size = (0 * 1024);
    }

    reg_bt = (REG_RD(0X4000002C) & 0x00000018) >> 3;
    if (reg_bt == 1) {
        ram_layout->bt_size = (16 * 1024);
    } else if (reg_bt == 3) {
        ram_layout->bt_size = (32 * 1024);
    } else {
        ram_layout->bt_size = (0 * 1024);
    }

    ram_layout->soc_size = (128 * 1024) - ram_layout->wifi_size - ram_layout->bt_size;
    ram_layout->wifi_size += WIFI_ADC_SHARED;

    return 0;
}