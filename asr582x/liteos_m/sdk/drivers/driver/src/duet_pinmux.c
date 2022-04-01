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

#include "duet_pinmux.h"

Pad_Config_State duet_pinmux_config(Pad_Num_Type pad_num, Pad_Func_Type pad_func)
{
    uint32_t pad_reg_addr = PINMUX_REG_BASE + 4 * (pad_num >> 3); // pinmux register address of pad pad_num
    uint32_t reg_offset = (pad_num % 8) * 4; // offset from pad_reg_addr in bits
    Pad_Config_State config_state = Config_Success;
    uint32_t pad_func_value = 0;
    switch (pad_num) {
        case PAD0:
            switch (pad_func) {
                case PF_GPIO0:
                    pad_func_value = 0;
                    break;
                case PF_UART0:
                    pad_func_value = 1;
                    break;
                case PF_SWD:
                    pad_func_value = 2;
                    break;
                case PF_SPI1:
                    pad_func_value = 3;
                    break;
                case PF_PWM5:
                    pad_func_value = 4;
                    break;
                default:
                    config_state = Config_Fail;
                    break;
            }
            break;
        case PAD1:
            switch (pad_func) {
                case PF_GPIO1:
                    pad_func_value = 0;
                    break;
                case PF_UART0:
                    pad_func_value = 1;
                    break;
                case PF_SWD:
                    pad_func_value = 2;
                    break;
                case PF_SPI1:
                    pad_func_value = 3;
                    break;
                case PF_PWM7:
                    pad_func_value = 4;
                    break;
                default:
                    config_state = Config_Fail;
                    break;
            }
            break;
        case PAD2:
            switch (pad_func) {
                case PF_GPIO2:
                    pad_func_value = 0;
                    break;
                case PF_UART1:
                    pad_func_value = 1;
                    break;
                case PF_SPI1:
                    pad_func_value = 3;
                    break;
                case PF_I2C0:
                    pad_func_value = 4;
                    break;
                default:
                    config_state = Config_Fail;
                    break;
            }
            break;
        case PAD3:
            switch (pad_func) {
                case PF_GPIO3:
                    pad_func_value = 0;
                    break;
                case PF_UART1:
                    pad_func_value = 1;
                    break;
                case PF_SPI1:
                    pad_func_value = 3;
                    break;
                case PF_I2C0:
                    pad_func_value = 4;
                    break;
                default:
                    config_state = Config_Fail;
                    break;
            }
            break;
        case PAD4:
            switch (pad_func) {
                case PF_SWD:
                    pad_func_value = 0;
                    break;
                case PF_GPIO4:
                    pad_func_value = 1;
                    break;
                case PF_SDIO0:
                    pad_func_value = 2;
                    break;
                case PF_UART0:
                    pad_func_value = 3;
                    break;
                case PF_PWM0:
                    pad_func_value = 4;
                    break;
                case PF_PSRAM:
                    pad_func_value = 7;
                    break;
                default:
                    config_state = Config_Fail;
                    break;
            }
            break;
        case PAD5:
            switch (pad_func) {
                case PF_SWD:
                    pad_func_value = 0;
                    break;
                case PF_GPIO5:
                    pad_func_value = 1;
                    break;
                case PF_SDIO0:
                    pad_func_value = 2;
                    break;
                case PF_UART0:
                    pad_func_value = 3;
                    break;
                case PF_PWM2:
                    pad_func_value = 4;
                    break;
                case PF_PSRAM:
                    pad_func_value = 7;
                    break;
                default:
                    config_state = Config_Fail;
                    break;
            }
            break;
        case PAD6:
            switch (pad_func) {
                case PF_GPIO6:
                    pad_func_value = 0;
                    break;
                case PF_SPI0:
                    pad_func_value = 1;
                    break;
                case PF_SDIO0:
                    pad_func_value = 2;
                    break;
                case PF_UART0:
                    pad_func_value = 3;
                    break;
                case PF_PWM4:
                    pad_func_value = 4;
                    break;
                case PF_PSRAM:
                    pad_func_value = 7;
                    break;
                default:
                    config_state = Config_Fail;
                    break;
            }
            break;
        case PAD7:
            switch (pad_func) {
                case PF_GPIO7:
                    pad_func_value = 0;
                    break;
                case PF_SPI0:
                    pad_func_value = 1;
                    break;
                case PF_SDIO0:
                    pad_func_value = 2;
                    break;
                case PF_UART0:
                    pad_func_value = 3;
                    break;
                case PF_PWM6:
                    pad_func_value = 4;
                    break;
                case PF_I2S:
                    pad_func_value = 6;
                    break;
                case PF_PSRAM:
                    pad_func_value = 7;
                    break;
                default:
                    config_state = Config_Fail;
                    break;
            }
            break;
        case PAD8:
            switch (pad_func) {
                case PF_GPIO8:
                    pad_func_value = 0;
                    break;
                case PF_SPI0:
                    pad_func_value = 1;
                    break;
                case PF_SDIO0:
                    pad_func_value = 2;
                    break;
                case PF_I2C1:
                    pad_func_value = 3;
                    break;
                case PF_UART1:
                    pad_func_value = 4;
                    break;
                case PF_I2S:
                    pad_func_value = 6;
                    break;
                case PF_PSRAM:
                    pad_func_value = 7;
                    break;
                default:
                    config_state = Config_Fail;
                    break;
            }
            break;
        case PAD9:
            switch (pad_func) {
                case PF_GPIO9:
                    pad_func_value = 0;
                    break;
                case PF_SPI0:
                    pad_func_value = 1;
                    break;
                case PF_SDIO0:
                    pad_func_value = 2;
                    break;
                case PF_I2C1:
                    pad_func_value = 3;
                    break;
                case PF_UART1:
                    pad_func_value = 4;
                    break;
                case PF_I2S:
                    pad_func_value = 6;
                    break;
                case PF_PSRAM:
                    pad_func_value = 7;
                    break;
                default:
                    config_state = Config_Fail;
                    break;
            }
            break;
        case PAD10:
            switch (pad_func) {

                case PF_PWM1:
                    pad_func_value = 1;
                    break;
                case PF_GPIO10:
                    pad_func_value = 2;
                    break;
                case PF_UART2:
                    pad_func_value = 3;
                    break;
                case PF_SPI2:
                    pad_func_value = 4;
                    break;
                case PF_I2S:
                    pad_func_value = 6;
                    break;
                default:
                    config_state = Config_Fail;
                    break;
            }
            break;
        case PAD11:
            switch (pad_func) {
                case PF_GPIO11:
                    pad_func_value = 0;
                    break;
                case PF_PWM3:
                    pad_func_value = 1;
                    break;
                case PF_SDIO0:
                    pad_func_value = 2;
                    break;
                case PF_UART2:
                    pad_func_value = 3;
                    break;
                case PF_SPI2:
                    pad_func_value = 4;
                    break;
                case PF_I2S:
                    pad_func_value = 6;
                    break;
                default:
                    config_state = Config_Fail;
                    break;
            }
            break;
        case PAD12:
            switch (pad_func) {
                case PF_GPIO12:
                    pad_func_value = 0;
                    break;
                case PF_SPI2:
                    pad_func_value = 2;
                    break;
                case PF_UART2:
                    pad_func_value = 3;
                    break;
                case PF_I2S:
                    pad_func_value = 6;
                    break;
                default:
                    config_state = Config_Fail;
                    break;
            }
            break;
        case PAD13:
            switch (pad_func) {
                case PF_GPIO13:
                    pad_func_value = 0;
                    break;
                case PF_SPI2:
                    pad_func_value = 2;
                    break;
                case PF_UART2:
                    pad_func_value = 3;
                    break;
                default:
                    config_state = Config_Fail;
                    break;
            }
            break;
        case PAD14:
            switch (pad_func) {
                case PF_PWM0:
                    pad_func_value = 1;
                    break;
                case PF_SPI2:
                    pad_func_value = 2;
                    break;
                case PF_UART1:
                    pad_func_value = 3;
                    break;
                case PF_GPIO14:
                    pad_func_value = 4;
                    break;
                default:
                    config_state = Config_Fail;
                    break;
            }
            break;
        case PAD15:
            switch (pad_func) {
                case PF_PWM2:
                    pad_func_value = 1;
                    break;
                case PF_SPI2:
                    pad_func_value = 2;
                    break;
                case PF_UART1:
                    pad_func_value = 3;
                    break;
                case PF_GPIO15:
                    pad_func_value = 4;
                    break;
                default:
                    config_state = Config_Fail;
                    break;
            }
            break;
        case PAD16:
            switch (pad_func) {
                case PF_GPIO16:
                    pad_func_value = 0;
                    break;
                case PF_UART0:
                    pad_func_value = 1;
                    break;
                case PF_SDIO0:
                    pad_func_value = 2;
                    break;
                case PF_SPI1:
                    pad_func_value = 3;
                    break;
                case PF_PSRAM:
                    pad_func_value = 6;
                    break;
                default:
                    config_state = Config_Fail;
                    break;
            }
            break;
        case PAD17:
            switch (pad_func) {
                case PF_GPIO17:
                    pad_func_value = 0;
                    break;
                case PF_UART0:
                    pad_func_value = 1;
                    break;
                case PF_SDIO0:
                    pad_func_value = 2;
                    break;
                case PF_SPI1:
                    pad_func_value = 3;
                    break;
                case PF_PSRAM:
                    pad_func_value = 6;
                    break;
                default:
                    config_state = Config_Fail;
                    break;
            }
            break;
        case PAD18:
            switch (pad_func) {
                case PF_GPIO18:
                    pad_func_value = 0;
                    break;
                case PF_UART1:
                    pad_func_value = 1;
                    break;
                case PF_SDIO0:
                    pad_func_value = 2;
                    break;
                case PF_SPI1:
                    pad_func_value = 3;
                    break;
                case PF_PSRAM:
                    pad_func_value = 6;
                    break;
                default:
                    config_state = Config_Fail;
                    break;
            }
            break;
        case PAD19:
            switch (pad_func) {
                case PF_GPIO19:
                    pad_func_value = 0;
                    break;
                case PF_UART1:
                    pad_func_value = 1;
                    break;
                case PF_SDIO0:
                    pad_func_value = 2;
                    break;
                case PF_SPI1:
                    pad_func_value = 3;
                    break;
                case PF_PSRAM:
                    pad_func_value = 6;
                    break;
                default:
                    config_state = Config_Fail;
                    break;
            }
            break;
        case PAD20:
            switch (pad_func) {
                case PF_GPIO20:
                    pad_func_value = 0;
                    break;
                case PF_I2C0:
                    pad_func_value = 1;
                    break;
                case PF_SDIO0:
                    pad_func_value = 2;
                    break;
                case PF_UART0:
                    pad_func_value = 3;
                    break;
                case PF_PSRAM:
                    pad_func_value = 6;
                    break;
                default:
                    config_state = Config_Fail;
                    break;
            }
            break;
        case PAD21:
            switch (pad_func) {
                case PF_GPIO21:
                    pad_func_value = 0;
                    break;
                case PF_I2C0:
                    pad_func_value = 1;
                    break;
                case PF_SDIO0:
                    pad_func_value = 2;
                    break;
                case PF_UART0:
                    pad_func_value = 3;
                    break;
                case PF_PSRAM:
                    pad_func_value = 6;
                    break;
                default:
                    config_state = Config_Fail;
                    break;
            }
            break;
        case PAD22:
            switch (pad_func) {
                case PF_GPIO22:
                    pad_func_value = 0;
                    break;
                case PF_I2C1:
                    pad_func_value = 1;
                    break;
                case PF_SDIO0:
                    pad_func_value = 2;
                    break;
                case PF_UART2:
                    pad_func_value = 3;
                    break;
                case PF_PSRAM:
                    pad_func_value = 6;
                    break;
                default:
                    config_state = Config_Fail;
                    break;
            }
            break;
        case PAD23:
            switch (pad_func) {
                case PF_GPIO23:
                    pad_func_value = 0;
                    break;
                case PF_I2C1:
                    pad_func_value = 1;
                    break;
                case PF_UART2:
                    pad_func_value = 3;
                    break;
                default:
                    config_state = Config_Fail;
                    break;
            }
            break;
        default:
            config_state = Config_Fail;
            break;
    }

    *(volatile uint32_t *)(pad_reg_addr) &= ~(0xf << reg_offset); // mask
    *(volatile uint32_t *)(pad_reg_addr) |= (pad_func_value << reg_offset); // set pinmux value
    return config_state;
}

void duet_pad_config(Pad_Num_Type pad_num, Pad_Pull_Type pull_type)
{
    /* assert params */
    // to-do
    switch (pull_type) {
        case PULL_UP:
            *(volatile uint32_t *)(HW_CTRL_PE_PS) &= ~(0x1 << (pad_num));
            *(volatile uint32_t *)(PAD_PE_REG) |= (0x1 << (pad_num));
            *(volatile uint32_t *)(PAD_PS_REG) |= (0x1 << (pad_num));
            break;
        case PULL_DOWN:
            *(volatile uint32_t *)(HW_CTRL_PE_PS) &= ~(0x1 << (pad_num));
            *(volatile uint32_t *)(PAD_PE_REG) |= (0x1 << (pad_num));
            *(volatile uint32_t *)(PAD_PS_REG) &= ~(0x1 << (pad_num));
            break;
        case PULL_NONE:
            *(volatile uint32_t *)(HW_CTRL_PE_PS) &= ~(0x1 << (pad_num));
            *(volatile uint32_t *)(PAD_PE_REG) &= ~(0x1 << (pad_num));
            break;
        default:
            break;
    }
}

