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

#ifndef _DUET_FLASH_H_
#define _DUET_FLASH_H_
#include <stdint.h>

#define LOW_LEVEL_FLASH_RW_SUPPORT 1

#define PAR_OPT_READ_POS  (0)
#define PAR_OPT_WRITE_POS (1)

#define PAR_OPT_READ_MASK  (0x1u << PAR_OPT_READ_POS)
#define PAR_OPT_WRITE_MASK (0x1u << PAR_OPT_WRITE_POS)

#define PAR_OPT_READ_DIS  (0x0u << PAR_OPT_READ_POS)
#define PAR_OPT_READ_EN   (0x1u << PAR_OPT_READ_POS)
#define PAR_OPT_WRITE_DIS (0x0u << PAR_OPT_WRITE_POS)
#define PAR_OPT_WRITE_EN  (0x1u << PAR_OPT_WRITE_POS)

typedef enum {
    PARTITION_ERROR = -1,
    PARTITION_BOOTLOADER,
    PARTITION_APPLICATION,
    PARTITION_ATE,
    // PARTITION_token_TEMP,
    PARTITION_OTA_TEMP,
    PARTITION_RF_FIRMWARE,
    PARTITION_PARAMETER_1,
    PARTITION_PARAMETER_2,
    PARTITION_PARAMETER_3,
    PARTITION_PARAMETER_4,
    PARTITION_BT_FIRMWARE,
    PARTITION_SPIFFS,
    PARTITION_CUSTOM_1,
    PARTITION_CUSTOM_2,
    PARTITION_RECOVERY,
#ifdef MS_CONFIG_OTA_SUPPORT
    PARTITION_OTA_MCU,
    PARTITION_OTA_PARA,
    PARTITION_OTA_HEAD_PARA,
#endif
    PARTITION_PARAMETER_5,
    PARTITION_MAX,
    PARTITION_NONE,
} duet_partition_t;

typedef enum {
    FLASH_EMBEDDED,
    FLASH_SPI,
    FLASH_QSPI,
    FLASH_MAX,
    FLASH_NONE,
} duet_flash_t;

typedef struct {
    duet_flash_t partition_owner;
    const char *partition_description;
    uint32_t    partition_start_addr;
    uint32_t    partition_length;
    uint32_t    partition_options;
} duet_logic_partition_t;

/**
 * lega flash init
 *
 * @note this function must be called before flash erase/write operation
 *       also, this function must not be interrupted, it should be called
 *       with interrupt disabled
 * @param[in]  none
 *
 * @return     HAL_logi_partition struct
 */
int32_t duet_flash_init(void);

/**
 * Get the infomation of the specified flash area
 *
 * @param[in]  in_partition  The target flash logical partition
 *
 * @return     HAL_logi_partition struct
 */
duet_logic_partition_t *duet_flash_get_info(duet_partition_t in_partition);

/**
 * Erase an area on a Flash logical partition
 *
 * @note  Erase on an address will erase all data on a sector that the
 *        address is belonged to, this function does not save data that
 *        beyond the address area but in the affected sector, the data
 *        will be lost.
 *
 * @param[in]  in_partition  The target flash logical partition which should be erased
 * @param[in]  off_set       Start address of the erased flash area
 * @param[in]  size          Size of the erased flash area
 *
 * @return  0 : On success, EIO : If an error occurred with any step
 */
int32_t duet_flash_erase(duet_partition_t in_partition, uint32_t off_set, uint32_t size);

/**
 * Write data to an area on a flash logical partition without erase
 *
 * @param[in]  in_partition    The target flash logical partition which should be read which should be written
 * @param[in]  off_set         Point to the start address that the data is written to, and
 *                             point to the last unwritten address after this function is
 *                             returned, so you can call this function serval times without
 *                             update this start address.
 * @param[in]  inBuffer        point to the data buffer that will be written to flash
 * @param[in]  inBufferLength  The length of the buffer
 *
 * @return  0 : On success, EIO : If an error occurred with any step
 */
int32_t duet_flash_write(duet_partition_t in_partition, uint32_t *off_set,
                         const void *in_buf, uint32_t in_buf_len);

