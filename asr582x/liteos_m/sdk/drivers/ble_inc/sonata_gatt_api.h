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
 * @file sonata_gatt_api.h
 *
 * @brief header file - gatt application interface
 *
 ****************************************************************************************
 */

#ifndef _SONATA_GATT_API_H_
#define _SONATA_GATT_API_H_

/*
 * INCLUDE FILES
 ****************************************************************************************
 */
#include "sonata_config.h"
/// @cond
#if defined(STACK_BLE_HOST_PRESENT) || defined(BLE_HOST_PRESENT)
/// @endcond

#include "sonata_utils_api.h"
#include "sonata_att.h"

/**
 * @defgroup SONATA_GATT_API GATT_API
 * @{
 */

/*
 * MACRO DEFINITIONS
 ****************************************************************************************
 */

/// GATT read perm auth     (0x0202)
#define PRD_AU   (SONATA_PERM(RD, ENABLE)        | SONATA_PERM(RP, AUTH))
/// GATT write perm  auth   (0x0808)
#define PWR_AU   (SONATA_PERM(WRITE_REQ, ENABLE) | SONATA_PERM(WP, AUTH))
/// GATT ind perm  auth     (0X2020)
#define PIND_AU  (SONATA_PERM(IND, ENABLE)       | SONATA_PERM(IP, AUTH))
/// GATT notify perm  auth  (0x1080)
#define PNTF_AU  (SONATA_PERM(NTF, ENABLE)       | SONATA_PERM(NP, AUTH))

/// GATT read perm unauth   (0x0201)
#define PRD_UA   (SONATA_PERM(RD, ENABLE)        | SONATA_PERM(RP, UNAUTH))
/// GATT write perm unauth  (0x0804)
#define PWR_UA   (SONATA_PERM(WRITE_REQ, ENABLE) | SONATA_PERM(WP, UNAUTH))
/// GATT ind perm  unauth   (0X2010)
#define PIND_UA  (SONATA_PERM(IND, ENABLE)       | SONATA_PERM(IP, UNAUTH))
/// GATT notify perm unauth (0x1040)
#define PNTF_UA  (SONATA_PERM(NTF, ENABLE)       | SONATA_PERM(NP, UNAUTH))

/// GATT read perm no auth  (0x0200)
#define PRD_NA   (SONATA_PERM(RD, ENABLE)        | SONATA_PERM(RP, NO_AUTH))
/// GATT write perm no auth (0x0800)
#define PWR_NA   (SONATA_PERM(WRITE_REQ, ENABLE) | SONATA_PERM(WP, NO_AUTH))
/// GATT ind perm  no auth  (0X2000)
#define PIND_NA  (SONATA_PERM(IND, ENABLE)       | SONATA_PERM(IP, NO_AUTH))
/// GATT notify perm no auth(0x1000)
#define PNTF_NA  (SONATA_PERM(NTF, ENABLE)       | SONATA_PERM(NP, NO_AUTH))

/// extended Descriptor Present (0X8000)
#define PEXT     (SONATA_PERM(EXT,ENABLE))
/// enable Read Indication (1 = Value not present in Database) (0x8000)
#define PRI      (SONATA_PERM(RI, ENABLE))

#define PWC_NA (SONATA_PERM(WRITE_COMMAND, ENABLE) | SONATA_PERM(WP, NO_AUTH))

/*
 * ENUM DEFINITIONS
 ****************************************************************************************
 */
/*!
 * @brief request operation type - application interface
 */
