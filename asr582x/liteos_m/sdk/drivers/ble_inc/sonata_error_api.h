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
 * @file sonata_error_api.h
 *
 * @brief header file - this file contains the ble error code definition
 *
 ****************************************************************************************
 */

#ifndef _SONATA_ERROR_API_H_
#define _SONATA_ERROR_API_H_

/*
 * INCLUDE FILES
 ****************************************************************************************
 */

/*
 * MACRO DEFINITIONS
 ****************************************************************************************
 */
/// error code - all module base code
#define API_SUCCESS                 0x0000
#define API_FAILURE                 0xFFFF
#define SONATA_BLE_API_ERR_ID          0x0100
#define SONATA_API_TASK_ERR_ID         0x0200
#define SONATA_GAP_API_ERR_ID          0x0300
#define SONATA_GATT_API_ERR_ID         0x0400
#define SONATA_LL_API_ERR_ID           0x0500
#define SONATA_SEC_API_ERR_ID          0x0600
#define SONATA_MESH_API_ERR_ID         0x1100
#define SONATA_DIS_API_ERR_ID          0x1200
#define SONATA_PROX_API_ERR_ID         0x1300
#define SONATA_OTA_API_ERR_ID          0x1400
#define SONATA_BAS_API_ERR_ID          0x1500

#define SONATA_NVDS_API_ERR_ID         0x2100
#define SONATA_SLEEP_API_ERR_ID        0x2200
#define SONATA_UTILS_API_ERR_ID        0x2300

/// asr ble api error code

/// asr api task error code

/// asr gap api error code
#define SONATA_GAP_ERR_NO_ERROR                      (0X00 | SONATA_GAP_API_ERR_ID)
#define SONATA_GAP_ERR_INVALID_PARAM                 (0x40 | SONATA_GAP_API_ERR_ID)
#define SONATA_GAP_ERR_PROTOCOL_PROBLEM              (0x41 | SONATA_GAP_API_ERR_ID)
#define SONATA_GAP_ERR_NOT_SUPPORTED                 (0x42 | SONATA_GAP_API_ERR_ID)
#define SONATA_GAP_ERR_COMMAND_DISALLOWED            (0x43 | SONATA_GAP_API_ERR_ID)
#define SONATA_GAP_ERR_CANCELED                      (0x44 | SONATA_GAP_API_ERR_ID)
#define SONATA_GAP_ERR_TIMEOUT                       (0x45 | SONATA_GAP_API_ERR_ID)
#define SONATA_GAP_ERR_DISCONNECTED                  (0x46 | SONATA_GAP_API_ERR_ID)
#define SONATA_GAP_ERR_NOT_FOUND                     (0x47 | SONATA_GAP_API_ERR_ID)
#define SONATA_GAP_ERR_REJECTED                      (0x48 | SONATA_GAP_API_ERR_ID)
#define SONATA_GAP_ERR_PRIVACY_CFG_PB                (0x49 | SONATA_GAP_API_ERR_ID)
#define SONATA_GAP_ERR_ADV_DATA_INVALID              (0x4A | SONATA_GAP_API_ERR_ID)
#define SONATA_GAP_ERR_INSUFF_RESOURCES              (0x4B | SONATA_GAP_API_ERR_ID)
#define SONATA_GAP_ERR_UNEXPECTED                    (0x4C | SONATA_GAP_API_ERR_ID)
#define SONATA_GAP_ERR_MISMATCH                      (0x4D | SONATA_GAP_API_ERR_ID)

/// asr gatt api error code
#define SONATA_GATT_ERR_NO_ERROR                      (0X00 | SONATA_GATT_API_ERR_ID)
#define SONATA_GATT_ERR_INVALID_PARAM                 (0x40 | SONATA_GATT_API_ERR_ID)
/// asr ll api error code

/// asr sec api error code

/// asr mesh api error code

/// asr dis api error code
#define SONATA_PRF_DIS_ERR_NO_ERROR                      (0X00 | SONATA_DIS_API_ERR_ID)
#define SONATA_PRF_DIS_ERR_INVALID_PARAM                 (0x40 | SONATA_DIS_API_ERR_ID)
/// asr bat api error code
#define SONATA_PRF_BAS_ERR_NO_ERROR                      (0X00 | SONATA_BAS_API_ERR_ID)
#define SONATA_PRF_BAS_ERR_INVALID_PARAM                 (0x40 | SONATA_BAS_API_ERR_ID)
/// asr prox api error code

/// asr ota api error code

/// asr nvds api error code

/// asr sleep api error code

/// asr utils api error code

/// debug trace
#ifdef SONATA_UTILS_API_DBG
#define SONATA_UTILS_API_TRC    printf
#else
#define SONATA_UTILS_API_TRC(...)
#endif // SONATA_API_TASK_DBG

