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

#ifndef __LEGA_RTOS_PORT_H__
#define __LEGA_RTOS_PORT_H__


#include "los_arch_context.h"
// typedef unsigned int           UINTPTR;
#define lega_rtos_declare_critical()  UINTPTR uvIntSave
/** @brief Enter a critical session, all interrupts are disabled
  *
  * @return    none
  */
#define lega_rtos_enter_critical() do{uvIntSave = LOS_IntLock();}while(0)
//void lega_rtos_enter_critical( void );

/** @brief Exit a critical session, all interrupts are enabled
  *
  * @return    none
  */
#define lega_rtos_exit_critical() do{LOS_IntRestore(uvIntSave);}while(0)
//void lega_rtos_exit_critical( void );

#endif //__LEGA_RTOS_PORT_H__