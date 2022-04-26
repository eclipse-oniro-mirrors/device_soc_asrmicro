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
 * @file sonata_prf_hogprh_api.h
 *
 * @brief header file - HOGPRH service server api
 *
 ****************************************************************************************
 */

#ifndef _SONATA_PRF_HOGPRH_API_H_
#define _SONATA_PRF_HOGPRH_API_H_

/*
 * INCLUDE FILES
 ****************************************************************************************
 */

/// @cond
#if BLE_HOST_PRESENT
/// @endcond

#include "sonata_utils_api.h"

/**
 * @defgroup SONATA_PRF_HOGPRH_API PRF_HOGPRH_API
 * @{
 */

/*
 * MACRO DEFINITIONS
 ****************************************************************************************
 */

/*
 * Type Definition
 ****************************************************************************************
 */
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
    /// EDIV (2bytes), RAND NB (8bytes),  LTK (16 bytes), Key Size (1 byte)
    SONATA_TAG_LTK,
    /// app_ltk_key_in
    SONATA_TAG_LTK_IN,
    /// app irk addr
    SONATA_TAG_IRK_ADDR,
    /// app irk
    SONATA_TAG_IRK,
    /// RCS mode control handle
    SONATA_TAG_CMD_HANDLE,
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
    /// EDIV (2bytes), RAND NB (8bytes),  LTK (16 bytes), Key Size (1 byte)
    SONATA_LEN_LTK                        = 27,
    /// app_ltk_key_in len
    SONATA_LEN_LTK_IN                     = 27,
    /// app irk addr len
    SONATA_LEN_IRK_ADDR                   = 6,
    /// app irk len
    SONATA_LEN_IRK                        = 16,
    /// RCS mode control handle len
    SONATA_LEN_CMD_HANDLE                 = 2,
} sonata_app_nvds_len;

/// Peer HID service info that can be read/write
enum sonata_hogprh_info {
    /// Protocol Mode
    SONATA_HOGPRH_PROTO_MODE,
    /// Report Map
    SONATA_HOGPRH_REPORT_MAP,
    /// Report Map Char. External Report Reference Descriptor
    SONATA_HOGPRH_REPORT_MAP_EXT_REP_REF,

    /// HID Information
    SONATA_HOGPRH_HID_INFO,
    /// HID Control Point
    SONATA_HOGPRH_HID_CTNL_PT,
    /// Report
    SONATA_HOGPRH_REPORT,
    /// Report Char. Report Reference
    SONATA_HOGPRH_REPORT_REF,
    /// Report Notification config
    SONATA_HOGPRH_REPORT_NTF_CFG,

    SONATA_HOGPRH_INFO_MAX,
};

/// Parameters of the @ref HOGPRH_READ_INFO_RSP message
typedef struct sonata_hogprh_read_info_req {
    /// Characteristic info @see enum hogprh_info
    uint8_t info;
    /// HID Service Instance - From 0 to HOGPRH_NB_HIDS_INST_MAX-1
    uint8_t hid_idx;
    /// HID Report Index: only relevant for:
    ///  - info = HOGPRH_REPORT
    ///  - info = HOGPRH_REPORT_REF
    ///  - info = HOGPRH_REPORT_NTF_CFG
    uint8_t report_idx;
} sonata_hogprh_read_info_req_t;

/// HID Information structure
struct sonata_hids_hid_info {
    /// bcdHID
    uint16_t bcdHID;
    /// bCountryCode
    uint8_t  bCountryCode;
    /// Flags
    uint8_t  flags;
};

/// HID report info
struct sonata_hogprh_report {
    /// Report Length
    uint8_t length;
    /// Report value
    uint8_t value[__ARRAY_EMPTY];
};

/// HID report Reference
struct sonata_hogprh_report_ref {
    /// Report ID
    uint8_t id;
    /// Report Type
    uint8_t type;
};

/// HID report MAP info
struct sonata_hogprh_report_map {
    /// Report MAP Length
    uint16_t length;
    /// Report MAP value
    uint8_t value[__ARRAY_EMPTY];
};

