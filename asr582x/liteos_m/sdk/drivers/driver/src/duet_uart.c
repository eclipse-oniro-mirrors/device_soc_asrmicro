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

#include "duet_uart.h"
#include <stdbool.h>
#include "duet_dma.h"

#define DUET_UART0_USE_DMA 1
#if DUET_UART0_USE_DMA
#define DMA_MAX_SIZE 256
volatile uint8_t uart_dma_complete_flag = 1;
static uint8_t uart_dma_tx_mem[DMA_MAX_SIZE] = {0};
void uart0_dma_callback(uint32_t i)
{
    uart_dma_complete_flag = 1;
}
#endif
duet_uart_callback_func g_duet_uart_callback_handler[UART_NUM] = {0};

UART_TypeDef *getUartxViaIdx(uint8_t uart_idx)
{
    switch (uart_idx) {
        case UART0_INDEX:
            return UART0;
        case UART1_INDEX:
            return UART1;
        case UART2_INDEX:
            return UART2;
        default:
            return NULL;
    }
}

void duet_uart_struct_init(duet_uart_dev_t *UART_InitStruct)
{
    UART_InitStruct->config.baud_rate = UART_BAUDRATE_115200;
    UART_InitStruct->config.data_width = DATA_8BIT;
    UART_InitStruct->config.parity = PARITY_NO;
    UART_InitStruct->config.stop_bits = STOP_1BIT;
    UART_InitStruct->config.flow_control = FLOW_CTRL_DISABLED;
    UART_InitStruct->config.mode = TX_RX_MODE;
    UART_InitStruct->priv = NULL;
}

uint32_t duet_uart_calc_baud(uint32_t baud)
{
    uint32_t int_div;
    uint32_t fac_div;
    uint32_t remainder;
    uint32_t temp;
    uint32_t uart_clk = UART_CLK;

    temp = 16 * baud;
    if ((0 == baud) || uart_clk < temp) {
        return 0;
    }
    int_div = (uint32_t)(uart_clk / temp);
    remainder = uart_clk % temp;
    temp = 8 * remainder / baud;
    fac_div = (temp >> 1) + (temp & 1);

    temp = ((int_div << 16) | (fac_div & 0xFFFF));
    return temp;
}

/* get uart flag status */
ITstatus duet_uart_get_flag_status(UART_TypeDef *UARTx, uint8_t uart_flag)
{
    if (UARTx->FR & uart_flag) {
        return SET;
    } else {
        return RESET;
    }
}

void UART_SendData(UART_TypeDef *UARTx, unsigned char Data)
{
    if (UARTx == NULL) {
        return;
    }
    /* wait till tx fifo is not full */
    while (duet_uart_get_flag_status(UARTx, UART_FLAG_TX_FIFO_FULL) == SET);
    UARTx->DR = Data;
}

uint8_t UART_ReceiveData(UART_TypeDef *UARTx)
{
    /* wait till rx fifo is not empty */
    while (duet_uart_get_flag_status(UARTx, UART_FLAG_RX_FIFO_EMPTY) == SET);
    return UARTx->DR;
}

void duet_uart_interrupt_config(UART_TypeDef *UARTx, uint32_t uart_int, bool new_state)
{
    if (new_state == DISABLE) {
        UARTx->IMSC &= ~(uart_int);
    } else {
        UARTx->IMSC |= (uart_int);
    }
}

ITstatus duet_uart_get_interrupt_status(UART_TypeDef *UARTx, uint32_t uart_interrupt)
{
    if (UARTx->MIS & uart_interrupt) {
        return SET;
    } else {
        return RESET;
    }
}

ITstatus duet_uart_get_raw_interrupt_status(UART_TypeDef *UARTx, uint32_t uart_interrupt)
{
    if (UARTx->RIS & uart_interrupt) {
        return SET;
    } else {
        return RESET;
    }
}

uint8_t duet_uart_clear_interrupt(UART_TypeDef *UARTx, uint32_t uart_interrupt)
{
    UARTx->ICR |= uart_interrupt;
    return 0;
}

int32_t duet_uart_dma_config(duet_uart_dev_t *uart, uint8_t dma_tx_rx_sel, uint8_t new_state)
{
    UART_TypeDef *UARTx = NULL;
    if (DUET_UART0_INDEX == uart->port) {
        UARTx = UART0;
    } else if (DUET_UART1_INDEX == uart->port) {
        UARTx = UART1;
    } else if (DUET_UART2_INDEX == uart->port) {
        UARTx = UART2;
    } else {
        return EIO;
    }
    if (new_state == ENABLE) {
        UARTx->DMACR |= dma_tx_rx_sel;
    } else {
        UARTx->DMACR &= ~dma_tx_rx_sel;
    }
    return 0;
}

