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
 * @file arch.h
 *
 * @brief This file contains the definitions of the macros and functions that are
 * architecture dependent.  The implementation of those is implemented in the
 * appropriate architecture directory.
 *
 ****************************************************************************************
 */

#ifndef _ARCH_H_
#define _ARCH_H_

/*
 * INCLUDE FILES
 ****************************************************************************************
 */
#include <stdint.h>        // standard integer definition
#include <stdio.h>
#include "compiler.h"      // inline functions
#include "sonata_stack_config.h"

/*
 * MACRO DEFINES
 ****************************************************************************************
 */
// SECTION PARTITIONING
#ifdef CFG_SEG
#define CRITICAL_FUNC_SEG __attribute__((section("seg_critical_function")))
#else
#define CRITICAL_FUNC_SEG
#endif // CFG_SEG

// ARM is a 32-bit CPU
#define CPU_WORD_SIZE      4

// ARM is little endian
#define CPU_LE             1

// debug configuration
#if defined(CFG_DBG)
#define PLF_DEBUG          1
#else // CFG_DBG
#define PLF_DEBUG          0
#endif // CFG_DBG

// NVDS configuration
#ifdef CFG_NVDS
#define PLF_NVDS             1
#else // CFG_NVDS
#define PLF_NVDS             0
#endif // CFG_NVDS

// ble rom support
#if defined(CFG_BLE_ROM)
#define BLE_ROM               1
#else
#define BLE_ROM               0
#endif

// Possible errors detected by FW
#define    RESET_NO_ERROR           0x00000000
#define    RESET_MEM_ALLOC_FAIL     0xF2F2F2F2

// Reset platform and stay in ROM
#define    RESET_TO_ROM             0xA5A5A5A5
// Reset platform and reload FW
#define    RESET_AND_LOAD_FW        0xC3C3C3C3

// Exchange memory size limit
#define    EM_SIZE_LIMIT            0x8000

// maximum assert number
#define ASSERT_ID_LAST              3703s

/*
 * EXPORTED FUNCTION DECLARATION
 ****************************************************************************************
 */

/**
 ****************************************************************************************
 * @brief Compute size of SW stack used.
 *
 * This function is compute the maximum size stack used by SW.
 *
 * @return Size of stack used (in bytes)
 ****************************************************************************************
 */
uint16_t get_stack_usage(void);

/**
 ****************************************************************************************
 * @brief Re-boot FW.
 *
 * This function is used to re-boot the FW when error has been detected, it is the end of
 * the current FW execution.
 * After waiting transfers on UART to be finished, and storing the information that
 * FW has re-booted by itself in a non-loaded area, the FW restart by branching at FW
 * entry point.
 *
 * Note: when calling this function, the code after it will not be executed.
 *
 * @param[in] error      Error detected by FW
 ****************************************************************************************
 */
void platform_reset(uint32_t error);

/**
 ****************************************************************************************
 * @brief Print the assertion error reason and loop forever.
 *
 * @param condition C string containing the condition.
 * @param file C string containing file where the assertion is located.
 * @param line Line number in the file where the assertion is located.
 ****************************************************************************************
 */
void assert_err(uint16_t id, int cond);

/**
 ****************************************************************************************
 * @brief Print the assertion error reason and loop forever.
 * The parameter value that is causing the assertion will also be disclosed.
 *
 * @param param0 parameter value 0.
 * @param param1 parameter value 1.
 * @param file C string containing file where the assertion is located.
 * @param line Line number in the file where the assertion is located.
 ****************************************************************************************
 */
void assert_param(uint16_t id, int param0, int param1);

/**
 ****************************************************************************************
 * @brief Print the assertion warning reason.
 *
 * @param param0 parameter value 0.
 * @param param1 parameter value 1.
 * @param file C string containing file where the assertion is located.
 * @param line Line number in the file where the assertion is located.
 ****************************************************************************************
 */
void assert_warn(uint16_t id, int param0, int param1);

/**
 ****************************************************************************************
 * @brief Dump data value into FW.
 *
 * @param data start pointer of the data.
 * @param length data size to dump
 ****************************************************************************************
 */
void dump_data(uint8_t *data, uint16_t length);

/*
 * ASSERTION CHECK
 ****************************************************************************************
 */
#if PLF_DEBUG

extern void (*pf_sonata_assert_err)(uint16_t id, int cond);
extern void (*pf_sonata_assert_param)(uint16_t id, int param0, int param1);
extern void (*pf_sonata_assert_warn)(uint16_t id, int param0, int param1);

// Assertions showing a critical error that could require a full system reset
#define ASSERT_ERR(id, cond)                                     \
    do {                                                     \
        if (!(cond)) {                                       \
            if(pf_sonata_assert_err)                                \
            {                                                \
                pf_sonata_assert_err(id,1);                            \
            }                                                \
        }                                                    \
    } while (0)

// Assertions showing a critical error that could require a full system reset
#define ASSERT_INFO(id, cond, param0, param1)                             \
    do {                                                              \
        if (!(cond)) {                                                \
            if(pf_sonata_assert_param)                                       \
            {                                                         \
                pf_sonata_assert_param(id, (int)(param0), (int)(param1));            \
            }                                                         \
        }                                                             \
    } while (0)

// Assertions showing a non-critical problem that has to be fixed by the SW
#define ASSERT_WARN(id, cond, param0, param1)                                      \
    do {                                                                        \
        if (!(cond)) {                                                          \
            if(pf_sonata_assert_warn)                                                  \
            {                                                                   \
                pf_sonata_assert_warn(id, (int)(param0), (int)(param1));                       \
            }                                                                   \
        }                                                                       \
    } while (0)

// modified for rom code 20200224

#define DUMP_DATA(data, length) \
    dump_data((uint8_t*)(data), length)

#else
// Assertions showing a critical error that could require a full system reset
#define ASSERT_ERR(id, cond)

// Assertions showing a critical error that could require a full system reset
#define ASSERT_INFO(id, cond, param0, param1)

// Assertions showing a non-critical problem that has to be fixed by the SW
#define ASSERT_WARN(id, cond, param0, param1)

// DUMP data array present in the SW.
#define DUMP_DATA(data, length)
#endif // PLF_DEBUG

// required to define GLOBAL_INT_** macros as inline assembly. This file is included after
// definition of ASSERT macros as they are used inside ll.h

#ifdef SONATA_RTOS_SUPPORT

/* Initialises a counting semaphore and set count to 0 */
int rtos_init_semaphore(void **semaphore, int value);

/* Get (wait/decrement) a semaphore */
int rtos_get_semaphore(void **semaphore, uint32_t timeout_ms);

/* Set (post/put/increment) a semaphore */
int rtos_set_semaphore(void **semaphore);

#endif // (SONATA_RTOS_SUPPORT)

// @} DRIVERS
#endif // _ARCH_H_