/*
 * ENUM DEFINITIONS
 ****************************************************************************************
 */

// C error
#define SONATA_CO_ERROR_NO_ERROR                        (0x00)
#define SONATA_CO_ERROR_UNKNOWN_HCI_COMMAND             (0x01)
#define SONATA_CO_ERROR_UNKNOWN_CONNECTION_ID           (0x02)
#define SONATA_CO_ERROR_HARDWARE_FAILURE                (0x03)
#define SONATA_CO_ERROR_PAGE_TIMEOUT                    (0x04)
#define SONATA_CO_ERROR_AUTH_FAILURE                    (0x05)
#define SONATA_CO_ERROR_PIN_MISSING                     (0x06)
#define SONATA_CO_ERROR_MEMORY_CAPA_EXCEED              (0x07)
#define SONATA_CO_ERROR_CON_TIMEOUT                     (0x08)
#define SONATA_CO_ERROR_CON_LIMIT_EXCEED                (0x09)
#define SONATA_CO_ERROR_SYNC_CON_LIMIT_DEV_EXCEED       (0x0A)
#define SONATA_CO_ERROR_CON_ALREADY_EXISTS              (0x0B)
#define SONATA_CO_ERROR_COMMAND_DISALLOWED              (0x0C)
#define SONATA_CO_ERROR_CONN_REJ_LIMITED_RESOURCES      (0x0D)
#define SONATA_CO_ERROR_CONN_REJ_SECURITY_REASONS       (0x0E)
#define SONATA_CO_ERROR_CONN_REJ_UNACCEPTABLE_BDADDR    (0x0F)
#define SONATA_CO_ERROR_CONN_ACCEPT_TIMEOUT_EXCEED      (0x10)
#define SONATA_CO_ERROR_UNSUPPORTED                     (0x11)
#define SONATA_CO_ERROR_INVALID_HCI_PARAM               (0x12)
#define SONATA_CO_ERROR_REMOTE_USER_TERM_CON            (0x13)
#define SONATA_CO_ERROR_REMOTE_DEV_TERM_LOW_RESOURCES   (0x14)
#define SONATA_CO_ERROR_REMOTE_DEV_POWER_OFF            (0x15)
#define SONATA_CO_ERROR_CON_TERM_BY_LOCAL_HOST          (0x16)
#define SONATA_CO_ERROR_REPEATED_ATTEMPTS               (0x17)
#define SONATA_CO_ERROR_PAIRING_NOT_ALLOWED             (0x18)
#define SONATA_CO_ERROR_UNKNOWN_LMP_PDU                 (0x19)
#define SONATA_CO_ERROR_UNSUPPORTED_REMOTE_FEATURE      (0x1A)
#define SONATA_CO_ERROR_SCO_OFFSET_REJECTED             (0x1B)
#define SONATA_CO_ERROR_SCO_INTERVAL_REJECTED           (0x1C)
#define SONATA_CO_ERROR_SCO_AIR_MODE_REJECTED           (0x1D)
#define SONATA_CO_ERROR_INVALID_LMP_PARAM               (0x1E)
#define SONATA_CO_ERROR_UNSPECIFIED_ERROR               (0x1F)
#define SONATA_CO_ERROR_UNSUPPORTED_LMP_PARAM_VALUE     (0x20)
#define SONATA_CO_ERROR_ROLE_CHANGE_NOT_ALLOWED         (0x21)
#define SONATA_CO_ERROR_LMP_RSP_TIMEOUT                 (0x22)
#define SONATA_CO_ERROR_LMP_COLLISION                   (0x23)
#define SONATA_CO_ERROR_LMP_PDU_NOT_ALLOWED             (0x24)
#define SONATA_CO_ERROR_ENC_MODE_NOT_ACCEPT             (0x25)
#define SONATA_CO_ERROR_LINK_KEY_CANT_CHANGE            (0x26)
#define SONATA_CO_ERROR_QOS_NOT_SUPPORTED               (0x27)
#define SONATA_CO_ERROR_INSTANT_PASSED                  (0x28)
#define SONATA_CO_ERROR_PAIRING_WITH_UNIT_KEY_NOT_SUP   (0x29)
#define SONATA_CO_ERROR_DIFF_TRANSACTION_COLLISION      (0x2A)
#define SONATA_CO_ERROR_QOS_UNACCEPTABLE_PARAM          (0x2C)
#define SONATA_CO_ERROR_QOS_REJECTED                    (0x2D)
#define SONATA_CO_ERROR_CHANNEL_CLASS_NOT_SUP           (0x2E)
#define SONATA_CO_ERROR_INSUFFICIENT_SECURITY           (0x2F)
#define SONATA_CO_ERROR_PARAM_OUT_OF_MAND_RANGE         (0x30)
#define SONATA_CO_ERROR_ROLE_SWITCH_PEND                (0x32)
#define SONATA_CO_ERROR_RESERVED_SLOT_VIOLATION         (0x34)
#define SONATA_CO_ERROR_ROLE_SWITCH_FAIL                (0x35)
#define SONATA_CO_ERROR_EIR_TOO_LARGE                   (0x36)
#define SONATA_CO_ERROR_SP_NOT_SUPPORTED_HOST           (0x37)
#define SONATA_CO_ERROR_HOST_BUSY_PAIRING               (0x38)
#define SONATA_CO_ERROR_CONTROLLER_BUSY                 (0x3A)
#define SONATA_CO_ERROR_UNACCEPTABLE_CONN_PARAM         (0x3B)
#define SONATA_CO_ERROR_ADV_TO                          (0x3C)
#define SONATA_CO_ERROR_TERMINATED_MIC_FAILURE          (0x3D)
#define SONATA_CO_ERROR_CONN_FAILED_TO_BE_EST           (0x3E)
#define SONATA_CO_ERROR_CCA_REJ_USE_CLOCK_DRAG          (0x40)
#define SONATA_CO_ERROR_TYPE0_SUBMAP_NOT_DEFINED        (0x41)
#define SONATA_CO_ERROR_UNKNOWN_ADVERTISING_ID          (0x42)
#define SONATA_CO_ERROR_LIMIT_REACHED                   (0x43)
#define SONATA_CO_ERROR_OPERATION_CANCELED_BY_HOST      (0x44)
#define SONATA_CO_ERROR_PKT_TOO_LONG                    (0x45)
#define SONATA_CO_ERROR_UNDEFINED                       (0xFF)
#define SONATA_CO_ERROR_HW_UART_OUT_OF_SYNC             (0x00)
#define SONATA_CO_ERROR_HW_MEM_ALLOC_FAIL               (0x01)
// C error end

