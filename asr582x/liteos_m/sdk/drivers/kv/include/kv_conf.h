/*
 * Copyright (C) 2015-2017 Alibaba Group Holding Limited.
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

#ifndef KV_CONF_H
#define KV_CONF_H

#include "duet_cm4.h"
#include "cmsis_os2.h"

#define KV_CONFIG_TOTAL_SIZE 0x8000

/* The totally storage size for key-value store */
#ifndef KV_CONFIG_TOTAL_SIZE
#define KV_TOTAL_SIZE KV_MAX_SIZE
#else
#define KV_TOTAL_SIZE KV_CONFIG_TOTAL_SIZE
#endif

#if KV_TOTAL_SIZE < 0x2000
#error KV TOTALSIZE ERROR
#endif

/* The physical parition for key-value store */
#ifndef KV_CONFIG_PARTITION
#define KV_PARTITION PARTITION_PARAMETER_2
#else
#define KV_PARTITION KV_CONFIG_PARTITION
#endif

/* The number of bits in block size */
#ifndef KV_CONFIG_BLOCK_SIZE_BITS
/* Default is 4K bytes, should equal or larger than erase sector size */
#define KV_BLOCK_SIZE_BITS 14
#else
#define KV_BLOCK_SIZE_BITS KV_CONFIG_BLOCK_SIZE_BITS
#endif

#ifndef KV_CONFIG_TASK_PRIORITY
#define KV_TASK_PRIORITY osPriorityAboveNormal
#else
#define KV_TASK_PRIORITY KV_CONFIG_TASK_PRIORITY
#endif

#ifndef KV_CONFIG_TASK_STACK_SIZE
#define KV_TASK_STACK_SIZE 1024
#else
#define KV_TASK_STACK_SIZE KV_CONFIG_TASK_STACK_SIZE
#endif

#ifndef KV_CONFIG_MAX_KEY_LEN
#define KV_MAX_KEY_LEN 256
#else
#define KV_MAX_KEY_LEN KV_CONFIG_MAX_KEY_LEN
#endif

#ifndef KV_CONFIG_MAX_VAL_LEN
#define KV_MAX_VAL_LEN 8192
#else
#define KV_MAX_VAL_LEN KV_CONFIG_MAX_VAL_LEN
#endif

#endif  /* KV_CONF_H */

