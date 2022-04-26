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

#ifndef _MSM_BLE_API_H_
#define _MSM_BLE_API_H_

#include <stdint.h>
#include "sonata_att.h"
#include "sonata_gatt_api.h"

#define BLE_APP_FRAME_DATA_MAX_LENGTH    300

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

#define PWC_AU   (SONATA_PERM(WRITE_COMMAND, ENABLE) | SONATA_PERM(WP, AUTH))

/// GATT read perm unauth   (0x0201)
#define PRD_UA   (SONATA_PERM(RD, ENABLE)        | SONATA_PERM(RP, UNAUTH))
/// GATT write perm unauth  (0x0804)
#define PWR_UA   (SONATA_PERM(WRITE_REQ, ENABLE) | SONATA_PERM(WP, UNAUTH))
/// GATT ind perm  unauth   (0X2010)
#define PIND_UA  (SONATA_PERM(IND, ENABLE)       | SONATA_PERM(IP, UNAUTH))
/// GATT notify perm unauth (0x1040)
#define PNTF_UA  (SONATA_PERM(NTF, ENABLE)       | SONATA_PERM(NP, UNAUTH))

/// GATT write perm unauth  (0x0404)
#define PWC_UA   (SONATA_PERM(WRITE_COMMAND, ENABLE) | SONATA_PERM(WP, UNAUTH))

/// GATT read perm no auth  (0x0200)
#define PRD_NA   (SONATA_PERM(RD, ENABLE)        | SONATA_PERM(RP, NO_AUTH))
/// GATT write perm no auth (0x0800)
#define PWR_NA   (SONATA_PERM(WRITE_REQ, ENABLE) | SONATA_PERM(WP, NO_AUTH))
/// GATT ind perm  no auth  (0X2000)
#define PIND_NA  (SONATA_PERM(IND, ENABLE)       | SONATA_PERM(IP, NO_AUTH))
/// GATT notify perm no auth(0x1000)
#define PNTF_NA  (SONATA_PERM(NTF, ENABLE)       | SONATA_PERM(NP, NO_AUTH))

#define PWC_NA   (SONATA_PERM(WRITE_COMMAND, ENABLE) | SONATA_PERM(WP, NO_AUTH))

/// extended Descriptor Present (0X8000)
#define PEXT     (SONATA_PERM(EXT,ENABLE))
/// enable Read Indication (1 = Value not present in Database) (0x8000)
#define PRI      (SONATA_PERM(RI, ENABLE))

/** * @brief       Minimum advertising interval.The range is from 0x0020 to 0x4000. */
#define BLE_MCRC_MIN_INTERVAL              192 // (0x01E0)
/* 0.3s *//** * @brief       Maximum advertising interval.The range is from 0x0020 to 0x4000 and should be greater than the minimum advertising interval. */
#define BLE_MCRC_MAX_INTERVAL              192 // (0x01E0)
/* 0.3s *//** * @brief       Advertising channel map */
#define MS_BLE_CHANNEL_NUM               (7)
/** * @brief       Advertising filter policy */
#define MS_BLE_FILTER_POLICY             (0)

#define MAX_SERVICE_NUM 10

enum BLE_ADV_STATE {
    /// Advertising activity does not exists
    BLE_ADV_STATE_IDLE = 0,
    /// Creating advertising activity
    BLE_ADV_STATE_CREATING,
    /// Setting advertising data
    BLE_ADV_STATE_SETTING_ADV_DATA,
    /// Setting scan response data
    BLE_ADV_STATE_SETTING_SCAN_RSP_DATA,
    /// Advertising activity created
    BLE_ADV_STATE_CREATED,
    /// Starting advertising activity
    BLE_ADV_STATE_STARTING,
    /// Advertising activity started
    BLE_ADV_STATE_STARTED,
    /// Stopping advertising activity
    BLE_ADV_STATE_STOPPING,
};

typedef enum {
    BLE_ENABLE_ADV,
    BLE_DISABLE_ADV,
} ble_adv_enable;

typedef enum {
    BLE_GATTC_INDICATE,
    BLE_GATTC_NOTIFY,
} ble_att_op_t;

