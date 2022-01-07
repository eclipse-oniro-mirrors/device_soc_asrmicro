

#ifndef __SONATA_GATT_API_INT_H
#define __SONATA_GATT_API_INT_H

#include "att.h"
#include "gap.h"
#include "rwip_task.h" // Task definitions
#include "compiler.h"
#include <stdbool.h>
#include "ke_msg.h"
#include "gattc_task.h"
#include "sonata_gatt_api.h"

/*
 * MACRO DEFINITIONS
 ****************************************************************************************
 */
#define SONATA_GATT_DEBUG (1)
/*!
 * @brief Callback define for GATT complete message.
 */
typedef void (*GATT_COMPLETE_CB)(uint8_t conidx, uint16_t status);



enum sonata_gattc_msg_id
{
    /* Default event */
    /// Command Complete event
    SONATA_GATTC_CMP_EVT                   = MSG_ID(GATTC, 0x00),

    /* ATTRIBUTE CLIENT */
    /// Server configuration request
    SONATA_GATTC_EXC_MTU_CMD               = MSG_ID(GATTC, 0x01),
    /// Indicate that the ATT MTU has been updated (negotiated)
    SONATA_GATTC_MTU_CHANGED_IND           = MSG_ID(GATTC, 0x02),

    /*Discover All Services */
    /*Discover Services by Service UUID*/
    /*Find Included Services*/
    /*Discover Characteristics by UUID*/
    /*Discover All Characteristics of a Service*/
    /*Discover All Characteristic Descriptors*/
    /// Discovery command
    SONATA_GATTC_DISC_CMD                  = MSG_ID(GATTC, 0x03),
    /* GATT -> HL: Events to Upper layer */
    /*Discover All Services*/
    /// Discovery services indication
    SONATA_GATTC_DISC_SVC_IND              = MSG_ID(GATTC, 0x04),
    /*Find Included Services*/
    /// Discover included services indication
    SONATA_GATTC_DISC_SVC_INCL_IND         = MSG_ID(GATTC, 0x05),
    /*Discover All Characteristics of a Service*/
    /// Discover characteristic indication
    SONATA_GATTC_DISC_CHAR_IND             = MSG_ID(GATTC, 0x06),
    /*Discover All Characteristic Descriptors*/
    /// Discovery characteristic descriptor indication
    SONATA_GATTC_DISC_CHAR_DESC_IND        = MSG_ID(GATTC, 0x07),

    /*Read Value*/
    /*Read Using UUID*/
    /*Read Long Value*/
    /*Read Multiple Values*/
    /// Read command
    SONATA_GATTC_READ_CMD                  = MSG_ID(GATTC, 0x08),
    /// Read response
    SONATA_GATTC_READ_IND                  = MSG_ID(GATTC, 0x09),

    /*Write without response*/
    /*Write without response with Authentication*/
    /*Write Characteristic Value*/
    /*Signed Write Characteristic Value*/
    /*Write Long Characteristic Value*/
    /*Characteristic Value Reliable Write*/
    /*Write Characteristic Descriptors*/
    /*Write Long Characteristic Descriptors*/
    /*Characteristic Value Reliable Write*/
    /// Write command request
    SONATA_GATTC_WRITE_CMD                 = MSG_ID(GATTC, 0x0A),

    /* Cancel / Execute pending write operations */
    /// Execute write characteristic request
    SONATA_GATTC_EXECUTE_WRITE_CMD         = MSG_ID(GATTC, 0x0B),

    /* Reception of an indication or notification from peer device. */
    /// peer device triggers an event (notification)
    SONATA_GATTC_EVENT_IND                 = MSG_ID(GATTC, 0x0C),
    /// peer device triggers an event that requires a confirmation (indication)
    SONATA_GATTC_EVENT_REQ_IND             = MSG_ID(GATTC, 0x0D),
    /// Confirm reception of event (trigger a confirmation message)
    SONATA_GATTC_EVENT_CFM                 = MSG_ID(GATTC, 0x0E),

    /// Registration to peer device events (Indication/Notification).
    SONATA_GATTC_REG_TO_PEER_EVT_CMD       = MSG_ID(GATTC, 0x0F),

    /* -------------------------- ATTRIBUTE SERVER ------------------------------- */
    /*Notify Characteristic*/
    /*Indicate Characteristic*/
    /// send an event to peer device
    SONATA_GATTC_SEND_EVT_CMD              = MSG_ID(GATTC, 0x10),


    /* Indicate that read operation is requested. */
    /// Read command indicated to upper layers.
    SONATA_GATTC_READ_REQ_IND              = MSG_ID(GATTC, 0x13),
    /// REad command confirmation from upper layers.
    SONATA_GATTC_READ_CFM                  = MSG_ID(GATTC, 0x14),

    /* Indicate that write operation is requested. */
    /// Write command indicated to upper layers.
    SONATA_GATTC_WRITE_REQ_IND             = MSG_ID(GATTC, 0x15),
    /// Write command confirmation from upper layers.
    SONATA_GATTC_WRITE_CFM                 = MSG_ID(GATTC, 0x16),

    /* Indicate that write operation is requested. */
    /// Request Attribute info to upper layer - could be trigger during prepare write
    SONATA_GATTC_ATT_INFO_REQ_IND          = MSG_ID(GATTC, 0x17),
    /// Attribute info from upper layer confirmation
    SONATA_GATTC_ATT_INFO_CFM              = MSG_ID(GATTC, 0x18),

    /* ----------------------- SERVICE DISCOVERY PROCEDURE  --------------------------- */
    /// Service Discovery command
    SONATA_GATTC_SDP_SVC_DISC_CMD          = MSG_ID(GATTC, 0x19),
    /// Service Discovery indicate that a service has been found.
    SONATA_GATTC_SDP_SVC_IND               = MSG_ID(GATTC, 0x1A),

    /* -------------------------- TRANSACTION ERROR EVENT ----------------------------- */
    /// Transaction Timeout Error Event no more transaction will be accepted
    SONATA_GATTC_TRANSACTION_TO_ERROR_IND  = MSG_ID(GATTC, 0x1B),
    /// Indication to the task that sends the unknown message
    SONATA_GATTC_UNKNOWN_MSG_IND           = MSG_ID(GATTC, 0x1C),

