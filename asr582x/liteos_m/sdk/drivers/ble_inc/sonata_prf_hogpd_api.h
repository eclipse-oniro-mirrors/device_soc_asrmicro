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
 * @file sonata_prf_hogpd_api.h
 *
 * @brief header file - HOGPD service server api
 *
 ****************************************************************************************
 */

#ifndef _SONATA_PRF_HOGPD_API_H_
#define _SONATA_PRF_HOGPD_API_H_

/*
 * INCLUDE FILES
 ****************************************************************************************
 */
/// @cond
#if BLE_HOST_PRESENT
/// @endcond
#include "sonata_utils_api.h"
#include "gapc_task.h"
#include "sonata_api_task.h"
#include "sonata_multi_keypad.h"

/*
 * MACRO DEFINITIONS
 ****************************************************************************************
 */

#define PRF_HOGPD_FEATURES                   (0xFF)
#define HID_NB                               (1)
#define HID_REPORT_INST_NB                   (4)
/// Maximal length of the Device Name value
#define APP_DEVICE_NAME_MAX_LEN              (18)
#define MOUSE_TIMEOUT_TIMER                  (100)
#define DEMO_MOUSE                           (0)
#define DEMO_KEYBOARD                        (1)

/// Maximal number of HIDS that can be added in the DB
#define SONATA_HOGPD_NB_HIDS_INST_MAX              (2)
/// Maximal number of Report Char. that can be added in the DB for one HIDS - Up to 11
#define SONATA_HOGPD_NB_REPORT_INST_MAX            (5)

/// Length of the HID Keyboard vendor-defined Report
#define SONATA_HOGP_VENDOR_DEFINED_REPORT_LEN  (19)

/// Keyboard Row length
#define SONATA_HOGP_KEYBOARD_ARRAY_ROW_LEN    (8)
/// Keyboard Column length
#define SONATA_HOGP_KEYBOARD_ARRAY_COLUMN_LEN (18)

#define HID_REPORT_ID_KEYBOARD_INDEX          (0)
#define HID_REPORT_ID_LED_INDEX               (1)
#define HID_REPORT_ID_CONSUMER_CTRL_INDEX     (2)
#define HID_REPORT_ID_SERVICE_CTRL_INDEX      (3)
#define HID_REPORT_ID_VENDOR_DEFINED_INDEX    (4)
#define HID_REPORT_ID_MOUSE_INDEX             (1)

#define HID_REPORT_ID_KEYBOARD                (1)
#define HID_REPORT_ID_MOUSE                   (2)
#define HID_REPORT_ID_CONSUMER_CTRL           (3)
#define HID_REPORT_ID_SERVICE_CTRL            (4)
#define HID_REPORT_ID_VENDOR_DEFINED          (17)
#define HID_REPORT_ID_VOICE                   (3)
#define HID_REPORT_ID_VOICE_INDEX             (2)
#define SONATA_ATT_ERR_INSUFF_AUTHEN          (0x05)
/*
 * Keyboard Keycode
 */
