/**
 ****************************************************************************************
 *
 * @file sonata_error_api.h
 *
 * @brief header file - this file contains the ble error code definition
 *
 * Copyright (C) ASR 2020 - 2029
 *
 *
 ****************************************************************************************
 */


#ifndef _SONATA_ERROR_API_H_
#define _SONATA_ERROR_API_H_

/*
 * INCLUDE FILES
 ****************************************************************************************
 */
#include "co_int.h"
#include "co_error.h"
#include "rwble_hl_error.h"



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
#endif //SONATA_API_TASK_DBG

/*
 * ENUM DEFINITIONS
 ****************************************************************************************
 */








#endif //_SONATA_ERROR_API_H_

