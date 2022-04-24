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
 * @file sonata_gap.h
 *
 * @brief Header file - GAP.
 *
 ****************************************************************************************
 */
#ifndef SONATA_GAP_H_
#define SONATA_GAP_H_
/**
 ****************************************************************************************
 * @addtogroup HOST
 * @ingroup ROOT
 * @brief Bluetooth Low Energy Host
 *
 * The HOST layer of the stack contains the higher layer protocols (@ref ATT "ATT",
 * @ref SMP "SMP") and transport module (@ref L2C "L2C"). It also includes the Generic
 * Access Profile (@ref GAP "GAP"), used for scanning/connection operations.
 ****************************************************************************************
 */
/**
 ****************************************************************************************
 * @addtogroup GAP Generic Access Profile
 * @ingroup HOST
 * @brief Generic Access Profile.
 *
 * The GAP module is responsible for providing an API to the application in order to
 * configure the device in the desired mode (discoverable, connectable, etc.) and perform
 * required actions (scanning, connection, pairing, etc.). To achieve this, the GAP
 * interfaces with both the @ref SMP "SMP", @ref L2C "L2C" and the @ref CONTROLLER "CONTROLLER"
 *
 * @{
 ****************************************************************************************
 */

#include <stdint.h>
#include "compiler.h"

/*
 * DEFINES
 ****************************************************************************************
 */

// BD address length
#define SONATA_GAP_BD_ADDR_LEN       (6)
// LE Channel map length
#define SONATA_GAP_LE_CHNL_MAP_LEN   (0x05)
// LE Feature Flags Length
#define SONATA_GAP_LE_FEATS_LEN      (0x08)
// ADV Data and Scan Response length
#define SONATA_GAP_ADV_DATA_LEN      (0x1F)
#define SONATA_GAP_SCAN_RSP_DATA_LEN (0x1F)
// Random number length
#define SONATA_GAP_RAND_NB_LEN       (0x08)
// Key length
#define SONATA_GAP_KEY_LEN           (16)
// P256 Key Len
#define SONATA_GAP_P256_KEY_LEN      (0x20)

/***** AD Type Flag - Bit set *******/
// Limited discovery flag - AD Flag
#define SONATA_GAP_LE_LIM_DISCOVERABLE_FLG             0x01
// General discovery flag - AD Flag
#define SONATA_GAP_LE_GEN_DISCOVERABLE_FLG             0x02
// Legacy BT not supported - AD Flag
#define SONATA_GAP_BR_EDR_NOT_SUPPORTED                0x04
// Dual mode for controller supported (BR/EDR/LE) - AD Flag
#define SONATA_GAP_SIMUL_BR_EDR_LE_CONTROLLER          0x08
// Dual mode for host supported (BR/EDR/LE) - AD Flag
#define SONATA_GAP_SIMUL_BR_EDR_LE_HOST                0x10

/*********** GAP Miscellaneous Defines *************/
// Invalid activity index
#define SONATA_GAP_INVALID_ACTV_IDX                    0xFF
// Invalid connection index
#define SONATA_GAP_INVALID_CONIDX                      0xFF

// Invalid connection handle
#define SONATA_GAP_INVALID_CONHDL                      0xFFFF

// Connection interval min (N*1.250ms)
#define SONATA_GAP_CNX_INTERVAL_MIN            6       // (0x06)
// Connection interval Max (N*1.250ms)
#define SONATA_GAP_CNX_INTERVAL_MAX            3200    // (0xC80)
// Connection latency min (N*cnx evt)
#define SONATA_GAP_CNX_LATENCY_MIN             0       // (0x00)
// Connection latency Max (N*cnx evt
#define SONATA_GAP_CNX_LATENCY_MAX             499     // (0x1F3)
// Supervision TO min (N*10ms)
#define SONATA_GAP_CNX_SUP_TO_MIN              10      // (0x0A)
// Supervision TO Max (N*10ms)
#define SONATA_GAP_CNX_SUP_TO_MAX              3200    // (0xC80)

