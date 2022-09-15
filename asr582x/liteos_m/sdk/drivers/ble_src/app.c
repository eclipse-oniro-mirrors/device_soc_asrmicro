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
 * @file app.c
 *
 * @brief Application entry point
 *
 ****************************************************************************************
 */

/**
 ****************************************************************************************
 * @addtogroup APP
 * @{
 ****************************************************************************************
 */

/*
 * INCLUDE FILES
 ****************************************************************************************
 */

#include <stdio.h>
#include "arch.h"                   // Application Definition
#include "sonata_gap.h"
#include "sonata_att.h"
#include "sonata_ble_api.h"
#include "sonata_gap_api.h"
#if !defined ALIOS_SUPPORT && !defined HARMONYOS_SUPPORT
#include "user_platform.h"
#endif
#include "sonata_ble_hook.h"
#include "sonata_gatt_api.h"
#include "sonata_api_task.h"
#include "sonata_gap.h"
#include "sonata_log.h"
#if !defined ALIOS_SUPPORT && !defined HARMONYOS_SUPPORT
#include "at_api.h"
#endif
#ifdef HARMONYOS_TEMP
#else

#endif
#include "app.h"
#if BLE_BATT_SERVER
#include "sonata_prf_bass_api.h"
#endif // BLE_BATT_SERVER
#ifdef SONATA_RTOS_SUPPORT
#include "lega_rtos.h"
#endif

/*
 * DEFINES
 ****************************************************************************************
 */
#define APP_BLE_ADV_ON   1
#define APP_BLE_ADV_OFF  0
#define APP_BLE_DEVICE_NAME_LEN  255
#define APP_BLE_CONNECT_MAX  10
#define APP_BLE_INVALID_CONIDX  0xFF
#define APP_BLE_ADV_MAX  0xB
#define APP_BLE_INVALID_ADVIDX  0xFF
#define APP_MAX_SERVICE_NUM 5

typedef struct ble_gatt_att_reg_list {
    uint16_t start_hdl;
    uint8_t nb_att;
    uint8_t state;
    uint8_t perm;
    uint8_t uuid[SONATA_ATT_UUID_128_LEN];
    ble_gatt_att_opr_t *att_opr_list;
    sonata_gatt_att_desc_t *att_desc;
} ble_gatt_att_reg_list_t;

typedef struct ble_gatt_att_manager {
    uint8_t reg_nb;
    uint8_t add_nb;
    ble_gatt_att_reg_list_t *reg_list[MAX_SERVICE_NUM];
} ble_gatt_att_manager_t;

/*!
 * @brief save local handle start index
 */
// Mark for profile dis
uint8_t ble_adv_set_state = APP_BLE_ADV_OFF;
uint8_t app_connected_state = APP_STATE_DISCONNECTED;
uint8_t target_address[SONATA_GAP_BD_ADDR_LEN] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
uint8_t ble_connect_id = 0xFF;
uint16_t read_handle = 0xFF;
uint16_t write_handle = 0xFF;
uint8_t write_uuid[16] = {0xF0, 0x80, 0x0};
enum BLE_ADV_STATE ble_adv_state = BLE_ADV_STATE_IDLE;
uint8_t read_uuid[16] = {0xF0, 0x80, 0x0};
uint8_t current_adv_id = 0;

bool bFound = false;
app_ble_scan_callback_t  p_scan_cb = NULL;
static bool app_adv_bonding = false;
static uint8_t app_connection_state = 0;
static peer_conn_param_t app_peer_conn_param = {0};

static bool app_bond = false;
static uint8_t app_loc_irk[KEY_LEN] = {0}; // Get in app_gap_gen_random_number_callback()
// static uint8_t app_loc_irk[KEY_LEN]= {0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x10,0x11,0x12,0x13,0x14,0x15,0x16}; // Get in app_gap_gen_random_number_callback()

static uint8_t app_rand_cnt = 0;
static uint32_t app_passkey_pincode = 0;

uint8_t app_csrk[SONATA_GAP_KEY_LEN] = {0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0};
uint8_t app_irk_addr[SONATA_GAP_BD_ADDR_LEN] = {0};
uint8_t app_irk_key[SONATA_GAP_KEY_LEN] = {0};

uint8_t app_loc_addr[SONATA_GAP_BD_ADDR_LEN] = {0x0C, 0x20, 0x18, 0xA7, 0x9F, 0xDD};
bonded_dev_info_list_t bonded_dev_info = {0};
uint8_t peer_dbaddr[SONATA_GAP_BD_ADDR_LEN] = {0};
uint8_t local_dev_name[APP_BLE_DEVICE_NAME_LEN] = "asr_ble_demo";
static connect_req_info_t  connect_req_list [APP_BLE_CONNECT_MAX] = { 0 };
// static adv_idx_info_t adv_idx_tbl[APP_BLE_ADV_MAX];
static enum sonata_gap_io_cap       app_iocap    = SONATA_GAP_IO_CAP_NO_INPUT_NO_OUTPUT;
static enum sonata_gap_auth         app_auth     = SONATA_GAP_AUTH_REQ_MITM_BOND;
static  uint8_t  app_req_auth = SONATA_GAP_AUTH_REQ_NO_MITM_NO_BOND;

static ble_gatt_att_manager_t  service_reg_env;
ble_adv_data_set_t app_ble_adv_data;
ble_scan_data_set_t app_ble_scan_data;
static uint16_t app_duration = 0;
static uint8_t app_max_adv_evt = 0;
static app_core_evt_ind_cb app_evt_cb = NULL;
static app_sec_req_cb sec_req_call = NULL;
uint8_t  adv_value[32] = {0x02, 0x01, 0x06, 0x06, 0x09, 0x6d, 0x69, 0x64, 0x64, 0x64, 0x12, 0xFF, 0xA8, 0x06, 0x01 \
                          , 0x31, 0x38, 0x32, 0x37, 0x33, 0x36, 0x34, 0x35, 0x46, 0x43, 0x30, 0x30, 0x30, 0x34
                         };
uint16_t adv_length = 29;
static struct sonata_gap_ltk app_ltk = { 0 };
static uint8_t need_connect_confirm = 0;
static app_env gAppEnv;

/*
 * TYPE DEFINITIONS
 ****************************************************************************************
 */
static uint16_t app_ble_set_adv_data(uint8_t adv_id);

/*
 * ENUMERATIONS
 ****************************************************************************************
 */
/*
void print_serv_env(void)
{

    printf("service_reg_env.reg_nb %d %d\r\n",service_reg_env.reg_nb,service_reg_env.add_nb);
    for(int i = 0; i < service_reg_env.reg_nb; i++)
    {
        printf("list :0x%lx %d\r\n",service_reg_env.reg_list[i],service_reg_env.reg_list[i]->nb_att);
    }
}*/
/*!
 * @brief
 */
typedef enum {
    // Peripheral Bonded
    SONATA_TAG_PERIPH_BONDED              = (APP_DATA_SAVE_TAG_FIRST + 0),
    // Mouse NTF Cfg
    SONATA_TAG_MOUSE_NTF_CFG,
    // Mouse Timeout value
    SONATA_TAG_MOUSE_TIMEOUT,
    // Peer Device BD Address
    SONATA_TAG_PEER_BD_ADDRESS,
    // EDIV (2bytes), RAND NB (8bytes),  LTK (16 bytes), Key Size (1 byte)
    SONATA_TAG_LTK,
    // app_ltk_key_in
    SONATA_TAG_LTK_IN,
    // app irk addr
    SONATA_TAG_IRK_ADDR,
    // app irk
    SONATA_TAG_IRK,
    // device address
    SONATA_TAG_BD_ADDRESS,
    // bonded dev info
    SONATA_TAG_BONDED_DEV_INFO,
    // start pair on boot
    SONATA_TAG_PAIR_ON_BOOT,
} sonata_app_nvds_tag;

/*!
 * @brief
 */
typedef enum {
    // Peripheral Bonded len
    SONATA_LEN_PERIPH_BONDED              = 1,
    // Mouse NTF Cfg len
    SONATA_LEN_MOUSE_NTF_CFG              = 2,
    // Mouse Timeout value len
    SONATA_LEN_MOUSE_TIMEOUT              = 2,
    // Peer Device BD Address len
    SONATA_LEN_PEER_BD_ADDRESS            = 6,
    // EDIV (2bytes), RAND NB (8bytes),  LTK (16 bytes), Key Size (1 byte)
    SONATA_LEN_LTK                        = 27,
    // app_ltk_key_in len
    SONATA_LEN_LTK_IN                     = 16,
    // app irk addr len
    SONATA_LEN_IRK_ADDR                   = 6,
    // app irk len
    SONATA_LEN_IRK                        = 16,
    // device address
    SONATA_LEN_BD_ADDRESS                 = 6,
    // bonded dev info len
    SONATA_LEN_BONDED_DEV_INFO            = 218, // 218: 3, 290:4,
    // start pair on boot
    SONATA_LEN_PAIR_ON_BOOT               = 1,
} sonata_app_nvds_len;

/*!
 * @brief
 */
typedef enum {
    SONATA_SERVICE_INIT                     = 0,
    SONATA_SERVICE_ENABLE,
    SONATA_SERVICE_DISABLE
} sonata_serivice_state;

/*
 * EXTERNAL FUNCTION DECLARATION
 ****************************************************************************************
 */
extern int __wrap_printf(const char *format, ...);
static uint8_t app_timer_handler(uint16_t id);

/*
 * LOCAL FUNCTION DEFINITIONS
 ****************************************************************************************
 */

static sonata_app_timer_callback_t app_timer_callbacks = {
    .timeout                  = app_timer_handler,
};

static void app_print_hex(uint8_t *hex, uint8_t len)
{
    for (int i = 0; i < len; i++) {
        APP_TRC("%x%x", hex[i] >> 4, hex[i] & 0xf);
    }
    APP_TRC("\r\n");
}

void app_data_init(void)
{
    memset(connect_req_list, 0xff, sizeof(connect_req_list));
    // memset(adv_idx_tbl,0xff,sizeof(adv_idx_tbl));
}

void app_connect_req_list_add(uint8_t *addr, uint8_t conidx)
{
    for (int idx = 0; idx < APP_BLE_CONNECT_MAX; idx++) {
        if (connect_req_list[idx].conidx == APP_BLE_INVALID_CONIDX) {
            connect_req_list[idx].conidx = conidx;
            memmove(connect_req_list[idx].bd_addr, addr, APP_BD_ADDR_LEN);
            return;
        }
    }
    APP_TRC("APP: %s  no space\r\n", __FUNCTION__);
}

static void app_connect_req_list_del(uint8_t conidx)
{
    for (int idx = 0; idx < APP_BLE_CONNECT_MAX; idx++) {
        if (connect_req_list[idx].conidx == conidx) {
            connect_req_list[idx].conidx = APP_BLE_INVALID_CONIDX;
            return;
        }
    }
    APP_TRC("APP: %s  not found\r\n", __FUNCTION__);
}

static uint8_t  app_get_conidx_by_addr(uint8_t *addr)
{
    for (int idx = 0; idx < APP_BLE_CONNECT_MAX; idx++) {
        if (!memcmp(connect_req_list[idx].bd_addr, addr, APP_BD_ADDR_LEN)) {
            return connect_req_list[idx].conidx;
        }
    }
    APP_TRC("APP: %s  not found\r\n", __FUNCTION__);
    return APP_BLE_INVALID_CONIDX;
}

static uint8_t  *app_get_addr_by_conidx(uint8_t  conidx)
{
    for (int idx = 0; idx < APP_BLE_CONNECT_MAX; idx++) {
        if (connect_req_list[idx].conidx == conidx) {
            return connect_req_list[idx].bd_addr;
        }
    }
    APP_TRC("APP: %s  not found\r\n", __FUNCTION__);
    return NULL;
}

void app_ble_set_device_name(uint8_t *name, uint32_t len)
{
    APP_TRC("APP: %s  %d\r\n", __FUNCTION__, len);
    if (APP_BLE_DEVICE_NAME_LEN < len) {
        return;
    }
    memmove(local_dev_name, name, len);
}

/*
uint8_t app_adv_idx_list_alloc(void)
{
    for(int idx = 0; idx < APP_BLE_ADV_MAX; idx++)
    {
        if(adv_idx_tbl[idx].local_idx == APP_BLE_INVALID_ADVIDX)
        {
             adv_idx_tbl[idx].local_idx = idx;
             return idx;
        }
    }
    APP_TRC("APP: %s  no space\r\n", __FUNCTION__);
}

void app_adv_idx_list_free(uint8_t lid)
{
    if(lid < APP_BLE_ADV_MAX)
    {
        adv_idx_tbl[lid].local_idx = APP_BLE_INVALID_ADVIDX;
    }
    else APP_TRC("APP: %s  not found\r\n", __FUNCTION__);
}

void app_adv_idx_list_set_advid(uint8_t lid,uint8_t adv_id)
{
    if(lid < APP_BLE_ADV_MAX)
    {
        adv_idx_tbl[lid].adv_id = adv_id;
    }
    else APP_TRC("APP: %s  not found\r\n", __FUNCTION__);
}

uint8_t  app_get_adv_idx_by_lid(uint8_t  lid)
{
    for(int idx = 0; idx < APP_BLE_ADV_MAX; idx++)
    {
        if(adv_idx_tbl[idx].local_idx == lid)
        {
             return adv_idx_tbl[idx].adv_id;
        }
    }
    APP_TRC("APP: %s  not found\r\n", __FUNCTION__);
    return APP_BLE_INVALID_CONIDX;
}
*/
/*!
 * @brief config legacy advertising
 */
void app_ble_config_legacy_advertising(void)
{
    APP_TRC("APP: %s  \r\n", __FUNCTION__);

    sonata_gap_directed_adv_create_param_t param = {0};
    param.disc_mode = SONATA_GAP_ADV_MODE_GEN_DISC;
    param.prop = SONATA_GAP_ADV_PROP_UNDIR_CONN_MASK;
    // param.max_tx_pwr = 0xE2;
    param.filter_pol = SONATA_ADV_ALLOW_SCAN_ANY_CON_ANY;
    //    msg->adv_param.adv_param.peer_addr.addr.addr:00
    param.addr_type = SONATA_GAP_STATIC_ADDR;
    param.adv_intv_min = BLE_MCRC_MIN_INTERVAL;
    param.adv_intv_max = BLE_MCRC_MIN_INTERVAL;
    param.chnl_map = MS_BLE_CHANNEL_NUM;
    param.phy = SONATA_GAP_PHY_LE_1MBPS;
    ble_adv_state = BLE_ADV_STATE_CREATING;
    uint16_t ret = sonata_ble_config_legacy_advertising(SONATA_GAP_STATIC_ADDR,
                   &param); // Next event:SONATA_GAP_CMP_ADVERTISING_CONFIG
    if (ret != API_SUCCESS) {
        APP_TRC("APP: %s  ERROR:%02X\r\n", __FUNCTION__, ret);
        ble_adv_state = BLE_ADV_STATE_IDLE;
    }
}

void app_ble_config_legacy_advertising_with_param(uint8_t own_addr_type, sonata_gap_directed_adv_create_param_t *param)
{
    APP_TRC("APP: %s  \r\n", __FUNCTION__);

    ble_adv_state = BLE_ADV_STATE_CREATING;
    uint16_t ret = sonata_ble_config_legacy_advertising(own_addr_type, param); // Next event:SONATA_GAP_CMP_ADVERTISING_CONFIG
    if (ret != API_SUCCESS) {
        APP_TRC("APP: %s  ERROR:%02X\r\n", __FUNCTION__, ret);
        ble_adv_state = BLE_ADV_STATE_IDLE;
    }
}

