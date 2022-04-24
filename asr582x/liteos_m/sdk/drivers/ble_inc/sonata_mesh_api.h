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
 * @file sonata_mesh_api.h
 *
 * @brief header file - mesh api
 *
 ****************************************************************************************
 */

#ifndef _SONATA_MESH_API_H_
#define _SONATA_MESH_API_H_

/*
 * INCLUDE FILES
 ****************************************************************************************
 */
#include "sonata_utils_api.h"

/**
 * @defgroup SONATA_MESH_API Mesh API
 * @{
 */

/**
 * @defgroup PROFILE Mesh Profile
 * @ingroup SONATA_MESH_API
 * @brief Mesh Profile
 * @{
 */

/*
 * DEFINE
 **********************************************************
 */

// System Address Length
#define MESH_ADDR_LEN              6
// UUID Length
#define MESH_UUID_LEN              16
// Keys Length
#define MESH_KEYS_LEN              16

// Appkey Invaild local Index
#define MESH_APPKEY_INVALID_LID    (0xFFFF)
/**@} PROFILE */

/**
 * @defgroup ERROR Error Code
 * @brief Error Code
 * @ingroup SONATA_MESH_API
 * @{
 */

// Not Ready
#define MESH_ERROR_NOT_READY       (-2)
// Invalid Parameter
#define MESH_ERROR_INVALID_PARAM   (-1)
// No Error
#define MESH_ERROR_NO_ERROR         (0)
// No Space
#define MESH_ERROR_NO_SPACE         (1)
/**@} ERROR */

/**
 * @defgroup PROVISION Provison
 * @brief Provision
 * @ingroup SONATA_MESH_API
 * @{
 */

// Max size of Auth value
#define MESH_PROVISION_AUTH_MAX_NUM 16
/**@} PROVISION */

/**
 * @defgroup MODELS Mesh Models
 * @brief Mesh Models
 * @ingroup SONATA_MESH_API
 * @{
 */

// Max number of Mesh Model
#define MESH_MODEL_NUM_MAX         (10)
// Invalid Mesh Model Local Index
#define MESH_MODEL_INVALID_LID     (0xFF)
#define MESH_VENDOR_MODEL_NUM_MAX   (2)

/****************** Model IDs *******************/

// Generic Server - OnOff
#define MESH_MODELID_GENS_OO           (0x1000)
// Generic Server - Level
#define MESH_MODELID_GENS_LVL          (0x1002)
// Generic Server - Default Transition Time
#define MESH_MODELID_GENS_DTT          (0x1004)
// Generic Server - Power OnOff
#define MESH_MODELID_GENS_POO          (0x1006)
// Generic Server - Power OnOff Setup
#define MESH_MODELID_GENS_POOS         (0x1007)
// Generic Server - Power Level
#define MESH_MODELID_GENS_PLVL         (0x1009)
// Generic Server - Power Level Setup
#define MESH_MODELID_GENS_PLVLS        (0x100A)
// Generic Server - Battery
#define MESH_MODELID_GENS_BAT          (0x100C)
// Generic Server - Location
#define MESH_MODELID_GENS_LOC          (0x100E)
// Generic Server - Location Setup
#define MESH_MODELID_GENS_LOCS         (0x100F)
// Generic Server - User Property
#define MESH_MODELID_GENS_UPROP        (0x1013)
// Generic Server - Admin Property
#define MESH_MODELID_GENS_APROP        (0x1011)
// Generic Server - Manufacturer Property
#define MESH_MODELID_GENS_MPROP        (0x1012)
// Generic Server - Client Property
#define MESH_MODELID_GENS_CPROP        (0x1014)

// Generic Client - OnOff
#define MESH_MODELID_GENC_OO           (0x1001)
// Generic Client - Level
#define MESH_MODELID_GENC_LVL          (0x1003)

// Lighting Server - Light Lightness
#define MESH_MODELID_LIGHTS_LN         (0x1300)
// Lighting Server - Light CTL
#define MESH_MODELID_LIGHTS_CTL        (0x1303)
// Lighting Server - Light HSL
#define MESH_MODELID_LIGHTS_HSL        (0x1307)

// Ali Vendor Sever Model ID
#define MESH_MODELID_VENDOR_ALI        (0x01A80000)
// asr vendor sever model ID
#define MESH_MODELID_VENS_ASR          (0x09170002)
/// asr vendor client model ID
#define MESH_MODELID_VENC_ASR          (0x09170001)
// a
/*********** MESSAGE LENGTH******************/

// Minimal length of Generic OnOff Set/Set Unacknowledged message
#define MESH_MODELID_GEN_OO_SET_MIN_LEN              (2)
// Length of Generic OnOff Set/Set Unacknowledged message when all field are present
#define MESH_MODELID_GEN_OO_SET_LEN                  (4)
// Minimal length of Generic OnOff Status message
#define MESH_MODELID_GEN_OO_STATUS_MIN_LEN           (1)
// Length of Generic OnOff Status message when all fields are present
#define MESH_MODELID_GEN_OO_STATUS_LEN               (3)

// Minimal length of Generic Level Set/Set Unacknowledged message
#define MESH_MODELID_GEN_LVL_SET_MIN_LEN             (3)
// Length of Generic Level Set/Set Unacknowledged message when all field are present
#define MESH_MODELID_GEN_LVL_SET_LEN                 (5)
// Minimal length of Generic Delta Set/Set Unacknowledged message
#define MESH_MODELID_GEN_LVL_SET_DELTA_MIN_LEN       (5)
// Length of Generic Delta Set/Set Unacknowledged message when all field are present
#define MESH_MODELID_GEN_LVL_SET_DELTA_LEN           (7)
// Minimal length of Generic Move Set/Set Unacknowledged message
#define MESH_MODELID_GEN_LVL_SET_MOVE_MIN_LEN        (3)
// Length of Generic Move Set/Set Unacknowledged message when all field are present
#define MESH_MODELID_GEN_LVL_SET_MOVE_LEN            (5)
// Minimal length of Generic Level Status message
#define MESH_MODELID_GEN_LVL_STATUS_MIN_LEN          (2)
// Length of Generic Level Status message when all fields are present
#define MESH_MODELID_GEN_LVL_STATUS_LEN              (5)

/**@} MODELS */

/**
 * @addtogroup PROVISION
 * @{
 */

/*
 *ENUMERATIONS
 **********************************************************
 */

// Algorithms field values
enum mesh_prov_algo {
    // Bit[0]: FIPS P-256 Elliptic Curve
    MESH_PROV_ALGO_FIPS_P256_ELLIPTIC_CURVE = 0x0001,
    /// < Bit[1-15]: Reserved for Future Use
};

// Public Key Type field values
enum mesh_prov_pub_key_val {
    // No OOB Public Key is used
    MESH_PROV_PUB_KEY_OOB_NOT_USED = 0x00,
    // OOB Public Key is used
    MESH_PROV_PUB_KEY_OOB_USED     = 0x01,

    MESH_PROV_PUB_KEY_OOB_MAX,
};

// Authentication Method field values
enum mesh_prov_auth_method {
    // No OOB authentication is used
    MESH_PROV_AUTH_NO_OOB,
    // Static OOB authentication is used
    MESH_PROV_AUTH_STATIC_OOB,
    // Output OOB authentication is used
    MESH_PROV_AUTH_OUTPUT_OOB,
    // Input OOB authentication is used
    MESH_PROV_AUTH_INPUT_OOB,
};

// Static OOB Type field values
enum mesh_prov_static_oob {
    // Bit[0]: Static OOB information available
    MESH_PROV_STATIC_OOB_AVAILABLE = 0x01,
    /// < Bit[1-7]: Prohibited
};

// Output OOB Action field values
enum mesh_prov_out_oob {
    // Bit[0]: Blink
    MESH_PROV_OUT_OOB_BLINK          = 0x0001,
    // Bit[1]: Beep
    MESH_PROV_OUT_OOB_BEEP           = 0x0002,
    // Bit[2]: Vibrate
    MESH_PROV_OUT_OOB_VIBRATE        = 0x0004,
    // Bit[3]: Output Numeric
    MESH_PROV_OUT_OOB_NUMERIC        = 0x0008,
    // Bit[4]: Output Alphanumeric
    MESH_PROV_OUT_OOB_ALPHANUMERIC   = 0x0010,
    /// < Bit[5-15]: Reserved for Future Use
};

// Input OOB Action field values
enum mesh_prov_in_oob {
    // Bit[0]: Push
    MESH_PROV_IN_OOB_PUSH             = 0x0001,
    // Bit[1]: Twist
    MESH_PROV_IN_OOB_TWIST            = 0x0002,
    // Bit[2]: Input Numeric
    MESH_PROV_IN_OOB_NUMERIC          = 0x0004,
    // Bit[3]: Input Alphanumeric
    MESH_PROV_IN_OOB_ALPHANUMERIC     = 0x0008,
    /// < Bit[4-15]: Reserved for Future Use
};

