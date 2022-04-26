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
 * @file sonata_gap_api.h
 *
 * @brief header file - gap api
 *
 ****************************************************************************************
 */

#ifndef _SONATA_GAP_API_H_
#define _SONATA_GAP_API_H_

/*
 * INCLUDE FILES
 ****************************************************************************************
 */
#include "sonata_config.h"
/// @cond
#if defined(STACK_BLE_HOST_PRESENT) || defined(BLE_HOST_PRESENT)
/// @endcond
#include "sonata_utils_api.h"
#include "sonata_gap.h"

/**
 * @defgroup SONATA_GAP_API GAP_API
 * @{
 */

/*
 * MACRO DEFINITIONS
 ****************************************************************************************
 */
/// Minimum Encryption key size
#define SONATA_GAP_SMP_MIN_ENC_SIZE_LEN                      (7)
/// Maximum Encryption Key size
#define SONATA_GAP_SMP_MAX_ENC_SIZE_LEN                     (16)

/// GAP Manager operation type - application interface
typedef enum {
    /* No Operation (if nothing has been requested)     */
    /* ************************************************ */
    //    /// No operation.
    //    SONATA_GAP_NO_OP                                     = 0x00,

    /* Default operations                               */
    /* ************************************************ */
    /// Reset BLE subsystem: LL and HL.
    SONATA_GAP_RESET                                     = 0x01,

    /* Configuration operations                         */
    /* ************************************************ */
    /// Set device configuration
    SONATA_GAP_SET_DEV_CONFIG                            = 0x03,
    /// Set device channel map
    SONATA_GAP_SET_CHANNEL_MAP                           = 0x04,

    /* Retrieve device information                      */
    /* ************************************************ */
    /// Get Local device version
    SONATA_GAP_GET_DEV_VERSION                           = 0x05,
    /// Get Local device BD Address
    SONATA_GAP_GET_DEV_BDADDR                            = 0x06,
    /// Get device advertising power level
    SONATA_GAP_GET_DEV_ADV_TX_POWER                      = 0x07,
    /// Get White List Size.
    SONATA_GAP_GET_WLIST_SIZE                            = 0x08,
    /// Retrieve Antenna information
    SONATA_GAP_GET_ANTENNA_INFO                          = 0x09,

    /* Security / Encryption Toolbox                    */
    /* ************************************************ */
    /// Resolve device address
    SONATA_GAP_RESOLV_ADDR                               = 0x17,
    /// Generate a random address
    SONATA_GAP_GEN_RAND_ADDR                             = 0x18,
    /// Use the controller's AES-128 block
    SONATA_GAP_USE_ENC_BLOCK                             = 0x19,
    /// Generate a 8-byte random number
    SONATA_GAP_GEN_RAND_NB                               = 0x1A,

    /* Profile Management                               */
    /* ************************************************ */
    /// Create new task for specific profile
    SONATA_GAP_PROFILE_TASK_ADD                          = 0x1B,

    /* DEBUG                                            */
    /* ************************************************ */
    /// Get memory usage
    SONATA_GAP_DBG_GET_MEM_INFO                          = 0x1C,
    /// Perform a platform reset
    SONATA_GAP_PLF_RESET                                 = 0x1D,

    /* Data Length Extension                            */
    /* ************************************************ */
    /// Set Suggested Default LE Data Length
    SONATA_GAP_SET_SUGGESTED_DFLT_LE_DATA_LEN            = 0x1E,
    /// Get Suggested Default LE Data Length
    SONATA_GAP_GET_SUGGESTED_DFLT_LE_DATA_LEN            = 0x1F,
    /// Get Maximum LE Data Length
    SONATA_GAP_GET_MAX_LE_DATA_LEN                       = 0x20,

    /* Operation on Resolving List                      */
    /* ************************************************ */
    /// Get resolving address list size
    SONATA_GAP_GET_RAL_SIZE                              = 0x21,
    /// Get resolving local address
    SONATA_GAP_GET_RAL_LOC_ADDR                          = 0x22,
    /// Get resolving peer address
    SONATA_GAP_GET_RAL_PEER_ADDR                         = 0x23,

    /* Change current IRK                               */
    /* ************************************************ */
    /// Set IRK
    SONATA_GAP_SET_IRK                                   = 0x28,

    /* LE Protocol/Service Multiplexer management       */
    /* ************************************************ */
    /// Register a LE Protocol/Service Multiplexer
    SONATA_GAP_LEPSM_REG                                 = 0x29,
    /// Unregister a LE Protocol/Service Multiplexer
    SONATA_GAP_LEPSM_UNREG                               = 0x2A,

    /* LE Direct Test Mode                              */
    /* ************************************************ */
    /// Stop the test mode
    SONATA_GAP_LE_TEST_STOP                              = 0x2B,
    /// Start RX Test Mode
    SONATA_GAP_LE_TEST_RX_START                          = 0x2C,
    /// Start TX Test Mode
    SONATA_GAP_LE_TEST_TX_START                          = 0x2D,

    /* Secure Connection                                */
    /* ************************************************ */
    /// Generate DH_Key
    SONATA_GAP_GEN_DH_KEY                                = 0x2E,
    /// Retrieve Public Key
    SONATA_GAP_GET_PUB_KEY                               = 0x2F,

    /* List Management                                  */
    /* ************************************************ */
    /// Set content of white list
    SONATA_GAP_SET_WL                                    = 0x90,
    /// Set content of resolving list
    SONATA_GAP_SET_RAL                                   = 0x91,
    /// Set content of periodic advertiser list
    SONATA_GAP_SET_PAL                                   = 0x92,
    /// Get periodic advertiser list size
    SONATA_GAP_GET_PAL_SIZE                              = 0x95,

    /* Air Operations                                   */
    /* ************************************************ */
    /// Create advertising activity
    SONATA_GAP_CREATE_ADV_ACTIVITY                       = 0xA0,
    /// Create scanning activity
    SONATA_GAP_CREATE_SCAN_ACTIVITY                      = 0xA1,
    /// Create initiating activity
    SONATA_GAP_CREATE_INIT_ACTIVITY                      = 0xA2,
    /// Create periodic synchronization activity
    SONATA_GAP_CREATE_PERIOD_SYNC_ACTIVITY               = 0xA3,
    /// Start an activity
    SONATA_GAP_START_ACTIVITY                            = 0xA4,
    /// Stop an activity
    SONATA_GAP_STOP_ACTIVITY                             = 0xA5,
    /// Stop all activities
    SONATA_GAP_STOP_ALL_ACTIVITIES                       = 0xA6,
    /// Delete an activity
    SONATA_GAP_DELETE_ACTIVITY                           = 0xA7,
    /// Delete all activities
    SONATA_GAP_DELETE_ALL_ACTIVITIES                     = 0xA8,
    /// Set advertising data
    SONATA_GAP_SET_ADV_DATA                              = 0xA9,
    /// Set scan response data
    SONATA_GAP_SET_SCAN_RSP_DATA                         = 0xAA,
    /// Set periodic advertising data
    SONATA_GAP_SET_PERIOD_ADV_DATA                       = 0xAB,
    /// Get number of available advertising sets
    SONATA_GAP_GET_NB_ADV_SETS                           = 0xAC,
    /// Get maximum advertising data length supported by the controller
    SONATA_GAP_GET_MAX_LE_ADV_DATA_LEN                   = 0xAD,
    /// Get minimum and maximum transmit powers supported by the controller
    SONATA_GAP_GET_DEV_TX_PWR                            = 0xAE,
    /// Get the RF Path Compensation values used in the TX Power Level and RSSI calculation
    SONATA_GAP_GET_DEV_RF_PATH_COMP                      = 0xAF,
    /// Enable/Disable reception of periodic advertising report
    SONATA_GAP_PER_ADV_REPORT_CTRL                       = 0xB0,
    /// Enable / Disable IQ sampling
    SONATA_GAP_PER_SYNC_IQ_SAMPLING_CTRL                 = 0xB1,
    /// Enable / Disable CTE transmission
    SONATA_GAP_PER_ADV_CTE_TX_CTL                        = 0xB2,

    /* Debug Commands                                   */
    /* ************************************************ */
    /// Configure the Debug Platform I&Q Sampling generator
    SONATA_GAP_DBG_IQGEN_CFG                             = 0x50,

    /* Internal Operations                              */
    /* ************************************************ */
    /// Renew random addresses
    SONATA_GAP_RENEW_ADDR                                = 0xF0,
} sonata_gap_operation;

/// Local device information type
typedef enum {
    /// Get Local device version
    SONATA_GET_DEV_VERSION                           = 0x05,
    /// Get Local device BD Address
    SONATA_GET_DEV_BDADDR                            = 0x06,
    /// Get device advertising power level
    SONATA_GET_DEV_ADV_TX_POWER                      = 0x07,
    /// Get White List Size.
    SONATA_GET_WLIST_SIZE                            = 0x08,
    /// Retrieve Antenna information
    SONATA_GET_ANTENNA_INFO                          = 0x09,
    /// Get memory usage
    SONATA_DBG_GET_MEM_INFO                          = 0x1C,
    /// Get Suggested Default LE Data Length
    SONATA_GET_SUGGESTED_DFLT_LE_DATA_LEN            = 0x1F,
    /// Get Maximum LE Data Length
    SONATA_GET_MAX_LE_DATA_LEN                       = 0x20,
    /// Get periodic advertiser list size
    SONATA_GET_PAL_SIZE                              = 0x95,
    /// Get resolving address list size
    SONATA_GET_RAL_SIZE                              = 0x21,
    /// Get number of available advertising sets
    SONATA_GET_NB_ADV_SETS                           = 0xAC,
    /// Get maximum advertising data length supported by the controller
    SONATA_GET_MAX_LE_ADV_DATA_LEN                   = 0xAD,
    /// Get minimum and maximum transmit powers supported by the controller
    SONATA_GET_DEV_TX_PWR                            = 0xAE,
    /// Get the RF Path Compensation values used in the TX Power Level and RSSI calculation
    SONATA_GET_DEV_RF_PATH_COMP                      = 0xAF,
} sonata_gap_local_dev_info;

/// List of device info that should be provided by application,peer device can get it
typedef enum {
    /// Device Name
    SONATA_GAP_DEV_NAME,
    /// Device Appearance Icon
    SONATA_GAP_DEV_APPEARANCE,
    /// Device Slave preferred parameters
    SONATA_GAP_DEV_SLV_PREF_PARAMS,
    /// Device Central address resolution
    SONATA_GAP_DEV_CTL_ADDR_RESOL,
    /// maximum device info parameter
    SONATA_GAP_DEV_INFO_MAX,
} sonata_gap_dev_info;

///// Option for PHY configuration
typedef enum {
    /// No preference for rate used when transmitting on the LE Coded PHY
    SONATA_GAP_PHY_OPT_LE_CODED_ALL_RATES     = (1 << 0),
    /// 500kbps rate preferred when transmitting on the LE Coded PHY
    SONATA_GAP_PHY_OPT_LE_CODED_500K_RATE     = (1 << 1),
    /// 125kbps  when transmitting on the LE Coded PHY
    SONATA_GAP_PHY_OPT_LE_CODED_125K_RATE     = (1 << 2),
} sonata_gap_phy_option;

/// Advertising properties bit field bit positions
typedef enum {
    /// Indicate that advertising is connectable, reception of CONNECT_REQ or AUX_CONNECT_REQ
    /// PDUs is accepted. Not applicable for periodic advertising.
    SONATA_GAP_ADV_PROP_CONNECTABLE_POS     = 0,
    SONATA_GAP_ADV_PROP_CONNECTABLE_BIT     = UTIL_BIT(SONATA_GAP_ADV_PROP_CONNECTABLE_POS),

    /// Indicate that advertising is scannable, reception of SCAN_REQ or AUX_SCAN_REQ PDUs is
    /// accepted
    SONATA_GAP_ADV_PROP_SCANNABLE_POS       = 1,
    SONATA_GAP_ADV_PROP_SCANNABLE_BIT       = UTIL_BIT(SONATA_GAP_ADV_PROP_SCANNABLE_POS),

    /// Indicate that advertising targets a specific device. Only apply in following cases:
    ///   - Legacy advertising: if connectable
    ///   - Extended advertising: connectable or (non connectable and non discoverable)
    SONATA_GAP_ADV_PROP_DIRECTED_POS        = 2,
    SONATA_GAP_ADV_PROP_DIRECTED_BIT        = UTIL_BIT(SONATA_GAP_ADV_PROP_DIRECTED_POS),

    /// Indicate that High Duty Cycle has to be used for advertising on primary channel
    /// Apply only if created advertising is not an extended advertising
    SONATA_GAP_ADV_PROP_HDC_POS             = 3,
    SONATA_GAP_ADV_PROP_HDC_BIT             = UTIL_BIT(SONATA_GAP_ADV_PROP_HDC_POS),

    /// Bit 4 is reserved
    SONATA_GAP_ADV_PROP_RESERVED_4_POS      = 4,
    SONATA_GAP_ADV_PROP_RESERVED_4_BIT      = UTIL_BIT(SONATA_GAP_ADV_PROP_RESERVED_4_POS),

    /// Enable anonymous mode. Device address won't appear in send PDUs
    /// Valid only if created advertising is an extended advertising
    SONATA_GAP_ADV_PROP_ANONYMOUS_POS       = 5,
    SONATA_GAP_ADV_PROP_ANONYMOUS_BIT       = UTIL_BIT(SONATA_GAP_ADV_PROP_ANONYMOUS_POS),

    /// Include TX Power in the extended header of the advertising PDU.
    /// Valid only if created advertising is not a legacy advertising
    SONATA_GAP_ADV_PROP_TX_PWR_POS          = 6,
    SONATA_GAP_ADV_PROP_TX_PWR_BIT          = UTIL_BIT(SONATA_GAP_ADV_PROP_TX_PWR_POS),

    /// Include TX Power in the periodic advertising PDU.
    /// Valid only if created advertising is a periodic advertising
    SONATA_GAP_ADV_PROP_PER_TX_PWR_POS      = 7,
    SONATA_GAP_ADV_PROP_PER_TX_PWR_BIT      = UTIL_BIT(SONATA_GAP_ADV_PROP_PER_TX_PWR_POS),

    /// Indicate if application must be informed about received scan requests PDUs
    SONATA_GAP_ADV_PROP_SCAN_REQ_NTF_EN_POS = 8,
    SONATA_GAP_ADV_PROP_SCAN_REQ_NTF_EN_BIT = UTIL_BIT(SONATA_GAP_ADV_PROP_SCAN_REQ_NTF_EN_POS),
} sonata_gap_adv_prop_bf;
/// Advertising discovery mode
typedef enum {
    /// Mode in non-discoverable
    SONATA_GAP_ADV_MODE_NON_DISC = 0,
    /// Mode in general discoverable
    SONATA_GAP_ADV_MODE_GEN_DISC,
    /// Mode in limited discoverable
    SONATA_GAP_ADV_MODE_LIM_DISC,
    /// Broadcast mode without presence of AD_TYPE_FLAG in advertising data
    SONATA_GAP_ADV_MODE_BEACON,
    SONATA_GAP_ADV_MODE_MAX,
} sonata_gap_adv_disc_mode;