#define    HID_BUTTON_A            0x04      // Keyboard A and a
#define    HID_BUTTON_B            0x05      // Keyboard B and b
#define    HID_BUTTON_C            0x06      // Keyboard C and c
#define    HID_BUTTON_D            0x07      // Keyboard D and d
#define    HID_BUTTON_E            0x08      // Keyboard E and e
#define    HID_BUTTON_F            0x09      // Keyboard F and f
#define    HID_BUTTON_G            0x0A      // Keyboard G and g
#define    HID_BUTTON_H            0x0B      // Keyboard H and h
#define    HID_BUTTON_I            0x0C      // Keyboard I and i
#define    HID_BUTTON_J            0x0D      // Keyboard J and J
#define    HID_BUTTON_K            0x0E      // Keyboard K and k
#define    HID_BUTTON_L            0x0F      // Keyboard L and l
#define    HID_BUTTON_M            0x10      // Keyboard M and m
#define    HID_BUTTON_N            0x11      // Keyboard N and n
#define    HID_BUTTON_O            0x12      // Keyboard O and o
#define    HID_BUTTON_P            0x13      // Keyboard P and p
#define    HID_BUTTON_Q            0x14      // Keyboard Q and q
#define    HID_BUTTON_R            0x15      // Keyboard R and r
#define    HID_BUTTON_S            0x16      // Keyboard S and s
#define    HID_BUTTON_T            0x17      // Keyboard T and t
#define    HID_BUTTON_U            0x18      // Keyboard U and u
#define    HID_BUTTON_V            0x19      // Keyboard V and v
#define    HID_BUTTON_W            0x1A      // Keyboard W and w
#define    HID_BUTTON_X            0x1B      // Keyboard X and x
#define    HID_BUTTON_Y            0x1C      // Keyboard Y and y
#define    HID_BUTTON_Z            0x1D      // Keyboard Z and z
#define    HID_BUTTON_1            0x1E      // Keyboard 1 and !
#define    HID_BUTTON_2            0x1F      // Keyboard 2 and @
#define    HID_BUTTON_3            0x20      // Keyboard 3 and #
#define    HID_BUTTON_4            0x21      // Keyboard 4 and $
#define    HID_BUTTON_5            0x22      // Keyboard 5 and %
#define    HID_BUTTON_6            0x23      // Keyboard 6 and ^
#define    HID_BUTTON_7            0x24      // Keyboard 7 and &
#define    HID_BUTTON_8            0x25      // Keyboard 8 and *
#define    HID_BUTTON_9            0x26      // Keyboard 9 and (
#define    HID_BUTTON_0            0x27      // Keyboard 0 and)
#define    HID_BUTTON_ENTER        0x28      // Keyboard Return and Enter
#define    HID_BUTTON_ESC          0x29      // Keyboard ESPACE
#define    HID_BUTTON_BACKSPACE    0x2A      // Keyboard DELET (Backspace)
#define    HID_BUTTON_TAB          0x2B      // Keyboard Tab
#define    HID_BUTTON_SPACEBAR     0x2C      // Keyboard Spacebar
#define    HID_BUTTON_MINUS        0x2D      // Keyboard - and underscore
#define    HID_BUTTON_QEQUAL       0x2E      // Keyboard = and +
#define    HID_BUTTON_LEFT_BRKT    0x2F      // Keyboard { and [
#define    HID_BUTTON_RIGHT_BRKT   0x30      // Keyboard } and ]
#define    HID_BUTTON_BACK_SLASH   0x31      // Keyboard \ and |
#define    HID_BUTTON_K42          0x32      // Keyboard K42
#define    HID_BUTTON_SEMI_COLON   0x33      // Keyboard ; and :
#define    HID_BUTTON_SGL_QUOTE    0x34      // Keyboard ' and "
#define    HID_BUTTON_GRV_ACCENT   0x35      // Keyboard Grave Accent and Tilde
#define    HID_BUTTON_COMMA        0x36      // Keyboard , and <
#define    HID_BUTTON_DOT          0x37      // Keyboard . and >
#define    HID_BUTTON_FWD_SLASH    0x38      // Keyboard / and ?
#define    HID_BUTTON_CAP          0x39      // Keyboard Caps Lock
#define    HID_BUTTON_F1           0x3A      // Keyboard F1
#define    HID_BUTTON_F2           0x3B      // Keyboard F2
#define    HID_BUTTON_F3           0x3C      // Keyboard F3
#define    HID_BUTTON_F4           0x3D      // Keyboard F4
#define    HID_BUTTON_F5           0x3E      // Keyboard F5
#define    HID_BUTTON_F6           0x3F      // Keyboard F6
#define    HID_BUTTON_F7           0x40      // Keyboard F7
#define    HID_BUTTON_F8           0x41      // Keyboard F8
#define    HID_BUTTON_F9           0x42      // Keyboard F9
#define    HID_BUTTON_F10          0x43      // Keyboard F10
#define    HID_BUTTON_F11          0x44      // Keyboard F11
#define    HID_BUTTON_F12          0x45      // Keyboard F12
#define    HID_BUTTON_PRNT_SCREEN  0x46      // Keyboard Print Screen
#define    HID_BUTTON_SCROLL_LOCK  0x47      // Keyboard Scroll Lock
#define    HID_BUTTON_PAUSE        0x48      // Keyboard Pause Break
#define    HID_BUTTON_INSERT       0x49      // Keyboard Insert
#define    HID_BUTTON_HOME         0x4A      // Keyboard Home
#define    HID_BUTTON_PAGE_UP      0x4B      // Keyboard Page Up
#define    HID_BUTTON_DELETE_FWD   0x4C      // Keyboard Delete Forward
#define    HID_BUTTON_END          0x4D      // Keyboard End
#define    HID_BUTTON_PAGE_DOWN    0x4E      // Keyboard Page Down
#define    HID_BUTTON_RIGHT_ARROW  0x4F      // Keyboard Right Arrow
#define    HID_BUTTON_LEFT_ARROW   0x50      // Keyboard Left Arrow
#define    HID_BUTTON_DOWN_ARROW   0x51      // Keyboard Down Arrow
#define    HID_BUTTON_UP_ARROW     0x52      // Keyboard Up Arrow
#define    HID_BUTTON_NUM_LOCK     0x53      // Keyboard Num Lock and Clear
#define    HID_BUTTON_KP_DEVIDE    0x54      // Keyboard /
#define    HID_BUTTON_KP_MULTIPLY  0x55      // Keyboard *
#define    HID_BUTTON_KP_SUBTRACT  0x56      // Keyboard -
#define    HID_BUTTON_KP_ADD       0x57      // Keyboard +
#define    HID_BUTTON_KP_ENTER     0x58      // Keyboard ENTER
#define    HID_BUTTON_KP_1         0x59      // Keyboard 1 and End
#define    HID_BUTTON_KP_2         0x5A      // Keyboard 2 and Down Arrow
#define    HID_BUTTON_KP_3         0x5B      // Keyboard 3 and PgDn
#define    HID_BUTTON_KP_4         0x5C      // Keyboard 4 and Left Arrow
#define    HID_BUTTON_KP_5         0x5D      // Keyboard 5
#define    HID_BUTTON_KP_6         0x5E      // Keyboard 6 and Right Arrow
#define    HID_BUTTON_KP_7         0x5F      // Keyboard 7 and Home
#define    HID_BUTTON_KP_8         0x60      // Keyboard 8 and Up Arrow
#define    HID_BUTTON_KP_9         0x61      // Keyboard 9 and PgUp
#define    HID_BUTTON_KP_0         0x62      // Keyboard 0 and Insert
#define    HID_BUTTON_KP_DOT       0x63      // Keyboard . and Delete
#define    HID_BUTTON_K45          0x64      // Keyboard K45
#define    HID_BUTTON_APP          0x65      // Keyboard app
#define    HID_BUTTON_K107         0x85      // Keyboard K107
#define    HID_BUTTON_K56          0x87      // Keyboard K56
#define    HID_BUTTON_K151         0x90      // Keyboard K151
#define    HID_BUTTON_K150         0x91      // Keyboard K150
#define    HID_BUTTON_CTRL_L       0xE0      // Keyboard CTRL_L
#define    HID_BUTTON_SHIFT_L      0xE1      // Keyboard Shift_l
#define    HID_BUTTON_ALT_L        0xE2      // Keyboard ALT-L
#define    HID_BUTTON_WIN_L        0xE3      // Win-L
#define    HID_BUTTON_CTRL_R       0xE4      // Keyboard CTRL_R
#define    HID_BUTTON_SHIFT_R      0xE5      // Keyboard Shift_R
#define    HID_BUTTON_ALT_R        0xE6      // Keyboard ALT_R
#define    HID_BUTTON_WIN_R        0xE7      // WIN-R
#define    HID_BUTTON_FN           0xFF      // Keyboard Fn
/*
 * Consumer Keycode
 */