void app_ble_start_advertising_with_param(sonata_gap_directed_adv_create_param_t *param, ble_adv_data_set_t *data,
        ble_scan_data_set_t *scan_data, uint8_t own_addr_type, uint16_t duration, uint8_t max_adv_evt)
{
    APP_TRC("APP: %s  \r\n", __FUNCTION__);
    app_ble_adv_data.advdataLen = data->advdataLen;
    memcpy((void *)app_ble_adv_data.advdata, (void *)data->advdata, data->advdataLen);
    if (scan_data != NULL && scan_data->respdataLen != 0) {
        app_ble_scan_data.respdataLen = scan_data->respdataLen;
        memcpy((void *)app_ble_scan_data.respdata, (void *)scan_data->respdata, scan_data->respdataLen);
    }

    app_duration = duration;
    app_max_adv_evt = max_adv_evt ;

    ble_adv_set_state = APP_BLE_ADV_ON;

    if ((ble_adv_state != BLE_ADV_STATE_IDLE) &&
        (ble_adv_state != BLE_ADV_STATE_CREATED) && ble_adv_state != BLE_ADV_STATE_STARTED) {
        return ;
    }

    if (ble_adv_state == BLE_ADV_STATE_CREATED) {
        ble_adv_state =  BLE_ADV_STATE_SETTING_ADV_DATA;
        app_ble_set_adv_data(current_adv_id);
    } else if (ble_adv_state == BLE_ADV_STATE_STARTED) {
        app_ble_set_adv_data(current_adv_id);
    } else {
        app_ble_config_legacy_advertising_with_param(own_addr_type, param);
    }
    return ;

}

/*!
 * @brief set advertising data
 */
static uint16_t app_ble_set_adv_data(uint8_t adv_id)
{
    APP_TRC("APP: %s  \r\n", __FUNCTION__);

    // Call API
    uint16_t ret;

    if (app_ble_adv_data.advdataLen >= 3) {
        ret = sonata_ble_set_advertising_data_byid(adv_id, app_ble_adv_data.advdataLen - 3, &app_ble_adv_data.advdata[3]);
    } else {
        ret = sonata_ble_set_advertising_data_byid(adv_id, 0, &app_ble_adv_data.advdata[3]);
    }
    printf("adv_data:%d\r\n", adv_id);
    // Next event:SONATA_GAP_CMP_SET_ADV_DATA
    if (ret != API_SUCCESS) {
        APP_TRC("APP: %s  ERROR:%02X\r\n", __FUNCTION__, ret);
    }
    return ret;
}

void app_ble_set_adv_data_default(void)
{
    APP_TRC("APP: %s  \r\n", __FUNCTION__);
    uint8_t advData[] = { // Advertising data format
        8, SONATA_GAP_AD_TYPE_COMPLETE_NAME, 'A', 'S', 'R', '-', '0', '0', '0',
        3, SONATA_GAP_AD_TYPE_COMPLETE_LIST_16_BIT_UUID, 0x12, 0x18,
        3, SONATA_GAP_AD_TYPE_APPEARANCE, 0xC1, 0x03        // 0x03C1: HID Keyboard
    };
    // Call API
    uint16_t ret = sonata_ble_set_advertising_data(sizeof(advData), advData);
    // Next event:SONATA_GAP_CMP_SET_ADV_DATA
    if (ret != API_SUCCESS) {
        APP_TRC("APP: %s  ERROR:%02X\r\n", __FUNCTION__, ret);
    }
}

static uint16_t app_ble_set_scansponse_data(uint8_t adv_id)
{
    APP_TRC("APP: %s  \r\n", __FUNCTION__);

    // Call API

    uint16_t ret = sonata_ble_set_scan_response_data_byid(adv_id, app_ble_scan_data.respdataLen,
                   app_ble_scan_data.respdata);
    // Next event:SONATA_GAP_CMP_SET_ADV_DATA
    if (ret != API_SUCCESS) {
        APP_TRC("APP: %s  ERROR:%02X\r\n", __FUNCTION__, ret);
        ble_adv_state = BLE_ADV_STATE_CREATED;
    }
    return ret;
}
uint8_t app_get_adv_status()
{
    if (ble_adv_state == BLE_ADV_STATE_IDLE || ble_adv_state == BLE_ADV_STATE_STOPPING
        || ble_adv_state == BLE_ADV_STATE_CREATED) {
        return 0;
    }
    return 1;

}

static void app_ble_print_info(void)
{
    // Print device info
    //    uint8_t dev_name_saved[APPNV_LEN_LOCAL_DEVICE_NAME] = {0};
    //    app_nv_get_local_device_name(dev_name_saved, true);
    //    printf("\r\n----BLE NAME[%s] ----\r\n", dev_name_saved + 1);
    uint8_t *addr = sonata_get_bt_address();
    printf("\r\n----BLE MAC[%02X:%02X:%02X:%02X:%02X:%02X] ----\r\n",
           addr[5], addr[4], addr[3], addr[2], addr[1], addr[0]);
    printf("\r\n");

}
uint16_t app_ble_start_advertising(uint8_t adv_id)
{
    APP_TRC("APP: %s  adv %d\r\n", __FUNCTION__, adv_id);
    // Call api

    uint16_t ret = sonata_ble_start_advertising_byid(adv_id, app_duration, app_duration);
    // Next event:SONATA_GAP_CMP_ADVERTISING_START
    if (ret != API_SUCCESS) {
        APP_TRC("APP: %s  ERROR:%02X\r\n", __FUNCTION__, ret);
    }
    // app_ble_print_info();
    ble_adv_set_state = APP_BLE_ADV_ON;

    return ret;

}

uint16_t app_ble_advertising_stop(uint8_t adv_id)
{
    APP_TRC("APP: %s  dv state %d \r\n", __FUNCTION__, ble_adv_state);
    ble_adv_set_state = APP_BLE_ADV_OFF;

    if (ble_adv_state != BLE_ADV_STATE_STARTED) {
        return API_FAILURE;
    }
    // Call api
    ble_adv_state =  BLE_ADV_STATE_STOPPING;
    uint16_t ret = sonata_ble_stop_advertising_byid(adv_id);
    // Next event:SONATA_GAP_CMP_ADVERTISING_START
    if (ret != API_SUCCESS) {
        APP_TRC("APP: %s  ERROR:%02X\r\n", __FUNCTION__, ret);
    }
    return ret;
}

uint16_t app_ble_stop_adv(uint8_t adv_id)
{
    APP_TRC("APP: %s  dv state %d \r\n", __FUNCTION__, ble_adv_state);
    ble_adv_set_state = APP_BLE_ADV_OFF;

    if (ble_adv_state != BLE_ADV_STATE_STARTED) {
        return API_FAILURE;
    }
    uint16_t ret = sonata_ble_stop_advertising_byid(adv_id);
    // Next event:SONATA_GAP_CMP_ADVERTISING_START
    if (ret != API_SUCCESS) {
        APP_TRC("APP: %s  ERROR:%02X\r\n", __FUNCTION__, ret);
    }
    return ret;
}

uint16_t app_ble_stop_adv_without_id(void)
{
    return app_ble_stop_adv(current_adv_id);
}

int app_ble_advertising_start(uint8_t *adv_id, ble_adv_data_set_t *data, ble_scan_data_set_t *scan_data)
{

    APP_TRC("app_ble_advertising_start!!!!!! %d %d %d\r\n", data->advdataLen, scan_data->respdataLen, *adv_id);
    app_ble_adv_data.advdataLen = data->advdataLen;
    app_ble_scan_data.respdataLen = scan_data->respdataLen;

    memmove((void *)app_ble_adv_data.advdata, (void *)data->advdata, data->advdataLen);
    memmove((void *)app_ble_scan_data.respdata, (void *)scan_data->respdata, scan_data->respdataLen);

    ble_adv_set_state = APP_BLE_ADV_ON;

    if ((ble_adv_state != BLE_ADV_STATE_IDLE) &&
        (ble_adv_state != BLE_ADV_STATE_CREATED) && ble_adv_state != BLE_ADV_STATE_STARTED) {
        return 0;
    }

    if (ble_adv_state == BLE_ADV_STATE_CREATED) {
        ble_adv_state =  BLE_ADV_STATE_SETTING_ADV_DATA;
        app_ble_set_adv_data(current_adv_id);
    } else if (ble_adv_state == BLE_ADV_STATE_STARTED) {
        app_ble_set_adv_data(current_adv_id);
    } else {
        app_ble_config_legacy_advertising();
    }
    return 0;

}

/*!
 * @brief Config scanning
 */
void app_ble_config_scanning(void)
{
    APP_TRC("APP: %s  \r\n", __FUNCTION__);
    uint16_t ret = sonata_ble_config_scanning(SONATA_GAP_STATIC_ADDR);
    // Next event:SONATA_GAP_CMP_SCANNING_CONFIG
    if (ret != API_SUCCESS) {
        APP_TRC("APP: %s  ERROR:%02X\r\n", __FUNCTION__, ret);
    }
}

/*!
 * @brief Start scanning
 */
void app_ble_start_scanning(void)
{
    APP_TRC("APP: %s  \r\n", __FUNCTION__);
    bFound = false;
    sonata_gap_scan_param_t param = {0};
    param.type = SONATA_GAP_SCAN_TYPE_OBSERVER;
    // For continuous scan, use OBSERVER type, use duration to control scan timeer.
    // if duration=0, will scan for ever until sonata_ble_stop_scanning() called
    // param.type = SONATA_GAP_SCAN_TYPE_OBSERVER;
    param.prop = SONATA_GAP_SCAN_PROP_ACTIVE_1M_BIT | SONATA_GAP_SCAN_PROP_PHY_1M_BIT; // 0x05
    param.dup_filt_pol = SONATA_GAP_DUP_FILT_EN;
    param.scan_param_1m.scan_intv = 0x0140;
    param.scan_param_1m.scan_wd = 0x00A0;
    param.scan_param_coded.scan_intv = 0x0140;
    param.scan_param_coded.scan_wd = 0x00A0;
    param.duration = 0;
    param.period = 0;
    uint16_t ret = sonata_ble_start_scanning(&param);
    // Scan result will show in app_gap_scan_result_callback()
    if (ret != API_SUCCESS) {
        APP_TRC("APP: %s  ERROR:%02X\r\n", __FUNCTION__, ret);
    }

}

void app_ble_stop_scanning(void)
{
    sonata_ble_stop_scanning();
}

/*!
 * @brief config initiating
 */
void app_ble_config_initiating(void)
{
    APP_TRC("APP: %s  \r\n", __FUNCTION__);
    // Call api to config init
    uint16_t ret = sonata_ble_config_initiating(SONATA_GAP_STATIC_ADDR);
    // Next event:SONATA_GAP_CMP_INITIATING_CONFIG
    if (ret != API_SUCCESS) {
        APP_TRC("APP: %s  ERROR:%02X\r\n", __FUNCTION__, ret);
    }
}

/*!
 * @brief Check MAC address
 * @param address
 * @return
 */
static bool app_ble_check_address(uint8_t *address)
{
    uint8_t error_address[SONATA_GAP_BD_ADDR_LEN] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
    if (address == NULL) {
        return false;
    }
    if (memcmp(address, error_address, SONATA_GAP_BD_ADDR_LEN) == 0) {
        return false;
    }
    return true;
}

/*!
 * @brief Start initiating
 */
void app_ble_start_initiating(uint8_t *target)
{
    APP_TRC("APP: %s  \r\n", __FUNCTION__);
    if (app_ble_check_address(target) == false) {
        APP_TRC("APP: %s, Target address is not right. Stop\r\n", __FUNCTION__);
        return;
    }
    sonata_gap_init_param_t param = {0};
    param.type = SONATA_GAP_INIT_TYPE_DIRECT_CONN_EST;
    param.prop = SONATA_GAP_INIT_PROP_1M_BIT | SONATA_GAP_INIT_PROP_2M_BIT | SONATA_GAP_INIT_PROP_CODED_BIT;
    param.conn_to = 0;
    param.peer_addr.addr_type = SONATA_GAP_STATIC_ADDR; //  Addr
    memcpy(param.peer_addr.addr.addr, target, SONATA_GAP_BD_ADDR_LEN);

    if (param.prop & SONATA_GAP_INIT_PROP_1M_BIT) {
        APP_TRC("APP: %s  (%02X)set SONATA_GAP_INIT_PROP_1M_BIT \r\n", __FUNCTION__, param.prop);
        param.scan_param_1m.scan_intv = 0x0200;
        param.scan_param_1m.scan_wd = 0x0100;
        param.conn_param_1m.conn_intv_min = 0x0028;
        param.conn_param_1m.conn_intv_max = 0x0028;
        param.conn_param_1m.conn_latency = 0;
        param.conn_param_1m.supervision_to = 0x02BC;
        param.conn_param_1m.ce_len_min = 0x0003;
        param.conn_param_1m.ce_len_max = 0x0003;
    }
    if (param.prop & SONATA_GAP_INIT_PROP_2M_BIT) {
        APP_TRC("APP: %s  (%02X)set SONATA_GAP_INIT_PROP_2M_BIT \r\n", __FUNCTION__, param.prop);

        param.conn_param_2m.conn_intv_min = 0x0028;
        param.conn_param_2m.conn_intv_max = 0x0028;
        param.conn_param_2m.conn_latency = 0;
        param.conn_param_2m.supervision_to = 0x02BC;
        param.conn_param_2m.ce_len_min = 0x0003;
        param.conn_param_2m.ce_len_max = 0x0003;
    }
    if (param.prop & SONATA_GAP_INIT_PROP_CODED_BIT) {
        APP_TRC("APP: %s  (%02X)set SONATA_GAP_INIT_PROP_CODED_BIT \r\n", __FUNCTION__, param.prop);
        param.scan_param_coded.scan_intv = 0x0200;
        param.scan_param_coded.scan_wd = 0x0100;
        param.conn_param_coded.conn_intv_min = 0x0028;
        param.conn_param_coded.conn_intv_max = 0x0028;
        param.conn_param_coded.conn_latency = 0;
        param.conn_param_coded.supervision_to = 0x02BC;
        param.conn_param_coded.ce_len_min = 0003;
        param.conn_param_coded.ce_len_max = 0003;
    }

    uint16_t ret = sonata_ble_start_initiating(&param);
    // Next event:If connected, SONATA_GAP_CMP_INITIATING_DELETE event will be received
    if (ret != API_SUCCESS) {
        APP_TRC("APP: %s  ERROR:%02X\r\n", __FUNCTION__, ret);
    }
}

/*!
 * @brief
 * @return
 */
void app_ble_stop_initiating(void)
{
    sonata_ble_stop_initiating();
}

void app_ble_disconnect(uint8_t conidx)
{
    app_connect_req_list_del(conidx);
    sonata_ble_gap_disconnect(conidx, 0);
    ble_connect_id = 0xFF;
}

uint8_t app_get_connect_status(void)
{
    if (app_connected_state == APP_STATE_CONNECTED) {
        return 1;
    }
    return 0;
}

void app_print_adv_status(void)
{
    printf("adv_ble_state %d  set state %d \r\n", ble_adv_state, ble_adv_set_state) ;
    for (int i = 0; i < sizeof(write_uuid); i++) {
        printf("%x", write_uuid[i]);
    }
    printf("uuid\r\n");
    for (int i = 0; i < sizeof(write_uuid); i++) {
        printf("%x", read_uuid[i]);
    }
}

/*!
 * @brief
 */