typedef enum {
    /*              Attribute Client Flags              */
    /* No Operation (if nothing has been requested)     */
    /* ************************************************ */
    /// No operation
    SONATA_GATT_NO_OP                                    = 0x00,

    /* Operation flags for MTU Exchange                 */
    /* ************************************************ */
    /// Perform MTU exchange
    SONATA_GATT_MTU_EXCH                                 = 0x01,

    /*      Operation flags for discovery operation     */
    /* ************************************************ */
    /// Discover all services
    SONATA_GATT_DISC_ALL_SVC                             = 0x02,
    /// Discover services by UUID
    SONATA_GATT_DISC_BY_UUID_SVC                         = 0x03,
    /// Discover included services
    SONATA_GATT_DISC_INCLUDED_SVC                        = 0x04,
    /// Discover all characteristics
    SONATA_GATT_DISC_ALL_CHAR                            = 0x05,
    /// Discover characteristic by UUID
    SONATA_GATT_DISC_BY_UUID_CHAR                        = 0x06,
    /// Discover characteristic descriptor
    SONATA_GATT_DISC_DESC_CHAR                           = 0x07,

    /* Operation flags for reading attributes           */
    /* ************************************************ */
    /// Read attribute
    SONATA_GATT_READ                                     = 0x08,
    /// Read long attribute
    SONATA_GATT_READ_LONG                                = 0x09,
    /// Read attribute by UUID
    SONATA_GATT_READ_BY_UUID                             = 0x0A,
    /// Read multiple attribute
    SONATA_GATT_READ_MULTIPLE                            = 0x0B,

    /* Operation flags for writing/modifying attributes */
    /* ************************************************ */
    /// Write attribute
    SONATA_GATT_WRITE                                    = 0x0C,
    /// Write no response
    SONATA_GATT_WRITE_NO_RESPONSE                        = 0x0D,
    /// Write signed
    SONATA_GATT_WRITE_SIGNED                             = 0x0E,
    /// Execute write
    SONATA_GATT_EXEC_WRITE                               = 0x0F,

    /* Operation flags for registering to peer device   */
    /* events                                           */
    /* ************************************************ */
    /// Register to peer device events
    SONATA_GATT_REGISTER                                 = 0x10,
    /// Unregister from peer device events
    SONATA_GATT_UNREGISTER                               = 0x11,

    /* Operation flags for sending events to peer devicd */
    /* ************************************************ */
    /// Send an attribute notification
    SONATA_GATT_NOTIFY                                   = 0x12,
    /// Send an attribute indication
    SONATA_GATT_INDICATE                                 = 0x13,

    /* Service Discovery Procedure                      */
    /* ************************************************ */
    /// Search specific service
    SONATA_GATT_SDP_DISC_SVC                             = 0x15,
    /// Search for all services
    SONATA_GATT_SDP_DISC_SVC_ALL                         = 0x16,
    /// Cancel Service Discovery Procedure
    SONATA_GATT_SDP_DISC_CANCEL                          = 0x17,

    /* Robust Cache                                     */
    /* ************************************************ */
    /// Read peer database hash
    SONATA_GATT_READ_DB_HASH                             = 0x20,
    /// Enable Robust database cache feature
    SONATA_GATT_ROBUST_DB_CACHE_EN                       = 0x21,

    /* Internal API                                     */
    /* ************************************************ */
    /// Send a service changed indication
    SONATA_GATT_SVC_CHANGED                              = 0x40,
} sonata_gatt_operation;

/// GATT response type
typedef enum {
    /// Add service in database response
    SONATA_GATT_ADD_SVC_RSP                   = 0x0B01,
    /// Get permission settings of service response
    SONATA_GATT_SVC_GET_PERMISSION_RSP        = 0x0B03,
    /// Set permission settings of service response
    SONATA_GATT_SVC_SET_PERMISSION_RSP        = 0x0B05,
    /// Get permission settings of attribute response
    SONATA_GATT_ATT_GET_PERMISSION_RSP        = 0x0B07,
    /// Set permission settings of attribute response
    SONATA_GATT_ATT_SET_PERMISSION_RSP        = 0x0B09,
    /// Get attribute value response
    SONATA_GATT_ATT_GET_VALUE_RSP             = 0x0B0B,
    /// Set attribute value request
    SONATA_GATT_ATT_SET_VALUE_RSP             = 0x0B0D,
    /// DEBUG ONLY: Destroy Attribute database request
    SONATA_GATT_DESTROY_DB_RSP                = 0x0B10,
    /// DEBUG ONLY: Retrieve list of services response
    SONATA_GATT_SVC_GET_LIST_RSP              = 0x0B12,
    /// DEBUG ONLY: Retrieve information of attribute response
    SONATA_GATT_ATT_GET_INFO_RSP              = 0x0B14,
    /// DEBUG ONLY: Set Service visibility response
    SONATA_GATT_SVC_VISIBILITY_SET_RSP        = 0x0B16,
    /// Compute hash value of the attribute database response
    SONATA_GATT_ATT_DB_HASH_COMP_RSP          = 0x0B21,
} sonata_ble_response_type;

