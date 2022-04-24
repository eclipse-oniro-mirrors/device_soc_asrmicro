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

#ifndef SONATA_STACK_CONFIG_H_
#define SONATA_STACK_CONFIG_H_
/*
 * INCLUDE FILES
 ****************************************************************************************
 */
#include "sonata_stack_user_config.h"

/*
 * DEFINES
 ****************************************************************************************
 */
//////////////////////////////////////////
/* user configurabld */
#ifdef SONATA_CFG_APP
#ifndef CFG_APP
#define CFG_APP
#endif
#endif // SONATA_CFG_APP

#ifdef SONATA_CFG_HOST
#ifndef CFG_HOST
#define CFG_HOST
#endif
#ifndef CFG_PRF
#define CFG_PRF
#endif
#ifndef CFG_CON
#define CFG_CON                    7
#endif
#ifndef CFG_ATTC
#define CFG_ATTC
#endif
#ifndef CFG_ATTS
#define CFG_ATTS
#endif
#ifndef CFG_EXT_DB
#define CFG_EXT_DB
#endif

#ifdef SONATA_CFG_BLE_TRANSPORT
#define CFG_AHITL
#endif // SONATA_CFG_BLE_TRANSPORT

#endif // SONATA_CFG_HOST

#ifdef SONATA_CFG_BLE_TRANSPORT
#define CFG_HCITL
#endif // SONATA_CFG_BLE_TRANSPORT

#ifdef SONATA_CFG_OTAS
#define CFG_PRF_OTAS
#endif // SONATA_CFG_OTAS

#ifdef SONATA_CFG_DIS
#define CFG_PRF_DISS
#endif // SONATA_CFG_DIS

#ifdef SONATA_CFG_BASS
#define CFG_PRF_BASS
#endif // SONATA_CFG_BASS

#ifdef SONATA_CFG_MESH
#define CFG_BLE_MESH

#define CFG_BLE_MESH_MSG_API

#define CFG_BLE_ADV_TEST_MODE

#if ((defined SONATA_CFG_MESH_DBG) && (defined SONATA_CFG_DBG))
#define CFG_BLE_MESH_DBG
#endif // ((defined SONATA_CFG_MESH_DBG) && (defined SONATA_CFG_DBG))

#define CFG_BLE_MESH_RELAY

#define CFG_BLE_MESH_GATT_PROXY

#define CFG_BLE_MESH_GATT_PROV

#define CFG_BLE_MESH_FRIEND

#define CFG_BLE_MESH_LPN

#define CFG_BLE_MESH_STORAGE_NVDS

#define CFG_BLE_MESH_MDL_SERVER

#define CFG_BLE_MESH_MDL_GENS

#define CFG_BLE_MESH_MDL_CLIENT

#define CFG_BLE_MESH_MDL_GENC

#define CFG_BLE_MESH_MDL_LIGHTS

#endif // SONATA_CFG_MESH

#ifdef SONATA_CFG_NVDS
#define CFG_NVDS
#endif // SONATA_CFG_NVDS

#ifdef SONATA_CFG_DISPLAY
#define CFG_DISPLAY
#endif // SONATA_CFG_DISPLAY

#ifdef SONATA_CFG_DBG
#define CFG_DBG

#define CFG_DBG_MEM

#define CFG_DBG_FLASH

#define CFG_DBG_STACK_PROF
#endif // SONATA_CFG_DBG

#ifdef SONATA_CFG_TRC
#define CFG_TRC_EN

#define CFG_TRC_ALL
#endif // SONATA_CFG_TRC

#endif /* SONATA_STACK_CONFIG_H_ */

