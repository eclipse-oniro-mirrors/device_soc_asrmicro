/**
 ****************************************************************************************
 *
 * @file sonata_gap_api.h
 *
 * @brief header file - gap api
 *
 * Copyright (C) ASR 2020 - 2029
 *
 *
 ****************************************************************************************
 */


#ifndef _SONATA_GAP_API_INT_H_
#define _SONATA_GAP_API_INT_H_



/*
 * INCLUDE FILES
 ****************************************************************************************
 */
#include "rwip_config.h"

#if BLE_HOST_PRESENT

#include "sonata_utils_api.h"
#include "gapm_task.h"
#include "gapc_task.h"
#include "sonata_gap_api.h"
#include "co_bt_defines.h"


/*
 * MACRO DEFINITIONS
 ****************************************************************************************
 */
#define SONATA_GAP_DEBUG (1)
#define APP_ACTIVITY_MAX (CFG_ACT)
#define APP_CONN_MAX     (CFG_CON)

#define SONATA_CMP_INDEX_GAP               (0x01)
#define SONATA_CMP_INDEX_GAPC              (0x02)
#define SONATA_CMP_INDEX_GATTM             (0x03)
#define SONATA_CMP_INDEX_GATTC             (0x04)


#define SONATA_CMP_BUILD_ID(type, index)   ((uint16_t)(((index) << 8)|(type)))
#define SONATA_CMP_TYPE_GET(id)            ((id) & 0xFF)
#define SONATA_CMP_IDX_GET(id)             (((id) >> 8) & 0xFF)

/*
 * ENUM DEFINITIONS
 ****************************************************************************************
 */
typedef enum
{
    /* Default event */
    /// Command Complete event
    SONATA_GAPM_CMP_EVT                        = MSG_ID(GAPM, 0x00),

    /* Default commands */
    /// Reset link layer and the host command
    SONATA_GAP_RESET_CMD                      = MSG_ID(GAPM, 0x02),

    /* Device Configuration */
    /// Set device configuration command
    SONATA_GAP_SET_DEV_CONFIG_CMD             = MSG_ID(GAPM, 0x04),
    /// Set device channel map
    SONATA_GAP_SET_CHANNEL_MAP_CMD            = MSG_ID(GAPM, 0x05),

    /* Local device information */
    /// Get local device info command
    SONATA_GAP_GET_DEV_INFO_CMD               = MSG_ID(GAPM, 0x06),
    /// Local device version indication event
    SONATA_GAP_DEV_VERSION_IND                = MSG_ID(GAPM, 0x07),
    /// Local device BD Address indication event
    SONATA_GAP_DEV_BDADDR_IND                 = MSG_ID(GAPM, 0x08),
    /// Advertising channel Tx power level
    SONATA_GAP_DEV_ADV_TX_POWER_IND           = MSG_ID(GAPM, 0x09),
    /// Indication containing information about memory usage.
    SONATA_GAP_DBG_MEM_INFO_IND               = MSG_ID(GAPM, 0x0A),
    /// Indication containing controller antenna information
    SONATA_GAP_ANTENNA_INF_IND                = MSG_ID(GAPM, 0x0B),

    /// Name of peer device indication
    SONATA_GAP_PEER_NAME_IND                  = MSG_ID(GAPM, 0x12),

    /* Security / Encryption Toolbox */
    /// Resolve address command
    SONATA_GAP_RESOLV_ADDR_CMD                = MSG_ID(GAPM, 0x14),
    /// Indicate that resolvable random address has been solved
    SONATA_GAP_ADDR_SOLVED_IND                = MSG_ID(GAPM, 0x15),
    /// Generate a random address.
    SONATA_GAP_GEN_RAND_ADDR_CMD              = MSG_ID(GAPM, 0x16),
    /// Use the AES-128 block in the controller
    SONATA_GAP_USE_ENC_BLOCK_CMD              = MSG_ID(GAPM, 0x17),
    ///  AES-128 block result indication
    SONATA_GAP_USE_ENC_BLOCK_IND              = MSG_ID(GAPM, 0x18),
    /// Generate a 8-byte random number
    SONATA_GAP_GEN_RAND_NB_CMD                = MSG_ID(GAPM, 0x19),
    /// Random Number Indication
    SONATA_GAP_GEN_RAND_NB_IND                = MSG_ID(GAPM, 0x1A),

    /* Profile Management */
    /// Create new task for specific profile
    SONATA_GAP_PROFILE_TASK_ADD_CMD           = MSG_ID(GAPM, 0x1B),
    /// Inform that profile task has been added.
    SONATA_GAP_PROFILE_ADDED_IND              = MSG_ID(GAPM, 0x1C),

    /// Indicate that a message has been received on an unknown task
    SONATA_GAP_UNKNOWN_TASK_IND               = MSG_ID(GAPM, 0x1D),

    /* Data Length Extension */
    /// Suggested Default Data Length indication
    SONATA_GAP_SUGG_DFLT_DATA_LEN_IND         = MSG_ID(GAPM, 0x1E),
    /// Maximum Data Length indication
    SONATA_GAP_MAX_DATA_LEN_IND               = MSG_ID(GAPM, 0x1F),

    /* Resolving list for controller-based privacy*/
    /// Resolving address list address indication
    SONATA_GAP_RAL_ADDR_IND                   = MSG_ID(GAPM, 0x22),

    /* Set new IRK */
    /// Modify current IRK
    SONATA_GAP_SET_IRK_CMD                    = MSG_ID(GAPM, 0x23),

    /* LE Protocol/Service Multiplexer Management */
    /// Register a LE Protocol/Service Multiplexer command
    SONATA_GAP_LEPSM_REGISTER_CMD             = MSG_ID(GAPM, 0x24),
    /// Unregister a LE Protocol/Service Multiplexer command
    SONATA_GAP_LEPSM_UNREGISTER_CMD           = MSG_ID(GAPM, 0x25),

    /* LE Test Mode */
    /// Control of the test mode command
    SONATA_GAP_LE_TEST_MODE_CTRL_CMD          = MSG_ID(GAPM, 0x26),
    /// Indicate end of test mode
    SONATA_GAP_LE_TEST_END_IND                = MSG_ID(GAPM, 0x27),
    /// Indicate reception of a IQ report in LE test mode
    SONATA_GAP_LE_TEST_IQ_REPORT_IND          = MSG_ID(GAPM, 0x2D),

    /// Provide statistic information about ISO exchange
    SONATA_GAP_ISO_STAT_IND                   = MSG_ID(GAPM, 0x28),

    /* Secure Connections */
    /// Request to provide DH Key
    SONATA_GAP_GEN_DH_KEY_CMD                 = MSG_ID(GAPM, 0x29),
    /// Indicates the DH Key computation is complete and available
    SONATA_GAP_GEN_DH_KEY_IND                 = MSG_ID(GAPM, 0x2A),
    /// Retrieve Public Key
    SONATA_GAP_GET_PUB_KEY_CMD                = MSG_ID(GAPM, 0x2B),
    /// Indicates the Public Key Pair value
    SONATA_GAP_PUB_KEY_IND                    = MSG_ID(GAPM, 0x2C),


    /* ************************************************ */
    /* ------------ NEW COMMANDS FOR BLE 5 ------------ */
    /* ************************************************ */

    /* List Management Operations */
    /// Get local or peer address
    /// @see struct gapm_get_ral_addr_cmd
    SONATA_GAP_GET_RAL_ADDR_CMD               = MSG_ID(GAPM, 0x90),
    /// Set content of either white list or resolving list or periodic advertiser list
    /// @see struct gapm_list_set_wl_cmd
    /// @see struct gapm_list_set_ral_cmd
    /// @see struct gapm_list_set_pal_cmd
    SONATA_GAP_LIST_SET_CMD                   = MSG_ID(GAPM, 0x91),
    /// Indicate size of list indicated in SONATA_GAP_GET_DEV_CONFIG_CMD message
    /// @see struct gapm_list_size_ind
    SONATA_GAP_LIST_SIZE_IND                  = MSG_ID(GAPM, 0x92),

    /* Extended Air Operations */
    /// Create an advertising, a scanning, an initiating or a periodic synchronization activity
    /// @see struct gapm_activity_create_cmd
    /// @see struct gapm_activity_create_adv_cmd
    SONATA_GAP_ACTIVITY_CREATE_CMD            = MSG_ID(GAPM, 0xA0),
    /// Start a previously created activity
    /// @see struct gapm_activity_start_cmd
    SONATA_GAP_ACTIVITY_START_CMD             = MSG_ID(GAPM, 0xA1),
    /// Stop either a given activity or all existing activities
    /// @see struct gapm_activity_stop_cmd
    SONATA_GAP_ACTIVITY_STOP_CMD              = MSG_ID(GAPM, 0xA2),
    /// Delete either a given activity or all existing activities
    /// @see struct gapm_activity_delete_cmd
    SONATA_GAP_ACTIVITY_DELETE_CMD            = MSG_ID(GAPM, 0xA3),
    /// Indicate that an activity has well been created
    /// @see struct gapm_activity_create_ind
    SONATA_GAP_ACTIVITY_CREATED_IND           = MSG_ID(GAPM, 0xA4),
    /// Indicate that an activity has been stopped and can be restarted
    /// @see struct gapm_activity_stopped_ind
    SONATA_GAP_ACTIVITY_STOPPED_IND           = MSG_ID(GAPM, 0xA5),
    /// Set either advertising data or scan response data or periodic advertising data
    /// @see struct gapm_set_adv_data_cmd
    SONATA_GAP_SET_ADV_DATA_CMD               = MSG_ID(GAPM, 0xA6),
    /// Indicate reception of an advertising report (periodic or not), a scan response report
    /// @see struct gapm_ext_adv_report_ind
    SONATA_GAP_EXT_ADV_REPORT_IND             = MSG_ID(GAPM, 0xA7),
    /// Indicate reception of a scan request
    /// @see struct gapm_scan_request_ind
    SONATA_GAP_SCAN_REQUEST_IND               = MSG_ID(GAPM, 0xA8),
    /// Indicate that synchronization has been successfully established with a periodic advertiser
    /// @see struct gapm_sync_established_ind
    SONATA_GAP_SYNC_ESTABLISHED_IND           = MSG_ID(GAPM, 0xA9),
    /// Indicate maximum advertising data length supported by controller
    /// @see struct gapm_max_adv_data_len_ind
    SONATA_GAP_MAX_ADV_DATA_LEN_IND           = MSG_ID(GAPM, 0xAA),
    /// Indicate number of available advertising sets
    /// @see struct gapm_nb_adv_sets_ind
    SONATA_GAP_NB_ADV_SETS_IND                = MSG_ID(GAPM, 0xAB),
    /// Indicate the transmit powers supported by the controller
    /// @see struct gapm_dev_tx_power_ind
    SONATA_GAP_DEV_TX_PWR_IND                 = MSG_ID(GAPM, 0xAC),
    /// Indicate the RF path compensation values
    /// @see struct gapm_dev_rf_path_comp_ind
    SONATA_GAP_DEV_RF_PATH_COMP_IND           = MSG_ID(GAPM, 0xAD),
    /// Indication to the task that sends the unknown message
    /// @see struct gapm_unknown_msg_ind
    SONATA_GAPM_UNKNOWN_MSG_IND                = MSG_ID(GAPM, 0xAE),
    /// Control reception or not of Periodic Advertising Report in a Periodic Advertising Sync activity
    /// @see struct gapm_per_adv_report_ctrl_cmd
    SONATA_GAP_PER_ADV_REPORT_CTRL_CMD        = MSG_ID(GAPM, 0xAF),
    /// Control capturing IQ samples from the Constant Tone Extension of periodic advertising packets
    SONATA_GAP_PER_SYNC_IQ_SAMPLING_CTRL_CMD  = MSG_ID(GAPM, 0xB0),
    /// Indicate reception of a IQ Report event over a periodic advertising sync activity
    SONATA_GAP_PER_ADV_IQ_REPORT_IND          = MSG_ID(GAPM, 0xB1),
    /// Control CTE transmission in a periodic advertising activity
    SONATA_GAP_PER_ADV_CTE_TX_CTL_CMD         = MSG_ID(GAPM, 0xB2),

    /* ************************************************ */
    /* ---------------- DEBUG COMMANDS ---------------- */
    /* ************************************************ */
    /// Configure the Debug Platform I&Q Sampling generator
    SONATA_GAP_DBG_IQGEN_CFG_CMD              = MSG_ID(GAPM, 0x50),

    /* ************************************************ */
    /* -------------- Internal usage only ------------- */
    /* ************************************************ */
    /// Message received to unknown task received
    SONATA_GAP_UNKNOWN_TASK_MSG               = MSG_ID(GAPM, 0xF0),

    /* Internal messages for timer events, not part of API */
    /// Address renewal timeout indication
    SONATA_GAP_ADDR_RENEW_TO_IND              = MSG_ID(GAPM, 0xF1),
    /// Automatic connection establishment timeout indication
    SONATA_GAP_AUTO_CONN_TO_IND               = MSG_ID(GAPM, 0xF2),

    /* Addresses Management */
    /// Renew random private addresses
    /// @see struct gapm_addr_renew_cmd
    SONATA_GAP_ADDR_RENEW_CMD                 = MSG_ID(GAPM, 0xF3),
}sonata_gapm_msg_id;



typedef enum
{
    /// Disable write access
    SONATA_GAP_WRITE_DISABLE     = 0,
    /// Enable write access - no authentication required
    SONATA_GAP_WRITE_NO_AUTH     = 1,
    /// Write access requires unauthenticated link
    SONATA_GAP_WRITE_UNAUTH      = 2,
    /// Write access requires authenticated link
    SONATA_GAP_WRITE_AUTH        = 3,
    /// Write access requires secure connected link
    SONATA_GAP_WRITE_SEC_CON     = 4
}sonata_gap_write_att_perm;

///// Attribute database configuration
/////   15   14   13   12   11   10    9    8    7    6    5    4    3    2    1    0
///// +----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+
///// | DBG|DBGT|               RFU                |PCP |   APP_PERM   |   NAME_PERM  |
///// +----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+
///// - Bit [0-2] : Device Name write permission requirements for peer device (@see gapm_write_att_perm)
///// - Bit [3-5] : Device Appearance write permission requirements for peer device (@see gapm_write_att_perm)
///// - Bit [6]   : Slave Preferred Connection Parameters present
///// - Bit [7-13]: Reserved
///// - Bit [14]  : if Debug Mode enabled, forward all L2cap traffic to application
///// - Bit [15]  : Enable Debug Mode
//typedef enum
//{
//    /// Device Name write permission requirements for peer device (@see gapm_write_att_perm)
//    SONATA_GAP_ATT_NAME_PERM_MASK            = 0x0007,
//    SONATA_GAP_ATT_NAME_PERM_LSB             = 0x00,
//    /// Device Appearance write permission requirements for peer device (@see gapm_write_att_perm)
//    SONATA_GAP_ATT_APPEARENCE_PERM_MASK      = 0x0038,
//    SONATA_GAP_ATT_APPEARENCE_PERM_LSB       = 0x03,
//    /// Slave Preferred Connection Parameters present in GAP attribute database.
//    SONATA_GAP_ATT_SLV_PREF_CON_PAR_EN_MASK  = 0x0040,
//    SONATA_GAP_ATT_SLV_PREF_CON_PAR_EN_LSB   = 0x06,
//    /// if Debug Mode enabled, forward all L2cap traffic to application
//    SONATA_GAP_ATT_DBG_L2CAP_TRAFFIC_EN_MASK = 0x4000,
//    SONATA_GAP_ATT_DBG_L2CAP_TRAFFIC_EN_LSB  = 0x0E,
//    /// Enable Debug Mode
//    SONATA_GAP_ATT_DBG_MODE_EN_MASK          = 0x8000,
//    SONATA_GAP_ATT_DBG_MODE_EN_LSB           = 0x0F,
//}sonata_gapm_att_cfg_flag;

///// Pairing mode authorized on the device
/////    7    6    5    4    3    2    1    0
///// +----+----+----+----+----+----+----+----+
///// |               RFU           | SCP| LP |
///// +----+----+----+----+----+----+----+----+
//enum sonata_gap_pairing_mode
//{
//    /// No pairing authorized
//    SONATA_GAP_PAIRING_DISABLE  = 0,
//    /// Legacy pairing Authorized
//    SONATA_GAP_PAIRING_LEGACY   = (1 << 0),
//    /// Secure Connection pairing Authorized
//    SONATA_GAP_PAIRING_SEC_CON  = (1 << 1),
//};

/// LE Audio Mode Configuration
///   15   14   13   12   11   10    9    8    7    6    5    4    3    2    1    0
/// +----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+
/// |                                 RFU                                      | AM0|
/// +----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+
typedef enum
{
    /// LE Audio Mode 0 Supported
    SONATA_GAP_MASK_AUDIO_AM0_SUP           = 0x0001,
    SONATA_GAP_POS_AUDIO_AM0_SUP            = 0x00,
}sonata_gapm_audio_cfg_flag;

/// Security level
///   7   6   5   4   3   2   1   0
/// +---+---+---+---+---+---+---+---+
/// |MI |      RFU      |EKS|SEC_LVL|
/// +---+---+---+---+---+---+---+---+
/// bit[0-1]: Security level requirement (0=NO_AUTH, 1=UNAUTH, 2=AUTH, 3=SEC_CON)
/// bit[2]  : Encryption Key Size length must have 16 bytes
/// bit[7]  : Multi-instantiated task
typedef enum
{
    /// bit[0-1]: Security level requirement (0=NO_AUTH, 1=UNAUTH, 2=AUTH, 3=SEC_CON)
    /// bit[2]  : Encryption Key Size length must have 16 bytes
    SONATA_GAP_LE_PSM_SEC_LVL_MASK  = 0x07,
    /// bit[7]  : Multi-instantiated task
    SONATA_GAP_LE_PSM_MI_TASK_MASK  = 0x80,
}sonata_gapm_le_psm_sec_mask;

/// Type of activities that can be created
/*@TRACE*/
typedef enum
{
    /// Advertising activity
    SONATA_GAP_ACTV_TYPE_ADV = 0,
    /// Scanning activity
    SONATA_GAP_ACTV_TYPE_SCAN,
    /// Initiating activity
    SONATA_GAP_ACTV_TYPE_INIT,
    /// Periodic synchronization activity
    SONATA_GAP_ACTV_TYPE_PER_SYNC,
}sonata_gapm_actv_type;



/// Advertising report type
typedef enum
{
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
}sonata_gapm_adv_report_type;