// hl_err
enum stack_hl_err {
    /// No error
    SONATA_HL_GAP_ERR_NO_ERROR                                                               = 0x00,

    // ----------------------------------------------------------------------------------
    // -------------------------  ATT Specific Error ------------------------------------
    // ----------------------------------------------------------------------------------
    /// No error
    SONATA_HL_ATT_ERR_NO_ERROR = 0x00,
    /// 0x01: Handle is invalid
    SONATA_HL_ATT_ERR_INVALID_HANDLE = 0x01,
    /// 0x02: Read permission disabled
    SONATA_HL_ATT_ERR_READ_NOT_PERMITTED = 0x02,
    /// 0x03: Write permission disabled
    SONATA_HL_ATT_ERR_WRITE_NOT_PERMITTED = 0x03,
    /// 0x04: Incorrect PDU
    SONATA_HL_ATT_ERR_INVALID_PDU = 0x04,
    /// 0x05: Authentication privilege not enough
    SONATA_HL_ATT_ERR_INSUFF_AUTHEN = 0x05,
    /// 0x06: Request not supported or not understood
    SONATA_HL_ATT_ERR_REQUEST_NOT_SUPPORTED = 0x06,
    /// 0x07: Incorrect offset value
    SONATA_HL_ATT_ERR_INVALID_OFFSET = 0x07,
    /// 0x08: Authorization privilege not enough
    SONATA_HL_ATT_ERR_INSUFF_AUTHOR = 0x08,
    /// 0x09: Capacity queue for reliable write reached
    SONATA_HL_ATT_ERR_PREPARE_QUEUE_FULL = 0x09,
    /// 0x0A: Attribute requested not existing
    SONATA_HL_ATT_ERR_ATTRIBUTE_NOT_FOUND                                                    = 0x0A,
    /// 0x0B: Attribute requested not long
    SONATA_HL_ATT_ERR_ATTRIBUTE_NOT_LONG = 0x0B,
    /// 0x0C: Encryption size not sufficient
    SONATA_HL_ATT_ERR_INSUFF_ENC_KEY_SIZE = 0x0C,
    /// 0x0D: Invalid length of the attribute value
    SONATA_HL_ATT_ERR_INVALID_ATTRIBUTE_VAL_LEN = 0x0D,
    /// 0x0E: Operation not fit to condition
    SONATA_HL_ATT_ERR_UNLIKELY_ERR = 0x0E,
    /// 0x0F: Attribute requires encryption before operation
    SONATA_HL_ATT_ERR_INSUFF_ENC = 0x0F,
    /// 0x10: Attribute grouping not supported
    SONATA_HL_ATT_ERR_UNSUPP_GRP_TYPE = 0x10,
    /// 0x11: Resources not sufficient to complete the request
    SONATA_HL_ATT_ERR_INSUFF_RESOURCE = 0x11,
    /// 0x12: The server requests the client to rediscover the database.
    SONATA_HL_ATT_ERR_DB_OUT_OF_SYNC = 0x12,
    /// 0x13: The attribute parameter value was not allowed.
    SONATA_HL_ATT_ERR_VALUE_NOT_ALLOWED = 0x13,
    /// 0x80: Application error (also used in PRF Errors)
    SONATA_HL_ATT_ERR_APP_ERROR = 0x80,

