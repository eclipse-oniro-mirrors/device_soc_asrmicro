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

#include <stdint.h>
#ifdef ALIOS_SUPPORT
#include "lega_rhino.h"
#else
#include "lega_rtos.h"
#endif
#include "duet_pinmux.h"
#include "duet_uart.h"
#include "uart.h"
#include "board.h"

#define HAL_UART_BUF_QUEUE_BYTES 256
lega_queue_t hal_uart_buf_queue[UART_NUM];
char * hal_uart_buf_queue_name[UART_NUM] =
{
    "uart0_buffer_queue",
    "uart1_buffer_queue",
    "uart2_buffer_queue"
};

void hal_uart0_callback_handler(char ch)
{
    lega_rtos_push_to_queue(&hal_uart_buf_queue[UART0_INDEX], &ch, LEGA_NEVER_TIMEOUT);
}

void hal_uart1_callback_handler(char ch)
{
    lega_rtos_push_to_queue(&hal_uart_buf_queue[UART1_INDEX], &ch, LEGA_NEVER_TIMEOUT);
}

void hal_uart2_callback_handler(char ch)
{
    lega_rtos_push_to_queue(&hal_uart_buf_queue[UART2_INDEX], &ch, LEGA_NEVER_TIMEOUT);
}

void set_uart_pinmux(uint8_t uart_idx,uint8_t hw_flow_control)
{
    switch(uart_idx){
        case UART0_INDEX:
            duet_pinmux_config(PAD0,PF_UART0);
            duet_pinmux_config(PAD1,PF_UART0);
            if(hw_flow_control){
                duet_pinmux_config(PAD6,PF_UART0);
                duet_pinmux_config(PAD7,PF_UART0);
            }
            break;
        case UART1_INDEX:
            duet_pinmux_config(PAD2,PF_UART1);
            duet_pinmux_config(PAD3,PF_UART1);
            if(hw_flow_control){
                duet_pinmux_config(PAD14,PF_UART1);
                duet_pinmux_config(PAD15,PF_UART1);
            }
            break;
        case UART2_INDEX:
            duet_pinmux_config(PAD12,PF_UART2);
            duet_pinmux_config(PAD13,PF_UART2);
            if(hw_flow_control){
                duet_pinmux_config(PAD10,PF_UART2);
                duet_pinmux_config(PAD11,PF_UART2);
            }
            break;
        default:
            break;
    }
}
/**
 * Initialises a UART interface
 *
 *
 * @param[in]  uart  the interface which should be initialised
 *
 * @return  0 : on success, EIO : if an error occurred with any step
 */
int32_t hal_uart_init(uart_dev_t *uart)
{
    if(!uart) return -1;
    if(uart->port == PORT_UART_STD){
        uart->port = UART1_INDEX;
       // uart->priv = (void *)(hal_uart1_callback_handler);
    }
    else if(uart->port == PORT_UART_TEMP){
        uart->port = UART0_INDEX;
    }
    set_uart_pinmux(uart->port,uart->config.flow_control);
    duet_uart_init((duet_uart_dev_t *)uart);
   // lega_rtos_init_queue(&hal_uart_buf_queue[uart->port], hal_uart_buf_queue_name[uart->port], sizeof(char), HAL_UART_BUF_QUEUE_BYTES);
    return 0;
}
int32_t hal_uart_send(uart_dev_t *uart, const void *data, uint32_t size, uint32_t timeout)
{
    uint32_t i=0;
    if(NULL == uart){
        return -1;
    }
    /*logic port to physical port*/
    if(uart->port == PORT_UART_STD){
        uart->port = UART1_INDEX;
        uart->priv = (void *)(hal_uart1_callback_handler);
    }
    else if(uart->port == PORT_UART_TEMP){
        uart->port = UART0_INDEX;
    }

    return duet_uart_send((duet_uart_dev_t *)uart, data, size, timeout);
}

