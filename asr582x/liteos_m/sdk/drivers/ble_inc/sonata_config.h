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
 * @file ($HEADER_FILENAME)
 *
 * @brief
 *
 ****************************************************************************************
 */
#ifndef _SONATA_CONFIG_H_
#define _SONATA_CONFIG_H_

/*
 * INCLUDE FILES
 ****************************************************************************************
 */
#include "sonata_stack_fixed_config.h"

/*
 * MACRO DEFINES
 ****************************************************************************************
 */
#define UTIL_BIT(pos) (1UL<<(pos))

#define SONATA_KEY_LEN             (0x10)
#define SONATA_RAND_NB_LEN         (0x08)
#define SONATA_BD_ADDR_LEN         (6)

/*
 * VARIABLE DECLARATIONS
 ****************************************************************************************
 */

/// BD address type
enum sonata_addr_type {
    /// Public BD address
    SONATA_ADDR_PUBLIC                   = 0x00,
    /// Random BD Address
    SONATA_ADDR_RAND,
    /// Controller generates Resolvable Private Address based on the
    /// local IRK from resolving list. If resolving list contains no matching
    /// entry, use public address.
    SONATA_ADDR_RPA_OR_PUBLIC,
    /// Controller generates Resolvable Private Address based on the
    /// local IRK from resolving list. If resolving list contains no matching
    /// entry, use random address.
    SONATA_ADDR_RPA_OR_RAND,
    /// mask used to determine Address type in the air
    SONATA_ADDR_MASK                     = 0x01,
    /// mask used to determine if an address is an RPA
    SONATA_ADDR_RPA_MASK                 = 0x02,
    /// Random device address (controller unable to resolve)
    SONATA_ADDR_RAND_UNRESOLVED          = 0xFE,
    /// No address provided (anonymous advertisement)
    SONATA_ADDR_NONE                     = 0xFF,
};

/// Advertising filter policy
enum stack_adv_filter_policy {
    /// Allow both scan and connection requests from anyone
    SONATA_ADV_ALLOW_SCAN_ANY_CON_ANY    = 0x00,
    /// Allow both scan req from White List devices only and connection req from anyone
    SONATA_ADV_ALLOW_SCAN_WLST_CON_ANY,
    /// Allow both scan req from anyone and connection req from White List devices only
    SONATA_ADV_ALLOW_SCAN_ANY_CON_WLST,
    /// Allow scan and connection requests from White List devices only
    SONATA_ADV_ALLOW_SCAN_WLST_CON_WLST,
};

/// Random Address type
enum stack_random_addr_type {
    /// Mask over the 2 MSBs
    SONATA_BD_ADDR_RND_ADDR_TYPE_MSK = 0xC0,

    /// Static random address           - 11 (MSB->LSB)
    SONATA_BD_ADDR_STATIC      = 0xC0,
    /// Private non resolvable address  - 00 (MSB->LSB)
    SONATA_BD_ADDR_NON_RSLV    = 0x00,
    /// Private resolvable address      - 01 (MSB->LSB)
    SONATA_BD_ADDR_RSLV        = 0x40,
};

/// Constant defining the role
enum {
    /// Master role
    SONATA_ROLE_MASTER,
    /// Slave role
    SONATA_ROLE_SLAVE,
};

/// Privacy mode type
enum {
    /// Network privacy mode
    SONATA_PRIV_TYPE_NETWORK = 0x00,
    /// Device privacy mode
    SONATA_PRIV_TYPE_DEVICE  = 0x01,
};

/*
 * FUNCTION DECLARATIONS
 ****************************************************************************************
 */

#if defined(CFG_BLE) && defined(CFG_HOST)
#define STACK_BLE_HOST_PRESENT            1
#else // defined(CFG_BLE) && defined(CFG_HOST)
#define STACK_BLE_HOST_PRESENT            0
#endif // defined(CFG_BLE) && defined(CFG_HOST)

#define SONATA_TASK_ID_APP   (15)
#define SONATA_TASK_ID_BASS  (36)
#define SONATA_TASK_ID_HOGPD (38)

#define SONATA_GAP_MAX_NAME_SIZE            (0x20)

#define STACK_TASK_FIRST_MSG(task) ((uint16_t)((task) << 8))

#endif // _SONATA_CONFIG_H_