static void app_ble_on(void)
{
    APP_TRC("APP: %s  \r\n", __FUNCTION__);
    sonata_gap_set_dev_config_cmd cmd = {0};
    cmd.role = SONATA_GAP_ROLE_ALL;
    cmd.gap_start_hdl = 0;
    cmd.gatt_start_hdl = 0;
    cmd.renew_dur = 0x0096;
    cmd.privacy_cfg = 0;
    cmd.pairing_mode = SONATA_GAP_PAIRING_SEC_CON | SONATA_GAP_PAIRING_LEGACY;
    cmd.att_cfg = 0x0080;
    cmd.max_mtu = 512;
    cmd.max_mps = 0x02A0;
    cmd.max_nb_lecb = 0x0A;
    cmd.hl_trans_dbg = false;

    // Get bond status from FS
    uint8_t length = SONATA_LEN_PERIPH_BONDED;
    if (sonata_fs_read(SONATA_TAG_PERIPH_BONDED, &length, &app_bond) != SONATA_FS_OK) {
        // If read value is invalid, set status to not bonded
        app_bond = 0;
    }
    if (app_bond == 1) {
        memcpy(cmd.irk.key, app_loc_irk, sizeof(cmd.irk.key));
    }
    length = SONATA_LEN_BONDED_DEV_INFO;
    if (sonata_fs_read(SONATA_TAG_BONDED_DEV_INFO, &length, (uint8_t *)&bonded_dev_info) != SONATA_FS_OK) {
        APP_TRC("APP: %s  not bonded device info  \r\n", __FUNCTION__);
    }

    uint16_t ret = sonata_ble_on(&cmd); // Next event:SONATA_GAP_CMP_BLE_ON
    if (ret != API_SUCCESS) {
        APP_TRC("APP: %s  ERROR:%02X\r\n", __FUNCTION__, ret);
    }
}

extern void ble_reset_cmp(void);
/*
 * LOCAL FUNCTION DEFINITIONS    Callback functions
 ****************************************************************************************
 */
static void test_adv_h(void)
{
    sonata_gap_directed_adv_create_param_t param = {0};
    param.disc_mode = SONATA_GAP_ADV_MODE_GEN_DISC;
    param.prop = SONATA_GAP_ADV_PROP_UNDIR_CONN_MASK;
    // param.max_tx_pwr = 0xE2;
    param.filter_pol = SONATA_ADV_ALLOW_SCAN_ANY_CON_ANY;
    //    msg->adv_param.adv_param.peer_addr.addr.addr:00
    param.addr_type = SONATA_GAP_STATIC_ADDR;
    param.adv_intv_min = BLE_MCRC_MIN_INTERVAL;
    param.adv_intv_max = BLE_MCRC_MIN_INTERVAL;
    param.chnl_map = MS_BLE_CHANNEL_NUM;
    param.phy = SONATA_GAP_PHY_LE_1MBPS;
    ble_adv_data_set_t data_Set;
    memmove(data_Set.advdata, adv_value, adv_length);
    data_Set.advdataLen = adv_length;

    app_ble_start_advertising_with_param(&param, &data_Set, NULL, SONATA_GAP_STATIC_ADDR, 0, 0);
}
/*!
 * @brief BLE complete event handler
 * @param opt_id @see sonata_ble_complete_type
 * @param status complete status
 * @param param param and deparam will be difference according to difference complete event.
 * @param dwparam param and deparam will be difference according to difference complete event.
 * @return
 */
static uint16_t app_ble_complete_event_handler(sonata_ble_complete_type opt_id, uint8_t status, uint16_t param,
        uint32_t dwparam)
{
    APP_TRC("APP_COMPLETE: %s  opt_id=%04X,status=%02X,param=%04X,dwparam=%lu\r\n", __FUNCTION__, opt_id, status, param,
            dwparam);
    uint16_t cb_result = CB_DONE;
    switch (opt_id) {
        case SONATA_GAP_CMP_BLE_ON: // 0x0F01
            // ble_adv_set_state = APP_BLE_ADV_ON;
            // test_adv_h();
            if (ble_cb_fun != NULL) {
                ble_cb_fun(MS_BLE_STACK_EVENT_STACK_READY);
            }
            break;
        case SONATA_GAP_CMP_ADVERTISING_CONFIG: // 0x0F02
            current_adv_id = param;
            if (ble_adv_set_state == APP_BLE_ADV_ON) {
                ble_adv_state = BLE_ADV_STATE_SETTING_ADV_DATA;
                if (API_SUCCESS != app_ble_set_adv_data(current_adv_id)) {
                    ble_adv_state = BLE_ADV_STATE_CREATED;
                }
            } else {
                ble_adv_state = BLE_ADV_STATE_CREATED;
                if (ble_cb_fun != NULL) {
                    ble_cb_fun(MS_BLE_STACK_EVENT_ADV_OFF);
                }
                app_ble_set_adv_data_default();
            }
            break;
        case SONATA_GAP_CMP_SET_ADV_DATA: // 0x01A9
            if (ble_adv_set_state == APP_BLE_ADV_ON) {
                if (API_SUCCESS == app_ble_set_scansponse_data(current_adv_id)) {
                    if (ble_adv_state ==  BLE_ADV_STATE_SETTING_ADV_DATA) {
                        ble_adv_state = BLE_ADV_STATE_SETTING_SCAN_RSP_DATA;
                    }
                } else {

                }
                app_ble_start_advertising(0);

            } else {
                if (ble_adv_state  == BLE_ADV_STATE_STARTED) {
                    if (API_SUCCESS == app_ble_stop_adv(current_adv_id)) {
                        ble_adv_state = BLE_ADV_STATE_STOPPING;
                    }
                } else {
                    ble_adv_state = BLE_ADV_STATE_CREATED;
                    if (ble_cb_fun != NULL) {
                        ble_cb_fun(MS_BLE_STACK_EVENT_ADV_OFF);
                    }
                }
                app_ble_start_advertising(0);

            }
            break;
        case SONATA_GAP_CMP_SET_SCAN_RSP_DATA:
            if (ble_adv_set_state == APP_BLE_ADV_ON) {
                if (ble_adv_state == BLE_ADV_STATE_SETTING_SCAN_RSP_DATA) {
                    if (API_SUCCESS == app_ble_start_advertising(current_adv_id)) {
                        ble_adv_state = BLE_ADV_STATE_STARTING;
                    } else {
                        ble_adv_state = BLE_ADV_STATE_CREATED;
                    }
                } else {
                    // adv is on ,only update data
                }
            } else {

                if (ble_adv_state  == BLE_ADV_STATE_STARTED) {
                    if (API_SUCCESS == app_ble_stop_adv(current_adv_id)) {
                        ble_adv_state = BLE_ADV_STATE_STOPPING;
                    }
                } else {
                    ble_adv_state = BLE_ADV_STATE_CREATED;
                    if (ble_cb_fun != NULL) {
                        ble_cb_fun(MS_BLE_STACK_EVENT_ADV_OFF);
                    }
                }
            }
            break;
        case SONATA_GAP_CMP_ADVERTISING_START: // 0x0F06

            APP_TRC("ble_adv_state: %d    set state %d\r\n", ble_adv_state, ble_adv_set_state);
            ble_adv_state = BLE_ADV_STATE_STARTED;
            if (ble_adv_set_state == APP_BLE_ADV_ON) {
                if (ble_cb_fun != NULL) {
                    APP_TRC("ble_cb_fun\r\n");
                    ble_cb_fun(MS_BLE_STACK_EVENT_ADV_ON);

                }
                if (app_evt_cb != NULL) {
                    app_adv_status_ind_t status_ind;
                    status_ind.advId = param;
                    status_ind.status = status;
                    app_evt_cb(BLE_ADV_START, (void *)&status_ind);
                }
            } else {
                if (API_SUCCESS == app_ble_stop_adv(current_adv_id)) {
                    ble_adv_state = BLE_ADV_STATE_STOPPING;
                }
            }
            break;
        case SONATA_GAP_CMP_ADVERTISING_STOP:
            ble_adv_state = BLE_ADV_STATE_CREATED;
            if (ble_adv_set_state == APP_BLE_ADV_OFF) {
                if (ble_cb_fun != NULL) {
                    ble_cb_fun(MS_BLE_STACK_EVENT_ADV_OFF);
                }
            } else {
                if (API_SUCCESS == app_ble_start_advertising(current_adv_id)) {
                    ble_adv_state = BLE_ADV_STATE_STARTING;
                }
            }
            break;
        case  SONATA_GAP_CMP_ADVERTISING_DELETE:
            APP_TRC("SONATA_GAP_CMP_ADVERTISING_DELETE %d!!!! \r\n", param);
            ble_adv_state = BLE_ADV_STATE_IDLE;
            break;
        case SONATA_GAP_CMP_SCANNING_CONFIG: // 0x0F03
            app_ble_start_scanning();
            break;
        case SONATA_GAP_CMP_SCANNING_START: // 0x0F07
            APP_TRC("ACP: %s  SONATA_GAP_CMP_SCANNING_START \r\n", __FUNCTION__);
            // app_add_profiles();
            break;
        case SONATA_GAP_CMP_PROFILE_TASK_ADD: // 0x011B
            break;
        case SONATA_GAP_CMP_SCANNING_STOP: // 0x0F08
            cb_result = CB_REJECT; // delete scan instance
            break;
        case SONATA_GAP_CMP_SCANNING_DELETE: // 0x0F0F
            break;
        case SONATA_GAP_CMP_INITIATING_CONFIG: // 0x0F04
            app_ble_start_initiating(target_address);
            break;
        case SONATA_GAP_CMP_INITIATING_STOP:
            cb_result = CB_REJECT; // delete scan instance
            break;
        case SONATA_GAP_CMP_INITIATING_DELETE: // 0x0F10
            if (gAppEnv.appUuids.service != 0) {
                sonata_ble_gatt_disc_all_characteristic(ble_connect_id, 1, 0XFFFF);
            }
            // sonata_ble_gatt_disc_all_descriptor(ble_connect_id, 1, 0XFFFF);
            break;
        case SONATA_GATT_CMP_NOTIFY:
            APP_TRC("APP_COMPLETE: %s  SONATA_GATT_CMP_NOTIFY, seq:%d \r\n", __FUNCTION__, (uint16_t)dwparam);
            break;
        case SONATA_GATT_CMP_DISC_ALL_SVC: // 0x0402
            // sonata_ble_gatt_read_by_handle(param, demo_handle_id);
            break;
        case SONATA_GATT_CMP_READ: // 0x0408
            break;
        case SONATA_GAP_CMP_SECURITY_REQ:
            APP_TRC("APP_COMPLETE: %s  SONATA_GAP_CMP_SECURITY_REQ, seq:%d \r\n", __FUNCTION__, (uint16_t) dwparam);
            break;
        case SONATA_GAP_CMP_GEN_RAND_NB:
            APP_TRC("APP_COMPLETE: %s  SONATA_GAP_CMP_GEN_RAND_NB,  \r\n", __FUNCTION__);
            if (app_rand_cnt == 1) {
                // Generate a second random number
                app_rand_cnt++;
                sonata_ble_gap_generate_random_number();

            } else {
                app_rand_cnt = 0;
                sonata_ble_gap_set_irk(app_loc_irk);

            }
            break;
        case SONATA_GAP_CMP_SET_IRK:
            APP_TRC("APP_COMPLETE: %s  SONATA_GAP_CMP_SET_IRK,  \r\n", __FUNCTION__);
            app_rand_cnt = 0;
            break;
        case   SONATA_GATT_CMP_INDICATE:
            APP_TRC("APP_COMPLETE: %s  SONATA_GATT_CMP_INDICATE %d,  \r\n", __FUNCTION__, status);
            if (app_evt_cb != NULL) {
                app_ind_sent_ind_t status_ind;
                status_ind.connId = param;
                status_ind.status = status;
                app_evt_cb(BLE_IND_SENT, (void *)&status_ind);
            }
            break;
        case SONATA_GAP_CMP_RESET:
            // APP_TRC("APP_COMPLETE: %s  SONATA_GAP_CMP_RESET, seq:%d \r\n",__FUNCTION__,(uint16_t)dwparam);
            ble_reset_cmp();
            break;
        default:
            break;
    }
    APP_TRC("ble_adv_state3: %d    set state %d\r\n", ble_adv_state, ble_adv_set_state);
    return cb_result;
}

static void app_ble_gatt_add_srv_rsp_hand(uint16_t handle)
{
    APP_TRC("app_ble_gatt_add_srv_rsp_hand\r\n");
    uint8_t reg_nb = service_reg_env.reg_nb;
    service_reg_env.reg_list[service_reg_env.reg_nb]->state = SONATA_SERVICE_ENABLE;
    service_reg_env.reg_list[service_reg_env.reg_nb]->start_hdl = handle;
    service_reg_env.reg_nb++;
    // print_serv_env();
    if (service_reg_env.add_nb != service_reg_env.reg_nb) {
        APP_TRC("add new service\r\n");
        uint8_t perm = service_reg_env.reg_list[service_reg_env.reg_nb]->perm;
        uint8_t *uuid = service_reg_env.reg_list[service_reg_env.reg_nb]->uuid;
        uint8_t nb_att = service_reg_env.reg_list[service_reg_env.reg_nb]->nb_att;
        sonata_gatt_att_desc_t *atts = service_reg_env.reg_list[service_reg_env.reg_nb]->att_desc;
        sonata_ble_gatt_add_service_request(service_reg_env.reg_list[service_reg_env.reg_nb]->start_hdl, perm, uuid, nb_att,
                                            &atts[1]);
    }

    if (app_evt_cb != NULL) {
        app_reg_service_cmp_t status_ind;
        status_ind.len  = SONATA_PERM_GET(service_reg_env.reg_list[reg_nb]->perm, SVC_UUID_LEN);
        memmove(status_ind.uuid, service_reg_env.reg_list[reg_nb]->uuid, SONATA_ATT_UUID_128_LEN);
        status_ind.status = 0;
        status_ind.handler = service_reg_env.reg_nb << 16;
        app_evt_cb(BLE_SERVICE_ADD_CMP, (void *)&status_ind);
    }
}

static uint16_t app_ble_rsp_event_handler(uint16_t opt_id, uint8_t status, uint16_t handle, uint16_t perm,
        uint16_t ext_perm, uint16_t length, void *param)
{
    APP_TRC("APP_RESPONSE: %s  opt_id=%04X, %d %d %d \r\n", __FUNCTION__, opt_id, perm, ext_perm, length);
    if (status != 0) {
        APP_TRC("APP_RESPONSE: %s  ERROR=%04X,%x\r\n", __FUNCTION__, status, param);
    }
    switch (opt_id) {
        case SONATA_GATT_ADD_SVC_RSP: {
            APP_TRC("APP_RESPONSE: %s  handle=%04X,\r\n", __FUNCTION__, handle);
            // Should save the start handle id for future use
            app_ble_gatt_add_srv_rsp_hand(handle);
            break;
        }
        default:
            break;
    }

    return CB_DONE;
}

/*!
 * @brief get devcie informations
 * @param info_type @see sonata_gap_local_dev_info
 * @param info
 */