    /* --------------------------------- ROBUST CACHE --------------------------------- */
    /// Command used to read peer database hash
    SONATA_GATTC_READ_DB_HASH_CMD          = MSG_ID(GATTC, 0x20),
    /// Provide value of the peer database hash
    SONATA_GATTC_DB_HASH_IND               = MSG_ID(GATTC, 0x21),
    /// Command used to enable Robust database caching
    /// (Register to service changed, Set client supported features and read database hash)
    SONATA_GATTC_ROBUST_DB_CACHE_EN_CMD    = MSG_ID(GATTC, 0x22),
    /// Informs that peer database updated and need to be refreshed
    /// This also means that no more indication/notification will be received till database hash isn't read
    SONATA_GATTC_DB_CACHE_OUT_OF_SYNC_IND  = MSG_ID(GATTC, 0x23),
    /// Informs that peer device database updated using service changed indication
    SONATA_GATTC_SVC_CHG_REQ_IND           = MSG_ID(GATTC, 0x24),
    /// Message used to confirm that database updated is understood by application
    SONATA_GATTC_SVC_CHG_CFM               = MSG_ID(GATTC, 0x25),

    /* ----------------------------------- GATT INFO ---------------------------------- */
    /// Provide information about GATT for current connection that can be reuse on another connection
    SONATA_GATTC_CON_INFO_IND              = MSG_ID(GATTC, 0x28),

    /* ------------------------------- INTERNAL API ----------------------------------- */
    /// Client Response timeout indication
    SONATA_GATTC_CLIENT_RTX_IND            = MSG_ID(GATTC, 0x40),
    /// Server indication confirmation timeout indication
    SONATA_GATTC_SERVER_RTX_IND            = MSG_ID(GATTC, 0x41),
};


/// request operation type - application interface
/*@TRACE*/
//enum sonata_gattc_operation
//{
//    /*              Attribute Client Flags              */
//    /* No Operation (if nothing has been requested)     */
//    /* ************************************************ */
//    /// No operation
//    SONATA_GATTC_NO_OP                                    = 0x00,
//
//    /* Operation flags for MTU Exchange                 */
//    /* ************************************************ */
//    /// Perform MTU exchange
//    SONATA_GATTC_MTU_EXCH                                 = 0x01,
//
//    /*      Operation flags for discovery operation     */
//    /* ************************************************ */
//    /// Discover all services
//    SONATA_GATTC_DISC_ALL_SVC                             = 0x02,
//    /// Discover services by UUID
//    SONATA_GATTC_DISC_BY_UUID_SVC                         = 0x03,
//    /// Discover included services
//    SONATA_GATTC_DISC_INCLUDED_SVC                        = 0x04,
//    /// Discover all characteristics
//    SONATA_GATTC_DISC_ALL_CHAR                            = 0x05,
//    /// Discover characteristic by UUID
//    SONATA_GATTC_DISC_BY_UUID_CHAR                        = 0x06,
//    /// Discover characteristic descriptor
//    SONATA_GATTC_DISC_DESC_CHAR                           = 0x07,
//
//    /* Operation flags for reading attributes           */
//    /* ************************************************ */
//    /// Read attribute
//    SONATA_GATTC_READ                                     = 0x08,
//    /// Read long attribute
//    SONATA_GATTC_READ_LONG                                = 0x09,
//    /// Read attribute by UUID
//    SONATA_GATTC_READ_BY_UUID                             = 0x0A,
//    /// Read multiple attribute
//    SONATA_GATTC_READ_MULTIPLE                            = 0x0B,
//
//    /* Operation flags for writing/modifying attributes */
//    /* ************************************************ */
//    /// Write attribute
//    SONATA_GATTC_WRITE                                    = 0x0C,
//    /// Write no response
//    SONATA_GATTC_WRITE_NO_RESPONSE                        = 0x0D,
//    /// Write signed
//    SONATA_GATTC_WRITE_SIGNED                             = 0x0E,
//    /// Execute write
//    SONATA_GATTC_EXEC_WRITE                               = 0x0F,
//
//    /* Operation flags for registering to peer device   */
//    /* events                                           */
//    /* ************************************************ */
//    /// Register to peer device events
//    SONATA_GATTC_REGISTER                                 = 0x10,
//    /// Unregister from peer device events
//    SONATA_GATTC_UNREGISTER                               = 0x11,
//
//    /* Operation flags for sending events to peer device*/
//    /* ************************************************ */
//    /// Send an attribute notification
//    SONATA_GATTC_NOTIFY                                   = 0x12,
//    /// Send an attribute indication
//    SONATA_GATTC_INDICATE                                 = 0x13,
//
//    /* Service Discovery Procedure                      */
//    /* ************************************************ */
//    /// Search specific service
//    SONATA_GATTC_SDP_DISC_SVC                             = 0x15,
//    /// Search for all services
//    SONATA_GATTC_SDP_DISC_SVC_ALL                         = 0x16,
//    /// Cancel Service Discovery Procedure
//    SONATA_GATTC_SDP_DISC_CANCEL                          = 0x17,
//
//    /* Robust Cache                                     */
//    /* ************************************************ */
//    /// Read peer database hash
//    SONATA_GATTC_READ_DB_HASH                             = 0x20,
//    /// Enable Robust database cache feature
//    SONATA_GATTC_ROBUST_DB_CACHE_EN                       = 0x21,
//
//    /* Internal API                                     */
//    /* ************************************************ */
//    /// Send a service changed indication
//    SONATA_GATTC_SVC_CHANGED                              = 0x40,
//};

/// Service Discovery Attribute type
/*@TRACE*/
enum sonata_gattc_sdp_att_type {
    /// No Attribute Information
    SONATA_GATTC_SDP_NONE,
    /// Included Service Information
    SONATA_GATTC_SDP_INC_SVC,
    /// Characteristic Declaration
    SONATA_GATTC_SDP_ATT_CHAR,
    /// Attribute Value
    SONATA_GATTC_SDP_ATT_VAL,
    /// Attribute Descriptor
    SONATA_GATTC_SDP_ATT_DESC,
};

/// Command complete event data structure
struct sonata_gattc_op_cmd {
    /// GATT request type
    uint8_t operation;
    /// operation sequence number
    uint16_t seq_num;
};

/// Command complete event data structure
/*@TRACE*/
struct sonata_gattc_cmp_evt {
    /// GATT request type
    uint8_t operation;
    /// Status of the request
    uint8_t status;
    /// operation sequence number - provided when operation is started
    uint16_t seq_num;
};


