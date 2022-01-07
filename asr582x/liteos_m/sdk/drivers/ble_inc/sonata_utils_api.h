/**
 ****************************************************************************************
 *
 * @file sonata_utils_api.h
 *
 * @brief header file - asr utilities
 *
 * Copyright (C) ASR 2020 - 2029
 *
 *
 ****************************************************************************************
 */


#ifndef _SONATA_UTILS_API_H_
#define _SONATA_UTILS_API_H_

/*
 * INCLUDE FILES
 ****************************************************************************************
 */
#include "co_int.h"
#include "sonata_error_api.h"
#include "sonata_ble_hook.h"

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
#endif //SONATA_API_TASK_DBG

/// file system first tag id for application
#define APP_DATA_SAVE_TAG_FIRST              (0x90)
/// file system last tag id for application
#define APP_DATA_SAVE_TAG_LAST               (0xAF)
/// file system bluetooth device address tag id
#define SONATA_FS_TAG_BD_ADDR                (0x01)
/// file system bluetooth device address tag length
#define SONATA_FS_TAG_BD_ADDR_LEN            6


/*
 * ENUM DEFINITIONS
 ****************************************************************************************
 */


 /*!
  * @brief app msg operation type
  */
 typedef enum
 {
     /// at command operation
     APP_MSG_AT_CMD             = (1 << 0),
     APP_MSG_UART_CMD           = (5),

     APP_MSG_HID_TIMER          = (10),

     APP_MSG_LAST,
 }app_msg_op_t;

 /*!
  * @brief file system possible return status
  */
 typedef enum
 {
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
 }SONATA_FS_STATUS;
typedef uint8_t sonata_fs_len_t;
typedef uint8_t sonata_fs_tag_t;




/*
 * Type Definition
 ****************************************************************************************
 */
/// application message handler
typedef uint8_t (*PF_SONATA_API_APP_MSG_HANDLER)(void *p_param);

/// asr api task messages
typedef struct sonata_api_app_msg{
    uint8_t operation;
    PF_SONATA_API_APP_MSG_HANDLER function;
}sonata_api_app_msg_t;

/// asr api task message array
typedef struct sonata_api_app_msg_array{
    int msg_num;
    sonata_api_app_msg_t *msg[SONATA_API_MAX_APP_MSG];
}sonata_api_app_msg_array_t;

/// asr api task message array
typedef struct sonata_api_app_ke_msg
{
    uint8_t operation;
    void    *p_param;
}sonata_api_app_ke_msg_t;



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

///@hide
uint8_t sonata_api_app_ke_msg_handler(void *p_param);

///@hide
void sonata_api_util_gap_status_print(uint8_t status);

///@hide
void sonata_api_util_gatt_status_print(uint8_t operation,uint8_t status);


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
uint8_t sonata_fs_read(sonata_fs_tag_t tag, sonata_fs_len_t * lengthPtr, uint8_t *buf);

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
uint8_t * sonata_get_bt_address();

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
void sonata_set_bt_address(uint8_t * bd_addr,uint8_t length);


/*!
 * @brief change pin code to byte
 * @param pin_code
 * @param bytes
 * @param byte_len
 */
void sonata_passkey_pincode_to_byte(uint32_t pin_code, uint8_t * bytes, uint8_t byte_len);

/*!
 * @brief change byte to pin code
 * @param bytes
 * @return
 */
uint32_t sonata_passkey_byte_to_pincode(uint8_t * bytes);

/** @}*/
#endif //_SONATA_UTILS_API_H_