/*
 * Type Definition
 ****************************************************************************************
 */

/**
 * Attribute Description
 */
/* @TRACE */
typedef struct sonata_gatt_att_desc {
    /** Attribute UUID (LSB First) */
    uint8_t uuid[SONATA_ATT_UUID_128_LEN];

    /**
     *  Attribute Permission (@see attm_perm_mask)
     */
    uint16_t perm;

    /**
     * Maximum length of the attribute
     *
     * Note:
     * For Included Services and Characteristic Declarations, this field contains targeted
     * handle.
     *
     * For Characteristic Extended Properties, this field contains 2 byte value
     *
     * Not used Client Characteristic Configuration and Server Characteristic Configuration,
     * this field is not used.
     */
    uint16_t max_len;

    /**
     * Attribute Extended permissions
     *
     * Extended Value permission bit field
     *
     *   15   14   13   12   11   10    9    8    7    6    5    4    3    2    1    0
     * +----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+
     * | RI |UUID_LEN |EKS |                       Reserved                            |
     * +----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+
    *
     * Bit [0-11] : Reserved
     * Bit [12]   : Encryption key Size must be 16 bytes
     * Bit [13-14]: UUID Length             (0 = 16 bits, 1 = 32 bits, 2 = 128 bits, 3 = RFU)
     * Bit [15]   : Trigger Read Indication (0 = Value present in Database, 1 = Value not present in Database)
     */
    uint16_t ext_perm;
} sonata_gatt_att_desc_t;

/// Service information
/* @TRACE */
typedef struct sonata_gatt_svc_info {
    /// Service start handle
    uint16_t start_hdl;
    /// Service end handle
    uint16_t end_hdl;
    /// Service task_id
    uint16_t task_id;
    /// Service permission
    uint8_t perm;
} sonata_gatt_svc_info_t;

/// @brief Callbacks for GATT module
typedef struct {
    /*************** GATT Controller  ***************/
    /// @deprecated use @see ble_complete_callback
    void (*gatt_disc_all_svc_complete)(uint8_t conidx, uint16_t status);
    /// @deprecated use @see ble_complete_callback
    void (*gatt_read_complete)(uint8_t conidx, uint16_t status);

    /// Callback for discover services
    uint16_t (*gatt_disc_svc)(uint8_t conidx, uint16_t start_hdl, uint16_t end_hdl, uint8_t uuid_len, uint8_t *uuid);

    /// Callback when discover all inclued service complete
    uint16_t (*gatt_disc_svc_incl)(uint8_t conidx, uint16_t attr_hdl, uint16_t start_hdl, uint16_t end_hdl,
                                   uint8_t uuid_len, uint8_t *uuid);

    /// Callback for discover characteristic
    uint16_t (*gatt_disc_char)(uint8_t conidx, uint16_t attr_hdl, uint16_t pointer_hdl, uint8_t prop, uint8_t uuid_len,
                               uint8_t *uuid);    /* void(*gatt_disc_char_desc)(void); */

    /// Callback for discover characteristic description
    uint16_t (*gatt_disc_char_desc)(uint8_t conidx, uint16_t attr_hdl, uint8_t uuid_len,
                                    uint8_t *uuid);    /* void(*gatt_disc_char_desc)(void); */

    /// Callback for GATT read complete
    uint16_t (*gatt_read)(uint8_t conidx, uint16_t handle, uint16_t offset, uint16_t length, uint8_t *value);
    /// Callback for GATT read request
    uint16_t (*gatt_read_req)(uint8_t conidx, uint16_t handle);
    /// Callback for GATT write request
    uint16_t (*gatt_write_req)(uint8_t conidx, uint16_t handle, uint16_t offset, uint16_t length, uint8_t *value);

    /// Callback for GATT mtu changed indicate
    uint16_t (*gatt_mtu_changed)(uint8_t conidx, uint16_t mtu);

    /// Callback for GATT ATT information request
    uint16_t (*gatt_att_info_req)(uint8_t conidx, uint16_t handle);

    /// Callback for GATT SDP service
    uint16_t (*gatt_sdp_service)(uint8_t conidx, uint16_t handle);

    /// Callback for GATT event
    uint16_t (*gatt_event)(uint8_t conidx, uint16_t handle, uint16_t type, uint16_t length, uint8_t *value);

    /// Callback for GATT service change request
    uint16_t (*gatt_service_change_req)(uint8_t conidx, uint16_t start_handle, uint16_t end_handle);

    /// Callback for connection information(about GATT for current connection that can be reuse on another connection)
    uint16_t (*gatt_connection_info)(uint8_t conidx, uint16_t gatt_start_handle, uint16_t gatt_end_handle,
                                     uint16_t svc_chg_handle,
                                     uint8_t cli_info, uint8_t cli_feat);

    /*************** GATT Manager  ***************/
} ble_gatt_callback;