    // ----------------------------------------------------------------------------------
    // -------------------------- L2C Specific Error ------------------------------------
    // ----------------------------------------------------------------------------------
    /// Message cannot be sent because connection lost. (disconnected)
    SONATA_HL_L2C_ERR_CONNECTION_LOST = 0x30,
    /// Invalid PDU length exceed MTU
    SONATA_HL_L2C_ERR_INVALID_MTU_EXCEED = 0x31,
    /// Invalid PDU length exceed MPS
    SONATA_HL_L2C_ERR_INVALID_MPS_EXCEED = 0x32,
    /// Invalid Channel ID
    SONATA_HL_L2C_ERR_INVALID_CID = 0x33,
    /// Invalid PDU
    SONATA_HL_L2C_ERR_INVALID_PDU = 0x34,
    /// Connection refused - no resources available
    SONATA_HL_L2C_ERR_NO_RES_AVAIL = 0x35,
    /// Connection refused - insufficient authentication
    SONATA_HL_L2C_ERR_INSUFF_AUTHEN = 0x36,
    /// Connection refused - insufficient authorization
    SONATA_HL_L2C_ERR_INSUFF_AUTHOR = 0x37,
    /// Connection refused - insufficient encryption key size
    SONATA_HL_L2C_ERR_INSUFF_ENC_KEY_SIZE = 0x38,
    /// Connection Refused - insufficient encryption
    SONATA_HL_L2C_ERR_INSUFF_ENC = 0x39,
    /// Connection refused - LE_PSM not supported
    SONATA_HL_L2C_ERR_LEPSM_NOT_SUPP = 0x3A,
    /// No more credit
    SONATA_HL_L2C_ERR_INSUFF_CREDIT = 0x3B,
    /// Command not understood by peer device
    SONATA_HL_L2C_ERR_NOT_UNDERSTOOD = 0x3C,
    /// Credit error, invalid number of credit received
    SONATA_HL_L2C_ERR_CREDIT_ERROR = 0x3D,
    /// Channel identifier already allocated
    SONATA_HL_L2C_ERR_CID_ALREADY_ALLOC = 0x3E,
    /// Unknown PDU
    SONATA_HL_L2C_ERR_UNKNOWN_PDU = 0x3F,

    // ----------------------------------------------------------------------------------
    // -------------------------- GAP Specific Error ------------------------------------
    // ----------------------------------------------------------------------------------
    /// Invalid parameters set
    SONATA_HL_GAP_ERR_INVALID_PARAM = 0x40,
    /// Problem with protocol exchange, get unexpected response
    SONATA_HL_GAP_ERR_PROTOCOL_PROBLEM = 0x41,
    /// Request not supported by software configuration
    SONATA_HL_GAP_ERR_NOT_SUPPORTED = 0x42,
    /// Request not allowed in current state.
    SONATA_HL_GAP_ERR_COMMAND_DISALLOWED = 0x43,
    /// Requested operation canceled.
    SONATA_HL_GAP_ERR_CANCELED = 0x44,
    /// Requested operation timeout.
    SONATA_HL_GAP_ERR_TIMEOUT = 0x45,
    /// Link connection lost during operation.
    SONATA_HL_GAP_ERR_DISCONNECTED = 0x46,
    /// Search algorithm finished, but no result found
    SONATA_HL_GAP_ERR_NOT_FOUND = 0x47,
    /// Request rejected by peer device
    SONATA_HL_GAP_ERR_REJECTED = 0x48,
    /// Problem with privacy configuration
    SONATA_HL_GAP_ERR_PRIVACY_CFG_PB = 0x49,
    /// Duplicate or invalid advertising data
    SONATA_HL_GAP_ERR_ADV_DATA_INVALID = 0x4A,
    /// Insufficient resources
    SONATA_HL_GAP_ERR_INSUFF_RESOURCES = 0x4B,
    /// Unexpected Error
    SONATA_HL_GAP_ERR_UNEXPECTED = 0x4C,
    /// Feature mismatch
    SONATA_HL_GAP_ERR_MISMATCH = 0x4D,