/**
 * Initialises a UART interface
 *
 *
 * @param[in]  uart  the interface which should be initialised
 *
 * @return  0 : on success, EIO : if an error occurred with any step
 */
int32_t duet_uart_init(duet_uart_dev_t *uart)
{
    // uart sclk enable and sclk root clk setting (XTAL)
    uint32_t tmp_value;
    UART_TypeDef *UARTx = NULL;
    if (DUET_UART0_INDEX == uart->port) {
        UARTx = UART0;
    } else if (DUET_UART1_INDEX == uart->port) {
        UARTx = UART1;
    } else if (DUET_UART2_INDEX == uart->port) {
        UARTx = UART2;
    } else {
        return EIO;
    }
    // enable uart clk
    if (UARTx == UART0) {
        tmp_value = REG_RD(PERI_CLK_EN_REG1) & (~(UART0_BUS_CLK_EN | UART0_PERI_CLK_EN));
        REG_WR(PERI_CLK_EN_REG1, (tmp_value | (UART0_BUS_CLK_EN | UART0_PERI_CLK_EN)));
    } else if (UARTx == UART1) {
        tmp_value = REG_RD(PERI_CLK_EN_REG1) & (~(UART1_BUS_CLK_EN | UART1_PERI_CLK_EN));
        REG_WR(PERI_CLK_EN_REG1, (tmp_value | (UART1_BUS_CLK_EN | UART1_PERI_CLK_EN)));
    } else {
        tmp_value = REG_RD(PERI_CLK_EN_REG1) & (~(UART2_BUS_CLK_EN | UART2_PERI_CLK_EN));
        REG_WR(PERI_CLK_EN_REG1, (tmp_value | (UART2_BUS_CLK_EN | UART2_PERI_CLK_EN)));
    }

    // fpga no effect, soc need

    // wait for the end of current charater
    while (duet_uart_get_flag_status(UARTx, UART_FLAG_BUSY));

    // disable UART
    UARTx->CR &= ~1;

    // flush fifo by setting FEN = 0
    UARTx->LCR_H &= ~(1 << 4);

    // set baudrate
    uint32_t br_div = duet_uart_calc_baud(uart->config.baud_rate);
    UARTx->IBRD = br_div >> 16;  /* baudrate divdier register must be placed before a LCR_H write */
    UARTx->FBRD = br_div & 0x3f;

    // set LCR_H
    UARTx->LCR_H |= (uart->config.data_width << 5) | (uart->config.stop_bits << 3);

    switch (uart->config.parity) {
        case PARITY_ODD:
            UARTx->LCR_H &= ~(3 << 1);
            UARTx->LCR_H |= (1 << 1);
            break;
        case PARITY_EVEN:
            UARTx->LCR_H |= (3 << 1);
            break;
        case PARITY_NO:
            UARTx->LCR_H &= ~(1 << 1);
            break;
        default:
            break;
    }
    if (uart->config.mode == 0) {
        uart->config.mode = TX_RX_MODE;
    }
    // set CR
    UARTx->CR &= ~(3 << 8); // set tx/rx mode to 0
    UARTx->CR |= (uart->config.flow_control << 14) | (uart->config.mode << 8);
    // enable fifo
    UARTx->LCR_H |= (ENABLE << 4);
    UARTx->IFLS &= ~(0x7);
    UARTx->IFLS |= FIFO_HALF_FULL;  // tx fifo threshold
    UARTx->IFLS &= ~(0x7 << 3);
    UARTx->IFLS |= (FIFO_HALF_FULL << 3); // rx fifo threshold

    if (uart->priv) {
        // enable rx interrupt
        UARTx->IMSC |= (UART_RX_TIMEOUT_INTERRUPT | UART_RX_INTERRUPT);
        // enable cm4 interrupt
        if (UARTx == UART0) {
            tmp_value = REG_RD(DUTE_IRQ_EN_REG) & (~UART0_IRQ_BIT);
            REG_WR(DUTE_IRQ_EN_REG, (tmp_value | (UART0_IRQ_BIT)));
            NVIC_EnableIRQ(UART0_IRQn);
#if DUET_UART0_USE_DMA
            duet_dma_init();
            duet_uart_dma_config(uart, UART_DMA_TX_EN, ENABLE);
            duet_dma_callback_register(DMA_CH_UART0_TX, uart0_dma_callback);
            uart_dma_complete_flag = 1;
#endif
        } else if (UARTx == UART1) {
            tmp_value = REG_RD(DUTE_IRQ_EN_REG) & (~UART1_IRQ_BIT);
            REG_WR(DUTE_IRQ_EN_REG, (tmp_value | (UART1_IRQ_BIT)));
            NVIC_EnableIRQ(UART1_IRQn);
        } else {
            tmp_value = REG_RD(DUTE_IRQ_EN_REG) & (~UART2_IRQ_BIT);
            REG_WR(DUTE_IRQ_EN_REG, (tmp_value | (UART2_IRQ_BIT)));
            NVIC_EnableIRQ(UART2_IRQn);
        }
        g_duet_uart_callback_handler[uart->port] = (duet_uart_callback_func)(uart->priv);
    }
    UARTx->CR |= 0x1;
    return 0;
}