/// Service Discovery Command Structure
/*@TRACE*/
struct sonata_gattc_exc_mtu_cmd {
    /// GATT request type
    uint8_t operation;
    /// operation sequence number
    uint16_t seq_num;
};

/// Indicate that the ATT MTU has been updated (negotiated)
/*@TRACE*/
struct sonata_gattc_mtu_changed_ind {
    /// Exchanged MTU value
    uint16_t mtu;
    /// operation sequence number
    uint16_t seq_num;
};

/// Service Discovery Command Structure
/*@TRACE*/
struct sonata_gattc_disc_cmd {
    /// GATT request type
    uint8_t operation;
    /// UUID length
    uint8_t uuid_len;
    /// operation sequence number
    uint16_t seq_num;
    /// start handle range
    uint16_t start_hdl;
    /// start handle range
    uint16_t end_hdl;
    /// UUID
    uint8_t uuid[__ARRAY_EMPTY];
};


/// Discover Service indication Structure
/*@TRACE*/
struct sonata_gattc_disc_svc_ind {
    /// start handle
    uint16_t start_hdl;
    /// end handle
    uint16_t end_hdl;
    /// UUID length
    uint8_t uuid_len;
    /// service UUID
    uint8_t uuid[__ARRAY_EMPTY];
};

/// Discover Service indication Structure
/*@TRACE*/
struct sonata_gattc_disc_svc_incl_ind {
    /// element handle
    uint16_t attr_hdl;
    /// start handle
    uint16_t start_hdl;
    /// end handle
    uint16_t end_hdl;
    /// UUID length
    uint8_t uuid_len;
    /// included service UUID
    uint8_t uuid[__ARRAY_EMPTY];
};

/// Discovery All Characteristic indication Structure
/*@TRACE*/
struct sonata_gattc_disc_char_ind {
    /// database element handle
    uint16_t attr_hdl;
    /// pointer attribute handle to UUID
    uint16_t pointer_hdl;
    /// properties
    uint8_t prop;
    /// UUID length
    uint8_t uuid_len;
    /// characteristic UUID
    uint8_t uuid[__ARRAY_EMPTY];
};

/// Discovery Characteristic Descriptor indication Structure
/*@TRACE*/
struct sonata_gattc_disc_char_desc_ind {
    /// database element handle
    uint16_t attr_hdl;
    /// UUID length
    uint8_t uuid_len;
    /// Descriptor UUID
    uint8_t uuid[__ARRAY_EMPTY];
};


/// Simple Read (SONATA_GATTC_READ or SONATA_GATTC_READ_LONG)
/*@TRACE
 gattc_read = gattc_read_simple
 gattc_read_long = gattc_read_simple*/
struct sonata_gattc_read_simple {
    /// attribute handle
    uint16_t handle;
    /// start offset in data payload
    uint16_t offset;
    /// Length of data to read (0 = read all)
    uint16_t length;
};

/// Read by UUID: search UUID and read it's characteristic value (SONATA_GATTC_READ_BY_UUID)
/// Note: it doesn't perform an automatic read long.
/*@TRACE*/
struct sonata_gattc_read_by_uuid {
    /// Start handle
    uint16_t start_hdl;
    /// End handle
    uint16_t end_hdl;
    /// Size of UUID
    uint8_t uuid_len;
    /// UUID value
    uint8_t uuid[__ARRAY_EMPTY];
};

/// Read Multiple short characteristic (SONATA_GATTC_READ_MULTIPLE)
/*@TRACE*/
struct sonata_gattc_read_multiple {
    /// attribute handle
    uint16_t handle;
    /// Known Handle length (shall be != 0)
    uint16_t len;
};

/// request union according to read type
/*@TRACE
 */
union sonata_gattc_read_req {
    /// Simple Read (SONATA_GATTC_READ or SONATA_GATTC_READ_LONG)
    //@trc_union parent.operation == SONATA_GATTC_READ or parent.operation == SONATA_GATTC_READ_LONG
    struct sonata_gattc_read_simple simple;
    /// Read by UUID (SONATA_GATTC_READ_BY_UUID)
    //@trc_union parent.operation == SONATA_GATTC_READ_BY_UUID
    struct sonata_gattc_read_by_uuid by_uuid;
    /// Read Multiple short characteristic (SONATA_GATTC_READ_MULTIPLE)
    //@trc_union parent.operation == SONATA_GATTC_READ_MULTIPLE
    struct sonata_gattc_read_multiple multiple[1];
};
struct sonata_gattc_read_simple_long_cmd {
    /// request type
    uint8_t operation;
    /// number of read (only used for multiple read)
    uint8_t nb;
    /// operation sequence number
    uint16_t seq_num;

    uint16_t handle;
    /// start offset in data payload
    uint16_t offset;
    /// Length of data to read (0 = read all)
    uint16_t length;

};



struct sonata_gattc_read_by_uuid_cmd {
    /// request type
    uint8_t operation;
    /// number of read (only used for multiple read)
    uint8_t nb;
    /// operation sequence number
    uint16_t seq_num;
    /// request union according to read type
    /// Start handle
    uint16_t start_hdl;
    /// End handle
    uint16_t end_hdl;
    /// Size of UUID
    uint8_t uuid_len;
    /// UUID value
    uint8_t uuid[__ARRAY_EMPTY];
};

struct sonata_gattc_read_multiple_cmd {
    /// request type
    uint8_t operation;
    /// number of read (only used for multiple read)
    uint8_t nb;
    /// operation sequence number
    uint16_t seq_num;
    /// request union according to read type
    struct gattc_read_multiple multiple[1];
};

/// Read command (Simple, Long, Multiple, or by UUID)
/*@TRACE*/
struct sonata_gattc_read_cmd {
    /// request type
    uint8_t operation;
    /// number of read (only used for multiple read)
    uint8_t nb;
    /// operation sequence number
    uint16_t seq_num;
    /// request union according to read type
    union gattc_read_req req;
};

/// Attribute value read indication
/*@TRACE*/
struct sonata_gattc_read_ind {
    /// Attribute handle
    uint16_t handle;
    /// Read offset
    uint16_t offset;
    /// Read length
    uint16_t length;
    /// Handle value
    uint8_t value[__ARRAY_EMPTY];
};

