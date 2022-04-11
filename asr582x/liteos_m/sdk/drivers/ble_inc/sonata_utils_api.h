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
 * @file sonata_utils_api.h
 *
 * @brief header file - asr utilities
 *
 ****************************************************************************************
 */

#ifndef _SONATA_UTILS_API_H_
#define _SONATA_UTILS_API_H_

/*
 * INCLUDE FILES
 ****************************************************************************************
 */
#include "sonata_error_api.h"
#include "sonata_ble_hook.h"
#include "compiler.h"      // for __INLINE
#include "stdbool.h"

/**
 * @defgroup SONATA_UTILS_API UTILS_API
 * @{
 */

/*
 * MACRO DEFINITIONS
 ****************************************************************************************
 */
/// maxium supported messages from application
#define SONATA_API_MAX_APP_MSG                  32

/// debug trace
#ifdef SONATA_UTILS_API_DBG
#define SONATA_UTILS_API_TRC    printf
#else
#define SONATA_UTILS_API_TRC(...)
#endif // SONATA_API_TASK_DBG

/// file system first tag id for application
#define APP_DATA_SAVE_TAG_FIRST              (0x90)
/// file system last tag id for application
#define APP_DATA_SAVE_TAG_LAST               (0xAF)
/// file system bluetooth device address tag id
#define SONATA_FS_TAG_BD_ADDR                (0x01)
/// file system bluetooth device address tag length
#define SONATA_FS_TAG_BD_ADDR_LEN            6

#define PWR_ON_RST               0x00
#define HARDWARE_PIN_RST         0x01
#define SOFTWARE_RST             0x02
#define UNKNOWN_RST              0xFF

/*
 * ENUM DEFINITIONS
 ****************************************************************************************
 */

/*!
 * @brief app msg operation type
 */
typedef enum {
    /// at command operation
    APP_MSG_AT_CMD             = (1 << 0),
    APP_MSG_UART_CMD           = (5),

    APP_MSG_HID_TIMER          = (10),

    APP_MSG_LAST,
} app_msg_op_t;

/*!
 * @brief file system possible return status
 */
typedef enum {
    /// SONATA FILE SYSTEM status OK
    SONATA_FS_OK,
    /// generic SONATA FILE SYSTEM status KO
    SONATA_FS_FAIL,
    /// SONATA FILE SYSTEM TAG unrecognized
    SONATA_FS_TAG_NOT_DEFINED,
    /// No space for SONATA FILE SYSTEM
    SONATA_FS_NO_SPACE_AVAILABLE,
    /// Length violation
    SONATA_FS_LENGTH_OUT_OF_RANGE,
    /// SONATA FILE SYSTEM parameter locked
    SONATA_FS_PARAM_LOCKED,
    /// SONATA FILE SYSTEM corrupted
    SONATA_FS_CORRUPT,
    /// SONATA FILE TYPE OUT RANGE
    SONATA_FS_OT_RANGE,
} SONATA_FS_STATUS;
typedef uint8_t sonata_fs_len_t;
typedef uint8_t sonata_fs_tag_t;

/*!
* @brief ble addr priority return result
*/
typedef enum {
    /// efuse > nvds
    SONATA_MAC_USE_EFUSE,
    /// nvds > efuse
    SONATA_MAC_USE_NVDS,
} SONATA_MAC_PRIORITY;

/*
 * Type Definition
 ****************************************************************************************
 */
/// application message handler
typedef uint8_t (*PF_SONATA_API_APP_MSG_HANDLER)(void *p_param);

/// asr api task messages
typedef struct sonata_api_app_msg {
    uint8_t operation;
    PF_SONATA_API_APP_MSG_HANDLER function;
} sonata_api_app_msg_t;

/// asr api task message array
typedef struct sonata_api_app_msg_array {
    int msg_num;
    sonata_api_app_msg_t *msg[SONATA_API_MAX_APP_MSG];
} sonata_api_app_msg_array_t;

/// asr api task message array
typedef struct sonata_api_app_ke_msg {
    uint8_t operation;
    void    *p_param;
} sonata_api_app_ke_msg_t;

/// Ble bt addr priority callback
typedef struct {
    SONATA_MAC_PRIORITY (*ble_addr_priority)(void);

} ble_addr_callback_t;
/**
 ****************************************************************************************
 * @brief Call back definition of the function that can handle result of an AES based algorithm
 *
 * @param[in] status       Execution status
 * @param[in] aes_res      16 bytes block result
 * @param[in] src_info     Information provided by requester
 ****************************************************************************************
 */
