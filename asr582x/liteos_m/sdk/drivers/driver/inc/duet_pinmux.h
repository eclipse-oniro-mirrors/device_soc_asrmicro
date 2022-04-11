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

#ifndef __DUET_PINMUX_H
#define __DUET_PINMUX_H

#include "duet.h"

#ifdef __cplusplus
extern "c" {
#endif

    typedef enum {
        PAD0,
        PAD1,
        PAD2,
        PAD3,
        PAD4,
        PAD5,
        PAD6,
        PAD7,
        PAD8,
        PAD9,
        PAD10,
        PAD11,
        PAD12,
        PAD13,
        PAD14,
        PAD15,
        PAD16,
        PAD17,
        PAD18,
        PAD19,
        PAD20,
        PAD21,
        PAD22,
        PAD23,
        PAD24,
        PAD25,
        PAD26,
        PAD27,
        PAD28,
        PAD29,
        PAD30
    } Pad_Num_Type;

    typedef enum {
        PF_GPIO0, PF_GPIO1, PF_GPIO2, PF_GPIO3, PF_GPIO4, PF_GPIO5,
        PF_GPIO6, PF_GPIO7, PF_GPIO8, PF_GPIO9, PF_GPIO10, PF_GPIO11,
        PF_GPIO12, PF_GPIO13, PF_GPIO14, PF_GPIO15, PF_GPIO16, PF_GPIO17,
        PF_GPIO18, PF_GPIO19, PF_GPIO20, PF_GPIO21, PF_GPIO22, PF_GPIO23,
        PF_SWD,
        PF_UART0, PF_UART1, PF_UART2, PF_UART3,
        PF_SPI0,  PF_SPI1,  PF_SPI2,
        PF_PWM0,  PF_PWM1,  PF_PWM2,  PF_PWM3,  PF_PWM4, PF_PWM5, PF_PWM6, PF_PWM7,
        PF_I2C0,  PF_I2C1,
        PF_SDIO0,
        PF_I2S,
        PF_PSRAM
    } Pad_Func_Type;

    typedef enum {
        Config_Success,
        Config_Fail
    } Pad_Config_State;

    typedef enum {
        PULL_DEFAULT = 0x00,
        PULL_UP,
        PULL_DOWN,
        PULL_NONE
    } Pad_Pull_Type;

    typedef enum {
        DS1DS0_00 = 0x00,
        DS1DS0_01,
        DS1DS0_10,
        DS1DS0_11
    } Pad_DS_Type;

    Pad_Config_State duet_pinmux_config(Pad_Num_Type pad_num, Pad_Func_Type pad_func);
    void duet_pad_config(Pad_Num_Type pad_num, Pad_Pull_Type pull_type);

#ifdef __cplusplus
}
#endif

#endif /* __DUET_PINMUX_H */