#define    HID_CONSUMER_SLEEP          0xA0  // Keyboard sleep
#define    HID_CONSUMER_POWER          0xA1  // Keyboard power
#define    HID_CONSUMER_WEB_HOME       0xA4  // Web Home
#define    HID_CONSUMER_WWW_FORWARD    0xA6  // www Forward
#define    HID_CONSUMER_WWW_STOP       0xA7  // WWW STOP
#define    HID_CONSUMER_WWW_REFRESH    0xA8  // WWW Refresh
#define    HID_CONSUMER_WWW_FAVORITES  0xA9  // WWW Favorites
#define    HID_CONSUMER_MEDIA          0xAA  // Media
#define    HID_CONSUMER_MAIL           0xAB  // Mail
#define    HID_CONSUMER_CALCULATOR     0xAC  // Calculate
#define    HID_CONSUMER_MY_COMPUTER    0xAD  // My Computor
#define    HID_CONSUMER_STOP           0xB0  // Stop
#define    HID_CONSUMER_SCAN_NEXT_TRK  0xB5  // Scan Next Track
#define    HID_CONSUMER_SCAN_PREV_TRK  0xB6  // Scan Previous Track
#define    HID_CONSUMER_PLAY_PAUSE     0xCD  // Play/Pause
#define    HID_CONSUMER_MUTE           0xE2  // Mute
#define    HID_CONSUMER_VOLUME_UP      0xE9  // Volume Increment
#define    HID_CONSUMER_VOLUME_DOWN    0xEA  // Volume Decrement
#define    HID_CONSUMER_WWW_SEARCH     ((0x02 << 8 & 0xFF) | 0x21) // WWW Search 0x0221
#define    HID_CONSUMER_HOMEPAGE       ((0x02 << 8 & 0xFF) | 0x23) // Homepage 0x0223
#define    HID_CONSUMER_WWW_BACK       ((0x02 << 8 & 0xFF) | 0x24) // WWW Back 0x0224