/// Write peer attribute value command
/*@TRACE*/
struct sonata_gattc_write_cmd {
    /// Request type
    uint8_t operation;
    /// Perform automatic execution
    /// (if false, an ATT Prepare Write will be used this shall be use for reliable write)
    bool auto_execute;
    /// operation sequence number
    uint16_t seq_num;
    /// Attribute handle
    uint16_t handle;
    /// Write offset
    uint16_t offset;
    /// Write length
    uint16_t length;
    /// Internal write cursor shall be initialized to 0
    uint16_t cursor;
    /// Value to write
    uint8_t value[__ARRAY_EMPTY];
};

/// Write peer attribute value command
/*@TRACE*/
struct sonata_gattc_execute_write_cmd {
    /// Request type
    uint8_t operation;

    /// [True = perform/False cancel] pending write operations
    bool execute;
    /// operation sequence number
    uint16_t seq_num;
};
/// peer device triggers an event (notification)
/*@TRACE*/
struct sonata_gattc_event_ind {
    /// Event Type
    uint8_t type;
    /// Data length
    uint16_t length;
    /// Attribute handle
    uint16_t handle;
    /// Event Value
    uint8_t value[__ARRAY_EMPTY];
};

/// peer device triggers an event that requires a confirmation (indication)
/*@TRACE*/
struct sonata_gattc_event_req_ind {
    /// Event Type
    uint8_t type;
    /// Data length
    uint16_t length;
    /// Attribute handle
    uint16_t handle;
    /// Event Value
    uint8_t value[__ARRAY_EMPTY];
};

/// Confirm reception of event (trigger a confirmation message)
/*@TRACE*/
struct sonata_gattc_event_cfm {
    /// Attribute handle
    uint16_t handle;
};

/// Register to peer device events command
/*@TRACE*/
struct sonata_gattc_reg_to_peer_evt_cmd {
    /// Request type
    uint8_t operation;
    /// operation sequence number
    uint16_t seq_num;
    /// attribute start handle
    uint16_t start_hdl;
    /// attribute end handle
    uint16_t end_hdl;
};

/// Send an event to peer device
/*@TRACE*/
struct sonata_gattc_send_evt_cmd {
    /// Request type (notification / indication)
    uint8_t operation;
    /// operation sequence number
    uint16_t seq_num;
    /// characteristic handle
    uint16_t handle;
    /// length of packet to send
    uint16_t length;
    /// data value
    uint8_t value[__ARRAY_EMPTY];
};

/// Inform that attribute value is requested by lower layers.
/*@TRACE*/
struct sonata_gattc_read_req_ind {
    /// Handle of the attribute that has to be read
    uint16_t handle;
};

/// Confirm Read Request requested by GATT to profile
/*@TRACE*/
struct sonata_gattc_read_cfm {
    /// Handle of the attribute read
    uint16_t handle;
    /// Data length read
    uint16_t length;
    /// Status of read command execution by upper layers
    uint8_t status;
    /// attribute data value
    uint8_t value[__ARRAY_EMPTY];
};

/// Inform that a modification of database has been requested by peer device.
/*@TRACE*/
struct sonata_gattc_write_req_ind {
    /// Handle of the attribute that has to be written
    uint16_t handle;
    /// offset at which the data has to be written
    uint16_t offset;
    /// Data length to be written
    uint16_t length;
    /// Data to be written in attribute database
    uint8_t value[__ARRAY_EMPTY];
};

/// Confirm modification of database from upper layer when requested by peer device.
/*@TRACE*/
struct sonata_gattc_write_cfm {
    /// Handle of the attribute written
    uint16_t handle;
    /// Status of write command execution by upper layers
    uint8_t status;
};

/// Request Attribute info to upper layer - could be trigger during prepare write
/*@TRACE*/
struct sonata_gattc_att_info_req_ind {
    /// Handle of the attribute for which info are requested
    uint16_t handle;
};

/// Attribute info from upper layer confirmation
/*@TRACE*/
struct sonata_gattc_att_info_cfm {
    /// Handle of the attribute
    uint16_t handle;
    /// Current length of the attribute
    uint16_t length;
    /// use to know if it's possible to modify the attribute
    /// can contains authorization or application error code.
    uint8_t status;
};


/// Service Discovery command
/*@TRACE*/
struct sonata_gattc_sdp_svc_disc_cmd {
    /// GATT Request Type
    /// - SONATA_GATTC_SDP_DISC_SVC Search specific service
    /// - SONATA_GATTC_SDP_DISC_SVC_ALL Search for all services
    /// - SONATA_GATTC_SDP_DISC_CANCEL Cancel Service Discovery Procedure
    uint8_t operation;
    /// Service UUID Length
    uint8_t uuid_len;
    /// operation sequence number
    uint16_t seq_num;
    /// Search start handle
    uint16_t start_hdl;
    /// Search end handle
    uint16_t end_hdl;
    /// Service UUID
    uint8_t uuid[ATT_UUID_128_LEN];
};


/// Information about included service
/*@TRACE*/
struct sonata_gattc_sdp_include_svc {
    /// Attribute Type
    /// - SONATA_GATTC_SDP_INC_SVC: Included Service Information
    uint8_t att_type;
    /// Included service UUID Length
    uint8_t uuid_len;
    /// Included Service UUID
    uint8_t uuid[ATT_UUID_128_LEN];
    /// Included service Start Handle
    uint16_t start_hdl;
    /// Included service End Handle
    uint16_t end_hdl;
};

/// Information about attribute characteristic
/*@TRACE*/
struct sonata_gattc_sdp_att_char {
    /// Attribute Type
    /// - SONATA_GATTC_SDP_ATT_CHAR: Characteristic Declaration
    uint8_t att_type;
    /// Value property
    uint8_t prop;
    /// Value Handle
    uint16_t handle;
};

/// Information about attribute
/*@TRACE*/
struct sonata_gattc_sdp_att {
    /// Attribute Type
    /// - SONATA_GATTC_SDP_ATT_VAL: Attribute Value
    /// - SONATA_GATTC_SDP_ATT_DESC: Attribute Descriptor
    uint8_t att_type;
    /// Attribute UUID Length
    uint8_t uuid_len;
    /// Attribute UUID
    uint8_t uuid[ATT_UUID_128_LEN];
};

/// Attribute information
/*@TRACE
 @trc_ref gattc_sdp_att_type
 */