///// Advertising properties bit field bit positions
//enum sonata_gap_adv_prop_bf
//{
//    /// Indicate that advertising is connectable, reception of CONNECT_REQ or AUX_CONNECT_REQ
//    /// PDUs is accepted. Not applicable for periodic advertising.
//    SONATA_GAP_ADV_PROP_CONNECTABLE_POS     = 0,
//    SONATA_GAP_ADV_PROP_CONNECTABLE_BIT     = CO_BIT(SONATA_GAP_ADV_PROP_CONNECTABLE_POS),
//
//    /// Indicate that advertising is scannable, reception of SCAN_REQ or AUX_SCAN_REQ PDUs is
//    /// accepted
//    SONATA_GAP_ADV_PROP_SCANNABLE_POS       = 1,
//    SONATA_GAP_ADV_PROP_SCANNABLE_BIT       = CO_BIT(SONATA_GAP_ADV_PROP_SCANNABLE_POS),
//
//    /// Indicate that advertising targets a specific device. Only apply in following cases:
//    ///   - Legacy advertising: if connectable
//    ///   - Extended advertising: connectable or (non connectable and non discoverable)
//    SONATA_GAP_ADV_PROP_DIRECTED_POS        = 2,
//    SONATA_GAP_ADV_PROP_DIRECTED_BIT        = CO_BIT(SONATA_GAP_ADV_PROP_DIRECTED_POS),
//
//    /// Indicate that High Duty Cycle has to be used for advertising on primary channel
//    /// Apply only if created advertising is not an extended advertising
//    SONATA_GAP_ADV_PROP_HDC_POS             = 3,
//    SONATA_GAP_ADV_PROP_HDC_BIT             = CO_BIT(SONATA_GAP_ADV_PROP_HDC_POS),
//
//    /// Bit 4 is reserved
//    SONATA_GAP_ADV_PROP_RESERVED_4_POS      = 4,
//    SONATA_GAP_ADV_PROP_RESERVED_4_BIT      = CO_BIT(SONATA_GAP_ADV_PROP_RESERVED_4_POS),
//
//    /// Enable anonymous mode. Device address won't appear in send PDUs
//    /// Valid only if created advertising is an extended advertising
//    SONATA_GAP_ADV_PROP_ANONYMOUS_POS       = 5,
//    SONATA_GAP_ADV_PROP_ANONYMOUS_BIT       = CO_BIT(SONATA_GAP_ADV_PROP_ANONYMOUS_POS),
//
//    /// Include TX Power in the extended header of the advertising PDU.
//    /// Valid only if created advertising is not a legacy advertising
//    SONATA_GAP_ADV_PROP_TX_PWR_POS          = 6,
//    SONATA_GAP_ADV_PROP_TX_PWR_BIT          = CO_BIT(SONATA_GAP_ADV_PROP_TX_PWR_POS),
//
//    /// Include TX Power in the periodic advertising PDU.
//    /// Valid only if created advertising is a periodic advertising
//    SONATA_GAP_ADV_PROP_PER_TX_PWR_POS      = 7,
//    SONATA_GAP_ADV_PROP_PER_TX_PWR_BIT      = CO_BIT(SONATA_GAP_ADV_PROP_PER_TX_PWR_POS),
//
//    /// Indicate if application must be informed about received scan requests PDUs
//    SONATA_GAP_ADV_PROP_SCAN_REQ_NTF_EN_POS = 8,
//    SONATA_GAP_ADV_PROP_SCAN_REQ_NTF_EN_BIT = CO_BIT(SONATA_GAP_ADV_PROP_SCAN_REQ_NTF_EN_POS),
//};

///// Advertising discovery mode
//enum sonata_gap_adv_disc_mode
//{
//    /// Mode in non-discoverable
//    SONATA_GAP_ADV_MODE_NON_DISC = 0,
//    /// Mode in general discoverable
//    SONATA_GAP_ADV_MODE_GEN_DISC,
//    /// Mode in limited discoverable
//    SONATA_GAP_ADV_MODE_LIM_DISC,
//    /// Broadcast mode without presence of AD_TYPE_FLAG in advertising data
//    SONATA_GAP_ADV_MODE_BEACON,
//    SONATA_GAP_ADV_MODE_MAX,
//};

///// Scanning Types
//enum sonata_gap_scan_type
//{
//    /// General discovery
//    SONATA_GAP_SCAN_TYPE_GEN_DISC = 0,
//    /// Limited discovery
//    SONATA_GAP_SCAN_TYPE_LIM_DISC,
//    /// Observer
//    SONATA_GAP_SCAN_TYPE_OBSERVER,
//    /// Selective observer
//    SONATA_GAP_SCAN_TYPE_SEL_OBSERVER,
//    /// Connectable discovery
//    SONATA_GAP_SCAN_TYPE_CONN_DISC,
//    /// Selective connectable discovery
//    SONATA_GAP_SCAN_TYPE_SEL_CONN_DISC,
//};

///// Scanning properties bit field bit value
//enum sonata_gap_scan_prop
//{
//    /// Scan advertisement on the LE 1M PHY
//    SONATA_GAP_SCAN_PROP_PHY_1M_BIT       = (1 << 0),
//    /// Scan advertisement on the LE Coded PHY
//    SONATA_GAP_SCAN_PROP_PHY_CODED_BIT    = (1 << 1),
//    /// Active scan on LE 1M PHY (Scan Request PDUs may be sent)
//    SONATA_GAP_SCAN_PROP_ACTIVE_1M_BIT    = (1 << 2),
//    /// Active scan on LE Coded PHY (Scan Request PDUs may be sent)
//    SONATA_GAP_SCAN_PROP_ACTIVE_CODED_BIT = (1 << 3),
//    /// Accept directed advertising packets if we use a RPA and target address cannot be solved by the
//    /// controller
//    SONATA_GAP_SCAN_PROP_ACCEPT_RPA_BIT   = (1 << 4),
//    /// Filter truncated advertising or scan response reports
//    SONATA_GAP_SCAN_PROP_FILT_TRUNC_BIT   = (1 << 5),
//};

///// Initiating Types
//enum sonata_gap_init_type
//{
//    /// Direct connection establishment, establish a connection with an indicated device
//    SONATA_GAP_INIT_TYPE_DIRECT_CONN_EST = 0,
//    /// Automatic connection establishment, establish a connection with all devices whose address is
//    /// present in the white list
//    SONATA_GAP_INIT_TYPE_AUTO_CONN_EST,
//    /// Name discovery, Establish a connection with an indicated device in order to read content of its
//    /// Device Name characteristic. Connection is closed once this operation is stopped.
//    SONATA_GAP_INIT_TYPE_NAME_DISC,
//};

///// Initiating Properties
//enum sonata_gap_init_prop
//{
//    /// Scan connectable advertisements on the LE 1M PHY. Connection parameters for the LE 1M PHY are provided
//    SONATA_GAP_INIT_PROP_1M_BIT       = (1 << 0),
//    /// Connection parameters for the LE 2M PHY are provided
//    SONATA_GAP_INIT_PROP_2M_BIT       = (1 << 1),
//    /// Scan connectable advertisements on the LE Coded PHY. Connection parameters for the LE Coded PHY are provided
//    SONATA_GAP_INIT_PROP_CODED_BIT    = (1 << 2),
//};

/// Advertising report information
typedef enum
{
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
}sonata_gapm_adv_report_info;

///// Filtering policy for duplicated packets
//enum sonata_gap_dup_filter_pol
//{
//    /// Disable filtering of duplicated packets
//    SONATA_GAP_DUP_FILT_DIS = 0,
//    /// Enable filtering of duplicated packets
//    SONATA_GAP_DUP_FILT_EN,
//    /// Enable filtering of duplicated packets, reset for each scan period
//    SONATA_GAP_DUP_FILT_EN_PERIOD,
//};

/// Periodic synchronization types
typedef enum
{
    /// Do not use periodic advertiser list for synchronization. Use advertiser information provided
    /// in the SONATA_GAP_ACTIVITY_START_CMD.
    SONATA_GAP_PER_SYNC_TYPE_GENERAL = 0,
    /// Use periodic advertiser list for synchronization
    SONATA_GAP_PER_SYNC_TYPE_SELECTIVE,
    /// Use Periodic advertising sync transfer information send through connection for synchronization
    SONATA_GAP_PER_SYNC_TYPE_PAST,
}sonata_gapm_per_sync_type;

/// PHY Type
typedef enum
{
    /// LE 1M
    SONATA_GAP_PHY_TYPE_LE_1M = 0,
    /// LE 2M
    SONATA_GAP_PHY_TYPE_LE_2M,
    /// LE Coded
    SONATA_GAP_PHY_TYPE_LE_CODED,
}sonata_gapm_phy_type;

/// -------------------------------------------------------------------------------------
/// Masks for advertising properties
/// -------------------------------------------------------------------------------------

///// Advertising properties configurations for legacy advertising
//enum sonata_gap_leg_adv_prop
//{
//    /// Non connectable non scannable advertising
//    SONATA_GAP_ADV_PROP_NON_CONN_NON_SCAN_MASK  = 0x0000,
//    /// Broadcast non scannable advertising - Discovery mode must be Non Discoverable
//    SONATA_GAP_ADV_PROP_BROADCAST_NON_SCAN_MASK = SONATA_GAP_ADV_PROP_NON_CONN_NON_SCAN_MASK,
//    /// Non connectable scannable advertising
//    SONATA_GAP_ADV_PROP_NON_CONN_SCAN_MASK      = SONATA_GAP_ADV_PROP_SCANNABLE_BIT,
//    /// Broadcast non scannable advertising - Discovery mode must be Non Discoverable
//    SONATA_GAP_ADV_PROP_BROADCAST_SCAN_MASK     = SONATA_GAP_ADV_PROP_NON_CONN_SCAN_MASK,
//    /// Undirected connectable advertising
//    SONATA_GAP_ADV_PROP_UNDIR_CONN_MASK         = SONATA_GAP_ADV_PROP_CONNECTABLE_BIT | SONATA_GAP_ADV_PROP_SCANNABLE_BIT,
//    /// Directed connectable advertising
//    SONATA_GAP_ADV_PROP_DIR_CONN_MASK           = SONATA_GAP_ADV_PROP_DIRECTED_BIT | SONATA_GAP_ADV_PROP_CONNECTABLE_BIT,
//    /// Directed connectable with Low Duty Cycle
//    SONATA_GAP_ADV_PROP_DIR_CONN_LDC_MASK       = SONATA_GAP_ADV_PROP_DIR_CONN_MASK,
//    /// Directed connectable with High Duty Cycle
//    SONATA_GAP_ADV_PROP_DIR_CONN_HDC_MASK       = SONATA_GAP_ADV_PROP_DIR_CONN_MASK | SONATA_GAP_ADV_PROP_HDC_BIT,
//};

///// Advertising properties configurations for extended advertising
//typedef enum
//{
//    /// Non connectable non scannable extended advertising
//    SONATA_GAP_EXT_ADV_PROP_NON_CONN_NON_SCAN_MASK = 0x0000,
//    /// Non connectable scannable extended advertising
//    SONATA_GAP_EXT_ADV_PROP_NON_CONN_SCAN_MASK     = SONATA_GAP_ADV_PROP_SCANNABLE_BIT,
//    /// Non connectable scannable directed extended advertising
//    SONATA_GAP_EXT_ADV_PROP_NON_CONN_SCAN_DIR_MASK = SONATA_GAP_ADV_PROP_SCANNABLE_BIT | SONATA_GAP_ADV_PROP_DIRECTED_BIT,
//    /// Non connectable anonymous directed extended advertising
//    SONATA_GAP_EXT_ADV_PROP_ANONYM_DIR_MASK        = SONATA_GAP_ADV_PROP_ANONYMOUS_BIT | SONATA_GAP_ADV_PROP_DIRECTED_BIT,
//    /// Undirected connectable extended advertising
//    SONATA_GAP_EXT_ADV_PROP_UNDIR_CONN_MASK        = SONATA_GAP_ADV_PROP_CONNECTABLE_BIT,
//    /// Directed connectable extended advertising
//    SONATA_GAP_EXT_ADV_PROP_DIR_CONN_MASK          = SONATA_GAP_ADV_PROP_CONNECTABLE_BIT | SONATA_GAP_ADV_PROP_DIRECTED_BIT,
//}sonata_gapm_ext_adv_prop;

/// Advertising properties configurations for periodic advertising
typedef enum
{
    /// Undirected periodic advertising
    SONATA_GAP_PER_ADV_PROP_UNDIR_MASK = 0x0000,
    /// Directed periodic advertising
    SONATA_GAP_PER_ADV_PROP_DIR_MASK   = SONATA_GAP_ADV_PROP_DIRECTED_BIT,
}sonata_gapm_per_adv_prop;

/// Clock accuracy values
typedef enum
{
    /// 500 ppm
    SONATA_GAP_CLK_ACC_500 = 0,
    /// 250 ppm
    SONATA_GAP_CLK_ACC_250,
    /// 150 ppm
    SONATA_GAP_CLK_ACC_150,
    /// 100 ppm
    SONATA_GAP_CLK_ACC_100,
    /// 75 ppm
    SONATA_GAP_CLK_ACC_75,
    /// 50 ppm
    SONATA_GAP_CLK_ACC_50,
    /// 30 ppm
    SONATA_GAP_CLK_ACC_30,
    /// 20 ppm
    SONATA_GAP_CLK_ACC_20,
}sonata_gapm_clk_acc;

///// Privacy configuration
//typedef enum
//{
//    /// Indicate if identity address is a public (0) or static private random (1) address
//    SONATA_GAP_PRIV_CFG_PRIV_ADDR_BIT = (1 << 0),
//    /// Reserved
//    SONATA_GAP_PRIV_CFG_RSVD          = (1 << 1),
//    /// Indicate if controller privacy is enabled
//    SONATA_GAP_PRIV_CFG_PRIV_EN_BIT   = (1 << 2),
//}sonata_gapm_priv_cfg;

/// Constant Tone Extension sync filtering type
typedef enum
{
    /// Do not sync to packets with an AoA Constant Tone Extension
    SONATA_GAP_CTE_NO_SYNC_WITH_AOA          = (1 << 0),
    /// Do not sync to packets with an AoD Constant Tone Extension with 1 us slots
    SONATA_GAP_CTE_NO_SYNC_WITH_AOD_1US_SLOT = (1 << 1),
    /// Do not sync to packets with an AoD Constant Tone Extension with 2 us slots
    SONATA_GAP_CTE_NO_SYNC_WITH_AOD_2US_SLOT = (1 << 2),
    /// Do not sync to packets with a type 3 Constant Tone Extension (currently reserved for future use)
    SONATA_GAP_CTE_NO_SYNC_WITH_TYPE_3       = (1 << 3),
    /// Do not sync to packets without a Constant Tone Extension
    SONATA_GAP_CTE_NO_SYNC_WITHOUT_CTE       = (1 << 4),
}sonata_gapm_sync_cte_type;

/// Supported AoA AoD switching sampling rate
enum sonata_gapm_switch_sampling_rate
{
    /// 1 us switching supported for AoD transmission
    SONATA_GAP_AOD_TX_1_US          =  (1 << 0),
    /// 1 us sampling supported for AoD reception
    SONATA_GAP_AOD_RX_1_US          =  (1 << 1),
    /// 1 us switching and sampling supported for AoA reception
    SONATA_GAP_AOA_RX_1_US          =  (1 << 2),
};

/*
 * Type Definition
 ****************************************************************************************
 */
typedef void (*PF_GAPM_CMP_EVT_CB)(struct gapm_cmp_evt *param);

typedef struct sonata_gap_api_cb
{
    /// GAPM_CMP_EVT message callback
    PF_GAPM_CMP_EVT_CB pf_gapm_cmp_evt_cb;

    struct sonata_gap_api_cb * _next;
}sonata_gap_api_cb_t;


typedef void (*PF_GAP_RESET_EVT_CB)(void);

typedef struct sonata_gap_reset_cb
{
    /// GAPM_CMP_EVT message callback
    PF_GAP_RESET_EVT_CB pf_gap_reset_evt_cb;

    struct sonata_gap_reset_cb * _next;
}sonata_gap_reset_cb_t;


typedef struct sonata_gap_api_sec_cb
{

    struct sonata_gap_api_sec_cb * _next;
}sonata_gap_api_sec_cb_t;

/// Configuration for advertising on primary channel
/*@TRACE*/
struct sonata_gap_adv_prim_cfg
{
    /// Minimum advertising interval (in unit of 625us). Must be greater than 20ms
    uint32_t adv_intv_min;
    /// Maximum advertising interval (in unit of 625us). Must be greater than 20ms
    uint32_t adv_intv_max;
    /// Bit field indicating the channel mapping
    uint8_t chnl_map;
    /// Indicate on which PHY primary advertising has to be performed (@see enum sonata_gap_phy_type)
    /// Note that LE 2M PHY is not allowed and that legacy advertising only support LE 1M PHY
    uint8_t phy;
};

/// Configuration for advertising on secondary channel
/*@TRACE*/
struct sonata_gap_adv_second_cfg
{
    /// Maximum number of advertising events the controller can skip before sending the
    /// AUX_ADV_IND packets. 0 means that AUX_ADV_IND PDUs shall be sent prior each
    /// advertising events
    uint8_t max_skip;
    /// Indicate on which PHY secondary advertising has to be performed (@see enum sonata_gap_phy_type)
    uint8_t phy;
    /// Advertising SID
    uint8_t adv_sid;
};

/// Configuration for periodic advertising
/*@TRACE*/
struct sonata_gap_adv_period_cfg
{
    /// Minimum advertising interval (in unit of 1.25ms). Must be greater than 20ms
    uint16_t adv_intv_min;
    /// Maximum advertising interval (in unit of 1.25ms). Must be greater than 20ms
    uint16_t adv_intv_max;
    /// CTE count (number of CTEs to transmit in each periodic advertising interval, range 0x01 to 0x10)
    /// 0 to disable CTE transmission
    uint8_t cte_count;
    /// CTE type (0: AOA | 1: AOD-1us | 2: AOD-2us) (@see enum gap_cte_type)
    uint8_t cte_type;
    /// CTE length (in 8us unit)
    uint8_t cte_len;
    /// Length of switching pattern (number of antenna IDs in the pattern)
    uint8_t switching_pattern_len;
    /// Antenna IDs
    uint8_t antenna_id[__ARRAY_EMPTY];
};

//struct sonata_gap_directed_adv_create_param {
//    /// Advertising type (@see enum gapm_adv_type)
////    uint8_t type;
//    /// Discovery mode (@see enum sonata_gap_adv_disc_mode)
//    uint8_t disc_mode;
//    /// Bit field value provided advertising properties (@see enum sonata_gap_adv_prop for bit signification)
//    uint16_t prop;
//    /// Maximum power level at which the advertising packets have to be transmitted
//    /// (between -127 and 126 dBm)
//    int8_t max_tx_pwr;
//    /// Advertising filtering policy (@see enum adv_filter_policy)
//    uint8_t filter_pol;
//    /// Peer address configuration (only used in case of directed advertising)
//    bd_addr_t addr;
//    /// Address type of the device 0=public/1=private random
//    uint8_t addr_type;
//    /// Configuration for primary advertising
//    /// Minimum advertising interval (in unit of 625us). Must be greater than 20ms
//    uint32_t adv_intv_min;
//    /// Maximum advertising interval (in unit of 625us). Must be greater than 20ms
//    uint32_t adv_intv_max;
//    /// Bit field indicating the channel mapping
//    uint8_t chnl_map;
//    /// Indicate on which PHY primary advertising has to be performed (@see enum sonata_gap_phy_type)
//    /// Note that LE 2M PHY is not allowed and that legacy advertising only support LE 1M PHY
//    uint8_t phy;
//
//};

//struct sonata_gap_extended_adv_create_param {
//    /// Advertising type (@see enum sonata_gap_adv_type)
////    uint8_t type;
//    /// Discovery mode (@see enum sonata_gap_adv_disc_mode)
//    uint8_t disc_mode;
//    /// Bit field value provided advertising properties (@see enum sonata_gap_adv_prop for bit signification)
//    uint16_t prop;
//    /// Maximum power level at which the advertising packets have to be transmitted
//    /// (between -127 and 126 dBm)
//    int8_t max_tx_pwr;
//    /// Advertising filtering policy (@see enum adv_filter_policy)
//    uint8_t filter_pol;
//
//    uint32_t prim_adv_intv_min;
//    /// Maximum advertising interval (in unit of 625us). Must be greater than 20ms
//    uint32_t prim_adv_intv_max;
//    /// Bit field indicating the channel mapping
//    uint8_t prim_chnl_map;
//    /// Indicate on which PHY primary advertising has to be performed (@see enum sonata_gap_phy_type)
//    /// Note that LE 2M PHY is not allowed and that legacy advertising only support LE 1M PHY
//    uint8_t prim_phy;
//
//    /// Maximum number of advertising events the controller can skip before sending the
//    /// AUX_ADV_IND packets. 0 means that AUX_ADV_IND PDUs shall be sent prior each
//    /// advertising events
//    uint8_t second_max_skip;
//    /// Indicate on which PHY secondary advertising has to be performed (@see enum sonata_gap_phy_type)
//    uint8_t second_phy;
//    /// Advertising SID
//    uint8_t second_adv_sid;
//
//};

