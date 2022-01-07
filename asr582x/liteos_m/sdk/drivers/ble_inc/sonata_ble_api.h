/**
 ****************************************************************************************
 *
 * @file sonata_ble_api.h
 *
 * @brief header file - ble common operation
 *
 * Copyright (C) ASR 2020 - 2029
 *
 *
 ****************************************************************************************
 */


#ifndef _SONATA_BLE_API_H_
#define _SONATA_BLE_API_H_

/*
 * INCLUDE FILES
 ****************************************************************************************
 */
#include "rwip_config.h"
#include "sonata_utils_api.h"
#include "co_bt_defines.h"
#include "gap.h"
#include "sonata_gatt_api.h"
#include "sonata_gap_api.h"
#include "sonata_prf_diss_api.h"
#include "sonata_log.h"
/*
 * MACRO DEFINITIONS
 ****************************************************************************************
 */
/**
 * @defgroup SONATA_BLE_API BLE_API
 * @{
 */


/// debug trace
#define SONATA_BLE_API_DBG    1
#if SONATA_BLE_API_DBG
#define SONATA_BLE_API_TRC    printf
#else
#define SONATA_BLE_API_TRC(...)
#endif //SONATA_BLE_API_DBG


#define SONATA_TASK_FIRST_MSG(task) ((uint16_t)((task) << 8))


/*
 * ENUM DEFINITIONS
 ****************************************************************************************
 */

/// App callback status
typedef enum
{
    CB_DONE           = 0, //Done in callback
    CB_REJECT         = 1, //Do nothing in callback, SDK will do it
    CB_MAX            =0XFF,
}CBStatus;

/// Result of sleep state.
enum sonata_ble_sleep_state
{
    /// Some activity pending, can not enter in sleep state
    SONATA_BLE_ACTIVE    = 0,
    /// CPU can be put in sleep state
    SONATA_BLE_CPU_SLEEP,
    /// IP could enter in deep sleep
    SONATA_BLE_DEEP_SLEEP,
};


/// Definition of the bits preventing the system from sleeping
typedef enum
{
    /// Flag indicating that the ble app is WAKE
    SONATA_BLE_APP_WAKE                    = 0x2000,
}sonata_ble_prevent_sleep_bit;
/*
 * Type Definition
 ****************************************************************************************
 */


/*
 * GLOBAL VARIABLE DECLARATIONS
 ****************************************************************************************
 */

/*!
 * @brief
 */
extern uint32_t reset_error;

/*
 * FUNCTION DECLARATIONS             Developer use function
 ****************************************************************************************
 */
/*!
 * @brief Init ble module
 * @param hook @see sonata_ble_hook_t
 */
void sonata_ble_init(sonata_ble_hook_t hook);

/*!
 * @brief Set a bit in the prevent sleep bit field, in order to prevent the system from  going to sleep
 * @param prv_slp_bit Bit to be set in the prevent sleep bit field
 */
void sonata_ble_prevent_sleep_set(sonata_ble_prevent_sleep_bit prv_slp_bit);


/*!
 * @brief Clears a bit in the prevent sleep bit field, in order to allow the system
 *        going to sleep
 * @param prv_slp_bit Bit to be cleared in the prevent sleep bit field
 */
void sonata_ble_prevent_sleep_clear(sonata_ble_prevent_sleep_bit prv_slp_bit);

/*!
 * @brief Reset ble module
 * @return API_SUCCESS
 */
uint16_t sonata_ble_reset();

/*!
 * @brief ble stack schedule
 */

void sonata_ble_schedule(void);

/*!
 * @brief ble enter sleep
 * @return Sleep state
 */
uint8_t sonata_ble_sleep(void);

/*!
 * @brief ble irq handler
 */
void sonata_ble_isr(void);

/*
 * FUNCTION DECLARATIONS             Connection help function End
 ****************************************************************************************
 */


/** @}*/


#endif //_SONATA_BLE_API_H_