typedef enum {
    // HID Information
    SONATA_HOGPD_HID_INFORMATION_CHAR,
    // HID Control Point
    SONATA_HOGPD_HID_CONTROL_POINT_CHAR,
    // HID Report Map
    SONATA_HOGPD_HID_REPORT_MAP_CHAR,
    // Boot Mouse Input Report
    SONATA_HOGPD_HID_MOUSE_INPUT_REPORT_CHAR,
    // Report
    SONATA_HOGPD_HID_REPORT_CHAR,
} sonata_hogpd_info;

/*!
 * @brief
 */
typedef enum {
    /// Peripheral Bonded
    SONATA_TAG_PERIPH_BONDED              = (APP_DATA_SAVE_TAG_FIRST + 0),
    /// Mouse NTF Cfg
    SONATA_TAG_MOUSE_NTF_CFG,
    /// Mouse Timeout value
    SONATA_TAG_MOUSE_TIMEOUT,
    /// Peer Device BD Address
    SONATA_TAG_PEER_BD_ADDRESS,
    /// Peer 2.4G BD Address
    SONATA_TAG_PEER_2_4G_BD_ADDRESS,
    /// EDIV (2bytes), RAND NB (8bytes),  LTK (16 bytes), Key Size (1 byte)
    SONATA_TAG_LTK,
    /// app_ltk_key_in
    SONATA_TAG_LTK_IN,
    /// app irk addr
    SONATA_TAG_IRK_ADDR,
    /// app irk
    SONATA_TAG_IRK,
    /// device address
    SONATA_TAG_BD_ADDRESS,
    /// bonded dev info
    SONATA_TAG_BONDED_DEV_INFO,
    /// start pair on boot
    SONATA_TAG_PAIR_ON_BOOT,
} sonata_app_nvds_tag;