//struct sonata_gap_periodic_adv_create_param {
//    /// Advertising type (@see enum gapm_adv_type)
////    uint8_t type;
//    /// Discovery mode (@see enum sonata_gap_adv_disc_mode)
//    uint8_t disc_mode;
//    /// Bit field value provided advertising properties (@see enum sonata_gap_adv_prop for bit signification)
//    uint16_t prop;
//    /// Maximum power level at which the advertising packets have to be transmitted
//    /// (between -127 and 126 dBm)
//    int8_t max_tx_pwr;
//    /// Advertising filtering policy (@see enum adv_filter_policy)
//    uint8_t filter_pol;
//    /// Configuration for primary advertising
//    /// Minimum advertising interval (in unit of 625us). Must be greater than 20ms
//    uint32_t prim_adv_intv_min;
//    /// Maximum advertising interval (in unit of 625us). Must be greater than 20ms
//    uint32_t prim_adv_intv_max;
//    /// Bit field indicating the channel mapping
//    uint8_t prim_chnl_map;
//    /// Indicate on which PHY primary advertising has to be performed (@see enum sonata_gap_phy_type)
//    /// Note that LE 2M PHY is not allowed and that legacy advertising only support LE 1M PHY
//    uint8_t prim_phy;
//    /// Configuration for secondary advertising (valid only if advertising type is
//    /// SONATA_GAP_ADV_TYPE_EXTENDED or SONATA_GAP_ADV_TYPE_PERIODIC)
//    /// Maximum number of advertising events the controller can skip before sending the
//    /// AUX_ADV_IND packets. 0 means that AUX_ADV_IND PDUs shall be sent prior each
//    /// advertising events
//    uint8_t second_max_skip;
//    /// Indicate on which PHY secondary advertising has to be performed (@see enum sonata_gap_phy_type)
//    uint8_t second_phy;
//    /// Advertising SID
//    uint8_t second_adv_sid;
//    /// Configuration for periodic advertising (valid only if advertising type os
//    /// SONATA_GAP_ADV_TYPE_PERIODIC)
//    /// Minimum advertising interval (in unit of 1.25ms). Must be greater than 20ms
//    uint16_t period_adv_intv_min;
//    /// Maximum advertising interval (in unit of 1.25ms). Must be greater than 20ms
//    uint16_t period_adv_intv_max;
//    /// CTE count (number of CTEs to transmit in each periodic advertising interval, range 0x01 to 0x10)
//    /// 0 to disable CTE transmission
//    uint8_t period_cte_count;
//    /// CTE type (0: AOA | 1: AOD-1us | 2: AOD-2us) (@see enum gap_cte_type)
//    uint8_t period_cte_type;
//    /// CTE length (in 8us unit)
//    uint8_t period_cte_len;
//    /// Length of switching pattern (number of antenna IDs in the pattern)
//    uint8_t period_switching_pattern_len;
//    /// Antenna IDs
//    uint8_t period_antenna_id[__ARRAY_EMPTY];
//
//
//
//};
/// Advertising parameters for advertising creation
/*@TRACE*/
struct sonata_gap_adv_create_param
{
    /// Advertising type (@see enum sonata_gap_adv_type)
    uint8_t type;
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
    struct gap_bdaddr peer_addr;
    /// Configuration for primary advertising
    struct sonata_gap_adv_prim_cfg prim_cfg;
    /// Configuration for secondary advertising (valid only if advertising type is
    /// SONATA_GAP_ADV_TYPE_EXTENDED or SONATA_GAP_ADV_TYPE_PERIODIC)
    struct sonata_gap_adv_second_cfg second_cfg;
    /// Configuration for periodic advertising (valid only if advertising type os
    /// SONATA_GAP_ADV_TYPE_PERIODIC)
    struct sonata_gap_adv_period_cfg period_cfg;
};

/// Additional advertising parameters
/*@TRACE*/
struct sonata_gap_adv_param
{
    /// Advertising duration (in unit of 10ms). 0 means that advertising continues
    /// until the host disable it
    uint16_t duration;
    /// Maximum number of extended advertising events the controller shall attempt to send prior to
    /// terminating the extending advertising
    /// Valid only if extended advertising
    uint8_t max_adv_evt;
};

///// Scan Window operation parameters
///*@TRACE*/
//struct sonata_gap_scan_wd_op_param {
//    /// Scan interval
//    uint16_t scan_intv;
//    /// Scan window
//    uint16_t scan_wd;
//};

///// Scanning parameters
///*@TRACE*/
//struct sonata_gap_scan_param {
//    /// Type of scanning to be started (@see enum sonata_gap_scan_type)
//    uint8_t type;
//    /// Properties for the scan procedure (@see enum sonata_gap_scan_prop for bit signification)
//    uint8_t prop;
//    /// Duplicate packet filtering policy
//    uint8_t dup_filt_pol;
//    /// Reserved for future use
//    uint8_t rsvd;
//    /// Scan window opening parameters for LE 1M PHY
//    struct sonata_gap_scan_wd_op_param scan_param_1m;
//    /// Scan window opening parameters for LE Coded PHY
//    struct sonata_gap_scan_wd_op_param scan_param_coded;
//    /// Scan duration (in unit of 10ms). 0 means that the controller will scan continuously until
//    /// reception of a stop command from the application
//    uint16_t duration;
//    /// Scan period (in unit of 1.28s). Time interval betweem two consequent starts of a scan duration
//    /// by the controller. 0 means that the scan procedure is not periodic
//    uint16_t period;
//};

///// Connection parameters
///*@TRACE*/
//struct sonata_gap_conn_param {
//    /// Minimum value for the connection interval (in unit of 1.25ms). Shall be less than or equal to
//    /// conn_intv_max value. Allowed range is 7.5ms to 4s.
//    uint16_t conn_intv_min;
//    /// Maximum value for the connection interval (in unit of 1.25ms). Shall be greater than or equal to
//    /// conn_intv_min value. Allowed range is 7.5ms to 4s.
//    uint16_t conn_intv_max;
//    /// Slave latency. Number of events that can be missed by a connected slave device
//    uint16_t conn_latency;
//    /// Link supervision timeout (in unit of 10ms). Allowed range is 100ms to 32s
//    uint16_t supervision_to;
//    /// Recommended minimum duration of connection events (in unit of 625us)
//    uint16_t ce_len_min;
//    /// Recommended maximum duration of connection events (in unit of 625us)
//    uint16_t ce_len_max;
//};

///// Initiating parameters
///*@TRACE*/
//struct sonata_gap_init_param {
//    /// Initiating type (@see enum sonata_gap_init_type)
//    uint8_t type;
//    /// Properties for the initiating procedure (@see enum sonata_gap_init_prop for bit signification)
//    uint8_t prop;
//    /// Timeout for automatic connection establishment (in unit of 10ms). Cancel the procedure if not all
//    /// indicated devices have been connected when the timeout occurs. 0 means there is no timeout
//    uint16_t conn_to;
//    /// Scan window opening parameters for LE 1M PHY
//    struct sonata_gap_scan_wd_op_param scan_param_1m;
//    /// Scan window opening parameters for LE Coded PHY
//    struct sonata_gap_scan_wd_op_param scan_param_coded;
//    /// Connection parameters for LE 1M PHY
//    struct sonata_gap_conn_param conn_param_1m;
//    /// Connection parameters for LE 2M PHY
//    struct sonata_gap_conn_param conn_param_2m;
//    /// Connection parameters for LE Coded PHY
//    struct sonata_gap_conn_param conn_param_coded;
//    /// Address of peer device in case white list is not used for connection
//    struct gap_bdaddr peer_addr;
//};

///// Periodic advertising information
///*@TRACE*/
//struct sonata_gap_period_adv_addr_cfg {
//    /// Advertiser address information
//    struct gap_bdaddr addr;
//    /// Advertising SID
//    uint8_t adv_sid;
//};
//
///// Periodic synchronization parameters
///*@TRACE*/
//struct sonata_gap_per_sync_param {
//    /// Number of periodic advertising that can be skipped after a successful receive. Maximum authorized
//    /// value is 499
//    uint16_t skip;
//    /// Synchronization timeout for the periodic advertising (in unit of 10ms between 100ms and 163.84s)
//    uint16_t sync_to;
//    /// Periodic synchronization type (@see enum sonata_gap_per_sync_type)
//    uint8_t type;
//    /// Connection index used for periodic sync info reception (only valid for SONATA_GAP_PER_SYNC_TYPE_PAST)
//    uint8_t conidx;
//    /// Address of advertiser with which synchronization has to be established (used only if use_pal is false)
//    struct sonata_gap_period_adv_addr_cfg adv_addr;
//    /// 1 to disable periodic advertising report, 0 to enable them by default
//    uint8_t report_disable;
//    /// Type of Constant Tone Extension device should sync on (@see enum sonata_gap_sync_cte_type).
//    uint8_t cte_type;
//};

/// Operation command structure in order to keep requested operation.
//struct sonata_gap_operation_cmd {
//    /// GAP request type
//    uint8_t operation;
//};

/// Command complete event data structure
/*@TRACE*/
struct sonata_gap_cmp_evt
{
    /// GAP requested operation
    uint8_t operation;
    /// Status of the request
    uint8_t status;
    /// extra parameter
    uint32_t param;
};

/// Command complete event data structure
/*@TRACE*/
typedef struct sonata_complete_evt
{
    /// GAP requested operation
    uint8_t operation;
    /// Status of the request
    uint8_t status;
}sonata_complete_evt_t;

/////  Reset link layer and the host command
///*@TRACE*/
//struct sonata_gap_reset_cmd {
//    /// GAPM requested operation:
//    /// - SONATA_GAP_RESET: Reset BLE subsystem: LL and HL.
//    uint8_t operation;
//};

/// Set device configuration command
/*@TRACE*/
//struct sonata_gap_set_dev_config_cmd {
//    /// GAPM requested operation:
//    ///  - SONATA_GAP_SET_DEV_CONFIG: Set device configuration
//    //uint8_t operation;
//    /// Device Role: Central, Peripheral, Observer, Broadcaster or All roles.
//    uint8_t role;
//
//    /// -------------- Privacy Config -----------------------
//    /// Duration before regenerate device address when privacy is enabled. - in seconds
//    uint16_t renew_dur;
//    /// Provided own static private random address
//    bd_addr_t addr;
//    /// Device IRK used for resolvable random BD address generation (LSB first)
//    struct gap_sec_key irk;
//    /// Privacy configuration bit field (@see enum sonata_gap_priv_cfg for bit signification)
//    uint8_t privacy_cfg;
//
//    /// -------------- Security Config -----------------------
//
//    /// Pairing mode authorized (@see enum sonata_gap_pairing_mode)
//    uint8_t pairing_mode;
//
//    /// -------------- ATT Database Config -----------------------
//
////    /// GAP service start handle
//    uint16_t gap_start_hdl;
////    /// GATT service start handle
//    uint16_t gatt_start_hdl;
//
//    /// Attribute database configuration (@see enum sonata_gap_att_cfg_flag)
//    uint16_t att_cfg;
//
//    /// -------------- LE Data Length Extension -----------------------
//    ///Suggested value for the Controller's maximum transmitted number of payload octets to be used
////    uint16_t sugg_max_tx_octets;
//    ///Suggested value for the Controller's maximum packet transmission time to be used
////    uint16_t sugg_max_tx_time;
//
//    /// --------------- L2CAP Configuration ---------------------------
//    /// Maximal MTU acceptable for device
//    uint16_t max_mtu;
//    /// Maximal MPS Packet size acceptable for device
//    uint16_t max_mps;
//    /// Maximum number of LE Credit based connection that can be established
//    uint8_t max_nb_lecb;
//
//    /// --------------- LE Audio Mode Supported -----------------------
//    ///
//    /// LE Audio Mode Configuration (@see sonata_gap_audio_cfg_flag)
////    uint16_t audio_cfg;
//
//    /// ------------------ LE PHY Management  -------------------------
//    /// Preferred LE PHY for data transmission (@see enum gap_phy)
////    uint8_t tx_pref_phy;
////    /// Preferred LE PHY for data reception (@see enum gap_phy)
////    uint8_t rx_pref_phy;
//
//    /// ------------------ Miscellaneous 2 ----------------------------
//    /// RF TX Path Compensation value (from -128dB to 128dB, unit is 0.1dB)
////    uint16_t tx_path_comp;
////    /// RF RX Path Compensation value (from -128dB to 128dB, unit is 0.1dB)
////    uint16_t rx_path_comp;
//};

/// Set new IRK
/*@TRACE*/
//struct sonata_gap_set_irk_cmd {
//    /// GAPM requested operation:
//    ///  - SONATA_GAP_SET_IRK: Set device configuration
//    uint8_t operation;
//    /// Device IRK used for resolvable random BD address generation (LSB first)
//    struct gap_sec_key irk;
//};

/// Set device channel map
/*@TRACE*/
//struct sonata_gap_set_channel_map_cmd {
//    /// GAPM requested operation:
//    ///  - SONATA_GAP_SET_CHANNEL_MAP: Set device channel map.
//    ///  - SONATA_GAP_GET_SUGGESTED_DFLT_LE_DATA_LEN: Get Suggested Default LE Data Length
//    ///  - SONATA_GAP_GET_MAX_LE_DATA_LEN: Get Maximum LE Data Length
//    ///  - SONATA_GAP_GET_NB_ADV_SETS: Read number of advertising sets currently supported by the controller
//    ///  - SONATA_GAP_GET_MAX_LE_ADV_DATA_LEN: Get maximum data length for advertising data
//
//    ///
//    ///  - SONATA_GAP_GET_ANTENNA_INFO:
////    uint8_t operation;
//    /// Channel map
//    //le_chnl_map_t chmap;
//    ///5-byte channel map array
//    uint8_t map[GAP_LE_CHNL_MAP_LEN];
//};

/// Get local device info command
/*@TRACE*/
//struct sonata_gap_get_dev_info_cmd {
//    /// GAPM requested operation:
//    ///  - SONATA_GAP_GET_DEV_VERSION: Get Local device version
//    ///  - SONATA_GAP_GET_DEV_BDADDR: Get Local device BD Address
//    ///  - SONATA_GAP_GET_DEV_ADV_TX_POWER: Get device advertising power level
//    ///  - SONATA_GAP_DBG_GET_MEM_INFO: Get memory usage (debug only)
//    uint8_t operation;
//};

///// Local device version indication event
///*@TRACE*/
//struct sonata_gap_dev_version_ind {
//    /// HCI version
//    uint8_t hci_ver;
//    /// LMP version
//    uint8_t lmp_ver;
//    /// Host version
//    uint8_t host_ver;
//    /// HCI revision
//    uint16_t hci_subver;
//    /// LMP subversion
//    uint16_t lmp_subver;
//    /// Host revision
//    uint16_t host_subver;
//    /// Manufacturer name
//    uint16_t manuf_name;
//};

///// Local device BD Address indication event
///*@TRACE*/
//struct sonata_gap_dev_bdaddr_ind {
//    /// Local device address information
//    struct gap_bdaddr addr;
//    /// Activity index
//    uint8_t actv_idx;
//};

///// Advertising channel Tx power level indication event
///*@TRACE*/
//struct sonata_gap_dev_adv_tx_power_ind {
//    /// Advertising channel Tx power level
//    int8_t power_lvl;
//};


/// Indication containing controller antenna information
/*@TRACE*/
//struct sonata_gap_antenna_inf_ind
//{
//    /// Supported switching sampling rates bit field (@see enum sonata_gap_switch_sampling_rate)
//    uint8_t supp_switching_sampl_rates;
//    /// Number of antennae
//    uint8_t antennae_num;
//    /// Max length of switching pattern (number of antenna IDs in the pattern)
//    uint8_t max_switching_pattern_len;
//    /// Max CTE length
//    uint8_t max_cte_len;
//};

/// Resolving Address indication event
/*@TRACE*/
struct sonata_gap_ral_addr_ind
{
    /// Peer or local read operation
    uint8_t operation;
    /// Resolving List address
    struct gap_bdaddr addr;
};

/// Resolve Address command
/*@TRACE*/
struct sonata_gap_resolv_addr_cmd
{
    /// GAPM requested operation:
    ///  - SONATA_GAP_RESOLV_ADDR: Resolve device address
//    uint8_t operation;
    /// Number of provided IRK (sahlle be > 0)
    uint8_t nb_key;
    /// Resolvable random address to solve
    bd_addr_t addr;
    /// Array of IRK used for address resolution (MSB -> LSB)
    struct gap_sec_key irk[__ARRAY_EMPTY];
};

/// Indicate that resolvable random address has been solved
/*@TRACE*/
struct sonata_gap_addr_solved_ind
{
    /// Resolvable random address solved
    bd_addr_t addr;
    /// IRK that correctly solved the random address
    struct gap_sec_key irk;
};

/// Name of peer device indication
/*@TRACE*/
struct sonata_gap_peer_name_ind
{
    /// peer device bd address
    bd_addr_t addr;
    /// peer device address type
    uint8_t addr_type;
    /// peer device name length
    uint8_t name_len;
    /// peer device name
    uint8_t name[__ARRAY_EMPTY];
};

/// Generate a random address.
/*@TRACE*/
struct sonata_gap_gen_rand_addr_cmd
{
    /// GAPM requested operation:
    ///  - SONATA_GAP_GEN_RAND_ADDR: Generate a random address
//    uint8_t operation;
    /// Dummy parameter used to store the prand part of the address
    uint8_t prand[GAP_ADDR_PRAND_LEN];
    /// Random address type @see gap_rnd_addr_type
    ///  - BD_ADDR_STATIC: Static random address
    ///  - BD_ADDR_NON_RSLV: Private non resolvable address
    ///  - BD_ADDR_RSLV: Private resolvable address
    uint8_t rnd_type;
};

/// Parameters of the @ref SONATA_GAP_USE_ENC_BLOCK_CMD message
/*@TRACE*/
struct sonata_gap_use_enc_block_cmd
{
    /// Command Operation Code (shall be SONATA_GAP_USE_ENC_BLOCK)
    uint8_t operation;
    /// Operand 1
    uint8_t operand_1[GAP_KEY_LEN];
    /// Operand 2
    uint8_t operand_2[GAP_KEY_LEN];
};

/// Parameters of the @ref SONATA_GAP_USE_ENC_BLOCK_IND message
/*@TRACE*/
struct sonata_gap_use_enc_block_ind
{
    /// Result (16 bytes)
    uint8_t result[GAP_KEY_LEN];
};

/// Parameters of the @ref SONATA_GAP_GEN_DH_KEY_CMD message
/*@TRACE*/
struct sonata_gap_gen_dh_key_cmd
{
    /// Command Operation Code (shall be SONATA_GAP_GEN_DH_KEY)
//    uint8_t operation;
    /// X coordinate
    uint8_t operand_1[GAP_P256_KEY_LEN];
    /// Y coordinate
    uint8_t operand_2[GAP_P256_KEY_LEN];
};

/// Parameters of the @ref SONATA_GAP_GEN_DH_KEY_IND message
/*@TRACE*/
struct sonata_gap_gen_dh_key_ind
{
    /// Result (32 bytes)
    uint8_t result[GAP_P256_KEY_LEN];
};

/// Parameters of the @ref SONATA_GAP_GET_PUB_KEY_CMD message
/*@TRACE*/
//struct sonata_gap_get_pub_key_cmd {
//    /// Command Operation Code (shall be SONATA_GAP_GET_PUB_KEY)
//    uint8_t operation;
//};

/// Parameters of the @ref SONATA_GAP_PUB_KEY_IND message
/*@TRACE*/
struct sonata_gap_pub_key_ind
{
    /// X coordinate
    uint8_t pub_key_x[GAP_P256_KEY_LEN];
    /// Y coordinate
    uint8_t pub_key_y[GAP_P256_KEY_LEN];
};

/// Parameters of the @ref SONATA_GAP_GEN_RAND_NB_CMD message
/*@TRACE*/
//struct sonata_gap_gen_rand_nb_cmd {
//    /// Command Operation Code (shall be SONATA_GAP_GEN_RAND_NB)
//    uint8_t operation;
//};

/// Parameters of the @ref SONATA_GAP_GEN_RAND_NB_IND message
/*@TRACE*/
struct sonata_gap_gen_rand_nb_ind
{
    /// Generation Random Number (8 bytes)
    rand_nb_t randnb;
};



/// Inform that profile task has been added.
/*@TRACE*/
struct sonata_gap_profile_added_ind
{
    /// Profile task identifier
    uint16_t prf_task_id;
    /// Profile task number allocated
    uint16_t prf_task_nb;
    /// Service start handle
    /// Only applies for services - Ignored by collectors
    uint16_t start_hdl;
};

