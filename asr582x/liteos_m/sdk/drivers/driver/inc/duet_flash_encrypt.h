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

#ifndef _DUET_FLASH_ENCRYPT_H_
#define _DUET_FLASH_ENCRYPT_H_

#include <errno.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Get the encrypt status
 *
 * @param[in]  none
 *
 * @return  0 : un-encrypt, 1 : encrypt
 */
int duet_flash_encrypt_status(void);

/**
 * Switch flash to encrypt mode.
 *
 * @note This operation is irreversible
 *
 * @param[in]  none
 *
 * @return  0 : On success, EIO : If an error occurred with any step
 */
int duet_flash_encrypt(void);

/**
 * Switch flash to disable encrypt mode.
 *
 * @note This operation is irreversible
 *
 * @return  0 : On success, EIO : If an error occurred with any step
 *
 * @return  none
 */
int duet_flash_disable_encrypt(void);

/**
 * Get the total times of encrypt and dis-encrypt operation rest.
 * When the return value reduces to zero, duet_flash_encrypt and duet_flash_disable_encrypt will be invalid.
 *
 * @param[in]  none
 *
 * @return  The total times of encrypt and dis-encrypt operation rest.
 */
int duet_flash_encrypt_operation_rest(void);

#ifdef __cplusplus
}
#endif

#endif // _DUET_FLASH_ENCRYPT_H_