/*!
 * @brief
 */
typedef enum {
    /// Peripheral Bonded len
    SONATA_LEN_PERIPH_BONDED              = 1,
    /// Mouse NTF Cfg len
    SONATA_LEN_MOUSE_NTF_CFG              = 2,
    /// Mouse Timeout value len
    SONATA_LEN_MOUSE_TIMEOUT              = 2,
    /// Peer Device BD Address len
    SONATA_LEN_PEER_BD_ADDRESS            = 6,
    /// Peer 2.4G BD Address len
    SONATA_LEN_PEER_2_4G_BD_ADDRESS       = 6,
    /// EDIV (2bytes), RAND NB (8bytes),  LTK (16 bytes), Key Size (1 byte)
    SONATA_LEN_LTK                        = 27,
    /// app_ltk_key_in len
    SONATA_LEN_LTK_IN                     = 16,
    /// app irk addr len
    SONATA_LEN_IRK_ADDR                   = 6,
    /// app irk len
    SONATA_LEN_IRK                        = 16,
    /// device address
    SONATA_LEN_BD_ADDRESS                 = 6,
    /// bonded dev info len
    SONATA_LEN_BONDED_DEV_INFO            = 218, // 218: 3, 290:4,
    /// start pair on boot
    SONATA_LEN_PAIR_ON_BOOT               = 1,
} sonata_app_nvds_len;

/// Features Flag Values
typedef enum {
    /// Keyboard Device
    SONATA_HOGPD_CFG_KEYBOARD      = 0x01,
    /// Mouse Device
    SONATA_HOGPD_CFG_MOUSE         = 0x02,
    /// Protocol Mode present
    SONATA_HOGPD_CFG_PROTO_MODE    = 0x04,
    /// Extended Reference Present
    SONATA_HOGPD_CFG_MAP_EXT_REF   = 0x08,
    /// Boot Keyboard Report write capability
    SONATA_HOGPD_CFG_BOOT_KB_WR    = 0x10,
    /// Boot Mouse Report write capability
    SONATA_HOGPD_CFG_BOOT_MOUSE_WR = 0x20,

    /// Valid Feature mask
    SONATA_HOGPD_CFG_MASK          = 0x3F,

    /// Report Notification Enabled (to be shift for each report index)
    SONATA_HOGPD_CFG_REPORT_NTF_EN = 0x40,
} sonata_hogpd_cfg;

/*!
 * @brief
 */
typedef enum {
    // up
    MOTION_UP = 'U',
    // down
    MOTION_DOWN = 'D',
    // left
    MOTION_LEFT = 'L',
    // right
    MOTION_RIGHT = 'R',
    // diagonal
    MOTION_DIAGONAL = 'I',
    //wheel
    MOTION_WHEEL = 'W',
    // button
    BUTTON_EVENT = 'B',
    // multiple motion
    MOTION_MULTIPLE = 'M',
    // ADC
    MOTION_ADC = 'A',
} sonata_mouse_motion;

/// Report Char. Configuration Flag Values   // Report's read and write property
typedef enum {
    /// Input Report
    SONATA_HOGPD_CFG_REPORT_IN     = 0x01,
    /// Output Report , Report will be Writeable
    SONATA_HOGPD_CFG_REPORT_OUT    = 0x02,
    // HOGPD_CFG_REPORT_FEAT can be used as a mask to check Report type
    /// Feature Report
    SONATA_HOGPD_CFG_REPORT_FEAT   = 0x03,
    /// Input report with Write capabilities
    SONATA_HOGPD_CFG_REPORT_WR     = 0x10,
} sonata_hogpd_report_cfg;