static uint16_t app_get_dev_info_callback(sonata_gap_local_dev_info info_type, void *info)
{
    switch (info_type) {
        case SONATA_GET_DEV_VERSION: {
#if APP_DBG
            sonata_gap_dev_version_ind_t *dev_info = (sonata_gap_dev_version_ind_t *) info;
            APP_TRC("APP_CB: %s, hci_ver =0x%02X\r\n", __FUNCTION__, dev_info->hci_ver);
            APP_TRC("APP_CB: %s, lmp_ver =0x%02X\r\n", __FUNCTION__, dev_info->lmp_ver);
            APP_TRC("APP_CB: %s, host_ver =0x%02X\r\n", __FUNCTION__, dev_info->host_ver);
            APP_TRC("APP_CB: %s, hci_subver =0x%04X\r\n", __FUNCTION__, dev_info->hci_subver);
            APP_TRC("APP_CB: %s, lmp_subver =0x%04X\r\n", __FUNCTION__, dev_info->lmp_subver);
            APP_TRC("APP_CB: %s, host_subver =0x%04X\r\n", __FUNCTION__, dev_info->host_subver);
            APP_TRC("APP_CB: %s, manuf_name =0x%04X\r\n", __FUNCTION__, dev_info->manuf_name);
#endif // SONATA_API_TASK_DBG
        }
            break;
        case SONATA_GET_DEV_BDADDR: {
#if APP_DBG
            sonata_gap_dev_bdaddr_ind_t *param = (sonata_gap_dev_bdaddr_ind_t *) info;
            APP_TRC("APP_CB: %s, SONATA_GET_DEV_BDADDR:", __FUNCTION__);
            for (int i = 0; i < SONATA_GAP_BD_ADDR_LEN; ++i) {
                APP_TRC("%02X ", param->addr.addr.addr[i]);
            }
            APP_TRC("\r\n");
#endif // SONATA_API_TASK_DBG
        }
            break;

        case SONATA_GET_DEV_ADV_TX_POWER: {
#if APP_DBG
            sonata_gap_dev_adv_tx_power_ind_t *param = (sonata_gap_dev_adv_tx_power_ind_t *) info;
            APP_TRC("APP_CB: %s, SONATA_GET_DEV_ADV_TX_POWER power_lvl =0x%02X\r\n", __FUNCTION__, param->power_lvl);
#endif // SONATA_API_TASK_DBG
        }
            break;
        case SONATA_GET_WLIST_SIZE: {
#if APP_DBG
            sonata_gap_list_size_ind_t *param = (sonata_gap_list_size_ind_t *) info;
            APP_TRC("APP_CB: %s, SONATA_GET_WLIST_SIZE size =0x%02X\r\n", __FUNCTION__, param->size);
#endif // SONATA_API_TASK_DBG

            break;
        }
        case SONATA_GET_ANTENNA_INFO: {
#if APP_DBG
            sonata_gap_antenna_inf_ind_t *param = (sonata_gap_antenna_inf_ind_t *) info;
            APP_TRC(">>> SONATA_GET_ANTENNA_INFO supp_switching_sampl_rates =0x%02X, antennae_num =0x%02X, max_switching_pattern_len =0x%02X, max_cte_len =0x%02X\r\n",
                    param->supp_switching_sampl_rates, param->antennae_num, param->max_switching_pattern_len, param->max_cte_len);
#endif // SONATA_API_TASK_DBG
        }
            break;

        case SONATA_GET_SUGGESTED_DFLT_LE_DATA_LEN: {
#if APP_DBG
            sonata_gap_sugg_dflt_data_len_ind_t *param = (sonata_gap_sugg_dflt_data_len_ind_t *) info;
            APP_TRC(">>> SONATA_GET_SUGGESTED_DFLT_LE_DATA_LEN suggted_max_tx_octets =0x%02X, suggted_max_tx_time =0x%02X\r\n",
                    param->suggted_max_tx_octets, param->suggted_max_tx_time);
#endif // SONATA_API_TASK_DBG
            break;
        }
        case SONATA_GET_MAX_LE_DATA_LEN: {
#if APP_DBG
            sonata_gap_max_data_len_ind_t *param = (sonata_gap_max_data_len_ind_t *) info;
            APP_TRC(">>> SONATA_GET_MAX_LE_DATA_LEN suppted_max_tx_octets =0x%04X, suppted_max_tx_time =0x%04X, suppted_max_rx_octets =0x%04X, suppted_max_rx_time =0x%04X\r\n",
                    param->suppted_max_tx_octets, param->suppted_max_tx_time, param->suppted_max_rx_octets, param->suppted_max_rx_time);
#endif // SONATA_API_TASK_DBG
            break;
        }
        case SONATA_GET_PAL_SIZE: {
#if APP_DBG
            sonata_gap_list_size_ind_t *param = (sonata_gap_list_size_ind_t *) info;
            APP_TRC("APP_CB: %s, SONATA_GET_PAL_SIZE size =0x%02X\r\n", __FUNCTION__, param->size);
#endif // SONATA_API_TASK_DBG
            break;
        }
        case SONATA_GET_RAL_SIZE: {
#if APP_DBG
            sonata_gap_list_size_ind_t *param = (sonata_gap_list_size_ind_t *) info;
            APP_TRC("APP_CB: %s, SONATA_GET_RAL_SIZE size =0x%02X\r\n", __FUNCTION__, param->size);
#endif // SONATA_API_TASK_DBG
            break;
        }
        case SONATA_GET_NB_ADV_SETS: {
#if APP_DBG
            sonata_gap_nb_adv_sets_ind_t *param = (sonata_gap_nb_adv_sets_ind_t *) info;
            APP_TRC("APP_CB: %s, SONATA_GET_NB_ADV_SETS nb_adv_sets =0x%02X\r\n", __FUNCTION__, param->nb_adv_sets);
#endif // SONATA_API_TASK_DBG

            break;
        }
        case SONATA_GET_MAX_LE_ADV_DATA_LEN: {
#if APP_DBG
            sonata_gap_max_adv_data_len_ind_t *param = (sonata_gap_max_adv_data_len_ind_t *) info;
            APP_TRC(">>> SONATA_GET_MAX_LE_ADV_DATA_LEN param->length=0x%02X\r\n", param->length);
#endif // SONATA_API_TASK_DBG
            break;
        }
        case SONATA_GET_DEV_TX_PWR: {
#if APP_DBG
            sonata_gap_dev_tx_pwr_ind_t *param = (sonata_gap_dev_tx_pwr_ind_t *) info;
            APP_TRC(">>> SONATA_GET_DEV_TX_PWR min_tx_pwr =0x%04X, max_tx_pwr =0x%04X\r\n",
                    param->min_tx_pwr, param->max_tx_pwr);
#endif // SONATA_API_TASK_DBG
            break;
        }
        case SONATA_GET_DEV_RF_PATH_COMP: {
#if APP_DBG
            sonata_gap_dev_rf_path_comp_ind_t *param = (sonata_gap_dev_rf_path_comp_ind_t *) info;
            APP_TRC(">>> SONATA_GET_DEV_RF_PATH_COMP tx_path_comp =0x%04X, rx_path_comp =0x%04X\r\n",
                    param->tx_path_comp, param->rx_path_comp);
#endif // SONATA_API_TASK_DBG
            break;
        }
        default:
            APP_TRC("APP_CB: %s  No progress for info_type=%02X\r\n", __FUNCTION__, info_type);
            break;

    }
    return CB_DONE;
}

/*!
 * @brief Deal with peer device get local information request.
 * @param opt @see asr_gap_dev_info
 */
static uint16_t app_gap_peer_get_local_info_callback(uint8_t conidx, sonata_gap_dev_info opt)
{
    APP_TRC("APP_CB: %s  conidx=%02X\r\n", __FUNCTION__, conidx);
    switch (opt) {
        case SONATA_GAP_DEV_NAME: {
            sonata_ble_gap_send_get_dev_info_cfm_for_dev_name(conidx, sizeof(local_dev_name), local_dev_name);
        }
        break;

        case SONATA_GAP_DEV_APPEARANCE: {
            uint16_t appearance = 0;
            sonata_ble_gap_send_get_dev_info_cfm_for_dev_appearance(conidx, appearance);

        }
        break;

        case SONATA_GAP_DEV_SLV_PREF_PARAMS: {
            sonata_ble_gap_send_get_dev_info_cfm_for_slv_pref_params(conidx, 8, 10, 0, 200);

        }
        break;

        default:
            break;
    }

    return CB_DONE;
}

void app_gap_set_scan_cb(app_ble_scan_callback_t cb)
{
    p_scan_cb = cb;
}

/*!
 * @brief GAP scan result callback
 * @param result
 */
static uint16_t app_gap_scan_result_callback(sonata_gap_ext_adv_report_ind_t *result)
{
    if (p_scan_cb != NULL) {

        for (int i = 0; i < result->length; ++i) {
            APP_TRC("%02X ", result->data[i]);
        }
        p_scan_cb(result->data, result->length);
        return CB_DONE;
    }
    APP_TRC("APP_CB: %s  ", __FUNCTION__);
    APP_TRC("target_addr:");
    for (int i = 0; i < SONATA_GAP_BD_ADDR_LEN; ++i) {
        APP_TRC("%02X ", result->target_addr.addr.addr[i]);
    }
    APP_TRC("  trans_addr:");
    for (int i = 0; i < SONATA_GAP_BD_ADDR_LEN; ++i) {
        APP_TRC("%02X ", result->trans_addr.addr.addr[i]);
    }
    APP_TRC(" \r\n");
    return CB_DONE;
}
void app_active_delete(uint8_t conIdx)
{
    if (conIdx > APP_ACTIVE_MAX) {
        APP_TRC("APP: %s,ERROR:ACTIVE overflow\r\n", __FUNCTION__);
        return;
    }
    gAppEnv.act[conIdx].runing = false;

}

/*!
 * @brief
 * @param conidx
 * @param conhdl
 * @param reason
 * @return
 */
static uint16_t app_gap_disconnect_ind_callback(uint8_t conidx, uint16_t conhdl, uint8_t reason)
{
    APP_TRC("APP_CB: %s  conidx=0x%02X %d reason = %02x\r\n", __FUNCTION__, conidx, conhdl, reason);
    app_connected_state = APP_STATE_DISCONNECTED;
    if (ble_cb_fun != NULL) {
        ble_cb_fun(MS_BLE_STACK_EVENT_DISCONNECT);
    }
    if (app_evt_cb != NULL) {
        app_connect_status_ind_t status_ind;
        status_ind.connId = conidx;
        uint8_t *bd_addr = app_get_addr_by_conidx(conidx);
        if (NULL != bd_addr) {
            memmove(status_ind.addr, bd_addr, SONATA_GAP_BD_ADDR_LEN);
        }
        app_evt_cb(BLE_DEV_DISCONNECTED, (void *)&status_ind);
    }
    app_connect_req_list_del(conidx);
    app_active_delete(conidx);
    // test_adv_h();
    return CB_DONE;
}

void app_ble_gatt_add_srv_rsp(uint16_t handle)
{
    APP_TRC("app_ble_gatt_add_srv_rsp\r\n");
    APP_TRC("nb_att %d\r\n", service_reg_env.reg_list[service_reg_env.reg_nb]->nb_att);

    service_reg_env.reg_list[service_reg_env.reg_nb]->state = SONATA_SERVICE_ENABLE;
    service_reg_env.reg_list[service_reg_env.reg_nb]->start_hdl = handle;
    if
    (NULL != service_reg_env.reg_list[service_reg_env.reg_nb]->att_desc) {
        sonata_api_free(service_reg_env.reg_list[service_reg_env.reg_nb]->att_desc);
    }
    service_reg_env.reg_nb++;
    if (service_reg_env.add_nb != service_reg_env.reg_nb) {
        APP_TRC("add new service\r\n");
        uint8_t perm = service_reg_env.reg_list[service_reg_env.reg_nb]->perm;
        uint8_t *uuid = service_reg_env.reg_list[service_reg_env.reg_nb]->uuid;
        uint8_t nb_att = service_reg_env.reg_list[service_reg_env.reg_nb]->nb_att;
        sonata_gatt_att_desc_t *atts = service_reg_env.reg_list[service_reg_env.reg_nb]->att_desc;
        sonata_ble_gatt_add_service_request(service_reg_env.reg_list[service_reg_env.reg_nb]->start_hdl, perm, uuid, nb_att,
                                            &atts[1]);
    }
}

ble_gatt_att_reg_list_t *app_ble_get_reg_list_by_handle(uint16_t handle)
{
    // print_serv_env();
    for (int i = 0; i < service_reg_env.reg_nb; i++) {
        ble_gatt_att_reg_list_t *p_list = service_reg_env.reg_list[i];
        printf("nb_att:%d\r\n", p_list->nb_att);
        if (p_list->start_hdl <= handle && (p_list->start_hdl + p_list->nb_att) >= handle) {
            return p_list;
        }
    }
    // printf("[API]get null handle\r\n");
    return NULL;
}

void app_ble_gatt_read_request_handler(uint16_t handle, uint16_t *p_length, uint8_t *p_value)
{
    ble_gatt_att_reg_list_t *p_list = app_ble_get_reg_list_by_handle(handle);
    printf("[API]ble_gatt_read_request_handler %d\r\n", handle);
    if (NULL == p_list) {
        return;
    }
    uint16_t localhandle = handle - p_list->start_hdl - 1;
    if (p_list->att_opr_list[localhandle].read_request != NULL) {
        p_list->att_opr_list[localhandle].read_request(p_value, p_length);
    }
}

void app_user_config_write_cb(uint16_t handle, uint8_t *data, uint16_t size)
{
    static uint16_t s_indicateEnable = 0;
    // printf("user_config_write_cb\r\n");
    if (size != sizeof(s_indicateEnable)) { // Size check
        return ;
    }
    s_indicateEnable = *(uint16_t *)data;
    if (s_indicateEnable == 0x0002) {
        int32_t send_status = sonata_ble_gatt_send_indicate_event(0, handle, size, data); // 0x0200
        printf("send status %ld\r\n", send_status);
    }
}

void app_ble_gatt_write_request_handler(uint16_t handle, uint16_t length, uint8_t *p_value)
{
    // printf("[API]ble_gatt_write_request_handler %d\r\n",handle);

    ble_gatt_att_reg_list_t *p_list = app_ble_get_reg_list_by_handle(handle);
    if (NULL == p_list) {
        return;
    }
    uint16_t localhandle = handle - p_list->start_hdl - 1;
    if (p_list->att_opr_list[localhandle].write_request != NULL) {
        p_list->att_opr_list[localhandle].write_request(p_value, length);
    }

    else {
        app_user_config_write_cb(handle, p_value, length);
    }
}

void app_ble_gatt_get_att_info(uint16_t handle, uint16_t *p_length, uint8_t *p_status)
{
    printf("[API]ble_gatt_get_att_info %d\r\n", handle);

    ble_gatt_att_reg_list_t *p_list = app_ble_get_reg_list_by_handle(handle);
    if (NULL == p_list) {
        *p_length = 0;
        *p_status  = BLE_STATUS_FAILED;
        return;
    }
    *p_length = 200;
    *p_status  = BLE_STATUS_SUCCESS;
}

static uint16_t app_gatt_disc_svc_callback(uint8_t connection_id, uint16_t start_hdl, uint16_t end_hdl,
        uint8_t uuid_len, uint8_t *uuid)
{
    uint16_t service_uuid = 0;
    // APP_TRC("APP_CB: %s, start_hdl=0x%04X, end_hdl =0x%04X, uuid=", __FUNCTION__, start_hdl, end_hdl);

    for (int i = 0; i < uuid_len; ++i) {
        // APP_TRC("%02X", uuid[i]);
        if (i == 0) {
            service_uuid = uuid[i];
        }
        if (i == 1) {
            service_uuid = (uuid[i] << 8) | service_uuid;
        }
    }
    APP_TRC("\r\n");
    APP_TRC("APP_CB: %s,service_uuid = %04X, start=%02d, end=%02d,targetUUID= %04X\r\n", __FUNCTION__, service_uuid,
            start_hdl, end_hdl, gAppEnv.appUuids.service);

    if (service_uuid == gAppEnv.appUuids.service) {
        sonata_ble_gatt_disc_all_characteristic(connection_id, start_hdl, end_hdl);
    }

    return CB_DONE;
}

static uint16_t app_gatt_disc_char_callback(uint8_t conidx, uint16_t attr_hdl, uint16_t pointer_hdl, uint8_t prop,
        uint8_t uuid_len, uint8_t *uuid)
{
    uint16_t char_uuid = 0;
    // APP_TRC("APP_CB: %s, attr_hdl=0x%04X, uuid=", __FUNCTION__, attr_hdl);
    for (int i = 0; i < uuid_len; ++i) {
        // APP_TRC("%02X", uuid[i]);
        if (i == 0) {
            char_uuid = uuid[i];
        }
        if (i == 1) {
            char_uuid = uuid[i] << 8 | char_uuid;
        }
    }

    APP_TRC("APP_CB: %s,char_uuid = %04X,attr_hdl=%04X,prop=%02X, target Read:%04X, Write=%04X\r\n", __FUNCTION__,
            char_uuid, attr_hdl, prop,
            gAppEnv.appUuids.read, gAppEnv.appUuids.write);

    if (char_uuid == gAppEnv.appUuids.read) {
        gAppEnv.attrHandle = attr_hdl;
        gAppEnv.targetReadHandle = attr_hdl + 1;
        APP_TRC("APP_CB: %s, ***Find targetReadHandle =%d, UUID=%04X\r\n", __FUNCTION__, gAppEnv.targetReadHandle, char_uuid);
    }
    if (char_uuid == gAppEnv.appUuids.write) {
        gAppEnv.targetWriteHandle = attr_hdl + 1;
        APP_TRC("APP_CB: %s, ***Find targetWriteHandle=%d, UUID=%04X\r\n", __FUNCTION__, gAppEnv.targetWriteHandle, char_uuid);
    }
    if (char_uuid == gAppEnv.appUuids.ntf) {
        gAppEnv.targetNtfHandle = attr_hdl + 2;
        APP_TRC("APP_CB: %s, ***Find targetNtfHandle  =%d, UUID=%04X\r\n", __FUNCTION__, gAppEnv.targetNtfHandle, char_uuid);
    }

    return CB_DONE;
}