typedef void (*aes_func_result_cb) (uint8_t status, const uint8_t *aes_res, uint32_t src_info);

/*
 * FUNCTION DECLARATIONS
 ****************************************************************************************
 */

/**
***************************************************************************************
* @brief asr api send msg from app
* @param[in] uint8_t op : operation code
*       [in] void *p_param
*
* @return uint16_t : api result
***************************************************************************************
*/
uint16_t sonata_api_send_app_msg(uint8_t op, void *p_param);

/**
***************************************************************************************
* @brief asr api register message
* @param[in] const sonata_api_msg_t *cmd
*
*
* @return  uint16_t : api result
***************************************************************************************
*/
uint16_t sonata_api_app_msg_register(const sonata_api_app_msg_t *msg);

/// @hide
uint8_t sonata_api_app_ke_msg_handler(void *p_param);

/// @hide
void sonata_api_util_gap_status_print(uint8_t status);

/// @hide
void sonata_api_util_gatt_status_print(uint8_t operation, uint8_t status);

/**
***************************************************************************************
* @brief asr api create timer
* @param[in] uint8_t timer_id : timer id, should be uniquely in global
* @param[in] uint32_t delay : delay time
*
* @return  uint16_t : api result
***************************************************************************************
*/
uint16_t sonata_api_app_timer_set(uint8_t timer_id,  uint32_t delay);

/**
***************************************************************************************
* @brief asr api clear timer
* @param[in] uint8_t timer_id : timer id, should be uniquely in global
*
*
* @return  uint16_t : api result
***************************************************************************************
*/
void sonata_api_app_timer_clear(uint8_t timer_id);

/**
***************************************************************************************
* @brief asr api to judge time active status
* @param[in] uint8_t timer_id : timer id, should be uniquely in global
*
*
* @return  bool : true(active) or false(inactive)
***************************************************************************************
*/
bool sonata_api_app_timer_active(uint8_t timer_id);

/*****************************************************************************************

* @brief sonata malloc dynamic memory
* @param[in] uint16_t size : memory size
*
*
* @return void * : NULL or memory addr
****************************************************************************************
*/
void *sonata_api_malloc(uint16_t size);

/**
***************************************************************************************
* @brief free memory alloced by sonata_api_malloc
* @param[in] void *p_ptr : memory addr
*
*
* @return void
****************************************************************************************
*/
void sonata_api_free(void *p_ptr);

/**
***************************************************************************************
* @brief sonata file system write to flash
* @param[in] sonata_fs_tag_t tag : tag
* @param[in] sonata_fs_len_t length : write len
* @param[in] uint8_t *buf           : write buf
* @note      for example: write bluetooth device address
* @note      sonata_fs_write(SONATA_FS_TAG_BD_ADDR,SONATA_FS_TAG_BD_ADDR_LEN,buf);
*
* @return uint8_t @see SONATA_FS_STATUS
****************************************************************************************
*/
uint8_t sonata_fs_write(sonata_fs_tag_t tag, sonata_fs_len_t length, uint8_t *buf);

/**
***************************************************************************************
* @brief sonata file system read flash
* @param[in] sonata_fs_tag_t tag : tag
* @param[in] sonata_fs_len_t * lengthPtr: read length
* @param[in] uint8_t *buf           : read buf
* @note      for example: read bluetooth device address
* @note      sonata_fs_len_t length = SONATA_FS_TAG_BD_ADDR_LEN;
* @note      sonata_fs_read(SONATA_FS_TAG_BD_ADDR,&length,buf);
*
* @return uint8_t @see SONATA_FS_STATUS
****************************************************************************************
*/
uint8_t sonata_fs_read(sonata_fs_tag_t tag, sonata_fs_len_t *lengthPtr, uint8_t *buf);

/**
***************************************************************************************
* @brief sonata file system erase flash
* @param[in] sonata_fs_tag_t tag : tag
*
*
* @return uint8_t @see SONATA_FS_STATUS
****************************************************************************************
*/
uint8_t sonata_fs_erase(sonata_fs_tag_t tag);

/*!
 * @brief get BT address
 * @return address
 */
uint8_t *sonata_get_bt_address(void);

/**
***************************************************************************************
* @brief sonata set bd addr
* @param[in] uint8_t* bd_addr : bd_addr value
*             uint8_t  length: addr length
*
*
*
* @return void
****************************************************************************************
*/
void sonata_set_bt_address(uint8_t *bd_addr, uint8_t length);