/// Advertising properties configurations for legacy advertising
typedef enum {
    /// Non connectable non scannable advertising
    SONATA_GAP_ADV_PROP_NON_CONN_NON_SCAN_MASK  = 0x0000,
    /// Broadcast non scannable advertising - Discovery mode must be Non Discoverable
    SONATA_GAP_ADV_PROP_BROADCAST_NON_SCAN_MASK = SONATA_GAP_ADV_PROP_NON_CONN_NON_SCAN_MASK,
    /// Non connectable scannable advertising
    SONATA_GAP_ADV_PROP_NON_CONN_SCAN_MASK      = SONATA_GAP_ADV_PROP_SCANNABLE_BIT,
    /// Broadcast non scannable advertising - Discovery mode must be Non Discoverable
    SONATA_GAP_ADV_PROP_BROADCAST_SCAN_MASK     = SONATA_GAP_ADV_PROP_NON_CONN_SCAN_MASK,
    /// Undirected connectable advertising
    SONATA_GAP_ADV_PROP_UNDIR_CONN_MASK         = SONATA_GAP_ADV_PROP_CONNECTABLE_BIT | SONATA_GAP_ADV_PROP_SCANNABLE_BIT,
    /// Directed connectable advertising
    SONATA_GAP_ADV_PROP_DIR_CONN_MASK           = SONATA_GAP_ADV_PROP_DIRECTED_BIT | SONATA_GAP_ADV_PROP_CONNECTABLE_BIT,
    /// Directed connectable with Low Duty Cycle
    SONATA_GAP_ADV_PROP_DIR_CONN_LDC_MASK       = SONATA_GAP_ADV_PROP_DIR_CONN_MASK,
    /// Directed connectable with High Duty Cycle
    SONATA_GAP_ADV_PROP_DIR_CONN_HDC_MASK       = SONATA_GAP_ADV_PROP_DIR_CONN_MASK | SONATA_GAP_ADV_PROP_HDC_BIT,
} sonata_gap_leg_adv_prop;

/// Advertising properties configurations for extended advertising
typedef enum {
    /// Non connectable non scannable extended advertising
    SONATA_GAP_EXT_ADV_PROP_NON_CONN_NON_SCAN_MASK = 0x0000,
    /// Non connectable scannable extended advertising
    SONATA_GAP_EXT_ADV_PROP_NON_CONN_SCAN_MASK     = SONATA_GAP_ADV_PROP_SCANNABLE_BIT,
    /// Non connectable scannable directed extended advertising
    SONATA_GAP_EXT_ADV_PROP_NON_CONN_SCAN_DIR_MASK = SONATA_GAP_ADV_PROP_SCANNABLE_BIT | SONATA_GAP_ADV_PROP_DIRECTED_BIT,
    /// Non connectable anonymous directed extended advertising
    SONATA_GAP_EXT_ADV_PROP_ANONYM_DIR_MASK        = SONATA_GAP_ADV_PROP_ANONYMOUS_BIT | SONATA_GAP_ADV_PROP_DIRECTED_BIT,
    /// Undirected connectable extended advertising
    SONATA_GAP_EXT_ADV_PROP_UNDIR_CONN_MASK        = SONATA_GAP_ADV_PROP_CONNECTABLE_BIT,
    /// Directed connectable extended advertising
    SONATA_GAP_EXT_ADV_PROP_DIR_CONN_MASK          = SONATA_GAP_ADV_PROP_CONNECTABLE_BIT | SONATA_GAP_ADV_PROP_DIRECTED_BIT,
} sonata_gap_ext_adv_prop;

/// Own BD address source of the device
typedef enum {
    /// Public or Private Static Address according to device address configuration
    SONATA_GAP_STATIC_ADDR,
    /// Generated resolvable private random address
    SONATA_GAP_GEN_RSLV_ADDR,
    /// Generated non-resolvable private random address
    SONATA_GAP_GEN_NON_RSLV_ADDR,
} sonata_gap_own_addr;

/// Scanning Types
typedef enum {
    /// General discovery
    SONATA_GAP_SCAN_TYPE_GEN_DISC = 0,
    /// Limited discovery
    SONATA_GAP_SCAN_TYPE_LIM_DISC,
    /// Observer
    SONATA_GAP_SCAN_TYPE_OBSERVER,
    /// Selective observer
    SONATA_GAP_SCAN_TYPE_SEL_OBSERVER,
    /// Connectable discovery
    SONATA_GAP_SCAN_TYPE_CONN_DISC,
    /// Selective connectable discovery
    SONATA_GAP_SCAN_TYPE_SEL_CONN_DISC,
} sonata_gap_scan_type;

/// Privacy configuration
typedef enum {
    /// Indicate if identity address is a public (0) or static private random (1) address
    SONATA_GAP_PRIV_CFG_PRIV_ADDR_BIT = (1 << 0),
    /// Reserved
    SONATA_GAP_PRIV_CFG_RSVD          = (1 << 1),
    /// Indicate if controller privacy is enabled
    SONATA_GAP_PRIV_CFG_PRIV_EN_BIT   = (1 << 2),
} sonata_gap_priv_cfg;

/// Scanning properties bit field bit value
typedef enum {
    /// Scan advertisement on the LE 1M PHY
    SONATA_GAP_SCAN_PROP_PHY_1M_BIT       = (1 << 0),
    /// Scan advertisement on the LE Coded PHY
    SONATA_GAP_SCAN_PROP_PHY_CODED_BIT    = (1 << 1),
    /// Active scan on LE 1M PHY (Scan Request PDUs may be sent)
    SONATA_GAP_SCAN_PROP_ACTIVE_1M_BIT    = (1 << 2),
    /// Active scan on LE Coded PHY (Scan Request PDUs may be sent)
    SONATA_GAP_SCAN_PROP_ACTIVE_CODED_BIT = (1 << 3),
    /// Accept directed advertising packets if we use a RPA and target address cannot be solved by the
    /// controller
    SONATA_GAP_SCAN_PROP_ACCEPT_RPA_BIT   = (1 << 4),
    /// Filter truncated advertising or scan response reports
    SONATA_GAP_SCAN_PROP_FILT_TRUNC_BIT   = (1 << 5),
} sonata_gap_scan_prop;

/// Filtering policy for duplicated packets
typedef enum {
    /// Disable filtering of duplicated packets
    SONATA_GAP_DUP_FILT_DIS = 0,
    /// Enable filtering of duplicated packets
    SONATA_GAP_DUP_FILT_EN,
    /// Enable filtering of duplicated packets, reset for each scan period
    SONATA_GAP_DUP_FILT_EN_PERIOD,
} sonata_gap_dup_filter_pol;

/// Periodic synchronization types
typedef enum {
    /// Do not use periodic advertiser list for synchronization. Use advertiser information provided
    /// in the GAPM_ACTIVITY_START_CMD.
    SONATA_GAP_PER_SYNC_TYPE_GENERAL = 0,
    /// Use periodic advertiser list for synchronization
    SONATA_GAP_PER_SYNC_TYPE_SELECTIVE,
    /// Use Periodic advertising sync transfer information send through connection for synchronization
    SONATA_GAP_PER_SYNC_TYPE_PAST,
} sonata_gap_per_sync_type;

/// Initiating Types
typedef enum {
    /// Direct connection establishment, establish a connection with an indicated device
    SONATA_GAP_INIT_TYPE_DIRECT_CONN_EST = 0,
    /// Automatic connection establishment, establish a connection with all devices whose address is
    /// present in the white list
    SONATA_GAP_INIT_TYPE_AUTO_CONN_EST,
    /// Name discovery, Establish a connection with an indicated device in order to read content of its
    /// Device Name characteristic. Connection is closed once this operation is stopped.
    SONATA_GAP_INIT_TYPE_NAME_DISC,
} sonata_gap_init_type;

/// Initiating Properties
typedef enum {
    /// Scan connectable advertisements on the LE 1M PHY. Connection parameters for the LE 1M PHY are provided
    SONATA_GAP_INIT_PROP_1M_BIT       = (1 << 0),
    /// Connection parameters for the LE 2M PHY are provided
    SONATA_GAP_INIT_PROP_2M_BIT       = (1 << 1),
    /// Scan connectable advertisements on the LE Coded PHY. Connection parameters for the LE Coded PHY are provided
    SONATA_GAP_INIT_PROP_CODED_BIT    = (1 << 2),
} sonata_gap_init_prop;

/// Advertising report information
typedef enum {
    /// Report Type
    SONATA_GAP_REPORT_INFO_REPORT_TYPE_MASK    = 0x07,
    /// Report is complete
    SONATA_GAP_REPORT_INFO_COMPLETE_BIT        = (1 << 3),
    /// Connectable advertising
    SONATA_GAP_REPORT_INFO_CONN_ADV_BIT        = (1 << 4),
    /// Scannable advertising
    SONATA_GAP_REPORT_INFO_SCAN_ADV_BIT        = (1 << 5),
    /// Directed advertising
    SONATA_GAP_REPORT_INFO_DIR_ADV_BIT         = (1 << 6),
} sonata_gapm_adv_report_info;

/// gap role
typedef enum {
    /// No role set yet
    SONATA_GAP_ROLE_NONE        = 0x00,

    /// Observer role
    SONATA_GAP_ROLE_OBSERVER    = 0x01,

    /// Broadcaster role
    SONATA_GAP_ROLE_BROADCASTER = 0x02,

    /// Master/Central role
    SONATA_GAP_ROLE_CENTRAL     = (0x04 | SONATA_GAP_ROLE_OBSERVER),

    /// Peripheral/Slave role
    SONATA_GAP_ROLE_PERIPHERAL  = (0x08 | SONATA_GAP_ROLE_BROADCASTER),

    /// Device has all role, both peripheral and central
    SONATA_GAP_ROLE_ALL         = (SONATA_GAP_ROLE_CENTRAL | SONATA_GAP_ROLE_PERIPHERAL),
} sonata_gap_api_role_t;

/// Pairing mode authorized on the device
///    7    6    5    4    3    2    1    0
/// +----+----+----+----+----+----+----+----+
/// |               RFU           | SCP| LP |
/// +----+----+----+----+----+----+----+----+
typedef enum {
    /// No pairing authorized
    SONATA_GAP_PAIRING_DISABLE  = 0,
    /// Legacy pairing Authorized
    SONATA_GAP_PAIRING_LEGACY   = (1 << 0),
    /// Secure Connection pairing Authorized
    SONATA_GAP_PAIRING_SEC_CON  = (1 << 1),
} sonata_gap_pairing_mode;

