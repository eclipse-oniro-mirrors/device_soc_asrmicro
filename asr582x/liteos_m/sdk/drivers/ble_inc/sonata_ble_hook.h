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
 * @file sonata_ble_hook.h
 *
 * @brief Header file - This file contains application specific hooks
 *
 ****************************************************************************************
 */

#ifndef _SONATA_BLE_HOOK_H_
#define _SONATA_BLE_HOOK_H_

#include <stdint.h>    // standard integer definition

/*
 * TYPEDEF
 *****************************************************************************************
 */
typedef void (*PF_APP_INIT)(void);
typedef void (*PF_PLF_RESET)(uint32_t error);
typedef uint16_t (*PF_GET_STACK_USAGE)(void);
typedef int (*PF_PRINTF)(const char *format, ...);
typedef void (*PF_APP_PRF_API_INIT)(void);

typedef struct sonata_ble_hook {
#ifdef CFG_PLF_SONATA
    void (*pf_assert_err)(uint16_t id, int cond);
    void (*pf_assert_param)(uint16_t id, int param0, int param1);
    void (*pf_assert_warn)(uint16_t id, int param0, int param1);
#endif // CFG_PLF_SONATA

#ifdef CFG_PLF_DUET
    void (*pf_assert_err)(int cond);
    void (*pf_assert_param)(int param0, int param1);
    void (*pf_assert_warn)(int param0, int param1);
#endif // CFG_PLF_DUET

    PF_APP_INIT pf_app_init;
    PF_PLF_RESET pf_platform_reset;
    PF_GET_STACK_USAGE pf_get_stack_usage;
    PF_PRINTF pf_printf;
    PF_APP_PRF_API_INIT pf_app_prf_api_init;

#ifdef SONATA_RTOS_SUPPORT
    int (*pf_init_semaphore)(void **semaphore, int value);
    int (*pf_get_semaphore)(void **semaphore, uint32_t timeout_ms);
    int (*pf_set_semaphore)(void **semaphore);
#endif // (SONATA_RTOS_SUPPORT)
} sonata_ble_hook_t;

/*
 * VARIABLE DECLARATION
 *****************************************************************************************
 */
extern void (*pf_sonata_assert_err)(uint16_t id, int cond);
extern void (*pf_sonata_assert_param)(uint16_t id, int param0, int param1);
extern void (*pf_sonata_assert_warn)(uint16_t id, int param0, int param1);
extern PF_APP_INIT pf_app_init;
extern PF_PLF_RESET pf_platform_reset;
extern PF_GET_STACK_USAGE pf_get_stack_usage;
extern PF_PRINTF pf_printf;
extern PF_APP_PRF_API_INIT pf_app_prf_api_init;

/*
 * FUNCTION DECLARATION
 *****************************************************************************************
 */
extern void sonata_ble_hook_register(sonata_ble_hook_t hook);

#endif // _SONATA_BLE_HOOK_H_