union sonata_gattc_sdp_att_info {
    /// Attribute Type
    uint8_t att_type;
    /// Information about attribute characteristic
    //@trc_union att_type == SONATA_GATTC_SDP_ATT_CHAR
    struct sonata_gattc_sdp_att_char att_char;
    /// Information about included service
    //@trc_union att_type == SONATA_GATTC_SDP_INC_SVC
    struct sonata_gattc_sdp_include_svc inc_svc;
    /// Information about attribute
    //@trc_union att_type == SONATA_GATTC_SDP_ATT_VAL or att_type == SONATA_GATTC_SDP_ATT_DESC
    struct sonata_gattc_sdp_att att;
};


/// Service Discovery indicate that a service has been found.
/*@TRACE
 @trc_arr info $end_hdl - $start_hdl
 */
struct sonata_gattc_sdp_svc_ind {
    /// Service UUID Length
    uint8_t uuid_len;
    /// Service UUID
    uint8_t uuid[ATT_UUID_128_LEN];
    /// Service start handle
    uint16_t start_hdl;
    /// Service end handle
    uint16_t end_hdl;
    /// attribute information present in the service
    /// (length = end_hdl - start_hdl)
    union gattc_sdp_att_info info[__ARRAY_EMPTY];
};

/// Indicate that an unknown message has been received
/*@TRACE*/
struct sonata_gattc_unknown_msg_ind {
    /// Unknown message id
    ke_msg_id_t unknown_msg_id;
};

/// Command used to enable Robust database caching
/// (Register to service changed, Set client supported features and read database hash)
/*@TRACE*/
struct sonata_gattc_robust_db_cache_en_cmd {
    /// GATT Request Type
    /// - SONATA_GATTC_ROBUST_DB_CACHE_EN Enable Robust database caching
    uint8_t operation;
    /// operation sequence number
    uint16_t seq_num;
};

/// Command used to read peer database hash
/*@TRACE*/
struct sonata_gattc_read_db_hash_cmd {
    /// GATT Request Type
    /// - SONATA_GATTC_READ_DB_HASH Read peer database hash
    uint8_t operation;
    /// operation sequence number
    uint16_t seq_num;
};

/// Provide value of the peer database hash
/*@TRACE*/
struct sonata_gattc_db_hash_ind {
    /// Database Hash
    uint8_t hash[GAP_KEY_LEN];
};

/// Informs that peer device database updated using service changed indication
/*@TRACE
 @trc_arr info $end_hdl - $start_hdl
 */
struct sonata_gattc_svc_chg_req_ind {
    /// Start handle
    uint16_t start_handle;
    /// End Handle
    uint16_t end_handle;
};

/// Provide information about GATT for current connection that can be reuse on another connection
/*@TRACE*/
struct sonata_gattc_con_info_ind {
    // GATT Client Side
    /// Peer GATT Service Start handle
    uint16_t gatt_start_handle;
    /// Peer GATT Service End Handle
    uint16_t gatt_end_handle;
    /// Peer Service Change value handle
    uint16_t svc_chg_handle;

    // GATT Service Side
    /// Client bond data information (@see enum gapc_cli_info)
    uint8_t cli_info;
    /// Client supported features    (@see enum gapc_cli_feat)
    uint8_t cli_feat;
};



enum sonata_gattm_msg_id
{
    /* Database Management */
    /// Add service in database request
    SONATA_GATTM_ADD_SVC_REQ                   = MSG_ID(GATTM, 0x00),
    /// Add service in database response
    SONATA_GATTM_ADD_SVC_RSP                   = MSG_ID(GATTM, 0x01),

    /* Service management */
    /// Get permission settings of service request
    SONATA_GATTM_SVC_GET_PERMISSION_REQ        = MSG_ID(GATTM, 0x02),
    /// Get permission settings of service response
    SONATA_GATTM_SVC_GET_PERMISSION_RSP        = MSG_ID(GATTM, 0x03),
    /// Set permission settings of service request
    SONATA_GATTM_SVC_SET_PERMISSION_REQ        = MSG_ID(GATTM, 0x04),
    /// Set permission settings of service response
    SONATA_GATTM_SVC_SET_PERMISSION_RSP        = MSG_ID(GATTM, 0x05),

    /* Attribute Manipulation */
    /// Get permission settings of attribute request
    SONATA_GATTM_ATT_GET_PERMISSION_REQ        = MSG_ID(GATTM, 0x06),
    /// Get permission settings of attribute response
    SONATA_GATTM_ATT_GET_PERMISSION_RSP        = MSG_ID(GATTM, 0x07),
    /// Set permission settings of attribute request
    SONATA_GATTM_ATT_SET_PERMISSION_REQ        = MSG_ID(GATTM, 0x08),
    /// Set permission settings of attribute response
    SONATA_GATTM_ATT_SET_PERMISSION_RSP        = MSG_ID(GATTM, 0x09),

    /// Get attribute value request
    SONATA_GATTM_ATT_GET_VALUE_REQ             = MSG_ID(GATTM, 0x0A),
    /// Get attribute value response
    SONATA_GATTM_ATT_GET_VALUE_RSP             = MSG_ID(GATTM, 0x0B),
    /// Set attribute value request
    SONATA_GATTM_ATT_SET_VALUE_REQ             = MSG_ID(GATTM, 0x0C),
    /// Set attribute value response
    SONATA_GATTM_ATT_SET_VALUE_RSP             = MSG_ID(GATTM, 0x0D),
    /// Indication to the task that sends the unknown message
    SONATA_GATTM_UNKNOWN_MSG_IND               = MSG_ID(GATTM, 0x0E),

    /* Debug messages */
    /// DEBUG ONLY: Destroy Attribute database request
    SONATA_GATTM_DESTROY_DB_REQ                = MSG_ID(GATTM, 0x0F),
    /// DEBUG ONLY: Destroy Attribute database response
    SONATA_GATTM_DESTROY_DB_RSP                = MSG_ID(GATTM, 0x10),
    /// DEBUG ONLY: Retrieve list of services request
    SONATA_GATTM_SVC_GET_LIST_REQ              = MSG_ID(GATTM, 0x11),
    /// DEBUG ONLY: Retrieve list of services response
    SONATA_GATTM_SVC_GET_LIST_RSP              = MSG_ID(GATTM, 0x12),
    /// DEBUG ONLY: Retrieve information of attribute request
    SONATA_GATTM_ATT_GET_INFO_REQ              = MSG_ID(GATTM, 0x13),
    /// DEBUG ONLY: Retrieve information of attribute response
    SONATA_GATTM_ATT_GET_INFO_RSP              = MSG_ID(GATTM, 0x14),
    /// DEBUG ONLY: Set Service visibility request
    SONATA_GATTM_SVC_VISIBILITY_SET_REQ        = MSG_ID(GATTM, 0x15),
    /// DEBUG ONLY: Set Service visibility response
    SONATA_GATTM_SVC_VISIBILITY_SET_RSP        = MSG_ID(GATTM, 0x16),

