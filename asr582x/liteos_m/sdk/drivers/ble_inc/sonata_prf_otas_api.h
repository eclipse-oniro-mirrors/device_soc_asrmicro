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
 * @file sonata_prf_otas_api.h
 *
 * @brief header file - sonata_prf_otas_api
 *
 ****************************************************************************************
 */

#ifndef SONATA_PRF_OTAS_API_H_
#define SONATA_PRF_OTAS_API_H_

/*
 * INCLUDE FILES
 ****************************************************************************************
 */

/// @cond
#ifdef SONATA_CFG_OTAS
/// @endcond
#include <stdint.h>

/**
 * @defgroup SONATA_PRF_OTAS_API PRF_OTAS_API
 * @{
 */

/**
 ****************************************************************************************
 * @brief Callback for ota status
 ****************************************************************************************
 */
typedef struct prf_ota_callback {
    /// app call before ota start
    void (*prf_ota_before_start)(void);
    /// app call ota start
    uint8_t (*prf_ota_start)(uint8_t length, uint8_t *version);
    /// app call ota stop
    void (*prf_ota_stop)(void);
    /// app call ota fail
    void (*prf_ota_fail)(void);
} prf_ota_callback_t;

/*
 * DEFINES
 ****************************************************************************************
 */
/// the features of ota are supported
#define PRF_OTAS_FEATURES             (OTA_ALL_FEAT_SUP)

/*
 * GLOBAL VARIABLES DECLARATION
 ****************************************************************************************
 */

/// Table of message handlers
extern const struct sonata_api_subtask_handlers_t sonata_prf_otas_handlers;

/*
 * GLOBAL FUNCTIONS DECLARATION
 ****************************************************************************************
 */

/**
 ****************************************************************************************
 * @brief Initialize Device Information Service Application
 * @param[in] void
 * @return void
 ****************************************************************************************
 */
void sonata_prf_otas_init(void);

/**
 ****************************************************************************************
 * @brief Add a Device Information Service instance in the DB
 * @param[in] void
 * @return void
 ****************************************************************************************
 */
void sonata_prf_otas_add_otas(void);

void sonata_prf_otas_disable(bool disable);
/**
 ****************************************************************************************
 * @brief sonata_prf_ota_register_callback
 * @param[in] cb
 * @return API_SUCCESS
 ****************************************************************************************
 */
uint16_t sonata_prf_ota_register_callback(prf_ota_callback_t *cb);

/// Table of kernel message handlers
extern const sonata_api_subtask_handlers_t sonata_prf_otas_ke_msg_handlers;
#endif // SONATA_CFG_OTAS

/** @}*/
#endif // SONATA_PRF_OTAS_API_H_