/**
 * Receive data on a UART interface
 *
 * @param[in]   uart         the UART interface
 * @param[out]  data         pointer to the buffer which will store incoming data
 * @param[in]   expect_size  number of bytes to receive
 * @param[in]   timeout      timeout in milisecond, set this value to HAL_WAIT_FOREVER
 *                           if you want to wait forever
 *
 * @return  0 : on success, EIO : if an error occurred with any step
 */
int32_t hal_uart_recv(uart_dev_t *uart, void *data, uint32_t expect_size, uint32_t timeout)
{
    int i = 0;
    int32_t ret;
    uint8_t *pdata = (uint8_t *)data;

    if((uart == NULL) || (data == NULL))
    {
        return -1;
    }

    /*logic port to physical port*/
    if(uart->port == PORT_UART_STD){
        uart->port = UART1_INDEX;
        uart->priv = (void *)(hal_uart1_callback_handler);
    }
    else if(uart->port == PORT_UART_TEMP){
        uart->port = UART0_INDEX;
    }

    for (i = 0; i < expect_size; i++)
    {
        ret = lega_rtos_pop_from_queue(&hal_uart_buf_queue[uart->port], &pdata[i], timeout);
        if(ret)
        {
            return -1;
        }
    }

    return 0;
}

int32_t UartGetc()
{
    UART_TypeDef* UARTx = getUartxViaIdx(UART1_INDEX);
    /* wait till rx fifo is not empty */
   if( duet_uart_get_flag_status(UARTx, UART_FLAG_RX_FIFO_EMPTY) == SET)
     return 0;
   else
     return UARTx->DR;
}

/**
 * Receive data on a UART interface
 *
 * @param[in]   uart         the UART interface
 * @param[out]  data         pointer to the buffer which will store incoming data
 * @param[in]   expect_size  number of bytes to receive
 * @param[out]  recv_size    number of bytes received
 * @param[in]   timeout      timeout in milisecond, set this value to HAL_WAIT_FOREVER
 *                           if you want to wait forever
 *
 * @return  0 : on success, EIO : if an error occurred with any step
 */
int32_t hal_uart_recv_II(uart_dev_t *uart, void *data, uint32_t expect_size,
                      uint32_t *recv_size, uint32_t timeout)
{
    uint8_t *pdata = (uint8_t *)data;
    int i = 0;
    uint32_t rx_count = 0;
    int32_t ret;

    if((uart == NULL) || (data == NULL))
    {
        return -1;
    }

    /*logic port to physical port*/
    if(uart->port == PORT_UART_STD){
        uart->port = UART1_INDEX;
        uart->priv = (void *)(hal_uart1_callback_handler);
    }
    else if(uart->port == PORT_UART_TEMP){
        uart->port = UART0_INDEX;
    }

    for (i = 0; i < expect_size; i++)
    {
        ret = lega_rtos_pop_from_queue(&hal_uart_buf_queue[uart->port], &pdata[i], timeout);

        if(!ret)
        {
            rx_count++;
        }
        else
        {
            *recv_size = rx_count;
            return ret;
        }
    }
    if(recv_size)
    {
        *recv_size = rx_count;
    }

    if(rx_count != 0)
    {
        return 0;
    }
    else
    {
        return -1;
    }
}

/**
 * Deinitialises a UART interface
 *
 * @param[in]  uart  the interface which should be deinitialised
 *
 * @return  0 : on success, EIO : if an error occurred with any step
 */
int32_t hal_uart_finalize(uart_dev_t *uart)
{
    int32_t ret;

    if(NULL == uart){
        return -1;
    }

    /*logic port to physical port*/
    if(uart->port == PORT_UART_STD){
        uart->port = UART1_INDEX;
        uart->priv = (void *)(hal_uart1_callback_handler);
    }
    else if(uart->port == PORT_UART_TEMP){
        uart->port = UART0_INDEX;
    }

    ret = lega_rtos_deinit_queue(&hal_uart_buf_queue[uart->port]);
    if(!ret)
    {
        duet_uart_finalize((duet_uart_dev_t *)uart);
    }
    else
    {
        return ret;
    }
}