/// profiles id define
typedef enum {
    SONATA_PRF_ID_DISS         = 20,    /* *< Device Information Service Server Task  */
    SONATA_PRF_ID_DISC         = 21,    /* *< Device Information Service Client Task  */

    SONATA_PRF_ID_PROXM        = 22,    /* *< Proximity Monitor Task  */
    SONATA_PRF_ID_PROXR        = 23,    /* *< Proximity Reporter Task  */

    SONATA_PRF_ID_FINDL        = 24,    /* *< Find Me Locator Task  */
    SONATA_PRF_ID_FINDT        = 25,    /* *< Find Me Target Task  */

    SONATA_PRF_ID_HTPC         = 26,    /* *< Health Thermometer Collector Task  */
    SONATA_PRF_ID_HTPT         = 27,    /* *< Health Thermometer Sensor Task  */

    SONATA_PRF_ID_BLPS         = 28,    /* *< Blood Pressure Sensor Task  */
    SONATA_PRF_ID_BLPC         = 29,    /* *< Blood Pressure Collector Task  */

    SONATA_PRF_ID_HRPS         = 30,    /* *< Heart Rate Sensor Task  */
    SONATA_PRF_ID_HRPC         = 31,    /* *< Heart Rate Collector Task  */

    SONATA_PRF_ID_TIPS         = 32,    /* *< Time Server Task  */
    SONATA_PRF_ID_TIPC         = 33,    /* *< Time Client Task  */

    SONATA_PRF_ID_SCPPS        = 34,    /* *< Scan Parameter Profile Server Task  */
    SONATA_PRF_ID_SCPPC        = 35,    /* *< Scan Parameter Profile Client Task  */

    SONATA_PRF_ID_BASS         = 36,    /* *< Battery Service Server Task  */
    SONATA_PRF_ID_BASC         = 37,    /* *< Battery Service Client Task  */

    SONATA_PRF_ID_HOGPD        = 38,    /* *< HID Device Task  */
    SONATA_PRF_ID_HOGPBH       = 39,    /* *< HID Boot Host Task  */
    SONATA_PRF_ID_HOGPRH       = 40,    /* *< HID Report Host Task  */

    SONATA_PRF_ID_GLPS         = 41,    /* *< Glucose Profile Sensor Task  */
    SONATA_PRF_ID_GLPC         = 42,    /* *< Glucose Profile Collector Task  */

    SONATA_PRF_ID_RSCPS        = 43,    /* *< Running Speed and Cadence Profile Server Task  */
    SONATA_PRF_ID_RSCPC        = 44,    /* *< Running Speed and Cadence Profile Collector Task  */

    SONATA_PRF_ID_CSCPS        = 45,    /* *< Cycling Speed and Cadence Profile Server Task  */
    SONATA_PRF_ID_CSCPC        = 46,    /* *< Cycling Speed and Cadence Profile Client Task  */

    SONATA_PRF_ID_ANPS         = 47,    /* *< Alert Notification Profile Server Task  */
    SONATA_PRF_ID_ANPC         = 48,    /* *< Alert Notification Profile Client Task  */

    SONATA_PRF_ID_PASPS        = 49,    /* *< Phone Alert Status Profile Server Task  */
    SONATA_PRF_ID_PASPC        = 50,    /* *< Phone Alert Status Profile Client Task  */

    SONATA_PRF_ID_CPPS         = 51,    /* *< Cycling Power Profile Server Task  */
    SONATA_PRF_ID_CPPC         = 52,    /* *< Cycling Power Profile Client Task  */

    SONATA_PRF_ID_LANS         = 53,    /* *< Location and Navigation Profile Server Task  */
    SONATA_PRF_ID_LANC         = 54,    /* *< Location and Navigation Profile Client Task  */

    SONATA_PRF_ID_IPSS         = 55,    /* *< Internet Protocol Support Profile Server Task  */
    SONATA_PRF_ID_IPSC         = 56,    /* *< Internet Protocol Support Profile Client Task  */

    SONATA_PRF_ID_ENVS         = 57,    /* *< Environmental Sensing Profile Server Task  */
    SONATA_PRF_ID_ENVC         = 58,    /* *< Environmental Sensing Profile Client Task  */

    SONATA_PRF_ID_WSCS         = 59,    /* *< Weight Scale Profile Server Task  */
    SONATA_PRF_ID_WSCC         = 60,    /* *< Weight Scale Profile Client Task  */

    SONATA_PRF_ID_UDSS         = 61,    /* *< User Data Service Server Task  */
    SONATA_PRF_ID_UDSC         = 62,    /* *< User Data Service Client Task  */

    SONATA_PRF_ID_BCSS         = 63,    /* *< Body Composition Server Task  */
    SONATA_PRF_ID_BCSC         = 64,    /* *< Body Composition Client Task  */

    SONATA_PRF_ID_WPTS         = 65,    /* *< Wireless Power Transfer Profile Server Task  */
    SONATA_PRF_ID_WPTC         = 66,    /* *< Wireless Power Transfer Profile Client Task  */

    SONATA_PRF_ID_PLXS         = 67,    /* *< Pulse Oximeter Profile Server Task  */
    SONATA_PRF_ID_PLXC         = 68,    /* *< Pulse Oximeter Profile Client Task  */

    SONATA_PRF_ID_CGMS         = 69,    /* *< Continuous Glucose Monitoring Server Task  */
    SONATA_PRF_ID_CGMC         = 70,    /* *< Continuous Glucose Monitoring Client Task  */

    SONATA_PRF_ID_OTAS         = 80,

    SONATA_PRF_ID_MESH         = 200,   /* *<Mesh Task  */

    /*  240 -> 241 reserved for Audio Mode 0 */
    SONATA_PRF_ID_AM0          = 240,  /* *< BLE Audio Mode 0 Task  */
    SONATA_PRF_ID_AM0_HAS      = 241,  /* *< BLE Audio Mode 0 Hearing Aid Service Task  */

    SONATA_PRF_ID_THPP         = 242,  /* *< Throughput profile tester used for debugging */
} sonata_ble_profile_id;

/// peer information type
typedef enum {
    /// Retrieve name of peer device.   // Will show in @see sonata_gap_peer_att_info_ind_handler
    SONATA_GET_PEER_NAME                            = 0x02,
    /// Retrieve peer device version info. // Will show in  @see sonata_gap_peer_version_ind_handler
    SONATA_GET_PEER_VERSION                         = 0x03,
    /// Retrieve peer device features. // Will show in  @see sonata_gap_peer_features_ind_handler
    SONATA_GET_PEER_FEATURES                        = 0x04,
    /// Get Peer device appearance // Will show in   @see sonata_gap_peer_att_info_ind_handler
    SONATA_GET_PEER_APPEARANCE                      = 0x05,
    /// Get Peer device Slaved Preferred Parameters // Will show in  @see sonata_gap_peer_att_info_ind_handler
    SONATA_GET_PEER_SLV_PREF_PARAMS                 = 0x06,
    /// Retrieve connection RSSI. // Will show in  @see sonata_gap_con_rssi_ind_handler
    SONATA_GET_PEER_CON_RSSI                        = 0x07,
    /// Retrieve Connection Channel MAP. // Will show in  @see sonata_gap_con_channel_map_ind_handler
    SONATA_GET_PEER_CON_CHANNEL_MAP                 = 0x08,
    // Will show in  @see sonata_gap_le_ping_to_val_ind_handler and @see sonata_gap_le_ping_to_ind_handler
    SONATA_GET_LE_PING_TO                           = 0x12,
    // Will show in  @see sonata_gap_peer_att_info_ind_handler
    SONATA_GET_PEER_ADDR_RESOL_SUPP                 = 0x15,
    // Will show in @see sonata_gap_le_phy_ind_handler
    SONATA_GET_PHY                                  = 0x18,
    // Will show in @see sonata_gap_chan_sel_algo_ind_handler
    SONATA_GET_CHAN_SEL_ALGO                        = 0x19,
} sonata_peer_info_type;

/// Bond event type.
/* @TRACE */
typedef enum {
    /// Bond Pairing request
    SONATA_GAP_PAIRING_REQ,
    /// Respond to Pairing request
    SONATA_GAP_PAIRING_RSP,

    /// Pairing Finished information
    SONATA_GAP_PAIRING_SUCCEED,
    /// Pairing Failed information
    SONATA_GAP_PAIRING_FAILED,

    /// Used to retrieve pairing Temporary Key
    SONATA_GAP_TK_EXCH,
    /// Used for Identity Resolving Key exchange
    SONATA_GAP_IRK_EXCH,
    /// Used for Connection Signature Resolving Key exchange
    SONATA_GAP_CSRK_EXCH,
    /// Used for Long Term Key exchange
    SONATA_GAP_LTK_EXCH,

    /// Bond Pairing request issue, Repeated attempt
    SONATA_GAP_REPEATED_ATTEMPT,

    /// Out of Band - exchange of confirm and rand.
    SONATA_GAP_OOB_EXCH,

    /// Numeric Comparison - Exchange of Numeric Value -
    SONATA_GAP_NC_EXCH,
} sonata_gap_bond;

/// Type of activities that can be created
typedef enum {
    /// Advertising activity
    SONATA_GAP_ACTV_TYPE_ADV = 0,
    /// Scanning activity
    SONATA_GAP_ACTV_TYPE_SCAN,
    /// Initiating activity
    SONATA_GAP_ACTV_TYPE_INIT,
    /// Periodic synchronization activity
    SONATA_GAP_ACTV_TYPE_PER_SYNC,
} sonata_gapm_actv_type;

/// Type of advertising that can be created
typedef enum {
    /// Legacy advertising
    SONATA_GAP_ADV_TYPE_LEGACY = 0,
    /// Extended advertising
    SONATA_GAP_ADV_TYPE_EXTENDED,
    /// Periodic advertising
    SONATA_GAP_ADV_TYPE_PERIODIC,
} sonata_gap_adv_type;

/// Information source.
typedef enum {
    /// Local info.
    SONATA_GAP_SMP_INFO_LOCAL,
    /// Peer info.
    SONATA_GAP_SMP_INFO_PEER,
    /// Maximum info source.
    SONATA_GAP_SMP_INFO_MAX
} sonata_ble_gap_smp_addr_src;

/// Attribute database configuration
//   15   14   13   12   11   10    9    8    7    6    5    4    3    2    1    0
// +----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+
// | DBG|DBGT|               RFU                |PCP |   APP_PERM   |   NAME_PERM  |
// +----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+
// - Bit [0-2] : Device Name write permission requirements for peer device (@see sonata_gap_write_att_perm)
// - Bit [3-5] : Device Appearance write permission requirements for peer device (@see sonata_gap_write_att_perm)
// - Bit [6]   : Slave Preferred Connection Parameters present
// - Bit [7-13]: Reserved
// - Bit [14]  : if Debug Mode enabled, forward all L2cap traffic to application
// - Bit [15]  : Enable Debug Mode
typedef enum {
    /// Device Name write permission requirements for peer device (@see sonata_gap_write_att_perm)
    SONATA_GAP_ATT_NAME_PERM_MASK            = 0x0007,
    SONATA_GAP_ATT_NAME_PERM_LSB             = 0x00,
    /// Device Appearance write permission requirements for peer device (@see sonata_gap_write_att_perm)
    SONATA_GAP_ATT_APPEARENCE_PERM_MASK      = 0x0038,
    SONATA_GAP_ATT_APPEARENCE_PERM_LSB       = 0x03,
    /// Slave Preferred Connection Parameters present in GAP attribute database.
    SONATA_GAP_ATT_SLV_PREF_CON_PAR_EN_MASK  = 0x0040,
    SONATA_GAP_ATT_SLV_PREF_CON_PAR_EN_LSB   = 0x06,
    /// if Debug Mode enabled, forward all L2cap traffic to application
    SONATA_GAP_ATT_DBG_L2CAP_TRAFFIC_EN_MASK = 0x4000,
    SONATA_GAP_ATT_DBG_L2CAP_TRAFFIC_EN_LSB  = 0x0E,
    /// Enable Debug Mode
    SONATA_GAP_ATT_DBG_MODE_EN_MASK          = 0x8000,
    SONATA_GAP_ATT_DBG_MODE_EN_LSB           = 0x0F,
} sonata_gap_att_cfg_flag;

/// Keypress Notification types
typedef enum {
    /// Passkey entry started
    SONATA_GAP_KEY_NTF_ENTRY_STARTED = 0x00,
    /// Passkey digit entered
    SONATA_GAP_KEY_NTF_DIGIT_ENTERED,
    /// Passkey digit erased
    SONATA_GAP_KEY_NTF_DIGIT_ERASED,
    /// Passkey cleared
    SONATA_GAP_KEY_NTF_CLEARED,
    /// Passkey entry completed
    SONATA_GAP_KEY_NTF_ENTRY_COMPLETED
} sonata_gap_key_ntf_type;

/// Constant Tone Extension type
typedef enum {
    /// Allow AoA Constant Tone Extension Response
    SONATA_GAP_CTE_AOA          = (1 << 0),
    /// Allow AoD Constant Tone Extension Response with 1 us slots
    SONATA_GAP_CTE_AOD_1US_SLOT = (1 << 1),
    /// Allow AoD Constant Tone Extension Response with 2 us slots
    SONATA_GAP_CTE_AOD_2US_SLOT = (1 << 2),
} sonata_gap_cte_type;

/// Advertising report type
typedef enum {
    /// Extended advertising report
    SONATA_GAP_REPORT_TYPE_ADV_EXT = 0,
    /// Legacy advertising report
    SONATA_GAP_REPORT_TYPE_ADV_LEG,
    /// Extended scan response report
    SONATA_GAP_REPORT_TYPE_SCAN_RSP_EXT,
    /// Legacy scan response report
    SONATA_GAP_REPORT_TYPE_SCAN_RSP_LEG,
    /// Periodic advertising report
    SONATA_GAP_REPORT_TYPE_PER_ADV,
} sonata_gap_adv_report_type;

/// gap extent advertising report indicate
typedef struct sonata_gap_ext_adv_report_ind {
    /// Activity identifier
    uint8_t actv_idx;
    /// Bit field providing information about the received report (@see enum sonata_gap_adv_report_info)
    uint8_t info;
    /// Transmitter device address
    struct sonata_gap_bdaddr trans_addr;
    /// Target address (in case of a directed advertising report)
    struct sonata_gap_bdaddr target_addr;
    /// TX power (in dBm)
    int8_t tx_pwr;
    /// RSSI (between -127 and +20 dBm)
    int8_t rssi;
    /// Primary PHY on which advertising report has been received
    uint8_t phy_prim;
    /// Secondary PHY on which advertising report has been received
    uint8_t phy_second;
    /// Advertising SID
    /// Valid only for periodic advertising report
    uint8_t adv_sid;
    /// Periodic advertising interval (in unit of 1.25ms, min is 7.5ms)
    /// Valid only for periodic advertising report
    uint16_t period_adv_intv;
    /// Report length
    uint16_t length;
    /// Report
    uint8_t data[__ARRAY_EMPTY];
} sonata_gap_ext_adv_report_ind_t;

/// Indicate that a connection has been established
typedef struct sonata_gap_connection_req_ind {
    /// Connection handle
    uint16_t conhdl;
    /// Connection interval
    uint16_t con_interval;
    /// Connection latency
    uint16_t con_latency;
    /// Link supervision timeout
    uint16_t sup_to;
    /// Clock accuracy
    uint8_t clk_accuracy;
    /// Peer address type
    uint8_t peer_addr_type;
    /// Peer BT address
    sonata_bd_addr_t peer_addr;
    /// Role of device in connection (0 = Master / 1 = Slave)
    uint8_t role;

} sonata_gap_connection_req_ind_t;

