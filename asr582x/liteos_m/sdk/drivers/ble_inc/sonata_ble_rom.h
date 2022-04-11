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

/**
 ****************************************************************************************
 *
 * @file sonata_ble_rom.h
 *
 * @brief This file contains the declare of the rom
 *
 ****************************************************************************************
 */

#ifndef _SONATA_BLE_ROM_H_
#define _SONATA_BLE_ROM_H_

#if BLE_ROM
/*
 * INCLUDE FILES
 *****************************************************************************************
 */
#include <stddef.h>    // standard definitions
#include <stdint.h>    // standard integer definition
#include <stdbool.h>   // boolean definition

#define DATA_SEG_INIT   0x01
#define BSS_SEG_INIT    0x02
#define FUNC_SEG_INIT   0x04
#define ALL_SEG_INIT    (DATA_SEG_INIT|BSS_SEG_INIT|FUNC_SEG_INIT)
/*
 * FUNCTION DECLARATION
 *****************************************************************************************
 */
extern void sonata_ble_rom_init(uint8_t init);

#endif // BLE_ROM

#endif // _SONATA_BLE_ROM_H_