/*
 * FUNCTION DECLARATIONS
 ****************************************************************************************
 */

/*!
 * @brief Exchange mtu
 * @param conidx connecting index, used for multiple connection. for single connection, set it to 0.
 * @return API_SUCCESS
 */
uint16_t sonata_ble_gatt_exchange_mtu(uint8_t conidx);
/*!
 * @brief Discover All Services
 * @param conidx connecting index, used for multiple connection. for single connection, set it to 0.
 * @return API_SUCCESS
 */
uint16_t sonata_ble_gatt_disc_all_svc(uint8_t conidx);

/*!
 * @brief Disc service by uudi
 * @param conidx connecting index, used for multiple connection. for single connection, set it to 0.
 * @param start_hdl Service start handle
 * @param end_hdl Service end handle
 * @param uuid_len UUID length
* @param uuid UUID
* @return API_SUCCESS
 */
uint16_t sonata_ble_gatt_disc_svc_by_uuid(uint8_t conidx, uint16_t start_hdl, uint16_t end_hdl, uint8_t uuid_len,
        uint8_t *uuid);

/*!
 * @brief Disc included servcie
 * @param conidx connecting index, used for multiple connection. for single connection, set it to 0.
 * @param start_hdl Service start handle
 * @param end_hdl Service end handle
 * @return API_SUCCESS
 */
uint16_t sonata_ble_gatt_disc_included_svc(uint8_t conidx, uint16_t start_hdl, uint16_t end_hdl);
/*!
 * @brief Discover All characteristic
 * @param conidx connecting index, used for multiple connection. for single connection, set it to 0.
 * @param start_hdl Service start handle
 * @param end_hdl Service end handle
 * @return API_SUCCESS
 */
uint16_t sonata_ble_gatt_disc_all_characteristic(uint8_t conidx, uint16_t start_hdl, uint16_t end_hdl);

/*!
 * @brief Disc characteristic by uuid
 * @param conidx connecting index, used for multiple connection. for single connection, set it to 0.
 * @param start_hdl Service start handle
 * @param end_hdl Service end handle
 * @param uuid_len UUID length
* @param uuid UUID
* @return API_SUCCESS
 */
uint16_t sonata_ble_gatt_disc_characteristic_by_uuid(uint8_t conidx, uint16_t start_hdl, uint16_t end_hdl,
        uint8_t uuid_len, uint8_t *uuid);
/*!
 * @brief Discover All desc
 * @param conidx connecting index, used for multiple connection. for single connection, set it to 0.
 * @param start_hdl Service start handle
 * @param end_hdl Service end handle
 * @return API_SUCCESS
 */
