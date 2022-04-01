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

#ifndef __PRINTF_UART_H__
#define __PRINTF_UART_H__

#include "duet_uart.h"
extern uint8_t g_printf_uart;
/**
 * Initialize printf uart port
 *
 *@param[in]  uart port for printf
 */
void printf_uart_register(uint8_t uart_idx);

void uart_put_char(char character);

void printf_uart_log_init(uint8_t uart_idx);
#ifdef PRINTF2_SUPPORT
extern uint8_t g_printf2_uart;
void uart_put_char_2(char character);
void printf2_uart_register(uint8_t uart_idx);
#endif
#endif