    /* GATT Caching */
    /// Compute hash value of the attribute database request
    SONATA_GATTM_ATT_DB_HASH_COMP_REQ          = MSG_ID(GATTM, 0x20),
    /// Compute hash value of the attribute database response
    SONATA_GATTM_ATT_DB_HASH_COMP_RSP          = MSG_ID(GATTM, 0x21),
};









///**
// * Attribute Description
// */
///*@TRACE*/
//struct sonata_gatt_att_desc
//{
//    /** Attribute UUID (LSB First) */
//    uint8_t uuid[ATT_UUID_128_LEN];
//
//    /**
//     *  Attribute Permission (@see attm_perm_mask)
//     */
//    uint16_t perm;
//
//
//    /**
//     * Maximum length of the attribute
//     *
//     * Note:
//     * For Included Services and Characteristic Declarations, this field contains targeted
//     * handle.
//     *
//     * For Characteristic Extended Properties, this field contains 2 byte value
//     *
//     * Not used Client Characteristic Configuration and Server Characteristic Configuration,
//     * this field is not used.
//     */
//    uint16_t max_len;
//
//    /**
//     * Attribute Extended permissions
//     *
//     * Extended Value permission bit field
//     *
//     *   15   14   13   12   11   10    9    8    7    6    5    4    3    2    1    0
//     * +----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+
//     * | RI |UUID_LEN |EKS |                       Reserved                            |
//     * +----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+
//    *
//     * Bit [0-11] : Reserved
//     * Bit [12]   : Encryption key Size must be 16 bytes
//     * Bit [13-14]: UUID Length             (0 = 16 bits, 1 = 32 bits, 2 = 128 bits, 3 = RFU)
//     * Bit [15]   : Trigger Read Indication (0 = Value present in Database, 1 = Value not present in Database)
//     */
//    uint16_t ext_perm;
//};

/**
 * Service description
 */
/*@TRACE
 @trc_arr atts $nb_att*/
struct sonata_gattm_svc_desc
{
    /// Attribute Start Handle (0 = dynamically allocated)
    uint16_t start_hdl;
    /// Task identifier that manages service
    uint16_t task_id;

    /**
     *    7    6    5    4    3    2    1    0
     * +----+----+----+----+----+----+----+----+
     * |SEC |UUID_LEN |DIS |  AUTH   |EKS | MI |
     * +----+----+----+----+----+----+----+----+
     *
     * Bit [0]  : Task that manage service is multi-instantiated (Connection index is conveyed)
     * Bit [1]  : Encryption key Size must be 16 bytes
     * Bit [2-3]: Service Permission      (0 = NO_AUTH, 1 = UNAUTH, 2 = AUTH, 3 = Secure Connect)
     * Bit [4]  : Disable the service
     * Bit [5-6]: UUID Length             (0 = 16 bits, 1 = 32 bits, 2 = 128 bits, 3 = RFU)
     * Bit [7]  : Secondary Service       (0 = Primary Service, 1 = Secondary Service)
     */
    uint8_t perm;

    /// Number of attributes
    uint8_t nb_att;

    /** Service  UUID */
    uint8_t uuid[ATT_UUID_128_LEN];

    /**
     * List of attribute description present in service.
     */
    struct sonata_gatt_att_desc atts[__ARRAY_EMPTY];
};


/// Add service in database request
/*@TRACE*/
struct sonata_gattm_add_svc_req
{
    /// service description
    struct sonata_gattm_svc_desc svc_desc;
};

/// Add service in database response
/*@TRACE*/
struct sonata_gattm_add_svc_rsp
{
    /// Start handle of allocated service in attribute database
    uint16_t start_hdl;
    /// Return status of service allocation in attribute database.
    uint8_t status;
};

/* Service management */
/// Get permission settings of service request
/*@TRACE*/
struct sonata_gattm_svc_get_permission_req
{
    /// Service start attribute handle
    uint16_t start_hdl;
};

/// Get permission settings of service response
/*@TRACE*/
struct sonata_gattm_svc_get_permission_rsp
{
    /// Service start attribute handle
    uint16_t start_hdl;
    /// service permission
    uint8_t perm;
    /// Return status
    uint8_t status;
};

/// Set permission settings of service request
/*@TRACE*/
struct sonata_gattm_svc_set_permission_req
{
    /// Service start attribute handle
    uint16_t start_hdl;
    /// service permission
    uint8_t perm;
};

/// Set permission settings of service response
/*@TRACE*/
struct sonata_gattm_svc_set_permission_rsp
{
    /// Service start attribute handle
    uint16_t start_hdl;
    /// Return status
    uint8_t status;
};


/* Attribute management */
/// Get permission settings of attribute request
/*@TRACE*/
struct sonata_gattm_att_get_permission_req
{
    /// Handle of the attribute
    uint16_t handle;
};

/// Get permission settings of attribute response
/*@TRACE*/
struct sonata_gattm_att_get_permission_rsp
{
    /// Handle of the attribute
    uint16_t handle;
    /// Attribute permission
    uint16_t perm;
    /// Extended Attribute permission
    uint16_t ext_perm;
    /// Return status
    uint8_t status;
};

/// Set permission settings of attribute request
/*@TRACE*/
struct sonata_gattm_att_set_permission_req
{
    /// Handle of the attribute
    uint16_t handle;
    /// Attribute permission
    uint16_t perm;
    /// Extended Attribute permission
    uint16_t ext_perm;
};

/// Set permission settings of attribute response
/*@TRACE*/
struct sonata_gattm_att_set_permission_rsp
{
    /// Handle of the attribute
    uint16_t handle;
    /// Return status
    uint8_t status;
};


/// Get attribute value request
/*@TRACE*/
struct sonata_gattm_att_get_value_req
{
    /// Handle of the attribute
    uint16_t handle;
};

/// Get attribute value response
/*@TRACE*/
struct sonata_gattm_att_get_value_rsp
{
    /// Handle of the attribute
    uint16_t handle;
    /// Attribute value length
    uint16_t length;
    /// Return status
    uint8_t status;
    /// Attribute value
    uint8_t value[__ARRAY_EMPTY];
};