    // ----------------------------------------------------------------------------------
    // ------------------------- GATT Specific Error ------------------------------------
    // ----------------------------------------------------------------------------------
    /// Problem with ATTC protocol response
    SONATA_HL_GATT_ERR_INVALID_ATT_LEN = 0x50,
    /// Error in service search
    SONATA_HL_GATT_ERR_INVALID_TYPE_IN_SVC_SEARCH = 0x51,
    /// Invalid write data
    SONATA_HL_GATT_ERR_WRITE = 0x52,
    /// Signed write error
    SONATA_HL_GATT_ERR_SIGNED_WRITE = 0x53,
    /// No attribute client defined
    SONATA_HL_GATT_ERR_ATTRIBUTE_CLIENT_MISSING = 0x54,
    /// No attribute server defined
    SONATA_HL_GATT_ERR_ATTRIBUTE_SERVER_MISSING = 0x55,
    /// Permission set in service/attribute are invalid
    SONATA_HL_GATT_ERR_INVALID_PERM = 0x56,

    // ----------------------------------------------------------------------------------
    // ------------------------- SMP Specific Error -------------------------------------
    // ----------------------------------------------------------------------------------
    // SMP Protocol Errors detected on local device
    /// The user input of pass key failed, for example, the user canceled the operation.
    SONATA_HL_SMP_ERROR_LOC_PASSKEY_ENTRY_FAILED = 0x61,
    /// The OOB Data is not available.
    SONATA_HL_SMP_ERROR_LOC_OOB_NOT_AVAILABLE = 0x62,
    /// The pairing procedure cannot be performed as authentication requirements cannot be met
    /// due to IO capabilities of one or both devices.
    SONATA_HL_SMP_ERROR_LOC_AUTH_REQ = 0x63,
    /// The confirm value does not match the calculated confirm value.
    SONATA_HL_SMP_ERROR_LOC_CONF_VAL_FAILED = 0x64,
    /// Pairing is not supported by the device.
    SONATA_HL_SMP_ERROR_LOC_PAIRING_NOT_SUPP = 0x65,
    /// The resultant encryption key size is insufficient for the security requirements of
    /// this device.
    SONATA_HL_SMP_ERROR_LOC_ENC_KEY_SIZE = 0x66,
    /// The SMP command received is not supported on this device.
    SONATA_HL_SMP_ERROR_LOC_CMD_NOT_SUPPORTED = 0x67,
    /// Pairing failed due to an unspecified reason.
    SONATA_HL_SMP_ERROR_LOC_UNSPECIFIED_REASON = 0x68,
    /// Pairing or Authentication procedure is disallowed because too little time has elapsed
    /// since last pairing request or security request.
    SONATA_HL_SMP_ERROR_LOC_REPEATED_ATTEMPTS = 0x69,
    /// The command length is invalid or a parameter is outside of the specified range.
    SONATA_HL_SMP_ERROR_LOC_INVALID_PARAM = 0x6A,
    /// Indicates to the remote device that the DHKey Check value received doesn't
    /// match the one calculated by the local device.
    SONATA_HL_SMP_ERROR_LOC_DHKEY_CHECK_FAILED = 0x6B,
    /// Indicates that the confirm values in the numeric comparison protocol do not match.
    SONATA_HL_SMP_ERROR_LOC_NUMERIC_COMPARISON_FAILED = 0x6C,
    /// Indicates that the pairing over the LE transport failed due to a Pairing Request sent
    /// over the BR/EDR transport in process.
    SONATA_HL_SMP_ERROR_LOC_BREDR_PAIRING_IN_PROGRESS = 0x6D,
    /// Indicates that the BR/EDR Link Key generated on the BR/EDR transport cannot be
    /// used to derive and distribute keys for the LE transport.
    SONATA_HL_SMP_ERROR_LOC_CROSS_TRANSPORT_KEY_GENERATION_NOT_ALLOWED = 0x6E,
    // SMP Protocol Errors detected by remote device
    /// The user input of passkey failed, for example, the user canceled the operation.
    SONATA_HL_SMP_ERROR_REM_PASSKEY_ENTRY_FAILED = 0x71,
    /// The OOB Data is not available.
    SONATA_HL_SMP_ERROR_REM_OOB_NOT_AVAILABLE = 0x72,
    /// The pairing procedure cannot be performed as authentication requirements cannot be
    /// met due to IO capabilities of one or both devices.
    SONATA_HL_SMP_ERROR_REM_AUTH_REQ = 0x73,
    /// The confirm value does not match the calculated confirm value.
    SONATA_HL_SMP_ERROR_REM_CONF_VAL_FAILED = 0x74,
    /// Pairing is not supported by the device.
    SONATA_HL_SMP_ERROR_REM_PAIRING_NOT_SUPP = 0x75,
    /// The resultant encryption key size is insufficient for the security requirements of
    /// this device.
    SONATA_HL_SMP_ERROR_REM_ENC_KEY_SIZE = 0x76,
    /// The SMP command received is not supported on this device.
    SONATA_HL_SMP_ERROR_REM_CMD_NOT_SUPPORTED = 0x77,
    /// Pairing failed due to an unspecified reason.
    SONATA_HL_SMP_ERROR_REM_UNSPECIFIED_REASON = 0x78,
    /// Pairing or Authentication procedure is disallowed because too little time has elapsed
    /// since last pairing request or security request.
    SONATA_HL_SMP_ERROR_REM_REPEATED_ATTEMPTS = 0x79,
    /// The command length is invalid or a parameter is outside of the specified range.
    SONATA_HL_SMP_ERROR_REM_INVALID_PARAM = 0x7A,
    /// Indicates to the remote device that the DHKey Check value received doesn't
    /// match the one calculated by the local device.
    SONATA_HL_SMP_ERROR_REM_DHKEY_CHECK_FAILED = 0x7B,
    /// Indicates that the confirm values in the numeric comparison protocol do not match.
    SONATA_HL_SMP_ERROR_REM_NUMERIC_COMPARISON_FAILED = 0x7C,
    /// Indicates that the pairing over the LE transport failed due to a Pairing Request sent
    /// over the BR/EDR transport in process.
    SONATA_HL_SMP_ERROR_REM_BREDR_PAIRING_IN_PROGRESS = 0x7D,
    /// Indicates that the BR/EDR Link Key generated on the BR/EDR transport cannot be
    /// used to derive and distribute keys for the LE transport.
    SONATA_HL_SMP_ERROR_REM_CROSS_TRANSPORT_KEY_GENERATION_NOT_ALLOWED = 0x7E,
    // SMP Errors triggered by local device
    /// The provided resolvable address has not been resolved.
    SONATA_HL_SMP_ERROR_ADDR_RESOLV_FAIL = 0x20,
    /// The Signature Verification Failed
    SONATA_HL_SMP_ERROR_SIGN_VERIF_FAIL = 0x21,
    /// The encryption procedure failed because the slave device didn't find the LTK
    /// needed to start an encryption session.
    SONATA_HL_SMP_ERROR_ENC_KEY_MISSING = 0x22,
    /// The encryption procedure failed because the slave device doesn't support the
    /// encryption feature.
    SONATA_HL_SMP_ERROR_ENC_NOT_SUPPORTED = 0x23,
    /// A timeout has occurred during the start encryption session.
    SONATA_HL_SMP_ERROR_ENC_TIMEOUT = 0x24,