/// Type of reports
typedef enum {
    /// The Report characteristic is used to exchange data between a HID Device and a HID Host.
    SONATA_HOGPD_REPORT,
    /// The Report Map characteristic
    SONATA_HOGPD_REPORT_MAP,
    /// Boot Keyboard Input Report
    SONATA_HOGPD_BOOT_KEYBOARD_INPUT_REPORT,
    /// Boot Keyboard Output Report
    SONATA_HOGPD_BOOT_KEYBOARD_OUTPUT_REPORT,
    /// Boot Mouse Input Report
    SONATA_HOGPD_BOOT_MOUSE_INPUT_REPORT,
} sonata_hogpd_report_type;

/// type of operation requested by peer device
typedef enum {
    /// No operation
    SONATA_HOGPD_OP_NO,
    /// Read report value
    SONATA_HOGPD_OP_REPORT_READ,
    /// Modify/Set report value
    SONATA_HOGPD_OP_REPORT_WRITE,
    /// Modify Protocol mode
    SONATA_HOGPD_OP_PROT_UPDATE,
} sonata_hogpd_op;

/// Messages for HID Over GATT Profile Device Role
/* @TRACE */
enum sonata_hogpd_msg_id {
    /// Restore bond data the HID Over GATT Profile Device Role Task
    SONATA_HOGPD_ENABLE_REQ = STACK_TASK_FIRST_MSG(SONATA_TASK_ID_HOGPD),
    /// Response of enabled request
    SONATA_HOGPD_ENABLE_RSP,

    /// Request sending of a report to the host - notification
    SONATA_HOGPD_REPORT_UPD_REQ,
    /// Response sending of a report to the host
    SONATA_HOGPD_REPORT_UPD_RSP,

    /// Request from peer device to Read or update a report value
    SONATA_HOGPD_REPORT_REQ_IND,
    /// Confirmation for peer device for Reading or Updating a report value
    SONATA_HOGPD_REPORT_CFM,

    /// Inform Device APP that Protocol Mode Characteristic Value has been written on Device
    SONATA_HOGPD_PROTO_MODE_REQ_IND,
    /// Confirm if the new protocol mode value
    SONATA_HOGPD_PROTO_MODE_CFM,

    /// Inform Device APP that a Client Characteristic Configuration has been modified
    SONATA_HOGPD_NTF_CFG_IND,
    /// Inform APP that HID Control Point Characteristic Value has been written on Device
    SONATA_HOGPD_CTNL_PT_IND,

    SONATA_APP_HID_MOUSE_TIMEOUT_TIMER,
};

/// Messages for HID Over GATT Profile Device Role
typedef enum {
    SONATA_HOGPD_MOUSE_TIMEOUT_TIMER = SONATA_APP_HID_MOUSE_TIMEOUT_TIMER,
    SONATA_HOGPD_KEYBOARD_TIMEOUT_TIMER,
} app_hogpd_msg_id;

typedef enum {
    HID_DISCONNECTED,
    HID_DISCONNECTING,
    HID_CONNECTING,
    HID_CONNECTED,
} hid_connection_state;

typedef enum {
    DIR_BROADCAST_NO_NEED,
    DIR_BROADCAST_NEEDED,
    DIR_BROADCAST_ALREADY_SEND,
} directed_broadcast_state;

/// HID Information bit values
typedef enum {
    /// Device capable of providing wake-up signal to a HID host
    SONATA_HIDS_REMOTE_WAKE_CAPABLE           = 0x01,
    /// Normally connectable support bit
    SONATA_HIDS_NORM_CONNECTABLE              = 0x02,
} hogp_info_bit;

/*
 * Type Definition
 ****************************************************************************************
 */

/// External Report Reference
typedef struct sonata_hogpd_ext_ref {
    /// External Report Reference - Included Service
    uint16_t inc_svc_hdl;
    /// External Report Reference - Characteristic UUID
    uint16_t rep_ref_uuid;
} sonata_hogpd_ext_ref_t;

/// HID Information structure
typedef struct sonata_hids_hid_info {
    /// bcdHID
    uint16_t bcdHID;
    /// bCountryCode
    uint8_t  bCountryCode;
    /// Flags
    uint8_t  flags;
} sonata_hids_hid_info_t;