// Length of resolvable random address prand part
#define SONATA_GAP_ADDR_PRAND_LEN            (3)
// Length of resolvable random address hash part
#define SONATA_GAP_ADDR_HASH_LEN             (3)

// Number of bytes needed for a bit field indicated presence of a given Advertising Flag value
// in the Advertising or the Scan Response data
// Advertising Flags is a 8-bit value, hence 256 value are possible
// -> 256 / 8 bytes = 32 bytes are needed
#define SONATA_GAP_AD_TYPE_BITFIELD_BYTES                          (32)

/*
 * DEFINES - Optional for BLE application usage
 ****************************************************************************************
 */

// Central idle timer
// TGAP(conn_pause_central)
// recommended value: 1 s: (100 for ke timer)
#define SONATA_GAP_TMR_CONN_PAUSE_CT                               0x0064

// Minimum time upon connection establishment before the peripheral
// starts a connection update procedure: TGAP(conn_pause_peripheral)
// recommended value: 5 s: (500 for ke timer)
#define SONATA_GAP_TMR_CONN_PAUSE_PH                               0x01F4

// Minimum time to perform scanning when user initiated
// TGAP(scan_fast_period)
// recommended value: 30.72 s: (3072 for ke timer)
#define SONATA_GAP_TMR_SCAN_FAST_PERIOD                            0x0C00

// Minimum time to perform advertising when user initiated
// TGAP(adv_fast_period)
// recommended value: 30 s: (3000 for ke timer)
#define SONATA_GAP_TMR_ADV_FAST_PERIOD                             0x0BB8

// Scan interval used during Link Layer Scanning State when
// performing the Limited Discovery procedure
// TGAP(lim_disc_scan_int)
// recommended value: 11.25ms; (18 decimal)
#define SONATA_GAP_LIM_DISC_SCAN_INT                               0x0012

// Scan interval in any discovery or connection establishment
// procedure when user initiated: TGAP(scan_fast_interval)
// recommended value: 30 to 60 ms; N * 0.625
#define SONATA_GAP_SCAN_FAST_INTV                                  0x0030

// Scan window in any discovery or connection establishment
// procedure when user initiated: TGAP(scan_fast_window)
// recommended value: 30 ms; N * 0.625
#define SONATA_GAP_SCAN_FAST_WIND                                  0x0030

// Scan interval in any discovery or connection establishment
// procedure when background scanning: TGAP(scan_slow_interval1)
// recommended value: 1.28 s : 0x00CD (205); N * 0.625
#define SONATA_GAP_SCAN_SLOW_INTV1                                 0x00CD

// Scan interval in any discovery or connection establishment
// procedure when background scanning: TGAP(scan_slow_interval2)
// recommended value: 2.56 s : 0x019A (410); N * 0.625
#define SONATA_GAP_SCAN_SLOW_INTV2                                 0x019A

// Scan window in any discovery or connection establishment
// procedure when background scanning: TGAP(scan_slow_window1)
// recommended value: 11.25 ms : 0x0012 (18); N * 0.625
#define SONATA_GAP_SCAN_SLOW_WIND1                                 0x0012

// Scan window in any discovery or connection establishment
// procedure when background scanning: TGAP(scan_slow_window2)
// recommended value: 22.5 ms : 0x0024 (36); N * 0.625
#define SONATA_GAP_SCAN_SLOW_WIND2                                 0x0024

// Minimum to maximum advertisement interval in any discoverable
// or connectable mode when user initiated: TGAP(adv_fast_interval1)
// recommended value: 30 to 60 ms; N * 0.625
#define SONATA_GAP_ADV_FAST_INTV1                                  0x0030

// Minimum to maximum advertisement interval in any discoverable
// or connectable mode when user initiated: TGAP(adv_fast_interval2)
// recommended value: 100 to 150 ms; N * 0.625
#define SONATA_GAP_ADV_FAST_INTV2                                  0x0064

// Minimum to maximum advertisement interval in any discoverable or
// connectable mode when background advertising: TGAP(adv_slow_interval)
// recommended value: 1 to 1.2 s : 0x00B0 (176); N * 0.625
#define SONATA_GAP_ADV_SLOW_INTV                                   0x00B0