/// Set attribute value request
/*@TRACE*/
struct sonata_gattm_att_set_value_req
{
    /// Handle of the attribute
    uint16_t handle;
    /// Attribute value length
    uint16_t length;
    /// Attribute value
    uint8_t value[__ARRAY_EMPTY];
};

/// Set attribute value response
/*@TRACE*/
struct sonata_gattm_att_set_value_rsp
{
    /// Handle of the attribute
    uint16_t handle;
    /// Return status
    uint8_t status;
};

/// Indicate that an unknown message has been received
/*@TRACE*/
struct sonata_gattm_unknown_msg_ind
{
    /// Unknown message id
    ke_msg_id_t unknown_msg_id;
};

/// DEBUG ONLY: Destroy Attribute database request
/*@TRACE*/
struct sonata_gattm_destroy_db_req
{
    /// New Gap Start Handle
    uint16_t gap_hdl;
    /// New Gatt Start Handle
    uint16_t gatt_hdl;
};

/// DEBUG ONLY: Destroy Attribute database Response
/*@TRACE*/
struct sonata_gattm_destroy_db_rsp
{
    /// Return status
    uint8_t status;
};


///// Service information
///*@TRACE*/
//struct sonata_gattm_svc_info
//{
//    /// Service start handle
//    uint16_t start_hdl;
//    /// Service end handle
//    uint16_t end_hdl;
//    /// Service task_id
//    uint16_t task_id;
//    /// Service permission
//    uint8_t perm;
//};

/// DEBUG ONLY: Retrieve list of services response
/*@TRACE*/
struct sonata_gattm_svc_get_list_rsp
{
    /// Return status
    uint8_t status;
    /// Number of services
    uint8_t nb_svc;
    /// Array of information about services
    struct sonata_gatt_svc_info svc[__ARRAY_EMPTY];
};

/// DEBUG ONLY: Retrieve information of attribute request
/*@TRACE*/
struct sonata_gattm_att_get_info_req
{
    /// Attribute Handle
    uint16_t handle;
};

/// DEBUG ONLY: Retrieve information of attribute response
/*@TRACE*/
struct  sonata_gattm_att_get_info_rsp
{
    /// Return status
    uint8_t status;
    /// UUID Length
    uint8_t uuid_len;
    /// Attribute Handle
    uint16_t handle;
    /// Attribute Permissions
    uint16_t perm;
    /// Extended Attribute permission
    uint16_t ext_perm;
    /// UUID value
    uint8_t uuid[ATT_UUID_128_LEN];
};

/// DEBUG ONLY: Set Service visibility request
/*@TRACE*/
struct sonata_gattm_svc_visibility_set_req
{
    /// Service handle
    uint16_t handle;
    /// True to set service visible, false to hide it
    bool     visible;
};

/// DEBUG ONLY: Set Service visibility response
/*@TRACE*/
struct sonata_gattm_svc_visibility_set_rsp
{
    /// Return status
    uint8_t  status;
    /// Service handle
    uint16_t handle;
};

/// Compute hash value of the attribute database response
/*@TRACE*/
struct sonata_gattm_att_db_hash_comp_rsp
{
    /// Return status
    uint8_t status;
    /// Computed hash value (valid if status is GAP_ERR_NO_ERROR)
    uint8_t hash[GAP_KEY_LEN];
};


/*******************************   GATTC **************************************/

int sonata_gatt_exc_mtu_cmd_handler(uint8_t conidx, uint16_t seq_num) ;
int sonata_gatt_disc_all_svc_cmd_handler(uint8_t conidx, uint16_t seq_num);
//int sonata_gatt_disc_all_svc_cmd_handler(uint8_t conidx, uint16_t seq_num, uint16_t start_hdl,uint16_t end_hdl);
int sonata_gatt_disc_svc_by_uuid_cmd_handler(uint8_t conidx, uint16_t seq_num, uint16_t start_hdl, uint16_t end_hdl, uint8_t uuid_len, uint8_t*uuid) ;
int sonata_gatt_disc_included_svc_cmd_handler(uint8_t conidx, uint16_t seq_num, uint16_t start_hdl,uint16_t end_hdl) ;
int sonata_gatt_disc_all_char_cmd_handler(uint8_t conidx, uint16_t seq_num, uint16_t start_hdl,uint16_t end_hdl) ;
int sonata_gatt_disc_char_by_uuid_cmd_handler(uint8_t conidx, uint16_t seq_num, uint16_t start_hdl, uint16_t end_hdl, uint8_t uuid_len, uint8_t*uuid) ;
int sonata_gatt_disc_desc_char_cmd_handler(uint8_t conidx, uint16_t seq_num, uint16_t start_hdl,uint16_t end_hdl);
int sonata_gatt_read_by_handle_cmd_handler(uint8_t conidx, uint16_t seq_num, uint16_t handle);
int sonata_gatt_read_cmd_handler(uint8_t conidx, uint16_t seq_num, uint16_t handle, uint16_t offset, uint16_t length);
int sonata_gatt_read_long_cmd_handler(uint8_t conidx, uint16_t seq_num, uint16_t handle, uint16_t offset, uint16_t length);
int sonata_gatt_read_by_uuid_cmd_handler(uint8_t conidx, uint16_t seq_num,uint16_t start_hdl,uint16_t end_hdl,uint8_t uuid_len, uint8_t*uuid) ;
int sonata_gatt_write_cmd_handler(uint8_t conidx, uint16_t seq_num,uint16_t handle,uint16_t offset, uint16_t cursor,uint16_t length,uint8_t *value) ;
int sonata_gatt_write_no_response_cmd_handler(uint8_t conidx, uint16_t seq_num,uint16_t handle,uint16_t offset, uint16_t cursor,uint16_t length,uint8_t *value);
int sonata_gatt_write_signed_cmd_handler(uint8_t conidx, uint16_t seq_num, uint16_t handle, uint16_t offset, uint16_t cursor, uint16_t length,
                                         uint8_t *value);
