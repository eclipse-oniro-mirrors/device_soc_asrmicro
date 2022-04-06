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
 * @file duet_flash_kv.h
 *
 * @brief this module is based on classical key/value(KV) store system for flash storage.
 *        provide method to access flash memory by using key/value pairs.
 *        internal algorithm could balance frequency of flash utilization on different areas
 *        to improve flash using life.
 *
 ****************************************************************************************
 */
#ifndef _DUET_FLASH_KV_H_
#define _DUET_FLASH_KV_H_
#include "stdint.h"
/**
 * Init the flash kv module.
 *
 * @retrun 0 on success, otherwise will be failed.
 */
int32_t duet_flash_kv_init(void);

/**
 * Deinit the kv module.
 *
 * @retrun none
 */
void duet_flash_kv_deinit(void);

/**
 * Add a new flash KV pair.
 *
 * @param[in]  key    the key of the KV pair.
 * @param[in]  value  the value of the KV pair.
 * @param[in]  len    the length of the value.
 * @param[in]  sync   save the KV pair to flash right now (should always be 1).
 *
 * @return  0 on success, negative error on failure.
 */
int32_t duet_flash_kv_set(const char *key, const void *value, int32_t len, int32_t sync);

/**
 * Get the flash KV pair's value stored in buffer by its key.
 *
 * @note: the buffer_len should be larger than the real length of the value,
 *        otherwise buffer would be NULL.
 *
 * @param[in]      key         the key of the KV pair to get.
 * @param[out]     buffer      the memory to store the value.
 * @param[in-out]  buffer_len  in: the length of the input buffer.
 *                             out: the real length of the value.
 *
 * @return  0 on success, negative error on failure.
 */
int32_t duet_flash_kv_get(const char *key, void *buffer, int32_t *buffer_len);

/**
 * Delete the flash KV pair by its key.
 *
 * @param[in]  key  the key of the KV pair to delete.
 *
 * @return  0 on success, negative error on failure.
 */
int32_t duet_flash_kv_del(const char *key);

#endif  /* _DUET_FLASH_KV_H_ */