// Minimum to maximum connection interval upon any connection
// establishment: TGAP(initial_conn_interval)
// recommended value: 30 to 50 ms ; N * 1.25 ms
#define SONATA_GAP_INIT_CONN_MIN_INTV                              0x0018
#define SONATA_GAP_INIT_CONN_MAX_INTV                              0x0028

// RW Defines
#define SONATA_GAP_INQ_SCAN_INTV                                   0x0012
#define SONATA_GAP_INQ_SCAN_WIND                                   0x0012

// Connection supervision timeout
// recommended value: 20s
#define SONATA_GAP_CONN_SUPERV_TIMEOUT                             0x07D0

// Minimum connection event
// default value: 0x0000
#define SONATA_GAP_CONN_MIN_CE                                     0x0000

// Maximum connection event
// default value: 0xFFFF
#define SONATA_GAP_CONN_MAX_CE                                     0xFFFF

// Connection latency
// default value: 0x0000
#define SONATA_GAP_CONN_LATENCY                                    0x0000

// GAP Device name Characteristic
// Default device name
#define SONATA_GAP_DEV_NAME_DEFAULT                                        "ASR-BLE"

// GAP Appearance or Icon Characteristic - 2 octets
// Current appearance value is 0x0000 (unknown appearance)
// Description:
// http:// developer.bluetooth.org/gatt/characteristics/Pages/CharacteristicViewer.aspx?u=org.bluetooth.characteristic.gap.appearance.xml
#define SONATA_GAP_APPEARANCE                                      0x0000

/// GAP Peripheral Preferred Connection Parameter - 8 octets
#define SONATA_GAP_PPCP_CONN_INTV_MAX                              0x0064
#define SONATA_GAP_PPCP_CONN_INTV_MIN                              0x00C8
#define SONATA_GAP_PPCP_SLAVE_LATENCY                              0x0000
#define SONATA_GAP_PPCP_STO_MULT                                   0x07D0

/*
 * Macros
 ****************************************************************************************
 */

#define SONATA_GAP_AD_TYPE_SET_BIT(bitfield, adv_flag)                             \
    bitfield[(adv_flag) / 8] |= CO_BIT((adv_flag) % 8)

#define SONATA_GAP_AD_TYPE_CHECK_BIT(bitfield, adv_flag)                           \
    (bitfield[(adv_flag) / 8] & CO_BIT((adv_flag) % 8))

/*
 * Enumerations
 ****************************************************************************************
 */