uint16_t sonata_ble_gatt_disc_all_descriptor(uint8_t conidx, uint16_t start_hdl, uint16_t end_hdl);
/*!
 * @brief Read GATT value by handler
 * @param conidx connecting index, used for multiple connection. for single connection, set it to 0.
 * @param handle_id : attribute handle
 * @return API_SUCCESS
 */
uint16_t sonata_ble_gatt_read_by_handle(uint8_t conidx, uint16_t handle_id);

/*!
 * @brief Send NTF event
 * @param conidx connecting index, used for multiple connection. for single connection, set it to 0.
 * @param handle handle id
 * @param length value length
 * @param value  value
 * @return API_SUCCESS
 */
uint16_t sonata_ble_gatt_send_notify_event(uint8_t conidx, uint16_t handle, uint16_t length, uint8_t *value);

/*!
 * @brief Send indicate event
 * @param conidx connecting index, used for multiple connection. for single connection, set it to 0.
 * @param handle handle id
 * @param length value length
 * @param value  value
 * @return API_SUCCESS
 */
uint16_t sonata_ble_gatt_send_indicate_event(uint8_t conidx, uint16_t handle, uint16_t length, uint8_t *value);

/*!
 * @brief Gatt read
 * @param conidx connecting index, used for multiple connection. for single connection, set it to 0.
 * @param handle handle id
 * @param offset start offset in data payload
 * @param length Length of data to read (0 = read all)
 * @return API_SUCCESS
 */
uint16_t sonata_ble_gatt_read(uint8_t conidx, uint16_t handle, uint16_t offset, uint16_t length);

/*!
 * @brief Gatt read long
 * @param conidx connecting index, used for multiple connection. for single connection, set it to 0.
 * @param handle handle id
 * @param offset start offset in data payload
 * @param length Length of data to read (0 = read all)
 * @return API_SUCCESS
 */
uint16_t sonata_ble_gatt_gatt_read_long(uint8_t conidx, uint16_t handle, uint16_t offset, uint16_t length);

/*!
 * @brief Gatt read by UUID
 * @param conidx connecting index, used for multiple connection. for single connection, set it to 0.
 * @param seq_num
 * @param start_hdl Service start handle
 * @param end_hdl End handle
 * @param uuid_len UUID length
* @param uuid UUID
* @return API_SUCCESS
 */
uint16_t sonata_ble_gatt_read_by_uuid(uint8_t conidx, uint16_t seq_num, uint16_t start_hdl, uint16_t end_hdl,
                                      uint8_t uuid_len, uint8_t *uuid);

/*!
 * @brief Gatt write
 * @param conidx connecting index, used for multiple connection. for single connection, set it to 0.
 * @param handle handle id
 * @param offset Write offset
 * @param cursor Internal write cursor shall be initialized to 0
 * @param length Write length
 * @param value Value to write
 * @return API_SUCCESS
 */
uint16_t sonata_ble_gatt_write(uint8_t conidx, uint16_t handle, uint16_t offset, uint16_t cursor, uint16_t length,
                               uint8_t *value);

/*!
 * @brief Gatt write no response
 * @param conidx connecting index, used for multiple connection. for single connection, set it to 0.
 * @param handle handle id
 * @param offset Write offset
 * @param cursor Internal write cursor shall be initialized to 0
 * @param length Write length
 * @param value Value to write
 * @return API_SUCCESS
 */
uint16_t sonata_ble_gatt_write_no_response(uint8_t conidx,  uint16_t handle, uint16_t offset, uint16_t cursor,
        uint16_t length, uint8_t *value);

/*!
 * @brief Gatt write signed
 * @param conidx connecting index, used for multiple connection. for single connection, set it to 0.
 * @param handle handle id
 * @param offset Write offset
 * @param cursor Internal write cursor shall be initialized to 0
 * @param length Write length
 * @param value Value to write
 * @return API_SUCCESS
 */
uint16_t sonata_ble_gatt_write_signed(uint8_t conidx, uint16_t handle, uint16_t offset, uint16_t cursor,
                                      uint16_t length, uint8_t *value);