/// Indicate that a message has been received on an unknown task
/*@TRACE*/
struct sonata_gap_unknown_task_ind
{
    /// Message identifier
    uint16_t msg_id;
    /// Task identifier
    uint16_t task_id;
};

///// Indicates suggested default data length
///*@TRACE*/
//struct sonata_gap_sugg_dflt_data_len_ind
//{
//    ///Host's suggested value for the Controller's maximum transmitted number of payload octets
//    uint16_t suggted_max_tx_octets;
//    ///Host's suggested value for the Controller's maximum packet transmission time
//    uint16_t suggted_max_tx_time;
//};

///// Indicates maximum data length
///*@TRACE*/
//struct sonata_gap_max_data_len_ind
//{
//    ///Maximum number of payload octets that the local Controller supports for transmission
//    uint16_t suppted_max_tx_octets;
//    ///Maximum time, in microseconds, that the local Controller supports for transmission
//    uint16_t suppted_max_tx_time;
//    ///Maximum number of payload octets that the local Controller supports for reception
//    uint16_t suppted_max_rx_octets;
//    ///Maximum time, in microseconds, that the local Controller supports for reception
//    uint16_t suppted_max_rx_time;
//};

/// Register a LE Protocol/Service Multiplexer command
/*@TRACE*/
//struct sonata_gap_lepsm_register_cmd {
//    /// GAPM requested operation:
//    ///  - SONATA_GAP_LEPSM_REG: Register a LE Protocol/Service Multiplexer
////    uint8_t operation;
//    /// LE Protocol/Service Multiplexer
//    uint16_t le_psm;
//    /// Application task number
//    uint16_t app_task;
//    /// Security level
//    ///   7   6   5   4   3   2   1   0
//    /// +---+---+---+---+---+---+---+---+
//    /// |MI |      RFU      |EKS|SEC_LVL|
//    /// +---+---+---+---+---+---+---+---+
//    /// bit[0-1]: Security level requirement (0=NO_AUTH, 1=UNAUTH, 2=AUTH, 3=SEC_CON)
//    /// bit[2]  : Encryption Key Size length must have 16 bytes
//    /// bit[7]  : Does the application task is multi-instantiated or not
//    uint8_t sec_lvl;
//};

/// Unregister a LE Protocol/Service Multiplexer command
/*@TRACE*/
//struct sonata_gap_lepsm_unregister_cmd {
//    /// GAPM requested operation:
//    ///  - SONATA_GAP_LEPSM_UNREG: Unregister a LE Protocol/Service Multiplexer
//    uint8_t operation;
//    /// LE Protocol/Service Multiplexer
//    uint16_t le_psm;
//};

/// Control LE Test Mode command
struct sonata_gap_le_test_mode_ctrl_cmd
{
    /// GAPM requested operation:
    ///  - SONATA_GAP_LE_TEST_STOP: Unregister a LE Protocol/Service Multiplexer
    ///  - SONATA_GAP_LE_TEST_RX_START: Start RX Test Mode
    ///  - SONATA_GAP_LE_TEST_TX_START: Start TX Test Mode
    uint8_t operation;
    /// Tx or Rx Channel (Range 0x00 to 0x27)
    uint8_t channel;
    /// Length in bytes of payload data in each packet (only valid for TX mode, range 0x00-0xFF)
    uint8_t tx_data_length;
    /// Packet Payload type (only valid for TX mode @see enum gap_pkt_pld_type)
    uint8_t tx_pkt_payload;
    /// Test PHY rate (@see enum gap_phy_val)
    uint8_t phy;
    /// Modulation Index (only valid for RX mode @see enum gap_modulation_idx)
    uint8_t modulation_idx;
    /// CTE length (in 8us unit) (Expected for RX Mode)
    uint8_t cte_len;
    /// CTE type (0: AOA | 1: AOD-1us | 2: AOD-2us) (Expected for TX Mode)
    uint8_t cte_type;
    /// Slot durations (only valid for RX mode)
    uint8_t slot_dur;
    /// Length of switching pattern (number of antenna IDs in the pattern)
    uint8_t switching_pattern_len;
    /// Antenna IDs
    uint8_t antenna_id[__ARRAY_EMPTY];
};

/// Indicate end of test mode event
struct sonata_gap_le_test_end_ind
{
    /// Number of received packets
    uint16_t nb_packet_received;
};

/// Indicate reception of a IQ Report event over a direct test mode.
/*@TRACE*/
struct sonata_gap_le_test_iq_report_ind
{
    /// Data channel index
    uint8_t channel_idx;
    /// RSSI (in 0.1 dBm)
    int16_t rssi;
    /// RSSI antenna ID
    uint8_t rssi_antenna_id;
    /// CTE type (0: AOA | 1: AOD-1us | 2: AOD-2us) (@see enum gap_cte_type)
    uint8_t cte_type;
    /// Slot durations (1: 1us | 2: 2us)
    uint8_t slot_dur;
    /// Packet status
    uint8_t pkt_status;
    /// Periodic Adv Event Counter
    uint16_t pa_evt_cnt;
    /// Number of samples
    uint8_t nb_samples;
    /// I/Q sample
    struct gap_iq_sample sample[__ARRAY_EMPTY];
};

///// Provide statistic information about ISO exchange
//struct sonata_gap_iso_stat_ind
//{
//    /// ISO Handle of the isochronous channel (Range 0x0000-0x0EFF)
//    uint16_t iso_hdl;
//
//    /// Statistics - Number of transmission attempts
//    uint32_t nb_tx;
//    /// Statistics - Number of transmission attempts that succeed
//    uint32_t nb_tx_ok;
//    /// Statistics - Number of Not granted packet packets
//    uint32_t nb_tx_not_granted;
//
//    /// Statistics - Number of reception attempt
//    uint32_t nb_rx;
//    /// Statistics - Number of reception attempts that succeed
//    uint32_t nb_rx_ok;
//    /// Statistics - Number of Not granted packet packets
//    uint32_t nb_rx_not_granted;
//    /// Statistics - Number of wrongly received packet (invalid data)
//    uint32_t nb_rx_data_err;
//    /// Statistics - Number of CRC Errors
//    uint32_t nb_rx_crc_err;
//    /// Statistics - Number of SYNC Errors
//    uint32_t nb_rx_sync_err;
//    /// Statistics - Number of received empty packets
//    uint32_t nb_rx_empty;
//};

/// Create an advertising, a scanning, an initiating, a periodic synchonization activity command (common)
/*@TRACE*/
//struct sonata_gap_activity_create_cmd {
//    /// GAPM request operation:
//    ///  - SONATA_GAP_CREATE_ADV_ACTIVITY: Create advertising activity
//    ///  - SONATA_GAP_CREATE_SCAN_ACTIVITY: Create scanning activity
//    ///  - SONATA_GAP_CREATE_INIT_ACTIVITY: Create initiating activity
//    ///  - SONATA_GAP_CREATE_PERIOD_SYNC_ACTIVITY: Create periodic synchronization activity
//    uint8_t operation;
//    /// Own address type (@see enum sonata_gap_own_addr)
//    uint8_t own_addr_type;
//};

/// Create an advertising activity command
struct sonata_gap_activity_create_adv_cmd
{
    /// GAPM request operation:
    ///  - SONATA_GAP_CREATE_ADV_ACTIVITY: Create advertising activity
    uint8_t operation;
    /// Own address type (@see enum sonata_gap_own_addr)
    uint8_t own_addr_type;
    /// Advertising parameters (optional, shall be present only if operation is SONATA_GAP_CREATE_ADV_ACTIVITY)
    /// For prop parameter, @see enum sonata_gap_leg_adv_prop, @see enum sonata_gap_ext_adv_prop and @see enum sonata_gap_per_adv_prop for help
    struct sonata_gap_adv_create_param adv_param;
};

/// Activity parameters
/*@TRACE
 @trc_ref sonata_gap_actv_type
 */
union sonata_gap_u_param
{
    /// Additional advertising parameters (for advertising activity)
    //@trc_union @activity_map[$parent.actv_idx] == SONATA_GAP_ACTV_TYPE_ADV
    struct sonata_gap_adv_param adv_add_param;
    /// Scan parameters (for scanning activity)
    //@trc_union @activity_map[$parent.actv_idx] == SONATA_GAP_ACTV_TYPE_SCAN
    struct sonata_gap_scan_param scan_param;
    /// Initiating parameters (for initiating activity)
    //@trc_union @activity_map[$parent.actv_idx] == SONATA_GAP_ACTV_TYPE_INIT
    struct sonata_gap_init_param init_param;
    /// Periodic synchronization parameters (for periodic synchronization activity)
    //@trc_union @activity_map[$parent.actv_idx] == SONATA_GAP_ACTV_TYPE_PER_SYNC
    struct sonata_gap_per_sync_param per_sync_param;
};

/// Start a given activity command
/*@TRACE*/
struct sonata_gap_activity_start_cmd
{
    /// GAPM request operation:
    ///  - SONATA_GAP_START_ACTIVITY: Start a given activity
    uint8_t operation;
    /// Activity identifier
    uint8_t actv_idx;
    /// Activity parameters
    union sonata_gap_u_param u_param;
};

/// Stop one or all activity(ies) command
/*@TRACE*/
//struct sonata_gap_activity_stop_cmd {
//    /// GAPM request operation:
//    ///  - SONATA_GAP_STOP_ACTIVITY: Stop a given activity
//    ///  - SONATA_GAP_STOP_ALL_ACTIVITIES: Stop all existing activities
//    uint8_t operation;
//    /// Activity identifier - used only if operation is SONATA_GAP_STOP_ACTIVITY
//    uint8_t actv_idx;
//};

/// Delete one or all activity(ies) command
/*@TRACE*/
//struct sonata_gap_activity_delete_cmd {
//    /// GAPM request operation:
//    ///  - SONATA_GAP_DELETE_ACTIVITY: Delete a given activity
//    ///  - SONATA_GAP_DELETE_ALL_ACTIVITIES: Delete all existing activities
//    uint8_t operation;
//    /// Activity identifier - used only if operation is SONATA_GAP_STOP_ACTIVITY
//    uint8_t actv_idx;
//};

/// Indicate creation of an activity
/*@TRACE
 @trc_exec activity_map[$actv_idx] = $actv_type
 activity_map = {}*/
struct sonata_gap_activity_created_ind
{
    /// Activity identifier
    uint8_t actv_idx;
    /// Activity type (@see enum sonata_gap_actv_type)
    uint8_t actv_type;
    /// Selected TX power for advertising activity
    int8_t tx_pwr;
};

/// Indicate that an activity has been stopped
/*@TRACE*/
struct sonata_gap_activity_stopped_ind
{
    /// Activity identifier
    uint8_t actv_idx;
    /// Activity type (@see enum sonata_gap_actv_type)
    uint8_t actv_type;
    /// Activity stop reason
    uint8_t reason;
    /// In case of periodic advertising, indicate if periodic advertising has been stopped
    uint8_t per_adv_stop;
};

/// Set advertising, scan response or periodic advertising data command
/*@TRACE*/
//struct sonata_gap_set_adv_data_cmd {
//    /// GAPM request operation:
//    ///  - SONATA_GAP_SET_ADV_DATA: Set advertising data
//    ///  - SONATA_GAP_SET_SCAN_RSP_DATA: Set scan response data
//    ///  - SONATA_GAP_SET_PERIOD_ADV_DATA: Set periodic advertising data
//    uint8_t operation;
//    /// Activity identifier
////    uint8_t actv_idx;
//    /// Data length
//    uint16_t length;
//    /// Data
//    uint8_t data[__ARRAY_EMPTY];
//};

/// Indicate reception of scan request
/*@TRACE*/
struct sonata_gap_scan_request_ind
{
    /// Activity identifier
    uint8_t actv_idx;
    /// Transmitter device address
    struct gap_bdaddr trans_addr;
};

/// Indicate reception of advertising, scan response or periodic advertising data
/*@TRACE*/
//struct sonata_gap_ext_adv_report_ind {
//    /// Activity identifier
//    uint8_t actv_idx;
//    /// Bit field providing information about the received report (@see enum sonata_gap_adv_report_info)
//    uint8_t info;
//    /// Transmitter device address
//    struct gap_bdaddr trans_addr;
//    /// Target address (in case of a directed advertising report)
//    struct gap_bdaddr target_addr;
//    /// TX power (in dBm)
//    int8_t tx_pwr;
//    /// RSSI (between -127 and +20 dBm)
//    int8_t rssi;
//    /// Primary PHY on which advertising report has been received
//    uint8_t phy_prim;
//    /// Secondary PHY on which advertising report has been received
//    uint8_t phy_second;
//    /// Advertising SID
//    /// Valid only for periodic advertising report
//    uint8_t adv_sid;
//    /// Periodic advertising interval (in unit of 1.25ms, min is 7.5ms)
//    /// Valid only for periodic advertising report
//    uint16_t period_adv_intv;
//    /// Report length
//    uint16_t length;
//    /// Report
//    uint8_t data[__ARRAY_EMPTY];
//};

/// Indicate that synchronization has been established with a periodic advertiser
/*@TRACE*/
struct sonata_gap_sync_established_ind
{
    /// Activity identifier
    uint8_t actv_idx;
    /// PHY on which synchronization has been established (@see gap_phy_type)
    uint8_t phy;
    /// Periodic advertising interval (in unit of 1.25ms, min is 7.5ms)
    uint16_t intv;
    /// Advertising SID
    uint8_t adv_sid;
    /// Advertiser clock accuracy (@see enum sonata_gap_clk_acc)
    uint8_t clk_acc;
    /// Advertiser address
    struct gap_bdaddr addr;
    /// Only valid for a periodic sync transfer sync, else ignore
    uint16_t serv_data;
};

/// Read local or peer address
/*@TRACE*/
//struct sonata_gap_get_ral_addr_cmd {
//    /// GAPM request operation:
//    ///  - SONATA_GAP_GET_RAL_LOC_ADDR: Get resolving local address
//    ///  - SONATA_GAP_GET_RAL_PEER_ADDR: Get resolving peer address
//    uint8_t operation;
//    /// Peer device identity
//    struct gap_bdaddr peer_identity;
//};

/// Set content of either white list or resolving list or periodic advertiser list command (common part)
//struct sonata_gap_list_set_cmd {
//    /// GAPM request operation:
//    ///  - SONATA_GAP_SET_WHITE_LIST: Set white list content
//    ///  - SONATA_GAP_SET_RAL: Set resolving list content
//    ///  - SONATA_GAP_SET_PAL: Set periodic advertiser list content
//    uint8_t operation;
//    /// Number of entries to be added in the list. 0 means that list content has to be cleared
//    uint8_t size;
//};

/// Set content of white list
/*@TRACE*/
struct sonata_gap_list_set_wl_cmd
{
    /// GAPM request operation:
    ///  - SONATA_GAP_SET_WHITE_LIST: Set white list content
    uint8_t operation;
    /// Number of entries to be added in the list. 0 means that list content has to be cleared
    uint8_t size;
    /// List of entries to be added in the list
    struct gap_bdaddr wl_info[__ARRAY_EMPTY];
};

/// Set content of resolving list command
/*@TRACE*/
struct sonata_gap_list_set_ral_cmd
{
    /// GAPM request operation:
    ///  - SONATA_GAP_SET_RAL: Set resolving list content
    uint8_t operation;
    /// Number of entries to be added in the list. 0 means that list content has to be cleared
    uint8_t size;
    /// List of entries to be added in the list
    struct gap_ral_dev_info ral_info[__ARRAY_EMPTY];
};

/// Set content of periodic advertiser list command
/*@TRACE*/
struct sonata_gap_list_set_pal_cmd
{
    /// GAPM request operation:
    ///  - SONATA_GAP_SET_PAL: Set periodic advertiser list content
    uint8_t operation;
    /// Number of entries to be added in the list. 0 means that list content has to be cleared
    uint8_t size;
    /// List of entries to be added in the list
    struct sonata_gap_period_adv_addr_cfg pal_info[__ARRAY_EMPTY];
};

/*@TRACE
 @trc_ref sonata_gap_operation
 sonata_gap_list_set_cmd = sonata_gap_u_list_set
*/
union sonata_gap_u_list_set
{
    uint8_t operation;

    //@trc_union operation == SONATA_GAP_SET_WL
    struct sonata_gap_list_set_wl_cmd list_set_wl_cmd;
    //@trc_union operation == SONATA_GAP_SET_RAL
    struct sonata_gap_list_set_ral_cmd list_set_ral_cmd;
    //@trc_union operation == SONATA_GAP_SET_PAL
    struct sonata_gap_list_set_pal_cmd list_set_pal_cmd;
};

/// List Size indication event
///*@TRACE*/
//struct sonata_gap_list_size_ind
//{
//    /// Operation code, indicate list for which size has been read
//    ///     - SONATA_GAP_SET_WHITE_LIST
//    ///     - SONATA_GAP_SET_RAL
//    ///     - SONATA_GAP_SET_PAL
//    uint8_t operation;
//    /// List size
//    uint8_t size;
//};

///// Maximum advertising data length indication event
///*@TRACE*/
//struct sonata_gap_max_adv_data_len_ind
//{
//    /// Maximum advertising data length supported by controller
//    uint16_t length;
//};

///// Number of available advertising sets indication event
///*@TRACE*/
//struct sonata_gap_nb_adv_sets_ind
//{
//    /// Number of available advertising sets
//    uint8_t nb_adv_sets;
//};

///// Indicate the transmit powers supported by the controller
///*@TRACE*/
//struct sonata_gap_dev_tx_pwr_ind
//{
//    /// Minimum TX power
//    int8_t min_tx_pwr;
//    /// Maximum TX power
//    int8_t max_tx_pwr;
//};

///// Indicate the RF path compensation values
///*@TRACE*/
//struct sonata_gap_dev_rf_path_comp_ind
//{
//    /// RF TX path compensation
//    uint16_t tx_path_comp;
//    /// RF RX path compensation
//    uint16_t rx_path_comp;
//};

/// Indicate that an unknown message has been received
/*@TRACE*/
struct sonata_gap_unknown_msg_ind
{
    /// Unknown message id
    ke_msg_id_t unknown_msg_id;
};

/// Control reception or not of Periodic Advertising Report in a Periodic Advertising Sync activity
/*@TRACE*/
//struct sonata_gap_per_adv_report_ctrl_cmd {
//    /// GAPM request operation:
//    ///  - SONATA_GAP_PER_ADV_REPORT_CTRL: Enable / Disable reception of periodic advertising report
//    uint8_t operation;
//    /// Activity identifier
//    uint8_t actv_idx;
//    /// True to enable reception of periodic advertising report, false to disable
//    uint8_t enable;
//};

/// Control capturing IQ samples from the Constant Tone Extension of periodic advertising packets
/*@TRACE*/
struct sonata_gap_per_sync_iq_sampling_ctrl_cmd
{
    /// GAPM request operation:
    ///  - SONATA_GAP_PER_SYNC_IQ_SAMPLING_CTRL: Enable / Disable IQ sampling
//    uint8_t operation;
    /// Activity identifier
    uint8_t actv_idx;
    /// True to enable IQ sampling, false to disable
    uint8_t enable;
    /// Slot durations (1: 1us | 2: 2us)
    uint8_t slot_dur;
    /// Max sampled CTEs
    uint8_t max_sampl_cte;
    /// Length of switching pattern
    uint8_t switching_pattern_len;
    /// Antenna IDs
    uint8_t antenna_id[__ARRAY_EMPTY];
};

/// Indicate reception of a IQ Report event over a periodic advertising sync activity
/*@TRACE*/
//struct sonata_gap_per_adv_iq_report_ind
//{
//    /// Activity identifier
//    uint8_t actv_idx;
//    /// Data channel index
//    uint8_t channel_idx;
//    /// RSSI (in 0.1 dBm)
//    int16_t rssi;
//    /// RSSI antenna ID
//    uint8_t rssi_antenna_id;
//    /// CTE type (0: AOA | 1: AOD-1us | 2: AOD-2us) (@see enum gap_cte_type)
//    uint8_t cte_type;
//    /// Slot durations (1: 1us | 2: 2us)
//    uint8_t slot_dur;
//    /// Packet status
//    uint8_t pkt_status;
//    /// Periodic Adv Event Counter
//    uint16_t pa_evt_cnt;
//    /// Number of samples
//    uint8_t nb_samples;
//    /// I/Q sample
//    struct gap_iq_sample sample[__ARRAY_EMPTY];
//};

