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
 * @file lwifi_task.h
 *
 * @brief define basic type
 *
 ****************************************************************************************
 */
#ifndef _TYPES_H_
#define _TYPES_H_

#include <stdint.h>
#include <stdbool.h>

#if 0   // use standard defines

#ifndef int8_t
typedef signed char int8_t;
#endif

#ifndef uint8_t
typedef unsigned char uint8_t;
#endif

#ifndef int16_t
typedef signed short int16_t;
#endif

#ifndef uint16_t
typedef unsigned short uint16_t;
#endif

#ifndef int32_t
typedef signed int int32_t;
#endif

#ifndef uint32_t
typedef unsigned int uint32_t;
#endif

#ifndef int64_t
typedef signed long long int64_t;
#endif

#ifndef uint64_t
typedef unsigned long long uint64_t;
#endif

#ifndef true
#define true 1
#endif

#ifndef false
#define false 0
#endif

#ifndef bool
typedef unsigned char bool;
#endif

#endif
#ifndef NULL
#define NULL (void*)0
#endif
#define BIT(n)           (1<<(n))
#define CO_WF_BIT(pos)      (1U<<(pos))

// structure of a list element header
struct co_wf_list_hdr {
    // Pointer to the next element in the list
    struct co_wf_list_hdr *next;
};

inline static uint16_t  ___swap16(uint16_t x)
{
    uint16_t __x = x;
    return ((uint16_t)((((uint16_t)(__x) & (uint16_t)0x00ffU) << 8) |
                       (((uint16_t)(__x) & (uint16_t)0xff00U) >> 8)));
}

inline static uint16_t __arch__swap16(uint16_t x)
{
    return ___swap16(x);
}

inline static uint16_t __fswap16(uint16_t x)
{
    return __arch__swap16(x);
}

#define __swap16(x) __fswap16(x)

#define __cpu_to_be16(x) __swap16(x)
#define __be16_to_cpu(x) __swap16(x)

#define wlan_htons(x) __cpu_to_be16(x)
// #define ntohs(x) __be16_to_cpu(x)

#endif // _TYPES_H_

