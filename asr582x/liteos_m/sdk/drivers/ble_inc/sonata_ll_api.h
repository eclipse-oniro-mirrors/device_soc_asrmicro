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
 * @file sonata_ll_api.h
 *
 * @brief header file - ll api
 *
 ****************************************************************************************
 */

#ifndef _SONATA_LL_API_H_
#define _SONATA_LL_API_H_

/*
 * INCLUDE FILES
 ****************************************************************************************
 */

#include "sonata_utils_api.h"

/**
 * @defgroup SONATA_LL_API LL API
 * @{
 */

/*
 * MACRO DEFINITIONS
 ****************************************************************************************
 */

/*
 * ENUM DEFINITIONS
 ****************************************************************************************
 */

/*
 * Type Definition
 ****************************************************************************************
 */

/*
 * FUNCTION DECLARATIONS
 ****************************************************************************************
 */
/**
 ****************************************************************************************
 * @brief set link layer transmit power
 * @param[in] txpwr txpwr range [0,2,4,8,10]
 * @return bool false or value of the txpwr
 ****************************************************************************************
 */
bool sonata_ll_set_txpwr(int8_t txpwr);
/**
 ****************************************************************************************
 * @brief get link layer transmit power
 * @param[in] void
 * @return int8_t value of the txpwr
 ****************************************************************************************
 */
int8_t sonata_ll_get_txpwr(void);
/**
 ****************************************************************************************
 * @brief radio transmit tone
 * @param[in] tx_channel range: [02-80] = [2402-2480MHz]
 * @return bool status of true or false
 ****************************************************************************************
 */
bool sonata_ll_rf_tx_tone(uint8_t tx_channel);
/**
 ****************************************************************************************
 * @brief radio transmit test
 * @param[in] tx_channel range: [02-80] = [2402-2480MHz]
 * @param[in] pattern    range: [00-03] = [0: all 0 pattern 1: all 1 pattern 2: 0011_1100 pattern 3: prbs pattern]
 * @return bool status of true or false
 ****************************************************************************************
 */
bool sonata_ll_rf_tx_test(uint8_t tx_channel, uint8_t pattern);
/**
 ****************************************************************************************
 * @brief radio test mode end
 * @param[in] void
 * @return void
 ****************************************************************************************
 */
void sonata_ll_rf_test_end(void);

/**
 ****************************************************************************************
 * @brief direction finding initialization:
 *        - support up to 16 antennae
 *        - p08,p09,p10,p11
 * @param[in] void
 * @return void
 ****************************************************************************************
 */
void sonata_ll_df_init(void);

/// @cond
/**
 ****************************************************************************************
 * @brief ll_pa_test
 * @param[in] void
 * @return void
 ****************************************************************************************
 */
void sonata_ll_pa_enable(void);
/// @endcond
/** @}*/
void sonata_ll_df_cal(void);

/// @cond
extern bool sonata_rw_df_dbg_enable;
/// @endcond

#endif // _SONATA_LL_API_H_