/*!
 * @brief Gatt excute write
 * @param conidx connecting index, used for multiple connection. for single connection, set it to 0.
 * @param execute [True = perform/False cancel] pending write operations
 * @return API_SUCCESS
 */
uint16_t sonata_ble_gatt_execute_write(uint8_t conidx, bool execute);

/*!
 * @brief Gatt register to peer device event
 * @param conidx connecting index, used for multiple connection. for single connection, set it to 0.
 * @param start_hdl Service start handle
 * @param end_hdl End handle
 * @return API_SUCCESS
 */
uint16_t sonata_ble_gatt_register_to_peer_device_event(uint8_t conidx, uint16_t start_hdl, uint16_t end_hdl);

/*!
 * @brief Gatt unregister to peer device event
 * @param conidx connecting index, used for multiple connection. for single connection, set it to 0.
 * @param start_hdl Service start handle
 * @param end_hdl End handle
 * @return API_SUCCESS
 */
uint16_t sonata_ble_gatt_unregister_to_peer_device_event(uint8_t conidx, uint16_t start_hdl, uint16_t end_hdl);

/*!
 * @brief Gatt SDP discovery service
 * @param conidx connecting index, used for multiple connection. for single connection, set it to 0.
 * @param start_hdl Service start handle
 * @param end_hdl End handle
 * @param uuid_len UUID length
* @param uuid UUID
* @return API_SUCCESS
 */
uint16_t sonata_ble_gatt_sdp_discovery_service(uint8_t conidx, uint16_t start_hdl, uint16_t end_hdl, uint8_t uuid_len,
        uint8_t *uuid);

/*!
 * @brief Gatt sdp cancel discovery
 * @param conidx connecting index, used for multiple connection. for single connection, set it to 0.
 * @return API_SUCCESS
 */
uint16_t sonata_ble_gatt_sdp_cancel_discovery(uint8_t conidx);

/*!
 * @brief Gatt sdp discovery all service
 * @param conidx connecting index, used for multiple connection. for single connection, set it to 0.
 * @param start_hdl Service start handle
 * @param end_hdl Service end handle
 * @param uuid_len UUID length
 * @param uuid UUID
 * @return API_SUCCESS
 */
uint16_t sonata_ble_gatt_sdp_discovery_all_service(uint8_t conidx, uint16_t start_hdl, uint16_t end_hdl,
        uint8_t uuid_len, uint8_t *uuid);

/*!
 * @brief Gatt enable robust database cache
 * @param conidx connecting index, used for multiple connection. for single connection, set it to 0.
 * @return API_SUCCESS
 */
uint16_t sonata_ble_gatt_robust_db_cache_enable(uint8_t conidx);

/*!
 * @brief Gatt read database hase
 * @param conidx connecting index, used for multiple connection. for single connection, set it to 0.
 * @return API_SUCCESS
 */
uint16_t sonata_ble_gatt_read_db_hash_cmd_handler(uint8_t conidx);

/*!
 * @brief Gatt send servcie changed event
 * @param conidx connecting index, used for multiple connection. for single connection, set it to 0.
 * @param handle handle id
 * @param start_handle Start handle
 * @param end_handle End handle
 * @return API_SUCCESS
 */
uint16_t sonata_ble_gatt_send_service_changed_event(uint8_t conidx, uint16_t handle, uint16_t start_handle,
        uint16_t end_handle);

/*!
 * @brief Gatt send event confirm message
 * @param conidx connecting index, used for multiple connection. for single connection, set it to 0.
 * @param handle handle id
 * @return API_SUCCESS
 */
uint16_t sonata_ble_gatt_send_event_confirm(uint8_t conidx, uint16_t handle);

/*!
 * @brief Gatt send att info confirm message
 * @param conidx connecting index, used for multiple connection. for single connection, set it to 0.
 * @param handle handle id
 * @param length Current length of the attribute
 * @param status use to know if it's possible to modify the attribute can contains authorization or application error code.
 * @return API_SUCCESS
 */
uint16_t sonata_ble_gatt_send_att_info_confirm(uint8_t conidx, uint16_t handle, uint16_t length, uint8_t status);