// GAP Advertising Flags
enum sonata_gap_ad_type {
    // Flag
    SONATA_GAP_AD_TYPE_FLAGS                      = 0x01, // !< SONATA_GAP_AD_TYPE_FLAGS
    // Use of more than 16 bits UUID
    SONATA_GAP_AD_TYPE_MORE_16_BIT_UUID           = 0x02, // !< SONATA_GAP_AD_TYPE_MORE_16_BIT_UUID
    // Complete list of 16 bit UUID
    SONATA_GAP_AD_TYPE_COMPLETE_LIST_16_BIT_UUID  = 0x03, // !< SONATA_GAP_AD_TYPE_COMPLETE_LIST_16_BIT_UUID
    // Use of more than 32 bit UUD
    SONATA_GAP_AD_TYPE_MORE_32_BIT_UUID           = 0x04, // !< SONATA_GAP_AD_TYPE_MORE_32_BIT_UUID
    // Complete list of 32 bit UUID
    SONATA_GAP_AD_TYPE_COMPLETE_LIST_32_BIT_UUID  = 0x05, // !< SONATA_GAP_AD_TYPE_COMPLETE_LIST_32_BIT_UUID
    // Use of more than 128 bit UUID
    SONATA_GAP_AD_TYPE_MORE_128_BIT_UUID          = 0x06, // !< SONATA_GAP_AD_TYPE_MORE_128_BIT_UUID
    // Complete list of 128 bit UUID
    SONATA_GAP_AD_TYPE_COMPLETE_LIST_128_BIT_UUID = 0x07, // !< SONATA_GAP_AD_TYPE_COMPLETE_LIST_128_BIT_UUID
    // Shortened device name
    SONATA_GAP_AD_TYPE_SHORTENED_NAME             = 0x08, // !< SONATA_GAP_AD_TYPE_SHORTENED_NAME
    // Complete device name
    SONATA_GAP_AD_TYPE_COMPLETE_NAME              = 0x09, // !< SONATA_GAP_AD_TYPE_COMPLETE_NAME
    // Transmit power
    SONATA_GAP_AD_TYPE_TRANSMIT_POWER             = 0x0A, // !< SONATA_GAP_AD_TYPE_TRANSMIT_POWER
    // Class of device
    SONATA_GAP_AD_TYPE_CLASS_OF_DEVICE            = 0x0D, // !< SONATA_GAP_AD_TYPE_CLASS_OF_DEVICE
    // Simple Pairing Hash C
    SONATA_GAP_AD_TYPE_SP_HASH_C                  = 0x0E, // !< SONATA_GAP_AD_TYPE_SP_HASH_C
    // Simple Pairing Randomizer
    SONATA_GAP_AD_TYPE_SP_RANDOMIZER_R            = 0x0F, // !< SONATA_GAP_AD_TYPE_SP_RANDOMIZER_R
    // Temporary key value
    SONATA_GAP_AD_TYPE_TK_VALUE                   = 0x10, // !< SONATA_GAP_AD_TYPE_TK_VALUE
    // Out of Band Flag
    SONATA_GAP_AD_TYPE_OOB_FLAGS                  = 0x11, // !< SONATA_GAP_AD_TYPE_OOB_FLAGS
    // Slave connection interval range
    SONATA_GAP_AD_TYPE_SLAVE_CONN_INT_RANGE       = 0x12, // !< SONATA_GAP_AD_TYPE_SLAVE_CONN_INT_RANGE
    // Require 16 bit service UUID
    SONATA_GAP_AD_TYPE_RQRD_16_BIT_SVC_UUID       = 0x14, // !< SONATA_GAP_AD_TYPE_RQRD_16_BIT_SVC_UUID
    // Require 32 bit service UUID
    SONATA_GAP_AD_TYPE_RQRD_32_BIT_SVC_UUID       = 0x1F, // !< SONATA_GAP_AD_TYPE_RQRD_32_BIT_SVC_UUID
    // Require 128 bit service UUID
    SONATA_GAP_AD_TYPE_RQRD_128_BIT_SVC_UUID      = 0x15, // !< SONATA_GAP_AD_TYPE_RQRD_128_BIT_SVC_UUID
    // Service data 16-bit UUID
    SONATA_GAP_AD_TYPE_SERVICE_16_BIT_DATA        = 0x16, // !< SONATA_GAP_AD_TYPE_SERVICE_16_BIT_DATA
    // Service data 32-bit UUID
    SONATA_GAP_AD_TYPE_SERVICE_32_BIT_DATA        = 0x20, // !< SONATA_GAP_AD_TYPE_SERVICE_32_BIT_DATA
    // Service data 128-bit UUID
    SONATA_GAP_AD_TYPE_SERVICE_128_BIT_DATA       = 0x21, // !< SONATA_GAP_AD_TYPE_SERVICE_128_BIT_DATA
    // Public Target Address
    SONATA_GAP_AD_TYPE_PUB_TGT_ADDR               = 0x17, // !< SONATA_GAP_AD_TYPE_PUB_TGT_ADDR
    // Random Target Address
    SONATA_GAP_AD_TYPE_RAND_TGT_ADDR              = 0x18, // !< SONATA_GAP_AD_TYPE_RAND_TGT_ADDR
    // Appearance
    SONATA_GAP_AD_TYPE_APPEARANCE                 = 0x19, // !< SONATA_GAP_AD_TYPE_APPEARANCE
    // Advertising Interval
    SONATA_GAP_AD_TYPE_ADV_INTV                   = 0x1A, // !< SONATA_GAP_AD_TYPE_ADV_INTV
    // LE Bluetooth Device Address
    SONATA_GAP_AD_TYPE_LE_BT_ADDR                 = 0x1B, // !< SONATA_GAP_AD_TYPE_LE_BT_ADDR
    // LE Role
    SONATA_GAP_AD_TYPE_LE_ROLE                    = 0x1C, // !< SONATA_GAP_AD_TYPE_LE_ROLE
    // Simple Pairing Hash C-256
    SONATA_GAP_AD_TYPE_SPAIR_HASH                 = 0x1D, // !< SONATA_GAP_AD_TYPE_SPAIR_HASH
    // Simple Pairing Randomizer R-256
    SONATA_GAP_AD_TYPE_SPAIR_RAND                 = 0x1E, // !< SONATA_GAP_AD_TYPE_SPAIR_RAND
    // 3D Information Data
    SONATA_GAP_AD_TYPE_3D_INFO                    = 0x3D, // !< SONATA_GAP_AD_TYPE_3D_INFO