/**
* @brief enum Role bit meaning
*/
typedef enum {
    // Device Role
    MESH_PROV_ROLE_DEVICE       = 0x00,
    // Provisioner Role
    MESH_PROV_ROLE_PROVISIONER  = 0x01,
} mesh_prov_role_t;

/**@}*/

/**
 * @addtogroup PROFILE
 * @{
 */

/**
* @brief enum Mesh Supported Features
*/
enum mesh_user_api_feat {
    // Relay Node
    MESH_FEAT_RELAY_NODE_SUP       = (1 << 0),
    // Proxy Node
    MESH_FEAT_PROXY_NODE_SUP       = (1 << 1),
    // Friend Node
    MESH_FEAT_FRIEND_NODE_SUP      = (1 << 2),
    // Low Power Node
    MESH_FEAT_LOW_POWER_NODE_SUP   = (1 << 3),
};

/**
* @brief enum Mesh param set type
*/
typedef enum {
    // Role
    MESH_ROLE,
    // Addr
    MESH_SYSTEM_ADDR,
    // Addr Type
    MESH_ADDR_TYPE,
    // Provision Param for UnProv Device
    MESH_PROV_PARAM,
    // Node Feature
    MESH_FEATURE_SUPPORT,
    // Relay Transmit
    MESH_RELAY_TRANSMIT,
    // Network Transmit
    MESH_NETWORK_TRANSMIT,
    // Default TTL
    MESH_DEFAULT_TTL,
    // System timing
    MESH_RUNNING_TIME,
    // Friend Params
    MESH_FRIEND_PARAM,
    // Data Save Interval
    MESH_DATA_SAVE_INTERVAL,
    // NetKey Used By Provisioner
    MESH_PROVISIONER_NETKEY,
    // AppKey Used By Provisioner
    MESH_PROVISIONER_APPKEY,
    // Addr Pool Used By Provisioner
    MESH_PROVISIONER_ADDR_POOL,
    // mesh model replay number
    MESH_NUMBER_REPLAY,
} mesh_param_opr_types_t;

/**
* @brief enum Mesh addr type
*/
typedef enum {
    // Public Addr
    MESH_PUBLIC_ADDR,
    // Private Static Addr
    MESH_PRIVATE_STATIC_ADDR,
    // Resolvable Private Random Addr
    MESH_RSLV_ADDR,
    // Non-Resolvable Private Random Addr
    MESH_NON_RSLV_ADDR,
} mesh_addr_types_t;

/**
* @brief enum  sys cmd type
*/
typedef enum {
    // Reboot Sys
    MESH_SYS_REBOOT,
    // Erase Data And Reboot, Node Will Be Unprovisioned
    MESH_SYS_RESET,
} mesh_sys_cmd_t;

/**
* @brief enum core evt indicate type
*/
typedef enum {
    // Request Public Key OOB Info
    MESH_PUBLIC_KEY_REQUEST,
    // Request Auth Data OOB Info
    MESH_AUTH_OOB_IND,
    // Indicate Attation Time
    MESH_ATTATION_IND,
    // Indicate Current Provision State
    MESH_PROVISION_STATE,
    // Indicate MODEL And APPKEY Binding Info
    MESH_APPKEY_BINDING,
    // Indicate APPKEY Update
    MESH_APPKEY_UPDATE,
    // Indicate MODEL And APPKEY Binded
    MESH_APPKEY_BINDED,
    // Indicate MODEL Model Has Subscribed To A Group Addr
    MESH_GRPADDR_SUBSED,
    // Indicate Local NetKey Update
    MESH_NETKEY_UPDATE,
    // Indicate Local NetKey Set Compelete
    MESH_NETKEY_SET_CMP,
    // Indicate Local AppKey Set Compelete
    MESH_APPKEY_SET_CMP,
    // Indicate Local Model & App Compelete
    MESH_MODEL_APP_BIND_CMP,
    // Indicate Local Publish Set Compelete
    MESH_PUB_SET_CMP,
    // Indicate Local Addr Set Compelete
    MESH_ADDR_SET_CMP,
    // Indicate Self Addr Alloced By Provisioner
    MESH_SELF_ADDR_IND,
    /// Indicate run time
    MESH_GET_RUN_TIME,
    // Indicate Publish Addr Update
    MESH_PUB_UPDATE,
    // Indicate Sub Addrs Update
    MESH_SUBS_UPDATE,
    // Indicate Self Dev Key
    MESH_SELF_DEV_KEY_IND,
    // Indicate Self Dev Key
    MESH_SELF_IV_SEQ_IND,
    // Indicate Friend Offer
    MESH_FRIEND_OFFER_IND,
    // Indicate lowpower Node Status
    MESH_LPN_STATUS_IND,
    // Indicate  Lowpower Start CmdCompelete
    MESH_LOWPOWER_START_CMP,
    // Indicate  Lowpower Stop Cmd Compelete
    MESH_LOWPOWER_STOP_CMP,
    // Indicate  Lowpower Select Friend Cmd Compelete
    MESH_LOWPOWER_SELECT_FRIEND_CMP,
} mesh_core_evt_ind_t;
/** @} */

/**
 * @addtogroup PROVISION
 * @{
 */

// State of the provisioning
enum mesh_prov_state {
    // Provisioning started - procedure started by a provisioner
    MESH_PROV_STARTED,
    // Provisioning succeed
    MESH_PROV_SUCCEED,
    // Provisioning failed
    MESH_PROV_FAILED,
};

/**
* @brief enum core provisioner evt indicate type
*/
typedef enum {
    // Indicate Provisioner found a device
    MESH_PROV_DEVICE_IND,
    // Indicate Peer Capability
    MESH_PROVISIONER_PROV_CAP_IND,
    // Request Provision Auth Data
    MESH_PROVISIONER_PROV_AUTH_REQ,
    // Request Provision Auth Data
    MESH_PROVISIONER_PROVISIONING_DATA_REQ,
    // Indicate Provisioning Result
    MESH_PROVISIONER_STATE_IND,
    // Indicate  Appkey Add Result
    MESH_PROVISIONER_APPKEY_ADD_STATUS,
    // Indicate Model App Bind Result
    MESH_PROVISIONER_MODEL_APP_IND,
    // Indicate Composition Data
    MESH_PROVISIONER_COMPOSITION_DATA,
    // Indicate Set Sub Result
    MESH_PROV_SET_SUBS_IND,
    // Indicate Set Remote Publish Result
    MESH_PROV_SET_PUBLISH_IND,
    // Indicate Set Local Publish Result
    MESH_PROV_SET_LOCAL_PUBLISH_IND,
    // Indicate Add Node Result
    MESH_PROV_ADD_LOCAL_NODE_IND,
    // Indicate Delete Local Node Result
    MESH_PROV_DELETE_LOCAL_NODE_IND,
    // Indicate Remote NetTransmit Status
    MESH_PROV_NETTRANSMIT_IND,
} mesh_core_provisioner_evt_ind_t;

/**
* @brief enum  app notify sys to compelete evt type
*/
typedef enum {
    // App Input Public Key OOB Info
    MESH_PUBLIC_KEY_SET,
    // App Input Auth Data OOB Info
    MESH_AUTH_DATA_SET,
    // Setting Local Netkey
    MESH_SET_LOCAL_NETKEY,
    // Setting Local Appkey
    MESH_SET_LOCAL_APPKEY,
    // Bind Local Model & AppKey
    MESH_SET_LOCAL_MODEL_APP_BIND,
    // Setting Local Publish Param
    MESH_SET_LOCAL_PUBLISH_PARAM,
    // Setting Local Sub Param
    MESH_SET_LOCAL_SUBS_PARAM,
    // Setting Local dev key
    MESH_SET_LOCAL_DEV_KEY,
    // Setting Local primary addr
    MESH_SET_LOCAL_PRIM_ADDR,
    // Setting Local iv index and seq no
    MESH_SET_LOCAL_IV_SEQ,
    // App Start Node LPN, Stack Will Send Friend Request
    MESH_LOWPOWER_NODE_START,
    // App Stop Node LPN, Clean Up Friendship
    MESH_LOWPOWER_NODE_STOP,
    // App Select Friend
    MESH_LOWPOWER_SELECT_FRIEND,
    // App Set Friend State
    MESH_SET_LOCAL_FRIEND_STATE,
} mesh_core_evt_cb_t;