/// Control CTE transmission in a periodic advertising activity
/*@TRACE*/
//struct sonata_gap_per_adv_cte_tx_ctl_cmd {
//    /// GAPM request operation:
//    ///  - SONATA_GAP_PER_ADV_CTE_TX_CTL: Control CTE transmission in a periodic advertising activity
//    uint8_t operation;
//    /// Activity identifier
//    uint8_t actv_idx;
//    /// True to enable CTE transmission, false to disable
//    uint8_t enable;
//};

/// Request to renew all currently used random private addresses (non-resolvable or resolvable)
/// For internal use only
/*@TRACE*/
//struct sonata_gap_addr_renew_cmd {
//    /// GAPM request operation:
//    ///  - SONATA_GAP_RENEW_ADDR: Renew random private addresses
//    uint8_t operation;
//    /// Activity index, used by GAPM ADDR state machine in order to remind for which activity
//    /// a new address has been generated
//    uint8_t actv_idx;
//    /// Index of first created initiating/scanning for which address has been renewed,
//    /// any initiating/scanning met after shall use the same address if it uses a random
//    /// address
//    uint8_t init_scan_actv_idx;
//};

/// Debug I/Q sample control
/*@TRACE*/
struct sonata_gap_dbg_iq_ctrl
{
    /// I-sample control
    /// 0: up-count from 0x01
    /// 1: down-count from 0xFF
    /// 2: Fixed value 0x00
    /// 3: PRBS pattern
    /// 4..0xFF: Fixed value
    uint8_t i;
    /// Q-sample control
    /// 0: up-count from 0x01
    /// 1: down-count from 0xFF
    /// 2: Fixed value 0x00
    /// 3: PRBS pattern
    /// 4..0xFF: Fixed value
    uint8_t q;
};
/// Configure the Debug Platform I&Q Sampling generator
/*@TRACE*/
struct sonata_gap_dbg_iqgen_cfg_cmd
{

    /// GAPM request operation:
    ///  - SONATA_GAP_DBG_IQGEN_CFG: Configure the Debug Platform I&Q Sampling generator
//    uint8_t operation;
    /// Number of antenna patterns
    uint8_t nb_antenna;
    /// Antenna switch/sample control
    /// bit[0]: 0: up-sweep; 1: up-down sweep (internal switching mode)
    /// bit[1]: 0: 1us intervals; 1: 2us intervals (internal switching mode)
    /// bit[2]: 0: internal switching mode; 1: baseband switching mode
    uint8_t mode;
    /// I/Q sample control
    struct sonata_gap_dbg_iq_ctrl iq_ctrl[__ARRAY_EMPTY];
};


//////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////                  GAPC   /////////////////////////////////////////////

enum sonata_gap_msg_id
{
    /* Default event */
    /// Command Complete event
    SONATA_GAPC_CMP_EVT                                        = MSG_ID(GAPC, 0x00),

    /* Connection state information */
    /// Indicate that a connection has been established
    SONATA_GAP_CONNECTION_REQ_IND                             = MSG_ID(GAPC, 0x01),
    /// Set specific link data configuration.
    SONATA_GAP_CONNECTION_CFM                                 = MSG_ID(GAPC, 0x02),

    /// Indicate that a link has been disconnected
    SONATA_GAP_DISCONNECT_IND                                 = MSG_ID(GAPC, 0x03),

    /* Link management command */
    /// Request disconnection of current link command.
    SONATA_GAP_DISCONNECT_CMD                                 = MSG_ID(GAPC, 0x04),

    /* Peer device info */
    /// Retrieve information command
    SONATA_GAP_GET_INFO_CMD                                   = MSG_ID(GAPC, 0x05),
    /// Peer device attribute DB info such as Device Name, Appearance or Slave Preferred Parameters
    SONATA_GAP_PEER_ATT_INFO_IND                              = MSG_ID(GAPC, 0x06),
    /// Indication of peer version info
    SONATA_GAP_PEER_VERSION_IND                               = MSG_ID(GAPC, 0x07),
    /// Indication of peer features info
    SONATA_GAP_PEER_FEATURES_IND                              = MSG_ID(GAPC, 0x08),
    /// Indication of ongoing connection RSSI
    SONATA_GAP_CON_RSSI_IND                                   = MSG_ID(GAPC, 0x09),

    /* Device Name Management */
    /// Peer device request local device info such as name, appearance or slave preferred parameters
    SONATA_GAP_GET_DEV_INFO_REQ_IND                           = MSG_ID(GAPC, 0x0A),
    /// Send requested info to peer device
    SONATA_GAP_GET_DEV_INFO_CFM                               = MSG_ID(GAPC, 0x0B),
    /// Peer device request to modify local device info such as name or appearance
    SONATA_GAP_SET_DEV_INFO_REQ_IND                           = MSG_ID(GAPC, 0x0C),
    /// Local device accept or reject device info modification
    SONATA_GAP_SET_DEV_INFO_CFM                               = MSG_ID(GAPC, 0x0D),

    /* Connection parameters update */
    /// Perform update of connection parameters command
    SONATA_GAP_PARAM_UPDATE_CMD                               = MSG_ID(GAPC, 0x0E),
    /// Request of updating connection parameters indication
    SONATA_GAP_PARAM_UPDATE_REQ_IND                           = MSG_ID(GAPC, 0x0F),
    /// Master confirm or not that parameters proposed by slave are accepted or not
    SONATA_GAP_PARAM_UPDATE_CFM                               = MSG_ID(GAPC, 0x10),
    /// Connection parameters updated indication
    SONATA_GAP_PARAM_UPDATED_IND                              = MSG_ID(GAPC, 0x11),

    /* Bonding procedure */
    /// Start Bonding command procedure
    SONATA_GAP_BOND_CMD                                       = MSG_ID(GAPC, 0x12),
    /// Bonding requested by peer device indication message.
    SONATA_GAP_BOND_REQ_IND                                   = MSG_ID(GAPC, 0x13),
    /// Confirm requested bond information.
    SONATA_GAP_BOND_CFM                                       = MSG_ID(GAPC, 0x14),
    /// Bonding information indication message
    SONATA_GAP_BOND_IND                                       = MSG_ID(GAPC, 0x15),

    /* Encryption procedure */
    /// Start Encryption command procedure
    SONATA_GAP_ENCRYPT_CMD                                    = MSG_ID(GAPC, 0x16),
    /// Encryption requested by peer device indication message.
    SONATA_GAP_ENCRYPT_REQ_IND                                = MSG_ID(GAPC, 0x17),
    /// Confirm requested Encryption information.
    SONATA_GAP_ENCRYPT_CFM                                    = MSG_ID(GAPC, 0x18),
    /// Encryption information indication message
    SONATA_GAP_ENCRYPT_IND                                    = MSG_ID(GAPC, 0x19),

    /* Security request procedure */
    /// Start Security Request command procedure
    SONATA_GAP_SECURITY_CMD                                   = MSG_ID(GAPC, 0x1A),
    /// Security requested by peer device indication message
    SONATA_GAP_SECURITY_IND                                   = MSG_ID(GAPC, 0x1B),

    /* Signature procedure */
    /// Indicate the current sign counters to the application
    SONATA_GAP_SIGN_COUNTER_IND                               = MSG_ID(GAPC, 0x1C),

    /* Device information */
    /// Indication of ongoing connection Channel Map
    SONATA_GAP_CON_CHANNEL_MAP_IND                            = MSG_ID(GAPC, 0x1D),

    /* LE Ping */
    /// Update LE Ping timeout value
    SONATA_GAP_SET_LE_PING_TO_CMD                             = MSG_ID(GAPC, 0x28),
    /// LE Ping timeout indication
    SONATA_GAP_LE_PING_TO_VAL_IND                             = MSG_ID(GAPC, 0x29),
    /// LE Ping timeout expires indication
    SONATA_GAP_LE_PING_TO_IND                                 = MSG_ID(GAPC, 0x2A),

    /* LE Data Length extension*/
    /// LE Set Data Length Command
    SONATA_GAP_SET_LE_PKT_SIZE_CMD                            = MSG_ID(GAPC, 0x2B),
    /// LE Set Data Length Indication
    SONATA_GAP_LE_PKT_SIZE_IND                                = MSG_ID(GAPC, 0x2C),

    /* Secure Connections */
    /// Request to inform the remote device when keys have been entered or erased
    SONATA_GAP_KEY_PRESS_NOTIFICATION_CMD                     = MSG_ID(GAPC, 0x2D),
    /// Indication that a KeyPress has been performed on the peer device.
    SONATA_GAP_KEY_PRESS_NOTIFICATION_IND                     = MSG_ID(GAPC, 0x2E),

    /* PHY Management */
    /// Set the PHY configuration for current active link
    SONATA_GAP_SET_PHY_CMD                                    = MSG_ID(GAPC, 0x2F),
    /// Active link PHY configuration. Triggered when configuration is read or during an update.
    SONATA_GAP_LE_PHY_IND                                     = MSG_ID(GAPC, 0x30),

    /* Channel Selection Algorithm */
    /// Indication of currently used channel selection algorithm
    /// @see struct gapc_chan_sel_algo_ind
    SONATA_GAP_CHAN_SEL_ALGO_IND                              = MSG_ID(GAPC, 0x31),

    /* Preferred Slave Latency */
    /// Set the preferred slave latency (for slave only, with RW controller)
    SONATA_GAP_SET_PREF_SLAVE_LATENCY_CMD                     = MSG_ID(GAPC, 0x32),
    /// Set the preferred slave event duration (for slave only, with RW controller)
    SONATA_GAP_SET_PREF_SLAVE_EVT_DUR_CMD                     = MSG_ID(GAPC, 0x33),

    /// Indication to the task that sends the unknown message
    SONATA_GAPC_UNKNOWN_MSG_IND                                = MSG_ID(GAPC, 0x34),

    /* Periodic Sync Transfer */
    /// Transfer periodic advertising sync information to peer device
    SONATA_GAP_PER_ADV_SYNC_TRANS_CMD                         = MSG_ID(GAPC, 0x35),

    /* Constant Tone Extension */
    /// Constant Tone Extension Transmission configuration command
    SONATA_GAP_CTE_TX_CFG_CMD                                 = MSG_ID(GAPC, 0x38),
    /// Constant Tone Extension Reception configuration command
    SONATA_GAP_CTE_RX_CFG_CMD                                 = MSG_ID(GAPC, 0x39),
    /// Constant Tone Extension request control command (enable / disable)
    SONATA_GAP_CTE_REQ_CTRL_CMD                               = MSG_ID(GAPC, 0x3A),
    /// Constant Tone Extension Response control command (enable / disable)
    SONATA_GAP_CTE_RSP_CTRL_CMD                               = MSG_ID(GAPC, 0x3B),
    /// Indicate reception of a IQ Report event over a ble connection
    SONATA_GAP_CTE_IQ_REPORT_IND                              = MSG_ID(GAPC, 0x3C),

    // ---------------------- INTERNAL API ------------------------
    /* Internal messages for timer events, not part of API*/
    /// Signature procedure
    SONATA_GAP_SIGN_CMD                                       = MSG_ID(GAPC, 0xF0),
    /// Signature result
    SONATA_GAP_SIGN_IND                                       = MSG_ID(GAPC, 0xF1),

    /// Parameter update procedure timeout indication
    SONATA_GAP_PARAM_UPDATE_TO_IND                            = MSG_ID(GAPC, 0xF2),
    /// Pairing procedure timeout indication
    SONATA_GAP_SMP_TIMEOUT_TIMER_IND                          = MSG_ID(GAPC, 0xF3),
    /// Pairing repeated attempts procedure timeout indication
    SONATA_GAP_SMP_REP_ATTEMPTS_TIMER_IND                     = MSG_ID(GAPC, 0xF4),
};



enum sonata_gapc_operation
{
    /*                 Operation Flags                  */
    /* No Operation (if nothing has been requested)     */
    /* ************************************************ */
    /// No operation
//    SONATA_GAP_NO_OP                                    = 0x00,

    /* Connection management */
    /// Disconnect link
    SONATA_GAP_DISCONNECT                               = 0x01,

    /* Connection information */
    /// Retrieve name of peer device.
    SONATA_GAP_GET_PEER_NAME                            = 0x02,
    /// Retrieve peer device version info.
    SONATA_GAP_GET_PEER_VERSION                         = 0x03,
    /// Retrieve peer device features.
    SONATA_GAP_GET_PEER_FEATURES                        = 0x04,
    /// Get Peer device appearance
    SONATA_GAP_GET_PEER_APPEARANCE                      = 0x05,
    /// Get Peer device Slaved Preferred Parameters
    SONATA_GAP_GET_PEER_SLV_PREF_PARAMS                 = 0x06,
    /// Retrieve connection RSSI.
    SONATA_GAP_GET_CON_RSSI                             = 0x07,
    /// Retrieve Connection Channel MAP.
    SONATA_GAP_GET_CON_CHANNEL_MAP                      = 0x08,

    /* Connection parameters update */
    /// Perform update of connection parameters.
    SONATA_GAP_UPDATE_PARAMS                            = 0x09,
//
//    /* Security procedures */
//    /// Start bonding procedure.
    SONATA_GAP_BOND                                     = 0x0A,
//    /// Start encryption procedure.
    SONATA_GAP_ENCRYPT                                  = 0x0B,
//    /// Start security request procedure
    SONATA_GAP_SECURITY_REQ                             = 0x0C,

    /* LE Ping*/
    /// get timer timeout value
    SONATA_GAP_GET_LE_PING_TO                           = 0x12,
    /// set timer timeout value
    SONATA_GAP_SET_LE_PING_TO                           = 0x13,

    /* LE Data Length extension*/
    /// LE Set Data Length
    SONATA_GAP_SET_LE_PKT_SIZE                          = 0x14,

    /* Central Address resolution supported*/
    SONATA_GAP_GET_ADDR_RESOL_SUPP                      = 0x15,

    /* Secure Connections */
    /// Request to inform the remote device when keys have been entered or erased
    SONATA_GAP_KEY_PRESS_NOTIFICATION                   = 0x16,

    /* PHY Management */
    /// Set the PHY configuration for current active link
    SONATA_GAP_SET_PHY                                  = 0x17,
    /// Retrieve PHY configuration of active link
    SONATA_GAP_GET_PHY                                  = 0x18,

    /* Channel Selection Algorithm */
    /// Retrieve Channel Selection Algorithm
    SONATA_GAP_GET_CHAN_SEL_ALGO                        = 0x19,

    /* Preferred slave latency */
    /// Set the preferred slave latency (for slave only, with RW controller)
    SONATA_GAP_SET_PREF_SLAVE_LATENCY                   = 0x1A,
    /// Set the preferred slave event duration (for slave only, with RW controller)
    SONATA_GAP_SET_PREF_SLAVE_EVT_DUR                   = 0x1B,

    /* Periodic Sync Transfer */
    /// Transfer periodic advertising sync information to peer device
    SONATA_GAP_PER_ADV_SYNC_TRANS                       = 0x1C,

    /* Constant Tone Extension */
    /// Constant Tone Extension Transmission configuration
    SONATA_GAP_CTE_TX_CFG                                = 0x20,
    /// Constant Tone Extension Reception configuration
    SONATA_GAP_CTE_RX_CFG                                = 0x21,
    /// Constant Tone Extension request control (enable / disable)
    SONATA_GAP_CTE_REQ_CTRL                              = 0x22,
    /// Constant Tone Extension Response control (enable / disable)
    SONATA_GAP_CTE_RSP_CTRL                              = 0x23,

    // ---------------------- INTERNAL API ------------------------
    /* Packet signature */
    /// sign an attribute packet
    SONATA_GAP_SIGN_PACKET                              = 0xF0,
    /// Verify signature or an attribute packet
    SONATA_GAP_SIGN_CHECK                               = 0xF1,
};


///// Bond event type.
///*@TRACE*/
//enum sonata_gap_bond
//{
//    /// Bond Pairing request
//    SONATA_GAP_PAIRING_REQ,
//    /// Respond to Pairing request
//    SONATA_GAP_PAIRING_RSP,
//
//    /// Pairing Finished information
//    SONATA_GAP_PAIRING_SUCCEED,
//    /// Pairing Failed information
//    SONATA_GAP_PAIRING_FAILED,
//
//    /// Used to retrieve pairing Temporary Key
//    SONATA_GAP_TK_EXCH,
//    /// Used for Identity Resolving Key exchange
//    SONATA_GAP_IRK_EXCH,
//    /// Used for Connection Signature Resolving Key exchange
//    SONATA_GAP_CSRK_EXCH,
//    /// Used for Long Term Key exchange
//    SONATA_GAP_LTK_EXCH,
//
//    /// Bond Pairing request issue, Repeated attempt
//    SONATA_GAP_REPEATED_ATTEMPT,
//
//    /// Out of Band - exchange of confirm and rand.
//    SONATA_GAP_OOB_EXCH,
//
//    /// Numeric Comparison - Exchange of Numeric Value -
//    SONATA_GAP_NC_EXCH
//};

///// List of device info that should be provided by application
///*@TRACE*/
//enum sonata_gap_dev_info
//{
//    /// Device Name
//    SONATA_GAP_DEV_NAME,
//    /// Device Appearance Icon
//    SONATA_GAP_DEV_APPEARANCE,
//    /// Device Slave preferred parameters
//    SONATA_GAP_DEV_SLV_PREF_PARAMS,
//    /// Device Central address resolution
//    SONATA_GAP_DEV_CTL_ADDR_RESOL,
//    /// maximum device info parameter
//    SONATA_GAP_DEV_INFO_MAX,
//};

/// List of features available on a device
enum sonata_gap_features_list
{
    /// LE encryption
    SONATA_GAP_ENCRYPT_FEAT_MASK              = (1 << 0),
    /// Connection Parameters Request Procedure
    SONATA_GAP_CONN_PARAM_REQ_FEAT_MASK       = (1 << 1),
    /// Extended Reject Indication
    SONATA_GAP_EXT_REJECT_IND_FEAT_MASK       = (1 << 2),
    /// Slave-initiated Features Exchange
    SONATA_GAP_SLAVE_FEAT_EXCH_FEAT_MASK      = (1 << 3),
    /// LE ping
    SONATA_GAP_LE_PING_FEAT_MASK              = (1 << 4)
};

/// Option for PHY configuration
enum sonata_gap_phy_option
{
    /// No preference for rate used when transmitting on the LE Coded PHY
    SONATA_GAP_PHY_OPT_LE_CODED_ALL_RATES     = (1 << 0),
    /// 500kbps rate preferred when transmitting on the LE Coded PHY
    SONATA_GAP_PHY_OPT_LE_CODED_500K_RATE     = (1 << 1),
    /// 125kbps  when transmitting on the LE Coded PHY
    SONATA_GAP_PHY_OPT_LE_CODED_125K_RATE     = (1 << 2),
};



/// Client bond information
enum sonata_gap_cli_info
{
    /// Service changed indication enabled
    SONATA_GAP_SVC_CHANGED_IND_EN_BIT = (1 << 0),
    SONATA_GAP_SVC_CHANGED_IND_EN_POS = 0,
    /// Database updated since last connection
    SONATA_GAP_DB_UPDATED_BIT         = (1 << 1),
    SONATA_GAP_DB_UPDATED_POS         = 1,
};

/// Client supported features
enum sonata_gap_cli_feat
{
    /// Robust Cache feature enabled
    SONATA_GAP_ROBUST_CACHE_EN_BIT    = (1 << 0),
    SONATA_GAP_ROBUST_CACHE_EN_POS    = 0,
};
/*
 * TYPE DEFINITIONS
 ****************************************************************************************
 */

