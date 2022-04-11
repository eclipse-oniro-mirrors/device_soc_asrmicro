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

#ifndef __LITEOS_PATCH_H__
#define __LITEOS_PATCH_H__

#include "cmsis_os2.h"
#include "kal.h"
#include "los_sem.h"

UINT32 LOS_SemPendingNoTask(UINT32 semHandle);
osStatus_t osSemaphorePendingNoTask(osSemaphoreId_t semaphore_id);

#endif // __LITEOS_PATCH_H__