/**
* @brief enum  app notify sys to compelete provisoner evt type
*/
typedef enum {
    // App Confrim To Provision A Device
    MESH_PROV_PROVISION_DEVICE,
    // Confirm selected Capability
    MESH_PROVISIONER_PROV_CAP_CFM,
    // Confirm Provision Auth Data
    MESH_PROVISIONER_PROV_AUTH_CFM,
    // Request Provision Auth Data
    MESH_PROVISIONER_PROVISIONING_DATA_CFM,
    // Provisioner To Get Composition Data
    MESH_PROV_GET_COMPOSITION_DATA,
    // Provisioner Add APPKEY To Node
    MESH_PROV_ADD_APPKEY,
    // Provisioner Bind Mpdel And Appkey
    MESH_PROV_MODEL_APP_BIND,
    // Provisioner Bind Mpdel And Appkey
    MESH_PROV_MODEL_APP_UNBIND,
    // Provisioner Setting Sub Param
    MESH_PROV_SET_SUBS_PARAM,
    // Provisioner Add Provisioned Node
    MESH_PROV_ADD_LOCAL_NODE,
    // Provisioner Delete Provisioned Node
    MESH_PROV_DEL_LOCAL_NODE,
    // Provisioner Set Local Addr
    MESH_PROV_SET_LOCAL_ADDR,
    // Provisioner Setting Sub Param
    MESH_PROV_SET_PUB_PARAM,
    // Provisioner Setting Net Transmit Param
    MESH_PROV_SET_NETTRANSMIT_PARAM,
} mesh_core_provisioner_evt_cb_t;

/**
* @brief enum  Proxy connectable advertising control values
*/
typedef enum mesh_proxy_broadcast_ctl {
    // Stop connectable advertising
    MESH_PROXY_ADV_STOP = 0,
    // Start connectable advertising with Node Identity (duration = 60s)
    MESH_PROXY_ADV_START_NODE_ID,
    // Start connectable advertising with Network ID (duration = 60s)
    MESH_PROXY_ADV_START_NET,
} mesh_proxy_broadcast_ctl_t;

/**@}*/

/**
 * @addtogroup MODELS
 * @{
 */

/*
 * ENUMERATIONS (MESSAGE CONTENT)
 ****************************************************************************************
 */

// Positions in Generic OnOff Set/Set Unacknowledged message
enum mesh_model_gen_oo_set_pos {
    // OnOff state value
    MESH_MODEL_GEN_OO_SET_OO_POS = 0,
    // TID value
    MESH_MODEL_GEN_OO_SET_TID_POS,
    // Transition time value
    MESH_MODEL_GEN_OO_SET_TRANS_TIME_POS,
    // Delay value
    MESH_MODEL_GEN_OO_SET_DELAY_POS,
};

// Positions in Generic OnOff Status message
enum mesh_model_gen_oo_status_pos {
    // Present OnOff state value
    MESH_MODEL_GEN_OO_STATUS_OO_POS = 0,
    // Target OnOff state value
    MESH_MODEL_GEN_OO_STATUS_TGT_OO_POS,
    // Remaining time value
    MESH_MODEL_GEN_OO_STATUS_REM_TIME_POS,
};

// Positions in Generic Level Set/Set Unacknowledged message
enum mesh_model_gen_lvl_set_pos {
    // Level value
    MESH_MODEL_GEN_LVL_SET_LVL_POS = 0,
    // TID value
    MESH_MODEL_GEN_LVL_SET_TID_POS = 2,
    // Transition time value
    MESH_MODEL_GEN_LVL_SET_TRANS_TIME_POS,
    // Delay value
    MESH_MODEL_GEN_LVL_SET_DELAY_POS,
};

/**
* @brief enum  model state
*/
typedef enum {
    // Generic OnOff state
    MESH_STATE_GEN_ONOFF = 0,
    // Generic Level state
    MESH_STATE_GEN_LVL,
    // Generic Default Transition Time state
    MESH_STATE_GEN_DTT,
    // Generic Power Actual state
    MESH_STATE_GEN_POWER_ACTUAL,
    // Generic Power Last state
    MESH_STATE_GEN_POWER_LAST,
    // Generic Power Default state
    MESH_STATE_GEN_POWER_DFLT,
    // Generic Power Range state
    MESH_STATE_GEN_POWER_RANGE,
    // Generic OnPowerUp state
    MESH_STATE_GEN_ONPOWERUP,

    // Light Lightness
    MESH_STATE_LIGHT_LN = 50,
    // Light Lightness Linear
    MESH_STATE_LIGHT_LN_LIN,
    // Light Lightness Last
    MESH_STATE_LIGHT_LN_LAST,
    // Light Lightness Default
    MESH_STATE_LIGHT_LN_DFLT,
    // Light Lightness Range
    MESH_STATE_LIGHT_LN_RANGE,
    // Light Lightness Range Min
    MESH_STATE_LIGHT_LN_RANGE_MIN,
    // Light Lightness Range Max
    MESH_STATE_LIGHT_LN_RANGE_MAX,

    // Light CTL Lightness
    MESH_STATE_LIGHT_CTL_LN = 100,
    // Light CTL Temperature
    MESH_STATE_LIGHT_CTL_TEMP,
    // Light CTL Delta UV
    MESH_STATE_LIGHT_CTL_DELTA_UV,
    // Light CTL Temperature Default
    MESH_STATE_LIGHT_CTL_TEMP_DFLT,
    // Light CTL Temperature Range
    MESH_STATE_LIGHT_CTL_TEMP_RANGE,
    // Light CTL Delta UV Default
    MESH_STATE_LIGHT_CTL_DELTA_UV_DFLT,

    // Light HSL Lightness
    MESH_STATE_LIGHT_HSL_LN = 150,
    // Light HSL Hue
    MESH_STATE_LIGHT_HSL_HUE,
    // Light HSL Saturation
    MESH_STATE_LIGHT_HSL_SAT,
    // Light HSL Target
    MESH_STATE_LIGHT_HSL_TGT,
    // Light HSL Default (Lightness + Hue + Saturation)
    MESH_STATE_LIGHT_HSL_DFLT,
    // Light HSL Lightness Default
    MESH_STATE_LIGHT_HSL_DFLT_LN,
    // Light HSL Hue Default
    MESH_STATE_LIGHT_HSL_DFLT_HUE,
    // Light HSL Saturation Default
    MESH_STATE_LIGHT_HSL_DFLT_SAT,
    // Light HSL Hue Range
    MESH_STATE_LIGHT_HSL_RANGE_HUE,
    // Light HSL Saturation Range
    MESH_STATE_LIGHT_HSL_RANGE_SAT,

    // Light xyL Lightness
    MESH_STATE_LIGHT_XYL_LN = 200,
    // Light xyL x and y
    MESH_STATE_LIGHT_XYL_XY,
    // Light xyL Lightness Target
    MESH_STATE_LIGHT_XYL_LN_TGT,
    // Light xyL x and y Target
    MESH_STATE_LIGHT_XYL_XY_TGT,
    // Light xyL Lightness Default
    MESH_STATE_LIGHT_XYL_LN_DFLT,
    // Light xyL x and y Default
    MESH_STATE_LIGHT_XYL_XY_DFLT,
    // Light xyL x and y Range
    MESH_STATE_LIGHT_XYL_XY_RANGE
} mesh_state_type_t;

// Mesh Scan Opr Module Define,
typedef enum {
    USER_APP_INVALID_MODULE_ID,
    USER_APP_MODULE_ID,
    USER_WF_MODULE_ID,
    USER_AT_MODULE_ID,
    USER_MAX_MODULE_ID
} mesh_scan_opr_module_id_t;

/*
 * TYPE DEFINITIONS
**********************************************************
*/

// Model ID
typedef uint32_t modelid_t;
// Model Local Index
typedef uint8_t mesh_lid_t;
// Status
typedef int32_t STATUS     ;

/**@}*/

/**
 * @addtogroup PROFILE
 * @{
 */

// role struct
typedef struct {
    // Role
    uint8_t role;
} mesh_role_t;

/**@} PROFILE */

/**
 * @addtogroup PROVISION
 * @{
 */

/**
* @brief   provision param
*/
typedef struct {
    // Device UUID
    uint8_t uuid[MESH_UUID_LEN];
    // URI Hash
    uint8_t *p_uri;
    // Static OOB information available
    uint8_t  static_oob;
    // Static OOB information len
    uint8_t   static_oob_len;
    // OOB information
    uint8_t oob_info;
    // Supported Public Key OOB
    uint8_t  pub_key_oob;
    // Maximum size of Output OOB supported
    uint8_t output_size;
    // Supported Output OOB Actions
    uint8_t output_actions;
    // Maximum size in octets of Input OOB supported
    uint8_t input_size;
    // Supported Input OOB Actions
    uint8_t input_actions;
    // 16-bit company identifier assigned by the Bluetooth SIG
    uint16_t cid;
    // 16-bit vendor-assigned product identifier
    uint16_t pid;
    // 16-bit vendor-assigned product version identifier
    uint16_t vid;
    // Localization descriptor
    uint16_t loc;
} mesh_prov_param_t;
/**@}*/

/**
 * @addtogroup PROFILE
 * @{
 */

/**
* @brief   friend param
*/
typedef struct {
    // Number of addresses that can be stored for Message Replay Protection
    uint16_t nb_addr_replay;
    // Receive window in milliseconds when Friend feature is supported
    uint8_t frd_rx_window_ms;
    // Queue size when Friend feature is supported
    uint8_t frd_queue_size;
} mesh_friend_param_t;