static uint16_t app_gatt_disc_desc_callback(uint8_t conidx, uint16_t attr_hdl, uint8_t uuid_len, uint8_t *uuid)
{
    uint16_t service_uuid = 0;

    // APP_TRC("APP_CB: %s, attr_hdl=0x%04X, uuid=", __FUNCTION__, attr_hdl);
    for (int i = 0; i < uuid_len; ++i) {
        // APP_TRC("%02X", uuid[i]);
        if (i == 0) {
            service_uuid = uuid[i];
        }
        if (i == 1) {
            service_uuid = (uuid[i] << 8) | service_uuid;
        }
    }

    if (service_uuid == SONATA_ATT_DESC_CLIENT_CHAR_CFG && attr_hdl > gAppEnv.attrHandle && gAppEnv.targetNtfHandle == 0) {
        gAppEnv.targetNtfHandle = attr_hdl;
        APP_TRC("\r\n");
        APP_TRC("APP_CB: %s,service_uuid = %04X, Find targetNtfHandle = %d\r\n", __FUNCTION__, service_uuid,
                gAppEnv.targetNtfHandle);
    }
    return CB_DONE;
}

static uint16_t app_gatt_event_callback(uint8_t conidx, uint16_t handle, uint16_t type, uint16_t length, uint8_t *value)
{
    APP_TRC("APP_CB: %s,handle = %04X, type = %04X,length = %04X", __FUNCTION__,
            handle, type, length);

    APP_TRC("APP_CB: %s, Master get data form Slave. Data:", __FUNCTION__);
    for (int i = 0; i < length; ++i) {
        APP_TRC("%02X[%c] ", value[i], value[i]);
    }
    APP_TRC("\r\n");

    return CB_DONE;
}

void app_ble_gatt_data_send(uint16_t local_handle, uint16_t idx, uint16_t length, uint8_t *p_value)
{
    if (local_handle > service_reg_env.add_nb) {
        return;
    }
    ble_gatt_att_reg_list_t *p_list = service_reg_env.reg_list[local_handle];
    if (NULL == p_list) {
        return;
    }
    // idx = 5;
    uint16_t localhandle = p_list->start_hdl + idx;

    APP_TRC("localhandle %ld\r\n", localhandle);
    int32_t send_status = sonata_ble_gatt_send_indicate_event(0, localhandle, length, p_value);
    APP_TRC("send status %ld\r\n", send_status);
    return;
}

void app_ble_gatt_data_send_notify(uint16_t local_handle, uint16_t idx, uint16_t length, uint8_t *p_value)
{
    if (local_handle > service_reg_env.add_nb) {
        return;
    }
    ble_gatt_att_reg_list_t *p_list = service_reg_env.reg_list[local_handle];
    if (NULL == p_list) {
        return;
    }
    // idx = 5;
    uint16_t localhandle = p_list->start_hdl + idx;

    int32_t send_status = sonata_ble_gatt_send_notify_event(0, localhandle, length, p_value);
    APP_TRC("send status %ld\r\n", send_status);
    return;
}

static void app_ble_enable_service(uint16_t start_hdl, uint8_t perm)
{
    APP_TRC("app_ble_enable_service,handle =%02X(X) %d", start_hdl, perm);
    sonata_ble_gatt_set_service_visibility(start_hdl, true);
}

static void app_ble_disable_service(uint16_t start_hdl, uint8_t perm)
{
    APP_TRC("app_ble_disable_service,handle =%02X(X) %d", start_hdl, perm);
    sonata_ble_gatt_set_service_visibility(start_hdl, false);
}

int  app_ble_disable_service_by_handler(uint16_t start_hdl)
{
    if (start_hdl > service_reg_env.add_nb) {
        return -1;
    }
    ble_gatt_att_reg_list_t *p_list = service_reg_env.reg_list[start_hdl];
    if (NULL == p_list) {
        return -1;
    }
    app_ble_disable_service(p_list->start_hdl, p_list->perm);
    return 0;
}

static uint8_t app_ble_search_svc(uint8_t *service_uuid)
{
    for (int idx = 0; idx < service_reg_env.add_nb; idx++) {
        if (!memcmp(service_uuid, service_reg_env.reg_list[idx]->uuid, SONATA_ATT_UUID_128_LEN)) {
            return idx;
        }
    }
    return MAX_SERVICE_NUM;

}

int app_ble_gatt_add_svc_helper(uint16_t *start_hdl, uint8_t nb_att, ble_gatt_att_reg_t *atts)
{
    // printf("ble_gatt_add_svc_helper\r\n");

    uint8_t perm = atts[0].att_desc.perm;
    // PERM_SET(perm, SVC_UUID_LEN,2);
    uint8_t uuid[SONATA_ATT_UUID_128_LEN];
    uint16_t svc_hdl = *start_hdl;
    uint8_t  local_idx;
    APP_TRC("nb_att %d\r\n", nb_att);
    memmove(uuid, atts[0].att_desc.uuid, SONATA_ATT_UUID_128_LEN); // the first should  servicce attr!!!
    local_idx = app_ble_search_svc(uuid);
    if (MAX_SERVICE_NUM != local_idx) {
        APP_TRC("found  service %d\r\n", local_idx);
        if (service_reg_env.reg_list[local_idx]->state == SONATA_SERVICE_DISABLE) {
            app_ble_enable_service(service_reg_env.reg_list[local_idx]->start_hdl, service_reg_env.reg_list[local_idx]->perm);
            return BLE_STATUS_SUCCESS;
        }

        else if (service_reg_env.reg_list[local_idx]->state == SONATA_SERVICE_ENABLE) {
            APP_TRC("duplicate add service %d\r\n", local_idx);
            return BLE_STATUS_FAILED;
        }
    }
    APP_TRC("struc %d %d\r\n", sizeof(ble_gatt_att_reg_list_t), sizeof(ble_gatt_att_manager_t));
    ble_gatt_att_reg_list_t *p_list = (ble_gatt_att_reg_list_t *) sonata_api_malloc(sizeof(ble_gatt_att_opr_t) *
                                      (nb_att - 1) + sizeof(ble_gatt_att_reg_list_t));
    memset(p_list, 0, sizeof(ble_gatt_att_opr_t) * (nb_att - 1) + sizeof(ble_gatt_att_reg_list_t));
    p_list->att_opr_list  = (ble_gatt_att_opr_t *)((uint8_t *)p_list + sizeof(ble_gatt_att_reg_list_t));
    p_list->nb_att = nb_att - 1;
    APP_TRC("nb_att %d\r\n", p_list->nb_att);

    for (int i = 1; i < nb_att; i++) {
        memmove(p_list->att_opr_list + i - 1, &atts[i].att_opr, sizeof(ble_gatt_att_opr_t));
    };
    memmove(p_list->uuid, atts[0].att_desc.uuid, SONATA_ATT_UUID_128_LEN);
    p_list->perm = perm;
    sonata_gatt_att_desc_t *att_desc = sonata_api_malloc(nb_att * sizeof(sonata_gatt_att_desc_t));
    for (int i = 0; i < nb_att; ++i) {

        att_desc[i].perm = atts[i].att_desc.perm;
        att_desc[i].max_len = atts[i].att_desc.max_len;
        att_desc[i].ext_perm = atts[i].att_desc.ext_perm;
        // PERM_SET(msg->svc_desc.atts[i].ext_perm, UUID_LEN,2);
        memcpy(att_desc[i].uuid, atts[i].att_desc.uuid, SONATA_ATT_UUID_128_LEN);
    }
    APP_TRC("nb_att %d\r\n", p_list->nb_att);

#if (defined ALIOS_SUPPORT) || (defined HARMONYOS_SUPPORT)
    lega_rtos_declare_critical();
    lega_rtos_enter_critical();
#endif
    uint8_t add_nb = service_reg_env.add_nb;
    service_reg_env.add_nb ++;
    service_reg_env.reg_list[add_nb] = p_list;
    *start_hdl = add_nb;
    if (service_reg_env.add_nb != service_reg_env.reg_nb + 1) {
        service_reg_env.reg_list[add_nb]->att_desc = att_desc;
        APP_TRC("service adding %d %d\r\n", service_reg_env.add_nb, service_reg_env.reg_nb);
#if (defined ALIOS_SUPPORT) || (defined HARMONYOS_SUPPORT)
        lega_rtos_exit_critical();
#endif
        return BLE_STATUS_FAILED;
    }
#if (defined ALIOS_SUPPORT) || (defined HARMONYOS_SUPPORT)
    lega_rtos_exit_critical();
#endif
    // print_serv_env();

    sonata_ble_gatt_add_service_request(svc_hdl, perm, uuid, nb_att - 1, &att_desc[1]);
    sonata_api_free(att_desc);
    return BLE_STATUS_SUCCESS;
}

int app_set_security_io_cap(uint8_t cap)
{
    APP_TRC("app_set_security_io_cap,cap=%02X(X)", cap);
    app_iocap =  cap;
    return 0;
}

int app_set_security_auth_req(uint8_t auth_req)
{
    APP_TRC("app_set_security_auth_req,auth_req=%02X(X)", auth_req);
    app_auth =  auth_req;
    return 0;
}

uint8_t app_get_connection_state(void)
{
    return app_connection_state;
}

void app_set_connection_state(uint8_t state)
{
    app_connection_state = state;
}

static void print_peer_bond_request(struct sonata_gap_bond_req_ind *request)
{
    switch (request->request) {
        case SONATA_GAP_PAIRING_REQ:
            APP_TRC("PEER_PAIR: SONATA_GAP_PAIRING_REQ,auth_req=%02X(X)", request->data.auth_req);
            switch (request->data.auth_req) {
                case SONATA_GAP_AUTH_REQ_NO_MITM_NO_BOND:
                    APP_TRC(" (GAP_AUTH_REQ_NO_MITM_NO_BOND)\r\n");
                    break;
                case SONATA_GAP_AUTH_REQ_NO_MITM_BOND:
                    APP_TRC(" (GAP_AUTH_REQ_NO_MITM_BOND)\r\n");
                    break;
                case SONATA_GAP_AUTH_REQ_MITM_NO_BOND:
                    APP_TRC(" (GAP_AUTH_REQ_MITM_NO_BOND)\r\n");
                    break;
                case SONATA_GAP_AUTH_REQ_MITM_BOND:
                    APP_TRC(" (GAP_AUTH_REQ_MITM_BOND)\r\n");
                    break;
                case SONATA_GAP_AUTH_REQ_SEC_CON_NO_BOND:
                    APP_TRC(" (GAP_AUTH_REQ_SEC_CON_NO_BOND)\r\n");
                    break;
                case SONATA_GAP_AUTH_REQ_SEC_CON_BOND:
                    APP_TRC(" (GAP_AUTH_REQ_SEC_CON_BOND)\r\n");
                    break;
                default:
                    APP_TRC(" (Default)\r\n");
                    break;

            }
            break;
        case SONATA_GAP_TK_EXCH:
            APP_TRC("PEER_PAIR: SONATA_GAP_TK_EXCH,tk_type=%02X(X)\r\n", request->data.tk_type);
            switch (request->data.auth_req) {
                case SONATA_GAP_TK_OOB:
                    APP_TRC(" (GAP_TK_OOB)\r\n");
                    break;
                case SONATA_GAP_TK_DISPLAY:
                    APP_TRC(" (GAP_TK_DISPLAY)\r\n");
                    break;
                case SONATA_GAP_TK_KEY_ENTRY:
                    APP_TRC(" (GAP_TK_KEY_ENTRY)\r\n");
                    break;
                default:
                    APP_TRC(" (Default)\r\n");
                    break;
            }
            break;
        case SONATA_GAP_LTK_EXCH:
            APP_TRC("PEER_PAIR: SONATA_GAP_LTK_EXCH,key_size=%02X(X)\r\n", request->data.key_size);
            break;
        case SONATA_GAP_OOB_EXCH:
            APP_TRC("PEER_PAIR: \r\n");
            break;
        case SONATA_GAP_NC_EXCH:
            APP_TRC("PEER_PAIR: SONATA_GAP_NC_EXCH,NC Value:%02X %02X %02X %02X\r\n", request->data.nc_data.value[0],
                    request->data.nc_data.value[1],
                    request->data.nc_data.value[2], request->data.nc_data.value[3]);
            break;

    }
}

void app_gap_notify_pair_request_rsp(uint8_t *bd_addr, uint8_t accept)
{
    uint8_t key_size      = SONATA_GAP_SMP_MAX_ENC_SIZE_LEN;
    enum sonata_gap_oob_auth          oob        = SONATA_GAP_OOB_AUTH_DATA_NOT_PRESENT;
    enum sonata_gap_kdist        ikey_dist  = SONATA_GAP_KDIST_LINKKEY; // Initiator key distribution
    enum sonata_gap_kdist        rkey_dist  = SONATA_GAP_KDIST_NONE;  // Responder key distribution
    enum sonata_gap_sec_req      sec_req    = SONATA_GAP_NO_SEC;
    // ikey_dist = GAP_KDIST_ENCKEY | GAP_KDIST_IDKEY;
    //  ikey_dist = GAP_KDIST_NONE;
    //  rkey_dist = GAP_KDIST_NONE;
    ikey_dist  = SONATA_GAP_KDIST_ENCKEY | SONATA_GAP_KDIST_IDKEY;  // Initiator key distribution
    rkey_dist  = SONATA_GAP_KDIST_ENCKEY | SONATA_GAP_KDIST_IDKEY;  // Responder key distribution
    sec_req = SONATA_GAP_NO_SEC;
    uint8_t conidx = app_get_conidx_by_addr(bd_addr);
    sonata_ble_gap_send_bond_cfm_for_pairing_req(conidx, NULL, accept,
            app_iocap, oob, app_auth, key_size, ikey_dist, rkey_dist, sec_req);
    if (accept) {
        app_set_connection_state(APP_BONDING);
    } else {
        APP_TRC("APP_CB: %s  accept %d\r\n", __FUNCTION__, accept);
    }
}

static void app_gap_notify_pair_request(void)
{
    if (sec_req_call == NULL) {
        app_gap_notify_pair_request_rsp(target_address, 1);
    } else {
        sec_req_call(target_address);
    }
}