/// Database Creation Service Instance Configuration structure
typedef struct sonata_hogpd_hids_cfg {
    /// Service Features (@see enum sonata_hogpd_cfg)
    uint8_t svc_features;
    /// Number of Report Char. instances to add in the database
    uint8_t report_nb;
    /// Report Char. Configuration (@see enum sonata_hogpd_report_cfg)
    uint8_t report_char_cfg[SONATA_HOGPD_NB_REPORT_INST_MAX];
    /// Report id number
    uint8_t report_id[SONATA_HOGPD_NB_REPORT_INST_MAX];
    /// HID Information Char. Values
    sonata_hids_hid_info_t hid_info;
    /// External Report Reference
    struct sonata_hogpd_ext_ref ext_ref;

} sonata_hogpd_hids_cfg_t;

/// Parameters of the @ref HOGPD_CREATE_DB_REQ message
typedef  struct sonata_hogpd_db_cfg {
    /// Number of HIDS to add
    uint8_t hids_nb;
    /// Initial configuration for each HIDS instance
    struct sonata_hogpd_hids_cfg cfg[SONATA_HOGPD_NB_HIDS_INST_MAX];
} sonata_hogpd_db_cfg_t;

/*!
 * @brief Callback for hogpd value
 */
typedef struct {
    uint16_t (*hogpd_report_request)(uint8_t conidx, uint8_t operation, uint16_t handle, uint8_t report_type,
                                     uint8_t hid_idx, uint8_t length, uint8_t idx, uint8_t *value);
    uint16_t (*hogpd_protocol_mode_request)(uint8_t conidx, uint8_t operation, uint8_t proto_mode);
    uint16_t (*hogpd_control_point_write)(uint8_t conidx, uint8_t hid_idx);
    uint16_t (*hogpd_enable_response)(uint8_t conidx, uint8_t enable_state);
    uint16_t (*hogpd_report_update_response)(uint8_t conidx, uint8_t status);
    uint16_t (*hogpd_ntf_config)(uint8_t conidx, uint8_t ntf_len, uint16_t *ntf_cfg);
} prf_hogpd_callback;

/**
 *  Botton definition
 *  --------------------------------------------------------------------------
 *  Bit      |   7   |   6   |   5   |   4   |   3   |   2   |   1   |   0   |
 *  --------------------------------------------------------------------------
 *  Byte 0   |        Not Used       |-Y mark|-X mark| Middle| Right | Left  |
 *  --------------------------------------------------------------------------
 */
typedef struct ps_mouse_report {
    int16_t   repX;    // x-quaddec of the mouse
    int16_t   repY;    // y-quaddec of the mouse
} ps_mouse_report_t;

/*!
 * @brief sonata_get_connection_state
 * @param
 */
int sonata_get_connection_state();

/*!
 * @brief sonata_set_connection_state
 * @param state
 */
void sonata_set_connection_state(int state);

/*!
 * @brief send report update request message
 * @param hid_idx
 * @param report_idx index of the report in HID report map
 * @param report_type report type  see sonata_hogpd_report_type
 * @param report_len report length
 * @param report  report data
 * @return
 */
int sonata_prf_hogp_send_report_update_request(uint8_t conidx, uint8_t hid_idx, uint8_t report_idx, uint8_t report_type,
        uint16_t report_len, uint8_t *report);

/*!
 * @brief send protocol mode confirm message
 * @param status  Status of the request
 * @param proto_mode New Protocol Mode Characteristic Value
 * @return
 */
int sonata_prf_hogp_send_protocol_mode_confirm(uint8_t conidx, uint8_t hid_idx, uint8_t status, uint8_t proto_mode);

/*!
 * @brief send report map confirm
 * @param hid_idx index of hid, for single HID, set it to 0
 * @param report_idx report index in HID report map
 * @param report_type report type  see sonata_hogpd_report_type
 * @param report_map_len report map length
 * @param report_map report map
 * @return
 */