    // ----------------------------------------------------------------------------------
    //------------------------ Profiles specific error codes ----------------------------
    // ----------------------------------------------------------------------------------
    /// Application Error
    SONATA_HL_PRF_APP_ERROR = 0x80,
    /// Invalid parameter in request
    SONATA_HL_PRF_ERR_INVALID_PARAM = 0x81,
    /// Inexistent handle for sending a read/write characteristic request
    SONATA_HL_PRF_ERR_INEXISTENT_HDL = 0x82,
    /// Discovery stopped due to missing attribute according to specification
    SONATA_HL_PRF_ERR_STOP_DISC_CHAR_MISSING = 0x83,
    /// Too many SVC instances found -> protocol violation
    SONATA_HL_PRF_ERR_MULTIPLE_SVC = 0x84,
    /// Discovery stopped due to found attribute with incorrect properties
    SONATA_HL_PRF_ERR_STOP_DISC_WRONG_CHAR_PROP = 0x85,
    /// Too many Char. instances found-> protocol violation
    SONATA_HL_PRF_ERR_MULTIPLE_CHAR = 0x86,
    /// Attribute write not allowed
    SONATA_HL_PRF_ERR_NOT_WRITABLE = 0x87,
    /// Attribute read not allowed
    SONATA_HL_PRF_ERR_NOT_READABLE = 0x88,
    /// Request not allowed
    SONATA_HL_PRF_ERR_REQ_DISALLOWED = 0x89,
    /// Notification Not Enabled
    SONATA_HL_PRF_ERR_NTF_DISABLED = 0x8A,
    /// Indication Not Enabled
    SONATA_HL_PRF_ERR_IND_DISABLED = 0x8B,
    /// Feature not supported by profile
    SONATA_HL_PRF_ERR_FEATURE_NOT_SUPPORTED = 0x8C,
    /// Read value has an unexpected length
    SONATA_HL_PRF_ERR_UNEXPECTED_LEN = 0x8D,
    /// Disconnection occurs
    SONATA_HL_PRF_ERR_DISCONNECTED = 0x8E,
    /// Procedure Timeout
    SONATA_HL_PRF_ERR_PROC_TIMEOUT = 0x8F,
    /// Client characteristic configuration improperly configured
    SONATA_HL_PRF_CCCD_IMPR_CONFIGURED = 0xFD,
    /// Procedure already in progress
    SONATA_HL_PRF_PROC_IN_PROGRESS = 0xFE,
    /// Out of Range
    SONATA_HL_PRF_OUT_OF_RANGE = 0xFF,