/// device information data
typedef union sonata_gap_dev_info_val {
    /// Device name
    // @trc_union parent.req == SONATA_GAP_DEV_NAME
    struct sonata_gap_dev_name name;
    /// Appearance Icon
    // @trc_union parent.req == SONATA_GAP_DEV_APPEARANCE
    uint16_t appearance;
    /// Slave preferred parameters
    // @trc_union parent.req == SONATA_GAP_DEV_SLV_PREF_PARAMS
    struct sonata_gap_slv_pref slv_pref_params;
    /// Central address resolution
    // @trc_union parent.req == SONATA_GAP_DEV_CTL_ADDR_RESOL
    uint8_t ctl_addr_resol;
} sonata_gap_dev_info_val_t;

/// Peer device attribute DB info such as Device Name, Appearance or Slave Preferred Parameters
typedef struct sonata_gap_peer_att_info_ind {
    /// Requested information
    /// - SONATA_GAP_DEV_NAME: Device Name
    /// - SONATA_GAP_DEV_APPEARANCE: Device Appearance Icon
    /// - SONATA_GAP_DEV_SLV_PREF_PARAMS: Device Slave preferred parameters
    /// - SONATA_GAP_GET_ADDR_RESOL_SUPP: Address resolution supported
    uint8_t  req;
    /// Attribute handle
    uint16_t handle;

    /// device information data
    union sonata_gap_dev_info_val info;
} sonata_gap_peer_att_info_ind_t;

/// Indication of peer version info
/* @TRACE */
typedef struct sonata_gap_peer_version_ind {
    /// Manufacturer name
    uint16_t compid;
    /// LMP subversion
    uint16_t lmp_subvers;
    /// LMP version
    uint8_t  lmp_vers;
} sonata_gap_peer_version_ind_t;
/// Indication of peer features info

/* @TRACE */
typedef struct sonata_gap_peer_features_ind {
    /// 8-byte array for LE features
    uint8_t features[SONATA_GAP_LE_FEATS_LEN];
} sonata_gap_peer_features_ind_t;

/// Indication of ongoing connection RSSI
/* @TRACE */
typedef struct sonata_gap_con_rssi_ind {
    /// RSSI value
    int8_t rssi;
} sonata_gap_con_rssi_ind_t;

/// Indication of ongoing connection Channel Map
/* @TRACE */
typedef struct sonata_gap_con_channel_map_ind {
    /// channel map value
    sonata_le_chnl_map_t ch_map;
} sonata_gap_con_channel_map_ind_t;

/// Indication of LE Ping
/* @TRACE */
typedef struct sonata_gap_le_ping_to_val_ind {
    /// Authenticated payload timeout
    uint16_t timeout;
} sonata_gap_le_ping_to_val_ind_t;

/// Active link PHY configuration. Triggered when configuration is read or during an update.
/* @TRACE */
typedef struct sonata_gap_le_phy_ind {
    /// LE PHY for data transmission (@see enum gap_phy_val)
    uint8_t tx_phy;
    /// LE PHY for data reception (@see enum gap_phy_val)
    uint8_t rx_phy;
} sonata_gap_le_phy_ind_t;

/// Parameters of the @ref SONATA_GAP_CHAN_SEL_ALGO_IND
/* @TRACE */
typedef struct sonata_gap_chan_sel_algo_ind {
    /// Used channel selection algorithm
    uint8_t chan_sel_algo;
} sonata_gap_chan_sel_algo_ind_t;

/// peer device information indication for get peer version, features, con rssi, channel map, phy, algo
typedef struct sonata_gap_peer_info_ind {
    /// Requested information
    /// - SONATA_GET_PEER_VERSION:
    /// - SONATA_GET_PEER_FEATURES:
    /// - SONATA_GET_PEER_CON_RSSI:
    /// - SONATA_GET_PEER_CON_CHANNEL_MAP:
    /// - SONATA_GET_LE_PING_TO:
    /// - SONATA_GET_PHY:
    /// - SONATA_GET_CHAN_SEL_ALGO:
    uint8_t  req;

    union info_ {

        struct sonata_gap_peer_version_ind version;
        struct sonata_gap_peer_features_ind features;
        struct sonata_gap_con_rssi_ind rssi;
        struct sonata_gap_con_channel_map_ind channel_map;
        struct sonata_gap_le_ping_to_val_ind ping_to_value;
        struct sonata_gap_le_phy_ind le_phy;
        struct sonata_gap_chan_sel_algo_ind sel_algo;
    } info;

} sonata_gap_peer_info_ind_t;

/// Set device configuration command
typedef struct  {
    //    /// GAP requested operation:
    //    ///  - SONATA_GAP_SET_DEV_CONFIG: Set device configuration

    /// Device Role: Central, Peripheral, Observer, Broadcaster or All roles.
    uint8_t role;

    /// -------------- Privacy Config -----------------------
    /// Duration before regenerate device address when privacy is enabled. - in seconds
    uint16_t renew_dur;
    /// Provided own static private random address
    sonata_bd_addr_t addr;
    /// Device IRK used for resolvable random BD address generation (LSB first)
    struct sonata_gap_sec_key irk;
    /// Privacy configuration bit field (@see enum sonata_gap_priv_cfg for bit signification)
    uint8_t privacy_cfg;

    /// -------------- Security Config -----------------------

    /// Pairing mode authorized (@see enum sonata_gap_pairing_mode)
    uint8_t pairing_mode;

    /// -------------- ATT Database Config -----------------------

    /// GAP service start handle
    uint16_t gap_start_hdl;
    /// GATT service start handle
    uint16_t gatt_start_hdl;

    /// Attribute database configuration (@see enum sonata_gap_att_cfg_flag)
    uint16_t att_cfg;

    //    /// -------------- LE Data Length Extension -----------------------
    //    /// Suggested value for the Controller's maximum transmitted number of payload octets to be used
    //    uint16_t sugg_max_tx_octets;
    //    /// Suggested value for the Controller's maximum packet transmission time to be used
    //    uint16_t sugg_max_tx_time;

    /// --------------- L2CAP Configuration ---------------------------
    /// Maximal MTU acceptable for device
    uint16_t max_mtu;
    /// Maximal MPS Packet size acceptable for device
    uint16_t max_mps;
    /// Maximum number of LE Credit based connection that can be established
    uint8_t max_nb_lecb;

    //    /// --------------- LE Audio Mode Supported -----------------------
    //    ///
    //    /// LE Audio Mode Configuration (@see sonata_gap_audio_cfg_flag)

    //    /// ------------------ LE PHY Management  -------------------------
    //    /// Preferred LE PHY for data transmission (@see enum gap_phy)

    //    /// Preferred LE PHY for data reception (@see enum gap_phy)

    //    /// ------------------ Miscellaneous 2 ----------------------------
    //    /// RF TX Path Compensation value (from -128dB to 128dB, unit is 0.1dB)

    //    /// RF RX Path Compensation value (from -128dB to 128dB, unit is 0.1dB)

    ///------------------ Miscellaneou 3 -----------------------------
    bool hl_trans_dbg;
    /// <host layer transport debug
    uint8_t pa_en;
    /// <pa enable  1:pa enable 0:pa disable  default:0
    /// <pinmux: rxcore->pad12 txcore->pad17
} sonata_gap_set_dev_config_cmd;

/// create directed advertising
typedef struct sonata_gap_directed_adv_create_param {
    //    /// Advertising type (@see enum sonata_gap_adv_type)

    /// Discovery mode (@see enum sonata_gap_adv_disc_mode)
    uint8_t disc_mode;
    /// Bit field value provided advertising properties (@see enum sonata_gap_adv_prop for bit signification)
    uint16_t prop;
    /// Maximum power level at which the advertising packets have to be transmitted
    /// (between -127 and 126 dBm)
    int8_t max_tx_pwr;
    /// Advertising filtering policy (@see enum adv_filter_policy)
    uint8_t filter_pol;
    /// Peer address configuration (only used in case of directed advertising)
    sonata_bd_addr_t addr;
    /// Address type of the device 0=public/1=private random
    uint8_t addr_type;
    /// Configuration for primary advertising
    /// Minimum advertising interval (in unit of 625us). Must be greater than 20ms
    uint32_t adv_intv_min;
    /// Maximum advertising interval (in unit of 625us). Must be greater than 20ms
    uint32_t adv_intv_max;
    /// Bit field indicating the channel mapping
    uint8_t chnl_map;
    /// Indicate on which PHY primary advertising has to be performed (@see enum sonata_gap_phy_type)
    /// Note that LE 2M PHY is not allowed and that legacy advertising only support LE 1M PHY
    uint8_t phy;

} sonata_gap_directed_adv_create_param_t;

/// create extended advertising
typedef struct sonata_gap_extended_adv_create_param {
    //    /// Advertising type (@see enum sonata_gap_adv_type)

    /// Discovery mode (@see enum sonata_gap_adv_disc_mode)
    uint8_t disc_mode;
    /// Bit field value provided advertising properties (@see enum sonata_gap_adv_prop for bit signification)
    uint16_t prop;
    /// Maximum power level at which the advertising packets have to be transmitted
    /// (between -127 and 126 dBm)
    int8_t max_tx_pwr;
    /// Advertising filtering policy (@see enum adv_filter_policy)
    uint8_t filter_pol;

    uint32_t prim_adv_intv_min;
    /// Maximum advertising interval (in unit of 625us). Must be greater than 20ms
    uint32_t prim_adv_intv_max;
    /// Bit field indicating the channel mapping
    uint8_t prim_chnl_map;
    /// Indicate on which PHY primary advertising has to be performed (@see enum sonata_gap_phy_type)
    /// Note that LE 2M PHY is not allowed and that legacy advertising only support LE 1M PHY
    uint8_t prim_phy;

    /// Maximum number of advertising events the controller can skip before sending the
    /// AUX_ADV_IND packets. 0 means that AUX_ADV_IND PDUs shall be sent prior each
    /// advertising events
    uint8_t second_max_skip;
    /// Indicate on which PHY secondary advertising has to be performed (@see enum sonata_gap_phy_type)
    uint8_t second_phy;
    /// Advertising SID
    uint8_t second_adv_sid;

} sonata_gap_extended_adv_create_param_t;

///  create periodic advertising
typedef struct sonata_gap_periodic_adv_create_param {
    //    /// Advertising type (@see enum sonata_gap_adv_type)

    /// Discovery mode (@see enum sonata_gap_adv_disc_mode)
    uint8_t disc_mode;
    /// Bit field value provided advertising properties (@see enum sonata_gap_adv_prop for bit signification)
    uint16_t prop;
    /// Maximum power level at which the advertising packets have to be transmitted
    /// (between -127 and 126 dBm)
    int8_t max_tx_pwr;
    /// Advertising filtering policy (@see enum adv_filter_policy)
    uint8_t filter_pol;
    /// Configuration for primary advertising
    /// Minimum advertising interval (in unit of 625us). Must be greater than 20ms
    uint32_t prim_adv_intv_min;
    /// Maximum advertising interval (in unit of 625us). Must be greater than 20ms
    uint32_t prim_adv_intv_max;
    /// Bit field indicating the channel mapping
    uint8_t prim_chnl_map;
    /// Indicate on which PHY primary advertising has to be performed (@see enum sonata_gap_phy_type)
    /// Note that LE 2M PHY is not allowed and that legacy advertising only support LE 1M PHY
    uint8_t prim_phy;
    /// Configuration for secondary advertising (valid only if advertising type is
    /// SONATA_GAP_ADV_TYPE_EXTENDED or SONATA_GAP_ADV_TYPE_PERIODIC)
    /// Maximum number of advertising events the controller can skip before sending the
    /// AUX_ADV_IND packets. 0 means that AUX_ADV_IND PDUs shall be sent prior each
    /// advertising events
    uint8_t second_max_skip;
    /// Indicate on which PHY secondary advertising has to be performed (@see enum sonata_gap_phy_type)
    uint8_t second_phy;
    /// Advertising SID
    uint8_t second_adv_sid;
    /// Configuration for periodic advertising (valid only if advertising type os
    /// SONATA_GAP_ADV_TYPE_PERIODIC)
    /// Minimum advertising interval (in unit of 1.25ms). Must be greater than 20ms
    uint16_t period_adv_intv_min;
    /// Maximum advertising interval (in unit of 1.25ms). Must be greater than 20ms
    uint16_t period_adv_intv_max;
    /// CTE count (number of CTEs to transmit in each periodic advertising interval, range 0x01 to 0x10)
    /// 0 to disable CTE transmission
    uint8_t period_cte_count;
    /// CTE type (0: AOA | 1: AOD-1us | 2: AOD-2us) (@see enum gap_cte_type)
    uint8_t period_cte_type;
    /// CTE length (in 8us unit)
    uint8_t period_cte_len;
    /// Length of switching pattern (number of antenna IDs in the pattern)
    uint8_t period_switching_pattern_len;
    /// Antenna IDs
    uint8_t period_antenna_id[__ARRAY_EMPTY];
} sonata_gap_periodic_adv_create_param_t;

/// Scan Window operation parameters
typedef struct  sonata_gap_scan_wd_op_param {
    /// Scan interval
    uint16_t scan_intv;
    /// Scan window
    uint16_t scan_wd;
} sonata_gap_scan_wd_op_param_t;

/// Scanning parameters
typedef struct sonata_gap_scan_param {
    /// Type of scanning to be started (@see enum sonata_gap_scan_type)
    uint8_t type;
    /// Properties for the scan procedure (@see enum sonata_gap_scan_prop for bit signification)
    uint8_t prop;
    /// Duplicate packet filtering policy
    uint8_t dup_filt_pol;
    /// Reserved for future use
    uint8_t rsvd;
    /// Scan window opening parameters for LE 1M PHY
    struct sonata_gap_scan_wd_op_param scan_param_1m;
    /// Scan window opening parameters for LE Coded PHY
    struct sonata_gap_scan_wd_op_param scan_param_coded;
    /// Scan duration (in unit of 10ms). 0 means that the controller will scan continuously until
    /// reception of a stop command from the application
    uint16_t duration;
    /// Scan period (in unit of 1.28s). Time interval betweem two consequent starts of a scan duration
    /// by the controller. 0 means that the scan procedure is not periodic
    uint16_t period;
} sonata_gap_scan_param_t;