int sonata_prf_hogpd_send_report_map_confirm(uint8_t conidx, uint8_t hid_idx, uint8_t report_idx, uint8_t report_type,
        uint8_t report_map_len, const uint8_t *report_map);

/*!
 * @brief sonata_prf_hogpd_add
 * @param sec_lvl
 * @param start_hdl
 * @param db_cfg
 */
void sonata_prf_hogpd_add(uint8_t sec_lvl, uint16_t start_hdl, sonata_hogpd_db_cfg_t db_cfg);

/*!
 * @brief sonata_prf_hogpd_init
 * @param
 */
void sonata_prf_hogpd_init(void);

/*!
 * @brief sonata_prf_hogpd_enable
 * @param conidx
 */
void sonata_prf_hogpd_enable(uint8_t conidx);

/*!
 * @brief sonata_prf_app_hid_device_data_send_report
 * @param report_buff
 * @return
 */
void sonata_prf_hogp_mouse_data_send_report(ps_mouse_report_t report_buff);

/*!
 * @brief
 * @param report_idx
 * @param report
 * @note only suit for default mouse HID report map, is app use custom HID report map, use  sonata_prf_hogp_send_report_update_request()
 */
void sonata_prf_hogp_send_report_update_request_for_mouse(uint8_t report_idx, ps_mouse_report_t report);

/*!
 * @brief sonata_prf_hogp_send_report_update_request_for_keyboard
 * @param ps_keyboard_report_t
 * @return
 */
void sonata_prf_hogp_send_report_update_request_for_keyboard(uint8_t report_idx, uint8_t *ps_keyboard_report_t,
        uint8_t report_len);

/*!
 * @brief app_start_hid_keyboard
 * @param cb
 * @return
 */
uint16_t sonata_prf_hogpd_register_callback(prf_hogpd_callback *cb);

/*!
 * @brief sonata_prf_hogpd_boot_mouse_input_report
 * @param conidx
 * @param hid_idx
 * @param report_type
 * @param idx
 * @return
 */
void sonata_prf_hogpd_boot_mouse_input_report(uint8_t conidx, uint8_t hid_idx, uint8_t report_type, uint8_t idx);

/*!
 * @brief sonata_prf_hogpd_report
 * @param conidx
 * @param report_type
 * @param hid_idx
 * @param report_idx
 * @param length
 * @param value
 * @param send_cfm
 * @return
 */
void sonata_prf_hogpd_report(uint8_t conidx, uint8_t report_type, uint8_t hid_idx, uint8_t report_idx, uint8_t length,
                             uint8_t *value, bool send_cfm);

/*!
 * @brief sonata_prf_hogpd_proto_mode_req
 * @param p_param
 * @return
 */
uint16_t sonata_prf_hogpd_proto_mode_req(uint8_t conidx, uint8_t operation, uint8_t proto_mode);

/*!
 * @brief sonata_prf_hogpd_control_point_req
 * @param p_param
 * @return
 */
uint16_t sonata_prf_hogpd_control_point_req(uint8_t conidx, uint8_t hid_idx);

/*!
 * @brief sonata_prf_hogpd_updata_conn_params
 * @param intv_min
 * @param intv_max
 * @param latency
 * @param time_out
 */
void sonata_prf_hogpd_updata_conn_params(uint16_t intv_min, uint16_t intv_max, uint16_t latency, uint16_t time_out);

extern const sonata_api_subtask_handlers_t prf_hogpd_api_ke_msg_handlers;
/*!
 * @brief sonata_prf_hogpd_reset
 */
void sonata_prf_hogpd_reset();
void sonata_hogpd_send_error_code(uint8_t conidx, uint8_t operation, uint8_t hid_idx, uint8_t report_type,
                                  uint8_t report_idx, uint8_t error_code);

/** @}*/

/// @cond
#endif // BLE_HOST_PRESENT
/// @endcond

#endif // _SONATA_PRF_HOGPD_API_H_