static uint16_t app_gap_bond_req_callback(uint8_t conidx, struct sonata_gap_bond_req_ind *request)
{
    APP_TRC("APP_CB: %s  request->request=%d\r\n", __FUNCTION__, request->request);
    print_peer_bond_request(request);
    switch (request->request) {
        case SONATA_GAP_PAIRING_REQ: {
            APP_TRC("APP_CB: %s  SONATA_GAP_PAIRING_REQ\r\n", __FUNCTION__);
            app_req_auth = request->data.auth_req;
            app_gap_notify_pair_request();
        }
        break;
        case SONATA_GAP_LTK_EXCH: {
            APP_TRC("APP_CB: %s  SONATA_GAP_LTK_EXCH\r\n", __FUNCTION__);
            uint8_t counter = 0;
            struct sonata_gap_ltk data_ltk = {0};
            uint8_t accept = 1;
            // uint8_t request = SONATA_GAP_LTK_EXCH;

            // Generate all the values
            data_ltk.ediv = (uint16_t)util_rand_word();
            for (counter = 0; counter < SONATA_GAP_RAND_NB_LEN; counter++) {
                data_ltk.ltk.key[counter]   = (uint8_t)util_rand_word();
                data_ltk.randnb.nb[counter] = (uint8_t)util_rand_word();
            }
            for (counter = SONATA_GAP_RAND_NB_LEN; counter < SONATA_GAP_KEY_LEN; counter++) {
                data_ltk.ltk.key[counter]   = (uint8_t)util_rand_word();
            }
            APP_TRC("APP_CB  %s, app_ltk_key  :", __FUNCTION__);
            for (int i = 0; i < SONATA_GAP_KEY_LEN; ++i) {
                APP_TRC("%02X ", data_ltk.ltk.key[i]);
            }
            APP_TRC("\r\n");
            APP_TRC("APP_CB  %s, app_randnb_nb:", __FUNCTION__);
            for (int i = 0; i < SONATA_GAP_RAND_NB_LEN; ++i) {
                APP_TRC("%02X ", data_ltk.randnb.nb[i]);
            }
            APP_TRC("\r\n");
            if (sonata_fs_write(SONATA_TAG_LTK, SONATA_LEN_LTK, &data_ltk) != SONATA_FS_OK) {
                APP_TRC("LTK Save fail!!!\r\n");
            }
            sonata_ble_gap_send_bond_cfm_for_ltk_exchange(conidx,
                    accept,
                    data_ltk.ediv,
                    data_ltk.randnb.nb,
                    SONATA_GAP_KEY_LEN,
                    data_ltk.ltk.key);

            APP_TRC(" bonded_dev_info.current_dev_index = %d \r\n", bonded_dev_info.current_dev_index);
            memcpy(bonded_dev_info.bonded_device_info[bonded_dev_info.current_dev_index].peer_addr, peer_dbaddr, APP_BD_ADDR_LEN);
            memcpy(bonded_dev_info.bonded_device_info[bonded_dev_info.current_dev_index].ltk.ltk, data_ltk.ltk.key,
                   SONATA_GAP_KEY_LEN);
            memcpy(bonded_dev_info.bonded_device_info[bonded_dev_info.current_dev_index].ltk.randnb, data_ltk.randnb.nb, 8);
            bonded_dev_info.bonded_device_info[bonded_dev_info.current_dev_index].ltk.ediv = data_ltk.ediv;

            // Store the bonded dev info value in FS
            if (sonata_fs_write(SONATA_TAG_BONDED_DEV_INFO, SONATA_LEN_BONDED_DEV_INFO,
                                (uint8_t *)&bonded_dev_info) != SONATA_FS_OK) {
                ASSERT_ERR(3695, 0);
            }

        }
        break;
        case SONATA_GAP_IRK_EXCH: {
            APP_TRC("APP_CB: %s  SONATA_GAP_IRK_EXCH \r\n", __FUNCTION__);
            // Store the peer addr into FS
            uint8_t accept = true;

            uint8_t *local_addr = sonata_get_bt_address();
            uint8_t addr_type = (local_addr[5] & 0xC0) ? SONATA_ADDR_RAND : SONATA_ADDR_PUBLIC;
            sonata_ble_gap_send_bond_cfm_for_irk_exchange(conidx, accept, app_loc_irk, addr_type, local_addr);
        }
        break;
        case SONATA_GAP_TK_EXCH: {
            APP_TRC("APP_CB: %s  SONATA_GAP_TK_EXCH\r\n", __FUNCTION__);

            uint8_t data_tk[SONATA_GAP_KEY_LEN] = {0};
            // Generate a PIN Code- (Between 100000 and 999999)
            uint32_t pin_code = (100000 + (util_rand_word() % 900000));
            uint8_t accept  = true;

            data_tk[0] = (uint8_t) ((pin_code & 0x000000FF) >> 0);
            data_tk[1] = (uint8_t) ((pin_code & 0x0000FF00) >> 8);
            data_tk[2] = (uint8_t) ((pin_code & 0x00FF0000) >> 16);
            data_tk[3] = (uint8_t) ((pin_code & 0xFF000000) >> 24);
            sonata_ble_gap_send_bond_cfm_for_tk_exchange(conidx, accept, data_tk);
            APP_TRC("APP_CB: %s  pin_code=%lu,", __FUNCTION__, pin_code);
            APP_TRC(" TKey:");

            for (int i = 0; i < SONATA_GAP_KEY_LEN; ++i) {
                APP_TRC("%02X ", data_tk[i]);
            }
            APP_TRC("\r\n");

        }
        break;
        case SONATA_GAP_NC_EXCH: {
            APP_TRC("APP_CB: %s  SONATA_GAP_NC_EXCH\r\n", __FUNCTION__);
            APP_TRC("APP_CB  %s, NC Value:", __FUNCTION__);
            for (int i = 0; i < 4; ++i) {
                APP_TRC("%02X ", request->data.nc_data.value[i]);
            }
            APP_TRC("\r\n");
            uint8_t accept  = 1;
            sonata_ble_gap_send_bond_cfm_for_nc_exchange(conidx, accept);

        }
        break;
        case SONATA_GAP_CSRK_EXCH:
            APP_TRC("APP_CB: %s  SONATA_GAP_CSRK_EXCH\r\n", __FUNCTION__);
            uint8_t accept  = true;
            sonata_ble_gap_send_bond_cfm_for_csrk_exchange(conidx, accept, app_csrk);

            break;
        case SONATA_GAP_OOB_EXCH:
            APP_TRC("APP_CB: %s  SONATA_GAP_OOB_EXCH\r\n", __FUNCTION__);

            break;
    }

    return CB_DONE;

}

static uint16_t app_gap_bond_callback(uint8_t conidx, struct sonata_gap_bond_ind *ind)
{
    APP_TRC("APP_CB: %s  \r\n", __FUNCTION__);
    switch (ind->info) {
        case SONATA_GAP_PAIRING_SUCCEED:
            APP_TRC("APP_CB: %s  SONATA_GAP_PAIRING_SUCCEED,", __FUNCTION__);
            APP_TRC(" app_bond = 1,");
            app_bond = 1;
            if (sonata_fs_write(SONATA_TAG_PERIPH_BONDED, SONATA_LEN_PERIPH_BONDED, &app_bond) != SONATA_FS_OK) {
                // An error has occurred during access to the FS
                ASSERT_ERR(3697, 0);
            }
            struct sonata_gap_bdaddr *bdaddr = sonata_ble_gap_get_bdaddr(conidx, SONATA_GAP_SMP_INFO_PEER);
            if (sonata_fs_write(SONATA_TAG_PEER_BD_ADDRESS, SONATA_LEN_PEER_BD_ADDRESS, bdaddr->addr.addr) != SONATA_FS_OK) {
                // An error has occurred during access to the FS
                // ASSERT_ERR(3694, 0);
            } else {
                APP_TRC("peer_addr:");
                for (int i = SONATA_GAP_BD_ADDR_LEN - 1; i >= 0; --i) {
                    APP_TRC("%02X ", bdaddr->addr.addr[i]);
                }
                APP_TRC(" \r\n");
            }

            switch (ind->data.auth.info) {
                case SONATA_GAP_AUTH_REQ_NO_MITM_NO_BOND:
                    APP_TRC("Auth:GAP_AUTH_REQ_NO_MITM_NO_BOND \r\n");
                    break;
                case SONATA_GAP_AUTH_REQ_NO_MITM_BOND:
                    APP_TRC("Auth:GAP_AUTH_REQ_NO_MITM_BOND \r\n");
                    break;
                case SONATA_GAP_AUTH_REQ_MITM_NO_BOND:
                    APP_TRC("Auth:GAP_AUTH_REQ_MITM_NO_BOND \r\n");
                    break;
                case SONATA_GAP_AUTH_REQ_MITM_BOND:
                    APP_TRC("Auth:GAP_AUTH_REQ_MITM_BOND \r\n");
                    break;
                case SONATA_GAP_AUTH_REQ_SEC_CON_NO_BOND:
                    APP_TRC("Auth:GAP_AUTH_REQ_SEC_CON_NO_BOND \r\n");
                    break;
                case SONATA_GAP_AUTH_REQ_SEC_CON_BOND:
                    APP_TRC("Auth:GAP_AUTH_REQ_SEC_CON_BOND \r\n");
                    break;
            }
            app_set_connection_state(APP_BONDED);
            if (bonded_dev_info.current_dev_index < MAX_BONDED_DEV_INDEX) {
                bonded_dev_info.current_dev_index++;
            } else if (bonded_dev_info.current_dev_index == MAX_BONDED_DEV_INDEX) {
                bonded_dev_info.current_dev_index = 0;
            }
            break;
        case SONATA_GAP_PAIRING_FAILED:
            // Reason see (SONATA_GAP_SMP_REM_ERR_MASK|smp_pair_fail_reason)
            APP_TRC("APP_CB: %s  SONATA_GAP_PAIRING_FAILED,Reason:%02X(X)\r\n", __FUNCTION__, ind->data.reason);
            // app_ble_config_scanning();
            // app_stop_scan_timer_start();
            // sonata_ble_gap_start_security(conidx, GAP_AUTH_REQ_MITM_BOND);
            sonata_ble_gap_disconnect(conidx, SONATA_CO_ERROR_CONN_REJ_SECURITY_REASONS);
            break;
        case SONATA_GAP_LTK_EXCH:
            APP_TRC("APP_CB: %s  SONATA_GAP_LTK_EXCH\r\n", __FUNCTION__);
            if ((app_req_auth & SONATA_GAP_AUTH_SEC_CON) && (app_auth & SONATA_GAP_AUTH_SEC_CON)) {
                memcpy(bonded_dev_info.bonded_device_info[bonded_dev_info.current_dev_index].ltk.ltk, ind->data.ltk.ltk.key,
                       SONATA_GAP_KEY_LEN);
            }
            memcpy(bonded_dev_info.bonded_device_info[bonded_dev_info.current_dev_index].ltk_in, ind->data.ltk.ltk.key,
                   SONATA_GAP_KEY_LEN);
            // memcpy(bonded_dev_info.bonded_device_info[bonded_dev_info.current_dev_index].ltk.randnb, ind->data.randnb.nb, 8);
            // bonded_dev_info.bonded_device_info[bonded_dev_info.current_dev_index].ltk.ediv = ind->data.ediv;
            // Store the app_ltk_key_in value in FS
            if (sonata_fs_write(SONATA_TAG_BONDED_DEV_INFO, SONATA_LEN_BONDED_DEV_INFO,
                                (uint8_t *)&bonded_dev_info) != SONATA_FS_OK) {
                ASSERT_ERR(3698, 0);
            }
            APP_TRC("AAA  %s, app_ltk_key_in:", __FUNCTION__);
            for (int i = 0; i < SONATA_GAP_KEY_LEN; ++i) {
                APP_TRC("%02X ", ind->data.ltk.ltk.key[i]);
            }
            APP_TRC("\r\n");

            break;
        case SONATA_GAP_IRK_EXCH:
            APP_TRC("APP_CB: %s  SONATA_GAP_IRK_EXCH\r\n", __FUNCTION__);
            memcpy(bonded_dev_info.bonded_device_info[bonded_dev_info.current_dev_index].irk.irk_addr, ind->data.irk.addr.addr.addr,
                   SONATA_GAP_BD_ADDR_LEN);
            memcpy(bonded_dev_info.bonded_device_info[bonded_dev_info.current_dev_index].irk.irk, ind->data.irk.irk.key,
                   SONATA_GAP_KEY_LEN);
            // Store the irk addr value in FS
            if (sonata_fs_write(SONATA_TAG_BONDED_DEV_INFO, SONATA_LEN_BONDED_DEV_INFO,
                                (uint8_t *)&bonded_dev_info) != SONATA_FS_OK) {
                ASSERT_ERR(3700, 0);
            }
            break;
        case SONATA_GAP_CSRK_EXCH:
            APP_TRC("APP_CB: %s  SONATA_GAP_CSRK_EXCH\r\n", __FUNCTION__);
            break;
        case SONATA_GAP_REPEATED_ATTEMPT:
            APP_TRC("APP_CB: %s  SONATA_GAP_REPEATED_ATTEMPT\r\n", __FUNCTION__);
            sonata_ble_gap_disconnect(conidx, SONATA_HL_LL_ERR_REMOTE_USER_TERM_CON);
            break;
    }
    return CB_DONE;
}

uint8_t app_check_device_isbonded(uint16_t in_ediv, uint8_t *in_nb)
{
    APP_TRC("APP_CB: %s \r\n", __FUNCTION__);
    // check the latest device first
    for (int i = bonded_dev_info.current_dev_index; i < MAX_BONDED_DEV_NUM; i++) {
        if (in_ediv == bonded_dev_info.bonded_device_info[i].ltk.ediv &&
            !memcmp(&in_nb[0], bonded_dev_info.bonded_device_info[i].ltk.randnb, SONATA_GAP_RAND_NB_LEN)) {
            APP_TRC("APP_CB: %s 00 i = %d\r\n", __FUNCTION__, i);
            return i;
        }
    }
    for (int i = 0; i < bonded_dev_info.current_dev_index; i++) {
        if (in_ediv == bonded_dev_info.bonded_device_info[i].ltk.ediv &&
            !memcmp(&in_nb[0], bonded_dev_info.bonded_device_info[i].ltk.randnb, SONATA_GAP_RAND_NB_LEN)) {
            APP_TRC("APP_CB: %s 11 i = %d\r\n", __FUNCTION__, i);
            return i;
        }
    }
    return INVALID_BONDED_INDEX;
}

static uint16_t app_gap_encrypt_req_callback(uint8_t conidx, uint16_t in_ediv, uint8_t *in_nb)
{
    APP_TRC("APP_CB: %s in_ediv=%X(x), in_nb:", __FUNCTION__, in_ediv);
    for (int i = 0; i < SONATA_GAP_RAND_NB_LEN; ++i) {
        APP_TRC("%02X ", in_nb[i]);
    }
    APP_TRC("\r\n");
    uint8_t length = SONATA_LEN_PERIPH_BONDED;
    if (sonata_fs_read(SONATA_TAG_PERIPH_BONDED, &length, &app_bond) != SONATA_FS_OK) {
        app_bond = 0;
    }
    if (app_bond) {
        uint8_t found = 0;
        uint16_t keySize = SONATA_GAP_KEY_LEN;
        length = SONATA_LEN_BONDED_DEV_INFO;

        if (sonata_fs_read(SONATA_TAG_BONDED_DEV_INFO, &length, (uint8_t *)&bonded_dev_info) == SONATA_FS_OK) {
            // Check if the provided EDIV and Rand Nb values match with the stored values
            uint8_t index = app_check_device_isbonded(in_ediv, in_nb);
            if (index > INVALID_BONDED_INDEX) {
                APP_TRC("APP_CB: %s, found, send encrypt confirm, key:", __FUNCTION__);
                for (int i = 0; i < SONATA_GAP_KEY_LEN; ++i) {
                    APP_TRC("%02X ", bonded_dev_info.bonded_device_info[index].ltk.ltk[i]);
                }
                APP_TRC("\r\n");
                found = true;
                sonata_ble_gap_send_encrypt_cfm(conidx, found, keySize, bonded_dev_info.bonded_device_info[index].ltk.ltk);
                app_set_connection_state(APP_BONDED);
            } else {
                APP_TRC("APP_CB: %s, not found, send encrypt confirm\r\n", __FUNCTION__);
                uint8_t app_ltk_key_zero[SONATA_GAP_KEY_LEN] = {0};
                sonata_ble_gap_send_encrypt_cfm(conidx, found, keySize, app_ltk_key_zero);
            }
        } else {
            return CB_REJECT;
            APP_TRC("Error when read LTK in FS!!!\r\n");
        }
    } else {
        APP_TRC("APP_CB: %s, not bond, send encrypt confirm\r\n", __FUNCTION__);
        uint8_t app_ltk_key_zero[SONATA_GAP_KEY_LEN] = {0};
        sonata_ble_gap_send_encrypt_cfm(conidx, false, 0, app_ltk_key_zero);
    }

    return CB_DONE;
}