void duet_uart_start(UART_TypeDef *UARTx)
{

    UARTx->CR |= 0x1;

    if (UART0 == UARTx) {
        NVIC_EnableIRQ(UART0_IRQn);
    } else if (UART1 == UARTx) {
        NVIC_EnableIRQ(UART1_IRQn);
    }
    if (UART2 == UARTx) {
        NVIC_EnableIRQ(UART2_IRQn);
    }
}

void duet_uart_stop(UART_TypeDef *UARTx)
{
    UARTx->CR &= ~0x1;

    if (UART0 == UARTx) {
        NVIC_DisableIRQ(UART0_IRQn);
    } else if (UART1 == UARTx) {
        NVIC_DisableIRQ(UART1_IRQn);
    }
    if (UART2 == UARTx) {
        NVIC_DisableIRQ(UART2_IRQn);
    }
}

/**
 * Transmit data on a UART interface
 *
 * @param[in]  uart  the UART interface
 * @param[in]  data  pointer to the start of data
 * @param[in]  size  number of bytes to transmit
 *
 * @return  0 : on success, EIO : if an error occurred with any step
 */
extern bool lega_log_is_enable(void);
int32_t duet_uart_send(duet_uart_dev_t *uart, const void *data, uint32_t size, uint32_t timeout)
{
    UART_TypeDef *UARTx;
    int i = 0;

    if ((NULL == uart) || (NULL == data)) {
        return EIO;
    }

    if (DUET_UART0_INDEX == uart->port) {
        UARTx = UART0;
#if DUET_UART0_USE_DMA
        while (size > DMA_MAX_SIZE) {
            while (!uart_dma_complete_flag);
            uart_dma_complete_flag = 0;
            duet_dma_uart_tx(uart->port, data, DMA_MAX_SIZE);
            size -= DMA_MAX_SIZE;
            data += DMA_MAX_SIZE;
        }
        while (!uart_dma_complete_flag);
        uart_dma_complete_flag = 0;
        memcpy(uart_dma_tx_mem, data, size);
        duet_dma_uart_tx(uart->port, uart_dma_tx_mem, size);
        return 0;
#endif
    } else if (DUET_UART1_INDEX == uart->port) {
        if (!lega_log_is_enable()) {
            return 0;
        }
        UARTx = UART1;
    } else if (DUET_UART2_INDEX == uart->port) {
        UARTx = UART2;
    } else {
        return EIO;
    }

    for (i = 0; i < size; i++) {
        UART_SendData(UARTx, ((uint8_t *)data)[i]);
    }

    return 0;
}

/**
 * Deinitialises a UART interface
 *
 * @param[in]  uart  the interface which should be deinitialised
 *
 * @return  0 : on success, EIO : if an error occurred with any step
 */