/**
 * Write data to an area on a flash logical partition with erase first
 *
 * @param[in]  in_partition    The target flash logical partition which should be read which should be written
 * @param[in]  off_set         Point to the start address that the data is written to, and
 *                             point to the last unwritten address after this function is
 *                             returned, so you can call this function serval times without
 *                             update this start address.
 * @param[in]  inBuffer        point to the data buffer that will be written to flash
 * @param[in]  inBufferLength  The length of the buffer
 *
 * @return  0 : On success, EIO : If an error occurred with any step
 */
int32_t duet_flash_erase_write(duet_partition_t in_partition, uint32_t *off_set,
                               const void *in_buf, uint32_t in_buf_len);

/**
 * Read data from an area on a Flash to data buffer in RAM
 *
 * @param[in]  in_partition    The target flash logical partition which should be read
 * @param[in]  off_set         Point to the start address that the data is read, and
 *                             point to the last unread address after this function is
 *                             returned, so you can call this function serval times without
 *                             update this start address.
 * @param[in]  outBuffer       Point to the data buffer that stores the data read from flash
 * @param[in]  inBufferLength  The length of the buffer
 *
 * @return  0 : On success, EIO : If an error occurred with any step
 */
int32_t duet_flash_read(duet_partition_t in_partition, uint32_t *off_set,
                        void *out_buf, uint32_t in_buf_len);

/**  Erase internal flash
 *
 * This will erase the flash for the specified start address
 * and size
 *
 * \param addr Start address in flash (*MUST* 4 KB alignment)
 * \param len Number of bytes to be erased (*MUST* 4 KB alignment, erase must in one partition)
 * \return 0 on success
 */
int32_t duet_flash_erase_common(uint32_t addr, uint32_t len);

/**  Write data to internal flash
 *
 * This will write specified number of bytes to flash.
 * Note that flash don't need to be erased before writing.
 *
 * \param addr Write address in flash
 * \param buff Pointer to the data to be written
 * \param len Length of data to be written, make sure write only in one partition
 *
 * \return 0 on success
 */
int32_t duet_flash_write_common(uint32_t addr, const void *in_buff, uint32_t len);
/**
 * Set security options on a logical partition
 *
 * @param[in]  partition  The target flash logical partition
 * @param[in]  offset     Point to the start address that the data is read, and
 *                        point to the last unread address after this function is
 *                        returned, so you can call this function serval times without
 *                        update this start address.
 * @param[in]  size       Size of enabled flash area
 *
 * @return  0 : On success, EIO : If an error occurred with any step
 */
int32_t duet_flash_enable_secure(duet_partition_t partition, uint32_t off_set, uint32_t size);

/**
 * Disable security options on a logical partition
 *
 * @param[in]  partition  The target flash logical partition
 * @param[in]  offset     Point to the start address that the data is read, and
 *                        point to the last unread address after this function is
 *                        returned, so you can call this function serval times without
 *                        update this start address.
 * @param[in]  size       Size of disabled flash area
 *
 * @return  0 : On success, EIO : If an error occurred with any step
 */
int32_t duet_flash_dis_secure(duet_partition_t partition, uint32_t off_set, uint32_t size);
#ifdef LOW_LEVEL_FLASH_RW_SUPPORT
#define FLASH_MAC_ADDR_TOKEN_NAME "wifi_mac_addr_default_value"
#define FLASH_MAC_ADDR_TOKEN      (0xACBDEFFE)
#define FLASH_MAC_ADDR_TOKEN_LEN  (4)
#define MAC_ADDR_LEN              (6)
typedef struct {
    uint8_t   mac[MAC_ADDR_LEN];
    uint8_t   resv[2];
    uint32_t  token;
} flash_mac_addr_t;
/* *
 * * Wifi mac addr should use efuse value
 * * If efuse have no mac addr, use this value
 * * The mac addr saved in flash, use kv to store. if no kv, user should rewrite the read/write function
 * * Default set 0x8c59dc + radom value
 */
int32_t duet_flash_get_wifi_mac(flash_mac_addr_t *addr);
int32_t duet_flash_set_wifi_mac(flash_mac_addr_t *addr);
#endif
#endif // _LEGA_FLASH_H_