/**
* @brief   mesh support feature such as relay/proxy/friend
*/
typedef struct {
    // Mask of supported features (@see enum mesh_user_api_feat)
    uint32_t feature;  // m_user_api_feat
} mesh_feature_support_param_t;

/**
* @brief   mesh adv interval and count
*/
typedef struct {
    // Advertising Count
    uint8_t count;
    // Advertising interval in ms
    uint8_t interval;
} mesh_transmit_param_t;

/**
* @brief   mesh default ttl
*/
typedef struct {
    // TTL
    uint8_t ttl;
} mesh_default_ttl_param_t;

/**
* @brief   current time of unit ms from  power off
*/
typedef struct {
    // Current clock value in milliseconds
    uint32_t time_ms;
    // Number of time the clock has wrapped since up time
    uint16_t wrap;
} mesh_running_time_t;

/**
* @brief   interval of saving data
*/
typedef struct {
    // Data Save interval
    uint32_t time_secs;
} mesh_data_save_interval_t;

/**
* @brief   addr type struct
*/
typedef struct {
    // Addr
    mesh_addr_types_t type ;
} mesh_system_addr_type_t;
/** @} */

/**
 * @addtogroup PROVISION
 * @{
 */

/**
* @brief   provisioner netkey set  struct
*/
typedef struct {
    // opr
    bool addOrDel;
    // key
    uint8_t netkey[MESH_KEYS_LEN];
    // key_index
    uint16_t key_index;
} mesh_provisioner_netkey_t;

/**
* @brief   provisioner appkey struct
*/
typedef struct {
    // opr
    bool addOrDel;
    // key
    uint8_t appkey[MESH_KEYS_LEN];
    // key_index
    uint16_t key_index;
    // netkey_index
    uint16_t netkey_index;
} mesh_provisioner_appkey_t;

/**
* @brief   provisioner addr pool struct
*/
typedef struct {
    /// provisioner addr pool start
    uint16_t addr_start ;
    /// provisioner addr pool end
    uint16_t addr_end   ;
    /// used by provisioner to communicate to unprov device
    uint16_t addr_self  ;
} mesh_provisioner_addr_pool_t;

/**
* @brief   mesh set params union structure
*/
typedef union {
    /// Role struct
    mesh_role_t mesh_role;
    /// provision param
    mesh_prov_param_t prov_param;
    /// mesh support feature such as relay/proxy/friend
    mesh_feature_support_param_t feature_param;
    /// mesh adv interval and count
    mesh_transmit_param_t network_transmit;
    /// mesh adv interval and count
    mesh_transmit_param_t relay_transmit;
    /// mesh default ttl
    mesh_default_ttl_param_t default_ttl;
    /// current time of unit ms from  power off
    mesh_running_time_t running_time;
    /// friend param
    mesh_friend_param_t friend;
    /// interval of saving data
    mesh_data_save_interval_t interval;
    ///addr type struct
    mesh_system_addr_type_t addr_type;
    /// provisioner netkey set  struct
    mesh_provisioner_netkey_t netkey;
    /// provisioner appkey struct
    mesh_provisioner_appkey_t appkey;
    /// provisioner addr pool struct
    mesh_provisioner_addr_pool_t addr_pool;
    /// mesh model repaly number
    uint8_t replay_number;
} mesh_set_params_t;
/**@}*/

/**
 * @addtogroup PROFILE
 * @{
 */

/**
* @brief   addr struce
*/
typedef struct {
    // Addr
    uint8_t bd_addr[MESH_ADDR_LEN];
} mesh_system_addr_t;

/**
* @brief   mesh get params union structure
*/
typedef union {
    mesh_system_addr_t addr;
} mesh_get_params_t;

/**@}*/

/**
 * @addtogroup MODELS
 * @{
 */

/**
* @brief   Inform reception of a specific mesh message
*/
typedef struct {
    // Model ID
    uint32_t model_id;
    /// Element index of the message
    uint8_t element;
    // Application Key Local identifier (Required for a response)
    uint16_t   appkey_global_index;
    // Measured RSSI level for the received PDU.
    int8_t   rssi;
    // 1 = if message have been received by an immediate peer; 0 = it can have been relayed
    uint8_t  not_relayed;
    // Mesh message operation code (can be 1, 2 or 3 octet operation code)
    uint32_t opcode;
    // Source address of the message (Required for a response)
    uint16_t src;
    // Message length
    uint16_t msg_len;
    // Message content
    uint8_t  *msg;
} mesh_api_model_msg_rx_t;

// Mesh Vendor Model Message Rx
typedef  void (* m_vendor_model_msg_rx_t)(mesh_api_model_msg_rx_t *pkt);

/**
* @brief   server send msg params
*/
typedef struct {
    // Index of element the model belongs to (zero based)
    uint8_t element;
    // Application Key Local identifier (Required for a response)
    int8_t   mic_64;
    // TTL
    int8_t   ttl;
    // Application Key Global Index
    int16_t  appkey_global_index;
    // Model ID
    uint32_t modelid;
    // Mesh message operation code (can be 1, 2 or 3 octet operation code)
    uint32_t opcode;
    // Dst address of the message (Required for a response)
    uint16_t dst;
    // Message length
    uint16_t msg_len;
    // Message content
    uint8_t  msg[__ARRAY_EMPTY];
} mesh_model_msg_param_t;

// Mesh Model Message Sent Callback
typedef  void (* model_msg_sent_cb)(mesh_model_msg_param_t *p_param, STATUS status) ;

// OnOff state value set structure
typedef struct {
    // OnOff state value
    uint8_t onoff;
} generic_onoff_set_param_t;

// Level state value set structure
typedef struct {
    // Level state value
    uint32_t value;
} generic_level_set_param_t;

// Lightness state value set structure
typedef struct {
    // Lightness state value
    uint16_t lightness;
} generic_lightness_set_param_t;

/**
 * @brief   client send msg union struct
 */
typedef union {
    /// OnOff state value set structure
    generic_onoff_set_param_t onoff;
    /// Level state value set structure
    generic_level_set_param_t level;
    /// Lightness state value set structure
    generic_lightness_set_param_t lightness;
} generic_client_params_t;

/**
* @brief enum client msg send type
*/
typedef enum {
    // Generic OnOff Set
    MESH_MSG_GEN_OO_SET,
    // Generic OnOff Set Unacknowledged
    MESH_MSG_GEN_OO_SET_UNACK,
} mesh_client_msg_t;

// mesh client msg param structure
typedef struct {
    // Msg Type (@see mesh_client_msg_t)
    mesh_client_msg_t msg_type;
    // Transition time in milliseconds
    uint32_t trans_time_ms;
    // Destination
    uint16_t dst;
    // Delay in milliseconds
    uint16_t delay_ms;
    // Index of element the model belongs to (zero based)
    uint8_t  element;
    // Model ID
    modelid_t  modelid;
    // Application Key Global Index
    uint16_t  appkey_global_index;
    // Msg Send param
    generic_client_params_t set_param;
} mesh_client_msg_param_t;

// Client Message Sent Callback
typedef  STATUS (* client_msg_sent_cb)(mesh_client_msg_param_t *p_param, STATUS status) ;

/**@}*/

/**
*@addtogroup PROVISION
*@{
*/

// Authentication data param structure
typedef struct {
    // Authentication Method used
    uint8_t method;
    // Size of the Output OOB used or size of the Input OOB used or 0x00
    uint8_t size;
    // Selected Output OOB Action or Input OOB Action or 0x00
    uint8_t action;
} prov_auth_data_set_param_t;

// prov Authentication data param structure
typedef union {
    prov_auth_data_set_param_t prov_auth_data;
} mesh_core_evt_set_params_t;

// Node Provision State indicate structure
typedef struct {
    // Node Provision State
    uint8_t state;
} prov_state_ind_param_t;

// Appkey Binding Indicate Parameter
typedef struct {
    // Indicate To Add(1) Or Del(0) Binding Info
    uint8_t bind;
    // Index of element the model belongs to (zero based)
    uint8_t element;
    // Application Key Global Index
    uint16_t appkey_global_index;
    // Model ID
    uint32_t modelid;
} appkey_binding_ind_param_t;

// Unprovison Device uuid
typedef struct {
    // Device uuid
    uint8_t uuid[MESH_UUID_LEN];
} prov_unprov_device_ind_t;

// Provision Provisioner state indicate structure
typedef struct {
    // Device uuid
    uint8_t  uuid[MESH_UUID_LEN];
    // Device key
    uint8_t  devkey[MESH_KEYS_LEN];
    // Netkey Index
    uint16_t netkey_index;
    // Address
    uint16_t addr;
    // Element number
    uint16_t elm_nb;
    // state
    uint8_t  state;
} prov_provisioner_state_ind_t;

// Provisioner Appkey Add Status structure
typedef struct {
    /// Srouce Address
    uint16_t src_addr;
    /// Appkey Index
    uint16_t appkey_index;
    /// Netkey Index
    uint16_t netkey_index;
    /// Status
    uint8_t status;
} provisioner_appkey_add_status_t;

