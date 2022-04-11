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

#ifdef _UART_LITE_EN_
#include "uart_lite.h"
#else
#include <stdbool.h>
#include "duet_cm4.h"
#include "duet_uart.h"
#include "duet_pinmux.h"
#include "printf_uart.h"

uint8_t g_printf_uart = UART_NUM;
void printf_uart_register(uint8_t uart_idx)
{
#if 0
    uint32_t uart_clk_reg = *(volatile uint32_t *)(UART_CLK_REG_ADDR);

    if (uart_idx == UART0_INDEX) {
        if ((uart_clk_reg & UART0_SCLK_EN) == 0) {
            goto ERR;
        }
    } else if (uart_idx == UART1_INDEX) {
        if ((uart_clk_reg & UART1_SCLK_EN) == 0) {
            goto ERR;
        }
    } else if (uart_idx == UART2_INDEX) {
        if ((uart_clk_reg & UART2_SCLK_EN) == 0) {
            goto ERR;
        }
    } else {
        g_printf_uart = uart_idx;
        return;
    }
#endif
    g_printf_uart = uart_idx;
    // printf("printf use uart %d\n",uart_idx);
    return;
#if 0
ERR:
    g_printf_uart = UART_NUM;
#endif
}
#endif
extern bool lega_log_is_enable(void);
void uart_put_char(char character)
{
#ifndef _UART_LITE_EN_
    if (lega_log_is_enable()) {
        UART_SendData(getUartxViaIdx(g_printf_uart), (unsigned char)character);
    }
#else
    UART_SendData(UART_INDEX, (unsigned char)(character));
#endif
}

void printf_uart_log_init(uint8_t uart_idx)
{
    duet_uart_dev_t uart_config_struct  = {0};
    uart_config_struct.port = uart_idx;
    switch (uart_idx) {
        case UART0_INDEX:
            duet_pinmux_config(PAD0, PF_UART0);
            duet_pinmux_config(PAD1, PF_UART0);
            break;
        case UART1_INDEX:
            duet_pinmux_config(PAD2, PF_UART1);
            duet_pinmux_config(PAD3, PF_UART1);
            break;
        case UART2_INDEX:
            duet_pinmux_config(PAD12, PF_UART2);
            duet_pinmux_config(PAD13, PF_UART2);
            break;
        default:
            break;
    }

    duet_uart_struct_init(&uart_config_struct);
    duet_uart_init(&uart_config_struct);
    //    duet_uart_start(getUartxViaIdx(uart_idx));
    printf_uart_register(uart_idx);
    printf("set uart%d as printf log uart...\n", uart_idx);
}
#ifdef PRINTF2_SUPPORT
uint8_t g_printf2_uart = UART_NUM;
void printf2_uart_register(uint8_t uart_idx)
{
    g_printf2_uart = uart_idx;
}
void uart_put_char_2(char character)
{
    UART_SendData(getUartxViaIdx(g_printf2_uart), (unsigned char)character);
}
#endif