static uint16_t app_gap_gen_random_number_callback(uint8_t *number)
{
    APP_TRC("APP_CB: %s  \r\n", __FUNCTION__);
    if (app_rand_cnt == 1) {    // First part of IRK
        memcpy(&app_loc_irk[0], number, 8);
    } else if (app_rand_cnt == 2) { // Second part of IRK
        memcpy(&app_loc_irk[8], number, 8);
    }
    APP_TRC("app_loc_irk :");
    for (int i = 0; i < KEY_LEN; ++i) {
        APP_TRC("%02X ", app_loc_irk[i]);
    }
    APP_TRC("\r\n");
    return CB_DONE;
}

static uint16_t app_gap_security_callback(uint8_t conidx, uint8_t auth_level)
{
    APP_TRC("APP_CB: %s  auth_level=%02X(x)\r\n", __FUNCTION__, auth_level);

    return CB_DONE;

}

static uint16_t app_gap_encrypt_callback(uint8_t conidx, uint8_t auth_level)
{
    APP_TRC("APP_CB: %s  auth_level=%02X(x)\r\n", __FUNCTION__, auth_level);

    return CB_DONE;
}

static uint16_t app_gap_le_pke_size_callback(uint8_t conidx, uint16_t max_tx_octets, uint16_t max_tx_time,
        uint16_t max_rx_octets, uint16_t max_rx_time)
{
    APP_TRC("APP_CB: %s  %d max_tx_octets=%d(d), max_rx_octets=%d(d) time %d %d\r\n", __FUNCTION__, conidx, max_tx_octets,
            max_rx_octets, max_tx_time, max_rx_time);

    return CB_DONE;
}
static uint16_t app_gap_resolving_address_callback(uint8_t operation, uint8_t addr_type, uint8_t *addr)
{
    APP_TRC("APP_CB: %s  operation=%d(d), addr_type=%d(d)\r\n", __FUNCTION__, operation, addr_type);
    if (NULL == addr) {
        APP_TRC("ERROR\r\n");
    }
    return CB_DONE;
}

static void app_ble_print_peer_info(uint8_t *name, uint16_t nameLen, uint8_t *mac)
{
    printf("\r\n----PEER BLE Connected ----");
    printf("\r\n----PEER BLE NAME[%s] ----\r\n", name);
    printf("----PEER BLE MAC[%02X:%02X:%02X:%02X:%02X:%02X] ----\r\n",
           mac[5], mac[4], mac[3], mac[2], mac[1], mac[0]);
    printf("\r\n");

}

/*!
 * @brief
 * @param info
 */
static uint16_t app_gap_peer_att_info_callback(uint8_t conidx, sonata_gap_peer_att_info_ind_t *info)
{
    APP_TRC("APP_CB: %s  conidx=%02X\r\n", __FUNCTION__, conidx);
    sonata_gap_peer_att_info_ind_t *att_info = (sonata_gap_peer_att_info_ind_t *) info;
    APP_TRC("APP_CB: %s, req =0x%02X\r\n", __FUNCTION__, att_info->req);
    APP_TRC("APP_CB: %s, handle =0x%02X\r\n", __FUNCTION__, att_info->handle);
    switch (att_info->req) {
        case SONATA_GAP_DEV_NAME: {
            APP_TRC("APP  %s, Name:", __FUNCTION__);
            for (int i = 0; i < att_info->info.name.length; ++i) {
                APP_TRC("%c", att_info->info.name.value[i]);
            }
            APP_TRC("\r\n");
            uint8_t len = att_info->info.name.length > 20 ? 20 : att_info->info.name.length;
            memcpy(gAppEnv.act[conidx].name, att_info->info.name.value, len);
            app_ble_print_peer_info(att_info->info.name.value, len, gAppEnv.act[conidx].peer);
        }
        break;
        case SONATA_GAP_DEV_APPEARANCE: {
            APP_TRC("APP_CB: %s, appearance =0x%04X\r\n", __FUNCTION__, att_info->info.appearance);
        }
        break;
        case SONATA_GAP_DEV_SLV_PREF_PARAMS: {
            APP_TRC("APP_CB: %s, con_intv_min =0x%02X\r\n", __FUNCTION__, att_info->info.slv_pref_params.con_intv_min);
            APP_TRC("APP_CB: %s, con_intv_max =0x%02X\r\n", __FUNCTION__, att_info->info.slv_pref_params.con_intv_max);
            APP_TRC("APP_CB: %s, slave_latency =0x%02X\r\n", __FUNCTION__, att_info->info.slv_pref_params.slave_latency);
            APP_TRC("APP_CB: %s, conn_timeout =0x%02X\r\n", __FUNCTION__, att_info->info.slv_pref_params.conn_timeout);
        }
        break;
        case SONATA_GAP_DEV_CTL_ADDR_RESOL: {
            APP_TRC("APP_CB: %s, ctl_addr_resol =0x%02X\r\n", __FUNCTION__, att_info->info.ctl_addr_resol);
        }
        break;

        default:
            break;

    }
    return CB_DONE;
}

static uint16_t app_gap_peer_info_callback(uint8_t conidx, sonata_gap_peer_info_ind_t *info)
{
    APP_TRC("APP_CB: %s  conidx=%02X, info_type=%02X\r\n", __FUNCTION__, conidx, info->req);
    switch (info->req) {
        case SONATA_GET_PEER_VERSION:
            APP_TRC("APP_CB: SONATA_GET_PEER_VERSION, compid:%04X,lmp_subvers:%04X,lmp_vers:%02X,\r\n",
                    info->info.version.compid, info->info.version.lmp_subvers, info->info.version.lmp_vers);
            break;
        case SONATA_GET_PEER_FEATURES:
            APP_TRC("APP_CB: SONATA_GET_PEER_FEATURES, features:");
            for (int i = 0; i < SONATA_GAP_LE_FEATS_LEN; ++i) {
                APP_TRC("%02x ", info->info.features.features[i]);
            }
            APP_TRC("\r\n");
            break;
        case SONATA_GET_PEER_CON_RSSI:
            APP_TRC("APP_CB: SONATA_GET_PEER_CON_RSSI, rssi:%04X\r\n", info->info.rssi.rssi);

            break;
        case SONATA_GET_PEER_CON_CHANNEL_MAP:
            APP_TRC("APP_CB: SONATA_GET_PEER_CON_CHANNEL_MAP, map:");
            for (int i = 0; i < SONATA_GAP_LE_CHNL_MAP_LEN; ++i) {
                APP_TRC("%02x ", info->info.channel_map.ch_map.map[i]);
            }
            APP_TRC("\r\n");
            break;
        case SONATA_GET_LE_PING_TO:
            APP_TRC("APP_CB: SONATA_GET_LE_PING_TO, timeout:%04X,\r\n", info->info.ping_to_value.timeout);
            break;
        case SONATA_GET_PHY:
            APP_TRC("APP_CB: SONATA_GET_PHY, tx_phy:%02X, rx_phy:%02X\r\n", info->info.le_phy.tx_phy, info->info.le_phy.rx_phy);
            break;
        case SONATA_GET_CHAN_SEL_ALGO:
            APP_TRC("APP_CB: SONATA_GET_CHAN_SEL_ALGO, chan_sel_algo:%04X,\r\n", info->info.sel_algo.chan_sel_algo);
            break;
        default:
            break;
    }

    return CB_DONE;
}

/*!
 * @brief
 * @param conidx
 * @param intv_min
 * @param intv_max
 * @param latency
 * @param time_out
 */
static uint16_t app_gap_param_update_req_callback(uint8_t conidx, uint16_t intv_min, uint16_t intv_max,
        uint16_t latency, uint16_t time_out)
{
    APP_TRC("APP_CB: %s  conidx=%02X,intv_min=%04X,intv_max=%04X,latency=%04X,time_out=%04X\r\n", __FUNCTION__, conidx,
            intv_min, intv_max, latency, time_out);
    sonata_ble_gap_send_param_update_cfm(conidx, true, 2, 4);
    return CB_DONE;
}

/*!
 * @brief
 * @param conidx
 * @param con_interval
 * @param con_latency
 * @param sup_to
 * @return
 */
static uint16_t app_gap_param_updated_callback(uint8_t conidx, uint16_t con_interval, uint16_t con_latency,
        uint16_t sup_to)
{
    APP_TRC("APP_CB: %s  conidx=%02X,con_interval=%04X,con_latency=%04X,sup_to=%04X\r\n", __FUNCTION__, conidx,
            con_interval, con_latency, sup_to);

    return CB_DONE;

}

/*!
 * @brief
 * @param conidx
 * @param length
 * @param name
 * @return
 */
static uint16_t app_gap_peer_set_local_device_name_callback(uint8_t conidx, uint16_t length, uint8_t *name)
{
    sonata_ble_gap_send_set_dev_name_cfm(conidx, SONATA_GAP_ERR_REJECTED);
    APP_TRC("name:");
    for (int i = 0; i < length; ++i) {
        APP_TRC("%02X ", name[i]);
    }
    APP_TRC(" \r\n");
    return CB_DONE;
}

void app_set_connect_flag(uint8_t vaule)
{
    need_connect_confirm = vaule;
}

static void app_gap_send_connection_confirm(uint8_t conidx, uint8_t auth)
{
    sonata_gap_connection_cfm_t connectionCfm = {0};
    // Get bond status from FS
    if (app_bond == 1) {
        connectionCfm.auth = SONATA_GAP_AUTH_REQ_NO_MITM_BOND;
        APP_TRC("APP: %s  connectionCfm.auth=GAP_AUTH_REQ_NO_MITM_BOND\r\n", __FUNCTION__);
    } else {
        connectionCfm.auth = SONATA_GAP_AUTH_REQ_NO_MITM_NO_BOND;
        APP_TRC("APP: %s  connectionCfm.auth=GAP_AUTH_REQ_SEC_CON_BOND\r\n", __FUNCTION__);

    }
    memcpy(connectionCfm.lcsrk.key, app_csrk, SONATA_GAP_KEY_LEN);
    connectionCfm.auth = auth;

    sonata_ble_gap_send_connection_cfm(conidx, &connectionCfm);
}

void app_gap_connect_confirm(uint8_t *addr, uint8_t auth)
{
    uint8_t conidx = app_get_conidx_by_addr(addr);
    APP_TRC("APP_CB: %s  ", __FUNCTION__);
    sonata_ble_gap_start_security(conidx, auth);
    APP_TRC("id %d auth %d:", conidx, auth);
}

void app_active_update(uint8_t conIdx, uint8_t *mac)
{
    if (conIdx > APP_ACTIVE_MAX) {
        APP_TRC("APP: %s,ERROR:ACTIVE overflow\r\n", __FUNCTION__);
        return;
    }
    if (gAppEnv.act[conIdx].runing == true) {
        APP_TRC("APP: %s,ERROR:ACTIVE id error\r\n", __FUNCTION__);
        return;
    }
    gAppEnv.act[conIdx].runing = true;
    gAppEnv.act[conIdx].assign_id = conIdx;
    memcpy(gAppEnv.act[conIdx].peer, mac, SONATA_GAP_BD_ADDR_LEN);
}

static uint16_t app_gap_connection_req_callback(uint8_t conidx, sonata_gap_connection_req_ind_t *req)
{
    APP_TRC("APP_CB: %s  ", __FUNCTION__);
    APP_TRC("peer_addr:");
    for (int i = 0; i < SONATA_GAP_BD_ADDR_LEN; ++i) {
        APP_TRC("%02X ", req->peer_addr.addr[i]);
    }
    APP_TRC(" \r\n");
    app_active_update(conidx, req->peer_addr.addr);

    memmove(target_address, req->peer_addr.addr, SONATA_GAP_BD_ADDR_LEN);
    ble_connect_id = conidx;
    app_connect_req_list_add(req->peer_addr.addr, conidx);
    APP_TRC("ble_connect_id %d \r\n", ble_connect_id);
    // sonata_ble_gatt_exchange_mtu(conidx);
    app_connected_state = APP_STATE_CONNECTED;
    if (ble_cb_fun != NULL) {
        ble_cb_fun(MS_BLE_STACK_EVENT_CONNECTED);
    }

    if (app_evt_cb != NULL) {
        app_connect_status_ind_t status_ind;
        status_ind.connId = conidx;
        memmove(status_ind.addr, req->peer_addr.addr, SONATA_GAP_BD_ADDR_LEN);
        app_evt_cb(BLE_DEV_CONNECTED, (void *) &status_ind);
    }

    if (app_bond) {
        sonata_gap_connection_cfm_t connectionCfm = {0};

        memcpy(connectionCfm.lcsrk.key, app_csrk, SONATA_GAP_KEY_LEN);
        connectionCfm.auth = SONATA_GAP_AUTH_REQ_SEC_CON_BOND;

        sonata_ble_gap_send_connection_cfm(conidx, &connectionCfm);
        return CB_DONE; // SDK will send connection confirm message
    } else {
        sonata_gap_connection_cfm_t connectionCfm = {0};
        connectionCfm.auth = SONATA_GAP_AUTH_REQ_NO_MITM_NO_BOND;
        sonata_ble_gap_send_connection_cfm(conidx, &connectionCfm);
        sonata_ble_gap_get_peer_info(conidx, SONATA_GET_PEER_NAME);
        return CB_DONE;
    }
}

/*!
 * @brief
 * @param connection_id
 * @param handle
 * @return
 */
static uint16_t app_gatt_read_request_callback(uint8_t connection_id, uint16_t handle)
{
    // APP_TRC("APP_CB: %s, handle=0x%04X,custom_svc_start_handle=0x%04X", __FUNCTION__,handle,custom_svc_start_handle);
    APP_TRC("APP_CB: %s, handle=0x%04X,\r\n", __FUNCTION__, handle);
    uint16_t length = 250;
    uint8_t *value = sonata_api_malloc(length);
    app_ble_gatt_read_request_handler(handle, &length, value);
    sonata_ble_gatt_send_read_confirm(connection_id, handle, SONATA_GAP_ERR_NO_ERROR, length, value);
    sonata_api_free(value);
    return CB_REJECT;
}

static uint16_t app_gatt_read_callback(uint8_t conidx, uint16_t handle, uint16_t offset, uint16_t length,
                                       uint8_t *value)
{
    // APP_TRC("APP_CB: %s, handle=0x%04X,custom_svc_start_handle=0x%04X", __FUNCTION__,handle,custom_svc_start_handle);
    APP_TRC("APP_CB: %s, handle=0x%04X,\r\n", __FUNCTION__, handle);
    return CB_REJECT;
}

/*!
 * @brief
 * @param connection_id
 * @param handle
 * @param offset
 * @param length
 * @param value
 * @return
 */
static uint16_t app_gatt_write_request_callback(uint8_t connection_id, uint16_t handle, uint16_t offset,
        uint16_t length, uint8_t *value)
{
    // APP_TRC("APP_CB: %s, handle=0x%04X,custom_svc_start_handle=0x%04X", __FUNCTION__,handle,custom_svc_start_handle);
    APP_TRC("APP_CB: %s, handle=0x%04X offset %d\r\n", __FUNCTION__, handle, offset);

    sonata_ble_gatt_send_write_confirm(connection_id, handle, SONATA_GAP_ERR_NO_ERROR);
    app_ble_gatt_write_request_handler(handle, length, value);

    return CB_DONE;
}

/*!
 * @brief
 * @param connection_id
 * @param mtu
 * @return
 */