// Provisioner Model application status structure
typedef struct {
    /// Element Address
    uint16_t element_addr;
    /// Appkey Index
    uint16_t appkey_index;
    /// Netkey Index
    uint16_t netkey_index;
    /// Model Id
    uint32_t model_id;
    /// Status
    uint8_t status;
} provisioner_model_app_status_t;

// Provisioner Subcribe Add Status structure
typedef struct {
    /// Status
    uint8_t status;
    /// Element Address
    uint16_t element_addr;
    /// Subcribe Address
    uint16_t sub_addr;
    /// Model ID
    uint32_t model_id;
} provisioner_subs_add_status_t;

// Provisioner Composition Data structure
typedef struct {
    // Source Addr
    uint16_t src_addr;
    // Data Length
    uint16_t length;
    // Value
    uint8_t val[__ARRAY_EMPTY];
} provisioner_composition_data_t;

// Procision Select Device Parameter structure
typedef struct {
    // Device uuid
    uint8_t uuid[MESH_UUID_LEN];
    // Indicate To Provision(1) Or Not(0)
    bool select;
} prov_select_device_param_t;

// supported provisioning capabilities indicate structure
typedef struct {
    // Device uuid
    uint8_t uuid[MESH_UUID_LEN];
    //    Algorithms
    uint16_t algorithms;
    // Supported public key types
    uint8_t  pub_key_type;
    // Supported static OOB Types
    uint8_t  static_oob_type;
    // Maximum size of Output OOB supported
    uint8_t  out_oob_size;
    // Supported Output OOB Actions
    uint16_t out_oob_action;
    // Maximum size in octets of Input OOB supported
    uint8_t  in_oob_size;
    // Supported Input OOB Actions
    uint16_t in_oob_action;
} provisioner_prov_cap_ind_t;

// supported provisioning capabilities confirm structure
typedef struct {
    // Device uuid
    uint8_t uuid[MESH_UUID_LEN];
    //    Algorithms
    uint8_t  algorithms;
    // Public Key used
    uint8_t  pub_key;
    // Authentication Method used
    uint8_t  auth_method;
    // Selected Output OOB Action or Input OOB Action or 0x00
    uint8_t  auth_action;
    // Size of the Output OOB used or size of the Input OOB used or 0x00
    uint8_t  auth_size;
} provisioner_prov_cap_cfm_t;

// Provisioner Provsion Auth Request structure
typedef struct {
    // Device uuid
    uint8_t uuid[MESH_UUID_LEN];
    ///
    uint8_t  auth_method;
    // expected authentication maximum data size
    uint8_t  auth_size;
    // Authentication Action:
    //   - MESH_PROV_AUTH_NO_OOB     = Prohibited
    //   - MESH_PROV_AUTH_STATIC_OOB = 16 bytes LSB static out of band data required
    //   - MESH_PROV_AUTH_OUTPUT_OOB
    //   - MESH_PROV_AUTH_INPUT_OOB
    uint16_t auth_action;
} provisioner_prov_auth_req_t;

// Provision Provision Auth Confirm structure
typedef struct {
    // Device uuid
    uint8_t uuid[MESH_UUID_LEN];
    // 1, Accept pairing request, 0 reject
    uint8_t  accept;
    // Authentication data size (<= requested size else pairing automatically rejected)
    uint8_t  auth_size;
    // Authentication data (LSB for a number or array of bytes)
    uint8_t  auth_data[MESH_PROVISION_AUTH_MAX_NUM];
} provisioner_prov_auth_cfm_t;

// Provisioner provisioning data parameter structure
typedef struct {
    // Device uuid
    uint8_t uuid[MESH_UUID_LEN];
    // Element num
    uint16_t elm_nb;
} provisioner_provisioning_data_param_t;

// Provisioner Publish Set status structure
typedef struct {
    /// Status
    uint8_t status;
    /// Element Address
    uint16_t element_addr;
    /// Publish Address
    uint16_t pub_addr;
    /// Publish Appkey Index
    uint16_t appkey_index : 12;
    /// Publish Friendship CredentialFlag
    uint16_t credentialFlag : 1;
    /// Reserved for Future Use
    uint16_t rfu : 3;
    /// Default TTL value for the outgoing messages
    uint8_t  ttl;
    /// Period for periodic status publishing
    uint8_t  period;
    /// Number of retransmissions for each published message
    uint8_t  retransmitCount : 3;
    /// Number of 50-millisecond steps between retransmissions
    uint8_t  retransmitIntervalSteps : 5;
    /// Model Identifier
    uint32_t model_id;
} provisioner_pub_set_status_t;

// Network Transmit Status structure
typedef struct {
    /// Source Address
    uint16_t src_addr;
    /// Network Transmit Count
    uint8_t  retransmitCount : 3;
    /// Network Transmit Interval Steps
    uint8_t  retransmitIntervalSteps : 5;
} provisioner_nettransmit_status_t;

// Mesh Core Provision Event Indicate Parameter union
typedef union {

    /// Unprovison Device uuid
    prov_unprov_device_ind_t        unprov_device_ind;
    /// Provision Provisioner state indicate structure
    prov_provisioner_state_ind_t    provision_state_ind;
    /// Provisioner Appkey Add Status structure
    provisioner_appkey_add_status_t appkey_add_status;
    /// Provisioner Model application status structure
    provisioner_model_app_status_t  model_app_status;
    /// Provisioner Composition Data structure
    provisioner_composition_data_t  composition_data;
    /// Provisioner Subcribe Add Status structure
    provisioner_subs_add_status_t   sub_add_status;
    /// supported provisioning capabilities indicate structure
    provisioner_prov_cap_ind_t      prov_cap;
    // Provisioner Provsion Auth Request structure
    provisioner_prov_auth_req_t     prov_auth;
    /// Provisioner provisioning data parameter structure
    provisioner_provisioning_data_param_t     prov_data;
    /// Provisioner Publish Set status structure
    provisioner_pub_set_status_t    pub_set_status;
    /// Network Transmit Status structure
    provisioner_nettransmit_status_t nettransmit_status;
} mesh_core_provisioner_evt_ind_params_t;

/**
* @brief   provisioner netkey struct
*/
typedef struct mesh_set_local_netkey {
    // opr
    bool addOrDel;
    // key
    uint8_t netkey[MESH_KEYS_LEN];
    // key_index
    uint16_t key_index;
} mesh_set_local_netkey_t;

/**
* @brief   provisioner appkey struct
*/
typedef struct mesh_set_local_appkey {
    // opr
    bool addOrDel;
    // key
    uint8_t appkey[MESH_KEYS_LEN];
    // key_index
    uint16_t key_index;
    uint16_t netkey_index;
} mesh_set_local_appkey_t;

/**
* @brief  Model App Bind message structure
*/
typedef struct mesh_set_local_model_app_bind {
    /// Element Address
    uint16_t element_addr;
    /// Model Identifier
    uint32_t model_id;
    /// Appkey Index
    uint16_t appkey_index;
} mesh_set_local_model_app_bind_t;

// Mesh Model Local Publication set structure
typedef struct mesh_local_pub_set {
    /// Element Address
    uint16_t element_addr;
    /// Value of the Publish Address
    uint16_t pub_addr;
    /// Index of the application key
    uint16_t appkey_index : 12;
    /// Publish Friendship CredentialFlag
    uint16_t credentialFlag : 1;
    /// Reserved for Future Use
    uint16_t rfu : 3;
    /// Default TTL value for the outgoing messages
    uint8_t  ttl;
    /// Period for periodic status publishing
    uint8_t  period;
    /// Number of retransmissions for each published message
    uint8_t  retransmitCount : 3;
    /// Number of 50-millisecond steps between retransmissions
    uint8_t  retransmitIntervalSteps : 5;
    /// Model Identifier
    uint32_t model_id;
} mesh_local_pub_set_t;

// Structure For Appkey Update Ind
typedef struct mesh_appkey_update_ind {
    // AppKey index
    uint16_t appkey_index;
    // AppKey
    uint8_t  key[MESH_KEYS_LEN];
    // Added (> 0) or deleted (= 0)
    uint8_t added;
} mesh_appkey_update_ind_t;

// Structure For Netkey Update Ind
typedef struct mesh_netkey_update_ind {
    // Netkey index
    uint16_t netkey_index;
    // Network Key
    uint8_t  key[MESH_KEYS_LEN];
    // Added (> 0) or deleted (= 0)
    uint8_t added;
} mesh_netkey_update_ind_t;

// Structure For Self Addr Ind
typedef struct mesh_self_addr_ind {
    // Addr
    uint16_t addr;
} mesh_self_addr_ind_t;

// Structure For get run time Ind
typedef struct mesh_get_run_time_ind {
    // Addr
    uint32_t clock_ms;
    uint16_t nb_wrap;

} mesh_get_run_time_ind_t;

