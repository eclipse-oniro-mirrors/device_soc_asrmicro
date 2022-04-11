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

#ifndef __DUET_UART_H
#define __DUET_UART_H

#include <stdint.h>
#include "stdbool.h"
#include <errno.h>
#include "duet_cm4.h"
#include "system_cm4.h"
#include "duet_uart_index.h"
#include "duet.h"

#ifdef __cplusplus
extern "C" {
#endif

#define DUET_UART0_INDEX    0
#define DUET_UART1_INDEX    1
#define DUET_UART2_INDEX    2
#define DUET_UART_NUM       3

/* UART BAUDRATE */
#define UART_BAUDRATE_110                       (110)
#define UART_BAUDRATE_300                       (300)
#define UART_BAUDRATE_600                       (600)
#define UART_BAUDRATE_1200                      (1200)
#define UART_BAUDRATE_2400                      (2400)
#define UART_BAUDRATE_4800                      (4800)
#define UART_BAUDRATE_9600                      (9600)
#define UART_BAUDRATE_14400                     (14400)
#define UART_BAUDRATE_19200                     (19200)
#define UART_BAUDRATE_38400                     (38400)
#define UART_BAUDRATE_57600                     (57600)
#define UART_BAUDRATE_115200                    (115200)
#define UART_BAUDRATE_230400                    (230400)
#define UART_BAUDRATE_460800                    (460800)
#define UART_BAUDRATE_921600                    (921600)

/* fifo depth */
#define UART_TX_FIFO_DEPTH               (32)
#define UART_RX_FIFO_DEPTH               (32)

/* UART flags */
#define UART_FLAG_TX_FIFO_EMPTY          (1<<7)
#define UART_FLAG_RX_FIFO_FULL           (1<<6)
#define UART_FLAG_TX_FIFO_FULL           (1<<5)
#define UART_FLAG_RX_FIFO_EMPTY          (1<<4)
#define UART_FLAG_BUSY                   (1<<3)

/* UART interrutps */
#define UART_RX_INTERRUPT                (1<<4)    /* tx interrupt is only set when tx fifo level transisions through a level,  not based on fifo level itself */
#define UART_TX_INTERRUPT                (1<<5)
#define UART_RX_TIMEOUT_INTERRUPT        (1<<6)
#define UART_ERROR_FRAME_INTERRUPT                 (1<<7)
#define UART_ERROR_PARITY_INTERRUPT                (1<<8)
#define UART_ERROR_BREAK_INTERRUPT                 (1<<9)
#define UART_ERROR_OVERRUN_INTERRUPT               (1<<10)
#define UART_DISABLE_ALL_IRQ             (0x00)

#define UART_DMA_TX_EN                   (1<<1)
#define UART_DMA_RX_EN                   (1<<0)

#define UART_IRDA_ENABLE                 (1<<1)
#define UART_IRDA_LP_EN                  (1<<2)
#define UART_CLK                         (52000000)

typedef void (*duet_uart_callback_func)(char);
extern duet_uart_callback_func g_duet_uart_callback_handler[UART_NUM];
/*
 * UART data width
 */
typedef enum {
    DATA_5BIT,
    DATA_6BIT,
    DATA_7BIT,
    DATA_8BIT
} uart_data_width_t;

/*
 * UART stop bits
 */
typedef enum {
    STOP_1BIT,
    STOP_2BITS
} uart_stop_bits_t;

/*
 * UART flow control
 */
typedef enum {
    FLOW_CTRL_DISABLED,
    FLOW_CTRL_RTS,
    FLOW_CTRL_CTS,
    FLOW_CTRL_CTS_RTS
} uart_flow_control_t;

/*
 * UART parity
 */
typedef enum {
    PARITY_NO,
    PARITY_ODD,
    PARITY_EVEN
} uart_parity_t;

/*
 * UART mode
 */
typedef enum {
    TX_MODE = 1,
    RX_MODE,
    TX_RX_MODE
} uart_mode_t;

/*
 * UART fifo_threshold
 * TX/RX FIFO Level, both FIFO of depth 16
*/
typedef enum {
    FIFO_1_8_FULL = 0, // 1/8
    FIFO_1_4_FULL = 1, // 1/4
    FIFO_HALF_FULL = 2, // 1/2
    FIFO_3_4_FULL = 3, // 3/4
    FIFO_7_8_FULL = 4,  // 7/8
    FIFO_NULL
} uart_fifo_threshold_t;

/*
 * UART configuration
 */
typedef struct {
    uint32_t            baud_rate;
    uart_data_width_t   data_width;
    uart_parity_t       parity;
    uart_stop_bits_t    stop_bits;
    uart_flow_control_t flow_control;
    uart_mode_t         mode;

} duet_uart_config_t;

typedef struct {
    uint8_t       port;    /* uart port */
    duet_uart_config_t config;  /* uart config */
    void         *priv;    /* priv data */
} duet_uart_dev_t;

UART_TypeDef *getUartxViaIdx(uint8_t uart_idx);

void duet_uart_struct_init(duet_uart_dev_t *UART_InitStruct);
int32_t duet_uart_dma_config(duet_uart_dev_t *uart, uint8_t dma_tx_rx_sel, uint8_t new_state);
/**
 * Initialises a UART interface
 *
 *
 * @param[in]  uart  the interface which should be initialised
 *
 * @return  0 : on success, EIO : if an error occurred with any step
 */
int32_t duet_uart_init(duet_uart_dev_t *uart);

/**
 * Transmit data on a UART interface
 *
 * @param[in]  uart  the UART interface
 * @param[in]  data  pointer to the start of data
 * @param[in]  size  number of bytes to transmit
 *
 * @return  0 : on success, EIO : if an error occurred with any step
 */
int32_t duet_uart_send(duet_uart_dev_t *uart, const void *data, uint32_t size, uint32_t timeout);

/**
 * Deinitialises a UART interface
 *
 * @param[in]  uart  the interface which should be deinitialised
 *
 * @return  0 : on success, EIO : if an error occurred with any step
 */
int32_t duet_uart_finalize(duet_uart_dev_t *uart);

void duet_uart_start(UART_TypeDef *UARTx);

void duet_uart_stop(UART_TypeDef *UARTx);

void duet_uart_set_callback(uint8_t uart_idx, duet_uart_callback_func func);

/**
 * calculate integer divider & fractional divider
 *
 */
uint32_t duet_uart_calc_baud(uint32_t baud);

void duet_uart_interrupt_config(UART_TypeDef *UARTx, uint32_t uart_int, bool new_state);
uint8_t duet_uart_clear_interrupt(UART_TypeDef *UARTx, uint32_t uart_interrupt);
ITstatus duet_uart_get_raw_interrupt_status(UART_TypeDef *UARTx, uint32_t uart_interrupt);
ITstatus duet_uart_get_interrupt_status(UART_TypeDef *UARTx, uint32_t uart_interrupt);
ITstatus duet_uart_get_flag_status(UART_TypeDef *UARTx, uint8_t uart_flag);

void UART_SendData(UART_TypeDef *UARTx, unsigned char Data);
uint8_t UART_ReceiveData(UART_TypeDef *UARTx);

#ifdef __cplusplus
}
#endif
#endif // __DUET_UART_H