typedef enum {
    BLE_STATUS_SUCCESS,
    BLE_STATUS_INVALID_PARAM,
    BLE_STATUS_DISCONNETED,
    BLE_STATUS_FAILED,
} ble_status_t;

typedef struct BLE_ADV_PARAM_T {
    uint16_t advertising_interval_min;
    uint16_t advertising_interval_max;
    uint8_t advertising_type;
    uint8_t own_address_type;
    uint8_t advertising_channel_map;
    uint8_t advertising_filter_policy;
} ble_adv_param_t;

typedef struct BLE_ADV_DATA_T {
    uint8_t ble_advdata[32];     /* !< ?????? */
    uint8_t ble_advdataLen;     /* !< ?????? */

} ble_adv_data_t;

typedef struct BLE_SCAN_DATA_T {
    uint8_t ble_respdata[32];     /* !< ?????????? */
    uint8_t ble_respdataLen;     /* !< ?????????? */

} ble_scan_data_t;

typedef struct BLE_CMD_DISCONNECT_T {
    uint8_t link_id;
    uint8_t reason;

} ble_cmd_disconnect_t;

typedef struct BLE_VALUE_NOTIFICATION_INDICATION_T {
    uint8_t att_operation;
    uint16_t att_handle;
    uint16_t att_length;
    uint8_t  att_value[BLE_APP_FRAME_DATA_MAX_LENGTH];
} ble_value_notification_indication_t;

typedef struct BLE_ADV_ENABLE_T {
    uint8_t advertising_enable;
} ble_adv_enable_t;

typedef enum MS_BLE_STACK_EVENT_T {
    MS_BLE_STACK_EVENT_STACK_READY = 0,
    MS_BLE_STACK_EVENT_STACK_FAIL,
    MS_BLE_STACK_EVENT_ADV_ON,                  /* !< ???????????г?? */
    MS_BLE_STACK_EVENT_ADV_OFF,                /* !< ???????????г?? */
    MS_BLE_STACK_EVENT_DISCONNECT, /* !< ?????? */
    MS_BLE_STACK_EVENT_CONNECTED,  /* !< ?????? */
} ms_ble_stack_event_t;

typedef void (*ble_gatt_service_att_wirte_cb)(uint8_t *data, uint16_t size);
typedef void (*ble_gatt_service_att_read_cb)(uint8_t *data,  uint16_t *size);
typedef void (*ble_gatt_service_att_ind_cb)(uint8_t *data,  uint16_t size);

typedef struct ble_gatt_att_opr {
    ble_gatt_service_att_wirte_cb write_request;
    ble_gatt_service_att_read_cb  read_request;
    ble_gatt_service_att_ind_cb  ind_cb;
} ble_gatt_att_opr_t;

typedef struct ble_gatt_att_reg {
    sonata_gatt_att_desc_t att_desc;
    ble_gatt_att_opr_t  att_opr;
} ble_gatt_att_reg_t;

typedef void (*cb_fun)(ms_ble_stack_event_t);
typedef void (*app_ble_scan_callback_t)(uint8_t *data, uint16_t len);

extern cb_fun ble_cb_fun;

void ble_set_callback(cb_fun cb);
int ble_gatt_add_svc_helper(uint16_t *start_hdl, uint8_t nb_att, ble_gatt_att_reg_t *atts);
void ble_gatt_data_send(uint16_t local_handle, uint16_t idx, uint16_t length, uint8_t *p_value);
uint8_t ble_get_connect_status(void);
uint8_t ble_disconnect_request(void);
uint8_t ble_get_adv_status(void);
void ble_start_advertising(void);
void ble_set_max_mtu(uint16_t mtu);
uint16_t ble_get_mtu(void);
void ble_stop_adv(void);
void ble_gatt_add_srv_rsp(uint16_t handle);
void ble_gatt_read_request_handler(uint8_t connection_id, uint16_t handle, uint16_t *p_length, uint8_t **p_value);
void ble_gatt_write_request_handler(uint8_t connection_id, uint16_t handle, uint16_t length, uint8_t *p_value);
int ble_scan_start(app_ble_scan_callback_t cb);
int ble_scan_stop(void);
int ms_add_test_service(void);

#endif
