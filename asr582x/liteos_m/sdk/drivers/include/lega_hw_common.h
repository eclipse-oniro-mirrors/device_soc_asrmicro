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

#ifndef _LEGA_HW_COMMON_H_
#define _LEGA_HW_COMMON_H_

#if defined(ALIOS_SUPPORT)
#include "lega_rhino.h"

#elif defined(HARMONYOS_SUPPORT)
#include "lega_rtos.h"
#elif defined(SYSTEM_SUPPORT_OS)
#include "lega_rtos_api.h"
#endif
extern lega_mutex_t HwSecurityMutex;
#define LEGA_HW_OK         (0)
#define LEGA_HW_ERR        (-1)

/*!
HASH operation mode
*/
typedef enum {
    LEGA_HASH_SHA1_mode          = 0,    /* !< SHA1 */
    LEGA_HASH_SHA224_mode        = 1,    /* !< SHA224 */
    LEGA_HASH_SHA256_mode        = 2,    /* !< SHA256 */
    LEGA_HASH_SHA384_mode        = 3,    /* !< SHA384 */
    LEGA_HASH_SHA512_mode        = 4,    /* !< SHA512 */
    LEGA_HASH_MD5_mode           = 5,    /* !< MD5 */

    LEGA_HASH_NumOfModes,
    LEGA_HASH_OperationModeLast = 0x7FFFFFFF,
} LEGA_HASH_OperationMode_t;

/*! Defines the HASH result buffer. */
typedef uint32_t LEGA_HASH_Result_t[16];

extern uint8_t   *rndContext_ptr;
extern uint8_t  *rndWorkBuff_ptr;
extern int hw_lib_init_flag;

int lega_hw_lib_init(void);
int lega_hw_lib_deinit(void);
int lega_security_engine_init(void);
int lega_security_engine_deinit(void);

#endif // _LEGA_HW_COMMON_H_