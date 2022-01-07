#ifndef __BOARD_H__
#define __BOARD_H__

typedef enum {
    PORT_UART_STD,
    PORT_UART_AT,
    PORT_UART_RS485,
    PORT_UART_SCANNER,
    PORT_UART_LORA,
    PORT_UART_TEMP,
    PORT_UART_SIZE,
    PORT_UART_INVALID = 255
}PORT_UART_TYPE;

void board_init(void);
void board_before_init(void);
int board_after_init(void);
void board_uart_init(void);

#endif //__BOARD_H__