/// HID report MAP reference
struct sonata_hogprh_report_map_ref {
    /// Reference UUID length
    uint8_t uuid_len;
    /// Reference UUID Value
    uint8_t uuid[__ARRAY_EMPTY];
};

/// Information data
union sonata_hogprh_data {
    /// Protocol Mode
    ///  - info = HOGPRH_PROTO_MODE
    uint8_t proto_mode;

    /// HID Information value
    ///  - info = HOGPRH_HID_INFO
    struct sonata_hids_hid_info hid_info;

    /// HID Control Point value to write
    ///  - info = HOGPRH_HID_CTNL_PT
    uint8_t hid_ctnl_pt;

    /// Report information
    ///  - info = HOGPRH_REPORT
    struct sonata_hogprh_report report;

    /// Notification Configuration Value
    ///  - info = HOGPRH_REPORT_NTF_CFG
    uint16_t report_cfg;

    /// HID report Reference
    ///  - info = HOGPRH_REPORT_REF
    struct sonata_hogprh_report_ref report_ref;

    /// HID report MAP info
    ///  - info = HOGPRH_REPORT_MAP
    struct sonata_hogprh_report_map report_map;

    /// HID report MAP reference
    ///  - info = HOGPRH_REPORT_MAP_EXT_REP_REF
    struct sonata_hogprh_report_map_ref report_map_ref;
};

/// Parameters of the @ref HOGPRH_WRITE_REQ message
typedef struct sonata_hogprh_write_req {
    /// Characteristic info @see enum hogprh_info
    uint8_t info;
    /// HID Service Instance - From 0 to HOGPRH_NB_HIDS_INST_MAX-1
    uint8_t hid_idx;
    /// HID Report Index: only relevant for:
    ///  - info = HOGPRH_REPORT
    ///  - info = HOGPRH_REPORT_NTF_CFG
    uint8_t report_idx;
    /// Write type (Write without Response True or Write Request)
    /// - only valid for HOGPRH_REPORT
    bool    wr_cmd;
    /// Information data
    union sonata_hogprh_data data;
} sonata_hogprh_write_req_t;

/*!
 * @brief Callback for hogpd value
 */
typedef struct {
    uint16_t (*hogprh_report_indicate)(uint8_t hid_idx, uint8_t report_idx, uint8_t report_length, uint8_t *value);

} prf_hogprh_callback;

/*!
 * @brief sonata_prf_hogprh_init
 * @param
 */
void sonata_prf_hogprh_init();

/*!
 * @brief sonata_prf_hogprh_add
 * @param sec_lvl
 * @param start_hdl
 * @param db_cfg
 */
void sonata_prf_hogprh_add(uint8_t sec_lvl, uint16_t start_hdl, int db_cfg);

/*!
 * @brief sonata_prf_hogprh_enable
 * @param conidx
 */
void sonata_prf_hogprh_enable(uint8_t conidx);

/*!
 * @brief sonata_prf_hogprh_read_info_req_handler
 * @param param
 */
int sonata_prf_hogprh_read_info_req_handler(sonata_hogprh_read_info_req_t *param);

/*!
 * @brief sonata_prf_hogprh_updata_conn_params
 * @param intv_min
 * @param intv_max
 * @param latency
 * @param time_out
 */
void sonata_prf_hogprh_updata_conn_params(uint16_t intv_min, uint16_t intv_max, uint16_t latency, uint16_t time_out);

/*!
 * @brief sonata_prf_hogprh_write_req_handler
 * @param param
 */
int sonata_prf_hogprh_write_req_handler(sonata_hogprh_write_req_t *param);

/*!
 * @brief sonata_prf_hogprh_register_callback
 * @param cb
 */
uint16_t sonata_prf_hogprh_register_callback(prf_hogprh_callback *cb);

void sonata_prf_hogprh_reset();

extern const sonata_api_subtask_handlers_t prf_hogprh_api_ke_msg_handlers;

/** @}*/

/// @cond
#endif // BLE_HOST_PRESENT
/// @endcond

#endif // _SONATA_PRF_HOGPRH_API_H_