// Structure For Dev Key Ind
typedef struct mesh_dev_key_ind {
    uint8_t key[MESH_KEYS_LEN];
} mesh_dev_key_ind_t;

// Structure For pulish addr  Ind
typedef struct mesh_publish_addr_ind {
    uint16_t element_addr;
    // Model ID
    uint32_t model_id;
    // Publication address
    uint16_t addr;
    // AppKey ID
    uint16_t appkey_index;
    // TTL
    uint8_t  ttl;
    // Period
    uint8_t  period;
    // Retransmission parameters
    uint8_t  retx_params;
    // Friend credentials
    uint8_t  friend_cred;
    // Label UUID
    uint8_t  label_uuid[MESH_UUID_LEN];
} mesh_publish_addr_ind_t;

// Structure For sub addr  Ind
typedef struct mesh_sub_addr_ind {
    uint16_t element_addr;
    // Sub address
    uint16_t addr;
    // Model ID
    uint32_t model_id;
    // Label UUID
    uint8_t  label_uuid[MESH_UUID_LEN];
    // Add (> 0) or deleted (= 0)
    uint8_t  addOrDel;
} mesh_sub_addr_ind_t;

// Structure For iv seq Ind
typedef struct mesh_iv_seq_ind {
    // IV Index
    uint32_t iv;
    // Seq NO
    uint32_t seq;
} mesh_iv_seq_ind_t;

// Structure For Friend Offer Ind
typedef struct mesh_friend_offer_ind {
    uint16_t friend_addr;
    uint8_t  rx_window;
    uint8_t  queue_size;
    uint8_t  subs_list_size;
    int8_t   rssi;
} mesh_friend_offer_ind_t;

// Structure For Friend Offer Ind
typedef struct mesh_lowper_status_ind {
    uint16_t friend_addr;
    uint16_t  status;
} mesh_lowper_status_ind_t;

// Structure For Friend Offer Ind
typedef struct mesh_cmp_status_ind {
    uint16_t status;
} mesh_cmp_status_ind_t;

// Mesh  Set Local Subcribtion
typedef struct mesh_set_local_subs {
    // Add (> 0) or deleted (= 0)
    uint8_t  addOrDel;
    /// Element Address
    uint16_t element_addr;
    /// Subcribe Address
    uint16_t sub_addr;
    /// Model Identifier
    uint32_t  model_id;
} mesh_set_local_subs_t;

// Mesh Provisioner Set Primary Addr
typedef struct mesh_set_local_prim_addr {
    uint16_t addr;
} mesh_set_local_prim_addr_t;

// Mesh  Set Local Dev key
typedef struct mesh_set_local_dev_key {
    uint8_t dev_key[MESH_KEYS_LEN];
} mesh_set_local_dev_key_t;

// Mesh  Set Local Iv Index And Seq NO
typedef struct mesh_set_local_iv_seq {
    uint32_t iv;
    uint32_t seq;
} mesh_set_local_iv_seq_t;

// Mesh  start lpn
typedef struct mesh_start_lpn_param {
    uint32_t poll_timeout_100ms;
    uint32_t poll_intv_ms;
    uint16_t prev_addr;
    uint8_t  rx_delay;
    uint8_t  rssi_factor;
    uint8_t  rx_window_factor;
    uint8_t  min_queue_size_log;
} mesh_start_lpn_param_t;

// Mesh  select friend
typedef struct mesh_select_friend_param {
    uint16_t addr;
} mesh_select_friend_param_t;

// Mesh  Set friend state
typedef struct mesh_set_friend_state {
    uint32_t state;
} mesh_set_friend_state_t;

// Mesh Core Event indicate parameters structure
typedef union {
    /// Authentication data param structure
    prov_auth_data_set_param_t prov_auth_data_param;
    /// Node Provision State indicate structure
    prov_state_ind_param_t prov_state_ind;
    // Appkey Binding Indicate Parameter
    appkey_binding_ind_param_t appkey_bind;
    // Structure For Appkey Update Ind
    mesh_appkey_update_ind_t   appkey_update_ind;
    // Structure For Netkey Update Ind
    mesh_netkey_update_ind_t   netkey_updata_ind;
    // Structure For Self Addr Ind
    mesh_self_addr_ind_t       self_addr_ind;
    /// structture for get run time
    mesh_get_run_time_ind_t    run_time_ind;
    /// structure for dev key
    mesh_dev_key_ind_t    dev_key;
    /// structure for iv index  and seq no
    mesh_iv_seq_ind_t iv_seq;
    /// structure for friend offer info
    mesh_friend_offer_ind_t friend_offer;
    /// structure for status ind
    mesh_cmp_status_ind_t status_ind;
    /// structure for lowpower node status ind
    mesh_lowper_status_ind_t lpn_status;
} mesh_core_evt_ind_params_t;

// Mesh Core Event callback parameter structure
typedef union {
    /// provisioner appkey struct
    mesh_set_local_appkey_t              local_appkey_set;
    /// provisioner netkey struct
    mesh_set_local_netkey_t              local_netkey_set;
    /// Model App Bind message structure
    mesh_set_local_model_app_bind_t      local_model_app_bind;
    /// Mesh Model Local Publication set structure
    mesh_local_pub_set_t                 local_pub_set;
    /// Mesh  Set Local Subcribtion
    mesh_set_local_subs_t    local_sub_set;
    /// Mesh  Set Local Primary addr
    mesh_set_local_prim_addr_t prim_addr;
    /// Mesh  Set Local Dev key
    mesh_set_local_dev_key_t   local_dev_key;
    /// Mesh  Set Local iv and seq
    mesh_set_local_iv_seq_t    local_iv_seq;
    /// Mesh  Start lpn
    mesh_start_lpn_param_t     lpn_param;
    /// Mesh  Select Friend
    mesh_select_friend_param_t  friend_param;
    /// structure for local friend state
    mesh_set_friend_state_t friend_state;
} mesh_core_evt_cb_params_t;

// Provision Get Composition Structure
typedef struct mesh_provisioner_get_composition {
    /// Destination Address
    uint16_t dst;
    /// Index of the Network key
    uint16_t netkey_index;
    /// Page number of the Composition Data
    uint8_t page;
} mesh_provisioner_get_composition_t;

// Provisioner Add Appkey Structure
typedef struct mesh_provisioner_add_appkey {
    /// Destination Address
    uint16_t dst;
    /// Index of the Network key
    uint16_t netkey_index;
    /// Index of the Application key
    uint16_t appkey_index;
    /// Application key
    uint8_t appkey[MESH_KEYS_LEN];
} mesh_provisioner_add_appkey_t;

// Provisioner Model App Bind Structure
typedef struct mesh_provisioner_mmodel_app {
    /// Destination Address
    uint16_t dst;
    /// Element Address
    uint16_t element_addr;
    /// Index of the Network key
    uint16_t netkey_index;
    /// Index of the Application key
    uint16_t appkey_index;
    /// Model identifier
    uint32_t model_id;
} mesh_provisioner_model_app_t;

// Provisioner Subcription Set Structure
typedef struct mesh_provisioner_subs_set {
    // Added (> 0) or deleted (= 0)
    uint8_t  addOrDel;
    /// Element Address
    uint16_t element_addr;
    /// Subcription Address
    uint16_t sub_addr;
    /// Index of Network key
    uint16_t netkey_index;
    /// Model identifier
    uint32_t model_id;
} mesh_provisioner_subs_set_t;

// Provisioner Node Delete Structure
typedef struct mesh_provisioner_node_delete {
    /// Node Address
    uint16_t addr;
} mesh_provisioner_node_delete_t;

// Provisioner Node Add Structure
typedef struct mesh_provisioner_node_add {
    /// Device key
    uint8_t  devkey[MESH_KEYS_LEN];
    /// Netkey Index
    uint16_t netkey_index;
    /// Address
    uint16_t addr;
    /// Element number
    uint16_t elm_nb;
} mesh_provisioner_node_add_t;

// Node Address Set Structure
typedef struct mesh_provisioner_addr_set {
    /// Node Address
    uint16_t addr;
} mesh_provisioner_addr_set_t;

// Mesh Provision Data confirm Structure
typedef struct  mesh_prov_data_cfm {
    uint8_t uuid[MESH_UUID_LEN];
    // Network Key
    uint8_t  net_key[MESH_KEYS_LEN];
    // Network Key Index
    uint16_t netkey_index;
    // Unicast Address of the primary element
    uint16_t unicast_addr;
} mesh_prov_data_cfm_t;

