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

#ifndef _DUET_RAM_LAYOUT_H_
#define _DUET_RAM_LAYOUT_H_

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
    ITCM_DTCM_32_192,   // default
    ITCM_DTCM_96_128,
    ITCM_DTCM_NUM
} Tcm_Config_Type;

typedef enum {
    WIFI_RAM_0,         // default
    WIFI_RAM_32,
    WIFI_RAM_64,
    WIFI_RAM_96,
    WIFI_RAM_NUM
} Wifi_Ram_Config_Type;

typedef enum {
    BT_RAM_0,           // default
    BT_RAM_16,
    BT_RAM_32,
    BT_RAM_NUM
} Bt_Ram_Config_Type;

typedef struct {
    uint32_t itcm_addr;
    uint32_t itcm_size;

    uint32_t dtcm_addr;
    uint32_t dtcm_size;

    uint32_t soc_addr;
    uint32_t soc_size;

    uint32_t wifi_addr;
    uint32_t wifi_size;

    uint32_t bt_addr;
    uint32_t bt_size;
} Ram_Layout_Type;

/**
 * ram layout init
 *
 * @note This function must be called before the using of wifi and bt ram
 *
 * @param[in]  tcm_config   The config type of tcm.
 * @param[in]  wifi_config  The config type of wifi.
 * @param[in]  bt_config    The config type of bluetooth.
 *
 * @return  0 : On success, EIO : If an error occurred with any step
*/
uint32_t duet_ram_layout_init(Tcm_Config_Type tcm_config, Wifi_Ram_Config_Type wifi_config,
                              Bt_Ram_Config_Type bt_config);

/**
 * Get the current ram layout parameters
 *
 * @param[out] ram_layout   The layout parameters of the hole tcm and ram.
 *
 * @return  0 : On success, EIO : If an error occurred with any step
*/
uint32_t duet_get_ram_layout(Ram_Layout_Type *ram_layout);

#ifdef __cplusplus
}
#endif

#endif // _DUET_RAM_LAYOUT_H_