///*@TRACE*/
//enum sonata_gap_dev_info
//{
//    /// Device Name
//    SONATA_GAP_DEV_NAME,
//    /// Device Appearance Icon
//    SONATA_GAP_DEV_APPEARANCE,
//    /// Device Slave preferred parameters
//    SONATA_GAP_DEV_SLV_PREF_PARAMS,
//    /// Device Central address resolution
//    SONATA_GAP_DEV_CTL_ADDR_RESOL,
//    /// maximum device info parameter
//    SONATA_GAP_DEV_INFO_MAX,
//};
///// Indicate that a connection has been established
///*@TRACE*/
//struct sonata_gap_connection_req_ind
//{
//    /// Connection handle
//    uint16_t conhdl;
//    /// Connection interval
//    uint16_t con_interval;
//    /// Connection latency
//    uint16_t con_latency;
//    /// Link supervision timeout
//    uint16_t sup_to;
//    /// Clock accuracy
//    uint8_t clk_accuracy;
//    /// Peer address type
//    uint8_t peer_addr_type;
//    /// Peer BT address
//    bd_addr_t peer_addr;
//    /// Role of device in connection (0 = Master / 1 = Slave)
//    uint8_t role;
//
//};


///// Set specific link data configuration.
///*@TRACE*/
//struct sonata_gap_connection_cfm
//{
//    /// Local CSRK value
//    struct gap_sec_key lcsrk;
//    /// Local signature counter value
//    uint32_t           lsign_counter;
//    /// Remote CSRK value
//    struct gap_sec_key rcsrk;
//    /// Remote signature counter value
//    uint32_t           rsign_counter;
//    /// Authentication (@see enum gap_auth)
//    uint8_t            auth;
//    /// Client bond data information (@see enum gapc_cli_info)
//    uint8_t            cli_info;
//    /// LTK exchanged during pairing.
//    bool               ltk_present;
//    /// Client supported features    (@see enum gapc_cli_feat)
//    uint8_t            cli_feat;
//    /// Peer GATT Service Start handle
//    uint16_t           gatt_start_handle;
//    /// Peer GATT Service End Handle
//    uint16_t           gatt_end_handle;
//    /// Peer Service Change value handle
//    uint16_t           svc_chg_handle;
//};

/// Request disconnection of current link command.
/*@TRACE*/
//struct sonata_gap_disconnect_cmd
//{
//    /// GAP request type:
//    /// - GAPC_DISCONNECT: Disconnect link.
////    uint8_t operation;
//
//    /// Reason of disconnection
//    uint8_t reason;
//};


/// Indicate that a link has been disconnected
/*@TRACE*/
struct sonata_gap_disconnect_ind
{
    /// Connection handle
    uint16_t conhdl;
    /// Reason of disconnection
    uint8_t reason;
};


/// Retrieve information command
/*@TRACE*/
//struct sonata_gap_get_info_cmd
//{
//    /// GAP request type:
//    /// - GAPC_GET_PEER_NAME: Retrieve name of peer device.
//    /// - GAPC_GET_PEER_VERSION: Retrieve peer device version info.
//    /// - GAPC_GET_PEER_FEATURES: Retrieve peer device features.
//    /// - GAPC_GET_CON_RSSI: Retrieve connection RSSI.
//    /// - GAPC_GET_CON_CHANNEL_MAP: Retrieve Connection Channel MAP.
//    /// - GAPC_GET_PEER_APPEARANCE: Get Peer device appearance
//    /// - GAPC_GET_PEER_SLV_PREF_PARAMS: Get Peer device Slaved Preferred Parameters
//    /// - GAPC_GET_ADDR_RESOL_SUPP: Address Resolution Supported
//    /// - GAPC_GET_LE_PING_TIMEOUT: Retrieve LE Ping Timeout Value
//    uint8_t operation;
//};

///// device information data
///*@TRACE
// @trc_ref gapc_dev_info*/
//union sonata_gapc_dev_info_val
//{
//    /// Device name
//    //@trc_union parent.req == GAPC_DEV_NAME
//    struct gap_dev_name name;
//    /// Appearance Icon
//    //@trc_union parent.req == GAPC_DEV_APPEARANCE
//    uint16_t appearance;
//    /// Slave preferred parameters
//    //@trc_union parent.req == GAPC_DEV_SLV_PREF_PARAMS
//    struct gap_slv_pref slv_pref_params;
//    /// Central address resolution
//    //@trc_union parent.req == GAPC_DEV_CTL_ADDR_RESOL
//    uint8_t ctl_addr_resol;
//};

///// Peer device attribute DB info such as Device Name, Appearance or Slave Preferred Parameters
///*@TRACE*/
//struct sonata_gap_peer_att_info_ind
//{
//    /// Requested information
//    /// - GAPC_DEV_NAME: Device Name
//    /// - GAPC_DEV_APPEARANCE: Device Appearance Icon
//    /// - GAPC_DEV_SLV_PREF_PARAMS: Device Slave preferred parameters
//    /// - GAPC_GET_ADDR_RESOL_SUPP: Address resolution supported
//    uint8_t  req;
//    /// Attribute handle
//    uint16_t handle;
//
//    /// device information data
//    union sonata_gapc_dev_info_val info;
//};



/// Sign counter value changed due to packet signing or signature verification.
struct sonata_gap_sign_counter_updated_ind
{
    /// New Local signature counter value
    uint32_t lsign_counter;
    /// New Remote signature counter value
    uint32_t rsign_counter;
};



/// Peer device request local device info such as name, appearance or slave preferred parameters
/*@TRACE*/
struct sonata_gap_get_dev_info_req_ind
{
    /// Requested information
    /// - GAPC_DEV_NAME: Device Name
    /// - GAPC_DEV_APPEARANCE: Device Appearance Icon
    /// - GAPC_DEV_SLV_PREF_PARAMS: Device Slave preferred parameters
    uint8_t req;
};



/// Send requested info to peer device
/*@TRACE*/
struct sonata_gap_get_dev_info_cfm
{
    /// Requested information
    /// - GAPC_DEV_NAME: Device Name
    /// - GAPC_DEV_APPEARANCE: Device Appearance Icon
    /// - GAPC_DEV_SLV_PREF_PARAMS: Device Slave preferred parameters
    uint8_t req;

    /// Peer device information data
    union sonata_gap_dev_info_val info;
};

/// device information data
/*@TRACE
 @trc_ref gapc_dev_info*/
union sonata_gapc_set_dev_info
{
    /// Device name
    //@trc_union parent.req == GAPC_DEV_NAME
    struct gap_dev_name name;
    /// Appearance Icon
    //@trc_union parent.req == GAPC_DEV_APPEARANCE
    uint16_t appearance;
};

/// Peer device request to modify local device info such as name or appearance
/*@TRACE*/
struct sonata_gap_set_dev_info_req_ind
{
    /// Requested information
    /// - GAPC_DEV_NAME: Device Name
    /// - GAPC_DEV_APPEARANCE: Device Appearance Icon
    uint8_t req;

    /// device information data
    union sonata_gapc_set_dev_info info;
};

/// Local device accept or reject device info modification
/*@TRACE*/
//struct sonata_gap_set_dev_info_cfm
//{
//    /// Requested information
//    /// - GAPC_DEV_NAME: Device Name
//    /// - GAPC_DEV_APPEARANCE: Device Appearance Icon
//    uint8_t req;
//
//    /// Status code used to know if requested has been accepted or not
//    uint8_t status;
//};

/// Connection Parameter used to update connection parameters
struct sonata_gapc_conn_param    //Todo same name with gapm
{
    /// Connection interval minimum
    uint16_t intv_min;
    /// Connection interval maximum
    uint16_t intv_max;
    /// Latency
    uint16_t latency;
    /// Supervision timeout
    uint16_t time_out;
};

/// Perform update of connection parameters command
/*@TRACE*/
struct sonata_gap_param_update_cmd
{
    /// GAP request type:
    /// - GAPC_UPDATE_PARAMS: Perform update of connection parameters.
//    uint8_t operation;
    /// Internal parameter used to manage internally l2cap packet identifier for signaling
    uint8_t pkt_id;
    /// Connection interval minimum
    uint16_t intv_min;
    /// Connection interval maximum
    uint16_t intv_max;
    /// Latency
    uint16_t latency;
    /// Supervision timeout
    uint16_t time_out;
    /// Minimum Connection Event Duration
    uint16_t ce_len_min;
    /// Maximum Connection Event Duration
    uint16_t ce_len_max;
};

/// Request of updating connection parameters indication
/*@TRACE*/
struct sonata_gap_param_update_req_ind
{
    /// Connection interval minimum
    uint16_t intv_min;
    /// Connection interval maximum
    uint16_t intv_max;
    /// Latency
    uint16_t latency;
    /// Supervision timeout
    uint16_t time_out;
};

/// Connection parameters updated indication
/*@TRACE*/
struct sonata_gap_param_updated_ind
{
    ///Connection interval value
    uint16_t            con_interval;
    ///Connection latency value
    uint16_t            con_latency;
    ///Supervision timeout
    uint16_t            sup_to;
};

/// Master confirm or not that parameters proposed by slave are accepted or not
/*@TRACE*/
//struct sonata_gap_param_update_cfm
//{
//    /// True to accept slave connection parameters, False else.
//    bool accept;
//    /// Minimum Connection Event Duration
//    uint16_t ce_len_min;
//    /// Maximum Connection Event Duration
//    uint16_t ce_len_max;
//};

/// Parameters of the @ref GAPC_SET_PREF_SLAVE_LATENCY_CMD message
/*@TRACE*/
//struct sonata_gap_set_pref_slave_latency_cmd
//{
//    /// GAP request type:
//    /// - GAPC_SET_PREF_SLAVE_LATENCY_CMD : Set preferred slave latency
//    uint8_t operation;
//    /// Preferred latency that the controller should use on a connection (in number of connection events)
//    uint16_t latency;
//};

/// Parameters of the @ref GAPC_SET_PREF_SLAVE_EVT_DUR_CMD message
/*@TRACE*/
//struct sonata_gap_set_pref_slave_evt_dur_cmd
//{
//    /// GAP request type:
//    /// - GAPC_SET_PREF_SLAVE_EVT_DUR_CMD : Set preferred slave event duration
//    uint8_t operation;
//    /// Preferred event duration that the controller should use on a connection (N * 0.625 ms)
//    uint16_t duration;
//    /// Slave transmits a single packet per connection event (False/True)
//    uint8_t single_tx;
//};


/// Parameters of the @ref GAPC_PER_ADV_SYNC_TRANS_CMD message
/*@TRACE*/
struct sonata_gap_per_adv_sync_trans_cmd
{
    /// GAP request type:
    /// - GAPC_PER_ADV_SYNC_TRANS : Periodic Advertising Sync Transfer
    uint8_t  operation;
    /// Periodic Advertising or Periodic Sync activity index
    uint8_t  actv_idx;
    /// A value provided by application
    uint16_t service_data;
};

/// Parameters of the @ref GAPC_CTE_TX_CFG_CMD message
/*@TRACE*/
struct sonata_gap_cte_tx_cfg_cmd
{
    /// GAP request type:
    /// - GAPC_CTE_TX_CFG: Constant Tone Extension Transmission configuration
//    uint8_t  operation;
    /// CTE types (bit0: AOA | bit1: AOD-1us | bit2: AOD-2us) (@see enum gap_cte_type)
    uint8_t  cte_types;
    /// Length of switching pattern (number of antenna IDs in the pattern)
    uint8_t  switching_pattern_len;
    /// Antenna IDs
    uint8_t  antenna_id[__ARRAY_EMPTY];
};

/// Parameters of the @ref GAPC_CTE_RX_CFG_CMD message
/*@TRACE*/
struct sonata_gap_cte_rx_cfg_cmd
{
    /// GAP request type:
    /// - GAPC_CTE_RX_CFG: Constant Tone Extension Reception configuration
    uint8_t  operation;
    /// Sampling enable
    bool     sample_en;
    /// Slot durations (1: 1us | 2: 2us)
    uint8_t  slot_dur;
    /// Length of switching pattern (number of antenna IDs in the pattern)
    uint8_t  switching_pattern_len;
    /// Antenna IDs
    uint8_t  antenna_id[__ARRAY_EMPTY];
};

/// Parameters of the @ref GAPC_CTE_REQ_CTRL_CMD message
/*@TRACE*/
struct sonata_gap_cte_req_ctrl_cmd
{
    /// GAP request type:
    /// - GAPC_CTE_REQ_CTRL: Constant Tone Extension request control (enable / disable)
//    uint8_t  operation;
    /// True to enable TX or RX Constant Tone Extension, False to disable
    bool     enable;
    /// CTE request interval (in number of connection events)
    uint16_t interval;
    /// Requested CTE length (in 8us unit)
    uint8_t  cte_len;
    /// Requested CTE type (0: AOA | 1: AOD-1us | 2: AOD-2us)  (@see enum gap_cte_type)
    uint8_t  cte_type;
};

/// Parameters of the @ref GAPC_CTE_RSP_CTRL_CMD message
/*@TRACE*/
//struct sonata_gap_cte_rsp_ctrl_cmd
//{
//    /// GAP request type:
//    /// - GAPC_CTE_RSP_CTRL: Constant Tone Extension response control (enable / disable)
//    uint8_t  operation;
//    /// True to enable TX or RX Constant Tone Extension, False to disable
//    bool     enable;
//};

///// Indicate reception of a IQ Report event over a BLE connection
///*@TRACE*/
//struct sonata_gap_cte_iq_report_ind
//{
//    /// Rx PHY  (@see enum gap_phy_val)
//    uint8_t  rx_phy;
//    /// Data channel index
//    uint8_t  data_channel_idx;
//    /// RSSI (in 0.1 dBm)
//    int16_t  rssi;
//    /// RSSI antenna ID
//    uint8_t  rssi_antenna_id;
//    /// CTE type (0: AOA | 1: AOD-1us | 2: AOD-2us) (@see enum gap_cte_type)
//    uint8_t  cte_type;
//    /// Slot durations (1: 1us | 2: 2us)
//    uint8_t  slot_dur;
//    /// Packet status
//    uint8_t  pkt_status;
//    /// Connection event counter
//    uint16_t con_evt_cnt;
//    /// Number of samples
//    uint8_t  nb_samples;
//    /// I/Q sample
//    struct gap_iq_sample sample[__ARRAY_EMPTY];
//};

///// Pairing parameters
///*@TRACE*/
//struct sonata_gap_pairing
//{
//    /// IO capabilities (@see gap_io_cap)
//    uint8_t iocap;
//    /// OOB information (@see gap_oob)
//    uint8_t oob;
//    /// Authentication (@see gap_auth)
//    /// Note in BT 4.1 the Auth Field is extended to include 'Key Notification' and
//    /// and 'Secure Connections'.
//    uint8_t auth;
//    /// Encryption key size (7 to 16)
//    uint8_t key_size;
//    ///Initiator key distribution (@see gap_kdist)
//    uint8_t ikey_dist;
//    ///Responder key distribution (@see gap_kdist)
//    uint8_t rkey_dist;
//
//    /// Device security requirements (minimum security level). (@see gap_sec_req)
//    uint8_t sec_req;
//};

///// Long Term Key information
///*@TRACE*/
//struct sonata_gap_ltk
//{
//    /// Long Term Key
//    struct gap_sec_key ltk;
//    /// Encryption Diversifier
//    uint16_t ediv;
//    /// Random Number
//    rand_nb_t randnb;
//    /// Encryption key size (7 to 16)
//    uint8_t key_size;
//};
/// Out of Band Information
/*@TRACE*/
//struct sonata_gap_oob
//{
//    /// Confirm Value
//    uint8_t conf[GAP_KEY_LEN];
//    /// Random Number
//    uint8_t rand[GAP_KEY_LEN];
//};

/*@TRACE*/
//struct sonata_gap_nc
//{
//    uint8_t value[4];
//};

///// Identity Resolving Key information
///*@TRACE*/
//struct sonata_gap_irk
//{
//    /// Identity Resolving Key
//    struct gap_sec_key irk;
//    /// Device BD Identity Address
//    struct gap_bdaddr addr;
//};


/// Start Bonding command procedure
/*@TRACE*/
//struct sonata_gap_bond_cmd
//{
//    /// GAP request type:
//    /// - GAPC_BOND:  Start bonding procedure.
////    uint8_t operation;
//    /// Pairing information
//    struct sonata_gap_pairing pairing;
//};

///// Bond procedure requested information data
///*@TRACE
// @trc_ref gapc_bond*/
//union sonata_gapc_bond_req_data
//{
//    /// Authentication level (@see gap_auth) (if request = GAPC_PAIRING_REQ)
//    //@trc_union parent.request == GAPC_PAIRING_REQ
//    uint8_t auth_req;
//    /// LTK Key Size (if request = GAPC_LTK_EXCH)
//    //@trc_union parent.request == GAPC_LTK_EXCH
//    uint8_t key_size;
//    /// Device IO used to get TK: (if request = GAPC_TK_EXCH)
//    ///  - GAP_TK_OOB:       TK get from out of band method
//    ///  - GAP_TK_DISPLAY:   TK generated and shall be displayed by local device
//    ///  - GAP_TK_KEY_ENTRY: TK shall be entered by user using device keyboard
//    //@trc_union parent.request == GAPC_TK_EXCH
//    uint8_t tk_type;
//
//    /// Addition OOB Data for the OOB Conf and Rand values
//    //@trc_union parent.request == GAPC_OOB_EXCH
//    struct sonata_gap_oob  oob_data;
//    /// Numeric Comparison Data
//    //@trc_union parent.request == GAPC_NC_EXCH
//    struct sonata_gap_nc   nc_data;
//};

///// Bonding requested by peer device indication message.
///*@TRACE*/
//struct sonata_gap_bond_req_ind
//{
//    /// Bond request type (@see gapc_bond)
//    uint8_t request;
//
//    /// Bond procedure requested information data
//    union sonata_gapc_bond_req_data data;
//};

/*@TRACE
 @trc_ref gapc_bond
*/
//union sonata_gap_bond_cfm_data
//{
//    /// Pairing Features (request = GAPC_PAIRING_RSP)
//    //@trc_union parent.request == GAPC_PAIRING_RSP
//    struct sonata_gap_pairing pairing_feat;
//    /// LTK (request = GAPC_LTK_EXCH)
//    //@trc_union parent.request == GAPC_LTK_EXCH
//    struct sonata_gap_ltk ltk;
//    /// CSRK (request = GAPC_CSRK_EXCH)
//    //@trc_union parent.request == GAPC_CSRK_EXCH
//    struct gap_sec_key csrk;
//    /// TK (request = GAPC_TK_EXCH)
//    //@trc_union parent.request == GAPC_TK_EXCH
//    struct gap_sec_key tk;
//    /// IRK (request = GAPC_IRK_EXCH)
//    //@trc_union parent.request == GAPC_IRK_EXCH
//    struct sonata_gap_irk irk;
//    /// OOB Confirm and Random from the peer (request = GAPC_OOB_EXCH)
//    //@trc_union parent.request == GAPC_OOB_EXCH
//    struct sonata_gap_oob oob;
//};

/// Confirm requested bond information.
/*@TRACE*/
//struct sonata_gap_bond_cfm
//{
//    /// Bond request type (@see gapc_bond)
//    uint8_t request;
//    /// Request accepted
//    uint8_t accept;
//
//    /// Bond procedure information data
//    union sonata_gap_bond_cfm_data data;
//};

///**
// *  Authentication information
// */
///*@TRACE*/
//struct sonata_gap_bond_auth
//{
//    /// Authentication information (@see gap_auth)
//    uint8_t info;
//
//    /// LTK exchanged during pairing.
//    bool ltk_present;
//};

///// Bond procedure information data
///*@TRACE
// @trc_ref gapc_bond*/
//union sonata_gapc_bond_data
//{
//    /// Authentication information (@see gap_auth)
//    /// (if info = GAPC_PAIRING_SUCCEED)
//    //@trc_union parent.info == GAPC_PAIRING_SUCCEED
//    struct sonata_gap_bond_auth auth;
//    /// Pairing failed reason  (if info = GAPC_PAIRING_FAILED)
//    //@trc_union parent.info == GAPC_PAIRING_FAILED
//    uint8_t reason;
//    /// Long Term Key information (if info = GAPC_LTK_EXCH)
//    //@trc_union parent.info == GAPC_LTK_EXCH
//    struct sonata_gap_ltk ltk;
//    /// Identity Resolving Key information (if info = GAPC_IRK_EXCH)
//    //@trc_union parent.info == GAPC_IRK_EXCH
//    struct sonata_gap_irk irk;
//    /// Connection Signature Resolving Key information (if info = GAPC_CSRK_EXCH)
//    //@trc_union parent.info == GAPC_CSRK_EXCH
//    struct gap_sec_key csrk;
//};
//
///// Bonding information indication message
///*@TRACE*/
//struct sonata_gap_bond_ind
//{
//    /// Bond information type (@see gapc_bond)
//    uint8_t info;
//
//    /// Bond procedure information data
//    union sonata_gapc_bond_data data;
//};

