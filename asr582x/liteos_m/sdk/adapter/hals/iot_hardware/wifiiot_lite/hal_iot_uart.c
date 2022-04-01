/*
 * Copyright (c) 2020 Huawei Device Co., Ltd.
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

#include "iot_errno.h"
#include "iot_uart.h"
#include "duet_uart.h"
#include "lega_rtos.h"

static duet_uart_dev_t uart_config_struct[UART_NUM];
#define HAL_UART_BYTES 256
lega_queue_t hal_uart_queue[UART_NUM];
char *hal_uart_queue_name[UART_NUM] = {
    "uart0_buffer_queue",
    "uart1_buffer_queue",
    "uart2_buffer_queue"
};

void hal_uart0_cb(char ch)
{
    lega_rtos_push_to_queue(&hal_uart_queue[UART0_INDEX], &ch, LEGA_NEVER_TIMEOUT);
}

void hal_uart1_cb(char ch)
{
    lega_rtos_push_to_queue(&hal_uart_queue[UART1_INDEX], &ch, LEGA_NEVER_TIMEOUT);
}

void hal_uart2_cb(char ch)
{
    lega_rtos_push_to_queue(&hal_uart_queue[UART2_INDEX], &ch, LEGA_NEVER_TIMEOUT);
}

extern set_uart_pinmux(uint8_t uart_idx, uint8_t hw_flow_control);

unsigned int IoTUartInit(unsigned int id, const IotUartAttribute *param)
{
    if (id >= UART_NUM) {
        return IOT_FAILURE;
    }

    if (NULL == param) {
        return IOT_FAILURE;
    }

    uart_config_struct[id].port = id;
    if (id == 0) {
        uart_config_struct[id].priv = (void *)(hal_uart0_cb);
    } else if (id == 1) {
        uart_config_struct[id].priv = (void *)(hal_uart1_cb);
    } else {
        uart_config_struct[id].priv = (void *)(hal_uart2_cb);
    }
    uart_config_struct[id].config.baud_rate = param->baudRate;
    uart_config_struct[id].config.data_width = param->dataBits;
    uart_config_struct[id].config.parity = param->parity;
    uart_config_struct[id].config.stop_bits = param->stopBits;

    set_uart_pinmux(uart_config_struct[id].port, uart_config_struct[id].config.flow_control);
    lega_rtos_init_queue(&hal_uart_queue[id], hal_uart_queue_name[id], sizeof(char), HAL_UART_BYTES);

    return duet_uart_init(&uart_config_struct[id]);
}

int IoTUartRead(unsigned int id, unsigned char *data, unsigned int dataLen)
{
    int i = 0;
    uint8_t *pdata = (uint8_t *)data;
    int32_t ret;

    if (id >= UART_NUM) {
        return IOT_FAILURE;
    }

    for (i = 0; i < dataLen; i++) {
        ret = lega_rtos_pop_from_queue(&hal_uart_queue[id], &pdata[i], LEGA_WAIT_FOREVER);
        if (ret) {
            return IOT_FAILURE;
        }
    }

    return IOT_SUCCESS;
}

int IoTUartWrite(unsigned int id, const unsigned char *data, unsigned int dataLen)
{
    if (id >= UART_NUM) {
        return IOT_FAILURE;
    }

    return duet_uart_send(&uart_config_struct[id], data, dataLen, 0);
}

unsigned int IoTUartDeinit(unsigned int id)
{
    if (id >= UART_NUM) {
        return IOT_FAILURE;
    }

    return duet_uart_finalize(&uart_config_struct[id]);
}

unsigned int IoTUartSetFlowCtrl(unsigned int id, IotFlowCtrl flowCtrl)
{
    if (id >= UART_NUM) {
        return IOT_FAILURE;
    }

    if (flowCtrl == IOT_FLOW_CTRL_NONE) {
        uart_config_struct[id].config.flow_control = FLOW_CTRL_DISABLED;
    } else if (flowCtrl == IOT_FLOW_CTRL_RTS_CTS) {
        uart_config_struct[id].config.flow_control = FLOW_CTRL_CTS_RTS;
    } else if (flowCtrl == IOT_FLOW_CTRL_RTS_ONLY) {
        uart_config_struct[id].config.flow_control = FLOW_CTRL_RTS;
    } else if (flowCtrl == IOT_FLOW_CTRL_CTS_ONLY) {
        uart_config_struct[id].config.flow_control = FLOW_CTRL_CTS;
    }

    return IOT_SUCCESS;
}