static uint16_t app_gatt_mtu_changed_callback(uint8_t connection_id, uint16_t mtu)
{
    APP_TRC("APP_CB: %s, mtu=0x%04X\r\n", __FUNCTION__, mtu);
    if (app_evt_cb != NULL) {
        app_mtu_change_ind_t status_ind;
        status_ind.connId = connection_id;
        status_ind.mtu = mtu;
        app_evt_cb(BLE_MTU_CHANGE, (void *)&status_ind);
    }
    return CB_DONE;
}

/*!
 * @brief
 * @param connection_id
 * @param handle
 * @param offset
 * @param length
 * @param value
 * @return
 */
static uint16_t app_gatt_att_info_req_ind_callback(uint8_t connection_id, uint16_t handle)
{
    // APP_TRC("APP_CB: %s, handle=0x%04X,custom_svc_start_handle=0x%04X", __FUNCTION__,handle,custom_svc_start_handle);
    APP_TRC("APP_CB: %s, handle=0x%04X\r\n", __FUNCTION__, handle);
    uint16_t length = 0;
    uint8_t  status;
    length = 200;
    status = SONATA_GAP_ERR_NO_ERROR;
    sonata_ble_gatt_send_att_info_confirm(connection_id, handle, length, status);
    return CB_DONE;
}

bool app_ble_master_write_data(uint8_t conidx, uint16_t length, uint8_t *data)
{
    if (gAppEnv.targetWriteHandle != 0) {
        APP_TRC("APP: %s,targetWriteHandle=%d, dataLen=%d\r\n", __FUNCTION__, gAppEnv.targetWriteHandle, length);
        sonata_ble_gatt_write(conidx, gAppEnv.targetWriteHandle, 0, 0, length, data);
    } else {
        APP_TRC("ERR: %s,targetWriteHandle [%d] is wrong, can't write data\r\n", __FUNCTION__, gAppEnv.targetWriteHandle);

    }
    return false;
}

bool app_ble_master_read_data(uint8_t conidx, uint16_t length, uint8_t *data)
{
    if (gAppEnv.targetReadHandle != 0) {
        sonata_ble_gatt_read_by_handle(conidx, gAppEnv.targetReadHandle);
    } else {
        APP_TRC_ERROR("ERR: %s,targetReadHandle [%d] is wrong, can't read data\r\n", __FUNCTION__, gAppEnv.targetWriteHandle);
    }
    return false;
}

bool app_ble_master_turn_ntf(uint8_t conidx, bool on)
{
    if (gAppEnv.targetNtfHandle != 0) {
        APP_TRC("APP: %s,targetNtfHandle=%d, on=%d\r\n", __FUNCTION__, gAppEnv.targetNtfHandle, on);

        uint8_t ntf_cfg[2] = {0};
        if (on) {
            ntf_cfg[0] = 1;
        } else {
            ntf_cfg[0] = 0;
        }
        sonata_ble_gatt_write(conidx, gAppEnv.targetNtfHandle, 0, 0, 2, ntf_cfg);
        return true;
    } else {
        APP_TRC_ERROR("ERR: %s,targetNtfHandle [%d] is wrong, can't opt NTF\r\n", __FUNCTION__, gAppEnv.targetNtfHandle);
    }
    return false;
}

/*!
 * @brief: app profile api init
 * @param none
 * @return none
 */
void app_prf_api_init(void)
{
#if BLE_MESH
    sonata_mesh_api_init();
#endif
#if BLE_DIS_SERVER
    sonata_prf_diss_init();
#endif
#if BLE_BATT_SERVER
    sonata_prf_bass_init();
#endif
}

#if !defined ALIOS_SUPPORT && !defined HARMONYOS_SUPPORT
static uint8_t app_at_cmd_handler(void *p_param)
{
    at_command_process_ble();

    return API_SUCCESS;
}
#endif

/*
 * LOCAL VARIABLES DEFINITIONS
 ****************************************************************************************
 */

sonata_ble_hook_t app_hook = {
    assert_err,
    assert_param,
    assert_warn,
    app_init,
    platform_reset,
    get_stack_usage,
#if defined ALIOS_SUPPORT || defined HARMONYOS_SUPPORT
    printf,
#else
    __wrap_printf,
#endif
    app_prf_api_init,
#ifdef SONATA_RTOS_SUPPORT
    (void *)lega_rtos_init_semaphore,
    (void *)lega_rtos_get_semaphore,
    (void *)lega_rtos_set_semaphore,
#endif

};
uint16_t gap_active_stopped_callback(uint8_t actv_idx, uint8_t type, uint8_t reason, uint8_t per_adv_stop)
{
    APP_TRC("APP_CB: %s, reason=0x%04X\r\n", __FUNCTION__, reason);
    if (SONATA_GAP_ACTV_TYPE_ADV == type) {
        ble_adv_state = BLE_ADV_STATE_CREATED;
        if (app_evt_cb != NULL) {
            app_adv_status_ind_t status_ind;
            status_ind.advId = actv_idx;
            status_ind.status = reason;
            app_evt_cb(BLE_ADV_STOP, (void *)&status_ind);
        }
    }
    if (SONATA_GAP_ACTV_TYPE_SCAN == type) {
        return CB_REJECT; // delete scan instance
    }
    if (SONATA_GAP_ACTV_TYPE_INIT == type) {
        return CB_REJECT; // delete init instance
    }
    return CB_DONE;
}

static ble_gap_callback ble_gap_callbacks = {
    /*************** GAP Manager's callback ***************/

    // Must if use scan function, peer's information will show in this callback
    .gap_scan_result                    = app_gap_scan_result_callback,
    // Optional, use for get local devcie informations when call sonata_ble_get_dev_info()
    .get_local_dev_info                 = app_get_dev_info_callback,

    /*************** GAP Controller's callback  ***************/
    // Optional
    .gap_param_update_req               = app_gap_param_update_req_callback,
    // Optional
    .gap_param_updated                  = app_gap_param_updated_callback,
    // Optional, used for get peer att information when call  sonata_ble_gap_get_peer_info()
    .gap_get_peer_info                  = app_gap_peer_info_callback,
    // Optional, used for get peer att information when call  sonata_ble_gap_get_peer_info()
    .gap_get_peer_att_info              = app_gap_peer_att_info_callback,
    // Optional, if peer device get local device's information, app can deal with it in this callback
    .gap_peer_get_local_info            = app_gap_peer_get_local_info_callback,
    // Optional
    .gap_disconnect_ind                 = app_gap_disconnect_ind_callback,
    // Optional, if peer device set local device's name, app can deal with it in this callback
    .gap_peer_set_local_device_name     = app_gap_peer_set_local_device_name_callback,
    // Optional, app can save peer mac address in this callback when connected
    .gap_connection_req                 = app_gap_connection_req_callback,
    .gap_active_stopped                 = gap_active_stopped_callback,
    .gap_bond_req                       = app_gap_bond_req_callback,
    .gap_bond                           = app_gap_bond_callback,
    .gap_encrypt_req                    = app_gap_encrypt_req_callback,
    .gap_gen_random_number              = app_gap_gen_random_number_callback,
    .gap_security                       = app_gap_security_callback,
    .gap_encrypt                        = app_gap_encrypt_callback,
    .gap_le_pkt_size                    = app_gap_le_pke_size_callback,

};

static ble_gatt_callback ble_gatt_callbacks = {
    // Optional, add this callback if app need to save changed mtu value
    .gatt_mtu_changed                   = app_gatt_mtu_changed_callback,
    // Must,If app add custom service, app should add this callback to deal with peer device read request
    .gatt_read_req                      = app_gatt_read_request_callback,
    .gatt_read                          = app_gatt_read_callback,
    // Must,If app add custom service, app should add this callback to deal with peer device write request
    .gatt_write_req                     = app_gatt_write_request_callback,
    // Must if use discovery all servcie function
    .gatt_disc_svc                      = app_gatt_disc_svc_callback,
    // Must if use discovery all characteristic function
    .gatt_disc_char                     = app_gatt_disc_char_callback,
    // Must if use discovery all description function
    .gatt_disc_char_desc                = app_gatt_disc_desc_callback,
    .gatt_event                         = app_gatt_event_callback,
    .gatt_att_info_req                  = app_gatt_att_info_req_ind_callback,
};

static ble_complete_callback ble_complete_callbacks = {
    // Must, app can do next operation in this callback
    .ble_complete_event                 = app_ble_complete_event_handler,
};

static ble_response_callback ble_rsp_callbacks = {
    // Must,IF app add custom service, add should save this service's start handler id,
    // this id will be used in app_gatt_read_request_callback() and app_gatt_write_request_callback()
    .ble_rsp_event                      = app_ble_rsp_event_handler,
};

#if !defined ALIOS_SUPPORT && !defined HARMONYOS_SUPPORT
sonata_api_app_msg_t app_at_cmd_msg = {
    .operation = APP_MSG_AT_CMD,
    .function = app_at_cmd_handler,
};
#endif

/*
 * GLOBAL VARIABLE DEFINITIONS
 ****************************************************************************************
 */

/*
 * GLOBAL FUNCTION DEFINITIONS
 ****************************************************************************************
 */
void app_init(void)
{
    APP_TRC("APP: %s  \r\n", __FUNCTION__);
    sonata_log_level_set(SONATA_LOG_VERBOSE);

    sonata_ble_register_gap_callback(&ble_gap_callbacks);
    sonata_ble_register_gatt_callback(&ble_gatt_callbacks);
    sonata_ble_register_complete_callback(&ble_complete_callbacks);
    sonata_ble_register_response_callback(&ble_rsp_callbacks);
    app_data_init();
    app_ble_on();

    sonata_api_register_app_timer_callback(&app_timer_callbacks);

#if !defined ALIOS_SUPPORT && !defined HARMONYOS_SUPPORT
    // at command init
    at_init();

    // register app message
    sonata_api_app_msg_register(&app_at_cmd_msg);
#endif
}

uint16_t test_count = 0;
uint16_t test_interval = 0;
uint8_t  write_value[128] = {0x1, 0x2, 0x3, 0x4, 0x5};
bool ble_test_mode = false;

bool app_is_ble_test_mode(void)
{
    return ble_test_mode;
}

void app_set_ble_test_mode(bool mode)
{
    ble_test_mode = mode;
}

void app_ble_set_test_count(uint16_t counter)
{
    test_count = counter;
}

void app_ble_set_test_target(uint8_t *target)
{
    APP_TRC("APP: %s  \r\n", __FUNCTION__);

    memcpy(target_address, target, SONATA_GAP_BD_ADDR_LEN);
    for (int i = 0 ; i < SONATA_GAP_BD_ADDR_LEN; i++) {
        APP_TRC("%x ", target_address[i]);
    }
    APP_TRC("\r\n ");
}

void app_ble_set_test_write_uuid(uint8_t *uuid)
{
    APP_TRC("APP: %s  \r\n", __FUNCTION__);

    memcpy(write_uuid, uuid, 2);
    for (int i = 0 ; i < 2; i++) {
        APP_TRC("%x ", write_uuid[i]);
    }
    APP_TRC("\r\n ");
}

void app_ble_set_test_read_uuid(uint8_t *uuid)
{
    APP_TRC("APP: %s  \r\n", __FUNCTION__);

    memcpy(read_uuid, uuid, 2);
    for (int i = 0 ; i < 2; i++) {
        APP_TRC("%x ", read_uuid[i]);
    }
    APP_TRC("\r\n ");
}

void app_ble_set_test_interval(uint16_t interval)
{
    test_interval = interval;
}

static uint8_t app_timer_handler(uint16_t id)
{
    APP_TRC("APP: %s  . id=%d\r\n", __FUNCTION__, id);

    if (id == 6) {
        static uint8_t adv = 0;
        if (test_count > 0) {
            if (!adv) {
                adv_value[9] = test_count;
#ifdef HARMONYOS_TEMP
#else
#endif
            } else {
#ifdef HARMONYOS_TEMP
#else
#endif
            }
            adv = 1 - adv;
            test_count--;
            sonata_api_app_timer_set(6, 20);
            sonata_api_app_timer_active(6);
        } else {

            APP_TRC("APP: %s  . id=%d test done \r\n", __FUNCTION__, id);
            sonata_api_app_timer_clear(1);
        }

    }
    if (id == 7) {
        static uint8_t flag = 0;
        if (test_count > 0) {
            if (flag == 0) {
                flag = 1;
            } else {
                test_count--;

            }

            adv_value[9] = test_count;
            sonata_ble_set_advertising_data(adv_length - 3, adv_value + 3);
            sonata_api_app_timer_set(7, 20);
            sonata_api_app_timer_active(7);
        } else {

            flag = 0;
            APP_TRC("APP: %s  . id=%d test done \r\n", __FUNCTION__, id);
            sonata_api_app_timer_clear(2);
        }
    }

    if (id == 8) {
        static uint8_t scan_flag = 0;

        if (scan_flag == 0) {
            scan_flag = 1;
            app_ble_config_scanning();
            sonata_api_app_timer_set(8, 100);
            sonata_api_app_timer_active(8);
        } else {
            app_ble_start_scanning();

            scan_flag = 0;
            sonata_api_app_timer_clear(8);
        }
    }

    if (id == 3) {
        app_ble_config_initiating();
    }

    if (id == 4) {
        sonata_ble_gatt_write(ble_connect_id, write_handle + 1, 0, 0, 128, write_value);

        sonata_api_app_timer_set(4, test_interval);
        sonata_api_app_timer_active(4);
    }
    if (id == 5) {
        sonata_api_app_timer_set(5, 500);
        sonata_api_app_timer_active(5);
    }
    return 0;

}

extern int init_ble_task(void);
extern int deinit_ble_task(void);

uint32_t ble_open = 0;

// 连接相关
int app_ble_stack_start(ble_stack_opr_module_id_t module)
{

    if (0 != ble_open) {
        ble_open |= (1UL << module);
        return 0;
    }
    ble_open |= (1UL << module);
    init_ble_task();
    return 0;
}

int app_ble_stack_stop(ble_stack_opr_module_id_t module)
{

    ble_open &= ~(1UL << module);
    if (0 != ble_open) {
        return 0;
    }
    APP_TRC("APP: %s  \r\n", __FUNCTION__);
    ble_close();
    return 0;
}

int app_ble_disconnect_by_addr(uint8_t *addr)
{
    uint8_t conidx = app_get_conidx_by_addr(addr);
    if (conidx != ble_connect_id) {
        APP_TRC("APP: %s  not found device\r\n", __FUNCTION__);
        app_print_hex(addr, APP_BD_ADDR_LEN);
    }
    app_ble_disconnect(ble_connect_id);
    return 0;
}

void app_ble_set_target_address(uint8_t *target)
{
    APP_TRC("APP: %s  \r\n", __FUNCTION__);

    memcpy(target_address, target, SONATA_GAP_BD_ADDR_LEN);
    for (int i = 0 ; i < SONATA_GAP_BD_ADDR_LEN; i++) {
        APP_TRC("%x ", target_address[i]);
    }
    APP_TRC("\r\n ");
}

void app_ble_set_uuids(uint16_t service, uint16_t read, uint16_t write, uint16_t ntf)
{
    APP_TRC("APP: %s  service=0x%04X,read=0x%04X,write=0x%04X,ntf=0x%04X\r\n", __FUNCTION__, service, read, write, ntf);

    gAppEnv.appUuids.service = service;
    gAppEnv.appUuids.read = read;
    gAppEnv.appUuids.write = write;
    gAppEnv.appUuids.ntf = ntf;
}

app_uuids *app_ble_get_uuids()
{
    return &gAppEnv.appUuids;
}

actives *app_get_active()
{
    return gAppEnv.act;
}

void app_register_core_evt_ind(app_core_evt_ind_cb cb)
{
    app_evt_cb = cb;
}

void app_register_sec_cb(app_sec_req_cb  cb)
{
    sec_req_call = cb;
}

extern CRITICAL_FUNC_SEG void sonata_ble_isr(void);
CRITICAL_FUNC_SEG void BLE_IRQHandler(void)
{
    sonata_ble_isr();
}

// @} APP