/// Start Encryption command procedure
/*@TRACE*/
//struct sonata_gap_encrypt_cmd
//{
//    /// GAP request type:
//    /// - GAPC_ENCRYPT:  Start encryption procedure.
//    uint8_t operation;
//    /// Long Term Key information
//    struct sonata_gap_ltk ltk;
//};

/// Encryption requested by peer device indication message.
/*@TRACE*/
struct sonata_gap_encrypt_req_ind
{
    /// Encryption Diversifier
    uint16_t ediv;
    /// Random Number
    rand_nb_t rand_nb;
};

/// Confirm requested Encryption information.
/*@TRACE*/
struct sonata_gap_encrypt_cfm
{
    /// Indicate if a LTK has been found for the peer device
    uint8_t found;
    /// Long Term Key
    struct gap_sec_key ltk;
    /// LTK Key Size
    uint8_t key_size;
};

/// Encryption information indication message
/*@TRACE*/
struct sonata_gap_encrypt_ind
{
    /// Authentication  level (@see gap_auth)
    uint8_t auth;
};

/// Start Security Request command procedure
/*@TRACE*/
//struct sonata_gap_security_cmd
//{
//    /// GAP request type:
//    /// - GAPC_SECURITY_REQ: Start security request procedure
//    uint8_t operation;
//    /// Authentication level (@see gap_auth)
//    uint8_t auth;
//};
/// Security requested by peer device indication message
/*@TRACE*/
struct sonata_gap_security_ind
{
    /// Authentication level (@see gap_auth)
    uint8_t auth;
};

/// Parameters of the @ref GAPC_SIGN_COUNTER_IND message
/*@TRACE*/
struct sonata_gap_sign_counter_ind
{
    /// Local SignCounter value
    uint32_t local_sign_counter;
    /// Peer SignCounter value
    uint32_t peer_sign_counter;
};


/// Parameters of the @ref GAPC_SET_LE_PING_TO_CMD message
/*@TRACE*/
//struct sonata_gap_set_le_ping_to_cmd
//{
//    /// GAP request type:
//    /// - GAPC_SET_LE_PING_TO : Set the LE Ping timeout value
//    uint8_t operation;
//    /// Authenticated payload timeout
//    uint16_t timeout;
//};

/// Parameters of the @ref GAPC_SET_LE_PKT_SIZE_CMD message
/*@TRACE*/
struct sonata_gap_set_le_pkt_size_cmd
{
    /// GAP request type:
    /// - GAPC_SET_LE_PKT_SIZE : Set the LE Data length value
    uint8_t operation;
    ///Preferred maximum number of payload octets that the local Controller should include
    ///in a single Link Layer Data Channel PDU.
    uint16_t tx_octets;
    ///Preferred maximum number of microseconds that the local Controller should use to transmit
    ///a single Link Layer Data Channel PDU
    uint16_t tx_time;
};

/// Parameters of the @ref GAPC_LE_PKT_SIZE_IND message
/*@TRACE*/
struct sonata_gap_le_pkt_size_ind
{
    ///The maximum number of payload octets in TX
    uint16_t max_tx_octets;
    ///The maximum time that the local Controller will take to TX
    uint16_t max_tx_time;
    ///The maximum number of payload octets in RX
    uint16_t max_rx_octets;
    ///The maximum time that the local Controller will take to RX
    uint16_t max_rx_time;
};

/// Parameters of the @ref GAPC_KEY_PRESS_NOTIFICATION_CMD message
/*@TRACE*/
//struct sonata_gap_key_press_notif_cmd
//{
//    /// GAP request type:
//    /// - GAPC_KEY_PRESS_NOTIFICATION_CMD : Inform the remote device when keys have been entered or erased
//    uint8_t operation;
//    /// notification type
//    uint8_t notification_type;
//};

/// Parameters of the @ref GAPC_KEY_PRESS_NOTIFICATION_IND message
/*@TRACE*/
struct sonata_gap_key_press_notif_ind
{
    /// notification type
    uint8_t notification_type;
};

/// Set the PHY configuration for current active link
/*@TRACE*/
struct sonata_gap_set_phy_cmd
{
    /// GAP request type:
    /// - GAPC_SET_PHY : Set the PHY configuration for current active link
    uint8_t operation;
    /// Supported LE PHY for data transmission (@see enum gap_phy)
    uint8_t tx_phy;
    /// Supported LE PHY for data reception (@see enum gap_phy)
    uint8_t rx_phy;
    /// PHY options (@see enum gapc_phy_option)
    uint8_t phy_opt;
};


/// Parameters of the @ref GAPC_SIGN_CMD message
/*@TRACE*/
struct sonata_gap_sign_cmd
{
    /// GAP request type:
    /// - GAPC_SIGN_PACKET: Sign an attribute packet
    /// - GAPC_SIGN_CHECK:  Verify signature or an attribute packet
    uint8_t operation;
    /// Data PDU length (Bytes)
    uint16_t byte_len;
    /// Data PDU + SignCounter if generation, Data PDU + SignCounter + MAC if verification
    uint8_t msg[__ARRAY_EMPTY];
};

/// Parameters of the @ref GAPC_SIGN_IND message
/*@TRACE*/
struct sonata_gap_sign_ind
{
    /// GAP request type:
    /// - GAPC_SIGN_PACKET: Sign an attribute packet
    /// - GAPC_SIGN_CHECK:  Verify signature or an attribute packet
    uint8_t operation;
    /// Data PDU length (Bytes)
    uint16_t byte_len;
    /// Data PDU + SignCounter + MAC
    uint8_t signed_msg[__ARRAY_EMPTY];
};



//typedef struct sonata_activity_item
//{
//    uint8_t actv_idx;
//    sonata_gap_operation actv_create_type;//A0:Adv  A1:Scan  A2:Peer
//    uint8_t tx_pwr;
//    uint8_t running;
//    uint8_t peer[BD_ADDR_LEN];
//
//} sonata_activity_item_t;

typedef struct sonata_gap_conn_item
{
    uint8_t conidx;             //KE_IDX_GET(src_id)
    uint8_t clk_accuracy;        /// Clock accuracy
    uint8_t peer_addr_type;      /// Peer address type
    uint8_t role;                /// Role of device in connection (0 = Master / 1 = Slave)
    uint16_t conhdl;             /// Connection handle
    uint16_t con_interval;       /// Connection interval
    uint16_t con_latency;        /// Connection latency
    uint16_t sup_to;             /// Link supervision timeout
    bd_addr_t peer_addr;         /// Peer BT address
} sonata_gap_conn_item_t;
/*!
 * @brief
 */
typedef struct sonata_ble_entity
{
    uint8_t adv_actv_idx;
    uint8_t init_actv_idx;
    uint8_t scan_actv_idx;
    uint8_t per_sync_actv_idx;

    uint16_t seq_num;
    uint16_t gatt_current_id;
    struct sonata_gap_conn_item conns[APP_CONN_MAX];

//    uint8_t actv_idx_temp;
//    sonata_gap_operation actv_create_type_temp;
//    uint8_t adv_state;
//    struct sonata_activity_item activitys[APP_ACTIVITY_MAX];

} sonata_ble_entity_t;
/*
 * GLOBAL VARIABLES DECLARATIONS
 ****************************************************************************************
 */

/*!
 * @brief Callback define for ble complete message.
 */
typedef void (*APP_COMPLETE_CB)(uint16_t status);

/*
 * FUNCTION DECLARATIONS
 ****************************************************************************************
 */

/*!
 * @brief
 */
void sonata_gap_api_init(void);

/*!
 * @brief
 * @param cb
 */
void sonata_gap_api_cb_register(sonata_gap_api_cb_t *cb);


/*!
 * @brief Register ble reset evt callback
 * @param cb @see ble_reset_callback
 * @return API_SUCCESS
 */
uint16_t sonata_gap_register_reset_callback(sonata_gap_reset_cb_t *cb);


/***************************   GAPC ****************************************/

/*!
 * @brief
 * @return
 */
int sonata_gap_reset_cmd_handler();

/*!
 * @brief
 * @param param
 * @return
 */
int sonata_gap_bring_up_handler(sonata_gap_set_dev_config_cmd *param);
/*!
 * @brief
 * @param cmd
 * @return
 */
int sonata_gap_set_dev_config_cmd_handler(sonata_gap_set_dev_config_cmd *cmd);
/*!
 * @brief
 * @param own_addr_type
 * @param param
 * @return
 */
int sonata_gap_adv_activity_create_cmd_handler(uint8_t own_addr_type, struct sonata_gap_adv_create_param *param);
/*!
 * @brief
 * @param own_addr_type
 * @return
 */
int sonata_gap_init_activity_create_cmd_handler(uint8_t own_addr_type);
/*!
 * @brief
 * @param own_addr_type
 * @return
 */
int sonata_gap_scan_activity_create_cmd_handler(uint8_t own_addr_type);
/*!
 * @brief
 * @param own_addr_type
 * @return
 */
int sonata_gap_period_sync_activity_create_cmd_handler(uint8_t own_addr_type);
/*!
 * @brief
 * @param actv_idx
 * @param duration
 * @param max_adv_evt
 * @return
 */
int sonata_gap_adv_activity_start_cmd_handler(uint8_t actv_idx, uint16_t duration, uint8_t max_adv_evt);
/*!
 * @brief
 * @param actv_idx
 * @param param
 * @return
 */
int sonata_gap_init_activity_start_cmd_handler(uint8_t actv_idx, sonata_gap_init_param_t *param);
/*!
 * @brief
 * @param actv_idx
 * @param param
 * @return
 */
int sonata_gap_scan_activity_start_cmd_handler(uint8_t actv_idx, sonata_gap_scan_param_t *param);

/*!
 * @brief
 * @param actv_idx
 * @param param
 * @return
 */
int sonata_gap_period_sync_activity_start_cmd_handler(uint8_t actv_idx, sonata_gap_per_sync_param_t *param);

/*!
 * @brief
 * @param operation
 * @param actv_idx
 * @return
 */

int sonata_gap_activity_stop_cmd_handler(uint8_t operation, uint8_t actv_idx);

/*!
 * @brief
 * @param operation
 * @param actv_idx
 * @return
 */
int sonata_gap_activity_delete_cmd_handler(uint8_t operation, uint8_t actv_idx);

/*!
 * @brief
 * @param operation
 * @param actv_idx
 * @param length
 * @param data
 * @return
 */
int sonata_gap_set_adv_data_cmd_handler(uint8_t operation, uint8_t actv_idx, uint16_t length, uint8_t *data);

/*!
 * @brief
 * @param operation
 * @return
 */
int sonata_gap_get_dev_info_cmd_handler(sonata_gap_local_dev_info operation);

/*!
 * @brief
 * @param map
 * @return
 */
int sonata_gap_set_channel_map_cmd_handler(uint8_t *map);

/*!
 * @brief
 * @param cmd
 * @return
 */
/*!
 * @brief
 * @param cmd
 * @return
 */
int sonata_gap_resolv_addr_cmd_handler(uint8_t* address, uint8_t nb_key,struct gap_sec_key *irk);

/*!
 * @brief
 * @param cmd
 * @return
 */
int sonata_gap_gen_rand_addr_cmd_handler(uint8_t rnd_type,uint8_t *prand);

/*!
 * @brief
 * @param cmd
 * @return
 */
int sonata_gap_use_enc_block_cmd_handler(uint8_t *operand_1, uint8_t *operand_2);

/*!
 * @brief
 * @param cmd
 * @return
 */
int sonata_gap_gen_rand_nb_cmd_handler();

/*!
 * @brief
 * @param cmd
 * @return
 */
int sonata_gap_gen_dh_key_cmd_handler(uint8_t *operand_1, uint8_t *operand_2);

/*!
 * @brief
 * @return
 */
int sonata_gap_get_pub_key_cmd_handler();

/*!
 * @brief
 * @param actv_idx
 * @param enable
 * @return
 */
int sonata_gap_per_adv_cte_tx_ctl_cmd_handler(uint8_t actv_idx, uint8_t enable);

/*!
 * @brief
 * @param actv_idx
 * @param enable
 * @return
 */
int sonata_gap_per_adv_report_ctrl_cmd_handler(uint8_t actv_idx, uint8_t enable);

/*!
 * @brief
 * @param cmd
 * @return
 */
int sonata_gap_per_sync_iq_sampling_ctrl_cmd_handler(uint8_t actv_idx, uint8_t enable, uint8_t slot_dur, uint8_t max_sampl_cte,
                                                     uint8_t switching_pattern_len, uint8_t *antenna_id);

/*!
 * @brief
 * @param actv_idx
 * @param init_scan_actv_idx
 * @return
 */
int sonata_gap_addr_renew_cmd_handler(uint8_t actv_idx, uint8_t init_scan_actv_idx);

/*!
 * @brief
 * @param size
 * @param addrs
 * @return
 */
int sonata_gap_white_list_set_cmd_handler( uint8_t size,struct gap_bdaddr * addrs);

/*!
 * @brief
 * @param size
 * @param devInfos
 * @return
 */
int sonata_gap_resolving_list_set_cmd_handler(uint8_t size, struct gap_ral_dev_info *devInfos);

/*!
 * @brief
 * @param size
 * @param palInfos
 * @return
 */
int sonata_gap_periodic_advertiser_list_set_cmd_handler(uint8_t size, struct gapm_period_adv_addr_cfg *palInfos);

/*!
 * @brief
 * @param operation
 * @param peer_identity
 * @return
 */
int sonata_gap_get_ral_addr_cmd_handler(uint8_t operation, uint8_t addr_type, uint8_t *addr);

/*!
 * @brief
 * @param cmd
 * @return
 */
//int sonata_gap_profile_task_diss_add_cmd_handler(struct sonata_gap_profile_task_add_cmd *cmd, uint16_t param_len);


int sonata_gap_profile_task_add_cmd_handler(sonata_ble_profile_id prf_id, uint8_t sec_lvl, uint16_t start_hdl, uint16_t value_len, void * value);

/*!
 * @brief
 * @param cmd
 * @return
 */
int sonata_gap_set_irk_cmd_handler(uint8_t *key);

/*!
 * @brief
 * @param le_psm
 * @param sec_lvl
 * @return
 */
int sonata_gap_lepsm_register_cmd_handler(uint16_t le_psm, uint8_t sec_lvl);

/*!
 * @brief
 * @param le_psm
 * @return
 */
int sonata_gap_lepsm_unregister_cmd_handler(uint16_t le_psm);

/*!
 * @brief
 * @param cmd
 * @return
 */
int sonata_gap_le_test_mode_ctrl_cmd_handler(uint8_t operation, uint8_t channel, uint8_t tx_data_length, uint8_t tx_pkt_payload,
                                             uint8_t phy, uint8_t modulation_idx, uint8_t cte_len, uint8_t cte_type,
                                             uint8_t slot_dur, uint8_t switching_pattern_len, uint8_t *antenna_id);

/*!
 * @brief
 * @param cmd
 * @return
 */
int sonata_gap_dbg_iqgen_cfg_cmd_handler(struct sonata_gap_dbg_iqgen_cfg_cmd *cmd);


/*!
 * @brief
 * @param msgid
 * @param p_param
 * @param dest_id
 * @param src_id
 * @return
 */
int sonata_gap_dbg_mem_info_ind_handler(ke_msg_id_t msgid, void *p_param, ke_task_id_t dest_id, ke_task_id_t src_id);

/*!
 * @brief
 * @param msgid
 * @param p_param
 * @param dest_id
 * @param src_id
 * @return
 */
int sonata_gap_antenna_inf_ind_handler(ke_msg_id_t msgid, void *p_param, ke_task_id_t dest_id, ke_task_id_t src_id);

/*!
 * @brief
 * @param msgid
 * @param p_param
 * @param dest_id
 * @param src_id
 * @return
 */
int sonata_gap_peer_name_ind_handler(ke_msg_id_t msgid, void *p_param, ke_task_id_t dest_id, ke_task_id_t src_id);

/*!
 * @brief
 * @param msgid
 * @param p_param
 * @param dest_id
 * @param src_id
 * @return
 */
int sonata_gap_addr_solved_ind_handler(ke_msg_id_t msgid, void *p_param, ke_task_id_t dest_id, ke_task_id_t src_id);

/*!
 * @brief
 * @param msgid
 * @param p_param
 * @param dest_id
 * @param src_id
 * @return
 */
int sonata_gap_use_enc_block_ind_handler(ke_msg_id_t msgid, void *p_param, ke_task_id_t dest_id, ke_task_id_t src_id);

/*!
 * @brief
 * @param msgid
 * @param p_param
 * @param dest_id
 * @param src_id
 * @return
 */
int sonata_gap_gen_rand_nb_ind_handler(ke_msg_id_t msgid, void *p_param, ke_task_id_t dest_id, ke_task_id_t src_id);

/*!
 * @brief
 * @param msgid
 * @param p_param
 * @param dest_id
 * @param src_id
 * @return
 */
int sonata_gap_unknown_task_ind_handler(ke_msg_id_t msgid, void *p_param, ke_task_id_t dest_id, ke_task_id_t src_id);

/*!
 * @brief
 * @param msgid
 * @param p_param
 * @param dest_id
 * @param src_id
 * @return
 */
int sonata_gap_sugg_dflt_data_len_ind_handler(ke_msg_id_t msgid, void *p_param, ke_task_id_t dest_id, ke_task_id_t src_id);

/*!
 * @brief
 * @param msgid
 * @param p_param
 * @param dest_id
 * @param src_id
 * @return
 */
int sonata_gap_max_data_len_ind_handler(ke_msg_id_t msgid, void *p_param, ke_task_id_t dest_id, ke_task_id_t src_id);

/*!
 * @brief
 * @param msgid
 * @param p_param
 * @param dest_id
 * @param src_id
 * @return
 */
int sonata_gap_ral_addr_ind_handler(ke_msg_id_t msgid, void *p_param, ke_task_id_t dest_id, ke_task_id_t src_id);

/*!
 * @brief
 * @param msgid
 * @param p_param
 * @param dest_id
 * @param src_id
 * @return
 */
int sonata_gap_le_test_end_ind_handler(ke_msg_id_t msgid, void *p_param, ke_task_id_t dest_id, ke_task_id_t src_id);

/*!
 * @brief
 * @param msgid
 * @param p_param
 * @param dest_id
 * @param src_id
 * @return
 */
int sonata_gap_le_test_iq_report_ind_handler(ke_msg_id_t msgid, void *p_param, ke_task_id_t dest_id, ke_task_id_t src_id);

/*!
 * @brief
 * @param msgid
 * @param p_param
 * @param dest_id
 * @param src_id
 * @return
 */
int sonata_gap_iso_stat_ind_handler(ke_msg_id_t msgid, void *p_param, ke_task_id_t dest_id, ke_task_id_t src_id);

/*!
 * @brief
 * @param msgid
 * @param p_param
 * @param dest_id
 * @param src_id
 * @return
 */
int sonata_gap_gen_dh_key_ind_handler(ke_msg_id_t msgid, void *p_param, ke_task_id_t dest_id, ke_task_id_t src_id);

/*!
 * @brief
 * @param msgid
 * @param p_param
 * @param dest_id
 * @param src_id
 * @return
 */
int sonata_gap_pub_key_ind_handler(ke_msg_id_t msgid, void *p_param, ke_task_id_t dest_id, ke_task_id_t src_id);

/*!
 * @brief
 * @param msgid
 * @param p_param
 * @param dest_id
 * @param src_id
 * @return
 */
int sonata_gap_list_size_ind_handler(ke_msg_id_t msgid, void *p_param, ke_task_id_t dest_id, ke_task_id_t src_id);

/*!
 * @brief
 * @param msgid
 * @param p_param
 * @param dest_id
 * @param src_id
 * @return
 */