/*!
 * @brief Gatt send service changed confirm message
 * @param conidx connecting index, used for multiple connection. for single connection, set it to 0.
 * @param handle handle id
 * @return API_SUCCESS
 */
uint16_t sonata_ble_gatt_send_svc_change_confirm(uint8_t conidx, uint16_t handle);

/*!
 * @brief Add gatt service request
 * @param start_hdl Service start handle Attribute Start Handle (0 = dynamically allocated)
 * @param perm   |SEC |UUID_LEN |DIS |  AUTH   |EKS | MI |
 * @param uuid Service  UUID
 * @param nb_att Number of attributes
 * @param atts List of attribute description present in service.
 * @return API_SUCCESS
 */
uint16_t sonata_ble_gatt_add_service(uint16_t start_hdl, uint8_t perm, uint8_t *uuid,
                                     uint8_t nb_att, sonata_gatt_att_desc_t *atts);

/// @deprecated use sonata_ble_gatt_add_service()
uint16_t sonata_ble_gatt_add_service_request(uint16_t start_hdl, uint8_t perm, uint8_t *uuid,
        uint8_t nb_att, sonata_gatt_att_desc_t *atts);

/*!
 * @brief Gatt get value
 * @param handle handle id
 * @return API_SUCCESS
 */
uint16_t sonata_ble_gatt_att_get_value(uint16_t handle);
/*!
 * @brief Gatt set value
 * @param handle handle id
 * @param length Attribute value length
 * @param value Attribute value
 * @return API_SUCCESS
 */
uint16_t sonata_ble_gatt_att_set_value(uint16_t handle, uint16_t length, uint8_t *value);
/*!
 * @brief Gatt destory database(for debug)
 * @param gap_hdl New Gap Start Handle
 * @param gatt_hdl New Gatt Start Handle
 * @return API_SUCCESS
 */
uint16_t sonata_ble_gatt_destroy_db(uint16_t gap_hdl, uint16_t gatt_hdl);
/*!
 * @brief Gatt get service list
 * @param status
 * @param nb_svc Number of services
 * @param svc Array of information about services
 * @return API_SUCCESS
 */
uint16_t sonata_ble_gatt_get_service_list(uint8_t status, uint8_t nb_svc, sonata_gatt_svc_info_t *svc);
/*!
 * @brief Gatt get information
 * @param handle handle id
 * @return API_SUCCESS
 */
uint16_t sonata_ble_gatt_att_get_info(uint16_t handle);
/*!
 * @brief Gatt set visibility of a service
 * @param handle handle id
 * @param visible True to set service visible, false to hide it
 * @return API_SUCCESS
 */
uint16_t sonata_ble_gatt_set_service_visibility(uint16_t handle, bool visible);
/*!
 * @brief Gatt get database hash
 * @return API_SUCCESS
 */
uint16_t sonata_ble_gatt_att_compute_db_hash(void);
/*!
 * @brief GATT send read confirm message
 * @param conidx connecting index, used for multiple connection. for single connection, set it to 0.
 * @param handle gatt att handle id
 * @param status status
 * @param length value length
 * @param value value array
 * @return API_SUCCESS
 */
uint16_t sonata_ble_gatt_send_read_confirm(uint8_t conidx, uint16_t handle, uint8_t status, uint16_t length,
        uint8_t *value);

/*!
 * @brief GATT send write confirm message
 * @param conidx connecting index, used for multiple connection. for single connection, set it to 0.
 * @param handle gatt att handle id
 * @param status status
 * @return API_SUCCESS
 */
uint16_t sonata_ble_gatt_send_write_confirm(uint8_t conidx, uint16_t handle, uint8_t status);

/*!
 * @brief Register GATT callback functions for ble module
 * @param cb @see ble_gatt_callback
 * @return API_SUCCESS
 */
uint16_t sonata_ble_register_gatt_callback(ble_gatt_callback *cb);

/** @}*/

#endif // BLE_HOST_PRESENT

#endif // _SONATA_GATT_API_H_