/// Connection parameters
typedef struct sonata_gap_conn_param {
    /// Minimum value for the connection interval (in unit of 1.25ms). Shall be less than or equal to
    /// conn_intv_max value. Allowed range is 7.5ms to 4s.
    uint16_t conn_intv_min;
    /// Maximum value for the connection interval (in unit of 1.25ms). Shall be greater than or equal to
    /// conn_intv_min value. Allowed range is 7.5ms to 4s.
    uint16_t conn_intv_max;
    /// Slave latency. Number of events that can be missed by a connected slave device
    uint16_t conn_latency;
    /// Link supervision timeout (in unit of 10ms). Allowed range is 100ms to 32s
    uint16_t supervision_to;
    /// Recommended minimum duration of connection events (in unit of 625us)
    uint16_t ce_len_min;
    /// Recommended maximum duration of connection events (in unit of 625us)
    uint16_t ce_len_max;
} sonata_gap_conn_param_t;

/// Initiating parameters
typedef struct sonata_gap_init_param {
    /// Initiating type (@see enum sonata_gap_init_type)
    uint8_t type;
    /// Properties for the initiating procedure (@see enum sonata_gap_init_prop for bit signification)
    uint8_t prop;
    /// Timeout for automatic connection establishment (in unit of 10ms). Cancel the procedure if not all
    /// indicated devices have been connected when the timeout occurs. 0 means there is no timeout
    uint16_t conn_to;
    /// Scan window opening parameters for LE 1M PHY
    struct sonata_gap_scan_wd_op_param scan_param_1m;
    /// Scan window opening parameters for LE Coded PHY
    struct sonata_gap_scan_wd_op_param scan_param_coded;
    /// Connection parameters for LE 1M PHY
    struct sonata_gap_conn_param conn_param_1m;
    /// Connection parameters for LE 2M PHY
    struct sonata_gap_conn_param conn_param_2m;
    /// Connection parameters for LE Coded PHY
    struct sonata_gap_conn_param conn_param_coded;
    /// Address of peer device in case white list is not used for connection
    struct sonata_gap_bdaddr peer_addr;
} sonata_gap_init_param_t;

/// Periodic advertising information
typedef struct sonata_gap_period_adv_addr_cfg {
    /// Advertiser address information
    struct sonata_gap_bdaddr addr;
    /// Advertising SID
    uint8_t adv_sid;
} sonata_gap_period_adv_addr_cfg_t;

/// Periodic synchronization parameters
typedef struct sonata_gap_per_sync_param {
    /// Number of periodic advertising that can be skipped after a successful receive. Maximum authorized
    /// value is 499
    uint16_t skip;
    /// Synchronization timeout for the periodic advertising (in unit of 10ms between 100ms and 163.84s)
    uint16_t sync_to;
    /// Periodic synchronization type (@see enum sonata_gap_per_sync_type)
    uint8_t type;
    /// Connection index used for periodic sync info reception (only valid for SONATA_GAP_PER_SYNC_TYPE_PAST)
    uint8_t conidx;
    /// Address of advertiser with which synchronization has to be established (used only if use_pal is false)
    struct sonata_gap_period_adv_addr_cfg adv_addr;
    /// 1 to disable periodic advertising report, 0 to enable them by default
    uint8_t report_disable;
    /// Type of Constant Tone Extension device should sync on (@see enum sonata_gap_sync_cte_type).
    uint8_t cte_type;
} sonata_gap_per_sync_param_t;

/// Local device version indication event
typedef struct sonata_gap_dev_version_ind {
    /// HCI version
    uint8_t hci_ver;
    /// LMP version
    uint8_t lmp_ver;
    /// Host version
    uint8_t host_ver;
    /// HCI revision
    uint16_t hci_subver;
    /// LMP subversion
    uint16_t lmp_subver;
    /// Host revision
    uint16_t host_subver;
    /// Manufacturer name
    uint16_t manuf_name;
} sonata_gap_dev_version_ind_t;

/// Local device BD Address indication event
typedef struct sonata_gap_dev_bdaddr_ind {
    /// Local device address information
    struct sonata_gap_bdaddr addr;
    /// Activity index
    uint8_t actv_idx;
} sonata_gap_dev_bdaddr_ind_t;

/// Advertising channel Tx power level indication event
typedef struct sonata_gap_dev_adv_tx_power_ind {
    /// Advertising channel Tx power level
    int8_t power_lvl;
} sonata_gap_dev_adv_tx_power_ind_t;

/// Indication containing controller antenna information
typedef struct sonata_gap_antenna_inf_ind {
    /// Supported switching sampling rates bit field (@see enum sonata_gap_switch_sampling_rate)
    uint8_t     supp_switching_sampl_rates;
    /// Number of antennae
    uint8_t     antennae_num;
    /// Max length of switching pattern (number of antenna IDs in the pattern)
    uint8_t     max_switching_pattern_len;
    /// Max CTE length
    uint8_t     max_cte_len;
} sonata_gap_antenna_inf_ind_t;

/// Numeric Comparison Data
typedef struct sonata_gap_nc {
    uint8_t value[4];
} sonata_gap_nc_t;

/// Out of Band Information
typedef struct sonata_gap_oob {
    /// Confirm Value
    uint8_t conf[SONATA_GAP_KEY_LEN];
    /// Random Number
    uint8_t rand[SONATA_GAP_KEY_LEN];
} sonata_gap_oob_t;

/// Bond procedure requested information data
typedef union sonata_gap_bond_req_data {
    /// Authentication level (@see gap_auth) (if request = SONATA_GAP_PAIRING_REQ)
    // @trc_union parent.request == SONATA_GAP_PAIRING_REQ
    uint8_t auth_req;
    /// LTK Key Size (if request = SONATA_GAP_LTK_EXCH)
    // @trc_union parent.request == SONATA_GAP_LTK_EXCH
    uint8_t key_size;
    /// Device IO used to get TK: (if request = SONATA_GAP_TK_EXCH)
    ///  - GAP_TK_OOB:       TK get from out of band method
    ///  - GAP_TK_DISPLAY:   TK generated and shall be displayed by local device
    ///  - GAP_TK_KEY_ENTRY: TK shall be entered by user using device keyboard
    // @trc_union parent.request == SONATA_GAP_TK_EXCH
    uint8_t tk_type;

    /// Addition OOB Data for the OOB Conf and Rand values
    // @trc_union parent.request == SONATA_GAP_OOB_EXCH
    struct sonata_gap_oob  oob_data;
    /// Numeric Comparison Data
    // @trc_union parent.request == SONATA_GAP_NC_EXCH
    struct sonata_gap_nc   nc_data;
} sonata_gap_bond_req_data_t;

/// Bonding requested by peer device indication message.
typedef struct sonata_gap_bond_req_ind {
    /// Bond request type (@see sonata_gap_bond)
    uint8_t request;

    /// Bond procedure requested information data
    union sonata_gap_bond_req_data data;
} sonata_gap_bond_req_ind_t;

/// Long Term Key information
typedef struct sonata_gap_ltk {
    /// Long Term Key
    struct sonata_gap_sec_key ltk;
    /// Encryption Diversifier
    uint16_t ediv;
    /// Random Number
    sonata_rand_nb_t randnb;
    /// Encryption key size (7 to 16)
    uint8_t key_size;
} sonata_gap_ltk_t;

/// Identity Resolving Key information
typedef struct sonata_gap_irk {
    /// Identity Resolving Key
    struct sonata_gap_sec_key irk;
    /// Device BD Identity Address
    struct sonata_gap_bdaddr addr;
} sonata_gap_irk_t;

/// Authentication information
typedef struct sonata_gap_bond_auth {
    /// Authentication information (@see gap_auth)
    uint8_t info;

    /// LTK exchanged during pairing.
    bool ltk_present;
} sonata_gap_bond_auth_t;

/// Bond procedure information data
typedef union sonata_gap_bond_data {
    /// Authentication information (@see gap_auth)
    /// (if info = SONATA_GAP_PAIRING_SUCCEED)
    // @trc_union parent.info == SONATA_GAP_PAIRING_SUCCEED
    struct sonata_gap_bond_auth auth;
    /// Pairing failed reason  (if info = SONATA_GAP_PAIRING_FAILED)
    // @trc_union parent.info == SONATA_GAP_PAIRING_FAILED
    uint8_t reason;
    /// Long Term Key information (if info = SONATA_GAP_LTK_EXCH)
    // @trc_union parent.info == SONATA_GAP_LTK_EXCH
    struct sonata_gap_ltk ltk;
    /// Identity Resolving Key information (if info = SONATA_GAP_IRK_EXCH)
    // @trc_union parent.info == SONATA_GAP_IRK_EXCH
    struct sonata_gap_irk irk;
    /// Connection Signature Resolving Key information (if info = SONATA_GAP_CSRK_EXCH)
    // @trc_union parent.info == SONATA_GAP_CSRK_EXCH
    struct sonata_gap_sec_key csrk;
} sonata_gap_bond_data_t;

/// Bonding information indication message
typedef struct sonata_gap_bond_ind {
    /// Bond information type (@see sonata_gap_bond)
    uint8_t info;

    /// Bond procedure information data
    union sonata_gap_bond_data data;
} sonata_gap_bond_ind_t;

/// List Size indication event
typedef struct sonata_gap_list_size_ind {
    /// Operation code, indicate list for which size has been read
    ///     - SONATA_GAP_SET_WHITE_LIST
    ///     - SONATA_GAP_SET_RAL
    ///     - SONATA_GAP_SET_PAL
    uint8_t operation;
    /// List size
    uint8_t size;
} sonata_gap_list_size_ind_t;

/// Indicates suggested default data length
typedef struct sonata_gap_sugg_dflt_data_len_ind {
    /// Host's suggested value for the Controller's maximum transmitted number of payload octets
    uint16_t suggted_max_tx_octets;
    /// Host's suggested value for the Controller's maximum packet transmission time
    uint16_t suggted_max_tx_time;
} sonata_gap_sugg_dflt_data_len_ind_t;

/// Indicates maximum data length
typedef struct sonata_gap_max_data_len_ind {
    /// Maximum number of payload octets that the local Controller supports for transmission
    uint16_t suppted_max_tx_octets;
    /// Maximum time, in microseconds, that the local Controller supports for transmission
    uint16_t suppted_max_tx_time;
    /// Maximum number of payload octets that the local Controller supports for reception
    uint16_t suppted_max_rx_octets;
    /// Maximum time, in microseconds, that the local Controller supports for reception
    uint16_t suppted_max_rx_time;
} sonata_gap_max_data_len_ind_t;

/// Number of available advertising sets indication event
typedef struct sonata_gap_nb_adv_sets_ind {
    /// Number of available advertising sets
    uint8_t nb_adv_sets;
} sonata_gap_nb_adv_sets_ind_t;

/// Maximum advertising data length indication event
typedef struct sonata_gap_max_adv_data_len_ind {
    /// Maximum advertising data length supported by controller
    uint16_t length;
} sonata_gap_max_adv_data_len_ind_t;

/// Indicate the transmit powers supported by the controller
typedef struct sonata_gap_dev_tx_pwr_ind {
    /// Minimum TX power
    int8_t min_tx_pwr;
    /// Maximum TX power
    int8_t max_tx_pwr;
} sonata_gap_dev_tx_pwr_ind_t;

/// Indicate the RF path compensation values
typedef struct sonata_gap_dev_rf_path_comp_ind {
    /// RF TX path compensation
    uint16_t tx_path_comp;
    /// RF RX path compensation
    uint16_t rx_path_comp;
} sonata_gap_dev_rf_path_comp_ind_t;

/// Pairing parameters
typedef struct sonata_gap_pairing {
    /// IO capabilities (@see gap_io_cap)
    uint8_t iocap;
    /// OOB information (@see gap_oob)
    uint8_t oob;
    /// Authentication (@see gap_auth)
    /// Note in BT 4.1 the Auth Field is extended to include 'Key Notification' and
    /// and 'Secure Connections'.
    uint8_t auth;
    /// Encryption key size (7 to 16)
    uint8_t key_size;
    /// Initiator key distribution (@see gap_kdist)
    uint8_t ikey_dist;
    /// Responder key distribution (@see gap_kdist)
    uint8_t rkey_dist;

    /// Device security requirements (minimum security level). (@see gap_sec_req)
    uint8_t sec_req;
} sonata_gap_pairing_t;

/// Set specific link data configuration.
typedef struct sonata_gap_connection_cfm {
    /// Local CSRK value
    struct sonata_gap_sec_key lcsrk;
    /// Local signature counter value
    uint32_t           lsign_counter;
    /// Remote CSRK value
    struct sonata_gap_sec_key rcsrk;
    /// Remote signature counter value
    uint32_t           rsign_counter;
    /// Authentication (@see enum gap_auth)
    uint8_t            auth;
    /// Client bond data information (@see enum sonata_gap_cli_info)
    uint8_t            cli_info;
    /// LTK exchanged during pairing.
    bool               ltk_present;
    /// Client supported features    (@see enum sonata_gap_cli_feat)
    uint8_t            cli_feat;
    /// Peer GATT Service Start handle
    uint16_t           gatt_start_handle;
    /// Peer GATT Service End Handle
    uint16_t           gatt_end_handle;
    /// Peer Service Change value handle
    uint16_t           svc_chg_handle;
} sonata_gap_connection_cfm_t;