int sonata_gap_scan_request_ind_handler(ke_msg_id_t msgid, void *p_param, ke_task_id_t dest_id, ke_task_id_t src_id);

/*!
 * @brief
 * @param msgid
 * @param p_param
 * @param dest_id
 * @param src_id
 * @return
 */
int sonata_gap_sync_established_ind_handler(ke_msg_id_t msgid, void *p_param, ke_task_id_t dest_id, ke_task_id_t src_id);

/*!
 * @brief
 * @param msgid
 * @param p_param
 * @param dest_id
 * @param src_id
 * @return
 */
int sonata_gap_nb_adv_sets_ind_handler(ke_msg_id_t msgid, void *p_param, ke_task_id_t dest_id, ke_task_id_t src_id);

/*!
 * @brief
 * @param msgid
 * @param p_param
 * @param dest_id
 * @param src_id
 * @return
 */
int sonata_gap_dev_tx_pwr_ind_handler(ke_msg_id_t msgid, void *p_param, ke_task_id_t dest_id, ke_task_id_t src_id);

/*!
 * @brief
 * @param msgid
 * @param p_param
 * @param dest_id
 * @param src_id
 * @return
 */
int sonata_gap_dev_rf_path_comp_ind_handler(ke_msg_id_t msgid, void *p_param, ke_task_id_t dest_id, ke_task_id_t src_id);

/*!
 * @brief
 * @param msgid
 * @param p_param
 * @param dest_id
 * @param src_id
 * @return
 */
int sonata_gap_unknown_msg_ind_handler(ke_msg_id_t msgid, void *p_param, ke_task_id_t dest_id, ke_task_id_t src_id);

/*!
 * @brief
 * @param msgid
 * @param p_param
 * @param dest_id
 * @param src_id
 * @return
 */
int sonata_gap_per_adv_iq_report_ind_handler(ke_msg_id_t msgid, void *p_param, ke_task_id_t dest_id, ke_task_id_t src_id);


/***************************   GAPC ****************************************/

/*!
 * @brief
 * @param conidx
 * @param cmd
 * @return
 */
int sonata_gap_connection_cfm_cmd_handler(uint8_t conidx, struct sonata_gap_connection_cfm *cmd);

/// \brief
/// \param conidx
/// \param accept
/// \param ce_len_min
/// \param ce_len_max
/// \return
int sonata_gap_param_update_cfm_cmd_handler(uint8_t conidx, bool accept, uint16_t ce_len_min, uint16_t ce_len_max);

/*!
 * @brief
 * @param conidx
 * @param req
 * @param status
 * @return
 */
int sonata_gap_set_dev_info_cfm_cmd_handler(uint8_t conidx, uint8_t req, uint8_t status);

/*!
 * @brief
 * @param conidx
 * @param accept
 * @param cfm
 * @return
 */
int sonata_gap_bond_cfm_handler(uint8_t conidx, uint8_t accept, struct sonata_gap_bond_cfm *cfm);

/*!
 * @brief
 * @param accept
 * @param pair
 * @return
 */
int sonata_gap_bond_cfm_pairing_rsp_handler(uint8_t conidx, uint8_t accept, sonata_gap_pairing_t *pair);

/*!
 * @brief
 * @param conidx
 * @param accept
 * @param ediv
 * @param randnb
 * @param key_size
 * @param key
 * @return
 */
int sonata_gap_bond_cfm_ltk_exch_handler(uint8_t conidx,uint8_t accept, uint16_t ediv,uint8_t * randnb,uint8_t key_size,uint8_t *key);
/*!
 * @brief
 * @param accept
 * @param csrk
 * @return
 */
int sonata_gap_bond_cfm_csrk_exch_handler(uint8_t conidx, uint8_t accept, uint8_t *csrk);
/*!
 * @brief
 * @param accept
 * @param tk
 * @return
 */
int sonata_gap_bond_cfm_tk_exch_handler(uint8_t conidx, uint8_t accept, uint8_t *tk);
/*!
 * @brief
 * @param accept
 * @param irk
 * @param addr_type
 * @param addr
 * @return
 */
int sonata_gap_bond_cfm_irk_exch_handler(uint8_t conidx, uint8_t accept, uint8_t *irk,uint8_t addr_type, uint8_t * addr);
/*!
 * @brief
 * @param accept
 * @param oob_conf
 * @param oob_rand
 * @return
 */
int sonata_gap_bond_cfm_oob_exch_handler(uint8_t conidx, uint8_t accept, uint8_t *oob_conf, uint8_t* oob_rand);

/*!
 * @brief
 * @param cmd
 * @return
 */
int sonata_gap_bond_cfm_nc_exch_handler(uint8_t conidx, uint8_t accept);
/*!
 * @brief
 * @param cmd
 * @return
 */
int sonata_gap_encrypt_cfm_handler(uint8_t conidx, uint8_t found, uint8_t key_size, uint8_t *ltk);

/*!
 * @brief
 * @param cmd
 * @return
 */
int sonata_gap_get_dev_info_cfm_handler(uint8_t conidx, struct sonata_gap_get_dev_info_cfm *cmd);

/*!
 * @brief
 * @param length
 * @param name
 * @return
 */
int sonata_gap_get_dev_info_cfm_dev_name_handler(uint8_t conidx, uint16_t length, uint8_t *name);

/*!
 * @brief
 * @param appearance
 * @return
 */
int sonata_gap_get_dev_info_cfm_dev_appearance_handler(uint8_t conidx, uint16_t appearance);

/*!
 * @brief
 * @param con_intv_min
 * @param con_intv_max
 * @param slave_latency
 * @param conn_timeout
 * @return
 */
int sonata_gap_get_dev_info_cfm_dev_slv_perf_params_handler(uint8_t conidx, uint16_t con_intv_min, uint16_t con_intv_max, uint16_t slave_latency, uint16_t conn_timeout);

/*!
 * @brief
 * @param ctl_addr_resol
 * @return
 */
int sonata_gap_get_dev_info_cfm_ctl_addr_resol_handler(uint8_t conidx, uint8_t ctl_addr_resol);
/*!
 * @brief
 * @param msgid
 * @param p_param
 * @param dest_id
 * @param src_id
 * @return
 */
int sonata_gap_connection_cfm_handler(uint8_t conidx, ke_msg_id_t msgid, void *p_param, ke_task_id_t dest_id, ke_task_id_t src_id) ;

/*!
 * @brief
 * @param conidx
 * @param reason
 * @return
 */
int sonata_gap_disconnect_cmd_handler(uint8_t conidx, uint8_t reason);

/*!
 * @brief
 * @param conidx
 * @param operation
 * @return
 */
int sonata_gap_get_peer_info_cmd_handler(uint8_t conidx, uint8_t operation);

/*!
 * @brief
 * @param conidx
 * @param cmd
 * @return
 */
int sonata_gap_param_update_cmd_handler(uint8_t conidx, uint8_t pkt_id, uint16_t intv_min, uint16_t intv_max, uint16_t latency,
                                        uint16_t time_out, uint16_t ce_len_min, uint16_t ce_len_max);

/*!
 * @brief
 * @param conidx
 * @param latency
 * @return
 */
int sonata_gap_set_pref_slave_latency_cmd_handler(uint8_t conidx, uint16_t latency);

/*!
 * @brief
 * @param conidx
 * @param duration
 * @param single_tx
 * @return
 */
int sonata_gap_set_pref_slave_evt_dur_cmd_handler(uint8_t conidx, uint16_t duration, uint8_t single_tx);

/*!
 * @brief
 * @param conidx
 * @param para
 * @return
 */
int sonata_gap_bond_cmd_handler(uint8_t conidx, uint8_t iocap, uint8_t oob, uint8_t auth, uint8_t key_size,
                                uint8_t ikey_dist, uint8_t rkey_dist, uint8_t sec_req);

/*!
 * @brief
 * @param conidx
 * @param para
 * @return
 */
int sonata_gap_encrypt_cmd_handler(uint8_t conidx, struct sonata_gap_ltk *para);

/*!
 * @brief
 * @param conidx
 * @param auth_level
 * @return
 */
int sonata_gap_security_cmd_handler(uint8_t conidx, uint8_t auth_level);

/*!
 * @brief
 * @param conidx
 * @param cmd
 * @return
 */
int sonata_gap_sign_packet_cmd_handler(uint8_t conidx, uint16_t byte_len, uint8_t * data);

/*!
 * @brief
 * @param conidx
 * @param cmd
 * @return
 */
int sonata_gap_sign_check_cmd_handler(uint8_t conidx, uint16_t byte_len, uint8_t * data);
/*!
 * @brief
 * @param conidx
 * @param notification_type
 * @return
 */
int sonata_gap_key_press_notification_cmd_handler(uint8_t conidx, uint8_t notification_type);

/*!
 * @brief
 * @param conidx
 * @param timeout
 * @return
 */
int sonata_gap_set_le_ping_to_cmd_handler(uint8_t conidx, uint16_t timeout);

/*!
 * @brief
 * @param conidx
 * @param tx_octets
 * @param tx_time
 * @return
 */
int sonata_gap_set_le_pkt_size_cmd_handler(uint8_t conidx, uint16_t tx_octets, uint16_t tx_time);

/*!
 * @brief
 * @param conidx
 * @param tx_phy
 * @param rx_phy
 * @param phy_opt
 * @return
 */
int sonata_gap_set_phy_cmd_handler(uint8_t conidx, uint8_t tx_phy, uint8_t rx_phy, uint8_t phy_opt);

/*!
 * @brief
 * @param conidx
 * @param actv_idx
 * @param service_data
 * @return
 */
int sonata_gap_per_adv_sync_trans_cmd_handler(uint8_t conidx, uint8_t actv_idx, uint16_t service_data);

/*!
 * @brief
 * @param conidx
 * @param cmd
 * @return
 */
int sonata_gap_cte_tx_cfg_cmd_handler(uint8_t conidx, uint8_t cte_types, uint8_t switching_pattern_len, uint8_t*antenna_id);

/*!
 * @brief
 * @param conidx
 * @param cmd
 * @return
 */
int sonata_gap_cte_rx_cfg_cmd_handler(uint8_t conidx, bool sample_en,uint8_t slot_dur,uint8_t switching_pattern_len, uint8_t * antenna_id);

/*!
 * @brief
 * @param conidx
 * @param enable
 * @param interval
 * @param cte_len
 * @param cte_type
 * @return
 */
int sonata_gap_cte_req_ctrl_cmd_handler(uint8_t conidx, bool enable, uint16_t interval, uint8_t cte_len, uint8_t cte_type);

/*!
 * @brief
 * @param conidx
 * @param enable
 * @return
 */
int sonata_gap_cte_rsp_ctrl_cmd_handler(uint8_t conidx, bool enable);


/*!
 * @brief
 * @param msgid
 * @param p_param
 * @param dest_id
 * @param src_id
 * @return
 */
int sonata_gap_connection_req_ind_handler(ke_msg_id_t msgid, void *p_param, ke_task_id_t dest_id, ke_task_id_t src_id);

/*!
 * @brief
 * @param msgid
 * @param p_param
 * @param dest_id
 * @param src_id
 * @return
 */
int sonata_gap_param_update_req_ind_handler(ke_msg_id_t msgid, void *p_param, ke_task_id_t dest_id, ke_task_id_t src_id);

/*!
 * @brief
 * @param msgid
 * @param p_param
 * @param dest_id
 * @param src_id
 * @return
 */
int sonata_gap_disconnect_ind_handler(ke_msg_id_t msgid, void *p_param, ke_task_id_t dest_id, ke_task_id_t src_id);

/*!
 * @brief
 * @param msgid
 * @param p_param
 * @param dest_id
 * @param src_id
 * @return
 */
int sonata_gap_get_dev_info_req_ind_handler(ke_msg_id_t msgid, void *p_param, ke_task_id_t dest_id, ke_task_id_t src_id);

/*!
 * @brief
 * @param msgid
 * @param p_param
 * @param dest_id
 * @param src_id
 * @return
 */
int sonata_gap_param_updated_ind_handler(ke_msg_id_t msgid, void *p_param, ke_task_id_t dest_id, ke_task_id_t src_id);

/*!
 * @brief
 * @param msgid
 * @param p_param
 * @param dest_id
 * @param src_id
 * @return
 */
int sonata_gap_le_pkt_size_ind_handler(ke_msg_id_t msgid, void *p_param, ke_task_id_t dest_id, ke_task_id_t src_id);

/*!
 * @brief
 * @param msgid
 * @param p_param
 * @param dest_id
 * @param src_id
 * @return
 */
int sonata_gap_con_rssi_ind_handler(ke_msg_id_t msgid, void *p_param, ke_task_id_t dest_id, ke_task_id_t const src_id);

/*!
 * @brief
 * @param msgid
 * @param p_param
 * @param dest_id
 * @param src_id
 * @return
 */
int sonata_gap_peer_att_info_ind_handler(ke_msg_id_t msgid, void *p_param, ke_task_id_t dest_id, ke_task_id_t const src_id);

/*!
 * @brief
 * @param msgid
 * @param p_param
 * @param dest_id
 * @param src_id
 * @return
 */
int sonata_gap_peer_version_ind_handler(ke_msg_id_t msgid, void *p_param, ke_task_id_t dest_id, ke_task_id_t const src_id);

/*!
 * @brief
 * @param msgid
 * @param p_param
 * @param dest_id
 * @param src_id
 * @return
 */
int sonata_gap_peer_features_ind_handler(ke_msg_id_t msgid, void *p_param, ke_task_id_t dest_id, ke_task_id_t const src_id);

/*!
 * @brief
 * @param msgid
 * @param p_param
 * @param dest_id
 * @param src_id
 * @return
 */
int sonata_gap_update_conn_param_to_ind_handler(ke_msg_id_t msgid, void *p_param, ke_task_id_t dest_id, ke_task_id_t src_id);

/*!
 * @brief
 * @param msgid
 * @param p_param
 * @param dest_id
 * @param src_id
 * @return
 */
int sonata_gap_bond_req_ind_handler(ke_msg_id_t msgid, void *p_param, ke_task_id_t dest_id, ke_task_id_t const src_id);

/*!
 * @brief
 * @param msgid
 * @param p_param
 * @param dest_id
 * @param src_id
 * @return
 */
int sonata_gap_bond_ind_handler(ke_msg_id_t msgid, void *p_param, ke_task_id_t dest_id, ke_task_id_t const src_id);

/*!
 * @brief
 * @param msgid
 * @param p_param
 * @param dest_id
 * @param src_id
 * @return
 */
int sonata_gap_encrypt_req_ind_handler(ke_msg_id_t msgid, void *p_param, ke_task_id_t dest_id, ke_task_id_t const src_id);

/*!
 * @brief
 * @param msgid
 * @param p_param
 * @param dest_id
 * @param src_id
 * @return
 */
int sonata_gap_encrypt_ind_handler(ke_msg_id_t msgid, void *p_param, ke_task_id_t dest_id, ke_task_id_t const src_id);

/*!
 * @brief
 * @param msgid
 * @param p_param
 * @param dest_id
 * @param src_id
 * @return
 */
int sonata_gap_security_ind_handler(ke_msg_id_t msgid, void *p_param, ke_task_id_t dest_id, ke_task_id_t const src_id);

/*!
 * @brief
 * @param msgid
 * @param p_param
 * @param dest_id
 * @param src_id
 * @return
 */
int sonata_gap_sign_counter_ind_handler(ke_msg_id_t msgid, void *p_param, ke_task_id_t dest_id, ke_task_id_t const src_id);

/*!
 * @brief
 * @param msgid
 * @param p_param
 * @param dest_id
 * @param src_id
 * @return
 */
int sonata_gap_con_channel_map_ind_handler(ke_msg_id_t msgid, void *p_param, ke_task_id_t dest_id, ke_task_id_t const src_id);

/*!
 * @brief
 * @param msgid
 * @param p_param
 * @param dest_id
 * @param src_id
 * @return
 */
int sonata_gap_le_ping_to_val_ind_handler(ke_msg_id_t msgid, void *p_param, ke_task_id_t dest_id, ke_task_id_t const src_id);

/*!
 * @brief
 * @param msgid
 * @param p_param
 * @param dest_id
 * @param src_id
 * @return
 */
int sonata_gap_le_ping_to_ind_handler(ke_msg_id_t msgid, void *p_param, ke_task_id_t dest_id, ke_task_id_t const src_id);

/*!
 * @brief
 * @param msgid
 * @param p_param
 * @param dest_id
 * @param src_id
 * @return
 */
int sonata_gap_key_press_notification_ind_handler(ke_msg_id_t msgid, void *p_param, ke_task_id_t dest_id, ke_task_id_t const src_id);

/*!
 * @brief
 * @param msgid
 * @param p_param
 * @param dest_id
 * @param src_id
 * @return
 */
int sonata_gap_le_phy_ind_handler(ke_msg_id_t msgid, void *p_param, ke_task_id_t dest_id, ke_task_id_t const src_id);

/*!
 * @brief
 * @param msgid
 * @param p_param
 * @param dest_id
 * @param src_id
 * @return
 */
int sonata_gap_chan_sel_algo_ind_handler(ke_msg_id_t msgid, void *p_param, ke_task_id_t dest_id, ke_task_id_t const src_id);

/*!
 * @brief
 * @param msgid
 * @param p_param
 * @param dest_id
 * @param src_id
 * @return
 */
int sonata_gapc_unknown_msg_ind_handler(ke_msg_id_t msgid, void *p_param, ke_task_id_t dest_id, ke_task_id_t const src_id);

/*!
 * @brief
 * @param msgid
 * @param p_param
 * @param dest_id
 * @param src_id
 * @return
 */
int sonata_gap_cte_iq_report_ind_handler(ke_msg_id_t msgid, void *p_param, ke_task_id_t dest_id, ke_task_id_t const src_id);

/*!
 * @brief
 * @param msgid
 * @param p_param
 * @param dest_id
 * @param src_id
 * @return
 */
int sonata_gap_sign_ind_handler(ke_msg_id_t msgid, void *p_param, ke_task_id_t dest_id, ke_task_id_t const src_id);

/*!
 * @brief
 * @param msgid
 * @param p_param
 * @param dest_id
 * @param src_id
 * @return
 */
int sonata_gap_param_update_to_ind_handler(ke_msg_id_t msgid, void *p_param, ke_task_id_t dest_id, ke_task_id_t const src_id);

/*!
 * @brief
 * @param msgid
 * @param p_param
 * @param dest_id
 * @param src_id
 * @return
 */
int sonata_gap_smp_timeout_timer_ind_handler(ke_msg_id_t msgid, void *p_param, ke_task_id_t dest_id, ke_task_id_t const src_id);

/*!
 * @brief
 * @param msgid
 * @param p_param
 * @param dest_id
 * @param src_id
 * @return
 */
int sonata_gap_smp_rep_attempts_timer_ind_handler(ke_msg_id_t msgid, void *p_param, ke_task_id_t dest_id, ke_task_id_t const src_id);


/*!
 * @brief
 * @param
 * @return
 */
void sonata_gap_register_on_callback(void);


/***************************   BLE internal function ****************************************/
extern void sonata_ble_int_activity_log();

//extern void sonata_ble_int_activity_manager_init();

//extern void sonata_ble_int_activity_update_temp_index(uint8_t index);

extern void sonata_ble_int_activity_created(uint8_t actv_idx, enum gapm_actv_type  actv_type, int8_t tx_pwr);

extern void sonata_ble_int_activity_prepare_create(sonata_gap_operation create_type);

//extern void sonata_ble_int_activity_start(enum gapm_actv_type actv_type, uint8_t index, const uint8_t *addr);

extern void sonata_ble_int_activity_stop(enum gapm_actv_type actv_type, uint8_t index);

//extern void sonata_ble_int_activity_delete(uint8_t index);

//extern void sonata_ble_int_activity_delete_all();

//extern uint8_t sonata_ble_int_activity_get_type(uint8_t index);

extern void sonata_ble_int_set_connection(uint8_t conn_index, sonata_gap_connection_req_ind_t *param);

/***************************   BLE internal function  END****************************************/


#endif // BLE_HOST_PRESENT

#endif//_SONATA_GAP_API_INT_H_