    // Manufacturer specific data
    SONATA_GAP_AD_TYPE_MANU_SPECIFIC_DATA         = 0xFF, // !< SONATA_GAP_AD_TYPE_MANU_SPECIFIC_DATA
};

// Boolean value set
enum {
    // Disable
    SONATA_GAP_DISABLE = 0x00,
    // Enable
    SONATA_GAP_ENABLE
};

// GAP Attribute database handles
// Generic Access Profile Service
enum {
    SONATA_GAP_IDX_PRIM_SVC,
    SONATA_GAP_IDX_CHAR_DEVNAME,
    SONATA_GAP_IDX_DEVNAME,
    SONATA_GAP_IDX_CHAR_ICON,
    SONATA_GAP_IDX_ICON,
    SONATA_GAP_IDX_CHAR_SLAVE_PREF_PARAM,
    SONATA_GAP_IDX_SLAVE_PREF_PARAM,
    SONATA_GAP_IDX_CHAR_CNT_ADDR_RESOL,
    SONATA_GAP_IDX_CNT_ADDR_RESOL,
    SONATA_GAP_IDX_CHAR_RSLV_PRIV_ADDR_ONLY,
    SONATA_GAP_IDX_RSLV_PRIV_ADDR_ONLY,
    SONATA_GAP_IDX_NUMBER
};

// IO Capability Values
enum sonata_gap_io_cap {
    // Display Only
    SONATA_GAP_IO_CAP_DISPLAY_ONLY = 0x00,
    // Display Yes No
    SONATA_GAP_IO_CAP_DISPLAY_YES_NO,
    // Keyboard Only
    SONATA_GAP_IO_CAP_KB_ONLY,
    // No Input No Output
    SONATA_GAP_IO_CAP_NO_INPUT_NO_OUTPUT,
    // Keyboard Display
    SONATA_GAP_IO_CAP_KB_DISPLAY,
    SONATA_GAP_IO_CAP_LAST
};

// TK Type
enum sonata_gap_tk_type {
    //  TK get from out of band method
    SONATA_GAP_TK_OOB         = 0x00,
    // TK generated and shall be displayed by local device
    SONATA_GAP_TK_DISPLAY,
    // TK shall be entered by user using device keyboard
    SONATA_GAP_TK_KEY_ENTRY
};

// OOB Data Present Flag Values
enum sonata_gap_oob_auth {
    // OOB Data not present
    SONATA_GAP_OOB_AUTH_DATA_NOT_PRESENT = 0x00,
    // OOB data present
    SONATA_GAP_OOB_AUTH_DATA_PRESENT,
    SONATA_GAP_OOB_AUTH_DATA_LAST
};

// Authentication mask
enum sonata_gap_auth_mask {
    // No Flag set
    SONATA_GAP_AUTH_NONE    = 0,
    // Bond authentication
    SONATA_GAP_AUTH_BOND    = (1 << 0),
    // Man In the middle protection
    SONATA_GAP_AUTH_MITM    = (1 << 2),
    // Secure Connection
    SONATA_GAP_AUTH_SEC_CON = (1 << 3),
    // Key Notification
    SONATA_GAP_AUTH_KEY_NOTIF = (1 << 4)
};