int sonata_gatt_execute_write_cmd_handler(uint8_t conidx, uint16_t seq_num, bool execute);
int sonata_gatt_reg_to_peer_evt_cmd_handler(uint8_t conidx, uint16_t seq_num, uint16_t start_hdl, uint16_t end_hdl) ;
int sonata_gatt_unreg_to_peer_evt_cmd_handler(uint8_t conidx, uint16_t seq_num, uint16_t start_hdl, uint16_t end_hdl) ;
int sonata_gatt_sdp_disc_svc_cmd_handler(uint8_t conidx, uint16_t seq_num,uint16_t start_hdl,uint16_t end_hdl,uint8_t uuid_len,uint8_t* uuid) ;
int sonata_gatt_sdp_disc_cancel_cmd_handler(uint8_t conidx);
int sonata_gatt_sdp_disc_svc_all_cmd_handler(uint8_t conidx, uint16_t seq_num,uint16_t start_hdl,uint16_t end_hdl,uint8_t uuid_len,uint8_t* uuid);

int sonata_gatt_robust_db_cache_en_cmd_handler(uint8_t conidx, uint16_t seq_num) ;
int sonata_gatt_read_db_hash_cmd_handler(uint8_t conidx, uint16_t seq_num) ;
int sonata_gatt_send_notify_evt_cmd_handler(uint8_t conidx, uint16_t seq_num,uint16_t handle,uint16_t length,uint8_t * value);
int sonata_gatt_send_indicate_evt_cmd_handler(uint8_t conidx, uint16_t seq_num,uint16_t handle,uint16_t length,uint8_t * value);
int sonata_gatt_send_svc_changed_evt_cmd_handler(uint8_t conidx, uint16_t seq_num,uint16_t handle,uint16_t start_handle, uint16_t end_handle);

int sonata_gatt_event_cfm_handler(uint8_t conidx, uint16_t handle) ;
//int sonata_gatt_svc_chg_cfm_handler(uint8_t conidx, struct i_gap_svc_chg_cfm * cmd);
int sonata_gatt_read_cfm_handler(uint8_t conidx, uint16_t handle, uint8_t status, uint16_t length, uint8_t *value) ;
int sonata_gatt_write_cfm_handler(uint8_t conidx, uint16_t handle, uint8_t status) ;
int sonata_gatt_att_info_cfm_handler(uint8_t conidx, uint16_t handle, uint16_t length, uint8_t status) ;
int sonata_gatt_svc_change_cfm_handler(uint8_t conidx, uint16_t handle);

int sonata_gatt_mtu_changed_ind_handler(ke_msg_id_t msgid, void *p_param, ke_task_id_t dest_id, ke_task_id_t src_id) ;
int sonata_gatt_disc_svc_ind_handler(ke_msg_id_t msgid, void *p_param, ke_task_id_t dest_id, ke_task_id_t src_id) ;
int sonata_gatt_disc_svc_incl_ind_handler(ke_msg_id_t msgid, void *p_param, ke_task_id_t dest_id, ke_task_id_t src_id) ;
int sonata_gatt_disc_char_ind_handler(ke_msg_id_t msgid, void *p_param, ke_task_id_t dest_id, ke_task_id_t src_id) ;
int sonata_gatt_disc_char_desc_ind_handler(ke_msg_id_t msgid, void *p_param, ke_task_id_t dest_id, ke_task_id_t src_id) ;
int sonata_gatt_read_ind_handler(ke_msg_id_t msgid, void *p_param, ke_task_id_t dest_id, ke_task_id_t src_id) ;
int sonata_gatt_event_ind_handler(ke_msg_id_t msgid, void *p_param, ke_task_id_t dest_id, ke_task_id_t src_id) ;
int sonata_gatt_event_req_ind_handler(ke_msg_id_t msgid, void *p_param, ke_task_id_t dest_id, ke_task_id_t src_id) ;
int sonata_gatt_read_req_ind_handler(ke_msg_id_t msgid, void *p_param, ke_task_id_t dest_id, ke_task_id_t src_id) ;
int sonata_gatt_write_req_ind_handler(ke_msg_id_t msgid, void *p_param, ke_task_id_t dest_id, ke_task_id_t src_id) ;
int sonata_gatt_att_info_req_ind_handler(ke_msg_id_t msgid, void *p_param, ke_task_id_t dest_id, ke_task_id_t src_id);
int sonata_gatt_sdp_svc_ind_handler(ke_msg_id_t msgid, void *p_param, ke_task_id_t dest_id, ke_task_id_t src_id) ;
int sonata_gatt_db_hash_ind_handler(ke_msg_id_t msgid, void *p_param, ke_task_id_t dest_id, ke_task_id_t src_id) ;
int sonata_gatt_db_cache_out_of_sync_ind_handler(ke_msg_id_t msgid, void *p_param, ke_task_id_t dest_id, ke_task_id_t src_id) ;
int sonata_gatt_svc_chg_req_ind_handler(ke_msg_id_t msgid, void *p_param, ke_task_id_t dest_id, ke_task_id_t src_id) ;
int sonata_gatt_con_info_ind_handler(ke_msg_id_t msgid, void *p_param, ke_task_id_t dest_id, ke_task_id_t src_id) ;
int sonata_gatt_client_rtx_ind_handler(ke_msg_id_t msgid, void *p_param, ke_task_id_t dest_id, ke_task_id_t src_id) ;
int sonata_gatt_server_rtx_ind_handler(ke_msg_id_t msgid, void *p_param, ke_task_id_t dest_id, ke_task_id_t src_id);


/*******************************   GATTM **************************************/


int sonata_gatt_add_svc_req_handler(/*uint16_t task_id,*/ uint16_t start_hdl, uint8_t perm, uint8_t *uuid, uint8_t nb_att, sonata_gatt_att_desc_t *atts);
int sonata_gatt_svc_get_permission_req_handler(uint16_t start_hdl);
int sonata_gatt_svc_set_permission_req_handler(uint16_t start_hdl, uint8_t perm);
int sonata_gatt_att_get_permission_req_handler(uint16_t handle);
int sonata_gatt_att_set_permission_req_handler(uint16_t handle, uint16_t perm, uint16_t ext_perm);
int sonata_gatt_att_get_value_req_handler(uint16_t handle);
int sonata_gatt_att_set_value_req_handler(uint16_t handle, uint16_t length, uint8_t *value);
int sonata_gatt_destroy_db_req_handler(uint16_t gap_hdl, uint16_t gatt_hdl);
int sonata_gatt_svc_get_list_req_handler(uint8_t status, uint8_t nb_svc, sonata_gatt_svc_info_t *svc);
int sonata_gatt_att_get_info_req_handler(uint16_t handle);
int sonata_gatt_svc_visibility_set_req_handler(uint16_t handle, bool visible);
int sonata_gatt_att_db_hash_comp_req_handler();

#endif //__SONATA_GATT_API_INT_H