/**
***************************************************************************************
* @brief sonata set bd addr but no save
* @param[in] uint8_t* bd_addr : bd_addr value
*             uint8_t  length: addr length
*
*
*
* @return void
****************************************************************************************
*/
void sonata_set_bt_address_no_save(uint8_t *bd_addr, uint8_t length);

/**
 ****************************************************************************************
 * @brief Perform an AES encryption form app - result within callback
 * @param[in] key      Key used for the encryption
 * @param[in] val      Value to encrypt using AES
 * @param[in] res_cb   Function that will handle the AES based result (16 bytes)
 * @param[in] src_info Information used retrieve requester
 *
 * @return  bool : true for encryption is ongoing, false for some error
 ****************************************************************************************
 */
bool sonata_aes_app_encrypt(uint8_t *key, uint8_t *val, aes_func_result_cb res_cb, uint32_t src_info);

/**
 ****************************************************************************************
 * @brief Perform an AES decryption form app - result within callback
 * @param[in] key      Key used for the decryption
 * @param[in] val      Value to decrypt using AES
 * @param[in] res_cb   Function that will handle the AES based result (16 bytes)
 * @param[in] src_info Information used retrieve requester
 *
 * @return  bool : true for decryption is ongoing, false for some error
 ****************************************************************************************
 */
bool sonata_aes_app_decrypt(uint8_t *key, uint8_t *val, aes_func_result_cb res_cb, uint32_t src_info);

/*!
 * @brief change pin code to byte
 * @param pin_code
 * @param bytes
 * @param byte_len
 */
void sonata_passkey_pincode_to_byte(uint32_t pin_code, uint8_t *bytes, uint8_t byte_len);

/*!
 * @brief change byte to pin code
 * @param bytes
 * @return
 */
uint32_t sonata_passkey_byte_to_pincode(uint8_t *bytes);

/*!
 * @brief Get information form advertising report
 * @param type [in] GAP advertising flags, for example GAP_AD_TYPE_COMPLETE_NAME is the device's name in advertising report.
 * @param info [in]  save return value
 * @param info_length [in]  length of the info buffer
 * @param report  [in] advertising report data
 * @param report_length  [in] advertising report data length
 * @param rel_length  [out] for target information
 * @return TRUE for target fond, FALSE for not found
 */
bool sonata_get_adv_report_info(uint8_t type, uint8_t *info, uint16_t info_length, uint8_t *report,
                                uint16_t report_length, uint16_t *rel_length);

/*!
 * @brief Get system tick value
 * @return System tick
 */
uint32_t sonata_get_sys_time(void);

/*!
 * @brief Reset BLE stack
 * @return
 */
void sonata_ble_stack_reset(void);

/*!
 * @brief  Get boot type value
 * @note   in low power mode, if use this API should enable the peri_clk of RET_HCLK_EN
 * @return 0x00: PWR_ON_RST, 0x01: HARDWARE_PIN_RST, 0x02: SOFTWARE_RST, 0xFF: UNKNOWN_RST
 */
uint32_t sonata_get_boot_rst_type(void);

/*!
 * @brief  Get sonata rom version
 * @return rom version
 */
const char *sonata_get_rom_version(void);

/*!
 * @brief Ble addr priority callback
 */
void sonata_ble_register_bt_addr_callback(ble_addr_callback_t *cb);

extern int rand (void);

__INLINE uint8_t util_rand_byte(void)
{
    return (uint8_t)(rand() & 0xFF);
}
__INLINE uint32_t util_rand_word(void)
{
    return (uint32_t)rand();
}

__INLINE uint32_t util_min(uint32_t a, uint32_t b)
{
    return a < b ? a : b;
}

__INLINE void util_write16p(void const *ptr16, uint16_t value)
{
    uint8_t *ptr = (uint8_t *)ptr16;

    *ptr++ = value & 0xff;
    *ptr = (value & 0xff00) >> 8;
}

__INLINE uint16_t util_read16p(void const *ptr16)
{
    uint16_t value = (((uint8_t *)ptr16)[0]) | (((uint8_t *)ptr16)[1] << 8);
    return value;
}

__INLINE uint32_t util_read32p(void const *ptr32)
{
    uint16_t addr_l, addr_h;
    addr_l = util_read16p((uint16_t *)ptr32);
    addr_h = util_read16p((uint16_t *)ptr32 + 1);
    return (((uint32_t)addr_l) | (((uint32_t)addr_h) << 16));
}

void sonata_platform_reset(void);

/** @}*/
#endif // _SONATA_UTILS_API_H_