// Mesh Provisioner Set Pubication Parameter Structure
typedef struct mesh_provisioner_set_pub_param {
    /// Element Address
    uint16_t element_addr;
    /// Value of the publish address
    uint16_t pub_addr;
    /// Index of the application key
    uint16_t appkey_index : 12;
    /// Value of the Friendship Credential Flag
    uint16_t credentialFlag : 1;
    /// Reserved for Future Use
    uint16_t rfu : 3;
    /// Default TTL value for the outgoing messages
    uint8_t  ttl;
    /// Period for periodic status publishing
    uint8_t  period;
    /// Number of retransmissions for each published message
    uint8_t  retransmitCount : 3;
    /// Number of 50-millisecond steps between retransmissions
    uint8_t  retransmitIntervalSteps : 5;
    /// Model Identifier
    uint32_t model_id;
    /// Index of the Network key
    uint16_t netkey_index;
} mesh_provisioner_set_pub_param_t;

// Mesh Provisioner set nettransmit parameter Structure
typedef struct mesh_provisioner_set_nettransmit_param {
    /// Address of Element
    uint16_t element_addr;
    /// Index of Netkey
    uint16_t netkey_index;
    /// Number of transmissions for each Network PDU originating from the node
    uint8_t  retransmitCount : 3;
    /// Number of 10-millisecond steps between transmissions
    uint8_t  retransmitIntervalSteps : 5;
} mesh_provisioner_set_nettransmit_param_t;

// Mesh Core Provisioner Event Callback Parameter Union
typedef union {
    /// Procision Select Device Parameter structure
    prov_select_device_param_t         unprov_device_param;
    // supported provisioning capabilities confirm structure
    provisioner_prov_cap_cfm_t         prov_cap_cfm_param;
    // Provision Provision Auth Confirm structure
    provisioner_prov_auth_cfm_t        prov_auth_cfm_param;
    /// Mesh Provision Data confirm Structure
    mesh_prov_data_cfm_t               prov_data_cfm_param;
    /// Provision Get Composition Structure
    mesh_provisioner_get_composition_t get_composition_data;
    /// Provisioner Add Appkey Structure
    mesh_provisioner_add_appkey_t      add_appkey;
    /// Provisioner Model App Bind Structure
    mesh_provisioner_model_app_t       model_app_bind;
    /// Provisioner Subcription Set Structure
    mesh_provisioner_subs_set_t        subs_set;
    /// Provisioner Node Delete Structure
    mesh_provisioner_node_delete_t     node_delete;
    /// Provisioner Node Add Structure
    mesh_provisioner_node_add_t        node_add;
    /// Node Address Set Structure
    mesh_provisioner_addr_set_t        node_addr;
    /// Mesh Provisioner Set Pubication Parameter Structure
    mesh_provisioner_set_pub_param_t   pub_set;
    /// Mesh Provisioner set nettransmit parameter structure
    mesh_provisioner_set_nettransmit_param_t nettranssmit_set;
} mesh_core_provisioner_evt_cb_params_t;

/**@}*/

/**
 * @addtogroup PROFILE
 * @{
 */

// Mesh Core Event indicate Callback
typedef STATUS (*mesh_core_evt_ind_cb)(mesh_core_evt_ind_t evt, mesh_core_evt_ind_params_t *p_param);

// Server State indicate handler
typedef  void (* srv_state_ind_handle)(void *p_param) ;
/**@}*/

/**
 * @addtogroup PROVISION
 * @{
 */

// Mesh Core Provisioner Event indicate Callback
typedef STATUS (*mesh_core_provisioner_evt_ind_cb)(mesh_core_provisioner_evt_ind_t evt,
        mesh_core_provisioner_evt_ind_params_t *p_param);
/**@}*/

/**
 * @addtogroup MODELS
 * @{
 */

/**
* @brief   State entry structure
*/
typedef struct {
    // Indicate Node State Follow by type
    uint32_t state;
    // State Type (@see mesh_state_type_t)
    mesh_state_type_t type;
    //  Transition Time in milliseconds
    uint32_t trans_time_ms;
} mesh_state_ind_t;

// Model request to publish a new message
typedef struct mesh_model_publish_param {
    // - M_API_MODEL_PUBLISH: Model request to publish a new message
    // Operation code of the message
    uint32_t opcode;
    // Model ID
    uint32_t modelid;
    // Index of element the model belongs to (zero based)
    uint8_t element;
    // 1 = Segmented PDU force transport MIC to 64 bits ; 0 = 32 bits transport MIC
    uint8_t  trans_mic_64;
    // Message length
    uint16_t len;
    // Message content
    uint8_t  msg[__ARRAY_EMPTY];
} mesh_model_publish_param_t;

// Message Publishde callback
typedef  STATUS (* msg_published_cb)(mesh_model_publish_param_t *p_param, STATUS status) ;

// Sig Nodel State callback
typedef  void (* sig_model_state_cb)(mesh_state_ind_t *p_state) ;
/**@}*/

/**
 * @addtogroup PROFILE
 * @{
 */

// Mesh Start Complete fuc
typedef  void (* mesh_start_complete)(void) ;

// Mesh Pause Scan Complete fuc
typedef  void (* mesh_scan_pasuse_cb)(void) ;

// Mesh Resume Scan Complete fuc
typedef  void (* mesh_scan_resume_cb)(void) ;

/*
 * FUNCTION DECLARATIONS
**********************************************************
*/

/**
 ***********************************************************
 * @brief Func  start mesh protocol
 *
 * @param[in]  cb    started callback @see mesh_start_complete
 *
 ************************************************************
*/
void sonata_mesh_start(mesh_start_complete cb);
/**@}*/

/**
 * @addtogroup MODELS
 * @{
 */

/**
 ***********************************************************
 * @brief Func  publish msg
 *
 * @param[in]  param    @see mesh_model_publish_param_t
 * @param[in]  end_cb   @see msg_published_cb
 * @return STATUS
 * @retval MESH_ERROR_INVALID_PARAM  The parameter to publish is invalid.
 * @retval MESH_ERROR_NO_SPACE       No space to use
 * @retval MESH_ERROR_NO_ERROR       No error/Success
 ************************************************************
*/
STATUS  mesh_msg_publish(mesh_model_publish_param_t *param, msg_published_cb end_cb);

/**
 ***********************************************************
 * @brief Func  used for client model  to send msg
 *
 * @param[in]  param   message param   @see mesh_client_msg_param_t
 * @param[in]  end_cb  callback func after msg be sent    @see client_msg_sent_cb
 * @return STATUS
 * @retval MESH_ERROR_NO_SPACE       No space to use
 * @retval MESH_ERROR_NO_ERROR       No error/Success
 ************************************************************
*/
STATUS sonata_mesh_client_send_msg(mesh_client_msg_param_t *param, client_msg_sent_cb end_cb);

/**
 ***********************************************************
 * @brief Func   used for server model  to send msg
 *
 * @param[in]  param   message param   @see mesh_model_msg_param_t
 * @param[in]  sent_cb    callback func after msg be sent    @see model_msg_sent_cb
 * @return STATUS
 * @retval MESH_ERROR_NO_SPACE       No space to use
 * @retval MESH_ERROR_NO_ERROR       No error/Success
 ************************************************************
*/
STATUS  sonata_mesh_msg_send(mesh_model_msg_param_t *param, model_msg_sent_cb sent_cb);

/**
 ***********************************************************
 * @brief  Register SIG model
 *
 * @param[in]  modelid    SIG model ID
 * @param[in]  element    which element to reg
 * @param[in]  is_main    if it is a main service
 * @param[in]  cb         model state handle, callback by stack after recieve pkt @see sig_model_state_cb
 * @return Status of model register
 * @retval MESH_ERROR_NOT_READY      Not ready to register
 * @retval MESH_ERROR_NO_SPACE       No space to use
 * @retval MESH_ERROR_NO_ERROR       No error/Success
 ************************************************************
*/
STATUS sonata_mesh_model_register(uint32_t modelid, uint8_t element, bool is_main, sig_model_state_cb cb);

/**
 ***********************************************************
 * @brief Func  register vendor model
 *
 * @param[in]  modelid    vendor model ID
 * @param[in]  element    which element to reg
 * @param[in]  pkt_cb     model state handle, callback by stack after recieve pkt @see m_vendor_model_msg_rx_t
 * @return STATUS
 * @retval MESH_ERROR_NOT_READY      Not ready to register
 * @retval MESH_ERROR_NO_SPACE       No space to use,Vendor model toolbox is full
 * @retval MESH_ERROR_NO_ERROR       No error/Success
 ************************************************************
*/
STATUS mesh_vendor_model_register(uint32_t modelid, uint8_t element, m_vendor_model_msg_rx_t pkt_cb);

/**
 ***********************************************************
 * @brief Func  bind appkey of command
 *
 * @param[in]   element       which element to reg.
 * @param[in]   model_id      model id.
 * @param[in]   app_key_id    id of app key.
 *
 * @retval 0
 ************************************************************
*/
uint8_t sonata_mesh_app_key_bind(uint32_t element, uint32_t model_id, uint16_t app_key_id);

/**
 ***********************************************************
 * @brief Func  subscribed to a group addr
 *
 * @param[in]   element       which element to reg.
 * @param[in]   model_id      model id.
 * @param[in]   group_addr    group addr.
 *
 * @retval 0
 ************************************************************
*/
uint8_t sonata_mesh_subs_group_addr(uint32_t element, uint32_t model_id, uint16_t group_addr);