// Security Link Level
enum sonata_gap_lk_sec_lvl {
    // No authentication
    SONATA_GAP_LK_NO_AUTH             = 0,
    // Unauthenticated link
    SONATA_GAP_LK_UNAUTH,
    // Authenticated link
    SONATA_GAP_LK_AUTH,
    // Secure Connection link
    SONATA_GAP_LK_SEC_CON,
};

// Authentication Requirements
enum sonata_gap_auth {
    // No MITM No Bonding
    SONATA_GAP_AUTH_REQ_NO_MITM_NO_BOND  = (SONATA_GAP_AUTH_NONE),
    // No MITM Bonding
    SONATA_GAP_AUTH_REQ_NO_MITM_BOND     = (SONATA_GAP_AUTH_BOND),
    // MITM No Bonding
    SONATA_GAP_AUTH_REQ_MITM_NO_BOND     = (SONATA_GAP_AUTH_MITM),
    // MITM and Bonding
    SONATA_GAP_AUTH_REQ_MITM_BOND        = (SONATA_GAP_AUTH_MITM | SONATA_GAP_AUTH_BOND),
    // SEC_CON and No Bonding
    SONATA_GAP_AUTH_REQ_SEC_CON_NO_BOND  = (SONATA_GAP_AUTH_SEC_CON | SONATA_GAP_AUTH_MITM),
    // SEC_CON and Bonding
    SONATA_GAP_AUTH_REQ_SEC_CON_BOND     = (SONATA_GAP_AUTH_SEC_CON | SONATA_GAP_AUTH_MITM | SONATA_GAP_AUTH_BOND),

    SONATA_GAP_AUTH_REQ_LAST,

    // Mask of  authentication features without reserved flag
    SONATA_GAP_AUTH_REQ_MASK             = 0x1F,
};

// Key Distribution Flags
enum sonata_gap_kdist {
    // No Keys to distribute
    SONATA_GAP_KDIST_NONE = 0x00,
    // Encryption key in distribution
    SONATA_GAP_KDIST_ENCKEY = (1 << 0),
    // IRK (ID key)in distribution
    SONATA_GAP_KDIST_IDKEY  = (1 << 1),
    // CSRK(Signature key) in distribution
    SONATA_GAP_KDIST_SIGNKEY = (1 << 2),
    // LTK in distribution
    SONATA_GAP_KDIST_LINKKEY =   (1 << 3),

    SONATA_GAP_KDIST_LAST =   (1 << 4)
};

// Security Defines
enum sonata_gap_sec_req {
    // No security (no authentication and encryption)
    SONATA_GAP_NO_SEC = 0x00,
    // Unauthenticated pairing with encryption
    SONATA_GAP_SEC1_NOAUTH_PAIR_ENC,
    // Authenticated pairing with encryption
    SONATA_GAP_SEC1_AUTH_PAIR_ENC,
    // Unauthenticated pairing with data signing
    SONATA_GAP_SEC2_NOAUTH_DATA_SGN,
    // Authentication pairing with data signing
    SONATA_GAP_SEC2_AUTH_DATA_SGN,
    // Secure Connection pairing with encryption
    SONATA_GAP_SEC1_SEC_CON_PAIR_ENC,
};

// Bit field use to select the preferred TX or RX LE PHY. 0 means no preferences
enum sonata_gap_phy {
    // No preferred PHY
    SONATA_GAP_PHY_ANY               = 0x00,
    // LE 1M PHY preferred for an active link
    SONATA_GAP_PHY_LE_1MBPS          = (1 << 0),
    // LE 2M PHY preferred for an active link
    SONATA_GAP_PHY_LE_2MBPS          = (1 << 1),
    // LE Coded PHY preferred for an active link
    SONATA_GAP_PHY_LE_CODED          = (1 << 2),
};

