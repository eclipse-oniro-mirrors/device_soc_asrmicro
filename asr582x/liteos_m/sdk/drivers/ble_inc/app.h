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
 * @file app.h
 *
 * @brief Application entry point
 *
 ****************************************************************************************
 */

#ifndef APP_H_
#define APP_H_

/**
 ****************************************************************************************
 * @addtogroup APP
 * @ingroup RICOW
 *
 * @brief Application entry point.
 *
 * @{
 ****************************************************************************************
 */

/*
 * INCLUDE FILES
 ****************************************************************************************
 */
#include "sonata_ble_hook.h"
#include "msm_ble_api.h"
#include "sonata_gap.h"
#include "sonata_gap_api.h"

/*
 * DEFINES
 ****************************************************************************************
 */

/*
 * MACROS
 ****************************************************************************************
 */
// debug trace
#define APP_DBG    1
#if APP_DBG
#define APP_TRC    printf
#else
#define APP_TRC(...)
#endif // APP_DBG

#define APP_DBG_ERROR    1
#if APP_DBG_ERROR
#define APP_TRC_ERROR    printf
#else
#define APP_TRC_ERROR(...)
#endif // APP_DBG_HIGH
/*
 * ENUMERATIONS
 ****************************************************************************************
 */
#define MAX_BONDED_DEV_NUM               (3)
#define MAX_BONDED_DEV_INDEX             (MAX_BONDED_DEV_NUM - 1)
#define INVALID_BONDED_INDEX             (-1)
#define APP_GAP_KEY_LEN                  (0x10)
#define APP_GAP_RAND_NB_LEN              (0x08)
#define APP_BD_ADDR_LEN                  (6)
#define APP_UUID_LEN                     (16)
#define KEY_LEN                          0x10

enum app_connect_state {
    /// Connection succeeded
    APP_STATE_CONNECTED = 0,
    // Link is disconnected
    APP_STATE_DISCONNECTED,
};

typedef enum {
    APP_DISCONNECTED,
    APP_CONNECTED,
    APP_BONDING,
    APP_BONDED,
} bound_conn_state;

/*
 * TYPE DEFINITIONS
 ****************************************************************************************
 */

// Long Term Key information
typedef struct app_sonata_gap_ltk {
    // Long Term Key
    uint8_t ltk[APP_GAP_KEY_LEN];
    // Encryption Diversifier
    uint16_t ediv;
    // Random Number
    uint8_t randnb[APP_GAP_RAND_NB_LEN];
} app_sonata_gap_ltk_t;

// Short Term Key information
typedef struct app_sonata_gap_irk {
    // Short Term Key
    uint8_t irk[APP_GAP_KEY_LEN];
    // irk addr
    uint8_t irk_addr[APP_BD_ADDR_LEN];
} app_sonata_gap_irk_t;

typedef struct bonded_dev_info {
    uint8_t              peer_addr[APP_BD_ADDR_LEN];
    app_sonata_gap_ltk_t ltk;
    uint8_t              ltk_in[APP_GAP_KEY_LEN];
    app_sonata_gap_irk_t irk;
    uint8_t              periph_bond;
} bonded_dev_info_t;

typedef struct bonded_dev_info_list {
    uint8_t           total_dev;
    uint8_t           current_dev_index;
    bonded_dev_info_t bonded_device_info[MAX_BONDED_DEV_NUM];
} bonded_dev_info_list_t;

typedef struct peer_conn_param {
    // Connection interval maximum
    uint16_t intv_max;
    // Latency
    uint16_t latency;
    // Supervision timeout
    uint16_t time_out;
} peer_conn_param_t;

typedef struct connect_req_info {
    uint8_t           conidx;
    uint8_t           bd_addr[APP_BD_ADDR_LEN];
} connect_req_info_t;

typedef struct adv_idx_info {
    uint8_t           local_idx;
    uint8_t           adv_id;
} adv_idx_info_t;

typedef struct {
    uint8_t advdata[31];
    uint8_t advdataLen;

} ble_adv_data_set_t;

typedef struct {
    uint8_t respdata[31];
    uint8_t respdataLen;

} ble_scan_data_set_t;

typedef struct {
    int status;
    int len;
    int handler;
    uint8_t uuid[APP_UUID_LEN];
} app_reg_service_cmp_t;

typedef struct {
    int connId;
    uint8_t addr[APP_BD_ADDR_LEN];
} app_connect_status_ind_t;

typedef struct {
    int connId;
    int status;
} app_ind_sent_ind_t;

typedef struct {
    int connId;
    int mtu;
} app_mtu_change_ind_t;

typedef struct {
    int advId;
    int status;
} app_adv_status_ind_t;

