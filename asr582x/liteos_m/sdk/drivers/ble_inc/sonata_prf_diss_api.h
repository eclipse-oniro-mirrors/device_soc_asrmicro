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
 * @file sonata_prf_diss_api.h
 *
 * @brief header file - device information service server api
 *
 ****************************************************************************************
 */

#ifndef _SONATA_PRF_DISS_API_H_
#define _SONATA_PRF_DISS_API_H_

/*
 * INCLUDE FILES
 ****************************************************************************************
 */
/// @cond
#if BLE_HOST_PRESENT
/// @endcond
#include "sonata_utils_api.h"
#include "sonata_api_task.h"

/**
 * @defgroup SONATA_PRF_DISS_API PRF_DISS_API
 * @{
 */

/*
 * MACRO DEFINITIONS
 ****************************************************************************************
 */

#define PRF_DISS_FEATURES                   (0x01FF)

/*
 * ENUM DEFINITIONS
 ****************************************************************************************
 */

/// Attribute Table Indexes
typedef enum {
    /// Manufacturer Name
    SONATA_DIS_MANUFACTURER_NAME_CHAR,
    /// Model Number
    SONATA_DIS_MODEL_NB_STR_CHAR,
    /// Serial Number
    SONATA_DIS_SERIAL_NB_STR_CHAR,
    /// HW Revision Number
    SONATA_DIS_HARD_REV_STR_CHAR,
    /// FW Revision Number
    SONATA_DIS_FIRM_REV_STR_CHAR,
    /// SW Revision Number
    SONATA_DIS_SW_REV_STR_CHAR,
    /// System Identifier Name
    SONATA_DIS_SYSTEM_ID_CHAR,
    /// IEEE Certificate
    SONATA_DIS_IEEE_CHAR,
    /// Plug and Play Identifier
    SONATA_DIS_PNP_ID_CHAR,

    SONATA_DIS_CHAR_MAX,
} sonata_dis_info;

/*
 * Type Definition
 ****************************************************************************************
 */

/*!
 * @brief Callback for diss value
 */
typedef struct prf_diss_callback {
    uint16_t (*prf_diss_value_req)(sonata_dis_info info_id);

} prf_diss_callback_t;

/*
 * FUNCTION DECLARATIONS
 ****************************************************************************************
 */
/*!
 * @brief init diss profile
 */
void sonata_prf_diss_init(void);

/*!
* @brief Send value confirm message
* @param task_id task_id was get in SONATA_GAP_PROFILE_ADDED_IND event
* @param value @see sonata_dis_info
* @param data_len data length
* @param data data
* @return API_SUCCESS
*/
int sonata_prf_diss_send_value_cfm_cmd(sonata_dis_info value, uint8_t data_len, uint8_t *data);

/*!
 * @brief add diss profile
 */
void sonata_prf_diss_add_dis(void);

/*!
 * @brief Add diss callback
 * @param cb
 * @return API_SUCCESS
 * @note if App need to modify the diss value in runtime, then should use this callback
 */
uint16_t sonata_prf_diss_register_callback(prf_diss_callback_t *cb);

extern const sonata_api_subtask_handlers_t prf_diss_api_ke_msg_handlers;

/** @}*/

#endif // BLE_HOST_PRESENT

#endif // _SONATA_PRF_DISS_API_H_