/// Indicate reception of a IQ Report event over a BLE connection
/* @TRACE */
typedef struct sonata_gap_cte_iq_report_ind {
    /// Rx PHY  (@see enum gap_phy_val)
    uint8_t  rx_phy;
    /// Data channel index
    uint8_t  data_channel_idx;
    /// RSSI (in 0.1 dBm)
    int16_t  rssi;
    /// RSSI antenna ID
    uint8_t  rssi_antenna_id;
    /// CTE type (0: AOA | 1: AOD-1us | 2: AOD-2us) (@see enum gap_cte_type)
    uint8_t  cte_type;
    /// Slot durations (1: 1us | 2: 2us)
    uint8_t  slot_dur;
    /// Packet status
    uint8_t  pkt_status;
    /// Connection event counter
    uint16_t con_evt_cnt;
    /// Number of samples
    uint8_t  nb_samples;
    /// I/Q sample
    struct sonata_gap_iq_sample sample[__ARRAY_EMPTY];
} sonata_gap_cte_iq_report_ind_t;

/// Provide statistic information about ISO exchange
typedef struct sonata_gap_iso_stat_ind {
    /// ISO Handle of the isochronous channel (Range 0x0000-0x0EFF)
    uint16_t iso_hdl;

    /// Statistics - Number of transmission attempts
    uint32_t nb_tx;
    /// Statistics - Number of transmission attempts that succeed
    uint32_t nb_tx_ok;
    /// Statistics - Number of Not granted packet packets
    uint32_t nb_tx_not_granted;

    /// Statistics - Number of reception attempt
    uint32_t nb_rx;
    /// Statistics - Number of reception attempts that succeed
    uint32_t nb_rx_ok;
    /// Statistics - Number of Not granted packet packets
    uint32_t nb_rx_not_granted;
    /// Statistics - Number of wrongly received packet (invalid data)
    uint32_t nb_rx_data_err;
    /// Statistics - Number of CRC Errors
    uint32_t nb_rx_crc_err;
    /// Statistics - Number of SYNC Errors
    uint32_t nb_rx_sync_err;
    /// Statistics - Number of received empty packets
    uint32_t nb_rx_empty;
} sonata_gap_iso_stat_ind_t;

/// Indicate reception of a IQ Report event over a periodic advertising sync activity
typedef struct sonata_gap_per_adv_iq_report_ind {
    /// Activity identifier
    uint8_t  actv_idx;
    /// Data channel index
    uint8_t  channel_idx;
    /// RSSI (in 0.1 dBm)
    int16_t  rssi;
    /// RSSI antenna ID
    uint8_t  rssi_antenna_id;
    /// CTE type (0: AOA | 1: AOD-1us | 2: AOD-2us) (@see enum gap_cte_type)
    uint8_t  cte_type;
    /// Slot durations (1: 1us | 2: 2us)
    uint8_t  slot_dur;
    /// Packet status
    uint8_t  pkt_status;
    /// Periodic Adv Event Counter
    uint16_t pa_evt_cnt;
    /// Number of samples
    uint8_t  nb_samples;
    /// I/Q sample
    struct sonata_gap_iq_sample sample[__ARRAY_EMPTY];
} sonata_gap_per_adv_iq_report_ind_t;

union sonata_gap_bond_cfm_data {
    /// Pairing Features (request = SONATA_GAP_PAIRING_RSP)
    // @trc_union parent.request == SONATA_GAP_PAIRING_RSP
    struct sonata_gap_pairing pairing_feat;
    /// LTK (request = SONATA_GAP_LTK_EXCH)
    // @trc_union parent.request == SONATA_GAP_LTK_EXCH
    struct sonata_gap_ltk ltk;
    /// CSRK (request = SONATA_GAP_CSRK_EXCH)
    // @trc_union parent.request == SONATA_GAP_CSRK_EXCH
    struct sonata_gap_sec_key csrk;
    /// TK (request = SONATA_GAP_TK_EXCH)
    // @trc_union parent.request == SONATA_GAP_TK_EXCH
    struct sonata_gap_sec_key tk;
    /// IRK (request = SONATA_GAP_IRK_EXCH)
    // @trc_union parent.request == SONATA_GAP_IRK_EXCH
    struct sonata_gap_irk irk;
    /// OOB Confirm and Random from the peer (request = SONATA_GAP_OOB_EXCH)
    // @trc_union parent.request == SONATA_GAP_OOB_EXCH
    struct sonata_gap_oob oob;
};

struct sonata_gap_bond_cfm {
    /// Bond request type (@see sonata_gap_bond)
    uint8_t request;
    /// Request accepted
    uint8_t accept;

    /// Bond procedure information data
    union sonata_gap_bond_cfm_data data;
};

typedef void (*PF_NOTIFY_MESH_PRF_READY)(void);
extern PF_NOTIFY_MESH_PRF_READY pf_notify_mesh_prf_ready;

///  Ble complete event type
typedef enum {
    // Sonata event                       Sonata
    /// Ble module on complete
    SONATA_GAP_CMP_BLE_ON                                    = 0x0F01,
    /// Config advertising mode complete
    SONATA_GAP_CMP_ADVERTISING_CONFIG                        = 0x0F02,
    /// Config scanning mode complete
    SONATA_GAP_CMP_SCANNING_CONFIG                           = 0x0F03,
    /// Config initiating mode complete
    SONATA_GAP_CMP_INITIATING_CONFIG                         = 0x0F04,
    /// Config period sync mode complete
    SONATA_GAP_CMP_PERIOD_SYNC_CONFIG                        = 0x0F05,
    /// Start advertising mode complete
    SONATA_GAP_CMP_ADVERTISING_START                         = 0x0F06,
    /// Start scanning mode complete
    SONATA_GAP_CMP_SCANNING_START                            = 0x0F07,
    /// Start initiating mode complete
    SONATA_GAP_CMP_INITIATING_START                          = 0x0F08,
    /// Start period sync mode complete
    SONATA_GAP_CMP_PERIOD_SYNC_START                         = 0x0F09,
    /// Stop advertising mode complete
    SONATA_GAP_CMP_ADVERTISING_STOP                          = 0x0F0A,
    /// Stop scanning mode complete
    SONATA_GAP_CMP_SCANNING_STOP                             = 0x0F0B,
    /// Stop initiating mode complete
    SONATA_GAP_CMP_INITIATING_STOP                           = 0x0F0C,
    /// Stop period sync mode complete
    SONATA_GAP_CMP_PERIOD_SYNC_STOP                          = 0x0F0D,
    /// Delete advertising mode complete
    SONATA_GAP_CMP_ADVERTISING_DELETE                        = 0x0F0E,
    /// Delete scanning mode complete
    SONATA_GAP_CMP_SCANNING_DELETE                           = 0x0F0F,
    /// Delete initiating mode complete
    SONATA_GAP_CMP_INITIATING_DELETE                         = 0x0F10,
    /// Delete period sync mode complete
    SONATA_GAP_CMP_PERIOD_SYNC_DELETE                        = 0x0F11,

    // System event                           GAPM
    /// Reset BLE subsystem: LL and HL.
    SONATA_GAP_CMP_RESET                                     = 0x0101,
    /// Set device configuration
    SONATA_GAP_CMP_SET_DEV_CONFIG                            = 0x0103,
    /// Get Local device version
    SONATA_GAP_CMP_GET_DEV_VERSION                           = 0x0105,
    /// Get Local device BD Address
    SONATA_GAP_CMP_GET_DEV_BDADDR                            = 0x0106,
    /// Get device advertising power level
    SONATA_GAP_CMP_GET_DEV_ADV_TX_POWER                      = 0x0107,
    /// Get White List Size.
    SONATA_GAP_CMP_GET_WLIST_SIZE                            = 0x0108,
    /// Retrieve Antenna information
    SONATA_GAP_CMP_GET_ANTENNA_INFO                          = 0x0109,
    /// Resolve device address
    SONATA_GAP_CMP_RESOLV_ADDR                               = 0x0117,
    /// Generate a random address
    SONATA_GAP_CMP_GEN_RAND_ADDR                             = 0x0118,
    /// Use the controller's AES-128 block
    SONATA_GAP_CMP_ENC_BLOCK                                 = 0x0119,
    /// Generate a 8-byte random number
    SONATA_GAP_CMP_GEN_RAND_NB                               = 0x011A,
    /// Create new task for specific profile
    SONATA_GAP_CMP_PROFILE_TASK_ADD                          = 0x011B,
    /// Get memory usage
    SONATA_GAP_CMP_DBG_GET_MEM_INFO                          = 0x011C,
    /// Get Suggested Default LE Data Length
    SONATA_GAP_CMP_GET_SUGGESTED_DFLT_LE_DATA_LEN            = 0x011F,
    /// Get Maximum LE Data Length
    SONATA_GAP_CMP_GET_MAX_LE_DATA_LEN                       = 0x0120,
    /// Get resolving address list size
    SONATA_GAP_CMP_GET_RAL_SIZE                              = 0x0121,
    /// Set IRK
    SONATA_GAP_CMP_SET_IRK                                   = 0x0128,
    /// Set content of white list
    SONATA_GAP_CMP_SET_WL                                    = 0x0190,
    /// Get periodic advertiser list size
    SONATA_GAP_CMP_GET_PAL_SIZE                              = 0x0195,
    /// Stop all activities
    SONATA_GAP_CMP_STOP_ALL_ACTIVITIES                       = 0x01A6,
    /// Delete all activities
    SONATA_GAP_CMP_DELETE_ALL_ACTIVITIES                     = 0x01A8,
    /// Set advertising data
    SONATA_GAP_CMP_SET_ADV_DATA                              = 0x01A9,
    /// Set scan response data
    SONATA_GAP_CMP_SET_SCAN_RSP_DATA                         = 0x01AA,
    /// Get number of available advertising sets
    SONATA_GAP_CMP_GET_NB_ADV_SETS                           = 0x01AC,
    /// Get maximum advertising data length supported by the controller
    SONATA_GAP_CMP_GET_MAX_LE_ADV_DATA_LEN                   = 0x01AD,
    /// Get minimum and maximum transmit powers supported by the controller
    SONATA_GAP_CMP_GET_DEV_TX_PWR                            = 0x01AE,
    /// Get the RF Path Compensation values used in the TX Power Level and RSSI calculation
    SONATA_GAP_CMP_GET_DEV_RF_PATH_COMP                      = 0x01AF,

    // System event                           GAPC
    /// Disconnect
    SONATA_GAP_CMP_DISCONNECT                               = 0x0201,
    /// Retrieve name of peer device.
    SONATA_GAP_CMP_GET_PEER_NAME                            = 0x0202,
    /// Retrieve peer device version info.
    SONATA_GAP_CMP_GET_PEER_VERSION                         = 0x0203,
    /// Retrieve peer device features.
    SONATA_GAP_CMP_GET_PEER_FEATURES                        = 0x0204,
    /// Get Peer device appearance
    SONATA_GAP_CMP_GET_PEER_APPEARANCE                      = 0x0205,
    /// Get Peer device Slaved Preferred Parameters
    SONATA_GAP_CMP_GET_PEER_SLV_PREF_PARAMS                 = 0x0206,
    /// Retrieve connection RSSI.
    SONATA_GAP_CMP_GET_CON_RSSI                             = 0x0207,
    /// Retrieve Connection Channel MAP.
    SONATA_GAP_CMP_GET_CON_CHANNEL_MAP                      = 0x0208,
    /// Perform update of connection parameters.
    SONATA_GAP_CMP_UPDATE_PARAMS                            = 0x0209,
    /// Start security request procedure
    SONATA_GAP_CMP_SECURITY_REQ                             = 0x020C,
    /// get timer timeout value
    SONATA_GAP_CMP_GET_LE_PING_TO                           = 0x0212,
    /// set timer timeout value
    SONATA_GAP_CMP_SET_LE_PING_TO                           = 0x0213,
    /* LE Data Length extension */
    /// LE Set Data Length
    SONATA_GAP_CMP_SET_LE_PKT_SIZE                          = 0x0214,
    /// Central Address resolution supported
    SONATA_GAP_CMP_GET_ADDR_RESOL_SUPP                      = 0x0215,
    /// Retrieve PHY configuration of active link
    SONATA_GAP_CMP_GET_PHY                                  = 0x0218,
    /// Constant Tone Extension Transmission configuration
    SONATA_GAP_CMP_CTE_TX_CFG                               = 0x0220,
    /// Constant Tone Extension Reception configuration
    SONATA_GAP_CMP_CTE_RX_CFG                                = 0x0221,
    /// Constant Tone Extension request control (enable / disable)
    SONATA_GAP_CMP_CTE_REQ_CTRL                              = 0x0222,
    /// Constant Tone Extension Response control (enable / disable)
    SONATA_GAP_CMP_CTE_RSP_CTRL                              = 0x0223,

    // System event                           GATTM

    // System event                           GATTC
    /// Discover all services
    SONATA_GATT_CMP_DISC_ALL_SVC                            = 0x0402,
    /// Read attribute
    SONATA_GATT_CMP_READ                                    = 0x0408,
    /// Write attribute
    SONATA_GATT_CMP_WRITE                                   = 0x040C,
    /// Write no response
    SONATA_GATT_CMP_WRITE_NO_RESPONSE                       = 0x040D,
    /// Write signed
    SONATA_GATT_CMP_WRITE_SIGNED                            = 0x040E,
    /// Execute write
    SONATA_GATT_CMP_EXEC_WRITE                              = 0x040F,
    /// Send an attribute notification
    SONATA_GATT_CMP_NOTIFY                                  = 0x0412,
    /// Send an attribute indication
    SONATA_GATT_CMP_INDICATE                                = 0x0413,

} sonata_ble_complete_type;

/// Callbacks for GAP module. If app finished works in callback, should return CB_DONE @see CBStatus.