// Enumeration of TX/RX PHY values
enum sonata_gap_phy_val {
    // LE 1M PHY (TX or RX)
    SONATA_GAP_PHY_1MBPS        = 1,
    // LE 2M PHY (TX or RX)
    SONATA_GAP_PHY_2MBPS        = 2,
    // LE Coded PHY (RX Only)
    SONATA_GAP_PHY_CODED        = 3,
    // LE Coded PHY with S=8 data coding (TX Only)
    SONATA_GAP_PHY_125KBPS      = 3,
    // LE Coded PHY with S=2 data coding (TX Only)
    SONATA_GAP_PHY_500KBPS      = 4,
};

// Modulation index
enum sonata_gap_modulation_idx { // Assume transmitter will have a standard modulation index
    SONATA_GAP_MODULATION_STANDARD,
    // Assume transmitter will have a stable modulation index
    SONATA_GAP_MODULATION_STABLE,
};

// Packet Payload type for test mode
enum sonata_gap_pkt_pld_type { // PRBS9 sequence "11111111100000111101..." (in transmission order)
    SONATA_GAP_PKT_PLD_PRBS9,
    // Repeated "11110000" (in transmission order)
    SONATA_GAP_PKT_PLD_REPEATED_11110000,
    // Repeated "10101010" (in transmission order)
    SONATA_GAP_PKT_PLD_REPEATED_10101010,
    // PRBS15 sequence
    SONATA_GAP_PKT_PLD_PRBS15,
    // Repeated "11111111" (in transmission order) sequence
    SONATA_GAP_PKT_PLD_REPEATED_11111111,
    // Repeated "00000000" (in transmission order) sequence
    SONATA_GAP_PKT_PLD_REPEATED_00000000,
    // Repeated "00001111" (in transmission order) sequence
    SONATA_GAP_PKT_PLD_REPEATED_00001111,
    // Repeated "01010101" (in transmission order) sequence
    SONATA_GAP_PKT_PLD_REPEATED_01010101,
};

/*************** GAP Structures ********************/

// Device name
/* @TRACE */
struct sonata_gap_dev_name {
    // name length
    uint16_t length;
    // name value
    uint8_t value[__ARRAY_EMPTY];
};

// Slave preferred connection parameters
/* @TRACE */
struct sonata_gap_slv_pref {
    // Connection interval minimum
    uint16_t con_intv_min;
    // Connection interval maximum
    uint16_t con_intv_max;
    // Slave latency
    uint16_t slave_latency;
    // Connection supervision timeout multiplier
    uint16_t conn_timeout;
};

/// BD Address structure
/* @TRACE */
typedef struct {
    /// 6-byte array address value
    uint8_t  addr[SONATA_GAP_BD_ADDR_LEN];
} sonata_bd_addr_t;

/// Channel map structure
/* @TRACE */
typedef struct {
    /// 5-byte channel map array
    uint8_t map[SONATA_GAP_LE_CHNL_MAP_LEN];
} sonata_le_chnl_map_t;

/// Random number structure
/* @TRACE */
typedef struct {
    /// 8-byte array for random number
    uint8_t     nb[SONATA_GAP_RAND_NB_LEN];
} sonata_rand_nb_t;

// Address information about a device address
/* @TRACE */
struct sonata_gap_bdaddr {
    // BD Address of device
    sonata_bd_addr_t addr;
    // Address type of the device 0=public/1=private random
    uint8_t addr_type;
};

// Resolving list device information
/* @TRACE */
struct sonata_gap_ral_dev_info {
    // Device identity
    struct sonata_gap_bdaddr addr;
    // Privacy Mode
    uint8_t priv_mode;
    // Peer IRK
    uint8_t peer_irk[SONATA_GAP_KEY_LEN];
    // Local IRK
    uint8_t local_irk[SONATA_GAP_KEY_LEN];
};

// Generic Security key structure
/* @TRACE */
struct sonata_gap_sec_key {
    // Key value MSB -> LSB
    uint8_t key[SONATA_GAP_KEY_LEN];
};

// I/Q sample
/* @TRACE */
struct sonata_gap_iq_sample {
    // I sample
    int8_t i;
    // Q sample
    int8_t q;
};

typedef  uint16_t (*sonata_gap_check_pkt_type_t)(uint8_t pkt_type);

// @} GAP
#endif // SONATA_SONATA_GAP_H_