int32_t duet_uart_finalize(duet_uart_dev_t *uart)
{
    UART_TypeDef *UARTx;
    unsigned int tmp_value;

    if (NULL == uart) {
        return EIO;
    }

    if (DUET_UART0_INDEX == uart->port) {
        UARTx = UART0;
    } else if (DUET_UART1_INDEX == uart->port) {
        UARTx = UART1;
    } else if (DUET_UART2_INDEX == uart->port) {
        UARTx = UART2;
    } else {
        return EIO;
    }

    // disable all uart interrupt
    UARTx->IMSC  = UART_DISABLE_ALL_IRQ;
    // disable all uart config
    UARTx->LCR_H = 0;
    UARTx->CR = 0;

    // disable cm4 interrupt
    if (UART0 == UARTx) {
        tmp_value = REG_RD(DUTE_IRQ_DIS_REG) & (~UART0_IRQ_BIT);
        REG_WR(DUTE_IRQ_DIS_REG, (tmp_value | (UART0_IRQ_BIT)));
        NVIC_DisableIRQ(UART0_IRQn);
    } else if (UART1 == UARTx) {
        tmp_value = REG_RD(DUTE_IRQ_DIS_REG) & (~UART1_IRQ_BIT);
        REG_WR(DUTE_IRQ_DIS_REG, (tmp_value | (UART1_IRQ_BIT)));
        NVIC_DisableIRQ(UART1_IRQn);
    } else {
        tmp_value = REG_RD(DUTE_IRQ_DIS_REG) & (~UART2_IRQ_BIT);
        REG_WR(DUTE_IRQ_DIS_REG, (tmp_value | (UART2_IRQ_BIT)));
        NVIC_DisableIRQ(UART2_IRQn);
    }

    // uart sclk disable, fpga no effect, soc need
    if (UART0 == UARTx) {
        tmp_value = REG_RD(PERI_CLK_DIS_REG1) & (~(UART0_BUS_CLK_EN | UART0_PERI_CLK_EN));
        REG_WR(PERI_CLK_DIS_REG1, (tmp_value | (UART0_BUS_CLK_EN | UART0_PERI_CLK_EN)));
#if DUET_UART0_USE_DMA
        uart_dma_complete_flag = 1;
#endif
    } else if (UART1 == UARTx) {
        tmp_value = REG_RD(PERI_CLK_DIS_REG1) & (~(UART0_BUS_CLK_EN | UART0_PERI_CLK_EN));
        REG_WR(PERI_CLK_DIS_REG1, (tmp_value | (UART0_BUS_CLK_EN | UART0_PERI_CLK_EN)));
    } else {
        tmp_value = REG_RD(PERI_CLK_DIS_REG1) & (~(UART0_BUS_CLK_EN | UART0_PERI_CLK_EN));
        REG_WR(PERI_CLK_DIS_REG1, (tmp_value | (UART0_BUS_CLK_EN | UART0_PERI_CLK_EN)));
    }

    g_duet_uart_callback_handler[uart->port] = NULL;
    return 0;
}

static void UARTX_IRQHandler(uint8_t uart_idx)
{
    char tmp;
    UART_TypeDef *UARTx = getUartxViaIdx(uart_idx);
    if (duet_uart_get_interrupt_status(UARTx, UART_RX_INTERRUPT)
         ||  duet_uart_get_interrupt_status(UARTx, UART_RX_TIMEOUT_INTERRUPT)) {
        duet_uart_interrupt_config(UARTx, UART_RX_INTERRUPT | UART_RX_TIMEOUT_INTERRUPT, DISABLE);
        duet_uart_clear_interrupt(UARTx, UART_RX_INTERRUPT | UART_RX_TIMEOUT_INTERRUPT);

        /* read rx fifo till it's empty */
        while (! duet_uart_get_flag_status(UARTx, UART_FLAG_RX_FIFO_EMPTY)) {
            tmp = (char)(UARTx->DR); // uart_receive_data(UART);
            if (g_duet_uart_callback_handler[uart_idx] != NULL) {
                g_duet_uart_callback_handler[uart_idx](tmp);
            }
        }

        duet_uart_interrupt_config(UARTx, UART_RX_INTERRUPT | UART_RX_TIMEOUT_INTERRUPT, ENABLE);
    }
}

#if (LOSCFG_USE_SHELL == 1)
#include "los_event.h"
#include "target_config.h"
extern EVENT_CB_S g_shellInputEvent;
static void UARTX_Shell_IRQHandler(uint8_t uart_idx)
{
    // char tmp;
    UART_TypeDef *UARTx = getUartxViaIdx(uart_idx);
    if (duet_uart_get_interrupt_status(UARTx, UART_RX_INTERRUPT)
         ||  duet_uart_get_interrupt_status(UARTx, UART_RX_TIMEOUT_INTERRUPT)) {
        duet_uart_interrupt_config(UARTx, UART_RX_INTERRUPT | UART_RX_TIMEOUT_INTERRUPT, DISABLE);
        duet_uart_clear_interrupt(UARTx, UART_RX_INTERRUPT | UART_RX_TIMEOUT_INTERRUPT);

        if (uart_idx == UART1_INDEX) {
            (void)LOS_EventWrite(&g_shellInputEvent, 0x1);
        }

        duet_uart_interrupt_config(UARTx, UART_RX_INTERRUPT | UART_RX_TIMEOUT_INTERRUPT, ENABLE);
    }
}
#endif

void UART0_IRQHandler(void)
{
    duet_intrpt_enter();
    UARTX_IRQHandler(0);
    duet_intrpt_exit();
}

void UART1_IRQHandler(void)
{
    duet_intrpt_enter();
#if (LOSCFG_USE_SHELL == 1)
    UARTX_Shell_IRQHandler(1);
#else
    UARTX_IRQHandler(1);
#endif
    duet_intrpt_exit();
}

void UART2_IRQHandler(void)
{
    duet_intrpt_enter();
    UARTX_IRQHandler(2);
    duet_intrpt_exit();
}

void duet_uart_set_callback(uint8_t uart_idx, duet_uart_callback_func func)
{
    g_duet_uart_callback_handler[uart_idx] = func;

}