/**
* @brief enum core evt indicate type
*/
typedef enum {
    BLE_SERVICE_ADD_CMP,
    BLE_DEV_CONNECTED,
    BLE_DEV_DISCONNECTED,
    BLE_IND_SENT,
    BLE_MTU_CHANGE,
    BLE_ADV_START,
    BLE_ADV_STOP,
} app_core_evt_ind_t;

/*
 * GLOBAL VARIABLE DECLARATION
 ****************************************************************************************
 */
extern sonata_ble_hook_t app_hook;

/// app Core Event indicate Callback
typedef int (*app_core_evt_ind_cb)(app_core_evt_ind_t evt, void *p_param);

typedef int (*app_sec_req_cb)(uint8_t *addr);

typedef enum {
    USER_INVALID_MODULE_ID,
    USER_MIDEA_MODULE_ID,
    USER_OHOS_MODULE_ID,
    USER_MAX_MODULE_ID
} ble_stack_opr_module_id_t;

#define APP_ACTIVE_MAX 5

typedef struct app_uuid_t {
    uint16_t service;
    uint16_t read;
    uint16_t write;
    uint16_t ntf;
} app_uuids;
typedef struct actives_t {
    uint8_t assign_id ;
    uint8_t type; // A0:Adv  A1:Scan  A2:Peer
    bool runing;
    uint8_t peer[SONATA_GAP_BD_ADDR_LEN];
    uint8_t name[20];
} actives;
typedef struct app_env_t {

    uint8_t gAppStatus;
    uint16_t attrHandle;
    uint16_t targetWriteHandle;
    uint16_t targetReadHandle;
    uint16_t targetNtfHandle;
    app_uuids appUuids;
    actives act[APP_ACTIVE_MAX];
} app_env;
/*
 * FUNCTION DECLARATIONS
 ****************************************************************************************
 */

/**
 ****************************************************************************************
 * @brief Initialize the BLE demo application.
 ****************************************************************************************
 */
void app_init(void);
void app_ble_config_legacy_advertising(void);
void app_ble_config_scanning(void);
void app_ble_config_initiating(void);
void app_ble_stop_scanning(void);

uint8_t app_get_adv_status(void);
uint8_t app_get_connect_status(void);
uint16_t app_ble_start_advertising(uint8_t adv_id);
bool app_is_ble_test_mode(void);
void app_set_ble_test_mode(bool mode);
uint16_t app_ble_advertising_stop(uint8_t adv_id);
int app_ble_advertising_start(uint8_t *adv_id, ble_adv_data_set_t *data, ble_scan_data_set_t *scan_data);
void app_gap_set_scan_cb(app_ble_scan_callback_t cb);
int app_ble_stack_stop(ble_stack_opr_module_id_t module);
int app_ble_stack_start(ble_stack_opr_module_id_t module);
void app_ble_set_device_name(uint8_t *name, uint32_t len);
int app_set_security_io_cap(uint8_t cap);
int app_ble_disconnect_by_addr(uint8_t *addr);
int  app_ble_disable_service_by_handler(uint16_t start_hdl);
void app_register_core_evt_ind(app_core_evt_ind_cb cb);
void app_gap_notify_pair_request_rsp(uint8_t *bd_addr, uint8_t accept);
void app_register_sec_cb(app_sec_req_cb  cb);
int app_set_security_auth_req(uint8_t auth_req);
void app_set_connect_flag(uint8_t vaule);
void app_gap_connect_confirm(uint8_t *addr, uint8_t auth);
void app_ble_gatt_data_send_notify(uint16_t local_handle, uint16_t idx, uint16_t length, uint8_t *p_value);
uint16_t app_ble_stop_adv_without_id(void);
void app_ble_set_target_address(uint8_t *target);
void app_ble_set_uuids(uint16_t service, uint16_t read, uint16_t write, uint16_t ntf);
bool app_ble_master_write_data(uint8_t conidx, uint16_t length, uint8_t *data);
bool app_ble_master_read_data(uint8_t conidx, uint16_t length, uint8_t *data);
bool app_ble_master_turn_ntf(uint8_t conidx, bool on);
app_uuids *app_ble_get_uuids(void);
void app_ble_disconnect(uint8_t conidx);
actives *app_get_active(void);
void app_ble_set_test_write_uuid(uint8_t *uuid);
void app_ble_set_test_read_uuid(uint8_t *uuid);
void app_ble_start_advertising_with_param(sonata_gap_directed_adv_create_param_t *param, ble_adv_data_set_t *data,
        ble_scan_data_set_t *scan_data, uint8_t own_addr_type, uint16_t duration, uint8_t max_adv_evt);

// @} APP

#endif // APP_H_