typedef struct {
    /*************** GAP Manager  ***************/
    /// @deprecated use @see ble_complete_callback
    void (*ble_on_complete)(uint16_t status);
    /// @deprecated use @see ble_complete_callback
    void (*advertising_config_complete)(uint16_t status);
    /// @deprecated use @see ble_complete_callback
    void (*initiating_config_complete)(uint16_t status);
    /// @deprecated use @see ble_complete_callback
    void (*scanning_config_complete)(uint16_t status);
    /// @deprecated use @see ble_complete_callback
    void (*advertising_start_complete)(uint16_t status);
    /// @deprecated use @see ble_complete_callback
    void (*initiating_start_complete)(uint16_t status);
    /// @deprecated use @see ble_complete_callback
    void (*scanning_start_complete)(uint16_t status);
    /// @deprecated use @see ble_complete_callback
    void (*advertising_stopped_complete)(uint16_t reason);
    /// @deprecated use @see ble_complete_callback
    void (*initiating_stopped_complete)(uint16_t reason);
    /// @deprecated use @see ble_complete_callback
    void (*scanning_stopped_complete)(uint16_t reason);
    /// @deprecated use @see ble_complete_callback
    void (*advertising_deleted_complete)(uint16_t status);
    /// @deprecated use @see ble_complete_callback
    void (*initiating_deleted_complete)(uint16_t status);
    /// @deprecated use @see ble_complete_callback
    void (*scanning_deleted_complete)(uint16_t status);
    /// @deprecated use @see ble_complete_callback
    void (*set_advertising_data)(uint16_t status);
    /// @deprecated use @see ble_complete_callback
    void (*set_white_list_complete)(uint16_t status);
    /// @deprecated use @see ble_complete_callback

    /// Callback for getting device version, BT address, advertising TX power, antenna inf.
    uint16_t (*get_local_dev_info)(sonata_gap_local_dev_info info_type,
                                   void *info); // gap_dev_version,gap_dev_bdaddr,gap_dev_adv_tx_power,gap_antenna_inf,gap_dbg_mem_info

    /// Callback for scan result
    uint16_t (*gap_scan_result)(sonata_gap_ext_adv_report_ind_t *result);

    /// Callback for get peer name
    uint16_t (*gap_get_peer_name)(uint8_t addr_type, uint8_t *peer_addr, uint8_t name_len, uint8_t *name);

    /// Callback for random address has been solved
    uint16_t (*gap_addr_solved)(uint8_t *addr, uint8_t *key);

    /// Callback for AES-128 block result indication
    uint16_t (*gap_use_enc_block)(uint8_t *result);

    /// Callback for gap generate random number
    uint16_t (*gap_gen_random_number)(uint8_t *number);

    /// Callback for Resolving address list
    uint16_t (*gap_resolving_address)(uint8_t operation, uint8_t addr_type, uint8_t *addr);

    /// Callback for provide statistic information about ISO exchange
    uint16_t (*gap_iso_statistic)(sonata_gap_iso_stat_ind_t *param);

    /// Callback for DH Key computation is complete and available (GAP_P256_KEY_LEN)
    uint16_t (*gap_gen_dh_key)(uint8_t *key);

    /// Callback for public Key Pair value
    uint16_t (*gap_pubblic_key_pair_value)(uint8_t *keyX, uint8_t *keyY);

    /// Callback for reception of a scan request
    uint16_t (*gap_scan_request)(uint8_t actv_idx, uint8_t addr_type, uint8_t *addr);

    /// callback for indicate reception of a IQ Report event over a periodic advertising sync activity
    uint16_t (*gap_periodic_advertising_iq_report)(sonata_gap_per_adv_iq_report_ind_t *report);

    /// Callback for Indicate that synchronization has been successfully established with a periodic advertiser
    uint16_t (*gap_sync_established)(uint8_t actv_idx, uint8_t phy, uint16_t intv, uint8_t adv_sid,
                                     uint8_t clk_acc, uint16_t serv_data, uint8_t addr_type, uint8_t *addr);

    /*************** GAP Controller  ***************/
    /// @deprecated use @see ble_complete_callback
    uint16_t (*gap_disconnect_complete)(uint8_t conidx, uint16_t status);
    /// @deprecated use @see ble_complete_callback
    void (*gap_params_updated_complete)(uint8_t conidx, uint16_t status);
    /// @deprecated use @see ble_complete_callback
    void (*gap_bond_complete)(uint8_t conidx, uint16_t status);

    /// Callback for disconnect IND message
    uint16_t (*gap_disconnect_ind)(uint8_t conidx, uint16_t conhdl, uint8_t reason);

    /// Callback for param update request
    uint16_t (*gap_param_update_req)(uint8_t conidx, uint16_t intv_min, uint16_t intv_max, uint16_t latency,
                                     uint16_t time_out);

    /// Callback for param update
    uint16_t (*gap_param_updated)(uint8_t conidx, uint16_t con_interval, uint16_t con_latency, uint16_t sup_to);

    /// Callback for param update timeout
    uint16_t (*gap_param_update_timeout)(uint8_t conidx);

    /// Callback for peer device request set local device name information
    uint16_t (*gap_peer_set_local_device_name)(uint8_t conidx, uint16_t length, uint8_t *name);

    /// Callback for connection REQ message
    uint16_t (*gap_connection_req)(uint8_t conidx, sonata_gap_connection_req_ind_t *req);

    /// Callback for getting peer attribute info
    uint16_t (*gap_get_peer_att_info)(uint8_t conidx, sonata_gap_peer_att_info_ind_t *info);

    /// Callback for getting peer version, feature,device information @see sonata_peer_info_type
    uint16_t (*gap_get_peer_info)(uint8_t conidx, sonata_gap_peer_info_ind_t *info);

    /// Callback for peer device get local device informations.
    uint16_t (*gap_peer_get_local_info)(uint8_t conidx, sonata_gap_dev_info opt);

    /// Callback if advertising, initationg,scanning or period_sync active created
    uint16_t (*gap_active_created)(uint8_t actv_idx, uint8_t type, int8_t tx_pwr);

    /// Callback if advertising, initationg,scanning or period_sync active stopped.
    /// App can do some work and then delete the current active @see sonata_ble_delete_active() in this callback,
    /// Or app can return CB_REJECT @see CBStatus, SDK will delete the current active.
    /// But if app return CB_DONE, SDK will do nothing.
    uint16_t (*gap_active_stopped)(uint8_t actv_idx, uint8_t type, uint8_t reason, uint8_t per_adv_stop);

    /// Callback for profile added
    uint16_t (*gap_profile_added)(uint16_t prf_task_id, uint16_t prf_task_nb, uint16_t start_hdl);

    /// Callback for bond request
    uint16_t (*gap_bond_req)(uint8_t conidx, struct sonata_gap_bond_req_ind *request);
    /// Callback for bond
    uint16_t (*gap_bond)(uint8_t conidx, struct sonata_gap_bond_ind *ind);

    /// Callback for get le package size
    uint16_t (*gap_le_pkt_size)(uint8_t conidx, uint16_t max_tx_octets, uint16_t max_tx_time, uint16_t max_rx_octets,
                                uint16_t max_rx_time);

    /// Callback for encrypt request
    uint16_t (*gap_encrypt_req)(uint8_t conidx, uint16_t ediv, uint8_t *nb);

    /// Callback for encrypt indicate
    uint16_t (*gap_encrypt)(uint8_t conidx, uint8_t auth_level);

    /// Callback for security indicate
    uint16_t (*gap_security)(uint8_t conidx, uint8_t auth_level);

    /// Callback for sign counter
    uint16_t (*gap_sign_counter)(uint8_t conidx, uint32_t local_sign_counter, uint32_t peer_sign_counter);

    /// Callback for key press notification
    uint16_t (*gap_key_press_notification)(uint8_t conidx, uint8_t type);

    /// Callback for indicate reception of a IQ Report event over a ble connection
    uint16_t (*gap_cte_iq_report)(uint8_t conidx, sonata_gap_cte_iq_report_ind_t *param);

    /// Callback for signature result
    uint16_t (*gap_signature)(uint8_t conidx, uint8_t operation, uint16_t byte_len, uint8_t *signed_msg);

    /// Callback for pairing procedure timeout indication
    uint16_t (*gap_smp_timeout)(uint8_t conidx);
    /// Callback for pairing repeated attempts procedure timeout indication
    uint16_t (*gap_smp_rep_attempts_timeout)(uint8_t conidx);

} ble_gap_callback;

/// Ble complete event callback
typedef struct {
    uint16_t (*ble_complete_event)(sonata_ble_complete_type id, uint8_t status, uint16_t param, uint32_t dwparam);

} ble_complete_callback;

/// Ble gatt response event callback
typedef struct {
    uint16_t (*ble_rsp_event)(uint16_t opt_id, uint8_t status, uint16_t handle, uint16_t perm, uint16_t ext_perm,
                              uint16_t length, void *param);

} ble_response_callback;

/*!
 * @brief Turn on ble module
 * @param param @see sonata_gap_set_dev_config_cmd
 * @return API_SUCCESS
 */
uint16_t sonata_ble_on(sonata_gap_set_dev_config_cmd *param);

/*!
 * @brief Register GAP callback functions for ble module
 * @param cb @see ble_gap_callback
 * @return API_SUCCESS
 */
uint16_t sonata_ble_register_gap_callback(ble_gap_callback *cb);

/*!
 * @brief Register ble complete event callback functions
 * @param cb @see ble_complete_callback
 * @return API_SUCCESS
 */
uint16_t sonata_ble_register_complete_callback(ble_complete_callback *cb);

/*!
 * @brief Register ble response for GATT module
 * @param cb @see ble_response_callback
 * @return API_SUCCESS
 */
uint16_t sonata_ble_register_response_callback(ble_response_callback *cb);

/*!
 * @brief Config legacy advertising
 * @param own_addr_type: device's address type, @see sonata_gap_own_addr
 * @param param:data for advertising, @see sonata_gap_directed_adv_create_param_t
 * @return API_SUCCESS
 */
uint16_t sonata_ble_config_legacy_advertising(uint8_t own_addr_type, sonata_gap_directed_adv_create_param_t *param);

/*!
 * @brief Config extended advertising
 * @param own_addr_type: device's address type, @see sonata_gap_own_addr
 * @param param:data for advertising, @see sonata_gap_extended_adv_create_param_t
 * @return API_SUCCESS
 */
uint16_t sonata_ble_config_extended_advertising(uint8_t own_addr_type, sonata_gap_extended_adv_create_param_t *param);

// uint16_t sonata_ble_config_periodic_advertising(uint8_t own_addr_type, sonata_gap_periodic_adv_create_param_t *param);
/*!
 * @brief Config scanning data
 * @param own_addr_type: device's address type, @see sonata_gap_own_addr
 * @return API_SUCCESS
 */
uint16_t sonata_ble_config_scanning(uint8_t own_addr_type);

/*!
 * @brief Config initiating data
 * @param own_addr_type: device's address type, @see sonata_gap_own_addr
 * @return API_SUCCESS
 */
uint16_t sonata_ble_config_initiating(uint8_t own_addr_type);

/*!
 * @brief Set advertising data
 * @param length: Data length
 * @param data: Value for data
 * @return API_SUCCESS
 */
uint16_t sonata_ble_set_advertising_data(uint16_t length, uint8_t *data);

/*!
 * @brief Set advertising data
 * @param id The advertising id
 * @param length: Data length
 * @param data: Value for data
 * @return API_SUCCESS
 */
uint16_t sonata_ble_set_advertising_data_byid(uint8_t id, uint16_t length, uint8_t *data);

/*!
 * @brief Set scan response data
 * @param length: Data length
 * @param data: Value for data
 * @return API_SUCCESS
 */
uint16_t sonata_ble_set_scan_response_data(uint16_t length, uint8_t *data);

/*!
 * @brief Set scan response data
 * @param id The advertising id
 * @param length: Data length
 * @param data: Value for data
 * @return API_SUCCESS
 */
uint16_t sonata_ble_set_scan_response_data_byid(uint8_t id, uint16_t length, uint8_t *data);

/*!
 * @brief Start advertising
 * @param duration: Advertising duration (in unit of 10ms). 0 means that advertising continues until the host disable it
 * @param max_adv_evt: Maximum number of extended advertising events the controller shall attempt to send prior to
     terminating the extending advertising. Valid only if extended advertising
 * @return API_SUCCESS
 */
uint16_t sonata_ble_start_advertising(uint16_t duration, uint8_t max_adv_evt); /* uint8_t index */

/*!
 * @brief Start the advertising
 * @param id the advertisning id
 * @param duration: Advertising duration (in unit of 10ms). 0 means that advertising continues until the host disable it
 * @param max_adv_evt: Maximum number of extended advertising events the controller shall attempt to send prior to
     terminating the extending advertising. Valid only if extended advertising
 * @return API_SUCCESS
 */
uint16_t sonata_ble_start_advertising_byid(uint8_t id, uint16_t duration, uint8_t max_adv_evt);

/*!
 * @brief Stop advertising
 * @return API_SUCCESS
 */
uint16_t sonata_ble_stop_advertising(void);

/*!
 * @brief Stop the advertising
 * @param id the advertising id
 * @return API_SUCCESS
 */
uint16_t sonata_ble_stop_advertising_byid(uint8_t id);
/*!
 * @brief Start Scanning
 * @param param: @see sonata_gap_scan_param_t
 * @return API_SUCCESS
 */
uint16_t sonata_ble_start_scanning(sonata_gap_scan_param_t *param);

/*!
 * @brief Start the scanning
 * @param id The scanning id
 * @param param: @see sonata_gap_scan_param_t
 * @return API_SUCCESS
 */
uint16_t sonata_ble_start_scanning_byid(uint8_t id, sonata_gap_scan_param_t *param);
/*!
 * @brief Stop scanning
 * @return API_SUCCESS
 */
uint16_t sonata_ble_stop_scanning(void);

/*!
 * @brief Stop the scanning
 * @param id The scanning id
 * @return API_SUCCESS
 */
uint16_t sonata_ble_stop_scanning_byid(uint8_t id);
/*!
 * @brief Delete current advertising, initiating, scanning, or period_sync active
 * @param index active index
 * @return API_SUCCESS
 */
uint16_t sonata_ble_delete_active(uint8_t index);
/*!
 * @brief Start initiating
 * @param param: @see sonata_gap_init_param_t
 * @return API_SUCCESS
 */
