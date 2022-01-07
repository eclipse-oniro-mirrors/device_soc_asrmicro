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