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
 * @file sonata_prf_bass_api.h
 *
 * @brief header file - Battery service server api
 *
 ****************************************************************************************
 */

#ifndef _SONATA_PRF_BASS_API_H_
#define _SONATA_PRF_BASS_API_H_

/**
 * @defgroup SONATA_PRF_BASS_API PRF_BASS_API
 * @{
 */

/*
 * INCLUDE FILES
 ****************************************************************************************
 */

// #if (BLE_APP_BATT)

#include <stdint.h>          // Standard Integer Definition
#include "sonata_ble_api.h"

/*
 * STRUCTURES DEFINITION
 ****************************************************************************************
 */

/*
 * Type Definition
 ****************************************************************************************
 */

/*!
 * @brief Callback for diss value
 */
typedef struct prf_bas_callback {
    // Callback for battery level request, if config==1, app should send battery level value use sonata_prf_bas_send_battery_level()
    uint16_t (*prf_bass_value_req)(uint8_t conidx, uint8_t config);
    // Callback for bass enabled
    uint16_t (*prf_bass_enable)(uint8_t conidx, uint8_t status);
    // Callback for battery level notifiyed
    uint16_t (*prf_bass_level_updated)(uint8_t status);

} prf_bas_callback_t;

typedef enum {
    // Start the Battery Server - at connection used to restore bond data
    SONATA_PRF_BASS_ENABLE_REQ = SONATA_TASK_FIRST_MSG(SONATA_TASK_ID_BASS),
    // Confirmation of the Battery Server start
    SONATA_PRF_BASS_ENABLE_RSP,
    // Battery Level Value Update Request
    SONATA_PRF_BASS_BATT_LEVEL_UPD_REQ,
    // Inform APP if Battery Level value has been notified or not
    SONATA_PRF_BASS_BATT_LEVEL_UPD_RSP,
    // Inform APP that Battery Level Notification Configuration has been changed - use to update bond data
    SONATA_PRF_BASS_BATT_LEVEL_NTF_CFG_IND,
} sonata_bass_msg_id;

/*
 * GLOBAL VARIABLES DECLARATIONS
 ****************************************************************************************
 */

/*
 * FUNCTIONS DECLARATION
 ****************************************************************************************
 */

/*!
 * @brief Init environment
 */
void sonata_prf_bass_init(void);

/*!
 * @brief Add a Battery Service instance in the DB
 */
void sonata_prf_bas_add_bass(uint8_t bas_number, bool support_ntf);

/*!
 * @brief Enable the Battery Service
 * @param conidx connection index
 * @param batt_lvl battery level value
 */
void sonata_prf_bas_enable_bass(uint8_t conidx, uint8_t batt_lvl);

/*!
 * @brief Send a Battery level value
 * @param batt_id For single battery, batt_id is 0
 * @param batt_lvl battery level value (0~100)
 * @return API_SUCCESS
 */
int sonata_prf_bas_send_battery_level(uint8_t batt_id, uint8_t batt_lvl);

/*!
 * @brief Set callback function
 * @param cb callback functions
 * @return API_SUCCESS
 */
uint16_t sonata_prf_bas_register_callback(prf_bas_callback_t *cb);

// #endif // (BLE_APP_BATT)

/** @}*/

extern const sonata_api_subtask_handlers_t prf_bass_api_ke_msg_handlers;

#endif // _SONATA_PRF_BASS_API_H_