uint16_t sonata_ble_start_initiating(sonata_gap_init_param_t *param);

/*!
 * @brief Start the initiating
 * @param id The initiating id
 * @param param: @see sonata_gap_init_param_t
 * @return API_SUCCESS
 */
uint16_t sonata_ble_start_initiating_byid(uint8_t id, sonata_gap_init_param_t *param);
/*!
 * @brief Stop initiating
 * @return API_SUCCESS
 */
uint16_t sonata_ble_stop_initiating(void);

/*!
 * @brief Stop the initiating
 * @param id The initiating id
 * @return API_SUCCESS
 */
uint16_t sonata_ble_stop_initiating_byid(uint8_t id);
/*!
 * @brief Send confirm message when received peer device get device name request
 * @param length length of the data
 * @param name   value of the name
 * @return API_SUCCESS
 */
uint16_t sonata_ble_gap_send_get_dev_info_cfm_for_dev_name(uint8_t conidx, uint16_t length, uint8_t *name);
/*!
 * @brief Send confirm message when received peer device get device appearance request
 * @param appearance
 * @return API_SUCCESS
 */
uint16_t sonata_ble_gap_send_get_dev_info_cfm_for_dev_appearance(uint8_t conidx, uint16_t appearance);

/*!
 * @brief Send confirm message when received peer device get device slave preferred parameters request
 * @param con_intv_min Connection interval minimum
 * @param con_intv_max Connection interval maximum
 * @param slave_latency Slave latency
 * @param conn_timeout Connection supervision timeout multiplier
 * @return API_SUCCESS
 */
uint16_t sonata_ble_gap_send_get_dev_info_cfm_for_slv_pref_params(uint8_t conidx, uint16_t con_intv_min,
        uint16_t con_intv_max, uint16_t slave_latency, uint16_t conn_timeout);

/*!
 * @brief Send confirm message when peer device set local device's name
 * @param conidx connecting index, used for multiple connection. for single connection, set it to 0.
 * @param status
 * @return API_SUCCESS
 */
uint16_t sonata_ble_gap_send_set_dev_name_cfm(uint8_t conidx, uint8_t status);
/*!
 * @brief Send confirm message when received peer device get device device central address resolution request
 * @param ctl_addr_resol Central address resolution
 * @return API_SUCCESS
 */
uint16_t sonata_ble_gap_send_get_dev_info_cfm_for_ctl_addr_resol(uint8_t conidx, uint8_t ctl_addr_resol);

/*!
 * @brief Set white list
 * @param size white list size
 * @param addrs white list values
 * @return API_SUCCESS
 */
uint16_t sonata_ble_gap_set_white_list(uint8_t size, struct sonata_gap_bdaddr *addrs);

/*!
 * @brief Set IRK
 * @param key
 * @return API_SUCCESS
 */
uint16_t sonata_ble_gap_set_irk(uint8_t *key);

/*!
 * @brief Update connection parameters
 * @param conidx connecting index, used for multiple connection. for single connection, set it to 0.
 * @param pkt_id Internal parameter used to manage internally l2cap packet identifier for signaling, set to 0 for default
 * @param intv_min Connection interval minimum (6~3200 means 7.5ms~4S)
 * @param intv_max Connection interval maximum (6~3200 means 7.5ms~4S)
 * @param latency Latency (0~499)
 * @param time_out Supervision timeout (10~3200 means 100ms~32S)
 * @param ce_len_min Minimum Connection Event Duration
 * @param ce_len_max Maximum Connection Event Duration
 * @return API_SUCCESS
 */
uint16_t sonata_ble_gap_update_connection_params(uint8_t conidx, uint8_t pkt_id, uint16_t intv_min, uint16_t intv_max,
        uint16_t latency,
        uint16_t time_out, uint16_t ce_len_min, uint16_t ce_len_max);

/*!
 * @brief Disconnect
 * @param conidx connecting index, used for multiple connection. for single connection, set it to 0.
 * @param reason disconnect reason
 * @return API_SUCCESS
 */
uint16_t sonata_ble_gap_disconnect(uint8_t conidx, uint8_t reason);
/*!
 * @brief Send bond confirm message for pairing request mode
 * @param conidx connecting index, used for multiple connection. for single connection, set it to 0.
 * @param reqInd @see sonata_gap_bond_req_ind
 * @param accept Request accepted
 * @param iocap IO capabilities (@see sonata_gap_io_cap)
 * @param oob OOB information (@see sonata_gap_oob)
 * @param auth Authentication (@see sonata_gap_auth)
 * @param key_size Encryption key size (7 to 16)
 * @param ikey_dist Initiator key distribution (@see sonata_gap_kdist)
 * @param rkey_dist Responder key distribution (@see sonata_gap_kdist)
 * @param sec_req Device security requirements (minimum security level). (@see sonata_gap_sec_req)
 * @return API_SUCCESS
 */
uint16_t sonata_ble_gap_send_bond_cfm_for_pairing_req(uint8_t conidx, struct sonata_gap_bond_req_ind *reqInd,
        uint8_t accept,
        enum sonata_gap_io_cap iocap, enum sonata_gap_oob_auth oob, enum sonata_gap_auth auth, uint8_t key_size,
        enum sonata_gap_kdist ikey_dist, enum sonata_gap_kdist rkey_dist, enum sonata_gap_sec_req sec_req);

/*!
 * @brief Send bond confirm message for ltk exchange
 * @param conidx connecting index, used for multiple connection. for single connection, set it to 0.
 * @param accept Request accepted
 * @param ediv Encryption Diversifier
 * @param randnb Random Number
 * @param key_size Encryption key size (7 to 16)
 * @param key Encryption key
 * @return API_SUCCESS
 */
uint16_t sonata_ble_gap_send_bond_cfm_for_ltk_exchange(uint8_t conidx, uint8_t accept, uint16_t ediv, uint8_t *randnb,
        uint8_t key_size, uint8_t *key);

/*!
 * @brief Send bond confirm for csrk exchange
 * @param conidx connecting index, used for multiple connection. for single connection, set it to 0.
 * @param accept Request accepted
 * @param csrk CSRK
 * @return API_SUCCESS
 */
uint16_t sonata_ble_gap_send_bond_cfm_for_csrk_exchange(uint8_t conidx, uint8_t accept, uint8_t *csrk);

/*!
 * @brief Send bond confirm for tk exchange
 * @param conidx connecting index, used for multiple connection. for single connection, set it to 0.
 * @param accept Request accepted
 * @param tk TK
 * @return API_SUCCESS
 */
uint16_t sonata_ble_gap_send_bond_cfm_for_tk_exchange(uint8_t conidx, uint8_t accept, uint8_t *tk);
/*!
 * @brief Send bond confirm for irk exchange
 * @param conidx connecting index, used for multiple connection. for single connection, set it to 0.
 * @param accept Request accepted
 * @param irk IRK
 * @param addr_type Address type of the device 0=public/1=private random
 * @param addr Device BD Identity Address
 * @return API_SUCCESS
 */
uint16_t sonata_ble_gap_send_bond_cfm_for_irk_exchange(uint8_t conidx, uint8_t accept, uint8_t *irk, uint8_t addr_type,
        uint8_t *addr);

/*!
 * @brief Send bond confirm for OOB exchange
 * @param conidx connecting index, used for multiple connection. for single connection, set it to 0.
 * @param accept Request accepted
 * @param oob_conf OOB confirm value
 * @param oob_rand OOB random value
 * @return API_SUCCESS
 */
uint16_t sonata_ble_gap_send_bond_cfm_for_oob_exchange(uint8_t conidx, uint8_t accept, uint8_t *oob_conf,
        uint8_t *oob_rand);
/*!
 * @brief Send bond confirm for Number compair exchange
 * @param conidx connecting index, used for multiple connection. for single connection, set it to 0.
 * @param accept Request accepted
 * @return API_SUCCESS
 */
uint16_t sonata_ble_gap_send_bond_cfm_for_nc_exchange(uint8_t conidx, uint8_t accept);
/*!
 * @brief Get local device informations
 * @param operation @see sonata_gap_local_dev_info
 * @return API_SUCCESS
 */

uint16_t sonata_ble_gap_get_dev_info(sonata_gap_local_dev_info operation);

/*!
 * @brief Resolve address command
 * @param address address value
 * @param nb_key  Number of provided IRK (sahlle be > 0)
 * @param irk  Array of IRK used for address resolution (MSB -> LSB)
 * @return  API_SUCCESS
 */
uint16_t sonata_ble_gap_resolve_address(uint8_t *address, uint8_t nb_key, struct sonata_gap_sec_key *irk);
/*!
 * @brief Generate random address
 * @param rnd_type @see sonata_gap_rnd_addr_type.
 *        - BD_ADDR_STATIC: Static random address;
 *        - BD_ADDR_NON_RSLV: Private non resolvable address;
 *        - BD_ADDR_RSLV: Private resolvable address
 * @param prand Dummy parameter used to store the prand part of the address
 * @return
 */
uint16_t sonata_ble_gap_generate_random_address(uint8_t rnd_type, uint8_t *prand);

/*!
 * @brief Generate random number
 * @return API_SUCCESS
 */
uint16_t sonata_ble_gap_generate_random_number(void);
/*!
 * @brief Send encrypt confirm
 * @param conidx connecting index, used for multiple connection. for single connection, set it to 0.
 * @param found
 * @param key_size
 * @param ltk
 * @return
 */
uint16_t sonata_ble_gap_send_encrypt_cfm(uint8_t conidx, uint8_t found, uint8_t key_size, uint8_t *ltk);

/*!
 * @brief Send key press notification
 * @param conidx
 * @param notification_type
 * @return
 */
uint16_t sonata_ble_gap_send_key_press_notification(uint8_t conidx, uint8_t notification_type);
/*!
 * @brief Start bond
 * @param conidx connecting index, used for multiple connection. for single connection, set it to 0.
 * @param iocap @see sonata_gap_io_cap
 * @param oob @see sonata_gap_oob
 * @param auth @see sonata_gap_auth
 * @param key_size @see SONATA_GAP_SMP_MAX_ENC_SIZE_LEN, @see SONATA_GAP_SMP_MIN_ENC_SIZE_LEN
 * @param ikey_dist @see sonata_gap_kdist
 * @param rkey_dist @see sonata_gap_kdist
 * @param sec_req @see sonata_gap_sec_req
 * @return API_SUCCESS
 */
uint16_t sonata_ble_gap_bond(uint8_t conidx, uint8_t iocap, uint8_t oob, uint8_t auth, uint8_t key_size,
                             uint8_t ikey_dist, uint8_t rkey_dist, uint8_t sec_req);
/*!
 * @brief Add profile module
 * @param prf_id Profile task identifier
 * @param sec_lvl Security Level
 * @param start_hdl Service start handle
 * @param value_len value length for parameter value
 * @param value value to initialize profile
 * @return API_SUCCESS
 */
uint16_t sonata_ble_gap_add_profile(sonata_ble_profile_id prf_id, uint8_t sec_lvl, uint16_t start_hdl,
                                    uint16_t value_len, void *value);

/*!
 * @brief Get peer information
 * @param conidx connecting index, used for multiple connection. for single connection, set it to 0.
 * @param operation @see sonata_peer_info_type
 * @return API_SUCCESS
 */
uint16_t sonata_ble_gap_get_peer_info(uint8_t conidx, sonata_peer_info_type operation);

/*!
 * @brief Start security
 * @param conidx connecting index, used for multiple connection. for single connection, set it to 0.
 * @param auth_level
 * @return API_SUCCESS
 */
uint16_t sonata_ble_gap_start_security(uint8_t conidx, uint8_t auth_level);

/*!
 * @brief set packet size
 * @param conidx connecting index, used for multiple connection. for single connection, set it to 0.
 * @param tx_octets
 * @param tx_time
 * @return API_SUCCESS
 */
uint16_t sonata_ble_gap_set_le_pkt_size(uint8_t conidx, uint16_t tx_octets, uint16_t tx_time);

/*!
 * @brief Send parameter update confirm command
 * @param conidx connecting index, used for multiple connection. for single connection, set it to 0.
 * @param accept  True to accept slave connection parameters, False else.
 * @param ce_len_min Minimum Connection Event Duration
 * @param ce_len_max Maximum Connection Event Duration
 * @return API_SUCCESS
 */
uint16_t sonata_ble_gap_send_param_update_cfm(uint8_t conidx, bool accept, uint16_t ce_len_min, uint16_t ce_len_max);

/*
 * FUNCTION DECLARATIONS             Connection help function
 ****************************************************************************************
 */
/*!
 * @brief Get connection index form peer address
 * @param peer_addr peer devices address
 * @return Connection index OR GAP_INVALID_CONIDX
 */
uint8_t sonata_ble_get_conidx(uint8_t *peer_addr);
/*!
 * @brief Get peer device's address form connection index
 * @param connection_id connection index
 * @return peer devices's  address.
 */
uint8_t *sonata_ble_get_peer_addr(uint8_t connection_id);

/*!
 * @brief Send connection confirm command
 * @param conidx connecting index, used for multiple connection. for single connection, set it to 0.
 * @param cfm @see sonata_gap_connection_cfm_t
 * @return API_SUCCESS
 */
uint16_t sonata_ble_gap_send_connection_cfm(uint8_t conidx, sonata_gap_connection_cfm_t *cfm);

/// @hide
void sonata_ble_show_connection_info(void);

struct sonata_gap_bdaddr *sonata_ble_gap_get_bdaddr(uint8_t conidx, uint8_t src);

/// @hide
uint16_t sonata_ble_gap_cte_set_tx_config(uint8_t conidx, uint8_t cte_types, uint8_t switching_pattern_len,
        uint8_t *antenna_id);

/// @hide
uint16_t sonata_ble_gap_cte_response_control(uint8_t conidx, bool enable);

/** @}*/

#endif // BLE_HOST_PRESENT

#endif // _SONATA_GAP_API_H_