/**
 ***********************************************************
 * @brief Func  Set value of a local state
 *
 * @param[in]   state       State value.
 * @param[in]   state_id    State identifier.
 * @param[in]   mdl_lid     Model local index.
 *
 * @return None
 ************************************************************
*/

void m_api_srv_set(uint32_t state, modelid_t modelid);
/**@}*/

/**
 * @addtogroup PROFILE
 * @{
 */

/**
 ***********************************************************
 * @brief Func  config mesh param
 *
 * @param[in]  type     param type    @see mesh_param_opr_types_t
 * @param[in]  p_param   param struct  @see mesh_set_params_t
 * @return STATUS
 * @retval MESH_ERROR_NO_ERROR       No error/Success
 ************************************************************
*/
STATUS sonata_mesh_param_set(mesh_param_opr_types_t type, mesh_set_params_t *p_param);

/**
 ***********************************************************
 * @brief Func  get mesh setting info
 *
 * @param[in]  type     param type     @see mesh_param_opr_types_t
 * @param[in]  p_param   param struct  @see mesh_get_params_t
 * @return STATUS
 * @retval MESH_ERROR_NO_ERROR       No error/Success
 ************************************************************
*/
STATUS sonata_mesh_param_get(mesh_param_opr_types_t type, mesh_get_params_t *p_param);

/**
 ***********************************************************
 * @brief Func  notify mesh stack to clean data
 *
 * @param[in]  clean_cb  callback after clean finished
 *
 * @return STATUS
 * @retval MESH_ERROR_NO_ERROR       No error/Success
 ************************************************************
*/
STATUS sonata_mesh_clean_data(void (*clean_cb)(void));

/**
 ***********************************************************
 * @brief Func  save data immedimately
 *
 * @return STATUS
 * @retval MESH_ERROR_NO_ERROR       No error/Success
 ************************************************************
*/
STATUS sonata_mesh_save_data(void);

/**
 ***********************************************************
 * @brief Func  set interval of saving data
 *
 * @param[in]  secs   time of unit of sec
 *
 * @return STATUS
 * @retval MESH_ERROR_NO_ERROR       No error/Success
 ************************************************************
*/
STATUS sonata_mesh_set_save_interval(uint32_t secs);

/**
 ***********************************************************
 * @brief Func  to broadcast proxy service
 *
 * @param[in]  ctl  action @see mesh_proxy_adv_ctl_t
 *
 ************************************************************
*/
void sonata_mesh_proxy_service_broadcast(mesh_proxy_broadcast_ctl_t ctl);

/**
 ***********************************************************
 * @brief Func  register callback for event handle
 *
 * @param[in]  cb   callback list   @see mesh_core_evt_ind_cb
 *
 * @retval MESH_ERROR_NO_ERROR       No error/Success
 ************************************************************
*/
void sonata_mesh_register_core_evt_ind(mesh_core_evt_ind_cb cb);

/**
 ***********************************************************
 * @brief Func  app notify the provisioner to finish something
 *
 * @param[in]  evt       @see mesh_core_provisioner_evt_cb_t
 * @param[in]  p_param   @see mesh_core_provisioner_evt_cb_params_t
 *
 * @retval MESH_ERROR_NO_ERROR       No error/Success
 ************************************************************
*/
int mesh_core_provisioner_evt_cb(mesh_core_provisioner_evt_cb_t evt, mesh_core_provisioner_evt_cb_params_t *p_param);

/**
 ***********************************************************
 * @brief Func  app notify the stack to finish something
 *
 * @param[in]  evt       @see mesh_core_evt_cb_t
 * @param[in]  p_param   @see mesh_core_evt_cb_params_t
 *
 * @retval MESH_ERROR_NO_ERROR         No error/Success
 ************************************************************
*/
int mesh_core_evt_cb(mesh_core_evt_cb_t evt, mesh_core_evt_cb_params_t *p_param);
/**@}*/

/**
 * @addtogroup PROVISION
 * @{
 */

/**
 ***********************************************************
 * @brief Func  register evt indication for provisioner event
 *
 * @param[in]  cb   callback list   @see mesh_core_provisioner_evt_ind_cb
 *
 ************************************************************
*/
void sonata_mesh_provisioner_register_core_evt_ind(mesh_core_provisioner_evt_ind_cb cb);

/**
 ***********************************************************
 * @brief Func  judge the node is provisioned
 *
 * @retval TRUE  Provisioned
 * @retval FALSE Unprovisioned
 ************************************************************
*/
bool sonata_mesh_is_provisioned(void);

/**
 **********************************************************
 * @brief Func  response to provision authentication data request.
 *
 * @param[in]   accept       accept or not.
 * @param[in]   auth_size    length of authentication data
 * @param[in]   auth_data    pointer to authentication data memory
 *
 * @return  None.
 ************************************************************
 */
void sonata_mesh_send_prov_auth_data_rsp(uint8_t accept, uint8_t auth_size, uint8_t *auth_data);
/**@}*/

/**
 * @addtogroup PROFILE
 * @{
 */

/**
 ***********************************************************
 * @brief Func  random num generator word
 *
 * @return Random word value
 ************************************************************
*/
uint32_t sonata_mesh_rand_word(void);

/**
 ***********************************************************
 * @brief Func  random num generator for uint16_t
 *
 * @return Random half word value
 ************************************************************
*/
uint16_t sonata_mesh_rand_hword(void);

/**
 ***********************************************************
 * @brief Func  get src addr of Client
 *
 * @return Source Address
 ************************************************************
*/
uint16_t sonata_mesh_get_src_addr(void);
/**
 ****************************************************************************************
 * @brief Get device run time
 *
 ****************************************************************************************
 */
void app_mesh_api_get_run_time(void);

/**
 ***********************************************************
 * @brief Func  pause mesh scan
 *
 * @param[in]  module      module id defined @see mesh_scan_opr_module_id
 * @param[in]  cb          mesh scan pause complete callback
 *
 * @return void pause mesh scan
 ************************************************************
*/
void sonata_mesh_scan_pause(mesh_scan_opr_module_id_t module, mesh_scan_pasuse_cb cb);

/**
 ***********************************************************
 * @brief Func  resume mesh scan
 *
 * @param[in]  module      module id defined @see mesh_scan_opr_module_id
 * @param[in]  cb          mesh scan resume complete callback
 *
 * @return void
 ************************************************************
*/
void sonata_mesh_scan_resume(mesh_scan_opr_module_id_t module, mesh_scan_resume_cb cb);

/**
 ***********************************************************
 * @brief Func  get netkey after system is running
 * @param[in]  key_index        netkey global index
 * @param[in]  key          key storage
 *
 * @retval MESH_ERROR_NO_ERROR         No Error/Success
 * @retval MESH_ERROR_INVALID_PARAM    Not Found/Fail
 ************************************************************
*/
STATUS sonata_mesh_get_netkey(uint16_t key_index, uint8_t *key);

/**
 ***********************************************************
 * @brief Func  get appkey after system is running
 * @param[in]  key_index        appkey global index
 * @param[in]  key          key storage
 *
 * @retval MESH_ERROR_NO_ERROR         No error/Success
 * @retval MESH_ERROR_INVALID_PARAM    Not Found/Fail
 ************************************************************
*/
STATUS sonata_mesh_get_appkey(uint16_t key_index, uint8_t *key);

/**
 ***********************************************************
 * @brief Func  get node first addr
 *
 * @
 *
 * @retval uint16_t         addr

 ************************************************************
*/
uint16_t  sonata_mesh_get_prim_addr(void);

/**
 ***********************************************************
 * @brief Func  get devkey after system is running
 * @param[in]  key          key storage
 *
 * @retval void
 ************************************************************
*/
void sonata_mesh_get_dev_key(uint8_t *key);

/**
 ***********************************************************
 * @brief Func  get iv index and seq no
 * @param[in]  iv          porter to iv index
 * @param[in]  seq         porter to seq no
 *
 * @retval void
 ************************************************************
*/
void sonata_mesh_get_iv_seq(uint32_t *iv, uint32_t *seq);

/**
 ***********************************************************
 * @brief Func  get subscribe list size
 * @param[in]  unicast_addr          element addr
 * @param[in]  model_id         model ID
 *
 * @retval uint16_t           size
 ************************************************************
*/
uint16_t sonata_mesh_get_subs_size(uint16_t unicast_addr, uint32_t model_id);

/**
 ***********************************************************
 * @brief Func  start adv unprov beacon
 *
 *
 *
 * @retval void
 ************************************************************
*/
void sonata_mesh_start_unprov_beacon(void);

/**
 ***********************************************************
 * @brief Func  stop adv unprov beacon
 *
 *
 *
 * @retval void
 ************************************************************
*/
void sonata_mesh_stop_unprov_beacon(void);

/** @}*/

/** @}*/
#endif // _SONATA_MESH_API_H_

