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

#ifndef __SYSTEM_CM4_H
#define __SYSTEM_CM4_H

#include "duet_cm4.h"

#ifdef __cplusplus
extern "C" {
#endif

extern void SystemInit(void);
extern void SystemCoreClockUpdate(void);
extern void duet_system_reset(void);

#ifdef __cplusplus
}
#endif

#endif