    // ----------------------------------------------------------------------------------
    //-------------------- LL Error codes conveyed to upper layer -----------------------
    // ----------------------------------------------------------------------------------
    /// Unknown HCI Command
    SONATA_HL_LL_ERR_UNKNOWN_HCI_COMMAND = 0x91,
    /// Unknown Connection Identifier
    SONATA_HL_LL_ERR_UNKNOWN_CONNECTION_ID = 0x92,
    /// Hardware Failure
    SONATA_HL_LL_ERR_HARDWARE_FAILURE = 0x93,
    /// BT Page Timeout
    SONATA_HL_LL_ERR_PAGE_TIMEOUT = 0x94,
    /// Authentication failure
    SONATA_HL_LL_ERR_AUTH_FAILURE = 0x95,
    /// Pin code missing
    SONATA_HL_LL_ERR_PIN_MISSING = 0x96,
    /// Memory capacity exceed
    SONATA_HL_LL_ERR_MEMORY_CAPA_EXCEED = 0x97,
    /// Connection Timeout
    SONATA_HL_LL_ERR_CON_TIMEOUT = 0x98,
    /// Connection limit Exceed
    SONATA_HL_LL_ERR_CON_LIMIT_EXCEED = 0x99,
    /// Synchronous Connection limit exceed
    SONATA_HL_LL_ERR_SYNC_CON_LIMIT_DEV_EXCEED = 0x9A,
    /// ACL Connection exits
    SONATA_HL_LL_ERR_ACL_CON_EXISTS = 0x9B,
    /// Command Disallowed
    SONATA_HL_LL_ERR_COMMAND_DISALLOWED = 0x9C,
    /// Connection rejected due to limited resources
    SONATA_HL_LL_ERR_CONN_REJ_LIMITED_RESOURCES = 0x9D,
    /// Connection rejected due to security reason
    SONATA_HL_LL_ERR_CONN_REJ_SECURITY_REASONS = 0x9E,
    /// Connection rejected due to unacceptable BD Addr
    SONATA_HL_LL_ERR_CONN_REJ_UNACCEPTABLE_BDADDR = 0x9F,
    /// Connection rejected due to Accept connection timeout
    SONATA_HL_LL_ERR_CONN_ACCEPT_TIMEOUT_EXCEED = 0xA0,
    /// Not Supported
    SONATA_HL_LL_ERR_UNSUPPORTED = 0xA1,
    /// invalid parameters
    SONATA_HL_LL_ERR_INVALID_HCI_PARAM = 0xA2,
    /// Remote user terminate connection
    SONATA_HL_LL_ERR_REMOTE_USER_TERM_CON = 0xA3,
    /// Remote device terminate connection due to low resources
    SONATA_HL_LL_ERR_REMOTE_DEV_TERM_LOW_RESOURCES = 0xA4,
    /// Remote device terminate connection due to power off
    SONATA_HL_LL_ERR_REMOTE_DEV_POWER_OFF = 0xA5,
    /// Connection terminated by local host
    SONATA_HL_LL_ERR_CON_TERM_BY_LOCAL_HOST = 0xA6,
    /// Repeated attempts
    SONATA_HL_LL_ERR_REPEATED_ATTEMPTS = 0xA7,
    /// Pairing not Allowed
    SONATA_HL_LL_ERR_PAIRING_NOT_ALLOWED = 0xA8,
    /// Unknown PDU Error
    SONATA_HL_LL_ERR_UNKNOWN_LMP_PDU = 0xA9,
    /// Unsupported remote feature
    SONATA_HL_LL_ERR_UNSUPPORTED_REMOTE_FEATURE = 0xAA,
    /// Sco Offset rejected
    SONATA_HL_LL_ERR_SCO_OFFSET_REJECTED = 0xAB,
    /// SCO Interval Rejected
    SONATA_HL_LL_ERR_SCO_INTERVAL_REJECTED = 0xAC,
    /// SCO air mode Rejected
    SONATA_HL_LL_ERR_SCO_AIR_MODE_REJECTED = 0xAD,
    /// Invalid LMP parameters
    SONATA_HL_LL_ERR_INVALID_LMP_PARAM = 0xAE,
    /// Unspecified error
    SONATA_HL_LL_ERR_UNSPECIFIED_ERROR = 0xAF,
    /// Unsupported LMP Parameter value
    SONATA_HL_LL_ERR_UNSUPPORTED_LMP_PARAM_VALUE = 0xB0,
    /// Role Change Not allowed
    SONATA_HL_LL_ERR_ROLE_CHANGE_NOT_ALLOWED = 0xB1,
    /// LMP Response timeout
    SONATA_HL_LL_ERR_LMP_RSP_TIMEOUT = 0xB2,
    /// LMP Collision
    SONATA_HL_LL_ERR_LMP_COLLISION = 0xB3,
    /// LMP Pdu not allowed
    SONATA_HL_LL_ERR_LMP_PDU_NOT_ALLOWED = 0xB4,
    /// Encryption mode not accepted
    SONATA_HL_LL_ERR_ENC_MODE_NOT_ACCEPT = 0xB5,
    /// Link Key Cannot be changed
    SONATA_HL_LL_ERR_LINK_KEY_CANT_CHANGE = 0xB6,
    /// Quality of Service not supported
    SONATA_HL_LL_ERR_QOS_NOT_SUPPORTED = 0xB7,
    /// Error, instant passed
    SONATA_HL_LL_ERR_INSTANT_PASSED = 0xB8,
    /// Pairing with unit key not supported
    SONATA_HL_LL_ERR_PAIRING_WITH_UNIT_KEY_NOT_SUP = 0xB9,
    /// Transaction collision
    SONATA_HL_LL_ERR_DIFF_TRANSACTION_COLLISION = 0xBA,
    /// Unacceptable parameters
    SONATA_HL_LL_ERR_QOS_UNACCEPTABLE_PARAM = 0xBC,
    /// Quality of Service rejected
    SONATA_HL_LL_ERR_QOS_REJECTED = 0xBD,
    /// Channel class not supported
    SONATA_HL_LL_ERR_CHANNEL_CLASS_NOT_SUP = 0xBE,
    /// Insufficient security
    SONATA_HL_LL_ERR_INSUFFICIENT_SECURITY = 0xBF,
    /// Parameters out of mandatory range
    SONATA_HL_LL_ERR_PARAM_OUT_OF_MAND_RANGE = 0xC0,
    /// Role switch pending
    SONATA_HL_LL_ERR_ROLE_SWITCH_PEND = 0xC2,
    /// Reserved slot violation
    SONATA_HL_LL_ERR_RESERVED_SLOT_VIOLATION = 0xC4,
    /// Role Switch fail
    SONATA_HL_LL_ERR_ROLE_SWITCH_FAIL = 0xC5,
    /// Error, EIR too large
    SONATA_HL_LL_ERR_EIR_TOO_LARGE = 0xC6,
    /// Simple pairing not supported by host
    SONATA_HL_LL_ERR_SP_NOT_SUPPORTED_HOST = 0xC7,
    /// Host pairing is busy
    SONATA_HL_LL_ERR_HOST_BUSY_PAIRING = 0xC8,
    /// Controller is busy
    SONATA_HL_LL_ERR_CONTROLLER_BUSY = 0xCA,
    /// Unacceptable connection initialization
    SONATA_HL_LL_ERR_UNACCEPTABLE_CONN_INT = 0xCB,
    /// Direct Advertising Timeout
    SONATA_HL_LL_ERR_DIRECT_ADV_TO = 0xCC,
    /// Connection Terminated due to a MIC failure
    SONATA_HL_LL_ERR_TERMINATED_MIC_FAILURE = 0xCD,
    /// Connection failed to be established
    SONATA_HL_LL_ERR_CONN_FAILED_TO_BE_EST = 0xCE,
    /// MAC Connection Failed
    SONATA_HL_LL_ERR_MAC_CONN_FAILED = 0xCF,
    /// Coarse Clock Adjustment Rejected but Will Try to Adjust Using Clock Dragging
    SONATA_HL_LL_ERR_CCA_REJ_USE_CLOCK_DRAG = 0xD0,
    /// Type0 Submap Not Defined
    SONATA_HL_LL_ERR_TYPE0_SUBMAP_NOT_DEFINED = 0xD1,
    /// Unknown Advertising Identifier
    SONATA_HL_LL_ERR_UNKNOWN_ADVERTISING_ID = 0xD2,
    /// Limit Reached
    SONATA_HL_LL_ERR_LIMIT_REACHED = 0xD3,
    /// Operation Cancelled by Host
    SONATA_HL_LL_ERR_OPERATION_CANCELED_BY_HOST = 0xD4,
    /// Packet Too Long
    SONATA_HL_LL_ERR_PKT_TOO_LONG = 0xD5,
};
// hl_err end
#endif // _SONATA_ERROR_API_H_

